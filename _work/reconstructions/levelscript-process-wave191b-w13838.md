# LevelScript::process — wave 192 w13838 (state-12 expect + dispatch-tail expect)

Status: **code change landed, 2026-11-13**. Branch `wave192-w13838`, base master
`1e92c4705` (w13820 landed), prior w13838 commit `fe2d1c7fc`.
Target `_ZN11LevelScript7processEi` (`LevelScript::process`), original `0x137d18`,
**57232 B (0xdf90)**, size-exact.

## Result (landed)

```
                         before (fe2d1c7fc)   after
fn_diff raw              53077                53200     (+123)
fn_diff aligned          19228                19214     (-14)
size (orig/ours)         57232/57232          57232/57232
.text diffbytes          76132                76255     (+123)
.ARM.extab diffbytes     595                  594       (-1)
.rel.plt                 byte-identical       byte-identical
byte_exact               4508                 4508
linked_exact             4508                 4508
size-exact               4524/4524            4524/4524
data_exact               843/843              843/843
imports                  207                  207
lint                     CLEAN                CLEAN
drift                    0                    0
RATCHET                  PASS                 PASS
```

The `aligned` metric (non-matching instructions after normalising branch targets
to target instruction index) is the progress indicator; `raw` is the same-offset
byte diff, which rises when a hint changes block layout and shifts branch
offsets.  Precedent: w13810 landed raw +14 / aligned -82; w13820 raw +56 /
aligned -176; w13838 (fe2d1c7fc) raw +24 / aligned -127.

## The landed edits

Both are **faithful control-flow-hint restorations** read off the original
disassembly; no anchor, no `volatile`, no call added/removed, no padding.

1. **state-12 `>= 5500` explosion-update: drop the expect.**  Original
   `0x145abc`: `movw r2,#5500; subs; sbcs; blt.n 145ad8` skips the update when
   the timer is *below* 5500, i.e. the true branch is the fall-through
   (expected-true layout).  The source had `__builtin_expect(..., 0)`
   (expected-false), which is the wrong polarity.  Replaced with a plain
   `if (m_nScriptTimerA >= 5500)`.
2. **shared dispatch tail, state-0 success block: add the expect.**  Original
   `0x144e36..0x144f56`: `if (distance <= 49999 && player->player->field_5e == 0)`
   ends in `beq.n 144f80`, i.e. the success block is **out of line after the
   epilogue** → expected-false.  Added
   `__builtin_expect((distance <= 49999 && player->player->field_5e == 0), 0)`.

Measured individually (on top of `fe2d1c7fc`):

| edit | raw | aligned | size | .text | extab |
|---|---:|---:|---:|---:|---:|
| edit 1 alone (drop `>=5500` expect) | 53136 | 19214 | 57224 (-8) | 353243 | 595 |
| edit 2 alone (state-0 expect 0) | 55197 | 19187 | 57236 (+4) | +32 | 1281 |
| **both (landed)** | **53200** | **19214** | **57232 ✓** | **76255** | **594** |

Edit 1 is -8 B and edit 2 is +8 B relative to the same base; they compose to
exactly 0 net size change.

## Non-landed experiments (measured, reverted)

All on top of `fe2d1c7fc` unless noted.  `E*` are the w13820/earlier-pass
edits; `S0` is the landed state-0 expect.

- `E1` mission-94 state-5 tail reorder (resetGunDelay; setLookAtCam;
  setTarget; setComputerControlled; setVulnerable; setFreeze; setVisible;
  hud.visible; radar.enabled; resetCamera; forceUpdate; lockedEnemy;
  dockNavPtr; m_bCinematicBreak; m_nState++): raw 53868 / aligned 19217 /
  size 57224 (-8).
- `E2` = landed edit 1.
- `E3` state-12 radio-15 block reorder (setVisible x2; startFade; play):
  raw 53081 / aligned 19228 / size 57232 ✓ (neutral).
- `E4` remove radio-15 `m_pRadar->lockedEnemy = nullptr;`: raw 53134 /
  aligned 19226 / size 57224 (-8).
- `E5` remove the extra `createRadioMessage(14, …)` at the end of the
  dispatch-tail fail branch (original has no radio 14 there): raw 53180 /
  aligned 19218 / size 57208 (-24).
- `E1+E4`: -16; `E2+E4`: -16; `E1+E2`: -16; `E1+E3`: -8; `E3+E4`: -8;
  `E2+E3`: +4 (57236); `E2+E3+E4`: -4 (57228, aligned 19193);
  `E1+E3+E4`: -16.
- `state_a = E1+E2+E3+E4`: raw 55167 / aligned **19187** / size **57228 (-4)**
  — best aligned seen, but size-fatal.  It restores the original's store
  count: 4 `strd rX,rX,[r0,#4]` (lockedEnemy+dockNavPtr) sites, matching the
  original's 4 (`0x13dd56`, `0x13f3b8`, `0x13f8f2`, `0x140284`).  The missing
  +4 B could not be recovered without a layout change (see below).
- `state_a + __builtin_expect(m_nScriptTimerA >= 5000 && m_nScriptTimerA - delta < 5000, 0)`:
  raw 55083 / aligned 19212 / size 57232 ✓ but **`.rel.plt` 4 B differ**
  (the PLT order of `Level::switchSkyboxForSupernovaReversal` and
  `Player::setGammaHP` swaps: original 0x213300/0x213304, ours 0x213304/0x213300)
  — gate-fatal.  The hint lays the `>=6500` block (which calls `setGammaHP`)
  out before the `>=5000` switchSkybox block; at HEAD the order is correct.
- `state_a + __builtin_expect(..., 1)` on the same condition is a no-op
  (identical to `state_a`, `.rel.plt` clean, still -4).
- `E1+S0`: raw 53924 / aligned 19217 / size 57232 ✓ / .text 76979 / extab 594.
- `E2+S0` is the landed pair (above).  `E2+S0+S3` (state-3 block reorder:
  setAutoPilot before the `!isDocked` guard, resetGunDelay after it) is
  raw 53201 / aligned 19214 / size 57232 ✓ — the reorder is faithful but
  neutral, so it was not added.
- `E3+E4+S0`: raw 53204 / aligned 19226 / size 57232 ✓.

## Open items

- **`state_a` (aligned 19187) needs exactly +4 B.**  The only +4-B hint found
  (`>=5000 && -delta<5000` expect 0) flips `.rel.plt`, so it is not landable
  as-is.  A +4-B faithful statement or a hint that keeps the `>=6500` block
  after the switchSkybox block would unlock a further -27 aligned.
- **Mission-157 radio-15 `m_pRadar->lockedEnemy = nullptr;`** still has no
  counterpart in the original radio-15 block (`0x1455e6..0x14567c`); removing
  it is -8 B alone.  In `state_a` the mission-94 tail reorder fuses the
  mission-94 lockedEnemy/dockNavPtr stores into the 4th `strd`, so the store
  count then matches the original — the remaining -4 is a register-allocation
  / hoisting difference, not a missing store.
- **Shared dispatch tail**: ours still emits the state-0 success block inline
  (original out of line at `0x144f80`), and ours has an extra
  `createRadioMessage(14, …)` in the fail branch (original has only radio 15).
  Removing radio 14 is -24 B and needs a matching +24 B.
- **Big aligned blocks** (block-order, not statement-order): mission-94
  state-1 `0x13e0e6` (orig 614 / ours 1), `0x13e962` (423), `0x13d74a` (402),
  `0x13dbd8` (354), mission-106 `0x141028` (324).  These are clang block
  layout of the switch cases, not missing source.

## Verification

```
cmake --build cmake-build-match --target check   -> RATCHET PASS
verify: byte 4508  linked 4508  stubs 0  missing 0  extra 0  avg 99.94
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0  imports: 207
```

## Second landed candidate (state_a + faithful expected-true hints)

After the first commit (`7208e08f3`, raw 53,200 / aligned 19,214 / extab 594),
a second pass reconstructed the full `state_a` combination (`E1+E2+E3+E4`,
i.e. mission-94 tail reorder + state-12 radio-15 reorder + drop the spurious
state-12 `lockedEnemy` store) and then restored the **expected-true** polarity of
three state-12 branches whose source hints were wrong or missing.

### Original branch polarity (verified from disassembly)

| branch | original | layout |
|---|---|---|
| `m_nScriptTimerA < 3000` | `0x1457d6..0x1457f6` `blt 1458b0` skips body when `>=3000` | body fall-through = expected true |
| `else if (m_nScriptTimerA < 7000)` | `0x1458b0..0x1458b8` `blt 14594e` skips body when `>=7000` | body fall-through = expected true |
| `if (m_nScriptTimerA < 6000)` | `0x14591a..0x145928` `blt 145952` skips body when `>=6000` | body fall-through = expected true |
| `if (m_nScriptTimerA >= 5700 && -delta < 5700)` | `0x145a54..0x145a6e` two `blt 145abc` skips | body fall-through = expected true |
| `if (m_nScriptTimerA >= 6500)` | `0x145b42..0x145b4c` `blt.w 144e36` skips body when `<6500` | body fall-through = expected true |

The source had `__builtin_expect(..., 0)` on the `<7000` and `<6000` branches
(wrong polarity) and no hint on `>=6500`.

### Landed edit set (on top of `7208e08f3`)

1. mission-94 tail reorder to the original `0x13f880..0x13f8fe` order
   (`resetGunDelay; setLookAtCam; setTarget; setComputerControlled;
   setVulnerable; setFreeze; setVisible; hud.visible; radar.enabled;
   resetCamera; forceUpdate; lockedEnemy; dockNavPtr; m_bCinematicBreak;
   m_nState++`).  This fuses the `lockedEnemy`/`dockNavPtr` stores into the
   original's `strd r4,r4,[r0,#4]` at `0x13f8f2` (4th `strd` site).
2. state-12 radio-15 block reorder to `0x145a70..0x145ab4`:
   `setVisible(false); setVisible(false); startFade; play`.
3. remove the state-12 `m_pRadar->lockedEnemy = nullptr;` after
   `m_nScriptTimerA = 0` (no counterpart at `0x145444..0x14545e`).
4. `<7000` and `<6000`: drop the wrong-polarity `expect(...,0)` -> plain `if`.
5. `>=6500`: add `__builtin_expect(..., 1)` (expected-true fall-through).

### Result

```
                         before (7208e08f3)   after
fn_diff raw              53200                54382     (+1182)
fn_diff aligned          19214                19106     (-108)
size (orig/ours)         57232/57232          57232/57232
.text diffbytes          76255                77437     (+1182)
.ARM.extab diffbytes     594                  594       (0)
.rel.plt                 byte-identical       byte-identical
RATCHET                  PASS                 PASS
```

The `.text`/`raw` rise is the expected same-offset byte shift from the new
block layout; the aligned (branch-target-normalised) metric is the progress
indicator and drops by 108 instructions.

### Measured variants around the candidate (all size-exact unless noted)

Base = `state_a` on `7208e08f3` (raw 55167 / aligned 19187 / size 57228, -4).
`c1` = `<3000`, `c2` = `<7000`, `c4` = `<6000`, `c7` = `>=5700`,
`c10` = `>=6500`; `e0/e1` = `expect 0/1`, `p` = plain.

| variant | raw | aligned | size |
|---|---:|---:|---:|
| all plain (`c1p+c2p+c4p+c7p+c10p`) | 53931 | **19095** | 57236 (+4) |
| **landed: `c2p+c4p+c10e1`** | **54382** | **19106** | **57232 ✓** |
| `c1p+c2p+c4p+c10e1` | 54382 | 19106 | 57232 ✓ |
| `c2p+c4p+c7p+c10e1` | 54383 | 19107 | 57232 ✓ |
| `allplain + c9e0` (planet-scaling) | 53921 | 19154 | 57232 ✓ |
| `allplain + c11e0` | 53936 | 19181 | 57232 ✓ |
| `allplain + c9e1` | 53896 | 19174 | 57232 ✓ |
| `c2e1` alone | 53918 | 19103 | 57244 (+12) |
| `c2e1+c4p+c10e1` | 54382 | 19106 | 57232 ✓ |
| `c2e1+E5` | 53932 | 19094 | 57216 (-16) |
| `allplain+E5` | 53962 | 19086 | 57208 (-24) |
| `c1e1+E5` | 53931 | 19146 | 57228 (-4) |

`c1` plain is codegen-neutral here (`c1p` == `c1e1` in the landed set).  The
all-plain set is the most faithful source but is +4 B; no faithful -4 B knob was
found.

### Spurious radio-14 (E5) — confirmed, not landed

The original fail branch `0x144f20..0x144f52` has a single
`createRadioMessage(15, race)` at `0x144f32`; our source additionally calls
`createRadioMessage(14, race)` after `m_nState = 2;`.  The extra call is
**spurious** (behaviour bug) and removing it saves 24-28 B, but every exact-size
combination found so far that includes E5 needs a +24 B filler that costs more
aligned than it saves (best exact combo without E5: aligned 19106).

### Big remaining aligned blocks (block layout, not source order)

`0x13e0e6` (orig 614 / ours 1), `0x13e962` (423), `0x13d74a` (413),
`0x13dbd8` (354) — mission-94 `state==2`/`state==3`; `0x141028` (324) —
mission-106.  These are clang block ordering of the switch cases.

## Third landed candidate (mission-94 docked branch + radio-14 placement)

On top of `81db9e2e4` (raw 54,382 / aligned 19,106), two faithful
statement-placement fixes in the mission-94 case and the shared dispatch tail.

### The edits

1. **mission-94 state-1 docked branch** (`0x13e130..0x13e158`): the original
   order is `lookBehind(); setRotationAroundTarget(true);
   setFreeLookMode(true); if (field_0xab) setTurretMode(true);` — i.e.
   `setFreeLookMode(true)` is called unconditionally in the docked path, not
   only in the `!docked` else branch.  Source now matches: `lookBehind()` moved
   before `setRotationAroundTarget`, `setFreeLookMode(true)` added to the docked
   branch and removed from the else branch.
2. **shared dispatch tail, radio 14** (`0x144f80..0x144fd2`): the original has
   `createRadioMessage(14, race)` inside the state-0 **success** block (after the
   `setAlwaysEnemy` loop, before `m_nState = 1`); the fail branch has only radio
   15.  Source had radio 14 at the end of the fail branch.  Moved it into the
   success block before `m_nState = 1`.

### Result

```
                         before (81db9e2e4)   after
fn_diff raw              54382                53990     (-392)
fn_diff aligned          19106                19096     (-10)
size (orig/ours)         57232/57232          57232/57232
.text diffbytes          77437                77045     (-392)
.ARM.extab diffbytes     594                  594       (0)
.ARM.exidx diffbytes     0                    0
.rodata diffbytes        0                    0
.rel.plt                 byte-identical       byte-identical
byte_exact               4508                 4508
linked_exact             4508                 4508
size-exact               4524/4524            4524/4524
data_exact               843/843              843/843
imports                  207                  207
lint                     CLEAN                CLEAN
drift                    0                    0
RATCHET                  PASS                 PASS
```

### Measured variants (all size-exact unless noted)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| edit 1 alone (docked free-look order) | 54004 | 19104 | 57232 |
| edit 1 + edit 2 (landed) | 53990 | 19096 | 57232 |
| mission-94 state-3 `KIPlayer *ship` cache reorder | 54093 | 19104 | 57232 |
| mission-94 state-0 `field_0xab`/`setTurretMode`/`resetCamera` reorder | 54009 | 19111 | 57232 |

Both landed edits are pure statement moves; no anchor, no `volatile`, no call
added/removed, no padding.

## Harvest measurement (2026-11-13, on master 8ad8d261b)

The mission-94 docked free-look order + radio-14 placement candidate measures
raw **53,041 -> 54,229** (+1,188), aligned **19,158 -> 19,154** (-4), but size
**57,236/57,232** — size-fatal.  Kept as a note.
