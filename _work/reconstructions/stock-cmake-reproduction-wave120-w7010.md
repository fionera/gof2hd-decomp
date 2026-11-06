# Stock NDK r18b CMake reproduction (wave 120, w7010)

Status: research note, 2026-11-06. Worktree `/home/debian/GalaxyOnFire2/wt/w7010`,
branch `wave120-w7010`, base `ef7fb9cde`. Scratch: `/dev/shm/w7010`.
No `src/`, `tools/`, baseline or allowlist change is landed. This note is the only commit.

Hypothesis under test: the shipped `libgof2hdaa.so` was built with the **stock**
`$NDK/build/cmake/android.toolchain.cmake` (not the repo's hand-written toolchain),
`CMAKE_BUILD_TYPE=Release`, `ANDROID_ABI=armeabi-v7a`, `ANDROID_PLATFORM=android-16`,
`ANDROID_STL=c++_static`, `ANDROID_ARM_NEON=ON`, and a normal `CMakeLists.txt`
(single shared-library target, explicit source list, `target_link_libraries` in the
original order).

## 0. Bottom line

**The hypothesis is supported, and for the effective build it is effectively proven.**

* The stock r18b CMake toolchain compiles the repo's 192 TUs to **byte-identical
  codegen objects**. After `--strip-debug`, all **192/192 objects are byte-identical**
  to the canonical build's objects. Before stripping, the only differences are DWARF
  (`DW_AT_comp_dir`, which records the build directory) and the debug-loc mapping
  symbols that follow from it. Every `.text.*`, `.rodata.*`, `.data.*`, `.ARM.exidx.*`,
  `.ARM.extab.*` and non-debug `.rel.*` section is identical.
* Stock toolchain + the repo's existing link recipe reproduces the canonical gate
  **exactly**: `byte 4505 / linked 4505 / size 4524/4524 / data 843/843 /
  imports 207 / parity 5370/5370 / avg 99.94`, `RATCHET PASS`. The linked `.so` is
  byte-identical to the canonical `.so` except the 20-byte `.note.gnu.build-id`
  (which hashes the unstripped file, and therefore the build directory in
  `DW_AT_comp_dir`).
* Stock toolchain + a **minimal single-target CMakeLists** with the original library
  order reproduces the original's dynamic tables **byte-for-byte**:
  `.dynsym`, `.dynstr`, `.hash`, `.gnu.version*` and `.rel.dyn` are identical to
  the original (`.rel.plt` has the same known 6-byte `MGame::OnTouchEnd` residual as
  the canonical build), the object order is preserved (`.text` identical to
  canonical), and
  **`.rodata` is exactly the original size (269,159 B) and byte-identical**. The
  canonical build's `+192` `.rodata` is the `tools/mission_case_table.cpp` matching
  device, which the original did not have.
* The only codegen-visible thing the minimal shape does *not* reproduce is the
  current tree's two per-file `-mllvm` branch-weight knobs (`StarMap.cpp`,
  `SpaceLounge.cpp`). They are reconstruction aids, not original build flags.
  Without them the gate falls to `byte 2404 / linked 4504 / size 4523/4524`.

Original sha256 `d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483`
(2,187,328 B). Canonical build sha256
`24b4a0b7c926d59a4744e57a64e714e18d719a5996fd8022188654be45248522`.

## 1. What was built, and exactly how

Two scratch builds were made under `/dev/shm/w7010`; both use the stock toolchain:

```sh
NDK=/home/debian/GalaxyOnFire2/wt/w7010/.cache/ndk/android-ndk-r18b
STOCK_TC=$NDK/build/cmake/android.toolchain.cmake
REPO=/home/debian/GalaxyOnFire2/wt/w7010
ORIG=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
FMOD=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
```

### 1.1 Experiment A — stock toolchain, repo `CMakeLists.txt` and link recipe

The repo `CMakeLists.txt` already only applies its own toolchain when
`CMAKE_TOOLCHAIN_FILE` is undefined, so it can be configured directly with the stock
toolchain. The stock toolchain does not define the repo's `GOF2_NDK_*` cache
variables, so they were supplied on the command line (all point into the same r18b
tree the canonical build uses, including the darwin-provenance `libc++abi.a` the repo
already installs on Linux):

```sh
cmake -S "$REPO" -B /dev/shm/w7010/stockA -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE="$STOCK_TC" \
  -DANDROID_ABI=armeabi-v7a \
  -DANDROID_PLATFORM=android-16 \
  -DANDROID_STL=c++_static \
  -DANDROID_ARM_NEON=ON \
  '-DANDROID_CPP_FEATURES=rtti exceptions' \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_CXX_FLAGS="-I$REPO/src -I$REPO/third_party/fmod/inc -I$REPO/third_party/gl \
    -I$REPO/third_party/jni -I$REPO/third_party/libzip -I$REPO/third_party/crypto \
    -Wno-int-to-pointer-cast -Wno-int-to-void-pointer-cast" \
  -DCMAKE_C_FLAGS="<same -I/-Wno set>" \
  -DGOF2_ORIGINAL_SO="$ORIG" -DGOF2_FMOD_LIBDIR="$FMOD" \
  -DGOF2_NDK_ROOT="$NDK" \
  -DGOF2_NDK_CLANG="$NDK/toolchains/llvm/prebuilt/linux-x86_64/bin/clang" \
  -DGOF2_NDK_AR="$NDK/toolchains/llvm/prebuilt/linux-x86_64/bin/llvm-ar" \
  -DGOF2_NDK_GCC_BINDIR="$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/arm-linux-androideabi/bin" \
  -DGOF2_NDK_GOLD="$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/arm-linux-androideabi/bin/ld.gold" \
  -DGOF2_NDK_STRIP="$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-strip" \
  -DGOF2_NDK_OBJDUMP="$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump" \
  -DGOF2_NDK_CRT_LIBDIR="$NDK/platforms/android-16/arch-arm/usr/lib" \
  -DGOF2_NDK_LIBCXXABI_A="$NDK/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/libc++abi.a" \
  -DGOF2_NDK_ANDROID_SUPPORT_A="$NDK/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/libandroid_support.a" \
  -DGOF2_NDK_LIBGCC_A="$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/lib/gcc/arm-linux-androideabi/4.9.x/armv7-a/thumb/libgcc.a"
cmake --build /dev/shm/w7010/stockA --target link_so -j64
```

The `-I`/`-Wno` flags are the only thing moved out of the repo toolchain: the repo
toolchain had them in `CMAKE_CXX_FLAGS`; the stock toolchain leaves `CMAKE_CXX_FLAGS`
empty for the user. They are include/warning flags, not codegen flags.

### 1.2 Experiment B — stock toolchain, minimal "original-shaped" CMakeLists

`/dev/shm/w7010/stockB/CMakeLists.txt` is a normal single-shared-library project:

```cmake
cmake_minimum_required(VERSION 3.10)
project(gof2hdaa LANGUAGES C CXX)

file(STRINGS "${GOF2_REPO}/tools/source_order.txt" _srcs)
list(TRANSFORM _srcs STRIP)
list(FILTER _srcs INCLUDE REGEX "^src/")
set(_abs_srcs "")
foreach(s ${_srcs})
  list(APPEND _abs_srcs "${GOF2_REPO}/${s}")
endforeach()

add_library(gof2hdaa SHARED ${_abs_srcs})
target_include_directories(gof2hdaa PRIVATE
  "${GOF2_REPO}/src" "${GOF2_REPO}/third_party/fmod/inc"
  "${GOF2_REPO}/third_party/gl" "${GOF2_REPO}/third_party/jni"
  "${GOF2_REPO}/third_party/libzip" "${GOF2_REPO}/third_party/crypto")
target_compile_options(gof2hdaa PRIVATE
  -Wno-int-to-pointer-cast -Wno-int-to-void-pointer-cast)

# the canonical tree's two matching knobs (not original build flags)
set_source_files_properties("${GOF2_REPO}/src/game/world/StarMap.cpp"
  PROPERTIES COMPILE_OPTIONS "-mllvm;-likely-branch-weight=7;-mllvm;-unlikely-branch-weight=1")
set_source_files_properties("${GOF2_REPO}/src/game/menu/SpaceLounge.cpp"
  PROPERTIES COMPILE_OPTIONS "-mllvm;-likely-branch-weight=7;-mllvm;-unlikely-branch-weight=1")

target_link_libraries(gof2hdaa PRIVATE
  -Wl,--whole-archive ${ZLIB_A} ${LIBZIP_A} -Wl,--no-whole-archive
  log GLESv2 GLESv1_CM EGL android
  "${FMOD}/libfmodex.so" "${FMOD}/libfmodevent.so"
  ${CRYPTO_A}
  m)

# the original's post-link GNU strip + .shstrtab normalization
add_custom_command(TARGET gof2hdaa POST_BUILD
  COMMAND "${STRIP}" --strip-all -o "${CMAKE_BINARY_DIR}/libgof2hdaa.stripped.so" "$<TARGET_FILE:gof2hdaa>"
  COMMAND ${CMAKE_COMMAND} -E env python3 "${GOF2_REPO}/tools/reorder_shstrtab.py"
          "$<TARGET_FILE:gof2hdaa>" "${CMAKE_BINARY_DIR}/libgof2hdaa.stripped.so")
```

Configured with the same stock-toolchain settings:

```sh
cmake -S /dev/shm/w7010/stockB -B /dev/shm/w7010/stockB/build -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE="$STOCK_TC" \
  -DANDROID_ABI=armeabi-v7a -DANDROID_PLATFORM=android-16 \
  -DANDROID_STL=c++_static -DANDROID_ARM_NEON=ON \
  '-DANDROID_CPP_FEATURES=rtti exceptions' \
  -DCMAKE_BUILD_TYPE=Release
ninja -C /dev/shm/w7010/stockB/build -j64
```

`ZLIB_A`/`LIBZIP_A`/`CRYPTO_A` are the repo's already-built
`third_party/{zlib,libzip,openssl}/lib*_gof2.a`; the stock CMake link does not rebuild
them. The post-link strip + `reorder_shstrtab.py` is added only so the section table
is directly comparable with the shipped original; it is what the canonical build
already does.

## 2. Compile-command diff

Canonical C++ command (repo toolchain), codegen flags only:

```text
clang++ -target armv7-none-linux-androideabi16
  -march=armv7-a -mthumb -mfpu=neon -mfloat-abi=softfp
  -std=c++11 -fPIC -frtti -fstack-protector-strong -Oz -g -stdlib=libc++
  -isystem <libc++>/include -isystem <libc++abi>/include -isystem <support>/include
  --sysroot=<ndk>/sysroot -isystem <sysroot>/usr/include
  -isystem <sysroot>/usr/include/arm-linux-androideabi
  -D__ANDROID_API__=16 -DANDROID -DNDEBUG
  -ffunction-sections -funwind-tables
```

Stock C++ command:

```text
clang++ --target=armv7-none-linux-androideabi16 --gcc-toolchain=<ndk>/...-4.9
  --sysroot=<ndk>/sysroot
  -isystem <libc++>/include -isystem <support>/include -isystem <libc++abi>/include
  -isystem <sysroot>/usr/include/arm-linux-androideabi
  -g -DANDROID -ffunction-sections -funwind-tables -fstack-protector-strong
  -no-canonical-prefixes
  -march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16 -mthumb -mfpu=neon
  -Wa,--noexecstack -Wformat -Werror=format-security
  -std=c++11 -frtti -fexceptions
  -Oz -DNDEBUG -fPIC
  -ffunction-sections -funwind-tables        # repo target_compile_options
```

Set difference:

| flag | canonical | stock | codegen-visible? |
|---|---|---|---|
| `-target` / `--target=` | `-target armv7-none-linux-androideabi16` | `--target=armv7-none-linux-androideabi16` | no (same triple) |
| `-D__ANDROID_API__=16` | explicit | implicit from the triple | no (`clang -dM -E` gives `__ANDROID_API__ 16`) |
| `-stdlib=libc++` | yes | no (`-nostdlib++` at link, explicit `-isystem` at compile) | no |
| `-fexceptions` | implicit (C++ default) | explicit | no |
| `-mfpu=` | `neon` | `vfpv3-d16` then `neon` (last wins) | no |
| `-isystem <sysroot>/usr/include` | explicit | via `--sysroot` | no |
| `-isystem <support>/include` | C and C++ | C++ only | no (C TUs do not include it) |
| `-no-canonical-prefixes`, `-Wa,--noexecstack`, `-Wformat -Werror=format-security`, `--gcc-toolchain` | absent | present | no |

All common codegen flags agree: `-march=armv7-a -mthumb -mfpu=neon
-mfloat-abi=softfp -std=c++11 -fPIC -frtti -fstack-protector-strong -Oz -g
-ffunction-sections -funwind-tables -DANDROID -DNDEBUG` plus the same libc++ /
libc++abi / android-support / sysroot include paths.

**Object proof.** Comparing all 192 `gof2` objects:

* every non-debug section (`.text.*`, `.rodata.*`, `.data.*`, `.ARM.exidx.*`,
  `.ARM.extab.*`, `.rel.*`) is byte-identical;
* `.symtab` differs only in local `NOTYPE` symbols that point into `.debug_loc`;
* `DW_AT_comp_dir` is the build directory (`/home/.../cmake-build-match` vs
  `/dev/shm/w7010/stockA`);
* after `arm-linux-androideabi-objcopy --strip-debug`, **192/192 objects are
  byte-identical** (sha256 equal).

So the stock toolchain's compile flags are codegen-equivalent to the repo's
hand-written flags on this tree.

## 3. Link-command diff

Repo manual gold link (canonical / Experiment A):

```text
ld.gold -shared -soname libgof2hdaa.so --build-id -z now --exclude-libs=libgcc.a
  crtbegin_so.o -L <platforms/android-16/arch-arm/usr/lib>
  <source_order objects>
  --whole-archive libz_gof2.a libzip_gof2.a --no-whole-archive
  -l log -l GLESv2 -l GLESv1_CM -l EGL -l android
  libfmodex.so libfmodevent.so libcrypto_gof2.a -l m
  libc++abi.a libandroid_support.a libgcc.a mission_table.o -l dl -l c crtend_so.o
then arm-linux-androideabi-strip --strip-all, then reorder_shstrtab.py
```

Stock CMake link (Experiment B):

```text
clang++ --target=... --sysroot=... -fPIC ... -Oz -DNDEBUG
  -Wl,--exclude-libs,libgcc.a -Wl,--exclude-libs,libatomic.a -nostdlib++
  --sysroot <platforms/android-16/arch-arm>
  -Wl,--build-id -Wl,--warn-shared-textrel -Wl,--fatal-warnings
  -Wl,--fix-cortex-a8 -Wl,--exclude-libs,libunwind.a
  -L<sources/cxx-stl/llvm-libc++/libs/armeabi-v7a> -Wl,--no-undefined
  -Wl,-z,noexecstack -Qunused-arguments -Wl,-z,relro -Wl,-z,now
  -shared -Wl,-soname,libgof2hdaa.so -o libgof2hdaa.so
  <source_order objects>
  -Wl,--whole-archive libz_gof2.a libzip_gof2.a -Wl,--no-whole-archive
  -llog -lGLESv2 -lGLESv1_CM -lEGL -landroid
  libfmodex.so libfmodevent.so libcrypto_gof2.a -lm
  -latomic -lm libc++_static.a libc++abi.a libandroid_support.a libunwind.a -ldl
  (crtbegin_so.o / crtend_so.o added by the clang driver)
then arm-linux-androideabi-strip --strip-all, then reorder_shstrtab.py
```

Differences that could in principle be link-visible:

* `-Wl,--fix-cortex-a8`, `-Wl,--warn-shared-textrel`, `-Wl,--fatal-warnings`,
  `-Wl,--no-undefined`, `-Wl,-z,noexecstack`, `-Wl,-z,relro`: no effect on the
  output (the link succeeds and the sections are identical).
* `-latomic`, `libc++_static.a`, `libunwind.a`: **not pulled** (the app has no
  undefined `std::*` and `libc++abi.a` does not leave `_Unwind_*` for
  `libunwind.a`), so no symbols are added. `_Unwind_*` stay `UND` in both.
* `-nostdlib++`, duplicate `-lm`, `-ldl` vs `-ldl -lc`: no effect on `.dynsym`.
* The minimal link has no `mission_table.o`; that is the only structural
  difference (see §4).

## 4. Linked `.so` section sizes

`stockA` = stock toolchain + repo link recipe. `stockB` = stock toolchain +
minimal CMakeLists (no `mission_table`). All four files are 2,187,328 B.

| section | original | canonical | stockA | stockB | stockB diff vs orig |
|---|---:|---:|---:|---:|---:|
| `.text` | 1,259,464 | 1,259,464 | 1,259,464 | 1,259,464 | 88,216 |
| `.rodata` | 269,159 | 269,351 | 269,351 | **269,159** | **0** |
| `.ARM.exidx` | 28,528 | 28,528 | 28,528 | 28,528 | 863 |
| `.ARM.extab` | 156,312 | 156,312 | 156,312 | 156,312 | 109,364 |
| `.rel.plt` | 29,392 | 29,392 | 29,392 | 29,392 | 6 |
| `.note.gnu.build-id` | 36 | 36 | 36 | 36 | 20 |
| `.dynsym` | 89,248 | 89,248 | 89,248 | 89,248 | **0** |
| `.dynstr` | 189,208 | 189,208 | 189,208 | 189,208 | **0** |
| `.hash` | 38,716 | 38,716 | 38,716 | 38,716 | **0** |
| `.gnu.version*` | 11,156 / 28 / 64 | same | same | same | **0** |
| `.rel.dyn` | 28,744 | 28,744 | 28,744 | 28,744 | **0** |
| `.plt` | 44,108 | 44,108 | 44,108 | 44,108 | **0** |
| `.data.rel.ro` | 12,264 | 12,264 | 12,264 | 12,264 | **0** |
| `.init_array` / `.fini_array` | 12 / 8 | 12 / 8 | 12 / 8 | 12 / 8 | **0** |
| `.dynamic` | 328 | 328 | 328 | 328 | **0** |
| `.got` | 17,164 | 17,164 | 17,164 | 17,164 | **0** |
| `.data` | 10,604 | 10,604 | 10,604 | 10,604 | **0** |
| `.bss` | 27,052 | 27,052 | 27,052 | 27,052 | 0 (NOBITS) |
| `.comment` | 303 | 303 | 303 | 303 | **0** |
| `.note.android.ident` | 152 | 152 | 152 | 152 | **0** |
| `.note.gnu.gold-version` | 28 | 28 | 28 | 28 | **0** |
| `.ARM.attributes` | 56 | 56 | 56 | 56 | **0** |
| `.shstrtab` | 279 | 279 | 279 | 279 | **0** |

* `stockA` vs canonical: **only** `.note.gnu.build-id` (20 bytes) differs. The
  build-id hashes the unstripped file, which contains `DW_AT_comp_dir`; the two
  build directories differ. Every code/data/EH/dynamic section is identical.
* `stockB` vs original: `.rodata` is **exact size and 0 differing bytes**; every
  dynamic/data/`.comment`/`.note`/`.ARM.attributes`/`.shstrtab` section is exact.
  The remaining differences are the known source-shape residuals
  (`.text` 88,216, `.ARM.exidx` 863, `.ARM.extab` 109,364, `.rel.plt` 6) and the
  build-id (20). These are the same residuals as the canonical build; they are not
  build-flag differences.
* The canonical `.rodata` is `+192` because `tools/mission_case_table.cpp` is
  appended as the last `.rodata` input. The original `.rodata` is exactly the
  269,159 B the app TUs + vendored archives already produce. `stockB` proves that
  the `mission_table` is a reconstruction device, not original data.

## 5. Gate counts

Official verifier (`tools/verify/verify.py`), same original, same objdump:

| build | byte_exact | linked_exact | size_exact | data_exact | imports | parity | avg |
|---|---:|---:|---:|---:|---:|---:|---:|
| canonical | **4505/4524** | **4505/4524** | **4524/4524** | **843/843** | **207** | **5370/5370** | 99.94 |
| stockA (repo link) | **4505/4524** | **4505/4524** | **4524/4524** | **843/843** | **207** | **5370/5370** | 99.94 |
| stockB (minimal CMake link) | **4505/4524** | **4505/4524** | **4524/4524** | **843/843** | **207** | **5370/5370** | 99.94 |
| stockB3 (minimal, no per-file branch weights) | 2404/4524 | 4504/4524 | 4523/4524 | 843/843 | 207 | 5370/5370 | 99.93 |

`stockA` and `stockB` both print `RATCHET PASS` against
`tools/verify_baseline.json` (byte/linked/stub/missing/extra/imports/lint). `lint` is
unchanged (`CLEAN`), `drift` is unchanged (0) because no source changed.

The `stockB3` row is the important caveat: the current tree still needs the two
per-file `-mllvm -likely-branch-weight=7 -mllvm -unlikely-branch-weight=1` knobs on
`StarMap.cpp` and `SpaceLounge.cpp` to reproduce the canonical gate. They are
matching knobs (the CMakeLists says so), not stock r18b build flags; the original
source shape did not need them.

## 6. Link order and `.dynsym`

The minimal shape uses `add_library(gof2hdaa SHARED ${_abs_srcs})`, so the object
order is the explicit `tools/source_order.txt` list. The resulting `.text` is
**byte-identical to the canonical `.text`**, i.e. the original TU order is preserved.

`.dynsym` is determined by gold's first-mention order over the link inputs. Two
minimal-shape variants were built:

* **original order** (`libz/libzip whole`, `-llog -lGLESv2 -lGLESv1_CM -lEGL
  -landroid`, FMOD, libcrypto, `-lm`, then the STL group): `.dynsym` is **exactly
  equal to the original** — every `(name, st_value, st_size, st_shndx)` tuple is
  identical, and the raw `.dynsym`/`.dynstr`/`.hash` sections are byte-identical.
* **FMOD before the system libs** (same set, different order): the symbol set still
  passes `sodiff` (0 extra / 0 missing), but `.dynsym` order changes — the first
  differing index is **647** (`raise` vs `_ZN11AbyssEngine8AERandomC1Ex`).

So the original's `target_link_libraries` order is recoverable from `.dynsym`, and
the order used by the canonical link is the one that reproduces it.

## 7. Proven / disproven / unobservable

### Proven

* The shipped `.so` is an NDK r18b / clang 7.0.2 / gold 1.12 build: API 16,
  armeabi-v7a, Thumb-2, VFPv3, NEONv1, softfp, static libc++, `-Oz -g`,
  RTTI + exceptions, `-fstack-protector-strong`, `-ffunction-sections`,
  `-funwind-tables`, no `-fdata-sections`, default visibility.
  `.note.android.ident`, `.comment`, `.ARM.attributes` and `.note.gnu.gold-version`
  are byte-identical in the stock build.
* The **stock r18b CMake toolchain** (`Release`, `armeabi-v7a`, `android-16`,
  `c++_static`, `ANDROID_ARM_NEON=ON`) is codegen-equivalent to the repo's
  hand-written toolchain: 192/192 objects identical after `--strip-debug`, and the
  linked `.so` is identical except the build-id.
* `ANDROID_ARM_NEON=ON` is required: r18b defaults NEON off for API < 23, and the
  original's `.ARM.attributes` has `Tag_Advanced_SIMD_arch: NEONv1` (the stock
  build with the setting reproduces it byte-for-byte).
* API 16 is required and is selected by `ANDROID_PLATFORM=android-16`; the stock
  triple `armv7-none-linux-androideabi16` defines `__ANDROID_API__=16` implicitly.
* Release is required: `-Oz -DNDEBUG` is what the original codegen shows.
* A **normal single-target CMakeLists** with the original library order reproduces
  the original's `.dynsym`, `.dynstr`, `.hash`, `.rel.dyn`, `.data`,
  `.data.rel.ro`, `.got`, `.plt`, `.comment`, `.note.*`, `.ARM.attributes`,
  `.shstrtab` and **exact `.rodata` size/content**. The original `.rodata` has no
  extra 192-byte table; the `mission_table` is a reconstruction device.
* The original's link order is the canonical order: reordering FMOD changes
  `.dynsym` order at index 647.

### Disproven

* Nothing in the hypothesis is contradicted. In particular, there is no evidence
  for `-fdata-sections` (the stock CMake toolchain does not add it), no evidence
  for a different ABI/API/STL, and no evidence that the original's `.rodata`
  contains the `mission_table`.

### Unobservable

* The literal `CMakeLists.txt` text: no build file survives in the APK/IPA/source
  dumps. The binary proves the **effective** flags and link order, not the file.
* CMake vs a hand-written ndk-build with equivalent overrides: CMake is the only
  stock r18b build system that reproduces the effective flags/link order without
  overrides, but a hand-written ndk-build with `APP_STL := none`,
  `-fno-data-sections`, explicit `libc++abi`/`libandroid_support` after FMOD and a
  mini `libgcc.a` cannot be excluded from the binary alone.
* Whether `ANDROID_CPP_FEATURES="rtti exceptions"` was explicit: clang 7 defaults
  both on, so the objects are identical either way.
* Whether `-g` was explicit or a toolchain default: both stock CMake and ndk-build
  add it, and it is codegen-relevant at `-Oz`; the binary proves it was present.
* Whether `libc++_static.a` / `libunwind.a` / `-latomic` were on the link line: they
  are not pulled (no undefined `std::*`; `_Unwind_*` stay `UND`), so their presence
  or absence is invisible in the output.
* The exact spelling of the whole-archive group (`target_link_libraries` with
  `-Wl,--whole-archive` items vs `target_link_options`): only the effective order
  matters, and the original order is the one that reproduces `.dynsym`.
* The exact post-link strip command: the section headers prove GNU
  `strip --strip-all` followed by `.shstrtab` normalization, which is what both
  scratch builds use.

## 8. Reproduction commands

```sh
# canonical reference (already green)
cd /home/debian/GalaxyOnFire2/wt/w7010
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j32

# Experiment A: stock toolchain + repo CMakeLists/link recipe
/dev/shm/w7010/configure_stockA.sh
cmake --build /dev/shm/w7010/stockA --target link_so -j64

# Experiment B: stock toolchain + minimal single-target CMakeLists
/dev/shm/w7010/configure_stockB.sh
ninja -C /dev/shm/w7010/stockB/build -j64

# gate counts for a scratch build
python3 tools/verify/verify.py \
  --build-dir /dev/shm/w7010/verify-stockB \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /dev/shm/w7010/stockB/build/libgof2hdaa.stripped.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 tools/sodiff.py \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /dev/shm/w7010/stockB/build/libgof2hdaa.stripped.so \
  --allow tools/sodiff_allow.txt

# object identity (all 192 objects)
#   canonical: cmake-build-match/CMakeFiles/gof2.dir/**/*.obj
#   stockA:    /dev/shm/w7010/stockA/CMakeFiles/gof2.dir/**/*.o
#   objcopy --strip-debug each pair, then sha256/cmp -> 192/192 identical
```

Scratch files (not committed): `/dev/shm/w7010/{env.sh,configure_stockA.sh,
configure_stockB.sh,stockA,stockB,stockB2,stockB3,gateA,gateB,verify-*}`.
