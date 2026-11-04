# Hud::draw — wave-107 dock order recovered at 94.6 % with exact `.ARM.extab` (w5603)

**Status:** code change + analysis, 2026-11-04 (wave 108). Worktree `wt/w5603`,
branch `wave108-w5603`, base `master 8f98ec4d6`. Target
`_ZN3Hud4drawExxP9PlayerEgobjj` (`src/game/ui/Hud.cpp`), original size **9332 B**
(Ghidra `Hud_draw` @ `0x173b90`, file `0x163b90`).

Gate on entry and on exit (full `check`): **byte_exact 4500 / linked_exact 4500**,
size-exact **4524/4524**, `stub_zero_size 0`, `missing 0`, `wrong_type 0`,
`extra 0`, `data_exact 843/843`, imports **207**, `.text 0x1337c8`,
`.rodata 0x41b67`, `.ARM.exidx 0x6f70`, **`.ARM.extab 0x26298` (exact)**,
`.rodata` base **0x1c9e10** (exact), lint **CLEAN**, drift **0**, **RATCHET PASS**.

Official per-function score: **93.3 % -> 94.6 %** at the unchanged 9332-byte size.
The `.ARM.extab` section and `.rodata` base are unchanged (`0x26298`, `0x1c9e10`).
This note extends `hud-draw-wave106-w5406.md` and `hud-draw-wave107-w5517.md`.

---

## 0. TL;DR

The wave-107 docking declaration order `T P B H W D` (String text, progress,
barOffset, barH, barW, dt/fade) scores **94.6 %** but its Hud LSDA is
**480 B / 78 rows** (+4 vs the original 476 B / 77 rows), which grows
`.ARM.extab` to `0x2629c` and shifts `.rodata` to `0x1c9e20`.

A 120-permutation sweep of the second-bar declaration order and the wave-107
720-permutation docking sweep both show that **no Hud-only source shape scores
94.6 % with 476 B**: the 94.6 % shapes are exactly `T P B H W D` and
`T B P H W D` (both 480 B); the best 476-B shapes are 93.2 % and 9328 B
(not size-exact).

The exact-extab 94.6 % is recovered by the wave-106 Generator shave:
move `AbyssEngine::String agentName = agent->getName();` below the difficulty
`if`/`else` in `Generator::createMission`.  That keeps the function at
**1996 B** and shrinks its LSDA from **168 B to 164 B** (-8 vs the original
172 B), paying for Hud's +4 exactly.

Landed result: **Hud::draw 93.3 -> 94.6 %**, 9332 B, `.ARM.extab 0x26298`.
Collateral: **Generator::createMission 82.1 -> 67.9 %** (same 1996 B); it is
not byte-exact, so the gate counts are unchanged.

---

## 1. Hud docking order

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

The source order is `T P B H W D`.  The landed Hud diff is only the
declaration reorder in `src/game/ui/Hud.cpp`:

```diff
             short transferred = ego->getDockTransferedAmount();
             int total = ego->getDockTotalAmount();
+            String text = *Globals::gameText->getText(this->field_0x279 ? 3205 : 3204) + String(" ");
             float progress = (float) transferred / (float) total;
             if (this->field_0x279)
                 progress = 1.0f - progress;
-            int barW = Globals::Canvas->GetImage2DWidth(this->image_0x37c);
-            String text = *Globals::gameText->getText(this->field_0x279 ? 3205 : 3204) + String(" ");
-            int barH = Globals::Canvas->GetImage2DHeight(this->image_0x37c);
             barOffset = (int) ((float) textH + (float) frameH * 2.5f);
+            int barH = Globals::Canvas->GetImage2DHeight(this->image_0x37c);
+            int barW = Globals::Canvas->GetImage2DWidth(this->image_0x37c);
             this->field_0x468 += dt;
```

`short frameH` / `short transferred` are unchanged from w5318.

## 2. Why the Hud LSDA is 480 B

The extra row is the second-bar landing-pad split between `DrawImage2D` and
`DrawRegion2D` (the `float filled` lifetime start):

```
original : 0x20e0 +0xcc lp=0x2444      (SetColor, DrawImage2D,
                                        DrawRegion2D, DrawString)
wave-107 : 0x20e4 +0x28 lp=0x243e      (SetColor, DrawImage2D)
           0x212e +0x7e lp=0x2444      (DrawRegion2D, DrawString)
```

The docking-block rows also shift (`0x1d96/0x1dae/0x1dbe` -> `0x1d54/0x1db6/
0x1dc2`) but keep the same count; they are not the extra row.

Declaring `float filled` earlier (any position from the docking block through
`int y`) merges the second-bar row but changes codegen: all tested variants are
**9356 B / 84.4 %** (one is 9356 B / 83.9 %), never 9332 B.  The 9332-B
94.6 % shape therefore inherently carries the +4 record.

## 3. Measured variants

Docking declaration order (second-bar fixed at `T W H O B D Y`):

| docking order | score | size | Hud LSDA |
|---|---:|---:|---:|
| `P B W T H D` (HEAD, extab-neutral) | 93.3 | 9332 | 468 (76) |
| **`T P B H W D` (landed)** | **94.6** | **9332** | **480 (78)** |
| `T B P H W D` | 94.6 | 9332 | 480 (78) |
| `T P B W H D` (wave-106) | 94.4 | 9332 | 480 (78) |
| `T P B H D W` | 94.2 | 9332 | 480 (78) |
| `T P B W D H` | 94.1 | 9332 | 480 (78) |

Second-bar sweep (120 permutations of `T W H O B`, wave-107 docking order):
best `T W H O B` and `T W H B O` = 94.6 / 480; next 94.5 (`T W O H B`,
`T O W H B`) / 480.  No permutation gives 94.6 with 476.  The best 476-B
variants are 93.2 % and 9328 B (`W T O H B`, `W T H O B`, `W T H B O`,
`W O T H B`, `O W T H B`).

`float filled` declaration placements (all merge the row to 476, all grow):

| placement | score | size |
|---|---:|---:|
| uninitialized before `SetColor`, assign after `DrawImage2D` | 84.4 | 9356 |
| `= 0.0f` before `SetColor`, assign after | 84.4 | 9356 |
| before `SetColor` (initialized) | 83.9 | 9356 |
| after `String text` / `halfW` / `top` / `dt` / `fade` / `fadeColor` | 84.4 | 9356 |
| inline expression (no local) | 84.4 | 9356 |

## 4. Extab budget and the Generator shave

```
.ARM.exidx  0x19cc08  size 0x06f70
.ARM.extab  0x1a3b78  size 0x26298 (original)   ends 0x1c9e10
.rodata     0x1c9e10  size 0x41b67  (align 16)
```

Per-function deltas with the landed shape:

```
LevelScript::process       orig 348  ours 352  +4
Hud::draw                  orig 476  ours 480  +4
Generator::createMission   orig 172  ours 164  -8
sum                                            +0
```

Generator `createMission` measured variants (all 1996 B unless stated):

| shape | score | extab | total |
|---|---:|---:|---:|
| HEAD: `agentName` before difficulty | 82.1 | 168 | +4 |
| **`agentName` after difficulty (landed)** | **67.9** | **164** | **0** |
| `agentName` after `int itemId = 0;` | 67.9 | 164 | 0 |
| after `if (difficulty >= 10)` clamp | 52.6 | 128 | -36 (1948 B) |
| after `Galaxy *galaxy = ...` | 52.8 | 128 | -36 (1948 B) |
| after `int fromSystem = ...` | 52.1 | 120 | -44 (1960 B) |
| after `SolarSystem *fromSystemPtr = ...` | 52.0 | 120 | -44 (1964 B) |

The landed move is semantically neutral: `Agent::getName()` is a getter
returning a `String`, and `agentName` is first used at the later
`new Mission(...)` call.  The 82.1 -> 67.9 % change is a register-allocation
consequence of moving the String construction past the two `rnd->nextInt`
calls; the function remains 1996 B and is not byte-exact, so the gate counts
are unchanged.  This is the same collateral documented in
`hud-draw-wave106-w5406.md` (there measured 81.5 -> 67.9 %).

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

Scratch (dock/second-bar sweeps, filled-placement and Generator variants)
lived in `/dev/shm/w5603`.

## 6. Proven vs hypothesis

**Proven:** the original docking order is `T P B H W D`; it scores 94.6 % at
9332 B with a 480-B Hud LSDA; no Hud-only docking or second-bar order scores
94.6 % with 476 B (720 + 120 measured permutations); moving `String agentName`
below the difficulty block in `Generator::createMission` keeps 1996 B and
shrinks its LSDA to 164 B; the final `.ARM.extab` is exactly `0x26298` and
`.rodata` stays at `0x1c9e10`.

**Hypothesis:** `Agent::getName()` has no side effects, so reordering its call
past `getCurrentCampaignMission()` and `rnd->nextInt()` is semantically
neutral.  The remaining Hud score loss is register allocation / scheduling
(boost-block `Canvas` hoist, gamma status spill, damage-indicator rotation),
not source semantics.
