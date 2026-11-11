# `Layout::Layout` C1/C2 — wave 169 / worker w11410

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original
vaddr `0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Baseline (master `ec3ceb07c`): raw **5,656**, aligned **2,283**, 7,072 B.
Exit: raw **5,652**, aligned **2,284**, 7,072 B.

## Landed change

Two adjacent-statement swaps in the tail (statement order only; semantics
unchanged):

1. `field_0x27c` moved from just after `field_0x25c` to just after the
   `field_0x260` if/else chain (i.e. after `field_0x258`/`0x25c`/`0x260`,
   before `field_0x270`).
2. `field_0x285 = 1;` moved before `scrollBarInset` (it was between
   `scrollBarInset` and `scrollBarHandle`).

Measured `5,656 -> 5,652` raw, aligned `2,283 -> 2,284`, size exact
7,072.  Gate: byte 4507, linked 4507, size 4524/4524, data 843/843,
imports 207, lint CLEAN, RATCHET PASS.

## Original tail structure (objdump of `0xd21e2..0xd2800`)

Registers: `sl = iPadHD`, `fp = n9` (non-iPadHD paths only), `ip = n9`,
`r8 = retinaDisplay`, `r9 = !n9`, `lr = !(n9 || iPadHD)`.

```
d21e2  field_0x1e4 = (int)(float)s0   ; s0 = iPadHD 5.625f / n9 6.0f / retina 8.0f / else 4.0f
       field_0x1e8 = lr ? retina : 1
d2202  if (!iPadHD) goto d22b8
       BLOCK A (iPadHD) d2204..d22b2:
         0x1fc=2, 0x1ec=26, 0x234=1, 0x238.a=70, 0x27c=28, 0x280=0,
         scrollBarInset=1, scrollBarHandle=0, rewardBoxWidth=140,
         rewardBoxY=77, rewardBoxY2=132,
         vst1 groups 0x204..0x278
         if (!n9) goto d2530 else goto d24de
d22b8  if (!n9) goto d240e
       BLOCK B (retina/else) d22c0..d240c: retina selects for
         0x1fc,0x1ec,0x224,0x228,0x22c,0x230,0x234,0x238,0x24c,0x250,
         0x20c,0x218,0x220,0x204,0x208,0x210,0x214,0x21c,0x23c,0x240,
         0x244,0x248,0x278,0x264,0x268,0x26c,0x270,0x25c,0x254,0x258,
         0x260,0x274,0x27c; b d2498
d240e  BLOCK C (n9) d240e..d2494: 0x1fc=4, 0x1ec=34, 0x234=2,
         0x238.a=90, 0x27c=30 + same seven vst1 groups
d2498  shared (n9/else): 0x280, scrollBarInset, scrollBarHandle,
       rewardBoxWidth, rewardBoxY, rewardBoxY2 (from the NEW rewardBoxY);
       n9 -> 112/57/111, iPadLarge -> 200/110, iPad -> 100/55,
       else retina?150:75 / retina?76:38
d24de  n9 tail: rewardBoxHeight=352, rewardBoxX=75
d2530  else tail: rewardBoxHeight=retina?392:196, rewardBoxX=retina?100:50
d2548  shared: 0x2bc, footerButtonOffset, 0x200, 0x1f0, 0x1f4, 0x1f8,
       0x284, 0x285, 0x286, 0x288, 0x28c, 0x290
d2654  shared: 0x2f4, 0x2f8, 0x2fc, 0x300, 0x304, choiceWindowTitleHeight
d2704  shared: 0x30c, 0x310, 0x314, 0x318
d2774  shared: helpWindowX, helpWindowY
```

Original block order is **iPadHD, else, n9**; the per-field-ternary source
makes clang emit **iPadHD, n9, else**.  The `field_0x280` `!n9` rewrite
from wave 166 (already in baseline) is the one place that flip was a raw
win.

## Shapes measured and not landed (real `link_so` + `fn_diff`)

| shape | raw | aligned | size |
|---|---:|---:|---:|
| baseline HEAD | 5,656 | 2,283 | 7,072 |
| **landed 0x27c + 0x285 swaps** | **5,652** | **2,284** | **7,072** |
| `field_0x1e4 = (int)(iPadHD ? 4*1.40625f : (n9?6:(retina?8:4)))` | 5,792 | 2,258 | 7,072 |
| `field_0x1e4`/`0x1e8` swapped | 5,954 | 2,374 | 6,984 |
| `field_0x1e4`+`0x1e8` moved to start of 0x280 dispatch | 6,191 | 2,548 | 6,852 |
| rewardBoxY before rewardBoxY2 (field, no cast) | 5,712 | 2,258 | 7,080 |
| rewardBoxY before rewardBoxY2 + `(int)(float)(...)` | 5,746 | 2,329 | 7,096 |
| rewardBoxY2 `(int)(float)` cast only | 5,731 | 2,298 | 7,088 |
| explicit rewardBoxWidth/Y/Y2 device chain | 6,032 | 2,450 | 7,176 |
| explicit scrollBarInset/Handle in outer iPadHD dispatch | 5,857 | 2,601 | 7,144 |
| 0x20c/0x218/0x220 grouped consecutively | 5,759 | 2,348 | 7,132 |
| 0x20c/0x218/0x220 shared local variable | 6,094 | 3,917 | 6,968 |
| 0x20c/0x218/0x220 one shared 4-way block | 5,759 | 2,348 | 7,132 |
| whole tail reordered ascending by field offset | 5,755 | 2,353 | 7,108 |
| iPadHD-specific fields grouped in block-A order | 5,869 | 2,411 | 7,008 |
| full explicit iPadHD/n9/else blocks (duplicated) | 9,446 | 4,732 | 9,956 |
| `field_0x285` moved after `field_0x284` | 5,676 | 2,285 | 7,072 |

Adjacent-swap greedy over the 66 tail statements (dispatch + shared tail)
finds exactly the two landed swaps; no further single swap improves raw.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w11410
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
cmake --build cmake-build-match --target check -j2
```

Scratch `/dev/shm/w11410/`: `Layout.base.cpp`, `swap.py`, `greedy.py`,
`chunks.py`, `regions.py`, `orig_full.txt`, `ours_full.txt`.
