# `Status::missionCompleted` — the worklist really is MBB-ordered, but nothing in
# the CFG makes it so (wave 95, w4505)

Status: research note, **no source change landed** (gate unchanged).
Worktree `wt/w4505`, branch `wave95-w4505`, base master `4a88bd6e`.
Original binary: `../_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a,
NDK r18b clang 7.0.2 `r328903`, Android LLVM `1d739ffb…`, `-Oz`, image base
0x10000; real VMA = Ghidra − 0x10000).

Target: `_ZN6Status16missionCompletedEbbx`, original `0xabf38`, size
**1432 (0x598)**, currently **57.7 % / 1432 B** with the committed
`switch ((short) m->getType())` (the `(short)` is a 4-byte size anchor; the
original dispatch has no `sxth`).

This pass closes the "which MBP mechanism" question experimentally and shows
that **both** candidate mechanisms reproduce the original's *body* order, but
only one reproduces the *shared tails* — and that one is not reachable from
the CFG.  It also rules out the last compiler-side variable (the Android LLVM
fork at the NDK's exact revision is code-identical to LLVM 7.0.0 here).

## 0. Baseline (measured in this worktree)

```sh
cmake --build cmake-build-match --target check -j8
```

```
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`cmake-build-match/verify/report.json` row:
`{'name': '_ZN6Status16missionCompletedEbbx', 'pct': 57.7, 'byte': False,
'linked': False, 'osize': 1432, 'usize': 1432}`.

## 1. Ground truth (re-decoded this pass)

```
abfd4: mov   r0, sl
abfd6: blx   Mission::getType()          ; ONE call, i32 result, no sxth
abfda: sub.w r1, r0, #150
abfde: cmp   r1, #39
abfe0: bhi.n ac046
abfe2: tbh   [pc, r1, lsl #1]            ; 40 halfwords @ abfe6, types 150..189
ac046: sub.w r1, r0, #8
ac04a: cmp   r1, #6
ac04c: bhi.n ac0be
ac04e: tbh   [pc, r1, lsl #1]            ;  7 halfwords @ ac052, types 8..14
ac0be: cmp   r0, #0
ac0c0: bne.w ac45a                       ; latch
```

Decoding `target = table_base + 2*entry` (base `0xabfe6`, entries
`0x0095 0x0098 0x009b 0x00b5 0x00b8 0x00c4 0x01a2 0x01b7 0x00cd 0x023a
0x01d1 0x023a 0x00fb 0x01e8 0x01f6 0x01f8 0x012e 0x0028 0x01fc 0x020b
0x0160 0x020d 0x021a 0x023a 0x0028 0x023a … 0x0167 … 0x0184`) gives the
physical body order:

```
ac110 150  ac116 151  ac11c 152  ac150 153  ac156 154  ac16e 155
ac180 158  ac1dc 162  ac242 166  ac2a6 170  ac2b4 184  ac2ee 189
ac32a 156  ac354 157  ac388 160  ac3b6 163  ac3d2 164  ac3d6 165
ac3de 168  ac3fc 169  ac400 171  ac41a 172  ac45a latch
```

i.e. `150 151 152 153 154 155 158 162 166 170 184 189 156 157 160 163 164
165 168 169 171 172` — the committed `case` order, and (in our build) the MBB
order of the bodies (`150→bb.36 151→bb.38 152→bb.39 153→bb.44 154→bb.45
155→bb.46 158→bb.47 162→bb.57 166→bb.62 170→bb.66 184→bb.67 189→bb.71
156→bb.77 157→bb.80 160→bb.85 163→bb.88 164→bb.91 165→bb.92 168→bb.93
169→bb.96 171→bb.97 172→bb.98`).

The shared tails sit **after 154 / 155**, not after the first body that
reaches them:

```
ac156 154 … ac166 tail2 (cmp r5,r0; blt ac45a; b ac4a8)
ac16e 155   ac172 tail1 (mov r0,sl; blx getStatusValue; cmp r4,r0; blt ac45a; b ac4a8)
ac180 158
```

`tail2` is reached by fallthrough from 154 and by `b.n ac166` from the
167/174 body (`ac044`); `tail1` by fallthrough from 155 and by `b.n ac172`
from 150/151/153 (`ac114 ac11a ac154`).

## 2. What the current build does (MBP trace, LLVM 7.0.0 assertions build)

Local `llc` (assertions, `/home/debian/llvm7build/build/bin/llc`) on the
project IR (`int.ll`, emitted from `Status_int.cpp` = the plain-`int` form of
the committed source with `ccir.sh`):

```
Selecting best successor for: %bb.10
    Candidate: %bb.11  ... 18.33%          ; low range check
    Candidate: %bb.104 ... 81.67%          ; high jump table
    Selected: %bb.104
Merging from %bb.10 to %bb.104
Selecting best successor for: %bb.104
HBLP: BB=%bb.104 f=6250 Succ=%bb.36 f=256 ... 4.10%   ; 150
HBLP: BB=%bb.104 f=6250 Succ=%bb.38 f=256 ... 4.10%   ; 151
... every single-value body 4.10% ...
HBLP: BB=%bb.104 f=6250 Succ=%bb.16 f=512 ... 8.19%   ; 167/174 (two values)
    Selected: %bb.16
Merging from %bb.104 to %bb.16
Selecting best successor for: %bb.16
    ...
    %bb.11 ('') -> 1.022594752 (freq)
    %bb.36 ('') -> 0.1865889213 (freq)
    %bb.38 ('') -> 0.1865889213 (freq)
    ... all remaining bodies exactly 0.1865889213 ...
Merging from %bb.16 to %bb.11
```

and later, when the low group is done:

```
Selecting best successor for: %bb.35
    %bb.36 ('') -> 0.1865889213 (freq)
    %bb.38 ('') -> 0.1865889213 (freq)
    %bb.39 ('') -> 0.1865889213 (freq)
    %bb.44 ('') -> 0.1865889213 (freq)
    %bb.45 ('') -> 0.1865889213 (freq)
    %bb.46 ('') -> 0.1865889213 (freq)
    %bb.77 ('') -> 0.1865889213 (freq)
    %bb.80 ('') -> 0.1865889213 (freq)
    %bb.47 ('') -> 0.1865889213 (freq)
    %bb.85 ('') -> 0.1865889213 (freq)
    %bb.57 ('') -> 0.1865889213 (freq)
    %bb.88 ('') -> 0.1865889213 (freq)
    %bb.91 ('') -> 0.1865889213 (freq)
    %bb.92 ('') -> 0.1865889213 (freq)
    %bb.62 ('') -> 0.1865889213 (freq)
    %bb.93 ('') -> 0.1865889213 (freq)
    %bb.96 ('') -> 0.1865889213 (freq)
    %bb.66 ('') -> 0.1865889213 (freq)
    %bb.97 ('') -> 0.1865889213 (freq)
    %bb.98 ('') -> 0.1865889213 (freq)
    %bb.67 ('') -> 0.1865889213 (freq)
    %bb.71 ('') -> 0.1865889213 (freq)
Merging from %bb.35 to %bb.36
```

`BlockWorkList` at that point is exactly the high table's successor order
(ascending type), because `SelectionDAGBuilder::buildJumpTable` adds the
jump-table MBB's successors in `Table` order
(`// Add successors. Note: use table order for determinism.`), and
`selectBestCandidateBlock` keeps the **first** maximal-frequency entry
(`if (BestBlock && (IsEHPad ^ (BestFreq >= CandidateFreq))) continue;`).

`184` (bb.67) and `189` (bb.71) are the last two entries of that worklist and
therefore land **after the latch** in our build (`… 172, latch, 184, 189`),
which is the second visible difference from the original.

## 3. New experiment A — sorting the worklist by MBB number reproduces everything

Patch (env-gated, local LLVM only):

```cpp
  if (getenv("MBP_SORT_SBCB")) {
    std::sort(WorkList.begin(), WorkList.end(),
              [](MachineBasicBlock *A, MachineBasicBlock *B) {
                return A->getNumber() < B->getNumber();
              });
  }
```

Result (`llc -O2`, ARM, project IR):

```
MBP_SORT_SBCB=1 llc … int.ll -> sorted.o
size 0x584
order: [[167,174],[8],[11],[13],[14],[150],[151],[152],[153],[154],[155],
        [158],[162],[166],[170],[184],[189],[156],[157],[160],[163],[164],
        [165],[168],[169],[171],[172], default]
```

— the original's body order exactly, **and** the tails fall out of
`selectBestSuccessor` after 154/155 (the handoff's w4405 measurement).

## 4. New experiment B — the `getFirstUnplacedBlock` fallback reproduces the
## bodies but **not** the tails

Patch (env-gated): skip the worklist push in `markBlockSuccessors` for
successors of a block with more than 20 successors (i.e. never push the
high-table bodies; they then get placed by `getFirstUnplacedBlock`, which
walks the MachineFunction in MBB order):

```cpp
    auto *NewBB = *SuccChain.begin();
    if (getenv("MBP_SKIP_JT_PUSH") && MBB->succ_size() > 20) continue;
```

Result:

```
MBP_SKIP_JT_PUSH=1 llc … int.ll -> skip.o
size 0x588
order: [[167,174],[8],[11],[13],[14],[150],[151],[152],[153],[154],[155],
        [158],[162],[166],[170],[184],[189],[156],[157],[160],[163],[164],
        [165],[168],[169],[171],[172], default]
```

Same body order.  But the emitted assembly shows

```
.LBB70_36:   cmp r5, r0 ; blt ; b        <- tail2
.LBB70_37:   ldr.w r4, [r6, #452]        <- 150, falls through
.LBB70_38:   mov r0, r10 ; bl getStatusValue ; cmp r4, r0 ; blt ; b   <- tail1
.LBB70_39:   ldr.w r4, [r6, #448]        <- 151
```

`tail1` is laid out **immediately after 150** (150 falls through into it),
whereas the original has `tail1` after 155.  This is exactly the w4272/w4405
observation that the plain `MBP_FORCE_FALLBACK` misplaces the tails.

**Conclusion:** the original is *not* the fallback.  It is the worklist in
MBB order (experiment A), because only that keeps `tail1` after 155 and
`tail2` after 154.

## 5. New experiment C — each high body has exactly ONE predecessor

Instrumented `fillWorkLists` (env `MBP_DUMP_COUNT`) prints
`Chain.UnscheduledPredecessors` for every chain of the mission loop:

```
COUNT %bb.104 = 1     ; high jump table (pred: switch block bb.10)
COUNT %bb.71  = 1     ; 189
COUNT %bb.67  = 1     ; 184
COUNT %bb.98  = 1     ; 172
COUNT %bb.97  = 1     ; 171
COUNT %bb.66  = 1     ; 170
COUNT %bb.96  = 1     ; 169
COUNT %bb.93  = 1     ; 168
COUNT %bb.16  = 1     ; 167/174
COUNT %bb.62  = 1     ; 166
COUNT %bb.92  = 1     ; 165
COUNT %bb.91  = 1     ; 164
COUNT %bb.88  = 1     ; 163
COUNT %bb.57  = 1     ; 162
COUNT %bb.85  = 1     ; 160
COUNT %bb.47  = 1     ; 158
COUNT %bb.80  = 1     ; 157
COUNT %bb.77  = 1     ; 156
COUNT %bb.46  = 1     ; 155
COUNT %bb.45  = 1     ; 154
COUNT %bb.44  = 1     ; 153
COUNT %bb.39  = 1     ; 152
COUNT %bb.38  = 1     ; 151
COUNT %bb.36  = 1     ; 150
COUNT %bb.37  = 4     ; tail1 (150,151,153,155)
COUNT %bb.17  = 2     ; tail2 (154,167/174)
COUNT %bb.103 = 38    ; latch / default
```

This is confirmed independently from the original's own disassembly: every
high-body start address appears **exactly once** in the whole function
(`grep -c` over the disassembly: `ac110 1, ac116 1, ac11c 1, ac150 1,
ac156 1, ac16e 1, ac180 1, ac1dc 1, ac242 1, ac2a6 1, ac2b4 1, ac2ee 1,
ac32a 1, ac354 1, ac388 1, ac3b6 1, ac3d6 1, ac3de 1, ac3fc 1, ac400 1,
ac41a 1`), and every body is preceded by a branch (no fallthrough into a
body).  So the original's bodies also have exactly one predecessor.

**Consequence (the open contradiction):** with one predecessor the count is 1,
`markBlockSuccessors(highJT)` decrements it to 0 and pushes the bodies in
table order.  So the fallback can never be entered for these bodies — yet the
fallback's MBB order is the only order that matches, and the sorted-worklist
patch is the only mechanism that also fixes the tails.

## 6. The compiler is not the variable (re-checked at the NDK's exact revision)

The NDK r18b clang reports

```
Android (4751641 based on r328903) clang version 7.0.2
(https://android.googlesource.com/toolchain/llvm 1d739ffb0366421d383e04ff80ec2ee591315116)
```

Fetched from android.googlesource.com at that exact revision:

```sh
curl -s "https://android.googlesource.com/toolchain/llvm/+/1d739ffb0366421d383e04ff80ec2ee591315116/lib/CodeGen/MachineBlockPlacement.cpp?format=TEXT" | base64 -d > mbp_android.cpp
curl -s "https://android.googlesource.com/toolchain/llvm/+/1d739ffb0366421d383e04ff80ec2ee591315116/lib/CodeGen/SelectionDAG/SelectionDAGBuilder.cpp?format=TEXT" | base64 -d > sdb_android.cpp
```

* `MachineBlockPlacement.cpp`: the only differences vs llvmorg-7.0.0 are
  `\brief` doc-comment churn and whitespace.  `selectBestCandidateBlock`,
  `markBlockSuccessors`, `fillWorkLists`, `buildLoopChains`,
  `getFirstUnplacedBlock` and `collectLoopBlockSet` are code-identical.
* `SelectionDAGBuilder::{buildJumpTable,findJumpTables,visitSwitch}` diff to
  **zero** code lines (only `DEBUG`→`LLVM_DEBUG` formatting).

So `buildJumpTable` really does add the high table's successors in ascending
`Table` order in the NDK compiler too.

## 7. New: the local `llc` is only valid for *order*, not for sizes

```
NDK clang 7.0.2,  -Oz   int.ll            -> 0x594 (1428 B)  ascending order
local llc 7.0.0,  -O2   int.ll            -> 0x57c (1404 B)  ascending order
local llc + MBP_SORT_SBCB                 -> 0x584 (1412 B)  original order
local llc + MBP_SKIP_JT_PUSH              -> 0x588 (1416 B)  original bodies
```

The 24-byte gap between the NDK clang and the local `llc` (same IR, same
`-O2`/`-Oz` codegen level) means the local `llc` must not be used to predict
the final size: the reorder costs **+8…+12 B** locally but only **+4 B**
(1428 → 1432) with the NDK compiler.

## 8. New negative sweep (NDK clang, project flags, `try.sh`)

All of these compile to **0x594 (1428 B)** and the identical ascending body
order `167/174 8 11 13 14 150 151 152 153 154 155 156 157 158 160 162 163
164 165 166 168 169 170 171 172 default 184 189`:

| variant | size | order |
|---|---:|---|
| plain `switch (m->getType())` | 0x594 | ascending |
| `switch ((unsigned) m->getType())` | 0x594 | ascending |
| switch wrapped in `do { … } while (0);` | 0x594 | ascending |
| switch wrapped in `if (1) { … }` | 0x594 | ascending |
| explicit `default:` + trailing `;` after the switch | 0x594 | ascending |
| `i += 0;` after the switch | 0x594 | ascending |
| cases in ascending order | 0x594 | ascending |
| cases in reversed order | 0x594 | ascending |
| if/else-if chain (SimplifyCFG turns it back into a switch) | 0x580 (1408 B) | ascending |

Plus, with the NDK clang:

```
-mllvm -force-loop-cold-block                     -> 0x5b0 (1456 B)
   order: … 172 184 189 default        (184/189 still last)
-mllvm -force-loop-cold-block -loop-to-cold-block-ratio=1
                                                  -> 0x5a4 (1444 B)
   order: default 167/174 150 … 172 184 189
```

Neither reproduces the original order.

## 9. Conclusion — proven vs open

**Proven this pass:**

1. The original's high-body layout is the **MBB order**, i.e. the committed
   source order; the shared tails are placed after 154/155.
2. Sorting `BlockWorkList` by MBB number before `selectBestCandidateBlock`
   reproduces the original's layout exactly (bodies *and* tails).
3. The `getFirstUnplacedBlock` fallback reproduces the bodies but places
   `tail1` right after 150 instead of after 155, so the original is **not**
   the fallback.
4. Every high body has exactly **one** predecessor in our build
   (`UnscheduledPredecessors == 1`) and in the original (each body address
   occurs once in the disassembly, always preceded by a branch).
5. The Android LLVM fork at the NDK's exact revision is code-identical to
   LLVM 7.0.0 for `MachineBlockPlacement` and for
   `SelectionDAGBuilder::{buildJumpTable,findJumpTables,visitSwitch}`.
6. The local `llc` (`-O2`) is 24 B smaller than the NDK clang (`-Oz`) on the
   same IR, so only *orders* from it are trustworthy.

**Open:** with one predecessor per body, `markBlockSuccessors(highJT)` must
push them in table (ascending) order, so the worklist is ascending and the
fallback is unreachable.  Yet the original's layout is the MBB-ordered
worklist.  Either the original's high bodies had `UnscheduledPredecessors > 1`
through some CFG shape not yet found (a second in-loop predecessor placed
after the jump table), or its worklist was filled in MBB order by a mechanism
outside the source (a different `MachineBasicBlock::successors()` order).
No source shape tried in w3906/w4003/w4103/w4201/w4222/w4251/w4272/w4405 or
this pass produces either.

## 9b. The one known 1432-byte shape with the right layout is an anchor

For completeness: the `.missioncase` rank table (w3906 §3.2, commit
`8b634df3`, removed again by `6e3fd629`) makes the switch operand a *rank*
whose numeric order equals the source order, so clang's ascending `Table`
order coincides with the MBB order and the bodies land in the original
layout.  It measures **86.7 % / 1432 B** and passes `check`, but it adds a
28th ELF section (`.missioncase`, 0xc0 bytes), which breaks the "27 section
headers byte-identical / `.shstrtab 0x117`" property that `6e3fd629` restored.
It is an invented data anchor, not binary-proven source, so it is not
re-landed here.

## 10. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w4505
cmake --build cmake-build-match --target check -j8      # RATCHET PASS

# scratch (/dev/shm/w4505):
./ccir.sh Status_int.cpp int.ll        # project IR of the plain-int switch
./try.sh  Status_int.cpp               # NDK clang: 0x594, ascending order

# local assertions llc (patched with MBP_SORT_SBCB / MBP_SKIP_JT_PUSH /
# MBP_DUMP_COUNT env hooks; pristine copy of the file is
# /dev/shm/w4103/MBP_clean.cpp):
LL=/home/debian/llvm7build/build/bin/llc
$LL -mtriple=armv7-none-linux-androideabi16 -float-abi=soft -filetype=obj \
    -O2 int.ll -o base.o
MBP_SORT_SBCB=1     $LL … int.ll -o sorted.o   # 0x584, original order
MBP_SKIP_JT_PUSH=1  $LL … int.ll -o skip.o     # 0x588, original bodies,
                                               #   tail1 after 150
MBP_DUMP_COUNT=1    $LL … int.ll -o /dev/null 2> counts.txt   # COUNT lines
python3 order.py sorted.o / skip.o             # decode the TBH tables
```

Scratch (not committed): `/dev/shm/w4505/` — `Status_int.cpp` (plain-int
form), `int.ll`, `mbp_mc.txt` (full MBP trace), `counts.txt`
(`UnscheduledPredecessors` dump), `mbp_android.cpp`, `sdb_android.cpp`
(Android-fork sources at `1d739ffb…`), `sorted.s`, `skip.s`, `mkvars.py`,
`mk2sw.py`, `mkchain.py`, `blocks3.py`, `order.py`, `try.sh`, `cc.sh`,
`ccir.sh`.
