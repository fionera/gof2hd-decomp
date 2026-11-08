# Build provenance round 5 (w11018): NDK r18b / clang 7.0.2 / gold 1.12,
# API 16, C++11, `-Oz`, full `-g` — and the r18b **CMake** toolchain, not
# default ndk-build

Status: **research note only**, 2026-11-08. Worktree `wt/w11018`, base master
`b3d3a1f68`. Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(2,187,328 B, sha256 `d4c17be6…bed9483`, build-id `c011810d…0d310`). Scratch
`/dev/shm/w11018`. No `src/`, `CMakeLists.txt`, baseline, allowlist or doc change is
landed; this note + `build_provenance_round5.py` are the only commits.

Gate on entry and exit (fresh `cmake --build cmake-build-match --target check -j8`):
**RATCHET PASS**, `byte_exact 4507`, `linked_exact 4507`, size 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0, parity 5370/5370. `.text` **84,652** differing bytes,
`.ARM.extab` **725**, `.rel.plt` **0**, `.note.gnu.build-id` 20. All 27 section sizes
exact; file size 2,187,328 B.

Target question: *"we have no clue if the flags are correct; how could this have been
built?"* This note re-derives the toolchain and build system from the shipped artifact,
the APK, and the NDK r18b package, and tests each remaining hypothesis against the
binary. It is deliberately independent of `CMakeLists.txt`.

---

## 0. Bottom line

1. **Toolchain identity is proven, not inferred.** The original is **NDK r18b
   (`18.1.5063045`)**, **clang 7.0.2 build 4751641 based on r328903**, **GNU gold 1.12
   (binutils 2.27.0.20170315)**, post-link **GNU strip 2.27.0.20170315**, API **16**,
   armeabi-v7a Thumb-2 VFPv3+NEONv1 **softfp**, static **libc++abi**. Its `.comment`,
   `.note.android.ident`, `.note.gnu.gold-version` and `.ARM.attributes` are
   **byte-identical** to the canonical build (same four SHA-256s).
2. **The APK pins the era and packaging tool.** `META-INF/MANIFEST.MF` says
   `Created-By: Android Gradle 3.1.2`; `assets/crashlytics-build.properties` says
   `Mon Oct 22 10:18:17 CEST 2018`, versionName `2.0.16`, versionCode `47947032`,
   package `net.fishlabs.gof2hdallandroid2012`; `AndroidManifest.xml` says
   **minSdkVersion 16**, **targetSdkVersion 27**; the APK ships **only
   `lib/armeabi-v7a/`**.
3. **Build system: the r18b CMake toolchain is the best fit, not default ndk-build.**
   The decisive observable is **`-fdata-sections`**: default r18b ndk-build adds it,
   the r18b CMake toolchain does not. The original's `.rodata` (269,159 B), `.data`
   (10,604 B) and `.bss` (address `0x216970`, size 27,052 B) match the
   no-`-fdata-sections` layout exactly; `-fdata-sections` drops `byte_exact` 4507 ->
   **4315** and shifts `.rodata`/`.data`/`.bss`. The CMake compile line also matches
   every other observable (`-fPIC`, `-mfpu=neon`, `-std=c++11`, `-frtti -fexceptions`,
   `-Oz -DNDEBUG -g`). Main alternative: ndk-build with
   `APP_CFLAGS := -fno-data-sections` + `LOCAL_ARM_NEON := true`; a hand-written
   makefile copying the CMake defaults is also possible.
4. **C++ standard level is C++11.** `-std=c++14`/`c++17`/`gnu++14` change **1,147,035**
   `.text` bytes and drop `byte_exact` to **1512**; `-std=c++98` fails to compile
   (forward-declared scoped enum); `-std=gnu++11` is byte-identical to `-std=c++11`.
5. **`-g` is full DWARF (`-g2`/`-g3`).** `-g0`, `-g1`, `-gline-tables-only` and `-gmlt`
   all drop `byte_exact` to **2071**; `-g3` is byte-identical to `-g2`.
6. **`-Oz` is the only viable level.** `byte_exact`: `-O0` **61**, `-O2` **1168**,
   `-O3` **1167**, `-Os` **1263**, `-Oz` **4507**.
7. **Correction to wave155 §4:** `--no-undefined` is **neutral**, not "must be off".
   All 207 undefined imports are resolvable from the `DT_NEEDED` libraries (FMOD,
   GLES, EGL, log, android, libm, libdl, libc); a `--no-undefined` relink of the
   canonical objects is byte-identical. CMake's default `--no-undefined` is fine.
8. **New whole-build A/B cells** (previously TU-sampled or untested) confirm the
   defaults: `-fno-use-cxa-atexit` 1356, `-fno-threadsafe-statics` 1368,
   `-fno-rtti` 1701, `-fno-math-errno` 1356, `-fno-strict-aliasing` 1371,
   `-fwrapv` 1388, `-fno-zero-initialized-in-bss` 1522, `-fshort-enums` 2871,
   `-fno-slp-vectorize` 1484, `-fno-unroll-loops` 2886, `-fvisibility=hidden` 57,
   `-marm` 57, `-march=armv7ve` 1391, `-fno-pic` 1369,
   `-fno-stack-protector` 1377, `-fno-exceptions` 1344.
   `-fno-sized-deallocation`, `-fno-aligned-new`, `-fno-plt`,
   `-fno-inline-functions-called-once`, `-funsigned-char`,
   `-fno-merge-all-constants`, `-ffp-contract=off`, `-fno-trapping-math`,
   `-fno-delete-null-pointer-checks`, `-fno-asynchronous-unwind-tables`,
   `-mno-movt`, `-fno-omit-frame-pointer`, `-UNDEBUG`, `-UANDROID`,
   `-D__ANDROID_API__=21` are neutral.
9. **New link-level A/B:** `--gc-sections`, `--as-needed`, `--hash-style=both`,
   `--sort-section=name`, gold `-O2`, `--exclude-libs=ALL`,
   `--exclude-libs=libc++abi.a`, `--long-plt`, `--no-define-common`,
   `--icf=safe`/`--icf=all`, `--build-id=md5/uuid/none` all change the output;
   `--no-undefined`, `-O1`, `--sort-common=descending`, `-z noexecstack`,
   `-z nocopyreloc`, `--fix-cortex-a8`, `--warn-shared-textrel --fatal-warnings`
   are neutral.
10. **The `.note.android.ident` is emitted by `crtbegin_so.o`, not by clang/gold.**
    Its `API 16` / `r18b` / `5063045` values are the `platforms/android-16`
    crtbegin's, so the note pins the NDK package and
    `ANDROID_PLATFORM=android-16` / `APP_PLATFORM=android-16` (not the targetSdk).

---

## 1. Evidence sources and method

| source | what it witnesses |
|---|---|
| `_work/bins/android_2.0.16_libgof2hdaa.so` | the shipped artifact: `.comment`, `.note.android.ident`, `.note.gnu.gold-version`, `.ARM.attributes`, ELF headers, section sizes, dynsym/relocs, code |
| `Original APK/Galaxy on Fire 2™ HD_2.0.16_APKPure.apk` | packaging tool (AGP), build date, versionName/Code, package, minSdk/targetSdk, ABIs, sibling `.so` toolchains |
| `.cache/ndk/android-ndk-r18b/` | exact package identity: `source.properties`, `AndroidVersion.txt`, `clang --version`, `ld.gold --version`, `strip --version`, `build/cmake/android.toolchain.cmake`, `build/core/default-build-commands.mk` |
| stock r18b ndk-build / CMake tiny projects | the exact default compile and link lines of each build system |
| `cmake-build-match/` | the canonical match build (same toolchain, `-Oz`) for A/B and section diffs |

`raw`/`aligned`/`size` are the official verifier's counts; `diff bytes` is the sum of
differing bytes over the differing sections (canonical baseline **85,397** = `.text`
84,652 + `.ARM.extab` 725 + `.note.gnu.build-id` 20). The A/B harness is
`tools/flag_fingerprint.py`; the relink harness is a small script over the canonical
gold command (see §9). The artifact/APK/NDK fingerprint is reproduced by
`_work/reconstructions/build_provenance_round5.py`.

---

## 2. Toolchain identity (proven)

### 2.1 The four byte-identical fingerprint sections

| section | size | SHA-256 (original = canonical build) |
|---|---:|---|
| `.comment` | 303 | `d4f577f46aa5f1329f9898849122325b00c135130f431950d9600f5cb73c7fee` |
| `.note.android.ident` | 152 | `522bf4171215935bd062d824e7c00f3cbc0b5312942192199f3747e6cc11115b` |
| `.note.gnu.gold-version` | 28 | `47f758e9dbe910453f5b562efab608e6b2289e178fa76940332cfcf1ca1fd40e` |
| `.ARM.attributes` | 56 | `b03afea6aa56c9ca191d5385abb498e64c7ecdf8e249a80e68704050256b1bc2` |

Exact strings:

```text
.comment:
  Android (4751641 based on r328903) clang version 7.0.2
  (https://android.googlesource.com/toolchain/clang 003100370607242ddd5815e4a043907ea9004281)
  (https://android.googlesource.com/toolchain/llvm 1d739ffb0366421d383e04ff80ec2ee591315116)
  (based on LLVM 7.0.2svn)
  GCC: (GNU) 4.9.x 20150123 (prerelease)

.note.android.ident: namesz=8 descsz=0x84 type=1 "Android\0"
  api=0x10 (16), ndk="r18b", build="5063045"

.note.gnu.gold-version: namesz=4 descsz=9 type=4 "GNU\0" "gold 1.12"

.ARM.attributes:
  41 37 00 00 00 61 65 61 62 69 00 01 2d 00 00 00 05 41 52 4d
  20 76 37 00 06 0a 07 41 08 01 09 02 0a 03 0c 01 11 02 12 04
  14 01 15 01 17 03 18 01 1a 02 1b 03 22 01 26 01
```

Decoded `.ARM.attributes`:

```text
Tag_CPU_name: "ARM v7"          Tag_CPU_arch: v7
Tag_CPU_arch_profile: Application
Tag_ARM_ISA_use: Yes            Tag_THUMB_ISA_use: Thumb-2
Tag_FP_arch: VFPv3              Tag_Advanced_SIMD_arch: NEONv1
Tag_ABI_PCS_GOT_use: GOT-indirect
Tag_ABI_PCS_wchar_t: 4          Tag_ABI_FP_denormal: Needed
Tag_ABI_FP_exceptions: Needed   Tag_ABI_FP_number_model: IEEE 754
Tag_ABI_align_needed: 8-byte    Tag_ABI_enum_size: int
Tag_ABI_HardFP_use: Deprecated  Tag_CPU_unaligned_access: v6
Tag_ABI_FP_16bit_format: IEEE 754
```

There is **no** `Tag_ABI_optimization_goals` (clang 7 does not emit it) and **no**
`Tag_Virtualization_use` (clang 3.8 does). The `.ARM.attributes` merge is the
strongest ABI evidence: ARMv7-A, Thumb-2, VFPv3 (full, not D16), NEONv1, softfp
(`Tag_ABI_HardFP_use: Deprecated`, no `Tag_ABI_VFP_args`), int enums, 4-byte wchar.

### 2.2 The NDK package that produced those strings

```text
source.properties:                 Pkg.Revision = 18.1.5063045
toolchains/.../AndroidVersion.txt: 7.0.2 / based on r328903
clang --version:                   Android (4751641 based on r328903) clang version 7.0.2 …
ld.gold --version:                 GNU gold (GNU Binutils 2.27.0.20170315) 1.12
strip --version:                   GNU strip (GNU Binutils) 2.27.0.20170315
```

The r18b `CHANGELOG.md` says "Updated Clang to build 4751641, based on r328903".
The `.note.android.ident` build number `5063045` is the r18b package revision, not
r18 (`18.0.5008260`) and not r19 (clang 8). The `.note.gnu.gold-version` says gold
1.12, which is the r18b binutils.

### 2.3 Where `.note.android.ident` comes from

`grep -r 5063045` over the NDK hits only `source.properties` and the per-platform
`crtbegin_so.o` objects. The note is therefore a property of the **crtbegin object
for `platforms/android-16`**, not of clang or gold. This is why the note proves both
the NDK revision and the native API level: the original linked
`platforms/android-16/arch-arm/usr/lib/crtbegin_so.o` (and `crtend_so.o`). The
manifest's `minSdkVersion 16` is consistent.

### 2.4 Runtime / STL identity

- `DT_NEEDED`: `liblog, libGLESv2, libGLESv1_CM, libEGL, libandroid, libfmodex,
  libfmodevent, libm, libdl, libc` — exactly the `-l` order of the canonical link.
- No `libstdc++.so` / `libc++_shared.so` NEEDED; **static libc++abi**.
- `__cxa_atexit@LIBC` imported (2 relocs); `__cxa_guard_acquire/release/abort`
  defined in `libc++abi.a` (1/1/1 relocs); `__gxx_personality_v0` defined;
  `_Unwind_*` imported (resolved by `libfmodex.so`/`libfmodevent.so` at run time).
- **Zero `_ZNSt3__1` symbols** in the dynsym: the app's C++ is header-only libc++
  plus `libc++abi.a`; no `libc++_static.a` member was pulled. The canonical link
  omits `libc++_static.a` entirely and still matches.
- `_ZTI`/`_ZTS`/`_ZTV` = **176/176/99**, identical to the canonical build.
- `__stack_chk_fail` call sites = **787**, identical; `mov r7, sp` = **220**,
  `add r7, sp, #N` = **2,473**, identical (frame-pointer schedule matches).

---

## 3. Build system: r18b CMake toolchain (high confidence)

### 3.1 The APK's packaging metadata (new)

`META-INF/MANIFEST.MF`:

```text
Manifest-Version: 1.0
Built-By: Generated-by-ADT
Created-By: Android Gradle 3.1.2
```

`assets/crashlytics-build.properties`:

```text
#Mon Oct 22 10:18:17 CEST 2018
version_name=2.0.16
package_name=net.fishlabs.gof2hdallandroid2012
build_id=f527605b-309f-404d-8afc-b68027edc49e
version_code=47947032
app_name=GOF2
```

`AndroidManifest.xml` (parsed from binary AXML):

```text
manifest.versionCode   = 47947032
manifest.versionName   = 2.0.16
manifest.package       = net.fishlabs.gof2hdallandroid2012
uses-sdk.minSdkVersion = 16
uses-sdk.targetSdkVersion = 27
```

The APK contains only `lib/armeabi-v7a/` (`libgof2hdaa.so`, `libcrashlytics.so`,
`libfmodex.so`, `libfmodevent.so`). So the native build was a single
`armeabi-v7a`, `android-16` build. AGP 3.1.2 (May 2018) + NDK r18b (Sept 2018) +
build date 2018-10-22 is a coherent 2.0.16 maintenance rebuild.

### 3.2 The two stock r18b compile lines (measured, not quoted from memory)

A tiny `std::string`/`throw` TU built with stock **ndk-build**
(`APP_ABI := armeabi-v7a`, `APP_PLATFORM := android-16`,
`APP_STL := c++_static`, `APP_CPPFLAGS := -fexceptions -frtti`,
`APP_OPTIM := release`) compiles with:

```text
clang++ -target armv7-none-linux-androideabi16
  -fdata-sections -ffunction-sections -fstack-protector-strong -funwind-tables
  -no-canonical-prefixes -g
  -Wno-invalid-command-line-argument -Wno-unused-command-line-argument
  -fno-exceptions -fno-rtti
  -gcc-toolchain <NDK>/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64
  -fpic -march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16
  -mthumb -Oz -DNDEBUG
  -I<libc++>/include -I<libc++abi>/include -I<android_support>/include
  -std=c++11 -fexceptions -frtti -DANDROID
  -Wa,--noexecstack -Wformat -Werror=format-security
  -fexceptions -frtti -frtti -fexceptions
  --sysroot <NDK>/sysroot -isystem <sysroot>/usr/include/arm-linux-androideabi
```

The same TU built with the stock **r18b CMake toolchain**
(`ANDROID_ABI=armeabi-v7a`, `ANDROID_PLATFORM=android-16`,
`ANDROID_STL=c++_static`, `ANDROID_ARM_NEON=ON`,
`ANDROID_CPP_FEATURES="rtti exceptions"`, `CMAKE_BUILD_TYPE=Release`) compiles with:

```text
clang++ --target=armv7-none-linux-androideabi16
  --gcc-toolchain=<NDK>/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64
  --sysroot=<NDK>/sysroot
  -g -DANDROID -ffunction-sections -funwind-tables -fstack-protector-strong
  -no-canonical-prefixes
  -march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16 -mthumb -mfpu=neon
  -Wa,--noexecstack -Wformat -Werror=format-security
  -std=c++11 -frtti -fexceptions -Oz -DNDEBUG -fPIC
```

The codegen-affecting differences are exactly:

| observable | ndk-build default | CMake toolchain | original |
|---|---|---|---|
| `-fdata-sections` | **yes** | no | **no** (`.rodata` 269,159 / `.data` 10,604 / `.bss` `0x216970`) |
| `-fPIC` vs `-fpic` | `-fpic` | `-fPIC` | unobservable (`-fpic` byte-identical) |
| `-mfpu` | `vfpv3-d16` (+ per-source `neon` if `LOCAL_ARM_NEON`) | `neon` | `neon` (VFPv3+NEONv1) |
| `-frtti -fexceptions` | needs `APP_CPPFLAGS` override (defaults are off) | `ANDROID_CPP_FEATURES` | on |
| `-std=c++11` | from `APP_STL := c++_static` | from `ANDROID_STL=c++_static` | on |
| `-DANDROID` position | late | early | unobservable |
| `-Wno-invalid-command-line-argument` / `-Wno-unused-command-line-argument` | yes | no | unobservable |
| `-gcc-toolchain` vs `--gcc-toolchain=` | `-gcc-toolchain` | `--gcc-toolchain=` | unobservable |

### 3.3 The decisive `-fdata-sections` A/B

| build | `.rodata` | `.data` | `.bss` addr / size | `byte_exact` |
|---|---:|---:|---|---:|
| original | 269,159 (`0x41b67`) | 10,604 (`0x296c`) | `0x216970` / 27,052 (`0x69ac`) | — |
| canonical (no `-fdata-sections`) | 269,159 | 10,604 | `0x216970` / 27,052 | **4507** |
| `-fdata-sections` | 268,807 (`0x41a07`) | 10,588 (`0x295c`) | `0x216960` / 27,052 | **4315** |

The original matches the no-`-fdata-sections` layout **exactly**. Default r18b
ndk-build always adds `-fdata-sections`; the r18b CMake toolchain does not. That is
the single strongest build-system discriminator. The CMake line also matches every
other observable without any explicit override, whereas ndk-build would need
`APP_CFLAGS := -fno-data-sections` (and `LOCAL_ARM_NEON := true`, and
`APP_CPPFLAGS := -fexceptions -frtti`) to match.

### 3.4 A weaker second hint: DT_NEEDED order

The original's `DT_NEEDED` order is `…, libm, libdl, libc`. The CMake default link
line puts `-lm` before the STL archives and `-ldl` after them, with `-lc` implicit
last, which yields `libm, libdl, libc`. The ndk-build default `TARGET_LDLIBS` is
`-lc -lm`, which would put `libc` before `libm` unless the app's `LOCAL_LDLIBS`
already listed `-lm` earlier. This is consistent with CMake but not proof.

### 3.5 Verdict and alternatives

**Most likely:** the game was built by the NDK r18b **CMake toolchain** with
`ANDROID_ABI=armeabi-v7a`, `ANDROID_PLATFORM=android-16`,
`ANDROID_STL=c++_static`, `ANDROID_ARM_NEON=ON`,
`ANDROID_CPP_FEATURES="rtti exceptions"`, `CMAKE_BUILD_TYPE=Release`, and a
custom `target_link_libraries` list that links only `libc++abi.a` (plus
`libandroid_support.a`, `libgcc.a`) — no `libc++_static.a` member is pulled.

**Alternatives, in order of likelihood:**
1. ndk-build with `APP_CFLAGS := -fno-data-sections`, `LOCAL_ARM_NEON := true`,
   `APP_CPPFLAGS := -fexceptions -frtti`, and an `Android.mk` that links the FMOD
   prebuilts / libcrypto / zlib / libzip in the witnessed order.
2. A hand-written makefile that copies the CMake defaults (no `-fdata-sections`,
   `-fPIC`, `-mfpu=neon`, `-std=c++11`, `-frtti -fexceptions`, `-Oz -DNDEBUG -g`).
3. A Gradle `externalNativeBuild` CMake block (AGP 3.1.2 supports it) — the APK's
   `Created-By: Android Gradle 3.1.2` proves the packaging tool, not the native
   build system; the native `.so` could also have been built separately and
   checked in as a prebuilt.

The exact CMake version is not encoded; the r18b toolchain requires CMake >= 3.6.0,
and AGP 3.1.2's bundled CMake is 3.6.4111459.

---

## 4. Exact compile flags (evidence and confidence)

Baseline: canonical `-Oz` build, `byte_exact 4507 / linked 4507 / size 4524`,
`.text` diff 84,652, `.ARM.extab` 725, `.rel.plt` 0.

### 4.1 Proven

| flag | value | witness | A/B |
|---|---|---|---|
| NDK | **r18b 18.1.5063045** | `.note.android.ident` `r18b`/`5063045`; `source.properties` | — |
| compiler | **clang 7.0.2 (4751641/r328903)** | `.comment`; `AndroidVersion.txt` | — |
| target API | **16** | `.note.android.ident` `0x10`; `platforms/android-16` crtbegin; manifest minSdk 16 | — |
| `-target` | `armv7-none-linux-androideabi16` | API 16 + ARMv7 | spelling unobservable: `armv7`/`armv7a`, with/without `none` produce identical objects |
| `-march` | `armv7-a` | `.ARM.attributes` v7/Application | `-march=armv7ve` 1391 |
| `-mthumb` | yes | `Tag_THUMB_ISA_use: Thumb-2` | `-marm` 57 |
| `-mfpu` | `neon` | `Tag_FP_arch: VFPv3`, `Tag_Advanced_SIMD_arch: NEONv1` | `vfpv3-d16` fails `arm_neon.h`; `neon-vfpv4` gives VFPv4+Fused-MAC |
| `-mfloat-abi` | `softfp` | no `Tag_ABI_VFP_args`, `Tag_ABI_HardFP_use: Deprecated` | `hard` link fails ("uses VFP register arguments, output does not") |
| `-std` | **C++11** | signed `new[]` guard; full C++14/17 A/B | c++14/c++17/gnu++14 1512; c++98 fails; gnu++11 neutral |
| `-Oz` | yes | size/order | `-O0` 61, `-O2` 1168, `-O3` 1167, `-Os` 1263 |
| `-g` | **full DWARF (`-g2`/`-g3`)** | dynsym sizes of 40 `-g`-sensitive functions; A/B | `-g0`/`-g1`/`-gline-tables-only`/`-gmlt` 2071; `-g3` neutral |
| `-fPIC` | yes | `Tag_ABI_PCS_GOT_use: GOT-indirect`; no `R_ARM_REL32`/`DT_TEXTREL` | `-fno-pic` 1369; `-fpic` neutral |
| `-frtti` | yes | `_ZTI`/`_ZTS`/`_ZTV` 176/176/99 | `-fno-rtti` 1701 (`.rodata` -2,841 B, `.dynsym` -2,720 B) |
| `-fexceptions` | yes | `_Unwind_Resume` 540 calls; `__cxa_throw` 4 | `-fno-exceptions` 1344 (`.text` -90,596 B) |
| `-fstack-protector-strong` | yes | 787 `__stack_chk_fail` calls | `-fno-stack-protector` 1377; basic/all worse (wave155) |
| `-funwind-tables` | yes | 3,566 `.ARM.exidx` entries, `.ARM.extab` 156,312 B | `-fno-unwind-tables` 2878 (wave155; the harness's global-flag cell is overridden by the target's `-funwind-tables`) |
| `-ffunction-sections` | yes | per-function `.text.<name>`; 272 `CANTUNWIND` exidx entries; `.text` is input order | `-fno-function-sections` would reorder (target override in the harness) |
| `-fno-omit-frame-pointer` (default) | yes | 220 `mov r7, sp`; 2,473 `add r7, sp, #N`; identical | neutral |
| `-fno-threadsafe-statics` | **no** (on) | 1/1/1 `__cxa_guard_*` relocs | 1368 |
| `-fno-use-cxa-atexit` | **no** (on) | 2 `__cxa_atexit@LIBC` relocs, no `atexit` | 1356 |
| `-fno-sized-deallocation` | default (off) | 2 `_ZdlPv`, 2 `_ZdaPv`, 0 `_ZdlPvj`/`_ZdaPvj` call sites | neutral |
| `-fno-strict-aliasing` | **no** (strict on) | codegen | 1371 |
| `-fwrapv` | **no** | codegen | 1388 |
| `-fno-math-errno` | **no** (errno on) | codegen | 1356 |
| `-fno-builtin` | **no** | `memcpy`/`memset`/`__aeabi_idiv` patterns (wave155) | wave155 |
| `-fno-slp-vectorize` | **no** (SLP on) | `vpush`/`vpop`; A/B | 1484 |
| `-fno-vectorize` | neutral | loop vectorizer already off at `-Oz` | neutral |
| `-fno-unroll-loops` | **no** (unrolling on) | codegen | 2886 |
| `-mno-unaligned-access` | **no** | `Tag_CPU_unaligned_access: v6` | 1365 (wave155) |
| `-mno-movt` | **no** | `movt` present | neutral |
| `-mcpu=cortex-a9` | **no** | `Tag_CPU_name: "ARM v7"` | 1230 (wave155) |
| `-fvisibility=hidden` | **no** | 5,370 defined exports | 57 |
| `-fvisibility-inlines-hidden` | **no** | codegen | 1278 (wave155) |
| `-fshort-enums` | **no** | `Tag_ABI_enum_size: int` | 2871 |
| `-fshort-wchar` | **no** | `Tag_ABI_PCS_wchar_t: 4` | neutral |
| `-fno-common` | **no** (`-fcommon`) | duplicate `g_android_back_button_pressed` link error on our tree | link error |
| `-stdlib=libc++` | yes | `__cxa_*`, `_ZdlPv`, no `libc++_shared` NEEDED, 0 `_ZNSt3__1` | — |
| `-DANDROID` | yes (both build systems) | codegen-neutral | neutral |
| `-DNDEBUG` | yes (Release) | codegen-neutral | neutral |
| `-D__ANDROID_API__=16` | yes (driver triple) | API 16 proven by the note; macro itself unobservable | neutral |

### 4.2 Unobservable / neutral (whole-build A/B)

`-fno-sized-deallocation`, `-fno-aligned-new`, `-fno-plt`,
`-fno-inline-functions-called-once`, `-funsigned-char`,
`-fno-merge-all-constants`, `-fno-merge-constants`, `-ffp-contract=off`,
`-fno-trapping-math`, `-fno-delete-null-pointer-checks`,
`-fno-asynchronous-unwind-tables`, `-mno-movt`, `-fno-omit-frame-pointer`,
`-UNDEBUG`, `-UANDROID`, `-D__ANDROID_API__=21`, `-fpic`, `-fno-unwind-tables`
(target override), `-fno-function-sections` (target override).

`-fno-semantic-interposition` and `-mno-ldrd`/`-mno-ldrexd` are **not accepted by
clang 7.0.2** (`unknown argument`), so they cannot be part of the original build.

### 4.3 Whole-build A/B table (new cells)

`byte` = official `byte_exact`, `linked` = `linked_exact`, `size` = size-exact
functions, `diff` = summed differing section bytes (baseline 85,397).

| variant | flags | byte | linked | size | `.text` | `.rodata` | `.ARM.extab` | changed | diff |
|---|---|---:|---:|---:|---:|---:|---:|---:|---:|
| baseline | `-Oz` | **4507** | **4507** | 4524/4524 | `0x1337c8` | `0x41b67` | `0x26298` | 0 | 85,397 |
| fno-use-cxa-atexit | `-fno-use-cxa-atexit` | 1356 | 4504 | 4523/4524 | `0x133818` | `0x41b67` | `0x2628c` | 3168 | 7,009 |
| fno-threadsafe-statics | `-fno-threadsafe-statics` | 1368 | 4499 | 4520/4521 | `0x133684` | `0x419b7` | `0x2625c` | 3153 | 7,462 |
| fno-rtti | `-fno-rtti` | 1701 | 4504 | 4524/4524 | `0x1337c8` | `0x4104e` | `0x26298` | 2823 | 207,559 |
| fno-sized-dealloc | `-fno-sized-deallocation` | 4507 | 4507 | 4524/4524 | `0x1337c8` | `0x41b67` | `0x26298` | 0 | 85,397 |
| fno-aligned-new | `-fno-aligned-new` | 4507 | 4507 | 4524/4524 | `0x1337c8` | `0x41b67` | `0x26298` | 0 | 85,397 |
| fno-plt | `-fno-plt` | 4507 | 4507 | 4524/4524 | `0x1337c8` | `0x41b67` | `0x26298` | 0 | 85,397 |
| fno-slp | `-fno-slp-vectorize` | 1484 | 4386 | 4408/4524 | `0x132ea8` | `0x41b67` | `0x26288` | 3040 | 18,504 |
| fno-loopvec | `-fno-vectorize` | 4507 | 4507 | 4524/4524 | `0x1337c8` | `0x41b67` | `0x26298` | 0 | 85,397 |
| fno-unroll | `-fno-unroll-loops` | 2886 | 4496 | 4520/4524 | `0x1337a8` | `0x41b67` | `0x262a4` | 1631 | 7,055 |
| fno-inline-once | `-fno-inline-functions-called-once` | 4507 | 4507 | 4524/4524 | `0x1337c8` | `0x41b67` | `0x26298` | 0 | 85,397 |
| fno-zero-bss | `-fno-zero-initialized-in-bss` | 1522 | 4494 | 4516/4524 | `0x133768` | `0x41b67` | `0x26298` | 3002 | 12,655 |
| funsigned-char | `-funsigned-char` | 4507 | 4507 | 4524/4524 | `0x1337c8` | `0x41b67` | `0x26298` | 0 | 85,397 |
| fshort-enums | `-fshort-enums` | 2871 | 4483 | 4511/4524 | `0x1337c8` | `0x41b67` | `0x26298` | 1651 | 856,372 |
| fno-merge-all-const | `-fno-merge-all-constants` | 4507 | 4507 | 4524/4524 | `0x1337c8` | `0x41b67` | `0x26298` | 0 | 85,397 |
| ffp-contract-off | `-ffp-contract=off` | 4507 | 4507 | 4524/4524 | `0x1337c8` | `0x41b67` | `0x26298` | 0 | 85,397 |
| fno-trapping-math | `-fno-trapping-math` | 4507 | 4507 | 4524/4524 | `0x1337c8` | `0x41b67` | `0x26298` | 0 | 85,397 |
| fdata-sections | `-fdata-sections` | 4315 | 4505 | 4524/4524 | `0x1337c8` | `0x41a07` | `0x26298` | 199 | 87,475 |
| fno-omit-fp | `-fno-omit-frame-pointer` | 4507 | 4507 | 4524/4524 | `0x1337c8` | `0x41b67` | `0x26298` | 0 | 85,397 |
| marm | `-marm` | 57 | 235 | 342/4524 | `0x1a3a78` | `0x41b67` | `0x25b18` | 4467 | 22,470 |
| march-armv7ve | `-march=armv7ve` | 1391 | 4432 | 4469/4524 | `0x133608` | `0x41b67` | `0x261c0` | 3133 | 4,021 |
| g0 | `-g0` | 2071 | 4250 | 4485/4524 | `0x1337c8` | `0x41b67` | `0x26298` | 2453 | 811,346 |
| fno-stack-protector | `-fno-stack-protector` | 1377 | 3781 | 3791/4524 | `0x12e168` | `0x41b67` | `0x2607c` | 3147 | 126,672 |
| fno-pic | `-fno-pic` | 1369 | 2954 | 3078/4524 | `0x129e08` | `0x435c7` | `0x26154` | 3155 | 15,325 |
| fpic | `-fpic` | 4507 | 4507 | 4524/4524 | `0x1337c8` | `0x41b67` | `0x26298` | 0 | 85,397 |
| fno-jump-tables | `-fno-jump-tables` | 1363 | 4415 | 4436/4524 | `0x1348c8` | `0x41947` | `0x262bc` | 3161 | 26,843 |
| fno-math-errno | `-fno-math-errno` | 1356 | 4493 | 4514/4524 | `0x133638` | `0x41b67` | `0x26270` | 3168 | 1,855 |
| mno-movt | `-mno-movt` | 4507 | 4507 | 4524/4524 | `0x1337c8` | `0x41b67` | `0x26298` | 0 | 85,396 |
| fno-strict-aliasing | `-fno-strict-aliasing` | 1371 | 4350 | 4411/4524 | `0x133d0c` | `0x41b67` | `0x262a0` | 3153 | 23,259 |
| fwrapv | `-fwrapv` | 1388 | 4466 | 4499/4524 | `0x1338a8` | `0x41b67` | `0x26298` | 3136 | 16,394 |
| fno-delete-null | `-fno-delete-null-pointer-checks` | 4507 | 4507 | 4524/4524 | `0x1337c8` | `0x41b67` | `0x26298` | 0 | 85,397 |
| fno-merge-const | `-fno-merge-constants` | 4507 | 4507 | 4524/4524 | `0x1337c8` | `0x41b67` | `0x26298` | 0 | 85,397 |
| fno-async-unwind | `-fno-asynchronous-unwind-tables` | 4507 | 4507 | 4524/4524 | `0x1337c8` | `0x41b67` | `0x26298` | 0 | 85,397 |
| fvisibility-hidden | `-fvisibility=hidden` | 57 | 239 | 244/244 | `0x130f38` | `0x41b67` | `0x26280` | 187 | 16,819 |

### 4.4 C++ standard and `-g` level

| variant | byte | linked | size | `.text` diff | `.dynsym` diff | `.rel.plt` diff | verdict |
|---|---:|---:|---:|---:|---:|---:|---|
| `-std=c++11` (baseline) | **4507** | **4507** | 4524/4524 | 84,652 | 0 | 0 | baseline |
| `-std=gnu++11` | 4507 | 4507 | 4524/4524 | 84,652 | 0 | 0 | indistinguishable |
| `-std=c++14` | 1512 | 4494 | 4514/4524 | 1,147,035 | 5,258 | 0 | not original |
| `-std=c++17` | 1512 | 4494 | 4514/4524 | 1,147,035 | 5,258 | 0 | not original |
| `-std=gnu++14` | 1512 | 4494 | 4514/4524 | 1,147,035 | 5,258 | 0 | not original |
| `-std=c++98` / `gnu++98` | — | — | — | — | — | — | compile error (forward-declared scoped enum) |
| `-g0` | 2071 | 4250 | 4485/4524 | 811,346 | 3,683 | 11 | not original |
| `-g1` | 2071 | 4253 | 4485/4524 | 803,916 | 3,683 | 11 | not original |
| `-gline-tables-only` | 2071 | 4253 | 4485/4524 | 803,916 | 3,683 | 11 | not original |
| `-gmlt` | 2071 | 4253 | 4485/4524 | 803,916 | 3,683 | 11 | not original |
| `-g3` | 4507 | 4507 | 4524/4524 | 84,652 | 0 | 0 | indistinguishable from `-g2` |

### 4.5 Optimization level

| level | byte | linked | size | `.text` | `.rodata` | `.ARM.extab` |
|---|---:|---:|---:|---:|---:|---:|
| `-O0` | 61 | 253 | 259/4524 | `0x2671f8` | `0x419d7` | `0x2a664` |
| `-O2` | 1168 | 2157 | 2411/4296 | `0x163528` | `0x42d77` | `0x25098` |
| `-O3` | 1167 | 2117 | 2247/4295 | `0x171918` | `0x42c97` | `0x2544c` |
| `-Os` | 1263 | 2650 | 2883/4356 | `0x14bfe8` | `0x43057` | `0x2476c` |
| **`-Oz`** | **4507** | **4507** | **4524/4524** | **`0x1337c8`** | **`0x41b67`** | **`0x26298`** |

---

## 5. Exact link flags (evidence and confidence)

| flag | value | witness | status |
|---|---|---|---|
| `-shared` | yes | ELF `DYN` | proven |
| `-soname libgof2hdaa.so` | yes | `DT_SONAME` | proven |
| `--build-id` | SHA-1 (20 B) | `.note.gnu.build-id` 36 B; `md5`/`uuid` 32 B and shift `.text` 131,961 B; `none` removes the note | proven |
| `-z now` | yes | `DT_FLAGS BIND_NOW`, `DT_FLAGS_1 NOW`, `.got` before `.data` in `PT_GNU_RELRO` | proven |
| `--exclude-libs=libgcc.a` | yes | no libgcc exports; `__aeabi_ldivmod` not exported | proven |
| no `-s` | yes | build-id is over the unstripped output; gold `-s` hashes the stripped file | proven |
| `--whole-archive` zlib/libzip | yes | complete zlib/libzip symbol set in `.text` | proven |
| `-l log -l GLESv2 -l GLESv1_CM -l EGL -l android` | yes | `DT_NEEDED` order | proven |
| `libfmodex.so libfmodevent.so` | yes | `DT_NEEDED` order; APK prebuilts | proven |
| `libcrypto_gof2.a` | yes | GCC 4.9 `.comment` from `sha256.o`; SHA-256 code | proven |
| `-l m` | yes | `DT_NEEDED libm.so`; `LIBC` v3 version need | proven |
| `libc++abi.a` after `-lm` | yes | `__cxa_*` code; libc++abi build path; no `_ZNSt3__1` | proven |
| `libandroid_support.a` | yes | `posix_memalign` code | proven |
| `libgcc.a` | yes | `__aeabi_ldivmod` code; FMOD lacks it | proven |
| `-l dl -l c` | yes | `DT_NEEDED libdl.so libc.so` | proven |
| `crtbegin_so.o` / `crtend_so.o` | yes | `.note.android.ident` from crtbegin; `.init_array` 3 / `.fini_array` 2 | proven |
| no `--version-script` | yes | all 5,370 symbols exported | proven |
| no `--section-ordering-file` | yes | `.text` is input order; `tools/source_order.txt` reproduces it | proven |
| no `--gc-sections` | yes | `--gc-sections` changes `.text` 946,967 B, `.rodata` 3,526 B, `.dynsym` 3,211 B, `.ARM.exidx` 17,189 B | proven |
| no `--icf` | yes | `--icf=safe` `.text` `0x130964`; `--icf=all` `0x12f99c`; 112 duplicate bodies at distinct addresses | proven |
| no LTO | yes | full `-flto` `.text` `0x12d404`, `.rodata` `0x40477`; per-TU identity 4507 (wave155) | proven |
| no `--emit-relocs` | yes | no local-code `.rel.*` | proven |
| no `--allow-multiple-definition` | yes | no duplicate strong definitions | proven |
| no `--unresolved-symbols` | yes | default shared-library undefined imports; 207 imports | proven |
| `--hash-style=sysv` | default | `.hash` only, no `.gnu.hash`; `both` adds `.gnu.hash` and changes 45,558 `.dynsym` bytes | proven (default) |
| `--no-as-needed` | default | `--as-needed` changes `.text` 140,377 B, `.dynsym` 5,624 B, `.got` 3,674 B, `.plt` 4,407 B | proven (default) |
| `-z relro` | default | `PT_GNU_RELRO` | proven (default) |
| `-z noexecstack` | default | `PT_GNU_STACK` RW | proven (default) |
| `-z combreloc` | default | `DT_RELCOUNT 1162` | proven (default) |
| `--fix-cortex-a8` | neutral | no output change | unobservable |
| `--warn-shared-textrel` / `--fatal-warnings` | neutral | no output change | unobservable |
| `--exclude-libs=libatomic.a` / `libunwind.a` | neutral | not pulled (no `std::*`; `_Unwind_*` stay UND because FMOD supplies them) | unobservable |
| `--no-undefined` | **neutral** | all 207 imports resolvable from the `DT_NEEDED` libs; relink is byte-identical | **correction to wave155** |
| linker `-O1` | neutral | no output change | unobservable |
| linker `-O2` | no | `.rodata` 229,948 B, `.dynstr` 182,625 B, `.text` 154,584 B change | disproven |
| `--sort-section=name` | no | `.text` 1,230,365 B, `.rodata` 111,033 B, `.ARM.extab` 146,106 B change | disproven |
| `--exclude-libs=ALL` | no | `.text` 230,541 B, `.dynsym` 30,562 B, `.dynstr` 164,940 B change | disproven |
| `--exclude-libs=libc++abi.a` | no | `.text` 203,339 B, `.dynsym` 30,141 B, `.dynstr` 166,744 B change | disproven |
| `--long-plt` | no | `.plt` 31,048 B, `.text` 234,905 B, `.rel.plt` 3,674 B change | disproven |
| `--no-define-common` | no | `.bss` 0x5908 vs 0x69ac, `.dynsym` 76 B, `.text` 84,660 B | disproven |
| `--sort-common=descending` | neutral | no output change | unobservable |
| `--build-id=sha1` | yes | 36 B note, `.text` diff 84,652 | proven |
| `--build-id=md5` / `uuid` | no | 32 B note, `.text` diff 131,961 | disproven |
| `--build-id=none` | no | note absent | disproven |

### 5.1 `--no-undefined` correction

Wave155 §4 said `--no-undefined` "must be off" because the original has 207
undefined imports. That is not true: **all 207 imports are resolvable from the
`DT_NEEDED` libraries** (verified by intersecting the original's UND dynsym with
the exports of `liblog`, `libGLESv2`, `libGLESv1_CM`, `libEGL`, `libandroid`,
`libfmodex`, `libfmodevent`, `libm`, `libdl`, `libc`; unresolved = 0). Relinking
the canonical objects with `--no-undefined` produces byte-identical sections. So
CMake's default `--no-undefined` (without `ANDROID_ALLOW_UNDEFINED_SYMBOLS=ON`) is
fine. The FMOD libraries are the reason `_Unwind_*`, `__aeabi_*` and `__gnu_*`
imports resolve at link time.

---

## 6. What remains unknowable (and why it does not matter)

1. **Exact CMake version.** The r18b toolchain requires CMake >= 3.6.0; AGP 3.1.2's
   bundled CMake is 3.6.4111459. The CMake version is not encoded in the `.so`.
2. **Exact Gradle version.** Not encoded; AGP 3.1.2 is.
3. **Build directory / `DW_AT_comp_dir` / `-fdebug-prefix-map`.** Stripped; the
   build-id is a one-way SHA-1 over the unstripped output.
4. **`-g2` vs `-g3`.** Byte-identical in clang 7.0.2.
5. **`-fPIC` vs `-fpic`.** Byte-identical on this source.
6. **Triple spelling.** `armv7`/`armv7a`, with/without `none` produce identical
   objects; only API 16 is observable.
7. **`-mtune=*`.** A no-op in clang 7.0.2 (no `-tune-cpu` in cc1).
8. **`-DANDROID`, `-DNDEBUG`, `-D__ANDROID_API__` macro use.** Codegen-neutral on
   this source; only the API level is proven by the note.
9. **Neutral flags listed in §4.2.** They cannot be distinguished from their
   defaults on this tree.
10. **Whether Gradle invoked the native build.** `Created-By: Android Gradle 3.1.2`
    proves packaging; the native build could have been a separate CMake/make
    invocation with the `.so` checked in as a prebuilt.
11. **CMake target name / `-D<target>_EXPORTS`.** Not used by the source.
12. **Original unstripped size / build-id preimage.** Not shipped.

---

## 7. Corrections / refinements to prior notes

- **`--no-undefined` is neutral, not "must be off"** (wave155 §4). See §5.1.
- **`-fno-common` is consistent with `-fcommon`, not proof.** The duplicate
  `g_android_back_button_pressed` link error proves our reconstruction needs
  `-fcommon`; the original source could in principle have used `extern`
  declarations. Clang 7.0.2's default is `-fcommon`, so no flag is needed.
- **`-fno-sized-deallocation` "0 `_ZdlPvj`/`_ZdaPvj`" means 0 call sites, not 0
  symbols.** The shipped dynsym defines `_ZdlPvj`/`_ZdaPvj` as weak libc++abi
  aliases; the relocations use `_ZdlPv`/`_ZdaPv` (2 each), which is the C++11
  default. `-fsized-deallocation` changes the build (wave155 1347).
- **`__cxa_throw` call sites are 4, not 3** (wave155 §3 table). The canonical build
  matches 4/4.
- **The harness's global `-fno-unwind-tables` / `-fno-function-sections` cells are
  neutral** because `target_compile_options` appends `-funwind-tables
  -ffunction-sections` after `CMAKE_CXX_FLAGS`; use the wave155 launcher cell
  (2878) for the real `-fno-unwind-tables` result.

---

## 8. Reproduction

### 8.1 Artifact / APK / NDK fingerprint

```sh
cd /home/debian/GalaxyOnFire2/wt/w11018
python3 _work/reconstructions/build_provenance_round5.py \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --apk "/home/debian/GalaxyOnFire2/Original APK/Galaxy on Fire 2™ HD_2.0.16_APKPure.apk" \
  --ndk .cache/ndk/android-ndk-r18b
```

### 8.2 Stock build-system flag lines

```sh
# ndk-build
NDK=.cache/ndk/android-ndk-r18b
mkdir -p /dev/shm/w11018/ndktest/jni
# jni/Android.mk: LOCAL_MODULE := t; LOCAL_SRC_FILES := t.cpp;
#                 LOCAL_CPPFLAGS := -fexceptions -frtti; LOCAL_LDLIBS := -llog
# jni/Application.mk: APP_ABI := armeabi-v7a; APP_PLATFORM := android-16;
#                     APP_STL := c++_static; APP_CPPFLAGS := -fexceptions -frtti;
#                     APP_OPTIM := release
$NDK/ndk-build NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=jni/Android.mk \
  NDK_APPLICATION_MK=jni/Application.mk V=1 -B | grep clang

# CMake
$NDK/build/cmake/android.toolchain.cmake with:
  -DANDROID_ABI=armeabi-v7a -DANDROID_PLATFORM=android-16
  -DANDROID_STL=c++_static -DANDROID_ARM_NEON=ON
  -DANDROID_CPP_FEATURES="rtti exceptions" -DCMAKE_BUILD_TYPE=Release
```

### 8.3 Whole-build flag A/B

```sh
cd /home/debian/GalaxyOnFire2/wt/w11018
python3 tools/flag_fingerprint.py --mode variants \
  --variants-file /dev/shm/w11018/variants_new.json \
  --workdir /dev/shm/w11018/fp --jobs 8
```

### 8.4 Link-level A/B

Relink the canonical objects with the gold command from
`cmake-build-match/build.ninja` and add/replace the flag under test; compare
`readelf -SW` sizes and per-section differing bytes against the original. The
canonical baseline is `.text` 84,652, `.rodata` 0, `.data` 0, `.dynsym` 0,
`.rel.plt` 0, `.ARM.extab` 725.

---

## 9. Files changed

- `_work/reconstructions/build-provenance-round5-toolchain.md` — this note.
- `_work/reconstructions/build_provenance_round5.py` — artifact/APK/NDK
  fingerprint reproducer.
- No `src/`, `CMakeLists.txt`, baseline, allowlist or doc change.
