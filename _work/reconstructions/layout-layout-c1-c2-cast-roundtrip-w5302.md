# Layout::Layout C1/C2 — 3-arm `rewardBoxY2` + float-round-trip casts (w5302 pass 12)

Worker **w5302**, branch `wave105-w5302`, base `835748c18` (on top of the
wave-105 research note `42c415c6b`). Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev`
at ELF VMA `0xd1028`, original size **7072 B**.

## Result

Landed a **size-neutral fuzzy improvement** on the constructor only:

| metric | before | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` `usize` | 7072 | **7072** |
| raw verifier ratio (`autojunk=False`) | 47.77247414478918 | **47.84860557768924** |
| `byte_exact` | 4500 / 4524 | 4500 / 4524 |
| `linked_exact` | 4500 / 4524 | 4500 / 4524 |
| `size_exact` | 4524 / 4524 | 4524 / 4524 |
| `data_exact` | 843 / 843 | 843 / 843 |
| imports | 207 | 207 |
| lint / drift | CLEAN / 0 | CLEAN / 0 |

`RATCHET PASS`.

## The change

1. `rewardBoxY2` restored to the **true 3-arm** source proved in
   `layout-layout-c1-c2-branchtree-w5302.md` §1 (the 5-arm body's
   `iPadLarge`/`retina` arms are redundant — the original computes
   `(rewardBoxY + 17) << retinaDisplay` for every non-HD/non-n9 device).
   The 3-arm body alone is **7024 B / 47.9633 %**.
2. Three `(int)(float)` round-trip casts re-add the missing 48 B:
   - `field_0x150 = (int)(float)(double)(<ternary>)` (+16 B)
   - `field_0x194 = (int)(float)(<ternary>)` (+16 B)
   - `field_0x1a4 = (int)(float)((int)(float)(<ternary>))` (+16 B)
   Net: 7024 + 48 = **7072 B**, and the extra `vcvt` code matches the
   original's mid-block `vmov/vcvt.f32.s32/vcvt.s32.f32/vstr` idiom better
   than the flat int ternaries did.

All six device configurations (`iPadHD`, `n9`, `iPadLarge`, `retina`,
`iPad`, `default`) were re-emulated against the original with the Thumb
emulator; the whole `this` image is bit-identical (0 mismatches), so the
casts are value-preserving.

## Why this works (and what does not)

`w4401` §4 showed that `fptosi(sitofp(select))` folds to the int `select`
when the value is a pure constant select, which is why wrapping
`0x158/0x15c/0x170` in `(int)(float)` alone adds **0 B**. The three fields
chosen above are the ones where the cast survives `-Oz` and costs a real
16 B conversion sequence. A scan of `(int)(float)` on every single-line
ternary of the 7024 B body found exactly five +16 B fields
(`0x2d8, 0x188, 0x150, 0x194, 0x1a4`) and two +8 B fields
(`marqueeWidth, 0xa4`); the `0x150+0x194+0x1a4` combination is the only
one whose 7064 B intermediate scores above the old 47.7725, and the
double cast on one field supplies the last 8 B.

Measured alternatives at 7072 B (all worse than 47.8486):
`150+194+1a4+2d8` 47.7689, `150+194+1a4+marqueeWidth` 47.6703,
`(int)(float)` on `0x158/0x15c/0x170` (all-double form) 47.6608.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6   # RATCHET PASS
python3 /dev/shm/w5302/fscore.py                     # 47.84860557768924, 7072 B
python3 /dev/shm/w5302/verify_sem.py                 # 0 mismatches (6 configs)
```
