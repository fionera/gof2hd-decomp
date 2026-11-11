# `Layout::Layout` C1/C2 — wave 170 / worker w11609

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original
vaddr `0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Baseline (master `50716a763`): raw **5,640**, aligned **2,282**, 7,072 B.
Exit: raw **5,682**, aligned **2,102**, 7,072 B.

Scope: the first-half store map (`0x04..0x6c`), the
`0x2c0`/`0x2c4`/`0x98`/`0x2d0` grouping, the `0x2b4`/`0x2b8` pair and the
n9 shape.  Tail (`0x1e4` onward) untouched.

## Landed change (one block reorder, size-exact, aligned -180)

The four statements `0x44, 0x3c, 0x38, 0x34` (source order) are reordered
to ascending `0x34, 0x38, 0x3c, 0x44`.  Pure statement reordering; no
expression, constant or type edit.

| # | change | raw | aligned | size |
|---|---|---:|---:|---:|
| 0 | baseline | 5,640 | 2,282 | 7,072 |
| 1 | **landed: ascending `0x34,0x38,0x3c,0x44`** | **5,682** | **2,102** | **7,072** |

Raw grows by 42 bytes (the first-half block length changes, so every
later literal-pool address shifts); the aligned instruction diff drops by
**180** and the SLP store grouping now matches the original exactly:

```
original iPadHD: vst1 0x34,0x38  0x44,0x48  0x54,0x58  0x64,0x68
ours (landed)  : vst1 0x34,0x38  0x44,0x48  0x54,0x58  0x64,0x68
ours (baseline): vst1 0x38,0x3c  0x48,0x4c  0x58,0x5c  0x68,0x6c
```

The landed order also reproduces the original's first eleven iPadHD
stores verbatim (`0x14,0x04,0x24,0x34,0x44,0x54,0x64,0x2c8,0x7c,0x74`),
where the baseline diverged at the fourth group.  Gate on exit:
`RATCHET PASS`, byte_exact 4507/4524, linked 4507/4524, size 4524/4524,
data 843/843, imports 207, lint CLEAN.

## Original first-half store map (objdump `0xd1042..0xd1268`)

iPadHD block (550 B):
```
vst1 0x14 0x04 0x24 0x34 0x44 0x54 0x64 0x2c8 0x7c
strd 0x74  strd 0x2c0  str 0x2d8  str 0xac  strd 0x8c
stm 0x94 {0x94,0x98,0x9c,0xa0}  strd 0xa4
vst1 0xc0 0xb0 0xec 0x294 0x2a4 0x108 0x118
stm 0xd0 {0xd0,0xd4,0xd8}  strd 0xdc  strd 0xe4  strd 0xfc
str 0x104  strd 0x2b4  vst1 0x128 0x140..0x1d0  strd 0x138  str 0x1e0
```
n9 block (`0xd127e..0xd12fc`):
```
strd 0x74 (207,473)
stmia 0x2c0 { -3,108,45,106 }     ; 0x2c0..0x2cc contiguous
vst1 0x14 0x04 0x24 0x34 0x44 0x54 0x64
b common
```
else block (`0xd1570..0xd185e`, the linear witness of source order):
```
0x04,0x08 | 0x0c,0x10,0x14 | 0x18,0x1c | 0x20,0x24 | 0x28
0x30 0x2c 0x38 0x34 0x3c 0x44 0x40 0x4c 0x48 0x54 0x50 0x58
0x5c 0x60 0x64 0x68 0x6c 0x2c0 0x70 0x74,0x78 0x2c4 0x2c8 0x2cc
```
common block (`0xd1860..d1abe`, straight-line prefix):
```
0x2d0 | strd 0x2d4,0x2d8 | vst1 0x7c | strd 0x7c,0x80 | strd 0x84,0x88
vstr 0x8c | vstr 0x90 | str 0x94 | str 0x98 | vstr 0x9c | str 0xac
vstr 0xa8 | strd 0xa0,0xa4 | strd 0xd0,0xd4 | ...
```

## Measured and not landed (real `link_so`, size checked)

Every single-statement relocation of `0x2c4`(u29), `0x98`(u30),
`0x2c8`(u32), `0x2d0`(u33), `0x2cc`(u34), `0x2d8`(u35), `0x2b4`(u81),
`0x2b8`(u83) and `0x2c0`(u131) to all 145 positions was measured.

| shape | raw | aligned | size |
|---|---:|---:|---:|
| baseline | 5,640 | 2,282 | 7,072 |
| **landed ascending `0x34..0x44`** | **5,682** | **2,102** | **7,072** |
| `0x2d0` before `t_field_0x2c8` | 5,640 | 2,283 | 7,072 |
| `0x2d8` before the `0x2d4` chain | 5,640 | 2,282 | 7,072 (codegen-identical) |
| `0x2d0` after `0x2cc` (faithful common order) | 5,644 | 2,277 | 7,072 |
| `0x2c0` before `0x2c4` | 5,909 | 2,752 | 7,024 |
| `0x2c0` after `0x2c4` | 5,976 | 2,352 | 7,056 |
| `0x2b8` before `0x2b4` | 5,877 | 2,425 | 7,040 |
| `0x110` moved out so `0x2b4,0x2b8` pair | 5,884 | 2,389 | 7,024 |
| full ascending `0x2c..0x78` (first 29 fields) | 5,965 | 2,282 | 7,040 |
| full original else-block order (first 34 units) | 6,135 | 3,930 | 7,056 |
| ascending `0x34..0x6c` + `0x2c0` before `0x2c4` | 6,071 | 2,380 | 7,044 |
| ascending `0x34..0x6c` + `0x2c0` after `0x2c4` | 6,001 | 2,271 | 7,056 |
| ascending `0x34..0x6c` + `0x2d0` after `0x2cc` | 5,685 | 2,097 | 7,072 |

Every permutation of `{0x3c,0x38,0x34}` in place is size-negative (7,024)
except the identity and `0x38,0x3c,0x34` (5,656/2,296/7,072).  The
`0x2c0`/`0x2c4` pair and the `0x2b4`/`0x2b8` pair can only be formed by
shrinking the function below 7,072; the 7,072 size is held by the current
scrambled order.  A 350-step random walk over size-7,072 states found no
raw improvement.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w11609
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
cmake --build cmake-build-match --target check -j2
```

Scratch `/dev/shm/w11609/`: `harness.py` (build+measure), `units.py` /
`reorder.py` (statement-unit splitter/renderer), `sweep2.py`/`sweep3.py`
(single-relocation sweeps), `perm.py`/`perm_multi.py` (in-place
permutations), `walk.py` (random walk), `stores2.py` (base-register store
order parser), `rawdiff.py`/`aldiff.py` (diff region tools).
