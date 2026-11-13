# `Layout::Layout` C1/C2 — wave 191b / worker w13607

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, linked symbol size **7,072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave191b-w13607`, base master `855fc0820`.

**Result: one size-exact aligned win landed (raw +3, aligned −32).**

| | before | after |
|---|---:|---:|
| raw | 5,232 | 5,235 |
| aligned | 2,700 | **2,668** |
| size | 7,072/7,072 | 7,072/7,072 |
| insn | 2,517/2,552 | 2,517/2,552 |
| `.text` diff | 71,199 | 71,202 |
| `.ARM.extab` diff | 577 | 577 |

`check` = RATCHET PASS: byte_exact 4508, linked_exact 4508, size-exact 4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.
`.rel.plt` byte-identical.

## The win

Move the deferred `field_0x2c8` store (`this->field_0x2c8 =
(int)(float)t_field_0x2c8;`) up between the `t_field_0x2c8` declaration and
`field_0x2d0`, and move `field_0x2cc` after `field_0x2d0`:

```
before:  t=...; 2cc; 2d0; 2d8; 2c8; rd...
after:   t=...; 2c8; 2d0; 2cc; 2d8; rd...
```

All three stores are independent fields, so this is semantically a no-op.
Verified with the store-image emulator (`valall.py`, all 32 combinations of
`iPadHD/n9/iPad/iPadLarge/retinaDisplay`): 0 configs differ.

This supersedes the w13579 note (`field_0x2c8` before `0x2cc`, raw 5,239 /
aligned 2,674 on its base).  On the wave-191b base (which already contains the
w13573 `0x198/0x1a0` swap) the `0x2cc`/`0x2d0` swap is worth another 4 raw
bytes for 5 aligned instructions:

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base | 5,232 | 2,700 | 7,072 |
| 2c8 before 2cc only (w13579) | 5,239 | 2,663 | 7,072 |
| **2c8 before 2d0, 2cc after 2d0 (landed)** | **5,235** | **2,668** | **7,072** |

## Measured candidates (all real `link_so` builds, size-exact unless noted)

Adjacent-swap sweep of all 85 size-exact pairs on top of the w13579 variant
(`as_*.cpp`): best raw 5,235 (the landed variant); best aligned 2,622
(`0x188`/`0x194` swap, raw 5,298).  Pareto frontier:

| variant | raw | aligned | size |
|---|---:|---:|---:|
| landed | 5,235 | 2,668 | 7,072 |
| w13579 2c8/2cc | 5,239 | 2,663 | 7,072 |
| `0x26c`/`0x274` swap | 5,245 | 2,657 | 7,072 |
| `0x134`/`0x138` retina-arm swap | 5,254 | 2,659 | 7,072 |
| `0x188`/`0x194` swap | 5,298 | 2,622 | 7,072 |
| `0x188`/`0x194` + revert 0x198/0x1a0 | 5,298 | 2,611 | 7,072 |

The aligned-favouring variants all pay a large raw cost (`.text` +66 for the
`0x188` swap), so the landed variant is the best raw/aligned compromise.

## Negative / neutral results this session

* **Full adjacent-swap sweep on the wave-191b base** (85 size-exact pairs):
  no candidate with raw ≤ 5,232 and aligned < 2,700.
* **1,500 random single/2/3-line moves and swaps on the base** (object-size
  prefilter): no candidate below the base raw proxy (5,229 object / 5,232
  linked).  The raw landscape is flat at 5,232.
* **1,500 random moves on the landed variant**: all size-exact survivors are
  codegen-identical to the landed variant (5,235 / 2,668).
* **`0x20`/`0x24`/`0x28` permutations** (all 6): `0x20,0x24,0x28` is
  size-exact but 5,676 raw (CSEs 0x20/0x24 to one register but schedules them
  28 B apart, so no `strd`); `0x20,0x28,0x24` is 5,254 / 2,692 / 7,072.
  Chained assignment `field_0x24 = field_0x20_top = ...` and a shared local
  `int topPair` shrink to 7,040 B (fatal).
* **Float-arm → int 7 on all three of `0x20_top`/`0x24`/`buttonInsetX`**:
  6,211 / 3,364 / **6,796** — the `vmov/vcvt/vcvt/vstr` round trip is
  load-bearing for the 7,072 size ridge.
* **Ascending-offset reorder of the whole `0x84..0x124` block**: 6,013 /
  2,787 / 6,848.  **Original-store-order reorder** of the same block:
  6,210 / 2,711 / 6,644.
* **`0xb0..0xbc` and `0xc0..0xcc` group permutations** (24 each): sizes
  6,904–7,088; only the combination
  `b4,bc,b0,b8` + `c8,c4,cc,c0` is size-exact (5,391 / 2,695), raw worse
  than the landed variant.
* **`0x2d4` chain before `0x2d8`** (toward the original `strd r4,r6,[r0,#724]`):
  5,388 / 2,688 on top of the landed variant (size-exact, raw worse).
* **`field_0x184` restate moves**: 5,978–6,025, size 7,000–7,076.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13607
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
# raw=5235 aligned=2668 size=7072/7072 insn=2517/2552
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13607/`: `base.cpp`, `c_2c8.cpp`,
`as_56.cpp` (landed), `as_*.cpp`, `as2dir/`, `as3dir/`, `frdir/`, `fr2dir/`,
`measure.sh`, `batch.py`, `measure_many.py`, `fastsearch.py`, `fastraw.py`,
`storeorder.py`, `clusters.py`, `secdiff.py`, `valall.py`, `emu.py`.
