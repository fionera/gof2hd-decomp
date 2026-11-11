# HangarWindow::render — buy-list x as `w/2 - (bw+gsx)*(1-col)`, raw 1523 -> 1521 (wave 183, w12717)

Worktree `wt/w12717`, branch `wave183-w12717`, base master `9555474d8`
(wave-182 harvest 5).  Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Target `_ZN12HangarWindow6renderEv` (`0x1490d8`), 9,700 B, 3,734 insns,
`.ARM.extab` record 948 B / 156 rows, row 39 `(2508, 150, 9664)`.

Entry state (wave-182 landing): raw **1,523**, aligned **681**, size
**9,700/9,700**, insns **3,729/3,734**, `.text` **75,015**,
`.ARM.extab` **646**, total **75,681**.

## Result

One source change in the buy-credits grid loop: express `x` in the
`w/2 - (buttonWidth + gridSpacingX) * (1 - col)` form instead of the
expanded `(gridSpacingX + buttonWidth) * col - buttonWidth - gridSpacingX + w/2`.

```diff
-                int x = (this->gridSpacingX + this->buttonWidth) * col - this->buttonWidth - this->gridSpacingX + Globals::w / 2;
+                int x = Globals::w / 2 - (this->buttonWidth + this->gridSpacingX) * (1 - col);
```

The two forms are algebraically identical for all `col` (0..2).  The
factored form makes clang emit the original's `mul`/`sub`/`add` schedule
instead of the `mul`+`add` pair, matching two more bytes of the buy-list
block.

| metric | before | after |
|---|---:|---:|
| raw same-address diff | 1,523 | **1,521** |
| instruction-index alignment | 681 | 683 |
| function size | 9,700 / 9,700 | **9,700 / 9,700** |
| instructions | 3,729 / 3,734 | 3,729 / 3,734 |
| `.text` section diff | 75,015 | **75,013** |
| `.ARM.extab` section diff | 646 | **646** |
| `.note.gnu.build-id` | 20 | 20 |
| total differing bytes | 75,681 | **75,679** |
| file size | 2,187,328 | **2,187,328** |

`check` green (RATCHET PASS): byte_exact 4507/4524, linked_exact 4507/4524,
size-exact 4524/4524, stub_zero_size 0, missing 0, wrong_type 0, extra 0,
data_exact 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.
`.ARM.extab` size/base exact (`0x1a3b78`, 156,312 B), content unchanged
(646), `.rel.plt` byte-identical, 27/27 section headers, file size exact.

## Other forms measured (all on the 1,523 base, all 9,700 B unless noted)

| form | raw | aligned |
|---|---:|---:|
| **`w/2 - (bw+gsx)*(1-col)` (landed)** | **1,521** | **683** |
| `w/2 - (gsx+bw)*(1-col)` | 1,521 | 683 |
| `w/2 - (1-col)*(bw+gsx)` | 1,521 | 683 |
| `w/2 - (bw+gsx)*(1-(int)col)` | 1,521 | 683 |
| `w/2 + (bw+gsx)*(col-1)` | 1,880 | 684 |
| `w/2 - bw*(1-col) - gsx*(1-col)` | 3,475 / 9,696 B | 739 |
| `(bw+gsx)*col + (w/2-gsx-bw)` | 3,479 / 9,696 B | 727 |
| old expanded `(gsx+bw)*col - bw - gsx + w/2` | 1,523 | 681 |

Applying the same factoring to the grid-tail `buttons[17]` x expression
(`w/2 - bw - gsx + (gsx+bw)*2`) is **not** size-exact (9,720 B); the
scroll-hint B x expression likewise (9,688 B).  Both were reverted.

## Residual

Unchanged: the dominant residual is the `sl`/`r9` role swap at
`0x1496da` (`ldrd sl,r9` vs `ldrd r9,sl`) and the buy-list `lay`
materialization (`ldr.w sl,[sl]` at `0x14a960` plus the `strd` spill at
`0x14a8ec`).  The free-credits block is shifted +4 by the `lay` restore
at `0x14ac64`; the free-credits `width`/`rh` roles are swapped
(orig width->sl, rh->r9; ours width->r9, rh->sl).  Adjacent-statement
swaps (whole body), distance-2/3 swaps, declaration permutations,
`volatile`/`const` qualifier sweeps, `__builtin_expect` hints and
`lay` hoists were all re-measured on the 1,523 base and none was a
size-exact win.

## Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN12HangarWindow6renderEv
cmake --build cmake-build-match --target check -j2
```
