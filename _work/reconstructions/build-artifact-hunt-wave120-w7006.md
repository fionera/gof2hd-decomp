# Build-artifact hunt, wave 120 (w7006)

Status: research note, 2026-11-06. Worktree `/home/debian/GalaxyOnFire2/wt/w7006`,
branch `wave120-w7006`, base `ef7fb9cde9b7b58f93721abed39ff871c66e593a`.
Scratch: `/dev/shm/w7006`. **No `src/`, `tools/`, baseline or allowlist change is
landed.** This note is the only commit.

Canonical gate at this base (unchanged by this pass):

```text
verify: byte 4505/4524  linked 4505/4524  size 4524/4524  avg 99.93
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
lint CLEAN ✓   drift 0
```

Ground truth: `_work/bins/android_2.0.16_libgof2hdaa.so`,
sha256 `d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483`,
2,187,328 bytes.

## 0. Bottom line

**No original Android build file survives.** No `Android.mk`, `Application.mk`,
`CMakeLists.txt`, `*.cmake`, `build.gradle`, `gradle.properties`, `*.ninja`,
`compile_commands.json`, `Makefile`, `*.pbxproj`, `*.vcxproj`, `*.sln`, build
script, build log, object file, static archive, dependency file or unstripped
symbol file belonging to the original Fishlabs Android build was found in the
APK, either IPA, the Windows build, the source-layout dumps, the KiritoJPK
archives/OBB, the sibling repos, or the public web.

What *does* survive is the ELF itself, and it pins the toolchain hard:

* **NDK r18b** (`Pkg.Revision = 18.1.5063045`, `.note.android.ident` build
  `5063045`), **clang 7.0.2** (`Android (4751641 based on r328903) ... based on
  LLVM 7.0.2svn`), **gold 1.12**, API 16, `armeabi-v7a`, Thumb-2, VFPv3,
  NEONv1, softfp, static libc++, `-Oz -g`, RTTI + exceptions,
  `-fstack-protector-strong`, `-ffunction-sections`, **no `-fdata-sections`**.
* The original was built with the **darwin/macOS NDK package** (high
  confidence): the only path-like string in `.rodata` is the darwin libc++abi
  buildbot path
  `/Volumes/Android/buildbot/src/android/ndk-release-r18/external/libcxx/../../external/libcxxabi/src/abort_message.cpp`
  (the Linux r18b `libc++abi.a` has the shorter `/buildbot/...` path, a 16-byte
  `.rodata` shift). The `.rel.plt` `sh_flags=AI`/`sh_info=20` header is a
  **GNU-strip bfd normalization**, not a host fingerprint: the original was
  gold-linked then GNU-stripped, and our Linux gold+strip build has the same
  header.
* The native build is **most consistent with the NDK r18b CMake toolchain
  defaults**, not stock ndk-build: no `-fdata-sections`, and the STL archives
  come *after* the user/shared libraries (FMOD before `libc++abi.a`;
  `_Unwind_*` stay undefined). This is a **medium-high confidence inference**,
  not proof of the original build file.

A second 2.0.16 APK in circulation (KiritoJPK `Original_Price`, VSFLima
stock, Joppie) carries a **different native lib**: NDK r14, clang 3.8.275480,
gold 1.11, gnustl (`libstdc++.so` NEEDED). Its manifest is a **modern repack**
(minSdk 19, targetSdk 30, compileSdk 23, platformBuild 30/11), so the APK
packaging is not original either; the r14 lib is still a useful second native
build witness, but it is not the ground-truth build.

## 1. Search inventory

Every location below was actually opened/listed/extracted; "no build files"
means a case-insensitive match for `Android.mk`, `Application.mk`,
`CMakeLists.txt`, `*.cmake`, `*.gradle`, `*.ninja`, `compile_commands.json`,
`Makefile`, `*.pbxproj`, `*.xcworkspace`, `*.xcconfig`, `*.vcxproj`, `*.sln`,
`*.suo`, `*.ncb`, `*.o`, `*.obj`, `*.a`, `*.lib`, `*.exp`, `*.map`, `*.d`,
`*.pdb`, `*.dSYM`, `build.sh`, `build.bat`, `*.log`.

| # | artifact / location | what was inspected | result |
|---:|---|---|---|
| 1 | `Original APK/Galaxy on Fire 2™ HD_2.0.16_APKPure.apk` | 143 ZIP entries; `META-INF/MANIFEST.MF`; `assets/crashlytics-build.properties`; `classes.dex`; all 4 native libs | **no build files**; `Created-By: Android Gradle 3.1.2`, `Built-By: Generated-by-ADT`; Crashlytics stamp `Mon Oct 22 10:18:17 CEST 2018`, `version_name=2.0.16`, `version_code=47947032`, `build_id=f527605b-309f-404d-8afc-b68027edc49e`; `classes.dex` has only generic Gradle/Firebase strings |
| 2 | APK native libs | `libgof2hdaa.so`, `libfmodex.so`, `libfmodevent.so`, `libcrashlytics.so` | no build-file strings in any; `libgof2hdaa.so` sha256 `6d38fd767ee3b4f8174c371bce371bfc6397cf76e13c1756938b2a94658e3b58` |
| 3 | Ground-truth `.so` | `readelf -n/-p/-S/-l/-d`, `strings -a` | `.comment`, `.note.android.ident`, `.note.gnu.gold-version`, build-id, 27 sections; **0** occurrences of `Android.mk`, `Application.mk`, `CMakeLists.txt`, `build.gradle`, `gradle.properties`, `Makefile`, `ndk-build`, `cmake`, `ninja`; only app source names are `main.c` log strings |
| 4 | `KiritoJPK_.../GOF2FHD_APK.7z` | 27 files / 5 dirs; 14 APKs + docs | **no build files**; `GOF2_2.0.16_Original_Price.apk` lib is r14/clang 3.8 (sha256 `9a166d19...`), manifest is a modern repack (minSdk 19 / targetSdk 30 / compileSdk 23 / platformBuild 30); `Experimental/New system` lib is r14 (sha256 `9183a68b...`); `Save fix` lib is r18b APKPure + 4 bytes |
| 5 | `KiritoJPK_.../GOF2FHD_OBB.7z` | 2,460-entry OBB (`main.47947006...obb`) | **no build files**; game content only |
| 6 | `KiritoJPK` `Save fix/GOF2_2.0.16_For_Save_Fix.apk` | lib extracted and byte-diffed | **no build files**; lib sha256 `7ce001c543b751a47a1f7ce5185ab98eb66c800f0c524fd94072d1d1f8541930`, build-id `c011810d...`, exactly 4 bytes differ from APKPure at file offset `0xcd7b0` (`f0 b5 03 af` → `01 20 70 47`) |
| 7 | `VSFLima/GOF2-HD-RE` stock APK | 16 ZIP entries; lib extracted | **no build files**; lib sha256 `125dfdb218470b76d5c41ee2eb00b55ce379963df8869ba5c6bece0fd040a50e`; `.comment` = clang 3.8.275480 + GCC 4.9.x; gold 1.11; `libstdc++.so` NEEDED; build-id `1dbb7d451b7ca0ff0cfccf2f2d2fae0e8f9c5a1c`; manifest is a modern repack (minSdk 19 / targetSdk 30 / compileSdk 23 / platformBuild 30) |
| 8 | `VSFLima/GOF2-HD-RE/scripts/build_mod.sh` | full script | only APK repacking: `unzip`, replace `libgof2hdaa.so`, `zip`, `zipalign`, `apksigner`; **cannot produce the ELF** |
| 9 | `VSFLima/GOF2-Mod` | `scripts/build.sh`, decompiled libs | `apktool`/smali APK build only; no native build; `libgof2hdaa_original.so` sha256 `ca7364c1...` differs from APKPure by 4 bytes |
| 10 | `JoppieToppie/Galaxy-on-Fire-2-Unity-Remake` | `Reference/binaries/GOF2_2.0.16_Android_Price.apk` (25 entries), `Reference/binaries/libgof2hdaa.so`, decompiled Java/native tree | **no build files**; APK sha256 `1100f136...`; lib sha256 `125dfdb2...` (r14); manifest is a modern repack (minSdk 19 / targetSdk 30 / compileSdk 23 / platformBuild 30); README calls it a modded APK, not original source |
| 11 | `Original IPA/Galaxy_on_Fire_2__1.0.5_ios_3.0.ipa` | 2,105 ZIP entries; `Payload/GalaxyOnFire2.app/Info.plist` | **no build files**; `DTXcode=0400`, `DTXcodeBuild=4A304a`, `DTSDKName=iphoneos4.3`, `DTCompiler=4.2`, `BuildMachineOSBuild=10J869`, `MinimumOSVersion=3.0` |
| 12 | `Original IPA/Galaxy_on_Fire_2__HD_1.1.4_ios_4.3.ipa` | 3,180 ZIP entries; `Payload/Galaxy on Fire 2 HD.app/Info.plist` | **no build files**; `DTXcode=0450`, `DTXcodeBuild=4G182`, `DTSDKName=iphoneos6.0`, `DTCompiler=com.apple.compilers.llvm.clang.1_0`, `BuildMachineOSBuild=11E53`, `MinimumOSVersion=4.3` |
| 13 | `Original Windows/Steam/Galaxy On Fire 2 HD/` | 2,212 files; `GOF2.exe`, `GoF2Launcher.exe` | **no build files** (no `.pdb`, `.lib`, `.exp`, `.map`, `.sln`, `.vcxproj`, `.vcproj`, `.suo`, `.ncb`, `.obj`, `.o`, `.a`, `.d`, `.log`, `Android.mk`, `Application.mk`, `CMakeLists.txt`, `Makefile`); PE32 i386; `GOF2.exe` linker 10.0, timestamp `2012-09-05 16:30:26`, PDB `C:\Users\s.lohse\Desktop\GoF2_PC_REP\GOF2.pdb`; `GoF2Launcher.exe` timestamp `2012-09-05 16:27:00`, PDB `...\GoF2Launcher.pdb` |
| 14 | `source file list dump/` | 4 files: `gof2hd_1.1.19_ios.cpp`, `gof2sd_1.0.1_ios.cpp`, `gof3d_1.1.4_ios.cpp`, `source file lists dump.txt` | **no build files**; sections are `windows`, `mac fhd`, `gof2 hd IOS 1.1.19`, `gof2 sd 1.0.1 ios`, `gof3d 1.1.5`, `gof3d 1.1.4`, `gof3d zeebo`; **0** occurrences of `Android`; 0 hits for build-system strings |
| 15 | Sibling repos `DeepOpen`, `gof2-vita`, `KaamoClubModApi`, `gof2edit`, `FMOD-GOF2-Recreated` | tree scan | only `gof2-vita/CMakeLists.txt` is a native build file; it is a fan Vita port, not the original Android build; no `Android.mk`, `Application.mk`, `build.gradle` or original `CMakeLists.txt` |
| 16 | `fionera/gof2hd-decomp` (mirror of this project) | HEAD `ef7fb9cde`; tree scan | only the reconstruction `CMakeLists.txt`; `Android.mk` appears only as text inside `_work/reconstructions/*.md` |
| 17 | `joewebkid/AbyssEngine` | HEAD `afe3a8e`; tree scan | only a reconstruction `CMakeLists.txt` |
| 18 | `KorolevSoftware/GOF2HD-linux-arm-port` | tree scan | fan Linux ARM port `CMakeLists.txt`; not the original Android build |
| 19 | `TheWWWorm/galaxian2` | tree scan | Godot remake; `platform/android_importer/build_android.py` is a Godot/NDK r27d build, not the original |
| 20 | `R00tFS/GOF2ModLoader`, `R00tFS/GOF2Haxx`, `DYY-Studio/GOF2HDPaymentCrack`, `Ravernstal/gof2-modding-scripts` | tree scan | mod loader/patch `Makefile`s and scripts only; no native game build |
| 21 | `Pb-207/GOF2_Re_FHD`, `SleathCobra/gof2-reverse`, `KiritoJPK/GOF2-FHD-Android-64-bits`, `Trimatix/AEPi`, `gxk2000/Galaxy-on-Fire2`, `Ravernstal/gof2-bin-info` | tree scan | README/notes/data only; no build files |
| 22 | GitHub code search (authenticated `gh`, account `fionera`) | `libgof2hdaa in:file` (1,974 hits), `gof2hdallandroid2012 in:file`, `GoF2_PC_REP in:file`, `fishlabs gof2 in:file`, `gof2 Android.mk in:file`, `gof2 Application.mk in:file`, `AbyssEngine CMakeLists in:file` | only decomp/mod/remake repos; **no original Fishlabs build file** |
| 23 | GitHub repo search | `gof2` (215), `"galaxy on fire 2"` (35), `fishlabs` (10), `GOF2 HD RE` | all known repos are mods, editors, remakes, ports, or this decomp; no source tree |
| 24 | Sourcegraph | `libgof2hdaa`, `gof2hdallandroid2012` | only `JoppieToppie/Galaxy-on-Fire-2-Unity-Remake` and `VelocityRa/awesome-game-file-format-reversing`; no build file |
| 25 | grep.app | `libgof2hdaa` | blocked by Vercel security checkpoint (no result) |
| 26 | searchcode.com API | `libgof2hdaa` | 404 (service unavailable) |
| 27 | archive.org advanced search | `libgof2hdaa` (0 hits); `"galaxy on fire 2" AND android` (4 items) | old 1.0.4/1.0.4.2/1.0.4.9 APKs only; downloaded `GalaxyOnFire2_1.0.4.apk` (sha256 `5784e492...`, 16,895,175 B) — **no build files** |
| 28 | GitLab / Bitbucket API | `gof2` | unrelated course/Java projects; no Fishlabs source |
| 29 | Web search (Brave/DuckDuckGo/Bing/Google HTML) | `libgof2hdaa`, `"libgof2hdaa" Android.mk`, `GOF2 HD source` | only GitHub decomp/mod repos and XDA save-editing threads; no build file |
| 30 | APKMirror / APKPure | `gof2` search pages | only APK download listings; no source/build assets |

## 2. Evidence table

| artifact | hard evidence | build-system implication | confidence |
|---|---|---|---:|
| ground-truth `.so` `.comment` | `Android (4751641 based on r328903) clang version 7.0.2 ... (based on LLVM 7.0.2svn)`; `GCC: (GNU) 4.9.x 20150123 (prerelease)` | NDK r18b clang 7.0.2; GCC string is libcrypto's `sha256.o`, not the app compiler | **certain** |
| `.note.android.ident` | API `16`, `r18b`, build `5063045` | NDK r18b (`18.1.5063045`), `APP_PLATFORM=android-16` (or omitted; r18b minimum is 16) | **certain** |
| `.note.gnu.gold-version` | `gold 1.12` | r18b gold | **certain** |
| `.note.gnu.build-id` | `c011810da45777b83ed721b2a87d517efed0d310` | plain gold `--build-id`; SHA-1 of the **unstripped** link output | **certain** |
| ELF header / `.ARM.attributes` | `ELF32 DYN ARM EABI5`, flags `0x5000200`; ARM v7, Thumb-2, VFPv3, NEONv1, no `Tag_ABI_VFP_args` | `-march=armv7-a -mthumb -mfpu=neon -mfloat-abi=softfp`; NEON explicitly enabled | **certain** |
| section set / layout | 27 sections; `.text 0x1337c8`, `.rodata 0x41b67`, `.ARM.exidx 0x6f70`, `.ARM.extab 0x26298`, `.data 0x296c`, `.bss 0x69ac`; `.hash` only, no `.gnu.hash` | r18b gold Android defaults; `-ffunction-sections`; **no `-fdata-sections`** (`.rodata`/`.data` match the no-`-fdata-sections` build, not the ndk-build default) | **certain** |
| `DT_NEEDED` order | `liblog libGLESv2 libGLESv1_CM libEGL libandroid libfmodex libfmodevent libm libdl libc` | FMOD before the STL archives; `libstdc++` absent; static libc++ | **certain** |
| `.rel.plt` section header | `sh_flags=AI`, `sh_info=20` | bfd-normalized by the post-link GNU `strip` (gold link then GNU strip); **not** a host fingerprint — our Linux gold+strip build has the same header | **certain** |
| only path-like app string | `/Volumes/Android/buildbot/src/android/ndk-release-r18/external/libcxx/../../external/libcxxabi/src/abort_message.cpp` | darwin NDK r18b package | **high** |
| `libgof2hdaa` strings | `Android.mk`, `Application.mk`, `CMakeLists.txt`, `build.gradle`, `gradle.properties`, `Makefile`, `ndk-build`, `cmake`, `ninja` all **0** occurrences | build file names/paths were not embedded; stripped `.so` has no `.symtab`/`.debug_*` | **certain (absence)** |
| APK `META-INF/MANIFEST.MF` | `Created-By: Android Gradle 3.1.2`, `Built-By: Generated-by-ADT` | APK packaging used Android Gradle plugin 3.1.2 (2018); says nothing about the native build system | **high (packaging) / low (native)** |
| APK `crashlytics-build.properties` | `#Mon Oct 22 10:18:17 CEST 2018`; `version_name=2.0.16`; `version_code=47947032`; `build_id=f527605b-...` | build/package date and app version; not a native build fingerprint | **high (date) / low (native)** |
| APK `AndroidManifest.xml` | `versionCode=47947032`, `versionName=2.0.16`, package `net.fishlabs.gof2hdallandroid2012`, `minSdkVersion=16`, `targetSdkVersion=27`, `glEsVersion=0x20000` | API 16 floor matches `.note.android.ident`; targetSdk is Java-side only | **high** |
| second 2.0.16 APK (KiritoJPK `Original_Price`, VSFLima stock, Joppie) | clang 3.8.275480, gold 1.11, `libstdc++.so` NEEDED, no `.note.android.ident`, path `/Volumes/Android/buildbot/src/android/ndk-r14-release/...`; manifest is a modern repack (minSdk 19 / targetSdk 30 / compileSdk 23 / platformBuild 30) | a **different** native 2.0.16 build: NDK r14 / gnustl, not the ground truth; the APK packaging is also not the original APKPure package | **certain** |
| Save-fix APK | same build-id `c011810d...`; exactly 4 bytes differ at `0xcd7b0` (`f0 b5 03 af` → `01 20 70 47`) | APKPure r18b lib + 4-byte patch, not a rebuild | **certain** |
| Windows `GOF2.exe` | PE32 i386, linker 10.0, timestamp `2012-09-05 16:30:26`, PDB `C:\Users\s.lohse\Desktop\GoF2_PC_REP\GOF2.pdb` | Visual Studio 2010 PC build by `s.lohse`; no Android build file | **high** |
| IPAs | Xcode 4.0/4.5, SDK 4.3/6.0, clang 4.2/Apple clang; no project files | iOS build metadata only; no Android build file | **high** |
| source-layout dumps | Windows/mac/iOS/Zeebo source lists; **0** `Android` occurrences | dumps predate or omit the Android TU list; no Android build file | **certain (absence)** |

## 3. Candidate build files and whether they can reproduce the original fingerprints

### 3.1 Reconstructed ndk-build `Android.mk` / `Application.mk`

From `_work/reconstructions/ndkbuild_android_mk_w3568.md` (a reconstruction, **not**
a surviving original file):

```make
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
LOCAL_SRC_FILES := $(shell cat /tmp/w3568/srcs.txt)
LOCAL_CPP_FEATURES := rtti exceptions
LOCAL_ARM_NEON := true
LOCAL_C_INCLUDES := $(LOCAL_PATH)/src \
    $(LOCAL_PATH)/third_party/fmod/inc \
    $(LOCAL_PATH)/third_party/gl \
    $(LOCAL_PATH)/third_party/jni \
    $(LOCAL_PATH)/third_party/libzip \
    $(LOCAL_PATH)/third_party/crypto
LOCAL_CFLAGS := -Wno-int-to-pointer-cast -Wno-int-to-void-pointer-cast
LOCAL_LDLIBS := -llog -lGLESv2 -lGLESv1_CM -lEGL -landroid \
                -L<apk-libs> -lfmodex -lfmodevent -lm -ldl
LOCAL_WHOLE_STATIC_LIBRARIES := zlib libzip
LOCAL_STATIC_LIBRARIES := crypto
include $(BUILD_SHARED_LIBRARY)
```

```make
APP_ABI := armeabi-v7a
APP_PLATFORM := android-16
APP_STL := c++_static
APP_OPTIM := release
```

**Can it produce the original?** Not with stock r18b ndk-build defaults.
ndk-build r18b adds `-fdata-sections` (original has none: `.rodata` `0x41b67`
vs `0x41a07`, 365 differing bytes) and places static STL archives *before* the
shared libraries, which pulls `libunwind.a` and defines `_Unwind_*` (original
keeps them UND). A real default ndk-build build of this tree gives
**byte 1345 / linked 3843** vs the canonical CMake **4500 / 4500** at that
tree state (**4505 / 4505** now). With
`LOCAL_CFLAGS := -fno-data-sections` and `APP_STL := none` plus explicit
`libc++abi.a`/`libandroid_support.a` and a custom link order, the full
ndk-build compile set is byte-identical to the canonical build except the
20-byte `.note.gnu.build-id`; the match-mode link is byte-identical to the
CMake baseline. So a hand-written ndk-build *can* reproduce the ELF, but it
must override the two ndk-build defaults that the binary refutes.

### 3.2 Reconstructed CMake recipe

From `_work/reconstructions/build-system-ndk-vs-cmake-wb105.md` §4.1 and
`build_provenance_w4208.md` §7.1 (again a reconstruction, **not** a surviving
original file):

```cmake
cmake_minimum_required(VERSION 3.10)
project(gof2hdaa CXX)
add_library(gof2hdaa SHARED ${GOF2_SOURCES})
target_compile_features(gof2hdaa PRIVATE cxx_std_11)
target_link_libraries(gof2hdaa PRIVATE
  ${FMOD_DIR}/libfmodex.so ${FMOD_DIR}/libfmodevent.so
  ${CRYPTO_A})
target_link_options(gof2hdaa PRIVATE
  -Wl,--whole-archive ${ZLIB_A} ${LIBZIP_A} -Wl,--no-whole-archive)
```

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

**Can it produce the original?** Yes, in the reconstructed tree: the two
observable CMake-vs-ndk-build discriminators both point at CMake — **no
`-fdata-sections`** (CMake r18b default) and **STL archives after the
user/shared libraries** (CMake default; ndk-build puts them before). The
current hand-written `cmake/toolchain.cmake` is byte-for-byte equivalent to
the stock NDK r18b CMake Release defaults for all 192 app TUs; the full
default link flags produce the same canonical `.so` hash. `ANDROID_ARM_NEON=ON`,
`ANDROID_CPP_FEATURES="rtti exceptions"` and `CMAKE_BUILD_TYPE=Release` are
the only non-default settings needed. This is the **best-supported**
reconstruction, but it is an inference from the ELF, not a witnessed file.

### 3.3 Other candidate files (all fan/decomp, not original)

| file | what it is | can it produce the original ELF? |
|---|---|---|
| `fionera/gof2hd-decomp/CMakeLists.txt` | this project's reconstruction | yes, by construction; not original |
| `joewebkid/AbyssEngine/CMakeLists.txt` | independent decomp reconstruction | no evidence it was the original build |
| `KorolevSoftware/GOF2HD-linux-arm-port/CMakeLists.txt` | Linux ARM fan port | no (different target/ABI) |
| `VSFLima/GOF2-HD-RE/scripts/build_mod.sh` | APK repackager (`unzip` → replace `.so` → `zip` → `zipalign` → `apksigner`) | no; it only swaps a prebuilt `.so` |
| `VSFLima/GOF2-Mod/scripts/build.sh` | `apktool`/smali APK build | no native build |
| `TheWWWorm/galaxian2/platform/android_importer/build_android.py` | Godot/NDK r27d remake build | no (different engine/toolchain) |
| `R00tFS/GOF2ModLoader/Makefile` | mod loader | no |

## 4. Hard evidence vs speculation

**Hard evidence (read directly from artifacts):**

* Toolchain identity: `.comment` = clang 7.0.2 / NDK r18b; `.note.android.ident`
  = API 16, `r18b`, build `5063045`; `.note.gnu.gold-version` = gold 1.12.
* ABI/ISA: ELF32 ARM EABI5, softfp, Thumb-2, VFPv3, NEONv1.
* Optimization/debug: `-Oz`, `-g` (from `.comment`/section layout and A/B),
  `-fstack-protector-strong`, `-ffunction-sections`, **no `-fdata-sections`**.
* Link shape: 27 sections, `.hash` only, `DT_NEEDED` order, `BIND_NOW`,
  `DT_RELCOUNT 1162`, `.rel.plt` `sh_flags=AI`/`sh_info=20`, no `R_ARM_REL32`,
  no `DT_TEXTREL`, `PT_GNU_RELRO`/`PT_GNU_STACK` layout.
* Host (high confidence): darwin/macOS (the libc++abi `/Volumes/Android/...`
  path; the Linux archive uses `/buildbot/...`, a 16-byte `.rodata` shift).
  The `.rel.plt` `AI`/`20` header is a GNU-strip normalization, not host
  evidence.
* Absence: no build file, build log, object, archive, dependency file, or
  unstripped symbol file in any searched artifact.
* Second 2.0.16 build: NDK r14 / clang 3.8 / gold 1.11 / gnustl.

**Inference (consistent with the evidence, not proven):**

* The native build used **CMake**, not ndk-build: the two decisive defaults
  (no `-fdata-sections`; STL archives after user libs) match CMake r18b and
  refute stock ndk-build r18b.
* The build was driven by Android Gradle plugin 3.1.2 (`Created-By`), most
  likely via `externalNativeBuild { cmake { ... } }`; the native build itself
  is CMake either way.
* The original source tree was the flat `Classes/` + `Classes/Game/` layout
  seen in the source dumps, with the Android TU list not preserved.

**Speculation (cannot be recovered from the stripped `.so`):**

* Exact `CMakeLists.txt` text, CMake version, build directory, generator
  (Ninja vs Make), and whether `ANDROID_ARM_NEON`/`ANDROID_CPP_FEATURES` were
  set in the build file or on the command line.
* Exact original `LOCAL_SRC_FILES` / source order (recoverable only from the
  `.text` order and the source dumps).
* Exact host macOS version and Xcode/command-line-tools version.
* Whether the original used a standalone `ndk-build` invocation with
  non-default overrides rather than CMake (possible, but less likely).

The build-id is the SHA-1 of the **unstripped** gold output. Because the
shipped `.so` is stripped, exact build-id matching requires reproducing the
original `.symtab`/`.debug_*`/paths/TU order, not just the final code; the
build-id therefore cannot be used to recover the build file.

## 5. Commands used (representative)

```sh
# APK / IPA / OBB inventory
unzip -Z1 "$APK" | grep -Ei 'Android\.mk|Application\.mk|CMakeLists|\.cmake|\.gradle|\.ninja|compile_commands|Makefile|\.o$|\.a$|\.d$|\.pdb|\.map'
unzip -p "$APK" META-INF/MANIFEST.MF
unzip -p "$APK" assets/crashlytics-build.properties
unzip -p "$APK" classes.dex > /tmp/classes.dex && strings /tmp/classes.dex | grep -Ei 'gradle|Android\.mk|CMakeLists'

# ELF fingerprints
sha256sum _work/bins/android_2.0.16_libgof2hdaa.so
readelf -h -S -l -d -n -p .comment -p .note.gnu.gold-version "$SO"
strings -a "$SO" | grep -E 'abort_message|Volumes|Android\.mk|CMakeLists|build\.gradle|ndk-build|cmake|ninja'

# APK vs ground truth
unzip -p "$APK" lib/armeabi-v7a/libgof2hdaa.so > /tmp/apk_lib.so
cmp -l /tmp/apk_lib.so _work/bins/android_2.0.16_libgof2hdaa.so | wc -l   # 288, all .shstrtab/sh_name

# second build
readelf -p .comment stock/libgof2hdaa_stock.so
readelf -d stock/libgof2hdaa_stock.so | grep NEEDED
readelf -n stock/libgof2hdaa_stock.so | grep 'Build ID'

# Windows PE
python3 - <<'PY'
import struct, datetime
d=open('GOF2.exe','rb').read()
pe=struct.unpack_from('<I',d,0x3c)[0]
print(hex(struct.unpack_from('<I',d,pe+8)[0]))
PY

# IPAs
unzip -Z1 "$IPA" | grep -Ei 'project\.pbxproj|\.xcodeproj|\.xcworkspace|\.xcconfig|\.dSYM|\.o$|\.a$|Makefile|CMakeLists'
unzip -p "$IPA" 'Payload/*.app/Info.plist' | plutil -p -   # or python plistlib

# public web
gh api -X GET 'search/code?q=libgof2hdaa+in:file&per_page=50'
gh api -X GET 'search/code?q=gof2+Android.mk+in:file&per_page=30'
gh api -X GET 'search/code?q=gof2+Application.mk+in:file&per_page=30'
curl -s 'https://sourcegraph.com/.api/search/stream?q=context:global+libgof2hdaa&v=V3&t=literal'
curl -s 'https://archive.org/advancedsearch.php?q=libgof2hdaa&fl%5B%5D=identifier&rows=20&output=json'
```

## 6. Confidence

| claim | confidence | basis |
|---|---:|---|
| No original Android build file survives in the searched artifacts | **very high (~98%)** | direct listing of every artifact; no build file found |
| Original toolchain = NDK r18b / clang 7.0.2 / gold 1.12 / API 16 / armeabi-v7a / softfp / static libc++ | **certain** | `.comment`, `.note.android.ident`, `.note.gnu.gold-version`, ELF header, `.ARM.attributes` |
| Original host = darwin/macOS | **high (~90%)** | libc++abi `/Volumes/Android/...` path (Linux archive is `/buildbot/...`, 16-byte `.rodata` shift) |
| Original native build = CMake r18b defaults (not stock ndk-build) | **medium-high (~80%)** | no `-fdata-sections`; STL archives after user libs; ndk-build defaults refuted by A/B |
| Exact original build-file text / CMake version / build dir | **unrecoverable** | stripped `.so`; build-id is SHA-1 of unstripped output |
| Second 2.0.16 APK is NDK r14 / clang 3.8 / gold 1.11 / gnustl | **certain** | `.comment`, gold version, `libstdc++.so` NEEDED |
| Public web has no original Fishlabs source tree or build scripts | **high (~90%)** | GitHub/Sourcegraph/archive.org/GitLab/Bitbucket/web searches |

## 7. Explicit statement

**No original Android build file survives.** The original `Android.mk`,
`Application.mk`, `CMakeLists.txt`, `build.gradle`, or equivalent was not
shipped in the APK, either IPA, the Windows build, the source-layout dumps,
the KiritoJPK archives/OBB, the sibling repos, or any public repository found.
Every build file discussed above is either a **reconstruction** (this project
or another decomp) or a **fan port/mod**; none is a witnessed Fishlabs file.
The original build system can only be inferred from the ELF fingerprints, and
the best-supported inference is **NDK r18b CMake toolchain defaults on a
darwin host**.
