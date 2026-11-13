# `Layout::Layout` C1/C2 — wave 191b / worker w13679

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, linked symbol size **7,072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave191b-w13679`, base master `b221b3826` (w13637 final: raw 5,244 /
aligned 2,575).

**Result: one size-exact aligned win landed (raw +15, aligned −62).**

| | before | after |
|---|---:|---:|
| raw | 5,244 | 5,259 |
| aligned | 2,575 | **2,513** |
| size | 7,072/7,072 | 7,072/7,072 |
| insn | 2,512/2,552 | 2,512/2,552 |
| `.text` diff | 71,247 | 71,262 |
| `.ARM.extab` diff | 577 | 577 |
| `.ARM.exidx` / `.rodata` | 0 / 0 | 0 / 0 |

`check` = RATCHET PASS: byte_exact 4508, linked_exact 4508, size-exact 4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.
`.rel.plt` byte-identical.  Semantics verified with `compare_combos.py`
(store image identical to the original for all 32 `iPadHD/n9/iPad/iPadLarge/
retinaDisplay` combinations, 807 bytes / 202 words).

## The win

Three independent-field statement swaps in the ctor body (each a semantic
no-op: every target is a distinct, side-effect-free store, and the RHS of each
reads only `Globals::`):

```
orbitInfoStationY if/else chain  <->  orbitInfoSystemY store   (raw +6, aligned −38)
field_0x1d0                      <->  field_0x1c0              (raw +10, aligned −21)
field_0x1dc                      <->  field_0x2c0              (raw +2, aligned −5)
```

The three are independent; the combination is the best Pareto point found at
raw ≤ 5,260 (see table).  The landed source order for the 0x1c0..0x1dc block is
`2c0, 1d4, 1c0, 1dc, 1d0`, and for the 0x214..0x224 block it is
`orbitInfoSystemY, field_0x218 chain, orbitInfoStationY chain`.

## Measured candidates (real `link_so` builds; size 7,072/7,072 exact unless noted)

| variant | raw | aligned | `.text` | notes |
|---|---:|---:|---:|---|
| base (w13637) | 5,244 | 2,575 | 71,247 | |
| **landed: 153/155 + 135/137 + 133/136** | **5,259** | **2,513** | **71,262** | |
| 153/155 only | 5,250 | 2,537 | 71,253 | |
| 135/137 only | 5,254 | 2,554 | 71,257 | |
| 133/136 only | 5,252 | 2,559 | 71,255 | |
| landed + 53/57 | 5,291 | 2,479 | 71,294 | more aligned, +32 raw |
| landed + 188/194 | 5,318 | 2,472 | 71,321 | more aligned, +59 raw |
| landed + 143/144 + 122/123 + 34/35 + 141/142 | 5,328 | 2,433 | 71,331 | hill-climb, raw +84 |
| 13/15 + 162/165 | **5,235** | 2,664 | 71,238 | recovers the full +9 raw, loses the aligned gain |
| 162/165 only (revert w13637 248/278) | 5,239 | 2,636 | 71,242 | |
| 13/15 only (revert w13637 3c/2c) | 5,240 | 2,603 | 71,243 | |

All keep `.ARM.extab` at 577 B.

## Fresh-angle results (negative / not landed)

* **(a) Recover the +9 raw with a compensating shape.**  The two raw-costing
  w13637 swaps are `3c/2c` (raw +4) and `248/278` (raw +5).  Reverting both
  (`13<->15` + `162<->165`) gives raw **5,235** exactly, but aligned rises to
  **2,664** — the aligned gain of w13637 is entirely carried by those two
  swaps.  Every combination that recovers raw to ≤ 5,244 has aligned > 2,575;
  no size-exact candidate with raw ≤ 5,244 and aligned < 2,575 was found.
* **(b) Another independent store-order pair with a raw win.**  Full adjacent
  swap sweep on the landed variant (190 safe pairs; 76 size-exact): best raw
  5,244 (codegen-identical pairs), no raw win.  Distance-2..5 swap sweep
  (743 pairs; 151 size-exact): the only raw win is `162<->165` (5,239) at
  aligned 2,636.  Single-statement move sweep of the 0x34..0x70 block
  (304 builds), of 0x248/0x258/0x25c (≈100 builds) and of the 0x214..0x234
  block (≈150 builds): no size-exact raw win with aligned < 2,575.
* **Early-block SLP grouping.**  The original vectorises the 0x34..0x70 block
  as `vst1` groups `[34..40] [44..50] [54..60] [64..70]`; ours emits
  `[38..44] [58..64] [68..74] [48..54]`.  Sorting the 16 fields ascending
  (contiguous, 120 placements; 300 random interleavings of 0x2c/0x30/0x74/
  0x78) always shrinks the body: best size-exact-ish points are
  5,781/2,557/**7,056** (full ascending 0x2c..0x78) and
  5,829/2,511/**7,040** (ascending 0x34..0x70, 0x74/0x78 after 0x70).  All
  are 16–200 B short of the 7,072 ridge; no +32 B compensation was found
  (`rewardBoxY2` `(int)(float)` round trip adds only +16 on that base, to
  7,056).  Not landed.
* **Late-block ascending order** (0x248..0x278, 0x238.c/d, 0x27c): size-exact
  but 5,501/2,627 (raw +257, aligned +52).
* **Hill climbs** from the landed base and from the landed variant with
  adjacent swaps: best aligned 2,433 at raw 5,328 (landed + 4 swaps), then
  flat; every step after the landed three costs ≥ 2 raw per aligned
  instruction saved.
* **Structural variants**: `volatile unsigned char *rd` → `const`/plain/removed
  gives 5,376/2,581/**7,064**; `field_0x2c8` without the `(int)(float)` round
  trip gives 5,423/2,610/**7,064**; dropping the redundant `footerTextInset`
  stores from the initial if/else gives 5,580/2,593, dropping the ternary
  gives 5,641/2,608 (both size-exact but raw/aligned worse).  `field_0x2c8`
  local removal and `field_0x2d0` cast removal are codegen-identical
  (5,244/2,575).

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13679
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
# raw=5259 aligned=2513 size=7072/7072 insn=2512/2552
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13679/`: `harness.py`,
`stmts.py`, `measure.py`, `sweep_adj.py`, `sweep_dist.py`, `move_block.py`,
`search_block.py`, `interleave.py`, `combos.py`, `hill.py`, `hill2.py`,
`diffruns.py`, `stores.py`, `compare_combos.py`, `adj_results.json`,
`dist_results.json`, `combo_results.json`, `block_results.json`,
`interleave_results.json`.
