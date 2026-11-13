# `Layout::Layout` C1/C2 — wave 191b / worker w13791

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, linked symbol size **7,072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave191b-w13791`, base master `f2ee89f4e`.

**Task.** w13772's ten-store swap set regressed current master by +12 aligned
(5,673 / 2,107 vs 5,494 / 2,095).  Re-test subsets of that set against the
current store order instead of landing the full set.

**Result: one size-exact aligned win landed — the singleton `s(173,175)`
(`field_0x280` if/else before `footerButtonOffset`).**

| | before | after |
|---|---:|---:|
| raw | 5,494 | 5,496 |
| aligned | 2,095 | **2,094** |
| size | 7,072/7,072 | 7,072/7,072 |
| insn | 2,512/2,552 | 2,512/2,552 |
| `.text` diff | 75,749 | 75,751 (+2) |
| `.ARM.extab` diff | 590 | 590 (0) |
| `.ARM.exidx` / `.rodata` | 0 / 0 | 0 / 0 |

`check` = RATCHET PASS: byte_exact 4508, linked_exact 4508, size-exact 4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.
`.rel.plt` unchanged (no new imports).  Store image verified with
`cmpcombos.py` over all 32 `iPadHD/n9/iPad/iPadLarge/retinaDisplay`
combinations: **bad combos 0 of 32** (807 bytes / 202 words).  The two
swapped units write distinct fields (`footerButtonOffset`, `field_0x280`)
with no reads between them; `field_0x285 = 1` stays between them.

## The ten w13772 swaps, re-indexed on current master

w13772's indices were relative to the old `403b587d1` store order; current
master already contains several of those moves, so the same swaps map to
different unit indices.  Names are the stable key.

| w13772 | field pair | current |
|---|---|---|
| `s(11,20)` | `field_0x34` <-> `field_0x44` | `s(13,20)` |
| `s(43,46)` | `field_0x8c` <-> `field_0x9c` | `s(43,46)` |
| `s(98,99)` | `field_0x154` <-> `field_0x204` | `s(99,100)` |
| `s(9,10)` | `field_0x20_top` <-> `field_0x40` | `s(7,14)` |
| `s(7,9)` | `field_0x20_top` <-> `field_0x24` | `s(7,9)` |
| `s(50,55)` | `field_0xa0` <-> `oreTextYOffset` | `s(50,56)` |
| `s(10,13)` | `field_0x2c_rowHeight` <-> `field_0x40` | `s(11,14)` |
| `s(75,78)` | `field_0x2ac_metricC` <-> `field_0xf0` | `s(75,78)` |
| `s(173,175)` | `footerButtonOffset` <-> `field_0x280` | `s(173,175)` |
| `s(6,9)` | `field_0x1c` <-> `field_0x20_top` | `s(6,7)` |

## Measured single swaps on current master (base 5,494 / 2,095)

| swap | raw | aligned | size |
|---|---:|---:|---:|
| **`s(173,175)`** | **5,496** | **2,094** | **7,072** |
| `s(11,14)` | 5,494 | 2,095 | 7,072 |
| `s(6,7)` | 5,496 | 2,095 | 7,072 |
| `s(75,78)` | 5,494 | 2,098 | 7,072 |
| `s(7,9)` | 5,758 | 2,122 | 7,072 |
| `s(43,46)` | 5,491 | 2,126 | 7,072 |
| `s(13,20)` | 5,510 | 2,249 | 7,072 |
| `s(99,100)` | 5,625 | 2,100 | 7,056 (16 short) |
| `s(50,56)` | 5,932 | 2,208 | 7,040 (32 short) |
| `s(7,14)` | 5,948 | 2,843 | 6,944 (128 short) |

The load-bearing `s(11,20)` of the old note is **`s(13,20)`** on current
master and is a regression (2,249).  The only aligned win is `s(173,175)`.

## Exhaustive subset search

Full power set of the ten swaps: **1,023 candidates, 95 size-exact**.  Best
aligned is **2,094**, reached by `s(173,175)` alone and by
`s(173,175)+s(11,14)` / `+s(6,7)` (raw 5,496 / 5,498).  No subset reaches
2,093 or below.  The full ten-set is 5,673 / 2,107, confirming the +12
regression.

Local neighbourhood (every swap of unit 173 or 175 with any unit, plus every
pair in 160..190): **815 candidates, 70 size-exact**; best is again
`s(173,175)` at 5,496 / 2,094 (`s(174,175)` ties aligned at 5,497).

## Follow-up not landed (outside the ten-swap set)

A greedy single-swap search on top of `s(173,175)` (adjacent + distance <= 6,
plus the 50..80 / 140..160 / 165..185 neighbourhoods) found further
independent-store swaps that keep size 7,072:

| swaps added | raw | aligned |
|---|---:|---:|
| `s(173,175)` | 5,496 | 2,094 |
| `+ s(149,152)` (`field_0x1fc` <-> `field_0x214`) | 5,554 | 2,089 |
| `+ s(58,62)` (`field_0xcc` <-> `ringNearMidThreshold`) | 5,564 | 2,088 |
| `+ s(58,64)` (`field_0xcc` <-> `progressBarYBase`) | 5,563 | 2,073 |
| `+ s(192,197)` (`field_0x300` <-> `field_0x1ec`) | 5,582 | 2,061 |

These are all distinct-field, read-free store swaps, but they were not
validated against the 32 `iPadHD/n9/iPad/iPadLarge/retinaDisplay` combinations
and are left for a later pass; only the singleton `s(173,175)` is landed here.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13791
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
# raw=5496 aligned=2094 size=7072/7072 insn=2512/2552
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13791/`: `harness.py` (single-TU
compile + gold link + `fn_diff`), `units.py` (ctor unit splitter), `named.py`
(name-indexed swap measurement), `powerset.py`, `local.py`, `onland.py`,
`greedy.py`, and the `*.json` measurement logs.
