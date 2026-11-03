# Build provenance: the original `libgof2hdaa.so` was built with CMake + NDK r18b (w4208)

Status: research note, 2026-11-03. Worktree `wt/w4208`, branch `wave92-w4208`, base
`b379d00d`. Scratch is `/dev/shm/w4208`. **No `src/` or build-flag change is landed**:
the note is the deliverable. Baseline and final gate are identical (see §2).

This note reconstructs the original build system from the shipped binary, the APK, the
NDK r18b build files, and controlled A/B rebuilds. It supersedes the "ndk-build with two
deviations" reading of `_work/reconstructions/definitive_build_recipe_w3697.md`: both
"deviations" are exactly the **NDK r18b CMake toolchain defaults**.

## 0. Bottom line

* The original is **NDK r18b, clang 7.0.2, gold 1.12, API 16, armeabi-v7a softfp,
  Thumb-2, NEON, `c++_static`**, built on **macOS/darwin**.
* The effective compile and link recipe is **the NDK r18b
  `build/cmake/android.toolchain.cmake` default set**, with
  `ANDROID_ABI=armeabi-v7a`, `ANDROID_PLATFORM=android-16`,
  `ANDROID_ARM_NEON=ON`, `ANDROID_STL=c++_static`,
  `ANDROID_CPP_FEATURES="rtti exceptions"`, `CMAKE_BUILD_TYPE=Release`,
  `CMAKE_CXX_STANDARD=11`.
* The two properties that made w3697 call the build "non-vanilla ndk-build" are
  CMake-toolchain defaults, not custom deviations:
  1. **No `-fdata-sections`.** The CMake toolchain adds `-ffunction-sections` but not
     `-fdata-sections`; ndk-build's `GLOBAL_CFLAGS` adds both.
  2. **FMOD before `libc++abi.a`, `libunwind.a` not pulled.** CMake appends the STL
     archives (`libc++_static.a libc++abi.a libandroid_support.a libunwind.a`) *after*
     `target_link_libraries`; ndk-build puts static archives before shared libraries.
     `libfmodex.so`/`libfmodevent.so` define `_Unwind_*`, so if they precede
     `libc++abi.a` those references bind to the shared library and `libunwind.a` is
     never extracted. That is exactly the original's `_Unwind_* = UND` signature.
* The repo's current `cmake/toolchain.cmake` + `CMakeLists.txt` reproduce the original's
  **compile flags and link inputs**; the only reconstruction aid is the
  `--section-ordering-file` (the original was a plain CMake link in original source
  order). No flag in the tree is provably wrong, so no build change is landed.

## 1. Method and baseline

Commands:

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8
cmake --build cmake-build-match --target lint  -j8
```

Before and after this note:

```text
verify: byte 4498  linked 4498  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
=== DRIFT: 0 field(s) across 0 class(es) ===
CLEAN ✓ (no new soft violations; hard invariants 0)
```

`size-exact` 4524/4524; `.text 0x1337c8 @ 0x69440`, `.rodata 0x41b67 @ 0x1c9e10`,
`.ARM.exidx 0x6f70`, `.ARM.extab 0x26290` (8 bytes short of the original's `0x26298`,
a source residue), `.data 0x296c`, `.bss 0x69ac`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`,
sha256 `d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483`,
2,187,328 bytes. The shipped APK lib is the same file after GNU strip (§8).

## 2. ELF evidence (all read from the original)

| observable | value |
|---|---|
| ELF header | `ELF32`, `DYN`, `ARM`, `EABI5`, flags `0x5000200` (`EF_ARM_ABI_FLOAT_SOFT`) |
| `.comment` | `Android (4751641 based on r328903) clang version 7.0.2 ... (based on LLVM 7.0.2svn)` and `GCC: (GNU) 4.9.x 20150123 (prerelease)` |
| `.note.android.ident` | API `16`, `r18b`, build `5063045` (r18b `source.properties`: `Pkg.Revision = 18.1.5063045`) |
| `.note.gnu.gold-version` | `gold 1.12` |
| `.note.gnu.build-id` | 20-byte SHA-1 style (`c011810d...`), `--build-id` |
| `.ARM.attributes` | `ARM v7`, Application profile, Thumb-2, `VFPv3`, `NEONv1`, GOT-indirect, wchar_t 4, enum int, 8-byte align, `Tag_CPU_unaligned_access: v6`, no `Tag_ABI_VFP_args` |
| section set (27) | `.note.android.ident .note.gnu.build-id .dynsym .dynstr .hash .gnu.version .gnu.version_d .gnu.version_r .rel.dyn .rel.plt .plt .text .ARM.exidx .ARM.extab .rodata .fini_array .data.rel.ro .init_array .dynamic .got .data .bss .comment .note.gnu.gold-version .ARM.attributes .shstrtab` |
| hash style | `.hash` only, **no `.gnu.hash`** (gold 1.12 Android default) |
| `.init_array` / `.fini_array` | 3 / 2 entries; bytes identical to our build (`__GLOBAL__sub_I_Engine.cpp`, `ParticleSettings.cpp`, `LODManager.cpp`) |
| `DT_NEEDED` | `liblog libGLESv2 libGLESv1_CM libEGL libandroid libfmodex libfmodevent libm libdl libc` |
| `DT_SONAME` | `libgof2hdaa.so` |
| `DT_FLAGS` / `DT_FLAGS_1` | `BIND_NOW` / `NOW`; `DT_RELCOUNT 1162` |
| relocations | `R_ARM_JUMP_SLOT 3674`, `R_ARM_ABS32 1817`, `R_ARM_RELATIVE 1162`, `R_ARM_GLOB_DAT 614`; **no `R_ARM_REL32`**, no `DT_TEXTREL` |
| program headers | `PT_GNU_RELRO` covers `.fini_array .data.rel.ro .init_array .dynamic .got`; `PT_GNU_STACK` `RW`; `ARM_EXIDX` segment `0x6f70` |
| `.ARM.exidx` | 3566 entries, 272 CANTUNWIND; 193 CANT+real pairs (gold `fix_exidx_coverage` empty-`.text` sentinels) |
| defined / undefined `.dynsym` | 5373 / 208 (207 import names after dedup) |
| `.rel.plt` section header | `sh_flags=AI`, `sh_info=20` (darwin-host gold; our linux gold emits `A`/`0`) |

The `.ARM.attributes` match is direct evidence for
`-march=armv7-a -mthumb -mfpu=neon -mfloat-abi=softfp`; the ELF header's soft-float
ABI flag and the absence of `Tag_ABI_VFP_args` exclude `-mfloat-abi=hard`.

## 3. The build system: CMake, not ndk-build

### 3.1 What ndk-build r18b emits (controlled sample)

A minimal `ndk-build` project with `APP_ABI := armeabi-v7a`,
`APP_PLATFORM := android-16`, `APP_STL := c++_static`, `APP_OPTIM := release`,
`LOCAL_ARM_NEON := true`, `LOCAL_CPP_FEATURES := rtti exceptions` produced:

```text
clang++ -target armv7-none-linux-androideabi16 \
  -fdata-sections -ffunction-sections -fstack-protector-strong -funwind-tables \
  -no-canonical-prefixes -g \
  -Wno-invalid-command-line-argument -Wno-unused-command-line-argument \
  -fno-exceptions -fno-rtti \
  -gcc-toolchain $NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64 \
  -fpic -march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16 \
  -mthumb -Oz -DNDEBUG -mfpu=neon \
  -I$NDK/sources/cxx-stl/llvm-libc++/include \
  -I$NDK/sources/cxx-stl/llvm-libc++abi/include \
  -I$NDK/sources/android/support/include \
  -std=c++11 -DANDROID -Wa,--noexecstack -Wformat -Werror=format-security \
  -frtti -fexceptions --sysroot=$NDK/sysroot \
  -isystem $NDK/sysroot/usr/include/arm-linux-androideabi \
  -c file.cpp -o file.o
```

Link line (abridged):

```text
clang++ -Wl,-soname,libt.so -shared --sysroot=$NDK/platforms/android-16/arch-arm \
  <objects> \
  libc++_static.a libc++abi.a libandroid_support.a libunwind.a \
  -lgcc -Wl,--exclude-libs,libgcc.a -latomic -Wl,--exclude-libs,libatomic.a \
  ... -Wl,--fix-cortex-a8 -Wl,--exclude-libs,libunwind.a -Wl,--build-id \
  -nostdlib++ -Wl,--no-undefined -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now \
  -Wl,--warn-shared-textrel -Wl,--fatal-warnings \
  -L$NDK/platforms/android-16/arch-arm/usr/lib -llog -ldl -lc -lm -o libt.so
```

Key point: ndk-build puts **static archives before shared libraries**, and always adds
`-fdata-sections`. Both are contradicted by the original.

### 3.2 What the NDK r18b CMake toolchain emits (controlled sample)

A minimal CMake project configured with
`-DCMAKE_TOOLCHAIN_FILE=$NDK/build/cmake/android.toolchain.cmake
-DANDROID_ABI=armeabi-v7a -DANDROID_PLATFORM=android-16 -DANDROID_ARM_NEON=ON
-DANDROID_STL=c++_static -DANDROID_CPP_FEATURES="rtti exceptions"
-DCMAKE_BUILD_TYPE=Release` produced:

```text
clang++ --target=armv7-none-linux-androideabi16 \
  --gcc-toolchain=$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64 \
  --sysroot=$NDK/sysroot -Dt_EXPORTS \
  -isystem $NDK/sources/cxx-stl/llvm-libc++/include \
  -isystem $NDK/sources/android/support/include \
  -isystem $NDK/sources/cxx-stl/llvm-libc++abi/include \
  -isystem $NDK/sysroot/usr/include/arm-linux-androideabi \
  -g -DANDROID -ffunction-sections -funwind-tables -fstack-protector-strong \
  -no-canonical-prefixes -march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16 \
  -mthumb -mfpu=neon -Wa,--noexecstack -Wformat -Werror=format-security \
  -std=c++11 -frtti -fexceptions -Oz -DNDEBUG -std=gnu++11 -fPIC \
  -c main.cpp -o main.cpp.o
```

Link line (abridged; note the order):

```text
clang++ --target=armv7-none-linux-androideabi16 ... -fPIC ... -g -DANDROID \
  -ffunction-sections -funwind-tables -fstack-protector-strong \
  -no-canonical-prefixes -march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16 \
  -mthumb -mfpu=neon -Wa,--noexecstack -Wformat -Werror=format-security \
  -std=c++11 -frtti -fexceptions -Oz -DNDEBUG \
  -Wl,--exclude-libs,libgcc.a -Wl,--exclude-libs,libatomic.a -nostdlib++ \
  --sysroot=$NDK/platforms/android-16/arch-arm \
  -Wl,--build-id -Wl,--warn-shared-textrel -Wl,--fatal-warnings \
  -Wl,--fix-cortex-a8 -Wl,--exclude-libs,libunwind.a \
  -L$NDK/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a -Wl,--no-undefined \
  -Wl,-z,noexecstack -Qunused-arguments -Wl,-z,relro -Wl,-z,now \
  -shared -Wl,-soname,libt.so -o libt.so main.cpp.o \
  <target_link_libraries in user order> \
  -latomic -lm libc++_static.a libc++abi.a libandroid_support.a libunwind.a -ldl
```

The CMake sample's `DT_NEEDED` order for
`target_link_libraries(t libfmodex.so libfmodevent.so log GLESv2 GLESv1_CM EGL android m dl)`
was `libfmodex libfmodevent liblog libGLESv2 libGLESv1_CM libEGL libandroid libm libdl libc`,
and its `_Unwind_*` symbols stayed **`UND`** (bound to FMOD) because FMOD precedes
`libc++abi.a`/`libunwind.a`.

### 3.3 The discriminating A/B

| property | original | ndk-build r18b | CMake r18b toolchain |
|---|---|---|---|
| `-fdata-sections` | **absent** (data layout matches no-`-fdata-sections`) | present | **absent** |
| `-fPIC` vs `-fpic` | either (codegen identical) | `-fpic` | **`-fPIC`** |
| `-Oz` release | yes | yes (thumb) | **yes (thumb)** |
| `-g` | yes (codegen-relevant) | yes | **yes** |
| STL archives vs shared libs | STL after FMOD | STL before FMOD | **STL after `target_link_libraries`** |
| `_Unwind_*` | `UND` (from FMOD) | defined hidden by `libunwind.a` | **`UND`** |
| `__gnu_Unwind_Find_exidx` | absent | `UND` | **absent** |
| `.text` | `0x1337c8` | `0x135d0c` (vanilla order) | **`0x1337c8`** |

Controlled measurements at the current 4498 baseline:

| variant | byte_exact | linked_exact | note |
|---|---:|---:|---|
| canonical tree build | **4498** | **4498** | `.so` sha `2283c01c…6527` |
| `-fpic` (instead of `-fPIC`) | 4498 | 4498 | `.so` byte-identical |
| `-fdata-sections` | **4334** | 4498 | `.rodata` shrinks, `.data` reorders |
| `-DANDROID -DNDEBUG -no-canonical-prefixes -Wa,--noexecstack` | 4498 | 4498 | CMake-authentic, neutral |
| `-std=gnu++11` | 4498 | 4498 | CMake's effective standard, neutral |

The `-fdata-sections` result is the clean discriminator: the original's `.rodata`,
`.data` and `.data.rel.ro` are byte-identical to the no-`-fdata-sections` build and
diverge massively with it (w3697 measured 113,935 / 3,273 / 405 differing bytes at the
4496 baseline). ndk-build cannot produce that layout without an explicit
`-fno-data-sections`; the CMake toolchain produces it by default.

## 4. Compile-flag -> evidence -> verdict

| flag | evidence in the original | verdict |
|---|---|---|
| `-target armv7-none-linux-androideabi16` | `.note.android.ident` API 16; `.ARM.attributes` v7; `DT_NEEDED` bionic | **proven** |
| `-march=armv7-a` | `Tag_CPU_arch: v7`, Application profile | **proven** |
| `-mthumb` | `Tag_THUMB_ISA_use: Thumb-2`; 152 `tbb`/`tbh`; Thumb prologues | **proven** |
| `-mfpu=neon` | `Tag_FP_arch: VFPv3`, `Tag_Advanced_SIMD_arch: NEONv1` (not `VFPv3-D16`) | **proven** |
| `-mfloat-abi=softfp` | ELF flags `0x5000200` soft-float ABI; no `Tag_ABI_VFP_args` | **proven** |
| `-mno-unaligned-access` | `Tag_CPU_unaligned_access: v6` (would be `None`) | **not used** (A/B 1404/4443) |
| `-Oz` | 4498 byte-exact at `-Oz`; w3697: `-Os` 1263, `-O1` 1033, `-O2` 1168, `-O3` 1167 | **proven** |
| `-g` | w3697: `-g0`/`-g1`/`-gline-tables-only` drop to 2069; `-g`/`-g2`/`-g3` identical | **proven** (codegen-relevant at `-Oz`) |
| `-ffunction-sections` | 193 CANT+real `.ARM.exidx` sentinels (gold `fix_exidx_coverage` for the empty plain `.text` every such object carries); no-flag rebuild 52 pairs | **proven** |
| `-funwind-tables` | 3566 `.ARM.exidx` entries, 2438 extab/inline records, `__gxx_personality_v0` | **proven** |
| `-fstack-protector-strong` | 788 `__stack_chk_fail` references; canary loaded from GOT `__stack_chk_guard`; `-fno-stack-protector` 1369, `-all` 83 | **proven** (strong; exact level from NDK default) |
| `-fPIC` / `-fpic` | GOT-indirect globals, `R_ARM_GLOB_DAT`, no `R_ARM_REL32`/`DT_TEXTREL`; A/B identical | **either** (CMake uses `-fPIC`) |
| `-frtti` | `_ZTI*`/`_ZTS*`/`_ZTV*`, `__cxxabiv1::__class_type_info`; `-fno-rtti` 1725/4498 | **proven** |
| `-fexceptions` | `__gxx_personality_v0`, `__cxa_throw`, `__cxa_begin_catch`, `_Unwind_Resume`, `.ARM.extab` personalities; `-fno-exceptions` 1725/4498 | **proven** |
| `-fno-threadsafe-statics` | `__cxa_guard_acquire`/`release`/`abort` exported, 39 guard calls; flag off -> 1368/4494, missing 3 | **not used** (threadsafe statics ON) |
| `-fno-use-cxa-atexit` | `__cxa_atexit@LIBC` imported, 3 calls; flag off -> 1356/4497 | **not used** (default ON) |
| `-fno-sized-deallocation` | only `_ZdlPv@plt`/`_ZdaPv@plt`; no `_ZdlPvj`/`_ZdaPvj` references | **proven** (C++11 default) |
| `-fno-jump-tables` | 152 `tbb`/`tbh`; flag off -> 1363/4410 | **not used** (jump tables ON) |
| `-fno-strict-aliasing` | flag on -> 1371/4348 | **not used** |
| `-fwrapv` | flag on -> 1398/4462 | **not used** |
| `-fmerge-constants` | flag off neutral (4498/4498); `.rodata` exact | **likely default** (unobservable) |
| `-fdata-sections` | flag on -> 4334/4498; `.rodata`/`.data` reorder | **not used** (CMake default) |
| `-fno-common` | repo C TUs have duplicate tentative `g_android_back_button_pressed`; `-fno-common` link-fails | **likely default `-fcommon`** (source-dependent) |
| `-fvisibility=hidden` | 5373 defined exports incl. inline/template symbols | **not used** (default visibility) |
| `-std=c++11` / `gnu++11` | signed `new[]` guard (`orr rX, n, n, asr #31`); A/B `-std=gnu++11` neutral | **proven C++11-era** |
| `-fuse-init-array` | `.init_array` 3 entries / `.fini_array` 2; no `.ctors`/`.dtors` | **proven** (default) |
| LTO | per-TU empty-`.text` sentinels and TU-local codegen; no LTO-specific symbols | **likely no LTO** |
| ICF | `.text` size exact without ICF; distinct functions at distinct addresses | **no ICF** |
| `--gc-sections` | all functions retained; CMake adds it only for executables | **not used** |

## 5. Link-flag -> evidence -> verdict

| link property | evidence | verdict |
|---|---|---|
| `-shared -soname libgof2hdaa.so` | `DT_SONAME` | **proven** |
| `--build-id` | 20-byte SHA-1 `.note.gnu.build-id` | **proven** |
| `-z now` (+ implied RELRO) | `DT_FLAGS=BIND_NOW`, `DT_FLAGS_1=NOW`, `.got` inside `PT_GNU_RELRO`; removing it 2932/4496 | **proven** |
| `-z relro` | `PT_GNU_RELRO` covers `.fini_array .data.rel.ro .init_array .dynamic .got` | **proven** |
| `-z noexecstack` | `PT_GNU_STACK RW`, no `E` | **proven** |
| `--exclude-libs=libgcc.a` | no libgcc symbols exported; `__gnu_ldivmod_helper` stays `UND` | **likely** (no member pulled; CMake/ndk-build default) |
| `--exclude-libs=libatomic.a`, `--exclude-libs=libunwind.a` | no libatomic/libunwind symbols exported | **likely** (CMake defaults) |
| `-Wl,--fix-cortex-a8` | no Cortex-A8 veneers needed; A/B neutral | **likely** (CMake/ndk-build default) |
| `-Wl,--no-undefined` | all `UND` names resolve from NEEDED shared libs | **likely** (CMake default) |
| `-Wl,--warn-shared-textrel`, `--fatal-warnings` | no text relocations; no output trace | **likely** (CMake/ndk-build default) |
| `-nostdlib++` | `DT_NEEDED` has no `libstdc++.so` | **proven** |
| `-latomic` | no libatomic symbols pulled | **likely** (CMake default) |
| `-lc -lm -ldl` | `DT_NEEDED` order `... m, dl, c` | **proven** |
| `-Wl,-s` (gold strip) | ground-truth `.shstrtab` is gold Stringpool order, no `.symtab` | **likely** (repo uses it; APK was separately GNU-stripped) |
| `--hash-style=sysv` | `.hash` only, no `.gnu.hash` | **proven** (gold Android default; no flag needed) |
| `--whole-archive` zlib/libzip | full zlib/libzip export set (`inflate`, `deflate`, `zip_open`, ...) | **proven** (or sources compiled in-tree) |
| no `--gc-sections` | all functions present | **proven** |
| no `--version-script` / no second pass | exports are the natural link set | **proven** |

## 6. Host, APK, and strip

* **Host = macOS/darwin.** The original `.rodata` contains the darwin buildbot path
  `/Volumes/Android/buildbot/src/android/ndk-release-r18/external/libcxx/../../external/libcxxabi/src/abort_message.cpp`
  (the linux package has `/buildbot/...`, 16 bytes shorter). The darwin `libc++abi.a`
  is therefore forced (w3904). The `.rel.plt` section header (`sh_flags=AI`,
  `sh_info=20`) is also the darwin-host gold form; our linux gold emits `A`/`0` and no
  tested link flag changes it.
* **APK manifest** (`AndroidManifest.xml`, parsed from binary AXML):
  `package=net.fishlabs.gof2hdallandroid2012`, `versionCode=47947032`,
  `versionName=2.0.16`, `minSdkVersion=16`, `targetSdkVersion=27`.
  Native libs: only `lib/armeabi-v7a/{libgof2hdaa.so, libfmodex.so, libfmodevent.so,
  libcrashlytics.so}`. This proves `APP_ABI=armeabi-v7a` (not `all`) and that the
  compile/link platform is API 16 (minSdk), while targetSdk 27 is a Java-only value.
* **Strip.** The shipped APK lib is
  `arm-linux-androideabi-strip --strip-unneeded` applied to the ground-truth file:
  `strip(ground_truth) == APK lib` (sha256 `6d38fd76…3b58`), and the 288-byte delta is
  entirely `.shstrtab` content (gold Stringpool order -> section-header order). The
  ground truth itself is the gold `-s` output. `llvm-strip` leaves the file unchanged
  (it does not rewrite `.shstrtab`), so the strip tool is GNU binutils strip, as in the
  NDK. The original link may have used gold `-s` or a separate strip; the APK is
  consistent with either, and the repo's gold `-s` reproduces the ground-truth
  `.shstrtab` byte-for-byte.

## 7. Reconstructed build

### 7.1 CMake (the proven recipe)

```cmake
cmake_minimum_required(VERSION 3.20)
project(gof2 C CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(gof2 SHARED
    # original flat source order: Classes/*.cpp, Classes/Game/*.cpp,
    # Classes/Game/ParticleSystems/*.cpp, Classes/Game/MeshMerger/*.cpp,
    # Classes/Shader/*.cpp, ... (the order is the .text order)
)

# zlib / libzip were exported whole; libcrypto is a partial archive.
target_link_libraries(gof2
    zlib zip crypto
    log GLESv2 GLESv1_CM EGL android
    ${APK}/lib/armeabi-v7a/libfmodex.so
    ${APK}/lib/armeabi-v7a/libfmodevent.so
    m dl)
```

Configure with:

```sh
cmake -S . -B build -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=$NDK/build/cmake/android.toolchain.cmake \
  -DANDROID_ABI=armeabi-v7a \
  -DANDROID_PLATFORM=android-16 \
  -DANDROID_ARM_NEON=ON \
  -DANDROID_ARM_MODE=thumb \
  -DANDROID_STL=c++_static \
  -DANDROID_CPP_FEATURES="rtti exceptions" \
  -DCMAKE_BUILD_TYPE=Release
```

`ANDROID_PLATFORM=android-16` is also the toolchain's default (r18b minimum), so it may
have been omitted. `ANDROID_ARM_NEON=ON` is **not** the default at API 16 (the toolchain
defaults NEON on only for API >= 23), so the project must have set it (or added
`-mfpu=neon`).

### 7.2 ndk-build equivalent (for comparison; not the original)

If one insists on `Android.mk`/`Application.mk`, the following reproduces the compile
flags but needs two non-default adjustments to match the link:

```make
# jni/Application.mk
APP_ABI      := armeabi-v7a
APP_PLATFORM := android-16
APP_OPTIM    := release
APP_STL      := c++_static
APP_CPPFLAGS := -frtti -fexceptions
# ndk-build adds -fdata-sections; the original does not have it.
APP_CFLAGS   := -fno-data-sections
```

```make
# jni/Android.mk
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE    := gof2hdaa
LOCAL_SRC_FILES := <original flat source list>
LOCAL_ARM_NEON  := true
LOCAL_CPP_FEATURES := rtti exceptions
LOCAL_STATIC_LIBRARIES := zlib zip crypto
LOCAL_LDLIBS := -llog -lGLESv2 -lGLESv1_CM -lEGL -landroid \
                -L$(FMOD_DIR) -lfmodex -lfmodevent -lm -ldl
include $(BUILD_SHARED_LIBRARY)
```

Even with `-fno-data-sections`, vanilla ndk-build still puts the STL archives before
the shared libraries, so it pulls `libunwind.a` and defines `_Unwind_*` locally
(1345 byte-exact, `.text 0x135d0c`). To match the original with ndk-build one must also
force the link order (e.g. `APP_STL := none` + manual `libc++abi.a` after FMOD, or a
custom link rule). That is exactly the CMake ordering, which is why CMake is the
parsimonious reconstruction.

### 7.3 Exact link line (original CMake toolchain)

```text
clang++ --target=armv7-none-linux-androideabi16 \
  --gcc-toolchain=$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/darwin-x86_64 \
  --sysroot=$NDK/sysroot -fPIC \
  -isystem $NDK/sysroot/usr/include/arm-linux-androideabi \
  -g -DANDROID -ffunction-sections -funwind-tables -fstack-protector-strong \
  -no-canonical-prefixes -march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16 \
  -mthumb -mfpu=neon -Wa,--noexecstack -Wformat -Werror=format-security \
  -std=c++11 -frtti -fexceptions -Oz -DNDEBUG -std=gnu++11 \
  -Wl,--exclude-libs,libgcc.a -Wl,--exclude-libs,libatomic.a -nostdlib++ \
  --sysroot=$NDK/platforms/android-16/arch-arm \
  -Wl,--build-id -Wl,--warn-shared-textrel -Wl,--fatal-warnings \
  -Wl,--fix-cortex-a8 -Wl,--exclude-libs,libunwind.a \
  -L$NDK/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a -Wl,--no-undefined \
  -Wl,-z,noexecstack -Qunused-arguments -Wl,-z,relro -Wl,-z,now \
  -shared -Wl,-soname,libgof2hdaa.so -o libgof2hdaa.so \
  crtbegin_so.o <objects in original source order> \
  --whole-archive libz.a libzip.a --no-whole-archive libcrypto.a \
  -llog -lGLESv2 -lGLESv1_CM -lEGL -landroid \
  $APK/lib/armeabi-v7a/libfmodex.so $APK/lib/armeabi-v7a/libfmodevent.so \
  -lm -ldl \
  -latomic -lm \
  libc++_static.a libc++abi.a libandroid_support.a libunwind.a -ldl \
  crtend_so.o
```

The repo's shorter gold line is byte-equivalent: `libc++_static.a`, `libunwind.a`,
`libatomic.a` are not pulled, so omitting them changes nothing. The only reconstruction
aid in the repo's line is `--section-ordering-file` (the original was a plain
input-order link).

## 8. What is new vs. w3697, and what remains

w3697 (`definitive_build_recipe_w3697.md`) established:
`-fdata-sections` absent, FMOD before `libc++abi.a`, `_Unwind_*` UND, and the exact
gold line. It interpreted these as **deviations from ndk-build** and left the build
system unidentified ("custom build script ... or ndk-build with
`LOCAL_CFLAGS := -fno-data-sections`"). This note shows both are the **NDK r18b CMake
toolchain defaults**, so the original build system is CMake (or a script replicating
that toolchain exactly), not ndk-build.

Still not provable from the stripped `.so`:

1. Whether the project literally used CMake or a hand-written build script that copied
   the CMake toolchain flags. The effective recipe is the CMake toolchain's.
2. Whether the link used gold `-s` or a separate GNU strip; the APK is consistent with
   either, and the ground truth is a gold `-s` artifact.
3. `-fno-common` vs the default `-fcommon` (source-dependent; the repo's C TUs have a
   duplicate tentative definition).
4. `-fmerge-constants` (neutral A/B; clang default).
5. The exact original source-file order inside CMake's `add_library` (recovered from the
   original `.text`/`.dynsym` by `tools/gen_link_order.py`).
6. Whether zlib/libzip were `--whole-archive` static libs or compiled directly into the
   app (same export set and `.text` order either way).

The remaining `.ARM.extab` -8 bytes and `.rel.plt` `sh_flags`/`sh_info` are source and
host-gold residues, not build-system flags (see `docs/IDENTICALITY.md`).

## 9. Reproduction

```sh
# baseline / final gate
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8
cmake --build cmake-build-match --target lint  -j8

# CMake-toolchain sample (shows the default flags and link order)
cmake -S /dev/shm/w4208/cmaketest -B /dev/shm/w4208/cmaketest/build -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=$NDK/build/cmake/android.toolchain.cmake \
  -DANDROID_ABI=armeabi-v7a -DANDROID_PLATFORM=android-16 \
  -DANDROID_ARM_NEON=ON -DANDROID_STL=c++_static \
  -DANDROID_CPP_FEATURES='rtti exceptions' -DCMAKE_BUILD_TYPE=Release
cmake --build /dev/shm/w4208/cmaketest/build -v

# ndk-build sample (shows -fdata-sections and static-before-shared order)
$NDK/ndk-build -C /dev/shm/w4208/ndktest \
  NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=jni/Android.mk \
  NDK_APPLICATION_MK=jni/Application.mk V=1 -B

# A/B flag sweep (wrapper appends GOF2_EXTRA_FLAGS to C and C++ flags)
cmake -S . -B /dev/shm/w4208/build-sweep -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/dev/shm/w4208/toolchain_extra.cmake \
  -DGOF2_MATCH_OPT=-Oz -DGOF2_EXTRA_FLAGS='-fdata-sections' \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build /dev/shm/w4208/build-sweep --target link_so -j8
python3 tools/verify/verify.py --build-dir /dev/shm/w4208/build-sweep/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /dev/shm/w4208/build-sweep/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
```
