# Build archaeology, wave 155 (w10120): the original recipe, flag by flag

Status: **note-only research commit**, 2026-11-08. Worktree
`/home/debian/GalaxyOnFire2/wt/w10120`, base master `f4e0ea40b` (wave 154).
Scratch: `/dev/shm/w10120`. No `src/`, `tools/`, `CMakeLists.txt`, baseline,
allowlist or doc change is landed. This note is the only commit.

Gate at the base and after this pass (fresh `cmake --build cmake-build-match --target check -j32`):

```text
verify: byte 4507/4524  linked 4507/4524  size 4524/4524  avg 99.93
data:   exact 843/843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0  imports 207
symbol parity 5370/5370 (0 extra / 0 missing)
lint CLEAN   drift 0
RATCHET PASS
```

Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`,
sha256 `d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483`,
2,187,328 B. Canonical build in this worktree:
sha256 `1b11776e5463b94417435be1a327c3b98af5e161126e16936966c94851a1044c`,
same size.

Section deltas of the canonical build vs the original (all 27 section sizes exact):

| section | size | differing bytes |
|---|---:|---:|
| `.text` | 1,259,464 | **86,347** |
| `.ARM.extab` | 156,312 | **1,255** |
| `.note.gnu.build-id` | 36 (20-byte descriptor) | **19** (descriptor unreproducible) |
| `.ARM.exidx`, `.rel.plt`, `.rodata`, `.data`, `.bss`, `.got`, `.dynsym`, `.dynstr`, `.hash`, `.gnu.version*`, `.rel.dyn`, `.plt`, `.comment`, `.ARM.attributes`, `.shstrtab` | exact | **0** |

The 17 residual functions (raw same-address diff sum **92,239 B**; instruction-index
aligned total over the 15 unique functions **23,941**) are source-shape residuals,
not build-flag residuals. The `.text` section diff is 86,347 B; the per-function sum
is larger because it double-counts the C1/C2 aliases and includes literal-pool bytes.

---

## 0. Bottom line

1. **No original Android build file survives.** The APK, both IPAs, the Windows
   build, the source-layout dumps, the KiritoJPK archives/OBB, the sibling repos
   and the public web contain no `Android.mk`, `Application.mk`, `CMakeLists.txt`,
   `*.cmake`, `build.gradle`, `*.ninja`, `compile_commands.json`, `Makefile`,
   `*.d`, build log, `.o`, `.a`, `.pdb` or `.dSYM` belonging to the original
   Fishlabs Android build. What survives is the ELF itself.
2. **The ELF pins the toolchain exactly:** NDK r18b (`18.1.5063045`), clang
   7.0.2 (`4751641 based on r328903`), gold 1.12, API 16, armeabi-v7a,
   Thumb-2, VFPv3, NEONv1, softfp, static libc++, `-Oz -g2`-class.
3. **The compile line is proven flag-by-flag** by `.comment`,
   `.note.android.ident`, `.ARM.attributes`, the section layout, and a large
   body of controlled A/B cells (w8119/w8319/w8419/w8619/w9517 plus this pass).
4. **The link line is proven by the dynamic tables and section layout:** a
   single plain gold 1.12 link
   `-shared -soname libgof2hdaa.so --build-id -z now --exclude-libs=libgcc.a`,
   no `-s`, no version script, no section-ordering file, no `--icf`, no LTO;
   then GNU bfd `strip` and the repo's `.shstrtab` reorder.
5. **The native build is CMake, not stock ndk-build** (two independent
   discriminators: no `-fdata-sections`; STL archives after the user/shared
   libraries). The exact CMake version and build-file text are unrecoverable.
6. **NEW this pass: ICF is disproven independently of the gate.** The original
   has 112 duplicate function bodies at distinct addresses (e.g.
   `AEMath::MatrixTransformVector` / `AEMath::operator*` 122 B,
   `Ship::getHandling` / `Ship::getHandlingForShop` 76 B). `--icf=safe`
   shrinks `.text` from `0x1337c8` to `0x130964`; `--icf=all` was already
   disproven. ICF is off.
7. **NEW this pass: LTO is disproven with the plugin present.** NDK r18b ships
   `LLVMgold.so`, and a `-flto` object links through gold with
   `-plugin .../LLVMgold.so`. A full `-flto` build gives `.text 0x12d404`
   (1,233,924 B) and `.rodata 0x40477`, not the original's `0x1337c8` /
   `0x41b67`. The original is not LTO.
8. **NEW this pass: the original `.text` rank order is exactly our build's.**
   All **4,522/4,522** common defined functions have the same address rank in
   both files. This is a direct proof that no profile-guided section-prefix
   relocation (and no LTO/ICF reordering) moved any function.
9. **PGO is still the one open build-provenance question.** The original's
   switch block order is source order, which needs branch weights. A full
   `-fprofile-instr-use`/`-fprofile-sample-use` build perturbs every function
   (and `-mllvm -profile-guided-section-prefix` is ON by default in clang
   7.0.2, which would move hot functions). The evidence is consistent with
   either (a) a profile injected at IR level for one function, or (b) a
   non-default `-mllvm -profile-guided-section-prefix=false` build with a
   profile that only covers the residual switch function. No shipped
   `.profraw`/`.profdata` exists, so the weight vector is unrecovered.

---

## 1. Artifact inventory

| artifact | path | size | sha256 | what it proves |
|---|---|---:|---|---|
| ground-truth `.so` | `_work/bins/android_2.0.16_libgof2hdaa.so` | 2,187,328 | `d4c17be6…bed9483` | toolchain, ABI, flags, link shape |
| shipping APK | `Original APK/Galaxy on Fire 2™ HD_2.0.16_APKPure.apk` | 6,389,308 | `9cfe4484…a9eb19` | AGP 3.1.2 packaging, build date |
| APK `lib/armeabi-v7a/libgof2hdaa.so` | in the APK | 2,187,328 | `6d38fd76…e3b58` | raw bfd-strip artifact (ground truth = this + `.shstrtab` reorder) |
| iOS 1.1.4 HD | `_work/bins/ios_1.1.4_HD` | 4,983,008 | `74ece5be…54958` | cross-platform source shape; no Android flags |
| iOS 1.0.5 armv7 | `_work/bins/ios_1.0.5_armv7` | 3,866,736 | `b8e8000f…f44661` | cross-platform source shape; no Android flags |
| Windows `GOF2.exe` | `_work/bins/win_GOF2.exe` | 2,822,736 | `1234131b…a1fb9` | VS2010 PC build; no Android flags |
| Android FMOD Ex | `_work/bins/apk_libs/libfmodex.so` | 920,204 | `60ec4de0…c3fa81` | GCC 4.4.3/4.6, ARMv5TE, prebuilt |
| Android FMOD Event | `_work/bins/apk_libs/libfmodevent.so` | 370,660 | `b92cd6ff…c97d` | GCC 4.4.3, ARMv5TE, prebuilt |
| source-layout dumps | `source file list dump/` | 4 files | — | Windows/mac/iOS/Zeebo TU lists; **0** `Android` occurrences |
| historical scratch build | `_bg/compile_commands.json` | 199 entries | — | an old reconstruction (API 21, `-pre-RA-sched=source`), not original |
| historical mac build | `_cmake-build-match.mac/compile_commands.json` | 201 entries | — | an old reconstruction (API 21, darwin host), not original |
| KiritoJPK APK archive | `KiritoJPK_.../GOF2FHD_APK.7z` | 142,323,352 | — | 14 APKs, no build files; one r14/clang 3.8 lib variant |
| OBB archive | `KiritoJPK_.../GOF2FHD_OBB.7z` | 1,581,092,293 | — | game content only |

No `Android.mk`, `Application.mk`, `CMakeLists.txt`, `*.cmake`, `build.gradle`,
`gradle.properties`, `*.ninja`, `compile_commands.json`, `Makefile`, `*.pbxproj`,
`*.vcxproj`, `*.sln`, `*.d`, `*.o`, `*.a`, `*.pdb`, `*.dSYM`, `build.sh`,
`build.bat` or build log was found in any of the original artifacts. The only
surviving build-system strings are:

```text
APK META-INF/MANIFEST.MF:  Built-By: Generated-by-ADT
                           Created-By: Android Gradle 3.1.2
APK assets/crashlytics-build.properties:
  #Mon Oct 22 10:18:17 CEST 2018
  version_name=2.0.16
  version_code=47947032
  package_name=net.fishlabs.gof2hdallandroid2012
  build_id=f527605b-309f-404d-8afc-b68027edc49e
AndroidManifest.xml:        minSdkVersion 16, targetSdkVersion 27,
                            glEsVersion 0x00020000
```

`classes.dex` contains only generic Gradle/Firebase/Crashlytics strings
(`gradle`, `build.gradle`, `LOCAL_`, `APP_`, `ANDROID_`, `ninja` in a TLD
regex); it has no `externalNativeBuild`, `cmake`, `arguments`, `cppFlags`,
`abiFilters`, `cFlags` or `stl` block. The source-layout dumps have sections
`windows`, `mac fhd`, `gof2 hd IOS 1.1.19`, `gof2 sd 1.0.1 ios`, `gof3d 1.1.5`,
`gof3d 1.1.4`, `gof3d zeebo` and **no Android section** (0 case-insensitive
`android` hits in all four files).

---

## 2. Reconstructed original command lines

### 2.1 Compile (one C++ TU)

```sh
$NDK/toolchains/llvm/prebuilt/<host>/bin/clang++ \
  -target armv7-none-linux-androideabi16 \
  -march=armv7-a -mthumb -mfpu=neon -mfloat-abi=softfp \
  -std=c++11 -fPIC -frtti -fstack-protector-strong \
  -ffunction-sections -funwind-tables -Oz -g \
  -stdlib=libc++ \
  -isystem $NDK/sources/cxx-stl/llvm-libc++/include \
  -isystem $NDK/sources/cxx-stl/llvm-libc++abi/include \
  -isystem $NDK/sources/android/support/include \
  --sysroot=$NDK/sysroot \
  -isystem $NDK/sysroot/usr/include \
  -isystem $NDK/sysroot/usr/include/arm-linux-androideabi \
  -D__ANDROID_API__=16 -DANDROID -DNDEBUG \
  -c <TU>.cpp -o <TU>.o
```

The C TUs use the same line minus `-std=c++11 -frtti -stdlib=libc++` and the
libc++/libc++abi include paths. `-fexceptions` is clang's C++ default and is
implicit; `-fno-omit-frame-pointer` is clang's ARM `-Oz` default and is
implicit; `-fdata-sections` is absent.

### 2.2 Link

```sh
$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/<host>/arm-linux-androideabi/bin/ld.gold \
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
  $NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/<host>/lib/gcc/arm-linux-androideabi/4.9.x/armv7-a/thumb/libgcc.a \
  -l dl -l c \
  $NDK/platforms/android-16/arch-arm/usr/lib/crtend_so.o
```

`tools/source_order.txt` is a plain 203-line list (11 comment lines + 192
`src/...` entries); gold emits symbols in input order, so this is the original
TU order.

### 2.3 Post-link

```sh
$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/<host>/bin/arm-linux-androideabi-strip \
  --strip-all -o libgof2hdaa.so libgof2hdaa.unstripped.so
python3 tools/reorder_shstrtab.py libgof2hdaa.unstripped.so libgof2hdaa.so
```

The `reorder_shstrtab.py` step is a **repo post-processing step**, not part of
the original build: the shipped APK library is the raw bfd-strip artifact; the
ground-truth file is that artifact after restoring gold's Stringpool
`.shstrtab` order. The two differ by exactly 288 bytes (259 in `.shstrtab`,
29 across 26 `sh_name` fields).

---

## 3. Compile-flag evidence table

| flag | value | artifact / witness | status |
|---|---|---|---|
| toolchain | NDK r18b (`18.1.5063045`) | `.note.android.ident` = `10 00 00 00 72 31 38 62 … 35 30 36 33 30 34 35` (API 16, `r18b`, `5063045`), byte-identical to r18b `crtbegin_so.o` | **proven** |
| compiler | clang 7.0.2 (`4751641 based on r328903`) | `.comment` string | **proven** |
| target | `armv7-none-linux-androideabi16` | `.note.android.ident` API 16; `.ARM.attributes` ARM v7 / Application; ELF `EF_ARM_ABI_FLOAT_SOFT` | **API/ABI proven; triple spelling unobservable** |
| `-march=armv7-a` | yes | `.ARM.attributes` `Tag_CPU_arch: v7`, `Tag_CPU_arch_profile: Application`; `movw`/`movt` | **proven** |
| `-mthumb` | yes | `.ARM.attributes` `Tag_THUMB_ISA_use: Thumb-2`; every function entry has the Thumb bit; `-marm` A/B 57 | **proven** |
| `-mfpu=neon` | yes | `.ARM.attributes` `Tag_Advanced_SIMD_arch: NEONv1`; `vld1`/`vst1`; `vfpv3-d16` fails `arm_neon.h` | **proven** |
| `-mfloat-abi=softfp` | yes | ELF flags `0x5000200`; no `Tag_ABI_VFP_args`; hard-float link fails against FMOD | **proven** |
| `-std=c++11` | yes | signed `new[]` guard (`orr rX, n, n, asr #31`) emitted only for C++98/C++11; full `c++14` build 1512 | **proven** |
| `-fPIC` | yes | `Tag_ABI_PCS_GOT_use: GOT-indirect`; 614 `GLOB_DAT`, 0 `REL32`, no `DT_TEXTREL`; `-fpic` object-identical | **PIC proven; spelling unobservable** |
| `-frtti` | yes | 176 `_ZTI` / 176 `_ZTS` / 99 `_ZTV`; `-fno-rtti` 1701 | **proven** |
| `-fexceptions` | yes | `_Unwind_Resume` 540 calls / 525 funcs; `__cxa_throw` 3/3; `-fno-exceptions` 1344 | **proven** |
| `-fstack-protector-strong` | yes | 678 canary functions / 787 `__stack_chk_fail` sites, per-function vector identical; basic 1375, all 89, none 1375 | **proven** |
| `-funwind-tables` | yes | 3,566 `.ARM.exidx` entries: 856 compact, 2,710 inline-pointer, 0 `CANTUNWIND`; `.ARM.extab` 156,312 B; `-fno-unwind-tables` 2878 | **proven** |
| `-ffunction-sections` | yes | every function has its own `.text.<name>` in the link map; `.text` order is input order | **proven** |
| `-fdata-sections` | **no** | `.rodata` 269,159 B and `.data` 10,604 B are byte-identical only without it; with it `.rodata` 268,807 B / `.data` 10,588 B and byte_exact 4315 | **disproven** |
| `-Oz` | yes | `-Os` 1263, `-O2` 1168, `-O3` 1167, `-O1` 1033, `-O0` 61; 192/192 TUs size-exact at `-Oz` | **proven** |
| `-g` | yes (`-g2` class) | `-g0`/`-g1`/`-gline-tables-only` change 2,453 function bodies and drop byte_exact to 2071; `-g2`/`-g3`/`-gdwarf-2/3/4` codegen-identical | **proven; exact level unobservable** |
| `-fno-omit-frame-pointer` | yes (default) | 2,473 `add r7, sp, #N` + 220 `mov r7, sp` prologues, identical; `-fomit-frame-pointer` 1310, `-momit-leaf-frame-pointer` 1390 | **proven (default)** |
| `-fno-threadsafe-statics` | no | 2 `__cxa_guard_acquire`, 1 `__cxa_guard_release`, 1 `__cxa_guard_abort`; variant 1366 | **default on** |
| `-fno-use-cxa-atexit` | no | 2 `__cxa_atexit`, no `atexit`; variant 1356 | **default on** |
| `-fno-sized-deallocation` | yes (default) | 0 `_ZdlPvj`/`_ZdaPvj`; sized variant 1347 | **default** |
| `-fno-strict-aliasing` | no | `RenderMesh` 1040 vs 1032; variant 1371 | **strict aliasing on** |
| `-fwrapv` / `-fno-strict-overflow` | no | both 1386 | **off** |
| `-fno-math-errno` | no | variant 1356 | **math-errno on** |
| `-ffast-math` | no | variant 1335 | **off** |
| `-fno-builtin` | no | `memcpy` 3, `memset` 5, `__aeabi_idiv` 68/46, `__aeabi_uidiv` 29/19; `-fno-builtin-malloc` 1488, `-fno-builtin-sqrtf` 1426 | **builtins on** |
| `-fno-slp-vectorize` | no | `vpush` 232 / `vpop` 83; variant 1484 | **SLP on** |
| `-fno-vectorize` | neutral | loop vectorizer already off at `-Oz` | **unobservable** |
| `-mno-unaligned-access` | no | `Tag_CPU_unaligned_access: v6`; variant 1365 | **unaligned on** |
| `-mno-movt` | no | 25 `movt` in both; neutral | **movt on** |
| `-mcpu=cortex-a9` | no | `.ARM.attributes` `Tag_CPU_name: "ARM v7"`; variant 1230 | **generic armv7-a** |
| `-mtune=*` | unobservable | not encoded; generic/cortex-a5/a7/a8/a9/a15 all neutral at `-Oz` | **unobservable** |
| `-mfpu=neon-vfpv4` | no | would change `.ARM.attributes` to VFPv4 / Fused-MAC; neutral gate but not authentic | **disproven by attributes** |
| `-fvisibility=hidden` | no | 5,370 defined exports; variant 57 | **default visibility** |
| `-fvisibility-inlines-hidden` | no | variant 1278 | **off** |
| `-fshort-enums` | no | `Tag_ABI_enum_size: int`; variant 2871; `Level.cpp` alone 2367 | **int enums** |
| `-fshort-wchar` | no | `Tag_ABI_PCS_wchar_t: 4`; neutral | **wchar 4** |
| `-fno-common` | no | duplicate `g_android_back_button_pressed` link error | **`-fcommon`** |
| `-DANDROID` | yes (CMake default) | codegen-neutral; no source use | **unobservable** |
| `-DNDEBUG` | yes (CMake Release) | codegen-neutral; no source use | **unobservable** |
| `-D__ANDROID_API__=16` | yes | API 16 proven by `.note.android.ident`; macro itself unobservable | **API proven; macro unobservable** |
| `-stdlib=libc++` | yes | `__cxa_*` / `__cxa_guard_*` bytes match `libc++abi.a`; no `libstdc++` NEEDED | **proven** |
| `-fmerge-constants` / `-fmerge-all-constants` | neutral | full build byte-identical | **unobservable** |
| `-fno-plt` | neutral | `.text` byte-identical | **unobservable** |
| `-fno-semantic-interposition` | unsupported | clang 7.0.2: unknown argument | **not used** |
| `-mno-ldrd` | unsupported | clang 7.0.2: unknown argument | **not used** |
| `-fno-integrated-as` | no | GNU as 4.9 build 2767 | **integrated as** |
| `-fexperimental-new-pass-manager` | no | 1300 | **legacy pass manager** |
| `-mllvm -pre-RA-sched=source` | no | 1199 | **default `list-hybrid`** |
| `-mllvm -enable-misched` | no | 1219 | **misched off** |
| `-mllvm -disable-cgp-branch-opts` | no | 4506 (breaks `Engine::SetUVMatrix`) | **CGP branch opts on** |
| `-mllvm -arm-promote-constant` | no | 1386; default is off | **default** |
| `-mllvm -arm-global-merge=false` | no | 1388 | **global merge on** |
| `-mllvm -arm-adjust-jump-tables=false` | no | 1571 | **jump-table adjustment on** |
| `-mllvm -arm-use-mulops=false` | no | 1472 | **mulops on** |
| `-mllvm -arm-constant-island-max-iteration=2` | no | 4451; default 10 | **default 10** |

The full A/B matrices are in `build-pgo-provenance-wave139-w8419.md`,
`build-flag-fingerprint-evidence-wave140-w8619.md`,
`build-provenance-a-flags-wave149-w9517.md` and `untested_compile_flags_w3695.md`.

---

## 4. Link-flag evidence table

| flag | value | artifact / witness | status |
|---|---|---|---|
| `-shared` | yes | ELF `DYN` | **proven** |
| `-soname libgof2hdaa.so` | yes | `DT_SONAME` | **proven** |
| `--build-id` | yes | 20-byte `.note.gnu.build-id`; gold SHA-1 of the unstripped output with the descriptor zeroed | **proven** |
| `-z now` | yes | `DT_FLAGS BIND_NOW`, `DT_FLAGS_1 NOW`; `.got` before `.data` inside `PT_GNU_RELRO` | **proven** |
| `--exclude-libs=libgcc.a` | yes | libgcc symbols local; `__aeabi_ldivmod` not exported | **proven** |
| no `-s` | yes | shipped build-id is not the SHA-1 of the stripped file; a gold `-s` link cannot reproduce it | **proven** |
| `--whole-archive libz libzip` | yes | zlib/libzip code is in `.text`; symbol set complete | **proven** |
| `-l log -l GLESv2 -l GLESv1_CM -l EGL -l android` | yes | `DT_NEEDED` order | **proven** |
| `libfmodex.so libfmodevent.so` | yes | `DT_NEEDED` order; APK prebuilts | **proven** |
| `libcrypto_gof2.a` | yes | `.comment` GCC 4.9 string from `sha256.o`; `SHA256_*` code in `.text` | **proven** |
| `-l m` | yes | `DT_NEEDED libm.so`; `libm` version need | **proven** |
| `libc++abi.a` after `-lm` | yes | `.comment` clang string from `crtbegin_so.o`; `__cxa_*` code; darwin path string | **proven** |
| `libandroid_support.a` | yes | `posix_memalign` code at `0x19ae14` | **proven** |
| `libgcc.a` | yes | `__aeabi_ldivmod` at `0x19ae54`; FMOD lacks it | **proven** |
| `-l dl -l c` | yes | `DT_NEEDED libdl.so libc.so` | **proven** |
| `crtbegin_so.o` / `crtend_so.o` | yes | `.note.android.ident` from crtbegin; `.init_array` 3 / `.fini_array` 2 | **proven** |
| no `--version-script` | yes | all 5,370 symbols exported | **proven** |
| no `--section-ordering-file` | yes | `.text` order is input order; `tools/source_order.txt` reproduces it | **proven** |
| no `--gc-sections` | yes | all sections retained | **proven** |
| no `--icf` | yes | 112 duplicate bodies at distinct addresses; `--icf=safe` `.text` `0x130964` vs `0x1337c8`; `--icf=all` 1347 | **disproven** |
| no LTO | yes | full `-flto` build `.text 0x12d404`, `.rodata 0x40477`; per-TU identity 4507 | **disproven** |
| no `--emit-relocs` | yes | no local-code `.rel.*` | **proven** |
| no `--allow-multiple-definition` | yes | no duplicate strong definitions | **proven** |
| no `--unresolved-symbols` | yes | default shared-library undefined imports; 207 imports | **proven** |
| `-z relro` | default | `PT_GNU_RELRO` present; gold default | **proven (default)** |
| `-z noexecstack` | default | `PT_GNU_STACK` RW | **proven (default)** |
| `--hash-style=sysv` | default | `.hash` only, no `.gnu.hash` | **proven (default)** |
| `--no-as-needed` | default | all `DT_NEEDED` present | **proven (default)** |
| `-z combreloc` | default | `DT_RELCOUNT 1162` | **proven (default)** |
| `--fix-cortex-a8` | neutral | no output change | **unobservable** |
| `--warn-shared-textrel` / `--fatal-warnings` | neutral | no output change | **unobservable** |
| `--exclude-libs=libatomic.a` / `libunwind.a` | neutral | not pulled (no `std::*`; `_Unwind_*` stay UND) | **unobservable** |
| `--no-undefined` | must be off | original has 207 undefined imports; CMake must set `ANDROID_ALLOW_UNDEFINED_SYMBOLS=ON` | **recipe requirement** |
| linker `-O1` | neutral | no output change | **unobservable** |
| linker `-O2` | no | wrong section layout | **disproven** |

---

## 5. Strip / post-link evidence

| item | value | witness | status |
|---|---|---|---|
| strip tool | GNU bfd `arm-linux-androideabi-strip` 2.27.0.20170315 | 27 sections; `llvm-strip` drops `.comment`, `.note.gnu.gold-version`, `.ARM.attributes` and gives 24 sections | **proven** |
| strip flag | `--strip-all` or `--strip-unneeded` | byte-identical output on this file | **tool proven; spelling unobservable** |
| strip timing | after the link, not gold `-s` | shipped build-id is not the SHA-1 of the stripped file | **proven** |
| `.shstrtab` reorder | repo post-processing | APK lib vs ground truth differ by exactly 288 bytes (259 `.shstrtab` + 29 `sh_name`) | **repo step, not original** |
| build-id | `c011810da45777b83ed721b2a87d517efed0d310` | 20-byte descriptor; SHA-1 of the unstripped output with the descriptor zeroed | **original value unreproducible (build dir/DWARF)** |

---

## 6. Build system: CMake, not stock ndk-build

Two independent discriminators:

1. **No `-fdata-sections`.** ndk-build r18b's `GLOBAL_CFLAGS` includes
   `-fdata-sections`; the r18b CMake toolchain does not. The original's
   `.rodata` is 269,159 B and `.data` 10,604 B, byte-identical to the
   no-`-fdata-sections` build; with it `.rodata` 268,807 B / `.data` 10,588 B
   and `byte_exact` drops 4507 -> 4315.
2. **STL archives after the user/shared libraries.** The original link order is
   `FMOD -> libcrypto -> -lm -> libc++abi.a -> libandroid_support.a ->
   libgcc.a -> -ldl -lc`, which is the CMake `target_link_libraries` order.
   ndk-build places static STL archives before the shared libraries, which
   pulls `libunwind.a` and defines `_Unwind_*`; the original keeps `_Unwind_*`
   UND.

The r18b CMake toolchain defaults that match the binary:

```text
ANDROID_ABI=armeabi-v7a
ANDROID_PLATFORM=android-16
ANDROID_STL=c++_static
ANDROID_ARM_NEON=ON
ANDROID_CPP_FEATURES="rtti exceptions"   (or default ON)
ANDROID_ALLOW_UNDEFINED_SYMBOLS=ON       (207 unresolved imports)
CMAKE_BUILD_TYPE=Release
```

The exact CMake version, generator (Ninja vs Make), build directory,
`CMakeLists.txt` text and whether the settings were in the file or on the
command line are **unrecoverable**. The best-supported reconstruction is the
stock `$NDK/build/cmake/android.toolchain.cmake` with the above settings; the
repo's `cmake/toolchain.cmake` is a hand-written equivalent that is
object-identical to the stock toolchain on all 192 TUs (w7010).

---

## 7. PGO status (open)

The original's switch block order is source order, not numeric. A new
jump-table census (w9319) reads every `TBH`/`TBB` table from the shipped `.so`:
of 16 source switches that could be matched to a table, **6 are exactly source
order and 10 are source order with one case moved; none is numeric**. That is
the direct binary witness that the original compiler had branch weights.

What is proven:

* A **flat** `-fprofile-instr-use` profile is falsified: it changes every TU
  (0/188 objects byte-identical) and drops the gate to 1274/3485.
* A **non-uniform** profile can reproduce source order, but the tested
  descending-weight vector gives 1408 B (original 1432 B) and raw 1369
  (canonical 722).
* `-fprofile-sample-use` always sets `function_entry_count`, so `LoopSink`
  runs and the body grows to 1440-1468 B; it is falsified.
* `switch (__builtin_expect(x, hot))` is the only C++11 source-level `!prof`
  producer; it moves one hot case to the front but leaves the rest numeric and
  gives 1392 B.
* `-mllvm -profile-guided-section-prefix` is **ON by default** in clang 7.0.2
  (`CodeGenPrepare.cpp:171`, `cl::init(true)`). Any function with profile info
  is renamed `.text.hot.<mangled>` and gold places it at the front of `.text`.
  A profile build must add `-mllvm -profile-guided-section-prefix=false` to
  keep the function at its TU-order address.

What is still open:

* The original's actual weight vector. No shipped `.profraw`/`.profdata`
  exists; the profile is not in the APK, OBB, IPA or source dumps.
* Whether the original used `-fprofile-instr-use`, `-fprofile-sample-use`, a
  source-level `__builtin_expect`, or an IR-level `!prof` injection.
* Why the original's `.text` rank order is exactly our non-profile build's
  (4,522/4,522). If a full profile was used, it must have been combined with
  `-mllvm -profile-guided-section-prefix=false`; if a per-function IR-level
  `!prof` injection was used, the rest of the TU is untouched and the rank
  order is preserved.

The wave-154 sample-profile signature (`Status::missionCompleted` flat vector
`default 1, {167,174,8,0,11,13,14}=4, group2=3, group3=2`, raw 558 / aligned
47 / 1,432 B) is the strongest current lead. It is a sample-profile-shaped
weight vector, but a real `-fprofile-sample-use` build adds
`function_entry_count` and grows the body, so the exact original mechanism is
still unresolved.

---

## 8. LTO status (disproven)

NDK r18b ships `LLVMgold.so`:

```text
.cache/ndk/android-ndk-r18b/toolchains/llvm/prebuilt/linux-x86_64/lib64/LLVMgold.so
.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/lib/bfd-plugins/LLVMgold.so
```

A `-flto` object is LLVM bitcode and links through gold with
`-plugin .../LLVMgold.so`:

```text
$ clang++ ... -flto -c src/engine/core/AERandom.cpp -o /dev/shm/w10120_aerandom_lto.o
$ file /dev/shm/w10120_aerandom_lto.o
/dev/shm/w10120_aerandom_lto.o: LLVM IR bitcode
$ ld.gold -shared -plugin .../LLVMgold.so -o /dev/shm/w10120_lto_test.so /dev/shm/w10120_aerandom_lto.o
# rc=0, ELF 32-bit ARM shared object
```

A full `-flto` build of the tree (all 192 TUs compiled with `-flto`, linked
with the plugin) gives:

```text
.text    0x12d404  (1,233,924 B)   original 0x1337c8 (1,259,464 B)
.rodata  0x40477   (263,287 B)     original 0x41b67  (269,159 B)
.data    0x295c    (10,588 B)      original 0x296c   (10,604 B)
```

The per-TU identity of 4,507 functions and the exact `.text` rank order also
rule out cross-TU LTO. **The original is not LTO.**

---

## 9. ICF status (disproven)

The original has **112 duplicate function bodies at distinct addresses**. The
largest examples:

| size | addresses | symbols |
|---:|---|---|
| 122 | `0x8073c`, `0x8060e` | `AEMath::MatrixTransformVector`, `AEMath::operator*(Matrix const&, Vector const&)` |
| 76 | `0x174a68`, `0x174a1c` | `Ship::getHandling`, `Ship::getHandlingForShop` |
| 50 | `0x73a64`, `0x73a08` | `AEMath::Vector::operator/=(Vector const&)`, `AEMath::Vector::operator*=(Vector const&)` |
| 38 | `0xa57a0`, `0x157d38` | `PlayerWormHole::getPosition`, `Waypoint::getPosition` |
| 28 | `0xdcb9a`, `0x153fb4` | `PlayerFighter::setPosition`, `PlayerFixedObject::setPosition` |

`--icf=all` would fold them and was already disproven (gate 1347). New this
pass: `--icf=safe` also shrinks `.text` from `0x1337c8` to `0x130964`
(1,247,588 B), so even the conservative ICF mode is ruled out. **ICF is off.**

---

## 10. Per-TU optimization level

Every one of the 192 app TUs is size-exact at `-Oz`; 176/192 are fully
byte-exact at `-Oz`. Per-TU `-O1/-O2/-Os/-O3` never helps the target and
always shifts `.text` (w4301, w5215). The original is a single `-Oz` build,
not a mixed-optimization build.

---

## 11. Unproven / unobservable

* Exact CMake version, generator, build directory and `CMakeLists.txt` text.
* Exact original host macOS version / Xcode command-line tools.
* `-g2` vs `-g3` (codegen-identical); DWARF version (stripped).
* `-fPIC` vs `-fpic` (object-identical); `-std=c++11` vs `gnu++11`.
* `-DANDROID`, `-DNDEBUG`, `-D__ANDROID_API__=16` macro spelling (codegen-neutral).
* `-fmerge-constants`, `-fno-plt`, `-mtune=*`, `-mfpu=neon-vfpv4` (neutral or
  attribute-changing but not authentic).
* `--strip-all` vs `--strip-unneeded` (byte-identical here).
* `--build-id=sha1` vs `--build-id=tree` (same 20-byte value on this file).
* `--fix-cortex-a8`, `--warn-shared-textrel`, `--fatal-warnings`,
  `--exclude-libs=libatomic.a`, `--exclude-libs=libunwind.a` (neutral).
* The exact PGO mode / weight vector (see §7).
* The original build-id `c011810d…` (one-way hash of an unstripped file that
  no longer exists; depends on `DW_AT_comp_dir`, debug info and every source
  byte).

---

## 12. Recovered build files (copies)

No original build file survives. The following are the **reconstruction**
files that reproduce the original's observable bytes; they are copied here so
this note is self-contained.

### 12.1 `cmake/toolchain.cmake` (repo reconstruction)

```cmake
set(_match_api 16)
set(CMAKE_CXX_FLAGS
    "-target armv7-none-linux-androideabi${_match_api} \
-march=armv7-a -mthumb -mfpu=neon -mfloat-abi=softfp \
-std=c++11 -fPIC -frtti -fstack-protector-strong ${GOF2_MATCH_OPT} -g \
-stdlib=libc++ \
-isystem ${GOF2_NDK_LIBCXX_INC} \
-isystem ${GOF2_NDK_LIBCXXABI_INC} \
-isystem ${GOF2_NDK_SUPPORT_INC} \
--sysroot=${GOF2_NDK_SYSROOT} \
-isystem ${GOF2_NDK_SYSROOT}/usr/include \
-isystem ${GOF2_NDK_SYSROOT}/usr/include/arm-linux-androideabi \
-D__ANDROID_API__=${_match_api} -DANDROID -DNDEBUG \
-Wno-int-to-pointer-cast -Wno-int-to-void-pointer-cast \
-I${_gof2_root}/src \
-I${_gof2_root}/third_party/fmod/inc \
-I${_gof2_root}/third_party/gl \
-I${_gof2_root}/third_party/jni \
-I${_gof2_root}/third_party/libzip \
-I${_gof2_root}/third_party/crypto"
    CACHE STRING "matching ARM compile flags" FORCE)
set(CMAKE_C_FLAGS
    "-target armv7-none-linux-androideabi${_match_api} \
-march=armv7-a -mthumb -mfpu=neon -mfloat-abi=softfp \
-fPIC -fstack-protector-strong ${GOF2_MATCH_OPT} -g \
-isystem ${GOF2_NDK_SUPPORT_INC} \
--sysroot=${GOF2_NDK_SYSROOT} \
-isystem ${GOF2_NDK_SYSROOT}/usr/include \
-isystem ${GOF2_NDK_SYSROOT}/usr/include/arm-linux-androideabi \
-D__ANDROID_API__=${_match_api} -DANDROID -DNDEBUG \
-Wno-int-to-pointer-cast -Wno-int-to-void-pointer-cast \
-I${_gof2_root}/src \
-I${_gof2_root}/third_party/fmod/inc \
-I${_gof2_root}/third_party/gl \
-I${_gof2_root}/third_party/jni \
-I${_gof2_root}/third_party/libzip \
-I${_gof2_root}/third_party/crypto"
    CACHE STRING "matching ARM C compile flags" FORCE)
```

### 12.2 `tools/source_order.txt` (repo reconstruction)

203 lines: 11 comment lines + 192 `src/...` entries. First source
`src/engine/audio/AESound.cpp`; last source
`src/engine/core/ArrayInstantiations.cpp`. The list is the original TU order;
gold emits symbols in input order, so it places every function at its original
address.

### 12.3 `CMakeLists.txt` link recipe (repo reconstruction)

```cmake
COMMAND ${GOF2_NDK_GOLD}
        -shared -soname libgof2hdaa.so --build-id
        -z now
        --exclude-libs=libgcc.a
        -o "${_so_unstripped}"
        ${_crtbegin} -L ${_libdir}
        ${_link_objs}
        --whole-archive ${_link_zlib} ${_link_libzip} --no-whole-archive
        -l log -l GLESv2 -l GLESv1_CM -l EGL -l android
        ${GOF2_FMOD_LIBDIR}/libfmodex.so ${GOF2_FMOD_LIBDIR}/libfmodevent.so
        ${_link_openssl}
        -l m
        ${_link_libcxxabi}
        ${_link_aos}
        ${_link_libgcc}
        -l dl -l c
        ${_libdir}/crtend_so.o
COMMAND ${GOF2_NDK_STRIP} --strip-all -o "${_so}" "${_so_unstripped}"
COMMAND ${CMAKE_COMMAND} -E env python3
        "${CMAKE_SOURCE_DIR}/tools/reorder_shstrtab.py"
        "${_so_unstripped}" "${_so}"
```

### 12.4 Historical scratch build `_bg/compile_commands.json` (not original)

199 entries, API 21, `-mllvm -pre-RA-sched=source`, section-ordering file.
Example command:

```text
clang++ -target armv7-none-linux-androideabi21 -march=armv7-a -mthumb
  -mfpu=neon -mfloat-abi=softfp -std=c++11 -fPIC -frtti
  -fstack-protector-strong -Oz -mllvm -pre-RA-sched=source -g -stdlib=libc++
  -isystem .../llvm-libc++/include -isystem .../llvm-libc++abi/include
  -isystem .../android/support/include --sysroot=.../sysroot
  -isystem .../sysroot/usr/include
  -isystem .../sysroot/usr/include/arm-linux-androideabi
  -D__ANDROID_API__=21 -Wno-int-to-pointer-cast -Wno-int-to-void-pointer-cast
  -I.../src -I.../third_party/fmod/inc -I.../third_party/gl
  -I.../third_party/jni -I.../third_party/libzip -I.../third_party/crypto
  -ffunction-sections -o CMakeFiles/gof2.dir/src/engine/core/AERandom.cpp.obj
  -c .../src/engine/core/AERandom.cpp
```

This is an old reconstruction, not the original build: it uses API 21,
`-pre-RA-sched=source`, and a `--section-ordering-file`.

### 12.5 Historical mac scratch build `_cmake-build-match.mac/compile_commands.json` (not original)

201 entries, darwin host, API 21, no `-g`. Example command:

```text
/Users/fionera/.../darwin-x86_64/bin/clang++ -target armv7-none-linux-androideabi21
  -march=armv7-a -mthumb -mfpu=neon -mfloat-abi=softfp -fPIC -frtti
  -fstack-protector-strong -Oz -stdlib=libc++
  -isystem .../llvm-libc++/include -isystem .../llvm-libc++abi/include
  -isystem .../android/support/include --sysroot=.../sysroot
  -isystem .../sysroot/usr/include
  -isystem .../sysroot/usr/include/arm-linux-androideabi
  -D__ANDROID_API__=21 -Wno-int-to-pointer-cast -Wno-int-to-void-pointer-cast
  -I.../src -I.../third_party/fmod/inc -I.../third_party/gl
  -I.../third_party/jni -I.../third_party/libzip -I.../third_party/crypto
  -ffunction-sections -o CMakeFiles/gof2.dir/src/engine/core/AERandom.cpp.obj
  -c .../src/engine/core/AERandom.cpp
```

This is an old reconstruction, not the original build.

### 12.6 Vendored archive build flags (repo reconstruction)

From `third_party/build_match_lib.cmake`:

```text
zlib 1.2.3:   clang -Oz -g -fstack-protector-strong -I. -mthumb
              --target=armv7a-linux-androideabi16 -mfpu=neon -mfloat-abi=softfp
              --sysroot=<ndk>/platforms/android-16/arch-arm
              -fPIC -ffunction-sections -funwind-tables
              member order: inflate compress deflate crc32 infback zutil
                            inftrees uncompr trees inffast adler32
libzip 0.9.3: same clang flags; compiled against NDK r18b sysroot zlib.h 1.2.7;
              HAVE_MKSTEMP and HAVE_STRUCT_TM_TM_ZONE set
libcrypto:    real GCC 4.9 from NDK r13b; OpenSSL 1.1.0l; Configure android-armeabi
              + -march=armv7-a; -O3 -pthread -mandroid -fPIC -Wa,--noexecstack
              -D__ANDROID_API__=16 -DOPENSSL_PIC -DSHA256_ASM
```

---

## 13. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10120
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j32

# section deltas
python3 - <<'PY'
import subprocess,re
O='/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so'
U='cmake-build-match/libgof2hdaa.so'
def secs(p):
    out=subprocess.check_output(['readelf','-SW',p],text=True); d={}
    for line in out.splitlines():
        m=re.match(r'\s*\[\s*\d+\]\s+(\S+)\s+\S+\s+([0-9a-f]+)\s+([0-9a-f]+)\s+([0-9a-f]+)',line)
        if m: d[m.group(1)]=(int(m.group(3),16),int(m.group(4),16))
    return d
o=secs(O); u=secs(U)
for n in o:
    if n in u:
        oo,os_=o[n]; uo,us=u[n]
        a=open(O,'rb').read()[oo:oo+os_]; b=open(U,'rb').read()[uo:uo+us]
        print(n, os_, us, sum(x!=y for x,y in zip(a,b)) if os_==us else 'size')
PY

# ICF duplicate-body census
python3 - <<'PY'
import subprocess, collections
NM='.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm'
O='/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so'
out=subprocess.check_output([NM,'-D','-S','--defined-only',O],text=True)
funcs=[]
for line in out.splitlines():
    p=line.split()
    if len(p)>=4 and p[2] in ('T','t','W','w'):
        try: a=int(p[0],16); s=int(p[1],16)
        except: continue
        if s: funcs.append((a,s,p[3]))
r=subprocess.check_output(['readelf','-SW',O],text=True)
for line in r.splitlines():
    if ' .text ' in line:
        q=line.split(); i=q.index('.text'); ta=int(q[i+2],16); to=int(q[i+3],16); ts=int(q[i+4],16)
data=open(O,'rb').read()[to:to+ts]
g=collections.defaultdict(list)
for a,s,n in funcs:
    if a>=ta and a+s<=ta+ts: g[(s,data[a-ta:a-ta+s])].append((a,n))
d=[(k,v) for k,v in g.items() if len(set(a for a,_ in v))>1]
print('duplicate bodies with distinct addresses:',len(d))
PY

# LTO A/B (plugin)
#   compile all TUs with -flto, link with
#   -plugin .cache/ndk/android-ndk-r18b/toolchains/llvm/prebuilt/linux-x86_64/lib64/LLVMgold.so
# ICF A/B
#   add --icf=safe to the gold command; .text 0x1337c8 -> 0x130964
```

Scratch (not committed): `/dev/shm/w10120/` — `lto/`, `lto.so`,
`lto.unstripped.so`, `icf_safe.so`, `icf_safe.unstripped.so`,
`aerandom_lto.o`, `lto_test.so`, `align_diff.py`, `fdiff.py`,
`classes.dex`, `apk/`.

---

## 14. Files touched

* `_work/reconstructions/build-archaeology-wave155-w10120.md` (this note).
* No source, tool, CMake, baseline, allowlist or doc change.
