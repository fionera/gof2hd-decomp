# w4115: `Level::createMission` (90.7 %, 17172 B) — the b2e04 materialised condition is a `bool`-typed `alienWave`, and it does *not* move the epilogue frequency

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w4115`, branch `wave91-w4115`,
base `a4557300`. Ground truth: `../_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; real address = Ghidra address − 0x10000).
No source change is committed: every variant below is neutral or a regression.

## Baseline (unchanged, gate green)

```text
_ZN5Level13createMissionEv  fuzzy 90.7 %  size 17172 B  @0xaffd8
normalized instruction stream: 6055 (orig) vs 6070 (ours)
gate: byte 4498 / linked 4498, size 4524/4524, stubs 0, missing 0, wrong_type 0,
      extra 0, data 843/843, imports 207, .text 0x1337c8, .rodata 0x41b67,
      .ARM.exidx 0x6f70, .ARM.extab 0x26290 (-8), lint CLEAN, drift 0,
      RATCHET PASS
```

`cmake --build cmake-build-match --target check -j8` passes before and after this
pass. `report.json` is not committed.

## Result in one line

The original's `b2e04` materialised `(i != X) | !alienWave` is reproduced exactly
by declaring `alienWave` as **`bool`** instead of `int` (the `and` operand then
stops being a sinkable `icmp`). It is a real source-shape discovery, but it
changes the function size (17156 B) and the fuzzy drops to 89.2 %, and — the
important negative result — **the epilogue block frequency is unchanged**
(`BB519 = 0.56876`), so this is *not* the lever that flips the raid/guard spill
slot. The wave-90 hypothesis that the `b2e04` shape is the slot-flip lever is
falsified for this base.

## 1. The `b2e04` materialised condition needs a bool-typed `alienWave`

Original (`b2e04`):

```asm
b2e04  subs.w r0, fp, r3       ; r0 = i - X
b2e08  mov    r1, r0
b2e0a  it     ne
b2e0c  movne  r1, #1           ; r1 = (i != X)
b2e0e  ldr    r2, [sp, #60]    ; alienWave
b2e10  eor.w  r2, r2, #1       ; !alienWave
b2e14  orrs   r1, r2
b2e16  bne.w  b2f2a            ; -> else-if (pirate check)
```

The branch condition at ISel is a materialised `or` of `(i != X)` and
`!alienWave`, not a short-circuit. Mechanism: `visitBr` only enters
`FindMergedConditions` for an `and`/`or` `BinaryOperator`. If the `and`'s
`alienWave` operand is an `icmp` in a different IR block, CodeGenPrepare's
`SinkCmpExpression` (`llvm/lib/CodeGen/CodeGenPrepare.cpp:1187`) clones the
`icmp` into the branch block, both operands become `InBlock`, and ISel
short-circuits. If the operand is a **bool `phi i1`** (not a `CmpInst`),
`SinkCmpExpression` cannot clone it, `InBlock` stays false, and
`EmitBranchForMergedCondition` materialises the whole condition — which the DAG
lowers as the inverse `or` of negations.

Evidence (baseline `-emit-llvm`, block 1410):

```llvm
; <label>:1410
  %1411 = icmp eq i32 %1403, %1404     ; i == X
  %1412 = and i1 %1124, %1411          ; %1124 = icmp ne i32 %1116, 0  (block 1123)
  br i1 %1412, label %1413, label %1529
```

`%1124` is an `icmp` in block 1123, so CGP sinks a copy into 1410 and we emit the
short-circuit:

```asm
; baseline ours: cmp i,X first, then ldr alienWave
cmp    r11, r3
bne.w  .LBB20_471
ldr    r0, [sp, #64]
cmp    r0, #0
beq.w  .LBB20_478
```

With `bool alienWave = false;` the IR becomes `%1116 = phi i1` and the branch
operand is the phi itself. The emitted shape (from the `-S` output) is:

```asm
subs.w r0, r11, r3
it     ne
movne  r0, #1
ldr    r1, [sp, #64]
eor.w  r1, r1, #1
orrs   r0, r1
bne.w  .LBB20_471
```

i.e. the original's materialised `(i != X) | !alienWave` (only the register
choice and the redundant `mov r1,r0` differ; the slot is still `sp+64` vs the
original's `sp+60`). This is the first source spelling that reproduces the shape.

### Why the other spellings do not work

* `if ((i == X) & alienWave)` (bitwise `&` on bools → `int`) produces
  `sub.w r0,r11,r3; clz r0,r0; lsrs r0,r0,#5; tst r1,r0` — an AND/test, not the
  original's OR of negations.
* `(i != X) | !alienWave` with `int alienWave` is folded by InstCombine to
  `or i1` and then short-circuited.
* `int c = (i != X) | !alienWave; if (c)` is also folded to `or i1`.
* `(i != X) | (alienWave ^ 1)` does materialise, but LICM hoists the loop-
  invariant `xor` into a stack slot (`eor; str [sp,#116]` before the loop), so
  the branch block reloads it instead of the original's in-loop `eor`; size
  17184 B, fuzzy 86.2 %.
* A `bool alien = alienWave != 0;` local is folded back to the `icmp` and
  short-circuits.

## 2. The materialised condition does not move the epilogue frequency

`-mllvm -print-machine-bfi -mllvm -print-bfi-func-name=_ZN5Level13createMissionEv`
(the second emitted section is the one used by `StackSlotColoring`):

```text
                       baseline      bool alienWave
  BB1   (entry)        0.625000      0.625000
  BB519 (epilogue)     0.568760      0.568760
  guard weight         1.193760      1.193760
```

The guard-address spill is still at `sp+84` (raid at `sp+88`) in the
`bool alienWave` build, i.e. the raid/guard slot does **not** flip. The wave-90
note argued the flip needs `epilogue > 0.572958`; reproducing the original's
materialised condition leaves it at `0.568760`. The b2e04 CFG shape is therefore
not the upstream lever for the slot swap.

The `bool alienWave` variant also changes the frame/rodata layout: size
**17156 B** (16 B smaller), fuzzy **89.2 %**, normalized stream 6064 vs 6055
instructions, `byte_exact` 3086 (size change shifts the link layout). Not
landable.

## 3. The current base's raid weight is not the wave-90 number

The wave-90 note's raid decomposition (`raid = 1.197958`, guard `1.193760`,
margin `0.004199`) is from base `360c7467`. On this base the same 19 raid
references in the `After Stack Slot Coloring` machine dump have

```text
  raid  = 1.301489   (BFI section 1, block list 23,30,35,44,52,63,67,69,71,
                      89,99,100,106,110,114,118,138,189,258)
  guard = 1.193760   (BB1 + BB519)
  margin = 0.107729, not 0.004199
```

so the slot gap on this base is ~26x larger than the wave-90 note states. Any
future flip experiment must be re-measured against this base, not the old
numbers. (The block numbering shifted by one at two places between the bases;
the note's `bb.24/68/70` are this base's `bb.23/67/69`.)

## 4. Other variants measured this pass

| variant | size | fuzzy | guard_sp | note |
|---|---:|---:|---:|---|
| **baseline** | **17172** | **90.7** | **84** | |
| `bool alienWave` | 17156 | 89.2 | 84 | materialises b2e04; 16 B smaller |
| `(i == X) & alienWave` | — | — | — | `clz`/`tst` shape, not the original |
| `(i != X) \| (alienWave ^ 1)` | 17184 | 86.2 | 84 | materialises, but xor hoisted by LICM |
| `__builtin_expect(raid, 0)` | 17184 | 84.8 | 84 | branch metadata changes layout |

`__builtin_expect` is recorded only as a negative control; it is not proposed as
a source shape.

## 5. What remains

1. **raid/guard slot swap**: the lever is not b2e04. On this base the raid
   weight is 1.301489 vs guard 1.193760, so the epilogue would have to rise by
   ~0.108 (not 0.004) or the raid reference mass would have to fall by the same
   amount. Candidate upstream regions are the mission-case switch probabilities
   (`bb.325` jump table: 8.75 %/17.5 %/3.75 % successors) and the `raid`
   computation (`b04c0`–`b04d8`, identical code, only the slot differs).
2. **`bool alienWave` is the correct type for b2e04** but the 16-byte size
   deficit (assignment-block `movs` vs the original's `mov.w`, and the
   alienWave/pirateFortress/bigAlienWave slot rotation `sp+64/60/56` vs the
   original's `sp+60/56/64`) must be recovered before it can be landed.
3. **convoy switch `b1408`**: unchanged from wave 90 — needs unequal `!prof`
   weights below the 66 % peel threshold.
4. **alienGuns `Vector` slot merge** and **EH landing pads**: unchanged.

## Method / commands

```sh
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8
# per-variant .s:
clang++ ... -S -o /dev/shm/w4115/v.s -c src/game/world/Level.cpp
# BFI:
clang++ ... -mllvm -print-machine-bfi -mllvm -print-bfi-func-name=_ZN5Level13createMissionEv ...
# machine CFG / spill refs:
clang++ ... -mllvm -print-machineinstrs ...
```
