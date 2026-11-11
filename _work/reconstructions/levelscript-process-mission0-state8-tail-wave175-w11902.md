# LevelScript::process — wave 175 / w11902: mission-0 state-8 duplicate + 11-hint companion

Target `_ZN11LevelScript7processEi` (original `0x137d18`, **57,232 B = 0xdf90**).
Worktree `wt/w11902`, branch `wave175-w11902`, base master `5f599a137`.
Scratch `/home/debian/GalaxyOnFire2/scratch/w11902/`.

Builds on `levelscript-process-mission0-state8-tail-wave173-w11802.md`
(the state-8 duplicate removal + the `expect(!isFading(), 0)` companion) and
`levelscript-process-mission0-state8-tail-wave174-w11902.md` (the 8-hint
companion, raw 53,894).  The wave-174 w11902 code was dropped from master by
`7aa2b8208` ("interacts with w11906; keep note"), so this pass re-lands the
duplicate removal on top of the wave-174 w11906 deactivation shape and finds a
better companion.

## Baseline (rebuilt from the clean tree at 5f599a137)

```
process raw diffbytes    53,673
process aligned insns    18,226
orig / ours size         57232 / 57232   (size-exact)
AEGeometry::setPosition  24 calls (original 23)
PlayerEgo::getPosition   42 calls (original 41)
.text diffbytes          80,439
.rel.plt                 byte-identical (md5 6725233d7fd4a9899080cc4d2e743830)
gate byte 4507 / linked 4507 / size 4524 / data 843 / imports 207
```

## The correctness win: remove the state-8 duplicate

The mission-0 `case 8` arm contains a second
`m_pGeometry5->setPosition(player->getPosition())` in the
`m_nScriptTimerA >= 4001` branch (base line 814).  The original arm
(`0x13d194..0x13d1da`) has no such call.  Removing it makes the two call
counts exactly match the original:

| callee | original | base | after removal |
|---|---:|---:|---:|
| `AEGeometry::setPosition(Vector const&)` | 23 | 24 | **23** |
| `PlayerEgo::getPosition()` | 41 | 42 | **41** |

Removal alone is size 57,208 (-24) and raw **54,664** (the -24 shifts the
mission-0 tail).  `.rel.plt` stays byte-identical.  The wave-173 companion
(`expect(!isFading(), 0)` in the state-16 handler) restores size exactly but
raw is **54,299**.

## Companion: 11 branch-weight hints

A full 537-condition x 2 `__builtin_expect` sweep was run on the removal-only
base.  Every size-exact cell (0xdf90) was linked and measured.  The best single
cell was line 4245 (the mission-0 `isDockedToDockingPoint() && hackingWon() &&
getHackingGameDockIndex() >= 0` condition, `expect 1`): raw **53,934**, size
exact, `.rel.plt` identical.

Starting from 4245 + line 1613 (`shipGroup == 8`, `expect 0`, raw 53,882), a
greedy hill-climb over the best size-exact cells plus two full re-sweeps on the
cumulative best produced the final 11-hint shape.  Each hint is a same-line
`// lint: arm_builtin` waiver.  Line numbers are the pre-edit base lines
(the removal is base line 814).

| step | line | condition | expect | cumulative raw |
|---:|---:|---|---:|---:|
| start | 4245 | `player->isDockedToDockingPoint() && player->hackingWon() && player->getHackingGameDockIndex() >= 0` | 1 | 53,882 |
| start | 1613 | `(*m_pLevel->getEnemies())[i]->shipGroup == 8` | 0 | 53,882 |
| 1 | 769 | `messages[15]->isOver()` | 1 | 53,875 |
| 2 | 2539 | `!messages[2]->isOver()` | 0 | 53,867 |
| 3 | 1116 | `m_nScriptTimerA >= 1100 - delta` | 1 | 53,792 |
| 4 | 1294 | `m_nScriptTime >= 38001` | 1 | 53,791 |
| 5 | 4189 | `messages[0]->isOver()` | 1 | 53,786 |
| 6 | 1543 | `!(*enemies)[i]->isWingMan()` | 0 | 53,783 |
| 7 | 2641 | `m_nScriptTimerA >= 3001` | 1 | 53,771 |
| 8 | 2129 | `player->isInWormhole()` | 0 | 53,760 |
| 9 | 1033 | `m_nState == 3` | 1 | **53,759** |

The hints change MachineBlockPlacement weights, not semantics.  The final
shape is size-exact and `.rel.plt` byte-identical.

## Final measurement

```
process raw diffbytes    53,759   (base 53,673, +86; removal-only 54,664, -905)
process aligned insns    18,298   (base 18,226, +72)
orig / ours size         57232 / 57232   (size-exact)
orig / ours insns        20,830 / 20,730
AEGeometry::setPosition  23 calls (original 23)
PlayerEgo::getPosition   41 calls (original 41)
.text diffbytes          80,525   (base 80,439, +86)
.rel.plt                 byte-identical (md5 6725233d7fd4a9899080cc4d2e743830)
gate byte 4507 / linked 4507 / size 4524 / data 843 / imports 207
lint CLEAN / drift 0 / symbol parity 5370/5370 / RATCHET PASS
```

The raw metric is 86 B worse than the base because the duplicate removal
shortens the state-8 block by 24 B and the companion hints restore the
function size without restoring the exact tail layout.  The win is the exact
`setPosition`/`getPosition` call parity; the companion recovers **905 raw bytes**
versus removal-only, **617 B** versus the wave-173 w11802 figure (54,376), and
**135 B** versus the wave-174 w11902 figure (53,894).  Size, `.rel.plt`,
imports and the gate are unchanged.

## Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 53759 aligned=18298 size= 57232/57232 insn= 20730/20830
readelf -rW cmake-build-match/libgof2hdaa.so | awk '/\.rel\.plt/{f=1;next} f&&/^$/{exit} f{print $1, $NF}'
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch harnesses (all under `/home/debian/GalaxyOnFire2/scratch/w11902/`):
`base.cpp`, `removal.cpp`, `removal_fade.cpp`, `hints8.cpp`, `final.cpp`,
`compile.sh`, `linkcmd.sh`, `measure.sh`, `sweep.py`, `sweep_all.py`,
`sweep2.py`..`sweep5.py`, `diffruns.py`, and the `sweep*.tsv` result tables.
