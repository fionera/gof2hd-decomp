# `RocketGun::update(int)` — wave 106 (w5412)

**Status:** research note, 2026-11-04. Worktree `wt/w5412`, branch
`wave106-w5412`, base `486f8a119`. **No source change landed**: every source
shape tried this pass scores <= 86.9 %. Tree clean; gate unchanged
(byte 4500, linked 4500, size 4524/4524, data 843/843, imports 207, lint
CLEAN, drift 0).

Target `_ZN9RocketGun6updateEi`: original **1908 B / 635 insns**, ours
**1908 B / 636 insns**, official verify score **86.9**.

Earlier notes: `rocketgun-update-w5315.md` (pass 16, the slot-weight fixed
point and the `%251` LICM hoist), `rocketgun-update-w5010.md` (pass 13), and
`rocketgun-update-wf210.md`, `-wf109`, `-w103`, `-w5010`, `-w4910`, `-w4812`,
`-w4710`, `-w4512`, `-w4412`, `-w4265`, `-w4248`, `-w4216`, `-w4111`,
`-w4011`, `-w3831`, `-w3811`.

---

## 1. Baseline (NDK r18b clang 7.0.2, this worktree)

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
```
```
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.91
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports 207
RATCHET PASS
```

`_ZN9RocketGun6updateEi`: 1908 B both sides, **635 vs 636** instructions,
**86.9 %**.

---

## 2. The residual, restated (from w5315)

The body is semantically faithful (REA decompile matches statement by
statement). The residual is codegen only:

* prologue: original `strd r4, sl, [sp,#16]` (elapsed@16, this@20); ours
  `str r5,[sp,#12]` + `str.w sl,[sp,#20]` (+1 insn);
* `elapsed` in `r4` (original) vs `r5` (ours); `player` the other way round;
* `s` spilled to `[sp,#8]` (original) vs held in `r6` (ours); `c` then moves
  from `[sp,#8]` to `[sp,#12]`;
* `this->trailMatrices` loaded three times from `[sl,#216]` (original) vs a
  cached local at `[sp,#16]` (ours);
* `i*60` cursor in `r4` (original) vs `r9` (ours).

The stack-slot weights (self-built clang 7, `-mllvm
-debug-only=stack-slot-coloring`) are:

```
ours:     dt 47.742 > this 36.742 > trail 32.484 > elapsed 11.312 > c 7.266 > guard 2.0
original: dt 47.742 > this 43.523 > elapsed 11.312 > c 7.266 = s 7.266 > guard 2.0
```

To match, one source shape must simultaneously (1) drop the trail slot,
(2) keep `this` below `dt`'s weight, and (3) make `s` spill.

The root cause is the greedy allocator's first-round eviction of `elapsed`
from `r4` by `%251` (`t2ADDri %stack.4.ref.tmp`, the loop-invariant address
of the shared `Vector` temporary hoisted by MachineLICM, weight 8.57e-3 vs
elapsed's 9.4e-4). With the patched self-built clang
(`FORCEEVICT=1 FORCEANYFN=1 FORCEVREG=2147484074:70`) the entry copy of
`elapsed` is forced into `r4` and the score rises to **88.3 %** (1908 B,
636 insns) — but the frame slots stay `elapsed@12, this@20` (no `strd`), so
the register colouring and the slot assignment are independent.

---

## 3. What was tried this pass (~700 new shapes, all <= 86.9)

All measured with the single-symbol oracle (`/dev/shm/w5412/score.py`,
imports `tools/verify/verify.py`; `try.sh <variant.cpp>` = copy +
`ninja link_so` + score, ~0.65 s/variant). Baseline 86.9 / 636 insns.

### 3.1 Splitting the `%251` hoist (the w5315 "next hypothesis")

The three `velocities[i] * dt * 0.5f` temporaries share `%stack.4`; the
address is hoisted to the preheader and evicts `elapsed`.  Every attempt to
give the two ahead computations distinct temporaries changes the whole
allocation:

| variant | pct | insns | size |
|---|---|---|---|
| baseline | **86.9** | 636 | 1908 |
| trail ahead via `Vector vel = velocities[i];` | 76.7 | 643 | 1936 |
| trail ahead via `Vector vel = velocities[i] * dt;` | 60.8 | 660 | 1872 |
| trail ahead via `Vector pos = positions[i];` | 57.9 | 632 | 1836 |
| trail ahead `v * (dt * 0.5f)` | 52.6 | 623 | 1812 |
| trail ahead `v * 0.5f * dt` | 86.5 | 636 | 1908 |
| top ahead `v * (dt * 0.5f)` | 53.7 | 619 | 1808 |
| top ahead `v * 0.5f * dt` | 61.5 | 627 | 1824 |
| both `v * (dt * 0.5f)` | 53.7 | 619 | 1808 |
| both `v * 0.5f * dt` | 61.5 | 627 | 1824 |
| trail ahead into a second `Vector ahead2` | 79.5 | 636 | 1920 |
| trail ahead `Vector vel = v * dt * 0.5f; ahead = pos + vel;` | 57.8 | 627 | 1824 |
| trail ahead `this->gun->...` | 86.9 | 636 | 1908 |
| both aheads `this->gun->...` | 86.9 | 636 | 1908 |
| aheads via `operator[]` instead of `.data()` | 85.3 | 636 | 1908 |

### 3.2 `elapsed` / `dt` lifetime and copies

| variant | pct |
|---|---|
| baseline | **86.9** |
| `(float) elapsed` directly in the loop, no `dt` local | 86.9 |
| `const int frameTime = elapsed;` then `dt = (float) frameTime` | 86.9 |
| `const int loopElapsed = elapsed;` used by the loop fade timer | 86.9 |
| `const int elapsedCopy = elapsed;` in the fade block | 86.9 |
| `dt` declared after the copy | 86.9 |
| `const float &dt = *(const float *) &elapsed;` | 53.0 |
| `const int elapsed2 = (int) dt;` used by the loop fade timer | 52.9 |
| `dt` declared inside the loop body | 86.9 |

### 3.3 Anchor placement / forms

The two `volatile` anchors are load-bearing; every move or type change is
worse:

| variant | pct |
|---|---|
| baseline (`unused` before `dt`, `padx` in the `z==50000` branch) | **86.9** |
| `unused` at function top | 83.4 |
| `unused` inside the loop body | 70.0 |
| `unused` after the loop | 85.0 |
| `unused = elapsed + 1` / `- 1` | 84.5 |
| `unused` as `float` / `short` / `char` / `double` | 84.6 / 84.6 / 84.6 / 75.4 |
| `unused` as `unsigned` | 86.9 |
| two `unused` anchors | 75.6 |
| `unused` after `dt` | 86.9 |
| `padx` at function top / in the loop / after the loop | 84.7 / 75.4 / 75.4 |
| `padx` as `float` / `unsigned` / `short` / `double` | 86.9 / 86.9 / 86.0 / 71.8 |
| `padx = 1` / `-1` / `2000` / `fadeTimer` | 85.9 / 80.8 / 80.8 / 80.8 |
| `padx` in the trail-copy `else` branch | 71.6 |

### 3.4 Trail-block statement order

| variant | pct |
|---|---|
| baseline | **86.9** |
| `Matrix &trail = ...data()[i]` hoisted before the first MST | 84.0 |
| `Matrix *trailData = ...data()` hoisted before the first MST | 84.0 |
| heading computed before the first MST | 59.3 |
| second MST via `this->trailMatrices` | 86.9 |
| first MST via `this->trailMatrices` (local kept) | 66.3 |

### 3.5 Cached `Array` references across the loop

| variant | pct |
|---|---|
| `Array<Vector> &velocities = gun->velocities;` | 75.1 |
| `Array<Vector> &upVectors = gun->upVectors;` | 73.4 |

### 3.6 Faithful no-local form and the `cross` default-construct

| variant | pct | insns | size |
|---|---|---|---|
| remove the trail local (member everywhere) | 57.6 | 626 | 1880 |
| `Vector cross; cross = VectorCross(...)` + no trail local | 85.5 | 640 | 1920 |
| that + remove `unused` | 85.6 | 640 | 1920 |
| `life` declared after `phase` (matches the original's post-`uidiv` load order) | 86.5 | 636 | 1908 |

The `cross`-default shape is the only one found that reproduces the
original's `strd` slot merge, but it pays five instructions (dead zero-init +
out-of-line `Vector::operator=`), so it loses on the fuzzy metric.

### 3.7 Launch-condition spelling

`__builtin_expect` is load-bearing (plain form 84.6, `ittt` if-conversion).
`!!`, per-operand `__builtin_expect`, implicit `bool`, `!= 0` on the result
and a `bool` temporary all score the same 86.9; `&` instead of `&&` scores
86.2.

### 3.8 Prologue screens

Two fast screens over the 43-edit space (compile with the self-built clang,
`objdump` the `.o`, inspect the first 40 instructions):

* all 43 singles + 903 pairs: 294 variants change the prologue, **none** puts
  `elapsed` in `r4`;
* 1500 random 1-5-edit combinations: **none** puts `elapsed` in `r4`;
* 2500 random 1-4-edit combinations: **none** emits `strd rX, sl` (the
  original's elapsed/this merge) in the prologue.

So the eviction of `elapsed` from `r4` is not reachable by any of the
declaration-order, lifetime, anchor, expression-shape or statement-order
levers in this edit space.

---

## 4. Proven vs hypothesis

**Proven (measured):**

* the body is semantically faithful; the residual is codegen only;
* `elapsed -> r4` alone is worth **+1.4 pts** (86.9 -> 88.3) but does not fix
  the frame slots;
* the current anchors are each worth 0.1-2.3 pts and no move/type change
  improves on them;
* no expression shape, declaration order, lifetime extension, anchor form or
  statement order tried (this pass plus passes 2-16, ~1400 variants total)
  moves `elapsed` into `r4` or produces the `strd` merge together with a
  competitive instruction count.

**Hypothesis (unproven):**

* the original source has a real construct (not an anchor) that raises
  `elapsed`'s spill weight above `%251`'s 8.57e-3, or prevents the
  MachineLICM hoist of the shared `Vector` temporary address.  The only
  shape found that changes the slot ordering (`Vector cross;`) pays 5
  instructions, so it is not the original's construct.
* the `[sp,#28]` hole in the original is alignment padding before the
  align-8 `[sp,#40]` frame object; our anchors happen to occupy it, which is
  why they are size-neutral.

---

## 5. Reproduction

```
# gate
cmake --build cmake-build-match --target check -j6

# side-by-side
python3 tools/verify/verify.py \
  --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN9RocketGun6updateEi

# slot weights (self-built clang 7, /tmp/llvm_android_702b/build2/bin/clang++)
/dev/shm/w5412/cc7.sh src/game/weapons/RocketGun.cpp -mllvm -debug-only=stack-slot-coloring

# forced-eviction ceiling (patched clang; not a source change)
FORCEEVICT=1 FORCEANYFN=1 FORCEVREG=2147484074:70 /dev/shm/w5412/cc7.sh <variant.cpp>
# then copy x.o over cmake-build-match/CMakeFiles/gof2.dir/src/game/weapons/RocketGun.cpp.obj
# and run the link command from `ninja -C cmake-build-match -t commands libgof2hdaa.so`
```

Scratch lives in `/dev/shm/w5412`: `base.cpp`, `cc7.sh`, `try.sh`,
`score.py`, `edits.py`, `screen.py`, `screen2.py`, `screen3.py`,
`batch6.py`..`batch17.py`, `orig.asm`, `ours.asm`, `sbs.txt`, `ra_base_full.txt`,
`ra_nol_full.txt`; none of it is in the repo.
