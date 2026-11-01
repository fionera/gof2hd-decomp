# `.dynsym` order and `DT_RELCOUNT` (w3444)

Status: research note + one landed source fix (2026-11-01). Worktree `wt/w3444`, branch
`wave87-w3444`, based on master `01f88b3e`. Every number below was measured against
`../_work/bins/android_2.0.16_libgof2hdaa.so` with the repo's NDK r18b gold
(`GNU gold (GNU Binutils 2.27.0.20170315) 1.12`; the original's `.note.gnu.gold-version` is
`gold 1.12`). The gold binary is statically linked (no `libstdc++.so` in `ldd`), so its
`std::unordered_map` implementation is fixed and reproducible.

## TL;DR

* `DT_RELCOUNT` is **not** an independent link knob: it is exactly the number of leading
  `R_ARM_RELATIVE` entries in the combreloc-sorted `.rel.dyn`. The original has 1162; we had
  1161 because our `ModStation::gStoryFaceTable` had 62 entries instead of the original's 63.
  The missing entry is a leading pointer to `gPirateGreetingFaceB` (a `.bss` array), i.e. one
  `R_ARM_RELATIVE` relocation.
* A second source artifact (`mtwPlayerStaticFarTypeInfoRef` in `MenuTouchWindow.cpp`) was a
  hand-placed 4-byte `.data.rel.ro` "anchor" that had been compensating for the missing table
  entry. Removing it together with the table fix makes the whole `.data.rel.ro` exported-symbol
  layout **exact** (15 mismatches -> 0) and the relocation type counts **exact**
  (1817 `ABS32` / 614 `GLOB_DAT` / 1162 `RELATIVE`).
* Gate: **byte_exact 4252 -> 4253**, linked_exact 4496 unchanged, lint CLEAN, drift 0.
  `.rel.dyn` (offset,type) parity 714 -> 3071 of 3593; the first 1162 relative entries
  144 -> 1097 match.
* `.dynsym` order is the iteration order of gold's `std::unordered_map` keyed by
  `(namepool_offset, version_offset)` — **not** input order. It is a derived observable of the
  original's exact symbol-insertion order (TU membership + link order), and no linker flag can
  fix it. The archive tail already matches (one 1095-entry block); the app region does not.

## Baseline (before any change)

```
cmake --preset default
cmake --build cmake-build-match --target check -j8
```

```
verify: byte 4252  linked 4496  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`cmake --build cmake-build-match --target lint` -> `CLEAN ✓`; `... --target drift` ->
`0 field(s) across 0 class(es)`.

## 1. `DT_RELCOUNT` — proven cause and fix

### What it is

```
readelf -dW ORIG | grep RELCOUNT     # 0x6ffffffa (RELCOUNT) 1162
readelf -rW ORIG | awk '/\.rel\.dyn/{f=1;next} /\.rel\.plt/{f=0} f&&/R_ARM_/{print $3}' \
  | sort | uniq -c
```

Original: `1817 R_ARM_ABS32`, `614 R_ARM_GLOB_DAT`, `1162 R_ARM_RELATIVE`.
Before fix ours: `1818 ABS32`, `614 GLOB_DAT`, `1161 RELATIVE` (same 3593 total).
`DT_RELCOUNT` equals the `R_ARM_RELATIVE` count because `-z combreloc` (gold default) sorts
the relative relocations first. So the delta was exactly **one pointer slot that is local
(`R_ARM_RELATIVE`) in the original but symbol-absolute (`R_ARM_ABS32`) in ours.**

### The one missing relocation

Matching the relative relocations by addend and by count histogram isolates it: the original
has 598 distinct relative targets with multiplicity 1, ours 597; every other multiplicity is
identical. The extra target is `0x217bf8` (`.bss`), and its slot is `0x20da34` — the first word
after the `ModStation` vtable (`_ZTV10ModStation` at `0x20d9e4`, size 80 = `0x50`).

```
orig 0x20da34: R_ARM_RELATIVE -> 0x217bf8 (.bss)
ours 0x20da34: R_ARM_RELATIVE -> 0x215204 (first gStoryFaces row)
```

The table is `ModStation::gStoryFaceTable`: orig has **63** entries, ours had 62. The 62
entries we had match orig[1..62] exactly; orig[0] is the extra `.bss` pointer. The `.bss`
target is `gPirateGreetingFaceB`: `ModStation::OnKeyPress` (at `0xd96b6`) loads it and it
resolves to `0x217bf8` in the original, the same address as the extra table slot.

The call site is `ModStation::OnUpdate` (byte-exact, both 3652 B at `0xd9c78`):

```
da6ee: movs r6, #58        ; face = 58
da6fe: moveq r6, #57       ; textId == 2717 -> 57
da708: movne r6, #0        ; firstVoice -> 0
da736: ldr r0, [pc, #884]  ; literal
da738: add r0, pc          ; r0 = 0x20da34 (gStoryFaceTable)
da73a: ldr.w r3, [r0, r6, lsl #2]
```

So index 0 is live and the original's table really has the leading `gPirateGreetingFaceB`
entry. Our table was one entry short and therefore also *semantically* off by one for
`face = 0/57/58`.

### The compensating anchor

`src/game/ui/MenuTouchWindow.cpp` carried:

```cpp
// .data.rel.ro slot at 0x20de20: the original holds an R_ARM_ABS32 to the
// PlayerStaticFar typeinfo here. ...
extern const char _ZTI15PlayerStaticFar[];
[[gnu::used]] static const void *const mtwPlayerStaticFarTypeInfoRef = _ZTI15PlayerStaticFar;
```

This was a hand-placed 4-byte `.data.rel.ro` anchor. The relocation it reproduced at
`0x20de20` is actually the base pointer of `_ZTI13PlayerStation` (which is
`_ZTI15PlayerStaticFar`); once the 63-entry table is present, the anchor is redundant and
must go, otherwise everything after `_ZTV7BombGun` shifts `+4`.

### Fix (landed)

```diff
-static int *const gStoryFaceTable[62] = {
+static int *const gStoryFaceTable[63] = {
+    gPirateGreetingFaceB,
     gStoryFaces[0], gStoryFaces[1], ...
```
```diff
-extern const char _ZTI15PlayerStaticFar[];
-[[gnu::used]] static const void *const mtwPlayerStaticFarTypeInfoRef = _ZTI15PlayerStaticFar;
```

### A/B measurements (same tree, full `check`)

| metric | before | after |
|---|---:|---:|
| `byte_exact` | 4252 | **4253** |
| `linked_exact` | 4496 | 4496 |
| `DT_RELCOUNT` | 1161 | **1162** |
| `R_ARM_RELATIVE` | 1161 | **1162** |
| `R_ARM_ABS32` | 1818 | **1817** |
| `R_ARM_GLOB_DAT` | 614 | 614 |
| `.data.rel.ro` exported-symbol mismatches | 15 | **0** |
| `.data.rel.ro` differing bytes | 900 | 869 |
| `.rel.dyn` same `(offset,type)` at same index | 714 / 3593 | **3071 / 3593** |
| first 1162 relative entries matching | 144 | **1097** |
| `.text` / `.rodata` / `.data.rel.ro` / `.data` / `.bss` sizes | exact | exact |
| lint / drift | CLEAN / 0 | CLEAN / 0 |

The one function that flips to byte-exact is `_ZN14PlayerAsteroid16getQualityStringEv` (its
only remaining delta was a literal-pool pointer into `.data.rel.ro`).

The remaining `.data.rel.ro` byte diffs (869) are the *contents* of the table/vtables, whose
pointers into `.data` still differ because `.data` is mis-ordered (see §3); the exported
symbol *addresses* are now all exact.

## 2. `.dynsym` order — proven mechanism

### It is a hash-map iteration order, not input order

Gold's `Symbol_table::sized_finalize` (`gold/symtab.cc`) walks `this->table_`, which is

```cpp
typedef Unordered_map<Symbol_table_key, Symbol*, Symbol_table_hash, Symbol_table_eq>
    Symbol_table_type;
```

`Unordered_map` is `std::unordered_map` (`gold/system.h`), the key is
`(Stringpool::Key name_key, Stringpool::Key version_key)` and the hash is
`name_key ^ version_key` (`gold/symtab.h`). `name_key` is the symbol name's **offset in the
linker's name stringpool**, which is built in symbol-insertion order by
`namepool_.add_with_length(...)` in `Symbol_table::add_from_relobj`. So the dynsym order is the
iteration order of a hash map keyed by namepool offsets — not the order symbols were read.

Four independent observations confirm it in this tree:

1. **`islower`/`isxdigit` at index 3.** `gold -y islower` shows they are referenced only by
   `libc++abi.a(m003_cxa_demangle.o)`, which is pulled in at the very end of the link. Under
   insertion order they would be near the end; they are at dynsym index 3.
2. **A single dummy symbol reorders unrelated symbols.** Linking one extra global
   `aaaa…` (64 chars) right after `crtbegin_so.o` changes the relative order of 164 unrelated
   symbols (5414/5578 same index after removing the dummy). A constant shift of every later
   namepool offset permutes the hash buckets. Command:
   `clang -target armv7-… -fPIC -Oz -c /tmp/dummy.c` then relink with `/tmp/dummy.o`
   inserted after `crtbegin_so.o`.
3. **The archive tail matches exactly.** All names inserted after the app/import set have the
   same namepool offset regardless of the app insertion order (the *set* of preceding names is
   the same), and the archives are linked in the same order. The largest matching block is
   `orig[4131..5225] == ours[4131..5225]` (1095 entries), which is the archive/app tail.
4. **Determinism.** Two relinks of the same objects are byte-identical, so the order is a
   deterministic function of the inputs (gold is statically linked).

### Current divergence

```
entries 5578 / 5578, same symbol set
same index: 1410 / 5578
defined symbols: 5370 / 5370, same index 1404
difflib matching blocks: 115
first divergence: ours inserts islower, isxdigit at index 3
largest block: (4131,4131,1095)
```

`.dynstr` is emitted in dynsym order (the `st_name` offsets are monotonic in dynsym index in
both files), so it carries the same information; it is not an independent witness of the
namepool order.

### Why no flag fixes it

The order is a function of the *namepool offsets*, i.e. of the exact sequence of global symbol
names inserted by the link. That sequence is determined by (a) the object link order and
(b) each object's global-symbol table order, which in turn is TU membership. This is the same
long-term work as `_work/original_layout/tu_mapping.md` and `tools/gen_link_order.py`. Link
flags only reorder sections/symbols *after* the fact (`--sort-section=name` was already shown
to hurt `.text`), so they cannot reproduce a hash-map iteration order. The actionable path is
to make the app symbol insertion order match; the archive tail already does.

## 3. Next blocker exposed: `.data` ordering

With `.data.rel.ro` exported addresses exact, the first relative-relocation mismatch moved to
`0x2151a4` vs `0x2151a0` (`property_name_pause`/`property_name_purge`) and the `.data`
exported-symbol layout still has 76 mismatches. The dominant one is

```
_ZN8GameText11currentLangE   orig 0x2150c4   ours 0x216910
```

i.e. a 2-byte `.data` object placed at the end of `.data` instead of at `0x2150c4`. That is a
TU/data-emission-order issue, not a link flag. Fixing it should remove most of the remaining
`.rel.dyn` and `.data.rel.ro` content deltas.

## Reproduction commands

```sh
# baseline / gate
cmake --preset default
cmake --build cmake-build-match --target check -j8

# RELCOUNT + relocation type counts
readelf -dW ../_work/bins/android_2.0.16_libgof2hdaa.so | grep RELCOUNT
readelf -rW ../_work/bins/android_2.0.16_libgof2hdaa.so \
  | awk '/\.rel\.dyn/{f=1;next} /\.rel\.plt/{f=0} f&&/R_ARM_/{print $3}' | sort | uniq -c

# dynsym order
python3 - <<'PY'
import sys, difflib
sys.path.insert(0, 'tools/verify')
from elf import Elf32, SHT_DYNSYM
o = [s['name'] for s in Elf32('../_work/bins/android_2.0.16_libgof2hdaa.so')._symbols(SHT_DYNSYM)]
u = [s['name'] for s in Elf32('cmake-build-match/libgof2hdaa.so')._symbols(SHT_DYNSYM)]
print('same index', sum(a == b for a, b in zip(o, u)), '/', len(o))
print('blocks', len(difflib.SequenceMatcher(None, o, u, autojunk=False).get_matching_blocks()))
PY

# .data.rel.ro exported-symbol layout (0 == exact)
python3 - <<'PY'
import sys
sys.path.insert(0, 'tools/verify')
from elf import Elf32, SHT_DYNSYM
def d(p):
    return {s['name']: s['value'] for s in Elf32(p)._symbols(SHT_DYNSYM)
            if s['shndx'] != 0 and s['name']}
o = d('../_work/bins/android_2.0.16_libgof2hdaa.so')
u = d('cmake-build-match/libgof2hdaa.so')
print(sum(1 for n in set(o) & set(u) if 0x20cbb0 <= o[n] < 0x20fb98 and o[n] != u[n]))
PY
```

## Proven vs hypothesis

**Proven (command/measurement above):**
* `DT_RELCOUNT` == number of `R_ARM_RELATIVE`; delta was exactly one relocation.
* The missing relocation is the leading `gPirateGreetingFaceB` entry of a 63-entry `gStoryFaceTable`.
* The `MenuTouchWindow` anchor was compensating for it; removing both makes the
  `.data.rel.ro` exported layout exact and the relocation type counts exact.
* Gate improves 4252 -> 4253 byte_exact with no other metric moving.
* `.dynsym` order is gold's `std::unordered_map` iteration order keyed by namepool offsets
  (gold source + islower + dummy-symbol + tail + determinism evidence).

**Hypothesis / not yet proven:**
* That reproducing the original's exact global-symbol insertion order will reproduce the
  original's `.dynsym` order exactly. The mechanism says it must, but we have not reconstructed
  that insertion order.
* That the remaining `.data.rel.ro` content diffs and `.rel.dyn` mismatches are all downstream
  of the `.data` ordering (`GameText::currentLang` first). Strongly suggested by the first
  mismatch location, not yet fixed.
