# Link-flag forensics: reconstructing the original gold command line

Status: research note (2026-11-01). Worktree `wt/w3303`, branch `research-w3303`,
based on master `81db17d1`. All numbers below were measured against
`../_work/bins/android_2.0.16_libgof2hdaa.so` with the repo's NDK r18b gold 1.12
(`.cache/ndk/android-ndk-r18b/.../ld.gold`).

## TL;DR

**The only missing link flag is `-z now`.** Adding it to the existing gold line:

* reproduces `DT_FLAGS = BIND_NOW` and `DT_FLAGS_1 = NOW`;
* moves `.got` to the original address `0x20fcf4`, `.data` to `0x214000`, and
  `.dynamic` to `0x148` at `0x20fba4`;
* puts `.got` before `.data` and covers it with `PT_GNU_RELRO`;
* makes `.got` (0x430c bytes) and `.plt` (0xac4c bytes) **byte-identical** to the
  original (0 differing bytes);
* raises the gate's `byte_exact` from **1415 to 1458 (+43)** with `linked_exact`
  unchanged at **4493**, `size-exact` unchanged at **4522/4523**, `.text` unchanged
  at **0x1343d0**, lint `CLEAN`, drift `0`.

Every other remaining section-size/address delta is compile/TU-caused, not link-flag
caused. The full `check` gate with `-z now` prints:

```
verify: byte 1458  linked 4493  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
  GAIN  byte_exact 1415 -> 1458 (lock with --update-baseline)
RATCHET PASS
```

Baseline (`master` HEAD, no `-z now`): `byte 1415  linked 4493  stubs 0  missing 0
extra 0`, `size-exact 4522/4523`, `.text 0x1343d0`, lint `CLEAN`, drift `0`.

## 1. Reproduction

### Baseline build

```
cmake --preset default
cmake --build cmake-build-match --target check -j32
```

### Extract the gold command

The link command is the `COMMAND` of the `libgof2hdaa.so` rule in
`cmake-build-match/build.ninja`. It is reproduced by this script (the exact command
used for every A/B below; only `"$@"` changes):

```bash
#!/bin/bash
# /tmp/lf/relink.sh OUT [extra gold args...]
set -e
OUT="$1"; shift
B=/home/debian/GalaxyOnFire2/wt/w3303/cmake-build-match
NDK=/home/debian/GalaxyOnFire2/wt/w3303/.cache/ndk/android-ndk-r18b
GOLD=$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/arm-linux-androideabi/bin/ld.gold
STRIP=$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-strip
"$GOLD" -shared -soname libgof2hdaa.so --build-id "$@" \
  --unresolved-symbols=ignore-all \
  --exclude-libs=libgcc.a \
  --section-ordering-file=$B/section_order.txt \
  -o "$OUT" \
  $B/crtbegin_so_renamed.o -L $NDK/platforms/android-16/arch-arm/usr/lib \
  @$B/link_order.rsp \
  /home/debian/GalaxyOnFire2/wt/w3303/third_party/openssl/libcrypto_gof2.a \
  --whole-archive /home/debian/GalaxyOnFire2/wt/w3303/third_party/zlib/libz_gof2.a \
                  /home/debian/GalaxyOnFire2/wt/w3303/third_party/libzip/libzip_gof2.a \
  --no-whole-archive \
  -l log -l GLESv2 -l GLESv1_CM -l EGL -l android \
  /home/debian/GalaxyOnFire2/wt/w3303/../_work/bins/apk_libs/libfmodex.so \
  /home/debian/GalaxyOnFire2/wt/w3303/../_work/bins/apk_libs/libfmodevent.so \
  $NDK/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/libc++abi.a \
  -l m \
  $NDK/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/libandroid_support.a \
  $NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/lib/gcc/arm-linux-androideabi/4.9.x/armv7-a/thumb/libgcc.a \
  -l dl -l c \
  $NDK/platforms/android-16/arch-arm/usr/lib/crtend_so.o
"$STRIP" --strip-all "$OUT"
```

`/tmp/lf/relink.sh /tmp/lf/now.so -z now` is the reference `-z now` link.

### Verify a relinked `.so`

```
python3 tools/verify/verify.py \
  --build-dir /tmp/lf/verify_now \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/lf/now.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
```

### Structural fingerprints

The section/phdr/dynamic dump used for every A/B is a small wrapper around
`readelf -SW`, `readelf -lW`, `readelf -dW`, `readelf -nW` (the script is
`/tmp/lf/fp.py`; it prints one line per section, program header, dynamic tag and
note). Equivalent one-off commands:

```
readelf -hW  OLD.so ; readelf -lW OLD.so ; readelf -SW OLD.so
readelf -dW  OLD.so ; readelf -nW OLD.so ; readelf -AW OLD.so
readelf -p .comment OLD.so
```

## 2. Program headers

Original vs current baseline vs `-z now`:

| phdr | orig | baseline | `-z now` |
|---|---|---|---|
| count | 8 | 8 | 8 |
| PHDR | off 0x34, va 0x34, fs/ms 0x100, R, al 4 | same | same |
| LOAD #1 | off 0, va 0, fs/ms 0x20b977, R E, al 0x1000 | fs/ms 0x20b8c7 | fs/ms 0x20b8c7 |
| LOAD #2 | off 0x20bba0, va 0x20cba0, fs 0x9dcc, ms 0x1077c, RW, al 0x1000 | off 0x20bea0, va 0x20cea0, fs 0x9de0, ms 0x107b8 | off 0x20bb80, va 0x20cb80, fs 0x9df4, ms 0x107d8 |
| DYNAMIC | off 0x20eba4, va 0x20fba4, fs/ms 0x148, RW, al 4 | off 0x20eec4, va 0x20fec4, fs/ms 0x138 | **exact: off 0x20eba4, va 0x20fba4, fs/ms 0x148** |
| NOTE | off 0x134, va 0x134, fs/ms 0xbc, R, al 4 | same | same |
| GNU_STACK | off 0, va 0, fs/ms 0, RW, al 0x10 | same | same |
| ARM_EXIDX | off/va 0x19cc08, fs/ms 0x6f70, R, al 4 | off/va 0x19d810, fs/ms 0x66c8 | off/va 0x19d810, fs/ms 0x66c8 |
| GNU_RELRO | off 0x20bba0, va 0x20cba0, fs/ms 0x7460, RW, al 0x10 | off 0x20bea0, va 0x20cea0, fs/ms 0x3160 | off 0x20bb80, va 0x20cb80, fs/ms 0x7480 |

With `-z now` the program-header **types, order, flags and alignments are exactly the
original's**. The only remaining numeric deltas are:

* `LOAD #1` filesz `-0xb0`: net of `.text +0xc08`, `.ARM.exidx -0x8a8`,
  `.ARM.extab +0xc`, `.rodata -0x420` (plus 4 bytes of `.rodata` alignment).
* `LOAD #2` / `GNU_RELRO` start `-0x20`: caused by `.data.rel.ro` being `+0x20`
  larger. Gold places the RELRO segment so that its *end* is page-aligned:
  `relro_off = align_up(rx_end + relro_size, 0x1000) - relro_size`. A `+0x20`
  RELRO size therefore moves its start `-0x20`. This is visible directly: the
  original `.fini_array` is `0x20cba0`, ours is `0x20cb80`, while both
  `.init_array` (0x20fb98), `.dynamic` (0x20fba4) and `.got` (0x20fcf4) match.
* `LOAD #2` filesz `+0x28`: `.data.rel.ro +0x20` plus the extra `.aezdata` 8 bytes.
* `LOAD #2` memsz `+0x5c`: `.bss +0x2c` plus the `.aezdata`/alignment effects.

`-z norelro` removes `PT_GNU_RELRO` (7 phdrs) and changes the RW section order;
`--rosegment` adds a third `LOAD` (9 phdrs). Neither matches the original.

## 3. Section table

Original vs baseline vs `-z now` (addresses/sizes from `readelf -SW`):

| section | orig addr | orig size | base size | `-z now` addr | `-z now` size | now−orig |
|---|---:|---:|---:|---:|---:|---:|
| .note.android.ident | 0x134 | 0x98 | 0x98 | 0x134 | 0x98 | 0 |
| .note.gnu.build-id | 0x1cc | 0x24 | 0x24 | 0x1cc | 0x24 | 0 |
| .dynsym | 0x1f0 | 0x15ca0 | 0x15ca0 | 0x1f0 | 0x15ca0 | 0 |
| .dynstr | 0x15e90 | 0x2e318 | 0x2e318 | 0x15e90 | 0x2e318 | 0 |
| .hash | 0x441a8 | 0x973c | 0x973c | 0x441a8 | 0x973c | 0 |
| .gnu.version | 0x4d8e4 | 0x2b94 | 0x2b94 | 0x4d8e4 | 0x2b94 | 0 |
| .gnu.version_d | 0x50478 | 0x1c | 0x1c | 0x50478 | 0x1c | 0 |
| .gnu.version_r | 0x50494 | 0x40 | 0x40 | 0x50494 | 0x40 | 0 |
| .rel.dyn | 0x504d4 | 0x7048 | 0x7048 | 0x504d4 | 0x7048 | 0 |
| .rel.plt | 0x5751c | 0x72d0 | 0x72d0 | 0x5751c | 0x72d0 | 0 |
| .plt | 0x5e7ec | 0xac4c | 0xac4c | 0x5e7ec | 0xac4c | 0 |
| .text | 0x69440 | 0x1337c8 | 0x1343d0 | 0x69440 | 0x1343d0 | +0xc08 |
| .ARM.exidx | 0x19cc08 | 0x6f70 | 0x66c8 | 0x19d810 | 0x66c8 | −0x8a8 |
| .ARM.extab | 0x1a3b78 | 0x26298 | 0x262a4 | 0x1a3ed8 | 0x262a4 | +0xc |
| .rodata | 0x1c9e10 | 0x41b67 | 0x41747 | 0x1ca180 | 0x41747 | −0x420 |
| .fini_array | 0x20cba0 | 0x8 | 0x8 | 0x20cb80 | 0x8 | 0 |
| .data.rel.ro | 0x20cbb0 | 0x2fe8 | 0x3008 | 0x20cb90 | 0x3008 | +0x20 |
| .init_array | 0x20fb98 | 0xc | 0xc | 0x20fb98 | 0xc | 0 |
| .dynamic | 0x20fba4 | 0x148 | 0x138 | 0x20fba4 | 0x148 | 0 |
| .got | 0x20fcf4 | 0x430c | 0x430c | 0x20fcf4 | 0x430c | 0 |
| .data | 0x214000 | 0x296c | 0x296c | 0x214000 | 0x296c | 0 |
| .bss | 0x216970 | 0x69ac | 0x69d8 | 0x216980 | 0x69d8 | +0x2c |
| .comment | 0 | 0x12f | 0x12f | 0 | 0x12f | 0 |
| .note.gnu.gold-version | 0 | 0x1c | 0x1c | 0 | 0x1c | 0 |
| .ARM.attributes | 0 | 0x38 | 0x38 | 0 | 0x38 | 0 |
| .shstrtab | 0 | 0x117 | 0x120 | 0 | 0x120 | +0x9 |

Extra section in our link: `.aezdata` (WA, addr 0x21696c, size 8, align 1). It is
emitted by `src/engine/file/AEFile.cpp:7` (`#pragma clang section data=".aezdata"`)
and does not exist in the original. It is the sole cause of the `.bss` address
`+0x10` and the `.shstrtab` `+0x9` (the 9-byte string `.aezdata`). It is a source
artifact, not a link flag: `--gc-sections` removes it (because nothing references
it), which makes `.bss` land at exactly `0x216970` and drops the section.

Section order with `-z now` is identical to the original except the extra
`.aezdata` between `.data` and `.bss`. All common sections have the same type,
flags, alignment and `entsize`; `.rel.plt`'s `sh_info` is 20 in both (the baseline
without `-z now` has 22, another observable of `-z now`).

## 4. `.dynamic`

With `-z now` the dynamic section is **tag-for-tag identical** to the original
(36 entries, same order, same values), except `DT_FINI_ARRAY` is `0x20cb80`
instead of `0x20cba0` (the RELRO-start shift above). Without `-z now` the section
is `0x138` and lacks `DT_FLAGS`/`DT_FLAGS_1` entirely.

Original tag order (all present and matching with `-z now`):

```
PLTGOT, PLTRELSZ, JMPREL, PLTREL, REL, RELSZ, RELENT, RELCOUNT,
SYMTAB, SYMENT, STRTAB, STRSZ, HASH, NEEDED x10, SONAME,
FINI_ARRAY, FINI_ARRAYSZ, INIT_ARRAY, INIT_ARRAYSZ,
FLAGS (BIND_NOW), FLAGS_1 (NOW), VERSYM, VERDEF, VERDEFNUM,
VERNEED, VERNEEDNUM, NULL
```

The 10 `DT_NEEDED` entries and their order are exactly:
`liblog, libGLESv2, libGLESv1_CM, libEGL, libandroid, libfmodex, libfmodevent,
libm, libdl, libc`.

Observables that pin the defaults:

* `--spare-dynamic-tags=0` (with `-z now`) → `.dynamic` `0x120` at `0x20fbd4`,
  `.init_array` moves to `0x20fbc8`; `=10` → `0x170` at `0x20fb84`.
  (Without `-z now` the same values are `0x110`/`0x160`.) The default `5` is
  required for `.dynamic 0x148` at `0x20fba4`.
* `-z nocombreloc` drops `DT_RELCOUNT` → `.dynamic 0x140`. The original has
  `RELCOUNT = 1162`, so `-z combreloc` (default) is required.
* `--as-needed` drops 4 `NEEDED` (`libGLESv1_CM`, `libEGL`, `libandroid`,
  `libdl`) → `.dynamic 0x128`. `--no-as-needed` (default) is required.

## 5. `.dynsym`, `.dynstr`, `.hash`

Sizes are **exactly equal** to the original:

| section | orig | ours (`-z now`) |
|---|---:|---:|
| .dynsym | 0x15ca0 (5578 entries) | 0x15ca0 |
| .dynstr | 0x2e318 | 0x2e318 |
| .hash | 0x973c | 0x973c |
| .gnu.version | 0x2b94 | 0x2b94 |
| .gnu.version_d | 0x1c | 0x1c |
| .gnu.version_r | 0x40 | 0x40 |

The `.hash` parameters are therefore identical: `nbucket = 4099`, `nchain = 5578`
(`(2 + 4099 + 5578)*4 = 0x973c`). The **contents differ** because the symbol
order differs: only 789 of 5577 names are at the same index. The symbol *set* is
the same; two function sizes differ (`_ZN11AbyssEngine13getAppVersionEv` 0 vs 2,
`_ZN9Generator13createMission...` 1996 vs 1992). This is a TU/source-order
difference, not a link flag: no tested flag changes the dynsym order except
`--sort-section=name`, which makes `.text`/byte matching worse.

The original's first symbols are `__cxa_atexit`, `__cxa_finalize`, the JNI entry,
then `__aeabi_unwind_cpp_pr0`; ours put the NFC symbols at index 4. That is the
same TU-membership/within-object symbol-order issue.

## 6. `.rel.dyn` and `.rel.plt`

Relocation **type counts are exactly equal**:

```
1817 R_ARM_ABS32
 614 R_ARM_GLOB_DAT
3674 R_ARM_JUMP_SLOT
1162 R_ARM_RELATIVE
```

`.rel.plt`: 3674 entries, same `r_offset` sequence (3674/3674 identical offsets),
same types. The symbol *names* at those offsets diverge at entry 81
(original `_ZN11AbyssEngine16TransformRelease...`; ours `glGenBuffers`). The first
81 PLT slots match. This is a link-order artifact of the input section/TU order,
not a flag: `-z nocombreloc`, `--sort-section=name`, `--gc-sections`,
`--icf=all`, and removing the section-ordering file all leave the first-81 /
then-diverge pattern unchanged.

`.rel.dyn`: 3593 entries, same types. `r_offset`s differ because the
`.data.rel.ro` layout differs (deltas of +8, −96, −28, −32, …; not a uniform
shift). `R_ARM_RELATIVE` addends follow the `.rodata`/`.text` shifts. This is
compile/layout-caused.

## 7. `.got` layout

With `-z now` the `.got` is **byte-for-byte identical** to the original:

* `.got` at `0x20fcf4`, size `0x430c` (4291 4-byte slots).
* `DT_PLTGOT = 0x21068c` = `.got + 0x998` (2456 bytes = 614 slots).
* First 614 slots are the `R_ARM_GLOB_DAT` entries; the next 3674 are the
  `R_ARM_JUMP_SLOT` slots (`0x998 + 3674*4 = 0x4300`); the final 3 slots are
  reserved. The original has the same split and the same bytes.
* `.plt` is also byte-for-byte identical with `-z now` (0 differing bytes out of
  `0xac4c`). Without `-z now`, `.plt` differs by 11024 bytes because its literal
  pool holds the (different) GOT slot addresses.

This is the strongest single observable for `-z now`: it is not just a section
address change, it makes the PLT/GOT machinery byte-identical.

## 8. `.note.gnu.build-id`, `.comment`, `.ARM.attributes`

* `.note.gnu.build-id` is 0x24 bytes (20-byte SHA-1) in the original.
  `--build-id=sha1` and the default `--build-id` both give 0x24;
  `--build-id=md5` gives 0x20; `--build-id=none` removes the section and shifts
  `.dynsym` etc. The original therefore used the default SHA-1 build-id.
  The ID value itself differs (`c011810d…` vs `7d151a0e…`) because the output
  content differs; it will match only when the whole `.so` matches.
* `.note.android.ident` (0x98), `.comment` (0x12f), `.note.gnu.gold-version`
  (0x1c, "gold 1.12") and `.ARM.attributes` (0x38) are **byte-identical** to the
  original. `.comment` contains the same clang 7.0.2 and GCC 4.9.x strings.

## 9. Flag A/B results

All variants below are relinks of the existing objects with the script in §1.
"byte" is the verify `byte_exact` count (baseline 1415, `-z now` 1458). Structural
columns are section sizes unless the cell is an address (`got/data/bss`).

| variant | extra flag(s) | byte | .text | exidx | extab | rodata | data.rel.ro | .dynamic | .got | .data | .bss | phdrs | extra sect |
|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| orig | — | — | 1337c8 | 6f70 | 26298 | 41b67 | 2fe8 | 148 | 20fcf4 | 214000 | 216970/69ac | 8 | — |
| base | — | 1415 | 1343d0 | 66c8 | 262a4 | 41747 | 3008 | 138 | 212974 | 210000 | 216c80/69d8 | 8 | .aezdata |
| **now** | **-z now** | **1458** | 1343d0 | 66c8 | 262a4 | 41747 | 3008 | **148** | **20fcf4** | **214000** | 216980/69d8 | 8 | .aezdata |
| nowrelro | -z now -z relro | 1458 | 1343d0 | 66c8 | 262a4 | 41747 | 3008 | 148 | 20fcf4 | 214000 | 216980/69d8 | 8 | .aezdata |
| nownorelro | -z now -z norelro | — | 1343d0 | 66c8 | 262a4 | 41747 | 3008 | 148 | 20c148 | 210460 | 215e00 | 7 | .aezdata |
| relro | -z relro | 1415 | 1343d0 | 66c8 | 262a4 | 41747 | 3008 | 138 | 212974 | 210000 | 216c80/69d8 | 8 | .aezdata |
| norelro | -z norelro | — | 1343d0 | 66c8 | 262a4 | 41747 | 3008 | 138 | 211adc | 20c140 | 215df0 | 7 | .aezdata |
| noexecstack | -z noexecstack | 1415 | 1343d0 | 66c8 | 262a4 | 41747 | 3008 | 138 | 212974 | 210000 | 216c80/69d8 | 8 | .aezdata |
| execstack | -z execstack | — | 1343d0 | 66c8 | 262a4 | 41747 | 3008 | 138 | 212974 | 210000 | 216c80/69d8 | 8 | .aezdata (GNU_STACK RWE) |
| hashsysv | --hash-style=sysv | 1415 | 1343d0 | 66c8 | 262a4 | 41747 | 3008 | 138 | 212974 | 210000 | 216c80/69d8 | 8 | .aezdata |
| hashgnu | --hash-style=gnu | — | 1343d0 | 66c8 | 262a4 | 41747 | 3008 | 138 | 213974 | 211000 | 217c80 | 8 | .gnu.hash, .aezdata (no .hash) |
| hashboth | --hash-style=both | — | 1343d0 | 66c8 | 262a4 | 41747 | 3008 | 140 | 21c974 | 21a000 | 220c80 | 8 | .gnu.hash, .aezdata |
| bidsha1 | --build-id=sha1 | 1415 | 1343d0 | 66c8 | 262a4 | 41747 | 3008 | 138 | 212974 | 210000 | 216c80/69d8 | 8 | .aezdata |
| bidmd5 | --build-id=md5 | — | 1343d0 | 66c8 | 262a4 | 41747 | 3008 | 138 | 212974 | 210000 | 216c80/69d8 | 8 | .aezdata (note 0x20) |
| bidnone | --build-id=none | — | 1343d0 | 66c8 | 262a4 | 41747 | 3008 | 138 | 212974 | 210000 | 216c80/69d8 | 8 | .aezdata (no note) |
| maxpage4096 | -z max-page-size=4096 | 1415 | 1343d0 | 66c8 | 262a4 | 41747 | 3008 | 138 | 212974 | 210000 | 216c80/69d8 | 8 | .aezdata |
| maxpage65536 | -z max-page-size=65536 | — | 1343d0 | 66c8 | 262a4 | 41747 | 3008 | 138 | 222974 | 220000 | 226c80 | 8 | .aezdata (LOAD al 0x10000) |
| commonpage4096 | -z common-page-size=4096 | 1415 | 1343d0 | 66c8 | 262a4 | 41747 | 3008 | 138 | 212974 | 210000 | 216c80/69d8 | 8 | .aezdata |
| spare0 | --spare-dynamic-tags=0 | — | 1343d0 | 66c8 | 262a4 | 41747 | 3008 | 110 | 212974 | 210000 | 216c80/69d8 | 8 | .aezdata |
| spare10 | --spare-dynamic-tags=10 | — | 1343d0 | 66c8 | 262a4 | 41747 | 3008 | 160 | 212974 | 210000 | 216c80/69d8 | 8 | .aezdata |
| nownocomb | -z now -z nocombreloc | — | 1343d0 | 66c8 | 262a4 | 41747 | 3008 | 140 | 20fcf4 | 214000 | 216980/69d8 | 8 | .aezdata (no RELCOUNT) |
| nowspare0 | -z now --spare-dynamic-tags=0 | — | 1343d0 | 66c8 | 262a4 | 41747 | 3008 | 120 | 20fcf4 | 214000 | 216980/69d8 | 8 | .aezdata (init_array 20fbc8) |
| nowspare5 | -z now --spare-dynamic-tags=5 | 1458 | 1343d0 | 66c8 | 262a4 | 41747 | 3008 | 148 | 20fcf4 | 214000 | 216980/69d8 | 8 | .aezdata |
| nowspare10 | -z now --spare-dynamic-tags=10 | — | 1343d0 | 66c8 | 262a4 | 41747 | 3008 | 170 | 20fcf4 | 214000 | 216980/69d8 | 8 | .aezdata (init_array 20fb78) |
| asneeded | --as-needed | — | 1343d0 | 66c8 | 262a4 | 41747 | 3008 | 118 | 212974 | 210000 | 216c80/69d8 | 8 | .aezdata (4 NEEDED gone) |
| nowasneeded | -z now --as-needed | 1374 | 1343d0 | 66c8 | 262a4 | 41747 | 3008 | 128 | 20fcf4 | 214000 | 216980/69d8 | 8 | .aezdata (4 NEEDED gone) |
| excllibgcc | --exclude-libs=libgcc.a | 1415 | 1343d0 | 66c8 | 262a4 | 41747 | 3008 | 138 | 212974 | 210000 | 216c80/69d8 | 8 | .aezdata |
| exclall | --exclude-libs=ALL | — | 134230 | 66c8 | 262a4 | 41747 | 3008 | 138 | 20d974 | 20b000 | 211a80 | 8 | .aezdata |
| nowexclall | -z now --exclude-libs=ALL | 1288 (linked 3427) | 134230 | 66c8 | 262a4 | 41747 | 3008 | 148 | 20af00 | 20f000 | 211980 | 8 | .aezdata |
| gc | --gc-sections | 1347 | 134320 | 6698 | 26298 | 41737 | 3008 | 138 | 21296c | 210000 | 216c80/69d8 | 8 | — |
| nowgc | -z now --gc-sections | 1392 | 134320 | 6698 | 26298 | 41737 | 3008 | 148 | 20fcf4 | 214000 | 216970/69d8 | 8 | — |
| icfall | --icf=all | 1344 | 1305a8 | 5788 | 262a4 | 41747 | 3008 | 138 | 20d974 | 20b000 | 211c80 | 8 | .aezdata |
| nowicf | -z now --icf=all | 1344 | 1305a8 | 5788 | 262a4 | 41747 | 3008 | 148 | 20acf4 | 20f000 | 211980 | 8 | .aezdata |
| icfsafe | --icf=safe | — | 131570 | 59c8 | 262a4 | 41747 | 3008 | 138 | 20e974 | 20c000 | 212c80 | 8 | .aezdata |
| sortname | --sort-section=name | 1341 | 134434 | 68f0 | 262a4 | 41720 | 3010 | 138 | 212974 | 210000 | 216c80/69c4 | 8 | .aezdata |
| nowsort | -z now --sort-section=name | 1341 | 134434 | 68f0 | 262a4 | 41720 | 3010 | 148 | 20fcf4 | 214000 | 216980/69c4 | 8 | .aezdata |
| notextreorder | --no-text-reorder | 1415 | 1343d0 | 66c8 | 262a4 | 41747 | 3008 | 138 | 212974 | 210000 | 216c80/69d8 | 8 | .aezdata |
| sortnone | --sort-section=none | 1415 | 1343d0 | 66c8 | 262a4 | 41747 | 3008 | 138 | 212974 | 210000 | 216c80/69d8 | 8 | .aezdata |
| mergeex | --merge-exidx-entries | 1415 | 1343d0 | 66c8 | 262a4 | 41747 | 3008 | 138 | 212974 | 210000 | 216c80/69d8 | 8 | .aezdata |
| nomergeex | --no-merge-exidx-entries | 1414 | 1343d0 | 8dc0 | 262a4 | 41747 | 3008 | 138 | 215974 | 213000 | 219c80 | 8 | .aezdata |
| nownomerge | -z now --no-merge-exidx-entries | 1414 | 1343d0 | 8dc0 | 262a4 | 41747 | 3008 | 148 | 212cf4 | 217000 | 219980 | 8 | .aezdata |
| O0 | -O0 | 1415 | 1343d0 | 66c8 | 262a4 | 41747 | 3008 | 138 | 212974 | 210000 | 216c80/69d8 | 8 | .aezdata |
| O1 | -O1 | 1415 | 1343d0 | 66c8 | 262a4 | 41747 | 3008 | 138 | 212974 | 210000 | 216c80/69d8 | 8 | .aezdata |
| O2 | -O2 | — | 1343d0 | 66c8 | 262a4 | 41547 | 3008 | 138 | 212974 | 210000 | 216c80/69d8 | 8 | .aezdata |
| O3 | -O3 | — | 1343d0 | 66c8 | 262a4 | 41547 | 3008 | 138 | 212974 | 210000 | 216c80/69d8 | 8 | .aezdata |
| nowO2 | -z now -O2 | 1382 | 1343d0 | 66c8 | 262a4 | 41547 | 3008 | 148 | 20fcf4 | 214000 | 216980/69d8 | 8 | .aezdata |
| roseg | --rosegment | 1386 | 1343d0 | 66c8 | 262a4 | 41747 | 3008 | 138 | 213974 | 211000 | 217c80 | 9 | .aezdata |
| nowroseg | -z now --rosegment | 1386 | 1343d0 | 66c8 | 262a4 | 41747 | 3008 | 148 | 210cf4 | 215000 | 217980 | 9 | .aezdata |
| noundef | --no-undefined | 1415 | 1343d0 | 66c8 | 262a4 | 41747 | 3008 | 138 | 212974 | 210000 | 216c80/69d8 | 8 | .aezdata |
| fatal | --fatal-warnings | 1415 | 1343d0 | 66c8 | 262a4 | 41747 | 3008 | 138 | 212974 | 210000 | 216c80/69d8 | 8 | .aezdata |
| warncommon | --warn-common | 1415 | 1343d0 | 66c8 | 262a4 | 41747 | 3008 | 138 | 212974 | 210000 | 216c80/69d8 | 8 | .aezdata |
| stacksize | -z stack-size=0 | 1415 | 1343d0 | 66c8 | 262a4 | 41747 | 3008 | 138 | 212974 | 210000 | 216c80/69d8 | 8 | .aezdata |
| nownoorder | -z now --section-ordering-file=/dev/null | — | 1343d0 | 66c8 | 262a4 | 41747 | 3008 | 148 | 20fcf4 | 214000 | 216980/69d8 | 8 | .aezdata |
| nowwholecrypto | -z now --whole-archive (libcrypto) | — | 1343d0 | 66c8 | 262a4 | 41747 | 3008 | 148 | 20fcf4 | 214000 | 216980/69d8 | 8 | .aezdata |
| nowallwhole | -z now, all archives whole | — | 1343d0 | 66c8 | 262a4 | 41747 | 3008 | 148 | 20fcf4 | 214000 | 216980/69d8 | 8 | .aezdata |
| nownorchive | -z now, zlib/libzip not whole | — | 12ce28 | 63c0 | 25f50 | 3d7f7 | 2f68 | 148 | 202dd8 | 207000 | 209940 | 8 | .aezdata |

Notes on the table:

* `-z now` is the only flag that moves `.dynamic`, `.got`, `.data` and the
  RELRO/PLT machinery onto the original addresses. It also gives the only
  positive `byte_exact` change (+43).
* `--gc-sections` and `--icf=*` shrink `.text`/`.ARM.exidx` and lower byte match;
  they move *away* from the original's `.text` and `.ARM.exidx` sizes.
* `--sort-section=name` happens to raise `.ARM.exidx` toward the original
  (0x68f0 vs 0x6f70) but raises `.text` and lowers byte match; not used.
* `--no-merge-exidx-entries` overshoots `.ARM.exidx` to 0x8dc0; the original is
  between the merged and unmerged outputs, so it used the default merge.
* `-O0`/`-O1` are neutral; `-O2`/`-O3` shrink `.rodata`/`.dynstr` and lower byte
  match. Gold's default is in the `-O0`/`-O1` class for this link.
* `--rosegment` adds a third `LOAD` (9 phdrs); the original has 2 `LOAD`s, so
  `--rosegment` is not used.
* `-z separate-code` / `-z noseparate-code`, `--emit-relocs`,
  `--pack-dyn-relocs=none` and `--no-rosegment` are not supported by gold 1.12
  (or fail); the original is a plain gold 1.12 `REL` link.
* `--whole-archive` around libcrypto (or all archives) is neutral for the
  stripped sections (all members are already pulled); it changes only the
  pre-strip build-id. `--whole-archive` around zlib/libzip is required: removing
  it shrinks `.dynsym` 0x15ca0→0x15670 and `.text` 0x1343d0→0x12ce28.

## 10. Could the original have used `--gc-sections`?

The original's `.text` contains at least one local function with **no incoming
reference at all**:

* `0x873a0` is a real 0x68-byte function (next exidx start `0x87408`), not in
  `.dynsym`, with no direct `b`/`bl`/`blx` target anywhere in `.text`, and no
  raw little-endian `0x873a0` pointer anywhere in the file (no data/vtable/GOT
  reference). It is a `ConfigReader` local helper immediately after the exported
  `ConfigReader::RegisterTokenReadFunction` at `0x8734c` (size 0x54).
* A call-graph scan of the original's `.text` (exidx starts as function starts,
  branch targets + literal-pool pointers + `.rel.dyn` `ABS32`/`RELATIVE`
  addends + `.init_array`/`.fini_array` as roots) finds 32 real-looking
  non-exported functions with no incoming branch and no pointer reference; 139
  more are address-only (pointer-referenced, no direct branch). `0x873a0` is the
  cleanest example.

If the original was compiled with `-ffunction-sections` (the 194 duplicate
alias exidx entries at zero-size alias sections are consistent with that), a
`--gc-sections` link would have removed `0x873a0`. It is present, so the original
was **not** linked with `--gc-sections`.

Even without that argument, `--gc-sections` is not supported by the measurements:
it removes 9 dead functions and `.aezdata`, shrinking `.text` by 0xb0 and
`.ARM.exidx` by 0x30, and lowers `byte_exact` from 1458 to 1392. The original's
`.text` is 0xc08 smaller than ours, so gc cannot explain the gap. The only
observables gc improves are `.ARM.extab` (removes the one dead
`modStationTouchEnd` extab entry, 0xc, making it exactly 0x26298) and the `.bss`
address (removing `.aezdata` makes it exactly 0x216970); both are side effects of
removing source artifacts, not link-flag evidence.

## 11. Recommended final gold command line

Keep the current line and add **`-z now`**. The minimal diff is:

```diff
             -shared -soname libgof2hdaa.so --build-id
+            -z now
             --unresolved-symbols=ignore-all
             --exclude-libs=libgcc.a
```

Flag by flag, with the observable that justifies it:

| flag | why | observable |
|---|---|---|
| `-shared -soname libgof2hdaa.so` | DYN + `DT_SONAME` | `readelf -h/-d` |
| `--build-id` (SHA-1) | original has a 20-byte build-id | note size 0x24; md5 0x20, none absent |
| **`-z now`** | original has `BIND_NOW`/`NOW`, `.got` before `.data`, RELRO over `.got` | `.dynamic 0x148` at 0x20fba4; `.got 0x20fcf4`; `.data 0x214000`; `.got`/`.plt` byte-identical; +43 byte_exact |
| `--unresolved-symbols=ignore-all` | original leaves app-provided imports undefined | sodiff imports 207, `missing 0` |
| `--exclude-libs=libgcc.a` | original has no defined `__aeabi_*`; all are UND | dynsym size 0x15ca0; `ALL` shrinks it |
| `--section-ordering-file=...` | reconstruction aid to reproduce original `.text` order | `.text` function order; original itself was linked without one |
| `--whole-archive zlib libzip --no-whole-archive` | original exports the full zlib/libzip symbol sets | without it `.dynsym` 0x15670, `.text` 0x12ce28 |
| `-l log -l GLESv2 -l GLESv1_CM -l EGL -l android` | original `DT_NEEDED` order | `--as-needed` drops 4 |
| FMOD libs, `libc++abi.a`, `-l m`, `libandroid_support.a`, `libgcc.a`, `-l dl -l c` | original `DT_NEEDED`/static ABI | dynamic section tag-for-tag |
| `crtbegin_so.o` / `crtend_so.o` | `.init_array` 3 entries / `.fini_array` | `.init_array 0xc`, `.fini_array 0x8` |

Defaults that must **not** be changed (they are already correct):

* `-z relro` (default): `PT_GNU_RELRO` present; `-z norelro` removes it and
  reorders the RW sections.
* `-z noexecstack` (default): `PT_GNU_STACK` is `RW`; `-z execstack` makes it
  `RWE`.
* `--hash-style=sysv` (default): original has `.hash`, no `.gnu.hash`.
* `-z combreloc` (default): original has `DT_RELCOUNT = 1162`;
  `-z nocombreloc` drops it (`.dynamic 0x140`).
* `-z max-page-size=4096` (default): `LOAD` align is 0x1000; 65536 gives 0x10000.
* `-z common-page-size=4096` (default): neutral.
* `--no-as-needed` (default): original has all 10 `NEEDED` entries.
* `--spare-dynamic-tags=5` (default, with `-z now`): `.dynamic 0x148`; 0 gives 0x120.
* no `--gc-sections`, no `--icf`, no `--rosegment`, no `--sort-section=name`,
  no `-O2`, no `--emit-relocs`, no `--exclude-libs=ALL`.

Making the defaults explicit is optional; this line was verified to produce a
**byte-identical `.so`** to `-z now` alone:

```
-z now -z relro --hash-style=sysv --build-id=sha1 \
-z max-page-size=4096 -z common-page-size=4096 \
--no-as-needed --exclude-libs=libgcc.a
```

(`cmp` against the `-z now` link: identical, including build-id.)

## 12. Remaining deltas and ownership

With `-z now` the link-level shape is correct. The remaining deltas are
compile/TU-caused and should be handled by source/TU work, not by more link
flags:

| delta | cause | evidence |
|---|---|---|
| `.text +0xc08` | source shape / function sizes | no flag reaches 0x1337c8; gc undershoots, icf overshoots |
| `.ARM.exidx -0x8a8` | function set/order/aliases | original 3566 entries (272 cantunwind, 194 duplicate alias addresses) vs ours 3289 (6 cantunwind, 1 duplicate); original used merge (3566 < 4523 exported funcs); `--no-merge` overshoots to 4536 |
| `.ARM.extab +0xc` | one dead local function's extab entry | gc removes exactly `modStationTouchEnd`'s 0xc entry |
| `.rodata -0x420` | string/RTTI literal layout | no flag reaches 0x41b67; `-O2`/gc shrink it further |
| `.data.rel.ro +0x20` | inter-section alignment padding | same 277 exported data symbols and sizes; gc/icf leave 0x3008; `--sort-section=name` gives 0x3010 |
| `.bss +0x2c` | local/static BSS | exported data exact 843/843; no flag changes the size |
| `.aezdata` 8 B | `#pragma clang section` in `AEFile.cpp` | not in original; gc removes it; `.bss` then lands at 0x216970 |
| dynsym order | TU membership / within-object symbol order | same 5577 names, 789 same index; `--sort-section=name` is worse |
| PLT slot order | input section/TU order | first 81 slots match, then diverge; no tested flag changes it |
| build-id value | output content | SHA-1 algorithm matches; value will match only with whole-file identity |

The actionable link pass is therefore exactly one flag: **`-z now`**. It is a
+43 `byte_exact` improvement with no regression, and it is the flag that makes
the original's PLT/GOT/RELRO layout reproducible.
