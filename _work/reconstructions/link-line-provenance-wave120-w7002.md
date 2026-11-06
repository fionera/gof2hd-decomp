# Link-line provenance and archive/member selection, wave 120 (w7002)

Status: independent research note, 2026-11-06. Worktree
`/home/debian/GalaxyOnFire2/wt/w7002`, branch `wave120-w7002`, base
`ef7fb9cde`. Scratch: `/dev/shm/w7002`. **No `src/`, `tools/`, baseline or
allowlist change is landed.** This note is the only commit.

Ground truth:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(ELF32 ARM, armeabi-v7a softfp, 2,187,328 B,
sha256 `d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483`).
Canonical build: `cmake-build-match/libgof2hdaa.so` (2,187,328 B,
sha256 `23703a684e63654a1489b63ce57a9fe56e1b120858f3151cfe816579992cd939`).
Baseline gate: `byte_exact 4505/4524`, `linked_exact 4505/4524`,
`size_exact 4524/4524`, `data_exact 843/843`, imports `207`, lint `CLEAN`,
drift `0`, `RATCHET PASS`.

## 0. Bottom line

* The checked-in gold link line is **independently re-derived and confirmed**.
  It is the section-byte-consistent recipe among the 78 link variants measured
  here: every non-neutral alternative changes the ELF, and no tested flag
  improves `4505/4505`.
* The exact link is a single plain gold 1.12 shared link:
  `-shared -soname libgof2hdaa.so --build-id -z now --exclude-libs=libgcc.a`,
  followed by GNU `strip --strip-all` and `tools/reorder_shstrtab.py`.
* Input order is **crtbegin, 192 app objects, whole-archive zlib+libzip,
  `-l log -l GLESv2 -l GLESv1_CM -l EGL -l android`, FMOD, libcrypto, `-l m`,
  libc++abi, libandroid_support, libgcc, (decomp-only mission table),
  `-l dl -l c`, crtend**.
* The original's observable ELF fixes the archive **member** set as well:
  zlib 11/11 members, libzip 54/54, libcrypto 3/3, libc++abi 16/19,
  libandroid_support 1/66 (`posix_memalign.o`), libgcc 14/1751. The exported
  symbol set (5,370), exact section sizes (apart from the documented +192 B
  decomp-only `.rodata` table) and exact `.dynsym`/`.dynstr`/`.hash` bytes are
  consistent with this set and no other tested set.
* **Proven by the binary:** gold 1.12; `--build-id` SHA-1; `-z now`;
  `--exclude-libs=libgcc.a`; `--whole-archive` for zlib+libzip; sysv hash;
  `-z combreloc`; `--no-as-needed`; `-z max-page-size=4096`;
  `--spare-dynamic-tags=5`; `-lm -ldl -lc` once in the canonical position;
  FMOD before libcrypto and before `-lm`; libcrypto before `-lm`;
  libc++abi before libandroid_support before libgcc; GNU strip (not gold `-s`);
  no ICF/GC/section sorting/rosegment/`-O2`.
* **Unobservable (byte-identical):** `--no-undefined`, `--fix-cortex-a8`,
  `-z noexecstack`, `-z relro`, `-z combreloc` spelling, `--hash-style=sysv`
  spelling, `--no-as-needed` spelling, `-z max-page-size=4096`,
  `-z common-page-size=4096` (and 65536), `--spare-dynamic-tags=5`,
  `--build-id=sha1`, `--build-id=tree`, `--exclude-libs=libatomic.a`,
  `--exclude-libs=libunwind.a`, `--warn-shared-textrel`, `--fatal-warnings`,
  `-O1`, the full ndk-build and CMake default link sets, `libc++_static.a`
  before/after `libc++abi.a`, `--whole-archive` around libcrypto, and
  `crtend_so.o` (present or absent).
* **Disproven:** gold `-s`; `-z lazy`; `-z norelro`; `-z nocombreloc`;
  `--hash-style=gnu`/`both`; `--as-needed`; `-z max-page-size=65536`;
  `--spare-dynamic-tags=0/10`; `--build-id=none`/`md5`; no
  `--exclude-libs=libgcc.a`; no whole-archive; zlib-only or libzip-only;
  `--gc-sections`; `--icf=safe`; `--sort-section=name`; `--rosegment`;
  `-O2`; all tested alternative archive positions except the trailing
  empty-input positions noted below.

## 1. Method

The exact Ninja command was extracted with:

```sh
ninja -C cmake-build-match -t commands libgof2hdaa.so | tail -1
```

It is a five-segment shell command: `cd`, gold, GNU strip, `reorder_shstrtab`,
`rm`. A harness (`/dev/shm/w7002/relink.py`) re-runs the exact gold/strip/
reorder chain with one token mutation at a time; the canonical variant
reproduces the checked-in `.so` byte-for-byte
(sha256 `23703a68...`). Each variant was measured with:

* the official `tools/verify/verify.py` (`--top 0`, `--fail-on-wrong-type`);
* an independent ELF32 parser (`/dev/shm/w7002/elfdiff.py`) for section sizes,
  section bytes, dynamic tags, NEEDED order, dynsym/dynstr order,
  `.rel.plt`/`.rel.dyn`, program headers and build-id;
* a fast `byte_exact` counter that compares the raw bytes at each common
  exported function's `(st_value, st_size)`. It is **validated against the
  official verifier on all 78 variants: 78/78 exact matches** (e.g. baseline
  4505, `-z lazy` 2932, `--gc-sections` 2176, `--icf=safe` 1347).

`linked_exact` below is always the official verifier's number.

## 2. Original ELF evidence

### 2.1 Toolchain and ABI fingerprints

| observable | original value | conclusion |
|---|---|---|
| `.note.gnu.gold-version` | `gold 1.12` | NDK r18b gold |
| `.note.android.ident` | API `16`, NDK `r18b`, build `5063045` | NDK r18b, `android-16` |
| `.comment` | clang 7.0.2 (`Android (4751641 based on r328903)`) then `GCC: (GNU) 4.9.x 20150123 (prerelease)` | app TUs clang 7.0.2; libcrypto `sha256.o` GCC 4.9 |
| ELF header | `ELF32 DYN ARM EABI5`, `e_flags 0x5000200` | armeabi-v7a, softfp |
| `.ARM.attributes` | ARM v7, Thumb-2, VFPv3, NEONv1, GOT-indirect, wchar_t 4, enum int, 8-byte align, unaligned v6 | `-march=armv7-a -mthumb -mfpu=neon -mfloat-abi=softfp -fPIC` |
| section count | 27 | plain gold shared link, no version script |

The `.comment` section is byte-identical between the original and the
canonical build. The GCC string is contributed by the vendored libcrypto
`sha256.o`; the clang string by the app/crt objects.

### 2.2 Dynamic tags, NEEDED, RELRO, hash, build-id

Original dynamic tags (all byte-identical to the canonical build):

```text
PLTGOT 0x21068c   PLTRELSZ 0x72d0   JMPREL 0x5751c   PLTREL REL
REL 0x504d4       RELSZ 0x7048      RELENT 8         RELCOUNT 1162
SYMTAB 0x1f0      SYMENT 16         STRTAB 0x15e90   STRSZ 0x2e318
HASH 0x441a8      SONAME libgof2hdaa.so
FINI_ARRAY 0x20cba0 (8 B)           INIT_ARRAY 0x20fb98 (12 B)
FLAGS BIND_NOW                     FLAGS_1 NOW
VERSYM 0x4d8e4    VERDEF 0x50478    VERDEFNUM 1
VERNEED 0x50494   VERNEEDNUM 2
```

`DT_NEEDED` order (original == canonical):

```text
liblog.so libGLESv2.so libGLESv1_CM.so libEGL.so libandroid.so
libfmodex.so libfmodevent.so libm.so libdl.so libc.so
```

This order is the direct fingerprint of the link-line order: system `-l`
libs first, then FMOD, then `-l m`, then `-l dl -l c`. Moving FMOD after
`libc++abi` moves `libm` ahead of FMOD in `DT_NEEDED` (see §4.3), so the
original's FMOD-before-`libm` order is a hard constraint.

Program headers (original == canonical):

```text
PHDR R       LOAD R E 0x0..0x20b977      LOAD RW 0x20cba0..0x214000
DYNAMIC RW   NOTE R    GNU_STACK RW (flags 6, align 16)
EXIDX R      GNU_RELRO RW 0x20cba0..0x214000
```

`PT_GNU_RELRO` covers `.fini_array .data.rel.ro .init_array .dynamic .got`;
`.got` (0x20fcf4) is before `.data` (0x214000). This is the `-z now`
layout. `PT_GNU_STACK` is `RW`, i.e. `-z noexecstack` (default).

The hash style is SysV: `.hash` is `SHT_HASH`, size `0x973c`, and there is
**no** `.gnu.hash`. `.rel.plt` is `SHT_REL` with `SHF_INFO_LINK` and
`sh_info=20`; `.rel.dyn` is `SHT_REL`. `.rel.dyn` has 3,593 entries
(1,817 `R_ARM_ABS32`, 1,162 `R_ARM_RELATIVE`, 614 `R_ARM_GLOB_DAT`);
`.rel.plt` has 3,674 `R_ARM_JUMP_SLOT` entries. `.rel.dyn` is
entry-for-entry identical to the canonical build.

Build-id: `.note.gnu.build-id` is a 20-byte SHA-1 descriptor,
`c011810da45777b83ed721b2a87d517efed0d310`. The original's build-id is **not**
the SHA-1 of the shipped stripped file with the descriptor zeroed
(`d7c05dc5e9804023d8786147addc48537b0bee6d`). It is the SHA-1 of the
unstripped gold output with the descriptor zeroed: our own unstripped gold
output reproduces its stored build-id exactly (`c2df62ad...`). This is the
proof that the original was linked **without** `-s` and then GNU-stripped.

### 2.3 Section table (original vs canonical)

`diffB` is the number of differing bytes when the size matches. The canonical
build is the checked-in `cmake-build-match/libgof2hdaa.so`.

| section | orig size | canon size | orig addr | canon addr | diffB |
|---|---:|---:|---:|---:|---:|
| `.note.android.ident` | 152 | 152 | 0x134 | 0x134 | 0 |
| `.note.gnu.build-id` | 36 | 36 | 0x1cc | 0x1cc | 20 |
| `.dynsym` | 89,248 | 89,248 | 0x1f0 | 0x1f0 | 0 |
| `.dynstr` | 189,208 | 189,208 | 0x15e90 | 0x15e90 | 0 |
| `.hash` | 38,716 | 38,716 | 0x441a8 | 0x441a8 | 0 |
| `.gnu.version` | 11,156 | 11,156 | 0x4d8e4 | 0x4d8e4 | 0 |
| `.gnu.version_d` | 28 | 28 | 0x50478 | 0x50478 | 0 |
| `.gnu.version_r` | 64 | 64 | 0x50494 | 0x50494 | 0 |
| `.rel.dyn` | 28,744 | 28,744 | 0x504d4 | 0x504d4 | 0 |
| `.rel.plt` | 29,392 | 29,392 | 0x5751c | 0x5751c | 6 |
| `.plt` | 44,108 | 44,108 | 0x5e7ec | 0x5e7ec | 0 |
| `.text` | 1,259,464 | 1,259,464 | 0x69440 | 0x69440 | 88,216 |
| `.ARM.exidx` | 28,528 | 28,528 | 0x19cc08 | 0x19cc08 | 863 |
| `.ARM.extab` | 156,312 | 156,312 | 0x1a3b78 | 0x1a3b78 | 109,364 |
| `.rodata` | 269,159 | 269,351 | 0x1c9e10 | 0x1c9e10 | size +192 |
| `.fini_array` | 8 | 8 | 0x20cba0 | 0x20cba0 | 0 |
| `.data.rel.ro` | 12,264 | 12,264 | 0x20cbb0 | 0x20cbb0 | 0 |
| `.init_array` | 12 | 12 | 0x20fb98 | 0x20fb98 | 0 |
| `.dynamic` | 328 | 328 | 0x20fba4 | 0x20fba4 | 0 |
| `.got` | 17,164 | 17,164 | 0x20fcf4 | 0x20fcf4 | 0 |
| `.data` | 10,604 | 10,604 | 0x214000 | 0x214000 | 0 |
| `.bss` | 27,052 | 27,052 | 0x216970 | 0x216970 | NOBITS |
| `.comment` | 303 | 303 | 0 | 0 | 0 |
| `.note.gnu.gold-version` | 28 | 28 | 0 | 0 | 0 |
| `.ARM.attributes` | 56 | 56 | 0 | 0 | 0 |
| `.shstrtab` | 279 | 279 | 0 | 0 | 0 |

The only section-size delta is `.rodata` +192 B: the decomp-only
`Status::missionCompleted` rank table (`tools/mission_case_table.cpp`) is the
last `.rodata` input and sits at `0x20b977`, the first byte past the original
`.rodata` image. Every original `.rodata` byte is identical. `.bss` is
`NOBITS`; its file bytes are linker padding.

### 2.4 `.init_array` / `.fini_array`

Both arrays are byte-identical between the original and the canonical build.

| array | entry | resolved symbol |
|---|---|---|
| `.init_array` | `0x69441` | `_GLOBAL__sub_I_Engine.cpp` |
| `.init_array` | `0x694c1` | `_GLOBAL__sub_I_NdkAutosave.cpp` |
| `.init_array` | `0x694dd` | `_GLOBAL__sub_I_ParticleSettings.cpp` |
| `.fini_array` | `0x69548` | `__on_dlclose_late` |
| `.fini_array` | `0x69534` | `__on_dlclose` |

The names are resolved from the unstripped canonical output's `.symtab`; the
addresses and 12/8-byte contents are identical in the original.

### 2.5 Symbols and relocations

* `.dynsym`: 5,578 entries (5,370 defined exports + 207 imports + null).
  **Every field of every entry is identical** between the original and the
  canonical build (`st_name`, `st_value`, `st_size`, `st_info`, `st_other`,
  `st_shndx`). `.dynstr` and `.hash` are byte-identical.
* Import set: 207 names, identical; export set: 5,370 names, identical.
* `.rel.dyn`: 3,593 entries, entry-for-entry identical.
* `.rel.plt`: 3,674 entries; 3,671 identical. The only difference is a
  3-slot rotation at slots 3329–3331:

  ```text
  original: MenuTouchWindow::inCinematicMode, MGame::setCinematicMode, Hud::resetAnalogStick
  canonical: Hud::resetAnalogStick, MenuTouchWindow::inCinematicMode, MGame::setCinematicMode
  ```

  This is a 6-byte source-body artifact of `MGame::OnTouchEnd`'s cold-block
  layout, not a link-order difference. The first-reference order is otherwise
  exactly the canonical object order.
* First exported function: `Java_net_fishlabs_gof2hdallandroid2012_ToJNI_BackButtonPressed`
  at `0x69578`; last: `__cxa_demangle` at `0x194030`; both identical.
* The app object list is `tools/source_order.txt`: 192 `src/` TUs in the
  original TU order.

### 2.6 Post-link strip normalization

The original has bfd-normalized section headers but gold's Stringpool
`.shstrtab` order. A gold `-s` link alone does **not** reproduce the headers:

| field | original | gold `-s` only |
|---|---|---|
| `.rel.plt` `sh_flags` | `66` (`SHF_ALLOC\|SHF_INFO_LINK`) | `2` |
| `.rel.plt` `sh_info` | `20` | `0` |
| `.init_array` `sh_entsize` | `4` | `0` |
| `.fini_array` `sh_entsize` | `4` | `0` |
| `.bss` `sh_offset` | `2,185,580` | `2,185,584` |

The canonical pipeline is gold (no `-s`) → GNU `strip --strip-all` → 
`tools/reorder_shstrtab.py`. The GNU strip normalizes the headers; the
reorder restores gold's `.shstrtab` Stringpool order (bfd rewrites
`.shstrtab` into section-header order). The resulting 27 section headers are
byte-identical to the original's.

## 3. Reconstructed link line

Paths abbreviated: `$NDK` = `.cache/ndk/android-ndk-r18b`, `$OBJ` = the 192
app objects in `tools/source_order.txt` order, `$APK` =
`/home/debian/GalaxyOnFire2/_work/bins/apk_libs`, `$MISSION` =
`cmake-build-match/CMakeFiles/mission_table.dir/./tools/mission_case_table.cpp.obj`.

```sh
$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/arm-linux-androideabi/bin/ld.gold \
  -shared -soname libgof2hdaa.so --build-id -z now --exclude-libs=libgcc.a \
  -o libgof2hdaa.unstripped.so \
  $NDK/platforms/android-16/arch-arm/usr/lib/crtbegin_so.o \
  -L $NDK/platforms/android-16/arch-arm/usr/lib \
  $OBJ \
  --whole-archive third_party/zlib/libz_gof2.a third_party/libzip/libzip_gof2.a --no-whole-archive \
  -l log -l GLESv2 -l GLESv1_CM -l EGL -l android \
  $APK/libfmodex.so $APK/libfmodevent.so \
  third_party/openssl/libcrypto_gof2.a \
  -l m \
  $NDK/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/libc++abi.a \
  $NDK/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/libandroid_support.a \
  $NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/lib/gcc/arm-linux-androideabi/4.9.x/armv7-a/thumb/libgcc.a \
  $MISSION \
  -l dl -l c \
  $NDK/platforms/android-16/arch-arm/usr/lib/crtend_so.o
$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-strip \
  --strip-all -o libgof2hdaa.so libgof2hdaa.unstripped.so
python3 tools/reorder_shstrtab.py libgof2hdaa.unstripped.so libgof2hdaa.so
```

`$MISSION` is a decomp-only input; the original link did not have it. It is
placed after `libgcc.a` and before `-l dl -l c` so it lands at the end of
`.rodata` without moving any original byte. The original likely spelled the
libgcc input as `-lgcc -Wl,--exclude-libs,libgcc.a` (ndk-build) or the direct
archive path (CMake); both are unobservable at the section-byte level.

### 3.1 Archive order and member selection

| # | input | members pulled / role |
|---:|---|---|
| 1 | `crtbegin_so.o` (API 16) | required; removing it fails with `R_ARM_REL32` |
| 2 | 192 app objects | original TU order; `.text`/`.rodata`/`.data`/`.bss` order |
| 3 | `--whole-archive libz_gof2.a` | all 11 members: `inflate.o compress.o deflate.o crc32.o infback.o zutil.o inftrees.o uncompr.o trees.o inffast.o adler32.o` |
| 4 | `--whole-archive libzip_gof2.a` | all 54 members (`zip_*.o`, `mkstemp.o`, ...) |
| 5 | `-l log -l GLESv2 -l GLESv1_CM -l EGL -l android` | NEEDED entries 1–5 |
| 6 | `libfmodex.so libfmodevent.so` | NEEDED entries 6–7; FMOD re-exports libgcc helpers |
| 7 | `libcrypto_gof2.a` | `sha256.o`, `armv4cpuid.o`, `sha256-armv4.o` (3/3) |
| 8 | `-l m` | NEEDED entry 8 |
| 9 | `libc++abi.a` | 16/19 members (not pulled: `cxa_aux_runtime.o`, `cxa_unexpected.o`, `stdlib_stdexcept.o`) |
| 10 | `libandroid_support.a` | 1/66 members: `posix_memalign.o` (hidden wrapper over `memalign`) |
| 11 | `libgcc.a` | 14/1751 members: `_udivsi3.o _divsi3.o _arm_addsubdf3.o _arm_addsubsf3.o _aeabi_ldivmod.o _aeabi_uldivmod.o _dvmd_lnx.o _fixsfdi.o _fixunssfdi.o bpabi.o unwind-arm.o libunwind.o pr-support.o emutls.o` |
| 12 | `$MISSION` | decomp-only rank table |
| 13 | `-l dl -l c` | NEEDED entries 9–10 |
| 14 | `crtend_so.o` | empty; present or absent is byte-identical |

The member mapping is from the unstripped canonical output's `.symtab`,
matched against each archive member's defined global symbols. Because the
original's exported symbol set (5,370), `.dynsym` bytes, and every original
section size (apart from the appended +192 B decomp-only `.rodata` table) are
identical, this is the only member set consistent with the original
(modulo local-only members, which would change `.text` size and are therefore
excluded).

## 4. A/B results

All variants start from the exact canonical link line. `byte`/`linked` are
the official verifier counts. "sections changed vs canon" lists sections whose
size or bytes differ from the canonical build; `—` means the whole file
(including build-id) is byte-identical to the canonical `.so`. The canonical
file is 2,187,328 B with 27 sections.

### 4.1 Requested flags

| variant | byte | linked | file size | secs | sections changed vs canon |
|---|---:|---:|---:|---:|---|
| `flag_no_undefined` | 4505 | 4505 | 2187328 | 27 | — |
| `flag_fix_cortex_a8` | 4505 | 4505 | 2187328 | 27 | — |
| `flag_no_fix_cortex_a8` | 4505 | 4505 | 2187328 | 27 | — |
| `flag_z_noexecstack` | 4505 | 4505 | 2187328 | 27 | — |
| `flag_z_relro` | 4505 | 4505 | 2187328 | 27 | — |
| `flag_z_norelro` | 2932 | 4505 | 2188456 | 27 | `.note.gnu.build-id`, `.dynsym`, `.rel.dyn`, `.rel.plt`, `.plt`, `.text`, `.fini_array`, `.data.rel.ro`, `.init_array`, `.dynamic`, `.got`, `.data` |
| `flag_z_lazy` | 2932 | 4505 | 2188108 | 27 | `.note.gnu.build-id`, `.dynsym`, `.rel.dyn`, `.rel.plt`, `.plt`, `.text`, `.data.rel.ro`, `.dynamic:size312`, `.data` |
| `flag_z_execstack` | 4505 | 4505 | 2187328 | 27 | `.note.gnu.build-id` (PT_GNU_STACK RW→RWX) |
| `flag_z_combreloc` | 4505 | 4505 | 2187328 | 27 | — |
| `flag_z_nocombreloc` | 4494 | 4505 | 2187328 | 27 | `.note.gnu.build-id`, `.dynsym`, `.rel.dyn`, `.text`, `.data.rel.ro`, `.dynamic:size320` |
| `flag_exclude_libatomic` | 4505 | 4505 | 2187328 | 27 | — |
| `flag_exclude_libunwind` | 4505 | 4505 | 2187328 | 27 | — |
| `flag_warn_shared_textrel` | 4505 | 4505 | 2187328 | 27 | — |
| `flag_fatal_warnings` | 4505 | 4505 | 2187328 | 27 | — |
| `flag_hash_sysv` | 4505 | 4505 | 2187328 | 27 | — |
| `flag_hash_gnu` | 1750 | 4505 | 2191428 | 27 | `.note.gnu.build-id`, `.dynsym`, `.hash:missing`, `.gnu.version`, `.rel.dyn`, `.rel.plt`, `.plt`, `.text`, `.ARM.extab`, `.fini_array`, `.data.rel.ro`, `.init_array`, `.dynamic`, `.got`, `.data`, `.shstrtab:size283`, `.gnu.hash:extra` |
| `flag_hash_both` | 1750 | 4505 | 2232428 | 28 | `.note.gnu.build-id`, `.dynsym`, `.hash`, `.gnu.version`, `.rel.dyn`, `.rel.plt`, `.plt`, `.text`, `.ARM.extab`, `.fini_array`, `.data.rel.ro`, `.init_array`, `.dynamic:size336`, `.got`, `.data`, `.shstrtab:size283`, `.gnu.hash:extra` |
| `flag_no_as_needed` | 4505 | 4505 | 2187328 | 27 | — |
| `flag_as_needed` | 1750 | 4505 | 2187328 | 27 | `.note.gnu.build-id`, `.dynsym`, `.dynstr:size189159`, `.rel.dyn`, `.plt`, `.text`, `.ARM.extab`, `.fini_array`, `.data.rel.ro`, `.init_array`, `.dynamic:size296`, `.got`, `.data` |
| `flag_max_page_4096` | 4505 | 4505 | 2187328 | 27 | — |
| `flag_max_page_65536` | 2932 | 4505 | 2240576 | 27 | `.note.gnu.build-id`, `.dynsym`, `.rel.dyn`, `.rel.plt`, `.plt`, `.text`, `.data.rel.ro`, `.dynamic`, `.data` |
| `flag_common_page_4096` | 4505 | 4505 | 2187328 | 27 | — |
| `flag_common_page_65536` | 4505 | 4505 | 2187328 | 27 | — |
| `flag_spare_dynamic_5` | 4505 | 4505 | 2187328 | 27 | — |
| `flag_spare_dynamic_0` | 4494 | 4505 | 2187328 | 27 | `.note.gnu.build-id`, `.dynsym`, `.rel.dyn`, `.text`, `.data.rel.ro`, `.dynamic:size288` |
| `flag_spare_dynamic_10` | 4494 | 4505 | 2187328 | 27 | `.note.gnu.build-id`, `.dynsym`, `.rel.dyn`, `.text`, `.data.rel.ro`, `.dynamic:size368` |
| `flag_buildid_sha1` | 4505 | 4505 | 2187328 | 27 | — |
| `flag_buildid_tree` | 4505 | 4505 | 2187328 | 27 | — |
| `flag_buildid_none` | 1750 | 4505 | 2187268 | 26 | `.note.gnu.build-id:missing`, `.dynsym`, `.plt`, `.text`, `.ARM.extab`, `.fini_array`, `.data.rel.ro`, `.init_array`, `.dynamic`, `.got`, `.data`, `.shstrtab:size260` |
| `flag_buildid_md5` | 1971 | 4505 | 2187328 | 27 | `.note.gnu.build-id:size32`, `.plt`, `.text`, `.ARM.extab`, `.dynamic`, `.got` |
| `flag_gold_s` | 4505 | 4505 | 2187328 | 27 | `.note.gnu.build-id` |
| `flag_O1` | 4505 | 4505 | 2187328 | 27 | — |
| `flag_O2` | 1720 | 4505 | 2187328 | 27 | `.note.gnu.build-id`, `.dynsym`, `.dynstr:size188980`, `.gnu.version_d`, `.gnu.version_r`, `.plt`, `.text`, `.ARM.extab`, `.rodata:size268839`, `.fini_array`, `.data.rel.ro`, `.init_array`, `.dynamic`, `.got`, `.data`, `.comment:size302` |
| `flag_sort_name` | 1342 | 4503 | 2187328 | 27 | `.note.gnu.build-id`, `.dynsym`, `.rel.dyn`, `.text:size1259532`, `.ARM.exidx:size26800`, `.ARM.extab`, `.rodata:size269328`, `.fini_array`, `.data.rel.ro:size12256`, `.init_array`, `.dynamic`, `.data`, `.bss:size27060` |
| `flag_gc_sections` | 2176 | 4505 | 2187328 | 27 | `.note.gnu.build-id`, `.dynsym`, `.text:size1259412`, `.ARM.exidx:size26256`, `.ARM.extab`, `.rodata:size269143`, `.data.rel.ro`, `.data`, `.bss:size27036` |
| `flag_icf_safe` | 1347 | 4505 | 2170944 | 27 | `.note.gnu.build-id`, `.dynsym`, `.rel.dyn`, `.rel.plt`, `.plt`, `.text:size1247588`, `.ARM.exidx:size22944`, `.ARM.extab`, `.data.rel.ro`, `.dynamic`, `.data` |
| `flag_rosegment` | 1720 | 4505 | 2187328 | 27 | `.note.gnu.build-id`, `.dynsym`, `.rel.dyn`, `.rel.plt`, `.plt`, `.text`, `.ARM.exidx`, `.ARM.extab`, `.fini_array`, `.data.rel.ro`, `.init_array`, `.dynamic`, `.got`, `.data` |
| `flag_no_exclude_libs` | 1720 | 4497 | 2187328 | 27 | `.note.gnu.build-id`, `.dynsym:size89296`, `.dynstr:size189252`, `.hash:size38728`, `.gnu.version:size11162`, `.rel.dyn`, `.rel.plt:size29408`, `.plt:size44132`, `.text:size1259480`, `.ARM.exidx`, `.ARM.extab`, `.fini_array`, `.data.rel.ro`, `.init_array`, `.dynamic`, `.got:size17172`, `.data` |
| `flag_no_whole_archive` | 1338 | 4421 | 2134016 | 27 | `.note.gnu.build-id`, `.dynsym:size87664`, `.dynstr:size187884`, `.hash:size38320`, `.gnu.version:size10958`, `.rel.dyn:size28496`, `.rel.plt:size28984`, `.plt:size43496`, `.text:size1229320`, `.ARM.exidx:size27376`, `.ARM.extab:size155460`, `.rodata:size253143`, `.fini_array`, `.data.rel.ro:size12104`, `.init_array`, `.dynamic`, `.got:size16936`, `.data:size10540` |
| `flag_no_z_now` | 2932 | 4505 | 2188108 | 27 | `.note.gnu.build-id`, `.dynsym`, `.rel.dyn`, `.rel.plt`, `.plt`, `.text`, `.data.rel.ro`, `.dynamic:size312`, `.data` |

`flag_no_rosegment` and `flag_emit_relocs` fail in gold 1.12
(`unknown option: --no-rosegment`; `internal error in relocate_relocs`).
`ord_no_crtbegin` fails with `requires unsupported dynamic reloc R_ARM_REL32`.

### 4.2 Full ndk-build / CMake default link sets

The NDK r18b defaults were read from
`build/core/default-build-commands.mk`, `build/core/build-binary.mk`,
`build/core/toolchains/arm-linux-androideabi-clang/setup.mk` and
`build/cmake/android.toolchain.cmake`. The tested sets add, on top of the
canonical line:

* ndk-build: `--no-undefined --fix-cortex-a8 -z noexecstack -z relro
  --warn-shared-textrel --fatal-warnings --exclude-libs=libatomic.a` and
  `libatomic.a` (`-latomic` equivalent). `-z now`, `--build-id`,
  `--exclude-libs=libgcc.a`, `-lc -lm` are already present.
* CMake: the same plus `--exclude-libs=libunwind.a` and `libatomic.a`.

| variant | byte | linked | file size | secs | sections changed vs canon |
|---|---:|---:|---:|---:|---|
| `set_ndkbuild_default` | 4505 | 4505 | 2187328 | 27 | — |
| `set_cmake_default` | 4505 | 4505 | 2187328 | 27 | — |

Both are byte-identical to the canonical build, including the build-id. The
binary therefore cannot distinguish the ndk-build driver from the CMake
driver; it only fixes the flags that leave a trace.

### 4.3 Archive order / member selection

| variant | byte | linked | file size | secs | sections changed vs canon |
|---|---:|---:|---:|---:|---|
| `ord_fmod_after_cxxabi` | 4505 | 4505 | 2187328 | 27 | `.note.gnu.build-id`, `.dynsym`, `.dynstr`, `.hash`, `.gnu.version`, `.gnu.version_r`, `.rel.dyn`, `.rel.plt`, `.dynamic` |
| `ord_fmod_after_libgcc` | 1345 | 3798 | 2195520 | 27 | `.note.gnu.build-id`, `.dynsym:size88976`, `.dynstr:size188899`, `.hash:size38648`, `.gnu.version:size11122`, `.gnu.version_r`, `.rel.dyn:size28800`, `.rel.plt:size29256`, `.plt:size43904`, `.text:size1266960`, `.ARM.exidx:size28824`, `.ARM.extab:size156360`, `.fini_array`, `.data.rel.ro`, `.init_array`, `.dynamic`, `.got:size17124`, `.data` |
| `ord_fmod_before_objects` | 4505 | 4505 | 2187328 | 27 | `.note.gnu.build-id`, `.dynsym`, `.dynstr`, `.hash`, `.gnu.version`, `.gnu.version_r`, `.rel.dyn`, `.rel.plt`, `.dynamic` |
| `ord_fmod_before_zlib` | 4505 | 4505 | 2187328 | 27 | `.note.gnu.build-id`, `.dynsym`, `.dynstr`, `.hash`, `.gnu.version`, `.rel.dyn`, `.rel.plt`, `.dynamic` |
| `ord_fmod_between_zlib_libzip` | 4505 | 4505 | 2187328 | 27 | `.note.gnu.build-id`, `.dynsym`, `.dynstr`, `.hash`, `.gnu.version`, `.rel.dyn`, `.rel.plt`, `.dynamic` |
| `ord_libgcc_before_cxxabi` | 4415 | 4505 | 2187328 | 27 | `.note.gnu.build-id`, `.dynsym`, `.dynstr`, `.hash`, `.gnu.version`, `.rel.dyn`, `.rel.plt`, `.text`, `.ARM.exidx`, `.ARM.extab`, `.data.rel.ro`, `.data` |
| `ord_libgcc_before_android_support` | 4496 | 4505 | 2187328 | 27 | `.note.gnu.build-id`, `.dynsym`, `.dynstr`, `.hash`, `.gnu.version`, `.rel.dyn`, `.rel.plt`, `.text`, `.ARM.exidx`, `.ARM.extab` |
| `ord_libgcc_before_crtend` | 4505 | 4505 | 2187328 | 27 | `.note.gnu.build-id` |
| `ord_libgcc_after_mission_table` | 4505 | 4505 | 2187328 | 27 | `.note.gnu.build-id` |
| `ord_libandroid_support_after_libgcc` | 4496 | 4505 | 2187328 | 27 | `.note.gnu.build-id`, `.dynsym`, `.dynstr`, `.hash`, `.gnu.version`, `.rel.dyn`, `.rel.plt`, `.text`, `.ARM.exidx`, `.ARM.extab` |
| `ord_libandroid_support_before_cxxabi` | 1505 | 4504 | 2187328 | 27 | `.note.gnu.build-id`, `.dynsym`, `.dynstr:size189214`, `.hash`, `.gnu.version`, `.rel.dyn`, `.rel.plt`, `.plt`, `.text:size1259400`, `.ARM.exidx:size28512`, `.ARM.extab:size156300`, `.data.rel.ro`, `.dynamic`, `.got`, `.data` |
| `ord_cxxstatic_before_abi` | 4505 | 4505 | 2187328 | 27 | — |
| `ord_cxxstatic_after_abi` | 4505 | 4505 | 2187328 | 27 | — |
| `ord_cxxstatic_replace_abi` | 1329 | 4407 | 2109492 | 27 | `.note.gnu.build-id`, `.dynsym:size84208`, `.dynstr:size183878`, `.hash:size37456`, `.gnu.version:size10526`, `.rel.dyn:size19056`, `.rel.plt:size28840`, `.plt:size43280`, `.text:size1222132`, `.ARM.exidx:size25736`, `.ARM.extab:size153060`, `.rodata:size262145`, `.fini_array`, `.data.rel.ro:size6952`, `.init_array`, `.dynamic`, `.got:size16800`, `.data:size10590`, `.bss:size26512` |
| `ord_libcxxabi_before_m` | 3204 | 4505 | 2187328 | 27 | `.note.gnu.build-id`, `.dynsym`, `.dynstr`, `.hash`, `.gnu.version`, `.gnu.version_r`, `.rel.dyn`, `.rel.plt`, `.text:size1259476`, `.ARM.exidx`, `.ARM.extab`, `.data.rel.ro`, `.dynamic`, `.data`, `.bss:size27064` |
| `ord_libcxxabi_before_crypto` | 3204 | 4505 | 2187328 | 27 | `.note.gnu.build-id`, `.dynsym`, `.dynstr`, `.hash`, `.gnu.version`, `.gnu.version_r`, `.rel.dyn`, `.rel.plt`, `.text:size1259476`, `.ARM.exidx`, `.ARM.extab`, `.data.rel.ro`, `.dynamic`, `.data`, `.bss:size27064` |
| `ord_libcrypto_before_fmod` | 4505 | 4505 | 2187328 | 27 | `.note.gnu.build-id`, `.dynsym`, `.dynstr`, `.hash`, `.gnu.version`, `.rel.dyn`, `.rel.plt` |
| `ord_libcrypto_after_cxxabi` | 3204 | 4505 | 2187328 | 27 | `.note.gnu.build-id`, `.dynsym`, `.dynstr`, `.hash`, `.gnu.version`, `.rel.dyn`, `.rel.plt`, `.text:size1259476`, `.ARM.exidx`, `.ARM.extab`, `.data.rel.ro`, `.data`, `.bss:size27064` |
| `ord_libcrypto_after_m` | 4505 | 4505 | 2187328 | 27 | `.note.gnu.build-id`, `.dynsym`, `.dynstr`, `.hash`, `.gnu.version`, `.rel.dyn`, `.rel.plt` |
| `ord_libcrypto_before_android` | 4505 | 4505 | 2187328 | 27 | `.note.gnu.build-id`, `.dynsym`, `.dynstr`, `.hash`, `.gnu.version`, `.rel.dyn`, `.rel.plt` |
| `ord_zlib_libzip_swap` | 4371 | 4505 | 2187340 | 27 | `.note.gnu.build-id`, `.dynsym`, `.dynstr`, `.hash`, `.gnu.version`, `.rel.dyn`, `.rel.plt`, `.text`, `.ARM.exidx`, `.ARM.extab`, `.rodata`, `.data.rel.ro`, `.data:size10616` |
| `ord_whole_crypto` | 4505 | 4505 | 2187328 | 27 | — |
| `ord_no_crtend` | 4505 | 4505 | 2187328 | 27 | — |
| `ord_crtend_before_libs` | 4505 | 4505 | 2187328 | 27 | — |
| `ord_crtbegin_after_objects` | 1378 | 4504 | 2187344 | 27 | `.note.gnu.build-id`, `.dynsym`, `.dynstr`, `.hash`, `.gnu.version`, `.gnu.version_d`, `.gnu.version_r`, `.rel.dyn`, `.rel.plt`, `.text:size1259496`, `.ARM.exidx`, `.ARM.extab`, `.fini_array`, `.data.rel.ro`, `.dynamic`, `.data:size10620` |
| `whole_zlib_only` | 1346 | 4431 | 2166844 | 27 | `.note.gnu.build-id`, `.dynsym:size87648`, `.dynstr:size187853`, `.hash:size38316`, `.gnu.version:size10956`, `.rel.dyn:size28488`, `.rel.plt:size28792`, `.plt:size43208`, `.text:size1246472`, `.ARM.exidx:size27440`, `.ARM.extab:size155604`, `.rodata:size268663`, `.fini_array`, `.data.rel.ro:size12168`, `.init_array`, `.dynamic`, `.got:size16832`, `.data:size10600`, `.bss:size27004` |
| `whole_libzip_only` | 1342 | 4458 | 2146308 | 27 | `.note.gnu.build-id`, `.dynsym:size88544`, `.dynstr:size188620`, `.hash:size38540`, `.gnu.version:size11068`, `.rel.dyn:size28504`, `.rel.plt:size29288`, `.plt:size43952`, `.text:size1236024`, `.ARM.exidx:size27984`, `.ARM.extab:size155772`, `.rodata:size253223`, `.fini_array`, `.data.rel.ro:size12104`, `.init_array`, `.dynamic`, `.got:size17092`, `.data:size10544` |

### 4.3.1 System-library presence and position

| variant | byte | linked | file size | secs | sections changed vs canon |
|---|---:|---:|---:|---:|---|
| `lib_no_m` | 1345 | 4505 | 2199660 | 27 | `.note.gnu.build-id`, `.dynsym:size89472`, `.dynstr:size189283`, `.hash:size38772`, `.gnu.version:size11184`, `.gnu.version_r:size32`, `.rel.dyn`, `.rel.plt:size29408`, `.plt:size44132`, `.text:size1268804`, `.ARM.exidx:size28784`, `.ARM.extab:size156472`, `.rodata:size269824`, `.fini_array`, `.data.rel.ro`, `.init_array`, `.dynamic:size320`, `.got:size17172`, `.data:size10648`, `.bss:size27068` |
| `lib_no_dl` | 1970 | 4505 | 2187328 | 27 | `.note.gnu.build-id`, `.dynsym`, `.dynstr:size189199`, `.rel.dyn`, `.plt`, `.text`, `.ARM.extab`, `.data.rel.ro`, `.dynamic:size320`, `.got` |
| `lib_no_c` | 1750 | 4505 | 2187328 | 27 | `.note.gnu.build-id`, `.dynsym`, `.dynstr:size189200`, `.hash`, `.gnu.version`, `.gnu.version_d`, `.gnu.version_r:size32`, `.rel.dyn`, `.rel.plt`, `.plt`, `.text`, `.ARM.extab`, `.fini_array`, `.data.rel.ro`, `.init_array`, `.dynamic:size320`, `.got`, `.data` |
| `lib_append_c` | 4505 | 4505 | 2187328 | 27 | — |
| `lib_prepend_c` | 4505 | 4505 | 2187328 | 27 | `.note.gnu.build-id`, `.dynsym`, `.dynstr`, `.hash`, `.gnu.version`, `.gnu.version_r`, `.rel.dyn`, `.rel.plt`, `.dynamic` |
| `lib_append_m` | 4505 | 4505 | 2187328 | 27 | — |
| `lib_prepend_m` | 4505 | 4505 | 2187328 | 27 | `.note.gnu.build-id`, `.dynsym`, `.dynstr`, `.hash`, `.gnu.version`, `.gnu.version_r`, `.rel.dyn`, `.rel.plt`, `.dynamic` |
| `lib_append_dl` | 4505 | 4505 | 2187328 | 27 | — |
| `lib_prepend_dl` | 4505 | 4505 | 2187328 | 27 | `.note.gnu.build-id`, `.dynsym`, `.dynstr`, `.hash`, `.gnu.version`, `.gnu.version_r`, `.rel.dyn`, `.rel.plt`, `.dynamic` |

Removing `-lm`, `-ldl` or `-lc` drops `byte_exact` to 1345 / 1970 / 1750
respectively and removes the corresponding `DT_NEEDED` entry. Appending a
duplicate `-lc`/`-lm`/`-ldl` after crtend is byte-identical (whole file,
including build-id). Prepending a duplicate before the app objects keeps
`byte_exact 4505` but reorders `DT_NEEDED` and changes
`.dynsym`/`.dynstr`/`.hash`/`.rel.*`, so the canonical position is the one
that matches the original's `DT_NEEDED` and dynsym order.

Key archive-order conclusions:

* **FMOD must be before `-lm` and before `libc++abi.a`.** Moving it after
  `libc++abi` puts `libm` before FMOD in `DT_NEEDED` and changes
  `.dynsym`/`.dynstr`/`.hash`/`.rel.*`; moving it after `libgcc` also changes
  `.text` (byte 1345). Moving it before the system `-l` libs or before
  zlib/libzip puts FMOD before `liblog` in `DT_NEEDED` and changes dynsym.
* **FMOD must be before libcrypto.** `ord_libcrypto_before_fmod` keeps
  `DT_NEEDED` correct but changes `.dynsym`/`.dynstr`/`.hash`/`.rel.*`.
* **libcrypto must be after FMOD and before `-lm`.** Moving it after
  `libc++abi` changes `.text` (byte 3204); moving it after `-lm` or before
  `-landroid` changes dynsym order.
* **libc++abi must be after `-lm` and before libandroid_support.**
  `ord_libcxxabi_before_m` and `ord_libcxxabi_before_crypto` change `.text`
  (byte 3204). `ord_libandroid_support_before_cxxabi` drops to byte 1505.
* **libandroid_support must be after libc++abi and before libgcc.**
  `ord_libandroid_support_after_libgcc` drops to byte 4496.
* **libgcc must be after libandroid_support.** Moving it before libc++abi
  drops to byte 4415; before libandroid_support drops to 4496. Its exact
  position relative to the *trailing empty inputs* (mission table, `-l dl
  -l c`, crtend) is unobservable: `ord_libgcc_after_mission_table` and
  `ord_libgcc_before_crtend` are section-identical and differ only in
  build-id.
* **libc++_static.a cannot replace libc++abi.a.** It loses 98 exported C++
  ABI symbols (`std::bad_*`, `std::exception`, ...), byte 1329, linked 4407.
  Adding `libc++_static.a` before or after `libc++abi.a` is byte-identical
  (0 members pulled), so its presence is unobservable.
* **zlib must precede libzip.** Swapping them changes `.text`, `.rodata`,
  `.data` and `.dynsym` (byte 4371, file size +12).
* **zlib and libzip must both be whole-archived.** Removing either loses
  members: zlib-only 74 missing, libzip-only 47 missing.
* **crtbegin must be first.** Moving it after the app objects changes
  `.text`/`.data`/`.dynsym` (byte 1378, file size +16).
* **crtend is unobservable.** Removing it or moving it before `-ldl` is
  byte-identical. `--whole-archive` around libcrypto is also byte-identical.

### 4.4 Section-size deltas for the size-changing variants

The canonical sizes are: `.text 0x1337c8`, `.ARM.exidx 0x6f70`,
`.ARM.extab 0x26298`, `.rodata 0x41c27` (canonical, +192),
`.dynsym 0x15ca0`, `.dynstr 0x2e318`, `.hash 0x973c`, `.rel.dyn 0x7048`,
`.rel.plt 0x72d0`, `.plt 0xac4c`, `.got 0x430c`, `.dynamic 0x148`.
The table above gives the changed sizes for every variant that moves them.
The most informative negatives:

* `--gc-sections`: `.text 0x133794` (-0x34), `.ARM.exidx 0x6690`,
  `.rodata 0x41b57`; byte 2176.
* `--icf=safe`: `.text 0x130964` (-0x2e64), `.ARM.exidx 0x59a0`;
  byte 1347.
* `--sort-section=name`: `.text 0x13380c`, `.ARM.exidx 0x68b0`,
  `.rodata 0x41c10`; byte 1342, linked 4503.
* `--rosegment`: `.text` size unchanged but every address-dependent section
  changes; byte 1720.
* `-O2` (gold): `.text` size unchanged (1,259,464) but content changes;
  `.rodata 0x41a27`; byte 1720.
* no `--exclude-libs=libgcc.a`: `.dynsym 0x15cd0`, `.dynstr 0x2e3d4`,
  `.hash 0x9748`, `.plt 0xac64`, `.got 0x4314`, `.text 0x1337d8`;
  3 extra exports, byte 1720, linked 4497.
* no whole-archive: `.text 0x12c208`, `.rodata 0x3dcd7`, `.dynsym 0x15670`;
  84 missing, byte 1338, linked 4421.

## 5. Proven / disproven / unobservable

### Proven by the original ELF (or by a variant that changes it)

1. **Gold 1.12** shared link; `-shared -soname libgof2hdaa.so`.
2. **`--build-id`** with a 20-byte SHA-1 descriptor; the descriptor is the
   SHA-1 of the unstripped gold output with the descriptor zeroed.
3. **`-z now`**: `DT_FLAGS=BIND_NOW`, `DT_FLAGS_1=NOW`, `.dynamic 0x148`,
   `.got` before `.data` inside `PT_GNU_RELRO`.
4. **`--exclude-libs=libgcc.a`**: removing it exports `__aeabi_idiv0`,
   `__aeabi_ldiv0`, `__aeabi_ldivmod` (3 extra, byte 1720, linked 4497).
5. **`--whole-archive` for zlib+libzip** at the current position: removing
   it loses 84 members (byte 1338, linked 4421); zlib-only/libzip-only also
   lose members.
6. **SysV hash** (`.hash`, no `.gnu.hash`): `--hash-style=gnu`/`both`
   changes the layout and drops byte to 1750.
7. **`-z combreloc`**: `DT_RELCOUNT=1162`; `-z nocombreloc` drops
   `DT_RELCOUNT`, changes `.dynamic` to 0x140 and byte to 4494.
8. **`--no-as-needed`**: `--as-needed` drops `libGLESv1_CM`, `libEGL`,
   `libandroid`, `libdl` from `DT_NEEDED` and drops byte to 1750.
9. **`-z max-page-size=4096`**: 65536 changes the file size to 2,240,576 and
   byte to 2932.
10. **`--spare-dynamic-tags=5`**: 0 and 10 change `.dynamic` to 0x120/0x170
    and byte to 4494.
11. **`-lm -ldl -lc` once, in the canonical position**: removing any one
    changes `DT_NEEDED` and drops byte to 1345/1970/1750; appending
    duplicates is neutral; prepending duplicates reorders `DT_NEEDED`.
12. **FMOD before libcrypto and before `-lm`**; **libcrypto before `-lm`**;
    **libc++abi before libandroid_support before libgcc**; **zlib before
    libzip**; **crtbegin first**. Each alternative tested changes
    `DT_NEEDED`, `.dynsym` order, or `.text`.
13. **GNU `strip --strip-all` after the link**, not gold `-s`: gold `-s`
    leaves `.rel.plt` flags/info, init/fini `sh_entsize` and `.bss` offset
    different, and its build-id is the hash of the stripped file.
14. **No `--gc-sections`, no `--icf`, no `--sort-section`, no `--rosegment`,
    no gold `-O2`**: each changes `.text`/`.rodata`/`.ARM.exidx`/`.ARM.extab`
    away from the original.
15. **The 192-object app order is the original TU order**: `.text` size and
    base exact, first/last exported functions at `0x69578`/`0x194030`,
    `.dynsym`/`.dynstr`/`.hash` byte-identical, `.rel.dyn` entry-for-entry
    identical, `.rel.plt` 3,671/3,674 identical.

### Unobservable (byte-identical to the canonical build)

`--no-undefined`, `--fix-cortex-a8` and `--no-fix-cortex-a8`,
`-z noexecstack`, `-z relro`, the explicit spelling `-z combreloc`,
`--hash-style=sysv`, `--no-as-needed`, `-z max-page-size=4096`,
`-z common-page-size=4096` (and 65536), `--spare-dynamic-tags=5`,
`--build-id=sha1` and `--build-id=tree`, `--exclude-libs=libatomic.a`,
`--exclude-libs=libunwind.a`, `--warn-shared-textrel`, `--fatal-warnings`,
gold `-O1`, the full ndk-build default link set, the full CMake default link
set, `libc++_static.a` before/after `libc++abi.a`, `--whole-archive` around
libcrypto, `crtend_so.o` present/absent, and `crtend` before `-ldl`.

The binary cannot distinguish these from the current recipe. In particular,
the full ndk-build and CMake default link sets are both byte-identical to the
current line, so the **driver** (ndk-build vs CMake) is not recoverable from
the stripped ELF; only the flags that leave a trace are.

### Disproven (changes the ELF away from the original)

Gold `-s`; `-z lazy`; `-z norelro`; `-z nocombreloc`; `-z execstack`
(PT_GNU_STACK RW→RWX); `--hash-style=gnu`; `--hash-style=both`;
`--as-needed`; `-z max-page-size=65536`; `--spare-dynamic-tags=0/10`;
`--build-id=none`; `--build-id=md5`; no `--exclude-libs=libgcc.a`; no
`--whole-archive`; zlib-only; libzip-only; `--gc-sections`; `--icf=safe`;
`--sort-section=name`; `--rosegment`; gold `-O2`; FMOD after libc++abi;
FMOD after libgcc; FMOD before objects; FMOD before zlib; FMOD between
zlib/libzip; libcrypto before FMOD; libcrypto after libc++abi; libcrypto
after `-lm`; libcrypto before `-landroid`; libc++abi before `-lm`;
libc++abi before libcrypto; libandroid_support before libc++abi;
libandroid_support after libgcc; libgcc before libc++abi; libgcc before
libandroid_support; libc++_static replacing libc++abi; zlib/libzip swap;
crtbegin after objects.

### Still hypothesis

* The exact original link-command text (spelling of `-lgcc` vs a direct
  `libgcc.a` path, `-latomic` presence, `--no-undefined`, `--fatal-warnings`,
  etc.) is not recoverable; those spellings are byte-identical.
* The exact position of `libgcc.a` relative to the trailing empty inputs
  (`-l dl -l c`, `crtend_so.o`) is not recoverable from section bytes; it
  changes only the build-id.
* The original build-id preimage (the unstripped output, including debug info
  and `.symtab`) no longer exists; the build-id is the last residual.
* The exact driver (ndk-build vs CMake) is not recoverable; both default link
  sets are byte-identical.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w7002
cmake --build cmake-build-match --target check -j8

# exact gold/strip/reorder command
ninja -C cmake-build-match -t commands libgof2hdaa.so | tail -1 \
  > /dev/shm/w7002/link_cmd.txt

# canonical reproduction (must equal the checked-in .so)
python3 /dev/shm/w7002/relink.py /dev/shm/w7002/spec_canon.json
sha256sum /dev/shm/w7002/variants/canon.so cmake-build-match/libgof2hdaa.so

# all variants + official verifier
python3 /dev/shm/w7002/matrix.py
xargs -a /dev/shm/w7002/verify_list.txt -n1 -P12 /dev/shm/w7002/run_verify_one.sh
```

The scratch scripts are `/dev/shm/w7002/relink.py`, `matrix.py`,
`elfdiff.py`, `run_verify_one.sh`; the durable record is this note.

## 7. Files

* `_work/reconstructions/link-line-provenance-wave120-w7002.md` — this note.
* `_work/reconstructions/gold-link-flags-wave111-w5918.md` — prior gold flag
  matrix (consistent with this pass).
* `_work/reconstructions/link-recipe-rederivation-wb103.md` — prior link
  recipe re-derivation (consistent).
* `_work/reconstructions/linker_flag_audit_w5203.md` — prior linker-flag
  audit (consistent).
* `_work/reconstructions/dynsym_dynstr_hash_order_w3567.md` — dynsym order
  model.
