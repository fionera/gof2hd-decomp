# `Status::missionCompleted` — the exact form is the MBP fallback, which no single-switch
# source shape reaches (w4222)

Status: research note, **no source change landed** (gate unchanged). Worktree `wt/w4222`,
branch `wave92-w4222`, base master `b379d00d`. Original binary:
`../_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a, NDK r18b clang 7.0.2
`r328903` / LLVM `1d739ffb`, `-Oz`, image base 0x10000; real VMA = Ghidra − 0x10000).

Target: `_ZN6Status16missionCompletedEbbx` (`Status::missionCompleted(bool, bool,
long long)`), original `0xabf38`, size **1432 (0x598)**, currently **57.7 % / 1432 B**
with the committed `switch ((short) m->getType())`.

This pass confirms and sharpens the w4201 conclusion with two new pieces of evidence:

1. **The original's body order is exactly the MBP `getFirstUnplacedBlock` fallback.**
   Re-running the forced-fallback experiment in this worktree reproduces the original's
   high **and** low body order body-for-body (including the low group between 167/174
   and 150 and 163 in its source position), from the project's own plain-`int` IR.
2. **The original is a single switch with the single-switch register allocation**, not
   the two-switch form: the two-switch form's prologue allocates `i` to `r8` and
   `atStation` to `r9`, while the original (and the plain-`int` single switch) allocates
   `i` to `fp` and `atStation` to `r8`. This rules the two-switch source out as the
   original form even though it reproduces the body order.

The remaining open problem is unchanged and is now proven to be a **compiler-decision**
problem, not a source-shape problem: `selectBestCandidateBlock` walks a 23-entry worklist
filled in jump-table successor order (ascending type) and keeps the first maximal-frequency
entry, so the bodies come out ascending.  The fallback is only reached when that worklist
is empty, and on this CFG every body has exactly one in-loop predecessor (the high jump
table), so `markBlockSuccessors` always pushes all 23.  No source shape or MBP flag tried
(≈45 source forms, ≈140 flag values) empties it.

## 0. Baseline (measured in this worktree)

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8
```

```
verify: byte 4498  linked 4498  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`report.json` row: `_ZN6Status16missionCompletedEbbx` → `pct 57.7`, `osize 1432`,
`usize 1432`. `.text 0x1337c8`, `.rodata 0x41b67`, `.ARM.exidx` exact,
`.ARM.extab 0x26290 (-8)`, lint `CLEAN ✓`, drift 0.

## 1. Original layout (re-derived from the ELF)

`Status::missionCompleted` at `0xabf38`, size 1432. One `Mission::getType()` call
(`0xabfd6`), **no `sxth`**, exactly two `tbh` in the whole function:

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

The low table gives `8 11 13 14` (holes 9/10/12 → latch), physically between 167/174
and 150.  This is exactly the `case` order in `src/game/mission/Status.cpp`.

Body identification spot-checks (body bytes identical to ours):

```
orig ac110: f8d6 41c4  ldr.w r4,[r6,#452] ; b.n ac172     == our case 150 at 0x1d6
orig ac116: f8d6 41c0  ldr.w r4,[r6,#448] ; b.n ac172     == our case 151 at 0x1dc
```

## 2. The original order is exactly the `getFirstUnplacedBlock` fallback

Using the local assertions-enabled LLVM 7.0.0 (`/home/debian/llvm7build`), patch
`MachineBlockPlacement::selectBestCandidateBlock` to `return nullptr;`, compile the
project's **plain-`int`** IR with the project flags, and decode both tables:

```
forced2.asm  HIGH: 167/174 150 151 152 153 154 155 158 162 166 170 184 189
                   156 157 160 163 164 165 168 169 171 172
             LOW : 8 11 13 14
```

This is **byte-for-byte the original's body order**, including:

* the low group placed immediately after 167/174 (before 150), and
* case 163 in its source position (between 160 and 164), not early.

The unforced local build on the same IR gives ascending order:

```
base.asm     HIGH: 167/174 150 151 152 153 154 155 156 157 158 160 162 163 164 165
                   166 168 169 170 171 172 184 189
             LOW : 8 11 13 14
```

So the existence proof is solid: the original's layout *is* the fallback.

## 3. Why the unforced build gives ascending order (MBP trace)

`llc -debug-only=block-placement` on the plain-`int` IR (`trace_plain.txt`):

```
Merging from %bb.104 to %bb.16          ; high JT -> 167/174 (8.19%, the only body
                                        ; with two case values, so double edge prob)
Selecting best successor for: %bb.16
    Not a candidate: %bb.17 -> 100.00% (non-cold CFG conflict)   ; latch has other preds
SBCB size=23 chain=11
    %bb.11 -> 1.022594752 (freq)        ; low jump table
    %bb.36 -> 0.1865889213 (freq)       ; 150
    %bb.38 -> 0.1865889213 (freq)       ; 151
    ...                                  ; all remaining bodies equal
Merging from %bb.16 to %bb.11           ; SBCB picks the low JT
```

`selectBestCandidateBlock` keeps the **first** maximal-frequency entry
(`if (BestBlock && (IsEHPad ^ (BestFreq >= CandidateFreq))) continue;`), and the worklist
was filled by `markBlockSuccessors(highJT)` iterating `MBB->successors()` — the jump-table
successor list, which `buildJumpTable` emits in **ascending case-value order**
(`Table` is built from `Clusters` sorted by value; `addSuccessorWithProb` appends in table
order).  The NDK MIR confirms the same order
(`%jump-table.1: %bb.32 %bb.34 %bb.36 %bb.41 ...` ascending).

## 4. Why the fallback is not reachable from source on this CFG

`buildChain` reaches `getFirstUnplacedBlock` only when both `selectBestSuccessor` and
`selectBestCandidateBlock` return null.  `selectBestCandidateBlock` returns null only when
`BlockWorkList` is empty after removing blocks already in the current chain.

`markBlockSuccessors` pushes a successor when

```cpp
if (BlockFilter && !BlockFilter->count(Succ)) continue;   // (a) not in the loop set
if (&Chain == &SuccChain || Succ == LoopHeaderBB) continue;
if (SuccChain.UnscheduledPredecessors == 0 ||
    --SuccChain.UnscheduledPredecessors > 0) continue;
BlockWorkList.push_back(*SuccChain.begin());
```

Every one of the 23 high bodies has **exactly one** predecessor — the high jump table
(verified in the NDK MIR: `bb.10 ['%bb.102']` … `bb.96 ['%bb.102']`).  With no profile
data `collectLoopBlockSet` returns all loop blocks, so (a) does not fire.  Therefore all
23 bodies are pushed, in ascending successor order, and `selectBestCandidateBlock` cannot
return null at the high JT.

The only other lever is `succ->predecessors()` count > 1 (a second in-loop predecessor),
which would leave the body unscheduled until that predecessor is placed.  A plain
`switch` whose cases all `break` to the loop latch has no such edge.

## 5. Sweep of source forms and MBP flags (all compile-only, project flags)

Source forms (NDK clang, `cc.sh`); body order decoded from the TBH tables:

| source form | size | high-table body order |
|---|---:|---|
| `switch ((short) m->getType())` (committed) | 1432 | ascending, 184/189 after default |
| `switch (m->getType())` (faithful) | 1428 | ascending |
| `switch ((unsigned) m->getType())` | 1428 | ascending |
| `int mt = m->getType(); switch (mt)` | 1428 | ascending |
| `switch (m->getType() & 0x7fffffff)` | 1432 | ascending |
| `default:` first / last / `default: break` / `default: continue` | 1428–1432 | ascending |
| `default: return 0;` | 1432 | ascending |
| `if (m->getType() != 0x7fff) switch (...)` | 1428 | ascending |
| `do { switch } while (0)` | 1428 | ascending |
| `goto nextMission;` for every case-level `break` (plain int) | 1428 | ascending |
| `goto nextMission;` for every case-level `break` (`short`) | 1432 | ascending |
| `while (i < size)` / `do{}while` / `for(;;)` | 1400–1432 | ascending |
| `for (i >= 1; i--)` | 1432 | ascending |
| hole cases `case 173:`, `case 159: case 161:`, `case 175..183:` | 1428 | ascending |
| two sequential switches, cached `int mt` | 1508 | **exact original** |
| two sequential switches, re-call `getType()` in 2nd | 1448 | wrong (group A only) |
| nested outer switch + inner `default:` switch | 1396 | 3 tables (25-TBH + 17-TBB + 17-TBH) |
| `if / else if` chain on `int t = m->getType()` | 1436 | 3 TBH (extra group-B table), different order |
| computed return (`result = m; goto done; ... if (result) return result;`) | 1580 | ascending |
| `__builtin_expect(m->getType(), N)` flat, N ∈ {167,174,150,155,156,172,8,0,11,13,14,189,184} | 1360–1404 | mostly ascending |

`-mllvm` flags on the local LLVM 7.0.0 (plain-`int` IR, `-function-sections`):

| flag | size | order |
|---|---:|---|
| `-block-placement-exit-block-bias=0..93` | 1404 | ascending |
| `-block-placement-exit-block-bias=94..100` | 1452 | reverse value order |
| `-block-placement-exit-block-bias=0..100` (NDK clang) | 1428–1452 | never exact |
| `-force-precise-rotation-cost` + `-block-placement-exit-block-bias=80..200` (NDK) | 1448 | never exact |
| `-tail-dup-placement=false` / `-tail-dup-placement-threshold=0` | 1404 | ascending |
| `-force-precise-rotation-cost` | 1424 | ascending |
| `-force-loop-cold-block -loop-to-cold-block-ratio=0..9` | 1408–1432 | ascending |
| `-force-loop-cold-block -loop-to-cold-block-ratio=10..20` | 1460–1468 | no TBH (switch lowered differently) |
| `-force-loop-cold-block -loop-to-cold-block-ratio≥21` | 1404 | ascending |
| **forced `selectBestCandidateBlock -> nullptr`** | 1416 (local) | **exact original** |

No source form and no flag reaches the exact order at 1432 B.

## 6. The two-switch form is ruled out by register allocation

The two-switch form reproduces the body order exactly (high table identical to the
original), but its prologue differs:

```
original        : mov.w fp, #0      ; i = 0
                  mov   r8, r1      ; atStation
plain-int switch: mov.w fp, #0      ; i = 0
                  mov   r8, r1      ; atStation
two-switch      : mov.w r8, #0      ; i = 0
                  mov   r9, r1      ; atStation
```

The original keeps `i` in `fp` and `atStation` in `r8`, exactly the single-switch
allocation.  The two-switch form keeps the cached `mtype` live across the first switch,
which pushes `i` to `r8` and `atStation` to `r9` and costs +76 B (1508 vs 1432).  So the
original cannot be the two-switch source; it is a single switch with the single-switch
allocation and a fallback layout.

## 7. Android LLVM vs LLVM 7.0.0 `MachineBlockPlacement`

Fetched `lib/CodeGen/MachineBlockPlacement.cpp` from the NDK's LLVM commit
`1d739ffb0366421d383e04ff80ec2ee591315116` and normalised (comments stripped,
`DEBUG`/`LLVM_DEBUG` unified).  The only remaining differences are debug-print
formatting and one debug-only `FORCED CONFLICT` block in `hasBetterLayoutPredecessor`
that is guarded by two literal block frequencies and is not taken for this function
(0 occurrences in the trace).  `selectBestCandidateBlock`, `markBlockSuccessors`,
`fillWorkLists`, `getFirstUnplacedBlock` and `buildChain` are semantically identical.
So the NDK's MBP is representative of 7.0.0 for this decision.

## 8. Conclusion — proven vs open

**Proven (this pass):**

1. The original's body layout is exactly the `getFirstUnplacedBlock` fallback: the
   forced-fallback build of the project's plain-`int` IR reproduces the original's high
   and low body order body-for-body, including the low group and case 163's position.
2. The unforced build lays the same 23 bodies out in ascending case-value order because
   `selectBestCandidateBlock` keeps the first maximal-frequency entry of a worklist filled
   in jump-table successor order.
3. Every high body has exactly one in-loop predecessor (the high jump table), so
   `markBlockSuccessors` always pushes all 23 and the fallback is not reachable from any
   plain single-switch CFG.
4. The original is a single switch with the single-switch register allocation
   (`i` in `fp`, `atStation` in `r8`); the two-switch source reproduces the order but
   uses `i` in `r8`, `atStation` in `r9` and costs +76 B.
5. The Android NDK's `MachineBlockPlacement` is semantically identical to LLVM 7.0.0's.

**Open:** a source shape (or a documented per-file `-mllvm` flag) that makes
`BlockWorkList` empty at the high jump table while keeping the single-switch register
allocation and 1432 B.  The only mechanism observed is the fallback itself; the two-switch
form is the only source shape that reproduces the order and it costs 76 B.

The committed `(short)` cast remains a size anchor (it inserts `sxth` and moves 184/189
after the default to restore 1432 B); the faithful plain-`int` form is 1428 B / 60.2 % but
shifts `.text` and is not landable alone.

## 9. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w4222
cmake --build cmake-build-match --target check -j8    # RATCHET PASS, byte 4498 linked 4498

OD=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OD -d --start-address=0xabf38 --stop-address=0xac4d0 \
    /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so

# forced-fallback experiment (local LLVM 7.0.0 with assertions):
#   patch selectBestCandidateBlock -> return nullptr
#   make -C /home/debian/llvm7build/build llc -j8
#   llc -mtriple=thumbv7-none-linux-androideabi -mattr=+neon -float-abi=soft \
#       -function-sections -filetype=obj base_f.ll -o forced2.o
#   (restore with /dev/shm/w4222/MBP_orig.cpp)
```

Scratch (not committed): `/dev/shm/w4222/` — `orig.asm`, `int.asm`, `base.asm`,
`forced2.asm`, `two_switch_cached.asm`, `trace_plain.txt` (unforced MBP trace),
`trace_forced2.txt` (forced trace), `mir.txt` (NDK MIR), `mbp_android.cpp`,
`SDB_orig.cpp`, `MBP_orig.cpp`, `sweep.py`, `sweep2.py`, `sweep3.py`, `flagtest.py`,
`localtest.py`, `layout.py`, `order.py`, `cc.sh`, `emitll.sh`.
