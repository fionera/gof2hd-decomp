# `Status::missionCompleted` — angle B: removing the rank table (wave 119, w6722)

Status: **research note (no source change landed)**.  Worktree `wt/w6722`,
branch `wave119-w6722`, base master `432167221`.  Original binary:
`../_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a, NDK r18b clang
7.0.2 r328903, `-Oz`).

Target: `_ZN6Status16missionCompletedEbbx` (`Status::missionCompleted(bool,
bool, long long)`), original `0xabf38`, size **1432 B**.  Baseline row:
**92.5 % / 1432 B** (byte False, linked False).  Gate after this pass:
`RATCHET PASS`, `byte_exact 4503`, `linked_exact 4503`, imports 207,
`lint CLEAN`, `drift 0`, 27 sections, exact file size 2,187,328 B.

This pass re-tested the "remove the 192-byte rank table with a plain switch
shape that makes clang 7 emit the original body order" hypothesis.  No
size-neutral variant beat the committed wave-117 rank table.  The note
records the measurements and the two structural facts that were
re-confirmed.

## 0. Baseline

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
# verify: byte 4503  linked 4503  stubs 0  missing 0  extra 0  avg 99.93
# data:   exact 843 of 843 exported objects
# sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
# RATCHET PASS
```

`report.json` row: `_ZN6Status16missionCompletedEbbx` → `pct 92.5`,
`osize 1432`, `usize 1432`, `byte False`, `linked False`.

## 1. Where the 92.5 % goes

The rank table maps `Mission::getType()` to a rank so that the single
switch's value-sorted jump table lays the bodies out in the original
(source) order.  The body layout is then byte-identical to the original
(verified by decoding both TBHs); the residual diffs are:

| region | diff |
|---|---|
| dispatch | `ldr r1,[pc,#804]; ldrb r0,[r0,r1]; cmp r0,#39; bhi; tbh` instead of `sub #150; cmp #39; bhi; tbh` (+1 insn) |
| second dispatch | `sub #208` instead of `sub #8` |
| second default | `cmp r0,#200` instead of `cmp r0,#0` |
| TBH entries | 40 `.short` entries are a permutation of the original's (LCS 29-30, 18 exact positions) |
| body 158 | one scheduler slot swap (`ldr r6,[sp,#16]` after `mov r8,r4; lsls` instead of before) |
| body 166 | 96 B vs 100 B (source reuses `index`; the faithful re-query is +8 B) |
| body 168 | 26 B vs 30 B (`uxth` size device instead of the original's `nop` + `Globals::status` pool word) |
| body 189 | 64 B vs 60 B (the rank table's address pool word lands at `0xac300`, inside body 189) |

The three body deltas cancel (`-4 -4 +4 = 0`), so the loop latch stays at
the original `0x522`.  The literal-pool words themselves compare by symbol
(`pool:pic:got:...`), so they do not count.

## 2. Plain switch (table removed)

`switch (m->getType())` with the cases in the original body order gives the
exact original dispatch (`sub #150; cmp #39; bhi; tbh`, `sub #8; cmp #6;
bhi; tbh`) but a **value-order** body layout: 58.4 % / **1424 B**.

The original layout is `167/174, 8, 11, 13, 14, 150..155, 158, 162, 166,
170, 184, 189, 156, 157, 160, 163, 164, 165, 168, 169, 171, 172`; a single
switch always lays the high-cluster bodies out in ascending type order
(`150,151,...,189`), so the ten group-B bodies (`156,157,160,163,164,165,
168,169,171,172`) land in the wrong place.  Any size change also shifts
every later function and collapses `byte_exact` (the wave-119 note measured
3043 for 1424 B).

## 3. Two sequential switches (the faithful shape)

The original is almost certainly two sequential switches: group A
(`167/174, 8, 0/11, 13, 14, 150..155, 158, 162, 166, 170, 184, 189`) then
group B (`156,157,160,163,164,165,168,169,171,172`).  `ARMConstantIslands`
merges the two high tables into the original 40-entry TBH.  Measured:

| variant | pct | usize | note |
|---|---:|---:|---|
| two-switch, cached `int type` | 47.1 | 1504 | exact original body order and table shape |
| two-switch, second switch re-queries `getType()` | 45.0 | 1444 | first table only 25 entries; 184/189 become comparisons |
| two-switch, `unsigned`/`short`/`volatile` type | 45.0-44.0 | 1444-1536 | same |
| two-switch, filler cases for 175..188 | 45.0 | 1444 | filler cases optimised away; table still 25 entries |
| two-switch, cached type + filler `goto` block | 45.0 | 1444 | same |
| hybrid: plain first switch + rank-table second switch | 44.7 | 1452 | second dispatch `ldr/ldrb/cmp/tbh` |
| hybrid cached | 36.4 | 1512 | `type` live across first switch |

The cached two-switch form is the only plain shape that reproduces the
original body order *and* the 40-entry table, but it costs **+72 B**:
`type` is live across the first switch, takes `fp`, pushes `i` to `r8` and
`atStation` to `r9`, and loses tail merges (`0/11` 34 B vs 10 B, `189`
90 B vs 60 B).  The original has no `mov fp, r0`; its `type` stays in `r0`
along the default path only.

## 4. Nested / default / continue forms (merge in SimplifyCFG)

The original's second dispatch is reached only via the first switch's
default.  A nested `default: switch (type)` should give that CFG, but
`SimplifyCFGOpt::FoldValueComparisonIntoPredecessors` merges the two
switches into one (the inner switch block contains only the switch and the
outer default is a value comparison on the same value).  Measured:

| shape | pct | usize | note |
|---|---:|---:|---|
| nested `default: switch (cached type)` | 58.4 | 1424 | merged to one switch |
| nested `default: switch (m->getType())` | 38.9 | 1396 | merged |
| nested + `if (type >= 0)` / `goto` / `for(k<1)` / `do/while(0)` | 58.4 | 1424 | merged |
| first-switch bodies `continue` (skip second switch) | 24.9 | 1364 | bodies shrink, layout value order |
| nested + `continue` | 24.9 | 1364 | same |
| plain switch + `default: return 0` / `continue` / `;` | 52.1-58.4 | 1412-1424 | value order |

No source shape found that keeps the two switches separate and keeps the
plain-`int` register allocation.

## 5. Rank-assignment search

The committed assignment is
`[0,1,2,3,4,5,6,9,10,11,12,13,14,15,16,17,18,19,20,22,23,39]` for the 22
group-A bodies plus `167→21`, `174→25`, gaps `7,8,24,26..38`.  It keeps the
size at 1432 and the body layout exact.

Re-tested alternatives (all 1432 B unless noted):

| assignment | pct | usize |
|---|---:|---:|
| committed | **92.5** | 1432 |
| gaps at the original default slots `9,11,23,25,26..33,35..38` | 91.9 | 1432 |
| `167→20`, `174→25` (a3) | 85.8 | 1432 |
| `167→21`, `174→24` (a1) | 92.3 | 1432 |
| `167→21`, `174→26` (a5) | 92.3 | 1432 |
| `167→20`, `169→21`, `171→22`, `172→23` (a6) | 92.5 | 1432 |
| packed bodies 0..23, `167/174` at 24/25 | 69.8 | 1408 |
| natural slots (LIS 17), `167/174` at 17/24 | 57.3 | 1428 |
| 200 random monotone assignments | ≤ 72.3 | 1400-1428 |

The committed assignment is a sharp local optimum: most reorderings change
the pool placement/body-189 size and drop the function below 1432 B, which
would collapse `byte_exact`.  The 200-sample search confirms the wave-118
LCS-30 optimality result.

## 6. Body-level variants

All size-neutral (1432 B) body rewrites produced byte-identical code:

| body | rewrite | pct |
|---|---|---:|
| 158 | `hasGood`/`hasSpecial` declaration swap | 92.1 |
| 158 | `int j`, `hasGood & hasSpecial`, `hasSpecial && hasGood` | 92.5 |
| 158 | `if (!hasGood || !hasSpecial) break; return m;` | 92.5 |
| 158 | `bool both = ...; if (both) ...` | 92.5 |
| 158 | nested `if (hasGood) if (hasSpecial) ...` | 92.5 |
| 166 | faithful `hasEquipment(m->getProductionGoodIndex(), 1)` | 75.8 (1440 B) |
| 166 | faithful re-query + drop `(unsigned short)` | 75.8 (1440 B) |
| 168 | drop `(unsigned short)` | 74.9 (1432 B, pool moves) |

The body-158 scheduler swap and the body-166/168/189 size deltas are
determined by the rank table's literal-pool placement, not by the source
shape.

## 7. Conclusion — proven vs open

**Proven (built and measured):**

1. The committed rank table is the best 1432 B form found: **92.5 %**.
2. Removing the table with a plain switch gives the exact original dispatch
   but value-order bodies: **58.4 % / 1424 B**.
3. The faithful two-switch shape reproduces the original body order and
   table shape but costs +72 B (cached `type`) or +12 B (re-query, wrong
   table shape); no size-neutral form found.
4. Nested/default/`continue`/`goto` shapes merge in `SimplifyCFG` and fall
   back to value order.
5. The committed rank assignment is a sharp local optimum (200 random
   monotone assignments ≤ 72.3 %; all 1432 B alternatives ≤ 92.5 %).
6. Body 158/166/168 rewrites do not improve the score at 1432 B.

**Open:** a plain source shape that keeps the two switches separate *and*
keeps `type` out of a callee-saved register (the original's `type` lives in
`r0` along the default path only), so the table can be deleted and `.rodata`
becomes exact.  `SimplifyCFG`'s `FoldValueComparisonIntoPredecessors` is the
pass to defeat.

## 8. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6   # RATCHET PASS, byte 4503
cmake --build cmake-build-match --target lint  -j6   # CLEAN
cmake --build cmake-build-match --target drift -j6   # 0 fields
```

Scratch (not committed): `/tmp/w6722/` — `Status.base.cpp`,
`Status.plain.cpp`, `Status.twosw_cached.cpp`, `Status.twosw_recall.cpp`,
`Status.nested_cached.cpp`, `Status.cont.cpp`, `Status.hybrid.cpp`,
`Status.dense2.cpp`, `Status.best.cpp`, `gen_*.py`, `try_*.py`,
`score.py`, `decode2.py`, `mission_case_table.*.cpp`.
