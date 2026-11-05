# Per-TU compile-flag audit of the 24 non-exact functions (wave 111, w5917)

Status: research note. Worktree `/home/debian/GalaxyOnFire2/wt/w5917`,
branch `wave111-w5917`, base `8eea060cca1e47f1b895ddc996bd52488f67420b`
("docs: record wave-110 harvest"). Scratch: `/dev/shm/w5917`.
**No `src/`, `tools/`, baseline or allowlist change is landed**: no candidate is a
strict gate improvement. This note is the deliverable.

Canonical gate before and after this pass (unchanged):

```text
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.92
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN   drift: 0
```

## 0. Bottom line

* **No per-TU compile flag in the tested space makes any of the 24 non-exact
  functions byte-exact.** 1,136 main-matrix `(TU, flag)` cells were attempted
  (1,118 built and relinked; 18 build failures), plus 128 extra-`-mllvm`, 160
  combination and 384 branch-weight cells.  No candidate raises `byte_exact` or
  `linked_exact`.
* **No candidate improves the fuzzy score of any of the 24 functions**, with one
  marginal exception: `SpaceLounge::drawLounge` moves **95.5 -> 95.7 %** under a
  per-file branch-weight flag (`-mllvm -likely-branch-weight=7
  -mllvm -unlikely-branch-weight=1`, or 10:1, or 20:12).  It changes only 4
  `.text` bytes (two 16-bit `b.n` targets), keeps `byte_exact`/`linked_exact`/
  `size_exact` at 4500/4500/4524, and leaves the rounded `avg_match` at 99.92.
  It is a real fuzzy improvement but **not a strict gate improvement**, so no
  build flag was landed.
* The canonical `-Oz` recipe is a local optimum in this flag space.  The 24
  residuals are source-shape residuals, consistent with wave-104's conclusion.
* The vendored zlib/libzip/libcrypto TUs are already fully byte-exact under
  their canonical flags (`-Oz` clang for zlib/libzip, GCC `-O3` for libcrypto);
  none of the 24 functions lives in a vendored TU.
* Positive fingerprints agree for all 24 functions: stack-canary call count,
  frame-pointer prologue count and `.ARM.exidx` record class are identical to
  the canonical build.  There is no observable per-TU stack/frame/unwind flag
  difference.

## 1. Baseline and structural state

```sh
cd /home/debian/GalaxyOnFire2/wt/w5917
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
```

Measured structural state (unchanged by this pass):

| property | value |
|---|---|
| section headers | 27 (+NULL), exact |
| file size | 2,187,328 B (`0x216040`), exact |
| `.text` | `0x1337c8`, 95,999 differing bytes |
| `.ARM.exidx` | `0x6f70`, 1,683 differing bytes |
| `.ARM.extab` | `0x26294` vs original `0x26298` (4 B short) |
| `.rodata` | `0x41c27` vs original `0x41b67`: 192-byte mission table appended at `0x20b977`; prefix byte-identical |
| `.data`, `.bss`, `.got`, `.rel.plt`, `.dynsym`, `.dynstr`, `.hash`, `.rel.dyn` | size/base exact; `.data`/`.got`/`.rel.plt`/`.dynsym`/`.dynstr`/`.hash`/`.rel.dyn` byte-identical |

The 24 non-exact functions and their 16 owning TUs (from
`cmake-build-match/verify/report.json` and the baseline objects' symtabs):

| function | TU |
|---|---|
| `Layout::Layout` C1/C2 | `src/game/ui/Layout.cpp` |
| `LevelScript::process` | `src/game/world/LevelScript.cpp` |
| `Radar::draw` | `src/game/weapons/Radar.cpp` |
| `Level::createShip`, `Level::createScene`, `Level::createMission` | `src/game/world/Level.cpp` |
| `MGame::OnTouchEnd`, `MGame::OnUpdate` | `src/game/menu/MGame.cpp` |
| `Globals::Globals` C1/C2 | `src/game/core/Globals.cpp` |
| `Generator::createMission` | `src/game/mission/Generator.cpp` |
| `Status::missionCompleted` | `src/game/mission/Status.cpp` |
| `PlayerEgo::calcCollision` | `src/game/ship/PlayerEgo.cpp` |
| `HangarWindow::render` | `src/game/ui/HangarWindow.cpp` |
| `Gun::calcCharacterCollision` | `src/game/weapons/Gun.cpp` |
| `RocketGun::update` | `src/game/weapons/RocketGun.cpp` |
| `Hud::draw` | `src/game/ui/Hud.cpp` |
| `SpaceLounge::drawLounge` | `src/game/menu/SpaceLounge.cpp` |
| `MenuTouchWindow` C1/C2, `OnTouchBegin` | `src/game/ui/MenuTouchWindow.cpp` |
| `ModStation` C1/C2 | `src/game/menu/ModStation.cpp` |

## 2. Method

For each of the 16 target TUs and each candidate flag:

1. Take the exact canonical compile command from
   `cmake-build-match/compile_commands.json`, replace `-o`, append the candidate
   flag(s), and compile the whole TU.
2. Relink the whole `.so` with the canonical gold link command from
   `build.ninja`, substituting only that TU's object; strip and reorder
   `.shstrtab` exactly as the build does.
3. Score the target function(s) with the official verifier's own
   `disassemble()`/`reconcile()`/`score()` machinery (the same code path as
   `tools/verify/verify.py`), against the original `.so`.
4. Record whether the target's object bytes changed, and whether any exact
   function in the same TU changed.

Sanity check: relinking the **canonical** object through this harness reproduces
`cmake-build-match/libgof2hdaa.so` byte-for-byte
(`sha256 c2ddd19e0a99b6578d5dfe8172e2f417939514676310e1512ebc4a7cb1710240`,
2,187,328 B), and the target score equals the baseline score.  The relink path is
therefore exact.

The necessary-condition argument: `byte_exact` can only rise if a non-exact
function becomes byte-exact.  The 24 non-exact functions are all covered, each
under its own TU's flag.  Since none becomes byte-exact, no candidate can raise
`byte_exact`, regardless of what it does to already-exact functions.  A candidate
can still be a fuzzy improvement, which is why every cell was scored.

## 3. Candidate set

70 valid candidates (one further `-mllvm` spelling, `-mllvm -enable-fp-mad`,
is rejected by clang 7 and was dropped):

```text
-O0 -O1 -O2 -O3 -Os
-fno-exceptions -fno-rtti -fno-exceptions\ -fno-rtti
-fvisibility=hidden -fvisibility=default
-fno-threadsafe-statics -fno-merge-constants
-fomit-frame-pointer -fno-omit-frame-pointer
-fstack-protector -fstack-protector-all -fno-stack-protector
-fno-inline-functions -fno-inline -fno-inline-functions-called-once
-fno-inline-small-functions
-fno-unwind-tables -fno-asynchronous-unwind-tables
-fno-builtin -fno-strict-aliasing
-fsized-deallocation -fno-sized-deallocation -fno-use-cxa-atexit
-fdata-sections -funroll-loops -fno-vectorize -fno-slp-vectorize
-fno-jump-tables -ffast-math -fno-math-errno -fwrapv -fno-strict-overflow
-fno-delete-null-pointer-checks -fno-common -fshort-enums
-mllvm -inline-threshold={0,1,5,10,25,50,100,200}
-mllvm -unroll-threshold={0,50,100,200}
-mllvm -tail-dup-size=0 -mllvm -tail-dup-placement-threshold=0
-mllvm -unroll-runtime -mllvm -unroll-count={1,4}
-mllvm -disable-early-taildup -mllvm -enable-machine-outliner=0
-mllvm -enable-gvn-hoist=0 -mllvm -enable-load-pre=0
-mllvm -enable-cond-stores-vec=0 -mllvm -enable-post-misched=0
-mllvm -disable-cgp-branch-opts -mllvm -disable-licm-promotion
-mllvm -slp-threshold=0 -mllvm -regalloc=basic
-mllvm -align-all-functions={2,4}
```

Additional wave-104-derived `-mllvm` candidates, tested separately:

```text
-mllvm -forward-switch-cond
-mllvm -ifcvt-limit=0
-mllvm -machine-sink-split-probability-threshold=64
-mllvm -disable-machine-dce
-mllvm -t2-reduce-limit2=0
-mllvm -condsider-local-interval-cost
-mllvm -disable-ifcvt-triangle-rev
-mllvm -likely-branch-weight=20 -mllvm -unlikely-branch-weight=12
```

A branch-weight pair sweep (24 pairs on all 16 TUs, then a finer sweep on
`SpaceLounge.cpp`) covered `(likely, unlikely)` = 1..2000 x 1..64.

## 4. Results: per-function table

`base %` is the canonical official fuzzy score.  `neutral flags` counts the
candidates that leave the target's object bytes (and therefore its linked code)
unchanged.  `best changing flag` is the best candidate that does change the
target; its score is always <= the baseline.  `-fvisibility=hidden` is excluded
from "best changing" because it removes the function from `.dynsym` (score 0 by
construction, not a code difference); it is not a viable per-TU flag because the
original exports all 24 symbols.

| function | TU | size | base % | neutral flags | best changing flag | best changing % |
|---|---|---:|---:|---:|---|---:|
| `_ZN6LayoutC1Ev` | game/ui/Layout.cpp | 7072 | 48.3 | 58/70 | `Os` | 46.9 |
| `_ZN6LayoutC2Ev` | game/ui/Layout.cpp | 7072 | 48.3 | 58/70 | `Os` | 46.9 |
| `_ZN11LevelScript7processEi` | game/world/LevelScript.cpp | 57232 | 56.1 | 43/70 | `fno_slp_vectorize` | 56.0 |
| `_ZN5Radar4drawEP6PlayerP3Hudi` | game/weapons/Radar.cpp | 9908 | 74.0 | 44/70 | `ffast_math` | 73.9 |
| `_ZN5Level10createShipEiiiP8Waypointbb` | game/world/Level.cpp | 2764 | 74.4 | 50/68 | `fsized_deallocation` | 65.9 |
| `_ZN5MGame10OnTouchEndEiiPv` | game/menu/MGame.cpp | 8080 | 78.9 | 45/70 | `fno_builtin` | 77.1 |
| `_ZN7GlobalsC1Ev` | game/core/Globals.cpp | 612 | 80.5 | 59/70 | `ffast_math` | 79.2 |
| `_ZN7GlobalsC2Ev` | game/core/Globals.cpp | 612 | 80.5 | 59/70 | `ffast_math` | 79.2 |
| `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | game/mission/Generator.cpp | 1996 | 82.6 | 51/70 | `fsized_deallocation` | 80.2 |
| `_ZN6Status16missionCompletedEbbx` | game/mission/Status.cpp | 1432 | 87.0 | 53/70 | `inline_threshold_25` | 73.7 |
| `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE` | game/ship/PlayerEgo.cpp | 968 | 87.6 | 53/70 | `inline_threshold_25` | 74.5 |
| `_ZN5Level11createSceneEv` | game/world/Level.cpp | 2252 | 88.3 | 47/68 | `ffast_math` | 88.1 |
| `_ZN12HangarWindow6renderEv` | game/ui/HangarWindow.cpp | 9700 | 88.7 | 48/70 | `fno_jump_tables` | 85.5 |
| `_ZN3Gun22calcCharacterCollisionEv` | game/weapons/Gun.cpp | 1984 | 90.0 | 54/70 | `ffast_math` | 79.4 |
| `_ZN5Level13createMissionEv` | game/world/Level.cpp | 17172 | 92.1 | 44/68 | `fsized_deallocation` | 88.7 |
| `_ZN9RocketGun6updateEi` | game/weapons/RocketGun.cpp | 1908 | 93.8 | 54/70 | `ffast_math` | 86.9 |
| `_ZN3Hud4drawExxP9PlayerEgobjj` | game/ui/Hud.cpp | 9332 | 94.6 | 50/70 | `fno_jump_tables` | 91.6 |
| `_ZN11SpaceLounge10drawLoungeEv` | game/menu/SpaceLounge.cpp | 1948 | 95.5 | 55/70 | `Os` | 78.2 |
| `_ZN15MenuTouchWindowC1Ei` | game/ui/MenuTouchWindow.cpp | 9600 | 95.7 | 45/70 | `ffast_math` | 95.2 |
| `_ZN15MenuTouchWindowC2Ei` | game/ui/MenuTouchWindow.cpp | 9600 | 95.7 | 45/70 | `ffast_math` | 95.2 |
| `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | game/ui/MenuTouchWindow.cpp | 1896 | 98.1 | 55/70 | `fno_strict_aliasing` | 90.9 |
| `_ZN10ModStationC1Ev` | game/menu/ModStation.cpp | 1008 | 98.6 | 52/70 | `Os` | 93.1 |
| `_ZN10ModStationC2Ev` | game/menu/ModStation.cpp | 1008 | 98.6 | 52/70 | `Os` | 93.1 |
| `_ZN5MGame8OnUpdateEv` | game/menu/MGame.cpp | 11440 | 99.9 | 46/70 | `load_pre_0` | 94.2 |

`Level.cpp` has 68 cells because `-fno-exceptions` (and the
`-fno-exceptions -fno-rtti` combo) fails on its `try` block.

### 4.1 `-O` level per function

The canonical `-Oz` is the best level (or tied) for every one of the 24.  This is
the strongest single-flag result: no TU is compiled at `-O0`/`-O1`/`-O2`/`-O3`/`-Os`
(`Globals::Globals` ties `-Os` at 80.5 %, but `-Os` changes 22 exact functions in
that TU).

| function | base (`-Oz`) | -O0 | -O1 | -O2 | -O3 | -Os |
|---|---:|---:|---:|---:|---:|---:|
| `Layout::Layout` C1/C2 | 48.3 | 0.8 | 2.5 | 43.7 | 23.2 | 46.9 |
| `LevelScript::process` | 56.1 | 2.7 | 6.8 | 24.7 | 14.2 | 29.3 |
| `Radar::draw` | 74.0 | 5.4 | 20.6 | 24.7 | 30.7 | 27.9 |
| `Level::createShip` | 74.4 | 9.0 | 19.6 | 34.4 | 34.4 | 36.8 |
| `MGame::OnTouchEnd` | 78.9 | 5.8 | 39.7 | 26.3 | 23.2 | 59.6 |
| `Globals::Globals` C1/C2 | 80.5 | 13.0 | 40.0 | 71.1 | 71.1 | 80.5 |
| `Generator::createMission` | 82.6 | 9.0 | 30.1 | 33.1 | 29.2 | 39.8 |
| `Status::missionCompleted` | 87.0 | 8.5 | 26.9 | 27.8 | 19.2 | 36.2 |
| `PlayerEgo::calcCollision` | 87.6 | 6.1 | 42.0 | 27.8 | 65.4 | 64.5 |
| `Level::createScene` | 88.3 | 3.1 | 49.0 | 49.5 | 38.5 | 53.9 |
| `HangarWindow::render` | 88.7 | 5.0 | 6.2 | 47.9 | 24.0 | 47.0 |
| `Gun::calcCharacterCollision` | 90.0 | 2.6 | 37.6 | 63.2 | 62.9 | 63.6 |
| `Level::createMission` | 92.1 | 5.3 | 22.5 | 13.2 | 29.9 | 39.9 |
| `RocketGun::update` | 93.8 | 6.0 | 35.9 | 61.6 | 53.4 | 75.9 |
| `Hud::draw` | 94.6 | 7.0 | 57.1 | 55.9 | 52.9 | 60.8 |
| `SpaceLounge::drawLounge` | 95.5 | 7.9 | 59.3 | 78.0 | 77.4 | 78.2 |
| `MenuTouchWindow` C1/C2 | 95.7 | 4.7 | 52.9 | 56.1 | 49.8 | 55.8 |
| `MenuTouchWindow::OnTouchBegin` | 98.1 | 4.5 | 36.0 | 44.7 | 44.5 | 44.7 |
| `ModStation` C1/C2 | 98.6 | 3.4 | 44.2 | 90.8 | 87.2 | 93.1 |
| `MGame::OnUpdate` | 99.9 | 6.7 | 32.1 | 29.1 | 28.8 | 63.5 |

### 4.2 Flags that are completely unobservable in a TU

For each TU, the table counts candidates that change **neither** the target
function nor any already-exact function in that TU.  Those flags are
unobservable in the binary and cannot be distinguished from the canonical recipe
by code/data; they are not evidence of a different per-TU flag.

| TU | candidates | fully unobservable | target-neutral but changes an exact function |
|---|---:|---:|---:|
| game/core/Globals.cpp | 70 | 38 | 21 |
| game/menu/MGame.cpp | 70 | 41 | 2 |
| game/menu/ModStation.cpp | 70 | 40 | 12 |
| game/menu/SpaceLounge.cpp | 70 | 39 | 16 |
| game/mission/Generator.cpp | 70 | 42 | 9 |
| game/mission/Status.cpp | 70 | 38 | 15 |
| game/ship/PlayerEgo.cpp | 70 | 38 | 15 |
| game/ui/HangarWindow.cpp | 70 | 39 | 9 |
| game/ui/Hud.cpp | 70 | 38 | 12 |
| game/ui/Layout.cpp | 70 | 42 | 16 |
| game/ui/MenuTouchWindow.cpp | 70 | 38 | 5 |
| game/weapons/Gun.cpp | 70 | 43 | 11 |
| game/weapons/Radar.cpp | 70 | 40 | 4 |
| game/weapons/RocketGun.cpp | 70 | 42 | 12 |
| game/world/Level.cpp | 68 | 37 | 6 |
| game/world/LevelScript.cpp | 70 | 40 | 3 |

Representative fully-unobservable flags on every TU: `-fno-rtti`,
`-fvisibility=default`, `-fno-threadsafe-statics`, `-fno-merge-constants`,
`-fno-omit-frame-pointer`, `-fno-sized-deallocation`, `-fno-use-cxa-atexit`,
`-fdata-sections`, `-funroll-loops`, `-fno-vectorize`, `-fno-math-errno`,
`-fno-delete-null-pointer-checks`, `-fno-common`, most
`-mllvm -inline-threshold` values, and most `-mllvm -unroll-threshold` values.
`-fno-rtti` is globally invisible on code but would remove typeinfo from
`.rodata`/`.data.rel.ro` in TUs that have RTTI objects; the global
`data_exact 843/843` already pins RTTI on where it is observable.

The "target-neutral but changes an exact function" column is why a flag that
looks free for the target is not free for the gate: e.g. `Globals.cpp -Os`
leaves `Globals::Globals` at 80.5 % but changes 22 exact functions in the same
TU.

### 4.3 Positive fingerprints (all 24 agree with the canonical build)

Counts are per function, original vs canonical build.  `extab` means the
`.ARM.exidx` entry points at a real `.ARM.extab` record (not `CANTUNWIND`), i.e.
`-funwind-tables` is on.

| function | canary orig/ours | frame-ptr orig/ours | exidx orig/ours |
|---|---:|---:|---|
| `Layout::Layout` C1/C2 | 0/0 | 1/1 | extab/extab |
| `LevelScript::process` | 1/1 | 2/2 | extab/extab |
| `Radar::draw` | 1/1 | 1/1 | extab/extab |
| `Level::createShip` | 0/0 | 1/1 | extab/extab |
| `MGame::OnTouchEnd` | 7/7 | 1/1 | extab/extab |
| `Globals::Globals` C1/C2 | 0/0 | 1/1 | extab/extab |
| `Generator::createMission` | 1/1 | 1/1 | extab/extab |
| `Status::missionCompleted` | 0/0 | 1/1 | extab/extab |
| `PlayerEgo::calcCollision` | 1/1 | 1/1 | extab/extab |
| `Level::createScene` | 3/3 | 1/1 | extab/extab |
| `HangarWindow::render` | 1/1 | 1/1 | extab/extab |
| `Gun::calcCharacterCollision` | 1/1 | 1/1 | extab/extab |
| `Level::createMission` | 1/1 | 1/1 | extab/extab |
| `RocketGun::update` | 1/1 | 1/1 | extab/extab |
| `Hud::draw` | 2/2 | 2/2 | extab/extab |
| `SpaceLounge::drawLounge` | 1/1 | 1/1 | extab/extab |
| `MenuTouchWindow` C1/C2 | 1/1 | 1/1 | extab/extab |
| `MenuTouchWindow::OnTouchBegin` | 0/0 | 1/1 | extab/extab |
| `ModStation` C1/C2 | 1/1 | 2/2 | extab/extab |
| `MGame::OnUpdate` | 1/1 | 1/1 | extab/extab |

This rules out per-TU `-fstack-protector` / `-fstack-protector-all` /
`-fno-stack-protector`, `-fomit-frame-pointer`, and `-fno-unwind-tables` for the
24 functions: each would change at least one of these counts or the EXIDX class.

## 5. The one fuzzy improvement: `SpaceLounge::drawLounge`

A branch-weight pair sweep found exactly one cell above baseline:

```text
SpaceLounge.cpp  -mllvm -likely-branch-weight=7  -mllvm -unlikely-branch-weight=1
                 (equivalently 10:1 or 20:12)
SpaceLounge::drawLounge   95.5 -> 95.7 %   (byte false, linked false, size 1948)
```

Measured facts:

* Only `SpaceLounge::drawLounge` changes in the TU; no exact function changes.
* `.text` size unchanged; **4 bytes** differ vs the canonical build, all inside
  the function (two 16-bit `b.n` target displacements).
* `.text` differing bytes vs the original: 95,999 -> 95,996.
* Full official verifier on the relinked `.so`:
  `byte_exact 4500  linked_exact 4500  size_exact 4524  avg 99.92`
  — identical gate counts, same rounded average.
* The difference is a 3-instruction block-order/target difference in the
  switch-dispatch tail:

```text
original:   0x1e2  0x734  0x756  0x72a  0x734  0x756  0x72a
canonical:  0x1e2  0x734  0x758  0x72a  0x72a  0x734  0x758
bw 7:1:     0x1e2  0x734  0x758  0x72a  0x734  0x758  0x72a
            (b.n targets, in order)
```

The fuzzy score rises because the second repeated `b.n 0x758` aligns slightly
better; the original's `0x756` target is still not reproduced.  This is a
matching knob, not a source-shape fix.

History: `CMakeLists.txt` records that `SpaceLounge.cpp` used to carry a 20:12
per-file branch-weight override, removed in wave 100 because at that time the
linked `.so` was byte-identical with the default 2000:1 weight.  After the
wave-92..110 source changes the knob is no longer byte-identical: it is worth
+0.2 fuzzy on this one function.  It is **not** a strict gate improvement
(`byte_exact` cannot rise without a byte-exact function), so it was not landed.

A finer sweep around the winning values (`likely` 3,7,10,14,20 x `unlikely`
1..64) found no value above 95.7; the improvement is discrete and saturates at
95.7.

## 6. Vendored zlib / libzip / libcrypto TUs

The vendored TUs do not own any of the 24 functions, but their flags were
re-checked:

| library | source | canonical flags | functions in report | byte-exact |
|---|---|---|---:|---:|
| zlib 1.2.3 | `third_party/zlib/zlib-1.2.3` | clang `-Oz -g -fstack-protector-strong -mthumb` | 47 | 47/47 |
| libzip 0.9.3 | `third_party/libzip/libzip-0.9.3/lib` | clang `-Oz -g -fstack-protector-strong -mthumb` | 74 | 74/74 |
| libcrypto 1.1.0l | `third_party/openssl/openssl-1.1.0l` | GCC 4.9 `-O3 -pthread -mandroid -fPIC ...` | 23 | 23/23 |

`tools/flag_fingerprint.py --mode per-tu` recompiles representative vendored
sources at `-O1`/`-O2`/`-Os`; every level changes functions (e.g. zlib
`inflate.c` 12/11/4 changed functions, libzip `zip_dirent.c` 13/7/8, libcrypto
`sha256.c` 9/0/6).  Since all vendored functions are already byte-exact under
the canonical flags, the canonical flags are pinned; no vendored TU differs.

## 7. Proven vs hypothesis

**Proven by measurement (this pass):**

* No tested per-TU flag makes any of the 24 functions byte-exact.
* No tested per-TU flag improves any of the 24 fuzzy scores, except
  `SpaceLounge::drawLounge` +0.2 under branch weights 7:1 / 10:1 / 20:12.
* The canonical `-Oz` is the unique best optimization level for every one of the
  24 functions.
* Stack-canary count, frame-pointer prologue count and EXIDX class are identical
  for all 24 functions.
* The vendored zlib/libzip/libcrypto functions are all byte-exact; their
  canonical flags are correct and no vendored TU owns a residual function.
* Relinking a canonical object through the harness reproduces the canonical
  `.so` byte-for-byte.

**Hypothesis / unobservable:**

* Flags that are completely unobservable in a TU (table in §4.2) cannot be
  excluded from the original build; they also cannot explain the residual.
* A multi-flag combination outside the tested set could in principle make a
  function exact; the single-flag space and a 10-combo screen found none.
* The exact original build driver and per-file flag list are not recoverable
  from the stripped `.so`; wave 104/106 already established the canonical
  CMake/r18b recipe as the only one consistent with the binary.

## 8. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w5917
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6

# per-TU optimization spot check (official tool)
python3 tools/flag_fingerprint.py --mode per-tu \
  --workdir /dev/shm/w5917/ff --jobs 8

# the wider matrix harness used for this note lives in /dev/shm/w5917/harness.py,
# matrix2.py, combo.py, extra.py, bw.py; the core is:
#   1. compile: canonical compile_commands.json command + candidate flag
#   2. relink:  build.ninja gold command with the variant object substituted
#   3. score:   tools/verify/verify.py's disassemble()/reconcile()/score()
# Results: /dev/shm/w5917/matrix2.json (1,136 cells), combo.json, extra.json,
# bw.json.

# single-function side-by-side
python3 tools/verify/verify.py --build-dir /dev/shm/w5917/show \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN11SpaceLounge10drawLoungeEv
```

Harness core (self-contained; the full scripts are small and reproduce the
numbers above):

```python
import json, shlex, subprocess
from pathlib import Path
ROOT = Path('/home/debian/GalaxyOnFire2/wt/w5917')
BUILD = ROOT / 'cmake-build-match'
CC = {e['file']: e for e in json.loads((BUILD/'compile_commands.json').read_text())}

def compile_variant(src, flags, out):
    e = CC[src]
    cmd = shlex.split(e['command'])
    cmd[cmd.index('-o')+1] = str(out)
    cmd += shlex.split(flags)
    return subprocess.run(cmd, cwd=e['directory'], capture_output=True, text=True)

def link_command():
    for line in (BUILD/'build.ninja').read_text().splitlines():
        if line.startswith('  COMMAND = ') and 'ld.gold' in line:
            return line[len('  COMMAND = '):]

def relink(src, obj, outdir):
    outdir = Path(outdir); outdir.mkdir(parents=True, exist_ok=True)
    cmd = link_command().replace('CMakeFiles/gof2.dir/./', 'CMakeFiles/gof2.dir/')
    canon = str(BUILD / CC[src]['output'])
    cmd = cmd.replace(canon, str(obj))
    cmd = cmd.replace(str(BUILD/'libgof2hdaa.unstripped.so'), str(outdir/'lib.unstripped.so'))
    cmd = cmd.replace(str(BUILD/'libgof2hdaa.so'), str(outdir/'libgof2hdaa.so'))
    return subprocess.run(cmd, shell=True, capture_output=True, text=True, cwd=str(BUILD))
```

Scoring uses `tools/verify/verify.py`'s `Elf32`, `Resolver`, `disassemble`,
`reconcile` and `score` exactly as `verify.py --show` does; a function is scored
against the original with the same resolver/PLT/literal-pool normalization as
the official verifier.

## 9. Files touched

* `_work/reconstructions/per-tu-compile-flag-audit-wave111-w5917.md` (this note).
* Scratch only under `/dev/shm/w5917/` (per-cell objects, relinked `.so`s,
  matrix JSONs, official verifier reports).
* No `src/`, `tools/`, `CMakeLists.txt`, baseline or allowlist change.
