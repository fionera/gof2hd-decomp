# `Status::missionCompleted` — wave 191b (w13658): tiered-body-order / case-158 spelling sweep

Worktree `wt/w13658`, branch `wave191b-w13658`, base master `fda02cb7e`.
Target `_ZN6Status16missionCompletedEbbx` (`src/game/mission/Status.cpp`,
1,432 B, VMA `0xabf38`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13658`.

**Status: research note, no `src/` change landed.**  Baseline re-confirmed on
the committed source: raw **71**, aligned **38**, size **1,432/1,432** exact,
527/527 instructions.  Whole-file section deltas: `.text` **71,201**,
`.ARM.extab` **577**, `.ARM.exidx` **0**, `.rodata` **0**, `.rel.plt` **0**,
`.dynsym`/`.dynstr`/`.data`/`.bss` **0**; all 27 section sizes and VMAs
identical.  Tree clean.

## 0. Residual decomposition (re-measured)

| region | diff bytes | VMA |
|---|---:|---|
| 40-entry TBH table | 24 | `abff2..ac034` |
| case-158 body (production-good) | 46 | `ac266..ac2a5` |
| literal pool (`Globals::status` GOT offset) | 1 | `ac4c4` |

The literal byte is downstream of the body (GOT-relative offset); the body is
the only independently addressable residual.

## 1. Why the TBH needs a body reorder *and* a relabel

Decoding both TBHs (`base = abfe6`, `target = base + 2*entry`) gives the
original body order by VMA:

```
167/174, 8, 11, 13, 14, 150,151,152,153,154,155,
158,162,166,170,184,189, 156,157,160,163,164,165,168,169,171,172, default
```

i.e. tier-1 `{167,174}` (shared body, 2/40), tier-2
`{150..155,158,162,166,170,184,189}` (12 bodies), tier-3
`{156,157,160,163,164,165,168,169,171,172}` (10 bodies).  MBP's
`selectBestSuccessor` (`MachineBlockPlacement.cpp` release_70) iterates the
jump-table MBB's successors in **table (value) order** and keeps a strictly
greater probability, so a flat switch can only produce ascending value order
(ours).  The tier split therefore requires tier-2 edge probability > tier-3.

The original's case *values* are also a permutation of ours (w13618 §1):
our→orig `156→158, 157→162, 158→166, 160→170, 162→184, 163→189, 164→156,
165→157, 166→160, 168→163, 169→164, 170→165, 171→168, 172→169, 184→171,
189→172`.  Relabelling alone changes the layout (body order is ascending in
the relabelled values), so the TBH is still wrong; reordering the bodies
without relabelling also leaves the same 16-entry TBH permutation (the
permutation is a bijection: `TBH_ours[v] = TBH_orig[MAP(v)]`, 16 entries
differ either way, 28 raw bytes by entry comparison / 24 by byte-run count).
Both the relabel **and** the tier order are required.

## 2. Tier order via a folded nested switch + `__builtin_expect`

`SimplifyCFG::FoldValueComparisonIntoPredecessors` folds a switch in the
default of a switch on the same value.  With no `!prof` the fold is
weight-neutral.  With `__builtin_expect` on the **inner** switch condition
(clang 7 emits `branch_weights` = `[1, 1, …, 2000, …, 1]`), the fold scales
the outer cases by `ValidTotalSuccWeight` and the inner cases by the outer
default weight (`SimplifyCFG.cpp` release_70 lines 1100-1160).  The result is

```
tier-2 = 2010, tier-3 = 2000/1, 167/174 = 4020, low block = 5*2010
```

which reproduces the original body order exactly:
`167/174, low(8,11,13,14), tier-2 ascending, tier-3 ascending`.

The fold only fires if the inner switch is the first instruction of its
block, so the two `getType()` calls must be CSE'd through a local
`int mtype = m->getType();` (otherwise the inner block starts with a second
`blx getType` and the fold is skipped; measured raw 1300/1396).

Measured (relabelled source, `tools/fn_diff.py`):

| variant | raw | aligned | size | insn |
|---|---:|---:|---:|---:|
| committed baseline | **71** | **38** | **1432** | 527 |
| relabel only, single switch, ascending source | 904 | 346 | **1424** | 529 |
| relabel + nested switch, no expect | 904 | 342 | **1424** | 529 |
| relabel + nested, inner expect 156 | 1288 | 386 | **1368** | 505 |
| relabel + nested, inner expect 0 | 1324 | 381 | **1376** | 506 |
| relabel + nested, outer expect 167 | 1301 | 518 | **1420** | 513 |
| relabel + nested, double expect (outer 167, inner 156) | 1291 | 529 | **1420** | 513 |
| relabel + nested, low group in inner, expect 156 | 1298 | 674 | **1364** | 503 |
| relabel + nested, inner expect 156, case-8 perturbed (`amount,cargo,index`) | 1310 | 377 | **1416** | 521 |

All 45 possible inner-expect values (150..189, 0, 8, 11, 13, 14) were swept:
sizes 1360–1376, never 1432.

### 2.1 Why the expect variant shrinks by 56–64 B

The `!prof` frequencies change the whole-function register allocation
(`this`: r6 → fp, `m`: sl → r9) and make `BranchFolder` tail-merge the two
identical production-good bodies (low-group type 8 at `ac09e` and type 172 at
`ac41a`).  The merged variant has only **2** `getProductionGoodIndex`, **3**
`getProductionGoodAmount`, **3** `getCargo`, **9** `getStation::getIndex`,
**9** `getTargetStation` calls vs the original's 4/4/4/10/10.  The
production-good merge alone accounts for the ~62-byte deficit.  Perturbing
the type-8 body's declaration order partially breaks the merge (size 1416)
but not fully, and the perturbation itself changes the type-8 body away from
the original (`index→r9, amount→r5, cargo→r2`).

## 3. Case-158 body spelling sweep (baseline single switch)

The original body is `index→r9, amount→r5, cargo→r2, this→r6`, with a
re-query `getProductionGoodIndex()` in `hasEquipment` and a single
`b.n ac458` tail into the shared `cbnz r0` test.  The committed spelling
(`index, cargo, amount` + `!hasEquipment(index,1)`) is the only one that
keeps 1,432 B at raw 71.  Full 6×4 assignment-order × tail sweep:

| assignment order | tail | raw | aligned | size | this spill |
|---|---|---:|---:|---:|---|
| index,cargo,amount | `!hasEquipment(index,1)` (committed) | **71** | **38** | 1432 | yes |
| amount,cargo,index | `!hasEquipment(index,1)` | 72 | 39 | 1432 | yes |
| cargo,index,amount | `!hasEquipment(index,1)` | 77 | 35 | 1432 | yes |
| cargo,amount,index | `!hasEquipment(index,1)` | 77 | 40 | 1432 | yes |
| index,amount,cargo | `!hasEquipment(index,1)` | 412 | 46 | 1432 | yes |
| amount,index,cargo | `!hasEquipment(index,1)` | 420 | 45 | 1432 | yes |
| amount,cargo,index | `hasEquipment(re-query,1)` | 90 | 32 | 1432 | no |
| cargo,amount,index | `hasEquipment(re-query,1)` | 94 | 35 | 1432 | no |
| index,cargo,amount | `hasEquipment(re-query,1)` | 384 | 43 | 1432 | no |
| cargo,index,amount | `hasEquipment(re-query,1)` | 390 | 44 | 1432 | no |
| index,amount,cargo | `hasEquipment(re-query,1)` | 682 | 145 | **1436** | no |
| amount,index,cargo | `hasEquipment(re-query,1)` | 686 | 144 | **1436** | no |

Declaration order is inert (all 6 permutations of `{index,amount,cargo}`
with a fixed assignment order produce identical code).  The best *aligned*
result (32) is `amount,cargo,index` + re-query, but it is raw 90 because the
4-byte `cmp r0,#0; b.n ac1d4` tail does not merge into `ac458` (the branch
folder picks the type-156 tail instead).  No size-exact shape below raw 71
exists in this family.

## 4. Source-order sweeps

* 60 random orderings of the 27 case blocks (baseline single switch): min raw
  91; the committed order (71) is better than every random order.
* 26 adjacent swaps + 26 "move case 158 to position k": 30 give raw 71, the
  rest 76–659; no swap/move beats 71.
* 50 random orderings with the `amount,cargo,index` + re-query body: min raw
  100.

So the committed source order is a local optimum and the body allocation is
robust to source order.

## 5. Conclusion

* TBH (24 B): the original body order is tiered (`167/174`, low, tier-2,
  tier-3).  Reproducing it needs tier-2 edge probability > tier-3, which is
  only source-expressible through a folded nested switch carrying
  `__builtin_expect` weights.  Those weights perturb the whole-function
  register allocation and tail-merge the two production-good bodies,
  shrinking the function by 56–64 B (never 1,432).
* Body (46 B): pure greedy-allocator live-range ordering.  The original
  allocation (`index→r9, amount→r5, cargo→r2, this→r6`) is what the type-8
  body gets in the *baseline* layout; the extra `hasEquipment` block changes
  the allocator's global state.  The closest size-exact re-query shapes are
  raw 90/384; the committed negated-tail device is raw 71.
* Literal (1 B): GOT-relative offset, downstream of the body/GOT layout.

No size-exact shape below raw 71 was found, so `src/` is left unchanged.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13658
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN6Status16missionCompletedEbbx     # raw=71 aligned=38 size=1432/1432
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13658/`: `gen3.py` (relabelled
nested-switch builder), `gen4.py` (baseline source-order builder), `gen5.py`
(case-158 replacement), `gen6.py` (relabelled single switch), `sweep*.py`
(measurement drivers), `sweep158.py` (case-158 6×4 sweep), `tbh2.py` (TBH
decoder), `measure.py` (section deltas), `MBP2.cpp`/`BPI.cpp`/`RAG.cpp`
(LLVM release_70 sources used for the placement/weight analysis).
