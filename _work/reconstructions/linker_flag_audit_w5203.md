# Linker-flag audit (w5203)

Worktree: `/home/debian/GalaxyOnFire2/wt/w5203`
Branch: `wave104-w5203`
Base commit: `8c5fd3646`
Date: 2026-11-04
Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(ELF32 ARM, armeabi-v7a, softfp, NDK r18b clang 7.0.2, `-Oz`, stripped).

This note is a controlled relink audit of the canonical gold link line. Every
variant below was produced from the **same canonical objects** by changing only
the link flags / input order. No `src/` change was made. The canonical build is
unchanged and `check` is green.

## 1. Canonical state (before any experiment)

Commands:

```sh
cd /home/debian/GalaxyOnFire2/wt/w5203
export TMPDIR=/dev/shm/w5203/tmp
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
```

Raw `check` output:

```
compared 4524  avg 99.9%  byte_exact 4500  linked_exact 4500  fuzzy_perfect 4500
stub_zero_size 0  orig_zero_size 0  missing 0 (wrong_type 0  absent 0)  extra 0
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Canonical `.so`: `cmake-build-match/libgof2hdaa.so`, 2,187,328 bytes,
sha256 `7008e892cb1e329d8e7cb8532a07adcf38a7ae688a2f6293102751b91de6b802`,
build-id `bafe2bb4386192fc59d580efb27ad6967c2caaae`.

Original `.so`: 2,187,328 bytes,
sha256 `d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483`,
build-id `c011810da45777b83ed721b2a87d517efed0d310`.

### Canonical link line (the line under test)

The exact command is the one Ninja runs for `libgof2hdaa.so`; it was captured
with `ninja -t commands libgof2hdaa.so`.  In abbreviated form:

```sh
$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/arm-linux-androideabi/bin/ld.gold \
  -shared -soname libgof2hdaa.so --build-id -z now --exclude-libs=libgcc.a \
  -o libgof2hdaa.unstripped.so \
  $NDK/platforms/android-16/arch-arm/usr/lib/crtbegin_so.o -L $NDK/platforms/android-16/arch-arm/usr/lib \
  <193 gof2 objects in tools/source_order.txt order> \
  --whole-archive third_party/zlib/libz_gof2.a third_party/libzip/libzip_gof2.a --no-whole-archive \
  -l log -l GLESv2 -l GLESv1_CM -l EGL -l android \
  /home/debian/GalaxyOnFire2/_work/bins/apk_libs/libfmodex.so \
  /home/debian/GalaxyOnFire2/_work/bins/apk_libs/libfmodevent.so \
  third_party/openssl/libcrypto_gof2.a -l m \
  $NDK/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/libc++abi.a \
  $NDK/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/libandroid_support.a \
  $NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/lib/gcc/arm-linux-androideabi/4.9.x/armv7-a/thumb/libgcc.a \
  -l dl -l c \
  $NDK/platforms/android-16/arch-arm/usr/lib/crtend_so.o
$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-strip \
  --strip-all -o libgof2hdaa.so libgof2hdaa.unstripped.so
python3 tools/reorder_shstrtab.py libgof2hdaa.unstripped.so libgof2hdaa.so
```

## 2. Method

A small harness (`/dev/shm/w5203/relink.py`) parsed the exact Ninja command,
replaced only the `-o` path, optionally inserted/dropped flags or libraries,
then re-ran the same GNU `strip --strip-all` + `tools/reorder_shstrtab.py`
post-processing. `canon` (no change) reproduces the canonical `.so`
byte-for-byte (sha256 `7008e892...`), so every difference below is attributable
to the flag under test.

Gate counts for each variant were produced with the project's own verifier:

```sh
python3 tools/verify/verify.py \
  --build-dir /dev/shm/w5203/verify/<variant> \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /dev/shm/w5203/variants/<variant>.so \
  --objdump $NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
```

ELF sections, hashes, dynamic tags, NEEDED order, `.rel.plt`/`.rel.dyn`,
program headers and the raw section-header table were compared with a
standalone ELF32 parser (`/dev/shm/w5203/elfcompare.py`); no `readelf`
formatting was relied on for the byte comparisons.

## 3. Reference ELF (original vs canonical)

Sizes are exact for every section below; `sha` is the first 12 hex digits of
the SHA-256 of the section bytes.

| section | orig size | canon size | orig sha | canon sha | verdict |
|---|---:|---:|---|---|---|
| `.text` | 1,259,464 | 1,259,464 | `d49e32c1330e` | `144a3c3799cd` | size exact, content differs |
| `.rodata` | 269,159 | 269,159 | `73c737319958` | `73c737319958` | **byte-identical** |
| `.data` | 10,604 | 10,604 | `bf3a87118411` | `bf3a87118411` | **byte-identical** |
| `.data.rel.ro` | 12,264 | 12,264 | `34ad551db3f4` | `34ad551db3f4` | **byte-identical** |
| `.bss` | 27,052 | 27,052 | `fcd5c12f765b` | `fcd5c12f765b` | **byte-identical** |
| `.got` | 17,164 | 17,164 | `87e6b167273f` | `87e6b167273f` | **byte-identical** |
| `.plt` | 44,108 | 44,108 | `31d4766e6f3d` | `31d4766e6f3d` | **byte-identical** |
| `.dynamic` | 328 | 328 | `8a0e39f4409e` | `8a0e39f4409e` | **byte-identical** |
| `.dynsym` | 89,248 | 89,248 | `5a3b59e8436c` | `5a3b59e8436c` | **byte-identical** |
| `.dynstr` | 189,208 | 189,208 | `a42e036792f6` | `a42e036792f6` | **byte-identical** |
| `.hash` | 38,716 | 38,716 | `fdc6fe8d74f1` | `fdc6fe8d74f1` | **byte-identical** |
| `.rel.plt` | 29,392 | 29,392 | `43bf7c4eb170` | `43bf7c4eb170` | **byte-identical** |
| `.rel.dyn` | 28,744 | 28,744 | `4f55867c1547` | `4f55867c1547` | **byte-identical** |
| `.ARM.exidx` | 28,528 | 28,528 | `d7490b590872` | `652126c2ad2d` | size exact, content differs |
| `.ARM.extab` | 156,312 | 156,312 | `1880b0ef21ad` | `6b66adedd2ed` | size exact, content differs |
| `.shstrtab` | 279 | 279 | `c34e875ec1fd` | `c34e875ec1fd` | **byte-identical** |
| `.note.gnu.build-id` | 36 | 36 | `375800e6e941` | `be5a247e3670` | size exact, descriptor differs |

The raw section-header table of the canonical `.so` is byte-identical to the
original's (27 entries x 40 bytes). Program headers are byte-identical too.

Dynamic tags (original == canonical):

```
DT_FLAGS   = BIND_NOW (0x8)
DT_FLAGS_1 = NOW (0x1)
DT_SYMTAB  = 0x1f0, DT_STRTAB = 0x15e90, DT_HASH = 0x441a8
DT_REL     = 0x504d4, DT_RELSZ = 0x7048
DT_JMPREL  = 0x5751c, DT_PLTRELSZ = 0x72d0, DT_PLTREL = 2
```

NEEDED order (original == canonical):

```
liblog.so libGLESv2.so libGLESv1_CM.so libEGL.so libandroid.so
libfmodex.so libfmodevent.so libm.so libdl.so libc.so
```

## 4. Variant matrix (raw results)

All variants start from the canonical link line. `canon sha = Y` means the
final stripped `.so` is byte-identical to the canonical build (including its
build-id). `shdr = Y` means the raw section-header table is byte-identical to
the original's. `NEEDED`, `dyn tags`, `rel.plt`, `rel.dyn` compare to the
original. `cmp` is the number of compared functions (4,524 unless symbols were
lost).

| variant | byte | linked | cmp | miss | extra | size | canon sha | shdr | .text | .rodata | .exidx | .extab | .dynamic | NEEDED | dyn tags | rel.plt | rel.dyn |
|---|---:|---:|---:|---:|---:|---:|---|---|---:|---:|---:|---:|---:|---|---|---|---|
| buildid_md5 | 1969 | 4500 | 4524 | 0 | 0 | 2187328 | n | n | 1259464 | 269159 | 28528 | 156312 | 328 | Y | n | Y | Y |
| buildid_none | 1750 | 4500 | 4524 | 0 | 0 | 2187268 | n | n | 1259464 | 269159 | 28528 | 156312 | 328 | Y | n | Y | Y |
| buildid_sha1 | 4500 | 4500 | 4524 | 0 | 0 | 2187328 | Y | Y | 1259464 | 269159 | 28528 | 156312 | 328 | Y | Y | Y | Y |
| buildid_tree | 4500 | 4500 | 4524 | 0 | 0 | 2187328 | Y | Y | 1259464 | 269159 | 28528 | 156312 | 328 | Y | Y | Y | Y |
| canon | 4500 | 4500 | 4524 | 0 | 0 | 2187328 | Y | Y | 1259464 | 269159 | 28528 | 156312 | 328 | Y | Y | Y | Y |
| fatal_warnings | 4500 | 4500 | 4524 | 0 | 0 | 2187328 | Y | Y | 1259464 | 269159 | 28528 | 156312 | 328 | Y | Y | Y | Y |
| fix_cortex | 4500 | 4500 | 4524 | 0 | 0 | 2187328 | Y | Y | 1259464 | 269159 | 28528 | 156312 | 328 | Y | Y | Y | Y |
| gc_sections | 2176 | 4500 | 4524 | 0 | 0 | 2187328 | n | n | 1259412 | 269143 | 26256 | 156312 | 328 | Y | Y | Y | Y |
| gold_strip | 4500 | 4500 | 4524 | 0 | 0 | 2187328 | n | n | 1259464 | 269159 | 28528 | 156312 | 328 | Y | Y | Y | Y |
| hash_both | 1750 | 4500 | 4524 | 0 | 0 | 2232428 | n | n | 1259464 | 269159 | 28528 | 156312 | 336 | Y | n | n | n |
| hash_gnu | 1750 | 4500 | 4524 | 0 | 0 | 2191428 | n | n | 1259464 | 269159 | 28528 | 156312 | 328 | Y | n | n | n |
| hash_sysv | 4500 | 4500 | 4524 | 0 | 0 | 2187328 | Y | Y | 1259464 | 269159 | 28528 | 156312 | 328 | Y | Y | Y | Y |
| icf_all | 1347 | 4499 | 4524 | 0 | 0 | 2166848 | n | n | 1243548 | 269159 | 22368 | 156312 | 328 | Y | Y | n | n |
| icf_safe | 1347 | 4500 | 4524 | 0 | 0 | 2170944 | n | n | 1247588 | 269159 | 22944 | 156312 | 328 | Y | Y | n | n |
| landroid_support_again | 4500 | 4500 | 4524 | 0 | 0 | 2187328 | Y | Y | 1259464 | 269159 | 28528 | 156312 | 328 | Y | Y | Y | Y |
| latomic | 4500 | 4500 | 4524 | 0 | 0 | 2187328 | Y | Y | 1259464 | 269159 | 28528 | 156312 | 328 | Y | Y | Y | Y |
| lc_again | 4500 | 4500 | 4524 | 0 | 0 | 2187328 | n | Y | 1259464 | 269159 | 28528 | 156312 | 328 | n | Y | n | n |
| lc_append | 4500 | 4500 | 4524 | 0 | 0 | 2187328 | Y | Y | 1259464 | 269159 | 28528 | 156312 | 328 | Y | Y | Y | Y |
| lcxx_static | 4500 | 4500 | 4524 | 0 | 0 | 2187328 | Y | Y | 1259464 | 269159 | 28528 | 156312 | 328 | Y | Y | Y | Y |
| lcxxabi_again | 4500 | 4500 | 4524 | 0 | 0 | 2187328 | Y | Y | 1259464 | 269159 | 28528 | 156312 | 328 | Y | Y | Y | Y |
| ldl_again | 4500 | 4500 | 4524 | 0 | 0 | 2187328 | n | Y | 1259464 | 269159 | 28528 | 156312 | 328 | n | Y | n | n |
| ldl_append | 4500 | 4500 | 4524 | 0 | 0 | 2187328 | Y | Y | 1259464 | 269159 | 28528 | 156312 | 328 | Y | Y | Y | Y |
| lgcc_again | 4500 | 4500 | 4524 | 0 | 0 | 2187328 | Y | Y | 1259464 | 269159 | 28528 | 156312 | 328 | Y | Y | Y | Y |
| lm_again | 4500 | 4500 | 4524 | 0 | 0 | 2187328 | n | Y | 1259464 | 269159 | 28528 | 156312 | 328 | n | Y | n | n |
| lm_append | 4500 | 4500 | 4524 | 0 | 0 | 2187328 | Y | Y | 1259464 | 269159 | 28528 | 156312 | 328 | Y | Y | Y | Y |
| lunwind | 4500 | 4500 | 4524 | 0 | 0 | 2187328 | Y | Y | 1259464 | 269159 | 28528 | 156312 | 328 | Y | Y | Y | Y |
| no_buildid | 1750 | 4500 | 4524 | 0 | 0 | 2187268 | n | n | 1259464 | 269159 | 28528 | 156312 | 328 | Y | n | Y | Y |
| no_exclude_libs | 1720 | 4492 | 4524 | 0 | 3 | 2187328 | n | n | 1259480 | 269159 | 28528 | 156312 | 328 | Y | n | n | n |
| no_lc | 1750 | 4500 | 4524 | 0 | 0 | 2187328 | n | n | 1259464 | 269159 | 28528 | 156312 | 320 | n | n | n | n |
| no_ldl | 1968 | 4500 | 4524 | 0 | 0 | 2187328 | n | n | 1259464 | 269159 | 28528 | 156312 | 320 | n | n | Y | n |
| no_lm | 1345 | 4500 | 4524 | 0 | 12 | 2199660 | n | n | 1268804 | 269632 | 28784 | 156472 | 320 | n | n | n | n |
| no_undefined | 4500 | 4500 | 4524 | 0 | 0 | 2187328 | Y | Y | 1259464 | 269159 | 28528 | 156312 | 328 | Y | Y | Y | Y |
| no_whole_archive | 1338 | 4416 | 4440 | 84 | 0 | 2134016 | n | n | 1229320 | 252951 | 27376 | 155460 | 328 | Y | n | n | n |
| no_z_now | 2932 | 4500 | 4524 | 0 | 0 | 2188108 | n | n | 1259464 | 269159 | 28528 | 156312 | 312 | Y | n | n | n |
| nofix_cortex | 4500 | 4500 | 4524 | 0 | 0 | 2187328 | Y | Y | 1259464 | 269159 | 28528 | 156312 | 328 | Y | Y | Y | Y |
| nostdlibpp | 1329 | 4402 | 4426 | 98 | 0 | 2109492 | n | n | 1222132 | 261953 | 25736 | 153060 | 328 | Y | n | n | n |
| rosegment | 1720 | 4500 | 4524 | 0 | 0 | 2187328 | n | n | 1259464 | 269159 | 28528 | 156312 | 328 | Y | n | n | n |
| sort_section_name | 1342 | 4498 | 4524 | 0 | 0 | 2187328 | n | n | 1259532 | 269136 | 26800 | 156312 | 328 | Y | Y | Y | n |
| sort_section_none | 4500 | 4500 | 4524 | 0 | 0 | 2187328 | Y | Y | 1259464 | 269159 | 28528 | 156312 | 328 | Y | Y | Y | Y |
| warn_textrel | 4500 | 4500 | 4524 | 0 | 0 | 2187328 | Y | Y | 1259464 | 269159 | 28528 | 156312 | 328 | Y | Y | Y | Y |
| whole_crypto | 4500 | 4500 | 4524 | 0 | 0 | 2187328 | Y | Y | 1259464 | 269159 | 28528 | 156312 | 328 | Y | Y | Y | Y |
| z_defs | 4500 | 4500 | 4524 | 0 | 0 | 2187328 | Y | Y | 1259464 | 269159 | 28528 | 156312 | 328 | Y | Y | Y | Y |
| z_noexecstack | 4500 | 4500 | 4524 | 0 | 0 | 2187328 | Y | Y | 1259464 | 269159 | 28528 | 156312 | 328 | Y | Y | Y | Y |
| z_norelro | 2932 | 4500 | 4524 | 0 | 0 | 2188456 | n | n | 1259464 | 269159 | 28528 | 156312 | 328 | Y | n | n | n |
| z_relro | 4500 | 4500 | 4524 | 0 | 0 | 2187328 | Y | Y | 1259464 | 269159 | 28528 | 156312 | 328 | Y | Y | Y | Y |

Notes on the columns:

* `byte` / `linked` are `byte_exact` / `linked_exact` from `report.json`.
* `miss` / `extra` are the verifier's missing / extra defined-symbol counts.
* `size` is the final stripped file size in bytes.
* `canon sha` compares the whole-file SHA-256 to the canonical build.
* `shdr` compares the raw 27 x 40-byte section-header table to the original.
* `.text`, `.rodata`, `.exidx`, `.extab`, `.dynamic` are section sizes in bytes.
* `rel.plt` / `rel.dyn` compare section content hashes to the original.

## 5. Per-flag findings

### 5.1 `-z now` — PROVEN REQUIRED

The original is fully RELRO-bound: `DT_FLAGS=BIND_NOW`, `DT_FLAGS_1=NOW`,
`.dynamic` size 328, and `.got` (0x20fcf4) sits **before** `.data` (0x214000)
inside `PT_GNU_RELRO` (0x20cba0, size 0x7460).

Dropping `-z now`:

| property | original / canon | no `-z now` |
|---|---|---|
| `DT_FLAGS` | `BIND_NOW` | absent |
| `DT_FLAGS_1` | `NOW` | absent |
| `.dynamic` size | 328 | 312 |
| `.got` addr | 0x20fcf4 | 0x21296c |
| `.data` addr | 0x214000 | 0x210000 |
| file size | 2,187,328 | 2,188,108 (+780) |
| `byte_exact` | 4500 | 2932 |
| `linked_exact` | 4500 | 4500 |

The `.got`/`.data` order reversal is visible in the section table and is the
direct signature of gold's `-z now` layout. `.rel.plt` and `.rel.dyn` content
also change (hashes `1214cf7b...` / `96a30e4b...` vs original `43bf7c4e...` /
`4f55867c...`). **Conclusion: `-z now` is provably part of the original link.**

### 5.2 `-z relro` / `-z norelro` — relro is on, but `-z relro` is the default

`-z relro` is byte-identical to canonical (whole-file sha256 equal). Gold's
default for this link already emits `PT_GNU_RELRO`; the explicit flag is
neutral. `-z norelro` is **wrong**: `.dynamic` moves to 0x20c000, `.got` to
0x20c148, `.data` to 0x210460, file size 2,188,456 (+1,128), `byte_exact`
2932, `.rel.plt`/`.rel.dyn` hashes differ. So RELRO is proven, but the flag
spelling `-z relro` cannot be distinguished from gold's default.

### 5.3 `-z noexecstack` — NEUTRAL / default

Byte-identical to canonical. Gold already marks the output non-executable
(`PT_GNU_STACK`), so the explicit flag is unobservable.

### 5.4 `--build-id` and build-id behavior — PROVEN (default sha1)

Removing `--build-id` (or `--build-id=none`, which is byte-identical to
removal):

* `.note.gnu.build-id` disappears (26 sections instead of 27),
* `.dynsym`/`.dynstr`/`.hash`/`.rel.*` shift down by 36 bytes (`.dynsym`
  offset 460 vs 496),
* `.shstrtab` shrinks 279 -> 260,
* file size 2,187,268 (-60), `byte_exact` 1750.

`--build-id=sha1` is byte-identical to bare `--build-id`: gold's default style
is SHA-1. `--build-id=tree` is also byte-identical **for this file** (gold's
tree hash falls back to SHA-1 below its size threshold), so tree vs sha1 is
unobservable here. `--build-id=md5` is **wrong**: 16-byte descriptor, note size
32, `byte_exact` 1969.

Build-id is computed over the **unstripped** gold output, with the note
descriptor zeroed:

| file | stored build-id | SHA-1(file with descriptor zeroed) | match |
|---|---|---|---|
| `canon.unstripped.so` (19,353,700 B) | `bafe2bb4386192fc59d580efb27ad6967c2caaae` | `bafe2bb4386192fc59d580efb27ad6967c2caaae` | yes |
| `canon.so` (GNU-stripped) | `bafe2bb4386192fc59d580efb27ad6967c2caaae` | `091aba9acc46d6e615d5dfc8a2e8a956b35d20b9` | no |
| `gold_strip.so` (gold `-s`) | `27fb8e29494f8464ee885d720525ea929c0b8f7a` | `27fb8e29494f8464ee885d720525ea929c0b8f7a` | yes |
| original (GNU-stripped) | `c011810da45777b83ed721b2a87d517efed0d310` | `d7c05dc5e9804023d8786147addc48537b0bee6d` | no |

This is direct proof that the original was linked **without** `-s` and then
GNU-stripped: its build-id is not the hash of the shipped stripped file.
**Conclusion: `--build-id` (sha1) is required; the post-link GNU strip is
required for the build-id to have the original's provenance.**

### 5.5 `--hash-style` — sysv PROVEN, gnu/both WRONG

The original has a SysV `.hash` (38,716 B) and **no** `.gnu.hash`.
`--hash-style=sysv` is byte-identical to canonical, confirming gold's default
is sysv.

* `--hash-style=gnu`: replaces `.hash` with `.gnu.hash`, shifts every later
  section, file size 2,191,428 (+4,100), `byte_exact` 1750, `.rel.plt` /
  `.rel.dyn` hashes differ.
* `--hash-style=both`: adds `.gnu.hash` while keeping `.hash`, 28 sections,
  file size 2,232,428 (+45,100), `.dynamic` 336, `byte_exact` 1750.

**Conclusion: `--hash-style=sysv` (or gold's default) is proven; gnu/both are
disproven.**

### 5.6 `--sort-section` — default `none` PROVEN, `name` WRONG

`--sort-section=none` is byte-identical to canonical. `--sort-section=name`
changes the section order/sizes: `.text` 1,259,532 (+68), `.rodata` 269,136
(-23), `.ARM.exidx` 26,800 (-1,728), `.data.rel.ro` 12,256 (-8), `.bss`
27,060 (+8), `byte_exact` 1342, `linked_exact` 4498 (loses
`PlayerFighter::roll` and `PlayerEgo::roll`). **Conclusion: the original was
not name-sorted; the default is correct.**

### 5.7 `--gc-sections` — WRONG (not used)

`--gc-sections` removes unreferenced sections: `.text` 1,259,412 (-52),
`.ARM.exidx` 26,256 (-2,272), `.rodata` 269,143 (-16), `.bss` 27,036 (-16),
`byte_exact` 2176 (2,324 byte-exact functions lost), `linked_exact` 4500.
The original has no GC'd sections. **Conclusion: `--gc-sections` was not
used.**

### 5.8 `--icf=all` / `--icf=safe` — WRONG (not used)

ICF folds identical code sections: `.text` 1,243,548 / 1,247,588 (-15,916 /
-11,876), `.ARM.exidx` 22,368 / 22,944, file size 2,166,848 / 2,170,944.
`byte_exact` 1347 in both; `--icf=all` also loses one `linked_exact`
(`OnCreateApplication(AbyssEngine::Engine*)`). **Conclusion: no ICF.**

### 5.9 `--fix-cortex-a8` / `--no-fix-cortex-a8` — NEUTRAL / unobservable

Both are byte-identical to canonical. The link has no Cortex-A8 erratum sites
that gold would patch, so the two spellings cannot be distinguished. Gold's
default is therefore behaviorally equivalent to both.

### 5.10 `--no-undefined` / `-z defs` — NEUTRAL

Both are byte-identical to canonical. All 207 imports are satisfied by the
DT_NEEDED shared libraries (`libfmodex.so`, `libfmodevent.so`, GLES, libc,
libm, libdl, ...), so gold's `--no-undefined` has nothing to reject. This does
not prove the original used it; it only proves it is harmless.

### 5.11 `--fatal-warnings` — NEUTRAL

Byte-identical. The link emits no warnings, so the flag is unobservable.

### 5.12 `--warn-shared-textrel` — NEUTRAL

Byte-identical. There are no text relocations, so no warning is emitted.

### 5.13 `--exclude-libs=libgcc.a` — PROVEN REQUIRED

Dropping it exports three libgcc helpers the original keeps local:

```
__aeabi_idiv0  __aeabi_ldiv0  __aeabi_ldivmod
```

Effects: `.dynsym` 89,296 (+48), `.dynstr` 189,252 (+44), `.hash` 38,728
(+12), `.rel.plt` 29,408 (+16), `.plt` 44,132 (+24), `.text` 1,259,480 (+16),
`.got` 17,172 (+8), `extra` 3, `linked_exact` 4492 (8 functions lost),
`byte_exact` 1720. **Conclusion: `--exclude-libs=libgcc.a` is provably part
of the original link.**

### 5.14 `--whole-archive` for zlib / libzip — PROVEN REQUIRED

The canonical link wraps `third_party/zlib/libz_gof2.a` and
`third_party/libzip/libzip_gof2.a` in `--whole-archive ... --no-whole-archive`.
Dropping both:

* 84 defined functions disappear (`_tr_*`, `adler32`, `crc32`, `deflate*`,
  `inflate*`, `compress*`, `uncompress`, `zip_*`, ...),
* `.text` 1,229,320 (-30,144), `.rodata` 252,951 (-16,208),
  `.ARM.exidx` 27,376 (-1,152), `.ARM.extab` 155,460 (-852),
* `.dynsym` 87,664 (-1,584), `.dynstr` 187,884 (-1,324),
* `missing` 84, `linked_exact` 4416, `byte_exact` 1338.

**Conclusion: whole-archive for zlib/libzip is provably part of the original
link.** By contrast, wrapping `libcrypto_gof2.a` in whole-archive is neutral
(byte-identical to canonical), because every libcrypto member the output needs
is already pulled; it cannot be distinguished.

### 5.15 `-s` vs GNU `strip --strip-all` — GNU strip PROVEN

Gold `-s` produces the same file size and the same `byte_exact`/`linked_exact`
counts (4500/4500), but its section-header table differs from the original in
exactly five fields:

| field | original | gold `-s` |
|---|---|---|
| `.rel.plt` `sh_flags` | 66 (`SHF_ALLOC\|SHF_INFO_LINK`) | 2 (`SHF_ALLOC`) |
| `.rel.plt` `sh_info` | 20 | 0 |
| `.fini_array` `sh_entsize` | 4 | 0 |
| `.init_array` `sh_entsize` | 4 | 0 |
| `.bss` `sh_offset` | 2,185,580 | 2,185,584 |

The `.shstrtab` bytes are the same in both cases; the difference is the
section-header normalization bfd performs. The build-id also differs
(`27fb8e29...` vs canonical `bafe2bb4...`) because gold `-s` hashes the
stripped file (see 5.4). **Conclusion: the original used a plain gold link
followed by GNU `strip --strip-all`, not gold `-s`.**

### 5.16 `--no-rosegment` / `--rosegment` — default is no-rosegment; explicit spelling unsupported

The r18b gold binary rejects `--no-rosegment` with
`unknown option: --no-rosegment`. The default is already no separate
read-only segment (the original has only the R E and RW `LOAD`s), so the
explicit flag is not applicable. `--rosegment` is **wrong**: it adds a
read-only segment, shifts `.text`/`.data`/`.got`/`.dynamic`, and drops
`byte_exact` to 1720. **Conclusion: the original used gold's default
(no rosegment); `--rosegment` is disproven.**

### 5.17 `-nostdlib++` — not a gold flag; semantic removal WRONG

`-nostdlib++` is a clang **driver** option, not a gold option. The canonical
link invokes gold directly and passes `libc++abi.a` and `libandroid_support.a`
explicitly. Removing those two archives (the semantic equivalent) loses 98
defined functions (`std::bad_*`, `std::exception` vtables/`what()`, ...),
`.text` 1,222,132 (-37,332), `.rodata` 261,953 (-7,206), `.rel.dyn` 19,056
(-9,688), `missing` 98, `linked_exact` 4402, `byte_exact` 1329. A
`clang++ -### -stdlib=libc++ -nostdlib++` probe confirms the driver drops
`-lc++` (the libc++ runtime). **Conclusion: the C++ runtime archives are
required; `-nostdlib++` would be wrong for this link.**

### 5.18 `-latomic`, `-lunwind`, `-lc++_static`, `-lc++abi`, `-landroid_support`, `-lgcc` — NEUTRAL

Adding any of these to the canonical line is byte-identical to canonical:

* `-latomic` (armv7-a/thumb `libatomic.a`): no member pulled.
* `-lunwind` (`libunwind.a`): no member pulled.
* `-lc++_static`: no member pulled (libc++abi already resolves the C++
  runtime references).
* `-lc++abi` again, `-landroid_support` again, `-lgcc` again: already
  present; re-adding at the front does not change the output.

They are harmless but **unobservable**: the audit cannot prove whether the
original link line contained them.

### 5.19 `-lm`, `-ldl`, `-lc` — REQUIRED, and position matters

Removing any one of them is wrong:

| removed | NEEDED result | `.dynamic` | `byte_exact` | other |
|---|---|---|---|---|
| `-l m` | no `libm.so` | 320 | 1345 | `.text` +9,340, `.rodata` +473, 12 extra exports (`acosl`, `atan2l`, ..., `tanl`) |
| `-l dl` | no `libdl.so` | 320 | 1968 | `.dynstr` -9, `.rel.dyn` hash differs |
| `-l c` | no `libc.so` | 320 | 1750 | `.dynstr` -8, `.hash`/`.rel.plt`/`.rel.dyn` hashes differ |

**Appending** a duplicate `-lc`, `-lm` or `-ldl` at the end of the link line
is byte-identical to canonical. **Prepending** the same duplicate (before the
objects) is **wrong**: it moves that library to the front of NEEDED
(`libc.so liblog.so ...`, `libdl.so liblog.so ...`, `libm.so liblog.so ...`)
and changes `.dynsym`/`.dynstr`/`.hash`/`.rel.plt`/`.rel.dyn`, even though the
section-header table stays identical. **Conclusion: each of `-lm -ldl -lc` is
required exactly once, in the canonical position (after `libgcc.a`), which is
what fixes the NEEDED order.**

## 6. Affected functions

The canonical build's 24 non-byte-exact functions (unchanged by every neutral
variant):

```
Layout::Layout C1/C2                    LevelScript::process
Status::missionCompleted                Level::createShip
Radar::draw                             MGame::OnTouchEnd
Generator::createMission                Globals::Globals C1/C2
HangarWindow::render                    RocketGun::update
PlayerEgo::calcCollision                Level::createScene
Gun::calcCharacterCollision             Level::createMission
Hud::draw                               MenuTouchWindow::MenuTouchWindow C1/C2
SpaceLounge::drawLounge                 MenuTouchWindow::OnTouchBegin
ModStation::ModStation C1/C2            MGame::OnUpdate
```

Per-variant set differences against canonical (`byte lost` = functions that
are `byte_exact` in canonical but not in the variant; `linked lost` = same for
`linked_exact`):

| variant | byte lost | byte gained | linked lost | missing | extra | linked-lost names | missing/extra names |
|---|---:|---:|---:|---:|---:|---|---|
| buildid_md5 | 2531 | 0 | 0 | 0 | 0 |  |  |
| buildid_none | 2750 | 0 | 0 | 0 | 0 |  |  |
| buildid_sha1 | 0 | 0 | 0 | 0 | 0 |  |  |
| buildid_tree | 0 | 0 | 0 | 0 | 0 |  |  |
| canon | 0 | 0 | 0 | 0 | 0 |  |  |
| fatal_warnings | 0 | 0 | 0 | 0 | 0 |  |  |
| fix_cortex | 0 | 0 | 0 | 0 | 0 |  |  |
| gc_sections | 2324 | 0 | 0 | 0 | 0 |  |  |
| gold_strip | 0 | 0 | 0 | 0 | 0 |  |  |
| hash_both | 2750 | 0 | 0 | 0 | 0 |  |  |
| hash_gnu | 2750 | 0 | 0 | 0 | 0 |  |  |
| hash_sysv | 0 | 0 | 0 | 0 | 0 |  |  |
| icf_all | 3153 | 0 | 1 | 0 | 0 | _Z19OnCreateApplicationPN11AbyssEngine6EngineE |  |
| icf_safe | 3153 | 0 | 0 | 0 | 0 |  |  |
| landroid_support_again | 0 | 0 | 0 | 0 | 0 |  |  |
| latomic | 0 | 0 | 0 | 0 | 0 |  |  |
| lc_again | 0 | 0 | 0 | 0 | 0 |  |  |
| lc_append | 0 | 0 | 0 | 0 | 0 |  |  |
| lcxx_static | 0 | 0 | 0 | 0 | 0 |  |  |
| lcxxabi_again | 0 | 0 | 0 | 0 | 0 |  |  |
| ldl_again | 0 | 0 | 0 | 0 | 0 |  |  |
| ldl_append | 0 | 0 | 0 | 0 | 0 |  |  |
| lgcc_again | 0 | 0 | 0 | 0 | 0 |  |  |
| lm_again | 0 | 0 | 0 | 0 | 0 |  |  |
| lm_append | 0 | 0 | 0 | 0 | 0 |  |  |
| lunwind | 0 | 0 | 0 | 0 | 0 |  |  |
| no_buildid | 2750 | 0 | 0 | 0 | 0 |  |  |
| no_exclude_libs | 2780 | 0 | 8 | 0 | 3 | _ZN10NewsTicker13replaceTokensEN11AbyssEngine6StringE, _ZN11AbyssEngine17TextureConference14UpdateMeshDataEPNS_4MeshEPNS_6EngineE, _ZN11AbyssEngine18ApplicationManager8OnUpdateEx, _ZN11AbyssEngine9Transform6UpdateExb, _ZN12Achievements16checkForNewMedalEP9PlayerEgo, _ZN7Globals16longToTimeStringExRN11AbyssEngine6StringE … (+2) | __aeabi_idiv0, __aeabi_ldiv0, __aeabi_ldivmod |
| no_lc | 2750 | 0 | 0 | 0 | 0 |  |  |
| no_ldl | 2532 | 0 | 0 | 0 | 0 |  |  |
| no_lm | 3155 | 0 | 0 | 0 | 12 |  | acosl, atan2l, atanl, cosl, expl, frexpl … (+6) |
| no_undefined | 0 | 0 | 0 | 0 | 0 |  |  |
| no_whole_archive | 3162 | 0 | 84 | 84 | 0 | _tr_align, _tr_flush_block, _tr_init, _tr_stored_block, _tr_tally, _zip_error_strerror … (+78) | _tr_align, _tr_flush_block, _tr_init, _tr_stored_block, _tr_tally, _zip_error_strerror … (+78) |
| no_z_now | 1568 | 0 | 0 | 0 | 0 |  |  |
| nofix_cortex | 0 | 0 | 0 | 0 | 0 |  |  |
| nostdlibpp | 3171 | 0 | 98 | 98 | 0 | _ZNKSt10bad_typeid4whatEv, _ZNKSt13bad_exception4whatEv, _ZNKSt16bad_array_length4whatEv, _ZNKSt20bad_array_new_length4whatEv, _ZNKSt8bad_cast4whatEv, _ZNKSt9bad_alloc4whatEv … (+92) | _ZNKSt10bad_typeid4whatEv, _ZNKSt13bad_exception4whatEv, _ZNKSt16bad_array_length4whatEv, _ZNKSt20bad_array_new_length4whatEv, _ZNKSt8bad_cast4whatEv, _ZNKSt9bad_alloc4whatEv … (+92) |
| rosegment | 2780 | 0 | 0 | 0 | 0 |  |  |
| sort_section_name | 3158 | 0 | 2 | 0 | 0 | _ZN13PlayerFighter4rollEi, _ZN9PlayerEgo4rollEi |  |
| sort_section_none | 0 | 0 | 0 | 0 | 0 |  |  |
| warn_textrel | 0 | 0 | 0 | 0 | 0 |  |  |
| whole_crypto | 0 | 0 | 0 | 0 | 0 |  |  |
| z_defs | 0 | 0 | 0 | 0 | 0 |  |  |
| z_noexecstack | 0 | 0 | 0 | 0 | 0 |  |  |
| z_norelro | 1568 | 0 | 0 | 0 | 0 |  |  |
| z_relro | 0 | 0 | 0 | 0 | 0 |  |  |

Interpretation:

* Variants that shift section addresses (`hash_gnu`, `hash_both`,
  `no_buildid`, `buildid_none`, `buildid_md5`, `no_z_now`, `z_norelro`,
  `rosegment`, `no_lc`, `no_ldl`, `no_lm`, `no_exclude_libs`,
  `no_whole_archive`, `nostdlibpp`, `gc_sections`, `icf_*`,
  `sort_section_name`) lose `byte_exact` broadly because every PC-relative
  literal / GOT / PLT word in the affected functions changes. The count is the
  useful number; the names are essentially "all functions whose address-
  dependent words moved".
* `linked_exact` losses are small and specific: `icf_all` loses
  `OnCreateApplication(AbyssEngine::Engine*)`; `sort_section_name` loses
  `PlayerFighter::roll` and `PlayerEgo::roll`; `no_exclude_libs` loses the 8
  functions listed above (all callers of the newly exported libgcc helpers or
  affected by the `.text`/`.plt` shift).
* `no_whole_archive` and `nostdlibpp` lose defined symbols entirely (84 and
  98 missing), so their `cmp` drops from 4,524 to 4,440 and 4,426.

## 7. Confidence table

| claim | confidence | hard evidence |
|---|---|---|
| `-z now` is in the original link | **high** | original `DT_FLAGS=BIND_NOW`, `DT_FLAGS_1=NOW`, `.dynamic` 328, `.got` before `.data`; dropping it gives `.dynamic` 312, `.got` after `.data`, byte 2932 |
| `--build-id` (sha1) is in the original link | **high** | original has a 20-byte GNU build-id note; removing it drops 60 bytes / 2,750 byte-exact; `--build-id=sha1` is byte-identical to default |
| build-id is hashed over the unstripped output | **high** | `canon.unstripped.so`: SHA-1(descriptor zeroed) == stored build-id; GNU-stripped `canon.so`: it does not |
| GNU `strip --strip-all` (not gold `-s`) is the post-link step | **high** | gold `-s` leaves 5 section-header fields different (`.rel.plt` flags/info, init/fini entsize, `.bss` offset); build-id provenance differs |
| `--hash-style=sysv` (gold default) | **high** | original has `.hash`, no `.gnu.hash`; `--hash-style=sysv` byte-identical; gnu/both shift layout and drop byte to 1750 |
| `--exclude-libs=libgcc.a` | **high** | removing it exports `__aeabi_idiv0`, `__aeabi_ldiv0`, `__aeabi_ldivmod` (+3 extra, linked 4492) |
| `--whole-archive` for zlib/libzip | **high** | removing it loses 84 zlib/libzip functions, `.text` -30,144, linked 4416 |
| `-lm -ldl -lc` required once, in canonical order | **high** | removing any one changes NEEDED and drops byte to 1345/1968/1750; appending duplicates is neutral; prepending duplicates reorders NEEDED |
| `libc++abi.a` + `libandroid_support.a` required | **high** | removing them loses 98 C++ runtime functions, linked 4402 |
| no `--gc-sections` | **high** | `.text` -52, `.ARM.exidx` -2,272, byte 2176 |
| no `--icf` | **high** | `.text` -15,916 / -11,876, byte 1347 |
| no `--rosegment` | **high** | `--rosegment` shifts layout, byte 1720; r18b gold default is no rosegment |
| default `--sort-section=none` (not `name`) | **high** | `--sort-section=name` changes `.text` +68 / `.ARM.exidx` -1,728, linked 4498 |
| `-z relro` is gold's default | **high** | `-z relro` byte-identical; `-z norelro` shifts `.got`/`.dynamic`, byte 2932 |
| `-z noexecstack` is neutral/default | **high** | byte-identical |
| `--no-undefined` / `-z defs` neutral | **high** | byte-identical; all 207 imports resolve against DT_NEEDED libs |
| `--fatal-warnings` / `--warn-shared-textrel` neutral | **high** | byte-identical; no warnings / no text relocs |
| `--fix-cortex-a8` vs `--no-fix-cortex-a8` | **medium** | both byte-identical; no erratum site, so the audit cannot distinguish them |
| `-latomic` / `-lunwind` / `-lc++_static` | **medium** | byte-identical; no archive member pulled, so presence is unobservable |
| `--whole-archive` around libcrypto | **medium** | byte-identical; all needed members already pulled |
| `--build-id=tree` vs sha1 | **medium** | byte-identical for this file (tree falls back to SHA-1 below threshold) |
| `--build-id=md5` is wrong | **high** | 16-byte descriptor, note size 32, byte 1969 |
| `--no-rosegment` explicit spelling | **high** | r18b gold: `unknown option: --no-rosegment`; default behavior is no rosegment |
| `-nostdlib++` would be wrong | **high** | not a gold flag; semantic removal loses 98 functions |

## 8. Is there a strict improvement?

**No.** No tested flag variant exceeds the canonical gate:

* max `byte_exact` = 4500 (canonical and all neutral variants),
* max `linked_exact` = 4500 (canonical; `icf_all` 4499, `sort_section_name`
  4498, `no_exclude_libs` 4492, `no_whole_archive` 4416, `nostdlibpp` 4402),
* max `size_match` = 4524 (canonical and most variants),
* canonical already has the original's 27 section headers byte-for-byte,
  byte-identical `.rodata`/`.data`/`.data.rel.ro`/`.bss`/`.got`/`.plt`/
  `.dynamic`/`.dynsym`/`.dynstr`/`.hash`/`.rel.plt`/`.rel.dyn`/`.shstrtab`,
  and identical NEEDED order + dynamic tags.

The remaining differences (`.text` content for 24 functions, `.ARM.exidx`,
`.ARM.extab`, build-id descriptor) are source-level codegen / EH-layout
differences, not link-flag differences. No link flag can close them without
changing the object code. Therefore **the canonical link line is already the best link-flag
configuration among the audited flags**; this audit commits the note only,
with no build or source change.

## 9. Reproduction / artifacts

* Harness: `/dev/shm/w5203/relink.py` (parses the exact Ninja link command,
  substitutes the output path, optionally adds/drops flags or libraries,
  re-runs GNU strip + `tools/reorder_shstrtab.py`).
* Variant `.so` files: `/dev/shm/w5203/variants/<name>.so`.
* Per-variant verifier reports: `/dev/shm/w5203/verify/<name>/report.json`.
* ELF comparison helper: `/dev/shm/w5203/elfcompare.py`.
* Raw variant matrix: `/dev/shm/w5203/table.md`;
  affected-function matrix: `/dev/shm/w5203/affected.md`.

These `/dev/shm` artifacts are scratch; the numbers above are the durable
record.

### Exact variant invocations

`relink.py` was called as:

```sh
/dev/shm/w5203/relink.py <name> [--add '<gold flags>'] [--append '<gold flags>'] \
  [--drop '<literal tokens>'] [--drop-lib '<path substring>'] [--gold-strip] [--keep-unstripped]
```

Examples:

```sh
# canonical (must reproduce the canonical .so byte-for-byte)
/dev/shm/w5203/relink.py canon

# drop -z now
/dev/shm/w5203/relink.py no_z_now --drop "-z now"

# add a flag after --exclude-libs=libgcc.a
/dev/shm/w5203/relink.py hash_gnu --add "--hash-style=gnu"

# append a duplicate library at the end (neutral)
/dev/shm/w5203/relink.py lc_append --append "-lc"

# prepend a duplicate library before the objects (changes NEEDED order)
/dev/shm/w5203/relink.py lc_again --add "-lc"

# remove a library token
/dev/shm/w5203/relink.py no_lm --drop "-l m"
```

Each variant was then verified with the command in section 2. The canonical
`canon` variant reproduced the canonical `.so` exactly
(sha256 `7008e892...`), which validates the harness.

## 10. Summary classification

| class | flags |
|---|---|
| **Provable / required** | `-z now`; `--build-id` (sha1); `--hash-style=sysv` (default); `--exclude-libs=libgcc.a`; `--whole-archive` zlib/libzip; GNU `strip --strip-all` after link; `-lm -ldl -lc` once in canonical order; `libc++abi.a` + `libandroid_support.a`; no `--gc-sections`; no `--icf`; no `--rosegment`; default `--sort-section=none` |
| **Neutral (byte-identical)** | `-z relro`; `-z noexecstack`; `--build-id=sha1`; `--build-id=tree`; `--hash-style=sysv`; `--sort-section=none`; `--fix-cortex-a8`; `--no-fix-cortex-a8`; `--no-undefined`; `-z defs`; `--fatal-warnings`; `--warn-shared-textrel`; `-latomic`; `-lunwind`; `-lc++_static`; `-lc++abi` (again); `-landroid_support` (again); `-lgcc` (again); `--whole-archive` around libcrypto; appended duplicate `-lc`/`-lm`/`-ldl` |
| **Wrong (changes output away from original)** | `-z norelro`; `--hash-style=gnu`; `--hash-style=both`; `--build-id=none`; `--build-id=md5`; `--sort-section=name`; `--gc-sections`; `--icf=all`; `--icf=safe`; `--rosegment`; removing `--exclude-libs=libgcc.a`; removing `--whole-archive`; removing `-lm`/`-ldl`/`-lc`; prepending duplicate `-lm`/`-ldl`/`-lc`; gold `-s`; `-nostdlib++` semantic removal |
| **Unobservable / unknown** | `--no-rosegment` (unsupported by r18b gold; default behavior is no rosegment); `--fix-cortex-a8` vs `--no-fix-cortex-a8`; `-latomic`/`-lunwind`/`-lc++_static` presence; `--no-undefined`/`-z defs` presence; `--fatal-warnings`/`--warn-shared-textrel` presence; `--build-id=tree` vs sha1; `--whole-archive` around libcrypto; appended duplicate `-l` flags |
