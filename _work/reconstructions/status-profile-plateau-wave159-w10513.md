# Status::missionCompleted — flow-consistent profiles, goto-fold and
# sample-head experiments (wave 159, w10513)

Status: **analysis only, no `src/` / `CMakeLists.txt` change**. Worktree
`wt/w10513`, base master `cfde1f5f2` (wave 158). Scratch `/dev/shm/w10513`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370.

Target: `_ZN6Status16missionCompletedEbbx` (1432 B).
Baseline: **raw 722, aligned 320, size 1432, `.rel.plt` 0**.
Reference plateau (IR injection): **raw 558, aligned 47, size 1432**.

Mission (w10413 follow-up): find a profile or source shape that beats
722/320 at 1432 B.  Read first:
`status-source-plateau-wave158-w10413.md`,
`status-instrumented-1432-relplt0-wave157-w10313.md`,
`status-plateau-source-wave156-w10213.md`, `status-sample-profile-w10020.md`.

## 0. Bottom line

1. **The plateau is still IR-injection-only; no profile or source shape
   tried this wave beats 722/320 at 1432 B.**  The unprofiled body and the
   plateau body differ *only* in the switch `!prof`; every profile route
   (`-fprofile-instr-use`, `-fprofile-sample-use`) also changes the body
   because `hasProfileData()` is set (LoopSink / frequency-driven block
   placement), and every source route (`__builtin_expect`) is one-hot.
2. **NEW: a flow-consistent instrumented vector cannot hit 1432.**
   Setting *every* counter equal to `B` except the case counters
   (`4/3/2`) makes the switch default weight `B - 84`.  Even at `B=85`
   (default = 1, the exact plateau ratio) the body is **1396 B**
   (raw 1309 / aligned 588); `B` in 85..200 gives 1396..1420 B, never 1432.
   The profile-driven body is 36 B shorter than the unprofiled body.
3. **NEW: a goto-fold of the 7 group-1 cases into the switch does fold,
   but the fold weights overflow.**  `if (__builtin_expect(t==167,1)) goto
   L167; ... else switch (t) { ... }` with labels inside the switch folds
   into the mission switch and produces
   `!{!"branch_weights", i32 0, i32 -1672315827, i32 1310669, i32 654,
   i32 0, i32 0, ...}` (only the first 4 arms non-zero).  Body **1400 B**,
   raw **1295 / aligned 815**.  The fold formula multiplies by
   `ValidTotalSuccWeight`, so the gradient is geometric, never flat; the
   size also drifts off 1432.
4. **NEW: a sample profile with `head = 0` is a no-op.**  The sample text
   `func:0:0` gives `function_entry_count = 0 + 1 = 1`?  No: with head 0
   the loader emits no `!prof` at all and the build is byte-identical to
   the baseline (722/320/1432).  With `head >= 1` the body grows to
   1440..1456 B (reproducing w10020).
5. **NEW measurement of the current-source one-hot switch.**  With the
   wave-130 body `__builtin_expect` hints in place,
   `switch (__builtin_expect(m->getType(), 167))` is **1277 / 632 /
   1392 B** for every `-likely/-unlikely` pair tried (4/3, 4/2, 3/2,
   2/1).  The earlier note's 887/349/1428 could *not* be reproduced from
   the current source: removing the body hints and the switch hint gives
   894/340/1428, and removing the hints but keeping the one-hot switch
   gives 1280/633/1388.  The old number came from a body state that no
   longer exists in the tree, which is the "merge lesson" in action.

## 1. Measurements

`raw` = same-address differing bytes; `aligned` = instruction-index
alignment; `size` = function bytes.  Baseline 722 / 320 / 1432.

### 1a. Flow-consistent instrumented profile, all counters = B, cases 4/3/2

`-fprofile-instr-use`, `-mllvm -max-uses-for-sinking=0`,
`-mllvm -profile-guided-section-prefix=false`.

| B | raw | aligned | size |
|---:|---:|---:|---:|
| 1 | 1342 | 730 | 1424 |
| 2 | 1347 | 780 | 1412 |
| 3 | 1335 | 776 | 1412 |
| 4 | 1313 | 602 | 1408 |
| 5 | 1327 | 642 | 1412 |
| 12 | 1316 | 604 | 1404 |
| 28 | 1319 | 661 | 1400 |
| 85 (default = 1) | 1309 | 588 | 1396 |
| 100 | 1315 | 580 | 1400 |
| 200 | 1307 | 622 | 1400 |

Without `-max-uses-for-sinking=0` the same vectors are 1420 B and
raw 1369..1385.  The w10313 `base 1 / loop 3 / c10 5` vector reproduces
**1299 / 583 / 1432** exactly; its TBH body order is
`[default cases first, then 167,174,184,170,162,166,189,155,150,...]`,
i.e. the switch weights are *not* the plateau (the default is hot).

The `prof_instr_w10214.consistent(K)` vector (latch counters 6/7 = 0,
loop/filter counters = K, default counter = K - 84) *does* hit 1432 B for
K = 85..200 with `-max-uses-for-sinking=0`, but the body is still
frequency-driven: **raw 1297 / aligned 748 / 1432 B** at K = 85..90
(K = 100..200: 1301..1303 / 749 / 1432).  Without the sinking flag the
same vectors are 1448 B / raw 1371.  So even the exactly flow-consistent
`default 1, cases 4/3/2` count vector does not reproduce the unprofiled
body.

### 1b. Goto-fold of the group-1 cases into the switch

Source variant: `int mtype = m->getType(); if
(__builtin_expect(mtype == 167, 1)) goto L167; else if ... else switch
(mtype) { case 167: case 174: L167: L174: ... }`, labels on all seven
group-1 cases.

IR switch weights (default 2000/1, and unchanged for 4/2, 3/2, 2/1):
`!{!"branch_weights", i32 0, i32 -1672315827, i32 1310669, i32 654,
i32 0, i32 0, ...}` — geometric with 32-bit overflow; the last 25 arms
are all 0.

| variant | raw | aligned | size |
|---|---:|---:|---:|
| goto-fold, any likely/unlikely pair | 1295 | 815 | 1400 |
| one-hot 167, current body | 1277 | 632 | 1392 |
| one-hot 167, body hints removed | 1280 | 633 | 1388 |
| body hints removed, no switch hint | 894 | 340 | 1428 |

### 1c. Sample profile head sweep (`-max-uses-for-sinking=0`)

| head | vec (a,b,c,d,e) | raw | aligned | size |
|---:|---|---:|---:|---:|
| 0 | any | 722 | 320 | 1432 |
| 1 | 4/4/4/3/2 | 1341 | 572 | 1452 |
| 1 | 3/3/3/2/1 | 1326 | 559 | 1440 |
| 1 | 2/2/2/2/1 | 1326 | 614 | 1448 |

`head = 0` emits no `!prof` at all, so the sample vector is ignored.

## 2. Why

* `LowerExpectIntrinsic::handleSwitchExpect` (LLVM 7.0.2) is one-hot:
  exactly one successor gets `-likely-branch-weight`, every other gets
  `-unlikely-branch-weight`.  Two levels, never the 4>3>2 plateau.
* `SimplifyCFG::FoldValueComparisonIntoPredecessors` scales successor
  cases by the predecessor's *default* weight and the predecessor case by
  `ValidTotalSuccWeight`; with `L=4,U=2` the first arm is ~1000x the
  tail, and with 7 arms the product overflows 32 bits to 0.
* `-fprofile-instr-use` / `-fprofile-sample-use` set
  `function_entry_count`, which makes `hasProfileData()` true and lets
  LoopSink / frequency-driven `MachineBlockPlacement` change the body
  (1396..1456 B).  The IR plateau injection runs after the optimizer and
  leaves the unprofiled 1432 B body intact.
* The original body is the *unprofiled* body; only the switch carries the
  4:3:2 plateau.  That is exactly what the IR injection models and what no
  source/profile route reproduces.

## 3. Open questions

1. The plateau injection is performed on the *post-optimization* IR.  A
   source construct that survives to `LowerExpectIntrinsic` with three
   levels would win immediately; clang 7 has no
   `__builtin_expect_with_probability` and no other `!prof`-producing
   builtin.
2. A sample profile whose `function_entry_count` can be suppressed (or a
   `-mllvm` flag that stops LoopSink/frequency placement from changing
   the body) would leave the 1432 body; in 7.0.2 the count is
   unconditional (`getHeadSamples() + 1`).
3. The 47 aligned diffs of the plateau build are body code differences
   (case 162 `ldr r6,[sp,#16]`, case 166 r9/r5 allocation, the shared
   `setWon` tail, the 168/169 `ble`/`bgt` inversion) and are independent
   of the weights.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10513
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6

# plateau reference
W9920_SCRATCH=/dev/shm/w10513/w9920 \
  python3 _work/reconstructions/pgo_switchweight_w9920.py groups 4 4 4 3 2

# flow-consistent instrumented vector (all counters B, cases 4/3/2)
W9620_SCRATCH=/dev/shm/w10513/w9620 python3 - <<'PY'
import sys, os
sys.path.insert(0, '_work/reconstructions')
import pgo_combine_w9620 as H
meta = H.get_meta()
G1=[167,174,8,0,11,13,14]; G2=[150,151,152,153,154,155,158,162,166,170,184,189]
G3=[156,157,160,163,164,165,168,169,171,172]
v=[85]*meta['ncounters']
for c in G1: v[meta['case_ctr'][c]]=4
for c in G2: v[meta['case_ctr'][c]]=3
for c in G3: v[meta['case_ctr'][c]]=2
r=H.build(meta,v,'flow85',['-mllvm','-max-uses-for-sinking=0'])
print(r['raw'],r['aligned'],r['size'])   # 1309 588 1396
PY
```

Scratch (`/dev/shm/w10513/`): `fdiff.py`, `align_diff.py`, `score_src.py`,
`v_goto.cpp`, `v_onehot.cpp`, `v_noexp.cpp`, `sample_profile.py`,
`orig.asm`, `ours.asm`.

## 5. Files changed

* `_work/reconstructions/status-profile-plateau-wave159-w10513.md` — this note.
