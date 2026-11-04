# `RocketGun::update(int)` — pass 13 (w4910, wave 99)

**Status:** research note, 2026-11-04. Worktree `wt/w4910`, branch `wave99-w4910`,
base `44225192c`. **No source change landed**: ~50 new source shapes this pass all
score <= 86.9 %. Tree clean, gate unchanged (byte 4500, linked 4500, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0).

Target `_ZN9RocketGun6updateEi`: original **1908 B / 635 insns**, ours
**1908 B / 636 insns**, official verify score **86.9**.

Earlier notes: `rocketgun-update-w4812.md` (pass 11+12, the slot-weight model),
`-w4710` (pass 10, frame-slot map + regalloc trace), `-w4512` (pass 9),
`-w4412` (pass 8, Ghidra decomp appendix), `-w4265`, `-w4248`, `-w4216`,
`-w4111`, `-w4011`, `-w3831`, `-w3811`.

This pass contributes four things the earlier notes did not have:

1. a **register-normalised structural diff** that shows the residual is *one*
   register permutation plus its downstream consequences (section 2);
2. the **exact `CalcSpillWeights` decomposition** of `%29` (elapsed) and `%251`
   (the hoisted `%stack.4` address) from `WDBG`, and the resulting
   `shouldEvict` arithmetic (section 3);
3. the proof that the **`%251` eviction of `%29` is not the lever it looked
   like**: forcing `%251` off `r4` (`VRW=2147483677:20`, `NOEVICT=1`) leaves the
   final code *byte-identical* (section 3.3);
4. a **correction to the pass-12 weight model**: the original's `this` slot is
   *not* 12.52; it also has a `for.body` reload at `15f19c` (block freq 31.0),
   so its weight is ~43 (section 4).

---

## 1. Baseline (NDK r18b clang 7.0.2, this worktree)

```
cmake --build cmake-build-match --target check -j8
```
```
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`_ZN9RocketGun6updateEi`: 1908 B both sides, **635 vs 636** instructions, **86.9 %**.

Frame slots (confirmed again from `-mllvm -debug-only=stack-slot-coloring` plus
`-mllvm -debug-only=prologepilog`):

| `[sp,#N]` | original | ours |
|---|---|---|
| 4  | stack-guard copy | stack-guard copy |
| 8  | **`s` (Sinf result)** | `c` (Cosf result) |
| 12 | **`c`** | **`elapsed`** |
| 16 | **`elapsed`** | **`trailMatrices` local** |
| 20 | `this` | `this` |
| 24 | `dt` | `dt` |

PEI assigns `FI(32)->[sp,#24]`, `FI(33)->[sp,#20]`, ... `FI(37)->[sp,#4]`
(`alloc FI(32) at SP[-200] ... FI(37) at SP[-220]` with the 224-byte outgoing
frame), so the *heaviest* stack-slot-coloring interval still gets the *highest*
offset. The pass-12 mechanism is confirmed.

---

## 2. Register-normalised structural diff (new, complete)

Replacing every register with `R`, every immediate with `N` and every branch
target with `H`, `difflib` on the two 645/646-instruction streams gives the
**complete** list of non-register differences:

| # | original | ours | note |
|---|---|---|---|
| 1 | `add.w R, sp, #116` + `ldr.w R, [sl, #24]` | `add R, sp, #116` | MatrixGetDir sret ptr; geometry load elided |
| 2 | `strd r4, sl, [sp, #16]` | `str r5,[sp,#12]` + `str.w sl,[sp,#20]` | elapsed+this spill; **no `strd`** |
| 3 | — | `str r4, [sp, #28]` | `volatile int padx` anchor |
| 4 | `movs r4, #0` (cursor init) | `mov.w fp, #0` (order) | preheader instruction order |
| 5 | — | `str r5,[sp,#28]` + `ldr.w r0,[sl,#216]` + `str r0,[sp,#16]` | `unused` anchor + `trailMatrices` local |
| 6 | `add.w sl, sp, #116` | `add r6, sp, #116` | op* temp address register |
| 7 | `mov r9, r4` | — | cursor save across the cluster block |
| 8 | `ldr.w sl, [sp, #20]` | — | `this` reload after the temp clobber |
| 9 | `ldr.w r0, [sl, #216]` (x2) | `ldr r0, [sp, #16]` (+`ldr [r0,#4]`) | trail local vs member |
| 10 | `ldr.w r1, [r6, r8, lsl #2]` *after* uidiv | `ldr.w r6, [r2, r8, lsl #2]` *before* uidiv | `life` load hoisted |
| 11 | `subs r1, r4, r1` + `ldr r4, [sp, #24]` | `subs r1, r4, r6` | **dt reload placement** |
| 12 | `str r0, [sp, #8]` (s spilled) | `mov r6, r0` (s kept) | **`s` not spilled** |
| 13 | `ldr r0,[sp,#8]` + `vmov s0,r0` | `vmov s0, r6` | s reload vs register |
| 14 | — | `ldr r4, [sp, #24]` (after 2nd op*) | dt reload |
| 15 | `mov r4, r9` | — | cursor restore |
| 16 | `ldr.w r9, [r0, #12]` | `ldr r4, [r0, #12]` | positions base register |
| 17 | `add r5, sp, #128` | `add.w sl, sp, #128` | trail-block op* temp register |
| 18 | `add.w r1, r9, fp` | `add.w r1, r4, fp` | |
| 19 | `adds r1, r0, r4` (x2) | `add.w r1, r0, r9` (x2) | cursor register |
| 20 | `adds r4, #60` | `add.w r9, r9, #60` | cursor register (16-bit vs 32-bit) |
| 21 | `nop` before the literal pool | — | alignment |

Everything else is a branch-offset shift. So the residual is **one register
permutation**:

```
              original        ours
elapsed       r4              r5
player        r5              r4
cursor i*60   r4              r9
cross sret    r6              r4
op* tmp sret  r6 / sl         r6 / sl
s             spilled [sp,#8] r6
```

plus the `trailMatrices` local and the two anchors, which are what *create*
that permutation (they are not independent defects).

---

## 3. The `shouldEvict` arithmetic (new, exact)

`CalcSpillWeights` weight = `totalWeight / (size + 25*InstrDist)`, `InstrDist = 2`,
so `weight = totalWeight / (size + 50)`. `WDBG=1` (a hook in this LLVM build)
prints the per-instruction contributions for `RocketGun6update`:

### 3.1 `%29` = `elapsed` (raw vreg `2147483677`), size 14894

| ref | block freq | weight |
|---|---|---|
| `%29 = COPY $r1` (def, bb0) | 1.0 | 1.0 |
| `$r1 = COPY %29` (Gun::update arg, bb0) | 1.0 | 1.0 |
| `$r1 = COPY %29` (x2, bb16) | 0.19531 | 0.3906 |
| `%234 = t2SUBrr %11, %29` (bb32) | 0.15717 | 0.1572 |
| `%239 = COPY %29` (dt conversion, bb34) | 1.0 | 1.0 |
| `t2STRi12 %29, %stack.12` (volatile `unused`, bb34) | 1.0 | 1.0 |
| `%373 = t2SUBrr %24, %29` (fade, bb55) | 9.6875 | 9.6875 |
| **total** | | **14.23** |

`14.23 / 14944 = 9.52e-4` (trace: `9.400839e-04`).

### 3.2 `%251` = `t2ADDri %stack.4.ref.tmp, 0`, size 5518

| ref | block freq | weight |
|---|---|---|
| def (bb34) | 1.0 | 1.0 |
| `$r0 = COPY %251` (ahead op*, bb39 for.body) | 31.0 | 31.0 |
| `$r1 = COPY %251` (ahead op*, bb39) | 31.0 | 31.0 |
| `$r0/$r1 = COPY %251` (x2, bb51 trail) | 12.109 | 24.22 |
| `$r0/$r1 = COPY %251` (x4, bb49 cluster) | 3.6328 | 14.53 |
| **total** | | **101.75** |

### 3.3 The eviction, and why it is not the lever

`ra.log` (from `-mllvm -debug-only=regalloc`):

```
selectOrSplit rGPR:%29 ... weight:9.400839e-04
  hints: $r1 $r1 $r1 $r1   missed hint $r1
  assigning %29 to $r4
selectOrSplit GPR:%28 ... weight:1.479491e-02      assigning %28 to $r5
selectOrSplit GPR:%247 ... weight:8.627993e-03     assigning %247 to $r6
selectOrSplit GPRnopc:%251 ... weight:8.566814e-03
  $r8 is available at cost 1
  Only trying the first 9 regs.
  should evict: %29 ... weight:9.400839e-04
  evicting $r4 interference: Cascade 1
  unassigning %29 from $r4
  assigning %251 to $r4
...
selectOrSplit rGPR:%426 ... weight:1.357668e-03
  hints: $r1 $r1 $r1 $r1   missed hint $r1
  assigning %426 to $r5
```

`%426` is the spiller's copy of `elapsed` for the launch block. It takes `r5`
**because `%251` already holds `r4`**. To stop the eviction, `%29`'s weight
would have to exceed `8.57e-3`, i.e. `totalWeight > 128` — 9x its actual 14.2.
That is unreachable without adding ~4 hot-block uses of `elapsed`, which changes
the code.

**But the eviction is not the lever.** Two independent experiments:

* `VRW=2147483677:20` scales `%29`'s weight to `1.880168e-02`; the trace then
  does exactly what the model predicts — `%251` takes `$r8` ("available at
  cost 1") and `%29` keeps `$r4`. The **final code is unchanged** (86.9 %,
  `mov r5, r1`), because `%29` is later evicted again (by `%386`, the loop
  counter, weight `2.805497e-02`) and re-split.
* `NOEVICT=1` (disable `tryEvict` entirely) also leaves the code unchanged
  (86.9 %).

So the register permutation is a **fixed point of the whole greedy allocation**,
not a single `shouldEvict` comparison.

---

## 4. Correction to the pass-12 weight model

The pass-12 note modelled the original's `this` slot as
`1.0 + 0.625 + 3*3.6328 = 12.52`. That omits `15f19c ldr.w sl,[sp,#20]`, which
is inside the `for.body` block (`[15f17e, 15f32c)`, freq **31.0**; `15f32c` is a
branch target from `15f202`/`15f20e`/`15f23c`). The original's `this` slot
weight is therefore at least `1 + 31 + 3*3.6328 = 42.9`, still below `dt`'s
47.74 — so the ordering `dt > this > elapsed` still holds, but the margin is
much smaller than the note claimed.

The original's `elapsed` slot likewise has four `[sp,#16]` reloads
(`15efa6`, `15f016`, `15f02c`, `15f3e4`), not the two the note listed.

**What actually has to change** is unchanged: the `trailMatrices` slot (32.48)
must disappear from between `this` (36.74) and `elapsed` (11.31), and `s` must
spill so `c` moves to 12 and `elapsed` to 16. Both are consequences of the
`elapsed`->`r4` permutation.

---

## 5. Source variants tried this pass (~50, all <= 86.9)

Baseline **86.9**, 636 insns.

| variant | pct | note |
|---|---|---|
| baseline | **86.9** | |
| `cross` inlined into the `+=` | 60.0 | 628 insns |
| `cross` non-const / at function scope | 86.9 / 60.7 | |
| `s`,`c` declared then assigned | 86.9 | |
| `s * 2.0f` / `c * 2.0f` | 86.9 | |
| `phase` as float division | 59.0 | 630 |
| `total` / `n` / `life` via `this->gun->` (each and all) | 86.9 | |
| `upVectors` via the `gun` local | 86.9 | |
| `const Vector &vel` / `&upv` local | 86.9 / 84.6 | |
| `trailMatrices` local `const` | 86.9 | identical code |
| `trailMatrices` local before `dt` / `dt` after trail | 86.9 | |
| `this->trailMatrices` in the `!= nullptr` test | 68.8 | 635 insns, 1912 B |
| `this->trailMatrices` in the `== nullptr` test | 68.2 | 635 insns, 1912 B |
| `trailMatrices->data()[i]` for the first `MatrixSetTranslation` | 85.3 | |
| `(*trailMatrices)[i]` for the second access | 84.0 | 637 |
| `Matrix &trail = (*trailMatrices)[i]` for both | 53.1 | 1900 B |
| `heading` before the first `MatrixSetTranslation` | 57.9 | 630 |
| `heading` via the `gun` local | 86.9 | |
| `Vector ahead2` for the trail-block recompute | 77.6 | 1912 B |
| `while` loop instead of `for` | 86.5 | |
| `unused` as `unsigned` | 86.9 | |
| `unused` as `float` / `RocketGun*` / `Gun*` | 84.6 / 85.8 / 85.8 | |
| extra `volatile RocketGun *self = this` | 86.9 | |
| `volatile int u2 = elapsed` in the fade block | 83.3 | 1916 B |
| no `unused` / no `padx` / neither | 85.8 / 86.7 / 85.6 | |
| no trail local (any `unused` form) | 57.4-57.6 | 622-626 insns |
| `__builtin_expect` on `hitSmall` (0/1) | 40.3 / 66.2 | |
| `__builtin_expect` on the trail test (0/1) | 51.2 / **86.9** | |
| `__builtin_expect` on `trailMatrices == nullptr` (0/1) | 67.9 / 54.9 | |
| `__builtin_expect` on `active` (0/1) | 49.8 / 47.3 | |
| `__builtin_expect` on the cluster test (0/1) | 47.7 / 56.6 | |
| `__builtin_expect` on `ahead.z == 50000` (0/1) | 44.4 / 47.2 |
| `VectorCross` arguments swapped | 86.4 |
| `norm * dt * (s+s)` (dt before the scale) | 74.2 | 1912 B |
| `c` computed from `total - life` (same as `s`) | 53.2 | 621 insns |
| `Array<Matrix> *&trailMatrices = this->trailMatrices` (alias) | 57.6 | 1880 B, same as faithful | |

Register-allocation experiments (self-built clang 7, evidence only):

| knob | result |
|---|---|
| `VRW=2147483677:20` (elapsed weight x20) | `%251`->r8, `%29` keeps r4, **final code unchanged** (86.9) |
| `NOEVICT=1` | unchanged (86.9) |
| `FORCEVREG=2147484074:70` (%426 -> r4) | unchanged (86.9); the copy is re-split later |
| `FORCEVREG=2147483899:74` (%251 -> r8) | allocator assertion (`Cannot decrease cascade number`) |

---

## 6. Conclusion

The residual is a **closed register-allocation fixed point** with four coupled
properties, in priority order:

1. `elapsed` must be `r4` at the launch block (so `player` takes `r5` and `r4`
   is free for the `i*60` cursor afterwards);
2. `this` must stay in `sl` — which currently *requires* the `trailMatrices`
   local (removing it moves `this` to `r4`/`fp` and costs 29 points);
3. the `trailMatrices` slot must not exist (or must weigh < 11.31) so
   `elapsed` lands on 16 and the `elapsed`/`this` stores merge into `strd`;
4. `s` must spill, which follows from (1).

(2) and (3) pull against each other: the local that keeps `this` in `sl` is
exactly the slot that pushes `elapsed` down. Every source shape tried so far
(~500 across passes 2-13) preserves the trade-off. The remaining lever is the
pre-RA IR shape that makes the spiller's `%426` copy land in `r4` *and stay
there*, which is not reachable by reordering/renaming the existing expressions.

---

## 7. Reproduction

```
# gate
cmake --build cmake-build-match --target check -j8

# side-by-side
python3 tools/verify/verify.py \
  --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN9RocketGun6updateEi

# slot weights (self-built clang 7)
/dev/shm/w4910/cc7.sh -mllvm -debug-only=stack-slot-coloring

# per-instruction spill weight (WDBG hook in this LLVM build)
WDBG=1 /dev/shm/w4910/cc7.sh 2>&1 | grep 'WDBG raw=2147483677'

# regalloc trace
/dev/shm/w4910/cc7.sh -mllvm -debug-only=regalloc
```

Scratch lives in `/dev/shm/w4910`: `base.cpp`, `cc7.sh` (exact NDK compile
command with the self-built clang 7), `ccenv.sh` (compile + install + link +
score), `try.sh` (copy variant into `src/`, `ninja link_so`, score),
`score1.py` (official single-symbol verifier), `ssc.py`, `mapblk.py`,
`slotw.py`, `gen*.py`, `vv*/` (this pass's ~50 variants), `orig_fn.asm`,
`ours_fn.asm`, `gra_update.txt`, `vrr_update.txt`, `mir_pre.txt`, `ra.log`,
`bfi.log`; none of it is in the repo.
