# Hud::draw — wave 151 (w9715): the 224-byte frame slot is `Globals::w`, not `this`

**Status:** analysis-only, 2026-11-08 (wave 151). Worktree `wt/w9715`,
base `master db9658a51` (wave 150). Target `_ZN3Hud4drawExxP9PlayerEgobjj`
(`src/game/ui/Hud.cpp`, `Hud::draw`), original **9332 B** (VMA `0x163b90`).

| metric | before | after |
|---|---:|---:|
| raw same-address diff | **1325** | **1325** (no source landed) |
| aligned (instruction-index) diff | **249** | **249** |
| size | 9332 | 9332 |
| frame | 224 | 224 |

Gate at base: `check` green (`byte_exact 4507`, `linked_exact 4507`,
size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0).

---

## 0. TL;DR

1. **The mission's premise is slightly off**: the extra 4-byte slot that makes
   the original's frame 224 is **not** the `this` spill.  The w9413 faithful
   shape (THBY dock2, no `Globals::status` anchor, `pivotY=0`) already spills
   `this` and reloads it at the same five sites — but at `[sp,#100]`
   (original `[sp,#104]`), because its whole frame is 8 B smaller.
2. **The missing slot is `Globals::w`** in the `field_0x4b8` block.  The
   original keeps the `DrawImage2D` PLT pointer live in `r8` from the armour
   block (`0x163f2e`) all the way to `0x16437c`, so `Globals::w` (needed for
   the x argument after the two `GetImage2D*` calls) is spilled to `[sp,#60]`.
   In the w9413 shape `r8` is reused for `Globals::w` at `0x16432e` and the
   call is a direct `blx DrawImage2D`, so no slot is needed.
3. **No source shape found that gets frame 224 + `pivotY=0` + correct fp/r9.**
   Every zero-valued `dividerOffset` expression that keeps it live across the
   armour-regen block (`>>31`, `<0`, `&0x80000000`, `(x>=0)-1`, …) recovers the
   224 frame and the five `this` reloads, but adds an instruction and changes
   the gamma-block RA (`fp`/`r9` swap) — raw 7319-7991, aligned 517+.
   The plain `pivotY = dividerOffset` anchor remains the unique 1325/249 shape.

## 1. Measured variants (full build)

`base` = current HEAD source (YBTH dock2, both anchors): **1325 / 249 / 9332 /
frame 224**.

| variant | size | raw | frame | `this` spill |
|---|---:|---:|---:|---|
| base (landed) | 9332 | **1325** | 224 | yes `[sp,#104]` |
| base + `pivotY=0` | 9312 | 8261 | 216 | no |
| base + `pivotX=dividerOffset`, `pivotY=0` | 9336 | 7312 | 224 | yes |
| base + `pivotY=dividerOffset>>31` | 9336 | 7319 | 224 | yes |
| base + `pivotY=dividerOffset<0` | 9336 | 7319 | 224 | yes |
| base + `pivotY=dividerOffset&0x80000000` | 9340 | 7581 | 224 | yes |
| base + `pivotY=(dividerOffset>=0)-1` | 9336 | 7319 | 224 | yes |
| base + `pivotY=0` + local `int w=Globals::w` in 4b8 block | 9312 | 8258 | 216 | no |
| base + `pivotY=0` + local `canvas` in 4b8 block | 9324 | 8127 | 216 | no |
| w9413 shape, `pivotY=0` | 9332 | 6964 | 216 | yes `[sp,#100]` |
| w9413 shape, `pivotY=0` + local `int w` in 4b8 block | 9332 | 6961 | 216 | yes |
| w9413 shape, `pivotY=dividerOffset>>31` | 9352 | 7942 | 224 | yes |

`__builtin_expect` on `hasShieldBar`/`hasArmorRegen`/gamma (0 or 1), ternary
`shieldImage`, `canvas` before/after the shield block, `dividerOffset` as
`const`/`unsigned`/inline-at-each-use, `armorY`/`armorBarY` type changes, and
`hasShieldBar != 0` are all metric-neutral or worse (9312-9352 B, frame 216).

## 2. Why the anchor is unique

The original's `dividerOffset` live range is continuous from the shield block
(`0x163e54`) through the armour block (`0x163f36`) into the gamma block
(`0x16406a`), with `sl` reloaded at `0x163fb2`.  The anchor makes it live
*inside* the armour-regen `DrawRegion2D`; without it the allocator splits the
range, spills `dividerOffset` to `[sp,#92]`, and uses `sl` for `this`
(`mov sl,r5` at `0x163faa`).  That is the whole 6936-byte raw cascade.

## 3. Open questions

1. Force the original's `r8 = DrawImage2D` live range in the w9413 shape
   (e.g. by keeping one `PaintCanvas*` receiver live across the armour and
   `field_0x4b8` blocks).  That would add the `[sp,#60]` slot and should shift
   the whole w9413 frame from 216 to 224, collapsing its 6964 raw.
2. If the `r8` live range can be reproduced faithfully, the `pivotY=0`
   w9413 shape becomes the winning shape (correct fp/r9 + 224 frame).

## 4. Reproduction

```
cd /home/debian/GalaxyOnFire2/wt/w9715
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9715/measure.py
python3 /dev/shm/w9715/align_diff.py _ZN3Hud4drawExxP9PlayerEgobjj
```
Scratch harness in `/dev/shm/w9715`: `measure.py` (size/raw/frame/this-spill),
`dis.py`, `diffins.py`, `sweep.py` + `s1..s15.py` (variants).
