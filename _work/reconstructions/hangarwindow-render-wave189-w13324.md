# HangarWindow::render — wave 189 (w13324): row-39 LSDA pad restored, total −40 B

Worktree `wt/w13324`, branch `wave189-w13324`, base master `6854ca93a`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Target `_ZN12HangarWindow6renderEv` (`0x1490d8`), 9,700 B, 3,734 insns.

## Result (landed)

| metric | wave-188 base | landed |
|---|---:|---:|
| raw same-address diff | 1,510 | **1,494** |
| instruction-index alignment | 643 | **523** |
| function size | 9,700 / 9,700 | 9,700 / 9,700 |
| `.text` section diff | 74,017 | **74,001** |
| `.ARM.extab` section diff | 639 | **615** |
| render LSDA record diff | 81 | **57** (948 B / 156 rows both) |
| `.note.gnu.build-id` | 20 | 20 |
| total differing bytes | 74,676 | **74,636** (−40) |
| file size | 2,187,328 | 2,187,328 |

Gate: `check` RATCHET PASS — byte_exact 4507/4524, linked_exact 4507/4524,
size-exact 4524/4524, stub_zero_size 0, missing 0, wrong_type 0, extra 0,
data_exact 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.
`.rel.plt` untouched.

## What changed

The wave-176/w13302 ship-guard shape was rebuilt with the **upgrade check
inside the guard body** (so the body's cleanup scope covers the upgrade
region, matching the original's row-40 range) and the **upgrade button calls
reordered** so the function stays 9,700 B:

```cpp
if (tab != 0 && __builtin_expect(tab != 3 || Globals::status->getShip() != this->selectedItem->ship, 1)) { // lint: arm_builtin
    int upgTab = tab;
    (*this->buttons)[1]->setPosition(...);
    (*this->buttons)[1]->setVisible(true);
    (*this->buttons)[1]->draw();
    buttonOffset = (*this->buttons)[1]->getWidth() + Globals::layout->field_0x2c_rowHeight;
    if (this->upgradeMode && upgTab == 1) {
        (*this->buttons)[10]->setPosition(...);
        (*this->buttons)[10]->draw();
        (*this->buttons)[2]->setVisible(true);
        (*this->buttons)[10]->setVisible(true);
    }
}
```

The upgrade button order is a pure codegen-shape choice (the four calls are
independent).  With the original order (`setVisible[2]`, `setPosition`,
`setVisible[10]`, `draw`) the same source is 9,704 B; starting the block with
`setPosition` makes it exactly 9,700 B.

## LSDA rows 39-44

```
row  orig                          ours (landed)
39   (2508, 150, 9664)             (2508, 150, 9664)   ✓ exact
40   (2696, 140, 9562)             (2700, 138, 9538)
41   (2854,   6, 9664)             (2856,   6, 9664)
42   (2874,  26, 9356)             (2876,  26, 9356)
43   (2912,   4, 9224)             (2914,   4, 9224)
44   (2918,  32, 9328)             (2920,  32, 9328)
```

Row 39 — the `Status::getShip()` call site — is now byte-identical to the
original (pad 9664, length 150), which is the w13320 note's target.  Rows
41-44 keep the original pad values and differ only by the +2 code shift
introduced by the row-40 region (our body scope is 2 bytes shorter).  The
remaining LSDA deltas are row 40's start/length/pad and the +2 shift of the
later rows; there is no longer a wholesale 1-byte call-site-table shift.

## Negative results (all size 9,700 unless stated)

| variant | raw | text | extab | total |
|---|---:|---:|---:|---:|
| wave-188 base | 1510 | 74,017 | 639 | 74,676 |
| w13302 (upgrade check sibling, original order) | 1244 | 73,751 | 1,234 | 75,005 |
| w13302 + `expect` on 2nd operand (v1) | 1235 | 73,742 | 1,234 | 74,996 |
| nested, upgrade check inside, original order | — | — | — | 9,704 B |
| **nested, upgrade check inside, `s,d,v2,v` order** | **1494** | **74,001** | **615** | **74,636** |
| nested, `s,d,v,v2` order | 1494 | 74,001 | 615 | 74,636 |
| nested, `s,v2,v,d` / `s,v2,d,v` / `s,v,v2,d` / `s,v,d,v2` | 1506-1514 | 74,013-74,021 | 615 | 74,648-74,656 |
| sibling, `s,...` orders | — | — | — | 9,696 B |
| `bool sameShip = ...` before `int upgTab` | 1517 | 78,777 | 27,750 | 106,547 |
| `try` around body / guard | — | — | — | 9,712 B |
| `int shipRh` body local, upgrade uses `tab` | — | — | — | 9,672 B |
| `int upgTab` before guard + `int shipRh` in body | — | — | — | 9,684 B |
| `int &upgTab = tab` | 1244 | 73,751 | 1,234 | 75,005 |

Key mechanism found: the body-scope landing pad is created by the
`lifetime.end` of the first local declared in the `if (item->isShip())` block.
Declaring `upgTab` before the guard puts the `getShip()` call inside that
scope (row 40 starts at the call, length 182); declaring it inside the guard
body moves the scope start to the body, restoring row 39 to length 150.  The
`__builtin_expect` on the second guard operand is what keeps the body pad at
9562 rather than 9514.

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

Scratch (not committed): `/home/debian/GalaxyOnFire2/scratch/w13324/`.

## Harvest on wave-189 master (`860c8f633`)

Landed: raw 1,494 / 523 / 9,700, `.text` 73,983, `.ARM.extab` 615,
total 74,618.  The row-39 LSDA record is now byte-identical to the original;
the remaining render LSDA record diff is 57 bytes.  The next Hangar target is
the independent +4 tail shift at `0x14a960` (w13333) and the remaining row-40
scope.
