# `Layout::Layout` C1/C2 — wave 159 / worker w10507
## Fresh adjacent-swap sweep on the w10407 base + three size-exact
## swaps: `field_0x108`/`field_0x128`, `field_0x158`/`field_0x15c`,
## `field_0x286`/`field_0x288`.  Raw **5,739 -> 5,733**, aligned
## **1,978 -> 1,978**, size **7,072 B** (exact).  Gate green.

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original
vaddr `0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Baseline = wave-158 HEAD `cfde1f5f2` (w10407 composition landed): raw
same-address diff **5,739**, aligned instruction diff **1,978**, size 7,072.
Exit: raw **5,733**, aligned **1,978**, size 7,072.
Gate on exit: byte_exact 4507/4524, linked_exact 4507/4524, size
4524/4524, data 843/843, imports 207, lint CLEAN, drift 0, `RATCHET PASS`.

## 1. Method

The 205 top-level statements of the constructor were parsed with a real
mini-parser (if/else chains are single units; `{...}` blocks are units;
otherwise a statement ends at a top-level `;`).  Every variant was
compiled with the exact NDK r18b clang 7.0.2 `-Oz` command from
`build.ninja`, gold-linked and stripped with the real link command, then
measured with both `fdiff` (same-address raw bytes) and `align_diff.py`
(instruction-index alignment).  **Size is read from the `ours` field of
`fdiff` (`sizes <orig> <ours>`), not the first size** — an earlier
harness read the original size and wrongly accepted 6,952 B variants.

## 2. Fresh exhaustive adjacent-swap sweep (204 pairs)

Size-exact strict wins only (size 7,072):

| swap | raw | aligned |
|---|---:|---:|
| baseline | 5,739 | 1,978 |
| `field_0x108` / `field_0x128` (93/94) | 5,737 | 1,978 |
| `field_0x158` / `field_0x15c` (101/102) | 5,738 | 1,978 |
| `field_0x286` / `field_0x288` (189/190) | 5,736 | 1,978 |

All three compose monotonically (no merge penalty):

| composition | raw | aligned | size |
|---|---:|---:|---:|
| A = 108/128 | 5,737 | 1,978 | 7,072 |
| A + C = 158/15c | 5,736 | 1,978 | 7,072 |
| A + C + D = 286/288 (landed) | **5,733** | **1,978** | **7,072** |

Other adjacent swaps that improve raw but not aligned (all size 7,072):
`field_0x128` before the `0x130` if-chain (5,730/1,980),
`field_0x2ac_metricC`/`ringMidFarThreshold` (5,730/1,994).

## 3. Targeted move sweeps (three mission regions)

Remove-statement-i / re-insert-at-j sweeps, size-exact only:

- `0x2c4..0x2d8` (stmts 30..36, full): no size-exact win.  Best raw
  `field_0x2d0`/`field_0x2cc` (5,727) but size 7,048; `field_0x2cc`
  before `field_0x2d8` is size-exact but 5,738/1,981.
- `0x7c..0x94` (stmts 39..46, full): no size-exact win.  `field_0x9c`
  before `field_0x8c` gives raw 5,694 but size 7,032; the `0x80`/`0x7c`
  swap is 5,775/1,977 at size 7,064.
- `0x150..0x1d0` (stmts 98..133, window +/-6): **`field_0x160` moved
  before `field_0x158` is the big raw/aligned win alone** (5,702 /
  1,957) but it shrinks the function to **7,024 B**, so it is not
  landable under the size-exact gate.  `field_0x178` before
  `field_0x164` is 5,731/1,977 at 6,960 B.  Moving `field_0x160` before
  `field_0x164` is size-exact but neutral (5,739/1,978).

The size-exact aligned-only win is `field_0x1c8`/`field_0x1d8`
(5,743 / 1,975) — it trades 4 raw bytes for 3 aligned instructions, so
it is not a strict win and is not landed.

## 4. Landed source order (three adjacent swaps)

No expression, constant or type was edited; only three statement
relocations:

1. `field_0x108` moved after `field_0x128`.
2. `field_0x158` moved after `field_0x15c`.
3. `field_0x286` moved after `field_0x288`.

## 5. Store / pool tables (final build)

| build | size | `vst1.32` | `vld1.64` | `strd` |
|---|---:|---:|---:|---:|
| ORIGINAL | 7,072 | **81** | **86** | **62** |
| final | 7,072 | **70** | **77** | **49** |

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
              bba f44 f66 fb6 1200 1210 1262 126a 1272 12ea 12f2 12fa
              1302 130a 1312 131a 1380 1388 1390 1398 13a0 13a8 13b0
              14b2 14ba 14c2
```

The early store schedule (through `0x2e0`, `0x682`, `0x8c0`, `0xa70..0xaa8`,
`0xb94`, `0xbba`) is unchanged from the w10407 build; the tail stores
from `0xf44` onward are shifted **4 bytes earlier** (w10407 had
`0xf48 0xf6a 0xfba 0x1204 ...`), i.e. the three swaps removed 4 bytes of
code ahead of that point.  The 11 `vst1.32` / 9 `vld1.64` deficit and the
`0xec`, `0x140`, `0x204` group bases remain the structural residue
documented in the w10308 note (device-major `0x04..0x1e0` rewrite).

## 6. Open questions for the next worker

- The original's `0x140..0x1d4` region is a run of **adjacent-pair**
  vector stores `(0x140,0x144) (0x150,0x154) ... (0x1d0,0x1d4)`; our
  source has `0x144/0x184/0x194/0x1a4/0x1b4/0x1c4/0x1d4` scattered far
  from their pair partners.  A full pair-major reorder of the
  `0x140..0x1d4` statements may fix the `0x140` group base, but every
  such move tried so far changes the function size off 7,072.
- `field_0x160` before `field_0x158` is the strongest raw/aligned
  improvement (5,702/1,957) and is only 48 B short of size-exact; a
  compensating move that adds 48 B back would land it.

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10507
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10507/fdiff.py _ZN6LayoutC1Ev        # 7072/7072, diff 5733
python3 /dev/shm/w10507/align_diff.py _ZN6LayoutC1Ev   # 1978
cmake --build cmake-build-match --target check -j6     # RATCHET PASS
```

Scratch `/dev/shm/w10507/`: `split.py` (statement parser), `harness.py`
(compile+link+measure), `sweep_adj3.py` (size-aware adjacent sweep),
`sweep_move.py` (move sweep), `combo2.py`/`combo3.py` (move/swap
composition), `stores.py` (store census), `Layout_acd.cpp` (landed
source), `adj3_results.txt`, `move_150.txt`, `move_2c4.txt`,
`move_7c.txt`.
