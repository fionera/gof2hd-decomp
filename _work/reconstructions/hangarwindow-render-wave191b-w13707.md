# HangarWindow::render — wave 191b (w13707): ship/upgrade branch order + literal-pool sweep, no net win

Worktree `wt/w13707`, branch `wave191b-w13707`, base master `d4e8f032d`.
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
| instructions | 3,727 / 3,734 |
| `.text` section diff | 71,351 |
| `.ARM.extab` section diff | 577 |
| `.ARM.exidx` / `.rodata` / `.rel.plt` | 0 / 0 / 0 |
| `.note.gnu.build-id` | 20 |
| total differing bytes | **71,948** |

`check` RATCHET PASS: byte_exact 4508/4524, linked_exact 4508/4524,
size-exact 4524/4524, stub/missing/wrong_type/extra 0, data_exact 843/843,
imports 207, lint CLEAN, drift 0, parity 5370/5370.

## Residual clusters at base (74 clusters, 1,040 B)

```
0x1496dd-0x1497a5    68   blueprint `this`↔`&Globals::layout` ldrd tie
0x149b20-0x149bd1   172   ship body (guard target + V,D,P vs P,V,D order)
0x14a8ed-0x14abcf   130   buy-list preheader/loop (+2 switch-mask shift)
0x14ac52-0x14ae7f   530   free-credits body (downstream of the tie)
0x14b22c-0x14b30d   202   buttons[17] (downstream of the tie)
0x14b614-0x14b68c    54   cleanup landing pads
```

The `this`/`layout` tie at `0x1496da` is the dominant cause: the whole
blueprint run is instruction-for-instruction identical apart from `r9`/`sl`,
and the free-credits + `buttons[17]` clusters are downstream of the resulting
`&Globals::layout` spill.  Prior notes (w13580/w13613/w13664/w13682) already
established that no size-neutral source edit flips it.

## New evidence: the ship body can be made instruction-identical, but the LSDA scope moves

The original ship body is `setPosition, setVisible, draw` (P,V,D) and the
upgrade body is `setVisible(2), setPosition(10), setVisible(10), draw`
(V2,P10,V10,D).  Moving the upgrade check out of the ship-body `if` (a
sibling `if`, or the equivalent `goto`) makes the `tab == 0` arm reach the
upgrade check and produces the original's 38-byte guard:

```
149b1e  ldr   r0,[sp,#108]        ; tab
149b20  ldrd  r5,r4,[sp,#132]     ; layout,this  (hoisted)
149b24  cmp   r0,#0
149b26  beq.w 14a6d0
149b2a  cmp   r0,#3
149b2c  bne.n 149b44
        ... getShip compare, no reload (r4 preserved)
149b44  <ship body>               ; now byte-identical to the original
```

The body lands at exactly `0x149b44` and is instruction-for-instruction equal
to the original (only 3 small clusters remain: 15 B at the guard, 6 B at the
post-getShip reload, 6 B at the upgrade check).  Raw drops **1,040 -> 928**
and aligned **516 -> 469**.

The cost is the EH cleanup range.  The sibling shape puts the `getShip` call
in a different cleanup region, so LSDA row 39 shrinks from the original's
`(2508, 150, 9664)` to `(2508, 118, 9664)`.  The row length `118 < 128`
crosses the ULEB128 one-byte threshold, so the whole call-site table after
row 39 shifts by one byte: `.ARM.extab` diff **577 -> 1,216**.  Net:
`.text` saves 112 B, `.ARM.extab` loses 639 B (= +527 total differing bytes).

## Measured candidates (all size 9,700 unless noted)

| variant | raw | aligned | `.text` | extab | note |
|---|---:|---:|---:|---:|---|
| base | **1,040** | 516 | 71,351 | **577** | best net |
| sibling P,V,D + faithful V2,P10,V10,D (`sib_PVD_U0_f`) | **928** | **469** | 71,239 | 1,216 | ship body exact; extab-fatal |
| sibling V,D,P + faithful upgrade body | 984 | 491 | 71,295 | 1,216 | |
| `goto` into the ship-body if (P,V,D) | 928 | 469 | 71,239 | 1,216 | identical to sibling |
| two `if`s, P,V,D, current upgrade body | 4,466 | 807 | 74,777 | 1,216 | |
| base body D,P,V,B + `upgTab` after D | **1,030** | 593 | 71,341 | 1,210 | raw-only win; extab-fatal |
| base body V,P,D,B + `upgTab` after V | 1,038 | 595 | 71,349 | 1,210 | |
| base body V,D,P,B + `upgTab` after V (base) | 1,040 | 516 | 71,351 | 577 | |
| base body D,V,P,B + `upgTab` after V | 1,041 | 516 | 71,352 | 578 | |
| P,V,D + `upgTab` before P (nested) | 1,191 | 499 | 71,502 | 586 | body at +4 |
| faithful upgrade body only (`base_upgf`) | 1,187 | 497 | 71,498 | 581 | |
| V,D,P + `upgTab` before V | 1,186 | 521 | 71,497 | 587 | |
| upgrade `setVisible(this->upgradeMode)` | 1,048 | 624 | 71,359 | 577 | |
| `switch (count)` mask (faithful `bic`) | 3,444 | 619 | 379,111 | 1,425 | size 9,704 |
| `switch (count)` + D,P,V,B/after-D | 6,452 | 795 | 76,763 | 1,262 | size 9,700 |

Other negatives measured this session (all size 9,700 / extab 577 unless
noted): `bool visible = true` local in the ship body 1,266/544/27,511;
`bool visible` in the upgrade body 7,505/744/9,716; both 6,645/707/9,708;
`static_cast<bool>(1)` and `setVisible(1)` byte-identical to base;
`String()` instead of `String("")` in the default case 3,415/570/9,696;
`String price; String description;` 3,451/824/9,692; both 3,491/885/9,676;
`item == selectedItem` reversed 1,041/517/9,700; `isShip() != 0` identical;
`bool isShip` local 9,116/1,650/9,624; `float rate` first 7,657/782/9,692;
`(float)`/`(double)` cast on the rate 1,040/516/577.

## Conclusion

The base remains the unique size-exact **net** minimum: every shape that
makes the ship body instruction-identical (or lowers raw below 1,040) also
crosses the row-39 ULEB128 one-byte threshold and grows `.ARM.extab` by more
than it saves.  The residual is the single `this`↔`&Globals::layout` greedy
queue ordering at `0x1496da`, exactly as documented by w13580/w13613/w13664.
The only remaining lever would be a clang 7.0.2 build with asserts /
`-debug-only=regalloc` to read the actual `RS_*` stage and `Prio` of the two
post-`getCompletionRate` reloads.

## Scratch

`/home/debian/GalaxyOnFire2/scratch/w13707/` — `base.cpp`, `measure.sh`,
`sec.py`, `clusters.py`, `lsda3.py`, `sweep.py`, `sweep2.out`, `mir.txt`
(80 MB `-print-after-all` dump of the single function), `check.log`.
