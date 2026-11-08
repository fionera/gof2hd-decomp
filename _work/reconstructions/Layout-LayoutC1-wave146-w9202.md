# `Layout::Layout` C1/C2 — wave 146 / worker w9202
## 256-byte-window diff sweep: the worst window (0xe00) is a store-order shift, not a local
## difference; no size-neutral window reconstruction found. Best candidate remains
## 7068 B / raw 5675 (4 B short, size-negative). Baseline 7072 / 5704 kept.

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr `0xd1028`,
size **7072 B**, source `src/game/ui/Layout.cpp`.  Baseline = wave-145 master `31297fadc`
(target raw same-address diff **5704**, `.text` 85,651).

## 1. Per-256-byte-window raw same-address diff (before)

`python3 /dev/shm/w9202/win.py 256` (raw byte mismatch at the same function offset):

| win | start | diff | win | start | diff | win | start | diff |
|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| 0 | 0x000 | 211 | 10 | 0xa00 | 231 | 20 | 0x1400 | 242 |
| 1 | 0x100 | 213 | 11 | 0xb00 | 231 | 21 | 0x1500 | 240 |
| 2 | 0x200 | 236 | 12 | 0xc00 | 242 | 22 | 0x1600 | 233 |
| 3 | 0x300 |  74 | 13 | 0xd00 | 230 | 23 | 0x1700 | 240 |
| 4 | 0x400 |  99 | **14** | **0xe00** | **250** | 24 | 0x1800 | 212 |
| 5 | 0x500 | 203 | 15 | 0xf00 | 243 | 25 | 0x1900 |  75 |
| 6 | 0x600 | 230 | 16 | 0x1000 | 242 | 26 | 0x1a00 |  71 |
| 7 | 0x700 | 178 | 17 | 0x1100 | 243 | 27 | 0x1b00 |  71 |
| 8 | 0x800 | 238 | 18 | 0x1200 | 244 | | | |
| 9 | 0x900 | 240 | 19 | 0x1300 | 242 | | | |

Worst window = **14 (0xe00..0xeff, vaddr `0xd1e28..0xd1f28`), 250/256 bytes differ**.

## 2. What the worst window contains

Original `0xd1e28..0xd1f28` is the field block **0x164, 0x168, 0x16c..0x178 (SLP
vector), 0x17c, 0x180, 0x184, 0x188, 0x18c, 0x194, 0x1a0** (ascending store order, with
the `0x16c..0x178` group emitted as `vld1/vst1`).  Ours at the same addresses is the
**0x154, 0x148, 0x158, 0x2bc, 0x204, 0x160, 0x164** block: our stream is ~0xe0 bytes
behind because the `0x108..0x12c` block and `0x148/0x2bc/0x204` are emitted later in
our source order.  So the window mismatch is a **global store-order shift**, not a
local instruction-shape difference; matching the window requires moving ~0xe0 bytes of
preceding code, which changes every later window.

## 3. Local reconstructions tried (all measured with `link_so` + fdiff)

| variant | size | raw diff | note |
|---|---:|---:|---|
| HEAD (wave-145) | **7072** | **5704** | baseline |
| group `0x13c,0x140,0x144,0x148` contiguous (ascending) | 7000 | 5852 | reproduces the original `vst1 {0x13c..0x148}` at `d1d26`, but SLP shrinks the fn by 72 B |
| `0x160..0x1a8` ascending (original window order) | 7080 | 5873 | +8 B, worse |
| wave-145 candidate (`LayoutC1-w9008-candidate.cpp`) | 7068 | 5675 | best raw, 4 B short |

The `0x13c..0x148` grouping is the *right* source shape (the original does vector-store
those four), but the ~72 B it saves is exactly the "duplicated shared-tail" size the
original carries elsewhere; without reconstructing that tail the function cannot stay
7072 B.

## 4. The 4-byte gap, re-measured

* Candidate code+padding = `0xd28c0 - 0xd1028` = **6296 B**; literal pool = **772 B**
  (193 words) -> 7068.
* Original code+padding = `0xd2854 - 0xd1028` = **6188 B**; literal pool = **884 B**
  (221 words) -> 7072.
* The candidate's code is 108 B *longer* and its pool 112 B *shorter*; the net is one
  pool word (4 B).  The original's extra pool words are the retina/default select
  constants (`0.325, 0.65, 100, 50, 281.25, 400, ...`) that only its duplicated arms
  reference.
* Padding experiments on the candidate: a volatile byte read of `this->field_0x285` at
  the end adds a 4-byte `ldrb.w` but removes the 4-byte pool-alignment `nop` pair and
  one pool word -> stays 7068 (raw 5678).  Two such reads -> 7076.  A volatile float
  read of `this->field_0x8c` -> 7084.  A volatile byte write -> 7076.  No 4-byte
  addition lands on 7072 (the pool start is 8-aligned; +4 code consumes the padding,
  +8 code re-pads).

## 5. Candidate per-window table (after, for the coordinator)

`LayoutC1-w9008-candidate.cpp` (7068 B, raw 5675) vs baseline per window:

| win | base | cand | win | base | cand | win | base | cand |
|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| 0 | 211 | 208 | 10 | 231 | 243 | 20 | 242 | 252 |
| 1 | 213 | 222 | 11 | 231 | 250 | 21 | 240 | 240 |
| 2 | 236 | 203 | 12 | 242 | 216 | 22 | 233 | 248 |
| 3 |  74 |  78 | 13 | 230 | 238 | 23 | 240 | 184 |
| 4 |  99 |  95 | 14 | 250 | 242 | 24 | 212 |  86 |
| 5 | 203 | 199 | 15 | 243 | 255 | 25 |  75 |  74 |
| 6 | 230 | 232 | 16 | 242 | 248 | 26 |  71 |  71 |
| 7 | 178 | 245 | 17 | 243 | 206 | 27 |  71 |  71 |
| 8 | 238 | 186 | 18 | 244 | 248 | | | |
| 9 | 240 | 222 | 19 | 242 | 244 | | | |

The candidate is a different permutation, not a window-local fix: it wins windows
2/3/4/7/12/13/17/23/24 and loses 1/5/6/10/11/15/16/18/20/22.  Merging window-local
improvements from the two permutations is the open next step (the wave-141/142 merge
lesson applies: test both orders and state the exact raw diff).

## 6. Not landed

`src/game/ui/Layout.cpp` is unchanged (7072 / 5704).  The candidate is committed as
`_work/reconstructions/LayoutC1-w9008-candidate.cpp` (wave 145) and re-verified here:
7068 B, raw 5675, 5x `clz`, 10x `vbsl` (original 11).  A size-neutral landing needs the
duplicated device-major shared tail reconstructed (original pool 221 words vs 193).

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w9202
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9202/fdiff.py _ZN6LayoutC1Ev   # 7072/7072, diff 5704
python3 /dev/shm/w9202/win.py 256                # per-window table
```

Scratch (`/dev/shm/w9202/`): `fdiff.py`, `win.py`, `align.py`, `storetrace.py`,
`climb.py` (single-move random climb, 488 iters, no improvement from 5704),
`Layout.base.cpp`, `Layout.cur.cpp`, `win_base.txt`, `win_cand.txt`.

## 8. Open questions

* Merge the candidate's window wins (2/3/4/7/12/13/17/23/24) with the base's wins
  (1/5/6/10/11/15/16/18/20/22) by a constrained move climb that keeps size 7072.
* Reconstruct the duplicated device-major shared tail so the `0x13c..0x148` and
  `0x16c..0x178` SLP groups can be made contiguous without shrinking below 7072.
