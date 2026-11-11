# LevelScript::process — wave 177 / w12102: 10-spec alias re-test + new state-3 camera alias

Target `_ZN11LevelScript7processEi` (original `0x137d18`, **57,232 B = 0xdf90**),
owned TU `src/game/world/LevelScript.cpp`.  Worktree `wt/w12102`, branch
`wave177-w12102`, base master `9445ac5fb` (wave 176).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w12102`.

Follow-up to `levelscript-process-alias-greedy-wave176-w12002.md` (the wave-176
greedy set: ten block-scope aliases, 53,542 on the wave-175 base, neutral on the
combined tree; only the `enemiesC` alias was committed and then reverted) and the
wave-173/174/175 alias notes.

## Baseline (clean wave-176 tree)

```
process raw diffbytes    53,568
process aligned insns    18,498
orig / ours size         57232 / 57232   (size-exact)
orig / ours insns        20830 / 20726
.text diffbytes          79,591
.rel.plt                 0 differing bytes (byte-identical)
gate: byte 4507 / linked 4507 / size 4524 / data 843 / imports 207 / lint CLEAN / drift 0
```

## Re-test of the wave-176 ten-spec set

The ten specs are exactly the wave-176 w12002 set, re-applied one at a time on
the current base (line numbers are the current file's, unchanged from the
wave-176 note):

| name | expression | type | block | uses | raw alone | size |
|---|---|---|---:|---:|---:|---:|
| `canvas983` | `Globals::Canvas` | `AbyssEngine::PaintCanvas` | 983-1003 | 2 | **53,554** (-14) | 57,232 |
| `cam1281` | `m_pCamera` | `TargetFollowCamera` | 1281-1308 | 2 | **53,564** (-4) | 57,232 |
| `lvl1613` | `m_pLevel` | `Level` | 1613-1618 | 3 | **53,566** (-2) | 57,232 |
| `rnd1702` | `Globals::rnd` | `AbyssEngine::AERandom` | 1702-1741 | 2 | 53,572 (+4) | 57,232 |
| `lvl2337` | `m_pLevel` | `Level` | 2337-2353 | 3 | 54,796 (+1,228) | 57,240 (+8) |
| `lvl2791` | `m_pLevel` | `Level` | 2791-2808 | 2 | **53,565** (-3) | 57,232 |
| `pgeom3630` | `player->geometry` | `AEGeometry` | 3630-3653 | 2 | **53,558** (-10) | 57,232 |
| `lvl4280` | `m_pLevel` | `Level` | 4280-4287 | 2 | **53,567** (-1) | 57,232 |
| `enemies4568` | `m_pLevel->getEnemies()` | `Array<KIPlayer *>` | 4568-4599 | 2 | **53,564** (-4) | 57,232 |
| `galaxy5182` | `Globals::galaxy` | `Galaxy` | 5182-5196 | 2 | 53,610 (+42) | 57,236 (+4) |

Seven of the ten are size-exact raw wins on the current base.  Their deltas are
exactly additive: the combined seven-spec set measures **53,530** (57,232 B
exact), i.e. 53,568 - 38.  The three rejected specs are `rnd1702` (+4),
`lvl2337` (size +8, raw +1,228) and `galaxy5182` (size +4, raw +42); the full
ten-spec set is 54,775 / 57,244 B, so it is not size-exact.

The seven landed aliases are:

1. `AbyssEngine::PaintCanvas *canvas983 = Globals::Canvas;` in `case 0:`.
2. `TargetFollowCamera *cam1281 = m_pCamera;` in `case 2:`.
3. `Level *lvl1613 = m_pLevel;` in the mission-0 shipGroup-8 for body.
4. `Level *lvl2791 = m_pLevel;` in the `m_nScriptTimerA >= 2151` block.
5. `AEGeometry *pgeom3630 = player->geometry;` in the `m_nState == 4` block.
6. `Level *lvl4280 = m_pLevel;` in the `m_pLevel->getGasClouds()` block.
7. `Array<KIPlayer *> *enemies4568 = m_pLevel->getEnemies();` in the
   `m_nState == 2` block.

## New block found by the fresh sweep

The post-wave-176 base was re-swept family by family with the same brace-block
parser used in waves 170-175.  Each candidate was inserted at the block head,
every use in the block body rewritten, then the tree rebuilt and measured.

| family | blocks with >=N uses | best size-exact raw | result |
|---|---:|---:|---|
| `m_pCamera` | 167 (>=3) | **53,528** | **win: `m_nState == 1` block 1687-1702** |
| `m_pLevel` | 102 (>=3) | 53,532 | no win |
| `m_pHud` | 22 (>=2) | 53,728 | no win |
| `m_pRadar` | 37 (>=2) | 53,530 | neutral |
| `Globals::Canvas` | 51 (>=2) | 53,531 | no win |
| `Globals::galaxy` | 11 (>=2) | 53,605 | no win |
| `Globals::layout` | 16 (>=2) | 53,536 | no win |
| `Globals::status` | 46 (>=2) | 53,530 | neutral |
| `Globals::sound` | 44 (>=2) | 53,531 | no win |
| `Globals::rnd` | 43 (>=2) | 53,534 | no win |
| `player->player` | 30 (>=2) | 53,531 | no win |
| `m_pLevel->getEnemies()` | 34 (>=2) | 53,689 | no win |
| `m_pLevel->getPlayer()` | 9 (>=2) | 53,535 | no win |
| `Globals::appManager` | 3 (>=2) | 53,733 | no win |

The new win is the mission-? `m_nState == 1` block (file lines 1687-1702),
which already declares `Level *self = m_pLevel;` and uses `m_pCamera` three
times (`translate`, `setLookAtCam`, `setTarget`):

```c++
} else if (m_nState == 1) {
    TargetFollowCamera *cam1688 = m_pCamera;
    Level *self = m_pLevel;
    cam1688->translate((float) -delta, 0.0f, (float) (-3 * delta));
    ...
    cam1688->setLookAtCam(false);
    cam1688->setTarget(player->geometry);
```

On top of the seven-spec set it measures **53,528 / 18,925 / 57,232 B** (raw
-2, size exact, `.rel.plt` byte-identical).

## Additive re-check and near misses

Adding any one of the next-best size-exact candidates to the eight-spec set
gives 53,529 (one worse), so the eight-spec set is the local optimum:

| added candidate | raw on top of the 8-set |
|---|---:|
| `m_pCamera` 736-755 | 53,529 |
| `Globals::Canvas` 2659-2675 | 53,529 |
| `Globals::sound` 3860-3888 | 53,529 |
| `player->player` 1797-1826 | 53,529 |
| `Globals::status` 1651-1658 | 53,529 |
| `Globals::status` 2942-2988 | 53,528 (neutral) |
| `m_pRadar` 4046-4049 | 53,528 (neutral) |

Every pair of the five 53,529 candidates measures 53,530, and all five together
measure 53,533, so no additive-only win remains in this family.

## Final

```
process raw diffbytes    53,528   (was 53,568, -40)
process aligned insns    18,925   (was 18,498; raw is the ground truth)
orig / ours size         57232 / 57232   (size-exact)
orig / ours insns        20830 / 20730
.text diffbytes          79,551   (was 79,591, -40)
.rel.plt                 0 differing bytes (byte-identical)
.ARM.extab               1,321 differing bytes (unchanged)
gate                     byte 4507 / linked 4507 / size 4524 / data 843 /
                         imports 207 / lint CLEAN / drift 0 / parity 5370/5370
```

`.text` falls by exactly the function's raw delta (-40); `.ARM.extab`,
`.rel.plt`, `.rodata`, `.data`, `.bss` and `.ARM.exidx` are unchanged.

## Reproduction

```sh
ulimit -v 12000000
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 53528 aligned= 18925 size= 57232/57232 insn= 20730/20830
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
```

The eight source edits are the seven aliases listed above plus the
`TargetFollowCamera *cam1688 = m_pCamera;` alias at file line 1688.  The
sweep harnesses are `blocks.py`, `cand_enum.py`, `sweep_family.py`,
`apply.py`, `measure.py`, `test_add.py` and `test_group*.py` in
`scratch/w12102`; the sweep records are `sweep_*.jsonl`.
