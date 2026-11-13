# HangarWindow::render — wave 191b (w13767): free-credits `W R H C X Y` + volatile copy

Worktree `wt/w13767`, branch `wave191b-w13767`, base master `849795a7e`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Target `_ZN12HangarWindow6renderEv` (`0x1490d8`), 9,700 B.

## Landed win

The free-credits block was rebuilt in the original's `W R H C X Y` statement
order — `getWidth`, `rowHeight`, `getHeight`, `Globals::Canvas`, `SetColor`,
then `textX`/`textY` — with a `volatile int` **copy** (`volatile int textYv =
textY;`) feeding both `DrawString` calls (the `i==0` empty-string branch uses
`textYv + field_0x4`; the `else` branch keeps `textY + field_0x4`).

Measured on a full `link_so` build (`-j2`):

| metric | base | landed |
|---|---:|---:|
| raw same-address diff | **1,040** | **1,013** |
| instruction-index alignment | **516** | **479** |
| function size | 9,700 | 9,700 |
| instructions | 3,727 / 3,734 | 3,731 / 3,734 |
| `.text` diff | **74,270** | **74,243** |
| `.ARM.extab` diff | 561 | 561 |
| `.ARM.exidx` / `.rodata` / `.rel.plt` | 0 / 0 / 0 | 0 / 0 / 0 |
| `.note.gnu.build-id` | 20 | 20 |

Net: raw −27, aligned −37, `.text` −27, `.ARM.extab` unchanged, size exact,
`.rel.plt` byte-identical.  `avg_match` 99.93 → 99.94 (baseline locked).

This reproduces the original's free-credits call schedule at
`0x14acf4..0x14ad16` (setVisible → y → setPosition), which the `D V P` base
order left 3 instructions adrift; the `volatile int` copy (rather than a
`volatile int &` reference) is what keeps the function size-exact at 9,700.

## Additional variants measured this session (no further win)

All full `link_so` builds, `-j2`.  Base = the landed variant above.

| variant | raw | aligned | size | `.text` | extab |
|---|---:|---:|---|---:|---:|
| **landed (A_copy2)** | **1,013** | **479** | 9,700 | **74,243** | 561 |
| free-credits `setVisible→setPosition→draw` (`fc_vpd`) | 1,047 | 475 | 9,700 | 74,277 | 561 |
| `buttons[17]` `setPosition→replaceText→draw` (`b17_prd`) | 1,033 | 468 | 9,700 | 74,263 | 561 |
| ship body `P,V,D` (`ship_pvd`) | 1,164 | 462 | 9,700 | 74,394 | 570 |
| upgrade check hoisted out of ship guard (`ship_out1`) | 8,616 | 950 | **9,676** | 342,290 | −2 |
| `else` branch `textYv` (`else_tyv`) | 2,760 | 515 | **9,692** | 332,829 | 706 |
| `textYv` before `textX` (`tyv_tx`) | 1,013 | 479 | 9,700 | 74,243 | 561 |
| `textYv` separate assign (`tyv_assign`) | 1,013 | 479 | 9,700 | 74,243 | 561 |
| `textY` before `textX` (`ty_tx`) | 1,013 | 479 | 9,700 | 74,243 | 561 |

Notes:
- `fc_vpd` reproduces the original's free-credits `setVisible→y→setPosition→draw`
  order exactly at `0x14acf4..0x14ad16`, but the register-allocation tie moves
  downstream (`sl` vs `r9`, `[sp,#116]` vs `[sp,#120]`) and costs +34 raw.
- `b17_prd` improves alignment (`468`) but costs +20 raw / +20 `.text`.
- `ship_pvd` (original `P,V,D` order) shrinks row 39 below 128 B → `.ARM.extab`
  +9 (570) and raw +151.
- `ship_out1` (upgrade check moved outside the `if (tab != 0)` guard) changes
  the function size to 9,676 (−24) — gate-fatal.
- The `tyv_tx` / `tyv_assign` / `ty_tx` micro-reorderings are normalised by the
  compiler to the exact landed code.

## Residual clusters after landing (raw 1,013)

| addr | n | what |
|---|---:|---|
| `0x1496da`..`0x1497a3` | ~90 | blueprint `ldrd sl,r9` RA tie |
| `0x149b20` | 178 | ship guard (`cmp/beq.w` vs `cbz`/`ldrd` hoist) + body `V,D,P` vs `P,V,D` |
| `0x14a8dc` | ~95 | buy-list preheader `str.w r9` vs `strd r9,sl`; `bic/cmp/tbb` |
| `0x14ac52`..`0x14ae40` | ~430 | free-credits: `StringD1Ev` slot, `sl`/`r9` RA tie, `String("(+")` temp regs |
| `0x14b22c`..`0x14b2e2` | ~206 | `buttons[17]` x/y spill slots (w13740 greedy-queue tie) |
| `0x14b614`..`0x14b666` | ~60 | cleanup landing pads / buy-list jump table (LSDA-driven) |

The remaining diffs are dominated by post-RA RAGreedy queue ties and the
LSDA-row-39 length constraint; the two shapes that shrink `.text` pay more in
`.ARM.extab` (row 39 < 128 B → +638) or lose size exactness.

## Gate

`check` green: byte_exact 4508/4524, linked_exact 4508/4524, size-exact
4524/4524, stub_zero_size 0, missing 0, wrong_type 0, extra 0, data_exact
843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.
