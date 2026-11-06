# `Status::missionCompleted` — wave 119 (w6971): the plain-switch body order is
# jump-table order, and why the rank table cannot be dropped

Status: **no source change** (dead-end sweep; the landed rank-table build is
still the best).  Worktree `wt/w6971`, branch `wave119-w6971`, base master
`d512ec24e`.  Original binary: `../_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 r328903, `-Oz`, image base 0x10000).

Target: `_ZN6Status16missionCompletedEbbx` (`Status::missionCompleted(bool,
bool, long long)`), original `0xabf38`, size **1432 B**.

| | before (HEAD `d512ec24e`) | best found this pass |
|---|---:|---:|
| function score | **92.5 %** | **92.5 %** |
| function size | 1432 B | 1432 B |
| gate | byte 4505 / linked 4505 | unchanged |

Nothing structural moved: 27 sections, `.text` 0x69440/0x1337c8,
`.ARM.exidx` 0x19cc08/0x6f70, `.ARM.extab` 0x1a3b78/0x26298,
`.rodata` 0x1c9e10/0x41c27 (the 192-byte rank table past the exact original
image at 0x41b67), `.data` 0x214000/0x296c, file size 2,187,328 B.

## 1. The decisive measurement: the original's TBH is *value-ordered*

The original's big dispatch is `sub.w r1,r0,#150; cmp r1,#39; bhi; tbh`
(`0xabfda`, TBH base `0xabfe6` = rel 0xae, 40 halfwords).  Decoding it gives

```
slot 0..39 (type 150..189) -> body offset
1d8 1de 1e4 218 21e 236 3f2 41c 248 522 450 522 2a4 47e 49a 49e 30a fe
4a6 4c4 36e 4c8 4e2 522 fe 522 522 522 522 522 522 522 522 522 37c 522
522 522 522 3b6
```

Slot *i* is type `150+i`, so the jump table is **type/value ordered** — slot 6
is type 156's body (0x3f2), slot 8 is type 158's body (0x248).  But the *body
layout* is the A-then-B order

```
167/174, 150 151 152 153 154 155 158 162 166 170 184 189,
         156 157 160 163 164 165 168 169 171 172, default
```

So the body layout is **not** the jump-table successor order.  This is the crux
of the whole problem and it rules out every "reorder the case labels" idea:
`SelectionDAGBuilder::buildJumpTable` (`lib/CodeGen/SelectionDAG/
SelectionDAGBuilder.cpp`) adds the JT MBB's successors in `Table` order, and
`Table` is built from `Clusters`, which `sortAndRangeify` sorts by value.  The
`MachineBlockPlacement` worklist is therefore value-ordered, and
`selectBestCandidateBlock` keeps the *first* max-frequency block, so a plain
`switch (m->getType())` always lays the bodies out in ascending value order.

Verified directly with an instrumented clang (`JT_TRACE` print in
`buildJumpTable`): for the plain switch the 40-entry table is
`150,151,152,153,154,155,156,157,158,default,160,default,162,...` — value
order.  The MBP worklist is the same sequence (`WORKLIST size=23: bb.default
bb.150 bb.151 ...`), and the merges follow it
(`MERGE 150 -> 151`, `MERGE 151 -> 152`, ...).

## 2. Why the rank table is the only mechanism that works

The body layout is the *sorted case-value* order.  The original's layout is
A-then-B, so the original's switch values must have been A-then-B — but the
dispatch (`sub #150; cmp #39`) says the values are 150..189.  The two facts are
only consistent if the original's MBP did **not** follow the worklist, e.g. it
fell back to `getFirstUnplacedBlock` (MBB/source order).  Forcing that with an
instrumented clang (`MBP_FORCE_FALLBACK=1`) reproduces the original layout
exactly (1440 B, 63.8 %), which confirms the original source order is A-then-B
but does not give a landable source shape.

The landed rank table (`tools/mission_case_table.cpp`, indexed through
`#define MISSION_CASE ((const unsigned char *) 0x20b977)`) makes the *sorted
case values* A-then-B by permuting the case labels, which is why it reproduces
the original body layout.  Its cost is the extra `ldr/ldrb` dispatch and a
permuted TBH.

## 3. Measured variants (same `link_so` + single-symbol score)

| variant | usize | pct | body order |
|---|---:|---:|---|
| **landed rank table (HEAD)** | **1432** | **92.5** | original A-then-B |
| plain switch (w6943) | 1432 | 59.4 | value |
| rank table, ranks in 150..189 (identity dispatch) | 1436 | 75.6 | original |
| rank table, 167->17 / 174->24 | 1432 | 92.3 | original |
| rank table, 167->17 / 174->24 / 172->39 | 1432 | 92.3 | original |
| rank table, 174->24 | 1432 | 92.3 | original |
| rank table, 174->26 | 1432 | 92.3 | original |
| rank table, 172->24 / 174->39 | 1432 | 92.3 | original |
| rank table, 172->25 / 174->39 | 1432 | 92.3 | original |
| rank table, 167->20 / 168->21 | 1432 | 92.5 | original |
| rank table, 167->22 / 169->21 | 1432 | 92.3 | original |
| 12 random rank permutations (max rank 39) | 1432 | 82.5..90.4 | original |
| body 166 re-query (`hasEquipment(m->getProductionGoodIndex(),1)`) | 1440 | 75.8 | original |
| drop body-168 `(unsigned short)` cast | 1432 | 74.9 | original |
| re-query + drop cast | 1440 | 75.8 | original |
| `const unsigned char *missionCase = MISSION_CASE;` local | 1432 | 92.5 | original |
| plain switch + dup labels on all 12 A bodies | 1376 | 45.8 | original |
| plain switch + 3rd label on 167/174 + dup on A bodies | 1376 | 47.7 | original |

Notes:

* **Ranks in 150..189** (so the switch value is a permutation of the type
  range) makes the dispatch `ldr/ldrb/sub.w #150/cmp #39/bhi/tbh` — the `sub.w`
  is *added* because the rank is loaded from a byte table, so the function
  grows to 1436 B and the score collapses to 75.6 %.  The landed 0..39 ranks
  avoid the `sub` (range starts at 0) and keep the dispatch at 12 B, the same
  size as the original's.
* **Duplicate case labels** (each A body gets a second label taken from a
  default slot) is the only *plain-switch* shape that yields the original
  A-then-B layout: the A bodies then have frequency 2/40 and are placed before
  the B bodies.  But the extra labels change the CFG enough that the compiler
  emits 24 fewer instructions (499 vs 523) and the function is 1376 B
  (-56 B), so every body address shifts and the score is 45.8-47.7 %.
* **Body-size drift** in the landed build: body 166 is 4 B shorter than the
  original (cached `index` instead of a re-query), body 189 is 8 B longer (the
  rank-table literal-pool words land inside its loop), body 168 carries the
  `(unsigned short)` size device.  The three net to zero, which is what keeps
  the function at exactly 1432 B and the following symbol at `0xac4d0`.
  Fixing any one of them alone shifts the layout and costs 15-17 points.

## 4. TBH ceiling

The original TBH and the landed build's TBH have an LCS of **27/40** halfwords.
The ceiling is **30/40**: only the bodies whose addresses already match can
ever compare equal (`150..155`, `158`, `162`, `166`, `169`, `171`, `172`,
`167/174`, and the 16 default slots), and the A-then-B layout forces type 158's
body into slot 6 while the original has type 156 there.  The address drift of
`170/184/189/156/157/160/163/164/165/168` (from the body-size drift above)
removes the rest.  No rank permutation found beats the landed one; the 12
random permutations plus the targeted ones all scored 82.5-92.5 %.

## 5. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6     # RATCHET PASS, byte 4505

# single-function score (fast, ~1 s)
python3 /dev/shm/w6971/score.py _ZN6Status16missionCompletedEbbx
# TBH decode of the linked .so (base rel 0xae, 40 entries)
python3 /dev/shm/w6971/tbh.py
# TBH LCS against the original
python3 /dev/shm/w6971/lcs.py
```

Scratch (not committed): `/dev/shm/w6971/` — `score.py`, `tb.py`, `lcs.py`,
`bodies.py`, `diff.py` (instruction-level diff via `tools/verify/verify.py`),
`rebuild.sh`, `gen.py` (rank-permutation source generator), `sweep.py`
(random rank sweep), `Status_rank.cpp` (HEAD source), `Status_plain.cpp`
(w6943 plain switch), `Status_dupA.cpp` / `Status_dupB.cpp` (duplicate-label
variants), `Status_r17c.cpp` / `Status_r30.cpp` / `Status_r31.cpp` (rank
permutations), `Status_rq.cpp` / `Status_rq2.cpp` / `Status_nouxth.cpp`
(body-166 / body-168 variants).

The instrumented clang is `/tmp/llvm_android_702b/build2` (a `JT_TRACE` print
was added to `SelectionDAGBuilder::buildJumpTable` this pass; rebuild with
`ninja -j8 clang` in `build2`).

## 6. w6971 follow-up: rank-slot DP and extra variants

The TBH is 40 halfwords.  The body order is fixed (A-then-B), so the only
freedom left in `tools/mission_case_table.cpp` is *which* 24 of the 40 slots
the 24 bodies occupy (the rest stay `250`/default).  A DP over the slot choice
(original TBH as the target, our actual drifted body offsets as the source)
gives a strict-LCS ceiling of **31/40** with the ends pinned (body 150 at slot
0, body 172 at slot 39, so the table stays 40 entries and the `cmp #39`
dispatch is unchanged), versus the landed table's 28-29.  The optimal slot set
is `0..13, 15, 17..25, 39`, i.e. ranks
`150:0 151:1 152:2 153:3 154:4 155:5 158:6 162:7 166:8 170:9 184:10 189:11
156:12 157:13 160:15 163:17 164:18 165:19 168:20 167:21 174:22 169:23 171:24
172:39`.

Measured with that table: **still 92.4546 % (484/1047 matches)**, exactly the
same as the landed table.  Several other permutations (174<->22/23/24 swaps,
172<->25, the DP optimum) also give 484/1047.  The score is therefore *not*
limited by the TBH permutation: the 11 non-matching TBH halfwords and the 7
branch-target mismatches all come from the body-address drift, not from which
slot each body sits in.

Extra body-166 / body-168 variants (all 1432 B unless noted):

| variant | matches / 1047 | pct |
|---|---:|---:|
| landed (HEAD) | 484 | 92.4546 |
| body 166 re-query | 398 | 75.81 (1440 B) |
| body 166 `ship->hasEquipment(index,1)` | 480 | 91.87 (1428 B) |
| body 166 `ship->hasEquipment(re-query,1)` | 393 | 75.00 (1436 B) |
| body 166 swap index/amount decls | 483 | 92.26 |
| body 166 `unsigned` / `long` / `const` / cast | 484 | 92.4546 |
| body 168 `(short)` cast | 484 | 92.4546 |
| body 168 `(unsigned short)` on both sides | 482 | 92.07 |
| body 168 no cast | 392 | 74.88 |
| `switch ((int)MISSION_CASE[...])` | 484 | 92.4546 |
| `switch (MISSION_CASE[(unsigned char)...])` | 372 | 70.92 (1436 B) |
| `default: break;` added | 484 | 92.4546 |
| body 189 `while` loop | 484 | 92.4546 |
| `const unsigned char *missionCase` local | 484 | 92.4546 |
| static table in Status.cpp (PC-relative) | 372 | 70.92 (1436 B, .rodata shifts) |
| for-loop -> while-loop (plain switch) | 162 | 31.27 (1400 B) |

The remaining 39 mismatched instructions are: 1 dispatch (`sub/cmp` vs
`ldr/ldrb/cmp`), 1 `tbh` register, 11 TBH halfwords, 1 second-dispatch
(`#8` vs `#208`), 1 `cmp #0` vs `#200`, 7 branch-target offsets, 1 `uxth`,
2 pool words, and ~14 register-allocation/block-merge differences in body 166
(`this` is spilled to `r1` so `r6` can hold `amount`, and the `hasEquipment`
tail merges into the `and/lsls` block at `0xac1d4` instead of the `cbnz` block
at `0xac458`).  Those are all downstream of the three body-size drifts
(-4/+8/-4), which are in turn caused by the rank-table literal-pool word
creating the `0xac300` island and by the body-166 cached `index`.  Fixing any
one drift alone moves every later body and costs 15-17 points.
