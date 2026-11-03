# Independent build-provenance and flag audit (w3750)

Status: research note, 2026-11-03. Worktree `wt/w3750`, branch `wave87-w3750`,
base `12b42664` ("Ship getCargoValue order pass").  Every number below is from a
command run against `../_work/bins/android_2.0.16_libgof2hdaa.so`, the shipped
APK, or a controlled A/B rebuild/relink of this tree.  No source or build-flag
change is landed: the current flags are already the best measured set.  This
commit contains this note only.

The worktree is left clean; the scratch build directories are under
`cmake-build-w3750-*` (gitignored).  `/tmp` was inode-full on the build host, so
all scratch uses `TMPDIR=$ROOT/cmake-build-w3750-harness/tmp`.

## 0. Bottom line

* **Toolchain: NDK r18b clang 7.0.2 + gold 1.12, API 16, armeabi-v7a softfp,
  Thumb-2, NEON, `c++_static`.**  This is proven directly by the ELF:
  `.comment`, `.note.android.ident`, `.note.gnu.gold-version`,
  `.ARM.attributes`, `DT_NEEDED`, and the absence of `libc++_shared.so`.
* **Build system: most likely the NDK r18b CMake toolchain
  (`build/cmake/android.toolchain.cmake`), Release, `ANDROID_ABI=armeabi-v7a`,
  `ANDROID_PLATFORM=android-16`, `ANDROID_STL=c++_static`,
  `ANDROID_ARM_NEON=ON`.**  A minimal CMake probe with those settings emits a
  compile line that matches the original's effective compile line on every
  flag the binary can witness, and a link line whose `DT_NEEDED` order is
  exactly the original's.  The
  alternative is **ndk-build r18b with explicit overrides**, chiefly
  `LOCAL_CFLAGS := -fno-data-sections` (the ndk-build default is
  `-fdata-sections`, which the original's data layout rejects).  Confidence:
  CMake ~70%, ndk-build ~25%, custom script ~5%.  The binary cannot distinguish
  the two further because every differing token is codegen-neutral except
  `-fdata-sections`.
* **Effective compile flags are the NDK r18b defaults with `-fdata-sections`
  off.**  The load-bearing flags are `-Oz`, `-g` (debug info is codegen-relevant
  at clang 7 `-Oz`), `-std=c++11`, `-fstack-protector-strong`,
  `-funwind-tables`, `-ffunction-sections`, `-mthumb`, NEON, RTTI and
  exceptions.  `-fPIC`/`-fpic` is required but its spelling is not
  distinguishable.  `-march=armv7-a`, `-mfloat-abi=softfp`, `-mfpu=neon` and
  `-frtti` are redundant with the target triple / clang defaults and removing
  them is byte-identical.
* **Effective link flags are the NDK r18b defaults**: `-z now` (hence RELRO),
  SHA-1 `--build-id`, `--hash-style=sysv`, `--exclude-libs=libgcc.a`,
  zlib/libzip `--whole-archive`, `--spare-dynamic-tags=5`, `-z combreloc`,
  `--no-as-needed`, `-z max-page-size=4096`, `-s`.  The tree's shorter gold
  line is byte-equivalent to the full ndk-build/CMake line.
* **The shipped APK binary is not the comparison target.**  The APK's
  `lib/armeabi-v7a/libgof2hdaa.so` is exactly the ground-truth file after GNU
  `arm-linux-androideabi-strip` (288 differing bytes, all `.shstrtab` and
  `sh_name`).  The ground-truth file is the gold `-s` output.  The original
  build therefore ran a GNU strip as a post-link/install step, as ndk-build and
  CMake's Android install rules do.
* **The original's `libc++abi.a` came from the darwin-x86_64 NDK r18b
  package.**  The only path-like string in the `.so` is
  `/Volumes/Android/buildbot/.../libcxxabi/src/abort_message.cpp`; the
  linux-x86_64 archive carries `/buildbot/...` instead.  This is why the tree's
  canonical build needs the darwin `libc++abi.a` in the NDK cache (see §3.3).
* **No flag is a strict improvement.**  Every tested neutral flag leaves the
  `.so` byte-identical to the baseline except `.note.gnu.build-id`; no flag
  raises `byte_exact`/`linked_exact` above 4498.  The report is the only
  deliverable.

## 1. Baseline and gate

Fresh run at the start and end of this audit:

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8
cmake --build cmake-build-match --target lint  -j8
```

```text
verify: byte 4498  linked 4498  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
=== DRIFT: 0 field(s) across 0 class(es) ===
CLEAN ✓ (no new soft violations; hard invariants 0)
```

`report.json`: `compared 4524`, `size_exact 4524/4524`.
`.text` = `0x1337c8` at `0x69440` (exact size and address).

| artifact | sha256 | size |
|---|---|---:|
| tree `cmake-build-match/libgof2hdaa.so` | `25f0a7e773d670ea16440f7a78d1ce5a82b71f3a348cb40e9ad80076b33444e1` | 2,187,380 |
| ground truth `../_work/bins/android_2.0.16_libgof2hdaa.so` | `d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483` | 2,187,328 |
| APK `lib/armeabi-v7a/libgof2hdaa.so` | `6d38fd767ee3b4f8174c371bce371bfc6397cf76e13c1756938b2a94658e3b58` | 2,187,328 |

The tree is **not** fully byte-identical to the ground truth: 4498 of 4524
functions are byte-exact, `.text` has 107,760 differing bytes, and the residual
non-flag deltas in §8 remain.  The gate is per-function, not whole-file.

## 2. Toolchain identity (hard ELF evidence)

Commands:

```sh
O=../_work/bins/android_2.0.16_libgof2hdaa.so
readelf -hW  $O
readelf -AW  $O
readelf -nW  $O
readelf -p .comment $O
readelf -dW  $O
readelf -rW  $O
```

| observable | original | confidence |
|---|---|---|
| ELF | `ELF32`, `DYN`, `ARM`, `Version5 EABI, soft-float ABI` | certain |
| `.comment` | `Android (4751641 based on r328903) clang version 7.0.2` + `GCC: (GNU) 4.9.x 20150123 (prerelease)` | certain |
| `.note.android.ident` | API `16`, `r18b`, build `5063045` | certain |
| `.note.gnu.gold-version` | `gold 1.12` | certain |
| `.ARM.attributes` | ARM v7, Thumb-2, VFPv3, NEONv1, GOT-indirect, wchar_t 4, int enums, 8-byte align, unaligned v6 | certain |
| `DT_SONAME` | `libgof2hdaa.so` | certain |
| `DT_NEEDED` | `liblog, libGLESv2, libGLESv1_CM, libEGL, libandroid, libfmodex, libfmodevent, libm, libdl, libc` | certain |
| `DT_FLAGS` / `DT_FLAGS_1` | `BIND_NOW` / `NOW` | certain |
| `DT_RELCOUNT` | `1162` | certain |
| `.init_array` / `.fini_array` | 3 entries / 2 entries | certain |
| relocation types | `3674 R_ARM_JUMP_SLOT`, `1817 R_ARM_ABS32`, `1162 R_ARM_RELATIVE`, `614 R_ARM_GLOB_DAT`, **0 `R_ARM_REL32`** | certain |
| static C++ runtime | no `libc++_shared.so` NEEDED; `__cxa_*` defined; no `std::__ndk1` symbols pulled | certain |
| unwind | `_Unwind_*` are `UND`; no `__gnu_Unwind_Find_exidx` | certain |
| libc wrapper | `memalign@LIBC` imported (libandroid_support's hidden wrapper) | certain |

The GCC 4.9.x `.comment` string is not the app compiler: the app TUs are clang
7.0.2.  It comes from the GCC-built vendored libcrypto (OpenSSL 1.1.0l, NDK r13b
GCC 4.9) and/or the NDK's GCC 4.9 crt/libgcc objects.  The tree reproduces it by
building libcrypto with NDK r13b GCC 4.9 (`third_party/build_match_lib.cmake`).

## 3. Build-system identification: CMake vs ndk-build

### 3.1 The NDK r18b CMake toolchain default is an exact match

A minimal CMake project using the NDK r18b CMake toolchain
(the probe's compile line matches every flag the binary can witness):

```sh
NDK=$ROOT/.cache/ndk/android-ndk-r18b
cmake -S $ROOT/cmake-build-w3750-harness/cmakeprobe \
  -B $ROOT/cmake-build-w3750-harness/cmakeprobe/build -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=$NDK/build/cmake/android.toolchain.cmake \
  -DANDROID_ABI=armeabi-v7a \
  -DANDROID_PLATFORM=android-16 \
  -DANDROID_STL=c++_static \
  -DANDROID_ARM_NEON=ON \
  -DCMAKE_BUILD_TYPE=Release
```

emits (paths abbreviated; `$NDK` as above):

```text
$NDK/toolchains/llvm/prebuilt/linux-x86_64/bin/clang++ \
  --target=armv7-none-linux-androideabi16 \
  --gcc-toolchain=$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64 \
  --sysroot=$NDK/sysroot \
  -isystem $NDK/sources/cxx-stl/llvm-libc++/include \
  -isystem $NDK/sources/android/support/include \
  -isystem $NDK/sources/cxx-stl/llvm-libc++abi/include \
  -isystem $NDK/sysroot/usr/include/arm-linux-androideabi \
  -g -DANDROID -ffunction-sections -funwind-tables -fstack-protector-strong \
  -no-canonical-prefixes \
  -march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16 -mthumb -mfpu=neon \
  -Wa,--noexecstack -Wformat -Werror=format-security \
  -std=c++11 -Oz -DNDEBUG -fPIC -c file.cpp -o file.o
```

and links:

```text
$NDK/toolchains/llvm/prebuilt/linux-x86_64/bin/clang++ \
  --target=armv7-none-linux-androideabi16 \
  --gcc-toolchain=$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64 \
  --sysroot=$NDK/sysroot -fPIC \
  -isystem $NDK/sysroot/usr/include/arm-linux-androideabi \
  -g -DANDROID -ffunction-sections -funwind-tables -fstack-protector-strong \
  -no-canonical-prefixes \
  -march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16 -mthumb -mfpu=neon \
  -Wa,--noexecstack -Wformat -Werror=format-security \
  -std=c++11 -Oz -DNDEBUG \
  -Wl,--exclude-libs,libgcc.a -Wl,--exclude-libs,libatomic.a -nostdlib++ \
  --sysroot $NDK/platforms/android-16/arch-arm \
  -Wl,--build-id -Wl,--warn-shared-textrel -Wl,--fatal-warnings \
  -Wl,--fix-cortex-a8 -Wl,--exclude-libs,libunwind.a \
  -L$NDK/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a \
  -Wl,--no-undefined -Wl,-z,noexecstack -Qunused-arguments \
  -Wl,-z,relro -Wl,-z,now -shared -Wl,-soname,libt.so -o libt.so \
  file.o \
  -llog -lGLESv2 -lGLESv1_CM -lEGL -landroid \
  $APK_LIBS/libfmodex.so $APK_LIBS/libfmodevent.so -lm -ldl \
  -latomic -lm \
  $NDK/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/libc++_static.a \
  $NDK/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/libc++abi.a \
  $NDK/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/libandroid_support.a \
  $NDK/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/libunwind.a -ldl
```

The probe's `.ARM.attributes`, `.comment` (clang part),
`.note.android.ident` and `.note.gnu.gold-version` are byte-identical to the
original's.  Its `DT_NEEDED` order with `target_link_libraries(t log GLESv2
GLESv1_CM EGL android libfmodex.so libfmodevent.so m dl)` is exactly the
original's:

```text
liblog.so libGLESv2.so libGLESv1_CM.so libEGL.so libandroid.so
libfmodex.so libfmodevent.so libm.so libdl.so libc.so
```

Why this is the best single explanation:

* The CMake toolchain default has **no `-fdata-sections`**.  The original's
  `.rodata`/`.data` layout is the no-`-fdata-sections` layout (proven below).
* The CMake toolchain default `ANDROID_STL` is `c++_static`; the original is
  `c++_static`.  ndk-build's default `APP_STL` is `system` and must be
  overridden.
* The CMake toolchain default `ANDROID_ABI` is `armeabi-v7a` and
  `ANDROID_PLATFORM` defaults to the NDK minimum (`android-16` for r18b); both
  match the original without overrides.
* The CMake toolchain default `CMAKE_POSITION_INDEPENDENT_CODE=TRUE` adds
  `-fPIC`, matching the tree's `-fPIC`; ndk-build uses `-fpic` (codegen-neutral
  here, but a textual difference).
* The CMake toolchain defaults `ANDROID_ARM_NEON` to FALSE for API < 23, so the
  original build must set `ANDROID_ARM_NEON=ON` (or use
  `ANDROID_ABI="armeabi-v7a with NEON"`).  That is one standard variable; the
  ndk-build hypothesis needs `APP_ABI`, `APP_STL`, `LOCAL_ARM_NEON`,
  `LOCAL_CPP_FEATURES` and `LOCAL_CFLAGS := -fno-data-sections` overrides.
* The CMake toolchain default `-g` is unconditional in Release, explaining why
  a release build has load-bearing debug info.
* The CMake toolchain default link set (`-Wl,--build-id`,
  `-Wl,--warn-shared-textrel -Wl,--fatal-warnings`, `-Wl,--fix-cortex-a8`,
  `-Wl,--exclude-libs=libgcc.a/libatomic.a/libunwind.a`, `-nostdlib++`,
  `-Wl,--no-undefined`, `-Wl,-z,noexecstack`, `-Wl,-z,relro -Wl,-z,now`,
  `-latomic -lm`, `libc++_static.a libc++abi.a libandroid_support.a
  libunwind.a -ldl`) matches the original's link fingerprint.

### 3.2 The ndk-build r18b default is close but needs overrides

A minimal `ndk-build` project (`APP_ABI := armeabi-v7a`,
`APP_STL := c++_static`, `APP_PLATFORM := android-16`, `APP_OPTIM := release`,
`LOCAL_CPP_FEATURES := rtti exceptions`, `LOCAL_ARM_NEON := true`,
`LOCAL_LDLIBS := -llog -lGLESv2 -lGLESv1_CM -lEGL -landroid
-L$APK_LIBS -lfmodex -lfmodevent -lm -ldl`) with `V=1` emits:

```text
$NDK/toolchains/llvm/prebuilt/linux-x86_64/bin/clang++ \
  -target armv7-none-linux-androideabi16 \
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
  -frtti -fexceptions \
  --sysroot=$NDK/sysroot \
  -isystem $NDK/sysroot/usr/include/arm-linux-androideabi \
  -c file.cpp -o file.o
```

and links:

```text
$NDK/toolchains/llvm/prebuilt/linux-x86_64/bin/clang++ \
  -Wl,-soname,libt.so -shared \
  --sysroot=$NDK/platforms/android-16/arch-arm \
  file.o \
  $NDK/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/libc++_static.a \
  $NDK/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/libc++abi.a \
  $NDK/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/libandroid_support.a \
  $NDK/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/libunwind.a \
  -lgcc -Wl,--exclude-libs,libgcc.a \
  -latomic -Wl,--exclude-libs,libatomic.a \
  -target armv7-none-linux-androideabi16 -no-canonical-prefixes \
  -gcc-toolchain $NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64 \
  -Wl,--fix-cortex-a8 -Wl,--exclude-libs,libunwind.a \
  -Wl,--build-id -nostdlib++ -Wl,--no-undefined \
  -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now \
  -Wl,--warn-shared-textrel -Wl,--fatal-warnings \
  -L$NDK/platforms/android-16/arch-arm/usr/lib \
  -llog -lGLESv2 -lGLESv1_CM -lEGL -landroid \
  -L$APK_LIBS -lfmodex -lfmodevent -lm -ldl -ldl -lc -lm -o libt.so
```

The ndk-build line differs from the CMake line in `-fdata-sections` (material),
`-fpic` vs `-fPIC` (neutral), `-fno-exceptions -fno-rtti` (neutral because
re-enabled), and the two `-Wno-*` flags (neutral).  To reproduce the original
with ndk-build, the module must additionally set
`LOCAL_CFLAGS := -fno-data-sections` (and the usual `APP_STL`, `APP_ABI`,
`LOCAL_ARM_NEON`, `LOCAL_CPP_FEATURES` overrides).  That is possible but less
parsimonious than the CMake default.

### 3.3 Host package and archive provenance (darwin `libc++abi.a`)

The only path-like string in the original `.so` is:

```text
/Volumes/Android/buildbot/src/android/ndk-release-r18/external/libcxx/../../external/libcxxabi/src/abort_message.cpp
```

That string is emitted by `libc++abi.a`'s `abort_message.o`.  The NDK r18b
**darwin-x86_64** archive contains `/Volumes/Android/...`; the
**linux-x86_64** archive contains `/buildbot/...` instead:

```text
$ strings -a $NDK/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/libc++abi.a | grep abort_message
/Volumes/Android/buildbot/.../libcxxabi/src/abort_message.cpp   # darwin package
$ strings -a /tmp/w3750/linux-archives/libc++abi.a | grep abort_message
/buildbot/.../libcxxabi/src/abort_message.cpp                  # linux package
```

Controlled A/B (replace only the archive in the link):

| archive used | byte | linked | `.rodata` | diff vs baseline |
|---|---:|---:|---:|---:|
| darwin `libc++abi.a` (canonical) | 4498 | 4498 | 0x41b67 | 0 |
| linux `libc++abi.a` | 4344 | 4498 | 0x41b57 | 107538 |
| linux `libandroid_support.a` | 4498 | 4498 | 0x41b67 | 0 |
| linux `libgcc.a` | 4498 | 4498 | 0x41b67 | 0 |

So the original's `libc++abi.a` was the darwin-x86_64 NDK r18b archive.  This is
consistent with the macOS/iOS source-layout dumps and the APK build date.
APK metadata (`assets/crashlytics-build.properties`): package
`net.fishlabs.gof2hdallandroid2012`, version `2.0.16`, build date
`Mon Oct 22 10:18:17 CEST 2018` (shortly after r18b).  The tree's `cmake/DownloadNDK.cmake` selects
the host package; on a fresh Linux host it downloads the linux archive, whose
`libc++abi.a` is **not** byte-compatible with the original.  The canonical
result in this worktree relies on the darwin archive having been copied into
`.cache/ndk/.../libs/armeabi-v7a/libc++abi.a`.  `libandroid_support.a` and
`libgcc.a` may be either host's archive (no pulled path strings).

### 3.4 Confidence

| claim | confidence | evidence |
|---|---:|---|
| NDK r18b clang 7.0.2 + gold 1.12 | 99% | `.comment`, `.note.android.ident`, `.note.gnu.gold-version` |
| API 16 | 99% | `.note.android.ident` first word `0x10` |
| armeabi-v7a softfp, Thumb-2, NEON | 99% | `.ARM.attributes`, ELF flags |
| `c++_static` | 99% | no `libc++_shared.so`; `__cxa_*` defined; no `std::__ndk1` pulled |
| CMake + NDK r18b toolchain | ~70% | exact default compile/link match; no `-fdata-sections` |
| ndk-build r18b with `-fno-data-sections` | ~25% | same effective flags with explicit overrides |
| custom build script | ~5% | no positive evidence |
| darwin-x86_64 NDK package for `libc++abi.a` | ~90% | `/Volumes/Android/...` string in `.rodata` |
| GNU strip post-link/install step | ~99% | APK file == `strip(ground truth)`, 288-byte `.shstrtab` delta |
| `-fdata-sections` off | ~99% | `.rodata`/`.data` layout |
| `-g` on | ~99% | `-g0`/`-g1` drop byte_exact 4498 -> 2069 |
| `-Oz` | ~99% | `-Os/-O1/-O2/-O3` and no-`-O` all much worse |
| `-fPIC` vs `-fpic` | 50/50 | byte-identical output; both satisfy the no-`R_ARM_REL32` requirement |
| exact `-g` level (`-g`/`-g2`/`-g3`) | unknown | all three byte-identical; `-g0`/`-g1` excluded |
| explicit `-frtti` vs clang default | unknown | removal is byte-identical |

## 4. Controlled A/B methodology

### 4.1 Harness

All scratch files are under `cmake-build-w3750-harness/` (gitignored):

* `toolchain_extra.cmake` appends `GOF2_EXTRA_FLAGS` to both
  `CMAKE_CXX_FLAGS` and `CMAKE_C_FLAGS`; `GOF2_EXTRA_CXX_FLAGS` /
  `GOF2_EXTRA_C_FLAGS` are C++/C-only.
* `toolchain_filter.cmake` + `clangxx_filter.sh` / `clang_filter.sh` remove
  exact flag tokens listed in `GOF2_FILTER_FLAGS` before invoking the real NDK
  clang.  This is required for flags that appear later from
  `target_compile_options` (`-funwind-tables`, `-ffunction-sections`); appending
  a negative flag alone is a flag-order trap (see §6.3).
* `run_one.sh <tag> <extra> <extra_cxx> <extra_c> <filter>` configures
  `cmake-build-w3750-<tag>`, builds `gof2_libs` under `flock vendor.lock`,
  builds `link_so`, and runs `measure.py`.
* `measure.py <builddir> <tag>` runs the official
  `tools/verify/verify.py` and prints JSON with `byte_exact`, `linked_exact`,
  `size_exact`, section sizes, `diff_ex_buildid`, and sha.
* `relink.py <out.so> <tag> [gold args...]` extracts the canonical gold command
  from `ninja -C cmake-build-match -t commands libgof2hdaa.so`, replaces `-o`,
  inserts extra args after `-shared`, and removes exact tokens/prefixes with
  `--remove=` / `--remove-prefix=`.
* `link_measure.py <so> <tag>` reports `diff_vs_base`, `diff_vs_orig`, section
  sizes and sha; `verify_so.py <so> <tag>` runs the official verifier on a
  relinked `.so`.

### 4.2 Commands

```sh
ROOT=/home/debian/GalaxyOnFire2/wt/w3750
H=$ROOT/cmake-build-w3750-harness

# compile variant
TMPDIR=$H/tmp cmake -S $ROOT -B $ROOT/cmake-build-w3750-<tag> -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=$H/toolchain_filter.cmake \
  -DGOF2_MATCH_OPT=-Oz \
  -DGOF2_EXTRA_FLAGS='<extra>' \
  -DGOF2_EXTRA_CXX_FLAGS='<extra-cxx>' \
  -DGOF2_EXTRA_C_FLAGS='<extra-c>' \
  -DGOF2_FILTER_FLAGS='<filter>'
export GOF2_FILTER_FLAGS='<filter>'
flock $H/vendor.lock env TMPDIR=$H/tmp \
  cmake --build $ROOT/cmake-build-w3750-<tag> --target gof2_libs -j8
TMPDIR=$H/tmp cmake --build $ROOT/cmake-build-w3750-<tag> --target link_so -j8
python3 $H/measure.py $ROOT/cmake-build-w3750-<tag> <tag>

# link variant
python3 $H/relink.py $H/link_<tag>.so <tag> <gold args...>
python3 $H/link_measure.py $H/link_<tag>.so <tag>
python3 $H/verify_so.py $H/link_<tag>.so <tag>
```

`diff_ex_buildid` / `diff_vs_base` is a section-by-section byte comparison
against `cmake-build-w3750-harness/baseline.so` (a copy of the canonical
`cmake-build-match/libgof2hdaa.so`), skipping only `.note.gnu.build-id`.  A value
of `0` means the `.so` is byte-identical to the baseline except for the build-id.

## 5. Compile-flag A/B results

Baseline: `byte_exact 4498`, `linked_exact 4498`, `size_exact 4524/4524`,
`.text 0x1337c8`, `.rodata 0x41b67`, `.data 0x296c`, sha
`25f0a7e773d670ea...`.

### 5.1 Full table

`diff` is `diff_ex_buildid` vs baseline.  A `diff` of 0 means byte-identical
except the build-id.  `size` is `size_exact/compared`.

| variant | flag change vs canonical | byte | linked | size | `.text` | `.rodata` | `.data` | diff |
|---|---|---:|---:|---:|---:|---:|---:|---:|
| `fdata` | `-fdata-sections` | 4334 | 4498 | 4524/4524 | 0x1337c8 | 0x41a07 | 0x2970 | 119414 |
| `fnodata` | `-fno-data-sections` | 4498 | 4498 | 4524/4524 | 0x1337c8 | 0x41b67 | 0x296c | 0 |
| `fpic` | `-fpic` (vs `-fPIC`) | 4498 | 4498 | 4524/4524 | 0x1337c8 | 0x41b67 | 0x296c | 0 |
| `gnu11` | `-std=gnu++11` | 4498 | 4498 | 4524/4524 | 0x1337c8 | 0x41b67 | 0x296c | 0 |
| `g0` | `-g0` | 2069 | 4246 | 4485/4524 | 0x1337c8 | 0x41b67 | 0x296c | 809825 |
| `g1` | `-g1` | 2069 | 4246 | 4485/4524 | 0x1337c8 | 0x41b67 | 0x296c | 809825 |
| `g2` | `-g2` | 4498 | 4498 | 4524/4524 | 0x1337c8 | 0x41b67 | 0x296c | 0 |
| `g3` | `-g3` | 4498 | 4498 | 4524/4524 | 0x1337c8 | 0x41b67 | 0x296c | 0 |
| `stackprot` | `-fstack-protector` | 1369 | 3785 | 3793/4524 | 0x12e1c8 | 0x41b67 | 0x296c | 1500292 |
| `nostackprot` | `-fno-stack-protector` | 1369 | 3781 | 3789/4524 | 0x12e148 | 0x41b67 | 0x296c | 1511450 |
| `stackprotall` | `-fstack-protector-all` | 91 | 962 | 981/4524 | 0x14f728 | 0x41b67 | 0x296c | 1546450 |
| `marm` | `-marm` | 57 | 237 | 342/4524 | 0x1a3b98 | 0x41b67 | 0x296c | 1938003 |
| `vfpv3d16` | `-mfpu=vfpv3-d16` | build fail | — | — | — | — | — | — |
| `hardfp` | `-mfloat-abi=hard` | build fail | — | — | — | — | — | — |
| `softfp_soft` | `-mfloat-abi=soft` | build fail | — | — | — | — | — | — |
| `march_neutral` | `-march=armv7-a` | 4498 | 4498 | 4524/4524 | 0x1337c8 | 0x41b67 | 0x296c | 0 |
| `mcpu_cortexa8` | `-mcpu=cortex-a8` | 1330 | 4225 | 4366/4524 | 0x133fc8 | 0x41b67 | 0x296c | 1368100 |
| `Os` | `-Os` | 1263 | 2650 | 2882/4356 | 0x14bf08 | 0x43057 | 0x294c | 2030323 |
| `O1` | `-O1` | 1033 | 1783 | 2676/4524 | 0x143f68 | 0x41bc7 | 0x294c | 1805091 |
| `O2` | `-O2` | 1168 | 2159 | 2411/4296 | 0x1632c8 | 0x42d77 | 0x294c | 2133093 |
| `O3` | `-O3` | 1167 | 2119 | 2247/4295 | 0x1717b8 | 0x42c97 | 0x296c | 2189154 |
| `fnortti` | `-fno-rtti` | 1725 | 4498 | 4524/4524 | 0x1337c8 | 0x4104e | 0x296c | 465325 |
| `fnoexceptions` | `-fno-exceptions` | build fail | — | — | — | — | — | — |
| `fomitfp` | `-fomit-frame-pointer` | 1298 | 1921 | 2163/4524 | 0x12e008 | 0x41b67 | 0x296c | 1423846 |
| `fnoomitfp` | `-fno-omit-frame-pointer` | 4498 | 4498 | 4524/4524 | 0x1337c8 | 0x41b67 | 0x296c | 0 |
| `fnostrictalias` | `-fno-strict-aliasing` | 1371 | 4349 | 4411/4524 | 0x133d08 | 0x41b67 | 0x296c | 1377124 |
| `fnothreadsafe` | `-fno-threadsafe-statics` | 1368 | 4494 | 4520/4521 | 0x133684 | 0x419b7 | 0x296c | 1564021 |
| `fnosizeddealloc` | `-fno-sized-deallocation` | 4498 | 4498 | 4524/4524 | 0x1337c8 | 0x41b67 | 0x296c | 0 |
| `fnodeletenull` | `-fno-delete-null-pointer-checks` | 4498 | 4498 | 4524/4524 | 0x1337c8 | 0x41b67 | 0x296c | 0 |
| `fwrapv` | `-fwrapv` | 1398 | 4462 | 4500/4524 | 0x1338c8 | 0x41b67 | 0x296c | 1123876 |
| `fnostrictoverflow` | `-fno-strict-overflow` | 1398 | 4462 | 4500/4524 | 0x1338c8 | 0x41b67 | 0x296c | 1123876 |
| `fnobuiltin` | `-fno-builtin` | 1350 | 3986 | 4121/4524 | 0x1347b8 | 0x41b67 | 0x296c | 1765287 |
| `fnocxaatexit` | `-fno-use-cxa-atexit` | 1356 | 4497 | 4523/4524 | 0x133818 | 0x41b67 | 0x296c | 1223067 |
| `mnouunaligned` | `-mno-unaligned-access` | 1404 | 4443 | 4471/4524 | 0x1334c8 | 0x41b67 | 0x296c | 1394590 |
| `mnomovt` | `-mno-movt` | 4498 | 4498 | 4524/4524 | 0x1337c8 | 0x41b67 | 0x296c | 0 |
| `fnojumptables` | `-fno-jump-tables` | 1363 | 4410 | 4437/4524 | 0x134908 | 0x41947 | 0x296c | 1447346 |
| `fnovectorize` | `-fno-vectorize` | 4498 | 4498 | 4524/4524 | 0x1337c8 | 0x41b67 | 0x296c | 0 |
| `fnoslp` | `-fno-slp-vectorize` | 1484 | 4384 | 4408/4524 | 0x132ea8 | 0x41b67 | 0x296c | 1347269 |
| `fnovec_both` | `-fno-vectorize -fno-slp-vectorize` | 1484 | 4384 | 4408/4524 | 0x132ea8 | 0x41b67 | 0x296c | 1347269 |
| `fnoinline` | `-fno-inline` | 1219 | 3511 | 3571/4524 | 0x13bf88 | 0x41b67 | 0x296c | 1743409 |
| `fnoinlinefuncs` | `-fno-inline-functions` | 1219 | 3511 | 3571/4524 | 0x13bf88 | 0x41b67 | 0x296c | 1743409 |
| `fnosibling` | `-fno-optimize-sibling-calls` | 1316 | 3587 | 3686/4524 | 0x1315b8 | 0x41b67 | 0x296c | 1385246 |
| `vis_hidden` | `-fvisibility=hidden` | 57 | 243 | 244/244 | 0x130f58 | 0x41b67 | 0x2a2c | 1792616 |
| `fno_common` | `-fno-common` | build fail | — | — | — | — | — | — |
| `DNDEBUG` | `-DNDEBUG` | 4498 | 4498 | 4524/4524 | 0x1337c8 | 0x41b67 | 0x296c | 0 |
| `DANDROID` | `-DANDROID` | 4498 | 4498 | 4524/4524 | 0x1337c8 | 0x41b67 | 0x296c | 0 |
| `noexecstack` | `-Wa,--noexecstack` | 4498 | 4498 | 4524/4524 | 0x1337c8 | 0x41b67 | 0x296c | 0 |
| `format` | `-Wformat -Werror=format-security` | 4498 | 4498 | 4524/4524 | 0x1337c8 | 0x41b67 | 0x296c | 0 |
| `nocanon` | `-no-canonical-prefixes` | 4498 | 4498 | 4524/4524 | 0x1337c8 | 0x41b67 | 0x296c | 0 |
| `gcctoolchain` | `-gcc-toolchain $NDK/.../linux-x86_64` | 4498 | 4498 | 4524/4524 | 0x1337c8 | 0x41b67 | 0x296c | 0 |
| `nounwind_off` | filter `-funwind-tables` | 2878 | 4498 | 4524/4524 | 0x1337c8 | 0x41b67 | 0x296c | 198723 |
| `nofuncsec_off` | filter `-ffunction-sections` | 1345 | 4355 | 4399/4524 | 0x133134 | 0x41b67 | 0x296c | 1341724 |
| `nopic_off` | filter `-fPIC` | build fail | — | — | — | — | — | — |
| `nortti_off` | filter `-frtti` | 4498 | 4498 | 4524/4524 | 0x1337c8 | 0x41b67 | 0x296c | 0 |
| `nostackprot_off` | filter `-fstack-protector-strong` | 1369 | 3781 | 3789/4524 | 0x12e148 | 0x41b67 | 0x296c | 1511450 |
| `nothumb_off` | filter `-mthumb` | 57 | 237 | 342/4524 | 0x1a3b98 | 0x41b67 | 0x296c | 1938003 |
| `noneon_off` | filter `-mfpu=neon` | 4498 | 4498 | 4524/4524 | 0x1337c8 | 0x41b67 | 0x296c | 0 |
| `nosoftfp_off` | filter `-mfloat-abi=softfp` | 4498 | 4498 | 4524/4524 | 0x1337c8 | 0x41b67 | 0x296c | 0 |
| `nomarch_off` | filter `-march=armv7-a` | 4498 | 4498 | 4524/4524 | 0x1337c8 | 0x41b67 | 0x296c | 0 |
| `nostd_off` | filter `-std=c++11` | 1512 | 4485 | 4514/4524 | 0x133788 | 0x41b67 | 0x296c | 1157040 |
| `noOz_off` | filter `-Oz` | 61 | 255 | 259/4524 | 0x267058 | 0x419d7 | 0x29ec | 3233322 |
| `nog_off` | filter `-g` | 2069 | 4246 | 4485/4524 | 0x1337c8 | 0x41b67 | 0x296c | 809825 |

### 5.2 Extra equivalence experiments

These confirm that the tree's flags are equivalent to the full NDK default
sets, and that the only material difference is `-fdata-sections`.

| variant | flags | byte | linked | size | `.rodata` | `.data` | diff |
|---|---|---:|---:|---:|---:|---:|---:|
| `nounwind_appended` | append `-fno-unwind-tables` (positive flag still later) | 4498 | 4498 | 4524/4524 | 0x41b67 | 0x296c | 0 |
| `nofuncsec_appended` | append `-fno-function-sections` (positive flag still later) | 4498 | 4498 | 4524/4524 | 0x41b67 | 0x296c | 0 |
| `ndk_exc_rtti` | `-fno-exceptions -fno-rtti -frtti -fexceptions` | 4498 | 4498 | 4524/4524 | 0x41b67 | 0x296c | 0 |
| `ndk_fpu` | `-mfpu=vfpv3-d16 -mfpu=neon` | 4498 | 4498 | 4524/4524 | 0x41b67 | 0x296c | 0 |
| `ndk_full_nodata` | full ndk-build compile set minus `-fdata-sections` | 4498 | 4498 | 4524/4524 | 0x41b67 | 0x296c | 0 |
| `ndk_full_fdata` | full ndk-build compile set plus `-fdata-sections` | 4334 | 4498 | 4524/4524 | 0x41a07 | 0x2970 | 119414 |

`ndk_full_nodata` flags:

```text
-DNDEBUG -fpic -fno-exceptions -fno-rtti -frtti -fexceptions
-no-canonical-prefixes -Wno-invalid-command-line-argument
-Wno-unused-command-line-argument -gcc-toolchain <NDK gcc dir>
-mfpu=vfpv3-d16 -mfpu=neon -Wa,--noexecstack -Wformat
-Werror=format-security -DANDROID
```

This is byte-identical to the canonical build (sha `25f0a7e7...`), proving every
ndk-build compile token except `-fdata-sections` is codegen-neutral here.
`ndk_full_fdata` has the same sha as the `fdata` variant (`cb357dff...`).

### 5.3 Build failures

| variant | failure |
|---|---|
| `fno_common` | gold link: multiple definition of `g_android_back_button_pressed` (`BackButton.c` and `GOF2HD2012.c`) |
| `fnoexceptions` | compile: `Level.cpp:5217: cannot use 'try' with exceptions disabled` |
| `hardfp` | gold link: `uses VFP register arguments, output does not` |
| `softfp_soft` | compile: `arm_neon.h:28: "NEON support not enabled"` |
| `vfpv3d16` | compile: `arm_neon.h:28: "NEON support not enabled"` |
| `nopic_off` | gold link: `requires unsupported dynamic reloc R_ARM_REL32; recompile with -fPIC` |

`-fno-common` is a current-tree source issue, not evidence against the original
using `-fcommon`: clang 7 defaults to `-fcommon`, and the original's tentative
definitions would link under that default.  The original's exceptions/RTTI are
on; `-fno-exceptions` cannot be the original flag.

## 6. Link-flag A/B results

Baseline link line (canonical `cmake-build-match`):

```text
ld.gold -shared -soname libgof2hdaa.so --build-id -s -z now \
  --exclude-libs=libgcc.a \
  --section-ordering-file=.../section_order.txt \
  -o .../libgof2hdaa.so \
  .../crtbegin_so_renamed.o -L .../platforms/android-16/arch-arm/usr/lib \
  @.../exidx_markers/link_order.rsp \
  --whole-archive .../libz_gof2.a .../libzip_gof2.a --no-whole-archive \
  -l log -l GLESv2 -l GLESv1_CM -l EGL -l android \
  .../apk_libs/libfmodex.so .../apk_libs/libfmodevent.so \
  .../libcrypto_gof2.a -l m \
  .../libc++abi.a .../libandroid_support.a .../libgcc.a \
  -l dl -l c .../crtend_so.o
```

### 6.1 Full table

`diff` is `diff_vs_base` (section-by-section, skipping `.note.gnu.build-id`).

| variant | link change vs canonical | byte | linked | size | `.text` | `.rodata` | `.data` | diff |
|---|---|---:|---:|---:|---:|---:|---:|---:|
| `no_now` | remove `-z now` | 2932 | 4498 | 4524/4524 | 0x1337c8 | 0x41b67 | 0x296c | 41981 |
| `now_norelro` | add `-z norelro` | 2932 | 4498 | 4524/4524 | 0x1337c8 | 0x41b67 | 0x296c | 59756 |
| `buildid_md5` | append `--build-id=md5` (no-op) | 4498 | 4498 | 4524/4524 | 0x1337c8 | 0x41b67 | 0x296c | 0 |
| `buildid_none` | append `--build-id=none` (no-op) | 4498 | 4498 | 4524/4524 | 0x1337c8 | 0x41b67 | 0x296c | 0 |
| `buildid_md5_only` | replace `--build-id` with `--build-id=md5` | 1969 | 4498 | 4524/4524 | 0x1337c8 | 0x41b67 | 0x296c | 58606 |
| `buildid_none_only` | replace `--build-id` with `--build-id=none` | 1750 | 4498 | 4524/4524 | 0x1337c8 | 0x41b67 | 0x296c | 78308 |
| `buildid_sha1_only` | replace `--build-id` with `--build-id=sha1` | 4498 | 4498 | 4524/4524 | 0x1337c8 | 0x41b67 | 0x296c | 0 |
| `hash_gnu` | add `--hash-style=gnu` | 1750 | 4498 | 4524/4524 | 0x1337c8 | 0x41b67 | 0x296c | 229478 |
| `hash_both` | add `--hash-style=both` | 1750 | 4498 | 4524/4524 | 0x1337c8 | 0x41b67 | 0x296c | 208267 |
| `no_exclude` | remove `--exclude-libs=libgcc.a` | 1720 | 4491 | 4524/4524 | 0x1337d8 | 0x41b67 | 0x296c | 286396 |
| `exclude_all` | `--exclude-libs=ALL` | 1324 | 3431 | 4282/4282 | 0x133628 | 0x41b67 | 0x296c | 438265 |
| `no_whole` | remove zlib/libzip `--whole-archive` | 1338 | 4414 | 4440/4440 | 0x12c208 | 0x3dc17 | 0x292c | 617752 |
| `maxpage65536` | add `-z max-page-size=65536` | 2932 | 4498 | 4524/4524 | 0x1337c8 | 0x41b67 | 0x296c | 33652 |
| `spare0` | add `--spare-dynamic-tags=0` | 4487 | 4498 | 4524/4524 | 0x1337c8 | 0x41b67 | 0x296c | 4006 |
| `spare10` | add `--spare-dynamic-tags=10` | 4487 | 4498 | 4524/4524 | 0x1337c8 | 0x41b67 | 0x296c | 3787 |
| `nocombreloc` | add `-z nocombreloc` | 4487 | 4498 | 4524/4524 | 0x1337c8 | 0x41b67 | 0x296c | 18412 |
| `as_needed` | add `--as-needed` | 1750 | 4498 | 4524/4524 | 0x1337c8 | 0x41b67 | 0x296c | 78390 |
| `gc` | add `--gc-sections` | 2176 | 4498 | 4524/4524 | 0x133794 | 0x41b57 | 0x296c | 973814 |
| `icf` | add `--icf=all` | 1347 | 4497 | 4524/4524 | 0x12f99c | 0x41b67 | 0x296c | 1259302 |
| `no_merge_exidx` | add `--no-merge-exidx-entries` | 2878 | 4498 | 4524/4524 | 0x1337c8 | 0x41b67 | 0x296c | 64854 |
| `sort_name` | add `--sort-section=name` | 1342 | 4496 | 4524/4524 | 0x13381c | 0x41b50 | 0x296c | 1531544 |
| `rosegment` | add `--rosegment` | 1720 | 4498 | 4524/4524 | 0x1337c8 | 0x41b67 | 0x296c | 152991 |
| `no_order` | omit `--section-ordering-file` | 3031 | 4498 | 4524/4524 | 0x1337e8 | 0x41b67 | 0x296c | 161183 |
| `goldO2` | add gold `-O2` | 1720 | 4498 | 4524/4524 | 0x1337c8 | 0x41967 | 0x296c | 524821 |
| `no_strip` | remove `-s` | 4498 | 4498 | 4524/4524 | 0x1337c8 | 0x41b67 | 0x296c | 17250050 |

### 6.2 Neutral link flags (diff 0, byte 4498, linked 4498)

These are byte-identical to the baseline except the build-id:

| variant | change |
|---|---|
| `no_undefined` | add `--no-undefined` |
| `fix_cortex` | add `--fix-cortex-a8` |
| `warn_fatal` | add `--warn-shared-textrel --fatal-warnings` |
| `unresolved` | add `--unresolved-symbols=ignore-all` |
| `allow_multiple` | add `--allow-multiple-definition` |
| `threads` | add `--threads` |
| `no_threads` | add `--no-threads` |
| `discard_all` | add `--discard-all` |
| `goldO0` | add gold `-O0` |
| `now_relro` | add `-z relro` (already implied by `-z now`) |
| `buildid_sha1` | add `--build-id=sha1` (same as default) |
| `ndk_full` | add `--fix-cortex-a8 --no-undefined -z noexecstack -z relro --warn-shared-textrel --fatal-warnings --threads` |

`ndk_full` is the full ndk-build/CMake link flag set minus the flags already in
the tree line; it is byte-identical to the baseline (`diff 0`, sha
`25f0a7e7...`).  This proves the tree's shorter gold line is byte-equivalent to
the full official link line.

### 6.3 Flag-order trap

Appending `-fno-unwind-tables` or `-fno-function-sections` to
`CMAKE_CXX_FLAGS` is **neutral** (byte 4498, diff 0), because the positive
`-funwind-tables` / `-ffunction-sections` from `target_compile_options` appears
later and wins.  The proper A/B must remove the positive token with the filter
wrapper:

* filter `-funwind-tables`: byte 4498 -> **2878**, `.ARM.exidx` 0x6f70 ->
  0x6678, `.ARM.extab` 0x2628c -> 0x25560.
* filter `-ffunction-sections`: byte 4498 -> **1345**, `.text` 0x1337c8 ->
  0x133154, `.ARM.exidx` 0x6f70 -> 0x67f0.

This corrects the earlier note that called `-funwind-tables` neutral.

## 7. Proven / consistent / unknown

### 7.1 Proven load-bearing (removal or wrong value regresses the gate)

Compile: `-Oz`, `-g` (at least `-g2`), `-std=c++11`, `-fPIC`/`-fpic` (PIC
required), `-fstack-protector-strong` (not `-fstack-protector`/`-all`/off),
`-funwind-tables`, `-ffunction-sections`, `-mthumb`, NEON, RTTI on, exceptions
on, `-fdata-sections` off, default strict aliasing, frame pointers kept,
thread-safe statics on, SLP vectorization on, inlining on, sibling calls on,
jump tables on, unaligned access on, builtins on, `__cxa_atexit` used,
default signed-overflow semantics, default visibility, default `-fcommon`.

Link: `-z now` (RELRO/BIND_NOW), SHA-1 `--build-id`, `--hash-style=sysv`,
`--exclude-libs=libgcc.a`, zlib/libzip `--whole-archive`,
`--spare-dynamic-tags=5`, `-z combreloc`, `--no-as-needed`,
`-z max-page-size=4096`, `-s`, and (for this tree) the
`--section-ordering-file`.

### 7.2 Consistent but not distinguishable (neutral; either value works)

* `-fPIC` vs `-fpic`: byte-identical output; both satisfy the no-`R_ARM_REL32`
  requirement.  The CMake toolchain uses `-fPIC`; ndk-build uses `-fpic`.
* `-march=armv7-a`, `-mfloat-abi=softfp`, `-mfpu=neon`: redundant with
  `-target armv7-none-linux-androideabi16`; removing them is byte-identical.
  `-mthumb` is **not** redundant (removal -> byte 57).
* `-frtti`: redundant with clang's default; removal is byte-identical.
  `-fno-rtti` is worse, so RTTI is on.
* `-g` vs `-g2` vs `-g3`: all byte-identical; `-g0`/`-g1` are worse.
* `-fno-omit-frame-pointer`: neutral; the default already keeps frame pointers.
* `-fno-sized-deallocation`: neutral; `-std=c++11` already disables sized
  deallocation.
* `-fno-delete-null-pointer-checks`: neutral.
* `-mno-movt`: neutral.
* `-fno-vectorize`: neutral alone; `-fno-slp-vectorize` is worse, so SLP is on
  and loop vectorization is not observable.
* `-DNDEBUG`, `-DANDROID`, `-Wa,--noexecstack`,
  `-Wformat -Werror=format-security`, `-no-canonical-prefixes`,
  `-gcc-toolchain`: neutral; consistent with both official build systems.
* Link: `--no-undefined`, `--fix-cortex-a8`,
  `--warn-shared-textrel --fatal-warnings`, `--unresolved-symbols=ignore-all`,
  `--allow-multiple-definition`, `--threads`/`--no-threads`, `--discard-all`,
  gold `-O0`, `-z noexecstack`, `-z relro` (implied by `-z now`).
* `libc++_static.a`, `libunwind.a`, `libatomic.a`: present in the official link
  lines but no member is pulled; omitting them (as the tree does) is
  byte-identical.  Evidence: no `std::__ndk1` symbols; `_Unwind_*` are `UND`;
  no `__gnu_Unwind_Find_exidx`; no atomic helpers.

### 7.3 Unknown / not recoverable from the binary

* The exact build-system driver (CMake vs ndk-build) beyond the flag
  fingerprint.
* The exact `-fPIC`/`-fpic` spelling.
* The exact debug-info level (`-g`/`-g2`/`-g3`).
* Whether `-frtti`/`-fexceptions` were explicit or left to clang defaults.
* Whether `-fno-omit-frame-pointer`, `-fno-sized-deallocation`,
  `-fno-delete-null-pointer-checks`, `-mno-movt`, `-fno-vectorize` were
  explicit or default.
* The exact `Android.mk`/`Application.mk` or `CMakeLists.txt` variable names.
* The exact link-input archive member order beyond what `.dynsym`/`.text`
  witness (the tree reconstructs it from the original's symbol addresses).

## 8. Residual non-flag deltas in the current tree

These are not build-flag issues; they are recorded so the "byte-identical"
claim is not overstated.

| section | original | ours | delta |
|---|---:|---:|---:|
| `.text` | 0x1337c8 | 0x1337c8 | 0 (107,760 differing bytes; 26 functions not byte-exact) |
| `.rodata` | 0x41b67 | 0x41b67 | 0 (byte-identical) |
| `.data` | 0x296c | 0x296c | 0 (byte-identical) |
| `.data.rel.ro` | 0x2fe8 | 0x2fe8 | 0 (byte-identical) |
| `.plt` | 0xac4c | 0xac4c | 0 (byte-identical) |
| `.got` | 0x430c | 0x430c | 0 (byte-identical) |
| `.dynamic` | 0x148 | 0x148 | 0 (byte-identical) |
| `.init_array` | 0xc | 0xc | 0 (byte-identical) |
| `.fini_array` | 0x8 | 0x8 | 0 (byte-identical) |
| `.comment` | 0x12f | 0x12f | 0 (byte-identical) |
| `.note.android.ident` | 0x98 | 0x98 | 0 (byte-identical) |
| `.note.gnu.gold-version` | 0x1c | 0x1c | 0 (byte-identical) |
| `.ARM.attributes` | 0x38 | 0x38 | 0 (byte-identical) |
| `.ARM.exidx` | 0x6f70 | 0x6f70 | 0 size; 1,738 differing bytes |
| `.ARM.extab` | 0x26298 | 0x2628c | -12 (source LSDA residues) |
| `.dynsym` | 0x15ca0 | 0x15ca0 | 0 size; 1,697 differing bytes (order) |
| `.dynstr` | 0x2e318 | 0x2e318 | 0 size; 5,685 differing bytes (order) |
| `.hash` | 0x973c | 0x973c | 0 size; 505 differing bytes (order) |
| `.gnu.version` | 0x2b94 | 0x2b94 | 0 size; 56 differing bytes |
| `.rel.dyn` | 0x7048 | 0x7048 | 0 size; 92 differing bytes |
| `.rel.plt` | 0x72d0 | 0x72d0 | 0 size; 123 differing bytes |
| `.shstrtab` | 0x117 | 0x124 | +13 (`.missioncase` name) |
| `.missioncase` | absent | 0xc0 | source reconstruction aid |
| section count | 27 | 28 | +1 (`.missioncase`) |
| file size | 2,187,328 | 2,187,380 | +52 |

Specific notes:

* **`.missioncase`.**  `src/game/mission/Status.cpp` currently has
  `#pragma clang section rodata=".missioncase"` and a 192-byte
  `kMissionCase` rank table.  It adds a 28th section, extends the first LOAD
  segment by 0xc0, and adds 13 bytes to `.shstrtab`.  The original has zeros in
  the `.rodata`-to-`.fini_array` padding.  This is a source/codegen aid, not a
  flag.  `docs/IDENTICALITY.md` still says the section was removed by w3570;
  it was reintroduced by `6a98600e` and the doc is stale on this point.
* **`.rel.plt` flags.**  Original: `AI`, `sh_info=20` (`.got`).  Ours: `A`,
  `sh_info=0`.  This is not changed by `-z now`, `--build-id`, `-s`,
  `--section-ordering-file`, or the full ndk-build link flags.  The most likely
  cause is the host gold binary: the original was linked with the
  darwin-x86_64 NDK r18b gold, while this tree uses the linux-x86_64 gold.
  Both report `gold 1.12`.  The darwin gold is a Mach-O binary and was not
  executable on this Linux host, so this is not directly A/B-tested.
* **`.ARM.extab` -12.**  The docs attribute this to seven mismatched bodies
  (`Generator::createMission`, `Level::createShip`, `StarMap::draw`,
  `RecordHandler::recordStoreWrite`, `LevelScript::process`,
  `HangarWindow::render`, `Hud::draw`).  `--no-merge-exidx-entries` is much
  worse (2878), so the default merge is correct; the size delta is source, not
  flag.
* **`.shstrtab` / GNU strip.**  Ground truth is the gold `-s` output; the APK
  file is `strip(ground truth)`.  `cmp` shows 288 differing bytes, all in
  `.shstrtab` and the section headers' `sh_name` fields.  Both
  `--strip-all` and `--strip-unneeded` reproduce the APK file exactly (sha
  `6d38fd76...`).  The tree's gold `-s` reproduces the ground-truth
  `.shstrtab` order (plus the `.missioncase` name).
* **`.text` 107,760 differing bytes.**  The 26 non-byte-exact functions are
  source/register-allocation residues, not missing code; `.text` size and base
  are exact.  The gate is 4498/4524 byte-exact, 4498/4524 linked-exact.

## 9. Corrections to the existing research notes

| prior claim | correction | evidence |
|---|---|---|
| `-mthumb` removal is byte-identical / redundant with the target triple (w3750 handoff note) | **false**: removing `-mthumb` gives byte 57, `.text 0x1a3b98` (ARM mode); `-marm` is identical to removing `-mthumb` | `nothumb_off`, `marm` |
| `-funwind-tables` is neutral (`build_flags_research.md`) | **false**: appending `-fno-unwind-tables` is neutral only because `-funwind-tables` appears later; filtering the positive flag gives byte 2878 | `nounwind_appended` vs `nounwind_off` |
| `-fno-common` is neutral (`build_flags_research.md`) | **false at the current tree state**: it fails to link on duplicate `g_android_back_button_pressed`; the original likely used the clang 7 default `-fcommon` | `fno_common` |
| `-fPIC` is required because `-fpic` emits `R_ARM_REL32` (`cmake/toolchain.cmake` comment) | **not distinguishable**: `-fpic` is byte-identical to `-fPIC`; PIC is required, but the spelling is unknown | `fpic`, `nopic_off` |
| ndk-build r18b defaults are the best explanation (`definitive_build_recipe_w3697.md`) | **CMake r18b toolchain defaults are a closer match**: they have no `-fdata-sections`, default to `c++_static`, `armeabi-v7a`, API 16, thumb, and `-g`; the ndk-build hypothesis needs explicit overrides | §3 |
| `--section-ordering-file` omission is 1527/2723 (earlier notes) | **3031 at this tree state**, `.text 0x1337e8` | `no_order` |
| `.missioncase` was removed by w3570 (`docs/IDENTICALITY.md`) | **reintroduced by `6a98600e`**; current `.shstrtab` is 0x124, 28 sections | `readelf -SW`, `git log -S` |
| ground truth == shipped APK file (implicit in earlier notes) | **false**: APK file is GNU-stripped ground truth; 288-byte `.shstrtab`/`sh_name` delta | `cmp`, `strip` |

## 10. Reproduction commands (exact)

```sh
ROOT=/home/debian/GalaxyOnFire2/wt/w3750
NDK=$ROOT/.cache/ndk/android-ndk-r18b
O=$ROOT/../_work/bins/android_2.0.16_libgof2hdaa.so
APK="/home/debian/GalaxyOnFire2/Original APK/Galaxy on Fire 2™ HD_2.0.16_APKPure.apk"

# gate
cd $ROOT
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8
cmake --build cmake-build-match --target lint  -j8

# ELF metadata
readelf -hW $O; readelf -AW $O; readelf -nW $O; readelf -p .comment $O
readelf -dW $O; readelf -rW $O; readelf -SW $O

# APK vs ground truth
python3 - <<'PY'
import zipfile, hashlib
p="/home/debian/GalaxyOnFire2/Original APK/Galaxy on Fire 2™ HD_2.0.16_APKPure.apk"
d=zipfile.ZipFile(p).read("lib/armeabi-v7a/libgof2hdaa.so")
open("apk_lib.so","wb").write(d)
print(hashlib.sha256(d).hexdigest(), len(d))
PY
$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-strip \
  --strip-all -o gt_strip.so $O
cmp apk_lib.so gt_strip.so && echo "APK == strip(ground truth)"

# CMake toolchain probe
cmake -S $ROOT/cmake-build-w3750-harness/cmakeprobe \
  -B $ROOT/cmake-build-w3750-harness/cmakeprobe/build -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=$NDK/build/cmake/android.toolchain.cmake \
  -DANDROID_ABI=armeabi-v7a -DANDROID_PLATFORM=android-16 \
  -DANDROID_STL=c++_static -DANDROID_ARM_NEON=ON \
  -DCMAKE_BUILD_TYPE=Release
python3 -c "import json;print(json.load(open('$ROOT/cmake-build-w3750-harness/cmakeprobe/build/compile_commands.json'))[0]['command'])"
ninja -C $ROOT/cmake-build-w3750-harness/cmakeprobe/build -t commands libt.so | tail -1

# ndk-build probe
$NDK/ndk-build NDK_PROJECT_PATH=$ROOT/cmake-build-w3750-harness/ndktest \
  APP_BUILD_SCRIPT=$ROOT/cmake-build-w3750-harness/ndktest/jni/Android.mk \
  NDK_APPLICATION_MK=$ROOT/cmake-build-w3750-harness/ndktest/jni/Application.mk \
  V=1 -B

# compile A/B (one variant; see §4.2 for the wrapper)
H=$ROOT/cmake-build-w3750-harness
TMPDIR=$H/tmp cmake -S $ROOT -B $ROOT/cmake-build-w3750-<tag> -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=$H/toolchain_filter.cmake -DGOF2_MATCH_OPT=-Oz \
  -DGOF2_EXTRA_FLAGS='<extra>' -DGOF2_FILTER_FLAGS='<filter>'
export GOF2_FILTER_FLAGS='<filter>'
flock $H/vendor.lock env TMPDIR=$H/tmp \
  cmake --build $ROOT/cmake-build-w3750-<tag> --target gof2_libs -j8
TMPDIR=$H/tmp cmake --build $ROOT/cmake-build-w3750-<tag> --target link_so -j8
python3 $H/measure.py $ROOT/cmake-build-w3750-<tag> <tag>

# link A/B
python3 $H/relink.py $H/link_<tag>.so <tag> <gold args...>
python3 $H/link_measure.py $H/link_<tag>.so <tag>
python3 $H/verify_so.py $H/link_<tag>.so <tag>
```

## 11. Bottom-line answer to the task

1. **Toolchain/build system:** NDK r18b clang 7.0.2 + gold 1.12, API 16,
   armeabi-v7a softfp, Thumb-2, NEON, `c++_static`.  The most likely build
   driver is the NDK r18b CMake toolchain in Release with
   `ANDROID_ARM_NEON=ON` (~70%); ndk-build r18b with explicit
   `-fno-data-sections`/`c++_static`/NEON/RTTI overrides is the alternative
   (~25%).  Both produce the same effective flags.
2. **Effective compile line:** the NDK r18b defaults with `-fdata-sections`
   off; load-bearing `-Oz -g -std=c++11 -fstack-protector-strong
   -funwind-tables -ffunction-sections -mthumb`, NEON, RTTI, exceptions, PIC.
3. **Effective link line:** the NDK r18b defaults (`-z now`,
   SHA-1 `--build-id`, sysv hash, `--exclude-libs=libgcc.a`, zlib/libzip
   whole-archive, `--spare-dynamic-tags=5`, combreloc, no-as-needed,
   4 KiB pages, `-s`), with FMOD in `LOCAL_LDLIBS`/`target_link_libraries` after
   `-landroid` and before `-lm -ldl`.
4. **A/B results:** all requested flags tested; no flag is a strict
   improvement.  The neutral flags are byte-identical to the baseline except
   the build-id; the load-bearing flags and build failures are listed in §5-§7.
5. **No flag change landed.**  The report is the only commit.
