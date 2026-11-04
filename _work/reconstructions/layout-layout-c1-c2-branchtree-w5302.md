# Layout::Layout C1/C2 — per-device block tree, the `rewardBoxY2` true source,
# and the float-round-trip (`vstr`) set (w5302, wave 105)

Worker **w5302**, branch `wave105-w5302`, base `835748c18`.
Target: `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` at ELF VMA `0xd1028`, original size
**7072 B**. Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(Ghidra image base `0x10000`, so the ctor is `0xe1028` in the Ghidra project).

Independent second angle on the same two constructors. Focus: the
`Globals::iPad/iPadHD/retinaDisplay/n9` branch tree, the float/int
conversions, and the original field-store order. Read alongside
`layout-layout-c1-c2-pass11-w4802.md` (the landed 47.733 state),
`layout-layout-c1-c2-order-search-w4901.md`, `layout-layout-c1-c2-store-order-w5001.md`,
`layout-layout-c1-c2-ios-order-wf101.md`, `layout-layout-c1-c2-literal-pool-constants-wf102.md`,
`layout-layout-c1-c2-oracle-block-structure-wf301.md` and
`layout_ctor/ios_source_order.md`.

## Result

**No landable improvement found.** The tree stays at **7072 B /
47.77247414478918 %** (unrounded official `fscore.py`, `autojunk=False`).
This pass contributes four new, independent results:

1. **The true source of `rewardBoxY2` is a 3-arm chain**, not the 5-arm chain
   currently checked in (§1). The original computes
   `(rewardBoxY + 17) << retinaDisplay` for *every* non-HD/non-n9 device.
   The 3-arm body compiles to **7024 B / 47.96325878594249 %** — a real
   +0.19 pp improvement — but is 48 B short of the required 7072 B.
2. **The per-device block-tree source organisation is wrong** (§2): it
   compiles to 5480 B / 10.32 %.
3. **The float-round-trip (`vstr`) set is a hard fingerprint** (§3): the
   original stores exactly seven int fields through `vstr`; our body stores a
   disjoint set. Making the seven fields float-typed does not reproduce the
   round-trip because clang folds constant arms.
4. **Order and expression space is exhausted** (§4): 4200 random size-filtered
   reorderings of the 204-unit body and 300 random float/int expression
   toggles find no 7072 B candidate above the landed 47.7725.

Gate before == after (verified with `check` on the pristine tree):

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

## 1. `rewardBoxY2` — the true source is 3-arm (new)

The current `src/game/ui/Layout.cpp` writes `rewardBoxY2` as five arms:

```cpp
if (Globals::iPadHD)
    this->rewardBoxY2 = 132;
else if (Globals::n9)
    this->rewardBoxY2 = 111;
else if (Globals::iPadLarge)
    this->rewardBoxY2 = (Globals::retinaDisplay ? 254 : 127);
else if (Globals::retinaDisplay)
    this->rewardBoxY2 = (Globals::iPad ? 144 : 186);
else
    this->rewardBoxY2 = (this->rewardBoxY + 17) << Globals::retinaDisplay;
```

The original's tail block (`d24bc..d252c`, ELF VMA) is:

```
d24bc  load Globals::iPadLarge
d24c4  cbz  r1, d24ea
d24c6  movs r1,#200 ; str r1,[r0,#1000]      ; rewardBoxWidth = 200
d24cc  movs r1,#110                          ; rewardBoxY     = 110
d24ce  b    d2514
d24ea  load Globals::iPad
d24f2  cbz  r1, d24fe
d24f4  movs r1,#100 ; str r1,[r0,#1000]      ; rewardBoxWidth = 100
d24fa  movs r1,#55                           ; rewardBoxY     = 55
d24fc  b    d2514
d24fe  movs r1,#150 ; (retina ? 150 : 75)    ; rewardBoxWidth
d250c  movs r1,#76  ; (retina ? 76  : 38)    ; rewardBoxY
d2514  add.w r2, r1, #17
d2518  lsl.w r2, r2, r8                      ; r8 = retinaDisplay
d251c  vmov  s0, r2
d2520  vcvt.f32.s32 s0, s0
d2524  vcvt.s32.f32 s0, s0
d2528  str.w r1, [r0, #1012]                 ; rewardBoxY
d252c  vstr  s0, [r0, #1016]                 ; rewardBoxY2
```

So **all** of `iPadLarge`, `retina` and the default arm share one
`(rewardBoxY + 17) << retinaDisplay` computation. The correct source is:

```cpp
if (Globals::iPadHD)
    this->rewardBoxY2 = 132;
else if (Globals::n9)
    this->rewardBoxY2 = 111;
else
    this->rewardBoxY2 = (this->rewardBoxY + 17) << Globals::retinaDisplay;
```

Measured (official unrounded scorer):

| body | `usize` | raw % |
|---|---:|---:|
| landed 5-arm | **7072** | **47.77247414478918** |
| 3-arm (true source) | 7024 | **47.96325878594249** |
| 3-arm as a single ternary | 7024 | 47.96325878594249 |
| 3-arm with `(int)(float)` round-trip | 7040 | 46.85691478746757 |
| 3-arm with `* (retina ? 2.0f : 1.0f)` | 7056 | 46.85007974481658 |
| 5-arm, retina arm `<< 1`, else `rewardBoxY+17` | 7064 | 47.81050955414013 |
| 5-arm, retina arm `<< retina`, else `rewardBoxY+17` | 7064 | 47.82002787178977 |

The 48 B gap is *not* in this field. The 3-arm body is the true source but is
not landable at 7072 B without a second, unidentified +48 B source difference
elsewhere.

## 2. Per-device block tree (new negative result)

I generated a full `if (iPadHD) { … } else if (n9) { … } else if (iPadLarge)
{ … } else if (retina) { … } else if (iPad) { … } else { … }` body from the
original's per-config stored values (emulator, all six flag configurations;
values are bit-identical to the landed body). It compiles to **5480 B /
10.317961676142346 %** — clang tail-merges the identical assignments and the
scheduling collapses. So the original's tail-merged branch tree is *not*
written as per-device blocks; it is per-field ternaries (consistent with the
iOS oracle).

## 3. The float-round-trip (`vstr`) set (new measurement)

The original stores these int fields through `vstr` (float store of a
converted int): `9c 150 158 15c 170 1e4 3f8`. Our 7072 B body stores
`20 10c 148 1f4 204 3e8` instead (plus the common `60 68 6c 70 8c 90 a8 dc
e0 e4 e8 138 160 168 1a8 1e0 2c8 2d0 2f8 3e0`). The mechanism is
`vmov s0,rN ; vcvt.f32.s32 ; vcvt.s32.f32 ; vstr` — i.e. the source RHS is a
float expression whose value is an int temp.

Making each of the seven fields float-typed (adding `.0f` to one arm, or
`(int)(float)` casts, or a `float` local) does **not** reproduce the
round-trip: clang folds every constant arm at `-Oz`. Measured on the 7024 B
3-arm body, none of the seven adds the missing 48 B:
`150 +16`, `158/15c/170/9c/3f8 0`, `1e4 −8`. The round-trip therefore requires
a genuinely non-constant int temp; the exact source shape remains open.

## 4. Exhaustive negative searches (this pass)

All sizes are real `link_so` sizes; percentages are the official unrounded
`fscore.py`.

* **Order**: 4200 random size-filtered moves (single-unit relocation, pairwise
  swap, block move length 1–25, segment reversal) from the landed 204-unit
  order. No candidate above 47.7725 at 7072 B. (Independent confirmation of
  w4901/w4802.)
* **Expression toggles**: 300 random 1–3-field float/int toggles (the 166
  single-line `Globals::iPadHD ? N …` units). No 7072 B candidate above
  47.7725.
* **`rewardBoxY2` arm forms**: 5 large-arm × 5 retina-arm × 4 else-arm
  combinations, all six device values checked with the emulator. The only
  correct 7072 B forms score ≤ 47.7725.
* **Region reorders**: the `0xac..0xcc` block to ascending / descending / iOS /
  `0xc0`-first order: 6900/6984/7016/7016 B, 39.9/40.2/43.5/43.0 %.
* **`rewardBoxY2` as a 3-arm body plus known size edits**: all 1–4 subsets of
  the eight size-changing float edits (`150,1bc,1b0,1b4,1b8,1e4,188,1fc`).
  The best 7072 B result is 45.89 %, below the landed tree.

## 5. Exact remaining diff

* Original: 2529 instructions, 7072 B (code `0xd1028..0xd2854`, end pool
  `0xd2854..0xd2bc8`, 221 words).
* Ours: 2499 instructions, 7072 B (code `0xd1028..0xd28c0`, end pool
  `0xd28c0..0xd2bc8`, 194 words).
* `difflib.SequenceMatcher(autojunk=False)` ratio **47.77247414478918 %**;
  1200 matched normalised instructions.
* Every stored byte agrees in all six flag configurations (emulator diff of
  the whole `this` image is empty). No float constant differs between the two
  functions' literal pools. The residue is instruction selection, SLP group
  phase and register allocation.

## 6. Next hypothesis

The 48 B that separate the true 3-arm `rewardBoxY2` body (7024 B) from the
original (7072 B) are the same 48 B that the landed 5-arm body spends on its
two redundant arms. They are most likely a **non-constant int temp that the
original stores through a float round-trip** (the `vstr` set). A future pass
should try to recover the exact Android mid-block expression shape that keeps
an int phi live in a general register and converts it through `s0` — e.g. a
source-level `int v = <ternary>; field = (float)v;` where `v` is reused by a
later field, which would defeat constant folding and add both the `vstr`
store and the missing code bytes.

## 7. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6          # RATCHET PASS
python3 /dev/shm/w5302/fscore.py                            # 47.77247414478918, 7072 B
```

Scratch (`/dev/shm/w5302`): `fscore.py` (official unrounded linked scorer),
`harness.py` (rewrite ctor body → `link_so` → score), `genblocks.py`
(per-device block generator), `dumpvals.py` / `dumpvals_ours.py` (emulator
store dump over the six flag configurations), `scan_edits.py` (per-field
float/int scan), `rb2_search.py` (rewardBoxY2 arm-form search),
`base7024.cpp` (the true 3-arm body).
