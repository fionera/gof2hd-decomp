# `Gun::calcCharacterCollision` wave 191 (w13576): the whole residual is one
# greedy-RA region-split decision; ~70 more size-exact reshapes, all >= 455

Worker: w13576. Branch `wave191b-w13576`, base master `89b2c8dcd`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump `0x152f90`, size `1984 B`,
660 insns. Scratch: `/home/debian/GalaxyOnFire2/scratch/w13576`.

## 0. Result

**No `src/` change landed.** The committed body remains the best measured
shape: **raw 455 / aligned 93 / 1984 B**, 660/660 insns.

| build | raw | aligned | size | insns |
|---|---:|---:|---:|---:|
| before (HEAD `89b2c8dcd`) | **455** | **93** | 1984/1984 | 660/660 |
| after this pass (note only) | **455** | **93** | 1984/1984 | 660/660 |

`git status --porcelain` is empty apart from this note.

## 1. The residual is a *single* register decision, not four clusters

The five raw clusters reported by w13553 (§1 there) all collapse to **one**
greedy-RA choice: `%stack.10` (the 60-B slot shared by `view`, the scatter
`playerPosition` and the k-loop `getPosition` sret) is coloured **r4**, which
pushes `i*3` to **r5**. The original has it the other way round (`i*3` = r4,
`%stack.10` rematerialised per use, `&view` = r5).

Consequences, all downstream of that one choice:

* `i*3` in r5 instead of r4 — 10 raw bytes (`0x15306c/78/8c`, `0x1530cc`,
  `0x15315a`, `0x153436`, `0x153608/61c/638`).
* the 226-B phase shift — because r4 is held live across the `if (mine)`
  branch, RA materialises `add r4,sp,#104` at `0x153198`/`0x153260` (2 B) where
  the original reloads `i` with `ldr.w fp,[sp,#40]` (4 B), and then has to
  reload `i` again at `0x153278`.
* the 44-B view block — r5 is occupied by `i*3`, so `&view` cannot stay in one
  callee-saved register and is rematerialised into r0/r1/r4.
* the 167-B k-loop block order — the post-RA `BranchProbability`
  `MachineBlockPlacement` reacts to the RA-shaped CFG.

So a fix for the r4/r5 swap is worth ~450 of the 455 residual bytes at once.

## 2. Why the swap cannot be forced from source (new evidence)

`RAGreedy::enqueue` (LLVM 7 `RegAllocGreedy.cpp:678`) prioritises live ranges
by **size, largest first**:

```cpp
  // Prioritize live ranges by size, assigning larger ranges first.
  const unsigned Size = LI->getSize();
  ...
  Prio = (1u << 29) + Size;
```

`%stack.10`'s address is a *single* vreg (`%308` in the MIR) because
`Early MachineLICM` hoists every `t2ADDri %stack.X` to the e-loop preheader
(bb.5) and `EliminateCSE` merges all of them, so `%308` is live from the entry
block to the k-loop — longer than `i*3` (first def `0x15306c`). It is therefore
dequeued first and takes r4.

Hoisting is unavoidable for this instruction shape: `MachineLICM.cpp:1216`
returns `true` from `IsProfitableToHoist` for anything
`TII->isTriviallyReMaterializable`, and `t2ADDri %stack.N, 0` has no register
operands, so `TargetInstrInfo::isReallyTriviallyReMaterializableGeneric`
accepts it. There is no source form that makes a plain `Vector` sret slot
non-rematerialisable.

The only source-controlled lever that changes the decision is the *size* of
`%308`'s live range, and every form that shortens it (a second stack slot for
the k-loop sret) also changes the frame size and therefore the byte count.

## 3. New cells measured this pass (all `link_so` + `tools/fn_diff.py`, `-j2`)

Size-exact 1984 unless stated; none beats 455. (Complements w13553 §4 and
w13564 §4; nothing below was in either table.)

### 3.1 separate k-loop sret slot

| variant | raw | aligned | size | insns |
|---|---:|---:|---:|---:|
| faithful + `Vector kpos;` fn-scope, `kpos = getPosition(); *td = kpos;` | 1826 | 292 | 1972 | 664 |
| faithful + `Vector kpos = getPosition(); *td = kpos;` (in k-loop) | 463 | 57 | 1984 | 660 |
| committed + `alignas(8) Vector kpos = getPosition(); *td = kpos;` | 455 | 93 | 1984 | 660 |
| faithful + `alignas(8) Vector kpos = ...` | 463 | 57 | 1984 | 660 |
| committed/faithful + `alignas(16) Vector kpos = ...` | 1859/1862 | 245/209 | **1996** | 664 |
| committed/faithful + `alignas(32) Vector kpos = ...` | — | — | 1996 | 664 |

`alignas(16)` does **not** separate the slots: the k-loop still reuses the
view-block address live-in (`add r4,sp,#112` appears at the same four sites as
the committed `add r4,sp,#104`); the frame just grows 208 -> 216 for the
alignment padding and the whole body re-lays-out: +12 B / +4 insns.

### 3.2 `view` lifetime / shape

Codegen-identical (455/93/1984): `Matrix *vp = &view` + `*vp` at both call
sites; `Matrix &vr = view`; `Matrix *vp = (Matrix *)CameraGetLocal(...); Matrix
view = *vp;`; `Matrix view(*(Matrix *)...)` (copy-ctor form); the whole block in
a nested `{ }` scope.

| variant | raw | aligned | size |
|---|---:|---:|---:|
| committed + `alignas(16) Matrix view` | 1862 | 209 | 1996 |
| committed + `alignas(32) Matrix view` | 1864 | 209 | 1996 |
| committed + `Matrix view; memcpy(&view, CameraGetLocal(...), 60);` | 1919 | 277 | 2064 |
| committed + `Matrix view[1] = { *(...) };` | 503 | 161 | 1984 |
| committed + `Matrix view; view = *(...)` (assign) | 1889 | 244 | 2056 |
| committed + `impact` block before `explosive/geometries` block | 1221 | 159 | 1984 (661) |

### 3.3 branch-weight / `expect` sweep (committed base, guard kept at `expect(diff>0,0)`)

| added `expect` | raw | aligned | size |
|---|---:|---:|---:|
| `impact != nullptr` = 1 / `isRocket()` = 1 | 455 | 93 | 1984 |
| `mine` (radius) = 0 | 1049 | 157 | 1984 (659) |
| `!isActive()` = 0 | 672 | 161 | 1984 (659) |
| `getHitpoints() < 1` = 0 | 671 | 159 | 1984 (659) |
| `owner == nullptr || enemyFlagsLo` = 1 | 533 | 112 | 1984 |
| `impact != nullptr` = 0 | 1079 | 147 | 1992 |
| `isRocket()` = 0 | 1073 | 134 | 1988 |
| `scatter` = 0 / 1 | 1760/1804 | 371/349 | 1936/1932 |
| `isAsteroid()` = 1 | 1796 | 548 | 1968 |
| `egoFlag` = 0 | 1341 | 145 | 1988 |
| `playerGun` = 0 | 1252 | 134 | 1992 |
| `geometries != nullptr` = 1 | 1608 | 265 | 1956 |
| `explosive` = 1 | 1405 | 378 | 1976 |

On the **hot** (no-`expect`) base the same sites never reach 1984: best is
`expect(scatter,0)` at 1924 B (raw 1628); `empDamage>0`=0/1 -> 1912;
`falloff>1`=0/1 -> 1908; `impact`=0 -> 1920. This confirms w13553's claim that
`expect(diff>0,0)` on the k-loop guard is a *unique* 1984-B anchor.

### 3.4 k-loop body order with the anchor kept

| variant | raw | aligned | size |
|---|---:|---:|---:|
| committed E-C-D, `expect(diff>0,0)` | **455** | 93 | 1984 |
| `expect(this->magnitude - distance > 0, 0)` + C-D-E | 458 | 63 | 1984 |
| `expect(distance < this->magnitude, 0)` + C-D-E | 463 | 57 | 1984 |
| faithful C-D-E, `expect(magnitude>distance,0)` | 463 | 57 | 1984 |
| clamp hoisted out of the `if`, `expect(diff>0,0)` | 460 | 68 | 1984 |
| `if (!(mag > dist)) continue;` (no expect) | 1886 | 257 | 1908 |
| `if (expect(...)) { ... } else { continue; }` | 463 | 57 | 1984 |
| `expect(k < enemies->size(), 0/1)` on the loop condition | 1717/1800 | 303/355 | 1944/1948 |

### 3.5 declarations / types / statement order (committed base)

Codegen-identical (455/93/1984): `const Vector position/velocity/enemyPosition`;
`Player *const enemy`; `Vector *const td`; `int i`; `int mine/scatter/bomb`;
`bool explosive/explodeOnContact`; `long radius`; `float diff`;
`if (scatter && !mine)`; `if (scatter && !explodeOnContact)`;
`if (mine != 0)`; `if (!this->enemies)`; `if (this->impact)`; `if (geometries)`;
`if (!this->owner)`; `if (!ki)`; `if (getKIPlayer() != 0)`;
`this->targetDir` used directly instead of a `td` pointer.

| variant | raw | aligned | size |
|---|---:|---:|---:|
| `unsigned radius` | 459 | 99 | 1984 |
| `unsigned distance` (scatter) | 458 | 97 | 1984 |
| `getHitpoints() < 1` -> `!getHitpoints()` | 458 | 95 | 1984 |
| `Vector velocity` declared before `position` | 470 | 116 | 1984 |
| `hitPositions` store before `hitFlags` store | 476 | 99 | 1984 |
| swap the two `if (mine)` e-loop checks | 481 | 100 | 1984 |
| `Vector enemyPosition` declared first | 538 | 137 | 1984 |
| `int radius` declared first | 1785 | 239 | 1968 |
| `long long radius` | 1815 | 383 | 2004 |
| `float radius` | 1829 | 552 | 1908 |
| `double falloff` | 1197 | 169 | 1984 (659) |
| `unsigned diff` | 1047 | 136 | 1980 |
| `int outer` (instead of `bool`) | 1341 | 153 | 1976 |
| `Array<Player*> *enemies = this->enemies;` cached for the k-loop | 1742 | 348 | 1952 |
| `lifetimes[i] = -1000000` moved after the scatter block | 1820 | 245 | 1964 |

## 4. Root cause (now fully pinned)

1. `Early MachineLICM` hoists every `t2ADDri %stack.N, 0` to bb.5 (it is
   "trivially rematerialisable", `MachineLICM.cpp:1216`) and `EliminateCSE`
   merges them, so the shared 60-B slot has a single vreg `%308` live from the
   entry to the k-loop.
2. `RAGreedy::enqueue` orders by live-range size, so `%308` is allocated before
   `i*3` and takes r4.
3. `RAGreedy::tryRegionSplit` then splits `%308` at the 50/50 `isAsteroid`
   branch and rematerialises `$r4 = t2ADDri %stack.10` at both successors
   (`0x153198`, `0x153260`), which is the 226-B phase shift; the last piece
   (view block -> k-loop) keeps r4 instead of remat'ing, which is the 44-B
   view-block delta and the missing `add r4,sp,#104` at `0x153444`.
4. `i*3` falls to r5, which is the 10-B `i*3` cluster and, indirectly, the
   167-B k-loop block-order cluster (post-RA `BranchProbability`
   `MachineBlockPlacement` reacts to the RA-shaped CFG).
5. The committed `expect(diff > 0, 0)` is what keeps the spill-heavy 1984-B RA
   at all; without it the function re-allocates (`this` = r4) and shrinks to
   1908 B / 646 insns. No other `expect` site reproduces 1984 B.

The swap is therefore a genuine greedy-RA plateau: it can only be flipped by
shortening `%308`'s live range, and every source form that does so (a separate
k-loop sret slot) changes the frame size and lands on 1972/1976/1996 B.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13576
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN3Gun22calcCharacterCollisionEv
# -> raw=455 aligned=93 size=1984/1984 insn=660/660
```

MIR recipe (same as w13564 §6):

```sh
clang++ <original flags> -g0 -S -o mbp.s \
  -mllvm -print-after-all \
  -mllvm -filter-print-funcs=_ZN3Gun22calcCharacterCollisionEv \
  -c src/game/weapons/Gun.cpp 2> mbp.err
# hoist:   "IR Dump After Early Machine Loop Invariant Code Motion"
# CSE:     "IR Dump After Machine Common Subexpression Elimination"
# RA:      "IR Dump After Greedy Register Allocator"
# post-RA: "IR Dump After Branch Probability Basic Block Placement"
```

## 6. Files changed

`_work/reconstructions/gun-calccollision-wave191-w13576.md` only.
