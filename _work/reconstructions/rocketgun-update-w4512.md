# `RocketGun::update(int)` — pass 9 (w4512, wave 95)

**Status:** research note, 2026-11-03. Worktree `wt/w4512`, branch `wave95-w4512`,
base `4a88bd6e`. **No source change landed**: every source shape and every
spill-weight perturbation tried this pass scores <= 86.9 %. Tree clean, gate
unchanged.

Target `_ZN9RocketGun6updateEi`: original **1908 B / 635 insns**, ours
**1908 B / 636 insns**, official verify score **86.9**.

Earlier notes: `rocketgun-update-w4412.md` (pass 8, the most detailed),
`rocketgun-update-w4265.md` (pass 7), `rocketgun-update-w4248.md` (pass 6),
`rocketgun-update-w4216.md` (pass 5), `rocketgun-update-w4111.md` (pass 4),
`rocketgun-update-w4011.md` (pass 3), `rocketgun-update-w3831.md` (pass 2),
`rocketgun-update-86-9-1908-b_w3811.md`.

This pass adds three things the earlier notes did not have:

1. a **fully aligned instruction diff** of the two functions (section 2), so
   every structural difference is enumerated rather than inferred;
2. a **spill-weight sensitivity sweep** using a self-built clang 7 with a
   `VRW=<vreg>:<factor>` weight-override hook (section 3). It proves that the
   base is also a **local optimum in spill-weight space**, and — importantly —
   that the `%251` eviction identified in pass 8 is *not* the binding
   constraint: even at 1/1000 of its weight the `RS_Split` cost table is
   byte-for-byte identical and still picks `$r10`;
3. a further ~40 **source variants** (section 4), all <= 86.9.

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

`_ZN9RocketGun6updateEi`: 1908 B both sides, **635 vs 636** instructions,
**86.9 %**. Every other `RocketGun` function is byte-exact, so the TU's source
style is already faithful; only `update` is off.

Fast oracle used this pass (all scratch under `/dev/shm/w4512`, not in the
repo): `cc.sh` = the exact NDK compile of `RocketGun.cpp` with the self-built
clang; `score.py` = the official `tools/verify/verify.py`
`disassemble`/`reconcile`/`score` machinery for one symbol; `tryv.sh` =
compile with `VRW`, run the checked-in gold link command, score.

---

## 2. Aligned instruction diff (new)

`difflib.SequenceMatcher` over the *normalised* instruction lists (the same
lists `verify.py` scores) yields ratio 86.9 and the opcodes below. Stripping
pure register renames, the structural differences are exactly:

| # | original | ours | meaning |
|---|---|---|---|
| 1 | `mov r4, r1` | `mov r5, r1` | `elapsed` home register |
| 2 | `mov r5, r0` | `mov r4, r0` | `player` home register |
| 3 | `add.w r8, sp, #116` | `add r4, sp, #116` | launch-block temp base |
| 4 | `ldr.w r6, [sl, #24]` | `ldr.w r0, [sl, #24]` | `this->geometry` |
| 5 | `strd r4, sl, [sp,#16]` | `str r5,[sp,#12]` + `str.w sl,[sp,#20]` | elapsed+this spill (no `strd`) |
| 6 | `add.w r9, sp, #40` | `add r5, sp, #40` | temp base |
| 7 | — | `str r4, [sp, #28]` | `volatile int padx` anchor |
| 8 | `ldr r4, [sp, #16]` ×3 | `ldr r5, [sp, #12]` ×3 | elapsed reloads in the launch block |
| 9 | `movs r4, #0` | `mov.w fp, #0` | cursor init (order) |
| 10 | — | `str r5,[sp,#28]` + `ldr.w r0,[sl,#216]` + `mov.w r9,#0` + `str r0,[sp,#16]` | `volatile int unused` + `trailMatrices` local |
| 11 | `mov r9, r4` | — | cursor saved to r9 before the cluster block |
| 12 | `ldr.w sl, [sp, #20]` | — | `this` restored after the cluster calls (one site) |
| 13 | `ldr.w r0, [sl, #216]` ×2 | `ldr r0, [sp, #16]` ×2 | `this->trailMatrices` member vs local |
| 14 | `str r0, [sp, #8]` after `Sinf` | `mov r6, r0` | **original spills `s`, ours keeps it in r6** |
| 15 | `str r0, [sp, #12]` after `Cosf` | `str r0, [sp, #8]` | `c` slot |
| 16 | `ldr r4, [sp, #24]` before `Sinf` | `ldr r4, [sp, #24]` after `Cosf` | `dt` spill-reload placement |
| 17 | `ldr r6, [r0, #60]` (lifetimes base) | `ldr r2, [r0, #60]` | lifetimes base register |
| 18 | `adds r4, #60` | `add.w r9, r9, #60` | cursor increment |
| 19 | `nop` before the literal pool | — | alignment |

Everything else is a register rename or a branch-offset shift. There is no
block-order, scheduling or semantic difference.

Items 1, 5, 8, 11, 12, 18 follow directly from `elapsed` living in `r4`
(original) vs `r5` (ours); items 14–16 follow from the same colouring
permutation (see pass 8, section 3).

---

## 3. Spill-weight sensitivity sweep (new)

The self-built clang at `/tmp/llvm_android_702b/build2/bin/clang++` is LLVM 7
with a debug hook in `lib/CodeGen/CalcSpillWeights.cpp`:

```c
if (const char *vw = getenv("VRW")) { ... weight *= f for vr==li.reg ... }
```

`VRW=<vreg>:<factor>` multiplies the normalised spill weight of one virtual
register *after* `normalizeSpillWeight`. At `VRW=...:1` the compiler reproduces
the NDK 7.0.2 code for this function instruction-for-instruction (checked by
diffing the object disassembly against the NDK build), so the hook is a valid
probe of `RAGreedy::shouldEvict` (`A.weight > B.weight`).

Vreg numbers come from the `RA_NOHINT_CALL` lines in the regalloc trace:

| value | vreg | weight (base) |
|---|---|---|
| `%28` = `this` | 2147483676 | 1.479491e-02 |
| `%29` = `elapsed` | 2147483677 | 9.400839e-04 |
| `%247` = `dt` | 2147483895 | 8.627993e-03 |
| `%251` = hoisted `t2ADDri %stack.4.ref.tmp, 0` | 2147483899 | 8.566814e-03 |
| `%284` = `s` (`COPY $r0` after `Sinf`) | 2147483932 | — |
| `%386` = loop cursor `i*60` | 2147484034 | 2.805497e-02 |

Sweep results (official score; base is 86.9):

| vreg | factors | score |
|---|---|---|
| `%29` elapsed | 1.2, 1.5, 2, 3 | **86.9** |
| | 5 | 83.2 |
| | 8 | 80.4 |
| | 10 | 78.6 (635/635 insns) |
| | 15 | 76.6 (`elapsed`→r4, but slot still `[sp,#12]`) |
| | 20 | 43.4 |
| | 30 / 50 | 48.0 / 50.7 |
| `%251` stack addr | 0.9 | 85.4 |
| | 0.5 | **86.9** |
| | 0.2, 0.1, 0.05, 0.02, 0.01, 0.001 | 85.4 |
| `%247` dt | 2 / 5 / 0.5 | 53.3 / 61.4 / 85.9 |
| `%28` this | 2 / 5 / 0.5 | 66.2 / 66.2 / 53.7 |
| `%386` cursor | 2 / 0.5 / 0.2 | **86.9** / 63.2 / 74.6 |
| `%284` s | 0.5 … 0.01 | **86.9** (no change at all) |
| | 2 / 5 | 57.4 |

### 3.1 The `%251` eviction is *not* the binding constraint

Pass 8 concluded that `%251` (a loop-invariant stack address hoisted to the
preheader, weight 8.57e-3) evicts `%29` from `$r4` and that this is the root of
the colouring permutation. The weight sweep refutes the "root" part: with
`VRW=2147483899:0.001` (`%251` weight 8.57e-6, 1000x *below* `%29`), the
regalloc trace still shows

```
selectOrSplit rGPR:%29 [16r,9648B:0)[9840B,15104B:0) weight:9.400839e-04
  assigning %29 to $r4
  ... unassigning %29 from $r4
  assigning %29 to $r9
  ... unassigning %29 from $r9
RS_Split Cascade 5
  $r4  static = 0.0, v=53, total = 96.73237634 with bundles EB#1 ... EB#27.
  $r6  static = 0.0, v=54, total = 10.51362634 with bundles EB#1 ... EB#27.
  $r10 static = 0.0, v=54, total =  8.515625  with bundles EB#1 ... EB#27.
  Split for $r10 in 21 bundles, intv 1.
```

i.e. the `RS_Split` cost table is **identical to the base run** and still picks
`$r10`; the function still scores 85.4. So the binding constraint is
`RAGreedy::calculateRegionSplitCost`'s `calcGlobalSplitCost` for `$r4`
(96.73) vs `$r10` (8.52): the loop uses `r4` for the 60-byte cursor, so
isolating `$r4` costs an enter/leave in many more live bundles.

### 3.2 Forcing `elapsed` into `r4` is not sufficient

At `VRW=%29:15` the allocator does put `elapsed` in `r4`
(`mov r4, r1` at entry) — but the spill slot is still `[sp,#12]`, the
`strd r4, sl, [sp,#16]` still does not form, and the score drops to 76.6.
So "elapsed in r4" alone is *not* the target either; the frame-slot ordering
(`s`→8, `c`→12, `elapsed`→16, `this`→20) has to fall out of the same
allocation, and the weight lever moves them independently.

### 3.3 `s` cannot be made to spill by weight

Lowering `%284`'s weight by 100x changes nothing (86.9, identical code):
the inline spiller's decision for `s` is driven by the interference with the
`lifetimes` base pointer in `r6`, not by `s`'s own weight.

---

## 4. Source variants tried this pass (~40, all <= 86.9)

All measured with the official verify machinery against the pristine baseline.

| variant | pct |
|---|---|
| baseline | **86.9** (636 insns) |
| `dt` declared before the launch block | 86.9 |
| `dt` declared at the very top (before `g.update`) | 86.9 |
| `dt` declared after the `trailMatrices` local | 86.9 |
| `volatile int unused = elapsed;` + `(void) unused;` | 84.5 (638) |
| `unused` moved after the `trailMatrices` local | 86.9 |
| `volatile int unused = (int) dt;` | 84.5 |
| `volatile int unused = elapsed - (int) dt;` | 84.4 (640) |
| `int unused = elapsed;` (non-volatile) | 85.8 |
| `if (elapsed == 0x7fffffff) return;` instead of `unused` | 67.0 |
| `if (elapsed < 0) return;` | 67.1 (635/635) |
| `if (elapsed == -1) { fadeTimer = 0; }` | 84.3 |
| `if (elapsed == 0x7fffffff) { }` | 85.8 |
| `if (elapsed == 0x7fffffff) fadeTimer = 0;` | 84.3 (642) |
| `unused` + `if (elapsed == 0x7fffffff) return;` | 66.8 |
| `int unused = elapsed;` + `if (unused == 0x7fffffff) return;` | 67.0 |
| `for (int i = ...)` / `for (unsigned i = ...)` | 86.9 |
| cluster block via `this->gun->` instead of `gun->` | 86.9 |
| second trig arg `total - life` (shared `life`) | 53.2 (621) |
| second trig arg `gun->initialLifetime - gun->lifetimes[i]` | 86.9 |
| shared `arg` local for `Sinf` only | 86.9 |
| `ahead = pos + (vel * dt) * 0.5f` | 86.9 |
| `ahead = pos + vel * (dt * 0.5f)` | 55.6 (625) |
| `(s * 2.0f)` / `(c * 2.0f)` | 86.9 |
| `const Vector norm = VectorNormalize(cross);` local | 85.1 |
| `const Vector cross = VectorCross(gun->velocities..., gun->upVectors...)` | 86.9 |
| `Array<Matrix> *const trailMatrices` | 86.9 |
| `Matrix &trail = trailMatrices->data()[i];` | 85.7 (637) |
| `const Vector &upv` local for the cross | 84.6 (637) |
| `const Vector &vel` local for the cross | 86.9 |
| both `vel` and `upv` locals | 86.9 |
| `Vector &pos = ...positions.data()[i];` for the two `+=` | 59.4 (638) |
| swap the two `+=` statements | 56.6 (640) |
| `s` before `cross` (i.e. `c` and `cross` reordered) | 86.9 |
| `const int n` / `(uint32_t) n` / `(int) n` in the phase divide | 86.9 / 86.9 / 86.7 |
| `const uint32_t total` / `const uint32_t life` | 86.7 / 86.7 |
| `total * i` (no `(int)` cast) in the phase | 86.9 |
| `Gun *g = this->gun; g->update(elapsed)` instead of `Gun &g` | 86.9 |
| no `g` local (`this->gun->update(elapsed)`) | 86.9 |
| `gun = this->gun;` hoisted to the top of the function | 86.9 |
| `Vector ahead` declared after the launch block | 73.2 |
| `trailMatrices` local declared before `dt` | 86.9 |
| `trailMatrices` local declared at the top of the function | 80.1 (638) |
| anchor matrix (U=unused, P=padx, T=trail local) | |
| U P T (baseline) | **86.9** |
| - P T | 85.8 |
| U - T | 86.7 |
| U P - | 57.6 (626) |
| - P - | 57.4 (622) |
| - - T | 85.6 |
| U - - | 57.5 (624) |
| - - - (faithful) | 57.4 |

The anchor matrix confirms pass 8: the `trailMatrices` local is load-bearing
(57.x without it, because `this` moves to `fp`), and `unused`+`padx` together
buy 86.9 vs 85.6.

---

## 5. What remains

The residual is a single `RAGreedy` fixed point:

1. `%29` (`elapsed`) is allocated first and takes `$r4`.
2. It is evicted (by `%251`, then `%386`) and requeued.
3. `RS_Split Cascade 4/5` scores `$r4 static = 0.0, total = 96.73` vs
   `$r10 static = 0.0, total = 8.52` and picks `$r10`.
4. `elapsed` therefore lands in `r5`, `r4` is taken by `player` and later by
   the `VectorCross` sret pointer, `s` stays in `r6`, `c` lands on `[sp,#8]`,
   `elapsed` on `[sp,#12]`, `trailMatrices` on `[sp,#16]`; the `elapsed`/`this`
   stores are not adjacent and no `strd` merge happens (636 vs 635 insns).

To match, a source shape must simultaneously

1. keep `elapsed` in `r4` through the launch block (pre-spill interval ends at
   `15edea`);
2. keep `this` in `sl` (r10) — currently only reachable with the
   `trailMatrices` local;
3. force `s` to spill so `c`/`elapsed`/`this` land on `12`/`16`/`20` and the
   post-RA `ARMLoadStoreOptimizer` merges the two stores into one `strd`;
4. keep the 160-byte frame.

(1) and (2) pull in opposite directions, and section 3 shows the weight lever
cannot resolve it: the `RS_Split` cost for `$r4` is dominated by the loop's
use of `r4` as the 60-byte cursor, not by any spill weight. The remaining
lever is the exact pre-RA IR shape that changes the *interference bundles*
seen by `calcGlobalSplitCost` (in particular the hoisted `t2ADDri
%stack.4.ref.tmp, 0` that pass 8 identified), and that is not reachable by
reordering/renaming the existing expressions.

One unexplored idea for a future pass: the original computes the loop's
`Vector` temporary addresses **inside** the loop (`add r6, sp, #128`,
`add r5, sp, #40`, `add r6, sp, #116`), whereas ours hoists the shared
`%stack.4.ref.tmp` address to the preheader. A source shape that prevents that
IR-level hoist (e.g. distinct, non-mergeable `Vector` temporaries per call
site) would change the `$r4` bundle set and is the most plausible way to move
the split cost below `$r10`'s.

---

## 6. Reproduction

```
# gate
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8

# side-by-side
python3 tools/verify/verify.py \
  --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN9RocketGun6updateEi

# spill-weight probe (scratch clang 7 with the VRW hook)
VRW=2147483677:15 /dev/shm/w4512/cc.sh src/game/weapons/RocketGun.cpp /dev/shm/w4512/x.o \
  -mllvm -debug-only=regalloc
```

Scratch lives in `/dev/shm/w4512`: `base.cpp`, `cc.sh`, `try.sh`, `tryv.sh`,
`score.py`, `diffn.py`, `cls.py`, `linkcmd.sh`, `batch*.py`, `orig.asm`,
`ours.asm`, `o_base.asm`, `ra_base.log`, `ra_251low.log`; none of it is in the
repo.
