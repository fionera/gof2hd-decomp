# `Layout::Layout` C1/C2 — wave 191b / worker w13648

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, linked symbol size **7,072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave191b-w13648`, base master `4b2b1c10f`.

**Result: one size-exact aligned win landed (raw +39, aligned −50).**  It is a
new Pareto point: it dominates the unlanded w13624 point (5,294 / 2,616) on
raw by 20 bytes while giving up only 2 aligned instructions.

| | before | after |
|---|---:|---:|
| raw | 5,235 | 5,274 |
| aligned | 2,668 | **2,618** |
| size | 7,072/7,072 | 7,072/7,072 |
| insn | 2,517/2,552 | 2,517/2,552 |
| `.text` diff | 71,197 | 71,236 (+39) |
| `.ARM.extab` diff | 577 | 577 |
| `.ARM.exidx` / `.rodata` / `.data` / `.bss` | 0 | 0 |

`check` = RATCHET PASS: byte_exact 4508, linked_exact 4508, size-exact 4524,
data 843/843, imports 207, lint `CLEAN ✓`, drift 0, parity 5370/5370.
`.rel.plt` byte-identical.  All 27 section sizes/base addresses unchanged.

## The win

Three independent store-order changes (all RHS are pure constant/global
ternaries, so every change is a semantic no-op):

1. swap `field_0x188` (0x188) before `field_0x194` (0x194) — the w13624
   `0x188/0x194` swap;
2. swap `field_0x1a0` (0x1a0) before `field_0x198` (0x198) — reverts the
   w13573 landing, as in w13624;
3. **new:** move `this->field_0x14c = ...` (0x14c) up before the
   `field_0x120`/`field_0x108` stores, i.e. before the `field_0x130`
   if/else chain.

Change 3 is worth −20 raw on top of the w13624 pair (5,294 → 5,274) for
+2 aligned (2,616 → 2,618).  It was found by a greedy adjacent-swap
hill-climb from the w13624 point: the hill-climb slid the `0x14c` unit three
positions left (swap 95 → 94 → 93) and then stalled.

Semantics verified with the store-image emulator over all 32 flag
combinations (`cmpcombos.py`, patched `layout_ctor/emu.py`): **bad combos 0
of 32**.

## Raw floor (fresh-angle (a) result)

The raw metric cannot be pushed below **5,232** by any store reordering found:

* the 5-unit `0x2c8/0x2cc/0x2d0/0x2d4/0x2d8` block was swept exhaustively
  (all 120 permutations, with the `rd` declaration kept immediately before
  its `0x2d4` use): 18 are size-exact, and the raw minimum is
  **5,232 / 2,700** at order `2cc, 2d0, 2d8, 2c8, 2d4` — which is exactly the
  pre-w13607 base (w13607 traded it for 5,235 / 2,668).  Landing it would
  revert w13607's aligned win for −3 raw / +32 aligned.
* all 205 adjacent swaps of the current base: 83 size-exact, none below
  raw 5,235.
* all 431 pairs of the 30 non-identical size-exact adjacent swaps: minimum
  raw 5,236 (aligned 2,679).
* all 442 triples of the 15 most promising of those swaps: minimum raw
  5,237 (aligned 2,679).
* the same adjacent-swap sweep on the 5,232 base and on the 5,239 / 2,663
  base: no size-exact swap beats its own base.
* moving `field_0x14c` to any of 40 positions on the current base: all
  worse (raw 5,412+); the move only pays off on top of the `0x188/0x194`
  pair.

So no source edit was found that improves both raw and aligned: the Pareto
frontier at size 7,072 is

| variant | raw | aligned |
|---|---:|---:|
| pre-w13607 (revert) | 5,232 | 2,700 |
| **landed (w13624 pair + `0x14c` move)** | **5,274** | **2,618** |
| w13624 pair only | 5,294 | 2,616 |
| w13624 pair + `0x26c/0x274` swap | 5,300 | 2,610 |
| current base (w13607) | 5,235 | 2,668 |

## Other fresh-angle measurements (all real `link_so` builds)

### Faithful (iOS) source order — angle (b)

The iOS 1.1.4 source order recovered in
`layout_ctor/ios_source_order.md` is the true source order.  Applying it to
the whole ctor gives **5,959 / 2,617 / 6,960 B** (112 B short of the size
ridge).  Splitting it — iOS order for the first `k` units, current order for
the rest — was swept for all `k` (207 builds): only `k = 0,1,3` are
size-exact, and `k = 3` is 5,657 / 2,676.  The mirror split (current head,
iOS tail) was also swept for all `k`: only `k = 204..206` are size-exact
(identity); `k = 53` reaches **5,955 / 2,470 / 6,960** — the best aligned
point seen, but 112 B short.  The iOS order systematically shrinks the body
by ~112 B; landing it needs the full structural reconstruction described in
`Layout-LayoutC1-wave191-w13509.md` (outlined iPad tails + `strd` 0x2d4/0x2d8
+ `0x34`-phase SLP bundles) to compensate.

### Block reorders to the original default store order

Reordering one field range to `layout_ctor/order_default.txt` order:

| range | raw | aligned | size |
|---|---:|---:|---:|
| 0x280..0x320 | 6,105 | 2,673 | 7,144 |
| 0x140..0x17c | 5,868 | 2,999 | 7,032 |
| 0x108..0x12c | 5,930 | 2,724 | 7,000 |
| 0x1b0..0x1e8 | 6,049 | 2,935 | 6,964 |
| 0x204..0x27c | 6,035 | 2,886 | 6,980 |
| 0x180..0x1e8 | 6,081 | 2,687 | 6,928 |
| 0x84..0x124 | 6,310 | 3,607 | 6,892 |
| 0x7c..0xdc | 6,385 | 4,171 | 6,932 |

`0x280..0x320` is the only code-*growing* reorder (+72 B).  Pairing it with
each shrinking block reorder (9 combinations) never hits 7,072: closest are
7,104 (with 0x108..0x12c) and 7,092 (with 0x1ec..0x27c).

### Temporary / cast / condition-order angles

All codegen-identical to the base (5,235 / 2,668 / 7,072):

* move the `(int)(float)` round trip from `field_0x2d0` to `field_0x2cc`
  (and both / neither);
* `const int v`, `const int t_field_0x2c8`, `const volatile unsigned char *rd`;
* `field_0x2d0` as an explicit if/else chain with a temporary;
* `__builtin_expect(Globals::iPad, 0/1)` on the inner `iPad ? 45 : 30` and
  `iPad ? 64 : 44` arms of 0x2c8/0x2d0/0x60 (does not trigger the original's
  cold-block outlining);
* swapping the operands of `(retinaDisplay || n9)` in 0x2d8, and all 5
  permutations of the `helpWindowX` 3-way `||`;
* `field_0x24 = field_0x20_top = ...` chained assignment / shared local
  (no `strd ip,ip,[r0,#32]` reachable).

Size-fatal or worse: `field_0x2cc` as an if/else chain (5,720 / 7,056),
`field_0x60` as an if/else chain (5,723 / 7,016), `field_0x2c8` as a `float`
temporary (5,455 / 7,088), `(*rd || n9)` → `(n9 || *rd)` (5,996 / 6,948),
`0x114` `||` operand reversal (6,230 / 6,824).

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13648
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
# raw=5274 aligned=2618 size=7072/7072 insn=2517/2552
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13648/`: `harness.py` (unit
splitter + build/measure), `sweep_block2.py` (120 block permutations),
`sweep_adj.py` (205 adjacent swaps), `sweep_pairs.py`, `sweep_triples.py`,
`iosorder.py`/`ios_split.py`/`ios_split2.py`, `blockorder.py`, `grow.py`,
`hill.py`, `sweep_cast.py`, `sweep_expect.py`, `sweep_misc.py`,
`cmpcombos.py` + patched `emu.py`/`order.py`.
