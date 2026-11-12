# Radar::draw wave 191 (w13488) — branch/store order + literal-pool lens:
# enemy far-image hint, music predicate order, planet-loop load order

**Status:** landed, 2026-11-12. Worktree `wt/w13488`, branch `wave191-w13488`,
base `master` `f325e2d87`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave190-w13493.md` (asteroid-lock tail store, base
6,058), `-w13474.md` (statement-order/regalloc sweep), `-w13465.md` (gas-cloud
far tests), `-w13456*.md` (music tail / asteroid block).

## 0. The change — three independent, semantics-preserving edits

### (a) enemy far-image ladder: `carriesFriendCargoFlag` hinted likely

`Radar.cpp` ~line 826, inside the enemy far-image ladder:

```cpp
-                    } else if (__builtin_expect((enemy->player->carriesFriendCargoFlag), 0)) {  // lint: arm_builtin
+                    } else if (__builtin_expect((enemy->player->carriesFriendCargoFlag), 1)) {  // lint: arm_builtin
```

The original lays this branch out as the fall-through arm (the `image_0x94 /
image_0x68` pair is reached without a taken branch), i.e. the compiler treats
it as likely. Flipping the hint to `1` matches that layout and removes one
instruction from the ladder.

### (b) second music branch: `inAlienOrbit()` before `isAttackedByAliens()`

`Radar.cpp` ~line 1320:

```cpp
-                if (Globals::status->getStation()->isAttackedByAliens() || (Globals::status->inAlienOrbit())) {  // lint: arm_builtin
+                if (Globals::status->inAlienOrbit() ||
+                    Globals::status->getStation()->isAttackedByAliens()) {  // lint: arm_builtin
```

The original evaluates `inAlienOrbit()` first (`130396..1303ae`), then the
station query; the reversed source order forces the wrong `bl` sequence.

### (c) planet loop: `stationId` load before `currentStation`

`Radar.cpp` ~line 543 (planet-target loop, `campaign >= 2` block):

```cpp
             this->update(this->planetTargets->data()[i]);
+            int stationId = stations->data()[i];
             int currentStation = Globals::status->getStation()->getIndex();
             if (!this->onScreen) {
                 continue;
             }
-            int stationId = stations->data()[i];
```

The original loads the `stations[i]` element first and the
`getStation()->getIndex()` pair second; the source order was transposed.

## 1. Measured result

| metric | base (w13493) | after |
|---|---:|---:|
| raw same-address diff | 6,058 | **6,058 (neutral)** |
| instruction-index alignment | 2,051 | **1,976 (-75)** |
| function size | 9,908 / 9,908 | 9,908 / 9,908 |
| instruction count | 3,542 / 3,532 | **3,541 / 3,532** |
| `.text` differing bytes (whole `.so`) | 71,811 | 71,811 (neutral) |
| `.ARM.exidx` differing bytes | 0 | 0 |
| `.ARM.extab` differing bytes | 575 | 575 (no growth) |
| `.note.gnu.build-id` | 20 | 20 |
| total (text+extab+build-id) | 72,406 | 72,406 (neutral) |
| `.rel.plt` | byte-identical | **byte-identical** |

`RATCHET PASS`: byte_exact 4,507 / linked_exact 4,507 / size 4,524 / data 843 /
imports 207 / lint CLEAN / drift 0 / parity 5,370.

This is an alignment win, not a raw-byte win: the three edits rotate the
planet-loop prologue and the music/enemy arm layout toward the original
instruction sequence (aligned 2,051 -> 1,976, one fewer instruction), while
the same-address byte count and every section delta stay flat.

## 2. Reproduction

```sh
ulimit -v 12000000
python3 /home/debian/GalaxyOnFire2/scratch/w13488/fast.py \
  src/game/weapons/Radar.cpp /home/debian/GalaxyOnFire2/scratch/w13488/v.so
# -> (6058, 1976, 9908, 9908, 3541, 3532)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6058 aligned= 1976 size=9908/9908 insn=3541/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

## 3. Negative results (all reverted; useful for the next pass)

- Music mask reorder (`masksecond`, `noexp_masksecond`, `noexp_maskthird`,
  `noexp_maskfirst`, `exp_whole`, `inner_exp`, `inner_exp_mask2`, `noexp`,
  `noexp_mask2`): all worse or size-fatal (best 6,062; several 9,900/9,904 B).
- Music ladder id swap (`ladder_ab`: 151<->140 in the `<3`/`<5` arms):
  6,059 / 2,051 / 9,908 (raw +1); each half alone is size-fatal (9,904).
  Stacking it on the landed three edits gives 6,059 / 1,976 (raw +1), so it
  was rejected.
- Prologue store order: `setcolor_first` 6,089; `swap_stores` 6,060;
  `setcolor_mid` 6,072; `false_stores` neutral.
- Decl permutation of `turretMode`/`alienOrbit`/`campaign`: only the base
  order (0,1,2) gives 6,058; others 6,065-8,562 (one size-fatal 9,900).
- `missionBlocksPlanets` init forms (`decl_only`, `after_mission`,
  `zero_var`, `copy_init`) neutral; `bool_init` 7,651/9,916.
- `delta` forms (`paren`, `auto`, `len`, `delta2`) neutral; `split`
  8,724/9,932; `inline` 8,473/9,896; `delta_first`/`cx_after_delta` 6,083;
  `cx_cy_inside` 6,104.
- `camZ` variants: `nocamz` 6,175; `inside` 6,178; `inside2` 6,175.
- `autoPilotTarget` as `volatile int` -> 9,249 / 9,876 (size-fatal);
  `auto targets`/`auto system` neutral.
- `mission` guard: `noexp` 6,059; `exp0` 9,550/9,892; `last_exp` 6,798.
- `campaign >= 2` guard: `exp1` 8,637/9,920; `exp0` 9,100/9,972; `gt1`
  neutral; `unsigned` 6,059.
- 30-site exhaustive `__builtin_expect` hint flip on top of the landed three
  edits: best remains 6,058 / 1,976; only site 11 flip gives 6,060 / 1,977;
  sites 12/15 flips are neutral 6,058 / 1,976.
- Branch-weight sweep
  `-mllvm -likely-branch-weight=N -mllvm -unlikely-branch-weight=1`:
  N=800/720/740/795/810 give 7,488 / 2,012 / 9,908; all others size-fatal or
  worse. Default 2000 = 6,058.
- `-Xclang -target-feature -Xclang -cheap-predicable-cpsr` (cpsr feature
  off) -> 6,258 / 2,006 / 9,908.

## 4. Residual (unchanged; for future passes)

Global register-allocation rotation: original `campaign` in `sl`,
`missionBlocksPlanets` in `fp`, planet-loop `i` in `fp`, `stations` in `sl`
then spilled, `marked` in memory at `sp+132`, `mode` at `sp+100`. Ours:
`missionBlocksPlanets` in `sl`, `campaign` spilled at `sp+144`, `i` in `sl`,
`stations` spilled at `sp+132`, `marked` in `fp`, `mode` at `sp+104`. This
drives the `12e3c6`-`12ef55` cascade. `campaign` is `%stack.45` (single-use,
low spill weight); `missionBlocksPlanets` zero vreg `%2487`/`%2732` wins `sl`
in ours. MIR dumps in `/home/debian/GalaxyOnFire2/scratch/w13488/`:
`mi2.txt` (Greedy for `Radar::draw` starts line 403573; VReg rewriter
412840), `vreg_full.txt`, `greedy_draw.txt`, `sched_draw.txt`.

Other known structural residuals: `12fc20`-`12fd20` asteroid wreck x/y branch
layout; `12f7xx` literal pool (one word differs); `130250` music mask
(`sub #151` vs original `sub #136; cmp #15`); `130580` music tail
(`movs r1,#1; b.w` paths / `play` ordering).
