# w4015: `Level::createMission` (90.7 %, 17172 B) — spill-weight arithmetic, the raid/guard swap, and the convoy switch

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w4015`, branch `wave90-w4015`,
base `360c7467`. Ground truth: `../_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; real address = Ghidra address − 0x10000).
No source change is committed: every variant below is neutral or a regression.

## Baseline (unchanged, gate green)

```text
_ZN5Level13createMissionEv  fuzzy 90.721649 %  size 17172 B  @0xaffd8
normalized instruction stream: 6055 (orig) vs 6070 (ours), 5500 matched
gate: byte 4498 / linked 4498, size 4524/4524, stubs 0, missing 0, wrong_type 0,
      extra 0, data 843/843, imports 207, .text 0x1337c8, .rodata 0x41b67,
      .ARM.exidx 0x6f70, .ARM.extab 0x26290 (-8), lint CLEAN, drift 0,
      RATCHET PASS
```

`cmake --build cmake-build-match --target check -j8` passes before and after this
pass. `report.json` is not committed.

## Method: exact spill weights from `-print-machine-bfi`

The previous notes (w3815, w3835) established the *mechanism* of the raid/guard
swap but not the numbers. This pass measured them exactly.

1. `-mllvm -print-machine-bfi -mllvm -print-bfi-func-name=_ZN5Level13createMissionEv`
   emits the machine-level `BlockFrequencyInfo` (537 blocks, entry = 1.0). The
   second emitted section is the one used by `StackSlotColoring`; the first two
   sections are byte-identical.
2. `-mllvm -print-machineinstrs` gives the post-coloring machine function. Every
   `%stack.N` operand is a frame index; `StackSlotColoring::ScanForSpillSlotRefs`
   adds `LiveIntervals::getSpillWeight(false, true, MBFI, MI)` once per non-debug
   FI operand, and LLVM 7's implementation
   (`llvm/lib/CodeGen/LiveIntervals.cpp:860`) is
   `(isDef + isUse) * (MBFI->getBlockFreq(MBB).getFrequency() * (1/entryFreq))`,
   i.e. exactly the block frequency of the referencing block.
3. `ColorSlot` assigns the lowest still-free frame index to the highest-weight
   interval (`InitializeSlots` stable-sorts by weight descending;
   `ColorSlot` takes `NextColors[StackID] = AllColors[StackID].find_first()`).
   PEI then lays 4-byte slots out as `sp = 124 - 4*(fi-74)` (verified against
   the `After Prologue/Epilogue Insertion` frame-object dump:
   `fi#74 -> [SP-372]`, `fi#101 -> [SP-480]`, with a 496-byte frame).

### Measured weights (ours)

```text
  fi#   sp+   weight      name
  83    88    1.197958    raid
  84    84    1.193760    (__stack_chk_guard address spill)
  89    64    0.480124    alienWave
  90    60    0.309166    pirateFortress
  91    56    0.305567    bigAlienWave
```

`raid` wins by **0.004199** (0.35 %), so it gets `fi#83`/`sp+88`; the guard
address gets `fi#84`/`sp+84`. The original is the mirror image
(`raid` = 17 refs at `[sp,#84]`, guard = 2 refs at `[sp,#88]`), so in the
original **guard weight > raid weight**.

Disassembly confirmation (identical reference addresses, only the offset
changes):

```text
orig:  b0024  str.w fp,[sp,#88]   b02ca  ldr.w fp,[sp,#88]   ; guard, 2 refs
       b04c0..b3756  [sp,#84] x17                            ; raid, 17 refs
ours:  b0024  str.w fp,[sp,#84]   b02ca  ldr.w fp,[sp,#84]   ; guard, 2 refs
       b04c0..b3758  [sp,#88] x17                            ; raid, 17 refs
```

The raid references are at the same 17 instruction addresses in both binaries
(`b04c0 b04d8 b0546 b05da b0680 b0730 b07a6 b0882 b090a b0922 b0964 b098c
b09d8 b0a24 b0a4a b2a0a b3756/8`), so the *set* of raid references is not the
difference. The difference is the block frequencies those blocks receive.

### raid reference decomposition (ours)

```text
bb.24  0.039063   bb.30  0.117190   bb.35  0.156250   bb.44  0.156250
bb.52  0.156250   bb.63  0.061035   bb.68  0.023804   bb.70  0.014877
bb.71  0.024796   bb.89  0.009155   bb.99  0.078125   bb.100 0.058594
bb.106 0.036621   bb.110 0.019531   bb.114 0.078125   bb.118 0.078125
bb.138 0.048828   bb.189 0.031738   bb.258 0.009601
total = 1.197958
```

## The guard weight is the epilogue block frequency

The guard address is stored once in `bb.1` (frequency 0.625) and reloaded once
in `bb.519`, the shared epilogue (`ldr.w fp,[sp,#88]; b.n b0194`). So

```text
guard weight = freq(bb.1) + freq(bb.519) = 0.625000 + 0.568760 = 1.193760
```

`bb.519` has 17 predecessors. Its frequency decomposes exactly as:

```text
  bb.325  freq=0.128350 edge=3.75%  contrib=0.004813
  bb.326  freq=0.018601 edge=50.0%  contrib=0.009300
  bb.517  freq=0.009300 edge=100%   contrib=0.009300
  bb.488  freq=0.011230 edge=100%   contrib=0.011230
  bb.465  freq=0.011230 edge=100%   contrib=0.011230
  bb.454  freq=0.011230 edge=100%   contrib=0.011230
  bb.440  freq=0.011230 edge=100%   contrib=0.011230
  bb.417  freq=0.011230 edge=100%   contrib=0.011230
  bb.402  freq=0.011230 edge=100%   contrib=0.011230
  bb.396  freq=0.011230 edge=100%   contrib=0.011230
  bb.382  freq=0.011230 edge=100%   contrib=0.011230
  bb.368  freq=0.022461 edge=100%   contrib=0.022461
  bb.357  freq=0.009300 edge=100%   contrib=0.009300
  bb.318  freq=0.014850 edge=100%   contrib=0.014850
  bb.307  freq=0.067096 edge=100%   contrib=0.067096
  bb.133  freq=0.029297 edge=100%   contrib=0.029297
  bb.6    freq=10.00000 edge=3.125% contrib=0.312500
  total = 0.568759
```

The largest term is `bb.6`, the alien-orbit loop header: frequency 10.0 and an
exit edge of 3.125 % = 4/128. That is exactly LLVM 7's loop-branch heuristic
(`LBH_TAKEN_WEIGHT = 124`, `LBH_NONTAKEN_WEIGHT = 4` in
`llvm/lib/Analysis/BranchProbabilityInfo.cpp`), so the 0.3125 term is fixed by
the loop shape, not by the source. The remaining 0.256259 is the sum of the
mission-path return blocks.

To flip the slot the epilogue frequency would need to exceed
`1.197958 - 0.625000 = 0.572958`, i.e. rise by **0.0042** (0.74 % of the
epilogue frequency). That is a global CFG/branch-probability effect, not a
local source-order effect. It is consistent with the original having a slightly
different CFG in one of the known residual regions (the `b2e04` merged
condition and the mission-case switch), which changes the mass flowing into the
return blocks `bb.307`/`bb.133`/`bb.368`.

## What was tried against the slot (all measured with the official verifier)

`guard_sp` is the `[sp,#N]` of the `str.w r11` stack-protector address spill in
the freshly compiled `Level.s` (84 = ours, 88 = original). `size_marker` is the
length of the emitted `createMission` assembly segment (pristine 3287984).

| variant | guard_sp | size_marker | fuzzy | size | note |
|---|---:|---:|---:|---:|---|
| **baseline** | **84** | **3287984** | **90.72** | **17172** | |
| `if ((security==3) && raid)` | **88** | 3288075 | 87.09 | 17196 | flips slot, but `&&` short-circuits: +24 B, −3.6 pp |
| `if (raid && campaign>=32 && …)` → `campaign>=32 && raid` | **88** | 3287931 | 88.59 | 17180 | flips slot, −2.1 pp |
| `bool raid = roll<…; if (intro) raid=false;` | **88** | 3287254 | 84.90 | 17228 | flips slot, −5.8 pp |
| `bool raid = false;` / ternary / `int` / `char` | 84 | 3288009/3287821/3288622/3288472 | 90.7 | 17172 | neutral |
| `(security==3) & raid` operand permutations (`raid & …`, `(int)`, `(bool)`, `?:`, `&1`, `!=0`) | 84 | 3287984 | 90.7 | 17172 | IR identical |
| `*bigShipsCount >= 1` | 84 | 3287984 | 90.7 | 17172 | InstCombine canonicalises back to `sgt x,0` |
| `b2e04` `(i==X) & alienWave` / `& (alienWave!=0)` / `swap` / `!(…)` / `(bool)` | 84 | 3287984 (except `&` 3287807) | 90.7 | 17172 | IR identical |
| `case4Tail` `AERandom *rnd` before/inside loop, `Globals *g` removed | 84 | 3288486/3288090/3287836 | 90.6–90.7 | 17168–17172 | neutral |

Key negative result: **every size-preserving variant leaves `guard_sp` at 84**,
and every variant that flips it also changes the instruction stream enough to
lose more fuzzy than the slot swap gains (~1.1 pp). The flip is therefore not
reachable by local source reordering; it is downstream of the CFG.

## Convoy switch (`b1408`): why ours is ascending and the original descending

Source (`Level.cpp:2192`, inside `for (int k = 0; k < 4; k++)`):

```cpp
if (k == 2) { offset = {25000, 0, -36000}; }
else if (k == 1) { offset = {10000, -7000, -30000}; }
else if (k == 0) { offset = {20000, -3000, -50000}; }
else { offset = {12000, 6000, -45000}; }
```

Our IR (optimized, `-emit-llvm`) is already a switch:

```llvm
%2993 = trunc i32 %2934 to i31
switch i31 %2993, label %2998 [
  i31 2, label %2995
  i31 1, label %2996
  i31 0, label %2997
]
```

Machine code:

```text
orig:  b1408  bic.w r1,r5,#0x80000000
       b140c  cmp   r1,#2      b1418  beq.n b1438   ; case 2
       b141a  cmp   r1,#1      b141c  beq.n b142c   ; case 1
       b141e  cbnz  r1,b1444                        ; default
       b1420  ... case 0 falls through
ours:  b1408  bic.w r1,r5,#0x80000000
       b140c  cmp   r1,#0      b1418  beq.n b142e   ; case 0
       b141a  cmp   r1,#1      b141c  beq.n b143a   ; case 1
       b141e  cmp   r1,#2      b1420  bne.n b1448   ; default
       b1422  ... case 2 falls through
```

Mechanism (LLVM 7 `SelectionDAGBuilder::lowerWorkItem`,
`llvm/lib/CodeGen/SelectionDAG/SelectionDAGBuilder.cpp:9830`):

```cpp
llvm::sort(W.FirstCluster, W.LastCluster + 1,
           [](const CaseCluster &a, const CaseCluster &b) {
  return a.Prob != b.Prob ? a.Prob > b.Prob
                          : a.Low->getValue().slt(b.Low->getValue());
});
```

`visitSwitch` seeds each `CaseCluster::Prob` from
`BPI->getEdgeProbability(SI.getParent(), I.getSuccessorIndex())`. LLVM 7's
`BranchProbabilityInfo` has **no `calcSwitchWeights`**: a switch without
`!prof` metadata gets the default `1/N` for every edge. Equal probabilities
therefore sort by `Low` **ascending** → `0,1,2`, which is what we emit. The
original's descending `2,1,0` requires `w2 > w1 > w0`, i.e. unequal branch
weights.

`__builtin_expect` on the three if-conditions does produce weights, but the
merge in `SimplifyCFGOpt::FoldValueComparisonIntoPredecessors`
(`llvm/lib/Transforms/Utils/SimplifyCFG.cpp:1002`, the `Weights[i] *=
ValidTotalSuccWeight` scaling) overflows 32 bits:

```text
expect1  (all likely)  !{!"branch_weights", i32 0, i32 -290966296, i32 2001000, i32 1000}
expect210              !{!"branch_weights", i32 2000000, i32 -290966296, i32 1000, i32 1000}
expect0   (all unlik.) !{!"branch_weights", i32 -294967296, i32 2002000, i32 2001000, i32 2000000}
```

With `expect1` the case-2 probability becomes ~100 %, which trips
`peelDominantCaseCluster` (default `-switch-peel-threshold=66`): case 2 is
peeled and emitted first, the layout becomes `cmp2; bne next; case2; cmp1; …`,
the function shrinks to 17164 B, and the fuzzy collapses to **62.7 %**. So the
garbage weights are not the original's shape.

A nested-range rewrite (`if (k >= 2) { if (k == 2) … else … } else if (k == 1)
… else …`) lowers to `cmp r6,#2; blo …; bne …` — a range check, not the
original's linear chain, and is larger (size_marker 3288139).

## Why this is not landable as a code change

`byte_exact` compares raw function bytes. Changing `createMission`'s size
changes the linked section layout, and the gate's `byte_exact` falls from
**4498 to 2359** for a 24-byte growth (measured with the `&&` variant). Since
`createMission` is one of the 26 non-byte-exact functions (90.7 %), it cannot
raise `byte_exact` until it is fully exact; and every slot-flipping variant
found here also changes the size. The pass therefore lands the note only.

## What remains

1. **raid/guard swap** (~67 instructions, ~1.1 pp): requires the original's
   epilogue frequency to be > 0.572958 instead of 0.568760. The lever is the
   CFG mass in the mission-case return blocks (`bb.307`, `bb.133`, `bb.368`),
   not the raid source order. The most likely upstream cause is the `b2e04`
   materialised `(i != X) | !alienWave` condition (which changes the
   mission-case loop's branch probabilities) — reproducing that IR shape is the
   highest-value next step.
2. **convoy switch** (`b1408`, ~7 instructions): needs unequal `!prof`
   weights that keep every case below the 66 % peel threshold, e.g. roughly
   40/35/20/5. `__builtin_expect` cannot express that (only two levels, and the
   SimplifyCFG merge overflows). A hand-written branch-weight shape or a
   different IR construct is required.
3. **`b2e04`/`b2f2a` merged condition** (12 instructions): `FindMergedConditions`
   needs both operands `InBlock`; our `alienWave != 0` is hoisted to an earlier
   block. Not reproduced by any source spelling tried so far.
4. **alienGuns `Vector` slot merge** (~20 instructions): the original keeps two
   12-byte `Vector` locals whose slots `StackSlotColoring` merges; ours keeps
   them separate.
5. **EH landing pads** (`b41f0`–`b42d0`): block order, not a different
   try/catch structure (`.ARM.extab` is identical, 148 call sites).
6. **`cmp #0; ble` vs `cmp #1; blt`** at `b0eac`: both IRs are
   `icmp sgt i32 %x, 0`; the ARM backend canonicalises ours. Not
   source-controllable.
