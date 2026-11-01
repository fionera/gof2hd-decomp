# Vendored archive provenance: zlib, libzip, libcrypto

Status: research note (2026-11-01). Worktree `wt/w3425`, branch `wave87-w3425`,
base `d00e0030`. No `src/`, CMake or vendored-source change landed: every tested
alternative was neutral or worse on the gate, so this commit is the note only.

Scope: the three static archives the original `libgof2hdaa.so` statically linked
(`third_party/{zlib,libzip,openssl}`), not the NDK runtime archives
(`libc++abi.a`, `libandroid_support.a`, `libgcc.a`). The question is provenance:
which upstream versions, compiler and flags produced the code in the original,
and which parts of that claim are actually witnessed by the binary.

## TL;DR

| archive | upstream | compiler | codegen flags | evidence strength |
|---|---|---|---|---|
| `libz_gof2.a` | zlib **1.2.3** | NDK r18b clang 7.0.2 | `-Oz -g -fstack-protector-strong -mthumb` | **proven** by version/copyright strings, symbol set, and a 1.2.7 A/B |
| `libzip_gof2.a` | libzip **0.9.3** | NDK r18b clang 7.0.2 | same app flags; NDK r18b sysroot `zlib.h` 1.2.7 | **proven** by `_zip_nerr_str=24`, the pre-0.11 API, and config A/Bs |
| `libcrypto_gof2.a` | OpenSSL **1.1.0 line** (pinned 1.1.0l) | real **GCC 4.9.x** (NDK r13b) | `-O3 -march=armv7-a` ARM | **GCC + 1.1.0 line proven**; exact patch letter **not observable** |

All 144 exported functions defined by the three archives are `linked_exact`
(instruction-identical modulo link addresses) in the current tree; 40 are
`byte_exact`. The archive source trees are byte-identical to the upstream
tarballs except for generated headers/objdirs. The `.a` builds are deterministic
and location-independent.

## Baseline

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8
```

At `d00e0030`:

```
verify: byte 4182  linked 4495  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
size-exact 4523 / 4524
DRIFT: 0 field(s) across 0 class(es)
lint CLEAN
```

Every A/B below changes only the vendored archive (or one of its objects) and
relinks the existing tree; `src/` is untouched. The full gate line is reported
for each.

## 1. zlib is 1.2.3

### Direct binary witnesses

```sh
strings -a -t x ../_work/bins/android_2.0.16_libgof2hdaa.so | grep -E '1\.2\.[0-9]|Copyright 1995'
```

```
 1e6495 1.2.3
 1e650f 1.2.7
 deflate 1.2.3 Copyright 1995-2005 Jean-loup Gailly
 inflate 1.2.3 Copyright 1995-2005 Mark Adler
```

`zlibVersion` returns the `1.2.3` string:

```
0018a6f0 <zlibVersion@@Base>:
  18a6f0: ldr r0, [pc, #4]
  18a6f2: add r0, pc
  18a6f4: bx  lr
  ... pool word 0x0005bd9f -> 0x1e6495 ("1.2.3")
```

The `1.2.7` string is *not* zlib's own version: it is the `ZLIB_VERSION` that
libzip passes to `deflateInit2_`/`inflateInit2_` (see §2). The two references are
inside `zip_close` (`0x18d29a`) and `zip_fopen_index` (`0x18c834`); zlib's own
`compress2`/`uncompress` pass `1.2.3` (`0x187fe4`, `0x18aad8`).

The exported zlib symbol set is 1.2.3's: `adler32_combine`, `crc32_combine`,
`deflatePrime`, `deflateTune`, `inflateGetHeader`, `inflateBack`, ... and **no**
`inflateReset2`/`inflateResetKeep`/`deflatePending`/`*_combine64` (all added in
1.2.4+) and no `gz*` layer.

### Controlled A/B: zlib 1.2.7 instead of 1.2.3

Built 1.2.7 with the identical unit list, member order and flags, swapped the
archive, relinked, verified:

| variant | byte | linked | size-exact | extra | zlib funcs not linked or size-mismatch |
|---|---:|---:|---:|---:|---:|
| canonical 1.2.3 | **4182** | **4495** | **4523/4524** | 0 | 0 / 47 |
| 1.2.7 | 1339 | 4466 | 4501/4524 | **9** | **29 / 47** |

The 9 new exports are `_tr_flush_bits`, `adler32_combine64`, `crc32_combine64`,
`deflatePending`, `deflateResetKeep`, `inflateMark`, `inflateReset2`,
`inflateResetKeep`, `inflateUndermine`. 1.2.7 is decisively wrong.

### zlib build flags: `-Oz -g -fstack-protector-strong` are required

Each variant rebuilt all 11 zlib objects and relinked:

| zlib flags | byte | linked | size-exact | zlib funcs not linked or size-mismatch |
|---|---:|---:|---:|---:|
| canonical `-Oz -g -fstack-protector-strong` | **4182** | **4495** | **4523/4524** | 0 / 47 |
| drop `-g` | 4184 | 4475 | 4521/4524 | 20 / 47 |
| `-O2` instead of `-Oz` | 1333 | 4459 | 4487/4524 | not measured |
| drop `-fstack-protector-strong` | 3120 | 4488 | 4516/4524 | not measured |

The `-g` result is the same clang-7 `-Oz` PostRA/DBG_VALUE effect documented for
the app TUs: without `-g`, 20 zlib functions lose `linked_exact` and two change
size (`deflateCopy` 280->276, `inflateInit2_` 160->164). The `byte +2` in the
no-`-g` row is layout noise; `linked` and `size-exact` regress, so it is not a
strict improvement.

## 2. libzip is 0.9.3, compiled against zlib 1.2.7 headers

### Direct binary witnesses

```sh
NM=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm
$NM -D -S --defined-only ../_work/bins/android_2.0.16_libgof2hdaa.so | grep -E '_zip_nerr_str|_zip_err_str'
```

```
0020e678 00000060 D _zip_err_str     # 0x60 = 24 pointers
0020aafc 00000060 R _zip_err_type    # 0x60 = 24 ints
0020aaf8 00000004 R _zip_nerr_str    # value 24
```

Read the value directly (`.rodata` file offset == VMA in this `.so`):

```sh
python3 -c "import struct; d=open('../_work/bins/android_2.0.16_libgof2hdaa.so','rb').read(); print(struct.unpack_from('<I',d,0x20aaf8)[0])"
# 24
```

`_zip_nerr_str = 24` means the error table ends at `ZIP_ER_DELETED` (23). The
0.10 table adds `ZIP_ER_ENCRNOTSUPP` (24) and `ZIP_ER_RDONLY` (25), so the table
would be 26 entries. The original also uses the pre-0.11 API names
`zip_add`/`zip_add_dir`/`zip_get_num_files` (renamed in 0.11) and exports
`_zip_dirent_torrent_normalize`. The string `Entry has been deleted` is present;
`Encryption not supported`/`Read-only archive` are absent.

`zip_close` and `zip_fopen_index` pass `"1.2.7"` to zlib (the NDK r18b sysroot
`zlib.h`), while the zlib code itself is 1.2.3. This is the expected result of
building libzip against the NDK sysroot headers, not the vendored zlib headers.

### config.h A/Bs

`third_party/build_match_lib.cmake` writes a hand-written `lib/config.h`. Two
macros are directly witnessed:

| variant | byte | linked | size-exact | effect |
|---|---:|---:|---:|---|
| canonical `HAVE_MKSTEMP 1` | **4182** | **4495** | **4523/4524** | `zip_close` calls `mkstemp@plt`; import present |
| `HAVE_MKSTEMP` line removed | 1683 | 4494 | 4523/4524 | `zip_close` calls `_zip_mkstemp`; `mkstemp` import gone; `zip_close` 99.9% |
| canonical `HAVE_STRUCT_TM_TM_ZONE 1` | **4182** | **4495** | **4523/4524** | `_zip_dirent_torrent_normalize` uses `tm_gmtoff`/`tm_zone` |
| `HAVE_STRUCT_TM_TM_ZONE` line removed | 3121 | 4494 | 4522/4524 | `_zip_dirent_torrent_normalize` 49.3%, 148 B vs 216 B |

Gotcha: `#define HAVE_MKSTEMP 0` is **not** "off". `zipint.h` tests
`#ifndef HAVE_MKSTEMP`, so the line must be absent. The first A/B (value 0) was
gate-neutral for exactly that reason; the row above is the line-removed run.

Disassembly of the original's `_zip_dirent_torrent_normalize` (`0x18f380`)
shows the `HAVE_STRUCT_TM_TM_ZONE` path directly:

```
18f3d2: blx localtime@plt
18f3d6: ldr r1, [r0, #36]   ; tm_gmtoff
18f3d8: str r1, [r5, #36]
18f3da: ldr r0, [r0, #40]   ; tm_zone
18f3dc: str r0, [r6, #0]
18f3e0: blx mktime@plt
```

### libzip `-g`

| libzip flags | byte | linked | size-exact |
|---|---:|---:|---:|
| canonical `-Oz -g -fstack-protector-strong` | **4182** | **4495** | **4523/4524** |
| drop `-g` | 3122 | 4456 | 4507/4524 |

So libzip also requires `-g`, for the same clang-7 `-Oz` reason.

## 3. libcrypto is GCC 4.9, OpenSSL 1.1.0-line ARM

### Direct binary witnesses

`CRYPTO_memcmp` sits at `0x18feb0`, between `OPENSSL_cleanse` and
`_armv7_neon_probe`, in the `armv4cpuid.pl` routine order. In OpenSSL 1.0.2 it is
C in `cryptlib.c`; it moved into `armv4cpuid` in 1.1.0. The original also exports
the 1.1.0-era `_armv8_aes_probe`/`_armv8_sha1_probe`/`_armv8_sha256_probe`/
`_armv8_pmull_probe` probes. That pins the **1.1.0 line**, not 1.0.2.

The original `.comment` contains `GCC: (GNU) 4.9.x 20150123 (prerelease)`, which
is exactly NDK r13b's GCC string. The string alone is not decisive (r18b's
`libgcc.a` carries the same GCC build), so the compiler claim rests on codegen.

### Controlled A/B: clang 7.0.2 instead of GCC 4.9

Compiled the same three sources with NDK r18b clang 7.0.2 `-O3 -marm` and the
same defines, swapped the archive, relinked:

| variant | byte | linked | size-exact | SHA C funcs not linked |
|---|---:|---:|---:|---:|
| canonical GCC 4.9 r13b | **4182** | **4495** | **4523/4524** | 0 / 7 |
| clang 7.0.2 | 1345 | 4488 | 4516/4524 | **7 / 7** |

The 14 assembly functions (`armv4cpuid.S`, `sha256-armv4.S`) are byte-identical
under either assembler; only the C `sha256.c` functions differ, and they differ
in size (`SHA256_Update` 272 vs 292, `SHA256_Final` 1048 vs 796, ...). This is
the clean proof that `sha256.c` was compiled by GCC 4.9, not clang.

### OpenSSL source version: 1.1.0l vs 1.1.1w is not observable

Built the same subset from a pristine `openssl-1.1.1w` tarball with the same GCC
and flags. The `.text` of all three objects is **byte-identical**:

```
armv4cpuid:   384 B, 0 differing bytes
sha256:      1804 B, 0 differing bytes
sha256-armv4: 7520 B, 0 differing bytes
```

The full `sha256.o` hashes differ only in the compiler-generated local static
symbol names (`m.6561` vs `m.6621`), which do not survive the stripped link.
So the binary proves the 1.1.0/1.1.1-line subset, not the exact patch letter;
`1.1.0l` is a conservative pin (last 1.1.0 release), not a binary-derived fact.

### OpenSSL flags

| variant | sha256.c `.text` |
|---|---|
| `-O3 -march=armv7-a` (canonical) | 1804 B, reference |
| `-O2 -march=armv7-a` | 1804 B, **0 differing bytes** |
| `-O1` | 1220 B, 1014 differing bytes |
| `-Os` | 1140 B, 998 differing bytes |
| `-O3 -march=armv5te` | 1804 B, 664 differing bytes |
| `-O3 -march=armv6` | 1804 B, 674 differing bytes |

`-march=armv7-a` is required (the original uses ARMv7 instructions); `-O2` and
`-O3` are indistinguishable for this subset, so `-O3` is the OpenSSL Configure
default and is sufficient, but not uniquely proven.

The exact GCC NDK revision (r13b vs r11c vs r16b/r17c) is **not** re-tested
here. The build script records that r11c/r13b reproduce `SHA256_Update` and
r16b/r17c do not; r13b is the pinned compiler and reproduces the original. Treat
the exact revision as an earlier finding, not a fresh measurement.

## 4. Archive member order

The original link had no section-ordering file, so for a `--whole-archive`
library the member order is directly observable in the original `.text`.

* **zlib** (whole-archive): the archive order is
  `inflate, compress, deflate, crc32, infback, zutil, inftrees, uncompr, trees,
  inffast, adler32`. The original's exported-function address ranges are
  monotonically increasing in exactly that order. Proven. Reproduction: list
  each member's exported symbols with `nm -A --defined-only -g
  third_party/zlib/libz_gof2.a`, look each name up in `nm -D -n
  --defined-only ../_work/bins/...so`, and check that each member's minimum
  address is greater than the previous member's maximum.
* **libzip** (whole-archive): the 54-member order in
  `third_party/build_match_lib.cmake` is likewise monotone with the original's
  `.text` ranges. Proven.
* **libcrypto** (not whole-archive): only the linker's *pull order* is
  observable (`sha256.o`, `armv4cpuid.o`, `sha256-armv4.o` in `.text`), not the
  archive member order. The current archive order is `armv4cpuid, sha256,
  sha256-armv4`; the `.text` order is reproduced by the section-ordering file.
  A controlled A/B reordering the archive to `sha256, armv4cpuid,
  sha256-armv4` gave **byte 4182 / linked 4495 / size 4523/4524** — gate-neutral,
  as expected. Do not treat the libcrypto `.a` member order as evidence.

## 5. Download and build reproducibility

`third_party/build_match_lib.cmake` downloads the tarballs on first use and
extracts them into `third_party/<lib>/`. There is **no `EXPECTED_HASH`** on the
`file(DOWNLOAD ...)` calls. The hashes of the tarballs fetched in this pass:

```
1795c7d067a43174113fdf03447532f373e1c6c57c08d61d9e4e9be5e244b05e  zlib-1.2.3.tar.gz
0e282dfed9e3da7d52223a0ccaee7c950782f10ed73f9b778243128a03c8cb1c  libzip-0.9.3.tar.gz
74a2f756c64fd7386a29184dc0344f4831192d61dc2481a93a4c5dd727f41148  openssl-1.1.0l.tar.gz
```

The extracted trees are pristine upstream apart from generated files:

```sh
diff -rq --exclude=_obj_gof2 /tmp/vend/src/zlib-1.2.3 third_party/zlib/zlib-1.2.3
diff -rq --exclude=_obj_gof2 /tmp/vend/src/libzip-0.9.3 third_party/libzip/libzip-0.9.3
diff -rq --exclude=_obj_gof2 /tmp/vend/src/openssl-1.1.0l third_party/openssl/openssl-1.1.0l
```

```
zlib:    (no output)
libzip:  Only in third_party/libzip/libzip-0.9.3/lib: config.h
openssl: Only in ...: Makefile
         Only in ...: configdata.pm
         Only in .../include/openssl: opensslconf.h
```

The archives are deterministic: re-running the builder in place reproduces the
same bytes.

```
ed6a6e43d5927e5ba6d1c27bf32336b278d645e4e0875aa63c5b2ef24e6147bf  libz_gof2.a
4937dc20218439aaa7577c61aae35a50381c284e228d3535e888f7d31ca9dab7  libzip_gof2.a
4d793119233f6a10b25f84a025efc8bfc7516a2c3dff09325760862ca87be23f  libcrypto_gof2.a
```

The code is also location-independent: compiling the OpenSSL subset from a fresh
`/tmp` extraction produces byte-identical `.o` files, and compiling zlib from a
fresh `/tmp` extraction produces byte-identical `.text` for all 11 objects. The
`.o` full hashes for zlib differ only because `-g` embeds the source path; the
final link is stripped.

## 6. Per-function match of the vendored code

Canonical build, `report.json`, functions defined by each archive:

| archive | funcs | byte_exact | linked_exact | not linked |
|---|---:|---:|---:|---:|
| zlib | 47 | 14 | **47** | 0 |
| libzip | 74 | 12 | **74** | 0 |
| libcrypto | 23 | 14 | **23** | 0 |
| **total** | **144** | **40** | **144** | **0** |

The non-`byte_exact` rows differ only in link-dependent words (PLT slot
immediates, `.rodata` string offsets, `.got`/`.bss` deltas); none is a codegen
difference. That is the strongest single statement of provenance: every
instruction of every vendored function matches the original.

## 7. Proven vs hypothesis

**Proven by the original binary or a controlled A/B:**

* zlib is **1.2.3** (`zlibVersion`, copyright strings, symbol set, 1.2.7 A/B).
* zlib/libzip are clang 7.0.2 `-Oz -g -fstack-protector-strong -mthumb`; each
  flag's removal regresses `linked_exact` and/or `size-exact`.
* libzip is **0.9.3** (`_zip_nerr_str=24`, pre-0.11 API, error strings).
* libzip was compiled against the NDK r18b sysroot `zlib.h` **1.2.7**, while the
  zlib code is 1.2.3.
* libzip `HAVE_MKSTEMP` and `HAVE_STRUCT_TM_TM_ZONE` are set.
* libcrypto's `sha256.c` was compiled by **GCC 4.9**, not clang (clang A/B).
* libcrypto is the **1.1.0 line** (CRYPTO_memcmp in `armv4cpuid`).
* libcrypto uses `-march=armv7-a`; `-O2`/`-O3` are indistinguishable.
* zlib/libzip archive member order matches the original's `.text` order.

**Not proven / still hypothesis:**

* The exact OpenSSL patch letter: 1.1.0l and 1.1.1w produce identical `.text`
  for this subset. 1.1.0l is a conservative pin.
* The exact GCC NDK revision: r13b is pinned and matches; r11c/r16b/r17c were
  not re-tested in this pass (the build script records an earlier r11c/r13b vs
  r16b/r17c result).
* libcrypto archive member order: unobservable (not whole-archive); the
  observable pull order is reproduced by the section-ordering file.
* The tarball URLs are not hash-pinned, so a rebuild from the network is only as
  reproducible as the upstream URLs. The hashes above are the current ones.

## 8. Recommendations

1. Add `EXPECTED_HASH SHA256=...` to the three `file(DOWNLOAD ...)` calls in
   `ensure_source()` so the provenance is cryptographically pinned. This is
   gate-neutral (same sources, same `.a` bytes) and can be done separately.
2. Leave the zlib/libzip flags and member orders alone; every tested alternative
   regresses the gate.
3. Do not spend time on the libcrypto archive member order; it is unobservable.
4. If the exact OpenSSL patch letter ever matters, it must be established from
   something other than this subset's code (the binary cannot distinguish it).

## 9. Reproduction

Baseline and gate:

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8
```

Rebuild the vendored archives in place (deterministic):

```sh
NDK=.cache/ndk/android-ndk-r18b; HOST=linux-x86_64
DEFS="-DGOF2_NDK_ROOT=$PWD/$NDK \
 -DGOF2_NDK_CLANG=$PWD/$NDK/toolchains/llvm/prebuilt/$HOST/bin/clang \
 -DGOF2_NDK_AR=$PWD/$NDK/toolchains/llvm/prebuilt/$HOST/bin/llvm-ar \
 -DGOF2_NDK_GCC_BINDIR=$PWD/$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/$HOST/arm-linux-androideabi/bin \
 -DTP_DIR=$PWD/third_party"
for lib in zlib libzip openssl; do
  cmake $DEFS -DGOF2_LIB=$lib -P third_party/build_match_lib.cmake
done
sha256sum third_party/zlib/libz_gof2.a third_party/libzip/libzip_gof2.a \
          third_party/openssl/libcrypto_gof2.a
```

A/B swap pattern (used for every table above):

```sh
cp third_party/<lib>/lib<name>_gof2.a /tmp/canon.a
cp /tmp/variant.a third_party/<lib>/lib<name>_gof2.a
cmake --build cmake-build-match --target link_so -j8
cmake --build cmake-build-match --target verify -j8
python3 -c 'import json; r=json.load(open("cmake-build-match/verify/report.json")); print(r["summary"])'
# restore
cp /tmp/canon.a third_party/<lib>/lib<name>_gof2.a
cmake --build cmake-build-match --target link_so -j8
```

The zlib 1.2.7 and OpenSSL 1.1.1w sources used for the A/Bs were extracted under
`/tmp/vend/src/`; the tarball hashes are in §5.
