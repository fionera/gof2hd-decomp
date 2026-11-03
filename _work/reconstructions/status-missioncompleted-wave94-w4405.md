# `Status::missionCompleted` — the first 168 instructions are identical; only the
# last-group worklist order is left (w4405)

Status: research note, **no source change landed** (gate unchanged). Worktree
`wt/w4405`, branch `wave94-w4405`, base master `f262bd4b`. Original binary:
`../_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a, NDK r18b clang 7.0.2
`r328903`, `-Oz`, image base 0x10000; real VMA = Ghidra − 0x10000).

Target: `_ZN6Status16missionCompletedEbbx` (`Status::missionCompleted(bool, bool,
long long)`), original `0xabf38`, size **1432 (0x598)**, currently **57.7 % / 1432 B**
with the committed `switch ((short) m->getType())`.

This pass adds one decisive measurement that the earlier notes did not make, and
sharpens the conclusion of `status-missioncompleted-wave92-w4272.md`:

1. **The faithful plain-`int` build (1428 B, no `sxth`) is instruction-for-instruction
   identical to the original for the first 168 instructions / 0x248 bytes (40.8 % of
   the function)** — through `case 155` *and both shared tails*.  The single
   remaining structural difference is the placement of the ten-body group
   `{156,157,160,163,164,165,168,169,171,172}` relative to
   `{158,162,166,170,184,189}`.  That reordering is worth exactly the missing 4 bytes.
2. The original is **not** the `getFirstUnplacedBlock` fallback: the forced fallback
   reproduces the *body* order but misplaces the shared tails (`tail1` after 150
   instead of after 155, `tail2` after the case-0/11 block instead of after 154).
   The original's full layout — bodies *and* tails — is the **worklist order sorted by
   MBB number**, i.e. the ascending worklist with the `156..172` run deferred.
3. The NDK's `MachineBlockPlacement.cpp` at `r328903` is **byte-identical in code** to
   LLVM 7.0.0's for `markBlockSuccessors`, `fillWorkLists`, `buildLoopChains`,
   `selectBestCandidateBlock` and `getFirstUnplacedBlock` (fetched from the LLVM
   monorepo; only `DEBUG`→`LLVM_DEBUG` and the `allowTailDupPlacement()` refactor
   differ).  `SelectionDAGBuilder::buildJumpTable` is identical too.  So the residual
   is a source-CFG problem, not a compiler-version problem.

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

`cmake-build-match/verify/report.json` row:
`{'name': '_ZN6Status16missionCompletedEbbx', 'pct': 57.7, 'byte': False,
'linked': False, 'osize': 1432, 'usize': 1432}`.

## 1. Ground truth recap (re-verified from the ELF this pass)

`Status::missionCompleted` at `0xabf38`, size 1432.  One `Mission::getType()` call
(`0xabfd6`), **no `sxth`**, exactly **two `tbh`** (no `tbb`):

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

`tbh` target = `table_base + 2*entry` (verified: entry 0 of the high table is
`0x0095`, `0xabfe6 + 2*0x95 = 0xac110` = case 150).

Physical body order (by address):

```
ac036 167/174   ac060 8        ac0be 0-check   ac0c4 11      ac0ce 13
ac0dc 14        ac110 150      ac116 151       ac11c 152     ac150 153
ac156 154       ac166 tail2    ac16e 155       ac172 tail1   ac180 158
ac1dc 162       ac242 166      ac2a6 170       ac2b4 184     ac2ee 189
ac32a 156       ac354 157      ac388 160       ac3b6 163     ac3d2 164
ac3d6 165       ac3de 168      ac3fc 169       ac400 171     ac41a 172
ac45a latch/default
```

`tail2` (`ac166`: `cmp r5,r0; blt ac45a; b ac4a8`) is shared by 154 (fallthrough) and
167/174 (`ac044: b.n ac166`).  `tail1` (`ac172`: `mov r0,sl; blx getStatusValue;
cmp r4,r0; blt ac45a; b ac4a8`) is shared by 150/151/153 (forward branches) and 155
(fallthrough from `ac16e`).

## 2. New: the plain-`int` build matches the original for 168 instructions

Compile the faithful `switch (m->getType())` form with the project flags
(`/dev/shm/w4405/v5.cpp`, 1428 B) and diff its disassembly against the original,
normalising branch targets and relocations:

```
DIFF orig ac068 (bytes 48e3): ldr r0, [pc, #908] | ours 130 (bytes 48e0): ldr r0, [pc, #896]
DIFF orig ac180 (bytes f8d6 0190): ldr.w r0,[r6,#400] | ours 248 (bytes f1b8 0f00): cmp.w r8,#0
```

* `0xac068` is the `Globals::status` GOT load in the case-8 body; the literal-pool
  offset differs only because the constant pool is 4 bytes closer in the 1428-byte
  build.  Same instruction, same target.
* `0xac180` is the **first real divergence**: the original has `case 158`, ours has
  `case 156`.

Everything before that — prologue, loop test, mission checks, the two range checks,
both `tbh` dispatches, the low bodies 8/0-11/13/14, cases 150–155 and **both shared
tails** — is instruction-identical (168 instructions / 584 bytes / 40.8 %).

So the residual is *exactly* the ordering of the last group:

| | order |
|---|---|
| original | `158 162 166 170 184 189 156 157 160 163 164 165 168 169 171 172` |
| ours (plain `int`) | `156 157 158 160 162 163 164 165 166 168 169 170 171 172 184 189` |

Both groups are individually **ascending**; the original simply places the
`{156..172}` run *after* `{158,162,166,170,184,189}`.  That is exactly the source
order (two ascending runs), i.e. the MBB order.  The reordering is worth the 4 bytes
(1428 → 1432): the different neighbour relations change one branch from `b.n` to
`b.w`.

## 3. The original is the MBB-sorted worklist, not the fallback

The w4272 note proved that sorting `BlockWorkList` by MBB number (or forcing
`selectBestCandidateBlock` to return `nullptr`) reproduces the original's *body*
order.  This pass distinguishes the two by looking at the **shared tails**:

| build | 150 | 151 | 152 | 153 | 154 | tail2 | 155 | tail1 | 158 |
|---|---|---|---|---|---|---|---|---|---|
| original | ac110 | ac116 | ac11c | ac150 | ac156 | **ac166** | ac16e | **ac172** | ac180 |
| NDK, unpatched (ascending worklist) | 1d8 | 1de | 1e4 | 218 | 21e | **22e** | 236 | **23a** | 248 |
| local, `MBP_FORCE_FALLBACK=1` | 220 | 232 | 23a | 26e | 274 | *1a0* | 286 | *226* | 28c |

(The local build's absolute offsets are not comparable, but the *relative* order is.)

* The **unpatched** build puts `tail2` right after 154 and `tail1` right after 155 —
  exactly the original.  Mechanism: `selectBestSuccessor` from 154/155 succeeds
  (all other predecessors of the tail are already placed), so the tail is laid out by
  the CFG edge, not by the worklist.
* The **forced fallback** places `tail1` immediately after 150 (`150` inlines the
  tail, 151/153/155 branch forward to it) and `tail2` right after the case-0/11
  block.  That is *not* the original.

Therefore the original is **not** the `getFirstUnplacedBlock` fallback.  It is the
ascending worklist **re-ordered by MBB number** before `selectBestCandidateBlock`
picks — i.e. the worklist content is right, only its order is different.  Under that
order the tails still fall out of `selectBestSuccessor` after 154/155, giving the
original layout exactly.

## 4. The NDK `r328903` MBP is the same code as 7.0.0

Fetched from the LLVM monorepo
(`2fa1436206177291edb2d78c84d5822bb6e58cc9`, the last commit touching
`MachineBlockPlacement.cpp` before `llvm-svn: 328903`):

```sh
curl -sL https://raw.githubusercontent.com/llvm/llvm-project/2fa1436.../llvm/lib/CodeGen/MachineBlockPlacement.cpp
```

Diffing against `llvmorg-7.0.0` with comments stripped leaves only `DEBUG`→
`LLVM_DEBUG` churn, the `\brief` doc cleanup and the `allowTailDupPlacement()`
refactor (`TailDupPlacement && !requiresStructuredCFG()`; ARM does not require
structured CFG, so identical behaviour).  The five functions that matter
(`markBlockSuccessors`, `fillWorkLists`, `buildLoopChains`,
`selectBestCandidateBlock`, `getFirstUnplacedBlock`) diff to **zero code lines**.
`SelectionDAGBuilder::buildJumpTable` is likewise unchanged (still
`// Add successors. Note: use table order for determinism.` over the ascending
`Table`).

So the NDK compiler cannot produce the original layout from the current CFG; the
lever has to be the CFG.

## 5. What the worklist order actually is (and why the source cannot move it)

`markBlockSuccessors(highJT)` pushes every high body because each has exactly one
in-loop predecessor (the `tbh` block) and its `UnscheduledPredecessors` goes 1 → 0.
The push order is `MBB->successors()`, which is `buildJumpTable`'s `Table` order =
ascending case value.  `selectBestCandidateBlock` keeps the **first** maximal-frequency
entry; all single-case bodies have the same frequency (`0.1865889213`), so the layout
is the worklist order.

Consequences verified this pass (NDK clang, project flags, `/dev/shm/w4405`):

* Source order is irrelevant: reordering the `case` labels (ascending, reverse,
  `156..172` first) leaves the compiled order **ascending** and the size unchanged
  (1428 plain / 1432 with the `(short)` anchor).  `mkvar.py` + `try.sh` reproduce this
  in ~0.3 s per variant.
* `default: break;` / `default: continue;` / explicit empty hole cases
  (`case 9: case 10: case 12: case 159: case 161: case 173: case 175: ... break;`) /
  splitting `case 0` from `case 11` / `goto`-based breaks / `do{}while(0)` wrapping /
  a second switch nested in `default:` — all still ascending (or worse).
* `break`→`continue` changes the CFG (IR grows by ~1 kB) and scrambles the order
  (`… 163 165 166 168 169 164 170 171 172 184 189`), but never to the original order.

## 6. Landing the faithful plain-`int` form alone is a gate regression

Measured in this worktree:

```sh
sed -i 's/switch ((short) m->getType())/switch (m->getType())/' src/game/mission/Status.cpp
cmake --build cmake-build-match --target check -j8
```

```
verify: byte 3043  linked 4499  stubs 0  missing 0  extra 0  avg 99.9
  FAIL  byte_exact 4499 -> 3043
```

The 4-byte shrink shifts every later `.text` function, so `byte_exact` collapses
4499 → 3043.  It is only landable together with the last-group reorder (which restores
1432 B).  A 1432-byte plain-`int` variant *does* exist (`continue` for the whole
`156..172` run, `/dev/shm/w4405/v23_i.cpp`), but its layout is
`… 163 165 166 168 169 164 170 171 172 184 189` and the verifier scores it **22.3 %**
(vs 57.7 % for the committed `(short)` form), so it is not an improvement.

## 7. Conclusion — proven vs open

**Proven (this pass):**

1. The faithful plain-`int` single switch is instruction-identical to the original for
   the first 168 instructions (0x248 bytes, 40.8 %), including both shared tails.
   The only residual is the placement of the `{156,157,160,163,164,165,168,169,171,172}`
   group relative to `{158,162,166,170,184,189}`; that reorder accounts for the whole
   4-byte size delta.
2. The original is the **MBB-sorted worklist**, not the `getFirstUnplacedBlock`
   fallback: the fallback misplaces both shared tails (`tail1` after 150, `tail2`
   after the case-0/11 block), while the original has them after 155 / after 154 just
   like the unpatched ascending build.
3. The NDK `r328903` `MachineBlockPlacement`/`SelectionDAGBuilder` code is identical to
   LLVM 7.0.0's for every relevant routine, so the residual cannot be a
   compiler-version artefact.
4. Landing the plain-`int` form alone costs `byte_exact` 4499 → 3043 (measured).

**Open:** a source CFG in which `markBlockSuccessors(highJT)` does *not* push the
`156..172` bodies at the high jump table (their `UnscheduledPredecessors` must stay
> 0, i.e. they need a second in-loop predecessor that is placed later), while the
`158..189` bodies still are pushed there and the shared tails still fall out of
`selectBestSuccessor` after 154/155.  Equivalently: a CFG whose high-JT successor
list is in MBB order.  `buildJumpTable` always emits it in ascending `Table` order, so
this has to come from extra CFG edges, not from source label order.  Every shape tried
so far (≈120 variants across w3906/w4003/w4103/w4201/w4222/w4251/w4272 and this pass)
either keeps the ascending order or breaks the tail placement.

## 8. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w4405
cmake --build cmake-build-match --target check -j8      # RATCHET PASS

# per-variant compile + table decode (scratch, /dev/shm/w4405)
./cc.sh <variant>.cpp v.o        # exact project flags, -Oz, NDK r18b
python3 order.py v.o             # prints size + high/low table order + PHYS order

# instruction-level diff against the original
python3 diff_prefix.py           # normalises branch targets / relocs, stops at ac180
```

Scratch (not committed): `/dev/shm/w4405/` — `base.cpp` (committed source),
`v5.cpp`/`v5.o`/`v5.asm` (plain `int`, 1428 B), `plain.ll`/`plain_local.o`/
`plain_forced.o`/`plain_sorted.o` (local LLVM 7.0.0 experiments),
`mbp_r328806.cpp`/`sdb_r328806.cpp` (NDK-revision sources), `mkvar.py`, `cc.sh`,
`order.py`.

The local LLVM 7.0.0 `MachineBlockPlacement.cpp` was left with the env-gated hooks
`MBP_FORCE_FALLBACK` and `MBP_SORT_SBCB`; the pristine copy is
`/dev/shm/w4405/MBP_clean.cpp`.
