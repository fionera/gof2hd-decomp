# LevelScript::process — wave 173 / w11802: mission-0 state-8 duplicate + shared tail

Target `_ZN11LevelScript7processEi` (original `0x137d18`, 57,232 B = 0xdf90).
Worktree `wt/w11802`, branch `wave173-w11802`, base master `3fafa5b03`.
Scratch `/home/debian/GalaxyOnFire2/scratch/w11802/`.

Builds on `levelscript-process-mission0-state8-tail-wave172-w11703.md`
and `levelscript-process-mission0-state8-tail-wave170-w11603.md`.

## Baseline (rebuilt from the clean tree)

```
process raw diffbytes    53,750
process aligned insns    18,049
orig / ours size         57232 / 57232   (size-exact)
.rel.plt                 byte-identical (md5 6725233d7fd4a9899080cc4d2e743830)
gate byte 4507 / linked 4507 / size 4524 / data 843 / imports 207
```

## Landed shape

* Remove the state-8 duplicate `m_pGeometry5->setPosition(player->getPosition())`
  in the `m_nScriptTimerA >= 4001` arm.  The original arm (`0x13d194..0x13d1da`)
  has no setPosition.
* Keep the state-16 handler as the post-switch `if (m_nState == 16)` block
  (the switch's default/shared tail).  The state-15 over path still `break`s to
  the shared active tail (`0x137f7c`), exactly as the original's `b.w 0x137f7c`
  at `0x13d4f0`.
* Pay the 24-byte size loss with
  `__builtin_expect(!Globals::layout->isFading(), 0)` in the handler's fade
  test.  This is the branch weighting the original exhibits at `0x13f91a`
  (`bne.w 0x137f7c` is the hot edge for the whole 5 s fade), and it is the only
  measured hint that lands the function back at exactly 57,232 B without moving
  a block.

## Measured (full link, `tools/fn_diff.py`)

| shape | raw | aligned | size | `.rel.plt` |
|---|---:|---:|---:|---|
| baseline (HEAD) | **53,750** | **18,049** | 57,232 | identical |
| state-8 duplicate removed only | 54,710 | 18,043 | 57,208 | identical |
| **landed (removal + post-switch handler + `expect(!isFading(),0)`)** | **54,376** | **18,290** | **57,232** | **identical** |
| landed + `m_pCamera` instead of the `wcamera` local in state 8 | 54,377 | 18,289 | 57,232 | identical |
| landed, `case 16:` / `default:` handler instead of post-switch `if` | 55,146 | 19,225 | 57,228 | rotates |
| landed, `default:` without the hint | 55,007 | 19,288 | 57,236 | rotates |
| landed + remove the extra `Level::initStreamOutPosition = 1` store | 55,128 | 18,285 | 57,216 | identical |
| landed + `expect(m_nState == 16, 0)` instead of the fade hint | 55,097 | 19,399 | 57,220 | identical |
| landed + `expect(m_nState == 16, 1)` instead of the fade hint | 54,937 | 19,488 | 57,224 | rotates |
| landed + `goto shared_tail` reshape | 54,710 | 18,043 | 57,208 | identical |
| landed + `goto shared_tail` reshape + fade hint | 54,986 | 18,300 | 57,228 | identical |

A 34-condition `__builtin_expect` sweep over the whole mission-0 block
(lines 616–940) on this base found no size-exact, `.rel.plt`-identical cell
better than the landed 54,376; the only other size-exact cells were neutral
(54,376) or worse (54,391 at line 732 var 0).

## The actual win: call-count parity

The duplicate is a real extra call pair.  Counting `blx` sites inside the
57,232-byte body:

| callee | original | base | landed |
|---|---:|---:|---:|
| `AEGeometry::setPosition(Vector const&)` | 23 | 24 | **23** |
| `PlayerEgo::getPosition()` | 41 | 42 | **41** |

The landed shape makes both counts exactly match the original.  This is the
correctness win; the raw metric regresses because the state-8 block and its
literal pool move:

* the original state-8 arm is `0x13d13c..0x13d1da` (74 B); ours is
  `0x13d0ae..0x13d14c` (70 B).  The original reloads `m_pCamera` from
  `[sl,#20]` and ends in `b.w 0x137f7c`; ours keeps the camera in `r8` and ends
  in `b.n` to the nearby post-switch handler.
* the state-8 removal shifts the mission-0 tail by +0x5c and the literal pool
  that clang places at `0x13d150` is not present at the corresponding original
  address, so the same-address raw metric is layout-dominated.
* the fade hint restores the missing 24 B but costs 334 raw bytes
  (54,710 -> 54,376) while adding 247 aligned instructions.

The shape is size-exact, `.rel.plt` byte-identical, and gate-green:
`byte_exact 4507 / linked_exact 4507 / size 4524 / data 843 / imports 207`,
lint CLEAN, drift 0, parity 5370/5370.

## Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
readelf -rW cmake-build-match/libgof2hdaa.so | awk '/\.rel\.plt/{f=1;next} f&&/^$/{exit} f{print $1, $NF}'
cmake --build cmake-build-match --target check -j2
```
