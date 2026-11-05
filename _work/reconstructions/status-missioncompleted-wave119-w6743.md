# `Status::missionCompleted` — wave 119 (w6743): the original body order is the
# MachineBlockPlacement fallback order (source order), not value order

Status: **research note (no source change landed)**.  Worktree `wt/w6743`,
branch `wave119-w6743`, base master `b5cbf56bd`.  Original binary:
`../_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a, NDK r18b clang
7.0.2 r328903, LLVM `1d739ffb…`, `-Oz`, image base 0x10000).

Target: `_ZN6Status16missionCompletedEbbx` (`Status::missionCompleted(bool,
bool, long long)`), original `0xabf38`, size **1432 B**.  Baseline row:
**92.5 % / 1432 B** (byte False, linked False).  Gate after this pass:
`RATCHET PASS`, `byte_exact 4505`, `linked_exact 4505`, imports 207,
`lint CLEAN`, `drift 0`, 27 sections, exact file size 2,187,328 B.

No 1432 B variant beat the committed wave-117 rank table.  This note records
a new structural fact about the original, the source shapes that reproduce
its exact body layout, and why none of them is landable at 1432 B.

## 0. Baseline

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
# verify: byte 4505  linked 4505  stubs 0  missing 0  extra 0  avg 99.93
# data:   exact 843 of 843 exported objects
# sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
# RATCHET PASS
```

`report.json` row: `_ZN6Status16missionCompletedEbbx` → `pct 92.5`,
`osize 1432`, `usize 1432`, `byte False`, `linked False`.

## 1. The original body layout is exactly the source (MBB) order

Decoding the original's two TBHs (main `0xabfe2`, 40 entries, base
`type-150`; second `0xac04e`, 7 entries, base `type-8`) plus the type-0
check at `0xac0be` gives the body order by address:

```
167/174, 8, 0/11, 13, 14, 150,151,152,153,154,155,158,162,166,170,184,189,
156,157,160,163,164,165,168,169,171,172, default
```

That is *exactly* the order of the case labels in the source, i.e. the
`MachineBasicBlock` order.  Two independent confirmations:

1. Running the instrumented LLVM 7.0.2 `llc` on a plain single switch whose
   case labels are written in that order with `MBP_FORCE_FALLBACK=1` emits
   the identical layout (`getFirstUnplacedBlock` walks MBB order).
2. A two-sequential-switch source reproduces the same layout *without*
   forcing (see §3), because the second switch's bodies are placed after the
   first switch's bodies.

The committed wave-117 rank table already produces this exact layout (its
rank order is the original body order); the residual 7.5 % is therefore not
a layout mismatch but the rank-table dispatch (`ldr/ldrb/cmp/tbh` vs
`sub/cmp/bhi/tbh`), the permuted TBH offsets, and three body-size deltas
(166 −4, 168 −4, 189 +8) caused by the rank table's extra literal-pool word
at `0xac300`.

## 2. Why a plain single switch cannot reach it

For a jump table, `SelectionDAGBuilder::buildJumpTable` adds the successors
in table (ascending-value) order, and `MachineBlockPlacement`'s worklist then
picks them in that order, so a plain `switch (m->getType())` always lays the
bodies out in ascending type order.  Measured with a single switch whose
case labels are in the original order:

| variant | pct | usize | layout |
|---|---:|---:|---|
| single switch, original label order | 58.4 | 1424 | `167/174, 8, 11, 13, 14, 150..172` (value order), default, `184, 189` |

The fallback (`getFirstUnplacedBlock`) only runs when the worklist is empty,
which needs every case body to have ≥2 unscheduled predecessors; a plain
switch gives every body exactly one (the switch block), so the worklist is
never empty.  `MBP_FORCE_FALLBACK=1` on the same source gives the exact
original layout, which pins the mechanism.

## 3. The two-sequential-switch shape reproduces the exact layout

Writing the original as two switches over a cached `int`:

```c++
int mtype = m->getType();
switch (mtype) { /* 167/174, 8, 0/11, 13, 14, 150..155,158,162,166,170,184,189 */ }
switch (mtype) { /* 156,157,160,163,164,165,168,169,171,172 */ }
```

emits **exactly** the original layout and table shape (40-entry
`150..189` TBH + 7-entry `8..14` TBH + type-0 `cmp`), verified by decoding
the emitted TBHs:

```
layout: 167/174, 8, 11, 13, 14, 150,151,152,153,154,155,158,162,166,170,184,189,
        156,157,160,163,164,165,168,169,171,172, default
tables: (150, 40), (8, 7)
```

But it costs **+72 B** (1504 B): `mtype` is live across the first switch's
bodies, so it takes `fp`, pushing the loop counter `i` to `r8` and
`atStation` to `r9`; the original has `fp = i`, `r8 = atStation`, and no
saved type.  The changed register allocation also loses the tail merges
(e.g. 167/174 no longer shares `cmp; blt; setWon` with body 154), so the
bodies themselves grow.  Measured:

| variant | pct | usize | note |
|---|---:|---:|---|
| two switches, cached `int mtype` | 47.1 | **1504** | exact original layout + table shape |
| two switches, second re-queries `getType()` | 45.0 | 1444 | 25-entry first table; 184/189 become comparisons |
| second switch in first's `default` (cached) | — | 1424 | SimplifyCFG merges → value order |
| second switch in first's `default` (re-query) | — | 1396 | merged |
| second switch in `default` + `if (type>=156)` guard | — | 1400 | merged |
| both switches in `default` | — | 1680 | merged / duplicated tables |

Any size other than 1432 shifts every later function and collapses
`byte_exact` (the wave-119 w6712 note measured 3043 for 1424 B), so none of
these is landable.

## 4. Rank-table body-size deltas (unchanged)

The committed rank table keeps the exact layout but adds one literal-pool
word (`kMissionCase` at `0x20b977`) at `0xac300`, which lands inside body
189 and makes it 68 B instead of 60 B; the `(unsigned short)` cast in body
168 (26 B vs 30 B) and the reused `index` in body 166 (96 B vs 100 B)
cancel the rest.  The faithful body-166 re-query measures 1440 B
(`+8`), not 1432 B, so it cannot be landed without a compensating −8 B that
the pool placement does not offer.

## 5. Conclusion — proven vs open

**Proven (built and measured):**

1. The original body order is the source/MBB order, i.e. the
   `MachineBlockPlacement` fallback order; `MBP_FORCE_FALLBACK=1` on a plain
   switch with that label order emits the exact layout.
2. A plain single switch always emits ascending-value bodies (58.4 % /
   1424 B); the worklist is never empty for a jump-table switch.
3. The two-sequential-switch shape reproduces the exact original layout and
   TBH shape, but at 1504 B (+72 B) because `type` becomes live across the
   first switch and takes a callee-saved register.
4. The committed rank table is still the best 1432 B form: **92.5 %**.

**Open:** a source shape that makes the switch's successors have ≥2
unscheduled predecessors (so `BlockWorkList` is empty and
`getFirstUnplacedBlock` runs) while keeping `type` in `r0`.  That would let
the rank table be deleted, make the dispatch `sub #150; cmp #39; tbh`
byte-exact, restore the original literal-pool placement (body 189 60 B,
body 168 30 B) and make `.rodata` exact.

## 6. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6   # RATCHET PASS, byte 4505
cmake --build cmake-build-match --target lint  -j6   # CLEAN
cmake --build cmake-build-match --target drift -j6   # 0 fields
```

Scratch (not committed): `/dev/shm/w6743/` — `Status.orig.cpp`,
`gen.py`, `gen2.py`, `gen3.py` (shape generators), `cc.sh`, `ccs.sh`,
`llc.sh` (instrumented LLVM 7.0.2 `llc` with `MBP_TRACE` /
`MBP_FORCE_FALLBACK`), `decode.py` (TBH decoder), `layout3.py`
(layout-by-address), `m2.sh` (link + official verify), `fsize.py`
(fast object-size check), `v_origorder2.cpp` (single switch, 58.4 % /
1424 B), `v3_seqc.cpp` (two switches cached, 47.1 % / 1504 B),
`v3_seq.cpp` (two switches re-query, 45.0 % / 1444 B), `v_requery.cpp`
(rank table + body-166 re-query, 1440 B).
