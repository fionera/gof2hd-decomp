# `Layout::Layout` C1/C2 — wave 159 / worker w10508
## SLP group-base hunt: `0x140` group is not reachable by any statement
## reorder or per-branch assignment that keeps size 7,072 and aligned <= 1,978.
## Source unchanged; baseline re-measured raw **5,739**, aligned **1,978**, 7,072 B.

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original
vaddr `0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Baseline = wave-158 HEAD `cfde1f5f2`: raw same-address diff **5,739**,
aligned instruction diff **1,978**, size 7,072.
Exit: raw **5,739**, aligned **1,978**, size 7,072 (no source change).
Gate on exit: byte_exact 4507/4524, linked_exact 4507/4524, size
4524/4524, data 843/843, imports 207, lint CLEAN, `RATCHET PASS`.

## 1. Current group bases (measured, `vst1.32` base offsets)

```
ORIG  14 4 24 34 44 54 64 2c8 7c c0 b0 ec 294 2a4 108 118 128 140 150 160 170 180 190 1a0 1b0 1c0 1d0
OURS  14 4 24 34 44 54 64 2c8 7c b0 c0 294 2a4 dc 118 108 128 150 160 170 180 190 1a0 1b0 1c0 1d0
```

The current wave-158 base already has the original's `0xb0/0xc0` pair and the
whole `0x150..0x1d0` run.  The remaining differences are (a) the `c0/b0`
emission order, (b) the `0x108/0x118` order, (c) the missing `0x140` group
(ours jumps `128 -> 150`), and (d) the missing `0xec` group (ours groups the
four floats at `0xdc`).

## 2. New hypotheses tested (all real `link_so` + fdiff + align_diff)

1. **Move `0x140/0x144` alone** to every one of 61 insertion points
   (after lines 115..175).  Only `after=142` is size-exact (7,072), but it
   still groups `128,13c,150,...` and regresses to raw **5,884** /
   aligned **2,186**.  No insertion point produces a `0x140` group.
2. **All 24 permutations** of the four statements `0x140/0x144/0x148/0x14c`
   placed before `0x150`, with `0x13c` moved after `0x15c`.  The compiler
   always starts the group at `0x144`, `0x148` or `0x14c`, never `0x140`;
   sizes 0x1b2c..0x1b54 (not 7,072).
3. **Per-branch assignment** of `0x140/0x144` (replace the nested ternary
   with an explicit `if/else if/else` chain).  This is *byte-identical* to
   the baseline: raw 5,739, aligned 1,978, size 7,072.  So the missing
   `0x140` group is not caused by the ternary shape.
4. **Move the `0x130/0x134/0x138` if/else chain** away from `0x140`
   (before `0x128`, after `0x15c`, after `0x1d0`, after `0x1dc`).  Moving
   it after `0x15c` does produce a `0x140` group, but breaks the run at
   `0x160` (`140,150,130,160,...`) and shrinks the function to 6,952 B
   (raw 5,736 / aligned 2,191).  Moving it later keeps `140,150,160,170,180`
   but loses `0x190..0x1d0` (`1a8,1cc`) and shrinks to 6,972 B
   (raw 5,854 / aligned 2,830).
5. **Full memory-order sort** of the `0x128..0x1dc` region (statements
   sorted by field offset, if/else chains at their offset).  This yields
   `138,128,148,158,...` at 5,240 B: the `0x138` store joins the run and
   shifts every base by 8.
6. **`0xec` group**: moving `ringNearMidThreshold` next to `field_0xf0`
   does create the `0xec` group, but shifts `0x294 -> 0x298` and changes
   size to 0x1bac (7,084, raw 5,647 / aligned 2,053) or 0x1b9c
   (7,060, raw 5,668 / aligned 2,079).  No position gives 7,072.
7. **`c0/b0` order**: moving `field_0xb0` after `field_0xc0` does give the
   original emission order `c0,b0`, but size drops to 7,064 and both
   metrics regress (raw 5,821 / aligned 2,071).

## 3. Conclusion / open question

The `0x140` group is blocked by the compiler's decision to combine
`0x138/0x13c/0x140` into one `stmia` (3-register store) whenever the
`0x130/0x134/0x138` chain is in the same block.  Moving the chain away
creates `0x140` but changes the block scheduling enough to lose the
`0x190..0x1d0` groups and ~100 B.  A source shape that both moves the chain
and re-creates the `0x190..0x1d0` run at exactly 7,072 B was not found in
this pass.  The `0xec` group trades the correct `0x294` base for `0x298`
and costs ~75 aligned instructions.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10508
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10508/fdiff.py _ZN6LayoutC1Ev        # 7072/7072, diff 5739
python3 /dev/shm/w10508/align_diff.py _ZN6LayoutC1Ev   # 1978
cmake --build cmake-build-match --target check -j6     # RATCHET PASS
```

Scratch `/dev/shm/w10508/`: `compile.sh` (single-TU compile),
`try.py`/`reorder2.py` (statement movers), `groups_obj.py` (SLP group-base
extractor), `measure.sh` (link + fdiff + align_diff), `fdiff.py`,
`align_diff.py`, `base.cpp` (wave-158 source), `branch1.cpp`
(per-branch `0x140/0x144`), `e2_93.cpp`/`e2_96.cpp` (`0xec` variants),
`big1.cpp`/`big2.cpp` (memory-order sort).
