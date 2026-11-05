# `Status::missionCompleted` — wave 118: rank-table optimum confirmed, no source win (w6614)

Status: **research note (no source change landed)**.  Worktree `wt/w6614`,
branch `wave118-w6614`, base master `3d4a98a3e`.  Original binary:
`../_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a, NDK r18b clang
7.0.2, `-Oz`, image base 0x10000).

Target: `_ZN6Status16missionCompletedEbbx` (`Status::missionCompleted(bool,
bool, long long)`), original `0xabf38`, size **1432 B**.  Baseline row:
**92.5 % / 1432 B** (byte False, linked False).  Gate after this pass:
`RATCHET PASS`, `byte_exact 4501`, `linked_exact 4501`, imports 207,
`lint CLEAN`, `drift 0`, 27 sections, exact file size 2,187,328 B.

No variant beat the committed wave-117 rank table.  This note records the
measurements, the proof that the rank table is already LCS-optimal, and the
remaining irreducible deltas.

## 0. Baseline measurement

```sh
cmake --build cmake-build-match --target check -j6
# verify: byte 4501 linked 4501 stubs 0 missing 0 extra 0 avg 99.92
# data:   exact 843 of 843 exported objects
# sodiff: extra-beyond-allow 0 missing 0 allowed 0 imports: 207
# RATCHET PASS
python3 tools/verify/verify.py ... --show _ZN6Status16missionCompletedEbbx
# pct 92.5  osize 1432  usize 1432  byte False  linked False
```

LCS decomposition of the 92.5 % (difflib ratio `2*484/(523+524)`):

| region | mismatches |
|---|---:|
| dispatch (`sub #150` vs `ldr/ldrb`) | 3 |
| high TBH table (40 halfwords) | 11 |
| low dispatch (`sub #8` vs `sub #208`) | 1 |
| literal-pool `ldr` immediate | 1 |
| outer default (`cmp #0` vs `cmp #200`) | 1 |
| body 158 scheduler slot (`ldr r6,[sp,#16]`) | 2 |
| body 166 (index reuse instead of re-query) | 10 |
| branch-target offsets after the body-166 -4 shift | 7 |
| body 168 (`uxth` + pool placement) | 3 |

## 1. The rank table is already optimal (verified by search)

The body layout is fixed to the original order `L` by the case-label
permutation in `tools/mission_case_table.cpp`.  The only remaining degree of
freedom in the table is the monotone label assignment (labels increasing
along `L`, shared 167/174 body peeled first so its two labels are free).

A 200 000-sample random search over monotone assignments (22 high bodies
drawn from 0..39, shared labels free) against the original 40-entry TBH
target sequence finds **LCS 30** and the committed wave-117 assignment is a
global optimum.  No table re-tune can gain anything.

## 2. Variant sweep (official verifier, full `link_so`)

All variants were built with the real tree (`src/game/mission/Status.cpp` +
`tools/mission_case_table.cpp`) and scored with the official verifier.

| variant | pct | usize | note |
|---|---:|---:|---|
| **committed wave-117 rank table** | **92.5** | **1432** | baseline |
| `int rank = MISSION_CASE[...]; switch (rank)` | 92.5 | 1432 | identical codegen |
| `const int` / `(int)` / `(unsigned)` cast | 92.5 | 1432 | identical codegen |
| `(signed char) MISSION_CASE[...]` | 60.1 | 1168 | negative labels, different cluster |
| body 166: re-query `getProductionGoodIndex()` | 75.8 | 1440 | +8 B, shifts all later bodies |
| body 166: re-query via `||` / `bool ok` / `if(!ok)` | 75.8 | 1440 | same |
| body 166: `int eq = getProductionGoodIndex()` local | 74.7 | 1440 | same |
| body 168: drop `(unsigned short)` | 74.9 | 1432 | −2 B, breaks latch 0x522 |
| body 168: swap `setWon`/`field_178` order | 89.5 | 1424 | |
| body 158: swap `hasGood`/`hasSpecial` decls | 92.1 | 1432 | |
| body 158: `if (hasGood && hasSpecial)` variants | 92.5 | 1432 | h1/h2 identical |
| body 158: `Item *it` hoist (4 shapes) | 44.9–90.9 | 1364–1432 | worse |
| body 158: `int` instead of `bool` flags | 59.8 | 1472 | |
| body 166: `amount` before `index` | 92.3 | 1432 | |
| case 6 moved to source positions 0..5 | 92.5 | 1432 | neutral |
| case 6 moved to source positions 6..15 | 90.2 | 1432 | worse |
| split shared case 200/211 into two bodies | 92.5 | 1432 | tail-merged back |
| split shared case 21/25 into two bodies | 91.3 | 1432 | |
| high labels +15 (15..54, low 208..214) | 75.6 | 1436 | +4 B dispatch `sub #15` |
| high labels +1, outlier 0 (labels 0..40) | 63.7 | 1428 | outlier merges into high table |
| high labels 15..54, low 8..14, outlier 0 | 82.0 | 1432 | one 55-entry table (ranges merge) |
| two sequential switches on cached `int` | 47.1 | 1504 | reproduces `L`, +72 B |
| nested switch in `default` | 58.4 | 1424 | inner switch folded away |
| plain `switch (m->getType())`, L source order | 58.4 | 1424 | value-order layout |

## 3. Why the dispatch cannot be fixed cheaply

The original dispatch is

```
sub.w r1, r0, #150
cmp   r1, #39
tbh   [pc, r1, lsl #1]      ; 40 entries, types 150..189
...
sub.w r1, r0, #8
cmp   r1, #6
tbh   [pc, r1, lsl #1]      ; 7 entries, types 8..14
...
cmp   r0, #0                ; lone `case 0` outlier
```

The committed rank table emits

```
ldr   r1, [pc, #804]        ; MISSION_CASE at 0x20b977
ldrb  r0, [r0, r1]
cmp   r0, #39
tbh   [pc, r0, lsl #1]      ; same 40 entries (permuted)
...
sub.w r1, r0, #208
cmp   r1, #6
tbh   [pc, r1, lsl #1]
...
cmp   r0, #200
```

Both dispatches are 10 B, so the size and every body address match.  To match
`cmp r1,#39` / `tbh r1` the switch range must start at 0 *and* the rank must
live in `r1`; to match the low `sub #8` the low labels must be 8..14, which
overlaps the high 0..39 range and makes clang merge the two tables (measured:
82.0 %, one 55-entry table).  Any high range that starts above 0 costs a
`sub.w` (+4 B) and shifts every body and every TBH target, which costs far
more than the two dispatch instructions it recovers (measured: 75.6 % at
1436 B).

The `cmp r0,#200` vs `cmp r0,#0` outlier check is the same story: label 0 is
already used by body 150, and moving the outlier to 0 forces the high range
to start at 1 (+4 B) and merges the clusters (63.7 % at 1428 B).

## 4. Why the plain switch still emits value order

`MachineBlockPlacement` places a jump table's bodies from `BlockWorkList`,
filled by `markBlockSuccessors` from `MBB->successors()`.  For a TBH block
that list is `buildJumpTable`'s `Table` order — ascending case value.  The
instrumented LLVM 7.0.1 trace of the committed IR shows the high JT block
(`bb104`) pushing its 22 bodies in value order (`bb36`=150, `bb38`=151, …,
`bb71`=189) with `UnscheduledPredecessors == 1` each, so all 22 are pushed
and `selectBestCandidateBlock` returns the first one (all frequencies are
tied at 0.1865889213).  The `getFirstUnplacedBlock` fallback (which *does*
reproduce the original `L` order, see
`status-missioncompleted-single-switch-fallback-w4003.md`) only runs when
`BlockWorkList` is empty, and it never is: worklist size 23 at the JT merge,
shrinking monotonically.

The only source shape that empties it is the two-sequential-switch form,
which gives the group-B bodies (`156,157,160,163..172`) a second unplaced
predecessor (the surviving second dispatch).  That reproduces `L` exactly
with real labels and the original two TBHs, but costs **+72 B**: the cached
`type` stays live in `r11`, pushing `i` out of `fp` into `r8`, `atStation`
out of `r8` into `r9`, and forcing `this` reloads (`ldr r0,[sp,#16]`) in the
low-group bodies.  Measured 1504 B / 47.1 %.

## 5. Remaining irreducible deltas (unchanged from wave 117)

- **body 166** is 96 B vs the original's 100 B: the source reuses the
  `index` local instead of re-querying `getProductionGoodIndex()` for
  `hasEquipment`.  The faithful re-query is 104 B (+8 B), which shifts every
  later body and the loop latch (0x52a), costing ~17 fuzzy points.
- **body 168** is 26 B vs 30 B: ours carries the `(unsigned short)` `uxth`
  (2 B size device) and the `Globals::status` literal pool lands at 0x3c8
  (inside body 189) instead of 0x4c0 (inside body 168).
- **body 189** is 68 B vs 60 B because of that misplaced pool word.
- These three deltas cancel (−4 −4 +8) so the loop latch stays at the
  original 0x522.  Fixing any one of them without the other two moves the
  latch and loses far more than it gains.
- **body 158** has one scheduler slot difference (`ldr r6,[sp,#16]` before
  `mov r8,r4` in the original, after `lsls r0,r0,#31` in ours).

## 6. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6   # RATCHET PASS, byte 4501
cmake --build cmake-build-match --target lint  -j6   # CLEAN
cmake --build cmake-build-match --target drift -j6   # 0 fields
```

Scratch (not committed): `/dev/shm/w6614/` — `Status.base.cpp`,
`mission_case_table.base.cpp`, `fastscore.py`, `measure.sh`, `measure2.sh`,
`cc1.sh`, `tbh3.py`, `lcs.py`, `showregion.py`, `search_tbl.py`, the ~60
`Status.*.cpp` variants, and the instrumented LLVM 7.0.1 `llc`
(`/tmp/llvm701-build/bin/llc`).
