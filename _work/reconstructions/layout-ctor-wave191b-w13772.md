# `Layout::Layout` C1/C2 — wave 191b / worker w13772

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, linked symbol size **7,072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave191b-w13772`, base master `403b587d1` (raw 5,355 / aligned 2,307,
the w13733 landing).

**Result: one size-exact aligned win landed (raw +268, aligned −188).**

| | before | after |
|---|---:|---:|
| raw | 5,355 | 5,623 |
| aligned | 2,307 | **2,119** |
| size | 7,072/7,072 | 7,072/7,072 |
| insn | 2,512/2,552 | 2,509/2,552 |
| `.text` diff | 74,737 | 75,005 (+268) |
| `.ARM.extab` diff | 561 | 561 (0) |
| `.ARM.exidx` / `.rodata` / `.data` | 0 / 0 / 0 | 0 / 0 / 0 |

`check` = RATCHET PASS: byte_exact 4508, linked_exact 4508, size-exact 4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.
`.rel.plt` byte-identical (29,392 B); all 27 section sizes unchanged.
Store image verified with `compare_combos.py` over all 32
`iPadHD/n9/iPad/iPadLarge/retinaDisplay` combinations: **bad combos 0 of 32**
(807 bytes / 202 words).

## The win

Ten independent-store swaps in the constructor body.  Every target is a
distinct, side-effect-free `Globals`-only store, so the store image is
unchanged for every flag combination.

```
s(11,20)    field_0x44 <-> field_0x34       # early SLP block
s(43,46)    field_0x9c <-> field_0x8c
s(98,99)    field_0x204 <-> field_0x154     # both before the 0x158 read of 0x154
s(9,10)     field_0x20_top <-> field_0x40
s(7,9)      field_0x24 <-> field_0x20_top
s(50,55)    field_0xa0 <-> oreTextYOffset
s(10,13)    field_0x40 <-> field_0x2c_rowHeight
s(75,78)    field_0x2ac_metricC <-> field_0xf0
s(173,175)  footerButtonOffset <-> field_0x280 if/else
s(6,9)      field_0x1c <-> field_0x20_top
```

The load-bearing edit is **`s(11,20)`** (alone: 5,762 / 2,194).  It is the
first swap found in the early `0x2c..0x78` SLP block that changes the emitted
`vst1` group order toward the original's `[34..40] [44..50] [54..60]
[64..70]`: with it the first four groups become `0x14, 0x4, 0x24, 0x34`
(original: `0x14, 0x4, 0x24, 0x34`), instead of `0x14, 0x4, 0x24, 0x38`.
The remaining three groups are emitted reversed (`0x64, 0x54, 0x44`), and no
single/paired swap found restores their order without shrinking the body.

## Measured candidates (all size 7,072 exact unless noted)

| variant | raw | aligned |
|---|---:|---:|
| base (master 403b587d1) | 5,355 | 2,307 |
| `s(11,20)` | 5,762 | 2,194 |
| `s(43,46)` | 5,448 | 2,275 |
| `s(93,95)` | 5,390 | 2,300 |
| `s(98,99)` | 5,420 | 2,299 |
| `s(17,28)` | 5,575 | 2,296 |
| `s(46,58)` | 5,831 | 2,283 |
| `s(132,135)` | 5,348 | 2,307 |
| `s(113,115)` | 5,351 | 2,307 |
| `s(88,92)` | 5,352 | 2,307 |
| `s(11,20)+s(43,46)` | 5,856 | 2,156 |
| `s(11,20)+s(43,46)+s(98,99)` | 5,629 | 2,142 |
| + `s(9,10)` | 5,635 | 2,139 |
| + `s(7,9)` | 5,613 | 2,131 |
| + `s(50,55)` | 5,627 | 2,122 |
| + `s(10,13)` | 5,627 | 2,121 |
| + `s(75,78)` | 5,627 | 2,120 |
| + `s(173,175)` | 5,628 | 2,119 |
| **landed: + `s(6,9)`** | **5,623** | **2,119** |

## Negative / not landed

* **`field_0x198` move (w13764).**  A full move sweep of units 110–130 to
  every position 100–140 (819 builds) reproduces the size break:
  `m(117,124)` (`field_0x198` after `field_0x1a8`) = 5,467 / 2,292 /
  **7,052** and `m(117,125)` = 5,466 / 2,313 / **7,052**.  No size-exact
  move in that window improves aligned below 2,307; `m(117,116)` is
  5,771 / 2,513.
* **Full ascending early block** (`0x2c..0x78`, the iOS source order):
  5,771 / 2,378 / **7,056** (16 B short), and worse aligned than the base.
  Partial ascending variants: 7,016–7,056, all short.
* **Single-swap sweeps on the landed point** (all size-exact candidates):
  adjacent (205), distance 2–5 (810), full pairs 0–30 (465), 31–104
  (2,701), 105–135 (435), 135–205 (2,415).  No swap improves on
  5,623 / 2,119; the only equal-aligned swaps are codegen-identical.
* **Raw-reducing swaps** `s(132,135)`, `s(113,115)`, `s(88,92)` cost +2 raw
  each on the landed point (they are −7/−4/−3 on the bare base), so they
  were dropped.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13772
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
# raw=5623 aligned=2119 size=7072/7072 insn=2509/2552
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13772/`: `harness.py`
(single-TU compile + gold link + `fn_diff`), `units.py` (ctor unit splitter),
`sweep.py`/`sweep2.py`/`sweep3.py`/`sweepm.py` (swap/move sweeps),
`ev.py`/`evseq.py`/`evkeep.py`, `cmpcombos.py` + patched `emu.py`
(32-combination store-image check), and the `*.json` measurement logs.

## Current-master recheck (2026-11-13, master 78dc73c0a)

The same ten-swap set was cherry-picked onto current master and re-measured:

| | current master | + w13772 ten swaps |
|---|---:|---:|
| raw | 5,494 | 5,673 |
| aligned | 2,095 | 2,107 |
| size | 7,072/7,072 | 7,072/7,072 |

It is a **+12 aligned regression** on current master, so the code change was
not landed. The worker's original −188 aligned win was measured against the
older `403b587d1` baseline (5,355 / 2,307). The load-bearing `s(11,20)` swap
alone may still be worth re-testing against the current store order.
