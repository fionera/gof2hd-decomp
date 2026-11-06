# `HangarWindow::render` — wave 123 (w7313): free-credits `visible` as one expression

Target: `_ZN12HangarWindow6renderEv` (`src/game/ui/HangarWindow.cpp`), original
`0x1490d8`, size `0x25e4` = 9700 B. Worktree `/home/debian/GalaxyOnFire2/wt/w7313`,
base `1c8c9b20e` (wave-122 final snapshot; `render` 94.043023 %).

## Result

| metric | before (HEAD) | landed |
|---|---:|---:|
| `render` verify pct | 94.043023 % (report 94.0) | **94.057631 %** (report 94.1) |
| `render` size | 9700 | 9700 |
| normalized insns | 3626 / 3626 | 3626 / 3627 |
| raw objdump matched / diff | 3479 / 316 | 3489 / 287 |
| `.ARM.extab` record | 948 B (exact) | 948 B (exact) |
| byte_exact / linked_exact | 4506 / 4506 | 4506 / 4506 |
| size-exact / data / imports | 4524, 843, 207 | same |
| lint / drift | CLEAN / 0 | CLEAN / 0 |

`check`: **RATCHET PASS** — `verify: byte 4506 linked 4506 stubs 0 missing 0 extra 0
avg 99.94`; `data: exact 843 of 843`; `sodiff: imports 207`; lint `CLEAN ✓`;
drift `0 field(s)`. `exidx_table.py --diff` reports no differing extab record.

## Source change (free-credits block)

Old shape (wave-122):

```cpp
bool visible;
if (i == 0 || (Globals::options[73] && i == 1) || (Globals::options[74] && i == 2) ||
    (Globals::options[75] && i == 3)) {
    visible = false;
} else {
    visible = i != 4 || !Globals::options[76];
}
```

New shape:

```cpp
bool visible = !(i == 0 || (Globals::options[73] && i == 1) ||
                 (Globals::options[74] && i == 2) || (Globals::options[75] && i == 3)) &&
               (i != 4 || !Globals::options[76]);
```

The two forms are semantically identical; the single-expression form makes clang
7.0.2 lower the `i != 4 || !options[76]` arm as a branch (`cmp i,#4; bne ...`)
instead of the if/else's `subs/it ne/movne/clz/lsrs/orr` predicated sequence.
That changes the length of the free-credits else-block and re-aligns the
following cleanup landing pads / literal pool, which is where the +10 matched
normalized instructions come from. (The original's own else-block at
`0x14aeb4` is the predicated `subs/it ne/...` form; the win is not a local
opcode match but a better global alignment of the shifted tail.)

## Variants measured this wave (all on the wave-122 base unless noted)

| variant | pct | size |
|---|---:|---:|
| baseline (wave-122) | **94.043023** | 9700 |
| `visible` single expression (landed) | **94.057631** | 9700 |
| `visible` ternary `? false : (...)` | 94.057631 | 9700 |
| `visible` `i != 0 && !(...) && ...` | 94.057631 | 9700 |
| `i`-first in each `&&` (`i == 1 && options[73]`) | 94.043023 | 9700 |
| `i == 0` moved last in the `||` chain | 90.728751 | 9720 |
| `visible` `(i != 4 || ...) && !(...)` (&& swapped) | 91.289967 | 9708 |
| `visible` bitwise `&` | 90.691270 | 9720 |
| `visible` `!(... || (i == 4 && options[76]))` | 91.337931 | 9696 |
| non-volatile `int textY` | 75.495050 | 9672 |
| `i == 0 ? 3400 : i + 112` (select polarity) | 94.043023 | 9700 |
| `textX` rowHeight-first / baseX-first / width-last | 94.043023 | 9700 |
| `width`/`height` declaration swap | 91.330117 | 9708 |
| `height` before `width` | 73.946571 | 9668 |
| `String price` before `String description` | 93.932708 | 9700 |
| `String description; String price;` (default ctor) | 91.032009 | 9692 |
| landed + `String price` first | 93.947332 | 9700 |
| landed + `textX` rowHeight-first | 94.057631 | 9700 |

### Blueprint branch (RA tie at `0x1496da`)

All neutral (94.043023) at the wave-122 base: x-expression layout-first,
y-expression rowHeight-first, `Layout *lay = Globals::layout;` /
`HangarWindow *self = this;` at the top of the branch (both 88.937931, 9704 B,
because the local re-allocates `this` out of `fp`).

### Buy-credits `Globals::layout` local

`Layout *layout = Globals::layout;` before `drawFooter()` and used only in the
buy-credits block: 69.151139 (9604 B). Inside the buy-credits block: 73.165328
(9564 B). Free-credits-only: 72.562234 (9668 B). All re-allocate the whole
function (`this` leaves `fp`), so none is landable.

## Remaining residual (root cause)

Everything before `0x1496da` is instruction-identical. The residual is still
the wave-121/122 set of register-allocation / block-placement ties:

1. `0x1496da` blueprint branch: orig `ldrd sl,r9,[sp,#132]` (`sl`=&Globals::layout,
   `r9`=this) vs ours `ldrd r9,sl` — cascades ~30 insns through `0x149840`.
   The pair {r9, r10} is fixed; only the assignment order differs. LLVM's
   callee-saved allocation order puts r9 after r10, so the vreg whose live
   interval is processed first gets r10. Ours processes the `this` reload first
   (`%3338` before `%3361` in the GRA MIR), the original must have processed the
   `layout` reload first.
2. `0x149b20` ship branch: orig `ldrd r5,r4` + `cbz r0,149b96`; ours `ldrd r6,r5`
   + `cmp r0,#0; beq.w 14a6d0` (jump threading of the `tab == 0` edge to the
   `upgradeMode && tab == 1` test).
3. Buy-credits half `0x14a8xx-0x14ac7x`: orig keeps `Globals::layout` in `sl`
   from `0x14a85a` to `0x14ab02` and puts the `sp+312` String temp in `r6`;
   ours spills layout to `[sp,#128]` and uses `sl` for the String temp.
4. Free-credits tail `0x14ad2c-0x14ae8x`: orig keeps `sl`=width, `r9`=rowHeight,
   `r6`=Canvas; ours spills width to `[sp,#116]` and keeps `sl`=rowHeight.
   `volatile int textY` is still required to keep the 376-byte frame and the
   `visible`/`&options` register roles; dropping it scores 75.5 (9672 B).
5. Cleanup landing pads shifted by the above (identical code, different offsets).

## Reproduction

```sh
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
cmake --build cmake-build-match --target drift -j6      # 0 fields
cmake --build cmake-build-match --target lint           # CLEAN
python3 tools/verify/verify.py --show _ZN12HangarWindow6renderEv
```

Scratch (not committed): `/dev/shm/w7313/score.py` (single-function difflib
scorer), `/dev/shm/w7313/diff.py`, `/dev/shm/w7313/batch*.py` (variant sweeps),
`/dev/shm/w7313/HangarWindow.vis.cpp` (landed source snapshot).
