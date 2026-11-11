# `Layout::Layout` C1/C2 — wave 176 / worker w12008

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original
vaddr `0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Baseline (branch `wave176-w12008`, base master `b21088985`, post-wave-175
tree): raw **5,620**, aligned **2,289**, 7,072 B, 2,518 / 2,552 instructions.
`reload()` is byte-exact (raw 0, 1,064 B).

**Exit: no code change landed.**  A full re-run of the store-order / ternary /
field-order sweep on the post-wave-175 base found no size-7,072 state with
raw < 5,620.  The tree is left at the base.  The best size-exact alternative
is the wave-175 `0x2d0`-after-`0x2cc` swap at raw 5,624 (+4).

## 1. Region budget (unchanged from wave 175)

Boundaries from `objdump -M force-thumb`: prologue `d1028..d1042`; iPadHD
block `d1042..` first `b.w` target (`d126c` orig / `d127a` ours); merge = the
prologue `beq.w` target (`d21e2` orig / `d22a6` ours); code end = last
`b`/`pop` before the final literal-pool island (`d2852` orig / `d28ce` ours).

| region | original | ours | delta |
|---|---:|---:|---:|
| prologue | 26 | 26 | 0 |
| iPadHD block | 554 | 568 | **+14** |
| non-iPadHD first half | 3,958 | 4,140 | **+182** |
| tail + T block | 1,648 | 1,578 | **-70** |
| code total | 6,186 | 6,312 | **+126** |
| literal pool | 886 (221 w) | 760 (190 w) | **-126** |
| function | 7,072 | 7,072 | 0 |

The size is a three-way compensation: **+196 B of first-half code** is paid
for by **-70 B of tail/T** and **-126 B of literal pool**.  The merge point is
addressed 196 B late, which is why every faithful-tail reconstruction (which
matches the original's tail bytes but sits at the wrong address) measures
worse raw.

## 2. Store-set and store-count comparison (full parse)

A dataflow store-map parse (immediate `str`/`strd`/`vstr`, `vst1.32` 4-word
groups, and `stmia` with a resolved `r0+imm` base) confirms:

* **201 distinct field offsets in both binaries, identical sets.**  The
  ctor/reload split is already correct: the ctor writes exactly the same
  fields as the original, and `reload()` is byte-exact, so no field needs to
  migrate between the two functions.
* **Original 575 stores, ours 602** — we emit *more* stores, yet *less* code.
  Ours are merged into flat ternaries; the original duplicates them per
  device branch (8 `stmia` groups vs our 3, and extra `strd` pairs).

Offsets where the original has more stores (branch duplication):
`0x140, 0x154, 0x15c, 0x16c, 0x178, 0x180, 0x184, 0x188, 0x18c, 0x190,
0x198, 0x2cc, 0x3e8, 0x3ec, 0x3f4`.
Offsets where ours has more stores (flat-ternary sinking):
`0x98, 0x9c, 0xa0, 0xac, 0xd0, 0xd4, 0xd8, 0x118..0x124, 0x148, 0x158,
0x160, 0x170, 0x1a8, 0x1f0, 0x200, 0x20c, 0x218, 0x220, 0x28c, 0x294,
0x29c, 0x2b0, 0x2b4, 0x2b8, 0x2bc, 0x2c0, 0x2c4, 0x2c8, 0x2d0, 0x2fc,
0x300, 0x308, 0x3fc`.

The literal-pool deficit is the mirror image: the original materialises ~31
more pool words (small ints `0x0, 0x11, 0x14, 0x15, 0x19, 0x25, 0x46, 0x63,
0x64, 0x69, 0x6e, 0x8c, 0x91, 0x92, 0xa0, 0xaa, 0xb4, 0xbd, 0xc6, 0xd2,
0xdc, 0xf3, 0x10e, 0x12c, 0x154, 0x15a, 0x168, 0x1be, 0x1c2, 0x258, 0x320`
and floats `0x3ea66666 (0.325f), 0x3f266666 (0.65f), 0x42480000 (50f),
0x42980000 (76f), 0x42c80000 (100f), 0x43100000 (144f), 0x438ca000 (281.25f)`)
because its first-half schedule keeps more values alive and reloads them via
`ldr [pc]` instead of `movw`/`movs`.

## 3. Searches performed (all real `link_so` + `tools/fn_diff.py`)

Size had to stay exactly 7,072 B.  Every measurement below is the C1 alias
(`_ZN6LayoutC1Ev`); C2 is identical.

### 3.1 Adjacent statement swaps
All **203 adjacent pairs** of the 204 constructor units (the splitter treats
each `if/else` chain as one unit; `int t_field_0x2c8`, `int v` and the
`volatile unsigned char *rd` declaration are units too).  No swap lowers raw.
Best ties at 5,620: `orbitInfoSystemY <-> field_0x234` (aligned 2,288),
`field_0x24 <-> buttonInsetX`, `field_0x2d8 <-> rd`, several `0x2a0`/`0x2a8`
neighbours, `0x16c <-> 0x178`, `0x1b0 <-> 0x1bc`, `0x1c4 <-> 0x1c8`, ...
All other size-exact swaps are >= 5,620.

### 3.2 The `field_0x2c4 / 0x98 / 0x2c8 / 0x2d0 / 0x2cc` group
All **360 valid permutations** (temporary before its use) of the six units
`0x2c4, 0x98, t_field_0x2c8, 0x2c8, 0x2d0, 0x2cc`.  Only 8 states are
size-exact:

| order (unit indices) | raw | aligned |
|---|---:|---:|
| `28,29,30,31,32,33` (base) | **5,620** | 2,289 |
| `28,29,32,30,31,33` | 5,620 | 2,290 |
| `28,29,30,31,33,32` (`0x2d0` after `0x2cc`, wave-175 `F`) | 5,624 | **2,284** |
| `28,29,32,33,30,31` | 5,639 | 2,287 |
| `30,31,29,32,28,33` | 5,665 | 2,311 |
| `30,31,28,32,33,29` | 5,765 | 2,380 |
| `30,31,28,33,32,29` | 5,765 | 2,391 |
| `29,30,31,33,28,32` | 5,805 | 2,355 |

The original's store order here is `0x2c8, 0x2cc, 0x2d0` (see the Ghidra
decompile LAB_e1814/LAB_e1860 and the original's `d1820/d182c/d1882`), i.e.
the `F` order, which costs +4 raw at constant size.

### 3.3 Ternary-shape variants
Each was tested as a source replacement; all are code-neutral or worse:

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base | **5,620** | **2,289** | 7,072 |
| `0x2c8` direct `(int)(float)(...)` (no temp) | 5,620 | 2,289 | 7,072 |
| `0x2c8` direct with float iPadHD branch | 5,620 | 2,289 | 7,072 |
| `0x2d0` via `int` temp (like `0x2c8`) | 5,620 | 2,289 | 7,072 |
| `0x2cc` with `(int)(float)` cast | 5,620 | 2,289 | 7,072 |
| `0x160` without the inner `(int)` | 5,620 | 2,289 | 7,072 |
| `0x4`/`footerTextInset` order swapped in the top `if` | 5,620 | 2,289 | 7,072 |
| `0x2d0` triple cast `(int)(float)(int)` | 5,824 | 2,316 | 7,112 |
| `0x2d0` after `0x2cc` (statement swap) | 5,624 | 2,284 | 7,072 |
| shared `int u2cc` branch tree for `0x2cc`/`0x2d0` | 5,624 | 2,284 | 7,072 |
| shared `float u2cc` branch tree | 5,782 | 2,345 | 7,088 |
| shared `int u2c4` branch tree for `0x2c4`/`0x2c8` | 5,765 | 2,319 | 7,048 |
| remove the `volatile rd` for `0x2d4` | 5,731 | 2,295 | 7,064 |
| `0x10c` without the inner `(int)` | 5,739 | 2,341 | 7,064 |
| `0x150` without the `(int)(float)` | 5,727 | 2,290 | 7,064 |

### 3.4 The wave-175 A–G feature matrix (128 states)
Re-ran the 7-feature exhaustive search on the new base (A = `0x94` 3-way
if/else, B = `0x140` 4-way, C = `rewardBoxWidth` 5-way, D = `rewardBoxY`
5-way, E = `0x2cc` 5-way, F = `0x2d0` after `0x2cc`, G = `0x2d0` triple
cast).  Only **two** states are size-exact: base (5,620) and `F` (5,624).
Every if/else shape changes the size (7,032–7,220 B) and raises raw.

### 3.5 Targeted relocations
Single-unit moves that keep size 7,072 and their raw:

| move | raw | aligned |
|---|---:|---:|
| `0x98` to after `0x2d8` | 5,778 | 2,321 |
| `0x98` to before `0x9c` | 5,769 | 2,304 |
| `0x94`/`0x98` to `0x9c` | 5,923 | 2,597 (size 7,008) |
| `0x288` before `0x28c` | 5,740 | 2,278 (size 7,064) |
| `0x290` after `0x28c` | 5,902 | 2,534 (size 7,032) |
| `0x1f0`/`0x1f4` swap | 5,692 | 2,284 (size 7,080) |
| `0x2bc` before `0x3fc` | 5,971 | 2,585 (size 6,968) |
| `0x2bc/0x200/0x3fc` before `0x1f4` (T block) | 5,985 | 2,248 (size 6,984) |
| `0x2bc` alone before `0x1f4` | 5,954 | 2,577 (size 6,968) |
| `0x200` alone before `0x1f4` | 5,719 | 2,343 (size 7,048) |
| `0x3fc` alone before `0x1f4` | 5,699 | 2,299 (size 7,088) |
| `0x74,0x78` first then ascending `0x14..0x70` | 5,911 | 3,947 (size 6,972) |
| ascending `0x14..0x70` | 5,981 | 2,653 (size 6,948) |
| ascending `0x150..0x17c` | 5,725 | 2,299 (size 7,056) |
| ascending `0x150..0x17c` + `0x148` before | 5,787 | 2,275 (size 7,068) |

### 3.6 Random swap/move searches
Real-build searches accepting only size-7,072 states, with a plateau walk:

* tail region (units 130–204), 400 iterations seed 1: no improvement;
* tail region (units 120–204), 700 iterations seed 7: no improvement;
* early region (units 0–40), 400 iterations seed 2: no improvement;
* global (all 204 units), 300 iterations seed 1: no improvement.

The tail neighbourhood is saturated; the wave-175 `0x214 <-> 0x200` win is
still the only tail swap below the pre-wave-175 base.

### 3.7 Reordering to the original's store order
The original's non-iPadHD first-half store order (first occurrence per
offset, from the disassembly) is roughly ascending:
`74 78 14 18 1c 20 4 8 c 10 24 28 2c 30 34 38 3c 40 44 48 4c 50 54 58 5c
60 64 68 6c 70 2c0 2c4 2c8 2cc 2d0 2d4 2d8 7c 80 84 88 8c 90 94 98 9c ac
a8 a0 a4 d0 d4 d8 dc e0 e4 e8 fc 100 104 2b4 2b8 b0 b4 b8 bc c0 c4 c8 cc
ec f0 f4 f8 294 298 29c 2a0 2a4 2a8 2ac 2b0 118 11c 120 124 108 10c 110
114 128 12c 130 134 140 144 148 14c 138 13c 150 154 158 15c 160 164 168
17c 16c 170 174 178 180 184 188 18c 190 194 198 19c 1a0 1a4 1a8 1ac 1e0
1c0 1c4 1c8 1cc 1b0 1b4 1b8 1bc 1d0 1d4 1d8 1dc`.
Reordering our units into that sequence (keeping declarations before uses)
shrinks the body to **6,848 B** and raises raw to **6,089** — the original's
extra bytes are *branch-local store duplication*, not source order, so
matching the order alone moves away from 7,072 rather than toward it.

## 4. Conclusion

The post-wave-175 base is a strong local optimum for the raw metric.  The
remaining 5,620 is structural:

* the original's first half is **+182 B wider** because it duplicates stores
  per device branch (8 `stmia` groups vs 3, 575 stores vs 602 but with more
  branch overhead);
* that duplication is what pays for the **+126 B literal pool** (221 vs 190
  words) and the **-70 B tail**;
* every local store-order / ternary / field-order change either leaves the
  size at 7,072 with raw >= 5,620 or changes the size away from 7,072.

Landing a raw win requires reconstructing the original's non-iPadHD branch
duplication (the faithful tail dispatch plus the first-half per-branch store
copies) *together*, not the tail or the order alone.  That is the same
conclusion as wave 175 (w11909), now re-confirmed on the post-wave-175 base.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12008
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w12008/`: `Layout.base.cpp`
(pristine ctor), `split.py` (brace/if-aware unit splitter, 204 units),
`harness.py` (render + build + `fn_diff` + restore), `sweep_adj.py`
(`sweep_adj.json`, 203 adjacent swaps), `sweep_2c.py` (`sweep_2c.json`, 360
`0x2c` group permutations), `features128.py` (A–G matrix), `test_moves.py`,
`test_first.py`, `test_150.py`, `test_tblock.py`, `test_9498.py`,
`test_2cc_struct.py`, `test_2c4c8.py`, `test_ifblock.py`, `test_tricks.py`,
`tailsearch.py` (random swap/move), `stores2.py` (store-map parser),
`unitmap.py` (unit -> field offsets), `orig.dis`/`ours.dis`/`vdiff.txt`.

Gate at exit (base tree): byte_exact **4507/4524**, linked_exact
**4507/4524**, size-exact **4524/4524**, stub_zero_size **0**, missing **0**,
wrong_type **0**, extra **0**, data_exact **843/843**, imports **207**,
lint **CLEAN**, drift **0**, parity **5370/5370**, `.rel.plt` byte-identical.
`Layout` C1/C2 raw **5,620**, aligned **2,289**, size **7,072/7,072**,
2,518 instructions, merge `d22a6`.
