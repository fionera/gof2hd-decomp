# LevelScript::process — wave 191b w13820 (statement-order pass)

Status: **code change landed, 2026-11-13**. Branch `wave191b-w13820`, base master
`6b680e716`. Target `_ZN11LevelScript7processEi` (`LevelScript::process`),
original `0x137d18`, **57232 B (0xdf90)**, size-exact.

This pass is a set of **size-neutral source-statement reorders** in the
high-numbered mission cases (157/158) and the mission state machine, each
restoring the original disassembly's statement order.  Every edit is a pure
statement move: no anchor, no `volatile`, no call added/removed, no padding.
Size stays exactly `0xdf90`; `.rel.plt` stays byte-identical; every gate count
is unchanged (the ratchet tracks `byte_exact`, which is 4508 before and after).

## Result (cumulative stack, all 6 edits)

```
                         before            after
fn_diff raw              53005             53061
fn_diff aligned          19538             19362     (-176)
size (orig/ours)         57232/57232       57232/57232
.text diffbytes          76743             76799
.ARM.extab diffbytes     625               624        (-1)
byte_exact               4508              4508
linked_exact             4508              4508
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN             CLEAN
drift                    0                 0
RATCHET                  PASS              PASS
```

The `aligned` metric (non-matching instructions after normalising branch
targets to target instruction index) is the progress indicator; `raw` is the
same-offset byte diff, which rises when a reorder shifts branch offsets.  The
gate (`byte_exact`) is unchanged.  Precedent: w13810 landed raw +14 / aligned
-82; w13814 raw +1 / aligned -4.

## Per-edit deltas (measured cumulatively, each size-exact)

| # | edit | raw | aligned | .text | .ARM.extab |
|---|---|---:|---:|---:|---:|
| 0 | pristine baseline | 53005 | 19538 | 76743 | 625 |
| 1 | flash-block reorder (case 157 state 10) | 53010 | 19432 | 76748 | 625 |
| 2 | + mission-9 explosion-block reorder | 53016 | 19424 | 76754 | 624 |
| 3 | + radio-14 block reorder | 53029 | 19411 | 76767 | 624 |
| 4 | + mission-10 rumble reorder | 53031 | 19401 | 76769 | 624 |
| 5 | + radio-15 `m_pGeometry1` reorder | 53037 | 19390 | 76775 | 624 |
| 6 | + state-11 `m_pGeometry4` reorder | 53061 | 19362 | 76799 | 624 |

Per-edit deltas: varA raw +5 / aligned -106; mission-9 +6/-8 (extab -1);
radio-14 +13/-13; mission-10 +2/-10; radio-15 +6/-11; state-11 +24/-28.

## The edits

Original disassembly order confirmed in each block:

1. **case 157 state 10, flash block** (`0x14527a..0x1452e0`): original order is
   `m_nFlashCountdown = 0; setPosition; setDirection; setVisible(true);
   SetAnimationState(2)`.  Baseline had `SetAnimationState; m_nFlashCountdown=0;
   setVisible; setPosition; setDirection`.
2. **mission-9 explosion block** (`0x1451c6..0x145250`): original is
   `new Explosion(0); addFireStreaks; setScaling(5.0f);
   start(getPosition+{-2000,1000,-8000},{0,0,0}); play(2244);
   m_nScriptTimerA=0; m_nFlashCountdown=300`.
3. **radio-14 block** (`0x14538a..0x14545e`): original is
   `m_tempVec=getPosition; m_pCamera->setPosition(m_tempVec+{50000,0,70000});
   m_nScriptTimerA=0; m_nScriptTimerB=0; fighter->setVisible(false);
   player->setVisible(false); m_pGeometry3->setPosition; setDirection;
   setVisible(true); m_pGeometry2->setVisible(false); SetAnimationState(1);
   m_nState=10`.
4. **mission-10 rumble block** (`0x145520..0x145598`): original is
   `setScaling(10.0f); start(...); play(2243); setRumblePercentage(100,30)`.
5. **radio-15 / `m_pGeometry1` block** (`0x1455e6..0x14567c`): original is
   `setPosition; setDirection; SetVisible(false); SetAnimationState(1);
   m_nScriptTimerA=0; m_nState++`.
6. **state-11 `m_pGeometry4` block** (`0x14574e..0x1457ce`): original is
   `m_pGeometry4->setPosition; setDirection(getLightDirection(),{0,1,0});
   m_pCamera->setTarget(m_pGeometry4); m_nScriptTimerA=0;
   fighter->setVisible(false); fighter->setActive(false); m_nState++`.

## Open question (not resolved this pass)

The source's `m_pRadar->lockedEnemy = nullptr;` in the radio-15 block has **no
counterpart** in the original disassembly (`0x1455e6..0x14567c`).  Removing it
shrinks the function to 57,224 (size-fatal), so the store must live elsewhere;
its true location is unresolved.  It was left in place.

## Non-wins tried this pass (reverted, size-fatal or neutral)

- mission-142 `getGasClouds()` repeated-call shape (drop the `Level*` cache):
  size 57,240 (+8) — size-fatal.
- wormhole friend/enemy order swap: raw +1, aligned unchanged.
- mission-158 state-3 revive order (`setActive(true)` before `setPosition`):
  raw +1, aligned unchanged.
- mission-157 `m_nScriptTimerA >= 6001` expect removal: size 57,220 (-12) — size-fatal.

## Harvest rebase (master adc4f1c53)

Applied on top of the landed w13790/w13812/w13814 LevelScript state: the combined measurement is raw **52,997 -> 53,053** (+56), aligned **19,531 -> 19,355** (-176), size 57,232/57,232 exact; `.text` 76,052 -> 76,108, `.ARM.extab` 596 -> 595. The raw rise is the branch-offset shift from the statement reorders.
