# `Layout::Layout` C1/C2 — wave 153 / worker w9910
## Exhaustive single-statement-move sweep on the ascending skeleton: **624**
## size-7,072 candidates, but none beats the landed w9810 `1a8/198` swap
## (aligned 2,148).  The ascending-only skeleton is 7,068 / 2,139; every
## +4 B companion found so far costs >= +9 aligned.  Source unchanged.

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original
vaddr `0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Entry = wave-152 HEAD `4a6104a43`: raw same-address diff **5,800**, aligned
instruction diff **2,148**, size 7,072.  Gate on exit: byte_exact 4507/4524,
linked_exact 4507/4524, size 4524/4524, data 843/843, imports 207, lint CLEAN,
`RATCHET PASS`.

## 1. Reproduce the w9809/w9810 split

Undoing only the landed `field_0x198` <-> `field_0x1a8` statement swap
(source order back to `... 1a0, 1a8, 198, 1ac`) gives the w9809 "ascending
only" skeleton exactly:

| variant | fn size | raw | aligned |
|---|---:|---:|---:|
| HEAD (ascending + 1a8/198 swap) | **7,072** | **5,800** | **2,148** |
| undo 1a8/198 swap ("ascending only") | 7,068 | 5,801 | **2,139** |

So the missing 4 B is a real instruction, and the landed swap is a +4 B
companion that costs +9 aligned.  The mission's target is a *different* +4 B
companion that keeps aligned at 2,139.

## 2. Exhaustive single-statement-move sweep (new)

Built the ascending-only source (7,068) and swept **every** single-statement
move `i -> j` over all 231 constructor statements (53,130 variants).  Each
variant was compiled with the exact NDK r18b clang 7.0.2 flags and the
`.text._ZN6LayoutC2Ev` section size read from the object (this equals the
final `.so` symbol size, verified on the baseline).  624 moves land at
7,072 B.  The best of those, measured through the full gold link:

| move (statement indices) | fn size | raw | aligned |
|---|---:|---:|---:|
| `125 -> 126` = the landed 1a8/198 swap | **7,072** | **5,800** | **2,148** |
| `126 -> 124` / `126 -> 125` (equivalent pairing) | 7,072 | 5,800 | 2,148 |
| `100 -> 83` | 7,072 | 5,810 | 2,149 |
| `99 -> 74` | 7,072 | 5,814 | 2,150 |
| `99 -> 75` | 7,072 | 5,815 | 2,151 |
| `106 -> 74` | 7,072 | 5,767 | 2,154 |
| `195 -> 40` | 7,072 | **5,736** | 2,163 |

No single-statement move reaches aligned < 2,148 at 7,072 B.  (`195 -> 40`
is the best raw-diff move, 5,736, but costs +15 aligned.)

## 3. Targeted 0x1a8/0x198 pairing variants (all on HEAD)

| variant | fn size | raw | aligned |
|---|---:|---:|---:|
| HEAD | **7,072** | **5,800** | **2,148** |
| `1a0` after `198` (no-op reorder) | 7,072 | 5,800 | 2,148 |
| `1a4` after `1a0` | 7,064 | 5,816 | 2,245 |
| `1a4` after `198` | 7,044 | 5,812 | 2,110 |
| `1a4` after `19c` | 7,128 | 6,009 | 2,291 |
| `1a8` after `1ac` | 7,008 | 5,818 | 2,604 |
| `198` before `1a0` | 7,068 | 5,810 | 2,150 |
| `18c` after `1a0` | 7,068 | 5,815 | 2,141 |
| `19c` after `18c` | 7,068 | 5,810 | 2,141 |
| `180` after `19c` | 7,068 | 6,002 | 2,306 |
| `188` after `180` | 7,072 | 5,806 | 2,172 |

`1a4 after 198` is the best aligned variant seen anywhere (2,110) but is
28 B short of the required size.  `18c after 1a0` / `19c after 18c` are
2,141 at 7,068 (still 4 B short).

## 4. `0x2c4..0x2d8` group permutations (new)

All 720 permutations of the six statements `0x2c4, 0x2d0, 0x2cc, t_2c8,
0x2c8, 0x2d8` on the ascending-only skeleton: only 4 permutations are
7,072 B.  Full-link measurements:

| permutation (source order) | fn size | raw | aligned |
|---|---:|---:|---:|
| `2c4, 2d0, 2cc, t, 2d8, 2c8` | 7,072 | 5,889 | 2,214 |
| `2cc, 2d0, 2c4, t, 2d8, 2c8` | 7,072 | 5,829 | 2,330 |
| `2d0, 2cc, 2c4, t, 2d8, 2c8` | 7,072 | 5,834 | 2,392 |
| `2d8, t, 2d0, 2c8, 2cc, 2c4` | 7,072 | 6,178 | 4,171 |

All worse than 2,148.  The original's `0x2c4..0x2d8` store order is
`2c4, 2c8, 2cc, 2d0, 2d4/2d8` (see table below), i.e. ascending, but our
source already emits `2c4` first and the SLP pairing of `2cc/2d0` is not
reproducible by reordering alone (it is a device-major block in the original).

## 5. `0x7c..0x94` group

Moving `field_0x94` (statement 204) to immediately after `field_0x90`
(statement 47) keeps 7,072 B but regresses to raw 5,830 / aligned 2,216.
The original stores `0x94` at function offset `0x944` (right after
`vstr @0x90` at `0x934`); ours stores it at `0x1594`, but the move does not
compose with the rest of the flat chain.

## 6. `0x30c..0x318` if/else rewrite

Replacing the four nested ternaries `0x310/0x30c/0x314/0x318` with the
original's device-major `if/else` chain (iPadLarge, iPad, n9, retinaDisplay,
else; all four stores per arm) gives 7,068 B (A1: raw 5,793, aligned 2,204)
or 7,060 B (A2, `0x310` first: raw 5,789, aligned 2,193).  It does not add
the missing `strd @0x30c/@0x310`: the original emits **4** `strd @30c/@310`
(one per branch) plus `str @314`/`str @318`; ours emits 3 `strd` + one
`stmia @30c(3)`.

## 7. Store tables (function-relative offsets)

`0x198..0x1ac`:
```
ORIG   0eca strd @198/@19c  0ece str @1a0  1032 strd @198/@19c
       1036 str @1a0        1058 str @1a4   1064 str @1a4
       1072 vstr @1a8       1086 str @1ac   114c strd @198/@19c
       1166 str @1ac        1176 str @1ac
BASE   0ff0/1004/102e/105e/108c str @1a4    10d6 strd @198/@19c
       10da str @1a0        10f8 strd @198/@19c  1104 str @1a0
       1108 str @1a8        113a strd @198/@19c  113e str @1a0
       115c vstr @1a8       117c str @1ac   123e str @1ac   18a0 str @1ac
```
The original stores `0x1a4` *after* `0x1a0` (`0x1058`), ours stores it early
(`0x0ff0`); moving it there (`1a4 after 1a0`) is 7,064 / 2,245.

`0x2c4..0x2d8`:
```
ORIG   00dc vst1 @2c8  0100 str @2d8  07b8/07c6/07e2 str @2c4
       07f8 vstr @2c8  0804/0812/082e str @2cc  085a vstr @2d0
       088e strd @2d4/@2d8
BASE   00d6 vst1 @2c8  00f0 str @2c4  0104 str @2d8  0298 str @2c4
       02a8 strd @2cc/@2d0  07a6/07d0/07ee str @2c4  0808 vstr @2d0
       0812/0822/083e str @2cc  0860 vstr @2c8  0882 str @2d8
       08aa str @2d4
```
The original's `0x2c4` if-chain lives at `0x7b8..0x80a`; ours is split
between `0x0f0/0x298` and `0x7a6..0x7ee`, and our `0x2d4/0x2d8` are not a
`strd` pair.

`0x7c..0x94`:
```
ORIG   00ea vst1 @7c  010e strd @8c  0116 stmia @94(4)
       08a4 vst1 @7c  090c strd @7c  0910 strd @84
       0930 vstr @8c  0934 vstr @90  0944 str @94  096e str @94
BASE   00e4 vst1 @7c  010c strd @8c  08c0 vst1 @7c
       093c strd @7c  0940 strd @84  0962 vstr @8c  0966 vstr @90
       1594 str @94  160e str @94  164a str @94
```

`0x2f4..0x318` (unchanged from w9810): original `0x1642 strd @2f4/@2f8`,
`0x164a strd @2fc/@300`, `0x164e str @304`, `0x16de str @308`,
`0x16ee strd @30c/@310`, `0x16f2 str @314`, `0x1750 str @318`; ours
`0x170c/0x1714/0x1718`, `0x17a6`, `0x17d4/0x17e6/0x17fa`, `0x1808`,
`0x181a`.

## 8. Pool / code split

| build | size | code end | code | pool words | pool bytes |
|---|---:|---:|---:|---:|---:|
| ORIGINAL | 7,072 | `0x182c` | 6,188 | **221** | **884** |
| HEAD (asc + swap) | 7,072 | `0x18a8` | 6,312 | 190 | 760 |
| ascending only | 7,068 | `0x18a4` | 6,308 | 190 | 760 |
| `1a4 after 198` | 7,044 | — | — | — | — |

The 124 B code/pool gap is still the device-major rewrite (w9710): the
original spends 124 B less code and 124 B more pool.  The ascending reorder
moves 4 B of code but leaves the pool at 190 words.

## 9. Conclusion / next lever

Single-statement moves are exhausted: the landed `1a8/198` swap is the unique
best +4 B companion at 2,148.  The next lever must be a *multi-statement*
device-major rewrite that both adds the missing `strd @30c/@310` (the original
has 4, we have 3) and reproduces the original's early `if (iPadHD)` block
(9 `vst1.32 {d2-d3}` stores at `0x7c..0xea`), which is the source of the
124 B code/pool gap.

## 10. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w9910
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9910/fdiff.py _ZN6LayoutC1Ev        # 7072/7072, diff 5800
python3 /dev/shm/w9910/align_diff.py _ZN6LayoutC1Ev   # 2148
cmake --build cmake-build-match --target check -j6    # RATCHET PASS
```

Scratch `/dev/shm/w9910/`: `stmts.py` (statement splitter), `emit.py`,
`layout.py` (perm/move/measure), `sizesweep.py` (53,130-variant size sweep,
`Layout.asc.cpp` base), `measure624.py` (full-link measure of the 624
size-7,072 candidates), `perm2c4.py` (720-perm group sweep), `storetable.py`,
`pool.py`, `fastdiff2.py`, `measure_so.py`, `b1..b8` (parallel link dirs).
