# `Layout::Layout` C1/C2 — wave 190 / worker w13473

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original vaddr
`0xd1028`, linked symbol size **7,072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave190-w13473`, base master `db07789dd`.

**Result: no size-exact raw win found.  `src/` left at the base (raw 5,249,
aligned 2,725, size 7,072/7,072, insn 2,520/2,552).**  Gate re-run green:
`RATCHET PASS`, byte_exact 4507/4524, linked_exact 4507/4524, size-exact
4524/4524, imports 207, lint CLEAN.

Read `Layout-LayoutC1-wave190-w13420.md` and `...-w13443.md` first; this note
records only cells **not** already in those two.

## 1. Offset mapping correction (important for any order experiment)

`Layout.h` is not ordered by offset in a way a naive member walk recovers:
`Blk16` (`field_0x238_blk16`) is **16 bytes**, not 4.  A member walk that
assumes 4 bytes for it is off by **-0xc for every field after 0x238**
(e.g. it puts `helpWindowY` at 0x314 instead of the real 0x320).  Any
"reorder to the recovered store order" experiment must use `Blk16 = 16`.

## 2. Pool tail: current base matches **1** word, not 4

Word-aligned at the same address (base build, verified directly from the
linked `.so`):

| addr | original | ours |
|---|---|---|
| `d2ba0` | `0x0000000f` 15 | `0x42a40000` 82.0 |
| `d2ba4` | `0x00000005` 5 | `0x42240000` 41.0 |
| `d2ba8` | `0x00000003` 3 | `0x43480000` 200.0 |
| `d2bac` | `0x000000aa` 170 | `0x42c80000` 100.0 |
| `d2bb0` | `0x43480000` 200.0 | `0x43160000` 150.0 |
| `d2bb4` | `0x42c80000` 100.0 | `0x43100000` 144.0 |
| `d2bb8` | `0x43160000` 150.0 | `0x43160000` 150.0 **match** |
| `d2bbc` | `0x43100000` 144.0 | `0x42980000` 76.0 |
| `d2bc0` | `0x43160000` 150.0 | GOT addend |
| `d2bc4` | `0x42980000` 76.0 | GOT addend |

So the tail is `82, 41, 200, 100, 150, 144, 150, 76, GOT, GOT` in ours and
`200, 100, 150, 144, 150, 76` in the original.  The concrete target for a
future pass is: move the `field_0xac` non-HD constants (`82/41`) out of the
tail pool into an inline island, and drop the two trailing
`retinaDisplay` GOT words, then add **4 words (16 B) of code** so the total
stays 7,072.  That would make the six tail words match at `d2bb0..d2bc4`.
Both sub-goals are the same structural residue as w13420 §3 (callee-saved
`r8` retina pointer) and w13420 §1 (inline island placement).

## 3. Measured negatives (new cells; baseline `(5249, 2725, 7072)`)

### 3.1 Full reorder to the recovered source/store orders

Using the corrected offset map:

| order | raw | size |
|---|---:|---:|
| iOS 1.1.4 source order (204-field list) | **6,150** | 7,052 |
| Android 2.0.16 default-path store order (`layout_ctor/order_default.txt`) | **6,395** | 6,960 |

Both are far worse than the hill-climbed order.  The current order is a
genuinely better raw basin than the true source order on this compiler.

### 3.2 Full single-move sweep inside the early block

All 812 ordered pairs from the 29-statement block
`windowTopInset .. field_0x98` (lines 25-53): **no win**.  Best is identity
`5249`; 10 moves are codegen-identical neutrals (`5249/2725/7072`), every
other move is worse.

### 3.3 Block permutations

| block | perms | best raw | size |
|---|---:|---:|---:|
| `{0x34,0x38,0x3c,0x40,0x44}` | 120 | 5,759 | 7,048-7,056 |
| `{0x94,0x98,0x9c,0xa0}` | 24 | 5,994 | 6,912-6,936 |

The original's `{0x34,0x38,0x3c,0x40}` SLP group cannot be reproduced without
shrinking the body by 16-24 B.

### 3.4 Pair-adjacency moves (20 field pairs, both orders)

`(74,78) (2c0,2c4) (a4,a8) (94,98) (94,9c) (94,a0) (98,9c) (98,a0)
(9c,a0) (8c,90) (90,94) (7c,80) (80,84) (84,88) (a0,a4) (a8,ac)
(b0,b8) (b8,b4) (b4,bc) (bc,c0)`: no win.  Best non-identity is
`0x78 before 0x74` at `(5693, 2757, 7072)`.

### 3.5 Flag hoisting into a local (`const unsigned char v = Globals::F;`)

| flag | raw | size |
|---|---:|---:|
| `iPad` | 5,972 | 6,948 |
| `iPadLarge` | 6,004 | 6,932 |
| `n9` | 6,311 | 6,680 |
| `retinaDisplay` | build fail (name collision) | — |
| `iPadHD` | 5,719 | 6,956 |

Hoisting does not move the HD flag into `sl`; it only CSEs loads, exactly as
w13443 §2.7 found.

### 3.6 `(*rd)` substitution for individual late `retinaDisplay` uses

| statement(s) | raw | size |
|---|---:|---:|
| `helpWindowY` | 5,282 | 7,076 |
| `rewardBoxY2` | 5,277 | 7,088 |
| `field_0x2f4 + field_0x2f8` | 5,295 | 7,080 |
| `field_0x1f8` | 5,302 | 7,088 |
| `field_0x290` | 5,303 | 7,088 |

All grow the body by +4..+16 B and none is close enough on raw to compensate.

### 3.7 Placement sweeps

* `field_0xac` to 45 anchors (early, mid and late): best `5477` (`before 0x8c`).
* `helpWindowY` to 12 anchors: best `5309` (`before 0x290`).
* `field_0x140` to 70 anchors (units 95-165): identity `5249`; best
  non-identity `5252` (`before orbitInfoSecurityY`).
* Hoisting each of the first 10 flat statements above the initial
  `if (Globals::iPadHD)` block: all worse (best `5668`, `0x40`).

### 3.8 Random multi-move search

400 random two-move combinations from the 184 flat assignments: best
size-exact result **5,781**, no win.  300 random single moves were already
negative in w13443 §2.10.

## 4. Conclusion

The current order is a very sharp local optimum.  Every single-statement
move, every adjacent/distance-2/K=6 window, all block permutations of the
SLP groups, the true iOS source order and the recovered Android store order
are all worse.  A raw win now needs the structural reconstruction described
in §2: reproduce the original's non-HD SLP/`strd` grouping (which shrinks
code by ~132 B) while the pool grows by the same ~132 B, and in particular
move the `field_0xac` `82/41` pair out of the tail pool.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13473
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
# raw=5249 aligned=2725 size=7072/7072 insn=2520/2552
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13473/`: `move.py`,
`block.py`, `variant.py`, `hoist.py`, `ifelse.py`, `sweep5.py`,
`sweep4.py`, `sweep_early.py`, `snap_ios.py`, `reorder2.py`, `orig.dis`,
`ours.dis`.
