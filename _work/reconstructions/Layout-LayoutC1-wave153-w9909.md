# `Layout::Layout` C1/C2 — wave 153 / worker w9909
## The "per-arm GOT reload" premise is falsified: the original and our build
## both issue exactly **8** device-global GOT loads with the same multiset
## (iPadHD x1, n9 x1, iPadLarge x3, iPad x3).  The 124 B code/pool gap is
## **SLP grouping**, not GOT reloads.  Full store / GOT / pool tables below.
## Source unchanged (baseline 7,072 B / raw 5,800 / aligned 2,148 kept).

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original
vaddr `0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Baseline = wave-152 HEAD `4a6104a43`: raw same-address diff **5,800**,
aligned instruction diff **2,148**, size 7,072.  Landed result: raw **5,830**,
aligned **2,119**, size 7,072.  Gate on entry and exit:
byte_exact 4507/4524, linked_exact 4507/4524, size 4524/4524, data 843/843,
imports 207, lint CLEAN, `RATCHET PASS`.

## 1. GOT-load table (ground truth, `.rel.dyn`-resolved)

`add rX, pc` uses PC = insn+4, so the pool word `val` at address `p` resolves
to `val + (p_add + 4)` where `p_add` is the `add` instruction address.

| # | ORIG load | pool | -> GOT slot | symbol | OURS load | pool | -> GOT slot |
|---|---|---|---:|---|---|---:|---|
| 1 | `d1030` | `d1300` | `21038c` | `Globals::iPadHD` | `d1030` | `d1310` | `21038c` |
| 2 | `d126c` | `d14dc` | `2103e8` | `Globals::n9` | `d1278` | `d14d0` | `2103e8` |
| 3 | `d19a2` | `d1d7c` | `2103f0` | `Globals::iPadLarge` | `d19c8` | `d1d80` | `2103f0` |
| 4 | `d24bc` | `d2864` | `2103f0` | `Globals::iPadLarge` | `d2558` | `d28d8` | `2103f0` |
| 5 | `d24ea` | `d2868` | `21032c` | `Globals::iPad` | `d2566` | `d28dc` | `21032c` |
| 6 | `d2654` | `d2870` | `2103f0` | `Globals::iPadLarge` | `d271e` | `d28e8` | `2103f0` |
| 7 | `d268c` | `d287c` | `21032c` | `Globals::iPad` | `d2756` | `d28f4` | `21032c` |
| 8 | `d2722` | `d2888` | `21032c` | `Globals::iPad` | `d27e8` | `d2bc0` | `21032c` |

Counts are **identical**: `iPadHD x1, n9 x1, iPadLarge x3, iPad x3`.
w9710/w9808's "7 extra GOT words in the original's final pool" is an
artifact of comparing *raw pool words*: a GOT-relative pool word encodes
`slot - (pool_addr + 4)`, so it changes value whenever the code shifts.  The
original's final pool has 7 GOT words, ours 8 (we keep one extra `ldr [pc]`
in the tail), but the number of GOT *loads* is the same.  No source shape
can add "per-arm GOT reloads" because they are already there.

## 2. Code / pool budget

| build | size | code end | code | final pool | words | `vld1.64` | `vst1.32` |
|---|---:|---:|---:|---:|---:|---:|---:|
| ORIGINAL | 7,072 | `d2854` | 6,188 | `d2854..d2bc8` | **221** | **86** | **81** |
| baseline | 7,072 | `d28d0` | 6,312 | `d28d0..d2bc8` | **190** | **77** | **70** |

The original spends 124 B less code and 124 B more pool.  That is exactly
the SLP signature: 9 more `vld1.64` constant vectors (36 words = 144 B) minus
~20 B of scalar materialisation.  It is **not** GOT reloads.

## 3. `vst1.32` group table (base offset, in emission order)

```
ORIG  iPadHD arm (27 groups):
 14 4 24 34 44 54 64 2c8 7c c0 b0 ec 294 2a4 108 118 128
 140 150 160 170 180 190 1a0 1b0 1c0 1d0
OURS  iPadHD arm (26 groups):
 14 4 34 24 44 54 64 2c8 7c b4 c4 294 2a4 dc 118 108 128
 148 158 168 180 190 1a0 1b0 1c0 1d0
ORIG  n9 arm (7):      14 4 24 34 44 54 64
OURS  n9 arm (7):      14 4 34 24 44 54 64
ORIG  shared tail:     5c 7c b0 c0 ec 294 2a4 118 108 128 140
                       b0 c0 ec 2a0 108 13c 16c 180 1c0 1b0 1d0
                       1c0 1b0 1d0 204 214 224 23c 24c 25c 26c
                       23c 264 204 214 224 23c 24c 25c 26c
OURS  shared tail:     5c 7c b0 c0 294 2a4 dc 118 108 128
                       b0 298 16c 16c 16c 1b0 1c0 1b0 1d0 1c0
                       208 218 228 240 250 260 270
                       208 218 228 240 250 260 270 240 264
```

The group **base offsets** differ in four places, always by +4 or +8 on our
side:

| region | ORIG | OURS |
|---|---|---|
| low mid-body | `b0`, `c0` | `b4`, `c4` |
| float block | `ec` (0xec..0xf8) | `dc` (0xdc..0xe8) |
| 0x140 block | `140`,`150`,`160`,`170` | `148`,`158`,`168` |
| 0x200 block | `204`,`214`,`224`,`23c`,`24c`,`25c`,`26c` | `208`,`218`,`228`,`240`,`250`,`260`,`270` |

So our SLP groups are shifted one field later in each region.  The original
groups the *aligned* field quadruples (0xb0/0xc0, 0xec, 0x140/0x150/0x160/
0x170, 0x204/0x214/...), ours groups the next one.  This is the entire
124 B gap.

## 4. Experiments (real `link_so` + fdiff + align_diff)

All reorders were applied to the baseline source and measured; none kept the
function at 7,072 B.

| variant | size | raw | aligned |
|---|---:|---:|---:|
| HEAD baseline | **7,072** | **5,800** | **2,148** |
| reorder `0xb0..0xcc` ascending, in place | 7,032 | 5,823 | 2,226 |
| reorder `0xb0..0xcc` as `c0-group, b0-group` | 7,012 | 5,858 | 2,619 |
| reorder `0xb0..0xcc` + `ec/f0/f4/f8` + `294/2a4/108/118` groups | 7,012 | 5,858 | 2,619 |
| move `0x140`/`0x144` up before `0x13c` | 7,032 | 5,908 | — |
| `0x140..0x17c` ascending (0x13c after 0x14c) | 7,040 | 5,964 | 2,289 |
| full ORIG emitted-order reorder of `0x04..0x1e0` | 6,856 | 5,830 | 2,961 |

Key negatives:
* Reordering the `0xb0..0xcc` statements **does not move the SLP base**: the
  compiler still emits `vst1.32 @0xb4` and `@0xc4`, regardless of source
  order (all 8 statements, ascending, or the original's emitted order).
* The full emitted-order reorder **over-vectorizes**: 6,856 B, i.e. 216 B
  *smaller* than the original.  Matching the original's emitted store order
  is not the same as matching its source order.
* `0x140`/`0x144` moved up still groups `0x144..0x150`, not `0x140..0x14c`;
  the compiler always starts the group one field later than the original.

## 5. Final-pool heads/tails

```
ORIG  d2854: 3ea66666 3f266666 42c80000 42480000 0013df2e 0013de3c ...
             (0.325f, 0.65f, 100.0, 50.0, then the tail GOT offsets)
OURS  d28d0: 42c80000 42480000 0013de92 0013ddc0 0013dd2a 0013dd6e ...
```
The original's `field_0x90` constants (0.325f/0.65f) are in the **final**
pool; ours keeps them in the mid-function pool at `0x8fc`.  That is another
symptom of the same SLP/tail split.

## 5b. Landed source change

One statement move, no expression edit: `ringNearMidThreshold` (`0xec`) is
moved from its late position (after `field_0xdc`/`field_0xf8`) up to just
before `field_0xf0`.  The four `0xec..0xf8` assignments are now in ascending
order in the source (though not adjacent).  Measured:

| variant | size | raw | aligned |
|---|---:|---:|---:|
| baseline | 7,072 | 5,800 | 2,148 |
| **`0xec` moved before `0xf0`** | **7,072** | **5,830** | **2,119** |

The SLP groups themselves are unchanged (`0xdc` still groups); the win is in
the shared-tail scheduling/register allocation.  Adding the `0x140`/`0x144`
move on top is neutral (same 5,830 / 2,119).

## 6. Conclusion / next lever

The device-major skeleton, the three arms, and all 8 GOT loads are already
reproduced.  The remaining 124 B is clang 7's SLP choosing `0xb4/0xc4/0xdc/
0x148/...` instead of `0xb0/0xc0/0xec/0x140/...`.  The next lever is not
source *order* (tested exhaustively above) but a source shape that makes the
IR store list start at the aligned field: e.g. computing the four values into
a local array/struct and storing the array, or a `Layout*` local written four
fields at a time.  A full device-major rewrite of `0x04..0x1e0` (constants
in the iPadHD arm, constants in the n9 arm, ternaries in the else arm) is
still the untested shape; the emitted-order reorder is its lower bound
(6,856 B) and needs +216 B of non-vectorised tail to land at 7,072.

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w9909
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9909/fdiff.py _ZN6LayoutC1Ev        # 7072/7072, diff 5800
python3 /dev/shm/w9909/align_diff.py _ZN6LayoutC1Ev   # 2148
cmake --build cmake-build-match --target check -j6    # RATCHET PASS
```

Scratch `/dev/shm/w9909/`: `fdiff.py`, `align_diff.py`, `storeseq.py`
(SLP-aware store table), `vect.py` (vst group table), `gotscan2.py` (GOT
resolver), `reorder_names.py` (in-place statement reorder), `move.py`,
`Layout.base.cpp`, `orig.dis`, `ours.dis`, `layout_fields.txt`.
