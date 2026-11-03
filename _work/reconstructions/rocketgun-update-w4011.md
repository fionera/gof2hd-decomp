# `RocketGun::update(int)` — pass 3 (w4011)

**Status:** research note, 2026-11-03. Worktree `wt/w4011`, branch `wave90-w4011`,
base `360c7467`. No source change landed: every faithful source-shape variant tried
this pass scores <= 86.9 %. Tree clean, gate unchanged (byte 4498 / linked 4498,
size-exact 4524/4524, `.text 0x1337c8`, imports 207, lint `CLEAN ✓`, drift 0).

Target `_ZN9RocketGun6updateEi`: original **1908 B / 635 insns**, ours
**1908 B / 636 insns**, official verify score **86.9**. This pass re-derived the
real-clang post-RA allocation from the NDK clang itself (not the self-built
LLVM 7.0.0 tree), corrected the frame-layout description in the wave-88/89
notes, and swept ~60 more source shapes. See
`rocketgun-update-86-9-1908-b_w3811.md` and `rocketgun-update-w3831.md` for the
earlier spill-weight analysis.

## 1. Corrected baseline (NDK r18b clang 7.0.2, this worktree)

`cmake --build cmake-build-match --target check -j8`:

```
verify: byte 4498  linked 4498  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`verify.py --show _ZN9RocketGun6updateEi` gives 1908 B on both sides, 635 vs
636 instructions, score 86.9.

**Frame layout (corrected).** Both sides use a 160-byte frame; the earlier
notes' "ours = 144 B, `ahead` in the preheader" description was a stale
`ours.txt` from a different source state. The current NDK build has the
`ahead` zero-init in the *entry* block exactly like the original:

| slot | original | ours (baseline) |
| --- | --- | --- |
| `s` (`Sinf` result) | `[sp,#8]` | kept in `r6` |
| `c` (`Cosf` result) | `[sp,#12]` | `[sp,#8]` |
| `elapsed` | `[sp,#16]` | `[sp,#12]` |
| `trailMatrices` local | none (4x `ldr [sl,#216]`) | `[sp,#16]` |
| `this` | `[sp,#20]` | `[sp,#20]` |
| `dt` | `[sp,#24]` | `[sp,#24]` |
| `unused` anchor | none | `[sp,#28]` |
| `ahead` | `[sp,#144]` | `[sp,#144]` |
| stack guard | `[sp,#156]` | `[sp,#156]` |

The launch-block spill therefore differs only because `elapsed`/`this` are not
adjacent in ours: original `strd r4, sl, [sp,#16]`; ours `str r5,[sp,#12]` +
`str.w sl,[sp,#20]`.

**Register roles** (same in both except the first column):

| value | original | ours |
| --- | --- | --- |
| `elapsed` | `r4` until launch, then `[sp,#16]` | `r5` until launch, then `[sp,#12]` |
| `this` | `sl` (`r10`), spilled `[sp,#20]` | `sl`, spilled `[sp,#20]` |
| `dt` | `[sp,#24]` | `[sp,#24]` |
| `i*60` trail cursor | `r4` (`adds r4,#60`), saved in `r9` at loop top | `r9` (`add.w r9,r9,#60`) |
| `i*12` | `fp` (`r11`) | `fp` |
| `i` | `r8` | `r8` |
| `Sinf` result `s` | `[sp,#8]` | `r6` |
| `Cosf` result `c` | `[sp,#12]` | `[sp,#8]` |
| `trailMatrices` | no local, 4 direct `[sl,#216]` loads | `[sp,#16]` local |

The loop body is otherwise instruction-for-instruction identical (same calls,
same order, same literal pool). The residual diff is register colouring plus the
two encodings that follow from it (`adds r4,#60` vs `add.w r9,r9,#60`, and the
`strd` above). The one-instruction count delta is exactly the `trailMatrices`
local load/store pair (+2) and the `unused` anchor store (+1), minus the
original's cursor save/restore `mov r9,r4` / `mov r4,r9` (-2).

## 2. Real-clang post-RA allocation (NDK clang, `-mllvm -debug-only=regalloc`)

The NDK clang accepts `-mllvm -debug-only=regalloc -mllvm
-filter-print-funcs=_ZN9RocketGun6updateEi`. Spill weights (identical to the
w3811 numbers, re-confirmed):

| vreg | value | weight |
| --- | --- | --- |
| `%29` | `elapsed` | **9.400839e-04** |
| `%251` | `t2ADDri %stack.4.ref.tmp, 0` | **8.566814e-03** |
| `%252` | `t2ADDri %stack.3.barrel, 0` | **9.612001e-03** |
| `%255` | `t2ADDri %stack.7` (Matrix frame) | **1.007480e-02** |
| `%256` | `t2ADDri %stack.1` (`ahead`) | **3.753831e-03** |
| `%384` | `i*12` induction | 3.273002e-02 |
| `%385` | `i*60` trail cursor | 1.493983e-02 |
| `%386` | `i` | 2.805497e-02 |

Allocation cascade (`ra_base.log`):

1. `%29` elapsed -> `$r4` (first, longest live range 14894).
2. `%251` (weight 8.57e-3) `tryEvict`s `%29` (9.4e-4) out of `$r4`; `%29` -> `$r8`.
3. `%386` (`i`, 2.8e-2) evicts `%29` from `$r8`.
4. `%29` runs `RS_Split Cascade 4`; `Split for $r10 in 21 bundles` (cost 8.5),
   then the second round spills it: `queuing new interval ... weight 9.4e-4`.
5. `%105` then `%39` (`player`) take the now-free `$r4`.
6. `%385` (cursor) is allocated after `%252`/`%251` already own `r4`/`r9`, so it
   ends in `r9`; the original instead keeps the cursor in `r4` with an `r9`
   save/restore across the cluster-rocket calls.

`%251`/`%252` are the sret buffers of the loop's `Vector operator*(const
Vector&, float)` calls. They are *hoisted to the loop preheader* by MachineLICM
(they are `t2ADDri %stack.N, 0`, trivially rematerializable) and their live
ranges span the whole loop, so they are ~9x heavier than the whole-function
`elapsed` live range. `-mllvm -disable-machine-licm` does put `elapsed` in `r4`
but the loop colouring collapses (1884 B / 666 insns / 48.9).

**MIR placement of the `ahead` init.** `-mllvm -print-after-all` shows the
zero-store triple in `bb.0` (entry) at every MIR stage (ISel, machine-sink,
greedy RA, post-RA sink, PEI). The `-print-after-all` dumps use *uncoloured*
frame objects (160-byte frame, `ahead` at `[sp,#144]`), which is why they cannot
be used to read the final slot offsets; the linked assembly is the authority.
The self-built LLVM 7.0.0 tree reproduces the current baseline exactly (frame
160, `ahead` in the entry, `elapsed` in `r5`, cursor `r9`), so it is a valid
fast oracle for this function.

## 3. Variants measured this pass (all reverted; baseline 86.9)

Every row was built with the full `link_so` and scored with the official
`verify.score` (autojunk off). "n" is our instruction count.

| variant | size | n | score |
| --- | --- | --- | --- |
| baseline | 1908 | 636 | **86.9** |
| `ahead` declared before `dt` (`abd`) | 1908 | 636 | 73.2 |
| `abd` + no `unused` anchor | 1908 | 636 | 77.9 |
| `abd` + no `padx` anchor | 1908 | 636 | 73.2 |
| `abd` + no both anchors | 1908 | 636 | 77.9 |
| no `trailMatrices` local (`no_tm`) | 1880 | 626 | 57.6 |
| `no_tm` + no `unused` | 1876 | 624 | 57.5 |
| `no_tm` + no both anchors | 1872 | 622 | 57.4 |
| `no_tm` + `Gun *g0` local | 1868 | 643 | 51.3 |
| `no_tm` + no expect | 1868 | 657 | 61.5 |
| `vel0` (`const Vector vel0 = v*dt*0.5f; ahead = p + vel0;`) | 1896 | 631 | 62.9 |
| `vel0` non-const | 1896 | 631 | 62.9 |
| `vel0` in a block | 1832 | 624 | 58.6 |
| `vel0` + no `unused` | 1892 | 629 | 63.4 |
| `vel0` + no `padx` | 1892 | 629 | 62.8 |
| `vel0` + no both | 1888 | 627 | 63.4 |
| three named locals (`v1=v*dt; v2=v1*0.5f; ahead=p+v2`) | 1892 | 630 | 45.4 |
| `vel_dt` (`v1=v*dt; ahead=p+v1*0.5f`) | 1896 | 634 | 63.2 |
| `vel_only` (`v1=v; ahead=p+v1*dt*0.5f`) | 1920 | 645 | 56.7 |
| `pos_loc` (`p0=p; ahead=p0+v*dt*0.5f`) | 1836 | 631 | 55.3 |
| `both_loc` (`p0=p; v0=v; ahead=p0+v0*dt*0.5f`) | 1936 | 645 | 67.0 |
| `ahead = v*dt*0.5f + p` (reversed operands) | 1828 | 628 | 60.0 |
| `ahead = p; ahead += v*dt*0.5f` | 1904 | 635 | 62.5 |
| `ahead = p + v*(dt*0.5f)` | 1824 | 625 | 55.6 |
| `const float half = dt*0.5f; ahead = p + v*half` | 1824 | 625 | 55.6 |
| `ahead = p + v*(0.5f*dt)` | 1824 | 625 | 55.6 |
| `ahead = p + v*0.5f*dt` | 1824 | 627 | 61.8 |
| `p`/`v` via `this->gun->` | 1908 | 636 | 86.9 |
| second `ahead` via `this->gun->` | 1908 | 636 | 86.9 |
| `heading` via `gun->velocities` | 1908 | 636 | 86.9 |
| `Matrix *trail = &this->trailMatrices->data()[i]` | 1908 | 636 | 86.9 |
| `Matrix &trail = (*trailMatrices)[i]` | 1908 | 637 | 84.0 |
| first `MatrixSetTranslation` via `(*this->trailMatrices)[i]` | 1912 | 638 | 84.1 |
| `heading` computed before first `MatrixSetTranslation` | 1832 | 630 | 59.3 |
| `Gun *g0 = this->gun` local, used in loop | 1868 | 643 | 44.4 |
| `trailMatrices` local moved inside loop | 1888 | 631 | 59.7 |
| `Array<Matrix> &trailMatrices` reference | 1904 | 633 | 64.2 |
| `Array<Matrix> *const trailMatrices` | 1908 | 636 | 86.9 |
| `trailMatrices` local before `dt` | 1908 | 636 | 86.9 |
| `trailMatrices` local after the `unused` anchor | 1908 | 636 | 86.9 |
| first null check direct, rest via local | 1912 | 635 | 68.2 |
| second null check + MatrixSetTranslation direct | 1904 | 633 | 64.2 |
| no `unused` anchor | 1908 | 636 | 85.8 |
| no `padx` anchor | 1908 | 636 | 86.7 |
| no both anchors | 1908 | 636 | 85.6 |
| first `if` without `__builtin_expect` | 1908 | **635** | 84.6 |
| first `if` `__builtin_expect(...,0)` | 1912 | 636 | 70.3 |
| `__builtin_expect(hitSmall != 0, 1)` | 1892 | 633 | 66.2 |
| `__builtin_expect(active != 0, 1)` | 1908 | 666 | 47.3 |
| loop-condition `__builtin_expect(...,0/1)` | 1908 | 636 | 86.9 |
| no expect + no `unused` | 1908 | 635 | 83.5 |
| no expect + no `padx` | 1908 | 635 | 84.6 |
| no expect + no both | 1908 | 635 | 83.5 |
| no expect + `abd` | 1908 | 635 | 72.0 |
| no expect + `vel0` | 1896 | 631 | 62.9 |

The `no expect` row is interesting: it is the only faithful variant that hits
the original's **635** instruction count, but it moves `elapsed` to `r6`
(`mov r6,r1` at the entry) and scores 84.6. All variants that put `elapsed` in
`r4` (`vel0` family) collapse the loop allocation to 627-634 instructions and
score <= 63.4.

## 4. Root cause / what remains

The residual is a single allocator colouring permutation:

* `elapsed` is evicted from `r4` at the loop preheader by the hoisted sret
  stack-address vregs (`%251`/`%252`, weights 8.6e-3..9.6e-3 vs elapsed's
  9.4e-4), then split (`RS_Split Cascade 4`) and finally lands in `r5`.
* `%252`/`%251` then own `r4`/`r9` through the loop, so the strength-reduced
  `i*60` cursor takes `r9` instead of the original's `r4` + `r9` save/restore.
* `elapsed` in `r5` (not `r4`) and `this` in `sl` give the two separate
  launch-block stores instead of `strd r4, sl, [sp,#16]`.
* The extra `trailMatrices` stack slot and the missing `s` spill are downstream
  of the same colouring choice.

To match, a source shape must (a) keep `elapsed` in `r4` through the
preheader, i.e. stop `%251`/`%252` from evicting it, and (b) keep `this` in
`sl` so it is spilled at `[sp,#20]`. The `vel0` family proves (a) is reachable
but changes the sret-temp stack objects enough to collapse (b) and the loop
strength reduction. The wave-88/89 "next step" — making the `t2ADDri
%stack.N,0` vregs local (not MachineLICM-hoisted) without disabling LICM
globally — remains untried in a form that keeps the 635-instruction shape.

## 5. Reproduction

```
cmake --build cmake-build-match --target check -j8
python3 tools/verify/verify.py \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --build-dir cmake-build-match --show _ZN9RocketGun6updateEi
```

The NDK clang accepts `-mllvm -debug-only=regalloc -mllvm
-filter-print-funcs=_ZN9RocketGun6updateEi` for the live-interval/weight dump.
The self-built LLVM 7.0.0 at `/tmp/llvm_android_702b/build2/bin/clang++`
reproduces this function's baseline codegen exactly and is ~3x faster to
iterate with.
