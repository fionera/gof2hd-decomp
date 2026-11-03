# `Status::missionCompleted` — the worklist order is the only residual; the fallback needs
# an empty `BlockWorkList` at the high jump table (w4251)

Status: research note, **no source change landed** (gate unchanged). Worktree `wt/w4251`,
branch `wave92-w4251`, base master `b379d00d`. Original binary:
`../_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a, NDK r18b clang 7.0.2
`r328903` / LLVM `1d739ffb`, `-Oz`, image base 0x10000; real VMA = Ghidra − 0x10000).

Target: `_ZN6Status16missionCompletedEbbx` (`Status::missionCompleted(bool, bool,
long long)`), original `0xabf38`, size **1432 (0x598)**, currently **57.7 % / 1432 B**
with the committed `switch ((short) m->getType())`.

This pass adds one decisive experiment and a broad re-sweep:

1. **New: sorting `BlockWorkList` by MBB number reproduces the original's body order
   exactly** (local LLVM 7.0.0, 1412 B).  This proves the *only* residual is the order
   in which `markBlockSuccessors` pushes the high bodies — i.e. the jump table's
   successor order (ascending case value) versus the MBB order (source order).
2. The original layout is still exactly the `getFirstUnplacedBlock` fallback, and the
   fallback is reached only when `BlockWorkList` is empty at the high jump table.
3. Every high body has exactly one predecessor (re-verified from the ELF), so
   `markBlockSuccessors` always pushes all of them; no plain single-switch CFG empties
   the worklist.  A wide source/flag re-sweep (below) still finds no 1432-byte form.

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
`usize 1432`.  (The task brief's “86.7 %” is the pre-`6e3fd629` rank-table anchor;
the committed faithful form is 57.7 %.)

## 1. Original layout = MBB order = source order

Decoding the two TBHs (high table at `abfe6`, 40 halfwords for 150..189; low table at
`ac052`, 7 halfwords for 8..14) gives the physical body order

```
167/174 8 11 13 14 150 151 152 153 154 155 158 162 166 170 184 189
156 157 160 163 164 165 168 169 171 172  default
```

The MBB order of the same bodies in the NDK build (from the MIR, `%ir-block` order /
`bb.N` numbering) is

```
16(167/174) 36(150) 38(151) 39(152) 44(153) 45(154) 46(155) 47(158) 57(162)
62(166) 66(170) 67(184) 71(189) 77(156) 80(157) 85(160) 88(163) 91(164) 92(165)
93(168) 96(169) 97(171) 98(172)
```

— the same order.  The jump-table successor list is ascending value
(`bb.104`: `36 38 39 44 45 46 77 80 47 103 85 57 88 91 92 62 16 93 96 66 97 98 67 71`),
which is what `buildJumpTable` produces (`Table` is built from `Clusters` sorted by
value; the NDK's `SelectionDAGBuilder::buildJumpTable`/`sortAndRangeify` are identical
to 7.0.0 — verified against commit `1d739ffb`).

## 2. The decisive experiment: sort the worklist

`selectBestCandidateBlock` keeps the **first** maximal-frequency entry.  All high bodies
have the same frequency (4.10 %) except 167/174 (8.19 %, two case values), so the
layout is the worklist order.  Adding a debug-only sort of `BlockWorkList` by
`MachineBasicBlock::getNumber()` (i.e. MBB order) to the local LLVM 7.0.0 and compiling
the project's plain-`int` IR with the project flags gives

```
MBP_SORT_SBCB=1 : size 1412, order = 167/174 8 11 13 14 150 151 152 153 154 155
                  158 162 166 170 184 189 156 157 160 163 164 165 168 169 171 172
                  default
```

— **exactly the original order.**  The unforced build on the same IR gives the
ascending order (1404 B); the forced fallback (`selectBestCandidateBlock -> nullptr`)
gives the same exact order at 1416 B.  So the worklist order is the whole residual.

## 3. Why the fallback is still unreachable from a plain single switch

`markBlockSuccessors(highJT)` iterates `highJT->successors()` (ascending value) and
pushes each successor whose chain's `UnscheduledPredecessors` reaches 0.  Every high
body has exactly one predecessor in the original (re-verified: each body address
`ac110 ac116 ac11c ac150 ac156 ac16e ac180 ac1dc ac242 ac2a6 ac2b4 ac2ee ac32a ac354
ac388 ac3b6 ac3d2 ac3d6 ac3de ac3fc ac400 ac41a` occurs exactly once in the whole
disassembly, as the table target).  So all 23 bodies are pushed, in ascending order,
and `selectBestCandidateBlock` cannot return null at the high JT.

`getFirstUnplacedBlock` is reached only when both `selectBestSuccessor` and
`selectBestCandidateBlock` (twice: `BlockWorkList`, `EHPadWorkList`) return null.  On
this CFG that happens only once, at the very end of the loop chain, when every block is
already placed (the only `FALLBACK called` in the missionCompleted trace is at the end
of `buildChain`; `SBCB EMPTY` immediately precedes it).

The only remaining levers are therefore:
* a CFG in which the high bodies' `UnscheduledPredecessors` is 0 at fill time (their
  only in-loop predecessor must be outside the `BlockFilter` or in the same chain), or
* a worklist that is naturally in MBB order (the jump-table successor order would have
  to be MBB order).

## 4. Re-sweep of source shapes (NDK clang, project flags)

All variants are semantically equivalent rewrites of the switch/loop; each was compiled
with the project's exact flags and its body order decoded from the TBH tables.  None
gives the original order at 1432 B.

| source form | size | order |
|---|---:|---|
| committed `switch ((short) m->getType())` | 1432 | ascending, 184/189 after default |
| `switch (m->getType())` (faithful plain int) | 1428 | ascending |
| `for` / `while` / `do{}while` / `for(;;)` / `for(;i<size;)` / `while(i++<size)` | 1400–1468 | ascending |
| `default: break` / `default: continue` / `default: return 0` | 1428–1432 | ascending (184/189 move) |
| all `break` → `goto next` + `next:` | 1428 | ascending |
| all `break` → `continue` | 1468 | ascending-ish (164/165 swap) |
| `if (m != 0) switch` / `if` guard around switch | 1428 | ascending |
| `(short)` with source order ascending / reversed / GB-first | 1376–1432 | ascending |
| `(short)` + `default: return 0` | 1432 | ascending, 184/189 before default |
| two sequential switches, cached `int type` (GC then GB) | 1508 | **exact original** |
| two sequential switches, low group attached to first/second | 1440–1508 | exact / wrong |
| two switches, `const` / block-scoped / `short` / `unsigned` cached | 1508–1512 | exact |
| two switches, re-call `getType()` / `m->id` in 2nd | 1456–1492 | wrong |
| nested outer A / default inner B(+low) | 1212–1420 | wrong |
| if/else chain (cached or reloaded) | 1384–1568 | 0–1 TBH, wrong |

The two-switch form reproduces the order but emits a **third table** (a 17-entry `tbb`
for 156..172) and keeps the cached `type` live across the first switch, which moves
`i` from `fp` to `r8` and `atStation` from `r8` to `r9` (original: `i` in `fp`,
`atStation` in `r8`).  It is 1508 B, +76 B.

## 5. Flag re-sweep (NDK clang, project flags)

| flag | size | order |
|---|---:|---|
| `-mllvm -block-placement-exit-block-bias=0..93` | 1428 | ascending |
| `-mllvm -block-placement-exit-block-bias=94..100` | 1472 | reverse source order, 1 TBH |
| `-mllvm -static-likely-prob=1..99` | 1420–1444 | never exact |
| `-mllvm -tail-dup-placement=false` | 1428 | ascending |
| `-mllvm -tail-dup-placement-{threshold,aggressive-threshold,penalty}=0..100` | 1428 | ascending |
| `-mllvm -triangle-chain-count=0..100` | 1428 | ascending |
| `-mllvm -force-loop-cold-block -loop-to-cold-block-ratio=0..8` | 1432–1452 | 1–2 TBH, ascending |
| `-mllvm -force-loop-cold-block -loop-to-cold-block-ratio=10..20` | 1484–1488 | 0 TBH |
| `-mllvm -force-loop-cold-block -loop-to-cold-block-ratio=22..58` | 1424–1428 | 2 TBH, ascending |
| `-mllvm -force-precise-rotation-cost` | 1448 | ascending with 184/189 late |
| `-mllvm -disable-block-placement` | 1436 | 1 TBH, permutation |
| local `selectBestCandidateBlock -> nullptr` (forced fallback) | 1416 local / **1432 NDK** | **exact original** |
| local `MBP_SORT_SBCB` (sort worklist by MBB number) | 1412 local | **exact original** |

## 6. Conclusion — proven vs open

**Proven (this pass):**

1. The original's body order is the MBB/source order, and the MBP
   `getFirstUnplacedBlock` fallback reproduces it exactly (forced-fallback build:
   1432 B on the NDK, exact body order).
2. Sorting `BlockWorkList` by MBB number also reproduces it exactly (1412 B local),
   proving the **only** residual is the worklist order: ascending case value (jump-table
   successor order) versus MBB/source order.
3. Every high body has exactly one predecessor, so `markBlockSuccessors` always pushes
   all 23 in ascending order; the fallback is unreachable from a plain single-switch CFG.
4. The two-switch form reproduces the order but costs +76 B (extra 17-entry TBB + a
   different register allocation: `i` in `r8`, `atStation` in `r9`).
5. No source shape or flag tried (≈90 variants this pass) gives the original order at
   1432 B.

**Open:** a source CFG that either (a) makes the high bodies' `UnscheduledPredecessors`
0 at fill time (their only in-loop predecessor outside the `BlockFilter` or in the same
chain), or (b) makes the high jump table's successor list be in MBB order, while keeping
the single-switch register allocation and 1432 B.  A documented per-file `-mllvm` flag
would also work but is not currently available (there is no worklist-sort flag in
LLVM 7.0.0).

The committed `(short)` cast remains a size anchor (it inserts `sxth` and moves 184/189
after the default to restore 1432 B); the faithful plain-`int` form is 1428 B / 60.2 %
but shifts `.text` and is not landable alone.

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w4251
cmake --build cmake-build-match --target check -j8    # RATCHET PASS, byte 4499 linked 4499

OD=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OD -d --start-address=0xabf38 --stop-address=0xac4d0 \
    /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so

# worklist-sort experiment (local LLVM 7.0.0, assertions):
#   patch MachineBlockPlacement::selectBestCandidateBlock to sort WorkList by
#   MachineBasicBlock::getNumber() (or use the MBP_SORT_SBCB env hook in
#   /dev/shm/w4251/MBP_orig.cpp's debug copy)
#   llc -mtriple=thumbv7-none-linux-androideabi -mattr=+neon -float-abi=soft \
#       -function-sections -filetype=obj plain7.ll -o sorted.o
```

Scratch (not committed): `/dev/shm/w4251/` — `plain7.ll`, `plain7b.ll`, `trace2.txt`
(NDK-style unforced MBP trace), `trace_mbb.txt` (with MBBORD/PREMERGE dumps),
`trace_force.txt`, `MBP_orig.cpp`, `buildfn.py`, `genvar.py`, `ord2.py`, `decode2.py`,
`fn_plain_full.txt`, `s1.py`..`s10.py`, `flagsweep.py`.
