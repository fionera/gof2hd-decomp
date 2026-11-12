# `Layout::Layout` C1/C2 — wave 190 / worker w13443

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, linked symbol size **7,072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave190-w13443`, base master `f0f4d7918`.

**Result: no size-exact raw win found.  `src/` left at the base (raw 5,249,
aligned 2,725, size 7,072/7,072, insn 2,520/2,552).**  Gate re-run green:
`RATCHET PASS`, byte_exact 4507/4524, linked_exact 4507/4524, size-exact
4524/4524, imports 207, lint CLEAN.

This note records only cells **not** already in
`Layout-LayoutC1-wave190-w13420.md` (read that first; its §4 list is not
repeated here).

## 1. Correction to w13420 §1: the pool tail is 4 words, not 6

w13420 states "the last 6 words match exactly (`0xd2bb0..0xd2bc4` = 200.0,
100.0, 150.0, 144.0, 150.0, 76.0)".  Re-decoded word-aligned from the current
base build:

| addr | original | ours |
|---|---|---|
| `0xd2bb0` | `0x43160000` 150.0 | `0x43160000` 150.0 |
| `0xd2bb4` | `0x43100000` 144.0 | `0x43100000` 144.0 |
| `0xd2bb8` | `0x43160000` 150.0 | `0x43160000` 150.0 |
| `0xd2bbc` | `0x42980000` 76.0  | `0x42980000` 76.0  |
| `0xd2bc0` | `0x43480000` 200.0 | `0x0013db3a` (GOT addend) |
| `0xd2bc4` | `0x42c80000` 100.0 | `0x0013dab4` (GOT addend) |

So **4 words (16 B) match at the tail**, and the two trailing pointer words in
ours are the `Globals::retinaDisplay` GOT reloads at `0xd27ec` (the
`0x30c..0x318` chain) and `0xd286e` (`helpWindowY`).  In the original the
retina pointer stays in `r8` (loaded once at `0xd158c`), so those addends are
first-used much earlier in the pool.  This is the same callee-saved-rotation
residue as w13420 §3, not a separate pool-order bug.

## 2. Measured negatives (new cells; all size-exact at 7072 unless noted)

Baseline `(raw, aligned, size)` = `(5249, 2725, 7072)`.

### 2.1 `if/else` chain → ternary

| chain | result |
|---|---|
| `field_0x20c` | `(5501, 7088, 7072)` |
| `orbitInfoTextX` | `(5501, 7088, 7072)` |
| `orbitInfoStationY` | `(5344, 7048, 7072)` |
| `field_0x218` | `(5344, 7048, 7072)` |
| `field_0x260` | `(5853, 7072, 7072)` |
| `field_0x280` | `(5249, 7072, 7072)` **neutral** |
| `field_0x190` | `(5424, 7088, 7072)` |
| `field_0x130/0x134/0x138` (3 ternaries) | `(5407, 7080, 7072)` |

### 2.2 First block (`field_0x4` / `footerTextInset`)

* split into **two separate if/else chains** (one per field): **neutral**
  `(5249, 7072, 7072)` — clang merges them back.
* two independent ternaries: `(5257, 7072, 7072)` (cf. w13420's combined
  ternary `5257`).

### 2.3 Tail / pool-order experiments

| change | result |
|---|---|
| `0x30c..0x318` block moved before `0x300` | `(5276, 7072, 7072)` |
| `0x30c..0x318` before `0x2f8` | `(5884, 7136, 7072)` |
| `0x2f8` moved after `0x318` | `(5436, 7104, 7072)` |
| `helpWindowY` before `rewardBoxY2` | `(5297, 7032, 7072)` |
| `0x2f8` moved to the very end | `(5318, 7024, 7072)` |
| `0x30c` alone to positions 210..222 | best `(5382, 7108, 7072)` |
| `0x30c..0x318` block to every position 205..225 | best non-identity `(5276, 7072, 7072)` |

### 2.4 `field_0x2c0` (unit 134) moved to every position 120..225

All worse; best non-identity `(5901, 6944, 7072)`.  The original sinks
`0x2c0` out of the HD block into the mid block (`0xd177a..d1786`); ours stores
it in the HD block (`0xd1258`, `mvn r3,#1`).  No source position reproduces
the sink.

### 2.5 `field_0x34` (unit 19) moved among units 9..20

All worse (best `(5766, 7056, 7072)`); only `34@20` is neutral (identity).
This targets the original n9/HD SLP group `{0x34,0x38,0x3c,0x40}` vs ours
`{0x38,0x3c,0x40,0x44}`; the group shift is not statement-order driven.

### 2.6 `0x2c4/0x2c8/0x2cc` group

All 24 permutations of `{0x2c4, t_field_0x2c8, 0x2cc, 0x2c8}` inserted at the
`0x98` slot: no win (best identity `5249`).  `0x2c0` inserted into that group
in 7 orders: `(5944..6222)`, all worse.

### 2.7 HD-flag hoisting (register-allocation probe)

Replacing every `Globals::iPadHD` with a local:

| local decl | result |
|---|---|
| `const unsigned char hd` | `(5719, 6956, 7072)` |
| `const int hd` | `(5719, 6956, 7072)` |
| `unsigned char hd` / `int hd` | `(5719, 6956, 7072)` |
| `const bool hd` | `(6025, 6896, 7072)` |

Crucially the prologue is **unchanged** (`ldrb.w lr,[r1]`, `stmdb
{r7,r8,r9,sl,fp}`): hoisting does not move the flag to `sl`.  The 116 B shrink
is pure CSE of the flag loads, so this is not the original's shape.

### 2.8 `(int)(float)` round-trips on the mid-block fields

| field | result |
|---|---|
| `0x158` wrapped in `(int)(float)(...)` | `(5460, 7064, 7072)` |
| `0x15c` wrapped | `(5249, 7072, 7072)` **neutral (folds)** |
| `0x170` wrapped | `(5249, 7072, 7072)` **neutral (folds)** |
| `0x158+0x15c`, `0x158+0x170`, `0x158+0x15c+0x170` | `(5460, 7064, 7072)` |

Only `0x158` keeps the round-trip (its default arm reads the member
`this->field_0x154`, which is opaque), and it lands 8 B short.  `0x160`'s
`(int)(float)(int)(...)` form does not compile from the naive wrapper.

### 2.9 `rd` pointer variants

| form | result |
|---|---|
| `const volatile unsigned char *rd` | `(5249, 7072, 7072)` neutral |
| `volatile unsigned char * const rd` | `(5249, 7072, 7072)` neutral |
| `volatile unsigned char &rd = Globals::retinaDisplay` | `(5249, 7072, 7072)` neutral |
| `unsigned char *rd = (unsigned char *)&...` | `(5398, 7064, 7072)` |
| declaration moved to the top / after unit 0 | `(5249, 7072, 7072)` neutral |

### 2.10 Misc

| change | result |
|---|---|
| `0x2bc = n9 ? 3 : 2` | `(6332, 7048, 7072)` |
| `0x2bc = (n9 && !iPadHD) ? 3 : 2` | `(6194, 6928, 7072)` |
| `0x2d4` if/else → one ternary | `(5465, 6960, 7072)` |
| `0x2d8` moved after the `0x2d4` block | `(6248, 6880, 7072)` |
| `0x140`+`0x144` before `0x14c`/`0x150`/`0x148` | `(5911..5996)`, all worse |
| `0x94` before `0x14c` | `(5939, 7000, 7072)` |
| 300 random single-unit moves (seed 12345) | no win (best identity) |

## 3. Conclusion

The residue is still the w13420 §3 callee-saved rotation: the original keeps
`Globals::iPadHD` in `sl` and `&Globals::retinaDisplay` in `r8` for the whole
body; ours keeps the flag in `lr` (then `r5`, then `sl`) and the retina pointer
in `ip`, reloading it at `0xd27ec`/`0xd286e`.  Every source shape tried here
(hoist, pointer forms, `rd` placement, statement order) leaves the prologue
`ldrb.w lr,[r1]` unchanged.  A win still needs a source shape that makes clang
pick a callee-saved register for the flag/pointer; none of the local edits
above do.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13443
ulimit -v 12000000
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
# raw=5249 aligned=2725 size=7072/7072 insn=2520/2552
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13443/`: `Layout.cpp.base`,
`measure.sh`, `trial.py`, `batch1..10.py`, `randmove1.py`, `pool.py`,
`poolcmp.py`, `poolmulti.py`, `runs.py`, `region.py`, `ham.py`, `hdcmp.py`,
`diffview.py`, `orig.dis`, `ours.dis`.
