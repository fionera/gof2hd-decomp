# HangarWindow::render — buttons-block cleanup-scope constructs + tab-snapshot win (wave 169, w11414)

Worktree `wt/w11414`, branch `wave169-w11414`, base master `ec3ceb07c`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Target: `_ZN12HangarWindow6renderEv`, 9,700 B, `.ARM.extab` record 948 B / 156 rows.

## Baseline on entry (wave-164 shape)

| metric | value |
|---|---:|
| raw same-address diff | 2,245 |
| instruction-index alignment | 620 |
| function size | 9,700 / 9,700 |
| its `.ARM.extab` record diff | 97 |
| `.text` / `.ARM.extab` section diff | 82,327 / 668 |

Gate on entry: byte_exact 4507/4524, linked 4507/4524, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.

## Constructs tried this wave (size must stay 9,700)

The coordinator asked for a buttons-block cleanup scope that emits no code.
All measurements below are on the wave-164 baseline shape.

| construct around the buttons block | raw | aligned | size | record |
|---|---:|---:|---:|---:|
| baseline | 2,245 | 620 | 9,700 | 97 |
| nested block + trivially-destructible `struct ScopeTag { int tag; }` | 8,530 | 828 | 9,680 | 753 |
| nested block + empty `struct ScopeTag { }` | 8,530 | 828 | 9,680 | 753 |
| `bool tabIsZero = (tab == 0)` wrapper | 6,516 | 838 | 9,704 | 703 |
| `std::swap(buttonOffset, buttonOffset)` | 2,245 | 620 | 9,700 | 97 |
| `for (int once = 0; once < 1; ++once)` around buttons[1] | 7,352 | 1,891 | 9,692 | 711 |
| file-static `hwTabNotZero(tab)` helper (inlined) | 2,245 | 620 | 9,700 | 97 |
| buttons[0] block: trivially-destructible nested block | 2,419 | 891 | 9,700 | 280 |
| buttons[0] block: `for`-once | 2,419 | 891 | 9,700 | 280 |
| buttons[0] block: local `bool showMainButton` | 2,384 | 877 | 9,700 | 122 |
| buttons[0] block: `std::swap` | 2,245 | 620 | 9,700 | 97 |

Confirms wave-166/168: a nested block alone is free, but any object
declaration in it changes the register allocation and the size. `std::swap`
of the same lvalue and a static helper that inlines are completely free (no
code, no record change). None of these improves raw.

## Landed win: snapshot `tab` before the ship guard

The original's ship branch reloads `tab` from `sp+108` before the upgrade
check (`0x149b94 ldr r0,[sp,#108]; 0x149b96 cmp r0,#1`), and its `tab == 0`
edge is a 2-byte `cbz` to that reload block (`0x149b24`), not a threaded
`cmp+beq.w` to the far join. Our baseline threaded the edge and laid the
non-ship `switch` body *before* the buttons block.

Introducing a distinct local snapshot of the tab before the guard

```cpp
int upgTab = tab;
if (tab != 0 && !(tab == 3 && Globals::status->getShip() == this->selectedItem->ship)) {
    ... buttons[1] ...
    if (upgTab == 1 && this->upgradeMode) { ... }
}
```

makes the two reads distinct values to the allocator. The block order then
matches the original (ship body first, `switch` body after), the call site
list is identical, and the instruction count is 3734/3734.

| metric | before | after |
|---|---:|---:|
| raw same-address diff | 2,245 | **1,970** |
| instruction-index alignment | 620 | **294** |
| function size | 9,700 | **9,700** |
| instructions | 3735/3734 | **3734/3734** |
| its `.ARM.extab` record | 948 B / 156 rows, diff 97 | 948 B / 156 rows, diff 676 |
| `.text` section diff | 82,327 | **82,052** |
| `.ARM.extab` section diff | 668 | 1,247 |

The `.ARM.extab` record is still 948 B / 156 rows; rows 41-44 now match the
original exactly, but rows 39-40 keep a different row grouping and the
landing-pad thunk table shifts by 2-8 bytes, so the record *diff bytes* rise
from 97 to 676 (section 668 -> 1,247). The `.text` win is -275 bytes; the
trade is deliberate per the raw-is-ground-truth rule.

`check` passes unchanged: byte_exact 4507/4524, linked 4507/4524, size
4524/4524, data 843/843, imports 207, lint CLEAN, parity 5370/5370.

## Also measured (not landed)

| shape | raw | aligned | size | record |
|---|---:|---:|---:|---:|
| `upgTab` + `__builtin_expect(tab != 0, 1)` + no padX/vol | 2,249 | 405 | 9,700 | 460 |
| `upgTab` + expect + padX + vol (wave-164 anchors) | 2,514 | 482 | 9,700 | 163 |
| `upgTab` + padX, no expect | 2,282 | 413 | 9,700 | 689 |
| `upgTab` + volatile scrollOffset read | 6,519 | 584 | 9,704 | 710 |
| `const int &upgTab = tab` / `int *upgTab = &tab` | same as landed | | 9,700 | 676 |
| `bool upgTab = (tab == 1)` | same as landed | | 9,700 | 676 |
| `upgTab` declared inside the `!(...)` body | 6,604 | 841 | 9,708 | 706 |
| `upgTab` declared before the `isShip` check | 6,752 | 775 | 9,704 | 721 |
| `int upgTab = getCurrentTab()` (real reload) | 8,647 | 720 | 9,684 | 762 |

## Reproduction

```sh
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN12HangarWindow6renderEv
python3 _work/reconstructions/extab_lsda_decode.py \
    /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
    cmake-build-match/libgof2hdaa.so | grep -A200 'fn 0x1490d8'
cmake --build cmake-build-match --target check -j2
```
