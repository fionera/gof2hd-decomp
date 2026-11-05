# `Generator::createMission` — wave 113 (w6108): post-placement BranchFolder

Target: `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE`
(original `0x971b8`, 1996 B, `.ARM.extab` LSDA record 172 B).

Status: **research note, no source change landed.**  The wave-110 shape
(`volatile double padA` next to `volatile int amount`, `__builtin_expect(pick == 10, 1)`,
`__builtin_expect(pick == 12, 0)`) remains the unique best variant:
**82.6 % official / 81.5172 % local scorer / 1996 B**.  This pass measured
~90 further source variants and pinned down *where* the shared `type = pick`
store is actually merged (new: it is the **post-placement** `BranchFolder`
inside `MachineBlockPlacement`, not the pre-RA tail merge).

Gate at the start and end of this pass (full `check`):

```
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.92
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

27 sections, file size 2,187,328 B, `.ARM.extab` 0x26298 — all unchanged.
`git status --short` is empty after the note commit.

## 1. New finding: the store merge happens after block placement

Wave 112 attributed the shared-store placement to the pre-MBP tail merge +
`MachineBlockPlacement::hasBetterLayoutPredecessor`.  That is only half the
story.  Dumping the MIR at each pass (`-mllvm -print-machineinstrs=<pass>`)
shows:

* after `tailduplication` (`td_base.txt`) and after `branch-folder`
  (`bf_base.txt`, the post-RA `BranchFolder` from
  `TargetPassConfig::addMachineLateOptimization`) the two stores are still
  **separate**:
  * `bb.32` = `getStation; MOVi 8; STRi12 r0,[sp,#36]; B bb.50`
  * `bb.42` = `MOVi 13; STRi12 r0,[sp,#36]; B bb.50`
  * `bb.50` = `getName`
* `MachineBlockPlacement::runOnMachineFunction` (LLVM 7.0
  `MachineBlockPlacement.cpp:2778`+) then does:
  1. `buildCFGChains()` — first placement,
  2. `BranchFolder BF(/*EnableTailMerge=*/true, /*CommonHoist=*/false, *MBFI,
     *MBPI, TailMergeSize)` with `AfterBlockPlacement=true`, where
     `TailMergeSize = TailDupSize + 1 = 2` (`TailDupSize` is forced to 1 by
     `optForSize()`),
  3. if that returned true, `buildCFGChains()` **again**.

So the merge is the *post-placement* BranchFolder, and the final layout is a
**rebuild** of the chains over the merged CFG.  `CreateCommonTailOnlyBlock`
splits the cheaper block (`EstimateRuntime` returns `~0U` for a block with a
call, so the `MOVi 13` block `bb.42` is chosen over the `getStation` block
`bb.32`), producing the merged block `M` = `STRi12 r0,[sp,#36]` right after
`bb.42`, with `freq(M) = freq(bb.32) + freq(bb.42)` (`setCommonTailEdgeWeights`).

In the rebuild, the entry chain reaches `bb.31 (getOffer)` and picks
`bb.32` (type 8) over `bb.33` (else) on the 50/50 tie (first successor wins),
then `bb.32 -> M` because `hasBetterLayoutPredecessor(bb.32, M)` is false.
That is why the store lands at `0x19c` in our build.

## 2. The exact frequency ratio

Solving the MIR edge probabilities (`freq2.py` over the post-MBP MIR):

```
freq(bb.31 getOffer) = 1.0
freq(bb.32 type8)    = 0.5
freq(bb.42 type13)   = 0.0780986   (== 5/64)
freq(M  store)       = 0.578099
freq(bb.49 null)     = 0.046875
```

`hasBetterLayoutPredecessor` rejects the `bb.32 -> M` edge when

```
freq(bb.42) * HotProb  >=  freq(bb.32) * (1 - HotProb)     (HotProb = 0.8)
freq(bb.42) >= freq(bb.32) / 4 = 0.125
```

Our ratio is **0.15625 = 5/32**, i.e. 1.6x short of the 1/4 threshold.
`freq(bb.42)` is `freq(bb.41 while(!found) header) / 32`; the `/32` is the
`calcLoopBranchHeuristics` back-edge weight (124:4).  The path product to
`bb.42` is `0.5 (offer!=5) * 0.625 * 1/32 (while targetStation) * 0.75
(race<4) * 0.6667 (type==13) * 0.625 (routes!=null) * 1/32 (while !found)`
amplified ~410x by the two loops.

Because the CFG and all branch probabilities are identical in our build and
the rebuild is deterministic, **any source change that keeps the emitted
instructions identical must keep this layout**.  The original's layout can
therefore only come from a *different branch probability* (an invisible
`__builtin_expect`) or a different CFG — not from statement order, `goto`
forms, declaration order, or `if`/`else` inversion (all measured below).

## 3. Variants measured this pass (local scorer; baseline 81.5172 %)

All rows are `score2.py` on the single object; the official scorer reads
~1.1 points higher for the same bytes (baseline 82.6 %).  Rows marked
`IDENTICAL` produced a byte-identical function body (true no-ops).

### `offer == 5` / type-8 arm

| variant | pct | notes |
|---|---:|---|
| baseline | **81.5172** | store @0x19c, null @0x240 |
| `if (agent->getOffer() != 5) {else} else {type8}` | 81.5172 | IDENTICAL (clang canonicalises) |
| `if (5 == agent->getOffer())` | 81.5172 | IDENTICAL |
| `int offer = agent->getOffer(); if (offer == 5)` | 81.5172 | IDENTICAL |
| `const int offer = ...` / `if (5 == offer)` | 81.5172 | IDENTICAL |
| `agent->getOffer() - 5 == 0` / `!(offer - 5)` / `(offer ^ 5) == 0` | 81.5172 | IDENTICAL (InstCombine folds) |
| `goto afterOffer` in the type-8 arm | 81.5172 | IDENTICAL |
| `expect(offer == 5, 0)` | 61.79 | type-8 arm outlined to 0x738, 1972 B |
| `expect(offer == 5, 1)` | 37.24 | 2052 B |

### `pick == 12` / loop

| variant | pct | notes |
|---|---:|---|
| baseline | **81.5172** | |
| `type = pick` instead of `goto assignType` in the 12-else | 50.17 | shape collapse |
| plain `pick == 12` (no hint) | 78.90 | store @0x19e, null @0x240 |
| `expect(pick == 12, 1)` | 50.17 | |
| inverted `if (pick != 12) {...} else {...}` | 50.17 / 78.90 | two spellings |
| `while (found == false)` / `int found` | 81.5172 | IDENTICAL |
| `int i` / nested `if` in the for body | 81.5172 | IDENTICAL |
| `race`-first `&&` order | 67.45 | 1984 B |
| `do{}while` / `for(;;)+break` / `goto retry` | 46.52 | 2012 B |

### `while (!found)` / routes

| variant | pct | notes |
|---|---:|---|
| baseline | **81.5172** | |
| `expect(!found, 0)` | 46.16 | 2040 B |
| `!expect(found, 1)` | 81.5172 | IDENTICAL |
| `expect(!found, 1)` | 55.84 | store moves to 0x23e but loop branch flips |
| `!expect(found, 0)` | 81.5172 | IDENTICAL |
| `expect(routes == nullptr, 0)` | 61.07 | store @0x5ac, 2000 B |
| `expect(routes == nullptr, 1)` | 46.72 | 2040 B |
| ternary / temp for `type = 4 / 1` | 81.5172 | IDENTICAL |

### hints on other branches (all 81.5172 unless noted)

`expect(difficulty >= 10, 0|1)`, `expect(type == 12, 0|1)`,
`expect(race < 4, 0|1) && type == 13`, `expect(type == 13, 1)` — all
IDENTICAL.  `expect(type == 8, 0)` = 45.47, `expect(type == 8, 1)` = 37.74,
`expect(type == 13, 0)` = 40.89.

### frame / declarations

| variant | frame | pct |
|---|---:|---:|
| baseline (`volatile double padA`, `volatile int amount`) | 112 | **81.5172** |
| `volatile float padA` | 104 | 77.38 |
| `volatile short/char padA` | 104 | 77.05 |
| `volatile long long padA` | 112 | 76.94 |
| `volatile int padA` | 104 | 76.91 |
| no `padA` | 104 | 69.94 |
| `int amount` (no volatile) | 112 | 69.29 |
| `int amount` + no padA + `volatile int rem` | **96** | 76.57 |
| `volatile int amount` + no padA + `volatile int rem` | 96 | 69.43 |
| `volatile int count` | 112 | 81.24 |
| `volatile int itemId` / `difficulty` / `targetStation` | 112 | 45.5 / 67.4 / 48.5 |
| 40 random declaration-order permutations | 112 | 81.52 / 81.38 |
| `uint32_t type` / `int type` / `int level = 0` | 112 | 81.5172 (IDENTICAL) |
| `int selected` | 112 | 81.38 |

### type-8 arm `amount` ordering

| variant | pct |
|---|---:|
| baseline (`count; difficulty=...; amount=count+5`) | **81.5172** |
| `amount = count + 5` before `getTecLevel()` | 81.24 |
| `amount = 5 + count` | 81.5172 (IDENTICAL) |
| `int tec = ...; amount = ...; difficulty = tec` | 78.93 / 78.65 |

## 4. Frame map (unchanged from wave 112)

Original frame **96 B**: `[16] bonus/fromSystemPtr [20] chk [24] agent
[28] targetStation/amount [32] status/rem [36] race/type [40] &rnd
[44]/[56]/[68] String temps [80] agentName [92] canary`.

Ours **112 B**: `[24] targetStation [28] agent [92] amount [96] padA
[108] canary` — i.e. `agent`/`targetStation` are swapped and `amount`
gets its own slot.  The 16-byte excess is `amount` (4) + `padA` (8) +
alignment (4).  `volatile` on `amount` is what keeps it out of the shared
`[28]` slot; dropping it restores the frame-96 slot map but loses the four
instructions the original has (null `cmp;beq.w`, in-arm `amount` store,
`rem` spill, `amount` reload), netting 76.6 %.

## 5. Conclusion

The remaining 17.4 % is two coupled `MachineBlockPlacement` decisions:

1. the shared `type = pick` store joins the `type = 8` chain instead of the
   `type = 13` chain (needs `freq(type13) >= freq(type8)/4`; ours is 5/32),
2. the `getRoutes()==nullptr` block is therefore picked by
   `selectBestCandidateBlock` right after `type = 13` (@0x240) instead of
   after the `switch` arms (@0x3ce), which also costs the LSDA `lp=0` entry
   (record 168 B vs 172 B).

Both follow from (1).  No plain C++ spelling tried (this pass or waves
111–112, ~250 variants total) changes the ratio; only `__builtin_expect`
does, and every hint strong enough to cross 1/4 also outlines the type-8
arm or flips a loop branch, costing far more than it gains.  The one
`__builtin_expect` shape that would hit the threshold exactly
(`expect(offer==5, 0)` gives 1/2001, not 1/4) is not expressible in clang
7.0.2 (`__builtin_expect_with_probability` is clang 9+).

## 6. Commands / scratch

```
# single-function MIR at a given pass
/dev/shm/w6108/cc.sh -mllvm -print-machineinstrs=block-placement
/dev/shm/w6108/cc.sh -mllvm -print-machineinstrs=branch-folder
/dev/shm/w6108/cc.sh -mllvm -print-machineinstrs=tailduplication
# local score (object-level, baseline 81.5172 %)
python3 /dev/shm/w6108/getobj.py Generator.o && python3 /dev/shm/w6108/score2.py Generator.o
```

Scratch files: `/dev/shm/w6108/` (`bp_base.txt`, `bf_base.txt`,
`td_base.txt`, `freq2.py`, `score2.py`, `cc.sh`, `run3.py`, `try_*.py`,
`orig_rel.asm`).  Not part of the repo.
