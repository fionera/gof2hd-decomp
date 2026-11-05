# Layout::Layout C1/C2 (`_ZN6LayoutC1Ev`) — wave 119 / worker w6754

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (identical bodies), original size
**7072 B**, source `src/game/ui/Layout.cpp`. Branch `wave119-w6754`, base
`c2f379cb8` (wave-119 harvest). Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.

## Result — landed a real fuzzy improvement at the mandatory 7072-B size

| metric | before (HEAD) | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** (unchanged) |
| unrounded `difflib` ratio | 48.594778 % | **48.624950 %** |
| `byte_exact` / `linked_exact` | 4505 / 4505 | **4505 / 4505** |
| `size_exact` / `data_exact` | 4524 / 843 | **4524 / 843** |
| imports / lint / drift | 207 / CLEAN / 0 | **207 / CLEAN / 0** |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`.
The 27 section headers, file size 2,187,328 B, `.text` and `.ARM.exidx` are
unchanged (the function keeps its 7072-B symbol size).  `.ARM.extab` remains
the wave-118 state (ours `0x26290`, original `0x26298`, 8 B short — pre-existing).

## The landed source shape (three edits)

```cpp
// 1. field_0x9c: float-typed ternary (original materialises 100.0f/50.0f)
- this->field_0x9c = (int)(float)(Globals::iPadHD ? 70 : (Globals::n9 ? 75 : (Globals::retinaDisplay ? 100 : 50)));
+ this->field_0x9c = (int)(Globals::iPadHD ? 70.0f : (Globals::n9 ? 75.0f : (Globals::retinaDisplay ? 100.0f : 50.0f)));

// 2. field_0x2c8: fold the HD arm to int (original has ONE round trip, not two)
- int t_field_0x2c8 = Globals::iPadHD ? 45 * 1.40625f : (...);
+ int t_field_0x2c8 = Globals::iPadHD ? (int)(45 * 1.40625f) : (...);

// 3. store order: 0x1a0 before 0x1a8 (matches the original)
- this->field_0x1a8 = ...;
- this->field_0x1a0 = ...;
+ this->field_0x1a0 = ...;
+ this->field_0x1a8 = ...;
```

Edits 1+2 alone give a **7076-B** body scoring **48.685259 %** (4 B over the
hard wall; a 7076-B ctor collapses `byte_exact`).  The `0x1a0`/`0x1a8` swap
removes exactly 4 B and lands the 7072-B body at **48.624950 %**.

The swap is faithful, not a tuning trick: the original stores `0x1a0`
(`d1ec0`, `d1ef6`, `d205e`) before `0x1a8` (`d209a`); HEAD had `0x1a8`
(`d212c`, `d2174`) before `0x1a0` (`d2198`).

## Measurements behind the win (all real `link_so` builds, unrounded)

| variant | size | unrounded % |
|---|---:|---:|
| HEAD baseline | 7072 | 48.594778 |
| `0x9c` float ternary alone | 7084 | 48.665870 |
| `0x2c8` HD arm `(int)` alone | 7064 | 48.614158 |
| `0x9c` float + `0x2c8` HD arm (`u9c2`-shape) | 7076 | **48.685259** |
| **+ `0x1a0`/`0x1a8` swap (LANDED)** | **7072** | **48.624950** |
| `0x9c` float + `0x2c8` HD arm + `0x2c_rowHeight` wrap | 7072 | 48.106815 |
| `0x9c` float + `0x2c8` HD arm + `eventBannerScale` wrap | 7072 | 48.036675 |
| `0x9c` float + `0x2c8` HD arm + `0x68` `(int)(float)(int)` | 7072 | 27.905123 |
| `0x9c` float + `0x2c8` HD arm + `0x2d0` cast removal | 7072 | 48.594778 (neutral) |

The 7076-B body was searched for a −4 B edit:

* all 238 adjacent statement swaps: 15 land on 7072 B; best is the
  `0x1a0`/`0x1a8` swap at **48.624950** (next: `0x128`/`0x238_blk16.b`
  48.535565, `0x28c`/`0x286` 48.465524, `0x1a8`/`0x1a0` is the winner).
* all 190 single-line `this->X = ...;` wrapped `(int)(float)`: only
  `0x2c_rowHeight` and `eventBannerScale` land on 7072 (both below baseline).
* the same wrapped `(int)(float)(int)`: `0xac` 7072/48.408, `0x27c`
  7072/43.404.
* the same wrapped `(float)`: only `0x2c_rowHeight` (below baseline).
* removing each existing `(int)`/`(int)(float)` cast layer: `0x2d0` is the
  only 7072 hit and is exactly score-neutral (48.594778).

## Other findings this wave (measured, not landed)

### `field_0x3f8` store kind is wrong, but the faithful form is too big

Original writes `0x3f8` with `vstr s0` after an int→float→int round trip
(`d2514`–`d252c`); HEAD writes `str.w r4` (`d26aa`).  Spelling the
`rewardBoxY2` else arm as `(int)(float)((rewardBoxY + 17) << retinaDisplay)`
adds +16 B and drops the score:

| variant | size | % |
|---|---:|---:|
| `rewardBoxY2` else arm `(int)(float)` | 7088 | 47.490040 |
| `0x9c` float + `0x2c8` HD arm + `rewardBoxY2` round trip | 7080 | 47.508968 |

### `0x158`/`0x15c`/`0x170` round trips cannot be forced from source

The original has `vcvt.f32.s32`/`vcvt.s32.f32` pairs at `0x158`, `0x15c`,
`0x170` that HEAD lacks.  Spelling them as `(int)(float)(...)`, an `int`
local + `(int)(float)local`, an `if/else` local, a `float` local, a `double`
round trip or `(int)(float)(x + 0)` is **size- and score-neutral** — clang
7.0.2 folds the conversion on this body.  Separate `if` statements (not
`else if`) do emit the round trip but shrink the body to 6952 B / 30.6 %.

### Float-conversion census

Original **37** `vcvt`, HEAD **48**.  HEAD-only extra round trips at
`0x20`, `0xa8` (2 vs 1), `0x160` (2 vs 1), `0x3e8`, `0x148`, `0x204`,
`0x1f4`; original-only at `0x158`, `0x15c`, `0x170`, `0x3f8`.  Fixing them
one at a time lands on 7060/7064/7072 and costs score (see wave-118 note).

### Literal-pool hypothesis (wave-119 w6720, re-confirmed)

The 46 missing pool words are 8 extra SLP `vld1.64` four-int groups (+32),
one extra `100.0f` and one extra `50.0f` (+2), one fewer GOT offset (−1),
plus 52 B more cold outlined code.  The `0x9c` float ternary reproduces the
`100.0f`/`50.0f` pair; the remaining SLP groups need the original's
address-ordered four-field statement runs, which shrink the body below 7072.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6754
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6     # RATCHET PASS, byte 4505
cmake --build cmake-build-match --target verify-fn FN=_ZN6LayoutC1Ev
```

Scratch harnesses (not committed, `/dev/shm/w6754/`): `score.py` (unrounded
ratio), `try.py` (apply replacements → `link_so` → score), `diff.py`,
`diffsum.py`, `diffaddr.py`, `stores.py`, `storekind.py`, `vcvt.py`,
`vcvt2.py`, `scan8.py`, `scan8b.py`, `scan_nt.py`, `swapscan.py`, `move2.py`,
`movescan.py`, `scoremoves.py`, `scorehits.py`, `fuzz.py`, `ne0.py`,
`Layout.base.cpp`, `orig.dis`, `ours.dis`.
