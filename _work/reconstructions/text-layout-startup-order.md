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
