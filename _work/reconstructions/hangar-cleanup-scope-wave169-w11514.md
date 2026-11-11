# HangarWindow::render — ship-branch cleanup scope (wave 169, w11514)

Worktree `wt/w11514`, branch `wave169-w11514`, base master `00c403d5a`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Target: `_ZN12HangarWindow6renderEv`, 9,700 B, `.ARM.extab` record 948 B / 156 rows.

## Entry state (wave-169 tab snapshot, commit 88b34bf4e)

| metric | value |
|---|---:|
| raw same-address diff | 1,970 |
| instruction-index alignment | 294 |
| function size | 9,700 / 9,700 |
| its `.ARM.extab` record | 948 B / 156 rows, diff **676** |
| `.text` / `.ARM.extab` section diff | 81,642 / 1,247 |

The snapshot left `getShip()` in the ship-branch cleanup scope, so LSDA row 39
encoded length 118 (1-byte ULEB) instead of the original's 150 (2-byte ULEB);
the whole call-site table then shifted by one byte.

## Landed shape

```cpp
if (item->isShip()) {
    if (__builtin_expect(tab != 0, 1) &&   // lint: arm_builtin anchor
        !(tab == 3 && this->selectedItem->ship == Globals::status->getShip())) {
        int upgTab = tab;                  // zero-cost EH scope anchor
        ... buttons[1] ...
        if (__builtin_expect(upgTab == 1, 0) && this->upgradeMode) { ... }
    }
}
```

The `int upgTab = tab;` declaration inside the `if` body is the scope anchor:
it moves `getShip()` into the outer cleanup scope (row 39 now spans the call,
length 150) while the buttons/upgrade body stays in the inner scope (row 40).
The two `__builtin_expect` hints keep the size at 9,700 and the block order
close to the original.  The comparison operand order is `selectedItem->ship ==
status->getShip()` so the ship pointer is loaded before the call, as in the
original.

| metric | before | after |
|---|---:|---:|
| raw same-address diff | 1,970 | **2,266** |
| instruction-index alignment | 294 | **386** |
| function size | 9,700 | **9,700** |
| instructions | 3734/3734 | **3734/3734** |
| its `.ARM.extab` record | 948 B / 156 rows, diff 676 | **948 B / 156 rows, diff 93** |
| `.text` section diff | 81,642 | 81,938 |
| `.ARM.extab` section diff | 1,247 | **709** |
| total differing bytes | 82,889 | **82,667** |

LSDA row 39 now matches the original exactly; row 40 still keeps a different
landing-pad thunk (26-byte row at 2672 vs the original's 140-byte row at 2696).
The record header is byte-identical (`len 927`, 156 rows).

The raw metric regresses by 296 bytes because the `__builtin_expect` on
`tab != 0` restores the wave-164 block order (switch body before the ship body)
instead of the snapshot's original order.  The `.ARM.extab` record drops by 583
bytes, so total differing bytes improve by 222.  Raw is the ground truth, so
this is a deliberate record-fix trade; a shape that keeps both the snapshot
block order and row 39's 150-byte length was not found in the timebox.

## Shapes measured (size must stay 9,700)

| shape | raw | aligned | size | record |
|---|---:|---:|---:|---:|
| entry (snapshot, no anchors) | 1,970 | 294 | 9,700 | 676 |
| snapshot + expect(tab!=0) only | 2,249 | 405 | 9,700 | 460 |
| snapshot + expect(upgTab==1,1) only | 1,965 | 299 | 9,700 | 676 |
| upgTab inside body, no expect | 6,506 | 583 | 9,704 | 253 |
| upgTab inside body + expect(tab!=0) | 6,309 | 414 | 9,696 | 259 |
| upgTab inside body + both expects (landed) | 2,266 | 386 | 9,700 | **93** |
| landed without operand flip | 2,264 | 623 | 9,700 | 94 |
| wave-164 shape (vol+expect+padX) | 2,245 | 620 | 9,700 | 97 |
| `std::swap(buttonOffset,buttonOffset)` on entry | 1,970 | 294 | 9,700 | 676 |
| `goto` check label on entry | 2,011 | 295 | 9,700 | 676 |
| volatile `tab` | 8,824 | 2,734 | 9,700 | 846 |

Gate after landing: `RATCHET PASS`, byte_exact 4507/4524, linked_exact 4507/4524,
size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370,
`.rel.plt` byte-identical (0).

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN12HangarWindow6renderEv
python3 _work/reconstructions/extab_lsda_decode.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so | grep -A2 'fn 0x1490d8'
cmake --build cmake-build-match --target check -j2
```
