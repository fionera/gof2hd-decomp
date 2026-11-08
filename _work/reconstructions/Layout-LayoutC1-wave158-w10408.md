# `Layout::Layout` C1/C2 — wave 158 / worker w10408
## SLP chunk-boundary move: `field_0x128` after `field_0x108`.
## Landed: 7,072 B (size exact), raw **5,753 -> 5,745** (-8),
## aligned **1,984 -> 1,982** (-2).  Gate green.

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original
vaddr `0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Entry = wave-157 HEAD `9e4795206` (w10307 shape): raw same-address diff
**5,753**, aligned instruction diff **1,984**, size 7,072.
Exit: raw **5,745**, aligned **1,982**, size 7,072.
Gate on exit: byte_exact 4507/4524, linked_exact 4507/4524, size 4524/4524,
data 843/843, imports 207, lint CLEAN, `RATCHET PASS`.

## 1. The move

One pure statement relocation (no expression/constant/type change):

`this->field_0x128 = ...;` moved from between `field_0x124` and
`field_0x10c_rowBaseY` to just after `field_0x108`, i.e. immediately
before the `if (Globals::iPadHD) { field_0x130/134/138 ... }` chain.

```
before                          after
field_0x118                     field_0x118
field_0x124                     field_0x124
field_0x128  <-- moved          field_0x10c_rowBaseY
field_0x10c_rowBaseY            field_0x120
field_0x120                     field_0x114_strip5c
field_0x114_strip5c             field_0x238_blk16.b
field_0x238_blk16.b             field_0x108
field_0x108                     field_0x128  <-- here
if (iPadHD) { ... }             if (iPadHD) { ... }
```

## 2. Search that found it

The whole single-statement move space (200 statements x 200 insertion
points, 46,225 variants) was compiled with the exact NDK r18b clang 7.0.2
`-Oz` command and filtered to the 2,056 variants whose
`_ZN6LayoutC1Ev` section is exactly 0x1ba0 (7,072 B).  A `.o`-based
instruction-alignment proxy then ranked them; the top 60 were built with
the real gold link and measured.

Results (real `link_so` + fdiff + align_diff):

| variant | move | size | raw | aligned |
|---|---|---:|---:|---:|
| HEAD baseline | — | 7,072 | 5,753 | 1,984 |
| v_088_093 | stmt 88 before 93 | 7,072 | 5,747 | 1,982 |
| v_089_093 | stmt 89 before 93 | 7,072 | 5,747 | 1,982 |
| v_089_095 | `field_0x128` after `field_0x108` | **7,072** | **5,745** | **1,982** |
| v_101_103 | swap `0x14c`/`0x13c` | 7,072 | 5,748 | 1,986 |

`v_089_095` is the best on both metrics among the size-exact variants.

## 3. Group-base result (honest negative)

The SLP group bases are unchanged by this move (same first-block group
set as the baseline):

```
OURS 14 4 24 34 44 54 64 2c8 7c b0 c0 294 2a4 dc 118 108 128 150 ... 1d0
ORIG 14 4 24 34 44 54 64 2c8 7c c0 b0 ec 294 2a4 108 118 128 140 150 ... 1d0
```

The mission's open group bases `0xec`, `0x140`, `0x204` are **not** fixed.
A full exhaustive sweep of all 46,225 single-statement moves proves they
cannot be: only 17 size-any variants contain a `0xec` group (all are
`ringNearMidThreshold` moved next to `field_0xf0`, sizes 0x1b78..0x1bbc),
and **no** single-statement move at any size produces a `0x140` or
`0x204` group.  Those three require a structural change (per-branch
assignment / basic-block split), not a statement reorder.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10408
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10408/fdiff.py _ZN6LayoutC1Ev        # 7072/7072, diff 5745
python3 /dev/shm/w10408/align_diff.py _ZN6LayoutC1Ev   # 1982
cmake --build cmake-build-match --target check -j6     # RATCHET PASS
```

Scratch `/dev/shm/w10408/`: `base.cpp` (wave-157 source), `stmts.py`
(statement parser), `mvstmt.py`/`mvf.py`/`apply_moves.py` (movers),
`fastsweep.py`/`genall.py`/`compile_one.sh` (46,225-variant compile
sweep), `groupsof.py` (SLP group-base extractor), `align_obj.py`
(.o alignment proxy), `vars/` (variants), `sizes_all.txt`,
`groups_all.txt`, `align7072.txt`.
