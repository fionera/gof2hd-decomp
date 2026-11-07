# `Layout::Layout` C1/C2 — wave 133 / worker w7751
## Outer `if (iPadHD) / else if (n9) / else` tree rebuilt from Thumb: no score win; landed `0x170` reorder

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, size **7072 B**, source `src/game/ui/Layout.cpp`.
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
Baseline = wave-133 master `ec0acea1e` (7072 B / 45.028890217175 % unrounded, 5,789
differing bytes in the target function).

## Result — landed: `0x170` before `0x16c` (size-exact, −1 differing byte)

| metric | before (HEAD) | after |
|---|---:|---:|
| `_ZN6LayoutC1Ev` size | 7072 | **7072** |
| unrounded `difflib` ratio | 45.028890217175 % | **45.028890217175 %** |
| target-function differing bytes | 5,789 | **5,788** (−1) |
| `byte_exact` / `linked_exact` | 4506 / 4506 | **4506 / 4506** |
| `size_exact` / `data_exact` | 4524 / 843 | 4524 / 843 |
| imports / lint / drift | 207 / CLEAN / 0 | 207 / CLEAN / 0 |

`cmake --build cmake-build-match --target check -j6` → `RATCHET PASS`.

The landed source change is a two-line swap: `field_0x170` is assigned immediately
before `field_0x16c`, so the `0x170/0x174/0x178/0x17c` window is in ascending order.
The function stays 7072 B and the raw differing bytes fall by one.  The fuzzy ratio
is unchanged (the reorder does not alter the normalized instruction stream); this is
a byte-level win only.

## 1. New hypothesis: decode and rebuild the original outer device tree

Manual Thumb decode of the original prologue (function-relative `0x0..0x244`):

```
d1028  ldrb.w sl,[iPadHD]; cmp sl,#0; beq d126c
       --- HD arm d1042..d1268 ---
       vector constant groups: 0x14 0x4 0x24 0x34 0x44 0x54 0x64
                               0x2c8 0x7c 0x74/0x78 0x2c0/0x2c4 0x2d8 ...
       b d21e2
d126c  ldrb.w fp,[n9]; cmp fp,#0; beq d1570
       --- n9 arm d127e..d12fc ---
       0x74/0x78 0x2c0..0x2cc 0x14 0x4 0x24 0x34 0x44 0x54 0x64
       b d1860
d1570  --- default arm d1570..d185e ---
       0x4..0x78 0x2c0..0x2cc
       fallthrough d1860
d1860  --- non-HD common d1860..d21e0 ---
       0x2d0..0x2d8 0x7c..0x1e0 (n9/retina/iPadLarge/iPad branch tree)
d21e2  --- all-device common tail ---
       0x1e8..0x3fc
```

So the original source shape is:

```cpp
if (Globals::iPadHD) {
    /* HD arm: fields 0x4..0x1e0 + 0x2c0..0x2d8 */
} else {
    if (Globals::n9) { /* n9 arm: 0x4..0x78 + 0x2c0..0x2cc */ }
    else             { /* default arm: same field set */ }
    /* non-HD common: 0x2d0..0x2d8 + 0x7c..0x1e0 */
}
/* common tail: 0x1e8..0x3fc */
```

This is **not** `iPadHD / iPadLarge / else` at the top level: the second condition is
`n9`; `iPadLarge` only appears inside the default/non-HD branch tree.

## 2. Faithful three-arm rebuild (built, measured, not landed)

A generator (`/dev/shm/w7751/gen3.py`) emitted each arm in the original **store order**
(extracted from objdump with a base-register tracker, `/dev/shm/w7751/order.py`),
using the current source's HD/n9/default branch expressions:

* HD arm: 145 stores, order `0x14 0x18 0x1c 0x20 0x4 0x8 0xc 0x10 ... 0x1e0`.
* n9 arm: 34 stores, order `0x74 0x78 0x2c0..0x2cc 0x14 ... 0x70`.
* default arm: 34 stores, order `0x4 0x8 ... 0x58 0x5c..0x68 0x6c 0x2c0 0x70 0x74/0x78 0x2c4 0x2c8 0x2cc`.
* non-HD common: `0x2d0..0x2d8 0x7c..0x1e0` in store order.
* common tail: current source unchanged.

Result: **5492 B**, ratio **20.669643 %**, diffbytes 4,935.  The 1,580 B deficit is
real code the flat ternaries do not emit: the original non-HD common arm is a large
`n9`/`retina`/`iPadLarge`/`iPad` branch tree with `vbsl` vector selects, and the
all-device tail duplicates several stores.  Writing the tree explicitly (w7729's
variant A) already showed the same effect on the tail: it shrinks the body because
clang lowers the tree to cmov/scalar stores instead of the original's vector form.

The HD arm alone is not the lever: the faithful arm's first ~100 instructions still
differ because the original hoists all constant-vector loads before the stores
(`vld1 d0-d1`, `vld1 d30-d31`, … then `vst1` groups), while the current source lets
clang interleave each load with its store.  Matching that schedule needs the original
source order, which is not the store order.

## 3. Reorder sweeps on the flat-ternary baseline (all real `link_so` builds)

| variant | size | ratio % | diffbytes |
|---|---:|---:|---:|
| HEAD | 7072 | 45.0289 | 5,789 |
| **`0x170` before `0x16c` (landed)** | **7072** | **45.0289** | **5,788** |
| first group ascending | 7012 | 21.9365 | 6,005 |
| first group in HD store order | 6888 | 19.1537 | 5,917 |
| first group in default store order | 6932 | 17.5333 | 5,905 |
| `0x140..0x1dc` ascending | 6864 | 38.8451 | 5,784 |
| `0x194..0x1dc` ascending | 6936 | 38.8900 | 5,783 |
| `0x118` group contiguous | 7048 | 45.0468 | 5,875 |
| `0x128` group contiguous | 7088 | 17.6215 | 6,232 |
| `0x170..0x17c` group contiguous | 7080 | 45.2808 | 5,842 |
| `field_0x90` simplified to `(n9||retina||iPad) ? 0.325f : 0.65f` | 7072 | 45.3785 | 5,793 |
| `field_0x94` moved after `field_0x90` | 7072 | 46.8594 | 5,826 |
| `field_0x94` after `0x90` + `field_0x90` simplified | 7072 | 46.8594 | 5,826 |

`field_0x90` simplification and the `0x94` move both improve the fuzzy ratio but
increase raw differing bytes (register allocation / vector-group changes move the
literal pool and the `add rX,pc` immediates).  No variant was found that improves
both the ratio and the raw byte count while staying at 7072 B; the `0x170` swap is
the only size-exact byte win.

## 4. Why the outer tree does not win on its own

1. Register allocation: the original keeps `iPadHD` in `sl`; a literal three-arm
   source makes clang put it in `lr`/`fp` and changes the prologue.
2. The non-HD common arm is ~2,400 B of branch-tree/vbsl code in the original; the
   flat ternaries emit ~1,500 B.  The missing code is not padding, it is the original
   tree's duplicated stores and vector selects.
3. The current flat-ternary body is a strong local optimum: it is size-exact and the
   compiler's own lowering of the ternaries already produces many of the original's
   `vst1` groups (first nine HD groups match, see w7730).

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w7751
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
python3 /dev/shm/w7751/score.py                          # 7072 / 45.028890 / 5788
```

Scratch harnesses (not committed, `/dev/shm/w7751/`): `score.py` (single-function
unrounded ratio + same-address raw-byte diff), `order.py` (base-register store-order
census), `gen3.py` (three-arm generator), `reorder2.py`/`reorder4.py` (first-group
and window reorders), `group.py` (make a vector group contiguous),
`move.py` (move one assignment after another), `Layout.base.cpp`.

## 6. Open questions

* The original HD arm's hoisted-vector schedule needs the original **source** order,
  not the store order.  The first nine `vst1` groups already match; the residual is
  the `0xb0/0xc0/0xec` window and the `0x140/0x150/0x160/0x170` vs
  `0x148/0x158/0x168` tail shift.
* The non-HD common arm's `vbsl` retina selection is still unmodelled.  A faithful
  transcription must keep the whole `0x7c..0x1e0` run inside one `if (retina)` /
  `else` block, not per-field ternaries; w7729's variant A showed this costs the
  `fp` register flip and a 64 B deficit on the tail.
* `field_0x90`'s `0.325f`/`0.65f` pair is the only literal-pool word out of place
  (w7730); the simplified expression improves the fuzzy ratio but not the raw bytes.
