# Targeted `-mllvm` codegen sweep at wave 154 (w10116)

Status: **research note + harness, no `src/` / `CMakeLists.txt` change**. Worktree
`wt/w10116`, base master `f4e0ea40b` (wave 154). Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so` (NDK r18b
clang 7.0.2, `-Oz`, image base 0x10000). Scratch `/dev/shm/w10116`.

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370. `.text` 86,347 differing bytes, `.ARM.extab` 1,255.

## 0. Bottom line

* **No `-mllvm` option improves the gate.** 7 variants are exactly neutral
  (`byte 4507 / linked 4507 / size 4524`, all 17 residuals bit-for-bit
  unchanged); every other buildable variant is strictly worse.
* **No variant makes any residual byte-exact.** The two closest residuals stay
  `MenuTouchWindow::OnTouchBegin` (raw 29 / aligned 15) and
  `MGame::OnUpdate` (raw 37 / aligned 109).
* **No variant improves the aligned metric on any residual** (minimum Δ is 0).
  Three variants shave raw bytes off `Layout::Layout` (`-slp-threshold=2`
  raw −20, `-slp-threshold=8` raw −4, `-load-pre=false` raw −23) but all three
  make its aligned diff much worse (+105, +1204, +438). They are same-address
  byte artifacts, not structural wins.
* The neutral results pin the effective `-Oz` defaults: SLP threshold 0,
  inline threshold ≤ 5, unroll threshold 0, GVN hoist on but inert at `-Oz`,
  and no profile data for `-profile-guided-section-prefix`. The original's
  codegen is consistent with the stock clang 7.0.2 `-Oz` pipeline; the 17
  residual functions are source/TU shape (confirms `worst_function_flag_sensitivity_w3751.md`
  at the wave-154 tree).
* Several task spellings do not exist in clang 7.0.2: `-slp-max-vf`,
  `-enable-loop-sink` / `-loop-sink`, `-enable-early-taildup`,
  `-enable-merge-functions` / `-fmerge-functions`, `-enable-constant-islands`,
  `-arm-cp-islands`. The nearest valid spellings were measured where they
  exist and are worse. `-enable-machine-outliner` crashes clang 7.0.2 on
  `VirtualInput.cpp`.

## 1. Method

A scratch toolchain wrapper appends `GOF2_EXTRA_FLAGS` to both app C and C++
flags after the canonical set:

```cmake
include("/home/debian/GalaxyOnFire2/wt/w10116/cmake/toolchain.cmake")
set(GOF2_EXTRA_FLAGS "" CACHE STRING "extra matching flags")
if(GOF2_EXTRA_FLAGS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GOF2_EXTRA_FLAGS}" CACHE STRING "matching ARM compile flags" FORCE)
  set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   ${GOF2_EXTRA_FLAGS}" CACHE STRING "matching ARM C compile flags" FORCE)
endif()
```

Each variant is a full rebuild + link in `/dev/shm/w10116/build-sweep`, then
the official `tools/verify/verify.py` (for `byte` / `linked` / size-exact) and
a per-function raw/aligned scorer. The raw metric is the same-address byte
diff; the aligned metric is `_work/reconstructions/align_diff.py`'s
instruction-index alignment. Both are measured for all 17 residual functions.
The harness is committed as `_work/reconstructions/mllvm_sweep_w10116.py`.

Commands:

```sh
cmake -S . -B /dev/shm/w10116/build-sweep -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/dev/shm/w10116/toolchain_extra.cmake \
  -DGOF2_MATCH_OPT=-Oz -DGOF2_EXTRA_FLAGS='<flag>' \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build /dev/shm/w10116/build-sweep --target link_so -j6
python3 tools/verify/verify.py --build-dir /dev/shm/w10116/verify/<id> \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /dev/shm/w10116/build-sweep/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
```

## 2. Global matrix

`residuals moved` is the number of the 17 residual functions whose raw or
aligned diff changed. `ok` rows are full-gate measurements.

| variant | status | byte | linked | size-exact | residuals moved |
|---|---|---:|---:|---:|---:|
| `base` | ok | 4507 | 4507 | 4524/4524 | 0 |
| `-mllvm -slp-threshold=0` | ok | 4507 | 4507 | 4524/4524 | 0 |
| `-mllvm -slp-threshold=1` | ok | 1637 | 4503 | 4521/4524 | 17 |
| `-mllvm -slp-threshold=2` | ok | 1815 | 4499 | 4513/4524 | 17 |
| `-mllvm -slp-threshold=4` | ok | 1460 | 4407 | 4427/4524 | 17 |
| `-mllvm -slp-threshold=8` | ok | 1484 | 4393 | 4412/4524 | 17 |
| `-mllvm -slp-max-vf=1/2/4/8` | build-fail | - | - | - | - |
| `-mllvm -slp-max-reg-size=1/2/4/8` | ok | 1461 | 4392 | 4411/4524 | 17 |
| `-mllvm -enable-loop-sink=false/true` | build-fail | - | - | - | - |
| `-mllvm -loop-sink=false/true` | build-fail | - | - | - | - |
| `-mllvm -tail-merge-threshold=0` | ok | 1338 | 3855 | 3875/4524 | 17 |
| `-mllvm -tail-merge-threshold=1` | ok | 1324 | 3854 | 3875/4524 | 17 |
| `-mllvm -tail-merge-threshold=2` | ok | 1360 | 4074 | 4105/4524 | 17 |
| `-mllvm -tail-merge-threshold=4` | ok | 1367 | 4357 | 4373/4524 | 17 |
| `-mllvm -enable-tail-merge=false` | ok | 1338 | 3855 | 3875/4524 | 17 |
| `-mllvm -inline-threshold=0` | ok | 1356 | 4458 | 4498/4524 | 17 |
| `-mllvm -inline-threshold=1` | ok | 4507 | 4507 | 4524/4524 | 0 |
| `-mllvm -inline-threshold=5` | ok | 4507 | 4507 | 4524/4524 | 0 |
| `-mllvm -inline-threshold=25` | verify-fail | - | - | - | - |
| `-mllvm -inline-threshold=50` | verify-fail | - | - | - | - |
| `-mllvm -unroll-threshold=0` | ok | 4507 | 4507 | 4524/4524 | 0 |
| `-mllvm -unroll-threshold=50` | ok | 1554 | 4504 | 4521/4524 | 17 |
| `-mllvm -unroll-threshold=150` | ok | 1413 | 4498 | 4516/4524 | 17 |
| `-mllvm -unroll-threshold=300` | ok | 1413 | 4495 | 4512/4524 | 17 |
| `-mllvm -enable-gvn-hoist=false` | ok | 4507 | 4507 | 4524/4524 | 0 |
| `-mllvm -enable-load-pre=false` | ok | 1410 | 4194 | 4331/4524 | 17 |
| `-mllvm -enable-early-taildup=false` | build-fail | - | - | - | - |
| `-mllvm -disable-early-taildup=true` | ok | 3594 | 4493 | 4517/4524 | 9 |
| `-mllvm -enable-merge-functions` | build-fail | - | - | - | - |
| `-fmerge-functions` | build-fail | - | - | - | - |
| `-mllvm -enable-machine-outliner` | build-fail (clang crash) | - | - | - | - |
| `-mllvm -enable-constant-islands=false` | build-fail | - | - | - | - |
| `-mllvm -arm-cp-islands=false` | build-fail | - | - | - | - |
| `-mllvm -profile-guided-section-prefix=false` | ok | 4507 | 4507 | 4524/4524 | 0 |

Notes:

* `-inline-threshold=25/50` remove 126 functions from the link (`missing 126`),
  so the official verifier exits non-zero; the gate is unambiguously worse.
* `-slp-min-tree-size=0` was started but is pathological in clang 7.0.2: a
  single `Engine.cpp` compile exceeded 5 min CPU and 2.8 GB RSS. Not completed;
  it is not a plausible original flag.
* `-slp-max-vf` is not a clang 7.0.2 option; `-slp-max-reg-size` is the
  nearest valid SLP knob, and every value 1/2/4/8 gives the identical worse
  result (1461/4392), so it is a single distinct variant, not four.
* `-loop-sink` is only a pass name in this clang, not a `cl::opt`; LoopSink
  cannot be toggled from `-mllvm` in 7.0.2.
* `-enable-machine-outliner` crashes clang 7.0.2 while compiling
  `src/engine/input/VirtualInput.cpp` (compiler bug, not a codegen result).

## 3. Per-residual matrix

Absolute base values, then deltas `raw/aligned` for every variant that moved
at least one residual. Positive is worse. No aligned delta is negative.

| variant | LayoutC1 | LayoutC2 | LevelScript | createShip | Radar | Status | OnTouchEnd | createMission | Gun | Hangar | SpaceLounge | Hud | Generator | OnTouchBegin | ModStationC1 | ModStationC2 | OnUpdate |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| base raw/al | 5849/2006 | 5849/2006 | 55327/17176 | 1564/452 | 8577/1484 | 722/320 | 4391/903 | 5487/774 | 463/57 | 1965/291 | 175/41 | 1325/249 | 393/52 | 29/15 | 43/12 | 43/12 | 37/109 |
| `slp-threshold=1` | +0/+5 | +0/+5 | +16/+4 | +31/+3 | +71/+6 | +33/+0 | +234/+19 | +627/+167 | +58/+5 | +464/+238 | +135/+20 | +433/+73 | +87/+10 | +73/+20 | +40/+8 | +40/+8 | +769/+134 |
| `slp-threshold=2` | -20/+105 | -20/+105 | +152/+41 | +31/+3 | +60/+4 | +34/+0 | +246/+19 | +591/+39 | +55/+5 | +429/+74 | +117/+20 | +397/+71 | +87/+10 | +57/+31 | +33/+8 | +33/+8 | +745/+121 |
| `slp-threshold=4` | +114/+1239 | +114/+1239 | +184/+51 | +59/+67 | +168/+9 | +66/+0 | +575/+19 | +8081/+269 | +148/+5 | +1076/+74 | +304/+20 | +988/+64 | +146/+8 | +139/+20 | +860/+122 | +860/+122 | +1744/+110 |
| `slp-threshold=8` | -4/+1204 | -4/+1204 | +153/+105 | +59/+67 | +168/+9 | +66/+0 | +576/+19 | +8079/+266 | +149/+5 | +1075/+74 | +304/+20 | +987/+64 | +145/+8 | +139/+20 | +861/+122 | +861/+122 | +10521/+126 |
| `tail-merge=0` | +97/+52 | +97/+52 | +118/+341 | +968/+214 | +1021/+1479 | +472/+111 | +2732/+1072 | +10651/+1641 | +1156/+141 | +6911/+1206 | +1327/+363 | +6178/+581 | +1181/+181 | +1251/+182 | +172/+63 | +172/+63 | +10760/+1258 |
| `tail-merge=1` | +97/+52 | +97/+52 | +118/+341 | +968/+214 | +1021/+1479 | +472/+111 | +2730/+1072 | +10651/+1641 | +1156/+141 | +6911/+1206 | +1329/+363 | +6178/+581 | +1181/+181 | +1251/+182 | +172/+63 | +172/+63 | +10760/+1258 |
| `tail-merge=2` | +19/+14 | +19/+14 | +64/+317 | +928/+116 | +437/+1172 | +468/+111 | +2711/+992 | +10218/+1314 | +1003/+84 | +6862/+944 | +1040/+125 | +5678/+417 | +1167/+113 | +1252/+182 | +171/+61 | +171/+61 | +10703/+1235 |
| `tail-merge=4` | +3/+23 | +3/+23 | +194/+158 | +295/+72 | +349/+681 | +475/+69 | +2704/+588 | +7417/+319 | +160/+5 | +1374/+259 | +366/+37 | +3466/+201 | +391/+27 | +1250/+182 | +127/+23 | +127/+23 | +10627/+750 |
| `inline-threshold=0` | +1/+7 | +1/+7 | +1/+20 | +39/+70 | +83/+9 | +34/+0 | +361/+19 | +702/+213 | +1287/+363 | +532/+227 | +156/+73 | +544/+65 | +86/+8 | +80/+20 | +42/+8 | +42/+8 | +1067/+110 |
| `unroll-threshold=50` | +0/+4 | +0/+4 | +19/+10 | +26/+90 | +60/+4 | +31/+0 | +246/+25 | +556/+41 | +58/+5 | +467/+76 | +120/+20 | +421/+67 | +72/+8 | +57/+31 | +34/+10 | +34/+10 | +753/+110 |
| `unroll-threshold=150` | +1/+9 | +1/+9 | +27/+31 | +34/+70 | +74/+9 | +33/+0 | +414/+19 | +626/+221 | +63/+5 | +537/+257 | +232/+79 | +810/+61 | +82/+8 | +77/+20 | +43/+26 | +43/+26 | +1325/+110 |
| `unroll-threshold=300` | +1/+8 | +1/+8 | +48/+18 | +73/+70 | +141/+9 | +73/+0 | +503/+82 | +1256/+213 | +116/+5 | +934/+223 | +245/+20 | +941/+63 | +172/+8 | +119/+20 | +79/+8 | +79/+8 | +1587/+116 |
| `load-pre=false` | -23/+438 | -23/+438 | +92/+1075 | +60/+67 | +147/+104 | +66/+0 | +1414/+79 | +10314/+1935 | +181/+47 | +6957/+426 | +284/+20 | +5108/+292 | +131/+8 | +1682/+256 | +69/+8 | +69/+8 | +4947/+779 |
| `slp-max-reg-size=1` | -4/+1204 | -4/+1204 | +156/+105 | +33/+67 | +161/+8 | +66/+0 | +567/+18 | +8077/+260 | +145/+5 | +1059/+74 | +297/+20 | +972/+63 | +146/+8 | +134/+20 | +860/+122 | +860/+122 | +10523/+126 |
| `slp-max-reg-size=2` | -4/+1204 | -4/+1204 | +156/+105 | +33/+67 | +161/+8 | +66/+0 | +567/+18 | +8077/+260 | +145/+5 | +1059/+74 | +297/+20 | +972/+63 | +146/+8 | +134/+20 | +860/+122 | +860/+122 | +10523/+126 |
| `slp-max-reg-size=4` | -4/+1204 | -4/+1204 | +156/+105 | +33/+67 | +161/+8 | +66/+0 | +567/+18 | +8077/+260 | +145/+5 | +1059/+74 | +297/+20 | +972/+63 | +146/+8 | +134/+20 | +860/+122 | +860/+122 | +10523/+126 |
| `slp-max-reg-size=8` | -4/+1204 | -4/+1204 | +156/+105 | +33/+67 | +161/+8 | +66/+0 | +567/+18 | +8077/+260 | +145/+5 | +1059/+74 | +297/+20 | +972/+63 | +146/+8 | +134/+20 | +860/+122 | +860/+122 | +10523/+126 |
| `disable-early-taildup` | +0/+0 | +0/+0 | -57/+546 | +0/+0 | +78/+1 | +0/+0 | +227/+39 | +0/+0 | +55/+5 | +428/+75 | +117/+20 | +393/+66 | +0/+0 | +58/+20 | +0/+0 | +0/+0 | +704/+110 |
| `enable-tail-merge=false` | +97/+52 | +97/+52 | +118/+341 | +968/+214 | +1021/+1479 | +472/+111 | +2732/+1072 | +10651/+1641 | +1156/+141 | +6911/+1206 | +1327/+363 | +6178/+581 | +1181/+181 | +1251/+182 | +172/+63 | +172/+63 | +10760/+1258 |

`LayoutC1` and `LayoutC2` are the same constructor body; `ModStationC1/C2`
likewise.

## 4. Interpretation

* **SLP is the dominant residual lever.** `-slp-threshold=0` is exactly
  neutral, so the `-Oz` default is 0. Any positive threshold perturbs SLP
  grouping: `Layout::Layout` gains 1,204 aligned instructions at 4/8, and
  `Level::createMission` gains 7,417–8,081 raw bytes at 4/8. The original's
  SLP grouping is the default one.
* **Inlining is already minimal.** `-inline-threshold=1` and `=5` are
  neutral; `=0` costs 26 size-exact functions and 3,151 byte-exact functions.
  `-Oz`'s effective threshold is at most 5.
* **No loop unrolling.** `-unroll-threshold=0` is neutral; any positive value
  perturbs the residuals and the gate.
* **Tail merging is on and at its default threshold.** Disabling it
  (`-tail-merge-threshold=0` or `-enable-tail-merge=false`) is the single
  largest regression measured: 3,875 size-exact and 1,338 byte-exact.
* **GVN hoisting is inert at `-Oz`** (`-enable-gvn-hoist=false` neutral), and
  `-profile-guided-section-prefix` has no effect without profile data.
* **Load-PRE and early tail-dup are real regressions** when disabled.
* The original's codegen is therefore the stock clang 7.0.2 `-Oz` pipeline at
  this tree. The remaining 17 functions are source-shape/TU-shape work, not a
  missing `-mllvm` knob.

## 5. Open questions

1. `-mllvm -slp-min-tree-size=0` was not measured (clang 7.0.2 compile
   blow-up on `Engine.cpp`, >5 min / 2.8 GB). If someone wants the last SLP
   knob, run it per-TU with a timeout.
2. `-enable-machine-outliner` crashes clang 7.0.2 on `VirtualInput.cpp`; a
   per-TU run excluding that file would tell whether the outliner is a
   plausible original flag, but the outliner is off by default and the gate
   evidence points to defaults.
3. A sample-profile build (`-fprofile-sample-use`) is the one remaining
   provenance hypothesis for the flat switch weights, but it is a
   `CMakeLists.txt` / profile-artifact change, not an `-mllvm` knob (see
   `pgo-switchweight-wave152-w9920.md`).

## 6. Reproduction

```sh
# full sweep (writes /dev/shm/w10116/results.json)
python3 _work/reconstructions/mllvm_sweep_w10116.py
```

The harness configures the scratch build with
`/dev/shm/w10116/toolchain_extra.cmake`, builds `link_so`, runs the official
verifier, and scores the 17 residuals with both metrics. It does not touch the
repo build or `tools/verify_baseline.json`.
