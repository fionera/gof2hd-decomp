# `Layout::Layout` C1/C2 — wave 141 / worker w8709
## The `0x1b0..0x1e0` `vbsl` block is a vectorised `retinaDisplay ? retina : default` select; a 15-byte `eventBannerScale`/`0x98` swap lands at 5,713

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr `0xd1028`,
size **7072 B**, source `src/game/ui/Layout.cpp`.  Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`, image base 0x10000.
Baseline = wave-141 master `1d10d0694` (7072 B, target-function same-address diff **5,728**,
`.text` differing bytes 84,567).

## Result — landed: 5,728 -> 5,713 by swapping `eventBannerScale` and `field_0x98`

The new expression shape below reproduces the original's `vbsl` block byte-for-byte in shape
(three `vbsl` + three `vst1.32` + one `vstr`), but the block lands 186 B late and the whole
function re-lays-out; the best size-7072 nested-group variant found is **5,792** raw diff.
The landed change is a single semantic-neutral statement swap found by a 205-statement
swap sweep: `eventBannerScale` moves from the top block (before `0x2d0`) to the position of
`field_0x98` (after `0x90`), and `field_0x98` moves to the old `eventBannerScale` slot.

| metric | before | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** |
| target-function same-address diff | 5,728 | **5,713** |
| `.text` differing bytes | 84,567 | **84,552** |
| `byte_exact` / `linked_exact` | 4507 / 4507 | **4507 / 4507** |
| `size_exact` / `data_exact` | 4524 / 843 | 4524 / 843 |
| imports / lint / drift | 207 / CLEAN / 0 | 207 / CLEAN / 0 |
| symbol parity | 5370 / 5370 | 5370 / 5370 |

`cmake --build cmake-build-match --target check -j6` -> **RATCHET PASS**.

The swap moves the n9-branch scalar store from `0x1e0` to `0x98` (the n9 branch still has
one 4-byte extra store, so the literal pool stays at `0x2e8`), but `0x98`'s n9 value
(`420*1.40625f` = 590) matches the original's n9 branch better than `0x1e0`'s `2.0f` did.
Region histogram: arms 632 -> 631, pool 233 -> 233, middle 4,558 -> 4,544, tail 305 -> 305.

## 1. Ground truth: the block is two separate device blocks, not one ternary chain

Original HD arm ends at `d1268` (`b.w d21e2`).  The non-HD path is:

```
d126c: ... if (n9) { early n9 stores } ...        ; d127e..d12fc
d1570: ... if (retina) { early retina stores } ... ; d1570..d185e
d1860: ... shared tail (fields 0x7c..0x1ac) ...    ; d1860..d20b4
d20b4: vbsl block for 0x1b0..0x1e0                 ; retina/default path
d21a2: fixed-vector block for 0x1b0..0x1e0         ; n9 path
d21e2: common tail (0x1e4/0x1e8, then 0x204..0x26c ...)
```

The `d20b4` block is a **vectorised select of retina vs default** for the 13 fields
`0x1b0,0x1b4,0x1b8,0x1bc, 0x1c0,0x1c4,0x1c8,0x1cc, 0x1d0,0x1d4,0x1d8,0x1dc, 0x1e0`.
The n9 path has its own **three fixed-vector stores** at `d21a2..d21de`, and the HD arm has
its own three vector stores at `d122a..d123e`.  The exact constants (read from the literal
pools at `d1430`, `d14a8`, `d14c8`, `d2a00`, `d2a10`, `d2a20`, `d2a30`, `d2a40`, `d2a50`,
`d2a60`, `d2a70`, `d2a80`) are:

| fields | HD | n9 | retina | default |
|---|---|---|---|---|
| `0x1b0..0x1bc` | `18,42,46,1` | `19,45,40,2` | `26,60,66,2` | `13,30,33,1` |
| `0x1c0..0x1cc` | `92,26,40,50` | `80,34,43,54` | `132,38,58,72` | `66,19,29,36` |
| `0x1d0..0x1dc` | `7,17,5,14` | `7,18,6,14` | `10,24,8,20` | `5,12,4,10` |
| `0x1e0` | `1.40625f` | `2.0f` | `2.0f` | `1.0f` |

(`0x1c0..0x1cc` HD values are the truncated `66/19/29/36 * 1.40625f`; `0x1d0..0x1dc`
HD values are `5/17/4/10 * 1.40625f`.)

The original `d20b4` mask is built with the signed-shift idiom, not a `vceq`:

```
clz   r1, r8              ; r8 = Globals::retinaDisplay
mov   r1, r1, lsr #5      ; r1 = (retina == 0)
vdup.16 d18, r1
vmovl.u16 q9, d18
vshl.s32 q9, q9, #31      ; 0 or 0x80000000
vneg.s32 q8, q8           ; q8 = -31
vshl.s32 q8, q9, q8       ; 0 or 1
vbsl  q13, q12, q9        ; retina ? q9 : q12
vbsl  q14, q12, q9
vbsl  q8,  q11, q10
```

The source shape that produces it is a **single nested if/else on the device flags**, with the
13 assignments contiguous in ascending offset order:

```cpp
if (Globals::iPadHD) {
    this->field_0x1b0 = 18;  this->field_0x1b4 = 42;  this->field_0x1b8 = 46;
    this->field_0x1bc = 1;   this->field_0x1c0 = 66 * 1.40625f;
    this->field_0x1c4 = 19 * 1.40625f; this->field_0x1c8 = 29 * 1.40625f;
    this->field_0x1cc = 36 * 1.40625f; this->field_0x1d0 = 5 * 1.40625f;
    this->field_0x1d4 = 17.0f; this->field_0x1d8 = 4 * 1.40625f;
    this->field_0x1dc = 10 * 1.40625f; this->eventBannerScale = 1.40625f;
} else if (Globals::n9) {
    this->field_0x1b0 = 19;  this->field_0x1b4 = 45;  this->field_0x1b8 = 40;
    this->field_0x1bc = 2;   this->field_0x1c0 = 80;  this->field_0x1c4 = 34;
    this->field_0x1c8 = 43;  this->field_0x1cc = 54;  this->field_0x1d0 = 7;
    this->field_0x1d4 = 18;  this->field_0x1d8 = 6;   this->field_0x1dc = 14;
    this->eventBannerScale = 2.0f;
} else {
    this->field_0x1b0 = Globals::retinaDisplay ? 26 : 13;
    this->field_0x1b4 = Globals::retinaDisplay ? 60 : 30;
    this->field_0x1b8 = Globals::retinaDisplay ? 66 : 33;
    this->field_0x1bc = Globals::retinaDisplay ? 2 : 1;
    this->field_0x1c0 = Globals::retinaDisplay ? 132 : 66;
    this->field_0x1c4 = Globals::retinaDisplay ? 38 : 19;
    this->field_0x1c8 = Globals::retinaDisplay ? 58 : 29;
    this->field_0x1cc = Globals::retinaDisplay ? 72 : 36;
    this->field_0x1d0 = Globals::retinaDisplay ? 10 : 5;
    this->field_0x1d4 = Globals::retinaDisplay ? 24 : 12;
    this->field_0x1d8 = Globals::retinaDisplay ? 8 : 4;
    this->field_0x1dc = Globals::retinaDisplay ? 20 : 10;
    this->eventBannerScale = Globals::retinaDisplay ? 2.0f : 1.0f;
}
```

This variant emits the original's three-`vbsl` block at `d216e` (original `d20b4`; +186 B)
and the n9 three-vector block at `d2222` (original `d21a2`; +128 B), but the function size is
**7076** and raw diff **5,861**.  A 500-iteration move-to-position climb constrained to
`size == 7072` reaches **5,792** (still worse than 5,728).  The block is structurally right;
the residual is the 186 B common-arm shift, not the select itself.

## 2. The 16 B pre-pool gap: 12 B HD arm + 4 B n9 branch

Baseline literal pool starts at `d1310` (offset `0x2e8`); original at `d1300` (`0x2d8`).
The gap is exactly:

* **12 B in the HD arm.**  Original HD arm ends at `d1268`; ours at `d1274`.  Our HD arm
  carries scalar stores that the original emits in the common tail:
  `0x204` (`movs r1,#12; str.w`), `0x2b8`, `0x23c`, `0x2bc`, `0x178` (`strd`), `0x2c0`.
  The original's HD arm is the tight vector run `d122a..d123e` (`vst1` to `0x1b0/0x1c0/0x1d0`)
  then `0x138` + `0x1e0` only.  The original stores `0x204..0x210` as one `vst1` at `d227e`
  in the common tail (HD constants `[12,5,7,5]` at `d2a90`), and `0x23c..0x26c` as
  `vst1` groups at `d2292..d22ae`.
* **4 B in the n9 branch.**  Original n9 branch `d127e..d12fc` = 126 B; ours
  `d128a..d130c` = 130 B.  The extra 4 B is `str.w r1,[r0,#0x1e0]` at `d12c8`: our source
  assigns `eventBannerScale` at the *top* of the constructor, so the n9 branch stores it.
  The original stores `0x1e0` in the shared-tail n9 block at `d21c6` (and in the
  retina/default `vbsl` block at `d2128`), never in the n9 branch.  Moving
  `eventBannerScale` out of the top removes the 4 B but changes the group code and costs
  30-190 raw-diff bytes (best 7024/5917; 6968/5936 with the group reordered).

Fixing the 12 B HD arm requires moving the `0x204/0x208/0x20c/0x210` and `0x23c..0x26c`
assignments out of the `iPadHD` arm in source (they are common-tail vector groups in the
original), which is the same device-major restructuring as the 186 B common-arm shift.

## 3. Variants measured (all real `link_so` builds)

| variant | size | raw diff |
|---|---:|---:|
| HEAD (wave-141 baseline) | 7072 | 5,728 |
| **landed: `eventBannerScale` <-> `field_0x98` swap** | **7072** | **5,713** |
| nested group, ascending, `eventBannerScale` in group | 7076 | 5,861 |
| nested group + 500-iteration size-7072 climb | 7072 | 5,792 |
| nested group, `eventBannerScale` after group | 7068 | 5,873 |
| nested group, `eventBannerScale` at top | 7108 | 5,923 |
| flat group ascending + `eventBannerScale` before `0x1b0` | 7024 | 5,917 |
| flat group ascending + `eventBannerScale` after `0x1dc` | 6968 | 5,936 |
| group moved to constructor start | 6940 | 6,124 |

Region histogram (baseline): arms `0x000..0x2d8` 632, pool `0x2d8..0x548` 233,
middle `0x548..0x1880` 4,558, tail `0x1880..0x1ba0` 305.
Nested-group variant: 630 / 264 / 4,731 / 292.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w8709
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
python3 /dev/shm/w8709/fdiff.py _ZN6LayoutC1Ev          # 7072/7072, diff 5713
```

Scratch harnesses (`/dev/shm/w8709/`, not committed): `fdiff.py`, `regions.py`,
`split.py` (205-statement constructor splitter, handles single-statement `if/else` bodies),
`climb.py` (move-to-position climb with size gate), `insert_group.py` (group-position sweep),
`sweep_ebs.py` (`eventBannerScale` position sweep), `stores.py` (store-offset extractor),
`Layout.base.cpp` (pristine baseline), `Layout.newgroup.cpp` (nested-group variant).

## 5. Open questions

* The 186 B common-arm shift is the dominant residual.  The original's common tail starts at
  `d21e2`; ours at `d229e` (baseline) / `d225c` (nested group).  Aligning it would make the
  `vbsl` block byte-cheap.
* The original's common tail stores `0x204..0x210` and `0x23c..0x26c` as `vst1` groups
  (`d227e`, `d2292`), i.e. these assignments are not in the `iPadHD` arm.  Reconstructing
  that split is the next step for both the 12 B pool gap and the 186 B shift.
* `0x1e0` belongs in the shared tail (n9 block `d21c6` / retina block `d2128`), not at the
  top of the constructor; moving it is a real 4 B pool win but must be combined with the
  common-tail restructuring to avoid a net diff regression.
