# `Layout::Layout` C1/C2 — wave 191b / worker w13670

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, linked symbol size **7,072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave191b-w13670`, base master `2064dfd40` (w13637 final: raw 5,244 /
aligned 2,575).

**Result: one size-exact aligned win landed (`field_0x168` n9 arm 37.5f -> 37.0f).**

| | before | after |
|---|---:|---:|
| raw | 5,244 | 5,245 (+1) |
| aligned | 2,575 | **2,564 (-11)** |
| size | 7,072/7,072 | 7,072/7,072 |
| insn | 2,512/2,552 | 2,512/2,552 |
| `.text` diff | 71,227 | 71,228 (+1) |
| `.ARM.extab` diff | 577 | 577 |
| `.ARM.exidx` / `.rodata` | 0 / 0 | 0 / 0 |

`check` = RATCHET PASS: byte_exact 4508, linked_exact 4508, size-exact 4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.
`.rel.plt` byte-identical.  The change is a semantic no-op: `field_0x168` is
`int`, the ternary is float-typed, and both `37.5f` and `37.0f` truncate to
`37`, so the store image is identical for all 32 `Globals` flag combinations.

## The win

```
-    this->field_0x168 = Globals::iPadHD ? 25 * 1.40625f : (Globals::n9 ? 37.5f : (Globals::retinaDisplay ? 50.0f : 25.0f));
+    this->field_0x168 = Globals::iPadHD ? 25 * 1.40625f : (Globals::n9 ? 37.0f : (Globals::retinaDisplay ? 50.0f : 25.0f));
```

The two variants differ in exactly **one byte** of the body: at function offset
`+0xd62` the original and the base both hold `0x16` (instruction `tst r6,r2`),
the `37.0f` variant holds `0x14` (`tst r4,r2`) — a register-allocation
perturbation, not a value change.  The perturbation removes 11 aligned
instruction mismatches for that one raw byte.  The aligned-per-raw ratio
(11) matches the w13637 landing (10.3), so it is landed as a net aligned win.

## The +9 raw recovery (angle a): not achieved

The +9 raw introduced by w13637 is exactly the cost of its two raw-priced
swaps: reverting `field_0x3c`/`field_0x2c_rowHeight` (A) recovers 4 raw
(+28 aligned), reverting `field_0x248`/`field_0x278` (C) recovers 5 raw
(+61 aligned).  Reverting both recovers the full 9 raw (5,235) but costs
+89 aligned (2,664), i.e. it does not keep the aligned gain.  No shape
compensation for that 89-instruction loss was found; the best independent
aligned win is the 37.0f literal above (-11 at +1 raw).  The 37.0f change
on top of the reverts gives 5,236 / 2,653 — still far from 2,575.

## Measured candidates (all real `link_so` builds; size 7,072 exact, extab 577)

| variant | raw | aligned |
|---|---:|---:|
| base (w13637) | 5,244 | 2,575 |
| **landed: `field_0x168` 37.0f** | **5,245** | **2,564** |
| `field_0x8c` 57.0f | 5,244 | 2,576 |
| 37.0f + 57.0f | 5,245 | 2,565 |
| revert A (3c/2c) | 5,240 | 2,603 |
| revert C (248/278) | 5,239 | 2,636 |
| revert A + C | 5,235 | 2,664 |
| 37.0f + revert A | 5,241 | 2,592 |
| 37.0f + revert C | 5,240 | 2,625 |
| 37.0f + revert A + C | 5,236 | 2,653 |
| `0xb8`/`0xc4` swap (89_93) | 5,272 | 2,540 |
| `0x1d0`/`0x1c0` swap (201_203) | 5,254 | 2,554 |
| `0x1dc`/`0x2c0` swap (199_202) | 5,252 | 2,559 |
| `0x238_blk16`/`0x108` swap (127_130) | 5,252 | 2,566 |
| 37.0f + 201_203 | 5,255 | 2,543 |
| 37.0f + 199_202 | 5,253 | 2,548 |
| 37.0f + 89_93 | 5,273 | 2,529 |

## Negative / neutral results

All measured with `link_so -j2` + `tools/fn_diff.py`; size-exact unless noted.

* **Full adjacent + distance-2..6 swap sweep on the landed base** (934 pairs,
  157 size-exact): no candidate with raw < 5,244 and aligned <= 2,575.  Best
  raw is revert C (5,239 / 2,636); best aligned is 89_93 (5,272 / 2,540).
* **Full distance-2..6 swap sweep on the 37.0f base** (934 pairs, 157
  size-exact): no candidate with raw <= 5,244 and aligned <= 2,575.  Best raw
  is revert C (5,240 / 2,625); best aligned is 89_93 (5,273 / 2,529).
* **Large-distance random swaps** (158 samples, distance > 6): only 4
  size-exact; none raw < 5,244.  The size ridge is narrow: the size-exact rate
  falls from ~17 % at distance <= 6 to ~2.5 % beyond.
* **600 single int-arm -> float-arm rewrites** (`? 84 :` -> `? 84.0f :`):
  468 size-exact, all codegen-identical except three raw-5,455 variants.
* **70 `N * 1.40625f` -> int folds** and **55 `N.0f` -> int literals**:
  43 / 55 size-exact; every size-exact one is codegen-identical (5,244 /
  2,575).  Only `37.5f` (landed) and `57.6f` are codegen-changing; `57.6f`
  worsens aligned.  `5.0f`, `0.0f`, `84.0f`, `244.0f` int conversions are
  size-fatal.
* **Cast-shape variants** (drop `(int)(float)` / `(int)(float)(int)` round
  trips on `0x2c8`, `0x2d0`, `0xa8`, `0x10c`, `0x150`, `0x160`): most are
  codegen-identical; dropping the `0x2c8`/`0xa8`/`0x10c` round trip is
  size-fatal (7,064).
* **Boolean simplifications** (`field_0x90`, `field_0x1e8`, `helpWindowX`
  redundant-arm collapse): all 7,064 B (size-fatal, aligned 2,568).
* **Full iOS source-order reorder** of the ctor: 7,212 B (fatal); iOS prefix
  reorders are fatal for every prefix except the first 1-3 stores (7,080 B).
  The current hill-climbed order is a very narrow size optimum.
* **Mid-block (`0x170..0x1ac`) reorders** toward the iOS order: 6,864-7,048 B
  (fatal) except one adjacent pair (5,319 / 2,576).
* **Duplicate `footerTextInset` / `field_0x184` moves**: only the adjacent
  no-op positions are size-exact and codegen-identical; every other position
  is size-fatal.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13670
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
# raw=5245 aligned=2564 size=7072/7072 insn=2512/2552
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13670/`: `harness.py`, `sweep.py`,
`sweep_base.py`, `sweep37.py`, `pairs.py`, `reverts.py`, `shapes.py`,
`literals.py`..`lit6.py`, `foldprod*.py`, `intlit.py`, `int2float.py`,
`casts.py`, `bools.py`, `reorder_early.py`, `reorder_mid.py`, `ios_order.py`,
`ios_prefix.py`, `order_default_test.py`, `dups.py`, `topchain.py`, `t168.py`,
`move168.py`, and the `*.jsonl` measurement logs.

## Harvest onto wave-191b master `df4e658df`

The 37.0f hunk was re-measured on the current master (which already contains
w13690's `orbitInfoSystemY` hoist and the w13686 store-order swap):

| | before (master) | after |
|---|---:|---:|
| raw | 5,265 | 5,266 (+1) |
| aligned | 2,453 | **2,442 (-11)** |
| size | 7,072/7,072 | 7,072/7,072 |
| `.text` diff | 72,115 | 72,116 (+1) |
| `.ARM.extab` diff | 564 | 564 |

`check` = RATCHET PASS: byte_exact 4508/4524, size-exact 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.
