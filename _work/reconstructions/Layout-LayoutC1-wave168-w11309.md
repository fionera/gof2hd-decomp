# `Layout::Layout` C1/C2 — wave 168 / worker w11309

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original
vaddr `0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Entry baseline (master `631c26f0b`): raw **5,658**, aligned **2,280**,
7,072 B.  Exit: raw **5,657**, aligned **2,287**, 7,072 B (one landed
statement relocation).  Gate on exit: byte_exact 4507/4524, linked_exact
4507/4524, size 4524/4524, data 843/843, imports 207, lint CLEAN,
`RATCHET PASS`.

Scope: remaining first-half fields `0x00..0x60` (the
`0x2c0`/`0x2c4`/`0x98`/`0x2d0` grouping, the `0x2b8`/`0x2b4` pair, the
n9 shape).  Tail (`0x1e4` onward) not touched.

## 1. Landed change

Move `field_0x12c` from directly after `field_0x2b4` to after
`field_0x11c` (i.e. reorder the source block `0x2b4, 0x12c, 0x110,
0x2b8, 0x11c, 0x118, ...` to `0x2b4, 0x110, 0x2b8, 0x11c, 0x12c,
0x118, ...`).  Pure statement relocation, no expression/constant edit.

| shape | raw | aligned | size |
|---|---:|---:|---:|
| baseline | 5,658 | 2,280 | 7,072 |
| **landed: `0x12c` after `0x11c`** | **5,657** | 2,287 | **7,072** |

The single improved byte is at `0xd1ccc` (one byte in the else-block
cascade); instruction count is unchanged (2518/2552).  A single-move
sweep of the ten-statement block `0x2b4..0x114` to every position in the
block finds this as the unique raw optimum (the alternative
`0x2b4,0x110,0x2b8,0x12c,...` also gives 5,657 but aligned 2,289).
Adjacent swaps of every statement in the first half (`0x04..0x114`) find
no further raw improvement (all size-exact candidates are >= 5,658).

## 2. Measured and not landed (real `link_so`, size checked)

Baseline 5,658 / 2,280 / 7,072.  Every candidate below is a real rebuild;
`raw` is same-address byte diff.

| shape | raw | aligned | size |
|---|---:|---:|---:|
| `0x2d0` before `0x2cc` | 5,658 | 2,279 | 7,072 |
| `0x2d0` after `0x2cc` | 5,662 | 2,274 | 7,072 |
| `0x2b4,0x12c,0x2b8,0x110,...` | 5,658 | 2,283 | 7,072 |
| `0x2b4,0x110,0x12c,0x2b8,...` | 5,698 | 2,277 | 7,072 |
| `0x2b8` after `0x2b4` | 5,877 | 2,369 | 7,024 |
| `0x2b8` before `0x2b4` | 5,929 | 2,588 | 7,076 |
| `0x2c0` before `0x2c4` | 6,000 | 2,375 | 7,044 |
| `0x2c0` after `0x2cc` | 5,908 | 2,337 | 7,056 |
| `0x98` after `0x2cc` | 5,887 | 2,381 | 7,056 |
| `0x94` before `0x98` | 5,764 | 2,292 | 7,040 |
| all 24 permutations of `0x2c4/0x98/0x2d0/0x2cc` | >= 5,755 | — | only baseline 7,072 |
| original iPadHD group order `0x34,0x38,0x3c,0x40,0x44` | 5,698 | 2,094 | 7,072 |
| that + `0x2d0` before `0x2cc` | 5,700 | 2,093 | 7,072 |
| that + `0x2b8/0x2b4` pair | 5,697 | 2,103 | 7,072 |

## 3. Root cause of the residual (confirmed, not new)

With the original iPadHD group order (`0x34,0x38,0x3c,0x40,0x44`) the
vector groups become `0x14,0x04,0x24,0x34,0x44,0x54,0x64` and the n9
block does sink `0x74/0x78` as a `strd` — i.e. the n9 `0x74` miss is a
consequence of the wave-166 `0x34`/`0x44` swap, which puts `0x74` inside
the seventh iPadHD vector group (`0x68..0x74`).  But that shape costs
+40 raw (5,698) through literal-pool rotation, so it is not landed.

The n9 block still differs structurally from the original in three
places: it stores `0x98` and `0x2d0` (the original leaves both to the
common block), and it emits `strd 0x2c4/0x2c8` instead of the original
`stmia 0x2c0..0x2cc` (our `0x2c0` assignment is at source line 193, far
from the `0x2c4/0x2c8/0x2cc` group).  Every move that fixes one of these
changes the function size (7,024..7,120), because the else-block
cascade and literal-pool rotation absorb more bytes than the n9 block
saves.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w11309
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
cmake --build cmake-build-match --target check -j2
```
Scratch `/dev/shm/w11309/`: `measure.py`, `h.py` (semantic line-move
helpers), `exp*.py` (experiment batches), `sweep_adj.py` (adjacent-swap
sweep), `sweep_block.py` (single-move sweep of `0x2b4..0x114`),
`regions.py`, `cmp.py`.
