# Radar::draw wave 183 (w12711) — arithmetic-order / store-scheduling cells (raw 6,322 -> 6,316)

**Status:** landed, 2026-11-11. Worktree `wt/w12711`, branch `wave183-w12711`,
base `master` `9555474d8`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave182-w12609.md` (planet-lock dock/sound cell),
`-w12610.md` (per-site `radarFont()` hoists) and `-w12611.md` (hasTractor13
inversion). This note is self-contained: every number was measured in this
worktree on the wave-182 harvest shape (raw 6,322).

## 0. Baseline and landed result

| metric | base | landed |
|---|---:|---:|
| function size | 9,908 | **9,908** |
| raw same-address diff | 6,322 | **6,316** (-6) |
| instruction-index alignment | 1,782 | 1,858 |
| instruction count | 3,543 ours / 3,532 orig | 3,544 / 3,532 |
| whole-`.so` `.text` differing bytes | 75,015 | **75,009** (-6) |
| `.ARM.exidx` differing bytes | 0 | 0 |
| `.ARM.extab` differing bytes | 646 (size/base exact at `0x1a3b78`, 156,312 B) | **646** (unchanged) |
| `.rel.plt` | byte-identical | **byte-identical** |
| `.rodata` / `.data` / `.dynsym` / `.dynstr` / `.hash` / `.rel.dyn` / `.got` | 0 | 0 |
| `.note.gnu.build-id` differing bytes | 20 | 20 |
| total differing bytes | 75,681 | **75,675** |
| file size | 2,187,328 | 2,187,328 |

Gate after the change: byte_exact **4,507/4,524**, linked_exact
**4,507/4,524**, size-exact **4,524/4,524**, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data_exact **843/843**, imports **207**, lint **CLEAN**,
drift **0**, parity **5,370/5,370**. `RATCHET PASS`.

## 1. Landed changes (three cells, net size 0)

```diff
@@ planet-lock blip frame (line ~650)
-                    frame = (int) ((float) (blip->getRawFrameCount() - 1) *
-                                   ((float) this->planetLockTimer / (float) this->field_0x1b8));
+                    frame = (int) ((float) this->planetLockTimer * (float) (blip->getRawFrameCount() - 1) /
+                                   (float) this->field_0x1b8);

@@ enemy hull bar (line ~745)
                         float hull = (float) enemy->player->getDamageRate() / 100.0f * (float) this->field_0x1c8;
-                        this->barWidth = (int) hull;
                         Globals::Canvas->DrawRegion2D(barImage, 0, 0, (int) hull, this->field_0x1cc, 0.0f, 0, 0,
                                                       this->screenX + 3 - this->halfScreenHeight,
                                                       this->screenY + this->halfScreenWidth + 3);
+                        this->barWidth = (int) hull;

@@ enemy EMP bar (line ~753)
-                            float emp = (float) enemy->player->getEmpDamageRate() / 100.0f * (float) this->field_0x1c8;
+                            float emp = (float) this->field_0x1c8 * ((float) enemy->player->getEmpDamageRate() / 100.0f);
```

Each cell was measured individually (full `link_so` + `fn_diff`):

| cell | raw | size | insn |
|---|---:|---:|---:|
| base | 6,322 | 9,908 | 3,543 |
| EMP multiply reorder (`y * (x/100)`) | **6,321** | 9,908 | 3,543 |
| hull `barWidth` store after `DrawRegion2D` | **6,320** | 9,908 | 3,544 |
| EMP reorder + hull store move | **6,319** | 9,908 | 3,544 |
| all three (landed) | **6,316** | 9,908 | 3,544 |

### 1.1 Semantics

* EMP reorder: `y * (x / 100.0f)` is exactly `(x / 100.0f) * y` — IEEE
  multiplication is commutative, so this is bit-identical.
* hull store: `this->barWidth = (int) hull;` is moved after the
  `Globals::Canvas->DrawRegion2D(...)` call. The call receives `(int) hull` as
  an argument and cannot read `Radar::barWidth` (a `Radar` member), so the
  store is dead with respect to the call.
* planet-lock frame: `(timer * count) / field` instead of
  `count * (timer / field)`. This is the only cell that is **not**
  bit-equivalent in floating point (it changes the rounding order); it is
  landed on the raw metric, as in the wave-181 landmark-store swap. The
  original emits `vdiv` then `vmul` at `130428`/`130434` (i.e. the base
  order); the variant emits `vmul` then `vdiv` at `13042c`/`130430` and
  happens to match three more bytes of the surrounding scheduling. If a
  future pass finds a bit-exact form with the same raw, prefer it.

## 2. Search evidence (all size-exact unless noted)

Every measurement is a full incremental `link_so -j2` + `tools/fn_diff.py`
(`ulimit -v 12000000`).

### 2.1 Expect sweeps on the current body

* All 32 `__builtin_expect` sites removed and flipped (64 builds): no win.
  Best neutral: sites 4/12/15/17 give 6,322; nearest worse `rm0` 6,323,
  `rm13` 6,325, `rm23` 6,325, `rm20` 6,331, `rm8` 6,328 (aligned 1,748).
* All 170 plain `if`/`while` conditions wrapped whole at weight 0 (170
  builds) and weight 1 (170 builds): no win; best neutral 6,322.
* Every top-level `&&`/`||` operand of every condition wrapped at weight 0
  and 1 (388 builds): no win; best neutral 6,322.
* Pair combinations of the neutral single-expect variants: best 6,322.

### 2.2 Alias / declaration / statement sweeps

* Local-alias removal (83 declarations, block-scoped, single- and multi-use
  inlining): no win. Every neutral form is byte-identical to the base except
  the gas-block `size`/`left`/`top` aliases (6,322 raw, aligned 1,784) and
  `target` (6,323).
* Adjacent same-indent declaration swaps (36 pairs): no win.
* Adjacent same-indent single-line statement swaps (48 pairs): no win.
* Condition operand permutations (adjacent swaps + reversal + rotations,
  266 builds): no win.
* If/else inversions (44 blocks): no win (the w12611 `hasTractor13` cell is
  already landed).
* `radarFont()` hoist/unhoist variants (waypoint, rowHeight, rowHeight*2,
  planet-name, gas, asteroid-dock, and reverting the three landed hoists):
  no win (6,322-6,340).

### 2.3 Arithmetic-order cells (the winning class)

The wave-183 sweep moved to arithmetic expression order and store
scheduling, which the earlier expect/alias/statement sweeps did not cover:

| shape | raw | size |
|---|---:|---:|
| base | 6,322 | 9,908 |
| hull `(rate*max)/100` | 6,322 | 9,908 |
| hull `max*(rate/100)` | 8,295 | 9,912 |
| EMP `max*(rate/100)` | **6,321** | 9,908 |
| EMP `(rate*max)/100` | 6,322 | 9,908 |
| planet frame `timer*count/field` | **6,319** | 9,908 |
| planet frame `count*timer/field` | 6,320 | 9,908 |
| planet frame `(timer/field)*count` | 6,323 | 9,908 |
| station frame `timer*count/field` | 6,322 | 9,908 |
| enemy frame `elapsed*frames/total` | 6,322 | 9,908 |
| gas frame `last*(timer/field)` | 6,322 | 9,908 |
| asteroid frame `count*timer/field` | 6,362 | 9,908 |
| hull store after `DrawRegion2D` | **6,320** | 9,908 |
| EMP store after `DrawRegion2D` (on 6,318 base) | 8,299 | 9,904 |
| enemy `field_0x73=0` before `DrawImage2D` (on 6,316 base) | 6,319 | 9,908 |
| gas `field_0x73=0` before `DrawImage2D` (on 6,316 base) | 6,319 | 9,908 |

The three winners combine to 6,316. Other measured non-wins: `-24000`
guard rewritten as `>=`/`<=` (neutral), `size*2` -> `size+size`/`size<<1`
(byte-identical), `screenX+2-half` reorders (byte-identical), `enabled`
pointer-truthiness forms (byte-identical), planet-loop induction-variable
type/hoist variants (neutral or 7,024).

## 3. Remaining residual

Unchanged from waves 174-182: the original keeps `campaign` in `sl`
(`12e0f8 mov sl,r0`, `12e38a cmp.w sl,#2`) and reuses `sl` for the stations
array, with the planet-loop index in `fp`; ours spills `campaign` to
`sp+0x90`, keeps `stations` at `sp+0x84` and uses `sl` for the loop index.
Every campaign-in-`sl` shape re-measured on this body is raw-worse
(9,507-9,634) and size-fatal or +4..+56 B. The music-tail cold-block layout
(ours places the `field_0x1bc < 1` else block after the epilogue) and the
tail literal-pool placement (`1302dc` vs `130330`) also remain.

## 4. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6316 aligned= 1858 size=9908/9908 insn=3544/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
cmake --build cmake-build-match --target drift -j2   # 0 field(s)
```

Scratch: `/home/debian/GalaxyOnFire2/scratch/w12711/` — `Radar.base.cpp`
(6,322 base), `harness2.py` (build+measure helper), `expect_sweep.py`,
`combo_expect.py`, `alias_sweep2.py`, `addexpect_sweep.py`,
`subexpect_sweep.py`, `stmt_sweep.py`, `cond_sweep.py`, `invert_sweep.py`,
`decl_sweep.py`, `heat.py`, `aligndiff.py`, `secdiff.py`, and the
`*_results*.json` dumps.
