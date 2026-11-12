# HangarWindow::render — wave 188 (w13217): sl/r9 tie and free-credits textX schedule

Worktree `wt/w13217`, branch `wave188-w13217`, base master `ce16f45f1`
(wave-187 harvest 5).  Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Target `_ZN12HangarWindow6renderEv` (`0x1490d8`), 9,700 B, 3,734 insns.
Entry state (wave-187 w13117 body): raw **1,510**, aligned **643**, size
**9,700/9,700**, insns **3,730/3,734**, `.text` **74,223**,
`.ARM.extab` **639**, build-id **20**, total **74,882**, file size
2,187,328.

## Result

**No size-exact raw win found this wave.**  `src/game/ui/HangarWindow.cpp`
is left at the wave-187 base.  This note records the new evidence and the
negative results so the next wave does not re-run them.

Gate on the untouched base: `check` green (RATCHET PASS) — byte_exact
4507/4524, linked_exact 4507/4524, size-exact 4524/4524, stub_zero_size 0,
missing 0, wrong_type 0, extra 0, data_exact 843/843, imports 207, lint
CLEAN, drift 0, parity 5370/5370.  Section diffs re-measured directly:
`.text` 74,223, `.ARM.extab` 639 (size/base exact at `0x1a3b78`,
156,312 B), `.note.gnu.build-id` 20, total 74,882, 27/27 headers, file
size 2,187,328.

## New evidence: the `ldrd sl,r9` swap is a pure Greedy tie

The first differing instruction is still `0x1496da`:

```
O  1496da  ldrd sl, r9, [sp, #132]   ; r10 = word0, r9 = word1
U  1496da  ldrd r9, sl, [sp, #132]   ; r9  = word0, r10 = word1
```

Both builds write the *same* values to the *same* stack slots earlier:

* `0x14911a  str.w fp, [sp, #136]` — `fp = this` (identical in both).
* `0x1491b0  str.w sl, [sp, #132]` — `sl = &Globals::layout` (identical
  in both; the PIC sequence at `0x149132..0x149136` loads the GOT entry
  value, i.e. the *address* of the global, not the `Layout*` value).

So `[sp,#132] = &Globals::layout`, `[sp,#136] = this` in **both**.  The
original assigns `&Globals::layout -> r10` and `this -> r9`; ours assigns
`&Globals::layout -> r9` and `this -> r10`.  Everything else in the
ship/blueprint run (96 differing byte addresses in
`0x1496da..0x149bf0`) is a consequence of that one physical-register
choice.  It is not a stack-slot-order problem — reordering the spills is
not a lever.

This also confirms the wave-145 correction: the live register is `r10`
(`sl` in objdump spelling), and it holds the *address* of the global.

## New evidence: buy-list `lay` value form vs address form

The wave-186/187 body uses `Layout *lay = Globals::layout;` inside the
buy-list loop.  That makes clang materialise the *value* at the loop head
(`ldr.w sl,[sl]` at `0x14a960`) and restore the address from
`[sp,#128]` at the loop tail (`0x14ac64`).

Using the *address* form instead (direct `Globals::layout->field` at both
uses, or `Layout **lay = &Globals::layout;`, `Layout *const *`,
`Layout *&`, `Layout *volatile *`, `Layout *volatile &`) reproduces the
original's deref-at-use shape (`ldr.w rX,[sl]` at both use sites) and is
structurally much closer — but it is **9,696 B**, exactly 4 B short of the
sacred 9,700.  The 4 B are not the derefs (both forms spend 8 B there);
they come from a different `(1 - col)` fold in the `x` computation
(orig folds `w/2 - (bw+gsx)*(1-col)` into `w/2 - (bw+gsx) + (bw+gsx)*col`
via `mla`, the address form emits an extra `rsb r0,r0,#1` and `muls`).
No source shape tried in this wave produced the address form at 9,700 B.

## New evidence: free-credits `textX` is scheduled after `getText` in ours

The free-credits block (`0x14ac74..0x14ae7e`, 125 differing byte
addresses) has a second root cause besides the `sl`/`r9` role swap:

* original computes `textX = width + baseX + rh` **before**
  `Globals::gameText->getText(...)` (`add r1,sl,r8; add r8,r1,r9` at
  `0x14ad78/0x14ad7e`, before the `getText` call at `0x14ad90`), so
  `width`/`rh` are dead before the call and `r9`/`r10` are free for the
  canvas pointer and `textY`;
* ours computes `textX` **after** `getText` (`0x14ad96/0x14ad9a`), so
  `width`/`rh` stay live across the call and clang spills the canvas
  pointer to `[sp,#204]` and `textY` to `[sp,#192]`.

All attempts to force the earlier schedule (see table) either changed
nothing (the scheduler re-sinks the add) or grew the function.

## Variants measured this wave (all reverted)

Base = wave-187 body (raw 1,510 / aligned 643 / 9,700 B).  Every entry is
a source-only change; size is 9,700 B unless stated.

| variant | raw | aligned | size |
|---|---:|---:|---:|
| **base (wave-187)** | **1,510** | **643** | 9,700 |
| free-credits `textX` before `canvas` | 1,512 | 643 | 9,700 |
| `textX` before `textY` / after `textY` / before `height` | 1,510 | 643 | 9,700 |
| `const int textX` / `const int width/rh/height` | 1,510 | 643 | 9,700 |
| `volatile int textX` | 6,444 | 1,779 | 9,716 |
| `int textX` + `volatile int textXv = textX` | 4,097 | 1,539 | 9,716 |
| `volatile int *textXp = &textX` | 6,442 | 1,781 | 9,716 |
| `textX` associativity (`width+(baseX+rh)`, `width+rh+baseX`, `rh+width+baseX`) | 1,510 | 643 | 9,700 |
| `const String *label = getText(...)` then DrawString | 3,560 | 707 | 9,708 |
| `int tx = ...` after SetColor | 1,510 | 643 | 9,700 |
| `PaintCanvas *const volatile canvas` | 1,510 | 643 | 9,700 |
| `PaintCanvas *const canvas` | 3,607 | 733 | 9,708 |
| `PaintCanvas *volatile canvas; canvas = Globals::Canvas;` | 1,510 | 643 | 9,700 |
| `volatile int textY` (pre-wave-187) | 1,512 | 643 | 9,700 |
| `textYv` used at all three reads | 1,510 | 643 | 9,700 |
| free-credits visibility chain without `__builtin_expect` | 1,516 | 649 | 9,700 |
| free-credits visibility as one boolean expression | 3,337 | 638 | 9,704 |
| `row += 1` instead of `row++` | 1,510 | 643 | 9,700 |
| buy-list `Layout **lay = &Globals::layout` (address form) | 3,532 | 657 | **9,696** |
| buy-list `Layout *const *` / `Layout *&` / `Layout *volatile *` / `Layout *volatile &` | 3,446–3,532 | 655–657 | 9,692–9,696 |
| buy-list direct `Globals::layout` at both uses | 3,532 | 657 | **9,696** |
| buy-list `lay` used only for `y` (DrawImage2D via `Globals::layout`) | 1,852 | 593 | 9,700 |
| buy-list `lay` used only for DrawImage2D (`y` via `Globals::layout`) | 8,721 | 1,440 | 9,704 |
| buy-list faithful call order (replaceText, setSplitText, draw) | 1,538 | 655 | 9,700 |
| buy-list faithful order + address form | 3,530 | 668 | 9,696 |
| buy-list description-first assignments (revert wave-186) | 1,510 | 723 | 9,700 |
| buy-list description-first + faithful order | 1,538 | 735 | 9,700 |
| buy-list reverse case order | 1,510 | 643 | 9,700 |
| buy-list `TouchButton *btn = (*this->buttons)[12+count]` cache | 8,664 | 1,825 | 9,620 |
| blueprint `int pw = this->progressBarWidth` local | 7,577 | 797 | 9,704 |
| blueprint `int hintX = this->hintOffsetX` local | 7,681 | 751 | 9,708 |
| blueprint `int pbh = this->progressBarHeight` local | 7,537 | 806 | 9,704 |
| blueprint `rate` after the bg DrawImage2D | 7,607 | 852 | 9,704 |
| blueprint `hintOffsetX + textX + ...` operand order | 7,588 | 825 | 9,704 |
| blueprint `y + this->iconOffsetY` operand swap | 1,510 | 643 | 9,700 |
| blueprint `field_0x114` before `field_0x260` | 1,512 | 645 | 9,700 |
| blueprint `borderX` inlined into DrawImage2D | 1,816 | 609 | 9,696 |
| blueprint `borderX` with `textX` first | 2,043 | 684 | 9,700 |
| ship guard `isShip() == 1` / `!= 0` / `?: true:false` | 1,510 | 643 | 9,700 |
| ship guard `__builtin_expect(isShip(),1)` | 8,668 | 3,920 | 9,696 |
| ship guard `__builtin_expect(isShip(),0)` | 8,800 | 2,146 | 9,620 |
| ship guard plain outer condition | 6,631 | 590 | 9,704 |
| ship guard plain inner condition | 1,556 | 735 | 9,700 |
| ship guard outer `expect` only | 1,532 | 645 | 9,700 |
| ship guard `upgradeMode` first | 1,580 | 739 | 9,700 |
| ship guard `!(tab==3 && ship==sel)` | 1,510 | 643 | 9,700 |
| ship guard `getShip()!=sel || tab!=3` (reversed) | 8,568 | 796 | 9,680 |
| ship guard no `upgTab` snapshot | 1,510 | 643 | 9,700 |
| ship guard no `shipRh` snapshot | 1,521 | 664 | 9,700 |
| ship guard no `upgTab`, no `shipRh` | 6,889 | 850 | 9,700 |
| blueprint `isBluePrint` / `getCompletionRate` expect hints | 7,600–9,303 | 1,534–2,503 | 9,556–9,664 |
| `HangarWindow *self = this` alias in the blueprint block | 7,619 | 797 | 9,704 |

Notes:

* The wave-186 price-before-description assignment order is raw-neutral
  (1,510 either way) but aligned-better (643 vs 723); it is kept.
* The wave-145 "metric-only" call order (`draw` before
  `replaceTextKeepSize`) is still in the base and is still raw-better
  than the faithful order (1,510 vs 1,538).
* `lay` used only for `y` gives the best aligned score seen at size
  9,700 (593) but raw regresses to 1,852 — not a raw win.
* Every address-form/direct-`Globals::layout` shape lands at 9,696 B;
  every value-form shape lands at 9,700 B.  The 4 B is the `(1-col)`
  fold, not the global deref.

## Residual after this wave

Unchanged from wave-187 in kind:

* ship/blueprint run (`0x1496da..0x149bf0`): pure `r10`/`r9` physreg
  tie between `&Globals::layout` and `this` (96 differing byte
  addresses);
* buy-list run (`0x14a8ec..0x14ac74`): the `lay` value form's 4-byte
  `ldr.w sl,[sl]` at the loop head plus the `ldr.w sl,[sp,#128]`
  restore at the tail; the address form is 4 B short (239 differing
  byte addresses);
* free-credits run (`0x14ac74..0x14ae7e`): `width`/`rh` and
  `r9`/`r10` roles swapped, `textX` scheduled after `getText`, canvas
  and `textY` spilled (125 differing byte addresses);
* tail (`0x14ae7e..0x14b6bc`): landing-pad table shifted by one
  instruction because ours has 3,730 insns vs the original's 3,734
  (625 differing byte addresses, mostly branch encodings).

The mid region `0x149bf0..0x14a8ec` is semantically identical; its only
diffs are `ldr rX,[pc,#...]` literal-pool offsets shifted by 4 and two
literal-pool words, all consequences of the buy-list 4-byte delta.

## Reproduction

```sh
ulimit -v 12000000
cp /home/debian/GalaxyOnFire2/scratch/w13217/HangarWindow.base.cpp src/game/ui/HangarWindow.cpp
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN12HangarWindow6renderEv
cmake --build cmake-build-match --target check -j2
```

Scratch (not committed): `/home/debian/GalaxyOnFire2/scratch/w13217/`
(`HangarWindow.base.cpp`, `measure.sh`, `adiff.py`, `try*.py`).
