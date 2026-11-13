# `Layout::Layout` C1/C2 — wave 191b / worker w13591

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, linked symbol size **7,072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave191b-w13591`, base master `8b619ca66`.

**Result: one size-exact aligned win landed (raw unchanged, aligned −11).**

| | before | after |
|---|---:|---:|
| raw | 5,232 | 5,232 |
| aligned | 2,711 | **2,700** |
| size | 7,072/7,072 | 7,072/7,072 |
| insn | 2,517/2,552 | 2,517/2,552 |

`check` = RATCHET PASS: byte_exact 4508, linked_exact 4508, data 843/843,
imports 207, lint CLEAN, drift 0, `.rel.plt` byte-identical.

## The win

Swap the two standalone stores `field_0x1a0` (0x1a0) and `field_0x198`
(0x198) so the source order is 0x198 then 0x1a0, matching the original
default-path store order (`... 190 194 198 19c 1a0 ...`, see
`layout_ctor/order_default.txt`).  Semantically a no-op (independent fields);
verified with `compare_combos.py` (0 bad combos of 32).  Size stays exactly
7,072 B, instruction count unchanged, aligned diff drops 11.

## Diff-cluster map of the 5,232-byte residual (fresh build of the base)

Contiguous differing-byte clusters (same-address raw diff, gaps ≤ 2 merged),
top 12:

| offset | bytes | address range |
|---|---:|---|
| +0x0b3c | 323 | d1b64–d1ca6 (default arm, fields 0x84..0x120) |
| +0x1798 | 321 | d27c0–d2900 (tail / 0x30c..0x31c + cold blocks) |
| +0x104a | 255 | d2072–d2170 (default arm, fields 0x1a4..0x1e8) |
| +0x141e | 239 | d2446–d2534 (late block 0x24c..0x27c) |
| +0x0ea6 | 237 | d1ece–d1fba (default arm, fields 0x16c..0x17c) |
| +0x114c | 217 | d2174–d224c (default arm, fields 0x198..0x1a4) |
| +0x00ed | 211 | d1115–d11e7 (HD arm) |
| +0x12e3 | 188 | d230b–d23c6 (late block 0x204..0x234) |
| +0x08d1 | 184 | d18f9–d19b0 (default arm, fields 0x140..0x14c) |
| +0x0211 | 167 | d1239–d12df (HD arm) |
| +0x1512 | 137 | d253a–d25c2 (late block 0x280..0x2a0) |
| +0x0848 | 134 | d1870–d18f5 (default arm, 0x130..0x13c) |

The two largest clusters are pure scheduling/register-allocation shifts:
the instruction multisets differ (`it eq` vs `it ne`, `strd` vs scalar
`str.w`, `vst1` vs scalar), not just the order.  The original's
`vst1`/`strd` groups (e.g. `strd ip,ip,[r0,#32]` for 0x20/0x24,
`strd r1,r2,[r0,#304]` for 0x130/0x134) are not reachable by source
statement reordering while keeping the 7,072 B size ridge.

## Negative results this session (measured on the 5,232 base)

All measured with `link_so -j2` + `tools/fn_diff.py`; size-exact unless noted.

* **Adjacent swap of all 228 depth-1 ctor statements**: only the landed
  0x198/0x1a0 swap is a genuine size-exact aligned win (raw unchanged).
  (An apparent 0x229/0x231 swap is semantically wrong — it swaps the
  retina/else values of `orbitInfoTextX` without the conditions.)
* **400 random single-statement moves** (hill-climb on raw then aligned,
  size-exact required): no win; best stayed 5,232/2,711.
* **`field_0x20_top`/`field_0x24`/`buttonInsetX` float arm → int 7**
  (semantically identical for the int fields): 6,160 / 6,358 / 5,812,
  size 6,796 / 6,680 / 7,028 — the `vmov/vcvt/vcvt/vstr` round-trip is
  load-bearing for the 7,072 size ridge.
* **`field_0x130` arm permutations** (5 arms, else last): best non-base
  0,1,3,2,4 = 5,421 / 2,676 / 7,024 (48 short); none size-exact.
* **`field_0x190` arm permutations**: best non-base 0,3,2,1,4 =
  5,564 / 2,879 / 7,100 (28 long); none size-exact.
* **Cast removals**: `field_0x2d0`/`field_0x160`/`field_0x9c` codegen-
  identical; `field_0xa8` 5,380/7,064; `field_0x10c_rowBaseY` 5,382/7,064;
  `field_0x2c8` 5,434/7,064.
* **`field_0x94` moved next to 0x90** (iOS order): 5,932 / 6,992.
  **`field_0x8c` before 0x9c**: 5,970 / 7,000.  Both together:
  5,900 / 6,912.
* **`field_0x24`/`buttonInsetX`/`field_0x20_top` permutations** to get
  the original `strd ip,ip,[r0,#32]`: all worse (5,646–6,153) or
  size-fatal.
* **Early-block 0x40/0x44/0x30/0x3c/0x38/0x2c reorders toward the
  original dynamic order**: 5,648–5,975, size 6,928–7,072; none better.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13591
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
# raw=5232 aligned=2700 size=7072/7072 insn=2517/2552
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13591/`: `clusters.py`,
`alignblocks.py`, `storeseq.py`, `storecount.py`, `typecount.py`,
`strdlist.py`, `poolcmp.py`, `emu2.py` (patched `emu.py`: SO env,
TRACE mode), `cmpcombos.py`, `traceparse.py`, `reorder.py`,
`blockreorder.py`, `armperm.py`, `armperm2.py`, `randsearch.py`,
`adjsearch.py`, `measure.sh`, `Layout.base.cpp`.
