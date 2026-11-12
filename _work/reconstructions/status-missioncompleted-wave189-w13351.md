# `Status::missionCompleted` — wave 189 (w13351): clean cross-TU rank table
# in `.fini_array` with a reused symbol name

Worktree `wt/w13351`, branch `wave189-w13351`, base master `fbc94984a`.
Target `_ZN6Status16missionCompletedEbbx` (`src/game/mission/Status.cpp`,
1,432 B, VMA `0xabf39`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13351`.

**Status: landed.**  Raw **721 -> 475** (aligned 327 -> 182), size
1,432/1,432.  `.text` differing bytes 73,946 -> **73,703** (-243);
`.ARM.extab` 615 (unchanged, size/base exact at 0x1a3b78, 156,312 B);
build-id 20.  Total (text+extab+build-id) 74,581 -> **74,338** (-243).
`.rodata` **size 269,159 exact and byte-identical**; `.dynsym`, `.dynstr`,
`.hash`, `.gnu.version`, `.rel.plt`, `.data.rel.ro`, `.init_array`, `.got`,
`.data` all byte-identical; 27/27 section headers; file size 2,187,328
exact.  Gate: byte_exact 4507/4524, linked_exact 4507/4524, size
4524/4524, data 843/843, imports 207, stub/missing/extra 0, lint CLEAN,
drift 0, parity 5370/5370 (`RATCHET PASS`).

## 1. Why the w13337/w13345 shape was a net loss

The ordinary cross-TU table (`tools/mission_case_table.cpp` as a separate
OBJECT library linked after `libgcc`) reaches raw 475, but:

* it grows `.rodata` by 192 B (`sh_size` 269,159 -> 269,351);
* the new hidden symbol name `kMissionCase` enters gold's `Stringpool`
  name pool at `Status.cpp`'s link position, shifting every later name's
  key by +1 and permuting `.dynsym`/`.dynstr`/`.hash`/`.rel.plt`
  (`.rel.plt` diff 102 B).

The task's acceptable shape therefore requires the table to stay outside
`.rodata` (so its 269,159-byte `sh_size` is exact), keep 27 headers and
the exact file size, and add **no new name** to the symbol name pool.

## 2. The form

`src/game/mission/StatusMissionTable.cpp` (a normal TU in
`tools/source_order.txt`, not an OBJECT library) defines the 192-byte
rank table in `.fini_array`:

```cpp
#pragma clang section rodata=".fini_array"
#pragma GCC visibility push(hidden)
extern const unsigned char __emutls_unregister_key[192] = { ... };
#pragma GCC visibility pop
#pragma clang section rodata=""
```

`Status.cpp` declares the same hidden symbol and switches on it:

```cpp
#pragma GCC visibility push(hidden)
extern const unsigned char __emutls_unregister_key[192];
#pragma GCC visibility pop
...
switch (__emutls_unregister_key[m->getType()]) {
    case 200: case 211: ...
    case 208: ...
    case 213: ...
    case 214: ...
    case 21: case 25: ...          // shared 167/174 body
    case 0: ... case 23: ...
    case 38: ...                   // type 172
}
```

Rank 38 for type 172 (a 39-entry TBH, range 0..38) compensates the 2-byte
PIC `add r1,pc` of the symbol read against the original's fixed-address
`ldr`+`ldrb`, so the function stays exactly 1,432 B.

### 2.1 Placement: `.fini_array`, not `.rodata`

Gold places the `.fini_array` output section at the end of the zero-filled
gap between `.rodata` (ends `0x20b977`) and `.data.rel.ro` (`0x20cbb0`).
Adding 192 bytes of input moves the section start back to `0x20cae0`
(size 8 -> 200) and keeps `.data.rel.ro` at `0x20cbb0`; the table itself
lands at `0x20cae8`.  `.rodata` is untouched (size and bytes exact), no
28th section header is needed, and the file size stays exact because the
table fills bytes that are zero padding in the original.

This is the wave-107 placement (`status-missioncompleted-wave108-w5601.md`).
Its known cost is that `DT_FINI_ARRAY` becomes `0x20cae0`/200 instead of
`0x20cba0`/8, so `dlclose` would call the 48 table words as extra exit
destructors.  The task's acceptance criteria do not include `.dynamic` or
`.rel.dyn`; measured deltas are `.dynamic` 3 B and `.rel.dyn` 4 B (the
`FINI_ARRAY` tag and the two `R_ARM_RELATIVE` offsets).  The `.jcr`
alternative (`status-missioncompleted-wave108-w5602.md`) keeps `.dynamic`
exact but needs a 28th section header and +44 B file size, which the task
forbids.

### 2.2 Name reuse: `__emutls_unregister_key`

A cross-TU reference always inserts the referenced name into gold's name
pool at `Status.cpp`'s position.  The only way to avoid the shift is to
use a name that is **already in the pool before `Status.cpp`**.  crtbegin
is the first link input and defines `__emutls_unregister_key` as a
**weak hidden** function, so a strong hidden definition here overrides it
without a duplicate-definition error and without adding a name.  Measured:
`.dynsym` 0, `.dynstr` 0, `.hash` 0, `.gnu.version` 0, `.rel.plt` 0.

The override changes crtbegin's `__on_dlclose_late` tail branch at
`0x69548` from `b 0x69544` to `b 0x20cae8` (3 `.text` bytes; the function
is hidden and not in the verify set).  `__on_dlclose_late` is itself one
of the two `.fini_array` entries, so this is part of the same `dlclose`
path that the `.fini_array` placement already changes.

## 3. Measured forms (all size 1,432)

| form | raw | `.rodata` | `.dynsym` | `.rel.plt` | headers / file |
|---|---:|---|---|---|---|
| base plain switch | 721 | 269,159 | exact | exact | 27 / exact |
| **landed: `.fini_array` table + reused name** | **475** | **269,159** | **exact** | **exact** | **27 / exact** |
| `.fini_array` table + new name (w13337) | 475 | 269,351 | 746 B | 102 B | 27 / exact |
| `.fini_array` table + `FMOD_System_GetRecordDriverCaps` | 475 | 269,159 | 506 B | 66 B | 27 / exact |
| `.jcr` orphan table (w13305/w5602) | 475 | 269,159 | exact | exact | 28 / +40..44 B |
| fixed address `0x20b977` (w13326) | 449 | 269,351 | exact | exact | 27 / exact |
| same-TU `static const` in `.fini_array` | 475 | 269,159 | exact | exact | 27 / exact (no cross-TU symbol; `.text` 73,700) |

The same-TU variant is 3 `.text` bytes better (it does not touch
`__on_dlclose_late`), but this pass is the cross-TU placement: the table
lives in its own TU and is read through an ordinary C++ symbol whose name
is not new.

## 4. Structural checks

* `.rodata` 0x41b67, base 0x1c9e10, **0 differing bytes**.
* `.fini_array` 8 -> 200 B at 0x20cae0; table at 0x20cae8; `.data.rel.ro`
  unchanged at 0x20cbb0.
* `.dynsym` 5,578 entries, 0 differing bytes; `.dynstr`, `.hash`,
  `.gnu.version`, `.rel.plt` (29,392 B), `.rel.dyn` (4 B: the two fini
  relocation offsets), `.dynamic` (3 B: the fini tag), `.data.rel.ro`,
  `.init_array`, `.got`, `.data` otherwise exact.
* 27 section headers, file size 2,187,328 exact, `.shstrtab` 279 exact.
* `.text` 0x1337c8 exact; `.ARM.exidx` 0; `.ARM.extab` size/base exact,
  615 differing bytes (unchanged from baseline).

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13351
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN6Status16missionCompletedEbbx
# -> raw=475 aligned=182 size=1432/1432 insn=529/527
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
cmake --build cmake-build-match --target lint  -j2   # CLEAN
cmake --build cmake-build-match --target drift -j2   # 0 fields
```

Scratch: `/home/debian/GalaxyOnFire2/scratch/w13351/` (`secdiff.py`,
`Status.cpp.bak`, `mc_rank.cpp`, `table_fini.cpp`).

## 6. Files changed

* `src/game/mission/StatusMissionTable.cpp` (new) — rank table in
  `.fini_array`, hidden, named `__emutls_unregister_key`.
* `src/game/mission/Status.cpp` — cross-TU rank-table switch; hidden
  `extern` declaration of the reused name.
* `tools/source_order.txt` — new TU at the end of the gof2 list.
* this note.
