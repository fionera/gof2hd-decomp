# `Layout::Layout` C1/C2 — wave 191 / worker w13490

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, linked symbol size **7,072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave191-w13490`, base master `f325e2d87`.

**Result: no size-exact raw win found.  `src/` left at the base (raw 5,243,
aligned 2,728, size 7,072/7,072, insn 2,521/2,552).**  Tree clean; no src
change was landed.  This note records only cells **not** already in
`Layout-LayoutC1-wave190-w13420.md`, `...-w13443.md`, `...-w13467.md`,
`...-w13458.md` and `...-w13473.md` (read those first).

## 1. New tool: 100-copy compile-only proxy (validated)

`scratch/w13490/proxy.py` compiles N copies of the ctor body as free functions
`void ctorN(Layout *s)` in one TU with the exact `Layout.cpp` flags, then
measures each function's bytes offset-wise against the original ctor.  This is
~100× faster than a real `link_so` per candidate and is **accurate to 3 bytes**
(proxy baseline raw **5,240** vs linked **5,243**, size 7,072).  Use it to
filter; verify winners with `link_so` + `fn_diff`.

`scratch/w13490/search.py` splits the ctor into 204 atomic blocks (if/else
chains kept together; locals 31/36/47 kept before their uses) and exposes
`measure(bodies)`.

## 2. Exhaustive ordering sweeps (all negative)

Baseline proxy `(raw, size) = (5240, 7072)`; a win needs `< 5240` at 7,072.

| sweep | candidates | best size-exact |
|---|---:|---:|
| every block moved to every position within ±12 blocks | 4,674 | identity (no `<5240`) |
| 4,000 random **two**-block moves (each ±20) | 4,000 | identity (no `<5240`) |
| reversal of every contiguous block of length 2–6 | 972 | identity (no `<5240`) |
| every position for `field_0x94` / `field_0x140` / `field_0x144` | 3×~200 | 5243 / 5243 / 5322 |
| every position for `field_0xac` / `field_0x2f8` / `field_0xa8` | 3×~200 | 5727 / 5247 / 5663 |
| all 24 perms of `{0x300,0x2f4,0x2f8,0x2fc}`, `{0x30c..0x318}`, `{0x284..0x290}` | 72 | identity |

So the current order is still a sharp local optimum even after the two wave-190
landed moves.

## 3. `order_hd.txt` reorder (new; negative)

The HD-path store order (`_work/reconstructions/layout_ctor/order_hd.txt`) was
mapped to the 204 blocks (named-field offset table included in
`scratch/w13490`) and the whole body reordered to it:

* raw **6,429**, size **6,776** (vs 5,240 / 7,072).  Much worse, as with the
  iOS and `order_default` reorders.

## 4. `vstr`-set toggles (new; negative)

The original stores int fields via `vstr` at
`0x9c,0x150,0x158,0x15c,0x160,0x170,0x1e4,0x3f8,0x3e0`; ours at
`0x20,0x10c,0x144,0x148,0x1f4,0x204` instead.  Probe results:

* converting ours-extra float HD arms to int literals (`5*1.40625f`→`7`, etc.):
  `0x1f4/0x148/0x204/0x144` each shrink by 8 B (7,064); `0x20` collapses to
  6,796; `0x10c` to 6,992.
* forcing float arms on the original-`vstr` fields: `0x150/0x160/0x170` are
  **codegen-identical** (clang folds); `0x15c` 7,056; `0x158` 7,032;
  `0x158`-float-arms 7,072 but raw 5,890; `0x1e4` 6,980; `0x3f8`
  `(int)(float)(...)` 7,092.
* all 6×8 single pairs and the size-exact grow/shrink pairs
  (`wrap 0x2d0` +8 with `0x1f4` −8 → 7,072 raw 5,306; `wrap 0x9c` +8 with
  `0x1f4` −8 → 7,072 raw 5,378): none beats 5,240.

## 5. Boolean / cast simplifications (new; negative)

Value-identical rewrites tested (proxy, all 7,072 unless noted):

| rewrite | raw |
|---|---:|
| `field_0x1e8` → `(retina\|\|HD\|\|n9) ? 1 : 0` | 5,240 (folds, neutral) |
| `field_0x280` → `HD ? 0 : (n9 ? 4 : (retina ? 5 : 0))` | 5,240 (folds) |
| `field_0x190` → `(n9\|\|iPad) ? -4 : (retina ? -2 : -1)` | 5,690 (size 6,944) |
| `field_0x2bc` → `(n9 && !HD) ? 3 : 2` | 6,236 (size 6,936) |
| `field_0x1fc` → `(n9\|\|retina) ? 4 : 2` | 5,676 (size 7,096) |
| `field_0x234` → `(n9\|\|retina) ? 2 : 1` | 5,668 (size 7,048) |
| `Globals::F ?` → `(Globals::F != 0) ?` for F in {HD,n9,retina,iPad,iPadLarge} | 5,240 for n9/iPadLarge; 5,746/5,394/5,971 otherwise |
| `(int)(float)t_field_0x2c8` → `(int)(float)(int)t...` / `(float)t...` | 5,240 (folds) |
| `(int)(float)v` → `(int)(float)(int)v` / `(float)v` | 5,240 (folds) |
| swap `field_0x4`/`footerTextInset` inside each branch of unit 0 | 5,248 |
| local `d8v` for `field_0x2d8` (same expression, stored after the 0x2d4 block) | 5,240 (folds) |
| `field_0x2d8` moved after the 0x2d4 block (source) | 6,254 (size 6,888) |
| locals `d4v`/`d8v` then two stores (to invite `strd`) | 6,230 (size 6,880) |

## 6. Conclusion

The ordering space is exhausted at the current raw basin.  The remaining
residue is the wave-190 §3 callee-saved rotation (`sl` vs `lr` for the HD flag,
`r8` vs `ip` for the retina pointer) plus the original's 5 outlined cold tails
vs our 2; neither is reachable by statement reorder or by the value-equivalent
expression rewrites above.  A raw win now needs a source shape that makes clang
keep `Globals::iPadHD` in `sl` and `&Globals::retinaDisplay` in `r8` across the
whole body.

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13490
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
# raw=5243 aligned=2728 size=7072/7072 insn=2521/2552
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13490/`: `proxy.py` (100-copy
proxy), `search.py` (block splitter + batched measure), `sweep.py` (±12
single moves), `rand2.py` (two-move random), `move.py`/`movebatch.py`,
`reorder.py`/`reorder2.py`, `units.py`.
