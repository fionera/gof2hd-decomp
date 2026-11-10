# Build provenance round 7: reconstructing the original CMakeLists.txt / Android.mk

Status: **research note only**, 2026-11-10. Worktree `wt/w11219`, branch
`wave167-w11219`, base master `d00057e6d`. Scratch `/dev/shm/w11219`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(2,187,328 B, sha256 `d4c17be6…bed9483`, build-id `c011810d…0d310`).
APK `/home/debian/GalaxyOnFire2/Original APK/Galaxy on Fire 2™ HD_2.0.16_APKPure.apk`.

Gate on entry and exit (fresh `cmake --build cmake-build-match --target check -j2`,
plus `lint` and `drift`): **RATCHET PASS**, `byte_exact 4507/4524`,
`linked_exact 4507/4524`, size-exact 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, parity 5370/5370. `.text` **82,898** differing bytes,
`.ARM.extab` **660**, `.rel.plt` **0** (byte-identical), `.note.gnu.build-id`
20. All 27 section sizes exact; file size 2,187,328 B.

This is an independent round-7 re-derivation, deliberately using a different
evidence set from round 6: the **source-layout dumps**, the **APK/DEX
metadata**, the **ELF note/comment/version sections**, and fresh **object-level
flag A/Bs**. It confirms round 6's build-system conclusion and adds four new
measurements:

1. a **full-tree `-fvisibility=hidden` A/B** (57/244 byte-exact, 4,280 missing
   exports, `.plt` 44,108 -> 4,016 B) which refutes hidden visibility;
2. a **version-script A/B** plus the original's `.gnu.version_d` (one BASE
   entry, all 5,370 defined symbols at version index 1, no `@` names), which
   refutes a named version script;
3. a **new `-std=` size witness** (`RecordHandler::readRecordAsByteArray`
   180 B in the original and `-std=c++11`, 176 B at `-std=c++14`;
   `NewsItem::clone` 74 B vs 72 B), independent of round 6's signed-`new[]`
   argument;
4. a **per-TU `-D`/`-include`/`-std`/include-path audit** showing the
   candidate build macros and forced includes are codegen-neutral on this
   tree, while `.rodata`, `.data`, `.data.rel.ro`, `.bss`, `.dynsym`,
   `.dynstr`, `.hash`, `.got`, `.plt`, `.rel.dyn`, `.rel.plt`, `.shstrtab`,
   `.ARM.exidx`, `.comment`, `.note.android.ident`, `.note.gnu.gold-version`
   and `.ARM.attributes` are **byte-identical** to the canonical build.

No `src/`, `CMakeLists.txt`, baseline, allowlist, or doc change is landed.
This note is the only commit.

---

## 0. Bottom line

1. **The effective build is the NDK r18b CMake toolchain, not default
   ndk-build.** The decisive observable is still **no `-fdata-sections`**:
   the original's `.rodata` (269,159 B), `.data` (10,604 B),
   `.data.rel.ro` (12,264 B) and `.bss` (27,052 B) are byte-identical to the
   canonical no-`-fdata-sections` build. The r18b CMake toolchain does not
   add `-fdata-sections`; stock r18b ndk-build does. The link-line tail
   `libm, libdl, libc` and the user-library-before-STL order are also the
   CMake defaults. Confidence stays ~85% CMake, ~10% heavily coerced
   ndk-build, ~5% hand-written makefile.
2. **Every TU used the same codegen flags.** The 4,507 byte-exact exported
   functions and every local function are identical; the 82,898 `.text`
   differing bytes are exactly the 17 residual exported functions (15 unique
   bodies; C1/C2 aliases share an address) in 13 TUs. `.rodata`/`.data`/
   `.data.rel.ro`/`.bss` are byte-identical, so no TU had a `-D`, `-include`
   or include-path difference that changed data or code. The only witnessed
   macro is `NDEBUG` (app asserts absent).
3. **`-fvisibility=hidden` is refuted.** A full-tree hidden build exports
   only 244 functions (57 byte-exact, 4,280 missing) and shrinks `.plt` from
   44,108 to 4,016 B. The original exports 5,370 defined dynsym symbols (including the 3
   standard ABS end markers), all `GLOBAL DEFAULT`/`WEAK DEFAULT`, and its `.plt` is byte-identical to the
   default-visibility build.
4. **No named version script.** `.gnu.version_d` has exactly one BASE entry
   (`libgof2hdaa.so`); all 5,370 defined dynsym symbols are version index 1
   and no defined name contains `@`. A named version script (`V1 { … };`)
   adds a second verdef and `foo@@V1` names. An anonymous no-op
   `{ global: *; };` script is indistinguishable but pointless.
5. **The C++ standard is C++11.** New independent size witnesses:
   `RecordHandler::readRecordAsByteArray(void**, int, bool)` is **180 B** in
   the original and under `-std=c++11`, **176 B** under `-std=c++14`;
   `NewsItem::clone()` is **74 B** vs **72 B**. `-std=gnu++11` is
   byte-identical to `-std=c++11`. The C TUs are codegen-neutral across
   `c99/gnu99/c11/gnu11` (and fail `c89` only for Blowfish's `//`/`long long`).
6. **The source list order is the original Android link order**, not the
   Xcode/iOS dump order. `tools/source_order.txt` lists the 192 Android TUs
   in the order that reproduces the original `.text`; the iOS HD dump is
   alphabetical (`ParticleSettings.cpp`, `ParticleSystemManager.cpp`, …).
   The original `CMakeLists.txt`/`Android.mk` therefore had an explicit
   source list in that order, not a `file(GLOB)` sorted alphabetically.
7. **The shipped `.so` is stripped**: no `.debug_*`, `.symtab`, `.strtab` or
   `.gnu_debuglink`. The only debug-adjacent evidence is `.comment`,
   `.note.android.ident`, `.note.gnu.gold-version`, `.ARM.attributes` and the
   `.ARM.exidx`/`.ARM.extab` unwind tables. `-g` is inferred from codegen
   (round 6's per-TU sweep), not from surviving DWARF.

---

## 1. Evidence sources

| source | what it witnesses |
|---|---|
| `_work/bins/android_2.0.16_libgof2hdaa.so` | `.comment`, `.note.android.ident`, `.note.gnu.gold-version`, `.ARM.attributes`, `.gnu.version*`, `.dynsym`, `.dynstr`, `.hash`, `.text`, `.rodata`, `.data`, `.data.rel.ro`, `.bss`, `.ARM.exidx`, `.ARM.extab`, `.plt`, `.rel.plt`, `.rel.dyn`, build-id |
| `Original APK/…_2.0.16_APKPure.apk` | `lib/armeabi-v7a/libgof2hdaa.so`, `classes.dex`, `AndroidManifest.xml`, `META-INF/MANIFEST.MF`, `assets/crashlytics-build.properties`, zip metadata |
| `source file list dump/` | original source layout, TU membership, per-TU symbol ownership, header paths, MeeGo/mac assert strings |
| `_work/original_layout/` | parsed TU mapping (`tu_mapping.md`), original layout notes, `original_layout.json` |
| `.cache/ndk/android-ndk-r18b/` | exact r18b package, CMake toolchain, ndk-build defaults, crtbegin, libc++abi |
| `cmake-build-match/` | canonical match build for object-level A/B |
| `/dev/shm/w11219/vis`, `/dev/shm/w11219/build-hidden`, `/dev/shm/w11219/vs` | fresh flag A/Bs (scratch, not committed) |

`raw`/`aligned`/`size` are the official verifier's counts. `diff bytes` is the
sum of differing bytes over the differing sections (current baseline
**83,578** = `.text` 82,898 + `.ARM.extab` 660 + `.note.gnu.build-id` 20).

---

## 2. ELF metadata: what the binary itself says

### 2.1 Notes and `.comment` (byte-identical to the canonical build)

```text
.comment (303 B):
  Android (4751641 based on r328903) clang version 7.0.2
  (https://android.googlesource.com/toolchain/clang 003100370607242ddd5815e4a043907ea9004281)
  (https://android.googlesource.com/toolchain/llvm 1d739ffb0366421d383e04ff80ec2ee591315116)
  (based on LLVM 7.0.2svn)
  GCC: (GNU) 4.9.x 20150123 (prerelease)

.note.android.ident (152 B): namesz=8 descsz=0x84 type=1 "Android\0"
  api=0x10 (16), ndk="r18b", build="5063045"

.note.gnu.gold-version (28 B): "GNU\0" "gold 1.12"

.ARM.attributes (56 B): ARM v7-A, Thumb-2, VFPv3, NEONv1, GOT-indirect,
  wchar_t 4, enum int, 8-byte alignment, unaligned v6, no Tag_ABI_VFP_args
  (softfp).
```

All four sections are **byte-identical** to `cmake-build-match/libgof2hdaa.so`
(diff 0). The `.comment` contains exactly one clang identity and one GCC
identity; the GCC string is from the vendored OpenSSL archive, not from an app
TU. There is no second compiler identity, so no app TU used a different
compiler (or `-fno-ident` on every app TU, which is not a stock flag).

The `.note.android.ident` is byte-identical to the r18b
`platforms/android-16/arch-arm/usr/lib/crtbegin_so.o` note. It is emitted by
crtbegin, so it proves the **link** API 16, not per-TU API. The `.ARM.attributes`
is a single 56-byte set; because the linker merges per-object attributes, a
single set proves every TU used the same `-march/-mthumb/-mfpu/-mfloat-abi`.

### 2.2 Version sections and export policy

```text
.gnu.version_d: 1 entry, Flags BASE, Index 1, Name "libgof2hdaa.so"
.gnu.version_r: libc.so LIBC (version 2), libm.so LIBC (version 3)
defined dynsym: 5,370 symbols, all version index 1 (BASE); this count
  includes the 3 standard ABS end markers `_edata`, `_end`, `__bss_start`
defined names containing '@': 0
defined bind/visibility: 4,895 GLOBAL DEFAULT, 475 WEAK DEFAULT
```

The single BASE verdef is the normal gold output for a shared library linked
against versioned libc/libm; it is **not** evidence of a version script. A
named script would add a second verdef and `foo@@V1` names (see §6.2).

### 2.3 Section-level identity

Fresh `readelf`/byte comparison of the original and `cmake-build-match`:

| section | size | differing bytes |
|---|---:|---:|
| `.text` | 1,259,464 | **82,898** |
| `.ARM.exidx` | 28,528 | **0** |
| `.ARM.extab` | 156,312 | **660** |
| `.rodata` | 269,159 | **0** |
| `.data` | 10,604 | **0** |
| `.data.rel.ro` | 12,264 | **0** |
| `.bss` | 27,052 | **0** |
| `.dynsym` | 89,248 | **0** |
| `.dynstr` | 189,208 | **0** |
| `.hash` | 38,716 | **0** |
| `.got` | 17,164 | **0** |
| `.plt` | 44,108 | **0** |
| `.rel.plt` | 29,392 | **0** |
| `.rel.dyn` | 28,744 | **0** |
| `.shstrtab` | 279 | **0** |
| `.comment` | 303 | **0** |
| `.note.android.ident` | 152 | **0** |
| `.note.gnu.gold-version` | 28 | **0** |
| `.ARM.attributes` | 56 | **0** |
| `.note.gnu.build-id` | 36 | **20** |

The byte-identity of `.rodata`, `.data`, `.data.rel.ro`, `.bss`, `.dynsym`,
`.dynstr`, `.hash`, `.got`, `.plt` and `.rel.plt` is the strongest evidence
that the **export set, data layout and PLT shape** are exactly the canonical
default-visibility, no-`-fdata-sections` build. Any per-TU `-D`/`-include`
that changed a string, static initializer or symbol would break at least one
of these.

### 2.4 No surviving debug info

The section list is exactly 27 sections; there is no `.debug_*`, `.symtab`,
`.strtab` or `.gnu_debuglink`. The only debug-adjacent sections are
`.comment`, `.note.*`, `.ARM.attributes` and `.ARM.exidx`/`.ARM.extab`.
`-g` is therefore inferred from codegen (round 6's per-TU `-g0/-g1/-g2/-g3`
sweep: 48/192 TUs change under `-g1`, 0/192 under `-g2`/`-g3`), not from
surviving DWARF.

---

## 3. APK / DEX metadata: the packaging context

```text
META-INF/MANIFEST.MF:
  Manifest-Version: 1.0
  Built-By: Generated-by-ADT
  Created-By: Android Gradle 3.1.2

assets/crashlytics-build.properties:
  #Mon Oct 22 10:18:17 CEST 2018
  version_name=2.0.16
  package_name=net.fishlabs.gof2hdallandroid2012
  build_id=f527605b-309f-404d-8afc-b68027edc49e
  version_code=47947032
  app_name=GOF2

AndroidManifest.xml:
  package = net.fishlabs.gof2hdallandroid2012
  minSdkVersion = 16
  targetSdkVersion = 27

DEX BuildConfig:
  APPLICATION_ID = net.fishlabs.gof2hdallandroid2012
  BUILD_TYPE     = release
  DEBUG          = false
  FLAVOR         = ""
  VERSION_CODE   = 47947032
  VERSION_NAME   = 2.0.16
```

The APK ships only `lib/armeabi-v7a/` (`libgof2hdaa.so`, `libcrashlytics.so`,
`libfmodex.so`, `libfmodevent.so`). `Created-By: Android Gradle 3.1.2` proves
Gradle packaging, not the native driver: AGP 3.1.2 supports both
`externalNativeBuild.cmake` and `externalNativeBuild.ndkBuild`. The
Crashlytics timestamp is the closest witness to the native build date.

The APK `.so` is the same build as the ground truth: sha256
`6d38fd76…e3b58`, 288 differing bytes, all in `.shstrtab`/`sh_name`.
Re-running `tools/reorder_shstrtab.py` on the APK lib reproduces the
ground-truth sha256 `d4c17be6…bed9483` exactly (`cmp -l` 0). The difference is
the post-link bfd section-header rewrite, not a different build.

No build-system leftovers survive in the APK: no `Android.mk`,
`Application.mk`, `CMakeLists.txt`, `*.cmake`, `*.gradle` or `*.ninja`. The
DEX string table contains no `CMake`/`ndk-build`/`externalNativeBuild`
strings (only unrelated Firebase/Google Play text).

---

## 4. Reconstructed original build files

The exact build-file text is not recoverable from the stripped `.so`. What is
recoverable is the **effective compile line, link line and source order**. The
following is the best-fit reconstruction; comments distinguish witnessed
facts from inferred file text.

The source paths below are shown in the repo's `src/` taxonomy for
readability. The dumps witness the original paths as `AE2/Classes/...`
(engine) and `Classes/Game/...` (game, with `ParticleSystems/` and
`MeshMerger/` subfolders); the Android-only TUs (`NFC.cpp`,
`VirtualInput.cpp`, `main.c`, `GOF2HD2012.c`, `NdkHooks.cpp`, ...) have no
dump witness and their original Android path is not recoverable.

### 4.1 Reconstructed `CMakeLists.txt` (most likely)

```cmake
cmake_minimum_required(VERSION 3.6)          # AGP 3.1.2 bundles CMake 3.6.4111459
project(gof2hdaa C CXX)

# The 192 TUs in the original Android link order.  The order is load-bearing:
# gold emits .text in input order, so this list is the original .text order.
set(SOURCES
    platform/android/BackButton.c
    engine/NFC.cpp
    engine/VirtualInput.cpp
    engine/BlowfishSubkeyFile.c
    platform/android/main.c
    platform/android/GOF2HD2012.c
    engine/Mesh.cpp
    engine/GLES1Compat.cpp
    engine/FileInterfaceAndroid.cpp
    engine/EaseInOutMatrix.cpp
    # ... 182 more, exactly tools/source_order.txt ...
    engine/ParticleSystemSprite.cpp
    engine/ParticleSystemManager.cpp
    engine/ParticleSettings.cpp
    engine/ParticleSystemMesh.cpp
    engine/Array.cpp
    engine/ArrayInstantiations.cpp
)

add_library(gof2hdaa SHARED ${SOURCES})

target_include_directories(gof2hdaa PRIVATE
    AE2/Classes
    Classes/Game
    Classes/Game/ParticleSystems
    Classes/Game/MeshMerger
    Classes/Shader
    Classes/zlib
    FMOD/v44442/inc
    # Android glue: platforms/android, jni, GLES headers
)

target_compile_options(gof2hdaa PRIVATE
    -Oz -g -fPIC -frtti -fexceptions -fstack-protector-strong
    -ffunction-sections -funwind-tables
    -DANDROID -DNDEBUG
)

target_link_libraries(gof2hdaa
    log GLESv2 GLESv1_CM EGL android
    ${FMOD_DIR}/libfmodex.so ${FMOD_DIR}/libfmodevent.so
    -Wl,--whole-archive zlib libzip -Wl,--no-whole-archive
    crypto
    m c++abi android_support gcc dl c
)
```

The three non-default **toolchain settings** that the stock r18b
`android.toolchain.cmake` needs are:

```cmake
set(ANDROID_ABI armeabi-v7a)                 # r18b default
set(ANDROID_PLATFORM android-16)             # r18b default/minimum
set(ANDROID_STL c++_static)                  # r18b default
set(ANDROID_ARM_NEON ON)                     # NON-DEFAULT for API < 23
set(ANDROID_CPP_FEATURES "rtti exceptions")  # NON-DEFAULT (empty by default)
set(CMAKE_BUILD_TYPE Release)                # NON-DEFAULT (empty by default)
```

Round 6 / wave 120 already proved that a full-tree build with the stock r18b
CMake toolchain plus these three settings reproduces the canonical gate
(4507/4507/4524) and differs from the canonical build in only the 20-byte
build-id. The link line is the repo's custom/trimmed gold line (FMOD before
the STL archives, `libc++abi.a` + `libandroid_support.a` + `libgcc.a`, no
`libc++_static.a`/`libunwind.a`), not the vanilla CMake `c++_static` group.

### 4.2 Equivalent `Android.mk` / `Application.mk` (less likely)

```make
# Application.mk
APP_ABI := armeabi-v7a
APP_PLATFORM := android-16
APP_STL := none                 # avoid libc++_static.a / libunwind.a
APP_OPTIM := release
APP_CPPFLAGS := -fno-data-sections -frtti -fexceptions
APP_CFLAGS := -fno-data-sections

# Android.mk
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := gof2hdaa
LOCAL_SRC_FILES := ...          # the 192 TUs, same order as tools/source_order.txt
LOCAL_C_INCLUDES := $(LOCAL_PATH)/AE2/Classes \
                    $(LOCAL_PATH)/Classes/Game \
                    $(LOCAL_PATH)/Classes/Game/ParticleSystems \
                    $(LOCAL_PATH)/Classes/Game/MeshMerger \
                    $(LOCAL_PATH)/Classes/Shader \
                    $(LOCAL_PATH)/Classes/zlib \
                    $(LOCAL_PATH)/FMOD/v44442/inc
LOCAL_CPPFLAGS := -std=c++11 -fno-data-sections -frtti -fexceptions
LOCAL_CFLAGS := -Oz -g -DANDROID -DNDEBUG
LOCAL_LDLIBS := -llog -lGLESv2 -lGLESv1_CM -lEGL -landroid -lm -ldl -lc
LOCAL_SHARED_LIBRARIES := fmodex fmodevent
LOCAL_STATIC_LIBRARIES := zlib libzip crypto c++abi android_support gcc
include $(BUILD_SHARED_LIBRARY)
```

This is less likely because stock ndk-build adds `-fdata-sections`, defaults
to `APP_STL := system`, defaults to `-fno-exceptions -fno-rtti`, and links
static archives before shared libraries (`libdl, libc, libm` tail). A coerced
ndk-build with all of these overrides reaches only 2404/4504/4523 in the
wave-120 experiment, not byte-exact. The CMake toolchain matches all of these
observables without overrides.

### 4.3 Source-list order

`tools/source_order.txt` has 192 `src/` entries. The first ten and last six:

```text
0  src/platform/android/BackButton.c
1  src/engine/core/NFC.cpp
2  src/engine/input/VirtualInput.cpp
3  src/engine/file/BlowfishSubkeyFile.c
4  src/platform/android/main.c
5  src/platform/android/GOF2HD2012.c
6  src/engine/render/Mesh.cpp
7  src/engine/render/GLES1Compat.cpp
8  src/engine/file/FileInterfaceAndroid.cpp
9  src/engine/math/EaseInOutMatrix.cpp
...
187 src/engine/render/ParticleSystemManager.cpp
188 src/engine/render/ParticleSettings.cpp
189 src/engine/render/ParticleSystemMesh.cpp
190 src/engine/core/Array.cpp
191 src/engine/core/ArrayInstantiations.cpp
```

The iOS HD dump order is different and alphabetical:

```text
ParticleSettings.cpp, ParticleSystemManager.cpp, IParticleSystem.cpp,
ParticleSystemSprite.cpp, ParticleSystemMesh.cpp, Achievements.cpp,
AEGeometry.cpp, Agent.cpp, AutoPilotList.cpp, BeamGun.cpp, ...
```

So the original Android build had its own explicit source list, not a copy of
the Xcode project and not a lexicographic `file(GLOB)`. The 192-TU list is
the only witness; the exact `LOCAL_SRC_FILES`/`set(SOURCES)` text is
unrecoverable.

---

## 5. Per-TU flag audit

### 5.1 `-D` macros

The current source has no `#if` on `ANDROID`, `NDEBUG`, `DEBUG` or any other
build macro; the only conditionals are `__SIZEOF_POINTER__` (compiler-defined),
`GOF2_MATCH` (not defined in the build; `tools/drift_scan.py` notes it would
activate a known-failing Transform assert) and `GOF2_ARC_NOEXCEPT`
(source-defined in `PaintCanvas.cpp`). Fresh object A/Bs on
`ApplicationManager.cpp`, `RecordHandler.cpp` and `Galaxy.cpp`:

| variant | differing `.text`/`.rodata`/`.data` sections |
|---|---:|
| drop `-DANDROID` | 0 |
| drop `-DNDEBUG` | 0 |
| drop `-D__ANDROID_API__=16` | 0 |
| `-D__ANDROID_API__=21` | 0 |
| `-DDEBUG` | 0 |
| `-DGOF2_ARC_NOEXCEPT` | 0 on ApplicationManager/RecordHandler; 2 on Galaxy (source macro, not a build flag) |
| `-DGOF2_MATCH` | compile failure (known Transform assert) |

So `-DANDROID`, `-DNDEBUG` and the API macro are codegen-neutral on this
tree. The only macro with a witnessed effect is `NDEBUG`: the MeeGo 1.2.7 /
mac dumps list the app assert expressions, and none of them is present in
the Android `.so`:

```text
meshResourceIds.Length == objCount        absent
lodLevels > 0                             absent
duration[i] != -1                         absent
ParticleSettingsRef::assertInit == 42     absent
setIndex < particleSets.Length            absent
```

The original has exactly one `__assert2` import, from libc++abi's
`abort_message`. The app asserts are compiled out, so `NDEBUG` was on (or the
Android source removed the asserts). There is no evidence of a per-TU `-D`
difference; any such macro would have to be codegen- and data-neutral on all
4,507 exact functions and 843 exact data objects.

### 5.2 `-include`

No forced include is witnessed. Fresh `-include` A/Bs on
`ApplicationManager.cpp` with `Array.h`, `PaintCanvas.h`, `Vector.h` and
`AbyssEngine.h` give **0 differing `.text`/`.rodata`/`.data` sections**. The
original `.init_array` has exactly 3 entries and is byte-identical to the
canonical build, so a forced include with a static initializer would have
added a fourth entry. The `.rodata`/`.data` byte-identity rules out a forced
include that adds a string or data object. Any forced include must have been
a no-op on this source; it is not observable.

### 5.3 `-std=`

C++: the original is C++11. New independent size witnesses (both functions
are byte-exact in the canonical build):

| function | TU | original | `-std=c++11` | `-std=c++14` |
|---|---|---:|---:|---:|
| `RecordHandler::readRecordAsByteArray(void**, int, bool)` | `src/game/mission/RecordHandler.cpp` | **180 B** | 180 | 176 |
| `NewsItem::clone()` | `src/game/world/NewsItem.cpp` | **74 B** | 74 | 72 |

`-std=gnu++11` is byte-identical to `-std=c++11` on `Galaxy.cpp` (0
differing `.text` sections). `-std=c++14`/`c++17` also leave `Galaxy.cpp`
unchanged, but change `RecordHandler`/`NewsItem`; the original matches the
C++11 sizes. This is independent of round 6's signed-`new[]` guard argument.

C: the four C TUs (`BackButton.c`, `BlowfishSubkeyFile.c`, `main.c`,
`GOF2HD2012.c`) are codegen-neutral across `-std=c99/gnu99/c11/gnu11`
(0 differing sections). `-std=c89/gnu89` compiles `main.c`/`BackButton.c`/
`GOF2HD2012.c` unchanged but fails on `BlowfishSubkeyFile.c` (`//` comments /
`long long`). The original likely used the clang default (`gnu11`, no `-std`
flag); the exact C spelling is unobservable.

### 5.4 Include path

The source dumps give the original header roots (unique path counts from the
three dumps):

| dump | root | count |
|---|---|---:|
| gof2hd 1.1.19 | `AE2/Classes` | 9 |
| gof2hd 1.1.19 | `Classes/Game` | 108 |
| gof2hd 1.1.19 | `Classes/Game/ParticleSystems` | 8 |
| gof2hd 1.1.19 | `Classes/Game/MeshMerger` | 2 |
| gof2hd 1.1.19 | `Classes` (app glue) | 5 |
| gof2hd 1.1.19 | `Classes/Facebook`, `Classes/Facebook/JSON`, `Classes/AECommunityTools` | 13 (iOS-only) |
| gof2hd 1.1.19 | `FMOD/v44442/inc` | 3 |
| gof2sd 1.0.1 | `Svn_iPhone_AE2/Classes` (engine) | 52 |
| gof2sd 1.0.1 | `Svn_iPhone_AE2/Classes/Shader` | 9 |
| gof2sd 1.0.1 | `Classes/Game` | 98 |
| gof2sd 1.0.1 | `Classes/Game/ParticleSystems` | 8 |
| gof2sd 1.0.1 | `Classes/Game/MeshMerger` | 3 |
| gof2sd 1.0.1 | `Classes/zlib` | 10 |
| gof2sd 1.0.1 | `FMOD/v432/inc` | 3 |
| gof3d 1.1.4 | `Classes` (engine+game flat) | 52 |
| gof3d 1.1.4 | `Classes/Game` | 78 |
| gof3d 1.1.4 | `Classes/zlib` | 7 |

The Android build almost certainly used `-IAE2/Classes -IClasses/Game
-IClasses/Game/ParticleSystems -IClasses/Game/MeshMerger -IClasses/Shader
-IClasses/zlib -IFMOD/v44442/inc` plus Android glue paths. Our tree uses
`-Isrc -Ithird_party/fmod/inc -Ithird_party/gl -Ithird_party/jni
-Ithird_party/libzip -Ithird_party/crypto` (from `compile_commands.json`).
The spelling and order of the include paths are unobservable: no app
`__FILE__` string survives, and the resolved headers are the same. The
`.rodata`/`.data` byte-identity rules out a header-resolution difference.

---

## 6. `-fvisibility=hidden` and version scripts

### 6.1 `-fvisibility=hidden` is refuted

Object-level A/B (canonical compile command + `-fvisibility=hidden`):

| TU | differing `.text.*` sections | example |
|---|---:|---|
| `src/engine/core/ApplicationManager.cpp` | 7 / 88 | `OnUpdate` 4,576 -> 4,348 B; `EnablePerformanceTest` 124 -> 40 B |
| `src/engine/render/Engine.cpp` | 33 / 81 | `RenderMesh` 1,040 -> 1,008 B; `SetTextures` 312 -> 284 B |
| `src/game/menu/MGame.cpp` | 2 / 48 | `MGame::MGame()` 216 -> 212 B; `~MGame()` 56 -> 52 B |

These TUs are 100% byte-exact in the canonical build, so the original cannot
have hidden visibility.

Full-tree hidden build (`/dev/shm/w11219/build-hidden`, same link recipe):

```text
verify: compared 244  avg 99.92%  byte_exact 57  linked_exact 243
        missing 4,280 (wrong_type 0, absent 4,280)
```

| section | original | hidden build | differing bytes |
|---|---:|---:|---:|
| `.text` | 1,259,464 | 1,249,080 | 1,225,497 |
| `.plt` | 44,108 | 4,016 | 41,094 |
| `.rel.plt` | 29,392 | 2,664 | 28,226 |
| `.dynsym` | 89,248 | 12,576 | 82,306 |
| `.dynstr` | 189,208 | 14,311 | 188,693 |
| `.hash` | 38,716 | 5,236 | 35,709 |
| `.got` | 17,164 | 2,932 | 15,895 |

The original's `.dynsym`/`.dynstr`/`.hash`/`.plt`/`.rel.plt` are
**byte-identical** to the default-visibility canonical build. Hidden
visibility would have removed 4,280 exported functions and most of the PLT.
The original's 5,370 defined symbols are all `GLOBAL DEFAULT`/`WEAK DEFAULT`;
there is no `HIDDEN` visibility in dynsym. Conclusion: **default visibility**.

### 6.2 No named version script

Version-script probe (NDK r18b gold, `probe2.c` with `-lc`):

| link | `.gnu.version_d` | defined names |
|---|---|---|
| no script | 1 BASE entry (`libprobe2.so`) | `foo`, `bar` |
| anonymous `{ global: foo; bar; local: *; };` | 1 BASE entry | `foo`, `bar` |
| `{ global: *; };` | 1 BASE entry | `foo`, `bar`, `__bss_start`, `_edata`, `_end` |
| named `V1 { global: foo; bar; local: *; };` | **2 entries** (BASE + V1) | `foo@@V1`, `bar@@V1`, `V1` |

The original has exactly one BASE verdef, all 5,370 defined symbols at
version index 1, and no `@` in any defined name. Therefore there is **no
named version script**. An anonymous `{ global: *; };` script is
indistinguishable from no script and would be a no-op; a `local: *` script
would have to list all 5,370 symbols to preserve the export set, which is
implausible. Neither CMake nor ndk-build adds a version script by default.

### 6.3 What this means for the reconstructed build files

The reconstructed `CMakeLists.txt`/`Android.mk` should **not** contain
`-fvisibility=hidden`, `set_target_properties(... CXX_VISIBILITY_PRESET hidden)`,
`-Wl,--version-script=...`, or a `visibility` pragma. The original is a plain
default-visibility shared library with the standard gold BASE verdef from
linking against versioned libc/libm.

---

## 7. Per-TU flag uniformity: the data argument

The strongest general argument against per-TU flag differences is the
**byte-identity of the data and dynamic sections**:

```text
.rodata       269,159 B   diff 0
.data          10,604 B   diff 0
.data.rel.ro   12,264 B   diff 0
.bss           27,052 B   diff 0
.dynsym        89,248 B   diff 0
.dynstr       189,208 B   diff 0
.hash          38,716 B   diff 0
.got           17,164 B   diff 0
.plt           44,108 B   diff 0
.rel.plt       29,392 B   diff 0
.rel.dyn       28,744 B   diff 0
.init_array        12 B   diff 0 (3 entries)
.fini_array         8 B   diff 0 (2 entries)
```

A per-TU `-D` that changed a string literal, a static initializer or a data
object would move `.rodata`/`.data`/`.data.rel.ro`/`.bss`. A per-TU
`-fvisibility`/version-script difference would move `.dynsym`/`.dynstr`/
`.hash`/`.plt`/`.rel.plt`. A per-TU `-fdata-sections` difference would move
the data layout. None of these moved. The only residual is `.text` (17
exported functions) and `.ARM.extab` (their LSDA), which is a source-shape
residual, not a build-flag residual.

The `.ARM.attributes` single 56-byte set independently rules out a per-TU
`-march/-mfpu/-mfloat-abi/-mthumb` difference. The `.comment` single clang
identity rules out a per-TU compiler difference (or `-fno-ident` on every
TU). The `.note.android.ident` proves the link API 16.

---

## 8. Proven / disproven / unobservable

### Proven (direct artifact or controlled A/B)

* NDK **r18b**, clang **7.0.2**, gold **1.12**, API **16**, armeabi-v7a,
  Thumb-2, VFPv3, NEONv1, softfp, static libc++ (`libc++abi.a` +
  `libandroid_support.a` + `libgcc.a`), `-Oz -g`, RTTI + exceptions,
  `-fstack-protector-strong`, `-ffunction-sections`, **no
  `-fdata-sections`**, default visibility.
* The compile-flag set is exactly the stock r18b CMake toolchain defaults +
  `ANDROID_ARM_NEON=ON`, `ANDROID_CPP_FEATURES="rtti exceptions"`,
  `CMAKE_BUILD_TYPE=Release` (round 6 / wave 120 full-tree A/B: 4507/4507/4524,
  build-id-only difference from canonical).
* `-fvisibility=hidden` is refuted: full-tree hidden build is 57/244
  byte-exact, 4,280 missing, `.plt` 44,108 -> 4,016 B.
* No named version script: one BASE verdef, all 5,370 defined symbols at
  version index 1, no `@` names.
* C++11: `RecordHandler::readRecordAsByteArray` 180 B and
  `NewsItem::clone` 74 B match C++11, not C++14 (176/72).
* `-DANDROID`, `-DNDEBUG`, `-D__ANDROID_API__=16/21`, `-DDEBUG` are
  codegen-neutral on this tree; `NDEBUG` is witnessed by the absent app
  assert strings.
* `-include` of common headers is codegen-neutral; no forced include is
  witnessed.
* The original Android source-list order is `tools/source_order.txt`, not the
  alphabetical iOS dump order.
* The APK `.so` is the same build as the ground truth after the post-link
  `.shstrtab` rewrite.
* The shipped `.so` is stripped; no app debug info survives.

### Disproven

* **`-fvisibility=hidden`**: full-tree hidden build is 57/244 byte-exact with
  4,280 missing exports and `.plt` 4,016 B.
* **A named version script**: would add a second verdef and `@@V1` names; the
  original has one BASE verdef and no `@`.
* **Plain ndk-build defaults**: `-fdata-sections`, `APP_STL=system`,
  `-fno-exceptions -fno-rtti`, `libdl, libc, libm` tail, `_Unwind_*` local.
* **`-std=c++14`/`c++17`**: changes `RecordHandler`/`NewsItem` sizes away
  from the original.
* **A per-TU `-D`/`-include`/include-path difference that changes code or
  data**: `.rodata`/`.data`/`.data.rel.ro`/`.bss`/`.dynsym`/`.dynstr`/`.hash`
  are byte-identical.

### Unobservable (no surviving evidence)

* The exact `CMakeLists.txt` vs `Android.mk`/`Application.mk` text; the ELF
  proves the effective flags and link order, not the file that produced them.
* The exact CMake/Gradle version and generator (Ninja vs Make); CMake
  version is codegen-neutral. AGP 3.1.2 bundles CMake 3.6.4111459.
* The exact include-path spelling and order; no app `__FILE__` string
  survives, and the resolved headers are the same.
* The exact `-g2` vs `-g3` spelling; both are byte-identical in `.text`, and
  the debug sections are stripped.
* `-fPIC` vs `-fpic`, `-std=c++11` vs `gnu++11`; both are byte-identical on
  this source.
* `-DANDROID`/`-DNDEBUG` position, `-D__ANDROID_API__=16` spelling; codegen-
  neutral.
* The actual parallel compile order; only the link order is encoded.
* The native link timestamp; the Crashlytics `2018-10-22 10:18:17 CEST` is
  the closest witness.

---

## 9. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w11219
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
ulimit -v 12000000
cmake --build cmake-build-match --target check -j2
cmake --build cmake-build-match --target lint
cmake --build cmake-build-match --target drift

# section identity
python3 - <<'PY'
import struct
def secs(p):
    d=open(p,'rb').read(); shoff=struct.unpack_from('<I',d,32)[0]
    shent=struct.unpack_from('<H',d,46)[0]; shnum=struct.unpack_from('<H',d,48)[0]
    shstr=struct.unpack_from('<H',d,50)[0]
    S=[struct.unpack_from('<IIIIIIIIII',d,shoff+i*shent) for i in range(shnum)]
    sh=S[shstr]
    def sn(n):
        e=d.index(b'\0',sh[4]+n); return d[sh[4]+n:e].decode()
    return {sn(s[0]):d[s[4]:s[4]+s[5]] for s in S}
o=secs('/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so')
m=secs('cmake-build-match/libgof2hdaa.so')
for n in ['.text','.ARM.exidx','.ARM.extab','.rodata','.data','.data.rel.ro',
          '.bss','.dynsym','.dynstr','.hash','.got','.plt','.rel.plt',
          '.rel.dyn','.shstrtab','.comment','.note.android.ident',
          '.note.gnu.gold-version','.ARM.attributes','.note.gnu.build-id']:
    a,b=o[n],m[n]
    print(n,len(a),len(b),sum(x!=y for x,y in zip(a,b))+abs(len(a)-len(b)))
PY

# -fvisibility=hidden full-tree A/B
cat > /dev/shm/w11219/toolchain-hidden.cmake <<'EOF'
include("/home/debian/GalaxyOnFire2/wt/w11219/cmake/toolchain.cmake")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden" CACHE STRING "" FORCE)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fvisibility=hidden" CACHE STRING "" FORCE)
EOF
cmake -S . -B /dev/shm/w11219/build-hidden -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/dev/shm/w11219/toolchain-hidden.cmake \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build /dev/shm/w11219/build-hidden --target link_so -j2
python3 tools/verify/verify.py --build-dir /dev/shm/w11219/verify-hidden \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /dev/shm/w11219/build-hidden/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0

# version-script probe
N=.cache/ndk/android-ndk-r18b
GOLD=$N/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/arm-linux-androideabi/bin/ld.gold
RE=$N/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-readelf
# (see /dev/shm/w11219/vs for probe2.c and the four map files)
$RE -VW /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
```

Scratch helpers: `/dev/shm/w11219/vis` (object A/Bs), `/dev/shm/w11219/build-hidden`
(hidden full build), `/dev/shm/w11219/vs` (version-script probes). Not committed.

---

## 10. Files changed

* `_work/reconstructions/build-provenance-round7-build-system.md` — this note.
* No `src/`, `CMakeLists.txt`, baseline, allowlist, or doc change.
