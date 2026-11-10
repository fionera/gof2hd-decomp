# HangarWindow::render — buttons-block cleanup-scope construct sweep, round 2 (wave 167, w11213)

Worktree `wt/w11213`, branch `wave167-w11213`, base master `d00057e6d`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Target: `_ZN12HangarWindow6renderEv`, 9,700 B, `.ARM.extab` record 948 B / 156 rows.

Entry gate: `byte_exact 4507/4524`, linked 4507, size 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0, parity 5370/5370, `.text` 82,898,
`.ARM.extab` 660, `.rel.plt` 0.

## Baseline on entry (unchanged wave-164 shape)

| metric | value |
|---|---:|
| raw same-address diff | 2,245 |
| instruction-index alignment | 620 |
| function size | 9,700 / 9,700 |
| its `.ARM.extab` record diff | 97 |
| rows / record bytes | 156 / 948 |
| `.rel.plt` | byte-identical |

The residual record bytes are (re-confirmed with `extab_lsda_decode.py`):

* 1 byte  — row 1 LP (9544 vs 9532), a global landing-pad shift;
* 35 bytes — rows 39-44: the non-ship `switch` body is laid out **before**
  the buttons block (ours) instead of after it (original);
* 61 bytes — rows 96-135: 2-6 byte shifts from a register-allocation
  difference (`r9` vs `r4` for the `fp` copy) in the `drawHeader`/footer
  region.

The original's ship-branch guard is

```
149b1a: cmp r0,#0
149b1c: beq.n 149bf0      ; !isShip -> switch (after the buttons block)
149b1e: ldr r0,[sp,#108]  ; tab
149b20: ldrd r5,r4,[sp,#132]
149b24: cbz r0, 149b96    ; tab==0 -> upgrade check (after buttons[1])
149b26: cmp r0,#3
149b28: bne.n 149b44      ; tab!=3 -> buttons[1]
```

i.e. the buttons block is inline, the `switch` body is out of line, and the
`upgradeMode && tab == 1` block is a **sibling** of the `if (tab != 0)`
block.  Ours fuses `tab != 0` and `!(tab == 3 && ...)` into one condition
and places the `switch` body between the guard and the buttons block.

## Constructs tried this wave (all measured; size must stay 9,700)

Every row is `raw / aligned / size / record`.

| construct around the buttons block | raw | aligned | size | record |
|---|---:|---:|---:|---:|
| baseline | 2,245 | 620 | 9,700 | 97 |
| `{ struct Guard { ~Guard() {} }; Guard g; ... }` | 2,245 | 620 | 9,700 | 97 |
| `~Guard() noexcept(false) {}` (potentially throwing) | 2,245 | 620 | 9,700 | 97 |
| `struct Guard { int x; ~Guard() {} }` + `g.x = 0` | 2,245 | 620 | 9,700 | 97 |
| file-scope `RenderScope` with out-of-line empty dtor | 2,245 | 620 | 9,700 | 97 |
| same, `noexcept(false)` out-of-line dtor | 2,245 | 620 | 9,700 | 97 |
| `std::swap(a, b)` on two dead ints | 2,245 | 620 | 9,700 | 97 |
| `for (int once = 0; once < 1; ++once) { ... }` | 7,403 | 1,872 | 9,712 | 448 |
| `bool tabIsZero = (tab == 0); (void)tabIsZero;` | 2,245 | 620 | 9,700 | 97 |
| `static void renderButtonsGuard() {}` call | 2,245 | 620 | 9,700 | 97 |
| bare nested `{ ... }` | 2,245 | 620 | 9,700 | 97 |
| `if (tab >= 0) { ... }` | 6,697 | 679 | 9,712 | 298 |
| `if (tab < 0) {} else { ... }` | 6,697 | 679 | 9,712 | 298 |
| `if (tab == 0) {} else if (!(...)) { ... }` | 6,677 | 839 | 9,708 | 266 |
| `if (tab == 0) { bool u = true; (void)u; } else if (...)` | 2,245 | 620 | 9,700 | 97 |
| `bool tabIsZero` in the `__builtin_expect` condition | 6,516 | 838 | 9,704 | 703 |
| `try { ... } catch (...) { throw; }` around buttons | 2,502 | 880 | 9,716 | 932 |
| `try { ... } catch (...) { }` around buttons | 4,044 | 816 | 9,712 | 922 |
| `try { ... } catch (...) { throw; }` around ship branch | 6,756 | 961 | 9,724 | 932 |
| `if (!item->isShip()) { switch } else { ship }` (inverted source) | 2,245 | 620 | 9,700 | 97 |
| `padX` moved before the `!(...)` condition | 6,527 | 665 | 9,704 | 495 |
| `padX` moved to the ship-branch top | 7,581 | 1,414 | 9,712 | 518 |
| `padX` after `setPosition` | 6,523 | 673 | 9,704 | 274 |
| `padX` after `draw` | 6,517 | 673 | 9,704 | 707 |
| `volatile int padX[2]` | 2,245 | 620 | 9,700 | 97 |
| `volatile int padX[1] = {0}` | 8,610 | 893 | 9,740 | 522 |
| `padX` + `padY` in the same scope | 8,666 | 1,676 | 9,740 | 522 |
| `padX` + nested `{ volatile int padY[1]; upgrade }` | 6,295 | 560 | 9,696 | 698 |
| `padX` + nested `{ volatile int padY[1]; setPosition }` | 6,284 | 560 | 9,696 | 698 |
| `(void)*(volatile int*)&this->scrollOffset` removed | 6,309 | 437 | 9,696 | 259 |
| ... removed + `volatile int padX[1]` at ship top | 7,444 | 1,445 | 9,708 | 522 |
| ... removed + second `padX2` in the body | 6,309 | 437 | 9,696 | 259 |
| volatile read of `visibleHeight` instead | 2,245 | 620 | 9,700 | 97 |
| volatile read of `iconOffsetY` instead | 2,245 | 620 | 9,700 | 97 |
| volatile read of `buttons` (pointer) instead | 6,309 | 437 | 9,696 | 259 |
| volatile read of `selectedItem` (pointer) instead | 6,309 | 437 | 9,696 | 259 |
| two volatile scrollOffset reads | 6,529 | 667 | 9,704 | 274 |
| `Array<TouchButton*> *buttons = this->buttons` alias | 8,810 | 957 | 9,672 | 752 |
| `__builtin_expect(item->isShip(), 1)` | 8,644 | 2,276 | 9,692 | 556 |
| `__builtin_expect(item->isShip(), 0)` | 8,554 | 1,250 | 9,728 | 752 |
| `__builtin_expect(!(tab==3 && ...), 1)` | 7,115 | 1,712 | 9,672 | 326 |
| `__builtin_expect(!(tab==3 && ...), 0)` | 7,531 | 815 | 9,728 | 342 |
| `__builtin_expect(tab == 3, 1)` on the `&&` RHS (wave-166 lever) | 8,558 | 730 | 9,688 | 330 |
| `switch (__builtin_expect(tab, 0))` | 5,431 | 1,188 | 9,700 | 407 |
| `upgradeMode && tab == 1` as a sibling of `if (tab != 0)` | 8,530 | 828 | 9,680 | 753 |
| ... sibling + flipped `getShip` operand order | 8,529 | 823 | 9,680 | 753 |
| ... sibling, no volatile scrollOffset read | 8,648 | 876 | 9,676 | 757 |
| ... sibling, no volatile read + flipped operand | 8,647 | 875 | 9,676 | 757 |

## Findings

1. **A trivially-destructible object does not create a cleanup scope.**  clang 7
   at `-Oz` folds `{ struct Guard { ~Guard() {} }; Guard g; ... }` away
   completely — even with `~Guard() noexcept(false)` and even when the
   destructor is defined out of line in an anonymous namespace.  The emitted
   code, the 156-row / 948-byte record and the 97-byte record diff are
   bit-identical to the bare block.

2. **A real cleanup scope (try/catch) emits code.**  `try { ... } catch (...) {}`
   adds a landing pad + handler and grows the function to 9,712 B; the record
   diff jumps to 922.  `catch (...) { throw; }` is even bigger (9,716 B).

3. **The 4-byte volatile `scrollOffset` read is a layout anchor, not a
   cleanup.**  Removing it shrinks the function to 9,696 B and the record
   diff jumps 97 -> 259; putting it back restores both.  Any volatile `int`
   read of a `this` field is equivalent (`visibleHeight`, `iconOffsetY`), but
   a volatile read of a *pointer* field is folded into the existing load and
   does not restore the 4 bytes (9,696 B).

4. **The `upgradeMode` sibling shape is faithful but 20 B short.**  It matches
   the original's `cbz tab, upgrade_check` dispatch, but the whole function
   shrinks to 9,676-9,680 B (the `cbz` saves 2 B and the fused condition
   saves more).  Recovering 20 B without disturbing the block order was not
   found.

5. **The block order is a `MachineBlockPlacement` decision that source order
   does not reach.**  Inverting the `if (item->isShip())` arms
   (`if (!item->isShip()) { switch } else { ship }`) produces bit-identical
   output.  The only lever that moves the buttons block before the `switch`
   is `__builtin_expect(tab == 3, 1)` on the `&&` RHS, and that changes the
   global register allocation (it shrinks the function to 9,688 B and the
   record diff rises to 330).

## Conclusion

No size-exact, raw-improving construct was found.  The tree is left at the
wave-164 shape (raw 2,245 / aligned 620 / size 9,700 / record 97 / 156 rows).
The remaining record bytes are the block-order swap (rows 39-44) and the
footer register-allocation shift (rows 96-135), neither of which is reachable
from a cleanup-scope construct.

## Reproduction

```sh
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN12HangarWindow6renderEv
python3 _work/reconstructions/extab_lsda_decode.py \
    /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
    cmake-build-match/libgof2hdaa.so | grep -A200 'fn 0x1490d8'
cmake --build cmake-build-match --target check -j2
```

Scratch (not committed): `/dev/shm/w11213/{measure.sh,try.py,try2.py,...,
try10.py,adiff.py,rawmap.py,HangarWindow.base.cpp}`.
