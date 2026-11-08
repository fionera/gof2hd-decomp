# `Layout::Layout` C1/C2 — wave 161 / worker w10707
## Adjacent-swap re-sweep on the wave-160 base: two size-exact raw wins
## (`field_0x34`/`field_0x30_rowHeight`, `field_0x19c`/`field_0x18c`).
## Raw **5,733 -> 5,722** (-11), aligned **1,978 -> 2,015** (+37),
## size **7,072 B** (exact).  Gate green.

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original
vaddr `0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Baseline = wave-160 HEAD `01c7f92d6`: raw same-address diff **5,733**,
aligned instruction diff **1,978**, size 7,072.
Exit: raw **5,722**, aligned **2,015**, size 7,072.
Gate on exit: byte_exact 4507/4524, linked_exact 4507/4524, size
4524/4524, data 843/843, imports 207, lint CLEAN, `RATCHET PASS`.

## 1. The String/Sprite hint does not apply

The w10607 float-literal note already established that this TU has **no
`bl`/`blx` at all** (`grep -c` on the original disassembly = 0): there is
no `String` copy-constructor call site, no `Sprite`/`Image` load and no
`ImageFactory`/`SpriteSystem` call to reorder.  The whole body is
constant stores.  The only remaining levers are store order, SLP
grouping and the literal pool.

## 2. Fresh exhaustive adjacent-statement sweep (204 pairs)

The 205 top-level constructor statements were split with the w10507
mini-parser (if/else chains are single units), each adjacent pair was
swapped, compiled with NDK r18b clang 7.0.2 `-Oz`, gold-linked, stripped
and measured with both `fdiff` and `align_diff.py`.  Size was read from
the `ours` field of `fdiff`.

Size-exact (7,072) raw improvements found (none improves aligned too):

| swap | raw | aligned |
|---|---:|---:|
| baseline | 5,733 | 1,978 |
| `field_0x34` / `field_0x30_rowHeight` (11/12) | 5,728 | 2,011 |
| `field_0x19c` / `field_0x18c` (118/119) | 5,727 | 1,982 |
| `field_0x2ac_metricC` / `ringMidFarThreshold` (76/77) | 5,731 | 1,994 |
| `field_0x1d4` / `field_0x1c0` (136/137) | 5,729 | 2,004 |
| `field_0x2fc` / `field_0x300` (194/195) | 5,732 | 1,991 |
| `field_0x30c` / `field_0x310` (199/200) | 5,732 | 1,981 |
| `field_0x1c8` / `field_0x1d8` (130/131) | 5,743 | **1,975** |

Compositions (all size 7,072):

| composition | raw | aligned |
|---|---:|---:|
| 118/119 | 5,727 | 1,982 |
| 118/119 + 11/12 | **5,722** | 2,015 |
| 118/119 + 11/12 + 199/200 | 5,744 | 1,978 |

The landed shape is the **118/119 + 11/12** composition: it is the
lowest raw diff measured on this base.  The aligned heuristic rises
because the two swaps move two 8-byte store groups across block
boundaries; raw same-address bytes (the ground truth for byte
identicality) fall by 11.

## 3. 0x140 / 0x94 group findings (open question, measured)

The original's iPadHD block groups `0x140/0x144` (`vst1.32 {d16-d17}` at
`d11f6`) and `0x94/0x98/0x9c/0xa0` (`stmia.w r4,{r5,r8,ip,lr}` at
`d113e`).  Our build emits `0x94` and `0x144` in the *tail* block
(`d25b8`, `d25be`) because their ternaries test only
`iPadHD/n9/retina` (3-way), while the fields the compiler hoists into
the iPadHD block also test `iPad/iPadLarge` (5-way).  Tested this pass:

* moving `field_0x94` before `field_0x98` (making `0x94..0xa0`
  contiguous): size **7,048**, raw 5,737, aligned 2,117.  No `stmia`
  appears (four separate `str.w`).
* making `0x94/0x98/0x9c/0xa0` explicit 5-way ternaries (adding
  `iPad`/`iPadLarge` arms equal to the default): size **7,128**, raw
  5,920, aligned 2,865.
* moving `field_0x140`+`field_0x144` after each of 41 anchors (both
  orders): no size-exact variant; best raw 5,765 at 7,024 B.
* `field_0x140`+`field_0x144` after `field_0x13c`: size 7,032, raw
  5,845, aligned 2,115.

The original's code section is ~124 B smaller than ours but its literal
pool is ~124 B larger (211 vs 180 words), i.e. the missing SLP groups
trade code bytes for pool words at net zero.  Reproducing the exact
grouping therefore needs the original's *source order* for the whole
`0x04..0x1e0` region, not a local move.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10707
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10707/fdiff.py _ZN6LayoutC1Ev        # 7072/7072, diff 5722
python3 /dev/shm/w10707/align_diff.py _ZN6LayoutC1Ev   # 2015
cmake --build cmake-build-match --target check -j6     # RATCHET PASS
```

Scratch `/dev/shm/w10707/`: `fdiff.py`, `align_diff.py`, `measure.sh`,
`split.py`, `harness.py`, `sweep_adj.py`, `combo.py`, `combo2.py`,
`Layout.base.cpp`, `adj_results.txt`.
