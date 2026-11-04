# Hud::draw — wave-109 register/block sweep (94.6 % holds; no score gain) — w5717

**Status:** analysis-only pass, 2026-11-04 (wave 109). Worktree `wt/w5717`,
branch `wave109-w5717`, base `master 7c34c4098`. Target
`_ZN3Hud4drawExxP9PlayerEgobjj` (`src/game/ui/Hud.cpp`), original size **9332 B**
(Ghidra `Hud_draw` @ `0x173b90`, file `0x163b90`).

Gate on entry and exit (full `check`): **byte_exact 4500 / linked_exact 4500**,
size-exact **4524/4524**, `stub_zero_size 0`, `missing 0`, `wrong_type 0`,
`extra 0`, `data_exact 843/843`, imports **207**, lint **CLEAN**, drift **0**,
**RATCHET PASS**. `.ARM.extab` unchanged at `0x26294` (Hud LSDA 480 B), file
size +44. **No source change was landed**: every measured variant scored
`<= 94.6 %`, so this commit is the note only.

---

## 0. TL;DR

The wave-108 shape (`T P B H W D`, 9332 B, 94.6 %) is a local optimum under a
broad source-level sweep.  The remaining delta is **register allocation**, not
source semantics:

1. The **Canvas GOT pointer** (`&Globals::Canvas`) lives in `r5` in our build
   but in **`fp`** in the original.  This single choice drives the whole
   docking block (`mov r5,r9` vs `mov r5,fp; mov fp,r9`) and the second-bar
   block (`ldr r0,[r5,#0]` vs `ldr r0,[fp]`, `ldrd fp,r9,[sp,#116]` vs
   `mov r9,fp; ldr fp,[sp,#116]`).
2. The original schedules the docking `String text` **`operator+` call after**
   the `progress` / `barOffset` float math (`0x165926`); ours issues it
   immediately after `String(" ")` (`0x1658ea`).
3. The original keeps `this->field_0x279` in **`r8`** across the `getText` /
   `String` calls and reuses it for `if (field_0x279)`; ours reloads it
   (`ldrb.w r0,[sl,#633]; cmp r0,#0`, +6 B).
4. The original's `frameH` / `transferred` are `int` (no `sxth`); our `short`
   locals emit `sxth.w r0,fp` + `sxth r0,r0` (+6 B).  Switching to `int`
   removes the `sxth` but shifts the inline literal pool by -8 and scores
   lower (93.6 %).

The docking block's net size delta is **+2 B** in the landed shape; this shifts
every downstream branch target by +2 and is the largest single source of
unmatched instructions in the tail (`0x165d00..0x166004`, ~86 slots).

---

## 1. Measured variants (all Hud-only, real C++)

All variants were built with `cmake --build cmake-build-match --target link_so -j6`
and scored with `tools/verify/verify.py` (same normalizer as `report.json`).
`short/short` is the landed shape.  The original is 9332 B.

| variant | size | pct |
|---|---:|---:|
| **landed `T P B H W D` (HEAD)** | **9332** | **94.6** |
| boost `1.0f > rate` ternary | 9332 | 94.6 |
| boost `rate >= 1.0f` inverted ternary | 9332 | 94.5 |
| boost local `float rate = getBoostRate()` | 9324 | 93.3 |
| boost `if/else` SetColor | 9324 | 93.3 |
| `int frameH` only | 9328 | 93.8 |
| `int transferred` only | 9328 | 94.4 |
| `int frameH` + `int transferred` | 9324 | 93.6 |
| dock `barW` before `barH` (faithful call order) | 9332 | 94.4 |
| gamma `gammaY`/`gammaBarY` declaration swap | 9332 | 94.6 |
| mission-panel `field_0x1f4`/`field_0x1f8` swap | 9332 | 94.5 |
| `fireImage` as `int` / separate if-else / init-from-On | 9332 | 94.6 |
| `bool/int/uchar reverse = field_0x279` + use in ternary+if | 9332 | 94.1 |
| `reverse` only for the `if` (ternary still reads field) | 9332 | 94.4 |
| `String` operator+ after `progress` (text_after_prog) | 9340 | 91.7 |
| `String` operator+ after `barOffset` | 9324 | 93.7 |
| `String` operator+ after `barH/barW` | 9340 | 92.5 |
| `String` operator+ after `field_0x468` | 9344 | 92.3 |
| split `String textTmp` + append | 9336 | 93.0 |
| `String *textPtr` + late operator+ | 9336 | 92.0 |
| `String space` local + late operator+ | 9316 | 93.0 |
| `PaintCanvas *&canvas = Globals::Canvas` block-local | 9332 | 94.6 (identical codegen) |
| `PaintCanvas *canvas` block-local | 9288 | 57.5 |
| `PaintCanvas *&canvas` whole-function | 9328 | 87.4 |
| `top` as `int`/`unsigned int` | 9332/9332 | 94.6/94.5 |
| `barYOffset` as `short`/`unsigned int` | 9332 | 94.6 |
| `show` as `int`, `rate = 0.0f`, `fadeColor` unsigned | 9332 | 94.6 |
| `progress` ternary / compound `/=` / braced if | 9332 | 94.6 |
| `field_0x279 != 0` / `> 0` / `!= 1` in the if | 9332 | 94.6/94.6/94.6 |
| `field_0x279 == 1` in the if | 9332 | 94.4 |

Two full **720-permutation** sweeps of the docking declaration order
(`S P B H W D`, pieces = `String text`, `progress`+if, `barOffset`, `barH`,
`barW`, `dt/fade/top/barYOffset`) were run on top of the two most promising
type/boost shapes:

| sweep | best order | size | pct |
|---|---|---:|---:|
| landed boost block, `short/short` (wave-108, re-confirmed) | `S P B H W D` | 9332 | 94.6 |
| boost local `float rate` | `S P B H W D` | 9324 | 93.3 |
| `int transferred` | `S P B H W D` | 9328 | 94.4 |

No permutation in either sweep beat 94.6 %.

---

## 2. Ground truth for the remaining delta

Original docking block (`0x165880..0x1659c0`) vs ours:

```
orig 1658a0  mov r5,fp ; mov fp,r9 ; mov r9,r0     (ego->r5, Canvas-ptr->fp, textH->r9)
ours 1658a0  mov r5,r9 ; mov r9,r0                 (Canvas-ptr->r5, textH->r9, ego stays fp)

orig 1658e2  vmov s0,r4 ; cmp.w r8,#0 ; vmov s2,r5  (field_0x279 kept in r8)
ours 1658f4  sxth.w r0,fp ; ... ; 16592c ldrb.w r0,[sl,#633] ; cmp r0,#0
                                                    (short sxth + field reload)

orig 165926  operator+ (String text)               (after progress/barOffset)
ours 1658ea  operator+ (String text)               (before progress/barOffset)

orig 16593e  GetImage2DWidth  -> r8 (barW)
orig 16594e  GetImage2DHeight -> fp (barH)
ours 165946  GetImage2DHeight -> r8 (barH)
ours 165952  GetImage2DWidth  -> r6 (barW)
```

The original call order is **Width then Height** (`0x16593e`, `0x16594e`), so
the faithful source order is `barW` before `barH`; it scores 94.4 %, not 94.6 %.
The landed `barH`-then-`barW` shape is a fuzzy-metric tuning, not the original
call order.  (The wave-107 note's `0x16593e/0x16594a` labels were swapped.)

Second-bar block (`0x165b60..0x165d4c`) is -4 B vs the original for the same
reason: original keeps the Canvas pointer in `fp` (`ldr r0,[fp]`,
`mov r9,fp; ldr fp,[sp,#116]`), ours uses `r5` (`ldr r0,[r5,#0]`,
`ldrd fp,r9,[sp,#116]`).

The `.ARM.extab` Hud record is still **480 B / 78 rows** in every tested
variant; no tested shape changed it.

---

## 3. Reproduction

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
```

Scratch sweeps lived in `/dev/shm/w5717` (`sweep*.py`, `sweep720*.log`,
`regdiff.py`, `align.py`).

---

## 4. Proven vs hypothesis

**Proven:** the landed 9332-B `T P B H W D` shape is the best of the tested
source-level space (720 docking permutations x {landed boost, local `rate`,
`int transferred`}, plus the ~60 single-knob variants in the table above);
the original calls `GetImage2DWidth` before `GetImage2DHeight`; the original
keeps `field_0x279` in `r8` and schedules the docking `operator+` after the
float math; the original's `frameH`/`transferred` are `int` (no `sxth`).

**Hypothesis:** the remaining ~5.4 % is entirely clang-7 `-Oz` register
allocation / scheduling.  The root lever is the Canvas GOT pointer's
callee-saved register (`fp` in the original vs `r5` in ours); no source shape
tried here moves it without also changing the inline literal-pool slotting and
losing more matches than it gains.  A future pass could try to make the
compiler keep `&Globals::Canvas` in `fp` (e.g. by reshaping an earlier block
that forces the allocator's tie-break), which would fix the docking block, the
second-bar block, and the +2 B docking net delta together.
