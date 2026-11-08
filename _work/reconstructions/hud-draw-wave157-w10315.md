# Hud::draw — wave 157 (w10315): every non-pivotY liveness anchor still costs the 224-byte frame

**Status:** analysis-only, 2026-11-08 (wave 157). Worktree `wt/w10315`,
base `master 08575efff` (wave 156). Target `_ZN3Hud4drawExxP9PlayerEgobjj`
(`src/game/ui/Hud.cpp`, `Hud::draw`), original **9332 B** (VMA `0x163b90`).

| metric | before | after |
|---|---:|---:|
| raw same-address diff | **1325** | **1325** (no source landed) |
| aligned (instruction-index) diff | **249** | **249** |
| size | 9332 | 9332 |
| frame | 224 | 224 |

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size-exact 4524/4524, `data_exact 843/843`, imports
**207**, lint **CLEAN**, drift **0**, parity 5370/5370.

---

## 0. TL;DR

1. The wave-156 mission's four suggestions were re-measured on the current
   (wave-156) source: a `volatile` read, a function-pointer call, a
   `dividerOffset` local used in the gamma block, and a `do/while` around the
   regen block. **None keeps 9332 B / 224 frame.**
2. New hypotheses swept this wave: every other `DrawRegion2D` argument slot
   (angle bitcast, srcX/srcY, pivotX, width, height, posX, posY), every other
   Hud field as `pivotY`, a pointer-to-member-function call, a local
   `PaintCanvas *` receiver, `volatile`/`const`/`long`/`unsigned`/`short`
   type variations of `dividerOffset`, always-true guards
   (`& 1`, `== 0 || == 1`, `<= 1`, `< 2`), `do/while` with seven conditions,
   gamma-block locals, gamma expression reassociation, and extending the
   live range into the rocket/damage blocks.
3. **The `pivotY = dividerOffset` anchor is still the unique 9332 B /
   224-frame shape.** The only metrics-identical alternative is the *angle*
   bitcast `*(float *)&dividerOffset`, which merely moves the single
   differing byte from `0x163ff7` (`strd r4,sl,[sp,#12]`) to
   `0x163ff3` (`strd r2,sl,[sp,#4]`). It is semantically worse (a nonzero
   angle), so the pivotY form is retained.
4. The gamma-block `fp`/`r9` swap is **not** caused by the anchor alone: it
   survives the angle anchor and every gamma-block reordering. It is a pure
   RA tie-break between the two `ldrh.w` loads of `field_0x44a`/`field_0x448`.

---

## 1. Register map at the armour-regen `DrawRegion2D` call (base)

| location | original | ours (anchor) |
|---|---|---|
| `r0` | `Globals::Canvas` | same |
| `r1` | `armorRegenFillImage` (`0x2c0`) | same |
| `r2` | `0` (srcX) | same |
| `r3` | `0` (srcY) | same |
| `r4` | `0` (angle) | same |
| `r5` | `this` | same |
| `r6` | `armorBarY` | same |
| `r8` | `DrawRegion2D` fn ptr | same |
| `r9` | `&Globals::Canvas` | same |
| `sl` (r10) | `dividerOffset` | `dividerOffset` |
| `fp` (r11) | `ego->player` | same |
| `sp+0` | `regenW` (width) | same |
| `sp+4` | `field_0x44c` (height) | same |
| `sp+8` | `0` (angle) | same |
| `sp+12` | `0` (pivotX) | same |
| `sp+16` | **`0` (pivotY)** | **`dividerOffset` (pivotY)** |
| `sp+20` | `field_0x440` (posX) | same |
| `sp+24` | `armorBarY` (posY) | same |

Frame `0xe0` = **224 B** in both. The only differing byte in this region is
the register field of `strd r4,sl,[sp,#12]` vs `strd r4,r4,[sp,#12]` at
`0x163ff4`.

---

## 2. Measured variants (full build; base = 1325 / 249 / 9332 / 224)

All replace the anchor line with `pivotY = 0` plus the listed change unless
noted.

### 2.1 Mission suggestions

| variant | size | raw | al | frame |
|---|---:|---:|---:|---:|
| **base (anchor `pivotY=dividerOffset`)** | **9332** | **1325** | **249** | **224** |
| `volatile int dv = dividerOffset;` before regen, `dv` in gamma | 9284 | 8857 | 1769 | 216 |
| `int d = dividerOffset;` before regen, `d` in gamma | 9312 | 8261 | 1009 | 216 |
| `const int &d = dividerOffset;`, `d` in gamma | 9312 | 8261 | 1009 | 216 |
| `const int *dp = &dividerOffset;`, `*dp` in gamma | 9312 | 8261 | 1009 | 216 |
| `long d = dividerOffset;`, `d` in gamma | 9312 | 8261 | 1009 | 216 |
| `do { regen } while (0)` | 9312 | 8261 | 1009 | 216 |
| `do { regen } while (dividerOffset < 0)` | 9368 | 8659 | 1498 | 216 |
| `do { regen } while (dividerOffset > 1)` | 9368 | 8660 | 1498 | 216 |
| `do { regen } while (dividerOffset == -1)` | 9368 | 8660 | 1498 | 216 |
| pointer-to-member `DrawRegion2D` call | 9312 | 8261 | 1009 | 216 |
| local `PaintCanvas *regenCanvas` receiver | 9328 | 7459 | 1044 | 216 |

### 2.2 Other argument slots

| slot | size | raw | al | frame |
|---|---:|---:|---:|---:|
| `pivotY` (base) | 9332 | **1325** | **249** | 224 |
| `angle = *(float *)&dividerOffset` | 9332 | **1325** | **249** | 224 |
| `pivotX = dividerOffset` | 9336 | 7312 | 454 | 224 |
| both pivots | 9352 | 7927 | 589 | 224 |
| `height = dividerOffset` | 9328 | 7173 | 550 | 224 |
| `posX = dividerOffset` | 9328 | 7169 | 549 | 224 |
| `posY = dividerOffset` | 9320 | 8114 | 1051 | 216 |
| shield-fill `pivotY = dividerOffset` | 9324 | 8134 | 820 | 216 |
| armour-fill `pivotY = dividerOffset` | 9328 | 7456 | 1028 | 216 |
| gamma-region `pivotY = dividerOffset` | 9328 | 8091 | 1018 | 216 |

### 2.3 Other fields as `pivotY`

| field | size | raw | al | frame |
|---|---:|---:|---:|---:|
| `field_0x43e` | 9332 | 2290 | 973 | 216 |
| `field_0x442` | 9332 | 2290 | 973 | 216 |
| `field_0x444` | 9332 | 2290 | 973 | 216 |
| `field_0x448` | 9332 | 2290 | 973 | 216 |
| `field_0x44a` | 9332 | 2290 | 973 | 216 |
| `(int)field_0x43e` | 9332 | 2290 | 973 | 216 |
| `armorY` | 9332 | 3766 | 1120 | 216 |
| `Globals::layout->field_0x1e8` | 9332 | 6785 | 857 | 216 |

### 2.4 Type / storage variations of `dividerOffset`

| type | size | raw | al | frame |
|---|---:|---:|---:|---:|
| `int` (base) | 9332 | **1325** | **249** | **224** |
| `long` | 9332 | **1325** | **249** | 224 |
| `long long` | 9332 | **1325** | **249** | 224 |
| `unsigned` | 9332 | **1325** | **249** | 224 |
| `const int` | 9332 | **1325** | **249** | 224 |
| `register int` | 9332 | **1325** | **249** | 224 |
| `unsigned short` | 9332 | 1326 | 250 | 224 |
| `short` | 9332 | 6761 | 577 | 224 |
| `unsigned char` | 9332 | 1326 | 250 | 224 |
| `volatile int` | 9292 | 8791 | 1955 | 216 |
| volatile initial load | 9332 | 1341 | 255 | 224 |

### 2.5 Always-true guards / folded uses

`dividerOffset + 0`, `-(-dividerOffset)`, `dividerOffset ^ 0`,
`dividerOffset + armorBarY - armorBarY`, `dividerOffset + armorY - armorY`,
`+ (dividerOffset & 0)` in `hullW`/`regenW`/`shieldW`/`gammaW`, and an empty
`(void)(dividerOffset & 0)` in the damage block all fold to the base:
**1325 / 249 / 9332 / 224**.

Guards `dividerOffset & 1` (9328 / 8020 / 1079 / 216),
`dividerOffset == 0 || dividerOffset == 1` (9324 / 8469 / 956 / 216),
`dividerOffset <= 1` (9324 / 8469 / 956 / 216),
`(unsigned)dividerOffset < 2` (9324 / 8469 / 956 / 216), and
`dividerOffset >= 0 && dividerOffset <= 1` (9324 / 8469 / 956 / 216) all
lose the frame.

### 2.6 Gamma-block rewrites

`gammaBarY` declared before `gammaY` (1325 / 249 / 9332 / 224);
`gammaBarY = 0x448 - (0x44a - 0x448)` (1325 / 249 / 9332 / 224);
`gammaY = 0x444 - (0x442 - 0x444)` (1325 / 249 / 9332 / 224);
`unsigned short`/`int` locals for the four bar fields
(1325 / 249 / 9332 / 224); `gammaY = 0x444 + 0x444 - 0x442`
(1340 / 482 / 9332 / 224); `gammaBarY = 0x448 + 0x448 - 0x44a`
(7021 / 562 / 9328 / 224); `gammaBarY = 0x448 * 2 - 0x44a`
(7021 / 562 / 9328 / 224).

---

## 3. Why no alternative keeps 9332 / 224

The original reloads `dividerOffset` into `sl` at `0x163fb2`, *before* the
`hasArmorRegen` branch, and keeps it live across the regen call to the gamma
`add.w r3,r4,sl` at `0x16406a`. That forces `this` out of `sl` and produces
the 224-byte frame (the `this` spill at `0x163e04`).

Without the anchor the allocator instead puts `this` in `sl` (`0x163faa
mov sl,r5`) and sinks the `dividerOffset` reload into the gamma branch
(`0x16405a`), giving 9312 B / 216 frame. Every construct that forces
`dividerOffset` live at the call must either:

* use it as a call argument (the `pivotY`/`angle` slot: 1-byte `strd` diff), or
* emit a real memory operation (`volatile` store/load: +4 B or worse, and the
  RA collapses to a different shape), or
* use it in an always-true guard before the call (dead by the call, so the
  reload still sinks; frame 216).

The `pivotY` anchor is the only *free* use: it changes one register field in
an existing `strd` (`strd r4,sl` vs `strd r4,r4`, same 4 bytes) and nothing
else. The `angle` bitcast is the only other free slot, but it moves the
differing byte rather than removing it.

---

## 4. Residual hunks (unchanged)

* `strd r4,sl,[sp,#12]` vs `strd r4,r4,[sp,#12]` at `0x163ff4` (1 byte);
* the gamma-body `fp`/`r9` swap at `0x164052`/`0x164060`/`0x164076`/
  `0x16407e` (5 bytes; survives every variant above);
* the two dead `ldr.w r8,[sp,#108]` (`Globals::status`) reloads at
  `0x164176`/`0x164210` in the damage-indicator block (~508 raw bytes);
* the mission-panel / switch-tail register allocation (~838 raw bytes).

---

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10315
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10315/fdiff.py _ZN3Hud4drawExxP9PlayerEgobjj
python3 /dev/shm/w10315/align.py _ZN3Hud4drawExxP9PlayerEgobjj
cmake --build cmake-build-match --target check -j6
```

Scratch harness in `/dev/shm/w10315`: `runner.py` (patch/build/measure
raw+aligned+size+frame), `batch.py`..`batch12.py` (the sweeps above),
`hunks.py` (per-range raw diff), `Hud.base.cpp` (baseline source).
