# Build archaeology: the APK and the binary (w4306, wave 93)

Status: research note, 2026-11-03. Worktree `wt/w4306`, branch `wave93-w4306`,
base `eea2e3c1`. Scratch is `/dev/shm/w4306`. **No `src/` or build-flag change is
landed**: the note is the deliverable. The canonical gate is unchanged
(byte_exact 4499/4524, linked_exact 4499/4524, size-exact 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0; `RATCHET PASS`).

This note answers the question "we have no clue if the flags are correct" with
**positive evidence from the shipped artifacts**, not only "the gate did not
improve". It supersedes the APK/binary sections of `build_provenance_w4208.md`
and adds the two facts those notes did not have: the APK's `MANIFEST.MF` names
**Android Gradle Plugin 3.1.2**, and the native lib's `.rodata` carries the
**darwin** `libc++abi` path. It re-measures the load-bearing flag hypotheses on
the wave-93 tree with the official verifier.

## 0. Bottom line

* The APK was built by **Android Gradle Plugin 3.1.2** (`META-INF/MANIFEST.MF`:
  `Created-By: Android Gradle 3.1.2`), signed with **APK Signature Scheme v2 +
  v1 SHA1withRSA**, and packaged with a standard 4-byte `zipalign` (native libs
  **deflated**, not page-aligned). The Java build timestamp is
  **Mon Oct 22 10:18:17 CEST 2018** (`assets/crashlytics-build.properties`).
* The native lib is **NDK r18b, clang 7.0.2, gold 1.12, API 16, armeabi-v7a,
  Thumb-2, NEON, softfp, `c++_static`**, built on a **darwin** host (the
  `libc++abi.a` `abort_message.cpp` path is `/Volumes/Android/buildbot/...`).
* The effective compile/link recipe is the **NDK r18b CMake toolchain default
  set**, with `ANDROID_ABI=armeabi-v7a`, `ANDROID_PLATFORM=android-16`,
  `ANDROID_ARM_NEON=ON`, `ANDROID_STL=c++_static`,
  `ANDROID_CPP_FEATURES="rtti exceptions"`, `CMAKE_BUILD_TYPE=Release`,
  `CMAKE_CXX_STANDARD=11`. The two properties that distinguish it from vanilla
  ndk-build are both **CMake-toolchain defaults**: no `-fdata-sections`, and the
  STL archives after `target_link_libraries` (so FMOD precedes `libc++abi.a` and
  `_Unwind_*` stay `UND`).
* Every flag that is observable in the binary is **confirmed by a positive
  fingerprint**, not just by an A/B gate count: 787 stack canaries (exactly the
  original's), 2 direct `__cxa_guard_acquire` calls, 2 direct `__cxa_atexit`
  calls (+1 indirect), 0 sized-delete calls, 401 `tbb` + 84 `tbh`, 2473 frame-pointer prologues,
  9030 `movw`/31 `movt`, 4621 `strd`/2056 `ldrd`, 4184 `vldr`, 99 vtables,
  3566 `.ARM.exidx` entries with 272 CANTUNWIND sentinels, and `.rodata`/
  `.data`/`.data.rel.ro` **byte-identical**.
* The only flags that remain genuinely unobservable are the codegen-neutral
  ones (`-fPIC` vs `-fpic`, `-std=c++11` vs `gnu++11`, `-fmerge-constants`,
  `-fno-plt`, `-fno-delete-null-pointer-checks`, `-DNDEBUG` on this source,
  `-DANDROID` on this source). They are marked **unknown** below, not
  "disproven".

## 1. Method and baseline

```sh
cd /home/debian/GalaxyOnFire2/wt/w4306
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8
```

Baseline (measured before and after this pass):

```text
verify: byte 4499  linked 4499  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`size-exact` 4524/4524. Section sizes: `.text 0x1337c8 @ 0x69440`,
`.rodata 0x41b67 @ 0x1c9e10`, `.data 0x296c @ 0x214000`,
`.bss 0x69ac @ 0x216970`, `.ARM.exidx 0x6f70`, `.ARM.extab 0x26298`.

The A/B harness is a scratch toolchain that includes the real
`cmake/toolchain.cmake` and appends `GOF2_EXTRA_FLAGS` to `CMAKE_CXX_FLAGS` and
`CMAKE_C_FLAGS`:

```cmake
# /dev/shm/w4306/toolchain_extra.cmake
include("/home/debian/GalaxyOnFire2/wt/w4306/cmake/toolchain.cmake")
set(GOF2_EXTRA_FLAGS "" CACHE STRING "extra matching flags")
if(GOF2_EXTRA_FLAGS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GOF2_EXTRA_FLAGS}" CACHE STRING "" FORCE)
  set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   ${GOF2_EXTRA_FLAGS}" CACHE STRING "" FORCE)
endif()
```

Each variant is a fresh `/dev/shm/w4306/build-<tag>` and
`/dev/shm/w4306/verify-<tag>`; the measurement is the official
`tools/verify/verify.py` (never a proxy), with the same `--objdump` and
`--fail-on-wrong-type --top 0` flags the `check` target uses. The canonical
`cmake-build-match` was not modified; `--update-baseline` was never run.

## 2. APK archaeology

### 2.1 Manifest (binary AXML)

Parsed with a small AXML reader (the APK has no `aapt` dependency):

```sh
python3 - <<'PY'
import zipfile,struct
p="/home/debian/GalaxyOnFire2/Original APK/Galaxy on Fire 2™ HD_2.0.16_APKPure.apk"
d=zipfile.ZipFile(p).read("AndroidManifest.xml")
# ... AXML string pool + RES_XML_START_ELEMENT walk ...
PY
```

| attribute | value |
|---|---|
| `package` | `net.fishlabs.gof2hdallandroid2012` |
| `versionCode` | `47947032` |
| `versionName` | `2.0.16` |
| `minSdkVersion` | **16** |
| `targetSdkVersion` | **27** |
| `uses-feature glEsVersion` | `0x00020000` (GLES 2.0), required |
| `application` | no `android:debuggable`, no `android:extractNativeLibs` |
| support libs | `android.support.VERSION=26.1.0`, `android.arch.lifecycle.VERSION=27.0.0-SNAPSHOT` |

**Implication.** `minSdkVersion=16` is the compile/link API: it matches
`.note.android.ident` API 16 and the NDK r18b `android-16` sysroot. `targetSdk
27` is a Java-only value (it does not change the native build). The absence of
`extractNativeLibs` means the platform default (`true` for targetSdk 27) applies,
which is consistent with the compressed native lib below.

### 2.2 `lib/` contents

The APK has exactly one ABI directory and four libraries:

```text
lib/armeabi-v7a/libcrashlytics.so   202,580 B
lib/armeabi-v7a/libfmodevent.so     370,660 B
lib/armeabi-v7a/libfmodex.so        920,204 B
lib/armeabi-v7a/libgof2hdaa.so    2,187,328 B
```

No `arm64-v8a`, `x86`, `x86_64` or `mips`. **Implication:** `APP_ABI` /
`abiFilters` was `armeabi-v7a` only. The app's `DT_NEEDED` (below) names
`libfmodex.so`/`libfmodevent.so`, which are the two prebuilt FMOD libs shipped
in this same directory.

### 2.3 Zip entry compression and alignment

```sh
python3 - <<'PY'
import zipfile,struct
p=".../Galaxy on Fire 2™ HD_2.0.16_APKPure.apk"
z=zipfile.ZipFile(p)
for n in ["lib/armeabi-v7a/libgof2hdaa.so","resources.arsc"]:
    i=z.getinfo(n); f=open(p,'rb'); f.seek(i.header_offset)
    h=f.read(30); nl,el=struct.unpack_from('<HH',h,26)
    print(n, "method",i.compress_type,"data_off",i.header_offset+30+nl+el,
          "mod4",(i.header_offset+30+nl+el)%4,"mod4096",(i.header_offset+30+nl+el)%4096)
PY
```

| entry | method | data offset | mod 4 | mod 4096 |
|---|---|---:|---:|---:|
| `lib/armeabi-v7a/libgof2hdaa.so` | **deflate (8)** | 3,578,468 | 0 | 2660 |
| `lib/armeabi-v7a/libfmodex.so` | deflate (8) | 3,127,504 | 0 | 2256 |
| `lib/armeabi-v7a/libfmodevent.so` | deflate (8) | 2,960,805 | 1 | 3493 |
| `lib/armeabi-v7a/libcrashlytics.so` | deflate (8) | 2,861,037 | 1 | 2029 |
| `resources.arsc` | stored (0) | 6,187,956 | 0 | 2996 |
| all stored PNGs | stored (0) | — | **0** | not page-aligned |

**Implication.** The native lib is **compressed**, so the platform extracts it
at install (`extractNativeLibs=true`); it is not `mmap`ed from the APK. The
stored entries are 4-byte aligned, not 4096-byte aligned: this is the classic
`zipalign` (4-byte) packaging of a pre-AGP-3.6 Gradle build, not the later
`zipalign -p 4096` / `useLegacyPackaging=false` layout. Both are consistent with
the 2018 build date and AGP 3.1.2 below.

### 2.4 Build files in the APK/OBB

There are **no build files** anywhere in the APK. A scan of all 143 entries for
`Android.mk`, `Application.mk`, `CMakeLists.txt`, `*.cmake`, `*.gradle`,
`*.mk`, `*.ninja`, `*.o`, `*.a` finds only the four `.so` files. The `assets/`
tree is game data (`*.lang`, `items.bin`, `crashlytics-build.properties`,
firebase/fabric properties).

The only OBB available in the workspace is a *different* distribution
(`GOF2FHD_OBB.7z` → `main.47947006.net.fishlabs.gof2hdallandroid2012.obb`,
versionCode 47947006, not the APK's 47947032). It is a ZIP of 2,460 entries,
all under `47947_GOF2CONTENT_ETC/assets/...` (`*.aem` meshes, textures, sounds);
a scan for build files finds none. **Implication:** the native build system is
not shipped in either artifact; it must be inferred from the binary.

### 2.5 The APK names the Gradle plugin

`META-INF/MANIFEST.MF` (the v1 signature manifest) begins:

```text
Manifest-Version: 1.0
Built-By: Generated-by-ADT
Created-By: Android Gradle 3.1.2
```

`META-INF/CERT.SF` contains `X-Android-APK-Signed: 2`, and the APK has an
`APK Sig Block 42` with a v2 pair (`0x7109871a`, 843 bytes) and padding.

**Implication (proven).** The APK was packaged by **Android Gradle Plugin
3.1.2** (May 2018) with **APK Signature Scheme v2 + v1**. AGP 3.1.2 supports
both `externalNativeBuild { cmake { ... } }` and `externalNativeBuild { ndkBuild
{ ... } }`; the native flags below select CMake.

### 2.6 Build timestamp

`assets/crashlytics-build.properties`:

```text
#Mon Oct 22 10:18:17 CEST 2018
version_name=2.0.16
package_name=net.fishlabs.gof2hdallandroid2012
build_id=f527605b-309f-404d-8afc-b68027edc49e
version_code=47947032
app_name=GOF2
```

**Implication (likely).** The Java/APK build was **2018-10-22 10:18:17 CEST**.
The native lib carries no `__DATE__`/`__TIME__` string (see §4.1), so its exact
build time is not recoverable; it was almost certainly built in the same Gradle
invocation. The date is after NDK r18b's release (Sep 2018), which is consistent
with the `.note.android.ident` r18b stamp.

### 2.7 Packaging strip

The APK's `libgof2hdaa.so` is exactly the ground-truth file after GNU
`arm-linux-androideabi-strip --strip-unneeded`:

```sh
python3 - <<'PY'
import zipfile,hashlib
d=zipfile.ZipFile("...apk").read("lib/armeabi-v7a/libgof2hdaa.so")
open("/dev/shm/w4306/apk_lib.so","wb").write(d)
print(hashlib.sha256(d).hexdigest(), len(d))
PY
$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-strip \
  --strip-unneeded -o /dev/shm/w4306/apk_strip.so \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
cmp /dev/shm/w4306/apk_lib.so /dev/shm/w4306/apk_strip.so && echo IDENTICAL
```

Both are `6d38fd767ee3b4f8174c371bce371bfc6397cf76e13c1756938b2a94658e3b58`,
2,187,328 bytes. **Implication (proven).** The packaging strip is the NDK's GNU
`arm-linux-androideabi-strip` (ndk-build's default `APP_STRIP_MODE`), not
`llvm-strip`; the ground truth is the gold `-s`/unstripped-gold artifact before
that strip.

## 3. Binary archaeology

All commands below read
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(sha256 `d4c17be6…9483`, 2,187,328 B).

### 3.1 ELF header and program headers

```sh
readelf -hW  $O
readelf -lW  $O
readelf -dW  $O
```

| observable | value | implication |
|---|---|---|
| ELF | `ELF32`, `DYN`, `ARM`, `EABI5`, flags `0x5000200` (`EF_ARM_ABI_FLOAT_SOFT`) | **proven** armeabi-v7a **softfp** shared object |
| `PT_GNU_STACK` | `RW`, size 0, align 0x10 | **proven** non-exec stack (`-z noexecstack`) |
| `PT_GNU_RELRO` | covers `.fini_array .data.rel.ro .init_array .dynamic .got` | **proven** full RELRO (`-z relro`, implied by `-z now`) |
| `PT_ARM_EXIDX` | `.ARM.exidx` `0x6f70` | **proven** ARM EHABI unwind tables |
| `DT_NEEDED` | `liblog libGLESv2 libGLESv1_CM libEGL libandroid libfmodex libfmodevent libm libdl libc` | **proven** link-input order (FMOD before `m/dl/c`) |
| `DT_SONAME` | `libgof2hdaa.so` | **proven** `-soname libgof2hdaa.so` |
| `DT_FLAGS` / `DT_FLAGS_1` | `BIND_NOW` / `NOW` | **proven** `-z now` |
| `DT_RELCOUNT` | `1162` | **proven** `-z combreloc` (gold default) |
| `DT_RELSZ` / `DT_PLTRELSZ` | `28744` / `29392` | 3593 `R_ARM_*` + 3674 `R_ARM_JUMP_SLOT` |
| `DT_STRSZ` | `189208` | `.dynstr` size |
| `DT_SYMENT` | `16` | ELF32 dynsym entry |

### 3.2 Notes

```sh
readelf -nW $O
readelf -p .comment $O
```

**`.note.android.ident`** (offset `0x134`, 0x98 bytes):

```text
api=16  ndk="r18b"  build="5063045"
```

`5063045` is exactly NDK r18b's `Pkg.Revision = 18.1.5063045`
(`.cache/ndk/android-ndk-r18b/source.properties`). **Proven: NDK r18b, API 16.**

**`.comment`** (offset `0x21596c`, 0x12f bytes):

```text
Android (4751641 based on r328903) clang version 7.0.2
  (https://android.googlesource.com/toolchain/clang 003100370607242ddd5815e4a043907ea9004281)
  (https://android.googlesource.com/toolchain/llvm 1d739ffb0366421d383e04ff80ec2ee591315116)
  (based on LLVM 7.0.2svn)
GCC: (GNU) 4.9.x 20150123 (prerelease)
```

The clang string is the app's compiler; the GCC 4.9 string is the vendored
libcrypto (built with the NDK r13b GCC 4.9) and/or `libgcc.a`. Both are present
in the tree's archives (`strings libcrypto_gof2.a | grep 'GCC: (GNU)'` →
`GCC: (GNU) 4.9.x 20150123 (prerelease)`). **Proven: clang 7.0.2 + GCC 4.9
objects.**

**`.note.gnu.gold-version`** (offset `0x215a9c`): `gold 1.12`
(`GNU gold (GNU Binutils 2.27.0.20170315) 1.12`). **Proven: gold 1.12.**

**`.note.gnu.build-id`** (offset `0x1cc`, 0x24 bytes):
`c011810da45777b83ed721b2a87d517efed0d310`. SHA-1-style 20-byte note.
**Proven: `--build-id`.** The value is the hash of the *unstripped* gold output
(gold zeroes the descriptor and hashes the whole file), so it converges last.

### 3.3 `.ARM.attributes`

```sh
readelf -AW $O
```

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

**Implication (proven for the app TUs):** `-march=armv7-a -mthumb -mfpu=neon
-mfloat-abi=softfp`; `-fshort-wchar` off (wchar_t 4); `-fshort-enums` off
(enum int); `-mno-unaligned-access` off (unaligned v6); no `Tag_ABI_VFP_args`
(softfp). **Caveat:** the final `.so`'s `.ARM.attributes` is a *merge* over all
inputs, including libcrypto/libgcc. The app object's own attributes are the
authoritative per-TU evidence; e.g. `-mno-unaligned-access` on the app TU gives
`Tag_CPU_unaligned_access: None` in `GameText.cpp.obj` while the final `.so`
still says `v6` (from libcrypto). The A/B in §6 is the decisive test for those
two flags.

### 3.4 `.init_array` / `.fini_array`

```sh
python3 - <<'PY'
# read .init_array/.fini_array words and map to functions
PY
```

| array | entries | values | functions |
|---|---|---|---|
| `.init_array` | 3 | `0x69441 0x694c1 0x694dd` (Thumb) | `Engine.cpp` global init (`Array<int>`, `AbyssEngine::String` ×2, `__cxa_atexit`); touch-buffer init (`realloc(touches, 16)`); `ParticleSettings.cpp` (`ParticleSettingsRef::init/cur`, `__cxa_atexit`) |
| `.fini_array` | 2 | `0x69534 0x69548` (ARM) | `__do_global_dtors_aux` (`__cxa_finalize`); `frame_dummy` (register_tm_clones) |

**Implication (proven).** `-fuse-init-array` is on (no `.ctors`/`.dtors`), and
`crtbegin_so.o`/`crtend_so.o` are the NDK r18b API-16 objects (their `.fini_array`
functions are ARM, not Thumb). The three `.init_array` entries are byte-identical
to the tree's build.

### 3.5 Sections, flags and alignment

`readelf -SW` (27 sections; the tree's output is byte-identical in all 27
section headers):

| section | type | addr | off | size | ES | flags | align |
|---|---|---:|---:|---:|---:|---|---:|
| `.note.android.ident` | NOTE | 0x134 | 0x134 | 0x98 | 0 | A | 4 |
| `.note.gnu.build-id` | NOTE | 0x1cc | 0x1cc | 0x24 | 0 | A | 4 |
| `.dynsym` | DYNSYM | 0x1f0 | 0x1f0 | 0x15ca0 | 16 | A | 4 |
| `.dynstr` | STRTAB | 0x15e90 | 0x15e90 | 0x2e318 | 0 | A | 1 |
| `.hash` | HASH | 0x441a8 | 0x441a8 | 0x973c | 4 | A | 4 |
| `.gnu.version` | VERSYM | 0x4d8e4 | 0x4d8e4 | 0x2b94 | 2 | A | 2 |
| `.gnu.version_d` | VERDEF | 0x50478 | 0x50478 | 0x1c | 0 | A | 4 |
| `.gnu.version_r` | VERNEED | 0x50494 | 0x50494 | 0x40 | 0 | A | 4 |
| `.rel.dyn` | REL | 0x504d4 | 0x504d4 | 0x7048 | 8 | A | 4 |
| `.rel.plt` | REL | 0x5751c | 0x5751c | 0x72d0 | 8 | **AI** | 4 |
| `.plt` | PROGBITS | 0x5e7ec | 0x5e7ec | 0xac4c | 0 | AX | 4 |
| `.text` | PROGBITS | 0x69440 | 0x69440 | 0x1337c8 | 0 | AX | **32** |
| `.ARM.exidx` | ARM_EXIDX | 0x19cc08 | 0x19cc08 | 0x6f70 | 8 | AL | 4 |
| `.ARM.extab` | PROGBITS | 0x1a3b78 | 0x1a3b78 | 0x26298 | 0 | A | 4 |
| `.rodata` | PROGBITS | 0x1c9e10 | 0x1c9e10 | 0x41b67 | 0 | A | 16 |
| `.fini_array` | FINI_ARRAY | 0x20cba0 | 0x20bba0 | 0x8 | 4 | WA | 4 |
| `.data.rel.ro` | PROGBITS | 0x20cbb0 | 0x20bbb0 | 0x2fe8 | 0 | WA | 16 |
| `.init_array` | INIT_ARRAY | 0x20fb98 | 0x20eb98 | 0xc | 4 | WA | 4 |
| `.dynamic` | DYNAMIC | 0x20fba4 | 0x20eba4 | 0x148 | 8 | WA | 4 |
| `.got` | PROGBITS | 0x20fcf4 | 0x20ecf4 | 0x430c | 0 | WA | 4 |
| `.data` | PROGBITS | 0x214000 | 0x213000 | 0x296c | 0 | WA | 16 |
| `.bss` | NOBITS | 0x216970 | 0x21596c | 0x69ac | 0 | WA | 16 |
| `.comment` | PROGBITS | 0 | 0x21596c | 0x12f | 1 | MS | 1 |
| `.note.gnu.gold-version` | NOTE | 0 | 0x215a9c | 0x1c | 0 | — | 4 |
| `.ARM.attributes` | ARM_ATTRIBUTES | 0 | 0x215ab8 | 0x38 | 0 | — | 1 |
| `.shstrtab` | STRTAB | 0 | 0x215af0 | 0x117 | 0 | — | 1 |

**Implication.** `.hash` only, **no `.gnu.hash`** → gold's Android default
`--hash-style=sysv`. `.rel.plt` has `SHF_INFO_LINK` (`AI`) and `sh_info=20`
(`.got`), which is the darwin-host gold form; the tree's current linux-gold
output now matches it too. `.text` align 32 comes from a 32-byte-aligned input
(libcrypto's SHA assembly), not from a `-falign-functions=32` flag. `.bss`
`sh_offset=0x21596c` is the unaligned end of `.data`, as bfd writes it.

### 3.6 `.ARM.exidx` CANTUNWIND sentinels and `.ARM.extab`

```sh
python3 - <<'PY'
# entries = size/8; CANTUNWIND = (word1 == 1); inline = word1 bit31;
# extab = otherwise; duplicate target = prel31 address repeats
PY
```

| statistic | original | tree build |
|---|---:|---:|
| `.ARM.exidx` entries | 3566 | 3566 |
| CANTUNWIND (`w1 == 1`) | **272** | 272 |
| inline compact (`w1 bit31`) | 856 | 856 |
| extab records | 2438 | 2438 |
| duplicate target addresses | 194 | 194 |
| CANT entries sharing an address with a real entry | **193** | 193 |
| `.ARM.extab` size | `0x26298` (156,312 B) | `0x26298` |

**Implication (proven).** The 193 CANT+real pairs are gold's
`fix_exidx_coverage()` sentinels for the **empty plain `.text` section every
`-ffunction-sections` object carries**. A rebuild without `-ffunction-sections`
has only 52. The `.ARM.extab` size is exact.

### 3.7 Relocations

```sh
readelf -rW $O | awk '/R_ARM_/{print $3}' | sort | uniq -c
```

| relocation | count |
|---|---:|
| `R_ARM_JUMP_SLOT` | 3674 |
| `R_ARM_ABS32` | 1817 |
| `R_ARM_RELATIVE` | 1162 |
| `R_ARM_GLOB_DAT` | 614 |

No `R_ARM_REL32`, no `DT_TEXTREL`. **Implication:** `-fPIC`/`-fpic` codegen
(GOT-indirect globals, PLT calls); both `-fPIC` and `-fpic` are byte-identical
here, so the exact spelling is **unknown** (CMake uses `-fPIC`, ndk-build
`-fpic`).

### 3.8 Byte-identical sections (tree build vs original)

```sh
python3 - <<'PY'
# section-by-section byte compare of build-base/libgof2hdaa.so vs $O
PY
```

| section | size (ours/orig) | differing bytes |
|---|---:|---:|
| `.note.android.ident` | `0x98` / `0x98` | 0 |
| `.note.gnu.build-id` | `0x24` / `0x24` | **20** |
| `.dynsym` | `0x15ca0` / `0x15ca0` | 0 |
| `.dynstr` | `0x2e318` / `0x2e318` | 0 |
| `.hash` | `0x973c` / `0x973c` | 0 |
| `.gnu.version*` | `0x2b94` / `0x1c` / `0x40` | 0 |
| `.rel.dyn` | `0x7048` / `0x7048` | 0 |
| `.rel.plt` | `0x72d0` / `0x72d0` | 0 |
| `.plt` | `0xac4c` / `0xac4c` | 0 |
| `.text` | `0x1337c8` / `0x1337c8` | **106,772** |
| `.ARM.exidx` | `0x6f70` / `0x6f70` | **839** |
| `.ARM.extab` | `0x26298` / `0x26298` | **108,462** |
| `.rodata` | `0x41b67` / `0x41b67` | **0** |
| `.fini_array` | `0x8` / `0x8` | 0 |
| `.data.rel.ro` | `0x2fe8` / `0x2fe8` | **0** |
| `.init_array` | `0xc` / `0xc` | 0 |
| `.dynamic` | `0x148` / `0x148` | 0 |
| `.got` | `0x430c` / `0x430c` | 0 |
| `.data` | `0x296c` / `0x296c` | **0** |
| `.bss` | `0x69ac` / `0x69ac` | 0 |
| `.comment` | `0x12f` / `0x12f` | 0 |
| `.note.gnu.gold-version` | `0x1c` / `0x1c` | 0 |
| `.ARM.attributes` | `0x38` / `0x38` | 0 |
| `.shstrtab` | `0x117` / `0x117` | 0 |

All 27 section headers are byte-identical. **Implication:** the data layout,
dynamic tables and link metadata are already exact; the only remaining content
gaps are the 25 function bodies (`.text`), their EH records (`.ARM.exidx`/
`.ARM.extab`) and the build-id.

## 4. Codegen evidence for defines/macros

### 4.1 `__DATE__` / `__TIME__` / `__FILE__` / `assert`

```sh
strings -a -t x $O > so.strings
grep -nE '(Jan|Feb|...|Dec) [ 0-9][0-9] 20[0-9][0-9]' so.strings   # none
grep -nE '\b[0-2][0-9]:[0-5][0-9]:[0-5][0-9]\b' so.strings          # none
grep -nE '\.(cpp|cc|cxx|h|hpp|c|mk|cmake|gradle)(:|$)' so.strings
```

The only path-like strings are:

```text
1ca103 dataDirectory in main.c: %s
1ca11f rootPath in main.c: %s
1ca136 zipDirectory in main.c: %s
1e6b2b /Volumes/Android/buildbot/src/android/ndk-release-r18/external/libcxx/../../external/libcxxabi/src/abort_message.cpp
```

**Implication.** No `__DATE__`/`__TIME__` string survives (the app does not use
them, or only in dead code). The `main.c` strings are literal log format
strings, not `__FILE__`. The only compiler-generated source path is the
**darwin** libc++abi `abort_message.cpp` path, which proves the darwin NDK
package's `libc++abi.a` was linked (see `toolchain_archive_provenance_w3904.md`).
`__assert2` is imported but has **one** call site, inside libc++abi's
`std::bad_typeid::what()`; the app has no runtime `assert()` (its 744 `assert`
hits are `static_assert`). **NDEBUG is therefore unobservable from app codegen**
on this source; it is a CMake Release default (**likely**).

### 4.2 Thread-safe statics: `__cxa_guard_acquire`

```sh
readelf --dyn-syms -W $O | grep __cxa_guard
objdump -d $O | grep -c '__cxa_guard_acquire'
```

| symbol | value |
|---|---|
| `__cxa_guard_acquire` | defined, `0x1922a9`, 116 B |
| `__cxa_guard_release` | defined, `0x19231d`, 88 B |
| `__cxa_guard_abort` | defined, `0x192375`, 88 B |
| call sites | 2 acquire, 1 release, 1 abort (all in `GameText::getText(int)`, `0x81818`) |

The `GameText::getText` disassembly shows the full inline guard fast path
(`ldrb; dmb ish; tst #1; bne` → `blx __cxa_guard_acquire` → construct
`AbyssEngine::String` → `blx __cxa_atexit` → `blx __cxa_guard_release`).
**Proven: `-fthreadsafe-statics` (the default) is ON.** `-fno-threadsafe-statics`
drops byte_exact 4499 → 1368.

### 4.3 `__cxa_atexit` vs `atexit`

```sh
readelf --dyn-syms -W $O | grep -E '__cxa_atexit|\batexit\b'
objdump -d $O | grep -c '__cxa_atexit'
```

`__cxa_atexit@LIBC` is imported; `atexit` is **not imported at all**. There are
2 **direct** `__cxa_atexit@plt` call sites (one in `GameText::getText`, one in
the `ParticleSettings.cpp` initializer), plus the `Engine.cpp` initializer's
indirect call through a GOT-loaded function pointer (the `blx r4` at `0x69468`). **Proven: `-fuse-cxa-atexit` (the
default) is ON.** `-fno-use-cxa-atexit` drops byte_exact 4499 → 1356.

### 4.4 Sized deallocation

```sh
readelf --dyn-syms -W $O | grep -E '_ZdlPvj|_ZdaPvj'
objdump -d $O | grep -cE '(bl|blx).*<_ZdlPvj@plt>'
```

`_ZdlPvj`/`_ZdaPvj` are **defined** (weak, libc++abi's `stdlib_new_delete.o`)
but have **0 call sites**; the code calls `_ZdlPv` (1057) and `_ZdaPv` (249).
**Proven: `-fno-sized-deallocation` (the C++11 default) is in effect.** The
flag is gate-neutral (4499 → 4499, `.text`/`.rodata` identical), so it is a
positive default, not a matching knob.

### 4.5 RTTI / vtables

```sh
readelf --dyn-syms -W $O | grep -c '_ZTV'   # 99
readelf --dyn-syms -W $O | grep -c '_ZTI'   # 176
readelf --dyn-syms -W $O | grep -c '_ZTS'   # 176
```

`_ZTVN10__cxxabiv1*` vtables and `__class_type_info`/`__si_class_type_info`/
`__vmi_class_type_info`/`__pointer_type_info`/`__function_type_info`/
`__fundamental_type_info`/`__enum_type_info` typeinfo objects are present.
**Proven: `-frtti` is ON.** `-fno-rtti` drops byte_exact 4499 → 1701 (and
`.rodata` 0x41b67 → 0x4104e, `.data.rel.ro` 0x2fe8 → 0x2c1c).

### 4.6 Exceptions

```sh
readelf --dyn-syms -W $O | grep -E '__cxa_throw|__cxa_begin_catch|__gxx_personality'
objdump -d $O | grep -c '_Unwind_Resume'
```

`__cxa_throw` (3 calls), `__cxa_begin_catch` (12), `__cxa_end_catch` (10),
`_Unwind_Resume` (540), `__gxx_personality_v0`, and the 156,312-byte
`.ARM.extab` are present. **Proven: `-fexceptions` is ON.** `-fno-exceptions`
fails to build (`Level.cpp:5217: cannot use 'try' with exceptions disabled`).

### 4.7 Stack protector

```sh
objdump -d $O | grep -cE '(bl|blx).*<__stack_chk_fail@plt>'   # 787
readelf --dyn-syms -W $O | grep __stack_chk
```

`__stack_chk_fail@LIBC` (787 call sites) and `__stack_chk_guard@LIBC` are
imported. **Proven: `-fstack-protector-strong`.** The fingerprint is exact:
the tree's `-fstack-protector-strong` build has **787** canaries (identical to
the original), while `-fstack-protector` (basic) has **88** and
`-fstack-protector-all` has **3984**. A/B: strong 4499, basic 1377, all 91.

### 4.8 Unwind tables and `__gnu_Unwind_Find_exidx`

```sh
readelf -dW $O | grep -E 'ARM_EXIDX'
readelf --dyn-syms -W $O | grep -E '_Unwind_|__gnu_Unwind_Find_exidx'
```

`.ARM.exidx` is 3566 entries / `0x6f70`; `.ARM.extab` is 156,312 B. The
`_Unwind_*` symbols are **`UND`** (resolved from FMOD at runtime), and
`__gnu_Unwind_Find_exidx` is **absent**. **Proven: `-funwind-tables` is ON**,
and **`libunwind.a` was not pulled** (the FMOD shared libs precede the STL
archives on the link line). Removing `-funwind-tables` drops byte_exact
4499 → 2878 (`.ARM.exidx` 0x6f70 → 0x6678, `.ARM.extab` 0x26298 → 0x2556c).

### 4.9 `__aeabi_*` local definitions

```sh
readelf --dyn-syms -W $O | grep '__aeabi_'
```

All `__aeabi_*` are **`UND`**: `__aeabi_idiv`, `__aeabi_idivmod`,
`__aeabi_uidiv`, `__aeabi_uidivmod`, `__aeabi_uldivmod`, `__aeabi_l2f`,
`__aeabi_l2d`, `__aeabi_f2lz`, `__aeabi_f2ulz`, `__aeabi_ul2f`, `__aeabi_mem*`,
`__aeabi_unwind_cpp_pr0/pr1`. `__gnu_ldivmod_helper` is `UND`;
`__aeabi_ldivmod` is **not defined**. **Implication (proven):** the app did not
pull libgcc's helpers into the `.so`; they bind to **FMOD**, which exports all
of them (`readelf --dyn-syms libfmodex.so | grep __aeabi_`). This requires FMOD
to precede `libgcc.a`/`libunwind.a` on the link line — the CMake
`target_link_libraries` order, not ndk-build's static-before-shared order.

### 4.10 `__android_log_*`

```sh
readelf --dyn-syms -W $O | grep -E '__android_log'
objdump -d $O | grep -c '__android_log_print'
```

`__android_log_print` is `UND` (3 call sites). **Proven:** the app links
`-llog` and uses the NDK logging API; the `DT_NEEDED` order starts with
`liblog.so`.

### 4.11 Instruction-selection fingerprints (original vs tree build)

```sh
objdump -d --no-show-raw-insn $O > orig.dis
objdump -d --no-show-raw-insn build-base/libgof2hdaa.so > base.dis
for p in tbb tbh movw movt strd ldrd vldr vstr 'add\tr7, sp' ...; do
  grep -cE "$p" orig.dis; grep -cE "$p" base.dis; done
```

| fingerprint | original | tree build | implication |
|---|---:|---:|---|
| `tbb` | 401 | 400 | jump tables ON |
| `tbh` | 84 | 84 | jump tables ON |
| `movw` | 9030 | 9033 | `movw`/`movt` ON |
| `movt` | 31 | 31 | `movw`/`movt` ON |
| `strd` | 4621 | 4589 | `ldrd`/`strd` ON |
| `ldrd` | 2056 | 2045 | `ldrd`/`strd` ON |
| `vldr` | 4184 | 4179 | NEON/VFP ON |
| `vstr` | 2367 | 2369 | NEON/VFP ON |
| `add r7, sp, #…` | 2473 | 2473 | frame pointer ON |
| `__stack_chk_fail@plt` | 788 | 788 | stack protector strong |
| `__cxa_guard_acquire@plt` | 3 | 3 | thread-safe statics |
| `__cxa_atexit@plt` | 3 | 3 | cxa_atexit |
| `_ZdlPvj@plt` | 0 | 0 | no sized delete |
| `_ZdlPv@plt` | 1059 | 1059 | unsized delete |
| `_ZdaPv@plt` | 253 | 253 | unsized delete[] |

The `@plt` rows count the PLT label plus the call sites; the call-site counts
are 787 `__stack_chk_fail`, 2 `__cxa_guard_acquire` and 2 direct
`__cxa_atexit`. The small deltas (1 `tbb`, 3 `movw`, 32 `strd`, 11 `ldrd`,
5 `vldr`, 2 `vstr`) are consistent with the 25 non-byte-exact function bodies
(not individually attributed here); the *selection* of each instruction family
is essentially identical. **Implication:** no missing instruction-set
flag. `-fno-jump-tables` changes `tbb`/`tbh` to 343/11; `-marm` changes the
whole file (byte_exact 57); `-fno-slp-vectorize` drops to 1492.

## 5. Cross-check: iOS/mac/windows source dumps

`../source file list dump/source file lists dump.txt` and the three per-platform
symbol dumps (`gof2hd_1.1.19_ios.cpp`, `gof2sd_1.0.1_ios.cpp`,
`gof3d_1.1.4_ios.cpp`) are **source/TU lists with symbol info**, not build
scripts. They confirm:

* the original flat tree (`AE2/Classes/`, `Classes/Game/`,
  `Classes/Game/ParticleSystems/`, `Classes/Game/MeshMerger/`,
  `Classes/Shader/`) that `_work/original_layout/` already encodes;
* the iOS build was **Xcode Release-iphoneos arm64** (the dump embeds
  `.../DerivedData/GalaxyOnFire2-.../Build/Intermediates.noindex/ArchiveIntermediates/
  Galaxy on Fire 2 HD/.../Release-iphoneos/.../Objects-normal/arm64/*.o`);
* the mac build was `/Users/hck/Documents/Dev/GalaxyOnFire2_merged/...`;
* the windows build was a 7-file partial list (`Classes\PaintCanvas.cpp`,
  `Classes\Engine_DirectX.cpp`, …).

They contain **no Android build files and no compiler flags**. The iOS dump's
`Release-iphoneos`/`arm64`/`DerivedData` strings are the only build-system
evidence in the dumps, and they are iOS-specific. **Implication:** the source
dumps are authoritative for TU membership (already used by the tree) but give
no positive evidence for any Android flag; the Android evidence is the binary
and the APK.

## 6. A/B matrix (official verifier, wave-93 tree)

All variants are one flag on top of the canonical `-Oz -g -fPIC -frtti
-fstack-protector-strong -ffunction-sections -funwind-tables -DANDROID -DNDEBUG
-std=c++11` build. `size` is `sum(osize == usize)` over the verifier rows;
`.text`/`.rodata` are `readelf -SW` sizes of the linked `.so`. The canonical
baseline is **4499 / 4499 / 4524 / 99.9**, `.text 0x1337c8`, `.rodata 0x41b67`.

| variant | flag | byte | linked | size | avg | .text | .rodata | .ARM.exidx | .ARM.extab |
|---|---|---:|---:|---:|---:|---:|---:|---:|---:|
| base | — | **4499** | **4499** | 4524/4524 | 99.9 | `0x1337c8` | `0x41b67` | `0x6f70` | `0x26298` |
| fdata | `-fdata-sections` | 4311 | 4499 | 4524/4524 | 99.9 | `0x1337c8` | `0x41a07` | `0x6f70` | `0x26298` |
| fnosizeddealloc | `-fno-sized-deallocation` | **4499** | **4499** | 4524/4524 | 99.9 | `0x1337c8` | `0x41b67` | `0x6f70` | `0x26298` |
| fnothreadsafestatics | `-fno-threadsafe-statics` | 1368 | 4495 | 4520/4521 | 99.89 | `0x133684` | `0x419b7` | `0x6f50` | `0x2625c` |
| fnocxaatexit | `-fno-use-cxa-atexit` | 1356 | 4498 | 4523/4524 | 99.9 | `0x133818` | `0x41b67` | `0x6f80` | `0x2628c` |
| stackprot | `-fstack-protector` | 1377 | 3785 | 3793/4524 | 93.5 | `0x12e168` | `0x41b67` | `0x6f18` | `0x2607c` |
| stackprotall | `-fstack-protector-all` | 91 | 963 | 981/4524 | 45.97 | `0x14f728` | `0x41b67` | `0x7230` | `0x270bc` |
| fnortti | `-fno-rtti` | 1701 | 4499 | 4524/4524 | 99.9 | `0x1337c8` | `0x4104e` | `0x6f70` | `0x26298` |
| fnojumptables | `-fno-jump-tables` | 1363 | 4411 | 4436/4524 | 99.13 | `0x134828` | `0x41947` | `0x6f78` | `0x262b8` |
| fnoslpvectorize | `-fno-slp-vectorize` | 1492 | 4385 | 4408/4524 | 99.26 | `0x132e88` | `0x41b67` | `0x6f70` | `0x26288` |
| mnounaligned | `-mno-unaligned-access` | 1391 | 4444 | 4471/4524 | 99.68 | `0x1334c8` | `0x41b67` | `0x6f70` | `0x2629c` |
| fshortenums | `-fshort-enums` | 2869 | 4477 | 4511/4524 | 99.84 | `0x1337c8` | `0x41b67` | `0x6f70` | `0x26298` |
| fomitfp | `-fomit-frame-pointer` | 1298 | 1922 | 2163/4524 | 84.09 | `0x12dfc8` | `0x41b67` | `0x6868` | `0x214f4` |
| fnoinline | `-fno-inline` | 1219 | 3511 | 3571/4524 | 89.02 | `0x13c0c8` | `0x41b67` | `0x7858` | `0x26998` |
| fnostrictalias | `-fno-strict-aliasing` | 1371 | 4350 | 4411/4524 | 98.98 | `0x133ce8` | `0x41b67` | `0x6f80` | `0x262a0` |
| fwrapv | `-fwrapv` | 1398 | 4463 | 4500/4524 | 99.8 | `0x1338c8` | `0x41b67` | `0x6f70` | `0x26298` |
| ffastmath | `-ffast-math` | 1319 | 4225 | 4369/4524 | 98.59 | `0x132998` | `0x41b67` | `0x6f58` | `0x26280` |
| vishidden | `-fvisibility=hidden` | 57 | 243 | 244/244 | 99.92 | `0x130f38` | `0x41b67` | `0x6f70` | `0x26280` |
| g0 | `-g0` | 2069 | 4246 | 4485/4524 | 99.55 | `0x1337c8` | `0x41b67` | `0x6f70` | `0x26298` |
| marm | `-marm` | 57 | 237 | 342/4524 | 39.06 | `0x1a3618` | `0x41b67` | `0x6eb8` | `0x25b18` |
| stdcxx14 | `-std=c++14` | 1512 | 4486 | 4514/4524 | 99.86 | `0x133788` | `0x41b67` | `0x6f70` | `0x26298` |
| nounwind_filter | remove `-funwind-tables` | 2878 | 4499 | 4524/4524 | 99.9 | `0x1337c8` | `0x41b67` | `0x6678` | `0x2556c` |
| Os | `-Os` | 1263 | 2650 | 2883/4356 | 84.54 | `0x14bf28` | `0x43057` | `0x6a30` | `0x24770` |
| fnocommon | `-fno-common` | **LINK FAIL** | | | | | | | |
| mfpu_vfpv3d16 | `-mfpu=vfpv3-d16` (after `-mfpu=neon`) | **BUILD FAIL** | | | | | | | |
| mhardfloat | `-mfloat-abi=hard` | **LINK FAIL** | | | | | | | |
| fnoexceptions | `-fno-exceptions` | **BUILD FAIL** | | | | | | | |

Notes:

* `-fno-sized-deallocation` is **byte-identical to base except the build-id**
  (all section bytes 0 diff; the build-id differs because it hashes the
  unstripped output, including debug info/symbol data that records the flag).
* `-fdata-sections` is the build-system discriminator: `.rodata` shrinks
  `0x41b67 → 0x41a07` and differs by **113,935 bytes**, `.data` differs by
  3,023, `.data.rel.ro` by 365, `.dynsym` by 753, `.rel.dyn` by 180.
* `-fno-common` fails with `multiple definition of 'g_android_back_button_pressed'`
  (`GOF2HD2012.c.obj` vs `BackButton.c.obj`); the original has the symbol once
  at `0x21d2fc` (4 B, `.bss`).
* `-mfpu=vfpv3-d16` fails because the app source uses NEON intrinsics
  (`arm_neon.h: NEON support not enabled`); `-mfloat-abi=hard` fails at link
  (`uses VFP register arguments, output does not`) against the softfp FMOD libs.
* `-fno-exceptions` fails at `Level.cpp:5217: cannot use 'try' with exceptions
  disabled`.

## 7. Observable → value → implication

| observable | value | implication | verdict |
|---|---|---|---|
| APK `MANIFEST.MF` `Created-By` | `Android Gradle 3.1.2` | APK built by AGP 3.1.2 | **proven** |
| APK `CERT.SF` `X-Android-APK-Signed` | `2` | v2 signing | **proven** |
| APK `lib/` | only `armeabi-v7a`, 4 libs | `APP_ABI=armeabi-v7a` | **proven** |
| APK `.so` zip method | deflate (8) | native lib extracted at install | **proven** |
| APK stored-entry alignment | mod 4 = 0, not page-aligned | standard `zipalign` (4-byte) | **proven** |
| APK build timestamp | `Mon Oct 22 10:18:17 CEST 2018` | build date | **likely** (Java; native same build) |
| APK build files | none | build system not shipped | **proven** |
| OBB build files | none (2,460 asset entries) | build system not shipped | **proven** |
| ELF machine/flags | `ELF32 DYN ARM`, `0x5000200` | armeabi-v7a softfp | **proven** |
| `.note.android.ident` | api 16, `r18b`, build `5063045` | NDK r18b, API 16 | **proven** |
| `.comment` | clang 7.0.2 + GCC 4.9.x | clang 7.0.2 app + GCC 4.9 libcrypto/libgcc | **proven** |
| `.note.gnu.gold-version` | `gold 1.12` | gold 1.12 | **proven** |
| `.note.gnu.build-id` | 20-byte SHA-1 | `--build-id` | **proven** |
| `.ARM.attributes` | ARM v7, Thumb-2, VFPv3, NEONv1, wchar 4, enum int, unaligned v6 | `-march=armv7-a -mthumb -mfpu=neon -mfloat-abi=softfp`, no `-fshort-wchar/-fshort-enums/-mno-unaligned-access` | **proven** (app object) |
| `.init_array` / `.fini_array` | 3 / 2 entries | `-fuse-init-array`, NDK crtbegin/crtend | **proven** |
| `DT_NEEDED` | `log GLESv2 GLESv1_CM EGL android fmodex fmodevent m dl c` | FMOD before `m/dl/c` | **proven** |
| `DT_FLAGS`/`DT_FLAGS_1` | `BIND_NOW`/`NOW` | `-z now` | **proven** |
| `PT_GNU_RELRO` | covers arrays/dynamic/got | `-z relro` | **proven** |
| `PT_GNU_STACK` | `RW` | `-z noexecstack` | **proven** |
| `.hash` only | no `.gnu.hash` | `--hash-style=sysv` (gold default) | **proven** |
| `.rel.plt` flags/info | `AI`/20 | darwin-host gold form | **proven** (matches) |
| `.ARM.exidx` | 3566 entries, 272 CANT, 193 CANT+real pairs | `-ffunction-sections` + `-funwind-tables` | **proven** |
| `.ARM.extab` | `0x26298` | exceptions/unwind tables | **proven** |
| relocations | no `R_ARM_REL32`, `R_ARM_GLOB_DAT` 614 | PIC codegen; `-fPIC` vs `-fpic` unobservable | **proven** (PIC) / **unknown** (spelling) |
| `.rodata`/`.data`/`.data.rel.ro` | 0 differing bytes | no `-fdata-sections` | **proven** |
| `__cxa_guard_acquire` | defined + 2 call sites | thread-safe statics ON | **proven** |
| `__cxa_atexit` | imported + 2 direct call sites (+1 indirect); no `atexit` | `-fuse-cxa-atexit` ON | **proven** |
| `_ZdlPvj`/`_ZdaPvj` | defined, 0 call sites | `-fno-sized-deallocation` | **proven** |
| RTTI | 99 `_ZTV`, 176 `_ZTI`, 176 `_ZTS` | `-frtti` ON | **proven** |
| exceptions | `__cxa_throw`, `_Unwind_Resume` 540, `.ARM.extab` | `-fexceptions` ON | **proven** |
| `__stack_chk_fail` | 787 call sites | `-fstack-protector-strong` | **proven** (exact count) |
| `_Unwind_*` / `__gnu_Unwind_Find_exidx` | `UND` / absent | no `libunwind.a` pulled; FMOD first | **proven** |
| `__aeabi_*` | all `UND`; `__aeabi_ldivmod` not defined | libgcc helpers bind to FMOD | **proven** |
| `__android_log_print` | `UND`, 3 calls | `-llog` | **proven** |
| `__DATE__`/`__TIME__` | no date/time strings | not used / dead | **unknown** |
| `__FILE__` | only `main.c` literals + libc++abi path | no app `__FILE__` | **likely** |
| `assert` | 1 `__assert2` call (libc++abi) | app NDEBUG or no asserts | **unknown** (source has 0 runtime asserts) |
| `-DANDROID` | no `#ifdef ANDROID` in src | unobservable | **unknown** |
| `-DNDEBUG` | no app asserts | unobservable | **unknown** (CMake Release default) |
| `-fPIC` vs `-fpic` | identical A/B | either | **unknown** |
| `-std=c++11` vs `gnu++11` | identical A/B | either | **unknown** |
| `-fmerge-constants` | neutral A/B | default | **unknown** |
| `-fno-plt` | neutral A/B | default | **unknown** |
| `-fno-delete-null-pointer-checks` | neutral A/B | default | **unknown** |
| `-fno-common` | link fails on this source | `-fcommon` (default) | **likely** |
| `-fdata-sections` | 4311 vs 4499; `.rodata` 113,935 diff | **not used** | **disproven** |
| `-Os` | 1263 vs 4499 | `-Oz` used | **disproven** |
| `-O1/-O2/-O3` | 1033/1168/1167 (prior notes) | `-Oz` used | **disproven** |
| `-g0` | 2069 vs 4499 | `-g` used | **disproven** |
| `-marm` | 57 vs 4499 | `-mthumb` used | **disproven** |
| `-mfpu=vfpv3-d16` | build fail (NEON intrinsics) | `-mfpu=neon` used | **disproven** |
| `-mfloat-abi=hard` | link fail | `-mfloat-abi=softfp` used | **disproven** |
| `-mno-unaligned-access` | 1391 vs 4499 | unaligned access ON | **disproven** |
| `-fshort-enums` | 2869 vs 4499 | int enums | **disproven** |
| `-fomit-frame-pointer` | 1298 vs 4499 | frame pointers ON | **disproven** |
| `-fno-inline` | 1219 vs 4499 | inlining ON | **disproven** |
| `-fno-strict-aliasing` | 1371 vs 4499 | strict aliasing ON | **disproven** |
| `-fwrapv` | 1398 vs 4499 | no `-fwrapv` | **disproven** |
| `-ffast-math` | 1319 vs 4499 | no fast-math | **disproven** |
| `-fvisibility=hidden` | 57 vs 4499 | default visibility | **disproven** |
| `-fno-jump-tables` | 1363 vs 4499 | jump tables ON | **disproven** |
| `-fno-slp-vectorize` | 1492 vs 4499 | SLP vectorization ON | **disproven** |
| `-fno-threadsafe-statics` | 1368 vs 4499 | thread-safe statics ON | **disproven** |
| `-fno-use-cxa-atexit` | 1356 vs 4499 | cxa_atexit ON | **disproven** |
| `-fno-rtti` | 1701 vs 4499 | RTTI ON | **disproven** |
| `-fno-exceptions` | build fail | exceptions ON | **disproven** |
| `-std=c++14` | 1512 vs 4499 | C++11 | **disproven** |
| `-fno-unwind-tables` | 2878 vs 4499 | unwind tables ON | **disproven** |
| `-fstack-protector` (basic) | 1377 vs 4499 | strong, not basic | **disproven** |
| `-fstack-protector-all` | 91 vs 4499 | strong, not all | **disproven** |
| `-fno-sized-deallocation` | 4499 (neutral) | C++11 default | **proven** (positive default) |

## 8. Reconstructed build files

### 8.1 The proven CMake recipe

```cmake
# CMakeLists.txt
cmake_minimum_required(VERSION 3.6)
project(gof2hdaa C CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(gof2hdaa SHARED
    # the original flat source list, in the original link order:
    #   Classes/*.cpp, Classes/Game/*.cpp,
    #   Classes/Game/ParticleSystems/*.cpp, Classes/Game/MeshMerger/*.cpp,
    #   Classes/Shader/*.cpp, ...
)

# zlib/libzip were exported whole; libcrypto is a partial archive.
target_link_libraries(gof2hdaa
    zlib zip crypto
    log GLESv2 GLESv1_CM EGL android
    ${FMOD_DIR}/libfmodex.so
    ${FMOD_DIR}/libfmodevent.so
    m dl)
```

Configure with the NDK r18b CMake toolchain:

```sh
cmake -S . -B build -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=$NDK/build/cmake/android.toolchain.cmake \
  -DANDROID_ABI=armeabi-v7a \
  -DANDROID_PLATFORM=android-16 \
  -DANDROID_ARM_NEON=ON \
  -DANDROID_ARM_MODE=thumb \
  -DANDROID_STL=c++_static \
  -DANDROID_CPP_FEATURES="rtti exceptions" \
  -DCMAKE_BUILD_TYPE=Release
```

This emits exactly the observed compile line:

```text
clang++ --target=armv7-none-linux-androideabi16 \
  --gcc-toolchain=$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/darwin-x86_64 \
  --sysroot=$NDK/sysroot -Dt_EXPORTS \
  -isystem $NDK/sources/cxx-stl/llvm-libc++/include \
  -isystem $NDK/sources/android/support/include \
  -isystem $NDK/sources/cxx-stl/llvm-libc++abi/include \
  -isystem $NDK/sysroot/usr/include/arm-linux-androideabi \
  -g -DANDROID -ffunction-sections -funwind-tables -fstack-protector-strong \
  -no-canonical-prefixes -march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16 \
  -mthumb -mfpu=neon -Wa,--noexecstack -Wformat -Werror=format-security \
  -std=c++11 -frtti -fexceptions -Oz -DNDEBUG -std=gnu++11 -fPIC \
  -c file.cpp -o file.o
```

and the observed link order:

```text
clang++ --target=armv7-none-linux-androideabi16 ... \
  -Wl,--exclude-libs,libgcc.a -Wl,--exclude-libs,libatomic.a -nostdlib++ \
  -Wl,--build-id -Wl,--warn-shared-textrel -Wl,--fatal-warnings \
  -Wl,--fix-cortex-a8 -Wl,--exclude-libs,libunwind.a \
  -Wl,--no-undefined -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now \
  -shared -Wl,-soname,libgof2hdaa.so -o libgof2hdaa.so \
  crtbegin_so.o <objects in original source order> \
  --whole-archive libz.a libzip.a --no-whole-archive libcrypto.a \
  -llog -lGLESv2 -lGLESv1_CM -lEGL -landroid \
  $APK/lib/armeabi-v7a/libfmodex.so $APK/lib/armeabi-v7a/libfmodevent.so \
  -lm -ldl \
  -latomic -lm libc++_static.a libc++abi.a libandroid_support.a libunwind.a -ldl \
  crtend_so.o
```

The key non-obvious points are both **CMake toolchain defaults**:

1. **No `-fdata-sections`** (the CMake toolchain adds only
   `-ffunction-sections`; ndk-build adds both). This is why `.rodata`/`.data`/
   `.data.rel.ro` are byte-identical to the no-`-fdata-sections` build and
   differ by 113,935 / 3,023 / 365 bytes with it.
2. **STL archives after `target_link_libraries`** (so FMOD precedes
   `libc++abi.a`/`libgcc.a`). FMOD exports `_Unwind_*` and `__aeabi_*`, so they
   stay `UND` and `__gnu_Unwind_Find_exidx` is absent. ndk-build puts static
   archives before shared libs, pulls `libunwind.a`, and defines `_Unwind_*`
   locally (1345 byte-exact, `.text 0x135d0c`).

### 8.2 The ndk-build equivalent (not the original)

If one insists on `Android.mk`/`Application.mk`, the following reproduces the
compile flags but needs two non-default adjustments to match the link:

```make
# jni/Application.mk
APP_ABI      := armeabi-v7a
APP_PLATFORM := android-16
APP_OPTIM    := release
APP_STL      := c++_static
APP_CPPFLAGS := -frtti -fexceptions
# ndk-build adds -fdata-sections; the original does not have it.
APP_CFLAGS   := -fno-data-sections
```

```make
# jni/Android.mk
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE    := gof2hdaa
LOCAL_SRC_FILES := <original flat source list>
LOCAL_ARM_NEON  := true
LOCAL_CPP_FEATURES := rtti exceptions
LOCAL_STATIC_LIBRARIES := zlib zip crypto
LOCAL_LDLIBS := -llog -lGLESv2 -lGLESv1_CM -lEGL -landroid -lm -ldl
LOCAL_SHARED_LIBRARIES := fmodex fmodevent
include $(BUILD_SHARED_LIBRARY)
```

Even with `-fno-data-sections`, vanilla ndk-build still puts the STL archives
before the shared libraries, so it pulls `libunwind.a` and defines `_Unwind_*`
locally. To match the original with ndk-build one must also force the link order
(e.g. `APP_STL := none` + a manual `libc++abi.a` after FMOD, or a custom link
rule). That is exactly the CMake ordering, which is why CMake is the
parsimonious reconstruction.

## 9. Verdict per hypothesis

| hypothesis | verdict | evidence |
|---|---|---|
| Original is NDK r18b clang 7.0.2, gold 1.12, API 16, armeabi-v7a softfp, Thumb-2, NEON, `c++_static` | **proven** | `.comment`, `.note.android.ident`, `.note.gnu.gold-version`, `.ARM.attributes`, `DT_NEEDED` |
| Original was built on a darwin host | **proven** | `/Volumes/Android/buildbot/.../abort_message.cpp` in `.rodata` (darwin `libc++abi.a`) |
| Original used the NDK r18b CMake toolchain defaults | **likely** | no `-fdata-sections`; STL after `target_link_libraries`; both are CMake defaults, both contradicted by ndk-build |
| Original was built by AGP 3.1.2 | **proven** | `META-INF/MANIFEST.MF: Created-By: Android Gradle 3.1.2` |
| Original used `externalNativeBuild { cmake { ... } }` (not `ndkBuild`) | **likely** | native flags/order are CMake-toolchain defaults; AGP 3.1.2 supports both |
| Build date is 2018-10-22 | **likely** | Crashlytics `Mon Oct 22 10:18:17 CEST 2018`; native lib has no date string |
| `-Oz` | **proven** | A/B: `-Os` 1263, `-O1` 1033, `-O2` 1168, `-O3` 1167 vs 4499 |
| `-g` | **proven** | A/B: `-g0` 2069 vs 4499 |
| `-fstack-protector-strong` | **proven** | 787 canaries exact; basic 88, all 3984 |
| `-ffunction-sections` | **proven** | 193 CANT+real `.ARM.exidx` sentinels |
| `-funwind-tables` | **proven** | 3566 exidx entries; filtered removal 2878 |
| `-fdata-sections` | **disproven** | `.rodata` 0 diff without, 113,935 with; A/B 4311 |
| `-frtti` | **proven** | 99 vtables, 176 typeinfo; `-fno-rtti` 1701 |
| `-fexceptions` | **proven** | `.ARM.extab`, `__cxa_throw`, `_Unwind_Resume`; `-fno-exceptions` build fail |
| `-fthreadsafe-statics` | **proven** | guard symbols + call sites; `-fno-threadsafe-statics` 1368 |
| `-fuse-cxa-atexit` | **proven** | `__cxa_atexit` imported/called; no `atexit`; flag off 1356 |
| `-fno-sized-deallocation` | **proven** | 0 `_ZdlPvj`/`_ZdaPvj` calls; flag neutral |
| `-fPIC` vs `-fpic` | **unknown** | no `R_ARM_REL32`; A/B identical |
| `-std=c++11` vs `gnu++11` | **unknown** | A/B identical; `-std=c++14` 1512 |
| `-fmerge-constants` | **unknown** | neutral A/B |
| `-fno-plt` | **unknown** | neutral A/B |
| `-fno-delete-null-pointer-checks` | **unknown** | neutral A/B |
| `-DNDEBUG` | **unknown** | no app runtime asserts; CMake Release default |
| `-DANDROID` | **unknown** | no `#ifdef ANDROID` in src; both CMake/ndk-build add it |
| `-fcommon` (default) | **likely** | `-fno-common` link-fails on the duplicate tentative `g_android_back_button_pressed`; original has it once |
| `-mno-unaligned-access` | **disproven** | app object attr `v6`; A/B 1391 |
| `-fshort-enums` | **disproven** | app object attr `int`; A/B 2869 |
| `-fshort-wchar` | **disproven** | app object attr `wchar_t 4`; A/B neutral |
| `-fomit-frame-pointer` | **disproven** | 2473 frame-pointer prologues; A/B 1298 |
| `-fno-inline` | **disproven** | A/B 1219 |
| `-fno-strict-aliasing` | **disproven** | A/B 1371 |
| `-fwrapv` | **disproven** | A/B 1398 |
| `-ffast-math` | **disproven** | A/B 1319 |
| `-fvisibility=hidden` | **disproven** | 5370 defined exports; A/B 57 |
| `-fno-jump-tables` | **disproven** | 401 `tbb` + 84 `tbh`; A/B 1363 |
| `-fno-slp-vectorize` | **disproven** | A/B 1492 |
| `-marm` | **disproven** | Thumb-2 attr; A/B 57 |
| `-mfpu=vfpv3-d16` | **disproven** | build fail (NEON intrinsics); NEONv1 attr |
| `-mfloat-abi=hard` | **disproven** | link fail; ELF soft-float flag |
| `-fno-common` | **disproven** | link fail; original links |
| `-fno-exceptions` | **disproven** | build fail |
| `-fno-rtti` | **disproven** | A/B 1701 |
| `-fno-threadsafe-statics` | **disproven** | A/B 1368 |
| `-fno-use-cxa-atexit` | **disproven** | A/B 1356 |
| `-fno-unwind-tables` | **disproven** | A/B 2878 |
| `-std=c++14` | **disproven** | A/B 1512 |
| `-Os` | **disproven** | A/B 1263 |

## 10. What remains

1. **The exact build-system file** (`CMakeLists.txt` vs a hand-written script
   that copies the CMake toolchain flags) is not recoverable from the stripped
   `.so` or the APK. The effective recipe is the CMake toolchain's; the
   parsimonious reconstruction is CMake.
2. **The CMake and Gradle versions** are not in the APK. AGP 3.1.2 is proven;
   the Gradle wrapper version and CMake version are unknown (AGP 3.1.x's
   bundled CMake is 3.6.4111459 by general knowledge, but that is not
   observable from the APK or the `.so`).
3. **The exact native build time** is not in the `.so` (no `__DATE__`/`__TIME__`);
   the Java build time is 2018-10-22 10:18:17 CEST.
4. **Codegen-neutral flags** (`-fPIC`/`-fpic`, `-std=c++11`/`gnu++11`,
   `-fmerge-constants`, `-fno-plt`, `-fno-delete-null-pointer-checks`,
   `-DNDEBUG`, `-DANDROID`) cannot be distinguished by this binary. They are
   **unknown**, not wrong.
5. **The remaining byte gaps** are source/TU shape, not flags: `.text` 106,772
   differing bytes across the 25 non-byte-exact functions, `.ARM.exidx` 839,
   `.ARM.extab` 108,462, `.note.gnu.build-id` 20. `.rodata`, `.data`,
   `.data.rel.ro`, `.dynsym`, `.dynstr`, `.hash`, `.rel.dyn`, `.rel.plt`,
   `.got`, `.dynamic`, `.plt`, `.init_array`, `.fini_array`, `.comment`,
   `.note.*`, `.ARM.attributes` and `.shstrtab` are already byte-identical.

## 11. Reproduction commands

```sh
ROOT=/home/debian/GalaxyOnFire2/wt/w4306
O=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
APK="/home/debian/GalaxyOnFire2/Original APK/Galaxy on Fire 2™ HD_2.0.16_APKPure.apk"
NDK=$ROOT/.cache/ndk/android-ndk-r18b

# baseline gate
cd $ROOT
cmake --preset default -DGOF2_ORIGINAL_SO=$O \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8

# APK manifest / libs / zip alignment
python3 - <<'PY'
import zipfile,struct
p=".../Galaxy on Fire 2™ HD_2.0.16_APKPure.apk"
z=zipfile.ZipFile(p)
print(z.read("META-INF/MANIFEST.MF").decode()[:200])
print(z.read("assets/crashlytics-build.properties").decode())
for i in z.infolist():
    if i.filename.startswith("lib/") or i.compress_type==0:
        f=open(p,'rb'); f.seek(i.header_offset); h=f.read(30)
        nl,el=struct.unpack_from('<HH',h,26)
        off=i.header_offset+30+nl+el
        print(i.filename, i.compress_type, off, off%4, off%4096)
PY

# binary metadata
readelf -hW  $O
readelf -lW  $O
readelf -dW  $O
readelf -nW  $O
readelf -p .comment $O
readelf -AW $O
readelf -SW $O
readelf -rW $O | awk '/R_ARM_/{print $3}' | sort | uniq -c
readelf --dyn-syms -W $O > /dev/shm/w4306/dynsym.txt

# codegen fingerprints
OBJ=$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OBJ -d --no-show-raw-insn $O > /dev/shm/w4306/orig.dis
for p in tbb tbh movw movt strd ldrd vldr vstr 'add\tr7, sp' \
         '__stack_chk_fail@plt' '__cxa_guard_acquire@plt' '__cxa_atexit@plt' \
         '_ZdlPvj@plt' '_ZdlPv@plt' '_ZdaPv@plt'; do
  echo "$p $(grep -cE "$p" /dev/shm/w4306/orig.dis)"
done

# exidx / extab
python3 - <<'PY'
import struct
from collections import Counter
p="/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so"
b=open(p,'rb').read()
e_shoff=struct.unpack_from('<I',b,0x20)[0]; e_shentsize=struct.unpack_from('<H',b,0x2e)[0]
e_shnum=struct.unpack_from('<H',b,0x30)[0]; e_shstrndx=struct.unpack_from('<H',b,0x32)[0]
sh=[struct.unpack_from('<IIIIIIIIII',b,e_shoff+i*e_shentsize) for i in range(e_shnum)]
st=sh[e_shstrndx]; stb=b[st[4]:st[4]+st[5]]
names={stb[s[0]:stb.find(b'\0',s[0])].decode():s for s in sh}
s=names['.ARM.exidx']; off,size=s[4],s[5]; n=size//8
cant=inline_=extab=0; t0=[]; t1=[]
for i in range(n):
    w0,w1=struct.unpack_from('<II',b,off+i*8); t0.append(w0); t1.append(w1)
    if w1==1: cant+=1
    elif w1 & 0x80000000: inline_+=1
    else: extab+=1
def prel31(v,addr): return (addr + ((v&0x7fffffff)-(0x80000000 if v&0x80000000 else 0)))&0xffffffff
addrs=[prel31(t0[i],s[3]+i*8) for i in range(n)]
c=Counter(addrs)
print(n,cant,inline_,extab,sum(1 for k,v in c.items() if v>1),
      sum(1 for i in range(n) if t1[i]==1 and c[addrs[i]]>1))
PY

# A/B matrix (one variant; see §6 for the full list)
cat > /dev/shm/w4306/toolchain_extra.cmake <<'CMAKE'
include("/home/debian/GalaxyOnFire2/wt/w4306/cmake/toolchain.cmake")
set(GOF2_EXTRA_FLAGS "" CACHE STRING "extra matching flags")
if(GOF2_EXTRA_FLAGS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GOF2_EXTRA_FLAGS}" CACHE STRING "" FORCE)
  set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   ${GOF2_EXTRA_FLAGS}" CACHE STRING "" FORCE)
endif()
CMAKE
cmake -S $ROOT -B /dev/shm/w4306/build-fdata -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/dev/shm/w4306/toolchain_extra.cmake \
  -DGOF2_MATCH_OPT=-Oz -DGOF2_EXTRA_FLAGS='-fdata-sections' \
  -DGOF2_ORIGINAL_SO=$O -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build /dev/shm/w4306/build-fdata --target link_so -j8
python3 $ROOT/tools/verify/verify.py --build-dir /dev/shm/w4306/verify-fdata \
  --original $O --ours /dev/shm/w4306/build-fdata/libgof2hdaa.so \
  --objdump $OBJ --fail-on-wrong-type --top 0
```

## 12. Files changed and gate counts

Tracked change: this note only. No `src/`, `cmake/`, `CMakeLists.txt` or
`docs/` change; no `--update-baseline`; `report.json` not committed.

Gate before/after (canonical build): **byte_exact 4499 -> 4499,
linked_exact 4499 -> 4499, size_exact 4524/4524 -> 4524/4524, stub_zero_size 0,
missing 0, wrong_type 0, extra 0, data_exact 843/843, imports 207,
`.text 0x1337c8`, `.rodata 0x41b67`, lint CLEAN, drift 0.**
