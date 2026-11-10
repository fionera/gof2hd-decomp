# Status::missionCompleted — source producers for the 4/3/2 switch weights (wave 166, w11116)

Status: **analysis only, no `src/` win landed**. Worktree `wt/w11116`,
base master `85c1f88df` (wave 165). Scratch `/dev/shm/w11116`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370.

Target `_ZN6Status16missionCompletedEbbx` (1432 B).  Baseline **raw 722 /
aligned 320 / 1432 B**.  Plateau reference (IR `!prof` injection, default 1 /
G1 4 / G2 3 / G3 2) **raw 558 / aligned 47 / 1432 B** (reproduced this wave).

Read first: `status-432-switchweights-wave164-w11016.md`,
`status-432-progression-wave163-w10915.md`,
`build-provenance-round4-w10920.md`.

## 0. Bottom line

No source-only shape tried this wave is size-exact (1432 B) **and** a raw win.
The new measured facts:

1. **The 1432-B weight cell is narrower than "any strictly decreasing
   vector".**  Injection sweep (default 1): `4/3/2` -> 558/47/1432;
   `4/2/1` (ratio 2) -> **1186/200/1444**; `6/3/2` -> 1067/191/1444;
   `2/1/1` -> 1315/464/1412; `130/10/2` -> 1332/473/1412;
   `100/10/2` -> 1344/461/1404.  The cell needs **mild ratios**
   (`G1/G2 <= ~1.33`, `G2/G3 <= ~1.5`), not just a strict decrease.
   A nested switch can only produce an integer ratio `n_inner + 1 >= 2`,
   so it cannot enter the cell (this sharpens the wave-164 conclusion).
2. **The `default` successor sits in the middle of the jump-table successor
   list** (its first gap slot, e.g. case 159), so when G3 ties with the
   default the default is laid out *inside* G3.  Every source shape that
   leaves G3 == default (all plain nested switches) therefore puts the
   default between G2 and G3, not last.
3. **A chain of single-comparison `if`s folds into one switch and preserves
   `!prof`** (`FoldValueComparisonIntoPredecessors`), but the geometric
   multiplication overflows for a 29-deep chain: the merged `!prof` comes out
   as `i32 0, i32 -1892424267, i32 1200663, i32 592, 0, ...` and the body
   order is scrambled (raw 1328 / size 1420).  A plain (unweighted) chain
   folds to `2^(n-1):...:2:1` (ratio 2) and gives raw 1295 / size 1424.
4. **`__builtin_expect` on the loop condition is layout-neutral** for this
   function (`+1` gives the exact baseline 722/320/1432); `+0` moves the
   default but still leaves it second (1338/612/1416).
5. **The `switch`-over-rank shape is 1472 B** (raw 1246): the rank if-chain
   is folded to a switch and the three group switches are separate tables.
6. **The closest source shapes to the original order are the two-level
   `expect`-guarded nests** (`nestexp2`, raw 1328 / 1380; `nestexp`, raw
   1327 / 1384): they get `G1, default, G2, G3` -- the default is the
   outer switch's default block (the inner dispatch), which has the highest
   block frequency and is placed second.

## 1. Measurements

`raw` = same-address differing bytes (`tools/fn_diff.py`); `aligned` =
instruction-index alignment; `size` = function bytes.  All shapes keep the
29 case bodies verbatim; only the dispatch wrapper changes.

| shape | raw | aligned | size | table-0 body order |
|---|---:|---:|---:|---|
| **baseline `switch (m->getType())`** | **722** | **320** | **1432** | ascending, 167/174 hoisted |
| plateau reference (IR `!prof` 4/3/2) | 558 | 47 | 1432 | G1, G2, G3, default |
| `nest2` (G1 switch, default -> G2 switch) | 722 | 320 | 1432 | identical to baseline |
| `nest3` (G1 -> G2 -> G3 nested switches) | 722 | 320 | 1432 | identical to baseline |
| `nestexp` (outer `switch(expect(t,167))`, default -> G2 -> G3) | 1327 | 499 | 1384 | 167/174, default, G2, G3 |
| `nestexp2` (outer `expect(167)`, inner `expect(150)`, G3) | 1328 | 480 | 1380 | 167/174, default, G2, G3 |
| `exp3` (`if(expect(G3,0))` / `if(expect(G2,0))` / else G1) | 1311 | 809 | 1412 | G3, G2, default, G1 |
| `exp3b` (`if(expect(G1,1))` / `if(expect(G2,1))` / else G3) | 1378 | 557 | 1448 | 167/174, G2, default, G3 |
| `exp3c` (`if(expect(G1,1))` / `if(expect(G2,0))` / else G3) | 1375 | 543 | 1460 | 167/174, G2, default, G3 |
| `exp3e` (`if(expect(G1,1))` / `if(expect(G3,0))` / else G2) | 1328 | 652 | 1360 | 167/174, default, G3, G2 |
| `ifplain` (single-comparison if-chain, source order) | 1295 | 611 | 1424 | 167/174, default, ascending |
| `ifplain` reversed | 1248 | 813 | 1448 | 150, default, 167/174, ascending |
| `ifexp` (every if `__builtin_expect(_,1)`) | 1328 | 806 | 1420 | scrambled (overflow) |
| `iflast` (only the last if `__builtin_expect(_,1)`) | 1291 | 615 | 1420 | 167/174, default, ascending |
| `rank` (3-valued rank switch + per-group switches) | 1246 | 329 | 1472 | G3, G2, ... |
| `switch(expect(t,167))` (one-hot) | 1317 | 632 | 1392 | 167/174, default, ascending |
| `switch(expect(t,150))` | 1345 | 593 | 1404 | 150, 167/174, ascending |
| `switch(expect(t,156))` | 1313 | 679 | 1388 | 156, 167/174, ascending |
| `switch(expect(t,184))` | 1349 | 591 | 1400 | 184, 167/174, ascending |
| loop condition `__builtin_expect(...,1)` | 722 | 320 | 1432 | identical to baseline |
| loop condition `__builtin_expect(...,0)` | 1338 | 612 | 1416 | 167/174, default, ascending |

Injection cell map (groups = default / G1 / G2 / G3):

| vector | raw | aligned | size |
|---|---:|---:|---:|
| **4/3/2** | **558** | **47** | **1432** |
| 4/2/1 | 1186 | 200 | 1444 |
| 6/3/2 | 1067 | 191 | 1444 |
| 2/1/1 | 1315 | 464 | 1412 |
| 130/10/2 | 1332 | 473 | 1412 |
| 100/10/2 | 1344 | 461 | 1404 |
| 4/4/4/3/2 with default 0 | 558 | 47 | 1432 |

## 2. Why the source cannot enter the cell

`FoldValueComparisonIntoPredecessors` merges a nested switch into its
predecessor with `pred_case *= total(succ)` and
`inner_case = pred_default * inner_case`.  With plain switches the merged
vector is `n2*(n3+1)+... : n3+1 : 1 : 1` (ratio `n_inner+1 >= 2`).  The
1432-B cell needs ratios below 2, so no plain nesting can reach it.  The
only per-case weight source is `__builtin_expect` on the switch value, and
`LowerExpectIntrinsic::handleSwitchExpect` is one-hot (2000 / 1 / 1).  A
chain of single-comparison ifs does preserve `!prof`, but its geometric
product overflows for 29 cases; with fewer expects the vector is still
geometric (ratio 2000 or 2).

## 3. What would land it

Only post-optimisation `!prof` weights (or a real sample/instrumented
profile) reproduce the plateau.  A source-only landing would need a
clang-level knob that lets the source set per-case weights (C++20
`[[likely]]` on cases, not available in C++11/clang 7) or a build flag such
as `-mllvm -likely-branch-weight=4 -mllvm -unlikely-branch-weight=3` on a
one-hot expect -- a CMake change, not a `src/` change.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w11116
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j2

# baseline
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN6Status16missionCompletedEbbx
```

Scratch (`/dev/shm/w11116/`): `Status.orig.cpp` (baseline source), `m.sh`
(compile one TU + relink + fn_diff), `mk2.py` (switch-shape generator:
`base`, `nest2`, `nest3`, `nestexp`, `nestexp2`, `exp3*`, `ifplain*`,
`ifexp*`, `rank`, `onehot<K>`), `order.py` (decode the first `tbh` body
order), `inj.py` / `injv.py` (IR `!prof` injection + relink + fn_diff),
`fold.py` (folding simulator), `base.ll`, `cases.json`.

## 5. Files changed

* `_work/reconstructions/status-432-switchweights-wave166-w11116.md` -- this note.
* No `src/`, `CMakeLists.txt`, baseline, allowlist or doc change.
