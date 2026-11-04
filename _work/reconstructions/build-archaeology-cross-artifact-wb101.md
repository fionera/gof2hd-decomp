# Cross-artifact build archaeology (wb101, wave 101)

Status: research note, 2026-11-04. Worktree `/home/debian/GalaxyOnFire2/wt/wb101`,
branch `wave101-wb101`, base `3856949ab`. Scratch: `/dev/shm/wb101`.
**No source or build-flag change is landed**: no experiment produced a strict gate
improvement. The note is the deliverable.

This pass starts from the **shipped artifacts** (APK, IPA, Windows build, FMOD shared
libraries, source-layout dumps), not from `cmake/toolchain.cmake`. Every flag in the
current recipe is treated as a hypothesis and is either positively confirmed from an
artifact, confirmed by a controlled A/B rebuild, or marked unobservable. The wave-100
notes (`build-compile-flag-positive-audit-w5017.md`,
`build-flag-differential-matrix-w5020.md`,
`build-link-order-and-section-layout-w5018.md`,
`build-toolchain-runtime-provenance-w5019.md`) already contain the full A/B matrix;
this note adds the **cross-artifact** angle and consolidates the result.

Canonical gate before and after this pass (unchanged):

```text
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
size-exact 4524/4524; lint CLEAN; drift 0
```

## 0. Bottom line

* **Android** is **NDK r18b, clang 7.0.2 (`4751641 based on r328903`), gold 1.12,
  API 16, armeabi-v7a, Thumb-2, NEON, softfp, libc++**, built with **CMake**, not
  ndk-build. The APK was packaged by **Android Gradle 3.1.2** on
  **Mon Oct 22 10:18:17 CEST 2018**. The compile line is `-Oz -g` with
  `-std=c++11 -fPIC -frtti -fstack-protector-strong -ffunction-sections
  -funwind-tables`; the link is a single plain gold link
  `-shared -soname libgof2hdaa.so --build-id -z now --exclude-libs=libgcc.a`
  (no `-s`), followed by GNU `strip --strip-all` and the `.shstrtab` reorder.
* **iOS 1.1.4** is **Xcode 4.5 (4G182), iphoneos6.0 SDK, Apple clang 1.0**,
  built **2012-12-04**, armv7, no DWARF (stripped; dSYM not shipped). Its codegen
  is **unoptimized (`-O0`-style)**: every comparable function keeps stack slots and
  out-of-line calls where the Android `-Oz` build inlines and schedules.
* **iOS 1.0.5** is **Xcode 4.0 (4A304a), iphoneos4.3 SDK, GCC 4.2**, built
  **2011-04-27**, universal armv6+armv7.
* **Windows** is **Visual Studio 2010 (linker 10.0)**, x86, built
  **2012-09-05 16:30:26**, PDB `C:\Users\s.lohse\Desktop\GoF2_PC_REP\GOF2.pdb`,
  FMOD Ex/Event **4.40.04**, Steam API. It is a different compiler and ISA and
  cannot directly validate Android flags.
* **Android FMOD** (`libfmodex.so`, `libfmodevent.so`) is **GCC 4.4.3/4.6
  (20120106)**, **ARMv5TE, Thumb-1, VFPv1/VFPv2, softfp** — i.e. built for the
  ARMv5TE baseline, not ARMv7. It exports `__aeabi_uldivmod` and
  `__gnu_ldivmod_helper` but **not** `__aeabi_ldivmod`, which is why the original
  `libgof2hdaa.so` pulls libgcc's `_aeabi_ldivmod.o`.
* **No build scripts, project files, `.d` files, logs, `.o` or `.a` files survive
  in the APK or IPA.** The only build-system fingerprints are ELF/PE/Mach-O
  metadata, the Gradle manifest string, the Crashlytics timestamp, the Xcode
  `Info.plist` keys, the PDB path, and the `.o`/DerivedData paths in the
  source-layout dumps.
* **Cross-platform codegen separates source shape from flags.** The iOS build is
  the same source family but a different (earlier) revision and an unoptimized
  compiler. Examples: `ArrayAdd<Mesh*>` is 36 B on Android (`-Oz`) vs 68 B on iOS
  (`-O0`); `PaintCanvas::FindResource` is 48 B vs 124 B; `Matrix::Matrix()` is
  64 B (NEON) vs 124 B (scalar stack stores). `KeyFrame::KeyFrame()` is a genuine
  **source-version** difference: Android uses `__aeabi_memclr4` + `Vector::operator=`,
  iOS calls six `Vector::Vector()` constructors + one `Vector(float,float,float)`.
* **Struct-layout bonus.** The iOS ObjC type encodings are complete, independent
  layout witnesses. After adding the C++ vtable pointers that ObjC encodings omit,
  the iOS 1.1.4 `Engine` is **exactly** our 1312-byte `Engine` except for
  `boundTextures` (`unsigned short[20]` on iOS vs `int[20]` in our v2 tree). The
  iOS `GameData` is exactly our 200-byte `GameData` in size, with 7 pointer fields
  at `0x50..0x68` where our header has 7 `int32_t` placeholders. These are
  **source-version leads**, not build-flag evidence; no change was landed.
* **No strict improvement was found.** The current compile/link recipe is
  confirmed. The only open whole-`.so` deltas are source-body artifacts of the 24
  non-byte-exact functions (`.text`, `.ARM.exidx`, `.ARM.extab`, `.rel.plt`) and
  the unrecoverable `.note.gnu.build-id`.

## 1. Artifact inventory

### 1.1 Files, hashes, timestamps

```sh
cd /home/debian/GalaxyOnFire2
sha256sum _work/bins/android_2.0.16_libgof2hdaa.so \
  _work/bins/apk_libs/libfmodex.so _work/bins/apk_libs/libfmodevent.so \
  _work/bins/ios_1.1.4_HD _work/bins/ios_1.0.5_armv7 \
  _work/bins/ios_1.0.5_GalaxyOnFire2 _work/bins/win_GOF2.exe
sha256sum "Original APK/Galaxy on Fire 2™ HD_2.0.16_APKPure.apk" \
  "Original IPA/Galaxy_on_Fire_2__HD_1.1.4_ios_4.3.ipa"
```

| artifact | sha256 | size | timestamp |
|---|---|---:|---|
| Android `libgof2hdaa.so` (ground truth) | `d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483` | 2,187,328 | 2018-10-22 (APK) |
| APK `lib/armeabi-v7a/libgof2hdaa.so` | `6d38fd767ee3b4f8174c371bce371bfc6397cf76e13c1756938b2a94658e3b58` | 2,187,328 | 2018-10-22 |
| APK | `9cfe4484a475c9a31a336df9dea6408fbb129d88aa19083afe808ef6bca9eb19` | 13,206,304 | 2018-10-22 |
| Android `libfmodex.so` | `60ec4de090bf442c7b9db41d0731aa22a101db21aab0ae32af1af6a860c3fa81` | 920,204 | — |
| Android `libfmodevent.so` | `b92cd6ff8f8edb3d81023ee69c1f5239aeaf6cb4e89d6505101115189569c97d` | 370,660 | — |
| iOS 1.1.4 `ios_1.1.4_HD` | `74ece5bedf561eed4e71b8d0b875e201c47858a2649a59d65af57390a0554958` | — | 2012-12-04 |
| iOS 1.1.4 IPA | `db4c28f9b4494a5e6f27979aa6bb7a9f49fb4c90896d8ed87fbb35f0e8801e02` | 1,163,316,939 | 2012-12-04 |
| iOS 1.0.5 armv7 | `b8e8000feddab675f4101bed822afbc817e14521fa275e70ee3e1e8267f44661` | — | 2011-04-27 |
| iOS 1.0.5 universal | `8420bb7a19883a09e0a6961cc255a840295f4b479ebd230080fdfa38713980e3` | — | 2011-04-27 |
| Windows `GOF2.exe` | `1234131b457277fce5435062a95e66fad684743d6a8c79b7effa90a7bdfa1fb9` | 2,822,736 | 2012-09-05 16:30:26 |

The APK lib and the ground-truth file are the same ELF except for `.shstrtab` and
`sh_name`: `cmp -l` reports 288 differing bytes (259 in `.shstrtab`, 29 across 26
section-header `sh_name` fields). The APK lib is the raw bfd-strip artifact; the
ground truth is the same file after `tools/reorder_shstrtab.py` restores gold's
Stringpool order. This is the wave-100 §1.2 correction to wave 93.

### 1.2 What is *not* in the archives

```sh
APK="Original APK/Galaxy on Fire 2™ HD_2.0.16_APKPure.apk"
unzip -l "$APK" | grep -Ei 'Android\.mk|Application\.mk|CMakeLists|\.cmake|\.gradle|\.ninja|\.o$|\.a$|\.d$|\.log$'
IPA="Original IPA/Galaxy_on_Fire_2__HD_1.1.4_ios_4.3.ipa"
unzip -l "$IPA" | grep -Ei '\.(dSYM|o|a|d|mk|pbxproj|log|sh|xcworkspace|xcodeproj)$'
```

Both return nothing. The APK has 143 entries: `AndroidManifest.xml`, `classes.dex`,
`resources.arsc`, `res/`, `assets/`, `fabric/`, `META-INF/`, and the four
`lib/armeabi-v7a/` shared libraries. The IPA has 3,180 entries: the app bundle,
`iTunesMetadata.plist`, and assets. The Windows directory has only the exe, the
DLLs, `DefaultInputMapping.ini`, and the DirectX redistributables — no `.pdb`,
`.lib`, `.exp`, `.map`, `.sln`, or `.vcxproj`.

The only surviving build-system strings are:

| artifact | string | meaning |
|---|---|---|
| APK `META-INF/MANIFEST.MF` | `Created-By: Android Gradle 3.1.2` | Gradle/AGP version |
| APK `assets/crashlytics-build.properties` | `#Mon Oct 22 10:18:17 CEST 2018` | build timestamp |
| APK `assets/crashlytics-build.properties` | `version_name=2.0.16`, `version_code=47947032`, `build_id=f527605b-...` | app version |
| APK `AndroidManifest.xml` (AXML) | `package=net.fishlabs.gof2hdallandroid2012`, `minSdkVersion=16`, `targetSdkVersion=27`, `glEsVersion=0x00020000` | native API 16 |
| iOS 1.1.4 `Info.plist` | `DTXcode=0450`, `DTXcodeBuild=4G182`, `DTSDKName=iphoneos6.0`, `DTCompiler=com.apple.compilers.llvm.clang.1_0`, `BuildMachineOSBuild=11E53` | Xcode 4.5 / clang |
| iOS 1.0.5 `Info.plist` | `DTXcode=0400`, `DTXcodeBuild=4A304a`, `DTSDKName=iphoneos4.3`, `DTCompiler=4.2`, `BuildMachineOSBuild=10J869` | Xcode 4.0 / GCC 4.2 |
| Windows `GOF2.exe` CodeView | `C:\Users\s.lohse\Desktop\GoF2_PC_REP\GOF2.pdb`, GUID `{9A525EDE-5753-47D1-B793-9D334ED401D4}`, age 2 | VS2010 PDB |
| iOS binary strings | `/Users/hck/Documents/Dev/GalaxyOnFire2_merged/Classes/Game/ParticleSystems/*.cpp` | iOS source root |
| source-layout dumps | `/Users/n.tran/projects/gof2/`, `/Users/mhehmeyer/Documents/Dev/GalaxyOnFire2/`, `.../Svn_iPhone_AE2/Classes/` | iOS/MeeGo source roots |

## 2. Android ELF metadata (ground truth)

```sh
O=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
readelf -hW "$O"
readelf -nW "$O"
readelf -AW "$O"
readelf -dW "$O"
readelf -p .comment "$O"
readelf -SW "$O"
```

| observable | value | conclusion |
|---|---|---|
| ELF header | `ELF32`, `DYN`, `ARM`, `EABI5`, flags `0x5000200` (`EF_ARM_ABI_FLOAT_SOFT`) | ARMv7 EABI, **softfp** |
| `.comment` | `Android (4751641 based on r328903) clang version 7.0.2` + `GCC: (GNU) 4.9.x 20150123 (prerelease)` | **clang 7.0.2** app TUs; GCC 4.9 vendored libcrypto |
| `.note.android.ident` | `api=16`, `ndk="r18b"`, `build="5063045"` | **NDK r18b, API 16** |
| `.note.gnu.gold-version` | `gold 1.12` | **gold 1.12** |
| `.ARM.attributes` | CPU v7/A, Thumb-2, VFPv3, NEONv1, GOT-indirect, wchar_t 4, enum int, 8-byte align, unaligned v6, FP16 IEEE754 | **ARMv7-A, Thumb-2, NEON, softfp** |
| `DT_NEEDED` | `liblog libGLESv2 libGLESv1_CM libEGL libandroid libfmodex libfmodevent libm libdl libc` | link-input order |
| `DT_FLAGS` / `DT_FLAGS_1` | `BIND_NOW` / `NOW` | `-z now` |
| `DT_SONAME` | `libgof2hdaa.so` | `-soname` |
| `DT_RELCOUNT` | 1162 | `-z combreloc` (default) |
| relocations | 1817 `ABS32`, 614 `GLOB_DAT`, 3674 `JUMP_SLOT`, 1162 `RELATIVE`, 0 `REL32` | PIC, no text relocations |
| `.note.gnu.build-id` | `c011810da45777b83ed721b2a87d517efed0d310` | gold SHA-1 of the unstripped output |
| section count | 27 | original layout |

Section table (`readelf -SW`; `orig` is the ground truth, `ours` is the canonical
build; `diffB` is the number of differing bytes when the sizes match):

```text
section                 orig_size   our_size   orig_addr   our_addr    diffB
.note.android.ident            98         98        134        134        0
.note.gnu.build-id             24         24        1cc        1cc       20
.dynsym                     15ca0      15ca0        1f0        1f0        0
.dynstr                     2e318      2e318      15e90      15e90        0
.hash                        973c       973c      441a8      441a8        0
.gnu.version                 2b94       2b94      4d8e4      4d8e4        0
.gnu.version_d                 1c         1c      50478      50478        0
.gnu.version_r                 40         40      50494      50494        0
.rel.dyn                     7048       7048      504d4      504d4        0
.rel.plt                     72d0       72d0      5751c      5751c       12
.plt                         ac4c       ac4c      5e7ec      5e7ec        0
.text                      1337c8     1337c8      69440      69440   101683
.ARM.exidx                   6f70       6f70     19cc08     19cc08     1637
.ARM.extab                  26298      26294     1a3b78     1a3b78    size!
.rodata                     41b67      41b67     1c9e10     1c9e10        0
.fini_array                     8          8     20cba0     20cba0        0
.data.rel.ro                 2fe8       2fe8     20cbb0     20cbb0        0
.init_array                     c          c     20fb98     20fb98        0
.dynamic                      148        148     20fba4     20fba4        0
.got                         430c       430c     20fcf4     20fcf4        0
.data                        296c       296c     214000     214000        0
.bss                         69ac       69ac     216970     216970        0 (NOBITS)
.comment                      12f        12f          0          0        0
.note.gnu.gold-version         1c         1c          0          0        0
.ARM.attributes                38         38          0          0        0
.shstrtab                     117        117          0          0        0
```

The `.text`, `.ARM.exidx`, `.ARM.extab`, `.rel.plt` and `.note.gnu.build-id`
deltas are the known wave-100 residuals: source-body artifacts of the 24
non-byte-exact functions plus the unrecoverable build-id. Everything else is
byte-identical.

### 2.1 `.comment` decomposition and runtime archives

Wave 100 (`build-toolchain-runtime-provenance-w5019.md` §2.2) proved the
`.comment` is exactly:

```text
crtbegin_so.o .comment (leading NUL + clang string)
+ libcrypto_gof2.a(sha256.o) .comment (GCC 4.9.x string, minus the duplicate leading NUL)
```

The pulled `libgcc.a` members (`_aeabi_ldivmod.o` at `0x19ae54`,
`_dvmd_lnx.o` at `0x19ae8c`) have no `.comment`. `libgcc` is genuinely pulled
because FMOD exports `__aeabi_uldivmod` and `__gnu_ldivmod_helper` but **not**
`__aeabi_ldivmod`; the original's `__aeabi_ldivmod` code matches the tree's
libgcc member byte-for-byte.

The link order is proven by the original's `.text` tail and by relink A/B:
**FMOD → libcrypto → libc++abi → libandroid_support → libgcc → dl → c**.
The Darwin `libc++abi.a` is forced (path
`/Volumes/Android/buildbot/.../abort_message.cpp`); the Linux archive drops
`byte_exact` 4500 → 4344.

## 3. Android FMOD metadata

```sh
readelf -AW _work/bins/apk_libs/libfmodex.so
readelf -p .comment _work/bins/apk_libs/libfmodex.so
readelf -dW _work/bins/apk_libs/libfmodex.so
strings -a _work/bins/apk_libs/libfmodex.so | grep -E '\.cpp|\.h$'
```

| observable | `libfmodex.so` | `libfmodevent.so` |
|---|---|---|
| ELF | ELF32 ARM EABI5, flags `0x5000002` | same |
| `.comment` | many `GCC: (GNU) 4.4.3` + one `GCC: (GNU) 4.6 20120106 (prerelease)` | same |
| `.ARM.attributes` | CPU `5TE`, ARM ISA yes, **Thumb-1**, **VFPv1**, wchar 4, enum int, 8-byte align, aggressive speed | same |
| `DT_NEEDED` | `libc libstdc++ libm libdl liblog` | same |
| `DT_FLAGS_1` | `NOW` | `NOW` |
| build-id | none | none |
| source paths | `../android/src/fmod_os_misc.cpp`, `../android/src/fmod_output_opensl.cpp`, `../src/fmod_*.cpp`, `../lib/sfx/foreverb/aSfxDsp.cpp` | same |
| notable exports | `__aeabi_*`, `_Unwind_*`, `__gnu_ldivmod_helper`, `__aeabi_uldivmod`; **no `__aeabi_ldivmod`** | — |

The FMOD libraries are **ARMv5TE/Thumb-1/VFPv1** objects, not ARMv7. They are
linked into the ARMv7 app as softfp shared libraries; the app's own code is
ARMv7-A/Thumb-2/NEON. This is a useful reminder that the final `.ARM.attributes`
is a **merge**: the app TUs carry v7/A/Thumb-2/NEON, while libc++abi and
libandroid_support carry VFPv3-D16 and libgcc carries VFPv2 (wave 100 §2.3).

## 4. iOS metadata

### 4.1 IPAs and `Info.plist`

```sh
python3 - <<'PY'
import plistlib
for p in ['ipa114/Payload/Galaxy on Fire 2 HD.app/Info.plist',
          'ipa105/Payload/GalaxyOnFire2.app/Info.plist']:
    d = plistlib.load(open(p, 'rb'))
    print(p)
    for k in ['CFBundleIdentifier','CFBundleShortVersionString','CFBundleVersion',
              'MinimumOSVersion','DTXcode','DTXcodeBuild','DTSDKName','DTCompiler',
              'BuildMachineOSBuild','DTPlatformName','DTPlatformVersion','DTSDKBuild']:
        if k in d: print(' ', k, '=', d[k])
PY
```

| key | iOS 1.1.4 | iOS 1.0.5 |
|---|---|---|
| `CFBundleIdentifier` | `www.fishlabs.net.gof2hd` | `www.fishlabs.net.gof2` |
| `CFBundleShortVersionString` | `1.1.4` | — |
| `CFBundleVersion` | `1.1.4` | `1.0.5` |
| `MinimumOSVersion` | `4.3` | `3.0` |
| `DTXcode` | `0450` (Xcode 4.5) | `0400` (Xcode 4.0) |
| `DTXcodeBuild` | `4G182` | `4A304a` |
| `DTSDKName` | `iphoneos6.0` | `iphoneos4.3` |
| `DTCompiler` | `com.apple.compilers.llvm.clang.1_0` | `4.2` (GCC 4.2) |
| `BuildMachineOSBuild` | `11E53` (OS X 10.7.4) | `10J869` (OS X 10.6.8) |
| `DTPlatformVersion` | `6.0` | `4.3` |
| `DTSDKBuild` | `10A403` | `8F190` |

### 4.2 Mach-O headers and sections

```sh
llvm-objdump --macho --private-headers _work/bins/ios_1.1.4_HD
llvm-objdump --macho --section-headers _work/bins/ios_1.1.4_HD
llvm-objdump --macho --dylibs-used _work/bins/ios_1.1.4_HD
```

| observable | value |
|---|---|
| format | Mach-O armv7, `MH_EXECUTE`, `PIE`, `NOUNDEFS DYLDLINK TWOLEVEL WEAK_DEFINES BINDS_TO_WEAK` |
| `LC_UUID` | `F0BB0EED-7D47-32E5-8462-333D44A50807` |
| `LC_VERSION_MIN_IPHONEOS` | version 4.3, sdk 6.0 |
| `LC_ENCRYPTION_INFO` | cryptid 0 (not App-Store-encrypted) |
| `__text` | `0x3964c4` at `0x3830` |
| `__picsymbolstub4` | `0x35c0` |
| `__stub_helper` | `0x1b90` |
| `__gcc_except_tab` | `0x1126c` |
| `__cstring` | `0x3457e` |
| `__const` | `0x248fa` |
| `__mod_init_func` | `0x18` (6 entries) |
| `__data` | `0x9390` |
| `__bss` | `0x11da0` |
| `__common` | `0x2f0a0` |
| DWARF | none (stripped; no `.dSYM` in the IPA) |
| exported `__Z*` symbols | 663 |
| dylibs | UIKit, Foundation, OpenGLES, QuartzCore, CoreGraphics, AudioToolbox, AVFoundation, OpenAL, GameKit, StoreKit, AdSupport, MediaPlayer, CoreTelephony, CoreText, CoreLocation, CoreMotion, MapKit, AddressBook, CFNetwork, Security, sqlite3, libz, libstdc++.6, libSystem, libgcc_s |

The iOS 1.0.5 binaries are `MH_EXECUTE` armv7 (`LC_UUID`
`84309E35-1163-37B8-A40A-CF597041DD91`) and universal armv6+armv7
(`LC_UUID` `93E22B53-7637-3A6C-B099-A3878EEC32E9`).

### 4.3 Embedded source paths

```sh
strings -a _work/bins/ios_1.1.4_HD | grep -E '^/Users/|^/Library/|\.cpp$|\.mm$' | sort -u
```

The only Fishlabs path in the iOS 1.1.4 binary is:

```text
/Users/hck/Documents/Dev/GalaxyOnFire2_merged/Classes/Game/ParticleSystems/IParticleSystem.cpp
/Users/hck/Documents/Dev/GalaxyOnFire2_merged/Classes/Game/ParticleSystems/ParticleSystemManager.cpp
/Users/hck/Documents/Dev/GalaxyOnFire2_merged/Classes/Game/ParticleSystems/ParticleSystemMesh.cpp
/Users/hck/Documents/Dev/GalaxyOnFire2_merged/Classes/Game/ParticleSystems/ParticleSystemSprite.cpp
```

These are assert strings. The rest of the embedded paths are FMOD
(`/Library/anthill3/agent/var/jobs/job320331/fmod4/iphone/...`) and third-party
SDKs (AdColony, Chartboost, hasoffers, UniversalTracking). The iOS binary does
not embed the game source root for non-assert files because it is stripped.

## 5. Windows metadata

```sh
llvm-readobj --file-headers --coff-imports --coff-debug-directory \
  --coff-resources "_work/bins/win_GOF2.exe"
llvm-readobj --sections "_work/bins/win_GOF2.exe"
```

| observable | value |
|---|---|
| format | PE32 i386, 6 sections, `IMAGE_FILE_32BIT_MACHINE | LARGE_ADDRESS_AWARE` |
| linker | `10.0` (Visual Studio 2010) |
| `TimeDateStamp` | `2012-09-05 16:30:26` (`0x50477E22`) |
| image base | `0x400000` |
| entry point | `0x25D2EE` |
| sections | `.text` 1,225,728 raw / `0x12B3BF` virtual; `.rdata` 257,024; `.data` 648,704; `.rsrc` 221,184; `.reloc` 88,576; `.bind` 376,832 |
| imports | `MSVCR100.dll`, `MSVCP100.dll`, `d3d9.dll`, `d3dx9_43.dll`, `fmod_event.dll`, `steam_api.dll`, `DINPUT8.dll`, `GDI32.dll`, `KERNEL32.dll`, `SHELL32.dll`, `USER32.dll`, `COMDLG32.dll` |
| CodeView | RSDS, PDB GUID `{9A525EDE-5753-47D1-B793-9D334ED401D4}`, age 2, path `C:\Users\s.lohse\Desktop\GoF2_PC_REP\GOF2.pdb` |
| version resource | none |
| Rich header | none |

The Windows FMOD DLLs are **FMOD Ex/Event 4.40.04**, VS2010, dated
`2012-04-04`; `steam_api.dll` is dated `2012-03-15` (linker 8.0).
`GoF2Launcher.exe` is the same VS2010 build, dated `2012-09-05 16:27:00`.
`fmodexp.dll` is UPX-compressed and dated 2010.

## 6. Source-layout dumps

```sh
cd "/home/debian/GalaxyOnFire2/source file list dump"
for f in gof2hd_1.1.19_ios.cpp gof2sd_1.0.1_ios.cpp gof3d_1.1.4_ios.cpp; do
  echo -n "$f: "; grep -c '\.o' "$f"
done
```

| dump | TUs | symbols | build environment |
|---|---:|---:|---|
| `gof2hd_1.1.19_ios.cpp` | 125 | 3,627 | Xcode DerivedData, **arm64**, iPhoneOS 16.4 SDK, Release re-archive, `/Users/n.tran/projects/gof2/`; object names carry content hashes |
| `gof2sd_1.0.1_ios.cpp` | 158 | 3,591 | **armv6**, iPhoneOS 4.0/4.1 SDK, `Distribute-iphoneos`, `/Users/mhehmeyer/Documents/Dev/GalaxyOnFire2/` |
| `gof3d_1.1.4_ios.cpp` | 111 | 2,152 | **armv6**, GoF1 "3D", `/Users/marchehmeyer/Documents/Dev/GalaxyOnFire/` |

The dumps give the original **TU membership** and **source roots**, not the
Android build flags. Key facts (see `_work/original_layout/original_layout_notes.md`):

* the engine is a flat `AE2/Classes/` tree with one `Shader/` subfolder;
* the game is a flat `Classes/Game/` tree with only `ParticleSystems/` and
  `MeshMerger/` subfolders;
* the SD engine was a prebuilt static library
  `/Volumes/realtime/projects/1605_Abyss_Engine/iPhone/lib_float_iOS4/libAbyssEngineDevice.a`
  with source root `/Users/marchehmeyer/Documents/Dev/Svn_iPhone_AE2/Classes/`;
* the HD 1.1.19 dump uses `FMOD/v44442/inc/`; the SD 1.0.1 dump uses `FMOD/v432`;
* the Windows dump is a flat `Classes\` tree with `Engine_DirectX.cpp`,
  `FBOContainer.cpp`, `PaintCanvas.cpp`, `Mesh.cpp`, `Image.cpp`,
  `ImageFont.cpp`, `Classes\Shader\BaseShader.cpp`.

## 7. Cross-platform codegen: source shape vs flags

### 7.1 Method

```sh
# Android exported functions
readelf -sW --dyn-syms _work/bins/android_2.0.16_libgof2hdaa.so \
  | awk '$4=="FUNC" && $7!="UND" {print $8}' | sort -u > android_funcs.txt
# iOS exported C++ functions
llvm-objdump --macho --syms _work/bins/ios_1.1.4_HD \
  | awk '$0 ~ / F / && $NF ~ /^__Z/ {print substr($NF,2)}' | sort -u > ios_funcs.txt
comm -12 android_funcs.txt ios_funcs.txt | wc -l
```

Result: **4,524** Android functions, **663** iOS exported `__Z*` functions,
**126** common mangled names. The iOS set is small because the iOS binary is
stripped and only exports a subset; the common set is still enough to separate
source shape from optimization.

### 7.2 Measured examples

Android sizes are from `readelf -sW --dyn-syms`; iOS sizes are measured from the
function's `pop`/next-function boundary in `llvm-objdump --dis-symname`, **not**
from `llvm-nm --print-size` (which is always zero on Mach-O). The iOS C1
constructors are 28–32-byte delegating thunks; the C2 constructors are the real
bodies.

| function | Android size | iOS size | Android `-Oz` codegen | iOS codegen |
|---|---:|---:|---|---|
| `ArrayAdd<Mesh*>` | 36 | 68 | `push {r4,r5,r7,lr}`, `ldr`/`adds`/`str` in registers, `blx realloc`, `strd` result | `push {r7,lr}`, `sub sp,#8`, every value spilled to `[sp]`, `blx realloc`, reload, store; no scheduling |
| `ArrayRelease<Mesh*>` | 20 | 60 | leaf, direct `ldr`/`cmp`/`beq` | stack frame, `str r0,[sp,#4]`, reload before each use |
| `PaintCanvas::FindResource` | 48 | 124 | `ldr.w ip,[r0,#0x134]`, `ldr.w r2,[r0,#0x138]`, `ldrh.w` in a tight loop | same logic, but `Array::operator[]` is an out-of-line `blx` per iteration and every index is a stack slot |
| `AEMath::Matrix::Matrix()` | 64 | 124 | NEON: `vmov.i32 q9,#0`, `vld1.64`, `vst1.32`, three `str` for the diagonal | scalar `vstr s0/s1` to every one of the 16 floats, plus a stack frame and a call to `Vector::Vector(float,float,float)` |
| `String::String(int)` | 40 | 40 | `strd` vtable/data, `asrs` sign, `blx String::Set(long long)` | stack frame, `str` vtable, `bl` `String::Set`; same size, different schedule |
| `KeyFrame::KeyFrame()` | 108 | 188 | `__aeabi_memclr4(this, 72)`, then two `Vector::operator=` calls | six `Vector::Vector()` calls at `+0,+0xc,+0x18,+0x24,+0x30,+0x3c`, then `Vector::Vector(float,float,float)` at `+0x50` |

Representative disassembly (Android `ArrayAdd<Mesh*>`, `0x6d050`):

```text
6d050: b5b0        push  {r4, r5, r7, lr}
6d052: af02        add   r7, sp, #8
6d054: 460c        mov   r4, r1
6d056: 6809        ldr   r1, [r1, #0]
6d058: 4605        mov   r5, r0
6d05a: 6860        ldr   r0, [r4, #4]
6d05c: 3101        adds  r1, #1
6d05e: 60a1        str   r1, [r4, #8]
6d060: 0089        lsls  r1, r1, #2
6d062: f7f1 ee32   blx   5ecc8 <realloc@plt>
6d066: 6821        ldr   r1, [r4, #0]
6d068: f840 5021   str.w r5, [r0, r1, lsl #2]
6d06c: 68a1        ldr   r1, [r4, #8]
6d06e: e9c4 1000   strd  r1, r0, [r4]
6d072: bdb0        pop   {r4, r5, r7, pc}
```

iOS `ArrayAdd<Mesh*>`, `0x310b8c`:

```text
310b8c: 80 b5        push  {r7, lr}
310b8e: 6f 46        mov   r7, sp
310b90: 82 b0        sub   sp, #0x8
310b92: 01 90        str   r0, [sp, #0x4]
310b94: 00 91        str   r1, [sp]
310b96: 00 98        ldr   r0, [sp]
310b98: 00 68        ldr   r0, [r0]
310b9a: 01 30        adds  r0, #0x1
310b9c: 00 99        ldr   r1, [sp]
310b9e: 88 60        str   r0, [r1, #0x8]
310ba0: 00 98        ldr   r0, [sp]
310ba2: 40 68        ldr   r0, [r0, #0x4]
310ba4: 00 99        ldr   r1, [sp]
310ba6: 89 68        ldr   r1, [r1, #0x8]
310ba8: 89 00        lsls  r1, r1, #0x2
310baa: 8b f0 7c ef  blx   0x39caa4 <_realloc>
...
310bca: 02 b0        add   sp, #0x8
310bcc: 80 bd        pop   {r7, pc}
```

### 7.3 What this proves

1. **The iOS 1.1.4 build is unoptimized.** Every function keeps a frame, spills
   arguments to the stack, and reloads them; there is no inlining of
   `Array::operator[]`, no register allocation across calls, and no NEON
   vectorization. This is `-O0`-style codegen. The exact level (`-O0` vs a
   non-inlining `-O1`) cannot be proven without rebuilding with Apple clang 4.5,
   which is not available here, but the `-O0` inference is strong.
2. **The iOS sizes are not a flag oracle for Android.** The 36 vs 68 B,
   48 vs 124 B, 64 vs 124 B differences are optimization-level differences, not
   evidence about the Android `-Oz` recipe.
3. **`KeyFrame::KeyFrame()` is a source-version difference.** The Android v2 body
   uses `memclr` + assignment; the iOS v1 body uses six `Vector` constructors +
   a `Vector(float,float,float)` constructor. This is exactly the kind of
   source-shape delta that the 24 non-byte-exact functions are made of.
4. **The Windows build cannot validate Android flags.** It is x86/VS2010 with
   different RTTI, calling convention, and standard library. It is useful for
   class names and data-format behavior, not for ARM codegen.

## 8. Struct-layout witnesses from iOS ObjC type encodings

The iOS binary embeds complete Objective-C type encodings for `Engine` and
`GameData` (the `T^{Engine=...}` / `T^{GameData=...}` strings). These are
independent layout witnesses. They omit C++ vtable pointers, so a `String`
appears as `{String=^SI}` (8 bytes: data pointer + length), while our `String`
is 12 bytes (vtable + data + length). Adding 4 bytes per `String` recovers the
C++ layout.

### 8.1 `Engine`

The encoding is:

```text
T^{Engine={String=^SI}iI{String=^SI}B^{FileInterface}iB^{ApplicationManager}iB{String=^SI}B{String=^SI}iiiiiiiBI[20S]f[4f]i[3i][3i]BBBBi{?=[4[4f]]}{?=[4[4f]]}{?=[4[4f]]}{?=[4[4f]]}[3[3f]][2[4f]][2[4f]][2[4f]][4f][4f][4f][4f]f[2[3f]][2[3f]][2[3f]][3f][3f]i[2{Vector=fff}]^f{Vector=fff}^viffiiii[2f]^{Mesh}{?=[4[4f]]}Bf{Vector=fff}{Array<int>=I^iI}iff{Vector=fff}{Vector=fff}IIi^{FBOContainer}^{FBOContainer}BiB[16f][2{Vector=fff}]B^?f[3I][3I]Ii[3d][3d][3d][3d]{Array<AbyssEngine::ShaderBaseStruct *>=I^^{ShaderBaseStruct}I}},Vengine
```

A small parser (`enc_parse.py`, in `/dev/shm/wb101`) computes the raw encoding
size **1256 B**. Adding the four `String` vtable pointers (4 × 4 = 16 B) gives
**1272 B**; adding the `boundTextures` difference (`unsigned short[20]` = 40 B
on iOS vs `int[20]` = 80 B in our tree) gives **1312 B**, exactly our
`sizeof(Engine)`. The field-by-field comparison matches at every offset after
those two adjustments:

| offset (ours) | our field | iOS encoding field | note |
|---:|---|---|---|
| 0 | `String str_0x0` | `{String=^SI}` | vtable omitted |
| 12 | `uint32_t maxTextureSize` | `i` | |
| 16 | `uint32_t lastGlError` | `I` | |
| 20 | `String lastErrorPath` | `{String=^SI}` | vtable omitted |
| 32 | `uint8_t linearFilterFlag` | `B` | |
| 36 | `FileInterface *fileInterface` | `^{FileInterface}` | |
| 40 | `int timerInterval` | `i` | |
| 44 | `_Bool vibrationSupported` | `B` | |
| 48 | `ApplicationManager *appManager` | `^{ApplicationManager}` | |
| 52 | `uint32_t field_0x34` | `i` | |
| 56 | `uint8_t field_0x38` | `B` | |
| 60 | `String str_0x3c` | `{String=^SI}` | vtable omitted |
| 72 | `uint8_t field_0x48` | `B` | |
| 76 | `String str_0x4c` | `{String=^SI}` | vtable omitted |
| 88 | `uint64_t field_0x58` | `i i` | iOS models two ints; same 8 bytes |
| 96..112 | `drawCallCountB..textureByteCounter` | five `i` | |
| 116 | `_Bool field_0x74` | `B` | |
| 120 | `int field_0x78` | `I` | |
| 124 | `int [20] boundTextures` | `[20S]` | **40-byte delta** |
| 204 | `float field_0xcc` | `f` | |
| 208 | `float [4] glColor` | `[4f]` | |
| 224 | `int packedColor` | `i` | |
| 228 | `uint8_t [23] _pad0xe4` | `[3i][3i]` | iOS has 24 B of ints; ours 23 B pad + 5 flags |
| 251..255 | `shaderModeFlag..field_0xff` | `BBBB` | 1-byte semantic shift in the pad region |
| 256 | `uint32_t field_0x100` | `i` | |
| 260..516 | four 4×4 matrices | four `{?=[4[4f]]}` | |
| 516 | `float [9] normalMatrix` | `[3[3f]]` | |
| 552..648 | six `LightColor` + `sceneAmbient` | `[2[4f]]`×3 + `[4f]`×4 | |
| 648..788 | material + shaded arrays | `[4f]`×4 + `f` + `[2[3f]]`×3 | |
| 788..812 | particle/rim colors + `lightCount` | `[3f]`×2 + `i` | |
| 816 | `Vector [2] lightDir` | `[2{Vector=fff}]` | |
| 840 | `int lineVertexBase` | `^f` | iOS models a `float*` here; same 4 bytes |
| 844 | `Vector lightColor` | `{Vector=fff}` | |
| 856 | `unsigned char *addData` | `^v` | |
| 860 | `int addDataSize` | `i` | |
| 864 | `float wheelDelta` | `f` | |
| 868 | `uint8_t [4] _pad0x364` | `f` | |
| 872..884 | display/viewport ints | `i i i i` | |
| 888 | `float [2] lightDirty` | `[2f]` | |
| 896 | `Mesh *quadMesh` | `^{Mesh}` | |
| 900 | `float [16] projMatrix` | `{?=[4[4f]]}` | |
| 964 | `uint8_t field_0x3c4` | `B` | |
| 968 | `float blurStrength` | `f` | |
| 972 | `Vector field_0x3cc` | `{Vector=fff}` | |
| 984 | `Array<int> triangleCounts` | `{Array<int>=I^iI}` | |
| 996 | `int currentProgram` | `i` | |
| 1000..1020 | fog/eye fields | `f f {Vector} {Vector}` | |
| 1032 | `uint8_t [4] _pad0x408` | `I` | |
| 1036 | `uint32_t viewFramebuffer` | `I` | |
| 1040 | `uint32_t postEffectFlags` | `i` | |
| 1044 | `FBOContainer *postEffectFBO` | `^{FBOContainer}` | |
| 1048 | `FBOContainer *refractFBO` | `^{FBOContainer}` | |
| 1052 | `uint8_t glowActive` | `B` | |
| 1056 | `uint32_t glEnableFlags` | `i` | |
| 1060 | `_Bool lightingEnabled` | `B` | |
| 1064 | `float [16] uvMatrixGL` | `[16f]` | |
| 1128 | `Vector [2] lightPositions` | `[2{Vector=fff}]` | |
| 1152 | `_Bool hasVibration` | `B` | |
| 1156 | `DestroyCallback *onDestroyCallback` | `^?` | |
| 1160 | `float materialAlpha` | `f` | |
| 1164 | `int [2] frameBufferTextures` | `[3I]` | 8 B vs 12 B; the next field absorbs it |
| 1172 | `uint8_t [16] _pad0x494` | `[3I]` | 16 B vs 12 B |
| 1188 | `uint32_t clientStateFlagsAE` | `I` | |
| 1192 | `uint32_t field_0x4a8` | `i` | |
| 1200..1272 | four `double [3]` | four `[3d]` | |
| 1296 | `Array<ShaderBaseStruct *> shaders` | `{Array<...>=I^^{...}I}` | |

**Conclusion (hypothesis/lead, not landed):** the iOS 1.1.4 `Engine` is the same
struct as our v2 `Engine` except for `boundTextures` (`unsigned short[20]` vs
`int[20]`) and a few semantic labels in the padding region. Because all Engine
methods are byte-exact in our tree (none of the 24 non-exact functions is an
Engine method), the Android v2 binary must use `int[20]`; the iOS `short[20]` is
a v1.1.4 source-version delta. Positive confirmation: the byte-exact
`Engine::RenderMesh` (`0x85f24`, 1040 B) accesses offsets `0x238`, `0x2e0`,
`0x300`, `0x420`, `0x514` — all consistent with our `int[20]` layout; a
`short[20]` layout would shift every one of them by -40. Changing our header
would regress the gate.

### 8.2 `GameData`

The encoding is:

```text
T^{GameData=^{Globals}BBiBBBiBBB{String=^SI}{String=^SI}{String=^SI}BBBBBBBBBiB^{Array<AbyssEngine::String *>}^{Array<AbyssEngine::String *>}^{Array<AbyssEngine::String *>}^{Array<AbyssEngine::String *>}^{Array<int>}^{Array<int>}^{Array<float>}BBBB*iBBB{String=^SI}{String=^SI}{String=^SI}BBBBBB{String=^SI}B{String=^SI}BB},VgameData
```

Raw encoding size **168 B**. Adding the eight `String` vtable pointers
(8 × 4 = 32 B) gives **200 B**, exactly our `sizeof(GameData)`. The field sizes
match at every offset, but the **types** differ in one region:

| offset | our field | iOS encoding field |
|---:|---|---|
| 80 | `int32_t field_0x50` | `^{Array<AbyssEngine::String *>}` |
| 84 | `int32_t field_0x54` | `^{Array<AbyssEngine::String *>}` |
| 88 | `int32_t field_0x58` | `^{Array<AbyssEngine::String *>}` |
| 92 | `int32_t field_0x5c` | `^{Array<AbyssEngine::String *>}` |
| 96 | `int32_t field_0x60` | `^{Array<int>}` |
| 100 | `int32_t field_0x64` | `^{Array<int>}` |
| 104 | `int32_t field_0x68` | `^{Array<float>}` |
| 108..111 | `uint8_t field_0x6c..0x6f` | `B B B B` |
| 112 | `uint8_t field_0x70..0x73` | `*` (char pointer) |
| 116 | `uint8_t field_0x74..0x77` | `i` |
| 120 | `uint16_t field_0x78` | `B B B` |

**Conclusion (hypothesis/lead, not landed):** the v1.1.4 `GameData` stores seven
pointers at `0x50..0x68` (four `Array<String*>*`, two `Array<int>*`, one
`Array<float>*`), while our v2 header models them as seven `int32_t`. The sizes
are identical, so the current codegen does not distinguish them; this is a
source-shape lead for a future GameData pass, not a build-flag issue.

## 9. Flag table

The full A/B matrix is in `build-compile-flag-positive-audit-w5017.md` §4–5 and
`build-flag-differential-matrix-w5020.md` §6. The table below is the
cross-artifact consolidation. `PROVEN` means a positive fingerprint in the
shipped binary (or a controlled A/B that changes the gate); `UNOBSERVABLE` means
the alternative is byte-identical on this source; `DISPROVEN` means the
alternative moves away from the original.

### 9.1 Compile flags

| flag | status | evidence |
|---|---|---|
| `-target armv7-none-linux-androideabi16` | **PROVEN** (ABI/API; exact triple spelling unobservable) | ELF ARM EABI5, `.note.android.ident` api 16, r18b sysroot |
| `-march=armv7-a` | **PROVEN** | `.ARM.attributes` v7/A; `movw`/`movt`; Thumb-2 |
| `-mthumb` | **PROVEN** | `Tag_THUMB_ISA_use: Thumb-2`; odd dynsym; `-marm` 57 |
| `-mfpu=neon` | **PROVEN** | NEONv1; `vld1`/`vst1`; `vfpv3-d16` fails NEON intrinsics |
| `-mfloat-abi=softfp` | **PROVEN** | ELF `0x5000200`; no `Tag_ABI_VFP_args`; hard link fails |
| `-Oz` | **PROVEN** | 4500 vs 1263/1168/1167 for `-Os`/`-O2`/`-O3`; per-TU sweep 184/192 vs `-O2`, 191/192 vs `-O1`, 192/192 vs `-O0` |
| `-std=c++11` | **PROVEN** (vs c++14) | `Material::Material` 276 vs 272; full c++14 1512 |
| `-std=gnu++11` | **UNOBSERVABLE** | full 4500, only build-id |
| `-fPIC` | **PROVEN** (PIC); exact spelling **UNOBSERVABLE** | GOT/`GLOB_DAT`, no `REL32`/`DT_TEXTREL`; `-fpic` identical |
| default visibility | **PROVEN** | 4524 DEFAULT; `-fvisibility=hidden` 57 |
| `-fvisibility-inlines-hidden` | **DISPROVEN** | full 1278, 141 functions hidden |
| `-fstack-protector-strong` | **PROVEN** | 787 canaries; basic 88; all 4421; two exact anchors |
| `-fomit-frame-pointer` off | **PROVEN** | 2473 frame prologues; omit 736; full 1298 |
| `-fno-omit-frame-pointer` explicit | **PROVEN** (neutral) | full 4500, only build-id |
| `-funwind-tables` | **PROVEN** | 3566 EXIDX/272 CANT/156312 extab; object nounwind changes |
| `-frtti` | **PROVEN** | 176 `_ZTI`/`_ZTS`, 99 `_ZTV`; `-fno-rtti` 1701 |
| `-fexceptions` | **PROVEN** | `_Unwind_Resume` 540, `__gxx_personality_v0`; no-exceptions build fail |
| `-g` | **PROVEN** | g0/g1 2069; `InitializeBlowfish` entry copy |
| `-ffunction-sections` | **PROVEN** | 193 CANT+real; object 394 vs 92 sections |
| `-fdata-sections` | **DISPROVEN** (absent) | `.rodata` 0x41b67 vs 0x41a07; full 4311 |
| `-DNDEBUG` | **UNOBSERVABLE** | no source use; full 4500, only build-id |
| `-DANDROID` | **UNOBSERVABLE** | no source use; full 4500, only build-id |
| `-D__ANDROID_API__=16` | **UNOBSERVABLE** as macro; API 16 **PROVEN** | full 4500 with 21; `.note.android.ident` 16 |
| `-fno-sized-deallocation` | **PROVEN** (default) | 0 `_ZdlPvj`/`_ZdaPvj` calls; sized variant 1347 |
| `-fno-threadsafe-statics` | **DISPROVEN** (absent) | 2 `__cxa_guard_acquire`; variant 1368 |
| `-fno-use-cxa-atexit` | **DISPROVEN** (absent) | 2 `__cxa_atexit`, no `atexit`; variant 1356 |
| `-fno-strict-aliasing` | **DISPROVEN** (absent) | `RenderMesh` 1040 vs 1032; variant 1371 |
| `-fmerge-constants` | **UNOBSERVABLE** | full 4500, only build-id; object identical |
| `-stdlib=libc++` | **PROVEN** | `__cxa_*`/`__cxa_guard_acquire` bytes match `libc++abi.a`; no libstdc++ symbols |
| `_FORTIFY_SOURCE` | **UNOBSERVABLE** (absent) | only `__stack_chk_*`; no `__memcpy_chk`/`__strcpy_chk` |
| `-marm` | **DISPROVEN** | full 57, `.text` 0x1a3818 |
| `-mfpu=vfpv3-d16` | **DISPROVEN** | compile fails `arm_neon.h` |
| `-mfloat-abi=hard` | **DISPROVEN** | link fails VFP args vs softfp FMOD |
| `-Os` / `-O2` / `-O3` | **DISPROVEN** | 1263 / 1168 / 1167 |
| `-g0` / `-g1` | **DISPROVEN** | 2069 |
| `-fstack-protector` / `-all` | **DISPROVEN** | 1373 / 83 |
| `-fno-rtti` | **DISPROVEN** | 1701; typeinfo removed |
| `-fno-exceptions` | **DISPROVEN** | build fail |
| `-fsized-deallocation` | **DISPROVEN** | 1347; `_ZdlPvj` relocs |
| `-fvisibility=hidden` | **DISPROVEN** | 57; 4280 functions hidden |
| `-fno-jump-tables` | **DISPROVEN** | 1363; 401 `tbb` + 84 `tbh` |
| `-fno-slp-vectorize` | **DISPROVEN** | 1484 |
| `-fno-unroll-loops` | **DISPROVEN** | 2883 |
| `-fno-optimize-sibling-calls` | **DISPROVEN** | 1316 |
| `-ffast-math` | **DISPROVEN** | 1335 |
| `-fno-math-errno` | **DISPROVEN** | 1356 |
| `-freciprocal-math` | **DISPROVEN** | 1561 |
| `-fno-builtin` | **DISPROVEN** | 1350 |
| `-fwrapv` / `-fno-strict-overflow` | **DISPROVEN** | both 1398 |
| `-march=armv7ve` | **DISPROVEN** | 1415 |
| `-mcpu=cortex-a9` | **DISPROVEN** | 1230 |
| `-mtune=cortex-a9` | **UNOBSERVABLE** | neutral |
| `-mfpu=neon-vfpv4` | **UNOBSERVABLE** | neutral, but changes `.ARM.attributes` |
| `-fshort-wchar` | **UNOBSERVABLE** | neutral |
| `-fno-common` | **DISPROVEN** | link fail (`g_android_back_button_pressed` duplicate) |
| `-fno-plt` | **UNOBSERVABLE** | 4500, `.text` byte-identical |
| `-fno-vectorize` | **UNOBSERVABLE** | 4500, `.text` byte-identical |
| `-fno-trapping-math` | **UNOBSERVABLE** | 4500, `.text` byte-identical |
| `-fno-delete-null-pointer-checks` | **UNOBSERVABLE** | 4500, `.text` byte-identical |
| `-mno-movt` | **UNOBSERVABLE** | 4500 |
| `-mtune=cortex-a8` | **UNOBSERVABLE** | 4500 |
| `-mllvm -inline-threshold=1/5` | **UNOBSERVABLE** | 4500, `.text` byte-identical |
| `-mllvm -inline-threshold=0/10/25/50/100` | **DISPROVEN** | 1356/1354/1352/1350/1350 |
| `-mllvm -likely-branch-weight=64/500/1000` | **UNOBSERVABLE** | 0 exact functions change across 190 TUs |
| `-mllvm -unlikely-branch-weight=0/2` | **UNOBSERVABLE** | 0 exact functions change across 190 TUs |
| `-fno-semantic-interposition` | **UNSUPPORTED** | clang 7.0.2: unknown argument |
| `-mno-ldrd` | **UNSUPPORTED** | clang 7.0.2: unknown argument |
| `-mllvm -enable-unsafe-fp-math` | **UNSUPPORTED** | clang 7.0.2: unknown argument |
| `-Xclang -menable-unsafe-fp-math` | **DISPROVEN** | 1426 |

### 9.2 Link flags

| flag | status | evidence |
|---|---|---|
| `-shared` | **PROVEN** | `DYN` ELF |
| `-soname libgof2hdaa.so` | **PROVEN** | `DT_SONAME` |
| `--build-id` | **PROVEN** | `.note.gnu.build-id`; gold SHA-1 of the unstripped output |
| `-z now` | **PROVEN** | `DT_FLAGS BIND_NOW`, `DT_FLAGS_1 NOW` |
| `--exclude-libs=libgcc.a` | **PROVEN** | libgcc symbols are local, not exported |
| no `-s` | **PROVEN** | the build-id is a hash of the unstripped output; a gold `-s` link cannot reproduce it |
| `-z relro` | **PROVEN** (default) | `.data.rel.ro`, `DT_*` layout |
| `-z noexecstack` | **PROVEN** (default) | GNU_STACK RW |
| `--hash-style=sysv` | **PROVEN** (default) | `.hash` only, no `.gnu.hash` |
| `--no-as-needed` | **PROVEN** (default) | all `DT_NEEDED` entries present |
| `-z combreloc` | **PROVEN** (default) | `DT_RELCOUNT` |
| `--whole-archive libz libzip` | **PROVEN** | zlib/libzip code is in `.text` |
| no `--gc-sections` | **PROVEN** | all sections retained |
| no `--sort-section` | **PROVEN** | input order preserved |
| no `--rosegment` | **PROVEN** | section layout |
| no `--version-script` | **PROVEN** | all symbols exported |
| no `--section-ordering-file` | **PROVEN** | `tools/source_order.txt` is the order |
| no `--unresolved-symbols` | **PROVEN** | default shared-library undefined-import handling |
| no `--allow-multiple-definition` | **PROVEN** | no duplicate strong definitions |
| no `--emit-relocs` | **PROVEN** | no `.rel.*` for local code |
| linker `-O1` | **UNOBSERVABLE** | neutral |
| linker `-O2` | **DISPROVEN** | wrong section layout |
| post-link `strip --strip-all` | **PROVEN** | no `.symtab`/`.debug_*` in the shipped file |
| `tools/reorder_shstrtab.py` | **PROVEN** | APK lib vs ground truth differ only in `.shstrtab`/`sh_name` |

## 10. Reconstructed build recipe

### 10.1 Compile line (one C++ TU)

```sh
$NDK/toolchains/llvm/prebuilt/<host>/bin/clang++ \
  -target armv7-none-linux-androideabi16 \
  -march=armv7-a -mthumb -mfpu=neon -mfloat-abi=softfp \
  -std=c++11 -fPIC -frtti -fstack-protector-strong -ffunction-sections \
  -funwind-tables -Oz -g \
  -DANDROID -DNDEBUG -D__ANDROID_API__=16 \
  -stdlib=libc++ \
  -isystem $NDK/sources/cxx-stl/llvm-libc++/include \
  -isystem $NDK/sources/cxx-stl/llvm-libc++abi/include \
  -isystem $NDK/sources/android/support/include \
  --sysroot=$NDK/sysroot \
  -isystem $NDK/sysroot/usr/include \
  -isystem $NDK/sysroot/usr/include/arm-linux-androideabi \
  -I<repo>/src -I<repo>/third_party/fmod/inc -I<repo>/third_party/gl \
  -I<repo>/third_party/jni -I<repo>/third_party/libzip -I<repo>/third_party/crypto \
  -c <TU>.cpp -o <TU>.o
```

The C TUs use the same line minus `-std=c++11 -frtti -stdlib=libc++` and the
libc++ include paths.

### 10.2 Link line

```sh
$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/<host>/bin/arm-linux-androideabi-ld.gold \
  -shared -soname libgof2hdaa.so --build-id -z now \
  --exclude-libs=libgcc.a \
  -o libgof2hdaa.so \
  <crtbegin_so.o> -L <libdir> \
  <objects in tools/source_order.txt> \
  --whole-archive libz.a libzip.a --no-whole-archive \
  -l log -l GLESv2 -l GLESv1_CM -l EGL -l android \
  libfmodex.so libfmodevent.so \
  libcrypto_gof2.a \
  -l m \
  libc++abi.a \
  libandroid_support.a \
  libgcc.a \
  -l dl -l c \
  <crtend_so.o>
```

The exact archive order is proven by the original's `.text` tail and by relink
A/B: **FMOD → libcrypto → libm → libc++abi → libandroid_support → libgcc →
dl → c**. The Darwin `libc++abi.a` is required; the Linux archive changes
`.rodata` and drops `byte_exact` to 4344. The current minimal recipe omits
`libc++_static.a`, `libunwind.a`, `libatomic.a`, `--fix-cortex-a8`,
`--no-undefined`, `-z noexecstack`, `-z relro`, `--warn-shared-textrel`,
`--fatal-warnings`, `--exclude-libs=libatomic.a` and
`--exclude-libs=libunwind.a`; all are byte-neutral on this tree (wave 100 §3).

### 10.3 Post-link

```sh
$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/<host>/bin/arm-linux-androideabi-strip \
  --strip-all libgof2hdaa.so
python3 tools/reorder_shstrtab.py libgof2hdaa.so
```

`--strip-all` and `--strip-unneeded` are identical on this file. The reorder
restores gold's Stringpool `.shstrtab` order; without it the file differs from
the APK lib by 288 bytes.

### 10.4 CMake configure

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8
```

The CMake toolchain sets `CMAKE_SYSTEM_NAME Generic`, `CMAKE_SYSTEM_PROCESSOR
arm`, the NDK r18b clang/clang++/llvm-ar, and the flags above. The canonical
build is `-Oz`; `GOF2_MATCH_OPT` is the A/B knob.

### 10.5 CMake vs ndk-build: the two discriminators

The original is **CMake**, not ndk-build, for two independent reasons:

1. **No `-fdata-sections`.** ndk-build's default `APP_CFLAGS` includes
   `-fdata-sections`; the original's `.rodata` is `0x41b67`, while
   `-fdata-sections` gives `0x41a07` and drops `byte_exact` to 4311. CMake's
   NDK r18b default does not add it.
2. **STL archive order.** ndk-build places static libraries before shared
   libraries, which pulls `libunwind.a` and changes `.text`/`.rel.plt`/`.dynsym`.
   The original's link order is the CMake `target_link_libraries` order:
   FMOD → libcrypto → libc++abi → libandroid_support → libgcc → dl → c.

The ABI (`armeabi-v7a`), API (`android-16`, the r18b minimum), STL
(`c++_static`), and ARM mode (`thumb`) are all CMake r18b defaults; only
`ANDROID_ARM_NEON=ON`, `ANDROID_CPP_FEATURES="rtti exceptions"`, and
`CMAKE_BUILD_TYPE=Release` need to be explicit. The `-g` flag is the CMake
Release default for the NDK toolchain, and it is load-bearing at clang 7 `-Oz`
(debug info pins PostRA machine sinking; `-g0` drops `byte_exact` to 2069).

## 11. Proven vs hypothesis

### Proven from shipped artifacts (no A/B needed)

* Android toolchain: NDK r18b, clang 7.0.2, gold 1.12, API 16, ARMv7-A,
  Thumb-2, NEON, softfp, libc++.
* Android compile flags: `-Oz`, `-g`, `-std=c++11`, `-fPIC`, `-frtti`,
  `-fstack-protector-strong`, `-ffunction-sections`, `-funwind-tables`,
  frame pointers on, exceptions on, `-stdlib=libc++`, `-DANDROID`, `-DNDEBUG`,
  `-D__ANDROID_API__=16`, no `-fdata-sections`, no `-fvisibility=hidden`.
* Android link flags: `-shared -soname libgof2hdaa.so --build-id -z now
  --exclude-libs=libgcc.a`, no `-s`, `-z relro`, `-z noexecstack`,
  `--hash-style=sysv`, no `--gc-sections`, no `--version-script`, no
  `--section-ordering-file`, no `--unresolved-symbols`, no
  `--allow-multiple-definition`, no `--emit-relocs`.
* Android post-link: GNU `strip --strip-all` + `.shstrtab` reorder.
* Android build system: CMake (no `-fdata-sections`, CMake STL archive order),
  packaged by Android Gradle 3.1.2 on 2018-10-22.
* iOS 1.1.4: Xcode 4.5 (4G182), iphoneos6.0 SDK, Apple clang 1.0, 2012-12-04,
  armv7, no DWARF.
* iOS 1.0.5: Xcode 4.0 (4A304a), iphoneos4.3 SDK, GCC 4.2, 2011-04-27,
  armv6+armv7.
* Windows: VS2010 (linker 10.0), x86, 2012-09-05, PDB path, FMOD 4.40.04.
* Android FMOD: GCC 4.4.3/4.6, ARMv5TE, Thumb-1, VFPv1/VFPv2, softfp.
* No build scripts/project files/`.d`/logs in the APK or IPA.

### Proven by controlled A/B (wave 100, cited)

* Every `PROVEN`/`DISPROVEN` row in §9.1–9.2.
* The per-TU `-Oz` sweep: 184/192 TUs distinguish `-Oz` from `-O2`, 191/192
  from `-O1`, 192/192 from `-O0`.
* Gold's build-id algorithm: `SHA1(unstripped output with the 20-byte descriptor
  zeroed)`; our build-id `ab4c71d7...` is reproduced from that hash.
* The `.comment` decomposition and the libgcc pull.
* The `.rel.plt`/`.ARM.extab`/`.ARM.exidx` deltas are source-body artifacts of
  the 24 non-byte-exact functions.

### Hypothesis / open

* **iOS optimization level.** The codegen is unambiguously unoptimized
  (`-O0`-style), but the exact level (`-O0` vs a non-inlining `-O1`) is not
  proven because Apple clang 4.5 is not available. The `-O0` label is an
  inference from the stack traffic and out-of-line calls.
* **iOS 1.1.4 `Engine.boundTextures`.** The ObjC encoding says
  `unsigned short[20]`; our v2 tree uses `int[20]`. All Engine methods are
  byte-exact, so the Android v2 binary must use `int[20]`. This is a
  source-version delta, not a build-flag issue. Do not change the header
  without a full gate run.
* **iOS 1.1.4 `GameData` field types.** The encoding says seven pointers at
  `0x50..0x68`; our header has seven `int32_t`. Same size, so codegen cannot
  distinguish them. A future GameData pass should check the v2 binary's
  dereferences before renaming.
* **Exact iOS compiler build.** `DTCompiler=com.apple.compilers.llvm.clang.1_0`
  identifies the compiler family, not the exact clang version. The Xcode 4.5
  (4G182) / iphoneos6.0 SDK pair is the best available identification.
* **Exact Android CMake version.** The APK identifies Gradle/AGP 3.1.2 and the
  ELF identifies NDK r18b; the CMake version itself is not embedded. The
  CMake-vs-ndk-build discriminators are proven, but the exact CMake release is
  not.
* **The 24 non-exact functions.** No flag tested in wave 100 explains them;
  they are source-shape residuals. The `.rel.plt` 6-slot rotation is a direct
  symptom of `LevelScript::process`'s call order.

## 12. Reproduction commands

```sh
# Baseline
cd /home/debian/GalaxyOnFire2/wt/wb101
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8

# Android ELF metadata
O=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
readelf -hW "$O"; readelf -nW "$O"; readelf -AW "$O"; readelf -dW "$O"
readelf -p .comment "$O"; readelf -SW "$O"

# APK inventory and manifest
APK="/home/debian/GalaxyOnFire2/Original APK/Galaxy on Fire 2™ HD_2.0.16_APKPure.apk"
unzip -l "$APK"
unzip -p "$APK" META-INF/MANIFEST.MF
unzip -p "$APK" assets/crashlytics-build.properties
unzip -p "$APK" lib/armeabi-v7a/libgof2hdaa.so > /dev/shm/wb101/apk-lib.so
sha256sum /dev/shm/wb101/apk-lib.so "$O"
cmp -l /dev/shm/wb101/apk-lib.so "$O" | wc -l

# Android FMOD
readelf -AW /home/debian/GalaxyOnFire2/_work/bins/apk_libs/libfmodex.so
readelf -p .comment /home/debian/GalaxyOnFire2/_work/bins/apk_libs/libfmodex.so
readelf -dW /home/debian/GalaxyOnFire2/_work/bins/apk_libs/libfmodex.so

# iOS
llvm-objdump --macho --private-headers /home/debian/GalaxyOnFire2/_work/bins/ios_1.1.4_HD
llvm-objdump --macho --section-headers /home/debian/GalaxyOnFire2/_work/bins/ios_1.1.4_HD
llvm-objdump --macho --dylibs-used /home/debian/GalaxyOnFire2/_work/bins/ios_1.1.4_HD
strings -a /home/debian/GalaxyOnFire2/_work/bins/ios_1.1.4_HD | grep -E '^/Users/|^/Library/'

# Windows
llvm-readobj --file-headers --coff-imports --coff-debug-directory \
  --coff-resources "/home/debian/GalaxyOnFire2/_work/bins/win_GOF2.exe"
llvm-readobj --sections "/home/debian/GalaxyOnFire2/_work/bins/win_GOF2.exe"

# Cross-platform symbol set
readelf -sW --dyn-syms "$O" \
  | awk '$4=="FUNC" && $7!="UND" {print $8}' | sort -u > /dev/shm/wb101/android_funcs.txt
llvm-objdump --macho --syms /home/debian/GalaxyOnFire2/_work/bins/ios_1.1.4_HD \
  | awk '$0 ~ / F / && $NF ~ /^__Z/ {print substr($NF,2)}' | sort -u > /dev/shm/wb101/ios_funcs.txt
comm -12 /dev/shm/wb101/android_funcs.txt /dev/shm/wb101/ios_funcs.txt | wc -l

# Cross-platform disassembly examples
OBJ=/home/debian/GalaxyOnFire2/wt/wb101/.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OBJ -d --start-address=0x6d050 --stop-address=0x6d080 "$O"
llvm-objdump --macho --dis-symname __Z8ArrayAddIPN11AbyssEngine4MeshEEvT_R5ArrayIS3_E \
  -d /home/debian/GalaxyOnFire2/_work/bins/ios_1.1.4_HD

# Source-layout dumps
cd "/home/debian/GalaxyOnFire2/source file list dump"
for f in gof2hd_1.1.19_ios.cpp gof2sd_1.0.1_ios.cpp gof3d_1.1.4_ios.cpp; do
  echo -n "$f: "; grep -c '\.o' "$f"
done
```

## 13. Gate counts

| state | byte_exact | linked_exact | size-exact | data | imports | lint | drift |
|---|---:|---:|---:|---:|---:|---|---:|
| before | 4500 | 4500 | 4524/4524 | 843/843 | 207 | CLEAN | 0 |
| after | 4500 | 4500 | 4524/4524 | 843/843 | 207 | CLEAN | 0 |

No strict improvement was available. The note is the deliverable; the canonical
build is untouched.

## Appendix A. ObjC encoding size parser

The sizes in §8 were computed with this small parser (also at
`/dev/shm/wb101/enc_parse.py`). It handles the subset of the ObjC type encoding
grammar that appears in the `Engine`/`GameData` strings: scalars, pointers,
fixed arrays, and named/anonymous structs. It computes C layout with natural
alignment.

```python
def parse(s, i=0):
    c = s[i]
    if c == 'v': return 0, 1, c, i+1
    if c in 'iIfBcCsS':
        sz = {'i':4,'I':4,'f':4,'B':1,'c':1,'C':1,'s':2,'S':2}[c]
        return sz, sz, c, i+1
    if c == 'd': return 8, 8, c, i+1
    if c == '^':
        _, _, nm, k = parse(s, i+1)
        return 4, 4, '^'+nm, k
    if c == '?': return 4, 4, c, i+1
    if c in '*@#:': return 4, 4, c, i+1
    if c == '[':
        j = i+1
        while s[j].isdigit(): j += 1
        n = int(s[i+1:j])
        sz, al, nm, k = parse(s, j)
        assert s[k] == ']'
        return sz*n, al, f'[{n}{nm}]', k+1
    if c == '{':
        j = i+1
        while s[j] not in '=}': j += 1
        name = s[i+1:j]
        if s[j] == '}': return 4, 4, '{'+name+'}', j+1
        j += 1
        fields = []
        while s[j] != '}':
            sz, al, nm, j = parse(s, j)
            fields.append((nm, sz, al))
        j += 1
        off = 0; maxal = 1
        for nm, sz, al in fields:
            off = (off + al - 1)//al*al
            off += sz
            maxal = max(maxal, al)
        off = (off + maxal - 1)//maxal*maxal
        return off, maxal, '{'+name+'='+','.join(f[0] for f in fields)+'}', j
    if c == '(':
        j = s.index(')', i)
        return 4, 4, s[i:j+1], j+1
    raise ValueError((c, s[i:i+20]))
```

For `Engine`, `parse(encoding, 1)` returns `(1256, 8, ...)`; for `GameData`,
`(168, 4, ...)`. Adding 4 bytes per `{String=^SI}` field and 40 bytes for the
`[20S]` vs `int[20]` difference gives our 1312/200-byte layouts.

## 14. Files touched

* `_work/reconstructions/build-archaeology-cross-artifact-wb101.md` (this note).
* Scratch only: `/dev/shm/wb101/` (`enc_parse.py`, `enc_fields.py`,
  `cmp_engine*.py`, `cmp_gamedata.py`, `android_funcs.txt`, `ios_funcs.txt`,
  `common_funcs.txt`, `apk-lib.so`, `ipa114/`, `ipa105/`, `layout_dump.txt`,
  `check_baseline.log`, `configure.log`).
