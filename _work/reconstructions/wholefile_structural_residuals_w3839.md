# Whole-file structural residuals: `.missioncase`, `.shstrtab`, `.rel.plt`, CMake layout OFF (w3839)

Status: landed source/build fix + audit. Worktree `wt/w3839`, branch `wave89-w3839`,
based on master `5710f6d0`. Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000; real VMA = Ghidra - 0x10000).

## TL;DR

| residual | classification | action |
|---|---|---|
| `.missioncase` (28 vs 27 sections) | **real source difference**: invented 192-byte `kMissionCase` rank table in `Status.cpp` | removed; direct `(short) m->getType()` switch restored |
| `.shstrtab` 0x124 vs 0x117 | **consequence of `.missioncase`**: the string list is the original's plus `.missioncase` | now byte-identical (0x117, 0 differing bytes) |
| `.rel.plt` flags `AI`/`sh_info=20` vs `A`/0 | **toolchain artifact**: gold 1.12 writes `A`/0; GNU bfd/strip writes `AI`/`<.got index>` | not fixable in source; left alone |
| `.rel.plt` content (21 differing bytes) | **real source difference**: 6-entry PLT-slot rotation from `LevelScript::process` body layout | unchanged (same as w3695) |
| `GOF2_MATCH_LAYOUT=OFF` build | **real CMake bug**: `${_order}` was an unconditional dependency but only generated when ON | fixed; OFF build links now |

Gate before and after: **byte 4498 / linked 4498**, `.text 0x1337c8 @ 0x69440`,
`data_exact 843/843`, `sodiff` 0 extra / 0 missing, imports 207, `RATCHET PASS`,
lint `CLEAN ✓`, drift 0. The only metric movement is
`Status::missionCompleted` **86.7% -> 57.7%** (1432 B, size unchanged); `avg_match`
stays 99.9. No baseline update was needed.

## 1. `.missioncase` / `.shstrtab`

The tree carried a 192-byte `kMissionCase[192]` type->rank permutation in its own
ELF section (`.missioncase`, 0xc0 bytes at 0x20b977, immediately after `.rodata`):

```cpp
#pragma clang section rodata=".missioncase"
extern const unsigned char kMissionCase[192] = { ... };
#pragma clang section rodata=""
```

The original has no such section and no such table. The table is a codegen-shaping
device: it makes clang 7's ascending jump-table order coincide with the original's
source-order case-body layout (see `status-missioncompleted-86-7-1432-b_w3810.md`
and `Status-missionCompleted-no-rank-table-w3590.md`).

Removing the table and switching directly on `(short) m->getType()` with the real
mission-type case labels:

* keeps `.text` size/address exact (`0x1337c8 @ 0x69440`) and the gate at
  **byte 4498 / linked 4498**;
* removes the extra section: **28 -> 27 sections** (original: 27);
* makes `.shstrtab` **byte-identical**: size **0x124 -> 0x117**, 0 differing bytes
  (the string list was exactly the original's plus `.missioncase` at offset 0xca);
* drops `Status::missionCompleted` from 86.7% to 57.7% (1432 B, size unchanged).

The 57.7% is the faithful direct-switch form: the original has no table load,
no `sxth`, and two in-text TBH tables (40 + 7 entries). The remaining delta is
clang 7's `MachineBlockPlacement` following the value-sorted jump-table successor
order; no source ordering or tested `-mllvm` flag reproduces the original's
source-order body layout without a rank table (w3590). The rank table remains
available as a future anchor if the `Mission::id` rank semantics are recovered
cross-TU.

## 2. `.rel.plt` header flags (`AI`/20 vs `A`/0)

Both files have the same `.rel.plt` size (`0x72d0`), `sh_link=3` (`.dynsym`), and
the same `r_offset` sequence. The section **header** differs:

```
original: sh_flags=0x42 (SHF_ALLOC|SHF_INFO_LINK)  sh_info=20 (.got)
ours:     sh_flags=0x02 (SHF_ALLOC)                sh_info=0
```

This is a linker/strip artifact, not a source/TU difference:

* gold 1.12 creates `.rel.plt` with `SHF_ALLOC` and `sh_info=0` on ARM
  (`Output_data_plt_arm` calls `add_output_section_data(".rel.plt", SHT_REL,
  SHF_ALLOC, ...)` and never calls `set_info_section`). The same is true of the
  NDK r18b **darwin** gold (verified by disassembling its
  `Output_data_plt_arm` constructor and `Output_section::write_header`).
* GNU bfd (and GNU `strip`/`objcopy`) sets `SHF_INFO_LINK` and `sh_info` to the
  `.got` section index. On our gold `-s` output, GNU strip produces exactly
  `AI`/`<.got index>` (21 with `.missioncase`, 20 without it).
* Our single plain gold link therefore cannot reproduce the original's header
  without a post-link/linker trick, which the project forbids.

The original's `.shstrtab` is gold `-s` Stringpool order, while its `.rel.plt`
header is bfd/strip style; the original build evidently used a strip/link path
that we do not reproduce. No source change can affect this field.

The 21 differing `.rel.plt` **content** bytes are a separate, real source
difference: the same 6-entry PLT-slot rotation documented in
`nontext_relplt_shstrtab_w3695.md` (`PlayerEgo::deleteHackingGame` first in ours,
last in the original). It is caused by the `LevelScript::process` block layout
and will follow when that body matches.

## 3. `GOF2_MATCH_LAYOUT=OFF` CMake bug

`CMakeLists.txt` always listed `${_order}` (`section_order.txt`) in the
`libgof2hdaa.so` `DEPENDS`, but the rule that generates it exists only inside
`if(GOF2_MATCH_LAYOUT)`. With `-DGOF2_MATCH_LAYOUT=OFF` the build failed:

```
gmake[3]: *** No rule to make target 'section_order.txt', needed by 'libgof2hdaa.so'.  Stop.
```

Fix: set `_order_dep` to `${_order}` in the ON branch and to empty in the OFF
branch, and use `${_order_dep}` in `DEPENDS`. Verified:

```sh
cmake -S . -B cmake-build-noorder -DGOF2_MATCH_LAYOUT=OFF \
      -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
cmake --build cmake-build-noorder --target link_so -j8   # succeeds
```

The default ON build is unchanged (gate 4498/4498).

## 4. Remaining whole-file deltas after this pass

Sections are now the original's 27, with matching names/order. Section sizes and
bases match except the known `.ARM.exidx`/`.ARM.extab` pair:

| section | orig | ours | note |
|---|---|---|---|
| `.text` | 0x1337c8 @ 0x69440 | same | size/base exact; 107,444 differing bytes (code/addressing) |
| `.rodata` | 0x41b67 @ 0x1c9e10 | same | **0 differing bytes** |
| `.data` | 0x296c @ 0x214000 | same | **0 differing bytes** |
| `.data.rel.ro` | 0x2fe8 @ 0x20cbb0 | same | **0 differing bytes** |
| `.plt` / `.got` | — | same | **0 differing bytes** |
| `.dynamic` / `.init_array` / `.fini_array` | — | same | **0 differing bytes** |
| `.shstrtab` | 0x117 | 0x117 | **0 differing bytes** |
| `.ARM.exidx` | 0x6f70 @ 0x19cc08 | 0x6f78 | +8: the `layoutCtorPad` CANTUNWIND sentinel |
| `.ARM.extab` | 0x26298 @ 0x1a3b78 | 0x2628c @ 0x1a3b80 | -12: mismatched bodies |
| `.dynsym` / `.dynstr` / `.hash` | — | same size | order only (gold hash-map iteration) |
| `.rel.dyn` / `.rel.plt` | — | same size | order/address only; `.rel.plt` header flags as above |
| `.note.gnu.build-id` | — | — | content (build-id) |

The remaining `.text`/`.ARM.exidx`/`.ARM.extab` deltas are the known
per-function bodies (`Layout::Layout` + `layoutCtorPad`, `LevelScript::process`,
`Generator::createMission`, `HangarWindow::render`, `Hud::draw`, `StarMap::draw`,
`Level::createShip`, `RecordHandler::recordStoreWrite`, and the `.dynsym` order
that follows from them). `.missioncase` is no longer one of them.

## 5. Reproduction

```sh
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint -j8
cmake --build cmake-build-match --target drift -j8

OD=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-readelf
$OD -SW /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so | grep -E 'missioncase|shstrtab|rel.plt'
$OD -SW cmake-build-match/libgof2hdaa.so | grep -E 'missioncase|shstrtab|rel.plt'

# OFF build
cmake -S . -B cmake-build-noorder -DGOF2_MATCH_LAYOUT=OFF \
      -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
cmake --build cmake-build-noorder --target link_so -j8
```
