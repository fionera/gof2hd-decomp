# Build-system reconstruction: ndk-build vs CMake (wb105, wave 101)

Status: research note, 2026-11-04. Worktree `/home/debian/GalaxyOnFire2/wt/wb105`,
branch `wave101-wb105`, base `3856949ab`. Scratch is `/dev/shm/wb105`.
Every number below is from a command run against the shipped
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so` or from a
controlled A/B rebuild of this tree with the official verifier. No `src/` or build-flag
change is landed; the canonical build is already the best match. The deliverable is this
note.

Canonical gate, measured before and after this pass:

```text
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.91
data:   exact 843 of 843 exported objects
sodiff: original 5370  ours 5370  extra 0  missing 0
imports: 207   size-exact: 4524/4524   lint: CLEAN   drift: 0
RATCHET PASS
```

Original sha256 `d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483`;
canonical build sha256 `08aca26fd213a52c8d1f7f84b4500ad000535312814e1cff4c864f98488fba6f`.

## 0. Bottom line

* The shipped ELF is an **NDK r18b** build: clang 7.0.2, gold 1.12, API 16,
  armeabi-v7a, Thumb-2, VFPv3, NEONv1, softfp, static libc++, RTTI + exceptions,
  `-Oz -g`.
* Its effective compile/link flags are the **r18b CMake toolchain defaults** plus three
  explicit settings: `ANDROID_ARM_NEON=ON`, `ANDROID_CPP_FEATURES="rtti exceptions"`,
  `CMAKE_BUILD_TYPE=Release`. The two observable CMake-vs-ndk-build discriminators both
  point at CMake:
  * **no `-fdata-sections`** (CMake r18b default; ndk-build r18b default adds it);
  * **STL archives after the user/shared libraries** (CMake default; ndk-build puts
    static archives before shared libraries, which pulls `libunwind.a` and defines
    `_Unwind_*` locally).
* The ndk-build r18b default compile command and link order are both **refuted** by the
  binary. A real ndk-build default build of this tree gives **byte 1345 / linked 3843**;
  the canonical CMake build gives **4500 / 4500**.
* The full ndk-build r18b compile set **minus `-fdata-sections`** is byte-identical to
  the canonical build except the 20-byte `.note.gnu.build-id`; the full ndk-build r18b
  link *flag* set is byte-identical to the canonical link.
* `-ffunction-sections` is present and load-bearing: removing it drops the gate to
  **1345 / 4357**, shrinks `.text` by `0x670`, and cuts the CANTUNWIND sentinels from
  **272 to 83** (the 193 empty-`.text` sentinels that share a real function address drop
  from **193 to 51**).
* `-fdata-sections` is absent: adding it drops the gate to **4311 / 4500**, changes
  `.rodata` from `0x41b67` to `0x41a07`, and changes `.data` / `.data.rel.ro`.
* No flag or source change is a strict improvement, so nothing was landed.

## 1. Baseline and artifact metadata

```sh
cd /home/debian/GalaxyOnFire2/wt/wb105
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8
```

```text
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.91
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
=== DRIFT: 0 field(s) across 0 class(es) ===
```

The original's hard metadata (commands in §7):

| observable | value | conclusion |
|---|---|---|
| ELF | `ELF32 DYN ARM EABI5`, flags `0x5000200` (`EF_ARM_ABI_FLOAT_SOFT`) | armeabi-v7a **softfp** |
| `.note.android.ident` | API `16`, `r18b`, build `5063045` | NDK r18b, API 16 |
| `.comment` | clang 7.0.2 `4751641 based on r328903` + `GCC: (GNU) 4.9.x 20150123` | clang app TUs + GCC 4.9 libcrypto |
| `.note.gnu.gold-version` | `gold 1.12` | NDK gold linker |
| `.ARM.attributes` | ARM v7/A, Thumb-2, VFPv3, NEONv1, GOT-indirect, wchar_t 4, enum int, 8-byte align, unaligned v6 | `-march=armv7-a -mthumb -mfpu=neon -mfloat-abi=softfp -fPIC`; no `-fshort-enums`, no `-mno-unaligned-access` |
| `DT_NEEDED` | `liblog libGLESv2 libGLESv1_CM libEGL libandroid libfmodex libfmodevent libm libdl libc` | link-line order |
| `DT_FLAGS` / `DT_FLAGS_1` | `BIND_NOW` / `NOW` | `-z now` |
| `DT_SONAME` | `libgof2hdaa.so` | `-soname libgof2hdaa.so` |
| `.hash` only | no `.gnu.hash` | gold `--hash-style=sysv` default |
| defined / undefined dynsym | 5370 / 208 | symbol parity 5370/5370, 0 extra / 0 missing |

Section comparison, original vs canonical build:

| section | original | ours | differing bytes |
|---|---:|---:|---:|
| `.note.android.ident` | 152 | 152 | **0** |
| `.comment` | 303 | 303 | **0** |
| `.ARM.attributes` | 56 | 56 | **0** |
| `.note.gnu.gold-version` | 28 | 28 | **0** |
| `.dynstr` | 189208 | 189208 | **0** |
| `.hash` | 38716 | 38716 | **0** |
| `.rodata` | 269159 | 269159 | **0** |
| `.data` | 10604 | 10604 | **0** |
| `.data.rel.ro` | 12264 | 12264 | **0** |
| `.got` | 17164 | 17164 | **0** |
| `.dynamic` | 328 | 328 | **0** |
| `.init_array` / `.fini_array` | 12 / 8 | 12 / 8 | **0** |
| `.rel.dyn` | 28744 | 28744 | **0** |
| `.rel.plt` | 29392 | 29392 | **0** |
| `.dynsym` | 89248 | 89248 | 661 (`st_shndx` only; extra `.missioncase` section shifts section indices) |
| `.text` | 1259464 | 1259464 | 101704 (24 non-exact bodies) |
| `.ARM.exidx` | 28528 | 28528 | 1637 (LSDA shifts) |
| `.ARM.extab` | 156312 | 156308 | size -4 (known EH residual) |
| `.shstrtab` | 279 | 292 | extra `.missioncase` name |

The `.dynsym` 661-byte difference is not a build-flag difference: every symbol name,
value and size is identical; only `st_shndx` differs for symbols in sections after the
extra `.missioncase` section. The gate's `sodiff` is 0 extra / 0 missing.

### 1.1 `.note.android.ident` and `.comment` provenance

```sh
N=.cache/ndk/android-ndk-r18b
readelf -nW $N/platforms/android-16/arch-arm/usr/lib/crtbegin_so.o
readelf -p .comment $N/platforms/android-16/arch-arm/usr/lib/crtbegin_so.o
ar x third_party/openssl/libcrypto_gof2.a sha256.o
readelf -p .comment sha256.o
```

Measured:

* `.note.android.ident` is **byte-identical** (152/152 bytes) to the r18b
  `platforms/android-16/arch-arm/usr/lib/crtbegin_so.o` note.
* `.comment` is exactly `crtbegin_so.o`'s 264-byte clang string concatenated with
  `libcrypto_gof2.a(sha256.o)`'s 40-byte GCC string minus the duplicate leading NUL:
  `264 + 40 - 1 = 303` bytes, byte-for-byte equal to the original.
* The pulled `libgcc.a` members (`_aeabi_ldivmod.o`, `_dvmd_lnx.o`) have no `.comment`,
  so the GCC string is from libcrypto, not libgcc.

## 2. CMake r18b defaults vs ndk-build r18b defaults

The r18b CMake toolchain is
`.cache/ndk/android-ndk-r18b/build/cmake/android.toolchain.cmake`; the ndk-build
defaults are in `.cache/ndk/android-ndk-r18b/build/core/`. The decisive differences:

| setting | CMake r18b default | ndk-build r18b default | original | verdict |
|---|---|---|---|---|
| ABI | `armeabi-v7a` | all default ABIs | `armeabi-v7a` only | both possible |
| platform | `android-16` (minimum) | `android-16` (minimum) | API 16 | both |
| STL | `c++_static` | `system` (libstdc++) | static libc++ | CMake default; ndk-build needs `APP_STL := c++_static` |
| ARM mode | `thumb` | `thumb` | Thumb-2 | both |
| NEON | `FALSE` for API < 23 | `LOCAL_ARM_NEON` false | NEONv1 | **explicit ON** in both |
| C++ features | empty | empty | RTTI + exceptions | **explicit** in both |
| build type | empty | `release` | `-Oz -DNDEBUG` | **explicit Release** in CMake |
| `-ffunction-sections` | **yes** | **yes** | yes | both |
| `-fdata-sections` | **no** | **yes** | **no** | **CMake** |
| `-fstack-protector-strong` | yes | yes | yes | both |
| `-funwind-tables` | yes | yes | yes | both |
| `-g` | yes | yes | yes | both |
| `-DANDROID` | yes | yes | unobservable | both |
| `-std=c++11` | yes (for `c++_static`) | yes (for `c++_static`) | C++11 codegen | both |
| `-Wl,--fix-cortex-a8` | yes | yes | unobservable | both |
| `-Wl,--build-id` | yes | yes | yes | both |
| `-Wl,-z,noexecstack -z relro -z now` | yes | yes | yes | both |
| `-Wl,--no-undefined` | yes | no | unobservable | CMake |
| `-Wl,--exclude-libs,libgcc.a` | yes | yes | yes | both |
| `-Wl,--exclude-libs,libatomic.a` | yes | no | unobservable | CMake |
| `-Wl,--exclude-libs,libunwind.a` | yes | no | unobservable | CMake |
| STL archives after user libs | **yes** | **no** (static before shared) | yes | **CMake** |

### 2.1 Exact CMake r18b default command lines

A minimal probe (`/dev/shm/wb105/cmakeprobe`) was configured with the r18b toolchain and
the three explicit settings:

```sh
cmake -S /dev/shm/wb105/cmakeprobe -B /dev/shm/wb105/cmakeprobe/build -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/home/debian/GalaxyOnFire2/wt/wb105/.cache/ndk/android-ndk-r18b/build/cmake/android.toolchain.cmake \
  -DANDROID_ABI=armeabi-v7a -DANDROID_PLATFORM=android-16 -DANDROID_STL=c++_static \
  -DANDROID_ARM_NEON=ON '-DANDROID_CPP_FEATURES=rtti exceptions' \
  -DCMAKE_BUILD_TYPE=Release
ninja -C /dev/shm/wb105/cmakeprobe/build -v
```

Compile command (abridged to the codegen flags):

```text
clang++ --target=armv7-none-linux-androideabi16 --gcc-toolchain=... --sysroot=...
  -g -DANDROID -ffunction-sections -funwind-tables -fstack-protector-strong
  -no-canonical-prefixes -march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16
  -mthumb -mfpu=neon -Wa,--noexecstack -Wformat -Werror=format-security
  -std=c++11 -frtti -fexceptions -Oz -DNDEBUG -fPIC ...
```

Link command (abridged to the library order):

```text
clang++ ... -Wl,--exclude-libs,libgcc.a -Wl,--exclude-libs,libatomic.a
  -nostdlib++ --sysroot=... -Wl,--build-id -Wl,--warn-shared-textrel
  -Wl,--fatal-warnings -Wl,--fix-cortex-a8 -Wl,--exclude-libs,libunwind.a
  -L.../libs/armeabi-v7a -Wl,--no-undefined -Wl,-z,noexecstack
  -Qunused-arguments -Wl,-z,relro -Wl,-z,now -shared -Wl,-soname,libprobe.so
  -o libprobe.so probe.cpp.o <user libs, e.g. FMOD>
  -latomic -lm libc++_static.a libc++abi.a libandroid_support.a libunwind.a -ldl
```

The CMake default compile command contains **no `-fdata-sections`** and the CMake
default link line places the **user libraries (FMOD) before `libc++_static.a` /
`libc++abi.a`**. Both match the original.

### 2.2 Exact ndk-build r18b default command lines

A minimal ndk-build probe (`/dev/shm/wb105/ndkprobe`) with
`APP_ABI := armeabi-v7a`, `APP_PLATFORM := android-16`, `APP_STL := c++_static`,
`APP_OPTIM := release`, `LOCAL_CPP_FEATURES := rtti exceptions`,
`LOCAL_ARM_NEON := true`:

```sh
.cache/ndk/android-ndk-r18b/ndk-build \
  NDK_PROJECT_PATH=/dev/shm/wb105/ndkprobe \
  APP_BUILD_SCRIPT=/dev/shm/wb105/ndkprobe/jni/Android.mk \
  NDK_APPLICATION_MK=/dev/shm/wb105/ndkprobe/jni/Application.mk V=1 -j8
```

Compile command (abridged):

```text
clang++ -target armv7-none-linux-androideabi16
  -fdata-sections -ffunction-sections -fstack-protector-strong -funwind-tables
  -no-canonical-prefixes -g -Wno-invalid-command-line-argument
  -Wno-unused-command-line-argument -fno-exceptions -fno-rtti
  -gcc-toolchain ... -fpic -march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16
  -mthumb -Oz -DNDEBUG -mfpu=neon -std=c++11 -DANDROID -Wa,--noexecstack
  -Wformat -Werror=format-security -frtti -fexceptions --sysroot=...
```

Link command (abridged):

```text
clang++ -Wl,-soname,libprobe.so -shared --sysroot=...
  probe.o libc++_static.a libc++abi.a libandroid_support.a libunwind.a
  -lgcc -Wl,--exclude-libs,libgcc.a -latomic -Wl,--exclude-libs,libatomic.a
  ... -Wl,--fix-cortex-a8 -Wl,--exclude-libs,libunwind.a -Wl,--build-id
  -nostdlib++ -Wl,--no-undefined -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now
  -Wl,--warn-shared-textrel -Wl,--fatal-warnings
  -L.../platforms/android-16/arch-arm/usr/lib -llog -ldl -lc -lm
```

The ndk-build default compile command contains **`-fdata-sections`**, and its link
order places the **STL static archives before the shared libraries**, so
`libunwind.a` is pulled and `_Unwind_*` are defined locally. Both disagree with the
original.

## 3. Controlled A/B measurements

### 3.1 Compile flags (official verifier)

The A/B harness is a scratch toolchain wrapper that appends the ndk-build flag set to
the canonical CMake flags. The canonical `cmake-build-match` build is never modified;
`--update-baseline` is never run. The exact command is in §7.

| variant | byte | linked | size | `.text` | `.rodata` | `.data` | `.data.rel.ro` | verdict |
|---|---:|---:|---:|---:|---:|---:|---:|---|
| canonical CMake (`-Oz -g -fPIC -frtti -fstack-protector-strong -ffunction-sections -funwind-tables`) | **4500** | **4500** | 4524/4524 | `0x1337c8` | `0x41b67` | `0x296c` | 0 diff | reference |
| full ndk-build compile set **with** `-fdata-sections` | 4311 | 4500 | 4524/4524 | `0x1337c8` | `0x41a07` | `0x295c` | 365 diff | **-189 byte** |
| full ndk-build compile set **minus** `-fdata-sections` | **4500** | **4500** | 4524/4524 | `0x1337c8` | `0x41b67` | `0x296c` | 0 diff | byte-identical except build-id |
| `-fdata-sections` alone | 4311 | 4500 | 4524/4524 | `0x1337c8` | `0x41a07` | `0x295c` | 365 diff | **-189 byte** |
| `-fdata-sections -fno-data-sections` | **4500** | **4500** | 4524/4524 | `0x1337c8` | `0x41b67` | `0x296c` | 0 diff | byte-identical except build-id |
| no `-ffunction-sections` (all TUs) | 1345 | 4357 | 4524/4524 | `0x133158` | `0x41b67` | `0x296c` | 0 diff | **-3155 byte** |

Notes:

* The full ndk-build compile set minus `-fdata-sections` differs from the canonical
  `.so` in exactly **19 bytes**, all inside the 20-byte `.note.gnu.build-id` descriptor
  (file offsets 477–496). Every code/data/EH section is byte-identical.
* `-fdata-sections` changes `.rodata` by **114,287 bytes including the 352-byte size
  difference**, `.data` by **3,039 bytes including the 16-byte size difference**, and
  `.data.rel.ro` by **365 bytes**. The original's data layout is the no-fdata layout.
* `-ffunction-sections` is proven independently by `.ARM.exidx`: the original has
  **3566 entries, 272 CANTUNWIND, 194 duplicate target addresses, and 193 CANTUNWIND
  entries sharing a target with a real entry** (gold's empty-`.text` sentinel). Removing
  `-ffunction-sections` gives **3366 entries, 83 CANTUNWIND, 52 duplicate targets, 51
  CANT sharing a real target**, and shrinks `.text` by `0x670` (`0x1337c8` →
  `0x133158`). At object level, `Engine.cpp` goes from **394 section headers with an
  empty plain `.text` (0 B)** to **92 section headers with a monolithic `.text`
  (`0x2a90` B)**.

### 3.2 Link flags and link order (same canonical objects)

The canonical gold command was extracted with
`ninja -C cmake-build-match -t commands libgof2hdaa.so`, then re-run with one change.
The stripped outputs were compared by sha256 and the official verifier.

| relink variant | sha256 | byte | linked | extra | verdict |
|---|---|---:|---:|---:|---|
| canonical | `08aca26f…` | **4500** | **4500** | 0 | reference |
| full ndk-build link **flag** set (`-z relro -z noexecstack --warn-shared-textrel --fatal-warnings --fix-cortex-a8 --no-undefined --exclude-libs=libatomic.a --exclude-libs=libunwind.a`) | `08aca26f…` | **4500** | **4500** | 0 | **byte-identical** |
| full CMake default STL group (`libc++_static.a` before `libc++abi.a`, `libunwind.a` after `libandroid_support.a`, `libatomic.a` before `-l m`) | `08aca26f…` | **4500** | **4500** | 0 | **byte-identical** |
| ndk-build default link **order** (STL archives + `libgcc` + `libatomic` before the shared libraries) | `80a2f31c…` | 1345 | 4404 | 29 | **refuted** |

The ndk-build-order relink defines `_Unwind_Resume`, `_Unwind_RaiseException`,
`_Unwind_VRS_Get/Set`, `_Unwind_GetLanguageSpecificData`, `_Unwind_DeleteException`,
`_Unwind_GetRegionStart`, etc. locally and adds `__gnu_Unwind_Find_exidx` as `UND`
(29 extra exports). The original has all `_Unwind_*` as `UND` and no
`__gnu_Unwind_Find_exidx`. It also grows `.text` from `0x1337c8` to `0x135d20`,
`.ARM.exidx` from `0x6f70` to `0x7110`, and `.rodata` from `0x41b67` to `0x41ff7`.

### 3.3 Real ndk-build default build of the tree

A scratch ndk-build project (`/dev/shm/wb105/ndkfull`) compiled all 192 app TUs with
the reconstructed `Android.mk`/`Application.mk` (`APP_STL := c++_static`,
`APP_OPTIM := release`, `LOCAL_CPP_FEATURES := rtti exceptions`,
`LOCAL_ARM_NEON := true`, FMOD in `LOCAL_LDLIBS`, zlib/libzip whole-static, crypto
static):

```sh
.cache/ndk/android-ndk-r18b/ndk-build \
  NDK_PROJECT_PATH=/dev/shm/wb105/ndkfull \
  APP_BUILD_SCRIPT=/dev/shm/wb105/ndkfull/jni/Android.mk \
  NDK_APPLICATION_MK=/dev/shm/wb105/ndkfull/jni/Application.mk V=1 -j8
python3 tools/verify/verify.py \
  --build-dir /dev/shm/wb105/verify-ndkfull \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /dev/shm/wb105/ndkfull/libs/armeabi-v7a/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
```

```text
compared 4524  avg 99.71%  byte_exact 1345  linked_exact 3843  fuzzy_perfect 3864
stub_zero_size 0  orig_zero_size 0  missing 0 (wrong_type 0  absent 0)  extra 0
```

Sections: `.text 0x135f80`, `.ARM.exidx 0x7110`, `.ARM.extab 0x263fc`,
`.rodata 0x41e97`, `.data 0x2960`, `.bss 0x69b0`, `.dynsym 0x15b80`.
`DT_NEEDED` order is exact (`liblog libGLESv2 libGLESv1_CM libEGL libandroid
libfmodex libfmodevent libm libdl libc`). The gap is the ndk-build default
`-fdata-sections` plus the static-before-shared STL link order, not a compiler
difference.

## 4. Reconstructed build files

### 4.1 CMake recipe (best supported by the binary)

```cmake
# CMakeLists.txt (essential parts)
cmake_minimum_required(VERSION 3.10)
project(gof2hdaa CXX)
add_library(gof2hdaa SHARED ${GOF2_SOURCES})
target_compile_features(gof2hdaa PRIVATE cxx_std_11)
target_link_libraries(gof2hdaa PRIVATE
  # user libs, in the original link order
  ${FMOD_DIR}/libfmodex.so ${FMOD_DIR}/libfmodevent.so
  ${CRYPTO_A}
  # zlib/libzip are whole-archive
)
target_link_options(gof2hdaa PRIVATE
  -Wl,--whole-archive ${ZLIB_A} ${LIBZIP_A} -Wl,--no-whole-archive)
```

Configure with:

```sh
cmake -S . -B build -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=$NDK/build/cmake/android.toolchain.cmake \
  -DANDROID_ABI=armeabi-v7a \
  -DANDROID_PLATFORM=android-16 \
  -DANDROID_STL=c++_static \
  -DANDROID_ARM_NEON=ON \
  '-DANDROID_CPP_FEATURES=rtti exceptions' \
  -DCMAKE_BUILD_TYPE=Release
```

This reproduces the original's effective flags and link order without any flag
overrides. `ANDROID_ARM_NEON=ON` and `ANDROID_CPP_FEATURES="rtti exceptions"` are the
only non-default CMake settings; `CMAKE_BUILD_TYPE=Release` is needed to get
`-Oz -DNDEBUG`.

### 4.2 ndk-build recipe (if the original used ndk-build)

A plain ndk-build default build does **not** match. To match the binary with ndk-build,
the following overrides are required:

```make
# Application.mk
APP_ABI := armeabi-v7a
APP_PLATFORM := android-16
APP_STL := none            # avoid libc++_static.a/libunwind.a before the shared libs
APP_OPTIM := release
```

```make
# Android.mk
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := zlib
LOCAL_SRC_FILES := third_party/zlib/libz_gof2.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libzip
LOCAL_SRC_FILES := third_party/libzip/libzip_gof2.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := crypto
LOCAL_SRC_FILES := third_party/openssl/libcrypto_gof2.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := gof2hdaa
LOCAL_SRC_FILES := <the original TU list; see tools/source_order.txt>
LOCAL_CPP_FEATURES := rtti exceptions
LOCAL_ARM_NEON := true
LOCAL_C_INCLUDES := $(LOCAL_PATH)/src \
    $(LOCAL_PATH)/third_party/fmod/inc \
    $(LOCAL_PATH)/third_party/gl \
    $(LOCAL_PATH)/third_party/jni \
    $(LOCAL_PATH)/third_party/libzip \
    $(LOCAL_PATH)/third_party/crypto \
    <ndk>/sources/cxx-stl/llvm-libc++/include \
    <ndk>/sources/cxx-stl/llvm-libc++abi/include \
    <ndk>/sources/android/support/include
LOCAL_CFLAGS := -Wno-int-to-pointer-cast -Wno-int-to-void-pointer-cast -fno-data-sections
LOCAL_CPPFLAGS := -std=c++11 -stdlib=libc++
# These are the r18b ndk-build link defaults; they are optional (the binary is
# byte-identical with or without them on the canonical objects).
LOCAL_LDFLAGS := -Wl,--fix-cortex-a8 -Wl,-z,relro -Wl,-z,now -Wl,--build-id \
                 -Wl,--no-undefined -Wl,-z,noexecstack -Wl,--warn-shared-textrel \
                 -Wl,--fatal-warnings -Wl,--exclude-libs=libgcc.a
LOCAL_LDLIBS := -llog -lGLESv2 -lGLESv1_CM -lEGL -landroid \
                -L<apk-libs> -lfmodex -lfmodevent \
                -L<ndk>/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a \
                -lc++abi -landroid_support \
                -L<ndk>/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/lib/gcc/arm-linux-androideabi/4.9.x/armv7-a/thumb \
                -lgcc -ldl -lc
LOCAL_WHOLE_STATIC_LIBRARIES := zlib libzip
LOCAL_STATIC_LIBRARIES := crypto
include $(BUILD_SHARED_LIBRARY)
```

The binary's STL is static libc++ (`c++_static`). In the ndk-build recipe,
`APP_STL := none` is used only to keep ndk-build from placing
`libc++_static.a`/`libunwind.a` before the shared libraries; the explicit
`-lc++abi -landroid_support` supply the same libc++abi code. A vanilla
`APP_STL := c++_static` build is the plain ndk-build default measured in §3.3
(1345/3843), because it pulls `libunwind.a` and defines `_Unwind_*` locally.

Even with these overrides, ndk-build still places its own `-lgcc`/`-latomic` before
the shared libraries. To reproduce the original's `.text` tail exactly, the original
workaround was a two-member `libgcc.a` (`_aeabi_ldivmod.o`, `_dvmd_lnx.o`) on a `-L`
path and a renamed `crtbegin_so.o` (`-B`); see
`_work/reconstructions/ndkbuild_android_mk_w3568.md`. This is evidence that the
ndk-build route requires non-default overrides, while the CMake route matches with
defaults.

## 5. Proven vs hypothesis

### Proven by the shipped artifact and the controlled A/B

* NDK r18b, clang 7.0.2, gold 1.12, API 16, armeabi-v7a, Thumb-2, VFPv3, NEONv1,
  softfp, static libc++, RTTI + exceptions, `-Oz -g`, `-fstack-protector-strong`,
  `-funwind-tables`, `-ffunction-sections`, no `-fdata-sections`, PIC, default
  visibility.
* `.note.android.ident` is byte-identical to the r18b API-16 `crtbegin_so.o` note.
* `.comment` is exactly `crtbegin_so.o` + `libcrypto_gof2.a(sha256.o)` `.comment`.
* `.ARM.attributes` is byte-identical; `DT_NEEDED` order is identical; exports are
  5370/5370 with 0 extra / 0 missing.
* `-ffunction-sections` is present: 193 CANTUNWIND sentinels share a real function
  address; removing the flag drops the gate to 1345/4357 and the sentinels to 51.
* `-fdata-sections` is absent: adding it drops the gate to 4311/4500 and changes
  `.rodata`/`.data`/`.data.rel.ro`.
* The full ndk-build r18b compile set minus `-fdata-sections` is byte-identical to the
  canonical build except `.note.gnu.build-id`; the full ndk-build link flag set is
  byte-identical.
* The ndk-build default link order is refuted: it pulls `libunwind.a`, defines
  `_Unwind_*` locally, adds `__gnu_Unwind_Find_exidx`, and gives 1345/4404 with 29
  extra exports.
* A real ndk-build default build of the tree gives 1345/3843; the canonical CMake build
  gives 4500/4500.

### Hypothesis / not recoverable from the binary

* The exact build-system file (`CMakeLists.txt` vs `Android.mk`/`Application.mk`) is not
  in the APK. The binary proves the **effective flags and link order**, not the file
  that produced them. CMake is the only r18b default build system that reproduces the
  binary without overrides; a hand-written ndk-build with `APP_STL := none`,
  `-fno-data-sections`, explicit `libc++abi`/`libandroid_support` after FMOD, and a
  mini `libgcc.a` cannot be excluded.
* `APP_PLATFORM := android-16` explicit vs omitted (r18b default is 16).
* `APP_OPTIM := release` explicit vs omitted (release is the default).
* `-fPIC` vs `-fpic` exact spelling (byte-identical on this source).
* `-std=c++11` vs `-std=gnu++11` exact spelling (byte-identical on this source).
* `-DNDEBUG`, `-DANDROID`, `-D__ANDROID_API__=16` as macros (codegen-neutral here; the
  API level itself is proven by the note).
* FMOD in `LOCAL_LDLIBS` vs `LOCAL_SHARED_LIBRARIES`: `LOCAL_SHARED_LIBRARIES` puts FMOD
  before the system libs and does not match the original `DT_NEEDED` order;
  `LOCAL_LDLIBS` after `-landroid` does. For CMake, `target_link_libraries` order is the
  matching one.
* The exact original `LOCAL_SRC_FILES` list; only the TU order is recovered
  (`tools/source_order.txt`).
* The vendored zlib/libzip/libcrypto archive build recipe: zlib/libzip are clang
  `-Oz -g -fstack-protector-strong -ffunction-sections -funwind-tables -mthumb`;
  libcrypto is GCC 4.9 `-O3 -fPIC -march=armv7-a`. The original's exact archive build
  script is not recovered.

## 6. What was not landed

No flag or source change is a strict improvement:

* The canonical build already has the original's data layout (no `-fdata-sections`).
* The full ndk-build compile set minus `-fdata-sections` is byte-identical except the
  build-id, so it is not an improvement.
* The full ndk-build link flag set and the full CMake default STL group are
  byte-identical.
* Adding `-fdata-sections` or removing `-ffunction-sections` is a regression.

The commit contains this note only. No `--update-baseline` was run.

## 7. Reproduction commands

```sh
# baseline
cd /home/debian/GalaxyOnFire2/wt/wb105
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8

# original ELF metadata
O=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
readelf -hW $O
readelf -AW $O
readelf -nW $O
readelf -p .comment $O
readelf -dW $O
readelf -SW $O
readelf --dyn-syms -W $O | awk '$7!="UND"{n++} END{print "defined",n}'
readelf --dyn-syms -W $O | awk '$7=="UND"{n++} END{print "undef",n}'

# CMake r18b default probe (see §2.1)
cmake -S /dev/shm/wb105/cmakeprobe -B /dev/shm/wb105/cmakeprobe/build -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=$PWD/.cache/ndk/android-ndk-r18b/build/cmake/android.toolchain.cmake \
  -DANDROID_ABI=armeabi-v7a -DANDROID_PLATFORM=android-16 -DANDROID_STL=c++_static \
  -DANDROID_ARM_NEON=ON '-DANDROID_CPP_FEATURES=rtti exceptions' \
  -DCMAKE_BUILD_TYPE=Release
ninja -C /dev/shm/wb105/cmakeprobe/build -v

# ndk-build r18b default probe (see §2.2)
.cache/ndk/android-ndk-r18b/ndk-build \
  NDK_PROJECT_PATH=/dev/shm/wb105/ndkprobe \
  APP_BUILD_SCRIPT=/dev/shm/wb105/ndkprobe/jni/Android.mk \
  NDK_APPLICATION_MK=/dev/shm/wb105/ndkprobe/jni/Application.mk V=1 -j8

# compile-flag A/B (full ndk set +/- -fdata-sections)
python3 /dev/shm/wb105/run_variants.py

# no -ffunction-sections
cp -a cmake-build-match /dev/shm/wb105/build-nofunc2
sed -i 's/ -ffunction-sections//g' /dev/shm/wb105/build-nofunc2/build.ninja
ninja -C /dev/shm/wb105/build-nofunc2 link_so -j8
python3 tools/verify/verify.py \
  --build-dir /dev/shm/wb105/verify-nofunc \
  --original $O --ours /dev/shm/wb105/build-nofunc2/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0

# link-flag / link-order A/B
python3 /dev/shm/wb105/relink.py fullflags fullflags
python3 /dev/shm/wb105/relink.py cmakedefault cmakedefault
python3 /dev/shm/wb105/relink.py ndkorder ndkorder
sha256sum cmake-build-match/libgof2hdaa.so /dev/shm/wb105/relink-*.so

# real ndk-build default build (see §3.3)
.cache/ndk/android-ndk-r18b/ndk-build \
  NDK_PROJECT_PATH=/dev/shm/wb105/ndkfull \
  APP_BUILD_SCRIPT=/dev/shm/wb105/ndkfull/jni/Android.mk \
  NDK_APPLICATION_MK=/dev/shm/wb105/ndkfull/jni/Application.mk V=1 -j8
python3 tools/verify/verify.py \
  --build-dir /dev/shm/wb105/verify-ndkfull \
  --original $O --ours /dev/shm/wb105/ndkfull/libs/armeabi-v7a/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0

# CANTUNWIND / duplicate-target count
python3 - <<'PY'
import struct
from collections import Counter
p = '/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so'
b = open(p, 'rb').read()
shoff = struct.unpack_from('<I', b, 0x20)[0]
ent = struct.unpack_from('<H', b, 0x2e)[0]
n = struct.unpack_from('<H', b, 0x30)[0]
si = struct.unpack_from('<H', b, 0x32)[0]
sh = [struct.unpack_from('<IIIIIIIIII', b, shoff + i*ent) for i in range(n)]
st = sh[si]
names = b[st[4]:st[4]+st[5]]
def prel31(x, place):
    v = x & 0x7fffffff
    if x & 0x80000000: v -= 0x80000000
    return place + v
for s in sh:
    o = s[0]
    e = names.find(b'\0', o)
    if names[o:e] == b'.ARM.exidx':
        off, size, addr = s[4], s[5], s[2]
        rows = [struct.unpack_from('<II', b, off + i*8) for i in range(size//8)]
        targets = [prel31(w0, addr + i*8) for i, (w0, w1) in enumerate(rows)]
        real = set(t for (w0, w1), t in zip(rows, targets) if w1 != 1)
        print('entries', len(rows), 'cant', sum(1 for w0, w1 in rows if w1 == 1),
              'dup_targets', sum(1 for k, v in Counter(targets).items() if v > 1),
              'cant_sharing_real',
              sum(1 for (w0, w1), t in zip(rows, targets) if w1 == 1 and t in real))
PY
```
