# `Layout::Layout` C1/C2 — wave 160 / worker w10607
## Float-literal-pool audit: four real source-shape inversions found, but no
## size-exact variant improves the diff. Source unchanged; baseline re-measured
## raw **5,739**, aligned **1,978**, 7,072 B.

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original
vaddr `0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Baseline = wave-159 HEAD `08af4633b`: raw same-address diff **5,739**,
aligned instruction diff **1,978**, size 7,072.
Exit: raw **5,739**, aligned **1,978**, size 7,072 (no source change).
Gate on exit: byte_exact 4507/4524, linked_exact 4507/4524, size
4524/4524, data 843/843, imports 207, lint CLEAN, `RATCHET PASS`.

## 1. There are no String members and no calls in this function

`grep -c '\sbl\s'` on the original disassembly = **0**.  The whole 7,072 B
body is constant stores (scalar `str`/`strd`/`stmia`, `vst1.32` groups,
`vstr`).  There is **no** `String` copy-constructor call site and no
`Sprite`/`Image` load to diff.  The coordinator's "String members" hint does
not apply to this TU; the literal pool is ints/floats plus PC-relative GOT
offsets (`0x0013xxxx`), not `char*` addresses.

## 2. Literal-pool diff (word by word)

`vldr sN, [pc, #...]` sites (the only single-word float loads):

```
ORIG: 5.625 @d1254, 57.6 @d18c0, 75.0 @d1964, 37.5 @d1e22, 50.0 @d1e3c,
      37.5 @d1e4e, 32.0 @d2068, 400 @d2686, 200 @d2698, 150 @d26aa
OURS: 57.6 @d18dc, 75.0 @d1992, 122.0 @d19d8, 61.0 @d19e8, 98.0 @d1eaa,
      49.0 @d1eb4, 37.5 @d1efc, 37.5 @d1f0a, 50.0 @d1f26, 32.0 @d2166,
      400 @d2748, 200 @d275a, 150 @d276c
```

So ours carries **four extra pool floats** (122.0, 61.0, 98.0, 49.0) and is
**missing 5.625f**.  `vmov.f32` immediate counts: orig 15, ours 11 — orig has
`6.0`x2, `4.0`x2, `8.0`x2 extra; ours has `13.0`, `26.0` extra.
`vcvt.f32.s32` / `vcvt.s32.f32` counts: orig **15 / 22**, ours **17 / 24**.

## 3. The four inversions (original evidence)

The original does **runtime** float↔int conversions at the merge point of a
float ternary; our source forces the opposite (int ternary + explicit
`(int)(float)` round-trip, or vice versa).  This is a genuine source-shape
difference, confirmed instruction by instruction:

| source line | our shape | original shape (evidence) |
|---|---|---|
| `field_0x150` (line 139) | `(int)(float)(int ternary)` → `vmov;vcvt.f32;vcvt.s32` | `(int)(float ternary)` → **only** `vcvt.s32.f32` at `d1d2a`, then `vstr [r0,#336]` |
| `field_0x160` (line 146) | `(int)(float)(int)(float ternary)` → extra `vcvt.s32` at `d1eda` | `(int)(float)(int ternary)` → `vmov s0,r2; vcvt.f32.s32; vcvt.s32.f32` at `d1e02/0a/0e`, `vstr [r0,#352]` |
| `v` / `field_0xa8` (lines 69-70) | float ternary then `(int)(float)v` | int ternary in `r2`; `vmov s0,r2; str r2,[r0,#172]; vcvt.f32; vcvt.s32; vstr [r0,#168]` at `d19ca..d19de` |
| `field_0x1e4` (line 198) | `int` ternary (`mov.w r8,#5/6/8/4`) | float ternary: `vldr s0,5.625f` @d1254, `vmov.f32 s0,#6.0` @d21a6, `vcvt.s32.f32` @d21e2, `vstr [r0,#484]` @d21fe |

The 5.625f is exactly `4 * 1.40625f`, i.e. the original's iPadHD branch of
`field_0x1e4` is `(int)(4 * 1.40625f)` = 5, not the literal `5`.

## 4. Measured variants (all real `link_so` + fdiff + align_diff)

Let 1 = `v` int, 2 = `field_0x160` int, 3 = `field_0x1e4` float,
4 = `field_0x150` float.  Baseline size 7,072.

```
none  7072  raw 5739  aligned 1978
1     7024  raw 5769  aligned 1982
2     7048  raw 5767  aligned 1972
3     7080  raw 5832  aligned 2527
4     7064  raw 5779  aligned 2063
12    7008  raw 5758  aligned 1973
14    7016  raw 5765  aligned 2048
24    7040  raw 5781  aligned 2049
34    7072  raw 5845  aligned 2551   <- only size-exact variant, both metrics worse
124   7000  raw 5751  aligned 2057
134   7032  raw 5810  aligned 2548
234   7056  raw 5871  aligned 2544
1234  7008  raw 5812  aligned 2545
```

No size-exact variant beats the baseline.  `3` (field_0x1e4 float) is
especially disruptive: it changes register pressure and re-schedules the
whole body (aligned 1978 -> ~2530).  The four deltas do **not** sum to zero
(-64 B), so the baseline must contain ~64 B of compensating shape elsewhere
(candidates: the `t_field_0x2c8` temp, the `volatile unsigned char *rd`
reload hack, and the scalar-vs-group split of `field_0x10c` — the original
groups `0x108/0x10c/0x110/0x114` while ours emits a scalar `vstr [r0,#268]`
at `d1d2c`).  Finding that compensation is the next step.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10607
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10607/fdiff.py _ZN6LayoutC1Ev        # 7072/7072, diff 5739
python3 /dev/shm/w10607/align_diff.py _ZN6LayoutC1Ev   # 1978
cmake --build cmake-build-match --target check -j6     # RATCHET PASS
```

Scratch `/dev/shm/w10607/`: `fdiff.py`, `align_diff.py`, `measure.sh`,
`variants.py`, `variants2.py`, `Layout.cpp.base`, `orig_layout.asm`,
`our_layout.asm`, `pool.py`.
