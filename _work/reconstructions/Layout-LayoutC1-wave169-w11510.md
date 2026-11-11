# `Layout::Layout` C1/C2 — wave 169 / worker w11510

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original
vaddr `0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Baseline (master `00c403d5a`): raw **5,650**, aligned **2,284**, 7,072 B.
Exit: raw **5,640**, aligned **2,283**, 7,072 B.

## Landed change

Two statement moves in the shared tail (semantics unchanged):

1. `field_0x290` (offset 0x290) moved from just before `field_0x2f4` to
   just before `field_0x284_sliderSlot5Enabled`, i.e. after
   `choiceWindowTitleHeight` (0x304).  `5,650 -> 5,647`.
2. `field_0x284_sliderSlot5Enabled` then moved from just after
   `field_0x290` to just before `field_0x94`.  `5,647 -> 5,640`.

Measured `5,650 -> 5,640` raw, aligned `2,284 -> 2,283`, size exact
7,072.  Gate: byte 4507, linked 4507, size 4524/4524, data 843/843,
imports 207, lint CLEAN, RATCHET PASS.

## Original tail structure (objdump of `0xd21e2..0xd2800`)

Confirms the w11410 reading: the original tail is a three-way device
dispatch with shared tails, not a per-field ternary cascade.

```
A  iPadHD block  d2204..d22b2: 0x1fc=2,0x1ec=26,0x234=1,0x238.a=70,
   0x27c=28,0x280=0,scrollBarInset=1,scrollBarHandle=0,rewardBoxWidth=140,
   rewardBoxY=77,rewardBoxY2=132, then seven vst1 groups 0x204..0x278.
   Tail: if (n9) -> d24de (height 352 / x 75) else -> d2530
   (height retina?392:196 / x retina?100:50).
B  !n9 block     d22c0..d240c: retina selects for every field 0x204..0x278
   (plus 0x1fc/0x1ec/0x27c), then `b d2498`.
C  n9 block      d240e..d2494: 0x1fc=4,0x1ec=34,0x234=2,0x238.a=90,
   0x27c=30, seven vst1 groups, falls into d2498.
S  shared B/C    d2498..d252c: 0x280, scrollBarHandle, scrollBarInset,
   rewardBoxWidth (n9 112 / iPadLarge 200 / iPad 100 / retina?150:75),
   rewardBoxY (n9 57 / iPadLarge 110 / iPad 55 / retina?76:38),
   rewardBoxY2 = n9?111:(rewardBoxY+17)<<retina (float round-trip).
T  shared tail   d2548..d2800: 0x2bc, footerButtonOffset, 0x200, 0x1f0,
   0x1f4, 0x1f8, 0x284, 0x285, 0x286, 0x288, 0x28c, 0x290, then
   0x2f4..choiceWindowTitleHeight, 0x30c..0x318, helpWindowX/Y.
```

## Shapes measured and not landed (real `link_so` + `fn_diff`)

| shape | raw | aligned | size |
|---|---:|---:|---:|
| baseline HEAD | 5,650 | 2,284 | 7,072 |
| **landed `field_0x290` move** | **5,647** | **2,285** | **7,072** |
| **landed `field_0x290` + `field_0x284` moves** | **5,640** | **2,283** | **7,072** |
| full nested dispatch A / !n9 / n9 (all 0x204..0x27c fields, shared rewardBox) | 5,830 | 2,555 | 6,880 |
| same + 0x204 + blk.b in dispatch | 6,030 | 2,340 | 6,832 |
| dispatch with ascending-offset field order | 5,928 | 2,705 | 6,948 |
| dispatch with descending-offset field order | 5,929 | 3,008 | 6,956 |
| shared rewardBox block only (rest unchanged) | 5,843 | 2,596 | 7,012 |
| rewardBoxWidth/Y/Y2 as one explicit 5-arm chain | 6,034 | 2,456 | 7,168 |
| all tail `n9 ? A : B` ternaries flipped to `!n9 ? B : A` | 5,650 | 2,284 | 7,072 |
| 0x280 `!n9` chain moved to tail start / before 0x1fc / 0x1ec | 5,825 / 5,742 / 5,748 | 2,465 / 2,380 / 2,403 | 7,040 / 7,056 / 7,024 |
| `field_0x1e4 = (int)(float)(iPadHD ? 4*1.40625f : ...)` | 5,829 | 2,268 | 7,072 |
| `field_0x290` moved before 0x30c / helpWindowX | 5,657 / 5,658 | 2,287 / 2,283 | 7,072 / 7,084 |

The structured dispatch is consistently smaller (6.8-7.0 KB) than the
per-field-ternary source: clang tail-merges the shared blocks and drops
literal-pool words, so a full rewrite cannot keep the 7,072 B size.  The
tail's remaining raw residue is instruction selection / SLP phase, not
missing stores.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w11510
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
cmake --build cmake-build-match --target check -j2
```

Scratch `/dev/shm/w11510/`: `Layout.base.cpp`, `gen.py` (dispatch
generator), `swapsearch.py`, `movesearch.py`, `move290.py`, `aldiff.py`.
