# Status::missionCompleted — source-level switch-weight sweep (wave 164, w11016)

Status: **analysis only, no `src/` win landed**. Worktree `wt/w11016`,
base master `b3d3a1f68` (wave 163). Scratch `/dev/shm/w11016`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).

Gate on entry and exit (source restored): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370, `.text` 84,652, `.ARM.extab` 725, `.rel.plt` 0.

Target `_ZN6Status16missionCompletedEbbx` (1432 B).  Baseline **raw 722 /
aligned 320 / 1432 B**.  Reference plateau (post-optimisation IR `!prof`
injection, default 1 / G1 4 / G2 3 / G3 2) **raw 558 / aligned 47 / 1432 B**
(reproduced this wave with `pgo_switchweight_w9920.py groups 4 4 4 3 2`).

Read first: `status-432-progression-wave163-w10915.md`,
`build-provenance-round4-w10920.md`, `status-switch-nested2-wave162-w10815.md`,
`pgo-switchweight-wave152-w9920.md`.

## 0. Bottom line

No source-only shape tried this wave is size-exact (1432 B) **and** a raw
win.  The 4/3/2 plateau remains IR-injection-only.  The new measured facts:

1. **The first jump table's body order is fixed by the successor list, not by
   source order.**  The `tbh` for 150..189 is emitted with its successors
   sorted ascending by case value, and `MachineBlockPlacement` breaks the
   equal-probability ties in that list order.  Reordering the `case` blocks
   in the source (group order, ascending, descending, 40 random
   permutations) leaves the emitted body order ascending; only the branch
   relaxation distances change, so the size wobbles (1400/1412/1424/1428/
   1432) and the best size-exact raw is **723 / 329** (baseline is 722/320).
2. **The unprofiled switch is already two tables, exactly like the
   original** (`tbh` 150..189 + `tbh`/`tbb` 8..14).  The difference is only
   the body order inside the first table: ours ascending, the original the
   flat 4/3/2 weight order.  The prologue and the whole dispatch prefix are
   byte-identical (first differing byte is the 7th halfword of the first
   `tbh` table at `0xabff2`, i.e. case 156).
3. **`__builtin_expect` on the switch value is one-hot and changes the
   dispatch structure** (the 8..14 cluster becomes a `tbb` instead of a
   `tbh`, the first table's default body moves to position 2, and the body
   loses ~28 instructions).  Every one-hot value is a raw regression; the
   only 1432-B hit is `K=14` (raw 1295 / aligned 780).
4. **Nested switches with a shared local `t` fold back to a single switch
   with geometric weights** (`ratio = T_inner`), and the folded switch keeps
   the original two-table shape but a different body order.  The best
   size-exact nested cell is still the baseline; the closest nested raw is
   `[167,150,162,None]` at **1298 / 605 / 1376**.
5. **If-chains, `break`→`continue`, `default: __builtin_unreachable()`,
   casts and GNU case ranges are all worse** (see table).  None is size
   exact and a raw win.  A sweep of every contiguous 2-way and 3-way
   partition of 150..189 into plain nested switches (local `t`) merges back
   to the baseline: 722 / 324 / 1432.

## 1. Measurements

`raw` = same-address differing bytes (`tools/fn_diff.py`); `aligned` =
instruction-index alignment; `size` = function bytes.

| shape | raw | aligned | size |
|---|---:|---:|---:|
| **baseline `switch (m->getType())`** | **722** | **320** | **1432** |
| plateau reference (IR `!prof` 4/3/2) | 558 | 47 | 1432 |
| source order = ascending | 722 | 324 | 1432 |
| source order = group3 before group2 | 1173 | 481 | 1412 |
| source order = descending | 1097 | 409 | 1428 |
| best of 40 random permutations | 723 | 329 | 1432 |
| `switch (__builtin_expect(t, 167))` | 1317 | 632 | 1392 |
| `switch (__builtin_expect(t, 174))` | 1317 | 632 | 1392 |
| `switch (__builtin_expect(t, 14))` | 1295 | 780 | 1432 |
| `switch (__builtin_expect(t, 150))` | 1345 | 593 | 1404 |
| `switch (__builtin_expect(t, 156))` | 1313 | 679 | 1388 |
| `switch (__builtin_expect(t, 8))` | 1339 | 677 | 1384 |
| `switch (__builtin_expect(t, 0))` | 1327 | 625 | 1424 |
| `switch (__builtin_expect(t, 184))` | 1349 | 591 | 1400 |
| `switch (__builtin_expect(t, 999))` (default hot) | 1315 | 532 | 1400 |
| `switch (__builtin_unpredictable(t))` | 722 | 320 | 1432 |
| `switch ((short) t)` | 1093 | 352 | 1432 |
| `switch ((unsigned char) t)` | 1195 | 345 | 1436 |
| `switch ((long long) t)` | 1439 | 448 | 1500 |
| `break` → `continue` | 1406 | 676 | 1464 |
| `break` → `goto done` (single label) | 722 | 320 | 1432 |
| explicit `default: break;` | 722 | 320 | 1432 |
| `default: __builtin_unreachable()` | 1207 | 384 | 1420 |
| if-chain (`\|\|` groups, `continue`) | 1828 | 844 | 1900 |
| if-chain (single cases, `continue`) | 1828 | 844 | 1900 |
| 3-level nest, no expect, local `t` | 722 | 320 | 1432 |
| 3-level nest `[167,150,156]`, local `t` | 1333 | 496 | 1380 |
| 3-level nest `[167,150,156]`, `getType()` per level | 1334 | 614 | 1420 |
| 4-level nest `[167,150,162,None]`, local `t` | 1298 | 605 | 1376 |
| 4-level nest `[167,150,162,156]`, local `t` | 1320 | 606 | 1376 |
| 2-level nest (G1+G2 outer, G3 inner) | 1338 | 450 | 1392 |
| 2-level nest (8..14 outer, 150..189 inner) | 1343 | 678 | 1420 |
| plain 2/3-level nests, all contiguous cuts (local `t`) | 722 | 324 | 1432 |

## 2. Why source order cannot move the first table

The optimized IR keeps the `SwitchInst` cases in source order, but
`SelectionDAGBuilder`'s jump-table lowering emits the `tbh` successor list
sorted by case value.  `MachineBlockPlacement::getBestSuccessor` picks the
highest-probability successor and, on ties, keeps the first one in
`MBB->successors()`.  With no profile every case has the same weight
(4.10%; 167/174 share a body so 8.19%), the default is 1.71%, and the chain
therefore walks the table ascending.  Reordering the source only changes the
IR block list, which MBP does not consult for the switch chain.

The second table (8..14) is placed `8, 0, 11, 13, 14` -- the `tbh`/`tbb`
fall-through is 8, the `bhi` default target is 0, then the remaining cases
ascending -- and is likewise independent of source order.

## 3. Why the flat 4/3/2 vector is not source-reachable

* `LowerExpectIntrinsic::handleSwitchExpect` is one-hot: one case gets
  `-likely-branch-weight` (2000), every other case and the default get
  `-unlikely-branch-weight` (1).  It cannot give a whole group weight 4/3/2.
* `SimplifyCFG::FoldValueComparisonIntoPredecessors` on nested switches
  multiplies: `outer_case *= T_inner`, `inner_case *= outer_default`.
  With plain inner switches `T_inner = n_inner + 1`, so the merged vector is
  geometric with ratio `n_inner + 1` (11..13 for the real group sizes).
  With `__builtin_expect` at every level the ratio is ~2000.  Neither is the
  flat 4 > 3 > 2 plateau.
* The 1432-B cell needs the last three groups distinct and nearly flat
  (`4/3/2`, `5/4/3`, `8/6/4`, `17/13/9`, `100/99/98`); an integer source
  producer cannot make a ratio below 2.
* The one-hot and nested shapes also change the dispatch/table structure
  (one-hot merges the two clusters; nesting adds a third table), which
  shifts every body and every branch/literal-pool offset, so even the
  correct group order scores raw > 1290.

## 4. What would land it

Only post-optimisation `!prof` weights (or a real sample/instrumented
profile) reproduce the plateau.  The w10920 result stands: a sample profile
cannot weight the empty-`br` successors (cases 0/11/13) and an instrumented
profile moves the non-switch layout.  A source-only landing would need a
clang-level knob that lets the source set per-case weights (e.g. C++20
`[[likely]]` on cases, not available in C++11/clang 7) or a build flag such
as `-mllvm -likely-branch-weight=4 -mllvm -unlikely-branch-weight=3` on a
one-hot expect -- a CMake change, not a `src/` change.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w11016
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j8

# plateau reference: raw 558 / aligned 47 / size 1432
W9920_SCRATCH=/dev/shm/w11016/ref \
  python3 _work/reconstructions/pgo_switchweight_w9920.py groups 4 4 4 3 2

# baseline
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN6Status16missionCompletedEbbx
```

Scratch (`/dev/shm/w11016/`): `exp/blocks.py` (case-block parser/reorderer),
`exp/measure.sh`, `exp/reset.sh`, `exp/try.py`, `exp/sweep.py`,
`exp/nest.py`, `exp/ifchain.py`, `exp/textvar.py`, `exp/order.py`,
`exp/order2.py`, `exp/bodies2.py`, `ref/ideal.so`, `Status.orig.cpp`.

## 6. Files changed

* `_work/reconstructions/status-432-switchweights-wave164-w11016.md` -- this note.
* No `src/`, `CMakeLists.txt`, baseline, allowlist or doc change.
