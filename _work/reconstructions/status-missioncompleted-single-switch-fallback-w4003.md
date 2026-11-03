# `Status::missionCompleted` — one switch, and the original is the MBP MBB-order fallback (w4003)

Status: research note (no source change landed). Worktree `wt/w4003`, branch
`wave90-w4003`, base master `360c7467`. Original binary:
`../_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a, NDK r18b clang
7.0.2 `r328903`, `-Oz`, image base 0x10000; real VMA = Ghidra − 0x10000).

Topic: `_ZN6Status16missionCompletedEbbx` is **57.7 % / 1432 B** with the
faithful-but-anchored `switch ((short) m->getType())` (the `sxth` is a size
anchor; the binary has none). This pass establishes two things that the
previous notes got wrong or left open:

1. **The original is a single `switch`, not two sequential switches.** It has
   exactly two `tbh` instructions, both belonging to the one dispatch; there is
   no second dispatch after the bodies.
2. **The original's body order is `MachineBlockPlacement`'s
   `getFirstUnplacedBlock` fallback (MBB order = source order), not a
   two-switch layout artifact.** Forcing `selectBestCandidateBlock` to return
   `nullptr` in a local LLVM build reproduces the original's body order
   *exactly*, including the default/latch at the end.

No landable change was found: the plain `int` switch (the faithful form, no
`sxth`) matches the original's prologue/dispatch byte-for-byte but is **1428 B
/ 60.2 %**, and a 4-byte size change shifts `.text` and drops `byte_exact`
4498 → 3043. Landing it requires the exact layout (which would also recover
the 4 bytes, see §5).

## 0. Baseline (measured in this worktree)

```sh
cmake --build cmake-build-match --target check -j8
```

```
verify: byte 4498  linked 4498  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`report.json` row: `_ZN6Status16missionCompletedEbbx` → `pct 57.7`,
`osize 1432`, `usize 1432`. `.text 0x1337c8`, `.rodata 0x41b67`, lint
`CLEAN ✓`, drift 0.

## 1. The original is a single switch (supersedes the two-switch reading)

`Status::missionCompleted` is at `0xabf38`, size 1432. It calls
`Mission::getType()` exactly once (`0xabfd6`) and contains **exactly two `tbh`
instructions in the whole function**:

```
$ grep -c tbh orig.asm
2
abfe2: tbh [pc, r1, lsl #1]     ; 40 halfwords @ 0xabfe6 (types 150..189)
ac04e: tbh [pc, r1, lsl #1]     ;  7 halfwords @ 0xac052 (types 8..14)
```

The second dispatch is *not* a second switch on the same value: it is the
`sub.w r1, r0, #8 / cmp r1, #6 / bhi` fallback of the same `SelectionDAGBuilder`
jump-table lowering (the high range is tested first, then the low range, then
`cmp r0, #0` for the lone outlier `case 0` at `0xac0be`).

Crucially, the bodies `156, 157, 160, 163, 164, 165, 168, 169, 171, 172`
(group B in the old two-switch hypothesis) are reached **directly from the
first TBH**:

```
idx type  entry  target
  6  156  0x01a2 0xac32a
  7  157  0x01b7 0xac354
 10  160  0x01d1 0xac388
 13  163  0x01e8 0xac3b6
 14  164  0x01f6 0xac3d2
 15  165  0x01f8 0xac3d6
 18  168  0x01fc 0xac3de
 19  169  0x020b 0xac3fc
 21  171  0x020d 0xac400
 22  172  0x021a 0xac41a
```

and every body's `break` goes to the loop latch `0xac45a`
(`add fp, fp, #1; b 0xabf82`). There is no dispatch after the bodies, so the
source is one `switch` and the *body layout* — not the source — is what has to
be explained.

Original body order (from the table targets):

```
167/174  8  0/11  13  14  150 151 152 153 154 155 158 162 166 170 184 189
156 157 160 163 164 165 168 169 171 172  default(latch)
```

which is exactly the `case` order in `src/game/mission/Status.cpp`.

## 2. The decisive experiment: force the MBP fallback

`MachineBlockPlacement` lays the bodies out through
`selectBestCandidateBlock(Chain, BlockWorkList)`. `BlockWorkList` is filled by
`markBlockSuccessors`, which walks `MBB->successors()`; for a jump-table MBB
that list is `buildJumpTable`'s `Table` order, i.e. **ascending case value**
(`SelectionDAGBuilder.cpp`: `// Add successors. Note: use table order for
determinism.`). All case bodies have identical block frequency
(`0.1865889213`), so `selectBestCandidateBlock` keeps the *first* one in the
worklist and the bodies come out in value order.

The only other placement path is `getFirstUnplacedBlock`, which walks the
`MachineFunction` in MBB order. To test whether that path reproduces the
original, a local LLVM 7.0.0 with assertions was built and
`selectBestCandidateBlock` was replaced by `nullptr`:

```c
// llvm-7.0.0.src/lib/CodeGen/MachineBlockPlacement.cpp, in buildChain():
     if (!BestSucc)
-      BestSucc = selectBestCandidateBlock(Chain, BlockWorkList);
+      BestSucc = nullptr; // FORCE
     if (!BestSucc)
       BestSucc = getFirstUnplacedBlock(Chain, PrevUnplacedBlockIt, BlockFilter);
```

(restore with `cp MBP_orig.cpp llvm-7.0.0.src/lib/CodeGen/MachineBlockPlacement.cpp`)

Compiling the project's own `Status.cpp` IR (the plain `int` switch) with the
patched `llc` and decoding the emitted jump tables gives:

```
order: 167/174 8 11 13 14 150 151 152 153 154 155 158 162 166 170 184 189
       156 157 160 163 164 165 168 169 171 172  default
```

— **byte-for-byte the original's body order**, including the latch/default at
the end. (The unpatched `llc` on the same IR gives ascending value order:
`167/174 8 11 13 14 150 151 152 153 154 155 156 157 158 160 162 163 164 165
166 168 169 170 171 172 default 184 189`.)

This is the existence proof: the original's layout is the MBP
`getFirstUnplacedBlock` fallback (MBB order), not a two-switch artifact. The
earlier note `status-missioncompleted-mbp-fallback-w4003.md` §3 ("the original
source really was written as two sequential switches") is superseded on this
point — a two-switch source also happens to reproduce the order (via
per-switch jump tables), but it is not what the binary shows (one dispatch,
one table) and it costs 76 B of register pressure.

Caveat: the forced-fallback build reproduces the **body-entry order** exactly,
but not the shared-tail placement. In the original, bodies 150/151/153/154/155
all branch to the shared `ac172` tail (`mov r0, sl; blx getStatusValue; cmp;
blt; b`), which sits immediately after body 155; in the local 7.0.0 forced
build body 150 inlines that tail and body 151 branches to it. Consequently the
forced build is 1416 B, not 1432 B (the local 7.0.0 build is 24 B smaller than
the NDK 7.0.2 build on the unpatched source, so the absolute size is not
comparable; the *order* is).

### 2.1 MBB order really is source order

From the unpatched `-debug-only=block-placement` trace of the same IR, the
jump-table successor list is value order:

```
%bb.11 (low JT, 1.02) %bb.36(150) %bb.38(151) %bb.39(152) %bb.44(153)
%bb.45(154) %bb.46(155) %bb.77(156) %bb.80(157) %bb.47(158) %bb.85(160)
%bb.57(162) %bb.88(163) %bb.91(164) %bb.92(165) %bb.62(166) %bb.93(168)
%bb.96(169) %bb.66(170) %bb.97(171) %bb.98(172) %bb.67(184) %bb.71(189)
```

but sorting those same blocks by MBB number gives the source order:

```
16(167/174) 36(150) 38(151) 39(152) 44(153) 45(154) 46(155) 47(158)
57(162) 62(166) 66(170) 67(184) 71(189) 77(156) 80(157) 85(160) 88(163)
91(164) 92(165) 93(168) 96(169) 97(171) 98(172)
```

So `getFirstUnplacedBlock` (MBB order) is exactly the original's order, and
`selectBestCandidateBlock` (worklist = successor order) is exactly the current
build's order. The two mechanisms are cleanly distinguished.

## 3. Why the fallback is not taken in our build

`getFirstUnplacedBlock` is only reached when `selectBestCandidateBlock` returns
`nullptr`, i.e. when `BlockWorkList` is empty after dropping already-placed
chains. Instrumenting `selectBestCandidateBlock` to print `WorkList.size()`
shows that the worklist is **33 entries at the moment the high jump table is
merged** and only ever shrinks to 0 at the very end of the loop chain (for a
block outside the loop):

```
Merging from %bb.104 to %bb.16            ; high JT -> 167/174
Selecting best successor for: %bb.16
    Not a candidate: %bb.17 -> 100.00% (prob) (non-cold CFG conflict)
  [selectBestCandidateBlock: worklist=33]
    %bb.11 -> 1.022594752 (freq)          ; low JT, picked
    %bb.36 -> 0.1865889213 (freq)         ; 150
    ...
```

The 33 entries are the 23 high bodies (pushed by `markBlockSuccessors` in
table/value order) plus 10 chains that `fillWorkLists` pushed at loop start.
Every body is still unplaced, so the worklist can never be empty there.

The original must therefore have had an **empty worklist at the jump table**,
which in LLVM 7 can only happen if the bodies are not pushed by
`markBlockSuccessors` — i.e. their `UnscheduledPredecessors` does not reach 0
at that point, or they are not in the loop's `BlockFilter`. Nothing in the
current source shape produces that.

## 4. Variant sweep (compile-only, `layout.py` decodes the TBH targets)

All variants below are semantically equivalent rewrites of the switch header;
each was compiled with the project's exact flags and its body order decoded.

| source form | size | high-table body order |
|---|---:|---|
| `switch ((short) m->getType())` (committed) | 1432 | value order (57.7 %) |
| `switch (m->getType())` (int, faithful) | 1428 | value order (60.2 %) |
| `switch ((int) m->getType())` | 1428 | value order |
| `int t = m->getType(); switch (t)` / `const int` | 1428 | value order |
| `switch (m->getType() + 0)` / `- 0` / `* 1` / `\| 0` / `0 + …` | 1428 | value order |
| `switch (m->getType() & 0x7fffffff)` | **1432** | value order (57.7 %) |
| `default: break;` first / last in the switch | 1428 | value order |
| `default: continue;` | 1428 | value order |
| `if (1) switch (…)` | 1428 | value order |
| `if (m->getType() != -1) switch (…)` | 1424 | value order |
| `while (i < missions->size())` instead of `for` | 1428 | default first, then value order |
| `-mllvm -disable-block-placement` | 1436 | garbled |
| `-mllvm -force-loop-cold-block` | 1456 | value order, 184/189 before default |
| `-mllvm -loop-to-cold-block-ratio=1/2/10/100` | 1428 | value order |
| `-mllvm -block-placement-exit-block-bias=100` | 1472 | reverse value order |
| `-mllvm -force-precise-rotation-cost` | 1448 | value order, 184/189 before default |
| **forced `getFirstUnplacedBlock` (patched llc)** | — | **exact original order** |

Note that `& 0x7fffffff` reaches 1432 B but still scores 57.7 % (the extra
`and` in the dispatch offsets the size win), so matching the size alone buys
nothing.

## 5. Where the 4 bytes are: layout, not missing code

Per-body instruction counts and sizes of the original vs the plain `int`
build are identical for 167/174, 8, 0/11, 13, 14, 150..155, 158, 162, 166,
157, 164, 165, 169, 171; the only size deltas are short/wide branch
encodings caused by the different layout:

| body | orig | int | Δ |
|---|---:|---:|---:|
| 156 | 42 | 44 | +2 |
| 160 | 46 | 50 | +4 |
| 163 | 28 | 30 | +2 |
| 168 | 30 | 24 | −6 (literal-pool `nop` padding in the original) |
| 170 | 14 | 12 | −2 |
| 172 | 64 | 66 | +2 |

Net +4 ⇒ 1432 vs 1428. There is **no missing statement**: the prologue and the
whole dispatch are byte-identical between the original and the `int` build
(first difference is only the 4-byte-shifted branch offsets). Recovering the
4 bytes therefore requires the layout, not a source addition.

## 6. Conclusion — proven vs open

**Proven:**

1. The original is a **single** `switch` (2 `tbh`, one dispatch, no second
   dispatch; group-B bodies hang directly off the first table).
2. The original's body order is **MBB order = source order**; the current
   build's is **jump-table successor order = value order**.
3. Forcing `MachineBlockPlacement::getFirstUnplacedBlock` reproduces the
   original's body order **exactly** (patched local LLVM 7.0.0, project IR).
4. The fallback is never taken in our build: the worklist holds 33 entries
   when the high jump table is merged and never empties during body placement.
5. The plain `int` switch is the faithful form (matches the prologue/dispatch
   byte-for-byte) and is 1428 B / 60.2 %; the 4-byte deficit is entirely
   branch-encoding drift from the wrong layout. The `(short)` cast is a size
   anchor.
6. No plain single-switch shape, case order, `default` placement, loop form,
   or MBP flag tried here produces the fallback order at 1432 B.

**Open:** the source shape that makes `BlockWorkList` empty at the jump table
(so `getFirstUnplacedBlock` runs) while keeping the plain-`int` register
allocation *and* the original's shared-tail placement. That is the only
remaining blocker; if found, the function should become byte-exact (1432 B)
and `byte_exact` should rise 4498 → 4499. Note that the forced-fallback build
gets the order right but not the tails, so the fallback alone is necessary but
not sufficient.

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w4003
cmake --build cmake-build-match --target check -j8   # RATCHET PASS, byte 4498 linked 4498
cmake --build cmake-build-match --target lint  -j8   # CLEAN
cmake --build cmake-build-match --target drift -j8   # 0 fields

OD=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OD -d --start-address=0xabf38 --stop-address=0xac4d0 \
    /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so

# body order of the current object / of a variant
/dev/shm/w4003/cc.sh "$PWD/src/game/mission/Status.cpp" /dev/shm/w4003/cur.o
python3 /dev/shm/w4003/layout.py /dev/shm/w4003/cur.o

# forced-fallback experiment (local assertions LLVM 7.0.0)
#   1. patch selectBestCandidateBlock -> nullptr in
#      llvm-7.0.0.src/lib/CodeGen/MachineBlockPlacement.cpp
#   2. ninja -C llvm-build llc -j16
#   3. llvm-build/bin/llc -mtriple=thumbv7-none-linux-androideabi \
#          -mattr=+neon -float-abi=soft -filetype=asm -o out.s sint_only7.ll
#   4. python3 asmorder.py out.s
```

Scratch (not committed): `/dev/shm/w4003/` — `sint_only7.ll` (plain-int IR),
`force_int.s` + `asmorder.py` (forced-fallback layout), `sint_trace2.txt`
(instrumented MBP trace), `g_*.cpp` / `h_*.cpp` (variant sweep),
`bodies_orig.txt` / `bodies_ours.txt` (per-body disassembly),
`llvm-7.0.0.src/`, `llvm-build/`.
