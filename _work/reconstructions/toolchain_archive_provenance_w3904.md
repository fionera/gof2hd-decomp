# Toolchain/archive provenance and fresh-host reproducibility (w3904)

Status: research note + one build fix, 2026-11-03. Worktree `wt/w3904`, branch
`wave89-w3904`, base `dfc9d7a1`. Scratch is `/dev/shm/w3904`; the tracked
changes are `cmake/DownloadNDK.cmake`, `README.md` (host-support text), and
this note.

## 0. Bottom line

* The original is **NDK r18b, clang 7.0.2, gold 1.12, API 16, armeabi-v7a
  softfp, Thumb-2, NEON, `c++_static`** -- proven by `.comment`,
  `.note.android.ident`, `.note.gnu.gold-version`, `.ARM.attributes` and
  `DT_NEEDED`.
* **Exactly one host-package archive is forced by the original binary: the
  darwin-x86_64 `libc++abi.a`.** Its `abort_message.o` embeds
  `/Volumes/Android/buildbot/.../libcxxabi/src/abort_message.cpp`; the
  linux-x86_64 archive embeds `/buildbot/...` (16 bytes shorter). That 16-byte
  `.rodata.str1.1` difference shifts `.rodata` and drops `byte_exact` from
  4498 to 4344 (`linked_exact` stays 4498).
* **Every other NDK archive is neutral.** `libandroid_support.a` (66 members)
  and `libgcc.a` (1751 members) have byte-identical non-debug sections in both
  host packages; relinking with the linux archives is byte-identical to the
  canonical `.so`. `libc++_static.a` (30 members) and `libunwind.a` (7 members)
  are not pulled by this link; adding either host's archives is byte-identical.
  `libatomic.a` is not pulled either. `crtbegin_so.o`/`crtend_so.o` are
  byte-identical across packages.
* **The shipped APK is the ground truth after GNU
  `arm-linux-androideabi-strip --strip-unneeded`** (ndk-build r18b's default
  `APP_STRIP_MODE`). The 288-byte delta is entirely `.shstrtab` content plus the
  27 section-header `sh_name` fields. `llvm-strip` is not the tool.
* **A fresh Linux checkout did not reproduce the gate before the fix**
  (`byte_exact 4344/4524`, `RATCHET FAIL`). The landed
  `cmake/DownloadNDK.cmake` fix fetches the darwin package once, installs only
  its `libc++abi.a`, and makes a fresh Linux build reproduce
  **4498/4498/4524, `RATCHET PASS`** with the same `.so` sha256 as the canonical
  build.
* ELF link metadata (SONAME, NEEDED order, BIND_NOW/NOW, RELRO, sysv hash,
  5 spare dynamic tags, SHA-1 build-id style, 4 KiB pages, non-exec stack) is
  identical. The only remaining *link-metadata* section-header delta is
  `.rel.plt` `sh_flags`/`sh_info` (`AI`/20 vs `A`/0), which no tested link flag
  changes and which is consistent with the darwin-host gold binary; it is still
  unproven. The other section-header deltas are source residues: `.ARM.extab`
  size -12 and `.bss` `sh_offset` +4 (NOBITS).

## 1. Baseline (canonical build)

```sh
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8
cmake --build cmake-build-match --target lint  -j8
```

```text
verify: byte 4498  linked 4498  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
=== DRIFT: 0 field(s) across 0 class(es) ===
CLEAN ✓ (no new soft violations; hard invariants 0)
```

| artifact | sha256 | size |
|---|---|---:|
| canonical `cmake-build-match/libgof2hdaa.so` | `88aff1154772f836a53b12cc094637deec50026c74db18c1cd59fdced0cbfecd` | 2,187,328 |
| ground truth `_work/bins/android_2.0.16_libgof2hdaa.so` | `d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483` | 2,187,328 |
| APK `lib/armeabi-v7a/libgof2hdaa.so` | `6d38fd767ee3b4f8174c371bce371bfc6397cf76e13c1756938b2a94658e3b58` | 2,187,328 |

Section sizes: `.text 0x1337c8 @ 0x69440`, `.rodata 0x41b67 @ 0x1c9e10`,
`.data 0x296c @ 0x214000`, `.bss 0x69ac @ 0x216970`, `.shstrtab 0x117`,
27 sections. Baseline section-byte diffs vs the original: `.text` 107,487;
`.ARM.exidx` 1,738; `.ARM.extab` size -12; `.dynsym` 324; `.dynstr` 1,268;
`.hash` 133; `.rel.dyn` 74; `.rel.plt` 21; `.note.gnu.build-id` 20. All other
sections, including `.rodata`, `.data`, `.data.rel.ro`, `.got`, `.dynamic`,
`.plt`, `.init_array`, `.fini_array`, `.comment`, `.note.*`,
`.ARM.attributes` and `.shstrtab`, are byte-identical.

## 2. NDK r18b host packages

Both official packages were downloaded from `dl.google.com` and extracted under
`/dev/shm/w3904`:

| package | URL | size | sha256 |
|---|---|---:|---|
| linux-x86_64 | `https://dl.google.com/android/repository/android-ndk-r18b-linux-x86_64.zip` | 557,038,702 | `4f61cbe4bbf6406aa5ef2ae871def78010eed6271af72de83f8bd0b07a9fd3fd` |
| darwin-x86_64 | `https://dl.google.com/android/repository/android-ndk-r18b-darwin-x86_64.zip` | 542,911,996 | `dd6524c3cc91725b5c39370f6deb27d8eee056e2bb2efef0a8008b4ca6d83891` |

Both extract to `android-ndk-r18b/`. The original's `.note.android.ident`
contains `r18b` and build `5063045`, matching these packages. The original's
`.comment` names clang 7.0.2 (`4751641 based on r328903`) and GCC 4.9.x
(`20150123`), matching r18b's clang and the r13b GCC used for vendored
libcrypto.

## 3. Link input map

The canonical link line is:

```text
gold -shared -soname libgof2hdaa.so --build-id -s -z now
  --exclude-libs=libgcc.a --section-ordering-file=.../section_order.txt
  -o libgof2hdaa.so crtbegin_so_renamed.o -L platforms/android-16/arch-arm/usr/lib
  @exidx_markers/link_order.rsp
  --whole-archive libz_gof2.a libzip_gof2.a --no-whole-archive
  -l log -l GLESv2 -l GLESv1_CM -l EGL -l android
  apk_libs/libfmodex.so apk_libs/libfmodevent.so
  libcrypto_gof2.a -l m libc++abi.a libandroid_support.a libgcc.a -l dl -l c
  crtend_so.o
```

| artifact | source | sha256 | linked? | forced? |
|---|---|---|---|---|
| `crtbegin_so.o` | NDK r18b `platforms/android-16/arch-arm/usr/lib` | `68be056ee637c2ef7b7acc899bc075857f7999e4ebd46b8ed2ff6d31ab0d909c` (both hosts) | yes | no (identical) |
| `crtend_so.o` | same | `31dbabfa0740e8a0ce4a3cdd03874d0aa0e4705ab861554243883c5b94a4a31e` (both hosts) | yes | no (identical) |
| `libc++abi.a` | `sources/cxx-stl/llvm-libc++/libs/armeabi-v7a` | linux `6993e991ff2d9e4f45febab2f7758fe293c14d08f2364796c6d63f2b5cc210a6` (1,144,222 B); darwin `9a0f50d22ef53a946b76d689f723b2ca88d2cb0f7e17ee133ab19395a925fadb` (1,147,158 B) | yes | **darwin** |
| `libandroid_support.a` | same | linux `0a16df88199802b713cf57dd493d11a0dd50e5115f8f6780c62bbcaecebb83c3` (438,194 B); darwin `15e22048afbac9260f4e3c6c5be65f895453d774e5bf01024cdf377ac4353d65` (443,086 B) | yes | no (non-debug identical) |
| `libgcc.a` | `toolchains/arm-linux-androideabi-4.9/prebuilt/<host>/lib/gcc/arm-linux-androideabi/4.9.x/armv7-a/thumb` | linux `350de2b4bc4a49783ada2530273f4b8ca3dc668b22b4b803f3211ee8f41a9c51` (6,052,512 B); darwin `3128a559d4604e364be09b5a9d4f6645b6fc7c910307b891006af818404044f5` (6,125,984 B) | yes | no (non-debug identical) |
| `libc++_static.a` | `sources/cxx-stl/llvm-libc++/libs/armeabi-v7a` | linux `ede64d8302c97f1e6d3ab49b977a59ac0a151ec59217cc1934c3ce84ef94f3fc`; darwin `c17f47b6a6195dfaa132249248774616c60b7ee329d91a560afb7f3c6c27192d` | no (no member pulled) | no |
| `libunwind.a` | same | linux `d3c50f56b4f9cb5e6c3955232e076d19c761876dc1b49fc6d6202e0865f2512b`; darwin `50d347fc7361ee1fa9891714c856afbf7d8003163aa10faca06b6d7602e57f57` | no | no |
| `libatomic.a` | `toolchains/.../arm-linux-androideabi/lib/armv7-a/thumb` | host package | no | no |
| `libz_gof2.a` | vendored zlib 1.2.3, built by `third_party/build_match_lib.cmake` | `d9d68f380915827ce1c11c4a6fca6a272f98d85a773c89fdb5f2b0f1690e83f3` | yes (`--whole-archive`) | build, not a host archive |
| `libzip_gof2.a` | vendored libzip 0.9.3 | `916a1f28e5ab379673101fc745fd99892abc673188bb3840aa69e50a3652c3a4` | yes (`--whole-archive`) | build, not a host archive |
| `libcrypto_gof2.a` | OpenSSL 1.1.0l, built with r13b GCC 4.9 | `50662126d068d8b96ece99f83dafd04f975c43263c193eedca5ac5f3fa5595f2` | yes | build, not a host archive |
| `libfmodex.so` | original APK `lib/armeabi-v7a` | `60ec4de090bf442c7b9db41d0731aa22a101db21aab0ae32af1af6a860c3fa81` (920,204 B) | yes (NEEDED) | APK |
| `libfmodevent.so` | original APK `lib/armeabi-v7a` | `b92cd6ff8f8edb3d81023ee69c1f5239aeaf6cb4e89d6505101115189569c97d` (370,660 B) | yes (NEEDED) | APK |

The vendored zlib/libzip/libcrypto are built from pinned sources by the tree,
not taken from either NDK package. Their object code is deterministic for the
pinned compiler/flags; the canonical and fresh builds produce the same archives
(sha256 above). FMOD is host-independent.

## 4. Archive member comparison

Method: `ar t` for member lists; `ar p` for each member; `readelf -SW` plus
`readelf -x` for every section; compare all non-`.debug*`, non-`.symtab`/
`.strtab` sections. `.debug*`/`.symtab` are discarded by the `-s` link and do
not affect the output.

### 4.1 `libc++abi.a` (19 members, same names/order)

| member | linux size | darwin size | non-debug difference |
|---|---:|---:|---|
| `abort_message.o` | 4,896 | 5,008 | `.rodata.str1.1` 115 -> 131 B |
| `cxa_aux_runtime.o` | 12,644 | 12,788 | none |
| `cxa_default_handlers.o` | 24,440 | 24,620 | none |
| `cxa_demangle.o` | 648,152 | 648,408 | none |
| `cxa_exception.o` | 45,704 | 45,880 | none |
| `cxa_exception_storage.o` | 18,964 | 19,160 | none |
| `cxa_guard.o` | 16,500 | 16,660 | none |
| `cxa_handlers.o` | 25,180 | 25,356 | none |
| `cxa_personality.o` | 49,824 | 49,984 | none |
| `cxa_thread_atexit.o` | 20,148 | 20,340 | none |
| `cxa_unexpected.o` | 7,148 | 7,276 | none |
| `cxa_vector.o` | 36,764 | 36,908 | none |
| `cxa_virtual.o` | 4,296 | 4,376 | none |
| `fallback_malloc.o` | 27,940 | 28,116 | none |
| `private_typeinfo.o` | 93,136 | 93,264 | none |
| `stdlib_exception.o` | 20,040 | 20,168 | none |
| `stdlib_new_delete.o` | 23,360 | 23,488 | none |
| `stdlib_stdexcept.o` | 31,632 | 31,776 | none |
| `stdlib_typeinfo.o` | 19,364 | 19,492 | none |

The embedded path strings are:

```text
linux:  /buildbot/src/android/ndk-release-r18/external/libcxx/../../external/libcxxabi/src/abort_message.cpp
darwin: /Volumes/Android/buildbot/src/android/ndk-release-r18/external/libcxx/../../external/libcxxabi/src/abort_message.cpp
```

The original `.so` contains the darwin string verbatim:

```text
/Volumes/Android/buildbot/src/android/ndk-release-r18/external/libcxx/../../external/libcxxabi/src/abort_message.cpp
```

The linux archive's `.rodata.str1.1` is 115 bytes (`0x73`); the darwin archive's is 131
bytes (`0x83`). The 16-byte difference is exactly the `/Volumes/Android` prefix. Every
other non-debug section in every member is byte-identical.

### 4.2 Other archives

| archive | members | member list | non-debug differences |
|---|---:|---|---|
| `libc++_static.a` | 30 | identical names/order | 0/30 members |
| `libandroid_support.a` | 66 | identical names/order | 0/66 members |
| `libunwind.a` | 7 | identical names/order | 3/7 members, `.rodata.str1.1` path strings only (`libunwind.o`, `Unwind-EHABI.o`, `UnwindLevel1-gcc-ext.o`) |
| `libgcc.a` | 1751 | identical names/order | 0/1751 members |
| `libatomic.a` | host package | not linked | not compared for link effect |
| `crtbegin_so.o`, `crtend_so.o` | 1 each | identical | 0 |

`libunwind.a` is not pulled by this link; adding either host's archive is
byte-identical (see §5). `libc++_static.a` is not pulled either. `libgcc.a` and
`libandroid_support.a` are pulled, but their non-debug sections are identical,
so the linux archives are link-equivalent.

## 5. A/B relink: which archive choice is forced

Each variant was relinked with the canonical gold command, regenerating the
exidx markers and section-ordering file for the substituted archive, then run
through the official `tools/verify/verify.py`. `diff .text` is the number of
differing `.text` bytes vs the original.

| variant | byte_exact | linked_exact | size_exact | `.rodata` | `.data` | `.data.rel.ro` | `.text` diff | sha256 |
|---|---:|---:|---:|---:|---|---|---:|---|
| canonical (darwin `libc++abi`) | 4498 | 4498 | 4524/4524 | 269,159 (0 diff) | 10,604 (0) | 12,264 (0) | 107,487 | `88aff115...` |
| linux `libc++abi.a` | **4344** | 4498 | 4524/4524 | 269,143 (**-16**) | 10,604 (6 diff) | 12,264 (108 diff) | 108,246 | `1c9d9396...` |
| linux `libandroid_support.a` | 4498 | 4498 | 4524/4524 | 269,159 (0) | 10,604 (0) | 12,264 (0) | 107,487 | `88aff115...` |
| linux `libgcc.a` | 4498 | 4498 | 4524/4524 | 269,159 (0) | 10,604 (0) | 12,264 (0) | 107,487 | `88aff115...` |
| canonical + linux `libc++_static.a` + `libunwind.a` + `libatomic.a` | 4498 | 4498 | 4524/4524 | 269,159 | 10,604 | 12,264 | 107,487 | `88aff115...` |
| canonical + darwin `libc++_static.a` + `libunwind.a` + `libatomic.a` | 4498 | 4498 | 4524/4524 | 269,159 | 10,604 | 12,264 | 107,487 | `88aff115...` |

Interpretation:

* The darwin `libc++abi.a` is **forced** by the original's `.rodata` content.
  The 16-byte path string shifts `.rodata` and changes the PC-relative
  literal-pool words of 154 functions, hence `byte_exact 4498 -> 4344` while
  `linked_exact` stays 4498 (the verifier masks link-dependent address words).
* `libandroid_support.a` and `libgcc.a` are **not forced**: the linux archives
  produce a byte-identical `.so` (same sha256).
* `libc++_static.a`, `libunwind.a` and `libatomic.a` are **not pulled** by this
  link: adding either host's archives is byte-identical.

## 6. Host tools

| tool | linux r18b | darwin r18b | version |
|---|---:|---:|---|
| `clang`/`clang++` | 53,695,608 B, `af8268675ca343698d3fae4bca21b0563762442204cbbc34555c8ddcf362376a` | 49,137,148 B, `19a36944e1a69fe602b9d47b6dbeee6518e66be496ced789eb5af6bbad83f76e` | `Android (4751641 based on r328903) clang version 7.0.2` (both) |
| `llvm-ar` | 7,704,784 B, `76936928f1cf9f8649e32aebe868f9d9ccd3ca8a79c2dc4e2ae519a8fbcf7804` | 7,816,552 B, `d510889ce5c587b747adc4a274b55c059cd82811f82a78e2c92a6db70fa865bb` | LLVM 7.0.2svn |
| `llvm-strip` | 2,190,728 B, `ab3358e3b7efff343e93c37422a84be254aacc076a0470483705f14f3f010c16` | 2,420,676 B, `da71debe26113996a4cfafe4a4204429c301027080f56910c5434dfcdb5948a7` | LLVM 7.0.2svn |
| `ld.gold` | 7,967,456 B, `6384f94eaf4f86743be84df55081bd8b5526c1584b6d4a3f49341285728b95ca` | 14,259,592 B, `bcd953191b65f1931a7597a53946331278818391802a5c0d45bdcb47a58f459a` | `GNU gold (GNU Binutils 2.27.0.20170315) 1.12` (both) |
| `arm-linux-androideabi-strip` | 1,284,512 B, `a752dfec2ab43b371be7433ce9f4970f13a61e34cd98c31b9826c005d4e7c86b` | 1,351,164 B, `44b9300579972c8650c223ae883ada227655171f4b4f47dabf9ddcf06b2a5167` | `GNU strip (GNU Binutils) 2.27.0.20170315` |

* The darwin clang is a Mach-O binary and cannot be executed natively on this
  Linux host (no Mach-O loader; `qemu-x86_64` rejects it with `Exec format
  error`). Its LLVM revision string and its entire `lib64/clang/7.0.2/include`
  resource directory are byte-identical to the linux package (resource-tree
  sha256 `5d4b89b6df575b0f60be1ca4c1fab400447b759d3cf0b53e647450833de1f32b`
  for both). The two packages' own clang-built prebuilt archives are direct
  evidence: `libc++_static.a` (30 TUs) and `libandroid_support.a` (66 TUs)
  have byte-identical non-debug sections in the linux and darwin packages;
  only `.debug_*`/`.symtab` (which embed host paths) differ. The host clang
  therefore produces identical code for those TUs. The original's 4498
  byte-exact functions are also reproduced by the linux clang, so the host
  clang difference is not observable in codegen for the app TUs either. For
  example, the official verifier reports `InitializeBlowfish` 416/416 bytes
  byte-exact (100%) with the linux clang.
* Both host golds report **gold 1.12**, and the original's
  `.note.gnu.gold-version` (`gold 1.12`) is byte-identical to our output. The
  original's gold is therefore the r18b gold family. The host binaries differ
  in size/hash (host-specific builds), and the only output difference not
  explained by content is `.rel.plt` `sh_flags`/`sh_info` (see §9).
* `llvm-ar` and `llvm-strip` are the same LLVM 7.0.2svn revision in both
  packages. The archive format and strip behavior are host-independent; the
  APK strip evidence (§7) points to GNU strip, not llvm-strip.

## 7. APK strip delta

The APK's `lib/armeabi-v7a/libgof2hdaa.so` is the ground-truth file after a
GNU strip pass. Tested on the ground truth:

| tool/options | result | sha256 / diff |
|---|---|---|
| none (ground truth) | gold `-s` output | `d4c17be6...`, 2,187,328 B |
| `arm-linux-androideabi-strip --strip-all` | **APK exactly** | `6d38fd76...`, 2,187,328 B |
| `arm-linux-androideabi-strip --strip-unneeded` | **APK exactly** | `6d38fd76...`, 2,187,328 B |
| `llvm-strip --strip-all` | different | `f5029403...`, 2,186,780 B, 876 differing bytes |
| `llvm-strip --strip-unneeded` | no-op (same as ground truth) | `d4c17be6...` |
| host `/usr/bin/strip --strip-all` | cannot recognise ARM | unchanged |

The 288 differing bytes between the APK and the ground truth are **entirely**
inside `.shstrtab` (content reordered) and the 27 section-header `sh_name`
fields (4 bytes each). `.shstrtab` size is unchanged at `0x117`; the section
count stays 27. The APK's `.shstrtab` is in section-header order
(`.shstrtab .note.android.ident .note.gnu.build-id .dynsym ...`); the ground
truth's is in gold's Stringpool order (`.init_array .fini_array .ARM.exidx
.text .got ...`).

This is exactly ndk-build r18b's default install strip:
`build/core/add-application.mk:230` sets `APP_STRIP_MODE := --strip-unneeded`,
and `build/core/default-build-commands.mk:185` sets
`TARGET_STRIP = $(TOOLCHAIN_PREFIX)strip`. `llvm-strip --strip-all` additionally
removes `.comment`, `.note.gnu.gold-version` and `.ARM.attributes`, so it is not
the tool. The APK strip step is therefore a GNU `arm-linux-androideabi-strip
--strip-unneeded` (or `--strip-all`; both are identical here because gold
already removed the symbols).

## 8. Fresh-host reproduction

### 8.1 Before the fix

A clean copy of `HEAD` with no `.cache/ndk` was configured on Linux with
`cmake --preset default -DGOF2_ORIGINAL_SO=...` and built with `check`:

```text
verify: byte 4344  linked 4498  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
  FAIL  byte_exact 4498 -> 4344
RATCHET FAIL
```

`.so` sha256 `1c9d939620d911fba08bfb411d234351f6424473ab5a700a887d57080695d9b7`.
The linux package's `libc++abi.a` is the cause (§4.1); everything else is
identical.

### 8.2 After the landed fix

`cmake/DownloadNDK.cmake` now:

1. pins the official linux and darwin r18b zip sha256 values;
2. on a Linux host, downloads the darwin package once, extracts **only**
   `sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/libc++abi.a`, installs it over
   the linux archive, and writes a `.libcxxabi-darwin` marker so subsequent
   configures do not re-download;
3. leaves the macOS path unchanged (the darwin package already contains the
   archive).

A clean copy of `HEAD` + this fix, with no `.cache/ndk`, was configured and
built with `check`:

```text
verify: byte 4498  linked 4498  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`.so` sha256 `88aff1154772f836a53b12cc094637deec50026c74db18c1cd59fdced0cbfecd`,
exactly the canonical build. The installed archive sha256 is
`9a0f50d22ef53a946b76d689f723b2ca88d2cb0f7e17ee133ab19395a925fadb` (darwin).

### 8.3 Fresh-host recipe

```sh
# from a repo-root checkout (so ../_work resolves to the ground-truth tree):
cmake --preset default
cmake --build cmake-build-match --target check -j8

# from an arbitrary copy/worktree (the default ../_work path is relative to the
# source dir, so pass the ground-truth path explicitly):
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
cmake --build cmake-build-match --target check -j8
```

First configure downloads the linux NDK (~531 MB) and, on Linux, the darwin
package (~518 MB) for `libc++abi.a`. The first `gof2_libs` build also downloads
the r13b GCC toolchain (~650 MB) and the pinned zlib/libzip/OpenSSL sources.
Everything is cached under `.cache/ndk` (gitignored). The default
`GOF2_ORIGINAL_SO=../_work/bins/...` assumes the canonical repo-root layout; a
worktree or a copy under `/dev/shm` must pass `-DGOF2_ORIGINAL_SO` or provide a
matching `_work` symlink.

## 9. ELF metadata and link flags

Original and canonical output agree on every observable link-metadata field:

| observable | original | ours | evidence |
|---|---|---|---|
| ELF / machine | ELF32 DYN ARM, EABI5 soft-float | same | `readelf -hW` |
| `DT_SONAME` | `libgof2hdaa.so` | same | `readelf -dW` |
| `DT_NEEDED` order | `liblog, libGLESv2, libGLESv1_CM, libEGL, libandroid, libfmodex, libfmodevent, libm, libdl, libc` | same | `readelf -dW` |
| `DT_FLAGS` / `DT_FLAGS_1` | `BIND_NOW` / `NOW` | same | `readelf -dW` |
| RELRO | `PT_GNU_RELRO` covers `.fini_array .data.rel.ro .init_array .dynamic .got` | same | `readelf -lW` |
| hash style | `.hash` only, no `.gnu.hash` (sysv) | same | `readelf -SW` |
| spare dynamic tags | `.dynamic` 0x148 = 41 entries, 6 `DT_NULL` (1 terminator + 5 spare) | same | `.dynamic` parse |
| build-id | 20-byte SHA-1 note | 20-byte SHA-1 note, different value | `readelf -nW` |
| page size | LOAD align 0x1000 | same | `readelf -lW` |
| stack | `PT_GNU_STACK` RW (non-exec) | same | `readelf -lW` |
| `.init_array` / `.fini_array` | 3 / 2 entries | same | `readelf -SW` |
| `.note.android.ident` | API 16, `r18b`, build `5063045` | byte-identical | `readelf -nW` |
| `.comment` | clang 7.0.2 + GCC 4.9.x | byte-identical | `readelf -p .comment` |
| `.note.gnu.gold-version` | `gold 1.12` | byte-identical | `readelf -nW` |
| `.shstrtab` | gold Stringpool order, 0x117 | byte-identical | `readelf -p .shstrtab` |

The r18b default link lines (captured from a minimal probe with the linux
package; the darwin package emits the same flags with darwin paths) are:

```text
# NDK r18b CMake toolchain, Release, ANDROID_ABI=armeabi-v7a,
# ANDROID_PLATFORM=android-16, ANDROID_STL=c++_static, ANDROID_ARM_NEON=ON
clang --target=armv7-none-linux-androideabi16 --gcc-toolchain=... --sysroot=...
  -fPIC ... -Wl,--exclude-libs,libgcc.a -Wl,--exclude-libs,libatomic.a
  -nostdlib++ --sysroot ... -Wl,--build-id -Wl,--warn-shared-textrel
  -Wl,--fatal-warnings -Wl,--fix-cortex-a8 -Wl,--exclude-libs,libunwind.a
  -L... -Wl,--no-undefined -Wl,-z,noexecstack -Qunused-arguments
  -Wl,-z,relro -Wl,-z,now -shared -Wl,-soname,libt.so -o libt.so
  ... -llog -lGLESv2 -lGLESv1_CM -lEGL -landroid -lm -ldl -latomic -lm

# NDK r18b ndk-build, APP_ABI=armeabi-v7a, APP_STL=c++_static,
# APP_PLATFORM=android-16, APP_OPTIM=release
clang++ -Wl,-soname,libt.so -shared --sysroot=... ...t.o
  libc++_static.a libc++abi.a libandroid_support.a libunwind.a -lgcc
  -Wl,--exclude-libs,libgcc.a -latomic -Wl,--exclude-libs,libatomic.a
  -target armv7-none-linux-androideabi16 ... -Wl,--fix-cortex-a8
  -Wl,--exclude-libs,libunwind.a -Wl,--build-id -nostdlib++ -Wl,--no-undefined
  -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now -Wl,--warn-shared-textrel
  -Wl,--fatal-warnings -L... -llog -lGLESv2 -lGLESv1_CM -lEGL -landroid
  -lm -ldl -ldl -lc -lm -o libt.so
```

ndk-build's material compile-line difference is `-fdata-sections`; the
original's `.rodata`/`.data` layout is the no-`-fdata-sections` layout, so the
tree omits it (the rest of the ndk-build compile line is codegen-neutral).
`--spare-dynamic-tags=5` and `--hash-style=sysv` are gold defaults, not NDK
script flags: `ld.gold --help` says "Dynamic tag slots to reserve (default 5)",
and the original/our `.dynamic` both have 41 entries with 6 `DT_NULL`
(1 terminator + 5 spare), while `.hash` exists and `.gnu.hash` does not.

Link flags and their status:

* **Proven/load-bearing:** `-shared`, `-soname libgof2hdaa.so`, `--build-id`
  (SHA-1), `-s` (gold strip), `-z now` (BIND_NOW/NOW + RELRO order),
  `--exclude-libs=libgcc.a`, zlib/libzip `--whole-archive`,
  `--section-ordering-file` (tree-specific `.text` order), sysv hash (the
  default), 5 spare dynamic tags (the default), 4 KiB pages (the default),
  non-exec stack (the default), no-as-needed (the default; `--as-needed`
  changes the output).
* **Neutral / not distinguishable from the binary:** `--fix-cortex-a8`,
  `--no-undefined`, `--warn-shared-textrel --fatal-warnings`,
  `-z noexecstack`, `-z relro` (implied by `-z now`), `--threads`/
  `--no-threads`, `--discard-all`, `-z max-page-size=4096`,
  `--hash-style=sysv`, `--spare-dynamic-tags=5`, `--no-as-needed`,
  `--build-id=sha1`, `-latomic`, `libc++_static.a`, `libunwind.a`,
  `-nostdlib++`, `-lgcc` vs a direct `libgcc.a`, and
  `--exclude-libs=libatomic.a`/`libunwind.a`. The tree's shorter gold line is
  byte-equivalent to the full r18b CMake/ndk-build default line.
* **Not used:** `--version-script`, `--allow-multiple-definition`,
  `--unresolved-symbols=ignore-all` (gold's shared-library default already
  allows the app's undefined imports), `--gc-sections`, `--icf`,
  `--sort-section`, `--rosegment`, `--no-merge-exidx-entries`,
  `--build-id=md5/none`, `--hash-style=gnu/both`, `--spare-dynamic-tags=0/10`,
  `-z nocombreloc`, `-z max-page-size=65536`, `-z norelro`, `--as-needed`,
  `--no-whole-archive`, `--exclude-libs=ALL`, `--no-strip`.
* **Still unproven:** the `.rel.plt` section-header difference. Original:
  `sh_flags=AI`, `sh_info=20` (`.got`). Ours: `sh_flags=A`, `sh_info=0`.
  Removing `-z now`, removing the section-ordering file, and the other tested
  link-flag variants do not change it. The most likely cause is the host gold
  binary (darwin vs linux), but the darwin gold is a Mach-O executable and was
  not run. `.rel.plt` size is identical; 21 bytes of relocation content differ
  with the symbol order. The per-function gate is unaffected.

## 10. What remains

1. **`.rel.plt` `sh_flags`/`sh_info`** (`AI`/20 vs `A`/0): host-gold
   difference, unproven without running darwin gold. It is the only remaining
   link-metadata section-header delta; `.ARM.extab` size -12 and `.bss`
   `sh_offset` +4 are source/Nobits residues.
2. **`.note.gnu.build-id` value** differs because the `.so` content still
   differs; the SHA-1 style is proven. It will converge only with full byte
   identity.
3. **Full byte identity** is still blocked by the 26 non-byte-exact functions
   (`.text` 107,487 differing bytes), not by the toolchain/archive choice.
4. **Direct darwin-clang A/B** was not possible on this Linux host (Mach-O
   binary, no loader). The 4498 byte-exact functions reproduced by linux clang,
   the identical LLVM revision strings, and the byte-identical resource headers
   are the evidence that host clang choice is codegen-neutral here.
5. **Default `GOF2_ORIGINAL_SO` path** is `../_work/bins/...` relative to the
   source dir; it is correct for a canonical repo-root checkout but not for a
   worktree or a `/dev/shm` copy. Pass `-DGOF2_ORIGINAL_SO` in those cases.

## 11. Reproduction commands (exact)

```sh
ROOT=/home/debian/GalaxyOnFire2/wt/w3904
O=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
L=/dev/shm/w3904/ndk-linux/android-ndk-r18b
D=/dev/shm/w3904/ndk-darwin/android-ndk-r18b

# baseline gate
cd $ROOT
cmake --preset default -DGOF2_ORIGINAL_SO=$O
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8
cmake --build cmake-build-match --target lint  -j8

# package hashes
sha256sum /dev/shm/w3904/android-ndk-r18b-linux-x86_64.zip \
          /dev/shm/w3904/android-ndk-r18b-darwin-x86_64.zip

# libc++abi.a path strings
strings -a $L/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/libc++abi.a |
  grep abort_message.cpp
strings -a $D/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/libc++abi.a |
  grep abort_message.cpp
strings -a $O | grep abort_message.cpp

# archive member comparison (see /dev/shm/w3904/cmp_archives2.py)
python3 /dev/shm/w3904/cmp_archives2.py \
  /dev/shm/w3904/archives/linux/libc++abi.a \
  /dev/shm/w3904/archives/darwin/libc++abi.a

# A/B relink with the linux libc++abi.a
python3 /dev/shm/w3904/relink_alt.py linux_libcxxabi \
  /dev/shm/w3904/archives/linux/libc++abi.a 3

# APK strip delta
python3 - <<'PY'
import zipfile,hashlib
p="/home/debian/GalaxyOnFire2/Original APK/Galaxy on Fire 2™ HD_2.0.16_APKPure.apk"
d=zipfile.ZipFile(p).read("lib/armeabi-v7a/libgof2hdaa.so")
open("/dev/shm/w3904/apk_lib.so","wb").write(d)
print(hashlib.sha256(d).hexdigest(),len(d))
PY
$L/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-strip \
  --strip-unneeded -o /dev/shm/w3904/apk_strip.so $O
cmp /dev/shm/w3904/apk_lib.so /dev/shm/w3904/apk_strip.so && echo "APK == GNU strip --strip-unneeded"

# fresh-host build (no .cache/ndk)
mkdir -p /dev/shm/w3904/fresh && cd /dev/shm/w3904/fresh
git -C $ROOT archive HEAD | tar -x
cp $ROOT/cmake/DownloadNDK.cmake cmake/DownloadNDK.cmake
cmake --preset default -DGOF2_ORIGINAL_SO=$O
cmake --build cmake-build-match --target check -j8
```

## 12. Files changed and gate counts

Tracked changes:

* `cmake/DownloadNDK.cmake` -- pin both r18b package hashes; on Linux, fetch the
  darwin package once and install its `libc++abi.a` (the original's archive),
  with a marker so later configures are offline.
* `README.md` -- build-host text now covers Linux as well as macOS/Rosetta.
* `_work/reconstructions/toolchain_archive_provenance_w3904.md` -- this note.

Gate before/after (canonical build): **byte_exact 4498 -> 4498,
linked_exact 4498 -> 4498, size_exact 4524/4524 -> 4524/4524, stubs 0,
missing 0, wrong_type 0, extra 0, data_exact 843/843, imports 207,
`.text 0x1337c8`, `.rodata 0x41b67`, lint CLEAN, drift 0.**
Fresh Linux host before the fix: `byte_exact 4344/4524` (`RATCHET FAIL`);
after the fix: `4498/4498/4524` (`RATCHET PASS`), same `.so` sha256 as the
canonical build.
