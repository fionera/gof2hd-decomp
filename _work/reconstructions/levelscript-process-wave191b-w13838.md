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

## Harvest measurement (2026-11-13, on master 1b46e619d)

On the current tree the state-12 expect polarity + state-0 dispatch-tail expect
change measures raw **53,083 -> 53,206** (+123), aligned **19,162 -> 19,148**
(-14), `.text` **75,882 -> 76,005** (+123), `.ARM.extab` **552 -> 551** (-1).
The aligned/extab gain is real but the +123 `.text` is a net byte regression, so
the code change was reverted and kept as this note; re-land it if a later shape
removes the branch-offset growth.
