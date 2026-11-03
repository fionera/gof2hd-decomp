# ELF tail byte-identity: `.dynsym` / `.dynstr` / `.hash` / `.rel.dyn` / `.rel.plt` (w4219)

Status: research note + landed TU reconstruction (2026-11-03), worktree `wt/w4219`,
branch `wave92-w4219`, base master `b379d00d`. Every number below is measured against
`../_work/bins/android_2.0.16_libgof2hdaa.so` (image base 0x10000) or a controlled
rebuild of this tree with the repo's NDK r18b gold 1.12. No baseline was updated to
absorb a regression; the only baseline edit is a same-total `operator_call` relocation
(see §5).

## TL;DR

The `.dynsym` 324-byte, `.dynstr` 1268-byte, `.hash` 133-byte and `.rel.dyn` 74-byte
deltas were **all one root cause**: the original `NdkHooks.cpp` TU owns
`ndk_setNativeItemInformationList`, `pConstToNonConst`, `getStringUTFChars`,
`releaseStringUTFChars` and `ndk_resetNativeItemInformationList`; our tree had split
them into `NdkAutosave.cpp` / `LODManager.cpp`. Because gold emits `.dynsym` in
first-mention order and each object's global `.symtab` is lexicographic, that split
rotated a 26-symbol block. Re-merging the functions into `NdkHooks.cpp` and moving the
touch API (`AddTouch`, `GetTouchCount`, `RemoveTouches`, `GetTouch`) into the
touch-globals TU (`NdkAutosave.cpp`) makes all four sections **byte-identical** and
also fixes the `.ARM.exidx` empty-`.text` sentinel positions.

| section | before diff | after diff |
|---|---:|---:|
| `.dynsym` | 324 | **0** |
| `.dynstr` | 1268 | **0** |
| `.hash` | 133 | **0** |
| `.rel.dyn` | 74 | **0** |
| `.rel.plt` | 21 | **12** |
| `.ARM.exidx` | 1760 | **1728** |
| `.text` | 106587 | 106587 (unchanged) |
| `.ARM.extab` | 133769 (size −8) | 133769 (size −8) |
| `.note.gnu.build-id` | 20 | 20 |

Gate is unchanged: **byte_exact 4498/4524, linked_exact 4498/4524, size-exact
4524/4524, stub 0, missing 0, wrong_type 0, extra 0, data 843/843, imports 207,
`.text` 0x1337c8, `.rodata` 0x41b67, `.ARM.exidx` 0x6f70, `.ARM.extab` 0x26290 (−8),
lint CLEAN, drift 0.**

The remaining `.rel.plt` 12 bytes are the known `PlayerEgo::deleteHackingGame`
PLT-slot rotation, which is a `LevelScript::process` block-order issue, not a
symbol/link-order issue (§4). The remaining section-header deltas are 5 bytes of
bfd-strip normalization that no available single tool reproduces together with gold's
`.shstrtab` order (§5).

## 1. Baseline and method

```
cmake --preset default
cmake -S . -B cmake-build-match \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8
```

Baseline (`19cd3a7d`, clean):

```
verify: byte 4498  linked 4498  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓   drift: 0 field(s) across 0 class(es)   .text 0x1337c8
```

Per-section differing-byte helper (reads both `.so` files directly):

```python
import sys
sys.path.insert(0, 'tools/verify')
from elf import Elf32
O = '/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so'
U = 'cmake-build-match/libgof2hdaa.so'
def sec(p, n):
    e = Elf32(p); s = next(x for x in e.sections if x['name'] == n)
    return e.data[s['offset']:s['offset']+s['size']]
eo, eu = Elf32(O), Elf32(U)
for s in eo.sections:
    su = next(x for x in eu.sections if x['name'] == s['name'])
    if s['type'] == 8:
        print('%-20s size %d/%d (nobits)' % (s['name'], s['size'], su['size'])); continue
    a, b = sec(O, s['name']), sec(U, s['name'])
    print('%-20s size %7d/%7d diff %6d' % (s['name'], len(a), len(b),
          sum(x != y for x, y in zip(a, b)) + abs(len(a)-len(b))))
```

## 2. Root cause: the `NdkHooks` TU membership split

### 2.1 The `.dynsym` symptom

`Elf32._symbols(SHT_DYNSYM)` over the 5370 defined symbols: before the change
5314/5370 were at the same index, with one contiguous 26-symbol block displaced.
Printing defined-symbol ordinals 1820..1904 side by side showed:

* original at 1837: `_ZN7Globals14cItemListID_00E` .. `_ZN7Globals23cItemListDescription_04E`
  (25 symbols), then `_ZN7Globals5soundE`, `_ZN7Globals6statusE`, `_ZN7Globals7optionsE`,
  `checkFirstCreditPackBoughtWriteAction`, `currentstate`, `forceExit`, `gEngine`,
  `gRealHeight`, `gRealWidth`, `getStringUTFChars`, `hrwhs_country_code`, `loadAPK`,
  `loadAPKAndZip`, `ndk23_*`, `ndkEntrance`, `ndk_autosave`, `pConstToNonConst`,
  `releaseStringUTFChars`, `rootDirectory`, `rotateAccelValues`, `setBaughtCredits`,
  `sound_off`, `stateChanged`, `zip_get_name`, `zip_get_num_files`, `zip_open`;
* ours at 1837: the `sound` .. `zip_open` block first, then the `cItemList` block,
  then `curTouchSize`, `getStringUTFChars`, `maxTouchSize`, `pConstToNonConst`,
  `releaseStringUTFChars`, `touches`.

### 2.2 Why: gold's first-mention order

Gold emits `.dynsym` by iterating its `Symbol_table` hash map, whose key is the
1-based namepool insertion index (`name_key ^ version_key`); for unversioned symbols
that is exactly **first-mention order over the link inputs**, and each object's global
`.symtab` is lexicographic by mangled name. This is the model established in
`_work/reconstructions/dynsym_dynstr_hash_order_w3567.md`; it reproduces our table
5577/5577.

The original's block above is exactly the lexicographic order of one object's globals:
`_ZN7Globals14cItemList*` (`_Z...`, '1' < '5' < '6' < '7') then the lowercase names
`checkFirst...`, `currentstate`, `forceExit`, `gEngine`, `gRealHeight`, `gRealWidth`,
`getStringUTFChars`, `hrwhs_country_code`, `loadAPK`, `loadAPKAndZip`, `ndk23_*`,
`ndkEntrance`, `ndk_autosave`, `pConstToNonConst`, `releaseStringUTFChars`,
`rootDirectory`, `rotateAccelValues`, `setBaughtCredits`, `sound_off`, `stateChanged`,
`zip_*`. In our tree `NdkHooks.cpp` owned the `sound..zip_open` half but not the
`cItemList` / helper half, which lived in `NdkAutosave.cpp`; `NdkAutosave.cpp` is
linked immediately after `NdkHooks.cpp`, so its lexicographic block landed after
`zip_open`.

### 2.3 The `.text` / `.ARM.exidx` witness

The original's `.text` function order (from the original `.dynsym` function values)
shows the same TU:

```
0x9462c setBaughtCredits
0x94708 checkFirstCreditPackBoughtWriteAction
0x94770 ndk_autosave
0x947ac ndk_getCurrentApplicationModule
0x947d0 ndk_getLogoShown
0x947e0 ndk_isInMainMenu
0x947f0 pConstToNonConst
0x94818 getStringUTFChars
0x94828 releaseStringUTFChars
0x94858 ndk_setNativeItemInformationList
0x94e38 ndk_resetNativeItemInformationList
0x950dc _Z8AddTouchiiii
0x95150 _Z13GetTouchCountv
0x95160 _Z13RemoveTouchesv
0x95170 _Z8GetTouchi
0x9518c _ZN10LODManagerC1Ev
```

Our `.text` function addresses were already identical, but the **empty-`.text`
CANTUNWIND sentinels** (gold's `fix_exidx_coverage` marker for the plain `.text`
section every `-ffunction-sections` object carries) were at the wrong addresses:

```
before: only-ours sentinels 0x947f0, 0x94e38 ; only-orig sentinels 0x950dc, 0x9518c
after : only-ours sentinels (none)          ; only-orig sentinels (none)
```

i.e. the original's object boundaries are `NdkHooks` (ending at
`ndk_resetNativeItemInformationList`, 0x94e38..0x950dc) → touch TU (starting at
`AddTouch`, 0x950dc) → `LODManager` (starting at `LODManager::LODManager`, 0x9518c).
The 32-byte `.ARM.exidx` improvement is exactly this sentinel realignment.

### 2.4 The change

* `src/platform/android/NdkHooks.cpp`: append (after `ndk_isInMainMenu`, inside the
  existing `extern "C"` block) `pConstToNonConst`, `getStringUTFChars`,
  `releaseStringUTFChars`, `ndk_setNativeItemInformationList`, and
  `ndk_resetNativeItemInformationList` (moved from `LODManager.cpp`). Added
  `#include <new>` for the `operator delete` calls.
* `src/platform/android/NdkAutosave.cpp`: now owns only the touch globals
  (`touches`, `maxTouchSize`, `curTouchSize`) and the touch API
  (`AddTouch`, `GetTouchCount`, `RemoveTouches`, `GetTouch`, moved from
  `LODManager.cpp`); it includes `engine/render/LODManager.h` and `<cstdlib>`.
* `src/engine/render/LODManager.cpp`: keeps only `LODManager` (plus the
  `TouchBufferInit` static and the `extern` touch declarations).

Function order is preserved exactly, so every `.text` address is unchanged and
`.text` stays 0x1337c8. The `gen_link_order.py` override that pins
`NdkAutosave.cpp` immediately after `NdkHooks.cpp` is still correct.

Measured after the change (full `check`):

```
verify: byte 4498  linked 4498  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓   drift: 0 field(s) across 0 class(es)   .text 0x1337c8
```

`.dynsym` defined-symbol sequence is now 5370/5370 at the same index and the
undefined sequence 208/208.

## 3. `.rel.dyn` follows `.dynsym`

`.rel.dyn` was a pure permutation by name: the multiset of `(r_offset, name, type)`
matched but the entry order and `r_info` indices did not. The 39 differing entries
were all `R_ARM_GLOB_DAT` in `.data.rel.ro` (0x210134..0x2101ec), and their order was
exactly the `.dynsym` first-mention order (`cItemList*` block then `sound/status/
options/...` in the original; the reverse in ours). With `.dynsym` exact, `.rel.dyn`
is 0 differing bytes and 3593/3593 entries match `(offset, type, name)` and `r_info`.

## 4. `.rel.plt`: 21 -> 12 bytes, the `deleteHackingGame` rotation

Eight of the 21 bytes were downstream of the `.dynsym` index shift (same names,
different `r_info` symbol index for `zip_open`, `zip_get_name`,
`zip_get_num_files`, `loadAPK`, `loadAPKAndZip`, `ndk_autosave`, `setBaughtCredits`,
`checkFirstCreditPackBoughtWriteAction`). After the TU fix, `.rel.plt` has exactly
**6 differing entries / 12 bytes**, one cyclic shift:

```
idx 2831..2836  orig: switchPlanetForIntro, switchSkyboxForIntro,
                      ParticleSystemManager::reset, startSmokeEmission,
                      PlayerStation::setVisible, deleteHackingGame
                ours: deleteHackingGame, switchPlanetForIntro, ...
```

All six symbols are referenced only from `LevelScript::process` (same function start
0x137d18, size 0xdf90 in both). In our object the relocations are

```
576c R_ARM_THM_CALL deleteHackingGame
61aa R_ARM_THM_CALL switchPlanetForIntro
61b2 R_ARM_THM_CALL switchSkyboxForIntro
61d0/61da R_ARM_THM_CALL ParticleSystemManager::reset
645c R_ARM_THM_CALL startSmokeEmission
6776 R_ARM_THM_CALL PlayerStation::setVisible
```

vs the original's single `deleteHackingGame` call at object offset 0x61bc (after the
other five, at 0x54d4..0x5d06). The emitted block around the call is byte-identical
apart from the PLT slot and the 6-byte post-switch target (ours `0x144e3c`, orig
`0x144e36`); this is a clang block-placement difference in the 57 KB
`LevelScript::process` (54% body match), not a link/symbol-order knob. It will become
byte-identical when that body's block order matches; no separate action is possible.

## 5. Section-header deltas: a bfd-strip normalization, not a gold knob

`readelf -SW` differs in exactly 6 bytes of the 27-entry section header table:

| section | field | original | ours |
|---|---|---|---|
| `.rel.plt` | `sh_flags` low byte | 0x42 (`A`+`SHF_INFO_LINK`) | 0x02 (`A`) |
| `.rel.plt` | `sh_info` low byte | 20 (`.got`) | 0 |
| `.ARM.extab` | `sh_size` low byte | 0x98 | 0x90 (the known −8) |
| `.fini_array` | `sh_entsize` | 4 | 0 |
| `.init_array` | `sh_entsize` | 4 | 0 |
| `.bss` | `sh_offset` low byte | 0x6c (end of `.data`) | 0x70 (16-aligned) |

Reproduced exactly by the NDK r18b **GNU strip 2.27** on our gold output:

```
$ arm-linux-androideabi-strip --strip-all gold.so
  [10] .rel.plt  ... 08  AI  3  20  4
  [16] .fini_array ... 04  WA ...
  [18] .init_array ... 04  WA ...
  [22] .bss NOBITS 00216970 21596c ...
```

bfd's `assign_section_numbers` sets `sh_info = d->this_idx` and `SHF_INFO_LINK` for
every reloc section (binutils-2.27 `bfd/elf.c:3692`), `elf_fake_sections` gives
`SHT_INIT_ARRAY`/`SHT_FINI_ARRAY` `sh_entsize = 4`, and NOBITS offsets are not
aligned. Gold's arm backend does none of these: `arm.cc` creates `.rel.plt` with
`SHF_ALLOC` only and `Output_data_reloc_base::do_adjust_output_section` sets only
`sh_link`/`sh_entsize`; `Output_section::write_header` adds `SHF_INFO_LINK` only when
`info_section_` is set, which gold does only for relocatable links.

**But GNU strip rewrites `.shstrtab` into section-header order** (259 differing
bytes), while the original's `.shstrtab` is byte-identical to gold `-s`'s Stringpool
order:

```
orig .shstrtab: .init_array .fini_array .ARM.exidx .text .got .comment ...
gold -s       : identical (0 differing bytes)
GNU strip     : .shstrtab .note.android.ident .note.gnu.build-id .dynsym ... (259)
```

The original therefore has **both** gold's `.shstrtab` order **and** bfd's header
normalization. No single tool in NDK r18b does both: gold `-s` preserves
`.shstrtab` but leaves the 5 header bytes; GNU `strip`/`objcopy` (2.27 and host
2.44) set the header bytes but reorder `.shstrtab`. Tested and rejected: gold
`-s`/`--strip-all` (identical), `-z now`/`-z relro`/`-z norelro`,
`--sort-section=name`, `--gc-sections`, `--icf=all`, `llvm-strip-19` (drops
`.comment`/`.note.gnu.gold-version` -> 25 sections), `arm-none-eabi-strip`/
`objcopy` 2.44 (same 259-byte `.shstrtab` rewrite). Switching to GNU strip would
trade 5 header bytes for 259 `.shstrtab` bytes, so the current gold `-s` is the
better state. The 5 bytes are recorded here as a residual tool-chain delta; the
6th header byte is the known `.ARM.extab` −8.

## 6. `.note.gnu.build-id`

20 differing bytes, a content hash of the (still non-identical) `.text`/`.ARM.extab`.
Not independently reducible; it will follow the body fixes.

## 7. Proven vs hypothesis

**Proven by measurement**

* The original `NdkHooks.cpp` object owns `cItemList*`, `sound/status/options`,
  `checkFirst...`, `getStringUTFChars`, `pConstToNonConst`,
  `releaseStringUTFChars`, `ndk_setNativeItemInformationList` and
  `ndk_resetNativeItemInformationList`: its `.dynsym` block is exactly the
  lexicographic order of that set, and its `.text` functions are contiguous
  0x9462c..0x950dc.
* The touch API (`AddTouch`, `GetTouchCount`, `RemoveTouches`, `GetTouch`) belongs
  to the touch-globals TU: the original's empty-`.text` sentinels are at 0x950dc
  and 0x9518c, and `curTouchSize`/`maxTouchSize`/`touches` are first mentioned
  after the `NdkHooks` block and before `LODManager`'s symbols.
* Moving the functions as above makes `.dynsym` 5370/5370, `.dynstr` 0,
  `.hash` 0, `.rel.dyn` 0, `.rel.plt` 12, `.ARM.exidx` 1728, with the gate
  unchanged (byte 4498 / linked 4498).
* `.rel.plt`'s remaining 12 bytes are the six-entry `deleteHackingGame` rotation
  inside `LevelScript::process`; the emitted call block is otherwise identical.
* The 5 non-`.ARM.extab` header bytes are bfd's reloc/array/NOBITS normalization,
  reproduced by NDK GNU strip 2.27; gold `-s` cannot produce them, and GNU strip
  costs 259 `.shstrtab` bytes.

**Hypothesis / open**

* The original build used a strip tool that both normalizes the section headers and
  preserves gold's Stringpool `.shstrtab` order (not present in NDK r18b). The
  observable bytes are consistent with that; the exact tool is not recoverable from
  the stripped `.so`.
* `.rel.plt` will become byte-identical exactly when `LevelScript::process`'s block
  order matches the original (the same fix that would raise its body match).

## 8. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w4219
cmake --preset default
cmake -S . -B cmake-build-match \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8

# per-section diff: the snippet in §1
# defined-symbol sequence:
python3 - <<'PY'
import sys, difflib
sys.path.insert(0, 'tools/verify')
from elf import Elf32, SHT_DYNSYM
def defined(p):
    return [s['name'] for s in Elf32(p)._symbols(SHT_DYNSYM) if s['shndx'] != 0]
o = defined('/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so')
u = defined('cmake-build-match/libgof2hdaa.so')
print('same index', sum(a == b for a, b in zip(o, u)), '/', len(o))
PY

# section-header byte diff (27 entries x 40 bytes at e_shoff):
python3 - <<'PY'
import struct
O = open('/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so','rb').read()
U = open('cmake-build-match/libgof2hdaa.so','rb').read()
shoff = struct.unpack_from('<I', O, 0x20)[0]
entsz = struct.unpack_from('<H', O, 0x2e)[0]
n     = struct.unpack_from('<H', O, 0x30)[0]
for i in range(n):
    o = shoff + i*entsz
    for j in range(entsz):
        if O[o+j] != U[o+j]:
            print('sec', i, 'byte', j, O[o+j], U[o+j])
PY

# exidx sentinel addresses
python3 - <<'PY'
import sys, struct
sys.path.insert(0, 'tools/verify')
from elf import Elf32
def prel31(x):
    v = x & 0x7fffffff
    return v - 0x80000000 if v & 0x40000000 else v
def sent(p):
    e = Elf32(p); d = e.data
    s = next(x for x in e.sections if x['name'] == '.ARM.exidx')
    out = set()
    for i in range(s['size']//8):
        w0, w1 = struct.unpack_from('<II', d, s['offset']+i*8)
        if w1 == 1:
            out.add(s['addr'] + i*8 + prel31(w0))
    return out
o = sent('/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so')
u = sent('cmake-build-match/libgof2hdaa.so')
print('only orig', [hex(x) for x in sorted(o-u)])
print('only ours', [hex(x) for x in sorted(u-o)])
PY
```
