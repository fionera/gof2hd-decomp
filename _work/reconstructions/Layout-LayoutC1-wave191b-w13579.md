# `Layout::Layout` C1/C2 — wave 191b / worker w13579

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, linked symbol size **7,072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave191b-w13579`, base master `5b422acf1`.

**Result: one size-exact aligned win landed (raw +7, aligned −37).**

| | before | after |
|---|---:|---:|
| raw | 5,232 | 5,239 |
| aligned | 2,711 | **2,674** |
| size | 7,072/7,072 | 7,072/7,072 |
| insn | 2,517/2,552 | 2,517/2,552 |

`check` = RATCHET PASS: byte_exact 4507, linked_exact 4507, data 843/843,
imports 207, lint CLEAN, drift 0.

## The win

Move the deferred `field_0x2c8` store (`this->field_0x2c8 =
(int)(float)t_field_0x2c8;`) from after the `field_0x2d8` assignment to
immediately before the `field_0x2cc` assignment.  The original's default-path
store order is `... 2c4 2c8 2cc 2d0 2d4 2d8 ...` (see
`layout_ctor/order_default.txt`); this places the store in that order.  The
store is still after the `t_field_0x2c8` declaration, so it is a semantic
no-op.  Size stays exactly 7,072 B; the instruction count is unchanged; the
instruction-alignment diff drops by 37.

## Diff-cluster map of the 5,232-byte residual (base)

Contiguous differing-byte clusters, by section (same-address raw diff):

| region | differing / size |
|---|---:|
| HD arm `d1028–d126c` | 334 / 580 |
| n9 arm `d126c–d1570` | 346 / 772 |
| default arm `d1570–d21e2` | 2,658 / 3,186 |
| late block `d21e2–d28d8` | 1,635 / 1,782 |
| literal pool `d28d8–d2bc8` | 259 / 752 |

Largest contiguous clusters: `d2072–d2171` (255 B, default-arm
`0x1a4/0x1a8/0x1ac/0x1e0` + SLP vector groups), `d2830–d28dd` (173 B, code
end), `d1239–d12e0` (167 B, HD arm), `d1b80–d1c24` (164 B, n9 arm),
`d2316–d23af` (153 B, late block).  The default+late blocks carry 82 % of the
residual; both are dominated by SLP/`strd` scheduling, not by a single
reorderable store.

## Negative results this session (measured on the 5,232 base)

All measured with `link_so -j2` + `tools/fn_diff.py`; size-exact unless noted.

* **Full move sweep of the `0x180..0x1e8` tail units** (812 moves, unit i
  before unit j): no size-exact raw win.
* **Adjacent swaps of all 265 top-level units** (265 builds): none beat
  5,232.
* **Restate sweep**: duplicating each of the 225 `this->` statements
  immediately after itself and immediately before itself: no win.
* **iOS late order** (`0x1e4..0x320` in `ios_source_order.md` order):
  6,117 / 6,912.  **Default-path store order** for the same window:
  6,193 / 6,760.  Both shrink code; neither is size-exact or raw-better.
* **`0x180..0x1e8` in default store order**: 6,094 / 6,988; in memory order
  with/without the `0x190` block: 5,724 / 6,928 and 5,858 / 7,072 (size-exact,
  raw worse).
* **`eventBannerScale` (0x1e0) moved next to `0x18c/0x1ac/0x1a0/0x1b8/0x1d0/
  0x1dc`**: 5,956–5,979, all size-fatal.
* **`field_0x2c8` store before `0x2cc`** is the landed win (5,239 / 2,674).
  Moving it before `0x2c4` is 5,448 / 7,048; after `0x2cc`/`0x2d0` is
  6,207/6,271 and size-fatal.  Removing the `(int)(float)` round-trip is
  5,426 / 7,064.
* **`field_0x2d8` moved next to the `0x2d4` chain**: 6,268–6,271 / 6,892
  (the original's shared-tail `strd r4,r6,[r0,#724]` is not reachable this
  way without losing the HD-arm `str.w [r0,#728]`).
* **Local-lifetime reshapes**: `const bool retina/hd/n9` locals (with all
  uses rewritten) are 5,983–6,932 and size-fatal; hoisting `int v`,
  `int t_field_0x2c8` or `volatile unsigned char *rd` to the top of the ctor
  is 5,902–6,389 and size-fatal (moving `rd` alone is codegen-identical).
* **`__builtin_expect` on the opening `if (Globals::iPadHD)`**: 6,185–6,285,
  size-fatal.
* **`(int)(float)` added to `field_0x1e4` / `field_0x158` / `rewardBoxY2`**:
  5,337–5,863, size-fatal; adding it to `field_0x170`, `scrollBarInset` or
  `field_0x138` is codegen-identical (their original `vstr` stores are
  scheduler artifacts, not source casts).
* **Restating each brace-less if/else chain as a ternary after the chain**:
  `0x7c`, `orbitInfoTextX`, `0x280`, `rewardBoxY2` are codegen-identical;
  `0x20c` (5,519 / 7,088, aligned 2,663), `orbitInfoStationY`
  (5,327 / 7,048, aligned 2,662), `0x218` (5,403 / 7,088) improve aligned but
  are size-fatal / raw-worse.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13579
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
# raw=5239 aligned=2674 size=7072/7072 insn=2517/2552
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13579/`: `clusters.py`,
`cluster_kind.py`, `align.py`/`align.txt`, `units.py`, `measure2.py`,
`move.py`, `reorder2.py`, `multimove.py`, `sweep.py`, `adjswap.py`,
`restate.py`, `locals.py`, `lifetimes.py`, `chainrestate.py`, `casts2.py`,
`rand.py`, `layout_ctor/` (patched `emu.py` path).
