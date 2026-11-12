# `Status::missionCompleted` — wave 189 (w13305): ordinary-C++ rank table in its own section

Worktree `wt/w13305`, branch `wave189-w13305`, base master `247466fde`.
Target `_ZN6Status16missionCompletedEbbx` (`src/game/mission/Status.cpp`,
1,432 B, VMA `0xabf38`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13305`.

**Status: landed.**  Raw **721 -> 475** (aligned 327 -> 187), size
1,432/1,432.  `.text` differing bytes 74,047 -> **73,801**; `.ARM.extab`
639 and build-id 20 unchanged; total (text+extab+build-id) 74,706 ->
**74,460**.  `.rodata` content byte-identical (269,159 B); `.rel.plt`
content byte-identical.  byte_exact 4507/4524, linked_exact 4507/4524,
size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0, parity
5370/5370 (`RATCHET PASS`).

## 1. What changed

The wave-188 note measured the fixed-address table
`#define MISSION_CASE ((const unsigned char *) 0x20b977)` as raw 449 but
it is manual data.  This pass uses a normal `static const unsigned char
kMissionCase[192]` array inside `Status.cpp` (the same rank permutation)
and makes the linker put it where the fixed address used to be:

```cpp
#pragma clang section rodata="rodata"
static const unsigned char kMissionCase[192] = { ... };
#pragma clang section rodata=""
...
switch (kMissionCase[m->getType()]) { ... }
```

`#pragma clang section rodata="rodata"` (no leading dot) names a section
that does not match the linker script's `.rodata .rodata.*` input
pattern, so gold treats it as an orphan and places it immediately after
the last read-only section: `0x20b977`, the first byte past the original
269,159-byte `.rodata` image, inside the 4,649-byte alignment gap before
`.fini_array` at `0x20cba0`.  Appending it there shifts no existing
`.rodata` byte, so every literal pool keeps its address.  The section
name reuses the `rodata` suffix of the existing `.rodata` string in
`.shstrtab`, so the string table does not grow.

The rank table is the wave-117/188 permutation (types 0/8/11/13/14 ->
200/208/211/213/214, types 150..189 -> 0..23 plus 39, shared 167/174 ->
21/25, default 250).  Rank **38** for type 172 is used: the PIC table
load is two bytes longer than the fixed-address `ldr` (`ldr`+`add pc`+
`ldrb` instead of `ldr`+`ldrb`), and a 39-entry TBH (range 0..38) is two
bytes shorter than the original's 40-entry TBH, so the first body stays
at `0xac036` and the function stays 1,432 B.  With rank 39 the dispatch
is 2 B longer and every body shifts +2 (raw 1,202, size 1,436).

## 2. Measured forms (all size 1,432 unless noted)

| form | raw | size | byte_exact | .rel.plt | notes |
|---|---:|---:|---:|---|---|
| base plain `switch (m->getType())` | 721 | 1432 | 4507 | identical | wave-176 shape |
| wave-188 fixed `0x20b977` (rank 39) | 449 | 1432 | 4507 | identical | manual data |
| `static const` table in `.rodata` (rank 38) | 475 | 1432 | **4426** | identical | shifts .rodata +192, breaks 81 functions |
| `#pragma ... rodata=".rodata.zzzz"` (rank 38) | 475 | 1432 | **4428** | identical | still mid-.rodata |
| string literal `.rodata.str1.1` (rank 39) | 1202 | 1436 | — | identical | merge section, +4 B |
| `extern` hidden table in last TU (rank 38) | 475 | 1432 | 4507 | **244 lines** | `.dynsym` order permutes |
| table in last TU linked first (rank 38) | 475 | 1432 | 4507 | 416 lines | still permutes |
| writable orphan `.missioncase` (rank 38) | 475 | 1432 | **4476** | identical | shifts `.bss` |
| **this pass: orphan `rodata` (rank 38)** | **475** | **1432** | **4507** | **identical** | new section in the gap |

## 3. Cost

The orphan section is a 28th ELF section, so the section-header table
grows by one 40-byte entry: file size 2,187,328 -> **2,187,368 (+40)**.
26 of the original 27 section headers stay byte-identical; `.rel.plt`'s
`sh_info` (the `.got` section index) moves 20 -> 21 because the new
section sits before `.got`.  `.rel.plt` *content* is byte-identical.
`.rodata` content is byte-identical; its header is unchanged.

Alternatives that keep the file size exact all lose the byte_exact gate:
a table in `.rodata` proper shifts Status.cpp's 112th-of-140 `.rodata`
by +192 and changes the literal-pool words of 81 otherwise byte-exact
functions (byte_exact 4507 -> 4426); a real cross-TU symbol keeps the
address but gold reorders `.dynsym` and re-indexes 51 `.rel.plt` entries.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13305
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN6Status16missionCompletedEbbx
# -> raw=475 aligned=187 size=1432/1432 insn=528/527
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
cmake --build cmake-build-match --target drift -j2   # 0
cmake --build cmake-build-match --target lint  -j2   # CLEAN
```

Scratch: `Status_orphan.cpp` (landed shape), `Status_static38b.cpp`,
`Status_orphan39.cpp`, `Status_s38_zzzz.cpp`, `Status_orphan_vol.cpp`,
`Status_strlit.cpp`, `dirty_hidden_475.patch`, `base.so`,
`base_relplt_raw.txt`.

## 5. Files changed

* `src/game/mission/Status.cpp` — rank table in its own `rodata` section,
  switch on `kMissionCase[m->getType()]`, `case 38:` for type 172.
* this note.

## Harvest on wave-189 master (`eacfcc9ec`)

Measured on current master: the source form applies cleanly and gives
`.text` 73,753 (from 73,999, -246), but the 28th section costs
`.dynsym` 661, `.bss` 98, build-id 19 and file size +40
(2,187,328 -> 2,187,368).  True total differing bytes: 75,170 vs the current
74,658, a net loss of 512.  Kept as a note; w13326 is working the pure-C++
placement that keeps 27 sections and `.dynsym` exact.
