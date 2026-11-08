# `Layout::Layout` C1/C2 — wave 162 / worker w10806
## Full store-order dump + whole-constructor source-order rewrites.  No source
## win; baseline re-measured raw **5,715**, aligned **2,050**, size **7,072**.
## Key new finding: the original's *code* is exactly 128 B smaller than ours
## (6,186 vs 6,314 B) and its literal pool exactly 128 B larger, and the
## n9 branch is a 124-B "fast path" that jumps into a shared common block.

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original
vaddr `0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Baseline = wave-161 HEAD `29248c1d0`: raw **5,715**, aligned **2,050**,
7,072 B.  Exit: unchanged (no source change landed).

## 1. Complete original store order (Ghidra/objdump)

Parsed every `str/strd/strb/strh/vstr/vst1.32/stmia` with a base-register
tracker (`/dev/shm/w10806/stores.py`).  The constructor is one big device
dispatch, not per-field ternaries:

```
d1042..d1268  iPadHD block   (0x226 = 550 B)  -> b d21e2 (tail)
d1280..d12fc  n9 block       (0x07c = 124 B)  -> b d1860 (common)
d1570..d185e  else block     (0x2ee = 750 B)  -> falls into d1860
d1860..d21e0  common block   (0x980 = 2432 B)
d21e2..d2851  tail           (0x66f = 1647 B)  (fields 0x1e8..0x320)
d2802..d2850  out-of-line iPad checks (branch back into the else block)
```

The **n9 block only sets the first 8 groups** (`0x2c0..0x2cc`, then
`0x14/0x04/0x24/0x34/0x44/0x54/0x64` groups) and jumps to the common
block; the else block sets the same 8 groups with retina/iPad conditional
moves and falls into the same common block.  The iPadHD block sets
everything and jumps over the common block to the tail.

Our build has the same 5-block shape but the **n9 block is 730 B**
(all n9 values inline) and the common block is only 1,078 B.  This is the
whole 128-B code-size delta: ours = 6,314 B code + 758 B pool;
original = 6,186 B code + 886 B pool.

## 2. iPadHD block store order (field offsets, groups expanded)

```
14 18 1c 20 | 04 08 0c 10 | 24 28 2c 30 | 34 38 3c 40 | 44 48 4c 50
54 58 5c 60 | 64 68 6c 70 | 2c8 2cc 2d0 2d4 | 7c 80 84 88 | 74 78
2c0 2c4 | 2d8 | ac | 8c 90 | 94 98 9c a0 | a4 a8 | c0 c4 c8 cc
b0 b4 b8 bc | ec f0 f4 f8 | 294 298 29c 2a0 | 2a4 2a8 2ac 2b0
108 10c 110 114 | 118 11c 120 124 | d0 d4 d8 | dc e0 | e4 e8
fc 100 | 104 | 2b4 2b8 | 128 12c 130 134 | 140 144 148 14c
150 154 158 15c | 160 164 168 16c | 170 174 178 17c | 180 184 188 18c
190 194 198 19c | 1a0 1a4 1a8 1ac | 1b0 1b4 1b8 1bc | 1c0 1c4 1c8 1cc
1d0 1d4 1d8 1dc | 138 13c | 1e0
```

The **cold (else) block preserves source order** and is the best proxy for
the original statement order.  Its first-occurrence order is ascending
`0x04..0x78` with `0x2c0` inserted between `0x6c` and `0x70`, then
`0x2c4/0x2c8/0x2cc`, then `0x2d0/0x2d4/0x2d8`, then `0x7c..0x9c`,
`0xac/0xa8/0xa0/0xa4`, `0xd0..0xe8`, `0xfc..0x104`, `0x2b4/0x2b8`,
`0xb0/0xc0/0xec/0x294/0x2a4/0x118/0x108/0x128/0x140/0x138`, ...

## 3. Measured whole-constructor rewrites (all real `link_so`)

| variant | size | raw | aligned |
|---|---:|---:|---:|
| baseline | 7,072 | **5,715** | **2,050** |
| `0x04..0x78` ascending (30 stmts) | 7,060 | 5,820 | 2,230 |
| whole HD region in original HD store order | 6,796 | 5,834 | 2,500 |
| `0x94/0x98/0x9c/0xa0/a4/a8` + `0x2c0` local group | 6,984 | 5,921 | 2,615 |
| `0x2c0` next to `0x2c4` | 7,056 | 5,870 | 2,257 |
| `0x2c0`+`0x98` local group | 7,032 | 5,823 | 2,235 |
| `eventBannerScale` after `0x1dc` | 6,992 | 5,854 | 2,226 |
| `eventBannerScale`+`0x204`+`0x2bc` after `0x1dc` | 6,944 | 5,925 | 2,407 |

Note the last row: moving `eventBannerScale`, `field_0x204` and
`field_0x2bc` to the end of the HD region reproduces the original's
**exact 128-B code-size reduction** (7,072 -> 6,944 = code -128), but the
literal pool does *not* grow, so the grouping is still wrong.  This is the
cleanest confirmation that the 128-B delta is a code/pool split, not a
missing statement.

The current wave-161 statement order is locally optimal for the raw metric;
every whole-region permutation tried here is worse on both metrics.  The
remaining lever is the **n9 fast-path shape** (124 B vs our 730 B), i.e.
restructuring the first 8 groups as an explicit
`if (iPadHD) {...} else if (n9) {...} else {...}` chain so the compiler
shares the rest of the constructor in a common block.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10806
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10806/fdiff.py _ZN6LayoutC1Ev        # 7072/7072, diff 5715
python3 /dev/shm/w10806/align_diff.py _ZN6LayoutC1Ev   # 2050
```

Scratch `/dev/shm/w10806/`: `stores.py` (base-register store-order parser),
`orig_stores.txt`, `ours_stores.txt`, `reorder.py` (whole-region reorder
harness), `fastsize.sh` (0.13 s object-size harness), `Layout.cpp.base`.
