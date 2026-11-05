# `Status::missionCompleted` — wave 119 (w6765): the original big-table layout is
# frequency-ordered, not source-ordered; the rank table is still the best 1432 B form

Status: **research note (no source change landed)**.  Worktree `wt/w6765`,
branch `wave119-w6765`, base master `c8a402ec2`.  Original binary:
`../_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a, NDK r18b clang
7.0.2 r328903, `-Oz`, image base 0x10000).

Target: `_ZN6Status16missionCompletedEbbx` (`Status::missionCompleted(bool,
bool, long long)`), original `0xabf38`, size **1432 B**.  Baseline row:
**92.5 % / 1432 B** (byte False, linked False).  Gate after this pass:
`RATCHET PASS`, `byte_exact 4505`, `linked_exact 4505`, imports 207,
`lint CLEAN`, `drift 0`, 27 sections, exact file size 2,187,328 B.

This note adds one new structural fact (the original's big-table body layout
is **block-frequency ordered**, not source/MBB ordered) plus the measurements
that follow from it.  No 1432 B variant beat the committed wave-117 rank
table.

## 0. Baseline

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

`report.json` row: `_ZN6Status16missionCompletedEbbx` → `pct 92.5`,
`osize 1432`, `usize 1432`, `byte False`, `linked False`.

## 1. The original's big-table bodies split into two ascending runs

Decoding the original's 40-entry TBH (`0xabfe6`, base `type-150`) gives the
body order by address:

```
167/174, 8, 0/11, 13, 14,
150,151,152,153,154,155,158,162,166,170,184,189,
156,157,160,163,164,165,168,169,171,172, default
```

The big group is **two ascending runs**, not one:

* A = `150,151,152,153,154,155,158,162,166,170,184,189` (ascending)
* B = `156,157,160,163,164,165,168,169,171,172` (ascending)

A single jump table's successors are emitted by
`SelectionDAGBuilder::buildJumpTable` in `Table` order, i.e. **value order**
(`sortAndRangeify` sorts `Clusters` by value first).  So the placement order
`A ++ B` cannot be the successor order; it is the order
`MachineBlockPlacement::selectBestCandidateBlock` picks candidates by
**block frequency** — highest frequency first, ties broken by worklist order
(= value order).

Two independent confirmations:

1. Our plain single switch (real type labels, any source order) lays the big
   bodies out in full value order:
   `150,151,152,153,154,155,156,157,158,160,162,163,164,165,166,168,169,170,171,172,default,184,189`
   (1424 B).  Writing the case labels in ascending order, in the original
   order, or fully reversed all give this same layout — the source/MBB order
   is irrelevant to the placement.
2. Instrumenting the NDK LLVM 7.0.2 `llc` (`MBP_TRACE=1`) shows the jump-table
   block's successors all have `FQ=256` except the two-case body
   `167/174` (`FQ=512`), which is why it is placed first; the rest are then
   picked in worklist (value) order.

## 2. Frequency experiment: A ++ B is exactly what a frequency split produces

To test the mechanism, duplicate case labels were attached to the A bodies so
each A body owns two case values (2× frequency), and two extra labels were
attached to `167/174` (4×).  The resulting object layout is

```
167/174 (4x), 8, A (2x, ascending), 11, 13, 14, B (1x, ascending), default
```

i.e. the 2× group is placed before the 1× group, and within a group the order
is value order.  This is the same shape as the original's `A ++ B` split.
So the original's A bodies must have a **higher block frequency** than its B
bodies in whatever source shape the original used; a plain switch cannot
express that (all one-case bodies get equal `JTProbs`).

Measured (compile-only, object size + decoded TBH layout):

| variant | usize | layout |
|---|---:|---|
| plain switch (any case order) | 1424 | full value order |
| plain switch + A bodies 2× / `167/174` 4× | 1368 | `167/174, 8, A, 11,13,14, B, default` |
| plain switch + A bodies 2× only | 1368 | `150, 8, 151,152,153,154,155,158,162,166,167/174,170,184,189, 11,13,14, B, default` |

## 3. The two-switch form still reproduces the layout exactly, at +72 B

`switch (mtype) { A } switch (mtype) { B }` with a cached `int mtype`
reproduces the original layout **exactly** (verified by decoding the emitted
TBHs: `167/174, 8, 11, 13, 14, A, B, default`), but costs **1504 B** (+72).
Per-body deltas vs the original (offset of each body start):

| body | original | two-switch | delta |
|---|---:|---:|---:|
| 167/174 | 0xfe | 0xfe | 0 |
| 8 | 0x128 | 0x12e | +6 |
| 11 | 0x18c | 0x1a2 | +22 |
| 13 | 0x196 | 0x1c4 | +46 |
| 14 | 0x1a4 | 0x1d2 | +46 |
| 150 | 0x1d8 | 0x206 | +46 |
| 152 | 0x1e4 | 0x212 | +46 |
| 155 | 0x236 | 0x262 | +44 |
| 158 | 0x248 | 0x274 | +44 |
| 166 | 0x30a | 0x332 | +40 |
| 170 | 0x36e | 0x392 | +36 |
| 189 | 0x3b6 | 0x3da | +36 |
| 156 | 0x3f2 | 0x434 | +66 |
| 160 | 0x450 | 0x498 | +72 |
| 172 | 0x4e2 | 0x526 | +68 |
| default | 0x522 | 0x566 | +68 |

(`usize` 1504 = 1432 + 72; the extra 4 B over the final +68 delta is the
third jump table's dispatch.)  The extra bytes are register-pressure and lost
tail merges: `mtype` is live across the first switch, so it takes `fp`,
pushing `i` to `r8` and `atStation` to `r9`; the original has `fp = i`,
`r8 = atStation`, no saved type.  Body 11 (`atStation && station->getIndex()
== target`) grows from 10 B to 34 B because it no longer shares the
`0xac404` tail with body 19.

Every other two-switch arrangement measured this wave:

| variant | usize | layout |
|---|---:|---|
| two switches, cached `int` | 1504 | exact original |
| two switches, second re-queries `getType()` | 1444 | `189, 167/174, 8, 11, 13, 14, 150..170, B` |
| first switch A-only, second B-only (re-query) | 1444 | same as above |
| A switch then `if (156..172) switch` | 1468 | same as above |
| A switch then B switch, no default (merged) | 1444 | same as above |
| A switch then B switch, `default: break` (merged) | 1444 | same as above |
| single switch, `default:` in the middle | 1424 | full value order |

## 4. Why the rank table's TBH can never match the original's

The committed rank table gives the exact body layout, but its TBH is a
permutation:

* The dispatch is the same 12 B as the original's
  (`ldr r1,[pc,#804]` 2 B + `ldrb r0,[r0,r1]` 2 B replaces
  `sub.w r1,r0,#150` 4 B, then `cmp r0,#39; bhi; tbh`), so the `tbh` sits at
  the same address `0xabfe2` and the table base is identical.  Matching
  slots therefore match exactly (`0x95,0x98,0x9b,0xb5,0xb8,0xc4` for
  150..155, `0xcd` for 158, `0x12e` for 166, `0x28` for 167/174, `0x160` for
  170, `0x23a` for 173, …).
* A TBH slot `k` matches the original only if `rank(150+k) == k`.  Because
  the layout order *is* the ascending rank order (MBP sorts the table by
  value), that requires the body order to be value order — which the original
  is not.  The committed assignment reaches **30/40 LCS, 18 positional
  matches, difflib ratio 0.75**; a 20 000-sample random search over all valid
  (monotone-in-body-order) rank assignments found nothing better, confirming
  the wave-118 LCS-30 optimality proof.

So the rank table's 92.5 % is a hard ceiling for that approach; the remaining
7.5 % is the permuted TBH (40 `.short` lines), the `cmp r0,#200` case-0
compare (the original compares the type to 0) and the body-level
register-allocation deltas already documented in
`status-missioncompleted-wave119-w6712.md` (bodies 166 −4, 168 −4, 189 +8,
body 158 scheduler slot).

## 5. Conclusion — proven vs open

**Proven (built and measured):**

1. The original's big-table body layout is two ascending runs
   `A = {150..155,158,162,166,170,184,189}` then
   `B = {156,157,160,163,164,165,168,169,171,172}`, not value order.
2. A plain single switch always emits full value order regardless of the
   source case order (ascending / original / reversed all measured).
3. That `A ++ B` shape is exactly what `selectBestCandidateBlock`'s
   frequency ordering produces: giving the A bodies 2× frequency reproduces
   it (measured).
4. The two-switch form reproduces the exact layout but is 1504 B (+72), and
   every cheaper two-switch arrangement (1444/1468) scrambles the layout.
5. The rank table's TBH can never match: the rank order must equal the body
   order, so a slot `k` matches only if `rank(150+k) == k`; the committed
   assignment is optimal at LCS 30/40 (18 positional matches, ratio 0.75).

**Open:** a source shape that gives the A bodies a higher block frequency
than the B bodies *inside one jump table* (so the table stays a single
40-entry `sub #150; cmp #39; tbh`), or a shape that empties
`BlockWorkList` at the jump-table block so
`MachineBlockPlacement::getFirstUnplacedBlock` walks the MBB order.  Either
would let the rank table be deleted and make `.rodata` exact.

## 6. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6   # RATCHET PASS, byte 4505
cmake --build cmake-build-match --target lint  -j6   # CLEAN
cmake --build cmake-build-match --target drift -j6   # 0 fields
```

Scratch (not committed): `/dev/shm/w6765/` — `Status.plain.cpp` (plain
switch, real types, original case order), `Status.asc2.cpp` / `Status.rev.cpp`
(ascending / reversed case order), `Status.freq.cpp` / `Status.freq2.cpp`
(frequency experiment), `Status.two2.cpp` / `Status.two3.cpp` /
`Status.ab.cpp` (two-switch forms), `v_s1..v_s8.cpp`, `v_t1..v_t9.cpp`
(structural shapes), `reorder.py` + `genvar.py` + `gentwo.py` + `batch*.py`
(case-order generators), `layout3.py` (TBH decoder), `cc.sh` (project-flag
compile), `m2.sh` (link + official verify), `show_now.txt` (side-by-side
disassembly), and the instrumented LLVM 7.0.2 `llc`
(`/tmp/llvm_android_702b/build2/bin/llc`, `MBP_TRACE` / `MBP_FORCE_FALLBACK`).
