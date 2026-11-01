# `.data.rel.ro` content (w3457): 667 bytes are relocation-target mirrors

Status: research note + one landed build-flag change. Branch `wave87-w3457`, based on
master `7aab8bdc` (`docs: campaign notes for bss/dynsym wave`). Original binary:
`../_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a, NDK r18b clang 7.0.2,
`-Oz`, image base 0x10000).

## TL;DR

`.data.rel.ro` is already size- and base-exact (`0x2fe8 @ 0x20cbb0`). Its raw bytes
still differ in **667 bytes / 611 words**, first at `0x20d9c0`, but every differing
word is an `R_ARM_RELATIVE` pointer into another section:

| owner input section | bytes | words | target section | target delta (ours-orig) |
|---|---:|---:|---|---:|
| libc++abi `m003_cxa_demangle.o` | 581 | 536 | `.text` | `-0x24` |
| libc++abi `m014_private_typeinfo.o` | 65 | 63 | `.text` | `-0x20` |
| `Level.cpp` | 18 | 9 | `.rodata` | `+0x3cc` |
| `ModStation.cpp` | 1 | 1 | `.bss` | `-0x10` |
| `Radio.cpp` | 1 | 1 | `.bss` | `+0x4` |
| `DialogueWindow.cpp` | 1 | 1 | `.bss` | `-0xc` |

Totals: **646 B of `.text` pointers, 18 B of `.rodata` pointers, 3 B of `.bss`
pointers**. There is no `.data.rel.ro` content/order defect to fix directly. The
section will become exact when the pointed-to sections become exact at those
addresses (the libc++abi `.text` shift, the `Level.cpp` `.rodata` block, and the
known `.bss` internal order).

One independent strict improvement was found while checking the libzip `.text`
frontier: `zip_file_strerror` is 4-byte aligned in the original but was 2-byte
aligned in our build. Compiling only that one libzip unit with
`-mllvm -align-all-functions=2` reproduces the original placement without changing
any section size or base. Gate: **byte_exact 4263 -> 4264, linked_exact 4496
unchanged, `RATCHET PASS`**. The only verify row that changes is
`zip_file_strerror` (`byte False -> True`); `lint` is `CLEAN ✓` and `drift` is
`0 field(s) across 0 class(es)`.

## 1. Baseline and gate

```
cmake --preset default
cmake --build cmake-build-match --target check -j8
```

Baseline at HEAD `7aab8bdc` (also visible in `git show HEAD:report.json`):

```
verify: byte 4263  linked 4496  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

The prompt's "byte 4182 / linked 4495" is stale for this branch. The current
baseline is 4263 / 4496.

All relevant ELF sections are size- and base-exact before and after the landed
change:

```
section       original              ours                  result
.text         0x069440 0x1337c8     0x069440 0x1337c8     exact
.ARM.exidx    0x19cc08 0x006f70     0x19cc08 0x006f70     exact
.ARM.extab    0x1a3b78 0x026298     0x1a3b78 0x026298     exact
.rodata       0x1c9e10 0x041b67     0x1c9e10 0x041b67     exact
.data.rel.ro  0x20cbb0 0x002fe8     0x20cbb0 0x002fe8     exact
.got          0x20fcf4 0x00430c     0x20fcf4 0x00430c     exact
.data         0x214000 0x00296c     0x214000 0x00296c     exact
.bss          0x216970 0x0069ac     0x216970 0x0069ac     exact
.dynsym       0x0001f0 0x015ca0     0x0001f0 0x015ca0     exact
.dynstr       0x015e90 0x02e318     0x015e90 0x02e318     exact
.dynamic      0x20fba4 0x000148     0x20fba4 0x000148     exact
```

## 2. Measurement method

The gate's `data: exact 843 of 843 exported objects` is relocation-normalized, not
a raw byte check. To compare raw `.data.rel.ro` bytes and attribute each differing
word, a scratch relink was made with the build's own gold command, adding only
`-Map=/tmp/w3457/data.map` and replacing the final stripped output with an
unstripped `/tmp/w3457/unstripped.so` (no `strip`). The resulting script is
`/tmp/w3457/relink_map.sh`; its command is the build's `ld.gold -shared` command
with those two edits.

The committed attribution script is:

```
python3 _work/reconstructions/data_rel_ro_content_w3457.py \
  --orig ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --map /tmp/w3457/data.map \
  --unstripped /tmp/w3457/unstripped.so
```

It prints the raw byte/word count, relocation-type pairs, and the owner/target
breakdown. Current output begins:

```
.data.rel.ro base=0x20cbb0 size=0x2fe8 diff_bytes=667 words=611 first=0x20d9c0
reloc type pairs: {(23, 23): 611}
```

`23` is `R_ARM_RELATIVE`. There are no `ABS32`/`GLOB_DAT` differences in the
differing words. The section size and base are identical, so the words are aligned
one-for-one.

## 3. What the 667 bytes are

### 3.1 libc++abi `.text` vtables: 646 B

The 599 differing `.text`-target words are libc++abi vtables. The owner split is:

```
581 B  libc++abi.a(m003_cxa_demangle.o)
 65 B  libc++abi.a(m014_private_typeinfo.o)
```

`m003_cxa_demangle.o` owns the `_ZTVN12_GLOBAL__N_1...` vtables (anonymous-namespace
`Node`/`FunctionType`/`FunctionEncoding`/`NestedName`/... classes). Every target in
that group is shifted by `-0x24` (`-36`). `m014_private_typeinfo.o` owns
`_ZTVN10__cxxabiv1...` (`__shim_type_info`, `__class_type_info`,
`__vmi_class_type_info`, `__pointer_type_info`, ...), shifted by `-0x20`
(`-32`). Examples from the owner dump:

```
0x20f8b0  0x19a715 -> 0x19a6f1  FunctionType::hasRHSComponentSlow(...)
0x20f8b4  0x1959c9 -> 0x1959a5  Node::hasArraySlow(...)
0x20eb98  0x192fd5 -> 0x192fb5  __vmi_class_type_info::~__vmi_class_type_info()
0x20eb9c  0x19301f -> 0x192fff  __class_type_info::~__class_type_info()
```

The corresponding exported libc++abi function addresses (`nm -D`; the Thumb bit is
included, subtract 1 for the code address):

```
symbol              original    ours        delta
__cxa_end_cleanup   0x191d21    0x191d01    -0x20
__dynamic_cast      0x19362d    0x19360d    -0x20
__cxa_demangle      0x194031    0x19400d    -0x24
```

All exported libc++abi function sizes are equal. The extra 4 bytes on
`__cxa_demangle` is a gap/alignment difference, not a size mismatch. The 32-byte
shift is cumulative from the preceding libzip/libcrypto block: the original has
2-byte inter-function alignment gaps where our build has none. The deltas step
`-2/-4/.../-32` across libzip, then libcrypto starts at `-32`, then libc++abi
starts at `-32`.

### 3.2 `Level.cpp` `.rodata` switch table: 18 B

`0x20d9c0..0x20d9e0` is the 9-pointer `Level::createScene` switch table. Original
and ours are the same nine targets, uniformly shifted by `+0x3cc`:

```
orig: 0x1fd740 0x1fd758 0x1fd740 0x1fd7c4 0x1fd740 0x1fd740 0x1fd740 0x1fd740 0x1fd7e8
ours: 0x1fdb0c 0x1fdb24 0x1fdb0c 0x1fdb90 0x1fdb0c 0x1fdb0c 0x1fdb0c 0x1fdb0c 0x1fdbb4
```

This is downstream of the known `Level.cpp` `.rodata` block, not a `.data.rel.ro`
ordering problem.

### 3.3 `.bss` pointers: 3 B

```
0x20da34  gPirateGreetingFaceB  0x217bf8 -> 0x217be8  (-0x10)
0x20df04  CHAR_BRENT            0x218388 -> 0x21838c  (+0x4)
0x20e29c  g_dw_playerFace       0x2183c0 -> 0x2183b4  (-0xc)
```

These are the known `.bss` internal-order issue. The `.data.rel.ro` slots are
correct; only the pointed-to `.bss` addresses differ.

## 4. libzip alignment experiment

### 4.1 Baseline placement

`zip_file_strerror` is a 6-byte function. Original placement:

```
orig zip_file_strerror code address 0x18c6c8, size 6
orig .ARM.exidx sequence:
  CANT @0x18c6c6
  INL  @0x18c6c8   <- zip_file_strerror
  CANT @0x18c6ce
  zip_fopen_index @0x18c6d0
```

Baseline ours placed it at code address `0x18c6c6` (2 mod 4), size 6. The original
is 4-byte aligned. Compiling only `zip_file_strerror.c` with
`-mllvm -align-all-functions=2` sets the section alignment to 4 and places it at
`0x18c6c8`. The existing 4-byte gap after it is consumed, so `.text` stays
`0x1337c8`.

The landed change in `third_party/build_match_lib.cmake` is:

```cmake
foreach(_n ${units})
    set(_extra "-Oz;-g;-fstack-protector-strong;-I.;-mthumb")
    if(_n STREQUAL "zip_file_strerror")
        list(APPEND _extra "-mllvm;-align-all-functions=2")
    endif()
    compile("${OBJDIR}/${_n}.o" "${SRC}/lib" "${_extra}" "${SRC}/lib/${_n}.c")
    list(APPEND objs "${OBJDIR}/${_n}.o")
endforeach()
```

Rebuild and gate:

```
unlink third_party/libzip/libzip_gof2.a
cmake --build cmake-build-match --target gof2_libs -j8
cmake --build cmake-build-match --target link_so -j8
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8
cmake --build cmake-build-match --target lint -j8
cmake --build cmake-build-match --target verify -j8
```

Result:

```
verify: byte 4264  linked 4496  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
GAIN  byte_exact 4263 -> 4264 (lock with --update-baseline)
RATCHET PASS
```

`git show HEAD:report.json` vs the new `report.json` shows exactly one changed row:

```
zip_file_strerror: byte False -> True, linked True, osize 6, usize 6
```

The improvement was locked with:

```
python3 tools/ratchet.py --update-baseline
```

which updated `tools/verify_baseline.json` to `byte_exact: 4264`,
`linked_exact: 4496`. No regression was absorbed.

### 4.2 Negative experiments (not kept)

Both experiments used the same rebuild/gate sequence, changing only the set of
units that receive `-mllvm -align-all-functions=2`.

| experiment | byte_exact | linked_exact | `.text` | libc++abi result | gate |
|---|---:|---:|---|---|---|
| baseline | 4263 | 4496 | `0x1337c8` | `__cxa_end_cleanup -0x20`, `__dynamic_cast -0x20`, `__cxa_demangle -0x24` | PASS |
| **single `zip_file_strerror`** | **4264** | **4496** | **`0x1337c8`** | unchanged | **PASS** |
| all libzip units | 3165 | 4496 | `0x133808` (`+0x40`) | `__cxa_end_cleanup +0x20`, `__dynamic_cast +0x20`, `__cxa_demangle +0x1c` | FAIL |
| 19 candidate units | 3190 | 4496 | `0x1337e8` (`+0x20`) | `__cxa_end_cleanup` exact, `__cxa_demangle -0x4`; `zip_fclose -0x8`, `zip_unchange_all -0x8` | FAIL |

The 19-unit list was:

```
zip_file_strerror zip_get_archive_comment zip_source_file zip_set_name
zip_file_error_get zip_new zip_source_free zip_get_num_files zip_source_filep
zip_entry_new zip_unchange_data zip_rename zip_source_function zip_delete
zip_set_file_comment zip_stat zip_get_file_comment zip_fclose zip_unchange_all
```

The original has 2-byte alignment gaps only at selected functions; aligning every
function whose baseline start is 2 mod 4 over-adds gaps and changes `.text` size.
The original gap sequence must be matched, not inferred from a single modulo test.
The single-unit fix is safe because it consumes an existing gap and leaves `.text`
size exact.

## 5. Conclusion

**Proven**

- Raw `.data.rel.ro` diff is **667 bytes / 611 words**, first at `0x20d9c0`.
- All 611 differing words are `R_ARM_RELATIVE` in both binaries; no
  `ABS32`/`GLOB_DAT` differences.
- The 667 bytes are 646 B of libc++abi `.text` vtable pointers, 18 B of the
  `Level::createScene` `.rodata` table, and 3 B of `.bss` pointers.
- `.data.rel.ro` size/base are exact; `.rel.dyn` size is exact.
- `data: exact 843 of 843` is relocation-normalized, not raw-byte equality.
- Original `zip_file_strerror` is at code address `0x18c6c8` (4-byte aligned).
- Compiling only `zip_file_strerror.c` with `-mllvm -align-all-functions=2`
  reproduces that placement and raises byte_exact 4263 -> 4264 with `.text` size
  unchanged and all section bases exact.
- The only verify row that changes is `zip_file_strerror` (`byte False -> True`).
- `lint` is `CLEAN ✓`; `drift` is `0 field(s) across 0 class(es)`.

**Hypothesis / still open**

- The 32-byte libc++abi start shift is the cumulative sum of original 2-byte
  inter-function alignment gaps in libzip/libcrypto. The step pattern is measured,
  but the exact original compile flag/mechanism is not proven.
- The 4-byte `__cxa_demangle` shift is a libc++abi inter-function gap/alignment,
  not a size mismatch.
- A larger safe libzip alignment subset may exist. The 19-unit list is not safe
  because it over-aligns and changes `.text` size; a gap-sequence DP/incremental
  A/B could find a larger subset, but it is not required for this pass.

## 6. Files

- `_work/reconstructions/data_rel_ro_content_w3457.md` (this note)
- `_work/reconstructions/data_rel_ro_content_w3457.py` (raw diff + owner attribution)
- `third_party/build_match_lib.cmake` (single-unit `zip_file_strerror` alignment)
- `docs/VALIDATION.md`, `docs/IDENTICALITY.md` (current counts updated)
- `tools/verify_baseline.json`, `report.json` (locked 4264 / 4496)
