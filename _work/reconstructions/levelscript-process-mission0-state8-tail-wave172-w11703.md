# LevelScript::process — wave 172 / w11703: mission-0 state 8 + shared tail, `.rel.plt` safe

Target `_ZN11LevelScript7processEi` (original `0x137d18`, 57,232 B = 0xdf90).
Worktree `wt/w11703`, branch `wave172-w11703`, base master `41ef4c8b6`.
Scratch `/home/debian/GalaxyOnFire2/scratch/w11703/`.

Builds on `levelscript-process-mission0-state8-tail-wave170-w11603.md`.

## Baseline (rebuilt from the clean tree)

```
process raw diffbytes    53,812
process aligned insns    17,932
orig / ours size         57232 / 57232   (size-exact)
.rel.plt                 byte-identical
gate byte 4507 / linked 4507 / size 4524 / data 843 / imports 207
```

## Landed shape

* Remove the state-8 duplicate `m_pGeometry5->setPosition(player->getPosition())`
  in the `m_nScriptTimerA >= 4001` arm.  The original arm (`0x13d194..0x13d1da`)
  has no setPosition; the call is a real extra `AEGeometry::setPosition(Vector)` /
  `PlayerEgo::getPosition()` pair.
* Keep the state-16 handler as the post-switch `if (m_nState == 16)` block — i.e.
  the switch's default handler / shared tail.  The state-15 over path still
  `break`s to the shared active tail (`0x137f7c`), exactly as the original's
  `b.w 0x137f7c` at `0x13d4f0`; the not-over path cannot run the handler because
  `m_nState` is still 15.
* Add `__builtin_expect(!Globals::layout->isFading(), 0)` to the handler's fade
  test.  This is the branch-weighting the original exhibits: at `0x13f91a` the
  `bne.w 0x137f7c` (isFading() != 0 -> skip) is the hot edge for the whole 5 s
  fade, so `!isFading()` is the cold arm.  It is also what restores the missing
  +24 B: the state-8 removal alone is 57,208 B, and the hint lands the function
  back at exactly 57,232 B without moving any block.

`.rel.plt` stays byte-identical (verified by comparing the offset+symbol
sequence of `readelf -rW` for ours vs the original; 0 differing entries).

## Measured (full link, `tools/fn_diff.py`)

| shape | raw | aligned | size | `.rel.plt` |
|---|---:|---:|---:|---|
| baseline (HEAD) | **53,812** | **17,932** | 57,232 | identical |
| state-8 duplicate removed only (A) | 54,686 | 17,926 | 57,208 | identical |
| **landed (A + post-switch handler + `expect(!isFading(),0)`)** | **54,422** | **18,167** | **57,232** | **identical** |
| A + `expect(m_nState == 16, 1)` | 54,879 | 19,306 | 57,232 | rotates 4 (flashScreen group) |
| A + `default:` + `expect(m_nState == 16,0)` | 55,114 | 17,970 | 57,224 | identical |
| A + `expect(m_nState == 16,0)` | 55,097 | 19,231 | 57,228 | identical |
| A + `default:` (no hint) | 55,353 | 19,146 | 57,244 | rotates 10 |
| w11603 case-16 fallthrough on this base | 54,999 | 18,202 | 57,224 | rotates 10 (setVisible group) |
| case-15 over-break / not-over falls to `case 16:` | 55,383 | 18,912 | 57,256 | rotates 18 |
| case-15 over-break / not-over falls to `default:` | 55,305 | 19,996 | 57,256 | rotates 8 (flashScreen group) |
| landed + move `m_pParticleGeom1->setPosition` before the `currentTime` test | 55,290 | 18,174 | 57,240 | identical |

The raw metric remains layout-dominated: the state-8 arm itself is
instruction-for-instruction identical to the original (same 24 instructions:
`movs #9 / str m_nState / strd timer / setRumblePercentage /
TransformGetTransform / SetAnimationState(0) / TransformGetTransform /
SetAnimationState(3)`), but clang places a literal pool at `0x13d13c` in ours
that the original does not have there, shifting the mission-0 tail by +0x5c.
The landed shape is the best size-exact, `.rel.plt`-identical shape measured.

## Why the w11603 case-16 fallthrough rotates `.rel.plt`

Putting the state-16 handler inside the switch as `case 16:` changes
MachineBlockPlacement of the mission-125 state-7 block: its first
`PlayerStation::setVisible` reference moves from `0x13da1e` (after mission-0
state 9) to before the `StarSystem::switchPlanetForIntro` group, rotating the
`0x2132d4..0x2132e4` `.rel.plt` slots.  `default:`-based variants move a
different cold block and rotate the `Level::flashScreen` group instead.
Keeping the handler after the switch (the source shape above) leaves the block
order untouched and `.rel.plt` byte-identical.

## Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
readelf -rW cmake-build-match/libgof2hdaa.so | awk '/\.rel\.plt/{f=1;next} f&&/^$/{exit} f{print $1, $NF}'
cmake --build cmake-build-match --target check -j2
```
