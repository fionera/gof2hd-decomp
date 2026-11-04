# Build-system provenance from the APK and sibling artifacts (wave 111, w5919)

Status: research note, 2026-11-04. Worktree `/home/debian/GalaxyOnFire2/wt/w5919`,
branch `wave111-w5919`, base `8eea060cc`. Scratch: `/dev/shm/w5919`.
**No `src/`, `tools/`, baseline or allowlist change is landed.** The target for this
worker is `n/a` (no function to improve), and no build-flag experiment produced a
strict gate improvement. This note is the deliverable.

Canonical gate, measured before and after this pass (unchanged):

```text
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.92
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Original sha256 `d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483`,
2,187,328 B. Canonical build sha256 is
`3e11bf924e308657ef6567811e4d5443e26126c474f2f3e9810a7702e871659e`
(`cmake-build-match/libgof2hdaa.so`); its section table is compared in §1.

## 0. Bottom line

* The shipped `libgof2hdaa.so` is an **NDK r18b / clang 7.0.2 / gold 1.12** build:
  API 16, armeabi-v7a, Thumb-2, VFPv3, NEONv1, softfp, static libc++, `-Oz -g`,
  RTTI + exceptions, `-fstack-protector-strong`, `-ffunction-sections`,
  **no `-fdata-sections`**, default visibility.
* It was linked **once** with plain gold
  (`-shared -soname libgof2hdaa.so --build-id -z now --exclude-libs=libgcc.a`),
  then `arm-linux-androideabi-strip --strip-all`, then
  `tools/reorder_shstrtab.py`. The 192 app objects are in `tools/source_order.txt`
  order; `libz_gof2.a`/`libzip_gof2.a` are `--whole-archive`; FMOD is before the
  STL archives.
* The APK is a **2018 Gradle/AGP 3.1.2 release build**: `BUILD_TYPE=release`,
  `DEBUG=false`, `FLAVOR=""`, `VERSION_CODE=47947032`, `VERSION_NAME=2.0.16`,
  `minSdk=16`, `targetSdk=27`, package `net.fishlabs.gof2hdallandroid2012`,
  built `Mon Oct 22 10:18:17 CEST 2018`, signed with the Fishlabs cert
  (v1 `CERT.RSA` and v2 APK Signature Scheme, serial `4C2DC157`).
* The **second official 2.0.16 build** (KiritoJPK archive) is **NDK r14**, not
  "r13b-or-earlier": clang **3.8.275480**, gold **1.11**, gnustl_shared
  (`libstdc++.so`), no `.note.android.ident`, `.ARM.extab` before `.ARM.exidx`,
  `.data`/`.data.rel.ro` align 4, `.init_array` 16 B with a trailing null.
  The libc++abi path is `/Volumes/Android/buildbot/src/android/ndk-r14-release/...`.
  This is the main new discriminator of this pass.
* **No build file survives** in the APK, either IPA, the Windows build, the
  source-layout dumps, or the sibling repos: no `Android.mk`, `Application.mk`,
  `CMakeLists.txt`, `*.cmake`, `*.gradle`, `*.ninja`, `*.o`, `*.a`, `*.d`,
  `*.pbxproj`, `*.xcconfig`, `*.sln`, `*.vcxproj`, `*.pdb`, `*.map`.
* **Source-level flags are the same on every platform**: exceptions ON, RTTI ON,
  threadsafe statics ON, default visibility. The differences between the Android,
  iOS and Windows builds are toolchain/runtime, not source flags.
* Corrections to earlier notes, all measured here:
  * the raw APK lib and `_work/bins/android_2.0.16_libgof2hdaa.so` are the **same
    size**; they differ in **288 bytes** (259 in `.shstrtab`, 29 in the section
    header `sh_name` fields), not "+288 bytes";
  * the "Save fix" lib is the raw APK lib plus **exactly 4 bytes** at `0xcd7b0`,
    the first instruction of `RecordHandler::checkHash(unsigned int)` (not
    `loadOptions`); it makes `checkHash` return 1. Same size, same build-id;
  * `.note.gnu.build-id` differs in **19** bytes, not 20;
  * the 52 `Java_*` exports are **not** the same 52 native methods as the dex:
    3 dex natives are implemented elsewhere and 3 legacy JNI entries are exported
    by the `.so` (§3.4).

## 1. Baseline and section state

```sh
cd /home/debian/GalaxyOnFire2/wt/w5919
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
```

Raw result:

```text
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.92
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Section comparison, original vs canonical build (`/dev/shm/w5919/tools/secdiff.py`):

| section | original | canonical | differing bytes |
|---|---:|---:|---:|
| `.note.android.ident` | 152 | 152 | **0** |
| `.note.gnu.build-id` | 36 | 36 | 19 |
| `.dynsym` | 89,248 | 89,248 | **0** |
| `.dynstr` | 189,208 | 189,208 | **0** |
| `.hash` | 38,716 | 38,716 | **0** |
| `.gnu.version` / `_d` / `_r` | 11,156 / 28 / 64 | same | **0** |
| `.rel.dyn` | 28,744 | 28,744 | **0** |
| `.rel.plt` | 29,392 | 29,392 | **0** |
| `.plt` | 44,108 | 44,108 | **0** |
| `.text` | 1,259,464 | 1,259,464 | 95,999 |
| `.ARM.exidx` | 28,528 | 28,528 | 1,683 |
| `.ARM.extab` | 156,312 | **156,308** | 128,950 |
| `.rodata` | 269,159 | **269,351** | 192 |
| `.fini_array` | 8 | 8 | **0** |
| `.data.rel.ro` | 12,264 | 12,264 | **0** |
| `.init_array` | 12 | 12 | **0** |
| `.dynamic` | 328 | 328 | **0** |
| `.got` | 17,164 | 17,164 | **0** |
| `.data` | 10,604 | 10,604 | **0** |
| `.bss` | 1,748 | 1,748 | 3 (secdiff reads past EOF; artifact) |
| `.comment` | 303 | 303 | **0** |
| `.note.gnu.gold-version` | 28 | 28 | **0** |
| `.ARM.attributes` | 56 | 56 | **0** |
| `.shstrtab` | 279 | 279 | **0** |
| file size | 2,187,328 | 2,187,328 | **0** |

The 27-section set is exact (no extra or missing section); the only differing section bytes are `.text`,
`.ARM.exidx`, `.ARM.extab`, `.rodata` (the `Status::missionCompleted` rank table),
and `.note.gnu.build-id`. `.ARM.extab` is **4 B short** (`0x26294` vs `0x26298`),
as recorded in the wave-110 harvest.

## 2. Artifact inventory and zip-level provenance

### 2.1 The APKPure APK

Path:
`/home/debian/GalaxyOnFire2/Original APK/Galaxy on Fire 2™ HD_2.0.16_APKPure.apk`
(6,389,308 B). The zip central directory is at `6376080`, EOCD at `6389286`.

```sh
python3 - <<'PY'
import struct
p='/home/debian/GalaxyOnFire2/Original APK/Galaxy on Fire 2™ HD_2.0.16_APKPure.apk'
raw=open(p,'rb').read()
i=raw.rfind(b'PK\x05\x06'); cd_off=struct.unpack_from('<I',raw,i+16)[0]
n=struct.unpack_from('<H',raw,i+10)[0]
o=cd_off
for k in range(n):
    fl=struct.unpack_from('<H',raw,o+8)[0]
    method=struct.unpack_from('<H',raw,o+10)[0]
    mt,md=struct.unpack_from('<HH',raw,o+12)
    fnl=struct.unpack_from('<H',raw,o+28)[0]
    exl=struct.unpack_from('<H',raw,o+30)[0]
    cml=struct.unpack_from('<H',raw,o+32)[0]
    lho=struct.unpack_from('<I',raw,o+42)[0]
    name=raw[o+46:o+46+fnl].decode()
    extra=raw[o+46+fnl:o+46+fnl+exl]
    if method==0:
        ln,le=struct.unpack_from('<HH',raw,lho+26)
        do=lho+30+ln+le
        print('stored',name,'dataoff',do,'mod4',do%4)
    o+=46+fnl+exl+cml
PY
```

Measured:

| property | value |
|---|---|
| entries | **143** (99 deflated, 44 stored) |
| general-purpose flags | **0 for all 143** (no data descriptors) |
| extra field | **empty for all 143** (no `0xd935` alignment field) |
| DOS date/time | **0 for all 143** (`1980-00-00 00:00`) |
| stored-entry data offsets | **all `mod 4 == 0`** (44/44) |
| stored `.so` entries | deflated, so alignment does not apply |
| zip comment | none |
| v2 APK Signature Scheme block | **4096 B** total (`size` field 4088) at `6371984` |
| v2 block pairs | `0x7109871a` (v2, 843 B) + `0x42726577` (verity padding, 3205 B) |
| v1 `META-INF/CERT.RSA` | 727 B, serial `4C2DC157` |
| v1 cert validity | `notBefore=2010-07-02 10:37:11 GMT`, `notAfter=2060-06-19 10:37:11 GMT` |
| v1/v2 cert subject | `O=FISHLABS Entertainment GmbH` |
| v2 cert | same serial/validity/subject as v1 |

The v2 block parser:

```python
import struct
raw=open(p,'rb').read()
i=raw.rfind(b'PK\x05\x06'); cd_off=struct.unpack_from('<I',raw,i+16)[0]
j=raw.rfind(b'APK Sig Block 42',0,cd_off)
size=struct.unpack_from('<Q',raw,j-8)[0]
start=cd_off-(size+8)
o=start+8
while o<j-8:
    ln=struct.unpack_from('<Q',raw,o)[0]
    idv=struct.unpack_from('<I',raw,o+8)[0]
    print(hex(idv), ln)
    o+=8+ln
```

The v2 certificate is the first length-prefixed X.509 DER inside the
`0x7109871a` value; `openssl x509 -inform DER -noout -serial -dates -subject`
prints `serial=4C2DC157`, `notBefore=Jul  2 10:37:11 2010 GMT`,
`notAfter=Jun 19 10:37:11 2060 GMT`, `O=FISHLABS Entertainment GmbH`.

The all-zero DOS timestamps and the absence of the AGP `0xd935` extra field are
worth recording: the APK was **not** hand-zipped with normal file mtimes, and it
was **not** processed by the modern `zipalign` that writes `0xd935` alignment
records. All 44 stored entries are nevertheless 4-byte aligned, which is the
minimum `zipalign -p 4` / apksigner alignment guarantee. The verity-padding pair
(`0x42726577`) is written by `apksigner` when it adds the v2 block.

### 2.2 The two IPAs

| artifact | size | files | main binary | arch | minOS / SDK | Xcode / compiler | zip timestamps |
|---|---:|---:|---|---|---|---|---|
| `Galaxy_on_Fire_2__HD_1.1.4_ios_4.3.ipa` | 586,519,994 | 3,180 | 4,983,008 | thin armv7, PIE | 4.3 / 6.0 (`10A403`) | Xcode 4.5 (`0450`, `4G182`), `com.apple.compilers.llvm.clang.1_0` | 1,761 @ 2012-12-04 07:53; 1,415 @ 07:54; 3 @ 2012-12-07 10:06; 1 @ 2012-12-12 15:04 |
| `Galaxy_on_Fire_2__1.0.5_ios_3.0.ipa` | 172,100,849 | 2,105 | 7,839,856 (fat) | armv6 + armv7 | 3.0 / 4.3 (`8F190`) | Xcode 4.0 (`0400`, `4A304a`), `DTCompiler 4.2` (GCC 4.2) | 2,099 @ 2011-04-27 23:05; 3 @ 23:09; 2 @ 2011-01-10 21:10 |

The 1.1.4 `Info.plist` also records `BuildMachineOSBuild=11E53`,
`UIDeviceFamily=[1,2]`, `CFBundleIdentifier=www.fishlabs.net.gof2hd`,
`CFBundleVersion=1.1.4`. The 1.0.5 `Info.plist` records
`BuildMachineOSBuild=10J869`, `UIDeviceFamily=[1,2]`,
`CFBundleIdentifier=www.fishlabs.net.gof2`, `CFBundleVersion=1.0.5`.
The 1.0.5 main binary is a fat Mach-O: armv6 slice at file offset 4096
(3,966,704 B), armv7 slice at 3,973,120 (3,866,736 B). The extracted
`_work/bins/ios_1.0.5_armv7` is the armv7 slice.

### 2.3 The Windows build

`_work/bins/win_GOF2.exe` is a PE32 i386, 2,822,736 B, linker **10.0 (VS2010)**,
timestamp `2012-09-05 16:30:26`, imagebase `0x400000`, 6 sections. The debug
directory has an RSDS entry: GUID `de5e529a5357d147b7939d334ed401d4`, age 2,
PDB path `C:\Users\s.lohse\Desktop\GoF2_PC_REP\GOF2.pdb`.

Imports: `MSVCR100.dll`, `MSVCP100.dll`, `fmod_event.dll`, `steam_api.dll`,
`d3d9.dll`, `d3dx9_43.dll`, `DINPUT8.dll`, `USER32.dll`, `GDI32.dll`,
`COMDLG32.dll`, `SHELL32.dll`, `kernel32.dll`. That is the **dynamic `/MD` CRT**
(not a static CRT), plus FMOD, Steam and D3D9.

Section entropy: `.text` 8.000, `.bind` 0x5c000 B / 7.996, `.rdata` 5.367,
`.data` 3.953, `.rsrc` 7.443, `.reloc` 6.457. The `.text`/`.bind` entropy is
consistent with Steam DRM encryption, so the absence of some imports/strings in
the file is not evidence of absence.

### 2.4 What survives of the build system

A regex scan of all zip entries for
`Android.mk|Application.mk|CMakeLists.txt|*.cmake|*.gradle|*.ninja|*.o|*.a|*.d|*.pbxproj|*.xcconfig|*.sln|*.vcxproj|*.pdb|*.map`
returns **zero hits** in the APK, both IPAs, and the Windows directory. The
source-layout dump (`../source file list dump/`) has three platform lists
(`windows:`, `mac fhd:`, `gof2 hd IOS 1.1.19:`) and **no Android path**. The
`resources.arsc` has 1,562 strings and **no** `cmake`, `ndk`, `Android.mk`,
`externalNative`, `libgof2`, `release`, `debug`, `gradle` or `ninja` string.

## 3. Android APK: Gradle/AGP and dex evidence

### 3.1 `META-INF/MANIFEST.MF`

```text
Manifest-Version: 1.0
Built-By: Generated-by-ADT
Created-By: Android Gradle 3.1.2
```

The `Created-By` string is the only build-system name that survives in the APK.
The `Built-By: Generated-by-ADT` line is the old ADT-compatible header AGP still
wrote in 3.1.2.

### 3.2 `assets/crashlytics-build.properties`

```text
#Mon Oct 22 10:18:17 CEST 2018
version_name=2.0.16
package_name=net.fishlabs.gof2hdallandroid2012
build_id=f527605b-309f-404d-8afc-b68027edc49e
version_code=47947032
app_name=GOF2
```

This is the closest witness to the build date: **2018-10-22 10:18:17 CEST**.

### 3.3 `AndroidManifest.xml` and `BuildConfig`

Parsing the binary AXML string pool and attribute records gives:

```text
manifest:  versionCode=47947032  versionName=2.0.16
           package=net.fishlabs.gof2hdallandroid2012
uses-sdk:  minSdkVersion=16  targetSdkVersion=27
uses-feature: glEsVersion=0x20000 (GLES 2.0)
meta-data: android.support.VERSION=26.1.0
           android.arch.lifecycle.VERSION=27.0.0-SNAPSHOT
```

Parsing the `classes.dex` `class_def` for
`Lnet/fishlabs/gof2hdallandroid2012/BuildConfig;` (source file
`BuildConfig.java`, 6 static fields, 6 static values) gives:

| field | value |
|---|---|
| `APPLICATION_ID` | `net.fishlabs.gof2hdallandroid2012` |
| `BUILD_TYPE` | `release` |
| `DEBUG` | `false` |
| `FLAVOR` | `""` |
| `VERSION_CODE` | `47947032` |
| `VERSION_NAME` | `2.0.16` |

The dex parser used:

```python
import zipfile, struct
d=zipfile.ZipFile(apk).read('classes.dex')
def u32(o): return struct.unpack_from('<I',d,o)[0]
def u16(o): return struct.unpack_from('<H',d,o)[0]
def uleb(p):
    r=s=0
    while True:
        b=d[p]; p+=1; r|=(b&0x7f)<<s; s+=7
        if not b&0x80: return r,p
str_n,str_off=struct.unpack_from('<II',d,56)
type_n,type_off=struct.unpack_from('<II',d,64)
field_n,field_off=struct.unpack_from('<II',d,80)
class_n,class_off=struct.unpack_from('<II',d,96)
def string(i):
    so=u32(str_off+i*4); ln,p=uleb(so); e=d.index(b'\0',p)
    return d[p:e].decode('utf-8','replace')
def typ(i): return string(u32(type_off+i*4))
# find class_def for BuildConfig, read class_data static fields,
# then decode static_values_off (encoded_array) with the dex value types:
# 0x04 int (value_arg = size-1), 0x17 string (index), 0x1f boolean (value_arg),
# 0x1e null.
```

The exact encoded static values are
`06 37f358 37 3f5e 1f 17 00 64 189ddb02 37 6d03`:
`37 f358` = string `0x58f3` = `net.fishlabs...`; `37 3f5e` = string
`0x5e3f` = `release`; `1f` = boolean false; `17 00` = string `""`;
`64 189ddb02` = int `0x02db9d18` = 47947032; `37 6d03` = string `0x036d` =
`2.0.16`. This is a Gradle **release** build with **no flavor**.

### 3.4 `classes.dex` native methods and JNI surface

`classes.dex` is `dex\n035`, 3,723,352 B, 28,895 strings. The class
`Lnet/fishlabs/gof2hdallandroid2012/GOF2HD2012;` has a `<clinit>` with 16
16-bit insns; the sequence is:

```text
const-string v0, "fmodex"      ; invoke-static System.loadLibrary
const-string v0, "fmodevent"   ; invoke-static System.loadLibrary
const-string v0, "gof2hdaa"    ; invoke-static System.loadLibrary
return-void
```

so the load order is **fmodex → fmodevent → gof2hdaa**, matching the
`DT_NEEDED` order and the link line.

There are **52** methods with `ACC_NATIVE` (0x100) in the dex. The `.so` exports
**52** `Java_*` symbols. The two sets are **not equal**:

| dex-only native | implemented by |
|---|---|
| `com.crashlytics.android.ndk.JniNativeApi.nativeInit` | `libcrashlytics.so` |
| `net.fishlabs.gof2hdallandroid2012.GLViewRenderer.setKeyboardStatus` | not in this `.so` |
| `net.fishlabs.googleplay.ToJNI.gof2hd2012setPremiumContent` | not in this `.so` |

| `.so`-only `Java_*` export | note |
|---|---|
| `Java_net_fishlabs_gof2hdallandroid2012_ToJNI_STARTUP` | legacy JNI entry |
| `Java_net_fishlabs_playhaven_ToJNI_getCurrentApplicationModule` | legacy JNI entry |
| `Java_net_fishlabs_tapjoy_ToJNI_spentAmountOfCredits` | legacy JNI entry |

So the earlier "52 = exactly the 52 dex natives" statement is wrong; the
surface is 52 vs 52 with 3 replacements each way. This is a source/JNI-surface
fact, not a build-system fact, but it is a useful discriminator between the
2018 dex and the 2018 `.so`.

### 3.5 Native libraries in the APK

| library | `.comment` | gold | notes |
|---|---|---|---|
| `libgof2hdaa.so` | clang 7.0.2 (4751641) + `GCC: (GNU) 4.9.x 20150123` | **1.12** | the target |
| `libcrashlytics.so` | `GCC: (GNU) 4.9.x 20150123` | **1.11** | Crashlytics SDK prebuilt, older NDK |
| `libfmodex.so` | `GCC: (GNU) 4.4.3` (×4) | — | FMOD prebuilt |
| `libfmodevent.so` | `GCC: (GNU) 4.4.3` (×4) | — | FMOD prebuilt |

`libcrashlytics.so` being gold 1.11 / GCC 4.9 while `libgof2hdaa.so` is gold
1.12 / clang 7.0.2 shows the app's native libs were built at different times and
with different toolchains; only `libgof2hdaa.so` is the r18b rebuild.

### 3.6 The raw APK lib vs the ground-truth file

The APK's `lib/armeabi-v7a/libgof2hdaa.so` is **2,187,328 B**, the same size as
`_work/bins/android_2.0.16_libgof2hdaa.so`. It differs from the ground truth in
**288 bytes**:

* 259 bytes in `.shstrtab` (`0x215af0..0x215c07`): the APK lib has the raw
  gold Stringpool order; the ground truth has the order restored by
  `tools/reorder_shstrtab.py`;
* 29 bytes in the section header table (`0x215c08..0x216040`): the `sh_name`
  fields that point into the reordered `.shstrtab`.

So the ground truth is the APK lib **after** `tools/reorder_shstrtab.py`, not
the raw APK lib plus 288 bytes.

The "Save fix" lib (`kirito/savefix/lib/armeabi-v7a/libgof2hdaa.so`) is the raw
APK lib plus **exactly 4 bytes** at `0xcd7b0`:

```text
ground truth / APK lib:  f0 b5 03 af   ; push {r4,r5,r6,r7,lr}; add r7,sp,#12
save fix:                01 20 70 47   ; movs r0,#1; bx lr
```

`0xcd7b0` is the first instruction of `RecordHandler::checkHash(unsigned int)`
(`_ZN13RecordHandler9checkHashEj`, symbol value `0xcd7b1` because of the Thumb
bit, size `0x118`), not `loadOptions` (`_ZN13RecordHandler11loadOptionsEv`,
`0xcd451`, size `0x360`, which ends at `0xcd7b0`). The patch makes `checkHash`
always return `true` — the "Save fix". The savefix lib vs the ground truth has
**292** differing bytes (4 + 288), the same size and the same build-id
`c011810da45777b83ed721b2a87d517efed0d310`.

## 4. Ground-truth ELF: toolchain and link recipe

### 4.1 Hard metadata

| observable | value | conclusion |
|---|---|---|
| ELF | `ELF32 DYN ARM EABI5`, `e_flags 0x5000200` | armeabi-v7a **softfp** |
| `.note.android.ident` | API `16`, `r18b`, build `5063045` | NDK **r18b**, API 16 |
| `.comment` | clang 7.0.2 `4751641 based on r328903` + `GCC: (GNU) 4.9.x 20150123` | clang app TUs + GCC 4.9 libcrypto |
| `.note.gnu.gold-version` | `gold 1.12` | NDK gold linker |
| `.ARM.attributes` | 56 B (hex below) | ARMv7-A, Thumb-2, VFPv3, NEONv1, GOT-indirect, wchar_t 4, enum int, 8-byte align, unaligned v6 |
| `DT_NEEDED` | `liblog libGLESv2 libGLESv1_CM libEGL libandroid libfmodex libfmodevent libm libdl libc` | link-line order |
| `DT_FLAGS` / `DT_FLAGS_1` | `BIND_NOW` / `NOW` | `-z now` |
| `DT_SONAME` | `libgof2hdaa.so` | `-soname libgof2hdaa.so` |
| `.hash` only | no `.gnu.hash` | gold `--hash-style=sysv` default |
| defined / undefined dynsym | 5370 / 207 (raw 208 incl. null) | symbol parity 5370/5370 |
| `.init_array` | 12 B: `0x69441, 0x694c1, 0x694dd` | 3 initializers |
| `.fini_array` | 8 B: `0x69548, 0x69534` | 2 finalizers |

`.ARM.attributes` (56 B):

```text
4137000000616561626900012d0000000541524d20763700060a0741080109020a030c011102
120414011501170318011a021b0322012601
```

Decoded: `Tag_CPU_name="ARM v7"`, `Tag_CPU_arch=v7`, `Tag_CPU_arch_profile=Application`,
`Tag_ARM_ISA_use=Yes`, `Tag_THUMB_ISA_use=Thumb-2`, `Tag_FP_arch=VFPv3`,
`Tag_Advanced_SIMD_arch=NEONv1`, `Tag_ABI_PCS_GOT_use=GOT-indirect`,
`Tag_ABI_PCS_wchar_t=4`, `Tag_ABI_FP_denormal=Needed`,
`Tag_ABI_FP_exceptions=Needed`, `Tag_ABI_FP_number_model=IEEE 754`,
`Tag_ABI_align_needed=8-byte`, `Tag_ABI_enum_size=int`,
`Tag_ABI_HardFP_use=Deprecated`, `Tag_CPU_unaligned_access=v6`,
`Tag_ABI_FP_16bit_format=IEEE 754`. There is a `Tag_CPU_name` = `"ARM v7"`,
but **no** `Tag_ABI_optimization_goals` (0x1e) and **no**
`Tag_Virtualization_use` (0x44). Those absences are toolchain/linker
fingerprints (see §5.2).

### 4.2 Exceptions / RTTI / visibility / threadsafe statics

| property | evidence | verdict |
|---|---|---|
| exceptions | `__gxx_personality_v0` defined; `__cxa_throw` defined; 4 `__cxa_throw` calls; 12 `__cxa_begin_catch`; 10 `__cxa_end_catch`; 3 `__cxa_allocate_exception`; 1 `__cxa_rethrow`; 3,294 LSDA entries; `.ARM.extab` 156,312 B | **ON** (`-fexceptions`) |
| RTTI | 176 `_ZTI`, 99 `_ZTV`, 176 `_ZTS`; `__dynamic_cast` defined and called 21 times (14 PLT + 7 direct) | **ON** (`-frtti`) |
| visibility | 5,370 defined dynsym, all app symbols `GLOBAL DEFAULT`; no `HIDDEN` app symbols | **default** (`-fvisibility=default`) |
| threadsafe statics | `__cxa_guard_acquire`/`release`/`abort` defined (libc++abi); 2 `__cxa_guard_acquire` calls, both in `GameText::getText` | **ON** |
| stack protector | `__stack_chk_fail` 787 calls, `__stack_chk_guard` UND | **ON** (`-fstack-protector-strong`) |
| C++ runtime | `libc++abi.a` + `libandroid_support.a` + `libgcc.a`; no `libstdc++.so` in `DT_NEEDED` | **static libc++** |
| unwind | `_Unwind_DeleteException`, `_Unwind_GetLanguageSpecificData`, `_Unwind_GetRegionStart`, `_Unwind_RaiseException`, `_Unwind_Resume`, `_Unwind_VRS_Get`, `_Unwind_VRS_Set`, `__gnu_unwind_frame` all UND | libunwind.a not pulled |
| integer helpers | `__aeabi_idiv` 2 UND; no `__aeabi_ldivmod` | libgcc.a not pulled for these |

### 4.3 The link recipe

The canonical link (from `CMakeLists.txt` and `tools/source_order.txt`) is:

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

then:

```sh
arm-linux-androideabi-strip --strip-all -o libgof2hdaa.so libgof2hdaa.unstripped.so
python3 tools/reorder_shstrtab.py libgof2hdaa.unstripped.so libgof2hdaa.so
```

The `.comment` order is crtbegin (clang 7.0.2) → app objects (clang 7.0.2) →
`libcrypto_gof2.a` (GCC 4.9). The r18b `crtbegin_so.o` is byte-identical in its
`.note.android.ident` to the original (152/152 bytes), and its `.comment` is the
264-byte clang 7.0.2 string; the 40-byte GCC string is from
`libcrypto_gof2.a(sha256.o)`.

### 4.4 The two CMake-vs-ndk-build discriminators

These are reproduced from the wave-101/106 notes and were not changed by this
pass:

| setting | CMake r18b default | ndk-build r18b default | original | verdict |
|---|---|---|---|---|
| `-fdata-sections` | **no** | **yes** | **no** | **CMake** |
| STL archives after user libs | **yes** | **no** (static before shared) | **yes** | **CMake** |
| `-ffunction-sections` | yes | yes | yes | both |
| `-fstack-protector-strong` | yes | yes | yes | both |
| `-funwind-tables` | yes | yes | yes | both |
| `-g` | yes | yes | yes | both |
| `ANDROID_STL` | `c++_static` | `system` (libstdc++) | static libc++ | CMake default; ndk-build needs `APP_STL := c++_static` |
| `ANDROID_ARM_NEON` | `FALSE` for API < 23 | `LOCAL_ARM_NEON` false | NEONv1 | explicit ON in both |
| C++ features | empty | empty | RTTI + exceptions | explicit in both |
| build type | empty | `release` | `-Oz -DNDEBUG` | explicit Release in CMake |

Measured A/B (wave 101/106): a real ndk-build default build gives
**byte 1345 / linked 3843**; the canonical CMake build gives **4500 / 4500**.
Adding `-fdata-sections` drops the gate to **4311 / 4500**, changes `.rodata`
from `0x41b67` to `0x41a07`, and changes `.data`/`.data.rel.ro`. Removing
`-ffunction-sections` drops to **1345 / 4357**, shrinks `.text` by `0x670`, and
cuts CANTUNWIND sentinels from **272 to 83** (the 193 empty-`.text` sentinels
that share a real function address drop from **193 to 51**). The full ndk-build
r18b compile set **minus `-fdata-sections`** is byte-identical to the canonical
build except the 20-byte `.note.gnu.build-id`; the full ndk-build r18b link
*flag* set is byte-identical to the canonical link.

## 5. The second official 2.0.16 build: NDK r14, not r13b

### 5.1 Artifact and metadata

The KiritoJPK archive
`KiritoJPK_Galaxy-on-Fire-2-FULL-HD-Android/GOF2FHD_APK.7z` contains 14 APKs.
Thirteen ship a different `libgof2hdaa.so`; six are identical copies of the old
build (2,469,656 B, sha256
`9a166d19b9fde9552e2b061183909fa2f8d86467b69713403a33a4612f402fad`), six are
Bloom patches of it, and one is a New-system patch. The remaining APK
(`Save fix/...`) ships the r18b build with the 4-byte `checkHash` patch (§3.6).

The old lib's metadata:

| observable | value | conclusion |
|---|---|---|
| `.comment` | `\0GCC: (GNU) 4.9.x 20150123 (prerelease)\0Android clang version 3.8.275480  (based on LLVM 3.8.275480)\0` | clang **3.8.275480** |
| libc++abi path | `/Volumes/Android/buildbot/src/android/ndk-r14-release/external/libcxx/../../external/libcxxabi/src/abort_message.cpp` | **NDK r14** |
| `.note.gnu.gold-version` | `gold 1.11` | older gold |
| `.note.android.ident` | **absent** | r14 crtbegin has no Android note |
| `DT_NEEDED` | `... libm libstdc++ libdl libc` | **gnustl_shared** |
| section order | `.ARM.extab` **before** `.ARM.exidx` | r14 layout |
| `.data` / `.data.rel.ro` align | 4 / 4 | r14 layout (GT: 16 / 16) |
| `.init_array` | 16 B: `0x61951, 0x619d5, 0x619f1, 0x0` | r14 crtbegin/crtend pair |
| `.fini_array` | 8 B: `0x61940, 0x0` | r14 pair |
| `.ARM.attributes` | 60 B (hex below) | has `1e 02` and `44 01` |
| defined / undefined dynsym | 5093 / 204 (raw 205 incl. null) | 277 fewer defined than GT |

`clang 3.8.275480` is the **r14** clang; r13b is `3.8.256229`. The
`/ndk-r14-release/` path is the decisive string. The earlier "r13b-or-earlier"
guess is refuted.

### 5.2 `.ARM.attributes` as a toolchain fingerprint

| build | size | hex | tags of interest |
|---|---:|---|---|
| GT (r18b, clang 7.0.2) | 56 | `4137...1a021b0322012601` | no `1e`, no `44` |
| old (r14, clang 3.8) | 60 | `413b...1a021b031e02220126014401` | `1e 02` (optimization goals = Aggressive Speed), `44 01` (virtualization = TrustZone) |
| our r13b hybrid | 58 | `4139...1a021b03220126014401` | `44 01`, no `1e` |

Full hex:

```text
old r14:  413b00000061656162690001310000000541524d20763700060a0741080109020a030c011102120414011501170318011a021b031e02220126014401
r13b hyb: 4139000000616561626900012f0000000541524d20763700060a0741080109020a030c011102120414011501170318011a021b03220126014401
```

Controlled probes with the r18b clang 7.0.2 and r13b clang 3.8:

```sh
# r18b clang 7.0.2, -Oz
clang -c -Oz -target armv7-none-linux-androideabi16 \
  -march=armv7-a -mthumb -mfpu=neon -mfloat-abi=softfp t.c -o t-r18b.o
# r13b clang 3.8.256229, -O2/-Os/-Oz
clang -c -O2 -target armv7-none-linux-androideabi16 \
  -march=armv7-a -mthumb -mfpu=neon -mfloat-abi=softfp t.c -o t-r13b.o
```

The r18b clang 7 object has no `Tag_Virtualization_use`; the r13b clang 3.8
object has `44 01` (and `Tag_CPU_name "cortex-a8"`/`Tag_conformance "2.09"`;
the final link's `Tag_CPU_name` is `"ARM v7"`, and `Tag_conformance` is
dropped). The old lib's `44 01` therefore points at clang 3.8 (r13b/r14).
The `1e 02` is present in `libcrypto_gof2.a(sha256.o)` (GCC 4.9, `-O2`,
`Tag_ABI_optimization_goals: Aggressive Speed`); the old gold 1.11 keeps it,
while the r18b gold 1.12 link drops it even though GT links the same libcrypto.
So the `.ARM.attributes` merge itself is a gold-11-vs-gold-12 fingerprint,
independent of the clang version.

### 5.3 Section and symbol comparison

| section | GT (r18b) | old (r14) | our r13b hybrid |
|---|---:|---:|---:|
| `.text` | 1,259,464 | 1,475,272 | 1,531,352 |
| `.ARM.exidx` | 28,528 | 24,504 | 25,888 |
| `.ARM.extab` | 156,312 | 259,472 | 260,448 |
| `.rodata` | 269,159 | 272,340 | 273,751 |
| `.data` | 10,604 | 10,564 | 10,588 |
| `.data.rel.ro` | 12,264 | 8,204 | 12,248 |
| `.got` | 17,164 | 15,296 | 14,884 |
| `.dynsym` | 89,248 | 84,768 | 85,424 |
| `.dynstr` | 189,208 | 178,923 | 180,358 |
| `.hash` | 38,716 | 37,596 | 37,760 |
| `.init_array` | 12 | 16 | 12 |
| `.fini_array` | 8 | 8 | 8 |
| `.comment` | 303 | 101 | 364 |
| `.ARM.attributes` | 56 | 60 | 58 |
| `.note.android.ident` | 152 | absent | 152 |
| `.shstrtab` | 279 | 259 | 279 |
| file size | 2,187,328 | 2,469,656 | 2,539,632 |

| property | GT (r18b) | old (r14) | our r13b hybrid |
|---|---:|---:|---:|
| defined / UND dynsym | 5370 / 207 | 5093 / 204 | 5127 / 211 |
| `_ZTI` / `_ZTV` / `_ZTS` | 176 / 99 / 176 | 175 / 98 / 175 | 176 / 99 / 176 |
| `_ZGV` | 0 | 0 | 0 |
| `.ARM.exidx` entries / CANT / LSDA / dup / CANT-sharing-real | 3566 / 272 / 3294 / 194 / 193 | 3063 / 285 / 2369 / 208 / 207 | 3236 / 274 / 2645 / 161 / 161 |
| `__cxa_guard_acquire` calls | 2 | 2 | 2 |
| `__cxa_atexit` calls | 1 | 2 | — |
| `__stack_chk_fail` calls | 787 | 706 | — |
| `__dynamic_cast` calls (PLT + direct) | 21 (14+7) | 23 (16+7) | — |
| `__cxa_throw` calls | 4 | 4 | — |
| `__cxa_begin_catch` / `__cxa_end_catch` | 12 / 10 | 12 / 10 | — |
| `__cxa_allocate_exception` / `__cxa_rethrow` | 3 / 1 | 4 / 1 | — |
| `__aeabi_ldivmod` | UND (not pulled) | **defined locally** | — |
| `__aeabi_idiv0` / `__aeabi_ldiv0` | UND | **defined locally** | — |

The 277 extra GT defined symbols are dominated by `ArrayAdd`/`ArrayRemoveAll`/
`ArrayRelease`/`ArraySet` template instantiations, shader destructors
(`_ZN11AbyssEngine10BlurShaderD0Ev`, …), and 2 RTTI classes:
`_ZTI7Mission`, `_ZTV7Mission`, `_ZTS7Mission` and
`_ZTIN11AbyssEngine6StringE`, `_ZTVN11AbyssEngine6StringE`,
`_ZTSN11AbyssEngine6StringE`. The old lib instead exports the libstdc++
`_ZTIN10__cxxabiv130__qualified_function_type_infoE` and
`_ZSt17__throw_bad_allocv`, `_ZSt7nothrow`.

The `Mission`/`String` RTTI difference is **not** evidence of RTTI off in the
old build: the type-name strings `7Mission` and `N11AbyssEngine6StringE` are
present in both binaries (70/118 occurrences in the old lib), and the old lib
has 175 `_ZTI`/`_ZTS` and 98 `_ZTV`. It is an export/visibility or
typeid-use difference between the two builds, not a `-fno-rtti` difference.
The old lib's local `__aeabi_ldivmod`/`idiv0`/`ldiv0` are the expected
consequence of ndk-build's static-before-shared link order pulling `libgcc.a`
members; the GT's `_Unwind_*`/`__aeabi_*` UND set is the expected consequence of
the CMake link order (STL archives after user libs, `libgcc.a` only pulls what
is referenced).

### 5.4 The old APK's zip is a repack, not primary build evidence

`kirito/GOF2_2.0.16_Original_Price.apk` has 27 entries: 26 deflated + 1 stored,
22 entries with flag `0x808` (data descriptor + UTF-8), 4 with `0x800`, 1 with
0. Timestamps are 2023-04-06 12:33:36 (23 entries) and 2024-04-24 15:54:52
(4 entries: the `.so`, `MANIFEST.MF`, etc.). `META-INF/MANIFEST.MF` has no
`Created-By` line. This is a fan repack of the old official lib, so its zip
timestamps are not build dates; the lib itself is the official r14 build.

## 6. iOS and Windows: source-level flags

### 6.1 iOS 1.1.4 HD

Mach-O armv7, PIE, `LC_VERSION_MIN_IPHONEOS 4.3`, SDK 6.0 (`10A403`),
UUID `F0BB0EED-7D47-32E5-8462-333D44A50807`, Xcode 4.5 (`0450`, `4G182`),
`DTCompiler com.apple.compilers.llvm.clang.1_0`, `BuildMachineOSBuild 11E53`.
Dylibs: `/usr/lib/libstdc++.6.dylib`, `/usr/lib/libgcc_s.1.dylib`,
`/usr/lib/libSystem.B.dylib`, `/usr/lib/libobjc.A.dylib`, plus frameworks.
**No libc++.**

Symbols: 665 `T`, 47 `D`, 2 `S`; `__ZTV` 35, `__ZTI` 2, `__ZTS` 2, `__ZGV` 6.
Undefined: `__gxx_personality_sj0`, `__Unwind_SjLj_Register`,
`__Unwind_SjLj_Resume`, `__Unwind_SjLj_Unregister`, `__cxa_atexit`,
`__cxa_guard_acquire`, `__cxa_guard_release`, `__cxa_pure_virtual`,
`__stack_chk_fail`, `__stack_chk_guard`. The game type-name strings
`15IParticleSystem`, `20ParticleSystemSprite`, `18ParticleSystemMesh`,
`11AbstractGun`, `11BoundingAAB`, `13PlayerFighter` are present.

Verdict: **exceptions ON (sjlj), RTTI ON, threadsafe statics ON, stack
protector ON, libstdc++.6 (GCC-era ABI), default visibility, no libc++.**

### 6.2 iOS 1.0.5

Fat armv6 + armv7; armv7 slice UUID
`84309E35-1163-37B8-A40A-CF597041DD91`, `LC_VERSION_MIN_IPHONEOS 3.0`,
SDK 4.3 (`8F190`), Xcode 4.0 (`0400`, `4A304a`), `DTCompiler 4.2` (GCC 4.2),
`BuildMachineOSBuild 10J869`. Dylibs: `libstdc++.6.dylib`,
`libgcc_s.1.dylib`, `libSystem.B.dylib`, `libobjc.A.dylib`, plus frameworks.

Symbols (armv7 slice): `__ZTV` 6, `__ZTI` 3, `__ZTS` 3, `__ZGV` 0. Undefined:
`__gxx_personality_sj0`, `__Unwind_SjLj_Register/Resume/Unregister`,
`__cxa_atexit`, `__cxa_guard_acquire`, `__cxa_guard_release`,
`__cxa_pure_virtual`, `__cxa_begin_catch`, `__cxa_end_catch`,
`__cxa_rethrow`, `__cxa_call_unexpected`. No `__stack_chk_fail`.

Verdict: **exceptions ON (sjlj), RTTI ON, threadsafe statics ON, libstdc++.6
(GCC 4.2), no observed stack protector.**

### 6.3 Windows (VS2010)

PE32 i386, linker 10.0, timestamp 2012-09-05, `/MD` CRT
(`MSVCR100.dll`, `MSVCP100.dll`). Imports `__CxxFrameHandler3`,
`_CxxThrowException`, `_except_handler4_common`, `_purecall`, type_info dtor,
exception ctors → **exceptions ON, RTTI ON**. 62 `.?AV` type-name strings.
No `__security_cookie`/`__security_check_cookie`/`_Init_thread_header`/
`_RTC_`/`_CrtDbgReport` imports; the `.text`/`.bind` entropy (8.000/7.996)
indicates Steam DRM encryption, so the absence of those imports is not
conclusive. No `??_R0` strings because the COFF symbol table is stripped, not
because RTTI is off.

### 6.4 Cross-platform source-flag table

| flag | Android GT (r18b) | Android old (r14) | iOS 1.1.4 | iOS 1.0.5 | Windows VS2010 |
|---|---|---|---|---|---|
| exceptions | ON (`__gxx_personality_v0`, `__cxa_throw`, 3294 LSDA) | ON (`__gxx_personality_v0`, `__cxa_throw`, 2369 LSDA) | ON sjlj (`__gxx_personality_sj0`) | ON sjlj | ON (`__CxxFrameHandler3`, `_CxxThrowException`) |
| RTTI | ON (176 `_ZTI`, 99 `_ZTV`, 176 `_ZTS`) | ON (175/98/175) | ON (2 `_ZTI`, 35 `_ZTV`; type-name strings) | ON (3/6/3) | ON (62 `.?AV` names) |
| visibility | default (5,370 defined) | default (5,093) | default (665 `T`, 47 `D`, 2 `S`; rest local/stripped) | default | N/A |
| threadsafe statics | ON (2 `__cxa_guard_acquire` calls) | ON (2 calls) | ON (`__cxa_guard_acquire/release` UND) | ON | unobservable (VS2010 CRT; no `_Init_thread_header` import) |
| stack protector | ON (787 `__stack_chk_fail`) | ON (706) | ON (`__stack_chk_fail/guard` UND) | not observed | unobservable (DRM) |
| C++ runtime | static libc++ (`libc++abi.a` + `libandroid_support.a` + `libgcc.a`) | gnustl_shared (`libstdc++.so`) | libstdc++.6 + libgcc_s.1 | libstdc++.6 + libgcc_s.1 | MSVCR100/MSVCP100 (`/MD`) |
| std | C++11 | C++11 (gnustl) | GCC 4.2 libstdc++ | GCC 4.2 libstdc++ | VS2010 |

The important conclusion: **the source-level flags do not change across
platforms or across the two Android builds.** The differences are the C++
runtime, the exception unwinder (EHABI vs sjlj vs SEH), the linker, and the
optimization level.

## 7. Controlled flag probes

The following probes were run with the r18b clang 7.0.2 (the canonical compiler)
on a small `lib2.cpp`:

```cpp
extern "C" int rand(void);
struct Base { virtual ~Base(); virtual int f(); };
Base::~Base() {}
int Base::f() { return 1; }
struct Derived : Base { int f() override; };
int Derived::f() { return 2; }
struct WithDtor { ~WithDtor(); };
WithDtor::~WithDtor() {}
static WithDtor g_obj;
int counter() { static int x = rand(); return x; }
int use() { return counter(); }
int guarded(int n) { char buf[64]; buf[0] = (char)n; return buf[0]; }
int maythrow(int n) { try { if (n) throw n; } catch (int) { return 1; } return 0; }
int dyn(Base* b) { return dynamic_cast<Derived*>(b) ? 1 : 0; }
int main() { return use() + guarded(1) + maythrow(0) + dyn(0); }
```

```sh
clang++ -c -target armv7-none-linux-androideabi16 -march=armv7-a -mthumb \
  -mfpu=neon -mfloat-abi=softfp -std=c++11 -fPIC -frtti -fexceptions \
  -fstack-protector-strong -Oz -g -DANDROID -DNDEBUG \
  -isystem <r18b>/sources/cxx-stl/llvm-libc++/include \
  -isystem <r18b>/sources/cxx-stl/llvm-libc++abi/include \
  lib2.cpp -o lib2.o
```

| variant | `__cxa_guard_acquire` | `__gxx_personality_v0` | `_ZTI` | `HIDDEN` app symbols |
|---|---:|---:|---:|---:|
| base | 1 | 1 | 3 | 0 |
| `-fno-threadsafe-statics` | **0** | 1 | 3 | 0 |
| `-fvisibility=hidden` | 1 | 1 | 3 | **20** |
| `-fno-use-cxa-atexit` | 1 | 1 | 3 | 0 |

`-fno-exceptions`/`-fno-rtti` cannot compile this probe (it uses `throw` and
`dynamic_cast`), which is itself the point: the original uses both features, so
its `__gxx_personality_v0`/`_ZTI`/`_ZTV`/`__dynamic_cast` set is the expected
result of `-frtti -fexceptions`.

The `-g` level and CMake-version probes are reproduced from wave 106:
`-g`, `-g2`, `-g3`, `-gdwarf-4` are identical except `.note.gnu.build-id`;
`-g0`, `-g1`, `-gline-tables-only` drop to byte 2069 / linked 4247 / size 4485
with 39 function-size mismatches and `.text` still size-exact `0x1337c8`.
CMake 3.6.0-rc2, 3.20.6, 3.25.3 and 3.31.6 emit identical compile/link flags
from the r18b `android.toolchain.cmake`; repo builds under 3.20.6 and 3.25.3
are byte-identical to the canonical build except `.note.gnu.build-id`.

## 8. Confidence-ranked recipe table

### 8.1 Proven (direct binary/APK evidence)

| item | value | evidence |
|---|---|---|
| NDK | **r18b** | `.note.android.ident` = API 16, `r18b`, `5063045`; byte-identical to r18b `crtbegin_so.o` |
| clang | **7.0.2 (4751641 based on r328903)** | `.comment` |
| linker | **gold 1.12** | `.note.gnu.gold-version` |
| ABI | armeabi-v7a, Thumb-2, VFPv3, NEONv1, softfp | `.ARM.attributes`, `e_flags 0x5000200` |
| API | 16 | `.note.android.ident` |
| optimization | `-Oz` | code size / `.text` 1,259,464; `-O2`/`-Os` builds are larger |
| debug | `-g` (stripped) | `.comment`/build-id behavior; `-g0` drops the gate |
| PIC | `-fPIC` | GOT-indirect, shared object |
| exceptions | `-fexceptions` | `__gxx_personality_v0`, `__cxa_throw`, 3294 LSDA |
| RTTI | `-frtti` | 176 `_ZTI`, 99 `_ZTV`, 176 `_ZTS`, `__dynamic_cast` |
| stack protector | `-fstack-protector-strong` | 787 `__stack_chk_fail` calls |
| function sections | `-ffunction-sections` | 272 CANTUNWIND / 193 empty-`.text` sentinels; removing it drops the gate |
| data sections | **no `-fdata-sections`** | CMake r18b default; adding it drops the gate |
| visibility | default | 5,370 defined dynsym, all `GLOBAL DEFAULT` |
| C++ runtime | **static libc++** (`libc++abi.a` + `libandroid_support.a` + `libgcc.a`) | no `libstdc++.so`; `__cxa_guard_*`/`__gxx_personality_v0` defined |
| macros | `-DANDROID -DNDEBUG` | r18b CMake default; **likely**, not directly observable |
| link | one plain gold link, `-shared -soname libgof2hdaa.so --build-id -z now --exclude-libs=libgcc.a` | `.dynamic`, `.note.gnu.gold-version`, `.hash` only |
| link order | crtbegin, 192 app objects in `tools/source_order.txt`, `--whole-archive libz_gof2.a libzip_gof2.a --no-whole-archive`, `-l log -l GLESv2 -l GLESv1_CM -l EGL -l android`, `libfmodex.so libfmodevent.so`, `libcrypto_gof2.a`, `-l m`, `libc++abi.a libandroid_support.a libgcc.a`, `-l dl -l c`, crtend | `.comment` order, `DT_NEEDED` order, section sizes |
| post-link | `strip --strip-all` then `tools/reorder_shstrtab.py` | no `.debug_*`; APK lib vs GT differs only in `.shstrtab`/`sh_name` |
| Gradle | **AGP 3.1.2** | `META-INF/MANIFEST.MF`: `Created-By: Android Gradle 3.1.2` |
| build type | **release**, no flavor, `DEBUG=false` | `BuildConfig` static values |
| version | `2.0.16`, `47947032` | `BuildConfig`, manifest, Crashlytics properties |
| package | `net.fishlabs.gof2hdallandroid2012` | `BuildConfig`, manifest |
| SDK | min 16, target 27 | manifest |
| build timestamp (Crashlytics) | **2018-10-22 10:18:17 CEST** | `assets/crashlytics-build.properties` |
| signing | v1 + v2, Fishlabs cert serial `4C2DC157` | `META-INF/CERT.RSA`, v2 block |
| dex | single `classes.dex`, 52 native methods, load order fmodex → fmodevent → gof2hdaa | dex parser |
| second build | **NDK r14**, clang **3.8.275480**, gold **1.11**, gnustl_shared | `.comment`, libc++abi path `/ndk-r14-release/`, `DT_NEEDED libstdc++.so` |
| iOS 1.1.4 | Xcode 4.5, SDK 6.0, Apple clang, libstdc++.6, sjlj | `Info.plist`, load commands, symbols |
| iOS 1.0.5 | Xcode 4.0, SDK 4.3, GCC 4.2, libstdc++.6, sjlj | `Info.plist`, load commands, symbols |
| Windows | VS2010, `/MD`, Steam DRM | PE header, imports, `.text`/`.bind` entropy |

### 8.2 Likely (inference from defaults + evidence)

| item | value | why |
|---|---|---|
| build system | **CMake** with the r18b `android.toolchain.cmake` defaults | no `-fdata-sections`; STL archives after user libs |
| CMake version | **≥ 3.6** (AGP 3.1.2 bundles 3.6.4111459) | AGP 3.1.2 era; CMake version is codegen-neutral |
| generator | **Ninja** | AGP 3.1.2 externalNativeBuild default; unobservable in the artifact |
| build type | `CMAKE_BUILD_TYPE=Release` | `-Oz -DNDEBUG` |
| ABI | `ANDROID_ABI=armeabi-v7a` | `e_flags`, `.ARM.attributes` |
| platform | `ANDROID_PLATFORM=android-16` | `.note.android.ident` |
| STL | `ANDROID_STL=c++_static` | static libc++ |
| NEON | `ANDROID_ARM_NEON=ON` | NEONv1 in `.ARM.attributes` |
| C++ features | `ANDROID_CPP_FEATURES="rtti exceptions"` | `-frtti -fexceptions` |
| host | **macOS** | the NDK package's libc++abi was built on darwin (`/Volumes/Android/...`); the app host is not directly observable |
| `-Wl,--no-undefined` | present | CMake r18b default; unobservable |
| `-Wl,--fix-cortex-a8` | present | CMake r18b default; unobservable |
| `--exclude-libs=libatomic.a/libunwind.a` | present | CMake r18b default; unobservable |
| `-std=c++11` | present | CMake r18b `c++_static` default; code is C++11 |
| `-fPIC` vs `-fpic` | `-fPIC` | CMake r18b default |
| `-DANDROID -DNDEBUG` | present | CMake r18b default |
| `-Wl,--build-id` | present | `.note.gnu.build-id` |
| `-Wl,-z,noexecstack -z relro -z now` | present | `DT_FLAGS`, PT_GNU_RELRO, PT_GNU_STACK |
| `-Wl,--hash-style=sysv` | present | `.hash` only |
| `-Wl,-soname,libgof2hdaa.so` | present | `DT_SONAME` |

### 8.3 Unobservable (no surviving evidence)

| item | why |
|---|---|
| exact `CMakeLists.txt` text, project/source dir names | no build file survives |
| exact CMake version / generator | CMake version is codegen-neutral; Ninja vs Make is not in the ELF |
| host OS (Linux vs macOS) | the libc++abi path is from the NDK package, not the build host; macOS is likely but not proven |
| whether `libc++_static.a` / `libunwind.a` / `libatomic.a` were named on the link line | `_Unwind_*` are UND, so `libunwind.a` was not pulled; `libatomic.a` is not referenced; the exact line is not recoverable |
| `-DNDEBUG` / `-DANDROID` macros | no `assert`/`__ANDROID__` string survives |
| `-g` level (`-g`, `-g2`, `-g3`, `-gdwarf-4`) | stripped; all are codegen-identical except build-id |
| `-fPIC` vs `-fpic` | both produce the same code at this size |
| `-std=c++11` vs `-std=gnu++11` | no distinguishing runtime symbol |
| `-Wl,--no-undefined` | link succeeds either way |
| `--fix-cortex-a8` | no observable relocation difference |
| `--exclude-libs=libatomic.a/libunwind.a` | no pulled members |
| APK signing / zipalign configuration | only the result (v1+v2, 4-byte alignment) survives |
| exact native build date/time | the Crashlytics timestamp is the closest witness, not the native link time |
| why `Mission`/`AbyssEngine::String` RTTI are exported in GT but not old | could be a source `typeid`/`dynamic_cast` change, a visibility change, or a link/COMDAT difference; the type-name strings are present in both |
| why the old lib defines `__aeabi_ldivmod` locally | consistent with ndk-build static-before-shared link order, but the exact link line is not recoverable |

## 9. Historical arc

1. **2011-04-27 — iOS 1.0.5**: Xcode 4.0, GCC 4.2, SDK 4.3, fat armv6+armv7,
   libstdc++.6, sjlj.
2. **2012-09-05 — Windows/Steam**: VS2010, `/MD`, FMOD, Steam DRM, PDB
   `C:\Users\s.lohse\Desktop\GoF2_PC_REP\GOF2.pdb`.
3. **2012-12-04/07 — iOS 1.1.4 HD**: Xcode 4.5, Apple clang, SDK 6.0,
   thin armv7 PIE, libstdc++.6, sjlj, stack protector.
4. **pre-2018 — Android old official 2.0.16**: NDK r14, clang 3.8.275480,
   gold 1.11, gnustl_shared, `-O2` (ndk-build release default), `.text`
   1,475,272 B, `.ARM.extab` before `.ARM.exidx`, `.data` align 4.
5. **2018-10-22 — Android 2.0.16 rebuild**: NDK r18b, clang 7.0.2, gold 1.12,
   CMake `c++_static`, `-Oz`, `.text` 1,259,464 B (14.6% smaller than the r14
   build; the r14 build is 17.1% larger than GT), `.ARM.extab` after `.ARM.exidx`,
   `.data` align 16, static libc++. This is the ground-truth `libgof2hdaa.so`.

The 2018 rebuild is not a recompile of the same source with a newer NDK: it
also changed the C++ runtime (gnustl_shared → c++_static), the optimization
level (`-O2` → `-Oz`), the linker (gold 1.11 → 1.12), the section layout, and
the RTTI export surface relative to the r14 build; the 2018 dex's JNI surface
also does not exactly match the 2018 `.so`'s exports (§3.4). The version string
`2.0.16` does **not** pin the toolchain; the same version exists in both the
r14 and r18b builds.

## 10. Gate state and what changed

No source or build-flag change was landed. The target is `n/a`, so there is no
"before/after %" for a function; the gate is unchanged:

```text
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.92
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`tools/verify_baseline.json` was not touched. `report.json` was not committed.
The worktree is left clean.

## 11. Exact commands used

```sh
# baseline
cd /home/debian/GalaxyOnFire2/wt/w5919
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6

# ELF metadata / section diff
python3 /dev/shm/w5919/tools/elfinfo.py <elf>
python3 /dev/shm/w5919/tools/secdiff.py <gt> <ours>

# APK zip layout / signing block
unzip -l "/home/debian/GalaxyOnFire2/Original APK/Galaxy on Fire 2™ HD_2.0.16_APKPure.apk"
python3 - <<'PY'   # EOCD/CD parser, see §2.1
...
PY
openssl pkcs7 -inform DER -in apk/META-INF/CERT.RSA -print_certs -noout
openssl x509 -inform DER -in /dev/shm/w5919/v2cert.der -noout -serial -dates -subject

# dex / AXML
python3 - <<'PY'   # dex string/class parser, see §3.3/§3.4
...
PY
python3 - <<'PY'   # AXML string pool + attribute parser, see §3.3
...
PY

# ELF symbols / calls
readelf -sW --dyn-syms <elf>
readelf -p .comment <elf>
readelf -n <elf>
readelf -A <elf>
readelf -x .ARM.attributes <elf>
objdump -d <elf> > <elf>.dis
grep -cE '\bblx?\b.*__cxa_guard_acquire' <elf>.dis

# r13b hybrid build (for the section table)
cmake -S . -B /dev/shm/w5919/build-r13b-O2 -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/dev/shm/w5919/toolchain-r13b.cmake \
  -DCMAKE_BUILD_TYPE=Release
ninja -C /dev/shm/w5919/build-r13b-O2
bash /dev/shm/w5919/link_r13b.sh
```

The scratch helpers are `/dev/shm/w5919/tools/elfinfo.py`,
`/dev/shm/w5919/tools/secdiff.py`, `/dev/shm/w5919/gt.dis`,
`/dev/shm/w5919/old.dis`, `/dev/shm/w5919/dex_strings.txt`,
`/dev/shm/w5919/arsc_strings.txt`, and the probe sources in
`/dev/shm/w5919/probe/`.
