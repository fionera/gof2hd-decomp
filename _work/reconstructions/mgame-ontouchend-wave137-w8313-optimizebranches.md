# MGame::OnTouchEnd — wave 137 (w8313): attacking the OptimizeBranches move

Status: analysis-only, 2026-11-07. Worktree `/home/debian/GalaxyOnFire2/wt/w8313`,
base `96b069792`. Owned TU `src/game/menu/MGame.cpp` (`OnTouchEnd` only). Scratch
`/dev/shm/w8313`. **No source change landed** (base restored; full `check` green:
byte 4507, linked 4507, size 4524/4524, data 843/843, imports 207, lint CLEAN).

Target `_ZN5MGame10OnTouchEndEiiPv` (`MGame::OnTouchEnd`), original 8080 B @ `0x17a144`.
Baseline reproduced this wave: 82.1 % (raw 81.9607 %), 8080 B, shared `blx` owned by the
0x200 body (`movs r1,#1` falls through; 0x400 body branches to it).

## 1. Hypothesis

w8215 established that the tail-merge `PredBB` is decided after `OptimizeBranches`, and
w8216 killed the prefix-length lever. This wave attacks the `OptimizeBranches` reorder
itself: find a source shape where the join block (`if (hudTouchFlags == 0)`) is **not**
moved next to the 0x200 body.

## 2. Mechanism confirmed from the MIR

Pre-CFO (`-mllvm -print-after-all`, Prologue/Epilogue Insertion dump) block order:

```
bb.425  0x200 test
bb.426  0x200 body  (t2B bb.463)
bb.427  0x800 test
bb.428  0x800 body
bb.429  0x400 test
bb.430  0x400 body
...
bb.462  wingman tail
bb.463  join test  (predecessors: bb.426, bb.428, bb.430, bb.447, bb.448)
```

`OptimizeBranches` processes `bb.463`: its layout predecessor `bb.462` (wingman tail)
ends in `t2B %bb.471` (epilogue), so `!PrevBB.canFallThrough()` is true. The join test
itself cannot fall through (`t2Bcc epilogue; t2B flags-body`). `OptimizeBlock` then walks
`MBB->predecessors()` in vector order and moves the block after the first predecessor
that cannot fall through (`BranchFolding.cpp` ~1785):

```
for (MachineBasicBlock *PredBB : MBB->predecessors())
  if (PredBB != MBB && !PredBB->canFallThrough() && ... ) {
     MBB->moveAfter(PredBB);
     ...
  }
```

The vector is `[body1(0x200), body2(0x800), body3(0x400), 0x1000 test, 0x1000 body]`.
`body1` ends in an unconditional branch to the join test and its layout successor is the
0x800 test, so it cannot fall through; it is chosen. Post-CFO the join test (and its
split tail) sits immediately after `bb.404` (the 0x200 body):

```
bb.404  ir 2426  0x200 body
bb.405  ir 2567  split common tail (tBL initHudMenu)
bb.406  ir 2567  join test
```

The next `TailMergeBlocks` iteration therefore sees `PredBB = body1`, and
`CreateCommonTailOnlyBlock` early-breaks on it, making the 0x200 body the common tail.

## 3. The predecessor vector order is the blocker

The vector order is **not** the MIR block order. It follows the source/test order of the
bodies. Reordering the IR blocks does not change it:

* nested inverted chain
  (`if (!(f&0x200)) { if (!(f&0x800)) { if (f&0x400) body3; else rest; } else body2; } else body1;`)
  produces IR block order `body3, rest, body2, body1`, but the MIR vector is still
  `[body1, body2, body3, 0x1000 test, 0x1000 body]`;
* `goto body1/body2/body3` label shapes, with the labels in `body3, rest, body2, body1`
  order, produce the same vector;
* moving the `joinTest:` label up (directly after the 0x400 body, with `goto restChain`
  for the rest chain and a backward `goto weaponCheck`) still produces the same vector;
* swapping the `weaponSlot`/`wingmanCommand` checks is byte-identical (82.1 %, 8080 B).

In every case the compiler canonicalises the CFG back to the base block order, and the
join test is moved after `body1`.

## 4. The one shape that flips the owner also flips the test order

Reordering the three `else if` clauses to **0x400, 0x800, 0x200** (permutation CBA) makes
the MIR vector `[body3, body2, body1, ...]`, so `OptimizeBranches` moves the join test
after `body3` and the 0x400 body owns the shared tail. But the final test order becomes
`0x400/0x800/0x200` (the original is `0x200/0x800/0x400`), and the score is 82.0 %
(8080 B) — slightly worse than the 82.1 % base. The other five permutations:

| clause order | size | pct | owner |
|---|---:|---:|---|
| ABC (base) | 8080 | 82.1 | 0x200 |
| ACB | 8076 | 79.2 | 0x400 |
| BAC | 8064 | 78.1 | 0x400 |
| BCA | 8064 | 78.1 | 0x200 |
| CAB | 8076 | 79.1 | 0x200 |
| CBA | 8080 | 82.0 | **0x400** |

## 5. Estimate tie-break is still unreachable

`CreateCommonTailOnlyBlock` keeps the last `SameTails` element on an estimate tie.
`SameTails` is `[body3, body1]` (higher block number first), so the last is `body1`.
The nested shape gives `body3` the lower block number, which would make `body3` the last
and win the tie — but `PredBB == body1` early-breaks before the estimate. If `PredBB` is
not a body (join test not moved), `ProfitableToMerge` is false: `CommonTailLen == 1`
(the `tBL`), `EffectiveTailLen == 2 < MinCommonTailLength (3)`, and neither tail starts
at the block beginning. So the merge only happens when `PredBB` is one of the two bodies.

## 6. Conclusion / next lead

The owner cannot be flipped from 0x200 to 0x400 while keeping the 0x200/0x800/0x400
dispatch order by source-level block reordering: the compiler canonicalises the
predecessor vector to the source clause order. The remaining unexplored lever is to make
the join-test block's **IR block-list predecessor** the 0x400 body *before* the first
`TailMergeBlocks` iteration, i.e. get `CodeGenPrepare` to lay the join test immediately
after the 0x400 body. Every `goto`/nesting shape tried here is folded back; a shape that
survives `SimplifyCFG`/`CodeGenPrepare` block placement is still needed.

## 7. Reproduction

```
cp /dev/shm/w8313/MGame_base.cpp src/game/menu/MGame.cpp
cmake --build cmake-build-match --target check -j6     # green, byte 4507
/dev/shm/w8313/try.sh /dev/shm/w8313/MGame_perm_CBA.cpp  # 82.0 %, owner 0x400
/dev/shm/w8313/pa.sh /dev/shm/w8313/pa_base.txt          # MIR dumps
```

Candidate sources: `/dev/shm/w8313/MGame_nest.cpp`, `MGame_goto.cpp`, `MGame_goto2.cpp`,
`MGame_jointest.cpp`, `MGame_wc.cpp`, `MGame_swapws.cpp`, `MGame_perm_*.cpp`.
MIR extracts: `pre_cfo_base.txt`, `pre_cfo_nest.txt`, `pre_cfo_jt.txt`, `pre_cfo_wc.txt`.
