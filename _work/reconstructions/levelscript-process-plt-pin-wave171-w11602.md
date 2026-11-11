# LevelScript::process — wave 171 / w11602: case-8 duplicate removal is size-fatal

Target `_ZN11LevelScript7processEi` (orig `0x137d18`, **57,232 B = 0xdf90**),
`src/game/world/LevelScript.cpp`. Worktree `wt/w11602`, branch
`wave171-w11602`, base master `50716a763`. Scratch `/dev/shm/w11602/`.

## Baseline (HEAD, unchanged, re-measured)

```
process raw diffbytes    53,932
process aligned insns    17,900
orig / ours size         57232 / 57232   (size-exact)
insn                     20,830 / 20,722  (ours has 108 fewer instructions)
```

## What was tried and measured

1. **Removed the genuine case-8 duplicate**
   `m_pGeometry5->setPosition(player->getPosition())` (source line 815).
   The original case-8 block (`0x13d13c`..`0x13d1da`, 158 B) has **no**
   `setPosition` between `setRumblePercentage(0,0)` and the two
   `TransformGetTransform`/`SetAnimationState` calls; ours did.

   Result: **size 57,208 (-24 B)**, raw **54,661 (+729)**, aligned **17,894 (-6)**.
   The call histogram then matches for `AEGeometry::setPosition(Vector)`
   (24→23) and `PlayerEgo::getPosition` (42→41), but the -24 B shift moves
   every later instruction and destroys the same-address raw metric.
   `LODManager::forceUpdate` is still 65 vs the original's 66, so the missing
   call cannot compensate the 24 B.  **Not landed** (size is sacred).

2. `level->lodManager->forceUpdate(30, false)` in the *constructor's* mission-81
   arm (line 142) is not the process one; process already has
   `(delta, true)` at line 970 (`0x1381ca`).  Constructor edit reverted.

## Where the 24 B live (measured)

After the case-8 removal the function is still 57,208 B.  Case 8 itself is
now exactly 158 B, identical in size to the original.  The whole function is
therefore **+92 B before case 8** and **-116 B after it** relative to the
original.  The block order differs (compiler layout):

* original: case 10 (`0x13cf76`), case 7 (`0x13d000`), case 8
  (`0x13d13c`), case 9 (`0x13d1de`), inline pool (`0x13d276`), case 11
  (`0x13d2d4`), case 12 (`0x13d3b8`), case 13 (`0x13d440`), case 15
  (`0x13d4c6`), shared (`0x13d4ec`), case 14 (`0x13d4f4`);
* ours: case 8, case 10 (`0x13d238`), ... (case 9/11/12/13 reordered).

The trailing literal pool differs too: original 7 floats + 7 address addends
(`0x145c70`), ours 7 floats + 4 address addends (`0x145c60`) — the original
makes 3 more distinct global-address references.

## Remaining histogram deltas (unchanged at HEAD)

`AEGeometry::setPosition(Vector)` +1 and `PlayerEgo::getPosition` +1 (the
case-8 duplicate pair), `Level::getPlayer` +1, `Level::getStarSystem` +1
(.rel.plt-order anchor), `LODManager::forceUpdate` -1, plus the four
deliberate PLT anchors (`Status::getStanding`, `Standing::setStanding`,
`PlayerEgo::setAutoTurret`, `PlayerEgo::autoTurretIsEnabled`).

## Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
```
