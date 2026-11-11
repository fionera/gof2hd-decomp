# `Layout::Layout` C1/C2 — wave 169 / worker w11409

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original
vaddr `0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Entry baseline (wave-168 HEAD `ec3ceb07c`, with the wave-168
`+prof-unpr` per-TU knob): raw **5,656**, aligned **2,283**, 7,072 B.
Exit: raw **5,654**, aligned **2,283**, 7,072 B.  Gate on exit:
`RATCHET PASS`, byte_exact 4507/4524, linked 4507/4524, size 4524/4524,
data 843/843, imports 207, lint CLEAN.

Scope: the first-half store map (fields `0x04..0x60` and the
`0x2c0`/`0x2c4`/`0x98`/`0x2d0` and `0x2b4`/`0x2b8` groupings).  The tail
(`0x1e4` onward) was not touched.

## 1. Landed change (one statement permutation, size-exact)

Three local statement reorderings in the ternary chain, all in the
first half, measured together as one source edit:

| # | change | raw | aligned |
|---|---|---:|---:|
| 0 | baseline | 5,656 | 2,283 |
| 1 | `0x2ac,0x2a4,ringMidFar,0x2a8` (was `0x2a8,0x2ac,ringMidFar,0x2a4`) | **5,654** | **2,283** |
| 2 | `0x114,0x128,0x120,0x238.b` (was `0x120,0x114,0x238.b,0x128`) | (same edit) | |
| 3 | `0x1b4,0x1b8` (was `0x1b8,0x1b4`) | (same edit) | |

The three moves were found by a random-walk search over adjacent swaps
and single-statement moves restricted to statements `1..139`, accepting
only size-7072 states.  Pure statement reordering: no expression,
constant or type edits.  Raw is the byte-identicality ground truth and
this is a clean -2 with aligned unchanged.

## 2. Original first-half store map (reference)

iPadHD block (`0xd1042..0xd126c`, 554 B) store sequence:

```
vst1 0x14 0x04 0x24 0x34 0x44 0x54 0x64 0x2c8 0x7c
strd 0x74  strd 0x2c0  str 0x2d8  str 0xac  strd 0x8c
stmia 0x94  strd 0xa4  vst1 0xc0 0xb0 0xec 0x294 0x2a4
vst1 0x108 0x118  strd 0xdc 0xe4 0xfc  str 0x104
strd 0x2b4  vst1 0x128 0x140 0x150 0x160 0x170 0x180
0x190 0x1a0 0x1b0 0x1c0 0x1d0  strd 0x138  str 0x1e0
```

The nine 4-word groups are `0x14, 0x04, 0x24, 0x34, 0x44, 0x54, 0x64,
0x2c8, 0x7c`; `0x2c0+0x2c4`, `0x2b4+0x2b8`, `0x74+0x78`, `0xdc+0xe0`,
`0xe4+0xe8`, `0xfc+0x100`, `0x138+0x13c` are `strd` pairs, and
`0x94..0xa0` is a 4-store `stmia`.

Ours still differs structurally in this block (568 B vs 554 B):

* groups are `0x14, 0x04, 0x24, 0x38, 0x48, 0x58, 0x68, 0x2c8, 0x7c`
  — the group boundary is one field late because `0x34` (statement 14)
  is emitted as a scalar after the groups;
* `0x2c0` and `0x2c4` are separate `str`s (the original pairs them);
* `0x2b4` and `0x2b8` are separate `str`s (the original pairs them);
* `0x98` and `0x2d0` are stored in the iPadHD/n9 blocks; the original
  stores them in common blocks;
* `0xd0`/`0xd4`/`0xd8` (`centerYBase`/`marqueeWidth`/`progressBarYBase`),
  `0x204`, `0x23c`, `0x2bc` are in our first block but not the
  original's; `0x94`, `0x140`, `0x144` are missing from ours.

The original n9 block (`0xd127e..0xd12fc`) is `strd 0x74`, `stmia
0x2c0` (`0x2c0..0x2cc`), then the seven groups `0x14, 0x04, 0x24, 0x34,
0x44, 0x54, 0x64`.  Ours emits `str 0x34`, `str 0x78`, `str 0x98`,
`str 0x2d0`, `strd 0x2c4`, then groups `0x38, 0x48, 0x58, 0x68`.

## 3. Measured and not landed (all real `link_so`, size checked)

| shape | raw | aligned | size |
|---|---:|---:|---:|
| baseline | 5,656 | 2,283 | 7,072 |
| **landed: three local reorderings** | **5,654** | **2,283** | **7,072** |
| move `0x2d0` after `0x2cc` | 5,660 | 2,277 | 7,072 |
| move `0x98` after `0x2d8` + `0x2d0` after `0x2cc` | 5,803 | 2,289 | 7,072 |
| swap `0x2c8`/`0x2cc`-adjacent (`adj 34`) | 5,656 | 2,283 | 7,072 |
| swap `0x1d4`/`0x1e4` (`adj 138`) | 5,660 | 2,257 | 7,072 |
| move `0x34` before `0x38` | 6,018 | 2,390 | 7,024 |
| move `0x34` before `0x3c` | 6,018 | 2,377 | 7,024 |
| move `0x34` before `0x44` | 5,989 | 2,406 | 7,024 |
| move `0x2b8` after `0x2b4` | 5,893 | 2,367 | 7,024 |
| move `0x2b8` before `0x2b4` | 5,839 | 2,422 | 7,040 |
| move `0x2c0` before `0x2c4` | 6,000 | 2,378 | 7,044 |
| move `0x2c0` after `0x2c4` | 5,992 | 2,348 | 7,056 |
| move `0x2c0` after `0x2cc` | 5,906 | 2,340 | 7,056 |
| move `0x98` after `0x2cc` | 5,819 | 2,314 | 7,024 |
| move `0x2d0` after `0x2d8` | 5,921 | 2,333 | 7,104 |
| move `0x2c0` to position 139 (best of 20..140) | 5,766 | 2,709 | 7,072 |

Every targeted move of `0x2c0` to positions `20..140` is raw-worse at
size 7,072 (best 5,766 at position 139).  The `0x34`-before-`0x38`
grouping change is the shape that reproduces the original's `0x34`,
`0x44`, `0x54`, `0x64` group boundaries, but it costs 48 B of function
size (7,024) and is raw-worse; the 7,072 B size is held by the current
scrambled order.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w11409
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
cmake --build cmake-build-match --target check -j2
```

Scratch `/dev/shm/w11409/`: `harness.py` (statement splitter + build/
measure), `sweep_adj.py` (adjacent-swap sweep of statements 1..140),
`sweep_move.py` (single-statement move sweep), `combo.py` (candidate
move combinations), `walk.py` (random-walk search), `parse_stores2.py`
(base-register store-order parser), `orig_layout.dis` / `ours_layout.dis`.
