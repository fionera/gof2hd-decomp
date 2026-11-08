# Hud::draw — wave 154 (w10015): volatile / function-pointer / gamma-local anchors all cost bytes

**Status:** analysis-only, 2026-11-08 (wave 154). Worktree `wt/w10015`,
base `master 4417db0b4` (wave 153). Target `_ZN3Hud4drawExxP9PlayerEgobjj`
(`src/game/ui/Hud.cpp`, `Hud::draw`), original **9332 B** (VMA `0x163b90`).

| metric | before | after |
|---|---:|---:|
| raw same-address diff | **1325** | **1325** (no source landed) |
| aligned (instruction-index) diff | **249** | **249** |
| size | 9332 | 9332 |
| frame | 224 | 224 |

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size-exact 4524/4524, `data_exact 843/843`, imports
**207**, lint **CLEAN**, drift **0**.

---

## 0. TL;DR

1. **The w9915 mission's three suggestions are all measured and all fail.**
   Removing the regen `pivotY = dividerOffset` anchor and replacing it with a
   `volatile` read, a function-pointer call, or a `dividerOffset` local used in
   the gamma block never keeps both **9332 B** and the **224-byte frame**.
2. The reason is arithmetic: the anchor is **free** (it changes one register in
   an existing `strd`: `strd r4,sl,[sp,#12]` vs `strd r4,r4,[sp,#12]`, same 4
   bytes). Every non-folding alternative emits at least one extra instruction:
   a `volatile` store/load costs **+4 B** (best: 9336 B) and a real call costs
   **+6..+12 B** (9436–9440 B).
3. Every zero-valued expression (`&0`, `*0`, `-self`, `%1`, comma, empty
   `switch`/`if`, `__builtin_expect`, `__builtin_assume`, `__sync_synchronize`,
   `__atomic_signal_fence`, `__builtin_prefetch`, address-taken, `sizeof`,
   `long long`/`short`/`float` locals) is folded away before register
   allocation and reproduces the plain 9312 B / 216-frame build exactly.
4. **New equivalent anchor found:** the *angle* slot
   (`*(float *) &dividerOffset`) is metrics-identical to the pivotY anchor
   (1325 / 249 / 9332 / 224). It is not a win — it merely moves the single
   differing byte from `0x163ff7` (`strd r4,sl,[sp,#12]`) to `0x163ff3`
   (`strd r2,sl,[sp,#4]`). The regen `pivotY` anchor is still the cheapest and
   the most semantically harmless.

---

## 1. Measured variants

`base` = current HEAD source (regen `pivotY = dividerOffset` + `Globals::status`
null-test anchor). All variants below replace the regen anchor with `pivotY = 0`
and add the listed construct unless noted. Full build, `check`-equivalent
compile; raw = same-address byte diff, al = instruction-index diff.

### 1.1 The mission's three suggestions

| variant | size | raw | al | frame |
|---|---:|---:|---:|---:|
| **base (anchor `pivotY=dividerOffset`)** | **9332** | **1325** | **249** | **224** |
| `int divGamma = dividerOffset;` before regen, used in gamma | 9312 | 8261 | 1009 | 216 |
| `volatile int dv = dividerOffset; (void)dv;` in regen | 9340 | 7546 | 524 | 224 |
| `volatile int dv = dividerOffset;` (no `(void)dv`) | 9336 | 7274 | 523 | 224 |
| `volatile short dv = (short)dividerOffset;` | 9340 | 7527 | — | 224 |
| `volatile char dv = (char)dividerOffset;` | 9340 | 7527 | — | 224 |
| `volatile unsigned dv = ...` | 9340 | 7546 | — | 224 |
| `volatile bool dv = dividerOffset != 0;` | 9352 | 7827 | — | 224 |
| `volatile int dv; dv = dividerOffset;` | 9340 | 7546 | — | 224 |
| `(void) *(volatile int *) &dividerOffset;` | 9296 | 8841 | 1054 | 224 |
| `(void) *(volatile short *) &dividerOffset;` | 9324 | 8824 | — | 216 |
| `volatile int *p = &dividerOffset; (void)p;` | 9288 | 8844 | 1960 | 216 |
| `*(volatile int *) &dividerOffset = dividerOffset;` | 9296 | 8842 | 1065 | 224 |
| `Globals::Canvas->SetColor((unsigned)dividerOffset);` | 9436 | 8940 | 1274 | 224 |
| `Globals::Canvas->DrawImage2D(img, 0, dividerOffset);` | 9440 | 8953 | 1275 | 224 |
| `(void) Globals::Canvas->GetImage2DWidth(dividerOffset);` | 9436 | 8940 | 1274 | 224 |

The two smallest volatile forms are `9336 B` (a single 4-byte
`str.w sl,[sp,#208]` volatile store, no read) and `9296 B` (a volatile load
that re-colours the whole function). Neither is 9332.

Declaring the volatile variable at the top of the function (so its stack slot
is low) does not help either: `volatile int/char/short dv;` at the top +
`dv = dividerOffset;` in the regen block gives **9292 B / 8823 / 1105 / 224**,
and a top-level `volatile int *dvp` + `dvp = &dividerOffset; (void)*dvp;`
gives **9296 B / 8841 / 1054 / 224**.

### 1.2 Every folded zero (reproduces the plain 9312 / 216 build)

`dividerOffset & 0`, `* 0`, `>> 31 & 0`, `- self`, `% 1`, `(x, 0)`,
`(void)&dividerOffset`, `(void)*(int*)&dividerOffset`, `sizeof`,
`(void)(x?0:0)`, empty `if (dividerOffset) {}`, empty `switch`, `while(x&0)`,
`do{}while(x&0)`, `__builtin_expect(x,0)`, `__builtin_assume`, `__sync_synchronize`,
`__atomic_signal_fence`, `__builtin_prefetch`, `long long`/`short`/`unsigned`/
`float`/`double` gamma locals, a `const int` alias, and a `Hud *self = this`
alias: **all 9312 / 8261 / 1009 / 216** (or a small RA perturbation of it).

### 1.3 `__builtin_expect` on the guards (PGO-like layout)

| variant | size | raw | al | frame |
|---|---:|---:|---:|---:|
| `expect(hasArmorRegen, 1)` | 9320 | 8110 | 1051 | 216 |
| `expect(gamma > 0, 0)` | 9332 | 8757 | 1232 | 216 |
| `expect(hasArmorRegen, 1)` + `expect(gamma>0, 0)` | 9332 | 8757 | 1232 | 216 |
| `expect(gamma > 0, 1)` | 9336 | 7975 | 982 | 216 |
| anchor + `expect(hasArmorRegen, 1)` | 9348 | 7950 | 580 | 224 |
| anchor + `expect(gamma>0, 0)` | 9372 | 8723 | 830 | 224 |

`__builtin_expect_with_probability` is **not available in clang 7.0.2**
(`use of undeclared identifier`). Note that `expect(gamma>0,0)` gives the right
**size** (9332) but the wrong **frame** (216): the guard hint changes block
layout without forcing `dividerOffset` into `sl`.

### 1.4 Other argument slots (anchor moved)

| slot | size | raw | al | frame |
|---|---:|---:|---:|---:|
| `pivotY` (base) | 9332 | **1325** | **249** | 224 |
| `angle = *(float *)&dividerOffset` | 9332 | **1325** | **249** | 224 |
| `srcX = dividerOffset` | 9360 | 8619 | 1188 | 224 |
| `srcY = dividerOffset` | 9360 | 8620 | 1189 | 224 |
| `pivotX = dividerOffset` | 9336 | 7312 | 454 | 224 |
| `width = dividerOffset` | 9300 | 7853 | 591 | 224 |
| `height = dividerOffset` | 9328 | 7173 | 550 | 224 |
| `posX = dividerOffset` | 9328 | 7169 | 549 | 224 |
| `posY = dividerOffset` | 9324 | 8501 | 1417 | 216 |
| hull-fill `pivotY` only (regen 0) | 9312 | 8261 | 1009 | 216 |
| hull + regen `pivotY` | 9332 | 1325 | 249 | 224 |

Only `pivotY` and the angle bitcast are free; the angle bitcast is
semantically worse (the angle is a live argument, the pivot is dead when
`angle == 0`).

### 1.5 Removing the `Globals::status` anchor instead

| variant | size | raw | al | frame |
|---|---:|---:|---:|---:|
| remove status null test, keep pivotY anchor | 9268 | 7456 | 636 | 224 |
| remove status null test, `pivotY=0` | 9244 | 8303 | 1361 | 216 |
| anchor + `expect(Globals::status != 0, 1)` | 9336 | 7126 | 452 | 224 |

The status anchor is worth 64 B of code; removing it does not recover a
9332-B shape either.

---

## 2. Why no alternative is free

The original regen block has **no** `dividerOffset` use (disasm
`0x163fb8..0x164004` never touches `sl`); the value is only live because of the
gamma lock-bracket add at `0x16406a` (`add.w r3,r4,sl`). Clang's scheduler
hoists the reload to `0x163fb2` (`ldr.w sl,[sp,#96]`) only when the value is
live-in to the regen block. The anchor is the only *free* way to make it
live-in:

* a `volatile` access is a real memory operation and cannot be folded, so it
  costs at least one 16/32-bit instruction (4 B for `str.w`/`ldr.w` to the
  high frame slots; the volatile store `str.w sl,[sp,#208]` is 4 B);
* a call costs at least `mov r1,sl` + `blx` (6 B) plus receiver setup;
* every side-effect-free expression is folded by InstCombine before RA, so the
  value is no longer live at the call.

The anchor's cost is exactly **one byte**: the register field of
`strd r4,sl,[sp,#12]` vs the original `strd r4,r4,[sp,#12]`. No source
construct found so far can keep the `sl` live range for less.

---

## 3. Residual hunks (unchanged)

* `strd r4,sl,[sp,#12]` vs `strd r4,r4,[sp,#12]` at `0x163ff4` (1 byte);
* the gamma-body `fp`/`r9` swap at `0x164052`/`0x164060`/`0x164076`
  (4 one-byte hunks; declaration/call reordering does not change it);
* the two dead `ldr.w r8,[sp,#108]` (`Globals::status`) reloads at
  `0x164176`/`0x164210` in the damage-indicator block (~481 raw bytes);
* the mission-panel / switch-tail register allocation (~838 raw bytes).

---

## 4. Reproduction

```
cd /home/debian/GalaxyOnFire2/wt/w10015
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10015/fdiff.py _ZN3Hud4drawExxP9PlayerEgobjj
python3 /dev/shm/w10015/align_diff.py _ZN3Hud4drawExxP9PlayerEgobjj
```

Scratch harness in `/dev/shm/w10015`: `runner2.py` (patch/build/measure
raw+aligned+size+frame), `batch1..23.py` (the sweeps above), `rawrange.py`
(per-hunk raw diff), `Hud.cpp.base` (baseline source).
