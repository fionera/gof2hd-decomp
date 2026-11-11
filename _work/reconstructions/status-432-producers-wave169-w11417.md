# Status::missionCompleted — gap-label / split-switch re-sweep (wave 169, w11417)

Status: **analysis only, no `src/` win landed**.  Worktree `wt/w11417`,
branch `wave169-w11417`, base master `ec3ceb07c` (wave 168).  Scratch
`/dev/shm/w11417`.  Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).

Gate on entry and exit: baseline reproduced, target **raw 722 / aligned 320 /
1432 B**, `insn 529/527`.

Read first: `status-432-producers-wave167-w11216.md`,
`status-432-producers-wave168-w11317.md`,
`status-432-switchweights-wave166-w11116.md`,
`build-provenance-round6-branch-weights.md`.

## 0. Bottom line

The three requested producer classes (hand-written if-chain over a precomputed
rank, duplicated case bodies with different fallthroughs, `switch` over a
`(type, flag)` pair) were re-checked.  **None is size-exact (1432 B) and a raw
win.**  New measurements this wave:

1. **Splitting `case 167: case 174:` into two identical bodies is a no-op**
   (`722/320/1432`): BranchFolding tail-merges the two copies back into the
   single weight-2 successor, so the successor vector is unchanged.
2. **The gap-label producer is confirmed to be the only source shape that
   reaches the original `G1, G2, G3, default` body order**, but the extra
   case values change the IR switch (29 -> 41/42 cases), which changes the
   RA and shrinks the function.  A monotone sweep k = 1..12 gap labels
   (each G2 body +1) gives: k<=5 -> 1432 B but only a partial reorder
   (raw 946..1042); k=7 -> 1432 B / raw 1066 (first 8 bodies match);
   k>=8 -> 1368..1372 B.  The exact group order needs k>=12 and is 1372 B.
3. **A split high/low two-switch shape** (`if (t>=150 && t<=189) switch(t)
   {high} else switch(t) {low}`) is 1372 B (raw 1304) even without gap
   labels; adding gap labels is 1380 B (raw 1209).  Splitting the switch
   changes the RA by itself, so it cannot be size-exact.
4. **Using the second-switch gap slots 9/10/12 to boost three G2 bodies**
   (so the first switch gains fewer successors) is 1376 B / raw 1318.
5. Verdict unchanged: the 1432-B plateau is reachable only by post-opt
   `!prof` weights (or a profile); clang 7.0.2's only source-level switch
   weight producer is the two-level `__builtin_expect` (wave 166/round 6).

## 1. Measurements

`raw` = same-address differing bytes (`tools/fn_diff.py`); `aligned` =
instruction-index alignment; `size` = function bytes.  All shapes keep the
29 case bodies verbatim; only the dispatch wrapper changes.

| shape | raw | aligned | size | body order |
|---|---:|---:|---:|---|
| **baseline `switch (m->getType())`** | **722** | **320** | **1432** | ascending after 167/174 |
| split `case 167:` / `case 174:` bodies | 722 | 320 | 1432 | identical (tail-merged) |
| +1 gap label (150+159) | 946 | 379 | 1432 | partial |
| +2 gap labels | 951 | 387 | 1432 | partial |
| +3 gap labels | 1002 | 427 | 1432 | partial |
| +4 gap labels | 1013 | 433 | 1432 | partial |
| +5 gap labels | 1042 | 428 | 1432 | partial |
| +6 gap labels | 1310 | 527 | 1412 | partial |
| +7 gap labels | 1066 | 462 | 1432 | first 8 bodies |
| +8 gap labels | 1301 | 458 | 1368 | reordered |
| +9 gap labels | 1325 | 426 | 1364 | reordered |
| +10 gap labels | 1318 | 403 | 1364 | reordered |
| +11 gap labels | 1324 | 374 | 1372 | reordered |
| +12 gap labels (all G2 weight 2) | 1334 | 363 | 1372 | **G1,G2,G3,default** |
| +13 gap labels (+G1 weight 3) | 1286 | 371 | 1372 | **G1,G2,G3,default** |
| split high/low two switches | 1304 | 453 | 1372 | ascending |
| split high/low + 12 gap labels | 1209 | 376 | 1380 | grouped |
| split high/low + 13 gap labels | 1313 | 366 | 1380 | grouped |
| 9/10/12 + first-switch gaps (13 total) | 1318 | 752 | 1376 | grouped |
| (reference) IR `!prof` injection 4/3/2 | 558 | 47 | 1432 | source order |

## 2. Why the gap-label producer cannot land

The block frequency of a jump-table body is `(#case values mapping to it) /
(#case values + 1)`.  The baseline gives 167/174 weight 2 and every other
body weight 1, hence the ascending layout.  Adding one gap label per G2 body
raises G2 to weight 2 (and G1 to 3 with one more), which is exactly the
weight order that makes `MachineBlockPlacement` emit `G1, G2, G3, default`.
But the extra case values change the IR switch from 29 to 41/42 cases.  That
changes the SelectionDAG block order and therefore Greedy's register
assignment: `this` moves from `r6` to `fp`, `m` from `sl` to `r9`, and the
prologue spills `fp`.  Branch relaxation then shortens ~28 branches, so the
function is 1372 B instead of 1432 B.  The same layout reached by IR `!prof`
injection keeps the 29-case IR switch (same RA, same 1432 B) and measures
558/47.

## 3. Requested classes (re-checked)

* **Duplicated case bodies with different fallthroughs**: splitting the
  shared 167/174 body is tail-merged back to one block (no-op).  Duplicating
  a body under a gap label is exactly the gap-label producer above.
* **`switch` over a `(type, flag)` pair**: measured in wave 167 at
  **1447/436/1560 B** (80-entry jump table, bodies duplicated for both flag
  values); not size-exact.  Not re-run here because the size delta is
  structural (the table doubles).
* **Hand-written if-chain over a precomputed rank**: measured in wave 167 at
  **1455/411/1548 B** (29 equality tests + rank switch; not folded because
  the rank values are non-monotonic) and in wave 168 as `gen9`
  (**1270/652/1428 B**, G2/G3 order exact but low bodies misplaced).  The
  chain cannot be size-exact because it adds compares/extra switch.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w11417
ulimit -v 12000000
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN6Status16missionCompletedEbbx
```

Scratch (`/dev/shm/w11417/`): `Status.base.cpp`, `fn.base.txt`, `try.py`
(write function text + rebuild + fn_diff), `gen_gap.py` / `gen_gap2.py` /
`gen_gap3.py` (gap-label generators), `gen_split.py` (high/low split
generator), `sweep_gap.py` (k=1..12 sweep), `mi_base.txt` (machine
instruction dump around block placement).

## 5. Files changed

* `_work/reconstructions/status-432-producers-wave169-w11417.md` -- this note.
* No `src/`, `CMakeLists.txt`, baseline, allowlist or doc change.
