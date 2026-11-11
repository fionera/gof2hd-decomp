# Radar::draw wave 179 (w12309) — dead alias removal + barMax load sunk to use site

**Status:** source change landed, 2026-11-11. Worktree `wt/w12309`, branch
`wave179-w12309`, base `master` `64014ce6f` (wave-178 merge). Target
`src/game/weapons/Radar.cpp`, `_ZN5Radar4drawEP6PlayerP3Hudi`
(`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave178-w12209.md` (the landed shape this pass starts
from) and `radar-draw-wave174-w11907.md` (the `campaign`/`sl` residual
analysis).

## 0. Baseline and result

| metric | base | landed |
|---|---:|---:|
| function size | 9,908 | **9,908** |
| raw same-address diff | 6,458 | **6,424** (-34) |
| instruction-index alignment | 1,832 | **1,817** (-15) |
| instruction count | 3,540 ours / 3,532 orig | 3,540 / 3,532 |
| `.rel.plt` | byte-identical (0) | byte-identical (0) |

Gate after the change: byte_exact **4,507/4,524**, linked_exact
**4,507/4,524**, size-exact **4,524/4,524**, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data_exact **843/843**, imports **207**, lint
**CLEAN**, drift **0**, parity **5,370/5,370**. `RATCHET PASS`.

## 1. Landed change (two cells, net size 0)

Both cells are pure local-lifetime edits: an alias with no remaining use is
deleted, and a field load is sunk past the `isDead()` branch to its single
use site. Neither changes observable behaviour.

```diff
@@ enemy on-screen bar block (line ~717)
-                        auto barMax = this->field_0x1c8;
                         if (!enemy->isDead()) {
                             int cx = (int) PlayerEgo::crosshairPos.x;
                             int cy = (int) PlayerEgo::crosshairPos.y;
@@
                         }
                         Player *target = enemy->player;
+                        auto barMax = this->field_0x1c8;
                         unsigned int frameImage;
                         unsigned int barImage;

@@ enemy tractor block (line ~844)
-                        auto onPlanetCourse = this->field_0x1a8;
                         enemy->field_0x75 = 1;
-                        if ((wreck && this->hasTractor13) || (this->field_0x1b0 && !onPlanetCourse)) {
+                        if ((wreck && this->hasTractor13) || (this->field_0x1b0 && !this->field_0x1a8)) {
```

Individual contributions on the wave-178 base (all size 9,908):

| cell | raw | aligned |
|---|---:|---:|
| base | 6,458 | 1,832 |
| 1 drop unused `onPlanetCourse` alias | 6,445 | 1,827 |
| 2 cell 1 + `barMax` sunk after `Player *target` | **6,424** | **1,817** |

Cell 2 alone (on the base) is the `barMax` move; the combination is 21 bytes
better than cell 1 alone, so the two cells interact through the branch layout
of the surrounding `if/else` chain.

## 2. Search performed (all size-exact cells only)

Baseline for every cell below is the landed 6,424 / 1,817 / 9,908 unless
noted. All builds used `cmake --build cmake-build-match --target link_so -j2`
under `ulimit -v 12000000`; scratch is
`/home/debian/GalaxyOnFire2/scratch/w12309/`.

### 2.1 Rejected locals/hoists

| variant | raw | aligned | size |
|---|---:|---:|---:|
| drop `Player *target` alias (use `enemy->player`) | 6,424 | 1,817 | 9,908 |
| drop `Array<int> *cargo` alias | 6,424 | 1,817 | 9,908 |
| drop `auto camZ = this->cameraZ` alias | 6,498 | — | 9,908 |
| drop `auto hasScanner` alias | 6,562 | — | 9,908 |
| `rowHeight` moved after `if (i <= 1)` | 6,425 | — | 9,908 |
| `rowHeight` moved before the label `DrawString` | 8,504 | 1,824 | **9,904** |
| `rowHeight` used in the tec-level `DrawString` | 8,541 | 1,823 | **9,900** |
| `layoutField(4)` instead of `Globals::layout->field_0x4` | 6,424 | 1,817 | 9,908 |
| `Player *target` before `if (!enemy->isDead())` | 8,213 | 1,802 | **9,912** |
| `frameImage`/`barImage` decls before `Player *target` | 6,424 | 1,817 | 9,908 |
| `barMax` after `frameImage`/`barImage` decls | 6,424 | 1,817 | 9,908 |
| `barMax` moved before `float hull` (after the frame draw) | 8,315 | 1,820 | **9,904** |
| `int barMax` instead of `auto` | 6,424 | 1,817 | 9,908 |
| `hull`/`barWidth` moved before the frame `DrawImage2D` | 6,478 | 1,854 | 9,908 |
| `this->barWidth = (int) hull` moved after `DrawRegion2D` | 6,473 | 1,815 | 9,908 |
| `targetPos` assignment before `egoPos` | 9,172 | 2,230 | **9,936** |
| `targetPos` assignment before `enemy->field_0x76 = 1` | 6,425 | 1,810 | 9,908 |
| use `target` for `getDamageRate`/`getEmpPoints`/`getEmpDamageRate`/lock flag | 8,357 | 2,221 | **9,904** |
| drop `auto target = this->targetPos` alias | 6,425 | 1,815 | 9,908 |
| `auto target` hoisted before `inSight` | 8,583 | 1,846 | **9,920** |
| `int shipGroup` alias in the skip block | 8,531 | 1,913 | **9,900** |
| `int shipGroup` alias at loop top | 9,048 | 2,284 | **9,936** |
| `Player *ep = enemy->player` alias in the on-screen block | 9,266 | 2,277 | **9,988** |
| drop `KIPlayer *e` alias in the wreck image block | 8,512 | 1,829 | **9,900** |
| `auto barMax` before `Player *target` (decls order) | 6,424 | 1,817 | 9,908 |
| `Array<KIPlayer *> *landmarks` alias before the planet loop | 9,440 | 1,837 | **9,892** |
| `Status *status` alias before the planet loop | 8,524 | 1,802 | **9,900** |
| `KIPlayer *landmark` hoisted before `if (i <= 1)` | 8,234 | 1,777 | **9,896** |

### 2.2 Prior-session cells re-confirmed on the landed base

* Wreck-block statement order: `update; wreck; 75` 6,426, `wreck; update; 75`
  6,425; base `wreck; 75; update` is best.
* `skip nested` bossSeen 9,233; `bossSeen = expr` in the later block 8,368.
* `drop barMax` (direct `this->field_0x1c8` at both uses) 6,430.
* `barMax` after `isDead` / after `target` 6,424 (same as landed).
* asteroid `wreck` bool 7,102; independent-if 7,562.
* `campaign` unsigned 6,459; `alienOrbit` bool 6,466; `mbp` bool 6,921;
  `turretMode` int 9,238.
* music tail: no-expect `alwaysEnemySeen` 6,427; operand swap 6,424;
  removing `bc<3`/`bc<5` expects size-fatal (9,904); no-expect `bc>=1`
  6,497; `bc>=1` expect 0 → 8,550.

## 3. Remaining residual

Unchanged from wave 178: the original keeps `campaign` in `sl`
(`12e0f8 mov sl,r0`, `12e38a cmp.w sl,#2`) and reuses `sl` for the planet
loop index (`12e4be ldr.w r0,[sl,#4]`), while ours spills `campaign` to
`sp+0x90` and uses `sl` for the zero constant / planet loop index
(`U[17] mov.w sl,#0` vs `O[17] mov.w fp,#0`). The original stacks the bar
image pointers in different slots (`sp+0x5c/0x60/0x68` permutations) than
ours. The music tail still differs (`cmp r1,#106; ite lt; movwlt 2241;
movwge 2242` in the original). Every size-exact `campaign`-in-`sl` source
shape measured in waves 174-175 is raw-worse, so the rotation is still open.

## 4. Reproduction

```sh
ulimit -v 12000000
cp /home/debian/GalaxyOnFire2/scratch/w12309/Radar.base.cpp src/game/weapons/Radar.cpp
# apply the two edits from section 1, then:
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
cmake --build cmake-build-match --target check -j2
cmake --build cmake-build-match --target drift -j2
```

Scratch: `/home/debian/GalaxyOnFire2/scratch/w12309/` —
`Radar.base.cpp` (wave-178 base source), `Radar.win1.cpp` (cell 1 only),
`Radar.win2.cpp` (landed, both cells), `one.py` / `one2.py` (build+measure
harness), `v1.json`..`v22.json` (variant specs), `aldiff.py` (side-by-side
classifier), `shift.py` (byte-delta map).
