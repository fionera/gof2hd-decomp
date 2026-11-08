# Build provenance round 4 (w10920): the profile-reconstruction harness on
# wave-162 master — sample PGO cannot weight the empty-`br` switch successors,
# instrumented PGO moves the non-switch layout away; no source-only landing

Status: **research note only**, 2026-11-08. Worktree `wt/w10920`, base
master `e51db2fc7` (wave 162). Scratch `/dev/shm/w10920`. Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370. `.text` **85,042** differing bytes, `.ARM.extab`
**725**, `.rel.plt` **0** (byte-identical), `.note.gnu.build-id` 20. All 27
section sizes exact; file size 2,187,328 B.

Targets and baselines:

| function | mangled | raw | aligned | size |
|---|---|---:|---:|---:|
| `Status::missionCompleted` | `_ZN6Status16missionCompletedEbbx` | 722 | 320 | 1,432 |
| `LevelScript::process` | `_ZN11LevelScript7processEi` | 54,698 | 17,456 | 57,232 |

## 0. Bottom line

1. **The harness runs on this base and reproduces the committed sample
   result.** `status_sample_w10117.prof` still gives 1,400 / 607 / 1,464.
   The `extract` mode still recovers the original 30-case physical order
   (`131 64 135 145 142 144 139 69 73 67 70 29 41 80 56 125 92 157 40 102
   154 24 14 21 42 91 94 105 114 16`).
2. **NEW: a plateau-shaped *sample* profile is worse, and its switch
   weights are only a partial plateau.** A sample profile with line counts
   4/3/2 on the source case bodies (offsets 23/29/50/55/60/67/73/80/89/95/
   101/108/125/143/156/162/171/176/187/190/197/204/210/216/225/231/236/
   241/245) gives **1,384 / 833 / 1,468**. The loader produces
   `default 1, {167,174,8}=5, {150,151,152,153,154,155,158,170,184}=4,
   {157,168}=3`, but **cases 0/11/13 stay at 1**: at `-Oz` their switch
   successors are empty `br i1` blocks, and
   `SampleProfileLoader::getInstWeight` ignores `BranchInst`, so no line
   sample can weight them. The plateau requires case 0/11/13 = 4, so a
   sample profile cannot reproduce it.
3. **NEW: the partial plateau is size-exact only in its uniform form.**
   Sampling only the lines that actually map (3/2/1) gives
   **1,358 / 691 / 1,392** (40 B short); the 4/3/2 form gives
   **1,347 / 689 / 1,388**; the uniform 1/1/1 form gives
   **1,394 / 687 / 1,432** (size exact, but raw +672 and `.rel.plt` +5
   bytes on the full link, so not landable).
4. **The instrumented plateau vector is still the closest real profile,
   but it is worse than the unprofiled body.** The committed w10313 vector
   with `-mllvm -max-uses-for-sinking=0` and the harness's default
   `-profile-guided-section-prefix=false` gives **1,299 / 583 / 1,432**
   (localized `.text` diff 85,619, `.rel.plt` 0). With
   `-profile-guided-section-prefix=true` it gives **1,311 / 556 / 1,432**
   but reorders the whole `.text` (diff 351,173), so it is not landable.
   The plateau vector with all other counters `base` gives 1,323/814/1,444
   (`base=0`), 1,342/730/1,424 (`base=1`), 1,347/780/1,412 (`base=2`);
   `base=10..10000` is 1,318..1,366 raw and 1,392..1,444 B.
5. **LevelScript is worse under every profile.** Instrumented original-
   order gradient: **55,656 / 36,817 / 57,452** (prefix=false);
   55,716/36,839/57,452 (prefix=true); 55,685/36,859/57,460 (sink0).
   Uniform instrumented: 55,629/36,548/57,664. Sample profiles in original
   order: descending 55,614/30,669/57,264, ascending 55,715/32,189/57,292,
   uniform 55,697/27,980/57,544, top-1 55,602/27,998/57,276. All are worse
   than the 54,698/17,456/57,232 baseline.
6. **Verdict: no profile shape is a source-only win.** The original's
   4/3/2 plateau is still only reproducible by post-optimisation IR
   `!prof` injection (558/47/1,432). A real sample profile cannot weight
   the empty-`br` successors; a real instrumented profile also sets
   `function_entry_count` and weights on every other branch, which moves
   the non-switch layout away from the heuristic. No `src/`,
   `CMakeLists.txt`, baseline, allowlist or doc change is landed.

## 1. Method

`raw` = same-address differing bytes (`/dev/shm/w10920/fdiff.py`, a
worktree-local copy of the shared tool); `aligned` = instruction-index
alignment (`/dev/shm/w10920/align_diff.py`, the shared tool with the
worktree path substituted); `size` = function bytes.

The harness is the committed `_work/reconstructions/prof_reconstruct_w10820.py`
(extraction + sample/instrumented replay on top of `profgen_w10314.py`).
Scratch: `/dev/shm/w10920/run`.

```sh
cd /home/debian/GalaxyOnFire2/wt/w10920
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6

# original physical case order
W10820_SCRATCH=/dev/shm/w10920/run \
  python3 _work/reconstructions/prof_reconstruct_w10820.py extract

# committed sample profile
W10820_SCRATCH=/dev/shm/w10920/run \
  python3 _work/reconstructions/prof_reconstruct_w10820.py sample \
  src/game/mission/Status.cpp _ZN6Status16missionCompletedEbbx \
  _work/reconstructions/status_sample_w10117.prof

# original-order instrumented gradient
W10820_SCRATCH=/dev/shm/w10920/run \
  python3 _work/reconstructions/prof_reconstruct_w10820.py gradient \
  src/game/world/LevelScript.cpp _ZN11LevelScript7processEi
```

## 2. Why the sample profile cannot produce the plateau

The switch in `missionCompleted` has 29 case values. The plateau vector is
`default 1, {167,174,8,0,11,13,14}=4, group2=3, group3=2`. In the
`-Oz` IR the switch successors are:

| case value | IR successor | has a non-`br` instruction? | sample weight |
|---|---|---|---|
| 167,174 | label 58 | yes (`call getStatusValue`) | 4 |
| 8 | label 63 | yes (`call isCampaignMission`) | 4 |
| 0,11 | label 109 | no (`br i1 %atStation`) | **1** |
| 13 | label 115 | no (`br i1 %atStation`) | **1** |
| 14 | label 119 | no (`br i1 %atStation`) | 4 only if line 1375 is sampled (line 1374 does not map) |
| 150..158, 162, 166, 170, 184, 189 | 130..281 | yes | 3 |
| 156, 164, 165, 169, 171 | 302, 345, 346, 361, 362 | no (empty `br`) | **1** |
| 157, 160, 163, 168, 172 | 309, 329, 53, 352, 368 | yes | 2 |

`SampleProfileLoader::getInstWeight` (LLVM 7.0.2) returns early for
`isa<BranchInst>(Inst)`, so a switch successor that is a single `br i1`
gets block weight 0 and the switch edge gets `0 + 1 = 1`. The original's
case 0/11/13 blocks are `cmp r8,#0; bne` in the ARM code, but at the IR
level they are still `br i1 %atStation`, so a sample profile cannot give
them weight 4 either. This is a structural limit of the sample-profile
loader, not a tuning problem.

## 3. Sample-profile measurements (Status)

All profiles use the function header `_ZN6Status16missionCompletedEbbx:1000:0`
and line offsets relative to the function's start line 1314.

| profile shape | raw | aligned | size | note |
|---|---:|---:|---:|---|
| committed `status_sample_w10117.prof` | 1,400 | 607 | 1,464 | uniform-ish line samples |
| full plateau 4/3/2 on every case body | 1,384 | 833 | 1,468 | cases 0/11/13 stuck at 1 |
| partial plateau 3/2/1 (mapped lines only) | 1,358 | 691 | 1,392 | 40 B short |
| partial plateau 4/3/2 (mapped lines only) | 1,347 | 689 | 1,388 | 44 B short |
| partial uniform 1/1/1 (mapped lines only) | 1,394 | 687 | 1,432 | size exact; `.rel.plt` +5 on the full link |
| `total`/`head` sweep 1..10000 | 1,393 | 844 | 1,472 | header fields are layout-neutral |

The `total`/`head` fields do not change the body: the loader always sets
`function_entry_count` to 1 for this function. The only lever is the line
samples, and the empty-`br` successors cap the reachable weight vector.

## 4. Instrumented-profile measurements (Status)

The committed w10313 vector (`status_instr_1432_relplt0_w10313.proftext`,
116 counters) with `-mllvm -max-uses-for-sinking=0`:

| extra flags | raw | aligned | size | `.text` diff | `.rel.plt` |
|---|---:|---:|---:|---:|---:|
| `-profile-guided-section-prefix=false` (harness default) | **1,299** | 583 | 1,432 | 85,619 | 0 |
| `-profile-guided-section-prefix=true` | 1,311 | 556 | 1,432 | 351,173 | 0 |
| `-use-mbpi=false` | 1,337 | 700 | 1,400 | 948,938 | 45 |
| no sink0 | 1,375 | 675 | 1,416 | — | — |

The plateau vector with all other counters set to `base` and sink0:

| base | raw | aligned | size |
|---:|---:|---:|---:|
| 0 | 1,323 | 814 | 1,444 |
| 1 | 1,342 | 730 | 1,424 |
| 2 | 1,347 | 780 | 1,412 |
| 3 | 1,335 | 776 | 1,412 |
| 4 | 1,313 | 602 | 1,408 |
| 5 | 1,327 | 642 | 1,412 |
| 8 | 1,302 | 648 | 1,396 |
| 10 | 1,318 | 657 | 1,400 |
| 100 | 1,350 | 664 | 1,428 |
| 1000 | 1,350 | 729 | 1,416 |
| 10000 | 1,335 | 571 | 1,444 |

Entry-counter sweep on the w10313 vector (counter 0): 0/1 -> 1,299/583/
1,432; 2/3 -> 1,369/533/1,436; 5 -> 1,365/761/1,432; 10 -> 1,339/752/
1,420; 100 -> 1,336/548/1,440. No entry value reaches the plateau body.

## 5. LevelScript measurements

| profile | raw | aligned | size |
|---|---:|---:|---:|
| baseline (no profile) | **54,698** | **17,456** | **57,232** |
| instrumented original-order gradient, prefix=false | 55,656 | 36,817 | 57,452 |
| instrumented original-order gradient, prefix=true | 55,716 | 36,839 | 57,452 |
| instrumented original-order gradient, sink0 | 55,685 | 36,859 | 57,460 |
| instrumented uniform, prefix=false | 55,629 | 36,548 | 57,664 |
| sample original-order descending | 55,614 | 30,669 | 57,264 |
| sample original-order ascending | 55,715 | 32,189 | 57,292 |
| sample uniform | 55,697 | 27,980 | 57,544 |
| sample top-1 only | 55,602 | 27,998 | 57,276 |

Every profile grows the body by >= 32 B and roughly doubles the aligned
diff. The original body is the unprofiled heuristic layout; the mission
switch's `!prof` is the only profile-shaped signal, and it is not enough
to move the rest of the function toward the original.

## 6. Reproduction scratch

* `/dev/shm/w10920/run/map_offsets.py` — maps every sample line offset to
  the switch-successor index it weights (the empty-`br` table above).
* `/dev/shm/w10920/run/mkprof.py` — builds the plateau / partial / uniform
  sample profiles.
* `/dev/shm/w10920/run/plateau_instr.py`, `sweep_instr2.py`,
  `plateau_instr2.py` — instrumented plateau and entry/base sweeps.
* `/dev/shm/w10920/run/ls_sample.py`, `ls_grad.py` — LevelScript sample
  and instrumented replays.
* `/dev/shm/w10920/run/part111.prof`, `part321.prof`, `part432.prof`,
  `plateau_tot1000.prof` — the sample profiles quoted above.

## 7. Files changed

* `_work/reconstructions/build-provenance-round4-w10920.md` — this note.
* No `src/`, `CMakeLists.txt`, baseline, allowlist or doc change.

## Appendix: exact sample profiles

Full plateau (4/3/2 on every source case body; `total`/`head` are
layout-neutral):

```text
_ZN6Status16missionCompletedEbbx:1000:0
 23: 4
 29: 4
 50: 4
 55: 4
 60: 4
 67: 3
 73: 3
 80: 3
 86: 3
 89: 3
 95: 3
 101: 3
 108: 3
 125: 3
 143: 3
 156: 3
 162: 3
 171: 3
 176: 3
 187: 2
 190: 2
 197: 2
 204: 2
 210: 2
 216: 2
 225: 2
 231: 2
 236: 2
 241: 2
 245: 2
```

Partial plateau (only the offsets that actually map to a switch successor;
3/2/1 gives weights 4/3/2):

```text
_ZN6Status16missionCompletedEbbx:1000:0
 23: 3
 29: 3
 61: 3
 67: 2
 73: 2
 80: 2
 89: 2
 95: 2
 101: 2
 108: 2
 126: 2
 144: 2
 156: 2
 162: 2
 172: 2
 187: 1
 199: 1
 212: 1
 228: 1
 246: 1
```

The offset-to-successor map (`map_offsets.py`) is:

```text
23,24 -> 167,174     29,30 -> 8        61 -> 14
67,68 -> 150         73,74 -> 151      80 -> 152
89,90 -> 153         95,96 -> 154      101,102 -> 155
108,119 -> 158       126,127,128,137 -> 162
144,145,146,150 -> 166
156,157 -> 170       162,163,164 -> 184
172 -> 189           187 -> 157        199 -> 160
212 -> 163           228,229,230 -> 168
246,247,248 -> 172
```

Offsets 50-60 (cases 0/11/13/14 entry lines) do not map; 61 (case 14
`return m`) does. This is the empty-`br` limitation above.
