# Link-flag re-validation (w3694)

Status: research note, 2026-11-02. Worktree `wt/w3694`, branch `wave87-w3694`, based on
`88aaf59f`. All numbers below were measured against
`../_work/bins/android_2.0.16_libgof2hdaa.so` with the repo's NDK r18b clang 7.0.2 / gold 1.12.
No source or link-flag change was landed: no tested flag is a strict improvement over the current
link line. No `--update-baseline` was used.

## 1. Baseline (current tree)

Commands:

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint -j8
cmake --build cmake-build-match --target drift -j8
```

Measured:

```
verify: byte 4496  linked 4496  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
CLEAN ✓ (no new soft violations; hard invariants 0)
=== DRIFT: 0 field(s) across 0 class(es) ===
```

The baseline `.so` is 2,187,328 bytes (same size as the original), `.text` is
`0x1337c8` at `0x69440`, and the section table has 27 sections / 8 program headers.
`sha256` is `b85e7c4e122f0baa92fa2b85ed9cbdb44d0c274602b6e348617928b0e01850fc`
(original `d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483`):

```sh
sha256sum cmake-build-match/libgof2hdaa.so ../_work/bins/android_2.0.16_libgof2hdaa.so
cmp -l cmake-build-match/libgof2hdaa.so ../_work/bins/android_2.0.16_libgof2hdaa.so | wc -l
readelf -SW cmake-build-match/libgof2hdaa.so
readelf -lW cmake-build-match/libgof2hdaa.so
readelf -dW cmake-build-match/libgof2hdaa.so
```

`cmp -l | wc -l` reports **238,945** differing bytes.

### Baseline section state

Every section is size- and base-exact except `.ARM.extab`, which is 8 bytes short. The
section-payload differences are:

| section | original addr/size | ours addr/size | differing bytes |
|---|---|---|---:|
| `.note.gnu.build-id` | `0x1cc` / `0x24` | `0x1cc` / `0x24` | 20 |
| `.rel.plt` | `0x5751c` / `0x72d0` | `0x5751c` / `0x72d0` | 12 |
| `.text` | `0x69440` / `0x1337c8` | `0x69440` / `0x1337c8` | 114,696 |
| `.ARM.exidx` | `0x19cc08` / `0x6f70` | `0x19cc08` / `0x6f70` | 2,707 |
| `.ARM.extab` | `0x1a3b78` / `0x26298` | `0x1a3b78` / `0x26290` | 121,226 |
| `.shstrtab` | `0` / `0x117` | `0` / `0x117` | 259 |

All other section payloads are byte-identical, including `.dynsym`, `.dynstr`, `.hash`,
`.gnu.version*`, `.rel.dyn`, `.plt`, `.rodata`, `.fini_array`, `.data.rel.ro`,
`.init_array`, `.dynamic`, `.got`, `.data`, `.bss`, `.comment`,
`.note.gnu.gold-version` and `.ARM.attributes`. The `.shstrtab` difference is string-table
order (the original's table is consistent with section-creation/insertion order; ours is in
section-header order), not a gate metric. The section-payload table accounts for 238,920 of the 238,945 file bytes.

The program headers and dynamic tags are exactly the original's:

```
LOAD #1  R E  0x1000  filesz/memsz 0x20b977
LOAD #2  RW   0x1000  filesz 0x09dcc  memsz 0x1077c
DYNAMIC  RW   0x4     0x148 at 0x20fba4
GNU_STACK RW  0x10
GNU_RELRO RW  0x10    0x7460 at 0x20cba0
```

`readelf -dW` is tag-for-tag identical: `DT_FLAGS=BIND_NOW`, `DT_FLAGS_1=NOW`,
`DT_RELCOUNT=1162`, 10 `DT_NEEDED` entries in the original order, `.got` at `0x20fcf4`,
`.data` at `0x214000`, `.bss` at `0x216970`.

## 2. Current link line and reproduction

The current gold command (from `ninja -C cmake-build-match -t commands libgof2hdaa.so`)
is:

```sh
ld.gold -shared -soname libgof2hdaa.so --build-id -z now \
  --exclude-libs=libgcc.a \
  --section-ordering-file=cmake-build-match/section_order.txt \
  -o OUT cmake-build-match/crtbegin_so_renamed.o \
  -L .cache/ndk/android-ndk-r18b/platforms/android-16/arch-arm/usr/lib \
  @cmake-build-match/exidx_markers/link_order.rsp \
  --whole-archive .../libz_gof2.a .../libzip_gof2.a --no-whole-archive \
  -l log -l GLESv2 -l GLESv1_CM -l EGL -l android \
  ../_work/bins/apk_libs/libfmodex.so ../_work/bins/apk_libs/libfmodevent.so \
  .../libcrypto_gof2.a -l m .../libc++abi.a .../libandroid_support.a \
  .../libgcc.a -l dl -l c \
  .cache/ndk/android-ndk-r18b/platforms/android-16/arch-arm/usr/lib/crtend_so.o
strip --strip-all OUT
```

The exact relink helper used for every A/B row is:

```sh
#!/bin/bash
set -euo pipefail
ROOT=/home/debian/GalaxyOnFire2/wt/w3694
B=$ROOT/cmake-build-match
NDK=$ROOT/.cache/ndk/android-ndk-r18b
GOLD=$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/arm-linux-androideabi/bin/ld.gold
STRIP=$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-strip
OUT=$1; shift
if [ "${ZLIB_WHOLE:-1}" = 1 ]; then ZW=--whole-archive; ZNW=--no-whole-archive; else ZW=; ZNW=; fi
if [ "${CRYPTO_WHOLE:-0}" = 1 ]; then CW=--whole-archive; CNW=--no-whole-archive; else CW=; CNW=; fi
"$GOLD" -shared -soname libgof2hdaa.so --build-id ${BASE-} "$@" \
  ${EXCLUDE-"--exclude-libs=libgcc.a"} \
  ${ORDER-"--section-ordering-file=$B/section_order.txt"} \
  -o "$OUT" \
  "$B/crtbegin_so_renamed.o" -L "$NDK/platforms/android-16/arch-arm/usr/lib" \
  @"$B/exidx_markers/link_order.rsp" \
  $ZW "$B/exidx_markers/archives/libz_gof2.a" "$B/exidx_markers/archives/libzip_gof2.a" $ZNW \
  -l log -l GLESv2 -l GLESv1_CM -l EGL -l android \
  "$ROOT/../_work/bins/apk_libs/libfmodex.so" "$ROOT/../_work/bins/apk_libs/libfmodevent.so" \
  $CW "$B/exidx_markers/archives/libcrypto_gof2.a" $CNW \
  -l m \
  "$B/exidx_markers/archives/libc++abi.a" \
  "$B/exidx_markers/archives/libandroid_support.a" \
  "$B/exidx_markers/archives/libgcc.a" \
  -l dl -l c \
  "$NDK/platforms/android-16/arch-arm/usr/lib/crtend_so.o"
"$STRIP" --strip-all "$OUT"
```

The baseline was reproduced exactly by `BASE='-z now' relink.sh OUT` (same sha256 as
`cmake-build-match/libgof2hdaa.so`). Each variant was then relinked with one flag added or
removed and verified with:

```sh
python3 tools/verify/verify.py \
  --build-dir /tmp/.../verify \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/.../OUT.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
```

Section/address data came from `readelf -SW/-lW/-dW/-nW` and a small ELF parser that
compares every section payload against the original. The full machine-readable table is
`_work/reconstructions/link_flags_revalidation_w3694.csv` (78 rows); the same rows are in
the appendix below.

## 3. Results

### 3.1 The current line is output-optimal among the tested variants

No tested variant raises `byte_exact` above 4496. The only output-identical variants are
the ones that make already-default behavior explicit (or are output-neutral), so none is a
strict gate improvement:

| variant | extra flag(s) | byte | linked | sha256 prefix | verdict |
|---|---|---:|---:|---|---|
| baseline | — | 4496 | 4496 | `b85e7c4e` | reference |
| `combined_explicit_defaults` | `-z relro -z noexecstack --hash-style=sysv --build-id=sha1 -z max-page-size=4096 -z common-page-size=4096 --no-as-needed -z combreloc --spare-dynamic-tags=5 --no-undefined --warn-shared-textrel --fatal-warnings --fix-cortex-a8` | 4496 | 4496 | `b85e7c4e` | **byte-identical to baseline** |
| `relro` | `-z relro` | 4496 | 4496 | `b85e7c4e` | identical |
| `noexecstack` | `-z noexecstack` | 4496 | 4496 | `b85e7c4e` | identical |
| `hash_sysv` | `--hash-style=sysv` | 4496 | 4496 | `b85e7c4e` | identical |
| `buildid_sha1` | `--build-id=sha1` | 4496 | 4496 | `b85e7c4e` | identical |
| `spare5` | `--spare-dynamic-tags=5` | 4496 | 4496 | `b85e7c4e` | identical |
| `no_as_needed` | `--no-as-needed` | 4496 | 4496 | `b85e7c4e` | identical |
| `combreloc` | `-z combreloc` | 4496 | 4496 | `b85e7c4e` | identical |
| `maxpage4096` | `-z max-page-size=4096` | 4496 | 4496 | `b85e7c4e` | identical |
| `commonpage4096` | `-z common-page-size=4096` | 4496 | 4496 | `b85e7c4e` | identical |
| `no_undefined` / `z_defs` | `--no-undefined` / `-z defs` | 4496 | 4496 | `b85e7c4e` | identical; link succeeds |
| `warn_shared_textrel` | `--warn-shared-textrel` | 4496 | 4496 | `b85e7c4e` | identical |
| `fatal_warnings` | `--fatal-warnings` | 4496 | 4496 | `b85e7c4e` | identical |
| `fix_cortex_a8` / `no_fix_cortex_a8` | `--fix-cortex-a8` / `--no-fix-cortex-a8` | 4496 | 4496 | `b85e7c4e` | identical |
| `unresolved_ignore_all` | `--unresolved-symbols=ignore-all` | 4496 | 4496 | `b85e7c4e` | identical |
| `allow_multiple_definition` | `--allow-multiple-definition` | 4496 | 4496 | `b85e7c4e` | identical (but AGENTS says do not add) |
| `merge_exidx` | `--merge-exidx-entries` | 4496 | 4496 | `b85e7c4e` | identical |
| `sort_none` | `--sort-section=none` | 4496 | 4496 | `b85e7c4e` | identical |
| `no_text_reorder` | `--no-text-reorder` | 4496 | 4496 | `b85e7c4e` | identical |
| `crypto_whole` | whole-archive around libcrypto | 4496 | 4496 | `b85e7c4e` | identical |
| `z_text` / `z_notext` | `-z text` / `-z notext` | 4496 | 4496 | `b85e7c4e` | identical |
| `no_keep_memory` / `noinhibit_exec` | `--no-keep-memory` / `--noinhibit-exec` | 4496 | 4496 | `b85e7c4e` | identical |

### 3.2 Flags that are required

| variant | change | byte | linked | key structural effect |
|---|---|---:|---:|---|
| `no_now` | remove `-z now` | 2932 | 4496 | `.dynamic` `0x138`, no `BIND_NOW`/`NOW`; `.got`/`.data` move |
| `lazy` / `now_lazy` | `-z lazy` (with or after `-z now`) | 2932 | 4496 | same as `no_now`; last `-z` wins |
| `norelro` | `-z norelro` | 2932 | 4496 | 7 phdrs, no `PT_GNU_RELRO`; RW layout moves |
| `no_order_omitted` | omit `--section-ordering-file` | 1527 | 4496 | `.text` order wrong; `.ARM.exidx` `0x6f30`; `.rodata` base `-0x40` |
| `no_exclude` | remove `--exclude-libs=libgcc.a` | 1720 | 4489 | extra defined `__aeabi_*`; dynsym/byte loss |
| `no_zlib_whole` | remove zlib/libzip `--whole-archive` | 1338 | 4412 | missing 84 functions; `.ARM.extab` `0x25f3c` |
| `buildid_md5` | `--build-id=md5` | 1969 | 4496 | 0x20-byte note, address shift |
| `buildid_none` | `--build-id=none` | 1750 | 4496 | no build-id note, address shift |
| `hash_gnu` / `hash_both` | `--hash-style=gnu` / `both` | 1750 | 4496 | `.gnu.hash` replaces `.hash`; address shift |
| `spare0` / `spare10` | `--spare-dynamic-tags=0` / `10` | 4485 | 4496 | `.dynamic` `0x120` / `0x170` |
| `as_needed` | `--as-needed` | 1750 | 4496 | 4 `DT_NEEDED` entries dropped |
| `nocombreloc` | `-z nocombreloc` | 4485 | 4496 | `DT_RELCOUNT` dropped |
| `maxpage65536` | `-z max-page-size=65536` | 2932 | 4496 | LOAD align `0x10000`, address shift |
| `no_merge_exidx` | `--no-merge-exidx-entries` | 2878 | 4496 | `.ARM.exidx` `0x95c0` |
| `sort_name` | `--sort-section=name` | 1342 | 4494 | `.text` order changes; `.ARM.exidx` `0x68b8` |
| `gc_sections` | `--gc-sections` | 2176 | 4496 | dead code removed; `.text` `0x133794` |
| `icf_safe` / `icf_all` | `--icf=safe` / `--icf=all` | 1347 | 4496 / 4495 | `.text` shrinks to `0x130964` / `0x12f99c` |
| `rosegment` | `--rosegment` | 1720 | 4496 | 9 phdrs |
| `exclude_all` | `--exclude-libs=ALL` | 1324 | 3429 | missing 242 functions |

`-z execstack` is not a strict improvement: it keeps `byte 4496` but changes
`GNU_STACK` from the original's `RW` to `RWE`. `-z initfirst`, `-z origin`,
`-z nodelete`, `-z nodlopen`, `-z nodump` and `-z interpose` also keep `byte 4496` but
change `DT_FLAGS` / `DT_FLAGS_1` (e.g. `z_origin` sets `DT_FLAGS_1=ORIGIN`); they are
structurally worse.

### 3.3 Gold 1.12 flags that do not work

| variant | error |
|---|---|
| `copy_dt_needed` | `--copy-dt-needed-entries is not supported but is required for libstdc++.so in libfmodex.so` |
| `add_needed` | `--add-needed is not supported but is required for libstdc++.so in libfmodex.so` |
| `sort_alignment` | `--sort-section=alignment: must take one of: none, name` |
| `no_rosegment` | usage error |
| `separate_code` / `no_separate_code` | usage error |
| `pack_dyn_relocs_none` | usage error |
| `orphan_warn` / `orphan_place` / `orphan_error` | usage error |
| `reduce_memory_overheads` | usage error |
| `emit_relocs` | internal error in `relocate_relocs` |
| `no_order` (with `/dev/null`) | `file is empty`; omit the flag instead |

`--no-undefined` succeeds on the current objects (the app-provided imports are resolved
through the link inputs), so it is safe to add for robustness, but it is output-identical
and not a gate improvement.

## 4. Conclusion

**Proven (within the tested set):**

1. The current link line is already the best of 78 tested variants by `byte_exact`
   (4496) and `linked_exact` (4496). No flag raises either count.
2. `-z now` is required: removing it drops `byte_exact` 4496 -> 2932 and changes
   `.dynamic`/`.got`/`.data` away from the original.
3. `--section-ordering-file` is required: omitting it drops `byte_exact` to 1527 and
   changes `.text` order, `.ARM.exidx` size and `.rodata` base.
4. `--exclude-libs=libgcc.a` and zlib/libzip `--whole-archive` are required: removing
   them drops `byte_exact` to 1720 / 1338 and `linked_exact` to 4489 / 4412.
5. The original's dynamic/link shape is exactly reproduced by the current defaults:
   SHA-1 build-id, SysV hash, 5 spare dynamic tags, `--no-as-needed`, `-z combreloc`,
   4 KiB max/common page size, `-z relro`, `-z noexecstack`, `-z now`.
6. The remaining differences are not explained by any tested link flag: `.text`
   114,696 bytes, `.ARM.exidx` 2,707, `.ARM.extab` 121,226 with size -8, `.rel.plt`
   12, build-id 20, `.shstrtab` 259. These are source/TU/body residuals.

**Still hypothesis / not proven:**

* That no untested gold 1.12 flag can improve the gate. The tested set is broad but not
  exhaustive; the CSV lists exactly what was tried.
* That the original build used each explicit default (`--no-undefined`,
  `--fix-cortex-a8`, etc.). The output is consistent with ndk-build r18b defaults, but
  output-identical flags cannot be distinguished from omission.
* The cause of the `.shstrtab` order and the `.rel.plt` 12-byte residual is outside this
  link-flag pass; both are consistent with gold input/order behavior, not a missing flag.

## 5. Documentation drift observed (not changed here)

* `docs/VALIDATION.md` describes the compile target as `android21`; the current
  `cmake/toolchain.cmake` uses `_match_api 16`.
* `docs/VALIDATION.md`'s link-line paragraph omits `-z now`, although `CMakeLists.txt`
  passes it and `docs/IDENTICALITY.md` records it.
* `AGENTS.md` says the existing link has `--unresolved-symbols=ignore-all`; the current
  `CMakeLists.txt` does not pass it. Adding it is output-identical (row
  `unresolved_ignore_all`).

These are documentation/description drifts, not gate regressions; they were left for the
coordinator because this pass landed only the research note.

## Appendix: all 78 A/B rows

The same data is in `link_flags_revalidation_w3694.csv`. `link`/`verify` are process exit
codes; section sizes are ours. `sha` is omitted here for width; the CSV has the full hash.

| variant | base | extra args / env | link | verify | byte | linked | .text | .ARM.exidx | .ARM.extab | .dynamic | phdrs | note |
|---|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| baseline | `-z now` | `` | 0 | 0 | 4496 | 4496 | 0x1337c8 | 0x6f70 | 0x26290 | 0x148 | 8 |  |
| no_now | `` | `` | 0 | 0 | 2932 | 4496 | 0x1337c8 | 0x6f70 | 0x26290 | 0x138 | 8 |  |
| relro | `-z now` | `-z relro` | 0 | 0 | 4496 | 4496 | 0x1337c8 | 0x6f70 | 0x26290 | 0x148 | 8 |  |
| norelro | `-z now` | `-z norelro` | 0 | 0 | 2932 | 4496 | 0x1337c8 | 0x6f70 | 0x26290 | 0x148 | 7 |  |
| noexecstack | `-z now` | `-z noexecstack` | 0 | 0 | 4496 | 4496 | 0x1337c8 | 0x6f70 | 0x26290 | 0x148 | 8 |  |
| execstack | `-z now` | `-z execstack` | 0 | 0 | 4496 | 4496 | 0x1337c8 | 0x6f70 | 0x26290 | 0x148 | 8 |  |
| lazy | `` | `-z lazy` | 0 | 0 | 2932 | 4496 | 0x1337c8 | 0x6f70 | 0x26290 | 0x138 | 8 |  |
| now_lazy | `-z now` | `-z lazy` | 0 | 0 | 2932 | 4496 | 0x1337c8 | 0x6f70 | 0x26290 | 0x138 | 8 |  |
| hash_sysv | `-z now` | `--hash-style=sysv` | 0 | 0 | 4496 | 4496 | 0x1337c8 | 0x6f70 | 0x26290 | 0x148 | 8 |  |
| hash_gnu | `-z now` | `--hash-style=gnu` | 0 | 0 | 1750 | 4496 | 0x1337c8 | 0x6f70 | 0x26290 | 0x148 | 8 |  |
| hash_both | `-z now` | `--hash-style=both` | 0 | 0 | 1750 | 4496 | 0x1337c8 | 0x6f70 | 0x26290 | 0x150 | 8 |  |
| buildid_sha1 | `-z now` | `--build-id=sha1` | 0 | 0 | 4496 | 4496 | 0x1337c8 | 0x6f70 | 0x26290 | 0x148 | 8 |  |
| buildid_md5 | `-z now` | `--build-id=md5` | 0 | 0 | 1969 | 4496 | 0x1337c8 | 0x6f70 | 0x26290 | 0x148 | 8 |  |
| buildid_none | `-z now` | `--build-id=none` | 0 | 0 | 1750 | 4496 | 0x1337c8 | 0x6f70 | 0x26290 | 0x148 | 8 |  |
| spare0 | `-z now` | `--spare-dynamic-tags=0` | 0 | 0 | 4485 | 4496 | 0x1337c8 | 0x6f70 | 0x26290 | 0x120 | 8 |  |
| spare5 | `-z now` | `--spare-dynamic-tags=5` | 0 | 0 | 4496 | 4496 | 0x1337c8 | 0x6f70 | 0x26290 | 0x148 | 8 |  |
| spare10 | `-z now` | `--spare-dynamic-tags=10` | 0 | 0 | 4485 | 4496 | 0x1337c8 | 0x6f70 | 0x26290 | 0x170 | 8 |  |
| as_needed | `-z now` | `--as-needed` | 0 | 0 | 1750 | 4496 | 0x1337c8 | 0x6f70 | 0x26290 | 0x128 | 8 |  |
| no_as_needed | `-z now` | `--no-as-needed` | 0 | 0 | 4496 | 4496 | 0x1337c8 | 0x6f70 | 0x26290 | 0x148 | 8 |  |
| combreloc | `-z now` | `-z combreloc` | 0 | 0 | 4496 | 4496 | 0x1337c8 | 0x6f70 | 0x26290 | 0x148 | 8 |  |
| nocombreloc | `-z now` | `-z nocombreloc` | 0 | 0 | 4485 | 4496 | 0x1337c8 | 0x6f70 | 0x26290 | 0x140 | 8 |  |
| maxpage4096 | `-z now` | `-z max-page-size=4096` | 0 | 0 | 4496 | 4496 | 0x1337c8 | 0x6f70 | 0x26290 | 0x148 | 8 |  |
| maxpage65536 | `-z now` | `-z max-page-size=65536` | 0 | 0 | 2932 | 4496 | 0x1337c8 | 0x6f70 | 0x26290 | 0x148 | 8 |  |
| commonpage4096 | `-z now` | `-z common-page-size=4096` | 0 | 0 | 4496 | 4496 | 0x1337c8 | 0x6f70 | 0x26290 | 0x148 | 8 |  |
| no_undefined | `-z now` | `--no-undefined` | 0 | 0 | 4496 | 4496 | 0x1337c8 | 0x6f70 | 0x26290 | 0x148 | 8 |  |
| z_defs | `-z now` | `-z defs` | 0 | 0 | 4496 | 4496 | 0x1337c8 | 0x6f70 | 0x26290 | 0x148 | 8 |  |
| warn_shared_textrel | `-z now` | `--warn-shared-textrel` | 0 | 0 | 4496 | 4496 | 0x1337c8 | 0x6f70 | 0x26290 | 0x148 | 8 |  |
| fatal_warnings | `-z now` | `--fatal-warnings` | 0 | 0 | 4496 | 4496 | 0x1337c8 | 0x6f70 | 0x26290 | 0x148 | 8 |  |
| fix_cortex_a8 | `-z now` | `--fix-cortex-a8` | 0 | 0 | 4496 | 4496 | 0x1337c8 | 0x6f70 | 0x26290 | 0x148 | 8 |  |
| no_fix_cortex_a8 | `-z now` | `--no-fix-cortex-a8` | 0 | 0 | 4496 | 4496 | 0x1337c8 | 0x6f70 | 0x26290 | 0x148 | 8 |  |
| unresolved_ignore_all | `-z now` | `--unresolved-symbols=ignore-all` | 0 | 0 | 4496 | 4496 | 0x1337c8 | 0x6f70 | 0x26290 | 0x148 | 8 |  |
| unresolved_ignore_shlibs | `-z now` | `--unresolved-symbols=ignore-in-shared-libs` | 0 | 0 | 4496 | 4496 | 0x1337c8 | 0x6f70 | 0x26290 | 0x148 | 8 |  |
| allow_shlib_undefined | `-z now` | `--allow-shlib-undefined` | 0 | 0 | 4496 | 4496 | 0x1337c8 | 0x6f70 | 0x26290 | 0x148 | 8 |  |
| no_allow_shlib_undefined | `-z now` | `--no-allow-shlib-undefined` | 0 | 0 | 4496 | 4496 | 0x1337c8 | 0x6f70 | 0x26290 | 0x148 | 8 |  |
| allow_multiple_definition | `-z now` | `--allow-multiple-definition` | 0 | 0 | 4496 | 4496 | 0x1337c8 | 0x6f70 | 0x26290 | 0x148 | 8 |  |
| no_allow_multiple_definition | `-z now` | `--no-allow-multiple-definition` | 0 | 0 | 4496 | 4496 | 0x1337c8 | 0x6f70 | 0x26290 | 0x148 | 8 |  |
| z_muldefs | `-z now` | `-z muldefs` | 0 | 0 | 4496 | 4496 | 0x1337c8 | 0x6f70 | 0x26290 | 0x148 | 8 |  |
| no_copy_dt_needed | `-z now` | `--no-copy-dt-needed-entries` | 0 | 0 | 4496 | 4496 | 0x1337c8 | 0x6f70 | 0x26290 | 0x148 | 8 |  |
| copy_dt_needed | `-z now` | `--copy-dt-needed-entries` | 1 |  |  |  |  |  |  |  |  | link fail: --copy-dt-needed-entries is not supported but is required for libstdc++.so in /home/debian/GalaxyOnFire2/wt/w3694/../_work/bins/apk_libs/libfmodex.so |
| no_add_needed | `-z now` | `--no-add-needed` | 0 | 0 | 4496 | 4496 | 0x1337c8 | 0x6f70 | 0x26290 | 0x148 | 8 |  |
| add_needed | `-z now` | `--add-needed` | 1 |  |  |  |  |  |  |  |  | link fail: --add-needed is not supported but is required for libstdc++.so in /home/debian/GalaxyOnFire2/wt/w3694/../_work/bins/apk_libs/libfmodex.so |
| no_gnu_unique | `-z now` | `--no-gnu-unique` | 0 | 0 | 4496 | 4496 | 0x1337c8 | 0x6f70 | 0x26290 | 0x148 | 8 |  |
| merge_exidx | `-z now` | `--merge-exidx-entries` | 0 | 0 | 4496 | 4496 | 0x1337c8 | 0x6f70 | 0x26290 | 0x148 | 8 |  |
| no_merge_exidx | `-z now` | `--no-merge-exidx-entries` | 0 | 0 | 2878 | 4496 | 0x1337c8 | 0x95c0 | 0x26290 | 0x148 | 8 |  |
| sort_none | `-z now` | `--sort-section=none` | 0 | 0 | 4496 | 4496 | 0x1337c8 | 0x6f70 | 0x26290 | 0x148 | 8 |  |
| sort_name | `-z now` | `--sort-section=name` | 0 | 0 | 1342 | 4494 | 0x13381c | 0x68b8 | 0x26290 | 0x148 | 8 |  |
| sort_alignment | `-z now` | `--sort-section=alignment` | 1 |  |  |  |  |  |  |  |  | link fail: none, name |
| gc_sections | `-z now` | `--gc-sections` | 0 | 0 | 2176 | 4496 | 0x133794 | 0x6690 | 0x26290 | 0x148 | 8 |  |
| icf_all | `-z now` | `--icf=all` | 0 | 0 | 1347 | 4495 | 0x12f99c | 0x5770 | 0x26290 | 0x148 | 8 |  |
| icf_safe | `-z now` | `--icf=safe` | 0 | 0 | 1347 | 4496 | 0x130964 | 0x59b0 | 0x26290 | 0x148 | 8 |  |
| rosegment | `-z now` | `--rosegment` | 0 | 0 | 1720 | 4496 | 0x1337c8 | 0x6f70 | 0x26290 | 0x148 | 9 |  |
| no_rosegment | `-z now` | `--no-rosegment` | 1 |  |  |  |  |  |  |  |  | link fail: use the --help option for usage information |
| no_text_reorder | `-z now` | `--no-text-reorder` | 0 | 0 | 4496 | 4496 | 0x1337c8 | 0x6f70 | 0x26290 | 0x148 | 8 |  |
| no_order | `-z now` | `ORDER=/dev/null` | 1 |  |  |  |  |  |  |  |  | link fail: file is empty |
| no_order_omitted | `-z now` | `ORDER=` | 0 | 0 | 1527 | 4496 | 0x1337c8 | 0x6f30 | 0x26290 | 0x148 | 8 |  |
| no_exclude | `-z now` | `EXCLUDE=` | 0 | 0 | 1720 | 4489 | 0x1337d8 | 0x6f70 | 0x26290 | 0x148 | 8 |  |
| exclude_all | `-z now` | `EXCLUDE=--exclude-libs=ALL` | 0 | 1 | 1324 | 3429 | 0x133628 | 0x6f70 | 0x26290 | 0x148 | 8 | verify rc=1 (missing 242) |
| no_zlib_whole | `-z now` | `ZLIB_WHOLE=0` | 0 | 1 | 1338 | 4412 | 0x12c208 | 0x6af0 | 0x25f3c | 0x148 | 8 | verify rc=1 (missing 84) |
| crypto_whole | `-z now` | `CRYPTO_WHOLE=1` | 0 | 0 | 4496 | 4496 | 0x1337c8 | 0x6f70 | 0x26290 | 0x148 | 8 |  |
| separate_code | `-z now` | `-z separate-code` | 1 |  |  |  |  |  |  |  |  | link fail: use the --help option for usage information |
| no_separate_code | `-z now` | `-z noseparate-code` | 1 |  |  |  |  |  |  |  |  | link fail: use the --help option for usage information |
| z_text | `-z now` | `-z text` | 0 | 0 | 4496 | 4496 | 0x1337c8 | 0x6f70 | 0x26290 | 0x148 | 8 |  |
| z_notext | `-z now` | `-z notext` | 0 | 0 | 4496 | 4496 | 0x1337c8 | 0x6f70 | 0x26290 | 0x148 | 8 |  |
| z_initfirst | `-z now` | `-z initfirst` | 0 | 0 | 4496 | 4496 | 0x1337c8 | 0x6f70 | 0x26290 | 0x148 | 8 |  |
| z_origin | `-z now` | `-z origin` | 0 | 0 | 4496 | 4496 | 0x1337c8 | 0x6f70 | 0x26290 | 0x148 | 8 |  |
| z_nodelete | `-z now` | `-z nodelete` | 0 | 0 | 4496 | 4496 | 0x1337c8 | 0x6f70 | 0x26290 | 0x148 | 8 |  |
| z_nodlopen | `-z now` | `-z nodlopen` | 0 | 0 | 4496 | 4496 | 0x1337c8 | 0x6f70 | 0x26290 | 0x148 | 8 |  |
| z_nodump | `-z now` | `-z nodump` | 0 | 0 | 4496 | 4496 | 0x1337c8 | 0x6f70 | 0x26290 | 0x148 | 8 |  |
| z_interpose | `-z now` | `-z interpose` | 0 | 0 | 4496 | 4496 | 0x1337c8 | 0x6f70 | 0x26290 | 0x148 | 8 |  |
| emit_relocs | `-z now` | `--emit-relocs` | 1 |  |  |  |  |  |  |  |  | link fail: internal error in relocate_relocs, at /buildbot/src/android/binutils/toolchain/binutils/binutils-2.27/gold/target-reloc.h:853 |
| pack_dyn_relocs_none | `-z now` | `--pack-dyn-relocs=none` | 1 |  |  |  |  |  |  |  |  | link fail: use the --help option for usage information |
| orphan_warn | `-z now` | `--orphan-handling=warn` | 1 |  |  |  |  |  |  |  |  | link fail: use the --help option for usage information |
| orphan_place | `-z now` | `--orphan-handling=place` | 1 |  |  |  |  |  |  |  |  | link fail: use the --help option for usage information |
| orphan_error | `-z now` | `--orphan-handling=error` | 1 |  |  |  |  |  |  |  |  | link fail: use the --help option for usage information |
| no_keep_memory | `-z now` | `--no-keep-memory` | 0 | 0 | 4496 | 4496 | 0x1337c8 | 0x6f70 | 0x26290 | 0x148 | 8 |  |
| reduce_memory_overheads | `-z now` | `--reduce-memory-overheads` | 1 |  |  |  |  |  |  |  |  | link fail: use the --help option for usage information |
| noinhibit_exec | `-z now` | `--noinhibit-exec` | 0 | 0 | 4496 | 4496 | 0x1337c8 | 0x6f70 | 0x26290 | 0x148 | 8 |  |
| combined_explicit_defaults | `-z now` | `-z relro -z noexecstack --hash-style=sysv --build-id=sha1 -z max-page-size=4096 -z common-page-size=4096 --no-as-needed -z combreloc --spare-dynamic-tags=5 --no-undefined --warn-shared-textrel --fatal-warnings --fix-cortex-a8` | 0 | 0 | 4496 | 4496 | 0x1337c8 | 0x6f70 | 0x26290 | 0x148 | 8 |  |
