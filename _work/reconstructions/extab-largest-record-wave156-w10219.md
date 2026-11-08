# `.ARM.extab` per-record map + HangarWindow::render row 39/40 fix (wave 156, w10219)

Status: **landed source change**. Worktree `wt/w10219`, base `cf74f4ee6`
(wave 155). Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size-exact 4524/4524, `data_exact 843/843`, imports 207,
lint CLEAN, drift 0, parity 5370/5370.

## 0. Per-record `.ARM.extab` diff map

`_work/reconstructions/extab_record_map.py` decodes both `.ARM.exidx` tables,
matches records by (function VMA, occurrence), and ranks them by differing
bytes.  There are 2,438 records in each image; **11 differ**, all size-exact
(same record length and row count).  Before the fix:

```
  diff function                                                         k   olen    len orow urow
   676 _ZN12HangarWindow6renderEv                                       0    948    948  156  156
   182 _ZN11LevelScript7processEi                                       0    348    348   46   46
   159 _ZN5Level13createMissionEv                                       0    956    956  148  148
    87 _ZN5Radar4drawEP6PlayerP3Hudi                                    0    132    132   19   19
    77 _ZN5Level10createShipEiiiP8Waypointbb                            0    336    336   57   57
    37 _ZN3Hud4drawExxP9PlayerEgobjj                                    0    476    476   77   77
    30 _ZN5MGame10OnTouchEndEiiPv                                       0    280    280   44   44
    21 _ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE      0    172    172   26   26
    14 _ZN11SpaceLounge10drawLoungeEv                                   0    168    168   25   25
     7 _ZN5RadarC1EP5Level                                              0    256    256   39   39
     2 _ZN10ModStationC1Ev                                              0    112    112   15   15
TOTAL diff bytes 1292 in 11 records
```

`HangarWindow::render` is by far the largest (52 % of all extab diff bytes).
After the fix:

```
   296 _ZN12HangarWindow6renderEv                                       0    948    948  156  156
   ... (the other ten records unchanged) ...
TOTAL diff bytes 912 in 11 records
```

## 1. The fix: the upgrade block belongs inside the ship-branch `if`

The row 39/40 boundary is one landing-pad identity.  Original rows:

```
39 orig (2508, 150, 9664)   getIndex..getShip, LP = outer cleanup
40 orig (2696, 140, 9562)   setPosition..upgradeMode draw, LP = ship cleanup
```

The original's `getShip` invoke unwinds to the **outer** cleanup, while
`setPosition` through the upgrade-button draws unwind to the inner cleanup.
That requires a lifetime-tracked local whose `lifetime.start` is after
`getShip` and whose scope extends through the upgrade block.  The w10019 note
proved a dead `volatile int *` sink declared before `if (tab != 0)` cannot do
this, and that moving only the sink shrinks the function to 9,676 B.

The landed shape moves the `upgradeMode` block **inside** the nested `if`
(the block only runs for `tab == 1`, which implies `tab != 0` and makes the
`getShip` guard false, so this is semantically exact) and uses a
`volatile int` sink at the top of the nested body:

```cpp
if (item->isShip()) {
    if (tab != 0) {
        if (!(tab == 3 && Globals::status->getShip() == this->selectedItem->ship)) {
            // LSDA sink: the ship-branch cleanup starts after the
            // getShip() invoke and covers the upgrade buttons too.
            volatile int padX = 0;
            (*this->buttons)[1]->setPosition(...);
            ...
            if (this->upgradeMode && tab == 1) {
                ... upgrade buttons ...
            }
        }
    }
}
```

This is the only shape found that keeps the original **9,700 B** text size
*and* moves the row 39/40 boundary.  `volatile int *padX` gives a better extab
(253) and raw diff (6,506) but is 4 B too large (9,704); every non-volatile or
`String` sink either grows the frame or is SROA-deleted (9,676-9,732 B).

## 2. Measured result

| metric | before | after |
|---|---:|---:|
| `HangarWindow::render` raw same-address diff | 1,965 | **7,811** |
| instruction-index alignment | 291 | **842** |
| function size | 9,700 | **9,700** |
| its `.ARM.extab` record diff bytes | 676 | **296** |
| total `.ARM.extab` diff bytes | 1,292 | **912** |
| total `.text` diff bytes | 86,044 | **91,890** |

The raw/aligned regression is the price of the block move: it changes register
allocation across the whole 9.7 KB function (the compiler now picks `r4/r5/r6`
instead of `r9/fp` for the loop locals, and the `tab == 0` edge becomes a
6-byte `cmp/beq.w` instead of the original's 2-byte `cbz`).  The exception
table is structurally much closer: row 39/40 lengths are now **150/140**, the
exact original lengths.

### Row table after (fn 0x1490d8, 156 rows, 948 B)

```
row  orig (start,len,lp,act)   ours
  0  (0, 684, 0)               (0, 688, 0)          first row +4 (block move)
  1  (684, 20, 9544)           (688, 22, 9536)
 39  (2508, 150, 9664)         (2512, 148, 9666)    length 150 -> 148
 40  (2696, 140, 9562)         (2706, 134, 9514)    length 140 -> 134
```

The remaining 296 bytes are 2-byte start/LP fields shifted by the 4-byte code
growth plus the pre-existing credit-loop region (rows 96-135).  They cannot be
removed without making the `.text` byte-exact.

## 3. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10219
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 _work/reconstructions/extab_record_map.py    # per-record table
cmake --build cmake-build-match --target check -j6   # RATCHET PASS
```

Scratch (not committed), `/dev/shm/w10219/`: `fdiff.py`, `align_diff.py`,
`measure.py`, `rowdiff.py`, `records.py`, `maprec.py`, `sweep.py`,
`sweep2.py`, `sweep3.py`, `HangarWindow.base.cpp`.
