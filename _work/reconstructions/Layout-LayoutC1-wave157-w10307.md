# `Layout::Layout` C1/C2 — wave 157 / worker w10307
## Single statement order that combines the w10208 SLP chunk-boundary fix
## with w10207's aligned win: raw **5,822 -> 5,753**, aligned **1,984 ->
## 1,984**, size **7,072 B** (exact).  Gate green.

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original
vaddr `0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Entry = wave-156 HEAD `08575efff` (w10207 landed): raw same-address diff
**5,822**, aligned instruction diff **1,984**, size 7,072.
Exit: raw **5,753**, aligned **1,984**, size 7,072.
Gate on exit: byte_exact 4507/4524, linked_exact 4507/4524, size
4524/4524, data 843/843, imports 207, lint CLEAN, drift 0, RATCHET PASS.

## 1. What was combined

The two wave-156 Layout workers each won alone:

| file | raw | aligned | size |
|---|---:|---:|---:|
| wave-155 base (`fbccebe4f` = `cf74f4ee6`) | 5,837 | 1,986 | 7,072 |
| w10207 (c8/oreLabelYOffset swap) | 5,822 | **1,984** | 7,072 |
| w10208 (SLP chunk-boundary fix) | **5,792** | 2,010 | 7,072 |
| w10208 + w10207 swap (naive merge) | 5,800 | 2,021 | 7,072 |

The naive merge is worse than either on both metrics: the c8 swap does
**not** compose with the chunk fix.  All 32 subsets of w10208's four
statement moves (`0x54/0x50`, `b0`, `c4`, `marqueeWidth`) plus the c8 swap
were real-linked; none reached raw <= 5,792 and aligned <= 1,984.

## 2. The search that found the order

1. A 16x16 **pair sweep** of the two chunk-fix statements on the wave-155
   base: `field_0xb0` inserted after each statement 69..86, `marqueeWidth`
   inserted after each statement 69..86 (240 real-linked variants).  Best
   with raw <= 5,792: **`b0` after `field_0xc4`, `marqueeWidth` after
   `field_0xc8`** -> raw 5,792, aligned 1,991 (vs w10208's 2,010).
2. Adjacent-swap sweep of the first 100 statements of that base (77
   real-linked variants): swap **`field_0x7c` / `field_0x80_touchMargin`**
   -> raw 5,748, aligned 1,986.
3. Full adjacent-swap sweep of all 260 single-line statements of that base
   (204 real-linked variants): swap **`field_0x13c` / `field_0x14c`** ->
   raw **5,753**, aligned **1,984**.

## 3. Final source order (the four moves vs wave-155)

Only four pure statement relocations; no expression, constant or type was
edited:

1. `field_0x7c` moved after `field_0x80_touchMargin` (swap the adjacent
   pair).
2. `field_0xb0` moved from just after `field_0xa8` to just after
   `field_0xc4`.
3. `marqueeWidth` moved from just before `oreTextYOffset` to just after
   `field_0xc8`.
4. `field_0x13c` moved after `field_0x14c` (swap the adjacent pair).

The resulting `0xb0..0xd4` region is:

```
oreTextYOffset
field_0xbc
ringNearMidThreshold
field_0xcc
field_0xc4
field_0xb0
field_0xc8
marqueeWidth
oreLabelYOffset
field_0xc0
```

`field_0xb0` is inside the same 16-store SLP chunk as
`b4/b8/bc/c0/c4/c8/cc`, so SLP builds the original's `b0..bc` / `c0..cc`
chains (the w10208 chunk-boundary fix).  The `marqueeWidth` eviction is
what keeps `c0` in that chunk.  The w10207 `c8/oreLabelYOffset` swap is
**not** present: with `marqueeWidth` between them, the swap costs raw and
aligned (5,847 / 2,111 / 7,064 B).

## 4. Measurements (real `link_so` + fdiff + align_diff)

| variant | size | raw | aligned |
|---|---:|---:|---:|
| wave-156 HEAD (w10207) | 7,072 | 5,822 | 1,984 |
| w10208 chunk fix | 7,072 | 5,792 | 2,010 |
| w10208 + c8 swap | 7,072 | 5,800 | 2,021 |
| pair (b0 after c4, marquee after c8) | 7,072 | 5,792 | 1,991 |
| + swap 0x7c/0x80_touchMargin | 7,072 | 5,748 | 1,986 |
| **+ swap 0x13c/0x14c (landed)** | **7,072** | **5,753** | **1,984** |

## 5. Store / pool tables (final build)

Instruction counts and `vst1.32` instruction offsets (function-relative,
emission order), measured with `objdump`:

| build | size | `vst1.32` | `vld1.64` | `strd` |
|---|---:|---:|---:|---:|
| ORIGINAL | 7,072 | **81** | **86** | 62 |
| final | 7,072 | **70** | **77** | 47 |

```
ORIG vst1.32: 7c 88 96 a4 b2 c0 ce dc ea 130 13e 14c 15a 168 176 184
              1c0 1ce 1d6 1de 1e6 1ee 1f6 1fe 206 20e 216 2a2 2a8 2b0
              2b8 2c0 2c8 2d0 678 8a4 a50 a58 a60 a68 a70 a78 a80 a88
              a90 be0 bf4 c04 c0c c14 c1c c24 cfe e52 ec4 111c 1128
              1130 11a6 11ae 11b6 1256 125e 1266 126e 1276 127e 1286
              1358 1360 1368 13a0 13a8 13b0 143c 1444 144c 1454 145c
              1464 146c
OURS vst1.32: 78 84 92 a0 ae bc ca d8 e6 12a 138 146 154 162 1ae 1c0
              1ce 1d6 1de 1e6 1ee 1f6 1fe 206 23a 23e 2b2 2b8 2c0 2c8
              2d0 2d8 2e0 682 8c0 a70 a78 a80 a88 a90 a98 aa0 aa8 b94
              bba f44 f66 fb6 1200 1210 1262 126a 1272 12ee 12f6 12fe
              1306 130e 1316 131e 1382 138a 1392 139a 13a2 13aa 13b2
              14b4 14bc 14c4
```

`vst1.32`/`vld1.64`/`strd` counts are unchanged from the w10208 build
(the four moves only re-schedule the same stores).  The remaining 11
`vst1.32` / 9 `vld1.64` deficit and the 124 B code/pool gap are the same
structural residue documented in the w10208 note (0xec, 0x140, 0x204
group bases; device-major `0x04..0x1e0` rewrite).

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10307
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10307/fdiff.py _ZN6LayoutC1Ev        # 7072/7072, diff 5753
python3 /dev/shm/w10307/align_diff.py _ZN6LayoutC1Ev   # 1984
cmake --build cmake-build-match --target check -j6     # RATCHET PASS
```

Scratch `/dev/shm/w10307/`: `harness.py`/`harness2.py` (subset sweeps),
`sweep_pair.py` (b0/marquee 16x16 pair sweep), `sweep_adj2.py` (adjacent
swaps), `measure.py`, `stores.py`, `align_detail.py`, `Layout.w10208.cpp`
(the w10208 worker's file, recovered from dangling commit `d39453c6c`),
`Layout.v8384.cpp`, `Layout.v8384_s59.cpp`, `Layout.final.cpp`.
