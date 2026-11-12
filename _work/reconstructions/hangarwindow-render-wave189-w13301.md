# HangarWindow::render — wave 189 (w13301): register-allocation pass

Worktree `wt/w13301`, branch `wave189-w13301`, base master `247466fde`
(wave-188 harvest).  Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Target `_ZN12HangarWindow6renderEv` (`0x1490d8`), 9,700 B, 3,734 insns.

## Result

**No size-exact raw win found this wave.**  `src/game/ui/HangarWindow.cpp`
is left at the wave-188 base.  This note records the register-allocation
map and the new negative results so the next wave does not re-run them.

Entry state (wave-188 body): raw **1,510**, aligned **643**, size
**9,700/9,700**, insns **3,730/3,734**.  Gate on the untouched base:
`check` green (RATCHET PASS) — byte_exact 4507/4524, linked_exact
4507/4524, size-exact 4524/4524, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data_exact 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370.  `.text` diff 74,047, `.ARM.extab` 639
(size/base exact at `0x1a3b78`, 156,312 B), `.note.gnu.build-id` 20,
total 74,706, 27/27 headers, file size 2,187,328.

## Per-region raw-byte map of the base (same-address diff)

| region | range | diff |
|---|---|---:|
| prologue | 1490d8–1496da | 0 |
| ship/blueprint | 1496da–149bf0 | 257 |
| mid | 149bf0–14a8ec | 119 |
| buy-list | 14a8ec–14ac74 | 583 |
| free-credits | 14ac74–14ae7e | 228 |
| nlm (loop tail) | 14ae7e–14b1d2 | 70 |
| after-buylist | 14b1d2–14b4a0 | 201 |
| landing pads | 14b4a0–14b6bc | 52 |

Sum 1,510.  The `mid`/`nlm`/`landing-pad` numbers are mostly
address-shift fallout (ours is 4 B longer up to the buy-list and 4
insns shorter overall), not independent bugs.

## Callee-saved register / spill-slot map (prologue is byte-identical)

The prologue `0x1490d8..0x1496da` is identical in both builds, so the
frame layout is fixed:

```
1490d8  push {r4,r5,r6,r7,lr}
1490da  add  r7,sp,#12
1490dc  stmdb sp!,{r7,r8,r9,sl,fp}
1490e0  vpush {d8-d10}
1490e4  sub  sp,#376
1490e6  mov  fp,r0                 ; fp = this
14911a  str.w fp,[sp,#136]         ; slot 136 = this
149136  ldr.w sl,[r0]              ; sl = &Globals::layout
1491b0  str.w sl,[sp,#132]         ; slot 132 = &Globals::layout
```

| reg | owner in base | notes |
|---|---|---|
| fp | `this` | spilled to slot 136; reused as `#17`/`#0` in buy-list/free-credits loops |
| sl | `&Globals::layout` | spilled to slot 132; reused as scratch (layout value, String temp, width) |
| r9 | `this` (after 0x1496da), loop counters | in the original r9 is `this`; in ours r9 is `&Globals::layout` |
| r8 | `&Globals::appManager`, then scratch | |
| r4,r5,r6,r7 | scratch / loop vars / String temporaries | |

Key spill slots (identical addresses in both builds):
slot 128 = `listWidth`, then `&Globals::layout` in the buy-list;
slot 132 = `&Globals::layout`; slot 136 = `this`;
slot 140 = address used to re-read `Globals::Canvas`;
slot 120 = `top` / `rh`; slot 116 = `options` / second temp;
slot 124 = `tab`/`options`; slot 104 = `data`.

## Root cause: a systematic r9 <-> sl role swap

The first differing instruction is still `0x1496da`:

```
O  1496da  e9dd a921  ldrd sl, r9, [sp, #132]   ; slot132(&layout)->r10, slot136(this)->r9
U  1496da  e9dd 9a21  ldrd r9, sl, [sp, #132]   ; slot132(&layout)->r9,  slot136(this)->r10
```

This is not local to the blueprint block.  The *same* swap recurs in
every block that has a two-value tie:

* free-credits: original `width -> sl`, `rh -> r9`; ours `width -> r9`,
  `rh -> sl` (`0x14ad2c..0x14ad32`).
* buy-list tail: original `ldrd sl,r9,[sp,#128]`; ours `ldrd r9,sl`
  (`0x14ae6e`).
* free-credits tail: original `ldrd sl,r9,[sp,#128]`; ours `ldrd r9,sl`.

In every pair the original gives the **first** value the higher
callee-saved register (r10/sl) and the second value r9; ours gives the
first r9 and the second r10.  So one physical-register assignment
choice flips ~257 (ship/blueprint) + 228 (free-credits) + part of the
buy-list at once.

### MIR evidence (ours)

Pre-RA (`-mllvm -print-after-all`, Live Register Matrix dump):
`%630 = &Globals::layout`, `%614 = this`; both are live across the
whole ship/blueprint run and are reloaded together at `0x1496da`.

Post-Greedy / VirtRegRewriter dump of the reload block:

```
$r8  = t2LDRi12 %stack.75, 0     ; scratch
$r10 = t2LDRi12 %stack.76, 0     ; this      (slot 136)
$r9  = t2LDRi12 %stack.79, 0     ; &layout   (slot 132)
```

The original is the mirror image (`this -> r9`, `&layout -> r10`).
The Greedy dump shows the split vregs are `%3333 = this` and
`%3357 = &Globals::layout` (this has the *lower* number, yet ours gives
it the *higher* register), so the tie is not a simple vreg-number
tie-break; it is the Greedy spill-weight/hint order, which depends on
the exact IR shape.

No source-only perturbation tried in this wave or wave-188 flips it:
`self = this` alias (coalesced), `Layout **lay = &Globals::layout`
(canonicalised), removing the free-credits volatiles, reordering the
early declarations, and every `__builtin_expect` variant all keep the
swap.  This is the single biggest remaining lever and it is not
reachable from declaration order alone.

## Free-credits `textX` is sunk in the MIR, not by the post-RA scheduler

The original computes `textX = width + baseX + rh` **before**
`Globals::gameText->getText(...)` (`0x14ad78/0x14ad7e`, before the call
at `0x14ad90`); ours computes it after (`0x14ad96/0x14ad9a`).  In the
original `width`/`rh` are dead before the call, so `r5`/`r6` are free
for the canvas pointer and `textY`; in ours they stay live and clang
spills the canvas to `[sp,#204]` and `textY` to `[sp,#192]`.

The pre-RA MIR confirms the add is already after the call:

```
106560B  %2994:spr = VMLAS ...              ; textY
106960B  tBL @_ZN8GameText7getTextEi
107008B  %3010:gpr = t2LDRi12 %stack.31,0   ; volatile textY reload
107088B  %3013 = t2ADDrr %2982,%577          ; textX = width + baseX
107104B  %585  = t2ADDrr %3013,%2986         ; textX += rh
107184B  tBL DrawString
```

So it is a SelectionDAG/IR-level sink, not a post-RA move.  The sink is
driven by the `PaintCanvas *volatile canvas` + `volatile int &textYv`
pressure: removing either volatile changes the size (9704/9708/9716)
and does not recover the schedule.  Reordering the source declaration
of `textX` (before/after `canvas`, before/after `SetColor`, before
`height`, `+=` split, all associativities, `const`/`unsigned`) is
codegen-neutral (all 1,510/643/9,700).

## Buy-list: value form vs address form (the 4-byte trap)

`Layout *lay = Globals::layout;` (value form) is 9,700 B; the address
form (`Globals::layout->...` at both uses) is **9,696 B**.  The 4 B are
not the `(1-col)` fold as wave-188 thought: expanding `x` to
`w/2 - (bw+gsx) + (bw+gsx)*col` gives **9,692 B** in the value form and
still 9,696 B in the address form.  The address form also changes the
whole buy-list register allocation (`sl` becomes a String temp,
`r5`/`r6` swap, an extra `nop`), so even at size 9,700 it would not
match.  The value form's extra bytes vs the original are the
`ldr.w sl,[sl]` at the loop head and the `ldr.w sl,[sp,#128]` restore
at the tail; the original derefs `[sl]` at each of the two uses.

## After-buylist: `rh` is loaded early in ours

In the free-credits-button block ours loads
`Globals::layout->field_0x2c_rowHeight` at `0x14b22c` and spills it to
`[sp,#120]`; the original loads it lazily at `0x14b2f0` (inside the
`listModeFlag` branch, at the `y - rh` DrawImage2D).  Moving the
`int rh` declaration changes the size: after `setVisible` -> 9,704 B
(3,734 insns, the only size-exact instruction count seen), inside the
`listModeFlag` branch or inlined -> 9,700 B but raw 1,713.

## Variants measured this wave (all reverted; base = 1,510/643/9,700)

| variant | raw | aligned | size | insn |
|---|---:|---:|---:|---:|
| **base (wave-188)** | **1,510** | **643** | 9,700 | 3,730 |
| buy-list address form (no `lay`) | 3,532 | 657 | 9,696 | 3,731 |
| address form + expanded x | 3,600 | 653 | 9,692 | 3,731 |
| value form + expanded x | 3,470 | 653 | 9,696 | 3,728 |
| `rh` after `setVisible` | 2,337 | 546 | 9,704 | 3,734 |
| `rh` after `setVisible` + address form | 2,982 | **540** | 9,700 | 3,734 |
| `rh` in listMode branch / `rh` inlined | 1,713 | 585 | 9,700 | 3,731 |
| `rh` after `y` / between `x` and `y` | 1,510 | 643 | 9,700 | 3,730 |
| `rh` before the `if` | 2,307 | 705 | 9,708 | 3,733 |
| free-credits `width`+`rh` inlined into `textX` | 1,510 | 643 | 9,700 | 3,730 |
| `textX` after `canvas` / after `SetColor` / before `height` | 1,510 | 643 | 9,700 | 3,730 |
| `textX` `+=` split / all parens / `rh` first / `baseX` first / `unsigned` | 1,510 | 643 | 9,700 | 3,730 |
| free-credits `volatile int textY` (decl) | 1,512 | 643 | 9,700 | 3,730 |
| free-credits no canvas volatile | 3,607 | 733 | 9,708 | 3,730 |
| free-credits no textY volatile | 3,375 | 664 | 9,716 | 3,737 |
| free-credits direct `Globals::Canvas->` (no local) | 3,619 | 701 | 9,720 | 3,732 |
| free-credits direct + plain textY | 3,372 | 688 | 9,724 | 3,737 |
| `top` local removed (inline `field_0x20_top`) | **1,511** | **593** | 9,700 | 3,731 |
| `top`+`gsy` removed | 1,512 | 592 | 9,700 | 3,731 |
| `top` removed + `rh` after `setVisible` | 2,338 | 653 | 9,704 | 3,735 |
| `top` removed + `rh` in listMode | 1,714 | 634 | 9,700 | 3,732 |
| `gsy` removed | 1,511 | 642 | 9,700 | 3,730 |
| `top` after `x` / `gsy`+`top` swapped | 1,510 | 643 | 9,700 | 3,730 |
| `barPos`/`barHeight` swapped | 1,513 | 646 | 9,700 | 3,730 |
| `tab` before `drawBG` | 1,537 | 651 | 9,700 | 3,730 |
| `items` before `tab` | 9,064 | 1,157 | 9,668 | 3,722 |
| `SetColor` after the station check | 8,771 | 2,058 | 9,668 | 3,743 |
| `viewMode != 1` | 8,730 | 1,291 | 9,664 | 3,734 |
| `listWidth` before `barPos` | 8,994 | 876 | 9,708 | 3,731 |
| `visibleHeight` local | 1,532 | 649 | 9,700 | 3,730 |
| `drawBG` after `items` | 8,961 | 867 | 9,704 | 3,733 |
| `HangarWindow *self = this` (whole function) | 1,510 | 643 | 9,700 | 3,730 |
| global `Layout *lay` value form | 9,164 | 2,694 | 9,568 | 3,681 |
| global `Layout **lay = &Globals::layout` | 1,510 | 643 | 9,700 | 3,730 |
| `upgTab`/`shipRh` swapped | 1,510 | 643 | 9,700 | 3,730 |
| left/right swapped (both sites) | 7,944–8,660 | 1,112–1,656 | 9,644–9,692 | 3,711–3,719 |
| blueprint `name` after the rate `if` | 7,812 | 954 | 9,660 | 3,714 |
| `row`/`col` no `(unsigned char)` / col first | 1,510 | 643 | 9,700 | 3,730 |
| `count` unsigned | 1,510 | 643 | 9,700 | 3,730 |
| `i` signed | 1,511 | 644 | 9,700 | 3,730 |
| `__builtin_expect(i <= 4,1)` / `count < 5` | 1,510 | 643 | 9,700 | 3,730 |
| remove `expect(i == 0)` | 1,516 | 649 | 9,700 | 3,730 |
| remove `expect(i < 5)` | 2,053 | 955 | 9,700 | 3,731 |

Notes:

* `top` inlined (`no_top`) is the closest near-miss: raw 1,511 (one byte
  worse, entirely in the after-buylist region, where it is 246 insns
  vs the original's 247) but aligned 593, i.e. 50 bytes better than
  base.  It is not a raw win, so it is not landed.
* `rh after setVisible` is the only variant with the original's exact
  instruction count (3,734) and aligned 546, but it is 4 B over; the
  address form compensates the 4 B (9,700/3,734, aligned 540) yet raw
  rises to 2,982 because the address form re-allocates the buy-list.
* The `mid` region's 119 bytes are literal-pool `ldr [pc,#...]` offsets
  shifted by the buy-list's 4-byte length delta; they collapse only if
  the buy-list is made exactly 904 B *and* the total stays 9,700, which
  no source shape found achieves.

## Residual after this wave

Unchanged in kind from wave-188:

* ship/blueprint (`0x1496da..0x149bf0`): the r9/sl swap (257 B);
* mid (`0x149bf0..0x14a8ec`): literal-pool offsets + swap fallout (119 B);
* buy-list (`0x14a8ec..0x14ac74`): value-form head/tail loads + swap (583 B);
* free-credits (`0x14ac74..0x14ae7e`): swap + `textX` sink (228 B);
* after-buylist (`0x14b1d2..0x14b4a0`): early `rh` load + swap (201 B);
* landing pads (`0x14b4a0..0x14b6bc`): cleanup-block scheduling (52 B).

## Reproduction

```sh
ulimit -v 12000000
cp /home/debian/GalaxyOnFire2/scratch/w13301/HangarWindow.base.cpp src/game/ui/HangarWindow.cpp
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN12HangarWindow6renderEv
cmake --build cmake-build-match --target check -j2
```

Scratch (not committed): `/home/debian/GalaxyOnFire2/scratch/w13301/`
(`HangarWindow.base.cpp`, `measure.sh`, `align.py`, `fc_sweep.py`,
`bl_sweep.py`, `flip_sweep.py`, `gen*.py`, `allmir.txt`, `o_*.txt`,
`u_*.txt`).
