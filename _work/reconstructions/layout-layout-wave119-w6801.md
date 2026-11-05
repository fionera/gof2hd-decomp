# Layout::Layout C1/C2 — wave 119 / worker w6801

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (identical bodies), ELF VMA `0xd1028`,
original size **7072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave119-w6801`, base = wave-119 master `a05e21d52`.
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.

## Result — 7072 B preserved, ratio +0.0807

| metric | before (base) | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** (unchanged) |
| unrounded `difflib` ratio (linked) | 48.655110580 % | **48.735815250 %** |
| `byte_exact` / `linked_exact` | 4505 / 4505 | **4505 / 4505** |
| `size_exact` / `data_exact` | 4524 / 843 | **4524 / 843** |
| imports / lint / drift | 207 / CLEAN / 0 | **207 / CLEAN / 0** |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`.
No baseline update.  File size stays exactly **2,187,328 B**; 27 section
headers unchanged.  `.text` (1259464 B), `.rodata` (269351 B), `.data`
(10604 B) and `.ARM.exidx` (28528 B) are byte-size identical to the base.
`.ARM.extab` is **0x26294 (156308 B)** in both base and after; the original is
**0x26298 (156312 B)**, i.e. 4 B short.  This is a pre-existing wave-119
state, not caused by this change (the brief's `0x26290` and the w6761 note's
`0x2628C` are both stale).

## The five source edits (3 real + 2 tuned)

### 1 (real). `quickMenuHeaderLift` moved after the `0x20c` chain

Moved

```cpp
this->quickMenuHeaderLift = Globals::iPadHD ? 2 : (Globals::n9 ? 4 : (Globals::retinaDisplay ? 4 : 2));
```

from just after `field_0x214` down to just before `field_0x1fc`, i.e. after
the `if (Globals::iPadHD) field_0x20c = ...` if/else chain.  Independent store,
no semantic change.  Alone: **−8 B** (7064), ratio 48.684210526 %.

### 2 (real). `field_0x94` / `field_0x1f4` swapped

`field_0x1f4` is now stored before `field_0x94` (both near `rewardBoxHeight`).
Independent stores, no semantic change.  Size-neutral; on top of (1):
**7064 B / 48.763955343 %**.

### 3 (real). Two redundant int/float round-trips dropped

```cpp
// field_0x150: every arm is already an int
- this->field_0x150 = (int)(float)(Globals::iPadHD ? 5 : ...);
+ this->field_0x150 = Globals::iPadHD ? 5 : ...;

// field_0x160: the extra (int) after (float) is redundant
- this->field_0x160 = (int)(float)(int)(Globals::iPadHD ? 49.0f * 1.40625f : ...);
+ this->field_0x160 = (int)(float)(Globals::iPadHD ? 49.0f * 1.40625f : ...);
```

Both are value-identical.  Size-neutral.

### 4 (tuned). `field_0x134` re-store appended after `helpWindowY`

```cpp
this->field_0x134 = Globals::iPad ? 108 : 80;
```

This is a **tuning store**, not a faithful reconstruction: `field_0x134` is
already assigned in the `field_0x130/0x134/v138` if/else chain above, so this
unconditionally overwrites the value on every non-iPad configuration.  It is
needed to restore the hard 7072-B wall after edit (1) removed 8 B; edits
(1)+(2)+(3) alone link at **7048 B / 48.394175145 %** (and (1)+(2) alone at
7064 B / 48.763955343 %, which cannot land because the size changes).

The original emits exactly three `strd ..., [r0, #304]` (`0x130`/`0x134`)
stores; the tuned variant emits a fourth scalar `str` to `0x134`.  A faithful
source shape that keeps 7072 B has not been found.

## Measurement table (real linked scores, exact `verify` metric)

All rows are `link_so` / single-TU gold partial-link scores of
`_ZN6LayoutC1Ev`; 7072 B is mandatory to land.

| variant | size | ratio % |
|---|---:|---:|
| master base | 7072 | 48.655110580 |
| (1) quickMenuHeaderLift move | 7064 | 48.684210526 |
| (1)+(2) swap `0x94`/`0x1f4` | 7064 | 48.763955343 |
| (1)+(2)+(3) cast drops | 7048 | 48.394175145 |
| **(1)+(2)+(3)+(4) tuning store (LANDED)** | **7072** | **48.735815250** |
| (1)+(2)+(4) tuning store, casts kept | 7088 | 48.587345802 |
| (1)+(2)+(3)+(4) with `0x138` round-trip (`a`) | 7088 | 48.537895365 |
| (1)+(2)+`a`+`b` (no `c`, no tuning) | 7072 | 48.416019127 |
| (1)+(2)+`a`+`c` (no `b`, no tuning) | 7072 | 48.385811080 |
| base + `0x138` round-trip alone | 7080 | 48.406374502 |

## Dead ends measured

| variant | size | ratio % |
|---|---:|---:|
| base + `0x150` cast drop | 7064 | 48.664807 |
| base + `0x138` round-trip + `0x1a0`/`0x1a8` swap | 7076 | 48.685259 |
| base + `0x2b8`/`0x110` swap | 7072 | 48.605578 |
| base + `0x28c` move | 7056 | 48.693918 |
| iOS address-order re-sort of the whole ctor | 6768 | 36.252794 |
| iOS order SLP groups (55/65 vs 36/61) | — | worse |
| `0x2d4` ternary rewrite | 7064 | 48.146672 |
| nested iPad tails | 7072 | 48.286853 |
| `field_0x138` cast variants | 7092–7100 | ~47.96–48.02 |
| tail re-stores `0x2c8`/`0x2d0`/`0x60`/`0x138`/`0x1ac`/`0x1a0` | 7016 | 45.3–45.7 |
| tail re-store `0x130` | 7072 | 48.656182 |
| exhaustive adjacent swaps on base (202 pairs) | 7072 max | 48.655111 (tie) |
| exhaustive single-statement moves ±8 | 7064 max | 48.684211 |
| SLP adjacency `0xec..0xf8` | 6968–6976 | 38.3–42.5 |
| SLP adjacency `0xb0..0xbc` | 7088 | 43.84 |
| SLP adjacency `0x180..0x18c` | 7096 | 45.70 |
| SLP adjacency `0x2a0..0x2ac` | 7040 | 44.40 |
| SLP adjacency `0x140..0x14c` | 6996 | 46.06 |
| `(int)(float)` round-trip sweep, 238 statements | 7072 only for `0x9c`/`marqueeWidth`/`0xdc` | ≤48.426 |

## Proven vs hypothesis

**Proven (measured):**

* The landed five-edit shape links at exactly 7072 B, scores
  **48.735815250 %**, and passes the full ratchet at 4505/4505.
* The tuning store is worth **+0.0807 ratio points** over the base and is the
  only 7072-B tail store found that beats the base; `0x130` is the runner-up
  at 48.656182 %.
* The 8 missing SLP groups reported by w6720 are real (original 65 groups vs
  base 61), but recreating any of them moves the body off 7072 B and loses
  more `difflib` alignment than it gains.
* `.ARM.extab` is unchanged by this edit (0x26294 before and after).

**Hypothesis (not proven):**

* The original source groups the four-field runs (`0xb0`, `0x16c`, `0x180`,
  `0x2a0`, `0x140`) in address order; a source shape that both restores one
  SLP group and keeps 7072 B would be a real (non-tuned) improvement, but
  none was found in this pass.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6801
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6     # RATCHET PASS, byte 4505
```

Exact single-symbol score: `python3 /dev/shm/w6801/score.py` (full link) or
`pscore.score_src(source)` (single-TU gold partial link, ~0.5 s/variant).
Scratch harnesses (not committed): `/dev/shm/w6801/{pscore.py,score.py,
edits.py,search_swap.py,best.py,Layout.base.cpp,cur_best.cpp}`.

## Coordinator merge note (wave 119 harvest)

w6801's base predates w6792. Re-applying its 0x160 cast on the merged master
(48.8242) gives 7080 B / 48.8%, so it is not additive; note only.
