# `.data` / `.data.rel.ro` content map (w3443)

Research pass on the non-`.text` data sections. Everything below is measured against
`../_work/bins/android_2.0.16_libgof2hdaa.so` (sha256
`d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483`) with the
`cmake-build-match` build and a map-file relink. No `asm`, no `__attribute__`, no linker
flags were added.

## Baseline (HEAD `01f88b3e`, before this pass)

```
cmake --preset default
cmake --build cmake-build-match --target check -j8
verify: byte 4252  linked 4496  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Every ELF section is already size- and base-exact (`.text 0x1337c8 @ 0x69440`,
`.rodata 0x41b67 @ 0x1c9e10`, `.data.rel.ro 0x2fe8 @ 0x20cbb0`, `.got 0x430c @ 0x20fcf4`,
`.data 0x296c @ 0x214000`, `.bss 0x69ac @ 0x216970`).

## Measurement method

The gate's `data: exact 843 of 843` is not a byte check: `tools/datadiff.py` normalizes
`R_ARM_ABS32` relocations by **symbol name only and ignores the addend**. Two independent
measurements were used here:

1. Raw section byte diff (`Elf32` section bytes, original vs ours).
2. A map-file relink that dumps every input section and every relocation in `.data` /
   `.data.rel.ro`, then classifies each relocation as same / symbol-diff / addend-diff /
   relative-target / type-diff.

Relink command (extracted from `build.ninja`, `strip` removed, `-Map` added):

```
cd cmake-build-match
.../ld.gold -shared -soname libgof2hdaa.so --build-id -z now --exclude-libs=libgcc.a \
  --section-ordering-file=section_order.txt -Map=/tmp/w3443_data.map \
  -o /tmp/w3443_data_unstripped.so ... @exidx_markers/link_order.rsp ...
```

## Baseline content map

| section | size | differing bytes | differing words | relocs | same-offset |
|---|---|---|---|---|---|
| `.data.rel.ro` | 0x2fe8 | 900 | 816 | 2821 | 2814 |
| `.data` | 0x296c | 1626 | 1303 | 153 | 151 |
| `.rodata` | 0x41b67 | 13266 | — | — | — |
| `.got` | 0x430c | 0 | — | — | — |
| `.bss` | 0x69ac | 0 | — | — | — |

`.data.rel.ro` relocation classes: same 1843, relative-target-same-section 797,
symbol-diff 171, orig-only 7, ours-only 7, type-diff 2, relative-target-`.bss`->`.data` 1.
`.data` classes: same 88, relative-target-same-section 41, addend-diff 22, ours-only 2,
orig-only 2.

Top differing input sections (bytes): `.data.rel.ro` ModStation 84/340, DialogueWindow
65/252, Radio 65/252, Level 18/36, PlayerAsteroid 7/128, PlayerStaticFar 5/116, …
`.data` ImageFactory 626/1664, ModStation 266/1240, DialogueWindow 240/1240, Radio 240/1240,
ListItemWindow 82/248, MiningGame 46/224, Engine 30/40, VirtualInput 22/4268, MGame 13/60, …
Almost all of this is *address shift*, not wrong table content.

## Fix 1 — `VirtualInput::keys[]` pointer addends (committed)

Original `keys` at `.data+0x10`, 48 entries stride 0x58, `touchX/touchY` at +0x28/+0x2c.
The original stores `other_buttons_x/y` / `sub_menu_buttons_x/y` **with addends**; ours
stored the base pointers. Measured by the relink map (`R_ARM_ABS32`, addend in the word):

| key index | name | field | original addend | ours (before) |
|---|---|---|---|---|
| 25 | Fire (25, IsDialogVisible) | other_buttons_x/y | 12 (= +3 ints) | 0 |
| 33 | Back (202, IsDialogVisible) | other_buttons_x/y | 8 (= +2 ints) | 0 |
| 38 | "1" | sub_menu_buttons_x/y | 0 | 0 |
| 39..47 | "2".."0" | sub_menu_buttons_x/y | 4,8,12,16,20,24,28,32,36 (+1..+9) | 0 |

Fix: `Globals::other_buttons_x + 3`, `+ 2`, and `Globals::sub_menu_buttons_x + 1..+9`
(the arrays are `int[10]`, so the pointer addend is the int index × 4).

A/B: all 22 `addend-diff` relocations disappear; `.data` same 88 -> 110; `.data` differing
bytes 1626 -> 1604. Gate unchanged (byte 4252 / linked 4496) because `datadiff.py` ignores
addends — a proven gate blind spot.

## Fix 2 — `gStoryFaceTable` has 63 entries; drop the MenuTouchWindow anchor (committed)

Evidence from the original:

* `_ZTV10ModStation` = 0x20d9e4, size 80 (0x50) -> table starts at 0x20da34.
* `_ZTI10ModStation` = 0x20db30. `0x20db30 - 0x20da34 = 0xfc = 252` bytes = **63 pointers**.
* Original words at 0x20da34: `[0]=0x217bf8` (BSS), `[1..3]=gStoryFaces[0..2]`,
  `[4]=gPirateFace+5`, `[5]=gStoryFaces[3]`, … `[21]=gStoryFaces[19]`,
  `[22]=gPirateFace`, `[23]=gStoryFaces[20]`, … `[62]=gStoryFaces[59]`.
  `0x217bf8` is the only uninitialised face array (`gPirateGreetingFaceB`).

So the table is `{gPirateGreetingFaceB, gStoryFaces[0..2], gPirateFace+5,
gStoryFaces[3..19], gPirateFace, gStoryFaces[20..59]}` = 63 entries. Ours had 62 and
started at `gStoryFaces[0]`, so `_ZTI10ModStation` sat 4 bytes early at 0x20db2c.

The 4-byte `.data.rel.ro` slot that `MenuTouchWindow.cpp` carried at 0x20de20
(`mtwPlayerStaticFarTypeInfoRef`) was a false anchor: 0x20de20 is the `__base_type`
field of `_ZTI13PlayerStation` (`PlayerStation : public PlayerStaticFar`), which the
compiler already emits from `PlayerStation.cpp` (relocation at typeinfo +8). With the
62-entry table the anchor happened to land on 0x20de20 and masked the 4-byte deficit;
with the correct 63-entry table it pushed `_ZTV7BombGun` to 0x20de28 instead of 0x20de24.

Fix: table `[63]` with the leading `gPirateGreetingFaceB`; delete the anchor.

A/B: `_ZTI10ModStation` 0x20db2c -> 0x20db30 (matches), `_ZTV7BombGun` stays 0x20de24,
`.data.rel.ro` differing bytes 900 -> 869, all `symbol-diff`/`orig-only`/`ours-only`/
`type-diff` classes vanish (same-offset relocations 2814 -> 2821). Gate: byte 4252 -> 4253,
linked 4496.

## Fix 3 — `GameText.cpp` link order (committed)

Original `.data` symbol order:

```
0x2150c0  _ZN11AbyssEngine5i_fovE                    (PaintCanvas.cpp)
0x2150c4  _ZN8GameText11currentLangE                 (GameText.cpp, 2 bytes + 2 pad)
0x2150c8  _ZN11AbyssEngine6Engine14lodBiasDiffuseE   (Engine.cpp)
```

Ours had `currentLang` at 0x216910 (near the end) and `lodBiasDiffuse` at 0x2150c4, i.e.
GameText was linked after every TU with a strong function. `tools/gen_link_order.py` keys
function TUs by their lowest original `.text` address and data-only TUs by their lowest
original `.dynsym` index, so a data-only TU always sorts after all function TUs.

Fix: a second manual link-order move in `gen_link_order.py`, `GameText.cpp.obj` immediately
before `Engine.cpp.obj`. No TU between PaintCanvas and Engine owns a `.data` section, so
"before Engine" is exactly "after PaintCanvas" for `.data`.

A/B (scratch `link_order.rsp`, same gold command):

| metric | before | after |
|---|---|---|
| `currentLang` VMA | 0x216910 | 0x2150c4 |
| `lodBiasDiffuse` VMA | 0x2150c4 | 0x2150c8 |
| `particleMultiply` VMA | 0x21690c | 0x21691c |
| `.data` differing bytes | 1604 | 260 |
| `.data.rel.ro` differing bytes | 869 | 744 |
| byte_exact / linked_exact | 4253 / 4496 | **4254 / 4496** |

The `MGame` 12-byte gap that looked like a size delta is just its `alignas(16)` `.data`
section: after the +4 shift it pads from 0x2168d4 to 0x2168e0, exactly the original.

## Remaining difference — `gStoryFaces` / `gPirateFace` emission order (hypothesis)

After fixes 1–3 the only real `.data` content difference is inside `ModStation.cpp`:
the original lays out `gStoryFaces` (1200 B) at 0x2151e0 and `gPirateFace` (40 B) at
0x215690; clang emits them in the opposite order (`gPirateFace` at 0x2151e0,
`gStoryFaces` at 0x215208). `gStoryFaces` is only referenced by the `gStoryFaceTable`
initializer, while `gPirateFace` is referenced directly in functions; clang 7.0.2 `-Oz`
emits `.data` globals in first-use order, so the function use wins.

This accounts for 257 of the 260 remaining `.data` bytes and 63 of the 673 remaining
`.data.rel.ro` relocations (the table entries that point at the two arrays). The other
3 `.data` bytes are `__cxa_terminate_handler` / `__cxa_unexpected_handler` pointing at
libc++abi `.text` (0x193f25/0x194015 original vs 0x193f01/0x193ff1 ours — a vendored
libc++abi layout delta, not a data-content bug).

Two A/B attempts to force the order were **not** kept because they regress `byte_exact`:

1. One `struct StoryFaceData { int faces[60][5]; int pirate[10]; }` — puts `.data` at
   74 differing bytes, but clang emits `add.w r3, r0, #0x4b0` for `gStoryFaceData.pirate`
   instead of a direct literal load, growing `.text` by 0x20 (byte 4254 -> 2459).
2. One flat `int gStoryFaceData[62][5]` with `#define gPirateFace
   (reinterpret_cast<int *>(gStoryFaceData[60]))` — same `.data` result, same 0x20
   `.text` growth.

Both were reverted. The open hypothesis is that the original source had a function-level
use of `gStoryFaces` before any `gPirateFace` use (the source comment already notes that
`gStoryFaces[60]` aliases `gPirateFace`), or declared the pair in a form clang does not
reorder. A fix must keep `gPirateFace` a standalone symbol so the direct-address codegen
is preserved.

## Final numbers (this pass)

```
cmake --build cmake-build-match --target check -j8
verify: byte 4254  linked 4496  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

| section | before | after |
|---|---|---|
| `.data.rel.ro` differing bytes | 900 | 744 |
| `.data.rel.ro` same-offset relocs | 2814 / 2821 | 2821 / 2821 |
| `.data` differing bytes | 1626 | 260 |
| `.data` same relocs | 88 | 151 |
| `.rodata` differing bytes | 13266 | 13266 (unchanged) |
| `.got` / `.bss` differing bytes | 0 / 0 | 0 / 0 |
| byte_exact / linked_exact | 4252 / 4496 | **4254 / 4496** |

Exported-object byte comparison (relocation-normalized) still reports `_zip_err_str`,
`z_errmsg`, `LISTITEMWINDOW_UNITS`, `property_name_purge`, `property_name_pause`,
`ndkEntrance`, `NFC::interface_path` as differing, but every one is a uniform `.rodata`
target shift (original target 222274 vs ours 222310, i.e. +0x24); they are `.rodata`
content/order deltas, not `.data` content deltas.

## Proven vs hypothesis

* **Proven:** the 22 `keys` addends; the 63-entry `gStoryFaceTable` and the
  `_ZTI13PlayerStation` base-type relocation at 0x20de20; the GameText/Engine `.data`
  link order and the `MGame` alignment pad; `datadiff.py` ignores ABS32 addends.
* **Hypothesis:** the exact source form that made the original emit `gStoryFaces` before
  `gPirateFace`. The ordering difference itself is measured; the cause is inferred from
  clang's first-use emission order.
