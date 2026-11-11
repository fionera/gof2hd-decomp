# `Layout::Layout` C1/C2 — wave 177 / worker w12111

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Baseline (branch `wave177-w12111`, base master `9445ac5fb`, post-wave-176
tree): raw **5,611**, aligned **2,334**, 7,072 B, 2,519 / 2,552 instructions.
`reload()` is byte-exact (raw 0, 1,064 B).

**Exit: no code change landed.**  This pass targeted the
`Globals::iPadHD/n9/retinaDisplay/iPadLarge/iPad` ternaries, the tail field
store order and the `volatile unsigned char *rd` block.  Every size-exact
variant measured `raw >= 5,611`; the tree is left at the base.

## 1. Region budget and byte-diff map

Boundaries from `objdump -M force-thumb`: prologue `d1028..d1042`; iPadHD
block `d1042..d126c` (orig) / `d1042..d127a` (ours); non-iPadHD path
`d126c..d21e2`; tail `d21e2..d2852`; literal pool `d2852..d2bc8`.

Same-address differing bytes (raw = 5,611, confirmed by summing the regions):

| region | original | ours | differing bytes |
|---|---:|---:|---:|
| prologue | 26 B | 26 B | 5 |
| iPadHD block | 554 B | 568 B | **430** |
| non-iPadHD first half | 3,958 B | 4,140 B | **3,241** |
| tail | 1,648 B | 1,578 B | **1,544** |
| literal pool | 886 B (221 w) | 760 B (190 w) | **391** |

The iPadHD block is only +14 B longer than the original yet 430 of its 554
bytes differ: the whole store schedule is shifted, not just a few values.

## 2. Values are correct; the schedule is not

A full store-map parse (immediate `str`/`strd`/`vstr`, `vst1.32` 4-word groups,
`stmia` with a resolved `r0+imm` base) confirms the ctor writes exactly the
same 201 field offsets as the original with the same values.  The gap is
store *order* / vectorisation / branch duplication.

### 2.1 iPadHD vector-store grouping is shifted by one field

Decoding every `vst1.32` in the iPadHD block (literal pool → 4 words) gives:

| original group | words | ours group | words |
|---|---|---|---|
| `0x14` | 30,12,28,7 | `0x14` | 30,12,28,7 |
| `0x4` | 21,42,40,52 | `0x4` | 21,42,40,52 |
| `0x24` | 7,7,4,42 | `0x24` | 7,7,4,42 |
| `0x34` | 8,9,82,140 | `0x38` | 9,82,140,7 |
| `0x44` | 7,15,7,125 | `0x48` | 15,7,125,487 |
| `0x54` | 487,341,63,90 | `0x58` | 341,63,90,112 |
| `0x64` | 112,566,191,63 | `0x68` | 566,191,63,161 |
| `0x2c8` | 63,126,63,112 | `0x2c8` | 63,126,63,112 |
| `0x7c` | 12,14,112,42 | `0x7c` | 12,14,112,42 |

The values are identical; ours starts each group one field later because
`field_0x34` is emitted as a scalar `str` and `field_0x74` falls into the last
group.  The original's source order in this run is ascending
`0x34,0x38,0x3c,0x40,0x44,...`; our source order is
`0x40,0x44,0x30,0x2c,0x3c,0x38,0x34,0x48,...`, and clang's scheduler groups
from `0x38`.  Reordering `0x34/0x38/0x3c` to ascending changes the grouping
but shrinks the body to 7,024 B (raw 6,021) — size-fatal without a
compensating +48 B elsewhere.

### 2.2 `field_0x2d4` / `field_0x2d8` (`volatile unsigned char *rd` block)

Original (`d1870..d18b6`), resolved through the GOT
(`0x21032c = Globals::iPad`, `0x2103e4 = Globals::retinaDisplay`,
`0x2103e8 = Globals::n9`):

```
r3 = iPad; r8 = retina; r5 = retina | n9
r6 = (retina|n9) ? (iPad?200:125) : (iPad?125:75)   ; 0x2d8
r4 = (retina|n9) ? (iPad?160:100) : (iPad?100:60)   ; 0x2d4
strd r4, r6, [r0, #724]                             ; 0x2d4, 0x2d8 in ONE strd
```

Both values are branchless (`it`/`moveq`) and the two stores merge into one
`strd` (4 B).  Ours emits `0x2d8` as a separate `str` before the `0x2c8`
`vstr`, then the `rd` if/else chain computes `0x2d4` with real branches
(`beq`/`b`) and a second `str`.  The `volatile` pointer forces a second
`ldrb` of `retinaDisplay` (original reuses one load).

Measured variants (all real `link_so` + `tools/fn_diff.py`, C1 alias):

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base (if/else `rd` block, `volatile`) | **5,611** | **2,334** | 7,072 |
| `rd` block → nested ternary, keep `volatile` | 5,713 | 2,339 | 7,072 |
| `rd` block → nested ternary, drop `volatile` | 5,733 | 2,351 | 7,056 |
| ternary `0x2d4` then `0x2d8`, then `0x2c8` | 5,733 | 2,351 | 7,056 |
| `0x2d8` then ternary `0x2d4`, then `0x2c8` | 5,724 | 2,346 | 7,072 |
| `rd` if/else block moved before `0x2d8` | 5,687 | 2,352 | 7,072 |
| `0x2c8` before `0x2d8` before `rd` block | 6,259 | 3,353 | 6,860 |
| `rd` block moved to end of ctor (both ternary) | 6,282 | 3,635 | 6,856 |
| `*rd \| n9`, `retinaDisplay \| n9` (bitwise or) | 5,611 | 2,334 | 7,072 |
| `n9 \|\| retinaDisplay` operand swap | 5,611 | 2,334 | 7,072 |
| `volatile` removed, `retinaDisplay` in condition | 5,732 | 2,333 | 7,064 |
| `(*rd \|\| n9)` → `(n9 \|\| *rd)` | 5,750 | 2,672 | 7,040 |
| `0x2c8` first, then ternary `0x2d4`, `0x2d8` | 6,254 | 3,350 | 6,852 |
| `0x2c8` first, then per-branch duplicated `0x2d4`/`0x2d8` pair | 6,256 | 3,348 | 6,852 |
| per-branch duplicated `0x2d8`/`0x2d4` pair, then `0x2c8` | 6,256 | 3,348 | 6,852 |
| per-branch duplicated `0x2d4`/`0x2d8` pair, then `0x2c8` | 6,019 | 2,738 | 7,044 |
| ternary `0x2d4`, `0x2d8`, then `0x2c8` | 5,733 | 2,351 | 7,056 |
| ternary `0x2d8`, `0x2d4`, then `0x2c8` | 5,730 | 2,351 | 7,056 |

The base if/else `rd` block at its current position is optimal: a full
position sweep of the whole `rd` block over every statement boundary in the
ctor (≈250 builds) found **5,611 at the base position**; the next best was
5,617 (immediately before `0x2d8`) and 5,687 (before `0x2d0`).  Moving it
anywhere else is worse.

### 2.3 Tail field store order

The original's tail is a device-flag if/else chain with per-branch store
duplication, e.g. the reward-box block (`d24bc..d2548`):

```
n9:        width=112; y=57; y2=111; height=352; x=75
iPadLarge: width=200; y=110; -> shared y2=(y+17)<<retina, height=retina?392:196, x=retina?100:50
iPad:      width=100; y=55;  -> same shared tail
else:      width=retina?150:75; y=retina?76:38; -> same shared tail
iPadHD:    width=140; y=77; y2=132; -> height/x from the shared tail
```

Ours computes `rewardBoxWidth` as one flat ternary (with a float round-trip in
the non-iPadHD arms because the iPadHD arm is `100 * 1.40625f`), and `y` as a
flat ternary, so clang sinks the stores differently.  Converting the width or
y to an explicit if/else chain, or reordering `rewardBoxY` before
`rewardBoxY2`, changes the size away from 7,072:

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base | **5,611** | **2,334** | 7,072 |
| `rewardBoxWidth` if/else chain | 5,961 | 2,586 | 7,024 |
| `rewardBoxWidth` iPadHD arm `(int)(100*1.40625f)` | 5,961 | 2,586 | 7,024 |
| `rewardBoxY` if/else chain | 5,696 | 2,329 | 7,080 |
| `rewardBoxY` before `rewardBoxY2` | 5,695 | 2,336 | 7,080 |
| `rewardBoxY` iPadHD arm `(int)(55*1.40625f)` | 5,611 | 2,334 | 7,072 |
| `0x2fc/0x300/0x304` if/else chain + reorder | 5,779 | 2,525 | 7,060 |
| `0x304` moved before `choiceWindowTitleHeight` | 5,841 | 2,396 | 7,116 |
| `0x160` moved after `0x158` | 5,782 | 2,408 | 7,080 |
| `0x160` moved after `0x15c` | 5,712 | 2,302 | 7,064 |
| `0x3c/0x38/0x34` → ascending `0x34/0x38/0x3c` | 6,021 | 2,404 | 7,024 |
| `0x2c0` moved before `0x2c4` | 6,030 | 2,424 | 7,020 |
| `0x94` moved before `0x98` | 5,948 | 2,287 | 7,016 |
| `field_0x98` if/else chain (shared store) | 5,756 | 2,326 | 7,056 |

A full position sweep of `field_0x98` over every statement boundary in the
ctor (≈250 builds) likewise found the base position best (5,611); next best
5,617.

## 3. Why local changes cannot win

The original's code is 6,186 B / 2,552 insns; ours is 6,312 B / 2,519 insns.
The original has **33 more instructions but 126 fewer code bytes**, i.e. it
uses many more 16-bit `movs`/`it`/`moveq` instructions.  Those come from
per-device-branch store duplication in the non-iPadHD first half (the
original has 575 stores vs our 602, but 8 `stmia` groups vs our 3 and extra
`strd` pairs), which is what pays for the original's larger literal pool
(221 vs 190 words).  The size is a three-way compensation:
`+182 B first-half code − 70 B tail − 126 B pool = 0`.

Every local store-order / ternary / field-order edit either leaves the size
at 7,072 with raw >= 5,611 or changes the size away from 7,072.  Matching the
original requires reconstructing the non-iPadHD branch duplication (faithful
tail dispatch plus the first-half per-branch store copies) **together**, not
the tail or the order alone.  This confirms the wave-175/176 conclusion on
the post-wave-176 base.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12111
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
# raw=5611 aligned=2334 size=7072/7072 insn=2519/2552
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w12111/`: `Layout.base.cpp`
(pristine ctor), `measure.sh`, `runs.py` (per-address diff runs),
`storeseq.py` (tail store sequence), `vecstores2.py` (iPadHD `vst1.32` decode),
`gotmap2.py` (GOT symbol resolution), `sweep_move.py` (0x98 position sweep),
`sweep_block.py` (`rd` block position sweep).

Gate at exit (base tree): byte_exact **4507/4524**, linked_exact
**4507/4524**, size-exact **4524/4524**, stub_zero_size **0**, missing **0**,
wrong_type **0**, extra **0**, data_exact **843/843**, imports **207**,
lint **CLEAN**, drift **0**, parity **5370/5370**, `.rel.plt` byte-identical.
`Layout` C1/C2 raw **5,611**, aligned **2,334**, size **7,072/7,072**,
2,519 instructions.
