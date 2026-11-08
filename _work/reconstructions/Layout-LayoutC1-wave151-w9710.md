# `Layout::Layout` C1/C2 — wave 151 / worker w9710
## Device-major tail: the original is a full device-major constructor
## (iPadHD arm / n9 arm / shared retina arm), not a flat ternary chain.
## The ascending tail reproduces `0x164 @ 0xdf4` exactly but is 184 B short;
## explicit source if/else groups only recover ~80 B.  Source unchanged.

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Baseline (wave-150 HEAD `db9658a51`): raw same-address diff **5,741**,
aligned instruction diff **2,189**, size 7,072.  Gate on entry and exit:
byte_exact 4507/4524, linked_exact 4507/4524, size 4524/4524, data 843/843,
imports 207, lint CLEAN, `RATCHET PASS`.

## 1. New result: the original's control-flow skeleton

Manual Thumb disassembly of the original (objdump; Ghidra provider was
unavailable) shows the constructor is **device-major**:

* `0xd103e`: `beq.w 0xd126c` if `Globals::iPadHD == 0` — the whole first arm
  (`0xd1042..0xd126a`, ~552 B) sets fields `0x04..0x1e0` with iPadHD constants
  and then `b.w 0xd21e2` (its own tail copy at function offset `0x11ba`).
* `0xd126c`: loads `Globals::n9`; `beq.w 0xd1570` if zero.  The n9 arm
  (`0xd127e..0xd156e`) sets the same fields with n9 constants and then
  `b.n 0xd1860` (function offset `0x838`, the shared tail).
* `0xd1570`: shared retina/default arm; it keeps ternaries on
  `retinaDisplay`/`iPadLarge`/`iPad` and falls through to `0xd1860`.
* `0xd1860..0xd2800` is the shared tail (contains `0x164 @ 0xdf4`, the
  `0x2c4..0x2d8` group at `0xa9e`, the `0x7c..0x94` group at `0xb62`, and the
  `0x2f4..0x318` group at `0x1642..0x1750`).
* `0xd2802..0xd2850` are four out-of-line cold blocks (iPad/default
  `0x130..0x138`, `0x2c4`, `0x74`), then the final pool `0xd2854..0xd2bc8`
  (**884 B = 221 words**, not 242; the 242 figure includes the mid-function
  pool at `0xd1d7c..0xd1d9a` and the other literal words).

GOT mapping recovered from the in-function literals (PC = insn+4, *not*
4-aligned): `0xd14dc -> Globals::n9`, `0xd18d4 -> retinaDisplay`,
`0xd1d7c -> iPadLarge`, `0xd1300 -> iPadHD`, `0xd18e8 -> iPad`.

## 2. Ascending tail — reproduces `0x164 @ 0xdf4`

Reordering the tail statements `0x13c..0x1e8` into ascending field order
(moving `0x140`/`0x144` up from their late source positions and
`eventBannerScale`/`0x1e4`/`0x1e8` after `0x1dc`), and moving
`field_0x204`/`field_0x2bc` after `field_0x2c0`, gives the exact original
store sequence in the `0xce0..0xf00` window:

```
... 0x130@d26 0x130@d44 0x138@d82 0x140@d86 0x148@d8c 0x150@da8
0x154@dcc 0x158@dd0 0x15c@dd4 0x160@de8 0x164@df4 0x164@e0c
0x154@e1e 0x15c@e26 0x164@e36 0x168@e54 0x16c@e84 ...
```

which matches w9503's ASC table exactly (first `0x164` store at **`0xdf4`**).
Measured: **6,844 B / raw 5,684 / aligned 2,559**.  The raw metric improves
by 57 bytes, but the function is **228 B short** of 7,072 (pool 184 words vs
the original's 221).

## 3. Device-major groups — all measured

| variant | size | raw | aligned | `0x164` @ | note |
|---|---:|---:|---:|---:|---|
| HEAD baseline | **7,072** | **5,741** | **2,189** | `ed8` | landed |
| ASC (ascending tail) | 6,844 | 5,684 | 2,559 | **`df4`** | structurally right |
| ASC + explicit `0x2f4..0x308` device if/else | 6,888 | 5,708 | 2,553 | `df4` | +44 B |
| ASC + explicit `0x2d0..0x2d8` 6-arm if/else | 6,912 | 5,853 | 2,872 | `dc4` | +24 B, shifts `0x164` |
| ASC + explicit `0x2c4` 6-arm if/else | 6,912 | 5,839 | 2,823 | `dc4` | if-converted, no size |
| ASC + explicit `0x30c..0x318` 5-arm if/else | 6,924 | 5,804 | 2,879 | `dc4` | if-converted |
| whole body duplicated `if(iPadHD)/else if(n9)/else` | 6,756 | 5,891 | 3,980 | — | 316 B short |

Key negatives:
* clang 7 `-Oz` if-converts every small explicit `if/else` group back into
  `it`/`moveq` selects, so source-level device duplication does **not**
  reproduce the original's duplicated constant vectors; it adds only
  24–44 B per group.
* Duplicating the whole body three times (`if (iPadHD) {body} else if (n9)
  {body} else {body}`) folds to 6,756 B and *shrinks* the pool to 130 words,
  because the compiler shares the constant vectors across arms.
* The original's pool words that ours lacks are mostly **GOT-relative
  pointers** (`0x0013df2e`, `0x0013de3c`, `0x0013ddb4`, `0x0013dd96`,
  `0x0013dc9a`, `0x0013dc04`, `0x0013db84`) — i.e. the original reloads the
  device globals once per arm, while our flat ternary chain loads each GOT
  slot once and reuses it.  No source-level construct tried in this wave
  makes clang 7 reload them.

## 4. Conclusion / next lever

The ascending tail is the correct structural skeleton (`0x164 @ 0xdf4`,
`0x140..0x14c` SLP group, `0x138`/`0x13c` pair) but the original's 221-word
pool is only reachable by reproducing the per-arm GOT reloads of the
iPadHD/n9/shared arms, i.e. a **full device-major rewrite of the whole
constructor**, not just the tail.  The 184 B gap is 148 B pool + 36 B code.
No size-exact (7,072 B) variant beat the baseline in this wave; the source is
unchanged.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w9710
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9710/fdiff.py _ZN6LayoutC1Ev        # 7072/7072, diff 5741
python3 /dev/shm/w9710/align_diff.py _ZN6LayoutC1Ev   # 2189
cmake --build cmake-build-match --target check -j6    # RATCHET PASS
```

Scratch `/dev/shm/w9710/`: `Layout.base.cpp` (baseline), `parse.py`,
`stores.py`, `poolwords.py`, `got.py`, `fdiff.py`, `align_diff.py`,
`orig.dis`, `asc2.dis`, `v4.dis`, `v6.dis`, `orig.pool`, `ours.pool`.
