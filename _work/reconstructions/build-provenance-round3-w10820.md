# Build provenance round 3 (w10820): profile reconstruction — every real
# profile moves the residual bodies *away* from the original

Status: **research note + harness, no `src/` / `CMakeLists.txt` change**,
2026-11-08. Worktree `wt/w10820`, base master `29248c1d0` (wave 161).
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000). Scratch `/dev/shm/w10820`.

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370. `.text` **85,374** differing bytes, `.ARM.extab`
**725**, `.rel.plt` **0** (byte-identical), `.note.gnu.build-id` 20. All 27
section sizes exact; file size 2,187,328 B.

Targets and baselines:

| function | mangled | raw | aligned | size |
|---|---|---:|---:|---:|
| `Status::missionCompleted` | `_ZN6Status16missionCompletedEbbx` | 722 | 320 | 1,432 |
| `LevelScript::process` | `_ZN11LevelScript7processEi` | 55,013 | 17,308 | 57,232 |

## 0. Bottom line

1. **NEW: the sample-profile header is `name:total:head` in this clang, and
   the committed `status_sample_w10117.prof` (`func:1000:0`) is a *valid*
   applied profile, not an ignored one.** It grows the body to 1,464 B.
   A profile with `total = 0` (`func:0:N`) and line samples is ignored and
   leaves the body byte-identical. `-fprofile-sample-accurate` and
   `-mllvm -sample-profile-max-propagate-iterations=100/1000` are neutral
   on the committed profile (still 1,400 / 607 / 1,464).
2. **Sample PGO cannot reproduce the 1,432 B body.** Even a profile with no
   line samples (`func:0:1`) changes the body to 822 / 323 / 1,432 and
   perturbs the rest of the TU (`.text` diff 357,652, `.ARM.exidx` size
   -8), because the loader sets `function_entry_count` / branch weights.
3. **Instrumented PGO still cannot reproduce the 1,432 B body.** The
   w10313 flat vector replays at 1,311 / 556 / 1,432 (bias-neutral); a
   plateau vector with a zero/one/two base gives 1,323 / 814 / 1,444,
   1,342 / 730 / 1,424 and 1,347 / 780 / 1,412. The IR-injection plateau
   remains 558 / 47 / 1,432.
4. **NEW: a harness that extracts the original's physical case-body order
   from the disassembly** (`prof_reconstruct_w10820.py extract`) gives the
   LevelScript mission-switch order
   `131 64 135 145 142 144 139 69 73 67 70 29 41 80 56 125 92 157 40 102
   154 24 14 21 42 91 94 105 114 16` (30 distinct bodies; case 158 shares
   the default). Feeding that order as an instrumented gradient gives
   **55,589 / 36,861 / 57,472** — better than a uniform profile
   (55,704 / 36,363 / 57,724) but worse than the unprofiled baseline.
5. **NEW per-TU `-mllvm -likely-branch-weight` / `-unlikely-branch-weight`
   sweeps** find no size-exact raw win on either target. LevelScript
   `L=4/U=1` is the best raw (54,999, -14) but grows 4 B (gate-fatal);
   `L=10..200` is size-exact but +52 raw; `L=500..4000` is neutral.
   Status `L=7..4000` is neutral; `L=1..5` shrinks 4 B (gate-fatal).
6. **NEW: flipping individual `__builtin_expect` hints in Status** is not a
   win either: the case-166 hint flip (line 1457) is 735 / 319 / 1,432
   (aligned -1, raw +13); the case-184 hint flip (line 1476) is neutral;
   lines 1503/1521 grow the body.
7. **Verdict.** The residual body is the *unprofiled heuristic* body. Every
   real profile route (sample or instrumented) changes the non-switch
   branch weights / entry count and moves the body away; the 4:3:2 plateau
   is only expressible as post-optimisation IR `!prof`. No source-only
   landing is available from this round.

## 1. Method

`raw` = same-address differing bytes; `aligned` = instruction-index
alignment; `size` = function bytes; `relplt` = differing bytes of
`.rel.plt`. The harness is
`_work/reconstructions/prof_reconstruct_w10820.py` (committed):

```sh
# original physical case-body order from the binary (compare chain + TBH)
python3 _work/reconstructions/prof_reconstruct_w10820.py extract

# synthetic instrumented gradient over that order
W10820_SCRATCH=/dev/shm/w10820/run \
  python3 _work/reconstructions/prof_reconstruct_w10820.py gradient \
  src/game/world/LevelScript.cpp _ZN11LevelScript7processEi

# sample profile replay
W10820_SCRATCH=/dev/shm/w10820/run \
  python3 _work/reconstructions/prof_reconstruct_w10820.py sample \
  src/game/mission/Status.cpp _ZN6Status16missionCompletedEbbx \
  _work/reconstructions/status_sample_w10117.prof
```

The extraction side decodes the `cmp r0,#N` + `beq.w target` chain
(0x1387c6..0x138862) and the two contiguous-range TBH tables
(0x138a8e, 15 entries, values 131..145; 0x138c42, 10 entries, values
64..73), then sorts the targets by address. The profile side reuses the
committed `profgen_w10314.py` compile/link helpers (real gold + GNU strip +
`reorder_shstrtab.py`).

## 2. Sample-profile header semantics (new)

The committed `status_sample_w10117.prof` starts
`_ZN6Status16missionCompletedEbbx:1000:0`. In clang 7.0.2 the fields are
`name:total:head`; `total = 0` makes the loader skip the function.

With the committed line-sample body:

| profile header | raw | aligned | size | `.text` diff | `.rel.plt` | `.ARM.exidx` size |
|---|---:|---:|---:|---:|---:|---:|
| `func:1000:0` (committed) | 1,400 | 607 | 1,464 | 1,189,239 | 4 | 28,528 |
| `func:1:0` | 1,400 | 607 | 1,464 | 1,189,239 | 4 | 28,528 |
| `func:0:0`, `func:0:1`, `func:0:1000` | **722** | **320** | **1,432** | **85,374** | **0** | 28,528 |

With **no** line samples, every header gives 822 / 323 / 1,432,
`.text` diff 357,652, and shrinks `.ARM.exidx` by 8 B (gate-fatal):

| profile header | raw | aligned | size | `.text` diff | `.rel.plt` | `.ARM.exidx` size |
|---|---:|---:|---:|---:|---:|---:|
| `func:0:0` / `func:0:1` / `func:1:0` / `func:0:1000` | 822 | 323 | 1,432 | 357,652 | 0 | **28,520** |

So the wave-155/159 "head 0 is a no-op" observation was really the
`total = 0` skip, not the head field: the committed profile is applied.

`-fprofile-sample-accurate` and
`-mllvm -sample-profile-max-propagate-iterations=100/1000` leave the
committed profile at 1,400 / 607 / 1,464 (neutral). A uniform sample
profile for LevelScript (every line offset 0..3599 at 1 sample,
`func:1:3600`) gives **55,691 / 35,623 / 57,504**, `.rel.plt` 71 — worse
than the 55,013 / 17,308 / 57,232 baseline.

## 3. Instrumented PGO on Status

Plateau vector `default 1, {167,174,8,0,11,13,14}=4, group2=3, group3=2`
with every other counter = `base`, `-mllvm -max-uses-for-sinking=0`:

| base | raw | aligned | size |
|---:|---:|---:|---:|
| 0 | 1,323 | 814 | 1,444 |
| 1 | 1,342 | 730 | 1,424 |
| 2 | 1,347 | 780 | 1,412 |
| w10313 committed vector | **1,311** | **556** | **1,432** |
| IR-injection plateau | **558** | **47** | **1,432** |

The w10313 vector is size-exact but its body is frequency-driven;
`-mllvm -block-placement-exit-block-bias=0..200` is neutral on it
(1,311 / 556 / 1,432 at every bias), and `-mllvm -use-mbpi=false` gives
1,339 / 703 / 1,400. No profile route reaches the plateau body.

## 4. Instrumented PGO on LevelScript::process

The mission switch (`switch i32 %2523`, 31 cases, IR case order =
source order) was given a descending gradient in three different orders,
all other counters = 1, `-mllvm -profile-guided-section-prefix=false`:

| order | raw | aligned | size |
|---|---:|---:|---:|
| baseline (no profile) | **55,013** | **17,308** | **57,232** |
| uniform (all counters 1) | 55,704 | 36,363 | 57,724 |
| original physical order (extracted) | 55,589 | 36,861 | 57,472 |
| source order | 55,624 | 35,815 | 57,464 |
| reverse original order | 55,572 | 37,706 | 57,440 |
| original order + `-max-uses-for-sinking=0` | 55,572 | 36,799 | 57,472 |

The extracted order is the best of the profile variants on raw, but every
profile still grows the body by >= 208 B and roughly doubles the aligned
diff: `hasProfileData()` re-places blocks by frequency, while the original
body is the heuristic layout.

## 5. Per-TU branch-weight sweeps (new)

`-mllvm -likely-branch-weight=L -mllvm -unlikely-branch-weight=U` is a
per-TU flag; LevelScript has one `__builtin_expect` (line 3992) and Status
has four (lines 1457/1476/1503/1521).

### LevelScript::process

| L/U | raw | aligned | size | `.text` diff | `.rel.plt` | verdict |
|---|---:|---:|---:|---:|---:|---|
| default 2000/1 | **55,013** | **17,308** | **57,232** | **85,374** | **0** | baseline |
| 10/1, 20/1, 50/1, 100/1, 200/1 | 55,065 | 17,308 | 57,232 | 85,426 | 0 | +52 raw |
| 500/1, 1000/1, 1526/1, 2000/1, 4000/1 | 55,013 | 17,308 | 57,232 | 85,374 | 0 | neutral |
| **4/1** | **54,999** | 17,406 | **57,236** | 406,896 | 0 | +4 B, gate-fatal |
| 3/1 | 55,014 | 17,312 | 57,224 | 359,573 | 0 | -8 B, gate-fatal |
| 1/2 | 55,192 | 17,367 | 57,232 | 85,553 | 0 | +179 raw |
| 1/3 | 55,320 | **17,081** | 57,224 | 359,879 | 0 | -8 B, gate-fatal |
| 1/2000 | 55,174 | 17,610 | 57,244 | 409,767 | 0 | +12 B |
| 1/0 .. 10000/0 | 54,856 | 21,400 | 57,264 | 412,518 | 0 | +32 B |

The best size-exact raw is the baseline itself. `L=4/U=1` is the only
raw improvement (-14) and it costs 4 B, which shifts every later function.

### Status::missionCompleted

| L/U | raw | aligned | size | verdict |
|---|---:|---:|---:|---|
| default 2000/1 | **722** | **320** | **1,432** | baseline |
| 7/1 .. 4000/1 | 722 | 320 | 1,432 | neutral |
| 1/1 .. 5/1 | 884-896 | 339-349 | 1,428 | -4 B, gate-fatal |
| 1/2000 | 746 | 321 | 1,432 | +24 raw |
| 1/0, 2000/0 | 1,053 | 381 | 1,440 | +8 B |

### Individual `__builtin_expect` flips (source-only)

Changing one `expect(cond, 1)` to `expect(cond, 0)` at a time:

| hint line | raw | aligned | size | `.text` diff |
|---|---:|---:|---:|---:|
| 1457 (case 166) | 735 | **319** | 1,432 | 85,387 |
| 1476 (case 184) | 722 | 320 | 1,432 | 85,374 |
| 1503 (case 157 loop) | 1,151 | 418 | 1,464 | 925,493 |
| 1521 (case 163 loop) | 856 | 336 | 1,428 | 856,304 |

Line 1457 is the only size-exact flip and it trades -1 aligned for +13
raw; line 1476 is a no-op. No source-only win.

## 6. Why the profile cannot land the plateau

The unprofiled optimised IR of `missionCompleted` has exactly two `!prof`
nodes (the two body `__builtin_expect`s). The original's switch carries a
flat three-level weight vector; the plateau injection adds exactly that
node after optimisation and leaves the 1,432 B body intact (558 / 47).
Both profile loaders instead:

* set `function_entry_count` (sample always; instrumented via the profile
  header), which enables LoopSink / frequency-driven placement, and
* attach `branch_weights` to every conditional branch, changing the
  non-switch fallthrough choices.

So a profile can *move* the residual body (and the extracted-order
gradient does move LevelScript's case bodies) but it always moves the
non-switch blocks away from the heuristic layout. The remaining lever is
a source-level three-level weight producer, which clang 7.0.2 does not
have (`__builtin_expect` is one-hot; `__builtin_expect_with_probability`
does not exist).

## 7. Files changed

* `_work/reconstructions/prof_reconstruct_w10820.py` — extraction +
  profile-replay harness (committed).
* `_work/reconstructions/build-provenance-round3-w10820.md` — this note.
* No `src/`, `CMakeLists.txt`, baseline, allowlist or doc change.
