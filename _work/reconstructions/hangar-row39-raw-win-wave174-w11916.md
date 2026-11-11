# HangarWindow::render — row-39/record residual and the ship-compare operand flip (wave 174, w11916)

Worktree `wt/w11916`, branch `wave174-w11916`, base master `be9dd429b`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Target: `_ZN12HangarWindow6renderEv` (`0x1490d8`), 9,700 B, 3,734 insns,
`.ARM.extab` record 948 B / 156 rows.  Entry state (wave-173 landing,
commit `be9dd429b`):

| metric | before | after |
|---|---:|---:|
| raw same-address diff | 2,244 | **2,242** |
| instruction-index alignment | 382 | 621 |
| function size | 9,700 / 9,700 | **9,700 / 9,700** |
| instructions | 3734 / 3734 | 3735 / 3734 |
| its `.ARM.extab` record diff | 90 | **91** |
| row 39 (orig / ours) | 150 / 150 | 150 / 150 |
| `.text` section diff | 80,897 | **80,895** |
| `.ARM.extab` section diff | 675 | **676** |
| total differing bytes | 81,592 | **81,591** |

`check` green: byte_exact 4507/4524, linked_exact 4507/4524, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370,
`.rel.plt` byte-identical (0).

## Landed change

One operand-order flip in the selected ship branch:

```cpp
// before
if (__builtin_expect(!(tab == 3 && this->selectedItem->ship == Globals::status->getShip()), 1))
// after
if (__builtin_expect(!(tab == 3 && Globals::status->getShip() == this->selectedItem->ship), 1))
```

Semantically identical (pointer equality is symmetric).  It reorders the
`getShip()` call ahead of the `this` reload in the `tab == 3` block, which
matches the original's `cmp r4,r0` operand/register shape 2 bytes better.
The block order is unchanged (still the wave-173 flipped order), so the
EH rows are unchanged except for the record diff 90 -> 91.

## Why the original block order still fails the record gate

The original's ship body is contiguous (guard + buttons at
`0x149b1e..0x149b94`, upgrade check at `0x149b96`), and its `tab == 0`
edge is a 2-byte `cbz r0, 0x149b96` to the near upgrade check.  Every
9,700 B shape we can build puts the non-ship switch body first and uses
the flag-sharing `cmp #0; b.n body` form; the original order costs
`cmp; beq.w` (4 extra bytes) and is 9,704 B.

The closest original-order shape is the `upg_where:"outer"` form:

```cpp
if (item->isShip()) {
    int upgTab = tab;
    if (tab != 0) {
        if (!(tab == 3 && this->selectedItem->ship == Globals::status->getShip())) {
            buttons[1]...;
        }
    }
    if (upgTab == 1 && upgradeMode) { buttons[2]...; buttons[10]...; }
}
```

It is size-exact **9,700 B** with the original block order and measures
raw **1,990**, aligned **306** — a large win — but its `.ARM.extab`
record diff is **676**, because the EH row 39/40 split lands on the wrong
side of the `getShip()` call:

```
original rows:  39 (2508,150,9664)  40 (2696,140,9562)  41..46 exact
outer rows:     39 (2508,118,9664)  40 (2658,178,9512)  41..46 exact
```

Row 39 ends after `isShip` (118 B) instead of after `getShip` (150 B), so
the ULEB128 length changes from two bytes to one and the whole call-site
table shifts by one byte — that shift alone is ~660 of the 676 differing
record bytes.  The `getShip()` invoke unwinds to the buttons-body landing
pad (`%1765`) instead of the `isShip` body pad (`%452`) because clang
evaluates the `if` condition inside the `if` body's cleanup scope; every
attempt to hoist the comparison (bool temp, `goto`, `switch`, explicit
`default`, compound `if`, `do/while(0)`, block scope) is either sunk back
into the body by the optimizer or changes the code shape (size 9,676 /
9,704 / 9,716).  Adding a may-throw call in the `isShip` body scope does
push row 39 to a two-byte length (130 -> record 294), but it shifts the
whole ship body and costs ~5,000 raw bytes.  So the original-order shape
is not landable under the record <= 100 gate.

## Shapes measured (size must stay 9,700, record <= 100)

| shape | raw | aligned | size | record |
|---|---:|---:|---:|---:|
| wave-173 landed (nested guard) | 2,244 | 382 | 9,700 | 90 |
| **ship-compare operand flip (landed)** | **2,242** | **621** | **9,700** | **91** |
| `or` (`tab != 3 || ship != getShip()`) | 2,244 | 382 | 9,700 | 90 |
| `or` + flip | 2,242 | 621 | 9,700 | 91 |
| `bo_swap` (`rowHeight + getWidth()`) | 2,246 | 453 | 9,700 | 90 |
| `upgTab`/`tab`/`0 !=` operand swaps | 2,244 | 382 | 9,700 | 90 |
| `const int upgTab` + flip | 2,242 | 621 | 9,700 | 91 |
| original order, upgrade outside (`outer`) | 1,990 | 306 | 9,700 | 676 |
| `outer` + `expect(tab != 0, 1)` | 2,262 | 641 | 9,700 | 464 |
| `outer` + `expect(inner, 1)` | 1,974 | 306 | 9,700 | 676 |
| `outer` + `upgradeMode` first | 1,985 | 303 | 9,700 | 676 |
| `outer` + dummy `getShip()` after `SetColor` | 6,931 | 633 | 9,716 | 294 |
| `shipvar` (hoisted `getShip`, original order) | 8,646 | 731 | 9,676 | 764 |
| `shipvar2` (`bool same`, size-exact) | 2,531 | 820 | 9,700 | 690 |
| `body_switch` (`switch(tab){0;3;default}`) | 6,727 | 736 | 9,700 | 730 |
| `swap` (`if !isShip` then switch else ship) | 2,362 | 631 | 9,700 | 93 |

The operand flip is the only size-exact raw win that keeps the record at
or below 100.  It costs 239 aligned instructions (one extra `ldr`/`ldr`
in place of the original `ldrd`); the raw same-address diff is the primary
residual metric and improves.

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
