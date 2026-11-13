# `Layout::Layout` C1/C2 — wave 191b / worker w13690

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, linked symbol size **7,072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave191b-w13690`, base master `b3fd7e6a2` (current base: raw 5,244 /
aligned 2,575, the w13637 landing).

**Result: one size-exact structural aligned win landed (raw +21, aligned −83).**

| | before | after |
|---|---:|---:|
| raw | 5,244 | 5,265 |
| aligned | 2,575 | **2,492** |
| size | 7,072/7,072 | 7,072/7,072 |
| insn | 2,512/2,552 | 2,512/2,552 |
| `.text` diff | 71,270 | 71,291 (+21) |
| `.ARM.extab` diff | 577 | 577 |
| `.ARM.exidx` / `.rodata` | 0 / 0 | 0 / 0 |

`check` = RATCHET PASS: byte_exact 4508, linked_exact 4508, size-exact 4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.
`.rel.plt` byte-identical.  Store image verified over all 32
`iPadHD/n9/iPad/iPadLarge/retinaDisplay` combinations (`valall.py`,
0 bad combos).

## The win

Three independent-field store-order changes in the ctor body (all RHS are
pure constant/global ternaries, so every change is a semantic no-op):

1. **move** `this->orbitInfoSystemY = ...` from after the `field_0x218`
   if/else chain to just before `this->field_0x234 = ...`;
2. **swap** `this->field_0x208 = ...` with `this->orbitInfoSecurityY = ...`
   (adjacent statements);
3. **swap** `this->field_0x1d0 = ...` with `this->field_0x1c0 = ...`
   (`field_0x2c0` stays between them).

The move (1) alone is the load-bearing edit: raw 5,250 / aligned 2,537
(+6 / −38).  Adding (2) gives 5,255 / 2,516; adding (3) gives the landed
5,265 / 2,492.

## Measured candidates (all real `link_so` builds, size 7,072 exact)

Adjacent-swap sweep of the current base (230 builds, 74 size-exact): no swap
dominates 5,244/2,575.  Best aligned per raw tier:

| variant | raw | aligned |
|---|---:|---:|
| base (w13637) | 5,244 | 2,575 |
| swap `0x2d0`/`0x2cc` (P) | 5,248 | 2,570 |
| swap `0x188`/`0x194` (A) | 5,302 | 2,534 |
| swap `0xb8`/`0xc4` (B) | 5,272 | 2,540 |
| swap `0x9c`/`0x8c` (E) | 5,338 | 2,543 |
| swap `0x1d0`/`0x1c0` (C) | 5,254 | 2,554 |
| swap `0x1dc`/`0x2c0` (D) | 5,252 | 2,559 |
| swap `0x198`/`0x18c` (I) | 5,303 | 2,552 |

Distance-2..5 swap sweeps (1,139 builds): only two swaps reduce raw below
the base — `0x2c`/`0x3c` (5,240 / 2,603) and `0x248`/`0x278` (5,239 /
2,636) — both cost more aligned than they save.

Pairs/triples of the best aligned swaps:

| variant | raw | aligned |
|---|---:|---:|
| A+B | 5,330 | 2,499 |
| A+B+C | 5,338 | 2,478 |
| A+B+E | 5,424 | 2,467 |
| B+C | 5,282 | 2,519 |
| B+D | 5,280 | 2,524 |
| A+B+E+C | 5,507 | 2,449 |
| A+B+E+D | 5,432 | 2,451 |

Single-statement insertion moves, distance ≤ 10 (4,510 builds, 492
size-exact): no move dominates.  Best aligned moves:

| move | raw | aligned |
|---|---:|---:|
| `orbitInfoSystemY` → before `field_0x234` (M) | 5,250 | 2,537 |
| `field_0x300` → before `field_0x2fc` | 5,254 | 2,554 |
| `field_0xb0` → after `field_0xc0` | 5,444 | 2,529 |

Full-range moves of the promising units (`orbitInfoSystemY`, `field_0x300`,
`field_0xb0`, `field_0xc0`, `field_0x188`, `field_0x194`, `field_0x180`,
`field_0x2c`, `field_0x3c`, `field_0x248`, `field_0x278`; 2,990 builds):
no size-exact move outside the local window beats M.

Combinations on top of M:

| variant | raw | aligned |
|---|---:|---:|
| M | 5,250 | 2,537 |
| M + `0x2d0`/`0x2cc` | 5,254 | 2,532 |
| M + `0x208`/`orbitInfoSecurityY` (S) | 5,255 | 2,516 |
| **M + S + `0x1d0`/`0x1c0` (landed)** | **5,265** | **2,492** |
| M + S + `0x2d0`/`0x2cc` | 5,259 | 2,511 |
| M + S + B | 5,287 | 2,482 |
| M + S + A | 5,313 | 2,476 |
| M + S + A + B + E | 5,439 | 2,410 |
| M + S + C + `0x300` move + P | 5,279 | 2,475 |

The landed point strictly dominates the unlanded w13648 point
(5,274 / 2,618) on both raw and aligned, and is the best aligned point
found below raw 5,270.

## Negative / neutral results this session

* **No strict dominator of 5,244/2,575 was found** in any of the sweeps
  above.  The closest is `M + 0x2c/0x3c` at 5,246 / 2,565 (+2 raw, −10
  aligned); `M + 0x248/0x278` is 5,245 / 2,598.  The raw metric is dominated
  by the literal-pool ordering, so a 2-byte raw shave is not reachable by a
  local store reorder.
* **Exhaustive 120-permutation sweep of the `t_field_0x2c8/2c8/2d0/2cc/2d8`
  block**: best aligned is the `0x2d0`/`0x2cc` swap (5,248 / 2,570); no
  permutation gives raw ≤ 5,244 with aligned < 2,575.
* **`rd` declaration moves** within units 30..37 are codegen-identical;
  moving it into the `0x2d4` if/else chain is size-fatal.
* **`0x2cc` int-typed / `0x2d0` float-round-trip** experiments (matching the
  original's `str.w`/`vstr` split): `0x2cc` as `(int)(...)` per arm shrinks
  the body to 7,056 (16 B short); every `0x2d0` form tried
  (`(int)(float)(int)`, all-float ternary, local int/float, volatile local)
  is codegen-identical to the base or size-fatal.  The original's 2d0
  round-trip is not reproducible from a constant-ternary source at this
  optimisation level.
* **Adjacent swaps on top of M** (230 builds, 78 size-exact) and **on top of
  M+`0x2c`/`0x3c`** (230 builds, 77 size-exact): no raw ≤ 5,244 with
  aligned < 2,575.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13690
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
# raw=5265 aligned=2492 size=7072/7072 insn=2512/2552
python3 scratch/w13690/valall.py cmake-build-match/libgof2hdaa.so   # bad combos 0 of 32
cmake --build cmake-build-match --target check -j2                   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13690/`: `harness.py` (unit
splitter + direct compile/link/measure), `sweep_swaps.py` (adjacent and
distance-N swaps), `sweep_combos.py` / `sweep_multi.py` (pairs/triples),
`sweep_moves2.py` / `sweep_unit.py` (insertion moves), `sweep_block.py`
(2c8..2d8 permutations), `valall.py` (32-combo store-image check),
`storeseq.py`, `dumpdiff.py`.
