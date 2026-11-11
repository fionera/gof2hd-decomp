# Status::missionCompleted — rank-chain / fallthrough / (type,flag) re-sweep (wave 169, w11517)

Status: **analysis only, no `src/` win landed**.  Worktree `wt/w11517`,
branch `wave169-w11517`, base master `00c403d5a`.  Scratch `/dev/shm/w11517`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).

Gate on entry and exit (full `check`): **RATCHET PASS**, byte_exact 4507,
linked_exact 4507, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370.  Target `_ZN6Status16missionCompletedEbbx`
(1432 B) baseline **raw 722 / aligned 320 / 1432 B**, `insn 529/527`.

Read first: `status-432-producers-wave169-w11417.md`,
`status-432-producers-wave168-w11317.md`,
`status-432-switchweights-wave166-w11116.md`,
`status-missioncompleted-order-wave122-w7204.md`.

## 0. Bottom line

The requested producer classes were implemented and measured.  **None is
size-exact (1432 B) and a raw win.**  The new facts:

1. **A hand-written if-chain over a precomputed rank folds back to the
   byte-identical baseline.**  A 27-arm `if/else if` chain assigning ranks
   `0..26` in the original A-then-B order, followed by `switch (rank)`, is
   optimized by clang into the same single `switch (m->getType())` and the
   same ascending body order: **722/320/1432** (identical bytes).  The
   compiler proves the rank permutation and re-sorts the cases.
2. Adding `__builtin_expect(_,1)` to every rank condition stops the fold but
   is **1400 B** (raw 1319).
3. A *non-foldable* rank (`rank = t - 150 + (isB(t) ? 40 : 0)`, with `isB`
   a 10-term `||` chain) keeps the A-then-B rank order but is **1436 B**
   (raw 1336); clang turns `isB` into a bitmask test and the rank map into a
   `.rodata` table.  The +4 B is not recoverable by source tweaks tried.
4. **Duplicated case bodies with different fallthroughs**: splitting
   `case 167: case 174:` into two bodies is still tail-merged (no-op), and
   the full-A fallthrough variants are semantically wrong (an A body's
   failure path executes the next A body).  No size-exact variant.
5. **`switch` over a `(type, flag)` pair** with bodies shared between the
   two flag values is **1560 B** (raw 1447): the key expression adds a
   select and the table doubles; the body order stays ascending.
6. The only shape that reproduces the original **A-then-B body order** is
   still the sequential two-switch (A switch then B switch): body order is
   exact, but it is **1504 B** (raw 1229) with a cached `mtype`, or
   **1472 B** (raw 1249) when the second switch re-loads
   `(*missions)[i]->getType()`.  The +40..72 B is the second dispatch
   (`tbb`) plus body-8/body-11 tail duplication; it is not a dispatch-only
   cost.
7. A `switch` nested in the first switch's `default:` is hoisted/merged by
   SimplifyCFG and is a complete no-op (**722/320/1432**).

## 1. Measurements

`raw` = same-address differing bytes (`tools/fn_diff.py`); `aligned` =
instruction-index alignment; `size` = function bytes.  All shapes keep the
29 case bodies verbatim; only the dispatch wrapper changes.

| shape | raw | aligned | size | body order |
|---|---:|---:|---:|---|
| **baseline `switch (m->getType())`** | **722** | **320** | **1432** | ascending |
| if-chain in A-then-B source order | 722 | 320 | 1432 | ascending (folded) |
| rank chain `0..26` + `switch(rank)` | 722 | 320 | 1432 | ascending (folded) |
| rank chain + `expect(_,1)` on each arm | 1319 | 786 | 1400 | reordered |
| `rank = t-150 + (isB?40:0)` + `switch(rank)` | 1336 | 619 | 1436 | A-then-B |
| `rank = t-150` then `if(isB) rank+=40` (table) | 1316 | 581 | 1444 | A-then-B |
| `switch(t + (atStation?40:0))`, shared bodies | 1447 | 436 | 1560 | ascending |
| nested `default: switch(t)` | 722 | 320 | 1432 | ascending (merged) |
| sequential two-switch, cached `int mtype` | 1229 | 342 | 1504 | **A-then-B exact** |
| sequential two-switch, second `(*missions)[i]->getType()` | 1249 | 251 | 1472 | **A-then-B exact** |
| A switch + B if-chain | 1229 | 342 | 1504 | A-then-B exact (folded to two-switch) |
| A switch + range-`if` + B switch | 1346 | 480 | 1528 | A-then-B-ish |
| split 167/174 bodies | 722 | 320 | 1432 | identical (tail-merged) |

## 2. Why the rank producer folds

The original body order is two ascending runs:

```
A = 150 151 152 153 154 155 158 162 166 170 184 189
B = 156 157 160 163 164 165 168 169 171 172
```

A rank chain in exactly that order is a pure permutation of the raw type.
clang 7 `SimplifyCFG`/`SelectionDAG` recovers the inverse mapping and emits
the same 40-entry value-indexed `tbh` with the same ascending successor
order as the plain switch.  The rank values therefore have to be hidden from
the folder (a memory table, a non-invertible arithmetic expression, or a
`!prof`-carrying chain) before the rank order survives to
`MachineBlockPlacement`; all such shapes tried cost bytes.

## 3. Why the two-switch is still the only exact-order shape

The original `tbh` table is value-indexed but its body addresses are
A-then-B.  With uniform switch-edge weights MBP drains the table's
successors in ascending value order (baseline).  The sequential two-switch
gives every B body a second unplaced predecessor, so the first table's
`markBlockSuccessors()` does not push them; they are pushed only when the
second dispatch is placed, after A.  The second dispatch survives as a `tbb`
(17 entries) and the changed register allocation (`mtype` in `fp`, `i` in
`r8`, `atStation` in `r9`) also duplicates the body-8/body-11 tails, so the
function is +72 B rather than +34 B.  The exact 4/3/2 PGO weight vector
remains the only 1432-B producer (wave 166: raw 558 / aligned 47).

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w11517
ulimit -v 12000000
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN6Status16missionCompletedEbbx
```

Scratch (`/dev/shm/w11517/`): `Status.base.cpp`, `reorder.py`,
`ifchain.py`, `rank.py`, `rank2.py`, `flagpair.py`, `twoswitch.py`,
`twoswitch2.py`, `mix.py`, `mix2.py`, `nest.py`, `bodysize.py`, `tbl.py`,
`orig.dis`, `ours.dis`, `two.dis`, `two2.dis`, `rank2.dis`.

## 5. Files changed

* `_work/reconstructions/status-432-producers-wave169-w11517.md` — this note.
* No `src/`, `CMakeLists.txt`, baseline, allowlist or doc change.
