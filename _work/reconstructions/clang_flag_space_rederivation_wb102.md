# Clang 7.0.2 flag-space re-derivation (wave 101, wb102)

**Status:** research note, 2026-11-04. Worktree `wt/wb102`, branch `wave101-wb102`,
base `3856949ab`. No source or build-flag change is landed: no tested flag is a
strict improvement at this tree state. This commit contains this note plus the
`tools/flag_fingerprint.py` screening-harness extension.

## 0. Bottom line

* Baseline is **byte_exact 4500/4524, linked_exact 4500/4524, size-exact
  4524/4524, data_exact 843/843, imports 207, lint CLEAN, drift 0**.
  `.text 0x1337c8`, `.rodata 0x41b67`, `.ARM.exidx 0x6f70`, `.ARM.extab 0x26294`.
* The 24 remaining functions are all size-exact and non-byte-exact. No tested
  flag makes any of them byte-exact, and no tested flag raises `byte_exact`.
* I extended `tools/flag_fingerprint.py` with:
  * `--mode llvm-screen` — compile only the 16 TUs owning the 24 non-exact
    functions, with each candidate flag, and compare object-function bytes to
    the canonical `-Oz` objects;
  * `--variants-file` — an explicit JSON full-link variant list;
  * a guard for driver-only `-mllvm` flags that exit 0 without producing an
    object (e.g. `-mllvm -help`).
* Screening coverage: **218 bool `-mllvm` candidates**, **761 numeric
  `-mllvm` candidates**, **511 further no-value/`-enable-*`/`-disable-*`
  candidates not in the first two lists**, the inline-threshold sweep, and the
  driver-flag list in the task. 42 candidate spellings failed to build (mostly
  value-taking options or clang 7.0.2 crashes); the rest were screened.
* **40 full-link variants** (38 distinct flag spellings) were measured with the
  official verifier. 4 failed to build. The rest are tabulated below.
* Best target-moving per-TU cells are only fractional and are not strict:
  * `Level.cpp -mllvm -sink-insts-to-avoid-spills`: `Level::createScene`
    **88.3 -> 89.3**, but `Level::createMission` **91.1 -> 57.9** and 14 exact
    functions in `Level.cpp` change.
  * `LevelScript.cpp -mllvm -enable-aa-sched-mi`: `LevelScript::process`
    **53.2 -> 53.3**, but 3 exact functions in `LevelScript.cpp` change.
  * `Status.cpp -mllvm -disable-phi-elim-edge-splitting`:
    `Status::missionCompleted` **85.6 -> 85.9**, but 3 exact functions in
    `Status.cpp` change.
  * `Layout.cpp -mllvm -machine-sink-split-probability-threshold=100/64`:
    `Layout::Layout` **47.8 -> 47.9**, but full-link size changes 7072 -> 7024.
* `-mllvm -forward-switch-cond` is a new **gate-neutral** candidate: full link
  is **4500/4500/4524**, only `LevelScript::process` raw bytes change, and its
  fuzzy score stays 53.2. It is a plausible original flag but is unobservable
  on matched code.
* The weight of evidence still says the remaining 24 are **source/TU-shape
  driven**, not a missing global compile flag.

## 1. Baseline and method

Commands:

```sh
cd /home/debian/GalaxyOnFire2/wt/wb102
cmake --build cmake-build-match --target check -j8
```

Baseline output:

```text
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.91
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`size-exact` is 4524/4524. The 24 non-exact functions and their baseline
official-verifier fuzzy scores are:

| mangled name | orig B | fuzzy % |
|---|---:|---:|
| `_ZN6LayoutC1Ev` | 7072 | 47.8 |
| `_ZN6LayoutC2Ev` | 7072 | 47.8 |
| `_ZN11LevelScript7processEi` | 57232 | 53.2 |
| `_ZN5Radar4drawEP6PlayerP3Hudi` | 9908 | 73.9 |
| `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | 1996 | 74.3 |
| `_ZN5Level10createShipEiiiP8Waypointbb` | 2764 | 74.4 |
| `_ZN5MGame10OnTouchEndEiiPv` | 8080 | 78.4 |
| `_ZN7GlobalsC1Ev` | 612 | 80.5 |
| `_ZN7GlobalsC2Ev` | 612 | 80.5 |
| `_ZN12HangarWindow6renderEv` | 9700 | 80.8 |
| `_ZN6Status16missionCompletedEbbx` | 1432 | 85.6 |
| `_ZN9RocketGun6updateEi` | 1908 | 86.9 |
| `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE` | 968 | 87.6 |
| `_ZN5Level11createSceneEv` | 2252 | 88.3 |
| `_ZN3Gun22calcCharacterCollisionEv` | 1984 | 90.0 |
| `_ZN5Level13createMissionEv` | 17172 | 91.1 |
| `_ZN3Hud4drawExxP9PlayerEgobjj` | 9332 | 93.0 |
| `_ZN15MenuTouchWindowC1Ei` | 9600 | 95.1 |
| `_ZN15MenuTouchWindowC2Ei` | 9600 | 95.1 |
| `_ZN11SpaceLounge10drawLoungeEv` | 1948 | 95.5 |
| `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | 1896 | 97.7 |
| `_ZN10ModStationC1Ev` | 1008 | 98.4 |
| `_ZN10ModStationC2Ev` | 1008 | 98.4 |
| `_ZN5MGame8OnUpdateEv` | 11440 | 99.9 |

### 1.1 Object-level screen

The screen compiles only the TUs that own one or more non-exact functions:

```sh
python3 tools/flag_fingerprint.py --mode llvm-screen \
  --workdir /dev/shm/wb102/flag_fingerprint_missing \
  --llvm-candidates /dev/shm/wb102/llvm_candidates_missing.txt \
  --jobs 12 \
  --json /dev/shm/wb102/flag_fingerprint_missing/llvm_screen_missing.json
```

The 16 target TUs are:

```text
src/game/core/Globals.cpp
src/game/menu/MGame.cpp
src/game/menu/ModStation.cpp
src/game/menu/SpaceLounge.cpp
src/game/mission/Generator.cpp
src/game/mission/Status.cpp
src/game/ship/PlayerEgo.cpp
src/game/ui/HangarWindow.cpp
src/game/ui/Hud.cpp
src/game/ui/Layout.cpp
src/game/ui/MenuTouchWindow.cpp
src/game/weapons/Gun.cpp
src/game/weapons/Radar.cpp
src/game/weapons/RocketGun.cpp
src/game/world/Level.cpp
src/game/world/LevelScript.cpp
```

The screen is a sound pre-filter for the linked function bytes: with
`-ffunction-sections` and no LTO, a flag that leaves a target TU's function
bytes unchanged cannot change that function's linked instruction stream except
through a cross-function relocation, which the official verifier resolves by
symbol name. It also records exact-function changes in the same TUs, so a
candidate that is already visibly non-neutral on matched code can be rejected
before a full link.

The first two candidate lists are the existing 218 bool and 761 numeric
`-mllvm` screens. The 511-line `llvm_candidates_missing.txt` was generated from
`clang -mllvm --help` by taking every no-value option not already covered,
adding both bare and `=false` spellings for every missing `-enable-*` /
`-disable-*` option. This closes the obvious `-mllvm -enable-*` /
`-mllvm -disable-*` hole in the earlier lists.

### 1.2 Full-link variants

Each full-link variant is a fresh scratch build + gold link + the official
verifier:

```sh
python3 tools/flag_fingerprint.py --mode variants \
  --workdir /dev/shm/wb102/full_missing_promising \
  --variants-file /dev/shm/wb102/variants_missing_promising.json \
  --jobs 3 --full-verify \
  --json /dev/shm/wb102/full_missing_promising/results.json
```

`--full-verify` is used for all variants whose target fuzzy values matter.
`full_extra_global_fast` used the harness's fast verifier only for gate counts
(`byte_exact`, `linked_exact`, `size_exact` are independent of the difflib
short-circuit); its fuzzy values are not used below.

## 2. Whole-.so full-link results

All rows are measured against the canonical baseline. `size` is
`sum(osize == usize)`. No variant produced a new exact function (`new exact 0`
in every successful row).

### 2.1 Gate-neutral variants

These are **4500 / 4500 / 4524**. All except `-mllvm -forward-switch-cond`
change no code bytes; that flag is called out separately below.

| flag | byte | linked | size |
|---|---:|---:|---:|
| `-fno-vectorize` | 4500 | 4500 | 4524/4524 |
| `-fno-omit-frame-pointer` | 4500 | 4500 | 4524/4524 |
| `-fno-delete-null-pointer-checks` | 4500 | 4500 | 4524/4524 |
| `-fvisibility=default` | 4500 | 4500 | 4524/4524 |
| `-fmerge-all-constants` | 4500 | 4500 | 4524/4524 |
| `-fno-merge-constants` | 4500 | 4500 | 4524/4524 |
| `-falign-functions=16` | 4500 | 4500 | 4524/4524 |
| `-fno-inline-functions-called-once` | 4500 | 4500 | 4524/4524 |
| `-mllvm -inline-threshold=1` | 4500 | 4500 | 4524/4524 |
| `-mllvm -inline-threshold=2` | 4500 | 4500 | 4524/4524 |
| `-mllvm -inline-threshold=3` | 4500 | 4500 | 4524/4524 |
| `-mllvm -inline-threshold=4` | 4500 | 4500 | 4524/4524 |
| `-mllvm -inline-threshold=5` | 4500 | 4500 | 4524/4524 |
| `-mllvm -arm-promote-constant=false` | 4500 | 4500 | 4524/4524 |
| `-mllvm -forward-switch-cond` | 4500 | 4500 | 4524/4524 |

`-mllvm -forward-switch-cond` is the only gate-neutral flag in this pass that
changes a target function's raw bytes: it changes `LevelScript::process` only,
and the official fuzzy score remains 53.2. It is therefore a plausible
original flag, but there is no positive evidence that distinguishes it from
clang's default.

### 2.2 Non-neutral full-link variants

| flag | byte | linked | size |
|---|---:|---:|---:|
| `-mllvm -enable-dse-partial-overwrite-tracking=false` | 1587 | 4487 | 4518/4524 |
| `-mllvm -slp-schedule-budget=32` | 1802 | 4492 | 4515/4524 |
| `-mllvm -slp-schedule-budget=64` | 3221 | 4499 | 4521/4524 |
| `-mllvm -slp-schedule-budget=100` | 2454 | 4500 | 4522/4524 |
| `-mllvm -slp-schedule-budget=225` | 3221 | 4500 | 4522/4524 |
| `-mllvm -instcombine-maxarray-size=8` | 2691 | 4498 | 4523/4524 |
| `-mllvm -jump-threading-threshold=4` | 1378 | 4487 | 4509/4524 |
| `-mllvm -slp-min-tree-size=8` | 1637 | 4493 | 4513/4524 |
| `-mllvm -arm-constant-island-max-iteration=1` | build fail | — | — |
| `-fno-inline-functions` | 1219 | 3509 | 3571/4524 |
| `-mllvm -inline-threshold=6` | 1354 | 4414 | 4462/4524 |
| `-mllvm -disable-phi-elim-edge-splitting` | 1387 | 4407 | 4462/4524 |
| `-mllvm -machine-sink-split-probability-threshold=100` | 2036 | 4473 | 4502/4524 |
| `-mllvm -addr-sink-new-phis` | 1595 | 4490 | 4514/4524 |
| `-mllvm -arm-promote-constant` | 1370 | 4192 | 4221/4524 |
| `-mllvm -no-phi-elim-live-out-early-exit` | 1643 | 4497 | 4520/4524 |
| `-mllvm -extra-vectorizer-passes` | 1480 | 4493 | 4516/4524 |
| `-mllvm -switch-to-lookup` | 2084 | 4496 | 4519/4524 |
| `-mllvm -enable-aa-sched-mi` | 1408 | 4021 | 4498/4524 |
| `-mllvm -sink-insts-to-avoid-spills` | 1477 | 4345 | 4393/4524 |
| `-mllvm -align-all-functions=4` | 1378 | 4500 | 4524/4524 |
| `-mllvm -align-all-functions=16` | 1345 | 2317 | 4517/4524 |

`-mllvm -align-all-functions=4` keeps `linked_exact 4500` and size 4524/4524
but drops `byte_exact` to 1378 and grows `.text` to `0x13a8c8`; it is not a
strict improvement. `-mllvm -align-all-functions=16` is pathological at
`0xf6c5268` `.text` and `linked_exact 2317`.

### 2.3 Driver flags and object-level negatives

The driver screen (`/dev/shm/wb102/flag_fingerprint_driver/screen.json`) gives
the following target-TU object-level counts. A target count is the number of
the 24 non-exact functions whose object bytes change; an exact count is the
number of already-exact functions in the same TUs that change.

| flag | target functions changed | exact functions changed |
|---|---:|---:|
| `-fno-vectorize` | 0 | 0 |
| `-fno-omit-frame-pointer` | 0 | 0 |
| `-fno-delete-null-pointer-checks` | 0 | 0 |
| `-fvisibility=default` | 0 | 0 |
| `-fmerge-all-constants` | 0 | 0 |
| `-fno-merge-constants` | 0 | 0 |
| `-falign-functions=16` | 0 | 0 |
| `-fno-inline-functions-called-once` | 0 | 0 |
| `-fno-math-errno` | 0 | 0 |
| `-fno-trapping-math` | 0 | 0 |
| `-fno-common` | 0 | 0 |
| `-fomit-frame-pointer` | 24 | 545 |
| `-fno-slp-vectorize` | 11 | 25 |
| `-fno-strict-aliasing` | 12 | 45 |
| `-fwrapv` / `-fno-strict-overflow` | 2 | 17 |
| `-fno-jump-tables` | 9 | 31 |
| `-fvisibility=hidden` / `protected` | 5 | 52 |
| `-fno-inline-functions` | 19 | 239 |
| `-ffast-math` | 14 | 79 |
| `-fno-builtin` | 8 | 145 |

`-fno-common` is object-neutral but the full link fails on the duplicate
tentative definition `g_android_back_button_pressed` (`GOF2HD2012.c.obj` vs
`BackButton.c.obj`). The original therefore used clang 7's default `-fcommon`.

## 3. Per-TU target-moving cells

The full per-TU scan (`/dev/shm/wb102/per_tu/scan_missing_results.json`) tested
493 target-TU/flag pairs from the 511-line missing-candidate screen. The scan
uses a per-TU full link and the official `score_targets.py` scorer. The only
positive cells are:

| TU | flag | target | before | after | exact changes in TU |
|---|---|---|---:|---:|---:|
| `src/game/world/Level.cpp` | `-mllvm -sink-insts-to-avoid-spills` | `Level::createScene` | 88.3 | **89.3** | 14 |
| `src/game/world/Level.cpp` | `-mllvm -sink-insts-to-avoid-spills` | `Level::createMission` | 91.1 | 57.9 | 14 |
| `src/game/world/LevelScript.cpp` | `-mllvm -enable-aa-sched-mi` | `LevelScript::process` | 53.2 | **53.3** | 3 |
| `src/game/mission/Status.cpp` | `-mllvm -disable-phi-elim-edge-splitting` | `Status::missionCompleted` | 85.6 | **85.9** | 3 |
| `src/game/ui/Layout.cpp` | `-mllvm -machine-sink-split-probability-threshold=100` | `Layout::Layout` C1/C2 | 47.8 | **47.9** | 0 (but full-link size 7072 -> 7024) |
| `src/game/ui/Layout.cpp` | `-mllvm -machine-sink-split-probability-threshold=64` | `Layout::Layout` C1/C2 | 47.8 | **47.9** | 0 (but full-link size 7072 -> 7024) |

The `Status` and `Layout` rows are from the earlier per-TU scans
(`/dev/shm/wb102/per_tu/scan2_results.json`); the `Level` and `LevelScript`
rows are from this pass. None is strict: the target TU has exact-function
changes, the full-link gate drops, or both.

Two zero-exact target-moving cells were also checked:

* `-mllvm -addr-sink-new-phis` changes `Hud::draw` 93.0 -> 85.3 and
  `Radar::draw` 73.9 -> 64.0; full link is 1595/4490/4514.
* `-mllvm -forward-switch-cond` changes `LevelScript::process` bytes but leaves
  its fuzzy score at 53.2; full link is gate-neutral.

Other zero-exact target-moving cells in the missing screen are all worse:
`-mllvm -sink-insts-to-avoid-spills` on `MGame.cpp` (OnTouchEnd 78.4 -> 71.4,
OnUpdate 99.9 -> 89.8), `Radar.cpp` (73.9 -> 49.8), `RocketGun.cpp`
(86.9 -> 42.8), and `LevelScript.cpp` (53.2 -> 45.4).

The inline-threshold sweep is also negative for the targets:

* `-mllvm -inline-threshold=0` changes `Gun::calcCharacterCollision` and 3 exact
  functions; the per-TU score is 90.0 -> 48.1.
* `-mllvm -inline-threshold=1..5` is globally neutral and target-neutral.
* `-mllvm -inline-threshold=6` changes `HangarWindow::render` and 8 exact
  functions; the per-TU score is 80.8 -> 77.1.
* `-mllvm -inline-threshold=20` on `MGame.cpp` takes `OnTouchEnd` 78.4 -> 45.2
  and `OnUpdate` 99.9 -> 86.9; on `LevelScript.cpp` it takes `process`
  53.2 -> 47.6.

## 4. Proven vs still hypothesis

**Proven by the measurements above:**

1. Baseline is byte 4500 / linked 4500 / size 4524/4524, data 843/843,
   imports 207, lint CLEAN, drift 0.
2. No tested flag raises `byte_exact`, and no tested flag makes any of the 24
   functions byte-exact.
3. `-fno-vectorize`, `-fno-omit-frame-pointer`,
   `-fno-delete-null-pointer-checks`, `-fvisibility=default`,
   `-fmerge-all-constants`, `-fno-merge-constants`, `-falign-functions=16`,
   `-fno-inline-functions-called-once`, `-mllvm -inline-threshold=1..5`,
   `-mllvm -arm-promote-constant=false`, and `-mllvm -forward-switch-cond` are
   globally gate-neutral.
4. `-mllvm -forward-switch-cond` is gate-neutral while changing
   `LevelScript::process` raw bytes; its fuzzy score is unchanged.
5. `-fno-inline-functions`, `-fomit-frame-pointer`, `-fno-slp-vectorize`,
   `-fno-strict-aliasing`, `-fwrapv`, `-fno-jump-tables`, `-ffast-math`,
   `-fno-builtin`, hidden/protected visibility, and the tested
   `-mllvm -slp-*`, `-mllvm -dse-*`, `-mllvm -jump-threading-*`,
   `-mllvm -machine-sink-*`, `-mllvm -align-all-functions=*`, and
   `-mllvm -inline-threshold=6+` variants are worse.
6. `-mllvm -enable-aa-sched-mi` and `-mllvm -sink-insts-to-avoid-spills`
   improve one target cell each but fail the full gate
   (1408/4021/4498 and 1477/4345/4393 respectively).
7. `-fno-common` is not authentic for the current tree: it link-fails on the
   duplicate tentative `g_android_back_button_pressed`.
8. `-mllvm -enable-machine-outliner` crashes clang 7.0.2 on these TUs;
   `-mllvm -arm-constant-island-max-iteration=1` also fails to build.

**Still hypothesis / not settled:**

1. Neutral flags cannot be distinguished from clang 7.0.2 defaults by codegen
   on this tree. There is no positive gate, section, or attribute evidence for
   any of them.
2. `-mllvm -forward-switch-cond` could be an original flag, but it is
   unobservable on matched code and does not improve the fuzzy score.
3. The remaining 24 functions are source/TU-shape driven. Every flag that moves
   codegen enough to matter either changes exact functions or makes the target
   score worse.
4. `-mllvm -use-cfl-aa` and other value-taking `-mllvm` options were not
   exhaustively screened; the 511-line no-value screen covers the obvious
   `-enable-*` / `-disable-*` space.

## 5. Reproduction summary

```sh
# baseline
cmake --build cmake-build-match --target check -j8

# target-TU object screen
python3 tools/flag_fingerprint.py --mode llvm-screen \
  --workdir /dev/shm/wb102/flag_fingerprint_missing \
  --llvm-candidates /dev/shm/wb102/llvm_candidates_missing.txt \
  --jobs 12 \
  --json /dev/shm/wb102/flag_fingerprint_missing/llvm_screen_missing.json

# full-link variant list
python3 tools/flag_fingerprint.py --mode variants \
  --workdir /dev/shm/wb102/full_missing_promising \
  --variants-file /dev/shm/wb102/variants_missing_promising.json \
  --jobs 3 --full-verify \
  --json /dev/shm/wb102/full_missing_promising/results.json

# one per-TU full-link cell
cmake -S . -B /dev/shm/wb102/per_tu/scan_missing_build -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/dev/shm/wb102/per_tu/toolchain_extra.cmake \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs \
  -DCMAKE_PROJECT_INCLUDE=/dev/shm/wb102/per_tu/per_tu_flags.cmake \
  -DGOF2_AB_TU=src/game/world/Level.cpp \
  -DGOF2_AB_FLAGS='-mllvm -sink-insts-to-avoid-spills'
cmake --build /dev/shm/wb102/per_tu/scan_missing_build --target link_so -j8
python3 /dev/shm/wb102/per_tu/score_targets.py \
  --ours /dev/shm/wb102/per_tu/scan_missing_build/libgof2hdaa.so \
  --build-dir /dev/shm/wb102/per_tu/score_scan_missing \
  --names _ZN5Level11createSceneEv,_ZN5Level13createMissionEv
```

No `--update-baseline` was run. The canonical `cmake-build-match` was not
modified by the experiments.
