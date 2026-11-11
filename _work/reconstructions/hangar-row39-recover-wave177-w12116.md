# HangarWindow::render — row-39 LSDA recovered exactly (wave 177, w12116)

Worktree `wt/w12116`, branch `wave177-w12116`, base master `9445ac5fb`
(wave-176 merge). Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Target `_ZN12HangarWindow6renderEv` (`0x1490d8`), 9,700 B, 3,734 insns,
`.ARM.extab` record 948 B / 156 rows.

## Result

The wave-176 landing (w12014) reproduced the original ship-guard block order
and dropped the raw diff 2,242 -> 1,965, but it moved the `Status::getShip`
invoke out of the `isShip` cleanup scope: row 39 shortened 150 -> 118, the
length encoded in one ULEB byte instead of two, and the whole call-site table
shifted by one byte (record diff 676, `.ARM.extab` section diff 1,321).

This pass recovers row 39 **exactly** — `(2508, 150, 9664)` — while keeping
the function size-exact at 9,700 B:

```cpp
if (item->isShip()) {
    if (__builtin_expect(tab != 0, 1) && // lint: arm_builtin
        __builtin_expect(tab != 3 || this->selectedItem->ship != Globals::status->getShip(), 1)) { // lint: arm_builtin
        int upgTab = tab;
        ... buttons[1] body ...
        if (__builtin_expect(upgTab == 1, 0) && this->upgradeMode) { // lint: arm_builtin
            ... buttons[2]/[10] ...
        }
    }
}
```

| metric | before (wave-176 base) | after |
|---|---:|---:|
| raw same-address diff | 1,965 | 2,244 |
| instruction-index alignment | 291 | 382 |
| function size | 9,700 / 9,700 | **9,700 / 9,700** |
| instructions | 3,734 / 3,734 | **3,734 / 3,734** |
| its `.ARM.extab` record diff | 676 | **90** |
| LSDA cst_len | 926 / 927 | **927 / 927** |
| row 39 (orig / ours) | 150 / 118 | **150 / 150** |
| `.text` section diff | 79,591 | 79,870 |
| `.ARM.extab` section diff | 1,321 | **735** |
| total differing bytes | 80,932 | **80,625** |

`check` green: byte_exact 4507/4524, linked_exact 4507/4524, size-exact
4524/4524, data_exact 843/843, imports 207, lint CLEAN, drift 0,
parity 5370/5370, `.rel.plt` byte-identical (0).  `.ARM.extab` size/base
exact.  File size exact.

## The tradeoff: raw vs the row-39 record

The two requirements are on opposite sides of one codegen decision.  The
original emits

```
149b1a cmp  r0,#0            ; isShip()
149b1c beq.n 149bf0          ; non-ship switch
149b1e ldr  r0,[sp,#0x6c]    ; tab
149b20 ldrd r5,r4,[sp,#132]
149b24 cbz  r0,149b96        ; tab == 0 -> near tab == 1 check (2 B)
...
149b94 ldr  r0,[sp,#0x6c]    ; tab reload
149b96 cmp  r0,#1            ; check
```

`cbz` is only 2 B because the `tab == 0` edge targets the *near* check block
(the fall-through of the body).  If clang instead threads `tab == 0` straight
to the merge block, the edge becomes `cmp r0,#0; beq.w <merge>` (6 B, +4).

Row 39 is the contiguous call-site range with LP 9664.  It ends at the last
invoke in the `isShip` cleanup scope.  With the `getShip()` invoke inside that
scope (as in the original) row 39 spans `0x149aa4..0x149b3a` = 150 B.  With
`getShip()` sunk into the button-body scope it stops at `0x149b1a` = 118 B.

The wave-176 shape (`int upgTab = tab;` declared *before* the guard) creates a
lexical scope at the declaration, so `getShip()` is captured by the body
landing pad -> row 39 = 118.  Declaring the snapshot *inside* the body instead
puts `getShip()` back in the `isShip` scope (row 39 = 150) but the `tab == 0`
edge threads to the merge, which costs 4 B and makes the function 9,704 B.

The branch hints resolve it: `__builtin_expect(tab != 0, 1)` plus the inner
hint make clang keep the ship body out of line and emit the 2-byte `cbz` to
the near check, so the function stays exactly 9,700 B with row 39 = 150.
The price is the block layout: the non-ship `switch` is laid out before the
ship body, which is the wave-175 order and accounts for essentially all of the
+279 raw bytes (the wave-175 base was 2,242).

The row-39 recovery is therefore a net win on the section bytes
(`.ARM.extab` 1,321 -> 735, total 80,932 -> 80,625) but gives back the wave-176
`.text` win (79,591 -> 79,870).  No shape found in this pass achieves
row 39 = 150 with raw <= 1,965; the two are mutually exclusive in every
combination tried (see below).

## Row-39 / record at the target shape

```
orig rows:  35 (2268,10,9226) 36 (2336,52,9664) 37 (2412,30,9454) 38 (2456,34,9334)
            39 (2508,150,9664) 40 (2696,140,9562) 41..47 exact
ours rows:  39 (2508,150,9664)  40 (2672,26,9356)  41 (2710,4,9224) ...
```

Rows 35-39 and 45-47 are byte-identical; rows 40-44 differ only because the
ship body is out of line.  `cst_len` is 927/927, so the one-byte shift that
dominated the 676-byte record diff is gone; the residual 90 bytes are the
row 40-44 values.

## Shapes measured this pass

All rows are full build + `tools/fn_diff.py` + `extab_lsda_decode` row dump.

| shape | raw | al | size | insn | rec | row39 (ours) |
|---|---:|---:|---:|---:|---:|---|
| wave-176 base | **1,965** | **291** | 9,700 | 3,734 | 676 | (2508,118,9664) |
| **landed: expect(tab)+expect(inner)+`int upgTab` in body+expect(upg,0)** | 2,244 | 382 | **9,700** | **3,734** | **90** | **(2508,150,9664)** |
| same, inner hint dropped | 2,266 | 386 | 9,700 | 3,734 | 93 | (2508,150,9664) |
| same, `upgTab`-hint dropped | 6,308 | 634 | 9,696 | 3,733 | 258 | (2508,150,9660) |
| same, `upgTab`-hint = 1 | 7,619 | 1,840 | 9,664 | 3,743 | 338 | (2504,158,9628) |
| same, no hints (plain compound guard + body snapshot) | 6,511 | 611 | 9,704 | 3,736 | 254 | (2508,154,9668) |
| body snapshot, check outside, no hints | 8,635 | 800 | 9,676 | 3,730 | 760 | (2488,152,9640) |
| body snapshot, check outside, hints | 2,244 | 382 | 9,700 | 3,734 | 90 | (2508,150,9664) |
| `volatile int padX[1]` body sink instead of the snapshot | 2,244 | 382 | 9,700 | 3,734 | 90 | (2508,150,9664) |
| `int dummy` body sink, check on `tab` | 2,244 | 382 | 9,700 | 3,734 | 90 | (2508,150,9664) |
| wave-175 `nested_expect` (base operand order) | 2,242 | 621 | 9,700 | 3,735 | 91 | (2508,152,9664) |
| wave-176 + `expect(inner)` | 1,956 | 295 | 9,700 | 3,734 | 676 | (2508,118,9664) |
| `credits_sel` + body snapshot | 2,332 | 536 | 9,700 | 3,733 | 86 | (2508,156,9664) |
| `short`/`char` snapshot | 8,624 | 1,575 | 9,708 | 3,738 | 329 | (2512,150,9672) |

The operand order matters: `this->selectedItem->ship != getShip()` (the
original's order, ship loaded before the call) is required for the exact
`(2508,150,9664)`; the flipped `getShip() != ship` gives `(2508,152,...)` or
`(2508,150,9668)` depending on the rest of the shape.

## Why raw cannot also stay at 1,965

The wave-176 shape's 1,965 relies on the ship body being laid out first
(immediately after the `isShip` test), which is exactly the layout that keeps
`getShip()` in the button-body cleanup scope.  Every shape that moves
`getShip()` into the `isShip` scope changes the block layout (non-ship switch
first) and loses ~279 raw bytes.  Shapes that keep the wave-176 layout and
still reach row 39 >= 128 are all 9,696-9,708 B or have a shifted region start
(`2488`), i.e. not size-exact.  The residual raw is dominated by the tail
register allocation (`sl`/`r9` swap for `&this->buttons`, `Globals::options`
spill), which is independent of the ship-branch shape.

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

The shape generators used for this sweep live in
`/home/debian/GalaxyOnFire2/scratch/w12116/` (`harness4.py`, `sweep88.py`,
`sweep90.py`-`sweep96.py`, `rowdump.py`); they rewrite the selected-item
block of `src/game/ui/HangarWindow.cpp` from
`scratch/w12116/HangarWindow4.tmpl.cpp`.
