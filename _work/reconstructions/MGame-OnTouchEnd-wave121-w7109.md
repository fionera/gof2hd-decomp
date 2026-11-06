# MGame::OnTouchEnd / MGame::OnUpdate (wave 121, w7109)

Status: analysis + one landed source tweak, 2026-11-06. Worktree
`/home/debian/GalaxyOnFire2/wt/w7109`, owned TU `src/game/menu/MGame.cpp`.
Scratch: `/dev/shm/w7109*` (not committed).

Functions:

| function | before | after | size (orig/ours) | byte |
|---|---|---|---|---|
| `MGame::OnTouchEnd` (`_ZN5MGame10OnTouchEndEiiPv`) | 81.6 % | **81.7 %** | 8080 / 8080 | no |
| `MGame::OnUpdate` (`_ZN5MGame8OnUpdateEv`) | 99.9 % | 99.9 % (unchanged) | 11440 / 11440 | no |

Landed change (OnTouchEnd, `finishMission` block):

```cpp
- if (this->player->goingToWaypoint() == true) {
+ if (this->player->goingToWaypoint()) {
      this->player->setAutoPilot(nullptr);
  }
```

Evidence: the original at `0x177c` emits `blx PlayerEgo::goingToWaypoint; cbz r0, ...`
(direct bool test). Ours emitted `blx ...; cmp r0,#1; bne ...` because of the `== true`
idiom. Removing it reproduces the original's `cbz` exactly at that site and is the only
size-preserving improvement found (8080 B stays 8080 B, so the rest of `.text` does not
shift and `byte_exact` is unchanged at 4505).

Gate after the change (full `check`): byte 4505, linked 4505, data 843/843, imports 207,
lint CLEAN, RATCHET PASS.

## Why the rest of OnTouchEnd does not fall into place

The residual is dominated by **cold-block placement**, not by wrong code. The hot
instruction stream already matches almost instruction-for-instruction; the 274 difflib
regions are 90 % branch-target-only differences caused by blocks sitting at different
offsets. The real (non-layout) differences are:

1. **Mining-plant refine counter** (`0x486`). Original:
   `cmp.w r9,#0; beq` (`amount != 0` shape). Ours: `cmp.w r9,#-1; bgt`
   (`count > 0` where `count = -amount`). The original keeps the *negative* counter in
   r9 (`sub.w r9,r9,#1`, `rsb r1,r9,#0` for `String(-amount)`); ours does too, but the
   comparison is `-amount > 0`. Every source spelling that produces `cmp #0; beq`
   (`amount != 0`, `count != 0`, `if (amount)`, `if (amount < 0)`, `unsigned`, `short`,
   `long`) makes clang 7.0.2 canonicalise the counter to a *positive* induction variable
   (`add` instead of `sub`, counter in r8, fee in r9) and grows the function to
   8084–8088 B. `long count` keeps r9/`sub` but still emits `cmp #-1; bgt`.
2. **`refinedAny` type** (`0x528`). Original tests bit 0 (`lsls r0,r0,#31` → `bool`);
   ours tests bit 7 (`lsls r0,r0,#24` → `char`). `bool refinedAny = false;` is the
   semantically right fix, but on its own it changes the cold-block layout and the size
   to 8084 B (score 78.5).
3. **Mission-won block order** (`0xc84`). The original computes `keepObjectives`
   first (`0xc9c`: `getCampaignMission()->hasWon() ? !getMission()->isCampaignMission()
   : false`), then selects `mission` (`0xe1a`), then checks `isInstantActionMission`
   (`0xe3a`) before `isCampaignMission` (`0xe62`). Ours computes `mission` first, then
   `keepObjectives`. Moving the `keepObjectives` line before the `mission` line (and
   dropping the `campaign` temporary, since the original re-calls
   `getCampaignMission()` twice) reproduces the original block exactly, but the function
   grows to 8088 B (82.5 %). Swapping the `isInstantActionMission`/`isCampaignMission`
   order on top of that gives 82.6 % at 8084 B.
4. **`menuWindow->OnTouchEnd(...) == true`** (`0xd88`). Original `cbz`, ours
   `cmp #1; bne`. Removing the idiom is correct but changes the size to 8084 B.
5. **`initHudMenu` tail-merge placement.** The original shares the
   `initHudMenu(1)`/`initHudMenu(2)` call (`0xc7c` sets r1=1 and branches to the shared
   `blx` at `0xe9a`; the `0x400` case at `0xe94` sets r1=2 and falls through). Ours
   shares it too but places the shared call at `0xc88`, i.e. *before* the mission-won
   block instead of after it. This is a `MachineBlockPlacement` difference and is the
   single largest address shift in the function (`+0x50` at `0xe16`).

**Size is a hard constraint.** `OnTouchEnd` sits in the middle of `.text`; any size
change shifts `.text`/`.got`, changes every PC-relative GOT delta after it, and drops
`byte_exact` from 4505 to 3105 (measured with the 8084 B `m5h+g2` variant). So the
8084/8088 B structural fixes above cannot be landed even though they score higher on the
fuzzy metric. The 8080 B variants tried (base 81.6; `keepObjectives` first 81.4;
`long count` 81.6; switch-case reorder 81.6; both `goingToWaypoint` idioms 80.5;
`menuWindow` idiom 79.1) all score at or below the landed 81.7.

## .rel.plt residual (3 JUMP_SLOTs)

`readelf -r` at `0x213a9c..0x213aa4`:

```
orig: MenuTouchWindow::inCinematicMode, MGame::setCinematicMode, Hud::resetAnalogStick
ours: Hud::resetAnalogStick, MenuTouchWindow::inCinematicMode, MGame::setCinematicMode
```

The GOT entries are created in first-call-site order. In the original the cutsceneActive
tail (`0x179e`: `frameTime = 0; hud->resetAnalogStick(); hud->releaseAllKeys(); return;`)
is placed *after* the menuTouchOpen `inCinematicMode`/`setCinematicMode` blocks
(`0x16d2..0x1726`); in ours the tail is at `0x1718`, before them (`0x177c`). This is the
same cold-block-placement difference as (5) above, so the `.rel.plt` swap will only
disappear once the cutsceneActive/menuTouchOpen block order matches.

## MGame::OnUpdate (99.9 %, no source win)

Only residual: two `mov r0,r5` where the original reloads `ldr.w r0,[r9]`.

```
orig 1992: ldr.w r0,[r9]        ours 1992: ldr.w r5,[r9]
     1993: blx getCurrentCampaignMission   1993: mov r0,r5
     1994: ...                               1994: blx ...
...
orig 2005: ldr.w r0,[r9]        ours 2006: mov r0,r5
     2006: blx getPlayingTime              2007: blx getPlayingTime
```

Cause: the mission-23 idle-contact block has `Status *st = Globals::status;` and uses
`st->getPlayingTime()`. Clang CSEs the first `Globals::status` load into r5 (the local
`st`) and reuses it, while the original reloads `Globals::status` from `[r9]` at every
use (i.e. the original source had no `st` temporary). Removing the temporary is
semantically right and reproduces the `ldr.w r0,[r9]` shape, but clang then rematerialises
the *address* of `Globals::status` in r5 instead of r9 (`ldr r0,[r5,#0]`, 16-bit) and the
function shrinks 11440 → 11392 B (score 87.2). That size change shifts `.text` and drops
`byte_exact` to 3105, so it cannot be landed either.

Variants tried (all keep the 99.9 % residual or lose the size):
- no `st` (direct `Globals::status->getPlayingTime()`): 87.2 %, 11392 B
- `st` used only for the field access: 92.5 %, 11436 B
- `st` used for both playing time and field: 92.7 %, 11436 B
- `Status *&st = Globals::status;` / `Status **st = &Globals::status;`: 87.2 %, 11392 B
- comma-assignment `(st = Globals::status, st->...)`: 87.2 %, 11392 B
- `Status &st = *Globals::status;`: 99.9 %, 11440 B (identical to base)

The base (with `st`) is a local optimum: it is the only formulation that keeps the
11440 B size *and* the original's r9-based cold-block layout. A 100 % match needs the
compiler to reload `[r9]` for the `getPlayingTime` call while still choosing r9 as the
address base — no source formulation found that does both.
