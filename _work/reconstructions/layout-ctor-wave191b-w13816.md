# `Layout::Layout` C1/C2 — wave 191b / worker w13816

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, linked symbol size **7,072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave191b-w13816`, base master `342cf7a86` (raw 5,578 / aligned 2,012 /
size 7,072).

**Result: three independent-store order swaps landed; size and aligned
unchanged, raw −6 / `.text` −6.**

| | before | after |
|---|---:|---:|
| raw | 5,578 | **5,572** |
| aligned | 2,012 | 2,012 |
| size | 7,072/7,072 | 7,072/7,072 |
| insn | 2,516/2,552 | 2,516/2,552 |
| `.text` diff | 75,919 | **75,913** |
| `.ARM.extab` diff | 594 | 594 |
| `.ARM.exidx` / `.rodata` | 0 / 0 | 0 / 0 |

`check` = RATCHET PASS: byte_exact 4508, linked_exact 4508, size-exact 4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.
`.rel.plt` byte-identical (29,392 B), all other sections unchanged.

## The win

Three swaps of distinct, side-effect-free `Globals`-only stores.  None of the
moved statements reads `this`, and the only ctor read dependency
(`field_0x158 = field_0x154 - 1`) is untouched.  Statement indices are the
`combo.py` unit numbers (base source order).

```
s(77,78)    field_0x2a4_metricA <-> field_0xf0
            -> field_0x2a4_metricA after field_0xf0
s(147,150)  field_0x200 <-> orbitInfoTextX block
            -> orbitInfoTextX block before the field_0x20c block,
               field_0x200 after field_0x214
s(196,197)  field_0x300 <-> field_0x1ec
            -> field_0x300 after field_0x1ec
```

Measured with the single-TU harness (compile the exact `Layout.cpp` command,
link with the exact gold command, `tools/fn_diff.py`):

| candidate | raw | aligned | size |
|---|---:|---:|---:|
| base | 5,578 | 2,012 | 7,072 |
| `s:147,150` | 5,575 | 2,012 | 7,072 |
| `s:196,197` | 5,576 | 2,012 | 7,072 |
| `s:77,78` | 5,577 | 2,012 | 7,072 |
| `s:78,79` | 5,577 | 2,012 | 7,072 |
| `s:141,147` | 5,577 | 2,012 | 7,072 |
| `s:147,150 s:196,197 s:77,78` | **5,572** | **2,012** | **7,072** |

The landed set is the best of the combinations of the five raw-reducing
singles (`out_combos.json`); `s:147,150 s:196,197` alone is 5,573 and
`s:147,150 s:196,197 s:77,78 s:78,79` is also 5,572.

## Sweeps / negative results

All sweeps started from the pre-change `base.cpp` (raw 5,578 / aligned 2,012),
used at most 3 compile workers, and required size 7,072/7,072.

* **Distance 1..6 store swaps** (`out_d1_6.json`): 1,215 candidates, 265
  size-exact, 106 at aligned 2,012.  Only the five singles above reduce raw
  while keeping aligned 2,012; no candidate reaches aligned < 2,012.
* **Distance 11..20 swaps** (`out_d11_20.json`): 1,905 candidates, 42
  size-exact, best aligned 2,013.
* **Distance 21..40 swaps** (`out_d21_40.json`): 3,510 candidates, 83
  size-exact, best aligned 2,026.
* **Single-unit moves, units 130..205 radius 15** (`out_mv130_205.json`):
  2,160 candidates, 387 size-exact, best aligned 2,012 at raw 5,576
  (`m:196,198`); only two moves beat raw 5,578 at aligned 2,012.
* **Raw-reducer × neutral pair sweep** (`out_pairs2.json`): 440 pairs (5
  raw-reducing singles × 88 aligned-2,012/raw-5,578 singles), 438 size-exact;
  best raw 5,575, none below 5,572.
* **Best-combo × neutral extension** (`out_pairs3.json`): 88 triples, all
  size-exact; none below raw 5,572.

No aligned improvement below 2,012 was found in any sweep, so this pass is a
raw/`.text` win with aligned unchanged, analogous to the earlier raw-only
Layout harvests.  `.ARM.extab` is unchanged at 594.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/scratch/w13816
python3 applyops.py cand.cpp s:147,150 s:196,197 s:77,78   # == cand_best.cpp
python3 harness.py cand.cpp                                # 5572 2012 7072
# full build:
cd /home/debian/GalaxyOnFire2/wt/w13816
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
python3 /home/debian/GalaxyOnFire2/scratch/w13816/secdiff.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so
(ulimit -v 12000000; cmake --build cmake-build-match --target check -j2)
```

Scratch artifacts: `base.cpp`, `cand_best.cpp`, `harness.py`, `applyops.py`,
`sweep.py`, `secdiff.py`, `out_d1_6.json`, `out_d11_20.json`,
`out_d21_40.json`, `out_mv130_205.json`, `out_combos.json`, `out_pairs2.json`,
`out_pairs3.json`.
