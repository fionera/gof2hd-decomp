# `Layout::Layout` C1/C2 — wave 166 / worker w11109

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original
vaddr `0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Baseline (master `85c1f88df`): raw **5,688**, aligned **2,108**, 7,072 B.
Exit: raw **5,685**, aligned **2,120**, 7,072 B.

## Landed change

`field_0x280`'s 4-way chain was rewritten to put the non-n9 branch first
(the original's block order is iPadHD, else, n9, not iPadHD, n9, else):

```cpp
if (Globals::iPadHD)
    this->field_0x280 = 0;
else if (!Globals::n9)
    this->field_0x280 = Globals::retinaDisplay ? 5 : 0;
else
    this->field_0x280 = 4;
```

This is semantically identical to the old
`iPadHD?0 : n9?4 : retina?5:0` chain but flips the compiler's
`n9`/`else` block order.  Measured `5,688 -> 5,685` raw, aligned
`2,108 -> 2,120`, size exact.  Gate: byte 4507, linked 4507, size
4524/4524, data 843/843, imports 207, lint CLEAN, RATCHET PASS.

The same `!n9` swap on the other tail chains was measured and is worse:
`field_0x20c` 5,786/2,186/7,116; `orbitInfoTextX` 5,786/2,186/7,116;
`orbitInfoStationY` 5,721/2,158/7,088; `field_0x260` neutral
(5,685/2,120/7,072).  Swapping the `rewardBoxY2` if/else and the
`rewardBoxWidth`/`field_0x1fc` ternaries is neutral (compiler
canonicalises them).

## Original tail structure (objdump of `0xd21e2..0xd2800`)

Registers: `sl = iPadHD`, `fp = n9`, `ip = retinaDisplay`, `r9 = !n9`,
`lr = !(n9 || iPadHD)`.

```
d21e2  field_0x1e4 = (int)(float)s0 ; field_0x1e8 = lr ? retina : 1
d2202  if (!iPadHD) goto d22b8
       BLOCK A (iPadHD) d2204..d22b2:
         0x1fc=2, 0x1ec=26, 0x234=1, 0x238.a=70, 0x27c=28, 0x280=0,
         scrollBarInset=1, scrollBarHandle=0, rewardBoxWidth=140,
         rewardBoxY=77, rewardBoxY2=132,
         vst1 pairs (0x204,0x208)(0x214,0x218)(0x224,0x228)
                   (0x23c,0x240)(0x24c,0x250)(0x25c,0x260)(0x26c,0x270)
         if (!n9) goto d24de else goto d2530   ; n9/else tail
d22b8  if (!n9) goto d240e
       BLOCK B (else) d22c0..d240c: retina-selects for
         0x1fc,0x1ec,0x224,0x228,0x22c,0x230,0x234,0x238,0x24c,0x250,
         0x20c,0x218,0x220,0x204,0x208,0x210,0x214,0x21c,0x23c,0x240,
         0x278,0x264,0x268,0x254,0x258,0x25c,0x260,0x274,0x27c,0x280
         b d2498
d240e  BLOCK C (n9) d240e..d2494: 0x1fc=4, 0x1ec=34, 0x234=2,
         0x238.a=90, 0x27c=30 + same seven vst1 pairs
d2498  shared (n9/else): 0x280, 0x3e4 (scrollBarHandle), 0x3e0
       (scrollBarInset), rewardBoxWidth, rewardBoxY, rewardBoxY2
       (from the NEW rewardBoxY); n9 -> 112/57/111,
       iPadLarge -> 200/110, iPad -> 100/55, else retina?150:75 /
       retina?76:38
d24de  n9 tail: 0x3ec=352, 0x3f0=75
d2530  else tail: 0x3ec=retina?392:196, 0x3f0=retina?100:50
d2548  shared: 0x2bc, 0x3fc, 0x200, 0x1f0, 0x1f4, 0x1f8, 0x284,
       0x285, 0x286, 0x288, 0x28c, 0x290
d2654  shared: 0x2f4, 0x2f8, 0x2fc, 0x300, 0x304, 0x308
d2704  shared: 0x30c, 0x310, 0x314, 0x318
d2774  shared: 0x31c, 0x320
```

Original block order is **iPadHD, else, n9**; the current source's
per-field ternaries make clang emit **iPadHD, n9, else**.  The
`field_0x280` rewrite above is the one place where flipping the order
was a raw win.

## Shapes measured and not landed (real `link_so` + `fn_diff`)

| shape | raw | aligned | size |
|---|---:|---:|---:|
| baseline HEAD | 5,688 | 2,108 | 7,072 |
| **landed `field_0x280` `!n9` swap** | **5,685** | **2,120** | **7,072** |
| `field_0x20c` `!n9` swap | 5,786 | 2,186 | 7,116 |
| `orbitInfoTextX` `!n9` swap | 5,786 | 2,186 | 7,116 |
| `orbitInfoStationY` `!n9` swap | 5,721 | 2,158 | 7,088 |
| `field_0x260` `!n9` swap | 5,685 | 2,120 | 7,072 (neutral) |
| `rewardBoxY2` `!n9` swap | 5,685 | 2,120 | 7,072 (neutral) |
| `rewardBoxWidth` `!n9` ternary swap | 5,685 | 2,120 | 7,072 (neutral) |
| `field_0x1fc` `!n9` ternary swap | 5,685 | 2,120 | 7,072 (neutral) |
| `field_0x1e4` `(int)(float)` float constants | 5,803 | 2,448 | 7,088 |
| `field_0x1e4` `(int)(float)` int round-trip | 5,966 | 2,403 | 7,056 |
| `field_0x1e8 = (n9\|\|iPadHD) ? 1 : retina` | 5,868 | 2,295 | 7,024 |
| both of the above | 5,913 | 2,429 | 7,080 |
| `field_0x1e4` before `field_0x1d4` | 5,991 | 2,242 | 7,056 |
| `field_0x234` before `field_0x238.a` | 5,689 | 2,118 | 7,072 |
| `field_0x234` + `field_0x27c` relocated | 5,693 | 2,120 | 7,072 |
| `field_0x1fc`+`0x1ec`+`0x234` before `0x238.a` | 5,696 | 2,153 | 7,072 |
| `field_0x285` after `0x284` | 5,700 | 2,110 | 7,072 |
| whole tail reordered to original store order (ternaries) | 6,186 | 2,551 | 6,744 |
| explicit iPadHD/n9/else blocks for the dispatch | 6,092 | 2,774 | 6,628 |
| reward box explicit if/else + shared tail reorder | 6,007 | 2,411 | 6,928 |
| rewardBoxY before rewardBoxY2 (only) | 6,052 | 2,317 | 7,168 |

Adjacent-swap greedy over both the dispatch region (38 statements) and
the shared-tail region (29 statements) finds **no further raw
improvement**.  The per-field-ternary source is a strong local optimum;
the residual is the compiler's choice of which fields join the iPadHD/n9
blocks (it owns `0x208..0x27c` pairs and `0x200/0x285`, the original owns
`0x204..0x270` pairs plus `0x234/0x27c/0x3e8/0x3f4/0x3f8`).

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w11109
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
cmake --build cmake-build-match --target check -j2
```

Scratch `/dev/shm/w11109/`: `Layout.base.cpp` (baseline), `Layout.win.cpp`
(landed), `mk.py`/`mk2.py` (tail rewrites), `scan.py`/`scan2.py`
(adjacent-swap sweeps), `regions.py` (raw diff regions), `sbs.txt`
(side-by-side verify).
