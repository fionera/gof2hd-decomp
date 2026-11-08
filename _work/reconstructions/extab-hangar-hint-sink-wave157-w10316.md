# HangarWindow::render — branch-weight hint + array sink (wave 157, w10316)

Status: **landed source change**. Worktree `wt/w10316`, base `08575efff` (wave 156).
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Target: `_ZN12HangarWindow6renderEv`, the largest `.ARM.extab` record
(676 of 1,292 differing bytes at wave 156).

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size-exact 4524/4524, `data_exact 843/843`, imports 207,
lint CLEAN, drift 0, parity 5370/5370.

## Result

| metric | before (wave 156) | after |
|---|---:|---:|
| raw same-address diff | 1,965 | **2,514** |
| instruction-index alignment | 291 | **485** |
| function size | 9,700 | **9,700** |
| its `.ARM.extab` record diff bytes | 676 | **151** |
| total `.ARM.extab` diff bytes | 1,292 | **767** |
| total `.text` diff bytes | 85,831 | **86,380** |
| `.ARM.exidx` diff bytes | 0 | **0** |

The record keeps 948 bytes / 156 rows.  The extab gain (676 -> 151) costs
only 549 raw bytes / 194 aligned instructions — far below w10219's
7,811 / 842.  The `.text` diff rises by exactly the function's raw delta;
all section sizes stay exact.

## The landed shape

```cpp
if (item == this->selectedItem) {
    if (item->isShip()) {
        volatile int *dummy;          // register-pressure anchor (see below)
        (void) dummy;
        // LSDA sink: splits the ship-branch cleanup region ...
        if (__builtin_expect(tab != 0, 1)) { // lint: arm_builtin anchor: HangarWindow::render; ...
            if (!(tab == 3 && Globals::status->getShip() == this->selectedItem->ship)) {
                volatile int padX[1]; // non-promotable LSDA sink, scope starts after getShip
                (*this->buttons)[1]->setPosition(...);
                ...
                if (this->upgradeMode && tab == 1) {   // moved inside the nested if
                    ... upgrade buttons ...
                }
            }
        }
    } else { ... }
}
```

Three independent levers are needed together:

1. **`__builtin_expect(tab != 0, 1)`** changes the block layout so the
   `getShip` invoke is emitted in the outer cleanup's row (row 39) instead
   of the padX cleanup.  Without it the compiler merges the `tab == 0`
   edge with the function exit and `getShip` stays in row 40.
2. **`volatile int padX[1]`** (an array, not a scalar) is the non-promotable
   LSDA sink.  It creates the inner cleanup covering `setPosition` through
   the upgrade draws.  A scalar `volatile int padX` gives extab 296; the
   array gives 253 (rows 150/140 exact) but is 4 B too large (9,704).
3. **`volatile int *dummy; (void) dummy;`** at the top of the ship branch
   perturbs register allocation just enough to remove the 4-byte
   `ldr r4,[sp,#136]; ldr r5,[sp,#132]` reload that the array sink
   otherwise forces after `getShip`, restoring the original 9,700 B.

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

## Variant table (raw / aligned / extab-fn / size)

| variant | raw | al | extab | size |
|---|---:|---:|---:|---:|
| base (w156) | 1,965 | 291 | 676 | 9,700 |
| w10219 (volatile int padX, upgrade inside) | 7,811 | 842 | 296 | 9,700 |
| w10219_char (volatile char padX) | 7,803 | 844 | 296 | 9,700 |
| w10219p / w10219_arr (pointer/array sink) | 6,502 | 580 | 253 | **9,704** |
| w10219_arr + expect(tab!=0,1) | 6,305 | 437 | 259 | **9,696** |
| w10219_arr + expect + dummy ptr | 5,003 | 842 | 255 | **9,704** |
| **w10219_arr + expect + dummy ptr (landed)** | **2,514** | **485** | **151** | **9,700** |
| base + expect(tab!=0,1) only | 2,260 | 398 | 465 | 9,700 |
| hoist sameShip + volatile int* padX | 2,559 | 659 | 694 | 9,700 |
| do-while padX sink | 7,439 | 1,011 | 307 | 9,712 |

The `volatile int *dummy` declaration is not read; it only shifts the
register allocator off the reload path.  It is a plain C++ local (no
attribute, no asm).

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10316
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10316/fdiff.py _ZN12HangarWindow6renderEv       # 2514, 9700/9700
python3 /dev/shm/w10316/align_diff.py _ZN12HangarWindow6renderEv  # 485
python3 _work/reconstructions/extab_record_map.py                 # fn 151, total 767
cmake --build cmake-build-match --target check -j6                # RATCHET PASS
```

Scratch (not committed), `/dev/shm/w10316/`: `gen2.py` (variant generator),
`batch.sh`, `fdiff.py`, `align_diff.py`, `HangarWindow.base.cpp`.
