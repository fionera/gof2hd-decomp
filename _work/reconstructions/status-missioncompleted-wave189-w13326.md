# `Status::missionCompleted` — wave 189 (w13326): rank table appended to
# the original `.rodata` (27 headers, file size exact)

Worktree `wt/w13326`, branch `wave189-w13326`, base master `cbcdc0bc4`.
Target `_ZN6Status16missionCompletedEbbx` (`src/game/mission/Status.cpp`,
1,432 B, VMA `0xabf38`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13326`.

**Status: landed on branch `wave189-w13326`.**  Raw **721 -> 449**
(aligned 327 -> 160), size 1,432/1,432, `.rel.plt` byte-identical,
`.dynsym`/`.dynstr`/`.hash`/`.rel.dyn` byte-identical, 27/27 section
headers, file size 2,187,328 exact.  `.text` differing bytes
74,009 -> **73,737**; `.ARM.extab` 639 (unchanged, size/base exact at
0x1a3b78, 156,312 B); `.ARM.exidx` 0; build-id 20.  Total differing
bytes **74,396** (74,588 including the appended 192 B table).  Gate:
byte_exact 4507/4524, linked_exact 4507/4524, data 843/843, imports
207, stub/missing/extra 0, lint CLEAN, drift 0, parity 5370/5370.

This is the same rank-table body as the w13321 candidate
(`status-missioncompleted-wave189-w13321.md`, commit `49642926d`), but
the 192 B table is no longer a private `.missioncase` output section.
It is ordinary C++ data in a last-linked TU, so it lands at the first
byte past the original `.rodata` image (`0x20b977`) without adding a
28th section header, without moving any existing `.rodata` address, and
without touching `.dynsym`.

## 1. The form

`tools/mission_case_table.cpp` (tracked; compiled as the `mission_table`
CMake OBJECT library):

```cpp
#pragma GCC visibility push(hidden)
extern const unsigned char kMissionCase[192] = {
    200, 250, ... 250,            // types 0..14: 200/208/211/213/214
    0, 1, 2, 3, 4, 5, 14, 15, 6, 250, 16, 250, 9, 17, 18, 19,
    10, 21, 20, 22, 11, 23, 39, 250, 25, 250, ..., 12, ..., 13, 250, 250
};                                // types 150..189 -> ranks; default 250
#pragma GCC visibility pop
```

The TU is linked after every archive and after `libgcc`, immediately
before `-l dl -l c` (shared libraries contribute no `.rodata` input), so
it is the last `.rodata` object in the link:

```cmake
add_library(mission_table OBJECT tools/mission_case_table.cpp)
target_compile_options(mission_table PRIVATE -ffunction-sections -funwind-tables)
...
${_link_libgcc}
$<TARGET_OBJECTS:mission_table>
-l dl -l c
```

`Status.cpp` reads the table through its fixed link address rather than
through the symbol:

```cpp
#define MISSION_CASE ((const unsigned char *) 0x20b977)
...
switch (MISSION_CASE[m->getType()]) {
    case 200: case 211: ...
    case 208: ...
    case 213: ...
    case 214: ...
    case 21: case 25: ...
    case 0: ... case 23: ...
    case 39: ...
}
```

The case bodies are in the original's body order, so the ascending rank
order equals the original block layout: the emitted TBH has the same
40-entry shape, the same default slots, and the same body order.  The
fixed-address load is the original's own PIC form
(`ldr r1,[pc,#imm]; ldrb r0,[r0,r1]`, no `add r1,pc`), 2 B shorter than
a cross-TU symbol reference and therefore exactly the original's size.

Rank cells are the wave-188/wave-117 optimum: types 150..189 ->
`0..23,39,25` (type 172 -> **39**, matching the original TBH slot), low
group 0/8/11/13/14 -> 200/208/211/213/214, default 250.

## 2. Why the fixed-address read

A cross-TU reference to the hidden table symbol makes gold insert the
name into the dynamic name pool mid-link: `.dynsym` is permuted (144
entries move) and `.rel.plt` changes (102 B), dropping byte_exact to
4501.  Reading the same bytes through the fixed address keeps the
dispatch identical to the original and leaves `.dynsym`/`.rel.plt`
byte-identical.

A `static const` table in `Status.cpp`'s normal `.rodata` is the same
raw 475 but shifts every later `.rodata` address by 192 and drops
byte_exact 4507 -> 4426.  `#pragma clang section rodata=".missioncase"`
(w13321) avoids the shift but adds a 28th section header, +52 B file
size and 661 differing `.dynsym` bytes (`st_shndx` shift).  The
last-linked ordinary TU is the only form found that keeps 27 headers,
exact file size and exact `.dynsym` while landing the table at
`0x20b977`.

## 3. Measured dead ends (this pass)

| form | raw | size | .rel.plt | byte_exact | headers / file size |
|---|---:|---:|---|---:|---|
| base plain switch | 721 | 1432 | identical | 4507 | 27 / exact |
| **landed: hidden table in last-linked TU, fixed-address read** | **449** | **1432** | **identical** | **4507** | **27 / exact** |
| same table, `static const unsigned char *const` pointer | 975 | 1432 | identical | 4507 | 27 / exact |
| cross-TU symbol reference to the hidden table | 449 | 1432 | 102 B changed | 4501 | 27 / exact |
| `case 38` + `case 39` both present | 1083 | 1436 | identical | — | 27 / exact |
| table rank 38 for type 172 (w13321 value) | 449 | 1432 | identical | 4507 | 27 / exact (type-172 body wrong) |
| `#pragma clang section` private section (w13321) | 475 | 1432 | identical | 4507 | 28 / +52 B |

## 4. Structural checks

* `.rodata`: base `0x1c9e10` exact; original 269,159 B prefix
  byte-identical; sh_size 269,159 -> 269,351 (+192, the appended table
  at `0x20b977`); the 192 differing bytes are exactly the table.
* `.dynsym` 0 differing bytes, 5578 entries, name order and entry
  values identical; `.rel.plt` 0 (29,392 B), `.dynstr`, `.hash`,
  `.rel.dyn`, `.data`, `.got`, `.data.rel.ro` 0.
* 27 section headers, file size 2,187,328 exact.
* `check` RATCHET PASS, `lint` CLEAN, `drift` 0 fields.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13326
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN6Status16missionCompletedEbbx
# -> raw=449 aligned=160 size=1432/1432 insn=529/527
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
cmake --build cmake-build-match --target lint -j2    # CLEAN
cmake --build cmake-build-match --target drift -j2   # 0 fields
```

Scratch: `/home/debian/GalaxyOnFire2/scratch/w13326/` (`base.so` is the
pre-change build; `secdiff.py` and `dynsymcmp.py` are the section /
dynsym comparators used for the numbers above).

## Harvest on wave-189 master (`8225c71a8`)

Source candidate, not merged: it uses a fixed address
(`#define MISSION_CASE ((const unsigned char *) 0x20b977)`) and a separate
`mission_table` OBJECT library linked last.  That is manually linked data,
which the project objective forbids.  Measured on current master the same form
would give raw 449, `.text` 73,727 (from 73,999, -272), `.rodata` +192
appended bytes (prefix exact), `.dynsym`/`.rel.plt`/file size/27 headers exact;
true total 74,578 vs 74,658, a net -80.  The clean target is the same table
placed at the end of `.rodata` and read through an ordinary C++ symbol, with
no fixed address and no special OBJECT library.
