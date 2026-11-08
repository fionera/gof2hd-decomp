# Hud::draw + HangarWindow::render — wave 155 (w10109): moving the ship-branch upgrade block fixes the extab row split

Worktree `wt/w10109`, base `master f4e0ea40b` (wave 154). Targets
`_ZN3Hud4drawExxP9PlayerEgobjj` (`src/game/ui/Hud.cpp`, original 9332 B,
VMA 0x163b90) and `_ZN12HangarWindow6renderEv` (`src/game/ui/HangarWindow.cpp`,
original 9700 B, VMA 0x1490d8). Ground truth is the original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Metrics: raw same-address diff (`/dev/shm/w10109/fdiff.py`), instruction-index
alignment (`/dev/shm/w10109/align_diff.py`), and the `.ARM.extab` LSDA row
decoder (`_work/reconstructions/extab_lsda_decode.py`, driven by
`/dev/shm/w10109/rows.py`).

## Result

| function | raw before | raw after | aligned before | aligned after | size | frame |
|---|---:|---:|---:|---:|---:|---:|
| `Hud::draw` | **1325** | **1325** | **249** | **249** | 9332 | 224 |
| `HangarWindow::render` | 1965 | **7811** | 291 | **842** | 9700 | 376 |

`HangarWindow::render` `.ARM.extab` record: **676 -> 296 differing bytes**,
948-byte record, **156 rows** (both unchanged). Row 39/40 lengths are now
**150 / 140** (exactly the original lengths); only the record-relative start
and landing-pad offsets remain 4-6 bytes off. This is a structural extab win
at a raw-byte cost: the block move changes register allocation across the whole
function, so the same-address raw diff rises even though the exception table is
much closer.

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size-exact 4524/4524, `data_exact 843/843`, imports
**207**, lint **CLEAN**, drift **0**, parity 5370/5370.

## 1. `HangarWindow::render`: the upgrade block belongs inside the nested `if`

The w9514/w10019 notes established that row 39/40 is a single landing-pad
identity: the original's `getShip` unwinds to the **outer** cleanup, while
ours unwinds to the `padX` cleanup. A dead `padX` sink declared before
`if (tab != 0)` cannot fix it because its `lifetime.start` is before `getShip`.
Declaring `padX` inside the nested `if (!(tab == 3 && getShip() == ...))` body
does start its lifetime after `getShip`, but the function shrinks to 9676 B
because the `upgradeMode` block is left outside the `padX` scope.

New shape (landed):

```cpp
if (item->isShip()) {
    if (tab != 0) {
        if (!(tab == 3 && Globals::status->getShip() == this->selectedItem->ship)) {
            volatile int padX = 0;              // lifetime starts after getShip
            ... setPosition / setVisible / draw ...
            if (this->upgradeMode && tab == 1) { // moved inside the nested body
                ... upgradeMode buttons ...
            }
        }
    }
}
```

The move is semantically exact: the upgrade block only runs when `tab == 1`,
which implies `tab != 0` and `!(tab == 3 && ...)` is true. It is not a
behaviour change for any `tab` value. It puts `setPosition` through the
`upgradeMode` draw in the same cleanup region as `padX`, which is precisely
the original's row 40 (140 bytes), and leaves `getShip` in the outer cleanup
(row 39, 150 bytes).

### Row table (fn 0x1490d8)

Before (base, 43 differing rows):

```
39  orig (2508, 150, 9664)  ours (2508, 118, 9664)   getShip LP (outer vs padX)
40  orig (2696, 140, 9562)  ours (2654, 182, 9514)   getShip..upgradeMode
```

After (landed, 149 differing rows):

```
39  orig (2508, 150, 9664)  ours (2512, 148, 9666)   length 150 -> 148, start +4
40  orig (2696, 140, 9562)  ours (2706, 134, 9514)   length 140 -> 134, start +10
```

The record keeps 156 rows and 948 bytes; the differing-byte count falls from
676 to 296. The remaining row-39/40 offsets are the 4-byte code-size shift
introduced by the move (the first row is 688 vs 684 bytes) and the landing-pad
offsets, not a row-boundary mismatch.

### Variants measured (raw / aligned / extab diffbytes / size / row39 / row40)

| variant | raw | al | extab | size | row39 | row40 |
|---|---:|---:|---:|---:|---|---|
| base (`padX` before `if (tab != 0)`) | 1965 | 291 | 676 | 9700 | (2508,118,9664) | (2654,182,9514) |
| `padX` inside nested, upgrade block outside | 8610 | 754 | 760 | 9676 | (2488,146,9640) | (2678,40,9434) |
| `padX` inside nested + `padY` before upgrade | 6204 | 582 | 695 | 9696 | (2508,150,9662) | (2700,40,9456) |
| **move upgrade inside nested, `volatile int padX`** | **7811** | **842** | **296** | **9700** | **(2512,148,9666)** | **(2706,134,9514)** |
| move upgrade inside nested, `volatile int *padX` | 6493 | 603 | 252 | 9704 | (2508,150,9668) | (2700,140,9516) |
| move upgrade inside nested, no padX | 6731 | 734 | 718 | 9700 | (2508,362,9666) | (2884,22,9360) |
| `padX` after setPosition, upgrade inside | 6597 | 830 | 705 | 9708 | (2508,236,9672) | (2782,62,9428) |
| hoist `cond` before `padX`, upgrade inside | 2276 | 410 | 689 | 9700 | (2508,118,9666) | (2654,4,9328) |

The `volatile int *padX` form gives the best extab (252) but is 4 B too large;
the `volatile int padX` form is the only one with both 9700 B and the 948-byte
/ 156-row record, so it is the landed variant.

## 2. `Hud::draw`: no anchor alternative beats the `pivotY = dividerOffset` sink

The w10015 note's three suggestions were re-measured and new shapes were
tried. None keeps the original 9332 B / 224-byte frame while removing the
one-byte `strd r4,sl` vs `strd r4,r4` difference; the anchor remains the best
source shape.

| variant | raw | al | size | frame |
|---|---:|---:|---:|---:|
| **base (`pivotY = dividerOffset`)** | **1325** | **249** | **9332** | **224** |
| no anchor (`pivotY = 0`) | 8261 | 1009 | 9312 | 216 |
| hull-fill `pivotY = dividerOffset`, regen 0 | 7456 | 1028 | 9328 | 216 |
| shield-fill `pivotY = dividerOffset`, regen 0 | 8134 | 820 | 9324 | 216 |
| `dividerOffset` declared via a volatile load | 8275 | 1015 | 9312 | 216 |
| `volatile int dv = dividerOffset;` before regen | 7274 | 523 | 9336 | 224 |
| `volatile int dv = dividerOffset; (void)dv;` | 7546 | 524 | 9340 | 224 |
| `(void) *(volatile int *) &dividerOffset;` | 8841 | 1054 | 9296 | 224 |
| `*(volatile int *) &dividerOffset = dividerOffset;` | 8842 | 1065 | 9296 | 224 |
| `do { regen } while (0)` | 8261 | 1009 | 9312 | 216 |
| `do { regen } while (dividerOffset < 0)` | 7974 | 1763 | 9324 | 216 |
| `if (hasArmorRegen && dividerOffset >= 0)` | 7937 | 1080 | 9328 | 216 |
| `__builtin_prefetch(&dividerOffset + dividerOffset)` | 8812 | 1060 | 9300 | 224 |
| `switch (dividerOffset) { default: break; }` | 8261 | 1009 | 9312 | 216 |
| `__builtin_expect(hasArmorRegen, dividerOffset != 0)` | 1325 | 249 | 9332 | 224 |

The last row is byte-identical to the base (same 1-byte `0x163ff7` hunk), so
the guard hint is not a new lever. The anchor's cost remains exactly one byte
in the `strd` register field; every real use of `dividerOffset` in the regen
call costs at least one extra instruction or changes the frame.

## 3. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10109
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10109/fdiff.py _ZN3Hud4drawExxP9PlayerEgobjj
python3 /dev/shm/w10109/align_diff.py _ZN3Hud4drawExxP9PlayerEgobjj
python3 /dev/shm/w10109/fdiff.py _ZN12HangarWindow6renderEv
python3 /dev/shm/w10109/align_diff.py _ZN12HangarWindow6renderEv
python3 /dev/shm/w10109/rows.py 0x1490d8
cmake --build cmake-build-match --target check -j6
```

Scratch harness in `/dev/shm/w10109`: `fdiff.py`, `align_diff.py`,
`measure.py`, `rows.py`, `hud_sweep.py`, `hangar_sweep2/3/4.py`,
`hangar_move.py`, `hangar_hoist.py`, `Hud.base.cpp`,
`HangarWindow.base.cpp`.
