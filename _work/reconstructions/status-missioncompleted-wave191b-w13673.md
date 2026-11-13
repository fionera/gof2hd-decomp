# `Status::missionCompleted` — wave 191b (w13673): body-spelling / if-chain / weight sweeps

Worktree `wt/w13673`, branch `wave191b-w13673`, base master `2064dfd40`.
Target `_ZN6Status16missionCompletedEbbx` (`src/game/mission/Status.cpp`,
1,432 B, VMA `0xabf38`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13673`.

**Status: research note, no `src/` change landed.**  Baseline re-confirmed on
the committed source: raw **71**, aligned **38**, size **1,432/1,432** exact,
527/527 instructions.  Whole-file content deltas: `.text` **71,227**,
`.ARM.extab` **577**, `.ARM.exidx` **0**, `.rodata` **0**; all 27 section
sizes/VMAs identical.  `check` re-run in this worktree: `RATCHET PASS`
(byte_exact 4508, linked_exact 4508, size 4524/4524, data 843/843, imports
207, lint CLEAN, drift 0, parity 5370/5370).  Tree clean.

## 0. Residual map (re-measured)

| region | diff bytes | VMA |
|---|---:|---|
| 40-entry TBH table | 24 | `abfe6..ac036` |
| production-good body (source `case 158`) | 46 | `ac242..ac2a5` |
| literal pool (`Globals::status` GOT offset) | 1 | `ac4c4` |

## 1. Key layout fact: the original is the committed layout with the bodies permuted

The original's 22 body start addresses are **exactly** the committed build's
(`ac110, ac116, ac11c, ac150, ac156, ac16e, ac180, ac1dc, ac242, ac2a6,
ac2b4, ac2ee, ac32a, ac354, ac388, ac3b6, ac3d2, ac3d6, ac3de, ac3fc,
ac400, ac41a`), plus the shared `ac036` and the latch `ac45a`.  Only the
**type→body labels** are permuted (w13618 §1 mapping).  So the original's
layout is the *committed* layout with the 22 bodies permuted in place; the
latch (`add fp,#1; b loop`), the low-group bodies, the return blocks and the
literal pool keep their committed addresses.

The TBH is value-indexed (`sub #150; cmp #39; tbh`), so a non-monotonic TBH
requires the bodies to be placed non-monotonically in value order.  MBP's
`collectViableSuccessors` walks `MBB->successors()` (jump-table order =
ascending value) and `selectBestCandidateBlock` keeps the first max-frequency
block, so equal frequencies always give ascending value order.

## 2. Fresh angle (a): body spellings of `case 158`

All measured on the committed (unrelabelled) source with `tools/fn_diff.py`;
`raw / aligned / size / insn`.

| body spelling | raw | al | size | insn |
|---|---:|---:|---:|---:|
| committed `index,cargo,amount` + `!hasEquipment(index,1)` | **71** | **38** | **1432** | 527 |
| `index,cargo,amount` + `hasEquipment(re-query,1)` positive | 384 | 43 | 1432 | 527 |
| `cargo,index,amount` + `hasEquipment(re-query,1)` positive | 390 | 44 | 1432 | 527 |
| `index,amount,cargo` + `!hasEquipment(index,1)` | 412 | 46 | 1432 | 527 |
| `index,amount,cargo` + `hasEquipment(index,1)` positive | 672 | 168 | 1428 | 526 |
| `index,amount,cargo` + `hasEquipment(re-query,1)` positive | 682 | 145 | 1436 | 529 |
| no-locals faithful (`isInList(gi,ga,cargo)` + re-query positive) | 682 | 145 | 1436 | 529 |
| `||` combined / `bool` local / `else if` | 682 | 145 | 1436 | 529 |
| `hasEquipment` first, then `isInList` | 670 | 143 | 1396 | 514 |
| re-query negative | 660 | 143 | 1440 | 530 |

The committed spelling is a local optimum.  The 46-byte diff is the original's
`index→r9, amount→r5, cargo→r2, this→r6` allocation plus the short
`b.n ac458` tail; every faithful (positive, re-query) shape either spills
`this` (`ldr r6,[sp,#16]`) or merges the tail with `ac1d4`
(`cmp r0,#0; b.n ac1d4`), costing +4 B (1,436).

## 3. Fresh angle (b): `if`-chain instead of `switch`

Replacing the switch with `do { if (mtype==0||mtype==11) ... else if ... }
while (0)` (committed labels, source order) still lowers to jump tables but
changes the layout: **raw 1,314, aligned 354, size 1,368/1,432, 498 insns**
(three dispatch tables, not the original's two).  Not size-exact.

## 4. Fresh angle (c): nested-switch + `__builtin_expect` weight sweeps

The fold `SimplifyCFG::FoldValueComparisonIntoPredecessors` needs the inner
switch to be on the *same SSA value* (`int mtype = m->getType();`) and the
outer switch to carry `!prof`.  `__builtin_expect` is the only
source-expressible weight; it emits a 2000:1 one-hot vector, and the fold
scales the tier groups:

* **Structure A** outer = low group + `{167,174}` + tier-2
  `{150..155,158,162,166,170,184,189}`, inner = tier-3
  `{156,157,160,163,164,165,168,169,171,172}`.  With outer expect 167 the
  body order becomes the original's tiered order (TBH rank pattern matches
  except the latch/default slot), but the 2000:1 ratio moves the latch and
  tail-merges the two production-good bodies (`getProductionGoodIndex`
  4→2, `getProductionGoodAmount` 4→3, `getCargo` 4→3), shrinking the
  function.
* **Structure B** outer = low group + tier-3, inner = `{167,174}` + tier-2.
  Same effect.

Measured (raw / aligned / size / insn), 30 outer-expect values × 2 structures
plus inner-expect values; **no size-1432 hit**:

| variant | raw | al | size | insn |
|---|---:|---:|---:|---:|
| A, no expect (weight-neutral fold) | 904 | 342 | 1424 | 529 |
| A, outer expect 167 (or 174) | 1301 | 518 | **1420** | 513 |
| A, outer expect 14 | 1326 | 655 | **1424** | 516 |
| A, outer expect 152 | 1338 | 537 | 1420 | 518 |
| A, outer expect 158 | 1352 | 518 | 1344 | 505 |
| A, outer expect 162 | 1309 | 647 | 1360 | 503 |
| A, outer expect 166 | 1319 | 533 | 1416 | 516 |
| A, outer expect 170 | 1311 | 475 | 1384 | 499 |
| A, outer expect 184 | 1361 | 493 | 1384 | 504 |
| A, outer expect 189 | 1326 | 591 | 1416 | 517 |
| A, outer expect 8 / 0 | 1323 / 1341 | 536 / 625 | 1396 / 1396 | 508 / 506 |
| A, inner expect 156 / 157 / 160 | 1288 / 1320 / 1316 | 386 / 414 / 401 | 1368 / 1376 / 1376 | 505 |
| B, no expect | 903 | 355 | 1424 | 529 |
| B, outer expect 150/167/158 | 1258 | 481 | **1404** | 527 |
| B, outer expect 165 | 1312 | 619 | **1436** | 522 |
| B, outer expect 171 / 172 | 1345 / 1361 | 630 / 702 | 1424 / 1428 | 520 / 529 |
| B, outer expect 156 / 0 | 1319 / 1332 | 609 / 699 | 1376 / 1376 | 511 / 499 |

Structure B with outer expect 150/167/158 keeps the original instruction
count (527) but is 28 B short (1,404): the latch is placed between the bodies
instead of after them, so the `break` branches use short `b.n` encodings.

The only configuration known to give **size 1,432 with a byte-identical TBH**
is the flat `!prof` vector found by w13640 (default 1, `{167,174,8,0,11,13,14}`
= 4, tier-2 = 3, tier-3 = 2) injected into the optimized IR: raw **47**,
aligned 19, size 1432.  That vector is a mild 4:3:2:1 plateau, not
source-expressible (clang 7 `handleSwitchExpect` is one-hot 2000:1); the
2000:1 weights perturb the whole-function register allocation and layout.

## 5. Conclusion

* TBH (24 B): needs the tiered body order.  The mild flat `!prof` plateau
  reproduces it at size 1,432, but no source spelling (`__builtin_expect`
  one-hot, nested switch, if-chain) yields both the order and the size.
* Body (46 B): the committed `!hasEquipment(index,1)` spelling is the only
  size-exact local optimum (raw 71); every faithful positive/re-query shape
  is 1,428–1,440 B.
* Literal (1 B): downstream of the body/GOT layout.

No size-exact shape below raw 71 was found, so `src/` is left unchanged.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13673
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN6Status16missionCompletedEbbx     # raw=71 aligned=38 size=1432/1432
(ulimit -v 12000000; cmake --build cmake-build-match --target check -j2)
# RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13673/`: `Status.base.cpp`,
`Status.relabel.cpp`, `Status.nested.cpp`, `gen.py` / `gen2.py` (nested
switch builders), `gen_flat.py` (source-order builder), `gen_if.py`
(if-chain builder), `gen_body.py` (body variants), `measure.sh`, `try.sh`,
`tbh2.py` (TBH decoder + rank comparison), `sweep_oe.txt` (60-variant
outer-expect sweep), `MBP.cpp` / `SDB.cpp` / `SCFG.cpp` / `BF.cpp` / `MBB.cpp`
(LLVM release_70 sources used for the placement/weight/tail-merge analysis).
