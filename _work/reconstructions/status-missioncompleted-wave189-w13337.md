# `Status::missionCompleted` — wave 189 (w13337): ordinary cross-TU rank table

Worktree `wt/w13337`, branch `wave189-w13337`, base master `1de8e9254`.
Target `_ZN6Status16missionCompletedEbbx` (`src/game/mission/Status.cpp`,
1,432 B, VMA `0xabf38`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13337`.

**Status: landed on branch `wave189-w13337`.**  Raw **721 -> 475**
(aligned 327 -> 187), size 1,432/1,432.  `.text` differing bytes
73,999 -> **73,753** (-246); `.ARM.extab` 639 (unchanged, size/base
exact at 0x1a3b78, 156,312 B); build-id 20.  `.rodata` prefix exact,
sh_size 269,159 -> 269,351 (+192: the table appended at `0x20b977`,
inside the 4,649-byte gap before `.fini_array`).  27/27 section
headers, file size 2,187,328 exact.  Gate: byte_exact 4507/4524,
linked_exact 4507/4524, size 4524/4524, data 843/843, imports 207,
stub/missing/extra 0, lint CLEAN, drift 0, parity 5370/5370
(`RATCHET PASS`).

**Cost (honest):** the cross-TU symbol reference inserts a new name
into gold's symbol name pool at `Status.cpp`'s link position, which
shifts the sequential `Stringpool::Key` of every later name by +1 and
therefore permutes the hash-table iteration order that gold uses for
`.dynsym`.  Symbol set, values and order-independent content are
unchanged (parity 5370/5370, imports 207), but the bytes differ:
`.dynsym` 746, `.dynstr` 4,417, `.hash` 372, `.gnu.version` 56,
`.rel.dyn` 18, `.rel.plt` 102.  `.rel.plt` offsets are unchanged; only
the `r_info` symbol indices follow the new `.dynsym` order.  This is
the only form found that keeps 27 headers, exact file size and the
`.rodata` prefix while using a normal C++ symbol instead of a fixed
address; the fixed-address form (w13326) keeps `.dynsym`/`.rel.plt`
byte-identical but is manual data.

## 1. The form

`tools/mission_case_table.cpp` (tracked) is compiled as the
`mission_table` CMake OBJECT library and linked after `libgcc`, i.e.
after every archive, immediately before `-l dl -l c`:

```cpp
#pragma GCC visibility push(hidden)
extern const unsigned char kMissionCase[192] = { ... };   // rank 38 for type 172
#pragma GCC visibility pop
```

`Status.cpp` declares the same symbol hidden and reads it with an
ordinary cross-TU array reference:

```cpp
#pragma GCC visibility push(hidden)
extern const unsigned char kMissionCase[192];
#pragma GCC visibility pop
...
switch (kMissionCase[m->getType()]) {
    case 200: case 211: ...
    case 208: ...
    ...
    case 38: ...
}
```

Because the table is the last `.rodata` input, it lands at `0x20b977`
(the first byte past the original `.rodata` image) without moving any
existing `.rodata` byte, adding a section, or growing the file (the
gap before `.fini_array` at `0x20cba0` absorbs it).  Hidden visibility
keeps it out of `.dynsym`; the reference is a PIC load
(`ldr r1,[pc,#imm]; add r1,pc; ldrb r0,[r1,r0]`), 2 B longer than the
original's fixed-address `ldr`+`ldrb`.  Rank **38** for type 172 (a
39-entry TBH, range 0..38) is 2 B shorter than the original's 40-entry
TBH, exactly compensating the longer load: the function stays 1,432 B.
Rank 39 (the w13326 value) with the PIC load gives size 1,436.

## 2. Why the `.dynsym` permutation is not avoidable with a symbol

Gold 2.27's `Symbol_table::table_` is `std::tr1::unordered_map` keyed
by `(name_key, version_key)`, where `name_key` is a **sequential**
`Stringpool::Key` (the order the name string was first added to
`Symbol_table::namepool_`), and `Symbol_table::hash` is
`name_key ^ version_key`.  `sized_finalize` iterates that hash table
to emit `.dynsym`; `dynstr` is built in the same order.

Adding any global symbol name at `Status.cpp`'s position shifts every
later name's key by +1, changing its bucket and hence the iteration
order.  Measured with an unreferenced hidden dummy:

| name first added in | `.dynsym` name-order diff |
|---|---:|
| `BackButton.c` (first gof2 TU) | 203 lines |
| `Status.cpp` (this pass) | 144 lines |
| `ArrayInstantiations.cpp` (last gof2 TU) | 94 lines |
| OBJECT lib after `libgcc` (last input) | **0** |

A dummy whose name is *already* in the pool (`g_android_back_button_pressed`)
also gives 0, confirming the mechanism is the key shift, not a rehash
(the last-input dummy adds a node and still gives 0).  A `static`
(local) symbol does not enter the pool at all, which is why the
fixed-address form is exact.

The reference must be in `Status.cpp`, so the name is always added
there; the only zero-shift names are ones already in the pool, and
every such name is already a defined/undefined symbol (a second
definition conflicts).  `LIBC` is not in the name pool (version
strings go to the output `dynpool`, not `namepool_`), so naming the
table `LIBC` still shifts.

## 3. Measured forms (all size 1,432 unless noted)

| form | raw | `.dynsym` | `.rel.plt` | headers / file size |
|---|---:|---:|---|---|
| base plain switch | 721 | exact | exact | 27 / exact |
| **landed: hidden table in last OBJECT TU, symbol read** | **475** | 746 B permuted | 102 B permuted | **27 / exact** |
| same, fixed address `0x20b977` (w13326) | 449 | exact | exact | 27 / exact |
| table via `static const` in `ArrayInstantiations.cpp` | 475 | 915 B | 102 B | 27 / exact, `.rodata` +14,968 (shifts zlib/libc++abi) |
| table via `static const` in `Status.cpp` `.rodata` | 475 | exact | exact | 27 / exact, `.rodata` +192 mid-image (byte_exact 4426) |
| `#pragma clang section` private section (w13321) | 475 | 661 B | exact | 28 / +52 |
| `rodata` orphan (w13305) | 475 | 661 B | exact | 28 / +40 |
| `&__dso_handle - 0x86a9` | 1,133 | exact | exact | 27 / exact (PIC+extra, +8 B) |
| `_end - 0x119a5` | 1,231 | 44 lines | 1,354 B | 27 / exact (adds `.rel.dyn` entry) |

## 4. Structural checks

* `.rodata` 0x1c9e10, 269,159 -> 269,351; the original 269,159-byte
  prefix is byte-identical, the 192 differing bytes are the table.
* `.text` 0x69440, size 1,259,464 exact; `.ARM.exidx`/`.ARM.extab`
  size/base exact; `.data`, `.bss`, `.got`, `.data.rel.ro` size/base
  exact.
* `.dynsym` 5,578 entries, `.rel.plt` 29,392 B; symbol set, values and
  relocation offsets identical, order permuted (see §2).
* 27 section headers, file size 2,187,328 exact, `.rel.plt` size exact.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13337
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN6Status16missionCompletedEbbx
# -> raw=475 aligned=187 size=1432/1432 insn=528/527
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
cmake --build cmake-build-match --target lint  -j2   # CLEAN
cmake --build cmake-build-match --target drift -j2   # 0 fields
```

Scratch: `/home/debian/GalaxyOnFire2/scratch/w13337/` (`measure.sh`,
`secdiff.py`, `base_unstripped_new.so`, `relink_only.sh`).

## 6. Files changed

* `tools/mission_case_table.cpp` — rank 39 -> 38 for type 172 (PIC
  size compensation), comment updated (symbol read, not fixed address).
* `src/game/mission/Status.cpp` — rank-table switch body; hidden
  `extern` declaration of `kMissionCase`.
* `CMakeLists.txt` — `mission_table` OBJECT library linked after
  `libgcc`.
* this note.
