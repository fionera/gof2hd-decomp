# Removing `#pragma clang section` (w3521)

Status: research note + landed source change (2026-11-01). Worktree `wt/w3521`, branch
`wave87-w3521`, based on master `16e5c9a4`.

## TL;DR

Both `#pragma clang section` uses in `src/` are gone:

* `PlayerCreature.cpp` — plain `.rodata` selection is now obtained with
  `[[gnu::used]] static const int kCreatureTypeFactor[4]`.  The pragma and the
  `[[gnu::used]]` form compile to **identical function bytes**; the table still lands in
  plain `.rodata` at the original address `0x201dd0`.
* `BlowfishSubkeyFile.cpp` — the file is now `BlowfishSubkeyFile.c`.  The original
  Blowfish code is C-style, and compiling it as C makes `S`/`P` tentative definitions
  (`COMMON`) instead of ordinary C++ `.bss` objects.  Gold then places them in the
  `.bss` COMMON block at the tail, exactly where the original has them.  The
  `.bss.blowfish` section-ordering entry is no longer needed and was removed.

The gate is **unchanged**: `byte_exact 4495`, `linked_exact 4496`,
`size 4524/4524`, `lint CLEAN`, `drift 0`.  The pass is a structural/data-layout
improvement, not a `byte_exact` gain: `S`, `P` and `SubkeyFile` now have the original
`.bss` addresses (`0x21c278`, `0x21d278`, `0x21d2f4`) instead of the pragma-era
`0x21c2d4`, `0x21d2d4`, `0x21c2ac`.

## Baseline

Commands:

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint -j8
cmake --build cmake-build-match --target drift -j8
```

Baseline at `16e5c9a4` (both pragmas present):

```
verify: byte 4495  linked 4496  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
size exact 4524 / 4524
lint: CLEAN ✓
drift: 0 field(s) across 0 class(es)
```

The prompt's stated baseline `byte 4494` is one commit stale; this checkout's HEAD is
`4495`.  All numbers below are measured on this checkout.

Pragma inventory before the pass:

```sh
grep -RIn --exclude-dir=.git --exclude-dir=cmake-build-match \
  '#pragma[[:space:]]*clang[[:space:]]*section' src
```

```
src/game/ship/PlayerCreature.cpp:19:#pragma clang section rodata=".rodata"
src/game/ship/PlayerCreature.cpp:21:#pragma clang section rodata=""
src/engine/file/BlowfishSubkeyFile.cpp:12:#pragma clang section bss=".bss.blowfish"
src/engine/file/BlowfishSubkeyFile.cpp:15:#pragma clang section bss=""
```

After the pass the same command prints nothing.

## A/B table

Every row is a full `cmake --build cmake-build-match --target check -j8`; the
`PlayerCreature` row that removes the pragma without `[[gnu::used]]` is the only
negative experiment.  `rodata diff` is the raw byte difference over
`0x1c9e10..0x1c9e10+0x41b67`.

| variant | byte | linked | size | `.rodata` diff | `.bss` size | `S` | `P` | `SubkeyFile` |
|---|---:|---:|---:|---:|---:|---|---|---|
| baseline: both pragmas | 4495 | 4496 | 4524/4524 | 0 | `0x69ac` | `0x21c2d4` | `0x21d2d4` | `0x21c2ac` |
| `PlayerCreature` `[[gnu::used]]`, Blowfish pragma + `.bss.blowfish` entry | 4495 | 4496 | 4524/4524 | 0 | `0x69ac` | `0x21c2d4` | `0x21d2d4` | `0x21c2ac` |
| `PlayerCreature` plain `const` (no pragma), Blowfish C | **4480** | 4496 | 4524/4524 | 4219 | `0x69ac` | `0x21c278` | `0x21d278` | `0x21d2f4` |
| Blowfish C++ without pragma, `PlayerCreature` `[[gnu::used]]` | **4477** | 4496 | 4524/4524 | 0 | `0x69a4` | `0x2179a4` | `0x2189a4` | `0x21d2ec` |
| Blowfish C alone, `PlayerCreature` pragma | 4495 | 4496 | 4524/4524 | 0 | `0x69ac` | `0x21c278` | `0x21d278` | `0x21d2f4` |
| **landed: both replacements** | **4495** | **4496** | **4524/4524** | **0** | **`0x69ac`** | **`0x21c278`** | **`0x21d278`** | **`0x21d2f4`** |

Original for comparison:

```
S          0x21c278  4096
P          0x21d278    72
SubkeyFile 0x21d2f4     4
.bss       0x216970  0x69ac
```

Address commands:

```sh
readelf -sW ../_work/bins/android_2.0.16_libgof2hdaa.so | grep -E ' (S|P|SubkeyFile)$'
readelf -sW cmake-build-match/libgof2hdaa.so          | grep -E ' (S|P|SubkeyFile)$'
readelf -SW cmake-build-match/libgof2hdaa.so          | grep -E '\.bss|\.rodata'
```

## 1. `PlayerCreature.cpp` — `.rodata` selection without the pragma

The original keeps the 16-byte table in plain `.rodata` at `0x201dd0`.  Clang 7 emits
the same source as an `internal unnamed_addr constant`, which lands in the mergeable
`.rodata.str4.4` group.  The table's bytes are unchanged; only its input section and
therefore its linked address move.  Removing the pragma alone costs **15**
`byte_exact` functions (4495 -> 4480) and changes 4,219 `.rodata` bytes.

Object-level experiments (exact compile line taken from
`cmake-build-match/compile_commands.json`):

| source form | section | function bytes vs pragma form |
|---|---|---|
| `#pragma clang section rodata=".rodata"` + `static const int[4]` | `.rodata` | reference |
| plain `static const int[4]` | `.rodata.str4.4` | identical |
| plain + `-fno-merge-all-constants` | `.rodata.str4.4` | identical |
| plain + `-mno-global-merge` | `.rodata.str4.4` | identical |
| plain + `-fno-common` | `.rodata.str4.4` | identical |
| `alignas(4/8/16)` | `.rodata.str4.4/.8/.16` | identical |
| `static const volatile int[4]` | `.rodata` | **different** scheduling/register allocation in `PlayerCreature` ctor and `rage` |
| `[[gnu::used]] static const int[4]` | `.rodata` | **identical** |

The `[[gnu::used]]` form was checked by diffing the object disassembly of every
`PlayerCreature` function against the pragma object; the diff was empty.  It also keeps
the table at object `.rodata` offset 0 and final VMA `0x201dd0`, so `.rodata` remains
byte-identical (0 differing bytes).

This is the minimal source-level replacement found.  `volatile` is semantically wrong
for the original table and changes codegen; passing the table address to an external
function also forces plain `.rodata` but adds a call.  `[[gnu::used]]` changes only
address significance and section selection.

## 2. `BlowfishSubkeyFile.cpp` -> `.c` — COMMON placement without the pragma

The original `S`/`P` are not ordinary early `.bss` objects.  They sit at the tail of
the regular `.bss`, immediately before the COMMON block:

```
original .bss tail:
  0x21c278  S                 4096
  0x21d278  P                   72
  0x21d2c0  g_android_leaderboard_scores
  ...
  0x21d2f4  SubkeyFile
```

The original Blowfish code is C-style (`extern "C"` names, C stdio), and the source
dump has no witness for it (`_work/original_layout/tu_mapping.md` says "blowfish C code,
likely separate .c originally").  In C, `unsigned int S[4][256];` at file scope is a
tentative definition and clang 7's default `-fcommon` makes it `COMMON`.  Gold places
COMMON symbols in the `.bss` COMMON block at the tail.  In C++, the same definition is
an ordinary `.bss` object, and clang 7's C++ default is `-fno-common`; `-fcommon` on
C++ did not change the symbol to `COMMON` in this toolchain.

Measured object symbols:

```sh
# C++ (no pragma)
nm -S BlowfishSubkeyFile.cpp.obj | grep -E ' S$| P$'
# 00001000 00000048 B P
# 00000000 00001000 B S

# C
nm -S BlowfishSubkeyFile.c.obj | grep -E ' S$| P$'
# 00000048 00000048 C P
# 00001000 00001000 C S
```

The C conversion does not change any Blowfish function body.  Diffing
`opensubkeyfile`, `F`, `Blowfish_encipher`, `Blowfish_decipher` and
`InitializeBlowfish` between the C++ and C objects produced an empty diff.  The C
object's `.text`/`.ARM.exidx`/`.ARM.extab` sections are also the same sizes as the C++
object's.

With the C++ file and no pragma, `S`/`P` move to the early Blowfish link position,
`.bss` shrinks by 8 bytes (`0x69ac -> 0x69a4`), and the gate drops 18 functions
(4495 -> 4477).  With the C file, `.bss` is `0x69ac`, `S`/`P`/`SubkeyFile` are exact,
and the gate is unchanged.

`tools/gen_section_order.py` no longer appends `.bss.blowfish`; the COMMON block is
already handled by leaving `.bss` unlisted.  The section-ordering file therefore drops
from 5,886 to 5,885 names (1,362 -> 1,361 local), with no output change.

## 3. Final gate

```sh
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint -j8
cmake --build cmake-build-match --target drift -j8
```

```
verify: byte 4495  linked 4496  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
size exact 4524 / 4524
lint: CLEAN ✓
drift: 0 field(s) across 0 class(es)
```

Section sizes/bases after the pass:

```
.text       0x00069440  0x1337c8
.rodata     0x001c9e10  0x041b67
.ARM.exidx  0x0019cc08  0x006f70
.ARM.extab  0x001a3b78  0x026290
.bss        0x00216970  0x0069ac
```

Pre-existing observation, not caused by this pass: current HEAD's `.ARM.extab` is
`0x26290`, while the original is `0x26298` (the `docs/IDENTICALITY.md` text still says
`0x26298`).  The baseline `.so` built from HEAD has the same `0x26290`; this pass does
not change it.

## Proven vs hypothesis

**Proven by measured A/B:**

* The only two `#pragma clang section` uses were the ones above; both can be removed.
* Plain removal of the `PlayerCreature` pragma costs 15 `byte_exact` in the landed
  context; `[[gnu::used]]` restores the original `.rodata` section and keeps every
  `PlayerCreature` function byte-identical.
* Compiling Blowfish as C makes `S`/`P` `COMMON`; gold places them at the original
  tail addresses, and `.bss` returns to the original `0x69ac` size.
* The landed change is gate-neutral (`4495/4496`, size `4524/4524`), lint-clean and
  drift-free, and removes the need for the `.bss.blowfish` ordering entry.

**Hypothesis / not overclaimed:**

* The original Blowfish TU was compiled as C (or with an equivalent COMMON-producing
  mode).  This is strongly supported by the exact COMMON layout and the C-style code,
  but there is no source-dump witness for the original filename/language.
* `[[gnu::used]]` is a reconstruction device, not evidence that the original source
  spelled the table that way.  It is the smallest source-level form found that selects
  plain `.rodata` without changing codegen.
* The `.ARM.extab` 8-byte shortfall is pre-existing and unrelated to either pragma.
