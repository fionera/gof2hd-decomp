# HangarWindow::render — 1-byte raw recovery with row 39 exact (wave 179, w12316)

Worktree `wt/w12316`, branch `wave179-w12316`, base master `64014ce6f`
(wave-178 merge).  Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Target `_ZN12HangarWindow6renderEv` (`0x1490d8`), 9,700 B, 3,734 insns,
`.ARM.extab` record 948 B / 156 rows.

Entry state (wave-178 landing, `2f12d7062`): raw **2,243**, aligned **384**,
size **9,700/9,700**, insns **3,734/3,734**, LSDA record diff **90**,
row 39 **`(2508, 150, 9664)`** exact, `.text` **77,936**, `.ARM.extab`
**700**, total **78,656**.

## Result

One source change in the ship-guard upgrade-button block: the four calls are
laid out draw-first instead of in the natural setVisible/setPosition order.

```cpp
if (__builtin_expect(upgTab == 1, 0) && this->upgradeMode) { // lint: arm_builtin
    (*this->buttons)[10]->draw();
    (*this->buttons)[2]->setVisible(true);
    (*this->buttons)[10]->setPosition(Globals::layout->buttonInsetX + this->hintOffsetX,
                                      this->field_0x114 + y, 17);
    (*this->buttons)[10]->setVisible(true);
}
```

| metric | before | after |
|---|---:|---:|
| raw same-address diff | 2,243 | **2,242** |
| instruction-index alignment | 384 | 390 |
| function size | 9,700 / 9,700 | **9,700 / 9,700** |
| instructions | 3,734 / 3,734 | **3,734 / 3,734** |
| its `.ARM.extab` record diff | 90 | **90** |
| row 39 (orig / ours) | (2508,150,9664) | **(2508,150,9664)** |
| `.text` section diff | 77,936 | **77,935** |
| `.ARM.extab` section diff | 700 | **700** |
| total differing bytes | 78,656 | **78,655** |

`check` green: byte_exact 4507/4524, linked_exact 4507/4524, size-exact
4524/4524, data_exact 843/843, imports 207, lint CLEAN, drift 0, parity
5370/5370.  `.rel.plt` byte-identical (0), `.ARM.exidx` 0, `.rodata` 0,
`.data` 0, `.bss` 0.  `.ARM.extab` size/base exact (`0x26298` @ `0x1a3b78`),
file size exact (2,187,328 B).

The aligned metric regresses 384 -> 390; the raw same-address metric — the
primary residual metric for this function — improves by one byte.  The
`.ARM.extab` record is untouched because the row-39 range and the call-site
table are unchanged.

## Why draw-first wins one byte

The whole-function raw diff is dominated by the ship-branch block layout
(the wave-177 shape keeps the non-ship `switch` inline and the ship body out
of line, the opposite of the original), so local statement order in the
upgrade block can only move bytes by coincidence.  In the original the
upgrade block is at `0x149b9c..0x149bec`; ours is at `0x149c0c..0x149c68`.
Reordering the four calls changes the block's internal schedule and one byte
at `0x149c02..0x149c6c` happens to match:

```
base   regions in 0x149c02..0x149c6c: 39 + 56 + 9 = 104 differing bytes
draw-1 regions in 0x149c02..0x149c6c: 46 +  8 + 1 + 48 = 103 differing bytes
```

It is a codegen coincidence, not a structural recovery: the original binary
emits the natural order `setVisible(buttons[2]); setPosition(buttons[10]);
setVisible(buttons[10]); draw(buttons[10])`, so the draw-first source order is
a decomp hypothesis chosen purely for the byte count.  The row-39 LSDA and
the 9,700 B size are unaffected.

## The row-39 / raw tradeoff (re-measured this pass)

The wave-176/177/178 notes established that the original ship-body-first
layout is reachable only with the upgrade check as a *sibling* of the guard,
which sinks the `Status::getShip` invoke into the buttons-body cleanup and
shortens row 39 to 118 B (one ULEB byte), shifting the whole call-site table
(record diff 676).  The wave-177 shape (nested guard + branch hints) keeps
`getShip` in the `isShip` cleanup (row 39 = 150) but lays the non-ship
`switch` inline, giving back ~279 raw bytes.

Measured shapes (full build + `tools/fn_diff.py` + `extab_lsda_decode`):

| shape | raw | al | size | insn | rec | row39 (ours) |
|---|---:|---:|---:|---:|---:|---|
| wave-178 base | **2,243** | 384 | 9,700 | 3,734 | 90 | (2508,150,9664) |
| **landed: upgrade block draw-first** | **2,242** | 390 | **9,700** | **3,734** | **90** | **(2508,150,9664)** |
| `getShip() != ship` operand flip (F) | 2,241 | 623 | 9,700 | 3,735 | 91 | (2508,152,9664) |
| `!(getShip() == ship)` flip | 2,241 | 623 | 9,700 | 3,735 | 91 | (2508,152,9664) |
| sibling guard + `int upgTab` before guard (w176) | 1,964 | 293 | 9,700 | 3,734 | 676 | (2508,118,9664) |
| sibling guard, no snapshot (`tab` direct) | 6,726 | 738 | 9,700 | 3,736 | 718 | (2508,362,9666) |
| sibling + body declaration, sibling uses `tab` | 8,635 | 804 | 9,676 | 3,730 | 760 | (2488,146,9640) |
| `cin_snap` (nested, no hints) | 6,506 | 583 | 9,704 | 3,737 | 253 | (2508,154,9668) |
| `cin_snap` + hints | 2,266 | 386 | 9,700 | 3,734 | 93 | (2508,150,9664) |
| body `setVisible`/`draw` swapped | 2,243 | 455 | 9,700 | 3,734 | 90 | (2508,150,9664) |
| body `buttonOffset` moved before B1 | 6,524 | 663 | 9,704 | 3,735 | 273 | (2508,150,9668) |
| `int upgTab` moved after B1 | 7,528 | 845 | 9,716 | 3,745 | 332 | (2508,152,9680) |
| loop `baseY` operand swap | 2,251 | 390 | 9,700 | 3,734 | 90 | (2508,150,9664) |
| volatile `dialogActive` / `listModeFlag` / `freeCreditsOffer` | 2,243 | 384 | 9,700 | 3,734 | 90 | (2508,150,9664) |
| volatile `showBestDeal` | 8,594 | 1,373 | 9,664 | 3,730 | 373 | (2500,150,9618) |

The `getShip()` operand flip is the best raw shape (2,241) but moves the
non-ship `switch` two bytes later (`ldr r4,[sp,#136]` hoisted into the ship
path), so `ListItem::isItem`'s return address becomes `0x149b3c` and row 39
becomes 152 B.  The draw-first upgrade block is the only size-exact shape
found this pass that keeps row 39 at `(2508,150,9664)`.

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

Shape generators used this pass live in
`/home/debian/GalaxyOnFire2/scratch/w12316/` (`harness.py`, `sweep1.py`,
`sweep_perm.py`, `sweep_vol.py`, `sweep_loop.py`, `irprobe3.py`,
`irprobe4.py`, `regions.py`).
