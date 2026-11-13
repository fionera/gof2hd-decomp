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
