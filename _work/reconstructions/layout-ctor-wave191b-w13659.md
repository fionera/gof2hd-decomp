# `Layout::Layout` C1/C2 — wave 191b / worker w13659

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, linked symbol size **7,072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave191b-w13659`, base master `6c02aaeb4` (w13637 final: raw 5,244 /
aligned 2,575).

**Result: one size-exact Pareto win landed (raw −4, aligned −11).**

| | before (w13637) | after |
|---|---:|---:|
| raw | 5,244 | **5,240** |
| aligned | 2,575 | **2,564** |
| size | 7,072/7,072 | 7,072/7,072 |
| insn | 2,512/2,552 | 2,512/2,552 |
| `.text` diff | 71,210 | **71,206** |
| `.ARM.extab` diff | 577 | 577 |
| `.ARM.exidx` / `.rodata` | 0 / 0 | 0 / 0 |

`check` = RATCHET PASS: byte_exact 4508, linked_exact 4508, size-exact 4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.
`.rel.plt` byte-identical (29,392 B), all non-`.text` sections unchanged.

## The win

Two independent-field statement swaps in the constructor body:

```
progressBarYBase (0xd8)  <->  marqueeWidth (0xd4)     # new: -39 aligned, 0 raw
field_0x3c               <->  field_0x2c_rowHeight    # revert w13637's 3c/2c swap
```

`progressBarYBase`/`marqueeWidth` is a far-apart pair (source lines 89 and 101)
that the earlier adjacent / `j-i ≤ 6` sweeps never touched.  Alone on the
w13637 base it is worth **raw 5,244 / aligned 2,536** (zero raw, −39 aligned).
Reverting the w13637 `0x3c`/`0x2c` swap on top recovers 4 raw bytes at a cost
of 28 aligned instructions, giving the landed **5,240 / 2,564** — strictly
better than the w13637 base on both metrics.

Both swaps are semantic no-ops: the multiset of `(field, RHS)` statements is
byte-identical before/after (checked programmatically; the only duplicated
field, `field_0x184`, is untouched and both copies are identical), so the
store image is unchanged for all 32 `iPadHD/n9/iPadLarge/iPad/retinaDisplay`
combinations.

This recovers 4 of the +9 raw that w13637 spent.  The remaining 5 are the
`field_0x248`/`field_0x278` swap's literal-pool/scheduling cost; reverting it
too gives the full **5,235 / 2,625** (`candC`), but that gives back 50 aligned
instructions.  No single or double swap reachable from either point produced
raw 5,235 with aligned < 2,575 (see negative results).

## Measured candidates (all real `link_so` builds, size 7,072/7,072 exact)

Pareto-relevant points (raw / aligned):

| variant | raw | aligned |
|---|---:|---:|
| base_prev (w13607: all three w13637 swaps reverted) | 5,235 | 2,668 |
| base_prev + `progressBarYBase`/`marqueeWidth` | 5,235 | 2,629 |
| `candC` = landed + `pbY`/`marquee` + revert `3c/2c` + revert `248/278` | **5,235** | 2,625 |
| `candA` + `0x10`/`0x3c` swap | 5,241 | 2,554 |
| **landed: `candA` = w13637 + `pbY`/`marquee` + revert `3c/2c`** | **5,240** | **2,564** |
| w13637 + `pbY`/`marquee` | 5,244 | 2,536 |
| w13637 + `pbY`/`marquee` + `0x2d0`/`0x2cc` | 5,248 | 2,531 |
| w13637 + `0x188`/`0x194` | 5,302 | 2,534 |
| w13637 (before) | 5,244 | 2,575 |

Other measured points:

| variant | raw | aligned |
|---|---:|---:|
| w13637 + revert `3c/2c` | 5,240 | 2,603 |
| w13637 + revert `248/278` | 5,239 | 2,636 |
| w13637 + revert `3c/2c` + revert `248/278` | 5,235 | 2,664 |
| w13637 + `0x2d0`/`0x2cc` | 5,248 | 2,570 |
| `candA` + `0x2d0`/`0x2cc` | 5,244 | 2,559 |
| `candA` + revert `248/278` (= `candC`) | 5,235 | 2,625 |
| `candC` + `0x10`/`0x13` | 5,239 | 2,609 |
| `candC` + `0x10`/`0x3c` | 5,236 | 2,615 |
| `candC` + `0x188`/`0x194` | 5,294 | 2,584 |

## Negative / neutral results this session

All measured with `link_so -j2` + `tools/fn_diff.py`; size-exact unless noted.

* **Full adjacent-swap sweep on the w13637 base** (230 pairs, 74 size-exact):
  no raw win; best raw 5,244 (codegen-identical), best aligned 2,534
  (`0x188`/`0x194`, raw 5,302).
* **Non-adjacent sweep of the early (units 8–40) and late (units 170–194)
  regions on the w13637 base** (636 pairs): best raw 5,239 (revert
  `0x248`/`0x278`), best aligned 2,462 (`0x58`/`0x50`, raw 5,688).  No point
  with raw < 5,244 and aligned < 2,575.
* **900 random single swaps on the w13637 base**: found the
  `progressBarYBase`/`marqueeWidth` gem; no other point below 5,244 raw.
* **900 random single swaps on `candA`**: no improvement (best raw 5,240,
  best aligned 2,564 — codegen-identical).
* **900 + 900 random single swaps on `candC`**: no point with raw 5,235 and
  aligned < 2,625; best aligned at raw 5,235 is 2,625.
* **Full early+late region sweep on `candA`** (636 pairs): only
  `0x10`/`0x3c` = 5,241/2,554 (one raw worse, 10 aligned better); no
  dominating point.
* **Full units 40–84 region sweep on `candC`** (780 pairs): no raw-5,235
  point with aligned < 2,625.
* **Moves toward the original store order** (`0x34` after `0x38`, `0x44`
  before `0x40`, `0x48` before `0x54`, `0x50` before `0x58`, `0x74` after
  `0x70`): all size-fatal (7,012–7,064 B).
* **Move `field_0x278` immediately after `field_0x248`** (original adjacency):
  size 7,080 B (+8), fatal.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13659
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
# raw=5240 aligned=2564 size=7072/7072 insn=2512/2552
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13659/`: `base.cpp` (w13637),
`base_prev.cpp` (w13607), `candA.cpp` (landed), `candC.cpp`, `units.py`,
`parse_units.py`, `measure.py`, `mkvar.py`, `combos*.py`, `randsearch*.py`,
`sweep_adj.py`, `sweep_region*.py`, `focus.py`, `moves.py`, `secdiff.py`,
`dumpfn.py`, `*.json` result tables.

## Current-master recheck (2026-11-13, wave 191b harvest)

Master `04db7144b` already contains the `progressBarYBase`/`marqueeWidth`
swap from w13686.  Applying only the `field_0x3c`/`field_0x38`/
`field_0x2c_rowHeight` reorder on top gives raw **5,266 -> 5,262** (-4)
but aligned **2,442 -> 2,470** (+28), so it was not landed.  The checked-in
baseline remains raw 5,266 / aligned 2,442 / size 7,072 exact.
