# `Layout::Layout` C1/C2 — wave 171 / worker w11709

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original
vaddr `0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Baseline (master `41ef4c8b6`): raw **5,640**, aligned **2,282**, 7,072 B.

Scope: first-half store map (`0x00..0x60`), the
`0x2c0`/`0x2c4`/`0x98`/`0x2d0` grouping, the `0x2b4`/`0x2b8` pair and the
n9 shape.  Tail (`0x1e4` onward) untouched.  No code change landed: the
baseline is a strong local optimum for raw bytes and every measured
candidate was raw-worse or size-negative.

## Why the baseline is hard to beat

The iPadHD block runs `0xd1042..0xd1268` in the original = **550 B**
(offset 576 from the symbol base).  Ours runs `0xd1042..0xd1276` =
**564 B** (offset 590), i.e. **+14 B**.  The 14-byte excess shifts every
later instruction and literal-pool entry, so raw diff is ~80 % of the
whole body even though the aligned diff is only 2,282 instructions.
Fixing the block length is the only lever with a large raw payoff, and
the 7,072 size is held only by the current scrambled statement order.

Original iPadHD store order (objdump `0xd1042..0xd1268`):
```
vst1 0x14 0x04 0x24 0x34 0x44 0x54 0x64 0x2c8 0x7c
strd 0x74  strd 0x2c0  str 0x2d8  str 0xac  strd 0x8c
stm 0x94 {0x94,0x98,0x9c,0xa0}  strd 0xa4
vst1 0xc0 0xb0 0xec 0x294 0x2a4 0x108 0x118
stm 0xd0 {0xd0,0xd4,0xd8}  strd 0xdc  strd 0xe4  strd 0xfc
str 0x104  strd 0x2b4  vst1 0x128 0x140..0x1d0  strd 0x138  str 0x1e0
```
Ours (baseline):
```
vst1 0x14 0x04 0x24 0x38 0x48 0x58 0x68 0x2c8 0x7c
str 0x34  str 0x78  str 0x2c4  str 0x98  str 0x2d8  strd 0x8c
str 0x1e0  str 0x9c  str 0xac
vst1 0xb0 0xc0 0x294 0x2a4 0xdc  str 0xa0  str 0xa8  str 0xa4
strd 0xd0  str 0xd8  strd 0xec  strd 0xf4  strd 0xfc  str 0x104
str 0x2b4  vst1 0x118 0x128 0x140..0x1b0
str 0x2b8  str 0x23c  strd 0x138  strd 0x148  str 0x204  str 0x2bc
str 0x2c0  vst1 0x1c0 0x1d0
```
The original merges 27 vector stores, 9 `strd` and 2 `stmia`; ours has
26 vector stores, 7 `strd`, 0 `stmia` and 17 `str.w`.  The extra
`vst1 0x1c0/0x1d0` pair alone is 8 B of the 14-byte excess.

## Measured (real `link_so`, size checked)

| shape | raw | aligned | size |
|---|---:|---:|---:|
| baseline | **5,640** | 2,282 | 7,072 |
| ascending `0x34,0x38,0x3c,0x44` (w11609) | 5,682 | 2,102 | 7,072 |
| `0x2d0` after `0x2cc` (n9 4-store `stmia`) | 5,644 | 2,277 | 7,072 |
| ascending `0x34..0x44` + `0x2d0` after `0x2cc` | 5,685 | 2,097 | 7,072 |
| `0x2c0` after `0x2c4` + `0x2d0` after `0x2cc` | 6,015 | 2,357 | 7,064 |
| `0x2c0` before `0x2c4` + `0x2d0` after `0x2cc` | 5,967 | 2,373 | 7,044 |
| `0x98` after `0x2d8` + `0x2d0` after `0x2cc` | 5,830 | 2,387 | 7,080 |
| ascending `0x34,0x38,0x3c,0x40,0x44` | 5,838 | 2,168 | 7,088 |
| ascending `0x34..0x6c` (full) | 5,838 | 2,168 | 7,088 |
| full original else-block order, first 43 units | 6,057 | 2,531 | 7,040 |
| ascending `0x140..0x1dc` tail | 5,939 | 2,367 | 6,952 |

Single-relocation sweep of units `1..8` (`0x10,0x1c,0xc,0x14,0x18,0x24,
0x28,0x40`) to every position `1..43`: 20 size-7,072 states, all raw
`>= 5,640`; the only non-590 block length was `0x1c` before `0x2cc`
(raw 6,002, blen 584).  A 150-step random multi-move search over the
first-half and high-field units produced **no** size-7,072 state at all
(random moves shrink the body to 6,780..7,060), confirming that the
7,072 size is held by the exact baseline order.

## Conclusion

Every faithful grouping (`0x2c0`/`0x2c4` `strd`, `0x2b4`/`0x2b8`
`strd`, `0x94..0xa0` `stmia`, n9 4-store `stmia`, ascending
`0x34..0x44`) is size-negative or raw-worse in isolation; the original
reaches 7,072 with those merges because it contains code our
reconstruction does not yet have.  Reaching the original's 550-byte
iPadHD block therefore needs a compensating tail/size change, which is
outside this pass's scope.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w11709
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
```
Scratch `/home/debian/GalaxyOnFire2/scratch/w11709/`: `units.py`
(statement splitter), `harness.py` (build+measure+block length),
`mover.py`/`sweep_first.py`/`search.py` (relocation sweeps),
`cmp2.py`/`stores.py`/`aligndiff.py`/`rawregions.py` (diff tools).
