# LevelScript::process — wave 174 / w11902: mission-0 state-8 duplicate + branch-weight companion

Target `_ZN11LevelScript7processEi` (original `0x137d18`, 57,232 B = 0xdf90).
Worktree `wt/w11902`, branch `wave174-w11902`, base master `be9dd429b`.
Scratch `/home/debian/GalaxyOnFire2/scratch/w11902/`.

Builds on `levelscript-process-mission0-state8-tail-wave173-w11802.md`
(the state-8 duplicate removal and the `expect(!isFading(), 0)` companion)
and the wave-173 alias/expect passes.

## Baseline (rebuilt from the clean tree)

```
process raw diffbytes    53,711
process aligned insns    18,228
orig / ours size         57232 / 57232   (size-exact)
AEGeometry::setPosition  24 calls (original 23)
PlayerEgo::getPosition   42 calls (original 41)
.text diffbytes          80,897
.rel.plt                 byte-identical (md5 6725233d7fd4a9899080cc4d2e743830)
gate byte 4507 / linked 4507 / size 4524 / data 843 / imports 207
```

## The correctness win: remove the state-8 duplicate

The mission-0 `case 8` arm contains a second
`m_pGeometry5->setPosition(player->getPosition())` in the
`m_nScriptTimerA >= 4001` branch.  The original arm (`0x13d194..0x13d1da`)
has no such call.  Removing it makes the two call counts exactly match the
original:

| callee | original | base | after removal |
|---|---:|---:|---:|
| `AEGeometry::setPosition(Vector const&)` | 23 | 24 | **23** |
| `PlayerEgo::getPosition()` | 41 | 42 | **41** |

Removal alone is size 57,208 (-24) and raw **54,698** (the -24 shifts the
mission-0 tail).  `.rel.plt` stays byte-identical.

## Companion shape: eight branch-weight hints

A full 621-condition x 2 `__builtin_expect` sweep was run on the removal-only
base.  Each candidate was compiled with the exact NDK r18b `-Oz` command and
size-filtered with `nm -S`; every size-exact cell (0xdf90) was then linked and
measured with `tools/fn_diff.py`.  The best single cell was line 4244
(`expect(player->isDockedToDockingPoint() && player->hackingWon() &&
player->getHackingGameDockIndex() >= 0, 1)`): raw **53,972**, size exact,
`.rel.plt` identical.

The 4244 hint is special because it leaves the whole early mission-0 layout
unchanged: the same-address diff region starts at `0xed4` (baseline) instead
of `0x3e` (the `expect(!isFading(), 0)` shape).  That baseline-preserving
layout is what recovers the raw bytes lost to the duplicate removal.

The remaining hints were found by a second sweep over every size-neutral
(57,208) cell on top of 4244 v1, and then over the best cumulative base.
Each is a same-line `// lint: arm_builtin` waiver.

| line | condition | expect | cumulative raw |
|---:|---|---:|---:|
| 4244 | `player->isDockedToDockingPoint() && player->hackingWon() && player->getHackingGameDockIndex() >= 0` | 1 | 53,972 |
| 769 | `messages[15]->isOver()` | 1 | 53,931 |
| 2539 | `!messages[2]->isOver()` | 0 | 53,923 |
| 2451 | `messages[1]->isOver()` | 0 | 53,922 |
| 1116 | `m_nScriptTimerA >= 1100 - delta` | 0 | 53,916 |
| 2641 | `m_nScriptTimerA >= 3001` | 1 | 53,910 |
| 1294 | `m_nScriptTime >= 38001` | 1 | 53,899 |
| 4188 | `messages[0]->isOver()` | 1 | **53,894** |

The order in the table is the order in which the hints were added; the raw
column is the cumulative measurement at each step.  The final shape is
size-exact and `.rel.plt` byte-identical.

## Final measurement

```
process raw diffbytes    53,894   (base 53,711, +183)
process aligned insns    18,317   (base 18,228, +89)
orig / ours size         57232 / 57232   (size-exact)
AEGeometry::setPosition  23 calls (original 23)
PlayerEgo::getPosition   41 calls (original 41)
.text diffbytes          81,080   (base 80,897, +183)
.rel.plt                 byte-identical (md5 6725233d7fd4a9899080cc4d2e743830)
gate byte 4507 / linked 4507 / size 4524 / data 843 / imports 207
lint CLEAN / drift 0 / symbol parity 5370/5370
```

The raw metric regresses 183 B versus the base because the duplicate removal
shifts the mission-0 state-8 tail by -24 B and the companion hints only
restore the function size, not the exact tail layout.  The removal-only shape
was 54,698, so the companion recovers **804 raw bytes** and lands 482 B
better than the wave-173 w11802 figure (54,376).  The win is the exact
`setPosition`/`getPosition` call parity; size, `.rel.plt`, imports and the
gate are unchanged.

## Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
readelf -rW cmake-build-match/libgof2hdaa.so | awk '/\.rel\.plt/{f=1;next} f&&/^$/{exit} f{print $1, $NF}'
cmake --build cmake-build-match --target check -j2
```
