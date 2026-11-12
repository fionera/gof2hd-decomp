# HangarWindow::render — wave 191b (w13527): body-scope `upgTab`, LSDA 1,216 -> 586

Worktree `wt/w13527`, branch `wave191b-w13527`, base master `ce4bd3c6c`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Target `_ZN12HangarWindow6renderEv` (`0x1490d8`), 9,700 B.

## Result

Move the `tab` snapshot from above the ship guard into the guard body, and
restore the pre-w13522 upgrade order (`buttons[10]->setPosition` before
`buttons[2]->setVisible`):

```diff
-                            int upgTab = tab;
                             if (tab == 0) { } else if (__builtin_expect(tab != 3 || Globals::status->getShip() != this->selectedItem->ship, 1)) { // lint: arm_builtin
+                                int upgTab = tab;
                                 (*this->buttons)[1]->setPosition(...);
                                 (*this->buttons)[1]->setVisible(true);
                                 (*this->buttons)[1]->draw();
                                 buttonOffset = ...;
                                 if (__builtin_expect(upgTab == 1 && this->upgradeMode, 1)) { // lint: arm_builtin
-                                    (*this->buttons)[2]->setVisible(this->upgradeMode);
                                     (*this->buttons)[10]->setPosition(...);
+                                    (*this->buttons)[2]->setVisible(this->upgradeMode);
                                     (*this->buttons)[10]->setVisible(true);
                                     (*this->buttons)[10]->draw();
                                 }
```

| metric | before (w13522) | after |
|---|---:|---:|
| raw same-address diff | 928 | 1,191 |
| instruction-index alignment | 469 | 600 |
| function size | 9,700 / 9,700 | **9,700 / 9,700** |
| instructions | 3,727 / 3,734 | 3,726 / 3,734 |
| `.text` section diff | 71,268 | 71,531 |
| `.ARM.extab` section diff | 1,216 | **586** |
| `.rodata` section diff | 485 | 485 |
| `.note.gnu.build-id` | 20 | 20 |
| total differing bytes | 72,506 | **72,139** |
| render LSDA record diffbytes | 677 | **47** |
| file size | 2,187,328 | 2,187,328 |

`check` green (RATCHET PASS): byte_exact 4507/4524, linked_exact 4507/4524,
size-exact 4524/4524, stub_zero_size 0, missing 0, wrong_type 0, extra 0,
data_exact 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.
`.rel.plt` byte-identical (0), `.ARM.exidx` byte-identical (0), all section
sizes and the file size exact.

## Why it works: the hoist creates an EH scope at `getShip`

clang 7.0.2 gives a local declaration its own cleanup scope even for a
trivial `int`.  The w13522 hoist put `upgTab` in scope across the `getShip`
call, so the IR invoke unwound to the ship-guard landingpad instead of the
function-level one:

```
w13522  %1727 = invoke getShip ... unwind label %1805   (ship-guard pad)
pre     %1727 = invoke getShip ... unwind label %452    (function pad)
```

That split the LSDA call-site row at the `getShip` call (row 39 2,508/118
instead of the original 2,508/150) and shortened the row's uleb encoding by
one byte, shifting every later row.  The render record went from 38 to 677
differing bytes.

Declaring `upgTab` *inside* the guard body (after `getShip`) restores the
original unwind label, so row 39 is byte-for-byte the original's
`(2,508, 150, 9664)` and the record diff falls to 47 bytes.  Row 40 is
`(2,700, 138, 9586)` vs the original `(2,696, 140, 9562)` -- the remaining
47 bytes are the +4 guard shift and the landing-pad block order.

## Cost: the guard grows 4 B

With the body-scope declaration the guard reloads `this`/`selectedItem`
after `getShip`:

```
149b24  cmp r0,#0
149b26  beq.w 14a6d0       ; tab == 0 -> end
...
149b36  blx getShip
149b3a  ldr r4,[sp,#0x88]
149b3c  ldr r5,[sp,#0x84]
```

so the guard is 36 B instead of the original's 32 B and the body starts at
`0x149b48` (+4).  The `buttons[10]`-before-`buttons[2]` upgrade order is
4 B shorter than the original's `buttons[2]`-first order, which exactly
compensates: the function stays 9,700 B.  Keeping `buttons[2]` first gives
9,704 B (not size-exact).

## Negative results measured this wave (all size 9,700 unless stated)

* `buttons[2]` first + body-scope `upgTab`: 9,704 B; every body order
  (PVD/PDV/VPD/VDP/DPV/DVP) is 9,704 B.  The +4 is the guard, independent
  of the body order.
* `const int`, `int &`, `const int *`, `int upgTab = tab + 0`, split
  declaration: identical to the hoist (928 / 1,216).
* Hoist at the function top, view-mode block, or for-loop body: 9,700 B but
  raw 6,882 and extab 18,995 (the value is spilled, no register benefit).
* Upgrade check moved outside the guard, body-scope `upgTab` unused:
  9,676 B, extab 1,299.
* `bool sameShip` before the guard: 9,712 B.
* `switch (tab)` forms, `goto` join, `__builtin_expect(tab == 0, 0)`:
  9,684-9,712 B.
* `Ship *ship = this->selectedItem->ship;` local: 9,688-9,692 B.
* Full revert of w13522 (pre-w13522 source): 9,700 B, raw 1,038,
  `.text` 71,378, `.ARM.extab` 577, total 71,977 (net -529 vs w13522).
  That is a larger byte win but loses the original's body order
  (`setVisible`/`draw`/`setPosition`) and does not reproduce row 39 = 150.
  Its render LSDA record is still only 38 differing bytes: row 39 is
  `(2508, 172, 9664)` -- the range differs from the original's 150 but
  both encode as two uleb bytes, so the call-site table stays aligned.

## Additional sweeps (same wave, after the commit)

All size 9,700 unless stated; the target remains the impossible
`raw 928` + `extab 577` combination.

* `tab == 0u`, `tab < 1`, `if (tab == 0) { ; }`, an unused local in the
  empty arm: byte-identical to the committed variant (raw 1,191,
  extab 586).  The lone `72,138` total is a one-byte
  `.note.gnu.build-id` hash change, not a real win.
* Upgrade check without `__builtin_expect`: 9,700 B, raw 1,213,
  `.text` 71,553, `.ARM.extab` 583, total 72,158 (slightly worse).
* `if (tab != 0 && ...)`, nested `if (tab != 0) { if (...) }`,
  `if (tab == 0) {} else { ... }`, `goto upgCheck`, `switch (tab)`:
  all byte-identical to the committed variant (raw 1,191, extab 586).
* `__builtin_expect(tab == 0, 0/1)` on the zero-tab arm: 9,668-9,720 B,
  not size-exact.
* Hoisting `getShip` into a `Ship *ship` local before the `upgTab`
  declaration (conditional or unconditional): 9,696-9,704 B.
* Declaring `upgTab` before `tab` (line 495), at the top of `render()`,
  or before the `if (item->isShip())` block: falls back to the
  w13522-free 6,88x-raw shape (9,680-9,704 B).
* Duplicating the body for the `tab != 3` and `tab == 3` arms: 9,776 B.
* `volatile int upgTab`: 9,668 B.

Why `cbz` is unreachable in this shape: at `-O0`-style codegen the
`if (tab == 0) { }` empty arm branches to the join of the `if`, which is
the end of the `if (item->isShip())` statement (`14a6d0`), not the
upgrade check.  The original's `cbz r0, 149b96` targets the check, so its
`tab` value must be unprovable at the check (the check reloads the same
stack slot).  Every source form that keeps the check reachable from
`tab == 0` lets SimplifyCFG thread it to `14a6d0` and emit
`cmp+beq.w` (+4 B).  The `+4` is the only reason row 40 starts at
2,700 instead of 2,696.

## Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN12HangarWindow6renderEv
python3 _work/reconstructions/extab_lsda_decode.py \
        /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
        cmake-build-match/libgof2hdaa.so
cmake --build cmake-build-match --target check -j2
```

Scratch (not committed): `/home/debian/GalaxyOnFire2/scratch/w13527/`
(`measure.sh`, `secdiff.py`, `rowmap.py`, `rawd2.py`, `sweep*.py`).
