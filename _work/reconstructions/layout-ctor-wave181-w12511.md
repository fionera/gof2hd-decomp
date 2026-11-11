# `Layout::Layout` C1/C2 — wave 181 / worker w12511

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Baseline: branch `wave181-w12511`, base master `1e270aa0e` (post-wave-180
tree): raw **5,408**, aligned **2,438**, size **7,072/7,072**,
2,514 / 2,552 instructions. `reload()` is untouched (byte-exact).

**Landed: a size-exact raw win.**

| metric | before | after |
|---|---:|---:|
| raw same-address bytes | **5,408** | **5,401** (-7) |
| aligned (difflib) | 2,438 | 2,488 (+50) |
| size | 7,072/7,072 | **7,072/7,072** |
| instructions | 2,514/2,552 | 2,518/2,552 |

The raw metric is the worker objective; aligned is reported honestly and moved
the other way.  `.ARM.extab` content is unchanged (713 differing bytes).

## 1. Winning moves

Three pure statement moves in the constructor tail, no value/type/control-flow
change:

1. `field_0x208` moved **before** `orbitInfoSecurityY` and `quickMenuIconGap`
   (it was between `quickMenuIconGap` and `field_0x210`).  **-5**.
2. `field_0x278` moved **before** `field_0x248`.  **-1**.
3. `footerButtonOffset` moved **before** `scrollBarInset`.  **-1**.

Combined raw **5,408 -> 5,401** (-7), size 7,072 exact.

## 2. Method

The wave-180 pass (w12410) left the tail a local optimum for the statement
ordering it searched.  This pass re-ran a targeted search on the tail only:

* adjacent-statement swap sweep over units 150..216 (67 builds);
* greedy adjacent hill-climb on the result (one further swap);
* bounded-distance (`|from-to| <= 3`) single-statement move sweep over units
  150..216;
* exhaustive single-statement move sweep over the reward-box group
  (units 190..216, 702 builds) — no further improvement.

The three landed moves are the only size-exact raw improvements found in those
neighbourhoods.  The final order is a local optimum for distance-3 moves over
units 150..216.

## 3. Structural experiments that were size-fatal

The task also asked for the `Globals` ternaries, the `volatile unsigned char
*rd` block and the `rewardBoxY2` bug.  The reward-box `y2` bug is **already
fixed in codegen** by w12410: `rewardBoxY` is now assigned before the `y2`
block and clang forwards the value into the `(y+17)<<retina` computation (the
tail stores `adds r1,#17; lsl r1,r1,ip` at `0xd2852`).  The source still reads
`this->rewardBoxY`; replacing it with a local `ry` produces byte-identical code
(raw 5,408), so no code change is needed.

Measured variants (all real `link_so` + `tools/fn_diff.py`, C1 alias):

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base | **5,408** | 2,438 | 7,072 |
| **landed: 0x208 move + 0x278/0x248 swap + footer/scrollBar swap** | **5,401** | 2,488 | **7,072** |
| `rewardBoxY` before `rewardBoxY2` (bug fix, y2 at tail) | 5,408 | 2,438 | 7,072 |
| local `ry`, `y2 = (ry+17)<<retina` at tail | 5,408 | 2,438 | 7,072 |
| reward-box branch tree (n9-first, shared y2/height/x) | 5,838 | 2,716 | 7,120 |
| helpWindowX/Y branch tree at tail | 5,481 | 2,473 | 7,028 |
| helpWindowX moved next to helpWindowY (flat) | 5,547 | 2,435 | 7,060 |
| `rd` block moved before `field_0x2d8` (strd merge) | 6,217 | 3,658 | 6,912 |
| `field_0x2c8` after `rd` block | 5,414 | 2,473 | 7,072 |
| `field_0x2c8` before `field_0x2cc` | 5,415 | 2,401 | 7,072 |
| flat ternary `0x2d4` (volatile kept) | 5,540 | 2,449 | 7,072 |
| flat ternary `0x2d4` (volatile dropped) | 5,518 | 2,459 | 7,056 |
| `field_0x90` explicit `(retina\|\|iPad\|\|n9)` | 5,408 | 2,438 | 7,072 |
| `field_0x1e8` explicit `(retina\|\|iPadHD\|\|n9)` | 5,408 | 2,438 | 7,072 |
| early fields `0x2c..0x70` ascending | 5,849 | 2,315 | 7,072 |
| `0x34,0x38,0x3c,0x40` contiguous ascending | 6,309 | 2,989 | 6,712 |
| `field_0x34` after `field_0x48` | 5,410 | 2,438 | 7,072 |

The size is a three-way compensation (code length + literal pool).  Any change
that lets clang merge the `0x2d4`/`0x2d8` pair into one `strd` (the original's
shape) shrinks the code by ~160 B and is size-fatal unless the pool grows by
the same amount; the base keeps the two stores separated by the `0x2c8` store
so the function stays exactly 7,072 B.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12511
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
# raw=5401 aligned=2488 size=7072/7072 insn=2518/2552
cmake --build cmake-build-match --target check -j2
# RATCHET PASS: byte_exact 4507, linked_exact 4507, size-exact 4524/4524,
# data_exact 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w12511/`: `Layout.base.cpp`
(pristine ctor), `units.py` (top-level statement parser), `sweep_adj.py`
(adjacent-swap sweep), `greedy_adj.py` (greedy adjacent hill-climb),
`greedy_move.py` / `greedy_move_near.py` (single-statement move searches),
`near_result.cpp` (landed ctor), `measure.sh`.

Gate at exit: byte_exact **4507/4524**, linked_exact **4507/4524**,
size-exact **4524/4524**, stub_zero_size **0**, missing **0**, wrong_type
**0**, extra **0**, data_exact **843/843**, imports **207**, lint **CLEAN**,
drift **0**, parity **5370/5370**, `.rel.plt` byte-identical.
`.text` differing bytes **76,193** (-7), `.ARM.extab` **713** (unchanged),
build-id **20**, total **76,926**.  `Layout` C1/C2 raw **5,401**, aligned
**2,488**, size **7,072/7,072**, 2,518 instructions.
