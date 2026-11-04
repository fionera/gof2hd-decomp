# Build-system archaeology: how `libgof2hdaa.so` was built (w5202, wave 104)

Status: research note, 2026-11-04. Worktree `/home/debian/GalaxyOnFire2/wt/w5202`,
branch `wave104-w5202`, base `8c5fd36465a710882c93de69b76c38f624c1d45b`.
Scratch: `/dev/shm/w5202`. **No `src/`, `tools/`, baseline or allowlist change is
landed**: no experiment produced a strict gate improvement. This note is the
deliverable.

Canonical gate before and after this pass (unchanged):

```text
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: original 5370  ours 5370  extra 0  missing 0 (wrong_type 0 absent 0)
imports: 207   size-exact: 4524/4524   lint: CLEAN   drift: 0
RATCHET PASS
```

## 0. Bottom line

* The original native library is an **NDK r18b / clang 7.0.2 / gold 1.12** build:
  API 16, `armeabi-v7a`, Thumb-2, NEONv1, softfp, static libc++, `-Oz -g`.
* Its effective compile/link recipe is the **NDK r18b CMake toolchain default set**
  (`$NDK/build/cmake/android.toolchain.cmake`), not the vanilla ndk-build r18b
  default set. The two decisive binary discriminators both point at CMake:
  **no `-fdata-sections`** (ndk-build adds it) and **FMOD before the libc++/libgcc
  archives** (ndk-build puts static archives before shared libraries).
* The APK was packaged by **Android Gradle plugin 3.1.2** on
  **2018-10-22 10:18:17 CEST** (`META-INF/MANIFEST.MF`,
  `assets/crashlytics-build.properties`). The app's `BuildConfig` is
  `BUILD_TYPE=release`, `FLAVOR=""`, `DEBUG=false`, version `2.0.16`
  (versionCode `47947032`), package `net.fishlabs.gof2hdallandroid2012`.
* The most likely driver is **Gradle `externalNativeBuild { cmake { ... } }`**
  (APK is Gradle-packaged, native build is CMake), but a **standalone CMake build
  copied into `jniLibs/armeabi-v7a/`** is equally consistent with the shipped
  binary. No original `CMakeLists.txt`, `Android.mk`, `Application.mk`,
  `build.gradle`, `.ninja`, `.o` or `.a` survives in the APK, IPA, Windows
  distribution, source-file dumps or on the public web.
* The build host was **macOS**: the original statically linked the **darwin**
  NDK r18b `libc++abi.a` (its `abort_message.o` `.rodata.str1.1` is the
  131-byte `/Volumes/Android/buildbot/...` path; the Linux archive's path is
  16 bytes shorter and drops `byte_exact` to 4344). The exact macOS version is
  not embedded.
* **No strict improvement exists.** The canonical CMake build is already the
  best match. `-fdata-sections` costs 189 `byte_exact`; moving the STL archives
  before FMOD costs 3155 `byte_exact`; adding the full CMake default STL group
  after FMOD is sha256-identical to the canonical link.

## 1. Baseline and artifact hashes

Commands:

```sh
cd /home/debian/GalaxyOnFire2/wt/w5202
export TMPDIR=/dev/shm/w5202/tmp
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
cmake --build cmake-build-match --target drift -j6
sha256sum cmake-build-match/libgof2hdaa.so \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
```

Raw result:

```text
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
=== DRIFT: 0 field(s) across 0 class(es) ===
```

| artifact | sha256 | size |
|---|---|---:|
| original `android_2.0.16_libgof2hdaa.so` | `d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483` | 2,187,328 |
| APK `lib/armeabi-v7a/libgof2hdaa.so` | `6d38fd767ee3b4f8174c371bce371bfc6397cf76e13c1756938b2a94658e3b58` | 2,187,328 |
| canonical CMake build | `59019897d178e9f59dbbb99715687e18ee7900d63897bc3ca012c9ad859aeba7` | 2,187,328 |
| APK file | `9cfe4484a475c9a31a336df9dea6408fbb129d88aa19083afe808ef6bca9eb19` | 6,389,308 |
| APK uncompressed payload (143 entries) | — | 13,206,304 |

The APK library and the ground-truth file differ in exactly **288 bytes**, all in
`.shstrtab` content plus the 27 section-header `sh_name` fields:

```sh
unzip -p "$APK" lib/armeabi-v7a/libgof2hdaa.so > /dev/shm/w5202/apk-lib.so
cmp -l /dev/shm/w5202/apk-lib.so \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so | wc -l
# 288
```

The APK copy is the raw bfd-strip artifact; the ground truth is the same file
after `tools/reorder_shstrtab.py` restores gold's Stringpool `.shstrtab` order.
This is not a second build.

### 1.1 Canonical section table vs original

Measured with a direct ELF section-by-section byte comparison (same Python
routine as `definitive_build_recipe_w3697.md` §9):

| section | original size | ours size | delta | differing bytes |
|---|---:|---:|---:|---:|
| `.text` | `0x1337c8` | `0x1337c8` | 0 | 100,113 |
| `.ARM.exidx` | `0x6f70` | `0x6f70` | 0 | 829 |
| `.ARM.extab` | `0x26298` | `0x26298` | 0 | 108,295 |
| `.rodata` | `0x41b67` | `0x41b67` | 0 | **0** |
| `.data.rel.ro` | `0x2fe8` | `0x2fe8` | 0 | **0** |
| `.data` | `0x296c` | `0x296c` | 0 | **0** |
| `.bss` | `0x69ac` | `0x69ac` | 0 | **0** |
| `.dynsym` | `0x15ca0` | `0x15ca0` | 0 | **0** |
| `.dynstr` | `0x2e318` | `0x2e318` | 0 | **0** |
| `.hash` | `0x973c` | `0x973c` | 0 | **0** |
| `.rel.dyn` | `0x7048` | `0x7048` | 0 | **0** |
| `.rel.plt` | `0x72d0` | `0x72d0` | 0 | **0** |
| `.plt` | `0xac4c` | `0xac4c` | 0 | **0** |
| `.got` | `0x430c` | `0x430c` | 0 | **0** |
| `.dynamic` | `0x148` | `0x148` | 0 | **0** |
| `.note.android.ident` | `0x98` | `0x98` | 0 | **0** |
| `.comment` | `0x12f` | `0x12f` | 0 | **0** |
| `.ARM.attributes` | `0x38` | `0x38` | 0 | **0** |
| `.shstrtab` | `0x117` | `0x117` | 0 | **0** |

The only differing section bytes are `.text` (100,113), `.ARM.exidx` (829),
`.ARM.extab` (108,295) and `.note.gnu.build-id` (20). These are the known
24-function source/EH residuals, not build-flag differences.

### 1.2 The 24 non-byte-exact functions (current `report.json`)

```text
  47.8   7072  _ZN6LayoutC1Ev
  47.8   7072  _ZN6LayoutC2Ev
  53.4  57232  _ZN11LevelScript7processEi
  60.4   1432  _ZN6Status16missionCompletedEbbx
  64.4   1996  _ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE
  66.0   2764  _ZN5Level10createShipEiiiP8Waypointbb
  73.9   9908  _ZN5Radar4drawEP6PlayerP3Hudi
  74.8   8080  _ZN5MGame10OnTouchEndEiiPv
  80.2   9700  _ZN12HangarWindow6renderEv
  80.5    612  _ZN7GlobalsC1Ev
  80.5    612  _ZN7GlobalsC2Ev
  86.9   1908  _ZN9RocketGun6updateEi
  87.6    968  _ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE
  88.3   2252  _ZN5Level11createSceneEv
  90.0   1984  _ZN3Gun22calcCharacterCollisionEv
  92.0  17172  _ZN5Level13createMissionEv
  92.1   9332  _ZN3Hud4drawExxP9PlayerEgobjj
  95.1   9600  _ZN15MenuTouchWindowC1Ei
  95.1   9600  _ZN15MenuTouchWindowC2Ei
  95.5   1948  _ZN11SpaceLounge10drawLoungeEv
  97.7   1896  _ZN15MenuTouchWindow12OnTouchBeginEiiPv
  98.4   1008  _ZN10ModStationC1Ev
  98.4   1008  _ZN10ModStationC2Ev
  99.9  11440  _ZN5MGame8OnUpdateEv
```

## 2. The APK / DEX / manifest evidence

### 2.1 APK inventory and Gradle packaging

```sh
APK="/home/debian/GalaxyOnFire2/Original APK/Galaxy on Fire 2™ HD_2.0.16_APKPure.apk"
unzip -l "$APK"
unzip -p "$APK" META-INF/MANIFEST.MF | head -5
unzip -p "$APK" assets/crashlytics-build.properties
```

Raw:

```text
Archive:  .../Galaxy on Fire 2™ HD_2.0.16_APKPure.apk
143 files, 13,206,304 bytes uncompressed
lib/armeabi-v7a/libcrashlytics.so   202,580
lib/armeabi-v7a/libfmodevent.so     370,660
lib/armeabi-v7a/libfmodex.so        920,204
lib/armeabi-v7a/libgof2hdaa.so    2,187,328
```

```text
Manifest-Version: 1.0
Built-By: Generated-by-ADT
Created-By: Android Gradle 3.1.2
```

```text
#Mon Oct 22 10:18:17 CEST 2018
version_name=2.0.16
package_name=net.fishlabs.gof2hdallandroid2012
build_id=f527605b-309f-404d-8afc-b68027edc49e
version_code=47947032
app_name=GOF2
```

There is **no** `Android.mk`, `Application.mk`, `CMakeLists.txt`, `*.cmake`,
`*.gradle`, `*.ninja`, `*.o`, `*.a`, `*.d`, or build log in the APK:

```sh
unzip -l "$APK" | grep -Ei 'Android\.mk|Application\.mk|CMakeLists|\.cmake|\.gradle|\.ninja|\.o$|\.a$|\.d$|\.log$'
# (no output)
```

There is only one ABI (`armeabi-v7a`), no `libc++_shared.so`, no `libstdc++`,
and no second/unstripped `libgof2hdaa.so`. The native libraries are deflated
(method 8) in this APK; the ZIP entry timestamps are all `1980-00-00 00:00`.

### 2.2 `AndroidManifest.xml` (binary AXML)

Parsed with the same minimal AXML parser used in
`build_system_evidence.md` §9 (string pool + `START_TAG` walk):

```text
manifest: versionCode=47947032  versionName=2.0.16
          package=net.fishlabs.gof2hdallandroid2012
uses-sdk: minSdkVersion=16  targetSdkVersion=27
uses-feature: glEsVersion=0x00020000 (required=true)
```

### 2.3 `classes.dex` (`BuildConfig`, JNI library name)

`classes.dex` is 3,723,352 bytes, DEX 035. Parsing `class_defs` for
`Lnet/fishlabs/gof2hdallandroid2012/BuildConfig;` gives:

```text
APPLICATION_ID Ljava/lang/String; = 'net.fishlabs.gof2hdallandroid2012'
BUILD_TYPE     Ljava/lang/String; = 'release'
DEBUG          Z                  = False
FLAVOR         Ljava/lang/String; = ''
VERSION_CODE   I                  = 47947032
VERSION_NAME   Ljava/lang/String; = '2.0.16'
```

The DEX string table also contains `gof2hdaa` and `loadLibrary` (one occurrence
each), and the app source-file names `BuildConfig.java`, `GOF2HD2012.java`,
`ToJNI.java`, `NativeFunctionCalls.java`. This is the Java side of the Gradle
app module; it does not contain the native build script.

## 3. NDK r18b: CMake defaults vs ndk-build defaults

### 3.1 CMake r18b toolchain (the matching default set)

`$NDK/build/cmake/android.toolchain.cmake` starts with
`cmake_minimum_required(VERSION 3.6.0)`. Its defaults for this target are:

| setting | CMake r18b default | original |
|---|---|---|
| `ANDROID_ABI` | `armeabi-v7a` | `armeabi-v7a` |
| `ANDROID_PLATFORM` | `android-16` (r18b minimum) | API 16 |
| `ANDROID_STL` | `c++_static` | static libc++ |
| `ANDROID_ARM_MODE` | `thumb` | Thumb-2 |
| `ANDROID_ARM_NEON` | `FALSE` for API < 23 | **explicit `ON`** |
| `ANDROID_CPP_FEATURES` | empty | **explicit `rtti exceptions`** |
| `CMAKE_BUILD_TYPE` | empty | **explicit `Release`** |
| `-ffunction-sections` | yes | yes |
| `-fdata-sections` | **no** | **no** |
| `-fstack-protector-strong` | yes | yes |
| `-funwind-tables` | yes | yes |
| `-g` | yes | yes |
| `-DANDROID` | yes | unobservable |
| `-Oz -DNDEBUG` (Release) | yes | yes |
| `-std=c++11` (c++_static) | yes | C++11 codegen |
| link order | user libs before STL archives | FMOD before `libc++abi.a` |
| `-Wl,--exclude-libs,libgcc.a` | yes | yes |
| `-Wl,--exclude-libs,libatomic.a` | yes | unobservable |
| `-Wl,--exclude-libs,libunwind.a` | yes | unobservable |
| `-Wl,--no-undefined` | yes | unobservable |
| `-Wl,--build-id -z noexecstack -z relro -z now` | yes | yes |

A minimal CMake probe (`/dev/shm/w5202/cmakeprobe`) configured with
`-DANDROID_ABI=armeabi-v7a -DANDROID_PLATFORM=android-16 -DANDROID_STL=c++_static
-DANDROID_ARM_NEON=ON '-DANDROID_CPP_FEATURES=rtti exceptions'
-DCMAKE_BUILD_TYPE=Release` emits:

```text
clang++ --target=armv7-none-linux-androideabi16 ... -g -DANDROID
  -ffunction-sections -funwind-tables -fstack-protector-strong
  -no-canonical-prefixes -march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16
  -mthumb -mfpu=neon -Wa,--noexecstack -Wformat -Werror=format-security
  -std=c++11 -frtti -fexceptions -Oz -DNDEBUG -fPIC ...
```

There is **no `-fdata-sections`**. The link tail is:

```text
... -llog -latomic -lm libc++_static.a libc++abi.a libandroid_support.a
    libunwind.a -ldl
```

i.e. the user library (`-llog`; in the game, FMOD) is **before** the STL
archives.

### 3.2 ndk-build r18b defaults (the refuted default set)

`$NDK/build/core/default-build-commands.mk` has:

```make
GLOBAL_CFLAGS = \
    -target $(LLVM_TRIPLE)$(TARGET_PLATFORM_LEVEL) \
    -fdata-sections \
    -ffunction-sections \
    -fstack-protector-strong \
    -funwind-tables \
    -no-canonical-prefixes
GLOBAL_CXXFLAGS = $(GLOBAL_CFLAGS) -fno-exceptions -fno-rtti
```

A minimal `ndk-build` probe (`/dev/shm/w5202/ndkprobe`) with
`APP_ABI := armeabi-v7a`, `APP_STL := c++_static`, `APP_PLATFORM := android-16`,
`APP_OPTIM := release`, `LOCAL_CPP_FEATURES := rtti exceptions`,
`LOCAL_ARM_NEON := true`, `LOCAL_LDLIBS := -llog`, `V=1 -B` emits:

```text
clang++ -target armv7-none-linux-androideabi16 \
  -fdata-sections -ffunction-sections -fstack-protector-strong -funwind-tables \
  -no-canonical-prefixes -g ... -fno-exceptions -fno-rtti ... \
  -fpic -march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16 \
  -mthumb -Oz -DNDEBUG -mfpu=neon -std=c++11 -DANDROID ... \
  -frtti -fexceptions ...
```

and the link order is:

```text
... <objects> libc++_static.a libc++abi.a libandroid_support.a libunwind.a
    -lgcc -Wl,--exclude-libs,libgcc.a -latomic ... -llog -ldl -lc -lm
```

i.e. **static archives before shared libraries**, and `-fdata-sections` is
always present. Both differ from the original. A vanilla ndk-build r18b build
of this tree gives **byte 1345 / linked 3843** (see
`_work/reconstructions/build-system-ndk-vs-cmake-wb105.md` §3.3).

## 4. Controlled A/B: the two decisive discriminators

### 4.1 `-fdata-sections` is absent (CMake default)

Scratch build with `-fdata-sections` appended to both C and C++ flags
(`/dev/shm/w5202/fdata_toolchain.cmake`):

```sh
cmake -S . -B /dev/shm/w5202/build-fdata -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/dev/shm/w5202/fdata_toolchain.cmake \
  -DGOF2_MATCH_OPT=-Oz \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build /dev/shm/w5202/build-fdata --target link_so -j6
python3 tools/verify/verify.py \
  --build-dir /dev/shm/w5202/verify-fdata \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /dev/shm/w5202/build-fdata/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
```

Raw:

```text
canonical: byte 4500  linked 4500  size 4524/4524
-fdata:    byte 4311  linked 4500  size 4524/4524
```

`-fdata-sections` loses **189 byte-exact functions** and gains none (the
`report.json` diff is 189 lost, 0 gained; examples: `SHA224`, `SHA256`,
`MouseInput`, `SlowMotion`, `keyPressed`, `ModStation::*`, `StarSystem::*`,
`BlurShader::Init`, `BumpShader::Init`, `GlowShader::Init`, ...). Section
comparison:

| section | canonical | `-fdata-sections` | original |
|---|---:|---:|---:|
| `.rodata` | `0x41b67` (0 diff) | `0x41a07` (-0x160, 113,935 diff) | `0x41b67` |
| `.data` | `0x296c` (0 diff) | `0x295c` (-0x10, 3,023 diff) | `0x296c` |
| `.data.rel.ro` | `0x2fe8` (0 diff) | `0x2fe8` (365 diff) | `0x2fe8` |
| `.bss` | `0x69ac` (0 diff) | `0x69ac` (9 diff) | `0x69ac` |
| `.dynsym` | `0x15ca0` (0 diff) | `0x15ca0` (753 diff) | `0x15ca0` |
| `.rel.dyn` | `0x7048` (0 diff) | `0x7048` (180 diff) | `0x7048` |

The original's `.rodata`, `.data` and `.data.rel.ro` are **byte-identical** to
the no-`-fdata-sections` build. This is positive proof that the original did
not use `-fdata-sections`, i.e. it did not use the vanilla ndk-build r18b
default.

### 4.2 Link order: FMOD before the libc++/libgcc archives

The canonical gold link (from `ninja -C cmake-build-match -t commands
libgof2hdaa.so`) is:

```text
crtbegin_so.o
<app objects in tools/source_order.txt order>
--whole-archive libz_gof2.a libzip_gof2.a --no-whole-archive
-l log -l GLESv2 -l GLESv1_CM -l EGL -l android
libfmodex.so libfmodevent.so
libcrypto_gof2.a
-l m
libc++abi.a
libandroid_support.a
libgcc.a
-l dl -l c
crtend_so.o
```

The original's `.text` tail proves the last archives' order directly:

```sh
OBJ=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OBJ -d --start-address=0x19ae00 --stop-address=0x19aea0 \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
$OBJ -d --start-address=0x19ae00 --stop-address=0x19aea0 \
  cmake-build-match/libgof2hdaa.so
```

Both disassemble identically at `0x19ae54` (`__aeabi_ldivmod` from
`libgcc.a(_aeabi_ldivmod.o)`), with `memalign` immediately before it
(`libandroid_support.a(posix_memalign.o)`) and `__gnu_ldivmod_helper` called
through the PLT. `readelf --dyn-syms` on the original shows:

```text
memalign@LIBC (UND)
__gnu_ldivmod_helper (UND)
_Unwind_Resume, _Unwind_VRS_Get, _Unwind_RaiseException, _Unwind_VRS_Set,
_Unwind_GetLanguageSpecificData, _Unwind_DeleteException,
_Unwind_GetRegionStart (all UND)
(no __gnu_Unwind_Find_exidx)
```

This is the CMake order: FMOD first, then `libc++abi.a`, `libandroid_support.a`,
`libgcc.a`; no `libunwind.a` member is pulled.

Controlled relinks of the canonical objects (same objects, only the archive
order changed; `/dev/shm/w5202/relink.py`):

| variant | byte | linked | `.text` | `.rodata` | `.dynsym` | `.rel.plt` | `__gnu_Unwind_Find_exidx` |
|---|---:|---:|---:|---:|---:|---:|---|
| canonical | **4500** | **4500** | `0x1337c8` | `0x41b67` | `0x15ca0` | `0x72d0` | absent |
| A: `libc++abi.a libandroid_support.a libgcc.a` before FMOD | 1345 | 3796 | `0x135510` | `0x41b67` | `0x15b90` | `0x7248` | UND |
| B: A + `libc++_static.a` + `libunwind.a` before FMOD (`--exclude-libs=libunwind.a`) | 1345 | 3796 | `0x135d20` | `0x41ff7` | `0x15b80` | `0x7250` | UND |

Variant A is already the ndk-build static-before-shared order; variant B is the
vanilla ndk-build `c++_static` order. Both cost **3155 byte-exact** functions.
This is the second positive proof that the original is not vanilla ndk-build.

### 4.3 The full CMake default STL group is byte-neutral

Variant C adds `libc++_static.a` and `libunwind.a` in the **CMake default
positions after FMOD** (and `--exclude-libs=libatomic.a`,
`--exclude-libs=libunwind.a`):

```sh
python3 /dev/shm/w5202/relink_c.py
sha256sum /dev/shm/w5202/relink-C.so cmake-build-match/libgof2hdaa.so
# 59019897d178e9f59dbbb99715687e18ee7900d63897bc3ca012c9ad859aeba7  both
```

So the canonical minimal tail (`libc++abi.a libandroid_support.a libgcc.a -ldl
-lc`) is output-equivalent to the full CMake default STL group. No strict
improvement is available from a more faithful link line.

## 5. Host OS: macOS / darwin NDK package

The original's only path-like string is:

```text
/Volumes/Android/buildbot/src/android/ndk-release-r18/external/libcxx/../../external/libcxxabi/src/abort_message.cpp
```

This is the **darwin** NDK r18b `libc++abi.a(abort_message.o)`
`.rodata.str1.1` (131 bytes). The Linux package's path is 16 bytes shorter; a
relink with the Linux archive drops `byte_exact` to **4344** (vs 4500) and
changes `.rodata` by -16 bytes (see
`_work/reconstructions/build-toolchain-runtime-provenance-w5019.md` §2.5).
The canonical build uses the darwin archive
`sha256 9a0f50d22ef53a946b76d689f723b2ca88d2cb0f7e17ee133ab19395a925fadb`
(the Linux archive is `6993e991ff2d9e4f45febab2f7758fe293c14d08f2364796c6d63f2b5cc210a6`).

**Conclusion:** the original was built with the **darwin** NDK r18b package,
i.e. on macOS. The exact macOS version is not embedded. The iOS 1.1.4
`BuildMachineOSBuild=11E53` (OS X 10.7.4) is from the 2012 iOS build, not the
2018 Android build.

## 6. Cross-artifact search: APK, IPA, Windows, source dumps, web

### 6.1 iOS

```sh
IPA="/home/debian/GalaxyOnFire2/Original IPA/Galaxy_on_Fire_2__HD_1.1.4_ios_4.3.ipa"
unzip -p "$IPA" 'Payload/*.app/Info.plist' > /dev/shm/w5202/info114.plist
python3 - <<'PY'
import plistlib
d=plistlib.load(open('/dev/shm/w5202/info114.plist','rb'))
for k in ['CFBundleIdentifier','CFBundleShortVersionString','MinimumOSVersion',
          'DTXcode','DTXcodeBuild','DTSDKName','DTCompiler','BuildMachineOSBuild',
          'DTPlatformVersion','DTSDKBuild']:
    print(k,'=',d.get(k))
PY
```

| key | iOS 1.1.4 | iOS 1.0.5 |
|---|---|---|
| bundle id | `www.fishlabs.net.gof2hd` | `www.fishlabs.net.gof2` |
| version | 1.1.4 | 1.0.5 |
| minimum OS | 4.3 | 3.0 |
| `DTXcode` / `DTXcodeBuild` | `0450` / `4G182` (Xcode 4.5) | `0400` / `4A304a` (Xcode 4.0) |
| `DTSDKName` | `iphoneos6.0` | `iphoneos4.3` |
| `DTCompiler` | `com.apple.compilers.llvm.clang.1_0` | `4.2` (GCC 4.2) |
| `BuildMachineOSBuild` | `11E53` (OS X 10.7.4) | `10J869` (OS X 10.6.8) |
| `DTPlatformVersion` / `DTSDKBuild` | `6.0` / `10A403` | `4.3` / `8F190` |

The 1.1.4 Mach-O is armv7 `MH_EXECUTE`, `LC_VERSION_MIN_IPHONEOS 4.3`, SDK 6.0.
The IPA contains no `.pbxproj`, `.xcodeproj`, `.o`, `.a`, `.d` or build log.
The only Fishlabs source paths in the binary are the four
`/Users/hck/Documents/Dev/GalaxyOnFire2_merged/Classes/Game/ParticleSystems/*.cpp`
assert strings. These are iOS-era source roots, not Android build files.

### 6.2 Windows

```sh
llvm-readobj --file-headers --coff-debug-directory \
  --coff-imports /home/debian/GalaxyOnFire2/_work/bins/win_GOF2.exe
```

| observable | value |
|---|---|
| format | PE32 i386 (`IMAGE_FILE_MACHINE_I386`) |
| linker | `10.0` (Visual Studio 2010) |
| timestamp | `2012-09-05 16:30:26` (`0x50477E22`) |
| PDB | `C:\Users\s.lohse\Desktop\GoF2_PC_REP\GOF2.pdb`, GUID `{9A525EDE-5753-47D1-B793-9D334ED401D4}`, age 2 |
| imports | `MSVCR100.dll`, `MSVCP100.dll`, `d3d9.dll`, `d3dx9_43.dll`, `fmod_event.dll`, `steam_api.dll`, `DINPUT8.dll`, `GDI32.dll`, `KERNEL32.dll`, `SHELL32.dll`, `USER32.dll`, `COMDLG32.dll` |

The Windows distribution contains only the exe, DLLs, `DefaultInputMapping.ini`
and DirectX redistributables; no `.pdb`, `.lib`, `.exp`, `.map`, `.sln` or
`.vcxproj`. This is a separate VS2010 build, not evidence for the Android
native flags.

### 6.3 Source-file dumps

`/home/debian/GalaxyOnFire2/source file list dump/` contains
`source file lists dump.txt` and three per-TU symbol dumps:

| dump | build environment |
|---|---|
| `gof2hd_1.1.19_ios.cpp` | Xcode DerivedData, arm64, `/Users/n.tran/projects/gof2/` |
| `gof2sd_1.0.1_ios.cpp` | armv6, iPhoneOS 4.0/4.1 SDK, `/Users/mhehmeyer/Documents/Dev/GalaxyOnFire2/` |
| `gof3d_1.1.4_ios.cpp` | armv6, GoF1, `/Users/marchehmeyer/Documents/Dev/GalaxyOnFire/` |

They are iOS/mac/windows/MeeGo layouts (flat `AE2/Classes/` engine + flat
`Classes/Game/` game tree). There is **no Android path, `Android.mk`,
`Application.mk`, `CMakeLists.txt` or Gradle file** in the dumps. They are the
authority on TU membership, not on the Android build system.

### 6.4 Web

Searches (DuckDuckGo HTML endpoint, 2026-11-04):

```text
libgof2hdaa
gof2hdallandroid2012
"libgof2hdaa.so"
"Galaxy on Fire 2" Android ndk-build
"Galaxy on Fire 2 HD" CMake
"GalaxyOnFire2_merged"
"1605_Abyss_Engine"
"AE2/Classes"
site:github.com gof2 android jni
```

Results: only APK mirrors (APKPure, APKMirror, Google Play), fan ports and
decompilation repos:

* `fionera/gof2hd-decomp` — this decompilation repo.
* `JoppieToppie/Galaxy-on-Fire-2-Unity-Remake` — includes the same
  `Reference/binaries/libgof2hdaa.so` and decompiled Java (`BuildConfig.java`,
  `ToJNI.java`, `GOF2HD2012.java`), but no original build files.
* `KorolevSoftware/GOF2HD-linux-arm-port` — a Linux ARM port that loads the
  original Android `libgof2hdaa.so`; its CMake is a fan-port host shim, not the
  original build.
* `KiritoJPK/Galaxy-on-Fire-2-FULL-HD-Android` and
  `KiritoJPK/GOF2-FHD-Android-64-bits` — texture/APK mods, no build files.
* `v-atamanenko/gof2-vita` — Vita loader, not the Android build.

No public Fishlabs Android source tree, `CMakeLists.txt`, `Android.mk`,
`Application.mk` or `build.gradle` was found.

## 7. What is proven, disproven, and unobservable

### 7.1 Positively proven

* Toolchain: NDK r18b (`5063045`), clang 7.0.2 (`4751641 based on r328903`),
  gold 1.12, API 16, `armeabi-v7a`, Thumb-2, VFPv3, NEONv1, softfp, static
  libc++ (`readelf -h/-A/-n/-p .comment`, `DT_NEEDED`).
* Compile flags: `-Oz -g`, `-std=c++11`, `-fPIC`, `-frtti`, `-fexceptions`,
  `-fstack-protector-strong`, `-ffunction-sections`, `-funwind-tables`,
  frame pointers on, default visibility, **no `-fdata-sections`**.
* Link flags: `-shared -soname libgof2hdaa.so --build-id -z now
  --exclude-libs=libgcc.a`, no `-s` (gold), followed by GNU
  `strip --strip-all` and the `.shstrtab` reorder.
* Link input order: **FMOD → libcrypto → libm → libc++abi → libandroid_support
  → libgcc → dl → c**, with `libgcc.a` genuinely pulled (`__aeabi_ldivmod`
  at `0x19ae54`) and `libandroid_support.a` before it (`memalign` at
  `0x19ae14`).
* `-fdata-sections` is absent: original `.rodata`/`.data`/`.data.rel.ro` are
  byte-identical to the no-`-fdata` build; `-fdata` gives 4311 vs 4500 and
  changes those sections.
* Vanilla ndk-build r18b is disproven: its `-fdata-sections` and
  static-before-shared link order both differ; a vanilla build gives
  1345/3843.
* APK packaging: Android Gradle plugin 3.1.2, built 2018-10-22 10:18:17 CEST,
  `BuildConfig` release/no flavor, `minSdkVersion=16`, `targetSdkVersion=27`.
* Host: macOS/darwin NDK package (the 131-byte `/Volumes/Android` libc++abi
  path; the Linux archive gives 4344 vs 4500).

### 7.2 Positively disproven

* Vanilla ndk-build r18b defaults (`-fdata-sections`, static-before-shared
  link order).
* `APP_STL := c++_shared` / `system` (libstdc++): no `libc++_shared.so`,
  no libstdc++ symbols; static libc++ is linked.
* `-fno-rtti`, `-fno-exceptions`: RTTI typeinfo and EH are present;
  `-fno-exceptions` does not build.
* `-fno-threadsafe-statics`, `-fno-use-cxa-atexit`: `__cxa_guard_*` and
  `__cxa_atexit` are present; A/B drops to 1344/1348.
* `-fno-slp-vectorize`, `-fno-optimize-sibling-calls`: A/B drops to
  1411/1316.
* `-Os`, `-O1`, `-O2`, `-O3`: 1263/1033/1168/1167 vs 4500.
* `-marm`, `-fvisibility=hidden`, `-fdata-sections`, `-z norelro`,
  `-z lazy`, gold `-s`, `--section-ordering-file`, `--version-script`,
  `--unresolved-symbols`, `--allow-multiple-definition`, `--gc-sections`.
* `-fPIC` vs `-fpic` is **not** a discriminator (byte-identical); `-fPIC`
  is CMake's spelling and the conservative choice.
* `-std=c++11` vs `-std=gnu++11` is **not** a discriminator on this source
  (byte-identical); C++11 is the CMake/c++_static default.

### 7.3 Unobservable / unknown

* The exact build-system file (`CMakeLists.txt` vs `Android.mk`/
  `Application.mk` vs a custom script). The binary proves effective flags and
  link order, not the file that produced them.
* The exact driver: Gradle `externalNativeBuild { cmake { ... } }` vs a
  standalone CMake build copied into `jniLibs/armeabi-v7a/`. Both are
  consistent with the APK.
* The exact CMake version. The r18b toolchain itself requires
  `cmake_minimum_required(VERSION 3.6.0)`; AGP 3.1.2's SDK-bundled CMake
  `3.6.4111459` is the most likely, but no version string is embedded.
* The exact CMake generator (Ninja vs Unix Makefiles) and host macOS version.
* `-DNDEBUG`, `-DANDROID`, `-D__ANDROID_API__=16` as macros (codegen-neutral).
* `-g` vs `-g2`/`-g3` (byte-identical); `-g0`/`-g1` are disproven.
* `-fno-sized-deallocation`, `-fno-common`, `-fmerge-constants`, `-fno-plt`,
  `-fno-vectorize`, `-mtune=*`, `-mfpu=neon-vfpv4`, `-fshort-wchar`,
  `-fno-delete-null-pointer-checks`, `-mno-movt` (neutral/unsupported).
* Whether `libc++_static.a`, `libunwind.a` and `libatomic.a` were on the link
  line: their members are not pulled after FMOD; adding them in the CMake
  default positions is sha256-identical.
* The exact original `CMakeLists.txt`/`build.gradle`/`LOCAL_SRC_FILES` text;
  `tools/source_order.txt` is a reconstruction from the binary, not a
  recovered file.
* The exact original project directory names and source paths. The only
  surviving app source-file witness in the `.so` is `main.c` in three log
  strings; the only path-like string is the libc++abi buildbot path.

## 8. Confidence table

| claim | confidence | hard evidence |
|---|---:|---|
| NDK r18b, clang 7.0.2, gold 1.12, API 16 | **certain** | `.comment`, `.note.android.ident`, `.note.gnu.gold-version` |
| armeabi-v7a, Thumb-2, NEON, softfp, static libc++ | **certain** | ELF header, `.ARM.attributes`, `DT_NEEDED`, no `libc++_shared.so` |
| `-Oz -g`, `-ffunction-sections`, no `-fdata-sections` | **certain** | A/B; `.rodata`/`.data`/`.data.rel.ro` 0 diff; `-fdata` 4311 vs 4500 |
| Link order FMOD before libc++/libgcc archives | **certain** | `.text` tail, `_Unwind_*` UND, A/B 1345 vs 4500 |
| CMake r18b toolchain (not vanilla ndk-build) | **high (~90-95%)** | CMake defaults match both discriminators; ndk-build defaults differ |
| Gradle 3.1.2 APK packaging | **certain** | `META-INF/MANIFEST.MF`, `BuildConfig`, manifest |
| Build host macOS/darwin | **high (~90%)** | darwin `libc++abi.a` path/A-B; iOS/mac source roots |
| Gradle `externalNativeBuild` driver | **medium (~50-60%)** | APK Gradle-packaged; native build CMake; no build files in APK |
| Standalone CMake + `jniLibs` copy | **medium (~35-45%)** | same binary evidence; no build files |
| Custom script mimicking CMake defaults | **low (~5-10%)** | would have to reproduce both CMake discriminators |
| Exact CMake version | **unknown; >= 3.6.0** | toolchain `cmake_minimum_required`; no version string |
| Exact original project files | **unknown** | not in APK/IPA/Windows/source dumps/web |

The two CMake-vs-ndk-build discriminators are independent and both are
load-bearing: `-fdata-sections` changes the data layout (189 functions), and
the link order changes `.text`/`.rel.plt`/`.dynsym` (3155 functions). A
non-default ndk-build (`APP_STL := none`, `-fno-data-sections`, explicit
`libc++abi`/`libandroid_support` after FMOD, and a mini `libgcc.a`) can mimic
the binary, but it requires at least four non-default overrides; the CMake
route needs only `ANDROID_ARM_NEON=ON`, `ANDROID_CPP_FEATURES="rtti
exceptions"` and `CMAKE_BUILD_TYPE=Release`. CMake is therefore the
parsimonious reconstruction.

## 9. Strict improvement: none

No build-flag change improves the gate:

| experiment | byte_exact | linked_exact | verdict |
|---|---:|---:|---|
| canonical CMake | **4500** | **4500** | reference |
| `-fdata-sections` | 4311 | 4500 | **-189** |
| link order A (STL before FMOD) | 1345 | 3796 | **-3155** |
| link order B (vanilla ndk-build `c++_static`) | 1345 | 3796 | **-3155** |
| link order C (full CMake STL group after FMOD) | 4500 | 4500 | sha256-identical |
| full ndk-build r18b build (prior note) | 1345 | 3843 | **-3155** |

The canonical `CMakeLists.txt`/`cmake/toolchain.cmake` is already the best
match. The CMake-faithful additions (`-fpic`, `-DNDEBUG`, `-DANDROID`,
`-fexceptions`, `-no-canonical-prefixes`, `--fix-cortex-a8`,
`--no-undefined`, `-z noexecstack`, `-z relro`, `--warn-shared-textrel`,
`--fatal-warnings`, `--exclude-libs=libatomic.a`,
`--exclude-libs=libunwind.a`, `libc++_static.a`, `libunwind.a`, `-latomic`)
are byte-neutral on this tree (prior notes + variant C). No build-file change
is landed; this note is the only commit.

## 10. Reproduction commands

```sh
# baseline
cd /home/debian/GalaxyOnFire2/wt/w5202
export TMPDIR=/dev/shm/w5202/tmp
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
cmake --build cmake-build-match --target drift -j6

# original metadata
O=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
readelf -hW "$O"; readelf -AW "$O"; readelf -nW "$O"
readelf -p .comment "$O"; readelf -dW "$O"; readelf -SW "$O"
readelf --dyn-syms -W "$O" | grep -E '_Unwind_|__gnu_Unwind_Find_exidx|memalign|ldivmod'

# APK / DEX / manifest
APK="/home/debian/GalaxyOnFire2/Original APK/Galaxy on Fire 2™ HD_2.0.16_APKPure.apk"
unzip -l "$APK"
unzip -p "$APK" META-INF/MANIFEST.MF | head -5
unzip -p "$APK" assets/crashlytics-build.properties
unzip -p "$APK" lib/armeabi-v7a/libgof2hdaa.so > /dev/shm/w5202/apk-lib.so
sha256sum /dev/shm/w5202/apk-lib.so "$O"
cmp -l /dev/shm/w5202/apk-lib.so "$O" | wc -l
# AXML / BuildConfig parsers: see §2.2 and §2.3; both are small Python
# string-pool/class-def parsers, no Android SDK needed.

# -fdata-sections A/B
cat > /dev/shm/w5202/fdata_toolchain.cmake <<'X'
include("/home/debian/GalaxyOnFire2/wt/w5202/cmake/toolchain.cmake")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdata-sections" CACHE STRING "" FORCE)
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -fdata-sections" CACHE STRING "" FORCE)
X
cmake -S . -B /dev/shm/w5202/build-fdata -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/dev/shm/w5202/fdata_toolchain.cmake \
  -DGOF2_MATCH_OPT=-Oz \
  -DGOF2_ORIGINAL_SO="$O" \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build /dev/shm/w5202/build-fdata --target link_so -j6
python3 tools/verify/verify.py \
  --build-dir /dev/shm/w5202/verify-fdata \
  --original "$O" --ours /dev/shm/w5202/build-fdata/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0

# link-order A/B/C: /dev/shm/w5202/relink.py and relink_c.py
# (extract the gold line from `ninja -C cmake-build-match -t commands
#  libgof2hdaa.so`, move the archives, re-run strip + reorder_shstrtab)
python3 /dev/shm/w5202/relink.py A
python3 /dev/shm/w5202/relink.py B
python3 /dev/shm/w5202/relink_c.py
sha256sum /dev/shm/w5202/relink-C.so cmake-build-match/libgof2hdaa.so

# CMake / ndk-build probes
NDK=.cache/ndk/android-ndk-r18b
cmake -S /dev/shm/w5202/cmakeprobe -B /dev/shm/w5202/cmakeprobe/build -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=$NDK/build/cmake/android.toolchain.cmake \
  -DANDROID_ABI=armeabi-v7a -DANDROID_PLATFORM=android-16 \
  -DANDROID_STL=c++_static -DANDROID_ARM_NEON=ON \
  '-DANDROID_CPP_FEATURES=rtti exceptions' -DCMAKE_BUILD_TYPE=Release
ninja -C /dev/shm/w5202/cmakeprobe/build -v
$NDK/ndk-build NDK_PROJECT_PATH=/dev/shm/w5202/ndkprobe \
  APP_BUILD_SCRIPT=/dev/shm/w5202/ndkprobe/jni/Android.mk \
  NDK_APPLICATION_MK=/dev/shm/w5202/ndkprobe/jni/Application.mk V=1 -B -j6

# cross-artifact
llvm-objdump --macho --private-headers \
  /home/debian/GalaxyOnFire2/_work/bins/ios_1.1.4_HD
llvm-readobj --file-headers --coff-debug-directory --coff-imports \
  /home/debian/GalaxyOnFire2/_work/bins/win_GOF2.exe
grep -inE 'android|ndk|jni|cmake|gradle' \
  "/home/debian/GalaxyOnFire2/source file list dump"/*.txt \
  "/home/debian/GalaxyOnFire2/source file list dump"/*.cpp
```

## 11. Files touched

* `_work/reconstructions/build_system_archaeology_w5202.md` (this note).
* Scratch only: `/dev/shm/w5202/` (`classes.dex`, `AndroidManifest.xml`,
  `resources.arsc`, `apk-lib.so`, `fdata_toolchain.cmake`, `build-fdata/`,
  `relink.py`, `relink_c.py`, `relink-A/B/C.so`, `cmakeprobe/`, `ndkprobe/`,
  `verify-fdata/`).
