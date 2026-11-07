# Build-flag synthesis, wave 127 (w7601)

Status: synthesis note, 2026-11-07. Worktree `/home/debian/GalaxyOnFire2/wt/w7601`,
branch `wave126-w7601`, base `bf365a964` ("MGame::OnTouchEnd pass: campaign temp +
hudMenu else-if chain"). Scratch is under `/dev/shm/w7601` and the wave-126 agent
directories `/dev/shm/w7602` .. `/dev/shm/w7614`.

**This is a note-only commit. No `src/`, `tools/`, `CMakeLists.txt`, baseline,
allowlist or doc change is landed.**

Gate at the base (unchanged; `report.json`):

```text
verify: byte 4506/4524  linked 4506/4524  size 4524/4524  avg 99.94
data:   exact 843/843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0  imports 207
symbol parity 5370/5370 (0 extra / 0 missing)
lint CLEAN ✓   drift 0
```

Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`,
2,187,328 B, `sha256 d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483`.

This note synthesises the eight prior build-flag notes named in the assignment plus the
scratch JSON/logs of the wave-126 agents. It does not re-run any heavy build; the only
new measurement is the section-by-section diff of the existing `cmake-build-match`
output at HEAD (see §7).

---

## 0. Bottom line

1. **The current flags are correct.** The effective compile line is the stock NDK r18b
   CMake toolchain default set (Release, `ANDROID_ARM_NEON=ON`,
   `ANDROID_CPP_FEATURES="rtti exceptions"`); the effective link line is the repo's
   custom trimmed gold link. Every load-bearing flag has a controlled A/B behind it,
   and the stock-CMake reproduction is byte-identical to the canonical build except
   the 20-byte build-id.
2. **The original was built with CMake, not ndk-build.** Two independent binary
   discriminators settle it: **no `-fdata-sections`** and **FMOD before the STL
   archives** (plus exceptions+RTTI on and the `libm libdl libc` `DT_NEEDED` tail).
   Vanilla ndk-build defaults score 1345/3846/4523 with 189 imports; a heavily coerced
   ndk-build reaches 2404/4504/4523 with 207 imports but still misses the exact
   `.text`.
3. **No flag is missing.** The 18 residual functions are source shape, not build
   configuration. Across the wave-126 scratch (w7602–w7614) **no variant raises any
   residual function's official percentage**; the best sub-rounding movement is still
   the wave-123 `-mllvm -tail-dup-placement-penalty=0` on `MGame::OnTouchEnd` (−1 diff
   byte) and `-mllvm -likely-branch-weight=500` on `Radar::draw` (−2 diff bytes), both
   below the 0.1 % official precision.
4. **The matching knobs are not original flags.** Per-file `-mllvm` branch weights on
   `StarMap.cpp`/`SpaceLounge.cpp`, `__builtin_expect` hints, and the
   `-mllvm -align-all-functions=2` on `zip_file_strerror` are deliberate
   reconstruction aids. They are documented as such in `CMakeLists.txt` and
   `third_party/match_libs.cmake`.

---

## 1. Definitive recipe

### 1.1 Toolchain

| item | value | evidence |
|---|---|---|
| NDK | **r18b**, `Pkg.Revision 18.1.5063045` | `.note.android.ident` byte-identical to r18b API-16 `crtbegin_so.o`; `DownloadNDK.cmake` |
| C/C++ compiler | **clang 7.0.2** (`Android (4751641 based on r328903)`) | `.comment` byte-identical |
| linker | **gold 1.12** | `.note.gnu.gold-version` byte-identical |
| strip | GNU `arm-linux-androideabi-strip` (binutils 2.27 in r18b) | see §1.6 |
| target | `armv7-none-linux-androideabi16` / `armeabi-v7a`, **API 16** | `.note.android.ident` first word = 0x10; `.ARM.attributes` |
| ISA | **Thumb-2**, `-march=armv7-a`, VFPv3 + NEONv1, **softfp** | `.ARM.attributes` (`Tag_THUMB_ISA_use=Thumb-2`, `Tag_FP_arch=VFPv3`, `Tag_Advanced_SIMD_arch=NEONv1`), `e_flags 0x5000200` |
| C++ runtime | **static libc++**: `libc++abi.a` + `libandroid_support.a` + `libgcc.a` | imports/`DT_NEEDED`; no `libc++_static.a` members |
| host package | **macOS/darwin NDK package** | the only host path in the binary is `/Volumes/Android/buildbot/.../libcxxabi/src/abort_message.cpp`; the linux `libc++abi.a` drops byte_exact to 4344 |

### 1.2 Host

The compile host is not directly observable. The darwin NDK package is selected because
the only surviving source path string is a `/Volumes/Android/buildbot/...` macOS path.
The link timestamp witness is the Crashlytics `2018-10-22 10:18:17 CEST` record. CMake
version and generator (Ninja vs Make) are codegen-neutral and unobservable.

### 1.3 Build system

**CMake** with the stock r18b `android.toolchain.cmake` defaults, plus three settings:

```text
ANDROID_ARM_NEON=ON
ANDROID_CPP_FEATURES="rtti exceptions"
CMAKE_BUILD_TYPE=Release
```

The repo's own `cmake/toolchain.cmake` is a hand-written equivalent of that default set
(it spells the flags out so the build is reproducible without the NDK's CMake module).
The build-system *file* is unobservable from the stripped ELF; the effective flags and
link order are proven. See §5.

### 1.4 Compile flags (C++)

```text
-target armv7-none-linux-androideabi16
-march=armv7-a -mthumb -mfpu=neon -mfloat-abi=softfp
-std=c++11 -fPIC -frtti
-fstack-protector-strong
-Oz -g
-stdlib=libc++
-isystem <ndk>/sources/cxx-stl/llvm-libc++/include
-isystem <ndk>/sources/cxx-stl/llvm-libc++/libcxxabi/include
-isystem <ndk>/sources/android/support/include
--sysroot=<ndk>/sysroot
-isystem <sysroot>/usr/include
-isystem <sysroot>/usr/include/arm-linux-androideabi
-D__ANDROID_API__=16 -DANDROID -DNDEBUG
-Wno-int-to-pointer-cast -Wno-int-to-void-pointer-cast
-I src -I third_party/fmod/inc -I third_party/gl -I third_party/jni
-I third_party/libzip -I third_party/crypto
-ffunction-sections -funwind-tables
```

C TUs use the same set minus `-std=c++11 -frtti -stdlib=libc++` and the libc++ include
paths.

**No `-fdata-sections`.** This is the single most important negative flag.

### 1.5 Link flags (gold, single plain link, no `-s`)

```text
gold -shared -soname libgof2hdaa.so --build-id -z now --exclude-libs=libgcc.a
     -o libgof2hdaa.unstripped.so
     <libdir>/crtbegin_so.o -L <libdir>
     <192 objects in tools/source_order.txt order>
     --whole-archive libz_gof2.a libzip_gof2.a --no-whole-archive
     -l log -l GLESv2 -l GLESv1_CM -l EGL -l android
     <fmod>/libfmodex.so <fmod>/libfmodevent.so
     libcrypto_gof2.a
     -l m
     libc++abi.a
     libandroid_support.a
     libgcc.a
     -l dl -l c
     <libdir>/crtend_so.o
```

No `--version-script`, no `--gc-sections`, no `--icf`, no `--section-ordering-file`, no
second pass, no `--allow-multiple-definition`, no `--unresolved-symbols` switch.

Gold defaults that the original also exhibits (not spelled on our line, but proven by
the output): `--hash-style=sysv`, `--no-as-needed`, `-z combreloc`,
`-z max-page-size=4096`, `-z common-page-size=4096`, `--spare-dynamic-tags=5`,
`-z relro`, `-z noexecstack`.

### 1.6 Strip / packaging

```text
arm-linux-androideabi-strip --strip-all -o libgof2hdaa.so libgof2hdaa.unstripped.so
python3 tools/reorder_shstrtab.py libgof2hdaa.unstripped.so libgof2hdaa.so
```

The original is **gold without `-s`, then GNU strip, then the `.shstrtab` reorder**:

* GNU `strip --strip-all` differs from the unstripped gold output in exactly
  `.shstrtab` (259 bytes) + the 29 `sh_name` fields = **288 bytes** (`w7609`).
* `gold -s` differs from GNU-stripped in the `.rel.plt` header
  (`SHF_INFO_LINK`/`sh_info=20` vs `0`), the `.init_array`/`.fini_array`
  `sh_entsize` (4 vs 0), and the `.bss` `sh_offset` (0x21596c vs 0x215970), plus the
  build-id. `gold -s` also hashes the *stripped* file, so it can never reproduce the
  original build-id.
* `objcopy --strip-all` normalises the headers the same way but its build-id differs
  (bfd strip hashes the stripped file; GNU strip preserves the gold build-id from the
  unstripped link).
* `strip --strip-debug` leaves `.symtab`/`.strtab` and is wrong.

Packaging: the APK is an AGP 3.1.2 release build (2018-10-22). The APK's
`lib/armeabi-v7a/libgof2hdaa.so` and the ground-truth binary are the same build; the
288-byte difference is only the gold-vs-bfd `.shstrtab` order, and re-running
`tools/reorder_shstrtab.py` on the APK lib reproduces the ground-truth sha256 exactly.

---

## 2. Flag-by-flag table

Legend: **P** = proven by a controlled A/B or a byte-identical artifact; **D** = proven
effective but it is a tool default (not spelled on the command line); **N** = proven
neutral / not distinguishable on this source; **U** = unobservable from the ELF;
**G** = still guessed (plausible but no positive evidence).

### 2.1 Toolchain / ABI

| flag / property | status | evidence |
|---|---|---|
| NDK r18b | **P** | `.note.android.ident` byte-identical to r18b API-16 `crtbegin_so.o` |
| clang 7.0.2 | **P** | `.comment` byte-identical |
| gold 1.12 | **P** | `.note.gnu.gold-version` byte-identical |
| API 16 | **P** | `.note.android.ident` first word 0x10; `__ANDROID_API__=16` |
| armeabi-v7a / Thumb-2 / VFPv3 / NEONv1 / softfp | **P** | `.ARM.attributes`, `e_flags 0x5000200` |
| darwin NDK package (host) | **P** (strong) | `/Volumes/Android/buildbot/.../abort_message.cpp`; linux `libc++abi.a` → 4344 byte_exact |
| exact CMake version / generator | **U** | codegen-neutral |
| exact build host OS/CPU | **U** | only the darwin path string survives |
| link timestamp | **U** | Crashlytics `2018-10-22 10:18:17 CEST` is the closest witness |

### 2.2 Compile flags

| flag | status | evidence |
|---|---|---|
| `-Oz` | **P** | unique best: O1 1033, O2 1168, Os 1263 byte_exact; `w7602` shows every other `-O*` changes 3 target functions and 49–128 exact functions, while `-Oz` changes 0 |
| `-g` (or `-g2`/`-g3`/`-ggdb`/`-gdwarf-2..5`) | **P** | `w7319`: g0/g1/gline-tables-only → 2071 byte / 4247 linked / 4485 size; g2/g3/gdwarf-2..5 → 4506/4506/4524. Debug info pins PostRA machine sinking at `-Oz` (DBG_VALUE users) |
| `-fPIC` (vs `-fpic`) | **N** | byte-identical on this source; `w7613` `fpic` = base; `-fPIC` is CMake's spelling and the conservative choice |
| `-frtti` | **P** | `-fno-rtti` → 1701 byte_exact / 4506 linked (2823 functions change); typeinfo removed from `.rodata`/`.data.rel.ro` |
| exceptions on | **P** | `Level.cpp` has try/catch; `-fno-exceptions` fails to compile |
| `-fstack-protector-strong` | **P** | `-fno-stack-protector` 1369/3783; `-fstack-protector` 1373; `-fstack-protector-all` 83/968; `w7608` confirms `-fstack-protector` changes all 13 target TUs and `-all` changes 5 targets/537 exact |
| `-ffunction-sections` | **P** | proper removal 4497 → 1345; the original's `.ARM.exidx` has 194 empty-`.text` CANTUNWIND sentinels |
| `-funwind-tables` | **P** | proper removal 4497 → 2878; the earlier "neutral" result was a flag-order artifact |
| **no `-fdata-sections`** | **P** | adding it costs 186–192 byte_exact (`w5202` controlled A/B: 4505 → 4314; wave-123 `w7320`: 4506 → 4315; wave-119 `w6719`: 4500 → 4314/4501) and changes `.rodata` 113,935 B, `.data` 3,023 B, `.data.rel.ro` 365 B; the original's data sections are byte-identical to the no-`-fdata-sections` build |
| `-std=c++11` | **P** | the original guards signed `new T[n]` counts (`orr rX,n,n,asr #31`), which clang 7 emits for C++98/C++11 but not its gnu++14 default; `-std=c++14`/`gnu++14` → 1512 |
| `-std=gnu++11` | **N** | byte-identical to `-std=c++11` |
| `-DANDROID` | **N** | codegen-neutral (4498 both with and without) |
| `-DNDEBUG` | **N** | codegen-neutral |
| `-march=armv7-a` | **N** | `w7611`: 0 changed functions; `armv7ve` changes 31 functions and adds `Tag_DIV_use`/`Tag_Virtualization_use` |
| `-mthumb` | **N** | `w7611`: 0 changed; `-marm` changes code |
| `-mfpu=neon` | **P** | `.ARM.attributes` requires VFPv3+NEONv1; `vfpv3-d16`/`vfpv3`/`vfpv4`/`fp-armv8` change code |
| `-mfloat-abi=softfp` | **P** | `soft`/`hard` change code; `e_flags` |
| `-mtune=*` | **N** | `w7611`: 0 changed on the 13 target TUs |
| `-mcpu=cortex-a8/a9/a15` | **N** (but changes code) | `w7611`: cortex-a8 changes 11 targets, a9 18, a15 — all negative (adds `Tag_CPU_name` and different scheduling) |
| `-mno-movt`, `-mno-restrict-it` | **N** | `w7608`/`w7611`: 0 changed |
| `-mrestrict-it` | **N** (changes code) | `w7611` |
| `-mno-unaligned-access` | **N** (negative) | `w7608`: 2 targets / 26 exact change |
| `-fomit-frame-pointer` | **N** (negative) | 1298/1920 full-link; `w7608`: 18 targets / 422 exact change; frame pointers are kept |
| `-fno-omit-frame-pointer` | **N** | `w7608`: 0 changed |
| `-fno-jump-tables` | **N** (negative) | `w7608`: 9 targets / 24 exact change |
| `-fno-optimize-sibling-calls` | **N** (negative) | `w7608`: 3 targets / 90 exact change |
| `-fno-builtin` | **N** (negative) | `w7608`: 6 targets / 124 exact change |
| `-fno-strict-aliasing` | **N** (negative) | `w7608`: 7 targets / 38 exact change |
| `-fno-strict-overflow` / `-fwrapv` | **N** (negative) | `w7608`: 2 targets / 17 exact change |
| `-fno-slp-vectorize` | **N** (negative) | `w7608`: 7 targets / 18 exact change; vectorization is on |
| `-fno-vectorize` | **N** | `w7608`: 0 changed |
| `-fno-unroll-loops` | **N** (negative) | `w7608`: 1 target / 1 exact change |
| `-fno-ipa-cp` | **N** | `w7608`: 0 changed |
| `-fno-common` | **N** (link failure full-link) | `w7608`/`w7613`: 0 object change; wave-123 full-link `-fno-common` is the one real link failure |
| `-fno-zero-initialized-in-bss` | **N** | `w7608`: 0 targets / 3 exact change |
| `-fno-inline-functions-called-once` | **N** | `w7608`: 0 changed |
| `-fno-delete-null-pointer-checks` | **N** | `w7608`: 0 changed |
| `-fno-threadsafe-statics` | **N** (negative full-link) | `w7608`/`w7613`: 0 target-TU change; full-link 1368; threadsafe statics are on |
| `-fno-use-cxa-atexit` | **N** (negative full-link) | `w7608`/`w7613`: 0 target-TU change; full-link 1356 |
| `-fno-sized-deallocation` / `-fno-aligned-allocation` | **N** | `w7608`: 0 changed |
| `-fno-plt` | **N** | `w7608`/`w7613`: 0 changed |
| `-fno-semantic-interposition` | **U** (unknown arg) | `w7613`: clang 7.0.2 rejects `-fno-semantic-interposition` |
| `-fno-merge-constants` / `-fno-merge-all-constants` | **N** | `w7608`/`w7613`: 0 changed |
| `-fno-const-strings` / `-fno-weak` | **U** (unknown arg) | `w7613`: clang 7.0.2 rejects both as unknown arguments |
| `-fno-use-init-array` | **N** | `w7613`: 0 changed |
| `-mno-global-merge` | **N** | `w7613`: 0 changed |
| `-fvisibility=default` | **N** | `w7613`: 0 changed |
| `-fvisibility=hidden` / `protected` | **N** (negative) | `w7613`: 3 targets / 25 exact change |
| `-fvisibility-inlines-hidden` | **N** | `w7613`: 0 targets / 2 exact change |
| `-fPIE` / `-fpie` | **N** (negative) | `w7613`: 3 targets / 25 exact change |
| `-fnopic` | **N** (negative) | `w7613`: all 13 TUs / 18 targets / 264 exact change |
| `-mno-pic-data-is-text-relative`, `-mpic-register`, `-msingle-pic-base` | **U** (unknown arg) | `w7613`: clang 7.0.2 rejects all three as unknown arguments |
| `-fshort-wchar` | **N** | neutral in the linked `.so` |
| `-ffreestanding` | **N** | not a discriminator |
| `-fno-exceptions` | **P** (wrong) | fails on `Level.cpp`; original has EH/LSDA |
| `-fno-rtti` | **P** (wrong) | 1701/4506; typeinfo removed |

### 2.3 Preprocessor / macros (`w7606`, 170 variants)

Only these change any TU at all: `FORTIFY_1/2`, API level, `FILE_OFFSET_BITS_64`,
`USE_FILE_OFFSET64`, `BIONIC_LONG_DOUBLE`, `STDC_WANT_LIB_EXT1`, `HAVE_INTTYPES_H`,
`GOF2_MATCH`, `LP64`, `WIN32`, `_WIN32`, `DLL_EXPORTS`, `LIBCPP_DEBUG*`,
`GNU_SOURCE_UNDEF`. `no_NDEBUG`, `no_ANDROID`, `DDEBUG`, `D_DEBUG`, `FORTIFY_0` change
nothing. None of the changing macros is a plausible original macro — they are test
macros. `-DANDROID`/`-DNDEBUG` remain the CMake Release defaults and are
codegen-neutral, so they are build-fidelity, not matching knobs.

### 2.4 Link flags

| flag | status | evidence |
|---|---|---|
| `-z now` | **P** | without it 4497 → 2932; the original is fully RELRO-bound (`DT_FLAGS=BIND_NOW`, `DT_FLAGS_1=NOW`, `.got` before `.data` inside `PT_GNU_RELRO`) |
| `--build-id` (SHA-1) | **P** | md5/none/uuid worse; `buildid_0x` differs from base only in the build-id |
| `--exclude-libs=libgcc.a` | **P** | hides libgcc symbols as in the original |
| zlib/libzip `--whole-archive` | **P** | the original exports their full symbol set |
| FMOD before `libc++abi.a` | **P** | `_Unwind_*` stay UND (resolved from FMOD); `libgcc.a`'s `libunwind.o` is not pulled; `__gnu_Unwind_Find_exidx` absent |
| `libgcc.a` on the line | **P** | pulls `_aeabi_ldivmod.o`/`_dvmd_lnx.o`; leaves `__gnu_ldivmod_helper`/`raise@LIBC` UND |
| omit `libc++_static.a`/`libunwind.a` | **P** | no `libc++_static.a` members in the original |
| `--hash-style=sysv` | **D** | `.hash` only, no `.gnu.hash`; `gnu`/`both` add `.gnu.hash` and change dynsym/dynstr/rel.plt |
| `--no-as-needed` | **D** | all `DT_NEEDED` entries present; `--as-needed` drops `libGLESv1_CM`, `libEGL`, `libandroid`, `libdl` |
| `-z combreloc` | **D** | `DT_RELCOUNT=1162`; `-z nocombreloc` changes `.rel.dyn` 17,753 B, dynsym 305, `.text` 101,800 |
| `-z max-page-size=4096` | **D** | 65536 → 2932 byte_exact |
| `-z common-page-size=4096` | **D** | neutral |
| `--spare-dynamic-tags=5` | **D** | 0/10 → 4486; 5 → 4497 |
| `-z relro` | **D** | `w7610`: `-z relro` produces the same sha as base; `-z norelro` differs |
| `-z noexecstack` | **D** | neutral |
| `--fix-cortex-a8` | **N** | `w7604`: identical sha to base |
| `--no-undefined` | **N** | neutral |
| `--warn-shared-textrel --fatal-warnings` | **N** | neutral |
| `--sort-section=none` | **N** | `w7604`: identical sha to base |
| `--merge-exidx-entries` | **N** | neutral |
| `--no-text-reorder` | **N** (negative) | `w7604`: different sha |
| `--threads` / `--no-threads` | **N** | neutral |
| `--no-keep-memory` | **N** | neutral |
| `--discard-locals` / `--discard-all` | **N** | neutral |
| gold `-O0`/`-O1` | **N** | neutral |
| `--gc-sections` | **N** (negative) | 2176 |
| `--icf=all` / `--icf=safe` | **N** (negative) | 1347 / worse |
| `--sort-section=name` | **N** (negative) | 1342 |
| `--no-merge-exidx-entries` | **N** (negative) | 2878 |
| `--rosegment` | **N** (negative) | 1720 |
| gold `-O2`/`-O3` | **N** (negative) | 1720 |
| `--version-script` / section-ordering file | **P** (absent) | the original `.text` is plain object input order |
| `-s` on the gold line | **P** (absent) | see §1.6 |

### 2.5 Strip / packaging

| step | status | evidence |
|---|---|---|
| gold **without** `-s` | **P** | build-id is the SHA-1 of the *unstripped* link output |
| GNU `strip --strip-all` | **P** | bfd header normalisation: `.rel.plt` `SHF_INFO_LINK`/`sh_info=20`, `INIT/FINI_ARRAY` `sh_entsize=4`, unaligned NOBITS `.bss` `sh_offset` |
| `tools/reorder_shstrtab.py` | **P** | restores gold's Stringpool `.shstrtab` order; the APK lib + reorder reproduces the ground-truth sha256 |
| `strip --strip-debug` | **P** (wrong) | leaves `.symtab`/`.strtab` |
| `objcopy --strip-all` | **P** (wrong build-id) | headers match, build-id differs |
| APK packaging (AGP 3.1.2, 2018-10-22) | **P** | APK metadata |

---

## 3. Are the current flags correct? Which are matching knobs?

**Yes.** The current `cmake/toolchain.cmake` + `CMakeLists.txt` link line reproduce the
original's effective compile and link recipe. The evidence is not just "the gate is
green": an independent stock-CMake build (stock r18b `android.toolchain.cmake`, Release,
NEON, RTTI+exceptions) produces **0 differing function blobs and 0 symbol-set
differences** across all 192 TUs and, relinked with the full CMake/ndk-build default
link flag set, the **same `.so` SHA-256** as the canonical build except the 20-byte
build-id.

There are three categories to keep separate:

### 3.1 Original effective flags (must stay)

`-Oz`, `-g`, `-fPIC`, `-frtti`, exceptions, `-fstack-protector-strong`,
`-ffunction-sections`, `-funwind-tables`, **no `-fdata-sections`**, `-std=c++11`,
`-march=armv7-a -mthumb -mfpu=neon -mfloat-abi=softfp`, static libc++,
`-z now`, `--build-id` SHA-1, `--exclude-libs=libgcc.a`, whole-archive zlib/libzip,
FMOD-before-STL, `libgcc.a`, GNU strip + shstrtab reorder, and the gold defaults in
§2.4.

### 3.2 Matching knobs (not original flags)

These are deliberate reconstruction aids. They are allowed, but they are not evidence
about the original build:

| knob | where | why |
|---|---|---|
| `-mllvm -likely-branch-weight=7 -mllvm -unlikely-branch-weight=1` on `StarMap.cpp` | `CMakeLists.txt` (per-file, `GOF2_NO_PER_FILE_WEIGHTS` option) | reproduces the original 7:1 block frequency for the campaign-target arm |
| same 7:1 on `SpaceLounge.cpp` | `CMakeLists.txt` | reproduces the EH landing-pad order; 95.5 → 95.7 official |
| `__builtin_expect` hints in `Radar.cpp`, `SpaceLounge.cpp`, `Status.cpp`, etc. | source | reproduce branch weights at the default 2000:1 |
| `-mllvm -align-all-functions=2` on `zip_file_strerror` | `third_party/match_libs.cmake` | reproduces the original function alignment in the vendored zip archive |

`Radar.cpp`'s old 80:1 and `MenuTouchWindow.cpp`'s old 5:1 per-file overrides were
removed (wave 100 / wave 92) because the source hints reproduce them at the default
weight.

### 3.3 Unobservable / still guessed

* The exact build-system file (`CMakeLists.txt` vs `Android.mk` vs a hand-written
  script). The effective recipe is proven; the file is not.
* The exact CMake version and generator.
* The build host OS/CPU (darwin package is a strong inference, not a direct proof).
* Whether `-DANDROID`/`-DNDEBUG` were explicit or implied (codegen-neutral).
* The exact link-line text (only the effective order is recoverable).
* The native link timestamp.

Nothing in this category can change a residual function: every candidate was tested and
is either neutral or negative.

---

## 4. Positive flag hits from the 14 agents

**There are none.** No wave-126 agent found a flag that raises any residual function's
official percentage or makes a residual body byte-exact.

The wave-126 scratch directories and what they tested (`w7601` is this synthesis;
`w7600` is empty; `w7607`/`w7610`/`w7612` were target agents that also ran small
flag/link matrices):

| dir | agent | what it tested | result |
|---|---|---|---|
| `w7602` | opt/debug screen | `-O*`, `-g*`, `-gdwarf-*`, `-gline-tables-only` on `Level.cpp` | `-Oz` and `-g`/`-g2`/`-g3`/`-ggdb`/`-gdwarf-2..5` are the only 0-target-change variants; `-g0`/`-g1`/`-gline-tables-only` change 1 target / 8 exact; every other `-O*` changes 3 targets / 49–128 exact |
| `w7603` | `-mllvm` screen | 167 `-mllvm` candidates × 13 TUs | only 34 change any target; all are alignment/scan-limit/constant-island knobs; no score win recorded |
| `w7604` | link audit | 19 gold link variants | `full_ndk_defaults`, `fix_cortex`, `no_fix_cortex`, `icf_none`, `sort_none`, `text_reorder`, `buildid_sha1`, `hash_sysv` all produce the **identical sha** to base; `buildid_0x` differs only in build-id; `gc`/`icf`/`sort_name`/`no_text_reorder`/`buildid_none`/`md5`/`uuid` differ; `sort_alignment` is a gold error |
| `w7605` | LLVM release archaeology | 100 LLVM GitHub releases | no relevant flag provenance |
| `w7606` | preprocessor sweep | 170 macro variants | only the test macros in §2.3 change any TU; none is a plausible original macro |
| `w7607` | `MGame::OnTouchEnd` target | source pass | not a flag agent (landed 78.8 → 81.3 %) |
| `w7608` | simple per-TU matrix | 28 flags × 13 TUs = 364 rows | only the expected negative knobs change targets (table in §2.2); no positive hit |
| `w7609` | strip audit | gold `-s` vs GNU strip vs objcopy vs strip-debug | confirms the current strip recipe; no flag change |
| `w7610` | `Gun::calcCharacterCollision` + link matrix | `-z now`/`-z relro`/`-z norelro` | `now_removed` and `relro` produce the same sha as base; `norelro` differs; no positive hit |
| `w7611` | ARM audit | 34 ARM/ABI variants × 13 TUs | `-march=armv7-a`, `-mthumb`, `-mfpu=neon`, `-mfloat-abi=softfp`, `-mtune=*`, `-mno-movt`, `-mno-restrict-it` all neutral; everything else changes code and is negative; `.ARM.attributes` pins VFPv3+NEONv1 |
| `w7612` | `HangarWindow::render` + debug/opt matrix | `-g0/g1/g2` | confirms the wave-123 result: g0/g1 → 2071/4247/4485; g2 → 4506/4506/4524 |
| `w7613` | PIC/visibility object sweep | 23 variants × 13 TUs | `-fpic`, `-fno-plt`, `-fno-common`, `-mno-global-merge`, `-fno-merge-constants`, `-fno-merge-all-constants`, `-fno-use-init-array`, `-fno-use-cxa-atexit`, `-fno-threadsafe-statics`, `-fvisibility=default` are neutral; `-fnopic` changes all 13; `-fPIE`/`-fpie`/`-fvisibility=hidden/protected` change 7; `-fvisibility-inlines-hidden` changes 1; `-fno-const-strings`, `-fno-weak`, `-fno-semantic-interposition`, `-mno-pic-data-is-text-relative`, `-mpic-register`, `-msingle-pic-base` are unknown arguments in clang 7.0.2 |
| `w7614` | baseline section audit | 27 sections | all headers exact; only `.text` (85,552), `.ARM.extab` (1,438) and `.note.gnu.build-id` (20) differ |

The closest sub-rounding movements remain the wave-123 ones (reproduced here for
completeness; none crosses the 0.1 % official precision):

| TU | flag | target | diff bytes | official % |
|---|---|---|---|---|
| `MGame.cpp` | `-mllvm -tail-dup-placement-penalty=0` | `MGame::OnTouchEnd` | 5550 → 5549 (−1) | 81.7 unchanged |
| `Radar.cpp` | `-mllvm -likely-branch-weight=500` | `Radar::draw` | 8028 → 8026 (−2) | 74.5 unchanged |
| `HangarWindow.cpp` | `-mllvm -disable-postra-machine-licm` | `HangarWindow::render` | 2071 → 2089 (+18) | 94.0 unchanged |

Wave-123's global sweep (258 full-link variants + 140 per-TU cells) found **zero**
positive target deltas and a maximum target delta of exactly **+0.0**. Wave-126 did not
overturn that.

**Conclusion: the residual 18 functions are source shape, not a missing or mis-set
flag.**

---

## 5. CMake vs ndk-build: the contradiction resolved

### 5.1 The two default sets (NDK r18b)

| item | CMake `android.toolchain.cmake` | ndk-build `build/core` |
|---|---|---|
| `-fdata-sections` | **no** | **yes** (GLOBAL_CFLAGS) |
| exceptions / RTTI | on (default `c++_static`) | `-fno-exceptions -fno-rtti` (GLOBAL_CXXFLAGS) |
| `-fPIC` | yes | `-fpic` (TARGET_CFLAGS) |
| `-std` | `-std=c++11` (c++_static) | `-std=c++11` (c++_static) |
| `-g` | yes | yes |
| `-Oz` | yes (Release) | yes (TARGET_thumb_release_CFLAGS) |
| `-DANDROID -DNDEBUG` | yes (Release) | yes (Release) |
| `-mfpu` | `vfpv3-d16` then `neon` (NEON on) | `vfpv3-d16`, `neon` with `LOCAL_ARM_NEON` |
| link order | **user libs before STL archives** | **static libs (STL) before shared libs** |
| `DT_NEEDED` tail | `libm libdl libc` | `libdl libc libm` |
| `_Unwind_*` | **UND** (resolved from FMOD) | **defined locally** (libgcc `libunwind.o`) |
| `__gnu_Unwind_Find_exidx` | **absent** | **UND** |
| `__gnu_ldivmod_helper` / `raise@LIBC` | **UND** (libgcc `_aeabi_ldivmod.o`/`_dvmd_lnx.o`) | different |
| `libc++_static.a` members | **none** | present |

### 5.2 The `-fdata-sections` discriminator

This is the cleanest one. The original's `.rodata`, `.data`, `.data.rel.ro` and `.bss`
are byte-identical to the no-`-fdata-sections` build. Adding `-fdata-sections`:

* costs 186–192 byte_exact (controlled A/B: 4505 → 4314 in `w5202`; 4506 → 4315 in
  wave-123 `w7320`; 4500 → 4314/4501 in wave-119 `w6719`);
* changes `.rodata` by 113,935 B, `.data` by 3,023 B, `.data.rel.ro` by 365 B,
  `.bss` by 9 B;
* changes `.dynsym` by 753 B and `.rel.dyn` by 180 B.

Since ndk-build r18b **always** adds `-fdata-sections`, vanilla ndk-build cannot be the
original build system. A CMake build (which does not add it) matches the data layout
exactly.

### 5.3 The link-order discriminator

The original's `DT_NEEDED` tail is `libm libdl libc`, `_Unwind_*` are UND, and
`__gnu_Unwind_Find_exidx` is absent. That is the CMake order: user libraries (FMOD)
come before the STL archives, so `_Unwind_*` resolve from FMOD and `libgcc.a`'s
`libunwind.o` is not pulled. ndk-build puts the static STL archives before the shared
libraries, which defines `_Unwind_*` locally and pulls `libunwind.o`, and its
`TARGET_LDLIBS := -lc -lm` gives the `libdl libc libm` tail. Moving the STL archives
before FMOD costs **3155 byte_exact**; the full vanilla `c++_static` group gives
2404/4504/4523 (`.text` +32 B).

### 5.4 The scores

| build | byte | linked | size | imports | notes |
|---|---:|---:|---:|---:|---|
| vanilla ndk-build defaults | **1345** | **3846** | 4523 | **189** | `_Unwind_*` local, wrong `DT_NEEDED` tail, `.dynsym`/`.dynstr` short by 18 symbols |
| coerced ndk-build (`APP_STL=none`, `-fno-data-sections`, explicit STL libs, mini libgcc, custom link order) | **2404** | **4504** | 4523 | **207** | close, but `.text` +32 B; still not exact |
| stock CMake r18b toolchain + 3 settings | **4505/4506** | **4505/4506** | **4524** | **207** | 0 differing function blobs across 192 TUs; full default link flag set gives the same sha except build-id |
| canonical repo build | **4506** | **4506** | **4524** | **207** | current gate |

(The CMake row is 4505 in wave-120 and 4506 in wave-123/wave-126; the one-function
difference is a source change between those bases, not a build-system difference.)

### 5.5 Why the contradiction existed

The APK is a Gradle/AGP build, and AGP's default native build is ndk-build. That made
"ndk-build" the natural first guess. But the APK contains **no** `Android.mk`,
`Application.mk`, `CMakeLists.txt`, `*.cmake`, `*.gradle`, `*.ninja`, `*.o`, `*.a` or
`*.d` — only the stripped `.so`. The binary's effective flags are the CMake default set,
not the ndk-build default set, so the project was configured with
`externalNativeBuild.cmake` (or an equivalent standalone CMake invocation). The exact
build file is unobservable; the effective recipe is not.

### 5.6 Confidence

| hypothesis | confidence |
|---|---:|
| CMake + stock r18b `android.toolchain.cmake` defaults + 3 settings, with a custom/trimmed gold link | **~85–95 %** |
| ndk-build with 6+ overrides | ~10 % |
| hand-written custom link script | ~5 % |

The build-system *file* is genuinely unobservable from the stripped ELF. What is proven
is the **effective build recipe**.

---

## 6. Evidence index

Prior notes (all in `_work/reconstructions/`):

* `build_provenance_w3901.md` — toolchain, `.comment`, `.note.android.ident`, `-DANDROID/-DNDEBUG`
* `build_system_evidence.md` — CMake-vs-ndk-build, link order
* `definitive_build_recipe_w3697.md` — first full recipe
* `build_config_rederivation_w3745.md` — link-flag A/B table (`-z now`, hash, spare tags, page size)
* `build-flags-wave123-w7319.md` — 259-candidate flag screen, zero positive deltas
* `build-flags-wave123-w7320.md` — 258 global + 140 per-TU cells, zero positive deltas
* `build-system-provenance-wave120-w7001.md` — stock-CMake equivalence, ndk-build scores
* `build_system_archaeology_w5202.md` — `-fdata-sections` controlled A/B, link-order proof
* `build-link-order-and-section-layout-w5018.md` — gold flag matrix
* `build-archaeology-cross-artifact-wb101.md` — cross-artifact (APK) provenance
* `build-toolchain-runtime-provenance-w5019.md` — runtime/ABI
* `build_archaeology_w4306.md` — early flag archaeology

Wave-126 scratch (all under `/dev/shm/`):

* `w7602/test_screen/summary.json` — opt/debug screen
* `w7603/partial.json` — 167 `-mllvm` candidates
* `w7604/audit/results_core.json` — 19 link variants
* `w7605/llvmrel.json` — LLVM release list
* `w7606/pp_results.json` — 170 preprocessor variants
* `w7608/simple.json` — 28 flags × 13 TUs
* `w7609/out/audit-*.txt` — strip audit
* `w7611/arm-audit/object_screen.json` — 34 ARM variants
* `w7613/results/object_sweep.json` — 23 PIC/visibility variants
* `w7614/baseline-sections.json` — 27-section diff
* `w7610/` — `Gun::calcCharacterCollision` + link matrix (`-z now`/`relro`/`norelro`)
* `w7612/` — `HangarWindow::render` + `-g0/g1/g2` matrix
* `w7607/` — `MGame::OnTouchEnd` target agent (not a flag agent)
* `w7600/` — empty

---

## 7. New measurement at HEAD and a doc discrepancy

The existing `cmake-build-match/libgof2hdaa.so` at HEAD `bf365a964` was diffed against
the original section by section (no rebuild; the `.so` is newer than every `src/*.cpp`):

```text
27 section headers: all identical (type/flags/addr/offset/size/link/info/align/entsize)
only differing payloads:
  .note.gnu.build-id   20 B
  .text            85,552 B
  .ARM.extab        1,438 B
```

`docs/VALIDATION.md:109` and `docs/IDENTICALITY.md:28–29,472,475` currently state
`.text` **85,636** and `.ARM.extab` **1,436**. The fresh measurement is **85,552** and
**1,438**. The difference is 84 B in `.text` and 2 B in `.ARM.extab`; it reflects the
source changes landed after the docs were last refreshed, not a regression (the gate
counts are unchanged). This note does not edit the docs because it lands no counted
change; a future doc-only pass should refresh those four numbers to 85,552 / 1,438.

---

## 8. What would falsify this synthesis

* A residual function becoming byte-exact when a *global* compile flag is changed while
  the gate stays green. Wave-123/126 tested ~600 global/per-TU variants and found none.
* A section other than `.text`/`.ARM.extab`/`.note.gnu.build-id` differing. The fresh
  HEAD diff shows none.
* A reproducible build with `-fdata-sections` matching the original's `.rodata`/`.data`.
  It does not: the data sections are byte-identical only without it.
* A link order with the STL archives before FMOD matching the original's `_Unwind_*`
  imports. It does not: that order costs 3155 byte_exact.

Until one of those happens, the correct conclusion is: **the flags are right; the
remaining 18 functions are source reconstruction.**
