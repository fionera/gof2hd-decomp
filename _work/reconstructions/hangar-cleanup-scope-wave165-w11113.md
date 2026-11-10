# HangarWindow::render — the buttons-block cleanup scope (wave 165, w11113)

Worktree `wt/w11113`, branch `wave165-w11113`, base master `3633505f8`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Target: `_ZN12HangarWindow6renderEv`, 9,700 B, `.ARM.extab` record 948 B / 156 rows.

## Baseline on entry (wave 164 landed shape)

| metric | value |
|---|---:|
| raw same-address diff | 2,245 |
| instruction-index alignment | 620 |
| function size | 9,700 |
| its `.ARM.extab` record diff | 97 |
| total `.ARM.extab` diff | 659 |
| `.rel.plt` | byte-identical |

`check` on entry: byte_exact 4507/4524, linked 4507/4524, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.

## What the original encodes

Original rows 39-44 (start, length, landing pad; LP is relative to `0x1490d8`):

| row | original | ours |
|---|---|---|
| 39 | 2508,150,9664 | 2508,156,9664 |
| 40 | 2696,140,9562 | 2678,26,9356 |
| 41 | 2854,6,9664 | 2716,4,9224 |
| 42 | 2874,26,9356 | 2722,32,9328 |
| 43 | 2912,4,9224 | 2774,4,9664 |
| 44 | 2918,32,9328 | 2820,140,9586 |

The original's buttons block (row 40) uses LP **9562**, a thunk at `0x14b632`
that branches to `0x14b698` (destroy the `name` String at `sp+0x138` only).
It is the *unique* user of LP 9562 in the whole record.

Ours uses LP **9586** (`0x14b64a` -> `0x14b68e`), which destroys `sp+0x12c`
then `sp+0x138`.  LP 9586 is shared with row 101 (the item branch).  So our
buttons landing pad carries an extra `sp+0x12c` cleanup (the `formatCredits`
String temporary) that the original's does not.

The original's row 39 (ship preamble through `Status::getShip`) ends at
`0x149b3a`; ours ends at `0x149b40` because the non-ship `switch` (`isItem`
call at `0x149b3c`) is laid out between the ship dispatch and the buttons
block.  The original lays the non-ship switch out *after* the buttons block
(`0x149bf0`).  This block-order difference is the row-39 length delta.

## Constructs tried (all measured; size must stay 9,700)

Base: raw 2,245 / aligned 620 / size 9,700 / record 97.

| construct around the buttons block | raw | aligned | size | record |
|---|---:|---:|---:|---:|
| bare nested `{ ... }` | 2,245 | 620 | 9,700 | 97 |
| nested block + `int scopeMarker` (trivially destructible) | 2,245 | 620 | 9,700 | 97 |
| `HangarRenderScope` with empty user destructor | 2,245 | 620 | 9,700 | 97 |
| `switch (tab) { default: ... }` | 2,245 | 620 | 9,700 | 97 |
| `switch (tab) { case 0..3: default: ... }` | 2,245 | 620 | 9,700 | 97 |
| `switch (tab) { case 3: if (same) break; /*fall*/ default: ... }` | 2,245 | 620 | 9,700 | 97 |
| `while (tab == 0) { break; }` before body | 2,245 | 620 | 9,700 | 97 |
| `while (!(tab == 3 && ...)) { body; break; }` | 2,245 | 620 | 9,700 | 97 |
| `for (;;) { body; break; }` | 2,245 | 620 | 9,700 | 97 |
| `do { body } while (0)` | 2,245 | 620 | 9,700 | 97 |
| `std::swap(y, y)` before body | 2,245 | 620 | 9,700 | 97 |
| `if (bool always = true)` wrapper | 2,245 | 620 | 9,700 | 97 |
| `(void) *(volatile Array<TouchButton*> **)&this->buttons` | 2,245 | 620 | 9,700 | 97 |
| `(void) *(volatile Ship **)&this->selectedItem->ship` | 2,245 | 620 | 9,700 | 97 |
| `if (tab != 3 \|\| getShip() != ship)` (De Morgan) | 2,245 | 620 | 9,700 | 97 |
| `if (bool always = tab >= 0)` wrapper | 6,697 | 679 | 9,712 | 298 |
| `if (tab == 0) {} else if (...)` | 6,677 | 839 | 9,708 | 266 |
| `if (tab == 0) {} else if (...)` + local `bool tabIsZero` | 6,570 | 843 | 9,704 | 711 |
| `for (int once = 0; once < 1; ++once)` | 7,403 | 1,872 | 9,712 | 448 |
| `try { body } catch (...) {}` | 8,241 | 822 | 9,692 | 942 |
| `String scopeGuard;` in body | 8,663 | 930 | 9,724 | 559 |
| `static inline` helper call (inlined) | 2,245 | 620 | 9,700 | 97 |
| `__builtin_expect(item->isShip(), 1)` | 8,644 | 2,276 | 9,692 | 564 |
| local `Ship *selectedShip = selectedItem->ship` | 8,178 | 658 | 9,696 | 760 |
| upgrade block moved outside `if (tab != 0)` (expect kept) | 8,530 | 828 | 9,680 | 761 |
| upgrade block moved outside `if (tab != 0)` (expect removed) | 8,518 | 794 | 9,680 | 758 |
| `volatile int padX[1]` hoisted before `if (tab != 0)` | 6,527 | 665 | 9,704 | 499 |
| swap `==` operands (`selectedItem->ship == getShip()`) | 2,247 | 616 | 9,700 | 97 |

## Conclusion

None of the scope-only constructs changes the emitted code at all: clang 7
folds every empty cleanup scope away, so the buttons landing pad stays
shared with the item branch and keeps the `sp+0x12c` cleanup.  Every
construct that does create a real cleanup scope (`String`, `try`) or a
real branch (`if/else`, `for`, `switch` with distinct cases) changes the
size and/or the record far more than it helps.

The extra `sp+0x12c` cleanup is the `formatCredits` String temporary in the
ship price display; clang keeps its cleanup scope live through the
selected-item block even though the temporary is destroyed at `0x149a94`.
The row-39 length delta is the non-ship `switch` being laid out between the
ship dispatch and the buttons block, not a cleanup-scope artefact.

No size-exact, raw-improving shape was found.  The tree is left at the
wave-164 shape (raw 2,245 / aligned 620 / size 9,700 / record 97).
