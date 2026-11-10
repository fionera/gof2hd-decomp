# HangarWindow::render — buttons-block cleanup-scope construct sweep (wave 166, w11113)

Worktree `wt/w11113`, branch `wave166-w11113`, base master `85c1f88df`.
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

## Finding: the buttons block already has its own (name-only) cleanup

The wave-165 note read the landed buttons landing pad as
`0x14b64a -> 0x14b68e` destroying `sp+0x12c` then `sp+0x138`.  That is a
misread: in the wave-164 build `0x14b64a` is `b.n 0x14b698`, and
`0x14b698` is `mov r4,r0; add r0,sp,#312 (name); blx String::~String;
mov r0,r4; blx _Unwind_Resume` — i.e. it destroys **only** `name`, exactly
like the original's row-40 LP 9562 (`0x14b632 -> 0x14b698`).  The two LPs
differ only in *which thunk address* is encoded, not in cleanup semantics.

The residual 97 record bytes are therefore not a cleanup-scope artefact:

* 1 byte  — row 1 LP (9544 vs 9532), a global landing-pad shift;
* 35 bytes — rows 39-44: the non-ship `switch` body is laid out **before**
  the buttons block (ours) instead of after it (original);
* 61 bytes — rows 96-135: 2-6 byte shifts from a register-allocation
  difference (`r9` vs `r4` for `fp`) in the `drawHeader`/footer region.

Both binaries carry the same LP thunk *set* (9562, 9584, 9586, 9538, …),
only the row→LP assignment is rotated by the block order: the original's
buttons row uses 9562 and row 101 uses 9586; ours uses 9586 for the buttons
row and 9562 for row 122.

## Constructs tried (all measured; size must stay 9,700)

Base: raw 2,245 / aligned 620 / size 9,700 / record 97.

| construct around the buttons block | raw | aligned | size | record |
|---|---:|---:|---:|---:|
| local `struct Guard { ~Guard() {} }; Guard guard;` | 2,245 | 620 | 9,700 | 97 |
| local `struct Guard { int x; ~Guard() {} };` + `guard.x = 0` | 2,245 | 620 | 9,700 | 97 |
| `std::swap(y, y)` | 2,245 | 620 | 9,700 | 97 |
| `std::swap(this->scrollOffset, this->scrollOffset)` | 7,399 | 839 | 9,708 | 519 |
| bare nested `{ ... }` | 2,245 | 620 | 9,700 | 97 |
| nested `{ int scopeMarker = 0; (void)scopeMarker; }` before body | 2,245 | 620 | 9,700 | 97 |
| nested `{ volatile int scopeMarker = 0; ... }` before body | 6,661 | 601 | 9,712 | 278 |
| `int padX = 0; (void)padX;` instead of `volatile int padX[1]` | 2,245 | 620 | 9,700 | 97 |
| `volatile int padX[2]` / plain `int padX[1]` | 2,245 | 620 | 9,700 | 97 |
| `for (int once = 0; once < 1; ++once)` | 7,403 | 1,872 | 9,712 | 448 |
| `if (tab == 0) {} `-style `bool tabIsZero` wrapper | 6,570 | 843 | 9,704 | 711 |
| volatile read moved after `if (tab != 0)` | 6,630 | 835 | 9,708 | 266 |
| volatile read moved inside the `!(...)` body | 2,248 | 620 | 9,700 | 97 |
| volatile read of a different near field (lastTouchY, …) | 2,245 | 620 | 9,700 | 97 |
| De Morgan `tab != 3 \|\| getShip() != ship` | 2,245 | 620 | 9,700 | 97 |
| swap `==` operands of `item == selectedItem` | 2,246 | 621 | 9,700 | 97 |
| `__builtin_expect(tab == 3, 1)` on the `&&` RHS | 8,558 | 730 | 9,688 | 330 |
| both expects, + a `getCredits()` / second volatile read | 7,492-8,559 | 735 | 9,688-9,712 | 310-330 |
| non-volatile `this->scrollOffset` read | 6,309 | 437 | 9,696 | 259 |
| `int so = this->scrollOffset; (void)so;` | 2,514 | 485 | 9,700 | 151 |

None of the scope-only constructs changes the emitted code at all (clang 7
folds every empty cleanup scope away), so the record stays 948 B / 156 rows
with diff 97.  Every construct that does change the code changes the size
and/or the record far more than it helps.

## Block-order lever (measured, not landed)

Forcing the original block order with `__builtin_expect(tab == 3, 1)` makes
rows 40-44 look like the original (buttons row 40 = 140 B, then the switch
rows), but the function shrinks to 9,688 B and raw rises to 8,558.  Adding
code to recover the 12 bytes (`getCredits()`, a second volatile read) lands
at 9,688-9,712 B and never returns to the 97-byte record.  The landed
wave-164 shape remains the record optimum.

## Conclusion

No size-exact, raw-improving shape was found.  The tree is left at the
wave-164 shape (raw 2,245 / aligned 620 / size 9,700 / record 97).
