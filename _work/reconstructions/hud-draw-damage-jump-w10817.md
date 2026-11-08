# Hud::draw — damage/jump block sweep (wave 162, w10817)

Target: `_ZN3Hud4drawExxP9PlayerEgobjj` (`src/game/ui/Hud.cpp`, `Hud::draw`),
original **9,332 B**, VMA `0x163b90`.

| metric | before (wave 161) | after |
|---|---:|---:|
| raw same-address diff | **1,233** | **1,227** |
| aligned (instruction-index) diff | **256** | **248** |
| size | 9,332 | 9,332 |
| `.text` diff bytes | 85,374 | 85,368 |
| `.ARM.extab` diff | 725 | 725 |
| `.rel.plt` diff | 0 | 0 |
| `.rodata`/`.data`/`.bss`/`.ARM.exidx` | 0 | 0 |

Gate: `check` green — byte_exact 4507/4524, linked_exact 4507/4524,
size-exact 4524/4524, data_exact 843/843, imports 207, lint CLEAN, parity
5370/5370.

## Landed change (one line)

In the docking-transfer bar block (first jump-drive bar), swap the two
declarations so `barH` is declared (and called) before `barW`:

```cpp
-            int barW = Globals::Canvas->GetImage2DWidth(this->image_0x37c);
             int barH = Globals::Canvas->GetImage2DHeight(this->image_0x37c);
+            int barW = Globals::Canvas->GetImage2DWidth(this->image_0x37c);
```

The original calls `GetImage2DWidth` first, so this is *not* the original
call order; nevertheless it re-colours the callee-saved tie in the first
bar block and is a measured raw win (region split: damage 481, mission 8,
jump 732 — was 486/8/738).

## Region measurements (raw diff bytes, same-address)

| region | bytes | baseline | landed |
|---|---:|---:|---:|
| prologue `163b90-16412c` | — | 6 | 6 |
| damage `16412c-1643cc` | 672 | 481 | 481 |
| mission `1643cc-165700` | — | 8 | 8 |
| jump `165700-166000` | — | 738 | 732 |
| tail | — | 0 | 0 |

## Damage block: the 8-byte internal shift (key finding)

The damage block is **size-identical** (672 B) but internally shifted: ours
is 8 B shorter from `164176` to `1642ee` and 8 B longer at the
`field_0x4b8` guard.  The original has **two dead `ldr.w r8,[sp,#108]`
reloads** of `Globals::status` (at `164176` and `164210`, 4 B each) and
**no null test**; ours has no reloads and an 8-byte
`ldr/cmp/beq` `Globals::status &&` test at `1642ee`.  This shift is what
makes the damage block cost 481 raw bytes.

### Measured damage-block variants

| variant | size | raw | damage | mission | jump |
|---|---:|---:|---:|---:|---:|
| baseline (`Globals::status && field_0x4b8 >= 1`) | 9,332 | 1,233 | 481 | 8 | 738 |
| `field_0x4b8 >= 1 && Globals::status` | 9,336 | 6,793 | **214** | 4,436 | 2,131 |
| same + `volatile int dummy = 0;` in jump block | 9,332 | 6,133 | **214** | 4,436 | 1,472 |
| `field_0x4b8 >= 1` (drop the anchor) | 9,268 | 7,457 | 576 | 4,703 | 2,213 |
| `field_0x4ac >= 1 && Globals::status` | 9,340 | 7,221 | 508 | 4,542 | 2,159 |
| hoist `Status *st = Globals::status` + use in panel | 9,292 | 8,783 | — | — | — |
| replace all `Globals::status` with a top-of-function local | 9,100 | 8,064 | — | — | — |

**The `field_0x4b8 >= 1 && Globals::status` operand order reproduces the
original's two dead `ldr.w r8,[sp,#108]` reloads at `164176`/`164210`** and
cuts the damage block from 481 to 214 raw.  It costs exactly one extra
4-byte `cmp/beq` status test at `1642fe`, so the function is 9,336 B and
everything after the damage block shifts by 4 (mission 4,436).  If that
4-byte test can be removed while keeping the two reloads (e.g. a source
shape that lets clang prove `Globals::status` non-null, or a 4-byte
reduction inside the damage block after `1642fe`), the raw should fall to
roughly 214 + 8 + 738 ≈ 960.  This is the main open lead.

## Jump block: real source order vs. the anchor

The original's second (show) bar block order is
`String text` -> `GetImage2DWidth*0.5` (halfW) -> `GetImage2DHeight`
(barH) -> `field_0x464 += dt` -> `fade` -> `barYOffset` -> `top`.
Our source has `barYOffset`/`barH` before `String text` and `halfW` after
it.  Reordering it to the original order gives 9,324 B (8 B short) and
raw 1,965 / aligned 246; with the `volatile` anchor it is 9,332 B but raw
1,331 / aligned 231.  The reorder is structurally closer (aligned 231) but
the raw metric is dominated by the 8-byte shift, so it was not landed.

The original's first bar block allocates `barH` to `fp` and spills the
`Globals::w` address to `sp+96`; ours keeps `Globals::w` in `fp` and
`barH` in `r8`.  The extra `mov r5,fp` and the
`ldr r3,[sp,#96]; ldr r3,[r3]` pair in the original are consequences of
that allocation.  Forcing it from source has not been found.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10817/fdiff.py _ZN3Hud4drawExxP9PlayerEgobjj
python3 /dev/shm/w10817/align_diff.py _ZN3Hud4drawExxP9PlayerEgobjj
cmake --build cmake-build-match --target check -j6
```

Scratch harness in `/dev/shm/w10817`: `fdiff.py`, `align_diff.py`,
`side.py` (aligned side-by-side disassembly diff), `regions.py`
(per-region raw diff), `try.py` (build + measure).
