# HangarWindow::render — wave 191b (w13722): 64-combo sweep + guard/declaration spellings, no net win

Worktree `wt/w13722`, branch `wave191b-w13722`, base master `b3e9c3e09`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Target `_ZN12HangarWindow6renderEv` (`0x1490d8`), 9,700 B.

## Result: no net-positive win landed; `src/` untouched

Baseline re-measured and left pristine:

| metric | value |
|---|---:|
| raw same-address diff | **1,040** |
| instruction-index alignment | **516** |
| function size | 9,700 / 9,700 |
| `.text` section diff | **72,153** |
| `.ARM.extab` section diff | **564** |
| `.ARM.exidx` / `.rodata` / `.rel.plt` | 0 / 0 / 0 |
| total differing bytes (text+extab) | **72,717** |

`check` green at base: byte_exact 4508/4524, linked_exact 4508/4524,
size-exact 4524/4524, stub/missing/wrong_type/extra 0, data_exact 843/843,
imports 207, lint CLEAN, drift 0, parity 5370/5370.

Raw-diff clusters at base (256 B buckets, 1,040 B total):

```
1496d8   59   blueprint ldrd r9/sl tie
1497d8    4   blueprint tail
149ad8  161   ship guard / upgrade body
149bd8    1
14a8d8   83   buy-list preheader + loop head
14a9d8    6
14aad8   27   buy-list loop
14abd8  121   buy-list tail
14acd8  211   free-credits head/body
14add8  111   free-credits body
14aed8    1
14b1d8  150   buttons[17] y
14b2d8   50   buttons[17] draw
14b3d8    3
14b5d8   52   cleanup landing pads
```

## The 64-combination sweep

`gen_combo.py` swept {hoist `int upgTab = tab;` above the ship guard 0/1} ×
{ship body V-D-P / P-V-D} × {upgrade body P,V,V,D / V,P,V,D} ×
{`switch ((signed char) count)` / masked `switch (count)`} ×
{free-credits order} × {`buttons[17]` order}.  All 64 built; results in
`combo.out`.  The only size-exact shapes are the `g1` (hoisted) ones.

| variant | raw | aligned | size | `.text` | extab | text+extab |
|---|---:|---:|---|---:|---:|---:|
| **base** (`g0_vdp_pvvd_s0_f0_b0` shape) | **1,040** | **516** | 9,700 | **72,153** | **564** | **72,717** |
| `g1_pvd_vpvd_s0_f0_b0` (best raw) | **928** | **469** | 9,700 | 72,041 | 1,203 | 73,244 |
| `g1_pvd_vpvd_s0_f0_b1` | 948 | 458 | 9,700 | 72,061 | 1,203 | 73,264 |
| `g1_pvd_vpvd_s0_f1_b0` | 962 | 462 | 9,700 | 72,075 | 1,203 | 73,278 |
| `g1_vdp_vpvd_s0_f0_b0` (hoist only) | 984 | 491 | 9,700 | 72,097 | 1,203 | 73,300 |
| `g1_vdp_vpvd_s0_f1_b1` | 1,038 | 473 | 9,700 | 72,151 | 1,203 | 73,354 |
| `g1_pvd_pvvd_s1_f0_b0` | 4,452 | 632 | 9,700 | 75,565 | 1,203 | 76,768 |
| masked `switch` + hoist (`s1`) | 3,307+ | 572+ | 9,704 | 378,757+ | 1,920 | — |

The hoist is a genuine raw/alignment win (raw −112, aligned −47) but it
grows `.ARM.extab` by **+639**, i.e. a net **+527** total bytes.  This is
the same shape as the reverted w13522 commit `fa0481dc5` (raw 1038→928,
extab 577→1216), reverted by w13527 `1275443c6`.  Per the task's
"extab growth must not exceed the raw saving" rule it is a **net loss**.

## Why the hoist loses: LSDA row 39 crosses the 128-byte ULEB boundary

`lsdacmp.py` on the render LSDA (file offset `0x1c30fc`, cst 927,
156 rows).  Base vs original:

```
orig  row 39 (2508, 150, 9664)   row 40 (2696, 140, 9562)
base  row 39 (2508, 172, 9664)   row 40 (2688, 148, 9550)   -> 26 differing rows
hoist row 39 (2508, 118, 9664)   row 40 (2654, 182, 9598)   -> 35 differing rows
```

Base keeps both row lengths ≥128 (2-byte ULEB), so the call-site table
length matches the original's 927 and only 26 rows differ.  The hoist
moves the `getShip` invoke out of row 39, shrinking it to 118 (<128),
which changes the ULEB width and shifts the whole call-site table
(+639 extab diff).  The hoist additionally breaks rows 17/46/65/101/102/
105/106/109/110/113/114/117/118/121/122 while fixing 103/107/111/115/119/123.

To make the 928 shape landable, row 39 must stay ≥128 B.  w13548 showed
that inserting a call (`SetColor`) between `DrawString` and the
`if (item == selectedItem)` guard does extend it (record diff 676→332)
but costs 12 B of function size (9,688) and raw 8,711 — no size-neutral
way to extend row 39 was found.

## Fresh negative results this wave (all size 9,700 unless noted)

### Ship-guard spellings

| variant | raw | aligned | size | extab |
|---|---:|---:|---|---:|
| base `if (tab == 0) { } else if (expect(...))` | **1,040** | **516** | 9,700 | **564** |
| `if (tab == 0) { } else { if (expect(...)) }` | 1,040 | 516 | 9,700 | 564 (byte-identical) |
| `if (tab == 0) { } else if (!(tab == 3 && getShip() == ship))` | 4,322 | 700 | **9,704** | 1,425 |
| `if (tab == 0) { } else if (expect(!(tab == 3 && getShip() == ship),1))` | 1,040 | 516 | 9,700 | 564 (byte-identical) |
| `if (tab != 0 && expect(...))` (two-if) | 1,175 | 611 | 9,700 | 577 |
| `if (tab != 0) { if (expect(...)) }` (nested) | 1,175 | 611 | 9,700 | 577 |
| `if (tab != 0) if (expect(...))` (no braces) | 1,175 | 611 | 9,700 | 577 |
| two-if, base bodies, no/one/both expects | 2,673–8,614 | 794–977 | **9,696 / 9,676** | 18,994–19,048 |

The faithful two-sequential-`if` shape (upgrade check *outside* the ship
guard, which is what the original has: `cbz r0, 149b96` jumps to
`cmp r0,#1`) is size-fatal with the base bodies.  Only the faithful
upgrade body (`V2,P10,V10,D`) restores 9,700, and then raw is 6,875.

### `upgTab` declaration spellings

| variant | raw | aligned | size | extab |
|---|---:|---:|---|---:|
| base `int upgTab = tab;` after `setVisible` | **1,040** | **516** | 9,700 | **564** |
| `unsigned int` / `(int) tab` / `int upgTab; upgTab = tab;` / `long` / `const int` | 1,040 | 516 | 9,700 | 564 (byte-identical) |
| `volatile int upgTab = tab;` | 5,048 | 849 | **9,704** | 1,452 |
| `int upgTab` before `setVisible` | 1,186 | 521 | 9,700 | 574 |
| `int upgTab` after `setPosition` | 8,499 | 821 | **9,696** | 1,280 |
| `int upgTab` at top of `viewMode == 0` block | 8,607 | 846 | **9,676** | 19,048 |
| `bool upgrade = this->upgradeMode;` snapshot | 8,475 | 835 | **9,692** | 853 |

### Ship/upgrade body order (body-scope `upgTab`)

| variant | raw | aligned | size | extab |
|---|---:|---:|---|---:|
| base V,upg,D,P + upgrade P10,V2,V10,D | **1,040** | **516** | 9,700 | **564** |
| upgrade body V2,P10,V10,D (`setVisible(true)` first) | 1,187 | 497 | 9,700 | 568 |
| ship body upg,P,V,D | 1,191 | 499 | 9,700 | 573 |
| ship body upg,V,P,D | 1,191 | 511 | 9,700 | 574 |
| ship body V,P,upg,D | 6,651 | 685 | **9,704** | 1,916 |
| ship body V,P,upg,D + upgrade V2,P10,V10,D | 6,671 | 704 | **9,708** | 1,916 |

### Misc

| variant | raw | aligned | size | extab |
|---|---:|---:|---|---:|
| base | **1,040** | **516** | 9,700 | **564** |
| `listWidth = listWidth - (...)` | 1,040 | 516 | 9,700 | 564 (byte-identical) |
| `textX = field_0x4c + field_0x2cc + iconSpace` | 1,049 | 519 | 9,700 | 564 |
| `buttons[17]` list-mode P,R,D (faithful) | 1,058 | — | 9,700 | 577 |
| free-credits V,y,P,D (faithful) | 1,072 | — | 9,700 | 577 |

## Structural facts reconfirmed from the disassembly

* Original ship guard: `ldr r0,[sp,#108]` (tab); `ldrd r5,r4,[sp,#132]`
  (layout, this); `cbz r0,149b96`; `cmp r0,#3; bne`; `getShip` compare;
  `ldrd r5,r4,[sp,#132]` again.  `149b96` is the *separate* upgrade check
  `cmp r0,#1; bne` — i.e. the original nests nothing; the upgrade block
  is a second `if` after the ship body.
* Original ship body is P,V,D (`setPosition` 149b62, `setVisible`
  149b70, `draw` 149b7c).  Original upgrade body is V2,P10,V10,D
  (`setVisible(2)` 149bb0, `setPosition(10)` 149bce, `setVisible(10)`
  149bdc, `draw` 149be8).
* Original free-credits body is `setVisible(visible); y=…;
  setPosition(baseX,y); draw();` (14acfa/14ad16/14ad1c).
* Original `buttons[17]` list branch is `setPosition, replaceTextKeepSize,
  draw` (14ab86/14ab9a/14b2d4), with `setSplitText` shared after the
  if/else (14b462); the non-list branch is `setText, setPosition, draw`
  (14b40a/14b438/14b444), also followed by the shared `setSplitText`.
* The original's `this`/`&Globals::layout` reload at `0x1496da` is
  `ldrd sl, r9` (layout→sl, this→r9); ours is `ldrd r9, sl`.  The whole
  blueprint run is instruction-identical apart from that role swap.
  Everything downstream (buy-list String temps in `r5`/`sl` vs `r5`/`r6`,
  free-credits `rh` in `sl` vs `r9`, `buttons[17]`) follows from it.
  All size-neutral source edits tried keep the swap.

## Conclusion

The base remains the unique size-exact raw minimum.  The best raw win
(hoist, raw 928) is a net loss because it moves `getShip` out of LSDA
row 39 and pushes that row below the 128-byte ULEB boundary
(extab +639).  The faithful statement orders (ship P,V,D, upgrade
V2,P10,V10,D, free-credits V,y,P,D, two sequential `if`s) either cost
raw or change the function size.  No net-positive change was landed.

## Scratch

`/home/debian/GalaxyOnFire2/scratch/w13722/` — `measure.py`
(raw/aligned/size/text/extab/exidx), `runvar.py`/`one2.py` (apply +
`ninja link_so -j2` + measure + restore), `gen_combo.py` + `vcombo.json`
+ `combo.out` (64-combo sweep), `lsdacmp.py` (LSDA row diff),
`lsdadiff.py` (exidx→extab locator), `buckets.py` (256-B raw-diff
buckets), `sbs.py` (instruction side-by-side), `vnew.json`,
`vguard.json`, `vguard2.json`, `vdecl.json`, `vmisc.json`, `vpvd.json`,
`vpvd2.json` (this wave's variants), `base.so`/`best.so`,
`lsda_base.out`, `buckets_base.out`.
