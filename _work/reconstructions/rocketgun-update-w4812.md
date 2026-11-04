# `RocketGun::update(int)` — pass 11 + 12 (w4812, wave 98)

**Status:** research note, 2026-11-04. Worktree `wt/w4812`, branch `wave98-w4812`,
base `fe4f5e7cf`. **No source change landed**: every source shape tried this pass
scores <= 86.9 %. Tree clean, gate unchanged (byte 4500, linked 4500, size
4524/4524, data 843/843, imports 207, lint CLEAN, drift 0).

> **Pass 12 addendum is section 8 (below).** It replaces the weight table of
> sections 2-3 with the exact per-reference decomposition and resolves the
> apparent `this > dt` contradiction: the original's third loop `this` reload
> is inside the cluster block, so `this` weighs 12.52, not 52.0.

Target `_ZN9RocketGun6updateEi`: original **1908 B / 635 insns**, ours
**1908 B / 636 insns**, official verify score **86.9**.

Earlier notes: `rocketgun-update-w4710.md` (pass 10, frame-slot map + regalloc
trace), `rocketgun-update-w4512.md` (pass 9, aligned diff + spill-weight sweep),
`rocketgun-update-w4412.md` (pass 8, Ghidra decomp appendix), `-w4265`, `-w4248`,
`-w4216`, `-w4111`, `-w4011`, `-w3831`, `-w3811`.

This pass adds the missing piece the earlier notes only guessed at: **the exact
mechanism that decides the frame-slot offsets**. It is *not* spill-slot creation
order. It is `StackSlotColoring`'s **sum of block frequencies of every machine
instruction that references the slot** (`lib/CodeGen/StackSlotColoring.cpp`,
`ScanForSpillSlotRefs` -> `LiveIntervals::getSpillWeight(false, true, MBFI, MI)`
= `1.0 * blockFreq/entryFreq`). The slots are then coloured heaviest-first, and
`PEI::assignSpillSlots` gives the heaviest colour the highest frame offset. This
turns the residual into a *source-level weight-ordering* problem and explains
why every previous "creation order" experiment failed.

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
**86.9 %**. All other `RocketGun` functions are byte-exact.

The current source carries four anchors (all documented inline):

| anchor | removal |
|---|---|
| `__builtin_expect(hasGeometry && delayActive, 1)` on the launch `if` | 84.6 |
| `volatile int padx = 0;` in the second launch branch | 86.7 |
| `volatile int unused = elapsed;` before the loop | 85.8 |
| `Array<Matrix> *trailMatrices = this->trailMatrices;` before the loop | 57.6 |
| all four removed (faithful form) | 61.6 (657 insns) |

---

## 2. The slot-offset mechanism (new, authoritative)

`-mllvm -debug-only=stack-slot-coloring` on the baseline prints:

```
Spill slot intervals:
SS#32 [16r,9648B:0)[9840B,15104B:0)  0@x weight:1.131250e+01   ; elapsed
SS#33 [72r,9680r:0)[9840B,15104B:0)  0@x weight:2.000000e+00   ; __stack_chk_guard GOT ptr
SS#34 [9296r,9648B:0)[9840B,15104B:0)  0@x weight:4.774219e+01 ; dt
SS#35 [9248r,9648B:0)[9840B,15104B:0)  0@x weight:3.248438e+01 ; trailMatrices local
SS#36 [1900r,9648B:0)[9840B,15104B:0)  0@x weight:3.674219e+01 ; this
SS#37 [12032r,12704r:0)  0@x weight:7.265625e+00              ; c (Cosf result)

Color spill slot intervals:
Assigning fi#34 to fi#32   ; dt           47.74 -> colour 32 (highest offset)
Assigning fi#36 to fi#33   ; this         36.74 -> colour 33
Assigning fi#35 to fi#34   ; trailMatrices 32.48 -> colour 34
Assigning fi#32 to fi#35   ; elapsed      11.31 -> colour 35
Assigning fi#37 to fi#36   ; c             7.27 -> colour 36
Assigning fi#33 to fi#37   ; guard ptr     2.00 -> colour 37 (lowest offset)
```

Colour 32 gets `[sp,#24]`, 33 -> `[sp,#20]`, 34 -> `[sp,#16]`, 35 -> `[sp,#12]`,
36 -> `[sp,#8]`, 37 -> `[sp,#4]`. So the baseline slot map is exactly

```
[sp,#24] dt, [sp,#20] this, [sp,#16] trailMatrices local,
[sp,#12] elapsed, [sp,#8] c, [sp,#4] guard ptr
```

and the original's is

```
[sp,#24] dt, [sp,#20] this, [sp,#16] elapsed,
[sp,#12] c, [sp,#8] s, [sp,#4] guard ptr
```

i.e. the original's weight order is

```
dt (47.7) > this (36.7) > elapsed (11.3) > c (7.27) > s (?) > guard ptr (2.0)
```

The **only** ordering defect in the baseline is the `trailMatrices` local
(32.48), which sits between `this` and `elapsed` and pushes `elapsed` from 16
down to 12. With `elapsed` at 16 and `this` at 20 the two adjacent stores merge
into `strd r4, sl, [sp,#16]` (635 insns); today they are
`str r5,[sp,#12]` + `str.w sl,[sp,#20]` (636 insns). `s` is absent from our
slot set entirely (it lives in `r6`; see section 4).

### 2.1 Weight = sum of block frequencies of slot references

`ScanForSpillSlotRefs` walks every machine instruction and, for each frame-index
operand of a non-debug instruction, adds `1.0 * blockFreq/entryFreq` to the
slot's weight. There is **no live-range normalisation** for stack slots (unlike
virtual-register spill weights). Consequences:

* moving a *register* use of `elapsed` does not change its slot weight;
* a *reload/store of the slot* in a hotter block does;
* the weight is a pure sum over reference sites, so adding one reference in a
  block of frequency `f` adds exactly `f`.

This is why the pass-9 `VRW` sweep on virtual registers could not move the
slots: `VRW` perturbs `CalcSpillWeights` (vreg weights), not the stack-slot
weights computed later by `StackSlotColoring`.

---

## 3. Source experiments that move the slot weights

I used the self-built LLVM 7 (`/tmp/llvm_android_702b/build2/bin/clang++`,
verified to reproduce NDK 7.0.2 object code instruction-for-instruction for this
TU) with `-mllvm -debug-only=stack-slot-coloring` to read the weights directly,
and the official `tools/verify/verify.py` machinery for the score. Scratch is
`/dev/shm/w4812`.

### 3.1 Moving the `volatile int unused = elapsed;` store

The `unused` anchor is a volatile store of `elapsed` in the loop preheader. It
references the *register*, not the slot, so it does not change the elapsed slot
weight (11.31 in every position before the spill). Moving it **into the loop**
forces a slot reload and raises the weight:

| `unused` position | elapsed slot weight | score |
|---|---|---|
| preheader (baseline) | 11.31 | **86.9** |
| top of `if (gun->active != 0)` | **30.69** | 82.2 (637 insns) |
| end of `if (gun->active != 0)` | 26.87 | — |
| `else` branch (ahead.z != 50000) | 18.58 | 84.5 (638) |
| cluster block | 15.67 | 70.7 (633) |
| trail block | 24.15 | 70.7 (633) |
| `fade:` block | **43.04** | 70.0 (633) |
| inside `if (gun->hitSmall)` | 11.31 | 84.8 (636) |
| after the fade block / before `dt` | 11.31 | 86.9 |

The top-of-active-block position lands at 30.69, tantalisingly just below the
`trailMatrices` weight of 32.48. Adding a *second* volatile use in the homing
block gives **35.10**, which is exactly between `this` (36.74) and
`trailMatrices` (32.48):

| variant | elapsed weight | score |
|---|---|---|
| `unused` at active-block top + `volatile int u2 = elapsed;` in homing block | 35.10 | 70.6 (635 insns, 1908 B) |
| `unused` at active-block top + move the existing `padx` store into homing block as `padx = elapsed` | 35.10 | 70.3 (633 insns, 1904 B) |

The slot order is now right (`elapsed@16`, `this@20`), but the extra volatile
store changes the *spill point*: `elapsed` is now spilled at function entry
(`str r1,[sp,#16]` at instruction 18) instead of in the launch block, so the
two stores are no longer adjacent in the instruction stream and `strd` still
does not form. The second variant also shrinks the frame to 1904 B.

**Conclusion:** the weight lever works, but every source shape that raises
`elapsed` above `trailMatrices` also perturbs the spill point or the frame
layout enough to lose more than the `strd` merge gains.

### 3.2 Removing the `trailMatrices` slot

The faithful form (no `trailMatrices` local) gives the weight order
`this 66.99 > dt 44.90 > elapsed 11.31 > c 7.27 > guard 2.0`, i.e. `this` and
`dt` swap and `elapsed` is at 16 but `this` is at 24. The local's presence is
what suppresses `this`'s weight from 67 to 36.7 (it removes three
`this->trailMatrices` reloads from the loop). Trying to keep the suppression
without the slot — `const` pointer, per-iteration local, `this->` for one or two
of the three uses — either still spills the local or raises `this` to ~67.
All 8 subsets of {A=`if (trailMatrices==nullptr)`, B=`if (trailMatrices!=nullptr)`,
C=`MatrixSetTranslation((*trailMatrices)[i],...)`} using `this->` instead are
worse: 86.9 (all local), 66.3, 68.8, 64.2, 68.2, 64.2, 64.2, 57.6.

### 3.3 The `s` spill is a register-pressure effect, not a weight effect

`c` is spilled (weight 7.27) but `s` is not: after `Sinf`, `r6` is free in our
IR and the greedy allocator keeps `s` there (`mov r6,r0`). In the original both
trig results are spilled (`str r0,[sp,#8]` after `Sinf`, `str r0,[sp,#12]`
after `Cosf`). The direct cause is visible in the instruction schedules:

```
original (around Sinf/Cosf)              ours (baseline)
  ldr   r4, [sp, #24]   ; dt loaded       ...
  ...                   ; BEFORE Sinf     ...
  blx   Sinf                              blx   Sinf
  str   r0, [sp, #8]    ; s spilled       mov   r6, r0        ; s stays in r6
  ...                                     ...
  blx   Cosf                              blx   Cosf
  str   r0, [sp, #12]   ; c spilled       str   r0, [sp, #8] ; c spilled
  ...                                     ...
  (dt already in r4)                      ldr   r4, [sp, #24] ; dt loaded here
```

The original hoists the `dt` reload above `Sinf` and keeps it in `r4` across
`Sinf`, `Cosf`, `VectorCross`, `VectorNormalize` and the first `operator*`;
that extra live callee-saved value is what leaves no register for `s`. Ours
loads `dt` only after `Cosf`, so `r4` is free and `s` stays in `r6`. The
`dt`-before-`Sinf` load is a pre-RA list-scheduler decision (`list-hybrid` is
the default; `list-ilp` scores 66.5, `source` 50.3, `list-burr` 72.0), and no
source shape tried (`dt` non-const / `static_cast` / `float()` / declared at
top / before the launch block / before the fade block / `(float)elapsed` inline
in the loop) moves it.

---

## 4. Register colouring is a separate lever

Forcing the entry copy of `elapsed` (`%426`, raw vreg 2147484074) into `r4`
with the self-built clang's `FORCEVREG` hook gives **88.3 %** with the same
636-insn size: `elapsed` moves to `r4`, `player` to `r5`, matching the original
launch-block permutation, but the frame slots stay `12/20`. This confirms the
pass-10 conclusion that register colouring and slot assignment are independent.
The eviction is concrete in the regalloc trace:

```
selectOrSplit rGPR:%29 [16r,9648B:0)[9840B,15104B:0) weight:9.400839e-04
  assigning %29 to $r4                      ; elapsed -> r4
selectOrSplit GPRnopc:%251 [9312r,9648B:0)[9840B,15104B:0) weight:8.566814e-03
  should evict: %29 ... evicting $r4 interference: Cascade 1
  unassigning %29 from $r4
  assigning %251 to $r4
```

`%251` is `t2ADDri %stack.4.ref.tmp, 0`, the address of the stack slot that IR
`StackColoring` merged from `forward` / `up` / the three loop `Vector`
temporaries (`Merging #7 and slots #9/#29/#31/#2/#17/#20/#26` in
`-debug-only=stack-coloring`). It is loop-invariant, so MachineLICM hoists it
to the preheader and it stays live to `15104B`, evicting `elapsed` from `r4`.
Disabling MachineLICM drops the score to 48.9, so the hoist itself cannot be
removed.

---

## 5. Source variants tried this pass (all <= 86.9)

All measured with the official verify machinery. Baseline 86.9 / 636 insns.

### 5.1 cluster-block expression shapes

| variant | pct |
|---|---|
| baseline `positions += VectorNormalize(cross) * (s + s) * dt;` | **86.9** |
| `Vector dir = VectorNormalize(cross) * (s + s); positions += dir * dt;` | 85.4 |
| `Vector dir = VectorNormalize(cross); positions += dir * (s + s) * dt;` | 85.1 |
| `positions += VectorNormalize(cross) * dt * (s + s);` | 75.1 (638) |
| `positions += VectorNormalize(cross) * ((s + s) * dt);` | 70.0 (627) |
| `dir *= (s+s); dir *= dt; positions += dir;` | 54.2 (624) |
| `Vector dir = VectorNormalize(cross) * (s + s) * dt; positions += dir;` | 84.4 (638) |
| `dir = dir * (s+s); dir = dir * dt; positions += dir;` | 63.5 (666) |
| swap the two `+=` statements | 56.6 (640) |
| `cross` computed before the trig calls | 57.1 (640) |
| `Cosf` uses the locals `total - life` instead of `this->gun->...` | 53.2 (621) |

### 5.2 `s`/`c` declaration shapes

| variant | pct |
|---|---|
| `float s` / `float c` / both non-const | 86.9 |
| `double s` / `double c` | 86.9 |
| `float s, c; s = ...; c = ...;` | 86.9 |
| `s`/`c` declared before the cluster `if` or at loop top or at function scope | 86.9 |
| `s * 2.0f` / `c * 2.0f` instead of `s + s` / `c + c` | 86.9 |
| `(float) elapsed` inline instead of `dt` in the first `+=` | 86.9 |

### 5.3 `dt` shapes

| variant | pct |
|---|---|
| `float dt` non-const / `static_cast<float>` / `float(elapsed)` | 86.9 |
| `dt` declared before the launch block / at the very top | 86.9 |
| `dt` declared after the `trailMatrices` local | 86.9 |
| `n` before `life` / `life` before `n` in the cluster block | 86.7 |

### 5.4 launch-block shapes

| variant | pct |
|---|---|
| `barrel.z = barrel.z - 100.0f` | 86.9 |
| `up` declared before `forward` | 85.9 |
| `up` inlined as `Vector{0,1,0}` at the `setDirection` call | 86.1 |
| `forward` inlined at the `setDirection` call | 85.8 |
| `muzzle` computed as `getPosition() + MatrixRotateVector(...)` | 78.5 (637) |
| `forward`/`up` hoisted to function scope | 74.7 / 74.4 |

### 5.5 `unused` / `padx` / `trailMatrices` positions

| variant | pct |
|---|---|
| baseline | **86.9** |
| `unused` inside `if (gun->hitSmall)` | 84.8 |
| `unused` before `wasFiring` | 85.3 |
| `unused` before the fade block | 85.0 |
| `unused` at the top of the active block | 82.2 (637) |
| `unused` in the `else` branch | 84.5 (638) |
| `unused` in the cluster block / trail block / `fade:` | 70.7 / 70.7 / 70.0 (633) |
| `unused` after the fade block / before `dt` | 86.9 |
| `padx = elapsed` | 72.6 (633) |
| `padx = elapsed - elapsed` | 86.9 |
| `padx` removed | 86.7 |
| `trailMatrices` local at function top / after `g.update` / before launch / after launch / before `hitSmall` | 80.1 / 80.6 / 80.6 / 81.1 / 81.1 |
| `trailMatrices` local before the fade block | 70.0 (643) |
| `trailMatrices` local before `dt` | 86.9 |
| `trailMatrices` per-iteration local (loop top) | 59.7 (631) |
| `trailMatrices` per-iteration local, all uses replaced | 69.0 (632) |
| `Array<Matrix> *const trailMatrices` | 86.9 (identical code) |

### 5.6 scheduler flags (self-built clang, for evidence only)

| flag | pct |
|---|---|
| default (`list-hybrid`) | **86.9** |
| `-pre-RA-sched=list-ilp` | 66.5 (635 insns, 1912 B) |
| `-pre-RA-sched=list-burr` | 72.0 (638, 1904 B) |
| `-pre-RA-sched=source` | 50.3 (638, 1900 B) |
| `-disable-machine-licm` | 48.9 (666, 1884 B) |
| `-misched-postra=false` / `-enable-misched=false` / `-disable-early-ifcvt` | 86.9 |
| `-disable-copyprop` | 85.9 |

---

## 6. What remains

The residual is now fully characterised as a **stack-slot weight ordering**
problem plus the coupled **`dt`-hoist / `s`-spill** register-pressure problem:

1. `elapsed`'s slot weight must rise above `trailMatrices`'s 32.48 without
   moving its spill point out of the launch block. The only source lever found
   that raises it (a volatile slot reference in the active block) also moves
   the spill point to function entry, so the `strd` merge is lost again.
2. The `trailMatrices` local must stop occupying a slot while still suppressing
   `this`'s weight (67 -> 36.7). No source shape found does both.
3. `s` must spill, which requires the `dt` reload to be hoisted above `Sinf`
   (original) instead of after `Cosf` (ours). This is a list-scheduler decision
   that no source shape tried so far changes.

The most promising untried direction is a source shape that keeps the
`trailMatrices` local's weight below `elapsed`'s 11.31 — i.e. at most one
slot reference in a block of frequency < 2.8 — while still removing the three
`this->trailMatrices` reloads from the loop. That would produce
`dt@24, this@20, elapsed@16, trailMatrices@12, c@8, guard@4` and the `strd`
merge, even without the `s` spill.

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

# stack-slot weights (self-built clang 7)
CCERR=/dev/shm/w4812/ra.log /tmp/llvm_android_702b/build2/bin/clang++ ... \
  -mllvm -debug-only=stack-slot-coloring

# register colouring (self-built clang 7, RA_TRACE required for the hook)
RA_TRACE=1 FORCEEVICT=1 FORCEANYFN=1 FORCEVREG=2147484074:70 /tmp/llvm_android_702b/build2/bin/clang++ ...
```

Scratch lives in `/dev/shm/w4812`: `base.cpp`, `cc7.sh` (self-built clang
compile command), `linkcmd.sh` (checked-in gold link), `fast3.sh`
(compile+link+score, ~0.8 s), `score1.py` (single-symbol official verifier),
`ra.log`, `ra_faithful.log`, `pmi.log`, `ssc.log`, `irsc.log`, `diff*.txt`;
none of it is in the repo.

---

## 8. Pass 12 addendum (w4812, wave 98): the exact weight decomposition

This section supersedes the hand-wavy "weights" of sections 2-3 with the
**exact per-reference decomposition** (each slot weight is the sum of
`blockFreq/entryFreq` over every machine instruction that references the
frame index, `StackSlotColoring::ScanForSpillSlotRefs` ->
`LiveIntervals::getSpillWeight(false, true, MBFI, MI)`). Block frequencies are
read with `-mllvm -print-machine-bfi` (entry freq 273, loop body 31.0,
`for.cond` 32.0). Scratch: `/dev/shm/w4812`.

### 8.1 Baseline (our source, 636 insns, 86.9 %)

| slot | refs (block freq) | weight | offset |
|---|---|---|---|
| `dt` | preheader store (1.0) + for.body (31.0) + cluster (3.6328) + trail-block (12.109) | **47.742** | 24 |
| `this` | store (1.0) + launch (0.625) + cluster ×3 (3×3.6328) + trail-block ×2 (2×12.109) | **36.742** | 20 |
| `trailMatrices` local | preheader store (1.0) + A (12.109) + B (7.2656) + C (12.109) | **32.484** | 16 |
| `elapsed` | store (1.0) + `if.then76` (0.17578) + `if.then91` (0.24414) + `if.else104` (0.14648) + `itemEmit` (0.058594) + fade (9.6875) | **11.3125** | 12 |
| `c` | cluster store+load (2×3.6328) | **7.2656** | 8 |
| guard | (2.0) | 2.0 | 4 |

The `this` reloads are at `0x60c`, `0x620`, `0x63e` (cluster block,
3.6328 each) and `0x6ac`, `0x6b8` (trail block, 12.109 each). The last two
exist only because `sl` is clobbered by the `Vector::operator*` sret pointer
(`add.w sl, sp, #128` at `0x66c`).

### 8.2 The original (635 insns)

Same block frequencies (same CFG shape), but the reference set is different:

| slot | refs (block freq) | weight | offset |
|---|---|---|---|
| `dt` | preheader store (1.0) + for.body (31.0) + cluster (3.6328) + trail-block (12.109) | **47.742** | 24 |
| `this` | store (1.0) + launch (0.625) + cluster ×3 (3×3.6328) | **12.523** | 20 |
| `elapsed` | store (1.0) + `if.then72` (0.11719) + `if.else` (0.39063) + `itemEmit` (0.058594) + fade (9.6875) | **11.254** | 16 |
| `c` | cluster store+load (2×3.6328) | **7.266** | 12 |
| `s` | cluster store+load (2×3.6328) | **7.266** | 8 |
| guard | (2.0) | 2.0 | 4 |

**This resolves the apparent contradiction in section 2.** The original's
`ldr.w sl, [sp, #20]` at line 530 is *inside the cluster block* (it feeds the
third `+=`, still inside `if (weaponType == 40)`), not in the following block.
So the original has only **three** `this` reloads in the loop, all at 3.6328,
and `this` weighs 12.52 — just 1.27 above `elapsed`'s 11.25. The ordering
`dt > this > elapsed > c = s > guard` then reproduces the original's offsets
exactly.

### 8.3 What must change

Two independent defects, both now quantified:

1. **`this` is 24.2 too heavy** (36.74 vs 12.52) because of the two extra
   trail-block reloads (`0x6ac`, `0x6b8`). They disappear iff the sret pointer
   of the trail-block `Vector::operator*` takes `r6` instead of `sl`.
2. **The `trailMatrices` slot must not exist** (original weight table has no
   trail entry). With the local removed, however, `this` is reloaded far more
   (faithful form: `this` 72.27, `dt` 44.90, order `this > dt`), so the local
   is currently what keeps `this` cheap.

The only source lever that changes (1) is the `dt` reload placement: if `dt`
is live in `r4` when the sret pointer is chosen, the pointer takes `r6`
(original), `s` spills (7.27), `c` moves to 12, `elapsed` to 16 and `strd`
fires. In our build `dt` is reloaded after `Cosf`, so `r4` is free and takes
the sret pointer.

### 8.4 A/B/C subset weights (new)

Replacing individual local uses with `this->trailMatrices` (keeping the
declaration) changes the CFG enough that the model is not additive, but the
measurements are:

| variant (use replaced by member) | trail w | this w | dt w | elapsed w | pct |
|---|---|---|---|---|---|
| none (baseline) | 32.484 | 36.742 | 47.742 | 11.313 | **86.9** |
| A only | 25.105 | 72.271 | 44.904 | 5.258 | 68.2 |
| B only | 22.381 | 72.271 | 44.904 | 5.258 | 68.8 |
| C only | 20.375 | — (no slot) | 47.742 | 11.313 | 66.3 |
| A+B | 10.271 | 72.271 | 44.904 | 5.258 | 64.2 |
| A+C | 15.834 | 72.271 | 44.904 | 5.258 | 64.2 |
| B+C | 13.109 | 72.271 | 44.904 | 5.258 | 64.2 |
| all (faithful) | — | 72.271 | 44.904 | 12.040 | 57.6 |

Every member substitution pushes `this` above `dt` (72.27 > 44.90), so the
slot order flips regardless of the trail weight. This is why no A/B/C subset
can win: the local is load-bearing for `this`'s colouring.

### 8.5 New source experiments this pass (all <= 86.9)

| variant | pct | note |
|---|---|---|
| baseline | **86.9** | |
| `life` declared after `phase` (matches the original's post-`uidiv` load order) | 86.5 | weights identical; instruction order shifts |
| `life` before `total` | 86.7 | |
| `Sinf` uses `total - gun->lifetimes[i]` (no `life` local) | 86.5 | |
| `Cosf` uses `total - life` | 53.2 | |
| both trig args via member | 76.6 | |
| `n` as `int` | 86.9 | |
| `phase` via float division | 59.0 | |
| `trailMatrices->data()[i]` for the second access | 85.7 | |
| `(*trailMatrices)[i]` for the second access | 84.0 | |
| `Matrix *trail` + `->` | 84.0 | |
| `heading` computed before the first `MatrixSetTranslation` | 59.3 | |
| `first_uses_trail` / `both_via_trail` | 53.1 | |
| `RocketGun *self = this` + `self->trailMatrices` (7 subsets) | 57.6-68.8 | coalesces with `this` |
| `lifetimes` base-pointer local (both / s only) | 85.8 / 86.5 | |
| `lifeKeep` volatile after `Sinf` | 77.0 | |
| `life2 = life` after `Sinf` | 75.4 | 635 insns, 1904 B |
| `dt * 0.0f` added to the `s` / `c` / `phase` argument | 82.2-82.8 | forces a `vmul` |
| `const float zero = dt * 0.0f;` (dead) | 86.9 | folded |
| `__builtin_expect` on cluster / trail / active / ahead.z / homing (0 and 1) | 44.4-86.9 | `expect_trail1` = 86.9 |
| anchor grid `{expect,padx,unused,local}` × 2^4 | best 86.9 | `exp=0` variants give 635 insns but 83.5-84.6 |
| same grid with `life` after `phase` | best 86.5 | |

The 635-insn variants (`exp=0,padx=0,unused=0,local=1` -> 83.5;
`exp=0,padx=0,unused=1,local=1` -> 84.6) still emit
`str r6,[sp,#12]` + `str.w sl,[sp,#20]`, i.e. no `strd`: instruction count
alone is not the target.

### 8.6 Conclusion

The residual is now fully quantified as two coupled weights:

```
this  = 1 + 0.625 + 3*3.6328 + 2*12.109 = 36.742   (original: 12.523)
trail = 1 + 12.109 + 7.2656 + 12.109     = 32.484   (original: absent)
```

To match, a source shape must (a) make the trail-block sret pointer take `r6`
(dropping `this` to 12.52 and spilling `s`), and (b) have no trail slot. Both
are downstream of the `dt` reload being placed before `Sinf`; every source
reordering tried (this pass and passes 2-11, ~450 variants total) leaves it
after `Cosf`. The remaining lever is the pre-RA list-scheduler/regalloc
interaction around `%29` (`elapsed`, evicted from `r4` by `%251`, the hoisted
`%stack.4` address), not any expression shape in the source.
