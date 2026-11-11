# `Layout::Layout` C1/C2 — wave 175 / worker w11910

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original
vaddr `0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Baseline (branch `wave175-w11910`, base master `5f599a137`): raw **5,633**,
aligned **2,290**, 7,072 B, 2,519 / 2,552 instructions.

**Exit: one-commit raw win.**  Swapping the tail-store order of
`field_0x214` and `field_0x200` lowers raw **5,633 -> 5,629** at
7,072/7,072 B (aligned 2,290 -> 2,292).  The `.text` residue is otherwise
unchanged and the ratchet is green.

## 1. The landed change

In the tail (`src/game/ui/Layout.cpp`, the block that starts at
`field_0x1e4`), the two statements

```cpp
this->field_0x214 = Globals::iPadHD ? 1 : (Globals::n9 ? -2 : (Globals::retinaDisplay ? -2 : 1));
...
this->field_0x200 = Globals::iPadHD ? -18 : (Globals::n9 ? -26 : (Globals::retinaDisplay ? -26 : -13));
```

are swapped.  `field_0x200` now sits where `field_0x214` used to be (third
statement of the tail, immediately after `field_0x238_blk16.a`), and
`field_0x214` moves down next to `field_0x234`.  Both are plain scalar
assignments to distinct fields, so the swap is semantically neutral; it only
changes clang's store scheduling / constant materialisation.  The merge point
(`d22a6`) and the function size are unchanged.

```diff
-    this->field_0x214 = Globals::iPadHD ? 1 : (Globals::n9 ? -2 : (Globals::retinaDisplay ? -2 : 1));
+    this->field_0x200 = Globals::iPadHD ? -18 : (Globals::n9 ? -26 : (Globals::retinaDisplay ? -26 : -13));
     this->orbitInfoSecurityY = ...
...
     this->field_0x234 = ...
-    this->field_0x200 = Globals::iPadHD ? -18 : (Globals::n9 ? -26 : (Globals::retinaDisplay ? -26 : -13));
+    this->field_0x214 = Globals::iPadHD ? 1 : (Globals::n9 ? -2 : (Globals::retinaDisplay ? -2 : 1));
```

| shape | raw | aligned | size |
|---|---:|---:|---:|
| baseline | 5,633 | 2,290 | 7,072 |
| **landed swap** | **5,629** | **2,292** | **7,072** |

The swap was found by a seeded random swap search over the tail units
(`scratch/w11910/search_tail.py`, seed 11, iteration 274).  A follow-up
hill-climb (`climb.py`, 600 + 300 iterations, seeds 3 and 5, tail swaps plus
cross tail/T-block swaps) accepted no further improvement, so 5,629 is the
current local optimum of that neighbourhood.

## 2. Region budget (new measurement)

Boundaries from `objdump -M force-thumb`: prologue `d1028..d1042`;
iPadHD block `d1042..` first `b.w` target; merge = target of the prologue
`beq.w`; code end = last `b`/`pop` before the final literal-pool island
(`d2852` orig / `d28d0` ours).

| region | original | ours | delta |
|---|---:|---:|---:|
| prologue | 26 | 26 | 0 |
| iPadHD block | 554 | 568 | **+14** |
| non-iPadHD | 3,958 | 4,140 | **+182** |
| first half total | 4,538 | 4,734 | **+196** |
| tail + T block | 1,648 | 1,578 | **-70** |
| code total | 6,186 | 6,312 | **+126** |
| literal pool | 886 | 760 | **-126** |
| function | 7,072 | 7,072 | 0 |

So the size is a three-way compensation: **+196 B of first-half code** is
paid for by **-70 B of tail/T** and **-126 B of literal pool**.  The merge
point is therefore addressed 196 B late (`d22a6` vs `d21e2`), which is why
every faithful-tail reconstruction (which matches the original's tail bytes
but sits at the wrong address) measures *worse* raw than the baseline.

## 3. Why the faithful dispatch cannot win on its own

The exact tail dispatch from the wave-173 note was rebuilt with the checked-in
T block and measured end-to-end (`scratch/w11910/build.py`,
`faithful_tail.txt`, `tblock_base.txt`):

| shape | raw | aligned | size |
|---|---:|---:|---:|
| baseline | **5,633** | **2,290** | **7,072** |
| faithful tail + base T | 5,997 | 2,681 | 7,072 |
| faithful tail + `0x2bc`/`0x200`/`0x285` in T (best position, 21/33) | 5,929 | 2,698 | 7,072 |
| faithful tail + base T, first half reordered `0x40..0x100` | 6,119 | 3,045 | 6,864 |
| faithful tail + base T, first half reordered `0xc0..0x200` | 6,221 | 3,017 | 6,624 |

Moving the three stores into the T block does move the merge to `d225c`
(-74 B) and is the best size-exact faithful state found (5,929), but it is
still +296 raw over the baseline.  Reordering the first half towards the
original's store order can put the merge back on `d21e2` (`0xc0..0x200` gives
`d21e4`) but collapses the body to 6,624 B, i.e. the literal pool shrinks by
another ~450 B and the tail is compared at yet another offset.

## 4. The T block is saturated for single-unit moves

The T block (source lines `field_0x285` .. `helpWindowY`, 32 units after
grouping the `rewardBoxY2` if/else chain into one unit; splitter
`scratch/w11910/tb_units.py`) was swept exhaustively: every unit relocated to
every other position, **32 x 31 = 992 real `link_so` + `fn_diff` builds**
(`sweep_tb_move.py`).  No size-7,072 state beats raw 5,633; the closest
neighbours are 5,634 and 5,643.  A 300-state random swap search over the same
units (`search_tb3.py`, seed 7) likewise found only 5,634 / 5,643 / 5,681.
The T-block order is therefore locally saturated; the win came from the
*tail* block instead.

Note: the older `tblock_search.py` / `tblock_hits.json` in scratch used a
brace-based block splitter that silently swallowed the whole T block (the
`rewardBoxY2` chain has no braces), so its "no hits" result covered only a
5-unit space.  `tb_units.py` is the corrected splitter.

## 5. Literal-pool evidence (why the residue is structural)

The pool is the final contiguous data island.  Original `d2854..d2bc8`
(884 B / 221 words), ours `d28d0..d2bc8` (760 B / 190 words).  Comparing the
two pools word-for-word:

* end-aligned: 14 / 190 words equal, 269 / 760 bytes differ;
* start-aligned: 5 / 190 words equal;
* best relative offset (36 words = 144 B): 62 / 190 words equal.

The pools share long runs (`0xd, 0x25, 0x16, 2, -1, 2, 1, -1, 2, 0x16, 0x3c,
0x78, ...`) but ours is missing the original's early `60/120/300/600`
constants and instead carries `15.0f/1.5f/12.0f/1.5f` later.  Those are the
constants of `field_0x30_rowHeight`, `field_0xf0`, `field_0x264` /
`field_0x278` and `driftSpeedDivisor`; the original first-uses them in its
(iOS-ordered) first half, ours in the scrambled order.  Fixing the pool means
fixing the first-half source order, not the tail.

## 6. Conclusion

* The tail store order still had one byte-level degree of freedom:
  `field_0x214 <-> field_0x200` is worth **-4 raw** at constant size.
* The T block is exhausted for single-unit relocations (992 states).
* The remaining 5,629 raw is dominated by the +196 B first-half width and the
  -126 B pool deficit; landing the faithful tail dispatch requires
  reconstructing the original's non-iPadHD branch duplication (which shrinks
  the first half by ~196 B and grows the pool by ~126 B) first.

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w11910
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w11910/`: `Layout.base.cpp`
(pristine baseline ctor), `measure.sh` (build + `fn_diff`), `merge.py`
(prologue `b.w` target), `poolinfo.py` (code/pool split), `tb_units.py`
(corrected T/tail unit splitter), `search_tail.py` (seeded tail swap search,
found the win), `search_tb3.py`, `sweep_tb_move.py` (T-block sweeps),
`climb.py` (hill-climb), `build.py`, `faithful_tail.txt`, `tblock_base.txt`,
`tblock_2bc200.txt`, `sweep_range.py` (first-half partial reorders).

Gate at exit (`check` + `drift`): byte_exact **4507/4524**, linked_exact
**4507/4524**, size-exact **4524/4524**, data_exact **843/843**, imports
**207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370** (0 extra /
0 missing), `.rel.plt` byte-identical.  `Layout` C1/C2 raw **5,629**,
aligned **2,292**, size **7,072/7,072**, merge `d22a6`.
