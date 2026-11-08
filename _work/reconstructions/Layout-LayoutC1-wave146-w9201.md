# `Layout::Layout` C1/C2 — wave 146 / worker w9201
## Device-major / shared-tail hypothesis tested; best size-exact candidate is 7072 B but raw 5,723 (baseline 5,704). No source win landed.

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr `0xd1028`,
size **7072 B**, source `src/game/ui/Layout.cpp`.  Baseline = wave-145 master `31297fadc`
(target-function raw same-address diff **5,704**, `.text` differing bytes 85,651).

## 1. Ground truth re-confirmed: the original is a flat per-field chain, not a hand-written device split

The original's control flow is exactly what clang 7.0.2 `-Oz` if-conversion produces from a
**flat chain of 204 per-field ternaries in the iOS source order** (see
`layout_ctor/ios_source_order.md`):

```
d1036  ldrb.w sl,[iPadHD] ; cmp; beq d126c        <- HD arm hoisted
d1042..d1268  HD stores (all fields, SLP-vectorised); b d21e2
d126c  ldrb.w fp,[n9] ; cmp; beq d1570            <- n9 arm hoisted
d127e..d12fc  n9 stores (0x4..0x6c, 0x74/0x78, 0x2c0..0x2cc); b d1860
d1570..d185e  default/retina stores (same fields); fall through d1860
d1860..d21e2  shared !iPadHD tail (tests n9/retina/iPad per field)
d21e2..d2800  common tail shared by all three arms (0x1e4..0x320, 0x3e0..0x3fc)
```

So the "shared tail" is already emitted once in the original.  The residual is the *shape*
of the per-field selects inside it, not a missing source-level `if/else`.

## 2. Device-major body generated from `lay_orig.json` (new hypothesis)

`layout_ctor/gen.py hdblock` was run with the **iOS order** (204 fields) as both the HD-arm
and non-HD-arm order, producing a literal `if (Globals::iPadHD) { all HD stores } else { all
non-HD chains }` body (`LayoutC1-w9201-cand-asc.cpp` is a related reorder artifact).

| variant | size | raw diff | vbsl | clz |
|---|---:|---:|---:|---:|
| HEAD baseline | **7072** | **5704** | 7 | 2 |
| generated device-major (`hdblock`, iOS order) | 6624 | 5927 | 15 | 10 |
| HEAD + whole `0x16c..0x1e8` ascending (iOS order) | 6844 | 5986 | 6 | 3 |
| HEAD + `0x1b0..0x1dc` ascending (`1cc` ternary) | 7024 | 5895 | 6 | 2 |
| wave-145 candidate (`LayoutC1-w9008-candidate.cpp`) | 7068 | **5675** | 10 | 5 |

The generated body is 448 B *smaller* because a full literal HD arm lets the compiler
tail-merge more of the common suffix and SLP-vectorise more aggressively (15 `vbsl` vs the
original's 11).  The original's 7072 B comes from the **less-vectorised** shared tail
(11 `vbsl`, 6 `clz`), not from duplicating the tail.

## 3. The 4-byte gap of the wave-145 candidate (7068 -> 7072)

The candidate is the closest structural match (10 `vbsl`, 5 `clz`; original 11/6).  It is
exactly **4 B short**.  The function's code end is `d284a` (`pop`) vs the original's `d2800`;
the candidate's code is 74 B longer and its literal pool 78 B smaller.

Pool-slack measurements (all real `link_so` builds, candidate as the base):

| filler appended to the ctor | size | raw diff |
|---|---:|---:|
| none | 7068 | 5675 |
| `this->choiceWindowOpen = 0;` (4 B code) | 7068 | 5678 |
| `this->choiceWindowOpen = 0; this->field_0x3dc = 0;` (10 B code) | 7076 | 5693 |
| `this->field_0x2f8 = 150.0f;` (8 B code + 4 B pool) | **7072** | **5723** |
| `this->field_0x2f8 = 76.0f;` (8 B code + 4 B pool) | 7072 | 5725 |
| `if (Globals::n9 == 2) this->choiceWindowOpen = 0;` | 7076 | 5703 |
| `this->helpWindowY = 76;` (merges with the existing store) | 7000 | 5701 |

So the pool has ~8 B of slack: a 4 B code-only filler is absorbed, a 12 B code+pool filler
lands at exactly 7072.  The best size-exact variant is therefore
`_work/reconstructions/LayoutC1-w9201-candidate-7072.cpp` = candidate + `field_0x2f8 =
150.0f;` at the end: **7072 B, raw 5723** (gate green, `byte_exact` 4507/4507 unchanged).
It is 19 raw bytes worse than the baseline, so it is a structural experiment, not a win.

## 4. Why the missing 11th `vbsl` cannot be forced cheaply

The original's second `vbsl` block (`d20b4..d215c`) has **three** groups:
`0x1b0..0x1bc`, `0x1c0..0x1cc`, `0x1d0..0x1dc` (+ `eventBannerScale` at `0x1e0`).
The candidate's corresponding block (`d219e..d21b6`) has only **two** groups
(`0x1b0`, `0x1c0`); `0x1d0..0x1dc` stays scalar (`str.w` at `d2166`, `d218a`).
Making `0x1b0..0x1dc` ascending contiguous lands the third `vst1` but costs 44-48 B
(7024/5895, 7060/5728), so the group alone cannot fill the 4 B gap.

The original's third group is a `vbsl q8, q11, q10` with the same shared retina mask
(`clz r1,r8`) as the first two; our compiler splits it because the `0x1cc` select and the
`0x2c0` store are scheduled between the groups.  Forcing the mask with a shared
`unsigned char retina` local for the whole 12-statement group gives 7088 B / 5844 (6 `clz`,
matching the original's `clz` count) — 16 B over, not 4.

## 5. Not landed

`src/game/ui/Layout.cpp` is unchanged (baseline **7072 / 5704**, gate green:
byte 4507/4507, linked 4507/4507, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0).  The best size-exact candidate is committed as
`_work/reconstructions/LayoutC1-w9201-candidate-7072.cpp` (7072 / 5723) and the raw
wave-145 candidate as `_work/reconstructions/LayoutC1-w9008-candidate.cpp` (7068 / 5675).

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w9201
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9201/fdiff.py _ZN6LayoutC1Ev     # 7072/7072, diff 5704
# size-exact candidate:
cp _work/reconstructions/LayoutC1-w9201-candidate-7072.cpp src/game/ui/Layout.cpp
# -> 7072/7072, diff 5723
```

Scratch (`/dev/shm/w9201/`): `Layout.base.cpp`, `Layout.cand_asc*.cpp`,
`Layout.cand_grp.full.cpp`, `Layout.reorder*.full.cpp`, `Layout.final.cpp`, `reorder.py`,
`stores.py`, `orig.dis`, `ours.dis`, `vcand.dis`, `v*.dis`.

## 7. Open questions

* The original's 11th `vbsl` (`0x1d0..0x1dc`) needs a source shape that keeps the three
  groups in one basic block while preserving the 7072 B size.  The candidate's `0x1cc`
  if/else chain and the `0x2c0` store are the two statements that split the block.
* The pool has ~8 B of slack; a genuine (not filler) 12 B code+pool statement that the
  original also has would land 7072 without a semantic mismatch.  The candidate's missing
  4 B is one pool word, not a missing store.
