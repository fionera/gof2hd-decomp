# HangarWindow::render — row-39 record vs raw, second sweep (wave 171, w11714)

Worktree `wt/w11714`, branch `wave171-w11714`, base master `41ef4c8b6`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
Target `_ZN12HangarWindow6renderEv`, 9,700 B. Entry state (landed wave-170 shape):
raw **2,266**, aligned **386**, size 9,700, insn 3734/3734, `.ARM.extab` record
948 B / 156 rows, diff **93**.

## Confirmed root cause (the +4 is exactly the tab==0 branch)

The landed shape is byte-identical to the original through `0x149b24` and the
whole ship body, but the `tab == 0` edge is encoded differently:

```
orig 149b1e ldr  r0,[sp,#108]      ours 149b1e ldr  r0,[sp,#108]
     149b20 ldrd r5,r4,[sp,#132]        149b20 ldrd r5,r4,[sp,#132]
     149b24 cbz  r0, 149b96   (2 B)     149b24 cmp  r0,#0
     149b26 cmp  r0,#3                  149b26 beq.w 14a6d4   (6 B total)
     149b28 bne.n 149b44                149b2a cmp  r0,#3
                                        149b2c bne.n 149b48
```

The original branches `tab == 0` to the **near** upgrade check (`0x149b96`),
which then falls through to the far join; clang can use the 2-byte `cbz`.
Ours branches straight to the far join, so it needs `cmp + beq.w`.  The extra
4 bytes shift the ship body (start `0x149b48` vs `0x149b44`) and the whole
`.ARM.extab` call-site table (`row 39` length 154 vs 150), which is the 676-byte
record of the no-expect shapes.

The landed `__builtin_expect(tab != 0, 1)` buys the 4 bytes back (size 9,700,
row 39 = 150, record 93) but flips block placement: the switch body's `case 0`
block is hoisted to `0x149b26`, between the tab condition and the ship body, so
raw stays at 2,266.

## Why clang folds the edge

IR for the anchor-inside shape (`int upgTab = tab;` inside the if body):

```
%1723: switch i32 %93, label %1732 [ i32 0, label %1963   ; %1963 = tail
                                    i32 3, label %1724 ]
```

`%1963` is the buttons[0] tail.  The upgrade check is `%1769`:
`br i1 %203, label %1774, label %1963` with `%203 = icmp eq i32 %93, 1`.
Because `%93` is the `getCurrentTab()` SSA value, JumpThreading/SimplifyCFG
knows `tab == 0` implies the check is false and redirects the `case 0` edge to
`%1963` (the far tail).  The original's edge targets the check block, i.e. its
IR did not fold this edge.  Making the check a fresh, non-forwardable load (a
volatile read) prevents the fold but changes the whole function's codegen.

## Shapes measured (size must stay 9,700)

| shape | raw | aligned | size | insn | record |
|---|---:|---:|---:|---:|---:|
| landed (`expect(tab!=0,1)` + `expect(upgTab==1,0)`) | 2,266 | 386 | 9,700 | 3734 | **93** |
| landed, check uses `tab` instead of `upgTab` | 2,266 | 386 | 9,700 | 3734 | 93 |
| landed, `tab != 0 && (tab != 3 \|\| sel->ship != getShip())` | 2,266 | 386 | 9,700 | 3734 | 93 |
| landed, `expect(upgTab==1,0) && expect(upgradeMode,1)` | 2,276 | 392 | 9,700 | 3734 | 104 |
| landed, `expect(upgTab==1,0) && expect(upgradeMode,0)` | 2,276 | 392 | 9,700 | 3734 | 104 |
| landed, operand order `getShip() == sel->ship` | 2,264 | 623 | 9,700 | 3735 | 94 |
| anchor inside, no expects (original block order) | 6,511 | 611 | 9,704 | 3736 | 254 |
| `if (!isShip()) { switch } else { ship }` (landed expects) | 2,384 | 650 | 9,700 | 3736 | 96 |
| anchor inside + `expect(tab!=0,1)` only (no upg expect) | 6,310 | 625 | 9,696 | 3733 | 258 |
| anchor inside + `expect(tab!=0,1)` + `expect(upgradeMode,0)` | 6,356 | 626 | 9,696 | 3733 | 258 |
| explicit `switch (tab) { case 0: break; case 3: ...; default: body }` + check after | 7,661 | 794 | 9,780 | 3763 | 448 |
| `goto check` form (check after the body) | 6,556 | 621 | 9,704 | 3736 | 256 |
| check outside if, anchor inside, `*(volatile int*)&tab` | 9,086 | 2,582 | 9,648 | 3723 | 825 |
| check outside if, `*(volatile int*)&upgTab` | 7,976 | 944 | 9,692 | 3731 | 700 |
| anchor inside, check `*(volatile int*)&upgTab` inside | 7,814 | 829 | 9,700 | 3739 | 294 |
| anchor inside, `expect(tab!=0,1)` + `expect(upgTab==1,1)` | 6,516 | 620 | 9,708 | 3736 | 270 |

All shapes with the original block order (ship body first) land at 9,704 B
because of the 4-byte `cmp/beq.w`; all shapes at 9,700 B either flip the block
order (record 93, raw 2,266) or shorten the function elsewhere (9,648-9,696).

## Conclusion

No shape found in this timebox keeps the 93-byte record **and** lowers raw.
The remaining lever is still the same: make clang emit the 2-byte `cbz` to the
near upgrade check while keeping the body-local scope anchor.  The fold happens
at IR level (the check compares the same SSA value as the switch), so a
source-level fix needs the check to read a value the optimiser cannot prove is
`tab` on the `tab == 0` edge without perturbing the rest of the function.
`*(volatile int *)` read is the only spelling tried that blocks the fold, and it
costs 8-52 bytes of codegen elsewhere.

The best raw with a record <= 100 is the operand-flipped landed shape
(2,264 / 94) but aligned jumps 386 -> 623, a clear structural regression, so the
landed shape (2,266 / 386 / 93) is kept.  Tree left at HEAD; gate re-run green
(byte_exact 4507/4524, linked 4507/4524, imports 207, lint CLEAN).
