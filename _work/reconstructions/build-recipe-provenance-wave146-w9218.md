# Build archaeology: exact original recipe, APK forensics, strip/build-id provenance (wave 146, w9218)

Status: **note-only research commit**, 2026-11-08. Worktree
`/home/debian/GalaxyOnFire2/wt/w9218`, base `31297fadc` ("Wave 145 docs:
LevelScript 54,895, Radar 8,373, MGame 4,180, .text 85,651"). Scratch:
`/dev/shm/w9218`. No `src/`, `tools/`, `CMakeLists.txt`, baseline, allowlist
or doc change is landed. This note is the only commit.

Gate at the base (fresh `cmake --build cmake-build-match --target check -j6`):

```text
verify: byte 4507/4524  linked 4507/4524  size 4524/4524  avg 99.93
data:   exact 843/843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0  imports 207
symbol parity 5370/5370 (0 extra / 0 missing)
lint CLEAN ✓   drift 0
RATCHET PASS
```

Section deltas of the canonical build vs the original (2,187,328 B, all 27
section headers present, all section sizes exact):

| section | size | differing bytes |
|---|---:|---:|
| `.text` | 1,259,464 | **85,651** |
| `.ARM.extab` | 156,312 | **1,370** |
| `.note.gnu.build-id` | 36 | **20** |
| `.note.android.ident`, `.dynsym`, `.dynstr`, `.hash`, `.gnu.version*`, `.rel.dyn`, `.rel.plt`, `.plt`, `.ARM.exidx`, `.rodata`, `.fini_array`, `.data.rel.ro`, `.init_array`, `.dynamic`, `.got`, `.data`, `.bss`, `.comment`, `.note.gnu.gold-version`, `.ARM.attributes`, `.shstrtab` | exact | **0** |

Our canonical build: 2,187,328 B,
sha256 `232ff2c75e50efc80e774c8553432b00e8f7590943f5ab262fbc87d02ea8f87f`
(the hash changes with every source pass; the *section* numbers above are the
stable metric).

---

## 0. Bottom line (answers to the five questions)

1. **ndk-build vs CMake: CMake.** The only observable compile-line
   difference between the two r18b default recipes is `-fdata-sections`
   (ndk-build has it, the r18b CMake toolchain does not). The original's
   `.rodata`/`.data`/`.bss` are byte-identical to the no-`-fdata-sections`
   build and move when it is added (wave-141 A/B: `byte_exact` 4507 → 4315,
   `.rodata` 269,159 → 268,807, `.data` 10,604 → 10,588). New in this pass:
   `-fdata-sections` also changes a data-bearing TU's object
   (`Globals.cpp`: 520,772 → 528,140 B), while `-fpic` vs `-fPIC`,
   `-mfpu=vfpv3-d16 -mfpu=neon` vs `-mfpu=neon`, and `-gcc-toolchain` are
   object-identical. The natural reading is **NDK r18b CMake toolchain,
   `ANDROID_ABI=armeabi-v7a`, `ANDROID_PLATFORM=android-16`,
   `ANDROID_STL=c++_static`, `ANDROID_ARM_NEON=ON`,
   `CMAKE_BUILD_TYPE=Release`**. The alternatives (ndk-build with an explicit
   `-fno-data-sections`, or a hand-written script copying the CMake defaults)
   are not excluded by any artifact.
2. **Exact link line.** A single plain gold 1.12 link, no `-s`, no
   `--version-script`, no section ordering file:
   `ld.gold -shared -soname libgof2hdaa.so --build-id -z now
   --exclude-libs=libgcc.a`, then `crtbegin_so.o`, the 192 app objects in
   `tools/source_order.txt`, `--whole-archive libz libzip --no-whole-archive`,
   `-l log -l GLESv2 -l GLESv1_CM -l EGL -l android`, `libfmodex.so
   libfmodevent.so`, `libcrypto_gof2.a`, `-l m`, `libc++abi.a
   libandroid_support.a libgcc.a`, `-l dl -l c`, `crtend_so.o`. This is the
   r18b CMake default link line minus the byte-neutral flags
   (`-z relro`, `-z noexecstack`, `--hash-style=sysv`, `--fix-cortex-a8`,
   `--warn-shared-textrel`, `--fatal-warnings`, `--exclude-libs,libatomic.a`,
   `--exclude-libs,libunwind.a`, `--no-undefined`).
3. **Strip invocation: GNU bfd strip from NDK r18b, not llvm-strip.**
   `arm-linux-androideabi-strip --strip-all` and `--strip-unneeded` produce
   **byte-identical** output on this file (sha256 `58312057…`); `llvm-strip
   --strip-all` produces a different 2,186,780 B file with only 24 sections
   (it drops `.comment`, `.note.gnu.gold-version`, `.ARM.attributes`). The
   shipped APK copy's 27 section headers and its `.shstrtab` are
   **byte-identical** to our r18b GNU-strip output. ndk-build's default
   `APP_STRIP_MODE` is `--strip-unneeded`; AGP 3.1.2's packaging strip also
   uses the NDK strip. The `--strip-all` spelling in `docs/VALIDATION.md` is
   therefore **not witnessed**; both flags are equivalent here.
4. **Build-id derivation: SHA-1 over the unstripped gold output with the
   20-byte descriptor zeroed.** Our unstripped link's stored id
   `f8b44219…` equals `sha1(file with descriptor zeroed)` exactly; the strip
   preserves it. The shipped original's id `c011810d…` is **not** the hash of
   the shipped (stripped) file (`d7c05dc5…`), which proves the file was
   stripped **after** the link, not linked with `-s`. The 20-byte note rules
   out `--build-id=md5`/`uuid` (16 bytes); gold 1.12's `--build-id=tree`
   produces the same value as `sha1` on this file, so the two 20-byte styles
   are indistinguishable. The original build directory / `DW_AT_comp_dir` is
   still unrecoverable (one-way hash).
5. **Are the `docs/VALIDATION.md` flags all witnessed?** No — see §8.
   `-DANDROID`, `-DNDEBUG`, `-fPIC` vs `-fpic`, `-frtti` (clang default),
   `-fexceptions` (clang default), `-g2` vs `-g3`, the DWARF version and the
   exact `-target` spelling are unobservable; everything else is witnessed
   positively by the ELF or a controlled A/B.

**New evidence in this pass (not in the wave-141/132/101 notes):**

* The APKs under `/dev/shm/gof2flags` and `/tmp/gof2re2/stock` are the
  **old r14 / clang 3.8 / gnustl_shared** build, not the r18b ground truth.
  There are three same-build-id r14 variants (stock / allunlock / price);
  the mods are small post-link patches, and the build-id is stale (inherited).
* The r18b APK copy and the ground truth differ by exactly **288 bytes =
  259 in `.shstrtab` + 29 in `sh_name` fields, nothing else**.
* Our r18b GNU-strip output reproduces the APK copy's **entire section-header
  table and `.shstrtab` byte-for-byte**; `reorder_shstrtab.py` then reproduces
  the ground truth's `.shstrtab` byte-for-byte.
* `llvm-strip` is ruled out by section count/content; host `/usr/bin/strip`
  cannot read ARM.
* Positive build-id formula confirmation on our own unstripped link.
* ndk-build's `APP_STRIP_MODE` default is `--strip-unneeded`; CMake's
  `ANDROID_STL_FORCE_FEATURES` turns on `rtti exceptions` automatically, and
  its default `-Wl,--no-undefined` must be disabled for the original's 207
  unresolved imports.

---

## 1. Artifacts examined

| artifact | size | sha256 | what it is |
|---|---:|---|---|
| `_work/bins/android_2.0.16_libgof2hdaa.so` | 2,187,328 | `d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483` | r18b ground truth (APK copy + `reorder_shstrtab.py`) |
| `Original APK/Galaxy on Fire 2™ HD_2.0.16_APKPure.apk` | 6,389,308 | `9cfe4484a475c9a31a336df9dea6408fbb129d88aa19083afe808ef6bca9eb19` | r18b shipping APK |
| APK `lib/armeabi-v7a/libgof2hdaa.so` | 2,187,328 | `6d38fd767ee3b4f8174c371bce371bfc6397cf76e13c1756938b2a94658e3b58` | raw bfd-strip artifact |
| `/dev/shm/gof2flags/GOF2_2.0.16_Original_Price.apk` | 6,539,074 | `6bc380fa2f1da56f8f1837ad739b96728118e36e51720ba8ddd2933d51f389c2` | r14 build, 2024 repack |
| `/tmp/gof2re2/stock/net.fishlabs.gof2hdallandroid2012.apk` | 6,407,298 | `bd99ed7c3fcb7858c5b22214c7f8680466f34ca3debc9e108b35957d602463b5` | r14 build, 2026 repack |
| `/tmp/gof2re2/stock/libgof2hdaa_stock.so` | 2,469,656 | `125dfdb218470b76d5c41ee2eb00b55ce379963df8869ba5c6bece0fd040a50e` | r14 stock |
| `/tmp/gof2re2/stock/libgof2hdaa_allunlock.so` | 2,469,656 | `4a1e8376d15079e5e9b0728faa5c84e20b31758eaa2608fb5c7b966bb99c87c4` | r14 + 80-byte patch |
| `/dev/shm/gof2flags/lib/armeabi-v7a/libgof2hdaa.so` | 2,469,656 | `9a166d19b9fde9552e2b061183909fa2f8d86467b69713403a33a4612f402fad` | r14 + 358-byte patch |

The r18b APK has 143 ZIP entries and exactly four `lib/armeabi-v7a/*.so`
files (`libgof2hdaa`, `libcrashlytics`, `libfmodex`, `libfmodevent`), all
deflated (`compress_type=8`), all timestamps `1980-00-00 00:00` (zipalign /
repack). Its Java-side metadata is the only build-system string:

```text
META-INF/MANIFEST.MF:  Built-By: Generated-by-ADT
                       Created-By: Android Gradle 3.1.2
assets/crashlytics-build.properties:
  #Mon Oct 22 10:18:17 CEST 2018
  version_name=2.0.16  version_code=47947032
  package_name=net.fishlabs.gof2hdallandroid2012
  build_id=f527605b-309f-404d-8afc-b68027edc49e
AndroidManifest.xml (AXML): versionName 2.0.16, minSdkVersion 16,
  targetSdkVersion 27, glEsVersion 0x00020000
```

No `Android.mk`, `Application.mk`, `CMakeLists.txt`, `*.cmake`, `*.gradle`,
`*.ninja`, `*.o`, `*.a`, `*.d` or build log survives in the APK.

### 1.1 The `/dev/shm/gof2flags` and `/tmp/gof2re2/stock` APKs are the r14 build

This is the first thing to correct in the brief: those two APKs contain a
**2,469,656-byte** `libgof2hdaa.so`, not the 2,187,328-byte r18b ground
truth. They are the old official 2.0.16 build:

| observable | r14 APK build | r18b ground truth |
|---|---|---|
| `.comment` | `GCC: (GNU) 4.9.x 20150123` (offset 1) + `Android clang version 3.8.275480` (offset 28) | clang 7.0.2 (offset 1) + GCC 4.9.x (offset 108) |
| linker | gold 1.11 | gold 1.12 |
| `.note.android.ident` | absent | present, r18b / 5063045 / API 16 |
| sections | 26 | 27 |
| `.ARM.attributes` | v7, Thumb-2, VFPv3, NEONv1, **Aggressive Speed**, **TrustZone** | v7, Thumb-2, VFPv3, NEONv1, no optimization/TrustZone tags |
| `DT_NEEDED` | `… libm libstdc++ libdl libc` | `… libm libdl libc` (static libc++) |
| `.text` | 1,476,296 (`0x1682c8`) | 1,259,464 (`0x1337c8`) |
| build-id | `1dbb7d451b7ca0ff0cfccf2f2d2fae0e8f9c5a1c` | `c011810da45777b83ed721b2a87d517efed0d310` |
| `.shstrtab` | section-header order (bfd strip) | gold Stringpool order (repo reorder) |

The r14 build is a **gnustl_shared + gold 1.11** build (the `libstdc++.so`
NEEDED and the `Aggressive Speed`/`TrustZone` attributes come from the
GCC-built runtime). It is not a codegen oracle for r18b, but it independently
confirms the source/TU order (wave-132: 4,220/4,244 adjacent app-function
pairs preserved).

### 1.2 Three same-build-id r14 variants

All three r14 files are 2,469,656 B, have 26 sections, and carry the **same
build-id** `1dbb7d45…` but different sha256:

| file | differing bytes vs stock | where |
|---|---:|---|
| `libgof2hdaa_allunlock.so` | 80 | 2 B in `.hash` + 78 B in `.text` (a code patch) |
| `/dev/shm/gof2flags/…/libgof2hdaa.so` | 358 | ELF program-header `p_filesz/p_memsz` (0x251ac8 → 0x251924) + `.text`/`.data`/`.hash` patches; section table unchanged |

The build-id is therefore **not a hash of the shipped file** in the r14
build either: the mods were applied after the link and the id was inherited.
This is a useful independent confirmation of the r18b conclusion in §6.

---

## 2. r18b ELF / toolchain fingerprint

### 2.1 Toolchain identity strings

The local NDK r18b package is `Pkg.Revision = 18.1.5063045`
(`source.properties`), `AndroidVersion.txt` = `7.0.2 / based on r328903`,
`CHANGELOG.md` line 48 = "Updated Clang to build 4751641, based on r328903".
The binaries print:

```text
clang:      Android (4751641 based on r328903) clang version 7.0.2
            (…clang 003100370607242ddd5815e4a043907ea9004281)
            (…llvm 1d739ffb0366421d383e04ff80ec2ee591315116)
            (based on LLVM 7.0.2svn)
ld.gold:    GNU gold (GNU Binutils 2.27.0.20170315) 1.12
strip:      GNU strip (GNU Binutils) 2.27.0.20170315
```

The original's `.comment` (303 B) is exactly:

```text
byte 0        NUL
bytes 1..262  Android (4751641 based on r328903) clang version 7.0.2 …
byte 263      NUL
bytes 264..301 GCC: (GNU) 4.9.x 20150123 (prerelease)
byte 302      NUL
```

The clang string is **byte-identical to `crtbegin_so.o`'s `.comment`**
(`platforms/android-16/arch-arm/usr/lib/crtbegin_so.o`), and the GCC string is
**byte-identical to `sha256.o`'s `.comment`** in the vendored
`libcrypto_gof2.a`. The linker's `SHF_MERGE|SHF_STRINGS` handling drops the
duplicate leading NUL, giving 303 B. This is a direct proof of the link order
`crtbegin → … → libcrypto`.

### 2.2 Notes and attributes

* `.note.android.ident` (152 B) is **byte-identical to the note in
  `crtbegin_so.o`**: `api=0x10` (16), `ndk="r18b"`, `build="5063045"`.
  It is a *platform/toolchain* witness, not an app build-system witness.
* `.note.gnu.gold-version` (28 B) = `GNU` / `gold 1.12`; only gold emits it.
* `.ARM.attributes` (56 B) decodes to: `ARM v7`, `Application`, ARM ISA,
  Thumb-2, VFPv3, NEONv1, GOT-indirect, wchar_t 4, IEEE754, 8-byte align,
  enum int, unaligned v6. No `Tag_ABI_optimization_goals`, no
  `Tag_Virtualization_use` (clang 7 does not emit them; the GCC-built r14
  runtime does).
* All four metadata sections (`.comment`, `.note.android.ident`,
  `.note.gnu.gold-version`, `.ARM.attributes`) are **byte-identical** between
  the original and our unstripped link output.

### 2.3 Dynamic metadata

```text
DT_SONAME      libgof2hdaa.so
DT_NEEDED      liblog libGLESv2 libGLESv1_CM libEGL libandroid
               libfmodex libfmodevent libm libdl libc
DT_FLAGS       BIND_NOW (0x8)
DT_FLAGS_1     NOW (0x1)
DT_RELCOUNT    1162
PT_GNU_RELRO   0x20cba0, size 0x7460, align 0x10
PT_GNU_STACK   RW, align 0x10
.gnu.version_d 1 BASE entry (libgof2hdaa.so)
.gnu.version_r LIBC v2 from libc.so, LIBC v3 from libm.so
```

The `DT_NEEDED` order is exactly a hand-written `-l` list order
(`-llog -lGLESv2 -lGLESv1_CM -lEGL -landroid -lfmodex -lfmodevent -lm -ldl
-lc`); it is reproduced by the link line in §4. `-z now` is witnessed by
`DT_FLAGS`/`DT_FLAGS_1`; `-z relro`, `-z noexecstack`, `--hash-style=sysv`
and `-z combreloc` are gold defaults (the raw link in §4 passes none of them
explicitly and still matches).

---

## 3. Build system: ndk-build vs CMake

### 3.1 The two r18b default compile lines

From the local NDK r18b package:

**ndk-build** (`build/core/default-build-commands.mk` +
`build/core/toolchains/arm-linux-androideabi-clang/setup.mk`):

```text
GLOBAL_CFLAGS = -target armv7-none-linux-androideabi16
                -fdata-sections -ffunction-sections
                -fstack-protector-strong -funwind-tables
                -no-canonical-prefixes -g
TARGET_CFLAGS = -gcc-toolchain … -fpic -march=armv7-a
                -mfloat-abi=softfp -mfpu=vfpv3-d16
TARGET_CFLAGS.neon = -mfpu=neon
TARGET_thumb_release_CFLAGS = -mthumb -Oz -DNDEBUG
GLOBAL_CXXFLAGS = $(GLOBAL_CFLAGS) -fno-exceptions -fno-rtti
APP_STRIP_MODE (default) = --strip-unneeded
```

**CMake** (`build/cmake/android.toolchain.cmake`):

```text
-g -DANDROID -ffunction-sections -funwind-tables
-fstack-protector-strong -no-canonical-prefixes
-Oz -DNDEBUG
-march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16
-mfpu=neon            (ANDROID_ARM_NEON=ON)
-fPIC                 (CMAKE_POSITION_INDEPENDENT_CODE)
-frtti -fexceptions   (ANDROID_STL_FORCE_FEATURES default ON)
-std=c++11            (ANDROID_STL=c++_static)
-Wl,--build-id -Wl,--warn-shared-textrel -Wl,--fatal-warnings
-Wl,--exclude-libs,libgcc.a -Wl,--exclude-libs,libatomic.a
-Wl,--exclude-libs,libunwind.a -Wl,--fix-cortex-a8
-Wl,--no-undefined    (unless ANDROID_ALLOW_UNDEFINED_SYMBOLS=ON)
```

### 3.2 The only observable discriminator: `-fdata-sections`

`-fdata-sections` is in ndk-build's `GLOBAL_CFLAGS` and **not** in the r18b
CMake toolchain. It is observable because gold's default `--sort-section=none`
preserves per-object data-section order, so separate data sections change
alignment/padding:

| build | `.rodata` | `.data` | `.bss` addr | `byte_exact` |
|---|---:|---:|---:|---:|
| original / canonical (no `-fdata-sections`) | 269,159 | 10,604 | `0x216970` | **4507** |
| `-fdata-sections` (wave-141 A/B) | 268,807 | 10,588 | `0x216960` | 4315 |

New object-level check this pass: `-fdata-sections` changes a data-bearing
TU (`src/game/core/Globals.cpp`: 520,772 → 528,140 B, different sha256),
while the following are **object-identical** on `AERandom.cpp` and/or the
full build:

| variant | object sha256 | verdict |
|---|---|---|
| canonical | `7f91911a…` | — |
| `-mfpu=vfpv3-d16 -mfpu=neon` (ndk-build spelling) | `7f91911a…` | identical |
| `-fpic` instead of `-fPIC` | `7f91911a…` | identical |
| `-gcc-toolchain <r18b>` | `7f91911a…` | identical |

So the `-mfpu` double-spelling, `-fpic`/`-fPIC` and `-gcc-toolchain` cannot
distinguish the two build systems. `-fdata-sections` is the discriminator,
and it says **CMake**.

### 3.3 Other r18b build-system details (new)

* **ndk-build's default strip mode is `--strip-unneeded`**
  (`build/core/add-application.mk`: `APP_STRIP_MODE := --strip-unneeded`),
  not `--strip-all`. Both are byte-identical here (§5), so the shipped file
  cannot distinguish them.
* **CMake's `ANDROID_STL_FORCE_FEATURES` defaults ON**, so
  `ANDROID_STL=c++_static` automatically gets `-frtti -fexceptions`; ndk-build
  needs `APP_CPP_FEATURES := rtti exceptions` (its `GLOBAL_CXXFLAGS` otherwise
  has `-fno-exceptions -fno-rtti`).
* **CMake defaults to `-Wl,--no-undefined`**, but the original has 207
  unresolved imports (FMOD, GLES, …). A CMake build must therefore set
  `ANDROID_ALLOW_UNDEFINED_SYMBOLS=ON` (or `ANDROID_SO_UNDEFINED=ON`).
  ndk-build has the same knob (`LOCAL_ALLOW_UNDEFINED_SYMBOLS := true`).
  This is a recipe requirement, not an observable byte difference.
* Both systems default `APP_PLATFORM`/`ANDROID_PLATFORM` to the r18b minimum,
  `android-16`, which matches `.note.android.ident`.

**Conclusion.** CMake r18b Release is the best-supported recipe. The
remaining alternatives are ndk-build with an explicit `-fno-data-sections`
(or a `LOCAL_CFLAGS` override), or a hand-written build script that copies
the CMake defaults. No `Android.mk`, `Application.mk`, `CMakeLists.txt` or
build log survives in the APK to settle it.

---

## 4. Exact link line

The canonical link (from `ninja -C cmake-build-match -t commands
libgof2hdaa.so`, 232 tokens) is:

```sh
$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/arm-linux-androideabi/bin/ld.gold \
  -shared -soname libgof2hdaa.so --build-id -z now --exclude-libs=libgcc.a \
  -o libgof2hdaa.unstripped.so \
  $NDK/platforms/android-16/arch-arm/usr/lib/crtbegin_so.o \
  -L $NDK/platforms/android-16/arch-arm/usr/lib \
  <192 app objects in tools/source_order.txt> \
  --whole-archive third_party/zlib/libz_gof2.a third_party/libzip/libzip_gof2.a \
  --no-whole-archive \
  -l log -l GLESv2 -l GLESv1_CM -l EGL -l android \
  _work/bins/apk_libs/libfmodex.so _work/bins/apk_libs/libfmodevent.so \
  third_party/openssl/libcrypto_gof2.a \
  -l m \
  $NDK/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/libc++abi.a \
  $NDK/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/libandroid_support.a \
  $NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/lib/gcc/arm-linux-androideabi/4.9.x/armv7-a/thumb/libgcc.a \
  -l dl -l c \
  $NDK/platforms/android-16/arch-arm/usr/lib/crtend_so.o
```

`tools/source_order.txt` is a plain 203-line source list (11 comment lines +
192 source entries; one object per source). The app objects come first, then the whole-archive zlib/libzip,
then the system `-l` list, FMOD, libcrypto, libm, libc++abi,
libandroid_support, libgcc, dl, c, crtend. This is exactly the r18b CMake
`target_link_libraries` order; the repo uses raw gold to control the input
order (gold emits symbols in input order, so this is what places every
function at its original address).

The link line deliberately omits the CMake default flags that are
byte-neutral on this tree (`-z relro`, `-z noexecstack`, `--hash-style=sysv`,
`--fix-cortex-a8`, `--warn-shared-textrel`, `--fatal-warnings`,
`--exclude-libs,libatomic.a`, `--exclude-libs,libunwind.a`,
`--no-undefined`). The original's `.dynamic`, program headers, `.plt`,
`.rel.plt`, `.hash`, `.dynsym`, `.dynstr`, `.got`, `.ARM.exidx` and
`.shstrtab` are all byte-identical, so none of those flags changes an
observable byte.

### 4.1 Compile line (one C++ TU)

```sh
$NDK/toolchains/llvm/prebuilt/linux-x86_64/bin/clang++ \
  -target armv7-none-linux-androideabi16 \
  -march=armv7-a -mthumb -mfpu=neon -mfloat-abi=softfp \
  -std=c++11 -fPIC -frtti -fstack-protector-strong -Oz -g \
  -stdlib=libc++ \
  -isystem $NDK/sources/cxx-stl/llvm-libc++/include \
  -isystem $NDK/sources/cxx-stl/llvm-libc++abi/include \
  -isystem $NDK/sources/android/support/include \
  --sysroot=$NDK/sysroot \
  -isystem $NDK/sysroot/usr/include \
  -isystem $NDK/sysroot/usr/include/arm-linux-androideabi \
  -D__ANDROID_API__=16 -DANDROID -DNDEBUG \
  -Wno-int-to-pointer-cast -Wno-int-to-void-pointer-cast \
  -I<repo>/src -I<repo>/third_party/fmod/inc -I<repo>/third_party/gl \
  -I<repo>/third_party/jni -I<repo>/third_party/libzip -I<repo>/third_party/crypto \
  -c <TU>.cpp -o <TU>.o
```

The C TUs use the same line minus `-std=c++11 -frtti -stdlib=libc++` and the
libc++ include paths. The vendored zlib/libzip are compiled with
`--target=armv7a-linux-androideabi16 -mfpu=neon -mfloat-abi=softfp -fPIC
-ffunction-sections -funwind-tables -Oz -g -fstack-protector-strong -mthumb`
(no `-fdata-sections`), and libcrypto with real GCC 4.9 from NDK r13b
(`third_party/build_match_lib.cmake`). The GCC string in `.comment` is the
`sha256.o` member's.

---

## 5. Post-link strip: exact tool and byte-level proof

### 5.1 GNU bfd strip, not llvm-strip

Starting from our unstripped gold output (`ours.unstripped.so`, 19,345,392 B,
sha256 `5520fca8…`):

| strip | output size | sha256 | sections |
|---|---:|---|---:|
| `arm-linux-androideabi-strip --strip-all` | 2,187,328 | `58312057e53e85d012adcc943dea4bb043781bb20f3b6e69f6ca4c6bde3ec771` | 27 |
| `arm-linux-androideabi-strip --strip-unneeded` | 2,187,328 | `58312057…` (identical) | 27 |
| `llvm-strip --strip-all` | 2,186,780 | `c40e3afcdaea6ae860d9a0840261b1d74084d5c922b65b5b9d8d2d2b1f772e88` | 24 |
| host `/usr/bin/strip --strip-all` | — | — | cannot read ARM ELF |

`llvm-strip` drops `.comment`, `.note.gnu.gold-version` and
`.ARM.attributes`, so the shipped 27-section file is **GNU bfd strip** output.
`--strip-all` and `--strip-unneeded` are indistinguishable on this file.

### 5.2 The APK copy's section headers and `.shstrtab` are reproduced exactly

Comparing the shipped r18b APK copy
(`6d38fd76…`, raw bfd-strip artifact) with our `--strip-all` output:

* **Every section-header field** (`sh_type`, `sh_flags`, `sh_addr`,
  `sh_offset`, `sh_size`, `sh_link`, `sh_info`, `sh_addralign`,
  `sh_entsize`) is **identical**, for all 27 sections.
* The **`.shstrtab` content is byte-identical** (279 B, section-header
  order: `\0.shstrtab\0.note.android.ident\0…`).
* `reorder_shstrtab.py` then rewrites `.shstrtab`/`sh_name` to gold's
  Stringpool order, and the result's `.shstrtab` is **byte-identical to the
  ground truth's** (279 B, order `\0.init_array\0.fini_array\0…`).

The ground truth and the APK copy differ by **exactly 288 bytes**:

```text
259 bytes in .shstrtab (the table is the same 279 B size; only the order differs)
 29 bytes across the 26 sh_name fields
  0 bytes anywhere else
```

So the ground truth is the shipped APK copy after `reorder_shstrtab.py`, and
the reorder is a **repo post-processing step**, not part of the original
build. The original build ends with the GNU strip.

### 5.3 Where did the strip happen?

The stripped artifact is what AGP packaged. Both plausible pipelines end in
the same GNU strip:

* **ndk-build**: `cmd-strip = $(PRIVATE_STRIP) $(PRIVATE_STRIP_MODE)`, with
  `APP_STRIP_MODE` defaulting to `--strip-unneeded`.
* **CMake + AGP 3.1.2**: AGP's `stripDebugSymbols` packaging task invokes the
  NDK's `arm-linux-androideabi-strip` (the same r18b binutils).

Because `--strip-all` and `--strip-unneeded` are byte-identical here, the
shipped file cannot say which of the two produced it. The *tool* (GNU bfd
2.27.0.20170315 from NDK r18b) is witnessed; the *flag spelling* is not.

---

## 6. Build-id derivation

### 6.1 The formula, confirmed on our own build

Gold's `--build-id` (default style `sha1`) computes
`SHA1(output file with the 20-byte build-id descriptor zeroed)`. On our
unstripped link:

```text
ours.unstripped.so  size 19,345,392
stored build-id     f8b44219c7dc7c9030749a502bf599c0dc093fef
sha1(desc zeroed)   f8b44219c7dc7c9030749a502bf599c0dc093fef   ← exact match
```

The stripped file preserves the same id (`f8b44219…`), but its own
`sha1(desc zeroed)` is `fdf19d25…` — because the `.symtab`/`.debug_*` bytes
were removed after the hash was taken.

### 6.2 The original was stripped after linking, not linked with `-s`

```text
original (shipped)  size 2,187,328
stored build-id     c011810da45777b83ed721b2a87d517efed0d310
sha1(desc zeroed)   d7c05dc5e9804023d8786147addc48537b0bee6d   ← not equal
```

If gold had been invoked with `-s`, it would have hashed the final stripped
layout and the two values would match. They do not, so the original link
output was unstripped and a separate strip ran afterwards.

### 6.3 Style

The note is 20 bytes, so `--build-id=md5` and `--build-id=uuid` (16 bytes)
are ruled out. Gold 1.12's `--build-id=tree` produces the **same** 20-byte
value as `sha1` on this file (measured), so the two 20-byte styles cannot be
distinguished. Both r18b build systems pass plain `--build-id`, whose default
is `sha1`, so the witnessed spelling is `sha1`.

The original's build-id cannot be reproduced until every byte of the
unstripped output matches (including `.debug_*`, which embeds
`DW_AT_comp_dir`/`DW_AT_name`). The build directory is therefore
unrecoverable from the shipped file; only the one-way hash remains.

---

## 7. APK / sibling forensics

### 7.1 r18b APK (the real one)

`Original APK/Galaxy on Fire 2™ HD_2.0.16_APKPure.apk` (6,389,308 B,
sha256 `9cfe4484…`) has 143 entries and four `lib/armeabi-v7a/*.so` files.
Only `libgof2hdaa.so` is r18b/clang 7.0.2. The other three are prebuilts:

| library | size | sha256 | compiler / linker |
|---|---:|---|---|
| `libcrashlytics.so` | 202,580 | `afbe76e6…` | GCC 4.9.x / gold 1.11, no `.note.android.ident` |
| `libfmodex.so` | 920,204 | `60ec4de0…` | GCC 4.4.3/4.6, ARMv5TE, Thumb-1, VFPv1 |
| `libfmodevent.so` | 370,660 | `b92cd6ff…` | GCC 4.4.3, ARMv5TE, VFPv2 |

The packaging is Android Gradle 3.1.2 (2018-10-22), `minSdkVersion 16`,
`targetSdkVersion 27`, `versionCode 47947032`, `versionName 2.0.16`.

### 7.2 The `/dev/shm/gof2flags` and `/tmp/gof2re2/stock` APKs

Both are r14/clang 3.8 builds (see §1.1). Their `lib/` contents:

| APK | `lib/armeabi-v7a/` members |
|---|---|
| `/dev/shm/gof2flags/GOF2_2.0.16_Original_Price.apk` | `libcrashlytics.so` (202,580), `libfmodevent.so` (370,660), `libfmodex.so` (920,204), `libgof2hdaa.so` (2,469,656, **patched** `9a166d19…`) |
| `/tmp/gof2re2/stock/net.fishlabs.gof2hdallandroid2012.apk` | `libfmodevent.so`, `libfmodex.so`, `libgof2hdaa.so` (2,469,656, stock `125dfdb2…`); no `libcrashlytics.so` |

The `gof2flags` APK is a 2024 repack (its `libgof2hdaa.so` ZIP entry carries
an NTFS extra field with a 2024-04-24 timestamp; the other libs are
2023-04-06). The `gof2re2` APK is a 2026 repack (all entries 2026-10-06).
Neither has a `Created-By` manifest line; the `gof2re2` one uses SHA-256
digests, the `gof2flags` one SHA-1.

### 7.3 Sibling ports

The four sibling repos (`gof2-vita`, `KaamoClubModApi`, `gof2edit`,
`FMOD-GOF2-Recreated`) contain no native ARM ELF, `.a`, `.o`, `.dll`, `.exe`,
`.lib` or `.dylib` for the HD game. `gof2-vita` is a loader for the SD 1.0.4
`lib/armeabi/libgof2.so` (ARMv6, `ndk23_*` JNI names); `KaamoClubModApi` is a
Windows `d3d9.dll` Rust proxy; `gof2edit` is a Rust save editor;
`FMOD-GOF2-Recreated` is an FMOD Designer project. None is a build-flag
oracle for `libgof2hdaa.so`.

---

## 8. Flag audit for `docs/VALIDATION.md`

The doc's compile sentence is:

```text
-target armv7-none-linux-androideabi16 -march=armv7-a -mthumb -mfpu=neon
-mfloat-abi=softfp -std=c++11 -fPIC -frtti -fstack-protector-strong
-funwind-tables -Oz -g -DANDROID -DNDEBUG -stdlib=libc++
```

| flag | status | witness |
|---|---|---|
| `-target …16` | **API 16 witnessed**, triple spelling unobservable | `.note.android.ident` api=16; `.ARM.attributes` v7/softfp |
| `-march=armv7-a` | **PROVEN** | `.ARM.attributes` v7/Application; Thumb-2 |
| `-mthumb` | **PROVEN** | `Tag_THUMB_ISA_use: Thumb-2`; `-marm` A/B 57 |
| `-mfpu=neon` | **PROVEN** | `Tag_Advanced_SIMD_arch: NEONv1`; `vfpv3-d16` fails NEON intrinsics |
| `-mfloat-abi=softfp` | **PROVEN** | ELF flags `0x5000200`; no `Tag_ABI_VFP_args`; hard link fails |
| `-std=c++11` | **PROVEN** vs c++14 | signed `new[]` guard; `gnu++11` byte-identical |
| `-fPIC` | **PIC PROVEN**, spelling unobservable | GOT/GLOB_DAT, no REL32/DT_TEXTREL; `-fpic` object-identical |
| `-frtti` | **behaviour PROVEN**, flag is clang default | 176 `_ZTI`/`_ZTS`, 99 `_ZTV`; `-fno-rtti` 1701 |
| `-fstack-protector-strong` | **PROVEN** | 787 `__stack_chk_fail` sites; other modes A/B |
| `-funwind-tables` | **PROVEN** | `.ARM.exidx`/`.ARM.extab`; 3566 EXIDX entries |
| `-Oz` | **PROVEN** | A/B 4507 vs 1263/1168/1167 for `-Os`/`-O2`/`-O3` |
| `-g` | **PROVEN (`-g2` class)** | 40/40 shipped `.dynsym` sizes match `-g2`, not `-g0` |
| `-DANDROID` | **UNOBSERVABLE** | no source use; codegen-neutral |
| `-DNDEBUG` | **UNOBSERVABLE** | no source use; codegen-neutral |
| `-stdlib=libc++` | **PROVEN** | `__cxa_*`/`__cxa_guard_*` bytes match `libc++abi.a`; no libstdc++ |
| `-ffunction-sections` (not in the sentence) | **PROVEN** | 193 CANT/real sections; `.text` layout |
| `-fexceptions` (not in the sentence) | **behaviour PROVEN**, flag is clang default | `_Unwind_Resume`/`__gxx_personality_v0` |
| `-D__ANDROID_API__=16` (not in the sentence) | **API PROVEN**, macro unobservable | `.note.android.ident` |

The doc's link sentence is also accurate except for one unobservable
spelling:

* `-shared`, `-soname`, `--build-id`, `-z now`, `--exclude-libs=libgcc.a`,
  no `-s`, `-z relro`, `-z noexecstack`, `--hash-style=sysv`, no
  `--gc-sections`, no `--version-script`, no `--section-ordering-file`, no
  `--unresolved-symbols`, no `--allow-multiple-definition`, no `--emit-relocs`
  — all witnessed by the ELF/dynamic/section tables.
* **`strip --strip-all` is not witnessed**; `--strip-unneeded` is ndk-build's
  default and byte-identical. The doc should say "GNU bfd strip
  (`--strip-all` or `--strip-unneeded`, indistinguishable here)".

---

## 9. Remaining uncertainties

1. **Exact build system.** CMake r18b Release is the best-supported recipe
   (`-fdata-sections` absent). Alternatives: ndk-build with an explicit
   `-fno-data-sections`; a hand-written script copying the CMake defaults.
2. **`-g2` vs `-g3`.** Byte-identical in clang 7.0.2; the shipped `.dynsym`
   proves the `-g2`/`-g3` codegen class, not the spelling.
3. **DWARF version.** `-gdwarf-2/3/4` are codegen-neutral; clang 7.0.2
   defaults to 4; `-gdwarf-5` crashes on this source.
4. **Original build directory / `DW_AT_comp_dir` / `-fdebug-prefix-map`.**
   Stripped; only the one-way build-id remains.
5. **Original unstripped size and build-id preimage.** Not shipped; our
   unstripped link is 19,345,392 B, but the original's exact size is unknown.
6. **`--strip-all` vs `--strip-unneeded`.** Byte-identical here.
7. **`-fPIC` vs `-fpic`, `-mfpu` double spelling, `-gcc-toolchain`,
   `-DANDROID`, `-DNDEBUG`.** All unobservable / object-identical.
8. **Exact AGP strip task.** AGP 3.1.2 uses the NDK strip; the flag spelling
   is unobservable.
9. **Exact NDK of `libcrashlytics.so`.** GCC 4.9.x + gold 1.11 points to
   r13b/r14; the file does not encode the package revision.

---

## 10. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w9218
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6

# APK inventory (r18b)
unzip -l "/home/debian/GalaxyOnFire2/Original APK/Galaxy on Fire 2™ HD_2.0.16_APKPure.apk" | grep -E 'lib/|\.properties'
unzip -p "/home/debian/GalaxyOnFire2/Original APK/Galaxy on Fire 2™ HD_2.0.16_APKPure.apk" META-INF/MANIFEST.MF
unzip -p "/home/debian/GalaxyOnFire2/Original APK/Galaxy on Fire 2™ HD_2.0.16_APKPure.apk" assets/crashlytics-build.properties

# the two APKs named in the brief are the r14 build
sha256sum /dev/shm/gof2flags/lib/armeabi-v7a/libgof2hdaa.so \
          /tmp/gof2re2/stock/libgof2hdaa_*.so
readelf -p .comment /tmp/gof2re2/stock/libgof2hdaa_stock.so
readelf -AW /tmp/gof2re2/stock/libgof2hdaa_stock.so | grep Tag_

# build-id formula (on our own unstripped link)
ninja -C cmake-build-match -t commands libgof2hdaa.so | tail -1   # extract the gold command
# relink to /dev/shm/w9218/ours.unstripped.so, then:
python3 - <<'PY'
import hashlib,struct
d=bytearray(open('/dev/shm/w9218/ours.unstripped.so','rb').read())
# locate .note.gnu.build-id, zero the 20-byte descriptor, sha1
...
PY

# strip A/B
NDK=.cache/ndk/android-ndk-r18b
BIN=$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin
cp ours.unstripped.so s_all.so; $BIN/arm-linux-androideabi-strip --strip-all s_all.so
cp ours.unstripped.so s_un.so;  $BIN/arm-linux-androideabi-strip --strip-unneeded s_un.so
cmp s_all.so s_un.so && echo identical
cp ours.unstripped.so s_llvm.so
$NDK/toolchains/llvm/prebuilt/linux-x86_64/bin/llvm-strip --strip-all s_llvm.so
readelf -SW s_llvm.so | wc -l        # 24 sections vs 27

# ground truth = APK copy + reorder
unzip -p "…APKPure.apk" lib/armeabi-v7a/libgof2hdaa.so > apk.so
cmp -l apk.so _work/bins/android_2.0.16_libgof2hdaa.so | wc -l   # 288
python3 tools/reorder_shstrtab.py ours.unstripped.so s_all.so
cmp -l s_all.so _work/bins/android_2.0.16_libgof2hdaa.so | wc -l  # 87041 (source residual)
```

Scratch (not committed): `/dev/shm/w9218/` — `apk/`, `r18bapk/`,
`flagsapk/`, `ours.unstripped.so`, `s_all.so`, `s_unneed.so`, `s_llvm.so`,
`s_reordered.so`, `bid_*.so`, `link_cmd.txt`, `cc_one.txt`, `cc_glob.txt`.

---

## 11. Files touched

* `_work/reconstructions/build-recipe-provenance-wave146-w9218.md` (this note).
* No source, tool, CMake, baseline, allowlist or doc change.
