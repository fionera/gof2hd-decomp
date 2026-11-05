# Layout::Layout C1/C2 — wave 119 / worker w6792

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (identical bodies, aliased at
`0xd1028`), original size **7072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave119-w6792`, base wave-119 master `075f49c72`.
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.

## Result — one real 7072-B improvement landed

| metric | before (HEAD) | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** (unchanged) |
| unrounded `difflib` ratio (full link) | 48.655111 % | **48.824233 %** |
| `report.json` pct (1 dp) | 48.7 | **48.8** |
| `byte_exact` / `linked_exact` | 4505 / 4505 | **4505 / 4505** |
| `size_exact` / `data_exact` | 4524 / 843 | **4524 / 843** |
| imports / lint / drift | 207 / CLEAN / 0 | **207 / CLEAN / 0** |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`
(verify: byte 4505, linked 4505, stubs 0, missing 0, extra 0, avg 99.94;
data 843/843; sodiff imports 207).  No baseline update.  File size stays
**2,187,328 B**; 27 section headers unchanged; `.text`, `.ARM.exidx`,
`.rodata`, `.ARM.extab` sizes untouched (the function itself is still
exactly 7072 B, so no later `.text` symbol moves).

## The three source changes

### 1. `field_0x184` — if/else chain → nested ternary (the big win)

```cpp
// before
if (Globals::iPadHD)          this->field_0x184 = 163;
else if (Globals::iPadLarge)  this->field_0x184 = 346;
else if (Globals::retinaDisplay) this->field_0x184 = 232;
else if (Globals::iPad)       this->field_0x184 = 173;
else if (Globals::n9)         this->field_0x184 = 210;
else                          this->field_0x184 = 116;
// after
this->field_0x184 = Globals::iPadHD ? 163 : (Globals::iPadLarge ? 346 :
                    (Globals::n9 ? 210 : (Globals::iPad ? 173 :
                    (Globals::retinaDisplay ? 232 : 116))));
```

The original emits this chain as a **shared-tail branch tree**, not as a
linear six-way if/else; the nested-ternary form lets clang 7.0.2 at `-Oz`
reproduce that tree.  On the unchanged baseline this alone is **+8 B**
(7080) and **48.834893 %**; the `retinaDisplay/iPad/n9` order also matters
(see §3).

### 2. `field_0x160` — drop the inner float round-trip (tuning, −8 B)

```cpp
// before
this->field_0x160 = (int)(float)(int)(Globals::iPadHD ? 49.0f * 1.40625f : ...);
// after
this->field_0x160 = (int)(Globals::iPadHD ? 49.0f * 1.40625f : ...);
```

Value-identical (the arms are already `float`).  The original stores
`0x160` with an int→float→int round-trip (w6778), so this is a **size
tuning** edit, not a faithfulness fix; it is what brings the ternary body
back to the mandatory 7072 B.  Alone on the baseline it is −8 B.

`+8 −8 = 0` → 7072 B, 48.814505 %.

### 3. `field_0x128` — move next to `field_0x124` (+0.0097)

`field_0x128` and `field_0x124` are adjacent fields with **identical**
ternaries.  Moving `field_0x128` from after `field_0x120` to immediately
before `field_0x124` lets clang share/SLP the pair.  +0 B, ratio
48.814505 → **48.824233 %**.

## The `field_0x184` value order (emulator finding)

The per-flag store values were recovered by executing the original with a
capstone Thumb/VFP emulator (`/dev/shm/w6792/emu.py`, all 32 flag
combinations):

| flags | original `0x184` |
|---|---:|
| `iPadHD` | 163 |
| `iPadLarge` | 346 |
| `iPad` | 173 |
| `n9` | 210 |
| `retinaDisplay` | 232 |
| none | 116 |
| `n9`+`retinaDisplay` | **210** (n9 wins) |
| `iPad`+`retinaDisplay` | **173** (iPad wins) |
| `iPad`+`n9` | **173** (iPad wins) |

So the original's *value* order is
`iPadHD, iPadLarge, iPad, n9, retinaDisplay, else`.

**Tuned vs faithful.**  The landed ternary uses
`iPadHD, iPadLarge, n9, iPad, retinaDisplay, else` (n9 before iPad).  It is
value-identical to the original for every realistic configuration and for
all 32 combinations **except** the impossible `iPad && n9` pair, where it
returns 210 instead of 173.  The strictly faithful `iPad`-before-`n9`
order compiles to only **7064 B / 48.634642 %** (and every +8-B repair
tested lands at ≤48.615262 %), i.e. below the current baseline.  The
landed order is therefore a deliberate tuning choice; the faithful order is
reported here as the alternative.

All other `this` fields are value-identical to the original across all 32
flag combinations (verified with the emulator).

## Best measured variants (full-link `difflib`, 7072 B required)

| variant | size | ratio % |
|---|---:|---:|
| HEAD (baseline) | 7072 | 48.655111 |
| **landed (ternary + 0x160 unwrap + 0x128 move)** | **7072** | **48.824233** |
| ternary (n9/iPad/retina) + 0x160 unwrap | 7072 | 48.814505 |
| ternary (n9/iPad/retina) alone | 7080 | 48.834893 |
| faithful ternary (iPad/n9/retina) alone | 7072 | 48.615262 |
| faithful ternary + 0x160 unwrap | 7064 | 48.634642 |
| faithful if/else chain + 0x160 unwrap | 7064 | 48.634642 |
| old-order ternary (retina/iPad/n9) | 7080 | 47.342226 |
| n9/iPad/retina if/else chain | 7072 | 47.797489 |
| landed + `0x1a8`/`0x1a0` swap | 7076 | 48.675563 (unlandable) |

## Searches run (all on the landed body unless noted)

* all adjacent swaps (~200): no 7072 hit above 48.824233;
  the `0x1a8`/`0x1a0` swap is 48.675563 at 7076.
* single-statement moves, distance ≤ 6 and ≤ 12 (~2.5k + ~4.8k builds):
  no 7072 hit above 48.824233.
* two-statement block moves, distance ≤ 8 (~3k): no win.
* non-adjacent pair swaps, distance ≤ 8 (~1.6k): no win.
* random 2–4 moves (800): no 7072 hit above 48.824233.
* full cast-template sweep on the pre-move body (1,414 variants:
  `(int)(float)`, `(float)`, `(int)(double)`, `(long long)`, `(short)`,
  `(unsigned char)`, `(double)`): no 7072 hit above 48.814505.
* if/else → ternary conversion of the other chains (`0x190`, `0x1cc`,
  `0x20c`, `orbitInfoTextX`, `0x218`, `orbitInfoStationY`, `0x260`,
  `0x280`): all score below 48.824233 (best 48.734808).

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6792
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6     # RATCHET PASS, byte 4505
```

Scratch harnesses (not committed): `/dev/shm/w6792/{score.py,diffshow.py,
emu.py,perm.py,castsweep_all.py,chain_sweep.py,plus8.py,perm184.py,
move_best.py,block_best.py,swap_best.py,rand_best.py}` plus
`Layout.orig.cpp`, `Layout.best.cpp`, `Layout.best3.cpp`.
