# HangarWindow::render — row-39 raw win via nested-guard branch weights (wave 173, w11816)

Worktree `wt/w11816`, branch `wave173-w11816`, base master `3fafa5b03`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Target: `_ZN12HangarWindow6renderEv`, 9,700 B, `.ARM.extab` record 948 B /
156 rows.  Entry state (wave-171 landing, commit `9c44b964f`):

| metric | before | after |
|---|---:|---:|
| raw same-address diff | 2,266 | **2,244** |
| instruction-index alignment | 386 | **382** |
| function size | 9,700 / 9,700 | **9,700 / 9,700** |
| instructions | 3734 / 3734 | 3734 / 3734 |
| its `.ARM.extab` record diff | 93 | **90** |
| row 39 length (orig / ours) | 150 / 150 | 150 / 150 |
| `.text` section diff | 81,503 | **81,481** |
| `.ARM.extab` section diff | 711 | **708** |
| total differing bytes | 82,234 | **82,209** |

`check` green: byte_exact 4507/4524, linked_exact 4507/4524, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370,
`.rel.plt` byte-identical (0).

## Landed shape

The wave-169 shape is a compound guard:

```cpp
if (__builtin_expect(tab != 0, 1) &&
    !(tab == 3 && this->selectedItem->ship == Globals::status->getShip())) {
    int upgTab = tab;
    ... buttons[1] ...
    if (__builtin_expect(upgTab == 1, 0) && this->upgradeMode) { ... }
}
```

Splitting the compound guard into two nested `if`s and putting a branch
weight on the inner guard:

```cpp
if (__builtin_expect(tab != 0, 1)) {
    if (__builtin_expect(!(tab == 3 &&
                           this->selectedItem->ship == Globals::status->getShip()), 1)) {
        int upgTab = tab;
        ... buttons[1] ...
        if (__builtin_expect(upgTab == 1, 0) && this->upgradeMode) { ... }
    }
}
```

is semantically identical (for `tab == 0` the outer guard is false; for
`tab == 3` with the same ship the inner guard is false; otherwise the body
runs and the upgrade check follows) and produces the **same main-code
instructions**.  The only machine-code difference is in the EH landing-pad
thunk table at `0x14b5b8..0x14b632`: the inner `!prof` metadata changes the
cleanup-region ordering, which moves the `sp+324` thunk from `0x14b620` to
`0x14b614` and shifts the six `b 0x14b64e` thunks.  That reordering matches
the original's thunk table 22 bytes better, and the record header/rows 3
bytes better.

## Why the block order still flips

The original's `tab == 0` edge is a 2-byte `cbz` to the near `tab == 1`
check.  Clang's JumpThreading folds it to the far merge because the check
block costs 5 (`getelementptr + load + icmp + icmp + and`, threshold 6), so
the 9,700 B shapes need the flag-sharing `cmp #0; b.n body` form and place
the non-ship switch body before the ship body.  The wave-163 `__builtin_assume`
trick buys the `cbz` back but leaves row 39 at 118 and the record at 676.
The wave-169 anchor (check inside the body) fixes row 39 to 150 but keeps
the flipped order.  This pass keeps the wave-169 record and improves the
thunk table; it does not restore the original block order.

## Shapes measured (size must stay 9,700, record <= 100)

| shape | raw | aligned | size | record |
|---|---:|---:|---:|---:|
| wave-169 landed (compound guard) | 2,266 | 386 | 9,700 | 93 |
| **nested guard + inner `expect(...,1)` (landed)** | **2,244** | **382** | **9,700** | **90** |
| nested guard, inner `expect(...,2/3/5/10)` | 8,657 | 736 | 9,684 | 338 |
| nested guard, inner `expect(...,0)` | 7,515 | 795 | 9,724 | 332 |
| nested guard, no inner expect | 2,266 | 386 | 9,700 | 93 |
| nested guard, check `expect(...,1)` | 7,619 | 1,840 | 9,664 | 338 |
| nested guard, `expect(upgTab==1 && upgradeMode, 0)` | 6,353 | 642 | 9,696 | 258 |
| compound guard + `expect` on whole check | 6,353 | 639 | 9,696 | 258 |
| operand-flipped ship compare (`getShip() == ship`) | 2,242 | 621 | 9,700 | 91 |
| wave-163 `cbz` shape (assumes + volatile read) | 1,979 | 551 | 9,700 | 676 |
| wave-163 + compound guard | 1,973 | 569 | 9,700 | 676 |

The operand flip gives 2 fewer raw bytes but costs 239 aligned instructions
(9,700 B still, record 91), so the nested-guard shape is kept.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN12HangarWindow6renderEv
python3 _work/reconstructions/extab_record_map.py
cmake --build cmake-build-match --target check -j2
```
