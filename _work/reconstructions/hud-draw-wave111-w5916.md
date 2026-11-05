# Hud::draw — wave-111 dock/extab sweep (94.6 % holds; no score gain) — w5916

**Status:** analysis-only pass, 2026-11-05 (wave 111). Worktree `wt/w5916`,
branch `wave111-w5916`, base `master 8eea060cc` (wave-110 harvest). Target
`_ZN3Hud4drawExxP9PlayerEgobjj` (`src/game/ui/Hud.cpp`), original size **9332 B**
(Ghidra `Hud_draw` @ `0x173b90`, file `0x163b90`).

Gate on entry and exit (full `check`): **byte_exact 4500 / linked_exact 4500**,
size-exact **4524/4524**, `stub_zero_size 0`, `missing 0`, `wrong_type 0`,
`extra 0`, `data_exact 843/843`, imports **207**, `.text 0x1337c8`,
`.rodata 0x41b67`, `.ARM.exidx 0x6f70`, **`.ARM.extab 0x26294`** (4 B short),
`.rodata` base **0x1c9e10**, lint **CLEAN**, drift **0**, **RATCHET PASS**.
**No source change was landed**: every measured variant scored `<= 94.6 %`, so
this commit is the note only.

This note extends `hud-draw-wave109-w5717.md`, `hud-draw-wave108-w5603.md`,
`hud-draw-wave107-w5517.md` and `hud-draw-wave106-w5406.md`.

---

## 0. TL;DR

The landed `T P B H W D` dock shape (`String text`, `progress`, `barOffset`,
`barH`, `barW`, `dt/fade`) remains the best measured source shape:
**94.6 %, 9332 B, Hud LSDA 480 B (+4), section `.ARM.extab 0x26294`**.

The pass attacked the two known structural deltas:

1. **The Hud LSDA +4 row.**  The second-bar split is a *frontend* split: clang
   emits two `landingpad` blocks (`%2788` for `SetColor`/`DrawImage2D`,
   `%2790` for `DrawRegion2D`/`GetTextWidth`/`DrawString`) because the
   `float filled = ...` declaration between the two calls opens a new cleanup
   scope.  Removing the declaration (inline expression) or hoisting it above
   `SetColor` merges the rows and gives the original's **Hud 476 B / section
   `0x26290`**, and it reproduces the original's second-bar register
   allocation exactly (`mov fp,r9`, `ldr r0,[fp]`, `add.w r2,r9,r5`, ...).
   **But the same change re-colours the whole stack frame**: `ego` moves
   `#116 -> #120`, `&Globals::Canvas` `#120 -> #116`, the four int parameters
   shift `#64..#76 -> #60..#72`, and `&Globals::status` moves `#108 -> #100`.
   That costs far more than the merged row gains: **84.4 %**.
2. **The gamma-block spill.**  Replacing the second `st->` with
   `Globals::status->` reproduces the original's `r6 = &Globals::status;
   ldr r4,[r6]; ...; ldr r0,[r6]; str r6,[sp,#108]` shape, but the spill slot
   moves to `#100` and the score drops to **74.8 %**.

No tested shape produces *both* the faithful second-bar/`filled` code and the
original stack-slot map.  The 94.6 % shape is a local optimum: it trades the
+4 Hud LSDA row for the exact 16-slot stack map.

---

## 1. Ground truth re-confirmed

Original `Hud::draw` @ `0x163b90`, second-bar block (`0x165bcc..0x165d3c`):

```
0x165bcc  GetImage2DWidth(image_0x37c)   -> halfW
0x165bec  GetImage2DHeight(image_0x37c)  -> barH
0x165bfa  field_0x464 += dt; fade = field_0x464 / 1000
0x165c10  top = this->field_0x3e2
0x165c14  barYOffset = Globals::layout->field_0x218
0x165c70  SetColor(fadeColor)
0x165c74  DrawImage2D(image_0x378, w>>1, y, 0x11, 0x14)
0x165c9a  filled = progress * (float) halfW      (one vmul, CSE'd)
0x165cec  DrawRegion2D(...)
0x165d04  GetTextWidth(font, text)
0x165d38  DrawString(...)
```

Original Hud LSDA rows in this region:

```
165bd4 +4   lp=165fc4   GetImage2DWidth
165bf4 +4   lp=165fc2   GetImage2DHeight
165c70 +204 lp=165fd4   SetColor .. DrawString   (single row)
```

Our landed build:

```
165bd8 +4   lp=165fc2   GetImage2DWidth
165bf8 +4   lp=165fc0   GetImage2DHeight
165c74 +40  lp=165fce   SetColor, DrawImage2D
165cbe +126 lp=165fd4   DrawRegion2D, GetTextWidth, DrawString
```

The extra row and the extra `b 165fd4` thunk at `165fce` are the only
structural differences in the second bar; the instruction stream itself is
identical apart from the `fp`/`r5` Canvas choice and one scheduling slot.

`exidx_table.py --diff` on the landed build:

```
_ZN11LevelScript7processEi                          orig 348 ours 352  +4
_ZN3Hud4drawExxP9PlayerEgobjj                       orig 476 ours 480  +4
_ZN5Level10createShipEiiiP8Waypointbb               orig 336 ours 328  -8
_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE  orig 172 ours 168  -4
```

net **-4** => `.ARM.extab 0x26294`.

---

## 2. Measured variants (all Hud-only, real C++)

Built with `cmake --build cmake-build-match --target link_so -j6`, scored with
`tools/verify/verify.py` (same normalizer as `report.json`).  `extab` is the
`.ARM.extab` section size; baseline is `0x26294`.

### 2.1 `float filled` declaration placement (second bar)

The declaration is `float filled = progress * (float) halfW;` between
`DrawImage2D` and `DrawRegion2D` in the landed shape.

| placement of `filled` | size | pct | extab |
|---|---:|---:|---:|
| **after `DrawImage2D` (landed)** | **9332** | **94.6** | **0x26294** |
| inline expression (no variable) | 9356 | 84.4 | 0x26290 |
| declared before `SetColor`, assigned after `DrawImage2D` | 9332 | 84.4 | 0x26290 |
| declared after `progress` / `String text` / `halfW` / `barH` / `top` / `barYOffset` / `dt` / `fade` / `fadeColor` / `y` | 9332 | 83.4-84.4 | 0x26290 |
| declared before `SetColor`, initialized there | 9332 | 83.9 | 0x26290 |
| `float progress = 1.0f, filled = 0.0f;` combined | 9332 | 84.4 | 0x26290 |
| `static float filled` | 9332 | 83.6 | 0x26290 |
| `volatile float filled` | 9332 | 93.4 | 0x26294 |
| `double filled` | 9332 | 93.4 | 0x26294 |
| `filled + 0.0f` | 9332 | 93.6 | 0x26294 |
| `const` / `auto` / parenthesised / cast-order | 9332 | 94.6 | 0x26294 |

Any placement that makes `filled` live across `DrawImage2D` (or removes the
named local) merges the LSDA row (Hud 476) but shifts the stack-slot colouring
and costs ~10 points.  A dummy unused declaration or empty statement between
the calls does **not** change the extab (still 0x26294), so the split is caused
by the live `filled` local, not by "a declaration statement".

### 2.2 `String text` position (second bar)

| placement | size | pct | extab |
|---|---:|---:|---:|
| **before `halfW` (landed)** | **9332** | **94.6** | **0x26294** |
| before `dt` | 9332 | 89.9 | 0x26288 |
| before `fade` | 9332 | 89.5 | 0x26288 |
| before `fadeColor` | 9332 | 72.9 | 0x26288 |
| before `y` / before `SetColor` | 9332 | 89.3 | 0x26288 |
| before `filled` / before `DrawRegion2D` / after `filled` / after `DrawRegion2D` | 9332 | 72.1 | 0x26284 |
| before `DrawString` | 9332 | 70.6 | **0x26298** |

`text` before `DrawString` is the only Hud-only shape found with the exact
section size, but it costs 24 points.

### 2.3 Dock `barH` / `barW` declaration shapes

| shape | size | pct | extab |
|---|---:|---:|---:|
| **`int barH = H; int barW = W;` (landed)** | **9332** | **94.6** | **0x26294** |
| `int barH, barW = W; barH = H;` (declaration split) | 9332 | 93.5 | 0x26290 |
| `int barW, barH = H; barW = W;` | 9332 | 93.7 | 0x26290 |
| `int barW = W; int barH = H;` (faithful call order) | 9332 | 94.4 | 0x26294 |
| `int barH = H, barW = W;` (one declaration) | 9332 | 94.6 | 0x26294 |
| `int barW = W, barH = H;` | 9332 | 94.4 | 0x26294 |
| `int barH; barH = H; int barW = W;` | 9332 | 94.6 | 0x26294 |
| `int barW; barW = W; int barH = H;` | 9332 | 94.4 | 0x26294 |

No improvement; the original call order is Height then Width at `0x16593e` /
`0x16594e` (the wave-107/109 notes' `0x16594a` label was one instruction off).

### 2.4 Gamma `Status` spill

| shape | size | pct | extab |
|---|---:|---:|---:|
| **`Status *st = Globals::status;` (landed)** | **9332** | **94.6** | **0x26294** |
| first call `Globals::status->`, second `st->` | 9332 | 94.6 | 0x26294 |
| second call `Globals::status->`, first `st->` | 9332 | 74.8 | 0x26294 |
| no local, all three `Globals::status->` | 9332 | 74.8 | 0x26294 |

The `st_second_global` shape reproduces the original's
`r6 = &Globals::status; ldr r4,[r6]; ...; ldr r0,[r6]; str r6,[sp,#108]`
instruction sequence, but the spill slot lands at `#100` instead of `#108`
and the rest of the function shifts.

### 2.5 Combinations

| shape | size | pct | extab |
|---|---:|---:|---:|
| `st_second_global` + inline `filled` | 9332 | 77.0 | 0x26290 |
| `st_second_global` + faithful `W/H` dock | 9332 | 74.6 | 0x26294 |
| `st_second_global` + inline `filled` + faithful `W/H` | 9332 | 76.8 | 0x26290 |
| inline `filled` + `int/int` dock types | 9332 | 84.4 | 0x26290 |
| inline `filled` + faithful `W/H` dock | 9332 | 84.3 | 0x26290 |
| inline `filled` + `int frameH` | 9332 | 84.4 | 0x26290 |
| inline `filled` + `int transferred` | 9332 | 84.6 | 0x26290 |

---

## 3. Why the merged row and the exact stack map are mutually exclusive

The IR difference between the landed shape and the inline shape is exactly one
`unwind label` operand on the `DrawRegion2D`/`GetTextWidth`/`DrawString`
invokes (`%2790` vs `%2788`); the arithmetic, the `String` lifetime and the
call sequence are identical.  Yet the backend's `StackSlotColoring` re-colours
the frame:

```
landed   #116 ego        #120 &Globals::Canvas   #108 &Globals::status
inline   #120 ego        #116 &Globals::Canvas   #100 &Globals::status
```

and the four int parameters move `#64/#68/#72/#76 -> #60/#64/#68/#72`.
The frame stays 224 B; only the colouring changes.  The original has the
merged row *and* the landed colouring, so a further source difference is
missing, but no source-level knob tried here (declaration position, type,
const/volatile/static, `String text` position, dock order, gamma local shape,
or their combinations) recovers it.

---

## 4. Reproduction

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS

python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN3Hud4drawExxP9PlayerEgobjj

python3 _work/reconstructions/gameresource_gen/lsda.py \
  cmake-build-match/libgof2hdaa.so _ZN3Hud4drawExxP9PlayerEgobjj
python3 _work/reconstructions/exidx_table.py --diff \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so
```

Scratch sweeps lived in `/dev/shm/w5916` (`sweep_filled.py`,
`sweep_dockdecl.py`, `vrun.py`, `clusters5.py`, `emitll.sh`).

---

## 5. Proven vs hypothesis

**Proven:** the landed `T P B H W D` shape is the best of the wave-111 sweep
(`filled` placement x 16, dock declaration x 8, `String text` position x 11,
gamma local x 4, plus 7 combinations); the second-bar LSDA split is a clang
frontend cleanup-scope split caused by the live `filled` local; merging it
always re-colours the stack frame; the original's second-bar instruction
stream is reproduced exactly by the inline/early-`filled` shape.

**Hypothesis:** the remaining delta is still clang-7 `-Oz` register/slot
allocation.  The next lever is the stack-slot colouring tie-break that puts
`ego` at `#116` and `&Globals::Canvas` at `#120`; a source shape that merges
the Hud LSDA row *without* changing that colouring would likely score well
above 94.6 %.
