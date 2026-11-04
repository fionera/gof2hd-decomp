# Layout::Layout C1/C2 — mid-block `vstr` intphi + 0x180/0x188 order (w5606, wave 108)

Worker **w5606**, branch `wave108-w5606`, base `8f98ec4d6` (wave-107 master).
Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev`, ELF VMA `0xd1028`, original size
**7072 B**. Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
Source `src/game/ui/Layout.cpp`.

## Result

**Landed a real, size-exact fuzzy improvement.**

| metric | before | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** |
| `_ZN6LayoutC1Ev` unrounded `difflib` ratio | 48.000000000000 % | **48.049363057325 %** |
| `_ZN6LayoutC2Ev` | 48.000000000000 % | **48.049363057325 %** |
| normalised insns (orig/ours) | 2529 / 2496 | 2529 / **2495** |
| `byte_exact` / `linked_exact` | 4500 / 4500 | **4500 / 4500** |
| `size_exact` / `data_exact` | 4524 / 843 | **4524 / 843** |
| imports / lint / drift | 207 / CLEAN / 0 | **207 / CLEAN / 0** |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`.
No section change: still 27 sections, `.ARM.extab` size- and base-exact. The only
differing section remains the pre-existing wave-107 `.fini_array` (8 → 200 B).
`.text` size and every following symbol address are unchanged (hence
`byte_exact` stays 4500).

## The two edits (ctor only)

1. `field_0xa8` is stored through an **int local + float round trip**:

```cpp
int v = Globals::iPadHD ? 61.0f * 1.40625f : (Globals::iPadLarge ? 122.0f : (Globals::n9 ? 61.0f : (Globals::iPad ? 61.0f : (Globals::retinaDisplay ? 82.0f : 41.0f))));
this->field_0xa8 = (int)(float)v;
```

   (the float ternary is the same value set as before; `int v` truncates it, then
   `(int)(float)v` reproduces the original's `vmov s0,rN; vcvt.f32.s32;
   vcvt.s32.f32; vstr s0,[r0,#0xa8]` intphi fingerprint).

2. `field_0x188` is emitted **before** `field_0x180` (the two independent
   assignments are swapped; both are straight-line, no semantic effect).

## Why this shape (proven from the Thumb)

Original non-HD block (`0xd199a..0xd19de`):

```
d199a: str.w r1,[r0,#152]   ; 0x98
d199e: vcvt.s32.f32 s0,s0
d19a2: ldr r1,[pc,#984]     ; &Globals::iPadLarge
d19a6: ldr r5,[r1]
d19a8: vstr s0,[r0,#156]    ; 0x9c  (floatphi)
d19ac: ldrb.w r9,[r5]
...
d19ca: vmov s0,r2
d19ce: str.w r2,[r0,#172]   ; 0xac
d19d6: vcvt.f32.s32 s0,s0
d19da: vcvt.s32.f32 s0,s0
d19de: vstr s0,[r0,#168]    ; 0xa8  (intphi)
```

So the original's `0xa8` is an int→float→int round trip, not a plain int
store. The landed `int v = <float ternary>; field = (int)(float)v;` is the
local form that keeps the round trip in this flat body without hoisting.

## Measured table (real `link_so` builds, unrounded linked score)

| variant | size | % |
|---|---:|---:|
| baseline (wave-107 body) | 7072 | 48.000000000000 |
| `0xa8` intphi only | 7080 | 48.020688283270 |
| `0x188` before `0x180` only | 7064 | 48.028673835125 |
| **`0xa8` intphi + `0x188`/`0x180` swap (landed)** | **7072** | **48.049363057325** |
| `0xa8` floatphi only | 7064 | 48.028673835125 |
| `0xa8` intphi with the `0xac` expression in a separate local | 7096 | 47.510951812027 |
| shared `int v` for `0xac` and `0xa8` (faithful REA shape) | 7116 | 18.270544602772 |
| shared `int v`, `0xa8` adjacent to `0xac` | 7116 | 17.876002346959 |
| shared `int v`, `0x98` moved before `0xac` | 7092 | 18.121330724070 |
| `0x160` intphi on the swap body | 7072 | 48.009554140127 |
| `0x1a8` intphi on the swap body | 7072 | 48.009554140127 |

Exhaustive checks on the landed body (all real builds):

* all 206 adjacent unit swaps: no 7072 B variant above 48.0494 (99 are 7072 B,
  all exactly 48.0494 or lower);
* intphi / floatphi on every one of the 204 field statements: none above
  48.0494 at 7072 B;
* 400 random mutations (arbitrary pair swap / unit move / intphi / floatphi):
  none above 48.0494 at 7072 B.

The wave-89 all-ternary body (iOS source order) still scores **50.7885 % at
6800 B**; it is not landable because the ctor must stay 7072 B.

## Proven vs hypothesis

**Proven (binary / measured):**

* The original `0xa8` store is the intphi round trip (`vmov`/two `vcvt`/`vstr`).
* The landed body is size-exact 7072 B, both C1/C2 score 48.049363057325 %,
  and the full gate is `RATCHET PASS` with `byte_exact = linked_exact = 4500`.
* No section/`.ARM.extab` change is introduced; only the pre-existing
  `.fini_array` delta remains.

**Hypothesis (not proven):**

* The original source likely shared one `int` between `0xac` and `0xa8`
  (REA `uVar13`), but that form hoists in this flat body and collapses the
  fuzzy score to ~18 %. The landed form is a metric-compatible local variant.
* The `0x188`/`0x180` order is a metric tuning, not the iOS source order
  (iOS has `0x180`, `0x184`, `0x188`).

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w5606
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6     # RATCHET PASS
python3 /dev/shm/w5606/score1u.py _ZN6LayoutC1Ev       # 7072 B, 48.049363057325
```

The scratch scorer is the `tools/verify/verify.py` `difflib(autojunk=False)`
normalisation; the landed numbers above are the unrounded linked ratios.
