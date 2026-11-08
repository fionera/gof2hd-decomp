# Status::missionCompleted — a sample profile CAN set arbitrary switch
# weights, but always adds `function_entry_count` (body grows past 1432 B)
# (wave 154, w10020)

Status: **harness + note, no `src/` / `CMakeLists.txt` change**. Worktree
`wt/w10020`, base master `4417db0b4` (wave 153). Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000). Scratch `/dev/shm/w10020`.

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370.

Target: `_ZN6Status16missionCompletedEbbx` (original `0xabf38`, **1432 B**).
Baseline: raw same-address diff **722**, instruction-index aligned diff **320**.

Mission (w9920 follow-up): w9920 showed the original's source-order case
layout is reproduced by a flat `!prof` vector
`default 1, {167,174,8,0,11,13,14}=4, group2=3, group3=2` (raw 558,
aligned 47, size 1432) and concluded a sample profile was the only
remaining non-PGO route, *believing* `-fprofile-sample-use` does not add
`function_entry_count`.  This wave tried that route.

## 0. Bottom line

1. **NEW: the sample-profile text format uses line *offsets from the
   function's start line*, not absolute line numbers.**  The w9920 harness
   guessed absolute lines and got `0 of N records applied`.  With offsets
   (`line - 1314`) the profile matches and the switch gets `!prof`
   branch_weights.  This was the missing piece.
2. **A sample profile does produce arbitrary switch weights.**  Sampling
   the case-body lines at 3/3/3/2/1 (a/b/c/d/e) gives
   `[1, 4,4,4, 1,1,1,4, 3x12, 1,2,2,1,1,1,2,1,1,2]` — the group-2
   plateau (3) is exact, but the first-group cases 0/11/13 and several
   group-3 cases stay at 1 because the loader's edge propagation assigns
   them zero edge weight (their samples land on the `return` edge, not the
   switch edge).  The `+1` in `Weights.push_back(Weight + 1)` shifts the
   plateau: sample 3 -> weight 4, 2 -> 3, 1 -> 2.
3. **But `-fprofile-sample-use` always sets `function_entry_count`.**
   `SampleProfileLoader::emitAnnotations` calls
   `F.setEntryCount(ProfileCount(Samples->getHeadSamples() + 1, ...))`,
   i.e. at least 1, so `hasProfileData()` is true and `LoopSink` runs.
   The w9920 note's premise ("sample profile adds weights without
   `function_entry_count`") is **false for LLVM 7.0.2**.  The body grows:
   the best sample-profile build is **size 1440** (raw 1326, aligned 559)
   with `-mllvm -max-uses-for-sinking=0`, and **1468** without it.  Even
   injecting the *exact* w9920 target vector into the sample-profile IR
   gives size **1464**, so the body, not the weights, is the blocker.
4. **No source-level construct produces the flat vector.**
   `handleSwitchExpect` (clang 7.0.2) handles only `llvm.expect`, not
   `llvm.expect.with.probability` (which clang 7 does not even have:
   `__builtin_expect_with_probability` is unknown in this toolchain), and
   sets exactly one case to `-likely-branch-weight` and every other
   (including default) to `-unlikely-branch-weight` — a two-level vector.
   A linear `if`-chain fold multiplies by `(T+F)/F`, whose minimum with a
   cold default (`F=1`) is **2** per case; the flat plateau needs a ratio
   ~1.2.  So the flat vector is a genuine profile signature.

## 1. Measurements

`raw` / `aligned` / `size` are the target; samples are (a,b,c,d,e) on the
case-body lines.

| variant | raw | aligned | size |
|---|---:|---:|---:|
| master (no profile) | **722** | **320** | **1432** |
| sample 4/4/4/3/2 (LoopSink on) | 1384 | 833 | 1468 |
| sample 3/3/3/2/1 (LoopSink on) | 1393 | 844 | 1472 |
| sample 4/4/4/3/2 + `-max-uses-for-sinking=0` | 1341 | 572 | 1452 |
| sample 3/3/3/2/1 + `-max-uses-for-sinking=0` | 1326 | 559 | 1440 |
| sample 2/2/2/1/1 + `-max-uses-for-sinking=0` | 1329 | 583 | 1448 |
| exact w9920 vector injected into sample-profile IR | 1376 | 616 | 1464 |
| w9920 exact vector injected into unprofiled IR | **558** | **47** | **1432** |

The sample-profile switch weights for 3/3/3/2/1 are
`[1, 4,4,4, 1,1,1,4, 3,3,3,3,3,3,3,3,3,3,3,3, 1,2,2,1,1,1,2,1,1,2]`.

## 2. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10020
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6

# sample-profile build: raw 1326, aligned 559, size 1440
python3 _work/reconstructions/sample_profile_w10020.py 3 3 3 2 1 --extra \
  -mllvm -max-uses-for-sinking=0
# without the sinking flag: size 1468
python3 _work/reconstructions/sample_profile_w10020.py 3 3 3 2 1

# restore the baseline .so
cmake --build cmake-build-match --target link_so -j6
```

The harness writes `/dev/shm/w10020/status.prof` (offsets = source line -
1314), compiles `Status.cpp` with `-fprofile-sample-use=`, relinks with the
extracted gold command, and reports the same raw/aligned metrics as
`fdiff.py` / `align_diff.py`.

## 3. Open questions

1. The sample-profile loader's edge propagation zeroes the switch edge for
   cases whose body is a conditional `return m` (0/11/13/156/163/164/165/
   169/171): their samples land on the return edge.  Putting samples on
   the `return` line too did not fix it.  A profile that annotates the
   *switch terminator* line (offset 20, line 1334) with the case
   distribution might; not tried.
2. If a future LLVM (or a `-mllvm` flag) can suppress the
   `function_entry_count` metadata for one function, the sample profile
   would give the 1432 B body with the flat weights and be a real win.
   In 7.0.2 the count is unconditional (`getHeadSamples() + 1`).
3. The 47 aligned diffs of the unprofiled flat-vector build are code
   differences (case 162 `ldr r6,[sp,#16]`, case 166 r9/r5 allocation, the
   shared `setWon` tail, the 168/169 `ble`/`bgt` inversion), independent
   of the weights.
