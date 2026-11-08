# Status::missionCompleted — source-level plateau hunt, round 2 (wave 158, w10413)

Status: **analysis only, no `src/` / `CMakeLists.txt` change**. Worktree
`wt/w10413`, base master `9e4795206` (wave 157). Scratch `/dev/shm/w10413`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370.

Target: `_ZN6Status16missionCompletedEbbx` (1432 B).
Baseline: **raw 722, aligned 320, size 1432, `.rel.plt` 0**.

Mission (w10313 follow-up): find a *source-level* construct that produces the
flat `!prof` plateau `default 1, {167,174,8,0,11,13,14}=4, group2=3,
group3=2` at 1432 B.  The IR-injected plateau is **raw 558 / aligned 47 /
1432 B** (re-confirmed this wave with `pgo_switchweight_w9920.py groups 4 4 4 3 2`).

## 0. Bottom line

1. **No source construct reaches the plateau.**  clang 7.0.2 lowers
   `__builtin_expect` on a `switch` condition through
   `LowerExpectIntrinsic::handleSwitchExpect`, which is **one-hot by
   construction**: `Weights(n+1, UnlikelyBranchWeight); Weights[hot] =
   LikelyBranchWeight`.  So a source `switch` can only carry **two** weight
   levels, while the plateau needs **three** (`4 > 3 > 2`).  `__builtin_expect`
   on a case body's first `if` only weights the intra-body branch, not the
   switch edge, so it cannot add the third level either.
2. **The if-chain fold is the only source producer of graded switch
   weights**, but its weights are a geometric product (with 32-bit overflow),
   not the flat plateau, and the folded switch is smaller (1404 / 1376 /
   1372 B).  `switch (__builtin_expect(t, K))` is one-hot and also changes the
   size (1384..1424 B).
3. **Best source variant at exactly 1432 B**: one-hot at 167 with
   `-mllvm -likely-branch-weight=4 -mllvm -unlikely-branch-weight=2` plus an
   `__builtin_expect(...,1)` on the first `if` of every group-3 case body
   (`v_sw_xx1`): **raw 744 / aligned 323 / 1432 B** — slightly *worse* than
   the 722 / 320 baseline, so no gate win.  No source variant improved both
   metrics at 1432 B.
4. The plateau remains a genuine profile signature; the IR injection is still
   the only way to hit 558 / 47.

## 1. Measurements

`raw` = same-address differing bytes (`fdiff.py`); `aligned` =
instruction-index alignment (`align_diff.py`); `size` = function bytes.
All builds are the exact `Status.cpp` compile + gold link, only the source
variant differs.

| variant | raw | aligned | size | relplt |
|---|---:|---:|---:|---:|
| baseline | **722** | **320** | **1432** | 0 |
| IR plateau injection (reference) | **558** | **47** | **1432** | 0 |
| `switch (__builtin_expect(t,167))` default 2000/1 | 1277 | 632 | 1392 | 0 |
| `switch (__builtin_expect(t,150))` default | 1317 | 593 | 1404 | 0 |
| `switch (__builtin_expect(t,8))` default | 1291 | 677 | 1384 | 3 |
| `switch (__builtin_expect(t,0))` default | 1319 | 625 | 1424 | 5 |
| `switch (__builtin_expect(t,156))` default | 1269 | 679 | 1388 | 0 |
| one-hot 167, `-likely 4 -unlikely 2` | 887 | 349 | 1428 | 0 |
| one-hot 150, `-likely 4 -unlikely 3` | 957 | 356 | 1428 | 0 |
| one-hot 8, `-likely 3 -unlikely 2` | 887 | 349 | 1428 | 0 |
| if-chain (`if (__builtin_expect(t==C,1)) ... else if`) | 1252 | 714 | 1404 | 4 |
| if-chain, single-value arms | 1274 | 820 | 1376 | 4 |
| if-chain, group1=1 group2=1 group3=0 | 1256 | 812 | 1372 | 4 |
| case-body expects, all polarity 1 | 1276 | 643 | 1364 | 6 |
| case-body expects, g1=1 g2=1 g3=0 | 1301 | 672 | 1392 | 7 |
| case-body expects, g1=1 g2=0 g3=0 | 1334 | 686 | 1468 | 7 |
| one-hot 167 + group3 body expect 1, `4/2` | **744** | **323** | **1432** | **0** |
| one-hot 167 + group3 body expect 0, `4/2` | 887 | 354 | 1428 | 0 |
| one-hot 167 + group2 body expect 0, `4/2` | 1349 | 832 | 1444 | 7 |
| one-hot 167 + group1 body expect 0, `4/2` | 1291 | 378 | 1436 | 0 |
| `do { switch } while (0)` (w10213) | 722 | 320 | 1432 | 0 |

## 2. IR branch weights (from `-S -emit-llvm`)

The switch case order in the optimized IR is the source order
`[default, 167,174,8,0,11,13,14,150..189,156..172]`.

* `switch (__builtin_expect(t, 167))`, default flags:
  `!{!"branch_weights", i32 1, i32 2000, i32 1, ...}` — one hot case, two levels.
* same with `-mllvm -likely-branch-weight=4 -mllvm -unlikely-branch-weight=2`:
  `!{!"branch_weights", i32 2, i32 4, i32 2, i32 2, ...}` — still two levels.
* if-chain fold (`if (__builtin_expect(t==167,1)) ... else if ...`, 27 arms):
  `!{!"branch_weights", i32 0, i32 -689350850, i32 -689350850,
  i32 -691152757, i32 600635, i32 600635, i32 600335, i32 300, i32 0, ...}`
  — geometric product with 32-bit overflow; the first 7 arms are hot, the
  remaining 22 are all 0 (tied), so group2/group3 cannot be separated.
* if-chain with single-value arms: the fold produces **two** switches (the
  150..189 and 8..14 tables), first weights
  `!{!"branch_weights", i32 0, i32 -1892424267, i32 1200663, i32 592, i32 0, ...}`.
* The plateau vector is `default 1, {167,174,8,0,11,13,14}=4, group2=3,
  group3=2` — three levels, not reachable from `handleSwitchExpect`.

## 3. Why the plateau is not source-producible

`LowerExpectIntrinsic.cpp` (LLVM 7.0.0) `handleSwitchExpect`:

```cpp
SwitchInst::CaseHandle Case = *SI.findCaseValue(ExpectedValue);
unsigned n = SI.getNumCases();
SmallVector<uint32_t, 16> Weights(n + 1, UnlikelyBranchWeight);
if (Case == *SI.case_default())
  Weights[0] = LikelyBranchWeight;
else
  Weights[Case.getCaseIndex() + 1] = LikelyBranchWeight;
```

Exactly one successor gets `LikelyBranchWeight`; every other successor
(including the default) gets `UnlikelyBranchWeight`.  `__builtin_expect_with_
probability` is not available in clang 7.0.2, and `__builtin_unpredictable`
sets `!unpredictable`, not weights.  The only other `!prof` producers are PGO
(`-fprofile-instr-use` / `-fprofile-sample-use`), which the earlier waves
falsified at 1432 B.

The if-chain fold *does* produce three-or-more levels, but the fold formula
(`SimplifyCFG::FoldValueComparisonIntoPredecessors`) multiplies the branch
weights along the chain, so the levels are geometric (and overflow), not the
flat `4/3/2`, and the folded body is 1372..1404 B, not 1432 B.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10413
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6

python3 /dev/shm/w10413/fdiff.py _ZN6Status16missionCompletedEbbx
python3 /dev/shm/w10413/align_diff.py _ZN6Status16missionCompletedEbbx

# reference: IR plateau injection -> raw 558 / aligned 47 / 1432
W9920_SCRATCH=/dev/shm/w10413/w9920 \
  python3 _work/reconstructions/pgo_switchweight_w9920.py groups 4 4 4 3 2

# source scorer: compile one Status.cpp variant, relink, report raw/aligned/size
python3 /dev/shm/w10413/score_src.py <variant.cpp> [--extra FLAG ...]
```

Scratch (`/dev/shm/w10413/`): `fdiff.py`, `align_diff.py`, `score_src.py`,
`cases.json`, `v_*.cpp` (one-hot, if-chain, case-body-expect and 3-switch
variants), `*.ll`.

## 5. Files changed

* `_work/reconstructions/status-source-plateau-wave158-w10413.md` — this note.
