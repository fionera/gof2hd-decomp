# Hud::draw — wave 159 (w10515): every "volatile read without the store" still loses the 224-byte frame

**Status:** analysis-only, 2026-11-08 (wave 159). Worktree `wt/w10515`,
base `master cfde1f5f2` (wave 158). Target `_ZN3Hud4drawExxP9PlayerEgobjj`
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

1. The wave-158 mission's three suggestions were re-measured with **new source
   forms**: a volatile read of the local at every plausible point (before the
   `if`, in the regen block, at the declaration, through a `volatile int *`,
   through a `volatile int &`, through a `volatile` layout-field read), a
   volatile store at the declaration followed by a plain load, a
   comma-operator "use but pass 0", masks/shifts that are 0 for the 0/1
   `dividerOffset`, and a narrower live range (moving the declaration past the
   shield block).
2. **None keeps the 224-byte frame with 9332 B.** The `pivotY = dividerOffset`
   anchor is still the unique shape. The anchor's cost is exactly **one byte**
   in one `strd` (`strd r4,sl,[sp,#12]` vs `strd r4,r4,[sp,#12]` at
   `0x163ff4`).
3. The gamma-body `fp`/`r9` swap (5 one-byte hunks) also survives every new
   gamma form tried this wave (volatile field reads, hoisted field locals,
   inlining `gammaBarY` into the call, `unsigned`/`long`/`short` locals).
4. The remaining 1319 raw bytes are the damage-indicator `Globals::status`
   reload scheduling (~508 raw) and the mission-panel/switch-tail RA (~838
   raw), both independent of the anchor.

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

Frame `0xe0` = **224 B** in both. `this` is spilled at `0x163e04`
(`str r5,[sp,#104]`) and reloaded at `0x164040` (`ldr r6,[sp,#104]`); `sl`
holds `dividerOffset` from `0x163fb2` (`ldr.w sl,[sp,#96]`) to `0x16406a`
(`add.w r3,r4,sl`).

The original's `dividerOffset` is loaded at `0x163df4`
(`ldr.w sl,[r1,#0x1e8]`), spilled at `0x163e08`, used in the shield block
(`0x163e54`), reloaded at `0x163f36` for the armour lock bracket, and
reloaded again at `0x163fb2` for the gamma block. The last reload is placed
right after the hull-fill call and **before** the `hasArmorRegen` branch
(`cbz r0,0x164008`).

---

## 2. Measured variants (full build; base = 1325 / 249 / 9332 / 224)

All variants replace the anchor line with `pivotY = 0` plus the listed change
unless noted.

### 2.1 Volatile reads of the local (the wave-158 suggestion)

| variant | size | raw | al | frame |
|---|---:|---:|---:|---:|
| **base (anchor `pivotY=dividerOffset`)** | **9332** | **1325** | **249** | **224** |
| `volatile int dv = dividerOffset;` before `if`, `dv` in gamma | 9284 | 8857 | 1769 | 216 |
| `volatile int dv = dividerOffset;` before `if`, `dv` in gamma, anchor kept | 9332 | 8857 | 1769 | 216 |
| `volatile int dv = dividerOffset;` at declaration, `int g = dv;` before `if`, `g` in gamma | 9204 | 8812 | 2054 | 216 |
| `volatile int dv = dividerOffset;` at declaration, `dv` in gamma | 9280 | 8869 | 1958 | 216 |
| `volatile int dividerOffset = ...;` + anchor | 9292 | 8791 | 1955 | 216 |
| `volatile int dividerOffset = ...;` + `pivotY=0` | 9288 | 8765 | 1926 | 216 |
| `int *const p = &dividerOffset; int g = *(volatile int *)p;` before `if` | 9204 | 8812 | 2054 | 216 |
| `volatile int *const p = &dividerOffset; int g = *p;` before `if` | 9204 | 8812 | 2054 | 216 |
| `volatile int *const p = &dividerOffset;` gamma `gammaY + *p` | 9280 | 8869 | 1958 | 216 |
| `int *const p = &dividerOffset;` gamma `gammaY + *p` | 9312 | 8261 | 1009 | 216 |
| `int g = *(volatile int *)&dividerOffset;` at declaration | 9276 | 8867 | 1737 | 216 |
| `int g = *(volatile int *)&Globals::layout->field_0x1e8;` before `if` | 9328 | 7900 | 1251 | 216 |
| same at declaration | 9316 | 8262 | 1017 | 216 |

The volatile read always forces the compiler to materialise the local in a
fresh stack slot (a `str` at the read point) or to abandon the `sl` live
range; either way `this` goes back into `sl` and the frame drops to 216.

### 2.2 Zero-valued expressions that keep the use (all fold or add code)

| variant | size | raw | al | frame |
|---|---:|---:|---:|---:|
| `pivotY = (dividerOffset, 0)` | 9312 | 8261 | 1009 | 216 |
| `pivotY = dividerOffset - dividerOffset` | 9312 | 8261 | 1009 | 216 |
| `pivotY = dividerOffset ^ dividerOffset` | 9312 | 8261 | 1009 | 216 |
| `pivotY = dividerOffset & ~dividerOffset` | 9312 | 8261 | 1009 | 216 |
| `pivotY = dividerOffset - (dividerOffset & 1)` | 9340 | 7581 | 536 | 224 |
| `pivotY = dividerOffset & ~1` | 9340 | 7581 | 536 | 224 |
| `pivotY = dividerOffset & 0xfffffffe` | 9340 | 7581 | 536 | 224 |
| `pivotY = dividerOffset >> 1` | 9336 | 7319 | 517 | 224 |
| `pivotY = (int)((unsigned)dividerOffset >> 31)` | 9336 | 7319 | 517 | 224 |

The comma/self-subtract forms are folded by InstCombine (use dropped, frame
216); the mask/shift forms keep the frame but add a real instruction, so the
size grows and the branch target shifts.

### 2.3 Different field alias as `pivotY`

| field | size | raw | al | frame |
|---|---:|---:|---:|---:|
| `dividerOffset` (base) | 9332 | **1325** | **249** | **224** |
| `this->hasArmorRegen` | 9332 | 2291 | 973 | 216 |
| `this->field_0x4d4` | 9332 | 2290 | 973 | 216 |
| `this->field_0x470` | 9332 | 2291 | 974 | 216 |
| `this->shieldHitFlash` | 9332 | 2291 | 973 | 216 |
| `this->field_0x48c` | 9332 | 2291 | 974 | 216 |
| `this->field_0x490` | 9332 | 2291 | 974 | 216 |
| `Globals::layout->field_0x1e8` (direct, local kept) | 9332 | 6785 | 857 | 216 |

Any field other than `dividerOffset` is not the value already live in `sl`,
so `this` returns to `sl` and the frame is 216.

### 2.4 Narrower live range

| variant | size | raw | al | frame |
|---|---:|---:|---:|---:|
| base | 9332 | **1325** | **249** | **224** |
| move declaration to just before the armour lock bracket (direct field in shield block) | 9332 | 2154 | 601 | 224 |
| move declaration to just before `if (hasArmorRegen)` (direct field earlier) | 9340 | 8346 | 1522 | 224 |

### 2.5 Gamma-block rewrites (fp/r9 swap survives all)

| variant | size | raw | al | frame |
|---|---:|---:|---:|---:|
| base | 9332 | **1325** | **249** | **224** |
| `int f44a = field_0x44a; gammaBarY = 0x448 - f44a + 0x448;` | 9332 | 1353 | 269 | 224 |
| `unsigned int f44a = ...` | 9332 | 1353 | 269 | 224 |
| `int f448 = field_0x448; ...` | 9332 | 1325 | 249 | 224 |
| both `f448`/`f44a` hoisted before gammaY | 9332 | 1325 | 249 | 224 |
| `int f44a = *(volatile unsigned short *)&field_0x44a;` | 9332 | 1348 | 258 | 224 |
| `int f448 = *(volatile unsigned short *)&field_0x448;` | 9332 | 1370 | 271 | 224 |
| both volatile | 9332 | 1357 | 262 | 224 |
| inline `gammaBarY` into both call sites | 9348 | 7687 | 558 | 224 |
| `unsigned int`/`long` `gammaBarY` | 9332 | 1325 | 249 | 224 |
| `short gammaBarY` | 9332 | 1492 | 495 | 224 |

### 2.6 Damage-block `Globals::status` anchors (side experiment)

| variant | size | raw | al | frame |
|---|---:|---:|---:|---:|
| base (anchor on `field_0x4b8` only) | 9332 | **1325** | **249** | **224** |
| remove `Globals::status &&` from `field_0x4b8` | 9268 | 7456 | 636 | 224 |
| add `Globals::status &&` to `field_0x4b0` | 9340 | 7113 | 448 | 224 |
| add `Globals::status &&` to `field_0x4b4` | 9348 | 7070 | 512 | 224 |
| add to both | 9360 | 7228 | 563 | 224 |
| `Status *st = Globals::status;` before `field_0x4ac`, anchor uses `st` | 9320 | 7122 | 662 | 224 |
| `Status *st = Globals::status;` before `field_0x4b0`, anchor uses `st` | 9340 | 7348 | 474 | 224 |
| `Status *st = Globals::status;` before `field_0x4b4`, anchor uses `st` | 9336 | 6867 | 518 | 224 |

All of these change the size or the aligned count; none improves on the base.

---

## 3. Why the anchor is still unique

The 224-byte frame exists because `this` is spilled (`0x163e04`) and `sl`
holds `dividerOffset` across the `hasArmorRegen` branch. The only way to make
the compiler keep `dividerOffset` in `sl` across the regen call is to have a
use of it **inside** the regen block. The regen block's only free use is a
`DrawRegion2D` argument, and the only argument slot that does not change the
instruction size is `pivotY` (the `strd r4,r4` becomes `strd r4,sl`, same 4
bytes). Every other slot either shifts the code (`pivotX`/`height`/`posX`) or
adds a conversion (`angle` as an int), and every non-argument use either
folds away (losing the frame) or adds a memory operation (shifting the branch
target).

The gamma-body `fp`/`r9` swap is a pure allocator tie-break between the two
`ldrh.w` loads of `field_0x44a`/`field_0x448`; it is independent of the anchor
and survives every reordering, type, and volatile form tried.

---

## 4. Residual hunks (unchanged)

* `strd r4,sl,[sp,#12]` vs `strd r4,r4,[sp,#12]` at `0x163ff4` (1 byte,
  function offset `0x0467`);
* the gamma-body `fp`/`r9` swap at `0x164052`/`0x164060`/`0x164076`/
  `0x16407e` (5 one-byte hunks);
* the two dead `ldr.w r8,[sp,#108]` (`Globals::status`) reloads at
  `0x164176`/`0x164210` in the damage-indicator block (~508 raw bytes);
* the mission-panel / switch-tail register allocation (~838 raw bytes).

---

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10515
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10515/fdiff.py _ZN3Hud4drawExxP9PlayerEgobjj
python3 /dev/shm/w10515/align_diff.py _ZN3Hud4drawExxP9PlayerEgobjj
cmake --build cmake-build-match --target check -j6
```

Scratch harness in `/dev/shm/w10515`: `runvar.py` (patch/build/measure
raw+aligned+size+frame), `v_*.py` (the sweeps above), `rawruns.py` (per-range
raw diff), `adiff.py` (aligned hunk dump), `Hud.base.cpp` (baseline source).
