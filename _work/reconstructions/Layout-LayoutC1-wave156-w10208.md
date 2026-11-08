# `Layout::Layout` C1/C2 — wave 156 / worker w10208
## SLP chunk-boundary fix for the `0xb0`/`0xc0` group bases.
## Landed: 7,072 B (size exact), raw **5,837 -> 5,792** (-45),
## aligned **1,986 -> 2,010** (+24).  Gate green.

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original
vaddr `0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Entry = wave-155 HEAD `cf74f4ee6`: raw same-address diff **5,837**,
aligned instruction diff **1,986**, size 7,072.
Exit: raw **5,792**, aligned **2,010**, size 7,072.
Gate on exit: byte_exact 4507/4524, linked_exact 4507/4524, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, `RATCHET PASS`.

## 1. Result: `0xb0`/`0xc0` group bases now match the original

Original `vst1.32` group bases (iPadHD arm), function-relative:

```
ORIG  0x14 0x4 0x24 0x34 0x44 0x54 0x64 0x2c8 0x7c 0xc0 0xb0 0xec
      0x294 0x2a4 0x108 0x118 0x128 0x140 0x150 0x160 0x170 0x180
      0x190 0x1a0 0x1b0 0x1c0 0x1d0
OURS  0x14 0x4 0x24 0x34 0x44 0x54 0x64 0x2c8 0x7c 0xb0 0xc0 0x294
      0x2a4 0xdc 0x118 0x108 0x128 0x148 0x158 0x168 0x180 0x190
      0x1a0 0x1b0 0x1c0 0x1d0
```

The `0xb0`/`0xc0` pair is now at the original's bases (emission order
`b0,c0` vs the original's `c0,b0` remains).  `0xec`, `0x140` and `0x204`
are still not fixed (see §5).

## 2. Why the group was one field late (decisive)

`SLPVectorizer::vectorizeStoreChains` splits the BB's store list into
**chunks of 16 stores in IR order**.  In the wave-155 build the pre-SLP
store order around the group was:

```
index 47: 0xb0      <- last store of chunk 2
index 48: 0xa0      <- chunk 3 starts here
...
index 52: 0xb4
index 51: 0xb8
index 57: 0xbc
index 63: 0xc0
index 60: 0xc4
index 61: 0xc8
index 59: 0xcc
```

`0xb0` sits just *before* the 16-store chunk boundary, so `vectorizeStores`
never pairs `0xb0 -> 0xb4`; the chain starts at `0xb4` and the two groups
become `b4..c0` / `c4..d0`.  The original's `b0` is inside the chunk, so
its chain is `b0..bc` / `c0..cc`.

To move `b0` into chunk 3 **and keep `c0` in chunk 3**, one non-chain
store must leave the chunk.  The source shape that does this:

* move `field_0xb0` from its old position (after `field_0xa8`) down to
  just after `oreTextYOffset`, i.e. into the `0xb0..0xc0` region;
* move `marqueeWidth` (`0xd4`) out of that region to after `field_0xc0`.

`marqueeWidth` is the evicted non-chain store.  `b0` then lands in the
same 16-store chunk as `b4/b8/bc/c0/c4/c8/cc`; SLP builds the 8-store
chain `b0,b4,b8,bc,c0,c4,c8,cc` and emits the original's two groups.

## 3. The two companion moves (aligned recovery)

A 4,900-variant single-statement move sweep over the first 70 constructor
statements (compile + mini-gold-link + `align_diff`) was run twice on the
resulting base.  Two pure statement moves were landed:

1. `field_0x50` before `field_0x54` (swap the two adjacent assignments).
2. `field_0xc4` moved before `ringNearMidThreshold` (i.e. to the head of
   the `0xc4..0xcc` run).

No expression, constant or type was edited.  Together they recover part of
the aligned cost of the group fix: raw 5,825 -> 5,792, aligned 2,067 ->
2,010 on the group-fixed base.

## 4. Measurements (real `link_so` + fdiff + align_diff)

| variant | fn size | raw | aligned |
|---|---:|---:|---:|
| wave-155 HEAD | **7,072** | **5,837** | **1,986** |
| `b0` moved + `marqueeWidth` evicted | 7,072 | 5,825 | 2,067 |
| + `0x50` before `0x54` | 7,072 | 5,792 | 2,010 |
| + `0xc4` before `ringNearMidThreshold` | 7,072 | 5,805 | 2,016 |

The landed shape is `b0`-move + `marqueeWidth` evict + `0x50`/`0x54`
swap.  The `0xc4` move is a near-tie (raw +13, aligned +6) and was not
landed.

## 5. Remaining groups (open questions)

* **`0xec`**: chunk 4 contains the four floats `0xdc..0xe8` and the ints
  `0xf0/0xf4/0xf8`, but `0xec` is still in chunk 3, so ours vectorises
  the floats at `0xdc` and leaves the ints as `strd`.  Moving `0xec`
  next to `0xf0` gives the original `0xec` group but size grows to
  7,116 B and the `0x294/0x2a4` groups shift to `0x298`.
* **`0x140..0x170`**: moving `0x140/0x144` next to `0x13c` and evicting
  `0x204/0x2bc` gives the original's 10-group `0x140..0x1d0` run, but
  size drops to 7,008-7,040 B (the group fix removes code without adding
  pool words) and aligned rises to 2,204-2,225.
* **`0x204`**: the shared tail's chain starts at `0x208`; the original
  starts at `0x204`.  Same one-store chunk-boundary problem as `0xb0`,
  but in the post-merge tail BB.
* The 124 B code/pool split (original 221 pool words / 6,188 B code vs
  ours 190 / 6,312 at wave 155) is unchanged; the group fix does not
  create the original's extra pool words.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10208
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10208/fdiff.py _ZN6LayoutC1Ev        # 7072/7072, diff 5792
python3 /dev/shm/w10208/align_diff.py _ZN6LayoutC1Ev   # 2010
cmake --build cmake-build-match --target check -j6     # RATCHET PASS
```

Scratch `/dev/shm/w10208/`: `Layout.base.cpp` (wave-155 source),
`Layout.m19_21.cpp` (landed source), `groups.py` (vst1 base table),
`split.py` (statement splitter), `sweep.py` (compile + mini-link + align
sweep), `sweep70*.log` (three 4,900-variant rounds), `fdiff.py`,
`align_diff.py`, `align_mini.py`.
