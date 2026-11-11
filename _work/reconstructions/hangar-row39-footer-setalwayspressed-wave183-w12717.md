# HangarWindow::render — footer `setAlwaysPressed` plain options[78], raw 1521 -> 1514 (wave 183, w12717)

Worktree `wt/w12717`, branch `wave183-w12717`, base master `9555474d8`
(wave-182 harvest 5).  Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Target `_ZN12HangarWindow6renderEv` (`0x1490d8`), 9,700 B, 3,734 insns,
`.ARM.extab` record 948 B / 156 rows, row 39 `(2508, 150, 9664)`.

Entry state (after the first wave-183 landing `e36558aa9`, the buy-list
`x` factoring): raw **1,521**, aligned **683**, size **9,700/9,700**,
insns **3,729/3,734**, LSDA record diff **80**, row 39 exact,
`.text` **75,013**, `.ARM.extab` **646**, total **75,679**.

## Result

One source change in the footer block: read `Globals::options[78]` directly
instead of through the volatile-reinterpret cast.

```diff
-    (*this->buttons)[11]->setAlwaysPressed(!*(volatile bool *)&Globals::options[78]);
+    (*this->buttons)[11]->setAlwaysPressed(!Globals::options[78]);
```

`Globals::options` is declared `alignas(4) static bool options[100]`
(`src/game/core/Globals.h:162`), so the plain form is semantically identical
(the volatile cast only forced a reload).  The plain form makes clang emit
the original's load order in the `setAlwaysPressed` window:

```
orig  14a888 ldrb.w r1,[r2,#78]   ; options[78]
      14a88c ldr    r0,[r0,#44]   ; buttons[11]
      14a88e clz    r1,r1
      14a892 lsrs   r1,r1,#5
      14a894 blx    setAlwaysPressed
ours  14a888 ldrb.w r1,[r2,#78]   ; now matches
      14a88c ldr    r0,[r0,#44]
      14a88e clz    r1,r1
      14a892 lsrs   r1,r1,#5
      14a894 blx    setAlwaysPressed
```

(The previous volatile form emitted `ldr r0,[r1,#44]` before
`ldrb r1,[r2,#78]`, a 2-byte schedule difference that cascaded into the
`0x149830` branch-offset run.)

| metric | before | after |
|---|---:|---:|
| raw same-address diff | 1,521 | **1,514** |
| instruction-index alignment | 683 | 680 |
| function size | 9,700 / 9,700 | **9,700 / 9,700** |
| instructions | 3,729 / 3,734 | **3,729 / 3,734** |
| its `.ARM.extab` record diff | 80 | **80** |
| row 39 (orig / ours) | (2508,150,9664) | **(2508,150,9664)** |
| `.text` section diff | 75,013 | **75,006** |
| `.ARM.extab` section diff | 646 | **646** |
| `.note.gnu.build-id` | 20 | 20 |
| total differing bytes | 75,679 | **75,672** |
| file size | 2,187,328 | **2,187,328** |

`check` green (RATCHET PASS): byte_exact 4507/4524, linked_exact 4507/4524,
size-exact 4524/4524, stub_zero_size 0, missing 0, wrong_type 0, extra 0,
data_exact 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.
`.ARM.extab` size/base exact (`0x1a3b78`, 156,312 B), content unchanged
(646), `.rel.plt` byte-identical, 27/27 section headers, file size exact.

The aligned metric regresses 683 -> 680 (the footer block is now shifted
relative to the original), but raw is the primary residual metric for this
function and the `.text`/total byte counts both drop by 7.

## Other forms measured on the 1,521 base

| form | raw | aligned | size |
|---|---:|---:|---:|
| **`!Globals::options[78]` (landed)** | **1,514** | **680** | **9,700** |
| `Globals::options[78] == 0` | 1,514 | 680 | 9,700 |
| `*(volatile bool *)&Globals::options[78] ? false : true` | 1,521 | 683 | 9,700 |
| old `!*(volatile bool *)&Globals::options[78]` | 1,521 | 683 | 9,700 |

## Residual

Unchanged: the dominant residual is the `sl`/`r9` Greedy tie at
`0x1496da` (`ldrd sl,r9` vs `ldrd r9,sl`) and the buy-list `lay`
materialization (`ldr.w sl,[sl]` at `0x14a960` plus the `ldr.w sl,[sp,#128]`
restore at `0x14ac64`).  The free-credits block is shifted +4 by that
restore; its `width`/`rh` roles are swapped (orig width->sl, rh->r9; ours
width->r9, rh->sl).  The ship branch keeps the non-ship `switch` inline
(orig has it out of line at `0x149bf0`) because the row-39 LSDA requires
the two-hint compound guard.

Experiments on the 1,521 base that were neutral or worse (all reverted):
`lay` const/volatile/reference forms, `lay` removed (9,696 B), `lay`
hoisted before the buy block (9,716 B), `this`/`hw` alias at the top of
`render()` (identical codegen), `Globals::layout` local for the whole
function (9,620 B), blueprint-block field caching (`progressBarWidth`,
`progressBarHeight`, `hintOffsetX`, `field_0x70_rowHeight`), blueprint
`name` moved after the `if` (5,264-7,381), blueprint `rate` hoisted before
the `if` (7,696), grid `rh`/`top`/`gsy` declaration permutations (neutral),
grid `rh` removed (1,724) or declared at the use site (1,724-2,361),
free-credits `textX` operand orders (neutral), `visible` chain reorders
(1,525-9,708), free-credits loop `i <= 4` (neutral), buy-grid call
permutations (24: current `setPosition; draw; replaceTextKeepSize;
setSplitText` is best), and the buy `x` expanded forms (1,523-9,696).

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
