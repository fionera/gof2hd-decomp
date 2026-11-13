# `Layout::Layout` C1/C2 — wave 191b / worker w13621

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, linked symbol size **7,072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave191b-w13621`, base master `7301219a1`.

**Result: one size-exact aligned win landed (three independent store swaps).**

| | before | after |
|---|---:|---:|
| raw | 5,232 | 5,241 |
| aligned | 2,700 | **2,637** |
| size | 7,072/7,072 | 7,072/7,072 |
| insn | 2,517/2,552 | 2,517/2,552 |

`check` = RATCHET PASS: byte_exact 4508, linked_exact 4508, data 843/843,
imports 207, lint CLEAN, drift 0.  `.text` 71,196, `.ARM.extab` 577,
`.ARM.exidx` 0, `.rodata` 0 all unchanged; `.rel.plt` byte-identical.

## The win

Three independent (semantically no-op) swaps of adjacent-region store
statements in the late block, each moving a field toward its original
source/dynamic order:

1. `field_0x3c` <-> `field_0x2c_rowHeight` (lines 36/38 of the base).
2. `field_0x140` <-> `field_0x1ec` (lines 208/214).
3. `field_0x25c` <-> `field_0x248` (lines 258/260).

The third swap is the load-bearing one: it moves `field_0x248` ahead of
`field_0x258`/`field_0x25c`, matching the original's default-path store
order (`... 244 248 24c 250 254 258 25c 260 ...`, `layout_ctor/order_default.txt`).
Alone it gives raw 5,236 / aligned 2,668.  The other two swaps each add a
further aligned reduction (raw +4/+5) without changing size or sections.
All three together: raw 5,241 / aligned 2,637.

Semantics verified with `_work/reconstructions/layout_ctor/compare_combos.py`
(all 32 flag combinations: `bad combos 0 of 32`, store image identical).

## Diff-cluster map of the residual

Fresh build of the base: raw 5,232, aligned 2,700, size 7,072.  The named
clusters from the handoff (+0x0b3c default arm 0x84..0x120, +0x1798 tail,
+0x104a default arm 0x1a4..0x1e8, +0x141e late block 0x24c..0x27c) are
confirmed.  The landed swaps hit the +0x141e late block and the early
0x2c..0x44 block.

## Negative results this session (measured on the 5,232 base)

All measured with `link_so -j2` + `tools/fn_diff.py`; size-exact unless noted.

* **Full adjacent-swap sweep** of every simple `this->...;` pair in lines
  20..340 (two passes): no size-exact raw win.  The only size-exact aligned
  improvements were the semantically wrong `orbitInfoTextX` body swaps
  (`229<->231`: raw 5,232 / aligned 2,694; it moves `= 12` out of the
  retina branch).
* **Full distance-2/3/4/5 swap sweeps** over lines 20..340: best valid
  size-exact results were `36<->38` (5,236 / 2,671), `33<->36`
  (5,236 / 2,684), `258<->260` (5,236 / 2,668).  `227<->231` and
  `229<->231` are raw-neutral but semantically wrong.
* **Single-statement move sweeps** for `field_0x20_top`, `field_0x24`,
  `buttonInsetX`, `field_0xa0`, `field_0xa4`, `ringRadiusScale`,
  `field_0x8c`, `field_0x90`, `field_0x9c`, `field_0x248`, `field_0x258`,
  `field_0x25c`: only no-op positions are size-exact.  Nearest misses:
  `field_0xa4` after `field_0xa0` = 5,456 / 2,708 / 7,064 (8 short);
  `ringRadiusScale` into the early block = 5,452 / 2,619 / 7,044 (28 short).
* **600 random single-statement moves** (distance <= 40): no size-exact raw
  win; best valid aligned 2,662 at raw 5,238.
* **Block moves** of the `orbitInfoTextX` and `field_0x260` if/else chains:
  size-fatal (7,088) except no-op positions.
* **`if/else` -> ternary** for `orbitInfoTextX` (7,088), `orbitInfoStationY`
  (7,048), `field_0x218` (7,048).
* **Negate-and-swap** of the last two arms of every flag chain
  (`orbitInfoTextX`, `orbitInfoStationY`, `field_0x218`, `field_0x20c`,
  `field_0x260`, `field_0x280`, `rewardBoxY2`): codegen-identical
  (5,232 / 2,700 / 7,072) — clang canonicalises the negation.
* **Chained assignment / shared local for `field_0x20_top` and
  `field_0x24`** (they are identical in all arms): emits the original's
  `strd ip,ip,[r0,#32]` exactly, but drops the default arm's
  `vmov/vcvt/vcvt/vstr` float round trip and shrinks the body to 7,040
  (raw 5,676 / aligned 2,696).  `(int)`, `(int)(float)`, `float` local and
  `if (iPadHD) { ... } else { chained }` forms are all 7,040.  A +32 B
  compensation was not found; the round trip is load-bearing for the size
  ridge.
* **Grouping merge candidates** `field_0xa4` after `field_0xa0`,
  `oreLabelYOffset` after `hudInset`, `field_0x2b0/0x2b4/0x2b8` together:
  each induces the expected extra `strd`/`stmia` but shrinks the body
  (7,064 / 7,016 / 7,000).  The original's larger literal pool (+132 B vs
  ours) is the other half of the size ridge.
* **`float t_field_0x2c8`** (drop the round trip): 5,579 / 2,692 / 7,080.
  **`const unsigned char *rd`**: 5,381 / 2,705 / 7,064.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13621
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
# raw=5241 aligned=2637 size=7072/7072 insn=2517/2552
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13621/`: `measure.sh`,
`sweep_move.py`, `sweep_swap.py`, `sweep_swap2.py`, `sweep_swap_file.py`,
`sweep_swap2_file.py`, `sweep_moves_region.py`, `moveblock.py`,
`reorder_range.py`, `rndmove.py`, `cc.py` (patched `compare_combos.py`),
`Layout.base.cpp`, `T5.cpp`, `S.cpp`.
