# `Layout::Layout` C1/C2 — wave 153 / worker w10008
## SLP run alignment: two statement moves make the `0xb0/0xc0` and
## `0x150..0x1d0` groups land on the original's 16-byte-aligned offsets.
## Landed: 7,072 B (size exact), raw **5,830 → 5,781** (−49),
## aligned **2,119 → 2,128** (+9).  Gate green (byte_exact 4507).

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original
vaddr `0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Entry = wave-153 HEAD `4417db0b4`: raw same-address diff **5,830**,
aligned instruction diff **2,119**, size 7,072.  Gate on entry and exit:
byte_exact 4507/4524, linked_exact 4507/4524, size 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0, `RATCHET PASS`.

## 1. The two moves

Both are pure statement reorders inside the ctor (no value/type change):

1. `field_0xa0` before `field_0xb0` (swap the two adjacent assignments after
   `field_0xa8`).  `field_0xb0`'s ternary is float-typed (`3 * 1.40625f`), so
   it carries an `fptosi`; putting the plain-int `field_0xa0` store first
   changes the SLP chain boundary and the `0xb0..0xbc` quad now vectorises as
   `[4,14,19,b]` (the original's vector) instead of scalarising `0xb0` and
   grouping at `0xb4/0xc4`.
2. `field_0x12c` moved out of the `0x108..0x13c` run to just after
   `field_0x15c`.  This removes the `0x12c` store from between `0x128` and
   `0x130`, which lets the compiler build one contiguous int run
   `0x150..0x1dc` (9 aligned quads) instead of the baseline's
   `0x148/0x158/0x168` then `0x180..`.

Neither move alone lands on 7,072: `a0`-before-`b0` alone is 7,076,
`0x12c`-after-`0x15c` alone is 7,052; together they are exactly 7,072.

## 2. Store table (iPadHD arm, `vst1.32` base offsets)

```
ORIGINAL                              LANDED
0x14  [1e,0c,1c,07]                   0x14  [1e,0c,1c,07]
0x4   [15,2a,28,34]                   0x4   [15,2a,28,34]
0x24  [07,07,04,2a]                   0x34  [08,09,52,8c]   (order)
0x34  [08,09,52,8c]                   0x24  [07,07,04,2a]   (order)
0x44  [07,0f,07,7d]                   0x44  [07,0f,07,7d]
0x54  [e7,01,55,01,3f,5a]             0x54  [e7,01,55,01,3f,5a]
0x64  [70,236,bf,3f]                  0x64  [70,236,bf,3f]
0x2c8 [3f,7e,3f,70]                   0x2c8 [3f,7e,3f,70]
0x7c  [0c,0e,70,2a]                   0x7c  [0c,0e,70,2a]
0xc0  [09,0b,05,fd]   <-- aligned     0xb0  [04,14,19,0b]   <-- aligned
0xb0  [04,14,19,0b]   <-- aligned     0xc0  [09,0b,05,fd]   <-- aligned
0xec  [38,70,1901,3202]               0x294 [1901,76,10,10]
0x294 [1901,76,10,10]                 0x2a4 [9a,8c,93,2a]
0x2a4 [9a,8c,93,2a]                   0xdc  [float x4]
0x108 [fe,01,02,01]                   0x118 [09,23,15,01]   (order)
0x118 [09,23,15,01]                   0x108 [fe,01,02,01]   (order)
0x128 [01,15,4e,97]                   0x130 [4e,97,42,17]   <-- see §3
0x140 [0f,12,02,bd]   <-- aligned     0x150 [05,46,46,ce]   <-- aligned
0x150 [05,46,46,ce]                   0x160 [44,00,23,b8]   <-- aligned
0x160 [44,00,23,b8]                   0x170 [ae,54,54,d9]   <-- aligned
0x170 [ae,54,54,d9]                   0x180 [17,a3,17,f4]   <-- aligned
0x180 [17,a3,17,f4]                   0x190 [fa,08,10,5a]   <-- aligned
0x190 [fa,08,10,5a]                   0x1a0 [12,16,16,f3]   <-- aligned
0x1a0 [12,16,16,f3]                   0x1b0 [12,2a,2e,01]   <-- aligned
0x1b0 [12,2a,2e,01]                   0x1d0 [07,11,05,0e]   (order)
0x1c0 [5c,1a,28,32]                   0x1c0 [5c,1a,28,32]
0x1d0 [07,11,05,0e]
```

Every vector *content* in the `0x150..0x1dc` run is byte-identical to the
original (the values were already right; only the run base was wrong).
`0xb0/0xc0` are now the original's two vectors at the original aligned
offsets (emission order `b0,c0` vs the original's `c0,b0` remains).

## 3. What is still different

* `0x140`/`0x144` are still assigned in the late shared tail, so the
  `0x140..0x14c` quad is not in the run.  Moving them up (`0x140`/`0x144`
  after `0x13c`) gives the `0x140/0x150/0x160` groups but the function
  shrinks to 7,024 B (and with the `0x12c` move 7,076 B), so it cannot be
  landed without a size companion.  This is the main open question.
* The `0x128..0x134` quad of the original becomes `0x130..0x13c` here: the
  `0x12c` move shifts that chain by two words.  It is the price of the
  contiguous `0x150..0x1dc` run (9 groups fixed vs 1 shifted).
* `0xdc` vs the original `0xec` (float block) and the `0x24/0x34`,
  `0x108/0x118` emission-order swaps are unchanged from the baseline.

## 4. Measurements (real `link_so` + fdiff + align_diff)

| variant | fn size | raw | aligned |
|---|---:|---:|---:|
| HEAD baseline | **7,072** | **5,830** | **2,119** |
| `a0` before `b0` only | 7,076 | — | — |
| `0x12c` after `0x15c` only | 7,052 | — | — |
| **landed: both** | **7,072** | **5,781** | **2,128** |
| `a0`-before-`b0` + `0x140/0x144` after `0x13c` | 7,024 | 5,739 | 2,365 |
| + `0x12c` after `0x15c` | 7,076 | — | — |

The aligned metric rises by 9 because the extra `0x130` quad replaces the
original's `0x128` quad; the raw same-address diff improves by 49 bytes.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10008
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10008/fdiff.py _ZN6LayoutC1Ev        # 7072/7072, diff 5781
python3 /dev/shm/w10008/align_diff.py _ZN6LayoutC1Ev   # 2128
python3 /dev/shm/w10008/stores.py _ZN6LayoutC1Ev cmake-build-match/libgof2hdaa.so
```

Scratch `/dev/shm/w10008/`: `mk.py` (text-op reorder), `sz.py` (fast
object-size check), `sweep.py` + `filter.py` (parallel single-statement
move sweep over the `a0`-before-`b0` base, 59,292 variants; 1,091
size-7,072 hits), `stores.py`, `fdiff.py`, `align_diff.py`.
