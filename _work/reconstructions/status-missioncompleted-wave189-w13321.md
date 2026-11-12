# `Status::missionCompleted` — wave 189 (w13321): ordinary-C++ rank table in a
# private `.missioncase` section

Worktree `wt/w13321`, branch `wave189-w13321`, base master `1c901e8b0`.
Target `_ZN6Status16missionCompletedEbbx` (`src/game/mission/Status.cpp`,
1,432 B, VMA `0xabf38`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13321`.

**Status: candidate on branch `wave189-w13321` (commit `49642926d`); not merged to master.**  Raw **721 -> 475** (aligned 327 -> 187), size
1,432/1,432, `.rel.plt` byte-identical, `.rodata` byte-identical and
size-exact (0x41b67), byte_exact 4507/4524, linked_exact 4507/4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.
`.text` differing bytes 74,032 -> **73,786**; `.ARM.extab` 639
(unchanged, size/base exact at 0x1a3b78, 156,312 B); build-id 20;
total 74,691 -> **74,445**.  The cost is one extra section header
(`.missioncase`, 28 headers instead of 27) and +52 bytes of file size
(2,187,328 -> 2,187,380); see §3.

## 1. Why the plain switch loses

The original lays the 23 switch bodies out in
`167/174, 150,151,152,153,154,155,158,162,166,170,184,189,
156,157,160,163,164,165,168,169,171,172` order — two sorted runs.
clang 7's `MachineBlockPlacement` follows the jump table's value-sorted
successor list, so a plain `switch (m->getType())` emits the 150..189
bodies in ascending value order and leaves raw 721.  Switching on a
type -> rank table makes the ascending rank order equal the original
body order; the function stays 1,432 B and `.rel.plt` stays identical.

The wave-188 note (`status-missioncompleted-wave188-w13218.md`) landed
the rank table as a separate last-linked TU at the fixed address
`0x20b977` (raw 449).  That form is manual data: it hardcodes the
table address and needs a special CMake OBJECT library.  This pass
keeps the table in `Status.cpp` as ordinary `static const` data.

## 2. The form

```cpp
#pragma clang section rodata=".missioncase"
static const unsigned char kMissionCase[192] = { ... };
#pragma clang section rodata=""

...
switch (kMissionCase[m->getType()]) {
    case 200: case 211: ...
    case 208: ...
    case 213: ...
    case 214: ...
    case 21: case 25: ...
    case 0: ... case 23: ...
    case 38: ...
}
```

The pragma is a Clang source-level section directive (not
`__attribute__`, not asm).  The table lands in its own `.missioncase`
output section.  The linker's orphan placement puts it at `0x20b977`,
the first byte past the original `.rodata` (`.rodata` ends at
`0x20b977`; `.fini_array` is at `0x20cba0`), so no existing `.rodata`
address moves.  The dispatch is the ordinary PIC form
`ldr r1,[pc,#imm]; add r1,pc; ldrb r0,[r1,r0]` (2 B longer than the
fixed-address `ldr`), and the body block is laid out in the original
order.  The function is still exactly 1,432 B; the +2 B is absorbed by
the matching body layout.

The rank cells are the wave-188/wave-117 optimum (types 150..189 ->
0..23,25,38; low group 0/11/8/13/14 -> 200/211/208/213/214; default
250).  They are unchanged because they are the only known mapping that
keeps the emitted TBH's default slots and the longest matching TBH
target run.

## 3. Structural cost: one extra section header

The table is a separate output section, so the ELF now has 28 section
headers instead of 27.  The linker places `.missioncase` between
`.rodata` and `.fini_array`; the section header (40 B) and the
`.shstrtab` name (12 B) grow the file by 52 B.  `.dynsym` entries for
symbols in sections after `.missioncase` shift their `st_shndx` by one
(661 differing `.dynsym` bytes), but the `.dynsym` set and order are
unchanged (parity 5370/5370) and `.rel.plt` is byte-identical.  All
alloc section sizes and addresses except the new section are exact:
`.text`, `.ARM.exidx`, `.ARM.extab`, `.rodata`, `.data.rel.ro`,
`.data`, `.bss`, `.got` are size- and base-exact.

This is the price of keeping the table in `Status.cpp` without shifting
`.rodata`.  A `static const` table in the normal `.rodata` (no pragma)
is raw 475 too, but shifts every later `.rodata` address by 192 and
drops byte_exact 4507 -> 4426 (the wave-188 §2a measurement).  A
cross-TU hidden table at the end of `.rodata` avoids the shift but
permutes `.dynsym`/`.rel.plt` (102 `.rel.plt` bytes) and drops
byte_exact to 4501.  The private section is the only ordinary-C++ form
found that keeps byte_exact at 4507 and `.rel.plt` byte-identical.

## 4. Dead ends measured this pass

| form | raw | size | .rel.plt | byte_exact | .rodata |
|---|---:|---:|---|---:|---|
| base plain switch | 721 | 1432 | identical | 4507 | exact |
| **landed: private `.missioncase` section** | **475** | **1432** | **identical** | **4507** | **exact** |
| `static const` table in normal `.rodata` | 475 | 1432 | identical | 4426 | +192 |
| `static const` table via `.rodata.str1.1` pragma | 475 | 1432 | identical | 4427 | +192 |
| hidden extern table in `ArrayInstantiations.cpp` | 475 | 1432 | 102 B changed | 4501 | +192 |
| hidden extern table in last-linked OBJECT lib | 475 | 1432 | 102 B changed | 4501 | +192 |
| `static volatile` table in `.data` | 475 | 1432 | identical | 4471 | exact |
| table as a direct string literal | 475 | 1432 | identical | 4426 | +208 |
| two sequential switches on cached `int` | 1275 | 1508 | — | — | exact |
| two switches, `static int` scratch | 1358 | 1476 | 4 B | — | exact |
| two switches, `volatile int` scratch | 1489 | 1536 | 4 B | — | exact |
| rank computed by a 40-case switch | 915 | 1428 | identical | — | exact |
| fixed-address table (wave-188, manual data) | 449 | 1432 | identical | 4507 | +192 |

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13321
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN6Status16missionCompletedEbbx
# -> raw=475 aligned=187 size=1432/1432 insn=528/527
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
cmake --build cmake-build-match --target drift -j2   # 0 fields
cmake --build cmake-build-match --target diff -j2    # parity 5370/5370
```

Scratch: `/home/debian/GalaxyOnFire2/scratch/w13321/`
(`Status_volatile_current.cpp` is the rank-switch body used here;
`gen.py` and the `build_*.log` files are the variant sweep).

## Harvest on wave-189 master (`f4dd70186`)

The branch base was `1c901e8b0`; current master is `f4dd70186` with `.text`
74,009 and total 74,668.  Re-based on current master the same source would
give `.text` 73,763 (74,009 - 246) and the text/extab/build-id total 74,422,
**but** the extra `.missioncase` section adds **661 differing `.dynsym` bytes**
(`st_shndx` shift) and **+52 B file size**.  True total differing bytes:
74,422 + 661 = **75,083**, a net loss of 415 vs the current 74,668.  The
source is therefore kept on the branch, not merged.  The next step is the
same rank-table switch with the 192-byte table placed at the end of the
original `.rodata` (0x20b977) by ordinary C++ / link order, keeping 27 section
headers and `.dynsym` exact.
