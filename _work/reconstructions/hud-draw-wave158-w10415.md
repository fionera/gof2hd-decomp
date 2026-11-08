# Hud::draw — wave 158 (w10415): the `pivotY = dividerOffset` anchor survives every new liveness hypothesis

**Status:** analysis-only, 2026-11-08 (wave 158). Worktree `wt/w10415`,
base `master 9e4795206` (wave 157). Target `_ZN3Hud4drawExxP9PlayerEgobjj`
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

1. The wave-157 mission's five suggestions were re-measured with **new source
   forms** on the wave-157 source: `volatile` read (load, store, reference,
   pointer, top-of-function slot, low-slot byte), a real member-function-pointer
   call, a `dividerOffset` local used in the gamma block (before the shield
   block, before the `if`, inside the block), `do/while`/`while`/`for`
   around the regen block, and every plausible "different field" as the
   `pivotY` argument. **None keeps 9332 B / 224 frame with a lower raw diff.**
2. The `pivotY = dividerOffset` anchor is still the unique 9332 B / 224-frame
   shape. The angle bitcast is metrics-identical (1325 / 249 / 9332 / 224).
3. New measured near-misses:
   * `volatile int dv = dividerOffset;` inside the regen block:
     **9336 B / 7274 / 523 / 224**. It reproduces the original's
     `ldr.w sl,[sp,#96]` at `0x163fb2` and `strd r4,r4,[sp,#12]` (pivotY=0),
     but the 4-byte volatile store `str.w sl,[sp,#208]` shifts the regen block
     and the branch target, so the raw diff explodes to 7274.
   * `pivotY = dividerOffset` on the *gamma region* call as well:
     **9332 B / 1328 / 250 / 224** (3 raw worse; the gamma block's r3 becomes
     r0, the fp/r9 swap survives).
   * `volatile int *dvp = &Globals::layout->field_0x1e8; int dividerOffset = *dvp;`
     with the anchor: **9332 B / 1341 / 255 / 224**.
4. The gamma-body `fp`/`r9` swap is independent of the anchor: it survives the
   angle anchor, the gamma-region anchor, every gamma declaration/statement
   order tried, `*2`/paren/neg association rewrites, and local temp forms.

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
`0x163ff4` (function offset `0x0467`).

`this` is spilled at `0x163e04` (`str r5,[sp,#104]`) and reloaded at
`0x164040` (`ldr r6,[sp,#104]`); `sl` holds `dividerOffset` from `0x163fb2`
(`ldr.w sl,[sp,#96]`) to `0x16406a` (`add.w r3,r4,sl`).

---

## 2. Measured variants (full build; base = 1325 / 249 / 9332 / 224)

All variants replace the anchor line with `pivotY = 0` plus the listed change
unless noted.

### 2.1 `volatile` forms

| variant | size | raw | al | frame |
|---|---:|---:|---:|---:|
| **base (anchor `pivotY=dividerOffset`)** | **9332** | **1325** | **249** | **224** |
| `volatile int dv = dividerOffset;` in regen | 9336 | 7274 | 523 | 224 |
| `volatile char/short/uchar/ushort dv = dividerOffset;` in regen | 9336 | 7274 | 523 | 224 |
| `volatile bool dv = dividerOffset;` in regen | 9348 | 7805 | 544 | 224 |
| `volatile long long dv = dividerOffset;` in regen | 9344 | 7667 | 526 | 224 |
| `volatile int dv;` before `if`, `dv = dividerOffset;` in regen | 9292 | 8823 | 1105 | 224 |
| `volatile char/short dv;` before `if`, assign in regen | 9292 | 8823 | 1105 | 224 |
| `volatile int dv;` before `dividerOffset`, assign in regen | 9292 | 8823 | 1105 | 224 |
| `int dv = *(volatile int *)&dividerOffset;` before `if`, `dv` in gamma | 9204 | 8812 | 2054 | 216 |
| `int dv = *(volatile int *)&dividerOffset;` before `if`, anchor | 9212 | 8875 | 2072 | 216 |
| `int dv = *(volatile int *)&dividerOffset;` in regen, anchor | 9296 | 8841 | 1055 | 224 |
| `volatile int &dividerOffset = Globals::layout->field_0x1e8;` | 9332 | 2723 | 1147 | 216 |
| `volatile int &dividerOffset = ...` + anchor | 9332 | 2723 | 1147 | 216 |
| `volatile int *dvp = &Globals::layout->field_0x1e8; int dividerOffset = *dvp;` + anchor | 9332 | 1341 | 255 | 224 |
| `volatile int *dvp = ...; int dividerOffset = *dvp;` + `pivotY=*dvp` | 9328 | 7602 | 1538 | 216 |
| `volatile int dv0 = Globals::layout->field_0x1e8; int dividerOffset = dv0;` | 9276 | 8867 | 1737 | 216 |
| same + anchor | 9296 | 8832 | 1058 | 224 |

### 2.2 Function-pointer calls

| variant | size | raw | al | frame |
|---|---:|---:|---:|---:|
| member-function-pointer `dr = &PaintCanvas::DrawRegion2D`, `pivotY=0` | 9312 | 8261 | 1009 | 216 |
| member-function-pointer + anchor | 9332 | **1325** | **249** | 224 |
| local `PaintCanvas *rc = Globals::Canvas` receiver, `pivotY=0` | 9312 | 8261 | 1009 | 216 |

The member-pointer + anchor is byte-identical to the base (the compiler already
calls through `r8`).

### 2.3 `dividerOffset` local used in the gamma block

| variant | size | raw | al | frame |
|---|---:|---:|---:|---:|
| `int dv = dividerOffset;` before shield, `dv` in gamma | 9312 | 8261 | 1009 | 216 |
| same + anchor on `dividerOffset` | 9332 | **1325** | **249** | 224 |
| `const int dv = dividerOffset;` before shield, `dv` in gamma | 9312 | 8261 | 1009 | 216 |
| `int dv = dividerOffset;` before shield, `dv` in gamma, anchor on `dv` | 9332 | **1325** | **249** | 224 |
| `int *divp = &dividerOffset;`, `*divp` in gamma | 9312 | 8261 | 1009 | 216 |
| same + `pivotY=*divp` | 9332 | **1325** | **249** | 224 |
| `int *divp = &Globals::layout->field_0x1e8;`, `*divp` in gamma | 9348 | 8783 | 1354 | 216 |
| same + `pivotY=*divp` | 9356 | 8851 | 1308 | 216 |

### 2.4 Loop forms around the regen block

| variant | size | raw | al | frame |
|---|---:|---:|---:|---:|
| `do { regen } while (0)` | 9312 | 8261 | 1009 | 216 |
| `for (int i=0;i<1;i++) { regen }` | 9312 | 8261 | 1009 | 216 |
| `while (1) { regen; break; }` | 9312 | 8261 | 1009 | 216 |
| `do { regen } while (dividerOffset >= 0)` | 9348 | 8795 | 1488 | 216 |
| `if (dividerOffset >= 0) { regen }` | 9324 | 6980 | 1504 | 216 |

### 2.5 "Different field" as `pivotY`

| field | size | raw | al | frame |
|---|---:|---:|---:|---:|
| `dividerOffset` (base) | 9332 | **1325** | **249** | 224 |
| `this->field_0x44c` | 9312 | 8261 | 1010 | 216 |
| `armorBarY` | 9320 | 8114 | 1051 | 216 |
| `armorY` | 9332 | 3766 | 1120 | 216 |
| `this->field_0x440` | 9312 | 8261 | 1010 | 216 |
| `this->field_0x43e` | 9332 | 2290 | 973 | 216 |
| `this->field_0x43c` | 9332 | 2290 | 973 | 216 |
| `this->hasArmorRegen` | 9332 | 2291 | 973 | 216 |
| `regenW` | 9324 | 8103 | 990 | 216 |
| `this->field_0x446` | 9328 | 7421 | 836 | 216 |
| `Globals::layout->field_0x1e8` | 9332 | 6785 | 857 | 216 |

### 2.6 Boolean / cast forms of `dividerOffset` as `pivotY`

| form | size | raw | al | frame |
|---|---:|---:|---:|---:|
| `dividerOffset` (base) | 9332 | **1325** | **249** | 224 |
| `(long)` / `(long long)` / `(unsigned)` | 9332 | **1325** | **249** | 224 |
| `dividerOffset ? 1 : 0` / `!!dividerOffset` / `!= 0` | 9340 | 7571 | 548 | 224 |
| `dividerOffset & 1` | 9340 | 7581 | 536 | 224 |
| `(unsigned char)` / `(short)` / `(unsigned short)` / `(char)` | 9336 | 7319 | 517 | 224 |
| `dividerOffset > 0` / `>= 1` | 9344 | 7693 | 537 | 224 |
| `dividerOffset == 1` | 9356 | 8004 | 628 | 224 |

### 2.7 Second anchors / multiple uses

| variant | size | raw | al | frame |
|---|---:|---:|---:|---:|
| base | 9332 | **1325** | **249** | 224 |
| regen + gamma-region `pivotY` | 9332 | 1328 | 250 | 224 |
| regen + gamma-region `angle` bitcast | 9332 | 1328 | 250 | 224 |
| regen + gamma-region `pivotX` | 9336 | 7107 | 453 | 224 |
| regen + gamma-region `srcX` | 9344 | 8472 | 1123 | 224 |
| regen `pivotY` + `angle` bitcast on same call | 9352 | 7939 | 592 | 224 |
| regen `pivotY` + `posX` | 9336 | 7295 | 455 | 224 |
| regen `pivotY` + `height` | 9336 | 7293 | 453 | 224 |
| shield + regen `pivotY` | 9336 | 7634 | 487 | 224 |
| hull + regen `pivotY` | 9324 | 8042 | 630 | 224 |
| all four fills `pivotY` | 9336 | 7643 | 520 | 224 |

### 2.8 Gamma-block rewrites (fp/r9 swap survives all)

| variant | size | raw | al | frame |
|---|---:|---:|---:|---:|
| base | 9332 | **1325** | **249** | 224 |
| `int a448 = field_0x448; gammaBarY = a448 - field_0x44a + a448;` | 9332 | **1325** | **249** | 224 |
| both `a448`/`a44a` locals | 9332 | **1325** | **249** | 224 |
| `field_0x448 - (field_0x44a - field_0x448)` | 9332 | **1325** | **249** | 224 |
| `-field_0x44a + field_0x448 + field_0x448` | 9332 | 1353 | 269 | 224 |
| `field_0x448 + field_0x448 - field_0x44a` | 9328 | 7021 | 562 | 224 |
| `field_0x444 * 2 - field_0x442` (gammaY) | 9332 | 1340 | 482 | 224 |
| `field_0x448 * 2 - field_0x44a` (gammaBarY) | 9328 | 7021 | 562 | 224 |
| both `*2` | 9328 | 7033 | 587 | 224 |
| both `*2`, `unsigned short` locals | 9332 | 1400 | 505 | 224 |
| `gammaBarY` declared after the first two draws | 9340 | 7441 | 549 | 224 |
| swap 2nd/3rd gamma draws | 9336 | 7184 | 538 | 224 |
| `gammaBarY` declared before `gammaY` | 9332 | **1325** | **249** | 224 |
| four `unsigned short` field temps | 9332 | **1325** | **249** | 224 |

### 2.9 `__builtin_expect` on the guards

| variant | size | raw | al | frame |
|---|---:|---:|---:|---:|
| anchor + `expect(hasArmorRegen, 0)` | 9336 | 8331 | 1414 | 216 |
| anchor + `expect(hasArmorRegen, 1)` | 9348 | 7950 | 580 | 224 |
| anchor + `expect(hasShieldBar, 0)` | 9336 | 8798 | 1278 | 216 |
| anchor + `expect(hasShieldBar, 1)` | 9352 | 7699 | 762 | 224 |
| no anchor + `expect(hasArmorRegen, 0/1)` | 9336/9320 | 8331/8110 | 1414/1051 | 216 |
| no anchor + `expect(gamma>0, 0/1)` | 9332/9336 | 8757/7975 | 1232/982 | 216 |

---

## 3. Why the anchor is still unique

The original reloads `dividerOffset` into `sl` at `0x163fb2`, *before* the
`hasArmorRegen` branch, and keeps it live across the regen call to the gamma
`add.w r3,r4,sl` at `0x16406a`. That forces `this` out of `sl` and produces
the 224-byte frame (the `this` spill at `0x163e04`).

Without the anchor the allocator puts `this` in `sl` (`0x163faa mov sl,r5`)
and sinks the `dividerOffset` reload into the gamma branch (`0x16405a`),
giving 9312 B / 216 frame.

Every non-argument liveness construct either:

* emits a real memory operation (`volatile` store/load: at least +4 B, and the
  RA collapses to a different shape), or
* changes the CFG (`do/while`, `if`, `switch`: frame 216), or
* is folded by InstCombine before RA (all zero-valued expressions).

The `pivotY` anchor is the only *free* use: it changes one register field in
an existing `strd` (`strd r4,sl` vs `strd r4,r4`, same 4 bytes) and nothing
else. The `angle` bitcast is the only other free slot, but it moves the
differing byte rather than removing it.

The `volatile int dv = dividerOffset;` variant is the closest structural
match: it does reproduce the original's `ldr.w sl,[sp,#96]` at `0x163fb2` and
`strd r4,r4,[sp,#12]` (pivotY=0) with frame 224, but the 4-byte volatile store
`str.w sl,[sp,#208]` at `0x163fd8` grows the regen block and shifts the branch
target from `0x164008` to `0x16400c`, so the raw diff is 7274 and the size is
9336.

---

## 4. Residual hunks (unchanged)

* `strd r4,sl,[sp,#12]` vs `strd r4,r4,[sp,#12]` at `0x163ff4` (1 byte,
  function offset `0x0467`);
* the gamma-body `fp`/`r9` swap at `0x164052`/`0x164060`/`0x164076`/
  `0x16407e` (5 one-byte hunks; every reordering/association rewrite leaves it
  unchanged);
* the two dead `ldr.w r8,[sp,#108]` (`Globals::status`) reloads at
  `0x164176`/`0x164210` in the damage-indicator block (~508 raw bytes);
* the mission-panel / switch-tail register allocation (~838 raw bytes).

---

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10415
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10415/fdiff.py _ZN3Hud4drawExxP9PlayerEgobjj
python3 /dev/shm/w10415/align_diff.py _ZN3Hud4drawExxP9PlayerEgobjj
cmake --build cmake-build-match --target check -j6
```

Scratch harness in `/dev/shm/w10415`: `run.py` (patch/build/measure
raw+aligned+size+frame), `v1.py`..`v29.py` (the sweeps above), `hunks.py`
(per-range raw diff), `Hud.base.cpp` (baseline source).
