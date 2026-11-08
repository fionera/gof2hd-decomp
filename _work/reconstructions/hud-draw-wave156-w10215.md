# Hud::draw — wave 156 (w10215): the `pivotY` anchor is still the unique 9332 B / 224-frame lever

**Status:** analysis-only, 2026-11-08 (wave 156). Worktree `wt/w10215`,
base `master cf74f4ee6` (wave 155). Target `_ZN3Hud4drawExxP9PlayerEgobjj`
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

1. The mission's four suggestions (`volatile` read, function-pointer call,
   `dividerOffset` local used in the gamma block, `do/while` around the regen
   block) were re-measured on the wave-155 source. **None keeps 9332 B and
   the 224-byte frame.** The only 9332/224 shape is still the landed
   `pivotY = dividerOffset` anchor.
2. New hypotheses swept this wave (nested always-true `if`/`switch`/`while`
   around the regen call, `__builtin_expect` on every guard, `__builtin_assume`
   range hints, address-taken / pointer / `register` / `const` / `static`
   locals, hoisted field locals, volatile reads before the regen block,
   `unsigned short` gamma locals, integer-association rewrites of `gammaY` /
   `gammaBarY`, gamma statement moves) — **all fail to keep 9332/224**.
3. The residual anchor cost is exactly **one byte** in one `strd`
   (`strd r4,sl,[sp,#12]` vs `strd r4,r4,[sp,#12]` at `0x163ff4`), plus the
   independent gamma-body `fp`/`r9` swap (5 one-byte hunks). Both are
   irreducible from source: the original regen block has **no**
   `dividerOffset` use, so the `sl` live range is a pure allocator artifact.

---

## 1. Register map at the armour-regen `DrawRegion2D` call

Both the original and the landed anchor build have **identical** register
maps here; only the pivot store differs.

| location | original | ours (anchor) |
|---|---|---|
| `r0` | `Globals::Canvas` | same |
| `r1` | `armorRegenFillImage` (`0x2c0`) | same |
| `r2` | `0` (srcX) | same |
| `r3` | `0` (srcY) | same |
| `r4` | `0` (angle) | same |
| `r5` | `this` | same |
| `r6` | `armorBarY` | same |
| `fp` (r11) | `ego->player` | same |
| `r9` | `&Globals::Canvas` | same |
| `sl` (r10) | `dividerOffset` | `dividerOffset` |
| `sp+0` | `regenW` (width) | same |
| `sp+4` | `field_0x44c` (height) | same |
| `sp+8` | `0` (angle) | same |
| `sp+12` | `0` (pivotX) | same |
| `sp+16` | **`0` (pivotY)** | **`dividerOffset` (pivotY)** |
| `sp+20` | `field_0x440` (posX) | same |
| `sp+24` | `armorBarY` (posY) | same |

Frame `0xe0` = **224 B** in both. `this` is spilled at `0x163e04`
(`str r5,[sp,#104]`) and reloaded at `0x164040` (`ldr r6,[sp,#104]`); `sl`
holds `dividerOffset` from `0x163fb2` (`ldr.w sl,[sp,#96]`) to `0x16406a`
(`add.w r3,r4,sl`).

The `sl` value is clobbered by `armorBarY` at `0x163f0a` (`ldrh.w sl,[r1]`,
the hull-fill `posY`) and re-loaded at `0x163fb2`, so liveness must begin
*after* `0x163fb2`; a hull-fill anchor therefore removes the reload and
produces a different (9328 B / 216-frame) shape.

---

## 2. New variants measured (full build; base = 1325 / 249 / 9332 / 224)

All replace the anchor line with `pivotY = 0` plus the listed change unless
noted.

### 2.1 Mission suggestions

| variant | size | raw | al | frame |
|---|---:|---:|---:|---:|
| **base (anchor `pivotY=dividerOffset`)** | **9332** | **1325** | **249** | **224** |
| `volatile int dv = dividerOffset;` before regen | 9336 | 7274 | 523 | 224 |
| `volatile int dv = dividerOffset; (void)dv;` | 9340 | 7546 | 524 | 224 |
| `volatile int dividerOffset = ...` (whole var) | 9288 | 8765 | 1926 | 216 |
| `int d = *(volatile int *)&dividerOffset;` before regen, `d` in gamma | 9204 | 8812 | 2054 | 216 |
| `do { regen } while (0)` | 9312 | 8261 | 1009 | 216 |
| `do { regen } while (dividerOffset < 0)` | 9348 | 8795 | 1488 | 216 |
| `int d = dividerOffset;` before regen, `d` in gamma | 9312 | 8261 | 1009 | 216 |
| `*(volatile int *)&dividerOffset` as `pivotY` | 9296 | 8839 | 1053 | 224 |
| `Globals::Canvas->SetColor((unsigned)dividerOffset)` | 9436 | 8940 | 1274 | 224 |

### 2.2 Always-true guards around the regen block

| variant | size | raw | al | frame |
|---|---:|---:|---:|---:|
| `if (hasArmorRegen && dividerOffset != 0)` | 9328 | 8020 | 1079 | 216 |
| `if (hasArmorRegen && dividerOffset >= 0)` | 9324 | 8469 | 956 | 216 |
| `if (hasArmorRegen && (dividerOffset \| 0))` | 9328 | 8020 | 1079 | 216 |
| `if (hasArmorRegen && (dividerOffset + 1))` | 9328 | 8020 | 1079 | 216 |
| `if (hasArmorRegen && (unsigned)dividerOffset <= 1)` | 9324 | 8469 | 956 | 216 |
| `if (hasArmorRegen && dividerOffset*dividerOffset == dividerOffset)` | 9324 | 8469 | 956 | 216 |
| nested `if (dividerOffset >= 0)` inside regen | 9324 | 6980 | 1504 | 216 |
| nested `if (dividerOffset != -1)` inside regen | 9324 | 6980 | 1504 | 216 |
| `switch (dividerOffset) { default: regen }` | 9312 | 8261 | 1009 | 216 |
| `while (dividerOffset >= 0) { regen; break; }` | 9324 | 6980 | 1504 | 216 |
| `for (int i=0; i<=dividerOffset; i++) { regen }` | 9360 | 8854 | 1340 | 216 |

### 2.3 `__builtin_expect` / `__builtin_assume`

| variant | size | raw | al | frame |
|---|---:|---:|---:|---:|
| `__builtin_expect(hasArmorRegen, dividerOffset)` | 9312 | 8261 | 1009 | 216 |
| `__builtin_expect(hasArmorRegen, dividerOffset != 0)` | 9312 | 8261 | 1009 | 216 |
| `__builtin_expect(hasArmorRegen, 0)` (anchor kept) | 9336 | 8331 | 1414 | 216 |
| `__builtin_expect(hasArmorRegen, 1)` (anchor kept) | 9348 | 7950 | 580 | 224 |
| `__builtin_expect(gamma>0, 1)` (anchor kept) | 9344 | 7422 | 607 | 224 |
| `__builtin_expect(gamma>0, 0)` (anchor kept) | 9372 | 8723 | 830 | 224 |
| `__builtin_assume(dividerOffset == 0 \|\| dividerOffset == 1)` (anchor kept) | 9332 | **1325** | **249** | **224** |
| `__builtin_assume(dividerOffset >= 0)` (anchor kept) | 9332 | **1325** | **249** | **224** |

The two `__builtin_assume` rows are byte-identical to the base (the assume is
already implied by the unmodified IR), so they are not a new lever.

### 2.4 Locals / storage classes / gamma rewrites

| variant | size | raw | al | frame |
|---|---:|---:|---:|---:|
| `register int dividerOffset` | 9312 | 8261 | 1009 | 216 |
| `const int dividerOffset` | 9312 | 8261 | 1009 | 216 |
| `static int dividerOffset` | 9488 | 7947 | 1604 | 216 |
| direct `Globals::layout->field_0x1e8` at every use | 9360 | 8423 | 1693 | 216 |
| `(void)&dividerOffset` | 9312 | 8261 | 1009 | 216 |
| `int *dp = &dividerOffset;` used as `*dp` in gamma | 9312 | 8261 | 1009 | 216 |
| hoisted `img`/`h`/`x` field locals in regen | 9752 | 9031 | 1956 | 224 |
| `bool hr = hasArmorRegen; if (hr)` (anchor kept) | 9332 | **1325** | **249** | **224** |
| `gammaY = 444+444-442` | 9332 | 1340 | 482 | 224 |
| `gammaBarY = 448+448-44a` | 9328 | 7021 | 562 | 224 |
| `gammaBarY = -44a + 448 + 448` | 9332 | 1353 | 269 | 224 |
| `gammaBarY` as `unsigned short` | 9332 | 1492 | 495 | 224 |
| `gammaBarY` computed after first draw | 9340 | 7439 | 331 | 224 |
| `gammaBarY` split into `+=` statements | 9332 | 1325 | 249 | 224 |

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
else.

---

## 4. Residual hunks (unchanged)

* `strd r4,sl,[sp,#12]` vs `strd r4,r4,[sp,#12]` at `0x163ff4` (1 byte);
* the gamma-body `fp`/`r9` swap at `0x164052`/`0x164060`/`0x164076`/
  `0x164078`/`0x164080` (5 one-byte hunks; declaration/statement reordering
  and every integer-association rewrite leave it unchanged);
* the two dead `ldr.w r8,[sp,#108]` (`Globals::status`) reloads at
  `0x164176`/`0x164210` in the damage-indicator block (~481 raw bytes);
* the mission-panel / switch-tail register allocation (~838 raw bytes).

---

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10215
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10215/fdiff_local.py _ZN3Hud4drawExxP9PlayerEgobjj
python3 /dev/shm/w10215/align_local.py _ZN3Hud4drawExxP9PlayerEgobjj
cmake --build cmake-build-match --target check -j6
```

Scratch harness in `/dev/shm/w10215`: `runner.py` (patch/build/measure
raw+aligned+size+frame), `batch1..11.py` (the sweeps above), `hunks.py`
(per-range raw diff), `Hud.base.cpp` (baseline source).
