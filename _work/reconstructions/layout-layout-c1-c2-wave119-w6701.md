# Layout::Layout C1/C2 (`_ZN6LayoutC1Ev`) — wave 119 / worker w6701

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (identical bodies), original size
**7072 B**, source `src/game/ui/Layout.cpp`. Branch `wave119-w6701`, base
`47a1396bd` (wave-118 harvest).

## Result — one landable improvement

| metric | before (HEAD) | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** (unchanged) |
| unrounded `difflib` ratio | 48.634642 % | **48.664807 %** |
| instruction count (ours) | 2488 | 2489 |
| `byte_exact` / `linked_exact` | 4503 / 4503 | **4503 / 4503** |
| `size_exact` / `data_exact` | 4524 / 843 | **4524 / 843** |
| imports / lint / drift | 207 / CLEAN / 0 | **207 / CLEAN / 0** |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`.
No section changed: 27 section headers, file size 2,187,328 B.  `.text` and
`.ARM.exidx` are unchanged; `.ARM.extab` remains 0x26290 (8 B short, pre-existing).

The report metric rounds to 48.6 % before and after; the win is in the
unrounded ratio (+0.030165 pp).

## Landed edits (three, all real source shape)

### 1. `field_0x2c8` HD arm is an int conversion, not a float literal

```cpp
// before
int t_field_0x2c8 = Globals::iPadHD ? 45 * 1.40625f : (Globals::n9 ? 45 : ...);
// after
int t_field_0x2c8 = Globals::iPadHD ? (int)(45 * 1.40625f) : (Globals::n9 ? 45 : ...);
```

Original HD path (REA `rea.c` line 1040 and Thumb):
`*(undefined8 *)(this + 0x2c8) = 0x7e0000003f` → `0x2c8 = 0x3f = 63`, i.e.
`(int)(45 * 1.40625f) = (int)63.28125 = 63`, materialised with `vcvt.s32.f32`.
The float-literal spelling let clang constant-fold the value without the
`vcvt` round trip.  Alone this edit gives 7064 B / 48.654038 %.

### 2. `field_0x9c` uses float arms (the original stores it via `vcvt`/`vstr`)

```cpp
// before
this->field_0x9c = (int)(float)(Globals::iPadHD ? 70 : (Globals::n9 ? 75 : (Globals::retinaDisplay ? 100 : 50)));
// after
this->field_0x9c = (int)(float)(Globals::iPadHD ? 70.0f : (Globals::n9 ? 75.0f : (Globals::retinaDisplay ? 100.0f : 50.0f)));
```

The original stores `0x9c` with `vcvt.s32.f32` + `vstr` (wave-110 note), i.e.
the value is a float that is rounded through `(int)(float)`.  Alone this edit
gives 7084 B / 48.705695 %.

Edits 1+2 together give **7076 B / 48.725100 %** — the best-scoring body seen
this wave, but 4 B over the mandatory 7072-B wall.

### 3. `field_0x1a0` before `field_0x1a8` (address order, -4 B)

```cpp
// before (lines 188/189)
this->field_0x1a8 = ...;
this->field_0x1a0 = ...;
// after
this->field_0x1a0 = ...;
this->field_0x1a8 = ...;
```

The original HD SLP group is `0x1a0,0x1a4,0x1a8,0x1ac` (address order).  This
single adjacent-statement swap removes exactly 4 B from the 7076-B body and is
the only -4 B edit found that keeps the score above the baseline.  It is the
difference between the unlandable 7076 body and the landed 7072 body.

## Best measured variants (all real `link_so` builds, unrounded ratio)

| variant | size | score % |
|---|---:|---:|
| HEAD (baseline) | 7072 | 48.634642 |
| **edits 1+2+3 (landed)** | **7072** | **48.664807** |
| edits 1+2 | 7076 | 48.725100 |
| edit 2 only | 7084 | 48.705695 |
| edit 1 only | 7064 | 48.654038 |
| edit 4 only (`0x160` int HD arm) | 7044 | 48.683160 |
| edits 1+2+4 (`0x160` int arm) | 7044 | 48.633553 |
| edits 1+2+3+4 (all four faithful-arm edits) | 7004 | 48.940424 |
| all-ternary body (`30f1280d`, no pad) | 6800 | 50.869389 |

The 7072-B wall is mandatory: 7076 B collapses `byte_exact` 4503 → 2454,
7068 → 4458, 7064 → 3221.

## Negative searches performed this wave (all real builds)

* all 16 subsets of the four "faithful int/float arm" edits on HEAD: no 7072
  hit above baseline (sizes 6992..7084);
* all 286 pairs/triples of the twelve positive-size `(int)(float)` wraps on the
  7004-B body: the only 7072 hit is `0x2c4`+`0x10c` at 20.476472 %;
* all 323 combinations of a -4 B wrap with `0x2c4`(+56) + two +8 B wraps and
  with `0x1fc`(+32) + five +8 B wraps on the 7004-B body: sizes 7028..7092,
  **no 7072**;
* all 200 adjacent-statement swaps on the 7068-B body: one 7072 hit
  (`0x2f4`/`0x2f8`, 48.131955 %);
* all 200 adjacent swaps + every `(int)(float)` removal on the 7084-B body:
  three 7072 hits, best 48.524721 %;
* all 200 adjacent swaps on the 7076-B body (edits 1+2): fourteen 7072 hits,
  best **48.664807 %** (landed);
* `||` → `|` on `0x2d8` and on `*rd`, `volatile` removal, `float v138`:
  neutral or worse;
* reordering the missing SLP groups (`0xb0`, `0xc0`, `0xec`, `0x108`, `0x118`)
  into address order on the 7004-B body: sizes 6856..7020, scores 41.9..48.8 %.

## Structural findings (for the next wave)

* The original body is **6106 B code + 966 B pool (241 words)**; HEAD is
  6290 B code + 782 B pool (195 words).  The 46 missing pool words are SLP
  int-constant groups (`vld1.64 {dX-dY}` 4×int + `vst1.32`), not GOT entries.
* Original HD-block SLP group order:
  `0x14,0x4,0x24,0x34,0x44,0x54,0x64,0x2c8,0x7c,0xc0,0xb0,0xec,0x294,0x2a4,
  0x108,0x118,0x128,0x140,0x150,0x160,0x170,0x180,0x190,0x1a0,0x1b0,0x1c0,0x1d0`.
  HEAD reproduces the first nine groups exactly, then emits
  `0xb4,0xc4,0xdc,0x108,0x128,0x148,...` — it lacks `0xc0/0xb0/0xec/0x118`
  and its `0x140..` groups are offset by +4 because `field_0x140` is assigned
  late (line 307).
* REA (`rea.c`) confirms the original source is a tail-merged branch tree:
  `if (Globals::iPadHD == 0) { if (Globals::n9 == 0) {...} else {...} ... }
  else { ... HD constant stores ... }` followed by a common tail at
  `0xe21e2`.  The HD arm is ~550 B of constant stores; the non-HD arm carries
  the `n9`/`retinaDisplay`/`iPad` logic.  The flat-ternary reconstruction
  merges the arms, which is why ours is 184 B longer in code and 184 B shorter
  in pool.
* The 272-B gap to the all-ternary body's 6800 B is **not** a missing store
  (all 32 flag combinations store identical bytes); it is 100 extra 16-bit
  instructions (branches/`it` blocks) + 14 extra 32-bit pair/vector stores in
  the original, i.e. less if-conversion.  Reconstructing the branch tree (not
  hill-climbing the flat order) is the remaining work.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6701
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6     # RATCHET PASS
```

Scratch harnesses (not committed): `/tmp/w6701/{score.py,try.sh,combo.py,
combo2.py,adjswap.py,subsets.py,e1plus.py,e2search.py,e12swap.py,reorder.py,
move.py,final.py}`, `/tmp/w6701/rea.c`.
