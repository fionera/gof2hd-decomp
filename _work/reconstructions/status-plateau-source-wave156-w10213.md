# Status::missionCompleted — source-level plateau hunt (wave 156, w10213)

Status: **analysis only, no `src/` / `CMakeLists.txt` change**. Worktree
`wt/w10213`, base master `cf74f4ee6` (wave 155). Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000). Scratch `/dev/shm/w10213`.

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370.

Target: `_ZN6Status16missionCompletedEbbx` (original `0xabf38`, **1432 B**).
Baseline: raw same-address diff **722**, instruction-index aligned diff **320**.

Mission (w10110 follow-up): find a *source-level* construct that produces the
flat `!prof` vector `default 1, {167,174,8,0,11,13,14}=4, group2=3, group3=2`
(raw 558 / aligned 47 / 1432 B) without a profile.  Read first:
`status-sample-profile-w10020.md`, `pgo-switchweight-wave152-w9920.md`,
`status-generator-spacelounge-wave155-w10110.md`.

## 0. Bottom line

1. **No source-level construct tried reproduces the plateau at 1432 B.**  The
   switch `!prof` is the only lever that reorders the case bodies, and the only
   C++11 source producer, `switch (__builtin_expect(x, hot))`, is one-hot by
   construction (`LowerExpectIntrinsic::handleSwitchExpect` sets one case to
   `-likely-branch-weight` and every other successor, including default, to
   `-unlikely-branch-weight`).  The plateau needs *three* distinct levels.
2. **NEW: `__builtin_expect` on a case body's first `if` *does* reorder the
   case blocks** (the earlier waves only measured `raw`, not the layout).  The
   reorder is a chain-DFS driven by the intra-body branch weight, not a
   weight-sort; it scrambles the layout and changes the body size
   (1388..1444 B), so it cannot be the original source (the original body is
   the baseline body: injecting the plateau into the *baseline* IR gives
   aligned 47, i.e. the body is unchanged).
3. **The plateau vector is in source order.**  The harness injects weights in
   `SwitchInst` case order (source order); `calcMetadataWeights` maps them to
   successors and `MachineBlockPlacement` sorts the bodies by those
   probabilities.  The target therefore says "the first 7 source cases are
   hottest, the next 12 next, the last 10 coldest" — a *source-order* grouping,
   which is what a chain fold also produces (but geometrically, not flat).
4. **The size constraint is tight.**  The body is 1432 B only for weight
   vectors close to the plateau.  `[1,4,4,4,3,2]` -> 1432;
   `[1,16,16x6,12x12,9x10]` -> 1432; `[1,25,25x6,15x12,9x10]` -> 1444;
   `[1,2,2x6,2x12,1x10]` -> 1372; one-hot 4:3 -> 1428.  So a candidate must hit
   both the layout *and* 1432 B.

## 1. Measurements

`raw` / `aligned` / `size` = `fdiff.py` / `align_diff.py` / `missionCompleted`
size.  Baseline 722 / 320 / 1432.

### 1a. `switch (__builtin_expect(t, K))` — one-hot

IR weights (from `-S -emit-llvm`):

* default flags: `[1, 2000, 1, 1, ...]` (one case hot)
* `-mllvm -likely-branch-weight=4 -mllvm -unlikely-branch-weight=3`:
  `[3, 4, 3, 3, ...]`

| K | flags | raw | aligned | size |
|---|---|---:|---:|---:|
| 167 | default | 1277 | 632 | 1392 |
| 167 | 4/3 | 887 | 349 | 1428 |
| 0 | 4/3 | 887 | 349 | 1428 |
| 150 | 4/3 | 957 | 356 | 1428 |
| 156 | 4/3 | 1047 | 363 | 1428 |
| 158 | 4/3 | 1052 | 324 | 1436 |
| 8 | 4/3 | 1046 | 389 | 1436 |
| 184 | 4/3 | 1036 | 354 | 1436 |
| 189 | 4/3 | 1052 | 368 | 1428 |
| 999 (default) | 4/3 | 887 | 349 | 1428 |

The 4/3 ratio is insensitive: `2/1, 3/2, 4/3, 5/4, 6/5, 8/7, 10/9, 16/15`
all give 887 / 349 / 1428 for K=167.  Layout is "hot case first, then
ascending, with 184/189 last" — group d and group e are still interleaved.

### 1b. `__builtin_expect` on each case body's first `if`

IR: each case body's branch gets `!{!"branch_weights", i32 2000, i32 1}`.

| subset | polarity | raw | aligned | size |
|---|---|---:|---:|---:|
| all 29 cases | 1 | 1301 | 587 | 1388 |
| all 29 cases | 0 | 1277 | 380 | 1444 |
| group d (12) | 1 | 1312 | 763 | 1416 |
| group d 1 / group e 0 | mixed | 1319 | 765 | 1420 |
| single case 150/151/153/154/155 | 1 | 744 | 327 | 1432 |
| single case 170 | 1 | 744 | 324 | 1432 |
| single case 166 | 0 | 735 | **319** | 1432 |
| single case 168 | 1 | 755 | 331 | 1432 |
| single case 189 | 0 | 874 | 337 | 1432 |
| single case 172 | 0 | 938 | 336 | 1432 |
| single case 8 | 1 | 1264 | 402 | 1432 |
| single case 11 | 1 | 1316 | 475 | 1452 |
| single case 174 | 1 | 1276 | 695 | 1476 |

Single-case expects on 156, 157, 158, 160, 162(1), 163, 164, 165, 167, 171,
172(1), 184, 189(1) are no-ops (722 / 320 / 1432) — the `llvm.expect` folds
away or does not change the chain.  A greedy forward search over the 11
non-no-op single expects found **no** raw improvement at size 1432.  The one
aligned improvement (`166:0`, 319) costs 13 raw bytes, so it is not a win.

### 1c. Nested switches with `__builtin_expect`

Three nested switches (group a/b/c outer, group d middle, group e inner),
each `switch (__builtin_expect(t, K))`, `-likely 4 -unlikely 3`:
**1299 / 444 / 1404**.  The optimized IR has **no `!prof` at all** — the
nested `llvm.expect` is folded away before `LowerExpectIntrinsic`, so the
nested shape is just a different (smaller) body.

### 1d. Flat if-chain (do/while dispatch) with `__builtin_expect`

A `do { if (__builtin_expect(t==..., 1)) {...} else if ... } while (0)` chain
emitted from the same case bodies:

| flags | raw | aligned | size |
|---|---:|---:|---:|
| default | 1237 | 657 | 1348 |
| `-likely 4 -unlikely 3` | 1255 | 530 | 1372 |
| `-likely 6 -unlikely 5` | 1256 | 517 | 1372 |
| `-likely 5 -unlikely 4` | 1256 | 517 | 1372 |

IR weights are geometric with overflow/zeros, e.g.
`[0, -689350850, -689350850, -691152757, 600635, 600635, 600335, 300, 0, ...]`.
The body uses a **TBB** for the low group (8..14) instead of the original
**TBH**, so it can never be 1432 B.

### 1e. Loop-condition and source-order knobs

| variant | raw | aligned | size |
|---|---|---:|---:|
| `__builtin_expect(i < size, 1)` default | 722 | 320 | 1432 |
| `__builtin_expect(i < size, 0)` default | 754 | 320 | 1432 |
| loop expect 4/3 (either polarity) | 914 | 349 | 1428 |
| group-e cases moved first in source | 1153 | 481 | 1412 |
| swap `case 189`/`case 156` in source | 722 | 320 | 1432 |
| `while (true) { switch } break;` dispatch | 722 | 320 | 1432 |
| `do { switch } while (0);` dispatch | 722 | 320 | 1432 |

Source order alone does not reorder the bodies (the compiler sorts by case
value); it only moves the shared-tail blocks (`184`, `189`).

## 2. Why the plateau is not source-producible in clang 7.0.2

* `handleSwitchExpect` (LLVM 7.0.1 `LowerExpectIntrinsic.cpp:57`) is one-hot:
  `Weights(n+1, UnlikelyBranchWeight); Weights[hot] = LikelyBranchWeight`.
* `FoldValueComparisonIntoPredecessors` (`SimplifyCFG.cpp:1005`) scales a
  predecessor case by `w1 * ValidTotalSuccWeight` and successor cases by
  `w0 * s_i`.  With a successor of `n` cases the spike is `~n` times the
  successor weights, so a fold is geometric (ratio >= ~1.5), never the flat
  4/3/2 plateau.  A chain of 29 arms gives ratio ~2.4 even with 4:3 weights.
* The only other `!prof` producers are `-fprofile-instr-use` and
  `-fprofile-sample-use`; the latter adds `function_entry_count` and grows the
  body to >= 1440 B (w10020).

So the plateau is a genuine profile signature.  A source-only build keeps the
baseline 722 / 320 / 1432.

## 3. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10213
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10213/fdiff.py _ZN6Status16missionCompletedEbbx
python3 /dev/shm/w10213/align_diff.py _ZN6Status16missionCompletedEbbx

# variant scorer (compiles a Status.cpp variant, relinks, reports raw/aligned/size)
python3 /dev/shm/w10213/score_src.py <variant.cpp> [clang args...]
# case-body expect generator
python3 /dev/shm/w10213/genexp2.py out.cpp 150:1,156:0
# case-value -> block-address order
python3 /dev/shm/w10213/blockorder.py

# plateau reference (IR injection)
W9920_SCRATCH=/dev/shm/w10213/w9920 \
  python3 _work/reconstructions/pgo_switchweight_w9920.py groups 4 4 4 3 2
```

Scratch (`/dev/shm/w10213/`): `fdiff.py`, `align_diff.py`, `score_src.py`,
`genexp.py`, `genexp2.py`, `sweep1.py`, `greedy.py`, `blockorder.py`,
`v_*.cpp`, `*.ll`.

## 4. Files changed

* `_work/reconstructions/status-plateau-source-wave156-w10213.md` (this note).
