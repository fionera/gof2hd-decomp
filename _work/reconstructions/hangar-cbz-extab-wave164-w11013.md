# HangarWindow::render — the `tab == 0` `cbz` and the `.ARM.extab` cost (wave 164, w11013)

Worktree `wt/w11013`, branch `wave164-w11013`, base master `b3d3a1f68` (wave 163).
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Target: `_ZN12HangarWindow6renderEv`, 9,700 B, `.ARM.extab` record 948 B / 156 rows.

## Result (landed)

| metric | before (wave 163) | after |
|---|---:|---:|
| raw same-address diff | 2,514 | **2,245** |
| instruction-index alignment | 485 | 620 |
| function size | 9,700 | **9,700** |
| its `.ARM.extab` record diff | 151 | **97** |
| total `.ARM.extab` diff | 725 | **671** |
| `.text` diff | 84,652 | **84,383** |
| `.rel.plt` diff | 0 | **0** (byte-identical) |
| all section sizes | exact | **exact** |

`check` green: byte_exact 4507/4524, linked 4507/4524, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.

## The landed shape

The wave-162 ship branch carried a dead `volatile int *dummy; (void) dummy;`
"LSDA sink" whose only purpose was to split the cleanup region.  Replacing it
with a volatile read of `this->scrollOffset` keeps the same 156-row / 948-byte
record but shifts the whole cleanup block by 2 bytes, which is exactly the
+2 offset every landing pad in the record had:

```cpp
if (item->isShip()) {
    // Volatile read of scrollOffset ...
    (void) *(volatile int*)&this->scrollOffset;
    if (__builtin_expect(tab != 0, 1)) { // lint: arm_builtin anchor ...
        if (!(tab == 3 && Globals::status->getShip() == this->selectedItem->ship)) {
            volatile int padX[1];
            ...
```

Effect on the LSDA (row = start, length, landing pad; LP is relative to
`0x1490d8`):

| row | original | wave-163 | landed |
|---|---|---|---|
| 36 | 2336,52,9664 | 2336,52,9666 | **2336,52,9664** |
| 37 | 2412,30,9454 | 2412,30,9456 | **2412,30,9454** |
| 38 | 2456,34,9334 | 2456,34,9334 | **2456,34,9334** |
| 39 | 2508,150,9664 | 2508,158,9666 | 2508,156,9664 |
| 40 | 2696,140,9562 | 2680,26,9356 | 2678,26,9356 |
| 41 | 2854,6,9664 | 2718,4,9224 | 2716,4,9224 |

Rows 36-38 (and every LP before the ship branch) are now byte-equal to the
original; the residual 97 bytes are the row-39/40 split plus the wave-163
`getShip` operand-order difference (the original loads
`selectedItem->ship` *before* the `Status::getShip()` call, we load it
after).

## Why the `cbz` still costs the extab

The original's dispatch is

```
149b1e: ldr r0,[sp,#108]      ; tab
149b20: ldrd r5,r4,[sp,#132]
149b24: cbz r0, 149b96        ; tab == 0 -> the `tab == 1 && upgradeMode` check
149b26: cmp r0, #3
149b28: bne.n 149b44          ; tab != 3 -> buttons[1]
```

so the upgrade check must sit **outside** the `if (tab != 0)` block.  With it
outside, `getShip` (in the `&&` RHS) and the buttons block share one
landing-pad block in the IR (`getShip`'s invoke unwinds to the same
`landingpad` as the `setPosition`/`setVisible`/`draw` invokes), so LSDA row 39
stops at the `isShip` call:

* original row 39 = `2508,150,9664` (ends after `Status::getShip`),
* upgrade-out row 39 = `2508,118,9664` (ends after `ListItem::isShip`).

118 is a 1-byte ULEB, 150 is a 2-byte ULEB, so every later row shifts by one
byte: that single fact is the whole 151 -> 676 record-diff jump.  The
`__builtin_assume` pair that produces the `cbz` is irrelevant to the LSDA —
the same rows appear without it (raw 1,970, aligned 294, no `cbz`).

With the upgrade check **inside** the block (wave-162/landed shape) `getShip`
gets its own landing pad and row 39 covers it, which is why the record stays
near the original.

### Measured `cbz` shapes (all size-exact, 9,700 B)

| shape | raw | aligned | record diff | total extab | `cbz` |
|---|---:|---:|---:|---:|---|
| landed (wave 164) | 2,245 | 620 | 97 | 671 | no |
| wave-163 w10916 | 1,979 | 551 | 676 | 1,250 | yes |
| W2 (volatile read hoisted before `isShip`) | 2,265 | 541 | 676 | 1,250 | yes |
| upgrade-out, no assumes | 1,970 | 294 | 676 | 1,250 | no |
| upgrade-out + `__builtin_expect` | 2,260 | 398 | 461 | 1,035 | no |

No size-exact shape combines the `cbz` with the 948 B / 156-row record at
`<= 725`.

## What was tried to fix the row-39 split

All of these keep the `cbz` but change the record size or the row split:

* adding a local (`volatile int padX2[1];`, `int padX2[1];`, `String`) inside
  the `if` body gives `getShip` its own landing pad again (row 39 >= 128) but
  the record becomes 960 B / 158 rows (size 9,692-9,716);
* `switch (tab)` / `if (tab == 0) {} else if (...)` dispatch: `cbz` present,
  156 rows, but 9,696 B and record diff 708;
* swapping the `==` operands, `goto`-based skip, `do { } while (0)` skip,
  `||`-form condition, `bool same = ...`: no change to the row split.

The remaining lever is making the buttons block its own cleanup scope without
emitting code, which clang does not do for any construct tried here.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w11013
cmake --build cmake-build-match --target link_so -j8
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN12HangarWindow6renderEv
python3 _work/reconstructions/extab_record_map.py     # TOTAL 671
cmake --build cmake-build-match --target check -j8    # RATCHET PASS
```
