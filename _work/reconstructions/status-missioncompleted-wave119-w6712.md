# `Status::missionCompleted` — wave 119: no size-neutral win over the rank table (w6712)

Status: **research note (no source change landed)**.  Worktree `wt/w6712`,
branch `wave119-w6712`, base master `47a1396bd`.  Original binary:
`../_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a, NDK r18b clang
7.0.2 r328903, LLVM `1d739ffb…`, `-Oz`, image base 0x10000).

Target: `_ZN6Status16missionCompletedEbbx` (`Status::missionCompleted(bool,
bool, long long)`), original `0xabf38`, size **1432 B**.  Baseline row:
**92.5 % / 1432 B** (byte False, linked False).  Gate after this pass:
`RATCHET PASS`, `byte_exact 4503`, `linked_exact 4503`, imports 207,
`lint CLEAN`, `drift 0`, 27 sections, exact file size 2,187,328 B.

No variant beat the committed wave-117 rank table.  This note records the
wave-119 measurements and two structural facts that were re-confirmed.

## 0. Baseline measurement

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

## 1. The gate forces 1432 B

`byte_exact` is a per-function raw-byte comparison; every function after this
one carries PC-relative `bl` offsets to the PLT, so **any size change of this
function shifts every later function and drops `byte_exact`**.  Measured:

| variant | usize | byte_exact | pct |
|---|---:|---:|---:|
| committed rank table | 1432 | **4503** | **92.5** |
| plain switch (1424 B, table removed) | 1424 | 3043 | 58.4 |
| plain switch + body-166 re-query | 1432 | **4503** | 59.4 |
| rank table + body-166 re-query | 1440 | 2326 | 75.8 |

So the only landable forms are 1432 B.  The rank table is the best of them.

## 2. Variants measured this wave

All built with the project's exact flags (`cmake --build … --target link_so`)
and scored with the official verifier unless marked *compile-only*.

| variant | pct | usize | note |
|---|---:|---:|---|
| **committed wave-117 rank table** | **92.5** | **1432** | baseline |
| plain switch, real type labels, low group first (`g1b`) | 58.4 | 1424 | dispatch is exactly the original's (`sub #150; cmp #39; tbh`, `sub #8; cmp #6; tbh`, `cmp #0`), but the body layout is value order |
| plain switch, L source order (`plainL`) | 58.4 | 1424 | same; wave-90 result reproduced |
| plain switch, L order + body-166 re-query | 59.4 | **1432** | 1432 B but value-order layout |
| rank table + body-166 re-query (`requery`) | 75.8 | 1440 | +8 B shifts every later body |
| two sequential switches on a cached `int` | 47.1 | 1504 | reproduces the original layout, +72 B register pressure |
| rank table + `(unsigned short)` dropped in body 168 | 74.9 | 1432 | pool placement moves |
| 200 random monotone case-label assignments | ≤ 92.5 | 1432 | fastscore never exceeded the committed assignment |

The 200-sample label search used `tools/mission_case_table.cpp`'s freedom
(any strictly increasing label sequence in body order keeps the L layout) and
measured the object-level fuzzy score after each compile.  The committed
assignment `[0,1,2,3,4,5,6,9,10,11,12,13,14,15,16,17,18,19,20,22,23,39]`
(plus `167→21`, `174→25`) was never beaten, consistent with the wave-118
LCS-30 optimality proof.

## 3. Body-158 scheduler slot

The only remaining size-neutral mismatch inside a body is the two-instruction
swap at the end of body 158:

```
orig: and.w r0,r9,r6 ; ldr r6,[sp,#16] ; mov r8,r4 ; lsls r0,r0,#31 ; beq
ours: and.w r0,r9,r6 ; mov r8,r4 ; lsls r0,r0,#31 ; ldr r6,[sp,#16] ; beq
```

Every source rewrite tried produced byte-identical code (fastscore 79.3, same
as baseline) or worse:

| rewrite | fastscore |
|---|---:|
| `if (hasSpecial && hasGood)` | 79.3 |
| `if (hasGood) { if (hasSpecial) … }` | 79.3 |
| `bool both = hasGood && hasSpecial; if (both)` | 79.3 |
| `int j` instead of `unsigned j` | 79.3 |
| declare `j` before `equipment` | 79.3 |
| swap `hasGood`/`hasSpecial` declarations | 78.9 (official 92.1) |
| `if ((*e)[j] != 0) { if (…getType()==0) … }` | 77.7 |
| `unsigned char` flags | 46.7 (1472 B) |

## 4. Body 166 / 168 / 189: the three deltas cancel

| body | original | ours | cause |
|---|---:|---:|---|
| 166 | 100 B | 96 B | source reuses `index` for `hasEquipment`; the faithful re-query is 104 B (+8) |
| 168 | 30 B | 26 B | the original carries a `nop` + a `Globals::status` pool word (0xac3f8); ours has the `(unsigned short)` `uxth` instead |
| 189 | 60 B | 68 B | the rank table's address pool word lands at `0xac300` inside body 189 (original has no pool there) |

`−4 −4 +8 = 0`, so the loop latch stays at the original `0x522`.  The
body-166 re-query is +8 B and cannot be compensated without removing the
rank-table pool word — which is impossible while the table exists.  The
`(unsigned short)` cast is a deliberate size device that keeps the latch at
`0x522`; dropping it keeps 1432 B but moves the pool and drops the score to
74.9 %.

## 5. Fallback re-confirmed unreachable from source

The wave-90/118 proof that the original body order is
`MachineBlockPlacement::getFirstUnplacedBlock` (MBB = source order) was
re-checked.  The NDK 7.0.2 `BlockWorkList` is filled by
`markBlockSuccessors` in `MBB->successors()` order, i.e. `buildJumpTable`'s
value-sorted `Table`, so a plain switch always lays the bodies out in
ascending type order.  The fallback only runs when the worklist is empty,
which needs every case body to have a second unplaced predecessor.  Source
shapes tried this wave (all still value order, verified by decoding the
40-entry TBH):

| shape | result |
|---|---|
| `for` → `while` / `do { } while` | value order (or no table) |
| `for (;;) { if (i >= size) break; }` | value order |
| `do { switch } while (0)` | value order |
| `switch` in a plain block / after a `goto` label | value order |
| `if (m != 0) switch` / `if (type >= 0) switch` / `if (type != -1) switch` | value order |
| `default: break;` at top / bottom / `continue` / with a statement | value order |
| `for (int k = 0; k < 1; k++) switch` | value order |
| cached `int mtype = m->getType(); switch (mtype)` | value order |
| hoisted `const unsigned char *missionCase = MISSION_CASE;` | identical codegen |
| `switch ((int)/(type&0xff)/(type+0)/(type&0x7fffffff))` | identical codegen |

The two-sequential-switch form is the only plain source shape that reproduces
the original body order (group A then group B, each in value order), and it
costs +72 B because the cached `type` stays live across the first switch
(`fp`), pushing `i`→`r8`, `atStation`→`r9` and losing tail merges.

## 6. Conclusion — proven vs open

**Proven (built and measured):**

1. The committed rank table is the best 1432 B form found: 92.5 %.
2. Removing the table (plain switch) gives the exact original dispatch but a
   value-order body layout: 58.4 % / 1424 B, and any size change below/above
   1432 B drops `byte_exact` from 4503 to 3043/2326.
3. A plain-switch form padded back to 1432 B with the faithful body-166
   re-query scores 59.4 % — the table-free form is not competitive.
4. The rank table's TBH is a permutation of the original's; the committed
   monotone label assignment is optimal (200-sample search, LCS 30).
5. The three body-size deltas (166 −4, 168 −4, 189 +8) cancel exactly; the
   rank table's literal-pool word at `0xac300` is what makes body 189 8 B
   long, and it cannot be removed while the table exists.

**Open:** a source shape that makes `BlockWorkList` empty at the jump table
(so `getFirstUnplacedBlock` runs) while keeping the plain-`int` register
allocation and 1432 B.  If found, the function should become byte-exact and
the table can be deleted (`.rodata` would become exact too).

## 7. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6   # RATCHET PASS, byte 4503
cmake --build cmake-build-match --target lint  -j6   # CLEAN
cmake --build cmake-build-match --target drift -j6   # 0 fields
```

Scratch (not committed): `/dev/shm/w6712/` — `Status.orig.cpp` (baseline),
`Status.plainL.cpp`, `Status.plainL_req.cpp`, `Status.requery.cpp`,
`Status.g1b.cpp`, `Status.twosw.cpp`, `Status.t4.cpp`, `cc.sh`, `dis.sh`,
`fastscore.py`, `tbh.py` (TBH decoder), `chk.py`, `genvar.py`, `sw2.py`,
`search.py`, and the instrumented LLVM 7.0.2 `llc`
(`/tmp/llvm_android_702b/build2/bin/llc`, `MBP_TRACE` / `MBP_FORCE_FALLBACK`).
