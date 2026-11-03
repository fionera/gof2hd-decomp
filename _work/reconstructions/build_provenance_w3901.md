# Build-system provenance: how the original Android `libgof2hdaa.so` was built (w3901)

Status: research note, 2026-11-03. Worktree `wt/w3901`, branch `wave89-w3901`,
base `dfc9d7a1`. No source or build-flag change is landed: this commit contains
this note only. Scratch work is under `/dev/shm/w3901`; the canonical
`cmake-build-match` tree was used for the gate.

## 0. Bottom line

The shipped Android library was built with:

* **NDK r18b** (`Pkg.Revision = 18.1.5063045`), **clang 7.0.2**, **gold 1.12**,
  target `armv7-none-linux-androideabi16`, armeabi-v7a soft-float, Thumb-2,
  NEON, API 16, `c++_static`.
* **macOS host** using the **`android-ndk-r18b-darwin-x86_64`** package. The
  only path-like string in the `.so` is the darwin NDK's
  `/Volumes/Android/buildbot/.../libcxxabi/src/abort_message.cpp`; the linux
  package's identical archive carries `/buildbot/...` instead.
* **CMake with the NDK r18b `build/cmake/android.toolchain.cmake`**, Release,
  `ANDROID_ABI=armeabi-v7a`, `ANDROID_PLATFORM=android-16`,
  `ANDROID_STL=c++_static`, `ANDROID_ARM_NEON=ON` (standalone CMake or Android
  Gradle Plugin `externalNativeBuild`; the stripped `.so` cannot distinguish
  those two drivers). The CMake defaults are the only standard-driver defaults
  that match the two independent fingerprints: no `-fdata-sections`, and user
  libraries (FMOD) *before* the static libc++ archives.  Both CMake and
  ndk-build add `-std=c++11` once `c++_static` is selected, so that flag is
  consistent with either driver.
* **Effective compile flags**: the CMake/NDK r18b defaults —
  `-Oz -g -std=c++11 -fPIC -fstack-protector-strong -ffunction-sections
  -funwind-tables -mthumb -mfpu=neon -mfloat-abi=softfp`, RTTI and exceptions
  on, `-fdata-sections` off.
* **Effective link flags**: the CMake/NDK r18b defaults —
  `-z now` (RELRO/BIND_NOW), SHA-1 `--build-id`, sysv hash,
  `--exclude-libs=libgcc.a`, zlib/libzip `--whole-archive`, `-s`, and FMOD
  before the libc++ archives.
* **Packaging/strip**: Android Gradle Plugin **3.1.2**, v1 + v2 signing,
  4-byte zipalign, then GNU **`arm-linux-androideabi-strip`** (not
  `llvm-strip`).

The old 70/25/5 CMake-vs-ndk-build-vs-custom guess is replaced by the
confidence table in §1.

## 1. Confidence table

| claim | confidence | hard evidence |
|---|---:|---|
| NDK r18b, clang 7.0.2, gold 1.12 | 99% | `.comment`, `.note.android.ident`, `.note.gnu.gold-version` |
| API 16, armeabi-v7a softfp, Thumb-2, NEON | 99% | ELF header flags, `.ARM.attributes`, `.note.android.ident` |
| `c++_static` (not `c++_shared`) | 99% | no `libc++_shared.so` NEEDED; `__cxa_*` defined; no `std::__ndk1` imports |
| macOS host | 95% | darwin NDK `libc++abi.a` path string; macOS source-dump paths |
| `android-ndk-r18b-darwin-x86_64` package | 95% | `/Volumes/Android/...` in `.rodata`; darwin archive sha; linux archive has `/buildbot/...` |
| CMake NDK r18b toolchain | **90%** | CMake default compile/link lines match; ndk-build defaults do not (see §5) |
| custom build script | 8% | possible, but would have to reproduce CMake's default order and flags |
| ndk-build r18b | 2% | default `-fdata-sections` and STL-before-user-libs link order both contradict the binary |
| compile flags: CMake defaults, `-fdata-sections` off | 95% | `.rodata`/`.data` byte-identical; CMake probe compile line; w3750 A/B |
| link flags: CMake/NDK r18b defaults | 95% | full CMake default link line relinks to the exact canonical sha |
| GNU `arm-linux-androideabi-strip` packaging strip | 99% | `strip(ground truth) == APK .so` exactly; `llvm-strip` differs |
| Android Gradle Plugin 3.1.2 packaging | 99% | `META-INF/MANIFEST.MF` `Created-By: Android Gradle 3.1.2` |
| v1 + v2 signing, no v3 | 99% | `CERT.SF` `X-Android-APK-Signed: 2`; APK Signing Block IDs |
| 4-byte zipalign | 99% | 44 stored entries all 4-byte aligned; zipalign extra field `0xd935` |
| same HD/merged source line | 80% | TU membership matches HD 1.1.19; mac merged-tree paths; no Android path in dumps |
| exact CMake/Gradle/Ninja version | unknown | not embedded |
| exact Android project directory / build file | unknown | not embedded; only `main.c` survives |

## 2. Artifacts and hashes

```sh
APK="/home/debian/GalaxyOnFire2/Original APK/Galaxy on Fire 2™ HD_2.0.16_APKPure.apk"
O=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
sha256sum "$APK" "$O" /dev/shm/w3901/apk/lib/armeabi-v7a/libgof2hdaa.so
```

```text
9cfe4484a475c9a31a336df9dea6408fbb129d88aa19083afe808ef6bca9eb19  APK
d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483  ground truth
6d38fd767ee3b4f8174c371bce371bfc6397cf76e13c1756938b2a94658e3b58  APK lib/armeabi-v7a/libgof2hdaa.so
```

The current tree's canonical build is sha256
`88aff1154772f836a53b12cc094637deec50026c74db18c1cd59fdced0cbfecd`
(2,187,328 bytes), `.text 0x1337c8 @ 0x69440`,
`.rodata 0x41b67 @ 0x1c9e10`, `.data 0x296c @ 0x214000` — the same section
sizes and addresses as the original.

## 3. APK inventory: no build-system leftovers

```sh
python3 - <<'PY'
import zipfile, collections
p="/home/debian/GalaxyOnFire2/Original APK/Galaxy on Fire 2™ HD_2.0.16_APKPure.apk"
z=zipfile.ZipFile(p)
print("entries", len(z.infolist()))
for i in z.infolist():
    print(f"{i.filename}\t{i.file_size}\t{i.compress_size}\t{i.CRC:08x}\t{i.compress_type}\t{i.date_time}\t{i.extra.hex()}")
PY
```

Summary: **143 entries**.

```text
  1  AndroidManifest.xml
  3  META-INF/
 14  assets/
  1  classes.dex
  6  fabric/
  4  lib/
 96  res/
  1  resources.arsc
 17  root *.properties
```

The four `lib/` entries are all `armeabi-v7a`:

```text
lib/armeabi-v7a/libcrashlytics.so   202580  99707  0x2831a2a4  method 8  (1980,0,0,0,0,0)
lib/armeabi-v7a/libfmodevent.so     370660 166641  0x4b444e5c  method 8  (1980,0,0,0,0,0)
lib/armeabi-v7a/libfmodex.so        920204 450904  0x04d7da88  method 8  (1980,0,0,0,0,0)
lib/armeabi-v7a/libgof2hdaa.so     2187328 1009527 0xa7ae3617  method 8  (1980,0,0,0,0,0)
```

There is **no** `Android.mk`, `Application.mk`, `CMakeLists.txt`,
`build.gradle`, `gradle.properties`, `.ninja_log`, `.ninja_deps`,
`compile_commands.json`, `assets/buildinfo`, or `res/raw/*` in the APK:

```text
leftovers []
```

All entries have DOS timestamp `(1980, 0, 0, 0, 0, 0)` (the zip epoch), and the
central-directory extra fields are empty. Local extra fields are present only
on stored entries that zipalign padded (§8).

## 4. DEX and asset string scan: no native build path

The APK's only build-system string is in `META-INF/MANIFEST.MF`:

```text
Manifest-Version: 1.0
Built-By: Generated-by-ADT
Created-By: Android Gradle 3.1.2
```

`classes.dex` and every asset were scanned for
`ndk-build`, `cmake`, `ninja`, `gradle`, `Android.mk`, `Application.mk`,
`/Users/`, `/Volumes/`, `/buildbot/`, `r18`, `clang`, `gcc`, `-Oz`,
`-fdata-sections`, `LOCAL_`, `APP_`, `CMAKE_`, `ANDROID_`, build paths and
usernames:

```sh
python3 - <<'PY'
import zipfile
p="/home/debian/GalaxyOnFire2/Original APK/Galaxy on Fire 2™ HD_2.0.16_APKPure.apk"
z=zipfile.ZipFile(p)
toks=[b'ndk-build',b'cmake',b'CMake',b'ninja',b'gradle',b'Android.mk',
      b'Application.mk',b'/Users/',b'/Volumes/',b'/buildbot/',b'r18',
      b'clang',b'gcc',b'-Oz',b'-fdata-sections',b'LOCAL_',b'APP_',
      b'CMAKE_',b'ANDROID_',b'/home/',b'.cpp',b'jni',b'compile_commands',
      b'.ninja_log',b'.ninja_deps',b'build.gradle',b'gradle.properties']
for i in z.infolist():
    if i.filename.startswith('lib/') or i.filename.endswith('.png'): continue
    d=z.read(i.filename)
    hits=[(t.decode(),d.count(t)) for t in toks if d.count(t)]
    if hits: print(i.filename, hits)
PY
```

```text
assets/pl.lang [('jni', 18)]
classes.dex [('ninja', 4), ('gradle', 2), ('LOCAL_', 9), ('APP_', 61), ('ANDROID_', 8), ('build.gradle', 1)]
```

All of those are generic Java/library strings, not native-build paths:

* `gradle` / `build.gradle` are from
  `Default FirebaseApp failed to initialize ... google-services was not applied
  to your gradle project.` and
  `Incorrect provider authority in manifest. Most likely due to a missing
  applicationId variable in application's build.gradle.`
* `ninja` is inside the bundled public-suffix TLD regex (`...|nico|ninja|nissan|...`).
* `LOCAL_`, `APP_`, `ANDROID_` are Java identifiers/constants.
* `pl.lang`'s `jni` is a Polish word substring.

The DEX `BuildConfig` (parsed in w3566) is
`APPLICATION_ID=net.fishlabs.gof2hdallandroid2012`, `BUILD_TYPE=release`,
`DEBUG=false`, `FLAVOR=""`, `VERSION_CODE=47947032`, `VERSION_NAME=2.0.16`.
The binary manifest gives `minSdkVersion=16`, `targetSdkVersion=27`.
`assets/crashlytics-build.properties` gives the build date
`Mon Oct 22 10:18:17 CEST 2018`.

## 5. ELF forensics: toolchain and link inputs

```sh
readelf -hW  "$O"
readelf -AW  "$O"
readelf -nW  "$O"
readelf -p .comment "$O"
readelf -dW  "$O"
readelf --dyn-syms -W "$O" | grep -E 'memalign|posix_memalign|_Unwind_|__gnu_Unwind_Find_exidx|_ZNSt3__1'
```

Key output:

```text
Class: ELF32
Data:  2's complement, little endian
Type:  DYN (Shared object file)
Machine: ARM
Flags: 0x5000200, Version5 EABI, soft-float ABI
Number of program headers: 8
Number of section headers: 27
```

```text
Tag_CPU_name: "ARM v7"
Tag_CPU_arch: v7
Tag_CPU_arch_profile: Application
Tag_THUMB_ISA_use: Thumb-2
Tag_FP_arch: VFPv3
Tag_Advanced_SIMD_arch: NEONv1
Tag_ABI_PCS_wchar_t: 4
Tag_ABI_align_needed: 8-byte
Tag_ABI_enum_size: int
Tag_CPU_unaligned_access: v6
```

```text
.note.android.ident: API 16, "r18b", build "5063045"
.note.gnu.build-id:  c011810da45777b83ed721b2a87d517efed0d310
.note.gnu.gold-version: gold 1.12
.comment:
  Android (4751641 based on r328903) clang version 7.0.2 (...)
  GCC: (GNU) 4.9.x 20150123 (prerelease)
```

The `.note.android.ident` section is **byte-identical** to the NDK r18b
`platforms/android-16/arch-arm/usr/lib/crtbegin_so.o` note (152 bytes, same
`10 00 00 00 72 31 38 62 ... 35 30 36 33 30 34 35` payload). The GCC 4.9.x
`.comment` string comes from NDK r18b `libgcc.a`/libcrypto, not from the app
TUs.

```text
NEEDED: liblog, libGLESv2, libGLESv1_CM, libEGL, libandroid,
        libfmodex, libfmodevent, libm, libdl, libc
SONAME: libgof2hdaa.so
FLAGS:  BIND_NOW / NOW
RELCOUNT: 1162
```

```text
_Unwind_Resume, _Unwind_VRS_Get, _Unwind_RaiseException, _Unwind_VRS_Set,
_Unwind_GetLanguageSpecificData, _Unwind_DeleteException,
_Unwind_GetRegionStart                         UND
memalign@LIBC                                  UND
__gnu_Unwind_Find_exidx                        absent
std::__ndk1 symbols                            absent
```

That is the `c++_static` + `libandroid_support.a` + FMOD-before-libc++abi
fingerprint: `_Unwind_*` are resolved at runtime from `libfmodex.so`, and
`libunwind.a`'s `__gnu_Unwind_Find_exidx` is not pulled.

The only path-like string in the whole `.so` is:

```text
/Volumes/Android/buildbot/src/android/ndk-release-r18/external/libcxx/../../external/libcxxabi/src/abort_message.cpp
```

The only app source-file-name witness is `main.c`, in the three log strings
`dataDirectory in main.c: %s`, `rootPath in main.c: %s`,
`zipDirectory in main.c: %s`. No app source path survives.

## 6. The build-system discriminator: CMake vs ndk-build

A minimal CMake project using the NDK r18b toolchain and a minimal ndk-build
project were built with the same NDK and the same FMOD shared libraries. The
commands are in §10; the outputs are:

### 6.1 CMake r18b toolchain defaults

Compile line (paths abbreviated):

```text
clang++ --target=armv7-none-linux-androideabi16 \
  --gcc-toolchain=$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64 \
  --sysroot=$NDK/sysroot \
  -isystem $NDK/sources/cxx-stl/llvm-libc++/include \
  -isystem $NDK/sources/android/support/include \
  -isystem $NDK/sources/cxx-stl/llvm-libc++abi/include \
  -isystem $NDK/sysroot/usr/include/arm-linux-androideabi \
  -g -DANDROID -ffunction-sections -funwind-tables -fstack-protector-strong \
  -no-canonical-prefixes -march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16 \
  -mthumb -mfpu=neon -Wa,--noexecstack -Wformat -Werror=format-security \
  -std=c++11 -Oz -DNDEBUG -fPIC -c file.cpp -o file.o
```

Link line (paths abbreviated):

```text
clang++ --target=armv7-none-linux-androideabi16 ... -fPIC ... -std=c++11 -Oz -DNDEBUG \
  -Wl,--exclude-libs,libgcc.a -Wl,--exclude-libs,libatomic.a -nostdlib++ \
  --sysroot $NDK/platforms/android-16/arch-arm \
  -Wl,--build-id -Wl,--warn-shared-textrel -Wl,--fatal-warnings \
  -Wl,--fix-cortex-a8 -Wl,--exclude-libs,libunwind.a \
  -L$NDK/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a \
  -Wl,--no-undefined -Wl,-z,noexecstack -Qunused-arguments \
  -Wl,-z,relro -Wl,-z,now -shared -Wl,-soname,libt.so -o libt.so file.o \
  -llog -lGLESv2 -lGLESv1_CM -lEGL -landroid \
  $APK_LIBS/libfmodex.so $APK_LIBS/libfmodevent.so -lm -ldl -latomic -lm \
  $NDK/.../libc++_static.a $NDK/.../libc++abi.a \
  $NDK/.../libandroid_support.a $NDK/.../libunwind.a -ldl
```

The probe's `DT_NEEDED` order is exactly the original's:

```text
liblog, libGLESv2, libGLESv1_CM, libEGL, libandroid,
libfmodex, libfmodevent, libm, libdl, libc
```

The probe's unwind symbols are exactly the original's:

```text
_Unwind_*                          UND
__gnu_Unwind_Find_exidx            absent
```

### 6.2 ndk-build r18b defaults

Compile line (paths abbreviated):

```text
clang++ -target armv7-none-linux-androideabi16 \
  -fdata-sections -ffunction-sections -fstack-protector-strong -funwind-tables \
  -no-canonical-prefixes -g -Wno-invalid-command-line-argument \
  -Wno-unused-command-line-argument -fno-exceptions -fno-rtti \
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

Link line (paths abbreviated):

```text
clang++ -Wl,-soname,libt.so -shared \
  --sysroot=$NDK/platforms/android-16/arch-arm file.o \
  $NDK/.../libc++_static.a $NDK/.../libc++abi.a \
  $NDK/.../libandroid_support.a $NDK/.../libunwind.a \
  -lgcc -Wl,--exclude-libs,libgcc.a -latomic -Wl,--exclude-libs,libatomic.a \
  ... -Wl,--build-id -nostdlib++ -Wl,--no-undefined -Wl,-z,noexecstack \
  -Wl,-z,relro -Wl,-z,now -Wl,--warn-shared-textrel -Wl,--fatal-warnings \
  ... -llog -L$APK_LIBS -lfmodex -lfmodevent -lm -ldl -ldl -lc -lm -o libt.so
```

The ndk-build probe's unwind symbols are **different** from the original:

```text
_Unwind_*                          defined locally (hidden by --exclude-libs,libunwind.a)
__gnu_Unwind_Find_exidx            UND
```

### 6.3 Why this identifies CMake

| observable | original | CMake r18b default | ndk-build r18b default |
|---|---|---|---|
| `-fdata-sections` | **off** (data layout) | **off** | **on** |
| `-std=c++11` | on | on (toolchain default) | on (`c++_static` STL) |
| `-fPIC` vs `-fpic` | not distinguishable | `-fPIC` | `-fpic` |
| link order | FMOD before libc++abi | **FMOD before libc++abi** | STL archives before FMOD |
| `_Unwind_*` | **UND** | **UND** | defined locally |
| `__gnu_Unwind_Find_exidx` | **absent** | **absent** | **UND** |
| `DT_NEEDED` order | log, GLES, ..., fmodex, fmodevent, m, dl, c | **same** | depends on `LOCAL_LDLIBS`, but STL order already differs |

The ndk-build link order is fixed by
`$NDK/build/core/default-build-commands.mk`:
`objects, whole archives, static libs, libgcc, libatomic, shared libs,
LOCAL_LDFLAGS, LOCAL_LDLIBS`. `libc++_static.a`/`libc++abi.a`/`libandroid_support.a`/`libunwind.a`
are static libs, so they precede FMOD even when FMOD is in `LOCAL_LDLIBS` or
`LOCAL_SHARED_LIBRARIES`. CMake's `target_link_libraries` order puts the user
libraries (FMOD) before `CMAKE_CXX_STANDARD_LIBRARIES` (the STL archives). The
original is the CMake order.

A controlled relink of the canonical objects with the **full CMake default link
flag set** (plus `--unresolved-symbols=ignore-all` for the tree's decompiler
shims) produced the exact canonical sha256
`88aff1154772f836a53b12cc094637deec50026c74db18c1cd59fdced0cbfecd`; the gate
stayed at 4498/4498. That proves the CMake link flags are byte-equivalent to
the tree's shorter line and consistent with the original.

## 7. Host OS and NDK package

The original's only path-like string is the darwin NDK's libc++abi
`abort_message.cpp` path:

```text
/Volumes/Android/buildbot/src/android/ndk-release-r18/external/libcxx/../../external/libcxxabi/src/abort_message.cpp
```

The same archive in the linux NDK r18b package contains `/buildbot/...`
instead:

```sh
strings -a $NDK_LINUX/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/libc++abi.a \
  | grep abort_message
# /buildbot/src/android/ndk-release-r18/external/libcxx/../../external/libcxxabi/src/abort_message.cpp

strings -a $NDK_DARWIN/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/libc++abi.a \
  | grep abort_message
# /Volumes/Android/buildbot/src/android/ndk-release-r18/external/libcxx/../../external/libcxxabi/src/abort_message.cpp
```

Archive hashes:

```text
9a0f50d22ef53a946b76d689f723b2ca88d2cb0f7e17ee133ab19395a925fadb  darwin libc++abi.a
6993e991ff2d9e4f45febab2f7758fe293c14d08f2364796c6d63f2b5cc210a6  linux  libc++abi.a
```

The original `.so` contains the darwin string and its `abort_message` body is
the darwin archive member (w3566). Therefore the original linked the
**darwin-x86_64 NDK r18b** `libc++abi.a`. The linux package's `libc++_static.a`,
`libandroid_support.a`, `libunwind.a` and `libgcc.a` differ in bytes from the
darwin package's, but w3750's A/B showed that only `libc++abi.a` changes the
gate (`linux libc++abi.a` -> byte 4344; the other linux archives -> 4498).
The `libc++abi.a` is the one host-specific archive a Linux rebuild must
replace.

`$NDK/source.properties` gives `Pkg.Revision = 18.1.5063045`, matching the
`.note.android.ident` build string `5063045`.

## 8. Packaging: signing, zipalign, strip

### 8.1 Signing

`META-INF/CERT.SF`:

```text
Signature-Version: 1.0
Created-By: 1.0 (Android)
SHA1-Digest-Manifest: uKjn8o+t247T1imzl8XcmbVDVj0=
X-Android-APK-Signed: 2
```

`META-INF/CERT.RSA`:

```text
Subject/Issuer: O=FISHLABS Entertainment GmbH
Serial: 1278067031 (0x4c2dc157)
Signature Algorithm: sha1WithRSAEncryption
Validity: Jul 2 10:37:11 2010 GMT .. Jun 19 10:37:11 2060 GMT
Public Key: RSA 1024-bit
```

The APK Signing Block is present immediately before the central directory:

```text
central_dir 0x614a90  signing_block 0x613a90  size 0xff8  magic "APK Sig Block 42"
pair 0x7109871a len 843     # APK Signature Scheme v2
  digest alg 0x103 (RSA PKCS#1 v1.5 SHA-256), 32-byte digest
  signature alg 0x103, 128-byte signature
  public key 162 bytes (RSA-1024 SubjectPublicKeyInfo)
pair 0x42726577 len 3205    # verity padding, all zero
```

There is no v3 block (`0xf05368c0`). The APK is **v1 + v2**, not v3. This is
the AGP 3.1.2 / apksigner default.

### 8.2 Zipalign

The APK has 44 stored entries; every stored entry's local data offset is
4-byte aligned (`offset % 4 == 0`), and padded entries carry the zipalign
extra field `0xd935`:

```text
stored 44  misaligned []
('res/drawable-hdpi-v4/common_full_open_on_phone.png', 0, '35d902000400')
('res/drawable-hdpi-v4/common_google_signin_btn_icon_dark_normal_background.9.png', 0, '35d902000400')
...
```

The native libraries and `classes.dex` are deflated (`method 8`), so zipalign
does not align them; `resources.arsc` and the PNGs are stored and are aligned.
This is the standard AGP 3.1.2 packaging shape.

### 8.3 Strip

The ground-truth file and the APK library differ by **288 bytes**, all in
`.shstrtab` (`0x215af0..0x215c07`) and the section headers' `sh_name` fields
(section header table `0x215c08..0x216040`). Every section's content is
identical, including `.text`, `.rodata`, `.data`, `.dynsym`, and the build-id.

```sh
$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-strip \
  --strip-all -o /dev/shm/w3901/strip/gnu_strip_all.so "$O"
sha256sum /dev/shm/w3901/strip/gnu_strip_all.so \
          /dev/shm/w3901/apk/lib/armeabi-v7a/libgof2hdaa.so
```

```text
6d38fd767ee3b4f8174c371bce371bfc6397cf76e13c1756938b2a94658e3b58  gnu strip --strip-all
6d38fd767ee3b4f8174c371bce371bfc6397cf76e13c1756938b2a94658e3b58  APK lib
```

`--strip-unneeded` produces the same sha. `llvm-strip --strip-all` (LLVM 7.0.2)
does **not**:

```text
f5029403c74ddd75b1a54f4920543a49a3652788b9efb6c6ad00bd7842cc3b48  llvm-strip --strip-all
6d38fd767ee3b4f8174c371bce371bfc6397cf76e13c1756938b2a94658e3b58  APK lib
876 differing bytes (and llvm-strip's .shstrtab is 0xec, not 0x117)
```

GNU strip is version `GNU strip (GNU Binutils) 2.27.0.20170315` in the r18b
NDK. The shipped `.so` is therefore GNU-stripped. The ground-truth file's
`.shstrtab` is in gold Stringpool order; it is a normalized copy of the APK
library (or a gold `-s` link output), not the shipped artifact. The exact
link-time `-s` choice is not recoverable from the APK; the packaging strip is
GNU.

## 9. Source-layout cross-check: macOS, merged HD tree

The source dumps are macOS/Xcode artifacts:

```sh
grep -n '/Users/hck/Documents/Dev/GalaxyOnFire2_merged/' \
  "/home/debian/GalaxyOnFire2/source file list dump/source file lists dump.txt" | head
# /Users/hck/Documents/Dev/GalaxyOnFire2_merged/Classes/Game/MeshMerger.cpp
# /Users/hck/Documents/Dev/GalaxyOnFire2_merged/Classes/Game/MeshMerger/LodMeshMerger.cpp
# /Users/hck/Documents/Dev/GalaxyOnFire2_merged/Classes/Game/ParticleSystems/IParticleSystem.cpp
# ...

grep -n '/Users/n.tran/Library/Developer/Xcode/DerivedData/' \
  "/home/debian/GalaxyOnFire2/source file list dump/gof2hd_1.1.19_ios.cpp" | head
# /Users/n.tran/Library/Developer/Xcode/DerivedData/GalaxyOnFire2-.../Build/Intermediates.noindex/...
```

The mac fhd tree is `/Users/hck/Documents/Dev/GalaxyOnFire2_merged/`; the iOS
1.1.19 dump is an Xcode DerivedData archive under `/Users/n.tran/...`; the SD
1.0.1 dump is `/Users/mhehmeyer/Documents/Dev/GalaxyOnFire2/`. All are macOS
paths. This agrees with the darwin NDK package: the original Android build was
driven on macOS.

The Android `.so` has no app source paths (stripped; only `main.c` survives),
so the exact Android project directory is not recoverable. The TU membership
does match the HD line: 103/123 app TU stems of `gof2hd_1.1.19_ios` exist in
`src/`, and the Android additions (`PlayerCreature`, `SpriteGun`, the GLES2
shader zoo, `main.c`, `FileInterfaceAndroid`, `JniBridge`, `NdkHooks`,
`ApkLoader`) are not in the iOS/mac dumps. The closest structural witness is
the mac merged tree (`Classes/Game/...` flat, `MeshMerger.cpp`,
`ParticleSystems/`). That is consistent with the Android build reusing the
same HD/merged source line, but it does **not** prove the Android build was
driven from the same directory or Makefile; a separate Android project
compiling the same `Classes/` tree would look identical in the stripped `.so`.

## 10. Reproducible recipe

### 10.1 Fresh Linux host, exact original

Use the linux NDK r18b for the compiler/linker, but replace its
`libc++abi.a` with the darwin package's archive (the only host-specific input
that changes the gate):

```sh
set -e
ROOT=/path/to/gof2-decomp
NDK=$ROOT/.cache/ndk/android-ndk-r18b
mkdir -p "$ROOT/.cache/ndk"

# 1. linux NDK r18b (compiler, linker, sysroot)
curl -L -o /dev/shm/android-ndk-r18b-linux-x86_64.zip \
  https://dl.google.com/android/repository/android-ndk-r18b-linux-x86_64.zip
unzip -q /dev/shm/android-ndk-r18b-linux-x86_64.zip -d "$ROOT/.cache/ndk"

# 2. darwin libc++abi.a (the original's archive; sha 9a0f50d2...)
curl -L -o /dev/shm/android-ndk-r18b-darwin-x86_64.zip \
  https://dl.google.com/android/repository/android-ndk-r18b-darwin-x86_64.zip
unzip -j /dev/shm/android-ndk-r18b-darwin-x86_64.zip \
  'android-ndk-r18b/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/libc++abi.a' \
  -d "$NDK/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/"
sha256sum "$NDK/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/libc++abi.a"
# 9a0f50d22ef53a946b76d689f723b2ca88d2cb0f7e17ee133ab19395a925fadb

# 3. the repo's matching build
cd "$ROOT"
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8
cmake --build cmake-build-match --target lint  -j8
```

If the worktree is not at the main repo path, override the two path defaults:

```sh
cmake -S . -B cmake-build-match -G Ninja \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
```

The repo's `cmake/toolchain.cmake` already spells the effective compile line
(`-Oz -g -std=c++11 -fPIC -frtti -fstack-protector-strong
-ffunction-sections -funwind-tables -mthumb -mfpu=neon -mfloat-abi=softfp`,
`-fdata-sections` absent); `CMakeLists.txt` links with the original's
`-z now --exclude-libs=libgcc.a --build-id -s`, the zlib/libzip whole
archives, FMOD before `libc++abi.a`, and the section-ordering reconstruction
aid.

### 10.2 Minimal CMake probe (toolchain fingerprint)

```cmake
cmake_minimum_required(VERSION 3.10)
project(t CXX)
add_library(t SHARED t.cpp)
target_link_libraries(t log GLESv2 GLESv1_CM EGL android
  ${APK_LIBS}/libfmodex.so ${APK_LIBS}/libfmodevent.so m dl)
```

```sh
NDK=$ROOT/.cache/ndk/android-ndk-r18b
cmake -S probe -B probe/build -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=$NDK/build/cmake/android.toolchain.cmake \
  -DANDROID_ABI=armeabi-v7a \
  -DANDROID_PLATFORM=android-16 \
  -DANDROID_STL=c++_static \
  -DANDROID_ARM_NEON=ON \
  -DCMAKE_BUILD_TYPE=Release
ninja -C probe/build -t commands t
```

This emits `-std=c++11` from the toolchain default (no `CXX_STANDARD`
override), no `-fdata-sections`, and the CMake link order
`FMOD -> libc++_static.a -> libc++abi.a -> libandroid_support.a ->
libunwind.a`. The resulting probe has `_Unwind_*` UND and no
`__gnu_Unwind_Find_exidx`, matching the original.

### 10.3 ndk-build alternative (not recommended)

If you must use ndk-build, the compile side needs explicit overrides:

```make
APP_ABI := armeabi-v7a
APP_STL := c++_static
APP_PLATFORM := android-16
APP_OPTIM := release
LOCAL_CPP_FEATURES := rtti exceptions
LOCAL_ARM_NEON := true
LOCAL_CFLAGS := -fno-data-sections
```

but the stock ndk-build link order still puts the STL archives before FMOD,
which defines `_Unwind_*` locally and imports `__gnu_Unwind_Find_exidx` — the
opposite of the original. Reproducing the original with ndk-build therefore
requires a non-standard link-order override, which is why CMake is the better
explanation.

### 10.4 Packaging

The observed APK is AGP 3.1.2's packaging shape:

* 4-byte zipalign (`zipalign -f 4`);
* v1 JAR signature + v2 APK Signature Scheme (`apksigner` with
  `--v1-signing-enabled true --v2-signing-enabled true`);
* GNU `arm-linux-androideabi-strip --strip-all` (or `--strip-unneeded`) on the
  native library before packaging;
* deterministic DOS timestamp `1980-00-00` and no central-directory extra
  fields.

The exact Gradle wrapper version and the exact `build.gradle` text are not
recoverable.

## 11. What remains unknowable from the binary

* The exact build-system file names/contents (`CMakeLists.txt`,
  `build.gradle`, `Android.mk`, `Application.mk`, custom Makefile).
* The exact CMake, Gradle, and Ninja versions (AGP 3.1.2 is proven; the
  Gradle wrapper and CMake binary are not).
* The exact host OS version, Xcode version, username, and build directory.
* Whether the native build was standalone CMake or AGP `externalNativeBuild`.
* The exact Android project directory and whether it was the same
  `/Users/hck/.../GalaxyOnFire2_merged` tree or a separate Android project
  compiling the same `Classes/` sources.
* The exact compile-command order and which flags were explicit vs defaults.
* `-fPIC` vs `-fpic` (byte-identical).
* The exact `-g` level (`-g`, `-g2`, `-g3` are byte-identical; `-g0`/`-g1`
  are excluded).
* Whether `-frtti`, `-fexceptions`, `-fno-omit-frame-pointer`,
  `-fno-sized-deallocation`, `-fno-delete-null-pointer-checks`, `-mno-movt`,
  `-fno-vectorize`, etc. were explicit or default (all neutral).
* The exact link-input order beyond what `.dynsym`/`.text` witness; whether
  `libc++_static.a`/`libatomic.a` were on the original link line (no members
  are pulled).
* The exact link-time `-s` choice (the shipped APK is GNU-stripped; the
  ground-truth `.shstrtab` is gold order).
* The original build-id's preimage (the unstripped link output was not
  shipped).
* The cause of the DEX internal SHA-1 mismatch (the APK `MANIFEST.MF` digest
  is valid; w3566).
* The exact strip invocation (`--strip-all` and `--strip-unneeded` produce
  the same file).

## 12. Gate before/after

Before and after this analysis (no source/flag change):

```text
verify: byte 4498  linked 4498  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
=== DRIFT: 0 field(s) across 0 class(es) ===
CLEAN ✓ (no new soft violations; hard invariants 0)
```

`report.json`: `compared 4524`, `size_exact 4524/4524`.
`.text 0x1337c8 @ 0x69440`, `.rodata 0x41b67 @ 0x1c9e10`,
`.data 0x296c @ 0x214000`.

The full CMake default link-flag relink (§6.3) was gate-neutral: same sha256
`88aff115...`, byte 4498, linked 4498, size-exact 4524/4524. No flag change is
landed.
