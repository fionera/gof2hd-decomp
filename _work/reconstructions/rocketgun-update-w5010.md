# `RocketGun::update(int)` — pass 13 (w5010, wave 100)

**Status:** research note, 2026-11-04. Worktree `wt/w5010`, branch `wave100-w5010`,
base `44225192c`. **No source change landed**: every source shape tried this pass
scores <= 86.9 %. Tree clean, gate unchanged (byte 4500, linked 4500, size
4524/4524, data 843/843, imports 207, lint CLEAN, drift 0).

Target `_ZN9RocketGun6updateEi`: original **1908 B / 635 insns**, ours
**1908 B / 636 insns**, official verify score **86.9**.

Earlier notes: `rocketgun-update-w4812.md` (pass 11+12, the exact stack-slot
weight model), `rocketgun-update-w4710.md` (pass 10, frame-slot map + regalloc
trace), `rocketgun-update-w4512.md` (pass 9, aligned diff + spill-weight sweep),
`rocketgun-update-w4412.md` (pass 8, Ghidra decomp appendix), and the earlier
`-w4265`, `-w4248`, `-w4216`, `-w4111`, `-w4011`, `-w3831`, `-w3811` notes.

This pass attacked the residual from the angle the wave-100 brief names:
**local declaration order and lifetimes**. It confirms the w4812 weight model
(offsets are assigned by *weight rank*, not by slot creation order), and adds a
new, precise observation about the **`Vector` stack-object placement**: the
original's `cross`/`heading` temporaries live at `[sp,#128]`, ours at
`[sp,#40]`/`[sp,#104]`. It also records a source shape that reproduces the
original's *exact* spill-slot ordering (dt@24, this@20, elapsed@16, c@12) and
the `strd` merge, but pays 5 extra instructions for it (85.5 %).

---

## 1. Baseline (NDK r18b clang 7.0.2, this worktree)

```
cmake -S . -B cmake-build-match \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
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

## 2. The slot map and the weight-rank rule (confirms w4812)

`-mllvm -debug-only=stack-slot-coloring` on the baseline:

```
Spill slot intervals:
SS#32 weight:1.131250e+01   ; elapsed
SS#33 weight:2.000000e+00   ; __stack_chk_guard GOT ptr
SS#34 weight:4.774219e+01   ; dt
SS#35 weight:3.248438e+01   ; trailMatrices local
SS#36 weight:3.674219e+01   ; this
SS#37 weight:7.265625e+00   ; c
Color spill slot intervals:
Assigning fi#34 to fi#32   ; dt -> colour 32
Assigning fi#36 to fi#33   ; this -> 33
Assigning fi#35 to fi#34   ; trail -> 34
Assigning fi#32 to fi#35   ; elapsed -> 35
Assigning fi#37 to fi#36   ; c -> 36
Assigning fi#33 to fi#37   ; guard -> 37
```

Colour 32 gets `[sp,#24]`, 33 -> 20, 34 -> 16, 35 -> 12, 36 -> 8, 37 -> 4.
So the final offsets are **exactly the weight rank** (heaviest -> highest
offset); the slot-creation order only fixes the *set* of colour indices. This
is the key correction to the wave-100 brief's "creation order" framing: for
this function the offsets are a pure weight-ordering problem.

Baseline vs original:

| rank | baseline | weight | original | weight (inferred from disasm) |
|---|---|---|---|---|
| 1 | dt | 47.742 | dt | 47.742 |
| 2 | this | 36.742 | this | ~43.52 |
| 3 | trail local | 32.484 | elapsed | 11.3125 |
| 4 | elapsed | 11.3125 | c | 7.2656 |
| 5 | c | 7.2656 | s | 7.2656 |
| 6 | guard | 2.0 | guard | 2.0 |

Original slot map (from the disassembly):

```
[sp,#24] dt      [sp,#20] this    [sp,#16] elapsed
[sp,#12] c       [sp,#8]  s       [sp,#4]  guard
```

The original's `this` weight is inferred from its reload sites: store at
`0x15edea` (freq 1.0) + launch reload `0x15eece` (0.625) + **loop-body reload
`0x15f19c`** (31.0) + three cluster reloads `0x15f2e6/0x15f2fa/0x15f318`
(3.6328 each) = 43.52. (w4812's addendum quoted 12.52 by missing the
loop-body reload; the ordering conclusion `dt > this > elapsed` is unchanged.)

The baseline has the complementary pattern: no loop-body reload, but **two
trail-block reloads** at `0x15f384`/`0x15f390` (12.109 each). The two extra
reloads exist because the trail-block `Vector::operator*` sret pointer is
`sl` (clobbering `this`), whereas the original's trail block uses `r6`/`r5`
and leaves `sl` = `this` intact.

Net: baseline `this` = 1 + 0.625 + 3*3.6328 + 2*12.109 = 36.742.
Original `this` = 1 + 0.625 + 31 + 3*3.6328 = 43.523.

---

## 3. Local declaration order / lifetimes: what was tried

All measured with the official verifier; baseline 86.9 / 636 insns. None of
these change the score or the slot map:

| variant | pct |
|---|---|
| baseline | **86.9** |
| `s`/`c` declared at loop top (`float s, c; s = ...; c = ...`) | 86.9 |
| `s`/`c` declared at function scope | 86.9 |
| `s`/`c` non-`const` | 86.9 |
| `s`/`c` uninitialized at loop top, assigned in the cluster block | 86.9 |
| `life` declared after `phase` (matches original's post-`uidiv` load order) | 86.5 |
| `dt` declared inside the loop / inside the cluster block | 86.9 |
| `dt` non-`const` | 86.9 |
| `const float s2 = s + s; const float c2 = c + c;` then use s2/c2 | 86.9 |
| `Vector ahead;` (no zero initializer) instead of `= {0,0,0}` | 86.9 |
| `Vector cross(VectorCross(...))` (direct-init) | 86.9 |
| `Vector cross = VectorCross(...)` one-line | 86.9 |
| `Vector *const pos = gun->positions.data();` before the ahead computation | 86.9 |
| remove the loop-top / second `gun = this->gun;` | 86.9 |
| loop index `int` / `unsigned` / `size_t` / explicit `off += 60` | 86.9 |
| `Matrix *trailp = &...data()[i]; Matrix &trail = *trailp;` | 86.9 |

`life` after `phase` moves the `lifetimes[i]` load after `__aeabi_uidiv`
(matching the original) but does not move the `dt` reload before `Sinf`, so it
scores 86.5.

---

## 4. New: the `Vector` stack-object placement is different

The original and our baseline also differ in the **fixed `Vector` stack
objects** (not spill slots):

| object | original | ours |
|---|---|---|
| `ahead` | `[sp,#144]` | `[sp,#144]` |
| cross / heading / 2nd `operator*` temp | `[sp,#128]` | `[sp,#128]` (2nd op* only) |
| 1st `operator*` temp | `[sp,#116]` | `[sp,#116]` |
| `up` (launch) / normalize output | `[sp,#104]` / `[sp,#40]` | `[sp,#104]` / `[sp,#104]` |
| `operator+` temp / cross | `[sp,#40]` | `[sp,#40]` |

Concretely:
* original: `VectorCross` output at `[sp,#128]`, `VectorNormalize` output at
  `[sp,#40]`;
* ours: `VectorCross` output at `[sp,#40]`, `VectorNormalize` output at
  `[sp,#104]`.

`-mllvm -debug-only=stack-coloring` shows the merge groups for our build:
`#7 tmp` absorbs `#20 cross`, `#9 frame`, `#29 tmp380`, `#31 tmp401`, `#2
muzzle`, `#17 ref.tmp246`, `#26 ref.tmp364`; `#3 barrel` absorbs `#8 heading`,
`#18`, `#21`, `#24`, `#27`, `#30 heading381`; `#4 ref.tmp` absorbs `#5
forward`, `#10 up45`, `#19`, `#22`, `#25`, `#28`; `#6 up` absorbs `#11 side`,
`#23`; `#12 ref.tmp47` absorbs `#13`, `#14`, `#15 padx`, `#16 unused`. The
original's cross/heading share `[sp,#128]`, so its merge groups differ. This
is a downstream consequence of the same instruction-scheduling / register
allocation difference, not an independent source knob: moving `cross` to
function scope, loop top, before `s`, direct-init, or one-line all either keep
the baseline layout (86.9) or change the whole allocation (52-61 %).

---

## 5. A shape that reproduces the original slot ordering — but costs 5 insns

The one new source shape that produces the original's exact spill-slot
ordering is **`Vector cross;` (default-constructed) followed by
`cross = VectorCross(...)`**, combined with removing the `trailMatrices`
local:

```cpp
Vector cross;
cross = VectorCross(this->gun->velocities.data()[i],
                    this->gun->upVectors.data()[i]);
```

Slot weights: `[44.904 (dt), 43.523 (this), 11.312 (elapsed), 7.266 (c),
2.0 (guard)]`, i.e. the exact `dt > this > elapsed > c` order. The generated
code has `strd r5, sl, [sp,#16]` (the merge) and 635 insns — but it is
**1920 B / 640 insns** and scores **85.5 %**, because the default constructor
emits a dead zero-init (`strd r0,r0,[sp,#40]` + `str r0,[sp,#48]`) and the
assignment emits an out-of-line `Vector::operator=` call. The original has
neither.

`Vector cross(VectorCross(...))` (direct-init) elides both, but reverts to the
baseline slot map (86.9). So the `cross` object only changes the allocation
when it is a *named, zero-initialized* stack object — which the original is
not.

The same variant with the `trailMatrices` local kept (`crossu` + local) gives
7 slots `[63.897, 47.742, 32.484, 11.312, 7.266, 7.266, 2.0]` and 51.6 %.

Other shapes that produce both `c` and `s` slots but keep the trail slot:

| variant | weights (sorted) | pct |
|---|---|---|
| `cross` before `s` + trail local | 47.742, 42.875, 32.484, 12.04, 7.266, 7.266, 2.0 | 57.1 (640) |
| `cross` at loop top + trail local | 47.742, 32.484, 12.04, 7.266, 7.266, 2.0 | 52.2 (626) |
| `cross` at function top + trail local | 47.742, 32.484, 11.312, 7.266, 7.266, 2.0 | 60.7 (634) |
| both `+=` as `vec * ((s+s) * dt)` | 44.109, 32.625, 16.742, 12.04, 7.266, 7.266, 2.0 | 57.6 (619) |
| `VectorNormalize(cross) * ((s+s) * dt)` only | 47.742, 43.523, 16.742, 12.04, 7.266, 2.0 | 70.0 (627) |

The last row is interesting: it reproduces the original's `this` weight
(43.523) exactly, but the trail weight is 16.742 (still above elapsed 12.04)
and the code shrinks to 627 insns.

---

## 6. A/B/C subset confirmation

Replacing individual uses of the `trailMatrices` local by `this->trailMatrices`
(keeping the declaration) reproduces w4812's table and shows the coupling:
any member substitution raises `this` above `dt`, flipping the top two slots:

| variant | trail w | this w | dt w | elapsed w | pct |
|---|---|---|---|---|---|
| none (baseline) | 32.484 | 36.742 | 47.742 | 11.312 | **86.9** |
| A only (`if (trailMatrices == nullptr)`) | 25.105 | 72.271 | 44.904 | 5.258 | 68.2 |
| B only (`if (trailMatrices != nullptr)`) | 22.381 | 72.271 | 44.904 | 5.258 | 68.8 |
| C only (`(*trailMatrices)[i]`) | 20.375 | — (no slot) | 47.742 | 11.312 | 66.3 |
| A+B | 10.271 | 72.271 | 44.904 | 5.258 | 64.2 |
| A+C | 15.834 | 72.271 | 44.904 | 5.258 | 64.2 |
| B+C | 13.109 | 72.271 | 44.904 | 5.258 | 64.2 |
| all (faithful) | — | 72.271 | 44.904 | 12.040 | 57.6 |

The local is load-bearing: without it `this` is reloaded far more and rises
above `dt` (72.27 > 44.90), flipping the two highest offsets.

---

## 7. What remains

The residual is still the two coupled weights w4812 identified, now with the
exact numbers:

```
baseline:  dt 47.742 > this 36.742 > trail 32.484 > elapsed 11.312 > c 7.266 > guard 2.0
original:  dt 47.742 > this 43.523 > elapsed 11.312 > c 7.266 = s 7.266 > guard 2.0
```

To match, a source shape must simultaneously:

1. **drop the trail slot** (the original has no `trailMatrices` local — its
   trail block loads `this->trailMatrices` three times at `0x15f32c`,
   `0x15f370`, `0x15f39a`);
2. **keep `this` at 20** (weight below `dt`'s 47.742). Without the local
   `this` rises to 72.27; the original's 43.52 comes from one loop-body reload
   (the ahead temp clobbers `sl`) plus three cluster reloads, with *no*
   trail-block reloads;
3. **make `s` spill** (weight 7.266) so the sixth colour exists and the guard
   moves to `[sp,#4]`.

Every source lever tried this pass changes at most one of the three. The
remaining lever is the pre-RA list scheduler / register allocator's choice of
the ahead-computation temp register (`sl` in the original, `r6` in ours) and
of the trail-block sret pointer (`r6` in the original, `sl` in ours). Those
choices are what put the loop-body `this` reload in the original and the two
trail-block `this` reloads in ours. No expression shape, declaration order,
lifetime extension, or `Vector`-object placement tried (this pass plus
passes 2-12, ~600 variants total) moves them.

---

## 8. Reproduction

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

# stack-slot weights (self-built clang 7, /tmp/llvm_android_702b/build2/bin/clang++)
clang++ <RocketGun.cpp flags> -mllvm -debug-only=stack-slot-coloring 2>&1 | grep -A20 "Spill slot intervals"

# stack-object merges
clang++ <RocketGun.cpp flags> -mllvm -debug-only=stack-coloring 2>&1 | grep -A40 "Function: _ZN9RocketGun6updateEi"
```

Scratch lives in `/dev/shm/w5010`: `base.cpp`, `cc.sh` (self-built clang
compile), `try.sh` (copy variant + `ninja link_so` + score), `score.py`
(single-symbol official verifier), `slots.py` / `search*.py` (weight-set
search), `batch*.py` (variant batteries), and the `v_*.cpp` variants; none of
it is in the repo.

---

## 9. Register-allocation trace: why `elapsed` lands in `r5`, not `r4`

The wave-100 brief says the residual is not register colouring, but the trace
is worth recording because it explains the coupled weight problem. With the
self-built clang (`RA_TRACE=1`, `-mllvm -debug-only=regalloc`):

```
ENQ %29 size=14894 prio=3758111278          ; elapsed (arg)
select %29 ... try $r4 intf=0
assign %29 -> $r4                            ; elapsed first takes r4
select %28 ... try $r4 intf=1 i0=%29
assign %28 -> $r5                            ; this takes r5
select %247 ... try $r4 intf=1 i0=%29
assign %247 -> $r6                           ; dt
select %251 ... try $r4 intf=1 i0=%29
TRYASSIGN2 %251 phys=$r8 cheap=$r4 cost=1
assign %251 -> $r4                           ; hoisted %stack.4 address EVICTS elapsed
ENQ %29 ... select %29
try $r4 intf=1 i0=%251 ; try $r5 intf=1 i0=%28 ; try $r6 intf=1 i0=%247
try $r8 intf=0
assign %29 -> $r8                            ; elapsed evicted to r8
... RS_Split ...
select %426 (COPY %29) ... try $r5 intf=0
assign %426 -> $r5                           ; entry copy ends in r5
```

`%251` is `t2ADDri %stack.4.ref.tmp, 0`, the loop-invariant address of the
shared `Vector` temporary (`velocities[i] * dt * 0.5f`), hoisted to the
preheader and live across the whole loop. It evicts `%29` from `r4` even at
1/1000 of its weight (pass 9's `VRW` sweep). Forcing the entry copy with
`FORCEEVICT=1 FORCEANYFN=1 FORCEVREG=2147484074:70 RA_TRACE=1` reproduces
**88.3 %** (635/636 insns, 1908 B): `elapsed`→r4, `player`/`this`→r5, but the
frame slots stay `elapsed@12, this@20` (no `strd`). So register colouring and
slot assignment are indeed independent: the 88.3 % is real but does not fix
the byte diff, and no source shape tried this pass moves `elapsed` to `r4`
(the `%251` hoist is decided by MachineLICM, and disabling it drops the score
to 48.9).

The source lever implied by the trace is to stop the three
`velocities[i] * dt * 0.5f` temporaries from sharing one stack object, so the
address is not hoisted. Every shape tried (`Vector vel = ...`, `halfDt`
scalar, parenthesisation, `pos`/`vel` pointer locals, distinct forms in the
two ahead computations) either keeps the baseline allocation (86.9) or changes
it wholesale (52-62 %).
