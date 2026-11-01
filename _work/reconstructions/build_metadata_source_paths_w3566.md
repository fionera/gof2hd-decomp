# Build metadata and source-path mining (w3566)

Status: research note, 2026-11-01. Worktree `wt/w3566`, branch `wave87-w3566`,
base `113737ee` (`docs: wave 94 harvest notes`). Every number below is from a
command run against `../_work/bins/android_2.0.16_libgof2hdaa.so`, the original
APK, or a controlled rebuild of this tree. No source or build flag was changed
by this pass; the only commit is this note.

## 0. Bottom line

* The original is an **NDK r18b / clang 7.0.2 / gold 1.12, armeabi-v7a
  soft-float, API 16, `c++_static`** build. The `.note.android.ident` section is
  **byte-identical** to the NDK r18b `platforms/android-16/arch-arm/usr/lib/crtbegin_so.o`
  note, and the `.comment` section is the clang-7.0.2 + GCC-4.9.x merge.
* The **only path-like string in the whole original `.so`** is the NDK
  libc++abi buildbot path
  `/Volumes/Android/buildbot/src/android/ndk-release-r18/external/libcxx/../../external/libcxxabi/src/abort_message.cpp`.
  It is the `__FILE__` string of the NDK r18b prebuilt `libc++abi.a`
  `abort_message.o`; the original's `abort_message` function at `0x193e64` is
  byte-identical to that archive member apart from its seven relocation words.
  **No app source path survives.**
* The only app source-file-name witness in the binary is **`main.c`**, in the
  three log strings `dataDirectory in main.c: %s`, `rootPath in main.c: %s`,
  `zipDirectory in main.c: %s`. They are referenced from
  `Java_net_fishlabs_gof2hdallandroid2012_ToJNI_SetDirectories` at `0x6b6d0`.
* The APK's `classes.dex` supplies the Java-side source file names (29 unique
  `.java` files under `net/fishlabs/`) and the complete `BuildConfig`
  (`APPLICATION_ID`, `BUILD_TYPE=release`, `DEBUG=false`, `FLAVOR=""`,
  `VERSION_CODE=47947032`, `VERSION_NAME=2.0.16`). The DEX, manifest, and
  `assets/crashlytics-build.properties` agree.
* The current tree has **two residual ELF-metadata deltas that the gate does not
  catch**:
  1. an extra `.missioncase` section (28 sections vs the original's 27,
     `.shstrtab` `0x124` vs `0x117`, file size `2,187,380` vs `2,187,328`);
  2. `.ARM.extab` is `0x26290` vs the original's `0x26298` (8 bytes short),
     although `docs/IDENTICALITY.md` still says it is exact.
  Removing `.missioncase` makes the section count, `.shstrtab` (byte-for-byte),
  and file size exactly match the original, but costs **74 `byte_exact`**
  (`4496 -> 4422`), because the 192-byte rank table moves into `.rodata` and
  grows it by `0xc0`. The current tradeoff is therefore correct until the rank
  table is eliminated by a source-shape change.

## 1. Baseline gate

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint
cmake --build cmake-build-match --target drift
```

Measured at `113737ee`:

```text
verify: byte 4496  linked 4496  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
CLEAN ✓ (no new soft violations; hard invariants 0)
=== DRIFT: 0 field(s) across 0 class(es) ===
```

`.text` is `0x69440` / `0x1337c8` in both the original and the current build.

## 2. ELF build metadata of the original

### 2.1 Header and ABI

```sh
readelf -hW ../_work/bins/android_2.0.16_libgof2hdaa.so
readelf -AW ../_work/bins/android_2.0.16_libgof2hdaa.so
```

```text
Class: ELF32
Data:  2's complement, little endian
Type:  DYN (Shared object file)
Machine: ARM
Flags: 0x5000200, Version5 EABI, soft-float ABI
Number of program headers: 8
Number of section headers: 27
```

`readelf -A`:

```text
Tag_CPU_name: "ARM v7"
Tag_CPU_arch: v7
Tag_CPU_arch_profile: Application
Tag_ARM_ISA_use: Yes
Tag_THUMB_ISA_use: Thumb-2
Tag_FP_arch: VFPv3
Tag_Advanced_SIMD_arch: NEONv1
Tag_ABI_PCS_GOT_use: GOT-indirect
Tag_ABI_PCS_wchar_t: 4
Tag_ABI_FP_denormal: Needed
Tag_ABI_FP_exceptions: Needed
Tag_ABI_FP_number_model: IEEE 754
Tag_ABI_align_needed: 8-byte
Tag_ABI_enum_size: int
Tag_ABI_HardFP_use: Deprecated
Tag_CPU_unaligned_access: v6
Tag_ABI_FP_16bit_format: IEEE 754
```

The current build's `.ARM.attributes` is byte-identical to the original's
(compare the section bytes, not just `readelf -A`).

### 2.2 Notes and `.comment`

```sh
readelf -nW ../_work/bins/android_2.0.16_libgof2hdaa.so
readelf -p .comment ../_work/bins/android_2.0.16_libgof2hdaa.so
```

```text
.note.android.ident: API 16, "r18b", build "5063045"
.note.gnu.build-id:  c011810da45777b83ed721b2a87d517efed0d310
.note.gnu.gold-version: gold 1.12
.comment:
  [     1] Android (4751641 based on r328903) clang version 7.0.2
            (https://android.googlesource.com/toolchain/clang
             003100370607242ddd5815e4a043907ea9004281)
            (https://android.googlesource.com/toolchain/llvm
             1d739ffb0366421d383e04ff80ec2ee591315116)
            (based on LLVM 7.0.2svn)
  [   108] GCC: (GNU) 4.9.x 20150123 (prerelease)
```

The `.note.android.ident` section is **byte-identical** to the NDK r18b
`crtbegin_so.o` note:

```sh
python3 - <<'PY'
import struct
def getsec(p, name):
    d = open(p, 'rb').read()
    shoff = struct.unpack_from('<I', d, 32)[0]
    shent = struct.unpack_from('<H', d, 46)[0]
    shnum = struct.unpack_from('<H', d, 48)[0]
    shstrndx = struct.unpack_from('<H', d, 50)[0]
    S = [struct.unpack_from('<IIIIIIIIII', d, shoff + i*shent)
         for i in range(shnum)]
    sh = S[shstrndx]
    def sn(n):
        e = d.index(b'\0', sh[4] + n)
        return d[sh[4] + n:e].decode()
    for s in S:
        if sn(s[0]) == name:
            return d[s[4]:s[4] + s[5]]
a = getsec('../_work/bins/android_2.0.16_libgof2hdaa.so', '.note.android.ident')
b = getsec('.cache/ndk/android-ndk-r18b/platforms/android-16/arch-arm/usr/lib/crtbegin_so.o',
           '.note.android.ident')
print('orig note len', len(a), 'crtbegin note len', len(b), 'same', a == b)
PY
```

Result: `orig note len 152  crtbegin note len 152  same True`.
The note is therefore contributed by the NDK crt object, not by the app TUs.
The clang `.comment` string is also present in the same `crtbegin_so.o`; the
GCC 4.9.x string is present in many members of the NDK r18b
`.../armv7-a/thumb/libgcc.a` (for example `__gcc_bcmp.o`, `__main.o`,
`_absvdi2.o`, ...), consistent with `--exclude-libs=libgcc.a`.

### 2.3 Dynamic section, versions, relocations

```sh
readelf -dW ../_work/bins/android_2.0.16_libgof2hdaa.so
readelf -VW ../_work/bins/android_2.0.16_libgof2hdaa.so
readelf -rW ../_work/bins/android_2.0.16_libgof2hdaa.so
```

```text
SONAME: libgof2hdaa.so
NEEDED (in order): liblog, libGLESv2, libGLESv1_CM, libEGL, libandroid,
                   libfmodex, libfmodevent, libm, libdl, libc
DT_FLAGS:   BIND_NOW
DT_FLAGS_1: NOW
DT_RELCOUNT: 1162
VERDEF: 1 entry, BASE, name libgof2hdaa.so
VERNEED: libc.so LIBC (version 2), libm.so LIBC (version 3)
```

Relocation counts:

```text
.rel.dyn: 3593 entries
.rel.plt: 3674 entries
R_ARM_JUMP_SLOT 3674
R_ARM_ABS32     1817
R_ARM_RELATIVE  1162
R_ARM_GLOB_DAT   614
```

`.hash` parameters (read directly from the section):

```text
nbucket = 4099
nchain  = 5578
size    = (2 + 4099 + 5578) * 4 = 0x973c
```

`.dynsym` has `0x15ca0 / 16 = 5578` entries; `readelf --dyn-syms` reports
**5373 defined + 208 undefined**.

### 2.4 Sections and program headers

`readelf -SW` / `readelf -lW` (original):

```text
.text        0x69440  0x1337c8
.ARM.exidx   0x19cc08 0x6f70
.ARM.extab   0x1a3b78 0x26298
.rodata      0x1c9e10 0x41b67
.fini_array  0x20cba0 0x8
.data.rel.ro 0x20cbb0 0x2fe8
.init_array  0x20fb98 0xc
.dynamic     0x20fba4 0x148
.got         0x20fcf4 0x430c
.data        0x214000 0x296c
.bss         0x216970 0x69ac
```

8 program headers: `PHDR`, two `LOAD`s, `DYNAMIC`, `NOTE`, `GNU_STACK`,
`ARM_EXIDX`, `GNU_RELRO`. The RW `LOAD` starts at `0x20cba0`, the RELRO segment
covers `.fini_array`/`.data.rel.ro`/`.init_array`/`.dynamic`/`.got`, and
`GNU_STACK` is `RW`.

### 2.5 `.init_array` / `.fini_array`

```sh
readelf -x .init_array ../_work/bins/android_2.0.16_libgof2hdaa.so
readelf -x .fini_array ../_work/bins/android_2.0.16_libgof2hdaa.so
```

Both are byte-identical to the current build:

```text
.init_array: 41940600 c1940600 dd940600
.fini_array: 48950600 34950600
```

Relinking the current objects without the final `strip` names them:

```sh
# same gold line as the link_so target, minus the final strip
nm -n /tmp/w3566mine/unstripped.so | grep -E 'GLOBAL__sub_I|__on_dlclose'
```

```text
00069440 t _GLOBAL__sub_I_Engine.cpp
000694c0 t _GLOBAL__sub_I_LODManager.cpp
000694dc t _GLOBAL__sub_I_ParticleSettings.cpp
00069534 t __on_dlclose
00069548 t __on_dlclose_late
```

The original's `.init_array` is exactly those three entries and `.fini_array`
exactly those two.

## 3. Gold's build-id algorithm

Gold computes the build-id as **SHA-1 over the complete output file with the
20-byte `.note.gnu.build-id` descriptor zeroed**. This can be proved on our own
unstripped link:

```sh
# link without strip (see §8), then:
python3 - <<'PY'
import hashlib, struct
p = '/tmp/w3566mine/unstripped.so'
d = bytearray(open(p, 'rb').read())
# .note.gnu.build-id is at file offset 0x1cc, size 0x24;
# the 20-byte descriptor starts at 0x1cc + 12 + 4 = 0x1dc
desc = 0x1dc
target = bytes(d[desc:desc+20]).hex()
d[desc:desc+20] = b'\0' * 20
print('stored  ', target)
print('computed', hashlib.sha1(d).hexdigest())
PY
```

Result for the current unstripped link:

```text
stored   28e16aed997d40615dfaef179008cf5e591729a3
computed 28e16aed997d40615dfaef179008cf5e591729a3
```

The original's build-id is `c011810da45777b83ed721b2a87d517efed0d310`. It is a
hash of the **original unstripped link output**, which is not shipped. Running
the same computation on the stripped original gives
`d7c05dc5e9804023d8786147addc48537b0bee6d`, not the stored value, because
`strip` removed `.symtab`/`.strtab` after gold wrote the note. The original's
build-id can therefore only converge when the full unstripped link content
matches; it is not a usable independent oracle.

## 4. Source-path mining

### 4.1 Exhaustive scan

```sh
strings -a -n 6 ../_work/bins/android_2.0.16_libgof2hdaa.so \
  | grep -E '\.(cpp|cxx|cc|hpp|hxx|mm)$|^/[A-Za-z0-9_./-]+$'
```

Result: exactly one line,

```text
/Volumes/Android/buildbot/src/android/ndk-release-r18/external/libcxx/../../external/libcxxabi/src/abort_message.cpp
```

The same scan restricted to `.rodata`, `.data`, and `.data.rel.ro` (extracted
with the NDK `objcopy`) finds the same single path plus the three `main.c`
strings:

```sh
OBJCOPY=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objcopy
$OBJCOPY -O binary --only-section=.rodata ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/rodata.bin
strings -a -n 4 /tmp/rodata.bin | grep -E '\.(c|cc|cpp|h|hpp)$|/Volumes/|/Users/|/home/'
# the same objcopy/strings scan of .data and .data.rel.ro finds no further paths
```

```text
dataDirectory in main.c: %s
rootPath in main.c: %s
zipDirectory in main.c: %s
/Volumes/Android/buildbot/src/android/ndk-release-r18/external/libcxx/../../external/libcxxabi/src/abort_message.cpp
```

No other `.c`/`.cpp`/`.h` path exists in `.rodata`, `.data`, or
`.data.rel.ro`. The original has no `.symtab`/`.strtab`/`.debug_*`, so the app
source paths are not recoverable from the binary.

### 4.2 `abort_message.cpp` is the NDK r18b prebuilt libc++abi member

The string is the `__FILE__` literal of `abort_message.o` in the NDK r18b
prebuilt archive:

```sh
NDK=.cache/ndk/android-ndk-r18b
AR=$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-ar
mkdir -p /tmp/w3566mine/ar && cd /tmp/w3566mine/ar
$AR x $NDK/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/libc++abi.a abort_message.o
```

The original's `abort_message` function is the local function at
`0x193e64..0x193ec4` (96 bytes). It is found by resolving the PIC literal that
points at the path string:

```text
path string VMA           0x1e6b2b  (.rodata offset 0x1cd1b)
ldr r0, [pc, #12] @      0x193eac
literal word @           0x193ebc = 0x00052c77
add r0, pc @             0x193eb0 -> 0x1e6b2b
```

Compare the 96 function bytes with the archive member's
`.text.abort_message`:

```text
func len 96  obj len 96  byte diffs 25
differing words: 0x2c 0x34 0x40 0x50 0x54 0x58 0x5c
non-relocation word diffs: (none)
```

The seven differing words are exactly the seven relocation offsets in
`.rel.text.abort_message` (`vfprintf`, `fputc`, `vasprintf`, `__assert2`, and
the three literal-pool relocations). The path string and the
`abort_message.cpp` string are byte-identical to the archive member's
`.rodata.str1.1`. The current build's `abort_message` function at `0x193e64`
is also byte-identical to the original's (`diffs []`).

Conclusion: the original linked the NDK r18b prebuilt `libc++abi.a`
`abort_message.o` unchanged. The path is NDK buildbot provenance, not an app
source path.

### 4.3 The only app source-file-name witness: `main.c`

The three log strings are at:

```text
dataDirectory in main.c: %s   VMA 0x1ca103  (rodata + 0x2f3)
rootPath in main.c: %s        VMA 0x1ca11f  (rodata + 0x30f)
zipDirectory in main.c: %s    VMA 0x1ca136  (rodata + 0x326)
```

Resolving the PIC literal loads gives their code references:

```text
ldr@0x6b74e pool@0x6b7fc word=0x15e9a9 add@0x6b756 dest=0x1ca103
ldr@0x6b75e pool@0x6b800 word=0x15e9b3 add@0x6b768 dest=0x1ca11f
ldr@0x6b7bc pool@0x6b80c word=0x15e970 add@0x6b7c2 dest=0x1ca136
```

All three are inside `Java_net_fishlabs_gof2hdallandroid2012_ToJNI_SetDirectories`
at `0x6b6d0`, matching `src/platform/android/main.c`. No other app source file
name is embedded.

## 5. APK and DEX build metadata

### 5.1 The APK `.so` and `_work/bins` are the same build

```sh
APK="/home/debian/GalaxyOnFire2/Original APK/Galaxy on Fire 2™ HD_2.0.16_APKPure.apk"
unzip -p "$APK" lib/armeabi-v7a/libgof2hdaa.so > /tmp/apk_libgof2hdaa.so
sha256sum /tmp/apk_libgof2hdaa.so ../_work/bins/android_2.0.16_libgof2hdaa.so
cmp -l /tmp/apk_libgof2hdaa.so ../_work/bins/android_2.0.16_libgof2hdaa.so | wc -l
```

```text
6d38fd767ee3b4f8174c371bce371bfc6397cf76e13c1756938b2a94658e3b58  APK lib
d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483  _work/bins
288 differing bytes
```

The 288 bytes are entirely in `.shstrtab` and the `sh_name` fields of the
section header table (`0x215af2..0x216018`). Every section's content is
identical, including:

```text
.text sha256  d49e32c1330e5d94e7628b9a40e05500af963a22816a93f2c738dbd366f813ed  (both)
build-id      c011810da45777b83ed721b2a87d517efed0d310                         (both)
```

The APK `.shstrtab` order is the original order and matches the current build's
order (with `.missioncase` inserted after `.rodata`); the `_work/bins` copy has
a reordered `.shstrtab` (starts `.init_array`, `.fini_array`, ...). The
`_work/bins` file is a section-table-renamed copy of the APK library, not a
different build.

### 5.2 APK metadata

```sh
unzip -p "$APK" assets/crashlytics-build.properties
unzip -p "$APK" META-INF/MANIFEST.MF | head -5
unzip -p "$APK" META-INF/CERT.SF | head -5
openssl pkcs7 -inform DER -in META-INF/CERT.RSA -print_certs -text -noout
```

```text
crashlytics-build.properties:
  #Mon Oct 22 10:18:17 CEST 2018
  version_name=2.0.16
  package_name=net.fishlabs.gof2hdallandroid2012
  build_id=f527605b-309f-404d-8afc-b68027edc49e
  version_code=47947032
  app_name=GOF2

META-INF/MANIFEST.MF:
  Manifest-Version: 1.0
  Built-By: Generated-by-ADT
  Created-By: Android Gradle 3.1.2

META-INF/CERT.SF:
  Signature-Version: 1.0
  Created-By: 1.0 (Android)
  X-Android-APK-Signed: 2

CERT.RSA:
  Subject/Issuer: O=FISHLABS Entertainment GmbH
  Serial: 1278067031 (0x4c2dc157)
  Signature Algorithm: sha1WithRSAEncryption
  Validity: Jul 2 10:37:11 2010 GMT .. Jun 19 10:37:11 2060 GMT
  Public Key: RSA 1024-bit
```

Binary `AndroidManifest.xml` (parsed AXML):

```text
package          = net.fishlabs.gof2hdallandroid2012
versionCode      = 47947032
versionName      = 2.0.16
minSdkVersion    = 16
targetSdkVersion = 27
```

This agrees with the DEX `BuildConfig` and the crashlytics properties. The
native build date is not in the `.so`; the only date is the APK's
`Mon Oct 22 10:18:17 CEST 2018`.

### 5.3 DEX metadata

```sh
unzip -p "$APK" classes.dex > /tmp/classes.dex
```

Header:

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
```

The DEX checksum (Adler-32 over bytes 12..end) matches the stored checksum:

```text
stored   0x48e247fe
computed 0x48e247fe
```

The internal DEX SHA-1 signature (bytes 32..end) does **not** match the stored
signature:

```text
stored   3987124219329f73f1e5cc16c24286a4b8b08819
computed ea65af394fb6f44a149d74cb83acd30de4d06362
```

The APK `MANIFEST.MF` SHA-1 digest of `classes.dex` does match the whole-file
SHA-1 (`lNi5JZ5oTjSGN7T9Ft9t+2ruWAg=`), so the DEX is intact as packaged; the
stale internal signature is a post-processing artifact (cause not proven).

`BuildConfig` static values (parsed from `class_data_item` + `static_values`):

```text
APPLICATION_ID = net.fishlabs.gof2hdallandroid2012
BUILD_TYPE     = release
DEBUG          = false
FLAVOR         = ""
VERSION_CODE   = 47947032
VERSION_NAME   = 2.0.16
```

The DEX `SourceFile` attributes give the original Java glue source file names.
The complete `net/fishlabs/` set is:

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

(29 files; packages `net.fishlabs.gof2hdallandroid2012`,
`net.fishlabs.googleplay`, `net.fishlabs.ow_utils`.)

### 5.4 JNI surface cross-check

The DEX declares 51 native methods under `net/fishlabs/`; the original `.so`
exports 52 `Java_*` symbols. The differences are:

```text
DEX native, not exported by the .so:
  Java_net_fishlabs_gof2hdallandroid2012_GLViewRenderer_setKeyboardStatus
  Java_net_fishlabs_googleplay_ToJNI_gof2hd2012setPremiumContent

.so export, not declared native in the DEX:
  Java_net_fishlabs_gof2hdallandroid2012_ToJNI_STARTUP
  Java_net_fishlabs_playhaven_ToJNI_getCurrentApplicationModule
  Java_net_fishlabs_tapjoy_ToJNI_spentAmountOfCredits
```

The current tree's 52 `Java_*` exports are **exactly** the original's 52
(`diff` empty). The two DEX-only natives are not in any APK library and are not
registered by `JNI_OnLoad` (0x6b550 calls `FindClass` for
`net/fishlabs/gof2hdallandroid2012/GOF2HD2012`, not `RegisterNatives`); they are
likely dead Java declarations or resolved by a path not present in this APK.

## 6. Current tree metadata deltas and the `.missioncase` experiment

### 6.1 Section table

```sh
readelf -SW ../_work/bins/android_2.0.16_libgof2hdaa.so
readelf -SW cmake-build-match/libgof2hdaa.so
stat -c '%s %n' ../_work/bins/android_2.0.16_libgof2hdaa.so cmake-build-match/libgof2hdaa.so
```

| variant | sections | `.rodata` | `.missioncase` | `.shstrtab` | file size | byte_exact |
|---|---:|---:|---:|---:|---:|---:|
| original | 27 | `0x41b67` | — | `0x117` | 2,187,328 | — |
| current | 28 | `0x41b67` | `0xc0` @ `0x20b977` | `0x124` | 2,187,380 | 4496 |
| no `.missioncase` | 27 | `0x41c27` | — | `0x117` | 2,187,328 | 4422 |

The extra section comes from `src/game/mission/Status.cpp:1388`:

```cpp
#pragma clang section rodata=".missioncase"
static const unsigned char kMissionCase[192] = { ... };
#pragma clang section rodata=""
```

It is placed in the original's post-`.rodata` gap
(`0x20b977..0x20cba0`), so no allocated section or address moves. The
`.shstrtab` difference is exactly the 13-byte string `.missioncase\0`; the
current `.shstrtab` is the APK's `.shstrtab` with that string inserted after
`.rodata\0`. The file is 52 bytes larger (`40` bytes for the extra section
header + `13` bytes for `.shstrtab`, minus alignment).

### 6.2 Controlled A/B: remove the `.missioncase` pragmas

```sh
cp src/game/mission/Status.cpp /tmp/Status.cpp.orig
python3 - <<'PY'
p='src/game/mission/Status.cpp'
lines=open(p).read().splitlines(True)
out=[l for l in lines if l.strip() not in
     ('#pragma clang section rodata=".missioncase"',
      '#pragma clang section rodata=""')]
open(p,'w').writelines(out)
PY
cmake -S . -B /tmp/w3566-nomissioncase -G Ninja -DGOF2_MATCH_OPT=-Oz
cmake --build /tmp/w3566-nomissioncase --target check -j8
git checkout -- src/game/mission/Status.cpp
```

Measured:

```text
verify: byte 4422  linked 4496  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
  FAIL  byte_exact 4496 -> 4422
RATCHET FAIL
```

The no-`.missioncase` build has 27 sections, `.rodata 0x41c27`,
`.shstrtab 0x117` (byte-for-byte identical to the APK's original
`.shstrtab`), and file size 2,187,328 (exact). It is a metadata win but a
74-function `byte_exact` regression, so it is **not** committed. The current
tradeoff is the correct one under the ratchet's primary metric.

### 6.3 `.ARM.extab` is 8 bytes short

```sh
readelf -SW ../_work/bins/android_2.0.16_libgof2hdaa.so | grep -E 'ARM.exidx|ARM.extab'
readelf -SW cmake-build-match/libgof2hdaa.so | grep -E 'ARM.exidx|ARM.extab'
```

```text
original: .ARM.exidx 0x19cc08 0x6f70   .ARM.extab 0x1a3b78 0x26298
current:  .ARM.exidx 0x19cc08 0x6f70   .ARM.extab 0x1a3b78 0x26290
```

The `.ARM.exidx` size is exact; `.ARM.extab` is `0x26290` vs `0x26298`
(8 bytes short). This contradicts the current `docs/IDENTICALITY.md` line
“`.ARM.extab` is now `0x26298` ... both size- and base-exact”. The `check`
gate does not catch section-size deltas, so this is a real unguarded metadata
regression. It is independent of `.missioncase` (the no-`.missioncase` build
also has `0x26290`). Isolating the EH record is outside this pass.

## 7. Proven vs still hypothesis

Proven by commands against the original/APK and controlled rebuilds:

* NDK r18b, clang 7.0.2, gold 1.12; armeabi-v7a, soft-float, Thumb-2, VFPv3,
  NEONv1, API 16, `c++_static`.
* `.note.android.ident` is byte-identical to the NDK r18b android-16
  `crtbegin_so.o` note; the clang `.comment` string is from the same object,
  and the GCC 4.9.x string is from NDK r18b `libgcc.a` members.
* `.ARM.attributes`, `.comment`, `.note.gnu.gold-version`, `.dynamic`,
  `.init_array`, `.fini_array`, `.got`, `.plt`, `.rodata`, `.data`,
  `.data.rel.ro` are byte-identical between the original and the current build
  (`.rodata`/`.data`/`.data.rel.ro` are byte-exact; the others are size- and
  content-exact).
* The only path-like string in the original `.so` is the NDK libc++abi
  `abort_message.cpp` buildbot path; the `abort_message` function is
  byte-identical to the NDK r18b prebuilt `libc++abi.a` member except for its
  seven relocation words.
* The only app source-file-name witness is `main.c` (three log strings in
  `Java_..._SetDirectories`).
* The APK `.so` and `_work/bins` `.so` are the same build; the only difference
  is `.shstrtab`/`sh_name` (288 bytes). The APK `.shstrtab` order is the
  original order.
* Gold's build-id is SHA-1 over the complete unstripped output with the 20-byte
  build-id descriptor zeroed.
* The DEX `BuildConfig`, `AndroidManifest.xml`, and
  `assets/crashlytics-build.properties` agree on package, version code, version
  name, and release/debug status.
* The current tree has an extra `.missioncase` section and an 8-byte-short
  `.ARM.extab`; removing `.missioncase` matches the original's section count,
  `.shstrtab`, and file size but costs 74 `byte_exact`.

Still hypothesis / not recoverable from the shipped artifacts:

* The exact original `Android.mk`/`Application.mk` text; no build script
  survives (see `build_system_evidence.md`).
* The exact app source paths/TU list; the `.so` is stripped and has no app
  `__FILE__` strings. The source-layout dumps are the only witness.
* The original's build-id value cannot be reproduced without the original
  unstripped link output.
* The cause of the stale DEX internal SHA-1 signature (post-processing tool) is
  not proven; the APK manifest digest is valid.
* The exact EH record responsible for the `.ARM.extab` 8-byte shortfall is not
  isolated in this pass.

## 8. Reproduction commands

```sh
# baseline
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint
cmake --build cmake-build-match --target drift

# ELF metadata
O=../_work/bins/android_2.0.16_libgof2hdaa.so
readelf -hW $O; readelf -AW $O; readelf -nW $O; readelf -p .comment $O
readelf -dW $O; readelf -VW $O; readelf -rW $O; readelf -SW $O; readelf -lW $O
readelf --dyn-syms -W $O | awk '$7!="UND"{n++} END{print "defined",n}'
readelf --dyn-syms -W $O | awk '$7=="UND"{n++} END{print "undef",n}'

# note provenance
NDK=.cache/ndk/android-ndk-r18b
readelf -nW $NDK/platforms/android-16/arch-arm/usr/lib/crtbegin_so.o
readelf -p .comment $NDK/platforms/android-16/arch-arm/usr/lib/crtbegin_so.o

# source-path scan
strings -a -n 6 $O | grep -E '\.(cpp|cxx|cc|hpp|hxx|mm)$|^/[A-Za-z0-9_./-]+$'
OBJCOPY=$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objcopy
$OBJCOPY -O binary --only-section=.rodata $O /tmp/rodata.bin
strings -a -n 4 /tmp/rodata.bin | grep -E '\.(c|cc|cpp|h|hpp)$|/Volumes/|/Users/|/home/'

# abort_message.o provenance
AR=$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-ar
mkdir -p /tmp/w3566mine/ar && cd /tmp/w3566mine/ar
$AR x $NDK/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/libc++abi.a abort_message.o
# compare the 96 bytes at 0x193e64 with .text.abort_message (see §4.2)

# APK / DEX
APK="/home/debian/GalaxyOnFire2/Original APK/Galaxy on Fire 2™ HD_2.0.16_APKPure.apk"
unzip -p "$APK" lib/armeabi-v7a/libgof2hdaa.so > /tmp/apk_libgof2hdaa.so
sha256sum /tmp/apk_libgof2hdaa.so $O
cmp -l /tmp/apk_libgof2hdaa.so $O | wc -l
unzip -p "$APK" assets/crashlytics-build.properties
unzip -p "$APK" META-INF/MANIFEST.MF | head -5
unzip -p "$APK" META-INF/CERT.SF | head -5
unzip -p "$APK" classes.dex > /tmp/classes.dex
# DEX parser: see the compact script below

# build-id algorithm (on our unstripped link)
# link without strip, then:
python3 - <<'PY'
import hashlib
p='/tmp/w3566mine/unstripped.so'
d=bytearray(open(p,'rb').read())
desc=0x1dc
print('stored  ',bytes(d[desc:desc+20]).hex())
d[desc:desc+20]=b'\0'*20
print('computed',hashlib.sha1(d).hexdigest())
PY

# .missioncase A/B
cp src/game/mission/Status.cpp /tmp/Status.cpp.orig
python3 - <<'PY'
p='src/game/mission/Status.cpp'
lines=open(p).read().splitlines(True)
out=[l for l in lines if l.strip() not in
     ('#pragma clang section rodata=".missioncase"',
      '#pragma clang section rodata=""')]
open(p,'w').writelines(out)
PY
cmake -S . -B /tmp/w3566-nomissioncase -G Ninja -DGOF2_MATCH_OPT=-Oz
cmake --build /tmp/w3566-nomissioncase --target check -j8
git checkout -- src/game/mission/Status.cpp
```

Compact DEX parser used for §5.3/§5.4 (prints the `net/fishlabs/` source
files, `BuildConfig` static values, and native methods):

```python
import struct
d = open('/tmp/classes.dex', 'rb').read()
def u16(o): return struct.unpack_from('<H', d, o)[0]
def u32(o): return struct.unpack_from('<I', d, o)[0]
def uleb(o):
    r = s = 0
    while True:
        b = d[o]; o += 1; r |= (b & 0x7f) << s
        if not b & 0x80: return r, o
        s += 7
string_ids_off = u32(60); type_ids_off = u32(68)
proto_ids_off = u32(76); field_ids_off = u32(84)
method_ids_off = u32(92); class_defs_off = u32(100)
class_defs_size = u32(96)
def string(i):
    o = u32(string_ids_off + i*4); n, o = uleb(o)
    return d[o:o+n].decode('utf-8', 'replace')
def type_name(i): return string(u32(type_ids_off + i*4))
def proto_desc(i):
    ret = type_name(u32(proto_ids_off + i*12 + 4))
    po = u32(proto_ids_off + i*12 + 8)
    ps = []
    if po:
        n = u32(po); ps = [type_name(u16(po + 4 + k*2)) for k in range(n)]
    return ret, ps
def field_name(i):
    return (type_name(u16(field_ids_off + i*8)),
            type_name(u16(field_ids_off + i*8 + 2)),
            string(u32(field_ids_off + i*8 + 4)))
def method_name(i):
    return (type_name(u16(method_ids_off + i*8)),
            string(u32(method_ids_off + i*8 + 4)),
            proto_desc(u16(method_ids_off + i*8 + 2)))
def rint(o, n, signed):
    v = int.from_bytes(d[o:o+n], 'little')
    if signed and n < 4 and (v >> (n*8-1)) & 1: v -= 1 << (n*8)
    return v
def value(o):
    h = d[o]; o += 1; vt = h & 0x1f; n = (h >> 5) + 1
    if vt == 0x00: return None, o
    if vt in (0x02, 0x04, 0x06): return rint(o, n, True), o + n
    if vt in (0x03, 0x10, 0x11, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b):
        v = rint(o, n, False)
        if vt == 0x17: return string(v), o + n
        if vt == 0x18: return type_name(v), o + n
        return v, o + n
    if vt == 0x1c:
        k, q = uleb(o); a = []
        for _ in range(k):
            v, q = value(q); a.append(v)
        return a, q
    if vt == 0x1f: return (n == 2), o
    return d[o:o+n].hex(), o + n
files = set(); natives = []; bc = {}
for i in range(class_defs_size):
    o = class_defs_off + i*32
    ci = u32(o); data = u32(o + 24); sv = u32(o + 28)
    si = u32(o + 16); cn = type_name(ci)
    if cn.startswith('Lnet/fishlabs/') and si != 0xffffffff:
        files.add(string(si))
    if not data: continue
    p = data
    sfs, p = uleb(p); ifs, p = uleb(p); dms, p = uleb(p); vms, p = uleb(p)
    sf = []; idx = 0
    for _ in range(sfs):
        diff, p = uleb(p); acc, p = uleb(p); idx += diff; sf.append(idx)
    for _ in range(ifs):
        diff, p = uleb(p); acc, p = uleb(p)
    methods = []; idx = 0
    for _ in range(dms):
        diff, p = uleb(p); acc, p = uleb(p); code, p = uleb(p)
        idx += diff; methods.append((idx, acc))
    idx = 0
    for _ in range(vms):
        diff, p = uleb(p); acc, p = uleb(p); code, p = uleb(p)
        idx += diff; methods.append((idx, acc))
    if cn.startswith('Lnet/fishlabs/'):
        for mi, acc in methods:
            if acc & 0x100:
                c, n, (r, ps) = method_name(mi)
                natives.append((c, n, r, ps))
    if cn == 'Lnet/fishlabs/gof2hdallandroid2012/BuildConfig;' and sv:
        q = sv; k, q = uleb(q); vals = []
        for _ in range(k):
            v, q = value(q); vals.append(v)
        for fi, v in zip(sf, vals):
            c, t, n = field_name(fi); bc[n] = v
print('SOURCE FILES'); [print(' ', f) for f in sorted(files)]
print('BUILDCONFIG'); [print(' ', k, '=', v) for k, v in bc.items()]
print('NATIVE METHODS')
for c, n, r, ps in sorted(natives):
    print(f'  {c} {n} {r} ({", ".join(ps)})')
```
