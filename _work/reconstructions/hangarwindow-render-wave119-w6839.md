# HangarWindow::render — wave 119 (w6839): dead-decl sweep + targeted shape search, no gain

Target: `_ZN12HangarWindow6renderEv` (`src/game/ui/HangarWindow.cpp`), original
`0x1490d8`, size `0x25e4` = 9700 B. Worktree
`/home/debian/GalaxyOnFire2/wt/w6839`, branch `wave119-w6839`, base `fdb68f8ee`.

Builds on `hangarwindow-render-wave119-w6819.md` (91.199559 -> 91.337281 landed).
This pass: **91.337281 -> 91.337281** (no source change landed; note only).

## Result

| metric | HEAD `fdb68f8ee` | after this pass |
|---|---:|---:|
| `render` fuzzy (fresh Elf32/Resolver, same normalisation as `verify.py`) | 91.337281 | **91.337281** |
| `render` size | 9700 | 9700 |
| normalised insns (ours/orig) | 3635 / 3626 | 3635 / 3626 |
| byte_exact / linked_exact | 4505 / 4505 | 4505 / 4505 |
| size-exact / data / imports | 4524, 843, 207 | same |
| lint / drift | CLEAN / 0 | CLEAN / 0 |
| ELF section count / file size | 27 / 2,187,328 | same |

`cmake --build cmake-build-match --target check -j6` -> **RATCHET PASS**.
No section, base, size or `.ARM.extab` change (`.ARM.extab` still 0x26290, 8 B
short: `createShip` only). `src/game/ui/HangarWindow.cpp` is byte-identical to
HEAD; the worktree is clean.

## Residual map (unchanged from w6819)

Function-relative offsets; `base = 0x1490d8`.

| region | what |
|---|---|
| `0xbd4`–`0x1766` | **1135 insns instruction-identical** |
| `0x570`–`0x592` | amount block: orig keeps `item` in `r4` across `getStationAmount`; we reload from `sp+120`. Orig loads `this`(r6) before the branch and `&Globals::layout`(r5) after the call; we `ldrd r5,r6` both before |
| `0x602`–`0x768` | blueprint block: orig `ldrd sl,r9` (layout=sl, this=r9); ours `ldrd r9,sl` |
| `0xa48`–`0xae8` | ship branch: orig `ldrd r5,r4` + `cbz r0`; ours `ldrd r6,r5` + `cmp/beq.w` (SimplifyCFG threads the `tab==0` edge past the shared `tab==1` test; same target) |
| `0xbb4`–`0xbd2` | else-arm `item` reload (`mov r0,r4` vs `ldr r0,[sp,#120]`) |
| `0x1766`–`0x1e32` | buy-credits: `&Globals::layout` wants `sl`; ours moves it to `r8` (`mov r8,sl` @0x183c, `mov sl,r8` @0x1b9e/0x1d34) and gives `sl` to the `String` temporary. Loop body otherwise instruction-identical modulo r8/sl |
| `0x1e32`–`0x240c` | free-credits tail: `&Globals::layout` wants `r8`; ours `r5` |
| `0x240c`–`0x25e4` | EH cleanup dispatch table: `b.n` entry shifts (2/16 B), ours has extra entries |

Literal pool: orig starts `0x14aa88`, ours `0x14aa8c` (+4 B, from the extra
buy-preheader `str r0,[sp,#312]` = the `volatile int pV = 0;` store) -> the
buy-block `ldr r0,[pc,#N]` / `addw r0,pc,#N` immediates differ by 4 or 28
(28 = the extra buy-block shuffles). 32 `ldr [pc,#N]` pairs + 1 `addw` pair
differ this way (~33 normalised elements, ~0.9 pt).

Call stream is identical (258 `blx` in-function; 418 across `.text`) **except**
free-credits: the original calls `getWidth` (0x14ad28) then `getHeight`
(0x14ad38); ours is reversed. Forcing the faithful order costs >= 0.6 pt.

## Ghidra findings (rea decompile) worth recording

- Buy loop is `while(true){ if (4 < uVar27) break; ... switch(uVar27 & 0x7fffffff) }`
  with **unsigned** `uVar27` — consistent with `for (count = 0; count <= 4; count++)`
  (unsigned variant scores identically).
- Each case body is `String tmp(Globals::cItemListDescription_0X); description = tmp;
  ~tmp; String tmp(Globals::cItemListPrice_0X); price = tmp;` — our
  `description = String(...)` form is faithful (the temp is at `sp+288`, description
  at `sp+312` = `r6`, price at `sp+300` = `r5`).
- Free-credits original: `iVar26 = getWidth(button); iVar16 = layout->0x2c;
  uVar31 = getHeight(button);` then `SetColor`, then
  `textX = iVar26 + baseX + iVar16`, then `textY = (int)((float)y + (float)height*0.2f)`.
- Ship branch: `if (iVar11 != 0) { if (iVar11==3 && ship==getShip()) goto ...; ... }`
  then `if (iVar11==1 && this[0x11d])` (tab-first in the binary; source operand
  order is not the lever).
- The original has **no** `pV` store: at 0x1766 it is exactly
  `movs r0,#0; str.w r0,[fp,#88]; mov r0,fp; blx render; ldr.w r0,[fp,#24];
  movs r1,#1; str.w r1,[fp,#88]; blx draw`. The `volatile int pV = 0;` store is a
  padding anchor that happens to make the EH dispatch table match better.

## Measurements (baseline 91.337281 / 9700 B / 3626:3635)

Every variant below was built with `link_so` and scored with the exact
`verify.py` pipeline (fresh `Elf32`/`Resolver`). "same" means 91.337281 / 9700 /
3626:3635.

### Equal to baseline (no codegen change)
`desc_assign` (`String description = String("")`), `buy_xy_before_switch`,
`bp_swap2` (`y + 1 + field_0x70_rowHeight/2`), `bp_revargs`,
`amt_init0` (`int right = 0, left = 0;`), `amt_decl_sep` (`int right; int left;`),
`pv_uint` (`volatile unsigned int pV`), `items_null0` (`items != 0`),
`viewmode_not` (`!this->viewMode`), `barheight_gt0` (`0 < barHeight`).

### Worse
| variant | pct | size |
|---|---|---|
| `bp_lay` (local `Layout*` in blueprint block) | 68.835140 | 9516 |
| `buy_lay` | BUILD_FAIL | — |
| `desc_default` (`String description;`) | 88.411189 | 9692 |
| `swap_decl` (price declared first) | 91.144470 | 9700 |
| `visible_first` (setVisible before Strings) | 89.296046 | 9696 |
| `no_pad3` | 88.882766 | 9696 |
| `pad3_int` (non-volatile) | 90.924115 | 9700 |
| `pad3_use` (`(void)pad3;`) | 87.346826 | 9704 |
| `no_padX` | 83.551273 | 9700 |
| `padX_int` | 74.724670 | 9724 |
| `fc_nolay` | 90.944176 | 9700 |
| `fc_wfirst` (getWidth first) | 90.723639 | 9700 |
| `fc_wfirst2` | 90.778773 | 9700 |
| `fc_lay_top` | 88.300951 | 9696 |
| `buy_rowcol_before_switch` | 87.634261 | 9700 |
| `bp_swap1` (`... + hintOffsetX + buttonInsetX`) | 90.195538 | 9700 |
| `bp_swap3` | 90.195538 | 9700 |
| `bp_rate_early` (rate before `if`) | 86.064783 | 9692 |
| `bp_lay1` (local `lay` for first DrawImage2D) | 83.562775 | 9708 |
| `buy_namedtmp` (named `String tmp`) | 63.448653 | 9796 |
| `buy_cstr_assign` (`description = Globals::cItemListDescription_0X`) | 61.863113 | 9540 |
| `buy_price_first` | 90.813937 | 9700 |
| `buy_decl_outside` | 87.827045 | 9704 |
| `amt_swap` (left before right) | 86.139023 | 9708 |
| `amt_ii1` (`Item *ii = item->item` in tab==1 arm) | 87.252203 | 9704 |
| `pv_two` / `pv_ll` | 88.999036 | 9704 |
| `pv_arr` | 87.594658 | 9708 |
| `pv_short` | 89.011292 | 9704 |
| `pad3_before_vis` | 86.890664 | 9708 |
| `pad3_before_draw` | 87.083448 | 9708 |
| `pad3_after_draw` | 86.969697 | 9704 |
| `pad3_right` | 87.055907 | 9704 |
| `padX_after_if` | 86.119526 | 9696 |
| `fc_widthtmp` / `fc_widthtmp2` | 90.751206 | 9700 |
| `fc_textx_then_h` | 90.723639 | 9700 |
| `fc_textyf_plain` / `fc_texty_direct` / `fc_texty_float` | 90.214994 | 9700 |

### Earlier variants from the w6819 note (re-measured / carried over)
Equal to baseline: `dummy_int_buy`, `count_init0`, `lay_local_buy`, `loop_lt5`,
`self_alias`, `copyinit`, `explicit_bool`, `data_after_count`, `count_unsigned`,
`rowcol_uchar`, `x_y_swap`, `extra_in_loop`, `extra_in_loop2`, `extra_buy_top`,
`extra_buy_top2`, `extra_before_for`, `extra_after_for`, `extra_two`,
`layout_use_loop`, `layout_use_loop2`, switch case reorders
`order_40123/34012/02134/43210`, `bp_inset`, `amt_item2`, `pv_split`,
`pv_before_viewmode`, `ship_up_int`, `ship_tab2`, `pre_use`.

Worse: `price_first_decl` 91.144470; `pv_none`/`pv_int`/`pv_int_unused`/`pv_ptr`
91.144470; `pv_after` 91.254648; `pre_top_rowh` 91.046832 (3634);
`bp_inset`->`bp_hint` 85.694617 (9704); `bp_bgimg` 83.562775 (9708);
`decl_after_visible` 89.296046 (9696); `setvisible_after_switch` 88.542672 (9680);
`wf_only` 90.723639 (3629); `pv_wf` 90.530669 (3629); `fc_faithful` 90.104741
(3630); `fc_novol_hfirst` 90.214994 (3630); `fc_wf_vol` 90.723639 (3629);
`fc_wf_vol_after` 90.640937 (3629); `ship_swap_if` 83.946534 (9688);
`cond_swap` 59.862259 (9684); `loop_int_decl`/`loop_ne5` 79.5/79.48 (9676);
`amt_ii` 62.517213 (9680); `dummy_str_buy` 81.896907 (9736); `pv_char` 89.011292
(9704); `ship_up_use` BUILD_FAIL.

### Dead-decl sweep (this pass)
Inserting a dead `int padF;` after every statement line 489–1072 and keeping only
object-hash changes: lines 489–515, 560–660 and 810–920 were fully swept; no
position scored above 91.337281. (Most object-hash changes are `-g` debug-info
only and link to the identical `.so`.)

## Why it is a dead end from source shape alone

The 629-element diff decomposes as 164 pure-register elements and 465 structural.
The dominant pure-register term is `r8 <-> sl` (18 x `mov r0,r8` vs `mov r0,sl`,
12 x `mov r1,r8` vs `mov r1,sl`, plus 3 reverse) = ~33 elements, all from the
buy-block `&Globals::layout` / `&String-temp` swap. The rest is instruction
scheduling around the `String` constructions and the EH cleanup table.

`-mllvm -debug-only=regalloc` (debug LLVM build at `/tmp/llvm_android_build`)
shows why: `%629` (`&Globals::layout`) and `%613` (`this`) are both long-lived and
both get **evicted** from their first choice (`%629` r10/sl by `%670`, `%613`
r11/fp by `%3206`) and end up spilled; the `ldrd sl,r9,[sp,#132]` at 0x602 is a
spiller reload, and its register order is decided by the greedy allocator's
second-round free-register choice, not by any source-level ordering we can
express. Changing the source shape in the blueprint/buy/free-credits blocks
reliably perturbs *more* of the schedule than it fixes (all 60+ variants above).

The one remaining lever with real upside is the +4 B pool shift caused by the
`volatile int pV = 0;` anchor: removing it aligns the pool but moves the EH
cleanup target by 0x10 and costs 0.19 pt (`pv_none` 91.144470). No replacement
anchor tried (`short`, `char`, `unsigned`, `long long`, array, two ints, moved
before/after) beats the current one.

## Reproduction

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
python3 tools/verify/verify.py --show _ZN12HangarWindow6renderEv
```
