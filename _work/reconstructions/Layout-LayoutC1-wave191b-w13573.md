# `Layout::Layout` C1/C2 — wave 191b / worker w13573

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, linked symbol size **7,072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave191b-w13573`, base master `58eac6932` (w13546 final: raw 5,232).

**Result: no size-exact raw win found; note only, tree clean.**

| | before | after |
|---|---:|---:|
| raw | 5,232 | 5,232 |
| aligned | 2,711 | 2,711 |
| size | 7,072/7,072 | 7,072/7,072 |
| insn | 2,517/2,552 | 2,517/2,552 |

## 1. Diff-cluster map (fresh build of the current base)

`scratch/w13573/cluster2.py` (difflib over the normalised instruction streams,
clusters merged through equal runs <= 2):

```
790 B  O@0x1076  U@0x115e   fields 0x1ac..0x1e8 retina/else arm
562 B  O@0x0e3a  U@0x0f5a   fields 0x16c..0x17c
458 B  O@0x0004  U@0x0004   prologue (r6 not saved by us) + pool loads
412 B  O@0x0a8c  U@0x0ab4   fields 0x140..0x14c (orig vst1 0x140, ours scalar)
360 B  O@0x1500  U@0x15da   fields 0x204..0x270 vector-window shift
282 B  O@0x13da  U@0x1518
210 B  O@0x0c90  U@0x0d50
182 B  O@0x09ba  U@0x09e6
...
```

The top 15 clusters cover ~4.2 kB of the 5,232 differing bytes.

## 2. Largest cluster: the retina/else arm of 0x1ac..0x1e8

Store-trace comparison (emu.py, default flags) of the retina arm:

```
orig: 1e0 1c0(16B) 1b0(16B) 1d0(16B) 1e8 1e4
ours: 1e0(early) 1d8 1dc 1d4 1b0(16B) 1c0(16B) 1d0 1e4 1e8
```

The original's `vst1.32 {d16-d17},[r0,#0x1d0]` is a 16-byte window covering
0x1d0/0x1d4/0x1d8/0x1dc.  Ours emits four scalar `str.w`.  The IR explains why:

* base source order is 0x1d8, 0x1dc, 0x1d4, 0x1d0 (descending) so SLP cannot
  form a contiguous window;
* even after re-ordering the four statements ascending, SLP still does not
  vectorise them: the `icmp eq i8 %retina, 0` for 0x1dc is CSE'd with the
  4-lane vector `icmp` of the 0x1b0 window (`extractelement`), while 0x1d0 /
  0x1d4 / 0x1d8 each carry their own scalar `icmp`.  The four selects
  therefore do not share one condition SSA value.
* forcing a shared condition (`bool ret = Globals::retinaDisplay;` before the
  group) or rewriting the four as a config-major `if/else if/else` tree both
  still leave the group scalar.

**Measured (all real `link_so` builds):**

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base | 5,232 | 2,711 | 7,072 |
| 0x1d0..0x1dc ascending | 5,893 | 2,765 | 7,040 |
| ascending, block moved before 0x1b0 group | 5,956 | 2,807 | 7,016 |
| base order, block moved before 0x1b0 group | 5,946 | 2,807 | 7,024 |
| `bool ret` + ascending | 5,693 | 2,742 | 7,024 |
| config-major if/else for the four | 5,922 | 2,685 | 7,040 |

**All 24 permutations of the four statements were measured**: every order that
starts with 0x1d8 is codegen-identical to the base (5,232 / 7,072); every order
that does not start with 0x1d8 shrinks to 7,040 or 7,056.  So 0x1d8-first is
load-bearing for the 7,072 size ridge, and the internal order is irrelevant.

Likewise all 24 permutations of the 0x1b0/0x1b4/0x1b8/0x1bc statements are
codegen-identical (5,232 / 7,072) — that window already vectorises.

## 3. Size-ridge observations

The 7,072 B size is a narrow ridge: almost every statement re-order shrinks the
body (code shrinks faster than the literal pool grows).  Examples measured
this session (all size-fatal unless noted):

* 0x16c moved to any of 150..210: 5,246..6,221, size 6,816..7,072 (only
  0x16c->166..169 size-exact, raw 5,246..5,248).
* 0x170 moved next to 0x16c: 5,890 / 6,944.
* 0x18c moved next to 0x184: 5,843 / 6,992.
* 0x1ac moved after 0x1a8: 5,388 / 7,048 (24 short).
* 0x1a8 moved to any of 170..193: 5,717..5,874, size 6,912..7,008.
* 0x140/0x144/0x148/0x14c/0x150 ascending: 5,945 / 6,984.
* 0x180/0x184/0x188/0x18c ascending: 6,062 / **7,120** (one of the few
  size-*growing* re-orders; raw much worse).
* 0x204/0x208 adjacent: 5,976..6,009, size 6,960..7,008.
* 0x90 moved to 82..100: 5,410..6,031, size 6,832..7,064.
* 0x1a8 duplicated at 192..213: 5,337..6,036, size 6,904..7,088.

**Only size-exact re-order found:** `field_0x184` (line 187) moved to 176..179
gives 5,233 / 2,708 / 7,072 — one raw byte worse, three aligned better.

## 4. Size-exact but raw-worse combinations

* `field_0x130`/`0x134`/`0x138` if/else -> ternary chain (w13519 measured
  7,080) **plus** dropping `volatile` from the `rd` local (w13546 measured
  7,064) is exactly **7,072** but raw 5,373 / aligned 2,722.
* `(int)(float)(int-chain)` on `field_0x1a8` (int arms 22/24/32/16): 7,072,
  raw 5,399.
* `(int)(float)` on `field_0x18c`/`0x1d4`/`0x164`/`0x174`: codegen-identical.

## 5. Other axes measured (neutral or worse)

* `field_0x1e8` as `(retina||iPadHD||n9)?1:0` and all 6 flag permutations of
  the ternary: codegen-identical (5,232 / 7,072).
* `field_0x2bc = n9 ? 3 : 2` (drop the redundant iPadHD arm): 6,339 / 7,024.
* `field_0x114_strip5c` De Morgan: 5,455 / 7,056.
* `field_0x2d8` re-associations: 5,871..5,991, size 7,068..7,092.
* `int v` (field_0xa8) with int arms instead of float arms: 5,492 / 7,032.
* Duplicating every statement in lines 20..110 at i+1 / i+2 / i+3: no size-exact
  result below 5,232 (best 5,232, many 5,261..5,347).
* 300 random single-statement moves: 17 size-exact, all 5,232.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13573
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
# raw=5232 aligned=2711 size=7072/7072 insn=2517/2552
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13573/`: `measure.sh`,
`base.cpp`, `cluster2.py`, `sbs.py`, `trace.py`, `valall2.py`, `perm4.py`,
`perm4b.py`, `permblocks.py`, `sweep_far.py`, `sweep_win.py`, `rnd.py`.
`valall2.py` confirms the store image is identical for all 32 flag
combinations, so every residual is pure codegen.

## 7. Follow-up pass (same worker, after the first note commit)

### 7.1 Landed win: `field_0x184` restate / `field_0x1a0` swap

Swapping the *second* `field_0x184` restate (line 189) with `field_0x1a0`
(line 190) is a semantic no-op (all 32 flag configs: store image identical,
`valall2.py` -> `configs with diffs 0`) and gives

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base | 5,232 | 2,711 | 7,072 |
| swap 189<->190 | **5,232** | **2,700** | 7,072 |

Raw unchanged, aligned -11, size exact. This is strictly better than the
`0x184 -> 176..179` move (5,233 / 2,708) from section 3, so it is landed.

### 7.2 Root cause of the 0x1d0 SLP miss (refined)

Minimal repro (`scratch/w13573/t14.cpp`): 12 consecutive `int` stores whose
values are `flag ? A : B` with a *single* shared condition SSA (a function
parameter) -> SLP vectorises **all three** 4-lane chunks at `-Oz`.

`t9.cpp` (same 12 stores, condition from a global load) -> only 2 chunks: the
first store carries a *different* `icmp` instruction and is left scalar,
which shifts the chunk boundaries.

In `Layout` the retina block's four `0x1d0`-group selects do **not** share one
`icmp`: the pre-SLP IR has `%906` (0x1d8/0x1dc/0x1d4) and `%913` (0x1d0, also
used by 0x2c0/0x1c0).  SLP vectorises the `0x1c0` chunk with a splat
`icmp eq <4 x i8>` and the last `0x1d0` select's condition becomes
`extractelement(%vec_icmp, 0)`; the four conditions are then
`{icmp, icmp, icmp, extractelement}` and the `0x1d0` chunk is rejected.

Forcing a single shared condition **does** vectorise it, but every form tried
changes the surrounding block structure and is raw-worse:

| variant | raw | aligned | size |
|---|---:|---:|---:|
| `const unsigned char r = Globals::retinaDisplay;` before group (u1) | 5,958 | 2,690 | 7,008 |
| `const int r = ...` (u2) | 5,958 | 2,690 | 7,008 |
| `const unsigned char r = *rd;` (volatile, u4) | 5,979 | 3,079 | **7,072** |
| `const bool r = Globals::retinaDisplay;` all four (v2) | 5,693 | 2,742 | 7,024 |
| `const unsigned char rv = ...` at line 60 (w1) | 6,025 | 3,007 | 6,880 |

So the 0x1d0 vector store is not reachable by a shared-condition reshape
without a net raw regression.

### 7.3 Other chunk-alignment misses

The `0x140/0x144/0x148/0x14c` and `0x16c/0x170/0x174/0x178` groups are the
same story: the original has `vst1` at `0x140` and `0x16c`, ours at `0x170`
(and scalar at `0x140`).  The source statements are scattered
(`0x140` line 208, `0x144` line 284, `0x148` line 158, `0x14c` line 153), so
the chain never aligns.  Moving them adjacent (`0x140,0x144,0x148,0x14c`
ascending, z1) gives 6,110 / 2,765 / **6,880** (fatal) and *loses* 10 vector
stores elsewhere (57 vs 67 in the whole TU).

### 7.4 Exhaustive small-perturbation sweeps (all size-exact -> raw >= 5,232)

* duplicate every simple statement at `i+1` (120 size-exact): min raw 5,232.
* adjacent swap of every simple pair (83 size-exact): min raw 5,232.
* non-adjacent swaps `(i, i+2)` and `(i, i+3)` (111 size-exact): min raw
  5,232; best aligned 2,705 but that one (229<->231) is semantically wrong
  (moves `orbitInfoTextX = 12` out of its `else if`), `valall2` shows 8
  config diffs.
* regional moves 186..200 x 186..205 (31 size-exact) and 199..215 x 199..215
  (33 size-exact): only 189<->190 improves (aligned 2,700).
* moving the `rd` declaration (line 60) to 24..59: codegen-identical.
* moving `t_field_0x2c8` (55) to 43: 5,748 / 7,072.  Moving `int v` (85) to
  69/79/81/82: 6,379..6,387 / 7,072.

### 7.5 Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13573
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
# raw=5232 aligned=2700 size=7072/7072 insn=2517/2552
```
