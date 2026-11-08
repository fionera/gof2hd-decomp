# `Layout::Layout` C1/C2 — wave 154 / worker w10009
## SLP group-base placement: the levers are (a) the **16-store chunk
## boundary** in `vectorizeStores` and (b) the **store order in the IR**.
## Fixing the group bases shrinks the code by ~120 B (exactly the 124 B gap)
## but the pool does not grow, so no group-fix variant is 7,072 B.  Source
## unchanged (baseline 7,072 B / raw 5,830 / aligned 2,119 kept).

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original
vaddr `0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Baseline = wave-153 HEAD `4417db0b4`: raw same-address diff **5,830**,
aligned instruction diff **2,119**, size 7,072.  Gate on entry and exit:
byte_exact 4507/4524, linked_exact 4507/4524, size 4524/4524, data 843/843,
imports 207, lint CLEAN, `RATCHET PASS`.

## 1. How clang-7 SLP actually groups these stores (new, decisive)

Read `llvmorg-7.0.2/lib/Transforms/Vectorize/SLPVectorizer.cpp`
(`vectorizeStoreChains` / `vectorizeStores` / `vectorizeStoreChain`):

* `collectSeedInstructions` puts every simple store of a BB into one list in
  **IR order** (per underlying object).  The iPadHD arm of the constructor
  has **136** stores in one BB.
* `vectorizeStoreChains` splits that list into **chunks of 16 stores** and
  calls `vectorizeStores` per chunk.
* `vectorizeStores` does the quadratic pairing **in reverse order**; for each
  store `Idx` it searches `Idx-1, Idx+1, Idx-2, Idx+2, ...` for the first
  store `K` with `addr(K)+4 == addr(Idx)` and records `K -> Idx`.
* `vectorizeStoreChain` then walks each chain and vectorises **4 stores at a
  time starting at the chain head** (`i += VF-1` after each success).

So the group base is decided by **where the 16-store chunk boundary falls**
and by the **IR store order**, not by the field type.  The IR already folds
`3*1.40625f` to `i32 4` before SLP; `(int)` casts and plain int literals do
**not** change the grouping (tested).

## 2. Pre-SLP IR store order, iPadHD arm (baseline)

`python3 -mllvm -print-after-all` + `Demanded bits` dump, BB `%703`:

```
0xb0,0xa0,0xd4,0xd0,0xd8,0xb8,0xb4,0xa4,0xfc,0x104,0xbc,0xec,0xcc,0xc4,0xc0,0xc8,...
```

`0xb0` is at position 0 and `0xb4,0xb8,0xbc,0xc0` are at 6,7,11,15, so the
chain that SLP finds starts at `0xb4` and runs `0xb4,0xb8,0xbc,0xc0`; the
next chunk restarts the chain at `0xc4` and runs `0xc4,0xc8,0xcc,0xd0`.
That is exactly the observed `0xb4/0xc4` pair.

## 3. Store / pool tables (baseline)

`vst1.32` groups, iPadHD arm (offset -> 4 words):

```
ORIG 0x14 [30,12,28,7]   0x04 [21,42,40,52]  0x24 [7,7,4,42]
     0x34 [8,9,82,140]   0x44 [7,15,7,125]   0x54 [487,341,63,90]
     0x64 [112,566,191,63] 0x2c8 [63,126,63,112] 0x7c [12,14,112,42]
     0xc0 [9,11,5,253]   0xb0 [4,20,25,11]   0xec [56,112,281,562]
     0x294 [281,118,16,16] 0x2a4 [154,140,147,42]
     0x108 [-2,1,2,1]    0x118 [9,35,21,1]    0x128 [1,21,78,151]
     0x140 [15,18,2,189] 0x150 [5,70,70,206] 0x160 [68,0,35,184]
     0x170 [174,84,84,217] 0x180 [23,163,23,244] 0x190 [-6,8,16,90]
     0x1a0 [18,22,22,243] 0x1b0 [18,42,46,1] 0x1c0 [92,26,40,50]
     0x1d0 [7,17,5,14]
OURS 0x14 [30,12,28,7]   0x04 [21,42,40,52]  0x34 [8,9,82,140]  <- swapped
     0x24 [7,7,4,42]     0x44 [7,15,7,125]   0x54 [487,341,63,90]
     0x64 [112,566,191,63] 0x2c8 [63,126,63,112] 0x7c [12,14,112,42]
     0xb4 [20,25,11,9]   0xc4 [11,5,253,21]  0xdc [14.0625,1.40625,28.125,1.40625]
     0x294 [281,118,16,16] 0x2a4 [154,140,147,42]
     0x118 [9,35,21,1]    0x108 [-2,1,2,1]    0x128 [1,21,78,151]
     0x148 [2,189,5,70]  0x158 [70,206,68,0] 0x168 [35,184,174,84]
     0x180 [23,163,23,244] 0x190 [-6,8,16,90] 0x1a0 [18,22,22,243]
     0x1b0 [18,42,46,1]  0x1c0 [92,26,40,50] 0x1d0 [7,17,5,14]
```

Code / pool budget (function-relative):

| build | size | code end | code | pool words |
|---|---:|---:|---:|---:|
| ORIGINAL | 7,072 | `0x182c` | 6,188 | **221** |
| baseline | 7,072 | `0x18a8` | 6,312 | **190** |

The 124 B gap is 124 B *less code* + 124 B *more pool* in the original.

## 4. Group-fix variants (real `link_so` + fdiff + align_diff)

Each fix is a pure statement reorder (no expression edit):

* **B** = move `0xc0,0xc4,0xc8,0xcc` before `0xb0,0xb4,0xb8,0xbc`
* **C** = move `0x140,0x144` into the `0x13c..0x15c` ascending run
* **D** = convert the `field_0x20c` if/else to a ternary, then order
  `0x204,0x208,0x20c,0x210`
* **E** = device-major `0x2f4..0x318` block (iPadHD arm / iPadLarge / iPad /
  n9-retina-default + `iPadLarge` tail)

| variant | size | raw | aligned | groups fixed |
|---|---:|---:|---:|---|
| baseline | **7,072** | **5,830** | **2,119** | — |
| B | 7,064 | 5,883 | 2,119 | 0xb0 |
| C | 7,004 | 5,875 | 2,119 | 0x140 |
| B+C | 6,996 | **5,672** | 2,119 | 0xb0,0x140 |
| D | 6,980 | 5,895 | 2,119 | 0x204 |
| B+D | 6,980 | 5,774 | 2,119 | 0xb0,0x204 |
| C+D | 6,968 | 5,894 | 2,119 | 0x140,0x204 |
| B+C+D | 6,952 | 5,859 | 2,119 | 0xb0,0x140,0x204 |
| E | 7,108 | 5,825 | 2,119 | 0x2f4 device-major |
| B+E | 7,100 | 5,838 | 2,119 | 0xb0 |
| C+E | 7,060 | 5,899 | 2,119 | 0x140 |
| B+C+E | 7,028 | 5,836 | 2,119 | 0xb0,0x140 |
| D+E | 7,008 | 5,892 | 2,119 | 0x204 |
| B+D+E | 7,016 | 5,857 | 2,119 | 0xb0,0x204 |
| C+D+E | 7,024 | 5,934 | 2,119 | 0x140,0x204 |
| B+C+D+E | 6,992 | 5,867 | 2,119 | 0xb0,0x140,0x204 |

**No subset lands at 7,072 B.**  Every group fix removes code (the merged
vector store replaces 4 scalar/`strd` stores) without adding pool words.
B+C alone is the best raw result ever measured for this function
(**5,672**, 158 B better than baseline) but is 76 B short.

## 5. The 0xc0 and 0xec groups resist

* **0xc0**: even with the source order `0xc0,0xc4,0xc8,0xcc,0xb0,...` the
  compiler sinks the `0xc0` store (`str.w ip,[r0,#192]`) after the
  `0xc4..0xd0` vector; the 16-store chunk boundary falls between `0xc8` and
  `0xcc`, so SLP never sees a 4-long `0xc0..0xcc` chain.  Moving
  `centerYBase` (`0xd0`) next to the group does not help.
* **0xec**: ours vectorises the four *float* fields `0xdc..0xe8`
  (`vst1.32 {14.0625,1.40625,28.125,1.40625}`) and stores the four int
  fields `0xec..0xf8` as two `strd`; the original does the opposite.  All
  reorders tested (w9908/w9909) keep the float group.

## 6. Conclusion / next lever

The group fixes are correct and worth ~120 B of code, but the original's
extra **31 pool words** must be produced *at the same time*.  Those words
are the duplicated per-arm constant vectors (the original's iPadHD and n9
arms each carry their own copies; ours are merged).  A faithful
device-major rewrite of the **whole** `0x04..0x1e0` region with literal
per-arm constants should add the pool words; the naive generator tried here
(88 ternaries split into 3 arms) folds to 6,836 B and needs the arm bodies
to be ordered like the original's emitted order to avoid over-merging.

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10009
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10009/fdiff.py _ZN6LayoutC1Ev        # 7072/7072, diff 5830
python3 _work/reconstructions/align_diff.py _ZN6LayoutC1Ev   # 2119
cmake --build cmake-build-match --target check -j6    # RATCHET PASS
```

Scratch `/dev/shm/w10009/`: `fdiff.py`, `measure.sh`, `reorder.py`,
`stores.py` (vst/strd/str table), `storelist.py`, `pool.py`, `simslp2.py`
(SLP chain simulation), `gen_dm.py` (device-major generator),
`Layout.base.cpp`, `L.c0first.cpp`, `L.g140.cpp`, `L.g204b.cpp`,
`L.dmbase.cpp`, `combos.py`, `SLP.cpp` (LLVM 7.0.2 source).
