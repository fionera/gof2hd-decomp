# `Layout::Layout` C1/C2 — wave 167 / worker w11208

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original
vaddr `0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Entry baseline (master `d00057e6d`, wave-166 merge): raw **5,658**,
aligned **2,280**, 7,072 B.  Exit: raw **5,658**, aligned **2,280**,
7,072 B (no landed change; see below).

Scope: remaining first-half fields `0x00..0x60` — the
`0x2c0`/`0x2c4`/`0x98`/`0x2d0` grouping, the `0x2b8`/`0x2b4` pair, and
the n9 shape.  Tail (`0x1e4` onward) not touched.

## 1. First-half store map (re-measured, wave 167)

Original iPadHD block `0xd1042..0xd1268` (554 B), store order:

```
0x14 0x04 0x24 0x34 0x44 0x54 0x64 0x2c8 0x7c   (vst1 groups)
0x74/0x78 strd, 0x2c0/0x2c4 strd, 0x2d8, 0xac, 0x8c/0x90 strd,
0x94..0xa0 stmia, 0xa4/0xa8 strd, 0xc0, 0xb0, 0xec, 0x294, 0x2a4,
0x108, 0x118, 0xd0..0xd8 stmia, 0xdc/0xe0, 0xe4/0xe8, 0xfc/0x100,
0x104, 0x2b4/0x2b8 strd, 0x128, 0x140, ...
```

Original n9 block `0xd127e..0xd12fc` (128 B):

```
0x74/0x78 strd (207,473)
0x2c0..0x2cc stmia (-3,108,45,106)
0x14 0x04 0x24 0x34 0x44 0x54 0x64   (vst1 groups)
b 0xd1860
```

Our build's n9 block `0xd128c..0xd130e` (132 B) differs in every one of
those: it stores `0x34` separately (so the vector groups start at
`0x38`), stores `0x98` and `0x2d0` in the n9 block (the original leaves
them to the common block), emits `strd 0x2c4/0x2c8` instead of the
`0x2c0..0x2cc` stmia, and **does not store `0x74` at all** (only
`0x78`; `0x74` is uninitialised on the n9 path in our build — the
original stores `0x74/0x78` as a strd).

## 2. Measured and not landed (all real `link_so`, size checked)

Baseline 5,658 / 2,280 / 7,072.  Every candidate below was a real
rebuild; `raw` is same-address byte diff.

| shape | raw | aligned | size |
|---|---:|---:|---:|
| move `0x2c0` before `0x2c4` | 6,000 | 2,375 | 7,044 |
| move `0x2c0` after `0x2c4` | 6,000 | 2,347 | 7,064 |
| move `0x2c0` after `0x98` | 5,989 | 2,343 | 7,048 |
| move `0x2c0` after `0x2d0` | 5,916 | 2,339 | 7,056 |
| move `0x2c0` after `t_field_0x2c8` | 5,992 | 2,371 | 7,064 |
| move `0x2c0` after `0x2c8` | 6,010 | 2,403 | 7,064 |
| move `0x2c0` after `0x2cc` | 6,077 | 2,466 | 7,120 |
| move `0x2c0` after `0x2d8` | 6,020 | 2,334 | 7,056 |
| move `0x2c0` before `0x2c4` + `0x2d0` after `0x2cc` | 5,942 | 2,383 | 7,044 |
| move `0x2c0` before `0x2c4` + `0x98` after `0xa0` + `0x2d0` after `0x2cc` | 6,037 | 2,594 | 7,064 |
| move `0x98` after `0xa0` | 5,954 | 2,526 | 7,060 |
| move `0x98` before `0x9c` | 5,885 | 2,463 | 7,060 |
| move `0x98` after `0x9c` | 5,891 | 2,472 | 7,052 |
| move `0x98` after `0x2c8` | 5,909 | 2,386 | 7,040 |
| move `0x98` after `0x2cc` | 5,880 | 2,385 | 7,088 |
| move `0x2d0` after `0x2c8` | 5,822 | 2,381 | 7,080 |
| move `0x2d0` after `0x2cc` | 5,945 | 2,383 | 7,096 |
| move `0x2b8` after `0x2b4` | 5,932 | 2,263 | 7,056 |
| move `0x2b8` before `0x2b4` | 5,939 | 2,437 | 7,068 |
| move `0x2c0` after `0x2c4` + `0x2b8` before `0x2b4` | 5,926 | 2,599 | 7,060 |
| move `0x2c0` after `0x2c4` + `0x98` after `0x2cc` | 6,000 | 2,544 | 7,100 |
| move `0x34` before `0x38` | 6,020 | 2,387 | 7,024 |
| move `0x34` before `0x3c` | 6,020 | 2,374 | 7,024 |
| move `0x34` before `0x44` | 5,991 | 2,403 | 7,024 |
| move `0x40` after `0x3c` | 6,043 | 2,363 | 7,024 |
| `0xc0` group before `0xb8` | 5,858 | 2,308 | 7,056 |
| `0xc0` group before `0xb8` + `0x128` before `0x11c` | 5,832 | 2,307 | 7,056 |
| `0xc0` group at source head | 5,875 | 2,497 | 6,944 |
| `0x74/0x78` at source head | 6,103 | 2,618 | 6,948 |
| full n9 store order for `0x04..0x70` | 6,067 | 3,632 | 7,044 |
| else-block order for `0x04..0x58` | 6,043 | 2,606 | 7,012 |
| combined `0x74/0x78` if/else (identical code) | 5,658 | 2,280 | 7,072 |

Single-move sweeps of units 30..36 to positions 1..60 and units
82..88/132 to positions 1..100 found **no** size-exact raw improvement
(best size-exact was the unchanged 5,658).  The wave-166 conclusion
holds: the scrambled first-half order is a strong local optimum for
raw, and every move that fixes a store-order difference also changes
the literal-pool rotation and/or the else-block select chain, costing
more raw than it saves.

## 3. Root cause of the residual

The first-half code diff is **548 B in `0xd1028..0xd1308`** plus
**177/464 B in the literal pool** (`0xd1308..0xd14d8`); the n9 block
alone is 96/138 B.  The dominant cause is that our literal-pool order
is `0xb0, 0x118, 0x140, 0x150, 0x160, 0x170, 0x180, 0x190, 0x1a0,
0x14, ...` while the original's is `0xc0, 0x128, 0x140, 0x150, 0x160,
0x170, 0x180, 0x190, 0x1a0, 0x14, ...`.  That rotates every
`add rX,pc,#imm` in the block and swaps the d0-d1/d2-d3 roles of the
hoisted group.  The hoist order is a scheduler decision that source
reordering cannot reach without changing the function size.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w11208
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
```

Scratch `/dev/shm/w11208/`: `harness.py` (statement splitter),
`fast.py` (ninja + in-process raw/ELF measurement, ~0.8 s/build),
`sweep.py` (single-move sweeps), `stores.py` (store-order parser),
`orig.dis`/`ours.dis`.
