# `Layout::Layout` C1/C2 — wave 157 / worker w10308
## SLP group-base fix: `b0/c0` and `150/160/170` now match the original.
## Landed: 7,072 B (size exact), raw **5,822 -> 5,806** (-16),
## aligned **1,984 -> 1,999** (+15).  Gate green.

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original
vaddr `0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Entry = wave-156 HEAD `08575efff`: raw same-address diff **5,822**,
aligned instruction diff **1,984**, size 7,072.
Exit: raw **5,806**, aligned **1,999**, size 7,072.
Gate on exit: byte_exact 4507/4524, linked_exact 4507/4524, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, `RATCHET PASS`.

## 1. Result: group bases

Original `vst1.32` group bases (iPadHD arm), function-relative:

```
ORIG  14 4 24 34 44 54 64 2c8 7c c0 b0 ec 294 2a4 108 118 128
      140 150 160 170 180 190 1a0 1b0 1c0 1d0
OURS  14 4 24 34 44 54 64 2c8 7c b0 c0 294 2a4 dc 118 108 128
      150 160 170 180 190 1a0 1b0 1c0 1d0
```

The `0xb0/0xc0` pair now has the original's bases, and the whole
`0x150..0x1d0` run now starts at `0x150` (was `0x148/0x158/0x168`).
`0xec`, `0x140` and `0x204` remain open.

## 2. Source moves (all pure statement relocations)

1. `field_0xb0` moved from just after `field_0xa8` down to just after
   `oreTextYOffset`, i.e. into the 16-store SLP chunk that starts at
   `field_0xa0` (wave-156 w10208's chunk-boundary fix).
2. `marqueeWidth` moved from before `oreTextYOffset` to after
   `field_0xc0` (the evicted non-chain store; keeps `c0` inside the
   chunk).
3. `field_0xcc` before `field_0xc4` (adjacent swap).
4. `oreLabelYOffset` after `field_0xc8` (adjacent swap; undoes the
   wave-156 c8/oreLabel swap on this new base).
5. `field_0x238_blk16.b` before `field_0x114_strip5c` (adjacent swap).
6. `field_0x14c` before `field_0x13c` (adjacent swap; this is what moves
   the `0x150` group base into place).
7. `rewardBoxY` moved after the `rewardBoxY2` if/else chain.  This is a
   pure statement move and the compiler still reads `rewardBoxY` in the
   else arm; it is the aligned companion (see caveat below).

Moves 3-7 were found by greedy adjacent-statement swap sweeps on the
group-fixed base (each round 204 variants, mini-linked).

## 3. Measurements (real `link_so` + fdiff + align_diff)

| variant | fn size | raw | aligned |
|---|---:|---:|---:|
| wave-156 HEAD | **7,072** | **5,822** | **1,984** |
| b0 move + marquee evict (w10208 shape) | 7,072 | 5,772 | 2,061 |
| + cc/c4 swap | 7,072 | 5,802 | 2,027 |
| + oreLabel/c8 swap | 7,072 | 5,805 | 2,016 |
| + 114/238 swap | 7,072 | 5,783 | 2,007 |
| + 14c/13c swap (no rewardBoxY move) | 7,072 | 5,820 | 2,003 |
| **+ rewardBoxY move (landed)** | **7,072** | **5,806** | **1,999** |

The group fix alone is raw-optimal (5,772) but costs aligned (+77).
The landed shape trades 34 raw bytes for 62 aligned instructions versus
that shape.  No single statement move on the group-fixed base reached
aligned <= 1,984 (best single move 2,041 mini / ~2,020 real).

## 4. Caveat / open questions

* `rewardBoxY` is now assigned after the `rewardBoxY2` chain.  In the
  else arm the source reads `this->rewardBoxY` before the assignment;
  the original source order is not directly recoverable from the merged
  tail.  If a later pass proves the original assigned it before the
  chain, drop move 7 (raw 5,820 / aligned 2,003).
* `0xec`: the original groups the four ints `ec/f0/f4/f8`; ours groups
  the four floats at `0xdc`.  Moving `ringNearMidThreshold` next to
  `field_0xf0` gives the `0xec` group but grows size to 7,108 and
  shifts `0x294/0x2a4` to `0x298/0x2a8`.
* `0x140`: moving `0x140/0x144` up to `0x13c` gives the original's
  `0x140` group but shrinks the function to 7,024 B.
* `0x204`: the shared tail's chain still starts at `0x208`.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10308
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10308/fdiff.py _ZN6LayoutC1Ev        # 7072/7072, diff 5806
python3 /dev/shm/w10308/align_diff.py _ZN6LayoutC1Ev   # 1999
cmake --build cmake-build-match --target check -j6     # RATCHET PASS
```

Scratch `/dev/shm/w10308/`: `base.cpp` (wave-156 source), `vA.cpp`
(group-fix shape), `vA61e.cpp` (landed), `groups2.py`, `split.py`,
`sweep_adj.py`, `sweep_fast.py`, `fdiff.py`, `align_diff.py`.
