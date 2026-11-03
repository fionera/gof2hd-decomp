# `RocketGun::update(int)` — pass 4 (w4111, wave 91)

**Status:** research note, 2026-11-03. Worktree `wt/w4111`, branch `wave91-w4111`,
base `a4557300`. No source change landed: the best faithful variant is still the
current baseline (86.9 %). Tree clean, gate unchanged (byte 4498 / linked 4498,
size-exact 4524/4524, `.text 0x1337c8`, imports 207, lint `CLEAN ✓`, drift 0).

Target `_ZN9RocketGun6updateEi`: original **1908 B / 635 insns**, ours
**1908 B / 636 insns**, official verify score **86.9**. This pass re-derived the
MachineLICM hoist set, the preheader ordering that decides the greedy-allocator
priority order, and ran a per-TU `-no-stack-coloring` experiment that isolates
the root cause. See `rocketgun-update-w4011.md` (pass 3),
`rocketgun-update-w3831.md` (pass 2) and
`rocketgun-update-86-9-1908-b_w3811.md` for the earlier spill-weight analysis.

## 1. Baseline (NDK r18b clang 7.0.2, this worktree)

```
cmake --build cmake-build-match --target check -j8
verify: byte 4498  linked 4498  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`verify.py --show _ZN9RocketGun6updateEi`: 1908 B both sides, 635 vs 636
instructions, **86.9 %**. `drift` = 0 fields, `lint` = `CLEAN ✓`.

Frame slots (linked assembly):

| slot | original | ours |
| --- | --- | --- |
| stack guard | `[sp,#4]` | `[sp,#4]` |
| `s` (`Sinf` result) | `[sp,#8]` | kept in `r6` |
| `c` (`Cosf` result) | `[sp,#12]` | `[sp,#8]` |
| `elapsed` | `[sp,#16]` | `[sp,#12]` |
| `trailMatrices` local | none | `[sp,#16]` |
| `this` | `[sp,#20]` | `[sp,#20]` |
| `dt` | `[sp,#24]` | `[sp,#24]` |
| `unused` anchor | none | `[sp,#28]` |
| `ahead` | `[sp,#144]` | `[sp,#144]` |
| stack guard copy | `[sp,#156]` | `[sp,#156]` |

Register roles:

| value | original | ours |
| --- | --- | --- |
| `elapsed` | `r4` until launch, then `[sp,#16]` | `r5` until preheader, then `[sp,#12]` |
| `this` | `sl`, spilled `[sp,#20]` | `sl`, spilled `[sp,#20]` |
| `player` | `r5` | `r4` |
| `i*60` trail cursor | `r4` (`adds r4,#60`), saved in `r9` | `r9` (`add.w r9,r9,#60`) |
| `i*12` | `fp` | `fp` |
| `i` | `r8` | `r8` |

The loop body is otherwise instruction-for-instruction identical. The residual
diff is a single allocator colouring permutation: `elapsed` keeps `r4` in the
original (and the cursor then reuses `r4` after `elapsed` dies at the
preheader), while ours loses `r4` to a hoisted sret-address vreg and ends with
`elapsed` in `r5` / cursor in `r9`.

## 2. New evidence: the MachineLICM hoist set

`-mllvm -debug-only=machinelicm -mllvm
-filter-print-funcs=_ZN9RocketGun6updateEi` (self-built LLVM 7.0.0; the NDK
clang accepts the same `-mllvm` flags) prints the loop-invariant instructions
that are hoisted to the loop preheader. The `t2ADDri` entries are:

```
Hoisting %251:gprnopc = t2ADDri %stack.4.ref.tmp, 0, ...
Hoisting %252:gprnopc = t2ADDri %stack.3.barrel, 0, ...
Hoisting %255:gprnopc = t2ADDri %stack.7.tmp, 0, ...
Hoisting %256:gprnopc = t2ADDri %stack.1.ahead, 0, ...
Hoisting %340:gprnopc = t2ADDri %stack.4.ref.tmp, 0, ...   (CSE'd)
Hoisting %341:gprnopc = t2ADDri %stack.3.barrel, 0, ...    (CSE'd)
Hoisting %344:gprnopc = t2ADDri %stack.7.tmp, 0, ...       (CSE'd)
Hoisting %345:gprnopc = t2ADDri %stack.1.ahead, 0, ...     (CSE'd)
Hoisting %353:gprnopc = t2ADDri %stack.7.tmp, 0, ...       (CSE'd)
Hoisting %357:gprnopc = t2ADDri %stack.3.barrel, 0, ...    (CSE'd)
Hoisting %370:gprnopc = t2ADDri %stack.7.tmp, 0, ...       (CSE'd)
Hoisting %301:gprnopc = t2ADDri %stack.7.tmp, 0, ...       (CSE'd)
Hoisting %302:gprnopc = t2ADDri %stack.6.up, 0, ...
Hoisting %308:gprnopc = t2ADDri %stack.4.ref.tmp, 0, ...   (CSE'd)
Hoisting %310:gprnopc = t2ADDri %stack.3.barrel, 0, ...    (CSE'd)
Hoisting %321:gprnopc = t2ADDri %stack.4.ref.tmp, 0, ...   (CSE'd)
Hoisting %322:gprnopc = t2ADDri %stack.3.barrel, 0, ...    (CSE'd)
```

After MachineLICM's CSE the preheader holds **five unique sret-address vregs**:
`stack.4.ref.tmp`, `stack.3.barrel`, `stack.7.tmp`, `stack.1.ahead`,
`stack.6.up`.  They are the sret destinations of the four `AEMath::operator*` /
`operator+` calls in the loop plus the `Vector::operator=` destination `ahead`.

`t2ADDri` (the shifted-imm variant, `ARMInstrThumb2.td:769`) is
`isReMaterializable = 1`, and `TargetInstrInfo::isTriviallyReMaterializable`
returns true for a frame-index address with no virtual-register operands
(`TargetInstrInfo.cpp:912-1000`).  `MachineLICM::IsProfitableToHoist` therefore
returns true unconditionally for it (`MachineLICM.cpp:1206`); the only escape
(`CheapInstr && HasLoopPHIUse`, lines 1195-1202) cannot fire because the use
chain hits the physical `$r0 = COPY %251` and `HasLoopPHIUse` skips physical
defs (`MachineLICM.cpp:1044-1075`).

## 3. New evidence: why the cursor can never outrank the hoisted addresses

Greedy RA priority for global ranges is `(1<<31)|(1<<29)+Size` (plus the
`1<<30` hint bit), i.e. **longest interval first** (`RegAllocGreedy.cpp:720-766`).
The live intervals at the start of greedy RA (from
`-mllvm -debug-only=regalloc`) are:

| vreg | value | interval | size | weight |
| --- | --- | --- | --- | --- |
| `%29` | `elapsed` | `[16r,9648B)[9840B,15104B)` | 14894 | 9.400839e-04 |
| `%251` | `&stack.4` (1st sret) | `[9312r,9648B)[9840B,15104B)` | 5598 | 8.566814e-03 |
| `%252` | `&stack.3` (2nd sret) | `[9328r,...` | 5598 | 9.612001e-03 |
| `%255` | `&stack.7` (3rd sret) | `[9360r,...` | 5598 | 1.007480e-02 |
| `%256` | `&stack.1.ahead` | `[9376r,...` | 5536 | 3.753831e-03 |
| `%386` | `i` | `[9488r,...` | 5438 | 2.805497e-02 |
| `%385` | `i*60` cursor | `[9472r,...` | 5440 | 1.493983e-02 |

Allocation cascade (`ra.txt`):

1. `%29` elapsed -> `$r4` (first, size 14894).
2. `%251` (size 5598, weight 8.57e-3) `shouldEvict`s `%29` (weight 9.4e-4)
   and takes `$r4`; `%29` -> `$r8`.
3. `%252` (9.61e-3) evicts `%251` from `$r4`; `%251` -> `$r9`.
4. `%255` -> `$r6`, `%256` -> `$r11`; `%386` (`i`) evicts `%29` from `$r8`.
5. `%29` runs `RS_Split Cascade 4`, is re-queued and finally lands in `$r5`.
6. `%385` (cursor, size 5440) is processed last among the loop-spanning
   ranges. `$r4` is held by `%252` ("cost too high"), `$r9` by `%251`;
   it evicts `%251` from `$r9` and takes `$r9`.

**The decisive point is the preheader ordering.** MachineLICM runs before
"Eliminate PHI nodes for register allocation" (pass order in
`-print-after-all`: *Early Machine Loop Invariant Code Motion* at dump 112069,
*Eliminate PHI nodes* at dump 124087).  LICM splices the hoisted instructions
in before the preheader terminator (`MachineLICM.cpp:1460`); PHI elimination
then inserts the induction-variable copies at the same point, i.e. **after**
the hoisted block.  The greedy dump confirms the order:

```
9392B  %372:spr  = VLDRS %const.3          <- hoisted constant
9424B  %281:spr  = VLDRS %const.4          <- hoisted constant
9456B  %384:rgpr = t2MOVi 0                <- i*12  (PHI copy)
9472B  %385:rgpr = t2MOVi 0                <- i*60 cursor (PHI copy)
9488B  %386:rgpr = t2MOVi 0                <- i     (PHI copy)
```

The hoisted addresses are defined at 9312-9440, the cursor copy at 9472: the
addresses are always ~160 slot-index units (10 instructions) longer than the
cursor, so by the longest-first rule they are *always* allocated before it.
This is independent of the source shape as long as the addresses are hoisted.

## 4. Per-TU `-no-stack-coloring` experiment (mechanism proof, not landable)

The five sret addresses are shared between the first and the second `ahead`
computation because StackColoring merges the disjoint compiler temporaries of
the two clusters (both clusters use `sp+116`, `sp+128`, `sp+40` in the
original too, so the original also has stack colouring active).  To test the
mechanism, `-mllvm -no-stack-coloring` was added for `RocketGun.cpp` only:

```
set_source_files_properties("${CMAKE_SOURCE_DIR}/src/game/weapons/RocketGun.cpp"
    PROPERTIES COMPILE_OPTIONS "-mllvm;-no-stack-coloring")
```

Result: `n=637`, **cursor in `r4`** (`adds r4,#60`), `c` at `[sp,#12]`, no
`trailMatrices` stack slot, `elapsed` in `r9`, frame 592 B.  Official score
**48.4 %**, and it also breaks the other RocketGun functions
(`byte_exact` 4498 -> 2923), so it is not landable.  It does prove that the
cursor can take `r4` once the two clusters' sret addresses are distinct
(shorter intervals), and that the merged-slot hoisting is the blocker.

## 5. Variants measured this pass (all reverted)

Markers: `n` = our instruction count, `r4` = `mov r4,r1` in the entry,
`cur4` = `adds r4,#60`, `cur9` = `add.w r9,r9,#60`, `c12` = `str r0,[sp,#12]`,
`tm16` = `str r0,[sp,#16]`, `anch` = `str rX,[sp,#28]`.  "official" is the
verify score (autojunk off).

| variant | n | r4 | cur4 | cur9 | c12 | tm16 | official |
| --- | --- | --- | --- | --- | --- | --- | --- |
| baseline | 636 | 0 | 0 | 1 | 0 | 1 | **86.9** |
| `Vector vel = v; ahead = p + vel*dt*0.5f` | 645 | 0 | 0 | 0 | 1 | 0 | - |
| `Vector pos = p; ahead = pos + v*dt*0.5f` | 631 | 0 | 0 | 1 | 1 | 1 | - |
| `ahead = p; ahead += v*dt*0.5f` | 635 | 0 | 0 | 0 | 1 | 0 | 62.5 |
| `ahead = p + v*(dt*0.5f)` | 625 | 0 | 0 | 0 | 1 | 0 | - |
| `Vector &pp=p; Vector &vv=v;` | 636 | 0 | 0 | 1 | 0 | 1 | - |
| function-scope `Vector tmp` | 639 | 0 | 0 | 0 | 1 | 0 | - |
| `this->gun->` in first cluster | 636 | 0 | 0 | 1 | 0 | 1 | - |
| `Vector tmpv[2]; tmpv[i&1] = ...` | 643 | 0 | 0 | 0 | 1 | 0 | 48.0 |
| `tmpv[2]` + no `unused` | 643 | 0 | 0 | 0 | 0 | 1 | - |
| `tmpv[2]` + no tm local | 646 | 0 | 0 | 0 | 0 | 1 | - |
| `tmpv[2]` + both | 646 | 0 | 0 | 0 | 0 | 0 | - |
| `Vector tmpv[2]; Vector *tp=tmpv; tp[0]=...` | 644 | 0 | 0 | 0 | 1 | 0 | - |
| `Vector tmpv[2]; Vector &tp=tmpv[0];` | 644 | 0 | 0 | 0 | 1 | 0 | - |
| `tmpv[i]` (out-of-bounds index) | 641 | 0 | 0 | 0 | 1 | 0 | - |
| `tmpv[gun->currentProjectile & 1]` | 651 | 0 | 0 | 0 | 1 | 0 | - |
| `Vector tmp1, tmp2;` (first/second cluster) | 654 | 0 | 0 | 0 | 1 | 0 | - |
| `tmp1` only | 639 | 0 | 0 | 0 | 1 | 0 | - |
| `tmp2` only | 643 | 0 | 0 | 0 | 0 | 1 | - |
| no `unused` anchor | 636 | 0 | 0 | 1 | 0 | 1 | 85.8 (w4011) |
| no `trailMatrices` local | 626 | 0 | 0 | 0 | 1 | 0 | 57.6 (w4011) |
| no both | 624 | 0 | 0 | 0 | 1 | 0 | 57.4 (w4011) |
| `life` inline in Sinf only | 636 | 0 | 0 | 1 | 0 | 1 | - |
| `life` inline in Sinf and Cosf (Z3) | 636 | 0 | 0 | 1 | 1 | 1 | 76.6 |
| `phase` before `life` | 636 | 0 | 0 | 1 | 0 | 1 | - |
| `const volatile float s = Sinf(...)` | 637 | 0 | 0 | 1 | 1 | 0 | - |
| `const float *sptr = &s; ... (*sptr+*sptr)` | 636 | 0 | 0 | 1 | 0 | 1 | - |
| `Matrix *tb = trailMatrices->data(); tb[i]` | 638 | 0 | 0 | 0 | 1 | 0 | - |
| `uint32_t i = 0;` hoisted before loop | 636 | 0 | 0 | 1 | 0 | 1 | - |
| anchor `volatile float = dt` | 637 | 0 | 0 | 1 | 0 | 1 | - |
| anchor `volatile int = 0` | 638 | 0 | 0 | 1 | 0 | 1 | - |
| two anchors | 638 | 0 | 0 | 1 | 1 | 0 | - |
| anchor `volatile Vector = ahead` | 636 | 0 | 0 | 1 | 0 | 1 | - |
| `-no-stack-coloring` (flag) | 637 | 0 | **1** | 0 | 1 | 0 | 48.4 |
| `-disable-machine-licm` (flag) | 666 | 0 | 0 | 0 | 1 | 0 | - |

No faithful source shape tried in this pass (or in passes 2/3) puts `elapsed`
in `r4`.  The only way observed to get the cursor into `r4` is to stop the two
clusters' sret temporaries from being stack-coloured into one slot
(`-no-stack-coloring`), which changes the whole frame and destroys the match.

## 6. Root cause / what remains

The residual is one allocator decision:

* `elapsed` is allocated first (size 14894) and takes `r4`.
* The first MachineLICM-hoisted sret-address vreg `%251` (size 5598, weight
  8.57e-3) evicts it (elapsed weight 9.4e-4) and takes `r4`; `%252` then takes
  `r4`, `%251` `r9`, `%255` `r6`, `%256` `r11`.
* The `i*60` cursor (size 5440) is processed after all of them and can only
  get `r9`; the original has it in `r4` with an `r9` save/restore around the
  cluster-rocket calls.
* Because `elapsed` is in `r5` (not `r4`) and `this` in `sl`, the launch block
  emits two stores (`str r5,[sp,#12]` + `str.w sl,[sp,#20]`) instead of the
  original `strd r4, sl, [sp,#16]`; the `s`/`c` spill slots and the
  `trailMatrices` local follow from the same colouring.

To match, a source shape must make the five hoisted `t2ADDri %stack.N,0` vregs
either (a) not hoisted, or (b) defined *after* the induction-variable PHI
copies in the preheader, so that the cursor's live interval is the longest.
(a) requires the address computation to carry a virtual-register operand (so
`isTriviallyReMaterializable` is false); every such shape tried
(`tmpv[i&1]`, `tmpv[i]`, pointer/ref temps, runtime index) adds 5-15
instructions and/or collapses the loop colouring.  (b) is not reachable from
C++: LICM always runs before PHI elimination, so hoisted instructions always
precede the PHI copies.

One untried avenue remains: a source shape in which the first `ahead`
computation's sret temporaries are *not* stack-colour-merged with the second
cluster's (so the first cluster's addresses die early), while keeping the
frame at 160 B and the instruction count at 635.  The `-no-stack-coloring`
experiment shows that this is the right lever, but no source-level way to
prevent the merge without adding instructions was found.

## 7. Reproduction

```
cmake --build cmake-build-match --target check -j8
python3 tools/verify/verify.py \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --build-dir cmake-build-match/verify --show _ZN9RocketGun6updateEi
```

Note: the live verify report is `cmake-build-match/verify/report.json` (the
`check` target passes `--build-dir .../verify`); the repo-root `report.json`
and `cmake-build-match/report.json` can be stale.

Hoist set:

```
clang++ ... -mllvm -debug-only=machinelicm \
  -mllvm -filter-print-funcs=_ZN9RocketGun6updateEi -c RocketGun.cpp
```

Allocation:

```
clang++ ... -mllvm -debug-only=regalloc \
  -mllvm -filter-print-funcs=_ZN9RocketGun6updateEi -c RocketGun.cpp
```

The self-built LLVM 7.0.0 at `/tmp/llvm_android_702b/build2/bin/clang++`
reproduces the current baseline exactly (636 insns, elapsed `r5`, cursor `r9`)
and is ~3x faster to iterate with.
