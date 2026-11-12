# `Layout::Layout` C1/C2 — wave 190 / worker w13458

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, linked symbol size **7,072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave190-w13458`, base master `83d52fe52`.

**Result: landed a size-exact raw win.  raw 5,249 -> 5,244 (-5), size
7,072/7,072, insn 2,520/2,552.**  `aligned` moved 2,725 -> 2,727 (not part of
the gate).  `RATCHET PASS`: byte_exact 4507/4524, linked_exact 4507/4524,
size-exact 4524/4524, imports 207, lint CLEAN.

## 1. The landed change

Move the three-statement block

```
field_0x1d8, field_0x1dc, field_0x1d4
```

from after `field_0x1cc` to **before `field_0x1d0`**:

```
... 0x1c8
    0x1d8  0x1dc  0x1d4      <-- moved here
    0x1d0  0x2c0  0x1c0  0x1cc
    0x1e4 ...
```

This is a pure source-order change (no expression edits).  It was found by an
exhaustive **late-region block-move** sweep: every contiguous block of length
2 or 3 with a start index in `130..201` moved to every other position
(28,488 valid candidates), scored with a compile-only 100-copy proxy.  Exactly
one candidate beat the baseline; it is the change above.  The proxy baseline
is raw 5,246 (3 below the linked 5,249); the winner scored 5,241 proxy =
5,244 linked, confirmed with a real `link_so` + `fn_diff`.

## 2. Negative cells measured this session (do not repeat)

Unless noted, size is 7,072 and the baseline is `(raw, size) = (5246, 7072)`
proxy.  None of these is a win.

### 2.1 `field_0x130/0x134/0x138` chain (unit 94)

* all 24 permutations of the `hd/n9/iPadLarge/retina` arm order: only the
  current `hd,n9,large,retina` order is neutral; every other order is worse
  (best non-identity `perm_hnrl` = 5,441, size 7,024).
* explicit `else if (iPad) {56,108,47} else {40,80,35}` arms: 5,398, 7,064.
* nested `else { if (iPad) {...} else {...} }`: 5,398, 7,064.
* default arm hoisted into a separate `if (!hd && !n9 && !large && !retina)`
  after the chain (both `&&` and `||` spellings): 6,176, 6,900.

### 2.2 Source-order reorders (whole-body)

* **iOS 1.1.4 source order** (from
  `layout_ctor/ios_source_order.md`), units sorted by first field: 6,018,
  size 6,864.
* same with unit 0 split into separate `0x4` / `0x14` statements: 5,973,
  size 6,880.
* iOS order restricted to the **HD-block prefix** (units 0..28): 5,841,
  size 7,032; prefix through `0x50`: 5,819, size 7,032.
* **Android store order** (`layout_ctor/order_default.txt`), units sorted by
  first field: 6,149, size 6,672.

### 2.3 `field_0x4` / `footerTextInset` split

Splitting unit 0 into two `if/else` chains and moving the `0x14` statement to
positions 1..11 (after `0xc`, `0x8`, `0x18`, `0x10`, ...): all worse; only
keeping it adjacent (position 0) is neutral.  Two independent ternary
statements: 5,254.

### 2.4 Pointer / flag hoists

* `volatile unsigned char *rd = &Globals::retinaDisplay;` at the top, every
  `Globals::retinaDisplay` -> `(*rd)` (unit 36 dropped): 6,531, size 7,308.
* same with `* const rd`: 6,531, size 7,308.
* `volatile unsigned char *hd = &Globals::iPadHD;` + `(*hd)` everywhere:
  12,205, size 12,460.
* both: 12,286, size 12,548.
* non-volatile `const unsigned char *hd` + `(*hd)`: 5,478, size 6,912;
  `const unsigned char * const hd`: **neutral** (folds back);
  `const unsigned char *rd` + `(*rd)`: 5,419, size 7,040;
  `const unsigned char * const rd`: 5,394, size 7,064; both: 5,464, 7,016.

### 2.5 `__builtin_expect`

* on the last (`retina`) / `iPad` / `n9` condition of the `0x2c8` and `0x2d0`
  ternary chains: **all neutral** (if-converted, no branch to influence).
* on `0x18c`'s `iPad` / `retina` / `iPadLarge` conditions: neutral except
  `expect(iPad,0)` 5,620/7,064 and `expect(iPad,1)` 5,837/7,040 (worse).
* on the `0x130` chain's `retina` / `large` / `n9` conditions: 5,834..5,840,
  size 7,040..7,044 (worse).

### 2.6 `field_0x2c0` (unit 131) early positions

Moved to every position `0..130`: best non-identity 5,836 (size 7,000);
no position keeps size 7,072.  (w13443 covered 120..225.)

### 2.7 `0x2c4/0x2c8/0x2cc/0x2d0/0x2d4/0x2d8` group

Reordering units `{29,31,32,33,34,35,36,37}` to the iOS/store order
(`2c4, t2c8, 2c8, 2cc, 2d0, 2d4, 2d8`) and 9 other permutations: best 5,852,
size 7,056; all worse.

### 2.8 `0x284..0x28c` group

Placing `{0x284,0x285,0x286,0x288,0x28c}` (and `+0x290`) consecutively at
every position 150..204: best 5,350, size 7,056; all worse.

## 3. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13458
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
# raw=5244 aligned=2727 size=7072/7072 insn=2520/2552
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13458/`: `units.py` (204-unit
splitter), `genproxy.py`/`buildproxy.sh`/`scorebatch.py` (100-copy
compile-only proxy), `vsearch.py` (batched variant runner), `blocklate.py` /
`blocklate.json` (the winning 28,488-candidate block sweep), `refine.py` /
`refine.json`, `t94.py`, `tios.py`, `tandroid.py`, `tsplit0.py`, `tret.py`,
`tptr.py`, `texp.py`, `t2c0.py`, `t2c.py`, `tgrp.py`.
