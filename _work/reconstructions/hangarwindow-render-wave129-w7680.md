# `HangarWindow::render` — wave 129 (w7680): exact Jump-Threading cost, assume/lifetime probes

Target: `_ZN12HangarWindow6renderEv` (`src/game/ui/HangarWindow.cpp`), original
`0x1490d8`, size `0x25e4` = 9700 B. Worktree `/home/debian/GalaxyOnFire2/wt/w7680`,
base `003a9623a` (wave-129 MGame; `render` 94.747001 %, gate byte 4506 / linked 4506).

This pass: **94.747001 -> 94.747001** (raw difflib over the reconciled instruction
stream; 141 diffops, 3626/3627 insns). No source change landed; the deliverable is
this evidence record. `check` is green (byte 4506, linked 4506, size 4524, data 843,
imports 207, lint CLEAN, drift 0).

## What was already known (wave 128)

The residual ship-branch difference is three coupled facts:

1. original `149b24: cbz r0, 149b96` (case 0 -> upgrade test); ours
   `149b24: cmp r0,#0; beq.w 14a6d0` (case 0 -> join);
2. original evaluates `selectedItem->ship` before `getShip()`; ours evaluates
   `getShip()` first (the wave-127 operand flip that bought the `ldrd r5,r4` pair);
3. because of (2) the original reloads `this` (`149b3c: ldrd r5,r4,[sp,#132]`);
   ours keeps `this` in r4 and has no reload.

Wave 128 established the fold is Jump Threading and that the threaded block
`%1819` costs 5 (`getelementptr`, `load i8`, `icmp ne`, `icmp eq`, `and`), so
`5 <= BBDupThreshold(6)` and JT threads it.

## New evidence 1: pass trace re-confirmed from a clean build

`-mllvm -print-after-all` on the landed source (`/dev/shm/w7680/pa.txt`) shows the
ship switch (`switch i32 %91, label %... [ i32 0, ...; i32 3, ... ]`) is formed in
the **first** `Simplify the CFG` (before inlining) with case 0 already pointing at
the upgrade-test block. It stays there through `Remove unused exception handling
info`, `Function Integration/Inlining`, `Deduce function attributes`, `SROA` and
`Early CSE`:

| pass | case 0 target |
|---|---|
| Simplify the CFG (pre-inline) | `%1778` upgrade test |
| Function Integration/Inlining | `%1832` upgrade test |
| Early CSE w/ MemorySSA | `%1819` upgrade test |
| **Jump Threading** | **`%2024` join** |

So the fold is **Jump Threading**, not the inliner's CFG cleanup (wave-126's
one-pass-late attribution was wrong; wave-127/128's JT attribution is right).

The Early-CSE upgrade block is exactly:

```llvm
%1820 = phi i32 [...]                 ; free (phi)
call void @llvm.dbg.value(...)        ; free (DbgInfoIntrinsic)
%1821 = getelementptr this, 0, 84     ; 1
%1822 = load i8                       ; 2
%1823 = icmp ne i8 %1822, 0           ; 3
%1824 = icmp eq i32 %91, 1            ; 4
%1825 = and i1 %1823, %1824           ; 5
br i1 %1825, label %1826, label %2021
```

## New evidence 2: one intrinsic is enough to block the fold (cost model proven)

LLVM 7 `getJumpThreadDuplicationCost` (fetched `llvmorg-7.0.0`
`JumpThreading.cpp`, `/dev/shm/w7680/JumpThreading.cpp`) starts at
`getFirstNonPHI()`, skips `DbgInfoIntrinsic` and pointer bitcasts, and gives every
call +1 on top of its base unit (`IntrinsicInst` non-vector: +1, i.e. total 2).
`ThreadEdge` refuses only when `cost > BBDupThreshold` (6).

Therefore **one `llvm.lifetime.start` or `llvm.assume` in the block raises the
cost from 5 to 7 and blocks the fold.** This was confirmed empirically:

```cpp
if (__builtin_assume(tab < 5), this->upgradeMode && tab == 1) { ... }
```

produces the original's short branch:

```
149b20: ldrd r5, r4, [sp, #132]
149b24: cbz  r0, 149b92          ; case 0 -> upgrade test  (MATCHES original)
149b26: cmp  r0, #3
149b28: bne.n 149b40
...
149b32: blx  getShip
149b36: ldr  r1, [r4, #104]
149b38: ldr  r1, [r1, #12]
149b3a: cmp  r0, r1
149b3c: ldr  r0, [sp, #108]
149b3e: beq.n 149b92             ; ship-equal -> upgrade test (NOT original)
```

But it also blocks the **ship-equal** edge, which the original *does* thread to
the join (`149b40: beq.w 14a6d0`). The result is 9696 B / 90.197160 % / 248
diffops — a net loss. The same result is obtained whether the assume is written
in the condition or as a statement immediately before the `if`.

## New evidence 3: the original's threading is asymmetric, and a uniform cost bump cannot reproduce it

At Jump Threading the upgrade block has three predecessors:

* switch case 0 (`%91 == 0`) -> known false -> join,
* ship-equal (`%91 == 3`) -> known false -> join,
* body fallthrough (`%91` unknown) -> no thread.

`ProcessThreadableEdges` handles all known predecessors in one call, so a uniform
block-cost increase blocks **both** case 0 and ship-equal. The original threads
ship-equal but not case 0; that asymmetry cannot be produced by any source change
that only changes the upgrade block's cost. It requires the switch to be formed
*after* Jump Threading (so case 0 is not yet a switch edge at JT time), or a
different CFG where the ship-equal edge is already a direct edge to the join.

Attempts to delay switch formation past JT (all reverted):

| probe | result |
|---|---|
| `__builtin_assume(tab != 0);` inside `if (tab != 0)` | switch still forms in first SimplifyCFG; 94.747001 %, 9700 B |
| `int padZ = 0; (void) padZ;` inside `if (tab != 0)` | switch still forms in first SimplifyCFG; 9692 B, 89.391640 % |
| `volatile int *padX` moved from isShip entry to just before the upgrade test | case 0 already -> join at Early CSE; 9708 B, 89.816729 % |
| `volatile int *padX` moved inside `if (tab != 0)` | whole ship branch reallocates; 9676 B, 86.329149 % |
| `int padY; int *padYp = &padY; (void)padYp;` before upgrade test | 9692 B, 89.391640 % (fold intact) |
| `volatile int padY;` before upgrade test | 9692 B, 89.391640 % (SROA removes it; no lifetime marker survives) |
| `int padY[1]; (void)padY;` before upgrade test | 9692 B, 89.391640 % |

In every case the first `Simplify the CFG` still folds the two tab tests into a
switch, so Jump Threading still sees case 0 and threads it.

## New evidence 4: source-shape re-confirmation of the wave-127 trade-off

All base-order forms (`selectedItem->ship == getShip()`) still allocate
`ldrd r6,r5` (this=r5, layout=r6) and score 94.388529 % / 147 diffops:

| variant | pct / size |
|---|---|
| landed `getShip() == selectedItem->ship` | **94.747001**, 9700 |
| `selectedItem->ship == getShip()` | 94.388529, 9700 (`ldrd r6,r5`) |
| `selectedItem->ship == getShip() && tab == 3` | 94.127378, 9700 |
| `tab == 3 \|\| selectedItem->ship != getShip()` (De Morgan) | 94.388529, 9700 |
| `tab != 3 \|\| selectedItem->ship != getShip()` | 94.388529, 9700 |
| `if (tab == 3 && getShip() == ship) {} else if (tab != 0) { body }` | 94.747001, 9700 (same IR as landed) |
| same with `ship == getShip()` | 94.388529, 9700 |
| `goto` ship-skip label after the upgrade test | 9692 B, 86.529712 % |

## Conclusion / open questions

The exact cost model is now pinned: the upgrade block is 5; a single
`llvm.assume`/`lifetime.start` makes it 7 and blocks the fold. The blocker is
that the original's JT is **asymmetric** (ship-equal threaded, case 0 not), and
any uniform cost change blocks both. The next concrete lever is to make the
`tab != 0` / `tab == 3` pair survive as two branches until after Jump Threading
(so the switch is formed late), or to find a source shape where the ship-equal
edge reaches the join directly. The blueprint `ldrd sl,r9` swap, the
buy-credits `&Globals::layout` spill and the free-credits `sl`/`r9` role swap
remain untouched this wave.

## Reproduction

```sh
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN12HangarWindow6renderEv
```

Scratch (not committed): `/dev/shm/w7680/{score.py,cc.sh,cc_pa.sh,exp.py,
shipbatch.py,shipbatch2.py,assumebatch.py,HangarWindow.landed.cpp,pa.txt,
JumpThreading.cpp}`.
