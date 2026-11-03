# `Status::missionCompleted` — `Mission::id` semantics and the rank-load audit (w3830)

Status: research note + one semantic fix landed. Worktree `wt/w3830`, branch
`wave89-w3830`, base `5710f6d0`. Original:
`../_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a, NDK r18b clang 7.0.2
`r328903`, LLVM `1d739ffb…`, `-Oz`, image base 0x10000; real VMA = Ghidra − 0x10000).

Baseline and after: **byte_exact 4498 / linked_exact 4498 / .text 0x1337c8**,
`Status::missionCompleted` **86.7 % / 1432 B**, lint `CLEAN ✓`, drift 0.

## 1. `Mission::id` is the raw mission type, not a source-order rank

The wave-88 hypothesis was that `Mission::id` stores source-order ranks and
`getType()` returns that rank, which would make the original switch a direct
40-entry TBH with no lookup. The binary disproves it:

* `Mission::getType()` is exactly `ldr r0, [r0, #12]; bx lr` (4 B at
  `0x15bae6`). It returns the field at offset 12 unchanged.
* `Mission::setType(int)` is `str r1, [r0, #12]; bx lr` (`0x15bcc8`), no mapping.
* The `Mission` constructors store the caller's `id` directly at offset 12
  (`0x15b64d`, `0x15b771`, `0x15b89d`, `0x15b979`).
* `Generator::createMission` constructs `new Mission(type, …)` with the raw
  freelance types 0..15; `Status`'s campaign-mission creation constructs
  `new Mission(156, …)`, `new Mission(161, …)`, etc. with the raw campaign types
  150..189. Those are the same values the original jump table indexes.

So `id` is the raw type. A source-order-rank `id` would require changing every
`getType()` comparison in the game and is not the original layout.

## 2. Original dispatch (re-confirmed)

```
abfd6: blx  Mission::getType()          @ exactly one call
abfda: sub.w r1, r0, #150
abfde: cmp   r1, #39
abfe0: bhi.n ac046
abfe2: tbh   [pc, r1, lsl #1]           @ 40 halfwords @ 0xabfe6
ac046: sub.w r1, r0, #8
ac04a: cmp   r1, #6
ac04c: bhi.n ac0be
ac04e: tbh   [pc, r1, lsl #1]           @ 7 halfwords @ 0xac052
```

No `sxth`, no `ldrb`, no rank-table literal. The physical body order is
`167/174, 8, 11, 13, 14, 150,151,152,153,154,155,158,162,166,170,183,189,
156,157,160,163,164,165,168,169,171,172, default` — the current source order.

## 3. Semantic fix: original type 183, not 184

Decoding the original high table gives the type→body map. Every entry matched the
current `kMissionCase` rank table except one pair:

| raw type | original body | current rank table before this pass |
|---|---|---|
| 183 | `0xac2b4` = `statusValue == 0` campaign body (source `case 165`) | default (`190`) |
| 184 | default (`0xac45a`) | rank 165 (the same campaign body) |

The table row was corrected to `… 190,165,190,190,190,190,190,166,190,190`
(indices 183=165, 184=190). This is a behaviour fix only: the rank set and body
order are unchanged, so the gate and the fuzzy score are unchanged
(86.7 %, 1432 B, byte 4498 / linked 4498).

## 4. The rank load is not the remaining blocker

A direct-rank experiment was compiled from the current body code: remove the
lookup, switch directly on `m->getType()`, and assign the 24 high case values
(source-order ranks) so the table spans 150..189 (40 entries) with the bodies in
source order. It compiles to a 40-entry TBH + 7-entry TBH, no rank load, and the
linked function is **1432 B / 86.7 %** — exactly the same fuzzy score as the
rank-table build. The full gate drops `byte_exact 4498 -> 4459` only because
removing the `.missioncase` section changes the section-order placement of the
other functions (`linked_exact` stays 4498); the function itself is unchanged at
86.7 %. So the three-instruction rank load is not what is costing the 13.3 %.

A second codegen-shape observation: changing the table from
`extern const unsigned char kMissionCase[192]` (hidden) to `static const` keeps
the same 1432 B but changes the body order to ascending rank
(`151/163 8 11 13 14 150 152 …`). The `extern` hidden declaration is load-bearing
for the source-order layout; it is not just a visibility choice.

## 5. Root cause

`SelectionDAGBuilder::sortAndRangeify` sorts case clusters by value and
`buildJumpTable` adds the jump-table MBB successors in that sorted table order
(LLVM 7 `SelectionDAGBuilder.cpp`, `// Add successors. Note: use table order for
determinism.`). The original's `MachineBlockPlacement` nevertheless laid the
bodies out in source order. The rank table forces that layout by making the case
values themselves source-order ranks; a direct 40-entry TBH on the raw types
always lays the bodies out in ascending type order under our build.

The remaining 13.3 % is therefore **within-body code shape** (register
allocation, scheduling, short/wide branch encodings, stack-slot colouring), not
the dispatch. Matching it requires reproducing the original body code closely
enough that MBP naturally chooses the source-order layout; a cross-TU
`id`-as-rank change would still leave the table values permuted and does not
improve the score (measured 86.7 %).

## 6. Reproduction

```sh
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
cmake --build cmake-build-match --target check -j8   # RATCHET PASS, byte 4498 linked 4498
cmake --build cmake-build-match --target lint -j8    # CLEAN
cmake --build cmake-build-match --target drift -j8   # 0 fields

# original dispatch / body order
OD=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OD -d --start-address=0xabf38 --stop-address=0xac4d0 \
    /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
```
