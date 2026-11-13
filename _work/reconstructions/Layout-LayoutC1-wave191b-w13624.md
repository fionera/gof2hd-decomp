# `Layout::Layout` C1/C2 — wave 191b / worker w13624

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, linked symbol size **7,072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave191b-w13624`, base master `46baf235a`.

**Result: one size-exact aligned win landed (raw +59, aligned −52).**

| | before | after |
|---|---:|---:|
| raw | 5,235 | 5,294 |
| aligned | 2,668 | **2,616** |
| size | 7,072/7,072 | 7,072/7,072 |
| insn | 2,517/2,552 | 2,516/2,552 |
| `.text` diff | 71,199 | 71,258 |
| `.ARM.extab` diff | 577 | 577 |
| `.ARM.exidx` / `.rodata` | 0 / 0 | 0 / 0 |

`check` = RATCHET PASS: byte_exact 4508, linked_exact 4508, size-exact 4524,
data 843/843, imports 207, lint `CLEAN ✓`, drift 0, parity 5370/5370.
`.rel.plt` byte-identical.

## The win

Two independent-field store swaps (semantically no-ops; both RHS are pure
constant/global ternaries):

```
field_0x188 (0x188)  <->  field_0x194 (0x194)
field_0x198 (0x198)  <->  field_0x1a0 (0x1a0)
```

The second swap reverts the w13591 `0x198/0x1a0` landing.  Alone it costs
+11 aligned, but on top of the `0x188/0x194` swap it is worth **−11 aligned at
zero raw cost**: the pair is 5,294 / 2,616 while `0x188/0x194` alone is
5,294 / 2,627.  Both swaps are independent stores, so the change is a
semantic no-op.

## Measured candidates (all real `link_so` builds; size 7,072/7,072 exact)

Adjacent-swap sweep of every depth-1 ctor statement on the landed variant:
72 size-exact swaps.  Non-codegen-identical singles:

| variant | raw | aligned |
|---|---:|---:|
| landed base | 5,235 | 2,668 |
| `0x188`/`0x194` swap | 5,294 | 2,627 |
| `0x26c`/`0x274` swap | 5,241 | 2,662 |
| `0x2d0`/`0x2cc` swap (w13579 order) | 5,239 | 2,663 |
| `0x30`/`0x3c` swap (32/33) | 5,262 | 2,666 |
| `0x198`/`0x1a0` swap (w13591 order) | 5,235 | 2,679 |

Pairs/triples on top of `0x188`/`0x194` (best aligned per raw tier):

| variant | raw | aligned |
|---|---:|---:|
| **`0x188/0x194` + `0x198/0x1a0` (landed)** | **5,294** | **2,616** |
| + `0x2d0`/`0x2cc` | 5,298 | 2,611 |
| + `0x26c`/`0x274` | 5,300 | 2,610 |
| + `0x2d0`/`0x2cc` + `0x26c`/`0x274` | 5,304 | 2,605 |
| + `0x30`/`0x3c` + `0x2d0`/`0x2cc` + `0x26c`/`0x274` | 5,331 | 2,603 |
| `0x34`/`0x3c` + `0x38`/`0x2c` + `0x188/0x194` + `0x198/0x1a0` | 5,342 | 2,598 |

All keep `.ARM.extab` at 577 B; the landed pick is the minimum-raw point with
aligned < 2,622 (the previous Pareto frontier point was 5,298 / 2,622).

## Fresh-angle results (negative / not landed)

* **`0x20`/`0x24`/`0x28` `strd`** (original `strd ip,ip,[r0,#32]`): making the
  `5 * 1.40625f` arm an int (`(int)(5 * 1.40625f)`) on any subset of the three
  fields is size-fatal: 6,688–7,028 B (the `vmov/vcvt/vcvt/vstr` round trip is
  load-bearing).  Confirms w13607.
* **`0x2c8`/`0x2d0`/`0x2cc` permutations** (6): only `2c8,2d0,2cc` (landed)
  and `2c8,2cc,2d0` (w13579) are size-exact; the other four are 6,892–6,968 B.
* **`0x2c8..0x2d8` 5-unit permutations** (120): 18 size-exact.  Moving the
  `0x2d4` if-block first reaches aligned 2,575 but raw 5,665.
* **Full ascending-offset reorder** of the ctor (203 units, if/else chains
  kept intact): 6,200 / 3,952 / **6,940 B**.  Ascending order is not the
  original source order; it shortens the size ridge.
* **`rewardBoxY2` `(int)(float)` round trip** (original stores it with
  `vstr s0,[r0,#1016]`): 5,340 / 2,840 / **7,092 B** (+20).  The round trip is
  real but needs a compensating −20 B elsewhere; not landed.
* **Pairing adjacent stores** (`0xfc/0x100`, `0xdc/0xe0`, `0xe4/0xe8`,
  `0x7c/0x80`, `0x138/0x13c`): every move is size-changing or raw-worse.
* **`0x2d0`/`0x2cc` + `0x188`/`0x194` + `0x198`/`0x1a0`** (5,298 / 2,611) is
  the best aligned point at raw ≤ 5,300; the landed pair is 4 raw B cheaper
  for 5 aligned instructions.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13624
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
# raw=5294 aligned=2616 size=7072/7072 insn=2516/2552
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13624/`: `base.cpp`, `meas.py`,
`measure.py`, `sweep1.py`, `beam.py`, `pair171.py`, `perm5.py`,
`asc_all2.py`, `secdiff.py`, `clusters.py`.
