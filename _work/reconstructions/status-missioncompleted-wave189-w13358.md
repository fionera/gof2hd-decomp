# `Status::missionCompleted` — wave 189 (w13358): rank table at the end of `.rodata`

Worktree `wt/w13358`, branch `wave189-w13358`, base master `26fbed661`.
Target `_ZN6Status16missionCompletedEbbx` (`src/game/mission/Status.cpp`,
1,432 B, VMA `0xabf38`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13358`.

**Status: landed.**  Raw **475** (unchanged), aligned **182 -> 187** (a
literal-pool disassembly artifact; the code stream is identical to the
`.fini_array` form), size 1,432/1,432.  `.text` differing bytes **73,565**
(unchanged); `.ARM.extab` **610** (unchanged, size/base exact at
`0x1a3b78`, 156,312 B); build-id 20.  Total (text+extab+build-id)
**74,195** (unchanged).  The table moved from `.fini_array` to the end of
`.rodata`, restoring the original RELRO layout:

* `.fini_array` **8 B @ `0x20cba0`, byte-identical**;
* `.dynamic` **byte-identical** (the `DT_FINI_ARRAY` tag is back to
  `0x20cba0`/`8`);
* `.rel.dyn` **byte-identical** (the two `R_ARM_RELATIVE` offsets are back
  to `0x20cba0`/`0x20cba4`);
* `.data.rel.ro`, `.data`, `.dynsym`, `.dynstr`, `.hash`, `.gnu.version`,
  `.rel.plt`, `.init_array`, `.got` all byte-identical;
* 27/27 section headers, file size 2,187,328 exact.

The only cost is `.rodata` `sh_size` **269,159 -> 269,351** (+0xc0,
`0x41b67 -> 0x41c27`): the original 269,159 bytes are **0 differing
bytes** (base `0x1c9e10` exact), and the table fills the first 0xc0 bytes
of the zero-filled `.rodata`..`.fini_array` gap at `0x20b977`.  The R E
LOAD `FileSiz`/`MemSiz` grows by the same 0xc0 (`0x20b977 -> 0x20ba37`);
every other program header is unchanged.

Gate: byte_exact **4507/4524**, linked_exact **4507/4524**, size
4524/4524, data 843/843, imports 207, stub/missing/extra 0, lint CLEAN,
drift 0, parity 5370/5370 (`RATCHET PASS`).

## 1. Why the previous placements were not clean

The w13351 landed form put the 192 B table in `.fini_array`.  Gold places
`.fini_array` at the front of the RELRO group, and the RELRO group is
anchored at its end (`.got` ends at page-aligned `0x213000` file /
`0x214000` vaddr).  Adding 0xc0 to the group moves the group start from
`0x20bba0` to `0x20bae0`, so `.fini_array` becomes 200 B @ `0x20cae0`:
`DT_FINI_ARRAY` points at the 48 table words (a `dlclose` hazard), and
`.dynamic`/`.rel.dyn` differ by 3/4 B.

Putting the table at the front of `.data.rel.ro` instead keeps
`.fini_array` at 8 B but moves its address to `0x20cae0` and grows
`.data.rel.ro` to `0x30a8`; `.dynamic`/`.rel.dyn` still differ (2/4 B).

`.jcr` (w5602) keeps `.fini_array`/`.dynamic`/`.rel.dyn` exact but adds a
28th section header and 44 B of file size.  `.data.rel.ro.local` and
`.rodata.gap` were also measured; `.rodata.gap` merges into `.rodata` at
the TU's link position (before the archive `.rodata`), shifting the
zlib/libc++abi literal pool and dropping byte_exact to 4424.

## 2. The form

`src/game/mission/StatusMissionTable.cpp` emits the 192-byte rank table
into `.rodata`:

```cpp
#pragma clang section rodata=".rodata"
#pragma GCC visibility push(hidden)
extern const unsigned char __emutls_unregister_key[192] = { ... };
#pragma GCC visibility pop
#pragma clang section rodata=""
```

and `CMakeLists.txt` links that one object **after** the archives
(`$<FILTER:$<TARGET_OBJECTS:gof2>,EXCLUDE,StatusMissionTable>` for the
normal list, `INCLUDE` for the table object placed after `${_link_libgcc}`).
The table is therefore the last `.rodata` input: it lands at `0x20b977`,
the first byte of the gap, and no existing literal-pool address moves.
`.text` order is unaffected (the TU has no `.text`).

Gold's RELRO layout is unchanged because the table is outside the RELRO
group: with the group size back at 0x7460, the group start is again
`0x20bba0`, `.fini_array` is `0x20cba0`/8, and `.dynamic`/`.rel.dyn` are
byte-identical.  The gap is 0x1229 bytes, so the 0xc0 table is absorbed
without moving `.fini_array` or growing the file.

## 3. Measured forms (all size 1,432)

| form | raw | aligned | `.fini_array` | `.dynamic` | `.rel.dyn` | `.rodata` | headers / file |
|---|---:|---:|---|---|---|---|---|
| w13351 `.fini_array` table | 475 | 182 | 200 @ 0x20cae0 | 3 B | 4 B | exact | 27 / exact |
| `.data.rel.ro` front table | 475 | 182 | 8 @ 0x20cae0 | 2 B | 4 B | exact | 27 / exact |
| `.jcr` table (w5602) | 475 | 182 | 8 @ 0x20cba0 | exact | exact | exact | 28 / +44 |
| **landed: `.rodata` tail table** | **475** | **187** | **8 @ 0x20cba0** | **exact** | **exact** | **+0xc0 size, 0 diff** | **27 / exact** |

The aligned 182 -> 187 change is the literal pool at `0xac300`: its
32-bit word is the table's PC-relative offset, which differs between
`0x20cae8` and `0x20b977`, so `fn_diff` disassembles one pool word
differently (528 vs 529 "instructions").  The code stream is identical;
`raw` is unchanged at 475.

## 4. Structural checks

* `.rodata` `0x41b67 -> 0x41c27` @ `0x1c9e10`; 0 differing bytes over
  the original 269,159 B; the table occupies `0x20b977..0x20ba37`.
* `.fini_array` 8 B @ `0x20cba0`, `.dynamic` 0x148 @ `0x20fba4`,
  `.rel.dyn` 0x7048 @ `0x504d4`: byte-identical.
* `.data.rel.ro` 0x2fe8 @ `0x20cbb0`, `.data` 0x296c @ `0x214000`,
  `.dynsym`/`.dynstr`/`.hash`/`.gnu.version`/`.rel.plt`/`.init_array`/
  `.got`: byte-identical.
* 27 section headers, `.shstrtab` 0x117 exact, file size 2,187,328 exact.
* Program headers: only the R E LOAD `FileSiz`/`MemSiz`
  `0x20b977 -> 0x20ba37` (+0xc0); the RW LOAD, GNU_RELRO, DYNAMIC,
  EXIDX and NOTE headers are unchanged.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13358
ulimit -v 12000000
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN6Status16missionCompletedEbbx     # raw=475 aligned=187 size=1432/1432
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
cmake --build cmake-build-match --target lint  -j2   # CLEAN
cmake --build cmake-build-match --target drift -j2   # 0 fields
```

Scratch: `/home/debian/GalaxyOnFire2/scratch/w13358/` (`measure.py`,
`rodata.so`, `keep_rodata_mc.txt`, `ours_mc_fini.txt`, `orig_mc.txt`).

## 6. Files changed

* `src/game/mission/StatusMissionTable.cpp` — table emitted into
  `.rodata` (was `.fini_array`); comment updated.
* `CMakeLists.txt` — link the table object after the archives so it is
  the last `.rodata` input.
* this note; `docs/IDENTICALITY.md` and `docs/VALIDATION.md` inline
  numbers updated.
