# `Layout::Layout` C1/C2 — wave 130 / worker w7665
## NEW oracle: the Unity-remake tree ships a Ghidra decompile of a sibling `libgof2hdaa.so`

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original
vaddr `0xd1028`, size **7072 B (0x1ba0)**, source `src/game/ui/Layout.cpp`.
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(sha256 `d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483`).
Baseline = wave-129 final `003a9623a` (7072 B / 48.943802311678 % unrounded, 2529/2489 insns).

## Result — no landable source win (note only)

| metric | before (HEAD) | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** (unchanged) |
| unrounded `difflib` ratio | 48.943802311678 % | **48.943802311678 %** |
| `byte_exact` / `linked_exact` | 4506 / 4506 | **4506 / 4506** |
| `size_exact` / `data_exact` | 4524 / 843 | 4524 / 843 |
| imports / lint / drift | 207 / CLEAN / 0 | 207 / CLEAN / 0 |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`.
No `src/` change landed; the worktree is clean apart from this note.

## 1. NEW artifact: a second-version Ghidra decompile of `Layout::Layout`

`_work/oracles/Galaxy-on-Fire-2-Unity-Remake/Reference/decompiled/native/game/Layout.c`
contains a full Ghidra 12.1.4 pseudocode decompile of `Layout::Layout` from a
**sibling `libgof2hdaa.so`** (the checked-in binary is a 132-byte LFS pointer;
only the decompile is usable).  Its `_FUNCTIONS.tsv` entry is

```
Layout::Layout  000e2a00  8140  true      (image base 0x10000 -> vaddr 0xd2a00)
```

i.e. a *different, larger* build than our 7072-B `0xd1028` body, but the same
source family: the shared prefix values match almost everywhere
(`0x2d0` n9 = 53, `0x2d4`/`0x2d8` = 100/160/125/200, `0x94` 15/20/10,
`0x98` 630/840/420, `0x9c` 75/100/50, `0xac` 122/61/82/41, …).  Only a handful
of constants differ (`0x7c` n9 = 13.5 vs our 13, `0x78` n9 = 473.4 vs 473),
and the sibling has extra tail fields (`0x3e0..0x3fc`) and a 1044-B `reload`
(ours 1064).

The decompile is the first *source-shaped* oracle for this constructor.  It
shows the original as a field-major sequence of per-field `if (n9) … else
if (retina) …` blocks (Ghidra's rendering of the ternaries), with the field
order:

```
4 8 c 10 14 18 1c 20 24 28 2c 30 34 38 3c 40 44 48 4c 50 54 58 5c 60 64 68
6c 2c0 70 74 78 2c4 2c8 2cc 2d0 2d4 2d8 7c 80 84 88 8c 90 94 98 9c ac a8
a0 a4 b0 b8 b4 bc c4 cc d4 dc e4 ec f0 f4 f8 100 294 29c 2a4 2ac 2b4 2b8
108 10c 110 118 120 128 130 138 140 148 150 158 160 168 170 174 178 17c
180 184 188 18c 190 198 1a0 1a8 1b0 1b8 1c0 1c8 1d0 1d8 1e0 c0 c8 d0 d8 e0
e8 fc 104 298 2a0 2a8 2b0 114 11c 124 12c 134 13c 144 14c 154 15c 164 16c
194 19c 1a4 1ac 1b4 1bc 1c4 1cc 1d4 1dc 1e4 1e8 1ec 1fc 204 20c 214 21c
224 22c 234 238 23c 244 24c 254 25c 260 264 26c 274 278 27c 280 …
```

i.e. **ascending by offset inside groups**, with `2c0..2d8` after `6c/70..78`,
`ac,a8` before `a0,a4`, and the second group of each 8-byte stride
(`c0,c8,d0,…`) after the first.  The current source order is scrambled
(`4 14 1c 10 c 18 8 …`) — it is a hill-climbed order, not the oracle order.

## 2. Oracle-order experiments (real `link_so` builds, exact unrounded ratio)

All variants were generated mechanically from the oracle first-occurrence
order (statements kept verbatim; if/else blocks kept as units).

| variant | size | ratio % |
|---|---:|---:|
| **HEAD baseline** | **7072** | **48.943802** |
| whole constructor reordered to the oracle order | 6544 | 11.828631 |
| common1 first part (`2d0..9c,ac,a8,a0,a4,b0`) in oracle order | 6928 | 45.447220 |
| `0x7c/0x80/0x84/0x88` ascending (oracle order) | 7072 | 48.824233 |
| `0x94` moved between `0x90` and `0x98` | 7040 | 46.498599 |
| `0x94` move + `0x7c..0x88` ascending + int `0xa8` | 7000 | 46.641267 |
| `0x154/0x158/0x15c` explicit nested `if/else` tree (see §3) | 7060 | 18.607419 |
| int-typed `field_0xa8` (no `(int)(float)` round-trip) | 7032 | **49.011384** |

The oracle order is *not* the source order of our binary: applying it
collapses the body by 528 B (6544) and the fuzzy score to 11.8 %.  The
current scrambled order is a strong local optimum of the fuzzy metric, and
the oracle's ascending order is a different compiler/version's schedule.

## 3. `0x154/0x158/0x15c` — the duplicated common1 tail is real but not a size anchor

The oracle and the original disassembly agree on the shape:

```
if (iPadLarge) { 0x154=100; 0x158=100; 0x15c=294; v158=100; }
else if (iPad) { 0x154=50; v158=50; }
else if (!n9)  { 0x154=retina?30:15; v158=retina?28:14; }
else           { 0x154=14; v158=13; }
0x158 = (int)(float)v158;        // duplicates the iPadLarge arm store
0x15c = (int)(float)(...);       // separate phi chain
```

I transcribed exactly this tree (with the `(int)(float)` tail) into the
source.  It compiles to **7060 B / 18.6 %** — the tree is structurally right
but the fuzzy metric collapses because the surrounding field-major order no
longer aligns.  It does not recover the 40 B that the int-`0xa8` form drops.

## 4. NEW store-count census (original vs HEAD)

A base-register-tracked `str/strd/str.w/vstr/vst1/stmia` tracer over
`0xd1028..0xd2bc8` gives the per-offset store counts.  The differences are
the duplicated common1 tail:

```
offset  orig ours      offset  orig ours
0x98     2    3        0x180    4    3
0x140    3    1        0x184    6    5
0x154    5    4        0x188    4    3
0x158    3    4        0x18c    6    4
0x15c    3    4        0x190    6    5
0x16c    5    4        0x194    4    3
0x170    3    4        0x198    5    4
0x178    5    4        0x19c    5    4
0x17c    5    4        0x1a4    3    6
0x1e0    3    5        0x1f0    1    3
0x200    3    1        0x28c    1    3
0x3e8    5    1        0x3ec    2    1
0x3f8    3    2        0x3fc    1    3
```

The original writes `0x154..0x19c` more often and `0x1a4/0x1e0/0x1f0/0x200/
0x28c/0x3e8` less often than HEAD.  `0x3e8` (rewardBoxX) is the clearest
config-major tail: the original stores it in **five** arms
(`d226e,d24c8,d24d4,d24f6,d2508`), HEAD once (`d2580 vstr`).  Reproducing
that tail needs the reward-box block rewritten as a config tree, not the
single ternary at `src/game/ui/Layout.cpp:279`.

## 5. Conclusion / open questions

* The oracle is a genuine second version of the same source and is now
  documented here; it is the best available source-shaped reference.
* The oracle's field order is **ascending-by-group**, but applying it to our
  binary shrinks the body and loses the fuzzy alignment.  The current
  hill-climbed order must be kept until the *tail* (`rewardBoxX/Y/Height`,
  `0x3e8..0x3fc`) is reconstructed as the original's config tree.
* The int-typed `field_0xa8` remains the single best function-level score
  (49.011 % at 7032 B) and is still exactly 40 B short; the missing 40 B is
  *not* the `0x154` duplicate tail (that variant is 7060 B).
* Next step: transcribe the reward-box tail (`d24b2..d2548`) as a config
  tree, then re-test the int-`0xa8` form on top of it.  That block is the
  largest single store-count discrepancy (`0x3e8` 5 vs 1) and is the most
  likely 40-B anchor.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w7665
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
python3 /dev/shm/w7665/score.py                          # 7072 / 48.943802311678
```

Scratch harnesses (not committed, `/dev/shm/w7665/`): `score.py` (exact
single-function scorer), `stores.py` (store-offset tracer), `reorder.py`
(oracle-order generator), `Layout.base.cpp` (HEAD snapshot), `orig.dis` /
`ours.dis`, `orig_stores.txt` / `ours_stores.txt`.
