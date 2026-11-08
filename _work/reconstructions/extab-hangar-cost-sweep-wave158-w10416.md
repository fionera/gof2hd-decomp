# HangarWindow::render — extab record vs text-cost sweep (wave 158, w10416)

Status: **landed source change** (the w10316 shape, re-measured and swept).
Worktree `wt/w10416`, base `9e4795206` (wave 157). Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Target: `_ZN12HangarWindow6renderEv`, the largest `.ARM.extab` record
(676 of 1,292 differing bytes at wave 157).

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size-exact 4524/4524, `data_exact 843/843`, imports 207,
lint CLEAN, drift 0, parity 5370/5370.

## Result

| metric | before (wave 157) | after |
|---|---:|---:|
| raw same-address diff | 1,965 | **2,514** |
| instruction-index alignment | 291 | **485** |
| function size | 9,700 | **9,700** |
| its `.ARM.extab` record diff bytes | 676 | **151** |
| total `.ARM.extab` diff bytes | 1,292 | **767** |
| total `.text` diff bytes | 85,529 | **86,078** |
| `.ARM.exidx` diff bytes | 0 | **0** |

The record keeps 948 bytes / 156 rows.  All section sizes stay exact and the
file size stays 2,187,328 B.

## The landed shape

```cpp
if (item == this->selectedItem) {
    if (item->isShip()) {
        // Lifetime anchor: the original's LSDA has a cleanup region
        // covering the whole ship branch.  This unused local reproduces
        // its lifetime markers (and hence the cleanup blocks).
        volatile int *dummy;
        if (__builtin_expect(tab != 0, 1)) { // lint: arm_builtin anchor: HangarWindow::render; branch-weight hint
            if (!(tab == 3 && Globals::status->getShip() == this->selectedItem->ship)) {
                // LSDA sink: lifetime starts after getShip(), so the getShip
                // invoke unwinds to the outer cleanup (row 39) while
                // setPosition..upgrade unwind to this cleanup (row 40).
                volatile int padX[1];
                (*this->buttons)[1]->setPosition(...);
                ...
                if (this->upgradeMode && tab == 1) {   // moved inside
                    ... upgrade buttons ...
                }
            }
        }
    } else { ... }
}
```

Three levers are needed together and none of them can be dropped:

1. `__builtin_expect(tab != 0, 1)` — block layout / branch weights.  Without
   it the row 39/40 split collapses (extab 676).
2. `volatile int padX[1]` inside the nested `if` — non-promotable lifetime
   sink whose `lifetime.start` is after `getShip()`.
3. `volatile int *dummy;` at the top of the ship branch — a second
   non-promotable lifetime region covering the whole branch.  It is dead at
   IR level but emits two extra cleanup blocks (4 B), which both restores the
   original 9,700 B and moves the block layout back toward the original
   (raw 6,305 -> 2,514).  Any unused local type/name is equivalent.

The upgrade block move is semantically exact: it only runs when `tab == 1`,
which implies `tab != 0` and makes the `getShip` guard false.

## Row table (fn 0x1490d8, 156 rows, 948 B)

```
row  orig (start,len,lp)      ours
 36  (2336, 52, 9664)         (2336, 52, 9666)     LP +2
 37  (2412, 30, 9454)         (2412, 30, 9456)     LP +2
 38  (2456, 34, 9334)         (2456, 34, 9334)     exact
 39  (2508, 150, 9664)        (2508, 158, 9666)    getShip in outer cleanup
 40  (2696, 140, 9562)        (2680, 26, 9356)     comparison row
 41  (2854, 6, 9664)          (2718, 4, 9224)
 42  (2874, 26, 9356)         (2724, 32, 9328)
 43  (2912, 4, 9224)          (2776, 4, 9380)
 44  (2918, 32, 9328)         (2818, 140, 9588)    setPosition..upgrade
 45  (2992, 4, 9418)          (2992, 4, 9420)      LP +2
 46  (3008, 154, 9588)        (3008, 154, 9590)    LP +2
```

`getShip` now unwinds to the outer cleanup (row 39's LP 9666 vs the
original 9664; the +2 is the code shift), and the setPosition..upgrade
region is one 140-byte row (row 44) with the original length.  The
remaining 151 differing bytes are the +2 LP shift and the reordered
comparison row.

### Why the +2 LP shift is the remaining floor

The +2 shift is one extra `b.n` cleanup trampoline at fn offset 9382
(ours) that the original does not have.  It is a block-placement artefact:
`if (item->isShip())` is lowered as `cbz` (2 B) in ours and as
`cmp; beq.n` (4 B) in the original because the original keeps the ship body
inline while ours puts the non-ship `switch (tab)` body inline and jumps
over it with a 4-byte `b.w`.  Net +2 bytes before the landing-pad region,
which shifts every LP after it and drops the final alignment `nop`.
Removing it needs the ship body to be placed inline, which is a
MachineBlockPlacement decision not reachable from source order (see sweep).

## Sweep (all measured; raw / aligned / extab-fn / size)

| variant | raw | al | extab | size |
|---|---:|---:|---:|---:|
| base (wave 157) | 1,965 | 291 | 676 | 9,700 |
| **landed: expect + dummy + array padX + upgrade inside** | **2,514** | **485** | **151** | **9,700** |
| expect only (no sink/dummy) | 2,260 | 398 | 465 | 9,700 |
| expect + dummy, no sink | 2,260 | 398 | 465 | 9,700 |
| expect + array padX, no dummy | 6,305 | 437 | 259 | 9,696 |
| expect + scalar/ptr padX, no dummy | 6,305 | 437 | 259 | 9,696 |
| no expect + dummy + array padX + upgrade inside | 2,276 | 410 | 697 | 9,700 |
| expect + array padX, upgrade outside | 8,624 | 876 | 765 | 9,676 |
| expect + dummy + array padX, upgrade outside | 6,734 | 681 | 707 | 9,692 |
| w10219 (volatile int padX = 0, upgrade inside) | 7,811 | 842 | 296 | 9,700 |
| padX declared after setPosition | 2,468 | 490 | 684 | 9,700 |
| padX declared after setVisible/draw | 6,281/6,275 | 654/655 | 705 | 9,696 |
| padX declared before upgrade | 6,738 | 677 | 705 | 9,692 |
| padX + padY companion (two sinks) | 2,514 | 485 | 151 | 9,700 |
| padX + padY own scopes | 6,729 | 749 | 707 | 9,696 |
| String padX sink | 8,506 | 944 | 558 | 9,720 |
| String padX + dummy | 6,764 | 695 | 502 | 9,720 |
| short-circuit `tab != 0 && (tab != 3 || ...)` | 7,283 | 1,763 | 345 | 9,656 |
| bool `skip` form | 2,514 | 485 | 151 | 9,700 |
| empty-then `if (tab==0) {} else if (...)` | 2,240 | 407 | 694 | 9,700 |
| expect on inner `getShip` compare | 2,280 | 411 | 697 | 9,700 |
| expect on `item->isShip()` | 8,632 | 2,399 | 657 | 9,680 |
| expect on `item == selectedItem` | 7,355 | 2,490 | 551 | 9,668 |
| expect on `item->isSelectable()` | 9,022 | 1,399 | 462 | 9,720 |
| expect on upgrade guard (cold) | 2,521 | 497 | 174 | 9,700 |
| `tab == 1 && upgradeMode` order | 2,514 | 482 | 163 | 9,700 |
| dummy at func/loop/items/selectable top | 6,305 | 437 | 259 | 9,696 |
| dummy before selectedItem | 8,920 | 1,685 | 704 | 9,636 |
| dummy inside nested if / after inner if | 6,305 | 437 | 259 | 9,696 |
| dummy in its own `{ }` scope | 6,305 | 437 | 259 | 9,696 |
| dummy type: int/char/float/double/void*/array/2 dummies | 2,514 | 485 | 151 | 9,700 |
| dummy `= 0` (scalar int) | 7,260 | 1,120 | 313 | 9,708 |
| padX element type: int/char/short/float/double/ll/array size 1/2/4 | 2,514 | 485 | 151 | 9,700 |
| expect weight 2/3/4/5/8/16/100/1000 | 2,514 | 485 | 151 | 9,700 |

Every alternative that keeps the extab win (151) also pays the same
raw/aligned cost; every alternative with a smaller text cost loses most of
the extab win.  The landed shape is the Pareto point (extab win per aligned
instruction: 525/194 = 2.71, vs 211/107 = 1.97 for expect-only).

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10416
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10416/fdiff.py _ZN12HangarWindow6renderEv       # 2514, 9700/9700
python3 /dev/shm/w10416/align_diff.py _ZN12HangarWindow6renderEv  # 485
python3 /dev/shm/w10416/rows.py 0x1490d8                          # 156 rows
python3 _work/reconstructions/extab_record_map.py                 # fn 151, total 767
cmake --build cmake-build-match --target check -j6                # RATCHET PASS
```

Scratch (not committed), `/dev/shm/w10416/`: `gen*.py` variant generators,
`measure.sh`, `fdiff.py`, `align_diff.py`, `rows.py`, `diffruns.py`.
