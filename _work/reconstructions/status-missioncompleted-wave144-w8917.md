# Wave 144 (w8917): `Status::missionCompleted` — the group-arm fold is measured
# from source and **cannot** reach the 558 B layout: a nested-switch group arm
# emits an 11:1 split (the inner switch's successor count), and the winning
# family needs a 2:1..6:1 split.  The layout-40 family is re-mapped: it is a
# *two-level* "C cold" split `(A+low+B = 2..6, C = 1)`, not a four-group vector.

Status: **note-only** (no `src/` change landed).  Gate on the unchanged tree:
`byte_exact 4507/4524`, `linked_exact 4507/4524`, size 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0.  Worktree `wt/w8917`, base master `f5093474f`
(wave 143).  Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).

Target: `_ZN6Status16missionCompletedEbbx` — original `0xabf38`, **1432 B**,
baseline raw same-address diff **722 B**.  Metric: raw same-address diff bytes
(`/dev/shm/w8917/fdiff.py`), not the PYTHONHASHSEED-dependent fuzzy %.

## 0. New results

### 0.1 The 558 B target is reproduced, and its layout is `A, low, B, C`
Injecting `branch_weights 1, 3,3, 4x5, 3x12, 2x10` into the current optimised
IR switch and re-linking with the checked-in gold command gives **558 / 1432**
(reproduced).  Decoding both `tbh` tables (40 entries at `0xabfe2`) shows the
558 variant's block *order* is identical to the original's:

```
167/174, [low dispatch], 8, 0/11, 13, 14, 150,151,152,153,154,155,158,162,
166,170,184,189, 156,157,160,163,164,165,168,169,171,172
```

i.e. exactly the source order of the case bodies.  The remaining 558 bytes are
within-block (the w8820 §2 residuals), not layout.

### 0.2 NEW: the layout-40 weight family is a *two-level* split, not four groups
A fast layout scorer (compile one `.ll` to `.o`, decode the `tbh` targets, and
compare the *rank* sequence to the original; `/dev/shm/w8917/fs2.py`) was used
to sweep group weights `(default=1, A, low, B, C)` over `1..6` (1296 vectors).
The winners are a broad family; the load-bearing structure is:

* `(A+low+B, C) = (2, 1)` — **the whole 19-case run A+low+B hot, the whole
  10-case C run cold** — is a winner.  So is `(3,1)`, `(4,1)`, `(5,1)`, `(6,1)`.
* `(2,2,2,1)` (the same split expressed per group) is a winner.
* `(1,4,2,1)` is a winner; `(1,1,1,N)` is **not** for any N (match 0).
* `(11,11,11,1)` is **not** (match 22); `(11,11,11,1)` is what a fold produces
  (see 0.3).  The split ratio must be <= ~6:1.

So the original layout is a "C is cold" two-level split, and any producer must
emit that ratio.  This is a *sharper* statement than w8820 §0.1's 4:3:2:1.

### 0.3 The group-arm fold is blocked by `ValidTotalSuccWeight`
`FoldValueComparisonIntoPredecessors` (LLVM 7.0.1 `SimplifyCFG.cpp:1005`)
scales the predecessor's cases by `ValidTotalSuccWeight`, the *sum of the
successor switch's weights*.  A nested switch whose outer arm is the
`A+low+B` run and whose default is the `C` switch therefore emits
`outer = 1 * (10 + 1) = 11`, `inner = 1`, `default = 1`:

```
switch (__builtin_expect(type, 167)) {       // outer: A+low+B (19 cases)
  case 167: ... case 189: ... break;
  default: switch (type) { case 156: ... case 172: ... }   // inner: C (10 cases)
}
```
Measured on the real TU with `-mllvm -likely-branch-weight=1`:
`!prof = {1, 11 x19, 1 x10}` -> layout rank match **22/40**, size **1384 B**
(48 B too small; the outer jump table is folded away), raw **1264**.
With `-likely-branch-weight=3` it is `{1, 33/11, 11 x18, 1 x10}` -> 1309/1416.

### 0.4 A *single-case* arm fold never reaches layout 40
A chain of `k` `__builtin_expect(type == c, 0)` arms followed by the switch
folds into one switch with weights
`default = L^k`, `arm_j = (L+1)^(k-j+1) L^(j-1)`, `rest = L^k`
(measured with clang 7.0.2: k=4,L=3 -> `{81,192,144,108,81,...}`).  Because
only the k arm cases are hot and every other case is uniform, the layout rank
match is **24/40 for every arm choice** tried (k=1..8; arms 167/8/150/156 in
all 24 group permutations, and k=5/6 chains).  The group-uniformity of the
winning family is essential, and single-case arms cannot produce it.

### 0.5 `||` group arms lose their weights before the fold
`SimplifyBranchOnICmpChain` (`SimplifyCFG.cpp:3644`) converts
`x==a || x==b` into a switch but explicitly drops the branch weights
(`TODO: Preserve branch weight metadata`, line 3682).  For contiguous values
clang instead emits a range check (`icmp ult`) or a bitmask (`(x|k)==c`) on a
*different* SSA value, which `FoldValueComparisonIntoPredecessors` cannot fold
(it requires the same SSA value).  So a `||` group arm is either unfolded or
unweighted.  A `case a ... b:` range is expanded to individual IR cases, so it
is not a single equality comparison either.

## 1. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w8917/fdiff.py _ZN6Status16missionCompletedEbbx   # 722 / 1432

# reproduce 558: inject weights into the real switch IR, recompile, relink
python3 /dev/shm/w8917/inject.py status_base.ll status_inj.ll \
  1,3,3,4,4,4,4,4,3,3,3,3,3,3,3,3,3,3,3,3,2,2,2,2,2,2,2,2,2,2
# exact-TU compile + checked-in gold link: /dev/shm/w8917/cc.sh, linkcmd.sh
```

Scratch (`/dev/shm/w8917/`): `fdiff.py` (raw-diff scorer), `fs2.py` (fast
layout scorer: `.ll` -> `.o`, decode `tbh`, rank-compare), `inject.py`,
`cc.sh` / `cc_ll.sh` / `linkcmd.sh`, `sweepgw.py`, `status_base.ll`,
`vnest.ll` / `vnest2.ll` (nested-switch variants), `Status.base.cpp`.

## 2. Open items

1. The winning split is `(A+low+B hot, C cold)` at ratio 2..6:1.  The only
   source-level producer of a *group* weight is a nested switch, whose ratio
   is fixed at `inner_successor_count + 1` (11 here).  A source shape that
   makes the inner switch's weight sum 2..6 (e.g. a 1..5-case inner switch that
   still covers the whole C run via tail-merging) is the remaining lever.
2. Alternatively the original was built with real PGO; the 558 B ceiling is
   then an injection artefact and the residual 558 B (within-block code) is the
   only source-reachable part.
