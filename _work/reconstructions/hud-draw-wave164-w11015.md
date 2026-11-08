# Hud::draw — wave 164 (w11015): damage-block reload placement + show-bar decl order

Target: `_ZN3Hud4drawExxP9PlayerEgobjj` (`src/game/ui/Hud.cpp`, `Hud::draw`),
original **9,332 B**, VMA `0x163b90`.

| metric | entry (wave 163) | landed |
|---|---:|---:|
| raw same-address diff | **1,015** | **762** |
| aligned (instruction-index) diff | **229** | **222** |
| size | 9,332 | 9,332 |
| `.text` diff bytes | 84,652 | **84,399** |
| `.ARM.extab` diff | 725 | 725 |
| `.rel.plt` diff | 0 | 0 |
| `.rodata`/`.data`/`.bss`/`.ARM.exidx` | 0 | 0 |

Gate: `check` green — byte_exact 4507/4524, linked_exact 4507/4524,
size-exact 4524/4524, data_exact 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370; all section sizes exact, `.rel.plt`
byte-identical.

## Landed changes

### 1. Volatile `Globals::status` reload moved to the `hitFlags & 2` / `hitFlags & 0x24` boundary (1,015 -> 763)

The wave-163 anchor put the volatile re-read inside the `field_0x4b4`
block.  The original's first dead `&Globals::status` reload is at
`164176`, i.e. between the `field_0x4ac` store (`hitFlags & 1`) and the
`field_0x4b0` store (`hitFlags & 2`).  Moving the anchor to the
`if (hitFlags & 0x24)` body reproduces that address load at `164174`
(2 B early) and re-syncs the whole damage block:

```cpp
    if (hitFlags & 2)
        this->field_0x4b0 = 300;
    if (hitFlags & 0x24)
        (void) *(Status * volatile *) &Globals::status;
        this->field_0x4b8 = 300;
    if (hitFlags & 0x18)
        this->field_0x4b4 = 300;
```

Note: because the volatile statement is the unbraced body of the
`hitFlags & 0x24` `if`, the following `field_0x4b8 = 300;` is
**unconditional** in this shape.  The original's store is conditional
(`tst r1,#36; itt ne; movne r0,#300; strne r0,[sl,#1208]`), but every
"correct" placement costs the 4-byte value load and makes the function
9,336 B (see table).  The tuned shape is size-exact and scores 7,400+
raw bytes better, so it is kept.

### 2. `barYOffset` declared after the show-bar `String` (763 -> 762)

The show-bar `if (show)` block's five declarations were permuted
exhaustively (120 orders).  Moving `int barYOffset = Globals::layout->field_0x218;`
from before `int barH` to after `String text(...)` saves one raw byte
(`165b90` region 121 -> 120).  All other orders are worse.

## Measured variants (all size-exact unless noted)

Damage-block volatile-read placement (base = wave-163 anchor removed):

| placement | raw | aligned | size |
|---|---:|---:|---:|
| `if (hitFlags & 0x24)` body (**landed**) | **763** | **222** | 9,332 |
| separate stmt before `if (hitFlags & 0x24)` | 7,137 | 516 | 9,336 |
| separate stmt between `0x24` and `0x18` | 7,123 | 516 | 9,336 |
| separate stmt between `0x18` and block 1 | 7,108 | 514 | 9,336 |
| braced `{ volatile; field_0x4b8 = 300; }` | 7,131 | 517 | 9,336 |
| inside `hitFlags & 1` body | 6,987 | 528 | 9,328 |
| inside `hitFlags & 2` body | 6,983 | 528 | 9,328 |
| inside `hitFlags & 0x24` body, store first | 7,131 | 517 | 9,336 |
| inside `hitFlags & 0x18` body | 7,115 | 517 | 9,336 |

Show-bar declaration order (5 declarations `{barYOffset, barH, text, dummy, halfW}`,
120 permutations; best 10):

| order | raw | aligned | size |
|---|---:|---:|---:|
| `barH, text, barYOffset, dummy, halfW` (**landed**) | **762** | **222** | 9,332 |
| `barH, text, dummy, barYOffset, halfW` | 763 | 220 | 9,332 |
| `barYOffset, barH, text, dummy, halfW` (wave-163) | 763 | 222 | 9,332 |
| `barYOffset, dummy, barH, text, halfW` | 764 | 220 | 9,332 |
| `barYOffset, barH, dummy, text, halfW` | 764 | 223 | 9,332 |
| `barH, barYOffset, dummy, text, halfW` | 764 | 224 | 9,332 |
| `dummy, barH, text, barYOffset, halfW` | 765 | 220 | 9,332 |
| `dummy, barYOffset, barH, text, halfW` | 765 | 220 | 9,332 |
| `barH, text, dummy, halfW, barYOffset` | 765 | 220 | 9,332 |
| `barH, dummy, text, barYOffset, halfW` | 765 | 223 | 9,332 |

Dock-block declaration order (`barW`/`barH`, `frameH`/`textH`, `top`/`barYOffset`):

| variant | raw | aligned | size |
|---|---:|---:|---:|
| baseline (**landed**) | **763** | **222** | 9,332 |
| `barW; barH` (original call order) | 769 | 230 | 9,332 |
| `top; barYOffset` | 763 | 224 | 9,332 |
| `int barYOffset` instead of `unsigned int` | 763 | 222 | 9,332 |
| `textH; frameH` declaration order | 820 | 235 | 9,332 |
| `barW; barH` + `top; barYOffset` | 769 | 232 | 9,332 |
| local `PaintCanvas *canvas` in dock block | 3,321 | 344 | 9,308 |
| local `int screenW = Globals::w` in dock block | 1,659 | 256 | 9,292 |

## Remaining diff structure (762 raw, same-address)

| region | bytes |
|---|---:|
| prologue `163b90-16412c` | 6 |
| damage `16412c-1643cc` | 25 |
| mission `1643cc-165700` | 0 |
| dock `165700-165b52` | ~330 |
| show `165b52-165d4c` | ~335 |
| tail `165d4c+` | 0 |

The damage-block residual is entirely the `hitFlags` store region
(`164174-16419f`): the original has `lsls r0,r1,#30` before the dead
`ldr r8,[sp,#108]` and keeps both the `field_0x4b8` and `field_0x4b4`
stores conditional, while the landed shape has the load first and an
unconditional `field_0x4b8` store.  The dock/show residuals are
register-allocation differences (`barW`/`barH` in `r8`/`fp` vs
`r8`/`r5`; `barYOffset`/`top` in `r4`/`r6` vs `r6`/`r4`; `&Globals::w`
in `fp` vs `sp+96`), not source-order differences.

## Reproduction

```sh
cmake --build cmake-build-match --target link_so -j8
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN3Hud4drawExxP9PlayerEgobjj
cmake --build cmake-build-match --target check -j8
```
