# `Status::missionCompleted` — wave 191b (w13556): negated `hasEquipment` tail

Worktree `wt/w13556`, branch `wave191b-w13556`, base master `4c5d0b229`.
Target `_ZN6Status16missionCompletedEbbx` (`src/game/mission/Status.cpp`,
1,432 B, VMA `0xabf38`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13556`.

**Status: landed.**  Raw **75 -> 74** (-1), aligned 48 unchanged,
size **1,432/1,432** exact, 527/527 instructions.  `.text` differing
bytes 71,241 -> 71,240 (-1).  `.ARM.extab` 576 unchanged, `.ARM.exidx`
0, `.rodata` 485 unchanged, build-id 20 unchanged.  Gate: byte_exact
4507/4524, linked_exact 4507/4524, size 4524/4524, data 843/843,
imports 207, stub/missing/extra 0, lint CLEAN, drift 0, parity
5370/5370 (`RATCHET PASS`).  No baseline update (byte_exact unchanged).

## 1. What changed

The w13511 device in `case 158` was

```cpp
(void) m->getStatusValue();          // dead call
if (Globals::status->ship->hasEquipment(index, 1) < 0) { return m; }  // always false
```

with the locals declared `index, cargo, amount`.  This pass drops the
dead call and turns the always-false comparison into a real (negated)
test:

```cpp
int index = m->getProductionGoodIndex();
Array<Item *> *cargo = ship->getCargo();
int amount = m->getProductionGoodAmount();
if (Item::isInList(index, amount, cargo)) { return m; }
if (!Globals::status->ship->hasEquipment(index, 1)) { return m; }
```

The declaration order `index, cargo, amount` is load-bearing: the
faithful `index, amount, cargo` order (and every variant that keeps the
second `getProductionGoodIndex()` call) makes clang pick `r5` for
`index` and `r6` for `amount`, clobbering `ship` and forcing a stack
reload; that body is 1,436 B and raw 685.  With `cargo` second, the
negated test is the only formulation that stays size-exact at 1,432.

## 2. Why the negation

The original tail is `blx hasEquipment; b.n ac458`, where `ac458` is
the shared `cbnz r0, <return m>` test block also used by the case-8
`isInList` tail.  Our `index, cargo, amount` register allocation puts
the result in a different place, so a truthy test emits a local
`cmp; b` (1,436 B).  Testing `!hasEquipment(...)` emits the branch pair
`cmp r0,#0; bne <loop>` / `b <return m>` at the same total size as the
original, which removes the last byte of the case-158 epilogue
(`0x58c` literal-pool offset is now byte-identical).

Semantically the negation is a codegen device (the original returns `m`
when `hasEquipment` is *true*); the previous device was also
semantically dead (`< 0` on a `bool`), so this is not a regression in
fidelity of the surrounding logic.

## 3. Remaining raw bytes (74)

| offset | bytes | cause |
|---|---:|---|
| `0x09e`..`0x0a0` | 3 | `blx getType` (PLT `0x62874`) vs `blx __emutls_unregister_key` (`.rodata` `0x20b978`) |
| `0x0ba`..`0x0fd` | 24 | 40-entry TBH permutation (DP-optimal; irreducible) |
| `0x32e`..`0x36c` | 46 | case-158 register juggling (`index` in `r5` vs `r9`, `ship` stack reload) |
| `0x58c` | 1 | literal-pool PC-relative offset (consequence of case-158 body) |

The 46-byte case-158 region is pure register allocation: the original
keeps `index` in `r9`, `amount` in `r5`, `cargo` in `r2`, and `ship` in
`r6`; ours uses `r5`/`r1`/`r6` and reloads `ship` from `[sp,#16]`.
Every source variant tried (declaration order, `const`/`unsigned`/`long`,
`Ship *` locals, `bool` temporaries, nested/`== 0` conditions,
`__builtin_expect`, dead calls of 10 different `Mission` methods,
`m->getProductionGoodIndex()` vs reused `index`) leaves the allocation
unchanged or grows the function.
