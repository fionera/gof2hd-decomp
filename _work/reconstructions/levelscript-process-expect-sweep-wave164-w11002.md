# LevelScript::process — wave 164 / w11002: full `__builtin_expect` re-sweep

Target `_ZN11LevelScript7processEi` (`LevelScript::process`), original `0x137d18`,
**57,232 B (0xdf90)**, size-exact.  Owned TU `src/game/world/LevelScript.cpp`.
Worktree `wt/w11002`, base master `b3d3a1f68` (wave 163).  Scratch `/dev/shm/w11002/`.

## Baseline (wave-163 tree, unchanged)

```
process raw diffbytes    54,541
process aligned insns    17,427
orig / ours size         57232 / 57232   (size-exact)
.text diffbytes          84,652
.ARM.extab diffbytes     725
.rel.plt                 0        (BYTE-IDENTICAL)
byte_exact 4507 / linked_exact 4507 / size 4524 / data 843 / imports 207
```

## Method

A harness (`/dev/shm/w11002/sweep.py`, `ml_sweep.py`) wrapped every single-line and
multi-line `if (...)` condition in the whole `process()` body (lines 605-5304) in
`__builtin_expect(cond, 0)` and `__builtin_expect(cond, 1)`, one at a time, rebuilt the
full `.so` (`link_so`, NDK r18b clang 7.0.2, -Oz), and measured the function raw diff,
size, `.text`, `.rel.plt` and `.ARM.extab` after every candidate.  519 single-line and
19 multi-line `if`s = 1,076 measured builds.  `// lint: arm_builtin` is placed on the
same line as every `__builtin_expect` (multi-line conditions carry it on the
`__builtin_expect` line, not the closing-paren line).

Cells whose codegen is unchanged (hint ignored: raw == 54,541, size == 57,232,
`.rel.plt` == 0, extab == 725) are not listed individually below; they are the
complement of the 116 non-neutral rows in the JSON artifacts
(`/dev/shm/w11002/sweep_full.json`, `sweep_ml.json`, `aligned_sweep.json`).
Aligned for the neutral cells is identical to the baseline (17,427).

## Landed: 13 PLT-safe cells

| line | case | source guard | e |
|---|---|---|---:|
| L2848 | 91 | `else if (m_nState == 5)` | 1 |
| L4135 | 131 | `if (*counter >= 3001)` | 1 |
| L2493 | 70 | `if (messages[1]->isOver())` | 0 |
| L2392 | 67 | `else if (messages[11]->isOver() && m_nState == 6)` | 1 |
| L3775 | 105 | `if (player->getHitpoints() >= 1 && route/waypoint state != 0)` | 0 |
| L3069 | 92 | `else if (m_nState == 7 && messages[8]->isTriggered())` | 1 |
| L4399 | 4 | `if (m_nScriptTimerA >= 324)` | 0 |
| L1083 | 3 | `if (m_nScriptTime >= 9001)` | 0 |
| L4286 | 144 | `if (!field_0x13c && rnd->nextInt(100) <= 9)` | 1 |
| L2058 | 41 | `if (ship != nullptr && ship->shipGroup == 9)` | 0 |
| L4368 | 2 | `if (m_nScriptTimerA >= 1001)` | 1 |
| L4598 | 154 | `if (m_nScriptTimerA >= 1001)` | 0 |
| L1335 | 5 | `if (!Globals::layout->isFading())` | 1 |

Measured (full link):

| | raw | aligned | fn size | .text | .rel.plt | .ARM.extab |
|---|---:|---:|---:|---:|---:|---:|
| baseline | 54,541 | 17,427 | 57,232 | 84,652 | 0 | 725 |
| **landed** | **54,247** | **17,532** | **57,232** | **84,358** | **0** | **724** |

Raw **-294**, `.text` **-294**, aligned **+105** (indicative only; raw is the
byte-identicality ground truth).  Every section size is exact, `.rel.plt` is
byte-identical, `.ARM.extab` improves by 1.  `check` is green: byte_exact 4507/4524,
linked_exact 4507/4524, size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0.

### Per-cell contribution in the landed set (leave-one-out)

| cell | raw when dropped | delta |
|---|---:|---:|
| L3069:1 | 54,843 (+ size 57,240 fatal) | essential |
| L2058:0 | 54,673 (+ size 57,224 fatal) | essential |
| L2848:1 | 54,481 | -234 |
| L4135:1 | 54,280 | -33 |
| L2392:1 | 54,266 | -19 |
| L4399:0 | 54,257 | -10 |
| L2493:0 | 54,254 | -7 |
| L4368:1 | 54,253 | -6 |
| L1083:0 | 54,250 | -3 |
| L4286:1 | 54,249 | -2 |
| L3775:0 | 54,248 | -1 |
| L4598:0 | 54,248 | -1 |
| L1335:1 | 54,248 | -1 |

`L3069` and `L2058` are size-coupled with the rest: each alone is size-exact, but
dropping either from the full set leaves 57,240 / 57,224.

## Full tested-cell table

### Size-exact, PLT-safe, non-neutral cells (116)

| line | e | raw | aligned | extab |
|---|---:|---:|---:|---:|
| L5079 | 1 | 54477 | 19434 | 28832 |
| L2562 | 0 | 54494 | 17420 | 724 |
| L2848 | 1 | 54497 | 17352 | 723 |
| L4135 | 1 | 54503 | 17436 | 725 |
| L3401 | 0 | 54507 | 17903 | 725 |
| L3401 | 1 | 54507 | 17911 | 725 |
| L5231 | 1 | 54518 | 17514 | 726 |
| L2493 | 0 | 54534 | 17458 | 725 |
| L2392 | 1 | 54535 | 17428 | 725 |
| L3066 | 0 | 54535 | 17428 | 725 |
| L2835 | 1 | 54537 | 17428 | 725 |
| L1083 | 0 | 54538 | 17427 | 725 |
| L4626 | 0 | 54538 | 17427 | 725 |
| L2816 | 1 | 54539 | 17427 | 725 |
| L3910 | 0 | 54539 | 17434 | 725 |
| L4286 | 1 | 54539 | 17413 | 725 |
| L4920 | 1 | 54539 | 17433 | 725 |
| L1335 | 1 | 54540 | 17427 | 725 |
| L2215 | 0 | 54540 | 17430 | 725 |
| L3605 | 1 | 54540 | 17456 | 725 |
| L3702 | 1 | 54540 | 17427 | 725 |
| L3775 | 0 | 54540 | 17427 | 725 |
| L4598 | 0 | 54540 | 17411 | 725 |
| L4809 | 1 | 54540 | 17426 | 725 |
| L5125 | 0 | 54540 | 17426 | 725 |
| L2805 | 1 | 54541 | 17427 | 726 |
| L3823 | 1 | 54541 | 17427 | 724 |
| L4028 | 1 | 54541 | 17427 | 726 |
| L1436 | 0 | 54542 | 17425 | 725 |
| L1772 | 1 | 54542 | 17426 | 725 |
| L2171 | 0 | 54542 | 17428 | 725 |
| L2832 | 1 | 54542 | 17427 | 725 |
| L2912 | 1 | 54542 | 17427 | 725 |
| L3027 | 1 | 54542 | 17428 | 725 |
| L3561 | 1 | 54542 | 17429 | 726 |
| L1118 | 0 | 54543 | 17425 | 725 |
| L4210 | 0 | 54543 | 17448 | 725 |
| L4422 | 1 | 54543 | 17437 | 725 |
| L5033 | 0 | 54543 | 17474 | 725 |
| L1288 | 1 | 54544 | 17427 | 725 |
| L1294 | 1 | 54544 | 17427 | 725 |
| L3069 | 1 | 54545 | 17358 | 725 |
| L3746 | 0 | 54545 | 17429 | 725 |
| L4399 | 0 | 54545 | 17428 | 725 |
| L5033 | 1 | 54545 | 17438 | 725 |
| L5111 | 1 | 54545 | 17435 | 725 |
| L4368 | 1 | 54547 | 17427 | 726 |
| L4159 | 1 | 54548 | 17403 | 725 |
| L3518 | 0 | 54549 | 17438 | 725 |
| L4256 | 0 | 54552 | 17428 | 725 |
| L4257 | 0 | 54552 | 17428 | 725 |
| L1016 | 1 | 54553 | 17429 | 725 |
| L1638 | 0 | 54553 | 17429 | 727 |
| L3553 | 1 | 54555 | 17412 | 725 |
| L4285 | 0 | 54564 | 17446 | 725 |
| L4286 | 0 | 54565 | 17459 | 725 |
| L2038 | 0 | 54581 | 17102 | 725 |
| L4316 | 0 | 54589 | 17758 | 725 |
| L4277 | 0 | 54597 | 17449 | 725 |
| L4238 | 1 | 54603 | 17602 | 723 |
| L767 | 1 | 54614 | 17430 | 725 |
| L4091 | 0 | 54620 | 17435 | 725 |
| L4205 | 1 | 54627 | 17412 | 725 |
| L2115 | 1 | 54634 | 17434 | 725 |
| L1146 | 0 | 54638 | 17426 | 725 |
| L4102 | 1 | 54638 | 17369 | 725 |
| L1079 | 0 | 54641 | 17306 | 725 |
| L4273 | 0 | 54654 | 17435 | 725 |
| L1676 | 0 | 54655 | 17432 | 725 |
| L2705 | 0 | 54655 | 18112 | 728 |
| L2512 | 0 | 54657 | 17011 | 724 |
| L2512 | 1 | 54657 | 17011 | 724 |
| L4102 | 0 | 54660 | 17472 | 725 |
| L1111 | 1 | 54664 | 17441 | 725 |
| L4923 | 0 | 54705 | 17828 | 725 |
| L2058 | 0 | 54710 | 17431 | 725 |
| L3350 | 0 | 54715 | 17432 | 725 |
| L3316 | 1 | 54716 | 17390 | 725 |
| L4293 | 0 | 54737 | 17428 | 724 |
| L4295 | 0 | 54737 | 17428 | 724 |
| L5142 | 1 | 54737 | 17428 | 724 |
| L4804 | 1 | 54742 | 17432 | 725 |
| L2850 | 0 | 54743 | 17433 | 725 |
| L2252 | 0 | 54778 | 17436 | 724 |
| L1687 | 0 | 54795 | 18546 | 722 |
| L3700 | 1 | 54814 | 17921 | 725 |
| L3047 | 0 | 54864 | 17428 | 726 |
| L713 | 1 | 54900 | 17445 | 725 |
| L4245 | 0 | 54915 | 17658 | 726 |
| L2577 | 0 | 54929 | 17407 | 724 |
| L2617 | 0 | 54929 | 17960 | 737 |
| L1729 | 1 | 54931 | 17444 | 722 |
| L3331 | 1 | 54933 | 17867 | 729 |
| L3423 | 0 | 54933 | 17530 | 729 |
| L3421 | 0 | 54946 | 17484 | 725 |
| L1426 | 0 | 54958 | 17432 | 727 |
| L1772 | 0 | 54958 | 17449 | 724 |
| L1177 | 1 | 54971 | 17419 | 724 |
| L3203 | 1 | 54979 | 17410 | 729 |
| L4778 | 0 | 54986 | 21028 | 726 |
| L4517 | 1 | 54987 | 17398 | 725 |
| L1305 | 1 | 54989 | 17437 | 722 |
| L2230 | 0 | 55002 | 17367 | 729 |
| L2624 | 0 | 55013 | 17956 | 735 |
| L1430 | 0 | 55027 | 17418 | 726 |
| L2656 | 1 | 55067 | 17986 | 733 |
| L2456 | 1 | 55069 | 17198 | 729 |
| L1981 | 0 | 55071 | 17143 | 733 |
| L4564 | 1 | 55085 | 22147 | 728 |
| L3291 | 0 | 55087 | 18532 | 28840 |
| L2597 | 0 | 55102 | 18076 | 729 |
| L5276 | 1 | 55131 | 17811 | 726 |
| L1354 | 0 | 55186 | 17499 | 728 |
| L1731 | 0 | 55269 | 17520 | 722 |
| L5161 | 0 | 55373 | 20409 | 724 |
| L1673 | 1 | 55450 | 21969 | 742 |

### Size-changing or PLT-rotating cells (top 40 by raw)

| line | e | raw | size | rel.plt | extab |
|---|---:|---:|---:|---:|---:|
| L4720 | 0 | 54361 | 57240 | 0 | 1407 |
| L4613 | 0 | 54371 | 57240 | 0 | 1407 |
| L3751 | 1 | 54422 | 57236 | 0 | 1410 |
| L2252 | 1 | 54425 | 57248 | 0 | 29318 |
| L4610 | 0 | 54450 | 57244 | 0 | 1409 |
| L3820 | 1 | 54455 | 57236 | 6 | 1405 |
| L3476 | 0 | 54471 | 57240 | 0 | 1406 |
| L4960 | 0 | 54488 | 57236 | 0 | 29308 |
| L5079 | 0 | 54494 | 57248 | 0 | 29308 |
| L4565 | 0 | 54495 | 57236 | 0 | 1407 |
| L3836 | 0 | 54496 | 57240 | 0 | 1408 |
| L5048 | 1 | 54498 | 57236 | 0 | 29308 |
| L3786 | 0 | 54501 | 57240 | 0 | 1406 |
| L5000 | 1 | 54503 | 57256 | 0 | 29308 |
| L3471 | 0 | 54504 | 57240 | 0 | 1406 |
| L4982 | 1 | 54509 | 57244 | 0 | 29309 |
| L5048 | 0 | 54509 | 57252 | 0 | 29308 |
| L3107 | 0 | 54514 | 57248 | 0 | 1409 |
| L3758 | 1 | 54515 | 57240 | 0 | 1408 |
| L4550 | 0 | 54515 | 57220 | 0 | 729 |
| L2893 | 0 | 54516 | 57240 | 0 | 1407 |
| L3835 | 0 | 54525 | 57236 | 0 | 1409 |
| L5136 | 0 | 54526 | 57228 | 0 | 726 |
| L5136 | 1 | 54526 | 57228 | 0 | 726 |
| L2743 | 0 | 54527 | 57224 | 0 | 727 |
| L3110 | 1 | 54527 | 57236 | 0 | 1410 |
| L5134 | 1 | 54528 | 57228 | 0 | 726 |
| L4674 | 1 | 54529 | 57248 | 0 | 1408 |
| L5012 | 0 | 54531 | 57248 | 0 | 1408 |
| L3858 | 0 | 54533 | 57236 | 0 | 1409 |
| L4714 | 1 | 54533 | 57252 | 0 | 1405 |
| L5119 | 1 | 54534 | 57228 | 0 | 726 |
| L5183 | 1 | 54538 | 57228 | 0 | 725 |
| L5185 | 1 | 54538 | 57228 | 0 | 725 |
| L4626 | 1 | 54540 | 57228 | 0 | 726 |
| L4519 | 1 | 54541 | 57228 | 0 | 726 |
| L4565 | 1 | 54542 | 57236 | 0 | 1409 |
| L2753 | 1 | 54543 | 57228 | 0 | 727 |
| L2812 | 0 | 54543 | 57236 | 0 | 1408 |
| L4553 | 1 | 54543 | 57224 | 0 | 728 |

## Cold blocks found

The cells that behave as "cold / rare path" outlines (e=0) are the ones the original
compiler clearly placed out of line:

* `L2058` — case-41 `shipGroup == 9` filter; alone raw +40, but -426 in the landed set.
* `L4399` — case-4 wreck timer (`m_nScriptTimerA >= 324`); alone raw +4, -10 in the set.
* `L1083` — case-3 approach-music timer (`m_nScriptTime >= 9001`); alone raw -3.
* `L2493` — case-70 `messages[1]->isOver()` gate; alone raw -7.
* `L3775` — case-105 route/waypoint state gate; alone raw -1.
* `L4598` — case-154 `m_nScriptTimerA >= 1001`; alone raw +3, -1 in the set.

The strongest hot-path flips are `L3069:1` (case-92 state 7), `L2848:1` (case-91 state 5)
and `L4135:1` (case-131 camera counter): these make the original's fall-through block the
state body instead of the dispatch compare.  `L3069:1` alone is raw +4 / aligned -69 but
becomes the single largest lever (-596 with a size change) once the other case-92 cells
are in place.

## Alternatives measured (not landed)

### Aligned-first variant

`core10` = `[L2848:1, L2562:0, L4135:1, L2493:0, L2392:1, L3775:0, L3069:1, L4399:0,
L1083:0, L4286:1]` (raw 54,299 / aligned 17,540).  Adding `L2038:0` (-8 B alone) plus
`L1079:0` (+8 B alone) is size-exact and gives a much better instruction alignment at a
small raw cost:

| variant | raw | aligned | size | extab |
|---|---:|---:|---:|---:|
| landed 13-cell | **54,247** | 17,532 | 57,232 | 724 |
| core10 + L2038:0 + L1079:0 | 54,351 | **17,099** | 57,232 | 725 |

The aligned-first variant is structurally closer (aligned -328 vs baseline) but is
104 raw bytes worse; kept in the note rather than landed because raw is the primary
metric.

### Other notable single cells

| cell | raw | aligned | size | rel.plt | extab | note |
|---|---:|---:|---:|---:|---:|---|
| L5079:1 | 54,477 | 19,434 | 57,232 | 0 | 28,832 | raw win but `.ARM.extab` explodes |
| L3401:0/1 | 54,507 | 17,903/17,911 | 57,232 | 0 | 725 | raw win, aligned much worse |
| L5231:1 | 54,518 | 17,514 | 57,232 | 0 | 726 | aligned win, extab +1 |
| L1079:0 | 54,641 | 17,306 | 57,232 | 0 | 725 | best single aligned (-121) |
| L2512:0/1 | 54,657 | 17,011 | 57,232 | 0 | 724 | best aligned found (-416), raw +116 |
| L2038:0 | 54,581 | 17,102 | 57,232 | 0 | 725 | aligned -325, raw +40 |
| L2562:0 | 54,494 | 17,420 | 57,232 | 0 | 724 | raw -47 alone, but -28 (worse) in the landed set |

### Size-changing cells (need an opposite-size companion)

| cell | raw | size | note |
|---|---:|---:|---|
| L4720:0 | 54,361 | 57,240 (+8) | best raw single cell overall |
| L4613:0 | 54,371 | 57,240 (+8) | |
| L3751:1 | 54,422 | 57,236 (+4) | |
| L2252:1 | 54,425 | 57,248 (+16) | |
| L2743:0 | 54,527 | 57,224 (-8) | pairs with a +8 cell; best pair (L2743+L4720) is 54,481, worse than the landed set |

## Reproduction

```sh
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
cmake --build cmake-build-match --target link_so -j8
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
cmake --build cmake-build-match --target check -j8
```

## Open questions

1. The largest remaining same-address run is still `0x1396a8..0x139846` (415 B, case 69):
   the original lays the case-125 hacking block (`0x139690`) *before* the case-69 body
   (`0x139720`); no single expect cell moved it.  The outer `if (m_nState >= 4 && m_nState <= 6)`
   (L4027) and inner hacking gate (L4028) were both tested (raw 55,153 / 55,180, size-fatal).
2. `L2038:0` is the best aligned lever (-325) but is -8 B; a genuine -8 B source-shape
   change in the same block would let the aligned-first variant land without the +8 B
   `L1079:0` companion.
3. The literal pool at `0x145890` (300 B run) is still different; that is data layout,
   not branch weights.
