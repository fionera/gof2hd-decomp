# `Status::missionCompleted` — the layout is the MBP worklist order, not the fallback trigger (w4272)

Status: research note, **no source change landed** (gate unchanged). Worktree `wt/w4272`,
branch `wave92-w4272`, base master `b379d00d`. Original binary:
`../_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a, NDK r18b clang 7.0.2
`r328903` / LLVM `1d739ffb`, `-Oz`, image base 0x10000; real VMA = Ghidra − 0x10000).

Target: `_ZN6Status16missionCompletedEbbx` (`Status::missionCompleted(bool, bool,
long long)`), original `0xabf38`, size **1432 (0x598)**, currently **57.7 % / 1432 B**
with the committed `switch ((short) m->getType())`.

This pass **corrects the central claim of the w4222 note** and adds a decisive new
experiment.  w4222 claimed that the NDK jump-table successor list is emitted in
*source* order and that the original's layout therefore needs the
`getFirstUnplacedBlock` fallback.  The NDK MIR actually emits the successor list in
**ascending case-value order**; the original's layout is exactly the *worklist* order
that `selectBestCandidateBlock` would produce if the worklist were sorted by MBB
number.  Sorting the SBCB worklist by MBB number reproduces the original's body order
exactly (1412 B local LLVM 7.0.0; the same lever as the forced fallback).  Every
faithful single-switch source shape tried still produces the ascending order.

## 0. Baseline (measured in this worktree)

```sh
cmake --build cmake-build-match --target check -j8
```

```
verify: byte 4499  linked 4499  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`report.json` row: `_ZN6Status16missionCompletedEbbx` → `pct 57.7`, `osize 1432`,
`usize 1432`.  `.text 0x1337c8`, `.rodata 0x41b67`, `.ARM.exidx` exact,
`.ARM.extab 0x26290 (-8)`, lint `CLEAN ✓`, drift 0.  (The session brief said 4498;
the actual measured gate is 4499, matching master `d1de6672`.)

## 1. Original body order (re-derived from the ELF)

`Status::missionCompleted` at `0xabf38`, size 1432.  One `Mission::getType()` call
(`0xabfd6`), **no `sxth`**, exactly two `tbh`:

```
abfda: sub.w r1, r0, #150
abfde: cmp   r1, #39
abfe0: bhi.n ac046
abfe2: tbh   [pc, r1, lsl #1]      ; 40 halfwords @ abfe6, types 150..189
ac046: sub.w r1, r0, #8
ac04a: cmp   r1, #6
ac04c: bhi.n ac0be
ac04e: tbh   [pc, r1, lsl #1]      ;  7 halfwords @ ac052, types 8..14
ac0be: cmp   r0, #0
ac0c0: bne.w ac45a                 ; latch
```

Decoding the high table (`target = 0xabfe6 + 2*entry`) gives the physical body order:

```
167/174 150 151 152 153 154 155 158 162 166 170 184 189 156 157 160 163 164 165
168 169 171 172   default/latch
```

The low table gives `8 11 13 14` (holes 9/10/12 → latch), physically between
167/174 and 150.  This is exactly the `case` order in `src/game/mission/Status.cpp`.

Body identification spot checks (body bytes identical to ours):

```
orig ac110: f8d6 41c4  ldr.w r4,[r6,#452] ; b.n ac172     == our case 150
orig ac116: f8d6 41c0  ldr.w r4,[r6,#448] ; b.n ac172     == our case 151
```

## 2. The NDK high-jump-table successor list is ASCENDING, not source order

This is the correction to w4222 §3.  The NDK MIR
(`/dev/shm/w4272/mir.txt`, function dump at line 789551) shows:

```
%jump-table.1:  %bb.32 %bb.34 %bb.36 %bb.41 %bb.43 %bb.45 %bb.77 %bb.79 %bb.47
                %bb.99 %bb.84 %bb.99 %bb.55 %bb.10 %bb.88 %bb.89 %bb.60 %bb.11
                %bb.91 %bb.93 %bb.64 %bb.94 %bb.96 %bb.99 %bb.11 %bb.99 ... %bb.66
                ... %bb.71
```

and the MBB successor line:

```
bb.102 (%ir-block.43):
  successors: %bb.32(0x053e08bd), %bb.34(0x053e08bd), %bb.36(0x053e08bd),
              %bb.41(0x053e08bd), %bb.43(0x053e08bd), %bb.45(0x053e08bd),
              %bb.77(0x053e08bd), %bb.79(0x053e08bd), %bb.47(0x053e08bd),
              %bb.99(0x022f2e4e), %bb.84(0x053e08bd), %bb.55(0x053e08bd),
              %bb.10(0x053e08bd), %bb.88(0x053e08bd), %bb.89(0x053e08bd),
              %bb.60(0x053e08bd), %bb.11(0x0a7c1179), %bb.91(0x053e08bd),
              %bb.93(0x053e08bd), %bb.64(0x053e08bd), %bb.94(0x053e08bd),
              %bb.96(0x053e08bd), %bb.66(0x053e08bd), %bb.71(0x053e08bd)
```

Mapping the table entries to case values:

```
150 %bb.32   151 %bb.34   152 %bb.36   153 %bb.41   154 %bb.43   155 %bb.45
156 %bb.77   157 %bb.79   158 %bb.47   159.. %bb.99  160 %bb.84   162 %bb.55
163 %bb.10   164 %bb.88   165 %bb.89   166 %bb.60   167/174 %bb.11
168 %bb.91   169 %bb.93   170 %bb.64   171 %bb.94   172 %bb.96   184 %bb.66
189 %bb.71
```

So the successor list is `150,151,152,153,154,155,156,157,158,latch,160,162,163,
164,165,166,167/174,168,169,170,171,172,184,189` — ascending.  This is exactly what
`SelectionDAGBuilder::buildJumpTable` does (`/dev/shm/w4272/sdb_ndk.cpp:9180`):

```cpp
std::vector<MachineBasicBlock*> Table;
for (unsigned I = First; I <= Last; ++I) {        // Clusters sorted by value
  ...
  for (uint64_t J = 0; J < ClusterSize; ++J)
    Table.push_back(Clusters[I].MBB);
}
SmallPtrSet<MachineBasicBlock *, 8> Done;
for (MachineBasicBlock *Succ : Table) {           // ascending
  if (Done.count(Succ)) continue;
  addSuccessorWithProb(JumpTableMBB, Succ, JTProbs[Succ]);
  Done.insert(Succ);
}
JumpTableMBB->normalizeSuccProbs();               // probabilities only, no reorder
```

`normalizeSuccProbs()` only calls `BranchProbability::normalizeProbabilities`; it does
not reorder.  `MachineBasicBlock::addSuccessor` is a plain `push_back`.  The empirical
MIR is therefore authoritative: the successor list is ascending.

## 3. The worklist at the high JT is non-empty and ascending (measured)

Local LLVM 7.0.0 (`/home/debian/llvm7build`), `-debug-only=block-placement` on the
project IR (`/dev/shm/w4272/cur.ll`).  At `Merging from %bb.104 to %bb.16` (high JT →
167/174) the `selectBestCandidateBlock` worklist is:

```
%bb.11 -> 1.022594752 (freq)      ; low JT
%bb.36 -> 0.1865889213 (freq)     ; 150
%bb.38 -> 0.1865889213 (freq)     ; 151
%bb.39 -> 0.1865889213 (freq)     ; 152
%bb.44 -> ... %bb.45 %bb.46 %bb.77 %bb.80 %bb.47 %bb.85 %bb.57 %bb.88 %bb.91
%bb.92 %bb.62 %bb.93 %bb.96 %bb.66 %bb.97 %bb.98 %bb.67 %bb.71
```

i.e. `[low JT, 150, 151, 152, 153, 154, 155, 156, 157, 158, 160, 162, 163, 164, 165,
166, 168, 169, 170, 171, 172, 184, 189]` — ascending.  `SBCB EMPTY` is **not** printed
at the high JT; it is only printed at the end of the loop chain (`%bb.24`).

The worklist is filled by `markBlockSuccessors(highJT)`, which iterates
`MBB->successors()` (ascending) and pushes each body because its
`UnscheduledPredecessors` is 1 (the high JT is its only in-loop predecessor).  Every
high body has exactly one in-loop predecessor, so all 23 are pushed.

## 4. Decisive experiment: sorting the SBCB worklist by MBB number

The original's body order is *exactly* the MBB order (source order).  If the worklist
is sorted by MBB number before `selectBestCandidateBlock` picks, the layout is the
original's body order:

```cpp
// in selectBestCandidateBlock(), after the empty check
if (getenv("MBP_SORT_SBCB"))
  std::sort(WorkList.begin(), WorkList.end(),
            [](MachineBasicBlock *A, MachineBasicBlock *B) {
              return A->getNumber() < B->getNumber();
            });
```

Local LLVM 7.0.0, project IR (`cur.ll` = committed `(short)` source):

| experiment | size | high-table body order |
|---|---:|---|
| unpatched | 1408 | ascending (`150 151 152 153 154 155 156 157 158 160 162 163 164 165 166 168 169 170 171 172 184 189`) |
| `MBP_FORCE_FALLBACK=1` | 1420 | **exact original** (`150 151 152 153 154 155 158 162 166 170 184 189 156 157 160 163 164 165 168 169 171 172`) |
| `MBP_SORT_SBCB=1` | 1416 | **exact original** |
| `MBP_SKIP_JT=1` | 1424 | reversed (`150 151 152 153 154 155 156 157 158 160 162 163 164 165 166 168 169 170 171 172 184 189` reversed, with 14/13 swapped) |

Both the forced fallback and the sorted worklist produce the original's order.  The
forced fallback lays blocks out in MBB order; the sorted worklist makes
`selectBestCandidateBlock` pick in MBB order.  Since the MBB order is the source order,
both levers coincide.  **The original's layout is the MBB order.**  The only open
question is which mechanism the original compiler used to get there.

## 5. Why no plain single-switch source shape reaches it

`selectBestCandidateBlock` keeps the *first* maximal-frequency entry
(`if (BestBlock && (IsEHPad ^ (BestFreq >= CandidateFreq))) continue;`).  Without
profile data all high bodies have equal frequency (0.1865889213), so it picks the first
entry of the worklist.  The worklist is filled in successor order (ascending), so the
bodies come out ascending.  The fallback is reached only when the worklist is empty
after removing the current chain; on this CFG every body has one predecessor, so
`markBlockSuccessors` always pushes all 23.

The only source-level lever that changes the worklist is to give the bodies extra
in-loop predecessors.  The one shape that does this is **fallthrough between cases**:
each body then has two predecessors (the high JT and the previous body), so
`markBlockSuccessors(highJT)` decrements 2→1 and does *not* push it; the body is placed
by the chain following the fallthrough edge.  Measured:

```
fallthrough (all switch-level `break;` removed):
  size 1444  order [167, 8, 11, 172, 13, 14, 150, 151, 152, 153, 154, 155, 158,
                    162, 166, 170, 184, 189, 156, 157, 160, 163, 164, 165, 168,
                    169, 171]
```

This is *near* the original order but not exact (172 is placed after 11, and the
semantics are wrong).  The original's bodies do not fall through (each ends in
`b ac45a` / `b ac49e`), so this is not the original source.

### Source-variant sweep (NDK clang 7.0.2, project flags, `order2.py`)

All of the following still give `size 1428` (plain `int`) or `1432` (`(short)`) and the
**ascending** order unless noted:

| variant | size | order |
|---|---:|---|
| `switch ((short) m->getType())` (committed) | 1432 | ascending |
| `switch (m->getType())` (faithful, plain int) | 1428 | ascending |
| `switch ((unsigned) m->getType())` | 1428 | ascending |
| `int t = m->getType(); switch (t)` | 1428 | ascending |
| `switch (m->getType() & 0x7fffffff)` | 1432 | ascending |
| `default:` first / last / `default: break` / `default: continue` | 1428–1432 | ascending |
| `default: return 0;` | 1432 | ascending |
| `goto nextMission;` for every case-level `break` (label at loop end) | 1432 | ascending |
| `goto afterSwitch;` for every case-level `break` | 1432 | ascending |
| `do { switch } while (0)` | 1432 | ascending |
| `if (1) { switch }` / `if (m) { switch }` | 1432 | ascending |
| `if (m->getType() >= 0) switch` | 1428 | ascending |
| `while (i < size) { ...; i++; }` | 1400 | ascending |
| `for (i = 0; i < size; ) { ...; i++; }` | 1400 | ascending |
| `for (;;) { if (i >= size) break; ... }` | 1428 | ascending |
| `for (i = 0; i < size; i = i + 1)` | 1428 | ascending |
| `i != size` / `int i` / cached `size` | 1428–1432 | ascending |
| `case 174: case 167:` (label swap) | 1428 | ascending |
| braces around every case body | 1428 | ascending |
| `default:` between cases | 1428 | ascending |
| cases reordered ascending | 1428 | ascending |
| cases reordered reverse | 1340 | ascending |
| `continue` for every switch-level `break` | 1436 | 1 TBH, scrambled |
| all `break;` removed (fallthrough) | 1444 | near-source (see §5) |
| nested switch | 1436 | ascending |
| two sequential switches, cached `int` | 1508 | **exact original**, wrong regalloc |
| `__builtin_expect(m->getType(), N)` flat | 1360–1404 | mostly ascending |

The two-switch form is ruled out by register allocation: it keeps the cached `mtype`
live across the first switch, which pushes `i` to `r8` and `atStation` to `r9`; the
original (and the plain single switch) keeps `i` in `fp` and `atStation` in `r8`, and
the two-switch form costs +76 B (1508 vs 1432).

## 6. Reproduction recipe

The local assertions-enabled LLVM 7.0.0 tree is `/home/debian/llvm7build`.  The
instrumented `MachineBlockPlacement.cpp` is `/dev/shm/w4272/MBP_instr2.cpp` plus the
env-gated hunks below (the snapshot alone only has the `PREMERGE?` print):

```diff
@@ selectBestCandidateBlock
   if (WorkList.empty()) {
     LLVM_DEBUG(dbgs() << "SBCB EMPTY\n");
     return nullptr;
   }
+  if (getenv("MBP_FORCE_FALLBACK")) return nullptr;
+  if (getenv("MBP_SORT_SBCB")) {
+    std::sort(WorkList.begin(), WorkList.end(),
+              [](MachineBasicBlock *A, MachineBasicBlock *B) {
+                return A->getNumber() < B->getNumber();
+              });
+  }
@@ fillWorkLists
       if (BlockToChain[Pred] == &Chain)
         continue;
+      if (getenv("MBP_SKIP_JT") && Pred->succ_size() > 4) continue;
       ++Chain.UnscheduledPredecessors;
@@ fillWorkLists (push)
   MachineBasicBlock *BB = *Chain.begin();
+  LLVM_DEBUG(dbgs() << "FILLPUSH " << getBlockName(BB) << "\n");
@@ buildLoopChains (after fillWorkLists)
+  if (getenv("MBP_SORT_WL")) { /* sort BlockWorkList by MBB number */ }
```

Rebuild and run:

```sh
make -C /home/debian/llvm7build/build llc -j8
llc -mtriple=thumbv7-none-linux-androideabi -mattr=+neon -float-abi=soft \
    -function-sections -filetype=obj cur.ll -o out.o
MBP_FORCE_FALLBACK=1 llc ... -o forced.o
MBP_SORT_SBCB=1      llc ... -o sorted.o
python3 /dev/shm/w4272/order2.py out.o
```

**Caveat for the next worker.**  At the start of this session the local `llc` had an
*unconditional* `return nullptr;` left in `selectBestCandidateBlock` (a stale
`FORCE_SBCB` experiment).  With that binary, "unpatched" and `MBP_FORCE_FALLBACK` both
produced the source order, which is misleading.  Restore from `MBP_instr2.cpp` and add
the env-gated hunks above before trusting any local MBP measurement.

## 7. Conclusion — proved vs open

**Proved (this pass):**

1. The original's body order is exactly the MBB order (source order), including the low
   group between 167/174 and 150 and case 163 in its source position.
2. The NDK high-jump-table successor list is **ascending** (`150 … 189`), correcting
   w4222 §3.  `buildJumpTable` sorts clusters by value and appends in table order.
3. The SBCB worklist at the high JT is non-empty and ascending; the fallback is not
   reached there.
4. Sorting the SBCB worklist by MBB number reproduces the original's body order exactly
   (1416 B local; the same lever as the forced fallback at 1420 B local).
5. Every faithful single-switch source shape tried still gives the ascending order;
   the only shape that changes the worklist (case fallthrough) gives a near-source
   order but wrong semantics.
6. The two-switch form reproduces the order but uses `i`→`r8`, `atStation`→`r9` and
   costs +76 B; the original is a single switch with `i`→`fp`, `atStation`→`r8`.

**Open:** a faithful single-switch source CFG that makes the SBCB worklist come out in
MBB order (or empties it at the high JT) while keeping the single-switch register
allocation and exactly 1432 B.  The worklist order is fixed by
`SelectionDAGBuilder::buildJumpTable`; no source-level lever found changes it.

The committed `(short)` cast remains a size anchor (it inserts `sxth` and moves 184/189
after the default to restore 1432 B); the faithful plain-`int` form is 1428 B / 60.2 %
but shifts `.text` and is not landable alone.

## 8. Scratch (not committed)

`/dev/shm/w4272/` — `orig.asm`, `cur.ll`, `mir.txt` (NDK MIR), `mc_fresh.txt` (local
MBP trace of `missionCompleted`), `forced_trace.txt`, `loopset_trace.txt`,
`sdb_ndk.cpp`, `mbp_ndk.cpp`, `MBP_instr2.cpp`, `order2.py`, `cc.sh`, `emitir.sh`,
`runlocal.py`, `brute.py`, `brute2.py`, `brute3.py`, `reorder.py`, `variants/`.
