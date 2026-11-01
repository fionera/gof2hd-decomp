# Original build-system evidence — GoF2 HD 2.0.16 Android

Research note, 2026-11-01. Worktree `wt/w3301`, branch `research-w3301`, base `81db17d1`.
This note is independent of `_work/reconstructions/build_flags_research.md`: that note
recovered flags by A/B codegen matching; this note looks for primary build-system evidence
in the shipped artifacts, the NDK r18b build system itself, and the studio's other ports.

**Bottom line:** the original native build was almost certainly **ndk-build from NDK r18b**
(`android-ndk-r18b`, clang 7.0.2, gold 1.12), targeting `armeabi-v7a`, API 16, with
`APP_STL := c++_static`, `LOCAL_ARM_NEON := true`, and
`LOCAL_CPP_FEATURES := rtti exceptions`. Nearly every flag the repo currently passes
explicitly is an ndk-build r18b *default*. The one material link flag the tree is missing is
`-Wl,-z,relro -Wl,-z,now`; adding it raises `byte_exact` from 1415 to 1458 (+43) and makes the
RW segment layout match the original almost exactly.

---

## 0. Confidence table

| claim | confidence | primary evidence |
|---|---:|---|
| Native build system = **ndk-build** (not CMake) | **very high (~97%)** | every compile/link flag in the .so is an r18b ndk-build default; `.note.android.ident` API 16 = r18b minimum/default; `-z now`, `--build-id`, `--fix-cortex-a8`, `--exclude-libs=libgcc.a`, `-fdata-sections`, `-DNDEBUG`, `-g`, `-Oz` all come from `build/core` |
| NDK = **r18b**, clang 7.0.2, gold 1.12 | **certain** | `.comment`, `.note.android.ident`, `.note.gnu.gold-version` |
| ABI = **armeabi-v7a**, softfp, Thumb-2, VFPv3, NEONv1 | **certain** | `readelf -h/-A` |
| `APP_PLATFORM = android-16` (or omitted; r18b default is 16) | **very high** | `.note.android.ident` API 16; `platforms.mk: NDK_MIN_PLATFORM_LEVEL := 16`; `setup-app-platform.mk` default; manifest `minSdkVersion=16` |
| `APP_STL = c++_static` | **high (~90%)** | no `libc++_shared.so` NEEDED; `-std=c++11` is c++_static's default; libc++abi symbols exported; no libstdc++/GLIBCXX symbols |
| `LOCAL_ARM_NEON := true` | **high (~90%)** | `Tag_FP_arch: VFPv3`, `Tag_Advanced_SIMD_arch: NEONv1`; at API 16 ndk-build does **not** auto-enable NEON (`>=23` only) |
| `LOCAL_CPP_FEATURES := rtti exceptions` | **high (~90%)** | ndk-build default is `-fno-exceptions -fno-rtti`; original has RTTI typeinfo and EH; `-fno-rtti` A/B worse; `-fno-exceptions` build fails |
| `-Oz`, `-g`, `-fstack-protector-strong`, `-funwind-tables`, `-fdata-sections`, `-ffunction-sections`, `-fpic`, `-mthumb`, `-march=armv7-a`, `-mfloat-abi=softfp`, `-DNDEBUG` | **very high** | verbatim r18b `build/core/default-build-commands.mk` and `.../arm-linux-androideabi-clang/setup.mk`; `.ARM.attributes`; A/B codegen |
| `-mfpu=neon` (after default `-mfpu=vfpv3-d16`) | **high** | `TARGET_CFLAGS.neon := -mfpu=neon`; original NEON attributes; source needs NEON |
| `-std=c++11` | **high** | c++_static's `DEFAULT_STD_VERSION := -std=c++11`; signed `new[]` guard evidence in existing note |
| `-Wl,-z,relro -Wl,-z,now` | **very high** | `TARGET_RELRO_LDFLAGS`; original `DT_FLAGS=BIND_NOW`, `DT_FLAGS_1=NOW`; adding it +43 byte_exact and matches `.init_array`/`.dynamic`/`.got`/`.data` addresses |
| `-Wl,--build-id`, `-Wl,--fix-cortex-a8`, `-Wl,--exclude-libs=libgcc.a`, `-Wl,--no-undefined`, `-Wl,-z,noexecstack`, `-Wl,--warn-shared-textrel`, `-Wl,--fatal-warnings` | **high** | verbatim r18b `build/core`; original `.note.gnu.build-id`, `GNU_STACK RW`, no text relocs; full ndk-build link flags reproduce the same bytes as `-z now --no-undefined` |
| `-Wl,-soname,libgof2hdaa.so`, `-shared`, `-lc -lm` | **certain** | `cmd-build-shared-library`; original `SONAME` and NEEDED order |
| Exact `LOCAL_SRC_FILES` list | **medium** | source-layout dumps + `.dynsym`; not a build script |
| FMOD in `LOCAL_LDLIBS` vs `LOCAL_SHARED_LIBRARIES` | **medium** | NEEDED order constrains the link line; both are consistent |
| zlib/libzip via `LOCAL_WHOLE_STATIC_LIBRARIES` | **medium-high** | original exports the full zlib/libzip symbol set; `--whole-archive` in the tree |
| Gradle `externalNativeBuild { ndkBuild { ... } }` vs standalone `ndk-build` | **medium** | APK `Created-By: Android Gradle 3.1.2`; the native build itself is ndk-build either way |
| Exact `-g` codegen effect | **medium** | stripped final .so cannot show it; existing A/B table says `-g` changes 9 byte_exact at this tree state |

---

## 1. What was searched, and what was not found

### 1.1 No original Android build script survives

Searches over `/home/debian/GalaxyOnFire2/` (including `source file list dump/`, `_work/`,
`Original APK/`, `Original IPA/`, the sibling repos, and every worktree) found **no**
`Android.mk`, `Application.mk`, `Makefile`, `CMakeLists.txt`, `build.gradle`, `*.pbxproj`,
`*.vcxproj`, `*.sln`, `*.sh`, `*.bat`, or CI config belonging to the original GoF2 Android
build. The only build files are:

- this repo's reconstruction (`CMakeLists.txt`, `cmake/toolchain.cmake`, `CMakePresets.json`);
- the NDK r18b cache (`/.cache/ndk/android-ndk-r18b/build/core/*.mk`);
- fan ports (`gof2-vita/CMakeLists.txt`, `KaamoClubModApi/xmake.lua`);
- DeepOpen's J2ME scripts (`DeepOpen/code/build/build_GoF2_jsr.sh`, `.bat`), which are Java,
  not Android native.

Representative commands:

```bash
rg -l --hidden -g '!**/.git/**' -g '!**/cmake-build-*' -g '!**/.cache/**' \
  -i 'ndk-build|LOCAL_CFLAGS|APP_CFLAGS|APP_STL|Application\.mk|Android\.mk|LOCAL_ARM_NEON' \
  /home/debian/GalaxyOnFire2
find /home/debian -maxdepth 4 -type f \
  \( -iname 'Android.mk' -o -iname 'Application.mk' -o -iname 'Makefile' \
     -o -iname 'CMakeLists.txt' -o -iname '*.pbxproj' -o -iname '*.vcxproj' \
     -o -iname '*.sln' -o -iname 'build.gradle' -o -iname '*.mk' \) \
  2>/dev/null | grep -viE '/(\.cache|cmake-build|node_modules|\.git|usr|share)/'
```

The only hits are reconstruction files, NDK internals, and unrelated projects. `git log --all
--name-only --diff-filter=A -- '*Android.mk' '*Application.mk' '*build.gradle'` shows no such
file ever existed in this repo.

### 1.2 The APK is the primary Android artifact

Path:

```
/home/debian/GalaxyOnFire2/Original APK/Galaxy on Fire 2™ HD_2.0.16_APKPure.apk
```

```
sha256 9cfe4484a475c9a31a336df9dea6408fbb129d88aa19083afe808ef6bca9eb19
143 entries, 13,206,304 bytes uncompressed
```

Only one ABI is present:

```
lib/armeabi-v7a/libgof2hdaa.so   2,187,328 B
lib/armeabi-v7a/libfmodex.so       920,204 B
lib/armeabi-v7a/libfmodevent.so    370,660 B
lib/armeabi-v7a/libcrashlytics.so  202,580 B
```

There is **no** second/unstripped copy of `libgof2hdaa.so`, no `libc++_shared.so`, no
`libstdc++`, and no other ABI. The `.so` is stripped (`.dynsym` only).

`assets/crashlytics-build.properties` (verbatim):

```
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

`META-INF/MANIFEST.MF` starts:

```
Manifest-Version: 1.0
Built-By: Generated-by-ADT
Created-By: Android Gradle 3.1.2
```

This proves the **APK packaging** used Android Gradle plugin 3.1.2 and was built on
**2018-10-22**. It does not by itself prove how the native code was compiled, but it fixes the
timeline: NDK r18b (released September 2018) was current, and the `.so` names r18b.

`AndroidManifest.xml` (parsed binary AXML; exact command in §9) gives:

```
versionCode = 47947032
versionName = 2.0.16
minSdkVersion = 16
targetSdkVersion = 27
```

The `minSdkVersion=16` is consistent with ndk-build's r18b default (see §3).

### 1.3 The APK `.so` and `_work/bins` are the same build

```
sha256  APK lib            6d38fd767ee3b4f8174c371bce371bfc6397cf76e13c1756938b2a94658e3b58
sha256  _work/bins lib     d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483
```

They differ in **288 bytes**, all in the section-header string table / `sh_name` fields
(offset 0x215c73..0x215d39). The `.text` section is byte-identical:

```
.text sha256 d49e32c1330e5d94e7628b9a40e05500af963a22816a93f2c738dbd366f813ed   (both)
build-id    c011810da45777b83ed721b2a87d517efed0d310                         (both)
```

So `_work/bins/android_2.0.16_libgof2hdaa.so` is a section-table-renamed copy of the APK
library, not a different build. All build metadata in this note applies to both.

### 1.4 The 7z "original" APK is a 2024 mod, not ground truth

`KiritoJPK_Galaxy-on-Fire-2-FULL-HD-Android/GOF2FHD_APK.7z` contains a 2024 repack named
`GOF2_2.0.16_Original_Price.apk`. Its `libgof2hdaa.so` is 2,469,656 B and its `.comment` is:

```
GCC: (GNU) 4.9.x 20150123 (prerelease)
Android clang version 3.8.275480  (based on LLVM 3.8.275480)
```

with gold 1.11. That is **not** the r18b/clang 7.0.2 build. Do not use it as ground truth.
The OBB 7z contains only game content (`47947_GOF2CONTENT_ETC/assets/...`), no build metadata.

### 1.5 Sibling repos

`gof2-vita`, `KaamoClubModApi`, `gof2edit`, `FMOD-GOF2-Recreated`, `DeepOpen` contain no
original Android build flags. `gof2-vita` uses `-O3 -g -ffast-math -mfloat-abi=softfp` for the
Vita loader, but that is a fan port, not evidence for the Android build. DeepOpen's scripts are
Java (`javac -source 1.3 -target 1.1 -g:none`), not native.

---

## 2. The original ELF: hard metadata

All commands below run against
`../_work/bins/android_2.0.16_libgof2hdaa.so` (or the identical APK copy).

### 2.1 Header, ABI, attributes

```
$ readelf -h ../_work/bins/android_2.0.16_libgof2hdaa.so
  Class: ELF32
  Data:  2's complement, little endian
  Type:  DYN (Shared object file)
  Machine: ARM
  Entry point address: 0x0
  Flags: 0x5000200, Version5 EABI, soft-float ABI
```

```
$ readelf -A ../_work/bins/android_2.0.16_libgof2hdaa.so
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

**Proves:** ARMv7-A, Thumb-2, softfp ABI, VFPv3, NEONv1, GOT-indirect (`-fpic`/`-fPIC`),
unaligned access v6, 4-byte wchar_t, int enums. These are exactly the r18b
`TARGET_CFLAGS`/`TARGET_CFLAGS.neon`/`TARGET_thumb_release_CFLAGS` attributes.

### 2.2 Build identity

```
$ readelf -p .comment ../_work/bins/android_2.0.16_libgof2hdaa.so
  [     1]  Android (4751641 based on r328903) clang version 7.0.2
            (... based on LLVM 7.0.2svn)
  [   108]  GCC: (GNU) 4.9.x 20150123 (prerelease)

$ readelf -n ../_work/bins/android_2.0.16_libgof2hdaa.so
  .note.android.ident:
    description data: 10 00 00 00 72 31 38 62 ... 35 30 36 33 30 34 35
    -> API 16, "r18b", build "5063045"
  .note.gnu.build-id:
    Build ID: c011810da45777b83ed721b2a87d517efed0d310
  .note.gnu.gold-version:
    Version: gold 1.12
```

**Proves:** NDK r18b (build 5063045), clang 7.0.2, gold 1.12. The GCC 4.9.x string is from
the NDK's GCC 4.9 crt/libgcc objects (and/or the vendored libcrypto), not from the app TUs.

**Important:** the `10 00 00 00` API field is the **link-time crtbegin** platform. The r18b
`platforms/android-16/arch-arm/usr/lib/crtbegin_so.o` contains exactly this note; the
`android-21` crtbegin contains `15 00 00 00`. The tree already links the android-16 crt
(`cmake/DownloadNDK.cmake: GOF2_NDK_CRT_LIBDIR .../platforms/android-16/...`), so its note is
also 16 even though it compiles with `-target ...21`. See §3.4 for why API 16 is the r18b
default anyway.

### 2.3 Dynamic section and link flags

```
$ readelf -dW ../_work/bins/android_2.0.16_libgof2hdaa.so
  PLTGOT      0x21068c
  PLTRELSZ    29392
  JMPREL      0x5751c
  REL         0x504d4
  RELSZ       28744
  RELCOUNT    1162
  SYMTAB      0x1f0
  STRTAB      0x15e90
  STRSZ       189208
  HASH        0x441a8
  NEEDED      liblog.so
  NEEDED      libGLESv2.so
  NEEDED      libGLESv1_CM.so
  NEEDED      libEGL.so
  NEEDED      libandroid.so
  NEEDED      libfmodex.so
  NEEDED      libfmodevent.so
  NEEDED      libm.so
  NEEDED      libdl.so
  NEEDED      libc.so
  SONAME      libgof2hdaa.so
  FINI_ARRAY  0x20cba0   FINI_ARRAYSZ 8
  INIT_ARRAY  0x20fb98   INIT_ARRAYSZ 12
  FLAGS       BIND_NOW
  FLAGS_1     NOW
  VERSYM      0x4d8e4
  VERDEF      0x50478   VERDEFNUM 1
  VERNEED     0x50494   VERNEEDNUM 2
```

**Proves:** `-Wl,-soname,libgof2hdaa.so`, `-shared`, `-Wl,--build-id`, `-Wl,-z,relro
-Wl,-z,now` (BIND_NOW + NOW + GNU_RELRO), `-lc -lm` at the end, and the exact NEEDED order.
The NEEDED order is the link-line order; it is consistent with a single `LOCAL_LDLIBS` list
ending `... -lm -ldl` followed by ndk-build's `TARGET_LDLIBS := -lc -lm` (first occurrence of
`-lm` wins). It is also consistent with `LOCAL_SHARED_LIBRARIES` holding the first seven libs
in that order and `LOCAL_LDLIBS := -lm -ldl`; the binary cannot distinguish those two.

### 2.4 Sections, segments, relocations

```
$ readelf -SW ../_work/bins/android_2.0.16_libgof2hdaa.so
  [ 1] .note.android.ident  0x134  0x98
  [ 2] .note.gnu.build-id   0x1cc  0x24
  [ 3] .dynsym              0x1f0  0x15ca0
  [ 4] .dynstr              0x15e90 0x2e318
  [ 5] .hash                0x441a8 0x973c
  [ 6] .gnu.version         0x4d8e4 0x2b94
  [ 7] .gnu.version_d       0x50478 0x1c
  [ 8] .gnu.version_r       0x50494 0x40
  [ 9] .rel.dyn             0x504d4 0x7048
  [10] .rel.plt             0x5751c 0x72d0
  [11] .plt                 0x5e7ec 0xac4c
  [12] .text                0x69440 0x1337c8
  [13] .ARM.exidx          0x19cc08 0x6f70
  [14] .ARM.extab          0x1a3b78 0x26298
  [15] .rodata             0x1c9e10 0x41b67
  [16] .fini_array         0x20cba0 0x8
  [17] .data.rel.ro        0x20cbb0 0x2fe8
  [18] .init_array         0x20fb98 0xc
  [19] .dynamic            0x20fba4 0x148
  [20] .got                0x20fcf4 0x430c
  [21] .data               0x214000 0x296c
  [22] .bss                0x216970 0x69ac
  [23] .comment            0x21596c 0x12f
  [24] .note.gnu.gold-version 0x215a9c 0x1c
  [25] .ARM.attributes     0x215ab8 0x38
  [26] .shstrtab           0x215af0 0x117
```

```
$ readelf -lW ../_work/bins/android_2.0.16_libgof2hdaa.so
  LOAD  0x000000 0x00000000 0x00000000 0x20b977 0x20b977 R E 0x1000
  LOAD  0x20bba0 0x0020cba0 0x0020cba0 0x09dcc 0x1077c RW  0x1000
  DYNAMIC 0x20eba4 0x0020fba4 0x0020fba4 0x00148
  GNU_STACK 0x000000 0x00000000 0x00000000 0x00000 0x00000 RW 0x10
  GNU_RELRO 0x20bba0 0x0020cba0 0x0020cba0 0x07460
```

**Proves:** only `.hash` (no `.gnu.hash`), `.got` before `.data`, 3 `.init_array` entries,
2 `.fini_array` entries, full RELRO, non-executable stack, no `DT_TEXTREL`, no `.symtab`
(stripped). The `.got`-before-`.data` order is a direct consequence of BIND_NOW in gold; see
§6.4.

### 2.5 Symbol counts

```
$ readelf --dyn-syms -W ... | awk '$7!="UND"{n++} END{print n}'   -> 5373 defined
$ readelf --dyn-syms -W ... | awk '$7=="UND"{n++} END{print n}'   ->  208 undefined
$ readelf -rW ... | grep -E 'rel.dyn|rel.plt'
  .rel.dyn 3593 entries
  .rel.plt 3674 entries
```

The tree's current build has the same 5373/208/3593/3674 counts (see §6).

---

## 3. NDK r18b build system: the flags are defaults

This is the strongest independent evidence. The r18b `build/core` files are still on the
machine at `.cache/ndk/android-ndk-r18b/` (a symlink to the shared NDK cache). The original's
flags are not random: they are the exact strings ndk-build emits.

### 3.1 Global C/C++ flags

`build/core/default-build-commands.mk`:

```make
GLOBAL_CFLAGS = \
    -target $(LLVM_TRIPLE)$(TARGET_PLATFORM_LEVEL) \
    -fdata-sections \
    -ffunction-sections \
    -fstack-protector-strong \
    -funwind-tables \
    -no-canonical-prefixes \

# Always enable debug info. We strip binaries when needed.
GLOBAL_CFLAGS += -g

GLOBAL_CXXFLAGS = $(GLOBAL_CFLAGS) -fno-exceptions -fno-rtti
```

`build/core/toolchains/arm-linux-androideabi-clang/setup.mk`:

```make
TARGET_CFLAGS := \
    -gcc-toolchain $(call host-path,$(TOOLCHAIN_ROOT)) \
    -fpic \
    -march=armv7-a \
    -mfloat-abi=softfp \
    -mfpu=vfpv3-d16 \

TARGET_CFLAGS.neon := -mfpu=neon

TARGET_thumb_release_CFLAGS := \
    -mthumb \
    -Oz \
    -DNDEBUG \
```

`TARGET_thumb_release_CFLAGS` is selected for a release build of a Thumb source file
(`APP_OPTIM` default is release). This is the source of `-mthumb -Oz -DNDEBUG`. The
`-mfpu=vfpv3-d16` is the default; `LOCAL_ARM_NEON := true` appends `-mfpu=neon`, which wins
because it comes later on the command line. The original's `Tag_FP_arch: VFPv3` (not
`VFPv3-D16`) and `NEONv1` prove the NEON override was active.

### 3.2 Link flags

`build/core/default-build-commands.mk`:

```make
TARGET_NO_UNDEFINED_LDFLAGS := -Wl,--no-undefined

TARGET_RELRO_LDFLAGS := -Wl,-z,relro -Wl,-z,now

TARGET_LIBGCC = -lgcc -Wl,--exclude-libs,libgcc.a
TARGET_LIBATOMIC = -latomic -Wl,--exclude-libs,libatomic.a
TARGET_LDLIBS := -lc -lm

define cmd-build-shared-library
$(PRIVATE_CXX) \
    -Wl,-soname,$(notdir $(LOCAL_BUILT_MODULE)) \
    -shared \
    --sysroot=$(call host-path,$(PRIVATE_SYSROOT_LINK)) \
    $(PRIVATE_LINKER_OBJECTS_AND_LIBRARIES) \
    $(GLOBAL_LDFLAGS) \
    $(PRIVATE_LDFLAGS) \
    $(PRIVATE_LDLIBS) \
    -o $(call host-path,$(LOCAL_BUILT_MODULE))
endef
```

`build/core/build-binary.mk`:

```make
LOCAL_LDFLAGS += -Wl,--build-id

# If LOCAL_ALLOW_UNDEFINED_SYMBOLS is not true, the linker will allow the generation
# of a binary that uses undefined symbols.
ifneq ($(LOCAL_ALLOW_UNDEFINED_SYMBOLS),true)
  LOCAL_LDFLAGS += $(TARGET_NO_UNDEFINED_LDFLAGS)
endif

# Toolchain by default disallows generated code running from the heap and stack.
LOCAL_LDFLAGS += -Wl,-z,noexecstack

# Toolchain by default provides relro and GOT protections.
LOCAL_LDFLAGS += $(TARGET_RELRO_LDFLAGS)

LOCAL_LDFLAGS += -Wl,--warn-shared-textrel

ifneq ($(LOCAL_DISABLE_FATAL_LINKER_WARNINGS),true)
  LOCAL_LDFLAGS += -Wl,--fatal-warnings
endif
```

`build/core/toolchains/arm-linux-androideabi-clang/setup.mk`:

```make
TARGET_LDFLAGS += \
    -gcc-toolchain $(call host-path,$(TOOLCHAIN_ROOT)) \
    -Wl,--fix-cortex-a8 \
```

So the original link line contains, by default:
`-Wl,-soname,libgof2hdaa.so -shared --sysroot=... <objects/libs> -target
armv7-none-linux-androideabi16 -no-canonical-prefixes -gcc-toolchain ...
-Wl,--fix-cortex-a8 -Wl,--build-id -Wl,--no-undefined -Wl,-z,noexecstack
-Wl,-z,relro -Wl,-z,now -Wl,--warn-shared-textrel -Wl,--fatal-warnings
-Wl,--exclude-libs,libgcc.a ... -lc -lm -o libgof2hdaa.so`.

The original's `DT_FLAGS=BIND_NOW` / `DT_FLAGS_1=NOW` / `GNU_RELRO` are exactly
`TARGET_RELRO_LDFLAGS`. The tree currently omits it; see §6.4.

### 3.3 Platform default is API 16

`build/core/platforms.mk`:

```make
NDK_MAX_PLATFORM_LEVEL := 28
NDK_MIN_PLATFORM_LEVEL := 16
```

`build/core/setup-app-platform.mk`:

```make
ifndef APP_PLATFORM
    $(call __ndk_info,APP_PLATFORM not set. Defaulting to minimum supported \
        version $(NDK_MIN_PLATFORM).)
    APP_PLATFORM := $(NDK_MIN_PLATFORM)
endif
```

So if the original `Application.mk` did not set `APP_PLATFORM`, r18b would default to
**android-16**. The manifest's `minSdkVersion=16` is consistent. The original's
`.note.android.ident` API 16 therefore does **not** prove an explicit `APP_PLATFORM :=
android-16`; it proves only that the build used the r18b default (or explicitly set 16).
Either way, the compile target was 16, not 21.

### 3.4 STL: c++_static implies -std=c++11

`build/core/add-application.mk`:

```make
APP_STL := $(strip $(APP_STL))
ifndef APP_STL
    APP_STL := system
else
    ifneq ($(filter $(APP_STL),gnustl_static gnustl_shared stlport_static stlport_shared),)
        $(call __ndk_error,APP_STL $(APP_STL) is no longer supported. ...)
    endif
    $(call ndk-stl-check,$(APP_STL))
endif
```

`build/core/definitions.mk`:

```make
$(call ndk-stl-register,\
    c++_static,\
    cxx-stl/llvm-libc++,\
    c++_static,\
    ,\
    -std=c++11\
    )
```

`STL_DEFAULT_STD_VERSION` is inserted into every C++ compile command
(`ev-compile-cpp-source`), so **`-std=c++11` is automatic with `APP_STL := c++_static`**.
This matches the signed `new[]` guard evidence in the existing note. `APP_STL` defaults to
`system` (libstdc++), but the original has no `libstdc++`/`GLIBCXX` symbols and exports
libc++abi's `__cxa_*`/`_ZSt*`/`_ZNSt*` symbols; `c++_static` is the only r18b STL that fits.

### 3.5 RTTI and exceptions are not defaults

`GLOBAL_CXXFLAGS` ends with `-fno-exceptions -fno-rtti`. `build/core/build-binary.mk` and
`definitions.mk`:

```make
# If the module has any kind of C++ features, enable them in LOCAL_CPPFLAGS
ifneq (,$(call module-has-c++-features,$(LOCAL_MODULE),rtti))
    LOCAL_CPPFLAGS += -frtti
endif
ifneq (,$(call module-has-c++-features,$(LOCAL_MODULE),exceptions))
    LOCAL_CPPFLAGS += -fexceptions
endif
```

`module-handle-c++-features` derives the features from `LOCAL_CPP_FEATURES` or from
`-frtti`/`-fexceptions` already in the module flags, then removes the feature flags and
re-adds only the selected ones to `LOCAL_CPPFLAGS` (which comes after `GLOBAL_CXXFLAGS`).
The original has RTTI typeinfo and exception handling, so its `Android.mk` must have set
`LOCAL_CPP_FEATURES := rtti exceptions` (or equivalent `-frtti -fexceptions` in
`LOCAL_CPPFLAGS`/`LOCAL_CXXFLAGS`). This is a **required** non-default build-file line.

### 3.6 NEON is not automatic at API 16

`build/core/build-binary.mk`:

```make
LOCAL_ARM_NEON := $(strip $(LOCAL_ARM_NEON))
ifdef LOCAL_ARM_NEON
  ...
else ifeq ($(true),$(call gte,$(TARGET_PLATFORM_LEVEL),23))
  LOCAL_ARM_NEON := true
endif

ifeq ($(LOCAL_ARM_NEON),true)
  neon_sources += $(LOCAL_SRC_FILES:%.neon=%)
  ...
endif
```

At API 16, `TARGET_PLATFORM_LEVEL >= 23` is false, so NEON is **not** enabled unless the
module sets `LOCAL_ARM_NEON := true` or tags individual sources with `.neon`. The original's
global `Tag_FP_arch: VFPv3` + `NEONv1` and the fact that the tree's source needs NEON
(`-mfpu=vfpv3-d16` alone fails to build) make `LOCAL_ARM_NEON := true` the simplest and most
likely explanation. Per-file `.neon` tags cannot be ruled out from the final `.so`, but would
be unusual for a whole engine.

### 3.7 `--no-undefined` and `-nostdlib++`

`build/core/build-binary.mk` also adds:

```make
ifeq ($(NDK_TOOLCHAIN_VERSION),clang)
    ifeq ($(filter system stlport_shared stlport_static,$(NDK_APP_STL)),)
        LOCAL_LDFLAGS += -nostdlib++
    endif
endif
```

`-nostdlib++` prevents the clang driver from adding `-lstdc++`; it is part of the default
c++_static link. The original link almost certainly had it. The tree links with gold directly,
so it is irrelevant there.

---

## 4. Reconstructed original build files

These are **hypotheses** constrained by the binary and the r18b defaults, not recovered text.
The exact `LOCAL_SRC_FILES` list is not knowable from the stripped `.so`; the source-layout
dumps (`_work/original_layout/`) are the best witness for TU membership.

### 4.1 `Application.mk` (most likely)

```make
APP_ABI := armeabi-v7a
APP_STL := c++_static
APP_PLATFORM := android-16   # optional: r18b's minimum is already 16
APP_OPTIM := release         # optional: this is the default
```

Confidence: `APP_ABI` high (only armeabi-v7a ships), `APP_STL` high, `APP_PLATFORM` medium
(explicit vs default cannot be distinguished), `APP_OPTIM` medium (release is the default).
`APP_CPPFLAGS`/`APP_CFLAGS` are unknown; no evidence requires any.

### 4.2 `Android.mk` (most likely shape)

```make
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := gof2hdaa
LOCAL_SRC_FILES := <the original TU list>
LOCAL_CPP_FEATURES := rtti exceptions
LOCAL_ARM_NEON := true
LOCAL_LDLIBS := -llog -lGLESv2 -lGLESv1_CM -lEGL -landroid \
                -L<path-to-fmod> -lfmodex -lfmodevent -lm -ldl
LOCAL_WHOLE_STATIC_LIBRARIES := zlib libzip
LOCAL_STATIC_LIBRARIES := crypto
include $(BUILD_SHARED_LIBRARY)
```

Confidence: `LOCAL_MODULE` certain (SONAME); `LOCAL_CPP_FEATURES` high; `LOCAL_ARM_NEON`
high; `LOCAL_LDLIBS` order medium-high (NEEDED order); `LOCAL_WHOLE_STATIC_LIBRARIES`
medium-high (full zlib/libzip exports); `LOCAL_STATIC_LIBRARIES := crypto` medium (the
libcrypto subset is statically linked and its `.comment` is GCC 4.9). FMOD could instead be
in `LOCAL_SHARED_LIBRARIES`; the NEEDED order alone cannot distinguish.

### 4.3 Reconstructed compile command (one C++ TU)

This is the exact r18b ndk-build command shape, with `LOCAL_ARM_NEON := true` and
`LOCAL_CPP_FEATURES := rtti exceptions`:

```sh
$NDK/toolchains/llvm/prebuilt/linux-x86_64/bin/clang++ \
  -target armv7-none-linux-androideabi16 \
  -fdata-sections -ffunction-sections -fstack-protector-strong -funwind-tables \
  -no-canonical-prefixes -g \
  -Wno-invalid-command-line-argument -Wno-unused-command-line-argument \
  -fno-exceptions -fno-rtti \
  -gcc-toolchain $NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64 \
  -fpic -march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16 \
  -mthumb -Oz -DNDEBUG -mfpu=neon \
  <LOCAL_C_INCLUDES> -std=c++11 <APP_CFLAGS/APP_CPPFLAGS> <LOCAL_CFLAGS/CPPFLAGS> \
  -frtti -fexceptions \
  --sysroot=$NDK/sysroot \
  -isystem $NDK/sysroot/usr/include \
  -isystem $NDK/sysroot/usr/include/arm-linux-androideabi \
  -c <source.cpp> -o <source.o>
```

Order notes: `-mfpu=neon` must come after `-mfpu=vfpv3-d16`; `-frtti -fexceptions` must come
after `-fno-rtti -fno-exceptions`. Both are satisfied by ndk-build's flag concatenation.

### 4.4 Reconstructed link command

```sh
$NDK/toolchains/llvm/prebuilt/linux-x86_64/bin/clang++ \
  -Wl,-soname,libgof2hdaa.so -shared \
  --sysroot=$NDK/sysroot \
  <objects> <static libs> <whole static libs> \
  -lgcc -Wl,--exclude-libs,libgcc.a \
  <shared libs> \
  -target armv7-none-linux-androideabi16 -no-canonical-prefixes \
  -gcc-toolchain $NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64 \
  -Wl,--fix-cortex-a8 \
  -Wl,--build-id \
  -Wl,--no-undefined \
  -Wl,-z,noexecstack \
  -Wl,-z,relro -Wl,-z,now \
  -Wl,--warn-shared-textrel \
  -Wl,--fatal-warnings \
  -Wl,--exclude-libs,libgcc.a \
  -nostdlib++ \
  <LOCAL_LDLIBS> -lc -lm \
  -o libgof2hdaa.so
```

The exact order is from `cmd-build-shared-library`; `-nostdlib++` and the
`--exclude-libs` entries are from `LOCAL_LDFLAGS`/`TARGET_LIBGCC`. The original's final
stripped `.so` cannot show `-nostdlib++` or `--fatal-warnings`, but it does show the results
of `-z now`, `--build-id`, `--no-undefined`, `--exclude-libs=libgcc.a`, and gold.

---

## 5. The iOS / Windows artifacts: no Android flag evidence

### 5.1 iOS

Paths:

```
/home/debian/GalaxyOnFire2/_work/bins/ios_1.0.5_armv7
/home/debian/GalaxyOnFire2/_work/bins/ios_1.1.4_HD
/home/debian/GalaxyOnFire2/_work/bins/ios_1.0.5_GalaxyOnFire2
/home/debian/GalaxyOnFire2/Original IPA/Galaxy_on_Fire_2__HD_1.1.4_ios_4.3.ipa
/home/debian/GalaxyOnFire2/Original IPA/Galaxy_on_Fire_2__1.0.5_ios_3.0.ipa
```

`Info.plist` (1.1.4 HD):

```
DTXcode = 0450          (Xcode 4.5)
DTXcodeBuild = 4G182
DTSDKName = iphoneos6.0
DTPlatformVersion = 6.0
DTCompiler = com.apple.compilers.llvm.clang.1_0
BuildMachineOSBuild = 11E53
MinimumOSVersion = 4.3
```

`Info.plist` (1.0.5):

```
DTXcode = 0400          (Xcode 4.0)
DTXcodeBuild = 4A304a
DTSDKName = iphoneos4.3
DTCompiler = 4.2        (GCC 4.2)
BuildMachineOSBuild = 10J869
MinimumOSVersion = 3.0
```

`llvm-dwarfdump --debug-info` is empty for all three Mach-O binaries: the IPAs ship no DWARF
(dSYMs were separate). The binaries link `/usr/lib/libstdc++.6.dylib`, not libc++. Load
commands: 1.0.5 has no PIE, `__symbol_stub4`, `__mod_init_func` 0x12c (75 entries); 1.1.4 HD
has PIE, `__picsymbolstub4` + `__stub_helper`, `__mod_init_func` 0x18 (6 entries);
1.1.4 HD has `LC_VERSION_MIN_IPHONEOS 4.3 / SDK 6.0`.

Source paths in the dumps:

```
/Users/mhehmeyer/Documents/Dev/GalaxyOnFire2/...
/Users/hck/Documents/Dev/GalaxyOnFire2_merged/...
.../Release-iphoneos/.../Objects-normal/arm64/...
.../Distribute-iphoneos/.../Objects-normal/armv6/...
```

**Proves:** the studio built release iOS binaries with Xcode 4.x, separate dSYMs, and
libstdc++ in the 1.x era. **Does not prove** anything about Android flags. It is consistent
with the studio habit of release + debug info + strip, which matches ndk-build's `-g` +
packaging strip, but that is weak corroboration.

### 5.2 Windows

Path: `/home/debian/GalaxyOnFire2/_work/bins/win_GOF2.exe`

```
PE32 i386, linker version 10.0 (Visual Studio 2010)
TimeDateStamp 2012-09-05 16:30:26
Imports MSVCR100.dll, MSVCP100.dll
Debug directory: RSDS, age 2,
  C:\Users\s.lohse\Desktop\GoF2_PC_REP\GOF2.pdb
```

**Proves:** the PC build was VS2010 with a separate PDB, 2012. **Does not prove** Android
flags.

### 5.3 Source-layout dumps

`/home/debian/GalaxyOnFire2/source file list dump/` contains
`source file lists dump.txt`, `gof2hd_1.1.19_ios.cpp`, `gof2sd_1.0.1_ios.cpp`,
`gof3d_1.1.4_ios.cpp`. They are symbol/source-path dumps, not build scripts. They witness
TU membership and original file names (see `_work/original_layout/`), but contain no compiler
or linker flags.

---

## 6. Controlled experiments and measured numbers

All experiments were run in this worktree. The committed tree is unchanged; scratch builds
live under `/tmp`. Baseline command:

```bash
cmake --preset default
cmake --build cmake-build-match --target check -j32
```

Baseline output (master `81db17d1`, this worktree):

```
compared 4523  avg 99.89%  byte_exact 1415  linked_exact 4493  fuzzy_perfect 4494
stub_zero_size 0  orig_zero_size 1  missing 0 (wrong_type 0  absent 0)  extra 0
verify: byte 1415  linked 4493  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`.text` = `0x1343d0`; `size_match` = `4522/4523` (computed from `report.json` rows:
`sum(osize == usize)`); `lint` = `CLEAN ✓`; `drift` = `0 field(s) across 0 class(es)`.

### 6.1 API 16 vs 21

Temporary toolchain `/tmp/w3301_api16_toolchain.cmake` includes the repo toolchain and replaces
`androideabi21`/`__ANDROID_API__=21` with 16. Build:

```bash
cmake -S . -B /tmp/w3301-api16 -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/tmp/w3301_api16_toolchain.cmake -DGOF2_MATCH_OPT=-Oz
cmake --build /tmp/w3301-api16 --target link_so -j32
python3 tools/verify/verify.py --build-dir /tmp/w3301-api16/verify \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3301-api16/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
```

Result: **1415 / 4493 / 4494**, `.text 0x1343d0` — identical to the API 21 baseline.
API 16 compiles cleanly (the old note's "16 fails to compile" is stale for this tree).
**Conclusion:** API 16 is codegen-neutral here; the tree should use 16 to match the original's
compile target, but it is not a byte-count lever.

### 6.2 `-fpic` vs `-fPIC`

Temporary toolchain replaces `-fPIC` with `-fpic` (and API 21→16). Result: **1415 / 4493 /
4494**, `.text 0x1343d0`; the two `.so` files differ only in the build-id (20 bytes).
**Conclusion:** ndk-build's default `-fpic` and the tree's `-fPIC` are codegen-identical for
this GOT size. The tree's `-fPIC` is not evidence of a different build system.

### 6.3 `-DNDEBUG`

Temporary toolchain adds `-DNDEBUG` (API 16). With `-z now` (see §6.4): **1458 / 4493 /
4494**. Without `-z now`: 1415. **Conclusion:** `-DNDEBUG` is codegen-neutral here; it is
still authentic (r18b release default) and should be added for faithfulness.

### 6.4 `-Wl,-z,relro -Wl,-z,now` — the important one

The tree's current link command has no `-z now`; the original has `DT_FLAGS=BIND_NOW` and
`DT_FLAGS_1=NOW`. Relinking the current objects with `-z relro -z now`:

```bash
ninja -C cmake-build-match -t commands libgof2hdaa.so | grep 'ld.gold -shared' | tail -1 \
  > /tmp/w3301_main_linkcmd.txt
sed 's#-o /home/debian/GalaxyOnFire2/wt/w3301/cmake-build-match/libgof2hdaa.so#-o /tmp/w3301_main_now.so#;
     s#-shared #-shared -z relro -z now #' /tmp/w3301_main_linkcmd.txt > /tmp/w3301_main_relink_now.sh
bash /tmp/w3301_main_relink_now.sh
.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-strip \
  --strip-all /tmp/w3301_main_now.so
python3 tools/verify/verify.py --build-dir /tmp/w3301-main-now-verify \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so --ours /tmp/w3301_main_now.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
```

Result: **byte_exact 1458 (+43), linked_exact 4493, fuzzy 4494, size 4522/4523**.
`sodiff` still passes (5370/5370, 0 extra/missing); `datadiff` still 843/843. The `.text`
size is unchanged at `0x1343d0`.

Why the +43: BIND_NOW changes gold's PLT/GOT layout. The `.plt` size stays `0xac4c`, but its
contents and the `.rel.plt` order change; functions' `bl` offsets to PLT entries become
byte-identical to the original for 43 more functions. The RW section layout also snaps into
place:

| section | original | current (no `-z now`) | `-z now` |
|---|---:|---:|---:|
| `.fini_array` addr | 0x20cba0 | 0x20cea0 | 0x20cb80 |
| `.data.rel.ro` addr | 0x20cbb0 | 0x20ceb0 | 0x20cb90 |
| `.init_array` addr | **0x20fb98** | 0x20feb8 | **0x20fb98** |
| `.dynamic` addr | **0x20fba4** | 0x20fec4 | **0x20fba4** |
| `.got` addr | **0x20fcf4** | 0x212974 | **0x20fcf4** |
| `.data` addr | **0x214000** | 0x210000 | **0x214000** |
| `.bss` addr | 0x216970 | 0x216c80 | 0x216980 |
| `.dynamic` size | 0x148 | 0x138 | **0x148** |
| `DT_FLAGS` | BIND_NOW | absent | **BIND_NOW** |
| `DT_FLAGS_1` | NOW | absent | **NOW** |

The `.init_array`/`.fini_array` **contents** are byte-identical in both builds
(`41 94 06 00 c1 94 06 00 dd 94 06 00`; `48 95 06 00 34 95 06 00`); only their addresses
move. This is strong evidence that `-z now` is an original link flag, not a coincidence.

### 6.5 `-fdata-sections`

r18b's `GLOBAL_CFLAGS` always has `-fdata-sections`; the tree omits it. Temporary build with
API 16 + `-fdata-sections`, relinked with `-z now`:

```
byte_exact 1454, linked_exact 4493, fuzzy 4494, size 4522/4523
```

i.e. **-4** versus the `-z now` build without `-fdata-sections` (1458). `.rodata` shrinks by
0x100 (0x41747 → 0x41647) and `.data` grows by 4 (0x296c → 0x2970). The original `.rodata`
is 0x41b67, so the tree's data/TU layout is still too far off for `-fdata-sections` to pay
off today. **Conclusion:** the original definitely had `-fdata-sections`; add it when the
data/TU layout is faithful, not before. It is not evidence against ndk-build.

### 6.6 `-gcc-toolchain`, `-fno-exceptions -fno-rtti -frtti -fexceptions`

A single-TU compile of `src/game/world/Level.cpp` with and without
`-gcc-toolchain .../arm-linux-androideabi-4.9/prebuilt/linux-x86_64` produced **byte-identical
objects** (`sha256 85bac762...`, `cmp` 0 differences). The full ndk-build replica below also
shows the `-fno-*`/`-f*` ordering is neutral.

### 6.7 Full ndk-build replica

Temporary toolchain `/tmp/w3301_ndkbuild_replica_toolchain.cmake` sets the exact r18b flags:
API 16, `-fpic`, `-fdata-sections`, `-DNDEBUG`, `-gcc-toolchain`, `-no-canonical-prefixes`,
`-Wno-invalid-command-line-argument -Wno-unused-command-line-argument`,
`-fno-exceptions -fno-rtti`, `-std=c++11`, `-frtti -fexceptions`,
`-mfpu=vfpv3-d16 -mfpu=neon`, `-mthumb -Oz -g`, `-fstack-protector-strong`,
`-funwind-tables`, `-ffunction-sections`. Build + relink with the full ndk-build link flag
set (`-z relro -z now -z noexecstack --no-undefined --warn-shared-textrel --fatal-warnings`,
no `--unresolved-symbols=ignore-all`):

```
byte_exact 1454, linked_exact 4493, fuzzy 4494, size 4522/4523
sodiff: 5370/5370, extra 0, missing 0
datadiff: 843/843
```

Section layout vs original:

| section | original | ndk-build replica |
|---|---:|---:|
| `.text` | 0x1337c8 | 0x1343d0 |
| `.rodata` | 0x41b67 | 0x41647 |
| `.fini_array` | 0x20cba0 | 0x20cb80 |
| `.data.rel.ro` | 0x20cbb0 | 0x20cb90 |
| `.init_array` | **0x20fb98** | **0x20fb98** |
| `.dynamic` | **0x20fba4** | **0x20fba4** |
| `.got` | **0x20fcf4** | **0x20fcf4** |
| `.data` | **0x214000** | **0x214000** |
| `.bss` | 0x216970 | 0x216980 |
| `.fini_array` contents | `48 95 06 00 34 95 06 00` | identical |
| `.init_array` contents | `41 94 06 00 c1 94 06 00 dd 94 06 00` | identical |

`readelf -d` is identical after stripping address values; `readelf -A` is identical;
`readelf -n` differs only in the build-id. The section-name list is identical except for the
tree's extra `.aezdata` (from `#pragma clang section data=".aezdata"` in
`src/engine/file/AEFile.cpp`), which the original does not have.

**Conclusion:** the reconstructed ndk-build command set reproduces the original's dynamic
section, notes, ARM attributes, section order, init/fini arrays, GOT size/address, PLT size,
reloc counts, and RW segment layout. The remaining deltas are source/TU/data, not build flags.

### 6.8 `--unresolved-symbols=ignore-all` is unnecessary

The tree's link uses `--unresolved-symbols=ignore-all`. Relinking the same objects with
ndk-build's `--no-undefined` and **without** `--unresolved-symbols=ignore-all` succeeds and
produces a **byte-identical** `.so` (same sha256 as the `--no-undefined`+ignore-all link):

```
sha256 23a033938c40931711b97fc87393fbad6b4508c388ea2c1351060162a5e80050  (both)
byte_exact 1458, linked_exact 4493
```

The full ndk-build link flags (`-z relro -z now -z noexecstack --no-undefined
--warn-shared-textrel --fatal-warnings`) also produce the same bytes. **Conclusion:** the
original almost certainly used `--no-undefined` (ndk-build default), and the tree can drop
`--unresolved-symbols=ignore-all` without changing codegen.

---

## 7. Git history: earlier guesses, no primary script

Relevant commits (all repo hypotheses, not original build files):

| commit | change | note |
|---|---|---|
| `709e3aef` | `_work/reconstructions/build_flags_research.md` | the existing A/B flag note |
| `e4e4fbcb` | compile with `-g` (ndk-build default) | byte 1185→1194, linked 2779→2857 |
| `ad147cd4` | `-fstack-protector` → `-fstack-protector-strong` | linked 2466→2500 |
| `c932c88a` | `-std=c++11` | signed `new[]` guard evidence |
| `dbf3b4c2` | link like ndk-build: APK FMOD libs, `libgcc.a` last, `--exclude-libs=libgcc.a` | linked 3791→3794 |
| `e2b9bcb7` | ndk-build input order + `libandroid_support.a` | |
| `0601e522` | add `-funwind-tables` | `.ARM.extab` 0→99.98% |

`git log --all --name-only --diff-filter=A -- '*Android.mk' '*Application.mk'
'*build.gradle'` shows no original build file ever existed in this repo. The existing note's
A/B table remains valid for compile flags; this note adds the link-flag and build-system
evidence it lacked.

---

## 8. Strongest hypothesis: the original build

### 8.1 `Application.mk`

```make
APP_ABI := armeabi-v7a
APP_STL := c++_static
APP_PLATFORM := android-16   # optional; r18b minimum/default is 16
APP_OPTIM := release         # optional; default
```

### 8.2 `Android.mk`

```make
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := gof2hdaa
LOCAL_SRC_FILES := <original TU list>
LOCAL_CPP_FEATURES := rtti exceptions
LOCAL_ARM_NEON := true
LOCAL_LDLIBS := -llog -lGLESv2 -lGLESv1_CM -lEGL -landroid \
                -L<fmod-dir> -lfmodex -lfmodevent -lm -ldl
LOCAL_WHOLE_STATIC_LIBRARIES := zlib libzip
LOCAL_STATIC_LIBRARIES := crypto
include $(BUILD_SHARED_LIBRARY)
```

### 8.3 Compile command

```sh
$NDK/toolchains/llvm/prebuilt/linux-x86_64/bin/clang++ \
  -target armv7-none-linux-androideabi16 \
  -fdata-sections -ffunction-sections -fstack-protector-strong -funwind-tables \
  -no-canonical-prefixes -g \
  -Wno-invalid-command-line-argument -Wno-unused-command-line-argument \
  -fno-exceptions -fno-rtti \
  -gcc-toolchain $NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64 \
  -fpic -march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16 \
  -mthumb -Oz -DNDEBUG -mfpu=neon \
  <includes> -std=c++11 <APP_CFLAGS/APP_CPPFLAGS> <LOCAL_CFLAGS/CPPFLAGS> \
  -frtti -fexceptions \
  --sysroot=$NDK/sysroot \
  -isystem $NDK/sysroot/usr/include \
  -isystem $NDK/sysroot/usr/include/arm-linux-androideabi \
  -c <source.cpp> -o <source.o>
```

### 8.4 Link command

```sh
$NDK/toolchains/llvm/prebuilt/linux-x86_64/bin/clang++ \
  -Wl,-soname,libgof2hdaa.so -shared \
  --sysroot=$NDK/sysroot \
  <objects> <static libs> <whole static libs> \
  -lgcc -Wl,--exclude-libs,libgcc.a \
  <shared libs> \
  -target armv7-none-linux-androideabi16 -no-canonical-prefixes \
  -gcc-toolchain $NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64 \
  -Wl,--fix-cortex-a8 \
  -Wl,--build-id \
  -Wl,--no-undefined \
  -Wl,-z,noexecstack \
  -Wl,-z,relro -Wl,-z,now \
  -Wl,--warn-shared-textrel \
  -Wl,--fatal-warnings \
  -Wl,--exclude-libs,libgcc.a \
  -nostdlib++ \
  <LOCAL_LDLIBS> -lc -lm \
  -o libgof2hdaa.so
```

### 8.5 What the tree should change (for the coordinator, not done here)

1. **Add `-Wl,-z,relro -Wl,-z,now` to the link.** Measured +43 byte_exact (1415→1458),
   no linked/size/sodiff/data change, and it fixes the RW section layout. This is the single
   highest-value build-flag change.
2. **Switch `_match_api` 21→16.** Codegen-neutral (1415/4493 both), matches the original's
   compile target and r18b default. The old "16 fails to compile" note is stale.
3. **Replace `--unresolved-symbols=ignore-all` with `--no-undefined`.** Link succeeds and
   produces byte-identical output; matches ndk-build's default.
4. **Add `-DNDEBUG`** (neutral today, authentic).
5. **Consider `-fdata-sections`** only after data/TU layout is faithful; it currently costs
   -4 byte_exact (1458→1454) despite being authentic.
6. **`-fPIC` → `-fpic`** is optional (codegen-identical); `-gcc-toolchain` is optional
   (object-identical).
7. **Do not add `-fno-rtti`/`-fno-exceptions`**; the original had RTTI and exceptions.
8. **Do not remove `-frtti`; add `-fexceptions` explicitly** if you want the ndk-build
   command shape (clang's default already enables exceptions).

---

## 9. Reproduction commands

```bash
# --- APK / primary artifact -------------------------------------------------
APK="/home/debian/GalaxyOnFire2/Original APK/Galaxy on Fire 2™ HD_2.0.16_APKPure.apk"
sha256sum "$APK"
unzip -l "$APK"
unzip -p "$APK" assets/crashlytics-build.properties
unzip -p "$APK" META-INF/MANIFEST.MF | head -5
unzip -o -j "$APK" 'lib/armeabi-v7a/*' -d /tmp/gof2_apk_libs
sha256sum /tmp/gof2_apk_libs/libgof2hdaa.so ../_work/bins/android_2.0.16_libgof2hdaa.so
cmp -l /tmp/gof2_apk_libs/libgof2hdaa.so ../_work/bins/android_2.0.16_libgof2hdaa.so | wc -l
dd if=/tmp/gof2_apk_libs/libgof2hdaa.so bs=1 skip=$((0x69440)) count=$((0x1337c8)) status=none | sha256sum
dd if=../_work/bins/android_2.0.16_libgof2hdaa.so bs=1 skip=$((0x69440)) count=$((0x1337c8)) status=none | sha256sum

# --- AndroidManifest (binary AXML) -----------------------------------------
# (minimal parser; prints minSdkVersion/targetSdkVersion/versionCode/versionName)
unzip -p "$APK" AndroidManifest.xml > /tmp/gof2_manifest.xml
python3 - <<'PY'
import struct, zipfile
b=open('/tmp/gof2_manifest.xml','rb').read()
def strings(b,off):
    typ,hs,size,count,stylecount,flags,strstart,stylestart=struct.unpack_from('<HHIIIIII',b,off)
    utf8=bool(flags&0x100); offs=struct.unpack_from('<%dI'%count,b,off+hs); base=off+strstart; out=[]
    for o in offs:
        p=base+o
        if utf8:
            n=b[p]; p+=1
            if n&0x80: p+=1
            m=b[p]; p+=1
            if m&0x80: m=((m&0x7f)<<8)|b[p]; p+=1
            out.append(b[p:p+m].decode('utf-8','replace'))
        else:
            n=struct.unpack_from('<H',b,p)[0]; p+=2
            if n&0x8000: n=((n&0x7fff)<<16)|struct.unpack_from('<H',b,p)[0]; p+=2
            out.append(b[p:p+2*n].decode('utf-16le','replace'))
    return out
pos=8; S=None; R=None
while pos+8<=len(b):
    typ,hs,size=struct.unpack_from('<HHI',b,pos)
    if typ==1: S=strings(b,pos)
    elif typ==0x0180: R=struct.unpack_from('<%dI'%((size-hs)//4),b,pos+hs)
    elif typ==0x0102:
        ns,name,astart,asize,acount,_,_,_=struct.unpack_from('<IIHHHHHH',b,pos+16)
        tag=S[name] if S and name<len(S) else name
        if tag in ('manifest','uses-sdk'):
            print('TAG',tag)
            for i in range(acount):
                ao=pos+16+astart+i*asize
                ans,aname,araw,tsize,res0,dtype,adata=struct.unpack_from('<IIIHBBI',b,ao)
                print('  ',S[aname] if S and aname<len(S) else aname,dtype,adata)
    pos+=size
PY

# --- ELF metadata -----------------------------------------------------------
readelf -h ../_work/bins/android_2.0.16_libgof2hdaa.so
readelf -A ../_work/bins/android_2.0.16_libgof2hdaa.so
readelf -dW ../_work/bins/android_2.0.16_libgof2hdaa.so
readelf -SW ../_work/bins/android_2.0.16_libgof2hdaa.so
readelf -lW ../_work/bins/android_2.0.16_libgof2hdaa.so
readelf -rW ../_work/bins/android_2.0.16_libgof2hdaa.so
readelf -n ../_work/bins/android_2.0.16_libgof2hdaa.so
readelf -p .comment ../_work/bins/android_2.0.16_libgof2hdaa.so
readelf --dyn-syms -W ../_work/bins/android_2.0.16_libgof2hdaa.so | awk '$7!="UND"{n++} END{print "defined",n}'
readelf --dyn-syms -W ../_work/bins/android_2.0.16_libgof2hdaa.so | awk '$7=="UND"{n++} END{print "undef",n}'

# --- NDK r18b defaults ------------------------------------------------------
NDK=.cache/ndk/android-ndk-r18b
sed -n '130,160p' $NDK/build/core/default-build-commands.mk
sed -n '40,72p'  $NDK/build/core/toolchains/arm-linux-androideabi-clang/setup.mk
sed -n '45,60p'  $NDK/build/core/default-build-commands.mk
sed -n '1,12p'   $NDK/build/core/default-build-commands.mk
sed -n '110,120p' $NDK/build/core/default-build-commands.mk
sed -n '1,12p'   $NDK/build/core/platforms.mk
sed -n '17,28p'  $NDK/build/core/setup-app-platform.mk
sed -n '170,190p' $NDK/build/core/add-application.mk
sed -n '2140,2160p' $NDK/build/core/definitions.mk
sed -n '750,810p' $NDK/build/core/definitions.mk
sed -n '300,335p' $NDK/build/core/build-binary.mk
sed -n '175,215p' $NDK/build/core/build-binary.mk
sed -n '74,86p'  $NDK/build/core/default-build-commands.mk

# --- gate baseline ----------------------------------------------------------
cmake --preset default
cmake --build cmake-build-match --target check -j32
cmake --build cmake-build-match --target lint
cmake --build cmake-build-match --target drift

# --- API 16 / -fpic / -DNDEBUG / -fdata-sections / ndk-build replica ------
# (temporary toolchain files shown in §6; build dirs under /tmp)
cmake -S . -B /tmp/w3301-api16 -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/tmp/w3301_api16_toolchain.cmake -DGOF2_MATCH_OPT=-Oz
cmake --build /tmp/w3301-api16 --target link_so -j32
# ... same for /tmp/w3301-api16-fpic, /tmp/w3301-api16-ndebug, /tmp/w3301-api16-fdata,
#     /tmp/w3301-ndkbuild-replica

# --- -z now relink ----------------------------------------------------------
ninja -C cmake-build-match -t commands libgof2hdaa.so | grep 'ld.gold -shared' | tail -1 \
  > /tmp/w3301_main_linkcmd.txt
sed 's#-o /home/debian/GalaxyOnFire2/wt/w3301/cmake-build-match/libgof2hdaa.so#-o /tmp/w3301_main_now.so#;
     s#-shared #-shared -z relro -z now #' /tmp/w3301_main_linkcmd.txt > /tmp/w3301_main_relink_now.sh
bash /tmp/w3301_main_relink_now.sh
.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-strip \
  --strip-all /tmp/w3301_main_now.so
python3 tools/verify/verify.py --build-dir /tmp/w3301-main-now-verify \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so --ours /tmp/w3301_main_now.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 tools/sodiff.py --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3301_main_now.so --allow tools/sodiff_allow.txt
python3 tools/datadiff.py ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3301_main_now.so

# --- iOS / Windows ----------------------------------------------------------
unzip -p "/home/debian/GalaxyOnFire2/Original IPA/Galaxy_on_Fire_2__HD_1.1.4_ios_4.3.ipa" \
  'Payload/*.app/Info.plist' > /tmp/gof2_info.plist
python3 -c "import plistlib; d=plistlib.load(open('/tmp/gof2_info.plist','rb')); print({k:d.get(k) for k in ['DTXcode','DTXcodeBuild','DTSDKName','DTPlatformVersion','DTCompiler','BuildMachineOSBuild','MinimumOSVersion']})"
llvm-dwarfdump --debug-info ../_work/bins/ios_1.1.4_HD
llvm-objdump --macho --private-headers ../_work/bins/ios_1.1.4_HD | grep -A4 LC_VERSION_MIN_IPHONEOS
llvm-readobj --file-headers --coff-debug-directory ../_work/bins/win_GOF2.exe
objdump -p ../_work/bins/win_GOF2.exe | grep -iE 'Time/Date|MajorLinkerVersion|pdb'
```

---

## 10. Honest nulls / open questions

- **No original `Android.mk`/`Application.mk` was found.** Everything in §4 is a
  reconstruction from the binary + r18b defaults, not a recovered file.
- **The exact `LOCAL_SRC_FILES` list is not recoverable from the stripped `.so`.** Use
  `_work/original_layout/tu_mapping.md` for TU ownership.
- **`APP_PLATFORM` explicit vs default cannot be distinguished.** Both give API 16.
- **`APP_OPTIM` explicit vs default cannot be distinguished.** Both give release `-Oz`.
- **FMOD in `LOCAL_LDLIBS` vs `LOCAL_SHARED_LIBRARIES` cannot be distinguished.** The NEEDED
  order constrains the link-line order, not the make variable.
- **`-fdata-sections` is authentic but currently costs 4 byte_exact** because the tree's
  data/TU layout is not yet faithful. This is a null/negative result, not a reason to doubt
  ndk-build.
- **`-g` cannot be observed in the stripped final `.so`.** Its codegen effect is supported by
  the existing A/B table, not by ELF metadata.
- **The 7z "Original_Price" APK is a 2024 clang-3.8 mod**, not the r18b release. Do not use
  it as a second witness.
- **The iOS/Windows artifacts carry no Android flags.** They only corroborate the studio's
  release-build + separate-debug-symbol habit.
