# Radar::draw wave 182 (w12611) — if/else inversion cell (raw 6,339 -> 6,336)

**Status:** landed, 2026-11-11. Worktree `wt/w12611`, branch `wave182-w12611`,
base `master` `57e085059` (wave-181 harvest 5). Target
`src/game/weapons/Radar.cpp`, `_ZN5Radar4drawEP6PlayerP3Hudi`
(`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave181-w12507.md` (the 6,340 shape),
`radar-draw-wave181-w12509.md` (music-tail reorders) and
`radar-draw-wave179-w12307.md` (alias re-sweep). This note is self-contained:
every number was measured in this worktree on the 6,339 base.

## 0. Baseline and landed result

| metric | base | landed |
|---|---:|---:|
| function size | 9,908 | **9,908** |
| raw same-address diff | 6,339 | **6,336** (-3) |
| instruction-index alignment | 1,757 | 1,759 |
| instruction count | 3,542 ours / 3,532 orig | 3,542 / 3,532 |
| whole-`.so` `.text` differing bytes | 75,365 | **75,362** (-3) |
| `.ARM.exidx` differing bytes | 0 | 0 |
| `.ARM.extab` differing bytes | 655 (size/base exact at `0x1a3b78`, 156,312 B) | **655** (unchanged) |
| `.rel.plt` | byte-identical | **byte-identical** |
| `.rodata` / `.data` / `.dynsym` / `.dynstr` / `.hash` / `.rel.dyn` / `.got` | 0 | 0 |
| `.note.gnu.build-id` differing bytes | 20 | 20 |
| file size | 2,187,328 | 2,187,328 |
| total differing bytes | 76,040 | **76,037** |

Gate after the change: byte_exact **4,507/4,524**, linked_exact
**4,507/4,524**, size-exact **4,524/4,524**, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data_exact **843/843**, imports **207**, lint **CLEAN**,
drift **0**, parity **5,370/5,370**. `RATCHET PASS`.

## 1. Landed change (one if/else inversion, net size 0)

In the asteroid-lock block (line ~1223), the `hasTractor13` arm is inverted and
its two bodies exchanged:

```diff
                         } else {
-                            if (!this->hasTractor13) {
-                                hud->hudEvent(9, this->level->getPlayer(), 0);
-                            } else {
+                            if (this->hasTractor13) {
                                 this->field_0x1c = this->field_0x10;
+                            } else {
+                                hud->hudEvent(9, this->level->getPlayer(), 0);
                             }
                         }
```

`hasTractor13` is a plain member load and the two bodies are mutually
exclusive, so the inversion is semantics-preserving (the same call happens on
the same runtime condition). The relocation set is unchanged, so `.rel.plt`
stays byte-identical (verified: no `.rel.plt` row in the section diff).

## 2. Why this cell

The 6,339 body is a local optimum for every single expect/alias edit measured
here (section 3). An if/else inversion sweep — a cell type not covered by the
wave-179/180/181 expect/alias/condition/statement sweeps — finds exactly one
raw winner: this `hasTractor13` arm at raw **6,336**. Every other invertible
if/else is neutral (6,339) or worse (6,343+). The -3 bytes come from the
changed block layout around the `hudEvent` call and the `field_0x1c` store;
the branch condition itself is the same `ldrb` test.

## 3. Sweeps performed on the 6,339 base

Every measurement is a full `link_so` + `tools/fn_diff.py` (`-j2`,
`ulimit -v 12000000`).

### 3.1 Existing `__builtin_expect` remove/flip (66 variants)

All 33 sites removed and flipped. No single edit beats 6,339. Neutral:
`rm4`, `rm12`, `rm15`, `fl15`, `fl17`, `fl12`. Nearest worse: `rm0` 6,340,
`rm20` 6,340, `rm13` 6,342, `rm23` 6,342, `rm32` 6,343, `rm8` 6,345.

### 3.2 Add-hint sweep (174 plain `if` conditions x 2 weights = 348)

No plain `if` gains from a whole-condition `expect(...,0)` or `expect(...,1)`.
Best is neutral 6,339.

### 3.3 Sub-expression hints (every top-level `&&`/`||` operand, both weights)

No win; best neutral 6,339.

### 3.4 Alias removal (106 declarations, one-use and multi-use inlining)

No win. Neutral (6,339): `system`, `size`/`left`/`top` in the landmark,
planet, gas-spark, gas and asteroid crosshair blocks, `landmark`, `cx`,
`target` (both the `Player*` alias and the `Vector` copy), `e`, `cargo`,
`current`, `blip`, `total`, `level`, `stationIndex`-adjacent forms. The
`blip`/`fl17`/`left` variants that keep raw 6,339 change bytes but do not
combine into a win (all 66 pairs of the 12 distinct neutral shapes measured;
best 6,339).

### 3.5 Condition operand permutations

All adjacent swaps of uniform `&&`/`||` chains in `if`/`while` conditions
(198 conditions), plus reversal/move-to-front/move-to-back of every chain with
3+ operands: no win. All 134 adjacent same-indent single-line statement swaps:
no win (best neutral 6,339).

### 3.6 Type / initializer changes

All `bool`<->`int` flips and `false`/`0`, `true`/`1`, `nullptr`/`0`
initializer swaps: no win.

### 3.7 Alternative spellings of the wave-181 cells

`cargo == nullptr` -> `!cargo` / `cargo == 0` (neutral), weight 1 (7,003);
`getCurrentCampaignMission() != 145` -> plain (6,340), `==145` inverted
(8,381, size 9,900); `field_0x1a9` -> `!= 0` / `!!` (neutral), plain (6,340),
weight 0 (8,560); landmark store swap back (6,340); `hasScanner` swap back
(6,340). The landed wave-181 forms remain best.

### 3.8 If/else inversion (the winning cell)

Every `if (cond) {A} else {B}` with brace bodies was rewritten as
`if (!(cond)) {B} else {A}`. Only the `hasTractor13` arm wins (6,336); all
others are neutral or worse.

## 4. Remaining residual

Unchanged from wave 174/179/181: the original keeps `campaign` in `sl`
(`12e0f8 mov sl,r0`, `12e38a cmp.w sl,#2`) and later reuses `sl` for the
stations array, with the planet-loop index in `fp`; ours spills `campaign` to
`sp+0x90`, keeps `stations` at `sp+0x84` and uses `sl` for the loop index. The
stack-slot rotation (`hud` at `sp+0x50` vs the original's `sp+0x54`,
`missionBlocksPlanets`/`otherLock` slots at `sp+0x90`/`sp+0x94` vs
`sp+0x90`/`sp+0x94` with a different OR order) and the `marked` stack
normalization (`12e6c2`) are the visible residuals. The tail literal-pool
placement (`1302dc` vs `130330`) also remains.

## 5. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6336 aligned= 1759 size=9908/9908 insn=3542/3532
cmake --build cmake-build-match --target check   # RATCHET PASS
cmake --build cmake-build-match --target drift   # 0 field(s)
```

Scratch: `/home/debian/GalaxyOnFire2/scratch/w12611/` — `Radar.base.cpp`
(6,339 base), `harness.py`, `expect_sweep.py`, `addexpect_sweep.py`,
`alias_sweep.py`, `cond_sweep.py`, `cond_big_sweep.py`, `stmt_sweep.py`,
`subexpect_sweep.py`, `expr_sweep.py`, `type_sweep.py`, `alt_forms.py`,
`invert_sweep.py`, and the `*.json` measurement dumps.
