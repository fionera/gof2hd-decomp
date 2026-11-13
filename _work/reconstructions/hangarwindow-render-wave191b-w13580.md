# HangarWindow::render — wave 191b (w13580): buy-list / upgrade-body sweeps, no win

Worktree `wt/w13580`, branch `wave191b-w13580`, base master `5b422acf1`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Target `_ZN12HangarWindow6renderEv` (`0x1490d8`), 9,700 B.

## Result: no net-positive win landed

Baseline re-confirmed and left pristine: raw **1,038**, aligned 620,
size 9,700/9,700, insn 3,726/3,734, `.text` diff **71,228**,
`.ARM.extab` diff **577**, `.rodata` size delta 485, `.rel.plt`
byte-identical.  `check` RATCHET PASS (byte_exact 4507/4524,
linked_exact 4507/4524, size-exact 4524/4524, data_exact 843/843,
imports 207, lint CLEAN, drift 0, parity 5370/5370).

Note: the w13543 note quotes `.text` 71,334 / extab 577 for the same
shape; this worktree's base measures `.text` **71,228** (the wave-191b
master has moved on by other TUs).  Use 71,228 as the w13580 "before".

## The residual is one register tie-break, not structure

`flip.py` at `0x1496da`: ours `dde9219a` (`ldrd r9, sl, [sp,#132]`),
orig `dde921a9` (`ldrd sl, r9, [sp,#132]`).  The prologue stores are
byte-identical (`[sp+132]=&Globals::layout`, `[sp+136]=this`), and the
whole blueprint run `0x1496de..0x1497a5` is instruction-for-instruction
identical apart from `r9`/`sl`.  So the greedy allocator assigned
`this -> r10(sl)`, `layout -> r9`; the original is the reverse.

MIR (`render_greedy_new.mir`, fresh `-mllvm -print-after-all` dump of
the current base; identical to the old `render_greedy.mir`): bb.288
reloads `%3331 = load %stack.75` (this) then `%3356 = load %stack.78`
(layout); the VirtRegRewriter dump assigns `%3331 -> $r10`,
`%3356 -> $r9`.  Live ranges are 47212B..50336B (this, ~12 uses) and
47220B..50288B (layout, 4 uses), so `this` has the higher spill weight
and is processed second.  Every size-neutral source edit tried keeps
that ordering.

Downstream of the swap: the buy-list loop spills `&Globals::layout`
(`strd r9, sl, [sp,#128]`) and uses `r5`/`sl` for the two String
temporaries, where the original keeps `&layout` in `sl` and uses
`r5`/`r6`.  That single spill is what makes the free-credits block
(0x14ac52..0x14aeac, ~547 diff bytes) and `buttons[17]`
(0x14b22c..0x14b30d, ~206 bytes) diverge.

## Negative results measured this wave (all size 9,700 / extab 577 unless noted)

Buy-list `switch` mask (the original really has
`bic.w r0,r9,#0x80000000 / cmp r0,#4 / tbb`):

| variant | raw | size | extab |
|---|---:|---|---:|
| `switch (count & 0x7fffffff)` | 3,442 | 9,704 | 1,425 |
| mask + `setVisible` before the two `String`s | 3,408 | **9,700** | (size 156,308) |
| mask + `description` first | 3,445 | 9,704 | 1,425 |
| mask + `count < 5` | 3,442 | 9,704 | 1,425 |
| mask + plain loop (no expect) | 8,702 | 9,740 | 1,680 |

The mask costs +2..+4 at the switch; the −4 `setVisible`-first edit
lands the size at 9,700 but leaves the post-switch stream 2 bytes out
of phase (contiguous diff 0x14a908..0x14b6b8 = 3,505 bytes).

Buy-list declaration / lifetime moves (all neutral 1,038 or worse):
`description` first 1,040; `String price = String("")` 1,038;
`int count = 0` 1,038; `++count` 1,038; `listModeFlag != 0` 1,038;
`const int upgTab` 1,038.

Upgrade-button body `__builtin_expect` reshapes: nested
`if (upgTab==1) { if (upgradeMode) … }` 1,076 / extab 583; nested with
expect 1,078 / 580; `expect(upgTab==1) && expect(upgradeMode)` 1,038;
`bool doUpg` 4,269 / 9,704; `expect(upgradeMode && upgTab==1)` 1,038;
`setVisible(true)` literal 1,040.

Ship-guard empty-arm forms: `if (!tab)` 1,173 / extab 590; `(void)0`,
`;`, `0 == tab`, `tab <= 0`, `expect` on the inner `||` term all
byte-identical at 1,038; `expect(...,0)` 7,452 / 9,720; no-expect
4,322 / 9,704.

Two-if (upgrade check outside the guard) without `goto`:
`if (tab != 0 && (…))` 2,749 / **9,696**; PVD body 6,881 / 9,700;
VDP 6,887 / 9,700; PDV 6,875 / 9,700.  The faithful structure puts the
upgrade block far away (target `14a6d0`), so the guard still emits
`cmp/beq.w` instead of the original's `cbz 149b96`, and the early
`ldrd r5,r4,[sp,#132]` never appears.

Free-credits declaration order: `width,rh,height` 1,088 / extab 578;
`width,height,rh` 1,088; `textY` before `textX` 1,038;
`textX = width + rh + baseX` 1,038; `SetColor` before `textX`
2,756 / 9,692.

Blueprint `String progress` association: `(a+b)+c+d` (base) 1,038;
`a+(b+c)+d` 1,418 / 9,692; `a+b+(c+d)` 1,445 / 9,676;
`(a+b)+c+d` explicit parens 1,038; station-first 1,116 / extab 587;
`"%  ("` first 1,069 / extab 581.

Misc: `barPos || barHeight` 1,040; iPad/null-guard rewrites 1,038;
`button->draw`/`setPosition`/`replaceText` permutations 1,057–1,077;
`buttons[17]` faithful order 1,058.

Pairwise combinations of 15 near-neutral edits (100 combos): best
**1,042** (`J4_listwidth_swap2 + J5_basey_swap`), none below 1,038.

## The hoist shape is still a net loss (re-confirmed)

`int upgTab = tab;` above the ship guard + PVD body: raw **928**,
`.text` **71,118**, but `.ARM.extab` **1,216** (row 39 shrinks to
`(2508, 118, 9664)` because the `upgTab` scope starts at `getShip`,
so the row-length ULEB drops to one byte).  Inserting a call before
the `if (item == selectedItem)` guard extends row 39 but wrecks the
allocation: `Globals::Canvas->SetColor(0xffffffffu)` → 8,674 /
**9,688** / extab 871; `getText(0)` → 8,665 / 9,692 / 877.  Net is
still a loss.

## Scratch

`/home/debian/GalaxyOnFire2/scratch/w13580/` — `base.cpp` (pristine),
`measure.sh`, `sweep.py`, `regions.py`, `sbs.py`, `secdiff.py`,
`flip.py`, `norm2.py`, `render_decomp.c` (fresh headless-Ghidra
decompile of `0x1590d8`), `render_greedy_new.mir` (fresh greedy dump),
`allpass_new.txt` (255 MB, delete when done), `m1.json`…`m12.json`,
`pairs.json`, `pool.json`, `hoist.py`.

## Checkpoint 2 (w13580 continuation): fresh-angle sweeps, still no win

Baseline re-confirmed after every experiment: raw **1,038**, aligned
620, size **9,700/9,700**, insn 3,726/3,734, `.text` **71,228**,
`.ARM.extab` **577**, `.rodata` 485, `.rel.plt` identical, flip
`0x1496da = dde9219a` (ours `ldrd r9,sl` vs orig `ldrd sl,r9`).

### Body-order × `upgTab`-declaration-position matrix

The `upgTab` snapshot is declared either *before* the guard or *after*
one of the four body statements; the body statements were permuted.
Only `VDP`+decl-after-`setVisible` (the committed base) is 1,038/577.
Best raw wins are all extab-fatal:

| body / decl | raw | size | extab |
|---|---:|---|---:|
| VDP / after setVisible (base) | 1,038 | 9,700 | 577 |
| DVP / after draw | 1,039 | 9,700 | 578 |
| DPV / after draw | **1,028** | 9,700 | 1,210 |
| VPD / after setVisible | 1,036 | 9,700 | 1,210 |
| PVD / before setPosition (w13527) | 1,193 | 9,700 | 586 |
| VDP / before setVisible | 1,186 | 9,700 | 587 |

`DPV/after-draw` is the best raw of the whole wave but its LSDA row 40
is `(2710, 126, 9550)`; `126 < 128` shrinks the row-length ULEB and
shifts the whole table (671 differing record bytes).  The last call in
row 40 is `TouchButton::draw` at `+0xb10`; extending the scope by two
bytes would need a size-neutral call move, which every attempt broke.

### `__builtin_expect` reshapes on the upgrade check

`expect(tab==1)&&upgradeMode` 1,078/580; `tab==1&&expect(upgradeMode)`
1,103/581; `expect(upgradeMode)&&tab==1` 1,057/581;
`expect(...,0)` 1,121/602; no-expect 1,076/583; `expect(upgTab==1,0)`
1,262/595; `expect(upgTab!=1||!upgradeMode,0)` 6,628/9,696/801.
Base `expect(upgTab==1 && upgradeMode,1)` = 1,038/577.

### Buy-list String temporaries

All 24 upgrade-op permutations on the base body: only `P2VD` (base) is
1,038; next `PV2D` 1,050, `P2DV`/`PVD2` 1,054, `PD2V`/`PDV2` 1,062;
anything with `buttons[2]->setVisible` first is 1,144–1,189 / extab 581.
Case-assignment order (`price=…` before `description=…`) 1,053/577;
with `description` declared first 1,051/577.  `String description =
String("")` (vs `("")`) 1,040/577.  `unsigned int count` 1,038/577.
`switch (count)` (plain int, reproduces orig `bic.w r0,r9,#0x80000000`)
3,442/9,704/1,425.  Loop `count < 5` 8,721/9,736; `expect(count<5,1)`
1,038/577.  Moving `setVisible` after the switch 3,303/9,708/1,639.
`draw`/`setPosition` reorders 8,772–8,784 / 9,756.

### Free-credits String temporaries

`String("(+") + (formatCredits + String(")"))` 3,402/9,676/766;
`(String("(+") + formatCredits) + String(")")` 1,038/577 (neutral);
`String(")") + formatCredits + String("(+")` 1,042/577.
`int &textYv` / `int textYv` (non-volatile) 8,771/9,744/1,699;
`const volatile int &` 1,038/577 (neutral).  `String("")` →
`getText(0)` in the `i==0` DrawString 3,463/9,700.

### `String name` lifetime

`String name("")` / `= String("")` 8,185/9,708/1,525;
`String name` before `SetColor` 7,686/9,684; before the
`isSelectable` block 9,121/9,680; `int buttonOffset = 0` and
`name`/`buttonOffset` swap both 1,038/577 (neutral).

### Hoist row-39 extension, re-confirmed

`int upgTab = tab;` above the guard + PVD body = 928/71,118/1,216.
No-expect variant 922/71,112/1,215.  Adding a call before the
`if (item == selectedItem)` guard changes the allocation and size:
`getShip()` 8,336/9,668/1,543; `SetColor` 8,674/9,688/871;
`getText(0)` 8,665/9,692/877.  Moving an existing `SetColor` from
before `String name` to after the `DrawString` 9,130/9,680/909.
The `w13527` body-scope `upgTab` restores row 39 = `(2508,150,9664)`
(extab 586) but costs 155 raw / 153 `.text` in the ship body.

### Conclusion

The base remains the best net shape.  The residual is the single
`this`↔`&Globals::layout` register tie-break at `0x1496da`; every
size-neutral source edit tried (declaration order, lifetime, body
order, `__builtin_expect`, expression association) leaves the
allocator's choice unchanged (`dde9219a`), and every shape that moves
it (hoist, w13527) either grows `.ARM.extab` past its raw saving or
costs raw in the ship body.
