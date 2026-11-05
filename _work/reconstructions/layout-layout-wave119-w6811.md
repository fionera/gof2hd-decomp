# Layout::Layout C1/C2 — wave 119 / worker w6811

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (identical bodies), ELF VMA `0xd1028`,
original size **7072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave119-w6811`, base = current wave-119 master (`3981b32c5`).
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.

## Result — one 7072-B improvement landed

| metric | before (base) | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** (unchanged) |
| unrounded `difflib` ratio (full link) | 48.824232762 % | **48.833964521 %** |
| `report.json` pct (1 dp) | 48.8 | **48.8** |
| `byte_exact` / `linked_exact` | 4505 / 4505 | **4505 / 4505** |
| `size_exact` / `data_exact` | 4524 / 843 | **4524 / 843** |
| imports / lint / drift | 207 / CLEAN / 0 | **207 / CLEAN / 0** |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`
(verify: byte 4505, linked 4505, stubs 0, missing 0, extra 0, avg 99.94;
data 843/843; sodiff imports 207).  No baseline update.  File size stays
**2,187,328 B**; 27 section headers unchanged; `.text` = `0x1337c8`,
`.ARM.extab` = `0x26294` (unchanged; still 4 B short of the original
`0x26298`), `.ARM.exidx` = `0x6f70`, `.rodata` = `0x41c27` — all as in the
base, because the ctor body is still exactly 7072 B.

## The two source changes

### 1. `field_0x160` — restore the int→float→int round-trip (+8 B)

```cpp
// before
this->field_0x160 = (int)(Globals::iPadHD ? 49.0f * 1.40625f : ...);
// after
this->field_0x160 = (int)(float)(int)(Globals::iPadHD ? 49.0f * 1.40625f : ...);
```

This is the **faithful** form: the original stores `0x160` with an
int→float→int round-trip (w6778).  w6792 had removed it as a −8 B size
tuning; re-adding it alone gives **7080 B / 48.844621514 %** (a real
+0.0204 pp over the base, but 8 B too long).

### 2. `quickMenuHeaderLift` (0x22c) moved after `field_0x234` (−8 B)

```cpp
// before: between field_0x214 and the field_0x20c if/else chain
// after:  immediately after field_0x234
this->field_0x234 = ...;
this->quickMenuHeaderLift = Globals::iPadHD ? 2 : (Globals::n9 ? 4 : (Globals::retinaDisplay ? 4 : 2));
this->field_0x248 = ...;
```

Alone on the base this is 0 B / neutral.  Combined with (1) it is **−8 B**,
bringing the body back to exactly **7072 B** at **48.833964521 %**.

The move is a pure statement relocation (no value change); it changes the
store schedule around the `0x20c`/`orbitInfoTextX`/`0x218`/`orbitInfoStationY`
if/else chains enough to recover the 8 B without disturbing the literal-pool
layout that the fuzzy metric keys on.

## Measurement method

A **partial-link** scorer was used for the search (`/dev/shm/w6811/fast.sh`):
compile only `Layout.cpp` with the exact NDK r18b flags, gold-link that one
object into a throwaway `.so`, then run the `tools/verify/verify.py`
`Resolver`+`reconcile`+`difflib(autojunk=False)` pipeline on
`_ZN6LayoutC1Ev`.  It reproduces the full-link score bit-for-bit on the base
(48.824232762 %) and on the landed variant (48.833964521 %), at ~0.4 s per
variant.  Every candidate was required to land on **exactly 7072 B** (a 4 B
delta collapses `byte_exact` to ~2454 because every later function moves).

## Searches run (all real builds, 7072 B required unless noted)

| search | builds | best 7072 result |
|---|---:|---:|
| random 1–3 statement moves/swaps on the base | 2,000 | 48.824232762 (no win) |
| random 1–3 statement moves/swaps on base+`0x160` round-trip | 2,000 | **48.833964521** (landed) |
| random 1–3 statement moves/swaps on the landed body | 2,500 + 1,500 | 48.833964521 (local optimum) |
| all subsets (size 1–2) of 18 structural/typing edits on the base | 171 | 48.824232762 |
| random 2–5 subsets of the same 18 edits | 800 | 48.824232762 |
| all 6 arm orders of the `field_0x130` if/else → 3-ternary rewrite | 6 | best 7072 = 43.64 (the 7088 form is 48.825169) |
| `0x160` round-trip + every single/pair of the other 17 edits | 171 | 48.794100060 |
| full ctor reorder to the original block-C first-store order | 1 | 5024 B / 16.11 (dead end) |
| first-region (0x4..0x58) reorder to the original block-C order | 1 | 6980 B / 40.95 (dead end) |

### Structural findings (for the next pass)

* The original's block-C store order (recovered with a `strd`/`stmia`-aware
  parser) is **not** the source order: reordering the source to it collapses
  the body to 5024 B.  The current scrambled order is much closer to the
  original's actual source order than the store order suggests.
* The original's HD block has 27 four-word SLP groups, ours 26; block C has
  25 vs 20; the common tail has 16 vs 16.  The 8-group deficit quoted by the
  coordinator is concentrated in block C (`0xec`, `0x140`, `0x94/0x9c`,
  `0x23c`, `0x24c`, `0x25c`, `0x26c` windows).  Each individual move that
  forms one of those groups was measured and **lowers** the fuzzy ratio
  (e.g. `0xb0`→−48 B/42.69, `0xf0`→+4 B/42.70, `0x140`→−72 B/46.46,
  `0x94`→−64 B/46.26); they change the pool offsets that dominate the
  metric.  The faithful SLP shape is a byte-exactness move, not a fuzzy one.
* The original's four cold outlined tails (`0x130/0x134/0x138`, `t_0x2c8`,
  `0x2d0`, `0x60`) are all `iPad ? A : B` else-arms of a `retinaDisplay`
  test.  Ours keeps them inline and outlines one different tail
  (`field_0x1ac`).  The outlining decision is a global block-placement
  heuristic; no source-shape variation tried here moved it.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6811
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6     # RATCHET PASS, byte 4505
```

Scratch harnesses (not committed): `/dev/shm/w6811/{fast.sh,score.py,
search.py,search160.py,search_best1.py,rsearch.py,edits2.py,bc2.py,
align.py,groups.py}`, plus `Layout.base.cpp`, `Layout.best1.cpp`,
`cand1815.cpp`.
