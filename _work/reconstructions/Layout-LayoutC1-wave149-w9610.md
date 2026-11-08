# `Layout::Layout` C1/C2 — wave 149 / worker w9610
## 4-store `0x184` chain landed at 7,072 B: raw 5,768 -> 5,741, aligned 2,198 -> 2,189.

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.  Baseline (wave-149
master `7eed2966e`, w9504's landed variant): raw same-address diff **5,768**,
aligned instruction diff **2,198**, size 7,072, **3** `#388` stores
(1 `str.w` + 2 `strd`).  Original has **4** `str.w [r0,#388]`.

## 1. The store-shape problem

w9504's landed variant emits

```
str.w r1,[r0,#388]        ; 346 (iPadLarge arm)
strd  r2,r1,[r0,#388]     ; 0x184+0x188 paired (n9 arm)
strd  r2,r1,[r0,#388]     ; 0x184+0x188 paired (retina arm)
```

i.e. the `0x184` value is folded into two `strd` pairs with `0x188`.  The
original keeps `0x184` and `0x188` in separate stores in every arm:

```
d1ea2  str.w r9,[r0,#388]   ; 346
d1fae  str.w r2,[r0,#388]   ; 173
d1fc2  str.w r1,[r0,#388]   ; retina ? 232 : 116
d1fd6  str.w r1,[r0,#388]   ; 210
```

Candidate A from the w9504 note (dup-`iPad` expression + swap1, i.e. w9504's
landed source with swap2 **reverted**) restores the 4-store shape but measures
raw 5,793 / aligned 2,223, i.e. worse than the landed 3-store variant.

## 2. New hypothesis: keep the dup expression, re-hill-climb the statement order

The w9504 note treated the two swaps as a fixed size anchor.  Instead, start
from candidate A (4 `str.w`, exact 7,072 B) and run a fresh local search over
**adjacent single-line statement swaps** with the hard constraints
`size == 7072 && #388 stores == 4`, minimising `raw + aligned`.

The search converges (same result for every score weighting tried:
raw+align, raw only, align only, raw+3*align, 3*raw+align) to a 6-swap chain:

| # | swap (source order) |
|---|---|
| 1 | `field_0x54` <-> `field_0x58` |
| 2 | `ringNearMidThreshold` <-> `field_0x2b0` |
| 3 | `field_0x11c` <-> `field_0x110_strip58` |
| 4 | `field_0x1a4` <-> `field_0x188` (un-does w9504 swap2) |
| 5 | `field_0x1f0` <-> `field_0x144` |
| 6 | `field_0x310` <-> `field_0x30c` |

All are pure statement reorderings; every expression is unchanged from the
semantically-verified dup-`iPad` form

```cpp
this->field_0x184 = Globals::iPadHD ? 163 : (Globals::iPadLarge ? 346 :
    (Globals::n9 ? (Globals::iPad ? 173 : 210)
                 : (Globals::iPad ? 173 : (Globals::retinaDisplay ? 232 : 116))));
```

## 3. Result

| variant | size | raw | aligned | `#388` stores |
|---|---:|---:|---:|---:|
| master (w9504, 3-store) | **7,072** | **5,768** | **2,198** | 3 (1 `str.w` + 2 `strd`) |
| candidate A (dup + swap1) | 7,072 | 5,793 | 2,223 | **4 `str.w`** |
| **landed (dup + 6 swaps)** | **7,072** | **5,741** | **2,189** | **4 `str.w`** |

The landed variant beats the 3-store master on **both** metrics while keeping
the original's 4 `str.w [r0,#388]` chain.

Fast-search proxy: raw/aligned measured on the per-TU `.o`
(`objcopy --only-section=.text._ZN6LayoutC2Ev`) are ~8/~13 lower than the
linked `.so`; the final numbers above are from the real `link_so` +
`/dev/shm/w9610/fdiff.py` + `_work/reconstructions/align_diff.py`.

## 4. Rejected / tested

* 3-iPad-test expressions (`dup3a..d`, `s7..s10`): 7,064-7,088 B, 3 `str.w`
  (clang folds the extra test back into a select).
* `n9`-first / retina-first variants, split assignments
  (`s1..s5`): 6,968-7,064 B, 1-2 stores.
* `dup_retina` (`retina ? 232 : (iPad ? 173 : 116)`): 7,072 B, raw 5,754,
  but only **3** stores.
* `dup_n9out` (`n9 ? 210 : (iPad ? 173 : ...)`): 7,064 B, 4 stores, raw 5,785
  (8 B short; no size anchor found that keeps 4 stores).

## 5. Gate

`cmake --build cmake-build-match --target check`:
byte_exact **4507/4524**, linked_exact **4507/4524**, size 4524/4524, data
**843/843**, imports **207**, lint **CLEAN**, drift 0, `RATCHET PASS`.
Function size stays **7,072 B**, so no later function shifts and no section
size changes.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w9610
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9610/fdiff.py _ZN6LayoutC1Ev        # 7072 / 5741
python3 _work/reconstructions/align_diff.py _ZN6LayoutC1Ev   # 2189
cmake --build cmake-build-match --target check -j6
```

Scratch `/dev/shm/w9610/`: `fdiff.py`, `align_diff.py`, `cc.sh` (single-TU
compile), `fast.py` (size/store/raw/aligned on the `.o`), `climb3.py`
(parallel adjacent-swap hill climb), `candA.cpp`, `climb_best.cpp`.

## 7. Open questions

* The 4 stores still do not have the original's exact values: ours are
  `346, select(210,173), 173, 232/116`; the original is
  `346, 173, 232/116, 210`.  The original's `iPadLarge`(346) arm lives in the
  device-major partition and the `iPad`(173) arm in the shared tail, so clang
  never merges them; our flat chain always merges `iPadLarge`+`iPad` into one
  select unless the `iPad` test is duplicated.
* The shared tail is still ~0x150 B later than the original (our first `#388`
  store is at `0xfd8` vs the original `0xe7a`); that shift is the bulk of the
  remaining raw diff.
