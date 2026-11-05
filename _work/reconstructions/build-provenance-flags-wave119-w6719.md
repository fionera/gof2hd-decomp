# Build provenance and compiler-flag research, wave 119 (w6719)

Status: research note, 2026-11-05. Worktree `/home/debian/GalaxyOnFire2/wt/w6719`,
branch `wave119-w6719`, base `47a1396bd`. Scratch work is under `/dev/shm/w6719`.
**No `src/`, `tools/`, baseline or allowlist change is landed.** The target is the
build provenance / compiler-flag question (`src/CMakeLists.txt`), and no tested
flag produced a strict gate improvement. This note is the only commit.

Canonical gate, measured before and after this pass (unchanged):

```text
verify: byte 4503  linked 4503  stubs 0  missing 0  extra 0  avg 99.93
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint CLEAN ✓   drift 0
```

Original: `sha256 d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483`,
2,187,328 B. Canonical build: `sha256 d8a16a9a816a4e082d4ff603eedf68b989abd7ed5902800c4802c57ab6b5d230`,
same size.

## 0. Bottom line

* The shipped `libgof2hdaa.so` is an **NDK r18b / clang 7.0.2 / gold 1.12**
  build: API 16, armeabi-v7a, Thumb-2, VFPv3, NEONv1, softfp, static libc++,
  `-Oz -g`, RTTI + exceptions, `-fstack-protector-strong`, `-ffunction-sections`,
  **no `-fdata-sections`**, default visibility.
* The current CMake/toolchain recipe is **correct for every flag tested here**.
  A 72-variant global matrix (including `-O0/-O1/-O2/-O3/-Os`,
  `-g0/-g1/-g2/-g3/-gline-tables-only/-gdwarf-2/3/4`, frame-pointer, aliasing,
  inlining, vectorization, stack-protector and 30+ `-mllvm` backend options)
  **does not beat 4503/4503/4524**. The only positive per-function delta in the
  whole matrix is `Generator::createMission` +1.4 fuzzy under
  `-fno-strict-aliasing`, which grows the function from 1996 B to 2024 B and
  drops the gate to 1395/4349/4411 (global) or 2045/4501/4523 (Generator.cpp
  only). It is not a gate win.
* A 378-cell per-TU matrix (14 TUs owning the 21 non-exact functions × 27 flags)
  confirms the same: **no per-TU flag improves any non-exact function without a
  size or gate regression.** The only positive cell is `Generator.cpp` +
  `-fno-strict-aliasing` (+1.4 fuzzy, 1996 → 2024 B).
* `-Oz` is uniquely best: `-O2` 1168/2157/2411, `-O3` 1167/2117/2247,
  `-Os` 1263/2650/2881, `-O1` 1033/1781/2676, `-O0` 61/253/259.
* `-g` (the default `-g2`) is load-bearing: `-g0`, `-g1` and
  `-gline-tables-only` drop to **2071/4247/4485**. `-g2`, `-g3`, `-gdwarf-2`,
  `-gdwarf-3` and `-gdwarf-4` are gate-neutral.
* The remaining 21 non-exact functions are **source-shape-driven**, not
  flag-driven. All 21 are size-exact at baseline; their diffs are dominated by register
  allocation, instruction scheduling, block order and literal-pool placement.

## 1. Baseline and artifacts

Commands:

```sh
cd /home/debian/GalaxyOnFire2/wt/w6719
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
cmake --build cmake-build-match --target drift -j6
cmake --build cmake-build-match --target lint -j6
```

Baseline:

```text
compare 4524  avg 99.93%  byte_exact 4503  linked_exact 4503  fuzzy_perfect 4503
stub_zero_size 0  orig_zero_size 0  missing 0  wrong_type 0  extra 0
data: exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0  imports: 207
RATCHET PASS
=== DRIFT: 0 field(s) across 0 class(es) ===
CLEAN ✓ (no new soft violations; hard invariants 0)
```

The 21 non-exact functions (all size-exact) are:

```text
48.6  Layout::Layout C1/C2 (7072 B)
50.1  Generator::createMission (1996 B)
58.2  LevelScript::process (57232 B)
73.5  Level::createShip (2764 B)
74.0  Radar::draw (9908 B)
80.1  MGame::OnTouchEnd (8080 B)
87.6  PlayerEgo::calcCollision (968 B)
90.7  Level::createScene (2252 B)
91.2  HangarWindow::render (9700 B)
92.3  Level::createMission (17172 B)
92.5  Status::missionCompleted (1432 B)
93.4  Gun::calcCharacterCollision (1984 B)
95.7  SpaceLounge::drawLounge (1948 B)
96.0  Hud::draw (9332 B)
98.1  MenuTouchWindow::OnTouchBegin (1896 B)
99.2  ModStation::ModStation C1/C2 (1008 B)
99.7  MenuTouchWindow::MenuTouchWindow C1/C2 (9600 B)
99.9  MGame::OnUpdate (11440 B)
```

### 1.1 Section state

Only five sections differ from the original:

| section | original | canonical | differing bytes |
|---|---:|---:|---:|
| `.text` | 1,259,464 | 1,259,464 | **89,836** |
| `.ARM.exidx` | 28,528 | 28,528 | **1,327** |
| `.ARM.extab` | 156,312 | **156,304** | size −8 |
| `.rodata` | 269,159 | **269,351** | +192 (mission table; original prefix exact) |
| `.note.gnu.build-id` | 36 | 36 | **20** |

The other 22 sections are byte-identical, including `.dynsym`, `.dynstr`,
`.hash`, `.gnu.version*`, `.rel.dyn`, `.rel.plt`, `.plt`, `.data.rel.ro`,
`.init_array`, `.fini_array`, `.dynamic`, `.got`, `.data`, `.bss`, `.comment`,
`.note.android.ident`, `.note.gnu.gold-version`, `.ARM.attributes` and
`.shstrtab`. The 27-section set is exact. `.rodata` is a strict prefix match:
the first 269,159 bytes of our `.rodata` equal the original's `.rodata`; the
extra 192 bytes are the checked-in `Status::missionCompleted` rank table.
`.ARM.extab` is 8 B short (`0x26290` vs `0x26298`), as recorded in wave 118;
`Generator::createMission` is exact at 172 B and `Level::createShip` is the
−8 B contributor.

## 2. Primary evidence

### 2.1 APK

Artifact: `/home/debian/GalaxyOnFire2/Original APK/Galaxy on Fire 2™ HD_2.0.16_APKPure.apk`
(6,389,308 B, `sha256 9cfe4484a475c9a31a336df9dea6408fbb129d88aa19083afe808ef6bca9eb19`).

```sh
unzip -p "$APK" META-INF/MANIFEST.MF | head -3
unzip -p "$APK" assets/crashlytics-build.properties
python3 /dev/shm/w6719/axml.py /dev/shm/w6719/apk/AndroidManifest.xml
python3 /dev/shm/w6719/dexinfo.py /dev/shm/w6719/apk/classes.dex
```

Measured:

| observable | value |
|---|---|
| `META-INF/MANIFEST.MF` | `Created-By: Android Gradle 3.1.2` |
| `BuildConfig.APPLICATION_ID` | `net.fishlabs.gof2hdallandroid2012` |
| `BuildConfig.BUILD_TYPE` | `release` |
| `BuildConfig.DEBUG` | `false` |
| `BuildConfig.FLAVOR` | `""` |
| `BuildConfig.VERSION_CODE` | `47947032` |
| `BuildConfig.VERSION_NAME` | `2.0.16` |
| manifest `minSdkVersion` | **16** |
| manifest `targetSdkVersion` | **27** |
| manifest `versionCode` / `versionName` | `47947032` / `2.0.16` |
| Crashlytics build timestamp | `Mon Oct 22 10:18:17 CEST 2018` |
| `classes.dex` native load order | `fmodex` → `fmodevent` → `gof2hdaa` |

The APK's `lib/armeabi-v7a/libgof2hdaa.so` is
`sha256 6d38fd767ee3b4f8174c371bce371bfc6397cf76e13c1756938b2a94658e3b58`,
the same size as the ground truth and differing in **288 bytes**: 259 bytes of
`.shstrtab` plus 29 section-header `sh_name` bytes. The ground truth is the APK
lib after `tools/reorder_shstrtab.py`, not a different build.

The APK contains **no build-system leftovers**: no `Android.mk`,
`Application.mk`, `CMakeLists.txt`, `*.cmake`, `*.gradle`, `*.ninja`, `*.o`,
`*.a`, `*.d`, `*.pbxproj`, `*.xcconfig`, `*.sln`, `*.vcxproj`, `*.pdb` or
`*.map`. The only build-system name that survives is `Android Gradle 3.1.2`.

### 2.2 ELF metadata

```sh
O=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
readelf -hW $O
readelf -nW $O
readelf -p .comment $O
readelf -AW $O
readelf -dW $O
readelf -SW $O
```

| observable | value | conclusion |
|---|---|---|
| ELF header | `ELF32 DYN ARM EABI5`, `e_flags 0x5000200` | armeabi-v7a **softfp** |
| `.note.android.ident` | API `16`, `r18b`, build `5063045` | NDK **r18b**, API 16 |
| `.comment` | clang 7.0.2 (`4751641 based on r328903`) + `GCC: (GNU) 4.9.x 20150123` | clang app TUs + GCC 4.9 libcrypto |
| `.note.gnu.gold-version` | `gold 1.12` | NDK gold linker |
| `.ARM.attributes` | ARM v7, Thumb-2, VFPv3, NEONv1, GOT-indirect, wchar_t 4, enum int, 8-byte align, unaligned v6 | `-march=armv7-a -mthumb -mfpu=neon -mfloat-abi=softfp -fPIC`; no `-fshort-enums`, no `-mno-unaligned-access` |
| `DT_NEEDED` | `liblog libGLESv2 libGLESv1_CM libEGL libandroid libfmodex libfmodevent libm libdl libc` | link-line order; FMOD before `m/dl/c` |
| `DT_FLAGS` / `DT_FLAGS_1` | `BIND_NOW` / `NOW` | `-z now` |
| `.hash`, no `.gnu.hash` | SYSV hash only | gold Android default |
| `.init_array` | 12 B (3 entries) | 3 static initializers |
| `.fini_array` | 8 B (2 entries) | 2 finalizers |
| `.text` align | 32 | 32-byte-aligned input (libcrypto SHA asm), not a flag |

Our canonical build reproduces `.comment`, `.note.android.ident`,
`.note.gnu.gold-version` and `.ARM.attributes` byte-for-byte.

### 2.3 Source-layout dumps and sibling ports

`../source file list dump/` contains only the three iOS symbol dumps
(`gof2hd_1.1.19_ios.cpp`, `gof2sd_1.0.1_ios.cpp`, `gof3d_1.1.4_ios.cpp`) and
`source file lists dump.txt`. A regex scan for
`Android.mk|Application.mk|CMakeLists.txt|*.cmake|*.gradle|ndk-build` returns
**zero hits**. The dumps are authoritative for TU membership (flat
`AE2/Classes/` engine tree, flat `Classes/Game/` game tree with only
`ParticleSystems/` and `MeshMerger/` subfolders), not for Android flags.

The sibling ports add no Android build evidence:

* `gof2-vita/CMakeLists.txt` only names `lib/armeabi/libgof2.so` as a prebuilt
  input; it is a Vita port.
* `KaamoClubModApi`, `gof2edit` and `FMOD-GOF2-Recreated` contain no Android
  NDK build files or flag definitions.

## 3. Re-derived toolchain / API / ABI / flags

The following are **proven** by the shipped ELF or the APK:

| item | value | evidence |
|---|---|---|
| NDK | **r18b** | `.note.android.ident` = API 16, `r18b`, `5063045` |
| clang | **7.0.2 (4751641 based on r328903)** | `.comment` |
| linker | **gold 1.12** | `.note.gnu.gold-version` |
| ABI | armeabi-v7a, Thumb-2, VFPv3, NEONv1, softfp | `.ARM.attributes`, `e_flags 0x5000200` |
| API | **16** | `.note.android.ident`; manifest `minSdkVersion=16` |
| optimization | **`-Oz`** | 70-variant matrix; `-O2/-O3/-Os/-O1/-O0` all much worse |
| debug | **`-g` (g2)** | `-g0/-g1/-gline-tables-only` drop to 2071/4247/4485; `-g2/-g3/-gdwarf-2/3/4` neutral |
| PIC | **`-fPIC`** | shared object, GOT-indirect, no text relocations |
| exceptions | **on** | `__gxx_personality_v0`, `__cxa_throw`, LSDA |
| RTTI | **on** | `_ZTI`/`_ZTV`/`_ZTS`, `__dynamic_cast` |
| stack protector | **`-fstack-protector-strong`** | 787 `__stack_chk_fail` calls; `-fno-stack-protector`/`-fstack-protector`/`-all` all worse |
| function sections | **`-ffunction-sections`** | 193 empty-`.text` CANTUNWIND sentinels; removing it drops the gate |
| data sections | **no `-fdata-sections`** | CMake r18b default; adding it drops the gate (prior wave-104/105 A/B) |
| visibility | **default** | 5,370 defined dynsym, all `GLOBAL DEFAULT` |
| C++ runtime | **static libc++** (`libc++abi.a` + `libandroid_support.a` + `libgcc.a`) | no `libc++_shared.so`; `__cxa_*` defined |
| macros | **`-DANDROID -DNDEBUG`** | r18b CMake Release defaults; codegen-neutral on this tree |
| link | one plain gold link, `-shared -soname libgof2hdaa.so --build-id -z now --exclude-libs=libgcc.a` | `.dynamic`, `.note.gnu.gold-version`, `.hash` only |
| post-link | GNU `strip --strip-all` then `tools/reorder_shstrtab.py` | APK lib vs ground truth = 288 `.shstrtab`/`sh_name` bytes |

The exact build driver (CMake `externalNativeBuild` vs standalone CMake vs a
custom script) is not recoverable from the stripped ELF. The two independent
fingerprints that discriminate CMake from ndk-build are **no `-fdata-sections`**
and **FMOD before the libc++/libgcc archives**; both match the r18b CMake
defaults and contradict ndk-build's defaults. This is consistent with the
wave-104/105/111 conclusions; this pass adds an independent re-derivation but
does not claim to distinguish the two CMake drivers.

## 4. Build-id proof

Gold's `--build-id` note is a 20-byte SHA-1 of the **unstripped** link output
with the 20-byte descriptor zeroed. Reproduced on our canonical link:

```sh
# take the gold command from the link chain, keep the unstripped output
python3 - <<'PY'
import sys, subprocess, re, hashlib, struct
sys.path.insert(0,'/dev/shm/w6719')
import pertu
gold = pertu.link_cmd().split(' && ')[1]
gold = re.sub(r'-o \S*libgof2hdaa\.unstripped\.so',
              '-o /dev/shm/w6719/ours_unstripped.so', gold)
subprocess.run(gold, shell=True, cwd=pertu.BD, check=True)
data = open('/dev/shm/w6719/ours_unstripped.so','rb').read()
# locate .note.gnu.build-id, zero its descriptor, SHA-1 the whole file
...
PY
```

Measured:

```text
our build-id descriptor              0a856df452482f487f4035c817264cd636f88b03
SHA1(file with descriptor zeroed)    0a856df452482f487f4035c817264cd636f88b03
SHA1(file as-is)                     f1b831393f8d884b0d60f4340a56b241cd5d5bcb
original build-id                    c011810da45777b83ed721b2a87d517efed0d310
```

This proves the original was linked with `--build-id` **without** `-s`, and
that its build-id is a hash of an unstripped gold output. Reproducing
`c011810d...` therefore requires byte-identical unstripped output (debug info,
symbol tables, original source/build paths), so the build-id is the last
residual to converge, not a flag difference.

## 5. Global flag matrix

Method: a scratch toolchain wrapper appends `GOF2_EXTRA_FLAGS` after the
canonical flags to both `CMAKE_CXX_FLAGS` and `CMAKE_C_FLAGS`:

```cmake
include("<repo>/cmake/toolchain.cmake")
set(GOF2_EXTRA_FLAGS "" CACHE STRING "extra matching flags")
if(GOF2_EXTRA_FLAGS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GOF2_EXTRA_FLAGS}" CACHE STRING "" FORCE)
  set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   ${GOF2_EXTRA_FLAGS}" CACHE STRING "" FORCE)
endif()
```

Each variant is a fresh `/dev/shm/w6719/builds/<name>` configured with
`-DGOF2_EXTRA_FLAGS=<flags>`, built with `cmake --build ... --target link_so`,
then measured with the official verifier (fast fuzzy path for the gate counts)
and with the official `verify.score()` (`autojunk=False`) for the 21 targets.

**Result: 72 variants, no variant beats 4503/4503/4524.** The full table is in
Appendix A. Highlights:

* **Gate-neutral** (exactly 4503/4503/4524, and all 21 target fuzzies unchanged):
  `armadjustjumptables`, `armloadstoreopt1`, `armnorestrictit`,
  `armpromoteconst0`, `csr0`, `ffpcontract=off/fast`, `g2`, `g3`, `gdwarf-2`,
  `gdwarf-3`, `gdwarf-4`, `misched-bottomup`, `misched-cluster`,
  `misched-regpressure`, `misched-topdown`, `noinline-functions-called-once`,
  `no-omit-frame-pointer`, `no-standalone-debug`, `no-tree-vectorize`,
  `no-vectorize`, `post-RA-scheduler`, `tail-dup-placement`,
  `tail-dup-size=1`, `-fno-data-sections` (plus the baseline). `-fno-math-errno`, `-fno-rtti` and
  `-fno-threadsafe-statics` are *target-neutral* (all 21 fuzzies unchanged) but
  not gate-neutral: their gate counts are 1356/4489/4514, 1701/4500/4524 and
  1368/4495/4520 respectively.
* **Worse**: `-O0/-O1/-O2/-O3/-Os`, `-g0/-g1/-gline-tables-only`,
  `-fomit-frame-pointer`, `-fno-inline-functions`, `-fno-strict-aliasing`,
  `-fno-slp-vectorize`, `-fno-jump-tables`, `-fno-optimize-sibling-calls`,
  `-fno-stack-protector`, `-fstack-protector`, `-fstack-protector-all`,
  `-fno-math-errno`, `-fno-threadsafe-statics`, `-fno-rtti`, `-fdata-sections`,
  `-fno-common` (link failure),
  `-mllvm -regalloc-csr-first-time-cost=1/2/4/8/16`,
  `-mllvm -misched-postra`, `-mllvm -arm-load-store-opt=0`,
  `-mllvm -arm-promote-constant=1`, `-mllvm -tail-dup-size=2/4/8`,
  `-mllvm -tail-merge-size=1/2/4`, `-mllvm -align-all-blocks=2/3/4`,
  `-mllvm -align-all-nofallthru-blocks=2/3/4`,
  `-mllvm -align-all-functions=2/3/4/5` (gate 1378–1386/4500).
* **Build/link failures**: `-fno-common` (duplicate
  `g_android_back_button_pressed`), `-mllvm -arm-implicit-it` without a value
  (clang requires `=always`/`=never`), `-gdwarf-5` (clang 7.0.2 segfault on
  `PaintCanvas.cpp`).

The two most informative negatives:

* **`-g` is load-bearing.** `-g0`, `-g1` and `-gline-tables-only` drop to
  **2071/4247/4485**; `-g2`, `-g3`, `-gdwarf-2`, `-gdwarf-3` and `-gdwarf-4`
  are exactly 4503/4503/4524. At `-Oz`, debug info keeps PostRA machine sinking
  from moving entry-block argument copies past early exits; the visible target
  is `MenuTouchWindow::OnTouchBegin` (−14.9 fuzzy at `-g0`).
* **SLP vectorization is load-bearing.** `-fno-vectorize` and
  `-fno-tree-vectorize` are neutral, but `-fno-slp-vectorize` drops to
  1492/4384/4408 and costs `Layout::Layout` −30.2 fuzzy.
* **No `-fdata-sections`.** `-fdata-sections` drops to **4314/4501/4524**;
  `-fno-data-sections` is exactly 4503/4503/4524. This independently confirms
  the r18b CMake default (ndk-build adds `-fdata-sections`).

## 6. Worst-function sensitivity

The best global variant for each target (ties go to the baseline) is:

| target | bytes | base % | best global variant | Δ |
|---|---:|---:|---|---:|
| `_ZN6LayoutC1Ev` | 7072 | 48.6 | `(base)` | +0.0 |
| `_ZN6LayoutC2Ev` | 7072 | 48.6 | `(base)` | +0.0 |
| `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | 1996 | 50.1 | `nostrictalias` | +1.4 |
| `_ZN11LevelScript7processEi` | 57232 | 58.2 | `(base)` | +0.0 |
| `_ZN5Level10createShipEiiiP8Waypointbb` | 2764 | 73.5 | `(base)` | +0.0 |
| `_ZN5Radar4drawEP6PlayerP3Hudi` | 9908 | 74.0 | `(base)` | +0.0 |
| `_ZN5MGame10OnTouchEndEiiPv` | 8080 | 80.1 | `(base)` | +0.0 |
| `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE` | 968 | 87.6 | `(base)` | +0.0 |
| `_ZN5Level11createSceneEv` | 2252 | 90.7 | `(base)` | +0.0 |
| `_ZN12HangarWindow6renderEv` | 9700 | 91.2 | `(base)` | +0.0 |
| `_ZN5Level13createMissionEv` | 17172 | 92.3 | `(base)` | +0.0 |
| `_ZN6Status16missionCompletedEbbx` | 1432 | 92.5 | `(base)` | +0.0 |
| `_ZN3Gun22calcCharacterCollisionEv` | 1984 | 93.4 | `(base)` | +0.0 |
| `_ZN11SpaceLounge10drawLoungeEv` | 1948 | 95.7 | `(base)` | +0.0 |
| `_ZN3Hud4drawExxP9PlayerEgobjj` | 9332 | 96.0 | `(base)` | +0.0 |
| `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | 1896 | 98.1 | `(base)` | +0.0 |
| `_ZN10ModStationC1Ev` | 1008 | 99.2 | `(base)` | +0.0 |
| `_ZN10ModStationC2Ev` | 1008 | 99.2 | `(base)` | +0.0 |
| `_ZN15MenuTouchWindowC1Ei` | 9600 | 99.7 | `(base)` | +0.0 |
| `_ZN15MenuTouchWindowC2Ei` | 9600 | 99.7 | `(base)` | +0.0 |
| `_ZN5MGame8OnUpdateEv` | 11440 | 99.9 | `(base)` | +0.0 |

Only `Generator::createMission` has a positive global delta: `-fno-strict-aliasing`
50.1 → 51.5 (+1.4), but the function grows 1996 → 2024 B and the global gate
falls to 1395/4349/4411. Every other target is at its baseline fuzzy under every
tested global flag.

## 7. Per-TU flag matrix

Method: compile one TU with the extra flag, swap the object into the canonical
build directory, run the exact gold/strip/reorder link chain, and score the
targets owned by that TU. The canonical objects are restored after each cell.
The full 27 × 14 matrix is in Appendix B.

**Result: 378 cells, one positive cell.** `Generator.cpp` +
`-fno-strict-aliasing` gives `Generator::createMission` 50.1 → 51.5 (+1.4), but
the function grows 1996 → 2024 B and the per-TU gate is
**2045/4501/4523** (byte/linked/size), i.e. a regression (the other
`Generator.cpp` functions also change, which is why `byte_exact` falls far
below 4503). Every other cell is zero or negative. The per-TU `-O2`/`-Os` results match the global results for
the same TU (e.g. `Layout.cpp -O2` = 44.2, `Generator.cpp -O2` = 31.5,
`LevelScript.cpp -O2` = 24.8), confirming the harness applies the flag.

The per-TU matrix also confirms the global negatives: `-fno-inline-functions`,
`-fno-slp-vectorize`, `-mllvm -arm-load-store-opt=0`,
`-mllvm -align-all-nofallthru-blocks=2`, `-mllvm -misched-postra` and the
`-O2/-Os` variants all lower the owned targets; `-fno-inline-functions-called-once`,
`-fno-vectorize`, `-mllvm -regalloc-csr-first-time-cost=1/2/4/8`,
`-mllvm -tail-dup-placement`, `-mllvm -misched-bottomup/topdown`,
`-mllvm -post-RA-scheduler`, `-mllvm -arm-adjust-jump-tables`,
`-mllvm -arm-no-restrict-it`, `-mllvm -arm-implicit-it=always/never` are
codegen-neutral for every owned target.

## 8. Proven vs hypothesis

**Proven by this pass:**

1. Baseline and final gate are 4503/4503/4524, data 843/843, imports 207,
   lint `CLEAN ✓`, drift 0, `RATCHET PASS`.
2. Toolchain/API/ABI: NDK r18b, clang 7.0.2, gold 1.12, API 16, armeabi-v7a
   softfp, Thumb-2, VFPv3, NEONv1, static libc++.
3. Gold's build-id is SHA-1 of the unstripped link output with the descriptor
   zeroed; our `0a856df4...` reproduces exactly. The original's `c011810d...`
   is therefore a hash of an unstripped output, not a flag difference.
4. `-Oz` is uniquely best among `-O0/-O1/-O2/-O3/-Os/-Oz`.
5. `-g` (g2) is load-bearing; `-g0/-g1/-gline-tables-only` drop to
   2071/4247/4485.
6. `-fdata-sections` drops to 4314/4501/4524; `-fno-data-sections` is
   4503/4503/4524, confirming the r18b CMake default.
7. No tested global flag beats 4503/4503/4524. The only positive per-function
   delta is `Generator::createMission` +1.4 under `-fno-strict-aliasing`, with a
   size regression.
8. No tested per-TU flag improves the gate. The same `Generator.cpp` +
   `-fno-strict-aliasing` cell is the only positive fuzzy delta, with a size
   regression (1996 → 2024 B) and a gate drop (2045/4501/4523).

**Still hypothesis / not settled:**

1. The exact build driver (CMake `externalNativeBuild` vs standalone CMake vs a
   custom script) is not recoverable from the stripped ELF. The evidence is
   consistent with the r18b CMake defaults (no `-fdata-sections`, FMOD before
   the STL archives) and inconsistent with ndk-build defaults.
2. The original's exact global flag list beyond the observable set
   (`-Oz -g -std=c++11 -fPIC -frtti -fexceptions -fstack-protector-strong
   -ffunction-sections -funwind-tables -DANDROID -DNDEBUG -stdlib=libc++`) is
   not recoverable; the neutral flags cannot be excluded as used-but-neutral.
3. The 21 remaining functions are source-shape-driven. This pass does not
   change that; the next work is source/TU-shape reconstruction, not another
   global-flag sweep.

## 9. Reproduction commands

```sh
# baseline
cd /home/debian/GalaxyOnFire2/wt/w6719
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
cmake --build cmake-build-match --target drift -j6
cmake --build cmake-build-match --target lint -j6

# 70-variant global matrix
python3 /dev/shm/w6719/run_variants.py          # builds /dev/shm/w6719/builds/*
python3 /dev/shm/w6719/score_many.py \
  /dev/shm/w6719/global_manifest.json /dev/shm/w6719/global_scores.json

# 378-cell per-TU matrix
python3 /dev/shm/w6719/pertu_matrix.py          # builds /dev/shm/w6719/pertu/*.so
python3 /dev/shm/w6719/score_many.py \
  /dev/shm/w6719/pertu_manifest.json /dev/shm/w6719/pertu_all_scores.json

# APK / ELF primary evidence
python3 /dev/shm/w6719/axml.py /dev/shm/w6719/apk/AndroidManifest.xml
python3 /dev/shm/w6719/dexinfo.py /dev/shm/w6719/apk/classes.dex
readelf -hW -nW -dW -SW -AW "$O"
readelf -p .comment "$O"
```

Scratch artifacts (not tracked): `/dev/shm/w6719/run_variants.py`,
`pertu.py`, `pertu_matrix.py`, `score_targets.py`, `score_many.py`,
`axml.py`, `dexinfo.py`, `builds/`, `pertu/`, `global_scores.json`,
`pertu_all_scores.json`.

The per-TU harness in full (the only non-obvious part is that Ninja must not
recompile the swapped object, so the exact gold/strip/reorder chain is run
directly):

```python
import json, shlex, subprocess, shutil
from pathlib import Path
ROOT = Path('/home/debian/GalaxyOnFire2/wt/w6719')
BD = ROOT / 'cmake-build-match'
cc = json.load(open(BD / 'compile_commands.json'))
by_src = {Path(e['file']).name: e for e in cc}

def link_cmd():
    out = subprocess.run(['ninja', '-C', str(BD), '-t', 'commands', 'link_so'],
                         capture_output=True, text=True, check=True).stdout
    return [l for l in out.splitlines() if l.strip()][-1]   # gold + strip + reorder

def run(srcname, extra, tag):
    e = by_src[srcname]
    obj = BD / e['output']
    bak = Path('/dev/shm') / f'{tag}.orig.obj'
    shutil.copy2(obj, bak)
    cmd = shlex.split(e['command'])
    cmd[cmd.index('-o') + 1] = f'/dev/shm/{tag}.o'
    cmd += shlex.split(extra)
    subprocess.run(cmd, cwd=e['directory'], check=True)
    shutil.copy2(f'/dev/shm/{tag}.o', obj)
    try:
        subprocess.run(link_cmd(), shell=True, cwd=BD, check=True)
        shutil.copy2(BD / 'libgof2hdaa.so', f'/dev/shm/{tag}.so')
    finally:
        shutil.copy2(bak, obj)
```

Target scoring uses the official verifier's own `disassemble()`, `reconcile()`
and `score()` (`autojunk=False`) from `tools/verify/verify.py`, with the
original disassembly cached per target.

## 10. Gate state

No source or build-flag change was landed. `tools/verify_baseline.json` was not
touched; `report.json` was not committed. The final `check` is unchanged:

```text
verify: byte 4503  linked 4503  stubs 0  missing 0  extra 0  avg 99.93
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint CLEAN ✓   drift 0
```

The worktree is clean.

## Appendix A: full global matrix

`max Δ` / `min Δ` are the maximum and minimum per-target fuzzy deltas over the
21 non-exact functions; `+0.0` means every target is exactly at baseline.

| variant | extra flags | byte | linked | size | max Δ | min Δ |
|---|---|---:|---:|---:|---:|---:|
| `armadjustjumptables` | `-mllvm -arm-adjust-jump-tables` | 4503 | 4503 | 4524 | +0.0 | +0.0 |
| `armloadstoreopt1` | `-mllvm -arm-load-store-opt=1` | 4503 | 4503 | 4524 | +0.0 | +0.0 |
| `armnorestrictit` | `-mllvm -arm-no-restrict-it` | 4503 | 4503 | 4524 | +0.0 | +0.0 |
| `armpromoteconst0` | `-mllvm -arm-promote-constant=0` | 4503 | 4503 | 4524 | +0.0 | +0.0 |
| `base` | `(base)` | 4503 | 4503 | 4524 | +0.0 | +0.0 |
| `csr0` | `-mllvm -regalloc-csr-first-time-cost=0` | 4503 | 4503 | 4524 | +0.0 | +0.0 |
| `ffpcontractfast` | `-ffp-contract=fast` | 4503 | 4503 | 4524 | +0.0 | +0.0 |
| `ffpcontractoff` | `-ffp-contract=off` | 4503 | 4503 | 4524 | +0.0 | +0.0 |
| `fnodata` | `-fno-data-sections` | 4503 | 4503 | 4524 | +0.0 | +0.0 |
| `g2` | `-g2` | 4503 | 4503 | 4524 | +0.0 | +0.0 |
| `g3` | `-g3` | 4503 | 4503 | 4524 | +0.0 | +0.0 |
| `gdwarf2` | `-gdwarf-2` | 4503 | 4503 | 4524 | +0.0 | +0.0 |
| `gdwarf3` | `-gdwarf-3` | 4503 | 4503 | 4524 | +0.0 | +0.0 |
| `gdwarf4` | `-gdwarf-4` | 4503 | 4503 | 4524 | +0.0 | +0.0 |
| `mischedbottomup` | `-mllvm -misched-bottomup` | 4503 | 4503 | 4524 | +0.0 | +0.0 |
| `mischedcluster` | `-mllvm -misched-cluster` | 4503 | 4503 | 4524 | +0.0 | +0.0 |
| `mischedregpressure` | `-mllvm -misched-regpressure` | 4503 | 4503 | 4524 | +0.0 | +0.0 |
| `mischedtopdown` | `-mllvm -misched-topdown` | 4503 | 4503 | 4524 | +0.0 | +0.0 |
| `noinlinefuncsonce` | `-fno-inline-functions-called-once` | 4503 | 4503 | 4524 | +0.0 | +0.0 |
| `noomitfp` | `-fno-omit-frame-pointer` | 4503 | 4503 | 4524 | +0.0 | +0.0 |
| `nostandalonedebug` | `-fno-standalone-debug` | 4503 | 4503 | 4524 | +0.0 | +0.0 |
| `notreevec` | `-fno-tree-vectorize` | 4503 | 4503 | 4524 | +0.0 | +0.0 |
| `novectorize` | `-fno-vectorize` | 4503 | 4503 | 4524 | +0.0 | +0.0 |
| `postrasched` | `-mllvm -post-RA-scheduler` | 4503 | 4503 | 4524 | +0.0 | +0.0 |
| `taildupplacement` | `-mllvm -tail-dup-placement` | 4503 | 4503 | 4524 | +0.0 | +0.0 |
| `taildupsize1` | `-mllvm -tail-dup-size=1` | 4503 | 4503 | 4524 | +0.0 | +0.0 |
| `fdata` | `-fdata-sections` | 4314 | 4501 | 4524 | +0.0 | +0.0 |
| `nounroll` | `-fno-unroll-loops` | 2885 | 4493 | 4520 | +0.0 | -15.1 |
| `csr8` | `-mllvm -regalloc-csr-first-time-cost=8` | 2353 | 4486 | 4511 | +0.0 | +0.0 |
| `g0` | `-g0` | 2071 | 4247 | 4485 | +0.0 | -14.9 |
| `g1` | `-g1` | 2071 | 4247 | 4485 | +0.0 | -14.9 |
| `gline` | `-gline-tables-only` | 2071 | 4247 | 4485 | +0.0 | -14.9 |
| `csr2` | `-mllvm -regalloc-csr-first-time-cost=2` | 2027 | 4488 | 4513 | +0.0 | +0.0 |
| `csr4` | `-mllvm -regalloc-csr-first-time-cost=4` | 1997 | 4487 | 4512 | +0.0 | +0.0 |
| `csr1` | `-mllvm -regalloc-csr-first-time-cost=1` | 1967 | 4491 | 4516 | +0.0 | +0.0 |
| `csr16` | `-mllvm -regalloc-csr-first-time-cost=16` | 1890 | 4484 | 4510 | +0.0 | +0.0 |
| `tailmergesize4` | `-mllvm -tail-merge-size=4` | 1760 | 4468 | 4500 | +0.0 | -6.6 |
| `nortti` | `-fno-rtti` | 1701 | 4500 | 4524 | +0.0 | +0.0 |
| `mischedpostra` | `-mllvm -misched-postra` | 1575 | 3980 | 4504 | +0.0 | -7.3 |
| `noslp` | `-fno-slp-vectorize` | 1492 | 4384 | 4408 | +0.0 | -30.2 |
| `nostrictalias` | `-fno-strict-aliasing` | 1395 | 4349 | 4411 | +1.4 | -23.3 |
| `taildupsize4` | `-mllvm -tail-dup-size=4` | 1392 | 4285 | 4326 | +0.0 | -12.0 |
| `alignfunc2` | `-mllvm -align-all-functions=2` | 1386 | 4500 | 4524 | +0.0 | +0.0 |
| `alignfunc3` | `-mllvm -align-all-functions=3` | 1378 | 4500 | 4524 | +0.0 | +0.0 |
| `alignfunc4` | `-mllvm -align-all-functions=4` | 1378 | 4500 | 4524 | +0.0 | +0.0 |
| `alignfunc5` | `-mllvm -align-all-functions=5` | 1378 | 4500 | 4524 | +0.0 | +0.0 |
| `taildupsize8` | `-mllvm -tail-dup-size=8` | 1378 | 4253 | 4299 | +0.0 | -14.1 |
| `tailmergesize2` | `-mllvm -tail-merge-size=2` | 1377 | 4462 | 4505 | +0.0 | -16.4 |
| `armpromoteconst1` | `-mllvm -arm-promote-constant=1` | 1372 | 4193 | 4221 | +0.0 | -43.1 |
| `stackprot` | `-fstack-protector` | 1369 | 3785 | 3794 | +0.0 | -32.5 |
| `nostackprot` | `-fno-stack-protector` | 1369 | 3781 | 3790 | +0.0 | -32.5 |
| `nothreadsafestatics` | `-fno-threadsafe-statics` | 1368 | 4495 | 4520 | +0.0 | +0.0 |
| `tailmergesize1` | `-mllvm -tail-merge-size=1` | 1368 | 4405 | 4485 | +0.0 | -0.2 |
| `nojumptables` | `-fno-jump-tables` | 1363 | 4412 | 4438 | +0.0 | -79.2 |
| `taildupsize2` | `-mllvm -tail-dup-size=2` | 1359 | 4353 | 4385 | +0.0 | -2.1 |
| `nomatherrno` | `-fno-math-errno` | 1356 | 4489 | 4514 | +0.0 | +0.0 |
| `nosibcall` | `-fno-optimize-sibling-calls` | 1316 | 3590 | 3686 | +0.0 | -21.8 |
| `omitfp` | `-fomit-frame-pointer` | 1312 | 1920 | 2162 | -6.1 | -61.1 |
| `alignnofall2` | `-mllvm -align-all-nofallthru-blocks=2` | 1281 | 3185 | 3313 | -2.1 | -24.8 |
| `Os` | `-Os` | 1263 | 2650 | 2881 | -1.4 | -58.4 |
| `alignnofall3` | `-mllvm -align-all-nofallthru-blocks=3` | 1251 | 2904 | 2947 | -2.3 | -26.4 |
| `alignnofall4` | `-mllvm -align-all-nofallthru-blocks=4` | 1251 | 2802 | 2818 | -3.3 | -31.4 |
| `armloadstoreopt0` | `-mllvm -arm-load-store-opt=0` | 1233 | 3311 | 3906 | -0.1 | -44.6 |
| `noinlinefuncs` | `-fno-inline-functions` | 1219 | 3511 | 3572 | +0.0 | -63.8 |
| `alignblock2` | `-mllvm -align-all-blocks=2` | 1190 | 2467 | 2540 | -2.9 | -27.8 |
| `alignblock4` | `-mllvm -align-all-blocks=4` | 1187 | 2347 | 2350 | -7.9 | -45.2 |
| `alignblock3` | `-mllvm -align-all-blocks=3` | 1176 | 2359 | 2379 | -4.3 | -32.5 |
| `O2` | `-O2` | 1168 | 2157 | 2411 | -4.4 | -84.3 |
| `O3` | `-O3` | 1167 | 2117 | 2247 | -12.0 | -84.4 |
| `O1` | `-O1` | 1033 | 1781 | 2676 | -23.2 | -82.4 |
| `stackprotall` | `-fstack-protector-all` | 83 | 962 | 981 | +0.0 | -31.7 |
| `O0` | `-O0` | 61 | 253 | 259 | -41.2 | -95.8 |

## Appendix B: per-TU matrix

Each cell is the maximum fuzzy delta over the targets owned by that TU. `0`
means no change; `.` means the cell was not measured. The only positive cell is
`Generator` + `nostrictalias` (+1.4), with a 1996 → 2024 B size regression.

| TU flag | Layout | Gen | LScript | Level | Radar | MGame | PEGO | Hangar | Status | Gun | SLounge | Hud | MTW | ModSt |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| `O2` | -4.4 | -18.6 | -33.4 | -38.6 | -49.3 | -53.3 | -59.8 | -43.4 | -84.3 | -31.6 | -17.7 | -43.5 | -42.8 | -8.4 |
| `Os` | -1.4 | -13.4 | -29.8 | -36.6 | -46.1 | -14.2 | -23.1 | -45.2 | -58.4 | -30.6 | -17.5 | -34.7 | -43.0 | -5.8 |
| `g0` | 0 | -0.1 | 0 | 0 | 0 | 0 | -0.2 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| `g1` | 0 | -0.1 | 0 | 0 | 0 | 0 | -0.2 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| `noinlinefuncs` | 0 | -11.4 | -26.0 | -15.9 | -15.6 | -22.9 | -25.0 | -38.5 | -63.8 | -45.8 | -31.3 | 0 | -20.7 | -53.8 |
| `noinlinefuncsonce` | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| `nostrictalias` | 0 | +1.4 | -1.1 | 0 | -5.1 | -5.7 | 0 | 0 | 0 | 0 | 0 | 0 | -7.2 | 0 |
| `novectorize` | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| `noslp` | -30.2 | 0 | -0.2 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | -25.4 |
| `mischedpostra` | -1.8 | 0 | -0.2 | 0 | -0.3 | -0.1 | -0.5 | -0.5 | 0 | -0.3 | -0.6 | -7.3 | -0.4 | -0.6 |
| `csr1` | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| `csr2` | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| `csr4` | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| `csr8` | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| `taildupplacement` | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| `armpromoteconst1` | 0 | 0 | -4.1 | 0 | -0.3 | -7.5 | 0 | -19.9 | 0 | 0 | -4.0 | -8.0 | 0 | 0 |
| `armloadstoreopt0` | -26.0 | -0.1 | -14.7 | -8.7 | -8.3 | -4.1 | -4.6 | -12.1 | -0.7 | -6.9 | -16.2 | -26.9 | -11.1 | -44.6 |
| `alignnofall2` | -24.2 | -2.1 | -2.2 | -9.3 | -3.4 | -10.2 | -3.0 | -12.1 | -24.8 | -11.0 | -13.2 | -12.3 | -10.1 | -8.4 |
| `taildupsize2` | 0 | 0 | -0.1 | 0 | -1.7 | -0.8 | 0 | -2.1 | 0 | 0 | 0 | 0 | 0 | -0.4 |
| `tailmergesize2` | 0 | 0 | -0.2 | 0 | 0 | 0 | 0 | 0 | -16.4 | 0 | 0 | 0 | 0 | 0 |
| `mischedbottomup` | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| `mischedtopdown` | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| `postrasched` | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| `armadjustjumptables` | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| `armnorestrictit` | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| `armimplicititalways` | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| `armimplicititnever` | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
