# Toolchain and runtime provenance, wave 120 (w7007)

Status: research note, 2026-11-06. Worktree `/home/debian/GalaxyOnFire2/wt/w7007`,
branch `wave120-w7007`, base `ef7fb9cde`. Scratch is `/dev/shm/w7007`.
**No `src/`, `tools/`, baseline or allowlist change is landed**; this note is the
only commit. The canonical `cmake-build-match` was rebuilt and re-checked in this
worktree; the gate is unchanged.

Canonical gate (measured in this worktree, before and after the pass):

```text
verify: byte 4505  linked 4505  stubs 0  missing 0  extra 0  avg 99.94
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Original: `sha256 d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483`,
2,187,328 B. Canonical build: `sha256 ef82c033e784b335211cd0a459fdcfe41216dd08d418b22cac40ec0c34739843`,
same size. The only differing sections are `.text` (88,216 B), `.ARM.exidx`
(863 B), `.ARM.extab` (109,364 B, size-exact), `.rodata` (+192 B, original is a
strict prefix), `.rel.plt` (6 B) and `.note.gnu.build-id` (20 B); the other 21
sections, including `.comment`, `.note.android.ident`,
`.note.gnu.gold-version`, `.ARM.attributes`, `.dynsym`, `.dynstr`, `.hash`,
`.rel.dyn`, `.got`, `.data.rel.ro`, `.data`, `.bss` and `.shstrtab`, are
byte-identical.

## 0. Bottom line

* The original is **NDK r18b, clang 7.0.2 (`4751641 based on r328903`), gold
  1.12, API 16, armeabi-v7a, Thumb-2, VFPv3, NEONv1, softfp, `c++_static`**,
  built on a **darwin** host. Each of those facts has a positive artifact
  fingerprint below; the r18b and darwin parts are not merely consistent, they
  are forced by bytes.
* **`libc++abi.a` must come from the darwin r18b package.** The original's
  `.rodata` embeds `/Volumes/Android/buildbot/.../abort_message.cpp`; the linux
  r18b archive embeds `/buildbot/...` instead. Swapping in the linux archive
  changes `.text` (785 B), `.rodata` (size −16, 106,502 B of shifted bytes),
  `.dynsym` (195 B), `.data.rel.ro` (108 B) and `.data` (6 B), and drops
  `byte_exact` from 4505 to **4347** (linked stays 4505).
* **All other NDK r18b archives are host-neutral.** The full darwin-archive
  build (crt, libc++abi, libandroid_support, libgcc, headers) is *content
  identical* to the canonical linux-host build: only the 20-byte
  `.note.gnu.build-id` changes, and the gate stays 4505/4505.
* **r18 is disproved.** r18's clang/clang++/ld.gold are byte-identical to
  r18b's, but its `crtbegin_so.o` note says `r18`/`5002713` and its
  `libc++abi.a`/`libandroid_support.a` code differs. A full r18 build gives
  `byte_exact` **3226**, `linked_exact` **4501**, with `.note.android.ident`
  differing in 6 B and `.text` shrinking by 1,964 B.
* **`-g` is load-bearing and `-g2` is the level.** `-g0`, `-g1` and
  `-gline-tables-only` all emit no `llvm.dbg.value` intrinsics and produce the
  *same* code, but it is not the original's: `byte_exact` falls to **2071**
  (`linked_exact` 4252, size-exact 4485; 2,453 functions change bytes and 39
  change size). `-g2`, `-g3`, `-gdwarf-2/3/4` all emit `llvm.dbg.value` and are
  codegen-identical to the canonical `-g` build (4505/4505). The mechanism is
  the debug intrinsics perturbing the `-Oz` backend scheduler/regalloc, not a
  flag spelling difference.
* **The original build-id is not reproducible from the stripped file.** Gold's
  build-id is the SHA-1 of the *unstripped* link output with the 20-byte
  descriptor zeroed. The unstripped file contains full DWARF, including
  `DW_AT_comp_dir` = the build directory, so the build-id moves whenever the
  build directory or any debug-info flag moves, even with identical code. The
  original's `c011810d...` therefore depends on an unobservable build path.
* **Runtime archive code is proven, not inferred.** A link-map-driven check of
  every archive-owned `.text*` section in the original vs the canonical link
  finds 0 mismatches: libandroid_support **1/1**, libc++abi **425/425**,
  libcrypto **3/3**, libgcc **2/2**, zlib **66/66**, libzip **86/86**.
* **FMOD is not code-linked.** `libfmodex.so`/`libfmodevent.so` are the APK's
  prebuilts (GCC 4.4.3, `NEEDED libstdc++.so`); only their `DT_NEEDED` names,
  order and re-export behaviour are observable. They export
  `__aeabi_uldivmod` and `__gnu_ldivmod_helper` but **not** `__aeabi_ldivmod`,
  which is exactly why libgcc's `_aeabi_ldivmod.o` is pulled and its
  `__aeabi_ldivmod` stays local (`--exclude-libs=libgcc.a`).

## 1. Artifacts, hashes and scratch A/B builds

| artifact | path | sha256 | size |
|---|---|---|---:|
| ground-truth `.so` | `_work/bins/android_2.0.16_libgof2hdaa.so` | `d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483` | 2,187,328 |
| APK `lib/armeabi-v7a/libgof2hdaa.so` | `Original APK/Galaxy on Fire 2™ HD_2.0.16_APKPure.apk` | `6d38fd767ee3b4f8174c371bce371bfc6397cf76e13c1756938b2a94658e3b58` | 2,187,328 |
| canonical build | `cmake-build-match/libgof2hdaa.so` | `ef82c033e784b335211cd0a459fdcfe41216dd08d418b22cac40ec0c34739843` | 2,187,328 |
| NDK r18 linux zip | `dl.google.com/android/repository/android-ndk-r18-linux-x86_64.zip` | `c413dd014edc37f822d0dc88fabc05b64232d07d5c6e9345224e47073fdf140b` | 557 MB |
| NDK r18b linux zip | `.../android-ndk-r18b-linux-x86_64.zip` | `4f61cbe4bbf6406aa5ef2ae871def78010eed6271af72de83f8bd0b07a9fd3fd` | 557 MB |
| NDK r18b darwin zip | `.../android-ndk-r18b-darwin-x86_64.zip` | `dd6524c3cc91725b5c39370f6deb27d8eee056e2bb2efef0a8008b4ca6d83891` | 543 MB |

The APK `.so` differs from the ground-truth file in **`.shstrtab` only** (259
B; same 27 sections, same build-id). The APK carries bfd's section-header
`.shstrtab` order; the ground-truth file carries gold's Stringpool order, which
`tools/reorder_shstrtab.py` restores. A raw `strip --strip-all` of a fresh gold
link reproduces the APK `.shstrtab` byte-for-byte, so the APK is the raw
post-strip artifact and the ground-truth file is the reordered target.

## 2. Toolchain identity

| fact | evidence | verdict |
|---|---|---|
| ELF32 DYN ARM EABI5 | `readelf -h`: `Flags 0x5000200` = `EF_ARM_EABI_VER5 \| EF_ARM_ABI_FLOAT_SOFT` | proven |
| API 16 | `.note.android.ident` = `Android\0` + `16` + `r18b` + `5063045` | proven |
| NDK r18b | `.note.android.ident` is byte-identical to r18b `crtbegin_so.o` (`68be056e...`); r18's is `710431dc...` and says `r18`/`5002713` | proven |
| clang 7.0.2 | `.comment` string `Android (4751641 based on r328903) clang version 7.0.2 (...)` | proven |
| gold 1.12 | `.note.gnu.gold-version` = `gold 1.12`; NDK r18b `ld.gold` reports `GNU gold (GNU Binutils 2.27.0.20170315) 1.12` | proven |
| armeabi-v7a | `.ARM.attributes`: `Tag_CPU_name "ARM v7"`, `Tag_CPU_arch v7`, `Tag_CPU_arch_profile Application` | proven |
| Thumb-2 | `.ARM.attributes`: `Tag_THUMB_ISA_use Thumb-2`; every function entry has the Thumb bit | proven |
| VFPv3 / NEONv1 | `.ARM.attributes`: `Tag_FP_arch VFPv3`, `Tag_Advanced_SIMD_arch NEONv1` | proven |
| softfp | ELF flags `EF_ARM_ABI_FLOAT_SOFT`; `.ARM.attributes` `Tag_ABI_HardFP_use Deprecated`, `Tag_ABI_VFP_args` absent | proven |
| static libc++ | no `libc++_shared.so` in `DT_NEEDED`; no `libc++_static.a` member is pulled; no `_ZNSt3__1*` export | proven |
| RTTI + exceptions | `-frtti`; libc++abi `__gxx_personality_v0`, `__cxa_*`, `__dynamic_cast`, `type_info` all present | proven |
| `-fstack-protector-strong` | `__stack_chk_fail` import and stack canaries present | consistent (flag spelling not uniquely observable) |
| `-ffunction-sections` | every function has its own `.text.<name>` in the link map | proven |
| no `-fdata-sections` | `.rodata`/`.data` are not per-object sections in the original | consistent |

`.comment` decomposition is exact, not approximate:

```text
crtbegin_so.o .comment (264 B) + sha256.o .comment[1:] (39 B) == original .comment (303 B)
```

`sha256.o` is the tree's `third_party/openssl/libcrypto_gof2.a(sha256.o)`
(`sha256 63e14fe9b4236d42d63efc8d559ab49f44a205138d0e33d2518270f78d313db4`).
The pulled `libgcc.a` members have no `.comment` at all.

`DT_NEEDED` (order-sensitive, byte-identical to canonical):
`liblog libGLESv2 libGLESv1_CM libEGL libandroid libfmodex libfmodevent libm
libdl libc`; `SONAME libgof2hdaa.so`; `FLAGS BIND_NOW`; `FLAGS_1 NOW`.
`.init_array` has 3 entries, `.fini_array` 2, matching the canonical link.

## 3. NDK host/version A/B

### 3.1 r18b linux vs r18b darwin

| item | linux r18b | darwin r18b | identical? |
|---|---|---|---|
| `source.properties` | `18.1.5063045` | `18.1.5063045` | yes |
| clang/clang++ (host binary) | `af826867...` | (darwin binary, same version string) | same version |
| `ld.gold` | gold 1.12 | gold 1.12 | same version |
| libc++/libc++abi/support headers | tree hash `620a7527...` / `5e5f87dc...` / `1a645177...` | same | yes |
| `crtbegin_so.o` | `68be056e...` | `68be056e...` | yes |
| `crtend_so.o` | `31dbabfa...` | `31dbabfa...` | yes |
| `libc++abi.a` | `6993e991...` | `9a0f50d2...` | **no** (path string) |
| `libandroid_support.a` | `0a16df88...` | `15e22048...` | **archive differs, code identical** |
| `libc++_static.a` | `ede64d83...` | `c17f47b6...` | **archive differs, code identical** |
| `libunwind.a` | `d3c50f56...` | `50d347fc...` | **archive differs, path strings only** |
| `libgcc.a` (`armv7-a/thumb`) | `350de2b4...` | `3128a559...` | **archive differs, non-debug identical** |

Member-level results (the compare tool ignores `.debug*`, `.symtab`, `.strtab`,
`.shstrtab` and compares every other section's type/flags/addr/size/bytes):

| archive | members | differing members | nature of diff |
|---|---:|---:|---|
| `libc++abi.a` | 19 | 1 (`abort_message.o`) | `.rodata.str1.1` 115 vs 131 B (the buildbot path) |
| `libandroid_support.a` | 66 | 0 | fully identical |
| `libc++_static.a` | 30 | 0 | fully identical |
| `libunwind.a` | 7 | 3 | `.rodata.str1.1` path strings only |
| `libgcc.a` | 1751 | 0 | non-debug sections identical |

Full-build A/B (same source tree, same flags, only the archive/crt/header
provider changed):

| variant | `.so` sha256 | byte_exact | linked_exact | sections changed vs canonical |
|---|---|---:|---:|---|
| canonical (linux host + darwin `libc++abi.a`) | `ef82c033...` | 4505 | 4505 | baseline |
| full darwin archives | `541463a0...` | 4505 | 4505 | **only `.note.gnu.build-id` (20 B)** |
| linux r18b `libc++abi.a` only | `7eb52ae8...` | **4347** | 4505 | `.text` 785, `.rodata` −16 (106,502 B), `.dynsym` 195, `.data.rel.ro` 108, `.data` 6, build-id |
| full r18 | `c83fa542...` | **3226** | **4501** | `.note.android.ident` 6, `.text` −1964 (52,452 B), `.ARM.exidx` −8, `.ARM.extab` −44, `.rodata` 2544, `.data.rel.ro` 1496, `.data` 148, `.dynsym` 504, build-id |

Interpretation: **`libc++abi.a` is the only host-sensitive archive**, and it is
forced to darwin by the `.rodata` path string. Every other r18b archive is
host-neutral in code; the full darwin-archive build is byte-identical to the
canonical linux-host build except for the build-id.

### 3.2 r18 vs r18b

| item | r18 | r18b | identical? |
|---|---|---|---|
| `source.properties` | `18.0.5002713` | `18.1.5063045` | no |
| clang | `af826867...` | `af826867...` | **yes** |
| clang++ | `af826867...` | `af826867...` | **yes** |
| `ld.gold` | `6384f94e...` | `6384f94e...` | **yes** |
| libc++/libc++abi/support headers | `620a7527...` / `5e5f87dc...` / `1a645177...` | same | **yes** |
| `crtbegin_so.o` | `710431dc...` (`r18`/`5002713`) | `68be056e...` (`r18b`/`5063045`) | **no** |
| `crtend_so.o` | `31dbabfa...` | `31dbabfa...` | yes |
| `libc++abi.a` | `df393c1f...` | `6993e991...` (linux) | **no** (7 members differ, incl. `cxa_demangle.o`, `cxa_personality.o`, `private_typeinfo.o`) |
| `libandroid_support.a` | `7d5f48b9...` | `0a16df88...` (linux) | **no** (9 members differ, incl. `wchar.o`, `k_exp.o`, `k_rem_pio2.o`) |
| `libgcc.a` (`armv7-a/thumb`) | `350de2b4...` | `350de2b4...` (linux) | **yes** (1751/1751) |

So r18 and r18b share the compiler and linker binaries but not the runtime
archives or crt. The full r18 build collapses the gate to 3226/4501, so r18b is
proven.

## 4. Runtime archives: code-byte identity

The canonical link map attributes every archive-owned `.text*` section to its
archive member. Comparing the original's bytes at those linked addresses with
the canonical's bytes gives a complete, address-exact check of the pulled code:

| archive | archive-owned `.text*` sections | exact | exported functions compared | exact |
|---|---:|---:|---:|---:|
| `libandroid_support.a` | 1 | 1 | — | — |
| `libc++abi.a` | 425 | **425** | 29 | 29 |
| `libcrypto_gof2.a` | 3 | **3** | 23 | 23 |
| `libgcc.a` | 2 | **2** | — | — |
| `libz_gof2.a` | 66 | **66** | 47 | 47 |
| `libzip_gof2.a` | 86 | **86** | 74 | 74 |

Pulled members (from the link map):

* `libc++abi.a`: `abort_message.o`, `cxa_default_handlers.o`, `cxa_demangle.o`,
  `cxa_exception.o`, `cxa_exception_storage.o`, `cxa_guard.o`,
  `cxa_handlers.o`, `cxa_personality.o`, `cxa_virtual.o`, `fallback_malloc.o`,
  `private_typeinfo.o`, `stdlib_exception.o`, `stdlib_new_delete.o`,
  `stdlib_typeinfo.o` (14 of 19).
* `libandroid_support.a`: `posix_memalign.o` only.
* `libgcc.a`: `_aeabi_ldivmod.o`, `_dvmd_lnx.o` only.
* `libc++_static.a`, `libunwind.a`, `libatomic.a`: **no members pulled** (0
  occurrences in the link map).

Manual byte witnesses (original vs canonical, all exact):

| symbol | address | size | exact |
|---|---:|---:|---|
| `posix_memalign` | `0x19ae14` | 62 | yes |
| `__aeabi_ldivmod` | `0x19ae54` | 56 | yes |
| `__aeabi_ldiv0` / `__aeabi_idiv0` | `0x19ae8d` | 12 | yes |
| `abort_message` | `0x193e64` | 96 | yes |
| `__cxa_demangle` | `0x194030` | 568 | yes |
| `__gxx_personality_v0` | `0x1924e0` | 348 | yes |
| `__dynamic_cast` | `0x19362c` | 208 | yes |
| `_ZnwjSt11align_val_t` | `0x193d28` | 128 | yes |
| `SHA256_Update` | `0x18f838` | 272 | yes |
| `CRYPTO_memcmp` | `0x18feb0` | 56 | yes |
| `inflate` | `0x186b18` | 4360 | yes |
| `zip_close` | `0x18ccc4` | 2532 | yes |

### 4.1 libcrypto

The tree's `third_party/openssl/libcrypto_gof2.a` is built from
`openssl-1.1.0l`; the archive contains `sha256.o`, `armv4cpuid.o`,
`sha256-armv4.o`. All 23 exported functions are byte-identical, and
`sha256.o`'s `.comment` is the source of the original's GCC string. The exact
patch letter (1.1.0l vs another 1.1.0/1.1.1 patch) is **not observable**: no
OpenSSL version string is linked, so the shipped bytes cannot name the patch
level. `1.1.0l` is the only *consistent* choice, not a code-proven one.

### 4.2 zlib and libzip

`zlibVersion` (`0x18a6f0`) returns the literal at `0x1e6495` = `"1.2.3"`;
`zlibCompileFlags` returns `0x55`. The original also contains `"1.2.7"` at
`0x1e650f`, which is the NDK sysroot `zlib.h` `ZLIB_VERSION` that libzip was
compiled against (`deflateInit2_`/`inflateInit2_` pass it), not the linked zlib
code version. All 66 zlib `.text*` sections and all 47 exported zlib functions
are byte-identical to the tree's `libz_gof2.a` (zlib 1.2.3).

`_zip_nerr_str` (`0x20aaf8`) is **24** in both the original and canonical,
which brackets libzip 0.9.3 (`ZIP_ER_DELETED == 23`, indices 0..23). All 86
libzip `.text*` sections and all 74 exported libzip functions are
byte-identical to the tree's `libzip_gof2.a`.

### 4.3 FMOD

`libfmodex.so` (`60ec4de090bf442c7b9db41d0731aa22a101db21aab0ae32af1af6a860c3fa81`,
920,204 B) and `libfmodevent.so`
(`b92cd6ff8f8edb3d81023ee69c1f5239aeaf6cb4e89d6505101115189569c97d`,
370,660 B) are byte-identical to the APK's `lib/armeabi-v7a/` prebuilts. Both
are ELF32 ARM, `.comment` `GCC: (GNU) 4.4.3`, `NEEDED libstdc++.so`. They are
shared libraries, so no FMOD code is copied into the `.so`; only the
`DT_NEEDED` names/order and the re-export behaviour are observable. They export
`__aeabi_uldivmod` and `__gnu_ldivmod_helper` but not `__aeabi_ldivmod`; the
latter is therefore resolved from libgcc, which is why `_aeabi_ldivmod.o` is
pulled and `__aeabi_ldivmod` stays local under `--exclude-libs=libgcc.a`.

## 5. `-g` and debug-info-only flags

All variants below are full builds from the same tree, with the flag appended
after the canonical `-g`. The gate numbers are from the per-function verifier
(`byte_exact` / `linked_exact` out of 4524).

| variant | `.so` sha256 | byte | linked | `.text` diff | `.dynsym` diff | build-id |
|---|---|---:|---:|---:|---:|---|
| canonical (`-g`) | `ef82c033...` | 4505 | 4505 | 0 | 0 | `ef0c2e3d...` |
| `-g0` | `f5cd5bd6...` | **2071** | **4252** | 802,984 | 3,683 | `837849e2...` |
| `-g1` | `0a84972c...` | **2071** | **4252** | 802,984 | 3,683 | `6f2dff50...` |
| `-g2` | `a7c0a0f2...` | 4505 | 4505 | 0 | 0 | `5d653a55...` |
| `-g3` | `1877fd59...` | 4505 | 4505 | 0 | 0 | `1cab8e69...` |
| `-gdwarf-2` | `7d04667f...` | 4505 | 4505 | 0 | 0 | `7de24591...` |
| `-gdwarf-3` | `363bbe02...` | 4505 | 4505 | 0 | 0 | `1a88f0ba...` |
| `-gdwarf-4` | `19f70762...` | 4505 | 4505 | 0 | 0 | `f19833ac...` |
| `-gline-tables-only` | `b55b35ea...` | **2071** | **4252** | 802,984 | 3,683 | `cf4e2036...` |
| `-fdebug-prefix-map=<wt>=/src` | `27fc0494...` | 4505 | 4505 | 0 | 0 | `d129582d...` |
| `-Xclang -fdebug-compilation-dir -Xclang /tmp/gof2dbg` | `377f3a7f...` | 4505 | 4505 | 0 | 0 | `796369fd...` |

Key observations:

* **`-g0`, `-g1` and `-gline-tables-only` are codegen-identical to each other
  and *not* to the original.** They change 2,453 of 4,524 function bodies and
  change 39 function sizes (size-exact 4485). The `.text` size is unchanged
  (1,259,464 B), so this is instruction selection/scheduling, not layout.
* **`-g2`, `-g3` and `-gdwarf-2/3/4` are codegen-identical to the canonical
  `-g` build** (`.text` and `.dynsym` diff 0; gate 4505/4505).
* The mechanism is the `llvm.dbg.value` intrinsic. In `VirtualInput.cpp`, `-g`
  emits 138 `llvm.dbg.value` calls; `-g0`, `-g1` and `-gline-tables-only` emit
  0. The IR bodies are otherwise identical; the backend's `-Oz` scheduler /
  register allocator produces a different (but same-size) instruction order
  when the debug intrinsics are present. Example: `SetKeyCode` schedules
  `mov r8,r2; mov r9,r1; mov sl,r0` before the `cbz` under `-g`, and after it
  under `-g0`.
* `-ffile-prefix-map` and `-fmacro-prefix-map` are **rejected by clang 7.0.2**
  (`unknown argument`); `-fdebug-compilation-dir=...` is also rejected as a
  driver option and must be passed as
  `-Xclang -fdebug-compilation-dir -Xclang <dir>`.
* `-fdebug-prefix-map` and `-fdebug-compilation-dir` are codegen-neutral; they
  only change the debug info and therefore the build-id.

## 6. Build-id

Gold's build-id is the SHA-1 of the **unstripped** gold output with the 20-byte
descriptor zeroed. Direct check on a fresh relink:

```text
unstripped sha256            faf9e254cc3a8730faa9dfaa2f71b575843a3599a3056c7cd0f6ec4ff763b8c9
descriptor                   c24fdd0bfafe15125309575824cbd89d02143a9e
SHA1(file with descriptor 0) c24fdd0bfafe15125309575824cbd89d02143a9e
```

After GNU `strip --strip-all` the descriptor is unchanged (`c24fdd0b...`), even
though the stripped file's own SHA-1 is different. Therefore:

* The build-id is **not derivable from the stripped `.so`**.
* The unstripped file contains full DWARF. `DW_AT_comp_dir` is the build
  directory (e.g. `/home/debian/GalaxyOnFire2/wt/w7007/cmake-build-match`), and
  `-fdebug-prefix-map` / `-fdebug-compilation-dir` rewrite the paths. Any of
  those changes moves the build-id even when `.text` is byte-identical.
* The canonical build is deterministic in its own build directory: re-running
  `link_so` reproduces `ef82c033...` exactly.
* The original's build-id is `c011810da45777b83ed721b2a87d517efed0d310`. Its
  build directory and exact debug-info flags are unobservable, so the original
  build-id is **unreproducible from the shipped artifact** (not a gate item).

## 7. Proven / disproven / unobservable

### Proven by code- or data-byte identity

* NDK **r18b** (not r18): `.note.android.ident` byte-identical to r18b
  `crtbegin_so.o`; r18 note differs; r18 runtime code differs.
* clang **7.0.2** and gold **1.12**: `.comment` and
  `.note.gnu.gold-version` strings; r18b clang/gold binaries.
* **API 16**, **armeabi-v7a**, **Thumb-2**, **VFPv3**, **NEONv1**, **softfp**:
  `.note.android.ident`, `.ARM.attributes`, ELF flags.
* **darwin `libc++abi.a`**: `/Volumes/Android/.../abort_message.cpp` in
  `.rodata`; the linux archive changes `.text`/`.rodata`/`.dynsym` and drops
  byte_exact to 4347.
* **static libc++**: no `libc++_shared` `DT_NEEDED`, no `libc++_static` member
  pulled, no `_ZNSt3__1*` export.
* **libandroid_support.a**, **libgcc.a**, **libcrypto_gof2.a**,
  **libz_gof2.a**, **libzip_gof2.a** code: every pulled `.text*` section is
  byte-identical (1/1, 2/2, 3/3, 66/66, 86/86).
* **zlib 1.2.3** (`zlibVersion` returns `"1.2.3"`) and **libzip 0.9.3**
  (`_zip_nerr_str == 24`).
* **FMOD libs are the APK prebuilts** (byte-identical hashes), GCC 4.4.3,
  `NEEDED libstdc++.so`; `__aeabi_ldivmod` is absent, so libgcc supplies it.
* **`-g2`-class debug info is required**: `-g0`/`-g1`/`-gline-tables-only`
  change 2,453 function bodies and drop byte_exact to 2071.

### Disproven

* **r18** (full build 3226/4501; `.note.android.ident` differs; libc++abi and
  libandroid_support code differ).
* **linux r18b `libc++abi.a`** (byte_exact 4347; `.rodata` path string differs).
* **`-g0` / `-g1` / `-gline-tables-only`** (codegen differs; 2071/4252).
* **`-ffile-prefix-map` / `-fmacro-prefix-map`** (clang 7.0.2 rejects them).

### Only consistent, not code-proven

* The exact **libcrypto patch letter** (1.1.0l): no version string is linked,
  so the shipped bytes cannot name the patch level.
* The exact **darwin host** beyond `libc++abi`: `libandroid_support.a`,
  `libc++_static.a`, `libunwind.a`, `libgcc.a` and the crt are host-neutral in
  code, so the host package cannot be distinguished from them.
* The exact **NDK host package** for libgcc/crt/libandroid_support: linux and
  darwin archives are non-debug identical.
* The exact **`-g` level** beyond `-g2`: `-g2`, `-g3` and `-gdwarf-2/3/4` are
  codegen-identical; the DWARF version is stripped away.
* The exact **debug-prefix flags**: they are codegen-neutral and only move the
  build-id.
* The exact **FMOD build**: only the prebuilt hashes, `GCC 4.4.3` string and
  `NEEDED` set are observable.
* The exact **zlib/libzip source tarball hashes**: only the linked code and the
  version/error-count witnesses are observable.

### Unobservable

* The original **build-id** (SHA-1 of an unstripped file that no longer exists;
  depends on the original build directory and debug info).
* The original **build directory** (`DW_AT_comp_dir` was stripped).
* The original **debug info** (`.debug_*` stripped; 27 sections, no `.symtab`).
* The original **source paths** beyond the single libc++abi buildbot string.
* The original **link command line** (inferred from the output; the canonical
  single plain gold link reproduces every section except the known residuals).
* The original **archive file hashes** (only the linked member code/data is
  observable; `ar` member order/timestamps are not).

## 8. Reproduction

```sh
# canonical
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check

# A/B: append a flag after the canonical -g
cmake -S . -B /dev/shm/w7007/build-g0 \
  -DCMAKE_TOOLCHAIN_FILE=/dev/shm/w7007/toolchain-extra.cmake \
  -DGOF2_EXTRA_FLAGS=-g0 \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build /dev/shm/w7007/build-g0 --target verify
```

`/dev/shm/w7007/toolchain-extra.cmake` includes the canonical
`cmake/toolchain.cmake` and appends `GOF2_EXTRA_FLAGS` to both `CMAKE_C_FLAGS`
and `CMAKE_CXX_FLAGS`. The r18 / darwin / linux-libc++abi variants use
`toolchain-r18.cmake`, `toolchain-r18b-darwin-archives.cmake` and
`toolchain-r18b-linux-libcxxabi.cmake` in the same scratch directory.
