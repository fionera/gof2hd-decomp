# Status::missionCompleted — human-shape re-sweep for the 4/3/2 switch weights (wave 173, w11819)

Status: **analysis only, no `src/` win landed**.  Worktree `wt/w11819`,
branch `wave173-w11819`, base master `3fafa5b03`.  Scratch
`/home/debian/GalaxyOnFire2/scratch/w11819`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).

Target `_ZN6Status16missionCompletedEbbx` (1432 B).  Baseline **raw 722 /
aligned 320 / 1432 B**, `insn 529/527`.  Gate on entry: clean tree at
`3fafa5b03` (byte_exact 4507, size 4524/4524, data 843/843, imports 207).

Read first: `status-432-producers-wave172-w11717.md`,
`status-missioncompleted-wave143-w8820.md`,
`status-missioncompleted-wave134-w7761.md`,
`status-switch-nested-wave161-w10715.md`.

## 0. Bottom line

1. **Reproduced the exact 4:3:2:1 vector and its 558-B plateau.**  Injecting
   `!prof = [1, 3,3, 4,4,4,4,4, 3 x12, 2 x10]` (metadata order: default,
   167,174, 8,0,11,13,14, 150..189, 156..172) into the current optimised
   `Status.ll` switch and relinking gives **raw 558 / aligned 47 / 1432 B,
   insn 527/527**.  This is the same cell as wave-134/143/172.
2. **The plateau is a broad family, not one vector.**  Grouped vectors
   `{d, p, low, A, B}` with `low >= A > B` and mild ratios all give
   `558/47/1432`: `{1,2,5,4,3}`, `{1,3,4,3,2}`, `{2,2,4,3,2}`,
   `{1,100,99,98}` (flat).  The exact original weights are therefore not
   pinned by the 558 cell.
3. **No source-only shape tried this wave is both size-exact (1432 B) and a
   raw win.**  The new measurements are in §2.  The closest source shapes
   are the nested if/switch families (1364-1392 B) and the
   `-mllvm -likely-branch-weight=3` four-arm chain (1412-1500 B); none is
   1432 B.
4. **The fold family is real but needs group arms that the fold cannot
   express.**  A chain of `if (__builtin_expect(t == c, 0))` arms followed
   by a `switch` emits the wave-143 non-geometric weights
   (`default = L^k`, `arm_j = (L+1)^(k-j+1) L^(j-1)`, switch cases `L^k`);
   with `L=3, k=4` that is `81,64,48,36,27`, inside the 558 family.  But the
   arms are *single values*, so only single-case groups become uniform.
   The four real groups have 2/5/12/10 cases; a group-id helper adds a
   second dispatch table (+68 B -> 1500 B), and single-case arms give
   1412-1416 B with a scrambled layout.

## 1. Measurements

`raw` = same-address differing bytes (`tools/fn_diff.py`); `aligned` =
instruction-index alignment; `size` = function bytes.  All source shapes
keep the 29 case bodies verbatim; only the dispatch wrapper changes.

| shape | raw | aligned | size | body order |
|---|---:|---:|---:|---|
| **baseline `switch (m->getType())`** | **722** | **320** | **1432** | ascending |
| IR `!prof` `[1,3,3,4,4,4,4,4,3x12,2x10]` | **558** | **47** | **1432** | original |
| IR `!prof` `{d1,p2,low5,A4,B3}` | 558 | 47 | 1432 | original |
| IR `!prof` `{d1,p100,low100,A99,B98}` (flat) | 558 | 47 | 1432 | original |
| IR `!prof` `{d1,p1,low5,A4,B3}` | 1303 | 348 | 1372 | original |
| IR `!prof` `{d1,p11,low143,A11,B1}` (fold ratios) | 1314 | 515 | 1416 | original |
| nested if/switch, outer expect(167), high->low | 1239 | 742 | 1372 | ascending |
| nested if/switch, outer expect(167), P+low->A->B | 1327 | 499 | 1384 | 167/174, default, A, B |
| nested if/switch, outer expect(167), P+A+B->low | 1239 | 742 | 1372 | ascending |
| nested if/switch, outer expect(167), P+low+A->B | 1300 | 503 | 1392 | 167/174, default, A, B |
| nested if/switch, outer expect(167), P+low+B->A | 1324 | 767 | 1380 | scrambled |
| `switch(expect(t,167))`, default L=2000 | 1317 | 632 | 1392 | 167/174, default, ascending |
| `switch(expect(t,167))` + L=4/U=3 | 891 | 349 | 1428 | 167/174, default, ascending |
| 4-arm `if(expect(t==c,0))` chain, L=3 | 1332 | 548 | 1412 | scrambled |
| 4-arm `if(expect(t==c,0))` chain, L=4 | 1349 | 543 | 1416 | scrambled |
| 5-arm chain, L=3 | 1305 | 583 | 1416 | scrambled |
| 6-arm chain, L=3 | 1301 | 565 | 1416 | scrambled |
| full 29-arm `if(expect(_,0))` chain | 1313 | 503 | 1424 | 167/174, default, ascending |
| full 29-arm `if(expect(_,1))` chain | 1321 | 657 | 1348 | 167/174, default, ascending |
| group-id helper + 4 group arms, L=3 | 1260 | 486 | 1500 | A, 167/174, B |
| master + L=1..6 (5 in-body expects) | 884-896 | — | 1428 | — |
| master + L>=7 | 722 | 320 | 1432 | ascending |
| `switch(expect(t,C))` L=8, C in {167,174,170,184,168} | 1008-1187 | 339-398 | 1432 | C, default, ascending |
| master, no in-body expects | 898 | 340 | 1428 | — |
| master, +5 extra in-body expects | 1351 | 702 | 1476 | — |

## 2. New source shapes tried

All are real `Status.cpp` variants (scratch copies, same flags, real gold
link via the extracted `link_so` command).

* **Nested if/switch partitions.**  A `switch (__builtin_expect(t, 167))`
  whose `default:` is a second `switch` over one of the groups.  Every
  partition of `{P, low, A, B}` was tried (main = P+low+A / nested = B,
  main = P+low+B / nested = A, main = P+A+B / nested = low, main = P+low /
  nested = A+B, ...).  Sizes 1364-1392 B; the one-hot hint leaves the
  nested default at weight 1, tied with the ordinary cases, so
  `MachineBlockPlacement` places the default block second (wave-161's
  -40 B), not last.
* **`__builtin_expect` grouping.**  `switch (__builtin_expect(t, C))`
  (one-hot) and per-file `-mllvm -likely-branch-weight=L -mllvm
  -unlikely-branch-weight=U` for L,U in 1..2000.  Sizes 1364-1444 B;
  `L>=7` collapses to the 722/1432 baseline.
* **Four-arm fold chain.**  `if (__builtin_expect(t == c, 0))` arms for
  the first case of each group, followed by a `switch` for the rest, with
  `-mllvm -likely-branch-weight=3/4`.  The fold emits group-uniform
  weights for the *switch* cases and geometric weights for the arms, but
  the arms are single values, so the real groups are not uniform.  Sizes
  1412-1416 B.
* **Group-id helper.**  A `missionGroup(int)` switch mapping `t` to
  0/1/2/3, then four `if (__builtin_expect(g == k, 0))` arms, each a
  `switch (t)` over its group.  Clang folds the two switches back into one
  `switch (t)` but *drops* the `!prof` weights
  (`SimplifyEqualityComparisonWithOnlyPredecessor` only preserves them
  when the two comparisons are on the same SSA value).  Size 1500 B.
* **Duplicated tails.**  Splitting the shared 167/174 and 0/11 bodies
  tail-merges back to the baseline (722/320/1432), as wave-170 measured.
* **Body-level expects.**  Removing the 5 existing in-body
  `__builtin_expect`s: 898/340/1428.  Adding expects to the five A-case
  count tests: 1351/702/1476.

## 3. The weight plateau is broad; the original weights are not pinned

A grid over grouped vectors `{d,p,low,A,B}` (d=1, p,low,A,B in 1..8 with
`low >= A >= B`) was scored on the real IR + gold link.  The minimum raw is
**558** at several vectors (`{1,2,5,4,3}`, `{1,3,4,3,2}`, `{2,2,4,3,2}`,
...).  A TBH-offset search over the same grid found **no vector whose 40
high-table entries match the original exactly** (minimum 12 mismatched
entries).  So the original `!prof` weights are not a grouped vector of this
form; the 558-B plateau is the best a grouped vector can do, and the
remaining bytes are body-layout differences (the original's 158 body is
6 B larger, its 166 body 10 B smaller, etc.).

## 4. Why no source producer

The only source-level `!prof` producers on a `switch` in clang 7 are:

* `__builtin_expect(value, C)` on the switch condition:
  `LowerExpectIntrinsic::handleSwitchExpect` is one-hot
  (`L` at one case, `U` everywhere else, including the default).
* `SimplifyCFG::FoldValueComparisonIntoPredecessors` on an
  `if (__builtin_expect(t == c, 0))` chain: geometric weights
  (`default = L^k`, `arm_j = (L+1)^(k-j+1) L^(j-1)`, switch cases `L^k`).

Neither can produce a *group-uniform* vector for the real 2/5/12/10-case
groups without a second dispatch on a group id, and the group-id merge
drops the weights.  The observed 4:3:2:1 ratios are consistent with a
PGO/profile build (waves 124/125/126/134 reached the same conclusion).

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w11819
ulimit -v 12000000
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN6Status16missionCompletedEbbx
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w11819/`: `Status_base.cpp`,
`cc.sh` / `cc_l.sh` / `cc_ll.sh` (exact TU compile, optional `-mllvm`
flags), `fastlink.py` (extracted gold+strip link), `probe.py` (TBH body
order + size), `inject4.py` (full `!prof` vector injection),
`mkifchain.py` / `mkchain2.py` / `mkchain3.py` (if-chains),
`mknest.py` / `mknest2.py` / `mknest3.py` / `mkpart.py` (nested switches),
`mkarm.py` (fold arms), `mkgroup.py` (group-id helper), `search.py` /
`vecsearch.py` (weight searches), `base.ll`.

## 6. Files changed

* `_work/reconstructions/status-432-producers-wave173-w11819.md` — this note.
* No `src/`, `CMakeLists.txt`, baseline, allowlist or doc change.
