# Whole-file identity audit at the new layout (w3274)

**Worker:** research-w3274
**Repo:** `/home/debian/GalaxyOnFire2/wt/w3274`
**Branch:** `research-w3274` (based on `master` HEAD `1c4fd149`)
**Date:** 2026-11-01
**Deliverable:** this note only. `src/` and `CMakeLists.txt` are clean in the committed tree.
All experiments were made in the worktree and reverted before commit.

Original: `../_work/bins/android_2.0.16_libgof2hdaa.so` (stripped, `.dynsym` only).
Ours: `cmake-build-match/libgof2hdaa.so` after the standard gate build.

Prior audits used: `_work/reconstructions/dynsym_order_audit.md` (w3307, `.dynsym`
sequence matching), `_work/reconstructions/data_layout_audit.md` (w3305, data-section object
attribution), and `_work/reconstructions/local_gap_audit.md` (w3306, `.text` delta walk).
Their gate counts are stale for this HEAD (1458 / 1415), but the methods are unchanged; every
number below was re-measured at `1c4fd149`.

## TL;DR

* Whole file: original **2,187,328 B**, ours **2,187,384 B** (**+56 B**).
  sha256 original `d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483`;
  ours `7e07d4e243df9fee2a4a182e1e46cdbc22786de3fb52bc7e9d6896a7b5a812cf`.
* Gate at this HEAD:

```
compared 4523  avg 99.89%  byte_exact 1513  linked_exact 4492  fuzzy_perfect 4494
stub_zero_size 0  orig_zero_size 1  missing 0 (wrong_type 0  absent 0)  extra 0
verify: byte 1513  linked 4492  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

* `.text` is **-24 B** (`0x1337b0` vs `0x1337c8`), `.ARM.exidx` **+24 B**,
  `.ARM.extab` **-8 B**, `.data.rel.ro` **+32 B**, `.bss` **+44 B**, and there is one extra
  **`.aezdata`** section (8 B). `.rodata` is **exact in address and size** and its printable
  blob multiset is now **identical**; the remaining `.rodata` difference is ordering only.
* The largest remaining *byte-count* gaps are `.text` (1,105,634 raw bytes, mostly
  address-dependent words), `.dynstr` (177,258 differing bytes), `.ARM.extab` (143,496),
  `.rodata` (119,730), `.dynsym` (29,176), `.ARM.exidx` (15,171), `.hash` (8,788), `.rel.dyn`
  (6,748), `.rel.plt` (5,129), `.data.rel.ro` (3,102), `.data` (304), `.shstrtab` (259).
* The single biggest remaining **address-drift source** is still the plain-`.text` grouping in
  `tools/gen_section_order.py` (`PLAIN_TEXT_LEAD`): it inserts 9,804 B before `zip_fopen` and
  causes the `-10,168 / +7,544 / -7,904 / +816` oscillation. Its net effect is only +92 B.
* The biggest remaining **app body shortfall** is `HangarWindow::initialize` (**-68 B**), not
  `Generator::createMission` (-4 B). The biggest remaining **app extra helper** is
  `levelTextAnchor` (+40 B); the 2,934-byte `mtwTextLayoutAnchor` is gone (only a 2-byte empty
  function remains, +4 B with alignment).

---

## 1. Section-by-section comparison

Method: parse both ELF32 files, compare section bytes at the same section-relative offset.
For sections whose sizes differ, the differing-byte count is over the common prefix; the size
delta is shown separately. `first` is the first differing section-relative offset.

| section | orig addr / size | ours addr / size | size Δ | first | differing bytes | classification |
|---|---|---:|---:|---:|---:|---|
| `.note.android.ident` | `0x134` / `0x98` | `0x134` / `0x98` | 0 | — | 0 | exact |
| `.note.gnu.build-id` | `0x1cc` / `0x24` | `0x1cc` / `0x24` | 0 | `0x10` | 20 | content (build-id) |
| `.dynsym` | `0x1f0` / `0x15ca0` | `0x1f0` / `0x15ca0` | 0 | `0x20` | 29,176 | ordering + 4 attr/size diffs |
| `.dynstr` | `0x15e90` / `0x2e318` | `0x15e90` / `0x2e318` | 0 | `0x1` | 177,258 | ordering (same string multiset) |
| `.hash` | `0x441a8` / `0x973c` | `0x441a8` / `0x973c` | 0 | `0x8` | 8,788 | ordering (dynsym order) |
| `.gnu.version` | `0x4d8e4` / `0x2b94` | `0x4d8e4` / `0x2b94` | 0 | `0x6` | 338 | ordering |
| `.gnu.version_d` | `0x50478` / `0x1c` | `0x50478` / `0x1c` | 0 | `0x14` | 1 | ordering (dynstr offset of `libgof2hdaa.so`) |
| `.gnu.version_r` | `0x50494` / `0x40` | `0x50494` / `0x40` | 0 | `0x4` | 5 | ordering (dynstr offsets of `libm.so`) |
| `.rel.dyn` | `0x504d4` / `0x7048` | `0x504d4` / `0x7048` | 0 | `0x0` | 6,748 | ordering/address (same multiset) |
| `.rel.plt` | `0x5751c` / `0x72d0` | `0x5751c` / `0x72d0` | 0 | `0x5` | 5,129 | ordering (symbol index remap) |
| `.plt` | `0x5e7ec` / `0xac4c` | `0x5e7ec` / `0xac4c` | 0 | — | 0 | exact |
| `.text` | `0x69440` / `0x1337c8` | `0x69440` / `0x1337b0` | **-24** | `0x12` | 1,105,634 | relocation/code (branch/literal words) |
| `.ARM.exidx` | `0x19cc08` / `0x6f70` | `0x19cbf0` / `0x6f88` | **+24** | `0x0` | 15,171 | address/ordering + CANT sentinels |
| `.ARM.extab` | `0x1a3b78` / `0x26298` | `0x1a3b78` / `0x26290` | **-8** | `0x298` | 143,496 | ordering/address + record lengths |
| `.rodata` | `0x1c9e10` / `0x41b67` | `0x1c9e10` / `0x41b67` | 0 | `0xc2f` | 119,730 | ordering (blob multiset identical) |
| `.fini_array` | `0x20cba0` / `0x8` | `0x20cb80` / `0x8` | 0 | — | 0 | exact bytes; address -0x20 |
| `.data.rel.ro` | `0x20cbb0` / `0x2fe8` | `0x20cb90` / `0x3008` | **+32** | `0xc08` | 3,102 | relocation addend + layout (see §4) |
| `.init_array` | `0x20fb98` / `0xc` | `0x20fb98` / `0xc` | 0 | — | 0 | exact |
| `.dynamic` | `0x20fba4` / `0x148` | `0x20fba4` / `0x148` | 0 | `0xa4` | 5 | ordering/address (dynstr offsets + `fini_array`) |
| `.got` | `0x20fcf4` / `0x430c` | `0x20fcf4` / `0x430c` | 0 | — | 0 | exact |
| `.data` | `0x214000` / `0x296c` | `0x214000` / `0x296c` | 0 | `0x8d0` | 304 | relocation addend (same relocation set) |
| `.bss` | `0x216970` / `0x69ac` | `0x216980` / `0x69d8` | **+44** | — | NOBITS | ordering/local statics (see §4) |
| `.comment` | `0` / `0x12f` | `0` / `0x12f` | 0 | — | 0 | exact |
| `.note.gnu.gold-version` | `0` / `0x1c` | `0` / `0x1c` | 0 | — | 0 | exact |
| `.ARM.attributes` | `0` / `0x38` | `0` / `0x38` | 0 | — | 0 | exact |
| `.shstrtab` | `0` / `0x117` | `0` / `0x120` | **+9** | `0x2` | 259 | content/ordering (extra `.aezdata`) |
| `.aezdata` | missing | `0x21696c` / `0x8` | **+8** | — | 8 | extra section (content `.aez\0\0\0\0`) |

### 1.000 `.shstrtab` and `.aezdata`

`.aezdata` is an extra 8-byte `WA` section at `0x21696c` containing `.aez\0\0\0\0`. It is
emitted by `src/engine/file/AEFile.cpp:7` (`#pragma clang section data=".aezdata"` around
`static char pakExtension[8] = ".aez"`). The original has the `.aez` string only in
`.rodata` (file offset 1,877,115). The extra section name is exactly the +9 bytes in
`.shstrtab`. The 259 differing `.shstrtab` bytes are **not** just shifted offsets: the
original's section-name string table is in a different order from ours (original starts
`.init_array`, `.fini_array`, `.ARM.exidx`, `.text`, ...; ours starts `.shstrtab`,
`.note.android.ident`, `.note.gnu.build-id`, `.dynsym`, ...). So the diff is the reordered
section-name strings plus the extra `.aezdata` string; the section headers' `sh_name`
values differ accordingly.

### 1.00 `.rodata` content is exact as a multiset

Splitting both `.rodata` sections into maximal printable blobs (`[\x09\x0a\x0d\x20-\x7e]{4,}`)
gives **2,779 blobs / 200,846 bytes** on both sides, and the blob multiset difference is
**empty**. The 119,730 differing bytes are therefore pure ordering. The first difference at
`0xc2f` is the merged string pool: original has `BloomShader`, ours has `texSize` /
`s_texture_bloom` earlier.

### 1.0 Relocation-section detail

* `.rel.dyn`: both sides have 3,593 entries with the **same multiset** of
  `(r_type, symbol name)` (1,817 `R_ARM_ABS32`, 1,162 `R_ARM_RELATIVE`, 614
  `R_ARM_GLOB_DAT`), but only **211 / 3,593** entries have the same offset at the same
  index. The differences are ordering plus the `.text`/`.data.rel.ro` address shifts.
* `.rel.plt`: both sides have 3,674 entries and **all 3,674 offsets are identical at the
  same index**; only the symbol indices differ (`.dynsym` order). This section is pure
  ordering.

### 1.1 ELF header and program headers

ELF header (52 B): **3 differing bytes**:

| offset | original | ours | field |
|---:|---:|---:|---|
| `0x20` | `0x08` | `0x18` | `e_shoff` +16 |
| `0x30` | `0x1b` | `0x1c` | `e_shnum` 27 -> 28 |
| `0x32` | `0x1a` | `0x1b` | `e_shstrndx` 26 -> 27 |

Program headers (8 x 32 = 256 B): **18 differing bytes**, all in the three affected segments:

| segment | original | ours |
|---|---|---|
| `LOAD #2` offset/vaddr/paddr | `0x20bba0` / `0x20cba0` | `0x20bb80` / `0x20cb80` |
| `LOAD #2` filesz/memsz | `0x9dcc` / `0x1077c` | `0x9df4` / `0x107d8` |
| `ARM_EXIDX` offset/vaddr/paddr | `0x19cc08` | `0x19cbf0` |
| `ARM_EXIDX` filesz/memsz | `0x6f70` | `0x6f88` |
| `GNU_RELRO` offset/vaddr/paddr | `0x20bba0` / `0x20cba0` | `0x20bb80` / `0x20cb80` |
| `GNU_RELRO` filesz/memsz | `0x7460` | `0x7480` |

Section header table: 27 vs 28 entries; the extra 40-byte entry and the different `sh_name`
offsets (the original's `.shstrtab` is in a different order) account for the rest. The file-size `+56` is exactly
`+8` (`.aezdata` bytes) `+9` (`.shstrtab`) `+40` (one extra section header) `-1` (the
original has a 1-byte pad between `.shstrtab` and the section header table; ours does not).
The earlier section-size changes are address-neutral by the time `.shstrtab` is reached:
`.text -24` is canceled by `.ARM.exidx +24`, `.ARM.extab -8` is absorbed by 8 bytes of
alignment before `.rodata`, and `.data.rel.ro +32` exactly cancels its `-0x20` start shift
(so it ends at the same address, `0x20fb98`). `.bss` is NOBITS and contributes no file bytes.

---

## 2. `.dynsym` order audit

Both tables contain exactly **5,370 defined symbols**; the name set is identical
(no missing/extra symbol names). The alignment uses only defined symbols (`st_shndx != 0`):

```python
o = [name for defined symbols in original order]
u = [name for defined symbols in ours order]
sm = difflib.SequenceMatcher(None, o, u, autojunk=False)
sm.get_matching_blocks()
```

Measured:

| metric | value |
|---|---:|
| defined symbols | 5,370 each |
| non-empty matching blocks | **74** |
| difflib ratio | **0.9735567970** |
| matched symbols | **5,228** |
| unmatched symbol occurrences | **284** (142 each side) |
| symbols at the same absolute index | **807 / 5,370** |
| out-of-order gaps | **73** |

The ratio is the same as the w3307 post-archive-order measurement
(`0.973557`, matched 5,228), so the archive link-order fix is already in this HEAD.

### 2.1 Top 10 remaining out-of-order blocks (by max span)

| # | span | original range | ours range | content |
|---:|---:|---|---|---|
| 1 | 26 | `[5056:5057]` = `zip_error_to_str` | `[5030:5056]` = 26 `zip_*`/`_zip_*` symbols | libzip order |
| 2 | 17 | `[226:243]` = 17 `Java_..._GOF2HD2012_*` / `g_android_*` | `[247:247]` empty | JNI `main.c` order |
| 3 | 17 | `[5001:5018]` = `zip_source_function`..`_zip_new` | `[4993:4994]` = `zip_source_free` | libzip order |
| 4 | 17 | `[5028:5045]` = `zip_add_dir`..`zip_replace` | `[5008:5009]` = `_zip_unchange` | libzip order |
| 5 | 16 | `[4936:4952]` = `crc32`..`zcfree` | `[4937:4938]` = `adler32_combine` | zlib order |
| 6 | 16 | `[4980:4980]` empty | `[4969:4985]` = 16 `inflate_*` symbols | zlib order |
| 7 | 13 | `[1108:1121]` = `Array<int>` / `GameText` block | `[1127:1127]` empty | `GameText` split / `Array` TU |
| 8 | 13 | `[5054:5054]` empty | `[5013:5026]` = 13 `_zip_*` symbols | libzip order |
| 9 | 11 | `[152:152]` empty | `[155:166]` = 11 `Java_..._GOF2HD2012_*` | JNI `main.c` order |
| 10 | 9 | `[4991:5000]` = `_zip_filerange_crc`..`zip_fopen_index` | `[4991:4992]` = `zip_add_dir` | libzip order |

The next blocks are the `AbyssEngine.cpp` mega-TU (`_Z13AELabelObject...`, `Curve*`,
`getAppVersion`, `esMatrixMultiply`, ...) and the `GameText`/`Array<int>` block.

### 2.2 `.dynsym` content differences (beyond order)

| symbol | original | ours | kind |
|---|---|---|---|
| `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | size 1,996 | size 1,992 | body -4 |
| `_ZN12HangarWindow10initializeEv` | size 3,052 | size 2,984 | body **-68** |
| `_ZN11AbyssEngine11PaintCanvas8DrawMeshEPNS_4MeshERNS_6AEMath6MatrixES5_jS5_` | `STB_WEAK` | `STB_GLOBAL` | bind |
| `_ZN11AbyssEngine13getAppVersionEv` | size 0 | size 2 | 0-size alias vs 2-byte `bx lr` |

Across all 5,370 symbols: **4,488** value differences (text addresses), **302** `st_shndx`
differences (all `.bss` symbols: original section 22, ours section 23 because of `.aezdata`),
**3** size differences, **1** bind difference, **0** type differences.

`.dynstr` has the same multiset of strings (5,591 strings, 5,590 unique, 189,208 B); the
177,258 differing bytes are entirely string-table ordering.

---

## 3. `.ARM.exidx` / `.ARM.extab` entry-level diff

| metric | original | ours |
|---|---:|---:|
| `.ARM.exidx` address / size | `0x19cc08` / `0x6f70` | `0x19cbf0` / `0x6f88` |
| exidx entries | **3,566** | **3,569** (**+3**) |
| CANT entries | 272 | 273 (**+1**) |
| compact entries | 856 | 857 (**+1**) |
| extab entries | 2,438 | 2,439 (**+1**) |
| `.ARM.extab` address / size | `0x1a3b78` / `0x26298` | `0x1a3b78` / `0x26290` (**-8**) |

### 3.1 Named functions with no exidx record in one binary

| function | original records | ours records |
|---|---|---|
| `Java_..._GOF2HD2012_GetLeaderboardScore` | `CANT` + `compact` at `0x6b9d8` | none |
| `_ZN3NFCC2Ev` | `CANT` + `compact` at `0x69588` | none |
| `_Z7glErrorv` | `compact` at `0x83ee8` | none |
| `ExitFunction` | none | `CANT` + `compact` at `0x93cc8` |
| `__cxa_end_cleanup` | none | `CANT` at `0x191d50` |

`getAppVersion` is *not* in this table: both binaries have a compact record at `0x6db20`.
In the original, `getAppVersion` (size 0) and `Engine::InitGL` (size 408) share address
`0x6db20`; the compact record is `getAppVersion`'s and the extab record is `InitGL`'s. Ours
gives `getAppVersion` a 2-byte body, so `Engine::InitGL` starts at `0x6db24` and its extab
record moves +4; the compact record stays at `0x6db20`.

The two `CANT`+`compact` pairs are the clearest missing records: the input
`.ARM.exidx.text._ZN3NFCC2Ev` section exists in `NFC.cpp.obj` (8 B, compact record) and in
`main.c.obj` for `GetLeaderboardScore`, but gold drops both from the output link map.

### 3.2 Named CANT-sentinel count mismatches

**11 named functions are missing a CANT sentinel** that the original has:

```
Java_..._GOF2HD2012_GetLeaderboardScore
_Z8AddTouchiiii
_ZN10LODManagerC2Ev
_ZN11AbyssEngine12FBOContainerC2EPNS_6EngineENS_6StringE
_ZN11AbyssEngine9CheatCodeC2Ev
_ZN14PendingProductC2EP9BluePrint
_ZN15MenuTouchWindowC2Ei
_ZN3NFCC2Ev
_zip_mkstemp
zip_fread
zip_source_zip
```

**36 named functions have an extra CANT sentinel** in ours:

```
ExitFunction
_Z13loadPortraitsPN11AbyssEngine6EngineE
_ZN10StarSystemC2Ei
_ZN11AbyssEngine10QuaternionC2Ev
_ZN11AbyssEngine15ResourceTextureC2ERKNS_6StringEf
_ZN21ParticleSystemManager10emitManualEiRKN11AbyssEngine6AEMath6VectorEiS4_f
_ZN21ParticleSystemManager15addSpriteSystemEPKN11AbyssEngine6AEMath6MatrixERK5ArrayIN16ParticleSettings11ParticleSetEEb
_ZN4NodeC2Ei
_ZN5Level10createShipEiiiP8Waypointbb
_ZN6GalaxyC2Ev
_ZN6StatusC2Ev
_ZN6WantedC2EiN11AbyssEngine6StringEiibiiiiiiiii
_ZN8ListItemC2EiiPN11AbyssEngine6StringE
_ZNSt9exceptionD2Ev
__cxa_end_cleanup
_zip_cdir_free
_zip_entry_new
_zip_error_strerror
_zip_file_get_offset
_zip_new
_zip_unchange_data
inflateReset
ndk23_setDisplayHeightAndWidth
ndk_resetNativeItemInformationList
zip_close
zip_delete
zip_fclose
zip_file_strerror
zip_fopen_index
zip_get_archive_comment
zip_get_archive_flag
zip_rename
zip_replace
zip_source_free
zip_source_function
zip_unchange_all
```

Net CANT count is +1 (272 -> 273), so the 11 missing and 36 extra are offset by local-function
swaps.

### 3.3 Named functions with different `.ARM.extab` record length

| function | original length | ours length | Δ |
|---|---:|---:|---:|
| `_ZN11LevelScript7processEi` | 348 | 352 | +4 |
| `_ZN12HangarWindow10initializeEv` | 352 | 356 | +4 |
| `_ZN12HangarWindow6renderEv` | 948 | 912 | -36 |
| `_ZN13RecordHandler16recordStoreWriteEi` | 656 | 640 | -16 |
| `_ZN3Hud4drawExxP9PlayerEgobjj` | 476 | 492 | +16 |
| `_ZN5Level10createShipEiiiP8Waypointbb` | 336 | 328 | -8 |
| `_ZN5Level13createMissionEv` | 956 | 972 | +16 |
| `_ZN7StarMap4drawEv` | 116 | 128 | +12 |
| `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | 172 | 176 | +4 |

The remaining `.ARM.extab` length shifts are in local/libc++abi functions. The full
length-bucket multiset changes (bytes: original count -> ours count) are:
`12: 1720 -> 1721`, `28: 126 -> 128`, `32: 105 -> 103`, `96: 1 -> 2`, `104: 7 -> 6`,
`116: 3 -> 2`, `128: 2 -> 3`, `172: 4 -> 3`, `176: 1 -> 2`, `328: 0 -> 1`, `336: 1 -> 0`,
`348: 1 -> 0`, `356: 1 -> 2`, `476: 1 -> 0`, `492: 0 -> 1`, `640: 0 -> 1`, `656: 1 -> 0`,
`912: 0 -> 1`, `948: 1 -> 0`, `956: 2 -> 1`, `972: 0 -> 1`.

---

## 4. `.data.rel.ro` and `.bss` exact objects

### 4.0 `.data` (exact layout)

`.data` is exact in address and size (`0x214000` / `0x296c`), and all 82 exported `OBJECT`
anchors are at identical offsets. The 304 differing bytes start at `0x8d0`; every one is at
an `R_ARM_RELATIVE` word. The relocation set is identical, so the only difference is the
pointer addends into `.rodata` (e.g. `VirtualInput.cpp` `keys` and
`ListItemWindow.cpp` `LISTITEMWINDOW_UNITS`). Once `.rodata` ordering is fixed, `.data`
should become exact automatically.

### 4.1 `.data.rel.ro` (+32 B)

All 277 exported `OBJECT` anchors have the same size; 166 have a different offset. The offset
delta is piecewise constant:

| group | count | first object | last object | delta |
|---|---:|---|---|---:|
| 1 | 111 | `_ZTV20FileInterfaceAndroid` (`0x0`) | `_ZTV10ModStation` (`0xe34`) | 0 |
| 2 | 15 | `_ZTI10ModStation` (`0xf80`) | `_ZTI13PlayerStation` (`0x1268`) | **-4** |
| 3 | 151 | `_ZTV7BombGun` (`0x1274`) | `_ZTISt10bad_typeid` (`0x21ac`) | **+40** |

The exact budget is:

| component | bytes | effect |
|---|---:|---|
| extra `MenuTouchWindow_languageIds[11]` object (10 ints + function pointer) | **44** | ours +44 |
| missing 4-byte gap between `_ZTV10ModStation` and `_ZTI10ModStation` (orig gap `0xfc`, ours `0xf8`) | **-4** | ours -4 |
| missing 8-byte zero gap before the `__cxa_demangle` switch table (orig `0x2f68`, ours no gap) | **-8** | ours -8 |
| **net** | **+32** | |

The extra 44-byte object sits between `_ZTI13PlayerStation` and `_ZTV7BombGun`; the 151
objects after it are all +40. The 8-byte gap is visible in the tail dump: original has
`00000000` at `0x2f68` before `de721e00...`; ours starts the table at `0x2f90` with no gap
(so the switch table itself is +0x20 = +32, while the objects before the gap are +40).

Relocation offsets/types are identical for all 277 objects (`R_ARM_RELATIVE`/`R_ARM_ABS32`
multiset unchanged). 12 objects have different relocation **addends** (content bytes differ
only at relocation words):

```
_zip_err_str
_ZTVN10__cxxabiv116__shim_type_infoE
_ZTVN10__cxxabiv123__fundamental_type_infoE
_ZTVN10__cxxabiv117__array_type_infoE
_ZTVN10__cxxabiv120__function_type_infoE
_ZTVN10__cxxabiv116__enum_type_infoE
_ZTVN10__cxxabiv117__class_type_infoE
_ZTVN10__cxxabiv120__si_class_type_infoE
_ZTVN10__cxxabiv121__vmi_class_type_infoE
_ZTVN10__cxxabiv117__pbase_type_infoE
_ZTVN10__cxxabiv119__pointer_type_infoE
_ZTVN10__cxxabiv129__pointer_to_member_type_infoE
```

The first differing byte is at `0xc08` (a relocated pointer addend inside/after
`_ZTIN11AbyssEngine15SpecCubeMappingE`), not a layout difference.

### 4.2 `.bss` (+44 B)

* Original `0x216970` / `0x69ac`; ours `0x216980` / `0x69d8`. The address is +0x10 because
  `.aezdata` (8 B at `0x21696c`) forces 16-byte alignment of `.bss`.
* All **302** exported `OBJECT` anchors have the **same size**; their offsets differ because
  the original `.bss` order is not TU order. First divergence: `g_pActivity` at original
  `0x0`, ours `0x20e4`.
* The original `.bss` starts with the `main.c` COMMON symbol `g_pActivity` at 0, then the
  `VirtualInput` block (`stackX` at `0x1010`), then the rest of `main.c` at `0x1034`; ours
  starts with `g_android_leaderboard_scores` at 0 and puts `g_pActivity` at `0x20e4`.
* Two local-static size differences are directly attributable:

| object | original block | ours `.bss` | Δ | evidence |
|---|---:|---:|---:|---|
| `VirtualInput.cpp` | `0x1030` (`0x4..0x1034`) | `0x1040` | **+16** | our `g_inputE.0..3` (4 x 4 B) at `0x1030..0x1040`; original block ends at `0x1034` |
| `BlowfishSubkeyFile.cpp` | `0x1048` (`S` `0x5908` .. `g_android_leaderboard_scores` `0x6950`) | `0x104c` | **+4** | `S`+`P`+`SubkeyFile` padding |

The remaining **+24 B** is local statics in other TUs. Our per-object `.bss` blocks with
non-exported local statics are: `VirtualInput` `0x1024`, `NdkHooks` `0x38`,
`SpaceLounge` `0x20`, `LevelScript` `0x20`, `GameText` `0x20`, `Globals` `0x1f`,
`Radio` `0x14`, `ModStation` `0x14`, `DialogueWindow` `0x14`, `Engine` `0xe`, `Level` `0xc`,
`ApplicationManager` `0x5`, `StatusWindow` `0x4`. The stripped original has no local `.bss`
names/sizes, so the +24 residual cannot be attributed exactly from `.dynsym` alone; it is the
same limitation documented in w3305's audit.

---

## 5. `.text` per-function address delta

The first `.text` difference is at `0x12`: the original has `...d2ee` and ours `...e8ed`, a
Thumb `bl` offset to a different target address. It is a relocation/address difference, not a
source-body difference.

Method: match exported `STT_FUNC` symbols by mangled name, sort by original address, and print
every point where `delta = ours_addr - original_addr` changes. The delta is constant between
the rows, so this table covers every exported function. Full table in the appendix.

Summary:

| metric | value |
|---|---:|
| delta-change points | **63** |
| first function delta | `+0` |
| final exported-function delta | **-640** (`_ZSt14set_unexpectedPFvvE`) |
| maximum delta | **+9,764** (`zip_fopen`) |
| minimum delta | **-768** (`sha256_block_data_order`) |
| `.text` size delta | **-24** |

The single biggest remaining **drift source** is the plain-`.text` grouping in
`tools/gen_section_order.py` (`PLAIN_TEXT_LEAD=0x180`). It inserts the 9,804-byte
libcrypto/libgcc plain-`.text` block before `zip_fopen`, then inverts the SHA/OPENSSL region:

| point | gap_orig | gap_ours | gap_delta |
|---|---:|---:|---:|
| `zip_stat_init` -> `zip_fopen` | 0 | 9,804 | **+9,804** |
| `_zip_error_strerror` -> `SHA224_Init` | 0 | -10,168 | **-10,168** |
| `SHA224_Final` -> `OPENSSL_atomic_add` | 4 | 7,548 | **+7,544** |
| `OPENSSL_instrument_bus2` -> `sha256_block_data_order` | 328 | -7,576 | **-7,904** |
| `sha256_block_data_order_neon` -> `__cxa_end_cleanup` | 576 | 1,392 | **+816** |

Its net contribution is only **+92 B**, but it moves thousands of addresses and is the main
reason `.ARM.exidx`/`.ARM.extab` and the libcrypto `.text` functions are not byte-exact.

The biggest remaining **app** drift sources:

| source | bytes | note |
|---|---:|---|
| `HangarWindow::initialize` body | **-68** | exported size 3,052 vs 2,984; delta `+40 -> -28` |
| `levelTextAnchor` helper | +40 | `src/game/world/Level.cpp`; original has no such helper |
| `arabicFormOffset` helper | +16 | `src/engine/core/GameText.cpp`; original inlines |
| `ObjectGunProjectileMesh` helper | +16 | `src/game/weapons/ObjectGun.cpp`; original inlines |
| `levelMissionPad` helper | +8 | `src/game/world/Level.cpp` |
| `generatorMissionLayoutPad` helper | +4 | `src/game/menu/MTitle.cpp` |
| empty `mtwTextLayoutAnchor` | +4 | 2-byte empty function + alignment; the 2,934-byte body is gone |
| `ConfigReader::ReadTimer`/`ReadKeyMap` | -8 | each 4 B short (landing-pad `b.n` + `nop`) |
| `Generator::createMission` body | -4 | exported size 1,996 vs 1,992 |
| libc++abi local gaps | -588 / -96 | prebuilt archive, not app source |
| plain `.text` grouping | +92 net | see above |

The final delta is -640 even though `.text` is only -24 because the local libc++abi tail after
the last exported function is 616 B larger in ours.

---

## 6. Ranked next 10 fixes (expected whole-file byte effect)

`sha256` cannot be predicted from byte counts; the "bytes targeted" column is the number of
currently differing file bytes the fix can eliminate (or the section-size change). Every fix
also changes `.note.gnu.build-id` (20 B), so the whole-file sha256 will change even for a
size-neutral fix.

| rank | fix | expected effect | risk / notes |
|---:|---|---|---|
| 1 | Remove the extra `.aezdata` section (`src/engine/file/AEFile.cpp` `#pragma clang section data=".aezdata"`; `pakExtension[8]`). | file **-57 B** (8 B section data + 9 B `.shstrtab` + 40 B section header), `.bss` address **-0x10**; also removes the extra `.aezdata` section-header entry | Low risk; may shift `.bss` and data addresses. The original `.aez` string is already in `.rodata`. The separate 259-byte `.shstrtab` reordering remains until the section-name table order matches. |
| 2 | Remove the 44-byte `MenuTouchWindow_languageIds[11]` `.data.rel.ro` anchor and the empty `mtwTextLayoutAnchor` function. | `.data.rel.ro` **-44 B**, `.text` **-2/4 B**; after this `.data.rel.ro` is -12 B vs original | Medium; the anchor is a deliberate layout knob. Measure `byte_exact` before keeping. |
| 3 | Restore the `ModStation` 4-byte vtable->typeinfo gap and the `__cxa_demangle` 8-byte zero gap. | `.data.rel.ro` **+12 B**, making it exact after fix 2 | Low/medium; alignment/order only. |
| 4 | Fix `.bss` local statics: remove/relocate `g_inputE.0..3` (16 B), Blowfish +4, residual +24. | `.bss` **-44 B**, section size exact; NOBITS, so no file bytes, but program-header `memsz` and `.bss` addresses exact | Medium; original local `.bss` names are stripped. |
| 5 | Fix `.ARM.exidx`/`.ARM.extab`: 11 missing CANT, 36 extra CANT, 9 named extab length diffs; restore the dropped NFC/GetLeaderboardScore records. | `.ARM.exidx` **-24 B**, `.ARM.extab` **-8 B**; 32 B of file data plus entry ordering | Medium; the input exidx sections exist but gold drops them. |
| 6 | Fix app body deltas: `HangarWindow::initialize` **-68**, `Generator::createMission` -4, `ConfigReader` -8, `getAppVersion` +2. | `.text` **+78 B** (currently -24; would become +54 before local fixes); removes the largest app body gap | High; `HangarWindow::initialize` is a real 68-byte reconstruction. |
| 7 | Fix plain `.text` grouping (`PLAIN_TEXT_LEAD` / stop grouping plain `.text` archive members). | `.text` **-92 B** net; removes the ~10 KB address oscillation and should improve many `byte_exact`/`linked_exact` functions and the exidx table | High; moves thousands of addresses. |
| 8 | Fix `.dynsym` order: libzip/zlib archive order, JNI `main.c` order, `GameText`/`Array<int>` TU, `AbyssEngine.cpp` mega-TU. | `.dynsym` 29,176 B + `.dynstr` 177,258 B + `.hash` 8,788 B + `.gnu.version` 338 B + `.rel.plt` 5,129 B = **~220,689 B** targeted | High; TU membership/link order, but the largest byte-count win. |
| 9 | Fix `.rodata` ordering: make shader sources non-mergeable `const char[]`, restore per-TU shader arrays, reorder libz `crc32.o` and `ModStation`. | `.rodata` **119,730 B** + `.data` 304 B + `.data.rel.ro` 3,102 B addends = **~123,136 B** targeted | High; `.rodata` blob multiset is already exact, so this is pure ordering. |
| 10 | Fix the remaining `.text` body/relocation exactness per function. | 1,105,634 raw differing bytes at `0x12`; each byte-exact function removes its address-dependent branch/literal words | Long tail; use `verify-fn` and `report.json`. |

The practical order is 1-5 (small, structural, low risk), then 6-7 (body/grouping), then 8-9
(large ordering wins), then 10 (long tail). Fix 8 and 9 are the largest byte-count wins but
also the most invasive.

---

## 7. Reproduction commands

```sh
cd /home/debian/GalaxyOnFire2/wt/w3274
cmake --preset default
cmake --build cmake-build-match --target check -j32
```

### 7.1 Section diff

```sh
python3 /tmp/elfcmp.py ../_work/bins/android_2.0.16_libgof2hdaa.so \
    cmake-build-match/libgof2hdaa.so
```

The parser reads the ELF32 section table, extracts each section, and compares bytes at the
same section-relative offset. For NOBITS `.bss` only the header is compared.

### 7.2 `.dynsym` alignment

```sh
python3 /tmp/dynsym.py ../_work/bins/android_2.0.16_libgof2hdaa.so > /tmp/orig_dynsym.tsv
python3 /tmp/dynsym.py cmake-build-match/libgof2hdaa.so        > /tmp/ours_dynsym.tsv
python3 - <<'PY'
import difflib
def load(p):
    out=[]
    for l in open(p):
        i,b,t,sh,val,sz,n=l.rstrip('\n').split('\t')
        out.append(dict(idx=int(i),bind=int(b),typ=int(t),shndx=int(sh),
                        value=int(val,16),size=int(sz),name=n))
    return out
O=[s for s in load('/tmp/orig_dynsym.tsv') if s['shndx']!=0]
U=[s for s in load('/tmp/ours_dynsym.tsv') if s['shndx']!=0]
o=[s['name'] for s in O]; u=[s['name'] for s in U]
sm=difflib.SequenceMatcher(None,o,u,autojunk=False)
print('ratio',sm.ratio())
print('blocks',len([b for b in sm.get_matching_blocks() if b.size]))
print('matched',sum(b.size for b in sm.get_matching_blocks()))
PY
```

### 7.3 `.ARM.exidx` / `.ARM.extab`

```sh
python3 /tmp/exidx.py ../_work/bins/android_2.0.16_libgof2hdaa.so \
    cmake-build-match/libgof2hdaa.so
python3 /tmp/exidx_match2.py ../_work/bins/android_2.0.16_libgof2hdaa.so \
    cmake-build-match/libgof2hdaa.so
```

`exidx.py` decodes the 8-byte entries (`prel31` target, `prel31` extab or `1` = CANT) and
names targets from `.dynsym`. `exidx_match2.py` matches by exported name first, then by
`prel31` target + text delta, and compares kind/extab bytes. `dynsym.py` emits
`idx<TAB>bind<TAB>type<TAB>shndx<TAB>value<TAB>size<TAB>name`.

### 7.4 `.data.rel.ro` / `.bss`

```sh
python3 /tmp/data_audit.py ../_work/bins/android_2.0.16_libgof2hdaa.so \
    cmake-build-match/libgof2hdaa.so
```

The script matches exported `OBJECT` symbols by name, computes section-relative offsets,
masks relocation words, and compares relocation `(type, symbol)` sets. For the link map:

```sh
ninja -C cmake-build-match -t commands libgof2hdaa.so | grep 'ld.gold -shared' \
  > /tmp/linkcmd.txt
sed -e 's#-o [^ ]*/cmake-build-match/libgof2hdaa.so#-Map=/tmp/link.map -o /tmp/libgof2_map.so#' \
    -e 's# && [^ ]*/arm-linux-androideabi-strip --strip-all [^ ]*##' \
    /tmp/linkcmd.txt > /tmp/linkcmd_map.sh
bash /tmp/linkcmd_map.sh
```

### 7.5 `.text` delta walk

```sh
python3 /tmp/gapwalk.py ../_work/bins/android_2.0.16_libgof2hdaa.so \
    cmake-build-match/libgof2hdaa.so
```

### 7.6 Whole-file hashes

```sh
sha256sum ../_work/bins/android_2.0.16_libgof2hdaa.so cmake-build-match/libgof2hdaa.so
```

---

## Appendix: full `.text` delta-change table

`gap_orig` / `gap_ours` are the bytes between the previous exported function's end and the next
exported function's start (local code + alignment). Negative `gap_ours` means the two exported
functions are inverted in our link (plain-`.text` grouping).

| # | prev -> next | delta | gap_orig | gap_ours | gap_delta | cause |
|---:|---|---:|---:|---:|---:|---|
| 0 | `None` -> `Java_net_fishlabs_gof2hdallandroid2012_ToJNI_BackButtonPressed` | +0 | 0 | 0 | +0 | start |
| 1 | `_ZN11AbyssEngine13getAppVersionEv` -> `_ZN11AbyssEngine6Engine6InitGLEbii` | +4 | 0 | 2 | +2 | getAppVersion 0-size alias +2 |
| 2 | `_ZN11AbyssEngine15EaseInOutMatrixC2Ev` -> `_ZN11AbyssEngine6AEMath6MatrixC2Ev` | +0 | 6 | 2 | -4 | alignment -4 |
| 3 | `_Z14ArrayAddCachedIjEvT_R5ArrayIS0_E` -> `_ZN8GameText13getRegionCodeEv` | +16 | 2 | 18 | +16 | arabicFormOffset helper +16 |
| 4 | `_ZN11AbyssEngine6AEMath14MatrixDebugOutERKNS0_6MatrixE` -> `_ZN11AbyssEngine10QuaternionC1Ev` | +14 | 2 | 0 | -2 | alignment -2 |
| 5 | `_ZN11AbyssEngine10QuaternionC2ERKNS_6AEMath6MatrixE` -> `_ZN11AbyssEngine10Quaternion3SetERKNS_6AEMath6MatrixE` | +12 | 2 | 0 | -2 | alignment -2 |
| 6 | `_Z8ArrayAddIiEvT_R5ArrayIS0_E` -> `_ZN8GameText7getTextEi` | +52 | 2 | 42 | +40 | levelTextAnchor helper +40 |
| 7 | `_ZN11AbyssEngine18ApplicationManager10CheckCrackEPKc` -> `_Z7glErrorv` | +50 | 2 | 0 | -2 | alignment -2 |
| 8 | `_Z7glErrorv` -> `_ZN11AbyssEngine6EngineC1Ev` | +52 | 0 | 2 | +2 | alignment +2 |
| 9 | `_ZN11AbyssEngine6Engine11AfterGLInitEv` -> `_ZN11AbyssEngine6Engine15ResetLightParamEv` | +56 | 0 | 4 | +4 | local/alignment +4 |
| 10 | `_ZN11AbyssEngine12ConfigReader25RegisterTokenReadFunctionENS_6StringEPFvPS0_PvES3_` -> `_ZN5ArrayIPN11AbyssEngine11TokenStructEED2Ev` | +48 | 700 | 692 | -8 | ConfigReader ReadTimer/ReadKeyMap -8 |
| 11 | `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` -> `_ZN6MTitleC1Ev` | +44 | 0 | 0 | +0 | Generator::createMission body -4 |
| 12 | `_ZN6MTitle10OnTouchEndEii` -> `_ZN6MTitle9OnReleaseEv` | +48 | 0 | 4 | +4 | generatorMissionLayoutPad +4 |
| 13 | `_ZN8KIPlayer25projectCollisionOnSurfaceERKN11AbyssEngine6AEMath6VectorE` -> `_ZN8ListItemC1EiiPN11AbyssEngine6StringE` | +46 | 2 | 0 | -2 | alignment -2 |
| 14 | `_ZN8ListItem11getNumLinesEv` -> `_ZN8ListItem12checkCreditsEv` | +44 | 2 | 0 | -2 | alignment -2 |
| 15 | `_ZN9Explosion9translateERKN11AbyssEngine6AEMath6VectorE` -> `_ZN6StatusC1Ev` | +42 | 2 | 0 | -2 | alignment -2 |
| 16 | `_ZN6Status9isOnStackEP7Station` -> `_ZN6Status10setStationEP7Station` | +40 | 2 | 0 | -2 | alignment -2 |
| 17 | `_ZN5Level20setPlayerEngineColorEs` -> `_ZN5Level19initParticleSystemsEv` | +48 | 4 | 12 | +8 | levelMissionPad +8 |
| 18 | `_ZN11AbyssEngine15ResourceTextureC2ERKNS_6StringEf` -> `_ZN4NodeC1Ei` | +46 | 2 | 0 | -2 | alignment -2 |
| 19 | `_ZN16SystemPathFinder15getJumpDistanceEP5ArrayIP11SolarSystemEii` -> `_ZN16SystemPathFinder13getSystemPathEP5ArrayIP11SolarSystemEii` | +44 | 2 | 0 | -2 | alignment -2 |
| 20 | `_ZN14PlayerCreature8calmDownEv` -> `_ZN14PlayerCreature5resetEv` | +40 | 4 | 0 | -4 | alignment -4 |
| 21 | `_ZN13PlayerStation25projectCollisionOnSurfaceERKN11AbyssEngine6AEMath6VectorE` -> `_ZN6WantedC1EiN11AbyssEngine6StringEiibiiiiiiiii` | +38 | 2 | 0 | -2 | alignment -2 |
| 22 | `_ZN6Wanted18getCurrentLocationEv` -> `_ZN15MenuTouchWindowC1Ei` | +40 | 0 | 2 | +2 | alignment +2 |
| 23 | `_ZN15MenuTouchWindow23startSupernovaChallengeEv` -> `_ZN14PendingProductC1EP9BluePrint` | +44 | 0 | 4 | +4 | empty mtwTextLayoutAnchor +4 |
| 24 | `_ZN14ListItemWindow10OnTouchEndEii` -> `_ZN10StarSystemC1Ei` | +40 | 4 | 0 | -4 | alignment -4 |
| 25 | `_ZN12HangarWindow10initializeEv` -> `_ZN12HangarWindow27refreshCurrentContentHeightEv` | -28 | 0 | 0 | +0 | HangarWindow::initialize body -68 |
| 26 | `_ZN12Achievements12isEliteMedalEi` -> `_ZN12PlayerTurretC1EiP6PlayerP10AEGeometryfff` | -24 | 2 | 6 | +4 | alignment +4 |
| 27 | `_ZN12MarqueeImage4drawEv` -> `_ZN9ObjectGunC1EiP3GunijP5Level` | -8 | 4 | 20 | +16 | ObjectGunProjectileMesh +16 |
| 28 | `_ZN11ModMainMenu17ShowLoadingScreenEv` -> `_ZN6GalaxyC1Ev` | -10 | 2 | 0 | -2 | alignment -2 |
| 29 | `_ZN6Galaxy9getSystemEi` -> `_ZN6Galaxy10getStationEi` | -8 | 0 | 2 | +2 | alignment +2 |
| 30 | `_ZN18ParticleSystemMesh12getQuadCountEv` -> `inflateReset` | -10 | 2 | 0 | -2 | alignment -2 |
| 31 | `inflatePrime` -> `inflateInit2_` | -12 | 2 | 0 | -2 | alignment -2 |
| 32 | `_zip_get_name` -> `zip_file_strerror` | -14 | 2 | 0 | -2 | alignment -2 |
| 33 | `zip_file_strerror` -> `zip_fopen_index` | -16 | 2 | 0 | -2 | alignment -2 |
| 34 | `zip_error_clear` -> `zip_get_archive_comment` | -18 | 2 | 0 | -2 | alignment -2 |
| 35 | `zip_file_error_get` -> `_zip_file_get_offset` | -20 | 2 | 0 | -2 | alignment -2 |
| 36 | `_zip_unchange` -> `_zip_new` | -22 | 2 | 0 | -2 | alignment -2 |
| 37 | `_zip_new` -> `zip_close` | -24 | 2 | 0 | -2 | alignment -2 |
| 38 | `zip_unchange_archive` -> `zip_source_free` | -26 | 2 | 0 | -2 | alignment -2 |
| 39 | `zip_source_filep` -> `_zip_source_file_or_p` | -28 | 2 | 0 | -2 | alignment -2 |
| 40 | `zip_strerror` -> `_zip_entry_new` | -30 | 2 | 0 | -2 | alignment -2 |
| 41 | `_zip_entry_new` -> `zip_get_archive_flag` | -32 | 2 | 0 | -2 | alignment -2 |
| 42 | `zip_set_archive_flag` -> `_zip_unchange_data` | -34 | 2 | 0 | -2 | alignment -2 |
| 43 | `_zip_unchange_data` -> `zip_fread` | -32 | 0 | 2 | +2 | alignment +2 |
| 44 | `zip_fread` -> `zip_rename` | -34 | 2 | 0 | -2 | alignment -2 |
| 45 | `zip_rename` -> `zip_source_zip` | -32 | 0 | 2 | +2 | alignment +2 |
| 46 | `zip_stat_index` -> `zip_source_function` | -34 | 2 | 0 | -2 | alignment -2 |
| 47 | `zip_source_function` -> `zip_replace` | -36 | 2 | 0 | -2 | alignment -2 |
| 48 | `_zip_replace` -> `zip_delete` | -38 | 2 | 0 | -2 | alignment -2 |
| 49 | `zip_get_file_comment` -> `_zip_mkstemp` | -36 | 0 | 2 | +2 | alignment +2 |
| 50 | `zip_error_get` -> `zip_fclose` | -38 | 2 | 0 | -2 | alignment -2 |
| 51 | `zip_fclose` -> `_zip_cdir_free` | -40 | 2 | 0 | -2 | alignment -2 |
| 52 | `zip_stat_init` -> `zip_fopen` | +9764 | 0 | 9804 | +9804 | plain .text group +9804 |
| 53 | `zip_fopen` -> `zip_unchange_all` | +9762 | 2 | 0 | -2 | alignment -2 |
| 54 | `zip_unchange_all` -> `_zip_error_strerror` | +9760 | 2 | 0 | -2 | alignment -2 |
| 55 | `_zip_error_strerror` -> `SHA224_Init` | -408 | 0 | -10168 | -10168 | plain .text group -10168 |
| 56 | `SHA224_Final` -> `OPENSSL_atomic_add` | +7136 | 4 | 7548 | +7544 | plain .text tail +7544 |
| 57 | `OPENSSL_instrument_bus2` -> `sha256_block_data_order` | -768 | 328 | -7576 | -7904 | plain .text tail -7904 |
| 58 | `sha256_block_data_order_neon` -> `__cxa_end_cleanup` | +48 | 576 | 1392 | +816 | plain .text tail +816 |
| 59 | `__cxa_deleted_virtual` -> `__dynamic_cast` | -540 | 2212 | 1624 | -588 | libc++abi local -588 |
| 60 | `__dynamic_cast` -> `_ZNSt13bad_exceptionD1Ev` | -542 | 1300 | 1298 | -2 | alignment -2 |
| 61 | `_ZNSt9exceptionD0Ev` -> `_ZNKSt9exception4whatEv` | -544 | 2 | 0 | -2 | alignment -2 |
| 62 | `_ZNKSt10bad_typeid4whatEv` -> `_ZSt14set_unexpectedPFvvE` | -640 | 96 | 0 | -96 | libc++abi local -96 |

---

## Appendix B: scratch scripts

These are the exact scripts used for the measurements above. Copy them to `/tmp` (the
commands in §7 assume those paths). `data_audit.py` imports `dynsym.py`; `exidx_match2.py`
imports `exidx.py` and `dynsym.py`.

### `/tmp/elfcmp.py`

```python
#!/usr/bin/env python3
import struct, sys, hashlib

def load(path):
    d=open(path,'rb').read()
    assert d[:4]==b'\x7fELF'
    e_shoff=struct.unpack_from('<I',d,0x20)[0]
    e_shentsize=struct.unpack_from('<H',d,0x2e)[0]
    e_shnum=struct.unpack_from('<H',d,0x30)[0]
    e_shstrndx=struct.unpack_from('<H',d,0x32)[0]
    secs=[]
    for i in range(e_shnum):
        off=e_shoff+i*e_shentsize
        name,typ,flags,addr,offset,size,link,info,align,entsize=struct.unpack_from('<IIIIIIIIII',d,off)
        secs.append(dict(name=name,typ=typ,flags=flags,addr=addr,offset=offset,size=size,link=link,info=info,align=align,entsize=entsize,idx=i))
    shstr=secs[e_shstrndx]
    st=d[shstr['offset']:shstr['offset']+shstr['size']]
    for s in secs:
        e=st.find(b'\0',s['name'])
        s['sname']=st[s['name']:e].decode('latin1')
        if s['typ']!=8: # not NOBITS
            s['data']=d[s['offset']:s['offset']+s['size']]
        else:
            s['data']=b''
    return d,secs

def main(a,b):
    da,sa=load(a); db,sb=load(b)
    ma={s['sname']:s for s in sa}; mb={s['sname']:s for s in sb}
    names=[]
    for s in sa:
        if s['sname'] not in names: names.append(s['sname'])
    for s in sb:
        if s['sname'] not in names: names.append(s['sname'])
    print(f"{'section':20} {'o_addr':>9} {'u_addr':>9} {'o_size':>8} {'u_size':>8} {'dsize':>6} {'first':>8} {'ndiff':>7} {'min':>8}  note")
    for n in names:
        A=ma.get(n); B=mb.get(n)
        if A is None or B is None:
            print(f"{n:20} {'MISSING' if A is None else hex(A['addr']):>9} {'MISSING' if B is None else hex(B['addr']):>9} {'-' if A is None else A['size']:>8} {'-' if B is None else B['size']:>8}  one-sided")
            continue
        x=A['data']; y=B['data']; m=min(len(x),len(y))
        first=None; nd=0
        for i in range(m):
            if x[i]!=y[i]:
                nd+=1
                if first is None: first=i
        print(f"{n:20} {A['addr']:#09x} {B['addr']:#09x} {A['size']:#8x} {B['size']:#8x} {B['size']-A['size']:+6d} {('-' if first is None else hex(first)):>8} {nd:7d} {m:#8x}  {'NOBITS' if A['typ']==8 else ''}")

if __name__=='__main__': main(sys.argv[1],sys.argv[2])
```

### `/tmp/dynsym.py`

```python
#!/usr/bin/env python3
import struct,sys,difflib

def secs(d):
 e_shoff=struct.unpack_from('<I',d,0x20)[0]; e_shentsize=struct.unpack_from('<H',d,0x2e)[0]; e_shnum=struct.unpack_from('<H',d,0x30)[0]; e_shstrndx=struct.unpack_from('<H',d,0x32)[0]
 S=[]
 for i in range(e_shnum):
  name,typ,flags,addr,offset,size,link,info,align,entsize=struct.unpack_from('<IIIIIIIIII',d,e_shoff+i*e_shentsize)
  S.append(dict(name=name,typ=typ,addr=addr,offset=offset,size=size,link=link,entsize=entsize,idx=i))
 st=d[S[e_shstrndx]['offset']:S[e_shstrndx]['offset']+S[e_shstrndx]['size']]
 for s in S:
  e=st.find(b'\0',s['name']); s['sname']=st[s['name']:e].decode()
 return S

def dump(path):
 d=open(path,'rb').read(); S=secs(d); M={s['sname']:s for s in S}
 ds=M['.dynsym']; st=M['.dynstr']; strtab=d[st['offset']:st['offset']+st['size']]
 def g(n):
  e=strtab.find(b'\0',n); return strtab[n:e].decode('utf-8','replace')
 out=[]
 for i in range(ds['size']//16):
  name,value,size,info,other,shndx=struct.unpack_from('<IIIBBH',d,ds['offset']+i*16)
  out.append(dict(idx=i,name=g(name),value=value,size=size,info=info,other=other,shndx=shndx,bind=info>>4,typ=info&0xf))
 return out
if __name__=='__main__':
 for s in dump(sys.argv[1]): print(s['idx'],s['bind'],s['typ'],s['shndx'],hex(s['value']),s['size'],s['name'],sep='\t')
```

### `/tmp/exidx.py`

```python
#!/usr/bin/env python3
import struct,sys

def load(path):
 d=open(path,'rb').read()
 e_shoff=struct.unpack_from('<I',d,0x20)[0]; es=struct.unpack_from('<H',d,0x2e)[0]; en=struct.unpack_from('<H',d,0x30)[0]; si=struct.unpack_from('<H',d,0x32)[0]
 S=[]
 for i in range(en):
  name,typ,flags,addr,off,size,link,info,align,entsize=struct.unpack_from('<IIIIIIIIII',d,e_shoff+i*es)
  S.append(dict(name=name,typ=typ,addr=addr,offset=off,size=size,link=link))
 st=d[S[si]['offset']:S[si]['offset']+S[si]['size']]
 for s in S:
  e=st.find(b'\0',s['name']); s['sname']=st[s['name']:e].decode()
 return d,{s['sname']:s for s in S}

def sx31(w):
 v=w & 0x7fffffff
 if w & 0x40000000: v-=0x80000000
 return v

def funcs(d,S):
 ds=S['.dynsym']; st=S['.dynstr']; strtab=d[st['offset']:st['offset']+st['size']]
 out=[]
 for i in range(ds['size']//16):
  name,val,size,info,other,shndx=struct.unpack_from('<IIIBBH',d,ds['offset']+i*16)
  e=strtab.find(b'\0',name); nm=strtab[name:e].decode('utf-8','replace')
  if (info&0xf)==2 and shndx!=0:
   out.append((val&~1,size,nm))
 return out

def entries(path):
 d,S=load(path); ex=S['.ARM.exidx']; exd=d[ex['offset']:ex['offset']+ex['size']]
 fs=funcs(d,S)
 byaddr={}
 for v,sz,n in fs: byaddr[v]=n
 # map addr to containing func if exact miss
 addrs=sorted(byaddr)
 import bisect
 def name_for(a):
  if a in byaddr: return byaddr[a]
  i=bisect.bisect_right(addrs,a)-1
  if i>=0 and a<addrs[i]+ (dict((v,s) for v,s,n in fs).get(addrs[i],0)):
   return byaddr[addrs[i]]+f'+{a-addrs[i]:#x}'
  return f'?{a:#x}'
 out=[]
 for i in range(ex['size']//8):
  ea=ex['addr']+i*8
  w1,w2=struct.unpack_from('<II',exd,i*8)
  fn=ea+sx31(w1)
  if w2==1:
   kind='CANT'; ext=None
  elif w2 & 0x80000000:
   kind='compact'; ext=None
  else:
   ext=ea+4+sx31(w2); kind='extab'
  out.append(dict(i=i,ea=ea,fn=fn,name=name_for(fn),w1=w1,w2=w2,kind=kind,ext=ext))
 return d,S,out

if __name__=='__main__':
 for tag,p in [('orig',sys.argv[1]),('ours',sys.argv[2])]:
  d,S,E=entries(p)
  print(tag,'entries',len(E),'exidx addr',hex(S['.ARM.exidx']['addr']),'size',hex(S['.ARM.exidx']['size']))
  from collections import Counter
  print(' kinds',Counter(e['kind'] for e in E))
  print(' first10',[(e['i'],e['name'],e['kind']) for e in E[:10]])
  print(' last10',[(e['i'],e['name'],e['kind']) for e in E[-10:]])
```

### `/tmp/exidx_match2.py`

```python
#!/usr/bin/env python3
import sys,struct,bisect
sys.path.insert(0,'/tmp')
from exidx import entries,load
from dynsym import dump

def gapmap(po,pu):
    O=[s for s in dump(po) if s['typ']==2 and s['shndx']!=0]
    U=[s for s in dump(pu) if s['typ']==2 and s['shndx']!=0]
    um={s['name']:s for s in U}
    pts=[]
    for s in sorted(O,key=lambda x:x['value']):
        if s['name'] in um: pts.append((s['value'],um[s['name']]['value']-s['value']))
    return pts

def delta_at(pts,a):
    i=bisect.bisect_right(pts,(a,10**9))-1
    return pts[i][1] if i>=0 else pts[0][1]

def eb(d,S,addr,n=16):
    if addr is None: return b''
    for s in S.values():
        if s['addr']<=addr<s['addr']+s['size'] and s['typ']!=8:
            return d[s['offset']+addr-s['addr']:s['offset']+addr-s['addr']+n]
    return b''

def main(po,pu):
    do,So,Eo=entries(po); du,Su,Eu=entries(pu)
    pts=gapmap(po,pu)
    from collections import defaultdict
    byname=defaultdict(list); byaddr=defaultdict(list)
    for e in Eu: byname[e['name']].append(e); byaddr[e['fn']].append(e)
    matched=set(); diffs=[]; missing=[]
    for e in Eo:
        cand=[]
        # name match first
        if e['name'] in byname and not e['name'].startswith('?'):
            cand=[c for c in byname[e['name']] if c['i'] not in matched]
        if not cand:
            mapped=e['fn']+delta_at(pts,e['fn'])
            # search within +-32
            for a in range(mapped-32,mapped+33,2):
                cand += [c for c in byaddr.get(a,[]) if c['i'] not in matched]
        pick=None
        for c in cand:
            if c['kind']==e['kind']: pick=c; break
        if pick is None and cand: pick=cand[0]
        if pick is None:
            missing.append((e, e['fn']+delta_at(pts,e['fn']))); continue
        matched.add(pick['i'])
        eo=eb(do,So,e['ext']); eu=eb(du,Su,pick['ext'])
        if e['kind']!=pick['kind'] or eo!=eu:
            diffs.append((e,pick,eo,eu))
    extra=[e for e in Eu if e['i'] not in matched]
    print('orig',len(Eo),'ours',len(Eu),'matched',len(matched),'missing',len(missing),'extra',len(extra),'diffs',len(diffs))
    print('\nMISSING:')
    for e,m in missing:
        print(f"  idx {e['i']} {e['name']} orig {e['fn']:#x} mapped {m:#x} kind {e['kind']} w2={e['w2']:#x} ext={e['ext'] and hex(e['ext'])}")
    print('\nEXTRA:')
    for e in extra:
        print(f"  idx {e['i']} {e['name']} ours {e['fn']:#x} kind {e['kind']} w2={e['w2']:#x} ext={e['ext'] and hex(e['ext'])}")
    print('\nDIFFS:')
    for e,p,eo,eu in diffs:
        print(f"  {e['name']} orig {e['fn']:#x} idx {e['i']} {e['kind']} ext={e['ext'] and hex(e['ext'])} | ours {p['fn']:#x} idx {p['i']} {p['kind']} ext={p['ext'] and hex(p['ext'])}")
        print(f"    extab {eo.hex()} vs {eu.hex()}")
if __name__=='__main__': main(sys.argv[1],sys.argv[2])
```

### `/tmp/data_audit.py`

```python
#!/usr/bin/env python3
import struct,sys
sys.path.insert(0,'/tmp')
from dynsym import dump,secs

def relocs(path):
 d=open(path,'rb').read(); S=secs(d); M={s['sname']:s for s in S}
 ds=M['.dynsym']; st=M['.dynstr']; strtab=d[st['offset']:st['offset']+st['size']]
 def g(n):
  e=strtab.find(b'\0',n); return strtab[n:e].decode('utf-8','replace')
 out=[]
 for secname in ('.rel.dyn','.rel.plt'):
  s=M.get(secname)
  if not s: continue
  for i in range(s['size']//8):
   off,info=struct.unpack_from('<II',d,s['offset']+i*8)
   sym=info>>8; typ=info&0xff
   out.append(dict(sec=secname,off=off,sym=sym,typ=typ,name=g(ds['offset'] and struct.unpack_from('<I',d,ds['offset']+sym*16)[0]) if sym else ''))
 return out

def objs(path):
 out=[]
 for s in dump(path):
  if s['typ']==1 and s['shndx']!=0:
   out.append(s)
 return out

def secmap(path):
 d=open(path,'rb').read(); S=secs(d); return d,{s['sname']:s for s in S}

def main(a,b):
 da,Ma=secmap(a); db,Mb=secmap(b)
 oa={s['name']:s for s in objs(a)}; ob={s['name']:s for s in objs(b)}
 ra=relocs(a); rb=relocs(b)
 # map reloc by target address
 def rmap(rels):
  m={}
  for r in rels: m.setdefault(r['off'],[]).append(r)
  return m
 rma=rmap(ra); rmb=rmap(rb)
 for secname in ('.data.rel.ro','.data','.rodata'):
  A=Ma[secname]; B=Mb[secname]
  print('===',secname,'orig',hex(A['addr']),hex(A['size']),'ours',hex(B['addr']),hex(B['size']))
  names=sorted(set(oa)&set(ob), key=lambda n:(oa[n]['value'],n))
  offdiff=[]; sizediff=[]; content=[]; reldiff=[]
  for n in names:
   x=oa[n]; y=ob[n]
   if x['shndx']!=A['idx'] or y['shndx']!=B['idx']: continue
   ox=x['value']-A['addr']; oy=y['value']-B['addr']
   if ox!=oy: offdiff.append((n,ox,oy,x['size'],y['size']))
   if x['size']!=y['size']: sizediff.append((n,ox,oy,x['size'],y['size']))
   # content compare
   if x['size']==y['size'] and x['size']>0:
    bx=da[A['offset']+ox:A['offset']+ox+x['size']]
    by=db[B['offset']+oy:B['offset']+oy+y['size']]
    # mask relocation target words
    mask=set()
    for addr in range(x['value'],x['value']+x['size'],4):
     if addr in rma: mask.add(addr-x['value'])
    for addr in range(y['value'],y['value']+y['size'],4):
     if addr in rmb: mask.add(addr-y['value'])
    diffs=[i for i in range(len(bx)) if bx[i]!=by[i] and (i//4 not in mask)]
    if diffs: content.append((n,ox,oy,x['size'],len(diffs),diffs[:8]))
   # relocation compare by object-relative offset
   rx={}; ry={}
   for addr in range(x['value'],x['value']+x['size'],4):
    for r in rma.get(addr,[]): rx[addr-x['value']]=(r['typ'],r['name'])
   for addr in range(y['value'],y['value']+y['size'],4):
    for r in rmb.get(addr,[]): ry[addr-y['value']]=(r['typ'],r['name'])
   if rx!=ry:
    keys=sorted(set(rx)|set(ry))
    dd=[(k,rx.get(k),ry.get(k)) for k in keys if rx.get(k)!=ry.get(k)]
    reldiff.append((n,ox,oy,x['size'],dd[:8],len(dd)))
  print(' objects',len(names),'offset-diff',len(offdiff),'size-diff',len(sizediff),'content-diff',len(content),'reloc-diff',len(reldiff))
  print(' first offset diffs:')
  for z in offdiff[:10]: print('   ',z)
  print(' size diffs:')
  for z in sizediff[:20]: print('   ',z)
  print(' content diffs (non-reloc):')
  for z in content[:20]: print('   ',z)
  print(' reloc diffs:')
  for z in reldiff[:20]: print('   ',z)

if __name__=='__main__': main(sys.argv[1],sys.argv[2])
```

### `/tmp/gapwalk.py`

```python
#!/usr/bin/env python3
import struct, sys

def load(path):
    d = open(path, 'rb').read()
    e_shoff = struct.unpack_from('<I', d, 0x20)[0]
    e_shentsize = struct.unpack_from('<H', d, 0x2e)[0]
    e_shnum = struct.unpack_from('<H', d, 0x30)[0]
    e_shstrndx = struct.unpack_from('<H', d, 0x32)[0]
    secs = []
    for i in range(e_shnum):
        off = e_shoff + i * e_shentsize
        name, typ, flags, addr, offset, size, link, info, align, entsize = struct.unpack_from('<IIIIIIIIII', d, off)
        secs.append(dict(name=name, addr=addr, offset=offset, size=size))
    shstr = secs[e_shstrndx]
    st = d[shstr['offset']:shstr['offset'] + shstr['size']]
    for s in secs:
        e = st.find(b'\0', s['name'])
        s['sname'] = st[s['name']:e].decode()
    return d, secs

def funcs(path):
    d, secs = load(path)
    byname = {s['sname']: s for s in secs}
    ds, st = byname['.dynsym'], byname['.dynstr']
    strtab = d[st['offset']:st['offset'] + st['size']]
    def gstr(n):
        e = strtab.find(b'\0', n)
        return strtab[n:e].decode('utf-8', 'replace')
    text = byname['.text']
    t0, t1 = text['addr'], text['addr'] + text['size']
    out = []
    for i in range(ds['size'] // 16):
        off = ds['offset'] + i * 16
        name, value, size, info, other, shndx = struct.unpack_from('<IIIBBH', d, off)
        typ, bind = info & 0xf, info >> 4
        if typ != 2 or shndx == 0 or bind == 0:
            continue
        v = value & ~1
        if t0 <= v < t1:
            out.append(dict(name=gstr(name), value=v, size=size))
    return out

fo, fu = funcs(sys.argv[1]), funcs(sys.argv[2])
om = {s['name']: s for s in fo}
um = {s['name']: s for s in fu}
common = sorted([s for s in fo if s['name'] in um], key=lambda x: (x['value'], x['name']))
last = None
rows=[]
for i, s in enumerate(common):
    u = um[s['name']]
    delta = u['value'] - s['value']
    if last is None or delta != last:
        if i:
            ps, pu = common[i - 1], um[common[i - 1]['name']]
            go = s['value'] - (ps['value'] + ps['size'])
            gu = u['value'] - (pu['value'] + pu['size'])
            rows.append((i,last,delta,s['name'],ps['name'],ps['value'],ps['size'],pu['value'],pu['size'],go,gu,gu-go))
        else:
            rows.append((0,None,delta,s['name'],None,0,0,0,0,0,0,0))
        last = delta
for r in rows:
    i,ld,delta,name,pn,pv,psz,uv,usz,go,gu,gd=r
    print(f"DELTA {ld} -> {delta:+d} at {name}")
    if pn:
        print(f"   prev {pn} orig {pv:#x}+{psz:#x} ours {uv:#x}+{usz:#x} gap_orig={go:#x} ({go}) gap_ours={gu:#x} ({gu}) gap_delta={gd:+d}")
print('rows',len(rows))
```
