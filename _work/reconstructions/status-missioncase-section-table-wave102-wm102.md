# Removing `.missioncase`: section-table identity and the direct-switch reconstruction (wave102, wm102)

Status: **landed source change + research note**. Worktree `wt/wm102`, branch
`wave102-wm102`, base master `53d9ee4d5`. Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000; real VMA = Ghidra - 0x10000).
Target: `_ZN6Status16missionCompletedEbbx` (`Status::missionCompleted(bool, bool, long long)`),
original `0xabf38`, size **1432 (0x598)**.

## 0. Result

The 192-byte `kMissionCase[192]` rank table cannot be moved into any existing ELF section
without changing that section's size (or, for `.rodata`, without changing its size/content and
dropping `byte_exact`). Every original section is already size- and base-exact; the only
"free" space is the 0x1229-byte virtual gap between `.rodata` and `.fini_array`, and a section
placed there is exactly the extra `.missioncase` section.

The table was therefore eliminated. The direct `int` switch with the case-166 `return m` size
anchor keeps the function at 1432 bytes and restores the original 27-section table:

| metric | before (rank table) | after (direct switch) | original |
|---|---:|---:|---:|
| section headers | 28 | **27** | 27 |
| `.shstrtab` size | 0x124 | **0x117** | 0x117 |
| `.rel.plt` `sh_info` | 21 | **20** | 20 |
| `.dynsym` `st_shndx` | +1 shifted | **exact** | exact |
| file size | 0x216074 (+52) | **0x216040 (0)** | 0x216040 |
| `.text` size | 0x1337c8 | **0x1337c8** | 0x1337c8 |
| `.rodata` size / diff | 0x41b67 / 0 | **0x41b67 / 0** | 0x41b67 |
| `.ARM.extab` size | 0x2628c (-12) | **0x2628c (-12)** | 0x26298 |
| `byte_exact` | 4500 | **4500** | — |
| `linked_exact` | 4500 | **4500** | — |
| `missionCompleted` fuzzy | 87.0% | **60.4%** | — |

The only remaining section-header difference is the known `.ARM.extab` `-12` size residual.
`byte_exact`, `linked_exact`, size-exact, `data_exact`, imports, lint and drift are unchanged.
The trade-off is the `missionCompleted` fuzzy score: 87.0% -> 60.4%. The rank table was a
codegen device, not original data; the direct switch is the faithful dispatch (no `sxth`).

## 1. Gate (before -> after)

Before (master `53d9ee4d5`, rank table present):

```
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.91
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN
drift: 0 field(s) across 0 class(es)
```

After (this pass):

```
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN
drift: 0 field(s) across 0 class(es)
```

`missionCompleted` row: `pct 60.4`, `osize 1432`, `usize 1432`, `byte false`, `linked false`.
The 24 non-byte-exact functions are unchanged except for the target's fuzzy score.

## 2. Why the table cannot live in an existing section

The original section table (27 entries) has exact sizes for every section. The 192-byte table
needs a home; the candidates and their measured effects are below. All rows are full
`check` builds in a scratch build dir; `byte` is `report.json`'s `byte_exact`.

| # | placement | sections | file size | section-size change | `.rodata` diff | `byte` | fuzzy | gate |
|---|---|---:|---:|---|---:|---:|---:|---|
| 0 | `.missioncase` (baseline) | 28 | +52 | `.missioncase` +192 | 0 | 4500 | 87.0 | PASS |
| A | `.rodata` at `Status.cpp`'s position (remove pragma) | 27 | 0 | `.rodata` +192 | 32,876 | 4422 | 87.0 | FAIL |
| B | `.rodata` at end (table moved to last TU, `ArrayInstantiations.cpp`) | 27 | 0 | `.rodata` +192 | 14,776 | 1792 | 85.6 | FAIL |
| C | `.text` (`#pragma clang section rodata=".text"`) | 27 | 0 | `.text` +192 | 223,082 | 2438 | 87.0 | FAIL |
| D | `.data` (`#pragma clang section rodata=".data"`) | 27 | +192 | `.data` +192 | 0 | 4464 | 87.0 | FAIL |
| E | `.data.rel.ro` (`#pragma clang section rodata=".data.rel.ro"`) | 27 | 0 | `.data.rel.ro` +192, addr -0xc0 | 0 | 4498 | 87.0 | FAIL |
| F | `.got` (`#pragma clang section rodata=".got"`) | 27 | 0 | `.got` +192, flags WA->A | 0 | 2950 | 87.0 | FAIL |
| G | direct `int` switch, `continue` anchor | 27 | 0 | none | 0 | 3043 | 60.4 | FAIL |
| H | direct `int` switch, `return m` anchor | 27 | 0 | none | 0 | **4500** | **60.4** | **PASS** |
| I | direct `(short)` switch, `continue` anchor | 27 | 0 | none | 0 | 4500 | 57.8 | PASS |

Notes:

* **A** is the naive "put the table in `.rodata`" form. The table is emitted at
  `Status.cpp`'s `.rodata` position, so every later `.rodata` object and every PC-relative
  literal that points into it shifts by 192 bytes. `.rodata` grows to `0x41c27`; the section
  table is no longer exact and `byte_exact` drops 4500 -> 4422.
* **B** moves the table to the last TU (`ArrayInstantiations.cpp`, which has no other
  `.rodata`) so it is appended at the end of `.rodata`. It still grows `.rodata` by 192 bytes
  and shifts the archive `.rodata` (libc++/libgcc/zlib/libzip/openssl) that follows the
  objects. `byte_exact` drops to 1792. (The first attempt used a non-`extern` `const`
  definition, which has internal linkage and left `kMissionCase` as an undefined import; the
  1792 figure is the corrected `extern const` build with 207 imports.)
* **C-F** force the table into `.text`, `.data`, `.data.rel.ro` and `.got`. Each grows the
  target section by 192 bytes and shifts every later section. `.data.rel.ro` is the least
  damaging (4498, only 2 functions lost) because the `.data.rel.ro`/`.got`/`.data` page
  layout absorbs the shift, but it still fails the ratchet.
* **G** is the faithful direct `int` switch with the old `continue` anchor. The function is
  1428 bytes (4 short), so `.text` local layout shifts and `byte_exact` collapses to 3043.
* **H** is the landed form: the same direct `int` switch, but the case-166 null anchor returns
  `m` instead of `continue`. That splits the body into two MBBs and restores the 4 bytes lost
  to short branch encodings, landing exactly on 1432 bytes. This is the only source-only form
  tested that gives 27 sections and passes the ratchet.
* **I** uses a `(short)` cast instead of the `int` switch. It also passes the gate but scores
  57.8% (the cast emits an `sxth` the original does not have), so **H** is preferred.

The section table for **H** matches the original except for the known `.ARM.extab` size:

```
-- header field diffs (by name) --
.ARM.extab size 156312->156300
```

All other section names, types, addresses, offsets, sizes, flags, link/info and alignments
match. `.shstrtab`, `.dynsym`, `.dynstr`, `.hash`, `.gnu.version*`, `.rel.dyn`, `.rel.plt`,
`.plt`, `.rodata`, `.fini_array`, `.data.rel.ro`, `.init_array`, `.dynamic`, `.got`, `.data`,
`.comment`, `.note.android.ident`, `.note.gnu.gold-version` and `.ARM.attributes` are
byte-identical. `.text`, `.ARM.exidx`, `.ARM.extab` and `.note.gnu.build-id` differ in
content (expected); `.ARM.extab` is also 12 bytes short.

## 3. The direct-switch reconstruction (landed)

`src/game/mission/Status.cpp`:

1. Delete the `kMissionCase[192]` definition and its
   `#pragma clang section rodata=".missioncase"` / `#pragma clang section rodata=""` pair.
2. Change `switch (kMissionCase[m->getType()])` to `switch (m->getType())` (plain `int`,
   no cast).
3. Relabel each `case <rank>:` with the mission type(s) that map to that rank. The rank ->
   type inverse of the deleted table is:

   ```
   rank 150 -> 167,174   rank 151 -> 150   rank 152 -> 151   rank 153 -> 152
   rank 154 -> 153       rank 155 -> 154   rank 156 -> 155   rank 157 -> 158
   rank 158 -> 162       rank 159 -> 166   rank 160 -> 170   rank 161 -> 184
   rank 162 -> 189       rank 163 -> 156   rank 164 -> 157   rank 165 -> 160
   rank 166 -> 163       rank 167 -> 164   rank 168 -> 165   rank 169 -> 168
   rank 170 -> 169       rank 171 -> 171   rank 181 -> 172
   rank 0 -> 0           rank 8 -> 8       rank 11 -> 11     rank 13 -> 13
   rank 14 -> 14
   ```

   The body order is unchanged (it is the original body order):
   `0/11, 8, 13, 14, 167/174, 150, 151, 152, 153, 154, 155, 158, 162, 166, 170, 184,
   189, 156, 157, 160, 163, 164, 165, 168, 169, 171, 172`.
4. Keep the case-166 size anchor, but make it `return m` (not `continue`):

   ```cpp
   Ship *s = ship;
   if (s == 0) {
       return m;
   }
   ```

   This is a size/regalloc anchor, not original source (the original has no null check); it
   restores the 4 bytes the direct switch loses to short branch encodings and keeps the
   function at 1432 bytes. The `continue` form is 1428 bytes and fails `byte_exact` (3043).

The landed diff is 242 lines (mostly the case relabeling); the key hunks are the table
deletion, the `switch (m->getType())` change, and the `case 167: case 174:` split. The
source is committed in this pass; `git show` the `Status::missionCompleted` hunk for the full
relabeling.

## 4. Proven vs hypothesis

**Proven (commands in §5):**

1. The original has 27 section headers and no `.missioncase`; its `.shstrtab` is 0x117,
   `.rel.plt` `sh_info=20`, and its file size is 0x216040.
2. The rank table cannot be placed in `.rodata` without changing `.rodata`'s size: the naive
   `Status.cpp` placement grows `.rodata` to 0x41c27 and drops `byte_exact` to 4422; the
   end-of-`.rodata` placement grows it to 0x41c27 and drops `byte_exact` to 1792. Forcing it
   into `.text`, `.data`, `.data.rel.ro` or `.got` grows that section by 192 bytes and also
   fails the ratchet.
3. The direct `int` switch with the case-166 `return m` anchor is 1432 bytes, keeps
   `byte_exact 4500` / `linked_exact 4500`, `.text 0x1337c8`, `.rodata` 0 differing bytes,
   `.ARM.exidx 0x6f70`, lint `CLEAN`, drift 0, and produces the original 27-section table
   (`.shstrtab 0x117`, `.rel.plt sh_info=20`, exact file size) with only the known
   `.ARM.extab -12` size delta.
4. The direct `int` switch with the `continue` anchor is 1428 bytes and fails
   (`byte_exact 3043`). The `(short)` cast form is 1432 bytes and passes but scores 57.8%.
5. The landed form's `missionCompleted` fuzzy score is 60.4% (down from 87.0%). The rank
   table was a codegen device, not original data.

**Hypothesis (not proven):**

* The rank table is the only source-only way found to make clang 7's
  `MachineBlockPlacement` lay the switch bodies out in the original source order; the
  direct switch is faithful but clang orders the bodies by ascending case value. The
  original's exact source shape (or a source-level block-placement hint) is not recovered.
* The remaining `missionCompleted` 60.4% residual is the body order plus the `sxth`-free
  dispatch; a source form that makes clang take the `getFirstUnplacedBlock` fallback from a
  single switch without a table is still open (see
  `_work/reconstructions/status-missioncompleted-wave100-w5003.md`).

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/wm102

# scratch build (never the canonical build dir)
cmake --preset default -B /dev/shm/wm102/build \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build /dev/shm/wm102/build --target check -j8
cmake --build /dev/shm/wm102/build --target drift -j8

# section table / hashes / .rodata diff (same parser as the table above)
python3 /dev/shm/wm102/measure.py /dev/shm/wm102/build

# raw section table
RD=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-readelf
$RD -SW /dev/shm/wm102/build/libgof2hdaa.so
$RD -SW /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
```

Experiment variants (all are `Status.cpp`-only edits; rebuild + `check` after each):

* **A**: delete the two `#pragma clang section` lines, keep the table.
* **B**: move the table definition to `src/engine/core/ArrayInstantiations.cpp` as
  `extern const unsigned char kMissionCase[192] = {...};` under
  `#pragma GCC visibility push(hidden)` / `pop`, and declare it `extern` in `Status.cpp`.
* **C-F**: change the pragma's section name to `.text`, `.data`, `.data.rel.ro`, `.got`.
* **G**: direct `int` switch with the existing `continue` anchor.
* **H**: direct `int` switch with `return m` (landed).
* **I**: direct `(short) m->getType()` switch with `continue`.

The `measure.py` helper used for the table is scratch-only (not committed); it prints the
section table, per-section SHA-256 (first 16 hex), `.rodata` differing bytes, file size, the
`missionCompleted` report row and the verify summary. The raw `report.json` is the official
source for the `byte_exact`/`linked_exact`/`fuzzy` columns.
