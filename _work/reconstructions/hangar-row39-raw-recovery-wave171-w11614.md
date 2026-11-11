# HangarWindow::render — row-39 record vs raw recovery (wave 171, w11614)

Worktree `wt/w11614`, branch `wave171-w11614`, base master `50716a763`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
Target `_ZN12HangarWindow6renderEv`, 9,700 B.

## Entry state (wave-170 landed, commit 9c44b964f)

| metric | value |
|---|---:|
| raw same-address diff | 2,266 |
| aligned | 386 |
| size | 9,700 / 9,700 |
| insn | 3734 / 3734 |
| its `.ARM.extab` record | 948 B / 156 rows, diff **93** |

Landed shape: `int upgTab = tab;` inside the buttons `if` body +
`__builtin_expect(tab != 0, 1)` + `__builtin_expect(upgTab == 1, 0)` +
operand order `selectedItem->ship == getShip()`.

## Root cause of the raw regression

The landed shape's ship branch is byte-identical to the original **except one
branch**:

```
orig 149b1e ldr r0,[sp,#108]        ours 149b1e ldr r0,[sp,#108]
     149b20 ldrd r5,r4,[sp,#132]        149b20 ldrd r5,r4,[sp,#132]
     149b24 cbz  r0, 149b96             149b24 cmp r0,#0
     149b26 cmp  r0,#3                  149b26 beq.w 14a6d4   <-- far join
     149b28 bne.n 149b44                149b2a cmp r0,#3
                                        149b2c bne.n 149b48
```

The original's `tab == 0` edge is a 2-byte `cbz` to the **near** upgrade check
(`0x149b96`), which then falls through to the far join.  Our shape targets the
far join directly, so clang must use `cmp + beq.w` (6 bytes).  The extra 4
bytes shift the whole rest of the function (literal pool included), which is
what produces the ~6.5k raw diff in every "no-expect" shape.

The landed `__builtin_expect(tab != 0, 1)` buys the 4 bytes back (size 9,700)
but flips the block order: the non-ship `switch` body is laid out *between* the
getShip check and the buttons body, so raw stays at 2,266.

## Matrix measured (size must stay 9,700)

`upg` = upgTab placement (0 before if / 1 inside body / 2 none);
`op` = operand order (0 `sel==ship` / 1 `ship==sel`);
`et` = expect on `tab != 0` (0 none / 1 true / 2 false / 3 on `!(tab==3&&..)`);
`eu`/`ev` = expect on `upgTab == 1` and its value.

| upg op et eu ev | raw | aligned | size | insn | record |
|---|---:|---:|---:|---:|---:|
| 1 0 1 1 0 (landed) | 2,266 | 386 | 9,700 | 3734 | **93** |
| 1 1 1 1 0 | 2,264 | 623 | 9,700 | 3735 | 94 |
| 0 1 1 0 0 | 2,249 | 405 | 9,700 | 3734 | 460 |
| 0 1 1 1 1 | 2,258 | 457 | 9,700 | 3734 | 461 |
| 0 0 0 0 0 | 1,990 | 306 | 9,700 | 3734 | 676 |
| 0 1 0 0 0 (w11414 snapshot) | 1,970 | 294 | 9,700 | 3734 | 676 |
| 0 1 0 1 1 (w11520 operand) | 1,965 | 299 | 9,700 | 3734 | 676 |
| 0 1 3 0 0 | **1,954** | 294 | 9,700 | 3734 | 676 |
| 1 0 0 0 0 | 6,511 | 611 | 9,704 | 3736 | 254 |
| 1 1 0 0 0 | 6,506 | 583 | 9,704 | 3737 | 253 |
| 1 0 0 1 1 | 6,508 | 612 | 9,704 | 3736 | 254 |
| 1 1 0 1 1 | 6,503 | 584 | 9,704 | 3737 | 253 |
| 2 * 0 0 0 | 6,727 | 736 | 9,700 | 3736 | 718 |

Rows 40-44 for `1 0 0 0 0` are `(2700,140,9498) (2858,6,9668)
(2878,26,9360) …` — i.e. the original's rows shifted by exactly 4 bytes.
Row 39 is `(2508,154)` = original `(2508,150)` + 4.  This is the closest
structural match found: ship body first, row 40 = 140 bytes, only the 4-byte
branch and the resulting shift differ.

## Shapes that did not help

- Upgrade check moved outside the buttons `if` (with a body local): compiler
  removes 24 bytes elsewhere (size 9,676 / 9,692) and the whole function's
  register allocation shifts (first literal-pool load 8 bytes early).
- `if (tab == 0) goto check;` / `if (tab == 0) {} else if (...)`: same 9,676
  or 9,704, never 9,700.
- `int b = 1;` used as the button index instead of `buttons[1]`: size 9,676.
- `__builtin_expect(item->isShip(), 0/1)` / `!item->isShip()`: 9,652-9,676,
  raw 8.6k.
- `__builtin_expect(tab != 0, 0)`: 9,724, raw 7.6k.
- `if (tab && ...)`: 9,692, raw 6,695.

## Conclusion

No shape found in the timebox keeps the 93-byte record **and** lowers raw.
The 93-byte record is tied to the landed `expect(tab != 0, 1)` block order
(switch body first); every shape that restores the original ship-body-first
order also restores the 4-byte `cmp/beq.w` branch and lands at 9,704 B (or
9,676 B when the upgrade check is hoisted out).  The remaining lever is to
make clang emit the 2-byte `cbz` to the near upgrade check while keeping the
body-local scope split; the natural C++ spellings tried here all fold
`tab == 0` to the far join instead.  Tree left at the landed shape (raw 2,266,
record 93, 9,700 B); no commit beyond this note.
