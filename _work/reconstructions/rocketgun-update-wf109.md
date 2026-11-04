# `RocketGun::update(int)` — pass 14 (wf109, wave 101)

**Status:** research note, 2026-11-04. Worktree `wt/wf109`, branch
`wave101-wf109`, base `3856949ab`. **No source change landed**: every source
shape tried this pass scores <= 86.9 %. Tree clean, gate unchanged
(byte 4500, linked 4500, size 4524/4524, data 843/843, imports 207, lint
CLEAN, drift 0).

Target `_ZN9RocketGun6updateEi`: original **1908 B / 635 insns**, ours
**1908 B / 636 insns**, official verify score **86.9**.

Earlier notes: `rocketgun-update-w5010.md` (pass 13, stack-slot weight model),
`rocketgun-update-w4812.md`, `-w4710.md`, `-w4512.md`, `-w4412.md` (Ghidra
decomp appendix), and the earlier `-w4265`, `-w4248`, `-w4216`, `-w4111`,
`-w4011`, `-w3831`, `-w3811` notes.

This pass re-derived the residual with the **NDK r18b clang 7.0.2 itself**
(`-mllvm -print-machineinstrs` works; `-mllvm -debug-only=...` does not,
because the NDK binary is a release build without assertions). That gives the
real post-RA physical-register assignment for our build, and it confirms and
sharpens the wave-100 self-built-clang finding: the whole residual is one
register permutation plus the `strd` merge it prevents. The source shape is
already faithful (the pre-RA machine-IR order matches the original's
instruction order exactly); no source lever tried moves the allocation.

---

## 1. Baseline and gate (this worktree)

```
cmake -S . -B cmake-build-match \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8
```
```
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.91
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`_ZN9RocketGun6updateEi`: 1908 B both sides, **635 vs 636** instructions,
**86.9 %**. All other `RocketGun` functions are byte-exact
(`BombGun::update` is byte-exact at 100 %).

The current source carries four anchors (all documented inline):

| anchor | removal |
|---|---|
| `__builtin_expect(hasGeometry && delayActive, 1)` on the launch `if` | 84.6 (635 insns) |
| `volatile int padx = 0;` in the second launch branch | 86.7 |
| `volatile int unused = elapsed;` before the loop | 85.8 |
| `Array<Matrix> *trailMatrices = this->trailMatrices;` before the loop | 57.6 |
| all four removed (faithful form) | 61.6 (657 insns) |

The `__builtin_expect` anchor is load-bearing for the *launch block*: every
plain condition form tried (`&&`, nested `if`, `bool` variable, `!!`, `?:`,
`==1`, `&`, `*`) produces the compact `ittt ne` conditional-execution form and
scores 84.3-84.6 (635 insns). The original uses two separate `beq` branches.

---

## 2. The instruction streams are the same length

A two-column alignment of the official `--show` output (the tool's
`f"  {ol:38} {mark} {ul}"` layout) gives exactly **one** extra line on our
side, and it is a literal-pool word (`.word 0x000b0f96`), not an instruction.
All call sites match one-for-one: the same 60 `blx` sites with an identical target sequence.
The only real structural differences are:

* original `strd r4, sl, [sp,#16]` vs ours `str r5, [sp,#12]` +
  `str.w sl, [sp,#20]` (the `strd` merge, +1 instruction on our side);
* original `mov r9, r4` at the loop head (copy of the `i*60` cursor) and the
  `ldr.w sl, [sp,#20]` that follows the first `operator*` — both absent /
  moved in ours;
* a global register permutation (below).

So the residual is **not** a missing statement or a wrong constant: it is
purely register allocation plus the slot adjacency that follows from it.

---

## 3. NDK-clang post-RA register assignment (new evidence)

`-mllvm -print-machineinstrs` on the NDK r18b clang emits a NoVRegs dump
after Stack Slot Coloring. The loop body (bb.39) is:

```
$r4 = t2LDRi12 $r0, 12          ; positions.data_
$r0 = t2LDRi12 $r0, 24          ; velocities.data_
$r1 = t2ADDrr $r0, $r11         ; velocities[i]      (r11 = i*12)
$r6 = t2ADDri %stack.4, 0       ; temp1 address
... bl operator* (velocities[i] * dt)
$r5 = t2ADDri %stack.3, 0       ; temp2 address
... bl operator* (* 0.5f)
$r1 = t2ADDrr $r4, $r11         ; positions[i]
$r4 = t2ADDri %stack.7, 0       ; temp3 address
... bl operator+
$r0 = t2ADDri %stack.1, 0       ; ahead address
... bl Vector::operator=
```

and the latch is

```
$r11 = t2ADDri $r11, 12         ; i*12
$r9  = t2ADDri $r9, 60          ; i*60
$r8  = t2ADDri $r8, 1           ; i
```

The original's loop body (from the binary, 0x15f17e-0x15f1b8) is the same
sequence with a different colouring:

| value | original | ours |
|---|---|---|
| `this` | `sl` (spilled, reloaded) | `sl` (spilled, reloaded) |
| `i` | `r8` | `r8` |
| `i*12` | `fp` | `r11` |
| `i*60` cursor | **`r4`** (copied to `r9` at loop head) | **`r9`** |
| `positions.data_` | **`r6`** (then reused for temp3) | **`r4`** (then reused for temp3) |
| temp1 address | **`sl`** (after spilling `this`) | **`r6`** |
| temp2 address | `r5` | `r5` |
| temp3 address | `r6` | `r4` |

The downstream slot map follows from this:

| slot | original | ours |
|---|---|---|
| `[sp,#24]` | dt | dt |
| `[sp,#20]` | this | this |
| `[sp,#16]` | **elapsed** | **trailMatrices local** |
| `[sp,#12]` | c | **elapsed** |
| `[sp,#8]` | s | c |
| `[sp,#4]` | guard | guard |

Because our `elapsed` and `this` slots are not adjacent (12 and 20), the
allocator cannot merge the two stores into `strd`; the original's 16/20 pair
can.

---

## 4. Why the allocator picks `r9` for the cursor (NDK RA order)

The self-built clang 7.0.0 `RA_TRACE=1 -mllvm -debug-only=regalloc` trace
(the NDK release build has no assertions) shows the exact eviction chain:

```
select %251 (temp1 address, size 5598, w 8.57e-3)
  try $r4 intf=1 i0=%29(w=9.40e-4)   ; elapsed is cheap to evict
  assign %251 -> $r4                   ; elapsed evicted to r8
select %252 (temp2 address)
  assign %252 -> $r4                   ; %251 evicted to r9
...
select %385 (i*60, size 5438, w 1.49e-2)
  try $r4 i0=%252 ; $r5 i0=%28 ; $r6 i0=%255 ; $r8 i0=%386
  try $r9 i0=%251 ; $r10 i0=%247 ; $r11 i0=%384
  assign %385 -> $r9                   ; evicts %251 from r9
select %248 (positions.data_, size 240, w 9.69e-2)
  assign %248 -> $r4                   ; evicts %252 from r4
```

So the chain is: `%251` evicts `elapsed` from `r4`; `%252` takes `r4`; the
`i*60` induction variable is forced to `r9`; `positions.data_` then takes
`r4`. The original's `elapsed` is not live across the loop in a register (it
is spilled at the launch block and reloaded in the fade), so `%251` does not
interfere with it and the chain starts differently.

The wave-100 `FORCEEVICT` experiment reproduces **88.3 %** by forcing the
entry copy of `elapsed` into `r4`; the frame slots stay 12/20, so it is not
byte-exact but it confirms register colouring and slot assignment are
independent.

---

## 5. Measurements this pass (official verifier)

All 16 combinations of the four anchors were measured, plus the 16
combinations with `life` moved after `phase` (32 runs). The base is the
**unique** maximum:

| padx | unused | trail | expect | life after phase | pct | insns |
|---|---|---|---|---|---|---|
| 1 | 1 | 1 | 1 | 0 | **86.9** | 636 |
| 1 | 1 | 1 | 0 | 0 | 86.7 | 636 |
| 1 | 1 | 1 | 1 | 1 | 86.5 | 636 |
| 0 | 1 | 1 | 1 | 0 | 86.7 | 636 |
| 0 | 0 | 1 | 1 | 0 | 85.6 | 636 |
| 1 | 0 | 1 | 1 | 0 | 85.8 | 636 |
| 1 | 1 | 1 | 0 | 0 | 84.6 | 635 |
| 1 | 1 | 0 | 1 | 0 | 57.6 | 626 |
| 0 | 0 | 0 | 0 | 0 | 61.6 | 657 |
| 0 | 0 | 0 | 0 | 1 | 61.3 | 657 |

Other source shapes tried this pass (all <= 86.9):

| variant | pct |
|---|---|
| `life` after `phase` (matches original's post-`uidiv` load order) | 86.5 |
| no `total`/`n` locals (inline `gun->initialLifetime` / `count`) | 86.7 |
| `s`/`c` at function scope or loop top | 86.9 |
| `Gun &g` removed / `gun = this->gun` reloads removed | 86.9 |
| loop as `while` / `int i` / `size_t i` | 86.9 |
| `dt` moved before the launch block | 80.9 |
| `dt` inside the loop | 86.9 |
| no `dt` local (`(float) elapsed` at each use) | 86.9 |
| `int dt = elapsed` + casts | 86.9 |
| `int frameTime = elapsed` used in the fade | 86.9 / 85.8 |
| volatile `frameTime` | 52.9 |
| `Vector scaled = vel * dt; scaled = scaled * 0.5f;` | 48.3 |
| `halfDt` scalar | 55.6 |
| `(*trailMatrices)[i]` -> `trailMatrices->data()[i]` | 85.3 |
| `this->trailMatrices` for the first `MatrixSetTranslation` | 85.3 |
| `trailMatrices->data()[i]` for the third access | 85.7 |
| `heading` declared before `MatrixSetTranslation` | 59.3 |
| `cross`/`heading` at function scope | 59.5 / 57.2 |
| `cross` first, `c` first, cluster `+=` swapped | 57.1 / 45.2 / 56.6 |
| local `AEGeometry *geometry` for `setDirection` | 86.7-86.9 |
| inline `MatrixGetDir` in `setDirection` | 85.8 |
| `up` before `forward` | 85.9 |
| `volatile` `gun` loads (BombGun-style) at 5 sites | 66.4-86.9 |
| extra `volatile int` anchors at 7 sites x 5 values | all < 86.9 |
| `(void)` uses of 11 locals | 86.9 |
| `fadeTimer`/`activeShotId`/`hitSmall` store reordering | 85.9-86.4 |
| `padx` type/value sweep (int/float/long/char, 5 values) | 86.0-86.9 |
| `unused` type/value sweep | 84.5-86.9 |

The faithful form (no anchors, `life` after `phase`) is **61.3 % / 657
insns**; the tuned base is **86.9 % / 636 insns**. Both are reported as
required.

---

## 6. Conclusion and remaining hypothesis

The source shape is faithful: the pre-RA machine-IR order for the loop body
(`positions.data_` load, `velocities.data_` load, temp1 address, `operator*`,
temp2 address, `operator*`, `positions[i]`, temp3 address, `operator+`,
`ahead` address, `Vector::operator=`) matches the original's instruction
order exactly, and every call site and constant matches. The residual is a
single coupled register/slot permutation:

```
original:  r4 = i*60,  r6 = positions.data_,  sl = temp1,  elapsed@16
ours:      r9 = i*60,  r4 = positions.data_,  r6 = temp1,  elapsed@12
```

The root cause is the MachineLICM-hoisted `t2ADDri %stack.4.ref.tmp`
address (`%251`): it evicts `elapsed` from `r4`, which starts the chain
`%251 -> r4 -> r9`, `%252 -> r4`, `%385 (i*60) -> r9`, `%248
(positions.data_) -> r4`. To match, a source shape must simultaneously
(1) keep `elapsed` in `r4` through the launch block so its spill lands at
`[sp,#16]` next to `this` and merges to `strd`, (2) keep `this` below `dt`
in the slot weight order, and (3) make `s` spill instead of the trail local.
Every lever tried (declaration order, lifetimes, named temporaries,
parenthesisation, volatile anchors, loop form, `data()` vs `operator[]`,
`this->gun` vs local) moves at most one of the three; the remaining lever is
the pre-RA scheduler / greedy allocator's choice of register for the hoisted
temp address, which no source expression shape tried so far changes.

The only known way to reproduce the original colouring is the wave-100
`FORCEEVICT` hack (88.3 %, not byte-exact, not a source change).

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

# NDK machine IR (NDK clang supports -print-machineinstrs, not -debug-only)
.cache/ndk/android-ndk-r18b/toolchains/llvm/prebuilt/linux-x86_64/bin/clang++ \
  <RocketGun.cpp flags> -mllvm -print-machineinstrs 2>&1 | \
  sed -n '/After Stack Slot Coloring:/,/End machine code for function _ZN9RocketGun6updateEi/p'

# RA trace (self-built clang 7.0.0 with assertions, /tmp/llvm_android_702b)
RA_TRACE=1 clang++ <RocketGun.cpp flags> -mllvm -debug-only=regalloc 2>&1 | \
  grep -A60 'Function: _ZN9RocketGun6updateEi'
```

Scratch lives in `/dev/shm/wf109`: `base.cpp`, `try.sh` (copy variant +
`ninja link_so` + score), `score.py` (single-symbol official verifier),
`mi_ndk.log` (NDK `-print-machineinstrs`), `ra_trace.log` (self-built RA
trace), `orig.asm` / `ours.asm` / `faithful.asm`, and the `batch*.py` /
`sweep*.py` variant generators; none of it is in the repo.
