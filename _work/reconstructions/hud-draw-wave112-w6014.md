# Hud::draw — wave-112: second-bar `fadeColor` branch shape (94.6 -> 94.9 %) — w6014

**Status:** landed source change, 2026-11-05 (wave 112). Worktree `wt/w6014`,
branch `wave112-w6014`, base `master 5d98bd391` (wave-112 harvest).
Target `_ZN3Hud4drawExxP9PlayerEgobjj` (`src/game/ui/Hud.cpp`, `Hud::draw`),
original size **9332 B** (Ghidra `Hud_draw` @ `0x173b90`, file `0x163b90`).

Gate on entry and exit (full `check`): **byte_exact 4500 / linked_exact 4500**,
size-exact **4524/4524**, `stub_zero_size 0`, `missing 0`, `wrong_type 0`,
`extra 0`, `data_exact 843/843`, imports **207**, lint **CLEAN**, drift **0**,
**RATCHET PASS**.  Sections unchanged: `.text 0x1337c8`, `.rodata 0x41c27`
(base `0x1c9e10`), `.ARM.exidx 0x6f70`, **`.ARM.extab 0x26298`** (still exact),
file size **2,187,328 B**, 27 sections.

This note extends `hud-draw-wave111-w5916.md`, `hud-draw-wave109-w5717.md`,
`hud-draw-wave108-w5603.md`, `hud-draw-oracle-dock-w103-wf315.md` and
`hud-draw-dock-order-w4415.md`.

---

## 0. TL;DR

| shape | size | pct | `.ARM.extab` | Hud LSDA |
|---|---:|---:|---:|---:|
| before (`fadeColor` ternary) | 9332 | **94.6** | 0x26298 | 480 |
| **landed (`fadeColor` if/else)** | **9332** | **94.9** | **0x26298** | **480** |

The landed change is one statement in the second (jump-drive / cloak) bar:

```c++
// before
int fadeColor = fade < 1.0f ? (int) (fade * 255.0f) - 256 : 0xffffffff;
// after
int fadeColor = 0xffffffff;
if (fade < 1.0f)
    fadeColor = (int) (fade * 255.0f) - 256;
```

The original emits a **branch** for this select (`vcmpe; vmrs; bpl 165c68;
...; b 165c6c`), not an `ite` pair, and the `if` form reproduces it
instruction-for-instruction (the remaining delta in that block is that clang
computes the value in `r2` and copies to `r1` instead of computing in `r1`;
that is the 2 B that keeps the function size-exact at 9332 B).

Verified against the original at `0x165c22..0x165c74`: the whole
`field_0x464 += dt; fade = .../1000; fadeColor; SetColor` sequence now matches
except for the `r1`/`r2` choice and the resulting `mov r1, r2`.

---

## 1. What was re-measured (all Hud-only, real C++)

Every row was built with
`cmake --build cmake-build-match --target link_so -j6` and scored with
`tools/verify/verify.py` (same normaliser as `report.json`); `extab` is the
`.ARM.extab` section size, `Hud` is the LSDA record length from
`_work/reconstructions/exidx_table.py`.

### 1.1 Dock statement order (720-permutation re-sweep)

The full 6! permutation sweep of the dock block (`String text`, `progress`,
`barOffset`, `barH`, `barW`, `dt/fade`) was re-run on the wave-112 base.  The
local optimum is unchanged:

| dock order | size | pct | Hud LSDA |
|---|---:|---:|---:|
| **`T P B H W D` (landed)** | **9332** | **94.6** | 480 |
| `T B P H W D` | 9332 | 94.6 | 480 |
| `T P B W H D` | 9332 | 94.4 | 480 |
| `P B T W H D` (faithful) | 9324 | 93.5 | 480 |
| `P B T H W D` | 9324 | 93.7 | 480 |

The original's dock block is **`P B T W H D`** (`progress`, `barOffset`,
`String text`, `barW`, `barH`, `dt/fade`): `GetImage2DWidth` is at
`0x16593e` and `GetImage2DHeight` at `0x16594e` (Width first), and the
`operator+` for `String text` is at `0x165926`, i.e. *after* `progress` and
`barOffset`.  The landed `T P B H W D` shape is not the faithful order but it
keeps the 9332 B size and colours the frame like the original, so it scores
higher on the fuzzy metric.  Both numbers are reported here.

### 1.2 `frameH` / `transferred` types (cross product with both orders)

| order | `frameH` | `transferred` | size | pct |
|---|---|---|---:|---:|
| cur | `short` | `short` | 9332 | **94.6** |
| cur | `short` | `int` | 9328 | 94.4 |
| cur | `int` | `short` | 9328 | 93.8 |
| cur | `int` | `int` | 9324 | 93.6 |
| fai | `short` | `short` | 9324 | 93.5 |
| fai | `short` | `int` | 9324 | 93.8 |
| fai | `int` | `short` | 9324 | 93.6 |
| fai | `int` | `int` | 9324 | 93.6 |

`int frameH` + `int transferred` + the faithful order reproduces the original's
`0x165880..0x1658de` dock prologue *exactly* (no `sxth`; `mov r5, fp;
mov fp, r9; mov r9, r0; mov r0, r5; ...`), except that the `frameH` and
`Globals::gameText` spill slots are swapped (`#92`/`#96`).  That swap costs
more than the dock prologue gains, so it was not landed.

### 1.3 `float filled` / LSDA row (re-confirmed)

`exidx_table.py --diff` on the wave-112 base:

```
_ZN3Hud4drawExxP9PlayerEgobjj                     orig 476 ours 480  +4
_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE  orig 172 ours 168  -4
total delta +0 (0x0)
```

(the wave-111 note's `Level::createShip -8` and `LevelScript::process +4` rows
have since been fixed; only Hud +4 / Generator -4 remain, and they still
cancel, so `.ARM.extab` stays exact at `0x26298`.)

The Hud +4 is the second-bar split: ours has two rows
(`SetColor`+`DrawImage2D`, then `DrawRegion2D`+`GetTextWidth`+`DrawString`),
the original has one (`0x165c70 +204`).  Removing the live `float filled`
local merges the rows (Hud 476, section `0x26294`) but re-colours the stack
frame and drops the score:

| shape | size | pct | extab | Hud LSDA |
|---|---:|---:|---:|---:|
| **landed (`float filled` between the calls)** | **9332** | **94.9** | 0x26298 | 480 |
| inline `progress * halfW` (no local) | 9356 | 84.3 | 0x26294 | 476 |
| `float filled;` early, assigned after `DrawImage2D` | 9356 | 84.4 | 0x26294 | 476 |
| `volatile float filled` | 9352 | 93.4 | 0x26298 | 480 |
| `double filled` | 9344 | 93.4 | 0x26298 | 480 |

The recolouring is `ego #116 -> #120`, `&Globals::Canvas #120 -> #116`,
`&Globals::status #108 -> #100`, int params `#64..#76 -> #60..#72`.  No
source-level knob tried here (declaration position, type, `const`/`auto`,
`String text` position, dock order, gamma local shape, or combinations)
recovers the original's slot map together with the merged row.

### 1.4 Other statement-level `? :` -> `if` conversions

The original mixes branch-based and if-converted selects.  Each statement-level
ternary in `Hud::draw` was converted to `if`/`else` and measured on top of the
landed shape:

| ternary | result |
|---|---:|
| **`fadeColor` (landed)** | **94.9** |
| `sideImage` / `frontImage` (separate ifs) | 94.9 |
| `fireImage` | 94.9 |
| `amount` (mission cargo) | 94.9 |
| second-bar `progress` (`field_0x277 ? drive : cloak`) | 94.9 |
| `sideImage` / `frontImage` (single shared if) | 88.1 |
| boost alpha (`boosting() ? 0 : ...`) | 93.1 |
| boost colour (`getBoostRate() < 1.0f ? ...`) | 93.3 |
| armor frame (`getArmorHP() < 1 ? ...`) | 51.2 |

The boost-colour and armor-frame selects are `ite`-converted in the original,
so the ternary form is correct there; the `fadeColor` select is branch-based,
so the `if` form is correct.

### 1.5 Gamma `Status` local (re-confirmed)

`Status *st = Globals::status;` remains load-bearing.  The original keeps
`&Globals::status` in `r6` and reloads `st` for the second call
(`ldr r6,[r0]; ldr r4,[r6]; ...; ldr r0,[r6]; str r6,[sp,#108]`); ours spills
`&Globals::status` immediately and reuses `r4`.  Removing the local or routing
the second call through `Globals::status` moves the spill to `#100` and costs
~20 points (74.8 %).

---

## 2. Reproduction

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

python3 _work/reconstructions/exidx_table.py --diff \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so
```

Scratch sweeps lived in `/dev/shm/w6014` (`sweep2.py`, `sweep3.py`,
`v_fc*.py`, `v_boost*.py`, `v_faithful*.py`, `diffblocks.py`, `score.py`).

---

## 3. Proven vs hypothesis

**Proven:** the second-bar `fadeColor` select is branch-based in the original
and the `if` form reproduces it; this is a real 94.6 -> 94.9 improvement at
unchanged size (9332 B) and unchanged `.ARM.extab` (0x26298, Hud 480).
The faithful dock order (`P B T W H D`) and the merged Hud LSDA row (476) each
cost more than they gain on the fuzzy metric.

**Hypothesis:** the remaining Hud LSDA +4 and the dock-block register
permutation are the same clang-7 `-Oz` stack-slot-colouring tie-break; a source
shape that merges the second-bar row *and* keeps `ego #116` / `&Canvas #120`
would likely score well above 95 %.
