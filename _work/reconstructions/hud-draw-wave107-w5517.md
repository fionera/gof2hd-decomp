# Hud::draw — docking bar H/W order fix (94.4 -> 94.6 %, 9332 B) — w5517

**Status:** code change + analysis, 2026-11-04 (wave 107). Worktree `wt/w5517`,
branch `wave107-w5517`, base `master f857b5171`. Target
`_ZN3Hud4drawExxP9PlayerEgobjj` (`src/game/ui/Hud.cpp`), original size **9332 B**
(Ghidra `Hud_draw` @ `0x173b90`, file `0x163b90`).

Gate on entry and on exit (full `check`): **byte_exact 4500 / linked_exact 4500**,
size-exact **4524/4524**, `stub_zero_size 0`, `missing 0`, `wrong_type 0`,
`extra 0`, `data_exact 843/843`, imports **207**, `.text 0x1337c8`,
`.rodata 0x41b67`, `.ARM.exidx 0x6f70`, **`.ARM.extab 0x26294`** (4 B short, unchanged),
`.rodata` base **0x1c9e10** (unchanged), lint **CLEAN**, drift **0**, **RATCHET PASS**.

Official per-function score: **94.4 % -> 94.6 %** at the unchanged 9332-byte size.
This note extends `hud-draw-wave106-w5406.md` and the earlier docking-order notes.

---

## 0. TL;DR

The original docking-transfer block calls **`GetImage2DHeight(image_0x37c)` before
`GetImage2DWidth(image_0x37c)`**:

```
0x16593e  blx _ZN11AbyssEngine11PaintCanvas16GetImage2DHeightEj
0x16594a  blx _ZN11AbyssEngine11PaintCanvas15GetImage2DWidthEj
```

The previous wave-106 shape declared `barW` before `barH` (the note's `T P B W H D`),
which scores 94.4 %.  Swapping the two declarations to `T P B H W D` matches the
binary call order and scores **94.6 %** at the same 9332 B.  The change is only:

```diff
             barOffset = (int) ((float) textH + (float) frameH * 2.5f);
-            int barW = Globals::Canvas->GetImage2DWidth(this->image_0x37c);
             int barH = Globals::Canvas->GetImage2DHeight(this->image_0x37c);
+            int barW = Globals::Canvas->GetImage2DWidth(this->image_0x37c);
             this->field_0x468 += dt;
```

The rest of the docking block (`frameH`, `textH`, `transferred`, `total`, `String text`,
`progress`, `barOffset`, `dt/fade`) is unchanged and already in the original order.
The second-bar block remains `halfW` (Width) then `barH` (Height), which also matches
the original (`0x165bd8` Width, `0x165bf8` Height).

---

## 1. Ground truth for the docking block

Original `Hud::draw` @ `0x163b90`, docking block `0x165880..0x1659c0`:

```
0x165888  GetImage2DHeight(image_0x37c)   -> frameH
0x16589c  GetTextHeight(font)             -> textH
0x1658a6  getDockTransferedAmount()       -> transferred
0x1658b0  getDockTotalAmount()            -> total
0x1658b6  getText(3205/3204); String(" ") -> String text
0x165918  progress = transferred / total
0x16592c  field_0x279 ? progress = 1 - progress
0x165938  barOffset = textH + frameH*2.5
0x16593e  GetImage2DHeight(image_0x37c)   -> barH
0x16594a  GetImage2DWidth(image_0x37c)    -> barW
0x165950  field_0x468 += dt; fadeIn; fade
```

So the source order is `T P B H W D` (text, progress, barOffset, barH, barW, dt/fade),
not `T P B W H D`.  The wave-106 note's `W H` label was the old source order, not the
binary order.

---

## 2. Measured variants

All variants are 9332 B unless stated.  `check` was green for the landed variant.

| docking declaration order | score |
|---|---|
| `T P B W H D` (wave-106 landed) | 94.4 |
| **`T P B H W D` (landed)** | **94.6** |
| `T P B H D W` | 94.2 |
| `T P B W D H` | 94.1 |
| `T B P H W D` | 94.3 |
| full 720-permutation sweep of `S P B H W D` | best `S P B H W D` / `S B P H W D` = 94.6, next 94.4 |

Second-bar order sweep (120 permutations of `T W H O B` with the current docking
order): best `T W H O B` and `T W H B O` = 94.6; no improvement over the current
`T W H O B`.

Dead ends (all measured, none beat 94.6):

| variant | score | size |
|---|---|---|
| boost `SetColor` colour in a local `unsigned int` | 93.3 | 9324 |
| boost `SetColor` colour in a local `int` | 93.3 | 9324 |
| boost `float rate = getBoostRate()` local | 93.3 | 9324 |
| boost `if/else` instead of ternary | 93.3 | 9324 |
| boost `bool low = ...` local | 93.3 | 9324 |
| boost `PaintCanvas *canvas = Globals::Canvas` local | 93.5 | 9328 |
| boost inverted ternary (`>=`) | 94.5 | 9332 |
| boost `1.0f > rate` | 94.6 | 9332 |
| gamma `gammaY`/`gammaBarY` declaration swap | 94.6 | 9332 |
| gamma `2*x - y` form | 86.3 | 9348 |
| status guard `!= 0` / local pointer | 94.6 | 9332 |
| status guard `b8 >= 1 && status` | 88.7 | 9336 |
| mission-panel `field_0x1f4`/`field_0x1f8` term swap | 94.6 | 9332 |
| mission-panel local `l1`/`l2` | 92.7 | 9332 |

The boost-block local-colour variants make the `SetColor` scheduling match the
original (`ldr r0,[r9]` after `getBoostRate`) but shrink the function to 9324 B and
score 93.3; the base inline ternary keeps 9332 B and scores 94.6.

---

## 3. Remaining differences (not fixed)

`diff.py` still reports 27 hunks.  The dominant ones are:

1. **Boost button block** (`0x16566c..0x165690`): ours hoists `Globals::Canvas` into
   `r4` before `getBoostRate` (`mov r5,r4; ldr r4,[r9]; ...; mov r0,r4; mov r4,r5`),
   original loads it after the call.  This is a 6-byte scheduling difference.
2. **Downstream shift**: the boost-block size difference shifts the whole docking
   block by 2 bytes (branch targets and literal-pool words differ), producing the
   large `2469..2981` hunk.
3. **Gamma block** (`0x164010..0x16402e`): original keeps `Globals::status` in `r6`;
   ours spills it to `[sp,#108]` and reuses `r4`.
4. **Damage indicators** (`0x164174..0x1644c0`): original loads `r8 = [sp,#108]`
   early; ours loads it later, shifting the block by one instruction.
5. **`ldrd r5,r8` vs `ldrd r8,r5`** in the mission-panel text-Y expression
   (`field_0x1f4` / `field_0x1f8`).

All are register-allocation/scheduling differences; no semantic difference was found.

---

## 4. Extab / section state

Unchanged from wave 106:

```
.ARM.exidx  0x19cc08  size 0x06f70
.ARM.extab  0x1a3b78  size 0x26294  (original 0x26298, 4 B short)
.rodata     0x1c9e10  size 0x41b67
```

Per-function extab deltas (unchanged):

```
LevelScript::process       orig 348  ours 352  +4
Hud::draw                  orig 476  ours 480  +4
Level::createShip          orig 336  ours 328  -8
Generator::createMission   orig 172  ours 168  -4
sum                                            -4
```

The Hud::draw record is still **480 B / 78 rows**; this pass does not change it.

---

## 5. Reproduction

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
cmake --build cmake-build-match --target drift -j6      # 0 drifted fields

python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN3Hud4drawExxP9PlayerEgobjj

python3 _work/reconstructions/exidx_table.py --diff \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so
```

Scratch (dock/second-bar/gamma/guard/boost sweeps) lived in `/dev/shm/w5517`.

---

## 6. Proven vs hypothesis

**Proven:** the original docking block calls `GetImage2DHeight` before
`GetImage2DWidth` (`0x16593e`, `0x16594a`); the source order `T P B H W D` is the
faithful one and scores 94.6 % at 9332 B; the second-bar block is Width then Height
(`0x165bd8`, `0x165bf8`) and the current `T W H O B` order is already best; the
extab record and section sizes are unchanged.

**Hypothesis:** the remaining score loss is entirely register allocation and
instruction scheduling (boost-block `Canvas` load hoist, gamma status spill,
damage-indicator register rotation), not source semantics.  The 94.6 % shape is the
best measured variant; the faithful `T P B W H D` shape scores 94.4 %.
