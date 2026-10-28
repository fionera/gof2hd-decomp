# .text layout: local code before the first export

## Symptom

`verify`'s TEXT_LAYOUT target reported 0%: our `.text` began with an ARM veneer at
0x69440 while the original begins with a Thumb prologue, and the first exported symbol
`Java_..._BackButtonPressed` sat at 0x6bdb1 instead of 0x69579 — ~10,296 B of extra
local code at the front, while the total `.text` was only +3,080 B.

## Original layout (first 0x3000 bytes)

| addr | content |
|------|---------|
| 0x69440 | `_GLOBAL__sub_I_Engine.cpp` (`.text.startup`, 0x80) |
| 0x694c0 | `_GLOBAL__sub_I_LODManager.cpp` (`.text.startup`, 0x1c) |
| 0x694dc | `_GLOBAL__sub_I_ParticleSettings.cpp` (`.text.startup`, 0x58) |
| 0x69534 | crtbegin_so.o code (plain `.text`, 0x44) |
| 0x69578 | `Java_..._BackButtonPressed` (first export) |

The original was linked with no ordering file, so gold's default applies: `.text.startup`
sorts ahead of everything, then plain input order.

## What we placed first (before the fix)

`tools/gen_section_order.py` did not list `.text.startup`, crtbegin's plain `.text`, or the
plain `.text` group, and had no catch-all. gold gives every *unlisted* section ordering
index 0, so all of them sorted before the listed ones, in input order:

1. crtbegin_so.o's plain `.text` (ARM `__on_dlclose`/`__do_global_dtors_aux` veneers) — the
   ARM code at 0x69440,
2. the three `.text.startup` ctors,
3. libc++abi/libandroid_support/libgcc local sections (`fallback_malloc.o`,
   `posix_memalign.o`, `_aeabi_ldivmod.o`, …),
4. only then the address-sorted `.dynsym` list, with `Java_..._BackButtonPressed` at 0x6bdb1.

## Fix

`tools/gen_section_order.py` now emits, in order:

1. `.text.startup` (all objects; gold groups same-named sections in input order),
2. `.text.crtbegin` — `CMakeLists.txt` renames crtbegin_so.o's `.text` with `objcopy
   --rename-section` so it can be placed separately (a plain `.text` entry would group it
   with every archive's plain `.text`),
3. the address-sorted `.dynsym` list with local sections slotted as before,
4. `.text` (the plain-.text group: libcrypto's sha256.o / armv4cpuid.o / sha256-armv4.o,
   libgcc) at the first original address of a symbol defined by such a member,
5. a trailing `*` glob so unlisted sections get the *last* ordering index instead of 0.

Result: `.text` now starts byte-for-byte like the original and
`Java_..._BackButtonPressed` is at 0x69578. The first address divergence is
`MeshReadData` at 0x6d050 (ours -8 B: that function is 2624 B vs the original's 2632 B).

## Keeping `.text` at 0x1343d0

Reordering changes inter-section alignment padding: with the corrected order the total
`.text` came out at 0x1343c8 (-8). The plain `.text` block is 0x44 B larger than the
original's, which pushes the next 32-byte-aligned section (`__cxa_end_cleanup`) over a
boundary and adds 0x18 B. Starting the block one ZIP TU earlier (`PLAIN_TEXT_LEAD = 0x180`,
i.e. before `.text.zip_fopen`) removes that padding; with the existing `levelTextAnchor`
grown by one volatile store (0x28 -> 0x2c) the total is exactly 0x1343d0 again.

Gate after the change: byte_exact 1353 -> 1361, linked_exact 4485, .text 0x1343d0,
lint CLEAN, drift 0.

## EXTAB_LAYOUT pass: missing `-funwind-tables`

### Symptom

`.ARM.extab` was 4,572 B smaller than the original (orig `0x26298`, ours `0x250b4`).
`.ARM.exidx` was 552 entries short (orig 3,566, ours 3,014). The task's EXTAB_LAYOUT
report was 0% at 151,740/156,312 B.

### Entry-by-entry method

`readelf -SW` gives the two section ranges. Each `.ARM.exidx` entry is
`(prel31 function, word2)`; `word2 == 1` is `cantunwind`, `word2 & 0x80000000` is the
compact model (personality 0 carries its unwind bytes in the exidx word itself and
consumes no `.ARM.extab`), and `word2 & 0x80000000 == 0` is a prel31 reference into
`.ARM.extab` (base = the address of `word2`). Mapping each entry to its `.dynsym`
function (exact start, or nearest preceding export for locals) gives the per-function
table.

Before the fix the split was:

| binary | exidx entries | cantunwind | compact0 | prel31 -> extab |
|---|---:|---:|---:|---:|
| original | 3,566 | 272 | 856 | 2,438 |
| ours (before) | 3,014 | 566 | 384 | 2,064 |

The prel31 counts were already close, but the *locations* were wrong: whole TUs had
`cantunwind` where the original had real unwind. The largest cluster was the vendored
zlib/libzip C objects (203 original extab entries, ours 1). The rest was the app C++
tree: the project compile line had `-ffunction-sections` but not `-funwind-tables`, so
clang emitted `cantunwind` for functions whose original build had unwind data.

### Fix

Add `-funwind-tables` in both compile flag sets:

* `CMakeLists.txt`: `target_compile_options(gof2 PRIVATE -ffunction-sections -funwind-tables)`
* `third_party/build_match_lib.cmake`: add `-funwind-tables` to `CC_BASE` (zlib/libzip;
  libcrypto is GCC 4.9 and already carries its own EH tables).

This changes only EH metadata; `.text` stays exactly `0x1343d0`.

### Result

| binary | exidx entries | cantunwind | compact0 | prel31 -> extab | `.ARM.extab` |
|---|---:|---:|---:|---:|---:|
| original | 3,566 | 272 | 856 | 2,438 | 156,312 |
| ours (after) | 3,290 | 6 | 843 | 2,441 | 156,280 |

The extab gap is now **32 B (99.98%)**, down from 4,572 B.

The remaining 276-entry exidx gap is almost entirely duplicate `cantunwind`/`compact0`
entries at shared addresses: the original emits an extra entry per C1/C2 alias (and a
few other shared-start functions), e.g. `NFC::NFC()`, all the shader constructors, and
the zlib entry points. Those entries carry no `.ARM.extab` payload, so they do not
affect the extab size; reproducing them is an alias/duplicate-emission question, not an
unwind-data question.

The residual 32 B is the net of nine non-exact functions' unwind-table lengths:

| function | orig extab B | ours extab B | delta |
|---|---:|---:|---:|
| `HangarWindow::render()` | 948 | 900 | -48 |
| `Level::createShip(int,int,int,Waypoint*,bool,bool)` | 336 | 316 | -20 |
| `RecordHandler::recordStoreWrite(int)` | 656 | 640 | -16 |
| `Level::createMission()` | 956 | 952 | -4 |
| `Hud::draw(long,long,Player*,unsigned char,bool,unsigned int,bool,unsigned int)` | 476 | 492 | +16 |
| `SpaceLounge::startChat()` | 2636 | 2644 | +8 |
| `StarMap::draw()` | 116 | 120 | +4 |
| `LevelScript::process(int)` | 348 | 352 | +4 |
| `Generator::createMission(Agent*,Array<SolarSystem*>*)` | 172 | 176 | +4 |

These are the same functions that still differ in code size; fixing their bodies will
move the last 32 B.

Gate after the change: byte_exact 1361, linked_exact 4485, `.text` 0x1343d0, lint CLEAN,
drift 0.
