# `HangarWindow::render` — wave 128 (w7656): ship-branch evaluation order vs register pair

Target: `_ZN12HangarWindow6renderEv` (`src/game/ui/HangarWindow.cpp`), original
`0x1490d8`, size `0x25e4` = 9700 B. Worktree `/home/debian/GalaxyOnFire2/wt/w7656`,
base `fee528cbe` (wave-127 final; `render` 94.747001 %, byte_exact 4506).

This pass: **94.747001 -> 94.747001** (raw difflib over the reconciled
instruction stream). No source change landed; the deliverable is this
measurement/evidence record. The gate stays green (`check`: byte 4506,
linked 4506, size 4524, data 843, imports 207, lint CLEAN, drift 0).

The wave-127 ship-branch operand flip is the current shape:

```cpp
if (!(tab == 3 && Globals::status->getShip() == this->selectedItem->ship)) {
```

## What the remaining ship-branch residual actually is

Original `0x149b1e`..`0x149b44`:

```
149b1e: ldr  r0, [sp, #108]        ; tab
149b20: ldrd r5, r4, [sp, #132]    ; layout=r5, this=r4
149b24: cbz  r0, 149b96            ; tab==0 -> upgrade test (NOT the join)
149b26: cmp  r0, #3
149b28: bne.n 149b44               ; tab!=3 -> body
149b2a: ldr  r0, [pc, #340]        ; GOT status
149b2c: ldr  r1, [r4, #104]        ; this->selectedItem     (ship operand FIRST)
149b2e: add  r0, pc
149b30: ldr  r0, [r0, #0]          ; Globals::status
149b32: ldr  r4, [r1, #12]         ; selectedItem->ship -> r4 (clobbers this)
149b34: ldr  r0, [r0, #0]          ; *status
149b36: blx  getShip
149b3a: cmp  r4, r0                ; ship == getShip()
149b3c: ldrd r5, r4, [sp, #132]    ; reload layout+this
149b40: beq.w 14a6d0               ; -> join
```

Ours (landed, flipped source):

```
149b1e: ldr  r0, [sp, #108]
149b20: ldrd r5, r4, [sp, #132]    ; pair matches
149b24: cmp  r0, #0
149b26: beq.w 14a6d0               ; tab==0 -> join (folded)
149b2a: cmp  r0, #3
149b2c: bne.n 149b44
149b2e: ldr  r0, [pc, #336]
149b30: add  r0, pc
149b32: ldr  r0, [r0, #0]
149b34: ldr  r0, [r0, #0]
149b36: blx  getShip               ; getShip FIRST
149b3a: ldr  r1, [r4, #104]
149b3c: ldr  r1, [r1, #12]
149b3e: cmp  r0, r1                ; getShip == ship
149b40: beq.w 14a6d0
```

So the residual is exactly three coupled differences:

1. **`tab==0` edge target**: ours threads it to the join; the original keeps it
   on the `tab==1` test (`cbz` to `0x149b96`).
2. **evaluation order**: the original loads `selectedItem->ship` *before*
   `getShip()`; ours calls `getShip()` first.
3. **no reload**: because ours keeps `this` in r4 across the call, the original's
   `ldrd r5,r4` reload at `0x149b3c` is absent.

The body (`0x149b44`..) is instruction-identical in both.

## New evidence 1: the fold is Jump Threading, and the block is 5 insns

`-mllvm -print-after-all` on the landed source (scratch `/dev/shm/w7656/pa.txt`)
shows the ship switch is formed correctly and only folded at **Jump Threading**:

- `Simplify the CFG` (first): `switch i32 %91, label %1742 [ i32 0 -> %1778,
  i32 3 -> %1734 ]`; `%1778` is the upgrade test.
- `Early CSE w/ MemorySSA` (immediately before Jump Threading):
  `switch i32 %91, label %1775 [ i32 0 -> %1819, i32 3 -> %1767 ]`.
- `Jump Threading`: `i32 0 -> %2024` (the `buttonOffset` join).

The threaded block `%1819` is exactly 5 real instructions:

```
%1821 = getelementptr this, 0, 84      ; upgradeMode
%1822 = load i8
%1823 = icmp ne i8 %1822, 0
%1824 = icmp eq i32 %91, 1             ; tab == 1
%1825 = and i1 %1823, %1824
br i1 %1825, label %1826, label %2021
```

`-jump-threading-threshold` defaults to 6 and the pass threads when
`NumInst < threshold`; 5 < 6, so it threads. This confirms the wave-127
correction and gives a concrete lever: the block must reach 6 instructions
before Jump Threading.

## New evidence 2: redundant conditions do NOT grow the block (they fold early)

Every attempt to push `%1819` to 6 instructions by adding a *semantically
redundant* comparison was folded before Jump Threading (all kept 9700 B and
94.747001 %, fold intact):

`tab == 1 && tab != 2`, `&& tab < 2`, `&& tab >= 1`, `&& tab != 0`,
`&& tab != 3`, `&& tab <= 1`, `&& tab > 0`, `&& tab < 3`, `&& (tab & 1) == 1`,
`&& (tab ^ 0) == 1`, `&& (tab + 0) == 1`, `&& tab * 1 == 1`, `&& (tab | 0) == 1`,
`&& tab == 1`.

`&& (tab & 1) == 1` and `&& this->viewMode == 0` do grow the final code
(9716 B) and reallocate (72.7 % / 82.4 %) but still fold, because the extra
instructions land in a *separate* short-circuit block, not in `%1819`.

## New evidence 3: the evaluation order and the register pair are coupled

The base form `this->selectedItem->ship == Globals::status->getShip()` has the
original's evaluation order but a **+1 register pair** (`ldrd r6,r5`:
layout=r6, this=r5) and no reload (94.388529 %). The flipped form has the
original's pair (`ldrd r5,r4`) but reversed order (94.747001 %). This is the
wave-127 trade-off, re-confirmed.

A `Ship *` local for the *right* operand changes the coupling:

```cpp
if (tab != 0) {
    if (tab == 3) {
        Ship *cur = Globals::status->getShip();
        if (this->selectedItem->ship == cur) goto ship_done;
    }
    ...body...
}
```

gives `ldrd r5,r4` (correct pair) **and** `cmp r1,r0` (ship vs getShip, correct
order), but the call is hoisted before the `tab==3` test, the ship value lands
in r1 (not r4), there is still no reload, and the extra block perturbs the
`switch (tab)` jump table: **93.120088 %, 9700 B** (3626/3627). The ship-branch
diff is still the same 4 ops, so this is a net loss.

`bool skip = tab == 3 && ship == getShip(); if (!skip) body;` gives the correct
`ldrd r5,r4` and `cmp r1,r0` too but shrinks the function to **9696 B,
89.670390 %**. Placing the `ship_done:` label at the true join (after the
`buttons[0]` block) instead of after the upgrade block loses the `ldrd`
entirely (`ldr r2,[sp,#136]; ldr r3,[sp,#132]`) and drops to **9704 B,
86.757613 %**.

## New evidence 4: the blueprint `ldrd sl,r9` swap is not expression-orderable

Every blueprint-branch comparison/expression reorder folds to the same IR and
leaves `ldrd r9,sl` unchanged (all 94.747001 %, 9700 B):

`0.0f < getCompletionRate()`, `craftable != 0`, `100.0f * rate`,
`(float) progressBarWidth * rate`, `(float)(progressBarWidth-4) * rate`,
`2 + textX + ...`, `1 + y + rowHeight/2`, `3 + hintOffsetX + textX + ...`.

This re-confirms the wave-124 conclusion that the swap is a Greedy tie-break,
not a source-order effect.

## New evidence 5: the free-credits `visible` tail predication

Original `0x14aeb4`:

```
ldrb.w r0, [r1, #76]     ; options[76]
subs.w r1, fp, #4        ; i - 4
it ne
movne  r1, #1            ; r1 = (i != 4)
clz    r0, r0
lsrs   r0, r0, #5        ; r0 = !options[76]
orr.w  r6, r1, r0        ; r6 = (i != 4) | !options[76]
```

Ours branches on `i != 4` instead. Forcing both sides with bitwise OR
(`(i != 4) | !options[76]`) or a ternary (`!options[76] ? true : i != 4`) grows
the function to 9704 B and drops to 92.626 / 92.776 %. The short-circuit form
in the current source is still the best 9700 B shape.

## Variants measured this wave (all reverted; 9700 B unless noted)

| variant | pct |
|---|---:|
| landed `getShip() == ship` | **94.747001** |
| base `ship == getShip()` | 94.388529 |
| `tab != 3 \|\| getShip() != ship` | 94.747001 |
| `!(tab == 3 && getShip() != ship)` | 94.719426 |
| compound `tab != 0 && (tab != 3 \|\| ship != getShip())` | 94.388529 |
| compound `tab != 0 && (tab != 3 \|\| getShip() != ship)` | 94.747001 |
| `if (tab == 0) {} else if (!(...))` | 94.609127 |
| `if (tab == 0) {} else { if (!(...)) }` | 94.609127 |
| outer `switch (tab) { case 0: break; default: ... }` | 94.609127 |
| `if (tab)` / `if (0 != tab)` / `unsigned int tab` / `uint32_t tab` | 94.747001 |
| `if (tab > 0)` / `if (tab >= 1)` | 94.609127 |
| upgrade `tab == 1 && upgradeMode` | 94.691852 |
| upgrade `1 == tab` / `upgradeMode != 0` / `tab-1 == 0` / `!(tab != 1)` / `(unsigned)tab == 1u` | 94.747001 |
| `this->selectedItem == item` (selection compare flip) | 94.691852 |
| `3 == tab` / `item->isShip() != 0` | 94.747001 |
| `Ship *cur` local + goto (pair+order right, tail perturbed) | 93.120088 |
| `bool skip` form (9696 B) | 89.670390 |
| `goto` label at true join (9704 B) | 86.757613 |
| nested/duplicated body forms (9772 B) | 84.992448 |
| `volatile int *padX` removed (3629 insns) | 87.002068 |
| padX `nullptr` / `char*` / `void*` / `unsigned*` / uninitialized / `int**` | 94.747001 |
| padX `int *volatile` / `volatile int` (9708 B) | 86.946933 |
| free-credits tail `(!options[76] \|\| i != 4)` (9704 B) | 92.776399 |
| free-credits tail bitwise OR (9704 B) | 92.625775 |

## Conclusion / open questions

The ship branch is a three-way coupling: the original's *evaluation order*
(ship before getShip) forces the ship value to live across the call, and the
original allocator then spills `this` and reuses r4, producing the reload and
the r5/r4 pair. The flipped source gets the pair by changing the live ranges
but inverts the order. The `Ship *cur` local proves the pair+order combination
is reachable in the machine code, but it hoists the call and perturbs the
`switch (tab)` jump table.

The next concrete lever is the Jump Threading threshold: the upgrade-test block
must be 6 instructions *at the Jump Threading pass*. Redundant source
comparisons fold before then. A form that adds a real but later-eliminated
instruction to that block (e.g. a load that InstCombine removes after Jump
Threading) has not been found; `-jump-threading-threshold=4` (wave 126) blocks
the fold but reallocates the whole function to 9672 B.

## Reproduction

```sh
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN12HangarWindow6renderEv
```

Scratch (not committed): `/dev/shm/w7656/{score.py,diffops.py,checkfold.py,
try*.py,HangarWindow.landed.cpp,hw.ll,pa.txt,pa_base.txt,diff_landed.txt}`.

## Continuation (same wave): exact Jump Threading cost model + new source forms

The pass-trace claim above was refined by reading the LLVM 7 `JumpThreading.cpp`
(`getJumpThreadDuplicationCost` / `ThreadEdge`):

- The cost scan starts at `getFirstNonPHI()`, so the `buttonOffset` **phi is
  free**.
- `DbgInfoIntrinsic` (the `padX` `llvm.dbg.value`) and pointer-to-pointer
  bitcasts are skipped, so they are **free** too.
- `ThreadEdge` refuses only when `cost > BBDupThreshold` (default 6).

The upgrade-test block `%1819` is therefore cost **5** (`getelementptr`,
`load i8`, `icmp ne`, `icmp eq tab,1`, `and`), not 6: the block must grow to
**7 real non-phi, non-dbg instructions** to block the fold, not 6. This is why
every single-instruction "redundant comparison" attempt (which folds before the
pass) and every `volatile` single-load attempt still threads. The `padX`
`dbg.value` cannot influence the decision at all (it is explicitly skipped).

New source forms measured this wave (all reverted; landed stays 94.747001 %,
9700 B, 3626/3627):

| variant | result |
|---|---|
| `if (tab == 1 && this->upgradeMode)` | 94.691852 %, 9700 B (fold still; only the test order changes) |
| `if (tab == 1) { if (this->upgradeMode) {...} }` | 94.691852 %, 9700 B |
| `if (this->upgradeMode) { if (tab == 1) {...} }` | 94.747001 %, 9700 B |
| `this->upgradeMode == 1 && tab == 1` / `!= 0 && tab == 1` | 94.747001 %, 9700 B |
| `tab == 1 && this->upgradeMode != 0` | 94.691852 %, 9700 B |
| `this->upgradeMode ? tab == 1 : false` | 94.747001 %, 9700 B |
| `bool(upgradeMode) & bool(tab == 1)` / `upgradeMode & (tab == 1)` | 9692 B, 70.36 % |
| `(upgradeMode != 0) == (tab == 1)` | 9692 B, 73.03 % |
| `!(tab != 1 \|\| !upgradeMode)` | 9628 B, 58.64 % |
| `*(volatile bool *)&upgradeMode && tab == 1` | 9708 B, 89.84 % |
| `upgradeMode && *(volatile bool *)&upgradeMode && tab == 1` | 9708 B, 85.94 % |
| `upgradeMode && *(volatile int *)&tab == 1` | 9664 B, 55.97 % |
| `int pad2 = 0; ... pad2 = 1;` + `&& pad2` (phi condition) | 94.747001 %, 9700 B (phi folds) |
| `int pad2 = tab != 0;` + `&& pad2` | 94.747001 %, 9700 B (folds) |
| ship branch as `switch (tab) { case 0: break; case 3: if (getShip()==ship) break; default: body; }` | 94.747001 %, 9700 B (same IR as the `if` form) |
| ship branch `switch` with duplicated body in `case 3`/`default` | 9772 B, 85.21 % |
| `Ship *sel = selectedItem->ship;` then `sel == getShip()` | 9684 B, 86.46 % |
| `Ship *volatile sel = ...` | 9696 B, 86.62 % |
| `ListItem *selItem = this->selectedItem;` then `selItem->ship == getShip()` | 9684 B, 86.62 % |
| `ListItem *selItem = item;` then `selItem->ship == getShip()` | 9684 B, 86.65 % |
| base ship form + no `padX` | 9700 B, 87.03 % (3629 insns) |
| base ship form + `volatile int padX` | 9708 B, 86.81 % |
| upgrade block moved before the ship body | 9688 B, 86.80 % |
| `padX` after the upgrade block | 9708 B, 89.82 % |
| `padX` inside `if (tab != 0)` before the ship test | 9676 B, 86.33 % |
| free-credits tail `!(i == 4 && options[76])` | 9708 B, 92.54 % |
| free-credits tail `!(i == 4) \|\| !options[76]` | 9704 B, 91.73 % |
| free-credits tail `options[76] == 0` / `== false` | 94.747001 %, 9700 B (same code) |

The `switch (tab)` form is a useful control: it produces the *same* IR switch
(`i32 0 -> upgrade, i32 3 -> ship`) and the same 94.747001 %, confirming that
the residual is not an `if`-vs-`switch` source-shape issue.

## Conclusion (updated)

Two independent facts pin the remaining ship-branch delta:

1. The original kept the `tab==0 -> upgrade-test` edge; ours threads it to the
   join because the upgrade-test block costs 5 and Jump Threading threads at
   `cost <= 6`. Blocking it from source requires **7** non-phi/non-dbg
   instructions in that block; no semantically free form found so far reaches
   that without changing the emitted code (the only 7+-instruction forms tried
   add a real volatile load and reallocate the function).
2. The original's ship-before-`getShip()` order keeps `ship` live across the
   call and lets the allocator clobber `this` (r4) and reload it; the flipped
   form gets the original `ldrd r5,r4` pair but reverses the order, and the
   base form gets the order but shifts the pair to `ldrd r6,r5`.

No source win this wave. Landed source is unchanged (`getShip() ==
selectedItem->ship`); gate stays green (byte 4506 / linked 4506 / size 4524 /
data 843 / imports 207 / lint CLEAN / drift 0).
