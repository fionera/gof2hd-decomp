# Toolchain and runtime provenance (w5019, wave 100)

Status: research note, 2026-11-04. Worktree `wt/w5019`, branch
`wave100-w5019`, base `44225192c`. Scratch is `/dev/shm/w5019`. **No `src/`
or build-file change is landed**: the note is the deliverable. The canonical
gate is unchanged (byte_exact 4500/4524, linked_exact 4500/4524, size-exact
4524/4524, data 843/843, imports 207, lint CLEAN, drift 0; `RATCHET PASS`).

This note independently re-derives the build recipe from the shipped
artifacts rather than from `CMakeLists.txt`. It confirms the wave-93
(`build_archaeology_w4306.md`) and wave-89 (`toolchain_archive_provenance_w3904.md`)
conclusions, and adds three things those notes did not have:

1. **The `.comment` GCC string is exactly `libcrypto_gof2.a(sha256.o)`'s
   `.comment`**, not libgcc's: `crtbegin_so.o`'s `.comment` concatenated with
   `sha256.o`'s `.comment` (minus the duplicate leading NUL) is *byte-for-byte*
   the original `.comment`.
2. **`libgcc.a` is genuinely pulled**, contrary to wave-93's "libgcc.a not
   pulled" shorthand: `_aeabi_ldivmod.o` and `_dvmd_lnx.o` are linked, and the
   original's `__aeabi_ldivmod` code at `0x19ae54` matches the tree's libgcc
   member byte-for-byte. FMOD exports `__aeabi_uldivmod` and
   `__gnu_ldivmod_helper` but **not** `__aeabi_ldivmod`, which is exactly why
   libgcc is pulled and its helper call stays `UND`.
3. **The full CMake r18b default STL/link group reproduces the current
   minimal recipe byte-for-byte**: `-latomic -lm libc++_static.a libc++abi.a
   libandroid_support.a libunwind.a -lgcc -ldl -lc` (with the same FMOD/crypto
   prefix) gives the identical `.so` sha256. The current recipe's omissions
   (`libc++_static.a`, `libunwind.a`, `libatomic.a`, `--fix-cortex-a8`,
   `--no-undefined`, `-z noexecstack`, `-z relro`, `--warn-shared-textrel`,
   `--fatal-warnings`, `--exclude-libs=libatomic.a`,
   `--exclude-libs=libunwind.a`) are all byte-neutral on this tree.

The new A/B tests also add `-fno-vectorize`, `-fno-merge-constants`,
`-fno-delete-null-pointer-checks`, `-fno-plt`, `-mno-movt`, `-mtune=cortex-a8`,
`-mfpu=neon-vfpv4`, `-fpic`, `-fno-sized-deallocation`, `-fshort-wchar`,
`-std=gnu++11`, `-fno-builtin`, `-fno-unroll-loops`,
`-fno-optimize-sibling-calls`, `-march=armv7ve`, `-fno-common`,
`-fno-stack-protector`, and two clang-7-unknown spellings
(`-fno-semantic-interposition`, `-mno-ldrd`).

## 0. Bottom line

* The original is **NDK r18b, clang 7.0.2 (`4751641 based on r328903`), gold
  1.12, API 16, armeabi-v7a, Thumb-2, NEON, softfp, `c++_static`**, built on a
  **darwin** host. Every one of those facts has a positive artifact
  fingerprint below.
* The native build is the **NDK r18b CMake toolchain default set**, not
  ndk-build. The two decisive discriminators are both CMake defaults:
  **no `-fdata-sections`** (ndk-build adds it) and the **STL archives after
  `target_link_libraries`** (ndk-build puts static libraries before shared
  libraries, pulling `libunwind.a`). The CMake defaults for ABI
  (`armeabi-v7a`), platform (`android-16`, the r18b minimum), STL
  (`c++_static`), and ARM mode (`thumb`) are exactly what the binary shows;
  only `ANDROID_ARM_NEON=ON`, `ANDROID_CPP_FEATURES="rtti exceptions"`, and
  `CMAKE_BUILD_TYPE=Release` need to be explicit.
* The `.comment` GCC string is from **libcrypto's `sha256.o`** (GCC 4.9.x
  20150123). The pulled `libgcc.a` members have no `.comment` at all.
* The link order is proven by the original's `.text` tail and by relink A/B:
  **FMOD → libcrypto → libc++abi → libandroid_support → libgcc → dl → c**.
  Moving any of those across FMOD (or moving `libandroid_support` before
  `libc++abi`) changes `.text`/`.rel.plt`/`.dynsym`; the canonical order is
  the one that reproduces the original's `.text`, `.rel.plt`, `.dynsym`,
  `.dynstr`, `.hash`, `.got`, `.plt`, `.rel.dyn` (the only residual is the
  known 6-entry `.rel.plt` permutation from the 24 non-exact source bodies).
* The current `CMakeLists.txt` link recipe is **not wrong**: it is a minimal
  output-equivalent subset of the CMake default. No build-file correction is
  needed; this note records the equivalence and the extra no-op flags.

## 1. Artifacts and hashes

| artifact | path | sha256 | size |
|---|---|---|---:|
| ground-truth `.so` | `_work/bins/android_2.0.16_libgof2hdaa.so` | `d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483` | 2,187,328 |
| APK `lib/armeabi-v7a/libgof2hdaa.so` | original APK | `6d38fd767ee3b4f8174c371bce371bfc6397cf76e13c1756938b2a94658e3b58` | 2,187,328 |
| canonical tree build | `cmake-build-match/libgof2hdaa.so` | `8777224b8ea04833adf5efa423de03db463dba3d50956fb2e0f8a5f278888e95` | 2,187,328 |
| NDK r18b linux zip | `dl.google.com/android/repository/android-ndk-r18b-linux-x86_64.zip` | `4f61cbe4bbf6406aa5ef2ae871def78010eed6271af72de83f8bd0b07a9fd3fd` | 557,038,702 |
| NDK r18b darwin zip | `dl.google.com/android/repository/android-ndk-r18b-darwin-x86_64.zip` | `dd6524c3cc91725b5c39370f6deb27d8eee056e2bb2efef0a8008b4ca6d83891` | 542,911,996 |
| darwin `libc++abi.a` | `sources/cxx-stl/llvm-libc++/libs/armeabi-v7a` | `9a0f50d22ef53a946b76d689f723b2ca88d2cb0f7e17ee133ab19395a925fadb` | 1,147,158 |
| linux `libc++abi.a` | same path in the linux package | `6993e991ff2d9e4f45febab2f7758fe293c14d08f2364796c6d63f2b5cc210a6` | 1,144,222 |
| `crtbegin_so.o` | `platforms/android-16/arch-arm/usr/lib` | `68be056ee637c2ef7b7acc899bc075857f7999e4ebd46b8ed2ff6d31ab0d909c` | — |
| `crtend_so.o` | same | `31dbabfa0740e8a0ce4a3cdd03874d0aa0e4705ab861554243883c5b94a4a31e` | — |
| `libcrypto_gof2.a` | tree vendored OpenSSL 1.1.0l | `50662126d068d8b96ece99f83dafd04f975c43263c193eedca5ac5f3fa5595f2` | — |
| `libfmodex.so` | APK `lib/armeabi-v7a` | `60ec4de090bf442c7b9db41d0731aa22a101db21aab0ae32af1af6a860c3fa81` | 920,204 |
| `libfmodevent.so` | APK `lib/armeabi-v7a` | `b92cd6ff8f8edb3d81023ee69c1f5239aeaf6cb4e89d6505101115189569c97d` | 370,660 |

The APK library is the ground truth after the NDK's GNU
`arm-linux-androideabi-strip --strip-all` (or `--strip-unneeded`; identical
here because gold already removed symbols). The 288-byte delta between the
APK and the ground truth is entirely `.shstrtab` content plus the 27
section-header `sh_name` fields, i.e. the bfd strip's section-order rewrite;
`tools/reorder_shstrtab.py` restores gold's Stringpool order.

## 2. Toolchain identity

### 2.1 `.note.android.ident` — NDK r18b, API 16, from `crtbegin_so.o`

```sh
readelf -nW $O
```

The note is a standard `Android`/`NT_VERSION` note, 152 bytes:

| field | value |
|---|---|
| `namesz` / `descsz` / `type` | `8` / `0x84` / `1` |
| name | `Android\0` |
| API | `16` |
| NDK | `r18b` |
| build | `5063045` |

`5063045` is exactly NDK r18b's `Pkg.Revision = 18.1.5063045`
(`.cache/ndk/android-ndk-r18b/source.properties`).

**Origin.** The note is emitted by `crtbegin_so.o`, not by the app TUs:

```sh
readelf -nW $NDK/platforms/android-16/arch-arm/usr/lib/crtbegin_so.o
```

The section is **byte-identical** to the original's `.note.android.ident`
(152/152 bytes). `crtend_so.o` has no such note. This also pins the API-16
`crtbegin_so.o`/`crtend_so.o` objects, which are byte-identical across the
linux and darwin r18b packages.

### 2.2 `.comment` — clang 7.0.2 plus one GCC 4.9 object

```sh
readelf -p .comment $O
```

```text
Android (4751641 based on r328903) clang version 7.0.2
  (https://android.googlesource.com/toolchain/clang 003100370607242ddd5815e4a043907ea9004281)
  (https://android.googlesource.com/toolchain/llvm 1d739ffb0366421d383e04ff80ec2ee591315116)
  (based on LLVM 7.0.2svn)
GCC: (GNU) 4.9.x 20150123 (prerelease)
```

The exact decomposition is a **new positive proof**:

```python
# crtbegin_so.o .comment: b'\0' + clang string           (264 bytes)
# libcrypto sha256.o .comment: b'\0' + GCC string + b'\0'  (40 bytes)
assert crtbegin_comment + sha256_comment[1:] == original_comment   # 303 bytes
```

That is, the original `.comment` is exactly the merge of `crtbegin_so.o`'s
clang string and `libcrypto_gof2.a(sha256.o)`'s GCC string (gold deduplicates
the shared leading empty string). The pulled `libgcc.a` members
(`_aeabi_ldivmod.o`, `_dvmd_lnx.o`) have **no `.comment` section at all**, so
the GCC string is not from libgcc. The tree's `sha256.o` is built by the real
r13b GCC 4.9 (`arm-linux-androideabi-gcc (GCC) 4.9.x 20150123 (prerelease)`);
its `.comment` is the same 40-byte string.

The clang string is also the `.comment` of `crtbegin_so.o` and of all 192
app TUs (see §4). `clang++ --version` from the r18b package prints the same
`Android (4751641 based on r328903) clang version 7.0.2` line.

### 2.3 `.note.gnu.gold-version` — gold 1.12

```sh
readelf -nW $O
# GNU 0x9 NT_GNU_GOLD_VERSION: gold 1.12
```

`gold 1.12` is the r18b `ld.gold` version (`GNU gold (GNU Binutils
2.27.0.20170315) 1.12`). The section is byte-identical to the tree's output.

### 2.4 ELF and ABI

```sh
readelf -hW $O
readelf -AW $O
readelf -dW $O
```

| observable | value | implication |
|---|---|---|
| ELF | `ELF32`, `DYN`, `ARM`, `EABI5`, flags `0x5000200` (`EF_ARM_ABI_FLOAT_SOFT`) | **armeabi-v7a softfp** |
| `.ARM.attributes` | `ARM v7`, `Application`, `Thumb-2`, `VFPv3`, `NEONv1`, `wchar_t 4`, `enum int`, `unaligned v6` | `-march=armv7-a -mthumb -mfpu=neon -mfloat-abi=softfp`; no `-fshort-wchar`, no `-fshort-enums`, unaligned access on |
| `DT_NEEDED` | `liblog libGLESv2 libGLESv1_CM libEGL libandroid libfmodex libfmodevent libm libdl libc` | shared-library link order; FMOD before `m/dl/c` |
| `DT_SONAME` | `libgof2hdaa.so` | `-soname libgof2hdaa.so` |
| `DT_FLAGS`/`DT_FLAGS_1` | `BIND_NOW`/`NOW` | `-z now` |
| `PT_GNU_RELRO` | covers `.fini_array .data.rel.ro .init_array .dynamic .got` | `-z relro` |
| `PT_GNU_STACK` | `RW` | `-z noexecstack` |
| `.hash` only | no `.gnu.hash` | gold `--hash-style=sysv` default |
| `.rel.plt` | `SHF_INFO_LINK` (`AI`), `sh_info=20` | bfd strip normalization (see §3.5) |

The final `.ARM.attributes` is a merge over the app TUs, libc++abi,
libandroid_support, libcrypto, and libgcc. The app TUs carry
`Tag_CPU_arch: v7`, `Tag_THUMB_ISA_use: Thumb-2`, `Tag_FP_arch: VFPv3`,
`Tag_Advanced_SIMD_arch: NEONv1`, `Tag_CPU_unaligned_access: v6`; libc++abi
and libandroid_support carry `VFPv3-D16`; libgcc carries `VFPv2`; the merge
takes the maximum, which is the original's `VFPv3`/`NEONv1`.

### 2.5 Host was darwin

The only path-like string in the original `.rodata` is:

```text
/Volumes/Android/buildbot/src/android/ndk-release-r18/external/libcxx/../../external/libcxxabi/src/abort_message.cpp
```

at file offset `0x1e6b2b` (`.rodata` starts at `0x1c9e10`). It comes from
`libc++abi.a(abort_message.o)`'s `.rodata.str1.1`, which is 131 bytes in the
darwin package and 115 bytes in the linux package; the 16-byte difference is
exactly the `/Volumes/Android` prefix. The linux archive's path is
`/buildbot/src/android/...`.

**A/B (new independent re-run):** relink the canonical objects with the linux
`libc++abi.a` instead of the darwin one:

```sh
# replace the cxxabi token in the canonical gold command with
# /dev/shm/w5019/libc++abi_linux.a, then strip + reorder_shstrtab
python3 tools/verify/verify.py --build-dir /dev/shm/w5019/verify_linuxabi \
  --original $O --ours /dev/shm/w5019/relink_linuxabi.so \
  --objdump $OBJ --fail-on-wrong-type --top 0
```

| variant | byte_exact | linked_exact | `.rodata` | `.text` diff | `.data` diff | `.data.rel.ro` diff | `.dynsym` diff |
|---|---:|---:|---|---:|---:|---:|---:|
| darwin `libc++abi.a` (canonical) | **4500** | 4500 | 269,159 (0 diff) | 101,683 | 0 | 0 | 0 |
| linux `libc++abi.a` | **4344** | 4500 | 269,143 (-16) | 102,446 | 6 | 108 | 195 |

The 16-byte `.rodata` shift changes the PC-relative literal-pool words of 156
functions, which is exactly the `byte_exact 4500 -> 4344` drop while
`linked_exact` stays 4500. **The darwin archive is forced.**

## 3. Build system: CMake, not ndk-build

### 3.1 The NDK r18b defaults

The r18b CMake toolchain (`$NDK/build/cmake/android.toolchain.cmake`) and the
ndk-build makefiles differ in exactly the places the binary can see:

| setting | CMake r18b default | ndk-build r18b default | original | verdict |
|---|---|---|---|---|
| `ANDROID_ABI` | `armeabi-v7a` | all default ABIs | `armeabi-v7a` only | CMake default |
| `ANDROID_PLATFORM` | `android-16` (r18b minimum) | `android-16` (minimum) | API 16 | both default |
| `ANDROID_STL` | `c++_static` | `system` (libstdc++) | libc++ static | CMake default |
| `ANDROID_ARM_MODE` | `thumb` | `thumb` | Thumb-2 | both default |
| `ANDROID_ARM_NEON` | `FALSE` for API < 23 | `LOCAL_ARM_NEON` false | NEONv1 | **explicit ON** |
| `ANDROID_CPP_FEATURES` | empty | empty | RTTI + exceptions | **explicit** |
| `CMAKE_BUILD_TYPE` | empty | `release` | `-Oz -DNDEBUG` | **explicit Release** |
| `-ffunction-sections` | **yes** | **yes** | yes | both |
| `-fdata-sections` | **no** | **yes** | **no** | **CMake** |
| `-fstack-protector-strong` | yes | yes | yes | both |
| `-funwind-tables` | yes | yes | yes | both |
| `-g` | yes | yes | yes | both |
| `-DANDROID` | yes | yes | unobservable | both |
| `-std=c++11` | yes (for `c++_static`) | yes (for `c++_static`) | C++11 codegen | both |
| `-Wl,--fix-cortex-a8` | yes (armeabi-v7a) | yes | unobservable | both |
| `-Wl,--build-id` | yes | yes | yes | both |
| `-Wl,-z,noexecstack -z relro -z now` | yes | yes | yes | both |
| `-Wl,--no-undefined` | yes (default) | no | unobservable | CMake |
| `-Wl,--exclude-libs,libgcc.a` | yes | yes | yes | both |
| `-Wl,--exclude-libs,libatomic.a` | yes | no | unobservable | CMake |
| `-Wl,--exclude-libs,libunwind.a` | yes (c++_static/ARM) | no | unobservable | CMake |
| STL archives after `target_link_libraries` | **yes** | **no** (static before shared) | yes | **CMake** |

The CMake toolchain's compile-flag block is:

```cmake
# $NDK/build/cmake/android.toolchain.cmake
list(APPEND ANDROID_COMPILER_FLAGS
  -g -DANDROID -ffunction-sections -funwind-tables -fstack-protector-strong
  -no-canonical-prefixes)
# release, for armeabi + thumb:
list(APPEND ANDROID_COMPILER_FLAGS_RELEASE -Oz -DNDEBUG)
# c++_static:
list(APPEND ANDROID_COMPILER_FLAGS_CXX -std=c++11)
# ANDROID_CPP_FEATURES:
list(APPEND ANDROID_COMPILER_FLAGS_CXX -f${feature})
```

There is **no `-fdata-sections`** in the CMake flags. ndk-build's
`build/core/default-build-commands.mk` has:

```make
GLOBAL_CFLAGS = \
    -target $(LLVM_TRIPLE)$(TARGET_PLATFORM_LEVEL) \
    -fdata-sections \
    -ffunction-sections \
    -fstack-protector-strong \
    -funwind-tables \
    -no-canonical-prefixes
```

That is the decisive build-system discriminator.

### 3.2 `-fdata-sections` is disproven

A/B with the official verifier (one flag on top of the canonical build):

| variant | byte_exact | linked_exact | `.rodata` | `.data` | `.data.rel.ro` |
|---|---:|---:|---|---|---|
| canonical (no `-fdata-sections`) | **4500** | 4500 | 269,159 (0 diff) | 10,604 (0) | 12,264 (0) |
| `-fdata-sections` | **4311** | 4500 | 0x41a07 (smaller; 113,935 diff) | 3,023 diff | 365 diff |

The original's `.rodata`, `.data`, and `.data.rel.ro` are **byte-identical**
without `-fdata-sections`. With it they are not. This is positive proof of
the CMake default.

### 3.3 Link order is observable and proven

The canonical link order is:

```text
crtbegin_so.o
<app objects in tools/source_order.txt order>
--whole-archive libz_gof2.a libzip_gof2.a --no-whole-archive
-l log -l GLESv2 -l GLESv1_CM -l EGL -l android
libfmodex.so libfmodevent.so
libcrypto_gof2.a
-l m
libc++abi.a
libandroid_support.a
libgcc.a
-l dl -l c
crtend_so.o
```

The original's `.text` tail proves the last three archives' order directly.
Our canonical map (`ld.gold -Map -t`) places:

| address | object/member | content |
|---:|---|---|
| `0x19ae14` | `libandroid_support.a(posix_memalign.o)` | `posix_memalign` |
| `0x19ae54` | `libgcc.a(_aeabi_ldivmod.o)` | `__aeabi_ldivmod` |
| `0x19ae8c` | `libgcc.a(_dvmd_lnx.o)` | `__aeabi_idiv0`, `__aeabi_ldiv0` |

The original's bytes at `0x19ae54..0x19ae8c` are **byte-identical** to the
tree's `_aeabi_ldivmod.o`/`_dvmd_lnx.o` code, and the original's `.text` has
`posix_memalign` immediately before it. That is the order
`libc++abi.a -> libandroid_support.a -> libgcc.a`.

**Relink A/B** (same objects, only the link order changed; `.text`/`.rel.plt`
diffs are vs the canonical base):

| variant | `.text` size | `.text` diff | `.rel.plt` diff | `.dynsym` diff | `.dynstr` diff | `.rodata` diff |
|---|---:|---:|---:|---:|---:|---:|
| base | 1,259,464 | 101,683 | 12 | 0 | 0 | 0 |
| `libgcc.a` before FMOD | 1,266,980 | 144,210 | 12,594 | 32,293 | 180,801 | 0 |
| `libandroid_support.a` before `libc++abi.a` | 1,259,400 | 59,107 | 2,725 | 17,096 | 123,342 | 0 |
| `libunwind.a` before FMOD | 1,265,080 | 147,223 | 11,231 | 31,073 | 174,026 | 124,076 |
| `libc++abi.a` before FMOD | 1,259,476 | 58,791 | 3,158 | 23,968 | 141,494 | 0 |
| `libcrypto_gof2.a` before FMOD | 1,259,464 | 0 | 1,990 | 12,092 | 98,364 | 0 |
| `libc++_static.a` before `libc++abi.a` | 1,259,464 | 0 | 0 | 0 | 0 | 0 |
| `libgcc.a` after `-ldl` | 1,259,464 | 0 | 0 | 0 | 0 | 0 |
| full CMake default STL group + libgcc | 1,259,464 | 0 | 0 | 0 | 0 | 0 |

The `libcrypto`-before-FMOD row is especially informative: `.text` is
unchanged (the same libcrypto code is pulled), but `.rel.plt`/`.dynsym`/
`.dynstr` change because gold's first-reference order changes. The canonical
base's `.rel.plt` matches the original's except for the known 6-entry
permutation caused by the 24 non-exact source bodies. The full CMake default group
(`-latomic -lm libc++_static.a libc++abi.a libandroid_support.a libunwind.a
-lgcc -ldl -lc`) is **sha256-identical** to the canonical minimal recipe.

### 3.4 Archive member pull list

`ld.gold -Map -t` on the canonical link gives the exact member set:

| archive | members pulled | `.comment` |
|---|---:|---|
| `libz_gof2.a` | 11 | clang 7.0.2 (all) |
| `libzip_gof2.a` | 54 | clang 7.0.2 (all) |
| `libcrypto_gof2.a` | 3 (`sha256.o`, `sha256-armv4.o`, `armv4cpuid.o`) | **GCC 4.9.x** only on `sha256.o`; the two asm objects have none |
| `libc++abi.a` | 14 (`abort_message.o`, `cxa_default_handlers.o`, `cxa_demangle.o`, `cxa_exception.o`, `cxa_exception_storage.o`, `cxa_guard.o`, `cxa_handlers.o`, `cxa_personality.o`, `cxa_virtual.o`, `fallback_malloc.o`, `private_typeinfo.o`, `stdlib_exception.o`, `stdlib_new_delete.o`, `stdlib_typeinfo.o`) | clang 7.0.2 (all) |
| `libandroid_support.a` | 1 (`posix_memalign.o`) | clang 7.0.2 |
| `libgcc.a` | 2 (`_aeabi_ldivmod.o`, `_dvmd_lnx.o`) | **none** |
| `libc++_static.a` | 0 | — |
| `libunwind.a` | 0 | — |
| `libatomic.a` | 0 | — |

`libc++_static.a`, `libunwind.a`, and `libatomic.a` are not pulled. Their
presence/position is therefore unobservable; adding either host's archives in
the CMake default order is byte-identical.

### 3.5 `crtbegin_so.o` / `crtend_so.o` and the bfd strip

The original's `.init_array` has 3 entries and `.fini_array` has 2, both
byte-identical to the tree's. The `crtbegin_so.o`/`crtend_so.o` from the
API-16 sysroot are byte-identical across the linux and darwin r18b packages.
`crtbegin_so.o` is the source of `.note.android.ident` and of the clang
`.comment` string; `crtend_so.o` has no `.comment`.

The original's `.rel.plt` has `sh_flags=AI` and `sh_info=20` (`.got`), and
its `.init_array`/`.fini_array` have `sh_entsize=4`, while gold's own output
does not. These are **bfd strip normalizations**, not darwin-gold behavior.
Direct evidence from the canonical link:

| section | unstripped gold output | after GNU `strip --strip-all` | original |
|---|---|---|---|
| `.rel.plt` | `A`, `sh_info=0` | `AI`, `sh_info=20` | `AI`, `sh_info=20` |
| `.init_array` | `sh_entsize=0` | `sh_entsize=4` | `sh_entsize=4` |
| `.fini_array` | `sh_entsize=0` | `sh_entsize=4` | `sh_entsize=4` |

So the canonical link is gold (unstripped) followed by GNU
`arm-linux-androideabi-strip --strip-all`, and that is what produces the
observed header form. The APK/ground-truth delta is exactly 288 differing bytes (`cmp -l`);
wave-89's section analysis places all of them in `.shstrtab` content plus the
27 `sh_name` fields. `tools/reorder_shstrtab.py` restores gold's Stringpool
order.

### 3.6 FMOD and libgcc: why `__aeabi_ldivmod` is local

`libfmodex.so` exports:

```text
__aeabi_uldivmod          (GLOBAL, 0xd129c)
__gnu_ldivmod_helper      (GLOBAL, 0xd1478)
```

but **not** `__aeabi_ldivmod`. The app's `Transform.cpp.obj` references
`__aeabi_ldivmod`; because FMOD does not define it, gold pulls
`libgcc.a(_aeabi_ldivmod.o)`. That member's own undefined
`__gnu_ldivmod_helper` is already resolved by FMOD (which precedes libgcc),
so it stays `UND` and gets a PLT entry. `libgcc.a(_dvmd_lnx.o)` is pulled
because `_aeabi_ldivmod.o` references `__aeabi_ldiv0`. This is exactly the
original's shape:

```text
readelf --dyn-syms $O | grep -E '__aeabi_(l|ul)divmod|__gnu_ldivmod_helper'
# __aeabi_uldivmod      UND
# __gnu_ldivmod_helper  UND
# (no __aeabi_ldivmod; it is local, hidden by --exclude-libs=libgcc.a)
```

If libgcc were before FMOD, `__gnu_ldivmod_helper` would be pulled from
libgcc and defined locally; the A/B row above shows the resulting
`.text`/`.dynsym` change.

### 3.7 libc++abi source paths in `.rodata`

Only `abort_message.o`'s path survives in the original `.rodata`; the other
libc++abi members' darwin paths are in `.debug_*` (stripped). The darwin
`abort_message.o` `.rodata.str1.1` is 131 bytes; the linux one is 115 bytes.
The 16-byte difference is exactly the `/Volumes/Android` prefix, and the
linux-archive relink drops `byte_exact` from 4500 to 4344 (see §2.5).

## 4. Per-TU compiler fingerprints

All 192 app TUs in `cmake-build-match/CMakeFiles/gof2.dir` have exactly one
`.comment` value: the clang 7.0.2 string. The vendored zlib/libzip objects and
the pulled libc++abi/libandroid_support members are also clang 7.0.2. The
only non-clang object that contributes a `.comment` is libcrypto's
`sha256.o` (GCC 4.9.x). The pulled libgcc members have no `.comment`. **No TU
shows a different compiler or version fingerprint.**

`readelf -AW` on the pulled members:

| group | CPU arch | Thumb | FP | NEON | unaligned |
|---|---|---|---|---|---|
| app TUs | v7 | Thumb-2 | VFPv3 | NEONv1 | v6 |
| zlib/libzip | v7 | Thumb-2 | VFPv3 | NEONv1 | v6 |
| libc++abi/libandroid_support | v7 | Thumb-2 | VFPv3-D16 | — | v6 |
| libcrypto `sha256.o` | v7 | Thumb-2 | — | — | v6 |
| libcrypto asm objects | v7 | Thumb-2 | VFPv3 | NEONv1 | — |
| libgcc `_aeabi_ldivmod.o`/`_dvmd_lnx.o` | v7 | Thumb-2 | VFPv2 | — | — |

The final `.so`'s `.ARM.attributes` is the merge (VFPv3/NEONv1/v6), and it is
byte-identical to the original's.

## 5. A/B matrix (official verifier)

All variants are one flag on top of the canonical `-Oz -g -fPIC -frtti
-fstack-protector-strong -ffunction-sections -funwind-tables -DANDROID
-DNDEBUG -std=c++11` build. `byte`/`linked` are the verifier's counts; `size`
is `sum(osize == usize)`.

| variant | flag | byte | linked | size | verdict |
|---|---|---:|---:|---:|---|
| base | — | **4500** | **4500** | 4524/4524 | canonical |
| fdata | `-fdata-sections` | 4311 | 4500 | 4524/4524 | **disproven** |
| fnobuiltin | `-fno-builtin` | 1350 | 3988 | 4524/4524 | **disproven** |
| fnovectorize | `-fno-vectorize` | 4500 | 4500 | 4524/4524 | **unobservable** |
| fnounroll | `-fno-unroll-loops` | 2883 | 4493 | 4524/4524 | **disproven** |
| fnomergeconst | `-fno-merge-constants` | 4500 | 4500 | 4524/4524 | **unobservable** |
| fnooptsib | `-fno-optimize-sibling-calls` | 1316 | 3590 | 4524/4524 | **disproven** |
| fnodelnull | `-fno-delete-null-pointer-checks` | 4500 | 4500 | 4524/4524 | **unobservable** |
| fnoplt | `-fno-plt` | 4500 | 4500 | 4524/4524 | **unobservable** |
| mnomovt | `-mno-movt` | 4500 | 4500 | 4524/4524 | **unobservable** |
| mtuneca8 | `-mtune=cortex-a8` | 4500 | 4500 | 4524/4524 | **unobservable** |
| mfpu_neonvfpv4 | `-mfpu=neon-vfpv4` | 4500 | 4500 | 4524/4524 | **unobservable** |
| fpic | `-fpic` (after `-fPIC`) | 4500 | 4500 | 4524/4524 | **unobservable** |
| nosizeddealloc | `-fno-sized-deallocation` | 4500 | 4500 | 4524/4524 | **unobservable** |
| shortwchar | `-fshort-wchar` | 4500 | 4500 | 4524/4524 | **unobservable** |
| gnupp11cxx | `-std=gnu++11` (CXX only) | 4500 | 4500 | 4524/4524 | **unobservable** |
| marchv7ve | `-march=armv7ve` | 1415 | 4431 | 4524/4524 | **disproven** |
| nocommon | `-fno-common` | **LINK FAIL** | | | `g_android_back_button_pressed` duplicate |
| nostackprot | `-fno-stack-protector` | (not run to completion) | | | **disproven**: 85 `__stack_chk_fail@plt` hits (vendored archives only) vs original 788 |
| fnosemaninterp | `-fno-semantic-interposition` | **BUILD FAIL** | | | clang 7.0.2: unknown argument |
| mnoldrd | `-mno-ldrd` | **BUILD FAIL** | | | clang 7.0.2: unknown argument |

Wave-93's matrix remains valid for the flags not re-run here: `-Os` 1263,
`-O1` 1033, `-O2` 1168, `-O3` 1167, `-g0` 2069, `-marm` 57,
`-fno-rtti` 1701, `-fno-exceptions` build fail, `-fno-threadsafe-statics`
1368, `-fno-use-cxa-atexit` 1356, `-fno-unwind-tables` 2878,
`-fomit-frame-pointer` 1298, `-fno-inline` 1219, `-fno-strict-aliasing` 1371,
`-fwrapv` 1398, `-ffast-math` 1319, `-fvisibility=hidden` 57,
`-fno-jump-tables` 1363, `-fno-slp-vectorize` 1492,
`-mno-unaligned-access` 1391, `-fshort-enums` 2869, `-std=c++14` 1512,
`-mfpu=vfpv3-d16` build fail, `-mfloat-abi=hard` link fail,
`-fstack-protector` 1377, `-fstack-protector-all` 91.

## 6. Flag classification

### PROVEN (positive artifact fingerprint)

| fact | evidence |
|---|---|
| NDK r18b, API 16 | `.note.android.ident` = `api=16 ndk=r18b build=5063045`, byte-identical to r18b `crtbegin_so.o` |
| clang 7.0.2 r328903 | `.comment` clang string; all 192 app TUs; r18b `clang++ --version` |
| gold 1.12 | `.note.gnu.gold-version` |
| armeabi-v7a, Thumb-2, VFPv3, NEONv1, softfp | ELF flags `0x5000200`, `.ARM.attributes` |
| `-march=armv7-a -mthumb -mfpu=neon -mfloat-abi=softfp` | `.ARM.attributes`; `-marm`/`vfpv3-d16`/`hard` A/B fail or collapse |
| `-Oz` | A/B `-Os`/`-O1`/`-O2`/`-O3` all collapse |
| `-g` | A/B `-g0` 2069 |
| `-fstack-protector-strong` | 787 `__stack_chk_fail` call sites; basic 1377, all 91, none 85 PLT hits |
| `-ffunction-sections` | 193 CANTUNWIND+real `.ARM.exidx` pairs |
| `-funwind-tables` | 3566 exidx entries; removal 2878 |
| `-frtti` | 99 vtables, 176 typeinfo; `-fno-rtti` 1701 |
| `-fexceptions` | `.ARM.extab`, `__cxa_throw`, `_Unwind_Resume`; `-fno-exceptions` build fail |
| `-fthreadsafe-statics` | `__cxa_guard_acquire` + 2 call sites; off 1368 |
| `-fuse-cxa-atexit` | `__cxa_atexit` imported/called, no `atexit`; off 1356 |
| `-fno-sized-deallocation` | 0 `_ZdlPvj`/`_ZdaPvj` calls; flag neutral |
| no `-fdata-sections` | `.rodata`/`.data`/`.data.rel.ro` byte-identical; with it 4311 |
| `c++_static` | libc++/libc++abi symbols defined in `.so`; no `libc++_shared.so` NEEDED |
| darwin host | `/Volumes/Android/.../abort_message.cpp` in `.rodata`; linux A/B 4344 |
| FMOD before libcrypto/libc++abi/libandroid_support/libgcc | link-order A/B; `.text`/`.rel.plt`/`.dynsym` |
| libcrypto after FMOD and before libc++abi/libgcc | `crypto_before_fmod` / `crypto_after_cxxabi` / `crypto_after_libgcc` A/B change `.rel.plt`/`.dynsym`; canonical order matches the original |
| libgcc `_aeabi_ldivmod.o`/`_dvmd_lnx.o` pulled | original `__aeabi_ldivmod` bytes at `0x19ae54`; gold `-Map`/`-t` |
| `.comment` GCC string = libcrypto `sha256.o` | exact `crtbegin + sha256[1:]` byte decomposition |
| `-Wl,--build-id` (sha1 style) | 20-byte `.note.gnu.build-id` |
| `-z now`, `-z relro`, `-z noexecstack` | `DT_FLAGS`/`DT_FLAGS_1`, `PT_GNU_RELRO`, `PT_GNU_STACK` |
| `--hash-style=sysv` | `.hash` only, no `.gnu.hash` |
| GNU strip after link | APK/ground-truth 288-byte `.shstrtab` delta; bfd header normalization |

### UNOBSERVABLE (codegen-neutral on this tree)

| flag | evidence |
|---|---|
| `-fPIC` vs `-fpic` | A/B identical (4500) |
| `-std=c++11` vs `gnu++11` | A/B identical (4500) |
| `-fno-sized-deallocation` | A/B identical (4500) |
| `-fshort-wchar` | A/B identical (4500) |
| `-fno-vectorize` | A/B identical (4500) |
| `-fno-merge-constants` | A/B identical (4500) |
| `-fno-delete-null-pointer-checks` | A/B identical (4500) |
| `-fno-plt` | A/B identical (4500) |
| `-mno-movt` | A/B identical (4500) |
| `-mtune=cortex-a8` | A/B identical (4500) |
| `-mfpu=neon-vfpv4` | A/B identical (4500) |
| `-DANDROID` | no `#ifdef ANDROID` in `src/` |
| `-DNDEBUG` | no app runtime `assert`; CMake Release default |
| `-Wl,--fix-cortex-a8` | A/B identical (no veneers needed) |
| `-Wl,--no-undefined` | A/B identical (all imports resolved by NEEDED libs) |
| `-Wl,-z,noexecstack -z relro` | A/B identical (already emitted) |
| `-Wl,--warn-shared-textrel --fatal-warnings` | warnings only; A/B identical |
| `--exclude-libs=libatomic.a`, `--exclude-libs=libunwind.a` | archives not pulled; A/B identical |
| `libc++_static.a`, `libunwind.a`, `libatomic.a` position | 0 members pulled; A/B identical |
| `libgcc.a` position relative to `-ldl` | A/B identical |
| `CMAKE_BUILD_TYPE=Release` vs explicit `-Oz -DNDEBUG` | same flags |
| `ANDROID_CPP_FEATURES` spelling vs `APP_CPPFLAGS` | same `-frtti -fexceptions` |

### DISPROVEN

| flag/fact | evidence |
|---|---|
| ndk-build (`-fdata-sections`) | A/B 4311; `.rodata`/`.data` differ |
| `-Os`, `-O1`, `-O2`, `-O3` | 1263/1033/1168/1167 |
| `-g0` | 2069 |
| `-marm` | 57 |
| `-mfpu=vfpv3-d16` | build fail (NEON intrinsics) |
| `-mfloat-abi=hard` | link fail (softfp FMOD) |
| `-mno-unaligned-access` | 1391 |
| `-fshort-enums` | 2869 |
| `-fomit-frame-pointer` | 1298; 2473 frame-pointer prologues |
| `-fno-inline` | 1219 |
| `-fno-strict-aliasing` | 1371 |
| `-fwrapv` | 1398 |
| `-ffast-math` | 1319 |
| `-fvisibility=hidden` | 57 |
| `-fno-jump-tables` | 1363; 401 `tbb` + 84 `tbh` |
| `-fno-slp-vectorize` | 1492 |
| `-fno-threadsafe-statics` | 1368 |
| `-fno-use-cxa-atexit` | 1356 |
| `-fno-rtti` | 1701 |
| `-fno-exceptions` | build fail |
| `-fno-unwind-tables` | 2878 |
| `-std=c++14` | 1512 |
| `-fno-common` | link fail on duplicate `g_android_back_button_pressed` |
| `-fno-builtin` | 1350 |
| `-fno-unroll-loops` | 2883 |
| `-fno-optimize-sibling-calls` | 1316 |
| `-march=armv7ve` | 1415 |
| `-fno-stack-protector` | 85 `__stack_chk_fail@plt` hits (vendored archives only) vs original 788 |
| libgcc not pulled | original `__aeabi_ldivmod` bytes at `0x19ae54` |
| GCC string from libgcc | libgcc members have no `.comment`; `sha256.o` does |
| linux `libc++abi.a` | 4344 vs 4500 |

## 7. Reconstructed build files

### 7.1 The CMake recipe

```cmake
cmake_minimum_required(VERSION 3.4.1)
project(gof2hdaa C CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(gof2hdaa SHARED
    # the original flat source list, in the original link order:
    #   Classes/*.cpp, Classes/Game/*.cpp,
    #   Classes/Game/ParticleSystems/*.cpp, Classes/Game/MeshMerger/*.cpp,
    #   Classes/Shader/*.cpp, ...
)

target_link_libraries(gof2hdaa
    zlib zip crypto
    log GLESv2 GLESv1_CM EGL android
    ${FMOD_DIR}/libfmodex.so ${FMOD_DIR}/libfmodevent.so
    m dl)
```

Configure with the r18b CMake toolchain. The r18b defaults already give
`armeabi-v7a`, `android-16`, `c++_static`, and Thumb; the three explicit
settings are NEON, C++ features, and Release:

```sh
cmake -S . -B build -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=$NDK/build/cmake/android.toolchain.cmake \
  -DANDROID_ABI=armeabi-v7a \
  -DANDROID_PLATFORM=android-16 \
  -DANDROID_ARM_NEON=ON \
  -DANDROID_STL=c++_static \
  -DANDROID_CPP_FEATURES="rtti exceptions" \
  -DCMAKE_BUILD_TYPE=Release
```

This emits a compile line of the observed shape (the exact `-std` spelling is
codegen-neutral; see §6):

```text
clang++ --target=armv7-none-linux-androideabi16 \
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

and the CMake default link tail:

```text
... FMOD libfmodex.so libfmodevent.so libcrypto_gof2.a -lm \
    -latomic -lm libc++_static.a libc++abi.a libandroid_support.a \
    libunwind.a -lgcc -ldl -lc crtend_so.o
```

The current tree's minimal tail (`libc++abi.a libandroid_support.a libgcc.a
-ldl -lc`) is sha256-identical to this full tail on this source.

### 7.2 The ndk-build equivalent (not the original)

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

Even with `-fno-data-sections`, vanilla ndk-build puts the STL archives
before the shared libraries, pulls `libunwind.a`, and defines `_Unwind_*`
locally. That is why CMake is the parsimonious reconstruction.

## 8. Reproduction commands

```sh
ROOT=/home/debian/GalaxyOnFire2/wt/w5019
O=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
NDK=$ROOT/.cache/ndk/android-ndk-r18b
OBJ=$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump

# baseline
cd $ROOT
cmake --preset default -DGOF2_ORIGINAL_SO=$O \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6

# metadata
readelf -hW $O
readelf -nW $O
readelf -p .comment $O
readelf -AW $O
readelf -dW $O
readelf -SW $O

# .comment decomposition (see §2.2)
python3 - <<'PY'
import struct, subprocess, tempfile, os
def sec(p,name):
    b=open(p,'rb').read()
    e_shoff=struct.unpack_from('<I',b,0x20)[0]; es=struct.unpack_from('<H',b,0x2e)[0]
    n=struct.unpack_from('<H',b,0x30)[0]; si=struct.unpack_from('<H',b,0x32)[0]
    sh=[struct.unpack_from('<IIIIIIIIII',b,e_shoff+i*es) for i in range(n)]
    st=sh[si]; stb=b[st[4]:st[4]+st[5]]
    for s in sh:
        nm=stb[s[0]:stb.find(b'\0',s[0])].decode()
        if nm==name: return b[s[4]:s[4]+s[5]]
O='/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so'
NDK='/home/debian/GalaxyOnFire2/wt/w5019/.cache/ndk/android-ndk-r18b'
crt=sec(NDK+'/platforms/android-16/arch-arm/usr/lib/crtbegin_so.o','.comment')
data=subprocess.check_output(['ar','p','/home/debian/GalaxyOnFire2/wt/w5019/third_party/openssl/libcrypto_gof2.a','sha256.o'])
with tempfile.NamedTemporaryFile(delete=False,suffix='.o') as f: f.write(data); fn=f.name
sha=sec(fn,'.comment'); os.unlink(fn)
print(crt+sha[1:]==sec(O,'.comment'))   # True
PY

# archive member trace / map
cd $ROOT/cmake-build-match
ninja -t commands libgof2hdaa.so | tail -1 > /dev/shm/w5019/linkcmd.txt
python3 - <<'PY'
import shlex
s=open('/dev/shm/w5019/linkcmd.txt').read().strip()
t=shlex.split(s.split(' && ',1)[1])
if '&&' in t: t=t[:t.index('&&')]
i=t.index('-o')+1
t[i]='/dev/shm/w5019/relink_map.so'
t=t[:1]+['-Map','/dev/shm/w5019/link.map','-t']+t[1:]
open('/dev/shm/w5019/relink_map.sh','w').write('#!/bin/sh\n'+' '.join(shlex.quote(x) for x in t)+'\n')
PY
sh /dev/shm/w5019/relink_map.sh > /dev/shm/w5019/gold-trace.txt 2>&1
grep -oE '[^/ ]+\.a\([^)]*\)' /dev/shm/w5019/gold-trace.txt | sort | uniq -c

# linux libc++abi A/B (see §2.5)
unzip -p /dev/shm/w5019/android-ndk-r18b-linux-x86_64.zip \
  android-ndk-r18b/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/libc++abi.a \
  > /dev/shm/w5019/libc++abi_linux.a
# replace the cxxabi token in the canonical gold command, relink, strip, reorder
# then:
python3 tools/verify/verify.py --build-dir /dev/shm/w5019/verify_linuxabi \
  --original $O --ours /dev/shm/w5019/relink_linuxabi.so \
  --objdump $OBJ --fail-on-wrong-type --top 0

# A/B harness (one flag at a time)
cat > /dev/shm/w5019/toolchain_extra.cmake <<'CMAKE'
include("/home/debian/GalaxyOnFire2/wt/w5019/cmake/toolchain.cmake")
set(GOF2_EXTRA_FLAGS "" CACHE STRING "extra matching flags")
if(GOF2_EXTRA_FLAGS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GOF2_EXTRA_FLAGS}" CACHE STRING "" FORCE)
  set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   ${GOF2_EXTRA_FLAGS}" CACHE STRING "" FORCE)
endif()
CMAKE
cmake -S $ROOT -B /dev/shm/w5019/build-fdata -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/dev/shm/w5019/toolchain_extra.cmake \
  -DGOF2_MATCH_OPT=-Oz -DGOF2_EXTRA_FLAGS='-fdata-sections' \
  -DGOF2_ORIGINAL_SO=$O -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build /dev/shm/w5019/build-fdata --target link_so -j6
python3 $ROOT/tools/verify/verify.py --build-dir /dev/shm/w5019/verify-fdata \
  --original $O --ours /dev/shm/w5019/build-fdata/libgof2hdaa.so \
  --objdump $OBJ --fail-on-wrong-type --top 0
```

## 9. Files changed and gate counts

Tracked change: this note only. No `src/`, `cmake/`, `CMakeLists.txt` or
`docs/` change; no `--update-baseline`; `report.json` not committed.

Gate before/after (canonical build): **byte_exact 4500 -> 4500,
linked_exact 4500 -> 4500, size_exact 4524/4524 -> 4524/4524, stub_zero_size 0,
missing 0, wrong_type 0, extra 0, data_exact 843/843, imports 207,
`.text 0x1337c8`, `.rodata 0x41b67`, `.ARM.exidx 0x6f70`,
`.ARM.extab 0x26294 (-4)`, lint CLEAN, drift 0.**
