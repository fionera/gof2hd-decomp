# `Layout::Layout` C1/C2 — wave 173 / worker w11810

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original
vaddr `0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Baseline (branch `wave173-w11810`, base master `3fafa5b03`): raw **5,640**,
aligned **2,282**, 7,072 B.  **Exit: no code change landed** (best raw at
7,072 B was 5,643, i.e. +3).  The tree is left at the base.

Scope: the first-half store map (`0x04..0x1e0`) and the exact tail shared-block
dispatch (`0x1e4..0x280`, original `d21e2..d2548`), per waves 171/172.

## 1. Region budget (original vs baseline)

Measured with `objdump -M force-thumb` on the checked-in original and on the
baseline `link_so` (regions in original coordinates; `hd_end` = first `b.w`
after the prologue, `merge` = its target, `pop` = function end):

| region | original | baseline | delta |
|---|---:|---:|---:|
| prologue + iPadHD block (`d1028..hd_end`) | 576 | 590 | **+14** |
| iPadHD block alone (`d1042..hd_end`) | **550** | **564** | +14 |
| non-iPadHD paths (`hd_end..merge`) | 3,962 | 4,144 | **+182** |
| tail + T block (`merge..pop`) | 1,566 | 1,558 | -8 |
| code total (`d1028..pop`) | 6,104 | 6,292 | +188 |
| literal pool (`pop..end`) | 966 | 776 | **-190** |
| function size | 7,072 | 7,072 | 0 |

So the baseline buys its size with **+182 B of non-iPadHD code** and **-190 B
of literal pool**.  The iPadHD block is 14 B too wide; the pool is 190 B too
small.  Both are compiler-visible consequences of source order/expression
shape, not of missing stores.

## 2. The exact tail dispatch (reconstructed source)

The wave-172 shape was reproduced as a single replacement for source units
`0x1e4..rewardBoxX` (the current baseline units 131..171).  It is
self-contained below; the T-block units (`0x1f4`, `0x1f0`, `0x144`,
`footerButtonOffset`, `rewardBoxY2`, `rewardBoxY`, `0x1f8`, `0x28c`, `0x140`,
`0x288`, `0x286`) are kept after it.

```cpp
this->field_0x1e4 = Globals::iPadHD ? 5 : (Globals::n9 ? 6 : (Globals::retinaDisplay ? 8 : 4));
this->field_0x1e8 = Globals::retinaDisplay ? 1 : (Globals::iPadHD ? 1 : (Globals::n9 ? 1 : 0));
if (Globals::iPadHD) {
    this->field_0x1fc = 2;
    this->field_0x1ec = 26;
    this->field_0x234 = 1 * 1.40625f;
    this->field_0x238_blk16.a = 50 * 1.40625f;
    this->field_0x27c = 20 * 1.40625f;
    this->field_0x280 = 0;
    this->scrollBarInset = 1 * 1.40625f;
    this->scrollBarHandle = 0.0f;
    this->rewardBoxWidth = 100 * 1.40625f;
    this->rewardBoxY = 55 * 1.40625f;
    this->rewardBoxY2 = 132;
    this->field_0x204 = 9 * 1.40625f;
    this->field_0x208 = 4 * 1.40625f;
    this->field_0x20c = 5 * 1.40625f;
    this->field_0x210 = 5;
    this->field_0x214 = 1;
    this->field_0x218 = 5 * 1.40625f;
    this->orbitInfoTextX = 8;
    this->orbitInfoStationY = 5 * 1.40625f;
    this->orbitInfoSystemY = 20 * 1.40625f;
    this->orbitInfoSecurityY = 49;
    this->quickMenuHeaderLift = 2;
    this->quickMenuIconGap = 1 * 1.40625f;
    this->field_0x238_blk16.b = 15 * 1.40625f;
    this->field_0x238_blk16.c = 7;
    this->field_0x238_blk16.d = 3 * 1.40625f;
    this->field_0x248 = 170 * 1.40625f;
    this->field_0x24c = -2;
    this->field_0x250 = 1 * 1.40625f;
    this->field_0x254 = 5;
    this->field_0x258 = 2;
    this->field_0x25c = 16 * 1.40625f;
    this->field_0x260 = 1 * 1.40625f;
    this->field_0x264 = 300 * 1.40625f;
    this->field_0x268 = 11;
    this->field_0x26c = 66 * 1.40625f;
    this->field_0x270 = 70 * 1.40625f;
    this->field_0x274 = 97 * 1.40625f;
    this->field_0x278 = 421;
    this->rewardBoxHeight = Globals::n9 ? 352 : (Globals::retinaDisplay ? 392 : 196);
    this->rewardBoxX = Globals::n9 ? 75 : (Globals::retinaDisplay ? 100 : 50);
} else {
    if (Globals::n9) {
        this->field_0x1fc = 4;
        this->field_0x1ec = 34;
        this->field_0x234 = 2;
        this->field_0x238_blk16.a = 90;
        this->field_0x27c = 30;
        this->field_0x204 = 16;
        this->field_0x208 = 7;
        this->field_0x20c = 7;
        this->field_0x210 = 6;
        this->field_0x214 = -2;
        this->field_0x218 = 7;
        this->orbitInfoTextX = 10;
        this->orbitInfoStationY = 7;
        this->orbitInfoSystemY = 30;
        this->orbitInfoSecurityY = 52;
        this->quickMenuHeaderLift = 4;
        this->quickMenuIconGap = 2;
        this->field_0x238_blk16.b = 27;
        this->field_0x238_blk16.c = 9;
        this->field_0x238_blk16.d = 5;
        this->field_0x248 = 306;
        this->field_0x24c = -1;
        this->field_0x250 = 2;
        this->field_0x254 = 7;
        this->field_0x258 = 4;
        this->field_0x25c = 28;
        this->field_0x260 = 0;
        this->field_0x264 = 450;
        this->field_0x268 = 12;
        this->field_0x26c = 99;
        this->field_0x270 = 105;
        this->field_0x274 = 145;
        this->field_0x278 = 450;
    } else {
        this->field_0x1fc = Globals::retinaDisplay ? 4 : 2;
        this->field_0x1ec = Globals::retinaDisplay ? 38 : 19;
        this->field_0x234 = Globals::retinaDisplay ? 2 : 1;
        this->field_0x238_blk16.a = Globals::retinaDisplay ? 100 : 50;
        this->field_0x27c = Globals::retinaDisplay ? 40 : 20;
        this->field_0x204 = Globals::retinaDisplay ? 18 : 9;
        this->field_0x208 = Globals::retinaDisplay ? 8 : 4;
        this->field_0x20c = Globals::retinaDisplay ? 10 : 5;
        this->field_0x210 = Globals::retinaDisplay ? 8 : 4;
        this->field_0x214 = Globals::retinaDisplay ? -2 : 1;
        this->field_0x218 = Globals::retinaDisplay ? 10 : 5;
        this->orbitInfoTextX = Globals::retinaDisplay ? 12 : 6;
        this->orbitInfoStationY = Globals::retinaDisplay ? 10 : 5;
        this->orbitInfoSystemY = Globals::retinaDisplay ? 40 : 20;
        this->orbitInfoSecurityY = Globals::retinaDisplay ? 70 : 35;
        this->quickMenuHeaderLift = Globals::retinaDisplay ? 4 : 2;
        this->quickMenuIconGap = Globals::retinaDisplay ? 2 : 1;
        this->field_0x238_blk16.b = Globals::retinaDisplay ? 30 : 15;
        this->field_0x238_blk16.c = Globals::retinaDisplay ? 10 : 5;
        this->field_0x238_blk16.d = Globals::retinaDisplay ? 6 : 3;
        this->field_0x248 = Globals::retinaDisplay ? 340 : 170;
        this->field_0x24c = Globals::retinaDisplay ? -2 : 0;
        this->field_0x250 = Globals::retinaDisplay ? 2 : 1;
        this->field_0x254 = Globals::retinaDisplay ? 8 : 4;
        this->field_0x258 = Globals::retinaDisplay ? 4 : 2;
        this->field_0x25c = Globals::retinaDisplay ? 32 : 16;
        this->field_0x260 = Globals::retinaDisplay ? 0 : 1;
        this->field_0x264 = Globals::retinaDisplay ? 600 : 300;
        this->field_0x268 = Globals::retinaDisplay ? 16 : 8;
        this->field_0x26c = Globals::retinaDisplay ? 132 : 66;
        this->field_0x270 = Globals::retinaDisplay ? 140 : 70;
        this->field_0x274 = Globals::retinaDisplay ? 194 : 97;
        this->field_0x278 = Globals::retinaDisplay ? 600 : 300;
    }
    this->field_0x280 = Globals::n9 ? 4 : (Globals::retinaDisplay ? 5 : 0);
    this->scrollBarHandle = Globals::n9 ? 1 : Globals::retinaDisplay;
    this->scrollBarInset = Globals::n9 ? 2.0f : (Globals::retinaDisplay ? 2.0f : 1.0f);
    if (Globals::n9) {
        this->rewardBoxWidth = 112;
        this->rewardBoxY = 57;
        this->rewardBoxY2 = 111;
    } else if (Globals::iPadLarge) {
        this->rewardBoxWidth = 200;
        this->rewardBoxY = 110;
        this->rewardBoxY2 = (this->rewardBoxY + 17) << Globals::retinaDisplay;
    } else if (Globals::iPad) {
        this->rewardBoxWidth = 100;
        this->rewardBoxY = 55;
        this->rewardBoxY2 = (this->rewardBoxY + 17) << Globals::retinaDisplay;
    } else {
        this->rewardBoxWidth = Globals::retinaDisplay ? 150 : 75;
        this->rewardBoxY = Globals::retinaDisplay ? 76 : 38;
        this->rewardBoxY2 = (this->rewardBoxY + 17) << Globals::retinaDisplay;
    }
    this->rewardBoxHeight = Globals::retinaDisplay ? 392 : 196;
    this->rewardBoxX = Globals::retinaDisplay ? 100 : 50;
}
```

## 3. Measured shapes (real `link_so` + `fn_diff`; size must stay 7,072)

| shape | raw | aligned | size |
|---|---:|---:|---:|
| baseline HEAD | **5,640** | **2,282** | **7,072** |
| exact tail above (T units kept) | 6,045 | 2,714 | 7,064 |
| exact tail + 3 adjacent swaps that hit 7,072 (best) | 5,963 | 2,705 | 7,072 |
| exact tail + T-block unit moves (5 states hit 7,072; best) | 6,145 | 4,243 | 7,072 |
| first half reordered to original iPadHD store order, baseline tail | 5,976 | 4,001 | 6,848 |
| first half original store order + exact tail | 6,556 | 2,946 | 6,420 |
| first half iOS order + exact tail | 6,435 | 3,130 | 6,392 |
| baseline + `0x1cc` if/else -> ternary | 5,643 | 2,288 | 7,072 |
| baseline + unit-0 (`0x04/0x14/0x10`) -> ternaries | 5,651 | 2,280 | 7,072 |
| baseline + `0x34..0x44` permutation (best aligned) | 5,682 | 2,098 | 7,072 |

The exact tail is **8 B short** of 7,072 (wave 172 measured 56 B short on its
own source; the difference is the T-block split).  Every size-7,072
compensation found (adjacent swaps, T-block moves) raises raw by >=323 B
because the tail is still addressed ~190 B later than the original's `d21e2`
(our merge is `d22a6`; the exact tail moves it only to `d229e`).

## 4. Why the first-half order cannot compensate

The original's iPadHD store map (from objdump, expanded to field offsets) is:

```
14 18 1c 20 | 04 08 0c 10 | 24 28 2c 30 | 34 38 3c 40 | 44 48 4c 50
54 58 5c 60 | 64 68 6c 70 | 2c8 2cc 2d0 2d4 | 7c 80 84 88 | 74 78
2c0 2c4 | 2d8 | ac | 8c 90 | 94 98 9c a0 | a4 a8 | c0 c4 c8 cc
b0 b4 b8 bc | ec f0 f4 f8 | 294 298 29c 2a0 | 2a4 2a8 2ac 2b0
108 10c 110 114 | 118 11c 120 124 | d0 d4 d8 | dc e0 | e4 e8 | fc 100
104 | 2b4 2b8 | 128 12c 130 134 | 140..1dc | 138 13c | 1e0
```

Reordering the source to this map gives a 548-byte iPadHD block (original
550) but shrinks the whole body to 6,848 B: the non-iPadHD paths lose ~200 B
because the current hill order's extra code is what pads them.  The original
non-iPadHD code (3,962 B) is **182 B wider** than the baseline's, not narrower,
so the baseline's scrambled order is not simply "wrong" — it is compensating
for non-iPadHD code our reconstruction does not yet have.

The iOS source order (the true field order, per wave 101) is the strongest
structural witness: with the exact tail it compiles to 6,392 B but its literal
pool diff is only **203 B** (vs 466 for the baseline).  It is 680 B short of
7,072, again the missing non-iPadHD branching.

## 5. Conclusion

* The exact tail dispatch is pinned down and is self-contained above.  It is
  8 B short of 7,072 on the current T-block split.
* No first-half or T-block permutation found keeps 7,072 B **and** lowers raw.
  The best size-exact faithful-tail state is raw 5,963 (+323).
* The remaining raw residue is the 182-byte non-iPadHD deficit (and the
  190-byte pool deficit), not the tail stores.  A future pass should
  reconstruct the original's non-iPadHD if/else branching (wave 101: the
  original branches where the iOS build if-converts) before landing the exact
  tail.
* Best near-miss at 7,072 B: `0x1cc` if/else -> ternary, raw 5,643 (+3),
  aligned 2,288 (+6).  Not landed (raw regression).

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w11810
ulimit -v 12000000
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w11810/`: `Layout.base.cpp`
(pristine ctor), `faithful_tail.txt` (the exact tail block above), `units.py`
(statement splitter), `gen.py` (reorder writer), `measure.sh` (build+fn_diff),
`stores2.py` (store-map parser), `regions3.py`/`rawregions.py` (region budget),
`sweep_adj*.py`/`sweep_t.py`/`sweep_first.py`/`try_*.py` (searches).
