# Vendored library build flags: zlib 1.2.3, libzip 0.9.3, libcrypto 1.1.0-line (w5212)

Status: research note, 2026-11-04. Worktree `wt/w5212`, branch `wave104-w5212`,
base `8c5fd3646`. No `src/`, CMake, vendored-source or baseline change landed: **no strict
improvement exists**. This commit is the note only.

Scope: the three static archives the original `libgof2hdaa.so` statically linked
(`third_party/{zlib,libzip,openssl}`, built by `third_party/build_match_lib.cmake`), not the NDK
runtime archives (`libc++abi.a`, `libandroid_support.a`, `libgcc.a`).

## TL;DR

| archive | upstream | compiler | codegen flags (canonical) | evidence strength |
|---|---|---|---|---|
| `libz_gof2.a` | zlib **1.2.3** | NDK r18b clang 7.0.2 | `-Oz -g -fstack-protector-strong -mthumb -fPIC -ffunction-sections -funwind-tables` | **proven** by version strings, symbol set, and every flag A/B |
| `libzip_gof2.a` | libzip **0.9.3** | NDK r18b clang 7.0.2 | same app flags; `-mfpu=neon` is observable; NDK r18b sysroot `zlib.h` 1.2.7 | **proven** by `_zip_nerr_str=24`, pre-0.11 API, config A/Bs |
| `libcrypto_gof2.a` | OpenSSL **1.1.0 line** (pinned 1.1.0l) | real **GCC 4.9.x** (NDK r13b) | `-O3 -march=armv7-a` ARM; no stack protector, no unwind tables, no data sections; `-DSHA256_ASM` | **GCC + 1.1.0-line proven**; exact patch letter unobservable |

Baseline after restoring the canonical archives and relinking:

```
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.9
size-exact 4524/4524
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
DRIFT: 0 field(s) across 0 class(es)
lint CLEAN
```

The strongest single fact: **the entire vendored `.text` block is byte-identical to the
original**, not merely the 144 exported functions. The block is
`0x1869f4 .. 0x191d20` (45,868 bytes, from `inflateReset` to the end of
`sha256_block_data_order_neon`, immediately before `__cxa_end_cleanup`):

```
sha256(original[0x1869f4:0x191d20]) = 0a3829ddc1576202b43c2fe7516b70cafda708d31d179c073d4f5ee4006dafdf
sha256(ours    [0x1869f4:0x191d20]) = 0a3829ddc1576202b43c2fe7516b70cafda708d31d179c073d4f5ee4006dafdf
differing bytes: 0
```

All 144 exported vendored functions are `byte_exact` **and** `linked_exact` **and** size-exact
(144/144). `.rodata` and `.data` are byte-identical between the two links (0 differing bytes), so
the vendored tables/strings are exact too. The vendored unwind data is semantically exact: 112
vendored functions have `.ARM.exidx` entries whose `readelf --unwind` opcode rows are identical;
the other 32 (the OpenSSL functions and a few leaves) have no entry in either binary.

Consequence: no codegen flag can improve the vendored code. Every non-canonical codegen choice
either produces exactly the same bytes (unobservable) or breaks the block (regression). The
experiments below identify which is which.

## 1. Canonical recipe (as built)

`third_party/build_match_lib.cmake` is the only source of truth; the flags below are copied from
it. `CC_BASE` is shared by all three libraries:

```
--target=armv7a-linux-androideabi16 -mfpu=neon -mfloat-abi=softfp
--sysroot=<NDK>/platforms/android-16/arch-arm
-isystem <NDK>/sysroot/usr/include
-isystem <NDK>/sysroot/usr/include/arm-linux-androideabi
-fPIC -ffunction-sections -funwind-tables -Wno-everything
```

* **zlib** (11 units, member order `inflate compress deflate crc32 infback zutil inftrees
  uncompr trees inffast adler32`), per unit:
  `-Oz -g -fstack-protector-strong -I. -mthumb`
* **libzip** (54 units, member order in the script), per unit:
  `-Oz -g -fstack-protector-strong -I. -mthumb`; `zip_file_strerror.c` additionally gets
  `-mllvm -align-all-functions=2`.
* **libcrypto** (3 objects, archive order `sha256.o armv4cpuid.o sha256-armv4.o`), GCC 4.9.x from
  NDK r13b:
  ```
  --sysroot=<NDK>/platforms/android-16/arch-arm
  -isystem <NDK>/sysroot/usr/include -isystem <NDK>/sysroot/usr/include/arm-linux-androideabi
  -D__ANDROID_API__=16 -march=armv7-a
  -DDSO_DLFCN -DHAVE_DLFCN_H -DNDEBUG -DOPENSSL_THREADS -DOPENSSL_NO_DYNAMIC_ENGINE
  -DOPENSSL_PIC -DSHA256_ASM -DOPENSSL_USE_NODELETE
  -Wall -O3 -pthread -mandroid -fPIC -Wa,--noexecstack
  -I. -Icrypto/include -Iinclude -Icrypto -w
  ```
  `crypto/sha/sha256.c`, `crypto/armv4cpuid.pl` -> `armv4cpuid.S`,
  `crypto/sha/asm/sha256-armv4.pl` -> `sha256-armv4.S`, all `linux32` flavour.

Canonical archive hashes (after the final restore/rebuild in this worktree). Note that `-g`
embeds the source path in each `.o`, so an archive rebuilt from a different source directory has a
different archive hash; the linked `.so` is identical. The hashes below are from the worktree's own
source tree:

```
b7dfc6e2ca95af5ac839720d4c57c3dbf2b0a83292c483597cb9938f111c178d  third_party/zlib/libz_gof2.a
0c7a519760d4d62bbfa8f74e36c82f85f66149ac62800cd6a5d2eecef935e58c  third_party/libzip/libzip_gof2.a
50662126d068d8b96ece99f83dafd04f975c43263c193eedca5ac5f3fa5595f2  third_party/openssl/libcrypto_gof2.a
```

Source tarball hashes (re-downloaded and checked in this pass):

```
1795c7d067a43174113fdf03447532f373e1c6c57c08d61d9e4e9be5e244b05e  zlib-1.2.3.tar.gz
0e282dfed9e3da7d52223a0ccaee7c950782f10ed73f9b778243128a03c8cb1c  libzip-0.9.3.tar.gz
74a2f756c64fd7386a29184dc0344f4831192d61dc2481a93a4c5dd727f41148  openssl-1.1.0l.tar.gz
cf3098950cb4d853ad95c0841f1f9c6d3dc102dccfcacd521d93925208b76ac8  openssl-1.1.1w.tar.gz  (A/B only)
```

The extracted trees are pristine upstream apart from generated files:

```
zlib:    (no output)
libzip:  Only in third_party/libzip/libzip-0.9.3/lib: config.h
openssl: Only in ...: Makefile, configdata.pm, include/openssl/opensslconf.h
```

Canonical `.so` section sizes (original == ours for every header; only content differs):

| section | size | differing bytes |
|---|---:|---:|
| `.text` | 0x1337c8 (1,259,464) | 100,113 |
| `.ARM.exidx` | 0x6f70 (28,528) | 829 |
| `.ARM.extab` | 0x26298 (156,312) | 108,295 |
| `.rodata` | 0x41b67 (269,159) | **0** |
| `.data` | 0x296c (10,604) | **0** |
| `.bss` | 0x69ac (27,052) | NOBITS |
| `.rel.plt` | 0x72d0 (29,392) | **0** |
| `.dynsym` | 0x15ca0 (89,248) | **0** |
| `.dynstr` | 0x2e318 (189,208) | **0** |
| `.hash` | 0x973c (38,716) | **0** |
| `.rel.dyn` | 0x7048 (28,744) | **0** |
| `.ARM.attributes` | 0x38 | **0** |

The `.ARM.extab` content diff is upstream of the vendored block (the 24 non-byte-exact app
functions include `Generator::createMission` -4 and `LevelScript::process` +4, so extab records
between them are shifted); the vendored unwind rows themselves are identical (see above). Note
that `docs/IDENTICALITY.md`'s inline "only differing section bytes are `.text` 102,777;
`.ARM.exidx` 842; `.rel.plt` 0; `.note.gnu.build-id` 20" sentence is stale for this tree: the
`.text` count here is 100,113, and `.ARM.extab` also has 108,295 differing bytes even though its
size and base are exact. This is not caused by any vendored flag.

## 2. Method

All experiments changed only one vendored archive, relinked the existing tree, and re-ran
`tools/verify/verify.py` (the same tool the `verify`/`check` target uses). `src/` was never touched.
The variant builder is a copy of `third_party/build_match_lib.cmake` with only the flag token list
replaced; the source trees were copied to `/dev/shm/w5212/tpvar` so the worktree's downloaded trees
were not disturbed. The canonical archives were restored at the end and the final `check`/`drift`
were re-run (numbers above).

Exact commands:

```sh
# baseline
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
cmake --build cmake-build-match --target drift -j6

# one variant archive (TP_DIR is the /dev/shm copy of third_party)
cmake -DGOF2_NDK_ROOT=$PWD/.cache/ndk/android-ndk-r18b \
      -DGOF2_NDK_CLANG=$PWD/.cache/ndk/android-ndk-r18b/toolchains/llvm/prebuilt/linux-x86_64/bin/clang \
      -DGOF2_NDK_AR=$PWD/.cache/ndk/android-ndk-r18b/toolchains/llvm/prebuilt/linux-x86_64/bin/llvm-ar \
      -DGOF2_NDK_GCC_BINDIR=$PWD/.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/arm-linux-androideabi/bin \
      -DTP_DIR=/dev/shm/w5212/tpvar -DGOF2_LIB=zlib -P /dev/shm/w5212/build_variant.cmake

# swap + relink + verify
cp /dev/shm/w5212/archives/<variant>.a third_party/zlib/libz_gof2.a
cmake --build cmake-build-match --target link_so -j6
python3 tools/verify/verify.py --build-dir /dev/shm/w5212/verify/<variant> \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --top 0
```

The `.text` block was compared directly with:

```sh
python3 - <<'EOF'
import hashlib
o=open('/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so','rb').read()
u=open('cmake-build-match/libgof2hdaa.so','rb').read()
print(hashlib.sha256(u[0x1869f4:0x191d20]).hexdigest(),
      sum(1 for a,b in zip(o[0x1869f4:0x191d20],u[0x1869f4:0x191d20]) if a!=b))
EOF
```

Each table row is one full relink + full verify. `byte`/`linked` are the gate's global counts
(out of 4,524); `vendored` is the count of the 144 exported vendored functions that are
`byte_exact`. `REGRESSION` means the row is worse than canonical on the gate; `neutral` means
exactly 4500/4500 and 144/144 (the `.so` is identical apart from the build-id).

## 3. zlib 1.2.3

### 3.1 Binary witnesses

```sh
strings -a -t x .../android_2.0.16_libgof2hdaa.so | grep -E '1\.2\.[0-9]|Copyright 1995'
#  1e6495 1.2.3
#  1e650f 1.2.7
#  2076bc  deflate 1.2.3 Copyright 1995-2005 Jean-loup Gailly
#  209f9a  inflate 1.2.3 Copyright 1995-2005 Mark Adler
```

`zlibVersion` returns the `1.2.3` string. The exported set is 1.2.3's 47 functions; the 1.2.4+
symbols (`inflateReset2`, `inflateResetKeep`, `deflatePending`, `adler32_combine64`,
`crc32_combine64`, `inflateMark`, `deflateResetKeep`) are absent. The `1.2.7` string is libzip's
`ZLIB_VERSION` argument to `deflateInit2_`/`inflateInit2_`, not zlib's own version.

### 3.2 Flag matrix

### zlib 1.2.3

| variant | flags changed | byte | linked | vendored | .text | .ARM.exidx | .ARM.extab | .rodata | verdict |
|---|---|---:|---:|---:|---:|---:|---:|---:|---|
| `zlib_canon` | canonical | 4500 | 4500 | 144/144 | 0x1337c8 | 0x6f70 | 0x26298 | 0x41b67 | neutral |
| `zlib_Os` | -Os | 2259 | 4480 | 48/144 | 0x133aa8 | 0x6f70 | 0x26298 | 0x41b67 | REGRESSION |
| `zlib_O2` | -O2 | 1344 | 4464 | 28/144 | 0x134e38 | 0x6f40 | 0x26260 | 0x41b67 | REGRESSION |
| `zlib_O3` | -O3 | 1344 | 4464 | 28/144 | 0x135588 | 0x6f30 | 0x26254 | 0x41b67 | REGRESSION |
| `zlib_arm` | -marm | 2215 | 4453 | 26/144 | 0x136758 | 0x6f38 | 0x261b4 | 0x41b67 | REGRESSION |
| `zlib_nosp` | drop -fstack-protector-strong | 3178 | 4493 | 39/144 | 0x133668 | 0x6f70 | 0x26298 | 0x41b67 | REGRESSION |
| `zlib_nog` | drop -g | 4474 | 4480 | 118/144 | 0x1337c8 | 0x6f70 | 0x26298 | 0x41b67 | REGRESSION |
| `zlib_nounwind` | + -fno-unwind-tables | 4040 | 4500 | 126/144 | 0x1337c8 | 0x6d50 | 0x2607c | 0x41b67 | REGRESSION |
| `zlib_data` | + -fdata-sections | 4499 | 4500 | 143/144 | 0x1337c8 | 0x6f70 | 0x26298 | 0x41b67 | REGRESSION |
| `zlib_nodata` | + -fno-data-sections | 4500 | 4500 | 144/144 | 0x1337c8 | 0x6f70 | 0x26298 | 0x41b67 | neutral |
| `zlib_nofunc` | + -fno-function-sections | 1708 | 4495 | 45/144 | 0x133798 | 0x6f20 | 0x26298 | 0x41b67 | REGRESSION |
| `zlib_nopic` | + -fno-pic | 1377 | 4479 | 39/144 | 0x133748 | 0x6f70 | 0x26298 | 0x41c07 | REGRESSION |
| `zlib_fpic` | -fpic instead of -fPIC | 4500 | 4500 | 144/144 | 0x1337c8 | 0x6f70 | 0x26298 | 0x41b67 | neutral |
| `zlib_UNALIGNED_OK` | + -DUNALIGNED_OK | 3189 | 4500 | 50/144 | 0x133788 | 0x6f70 | 0x26298 | 0x41b67 | REGRESSION |
| `zlib_FASTEST` | + -DFASTEST | 2245 | 4496 | 43/144 | 0x133188 | 0x6f58 | 0x26274 | 0x41b67 | REGRESSION |
| `zlib_NO_DIVIDE` | + -DNO_DIVIDE | 2257 | 4498 | 46/144 | 0x133c48 | 0x6f70 | 0x26298 | 0x41b67 | REGRESSION |
| `zlib_INFLATE_STRICT` | + -DINFLATE_STRICT | 3179 | 4497 | 40/144 | 0x133828 | 0x6f70 | 0x26298 | 0x41b67 | REGRESSION |
| `zlib_DYNAMIC_CRC_TABLE` | + -DDYNAMIC_CRC_TABLE | 2247 | 4497 | 45/144 | 0x1338c8 | 0x6f78 | 0x262a4 | 0x3fb57 | REGRESSION |
| `zlib_NO_GZIP` | + -DNO_GZIP | 3169 | 4493 | 39/144 | 0x132ec8 | 0x6f70 | 0x26298 | 0x41b27 | REGRESSION |
| `zlib_DEBUG` | + -DDEBUG | 1351 | 4481 | 33/144 | 0x134238 | 0x6f98 | 0x262bc | 0x42067 | REGRESSION |
| `zlib_armv5te` | -march=armv5te | 3144 | 4456 | 27/144 | 0x133b58 | 0x6ed8 | 0x260e8 | 0x41b67 | REGRESSION |
| `zlib_armv6` | -march=armv6 | 3144 | 4456 | 27/144 | 0x133a18 | 0x6ed8 | 0x260e8 | 0x41b67 | REGRESSION |
| `zlib_vfp` | -mfpu=vfpv3-d16 | 4500 | 4500 | 144/144 | 0x1337c8 | 0x6f70 | 0x26298 | 0x41b67 | neutral |
| `zlib_swap_crc_inf` | swap crc32/infback member order | 4493 | 4500 | 137/144 | 0x1337c8 | 0x6f70 | 0x26298 | 0x41b57 | REGRESSION |

`zlib_swap_crc_inf` also shrinks `.rodata` from `0x41b67` to `0x41b57` (the linker merges the
three `1.2.3` copyright strings differently); the table's default `0x41b67` is corrected here.

### 3.3 What the zlib matrix proves

* **`-Oz` is required.** `-Os` (2259/4480), `-O2` (1344/4464), `-O3` (1344/4464) all collapse the
  block. `-Oz` is the only level that reproduces it.
* **`-mthumb` is required.** `-marm` gives 2215/4453 and changes the `.text` size by +0x2f90.
  Every zlib function is Thumb in the original (`readelf -s` shows odd `st_value`).
* **`-fstack-protector-strong` is required.** Dropping it gives 3178/4493; seven functions change
  size (`crc32_combine` 208->176, `inflate_table` 928->872, `uncompress` 144->108,
  `inflateBack` 2568->2468, `compress2` 136->100, `inflateSync` 236->206, `inflate` 4360->4316).
* **`-g` is required.** Dropping it gives 4474/4480; 26 vendored functions lose `byte_exact`,
  20 lose `linked_exact`, and two change size (`inflateInit2_` 160->164, `deflateCopy` 280->276).
  This is the clang-7 `-Oz` PostRA/DBG_VALUE effect, not a debug-info artifact in the shipped file.
* **`-ffunction-sections` is required.** `-fno-function-sections` gives 1708/4495 and changes the
  `.text`/`.ARM.exidx` sizes.
* **`-funwind-tables` is required for section identity.** `-fno-unwind-tables` keeps `.text` size
  but shrinks `.ARM.exidx` 0x6f70->0x6d50 and `.ARM.extab` 0x26298->0x2607c; 18 functions that
  reference `.rodata` strings lose `byte_exact` (4040/4500). The code is unchanged; the section
  shift is the regression.
* **`-fPIC` is required.** `-fno-pic` gives 1377/4479 and changes `.text`/`.rodata`/`.rel.dyn`.
  `-fpic` is byte-identical to `-fPIC` (neutral).
* **`-fdata-sections` was NOT used.** Adding it changes `_tr_flush_block` and drops byte 4500->4499.
  `-fno-data-sections` is neutral (the default).
* **No zlib `-D` macro was used.** `-DUNALIGNED_OK` (3189/4500), `-DFASTEST` (2245/4496),
  `-DNO_DIVIDE` (2257/4498), `-DINFLATE_STRICT` (3179/4497), `-DDYNAMIC_CRC_TABLE` (2247/4497,
  removes the static CRC table: `.rodata` 0x41b67->0x3fb57, `.bss` +0x2000), `-DNO_GZIP`
  (3169/4493), `-DDEBUG` (1351/4481) all regress.
* **`-march=armv7-a` is required.** `-march=armv5te` / `-march=armv6` give 3144/4456.
* **`-mfpu=neon` is unobservable for zlib.** `-mfpu=vfpv3-d16` is exactly neutral
  (4500/4500, 144/144, 0-byte block diff). zlib has no FP/NEON-sensitive code.
* **Member order is required.** Swapping `crc32` and `infback` gives 4493/4500 (`.rodata`
  0x41b57); the exported-function address ranges are monotone in the canonical order
  (`inflate 0x1869f4..0x187efc`, `compress 0x187fcc..0x188068`, ... `adler32 0x18c2d4..0x18c4d8`).

## 4. libzip 0.9.3

### 4.1 Binary witnesses

```sh
python3 -c "import struct; d=open('...so','rb').read(); print(struct.unpack_from('<I',d,0x20aaf8)[0])"
# 24
readelf -sW ...so | grep -E ' _zip_nerr_str| _zip_err_str'
# _zip_err_str 0x60 = 24 pointers; _zip_nerr_str = 24
```

`_zip_nerr_str = 24` means the error table ends at `ZIP_ER_DELETED` (23); libzip 0.10 adds
`ZIP_ER_ENCRNOTSUPP` (24) and `ZIP_ER_RDONLY` (25), so the table would be 26. The original also
exports the pre-0.11 API names (`zip_add`, `zip_add_dir`, `zip_get_num_files`) and
`_zip_dirent_torrent_normalize`; the string `Entry has been deleted` is present, while
`Encryption not supported` / `Read-only archive` are absent. `zip_close` and `zip_fopen_index`
pass `"1.2.7"` (the NDK r18b sysroot `zlib.h`) to zlib, while the zlib code itself is 1.2.3.

### 4.2 Flag matrix

### libzip 0.9.3

| variant | flags changed | byte | linked | vendored | .text | .ARM.exidx | .ARM.extab | .rodata | verdict |
|---|---|---:|---:|---:|---:|---:|---:|---:|---|
| `libzip_canon` | canonical | 4500 | 4500 | 144/144 | 0x1337c8 | 0x6f70 | 0x26298 | 0x41b67 | neutral |
| `libzip_Os` | -Os | 1369 | 4483 | 40/144 | 0x133848 | 0x6f58 | 0x26274 | 0x41b67 | REGRESSION |
| `libzip_O2` | -O2 | 1355 | 4459 | 37/144 | 0x133d08 | 0x6f38 | 0x26268 | 0x41b67 | REGRESSION |
| `libzip_O3` | -O3 | 1355 | 4452 | 37/144 | 0x133ea8 | 0x6f38 | 0x26268 | 0x41b67 | REGRESSION |
| `libzip_arm` | -marm | 1346 | 4426 | 28/144 | 0x135168 | 0x6f70 | 0x2622c | 0x41b67 | REGRESSION |
| `libzip_nosp` | drop -fstack-protector-strong | 3197 | 4490 | 58/144 | 0x1335a8 | 0x6f70 | 0x26298 | 0x41b67 | REGRESSION |
| `libzip_nog` | drop -g | 4432 | 4468 | 76/144 | 0x1337c8 | 0x6f70 | 0x26298 | 0x41b67 | REGRESSION |
| `libzip_nounwind` | + -fno-unwind-tables | 4040 | 4500 | 126/144 | 0x1337c8 | 0x6b30 | 0x25fd4 | 0x41b67 | REGRESSION |
| `libzip_data` | + -fdata-sections | 4500 | 4500 | 144/144 | 0x1337c8 | 0x6f70 | 0x26298 | 0x41b67 | neutral |
| `libzip_nodata` | + -fno-data-sections | 4500 | 4500 | 144/144 | 0x1337c8 | 0x6f70 | 0x26298 | 0x41b67 | neutral |
| `libzip_nofunc` | + -fno-function-sections | 3981 | 4496 | 81/144 | 0x133798 | 0x6db8 | 0x26298 | 0x41b67 | REGRESSION |
| `libzip_nopic` | + -fno-pic | 1378 | 4481 | 40/144 | 0x1336e8 | 0x6f70 | 0x26298 | 0x41bc7 | REGRESSION |
| `libzip_fpic` | -fpic instead of -fPIC | 4500 | 4500 | 144/144 | 0x1337c8 | 0x6f70 | 0x26298 | 0x41b67 | neutral |
| `libzip_armv5te` | -march=armv5te | 3165 | 4427 | 46/144 | 0x133828 | 0x6f00 | 0x26094 | 0x41b67 | REGRESSION |
| `libzip_armv6` | -march=armv6 | 3165 | 4428 | 46/144 | 0x1337c8 | 0x6f00 | 0x26094 | 0x41b67 | REGRESSION |
| `libzip_vfp` | -mfpu=vfpv3-d16 | 3188 | 4493 | 58/144 | 0x133768 | 0x6f70 | 0x262a4 | 0x41b67 | REGRESSION |
| `libzip_no_mkstemp` | drop HAVE_MKSTEMP | 1750 | 4499 | 58/144 | 0x1337c8 | 0x6f70 | 0x26298 | 0x41b67 | REGRESSION |
| `libzip_no_tmzone` | drop HAVE_STRUCT_TM_TM_ZONE | 3232 | 4499 | 93/144 | 0x133788 | 0x6f70 | 0x26298 | 0x41b67 | REGRESSION |
| `libzip_no_fseeko` | drop HAVE_FSEEKO | 2886 | 4500 | 84/144 | 0x1337c8 | 0x6f70 | 0x26298 | 0x41b67 | REGRESSION |
| `libzip_no_ftello` | drop HAVE_FTELLO | 2899 | 4500 | 97/144 | 0x1337c8 | 0x6f70 | 0x26298 | 0x41b67 | REGRESSION |
| `libzip_no_libz` | drop HAVE_LIBZ | 4500 | 4500 | 144/144 | 0x1337c8 | 0x6f70 | 0x26298 | 0x41b67 | neutral |
| `libzip_no_align` | drop zip_file_strerror -mllvm -align-all-functions=2 | 4500 | 4500 | 144/144 | 0x1337c8 | 0x6f70 | 0x26298 | 0x41b67 | neutral |
| `libzip_swap_add_frc` | swap zip_add/zip_filerange_crc member order | 4481 | 4500 | 125/144 | 0x1337c8 | 0x6f70 | 0x26298 | 0x41b57 | REGRESSION |

Corrections to the table: `libzip_no_align` was verified separately as exactly neutral
(4500/4500, 144/144, 0-byte block diff); `libzip_swap_add_frc` also shrinks `.rodata` from
`0x41b67` to `0x41b57`.

### 4.3 What the libzip matrix proves

* **`-Oz`, `-mthumb`, `-fstack-protector-strong`, `-g`, `-ffunction-sections`,
  `-funwind-tables`, `-fPIC`, `-march=armv7-a` are all required**, for the same reasons and with
  the same failure modes as zlib. The numbers: `-Os` 1369/4483, `-O2` 1355/4459, `-O3`
  1355/4452, `-marm` 1346/4426, no stack protector 3197/4490, no `-g` 4432/4468 (9 functions
  change size), no unwind tables 4040/4500 (`.ARM.exidx` 0x6f70->0x6b30, 18 functions lose
  `byte_exact`), no function sections 3981/4496, no PIC 1378/4481.
* **`-mfpu=neon` is observable for libzip.** `-mfpu=vfpv3-d16` gives 3188/4493 and changes the
  sizes of seven functions: `_zip_cdir_new` 98->94, `_zip_dirent_init` 42->38,
  `_zip_dirent_torrent_normalize` 216->176, `_zip_new` 74->68, `zip_close` 2532->2528,
  `zip_fopen_index` 476->460, `zip_stat_init` 48->22. The original therefore used the app's
  `-mfpu=neon` (or an equivalent NEON-enabled FPU), not a plain VFP default.
* **`-fdata-sections` is unobservable for libzip** (4500/4500, 144/144). zlib differs here:
  adding it to zlib changes `_tr_flush_block`. The canonical build uses the default
  (`-fno-data-sections`) for both.
* **Config macros.** `HAVE_MKSTEMP`, `HAVE_STRUCT_TM_TM_ZONE`, `HAVE_FSEEKO`, `HAVE_FTELLO` are
  all required: dropping them gives 1750/4499, 3232/4499, 2886/4500, 2899/4500 respectively.
  `HAVE_LIBZ` is unused in this 0.9.3 subset (dropping it is exactly neutral).
* **`zip_file_strerror`'s `-mllvm -align-all-functions=2` is unobservable at the linked level.**
  Removing it gives 4500/4500, 144/144, 0-byte block diff. It may have been needed at an earlier
  layout, but the current binary cannot prove it.
* **Member order is required.** Swapping `zip_add` and `zip_filerange_crc` gives 4481/4500
  (`.rodata` 0x41b57). The canonical 54-member order is strictly monotone with the original
  exported-function addresses (`zip_memdup 0x18c550`, `zip_add 0x18c588`, ...,
  `zip_error_strerror 0x18f618`). `zip_err_str.c` has no `.text`, so its archive position is not
  observable from `.text`; the script places it between `zip_unchange_archive` and
  `zip_get_num_files`.

## 5. libcrypto (OpenSSL 1.1.0-line)

### 5.1 Binary witnesses

```
0018fe40 T OPENSSL_atomic_add
0018fe5c T OPENSSL_cleanse
0018feb0 T CRYPTO_memcmp
0018ff00 T _armv7_neon_probe
0018ff08 T _armv7_tick
0018ff10 T _armv8_aes_probe
0018ff18 T _armv8_sha1_probe
0018ff20 T _armv8_sha256_probe
0018ff28 T _armv8_pmull_probe
0018ff30 T OPENSSL_wipe_cpu
0018ff88 T OPENSSL_instrument_bus
0018ff90 T OPENSSL_instrument_bus2
```

`CRYPTO_memcmp` sits between `OPENSSL_cleanse` and `_armv7_neon_probe` in the `armv4cpuid.pl`
routine order; in OpenSSL 1.0.2 it is C in `cryptlib.c`. The 1.1.0-era `_armv8_*` probes are
present. `.comment` contains both `clang version 7.0.2` and
`GCC: (GNU) 4.9.x 20150123 (prerelease)`; the latter is exactly NDK r13b's GCC.

### 5.2 Whole-archive matrix

### libcrypto 1.1.0-line (whole archive)

| variant | flags changed | byte | linked | vendored | .text | .ARM.exidx | .ARM.extab | .rodata | verdict |
|---|---|---:|---:|---:|---:|---:|---:|---:|---|
| `ossl_canon` | canonical | 4500 | 4500 | 144/144 | 0x1337c8 | 0x6f70 | 0x26298 | 0x41b67 | neutral |
| `ossl_O2` | -O2 | 4500 | 4500 | 144/144 | 0x1337c8 | 0x6f70 | 0x26298 | 0x41b67 | neutral |
| `ossl_Os` | -Os | 3234 | 4494 | 95/144 | 0x133548 | 0x6f70 | 0x26298 | 0x41b67 | REGRESSION |
| `ossl_O1` | -O1 | 3234 | 4491 | 95/144 | 0x133588 | 0x6f70 | 0x26298 | 0x41b67 | REGRESSION |
| `ossl_thumb` | -mthumb | 3200 | 4477 | 81/144 | 0x1330d8 | 0x6f70 | 0x26298 | 0x41b67 | REGRESSION |
| `ossl_arm` | -marm | 4500 | 4500 | 144/144 | 0x1337c8 | 0x6f70 | 0x26298 | 0x41b67 | neutral |
| `ossl_nosp` | + -fno-stack-protector | 4500 | 4500 | 144/144 | 0x1337c8 | 0x6f70 | 0x26298 | 0x41b67 | neutral |
| `ossl_sp` | + -fstack-protector-strong | 3240 | 4498 | 101/144 | 0x133828 | 0x6f70 | 0x26298 | 0x41b67 | REGRESSION |
| `ossl_nounwind` | + -fno-unwind-tables | 4500 | 4500 | 144/144 | 0x1337c8 | 0x6f70 | 0x26298 | 0x41b67 | neutral |
| `ossl_data` | + -fdata-sections | 4498 | 4499 | 142/144 | 0x1337c8 | 0x6f70 | 0x26298 | 0x41b67 | REGRESSION |
| `ossl_nodata` | + -fno-data-sections | 4500 | 4500 | 144/144 | 0x1337c8 | 0x6f70 | 0x26298 | 0x41b67 | neutral |
| `ossl_func` | + -ffunction-sections | 4500 | 4500 | 144/144 | 0x1337c8 | 0x6f70 | 0x26298 | 0x41b67 | neutral |
| `ossl_nofunc` | + -fno-function-sections | 4500 | 4500 | 144/144 | 0x1337c8 | 0x6f70 | 0x26298 | 0x41b67 | neutral |
| `ossl_nopic` | drop -fPIC | 4500 | 4500 | 144/144 | 0x1337c8 | 0x6f70 | 0x26298 | 0x41b67 | neutral |
| `ossl_fpic` | -fpic instead of -fPIC | 4500 | 4500 | 144/144 | 0x1337c8 | 0x6f70 | 0x26298 | 0x41b67 | neutral |
| `ossl_nomandroid` | drop -mandroid | 4500 | 4500 | 144/144 | 0x1337c8 | 0x6f70 | 0x26298 | 0x41b67 | neutral |
| `ossl_nopthread` | drop -pthread | 4500 | 4500 | 144/144 | 0x1337c8 | 0x6f70 | 0x26298 | 0x41b67 | neutral |
| `ossl_armv5te` | -march=armv5te | 1368 | 4486 | 30/144 | 0x132ac8 | 0x6f70 | 0x26298 | 0x41b67 | REGRESSION |
| `ossl_armv6` | -march=armv6 | 1369 | 4487 | 31/144 | 0x132a68 | 0x6f70 | 0x26298 | 0x41b67 | REGRESSION |
| `ossl_neon` | + -mfpu=neon | 4500 | 4500 | 144/144 | 0x1337c8 | 0x6f70 | 0x26298 | 0x41b67 | neutral |
| `ossl_vfp` | -mfpu=vfpv3-d16 | 4500 | 4500 | 144/144 | 0x1337c8 | 0x6f70 | 0x26298 | 0x41b67 | neutral |
| `ossl_armmax8` | + -D__ARM_MAX_ARCH__=8 | 4500 | 4500 | 144/144 | 0x1337c8 | 0x6f70 | 0x26298 | 0x41b67 | neutral |
| `ossl_no_sha256asm` | drop -DSHA256_ASM | 1376 | 4498 | 38/144 | 0x132da8 | 0x6f70 | 0x26298 | 0x41c67 | REGRESSION |
| `ossl_no_picmacro` | drop -DOPENSSL_PIC | 4500 | 4500 | 144/144 | 0x1337c8 | 0x6f70 | 0x26298 | 0x41b67 | neutral |
| `ossl_no_threads` | drop -DOPENSSL_THREADS | 4500 | 4500 | 144/144 | 0x1337c8 | 0x6f70 | 0x26298 | 0x41b67 | neutral |
| `ossl_no_ndebug` | drop -DNDEBUG | 4500 | 4500 | 144/144 | 0x1337c8 | 0x6f70 | 0x26298 | 0x41b67 | neutral |
| `ossl_no_dso` | drop -DDSO_DLFCN | 4500 | 4500 | 144/144 | 0x1337c8 | 0x6f70 | 0x26298 | 0x41b67 | neutral |
| `ossl_no_dlfcn` | drop -DHAVE_DLFCN_H | 4500 | 4500 | 144/144 | 0x1337c8 | 0x6f70 | 0x26298 | 0x41b67 | neutral |
| `ossl_no_nodynamic` | drop -DOPENSSL_NO_DYNAMIC_ENGINE | 4500 | 4500 | 144/144 | 0x1337c8 | 0x6f70 | 0x26298 | 0x41b67 | neutral |
| `ossl_no_nodetelete` | drop -DOPENSSL_USE_NODELETE | 4500 | 4500 | 144/144 | 0x1337c8 | 0x6f70 | 0x26298 | 0x41b67 | neutral |
| `ossl_no_androidapi` | drop -D__ANDROID_API__=16 | 4500 | 4500 | 144/144 | 0x1337c8 | 0x6f70 | 0x26298 | 0x41b67 | neutral |
| `ossl_no_defines` | drop all -D macros | 1376 | 4498 | 38/144 | 0x132da8 | 0x6f70 | 0x26298 | 0x41c67 | REGRESSION |

### 5.3 What the whole-archive matrix proves

* **GCC 4.9, not clang.** The old note's clang-7 A/B made all seven `sha256.c` functions
  non-`linked_exact` (byte 1345 / linked 4488); the canonical GCC build is 144/144. The 14
  assembly functions are assembler-identical under either compiler; only the C file distinguishes.
* **`-O3` is correct, `-O2` is indistinguishable.** The `-O2` archive is byte-identical to the
  `-O3` archive (same hash `50662126...`) and the linked `.so` is identical. `-Os` (3234/4494) and
  `-O1` (3234/4491) regress. `-O3` is OpenSSL's Configure default and is the conservative pin.
* **ARM mode is required.** `-mthumb` gives 3200/4477. `-marm` is exactly neutral (GCC's default
  for `-march=armv7-a`). The `.S` files also contain `.code 32` unless `__thumb2__` is defined.
* **`-march=armv7-a` is required.** `-march=armv5te` (1368/4486) and `-march=armv6`
  (1369/4487) collapse the block.
* **No stack protector.** `-fno-stack-protector` is exactly neutral (GCC's default here);
  adding `-fstack-protector-strong` gives 3240/4498 and changes 43 vendored functions.
* **No unwind tables.** `-fno-unwind-tables` is exactly neutral; the 32 OpenSSL functions have no
  `.ARM.exidx` entry in either binary. (The 18-function `byte_exact` loss seen in the zlib/libzip
  `-fno-unwind-tables` rows is the `.rodata` shift, not an OpenSSL effect.)
* **No data sections.** `-fdata-sections` gives 4498/4499 and changes exactly `SHA224_Final`
  (4 B -> 4 B) and `SHA256` (104 B -> 100 B); `-fno-data-sections` is the neutral default.
* **`-ffunction-sections` is unobservable.** Adding it changes the `.o` section layout but the
  linked `.text` is identical (4500/4500, 0-byte block diff); `-fno-function-sections` is also
  neutral. The canonical OpenSSL build does not pass either.
* **`-fPIC` is unobservable.** Dropping `-fPIC` and `-fpic` both produce byte-identical archives
  and linked `.so` (4500/4500). The original was built with `-fPIC` per Configure, but the binary
  cannot prove it for this subset.
* **`-mandroid`, `-pthread`, `-mfpu=neon`, `-mfpu=vfpv3-d16`, `-D__ARM_MAX_ARCH__=8` are all
  neutral.** `-mfpu` is irrelevant because the `.S` files carry `.fpu neon`; `__ARM_MAX_ARCH__=7`
  is the default and 8 changes no code. `__ARM_MAX_ARCH__=6` is a compile error
  (`__ARM_MAX_ARCH__ can't be less than __ARM_ARCH__`), so the minimum is 7.
* **`-DSHA256_ASM` is required.** Dropping it makes `sha256.c` provide the C
  `sha256_block_data_order`; the linker then does not pull `sha256-armv4.o`, `.text` shrinks by
  0x5c0, and the gate falls to 1376/4498. `-DOPENSSL_PIC`, `-DOPENSSL_THREADS`, `-DNDEBUG`,
  `-DDSO_DLFCN`, `-DHAVE_DLFCN_H`, `-DOPENSSL_NO_DYNAMIC_ENGINE`, `-DOPENSSL_USE_NODELETE` and
  `-D__ANDROID_API__=16` are all neutral for this subset.
* **Archive order.** The canonical archive member order is `sha256.o, armv4cpuid.o,
  sha256-armv4.o`, and the linked `.text` order is `sha256` functions (0x18f730..0x18fe38),
  `armv4cpuid` (0x18fe40..0x18ff90), `sha256-armv4` (0x1900e0..0x191d20). This corrects the old
  `vendored_archive_provenance.md` statement that the current archive order is
  `armv4cpuid, sha256, sha256-armv4`. Because libcrypto is linked without `--whole-archive`, the
  archive member order itself is not observable; only the pull order (which matches) is.

### 5.4 Per-file matrix

### libcrypto per-file

| variant | flags changed | byte | linked | vendored | .text | .ARM.exidx | .ARM.extab | .rodata | verdict |
|---|---|---:|---:|---:|---:|---:|---:|---:|---|
| `ossl_pf_sha_thumb` | sha256.c + -mthumb | 3214 | 4491 | 95/144 | 0x133538 | 0x6f70 | 0x26298 | 0x41b67 | REGRESSION |
| `ossl_pf_sha_O2` | sha256.c -O2 | 4500 | 4500 | 144/144 | 0x1337c8 | 0x6f70 | 0x26298 | 0x41b67 | neutral |
| `ossl_pf_sha_sp` | sha256.c + -fstack-protector-strong | 3240 | 4498 | 101/144 | 0x133828 | 0x6f70 | 0x26298 | 0x41b67 | REGRESSION |
| `ossl_pf_sha_data` | sha256.c + -fdata-sections | 4498 | 4499 | 142/144 | 0x1337c8 | 0x6f70 | 0x26298 | 0x41b67 | REGRESSION |
| `ossl_pf_sha_nounwind` | sha256.c + -fno-unwind-tables | 4500 | 4500 | 144/144 | 0x1337c8 | 0x6f70 | 0x26298 | 0x41b67 | neutral |
| `ossl_pf_sha_nopic` | sha256.c drop -fPIC | 4500 | 4500 | 144/144 | 0x1337c8 | 0x6f70 | 0x26298 | 0x41b67 | neutral |
| `ossl_pf_sha_armv5` | sha256.c -march=armv5te | 4496 | 4496 | 140/144 | 0x1337c8 | 0x6f70 | 0x26298 | 0x41b67 | REGRESSION |
| `ossl_pf_sha_vfp` | sha256.c -mfpu=vfpv3-d16 | 4500 | 4500 | 144/144 | 0x1337c8 | 0x6f70 | 0x26298 | 0x41b67 | neutral |
| `ossl_pf_cpuid_thumb` | armv4cpuid.S + -mthumb | 3231 | 4488 | 92/144 | 0x133768 | 0x6f70 | 0x26298 | 0x41b67 | REGRESSION |
| `ossl_pf_cpuid_armv5` | armv4cpuid.S -march=armv5te | 1370 | 4492 | 32/144 | 0x133788 | 0x6f70 | 0x26298 | 0x41b67 | REGRESSION |
| `ossl_pf_cpuid_nopic` | armv4cpuid.S drop -fPIC | 4500 | 4500 | 144/144 | 0x1337c8 | 0x6f70 | 0x26298 | 0x41b67 | neutral |
| `ossl_pf_shaasm_thumb` | sha256-armv4.S + -mthumb | 3241 | 4498 | 102/144 | 0x1333c8 | 0x6f70 | 0x26298 | 0x41b67 | REGRESSION |
| `ossl_pf_shaasm_armv5` | sha256-armv4.S -march=armv5te | 1376 | 4498 | 38/144 | 0x132b08 | 0x6f70 | 0x26298 | 0x41b67 | REGRESSION |
| `ossl_pf_shaasm_vfp` | sha256-armv4.S -mfpu=vfpv3-d16 | 4500 | 4500 | 144/144 | 0x1337c8 | 0x6f70 | 0x26298 | 0x41b67 | neutral |
| `ossl_pf_shaasm_amax8` | sha256-armv4.S -D__ARM_MAX_ARCH__=8 | 4500 | 4500 | 144/144 | 0x1337c8 | 0x6f70 | 0x26298 | 0x41b67 | neutral |

### 5.5 What the per-file matrix proves

* **`sha256.c`**: ARM mode is required (`-mthumb`: 3214/4491, sizes `SHA256_Update` 272->160,
  `SHA224_Init` 132->102, ...). `-O2` is byte-identical to `-O3` (neutral). Adding
  `-fstack-protector-strong` gives 3240/4498. Adding `-fdata-sections` gives 4498/4499 and
  changes exactly `SHA224_Final` and `SHA256` (104->100 B). `-fno-unwind-tables`, `-fno-pic` and
  `-mfpu=vfpv3-d16` are neutral. `-march=armv5te` gives 4496/4496 and changes exactly
  `SHA224_Init`, `SHA256_Init`, `SHA256_Update`, `SHA256_Final`.
* **`armv4cpuid.S`**: `-mthumb` gives 3231/4488; `-march=armv5te` gives 1370/4492 (it also
  changes `.data`/`.got` sizes); `-fno-pic` is neutral.
* **`sha256-armv4.S`**: `-mthumb` gives 3241/4498; `-march=armv5te` gives 1376/4498;
  `-mfpu=vfpv3-d16` is neutral (the file's `.fpu neon` directive wins); `-D__ARM_MAX_ARCH__=8`
  is neutral; `-D__ARM_MAX_ARCH__=6` is a compile error.

### 5.6 OpenSSL patch letter: 1.1.0l vs 1.1.1w

The exact patch letter is **not observable** for this subset. Built the same three objects from
a pristine `openssl-1.1.1w` tarball with the same GCC 4.9 and flags; the `.text` sections are
byte-identical:

```
sha256.c       1804 B, 0 differing bytes
armv4cpuid.S    384 B, 0 differing bytes
sha256-armv4.S 7520 B, 0 differing bytes
```

The source differences are cosmetic (`return (md)` vs `return md`, comment years, include-guard
spelling). `1.1.0l` is a conservative pin (last 1.1.0 release), not a binary-derived fact.

## 6. Proven vs disproven vs unobservable

### Positively proven

* zlib is **1.2.3**; libzip is **0.9.3**; libcrypto is the **1.1.0 line**.
* zlib/libzip were compiled by **clang 7.0.2** with
  `-Oz -g -fstack-protector-strong -ffunction-sections -funwind-tables -mthumb -fPIC
  -march=armv7-a`; libzip additionally used **`-mfpu=neon`**.
* libcrypto's `sha256.c` was compiled by **GCC 4.9.x** (r13b) with `-O3 -march=armv7-a` in
  **ARM mode**, with **no stack protector**, **no unwind tables**, **no data sections**, and
  **`-DSHA256_ASM`**.
* `HAVE_MKSTEMP`, `HAVE_STRUCT_TM_TM_ZONE`, `HAVE_FSEEKO`, `HAVE_FTELLO` are set for libzip.
* The zlib/libzip archive member orders are the canonical ones; the OpenSSL pull order is
  `sha256, armv4cpuid, sha256-armv4`.
* The entire vendored `.text` block, `.rodata` and `.data` are byte-identical to the original.

### Disproven (a controlled A/B regresses the gate)

* zlib/libzip: `-Os`, `-O2`, `-O3`, `-marm`, no `-fstack-protector-strong`, no `-g`, no
  `-ffunction-sections`, no `-funwind-tables`, `-fno-pic`, `-march=armv5te`/`armv6`.
* zlib: `-fdata-sections`, `-DUNALIGNED_OK`, `-DFASTEST`, `-DNO_DIVIDE`, `-DINFLATE_STRICT`,
  `-DDYNAMIC_CRC_TABLE`, `-DNO_GZIP`, `-DDEBUG`, swapped member order.
* libzip: `-mfpu=vfpv3-d16`, no `HAVE_MKSTEMP`, no `HAVE_STRUCT_TM_TM_ZONE`, no `HAVE_FSEEKO`,
  no `HAVE_FTELLO`, swapped member order.
* libcrypto: `-Os`, `-O1`, `-mthumb`, `-fstack-protector-strong`, `-fdata-sections`,
  `-march=armv5te`/`armv6`, no `-DSHA256_ASM`; per-file `sha256.c -mthumb`,
  `-fstack-protector-strong`, `-fdata-sections`, `-march=armv5te`; `armv4cpuid.S -mthumb`,
  `-march=armv5te`; `sha256-armv4.S -mthumb`, `-march=armv5te`.

### Unobservable (byte-identical output; the binary cannot decide)

* OpenSSL: `-O2` vs `-O3`, `-marm` vs default, `-fno-stack-protector` vs default,
  `-fno-unwind-tables` vs default, `-fno-data-sections` vs default, `-ffunction-sections` vs
  default, `-fPIC` vs `-fpic` vs no PIC, `-mandroid`, `-pthread`, `-mfpu=neon` vs
  `-mfpu=vfpv3-d16`, `-D__ARM_MAX_ARCH__=8`, and every `-D` macro except `SHA256_ASM`
  (`OPENSSL_PIC`, `OPENSSL_THREADS`, `NDEBUG`, `DSO_DLFCN`, `HAVE_DLFCN_H`,
  `OPENSSL_NO_DYNAMIC_ENGINE`, `OPENSSL_USE_NODELETE`, `__ANDROID_API__=16`).
* zlib: `-mfpu=neon` vs `-mfpu=vfpv3-d16`, `-fpic` vs `-fPIC`, `-fno-data-sections` vs default.
* libzip: `-fdata-sections`, `HAVE_LIBZ`, `zip_file_strerror -mllvm -align-all-functions=2`.
* OpenSSL exact patch letter (1.1.0l and 1.1.1w produce identical `.text` in this pass; the old
  note reports the same for 1.1.0a..1.1.0l).
* OpenSSL archive member order (non-whole archive; only the pull order is observable).
* libzip `zip_err_str.o`-style data-only member positions (`zip_err_str.o` has no `.text`).

### Unknown / not re-tested

* The exact GCC NDK revision beyond r13b: the build script records that r11c/r13b reproduce
  `SHA256_Update` and r16b/r17c do not. This pass used r13b and did not re-run r11c/r16b/r17c.
* Whether the original app's build system passed any additional `-D` that happens to be
  codegen-neutral for this subset (e.g. `-DOPENSSL_NO_*`); the binary cannot tell.

## 7. Confidence table

| claim | hard evidence | confidence |
|---|---|---|
| zlib 1.2.3 | `zlibVersion` -> "1.2.3"; copyright strings; 47-symbol set; no 1.2.4+ symbols | **proven** |
| libzip 0.9.3 | `_zip_nerr_str=24`; pre-0.11 API; error-string set | **proven** |
| libcrypto 1.1.0-line | `CRYPTO_memcmp` in `armv4cpuid` order; 1.1.0 probes; `.comment` GCC 4.9.x | **proven** |
| exact OpenSSL patch letter | 1.1.0l and 1.1.1w `.text` identical (1804/384/7520 B, 0 diffs) | unobservable |
| zlib/libzip compiler+flags | canonical block byte-identical; every removal/change regresses | **proven** |
| libzip `-mfpu=neon` | `-mfpu=vfpv3-d16` changes 7 function sizes; 3188/4493 | **proven** |
| zlib `-mfpu=neon` | `-mfpu=vfpv3-d16` exactly neutral | unobservable |
| libzip config macros | each removal regresses (1750/4499, 3232/4499, 2886/4500, 2899/4500) | **proven** |
| libzip `HAVE_LIBZ` | removal exactly neutral | unobservable |
| OpenSSL `-O3` | `-O2` archive byte-identical; `-Os`/`-O1` regress | **proven** (`-O3` is safe; `-O2` indistinguishable) |
| OpenSSL ARM mode | `-mthumb` regresses (3200/4477); `-marm` neutral | **proven** |
| OpenSSL `-march=armv7-a` | armv5te/armv6 regress; 4 SHA C functions change | **proven** |
| OpenSSL no stack protector | adding `-fstack-protector-strong` regresses (3240/4498); `-fno-...` neutral | **proven** |
| OpenSSL no unwind tables | `-fno-unwind-tables` neutral; 32 functions have no exidx entry in both | **proven** |
| OpenSSL no data sections | adding `-fdata-sections` regresses (4498/4499; `SHA224_Final`, `SHA256`) | **proven** |
| OpenSSL `-DSHA256_ASM` | dropping it selects the C block fn and regresses (1376/4498) | **proven** |
| OpenSSL other `-D` macros | each removal exactly neutral | unobservable |
| zlib/libzip member order | monotone with original addresses; reorder regresses (4493/4500, 4481/4500) | **proven** |
| OpenSSL pull order | `.text` order `sha256, armv4cpuid, sha256-armv4` matches; archive member order unobservable | **proven** (pull order) / unobservable (member order) |
| tarball provenance | SHA-256 hashes match upstream; trees pristine except generated files | **proven** for fetched tarballs |

## 8. Strict improvement?

**No.** The canonical recipe already produces a byte-identical vendored `.text` block
(45,868/45,868 bytes), byte-identical `.rodata`/`.data`, and 144/144 `byte_exact` vendored
functions. No tested variant exceeds 4500/4500 global `byte_exact`; every non-canonical codegen
choice either ties at 4500 (unobservable) or regresses. Therefore:

* no `src/` change is justified by this pass;
* no `third_party/build_match_lib.cmake` change is justified;
* no baseline update is needed.

The only candidate "cleanup" changes are neutral (e.g. removing `zip_file_strerror`'s
`-mllvm -align-all-functions=2`, or the unobservable OpenSSL `-D` macros). They are not strict
improvements and were left alone to avoid churn.

## 9. Reproduction summary

```sh
# canonical
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
cmake --build cmake-build-match --target drift -j6

# rebuild the three archives in place (deterministic; same hashes as above)
NDK=$PWD/.cache/ndk/android-ndk-r18b; HOST=linux-x86_64
DEFS="-DGOF2_NDK_ROOT=$NDK \
 -DGOF2_NDK_CLANG=$NDK/toolchains/llvm/prebuilt/$HOST/bin/clang \
 -DGOF2_NDK_AR=$NDK/toolchains/llvm/prebuilt/$HOST/bin/llvm-ar \
 -DGOF2_NDK_GCC_BINDIR=$NDK/toolchains/arm-linux-androideabi-4.9/prebuilt/$HOST/arm-linux-androideabi/bin \
 -DTP_DIR=$PWD/third_party"
for lib in zlib libzip openssl; do
  cmake $DEFS -DGOF2_LIB=$lib -P third_party/build_match_lib.cmake
done
sha256sum third_party/zlib/libz_gof2.a third_party/libzip/libzip_gof2.a \
          third_party/openssl/libcrypto_gof2.a
```

The variant harness is `/dev/shm/w5212/drive.py` (patch one flag token list, build one archive),
`/dev/shm/w5212/link_test.py` (swap, relink, compare the 45,868-byte block and section sizes) and
`/dev/shm/w5212/verify_one.py` (full `verify.py` run, per-function byte/linked and vendored
counts). The raw logs are `/dev/shm/w5212/linklog*.txt` and `/dev/shm/w5212/verifylog*.jsonl`.
