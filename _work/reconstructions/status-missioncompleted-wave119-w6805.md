# `Status::missionCompleted` — wave 119 (w6805): the original big-table layout is the
# MachineBlockPlacement *fallback* (MBB/source) order, not a frequency order

Status: **research note (no source change landed)**.  Worktree `wt/w6805`, branch
`wave119-w6805`, base master `a05e21d52`.  Original binary:
`../_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a, NDK r18b clang 7.0.2
r328903, `-Oz`, image base 0x10000).

Target: `_ZN6Status16missionCompletedEbbx` (`Status::missionCompleted(bool, bool,
long long)`), original `0xabf38`, size **1432 B**.  Baseline row: **92.5 % / 1432 B**
(byte False, linked False).  Gate after this pass:

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
# verify: byte 4505  linked 4505  stubs 0  missing 0  extra 0  avg 99.94
# data:   exact 843 of 843 exported objects
# sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
# RATCHET PASS
```

No 1432 B variant beat the committed wave-117 rank table.  This note records one
new structural fact (the original's layout is the MBP **fallback** order), the
branch-weight experiments that follow from it, and the measurements.

## 1. New fact: the original big-body order is exactly the MBB (source) order

The wave-119 w6765 note concluded the original's two ascending runs
`A = {150,151,152,153,154,155,158,162,166,170,184,189}` then
`B = {156,157,160,163,164,165,168,169,171,172}` must be *frequency*-ordered
because `SelectionDAGBuilder::buildJumpTable` emits the jump-table successors in
`Table` (value) order and `selectBestCandidateBlock` breaks equal-frequency ties
in worklist order.

That is not the only possibility.  The MBP trace of the *plain* switch
(`Status_plain.cpp`, no rank table, 1424 B) prints the pre-placement CFG in
`MachineBasicBlock` creation order.  Mapping the big-table successor list
(`%bb.100 -> %bb.34 %bb.36 %bb.37 %bb.42 %bb.43 %bb.44 %bb.73 %bb.76 %bb.45
%bb.81 %bb.53 %bb.84 %bb.87 %bb.88 %bb.58 %bb.31 %bb.89 %bb.91 %bb.62 %bb.92
%bb.93 %bb.63 %bb.67`, which is value order: 150,151,152,153,154,155,156,157,
158,160,162,163,164,165,166,167/174,168,169,170,171,172,184,189) onto the bb
numbers sorted by creation gives

```
bb.31 167/174, bb.34 150, bb.36 151, bb.37 152, bb.42 153, bb.43 154,
bb.44 155, bb.45 158, bb.53 162, bb.58 166, bb.62 170, bb.63 184,
bb.67 189, bb.73 156, bb.76 157, bb.81 160, bb.84 163, bb.87 164,
bb.88 165, bb.89 168, bb.91 169, bb.92 171, bb.93 172
```

i.e. `167/174, 150, 151, 152, 153, 154, 155, 158, 162, 166, 170, 184, 189,
156, 157, 160, 163, 164, 165, 168, 169, 171, 172` — **exactly the original's
big-body order** (original addresses: 0xac036, 0xac110, 0xac116, 0xac11c,
0xac150, 0xac156, 0xac16e, 0xac180, 0xac1dc, 0xac242, 0xac2a6, 0xac2b4,
0xac2ee, 0xac32a, 0xac354, 0xac388, 0xac3b6, 0xac3d2, 0xac3d6, 0xac3de,
0xac3fc, 0xac400, 0xac41a).

The MBB order is the IR block order, which is the *source* order of the cases
(`Status_plain.cpp` writes them A then B).  `MachineBlockPlacement::
getFirstUnplacedBlock` walks exactly that order.  So the original's big bodies
were placed by the **fallback**, not by `selectBestCandidateBlock`.

The low bodies confirm the split: the original places them in *worklist* order
(8, 0-check, 11, 13, 14 — value order of the 8..14 table), not MBB order
(bb.12=0, bb.13=11, bb.16=8, bb.26=13, bb.28=14).  So in the original the low
chain is placed from the worklist (bb.11 is hot, freq 1403 vs 256 for the
bodies), and by the time that chain ends the worklist is **empty**, so the big
bodies fall through to `getFirstUnplacedBlock` = source order.

For the worklist to be empty at that point, `markBlockSuccessors` must never
push the big bodies, i.e. every big body needs `UnscheduledPredecessors >= 2`
when `%bb.100` (the jump-table block) is marked.  In the plain build every big
body has exactly one predecessor, so all 22 are pushed (`WORKLIST size=23:
%bb.11 %bb.34 %bb.36 ... %bb.67`) and the value order wins.

**Open:** a source shape that gives each big-body block a second predecessor
(or otherwise keeps the worklist empty) while leaving the plain `sub #150;
cmp #39; tbh` dispatch and the r6=this / sl=m / fp=i register allocation
intact.  If found, the rank table can be deleted and `.rodata` becomes exact.

## 2. Branch-weight experiments (hand-edited IR, faithful proxy)

`Status_plain.ll` (`clang -S -emit-llvm`, no weights) compiles to the same
1424 B object as `Status_plain.cpp` with the same allocation
(`mov r6,r0` / `mov.w fp,#0` / `mov r8,r1`), so it is a faithful proxy.

Adding `!prof !{!"branch_weights", ...}` to the big switch (index 0 = default,
then cases in the IR's source order) reproduces the original's big-body order
with `default=1, A=2, 167/174=2, B=1` — the wave-119 w6765 result.  Sweeping
the weights further:

| weights (def, A, 167/174, low) | usize | full layout (address order) | prologue |
|---|---:|---|---|
| none (plain) | 1424 | 167/174, 8, 11, 13, 14, 150..172, default, 184, 189 | r6=this, sl=m, fp=i |
| 1, 2, 2, 1 | 1368 | 167/174, 8, A, B, 11, 13, 14, default | fp=this, r9=m, sl=i |
| 2, 2, 3, 2 | 1368 | **167/174, 8, 11, 13, 14, A, B, default** | fp=this, r9=m, sl=i |
| 1, 2, 2, 2 | 1368 | 167/174, 8, 11, A, 13, 14, B, default | fp=this, r9=m, sl=i |
| 1, 2, 2, 3 | 1376 | 167/174, 8, 11, 13, 14, A, B, default | fp=this, r9=m, sl=i |

The `def=2, A=2, 167/174=3, low=2` row is the **full** original order (big and
low), but it is 1368 B and the register allocation changes (`ldrd r3,r6`,
`mov fp,r0`, `mov.w sl,#0` instead of `ldrd r3,r5`, `mov r6,r0`,
`mov.w fp,#0`).  Body 8's `if (atStation && ...)` half is outlined to 0x4a8
instead of staying inline at 0xac080, which is most of the 64 B shortfall.
The TBH entries in that build are the original's shifted by a constant (the
relative structure matches; only the addresses differ).

## 3. `__builtin_expect` from C++: real `!prof`, but degenerate weights

`__builtin_expect(cond, 1)` emits `branch_weights = {2000, 1}` on the branch.
A chain of `if (__builtin_expect(t == N, w)) ... else if (...)` is folded by
`SimplifyCFG::FoldValueComparisonIntoPredecessors` into a real
`switch ... !prof !N`, which is the only C++-expressible way to attach switch
branch weights (a `switch` condition cannot carry them; `__builtin_expect` in
the switch operand is turned into a select chain).

Measured on the real function (chain in source order, `int mtype =
m->getType();` cached):

| weights | usize | TBH |
|---|---:|---|
| plain chain (no expect) | 1396 | value order, tbh at 0xa8 |
| all `expect(_,1)` | 1376 | none (range split picks the low table first) |
| A=1, B=0 | 1376 | none |
| A=1, B=2 | 1376 | none |
| A only, B plain | 1400 | none |
| A=1, B=0 (B plain) | 1392 | none |

The weights degenerate: e.g. `!prof = !{!"branch_weights", i32 0,
i32 -1378701684, i32 -1378701684, i32 -1382305501, i32 3601993, i32 1785,
i32 0, ...}` — the default and most cases end up 0.  `FoldValueComparisonInto
Predecessors` multiplies the predecessor default weight by each successor
weight as it folds, and `FitWeights` rescales after every fold; over 29 folds
the dynamic range collapses the small weights to 0.  The chain also makes
clang emit the low (8..14) table before the big table, unlike the original.

So the C++-chain form is not usable: no variant is 1432 B.

## 4. The rank table is at its ceiling

The TBH is indexed by the *switch value* (the rank); the original's is indexed
by the type.  With a permutation the two can only agree on a slot `k` when
`rank(150+k) == k`.  The layout order fixes the ideal rank sequence to

```
150 151 152 153 154 155 158 162 166 170 184 189 156 157 160 163 164 165
 0   1   2   3   4   5   8  12  16  20  34  39   6   7  10  13  14  15
168 167 169 171 174 172
 18  17  19  21  24  22
```

The longest increasing subsequence of that sequence has length **16**
(`0,1,2,3,4,5,6,7,10,13,14,15,18,19,21,24`), so at most 16 of the 40 body
slots can ever match.  The committed assignment gets those 16 plus 2 default
slots = 18 positional matches (LCS 30/40), which is optimal.

## 5. Conclusion — proven vs open

**Proven (built and measured):**

1. The original's big-body layout is the MBB creation order (= source case
   order = `getFirstUnplacedBlock` fallback order), not a frequency order.
2. The low bodies are in worklist (value) order, so the original's placement is
   "fallback for the big bodies, worklist for the low table".
3. Branch weights on the big switch reproduce the full original order
   (`def=2, A=2, 167/174=3, low=2`), but only at 1368 B and with a different
   register allocation.
4. `__builtin_expect` if/else-if chains do produce real switch `!prof`
   metadata from C++, but the weights degenerate to mostly 0 and the range
   splitting flips (low table first), giving 1376–1400 B.
5. The rank table's TBH is capped at 16 body matches (LIS); the committed
   assignment is optimal at 18/40 positional matches.

**Open:** a source shape that leaves `BlockWorkList` empty when the low chain
ends (each big body needs a second unscheduled predecessor) while keeping the
plain `sub #150; cmp #39; tbh` dispatch and the original's r6=this / sl=m /
fp=i allocation.  That would reproduce the original layout with no rank table,
match the TBH exactly and make `.rodata` exact.

## 6. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6   # RATCHET PASS, byte 4505
cmake --build cmake-build-match --target lint  -j6   # CLEAN
cmake --build cmake-build-match --target drift -j6   # 0 fields
```

Scratch (not committed): `/dev/shm/w6805/` — `Status_plain.cpp` / `.ll`
(plain switch, real types), `mkchain2.py` / `mkchain3.py` (C++
`__builtin_expect` chain generators), `profharness.py` + `ph2.py`
(hand-edited-IR branch-weight sweep, TBH/full-layout decoder), `mkrank2.py`
(rank-scheme generator), `show_now.txt` (official side-by-side),
`mbp_plain.log` (instrumented `llc` MBP trace), and the instrumented LLVM
7.0.2 `llc` (`/tmp/llvm_android_702b/build2/bin/llc`, `MBP_TRACE`).
