# `Layout::Layout` C1/C2 — wave 169 / worker w11509

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original
vaddr `0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Entry baseline (master `00c403d5a`): raw **5,650**, aligned **2,284**,
7,072 B.  Exit: raw **5,650**, aligned **2,283**, 7,072 B.  Gate on exit:
`RATCHET PASS`, byte_exact 4507/4524, linked 4507/4524, size 4524/4524,
data 843/843, imports 207, lint CLEAN.

Scope: the first-half store map (`0x04..0x60`, the
`0x2c0`/`0x2c4`/`0x98`/`0x2d0` grouping, the `0x2b4`/`0x2b8` pair, the n9
shape).  The tail (`0x1e4` onward) was not touched.

## 1. Landed change (one statement relocation, size-exact, raw-neutral)

`this->field_0x2d0 = ...` moved from immediately before the
`int t_field_0x2c8` declaration to immediately after
`this->field_0x2c8 = ...`.  Pure statement reordering; no expression,
constant or type edit.

| # | change | raw | aligned | size |
|---|---|---:|---:|---:|
| 0 | baseline | 5,650 | 2,284 | 7,072 |
| 1 | **landed: `0x2d0` after `0x2c8`** | **5,650** | **2,283** | **7,072** |

Raw is unchanged (byte-identicality ground truth not worsened) and the
aligned instruction diff drops by one.  The resulting source order around
the group is `0x2c4, 0x98, t=0x2c8, 0x2c8, 0x2d0, 0x2cc, 0x2d8`.

## 2. First-half store map (objdump of the original, `0xd1028`)

### iPadHD block (`0xd1042..0xd1268`, 554 B; ours `0xd1042..0xd1276`, 568 B)

```
vst1 0x14 0x04 0x24 0x34 0x44 0x54 0x64 0x2c8 0x7c
strd 0x74  strd 0x2c0  str 0x2d8  str 0xac  strd 0x8c
stmia 0x94  strd 0xa4  vst1 0xc0 0xb0 0xec 0x294 0x2a4
vst1 0x108 0x118  strd 0xdc 0xe4 0xfc  str 0x104
strd 0x2b4  vst1 0x128 0x140 0x150 0x160 0x170 0x180
0x190 0x1a0 0x1b0 0x1c0 0x1d0  strd 0x138  str 0x1e0
```

Ours still differs structurally in this block (568 B vs 554 B):
groups are `0x14,0x04,0x24,0x38,0x48,0x58,0x68,0x2c8,0x7c` (boundary one
field late because `0x34` is emitted scalar after the groups); `0x2c0` /
`0x2c4` and `0x2b4` / `0x2b8` are separate `str`s (original pairs them);
`0x98` and `0x2d0` are inside the iPadHD/n9 blocks (original stores them
in the common block); `0x1e0` is sunk early; `0x140` is missing from ours.

### n9 block (`0xd127e..0xd12fc`)

```
strd 0x74 (207,473)
stmia 0x2c0 { -3,108,45,106 }        ; 0x2c0..0x2cc contiguous
vst1 0x14 0x04 0x24 0x34 0x44 0x54 0x64
b common
```

Ours (`0xd128c..0xd130e`): `str 0x34`, `str 0x78`, `str 0x98`,
`str 0x2d0`, `strd 0x2c4 {108,45}`, then groups `0x38,0x48,0x58,0x68`.
The n9 shape therefore needs `0x34` before `0x38` and `0x2c0` adjacent to
`0x2c4` in source order; both are size-negative (see §3).

### else block (`0xd1570..0xd185e`) — the most linear witness of source order

```
0x04,0x08 | 0x0c,0x10,0x14 | 0x18,0x1c | 0x20,0x24 | 0x28
0x30 0x2c 0x38 0x34 0x3c 0x44 0x40 0x4c 0x48 0x54 0x50 0x58
0x5c 0x60 0x64 0x68 0x6c 0x2c0 0x70 0x74,0x78 0x2c4 0x2c8 0x2cc
```

i.e. ascending with adjacent-pair swaps (`0x30/0x2c`, `0x38/0x34`,
`0x44/0x40`, `0x4c/0x48`, `0x54/0x50`), `0x2c0` between `0x6c` and
`0x70`, and `0x2d0/0x2d4/0x2d8` in the common continuation.  Our flat
per-field-ternary source is scrambled relative to this; reordering the
first half to the else-block order shrinks the function to 7,024 B
(§3), so the 7,072 B size is held by the scrambled order.

## 3. Measured and not landed (all real `link_so`, size checked)

| shape | raw | aligned | size |
|---|---:|---:|---:|
| baseline | 5,650 | 2,284 | 7,072 |
| **landed: `0x2d0` after `0x2c8`** | **5,650** | **2,283** | **7,072** |
| `0x2d0` after `0x2cc` | 5,654 | 2,278 | 7,072 |
| `0x2d4` before `0x2d8` (pairs them) | 5,710 | 2,302 | 7,072 |
| `0x2d0` after `0x2cc` + `0x2d4` before `0x2d8` | 5,714 | 2,297 | 7,072 |
| `0x2c0` before `0x2c4` | 6,073 | 2,420 | 7,044 |
| `0x2c0` after `0x2c4` | 5,976 | 2,352 | 7,056 |
| `0x2c0` after `0x2cc` | 5,949 | 2,348 | 7,056 |
| `0x2c0` after `0x2c8` | 6,090 | 2,462 | 7,120 |
| `0x2b8` after `0x2b4` (pairs them) | 5,882 | 2,385 | 7,024 |
| `0x2b8` before `0x2b4` | 5,877 | 2,425 | 7,040 |
| `0x98` after `0x2cc` | 5,900 | 2,388 | 7,048 |
| `0x98` after `0x2d8` | 5,815 | 2,302 | 7,024 |
| `0x2d0` after `0x2d8` | 5,908 | 2,334 | 7,104 |
| `0x34` before `0x38` (original group boundary) | 6,044 | 2,391 | 7,024 |
| full original else-block order (first half) | 6,013 | 3,838 | 7,024 |
| `0x2b8>0x2b4` + `0x2c0>0x2c8` (size-compensated pair) | 6,026 | 2,399 | 7,112 |
| `0x34<0x38` + `0x2c0>0x2c8` | 5,996 | 2,401 | 7,072 |
| `0x2b8<0x2b4` + `0x2d0>0x2d8` | 5,944 | 2,471 | 7,072 |

Every adjacent swap of the first 100 statement units was measured: no
swap improves raw, and the raw-neutral swaps (`units 6/7`, `67/68`,
`73/74`, `75/76`, `76/77`, `87/88`, `107..110`, `124..127`, `134/135`)
are codegen-identical.  The iPadHD block length under adjacent swaps only
takes values 564/568/572/576/578, never the original 554, so the literal
pool cannot be re-aligned by local reordering.  Two random-walk searches
(400+ steps each, single-statement relocations in units 1..100, size-7072
states) found no raw improvement; the only find was the landed
raw-neutral/aligned-1 move.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w11509
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
cmake --build cmake-build-match --target check -j2
```

Scratch `/dev/shm/w11509/`: `h2.py` (line-unit splitter + build/measure),
`h3.py` (adds iPadHD-block length), `run.py` (move/swap driver),
`stores.py` (base-register store-order parser), `adj.py`/`adjblk.py`
(adjacent-swap sweeps), `walk.py` (random walk), `orig_head.dis` /
`ours_head.dis`, `orig_*.stores` / `ours_*.stores`.
