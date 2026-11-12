# HangarWindow::render — wave 189 (w13320): extab-aware full-body pass

Worktree `wt/w13320`, branch `wave189-w13320`, base master `1c901e8b0`
(wave-189 harvest).  Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Target `_ZN12HangarWindow6renderEv` (`0x1490d8`), 9,700 B, 3,734 insns.

## Result

**No size-exact text+extab win found this wave.**  `src/game/ui/HangarWindow.cpp`
is left at the wave-188 base.  This note records the `.ARM.extab` anatomy of
the w13302 candidate (the reason its 266-byte `.text` win is a net loss), a
concrete target for recovering it, and the negative results of a broad
source-shape sweep.

Entry state (wave-188 body), re-measured on this branch:

| metric | value |
|---|---:|
| raw same-address diff | 1,510 |
| instruction-index alignment | 643 |
| function size | 9,700 / 9,700 |
| instructions | 3,730 / 3,734 |
| `.text` section diff | 74,032 |
| `.ARM.extab` section diff | 639 |
| render's `.ARM.extab` record diff | 81 |
| `.note.gnu.build-id` | 20 |
| total differing bytes | 74,691 |
| file size | 2,187,328 |

Gate on the untouched base: `check` RATCHET PASS — byte_exact 4507/4524,
linked_exact 4507/4524, size-exact 4524/4524, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data_exact 843/843, imports 207, lint CLEAN, drift 0,
parity 5370/5370.

## w13302 candidate reproduced (exact)

The wave-176 ship-guard shape (no `__builtin_expect`, `int upgTab = tab;`
inside the ship branch, upgrade check hoisted out of the `tab != 0` block,
`setVisible` before `draw`, direct `Globals::layout->field_0x2c_rowHeight`)
reproduces exactly:

| metric | base | w13302 shape |
|---|---:|---:|
| raw | 1,510 | **1,244** |
| aligned | 643 | **481** |
| size | 9,700 | 9,700 |
| `.text` | 74,032 | **73,766** (−266) |
| `.ARM.extab` | 639 | **1,234** (+595) |
| render record diff | 81 | 676 |
| total | 74,691 | 75,020 (+329) |

So the `.text` win is real but the extab regression is larger; the candidate
is a net loss and was not landed.

## New evidence: the extab regression is a 2-byte call-site-table shift

The render LSDA record is 948 B / 156 rows in both builds.  Header fields:

| field | original | base | w13302 |
|---|---:|---:|---:|
| LPStart enc | 0xff | 0xff | 0xff |
| TType enc | 0xff | 0xff | 0xff |
| call-site enc | 0x01 | 0x01 | 0x01 |
| call-site table length | 927 | 927 | **926** |

A raw blob diff of the record shows the first difference at offset 19 (the
call-site length ULEB) and then **every byte from offset 253 on is shifted by
+2** in the w13302 build.  The shift — not a wholesale row rewrite — is what
turns 81 differing bytes into 676.

The rows around the ship guard explain it:

```
row 39  orig   (start 2508, len 150, pad 9664)
        base   (start 2508, len 152, pad 9664)
        w13302 (start 2508, len 118, pad 9664)
row 40  orig   (start 2696, len 140, pad 9562)
        base   (start 2674, len  26, pad 9356)
        w13302 (start 2654, len 182, pad 9514)
rows 41-44
        orig   (2854,6,9664) (2874,26,9356) (2912,4,9224) (2918,32,9328)
        w13302  identical to orig
        base    (2712,4,9224) (2718,32,9328) (2770,4,9664) (2818,140,9550)
```

Function-relative offset 2508 = `0x149aa4` (`Ship::getIndex`), 2696 =
`0x149b60`, 2654 = `0x149b36`.  The split point is the
`Status::getShip()` call in the ship guard at `0x149b36`:

* original: covered by landing pad **9664** (row 39, the `name` cleanup);
* w13302: covered by landing pad **9514** (row 40, the cleanup that destroys
  `sp+264`, `sp+240`, `sp+252` — the blueprint `String` temporaries).

That one landing-pad reassignment changes two row lengths (150→118 and
140→182), which changes the ULEB widths and shifts the rest of the call-site
table by 2 bytes.  The w13302 row *values* for rows 41-44 are in fact
**identical to the original**; only rows 39-40 and the shift differ.

Consequence: if the `getShip` call's landing pad could be forced back to 9664
while keeping the w13302 text shape, the 2-byte shift would disappear and the
record diff would collapse to roughly the two-row delta (~20 B).  That would
give `.ARM.extab` ≈ 578 (better than the base's 639) and a total of roughly
73,766 + 578 + 20 = **74,364**, i.e. a ~327-byte win.  This is the concrete
next target for the extab-aware pass.

## Second extab finding: landing-pad emission order (`#324`)

The landing-pad region (`0x14b4a0..0x14b6bc`, 52 differing bytes) differs in
one entry's position.  The `sp+324` cleanup (the `String("")` temporary of the
`Layout::drawBox` calls in the item branch, created at `0x14936c`) is emitted
**before** the six shared `b.n 14b64e` stubs in our build but **after** them in
the original.  This is the source of row 1's landing pad (base 9534 vs orig
9544) and the +4 pad deltas on rows 128-131.  Reordering that cleanup entry
would recover ~12 `.text` bytes and a handful of extab bytes, but it is driven
by EH-block emission order and was not reachable by brace/scope changes alone.

## Negative sweep (all size 9,700 unless noted)

| variant | raw | aligned | text | extab | total |
|---|---:|---:|---:|---:|---:|
| **base (wave-188)** | **1,510** | **643** | **74,032** | **639** | **74,691** |
| w13302 ship-guard shape | 1,244 | 481 | 73,766 | 1,234 | 75,020 |
| w13302 + explicit `{}` scopes (8 forms) | 1,244 | 481 | 73,766 | 1,234 | 75,019/20 |
| w13302, `!(tab==3 && ship==sel)` | 1,244 | 481 | 73,766 | 1,234 | 75,020 |
| w13302, `getShip()!=ship \|\| tab!=3` | 1,547 | 488 | 74,069 | 1,234 | 75,323 |
| w13302, `upgTab` at loop top | 6,870 | 757 | 79,392 | 19,031 | 98,441 |
| w13302, expects kept (tab-first) | 1,539 | 579 | 74,061 | 27,137 | 101,217 |
| w13302, expects kept (mode-first) | 7,295 | 2,049 | 391,521 | 27,669 | 419,210 (size 9,652) |
| w13302, setVisible/draw swapped | 1,258 | 483 | 73,780 | 1,234 | 75,033 |
| base + ship body setVisible-before-draw | 1,514 | 643 | 74,036 | 639 | 74,695 |
| base + ship body reorder + no `shipRh` | 1,521 | 662 | 74,043 | 639 | 74,702 |
| base + upgrade block 2,10,10,10 order | 1,513 | 643 | 74,035 | 639 | 74,694 |
| base + `upgradeMode` first | 1,580 | 739 | 74,102 | 650 | 74,771 |
| base + `expect` on first guard only | 1,532 | 645 | 74,054 | 642 | 74,716 |
| base + `||`-expect on second guard | 1,510 | 643 | 74,032 | 639 | 74,691 |
| base + `!(tab==3 && …)` / `and-ne` | 1,532 | 645 | 74,054 | 642 | 74,716 |
| free-credits `textX` after `textY` | 1,510 | 643 | 74,032 | 639 | 74,691 |
| free-credits `textX` before `height` | 1,510 | 643 | 74,032 | 639 | 74,691 |
| free-credits `textX` after `SetColor` | 1,510 | 643 | 74,032 | 639 | 74,691 |
| free-credits `textX` associativity (5 forms) | 1,510 | 643 | 74,032 | 639 | 74,691 |
| free-credits `textX` `long` / before `canvas` | 1,510 | 643 | 74,032 | 639 | 74,691 |
| free-credits `textY` via `float` local | 1,510 | 643 | 74,032 | 639 | 74,691 |
| free-credits `canvas` `const volatile` | 1,510 | 643 | 74,032 | 639 | 74,690 (build-id 19) |
| free-credits `textYv` non-volatile | 3,375 | 664 | 384,162 | 1,483 | 385,665 (size 9,716) |
| free-credits label local (`String&`/`String*`) | 3,560 | 707 | 381,353 | 1,470 | 382,843 (size 9,708) |
| free-credits `textX` volatile | 6,444 | 1,779 | 387,231 | 1,735 | 388,986 (size 9,716) |
| buy-list `Layout *lay` hoisted out of loop | 1,955 | 649 | 74,477 | 651 | 75,148 |
| buy-list `row`/`col` plain `int` | 1,510 | 643 | 74,032 | 639 | 74,691 |
| buy-list `x` expanded (value form) | 3,470 | 653 | 119,756 | 778 | 120,554 (size 9,696) |
| buy-list `x` `1-(count%3)` / col-first | 1,510 | 643 | 74,032 | 639 | 74,691 |
| buy-list `lay` `const` | 1,510 | 643 | 74,032 | 639 | 74,691 |
| buy-list price/description decl swap | 1,512 | 735 | 74,034 | 639 | 74,693 |
| buy-list case assignment order swap | 1,510 | 723 | 74,032 | 639 | 74,691 |
| buy-list button call order (setPos/replace/split/draw) | 1,538 | 655 | 74,060 | 639 | 74,719 |
| after-buylist `draw` before `setPosition` (b17) | 1,530 | 632 | 74,052 | 639 | 74,711 |
| after-buylist `rh` inlined at use | 1,712 | 573 | 74,234 | 639 | 74,893 |
| blueprint `bgImg`/`hintX` hoists | 7,681+ | 751+ | 385,686+ | 28,341+ | 414,047+ |
| `Image *bgImg = this->progressBarBgImage;` | compile error (`progressBarBgImage` is `int`) | | | | |

Notes:

* The base is a strong local optimum: every free-credits `textX`/`textY`
  declaration/associativity/volatile perturbation is codegen-neutral
  (1,510/643/9,700), and the buy-list `lay`/`x`/`row`/`col` knobs either stay
  neutral or change the size.
* `free-credits canvas const volatile` keeps `.text`/`.ARM.extab` identical
  and only changes the build-id hash (19 vs 20); it is not a text/extab win.
* The `after-buylist draw-before-setPosition` reorder improves the alignment
  metric (632 vs 643) but costs 20 raw bytes; it was reverted.

## Residual (unchanged in kind)

* ship/blueprint `0x1496da..0x149bf0`: systematic r9/sl role swap (257 B);
* mid `0x149bf0..0x14a8ec`: literal-pool offsets + swap fallout (119 B);
* buy-list `0x14a8ec..0x14ac74`: value-form `lay` head/tail loads + swap (583 B);
* free-credits `0x14ac74..0x14ae7e`: swap + `textX` sink (228 B);
* nlm `0x14ae7e..0x14b1d2`: literal-pool shift (70 B);
* after-buylist `0x14b1d2..0x14b4a0`: early `rh` load + swap (201 B);
* landing pads `0x14b4a0..0x14b6bc`: `#324` cleanup-entry order (52 B).

## Reproduction

```sh
ulimit -v 12000000
cmake --preset default -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs \
      -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN12HangarWindow6renderEv
python3 _work/reconstructions/extab_record_map.py
python3 _work/reconstructions/extab_lsda_decode.py \
        /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
        cmake-build-match/libgof2hdaa.so
cmake --build cmake-build-match --target check -j2
```

Scratch (not committed): `/home/debian/GalaxyOnFire2/scratch/w13320/`
(`HangarWindow.base.cpp`, `measure.sh`, `secdiff.py`, `regiondiff.py`,
`harness.py`, `sweep1.py`, `sweep2.py`, `o_*.txt`, `u_*.txt`,
`rec_orig.bin`, `rec_base.bin`, `rec_w13302.bin`).
