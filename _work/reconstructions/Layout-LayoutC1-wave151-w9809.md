# `Layout::Layout` C1/C2 — wave 151 / worker w9809
## Ascending tail + segment-A `0x2c0` move is the best structural variant
## (raw 5,741→5,712, aligned 2,189→2,341) but is 172 B short; the gap is
## 51 final-pool words (204 B) minus 32 B of extra code, dominated by the
## 9 missing `vld1.64` constant vectors, not per-arm GOT pointers.
## Source unchanged (baseline kept: only size-exact variant).

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Entry = wave-151 HEAD `9d491d46a`: raw same-address diff **5,741**, aligned
instruction diff **2,189**, size 7,072.  Gate on exit: byte_exact 4507/4524,
linked_exact 4507/4524, size 4524/4524, data 843/843, imports 207, lint CLEAN,
`RATCHET PASS`.

## 1. Method / tools

`/dev/shm/w9809/`: `parse.py` (tab-aware objdump parser — the regex parsers in
earlier waves silently ate `add`/`adc` opcodes as hex bytes), `stores2.py`
(SLP-aware store table), `vgroup.py` (vld1→vst1 map), `got3.py` (GOT target
resolver using `.rel.dyn`), `vect.py`, `pool.py`, `fdiff.py`, `align_diff.py`,
`mkasc.py`/`mkasc2.py`/`mkmid.py` (source reorder generators).

GOT map (authoritative, from `.rel.dyn`): `0x21032c=Globals::iPad`,
`0x21038c=iPadHD`, `0x2103e4=retinaDisplay`, `0x2103e8=n9`,
`0x2103ec=iPadLargePossible`, `0x2103f0=iPadLarge`,
`0x2103f4=iPadAssetsWithLowerRes`.

## 2. Original code / pool budget

| build | size | code end | code | final pool | pool words | vld1.64 |
|---|---:|---:|---:|---:|---:|---:|
| ORIGINAL | 7,072 | `d2854` | 6,188 | `d2854..d2bc8` | **221** | **86** |
| baseline | 7,072 | `d28d0` | 6,312 | `d28d0..d2bc8` | **190** | 77 |
| ASC (ascending tail) | 6,868 | `d27cc` | 6,052 | `d27cc..d2afc` | 204 | 69 |
| ASC2 (ASC + `0x2c0`→seg A) | 6,900 | `d2874` | 6,220 | `d2874..d2b1c` | **170** | 71 |
| ASC3 (ASC2 + full mid n9 dup) | 7,232 | — | — | — | — | 73 |
| base+mid n9 dup | 7,260 | — | — | — | — | — |

The original's per-device GOT loads (device globals only) are
**iPadHD×1, n9×1, retinaDisplay×2, iPadLarge×3, iPad×11 = 18**; the 7
GOT-offset words in the final pool (`13df2e,13de3c,13ddb4,13dd96,13dc9a,
13dc04,13db84`) are only 7 of the 51 missing words.  The dominant missing
component is **9 extra `vld1.64` constant vectors** (86 vs 77 in the baseline,
86 vs 71 in ASC2): 9×4 = 36 words, matching w9708's "9 extra vectors" figure.

## 3. Store table — original vs ASC2 (key windows)

Original first `0x164` store is at function offset **`0xdf4`**; ASC2 puts it
at `0xe1c` (+0x28).  Original `0x140/0x144` is a `vst1` pair at `d1ab8`
(second assignment); ASC2 has no mid-body `0x140` store.

`0x2c4..0x2d8` (orig window `0x7a0..0x890`):
```
ORIG   2c4@17e0 2c4@17ee 2c4@180a 2c8@1820 2cc@182c 2cc@183a 2cc@1856
       2d0@1882 2d4/2d8@18b6(strd)
ASC2   2c4@... (3) 2c8/2cc/2d0/2d4/2d8 order differs; 2d4/2d8 still split
```
`0x13c..0x1e8` (orig `0xcec..0x1100`): ASC2 reproduces the ascending order
`13c,140,144,148,14c,150,154,158,15c,160,164(×2),168,16c,...` but the SLP
base tracking shifts the `0x16c` group to `0x16c/0x170/0x174` (orig
`0x16c/0x170/0x174` too) and the `0x180..0x1ac` group is 4 B low.

## 4. Vector-group table (vld1 count per store offset, orig vs ASC2)

```
off   orig asc2        off   orig asc2
0c0   3   2            1b0   3   2
0ec   3   0 (ours 0xdc:2)  1c0   3   1
108   3   2            1d0   3   1
13c   1   0            204   2   0
140   2   0            214   2   0
150   1   0            224   2   0
160   1   0            23c   3   0 (ours 0x240:3)
170   1   0            24c   2   0 (ours 0x250:2)
180   2   0            25c   2   0 (ours 0x260:1)
190   1   0            26c   2   0 (ours 0x270:1)
1a0   1   0            2a0   1   0
```
The tail groups are present but at +4 B (SLP base tracking), and the
`0x1b0/0x1c0/0x1d0` group loses 1 vector each (orig 3/3/3, ASC2 2/1/1).

## 5. Experiments (real `link_so` + fdiff + align_diff)

| variant | size | raw | aligned | 0x164 @ | vld1 |
|---|---:|---:|---:|---:|---:|
| HEAD baseline | **7,072** | **5,741** | **2,189** | `ed8` | 77 |
| ASC: ascending tail (`0x13c..0x1e8`, then `0x204/0x2bc`) | 6,868 | 5,726 | 2,459 | `e08` | 69 |
| **ASC2: ASC + move `0x2c0` into segment A** | 6,900 | **5,712** | **2,341** | `e1c` | 71 |
| ASC2 + duplicate `0x140/0x144` after `0x128` | 6,884 | 5,798 | 2,581 | `e1c` | 72 |
| ASC2 + full mid-body `if(n9)` constant dup | 7,232 | 6,371 | 4,274 | — | 73 |
| baseline + full mid-body `if(n9)` constant dup | 7,260 | 6,310 | 4,164 | — | 81 |

Key negatives:
* The pure ascending reorder **shrinks** the final pool (190→170 words) and
  the vector count (77→69): clang shares/if-converts the constant vectors.
* Moving `0x2c0` into segment A (where the original stores it, `d1122` strd /
  `d12c0` stmia / `d175e` str) is worth **+32 B and −14 raw / −118 aligned**
  over ASC — the single best structural change found.
* A full `if (Globals::n9) {constants} else {ternaries}` around the whole
  mid-body block (`0xf0..0x108`, 34 statements) adds only **+4 vectors**
  (69→73) but **+332 B of code** and wrecks raw/aligned: the original's n9
  mid-body group covers a *subset* of those fields (0xa0,0xb0,0xc0,0xd0,0xd8,
  0xe0,0xe8,0xec,0xf0,0xfc,0x104,0x108,0x118,0x128,0x138,0x140,0x294,0x2a4,
  0x2b4), not all of them.
* Duplicating `0x140/0x144` alone is size-negative and worsens both metrics.

## 6. Conclusion / next lever

ASC2 is the best structural skeleton found (raw 5,712, aligned 2,341) but is
**172 B short**: its code is 32 B *longer* than the original's (6,220 vs
6,188) and its final pool is **51 words (204 B) shorter** (170 vs 221).  The
51 words are dominated by the **9 missing `vld1.64` constant vectors**
(86−71 = 15 vectors in ASC2, but only 51 words because of shared constants),
not by the 18 per-arm GOT pointers (only 7 GOT words are in the final pool).

Next pass: reconstruct the n9 mid-body group with **exactly** the original's
field subset (above) as an explicit `if (Globals::n9)` constant block, not the
whole `0xf0..0x108` range.  That should recover the `0xb0/0xc0/0xec/0x108/0x1b0/
0x1c0/0x1d0` vector copies without the +332 B of scalar stores.  Target:
ASC2 + 15 vectors (60 words) − 9 shared words = 221 words, size 7,072.

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w9809
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9809/fdiff.py _ZN6LayoutC1Ev        # 7072/7072, diff 5741
python3 /dev/shm/w9809/align_diff.py _ZN6LayoutC1Ev   # 2189
python3 /dev/shm/w9809/mkasc2.py /dev/shm/w9809/Layout.asc2.cpp
```
Scratch `/dev/shm/w9809/`: `Layout.base.cpp`, `Layout.asc.cpp`,
`Layout.asc2.cpp` (best), `Layout.asc3.cpp`, `Layout.asc4.cpp`,
`Layout.base_mid.cpp`, `orig.dis`, `ours.dis`, `asc2.dis`, `asc3.dis`,
`astore.txt`, `a2store.txt`, `ovg.txt`, `a2vg.txt`, `ostore.txt`,
`ustore.txt`.
