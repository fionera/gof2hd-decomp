# Layout::Layout C1/C2 — HD store table, the `(int)(float)` round-trip rule,
# and the irreducible residue (w4701, wave 97)

Worker **w4701**, branch `wave97-w4701`, base `9b58a39d5`.
Target: `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` at ELF VMA `0xd1028`, original size
**7072 B**. Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(Ghidra image base `0x10000`).

Continues `layout-layout-c1-c2-store-order-w4502.md` (the authoritative store
table and the landed 47.733 state), `layout-layout-c1-c2-order-revert-w4501.md`,
`layout-layout-c1-c2-vstr-shapes-w4401.md`,
`layout-layout-c1-c2-vstr-opacity-w4274.md`,
`layout-layout-c1-c2-midblock-vstr-w4267.md` and
`layout-layout-c1-c2-hd-block-order-w4221.md`.

## Result

**No landable improvement found.** The landed tree is at **7072 B /
47.7326968973747 %** (unrounded `fscore.py`, `autojunk=False`). This pass adds
one new *mechanism* result (§2, the `(int)(float)` fold rule), a
window-by-window match map (§4), and a set of exhaustive negative searches
(§5, §6) that together pin the residue to a single cause: the original's
mid-block `int`-temp + real-phi shape for the `(int)(float)` fields, which
cannot be reproduced without also changing the function size.

Gate is unchanged and green (`check` run on the pristine tree):

| metric | value |
|---|---:|
| `byte_exact` | 4500 / 4524 |
| `linked_exact` | 4500 / 4524 |
| `size_exact` | 4524 / 4524 |
| `stub_zero_size` / `missing` / `wrong_type` / `extra` | 0 |
| `data_exact` | 843 / 843 |
| imports | 207 |
| lint / drift | CLEAN / 0 |

`RATCHET PASS`.

## 1. HD store table (re-verified, unchanged from w4502 §2.1)

`stores2.py` over `objdump -d --start-address=0xd1028 --stop-address=0xd2bc8`
reproduces w4502's table exactly.

Original HD block `0xd1042..0xd1268`: **42 store groups, 137 fields**, in this
order (offset / width / instruction class):

```
0x014 w16 vst1  0x004 w16 vst1  0x024 w16 vst1  0x034 w16 vst1
0x044 w16 vst1  0x054 w16 vst1  0x064 w16 vst1  0x2c8 w16 vst1
0x07c w16 vst1  0x074 w8  strd  0x2c0 w8  strd  0x2d8 w4  str
0x0ac w4  str   0x08c w8  strd  0x094 w16 stmia 0x0a4 w8  strd
0x0c0 w16 vst1  0x0b0 w16 vst1  0x0ec w16 vst1  0x294 w16 vst1
0x2a4 w16 vst1  0x108 w16 vst1  0x118 w16 vst1  0x0d0 w12 stmia
0x0dc w8  strd  0x0e4 w8  strd  0x0fc w8  strd  0x104 w4  str
0x2b4 w8  strd  0x128 w16 vst1  0x140 w16 vst1  0x150 w16 vst1
0x160 w16 vst1  0x170 w16 vst1  0x180 w16 vst1  0x190 w16 vst1
0x1a0 w16 vst1  0x1b0 w16 vst1  0x1c0 w16 vst1  0x1d0 w16 vst1
0x138 w8  strd  0x1e0 w4  str
```

Our HD block `0xd1042..0xd1274`: **50 store groups, 137 fields**.

The three known differences (w4502 §2.3) still hold:

* **field-set**: original-only `0x94 0x140 0x144`; ours-only `0x204 0x23c
  0x2bc`. Values are identical (emulator, all five flag configs); only the
  block placement differs.
* **vector phase**: the original groups `0x140..0x17c` as four aligned
  four-word vectors (`0x140,144,148,14c | 0x150,154,158,15c | 0x160,164,168,16c
  | 0x170,174,178,17c`); ours is shifted two words because `0x140`/`0x144` are
  not adjacent to `0x148`/`0x14c` in our source.
* **RA**: the original prologue saves six registers (`r6,r7,r8,r9,sl,fp`);
  ours saves seven (`+r5`).

The HD block is a **pure straight-line store sequence in both binaries** (no
branches other than the final `b.w 0xd21e2`), so the whole remaining HD gap is
statement order + SLP grouping + register allocation.

**Value table.** The HD-branch values are constant-folded (independent of the
flag tests), so they carry no statement-order information. They were decoded
with `emu2.py` and are reproduced by the current source in all five
configurations (`emu2.py` diff over original vs. built `.so` is empty for
HD=1, n9=1, iPad=1, iPadLarge=1, retina=1 and default).

## 2. New mechanism result: when does `(int)(float)v` survive?

The original has **27 `vstr` stores**; ours has 25. The missing ones are
`0x9c 0x150 0x158 0x15c 0x170 0x1e4 0x3f8`; the extra ones are
`0x20 0x10c 0x148 0x1f4 0x204 0x3e8`.

Two distinct shapes produce a `vcvt.s32.f32` + `vstr`:

* **float-constant select** — `field = (int)(cond ? 8.0f : 4.0f)`. The
  original's `0x9c`, `0x150`, `0x1e4` are of this form (the float constants
  are loaded from `.text` literal pools at `d1964`/`d198a`, `d1cd4`/`d1cec`,
  `d1d88`/`d1d8c`).
* **int temp round-trip** — `vmov s0, rN; vcvt.f32.s32; vcvt.s32.f32; vstr`.
  The original's `0x158`, `0x15c`, `0x160`, `0x170` are of this form
  (`d1da6..d1db8`, `d1dd8..d1de6`, `d1e02..d1e12`, `d1f24..d1f34`).

The second shape is what previous notes could not reproduce ("every source form
folds to an int select"). The rule was pinned down with minimal `-Oz` tests
(`/dev/shm/w4701/t/t3.cpp`, `t4.cpp`, `t5.cpp`, exact project flags):

| source | emitted |
|---|---|
| `int v; if/else chain; s->y = (int)(float)v;` (v used **once**) | folds to an int select |
| same, but `v` used **twice** in the same block (`s->x = v;`) | **keeps** `vmov/vcvt/vcvt/vstr` |
| same, but `v` used in a later branch (`if (v) ...`) | folds |
| `(int)(float)(cond ? 70 : 42)` (a select, not a phi) | folds |

So the transform `fptosi(sitofp(v)) -> v` is applied by this clang only when
the operand has a **single use** and is not a real phi; a real `if/else` phi
with a second same-block use survives. This is exactly the original's shape:
the branches are real (each branch stores other fields), and the temp is used
more than once.

**Consequence:** reproducing `0x158/0x15c/0x160/0x170` requires a shared int
temp with ≥2 uses and a real branch chain. Every attempt to write that in our
ternary-per-field tree either folds (1 use) or changes the function size
(§6), and the size must stay 7072.

## 3. `0x150` / `0x1e4` are the same value in every configuration

Emulator value equality (all five flag configs) groups the fields; the groups
that matter here are:

```
{0x150, 0x1d8, 0x1e4, 0x210} = HD 5, n9 6, iPad 4, iPadLarge 4, retina 8, default 4
{0x180, 0x188}               = HD 23, n9 30, iPad 17, iPadLarge 17, retina 34, default 17
{0x144, 0x1b0}               = HD 18, n9 19, iPad 13, iPadLarge 13, retina 26, default 13
{0x10c, 0x124, 0x128, ...}   = HD 1, n9 2, iPad 1, iPadLarge 1, retina 2, default 1
```

`0x150` and `0x1e4` both round-trip in the original and are far apart in the
source, which suggested a shared temp. Writing

```cpp
int scaleSmall = Globals::iPadHD ? 5 : (...);
this->field_0x150 = (int)(float)scaleSmall;
...
this->field_0x1e4 = (int)(float)scaleSmall;
```

does emit the two round-trips, but the long live range wrecks RA:
**7056 B / 40.358 %** (measured). Not landable.

`0x210` (same value) is a plain `strd` in the original (`d23b0`), so it is not
part of the shared temp.

## 4. Window-by-window match map

`regwin.py` (difflib over the reconciled normalized instruction streams,
`autojunk=False`) over 0x100-byte windows of the original:

```
0d1028   29/ 80 =  36.2%    0d1928   26/ 79 =  32.9%
0d1128   15/ 76 =  19.7%    0d1a28   27/ 70 =  38.6%
0d1228   42/ 81 =  51.9%    0d1b28   15/ 73 =  20.5%
0d1328  119/127 =  93.7%    0d1c28   27/ 78 =  34.6%
0d1428   98/125 =  78.4%    0d1d28   32/ 96 =  33.3%
0d1528   61/101 =  60.4%    0d1e28   20/ 85 =  23.5%
0d1628   67/ 97 =  69.1%    0d1f28   14/ 99 =  14.1%
0d1728   58/ 94 =  61.7%    0d2028   12/ 76 =  15.8%
0d1828   46/ 94 =  48.9%    0d2128   24/ 77 =  31.2%
                            0d2228   20/ 69 =  29.0%
                            0d2328   27/ 78 =  34.6%
                            0d2428   24/ 80 =  30.0%
                            0d2528   28/ 84 =  33.3%
                            0d2628   45/ 91 =  49.5%
                            0d2728   31/ 88 =  35.2%
                            0d2828   82/123 =  66.7%
                            0d2928   40/126 =  31.7%
                            0d2a28  121/126 =  96.0%
                            0d2b28   50/ 79 =  63.3%
```

The worst windows are `0xd1128` (HD block, 19.7 %), `0xd1f28` (14.1 %),
`0xd2028` (15.8 %) and `0xd1b28` (20.5 %). `0xd1f28..0xd2128` is the
`0x170..0x18c` chain — the `(int)(float)` `0x170` round-trip plus the
`0x174/0x178/0x180/0x184/0x188/0x18c` per-flag if/else chain.

## 5. Exhaustive negative searches (this pass)

All sizes from the real compiler; percentages from the official
`fscore.py` unless marked "fast".

### 5.1 Statement order

* **Adjacent-swap hill-climb** over all 204 top-level statements (203 evals,
  `hill.py`): exactly one size-7072 swap improves the *fast* score —
  `0x19c` ↔ `0x1a8` (fast 47.0102 → 47.0796). Measured officially it
  **regresses: 7072 B / 47.6834 %**. No other swap improves.
* **500 random contiguous block moves** (block length 2–10, `randmove.py`):
  no size-7072 improvement over 47.0102 (fast).
* **Full 5! permutation of the `0x16c/0x170/0x174/0x178/0x17c` statements**
  (`perm2.py 107,108,109,110,111`): every permutation produces *identical*
  code (fast 47.0102) — clang reorders these independent stores freely.
* **Full 5! permutation of the `0x20c/0x1fc/...` group** (146–150): best
  7064 B / 47.038 (fast); no 7072 improvement.
* **Full "original store order" reorder** (w4502 §5): 6808 B / 32.6 %.
* **`0x2d0/0x2cc/0x2c8/0x2d8/0x2d4` permutation** (w4502): current order is
  the unique best.

### 5.2 Expression edits, single

Official, base tree (all `usize` in bytes, original 7072):

| edit | `usize` | raw % |
|---|---:|---:|
| `0x148` HD branch → `2` (int) | 7056 | 47.581 |
| `0x20` HD branch → `7` (int) | 6928 | 13.471 |
| `0x10c` HD branch → `1` (int) | 7008 | 42.675 |
| `0x204` HD branch → `12` (int) | 7072 | 47.435 |
| `0x1f4` HD branch → `22` (int) | 7056 | 47.701 |
| `0x3e8` HD branch → `140` (int) | 7076 | 42.428 |
| `0x158` all-float (`70.0f` …) | 7072 | 47.733 (code-identical) |
| `0x15c` all-float | 7072 | 47.733 (code-identical) |
| `0x170` all-float | 7072 | 47.733 (code-identical) |
| `0x160` all-float | 7088 | 47.683 |
| `0x150` all-float | 7080 | 47.246 |
| `0x150` all-float + `0x1f4` int | 7064 | 47.213 |
| `0x150` all-float + `0x148` int | 7064 | 47.054 |
| `0x150` all-float + `0x204` int | 7072 | 46.986 |
| `0x150` all-float + `0x9c` ternary | 7080 | 46.998 |

Note the three "all-float" forms for `0x158/0x15c/0x170` are **code-identical**
to the int form — clang folds them, confirming §2.

### 5.3 Exhaustive edit subsets

* **2^10 = 1024 subsets** of
  `{0x150allf, 0x9cf, 0x158r, 0x15cr, 0x170r, 0x1e4f, 0x3f8f, 0x204i, 0x148i,
  0x1f4i}` on the base tree: **57 subsets land at exactly 7072 B**, and the
  best of them is the empty set (fast 47.0102). The best non-empty is
  `{0x3f8f, 0x1f4i}` at fast 46.9315.
* **2^11 = 2048 subsets** of the same set plus `0x160f` on the 7024 B
  `rewardBoxY2` body (§6): **zero subsets land at 7072 B**.

## 6. The `rewardBoxY2` (0x3f8) lead and why it cannot land

Deleting the two redundant branches

```cpp
else if (Globals::iPadLarge)
    this->rewardBoxY2 = (Globals::retinaDisplay ? 254 : 127);
else if (Globals::retinaDisplay)
    this->rewardBoxY2 = (Globals::iPad ? 144 : 186);
```

leaves `if (iPadHD) 132; else if (n9) 111; else (rewardBoxY + 17) << retina`,
which is **semantically identical** (`(110+17)<<1 = 254`, `(76+17)<<1 = 186`,
`(55+17)<<1 = 144`) and measures **7024 B / 47.92332268370607 %** — a *higher*
ratio than the landed 47.733. It is not landable because `usize` must stay
7072: `size_exact` is 4524/4524 and any size change drops it.

The original is 48 B longer than this body. The natural explanation is the
four surviving `(int)(float)` round-trips (§2), each ≈ 12 B
(`vmov`+`vcvt`+`vcvt`+`vstr` vs. one `str`). But every attempt to add them back
changes the size by the wrong amount:

| variant on the 7024 body | `usize` | raw % |
|---|---:|---:|
| `0x150` all-float | 7032 | 47.475 |
| `0x1e4` float | 6992 | 45.847 |
| `0x9c` ternary | 7024 | 47.634 |
| `0x158` reorder (`retina` before `n9`) | 7040 | 47.706 |
| `0x15c` reorder | 7040 | 47.667 |
| `0x170` reorder | 6968 | 47.292 |
| `0x204` int | 7024 | 47.625 |
| `0x148` int | 7016 | 47.653 |
| `0x1f4` int | 7016 | 47.773 |
| `0x20` int | 6872 | 13.525 |
| `0x124` int | 6948 | 38.552 |
| `0x160` all-float | 7048 | 47.836 |

and no subset of the 11 edits reaches 7072 (§5.3).

## 7. Irreducible-residue statement

The remaining 52.27 % of `Layout::Layout` is **not** reachable by any local
source edit on the current tree. Precisely:

1. **HD block (0xd1042..0xd1268, 163 instructions, 25.8 % match).** Both
   binaries are straight-line. The original's `0x140..0x17c` four-word vector
   run requires `0x140`/`0x144` to be source-adjacent to `0x148`/`0x14c` and
   `0x94` to sit in the HD-specific region, while `0x204`/`0x23c`/`0x2bc` must
   move out. Every individual move loses (w4502 §5); the 5! permutation of the
   `0x16c..0x17c` group is code-invariant (§5.1). The residual is SLP phase +
   six-vs-seven-register prologue, i.e. RA, not source text.
2. **Mid block (0xd1860..0xd215c, 734 instructions, 29.2 % match).** The
   original's `0x158/0x15c/0x160/0x170` are `(int)(float)` round-trips of a
   real int phi with ≥2 uses (§2). Reproducing them needs a shared int temp
   and a real per-flag branch chain; in our per-field-ternary tree the value
   has one use and folds, and every shared-temp form tried changes `usize`
   (§3, §6). `0x9c`/`0x150`/`0x1e4` are float-constant selects; `0x150` and
   `0x1e4` reproduce exactly but cost +8 B each and no size-neutral partner
   exists (§5.3).
3. **`rewardBoxY2` (0x3f8).** The redundant-branch removal is a +0.19 pp win
   at −48 B, but the size constraint forbids it (§6).
4. **Everything else** (`0xd1328..0xd1928`, `0xd2228..0xd2b28`) is already
   49–96 % matched and dominated by the same RA/scheduling residue; no
   statement-order change improves it (§5.1).

In short: the tree is a strict local optimum under (a) all 203 adjacent
statement swaps, (b) 500 random 2–10-statement block moves, (c) all 120
permutations of the two most suspicious 5-statement groups, (d) all 1024
single-edit subsets and (e) all 2048 subsets on the 7024 B body. The residue is
the original's int-temp/real-phi expression shape, which is size-coupled to
the 7072-byte budget.

## 8. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8          # RATCHET PASS
python3 /dev/shm/w4701/fscore.py                            # [7072, 7072, 47.7326968973747]
python3 /dev/shm/w4701/regwin.py                            # window match map
python3 /dev/shm/w4701/stores2.py /dev/shm/w4701/orig.dis 0xd1042 0xd1268
python3 /dev/shm/w4701/emu2.py _ZN6LayoutC1Ev iPadHD=1 n9=0 iPad=0 iPadLarge=0 retinaDisplay=0
```

Scratch (`/dev/shm/w4701`): `fscore.py` (official unrounded), `fastscore.py`
(local 0.4 s scorer; **under-reports and mis-ranks** — always confirm),
`stores2.py`, `emu2.py`, `regwin.py`, `regdiff.py`, `units2.py`,
`perm2.py`, `randmove.py`, `subsetbase.py`, `subset7024.py`, `try1.py`,
`edits.py`, `var.py`, `Layout.base.cpp`, `Layout_7024.cpp`, `t/t3.cpp`,
`t/t4.cpp`, `t/t5.cpp`.
