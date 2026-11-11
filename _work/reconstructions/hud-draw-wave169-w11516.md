# Hud::draw — wave 169 (w11516): barH-first declaration order (raw 582 -> 548)

Target: `_ZN3Hud4drawExxP9PlayerEgobjj` (`src/game/ui/Hud.cpp`, `Hud::draw`),
original **9,332 B**, VMA `0x163b90`.

| metric | entry (wave-168) | landed |
|---|---:|---:|
| raw same-address diff | **582** | **548** |
| aligned (instruction-index) diff | 228 | 220 |
| size | 9,332 | 9,332 |
| `.text` diff bytes | 81,642 | **81,608** |
| `.ARM.extab` | 0x26298 | 0x26298 |
| `.rel.plt` | identical | identical |

Gate after the change: `check` PASS — byte_exact 4507/4524,
linked_exact 4507/4524, size-exact 4524/4524, data_exact 843/843, imports 207,
lint CLEAN, drift 0, parity 5370/5370.

## Context: the 572 T-first shape is still LSDA-fatal

Wave-169 w11416 found `T W D Y H F C P y` (String first) = 572 raw but the
show-block call-site table splits at the `float filled` statement (SetColor +
DrawImage2D get their own landing-pad thunk), growing Hud::draw's LSDA from
77 to 78 entries, `.ARM.extab 0x26298 -> 0x2629c`, `.rodata +0x10`, and
byte_exact 4507 -> 4044.  Re-measured here; all T-first permutations of
`T W D Y H` are 78 entries.  `filled` before `SetColor` merges the entries
(77) but is size 9,336 and >= 1,169 raw.

## Landed change

In the jump-drive show block the declaration order is now `barH, text, halfW,
dummy, barYOffset` (was `barYOffset, barH, text, dummy, halfW`), and the
size-anchor volatile read now re-reads the bar offset instead of the bar top:

```cpp
int barH = Globals::Canvas->GetImage2DHeight(this->image_0x37c);
String text(*Globals::gameText->getText(this->field_0x277 ? 318 : 317), false);
int halfW = (int) ((float) Globals::Canvas->GetImage2DWidth(this->image_0x37c) * 0.5f);
volatile int dummy = Globals::layout->field_0x218;
int barYOffset = Globals::layout->field_0x218;
```

This keeps the 77-entry LSDA (`.ARM.extab` 0x26298, `.rodata` unmoved) and is
size-exact.  The `barH`-first load lets the allocator keep `barH` in `r8` and
`barYOffset` in `r5` longer, matching the original's show-block register
assignment over more of the block; the volatile re-read of `field_0x218` is
the 4-byte size anchor (the previous `field_0x3e2` anchor gave 582).

## Measured variants (anchor `volatile int dummy = Globals::layout->field_0x218;`)

All 120 permutations of `T W D H Y` with `F C P y S I L R` fixed:

| order | raw | aligned | entries | size | extab |
|---|---:|---:|---:|---:|---:|
| `H T W D Y` (landed) | **548** | **220** | 77 | 9,332 | 0x26298 |
| `H T W Y D` | 548 | 220 | 77 | 9,332 | 0x26298 |
| `Y D H T W` | 552 | 224 | 77 | 9,332 | 0x26298 |
| `D Y H T W` | 553 | 224 | 77 | 9,332 | 0x26298 |
| `T W H D Y` | 515 | 211 | 78 | 9,332 | 0x2629c |
| `T W H Y D` | 515 | 211 | 78 | 9,332 | 0x2629c |
| `T W D Y H` | 536 | 212 | 78 | 9,332 | 0x2629c |

`T W H D Y` is the best raw (515) but 78 entries (gate-fatal).  `H T W D Y`
is the best 77-entry shape.

Other anchors tried with `T W D Y H`: `field_0x464` 572, `Globals::w` 1790,
`Globals::h` 1242, `dt` 7772 (all 78 entries).

## Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN3Hud4drawExxP9PlayerEgobjj
cmake --build cmake-build-match --target check -j2
```
