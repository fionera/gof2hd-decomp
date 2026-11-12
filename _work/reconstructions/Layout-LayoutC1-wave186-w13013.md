# `Layout::Layout` C1/C2 — wave 186 / worker w13013

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave186-w13013`, base master `2d02567f6` (post-wave-185 tree; w12912's
four permutation wins are landed, raw **5,358**).

**Result: no size-exact raw win found.  `src/` left at the base.**

| metric | base |
|---|---:|
| raw same-address bytes | **5,358** |
| aligned (difflib) | 2,626 |
| size | **7,072/7,072** |
| instructions | 2,517/2,552 |

Task angle: the `Globals::iPadHD/n9/retinaDisplay/iPadLarge/iPad` ternaries,
the `volatile unsigned char *rd` block, and the `rewardBoxY2` bug.  No blind
W/K permutation search was run (that is w13012's cell); every variant below is
a real `link_so` + `tools/fn_diff.py` measurement on the wave-185 base.

## 1. What the original actually does (new findings)

### 1a. `field_0x1e8` — real semantic bug in HEAD

The original's tail prologue (`0xd21e2..0xd21f8`) is

```
d21e2: vcvt.s32.f32 s0, s0
d21e6: cmp.w  lr, #0
d21ea: mov.w  r1, #1
d21ee: mov.w  r2, #1
d21f2: it     ne
d21f4: movne  r1, r8
d21fa: str.w  r1, [r0, #488]   ; 0x1e8
d21fe: vstr   s0, [r0, #484]   ; 0x1e4
```

`lr` is built at `d1248..d1258` as `!(ip | sl)` where `sl` is `Globals::iPadHD`
(loaded at `d1036`) and `ip`/`r8` are the **same** global: the GOT load at
`d1044` (`ldr r2,[pc,#720]` -> literal `0x13f38c` at `0xd1318`) plus
`add r2,pc` (`d1058`, pc=`0xd105c`) resolves to `0x2103e8`, which
`readelf -r` maps to `R_ARM_GLOB_DAT _ZN7Globals2n9E`.  Both `ldrb ip,[r2]`
(`d114e`) and `ldrb r8,[r2]` (`d1260`) are therefore `Globals::n9`.

So `field_0x1e8 = (n9 || iPadHD) ? 1 : 0`, i.e. `n9 | iPadHD`.
HEAD spells it

```
field_0x1e8 = retinaDisplay ? 1 : (iPadHD ? 1 : (n9 ? 1 : 0));
```

which is `retina | HD | n9` — wrong when `retina=1, n9=0, HD=0` (HEAD 1,
original 0).  Fixing it (`= (n9 || iPadHD) ? 1 : 0;`) is size-fatal:
raw **5,924**, size **7,016** (-56), insns 2,515.

### 1b. `field_0x2d4` / `field_0x2d8` — block-B corner bug and the `strd`

The original's block-C `0x2d4`/`0x2d8` builder (`d1864..d18b6`) is

```
r6 = 200; r4 = 160; r1 = 125
if (*rd == 0) { r1 = 75; r6 = 125; }
if ((iPad|n9) == 0) r6 = r1;
r1 = 100
if (*rd == 0) { r1 = 60; r4 = 100; }
if ((iPad|n9) == 0) r4 = r1;
strd r4, r6, [r0, #724]        ; 0x2d4, 0x2d8 in ONE store
```

i.e.

```
0x2d4 = (iPad||n9) ? (*rd ? 160 : 100) : (*rd ? 100 : 60)
0x2d8 = (iPad||n9) ? (*rd ? 200 : 125) : (*rd ? 125 : 75)
```

HEAD's shapes are value-equivalent in block C but differ in **block B (n9)**
when `n9 && retina && !iPad`: HEAD gives `0x2d4=100`, `0x2d8=125`; the
original gives `160` / `200`.  HEAD also stores `0x2d8` far from `0x2d4`
(before `0x2c8`), so clang emits two `str.w` instead of the original's `strd`.
Writing the faithful shape shrinks the function by 128-200 B (size-fatal).

### 1c. `rewardBoxY2` — missing `(int)(float)` round-trip

Original block-C tail (`d2514..d252c`):

```
r2 = rewardBoxY + 17
r2 = r2 << retinaDisplay
s0 = (float)r2; s0 = (int)(float)s0
str.w r1, [r0, #1012]   ; rewardBoxY
vstr  s0, [r0, #1016]   ; rewardBoxY2
```

so the source is `rewardBoxY2 = (int)(float)((rewardBoxY + 17) << retinaDisplay);`
(value-identical for these small ints, but the round-trip is visible in codegen).
HEAD omits the round-trip.  Adding it is size-fatal: raw **5,462**,
size **7,092** (+20).  The original also stores `rewardBoxY` and `rewardBoxY2`
adjacently; moving the HEAD `if/else` block next to `rewardBoxY` (with or
without the round-trip) gives 6,032/7,048 and 5,983/7,064.

## 2. Measured structural variants (all `_ZN6LayoutC1Ev`)

`raw` lower is better; every row was a full `link_so`.

| variant | raw | aligned | size |
|---|---:|---:|---:|
| **base** | **5,358** | 2,626 | **7,072** |
| `rewardBoxY2` round-trip (`ry2`) | 5,462 | 2,638 | 7,092 |
| exact `0x2d4` shape (`rd`) | 5,939 | 2,742 | 6,944 |
| exact `0x2d8` shape (`d8`) | 5,815 | 3,956 | 7,080 |
| `rd`+`d8` | 5,976 | 2,676 | 7,024 |
| `rd`+`d8`+`ry2` | 5,960 | 2,675 | 7,044 |
| `rd`+`ry2` | 5,908 | 2,849 | 6,956 |
| `d8`+`ry2` | 5,936 | 3,924 | 7,096 |
| `ry2` moved after `rewardBoxY` | 6,032 | 3,960 | 7,048 |
| same + round-trip (`adjrt`) | 5,983 | 3,980 | 7,064 |
| original tail order (width,Y,Y2,height,X) | 5,905 | 2,693 | 7,120 |
| same, no round-trip | 5,887 | 2,678 | 7,104 |
| drop `volatile` on `rd` (`novol`) | 5,495 | 2,631 | 7,064 |
| `rd` -> direct `Globals::retinaDisplay` | 5,495 | 2,631 | 7,064 |
| `novol`+`d8` | 5,889 | 2,922 | 6,960 |
| exact `0x2d4`+`0x2d8` as one `if/else` with adjacent stores | 6,278 | 3,619 | 6,872 |
| same, `0x2d8` separate after the block | 6,278 | 3,630 | 6,888 |
| `field_0x2d4` `(iPad\|\|n9)` condition, `retina` value | 5,862 | 2,933 | 6,960 |
| `field_0x2d8` `(iPad\|\|n9)` condition, `retina` value | 5,815 | 3,956 | 7,080 |
| `field_0x2d8` `(*rd\|\|n9)` condition (base value) | 5,358 | 2,626 | **7,072** |
| `field_0x2d8` `(n9\|\|retina)` condition (base value) | 5,358 | 2,626 | **7,072** |
| `field_0x2d4` `(*rd\|\|n9)` condition (base value) | 5,358 | 2,626 | **7,072** |
| `field_0x2d4` `(n9\|\|retina)` condition | 5,495 | 2,631 | 7,064 |
| `field_0x2d4` simplified `(retina\|\|iPad\|\|n9) ? 100 : 60` | 6,028 | 3,942 | 7,040 |
| `field_0x2d4` merged last `else if` | 5,499 | 2,637 | **7,072** |
| `field_0x1e8` semantic fix (`n9\|\|HD`) | 5,924 | 2,824 | 7,016 |
| move `0x2c4` after the `rd` block | 5,900 | 2,748 | 7,016 |
| move `0x98` next to `0x94` | 5,989 | 2,909 | 6,964 |
| move `0x2d8` after the `rd` block | 6,227 | 3,560 | 6,896 |
| move `0x2c4`+`0x98` | 5,998 | 2,735 | 6,952 |
| move `0x2c4`+`0x98`+`0x2d8` | 6,238 | 3,531 | 6,904 |
| move `rd` block after `0x2d8` | 5,364 | 2,661 | **7,072** |
| move `rd` block after `0x2c8` | 5,358 | 2,626 | **7,072** |
| move `rd` block before `0x2d8` | 5,512 | 2,650 | **7,072** |

`(int)(float)` round-trip sweeps: adding it to `0x150`, `0x170`, `0x1a8`,
`0x1d4`, `0x1e8`, `0x2f8`, `0x60`, `0x68`, `0x6c`, `0x70`, `scrollBarInset`
is codegen-neutral (5,358/7,072); `0x158` -> 5,654/7,072; `0x1e4` ->
5,944/6,968; `0x160` -> 6,176/6,992.  Removing the existing round-trip from
`0x10c`, `0x2c8`, `0xa8` gives 5,502/7,064, 5,533/7,064, 5,502/7,064.
`ry2` combos with those removals: `ry2`+`0xa8` 5,497/7,084,
`ry2`+`novol` 5,482/7,084, `ry2`+`0x10c`+`0x2c8` 5,545/7,076,
`ry2`+`0x10c`+`0x2c8`+`0xa8` 5,516/7,068.

## 3. Position sweeps (no blind W/K search)

* **`rd` block (0x2d4 if/else, 9 lines) moved to every one of the 204
  top-level positions.**  Best size-exact raw: **5,358 at its base position**
  (the identity); next 5,364 (after `field_0x2d8`), 5,512, 5,717, 5,829.
  No position beats 5,358.
* **`rewardBoxY2` if/else block moved to every position at/after the
  `rewardBoxY` assignment** (~70 positions).  Best size-exact raw: **5,358 at
  its base position**; no position beats it.

## 4. Why the residual is hard

Original code = 6,104 B (`pop` at `0xd2800`) + 968 B pool; HEAD code =
6,288 B (`pop` at `0xd28b6`) + 784 B pool.  The split is 184 B off in opposite
directions.  Original has 86 `vld1`/81 `vst1` groups and 62 `strd`; HEAD has
76/71 and 41.  Ten extra SLP groups is ~160 B of pool, i.e. the whole gap.
Every faithful local shape (exact `0x2d4`/`0x2d8`, `strd` merge, `rewardBoxY2`
round-trip, `field_0x1e8` fix) makes the function *shrink* without growing the
pool, so it is size-fatal; conversely a full ascending-field-order sort of the
ctor (the shape that maximises SLP) produces 90/85 groups but shrinks the
function to 6,868 B and scores raw 6,182.  The landed wave-185 order is a sharp
local optimum for the same-address byte metric.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13013
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
# raw=5358 aligned=2626 size=7072/7072 insn=2517/2552
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13013/`: `Layout.base.cpp`
(pristine ctor), `measure.sh`, `orig.dis`/`ours.dis`, `seq.py` (store-sequence
decoder), `sortctor.py`/`sortctor2.py` (ascending-order variants),
`sweep_rd.py`/`sweep_ry2.py` (position sweeps), `cand/` (all candidates).
