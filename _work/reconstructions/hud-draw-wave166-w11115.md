# Hud::draw — wave 166 (w11115): dock/show order sweep, no raw win

Target: `_ZN3Hud4drawExxP9PlayerEgobjj` (`src/game/ui/Hud.cpp`, `Hud::draw`),
original **9,332 B**.  Entry state (wave 164 landed): raw **762**, aligned
**222**, size 9,332 exact, `.rel.plt` byte-identical.

**Result: no source change landed.**  Every tested shape was raw-neutral or
worse; the tree is left at the wave-164 state.

## Per-region raw (same-address) breakdown of the entry state

| region | bytes |
|---|---:|
| prologue `163b90-16412c` | 6 |
| damage `16412c-1643cc` | 25 |
| mission `1643cc-165700` | 0 |
| dock `165700-165b52` | 327 |
| show `165b52-165d4c` | 395 |
| tail `165d4c+` | 9 |

The dock block is **2 bytes short** of the original (ends `165b4e` vs
`165b50`); the show block is 2 bytes long (starts `165b4e`, ends `165d4c`).
The tail is aligned.  The 2-byte dock deficit is the original's extra
`mov r5,fp` (Canvas saved before `fp` is reused for `barH`); our allocator
keeps Canvas in `fp` and puts `barH` in `r8`.

## Dominant residual: the `frameH` / `&Globals::font` stack-slot swap

Original spills `frameH` at `sp+96` and the font value at `sp+92`; ours is
exactly swapped (`frameH` `sp+92`, font `sp+96`).  This is visible in the
dock block, the marker loop (`image_0x380`/`image_0x384`) and the show
block.  Declaration-order experiments (`int textH;` first, `int frameH,
textH;`, `int textH = 0;`) do **not** move the slots — clang 7.0.2 assigns
them from the spill order, not the C++ declaration order.

## Measured variants (all size-exact unless noted)

Dock block (base = wave-164):

| variant | raw | aligned | size |
|---|---:|---:|---:|
| baseline | **762** | **222** | 9,332 |
| `int barW; int barH;` (original call order) | 768 | 230 | 9,332 |
| `unsigned int barH` | 763 | 223 | 9,332 |
| `barW` first + `unsigned int barH` | 769 | 231 | 9,332 |
| `barYOffset` declared before `top` | 762 | 224 | 9,332 |
| `top * 2 + barYOffset` / `barYOffset + (top << 1)` | 762 | 222 | 9,332 |
| `(barW >> 1)` instead of `barW / 2` | 1,563 | 231 | 9,328 |
| `unsigned int barW` | 1,564 | 232 | 9,328 |

Show block (base = wave-164; `BH`=barH, `TXT`=String, `BYO`=barYOffset,
`DUM`=volatile dummy, `HW`=halfW; the original binary order is
`TXT,HW,BH,T464,FADE,TOP,BYO,...`):

| order | raw | aligned | tail |
|---|---:|---:|---|
| baseline `BH,TXT,BYO,DUM,HW` | **762** | **222** | aligned |
| `TXT,BH,BYO,DUM,HW` | 809 | 220 | aligned |
| `TXT,BYO,DUM,HW,BH` | 805 | 209 | aligned |
| `TXT,HW,DUM,BYO,BH` | 809 | 207 | aligned |
| `TXT,BYO,HW,DUM,BH` | 806 | 207 | aligned |
| `TXT,BH,DUM,BYO,HW` | 810 | 218 | aligned |
| `TXT,HW,BH,T464,FADE,TOP,BYO,FC,SET,Y,DRAW` (faithful order, no dummy) | 1,473 | 410 | shifted -6 |
| faithful order + `DUM` after `BYO` | 1,509 | 412 | shifted -6 |
| faithful order + `DUM` between `HW` and `BH` | 1,307 | 403 | aligned, size 9,336 |
| `TXT,DUM,HW,BH,...` | 1,302 | 404 | aligned, size 9,336 |

The faithful show order is structurally closer (show raw drops ~22) but the
block is 6 bytes shorter than the original, so it shifts the tail by -6 and
the same-address raw metric explodes.  Adding the volatile dummy in the
faithful order does not restore the 6 bytes (the compiler drops the
`vmov s4,r0`/`vcvt s4` pair instead).  None of the tail-aligned variants
beats 762 raw.

## Conclusion

The remaining dock/show residual is register allocation (Canvas `fp` vs
`r9`/`r5`, `barH` `r8` vs `fp`, `top`/`barYOffset` `r6`/`r4` vs `r4`/`r6`)
plus the `frameH`/font stack-slot swap — not a source-order difference.
The wave-164 shape is a local optimum for raw bytes.
