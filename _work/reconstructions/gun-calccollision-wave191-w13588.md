# `Gun::calcCharacterCollision` wave 191 (w13588): the stack-slot swap is a
# source lever, but the RA plateau survives it; no landed win

Worker: w13588. Branch `wave191b-w13588`, base master `9ae3b6654`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump `0x152f90`, size `1984 B`,
660 insns. Scratch: `/home/debian/GalaxyOnFire2/scratch/w13588`.

## 0. Result

**No `src/` change landed.** The committed body remains the best measured
shape: **raw 455 / aligned 93 / 1984 B**, 660/660 insns.

| build | raw | aligned | size | insns |
|---|---:|---:|---:|---:|
| before (HEAD `9ae3b6654`) | **455** | **93** | 1984/1984 | 660/660 |
| after this pass (note only) | **455** | **93** | 1984/1984 | 660/660 |

`git status --porcelain` is empty apart from this note.

## 1. New evidence (a): the two 60-B frame slots and their owners

The frame is 208 B and contains **two** 60-B slots, not one:

* `fi#10` (size 60, align 8) at **`sp+104`** — the `Matrix view` alloca (`%11`);
* `fi#11` (size 60, align 4) at **`sp+44`** — the `MatrixSetTranslation`
  by-value-return sret temp (`%12`).

Both are live simultaneously in the scatter block, so `StackColoring`
("Merge disjoint stack slots", the pass *before* Early MachineLICM and
MachineCSE in this clang) cannot merge them.  It only merges the small
`Vector` allocas into them.  Frame map from the final disassembly:
`sp+12` (fi#0, 4 B), `sp+44..sp+104` (fi#11), `sp+104..sp+164` (fi#10),
`sp+164/176/192` (three 12-B slots), `sp+204` (canary).

## 2. New evidence (b): the original's slot assignment is the *swap* of ours

The small-`Vector` merge is the one thing that differs.  In the original:

```
1530d2: add r4, sp, #44     ; playerPosition  (getPosition sret)  -> fi#11
1530dc: add r5, sp, #104    ; scatter diff    (AEMath::operator-) -> fi#10
1533c8: add r5, sp, #104    ; view                                -> fi#10
1533d8: add r0, sp, #44     ; MatrixSetTranslation sret           -> fi#11
```

In our committed build it is exactly swapped:

```
1530d2: add r4, sp, #104    ; playerPosition -> fi#10
1530dc: add r5, sp, #44     ; scatter diff   -> fi#11
1533ca: add r0, sp, #104    ; view           -> fi#10
1533d6: add r0, sp, #44     ; MatrixSetTranslation sret -> fi#11
```

So the original merges `playerPosition` into the *MatrixSetTranslation* slot
and the scatter `diff` into the *view* slot; we do the opposite.  This is a
`StackColoring` greedy-colouring tie (both temporaries are 12 B and are
processed in alloca order `%5` = playerPosition, `%6` = diff; `%5` takes
`fi#10`, then `%6` interferes with it at the `operator-` call and falls to
`fi#11`).

## 3. New evidence (c): the swap *is* reachable from source

Declaring the diff as a named local whose initializer is evaluated after its
alloca makes the frontend emit the `diff` alloca *before* the `getPosition`
temp, which flips the colouring to the original's assignment:

```c++
} else if (scatter) {
    Vector diff = enemyPosition - ((Level *) this->level)->getPlayer()->getPosition();
    int distance = (int) AbyssEngine::AEMath::VectorLength(diff);
    ...
}
```

Measured (`f_diff_first`): **raw 467 / aligned 67 / 1984 B / 660 insns**.
The disassembly confirms the original's slots (`add r5,sp,#44` for
playerPosition, `add r4,sp,#104` for the diff).  Adding `alignas(8)` to
`view` (or to `diff`, or renaming `view` to `m`) gives the best diff-first
shape measured: **raw 459 / aligned 103 / 1984 B** (`df_view_a8`).

**But the RA plateau survives the swap.**  `i*3` is still in `r5`, `%stack.10`
is still in `r4`, the two `add r4,sp,#104` at `0x153198`/`0x153260` are still
there and the extra `ldr.w fp,[sp,#40]` is still at `0x153278`.  So the
slot assignment is *not* the binding constraint; the live-range priority is.

## 4. New evidence (d): `i` is spilled in *both* builds

The `i`-reload difference is not about `i` being in a register:

* both builds store `i` with `str.w fp,[sp,#40]` at `0x153074`;
* the original reloads it **4×** (`0x153198`, `0x1531fc`, `0x153256`,
  `0x153262`);
* ours reloads it **3×** (`0x1531fa`, `0x153254`, `0x153278`).

Ours replaces the two 4-B `ldr.w fp,[sp,#40]` at `0x153198`/`0x153260` with
2-B `add r4,sp,#104` and pays one extra 4-B `ldr.w fp,[sp,#40]` at
`0x153278`; the region re-synchronises at `0x15327c`.  The value kept live
across the 50/50 `if (mine)` branch is `%308` = the hoisted
`t2ADDri %stack.10`, not `i`.

## 5. New evidence (e): `%308` is hoisted to the e-loop preheader

`Early MachineLICM` moves the single `t2ADDri %stack.10` to `bb.5`
(`%ir-block.36`, the e-loop preheader) — MIR dump line 142 of the
"Early Machine Loop Invariant Code Motion" section.  In the "Live Interval
Analysis" dump `%308` is then used at slot offsets 1568 B (bb.5), 3616/3728 B
(scatter), and 14704/14896 B (view block).  Its live range therefore spans
almost the whole function, and `RAGreedy::enqueue` (size priority,
`(1u<<29)+Size`) dequeues it before `i` (which starts at the i-loop header,
2768 B) and gives it `r4`.

The view block is the tail of that range: ours rematerialises `&stack.10`
three times (`add r0,sp,#104` at `0x1533ca`, `add r1,sp,#104` at `0x1533dc`,
`add r4,sp,#104` at `0x1533ea`) where the original keeps it in `r5`
(`add r5,sp,#104` at `0x1533c8` + two `mov`s).  In ours `r5` is `i` and `r4`
is `%308`, so neither is free.

## 6. Cells measured this pass (all `link_so` + `tools/fn_diff.py`, `-j2`)

Size-exact 1984 unless stated; none beats 455.

### 6.1 faithful body + slot swap

| variant | raw | aligned | size | insns |
|---|---:|---:|---:|---:|
| committed base | **455** | **93** | 1984 | 660 |
| faithful C-D-E `expect(mag>dist,0)` | 463 | **57** | 1984 | 660 |
| `Vector diff = enemyPosition - getPlayer()->getPosition();` + faithful (`f_diff_first`) | 467 | 67 | 1984 | 660 |
| same + `alignas(8) Matrix view` / `alignas(8) Vector diff` / `view`→`m` | **459** | 103 | 1984 | 660 |
| same + committed E-C-D body (`df_ecd`) | 459 | 103 | 1984 | 660 |
| `Vector diff;` before `playerPosition`, assign (`f_diff_first2`) | 1644 | 281 | **1964** | 661 |
| `playerPosition` hoisted before `enemyPosition` (`f_pp_early`) | 1579 | 298 | **1964** | 661 |
| faithful + `Matrix view` declared before the two `SetAnimationState` (`f_view_early`) | 1162 | 120 | **1988** | 661 |
| `alignas(8) Vector playerPosition` (base / faithful) | 455 / 463 | 93 / 57 | 1984 | 660 |
| `alignas(8) Matrix view` (base) | 455 | 93 | 1984 | 660 |
| `alignas(16) Matrix view` (base) | 1859 | 245 | **1996** | 664 |
| `Matrix view; __builtin_memcpy(&view, …, 60)` | 1919 | 277 | **2064** | 690 |
| `const Matrix *cl = …; Matrix view = *cl;` | 455 | 93 | 1984 | 660 |

### 6.2 `Matrix` alloca-order / lifetime levers

| variant | raw | aligned | size |
|---|---:|---:|---:|
| `Matrix mst;` before `view`, `mst = MatrixSetTranslation(view,…)` | 1829 | 419 | **2000** |
| same, `TransformSetLocal(…, mst)` | 1833 | 382 | **2000** |
| diff-first + `mst` before `view` | 1832 | 412 | **2000** |
| faithful + `mst` before `view` | 1826 | 399 | **2000** |
| `Matrix view;` at i-loop top, assign in block | 1880 | 393 | **2024** |
| `Matrix &view = *(...)` (no copy) | 1195 | 177 | **1972** |
| `TransformSetLocal(…, *(Matrix*)CameraGetLocal(...))` (no translation) | 1677 | 368 | **1932** |
| `Matrix view;` uninitialised + `MatrixSetTranslation` | 1762 | 345 | **2008** |
| `static Matrix view;` | 1725 | 265 | **2056** |
| `view = MatrixSetTranslation(view,…)` | 1103 | 137 | **1992** |
| `TransformSetLocal(…, MatrixSetTranslation(view,…))` | 1324 | 308 | **1976** |
| `Matrix *vp = &view; … *vp` | 455 | 93 | 1984 |
| view block in its own `{ }` scope | 455 | 93 | 1984 |
| `Matrix m` instead of `view` | 455 | 93 | 1984 |

### 6.3 `expect` / loop-shape sweep on the committed base

| variant | raw | aligned | size |
|---|---:|---:|---:|
| `expect(mine,0/1)` on the e-loop `if (mine)` (faithful) | 1681 / 672 | 212 / 101 | 1984 |
| `expect(enemies==nullptr,0)` | 455 | 93 | 1984 |
| `expect(enemies==nullptr,1)` | 1862 | 346 | **1932** |
| `expect(e<size,0/1)` on the e-loop | 455 / 455 | 93 / 93 | 1984 |
| `expect(i<count,0/1)` on the i-loop | 455 / 455 | 93 / 93 | 1984 |
| `expect(k<size,1)` on the k-loop (`kcond1`) | 1798 | 379 | **1948** |
| faithful + `expect(k<size,1)` | 1800 | 355 | **1948** |
| faithful + `expect(k<size,0)` | 1717 | 303 | **1944** |
| `expect(diff>0,1)` (hot base) | 1887 | 281 | **1908** |
| `expect(getHitpoints()<1,0)` | 671 | 159 | 1984 |
| `i = i + 1` / `k = k + 1` / `unsigned long i` / `unsigned long k` / `long diff` / cached `magnitude` | 455 | 93 | 1984 |
| `int e` | 455 | 93 | 1984 |
| i-loop increment moved to the bottom | 1814 | 240 | **1964** |

### 6.4 statement reordering (committed base)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| `KIPlayer *ki` before the radius block | 1558 | 345 | **1964** |
| radius block before the `ki` check | 455 | 93 | 1984 |
| `velocity` before `position` | 470 | 116 | 1984 |
| `position` after `enemyPosition` | 509 | 132 | 1984 |
| `enemyPosition` first | 538 | 137 | 1984 |
| `position`/`velocity` via `const Vector *` | 455 | 93 | 1984 |
| cached `Array<Player*>* enemies` for the e-loop | 1355 | 183 | **1980** |
| `Vector *td` declared inside the scatter block | 455 | 93 | 1984 |
| scatter/mine radius branch order swapped | 1810 | 288 | **1976** |
| `if (mine)` and `if (scatter)` split | 1803 | 275 | **1976** |
| `AbyssEngine::Transform *tr` cached for the two `SetAnimationState` calls | 1174 | 142 | **1968** |

### 6.5 k-loop forms with the committed body

| variant | raw | aligned | size | k-loop |
|---|---:|---:|---:|---|
| committed `for (k…)` | **455** | **93** | 1984 | header-first |
| `while (k<size) { ++k; data[k-1] }` | 1585 | 171 | **1980** | body-first |
| `while (k<size) { enemy=data[k]; ++k; }` | 1585 | 171 | **1980** | body-first |
| diff-first + `while { ++k; data[k-1] }` | 1585 | 171 | **1980** | body-first |
| faithful + `alignas(8) Matrix view` + `while { ++k; data[k-1] }` | 1577 | 121 | **1980** | body-first |

## 7. Conclusion

The pass adds three facts that were not in w13553/w13564/w13576/w13583:

1. The frame has **two** 60-B slots (view = `fi#10` at `sp+104`,
   `MatrixSetTranslation` sret = `fi#11` at `sp+44`); the small `Vector`
   temporaries are coloured into them by `StackColoring`, and the original's
   colouring is the **swap** of ours.
2. That swap is reachable from source (`Vector diff = enemyPosition -
   getPlayer()->getPosition();`), and it reproduces the original's
   `sp+44`/`sp+104` assignment exactly — but the RA still gives `%stack.10`
   `r4` and `i` `r5`, so the swap alone is worth nothing (best diff-first
   shape 459/103, worse than 455/93).
3. `%308` is hoisted to the **e-loop preheader** (`bb.5`), which is why its
   live range beats `i`'s in `RAGreedy::enqueue`.  `i` is spilled in both
   builds; the residual is purely *where* `i` is reloaded, which is a
   consequence of `%308` holding `r4` across the `if (mine)` branch and the
   view block.

The committed `expect(diff > 0, 0)` + E-C-D body remains the unique optimum.

## 8. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13588
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN3Gun22calcCharacterCollisionEv
# -> raw=455 aligned=93 size=1984/1984 insn=660/660
```

Slot-swap cell (§3):

```c++
} else if (scatter) {
    Vector diff = enemyPosition - ((Level *) this->level)->getPlayer()->getPosition();
    int distance = (int) AbyssEngine::AEMath::VectorLength(diff);
    if (enemy->getKIPlayer() != nullptr && enemy->getKIPlayer()->field_0x71) {
        radius = (int) ((float) radius * 0.5f);
    } else if (distance > 20000) {
        radius = (int) (float) (radius * 3);
    } else {
        radius = (int) (distance > 10000 ? (float) (radius * 2) : (float) radius * 1.5f);
    }
}
# faithful k-loop body -> raw=467 aligned=67 size=1984
# + alignas(8) Matrix view -> raw=459 aligned=103 size=1984
```

MIR recipe (same as w13564 §6):

```sh
clang++ <original flags> -g0 -S -o mbp.s \
  -mllvm -print-after-all \
  -mllvm -filter-print-funcs=_ZN3Gun22calcCharacterCollisionEv \
  -c src/game/weapons/Gun.cpp 2> mbp.err
# StackColoring:   "IR Dump After Merge disjoint stack slots"
# LICM:            "IR Dump After Early Machine Loop Invariant Code Motion"
# CSE:             "IR Dump After Machine Common Subexpression Elimination"
# Live intervals:  "IR Dump After Live Interval Analysis"
```

## 9. Files changed

`_work/reconstructions/gun-calccollision-wave191-w13588.md` only.
