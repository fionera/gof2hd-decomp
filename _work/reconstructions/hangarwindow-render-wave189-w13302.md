# HangarWindow::render — wave 189 (w13302): ship-guard block order, raw 1510 -> 1244

Worktree `wt/w13302`, branch `wave189-w13302`, base master `247466fde`
(wave-188 notes).  Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Target `_ZN12HangarWindow6renderEv` (`0x1490d8`), 9,700 B, 3,734 insns.

## Result

Restored the **wave-176 ship-guard source shape** (originally landed by
w12014, later replaced by the wave-187 nested-`__builtin_expect` form).
The compound guard plus the `int upgTab = tab;` snapshot makes clang lay
the ship body out *first* (fallthrough from the `isShip` check) and the
non-ship `switch` block out of line, which is the original's block order:

```
orig  149b1a cmp  r0,#0        ; isShip()
      149b1c beq.n 149bf0      ; -> non-ship switch
      149b1e ldr  r0,[sp,#108] ; tab
      149b20 ldrd r5,r4,[sp,#132]
      149b24 cbz  r0,149b96    ; tab == 0 -> upgrade check
      149b26 cmp  r0,#3
      149b28 bne.n 149b44      ; tab != 3 -> body
      ...
      149b44 body
      149b94 ldr  r0,[sp,#108] ; tab reload
      149b96 cmp  r0,#1        ; upgrade check
```

Landed source (inside `if (item->isShip())`):

```cpp
int upgTab = tab;
if (tab != 0 && (tab != 3 || Globals::status->getShip() != this->selectedItem->ship)) {
    (*this->buttons)[1]->setPosition(
        Globals::layout->buttonInsetX + this->hintOffsetX + listWidth,
        this->field_0x114 + y, 18);
    (*this->buttons)[1]->setVisible(true);
    (*this->buttons)[1]->draw();
    buttonOffset = (*this->buttons)[1]->getWidth() + Globals::layout->field_0x2c_rowHeight;
}
if (this->upgradeMode && upgTab == 1) {
    (*this->buttons)[2]->setVisible(true);
    (*this->buttons)[10]->setPosition(Globals::layout->buttonInsetX + this->hintOffsetX,
                                      this->field_0x114 + y, 17);
    (*this->buttons)[10]->setVisible(true);
    (*this->buttons)[10]->draw();
}
```

## Before / after

| metric | before (wave-188 base) | after |
|---|---:|---:|
| raw same-address diff | 1,510 | **1,244** |
| instruction-index alignment | 643 | **481** |
| function size | 9,700 / 9,700 | **9,700 / 9,700** |
| instructions | 3,730 / 3,734 | 3,730 / 3,734 |
| `.text` section diff | 74,047 | **73,781** |
| `.ARM.extab` section diff | 639 | 1,234 |
| its `.ARM.extab` record diff | 81 | 676 |
| `.note.gnu.build-id` | 20 | 20 |
| total differing bytes | 74,706 | 75,035 |
| file size | 2,187,328 | 2,187,328 |

Per-region raw diff (same-address bytes):

```
region              before   after
1490d8-1496da            0       0
1496da-149bf0          257      84
149bf0-14a8ec          119       4
14a8ec-14ac74          583     583
14ac74-14ae7e          228     228
14ae7e-14b6bc          323     345
```

The 115-byte win in `149bf0..14a8ec` is the literal-pool offset shift
disappearing: the ship guard now emits the same instruction count as the
original through that run, so the `ldr rX,[pc,#...]` pools line up again.
The ship guard itself drops from ~194 to ~21 differing bytes; the first
divergence is now only the `tab == 0` edge (`cmp r0,#0; beq.w 14a6d0`
vs the original's 2-byte `cbz r0,149b96`).

## `.ARM.extab` tradeoff (honest)

The `.text` win is 266 bytes; the `.ARM.extab` record diff rises from 81
to 676 because the tail code shifts by 8 bytes and every landing-pad VMA
in the LSDA moves by +8 (`9664 -> 9672`, `9562 -> 9502`, ...).  The
`.ARM.extab` section *size* and base stay exact (156,312 B at
`0x1a3b78`), and the row count is unchanged (156).  Row 39 stays
`(2508, 150, 9664)` in the original and `(2508, 150, 9672)` in ours —
the length is still 150, so this is not the wave-176 row-39 shortening;
it is the +8 tail shift.  The wave-176 note's row-39 = 118 observation
applied to the wave-175 base and no longer holds here (the wave-187 body
already fixed the row length).

The gate is green either way (`check` RATCHET PASS), but the total
differing-byte count grows by 329, so this is a `.text`/raw win with an
`.ARM.extab`-content regression.  The remaining tail shift is the next
target (cold block before vs after the free-credits literal pool).

## Variants measured this wave (all size 9,700 unless stated)

| variant | raw | aligned | size | extab rec |
|---|---:|---:|---:|---:|
| **wave-188 base** | 1,510 | 643 | 9,700 | 81 |
| **landed (wave-176 shape)** | **1,244** | **481** | 9,700 | 676 |
| `upgTab == 1 && upgradeMode` | 1,249 | 484 | 9,700 | 676 |
| body draw-first | 1,258 | 483 | 9,700 | 676 |
| upgrade body setVisible-first | 1,286 | 493 | 9,700 | 676 |
| `int shipRh` local used in `buttonOffset` | 8,636 | 806 | 9,684 | 781 |
| nested `if (tab != 0) { if (...) }` + expect | 1,510 | 643 | 9,700 | 81 |
| `bool showUpgrade = ...` local | 1,767 | 667 | 9,700 | — |
| upgrade check outside the `tab != 0` block | 8,611 | 937 | 9,676 | — |
| upgrade check inside `tab != 0` block | 8,611 | 937 | 9,676 | — |
| `cin_snap` (upgTab inside body, no expect) | 6,722 | 613 | 9,708 | 264 |
| `cin_snap` draw-first | 6,724 | 623 | 9,708 | 264 |
| `cin_snap` upgradeMode-first | 6,730 | 611 | 9,708 | 264 |
| `expect(isShip(),1)` | 8,668 | 3,920 | 9,696 | — |
| `expect(isShip(),0)` | 8,629 | 1,285 | 9,716 | — |
| free-credits `SetColor` before textY | 1,551 | 622 | 9,700 | — |
| free-credits `SetColor` before textX | 1,551 | 622 | 9,700 | — |
| free-credits `SetColor` before height | 1,529 | 640 | 9,700 | — |
| free-credits `SetColor` before width | 1,532 | 641 | 9,700 | — |
| free-credits textX associativity (5 forms) | 1,510 | 643 | 9,700 | — |
| free-credits no `textYv` alias | 3,375 | 664 | 9,716 | — |
| free-credits no volatile canvas | 3,607 | 733 | 9,708 | — |
| buy-list address-form `Globals::layout` | 3,532 | 657 | 9,696 | — |
| buy-list address-form + expanded x | 3,556 | 642 | 9,696 | — |
| buy-list expanded x (value form) | 1,513 | 633 | 9,700 | — |

The wave-176 shape is the only one found that both keeps 9,700 B and
recovers the original ship-guard block order.  The `int upgTab = tab;`
snapshot is load-bearing: without it the `tab == 0` edge becomes
`cmp r0,#0; beq.w <merge>` (6 B) and the function is 9,704-9,708 B.

## Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN12HangarWindow6renderEv
python3 _work/reconstructions/extab_record_map.py
cmake --build cmake-build-match --target check -j2
```

Scratch (not committed): `/home/debian/GalaxyOnFire2/scratch/w13302/`
(`HangarWindow.base.cpp`, `HangarWindow.win1.cpp`, `measure.sh`,
`measure2.sh`, `secdiff.py`, `buckets.py`, `regiondiff.py`, `brdiff2.py`,
`v1.py`..`v13.py`).
