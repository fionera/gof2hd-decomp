# `Layout::Layout` C1/C2 — wave 152 / worker w9808
## The three "device groups" are NOT missing literal words in the current
## build: the store counts already match the original.  The pool gap is
## elsewhere.  Full store-offset and literal-pool tables committed alongside.
## Source unchanged (baseline 7,072 B / raw 5,741 / aligned 2,189 kept).

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Baseline = wave-151 HEAD `9d491d46a`: raw same-address diff **5,741**,
aligned instruction diff **2,189**, size 7,072.  Gate on entry and exit:
byte_exact 4507/4524, linked_exact 4507/4524, size 4524/4524, data 843/843,
imports 207, lint CLEAN, `RATCHET PASS`.

## 1. New result: the store-offset counts of the three target groups match

The wave-150 note (`Layout-LayoutC1-wave150-w9708.md`) claimed the
`0x2c4..0x2d8`, `0x2f4..0x318` and `0x7c..0x94` groups were missing
constant-vector stores (8 vs 2, 7 vs 0, 3 vs 0).  That count was taken on the
**ascending-tail** variant (w9608), not on current HEAD.  On HEAD the store
counts are already equal:

```
0x2c4..0x2d8   ORIG 3x 0x2c4, 1x 0x2c8, 3x 0x2cc, 1x 0x2d0, strd 0x2d4/0x2d8
               OURS 3x 0x2c4, 1x 0x2c8, 3x 0x2cc, 1x 0x2d0, 0x2d8 + 0x2d4 separate
0x2f4..0x318   ORIG 1x 0x2f4(HD), 1x 0x2f4, 1x 0x2f8, 1x 0x308, 4x 0x30c, 1x 0x314, 1x 0x318
               OURS 1x 0x2f4(HD), 1x 0x2f4, 1x 0x2f8, 1x 0x308, 4x 0x30c, 1x 0x314, 1x 0x318
0x7c..0x9c     ORIG strd 0x7c/0x80, strd 0x84/0x88, 2x 0x94, 1x 0x98, 1x 0x9c
               OURS strd 0x7c/0x80, strd 0x84/0x88, 2x 0x94, 1x 0x98, 1x 0x9c
```

The only *store-shape* difference in the three groups is the
`0x2d4`/`0x2d8` pair: the original emits one `strd r4,r6,[r0,#724]` at
`0x88e`; ours emits `str 0x2d8` at `0x882` and `str 0x2d4` at `0x8ac`
(the w9504 open question).  The source currently has an explicit `if/else`
for `0x2d4` (lines 51-59, with the `volatile unsigned char *rd` hack).

## 2. Experiment: make `0x2d4`/`0x2d8` one `strd` (variant D)

Replacing the `0x2d4` if/else with the equivalent ternary and placing it
directly before `0x2d8`:

```cpp
this->field_0x2d4 = Globals::iPadHD ? 112 : ((Globals::retinaDisplay || Globals::n9) ? (Globals::iPad ? 160 : 100) : (Globals::iPad ? 100 : 60));
this->field_0x2d8 = Globals::iPadHD ? 141 : ((Globals::retinaDisplay || Globals::n9) ? (Globals::iPad ? 200 : 125) : (Globals::iPad ? 125 : 75));
```

does produce the target `strd r5,r2,[r0,#724]` (at `0x8a0`) but shrinks the
function to **7,056 B** (-16).  Adding the ascending `0x2c8`-before-`0x2cc`
reorder (variant H) keeps 7,056 and raw 5,823/aligned 2,197.  The `-16` cannot
be paid back by any single reorder tried (all give 7,056 or 7,024).

## 3. Experiment: full device-major `0x2f4..0x318` (variants C/F)

| variant | size | raw | aligned | literal words |
|---|---:|---:|---:|---:|
| HEAD baseline | **7,072** | **5,741** | **2,189** | 120 |
| C: full 6-arm if/else `0x2f4..0x318` | 6,912 | 5,746 | 3,080 | 123 |
| F: 6-arm if/else `0x2f4..0x308` only | 7,092 | 5,743 | 2,259 | 121 |
| D: `0x2d4` ternary (strd) | 7,056 | 5,822 | 2,194 | 120 |
| H: D + `0x2c8` before `0x2cc` | 7,056 | 5,823 | 2,197 | 120 |
| J: H + ascending `0x2c4..0x2d8` | 7,064 | 5,861 | 2,247 | 118 |
| E: `0x140`/`0x144` moved into the `0x128` SLP group | 6,960 | 5,888 | 2,605 | 117 |
| I: D + `0x98` moved before `0xac` | 7,024 | 5,887 | 2,391 | 117 |

No variant grows the literal-word multiset.  The full device-major rewrite
(variant C) **shrinks** the pool (120 -> 123 distinct referenced words is only
+3, not +44; the function is 160 B short).  This falsifies the wave-150
premise that the three groups carry the 44 missing words.

## 4. Where the pool words actually are (multiset diff)

`Layout-LayoutC1-wave152-w9808-tables.txt` (committed alongside) contains the
full 302-store ORIG table, the 353-store OURS table, both literal-pool tables,
and the word-multiset diff.  Summary of the multiset diff:

* ORIG-only words include the GOT offsets `13db84 13dc04 13dc9a 13dd96
  13ddb4 13de3c 13df2e` (the per-arm `iPadLarge`/`iPad`/`retina`/`n9` GOT
  reloads), the floats `41400000 (12.0) 41e10000 (28.0) 42800000 (64.0)
  40b40000 (5.625)`, and `fffffffe x3`.
* OURS-only words include the GOT offsets `13daca 13db80 13dbe0 13dcda
  13dd38 13dd7c 13ddce 13dea0` and the floats `42a40000 (82.0) 42c40000
  (98.0) 42f40000 (122.0) 430ca000 (140.625) 42440000 (49.0) 42740000
  (61.0)`.

So the original reloads each device global **once per device arm** (7 extra
GOT words), while our flat ternary chain loads each GOT slot once and reuses
it.  The extra constant floats are the per-arm literals that the original
materialises as `vldr`/`vld1.64` instead of `movs` immediates.

## 5. Conclusion / next lever

The 44-word gap is **not** in the three named groups on current HEAD; it is
the per-arm GOT reload + per-arm float materialisation of the device-major
partition.  Reproducing it needs the `iPadHD`/`n9`/`shared` arms to be separate
source blocks over the *whole* early body (w9710's finding), not a rewrite of
the three tail groups.  The `0x2d4`/`0x2d8` `strd` is a real structural win
(variant D) but costs 16 B; a +16 B companion is still open.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w9808
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j16
python3 /dev/shm/w9808/fdiff.py _ZN6LayoutC1Ev        # 7072/7072, diff 5741
python3 /dev/shm/w9808/align_diff.py _ZN6LayoutC1Ev   # 2189
python3 /dev/shm/w9808/tables.py                      # store + pool tables
```

Scratch `/dev/shm/w9808/`: `fdiff.py`, `align_diff.py`, `poolw.py`,
`stores2.py`, `tables.py`, `eval.sh`, `mkC.py`..`mkJ.py`, `Layout.base.cpp`,
`Layout.C.cpp`..`Layout.J.cpp`, `orig.dis`, `ours.dis`, `fulltables.txt`.
