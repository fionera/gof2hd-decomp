# `Layout::Layout` C1/C2 — wave 147 / worker w9307
## Window 0xe00 reconstructed instruction-for-instruction: it is the iOS-order
## `0x164..0x1a0` field block, but landing it needs the device-major tail; the
## ascending `0x180..0x1a0` reorder cuts window 14 from 250 -> 242 at +8 B (7080).
## No size-exact (7072) win; baseline kept.

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, size **7072 B**, source `src/game/ui/Layout.cpp`.  Baseline = wave-146
master `9c9744ca5` (target raw same-address diff **5704**, `.text` 85,928,
byte_exact 4507/4524, align_diff **2304** non-matching instructions).

## 1. What the original window 0xe00 actually is

`0xd1e28..0xd1f28` (function offset `0xe00..0xeff`) is the **`0x164..0x1a0`
field block of the retina/default arm**, in this store order:

```
0xe00  movs r4,#0            (tail of the 0x15c select)
0xe02  vmov s0,r2 ; orrs r1,r4,r9 ; vcvt ; vcvt
0xe12  vstr s0,[r0,#352]     0x160
0xe16  beq 0xe28
0xe18  movs r1,#0 ; cmp r5,#0
0xe1c  str  r1,[r0,#356]     0x164   (n9/iPadLarge arm)
0xe28  cbz r5,0xe4c
0xe2a  movs r1,#25 ; cmp r8 ; it eq ; moveq r1,#13
0xe34  str  r1,[r0,#356]     0x164   (retina arm)
0xe38  vmov s2,#57 ; vldr s0 ; cmp r8 ; it eq ; vmoveq
0xe4c  movs r1,#23 ; vldr s0
0xe52  str  r1,[r0,#356]     0x164   (default arm)
0xe56  vcvt ; cmp r9,#0
0xe5e  vstr s0,[r0,#360]     0x168
0xe64  addw r1,pc ; cmp r5 ; vld1 {d16-d17}
0xe72  str  r1,[r0,#380]     0x17c   (hoisted before the SLP group)
0xe7a  vst1 {d16-d17},[r0+0x16c]   0x16c,0x170,0x174,0x178
0xe80  movs r2,#34 ; cmp r8 ; it eq ; moveq r2,#17
0xe9a  str  r2,[r0,#384]     0x180
0xea2  str  r9,[r0,#388]     0x184
0xea8  str  r2,[r0,#392]     0x188
0xeb0  strd r2,fp,[r0,#396]  0x18c,0x190
0xebc  stmia r2!,{r1,r3,r5}  0x194,0x198,0x19c
0xec0  str  r1,[r0,#416]     0x1a0
0xec4  b 0x2064
```

i.e. the **iOS source order** of the block:
`164, 168, 16c, 170, 174, 178, 17c, 180, 184, 188, 18c, 190, 194, 198, 19c, 1a0`
(the `0x17c` scalar is scheduled *before* the `vst1` that stores `0x16c..0x178`,
exactly the clang SLP shape for four contiguous ascending stores followed by a
scalar).  The full disassembly is committed as
`LayoutC1-w9307-orig-win0e00.dis`.

## 2. What ours emits at the same address

Baseline `0xd1e28..0xd1f28` is the **`0x154/0x148/0x158/0x2bc/0x204/0x160/0x164`**
block.  The base source order for this region is the hill-climbed permutation

```
13c, 14c, 150, 154, 148, 158, 15c, 204, 2bc, 160,
164, 168, 16c, 178, 174, 170, 17c, 194, 184, 1a4,
[190 if/else], 188, 180, 19c, 18c, 1a0, 198, 1a8, 1ac
```

so `0x148` (a separate scalar, not part of the `0x140..0x14c` SLP group),
`0x204` and `0x2bc` are emitted *before* `0x164`, pushing the whole `0x164`
block ~0xe0 B later.  `0x164` is at function offset `0xed8`; the original has
it at `0xdf4`.  This confirms w9202's "global store-order shift, not a local
instruction-shape difference".

## 3. Variants tried (all real `link_so` + fdiff + align_diff + win.py)

| variant | source change | size | raw | align | win14 |
|---|---|---:|---:|---:|---:|
| HEAD baseline | — | **7072** | **5704** | **2304** | 250 |
| v13 (`candidate-7080`) | `0x180..0x1a0` ascending (iOS order) | 7080 | 5873 | 2421 | **242** |
| v10 | v13 + `0x1a8/0x1ac` ascending | 7080 | 5873 | 2421 | 242 |
| v2 (`candidate-6952`) | full iOS order `13c..1ac` + move `140/144/148` up + move `204/2bc` out | 6952 | **5572** | 2344 | 247 |
| v6 | `150,154,160,[iOS block],148,158,15c,204,2bc` | 7004 | 5929 | 2422 | 250 |
| v7 | `150,154,[iOS block],160,148,158,15c,204,2bc` | 7016 | 5956 | 2402 | 250 |
| v15 | v13 + `148` after `144` | 6976 | 5835 | 2499 | 243 |
| v16 | v13 + `148` after `13c` | 7024 | 5842 | 2400 | 250 |

Key negative results:
* Reordering **only** `0x16c/0x170/0x174/0x178` (swap `0x178`/`0x170`) produces
  *identical* code to HEAD (the backend already schedules it that way).
* The window-14 improvement comes **only** from making `0x180..0x1a0`
  ascending (v13/v10), and it costs **+8 B** (7080).  Every attempt to pay the
  8 B back by moving `0x148` up (`v15`, `v16`, v2) also moves `0x164` away from
  `0xdf4` and loses the window win.
* Full iOS order (v2) gives the best raw diff (5572) but is **120 B short**
  (6952) and worse on align_diff (2344 vs 2304): it vectorises the
  `0x140..0x14c` and `0x16c..0x178` groups and drops the duplicated
  device-major code the original carries.  This is the same "shared tail"
  blocker as waves 143/145/146.

## 4. Window table before / after

Baseline (HEAD, 7072 / raw 5704):

| win | diff | win | diff | win | diff | win | diff |
|---:|---:|---:|---:|---:|---:|---:|---:|
| 0 | 211 | 7 | 178 | 14 | **250** | 21 | 240 |
| 1 | 213 | 8 | 238 | 15 | 243 | 22 | 233 |
| 2 | 236 | 9 | 240 | 16 | 242 | 23 | 240 |
| 3 |  74 | 10 | 231 | 17 | 243 | 24 | 212 |
| 4 |  99 | 11 | 231 | 18 | 244 | 25 | 75 |
| 5 | 203 | 12 | 242 | 19 | 242 | 26 | 71 |
| 6 | 230 | 13 | 230 | 20 | 242 | 27 | 71 |

v13 (`0x180..0x1a0` ascending, 7080 / raw 5873):

| win | diff | win | diff | win | diff | win | diff |
|---:|---:|---:|---:|---:|---:|---:|---:|
| 0 | 211 | 7 | 234 | 14 | **242** | 21 | 245 |
| 1 | 213 | 8 | 242 | 15 | 246 | 22 | 246 |
| 2 | 235 | 9 | 249 | 16 | 254 | 23 | 245 |
| 3 |  74 | 10 | 209 | 17 | 248 | 24 | 220 |
| 4 |  99 | 11 | 250 | 18 | 255 | 25 | 90 |
| 5 | 203 | 12 | 246 | 19 | 248 | 26 | 78 |
| 6 | 238 | 13 | 233 | 20 | 252 | 27 | 68 |

So the window-14 fix is real but not free: it buys 8 bytes in window 14 and
loses more in windows 6/8/9/16/18/20/22/23 (net raw 5704 -> 5873).

## 5. Why it cannot land at 7072 without the device-major tail

The original's `0xe00` block is emitted inside the **retina/default arm of a
device-major tail** (`d1860..d21e2`), where `iPadHD` is hoisted out and the
`n9/iPadLarge/retina/iPad` sub-arms are duplicated.  Our flat chain of
ternaries if-converts into a single shared sequence, so the `0x164` block is
reached at a different offset and the three `0x164` variants are laid out
differently (`0xdf4/0xe0c/0xe2a` vs `0xed8/0xef0/0xf00`).  Matching the window
*and* keeping 7072 requires the duplicated tail (original literal pool 221
words vs our 193), which is the open item from waves 143/145/146.

## 6. Not landed

`src/game/ui/Layout.cpp` is unchanged: **7072 / raw 5704 / align 2304**,
gate green.  Candidates committed:
* `LayoutC1-w9307-candidate-7080.cpp` — v13, 7080 B, raw 5873, win14 242.
* `LayoutC1-w9307-candidate-6952.cpp` — v2, 6952 B, raw 5572, win14 247.
* `LayoutC1-w9307-win13.txt`, `LayoutC1-w9307-orig-win0e00.dis`.

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w9307
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9307/fdiff.py _ZN6LayoutC1Ev   # 7072/7072, diff 5704
python3 /dev/shm/w9307/win.py _ZN6LayoutC1Ev 256 # per-window table
```

Scratch (`/dev/shm/w9307/`): `fdiff.py`, `win.py`, `align_diff.py`,
`storetrace2.py`, `evalvar.py`, `Layout.base.cpp`, `v2.cpp`, `v10.cpp`,
`v13.cpp`, `v15.cpp`, `v16.cpp`, `orig_e00.txt`, `v7_win.txt`.

## 8. Open questions

* Reconstruct the device-major tail so the `0x164..0x1a0` iOS-order block can be
  emitted at `0xdf4` while the function stays 7072 B.  The `+8 B` that the
  `0x180..0x1a0` ascending group costs is the exact amount the duplicated tail
  must supply.
* `0x148` must join the `0x140..0x14c` SLP group (it is a separate scalar in
  HEAD); that is worth ~104 B, so it has to be paired with the tail
  reconstruction, not tried alone.
