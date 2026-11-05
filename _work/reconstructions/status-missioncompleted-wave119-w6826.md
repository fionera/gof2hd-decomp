# `Status::missionCompleted` — wave 119 (w6826): rank-table variants measured,
# the TBH permutation is at its ceiling, and the body-size deltas are pinned

Status: **research note (no source change landed)**.  Worktree `wt/w6826`,
branch `wave119-w6826`, base master `d4d2d3f91`.  Original binary:
`../_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a, NDK r18b clang
7.0.2 r328903, `-Oz`, image base 0x10000).

Target: `_ZN6Status16missionCompletedEbbx` (`Status::missionCompleted(bool,
bool, long long)`), original `0xabf38`, size **1432 B**.  Baseline row:
**92.5 % / 1432 B** (byte False, linked False).  Gate after this pass (source
unchanged):

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

This note records (a) the exact remaining instruction deltas, (b) that the
committed `kMissionCase` rank assignment is optimal for the jump-table bytes,
and (c) the measurements of every source variant tried this wave.  No variant
beat 92.45 %.

## 1. Baseline instruction deltas (523 orig vs 524 ours)

`python3 /dev/shm/w6826/vdiff.py` (difflib over the verify-normalised stream,
autojunk off) gives **ratio 92.45 %**, 523 vs 524 instructions.  The deltas are:

1. **Dispatch** — original `sub.w r1,r0,#150; cmp r1,#39; bhi; tbh` (16 B);
   ours `ldr r1,[pc,#804]; ldrb r0,[r0,r1]; cmp r0,#39; bhi; tbh` (16 B).
   Same size (the `ldr` is the 16-bit `49c9` form), but one extra instruction
   and the compare/`tbh` register is `r0` instead of `r1`.
2. **TBH contents** — 29 of 40 entries match (see §2).
3. **Low dispatch** — ours `sub r1,r0,#208; cmp r1,#6` vs original
   `sub r1,r0,#8; cmp r1,#6`; the type-0 check is `cmp r0,#200` vs `cmp r0,#0`.
   Both are consequence of the 200..214 low ranks that force the second TBH.
4. **Body 166 (case 10)** — ours is 96 B, original 100 B.  The original
   re-queries `m->getProductionGoodIndex()` for the trailing `hasEquipment`;
   the committed source reuses `index` (a deliberate 4 B size device).  The
   original also keeps `index` in `r9` and `amount` in `r5`; ours uses
   `r5`/`r6` and emits `mov r1,r6; mov r6,r0; ldr [r1,#400]` where the
   original has `ldr [r6,#400]`.
5. **Body 189 (case 13)** — ours is 68 B, original 60 B.  The 8-byte literal
   pool holding `0x0020b977` (`MISSION_CASE`) and the `Globals::status` GOT
   word is emitted *inside* body 189 (right after its `b.n` preheader branch,
   at `0xac300`); the original's `Globals::status` word lives at `0xac3f8`
   (with a `nop` at `0xac3f6`).  This shifts every later body by +4..+8 and
   produces the branch-target deltas at orig[349], [352], [363], [383], [386],
   [397], [424].
6. **Body 168 (case 20)** — ours has an extra `uxth r0,r0` (the
   `(unsigned short)` size device); the original compares directly.
7. **End pool** — original ends with `nop; .word 0x001640ee`; ours ends
   without them (the word moved into body 189).

Body-size ledger (address deltas between consecutive TBH targets):

| body | original | ours | delta |
|---|---:|---:|---:|
| 166 (case 10) | 100 | 96 | -4 |
| 170 (case 11) | 14 | 14 | 0 |
| 184 (case 12) | 58 | 58 | 0 |
| 189 (case 13) | 60 | 68 | +8 |
| 156 (case 14) | 42 | 42 | 0 |
| 168 (case 20) | 30 | 26 | -4 |
| all others | — | — | 0 |

Net 0, so the function size is exact; the addresses drift by ±4/±8 in the
middle.  Fixing all three (166 +4, 189 -8, 168 +4) would put every body at the
original address and raise the TBH match from 29 to 30.

## 2. The rank table is at its ceiling

The committed `tools/mission_case_table.cpp` assignment is
`layout -> rank = [0,1,2,3,4,5,6,9,10,11,12,13,14,15,16,17,18,19,20,22,23,39]`
for the 22 worklist bodies plus `167->21`, `174->25` (the 167/174 body is
placed first as the jump table's fallthrough successor because it has 2/40
edge probability, so its rank does not affect the layout).

A 300 000-sample random search over *all* monotone 22-rank assignments plus
the two free 167/174 ranks (each sampled from the remaining 18 slots) finds
nothing better than the committed assignment: **29/40** difflib matches
(LCS also 29).  The search is in `/dev/shm/w6826` (`tbhop.py` is the earlier
LIS proof; the wave-119 w6805 note proves the 16-body LIS bound).

So the TBH bytes cannot be improved without changing the mechanism.

## 3. Variants measured this wave (all worse)

Every variant was built with `cmake --build cmake-build-match --target
link_so -j6` and scored with `vdiff.py` (same normalisation as `verify.py`).
`full` is the full `verify.py` fuzzy percentage where it was run.

| variant | size | score | notes |
|---|---:|---:|---|
| committed | 1432 | **92.45** | baseline |
| body 166 re-query `hasEquipment(m->getProductionGoodIndex(),1)` | 1440 | 75.8 (full) | +8 B shifts every later body; TBH values +8 |
| drop `(unsigned short)` cast (no `uxth`) | 1430 | 74.88 | -2 B shifts body 169 onward |
| `static const unsigned char MISSION_CASE[192]` at file scope | 1434 | 70.92 | PIC `ldr; add pc; ldrb` (3 insns), table +2 B |
| big ranks `150+p`, low ranks identity | 1436 | 76.15 | adds `sub` to the big dispatch |
| big ranks `40+p`, low ranks identity | 1442 | 44.88 | low+big clusters merge into one 66-entry TBH |
| body 166 `Array<Item*>* cargo = ship->getCargo();` | 1432 | 92.45 | identical codegen |
| body 166 swap `amount`/`index` declaration order | 1432 | 92.26 | different RA |
| body 166 `Globals::status->ship->getCargo()` | 1432 | 91.88 | extra GOT load |
| `int mtype = m->getType(); switch (MISSION_CASE[mtype])` | 1432 | 92.45 | identical |
| `const unsigned char *mcase = MISSION_CASE; switch (mcase[...])` | 1432 | 92.45 | identical |
| single-expression `Item::isInList(m->getProductionGoodIndex(), ...)` | 1440 | 75.81 | same 104 B body 166 as the re-query |
| body 166 re-query in a nested scope / `index2` local | 1440 | 75.81 / 74.67 | RA still picks r5/r6, not r9/r5 |
| body 166 `const int index` / `const int amount` | 1440 | 75.81 | identical |
| body 189 moved to rank 24 (`case 24:` + table[189]=24) | 1432 | 87.02 | pool moves to `0xac314` (body 156), not `0xac3f8`; TBH match drops |
| body 189 moved to rank 24 with table only (source unchanged) | 1432 | 92.45 | body stays at rank 13; dead `case 24` default |
| plain `switch (m->getType())`, type labels, ascending source order | 1432 | 58.40 | dispatch matches (`sub #150`) but TBH is value order |
| plain `switch (m->getType())`, type labels, original body order | 1432 | 58.40 | layout is still value order; source order does not move MBP |

The plain-switch variants are the important control: with type case labels
(`150..189`) the dispatch is the original `sub.w r1,r0,#150; cmp r1,#39`
and body 166 gets the original register allocation (`index` in `r9`,
`amount` in `r5`, `this` kept in `r6`), but the body layout is value order
and the score collapses to 58.4 %.  The rank table is what buys the layout;
the table lookup is also what perturbs body 166's allocation (the `ldrb`
clobbers `r0`, and the global allocator then spills `this` instead of using
`r9` for `amount`).  Moving body 189 to a later rank does move the pool
(from `0xac300` to `0xac314`) but not to the original `0xac3f8`: the
16-bit `ldr` at the dispatch can only reach 1020 B, so the shared
`MISSION_CASE`+`Globals::status` island must stay before `0xac3d6`.

## 4. Why the table cannot be removed (restated, with the new evidence)

The wave-119 w6805 note proved the original's big-body order is exactly the
`MachineBlockPlacement` **fallback** (MBB creation) order: with
`MBP_FORCE_FALLBACK=1` the plain switch reproduces the original layout
(including the low group in value order), and the plain switch alone gives
value order.  The blocker is `markBlockSuccessors`: when the big jump-table
block is placed, each big body's chain has `UnscheduledPredecessors == 1`
(only the jump table), so all 22 are pushed onto `BlockWorkList` and
`selectBestCandidateBlock` then picks them in value order (all frequencies
tie at 256).  The original's CFG also gives every big body exactly one
predecessor (verified by scanning every branch target in
`0xabf38..0xac4d0`), so a source shape that empties the worklist would have
to add a *second* predecessor edge to each of the 22 bodies, which no natural
C++ shape tested produces.  Removing the table therefore still requires
either a second predecessor per body or a source-order successor list, and
`buildJumpTable` emits value order.

## 5. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6   # RATCHET PASS, byte 4505
```

Scratch (not committed): `/dev/shm/w6826/` — `vdiff.py` (per-function difflib
dump), `layout2.py` / `check.py` (TBH decode), `cc.sh` (single-TU compile with
the project flags), `Status_plain.cpp` / `plain_fb.o` (plain switch and the
forced-fallback object), `mbp_plain.log` / `mbp_fb.log` (instrumented-`llc`
MBP traces), and the variant generators `gen.py` / `gen2.py` / `mkvar.py`.
