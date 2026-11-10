# Hud::draw — wave 167 (w11215): dock/show/damage sweep, no raw win

Target: `_ZN3Hud4drawExxP9PlayerEgobjj` (`src/game/ui/Hud.cpp`, `Hud::draw`),
original **9,332 B**, VMA `0x163b90`.

Entry state = wave-164 landed shape: raw **762**, aligned **222**, size
9,332 exact, `.rel.plt` byte-identical. **No source change landed**; the tree
is left at the wave-164/166 state.

Gate after the sweep (unchanged tree): `check` PASS — byte_exact 4507/4524,
linked_exact 4507/4524, size-exact 4524/4524, data_exact 843/843, imports 207,
lint CLEAN, drift 0, parity 5370/5370.

## Per-region raw (same-address) breakdown of the entry state

| region | bytes |
|---|---:|
| prologue `163b90-16412c` | 6 |
| damage `16412c-1643cc` | 25 |
| mission `1643cc-165700` | 0 |
| dock `165700-165b52` | 327 |
| show `165b52-165d4c` | 395 |
| tail `165d4c+` | 9 |

## Root cause of the remaining residual: a consistent register swap

The original's dock/show blocks keep **Canvas in `fp`** and the short-lived
`barYOffset` in **`r5`**; ours keeps **Canvas in `r5`** and **`barYOffset` in
`fp`**. Because `fp` (r11) cannot be encoded in the 16-bit `ldr` forms, the
original's `ldr.w r0,[fp]` / `ldr.w r6,[fp]` are 4 B where ours are 2 B. This
is exactly the 6-byte size deficit of the structurally faithful show order
(plus a 2-byte `nop` alignment pad). It is also why the dock is 2 B short:
the original has `mov r5,fp` (save Canvas before `fp` is reused for `barH`)
and `mov fp,r0` (barH), where ours has `mov r8,r0` (barH) and keeps Canvas in
`fp`.

So the dock/show diff is **not** a source-order difference; it is the
allocator's choice of which callee-saved register holds Canvas vs the
long-lived bar values. Every source-order permutation tested leaves the same
swap.

## Measured variants (all size-exact unless noted)

Dock block:

| variant | raw | aligned | size | dock |
|---|---:|---:|---:|---:|
| baseline (wave-164) | **762** | **222** | 9,332 | 327 |
| `barW = Width; barH = Height;` (original call order) | 768 | 230 | 9,332 | 333 |
| separate decl/assign `int barH; int barW = Width; barH = Height;` | 1,019 | 233 | 9,328 | 333 |
| `int barW, barH;` then assign Width, Height | 1,019 | 233 | 9,328 | 333 |
| `unsigned int font` local before Height | 839 | 235 | 9,332 | 447 (show 352) |
| `unsigned int font` local between Height and TextHeight | 770 | 227 | 9,332 | 335 |
| `int textH;` declared before `frameH` | 762 | 222 | 9,332 | 327 |
| `field_0x468 += dt` between the two image calls | 1,659-1,672 | 240-254 | 9,336 | 510-523 |

Show block:

| variant | raw | aligned | size | dock | show |
|---|---:|---:|---:|---:|---:|
| baseline | **762** | **222** | 9,332 | 327 | 395 |
| faithful order `text, halfW, barH, T464, fade, top, barYOffset, fadeColor, y` | 1,503 | 212 | 9,324 | 335 | 488 |
| `barH` after `text` (rest base) | 809 | 220 | 9,332 | 329 | 395 |
| `barH` after `halfW` (rest base) | 805 | 209 | 9,332 | 329 | 391 |
| `barH` after `dummy` (rest base) | 809 | 229 | 9,332 | 329 | 395 |
| `int y = ...` moved after `SetColor` | 1,257 | 437 | 9,332 | 465 | 355 |
| `float rate = 0.0f;` | 762 | 222 | 9,332 | 327 | 395 |
| `barYOffset` `int` / `unsigned short`, `top` `unsigned short` | 762 | 222-224 | 9,332 | 327 | 395 |

Damage block (`hitFlags` store region, `164174-16419f`, 25 raw):

The original is one unconditional `ldr r8,[sp,#108]` (loads `&Globals::status`,
dead) plus **four conditional** `itt ne / movne / strne` stores. Ours is the
same byte count but `field_0x4b8` is stored unconditionally and the volatile
read is `ldr r8,[sp,#108]` + a conditional `ldrne r0,[r8]`.

| variant | raw | aligned | size |
|---|---:|---:|---:|
| baseline (unbraced if, volatile deref, store uncond.) | **762** | **222** | 9,332 |
| `*(volatile int *) &Globals::status` unbraced (same codegen) | 762 | 222 | 9,332 |
| `*(Status ** volatile *) &Globals::status` unbraced (same) | 762 | 222 | 9,332 |
| braced if, volatile deref + store | 7,131 | 517 | 9,336 |
| braced if, `Globals::status` + store | 7,522 | 635 | 9,268 |
| unconditional volatile deref + conditional store | 7,123 | 516 | 9,336 |
| `(void) Globals::status;` unconditional + conditional store | 7,522 | 635 | 9,268 |
| `(void) &Globals::status;` + conditional store | 7,522 | 635 | 9,268 |

The unbraced-if shape is the only 9,332-byte form found: it keeps the
conditional volatile load that anchors 64 bytes of allocation elsewhere.
Making `field_0x4b8` conditional costs 4 B (9,336). The original's
single-load dead read (`ldr r8,[sp,#108]`) cannot be produced by any
volatile expression tried — every `*(volatile ...) &Globals::status` form
emits the address load *and* a value load.

## Conclusion

The wave-164 shape remains a local optimum for raw bytes. The residual is
purely register allocation (Canvas `fp` vs `r5`, `barH` `r8` vs `fp`,
`top`/`barYOffset` `r4`/`r6` vs `r6`/`r4`) plus the `frameH`/font spill-slot
swap, not a source-order difference. Fixing the show block's Canvas-in-`fp`
would recover the 6-byte encoding difference and align the tail, but no
source shape tried forces the allocator to pick `fp` for Canvas while keeping
`barYOffset` in `r5`.

## Reproduction

```sh
ulimit -v 12000000
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN3Hud4drawExxP9PlayerEgobjj
cmake --build cmake-build-match --target check -j2
```
