# Build provenance from external artifacts, wave 128 (w7640)

Status: research note, 2026-11-07. Worktree `/home/debian/GalaxyOnFire2/wt/w7641`,
branch `wave128-w7641`, base `4bbe88776` ("Generator::createMission pass:
amount/padA slot-form study (no source win)"). Scratch: `/dev/shm/w7641`.
**No `src/`, `tools/`, `CMakeLists.txt`, baseline or allowlist change is landed.**
This note is the only commit.

Gate at the base (unchanged; `report.json`):

```text
verify: byte 4506/4524  linked 4506/4524  size 4524/4524  avg 99.94
data:   exact 843/843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0  imports 207
symbol parity 5370/5370 (0 extra / 0 missing)
lint CLEAN ✓   drift 0
```

The full `check` target was re-run at this base after the note was written:
`byte 4506/4524`, `linked 4506/4524`, `data 843/843`, `imports 207`,
`RATCHET PASS`, `lint CLEAN ✓`, `drift 0`. The note changes no source.

Ground truth: `_work/bins/android_2.0.16_libgof2hdaa.so`, 2,187,328 B,
sha256 `d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483`.
APK: `Original APK/Galaxy on Fire 2™ HD_2.0.16_APKPure.apk`, 6,389,308 B,
sha256 `9cfe4484a475c9a31a336df9dea6408fbb129d88aa19083afe808ef6bca9eb19`,
143 ZIP entries.

---

## 0. Bottom line

1. **No original Android build file survives** in the APK, DEX, native libs,
   IPAs, Windows build, source-layout dumps, or the public web. This note
   confirms the wave-120 conclusion with fresh, exact evidence and adds the
   third-party library versions and toolchain mapping.
2. **The APK is a late-2018 Android Gradle 3.1.2 release build.** The Java-side
   dependency set is pinned: Crashlytics NDK **2.0.5.27**, Crashlytics
   **2.9.5.27**, Fabric **1.4.4.27**, Play Services **12.4.51**
   (`0xbdfcb8`), Billing **1.1**, Expansion Downloader **1.1**, Licensing
   **1.5**, support library **27.0.0-SNAPSHOT**, arch-lifecycle **26.1.0**.
3. **The bundled `libcrashlytics.so` is an NDK r13b/r14-era build**, not r18b:
   `.comment` is exactly `GCC: (GNU) 4.9.x 20150123 (prerelease)`,
   `.note.gnu.gold-version` is `gold 1.11`, `NEEDED` includes
   `libstdc++.so` (gnustl shared), and it has no `.note.android.ident`.
   The r13b and r14 GCC 4.9.x strings are identical and both use gold 1.11;
   the exact r13b-vs-r14 minor is not distinguishable from the stripped ELF.
   NDK r18b is clang 7.0.2 + gold 1.12 and is clearly excluded.
4. **The bundled FMOD libs are much older still**: `libfmodex.so` has 194
   `GCC: (GNU) 4.4.3` `.comment` entries plus one
   `GCC: (GNU) 4.6 20120106 (prerelease)`; `libfmodevent.so` has 56
   `GCC: (GNU) 4.4.3` entries. Both are ARMv5TE / Thumb-1 / VFPv1–VFPv2
   (`Tag_CPU_name: "5TE"`), i.e. built for the old `armeabi` ABI with
   NDK r4–r7-era GCC. They are FMOD's prebuilts, not Fishlabs code.
5. **The game lib remains the only r18b / clang 7.0.2 / gold 1.12 artifact**:
   `.comment` = clang 7.0.2 + GCC 4.9.x, `.note.android.ident` is
   byte-identical to the NDK r18b API-16 `crtbegin_so.o` note, and the only
   path-like string is the darwin libc++abi buildbot path.
6. **Cross-platform evidence gives the shared engine's build identity**:
   the iOS 1.1.4 HD binary contains
   `@(#)PROGRAM:AbyssEngineDevice  PROJECT:AbyssEngine-3` and four app
   source paths under `/Users/hck/Documents/Dev/GalaxyOnFire2_merged/`;
   the iOS 1.0.5 SD dump records the engine as a prebuilt
   `/Volumes/realtime/projects/1605_Abyss_Engine/iPhone/lib_float_iOS4/libAbyssEngineDevice.a`.
   The Android build statically compiles that engine into the `.so`.
7. **The Windows build is a separate MSVC 2010 project**: PE32 i386,
   linker 10.0, timestamp 2012-09-05 16:30:26, PDB
   `C:\Users\s.lohse\Desktop\GoF2_PC_REP\GOF2.pdb`; `.text` entropy is 8.000
   (packed), so no codegen comparison is possible. It confirms the shared
   `AbyssEngine` namespace and FMOD/D3DX use, not the Android build system.
8. **A second 2.0.16 APK in circulation is a different native build**:
   clang 3.8.275480, gold 1.11, `libstdc++.so` NEEDED, path
   `/Volumes/Android/buildbot/src/android/ndk-r14-release/...` (NDK r14), and
   a modern repack manifest (minSdk 19 / targetSdk 30 / compileSdk 23 /
   platformBuild 30/11). It is not the APKPure r18b build.
9. **The public web has no leaked Fishlabs source tree or build file.**
   GitHub code search, Sourcegraph, GitLab, Bitbucket, archive.org and
   general web search return only decompilations, mods, remakes and ports.
   `GalaxyOnFire2_merged`, `AbyssEngineDevice`, `libAbyssEngineDevice` and
   `libgof2hdaa` appear only in this project's own notes.
10. **No residual changes.** The external evidence is provenance only; it does
    not identify a missing compile/link flag or a different source shape for
    any of the 18 non-byte-exact functions.

---

## 1. APK metadata

### 1.1 `META-INF/MANIFEST.MF`

```sh
APK="/home/debian/GalaxyOnFire2/Original APK/Galaxy on Fire 2™ HD_2.0.16_APKPure.apk"
unzip -p "$APK" META-INF/MANIFEST.MF | head -5
```

```text
Manifest-Version: 1.0
Built-By: Generated-by-ADT
Created-By: Android Gradle 3.1.2
```

`Created-By: Android Gradle 3.1.2` pins the APK packaging tool, not the native
build system. `Built-By: Generated-by-ADT` is the legacy string emitted by the
Gradle `PackageApplication` task.

### 1.2 `assets/crashlytics-build.properties`

```sh
unzip -p "$APK" assets/crashlytics-build.properties
```

```text
#This file is automatically generated by Crashlytics to uniquely
#identify individual builds of your Android application.
#
#Do NOT modify, delete, or commit to source control!
#
#Mon Oct 22 10:18:17 CEST 2018
version_name=2.0.16
package_name=net.fishlabs.gof2hdallandroid2012
build_id=f527605b-309f-404d-8afc-b68027edc49e
version_code=47947032
app_name=GOF2
```

The APK packaging timestamp is **2018-10-22 10:18:17 CEST**. The native link
timestamp is not in the stripped `.so`; this is the closest witness.

### 1.3 `AndroidManifest.xml`

`aapt`/`apkanalyzer` are not installed in this environment; the binary AXML
was parsed directly (see §8). The relevant attributes are:

```text
package          = net.fishlabs.gof2hdallandroid2012
versionCode      = 47947032
versionName      = 2.0.16
minSdkVersion    = 16
targetSdkVersion = 27
uses-feature glEsVersion = 0x00020000 (GLES 2.0), required
supports-gl-texture = GL_OES_compressed_ETC1_RGB8_texture
```

The manifest string pool also contains the support-library meta-data values:

```text
android.arch.lifecycle.VERSION = 26.1.0
android.support.VERSION        = 27.0.0-SNAPSHOT
```

`27.0.0-SNAPSHOT` is a pre-release support library, consistent with a late-2018
build. The `com.google.android.gms.version` meta-data resolves through
`resources.arsc` to `0x00bdfcb8` = **12451000** (Play Services **12.4.51**).

There is no `android:debuggable`, no `android:extractNativeLibs` and no
`platformBuildVersionCode`/`platformBuildVersionName` attribute in the APKPure
manifest (unlike the repack in §4).

### 1.4 Signing certificate

```sh
unzip -p "$APK" META-INF/CERT.RSA > /tmp/cert.rsa
openssl pkcs7 -inform DER -in /tmp/cert.rsa -print_certs -text -noout
```

```text
Subject/Issuer: O=FISHLABS Entertainment GmbH
Serial: 1278067031 (0x4c2dc157)
Signature Algorithm: sha1WithRSAEncryption
Validity: Jul 2 10:37:11 2010 GMT .. Jun 19 10:37:11 2060 GMT
```

The certificate is the original Fishlabs 2010 release key. It says nothing
about the native build.

### 1.5 APK vs ground truth

```sh
unzip -p "$APK" lib/armeabi-v7a/libgof2hdaa.so > /tmp/apk_lib.so
sha256sum /tmp/apk_lib.so _work/bins/android_2.0.16_libgof2hdaa.so
cmp -l /tmp/apk_lib.so _work/bins/android_2.0.16_libgof2hdaa.so | wc -l
```

```text
6d38fd767ee3b4f8174c371bce371bfc6397cf76e13c1756938b2a94658e3b58  APK lib
d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483  _work/bins
288 differing bytes
```

The 288 bytes are entirely `.shstrtab`/`sh_name`; every section payload
(including `.text` and the build-id) is identical. The APK `.shstrtab` order is
the original order; the `_work/bins` copy is the reordered one.

---

## 2. DEX metadata

### 2.1 Header

```sh
unzip -p "$APK" classes.dex > /tmp/classes.dex
```

```text
magic          dex\n035\0
file_size      3723352
header_size    112
string_ids     28895
type_ids       5177
proto_ids      6978
field_ids      12385
method_ids     29833
class_defs     4077
stored checksum 0x48e247fe  computed 0x48e247fe
stored sig      3987124219329f73f1e5cc16c24286a4b8b08819
computed sig    ea65af394fb6f44a149d74cb83acd30de4d06362
```

The checksum is valid; the internal SHA-1 is stale (a post-processing
artifact), but the APK `MANIFEST.MF` digest of `classes.dex` matches the
whole-file SHA-1, so the DEX is intact as packaged. This is the same result as
wave-87 (`build_metadata_source_paths_w3566.md`).

### 2.2 `BuildConfig` values

The DEX contains `BuildConfig` classes for the app and all bundled libraries.
Parsed from `class_data_item` + `static_values`:

| class | field | value |
|---|---|---|
| `net.fishlabs.gof2hdallandroid2012.BuildConfig` | `APPLICATION_ID` | `net.fishlabs.gof2hdallandroid2012` |
| | `BUILD_TYPE` | `release` |
| | `DEBUG` | `false` |
| | `FLAVOR` | `""` |
| | `VERSION_CODE` | `47947032` |
| | `VERSION_NAME` | `2.0.16` |
| `com.crashlytics.android.ndk.BuildConfig` | `VERSION_NAME` | `2.0.5` |
| | `GOOGLE_PLAY_SDK_VERSION_STRING` | `!SDK-VERSION-STRING!:com.crashlytics.sdk.android:crashlytics-ndk:2.0.5.27` |
| `com.crashlytics.android.BuildConfig` | `VERSION_NAME` | `2.9.5` |
| `com.crashlytics.android.answers.BuildConfig` | `VERSION_NAME` | `1.4.3` |
| `com.crashlytics.android.beta.BuildConfig` | `VERSION_NAME` | `1.2.10` |
| `io.fabric.sdk.android.BuildConfig` | `VERSION_NAME` | `1.4.4` |
| `com.android.billingclient.BuildConfig` | `VERSION_NAME` | `1.1` |
| `com.android.vending.expansion.downloader.BuildConfig` | `VERSION_NAME` | `1.1` |
| `com.google.android.vending.licensing.BuildConfig` | `VERSION_NAME` | `1.5` |

All library `BUILD_TYPE`s are `release`, `DEBUG=false`, `FLAVOR=""`. The
Crashlytics family carries `BUILD_NUMBER=27` (Fabric build number). This pins
the Java-side dependency era and the exact Crashlytics NDK version that
supplied `libcrashlytics.so`.

### 2.3 Java source files

The DEX `SourceFile` attributes give 29 `net/fishlabs/` Java files:

```text
AccelerometerListener.java  AccelerometerManager.java  AlarmReceiver.java
BaseGameActivity.java       BaseGameUtils.java         BillingManager.java
BuildConfig.java            ConfigChooser.java         ContextFactory.java
DeviceInfo.java             DownloaderActivity.java    FMODExtractionHandler.java
FishlabsDownloaderService.java                         GLView.java
GLViewRenderer.java         GOF2HD2012.java            GameHelper.java
GameHelperUtils.java        Gof2AchievementIDs.java    GooglePlayIAP.java
LocalizationService.java    Messenger.java             MessengerSwitcher.java
MultisampleConfigChooser.java                          NativeFunctionCalls.java
R.java                      Security.java              ToJNI.java
TouchHandler.java
```

The DEX string table contains `loadLibrary` and `gof2hdaa`, and the
decompiled `GOF2HD2012.java` shows `System.loadLibrary("fmodex")`,
`System.loadLibrary("fmodevent")`, `System.loadLibrary("gof2hdaa")` in that
order, explaining the runtime dependency order and the FMOD-before-game-lib
link order.

### 2.4 Native build paths in the DEX

A full string scan of the 3.7 MB DEX (51,950 strings) finds **zero** hits for:

```text
Android.mk  Application.mk  CMakeLists  ndk-build  jni/  LOCAL_  APP_
externalNativeBuild  abiFilters  /Users/  /Volumes/  /home/  .cpp  .cc  .cxx
```

The only `cmake` hit is the Java method name
`CmakeGooglePlayServicesAvailable` (case-insensitive), and the only `gradle`
hits are Firebase/Play-services error messages. The DEX therefore contains no
native build-system path or file name.

---

## 3. Native libraries in the APK

The APK contains exactly four `lib/armeabi-v7a/*.so` files:

```text
libcrashlytics.so  afbe76e6b1b057b5650a85d8784a22030578df68be342a96072555ae483c604b
libfmodevent.so    b92cd6ff8f8edb3d81023ee69c1f5239aeaf6cb4e89d6505101115189569c97d
libfmodex.so       60ec4de090bf442c7b9db41d0731aa22a101db21aab0ae32af1af6a860c3fa81
libgof2hdaa.so     6d38fd767ee3b4f8174c371bce371bfc6397cf76e13c1756938b2a94658e3b58
```

`libfmodex.so` and `libfmodevent.so` are byte-identical to
`_work/bins/apk_libs/*` (verified with `cmp`), so the repo's link inputs are the
APK's own FMOD prebuilts.

### 3.1 `libgof2hdaa.so` — the game lib (NDK r18b)

```text
.comment:
  Android (4751641 based on r328903) clang version 7.0.2
  (https://android.googlesource.com/toolchain/clang 003100370607242ddd5815e4a043907ea9004281)
  (https://android.googlesource.com/toolchain/llvm 1d739ffb0366421d383e04ff80ec2ee591315116)
  (based on LLVM 7.0.2svn)
  GCC: (GNU) 4.9.x 20150123 (prerelease)
.note.android.ident: API 16, "r18b", build "5063045"
.note.gnu.build-id:  c011810da45777b83ed721b2a87d517efed0d310
.note.gnu.gold-version: gold 1.12
NEEDED: liblog libGLESv2 libGLESv1_CM libEGL libandroid
        libfmodex libfmodevent libm libdl libc
```

The `.note.android.ident` section is **byte-identical** (152 bytes) to
`.cache/ndk/android-ndk-r18b/platforms/android-16/arch-arm/usr/lib/crtbegin_so.o`:

```python
# getsec(...) both files; print(len(a), len(b), a == b)
# 152 152 True
```

The only path-like string in the whole `.so` is the darwin libc++abi buildbot
path:

```text
/Volumes/Android/buildbot/src/android/ndk-release-r18/external/libcxx/../../external/libcxxabi/src/abort_message.cpp
```

The GCC 4.9.x `.comment` string comes from NDK r18b `libgcc.a` members, not
from the app compiler. This is the same recipe as the wave-127 synthesis note.

### 3.2 `libcrashlytics.so` — NDK r13b/r14-era, GCC 4.9.x, gnustl shared

```text
.comment:
  GCC: (GNU) 4.9.x 20150123 (prerelease)
.note.gnu.build-id:  e3c978ad09d1fab2d6c86de96df95c6442868a1a
.note.gnu.gold-version: gold 1.11
NEEDED: liblog libandroid libstdc++ libm libc libdl
FLAGS: SYMBOLIC BIND_NOW; FLAGS_1: NOW
ARM attributes: Tag_CPU_name "ARM v7", Tag_THUMB_ISA_use Thumb-2,
                Tag_FP_arch VFPv3-D16, Tag_ABI_optimization_goals Aggressive Speed
```

There is **no** `.note.android.ident` section. This is the key discriminator:
NDK r13b's `crtbegin_so.o` also has no `.note.android.ident`, while NDK r18b's
does; the r14 build in §4 likewise has no such note. The exact GCC/gold strings
match the r13b/r14 pair:

```sh
.cache/ndk/android-ndk-r13b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc --version
# arm-linux-androideabi-gcc (GCC) 4.9.x 20150123 (prerelease)
.cache/ndk/android-ndk-r13b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-ld.gold --version
# GNU gold (binutils-2.25-0666073 2.25.51.20141117) 1.11
```

`libcrashlytics.so` is therefore a **Crashlytics NDK 2.0.5 prebuilt** made with
an NDK r13b/r14-era toolchain (gold 1.11 + GCC 4.9.x 20150123; the exact
r13b-vs-r14 minor is not distinguishable from the stripped ELF), using
`gnustl_shared` (`libstdc++.so` NEEDED) and the ndk-build default
`armeabi-v7a` VFPv3-D16 (no NEON). It is not a Fishlabs build and is not
evidence for the game lib's flags beyond the era.

### 3.3 `libfmodex.so` / `libfmodevent.so` — FMOD prebuilts, GCC 4.4.3/4.6

```text
libfmodex.so:
  .comment: 194 × "GCC: (GNU) 4.4.3"
            1 × "GCC: (GNU) 4.6 20120106 (prerelease)"
  ARM: Tag_CPU_name "5TE", Tag_CPU_arch v5TE, Tag_THUMB_ISA_use Thumb-1,
       Tag_FP_arch VFPv1
  NEEDED: libc libstdc++ libm libdl liblog
  FLAGS_1: NOW; RELCOUNT 1199
  no .note.gnu.build-id, no .note.android.ident, no .note.gnu.gold-version

libfmodevent.so:
  .comment: 56 × "GCC: (GNU) 4.4.3"
  ARM: Tag_CPU_name "5TE", Tag_CPU_arch v5TE, Tag_THUMB_ISA_use Thumb-1,
       Tag_FP_arch VFPv2
  NEEDED: libc libstdc++ libm libdl liblog libfmodex
  FLAGS_1: NOW; RELCOUNT 754
  no .note.gnu.build-id, no .note.android.ident, no .note.gnu.gold-version
```

The source strings are FMOD's own relative paths (`../src/fmod.cpp`,
`../android/src/fmod_os_misc.cpp`, …), not absolute build paths. The only
absolute path string in either lib is `/proc/cpuinfo`. The `5TE`/Thumb-1/VFPv1
attributes are the old `armeabi` ABI, not `armeabi-v7a`; the libs are
forward-compatible on v7a. The source dump records FMOD `v44442` for HD and
`v432` for SD, consistent with a 2012-era FMOD 4.44 prebuilt.

---

## 4. The second 2.0.16 APK (Joppie / `GOF2_2.0.16_Android_Price.apk`)

The JoppieToppie Unity-remake repo ships a second 2.0.16 APK and lib via Git
LFS. Fetched with:

```sh
curl -sL -o joppie.apk \
  'https://media.githubusercontent.com/media/JoppieToppie/Galaxy-on-Fire-2-Unity-Remake/main/Reference/binaries/GOF2_2.0.16_Android_Price.apk'
curl -sL -o joppie_lib.so \
  'https://media.githubusercontent.com/media/JoppieToppie/Galaxy-on-Fire-2-Unity-Remake/main/Reference/binaries/libgof2hdaa.so'
```

```text
joppie.apk      1100f13643d5c7c3fdd8415f0be2a9cf1d80fe6693e3cbccbd87564e9962aa81
joppie_lib.so   125dfdb218470b76d5c41ee2eb00b55ce379963df8869ba5c6bece0fd040a50e
```

The lib is a **different native build**:

```text
.comment:
  GCC: (GNU) 4.9.x 20150123 (prerelease)
  Android clang version 3.8.275480  (based on LLVM 3.8.275480)
.note.gnu.build-id:  1dbb7d451b7ca0ff0cfccf2f2d2fae0e8f9c5a1c
.note.gnu.gold-version: gold 1.11
NEEDED: liblog libGLESv2 libGLESv1_CM libEGL libandroid
        libfmodex libfmodevent libm libstdc++ libdl libc
only path-like string:
  /Volumes/Android/buildbot/src/android/ndk-r14-release/external/libcxx/../../external/libcxxabi/src/abort_message.cpp
```

This is **NDK r14 / clang 3.8.275480 / gold 1.11 / gnustl_shared**, not the
APKPure r18b build. Its JNI layer also uses different symbol names
(`ndk23_Init`, `ndk23_newrender`, …) than the APKPure lib (`ndk*`).

The APK manifest is a **modern repack** (parsed AXML):

```text
versionCode=47947032  versionName=2.0.16
minSdkVersion=19  targetSdkVersion=30
compileSdkVersion=23  compileSdkVersionCodename=6.0-2438415
platformBuildVersionCode=30  platformBuildVersionName=11
launcher activity: com.setting.Gof2Setting
```

So the Joppie APK's packaging is not original either; it is a modern repack
with an r14 native lib. It is useful as a second native-build witness only.

---

## 5. Cross-platform artifacts

### 5.1 iOS 1.1.4 HD

```text
ios_1.1.4_HD  74ece5bedf561eed4e71b8d0b875e201c47858a2649a59d65af57390a0554958
Mach-O armv7, PIE, LC_VERSION_MIN_IPHONEOS 4.3 / SDK 6.0, cryptid 0
```

The binary contains the shared engine's `__RCSID` string:

```text
@(#)PROGRAM:AbyssEngineDevice  PROJECT:AbyssEngine-3
```

and four app source paths:

```text
/Users/hck/Documents/Dev/GalaxyOnFire2_merged/Classes/Game/ParticleSystems/IParticleSystem.cpp
/Users/hck/Documents/Dev/GalaxyOnFire2_merged/Classes/Game/ParticleSystems/ParticleSystemManager.cpp
/Users/hck/Documents/Dev/GalaxyOnFire2_merged/Classes/Game/ParticleSystems/ParticleSystemMesh.cpp
/Users/hck/Documents/Dev/GalaxyOnFire2_merged/Classes/Game/ParticleSystems/ParticleSystemSprite.cpp
```

The `@(#)` string sits at `__TEXT` vmaddr `0x408da8` (file offset `0x407da8`).
It matches the SD dump's engine library name and path:

```text
/Volumes/realtime/projects/1605_Abyss_Engine/iPhone/lib_float_iOS4/libAbyssEngineDevice.a(Engine.o)
/Volumes/realtime/projects/1605_Abyss_Engine/iPhone/include_float_iOS4/...
```

So the shared engine was the Xcode project `AbyssEngine` (target
`AbyssEngineDevice`, version 3), delivered as a prebuilt static library on iOS
and compiled into the Android `.so`. The iOS app itself was built in the
`GalaxyOnFire2_merged` tree. This is the same tree name as the source-dump
`mac fhd` section (`/Users/hck/Documents/Dev/GalaxyOnFire2_merged/...`).

### 5.2 iOS 1.0.5 SD

```text
ios_1.0.5_GalaxyOnFire2  8420bb7a19883a09e0a6961cc255a840295f4b479ebd230080fdfa38713980e3
ios_1.0.5_armv7          b8e8000feddab675f4101bed822afbc817e14521fa275e70ee3e1e8267f44661
Mach-O armv6+armv7 / armv7, unoptimized (-O0-like), libstdc++.6, SJLj
```

The binary contains the older app source root and the same particle-system TUs:

```text
/Users/mhehmeyer/Documents/Dev/GalaxyOnFire2/Classes/Game/MeshMerger/LodMeshMerger.cpp
/Users/mhehmeyer/Documents/Dev/GalaxyOnFire2/Classes/Game/ParticleSystems/IParticleSystem.cpp
/Users/mhehmeyer/Documents/Dev/GalaxyOnFire2/Classes/Game/ParticleSystems/ParticleSystemManager.cpp
/Users/mhehmeyer/Documents/Dev/GalaxyOnFire2/Classes/Game/ParticleSystems/ParticleSystemMesh.cpp
/Users/mhehmeyer/Documents/Dev/GalaxyOnFire2/Classes/Game/ParticleSystems/ParticleSystemSprite.cpp
/Users/mhehmeyer/Documents/Dev/GalaxyOnFire2/Classes/Facebook/FBGOF2Request.m
/Users/mhehmeyer/Documents/Dev/GalaxyOnFire2/Classes/Facebook/JSON/SBJsonParser.m
```

The FMOD paths in both iOS binaries are FMOD's own build paths
(`/Users/Mathew/Desktop/Release/AntHill_Builds/fmod4/...` for 1.0.5;
`/Library/anthill3/agent/var/jobs/job320331/fmod4/...` for 1.1.4), not
Fishlabs paths.

### 5.3 Windows `win_GOF2.exe`

```text
PE32 i386, 6 sections, linker 10.0 (MSVC 2010), subsystem GUI
timestamp 1346862626 = 2012-09-05 16:30:26 UTC
PDB: C:\Users\s.lohse\Desktop\GoF2_PC_REP\GOF2.pdb
.text raw size 0x12b400, entropy 8.000 (packed/encrypted)
```

The `.rdata`/`.data` still contain MSVC RTTI (`.?AVMGame@@`,
`.?AVAbyssEngine@@...`) and FMOD/D3DX imports, but `.text` does not
disassemble on disk. No `CMake`, `MSBuild`, `.vcxproj`, `.sln`, `Makefile` or
`@(#)` build string survives. The PC build is a separate VS2010 project by
`s.lohse`, not the Android build.

---

## 6. Public web / GitHub search

Searches run (authenticated `gh`, account `fionera`; Sourcegraph; GitLab;
Bitbucket; archive.org; Bing/Google/DDG):

```sh
gh api -X GET search/code -f q='GalaxyOnFire2_merged' -f per_page=30
gh api -X GET search/code -f q='AbyssEngineDevice' -f per_page=30
gh api -X GET search/code -f q='libAbyssEngineDevice' -f per_page=30
gh api -X GET search/code -f q='libgof2hdaa' -f per_page=30
gh api -X GET search/code -f q='gof2hdallandroid2012' -f per_page=30
gh api -X GET search/code -f q='gof2 Android.mk' -f per_page=30
gh api -X GET search/code -f q='gof2 Application.mk' -f per_page=30
gh api -X GET search/code -f q='gof2 ndk-build' -f per_page=30
gh api -X GET search/repositories -f q='gof2' -f per_page=20
gh api -X GET search/repositories -f q='AbyssEngine' -f per_page=20
gh api -X GET search/repositories -f q='galaxy on fire 2' -f per_page=20
curl -s 'https://sourcegraph.com/.api/search/stream?q=context:global+libgof2hdaa&v=V3&t=literal'
curl -s 'https://gitlab.com/api/v4/projects?search=gof2&per_page=30'
curl -s 'https://api.bitbucket.org/2.0/repositories?q=name~%22gof2%22&pagelen=30'
curl -s 'https://archive.org/advancedsearch.php?q=libgof2hdaa&fl%5B%5D=identifier&rows=20&output=json'
```

Results:

| query | result |
|---|---|
| `GalaxyOnFire2_merged` (code) | 12 hits, **all** `fionera/gof2hd-decomp` notes |
| `AbyssEngineDevice` (code) | 3 hits, **all** `fionera/gof2hd-decomp` notes |
| `libAbyssEngineDevice` (code) | 3 hits, **all** `fionera/gof2hd-decomp` notes |
| `libgof2hdaa` (code) | 2,088 hits; only JoppieToppie, Ravernstal/gof2edit, VSFLima/GOF2-Mod, this repo; no Fishlabs source |
| `gof2hdallandroid2012` (code) | 648 hits; JoppieToppie decompiled Java, LaunchTime package lists, this repo |
| `gof2 Android.mk` / `gof2 Application.mk` / `gof2 ndk-build` | only this repo's reconstruction notes and `joewebkid/AbyssEngine` docs |
| `gof2` repos | 218; all decomp/mod/remake/port/editor projects, no original source tree |
| `AbyssEngine` repos | 23; all unrelated engines or decomp/remake projects |
| `galaxy on fire 2` repos | 41; all decomp/mod/remake/port projects |
| GitLab `gof2` | 10 projects, all unrelated course/Java projects |
| Bitbucket `gof2` | 0 |
| archive.org `libgof2hdaa` | 0 |
| archive.org `GalaxyOnFire2_merged` | 0 |

No public repository contains `Android.mk`, `Application.mk`,
`CMakeLists.txt`, `build.gradle`, `gradle.properties`, `*.ninja`,
`compile_commands.json`, `Makefile`, `*.pbxproj`, `*.vcxproj`, `*.sln`,
object files, static archives or unstripped symbols from the original Fishlabs
Android build. `joewebkid/AbyssEngine` is another reconstruction
(`CMakeLists.txt`, `cmake/toolchain.cmake`, `third_party/match_libs.cmake`),
not the original build.

---

## 7. What this does and does not prove

**Proven / high confidence:**

* The APK is a 2018-10-22 Android Gradle 3.1.2 release build of
  `net.fishlabs.gof2hdallandroid2012` 2.0.16, signed with the Fishlabs 2010 key.
* The game `.so` is the NDK r18b / clang 7.0.2 / gold 1.12 / API 16 /
  armeabi-v7a / softfp / static libc++ build (`.comment`,
  `.note.android.ident`, `.note.gnu.gold-version`, `.ARM.attributes`).
* `libcrashlytics.so` is a Crashlytics NDK 2.0.5 prebuilt from the NDK
  r13b/r14 era (GCC 4.9.x 20150123, gold 1.11, gnustl_shared, VFPv3-D16, no
  `.note.android.ident`).
* `libfmodex.so` / `libfmodevent.so` are FMOD prebuilts from the GCC 4.4.3/4.6
  era, built for `armeabi` (ARMv5TE/Thumb-1/VFPv1–2).
* The iOS 1.1.4 HD app tree is `/Users/hck/Documents/Dev/GalaxyOnFire2_merged/`;
  the shared engine is the Xcode project `AbyssEngine` (target
  `AbyssEngineDevice`, version 3), shipped as `libAbyssEngineDevice.a` on iOS.
* The Windows build is a separate MSVC 2010 project by `s.lohse` (2012-09-05).
* A second 2.0.16 APK is an NDK r14 / clang 3.8.275480 / gold 1.11 /
  gnustl_shared build in a modern repack.
* No original Android build file survives in the searched artifacts or the
  public web.

**Still unobservable / inference:**

* The exact original `Android.mk`, `Application.mk`, `CMakeLists.txt`,
  `build.gradle`, CMake version, generator, build directory and link-line text.
  The effective flags and link order are proven; the file is not.
* The exact native link timestamp. The APK packaging timestamp
  (2018-10-22 10:18:17 CEST) is the closest witness.
* The original build host OS/CPU. The darwin libc++abi path is a strong
  inference (~90%), not a direct proof.
* Whether the Android build was driven by `externalNativeBuild { cmake { ... } }`
  or a standalone CMake invocation. Both are consistent with the ELF.
* The exact FMOD version. The source dump says `v44442` (HD) / `v432` (SD);
  the `.comment` only pins the GCC era.

**No residual change.** This evidence is provenance only. It does not identify
a missing compile/link flag, a different TU, or a different source shape for
any of the 18 non-byte-exact functions. The wave-127 conclusion stands: the
flags are right; the remaining 18 functions are source reconstruction.

---

## 8. Reproduction commands

```sh
# APK inventory / metadata
APK="/home/debian/GalaxyOnFire2/Original APK/Galaxy on Fire 2™ HD_2.0.16_APKPure.apk"
unzip -Z1 "$APK" | wc -l
unzip -p "$APK" META-INF/MANIFEST.MF | head -5
unzip -p "$APK" assets/crashlytics-build.properties
unzip -p "$APK" AndroidManifest.xml > /tmp/AndroidManifest.xml
unzip -p "$APK" classes.dex > /tmp/classes.dex
unzip -p "$APK" resources.arsc > /tmp/resources.arsc
unzip -p "$APK" META-INF/CERT.RSA > /tmp/cert.rsa
openssl pkcs7 -inform DER -in /tmp/cert.rsa -print_certs -text -noout

# native libs
for l in libcrashlytics libfmodex libfmodevent libgof2hdaa; do
  unzip -p "$APK" lib/armeabi-v7a/$l.so > /tmp/$l.so
  echo "=== $l ==="
  readelf -p .comment /tmp/$l.so
  readelf -n /tmp/$l.so
  readelf -dW /tmp/$l.so | grep NEEDED
  readelf -A /tmp/$l.so
done

# NDK r13b vs r18b toolchain strings
.cache/ndk/android-ndk-r13b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc --version
.cache/ndk/android-ndk-r13b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-ld.gold --version
.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc --version
.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-ld.gold --version

# iOS / Windows
strings -a -n 5 _work/bins/ios_1.1.4_HD | grep -E '^/Users/(hck|mhehmeyer|n\.tran)|@\(#\)|libAbyssEngineDevice'
strings -a -n 5 _work/bins/ios_1.0.5_armv7 | grep -E '^/Users/(hck|mhehmeyer|n\.tran)'
strings -a -n 5 _work/bins/win_GOF2.exe | grep -E '\.pdb|AbyssEngine'

# second 2.0.16 APK
curl -sL -o /tmp/joppie.apk \
  'https://media.githubusercontent.com/media/JoppieToppie/Galaxy-on-Fire-2-Unity-Remake/main/Reference/binaries/GOF2_2.0.16_Android_Price.apk'
curl -sL -o /tmp/joppie_lib.so \
  'https://media.githubusercontent.com/media/JoppieToppie/Galaxy-on-Fire-2-Unity-Remake/main/Reference/binaries/libgof2hdaa.so'
readelf -p .comment /tmp/joppie_lib.so
readelf -n /tmp/joppie_lib.so
readelf -dW /tmp/joppie_lib.so | grep NEEDED
strings -a -n 6 /tmp/joppie_lib.so | grep '/Volumes/Android'

# web
gh api -X GET search/code -f q='GalaxyOnFire2_merged' -f per_page=30
gh api -X GET search/code -f q='AbyssEngineDevice' -f per_page=30
gh api -X GET search/code -f q='libAbyssEngineDevice' -f per_page=30
curl -s 'https://sourcegraph.com/.api/search/stream?q=context:global+libgof2hdaa&v=V3&t=literal'
```

The AXML and DEX parsers used are the compact Python scripts in
`build_metadata_source_paths_w3566.md` §8, extended to print all
`BuildConfig` classes and the `resources.arsc` integer value for
`google_play_services_version`.

---

## 9. Evidence index

Prior notes:

* `build-flag-synthesis-wave127-w7601.md` — flag synthesis, CMake-vs-ndk-build,
  section diff.
* `build-artifact-hunt-wave120-w7006.md` — exhaustive artifact inventory,
  second 2.0.16 APK, public web.
* `cross-platform-build-evidence-w5106.md` — iOS/Windows cross-platform
  codegen and build metadata.
* `build_metadata_source_paths_w3566.md` — ELF/APK/DEX metadata, `main.c`,
  `abort_message.cpp`, `.missioncase`.
* `_work/original_layout/original_layout_notes.md` — source-dump provenance,
  `libAbyssEngineDevice.a`, `GalaxyOnFire2_merged` tree.

New evidence in this note:

* APK `BuildConfig` dependency table (Crashlytics NDK 2.0.5.27, Fabric
  1.4.4.27, Play Services 12.4.51, Billing 1.1, Expansion Downloader 1.1,
  Licensing 1.5).
* `libcrashlytics.so` = NDK r13b/r14-era (GCC 4.9.x 20150123 + gold 1.11 +
  gnustl_shared, no `.note.android.ident`).
* `libfmodex.so` / `libfmodevent.so` = GCC 4.4.3/4.6, ARMv5TE/Thumb-1/VFPv1–2.
* iOS 1.1.4 HD `@(#)PROGRAM:AbyssEngineDevice  PROJECT:AbyssEngine-3` and
  `/Users/hck/Documents/Dev/GalaxyOnFire2_merged/...` paths.
* Joppie 2.0.16 APK = NDK r14 / clang 3.8.275480 / gold 1.11 / gnustl_shared,
  modern repack manifest.
* Windows `GOF2.exe` = MSVC 2010, PDB `C:\Users\s.lohse\Desktop\GoF2_PC_REP\GOF2.pdb`,
  timestamp 2012-09-05, packed `.text`.
* Public-web negative result for `GalaxyOnFire2_merged`, `AbyssEngineDevice`,
  `libAbyssEngineDevice`, `libgof2hdaa`, `gof2 Android.mk`,
  `gof2 Application.mk`.
