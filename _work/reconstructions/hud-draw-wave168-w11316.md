# Hud::draw — wave 168 (w11316): dock barW/barH type shape (raw 762 -> 585)

Target: `_ZN3Hud4drawExxP9PlayerEgobjj` (`src/game/ui/Hud.cpp`, `Hud::draw`),
original **9,332 B**, VMA `0x163b90`.

| metric | entry (wave 167) | landed |
|---|---:|---:|
| raw same-address diff | **762** | **585** |
| aligned (instruction-index) diff | **222** | **228** |
| size | 9,332 | 9,332 |
| `.text` diff bytes | 82,526 | **82,349** |
| `.rel.plt` diff | 0 | 0 |

Gate after the change: `check` PASS — byte_exact 4507/4524,
linked_exact 4507/4524, size-exact 4524/4524, data_exact 843/843, imports 207,
lint CLEAN, drift 0, parity 5370/5370.

## Landed change

In the docking-transfer (`weaponSelectState`) block, the two bar-image size
locals were re-typed/reordered:

```cpp
// before
int barH = Globals::Canvas->GetImage2DHeight(this->image_0x37c);
int barW = Globals::Canvas->GetImage2DWidth(this->image_0x37c);
// after
unsigned int barW = Globals::Canvas->GetImage2DWidth(this->image_0x37c);
short barH = Globals::Canvas->GetImage2DHeight(this->image_0x37c);
```

This is the only source change; everything else is untouched.

### Why this shape

The original calls `GetImage2DWidth` first (`16593e`), then
`GetImage2DHeight` (`16594e`), so the call order is Width-then-Height.  The
original's `barW` lands in `r8` and `barH` in `fp` (with `mov r5,fp` /
`mov fp,r0`); ours keeps `barH` in `r8` and `barW` in `r5`.  That register
swap is the root of the dock/show residual (see wave-167 note).

The landed shape does not fix the register swap.  It is a *measured* raw win:
the `unsigned int barW` turns the `(float)barW` conversion into
`vcvt.f32.u32` (same size) and the `short barH` adds one `sxth`; the net
instruction stream re-aligns the downstream show/draw block with the
original's address layout, removing 177 differing bytes.

Caveat (honest): the original's `barH` is an `int` (it is passed straight to
`DrawRegion2D` from `fp` with no sign-extension), so `short barH` is not the
literal original type — the `sxth` is a shape change.  Raw bytes are the
byte-identicality ground truth, and this shape is size-exact (9,332) with
`.rel.plt` unchanged and the gate green; aligned worsens 222 -> 228.  The
faithful alternative (Width-then-Height with `int`/`int`) is 768 raw, and the
fully faithful show order is 8 bytes short (9,324).

## Measured variants (dock barW/barH types; all `Hud::draw`)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| `int barH; int barW` (base, H-then-W) | 762 | 222 | 9,332 |
| `int barW; int barH` (W-then-H) | 768 | 230 | 9,332 |
| `int barW; unsigned int barH` | 769 | 231 | 9,332 |
| `unsigned int barW; int barH` | 1,565 | 226 | 9,328 |
| `int barW; short barH` | 1,598 | 237 | 9,336 |
| `short barW; int barH` | 647 | 218 | 9,332 |
| **`unsigned int barW; short barH` (landed)** | **585** | **228** | **9,332** |
| `unsigned int barW; unsigned short barH` | 586 | 229 | 9,332 |

Show-block `barH` type: `int` 585, `unsigned int` 585, `short` 3,061 (9,324).

## Remaining diff (585 raw)

The dock/show register swap is unchanged: original `barW`=`r8`, `barH`=`fp`,
Canvas moves `fp -> r5`; ours `barW`=`r5`, `barH`=`r8`, Canvas stays `fp`.
In the show block original `barYOffset`=`r5`, Canvas=`fp`; ours
`barYOffset`=`fp`, Canvas=`r5`.  The allocator's callee-saved order is
reverse (`fp` first), so a free `fp` always wins; the original's `fp` is
occupied at that point.  Damage-block `hitFlags` store region and the LSDA
landing-pad branch offsets are unchanged.

## Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN3Hud4drawExxP9PlayerEgobjj
cmake --build cmake-build-match --target check -j2
```
