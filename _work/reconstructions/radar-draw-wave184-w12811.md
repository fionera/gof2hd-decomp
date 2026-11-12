# Radar::draw wave 184 (w12811) — expect/alias re-sweep on the 6,315 body + commutative hull-bar multiply (raw 6,315 -> 6,311)

**Status:** landed, 2026-11-12. Worktree `wt/w12811`, branch `wave184-w12811`,
base `master` `4c06c2662` (wave-183 harvest). Target
`src/game/weapons/Radar.cpp`, `_ZN5Radar4drawEP6PlayerP3Hudi`
(`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave183-w12709.md` (second `lockedEnemy` cleanup revert),
`-w12711.md` (arithmetic-order/store-scheduling cells) and
`radar-draw-wave182-w12609/10/11.md` (planet-lock body, `radarFont()` hoists,
`hasTractor13` inversion). This note is self-contained: every number was
measured in this worktree on the 6,315 base.

## 0. Baseline and landed result

| metric | base | landed |
|---|---:|---:|
| function size | 9,908 | **9,908** |
| raw same-address diff | 6,315 | **6,311** (-4) |
| instruction-index alignment | 1,858 | 1,903 |
| instruction count | 3,544 ours / 3,532 orig | 3,544 / 3,532 |
| whole-`.so` `.text` differing bytes | 74,842 | **74,838** (-4) |
| `.ARM.exidx` differing bytes | 0 | 0 |
| `.ARM.extab` differing bytes | 646 (size/base exact at `0x1a3b78`, 156,312 B) | **646** (unchanged) |
| `.rel.plt` | byte-identical | **byte-identical** (diff 0) |
| `.rel.dyn` / `.rodata` / `.data` / `.dynsym` / `.dynstr` / `.hash` / `.got` | 0 | 0 |
| `.note.gnu.build-id` differing bytes | 20 | 20 |
| file size | 2,187,328 | 2,187,328 |
| total differing bytes | 75,508 | **75,504** |

Gate after the change: byte_exact **4,507/4,524**, linked_exact
**4,507/4,524**, size-exact **4,524/4,524**, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data_exact **843/843**, imports **207**, lint **CLEAN**,
drift **0**, parity **5,370/5,370**. `RATCHET PASS`.

## 1. Landed change (one commutative multiply operand swap, net size 0)

In the enemy hull-bar block (line ~746):

```diff
-                        float hull = (float) enemy->player->getDamageRate() / 100.0f * (float) this->field_0x1c8;
+                        float hull = (float) this->field_0x1c8 * ((float) enemy->player->getDamageRate() / 100.0f);
```

This is the same value: IEEE-754 multiplication is commutative, so
`(rate / 100.0f) * max` and `max * (rate / 100.0f)` are bit-identical (unlike
the wave-183 w12711 planet-frame cell, this one does not change the rounding
order). The original disassembly at `12f274`/`12f284` is `vdiv s0,s0,s22`
(`s22 = 100.0f`) then `vmul s0,s0,s2` (max), i.e. the value order is preserved;
only which VFP register holds the `max` operand changes, which lets the
surrounding `DrawRegion2D` argument setup schedule four more bytes in place.

This cell was neutral (6,322) on the wave-183 w12711 base; the three w12711
cells and the w12709 cleanup revert moved the surrounding enemy-loop block so
that it is now a -4 raw win.

## 2. Expect / alias sweep on the current body (no other win)

Every measurement is a full incremental `link_so -j2` + `tools/fn_diff.py`
(`ulimit -v 12000000`). Scratch:
`/home/debian/GalaxyOnFire2/scratch/w12811/`.

### 2.1 Existing `__builtin_expect` remove/flip (64 variants)

All 32 sites removed and weight-flipped. No single edit beats 6,315.
Neutral (6,315, size exact): `rm4`, `rm12`, `rm15`, `fl12`, `fl15`, `fl17`.
`fl17` is raw-neutral but drops instruction count 3,544 -> 3,543 and aligned
1,858 -> 1,834; combined with the landed hull cell it stays 6,311 / 3,543.

### 2.2 Plain `if`/`while` conditions wrapped whole (170 x 2 = 340)

Every condition without an existing expect wrapped at weight 0 and 1. No win;
best neutral 6,315.

### 2.3 Top-level `&&`/`||` operands wrapped (281 x 2 = 562)

Every top-level operand of every condition wrapped at weight 0 and 1. No win;
best neutral 6,315.

### 2.4 Local-alias removal (47 successful variants)

Every local declaration in `draw` inlined into its enclosing block (single-
and multi-use; assigned locals skipped). No win; best neutral 6,315
(`size`/`left`/`top`/`topOk` in the landmark loop, `size`/`left` in the planet
loop, `total` in the enemy tail, `size`/`top` in the gas block, and the music
tail `current`). Nearest worse: `techFont` 6,316, `lmFont` 6,317,
`target` 6,316.

### 2.5 Pair/triple combinations of the neutral expects (35) and neutral
expect x neutral alias (60) and near-miss x neutral (119)

No combination below 6,315. `fl12+fl17`, `fl15+fl17` and
`fl12+fl15+fl17` are 6,315 (aligned 1,834, insn 3,543).

### 2.6 Other cell classes re-measured on the 6,315 body

* if/else inversions (44 brace-brace blocks): no win (all 6,315 or worse).
* adjacent same-indent statement swaps (68 pairs): no win.
* arithmetic-order cells (wave-183 class): the landed hull swap is the only
  winner. `max * rate / 100` is also 6,311 but changes rounding; the
  commutative `max * (rate/100)` form is preferred. EMP commutative swap
  (6,312), EMP `rate*max/100` (6,312), station frame `timer*count/field`
  (6,315), enemy frame `elapsed*frames/total` (6,315), gas frame
  `last*(timer/field)` (6,315), asteroid frame `timer*count/field` (6,634
  size 9,904), `field_0x73=0` before `DrawImage2D` (6,318).
* declaration/type cells: `missionBlocksPlanets` init `0`/`uint8_t`/`char`
  neutral; `bool` 6,428 size 9,916; `campaign` `unsigned`/`uint32_t` 6,316,
  `long` neutral; moving `campaign` down to its use 9,219 size 9,904;
  moving the `alwaysEnemySeen`/`alwaysEnemy` initialisation to `enemies:`
  neutral.

## 3. Remaining residual

Unchanged from wave 183: the original keeps `campaign` in `sl`
(`12e0f8 mov sl,r0`, `12e38a cmp.w sl,#2`) and reuses `sl` for the stations
array, with the planet-loop index in `fp`; ours spills `campaign` to
`sp+0x90`, keeps `stations` at `sp+0x84` and uses `sl` for the loop index.
The stack-slot rotation and the `bossSeen` `subs/clz/lsrs` vs `it eq; moveq`
materialisation follow from that cascade. The music-tail cold-block layout
(ours places the `field_0x1bc < 1` else block after the epilogue) and the tail
literal-pool placement (`1302dc` vs `130330`) also remain.

## 4. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6311 aligned= 1903 size=9908/9908 insn=3544/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
cmake --build cmake-build-match --target drift -j2   # 0 field(s)
```

Scratch: `/home/debian/GalaxyOnFire2/scratch/w12811/` — `Radar.base.cpp`
(6,315 base), `harness.py` (build+measure helper), `expect_sweep.py`,
`alias_sweep.py`, `cond_sweep.py`, `subexpect_sweep.py`, `invert_sweep.py`,
`stmt_sweep.py`, `combo*.py`, `heat.py`, `secdiff.py`, and the
`*_results*.json` dumps.
