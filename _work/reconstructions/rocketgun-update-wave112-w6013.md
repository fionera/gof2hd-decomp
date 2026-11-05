# `RocketGun::update(int)` — wave 112 (w6013)

**Status:** dead end, no source change landed. Worktree `wt/w6013`, branch
`wave112-w6013`, base `5d98bd391`.

Target `_ZN9RocketGun6updateEi`: original **1908 B / 635 insns**.

| | pct | size | insns | gate |
|---|---:|---:|---:|---|
| baseline (wave-110 shape) | **93.8** | 1908 | 636 | PASS |
| best variant found this wave | 93.8 (tie) | 1908 | 636 | PASS |

```
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.92
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

No section/extab change: 27 sections, file size 2,187,328 B, `.ARM.extab`
0x26294. Only this note is added.

---

## 1. The one remaining blocker: `elapsed` r4↔r5

The normalised-stream diff of the baseline is dominated by a single
register swap:

| original | ours |
|---|---|
| `mov r4, r1` (entry, `elapsed`) | `mov r5, r1` |
| `mov r5, r0` (`player`) | `mov r4, r0` |
| `strd r4, sl, [sp,#16]` | `strd r5, sl, [sp,#16]` |
| `ldr r4, [sp,#16]` ×3 | `ldr r5, [sp,#16]` ×3 |
| `subs r0, r1, r4` | `subs r0, r1, r5` |
| `vmov s0, r4` | `vmov r0, s16` |

Everything else follows from it, including the muzzle-block encoding:

| original | ours |
|---|---|
| `add.w r8, sp, #116` (4 B) | `add r4, sp, #116` (2 B) |
| `ldr.w r6, [sl,#24]` hoisted before `MatrixGetDir` | `ldr.w r0,[sl,#24]` after it |
| `mov r0, r6` (2 B) | (load, 4 B) |

Because `r4` is free in our build, the LEA uses the narrow `add r4` and the
`geometry` load is not hoisted: the muzzle block is **4 B shorter**. This is
the *only* size difference before the mid-function constant pool.

## 2. Why the swap happens (greedy-allocator trace)

`-mllvm -print-after-all -mllvm -filter-print-funcs=_ZN9RocketGun6updateEi`
(the exact NDK clang 7.0.2) gives the greedy trace. `%28 = elapsed`
(`[16r,9552B)` + `[9744B,15104B)`, weight **8.74e-4**) is allocated first and
gets `$r4`; `%27 = this` gets `$r5`. Then, in the **loop preheader** (bb.34),
the hoisted stack-address LEAs evict it:

```
selectOrSplit GPRnopc:%245 [9200r,9552B) weight:7.94e-03   ; t2ADDri %stack.4
  $r8 is available at cost 1
  should evict: %28 weight:8.74e-04
  evicting $r4 interference: Cascade 1
  assigning %245 to $r4
```

`%245` is `t2ADDri %stack.4, 0` — one of five loop-invariant frame-index LEAs
(`%245 %246 %249 %250 %298`) that `MachineLICM` hoists to the preheader.
`r8..r12` have `CostPerUse = 1` (`ARMRegisterInfo.td`), so `tryEvict` prefers
evicting the much lighter `elapsed` from the cost-0 `r4` over using `r8`.
`elapsed` is then evicted again by `%384` (Cascade 4) and only lands in `r5`
in the second round.

Confirmation: compiling the same source with `-mllvm -disable-machine-licm`
gives `mov r4, r1` at the entry (but the whole function changes: 1892 B,
48.7 %). The original does **not** hoist the LEAs — its loop preheader is only
8 instructions (`0x15f15c..0x15f17c`) and the LEAs are rematerialised inside
the loop (`add r6, sp, #128` at `0x15f29e`, `add r5, sp, #40` at `0x15f2b4`,
…). So the original's allocator pulled the LEAs back down; ours keeps them.

## 3. The `dt` / z-arm coupling (why the faithful shape scores *lower*)

The original converts `dt` at the loop preheader (`vmov s0,r4; vcvt; str
r0,[sp,#24]`, `0x15f15c`) and stores `trailTimers[cur] = 0` **after**
`resetSystem` in the `z == 50000` arm (`0x15f6a` after `0x15f58`). Both are
faithful, and both change the code size *before* the mid-function pool:

| variant | pre-pool delta vs original | pool at | pct |
|---|---:|---|---:|
| baseline (dt early, z tuned) | 0 (dt vcvt +8, muzzle −4, z −4) | `0x15f044` | **93.8** |
| `dt` late, z tuned | −8 | `0x15f03c` | 88.9 |
| `dt` late, z faithful | −4 | `0x15f040` | 88.4 |
| `dt` early, z faithful | +4 | `0x15f048` | 88.8 |

The original is **dt late + z faithful + muzzle +4 B**, i.e. exactly the
faithful shape *with `elapsed` in `r4`*. The three deltas cancel. So the
`r4/r5` swap is the single key that unlocks both the `dt` placement and the
literal-pool address (`ldr r0,[pc,#856]`, `vldr s0,[pc,#784]`, …), which is
why the faithful variants currently lose 5–10 points: every PC-relative pool
reference shifts by 4–8 B.

## 4. Search performed (all measured, all ≤ 93.8 %)

Every variant was compiled with the exact match flags and scored with
`tools/verify/verify.py`'s `disassemble`/`reconcile`/`score` on
`_ZN9RocketGun6updateEi`.

### 4.1 New this wave

| variant | pct | size | note |
|---|---:|---:|---|
| 200 valid permutations of the 12-statement muzzle block | — | — | **none** produced `mov r4, r1`; entry stays `mov r10,r0 / mov r5,r1` (89 orders), `mov r5,r0 / mov r10,r1` (37+22), `mov r8,r1` (4) |
| 96 combinations of {dt early/late} × {z tuned/faithful} × {4 muzzle decl orders} × {3 `up` forms} × {2 cluster forms} | — | — | none produced `mov r4, r1` |
| 16 combinations of {dt} × {z} × {`AEGeometry *geom` local before `MatrixGetDir`} | — | — | none |
| `Vector tmp = MatrixRotateVector(...); muzzle += tmp;` (3 forms) | — | — | entry unchanged |
| `Vector dir = forward; setDirection(dir, up)` (2 forms) | — | — | entry unchanged |
| explicit `up.x/y/z = …` | — | — | entry unchanged |
| `if (player != nullptr) { … }` / `if (player) { … }` | — | — | entry unchanged |
| `if (!player) return;` (`n_null`) | 89.8 | 1916 | **entry `mov r4,r1`** but then `mov r5,r4`; +8 B |
| `n_null` + dt late (`n_null_dt`) | 92.2 | 1916 | entry `mov r4,r1`, then `mov r5,r4` |

### 4.2 Earlier variants re-measured (from `/dev/shm/w6013`, 559 files)

Only 7 of 559 produce `mov r4, r1`:

| variant | entry | pct | size |
|---|---|---|---:|
| `n_null`, `n_null_dt`, `nn_ret`, `nn_twice` | `r10,r0 / r4,r1 / r5,r4` | 89.8–92.2 | 1916 |
| `diag_loopfade` (`elapsed`→`16` in loop fade) | `r8,r0 / r4,r1` | 43.2 | 1872 |
| `vol_e`, `vol_e_all` (`volatile int e = elapsed`) | `r8,r0 / r4,r1` | 43.3 | 1876 |

`diag_loopfade`/`vol_e` shrink the frame to 152 B (the `[sp,#16]` elapsed
spill disappears), so they are not usable.

Representative scores from the earlier search (all 1908 B unless noted):
baseline 93.8; `comb_000` 93.8; `x_p_inside` 93.8; `z_c_v3direct` 93.8;
`y_t_nogun2` 93.8; `loop_no1/2/12` 93.8; `ahead2` 93.8; `x_p_top` 93.5;
`dt_top` 93.3; `x_p_ship` 93.2; `v_b_geom` 93.2; `comb_100` 88.9;
`v_dtlate` 88.9; `comb_010` 88.8; `comb_110` 88.4; `z_c_plainassign` 89.5;
`z_c_ref` 89.5; `comb_001` 56.8; `z_c_plain` 56.8 (1872 B, 623 insns — the
`V3 crossBuf` copy is load-bearing: without it the whole allocation collapses
and `this` moves to `r11`).

## 5. Conclusion / next lever

The residual is a **register-allocation artefact, not a source-shape
difference**. The original source is almost certainly the faithful shape
(`dt` before the loop, `trailTimers` store after `resetSystem`, muzzle block
as-is); it only differs in that LLVM 7.0.2's greedy allocator *rematerialised*
the five loop-invariant frame-index LEAs instead of hoisting them into
registers. That decision is made by `RAGreedy` after `MachineLICM` and is
driven by `elapsed`'s spill weight (`8.74e-4 = UseDefFreq/(Size+50)`,
`Size = 14848`) versus the LEAs' (`7.94e-3 = 3/(352+50)`).

A byte-exact reconstruction therefore needs a source shape that changes
`elapsed`'s live-range size or the LEAs' weights — e.g. one fewer/more use of
`elapsed` in a block that changes the split, or a loop body whose register
pressure makes `IsProfitableToHoist`/`CanCauseHighRegPressure` decline the
hoist. Statement reordering alone (≈600 variants tried across waves 110–112)
does not move it.

## 6. Reproduction

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6          # RATCHET PASS
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN9RocketGun6updateEi
```

RA trace: add `-mllvm -print-after-all -mllvm
-filter-print-funcs=_ZN9RocketGun6updateEi` to the TU compile line
(`/dev/shm/w6013/dump.sh` does this). Scratch for this wave lived in
`/dev/shm/w6013` (`base.cpp`, `fast2.sh`, `gen_muzzle.py`, `mz_*.cpp`,
`cb_*.cpp`, `cc_*.cpp`, `dd_*.cpp`, `ee_*.cpp`, `all_res.txt`); none of it is
in the repo.
