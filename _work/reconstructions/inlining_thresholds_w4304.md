# Inlining thresholds and cross-TU inlining (w4304, wave 93)

Status: research note, 2026-11-03. Worktree `wt/w4304`, branch `wave93-w4304`,
base `eea2e3c1`. Scratch is `/dev/shm/w4304`. **No source or build-flag change is
landed**: the note is the deliverable. The canonical gate was green before and after
(§1). Every number below comes from the official `tools/verify/verify.py` run against
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so` on a fresh link
of this tree, not from a proxy.

## 0. Bottom line

* **The original's inliner is exactly clang 7.0.2's `-Oz` default.** LLVM 7.0's
  `InlineConstants::OptMinSizeThreshold` is **5**; `-mllvm -inline-threshold=1..5`
  reproduce the baseline bit-for-bit (only the 20-byte `.note.gnu.build-id`
  differs; `cmp` reports exactly 20 differing bytes for each of `it1`–`it5`).
  **Threshold 6 changes 80 functions and lowers the gate; every larger threshold is
  worse.** The original is not a custom-threshold build.
* **`-inlinehint-threshold` is unobservable here**: 0/5/10/25/50/100/225 are all
  gate-neutral and move no function. The default (325) is consistent with the
  binary; no positive evidence for a custom value.
* **`-mllvm -inline-cost-full` and `-mllvm -enable-nontrivial-unswitch` are
  gate-neutral** (no effect). `-mllvm -inline-caller-superset-nobuiltin`,
  `-mllvm -enable-early-inlining` and `-mllvm -enable-inlining` **do not exist in
  clang 7.0.2** (build failure: unknown LLVM option), so they cannot have been used.
  `-fno-inline-functions` is much worse (1219/3511).
* **LTO is disproven.** A full `-flto` build gives **1277 byte / 2603 linked /
  3098 size-exact of 4170 compared, 354 missing**, `.text 0x12d664`,
  `.rodata 0x40477`, `.ARM.exidx 0x6348`, `.ARM.extab 0x22e40`. The 354 missing
  symbols are the weak `Array*`/template instantiations that LTO inlines and drops;
  the original exports all 4524 functions and keeps 193 empty-`.text` sentinels
  (CANTUNWIND+real pairs), while the LTO build keeps 52. The original is a plain
  per-TU, non-LTO build.
* **No true cross-TU inlining found.** A byte-search of every small leaf function
  in the original finds only identical-body functions (three candidates, all
  false positives). The original's `.text` is perfectly TU-grouped (3839 strong
  functions, 190 TUs, 189 transitions, 0 split TUs), and the 25 remaining
  functions' call-count deltas are mixed, not a systematic "original inlined more"
  signature.
* **No flag is landed.** The current global flag set already reproduces the
  original's inliner; the remaining 25 functions are source/TU shape.

## 1. Baseline and gate

Commands:

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8
```

Before and after this note:

```text
verify: byte 4499  linked 4499  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
CLEAN ✓ (no new soft violations; hard invariants 0)
=== DRIFT: 0 field(s) across 0 class(es) ===
```

`report.json`: `compared 4524`, `size_exact 4524/4524`, `avg 99.9014`.
Section sizes: `.text 0x1337c8`, `.rodata 0x41b67`, `.ARM.exidx 0x6f70`,
`.ARM.extab 0x26298`, `.data 0x296c`, `.bss 0x69ac` (all exact).

The 25 non-`linked` functions at this HEAD (the target set):

```text
Layout::Layout C1/C2, LevelScript::process, Status::missionCompleted,
Radar::draw, Level::createShip, Generator::createMission, MGame::OnTouchEnd,
HangarWindow::render, Globals::Globals C1/C2, RocketGun::update,
PlayerEgo::calcCollision, Level::createScene, Hud::draw,
Gun::calcCharacterCollision, Level::createMission, MenuTouchWindow C1/C2,
SpaceLounge::drawLounge, MenuTouchWindow::OnTouchBegin/OnTouchEnd,
ModStation::ModStation C1/C2, MGame::OnUpdate.
```

(The w3751 note's 26-function list included `StarMap::draw`; it is exact at this
HEAD, so the current target set is 25.)

## 2. Method

Each variant gets its own build directory under `/dev/shm/w4304/build-<tag>` and
its own verify directory. A scratch toolchain file includes the real
`cmake/toolchain.cmake` and appends exactly one candidate flag to both
`CMAKE_CXX_FLAGS` and `CMAKE_C_FLAGS`:

```cmake
# /dev/shm/w4304/toolchain_extra.cmake
include("/home/debian/GalaxyOnFire2/wt/w4304/cmake/toolchain.cmake")
set(GOF2_EXTRA_FLAGS "" CACHE STRING "extra matching flags")
if(GOF2_EXTRA_FLAGS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GOF2_EXTRA_FLAGS}" CACHE STRING "" FORCE)
  set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   ${GOF2_EXTRA_FLAGS}" CACHE STRING "" FORCE)
endif()
```

One variant:

```sh
cmake -S /home/debian/GalaxyOnFire2/wt/w4304 -B /dev/shm/w4304/build-<tag> -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/dev/shm/w4304/toolchain_extra.cmake \
  -DGOF2_MATCH_OPT=-Oz -DGOF2_EXTRA_FLAGS='<flag>' \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build /dev/shm/w4304/build-<tag> --target link_so -j64
python3 tools/verify/verify.py --build-dir /dev/shm/w4304/verify-<tag> \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /dev/shm/w4304/build-<tag>/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
```

Per-TU variants use `CMAKE_PROJECT_INCLUDE` and `COMPILE_FLAGS` (not
`COMPILE_OPTIONS`, which `CMakeLists.txt` already uses on `Radar.cpp`/`StarMap.cpp`):

```cmake
# /dev/shm/w4304/per_tu_flags.cmake
if(GOF2_AB_TU)
  set_source_files_properties("${GOF2_AB_TU}" PROPERTIES COMPILE_FLAGS "${GOF2_AB_FLAGS}")
  set_source_files_properties("${CMAKE_SOURCE_DIR}/${GOF2_AB_TU}" PROPERTIES COMPILE_FLAGS "${GOF2_AB_FLAGS}")
endif()
```

A per-TU sanity check was run first: `HangarWindow.cpp` + `-mllvm
-inline-threshold=6` reproduces the global it6 effect on `HangarWindow::render`
(78.8 -> 76.5) while leaving `Gun::calcCharacterCollision` at 90.0, proving the flag
is scoped to that TU. (The first attempt passed the flag through both
`GOF2_EXTRA_FLAGS` and `GOF2_AB_FLAGS` and was discarded; all per-TU numbers below
are from fresh build directories with `GOF2_EXTRA_FLAGS` empty.)

`.text`/`.rodata` are `readelf -SW` sizes of the linked `.so`; `size` is
`sum(osize == usize)` over the verifier rows.

## 3. What clang 7.0.2 actually does at `-Oz`

The NDK r18b compiler is clang 7.0.2 (`4751641 based on r328903`). The relevant
LLVM 7.0 source is `llvm/lib/Analysis/InlineCost.cpp` and
`llvm/include/llvm/Analysis/InlineCost.h`:

```cpp
// InlineCost.h
const int OptSizeThreshold = 50;        // -Os
const int OptMinSizeThreshold = 5;      // -Oz
const int OptAggressiveThreshold = 250; // -O3

// InlineCost.cpp
static cl::opt<int> InlineThreshold(
    "inline-threshold", cl::Hidden, cl::init(225), ...);
static cl::opt<int> HintThreshold(
    "inlinehint-threshold", cl::Hidden, cl::init(325), ...);
static cl::opt<int> ColdThreshold(
    "inlinecold-threshold", cl::Hidden, cl::init(45), ...);
static cl::opt<int> HotCallSiteThreshold(
    "hot-callsite-threshold", cl::Hidden, cl::init(3000), ...);
static cl::opt<int> LocallyHotCallSiteThreshold(
    "locally-hot-callsite-threshold", cl::Hidden, cl::init(525), ...);
static cl::opt<int> ColdCallSiteThreshold(
    "inline-cold-callsite-threshold", cl::Hidden, cl::init(45), ...);

static int computeThresholdFromOptLevels(unsigned OptLevel, unsigned SizeOptLevel) {
  if (OptLevel > 2) return InlineConstants::OptAggressiveThreshold;
  if (SizeOptLevel == 1) return InlineConstants::OptSizeThreshold;   // -Os
  if (SizeOptLevel == 2) return InlineConstants::OptMinSizeThreshold; // -Oz -> 5
  return InlineThreshold;
}
```

So the `-inline-threshold` cl::opt default (225) is **not** the effective `-Oz`
threshold: `-Oz` selects **5**. An explicit `-mllvm -inline-threshold=N` overrides
the opt-level selection entirely (`InlineThreshold.getNumOccurrences() > 0`), which
is why `-mllvm -inline-threshold=25` is far more aggressive than the default.
The measurement in §4 pins this exactly: 1–5 are identical, 6 differs.

The original's `.comment` names clang 7.0.2; the `.so` is `-Oz` (proven in
`_work/reconstructions/build_flags_matrix_w4207.md`: `-Os` 1263, `-O1` 1033,
`-O2` 1168, `-O3` 1167 byte-exact). Therefore the expected inliner threshold is 5,
and that is what the binary shows.

## 4. Global threshold matrix

| tag | flag | byte | linked | size | avg | .text | .rodata |
|---|---|---:|---:|---:|---:|---|---|
| `base` | `(none)` | 4499 | 4499 | 4524/4524 | 99.9014 | `0x1337c8` | `0x041b67` |
| `it0` | `-mllvm -inline-threshold=0` | 1356 | 4450 | 4498/4524 | 99.6090 | `0x133688` | `0x041b67` |
| `it1` | `-mllvm -inline-threshold=1` | 4499 | 4499 | 4524/4524 | 99.9014 | `0x1337c8` | `0x041b67` |
| `it2` | `-mllvm -inline-threshold=2` | 4499 | 4499 | 4524/4524 | 99.9014 | `0x1337c8` | `0x041b67` |
| `it3` | `-mllvm -inline-threshold=3` | 4499 | 4499 | 4524/4524 | 99.9014 | `0x1337c8` | `0x041b67` |
| `it4` | `-mllvm -inline-threshold=4` | 4499 | 4499 | 4524/4524 | 99.9014 | `0x1337c8` | `0x041b67` |
| `it5` | `-mllvm -inline-threshold=5` | 4499 | 4499 | 4524/4524 | 99.9014 | `0x1337c8` | `0x041b67` |
| `it6` | `-mllvm -inline-threshold=6` | 1354 | 4415 | 4462/4524 | 99.3081 | `0x1368f8` | `0x041b67` |
| `it7` | `-mllvm -inline-threshold=7` | 1354 | 4415 | 4462/4524 | 99.3081 | `0x1368f8` | `0x041b67` |
| `it8` | `-mllvm -inline-threshold=8` | 1354 | 4415 | 4462/4524 | 99.3081 | `0x1368f8` | `0x041b67` |
| `it9` | `-mllvm -inline-threshold=9` | 1354 | 4415 | 4462/4524 | 99.3081 | `0x1368f8` | `0x041b67` |
| `it10` | `-mllvm -inline-threshold=10` | 1354 | 4415 | 4462/4524 | 99.3081 | `0x1368f8` | `0x041b67` |
| `it25` | `-mllvm -inline-threshold=25` | 1352 | 3827 | 3865/4524 | 95.2475 | `0x139c28` | `0x041b67` |
| `it50` | `-mllvm -inline-threshold=50` | 1350 | 3710 | 3746/4524 | 93.5081 | `0x147658` | `0x043057` |
| `it100` | `-mllvm -inline-threshold=100` | 1352 | 3519 | 3553/4524 | 90.9819 | `0x14e208` | `0x043057` |
| `it225` | `-mllvm -inline-threshold=225` | 1350 | 3387 | 3419/4524 | 88.3693 | `0x155a48` | `0x043057` |
| `it225_iht225` | `-mllvm -inline-threshold=225 -mllvm -inlinehint-threshold=225` | 1350 | 3387 | 3419/4524 | 88.3693 | `0x155a48` | `0x043057` |
| `it50_iht50` | `-mllvm -inline-threshold=50 -mllvm -inlinehint-threshold=50` | 1350 | 3710 | 3746/4524 | 93.5081 | `0x147658` | `0x043057` |
| `iht0` | `-mllvm -inlinehint-threshold=0` | 4499 | 4499 | 4524/4524 | 99.9014 | `0x1337c8` | `0x041b67` |
| `iht5` | `-mllvm -inlinehint-threshold=5` | 4499 | 4499 | 4524/4524 | 99.9014 | `0x1337c8` | `0x041b67` |
| `iht10` | `-mllvm -inlinehint-threshold=10` | 4499 | 4499 | 4524/4524 | 99.9014 | `0x1337c8` | `0x041b67` |
| `iht25` | `-mllvm -inlinehint-threshold=25` | 4499 | 4499 | 4524/4524 | 99.9014 | `0x1337c8` | `0x041b67` |
| `iht50` | `-mllvm -inlinehint-threshold=50` | 4499 | 4499 | 4524/4524 | 99.9014 | `0x1337c8` | `0x041b67` |
| `iht100` | `-mllvm -inlinehint-threshold=100` | 4499 | 4499 | 4524/4524 | 99.9014 | `0x1337c8` | `0x041b67` |
| `iht225` | `-mllvm -inlinehint-threshold=225` | 4499 | 4499 | 4524/4524 | 99.9014 | `0x1337c8` | `0x041b67` |
| `inline_cost_full` | `-mllvm -inline-cost-full` | 4499 | 4499 | 4524/4524 | 99.9014 | `0x1337c8` | `0x041b67` |
| `nontrivial_unswitch` | `-mllvm -enable-nontrivial-unswitch` | 4499 | 4499 | 4524/4524 | 99.9014 | `0x1337c8` | `0x041b67` |
| `fno_inline_functions` | `-fno-inline-functions` | 1219 | 3511 | 3571/4524 | 89.0247 | `0x13c0c8` | `0x041b67` |
| `cold0` | `-mllvm -inlinecold-threshold=0` | 4499 | 4499 | 4524/4524 | 99.9014 | `0x1337c8` | `0x041b67` |
| `cold225` | `-mllvm -inlinecold-threshold=225` | 4499 | 4499 | 4524/4524 | 99.9014 | `0x1337c8` | `0x041b67` |
| `coldcs0` | `-mllvm -inline-cold-callsite-threshold=0` | 4499 | 4499 | 4524/4524 | 99.9014 | `0x1337c8` | `0x041b67` |
| `coldcs225` | `-mllvm -inline-cold-callsite-threshold=225` | 4499 | 4499 | 4524/4524 | 99.9014 | `0x1337c8` | `0x041b67` |
| `hot0` | `-mllvm -hot-callsite-threshold=0` | 4499 | 4499 | 4524/4524 | 99.9014 | `0x1337c8` | `0x041b67` |
| `locallyhot0` | `-mllvm -locally-hot-callsite-threshold=0` | 4499 | 4499 | 4524/4524 | 99.9014 | `0x1337c8` | `0x041b67` |

### Per-target fuzzy across global thresholds

| target | base | it0 | it5 | it6 | it10 | it25 | it50 | it100 | it225 | -fno-inline-functions |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| `_ZN6LayoutC1Ev` | 47.7 | 47.7 | 47.7 | 47.7 | 47.7 | 47.7 | 47.7 | 47.7 | 47.7 | 47.7 |
| `_ZN6LayoutC2Ev` | 47.7 | 47.7 | 47.7 | 47.7 | 47.7 | 47.7 | 47.7 | 47.7 | 47.7 | 47.7 |
| `_ZN11LevelScript7processEi` | 52.0 | 52.0 | 52.0 | 52.0 | 52.0 | 47.2 | 47.2 | 46.4 | 34.4 | 27.9 |
| `_ZN6Status16missionCompletedEbbx` | 57.7 | 57.7 | 57.7 | 57.7 | 57.7 | 57.6 | 57.6 | 57.6 | 57.6 | 21.6 |
| `_ZN5Radar4drawEP6PlayerP3Hudi` | 73.9 | 73.9 | 73.9 | 73.9 | 73.9 | 72.6 | 72.6 | 66.0 | 66.0 | 58.2 |
| `_ZN5Level10createShipEiiiP8Waypointbb` | 74.1 | 74.1 | 74.1 | 74.1 | 74.1 | 59.8 | 59.8 | 49.0 | 49.0 | 58.1 |
| `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | 74.3 | 74.3 | 74.3 | 74.3 | 74.3 | 74.3 | 74.3 | 74.3 | 74.3 | 38.4 |
| `_ZN5MGame10OnTouchEndEiiPv` | 78.4 | 78.4 | 78.4 | 78.4 | 78.4 | 45.2 | 45.2 | 45.0 | 37.9 | 52.9 |
| `_ZN12HangarWindow6renderEv` | 78.8 | 78.8 | 78.8 | 76.5 | 76.5 | 75.8 | 75.8 | 74.9 | 74.9 | 47.3 |
| `_ZN7GlobalsC1Ev` | 80.5 | 80.5 | 80.5 | 80.5 | 80.5 | 80.5 | 80.5 | 80.5 | 80.5 | 80.5 |
| `_ZN7GlobalsC2Ev` | 80.5 | 80.5 | 80.5 | 80.5 | 80.5 | 80.5 | 80.5 | 80.5 | 80.5 | 80.5 |
| `_ZN9RocketGun6updateEi` | 86.9 | 86.9 | 86.9 | 86.9 | 86.9 | 86.9 | 86.9 | 86.9 | 86.9 | 36.2 |
| `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE` | 87.3 | 87.3 | 87.3 | 87.3 | 87.3 | 71.2 | 71.2 | 71.2 | 71.2 | 68.4 |
| `_ZN5Level11createSceneEv` | 88.3 | 88.3 | 88.3 | 88.3 | 88.3 | 75.3 | 75.3 | 55.0 | 55.0 | 63.2 |
| `_ZN3Hud4drawExxP9PlayerEgobjj` | 89.1 | 89.1 | 89.1 | 89.1 | 89.1 | 83.4 | 83.4 | 83.4 | 64.4 | 89.1 |
| `_ZN3Gun22calcCharacterCollisionEv` | 90.0 | 48.1 | 90.0 | 90.0 | 90.0 | 90.0 | 90.0 | 90.0 | 90.0 | 45.4 |
| `_ZN5Level13createMissionEv` | 90.7 | 90.7 | 90.7 | 90.7 | 90.7 | 66.9 | 66.9 | 22.9 | 22.9 | 40.0 |
| `_ZN15MenuTouchWindowC1Ei` | 94.2 | 94.2 | 94.2 | 94.2 | 94.2 | 76.1 | 76.1 | 63.2 | 63.2 | 77.6 |
| `_ZN15MenuTouchWindowC2Ei` | 94.2 | 94.2 | 94.2 | 94.2 | 94.2 | 76.1 | 76.1 | 63.2 | 63.2 | 77.6 |
| `_ZN11SpaceLounge10drawLoungeEv` | 95.5 | 95.5 | 95.5 | 95.5 | 95.5 | 95.5 | 95.5 | 95.5 | 95.5 | 65.6 |
| `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | 97.7 | 97.7 | 97.7 | 97.7 | 97.7 | 97.7 | 97.7 | 97.7 | 97.7 | 48.5 |
| `_ZN15MenuTouchWindow10OnTouchEndEiiPv` | 97.9 | 97.9 | 97.9 | 97.9 | 97.9 | 88.2 | 88.2 | 88.2 | 77.8 | 33.1 |
| `_ZN10ModStationC1Ev` | 98.4 | 98.4 | 98.4 | 98.4 | 98.4 | 98.4 | 98.4 | 98.4 | 98.4 | 45.4 |
| `_ZN10ModStationC2Ev` | 98.4 | 98.4 | 98.4 | 98.4 | 98.4 | 98.4 | 98.4 | 98.4 | 98.4 | 45.4 |
| `_ZN5MGame8OnUpdateEv` | 99.9 | 99.9 | 99.9 | 99.9 | 99.9 | 86.9 | 86.9 | 86.9 | 53.5 | 77.0 |

### 4.1 Reading the matrix

* **The threshold plateau is 1–5.** `it0` (no inlining) is much worse
  (1356/4450, `.text 0x133688`). `it1`–`it5` are **bit-for-bit identical to the
  baseline**: `cmp -l build-it5/libgof2hdaa.so build-base/libgof2hdaa.so` reports
  exactly 20 differing bytes, all in the `.note.gnu.build-id` descriptor; every
  section is identical. (The build-id differs because the unstripped output's
  `.debug_info` differs by one byte; the stripped sections are identical.)
  `it6` changes 80 functions, `.text` grows to `0x1368f8`, and the gate drops to
  1354/4415. **The original's effective threshold is 5.**
* **Every larger threshold is worse.** `it25` 1352/3827, `it50` 1350/3710,
  `it100` 1352/3519, `it225` 1350/3387. `.rodata` also grows at 50+
  (`0x43057` vs `0x41b67`), so the aggressive thresholds change data emission too.
* **No threshold raises any function's fuzzy score.** For every variant, the set of
  functions whose `pct` changes has **max Δ ≤ 0.0** (all changes are negative):
  `it0` 50 moved / 0 up, `it6` 80/0, `it25` 560/0, `it50` 669/0, `it100` 788/0,
  `it225` 899/0, `-fno-inline-functions` 1008/0. This is the strongest form of the
  "no improvement" result.
* **`-inlinehint-threshold` is inert** for 0/5/10/25/50/100/225 (0 functions moved,
  `.text` unchanged). The default 325 is consistent with the binary but not
  positively witnessed.
* **`-inline-cost-full` and `-enable-nontrivial-unswitch` are inert** (0 moved).
* **Cold/hot callsite thresholds are inert**: `-inlinecold-threshold=0/225`,
  `-inline-cold-callsite-threshold=0/225`, `-hot-callsite-threshold=0`,
  `-locally-hot-callsite-threshold=0` are all 4499/4499/4524 and move no function.
* **`-fno-inline-functions`** (effectively `-fno-inline`) is much worse
  (1219/3511, `.text 0x13c0c8`), so the original uses normal `-Oz` inlining.
* **Three requested LLVM options do not exist in clang 7.0.2**:
  `-inline-caller-superset-nobuiltin`, `-enable-early-inlining`,
  `-enable-inlining` all fail with
  `clang (LLVM option parsing): Unknown command line argument`. They cannot have
  been used by the original.

### 4.2 Per-target sensitivity

The table above shows the 25 targets. The important rows:

* `Gun::calcCharacterCollision` is 90.0 at thresholds 1–225 but **48.1 at 0**:
  the original needs some inlining, and the default 5 supplies it.
* `HangarWindow::render` is 78.8 at 1–5 and **76.5 at 6–10**, 75.8 at 25,
  74.9 at 50+. Its first inlining decision flips exactly at 6.
* `Level::createShip` is 74.1 at 1–10 and **59.8 at 25**, 49.0 at 50+.
* `Level::createMission` is 90.7 at 1–10 and **66.9 at 25**, 22.9 at 50+.
* `MGame::OnTouchEnd` is 78.4 at 1–10 and **45.2 at 25**.
* `LevelScript::process` is 52.0 at 1–10, 47.2 at 25, 46.4 at 100, 34.4 at 225,
  27.9 with `-fno-inline-functions`.
* `Layout::Layout`, `Globals::Globals`, `Generator::createMission`,
  `RocketGun::update`, `SpaceLounge::drawLounge`, `MenuTouchWindow::OnTouchBegin`,
  `ModStation::ModStation` are threshold-insensitive (same pct at 0–225).

No target improves at any threshold; the original sits on the 1–5 plateau.

## 5. Per-TU matrix

The 14 TUs that own the 25 targets were each compiled with
`-mllvm -inline-threshold=0` and `=25` in isolation (28 builds). Results:

### Per-TU matrix (it0 / it25)

| TU | it0 byte | it0 linked | it0 size | it0 .text | it25 byte | it25 linked | it25 size | it25 .text |
|---|---:|---:|---:|---|---:|---:|---:|---|
| `src/game/ui/Layout.cpp` | 4499 | 4499 | 4524/4524 | `0x1337c8` | 1378 | 4494 | 4519/4524 | `0x133848` |
| `src/game/world/LevelScript.cpp` | 4499 | 4499 | 4524/4524 | `0x1337c8` | 2815 | 4497 | 4521/4524 | `0x133848` |
| `src/game/mission/Status.cpp` | 2766 | 4498 | 4523/4524 | `0x1337d8` | 1402 | 4473 | 4497/4524 | `0x1339c8` |
| `src/game/ui/Hud.cpp` | 4499 | 4499 | 4524/4524 | `0x1337c8` | 1356 | 4487 | 4511/4524 | `0x1339d8` |
| `src/game/world/Level.cpp` | 4499 | 4499 | 4524/4524 | `0x1337c8` | 1465 | 4477 | 4499/4524 | `0x134288` |
| `src/game/mission/Generator.cpp` | 4499 | 4499 | 4524/4524 | `0x1337c8` | 2167 | 4494 | 4519/4524 | `0x133868` |
| `src/game/menu/MGame.cpp` | 4499 | 4499 | 4524/4524 | `0x1337c8` | 3093 | 4496 | 4519/4524 | `0x133848` |
| `src/game/ui/HangarWindow.cpp` | 4499 | 4499 | 4524/4524 | `0x1337c8` | 1378 | 4492 | 4516/4524 | `0x1338c8` |
| `src/game/core/Globals.cpp` | 4499 | 4499 | 4524/4524 | `0x1337c8` | 2538 | 4490 | 4515/4524 | `0x133908` |
| `src/game/weapons/RocketGun.cpp` | 4499 | 4499 | 4524/4524 | `0x1337c8` | 2913 | 4496 | 4521/4524 | `0x1338a8` |
| `src/game/ship/PlayerEgo.cpp` | 4499 | 4499 | 4524/4524 | `0x1337c8` | 1383 | 4473 | 4503/4524 | `0x1339d8` |
| `src/game/menu/SpaceLounge.cpp` | 4499 | 4499 | 4524/4524 | `0x1337c8` | 1369 | 4488 | 4513/4524 | `0x1339a8` |
| `src/game/menu/ModStation.cpp` | 4499 | 4499 | 4524/4524 | `0x1337c8` | 1381 | 4492 | 4517/4524 | `0x133828` |

### Per-TU target deltas (only targets that move)

| TU variant | target | base % | per-TU % | delta |
|---|---|---:|---:|---:|
| `game_world_LevelScript` it25 | `_ZN11LevelScript7processEi` | 52.0 | 47.2 | -4.8 |
| `game_mission_Status` it25 | `_ZN6Status16missionCompletedEbbx` | 57.7 | 57.6 | -0.1 |
| `game_ui_Hud` it25 | `_ZN3Hud4drawExxP9PlayerEgobjj` | 89.1 | 83.4 | -5.7 |
| `game_world_Level` it25 | `_ZN5Level10createShipEiiiP8Waypointbb` | 74.1 | 59.8 | -14.3 |
| `game_world_Level` it25 | `_ZN5Level11createSceneEv` | 88.3 | 75.3 | -13.0 |
| `game_world_Level` it25 | `_ZN5Level13createMissionEv` | 90.7 | 66.9 | -23.8 |
| `game_menu_MGame` it25 | `_ZN5MGame10OnTouchEndEiiPv` | 78.4 | 45.2 | -33.2 |
| `game_menu_MGame` it25 | `_ZN5MGame8OnUpdateEv` | 99.9 | 86.9 | -13.0 |
| `game_ui_HangarWindow` it25 | `_ZN12HangarWindow6renderEv` | 78.8 | 75.8 | -3.0 |
| `game_ship_PlayerEgo` it25 | `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE` | 87.3 | 71.2 | -16.1 |

### 5.1 Reading the per-TU matrix

* **`it0` is neutral for 12 of 13 TUs.** Every TU except `Status.cpp` gives
  exactly 4499/4499/4524 with `.text 0x1337c8` when its inliner is disabled. This
  means those TUs contain no inlining decision in the 0..5 range that the original
  depends on; the default threshold 5 and threshold 0 produce the same code there.
  `Status.cpp` is the exception: `it0` gives 2766/4498/4523, `.text 0x1337d8`
  (some function in `Status.cpp` inlines at ≤5 but not at 0).
* **`it25` is worse for every TU**, and the target deltas are exactly the global
  it25 deltas for the target's own TU (e.g. `Level.cpp` it25: createShip 74.1 ->
  59.8, createScene 88.3 -> 75.3, createMission 90.7 -> 66.9; global it25 gives the
  same three). This confirms the global it25 damage is local to the target TUs, not
  a cross-TU layout artifact.
* **No per-TU variant improves any target** (0 positive deltas in the table above).
  The best per-TU result is the neutral `it0` on 12 TUs; the original is already at
  the default.
* The per-TU `it25` gate counts differ from the global `it25` (e.g. `Level.cpp`
  it25 is 1465/4477 vs global 1352/3827) because only one TU's `.text` changes and
  the layout cascade is smaller. This is why the per-TU isolation matters for
  attributing a target delta; the target pct is identical either way.

## 6. LTO evidence

### 6.1 The `-flto` build

The app TUs were compiled with `-flto` (bitcode objects), then linked manually with
the same gold command plus the LLVM gold plugin:

```sh
NDK=/home/debian/GalaxyOnFire2/wt/w4304/.cache/ndk/android-ndk-r18b
# configure/build bitcode objects
cmake -S . -B /dev/shm/w4304/build-lto -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/dev/shm/w4304/toolchain_extra.cmake \
  -DGOF2_MATCH_OPT=-Oz -DGOF2_EXTRA_FLAGS=-flto \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build /dev/shm/w4304/build-lto --target gof2 gof2_libs -j64
# take the canonical gold+strip+reorder command and insert the plugin
ninja -C /dev/shm/w4304/build-lto -t commands link_so | tail -1 > /dev/shm/w4304/linkcmd.txt
sed "s|$GOLD |$GOLD -plugin $NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/lib/bfd-plugins/LLVMgold.so -plugin-opt=O2 |" \
  /dev/shm/w4304/linkcmd.txt > /dev/shm/w4304/linklto.sh
bash /dev/shm/w4304/linklto.sh
```

`-plugin-opt=Oz` is rejected by gold 1.12 (`Optimization level must be between 0
and 3`); `-plugin-opt=O2` is used, and the per-function `optsize`/`minsize`
attributes from `-Oz` are still in the bitcode. The link took ~45 s.

### 6.2 LTO result

| metric | original | non-LTO (baseline) | `-flto` |
|---|---:|---:|---:|
| byte_exact | — | **4499/4524** | **1277/4170** |
| linked_exact | — | **4499/4524** | **2603/4170** |
| size-exact | — | 4524/4524 | 3098/4170 |
| missing | 0 | 0 | **354** |
| avg | — | 99.9014 | 83.5808 |
| `.text` | `0x1337c8` | `0x1337c8` | **`0x12d664`** |
| `.rodata` | `0x41b67` | `0x41b67` | **`0x40477`** |
| `.ARM.exidx` | `0x6f70` | `0x6f70` | **`0x6348`** |
| `.ARM.extab` | `0x26298` | `0x26298` | **`0x22e40`** |
| defined `FUNC` in `.dynsym` | **4524** | **4524** | **4170** |
| CANTUNWIND entries | **272** | **272** | **83** |
| CANTUNWIND+real same-address pairs | **193** | **193** | **52** |
| `.so` size | 2,187,328 | 2,187,328 | 2,089,008 |

The 354 missing symbols are the weak template instantiations that LTO inlines and
drops: `_Z11ArrayRemoveIP...`, `_Z14ArraySetLengthIP...`,
`_Z19ArrayReleaseClassesIP...`, etc. The original exports every one of them (and
the non-LTO build reproduces them byte-exactly). The 193 CANTUNWIND+real pairs are
gold's empty-`.text` sentinels, one per `-ffunction-sections` object: the original
has 193, the non-LTO build has 193, the LTO build has only 52 (the native vendored
archives). This is direct evidence that the original compiled the app TUs as
separate objects.

Every target except `Globals::Globals` (unchanged at 80.5) drops under LTO, often
catastrophically: `MGame::OnUpdate` 99.9 -> 22.0, `Hud::draw` 89.1 -> 17.1,
`Level::createMission` 90.7 -> 26.1, `Radar::draw` 73.9 -> 16.1.

### 6.3 TU grouping

The original `.text` is perfectly grouped by TU. Mapping every strong function in
our per-TU objects to its address in the original:

```text
mapped strong functions: 3839
distinct TUs: 190
TU transitions in .text order: 189
TUs split into >1 run: 0
```

189 transitions for 190 TUs means each TU occupies exactly one contiguous run. The
run sequence is exactly `tools/source_order.txt` restricted to TUs with strong
functions (`tu_order_check.py`: `sequence == source_order -> True`). The LTO build
also preserves this grouping (the LTO backend emits functions in bitcode order), so
TU grouping alone does not discriminate LTO; it does confirm that the original is
not a function-reordering / ICF build. The discriminating LTO evidence is the 354
missing weak symbols, the 193-vs-52 sentinels, and the `.text`/`.rodata` sizes.

## 7. Cross-TU inlining search

Cross-TU inlining of a non-inline function is impossible without LTO. To look for
it directly, every strong function of 12–48 bytes with no PC-relative literal load
and no branch/call was taken from our byte-exact linked `.so`, and its exact bytes
were searched in the original `.text`. An occurrence inside a *different* TU's
function, at a nonzero offset or in a larger function, would be an embedded inline
copy. Result: **3 candidates, all identical-body false positives**:

| candidate | size | found inside | TU |
|---|---:|---|---|
| `PlayerWormHole::getPosition` | 38 B | `PlayerStatic::getPosition` (54 B) at +0x10 | PlayerStatic.cpp |
| `Waypoint::getPosition` | 38 B | `PlayerStatic::getPosition` (54 B) at +0x10 | PlayerStatic.cpp |
| `Ship::equals` | 14 B | `Item::equals` (20 B) at +2 | Item.cpp |

These are separate out-of-line functions with the same machine code (the classes
share an identical `getPosition`/`equals` body), not an inlined callee: the
candidate itself still exists as a strong exported function at its own address.
No true cross-TU inline copy was found.

Call-count deltas on the 25 targets are mixed, not a systematic "original inlined
more" signature:

| target | orig calls | ours calls | delta |
|---|---:|---:|---:|
| `LevelScript::process` | 2900 | 2899 | +1 |
| `Radar::draw` | 354 | 361 | -7 |
| `Level::createShip` | 87 | 89 | -2 |
| `MGame::OnTouchEnd` | 481 | 479 | +2 |
| `HangarWindow::render` | 440 | 443 | -3 |
| `Level::createMission` | 810 | 811 | -1 |
| `Hud::draw` | 420 | 420 | 0 |
| `MGame::OnUpdate` | 594 | 594 | 0 |

The `HangarWindow::render` -3 is explained by three deliberate `String`
self-assignment anchors in the source (the comment at `HangarWindow.cpp:921`);
the others are source-shape branch differences (e.g. `Radar::draw`'s
`KIPlayer::isDead/isDying` vs `PlayerEgo::goingToPlanet` branches), not inlining.

## 8. Verdicts

| hypothesis | verdict | evidence |
|---|---|---|
| The original's `-inline-threshold` differs from clang 7 `-Oz` default (5) | **disproven** | `it1`–`it5` are bit-identical to baseline (only build-id differs); `it6` changes 80 functions and lowers the gate; every larger threshold is worse. LLVM 7 `OptMinSizeThreshold = 5`. |
| The original used `-inlinehint-threshold=N` with N ≠ 325 | **unknown / no evidence** | 0/5/10/25/50/100/225 all neutral and move no function; default 325 is consistent but unobservable. |
| The original used `-inline-cost-full` | **disproven** (no effect) | gate-neutral, 0 functions moved. |
| The original used `-enable-nontrivial-unswitch` | **disproven** (no effect) | gate-neutral, 0 functions moved. |
| The original used `-fno-inline-functions` | **disproven** | 1219/3511 vs 4499/4499. |
| The original used `-inline-caller-superset-nobuiltin`, `-enable-early-inlining`, or `-enable-inlining` | **disproven** (unsupported) | clang 7.0.2: unknown LLVM option. |
| The original used LTO / whole-program inlining | **disproven** | `-flto` 1277/2603, 354 missing weak symbols, `.text 0x12d664`, `.rodata 0x40477`, 52 vs 193 sentinels. |
| The original used cross-TU inlining of non-inline functions | **disproven / no evidence** | no embedded copy found; `.text` is TU-grouped; call deltas mixed. |
| The original used ICF / function reordering | **disproven** | `.text` is exactly TU-grouped (189 transitions / 190 TUs, 0 split); `.text` size exact without ICF. |
| The original used the clang 7.0.2 `-Oz` default inliner | **proven** | `it5` is the unique bit-identical threshold; `-Oz` is independently proven; LLVM 7 `OptMinSizeThreshold = 5`. |

## 9. Reproduction artifacts

Scratch (not tracked):

```text
/dev/shm/w4304/toolchain_extra.cmake    global A/B wrapper
/dev/shm/w4304/per_tu_flags.cmake       per-TU wrapper (COMPILE_FLAGS)
/dev/shm/w4304/run_one.sh               global variant driver
/dev/shm/w4304/run_pt.sh                per-TU variant driver
/dev/shm/w4304/variants_global.txt      global flag list
/dev/shm/w4304/variants_pt2.txt         per-TU flag list (corrected)
/dev/shm/w4304/variants_extra.txt       cold/hot threshold list
/dev/shm/w4304/out-*.json               per-variant gate summary
/dev/shm/w4304/pct-*.json               per-variant per-function pct
/dev/shm/w4304/verify-*/report.json     official verifier reports
/dev/shm/w4304/build-*/libgof2hdaa.so   per-variant linked .so
/dev/shm/w4304/build-lto/               LTO build
/dev/shm/w4304/cross_tu.py              cross-TU byte-search probe
/dev/shm/w4304/tu_grouping.py           TU-grouping probe
/dev/shm/w4304/tu_order_check.py        TU sequence vs source_order.txt
/dev/shm/w4304/exidx_pairs.py           CANTUNWIND sentinel counter
/dev/shm/w4304/InlineCost70.cpp/.h      LLVM 7.0 source used for the constants
```

The canonical `check`, `lint` and `drift` were re-run after the sweep:
`RATCHET PASS` at 4499/4499, `CLEAN ✓`, `0 field(s)`. `report.json` was not
committed; `git status --porcelain` is empty except for this note.

## 10. What remains

* The 25 non-exact functions are **source-shape**, not flag-shape. The inliner
  threshold, hint threshold, cold/hot thresholds, LTO and cross-TU inlining are all
  pinned or excluded. The next work on them is expression/statement order, local
  lifetimes, TU-membership and the scoped `-mllvm -likely-branch-weight`
  compensations, as in `worst_function_flag_sensitivity_w3751.md`.
* The unobservable knobs are `-inlinehint-threshold` and the cold/hot callsite
  thresholds: no inline-hinted callee's decision changes anywhere in the tested
  0..225 range, and the cold/hot thresholds are inert, so the binary cannot witness
  their defaults. These are negatives, not leads.
* The LTO test used `-plugin-opt=O2` (gold 1.12 rejects `Oz`). Even if a different
  LTO codegen mode could be forced, the 354 missing weak template instantiations
  and the 193-vs-52 sentinels are structural, not codegen-mode, evidence.
