# `Layout::Layout` C1/C2 — wave 164 / worker w11005

## First-half store map + explicit 3-way branch shape.  Landed: the first
## two fields as an explicit `if (iPadHD) / else if (n9) / else` chain.
## Raw **5,705 -> 5,695** (-10), aligned **2,082 -> 2,083** (+1), size
## **7,072 B exact**.  (w10906 claimed this in wave 163 but the merge note
## measured 5,705; on this base it reproduces 5,695 — see §5.)

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original
vaddr `0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Baseline = wave-163 final `b3d3a1f68`: raw **5,705**, aligned **2,082**,
7,072 B.  Exit: raw **5,695**, aligned **2,083**, 7,072 B.
Gate: byte_exact 4507/4524, linked_exact 4507/4524, size 4524/4524,
data 843/843, imports 207, lint CLEAN, `RATCHET PASS`.
`.text` diff **84,652 -> 84,642** (-10); `.rel.plt` byte-identical (0);
`.ARM.extab` 725; all section sizes exact.

## 1. Block map (original, from objdump with `--triple=thumbv7-...`)

```
d1028..d1042  head            26 B  (push, Globals::iPadHD load)
d1042..d126c  iPadHD block   554 B  -> b d21e2 (tail)
d126c..d1280  n9 check        20 B
d1280..d12fe  n9 block       126 B  -> b d1860 (common)
d1300..d1570  inline pool    624 B
d1570..d1860  else block     752 B  -> falls into common
d1860..d21e2  common        2434 B  (0x2d0.. then 0x7c..0x1dc)
d21e2..d2802  tail          1568 B  (fields 0x1e8..0x320)
d2802..d2854  out-of-line     82 B
d2854..d2bc8  final pool     884 B
```

Our baseline: iPadHD 568 (+14), n9 check 18 (-2), n9 132 (+6),
pool1 600 (-24), else 752 (equal), common 2434 (equal), tail 1568
(equal), out-of-line 18 vs 82, final pool 760 vs 884.  The 128-B
code/pool split (w10806) is entirely in the first region + pools.

## 2. Original iPadHD store order (per-field map, `0xd1042..0xd1268`)

Nine 4-field `vld1.64`/`vst1.32` groups, in this order:

| group | fields | values |
|---|---|---|
| 1 | 0x14 0x18 0x1c 0x20 | 30, 12, 28, 7 |
| 2 | 0x04 0x08 0x0c 0x10 | 21, 42, 40, 52 |
| 3 | 0x24 0x28 0x2c 0x30 | 7, 7, 4, 42 |
| 4 | 0x34 0x38 0x3c 0x40 | 8, 9, 82, 140 |
| 5 | 0x44 0x48 0x4c 0x50 | 7, 15, 7, 125 |
| 6 | 0x54 0x58 0x5c 0x60 | 487, 341, 63, 90 |
| 7 | 0x64 0x68 0x6c 0x70 | 112, 566, 191, 63 |
| 8 | 0x2c8 0x2cc 0x2d0 0x2d4 | 63, 126, 63, 112 |
| 9 | 0x7c 0x80 0x84 0x88 | 12, 14, 112, 42 |

then scalar/paired stores in this order:

```
0x74,0x78 = 161,369          (strd)
0x2c0,0x2c4 = -1,126         (strd)
0x2d8 = 141
0xac = 85
0x8c,0x90 = 45, 0.46222222f  (strd)
0x94,0x98,0x9c,0xa0 = 14,590,70,35   (stmia.w r4,{r5,r8,r12,lr})
0xa4,0xa8 = 7,85             (strd)
0xc0..0xcc = 9,11,5,253      (vst1)
0xb0..0xbc = 4,20,25,11      (vst1)
0xec..0xf8 = 56,112,281,562  (vst1)
0x294..0x2a0 = 281,562,281,118 (vst1)
0x2a4..0x2b0 = 154,140,147,42  (vst1)
0x108..0x114 = -6,8,16,90    (vst1)
0x118..0x124 = 18,22,22,243  (vst1)
0xd0,0xd4,0xd8 = 21,28,52    (stmia.w r11,{r3,r6,r8})
0xdc,0xe0 = 14.0625f,1.40625f (strd)
0xe4,0xe8 = 28.125f,1.40625f  (strd)
0xfc,0x100 = 7,2             (strd)
0x104 = 5
0x2b4,0x2b8 = 35,9           (strd)
0x128..0x134 = 1,21,78,151   (vst1)
0x140..0x14c = 15,18,2,189   (vst1)
0x150..0x15c = 5,70,70,206   (vst1)
0x160..0x16c = 68,0,35,184   (vst1)
0x170..0x17c = 174,84,84,217 (vst1)
0x180..0x18c = 23,163,23,244 (vst1)
0x190..0x19c = -6,8,16,90    (vst1)
0x1a0..0x1ac = 18,22,22,243  (vst1)
0x1b0..0x1bc = 18,42,46,1    (vst1)
0x1c0..0x1cc = 92,26,40,50   (vst1)
0x1d0..0x1dc = 7,17,5,14     (vst1)
0x138,0x13c = 66,23          (strd)
0x1e0 = 1.40625f
```

Key shape facts:
* The first group is **0x14..0x20**, not 0x04..0x10; the 0x04 group is
  second.  Our baseline already reproduces this order (the compiler
  reorders the per-field ternaries the same way).
* 0x94..0xa0 is a **4-store `stmia`**, not a vector group, because the
  four values are already in general registers.  0x2c0+0x2c4 is a
  `strd`, 0xa4+0xa8 is a `strd`.
* The whole block ends with `b.w 0xd21e2` (tail), so the iPadHD path
  never executes the n9/else/common blocks.

## 3. Original n9 block store order (`0xd1280..0xd12fc`)

```
0x74,0x78 = 207,473                 (strd)
0x2c0,0x2c4,0x2c8,0x2cc = -3,108,45,106  (stmia.w r12!,{r1,r2,r3,r6})
0x14 0x18 0x1c 0x20 = 37,11,30,7    (vst1)
0x04 0x08 0x0c 0x10 = 22,45,51,58   (vst1)
0x24 0x28 0x2c 0x30 = 7,9,6,51      (vst1)
0x34 0x38 0x3c 0x40 = 4,12,106,180  (vst1)
0x44 0x48 0x4c 0x50 = 9,16,10,160   (vst1)
0x54 0x58 0x5c 0x60 = 520,364,51,79 (vst1)
0x64 0x68 0x6c 0x70 = 108,545,170,51 (vst1)
b 0xd1860
```

Note the n9 block sets **0x2c0..0x2cc as one 4-store `stmia`** and then
jumps into the shared common block.  It does **not** set 0x98 or 0x2d0;
those live in the common block.  Our baseline n9 block instead stores
0x2c4, 0x98, 0x2cc, 0x2d0 and omits 0x2c0/0x2c8 (they are emitted
later) — this is the +6 B difference and a large part of the n9 diff.

## 4. Original else block store order (`0xd1570..0xd185e`)

```
0x04,0x08 = retina?30:15, retina?60:30       (strd r1,r4)
0x0c,0x10,0x14 = retina?58:29, retina?74:37, retina?50:22  (stm r1!,{r2,r5,r6})
0x18,0x1c = retina?14:9, retina?40:20        (strd r3,lr)
0x20,0x24 = retina?10:5, retina?10:5         (strd r12,r12)
0x28 = retina?10:5
0x30 = retina?60:30
0x2c = retina?6:3
0x38 = retina?14:7
0x34 = retina?12:6
0x3c = retina?118:59
0x44 = retina?10:5
0x40 = retina?200:100
0x4c = retina?10:5
0x48 = retina?22:11
0x54 = retina?694:347
0x50 = retina?178:89
0x58 = retina?486:243
0x5c..0x68 = iPadLarge ? {90,128,160,806} : retina?{60,88,120,606}:{30,44,60,303}
0x60 = (int)(float)(iPadLarge?128:retina?88:44)   [vstr]
0x64 = iPadLarge?160:retina?120:60
0x68 = (int)(float)(iPadLarge?806:retina?606:303) [vstr]
0x6c = (int)(float)(iPadLarge?272:retina?262:131) [vstr]
0x2c0 = retina?-4:-2
0x70 = (int)(float)(iPadLarge?90:retina?60:30)    [vstr]
0x74,0x78 = retina?230:115, retina?526:263        (strd)
0x2c4 = iPadLarge?180:retina?120:60
0x2c8 = (int)(float)(iPadLarge?90:retina?60:30)   [vstr]
0x2cc = iPadLarge?180:retina?120:60
-> common
```

The else block's first 22 stores are **ascending 0x04..0x58** with
adjacent-pair swaps (0x30 before 0x2c, 0x38 before 0x34, 0x44 before
0x40, 0x4c before 0x48, 0x54 before 0x50).  Our baseline source order
is scrambled (0x04,0x08,0x10,0x1c,0x0c,0x14,0x18,0x24,...) and the
else block emits a different store order.  Reordering the source to
ascending shrinks the function (7,012 B measured), so the scrambled
order is what holds 7,072 B.

## 5. Common block first stores (`0xd1860..`)

```
0x2d0 = (int)(float)(n9?53:retina?60:30)   [vstr s0]
0x2d4,0x2d8 = {retina/n9/iPad conditional}, {retina/n9/iPad conditional}  (strd)
0x7c..0x88 = retina?{18,20,160,60}:{9,10,80,30}  (vst1)
0x8c,0x90 = retina?64:32, retina?0.325:0.65   (vstr)
0x94,0x98 = retina?20:10, retina?840:420
0x9c = (int)(float)(retina?100:50)          [vstr]
0xac,0xa8,0xa0,0xa4 = ...
0xd0..0xe8, 0xfc..0x104, 0x2b4,0x2b8 = ...
0xb0,0xc0,0xec,0x294,0x2a4,0x118,0x108,0x128,0x140,0x138,0x13c = ...
```

Note the original stores **0x94,0x98,0x9c adjacent** in the common
block (0x94/0x98 as `str.w`, 0x9c as `vstr`).  Our source has 0x98 at
line 53, 0x9c at line 75 and 0x94 at line 310 (w10807's tail
relocation), so all three are emitted in the else/tail instead.  Moving
them together shrinks the function (6,984 B measured), so this is not a
free win.

## 6. Landed change

```cpp
if (Globals::iPadHD) {
    this->field_0x4 = 15 * 1.40625f;
    this->footerTextInset = 22 * 1.40625f;
} else if (Globals::n9) {
    this->field_0x4 = 22;
    this->footerTextInset = 37;
} else {
    this->field_0x4 = Globals::retinaDisplay ? 30 : 15;
    this->footerTextInset = Globals::retinaDisplay ? 50 : 22;
}
```

Pure source restructuring of the first two fields (0x04, 0x14) into the
original's explicit 3-way branch shape.  All other statements unchanged.
Size stays 7,072 B.  The order inside the chain is irrelevant (both
`{0x04,0x14}` and `{0x14,0x04}` measure 5,695).

## 7. Measured sweeps (all real link + fn_diff, size checked first)

* All 870 ordered pairs of the first 30 fields (0x04..0x78) as a
  3-way chain: 41 pairs hold 7,072 B.  Best raw 5,695 for any two of
  `{0x04,0x10,0x14}`; next 5,697 for `{0x0c,0x1c}` (aligned 2,081,
  the best aligned); `{0x2c,0x34}` and `{0x2c,0x3c}` give 5,703 with
  aligned 2,078/2,076.
* All triples of `{0x04,0x08,0x0c,0x10,0x14,0x18,0x1c,0x20,0x2c,0x34,0x3c}`
  (990 ordered): best 5,705 — i.e. the pair win is lost as soon as a
  third field joins the chain.
* In-place adjacent-pair chains (no reorder): best 5,702 for
  `{0x34,0x38}`; `{0x50,0x54}` and `{0x74,0x78}` are neutral.
* Stacking the top `{0x04,0x14}` chain with an in-place pair: best
  5,695 (no additional win).
* Chains pairing 0x04 with a far field (0x94, 0x98, 0x2c0, 0x2c4,
  0x2cc, eventBannerScale, 0x204, 0x2bc, 0x214, 0x108, 0x128, 0x140,
  0x2b4, 0x2b8): all shrink the function (6,944..7,056 B), none exact.
* Reordering the first 30 statements to the original else order
  (ascending) or the original iPadHD order: 7,012 / 7,040 B, not exact.
* Moving 0x2c0 next to 0x2c4 (with or without the pair chain): 7,064 B.
* Moving 0x94/0x98/0x9c together (either at 0x90 or at the tail):
  6,984 / 6,936 B.

## 8. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w11005
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j16
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
cmake --build cmake-build-match --target check -j8
```

Scratch `/dev/shm/w11005/`: `orig_layout.dis` / `ours_layout.dis`
(full disassembly), `stores.py` (base-register store-order parser),
`genchain2.py` (3-way chain generator), `sweep_pairs.py` /
`sweep_triples.py` / `sweep_inplace.py` / `sweep_multi.py` /
`sweep_any.py` (sweeps), `pairs.json` / `t2.json` (results),
`diffmap.py` / `regiondiff.py` (per-region raw diff).

## 9. Open questions

* The remaining 128-B lever is still the code/pool split: the original
  materialises ~11 more 4-field groups via `vld1.64`/`vst1.32` in the
  common block (81 `vst1.32` / 86 `vld1.64` / 62 `strd` vs our
  70/77/44).  Reproducing that grouping requires the common-region
  source order (0x7c,0x80,0x84,0x88,0x8c,0x90,0x94,0x98,0x9c,0xac,
  0xa8,0xa0,0xa4,0xd0,...,0x2b8,0xb0,0xc0,0xec,0x294,0x2a4,0x118,
  0x108,0x128,0x140,0x138,0x13c) but that shrinks the function to
  ~6,944 B, so a +128-B pool companion is needed.
* The n9 block should set 0x2c0..0x2cc as one `stmia` and leave 0x98
  and 0x2d0 to the common block; our source has 0x2c0 at line 193 and
  0x98/0x2d0 at lines 53/54.  Moving them alone is not size-exact.
