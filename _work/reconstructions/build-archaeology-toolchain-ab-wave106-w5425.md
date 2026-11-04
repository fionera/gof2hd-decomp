# Build archaeology and toolchain-version A/B (wave 106, w5425)

Status: research note, 2026-11-04. Worktree `/home/debian/GalaxyOnFire2/wt/w5425`,
branch `wave106-w5425`, base `486f8a119`. Scratch: `/dev/shm/w5425`.
**No `src/`, `tools/`, baseline or allowlist change is landed**: no experiment
produced a strict gate improvement. This note is the deliverable.

Canonical gate before and after this pass (unchanged):

```text
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.91
data:   exact 843 of 843 exported objects
sodiff: original 5370  ours 5370  extra 0  missing 0 (wrong_type 0 absent 0)
imports: 207   size-exact: 4524/4524   lint: CLEAN   drift: 0
RATCHET PASS
```

Section content vs the original (canonical build, `.so` sha256
`2855359c4264ae9e69ff9adbef7d25b8505d3dad04384f4b9db2c71354be022a`):

| section | size | differing bytes |
|---|---:|---:|
| `.text` | `0x1337c8` | 99,226 |
| `.ARM.exidx` | `0x6f70` | 829 |
| `.ARM.extab` | `0x26298` | 108,294 |
| `.note.gnu.build-id` | `0x24` | 20 |
| all other sections | — | **0** |

## 0. Bottom line

* **Wave 104 is confirmed, not refuted.** The effective compile/link flag set is
  exactly the NDK r18b `android.toolchain.cmake` default set: no
  `-fdata-sections`, FMOD before `libc++abi.a`/`libgcc.a`, `-Oz -g`, static
  libc++ on a macOS host. Both discriminators were re-measured from scratch
  (`-fdata-sections` 4311/4500; full STL group before FMOD 1345/4500).
* **The original is NDK r18b, not r18.** The `.note.android.ident` in the
  shipped `.so` is byte-identical to r18b's `crtbegin_so.o`
  (`API 16`, `"r18b"`, build `5063045`); r18's note says `"r18"` / `5002713`.
  The clang binaries of r18 and r18b are the same file
  (sha256 `af8268675ca343698d3fae4bca21b0563762442204cbbc34555c8ddcf362376a`),
  so the note is the only r18-vs-r18b discriminator, and it is decisive.
* **No original build file survives** anywhere searched: APK, both IPAs, the
  Windows build, the source-layout dumps, the sibling repos or the public web.
  The only path-like string in the shipped `.so` is the darwin libc++abi
  buildbot path `/Volumes/Android/buildbot/src/android/ndk-release-r18/.../abort_message.cpp`.
* **New artifact found: a second official 2.0.16 build.** The
  `KiritoJPK_Galaxy-on-Fire-2-FULL-HD-Android` archive contains a 2.0.16
  `libgof2hdaa.so` built with **clang 3.8.275480 / gold 1.11 / gnustl_shared**
  (NDK r13b-or-earlier era), `.text` 17% larger than the r18b build. It shows
  the version string does not pin the toolchain, and its size is consistent
  with ndk-build's `-O2` release default (our `-O2` build: `.text`
  `0x163268`, within 1.4% of it).
* **New artifact found: the "Save fix" APK is the APKPure r18b lib + a 4-byte
  patch** (`RecordHandler::loadOptions` → `return 1`), with the *same* build-id
  and the *same* bfd `.shstrtab` as the APKPure APK lib. It independently
  confirms that the ground-truth file is the APK lib after
  `tools/reorder_shstrtab.py`.
* **Toolchain A/B: no alternative improves anything.** r18 is byte-identical
  to r18b (only build-id differs, because the build directory is in
  `DW_AT_comp_dir`). r17c (clang 6.0.2) gives byte 1260 / linked 2471; r19c
  (clang 8.0.2) gives 1401 / 4006; a full r19c swap (its own headers/sysroot/
  archives) gives 1322 / 3988 with 10 missing symbols.
* **CMake version is codegen-neutral.** CMake 3.6.0-rc2, 3.20.6, 3.25.3 and
  3.31.6 emit identical compile and link flags from the r18b
  `android.toolchain.cmake`; repo builds under 3.20.6 and 3.25.3 are
  byte-identical to the canonical build except `.note.gnu.build-id`.
* **`-g` level:** `-g`, `-g2`, `-g3`, `-gdwarf-4` are identical (only build-id
  differs); `-g0`, `-g1`, `-gline-tables-only` all drop to byte 2069 /
  linked 4247 / size 4485 (39 function-size mismatches) with `.text` still
  size-exact `0x1337c8`.
* **`-fdebug-compilation-dir` / `-fdebug-prefix-map`:** only the build-id
  changes; every section size and every section byte except the build-id note
  is identical. `DW_AT_comp_dir` is the only debug-info field they touch, and
  the shipped `.so` is stripped. Build-id was not chased.

## 1. Baseline and reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w5425
export TMPDIR=/dev/shm/w5425/tmp
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
cmake --build cmake-build-match --target drift -j6
```

Raw result:

```text
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.91
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
=== DRIFT: 0 field(s) across 0 class(es) ===
```

The canonical compile line (from `compile_commands.json`) is:

```text
clang++ -target armv7-none-linux-androideabi16 -march=armv7-a -mthumb
  -mfpu=neon -mfloat-abi=softfp -std=c++11 -fPIC -frtti
  -fstack-protector-strong -Oz -g -stdlib=libc++
  -isystem <r18b>/sources/cxx-stl/llvm-libc++/include
  -isystem <r18b>/sources/cxx-stl/llvm-libc++abi/include
  -isystem <r18b>/sources/android/support/include
  --sysroot=<r18b>/sysroot
  -isystem <r18b>/sysroot/usr/include
  -isystem <r18b>/sysroot/usr/include/arm-linux-androideabi
  -D__ANDROID_API__=16 -DANDROID -DNDEBUG
  -ffunction-sections -funwind-tables
```

There is **no `-fdata-sections`**. The canonical gold link line is:

```text
ld.gold -shared -soname libgof2hdaa.so --build-id -z now
  --exclude-libs=libgcc.a -o libgof2hdaa.unstripped.so
  crtbegin_so.o -L <r18b>/platforms/android-16/arch-arm/usr/lib
  <192 app objects in tools/source_order.txt order>
  --whole-archive libz_gof2.a libzip_gof2.a --no-whole-archive
  -l log -l GLESv2 -l GLESv1_CM -l EGL -l android
  libfmodex.so libfmodevent.so
  libcrypto_gof2.a
  -l m
  libc++abi.a libandroid_support.a libgcc.a
  -l dl -l c
  crtend_so.o
```

then GNU `arm-linux-androideabi-strip --strip-all`, then
`tools/reorder_shstrtab.py`.

## 2. Artifact search: what survives, and what does not

| location | build files found | other build evidence |
|---|---|---|
| `Original APK/...2.0.16_APKPure.apk` | **none** (`Android.mk`, `Application.mk`, `CMakeLists.txt`, `*.cmake`, `*.gradle`, `*.ninja`, `*.o`, `*.a`, `*.d`, `*.log` all absent) | `META-INF/MANIFEST.MF`: `Created-By: Android Gradle 3.1.2`; `assets/crashlytics-build.properties`: `#Mon Oct 22 10:18:17 CEST 2018`, `version_name=2.0.16`, `version_code=47947032`; `BuildConfig`: `BUILD_TYPE=release`, `FLAVOR=""`, `DEBUG=false`; manifest `minSdkVersion=16`, `targetSdkVersion=27` |
| APK `lib/armeabi-v7a/*.so` | — | `libcrashlytics.so`: gold **1.11**, `GCC: (GNU) 4.9.x 20150123`; `libfmodex.so` / `libfmodevent.so`: `GCC: (GNU) 4.4.3` (FMOD's own prebuilt, not Fishlabs) |
| `Original IPA/...1.1.4...ipa` (3180 files) | **none** (`*.pbxproj`, `*.xcconfig`, `*.dSYM`, `*.o`, `*.a` absent) | armv7 `MH_EXECUTE`, `LC_VERSION_MIN_IPHONEOS 4.3`, SDK 6.0, `LC_UUID F0BB0EED-...`; 1528 symtab symbols, **no DWARF**; only game source paths are the four `/Users/hck/Documents/Dev/GalaxyOnFire2_merged/Classes/Game/ParticleSystems/*.cpp` assert strings |
| `Original IPA/...1.0.5...ipa` | none | Xcode 4.0 / iPhoneOS 4.3; FMOD paths only |
| `Original Windows/Steam/...` | **none** (no `.pdb`, `.lib`, `.exp`, `.map`, `.sln`, `.vcxproj`) | PE32 i386, linker 10.0 (VS2010), timestamp 2012-09-05, PDB path `C:\Users\s.lohse\Desktop\GoF2_PC_REP\GOF2.pdb` |
| `source file list dump/` | **none** | iOS/mac/MeeGo source roots (`/Users/hck/...`, `/Users/n.tran/projects/gof2/`, `/Users/mhehmeyer/...`); no Android path, no build-system string |
| sibling repos | `gof2-vita/CMakeLists.txt` is a **Vita loader** (host shim), not the original build | `KaamoClubModApi` (Rust/xmake), `gof2edit` (Rust), `FMOD-GOF2-Recreated` (FMOD project) have no Android build scripts |
| public web | none | only APK mirrors, fan ports and decomp repos |
| shipped `libgof2hdaa.so` | — | `.comment`: clang 7.0.2 (4751641) + `GCC: (GNU) 4.9.x 20150123`; `.note.android.ident`: API 16, `r18b`, `5063045`; `.note.gnu.gold-version`: gold 1.12; only path-like string: `/Volumes/Android/buildbot/src/android/ndk-release-r18/external/libcxx/../../external/libcxxabi/src/abort_message.cpp` |

The `.comment` order (clang line first, GCC line second) is the link input
order: app objects before `libcrypto_gof2.a`. The only app source-file witness
in the `.so` is the literal `main.c` in three log strings; no app `comp_dir`
survives (the file is stripped).

## 3. New artifact A: a second official 2.0.16 build (clang 3.8)

`KiritoJPK_Galaxy-on-Fire-2-FULL-HD-Android/GOF2FHD_APK.7z` contains 14 APKs.
Thirteen ship a **different** `libgof2hdaa.so`: six identical copies of the
old build (2,469,656 B, sha256 `9a166d19...`), six identical Bloom patches of
it (sha256 `b125c918...`) and one New-system patch (sha256 `9183a68b...`).
The remaining APK (`Save fix/...`) ships the r18b build with a 4-byte patch
(§4). The `Original_Price` lib is a second official 2.0.16 build:

| property | APKPure 2.0.16 (ground truth) | KiritoJPK `Original_Price` 2.0.16 |
|---|---|---|
| size | 2,187,328 | 2,469,656 |
| sha256 | `d4c17be6...bed9483` | `9a166d19...f402fad` |
| `.comment` | clang 7.0.2 (4751641) + GCC 4.9.x 20150123 | `GCC: (GNU) 4.9.x 20150123` + `Android clang version 3.8.275480 (based on LLVM 3.8.275480)` |
| gold | 1.12 | **1.11** |
| `.note.android.ident` | API 16, `r18b`, `5063045` | **absent** (r13b-or-earlier `crtbegin_so.o`) |
| `DT_NEEDED` | static libc++ (no `libstdc++`) | **`libstdc++.so`** (`gnustl_shared`) |
| `.text` | `0x1337c8` @ `0x69440` | `0x1682c8` @ `0x61940` |
| `.rodata` | `0x41b67` | `0x427d4` |
| sections | 27 (`.note.android.ident`, `.text` before `.ARM.exidx`) | 26 (no note, `.ARM.extab` before `.ARM.exidx`) |
| build-id | `c011810da45777b83ed721b2a87d517efed0d310` | `1dbb7d451b7ca0ff0cfccf2f2d2fae0e8f9c5a1c` |

Interpretation (high confidence, not proven): the older lib is an
**ndk-build / `-O2` / `gnustl_shared`** build from the NDK r13b-or-earlier era;
the APKPure 2.0.16 is a 2018 rebuild with **CMake / r18b / `-Oz` /
`c++_static`**. The `.text` size supports the optimization level: our tree
built with `-O2` gives `.text` `0x163268` (1,454,696 B, 1.4% below the old
lib) while `-Oz` gives `0x1337c8` (exactly the ground truth). The two builds
have different exported-symbol counts (`.dynsym` `0x14b20` vs `0x15ca0`), so
they are not the same source/visibility state either.

The mods `Experimental/New system/...` and `Experimental/Post-processing FX
(Bloom)/...` are the old clang-3.8 lib with 10 and 2 differing `.text` bytes
respectively — i.e. binary patches of the old build, not rebuilds.

## 4. New artifact B: the "Save fix" APK is the APKPure lib + 4 bytes

`Save fix/GOF2_2.0.16_For_Save_Fix.apk` ships a 2,187,328-byte lib
(sha256 `7ce001c5...f8541930`) with:

* the **same build-id** `c011810da45777b83ed721b2a87d517efed0d310` as the
  ground truth;
* exactly **4 differing `.text` bytes** at `0xcd7b0`:
  `f0 b5 03 af` → `01 20 70 47` (`push {r4,r5,r6,r7,lr}; add r7,sp,#12` →
  `movs r0,#1; bx lr`), inside `RecordHandler::loadOptions()` (0xcd451,
  size 0x864) — the "save fix" is a forced `return 1`;
* a `.shstrtab` **byte-identical to the APKPure APK lib** (bfd/strip order),
  and different from the ground truth (gold Stringpool order).

Since the build-id is the SHA-1 of the *unstripped* link output, an unchanged
build-id plus a 4-byte `.text` patch means the mod author patched the shipped
file in place (the build-id was not recomputed). This independently confirms
the established relationship: ground truth = APKPure APK lib +
`tools/reorder_shstrtab.py` (288 differing bytes, all `.shstrtab`/`sh_name`).

## 5. Confirming the wave-104 discriminators

### 5.1 `-fdata-sections` is absent

Wrapper toolchain appends `-fdata-sections` to both C and C++ flags; full
rebuild in `/dev/shm/w5425/build-fdata`:

```text
canonical:      byte 4500  linked 4500  size 4524/4524
-fdata-sections byte 4311  linked 4500  size 4524/4524
```

Section sizes:

| section | original | canonical | `-fdata-sections` |
|---|---:|---:|---:|
| `.rodata` | `0x41b67` | `0x41b67` | **`0x41a07`** (-0x160) |
| `.data` | `0x296c` | `0x296c` | **`0x295c`** (-0x10) |
| `.data.rel.ro` | `0x2fe8` | `0x2fe8` | `0x2fe8` |
| `.text` | `0x1337c8` | `0x1337c8` | `0x1337c8` |

The original's `.rodata`/`.data` match the no-`-fdata-sections` build exactly.
ndk-build r18b *always* passes `-fdata-sections`
(`$NDK/build/core/default-build-commands.mk` line 137); the CMake r18b
toolchain does not. This is the primary driver discriminator.

### 5.2 FMOD before the libc++/libgcc archives

Relinking the canonical objects with the full CMake STL group
(`libc++_static.a libc++abi.a libandroid_support.a libunwind.a libgcc.a`)
moved before `libfmodex.so`:

```text
canonical (FMOD before STL):  byte 4500  linked 4500  .text 0x1337c8 @ 0x69440
STL group before FMOD:        byte 1345  linked 4500  .text 0x134dc4 @ 0x699a0
```

The original's `_Unwind_*` are `UND` (0-size) because FMOD re-exports them;
in the reordered link they become locally defined (`_Unwind_Resume` @
0x199535, 104 B, etc.) and every `.text` address shifts. ndk-build's link line
puts the STL archives before the user libraries:

```text
<objs> libc++_static.a libc++abi.a libandroid_support.a libunwind.a
       -lgcc --exclude-libs,libgcc.a -latomic ...
       -llog -ldl -lc -lm
```

The CMake r18b toolchain puts user libraries first, which is the original's
order. The original's `DT_NEEDED` order
(`log, GLESv2, GLESv1_CM, EGL, android, fmodex, fmodevent, m, dl, c`) also
matches the CMake `-l` order exactly (`-l m` before `-l dl -l c`); ndk-build's
default tail would put `dl`/`c`/`m` after the user libs.

### 5.3 r18b, not r18: the `.note.android.ident` discriminator

```text
r18  crtbegin_so.o note: 10 00 00 00 "r18"   ... "5002713"
r18b crtbegin_so.o note: 10 00 00 00 "r18b"  ... "5063045"
original .so note:       10 00 00 00 "r18b"  ... "5063045"
```

The clang binaries are the same file
(`af8268675ca343698d3fae4bca21b0563762442204cbbc34555c8ddcf362376a` in both
packages), so this note is the only r18-vs-r18b discriminator — and it points
squarely at r18b.

## 6. Toolchain-version A/B

Method: a wrapper toolchain includes `cmake/toolchain.cmake` and then
overrides only `CMAKE_CXX_COMPILER` / `CMAKE_C_COMPILER` (and
`GOF2_NDK_CLANG*` for the vendored libs) to the alternate NDK's clang. The
r18b headers, sysroot, static libs and gold linker stay fixed, so this isolates
compiler codegen. Full `verify.py` over all functions.

```cmake
include("/home/debian/GalaxyOnFire2/wt/w5425/cmake/toolchain.cmake")
set(_alt "/dev/shm/w5425/ndks/android-ndk-rXX/toolchains/llvm/prebuilt/linux-x86_64/bin")
set(CMAKE_CXX_COMPILER "${_alt}/clang++" CACHE FILEPATH "" FORCE)
set(CMAKE_C_COMPILER   "${_alt}/clang"   CACHE FILEPATH "" FORCE)
set(CMAKE_CXX_COMPILER "${_alt}/clang++")
set(CMAKE_C_COMPILER   "${_alt}/clang")
set(GOF2_NDK_CLANGXX "${_alt}/clang++" CACHE FILEPATH "" FORCE)
set(GOF2_NDK_CLANG   "${_alt}/clang"   CACHE FILEPATH "" FORCE)
```

```sh
cmake -S . -B /dev/shm/w5425/build-rXX -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/dev/shm/w5425/toolchain-rXX.cmake -DGOF2_MATCH_OPT=-Oz \
  -DGOF2_ORIGINAL_SO=.../android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=.../apk_libs
cmake --build /dev/shm/w5425/build-rXX --target link_so -j6
python3 tools/verify/verify.py --build-dir /dev/shm/w5425/build-rXX/verify \
  --original .../android_2.0.16_libgof2hdaa.so \
  --ours /dev/shm/w5425/build-rXX/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/.../arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
```

(r17c's clang needs `libtinfo.so.5`; it was run with
`LD_LIBRARY_PATH=/dev/shm/w5425/libcompat`, where `libtinfo.so.5 ->
/usr/lib/x86_64-linux-gnu/libtinfo.so.6`.)

| build | clang | `.text` | compared | byte_exact | linked_exact | size_exact | avg | missing |
|---|---|---:|---:|---:|---:|---:|---:|---:|
| **r18b (baseline)** | 7.0.2 | `0x1337c8` | 4524 | **4500** | **4500** | **4524** | 99.91 | 0 |
| r18 (compiler only) | 7.0.2 (same sha256) | `0x1337c8` | 4524 | **4500** | **4500** | **4524** | 99.91 | 0 |
| r17c (compiler only) | 6.0.2 | `0x145338` | 4523 | 1260 | 2471 | 3401 | 88.55 | 0 |
| r19c (compiler only) | 8.0.2 | `0x1336c8` | 4524 | 1401 | 4006 | 4328 | 98.36 | 0 |
| r19c (full swap) | 8.0.2 | `0x133510` | 4514 | 1322 | 3988 | 4315 | 98.34 | 10 absent |

* **r18 is byte-identical to r18b.** The two `.so` files differ only in
  `.note.gnu.build-id` (20 bytes) — because the build directories differ and
  the path is in `DW_AT_comp_dir` of the stripped debug info.
* r17c and r19c break 3,099-3,240 functions each; none of the 24 residual
  functions becomes exact.
* The full r19c swap (its own libc++ headers/sysroot/`libc++abi.a`/
  `libandroid_support.a`/`libgcc.a`) is worse still and loses 10 symbols
  (`absent`), because the newer libc++ headers change inline codegen.

**Conclusion:** the residual is source shape, not a toolchain-version artifact.
No alternate NDK is an improvement.

## 7. CMake-version A/B

### 7.1 Flag output of the r18b `android.toolchain.cmake`

A minimal probe (`add_library(probe SHARED probe.cpp)`) configured with
`-DANDROID_ABI=armeabi-v7a -DANDROID_PLATFORM=android-16
-DANDROID_STL=c++_static -DANDROID_ARM_NEON=ON
'-DANDROID_CPP_FEATURES=rtti exceptions' -DCMAKE_BUILD_TYPE=Release` was built
with each CMake. The compile and link flag sets are **identical** for
3.6.0-rc2 (`3.6.4111459`, the AGP 3.1.2-bundled version), 3.20.6, 3.25.3 and
3.31.6:

```text
clang++ --target=armv7-none-linux-androideabi16 --gcc-toolchain=... --sysroot=...
  -g -DANDROID -ffunction-sections -funwind-tables -fstack-protector-strong
  -no-canonical-prefixes -march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16
  -mthumb -mfpu=neon -Wa,--noexecstack -Wformat -Werror=format-security
  -std=c++11 -frtti -fexceptions -Oz -DNDEBUG -fPIC
...
  -Wl,--exclude-libs,libgcc.a -Wl,--exclude-libs,libatomic.a -nostdlib++
  -Wl,--build-id -Wl,--warn-shared-textrel -Wl,--fatal-warnings
  -Wl,--fix-cortex-a8 -Wl,--exclude-libs,libunwind.a -Wl,--no-undefined
  -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now -shared -Wl,-soname,libprobe.so
  -latomic -lm libc++_static.a libc++abi.a libandroid_support.a libunwind.a -ldl
```

Only the source-path spelling differs across CMake versions (absolute vs
`../probe.cpp`). Note the probe's default link order: user libs before the STL
archives — the CMake signature.

### 7.2 Repo build under alternative CMake versions

```sh
/dev/shm/w5425/cmakes/cmake-3.20.6-linux-x86_64/bin/cmake -S . -B /dev/shm/w5425/bc-3.20 -G Ninja ...
/dev/shm/w5425/cmakes/cmake-3.25.3-linux-x86_64/bin/cmake -S . -B /dev/shm/w5425/bc-3.25 -G Ninja ...
```

| CMake | gate | section diff vs canonical |
|---|---|---|
| 3.20.6 | byte 4500 / linked 4500 / size 4524 | **only `.note.gnu.build-id`** (20 B) |
| 3.25.3 | byte 4500 / linked 4500 / size 4524 | **only `.note.gnu.build-id`** (20 B) |
| 3.31.6 (canonical) | byte 4500 / linked 4500 / size 4524 | reference |

The repo's `CMakeLists.txt` requires CMake >= 3.20, so 3.6.0-rc2 can only be
used for the probe. **CMake version is unobservable in the binary**; it does
not affect codegen once the flags are fixed. The exact original CMake version
remains unknown; the r18b toolchain requires >= 3.6.0, and AGP 3.1.2's
SDK-bundled `cmake-3.6.4111459` is the most likely candidate.

## 8. `-g` level and debug-path A/B

All variants were built from the canonical tree; "differing `.text` bytes" is
vs the canonical `.so`.

| variant | `.text` size | `.text` differing bytes | byte_exact | linked_exact | size_exact | build-id |
|---|---:|---:|---:|---:|---:|---|
| `-g` (canonical) | `0x1337c8` | 0 | 4500 | 4500 | 4524 | `1adf4aae...` |
| `-g2` | `0x1337c8` | **0** | 4500 | 4500 | 4524 | `645c79a6...` |
| `-g3` | `0x1337c8` | **0** | 4500 | 4500 | 4524 | `aa939bb0...` |
| `-gdwarf-4` | `0x1337c8` | **0** | 4500 | 4500 | 4524 | (differs) |
| `-g0` | `0x1337c8` | 803,129 | **2069** | 4247 | **4485** | `317fd269...` |
| `-g1` | `0x1337c8` | 803,129 | **2069** | 4247 | **4485** | `8e44dffd...` |
| `-gline-tables-only` | `0x1337c8` | 803,129 | **2069** | 4247 | **4485** | `2edec041...` |
| `-fdebug-compilation-dir=/Users/fionera/.../gof2-decomp` | `0x1337c8` | **0** | 4500 | 4500 | 4524 | `cd34413b...` |
| `-fdebug-compilation-dir=/Volumes/Android/buildbot/...` | `0x1337c8` | **0** | 4500 | 4500 | 4524 | `cd7c5428...` |
| `-fdebug-prefix-map=/dev/shm/w5425=/Users/fionera/...` | `0x1337c8` | **0** | 4500 | 4500 | 4524 | `3823c559...` |

* `-g2`/`-g3`/`-gdwarf-4` are **byte-identical to `-g` except the build-id**:
  the debug level is not a codegen or section-size knob above `-g`.
* `-g0`/`-g1`/`-gline-tables-only` are all identical to each other and lose
  2,431 byte-exact functions: clang 7 `-Oz` keeps entry-block argument copies
  from PostRA sinking only when `DBG_VALUE` users exist. `.text` stays
  size-exact (`0x1337c8`), but 39 individual functions change size
  (`size_exact` 4485/4524).
* `-fdebug-compilation-dir` and `-fdebug-prefix-map` change **only**
  `DW_AT_comp_dir` (verified with `readelf --debug-dump=info` on a probe
  object: `/dev/shm/w5425` → `/Users/fionera/Downloads/GalaxyOnFire2/gof2-decomp`)
  and therefore **only the build-id**. Every section size and every section
  byte except `.note.gnu.build-id` is identical. This is the section-size
  plausibility check: the debug path cannot explain any residual.

Build-id was not chased: it is the SHA-1 of the *unstripped* gold output, so it
is sensitive to the exact `DW_AT_comp_dir`, file paths and debug info of the
original build directory, none of which survive. Our build-id differs from
`c011810d...` for that reason (and because `.text` still differs).

## 9. Definitive build recipe

**Proven by the binary:**

* NDK **r18b** (`crtbegin_so.o` note `r18b` / `5063045`), clang **7.0.2**
  (4751641), gold **1.12**, API **16**, `armeabi-v7a`, Thumb-2, NEONv1,
  VFPv3, softfp, GOT-indirect, 8-byte alignment, enum `int`, unaligned-v6.
* Static **libc++** (`c++_static`; no `libc++_shared.so`, no libstdc++).
* Compile flags: `-Oz -g -std=c++11 -fPIC -frtti -fexceptions
  -fstack-protector-strong -funwind-tables -ffunction-sections
  -DANDROID -DNDEBUG`, default visibility, frame pointers kept, **no
  `-fdata-sections`**.
* Link: one plain gold link, `-shared -soname libgof2hdaa.so --build-id
  -z now --exclude-libs=libgcc.a`, no `-s`, no section-ordering file,
  no `--version-script`, no `--unresolved-symbols`; `zlib`/`libzip` pulled
  `--whole-archive`; FMOD before `libcrypto`/`libm`/`libc++abi`/
  `libandroid_support`/`libgcc`; followed by GNU `strip --strip-all` and the
  `.shstrtab` reorder.
* Host: **macOS/darwin** NDK package (the darwin `libc++abi.a`
  `abort_message.cpp` path; the Linux archive gives byte 4344 instead of 4500).
* Link input order: crtbegin, app objects in `tools/source_order.txt` order,
  zlib/libzip whole-archive, `-llog -lGLESv2 -lGLESv1_CM -lEGL -landroid`,
  `libfmodex.so libfmodevent.so`, `libcrypto_gof2.a`, `-lm`, `libc++abi.a`,
  `libandroid_support.a`, `libgcc.a`, `-ldl -lc`, crtend.

**High confidence (effective flag set is the CMake r18b toolchain default set):**

* The build driver was **CMake with the NDK r18b `android.toolchain.cmake`
  defaults**, not vanilla ndk-build. The two independent discriminators are
  the absence of `-fdata-sections` (ndk-build always adds it; 4311 vs 4500)
  and the user-libs-before-STL link order (ndk-build puts STL first; 1345 vs
  4500). A non-default ndk-build could mimic both only with at least four
  overrides (`-fno-data-sections`, explicit archive order, `-Oz`, static
  libc++); the CMake route needs only `ANDROID_ARM_NEON=ON`,
  `ANDROID_CPP_FEATURES="rtti exceptions"` and `CMAKE_BUILD_TYPE=Release`.
* The APK was packaged by **AGP 3.1.2** on **2018-10-22 10:18:17 CEST**
  (`META-INF/MANIFEST.MF`, `crashlytics-build.properties`), release build,
  no flavor, `minSdk 16`, `targetSdk 27`.

**Unobservable / still hypothesis:**

* The exact build-system file (`CMakeLists.txt` text) and the exact driver
  (`externalNativeBuild { cmake { ... } }` vs a standalone CMake build copied
  into `jniLibs/armeabi-v7a/`). Both are consistent with the binary.
* The exact CMake version (>= 3.6.0; AGP 3.1.2's `3.6.4111459` is most
  likely). CMake version is codegen-neutral in every test.
* The exact CMake generator (Ninja vs Unix Makefiles) and macOS version.
* The exact original project directory names and source paths. The only
  surviving app source-file witness in the `.so` is `main.c` in three log
  strings; the only path-like string is the libc++abi buildbot path.
* Whether `libc++_static.a`, `libunwind.a` and `libatomic.a` were on the
  original link line: their members are not pulled after FMOD, and adding them
  in the CMake default positions is byte-neutral.
* `-DNDEBUG`, `-DANDROID`, `-D__ANDROID_API__=16` as macros (codegen-neutral).
* `-g` vs `-g2`/`-g3`/`-gdwarf-4` (byte-identical except build-id).

## 10. Reproduction commands (scratch only)

```sh
# baseline
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
cmake --build cmake-build-match --target drift -j6

# metadata
O=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
readelf -hW "$O"; readelf -AW "$O"; readelf -nW "$O"; readelf -p .comment "$O"
readelf -dW "$O"; readelf -SW "$O"
strings -a "$O" | grep -E '^(/|[A-Za-z]:\\)'

# APK / IPA / Windows / dumps
APK="/home/debian/GalaxyOnFire2/Original APK/Galaxy on Fire 2™ HD_2.0.16_APKPure.apk"
unzip -l "$APK"; unzip -p "$APK" META-INF/MANIFEST.MF
unzip -p "$APK" assets/crashlytics-build.properties
unzip -p "$APK" lib/armeabi-v7a/libgof2hdaa.so > /dev/shm/w5425/apk-lib.so
cmp -l /dev/shm/w5425/apk-lib.so "$O" | wc -l          # 288
unzip -l "/home/debian/GalaxyOnFire2/Original IPA/Galaxy_on_Fire_2__HD_1.1.4_ios_4.3.ipa" \
  | grep -iE 'pbxproj|xcconfig|dSYM|\.o$|\.a$'
strings -a /home/debian/GalaxyOnFire2/_work/bins/ios_1.1.4_HD | grep -E 'Classes/'
llvm-readobj --file-headers --coff-debug-directory \
  "/home/debian/GalaxyOnFire2/Original Windows/Steam/Galaxy On Fire 2 HD/GOF2.exe"

# second official build / save-fix APK
7z x -y /home/debian/GalaxyOnFire2/KiritoJPK_Galaxy-on-Fire-2-FULL-HD-Android/GOF2FHD_APK.7z -o/dev/shm/w5425/kirito
unzip -p "/dev/shm/w5425/kirito/GOF2_2.0.16_Original_Price.apk" \
  lib/armeabi-v7a/libgof2hdaa.so > /dev/shm/w5425/kirito/origprice.so
unzip -p "/dev/shm/w5425/kirito/Save fix/GOF2_2.0.16_For_Save_Fix.apk" \
  lib/armeabi-v7a/libgof2hdaa.so > /dev/shm/w5425/kirito/savefix.so
readelf -p .comment /dev/shm/w5425/kirito/origprice.so
readelf -n /dev/shm/w5425/kirito/origprice.so
readelf -SW /dev/shm/w5425/kirito/origprice.so
cmp -l /dev/shm/w5425/kirito/savefix.so "$O" | head

# alternative compiler (see §6 for the wrapper toolchain)
for v in r17c r18 r19c; do
  cmake -S . -B /dev/shm/w5425/build-$v -G Ninja \
    -DCMAKE_TOOLCHAIN_FILE=/dev/shm/w5425/toolchain-$v.cmake -DGOF2_MATCH_OPT=-Oz \
    -DGOF2_ORIGINAL_SO="$O" -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
  cmake --build /dev/shm/w5425/build-$v --target link_so -j6
done

# alternative CMake
/dev/shm/w5425/cmakes/cmake-3.20.6-linux-x86_64/bin/cmake -S . -B /dev/shm/w5425/bc-3.20 -G Ninja ...
/dev/shm/w5425/cmakes/cmake-3.25.3-linux-x86_64/bin/cmake -S . -B /dev/shm/w5425/bc-3.25 -G Ninja ...

# -g / debug-dir variants (wrapper appends flags after the canonical -g)
#   -g0, -g1, -g2, -g3, -gline-tables-only, -gdwarf-4,
#   -Xclang -fdebug-compilation-dir -Xclang <dir>, -fdebug-prefix-map=<old>=<new>
```

## 11. Files touched

* `_work/reconstructions/build-archaeology-toolchain-ab-wave106-w5425.md`
  (this note).
* Scratch only under `/dev/shm/w5425/` (NDKs r17c/r18/r19c, CMake
  3.6.4111459/3.20.6/3.25.3, probe projects, all A/B build dirs, the
  `KiritoJPK` extraction, `secdiff.py`, `fastcount.py`, `relink*.py`).
