# `Layout::Layout` C1/C2 — wave 140 / worker w8507
## SLP store-group census of the original hot path + a size-neutral permutation (raw 5,765 -> 5,759)

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr `0xd1028`,
size **7072 B**, source `src/game/ui/Layout.cpp`.  Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`, image base `0x10000`.
Baseline = wave-139 master `d4857f174` (raw same-address diff **5,765**, official fuzzy 45.6).

## 1. NEW evidence: the original hot path is a 4-field SLP-group stream

The original specialises the whole first part on `Globals::iPadHD == true`
(`ldrb sl,[...]; cmp sl,#0; beq.w <cold>` at `0xd1036..0xd103e`).  The specialised
block `0xd1042..0xd1268` writes `this` with **`vst1.32` groups of four consecutive
int fields** plus a few `strd` pairs and scalars.  Decoding each group's literal
vector (base register tracked) gives the exact source field order:

```
0x14 footerTextInset, headerTitleY, field_0x1c, field_0x20_top        {30,12,28,7}
0x04 field_0x4, windowTopInset, field_0xc_leftMargin, field_0x10_rightMargin {21,42,40,52}
0x24 field_0x24, buttonInsetX, field_0x2c_rowHeight, field_0x30_rowHeight {7,7,4,42}
0x34 field_0x34, field_0x38, field_0x3c, field_0x40                     {8,9,82,140}
0x44 field_0x44, field_0x48, field_0x4c, field_0x50                     {7,15,7,125}
0x54 field_0x54, field_0x58, field_0x5c, field_0x60                     {487,341,63,90}
0x64 field_0x64, field_0x68, field_0x6c, field_0x70_rowHeight           {112,566,191,63}
0x2c8 field_0x2c8, field_0x2cc, field_0x2d0, field_0x2d4               {63,126,63,112}
0x7c field_0x7c, field_0x80_touchMargin, field_0x84, field_0x88        {12,14,112,42}
0x74 field_0x74, field_0x78                                            strd
0x2c0 field_0x2c0, field_0x2c4                                         strd
0x2d8 field_0x2d8                                                      str
0xac field_0xac                                                        str
0x8c field_0x8c, field_0x90                                            strd
0x94 field_0x94, field_0x98, field_0x9c, field_0xa0                    stmia
0xa4 field_0xa4, field_0xa8                                            strd
0xc0 field_0xc0, field_0xc4, field_0xc8, field_0xcc                    {9,11,5,253}
0xb0 field_0xb0, field_0xb4, field_0xb8, field_0xbc                    {4,20,25,11}
0xec ringNearMidThreshold, field_0xf0, ringMidFarThreshold, field_0xf8  {56,112,281,562}
0x294 field_0x294, field_0x298, field_0x29c, field_0x2a0                {281,360,12,200}
0x2a4 field_0x2a4, field_0x2a8, field_0x2ac, field_0x2b0                {110,140,147,30}
0x108 field_0x108, field_0x10c_rowBaseY, field_0x110_strip58, field_0x114_strip5c {-2,1,2,1}
0x118 field_0x118, field_0x11c, field_0x120, field_0x124                {9,35,21,1}
0xdc field_0xdc, animSpeedScale                                        strd
0xe4 driftSpeedDivisor, ringRadiusScale                                strd
0xfc hudInset, oreLabelYOffset                                         strd
0x104 oreTextYOffset                                                   str
0x2b4 field_0x2b4, field_0x2b8                                          strd
0x128 field_0x128, field_0x12c, field_0x130, field_0x134                {1,21,78,151}
0x140 field_0x140, field_0x144, field_0x148, field_0x14c                {15,18,2,189}
0x150 field_0x150, field_0x154, field_0x158, field_0x15c                {5,70,70,206}
0x160 field_0x160, field_0x164, field_0x168, field_0x16c                {68,0,35,184}
0x170 field_0x170, field_0x174, field_0x178, field_0x17c                {174,84,84,217}
0x180 field_0x180, field_0x184, field_0x188, field_0x18c                {23,163,23,244}
0x190 field_0x190, field_0x194, field_0x198, field_0x19c                {-6,8,16,90}
0x1a0 field_0x1a0, field_0x1a4, field_0x1a8, field_0x1ac                {18,22,22,243}
0x1b0 field_0x1b0, field_0x1b4, field_0x1b8, field_0x1bc                {18,19,26,13}
0x1c0 field_0x1c0, field_0x1c4, field_0x1c8, field_0x1cc                {92,26,40,50}
0x1d0 field_0x1d0, field_0x1d4, field_0x1d8, field_0x1dc                {7,17,5,14}
0x138 field_0x138, field_0x13c                                          strd
0x1e0 eventBannerScale                                                 str
```

Every value matches our per-field ternary for `iPadHD` (e.g. `0x190` = `-6`, `0x1c0` =
`66*1.40625f` = 92, `0x1cc` = `36*1.40625f` = 50), so the source is a straight-line
assignment stream and the if-chains at `0x130/0x134/0x138`, `0x190` and `0x1cc` are
**specialised and vectorised** by clang (they must sit adjacent to their group members).

## 2. Hypothesis tested: reorder the source to this store order

Reordering `0x140..0x1dc` ascending (and moving `field_0x140/0x144` into the run) makes
clang emit groups at `0x144,0x154,...,0x1c4` — still one field off, because `field_0x138/0x13c`
sit before the run in our source whereas the original emits them *after* `0x1dc`.
Converting the `0x190` if-chain to a ternary and moving `0x138/0x13c` after `0x1dc`
also fails on the size gate:

| variant | size | raw | fuzzy |
|---|---:|---:|---:|
| base | 7072 | **5,765** | 45.6 |
| region reorder, keep chains | 6920 | 5,747 | 42.7 |
| region reorder, `0x190` ternary | 6880 | 5,742 | 42.8 |
| move `field_0x138/0x13c` after `0x1dc` | 7028 | 5,956 | 42.1 |
| move only `field_0x13c` | 7056 | 5,929 | 38.5 |
| move `field_0x140/0x144` before `0x148` | 7032 | 5,939 | 43.5 |

The reorder creates extra `vst1` groups and therefore **removes** ~150 B; the original's
7072 B must contain compensating code in the cold (`iPadHD==false`) path that our source
does not yet reproduce.  Landing it requires reconstructing that cold path first.

## 3. Landed: size-neutral permutation of the first 16 assignments

A random-permutation hill climb over the first 16 independent assignments (only accepting
`size == 7072`) found a 3-line move that lowers the deterministic metric:

```
field_0x1c            -> after field_0x10_rightMargin
field_0x40            -> after field_0x24
field_0x2c_rowHeight  -> after field_0x38
```

| metric | before | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** |
| raw same-address diff | 5,765 | **5,759** |
| `.text` differing bytes | 84,651 | **84,645** |
| official fuzzy (advisory) | 45.6 % | 44.8 % |
| `byte_exact` / `linked_exact` | 4507 / 4507 | **4507 / 4507** |
| `size_exact` / `data_exact` | 4524 / 843 | 4524 / 843 |
| imports / lint / drift | 207 / CLEAN / 0 | 207 / CLEAN / 0 |

`cmake --build cmake-build-match --target check -j6` -> `RATCHET PASS`.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w8507
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
python3 /dev/shm/w8507/score.py                          # 7072 / 5759 / 44.8
```

Scratch harnesses (`/dev/shm/w8507/`, not committed): `score.py` (single-function raw +
exact verify.py fuzzy), `stores2.py` (base-register-tracked `vst1`/`vstr`/`strd`/`stmia`
census), `units.py` (constructor statement splitter), `search.py` (random swap/move climb),
`psearch.py` (first-16 permutation climb).

## 5. Open questions

- Reconstruct the cold (`iPadHD==false`) path so the store-order reorder can land at 7072 B.
  The cold blocks are `0xd126c..0xd12fc` (n9 branch) and `0xd1570..0xd185c`, then `0xd1860`.
- The `0xdc..0xe8` window: original emits `strd` pairs (`0xdc/0xe0`, `0xe4/0xe8`), ours
  emits one `vst1` at `0xdc`; that is a float-pair store, not an int SLP group.
- The first literal-pool load order (`d0-d1` first, `d2-d3` last in the original) is
  scheduler-determined and does not follow the store order.
