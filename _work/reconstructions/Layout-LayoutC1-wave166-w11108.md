# `Layout::Layout` C1/C2 — wave 166 / worker w11108

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original
vaddr `0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Entry baseline (wave-164/165 HEAD `85c1f88df`): raw **5,688**, aligned
**2,108**, 7,072 B.  Exit: raw **5,661**, aligned **2,268**, 7,072 B.
Gate on exit: byte_exact 4507/4524, linked_exact 4507/4524, size
4524/4524, data 843/843, imports 207, lint CLEAN, `RATCHET PASS`.

Scope: the first half of the constructor (fields `0x04..0x78`, the
iPadHD/n9/else dispatch).  The tail (`0x1e4` onward) was wave-164/165
territory; not touched here.

## 1. Landed changes (two statement relocations, both size-exact)

| # | change | raw | aligned |
|---|---|---:|---:|
| 0 | baseline | 5,688 | 2,108 |
| 1 | move `field_0x2c_rowHeight` before `field_0x34` | **5,687** | 2,105 |
| 2 | swap `field_0x34` and `field_0x44` (0x34 goes after 0x44) | **5,661** | 2,268 |

Both are pure statement reorderings (no expression/constant edits).  The
second costs aligned instructions (+163) and +2 instructions (2516 ->
2518), but raw is the byte-identicality ground truth and it is a clean
-26 raw.  After change 2, a full pairwise-swap sweep of the first 29
statements and a single-move sweep of statements 1..15 to every position
1..28 find **no further raw improvement** (all size-exact candidates are
>= 5,661).

Resulting first-half source order (fields):
`0x04/0x14 chain, 0x10, 0x1c, 0x0c, 0x18, 0x08, 0x24, 0x28, 0x40, 0x20,
0x30, 0x2c, 0x44, 0x3c, 0x38, 0x34, 0x48, 0x4c, 0x50, 0x54, ...`

## 2. Per-field store map, original iPadHD block (`0xd1042..0xd126c`)

Nine `vld1.64`/`vst1.32` 4-field groups, then scalar/paired stores.  The
group order is **not** ascending: the `0x14` group is emitted first, the
`0x04` group second.

| # | group / store | values |
|---|---|---|
| 1 | 0x14 0x18 0x1c 0x20 | 30, 12, 28, 7 |
| 2 | 0x04 0x08 0x0c 0x10 | 21, 42, 40, 52 |
| 3 | 0x24 0x28 0x2c 0x30 | 7, 7, 4, 42 |
| 4 | 0x34 0x38 0x3c 0x40 | 8, 9, 82, 140 |
| 5 | 0x44 0x48 0x4c 0x50 | 7, 15, 7, 125 |
| 6 | 0x54 0x58 0x5c 0x60 | 487, 341, 63, 90 |
| 7 | 0x64 0x68 0x6c 0x70 | 112, 566, 191, 63 |
| 8 | 0x2c8 0x2cc 0x2d0 0x2d4 | 63, 126, 63, 112 |
| 9 | 0x7c 0x80 0x84 0x88 | 12, 14, 112, 42 |

then, in order:

```
0x74,0x78 = 161,369          (strd)
0x2c0,0x2c4 = -1,126         (strd)
0x2d8 = 141
0xac = 85
0x8c,0x90 = 45, 0.46222222f  (strd)
0x94,0x98,0x9c,0xa0 = 14,590,70,35   (stmia.w r4,{r5,r8,r12,lr})
0xa4,0xa8 = 7,85             (strd)
0xc0..0xcc = 9,11,5,253      (vst1)
0xb0..0xbc = 4,20,25,11      (vst1)
0xec..0xf8 = 56,112,281,562  (vst1)
0x294..0x2a0 = 281,562,281,118 (vst1)
0x2a4..0x2b0 = 154,140,147,42  (vst1)
0x108..0x114 = -6,8,16,90    (vst1)
0x118..0x124 = 18,22,22,243  (vst1)
0xd0,0xd4,0xd8 = 21,28,52    (stmia.w r11,{r3,r6,r8})
0xdc,0xe0 = 14.0625f,1.40625f (strd)
0xe4,0xe8 = 28.125f,1.40625f  (strd)
0xfc,0x100 = 7,2             (strd)
0x104 = 5
0x2b4,0x2b8 = 35,9           (strd)
0x128..0x134 = 1,21,78,151   (vst1)
0x140..0x14c = 15,18,2,189   (vst1)
0x150..0x15c = 5,70,70,206   (vst1)
0x160..0x16c = 68,0,35,184   (vst1)
0x170..0x17c = 174,84,84,217 (vst1)
0x180..0x18c = 23,163,23,244 (vst1)
0x190..0x19c = -6,8,16,90    (vst1)
0x1a0..0x1ac = 18,22,22,243  (vst1)
0x1b0..0x1bc = 18,42,46,1    (vst1)
0x1c0..0x1cc = 92,26,40,50   (vst1)
0x1d0..0x1dc = 7,17,5,14     (vst1)
0x138,0x13c = 66,23          (strd)
0x1e0 = 1.40625f
```

Key shape facts:

* The first six groups (`0x04..0x60`) are **already reproduced exactly**
  in our build, in the same group order and with the same values.  The
  residual in that range is register allocation (`d0-d1` vs `d2-d3` for
  the per-group reload) and literal-pool placement, not store order.
* `0x94..0xa0` is a 4-store `stmia` (values already in GPRs), not a
  vector group; `0x2c0+0x2c4`, `0xa4+0xa8`, `0xdc+0xe0`, `0xe4+0xe8`,
  `0xfc+0x100`, `0x2b4+0x2b8`, `0x138+0x13c` are `strd` pairs.
* The original's early `vld1` hoist order is: `0xc0`, `0x128`, `0x140`,
  `0x150`, `0x160`, `0x170`, `0x180`, `0x190`, `0x1a0`, `0x14`.  Our
  build hoists `0x04` first and `0x14` last, so the literal pool is
  rotated by one 16-byte entry; this is the dominant raw-diff source in
  the block (the pool is ~600 B and every `add rX,pc,#imm` shifts).
* Our iPadHD block is 568 B vs the original's 554 B (+14).  The extra
  stores are `0x98`, `0x9c`, `0x2b8`, `0x23c`, `0x148`, `0x204`,
  `0x2bc`, `0x2c0`, which the original emits in the common/tail blocks
  instead.  Their relocation is not size-exact (see §4).

## 3. Original n9 block (`0xd1280..0xd12fc`)

```
0x74,0x78 = 207,473                  (strd)
0x2c0,0x2c4,0x2c8,0x2cc = -3,108,45,106  (stmia.w r12!,{r1,r2,r3,r6})
0x14 0x18 0x1c 0x20 = 37,11,30,7     (vst1)
0x04 0x08 0x0c 0x10 = 22,45,51,58    (vst1)
0x24 0x28 0x2c 0x30 = 7,9,6,51       (vst1)
0x34 0x38 0x3c 0x40 = 4,12,106,180   (vst1)
0x44 0x48 0x4c 0x50 = 9,16,10,160    (vst1)
0x54 0x58 0x5c 0x60 = 520,364,51,79  (vst1)
0x64 0x68 0x6c 0x70 = 108,545,170,51 (vst1)
b 0xd1860
```

The n9 block does **not** set `0x98` or `0x2d0`; those live in the
common block.  Ours sets `0x98` and `0x2d0` in the n9 block and leaves
`0x2c0`/`0x2cc` to the common block (0x2c0 is statement 134, far down
the source).  Moving `0x2c0` next to `0x2c4` and/or `0x98`/`0x2d0` out
of the n9 block is not size-exact (§4).

## 4. Original else block, first fields (`0xd1570..`)

```
0x04,0x08 = retina?30:15, retina?60:30       (strd)
0x0c,0x10,0x14 = retina?58:29, retina?74:37, retina?50:22  (stmia 3)
0x18,0x1c = retina?14:9, retina?40:20        (strd)
0x20,0x24 = retina?10:5, retina?10:5         (strd)
0x28 = retina?10:5
0x30 = retina?60:30
0x2c = retina?6:3
0x38 = retina?14:7
0x34 = retina?12:6
0x3c = retina?118:59
0x44 = retina?10:5
0x40 = retina?200:100
0x4c = retina?10:5
0x48 = retina?22:11
0x54 = retina?694:347
0x50 = retina?178:89
0x58 = retina?486:243
...
```

The else block's first 22 stores are ascending `0x04..0x58` with
adjacent-pair swaps (`0x30` before `0x2c`, `0x38` before `0x34`, `0x44`
before `0x40`, `0x4c` before `0x48`, `0x54` before `0x50`).  Our else
grouping for `0x04..0x1c` is `strd 0x04/0x08`, `strd 0x0c/0x10`,
`strd 0x14/0x18`, `str 0x1c` — the original merges `0x0c/0x10/0x14`
into one `stmia` and pairs `0x18/0x1c`.  Reordering the source to the
original ascending order shrinks the function to ~7,012 B (wave-164
w11005 §7), so the scrambled source order is what holds 7,072 B.

## 5. Measured and not landed (all real `link_so`, size checked)

| shape | raw | aligned | size |
|---|---:|---:|---:|
| baseline (wave-164/165) | 5,688 | 2,108 | 7,072 |
| **landed: 0x2c before 0x34** | **5,687** | 2,105 | 7,072 |
| **landed: swap 0x34/0x44** | **5,661** | 2,268 | 7,072 |
| move `0x98` after `0x2cc` | 5,989 | 2,357 | 7,040 |
| move `0x2d0` after `0x2cc` | 5,690 | 2,107 | 7,072 |
| move `0x98`+`0x2d0` after `0x2cc` | 5,764 | 2,142 | 7,048 |
| move `0x2c0` next to `0x2c4` | 6,002 | 2,344 | 7,044 |
| move `0x2c0` after `0x2c4` | 5,989 | 2,273 | 7,056 |
| move `0x2c0` after `0x2c8` | 5,972 | 2,300 | 7,088 |
| full original n9 shape (0x2c0 next to 0x2c4, 0x98/0x2d0 out) | 6,052 | 2,465 | 7,088 |
| move `0x2b8` next to `0x2b4` | 5,907 | 2,268 | 7,024 |
| move `0x2b8` before `0x2b4` | 5,907 | 2,402 | 7,076 |
| move `0xc0` before `0xb0` | 5,941 | 2,316 | 7,024 |
| move `0xc0` after `0xb0` | 5,733 | 2,174 | 7,072 |
| move `0xc0` to source head | 5,956 | 2,380 | 7,024 |
| move `0x98` after `0x2d8` | 5,863 | 2,241 | 7,024 |
| move `0x2d0` after `0x2d8` | 5,945 | 2,172 | 7,104 |
| adjacent swap `0x3c`/`0x2c` | 5,688 | 2,096 | 7,072 |
| adjacent swap `0x40`/`0x2c` | 5,688 | 2,098 | 7,072 |
| adjacent swap `0x44`/`0x4c` | 5,688 | 2,109 | 7,072 |

The first-half store order is a strong local optimum: after the two
landed moves, all 378 pairwise swaps of statements 1..28 and all 15*27
single-statement relocations of statements 1..15 are raw-worse or
raw-equal.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w11108
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
cmake --build cmake-build-match --target check -j2
```

Scratch `/dev/shm/w11108/`: `harness.py` (statement splitter + build/
measure), `sweep.py`/`pair.py`/`move.py` (adjacent/pairwise/single-move
sweeps), `perm5.py` (120-permutation sweep of 0x30/0x34/0x3c/0x2c/0x38),
`parse_stores.py` (base-register store-order parser), `orig.dis`/
`ours2.dis` (full disassembly), `regions.py` (raw-diff regions).
