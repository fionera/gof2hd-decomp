# HangarWindow::render — buttons-block cleanup-scope construct sweep, round 3 (wave 168, w11314)

Worktree `wt/w11314`, branch `wave168-w11314`, base master `631c26f0b`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Target: `_ZN12HangarWindow6renderEv`, 9,700 B, `.ARM.extab` record 948 B / 156 rows.
Entry gate: byte_exact 4507/4524, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, parity 5370/5370, `.text` 82,526, `.ARM.extab` 670,
`.rel.plt` byte-identical.

## Baseline on entry

| metric | value |
|---|---:|
| raw same-address diff | 2,245 |
| instruction-index alignment | 620 |
| function size | 9,700 / 9,700 |
| its `.ARM.extab` record diff | 97 |
| rows / record bytes | 156 / 948 |

## What was tried this wave (all measured; size must stay 9,700)

| construct around the buttons block | raw | aligned | size | record |
|---|---:|---:|---:|---:|
| baseline | 2,245 | 620 | 9,700 | 97 |
| `if (tab != 3 \|\| getShip() != ship)` (De Morgan) | 2,245 | 620 | 9,700 | 97 |
| `if (expect(tab != 0, 1) && !(...))` (fused) | 2,245 | 620 | 9,700 | 97 |
| `{ String tmp; (void)tmp; }` in buttons block | 8,663 | 930 | 9,724 | 551 |
| `{ String tmp; (void)tmp; }` nested scope | 6,723 | 732 | 9,712 | 493 |
| `String tmp;` at buttons-block top | 8,663 | 930 | 9,724 | 551 |
| `{ String tmpSel; }` in selectedItem block | 7,621 | 2,146 | 9,664 | 689 |
| `{ String tmpShip; }` in ship branch | 8,651 | 2,053 | 9,644 | 728 |
| `{ String tmpTab; }` in `if (tab != 0)` | 7,093 | 1,730 | 9,672 | 675 |
| `{ String tmpUpg; }` before upgrade block | 7,266 | 1,699 | 9,676 | 673 |
| `{ String tmpSw; }` in switch body | 7,411 | 2,891 | 9,664 | 682 |
| empty `try { } catch (...) { }` | 2,245 | 620 | 9,700 | 97 |
| `try { buttons[1] calls } catch (...) { }` | 4,016 | 660 | 9,712 | 813 |
| `try { !(...) body } catch (...) { }` | 8,241 | 822 | 9,692 | 922 |
| `if (tab != 0)` without `__builtin_expect` | 6,631 | 832 | 9,708 | 266 |
| no expect + no volatile `scrollOffset` read | 6,506 | 580 | 9,704 | 253 |
| `__builtin_expect(tab == 3, 0)` on `&&` LHS | 7,521 | 1,595 | 9,672 | 728 |
| `__builtin_expect(getShip() == ship, 0)` | 7,115 | 1,712 | 9,672 | 326 |
| `__builtin_expect(item == selectedItem, 1)` | 8,131 | 2,100 | 9,664 | 847 |
| sibling `if (upgradeMode && tab == 1)` (no expect) | 8,518 | 794 | 9,680 | 758 |
| sibling + no volatile read | 8,634 | 799 | 9,676 | 760 |
| sibling, nested `if (tab == 1) if (upgradeMode)` | 8,530 | 825 | 9,680 | 753 |

## Findings

1. **A `String` temporary does create a real cleanup scope, but it costs
   code.** Every `String`-based scope grows the function by 12-24 B and the
   record diff jumps to 493-728.  The scope is not free.

2. **The block order is a branch-weight decision, not a scope decision.**
   Dropping `__builtin_expect(tab != 0, 1)` reproduces the original's block
   order (ship dispatch → getShip check → buttons → switch body) and keeps
   156 rows, but the function grows to 9,708 B and every LP shifts by +8
   (record diff 266).  The expect is what saves the 8 B (it makes `tab == 0`
   cold, so the `cmp tab,0; beq.w` becomes `cmp tab,3; beq.n; cmp tab,0; b`),
   and it is also what moves the switch body before the buttons block.

3. **The 4-byte gap is the `tab == 0` branch.** In the original the
   `tab == 0` edge targets the nearby upgrade check (`cbz tab, 0x149b96`,
   2 B).  In every shape we can build, clang folds `tab == 0` straight to the
   join block (far away), so the branch needs `cmp+beq.w` (6 B).  The
   original's upgrade check is a *sibling* of `if (tab != 0)` with two
   predecessors (the `tab == 0` edge and the buttons fallthrough), and clang
   did not constant-fold the `tab == 0` edge through it.  We could not
   reproduce that non-fold: every sibling shape folds `tab == 0` to join.

4. **`ne_novol` is the closest structural match.**  no-expect + no volatile
   read gives the original's block order with 156 rows and a 253-byte record
   diff, but it is 9,704 B (4 B over).  The missing 4 B is exactly the
   `cmp+beq.w` vs `cbz` gap from finding 3.

## Conclusion

No size-exact, raw-improving construct was found.  The tree is left at the
wave-164 shape (raw 2,245 / aligned 620 / size 9,700 / record 97 / 156 rows).
The remaining lever is still the `tab == 0` → upgrade-check edge: if a source
shape can stop clang folding that edge to the join block, the branch becomes a
2-byte `cbz`, the size returns to 9,700, and the block order (and with it rows
39-44 and the footer `r9`/`r4` allocation in rows 96-135) should follow.

## Reproduction

```sh
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN12HangarWindow6renderEv
python3 _work/reconstructions/extab_lsda_decode.py \
    /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
    cmake-build-match/libgof2hdaa.so | grep -A200 'fn 0x1490d8'
cmake --build cmake-build-match --target check -j2
```

Scratch (not committed): `/dev/shm/w11314/{measure.sh,batch*.py,try.py,adiff.py,
HangarWindow.base.cpp,noexpect.cpp,ne_novol.cpp,sibling.cpp,...}`.
