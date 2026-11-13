# HangarWindow::render — wave 191b (w13613): blueprint-body reorder + expect-form sweeps, no win

Worktree `wt/w13613`, branch `wave191b-w13613`, base master `8c8c1826d`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Target `_ZN12HangarWindow6renderEv` (`0x1490d8`), 9,700 B.

## Result: no net-positive win landed; `src/` untouched

Baseline re-measured and left pristine:

| metric | value |
|---|---:|
| raw same-address diff | **1,038** |
| instruction-index alignment | 620 |
| function size | 9,700 / 9,700 |
| instructions | 3,726 / 3,734 |
| `.text` section diff | 71,196 |
| `.ARM.extab` section diff | 577 |
| `.note.gnu.build-id` | 20 |
| total differing bytes | **71,793** |

`check` RATCHET PASS: byte_exact 4508/4524, linked_exact 4508/4524,
size-exact 4524/4524, stub/missing/wrong_type/extra 0, data_exact
843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.
`.rel.plt`, `.ARM.exidx` and `.rodata` byte-identical.

(The w13580 note quotes `.text` 71,228 / extab 577; this worktree's base
measures **71,196 / 577** — use that as the w13613 "before".)

## Raw diff clusters at base (34 clusters, 1,038 B)

```
0x1496dd-0x1496f0  20   blueprint ldrd r10/r9 tie
0x14970a-0x14974b  66   blueprint (tie + vmov/vldr schedule)
0x149762-0x149778  23   blueprint (tie)
0x149789-0x1497a5  29   blueprint (tie)
0x149830-0x149840  17   blueprint tail
0x149b20-0x149bd1 178   ship body (sibling guard / cbz)
0x149c80            1   GOT pool offset
0x14a8ed-0x14a8f4   8   buy-list preheader (strd r9,sl vs str r9)
0x14a913-0x14a96a  88   buy-list loop head (sl = layout value)
0x14a982/9b4/9e4/aa1c/aa54/aa8c-aa94/aac8   1 each   literal-pool offsets
0x14aafe-0x14ab05   8   buy-list loop
0x14ab8a-0x14ab9e  21   buy-list loop
0x14abb0/abc2-abcf  1/14 buy-list loop
0x14ac52-0x14acf5 164   free-credits head
0x14ad1a-0x14ae7f 358   free-credits body
0x14ae94-0x14aeac  25   free-credits tail
0x14aeca/af18/b20c  1 each   literal-pool offsets
0x14b22c-0x14b291 102   buttons[17] y
0x14b2a6-0x14b30d 104   buttons[17] draw
0x14b3ec/14b410/14b44a  1 each   literal-pool offsets
0x14b614-0x14b632  31   cleanup landing pads
0x14b664-0x14b68c  41   cleanup landing pads
```

## New evidence: LLVM 7's greedy queue is ordered by interval *size*, not spill weight

The task brief (and the w13580 note) describe the tie as a "spill weight"
ordering.  The actual LLVM 7.0 `RegAllocGreedy::enqueue`
(`lib/CodeGen/RegAllocGreedy.cpp`) is:

```cpp
const unsigned Size = LI->getSize();          // sum of LiveRange lengths
...
Prio = (1u << 29) + Size;                     // global / split ranges
Prio |= (1u << 31);                           // global+local above RS_Split
if (VRM->hasKnownPreference(Reg)) Prio |= (1u << 30);
...
CurQueue.push(std::make_pair(Prio, ~Reg));    // lower vreg number wins ties
```

`LiveInterval::getSize()` is `sum(S.start.distance(S.end))` over the
interval's segments (`LiveInterval.cpp`), i.e. **interval length in slot
units**, not `CalcSpillWeights`' `weight`.  `RS_Split`-stage intervals
get `Prio = Size` with none of the high bits, so they are deferred
behind every `RS_Assign` range.

The three reloads after the second `getCompletionRate()` call
(`bb.288`, slot 47200B) are, in MIR order,

```
47208B  %3275 = LDRi12 %stack.74   (Canvas)
47212B  %3331 = LDRi12 %stack.75   (this)
47220B  %3356 = LDRi12 %stack.78   (Globals::layout)
```

and the greedy result is `%3275 -> r8`, `%3331 -> r10`, `%3356 -> r9`
(verified in the VirtRegRewriter dump at 47208B..47220B).  So the
observed queue order is **Canvas, layout, this**, while the naive
`(1<<29)+Size` ordering would be this (3124) before layout (3068)
before Canvas.  The deciding factor is therefore *not* a plain size
comparison: either one of the three is `RS_Split`/local while another is
`RS_Assign`, or r9 was still interfered at 47212 and freed by 47220.
Both are invisible in the `-print-after-all` dump (no stage/weight
printing in this release clang; `-debug-only=regalloc` is compiled out
and `-print-after=greedy` is not a valid pass name).

The original is `%3275 -> r8`, `%3331 -> r9`, `%3356 -> r10`, i.e. the
queue order is **Canvas, this, layout** — the naive size order.  So the
residual is exactly "our `this` split interval is deferred behind
`layout`'s".

## Fresh shapes measured this session (all size 9,700 unless noted)

`probe3.py`-style sweep; `ldrd` column is the reload at `0x1496da`
(`r9,r10` = ours, `r10,r9` = original roles).

### Branch-hint value flips on the five existing `__builtin_expect`s

All 2^5 = 32 value combinations of
`shipguard`, `upgcheck`, `count<=4`, `i<5`, `i==0` were built.  Only
four are size-exact, and none flips the pair:

| combo (ship,upg,cnt,ilt,ieq) | raw | aligned | `.text` | extab | ldrd |
|---|---:|---:|---:|---:|---|
| `11110` (base) | **1,038** | 620 | 71,196 | 577 | r9,r10 |
| `10110` (upg=0) | 1,121 | 630 | 71,279 | 602 | r9,r10 |
| `11011` | 9,062 | 2,486 | 79,222 | 1,083 | r10,r8 |
| `10011` | 9,058 | 2,488 | 79,218 | 1,083 | r10,r8 |

Removing a hint outright: `no_upgcheck` 1,076/583, `no_ieq0`
1,041/577, `no_shipguard` 4,322/9,704, `no_countle4` 8,721/9,736,
`no_ilt5` 3,578/9,724, `no_all` 9,000/9,668.

### Fresh `__builtin_expect` sites (0 and 1)

`tab==4||right>0` 7,138/9,708; `!isTextButton()` 8,716/9,716 and
8,539/9,740; `isMoveToCargoButton()` 4,684/9,704 and 4,269/9,696;
`isSellButton()` 4,318/9,704 and **1,508/9,700**; the
`isItem()||isShip()||isBluePrint()||isPendingProduct()` chain
1,343/9,700 and 1,038/9,700 (neutral); `barHeight>0||barPos>0`
3,897/9,708 and 1,050/9,700; `!dialogActive` 1,139/9,700 and
9,092/9,648; `count==4&&showBestDeal` 8,700/9,728 and 1,117/9,700.
Equivalent spellings (`isBluePrint()!=0`, `==1`, `!!`,
`selectedItem==item`, `!(item!=selectedItem)`, `!!(item==selectedItem)`)
are byte-identical to the plain `expect(...,1)` forms.

### Blueprint-body statement reorderings

The eight statements inside `if (rate > 0)` (S1 bg draw, S2 fill
region, S3 `borderX`, S4 border draw, S5 `SetColor(0x777777ff)`,
S6 `String progress`, S7 progress `DrawString`, S8 `SetColor(white)`)
were permuted.  The base order S1..S8 is 1,038.

* `S6` moved to position 2 (`S1,S6,S2,S3,S4,S5,S7,S8`) **flips the
  pair with the prologue intact** (`r11@88`, `ldrd r10,r9` at
  `0x1496da`) but is size **9,696** and re-lays the whole block
  (`r5`/`r3`/`r12` roles change), raw 1,577.
* `S6` at position 3 (`S1,S2,S6,...`) is 9,696, `ldrd r10,r11`, raw 1,982.
* `S6` at position 2 + `switch (count)` (the +4 mask form the original
  really has) is **9,700** and keeps `ldrd r10,r9`, but raw 3,586
  (the +4 switch shifts every later address).
* `S3` before `S2` (`S3,S2,S1,...`) is 9,700, aligned 1,015, raw 5,131
  (block moves; `ldrd` lands at `0x149b0c`).
* `S2`/`S1` swap is 9,684.  `S5` before `S4` is 9,704 (`ldrd r10,r11`).
* `S5` moved to positions 1/2/5: 1,196/1,150/1,255 at 9,700; `S6` to
  positions 1/4/5: 1,780/1,255/1,038.
* `progress` built with `+=` (3 or 1 statements) 7,802/9,620,
  7,726/9,680; `String progress; progress = …` 7,647/9,724;
  `int percent = (int)(rate*100.0f); String progress = String(percent)+…`
  1,038 (neutral).

### Other fresh shapes

* `const float rate`, `const int borderX`: byte-identical (neutral).
* `double rate` 8,397/9,720; `volatile float rate` 7,850/9,720;
  `float &rate` alias 7,548/9,704.
* `volatile int textYv = textY` (value, not reference) 2,853/9,716;
  `int &textYv` 8,771/9,744.
* Block-scope `Layout *layout = Globals::layout;` in the free-credits
  block 8,748/9,732; in the blueprint body 7,662/9,704; blueprint
  `Layout*`+`PaintCanvas*` locals 9,251/9,608.
* Buy-list local `TouchButton *button = (*this->buttons)[12+count];`
  (after the two `String`s) 8,549/9,636; before them 8,549/9,636.
* `int barPos`/`int barHeight`/`int listWidth` permutations: base best;
  `B,A,C` 1,041/9,700, `B,C,A` 8,504/9,668.
* Blueprint argument reorders
  (`textX+2+this->hintOffsetX+layout->buttonInsetX`) 7,597/9,704 and
  1,040/9,700; the `borderX` parenthesisation reorder is neutral.
* 110 random 2–4 edit combinations over 13 hint/alias levers: best
  size-exact is still 1,038 (only `barheight_1+tab41_1` and
  `lay_local+tab41_0` reproduce the base exactly).

## Conclusion

The base remains the unique size-exact minimum.  Every shape that flips
`this`↔`&Globals::layout` either changes the function size (1,577 at
9,696; 3,586 at 9,700 with a +4 switch; 8,500 at 9,672 for
`expect(item==selectedItem,1)`) or re-lays the blueprint block's other
registers (`r5`/`r3`/`r12`).  The irreducible residual is the greedy
queue order of the three post-call reloads; the deciding stage/hint is
not observable in the r18b `-print-after-all` dump, so the next angle
would be a build of clang 7.0.2 with `-debug-only=regalloc` enabled to
read the actual `RS_*` stage and `Prio` of `%3331`/`%3356`.

## Scratch

`/home/debian/GalaxyOnFire2/scratch/w13613/` — `base.cpp` (pristine),
`measure.sh`, `secdiff.py`, `probe3.py`, `sweep2.py`, `v18..v33.json`,
`rag.cpp`/`ao.cpp`/`li.h`/`lic.cpp`/`si.h` (LLVM 7.0.0 sources fetched
for the queue analysis), `check.log`.
