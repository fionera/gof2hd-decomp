# Status::missionCompleted — the original switch weights are flat
# (4,4,4,3,2), not geometric; no `handleSwitchExpect` shape reaches them
# (wave 152, w9920)

Status: **harness + note, no `src/` / `CMakeLists.txt` change**. Worktree
`wt/w9920`, base master `4a6104a43` (wave 152). Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000). Scratch `/dev/shm/w9920`.

Gate on exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370.

Target: `_ZN6Status16missionCompletedEbbx` (original `0xabf38`, **1432 B**).
Baseline: raw same-address diff **722**, instruction-index aligned diff **320**.

Mission (w9820 follow-up): the original's first jump-table bodies are in
*source* order, while the unprofiled build lays them out in ascending
case-value order.  w9820 showed a full `-fprofile-instr-use` profile shrinks
the body by 15 instructions (514 vs 527), so the original is the *unprofiled*
body with only the switch reordered.  This wave asks for a source-level
switch-weight shape that reproduces the source-order layout at 1432 B.

## 0. Bottom line

1. **The exact weight vector is flat, not geometric.**  Injecting `!prof`
   weights on the existing switch gives the original body order at exactly
   1432 B.  The winning vector is

   ```
   default 1
   {167,174,8,0,11,13,14}                              4
   {150,151,152,153,154,155,158,162,166,170,184,189}   3
   {156,157,160,163,164,165,168,169,171,172}           2
   ```

   **raw 558, aligned 47, size 1432** (baseline 722 / 320 / 1432).  This is
   w7761's `4:3:2:1` vector with 167/174 promoted from 3 to 4; both give the
   same layout.  The layout is a *plateau*: `[1,4,4,4,3,2]`,
   `[1,3,4,4,3,2]`, `[1,6,5,4,3,2]`, `[1,8,6,5,4,3]` and
   `[1,1296,1080,900,750,625]` all give 558/47.  What matters is that the
   last three groups are **distinct and nearly flat** (4 > 3 > 2); a
   geometric vector such as `[1,256,64,16,4,1]` gives 1297/465.

2. **`handleSwitchExpect` cannot produce the flat vector.**  In clang 7.0.2
   `LowerExpectIntrinsic::handleSwitchExpect` sets exactly one case weight to
   `-likely-branch-weight` (default 2000) and every other weight (including
   default) to `-unlikely-branch-weight` (default 1).  That is a two-level
   vector.  `switch (__builtin_expect(m->getType(), K))` was measured for
   all 29 case values `K`: sizes 1356..1456, aligned 566..780.  The only
   1432 B hit is `K=14` (raw 1295, aligned 780).  None is close to 558/47.

3. **A nested switch chain produces geometric weights, not the flat
   plateau.**  Four nested switches (one per group), each with
   `__builtin_expect`, fold back to one switch via
   `FoldValueComparisonIntoPredecessors`.  With the default 2000:1 weights
   the folded vector is `2000^4 : 2000^3 : 2000^2 : 2000 : 1` (with i32
   overflow); the real TU gives **raw 1257, aligned 554, size 1384**.
   With `-mllvm -likely-branch-weight=6 -mllvm -unlikely-branch-weight=5`
   the vector is `1296:1080:900:750:625` (a 558/47 plateau member) but the
   extra switch structure still gives **raw 1291, aligned 440, size 1408**.
   The fold's scaling is multiplicative, so it cannot produce the flat
   4 > 3 > 2 steps.

4. **An if-chain with per-case `__builtin_expect` is also geometric.**
   `if (__builtin_expect(t == X, 1)) ...` folded to a switch gives
   `-689350850, -689350850, -691152757, 600635, ..., 300, 0, ...`
   (decreasing geometric); the real TU gives **raw 1235, aligned 547,
   size 1352**.

5. **Conclusion.**  The source-order layout is reproduced by a *flat*
   four-level weight vector (4,4,4,3,2), which is what a **sample/PGO
   profile** with counts ~4,4,4,3,2 would produce, not by
   `handleSwitchExpect` (one-hot) or by any fold of `__builtin_expect`
   (geometric).  A sample profile (`-fprofile-sample-use`) adds branch
   weights without `function_entry_count`, so `hasProfileData()` stays
   false and `LoopSink` does not shrink the body — exactly the w9820
   observation.  This is the strongest remaining provenance hypothesis.

## 1. Measurements

`raw` / `aligned` / `size` are the target; `groups` is
`(a,b,c,d,e)` = (167/174, 8, 0/11/13/14, group2, group3), default 1.

| variant | raw | aligned | size |
|---|---:|---:|---:|
| master (no weights) | **722** | **320** | **1432** |
| `groups 4 4 4 3 2` | **558** | **47** | **1432** |
| `groups 3 4 4 3 2` | 558 | 47 | 1432 |
| `groups 6 5 4 3 2` | 558 | 47 | 1432 |
| `groups 8 6 5 4 3` | 558 | 47 | 1432 |
| `groups 4 4 4 3 3` | 722 | 320 | 1432 |
| `groups 4 4 4 4 2` | 1252 | 332 | 1432 |
| `groups 4 4 4 2 1` | 1174 | 200 | 1432 |
| `groups 256 64 16 4 1` | 1297 | 465 | 1432 |
| `onehot 167` (`switch(__builtin_expect(t,167))`) | 1277 | 632 | 1392 |
| `nest` (four nested switches, default 2000:1) | 1257 | 554 | 1384 |
| nested + `-mllvm -likely-branch-weight=6 -mllvm -unlikely-branch-weight=5` | 1291 | 440 | 1408 |
| if-chain, per-case `__builtin_expect` | 1235 | 547 | 1352 |

The `!prof` injection is on the *optimized* IR, so it changes only the
switch weights; the body is untouched.  That isolates the layout effect from
the source-shape effect.

## 2. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w9920
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6

# the winning flat vector: raw 558, aligned 47, size 1432
python3 _work/reconstructions/pgo_switchweight_w9920.py groups 4 4 4 3 2

# the source-level producers (all size regressions):
python3 _work/reconstructions/pgo_switchweight_w9920.py onehot 167
python3 _work/reconstructions/pgo_switchweight_w9920.py nest
python3 _work/reconstructions/pgo_switchweight_w9920.py nest --extra \
  -mllvm -likely-branch-weight=6 -mllvm -unlikely-branch-weight=5

# restore the baseline .so after an experiment
cmake --build cmake-build-match --target link_so -j6
```

The harness compiles `Status.cpp` to optimized IR, injects
`!{!"branch_weights", ...}` on the `missionCompleted` switch, recompiles the
IR with the exact TU flags, relinks with the extracted gold command, and
reports the same raw/aligned metrics as `fdiff.py` / `align_diff.py`.

## 3. Open questions

1. A sample-profile build (`-fprofile-sample-use`) with a crafted profile
   whose switch counts are 4,4,4,3,2 is the only remaining non-PGO route to
   the flat vector.  It would not add `function_entry_count`, so the body
   should stay unprofiled.  Not tried this wave.
2. The 47 aligned diffs of the winning injection are *code* differences,
   not layout: case 162's `ldr r6,[sp,#16]`, case 166's r9/r5 allocation,
   the shared `setWon` tail at `ac380`, and the 168/169 `ble`/`bgt`
   inversion.  They are independent of the switch weights and are the next
   source-reconstruction target once the weights are settled.
3. If a sample profile is accepted, the checked-in artifact would be a
   `.profdata`/`.afdo` file plus `-fprofile-sample-use` for `Status.cpp`
   only; that is a `CMakeLists.txt` change and was deliberately not made
   this wave.
