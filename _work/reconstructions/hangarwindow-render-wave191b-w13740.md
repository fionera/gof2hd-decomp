# HangarWindow::render — wave 191b (w13740): LDRD-hint check, LSDA row-39 scope mechanism, no net win

Worktree `wt/w13740`, branch `wave191b-w13740`, base master `22c4b25f5`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Target `_ZN12HangarWindow6renderEv` (`0x1490d8`), 9,700 B.

## Result: no net-positive win landed; `src/` untouched

Baseline re-measured and left pristine:

| metric | value |
|---|---:|
| raw same-address diff | **1,040** |
| instruction-index alignment | **516** |
| function size | 9,700 / 9,700 |
| instructions | 3,727 / 3,734 |
| `.text` section diff | 72,646 |
| `.ARM.extab` section diff | **564** |
| `.ARM.exidx` / `.rodata` / `.rel.plt` | 0 / 0 / 0 |
| `.note.gnu.build-id` | 20 |
| total differing bytes | **73,230** |

HangarWindow's own `.ARM.extab` record (948 B, `0x1c30fc`..`0x1c34b0`) contributes
**42** of the 564 extab bytes; the other 522 are other functions' records.

## New evidence 1: the `ldrd` "pairing hint" lead is a dead end

The handoff hypothesis was that LLVM 7 `ARMLoadStoreOptimizer`'s
`setRegAllocationHint(RegPairEven/Odd)` flips the greedy queue order and that the
original's `ldrd sl, r9, [sp,#132]` means `this` was the *first* register.

Disassembly of both binaries shows the pair order is **identical**:

```
orig 1496da  e9dd a921  ldrd sl, r9, [sp, #132]   ; [sp+132]=&Globals::layout, [sp+136]=this
ours 1496da  e9dd 9a21  ldrd r9, sl, [sp, #132]   ; same slots, same order
```

Both list the `[sp+132]` value (`&Globals::layout`) first; only the physical
registers differ.  The pair is formed **post-RA** (preSched2 `ARMLoadStoreOptimizer`
from two spill reloads), so no pre-RA hint can be involved.  The real lever is the
`RAGreedy` queue tie-break (`RegAllocGreedy.cpp:739`: for equal priority the
*lower virtual-register number* is dequeued first), i.e. the creation order of the
two spill-reload vregs.  The first *use* order is also identical in both binaries
(`ldr r0,[layout]` @1496e0, `ldr r6,[this+244]` @1496e6, `ldr r1,[this+120]` @1496f0),
so a source reorder of the blueprint arguments is not the lever either.

Caution for future IR-level work: `hw0.ll`/`llc` do **not** reproduce the build.
Recompiling the pristine TU with `-S -emit-llvm -g0` and feeding it back through
the NDK clang 7.0.2 backend yields a **9,696 B** function (the build uses `-g` and
is 9,700 B); llc 7.0.0 reproduces neither.  All measurements below are from full
`link_so` builds.

## New evidence 2: exactly what splits LSDA row 39 (the extab blow-up)

The hoist shape (`int upgTab = tab;` before the ship guard, body `P,V,D`, upgrade
`A,B,C,E`, no upgrade `__builtin_expect`) is size-exact and raw-cheap:

```
hoist: raw 911  aligned 477  size 9700  .text 72517  extab 1202
       row 39 (2508,118,9664)   row 40 (2654,182,9538)
orig   row 39 (2508,150,9664)   row 40 (2696,140,9562)
```

`118 < 128` makes the row-length ULEB one byte instead of two, shifting the whole
call-site table by one byte (rows 0-38 are otherwise byte-identical) → +638 extab.

The split is caused by the *declaration itself*, not by `upgTab`'s value:

* `int upgTab = tab;`, `int upgTab = 0; upgTab = tab;`, `unsigned`, `int dummy = tab;`,
  even an unused `int zz = 0;` before the guard all give row 39 = 118.
* Removing the declaration entirely (`if (tab==1 && upgradeMode)` directly) gives
  row 39 = 362 and raw 6,881 (clang keeps `tab` in a register and merges the scopes).
* clang 7.0.2 pushes a `RunCleanupsScope` for the declaration; its landing pad
  (`%1764`, `cleanup` → `%2003`) starts at the `getShip` invoke and covers the body.
  The IR CFG is otherwise identical to the base (`%1719 isShip unwind %451`,
  `%1723 getShip unwind %1764`).

Consequence: with the declaration **first in the body** instead of before the guard,
the scope boundary moves to *after* `getShip` and row 39 becomes **exactly 150**
(matching the original), row-40 length 140.  But the upgrade check must then live
inside the body, which turns the `tab == 0` test into `cmp r0,#0; beq.w` (+4 B) and
the function is 9,704 B unless the body/upgrade statement order is retuned.

## Measured candidates (all size 9,700 unless noted; `total = .text + extab + 20`)

| variant | raw | aligned | `.text` | extab | total |
|---|---:|---:|---:|---:|---:|
| **base (V,X,D,P,B / upgrade B,A,C,E)** | **1,040** | **516** | **72,646** | **564** | **73,230** |
| hoist (decl before guard, PVD, ABCE) | 911 | 477 | 72,517 | 1,202 | 73,739 |
| decl-first body, X,V,D,P,B / upgrade B,E,A,C | 1,166 | 531 | 72,772 | 574 | 73,366 |
| decl-first body, X,P,V,D,B / upgrade B,E,C,A | 1,171 | 502 | 72,777 | 573 | 73,370 |
| base shape, `V,X,P,D,B` | 1,038 | 595 | 72,644 | 1,197 | 73,861 |
| base shape, `D,X,V,P,B` | 1,041 | 516 | 72,647 | 565 | 73,232 |
| base shape, `X,P,V,D,B` | 1,191 | 499 | 72,797 | 573 | 73,390 |
| L_VDP dummy decl + upgrade expect | 2,563 | 770 | 74,169 | 1,221 | 75,410 |
| `__builtin_expect(tab != 3,1) \|\| …` split | 4,604 | 694 | 80,964 | — | — |

Sweeps run this wave (all full builds, `-j2`):

* 4 ship-body orders x 24 upgrade orders for the decl-first shape (`X…`): the only
  size-9,700 rows are `X,V,D,P,B` and `X,P,V,D,B` with upgrade `B,E,A,C`/`B,E,C,A`
  (extab 573-574, `.text` 72,772-72,777 — worse than base).
* 24 upgrade orders on the base body order `V,X,D,P,B`: base `B,A,C,E` is the
  unique minimum (72,646+564); next best `B,C,A,E` = 72,647+564.
* 4 ship-body orders with `upgTab` after the first statement: `V,X,P,D,B` gives
  `.text` 72,644 (-2) but extab 1,197 (+633); `P,X,V,D,B` is size 9,704.
* Blueprint `this`-first perturbations (`int img = this->progressBarBgImage;`,
  y-first, `hintOffsetX`-first, `rate` after the call): all size 9,704.

## Conclusion

The base remains the unique size-exact **net** minimum.  The residual is still the
single `this` <-> `&Globals::layout` greedy-queue tie at `0x1496da` (blueprint body
+ downstream free-credits / buttons[17] / buy-list String temps), plus the 4-byte
`cbz` guard that the tie's register pressure keeps out of reach.  The one shape
that reproduces the original's row 39 = 150 exactly (declaration first in the body)
costs 4 B in the guard and 126 B of `.text`; the one shape that saves 129 B of
`.text` (hoist) pays 638 B of extab for the 118-byte row 39.

## Scratch

`/home/debian/GalaxyOnFire2/scratch/w13740/` — `base.cpp` (pristine), `hwtry.py`
(build+measure+LSDA helper), `lsda2.py` (ARM EHABI LSDA parser), `sweepI.py`,
`sweepA.py`, `sweepU.py`, `sweepP.py`, `final_table.py`, `vHoist*.py`, `vSweep.py`.
