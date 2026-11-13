# HangarWindow::render — wave 191b (w13759): free-credits schedule + buy-list switch mask, no net win

Worktree `wt/w13759`, branch `wave191b-w13759`, base master `4d0cc4910`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Target `_ZN12HangarWindow6renderEv` (`0x1490d8`), 9,700 B.

## Result: no net-positive win landed; `src/` untouched

Baseline re-measured this session on the pristine tree (all numbers from full
`link_so` builds, `-j2`):

| metric | value |
|---|---:|
| raw same-address diff | **1,040** |
| instruction-index alignment | **516** |
| function size | 9,700 / 9,700 |
| instructions | 3,727 / 3,734 |
| `.text` section diff | **74,984** |
| `.ARM.extab` section diff | **564** |
| `.ARM.exidx` / `.rodata` | 0 / 0 |
| `.rel.plt` | 10 (5-entry r_info permutation, tolerated by `check`) |
| `.note.gnu.build-id` | 20 |
| total differing bytes (`.text`+extab+20) | **75,568** |

Raw-diff distribution (base, bytes per 0x100 of function address):
`149700:55 149b00:161 14a900:80 14ac00:149 14ad00:214 14ae00:80 14b200:186 14b600:52`.
The dominant residuals are free-credits (`0x14ac00..0x14ae00`, ~443 B) and
`buttons[17]` (`0x14b200`, ~186 B).

## What was re-checked against the Thumb this wave

1. **Free-credits call/store order.**  Original:
   `setVisible` (`14acfa`) → `y` (`14ad00..14ad14`) → `setPosition` (`14ad16`) →
   `draw` (`14ad1c`) → `if (visible)` → `getWidth` (`14ad28`) → load
   `layout->field_0x2c_rowHeight` (`14ad30`) → `getHeight` (`14ad38`) →
   `SetColor` (`14ad46`) → float `textX`/`textY` (`14ad78..14ad8c`).
   Base emits `draw` before `setVisible`, loads `rh` before `getWidth`, and
   computes `textX`/`textY` before `SetColor` (spilling `textY` to `[sp,#204]`).
   Moving `draw` after `setPosition` alone costs +34 raw; moving `SetColor`
   before `textX`/`textY` drops the function to 9,692 B.
2. **`buttons[17]` list-mode order.**  Original `setPosition` (`14b2aa`) →
   `replaceTextKeepSize` (`14b2c8`) → `draw` (`14b2d4`); base emits `draw`
   first.  Fixing the order alone costs +20 raw.  The non-list `else` branch
   (`setText` → `setPosition` → `draw`, `14b3f8..14b444`) already matches.
3. **Buy-list switch mask.**  Original `bic.w r0, r9, #0x80000000;
   cmp r0, #4; bhi; tbb [pc,r0]`; base `cmp.w r9, #4; bhi; tbb [pc,r9]`.
   `switch (count & 0x7fffffff)` / `(unsigned) count & 0x7fffffff` reproduces
   the `bic` but is 9,704 B (+4) and shifts `.ARM.extab` to 1,412 diff.
   `switch ((char) count)` is also 9,704 B.
4. **`buttons[17]` x/y spill slots.**  Original spills `r3=[fp,#288]` to
   `[sp,#120]` and `r2=[Globals::w]` to `[sp,#116]`; base uses `[sp,#116]`
   and `[sp,#108]`.  This is the established `this` ↔ `&Globals::layout`
   greedy-queue tie, not source-orderable (w13740).

## Variants measured this session (full builds)

All size 9,700 unless noted.  `total = .text + extab + 20`.

| variant | raw | aligned | `.text` | extab | total |
|---|---:|---:|---:|---:|---:|
| **base (pristine)** | **1,040** | **516** | **74,984** | **564** | **75,568** |
| free-credits `setVisible→setPosition→draw` (`fc_vpd`) | 1,074 | 509 | 75,018 | 564 | 75,602 |
| `fc_vpd` + `getWidth` → `rh` → `getHeight` (`fc_vpd_wrh`) | 1,088 | 482 | 75,032 | 565 | 75,617 |
| free-credits `getWidth` → `rh` → `getHeight` only | 1,090 | 490 | 75,034 | 565 | 75,619 |
| free-credits `setVisible→draw→y→setPosition` | 1,046 | 516 | 74,990 | 564 | 75,574 |
| free-credits `setVisible→y→draw→setPosition` | 3,702 | 680 | +32 (size) | 1,437 | — |
| free-credits `SetColor` before `textX`/`textY` (+volatile ref) | 2,758 | 559 | **9,692 B** | 709 | — |
| same, plain `textY` (no volatile ref) | 8,773 | 1,373 | **9,744 B** | 1,686 | — |
| same + `getWidth`/`rh`/`getHeight` (`fc_sc_first_wrh`) | 2,760 | 514 | **9,692 B** | 709 | — |
| `textX` before `SetColor`, `textY` after | 2,758 | 559 | **9,692 B** | 709 | — |
| `buttons[17]` `setPosition→replaceText→draw` (`b17_prd`) | 1,060 | 505 | 75,004 | 564 | 75,588 |
| `buttons[17]` `setPosition→draw→replaceText` | 1,059 | 513 | 75,003 | 564 | 75,587 |
| `buttons[17]` `draw→replaceText→setPosition` | 1,224 | 539 | 75,168 | 564 | 75,752 |
| `fc_draw_after_vis` + `b17_prd` | 1,066 | 505 | 75,010 | 564 | 75,594 |
| `fc_vpd` + `b17_prd` | 1,094 | 498 | 75,038 | 564 | 75,622 |
| buy-list `switch (count & 0x7fffffff)` | 3,444 | 619 | **9,704 B** | 1,412 | — |
| buy-list `switch ((unsigned) count & 0x7fffffff)` | 3,444 | 619 | **9,704 B** | 1,412 | — |
| buy-list `switch ((char) count)` | 3,411 | 578 | **9,704 B** | 1,409 | — |

Also re-measured from the prior-wave catalogue (same base): buy-list
`String description` declared before `price` = 1,042/505/74,986/564;
`String` decls after `setVisible` = 1,120/522/**9,696**/560;
`setVisible` after the `switch` = 3,305/896/**9,708**/1,626;
`setVisible` last = 9,141/1,544/**9,652**/560;
blueprint `Layout *lay` local = 7,662/819/**9,704**/572;
`buttons[17]` x/y after `setVisible` = 8,919/1,319/**9,608**/1,721;
`buttons[17]` all locals after `setVisible` = 1,872/620/**9,688**/685.

## Conclusion

The base remains the unique size-exact **net** minimum.  Every source shape that
reproduces the original's free-credits or `buttons[17]` call order either costs
raw bytes (the register-allocation tie moves elsewhere) or changes the function
size.  The two shapes that shrink `.text` (hoist, w13740: 72,517 at the old
base; `SetColor` first: 9,692 B) pay more in `.ARM.extab` (row 39 shrinks below
128 B → +638) or lose size exactness.  No change landed.

Gate re-run on the pristine tree: `RATCHET PASS` — byte_exact 4508/4524,
linked_exact 4508/4524, size-exact 4524/4524, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data_exact 843/843, imports 207, lint CLEAN, drift 0,
parity 5370/5370.
