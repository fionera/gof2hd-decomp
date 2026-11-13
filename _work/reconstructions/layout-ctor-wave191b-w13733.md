# `Layout::Layout` C1/C2 — wave 191b / worker w13733

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, linked symbol size **7,072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave191b-w13733`, base master `dc50e1082` (raw 5,266 / aligned 2,442).

**Result: one size-exact aligned win landed (raw +89, aligned −135).**

| | before | after |
|---|---:|---:|
| raw | 5,266 | 5,355 |
| aligned | 2,442 | **2,307** |
| size | 7,072/7,072 | 7,072/7,072 |
| insn | 2,512/2,552 | 2,512/2,552 |
| `.text` diff | 72,647 | 72,736 (+89) |
| `.ARM.extab` diff | 564 | 564 |
| `.ARM.exidx` / `.rodata` | 0 / 0 | 0 / 0 |

`check` = RATCHET PASS: byte_exact 4508, linked_exact 4508, size-exact 4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.
`.rel.plt` byte-identical (29,392 B), all other sections unchanged.

## The win

Ten independent-store moves/swaps in the constructor body.  Every target is a
distinct, side-effect-free store, and the only RHS that reads `this`
(`field_0x158 = ... field_0x154 - 1`) is not moved relative to
`field_0x154`, so the store image is unchanged for every `Globals` flag
combination.

```
s(117,125)   field_0x1ac <-> field_0x18c          # 0x1ac/0x18c
s(114,115)   field_0x188 <-> field_0x194          # 0x188/0x194
m(145,151)   quickMenuIconGap -> after orbitInfoSystemY
m(147,150)   field_0x1ec -> after orbitInfoTextX
s(122,123)   field_0x198 <-> field_0x1a0
s(34,35)     field_0x2d0 <-> field_0x2cc
s(122,125)   field_0x198 <-> field_0x18c
s(103,109)   field_0x16c <-> field_0x17c
s(94,95)     field_0x108 <-> field_0x130/0x134/0x138 if/else
s(88,89)     field_0x128 <-> field_0x114_strip5c
```

The load-bearing edits are `s(117,125)` (alone: 5,319 / 2,375) and
`s(114,115)` (alone: 5,322 / 2,401).  `s(94,95)` is size-load-bearing: the
same set without it is 7,080 B.  The final combination is the best
aligned/raw point found in this session.

## Search

* Full orbitInfo-block swap sweep (2,674 pairs, units 143–156, distance > 6):
  no size-exact swap below aligned 2,442; best was 5,269 / 2,443.
* Full orbitInfo-block move sweep (2,856 moves): best 5,268 / 2,441
  (`field_0x1ec` after `orbitInfoTextX`).
* Base swap sweeps: units 80–130 (1,225 pairs) found `s(117,125)`
  (5,319 / 2,375) and `s(103,109)` (5,267 / 2,438); units 130–205
  (2,775 pairs) found `s(149,153)` (5,264 / 2,435).
* Adjacent-swap hill climbs on the accumulating candidate found
  `s(122,123)`, `s(122,125)`, `s(94,95)`, `s(88,89)`, `s(98,99)`.
* The full ascending reorder of the `0x180..0x1ac` block is 5,444 / 2,368
  but **7,080 B**; adding `s(94,95)` restores 7,072 and gives 5,442 / 2,340.
  The landed low-raw combination is better on both metrics.

Measured Pareto points (all size 7,072 exact):

| variant | raw | aligned |
|---|---:|---:|
| base (master) | 5,266 | 2,442 |
| `s(117,125)` | 5,319 | 2,375 |
| `s(114,115)` | 5,322 | 2,401 |
| `s(149,153)` | 5,264 | 2,435 |
| `s(114,115)+s(117,125)+c_1+s(122,123)+s(34,35)+s(122,125)+s(103,109)` | 5,324 | 2,331 |
| **landed: above + `s(94,95)` + `s(88,89)`** | **5,355** | **2,307** |
| landed + `s(98,99)` | 5,420 | 2,299 |
| full `0x180` ascending + `s(94,95)` + c_1 | 5,442 | 2,340 |

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13733
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
# raw=5355 aligned=2307 size=7072/7072 insn=2512/2552
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13733/`: `base.cpp`,
`units.py`, `mkperm.py`, `sweep.py`, `link.py`, `secdiff.py`, `blocks.py`,
`gen_swaps.py`, `gen_moves.py`, `candAA1.cpp` (landed), plus the
`*out.tsv` measurement logs.
