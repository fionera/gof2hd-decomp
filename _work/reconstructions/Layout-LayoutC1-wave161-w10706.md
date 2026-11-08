# `Layout::Layout` C1/C2 — wave 161 / worker w10706
## Store-table audit + 6-statement block permutation.  The `field_0x160`
## before `field_0x158` move is 48 B short because it *folds* 0x160's
## int→float→int roundtrip into direct int stores.  A size-exact permutation
## of the surrounding 6-statement block lands raw **5,733 -> 5,719**, aligned
## **1,978 -> 2,013**, size **7,072 B** exact.

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original
vaddr `0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Baseline = wave-160 HEAD `01c7f92d6`: raw same-address diff **5,733**,
aligned instruction diff **1,978**, size 7,072.
Exit: raw **5,719**, aligned **2,013**, size **7,072**.
Gate on exit: byte_exact 4507/4524, linked_exact 4507/4524, size
4524/4524, data 843/843, imports 207, lint CLEAN, `RATCHET PASS`.
`.text` diff **85,518 -> 85,504** (-14).  `.rel.plt` still byte-identical
(0), `.ARM.extab` 1,258 unchanged, every section size exact.

## 1. The -48 B companion: what the move actually removes

The original stores 0x158, 0x15c and 0x160 with a **two-conversion
int→float→int roundtrip** (`vmov sN,rM; vcvt.f32.s32; vcvt.s32.f32;
vstr`):

```
d1db0/d1db4 -> d1db8 vstr [r0,#344]   ; 0x158
d1dde/d1de2 -> d1de6 vstr [r0,#348]   ; 0x15c
d1e0a/d1e0e -> d1e12 vstr [r0,#352]   ; 0x160
```

Our baseline (wave-160) 0x160 block at `d1ea8..d1eea` is a **three-conversion**
block: the source is `(int)(float)(int)(float ternary)`, so clang emits
`vcvt.s32.f32` (float->int), `vcvt.f32.s32` (int->float), `vcvt.s32.f32`
(float->int) plus the float-ternary `vmov.f32 #13.0/#26.0`/`vldr` — 66 B.

Moving the 0x160 statement immediately before 0x158 lets clang merge the
0x160 ternary with the 0x158/0x15c branch values and **fold it to direct
integer `str.w`** at `d1e62` / `d1e84`.  That removes the whole float block:
**7,072 -> 7,024 B = -48 B exactly**.  Raw drops 5,733 -> 5,697, aligned
1,978 -> 1,957.  The move is gate-fatal only because of the 48 B size loss.

## 2. Store table (0x140..0x1e8), original vs ours

Original general-path store order (offset, form):

```
0x14c str.w, 0x150 vstr, 0x154+0x158 strd, 0x15c str.w,
0x158 vstr, 0x15c vstr, 0x160 vstr, 0x164 str.w (x3 branches),
0x168 vstr, 0x17c str.w, 0x180 str.w, 0x184 str.w, 0x188 str.w,
0x18c strd, 0x1a0 str.w, 0x16c str.w, 0x190 strd, 0x198 strd,
0x1a0 str.w, 0x16c str.w (x2), 0x170 vstr, 0x174 str.w,
0x178 strd (x3), 0x180/0x184/0x188 str.w, 0x18c strd (x3),
0x194 str.w, 0x198 strd, 0x1a0 str.w, 0x1a4 str.w (x2),
0x1a8 vstr, 0x1ac str.w, 0x1e0 vstr, 0x198 strd, 0x1ac str.w (x2),
0x1e0 str.w, 0x1e8 str.w, 0x1e4 vstr
```

The original's iPadHD constant block (`d11f2..d123e`) is 10 `vst1.32`
groups at `0x140,0x150,0x160,...,0x1d0`; ours reproduces the same run
starting at `0x128` and ending at `0x1d0` (the `0x140`/`0x1c0` groups are
emitted elsewhere).  This is the known device-major residual, not the
-48 B lever.

Grouping census (original vs ours): `vst1.32` 81 vs 70, `vld1.64` 86 vs
77, `strd` 62 vs 49 — the original pairs/vectors ~11 more stores, which is
why its raw store-instruction count is 214 vs our 275.

## 3. Exhaustive 6-statement block permutation (720)

Block `{0x15c, 0x160, 0x158, 0x204, 0x2bc, 0x164}` (contiguous source
lines 141..146), every permutation compiled with the exact NDK r18b
`-Oz` command and the function size read from the object symbol:

* **83 / 720 permutations give exactly 7,072 B.**
* 51 of those keep 0x160 before 0x158.
* Best raw among the 83: order `0x204, 0x160, 0x158, 0x2bc, 0x15c, 0x164`
  = **raw 5,719, aligned 2,013, size 7,072** (landed).
* Moved-only shape (`0x15c,0x160,0x158,0x204,0x2bc,0x164`):
  5,697 / 1,957 at 7,024.
* Original store-order shape (`0x158,0x15c,0x160,0x164,0x204,0x2bc`):
  raw 5,731, aligned 2,021, size 7,072 (only -2 raw, +43 aligned).

## 4. +48 B candidates that were measured and rejected

| candidate | size | raw | aligned | why not |
|---|---:|---:|---:|---|
| moved + `(int)(float)((int)(float)v)` on 0xa8 | 7,072 | 6,381 | 3,897 | exact +48 B but destroys the diff |
| moved + move `0x2cc` before `0x160` | 7,072 | 5,941 | 2,191 | exact +48 B, worse than baseline |
| moved + `(int)(float)` on 0x158 | 7,044 | 5,750 | 1,961 | +20 only |
| moved + `(int)(float)` on 0x2c4+0x180+0x1a4 | 7,080 | 5,794 | 1,994 | deltas not additive (+56) |
| moved + `(int)(float)` on 0x1fc | 7,068 | — | — | +44 only |
| moved + temp `t_field_0x160` + `(int)(float)` | 7,024 | 5,697 | 1,957 | compiler still folds |
| moved + 0x160 int literals | 7,024 | 5,697 | 1,958 | compiler still folds |

The genuine roundtrip fields in the original are 0x60, 0x68, 0x6c, 0x70,
0x2c8, 0xa8, 0x158, 0x15c, 0x160, 0x170, 0x280, 0x3f4.  Converting our
float ternaries at 0x60/0x68/0x6c/0x70 to `(int)(float)(int)(...)` is
*not* additive either: 0x60 -32, 0x68 -64, 0x6c +8, 0x70 +? (the compiler
folds the inner cast and removes float literal loads).

## 5. Landed change

Three statement relocations only (no expression/constant/type edits):

```
0x15c, 0x160, 0x158, 0x204, 0x2bc, 0x164      (wave-160 moved base)
->
0x204, 0x160, 0x158, 0x2bc, 0x15c, 0x164      (landed)
```

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10706
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10706/fdiff.py _ZN6LayoutC1Ev        # 7072/7072, diff 5719
python3 /dev/shm/w10706/align_diff.py _ZN6LayoutC1Ev   # 2013
cmake --build cmake-build-match --target check -j6     # RATCHET PASS
```

Scratch `/dev/shm/w10706/`: `fastsize.py` (0.14 s/compile object-size
harness), `perm6.py`/`perm6_res.txt` (720-perm size sweep),
`measure_perms.py`/`perm6_meas.txt` (raw+aligned for all 7,072 perms),
`wrapall.py` (per-field cast size sweep), `moveall.py` (move-to-0x160
size sweep), `expand2.py` (store-table expansion).
