# HangarWindow::render — wave 191 (w13522): ship-body alignment, raw 1,038 -> 928

Worktree `wt/w13522`, branch `wave191b-w13522`, base master `5c99979c4`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Target `_ZN12HangarWindow6renderEv` (`0x1490d8`), 9,700 B, 3,734 insns.

## Result

One source change in the ship guard: hoist the `tab` snapshot above the
guard, and use the original's setPosition/setVisible/draw body order and
buttons[2]-setVisible-first upgrade order.

```diff
-                            if (tab == 0) { } else if (__builtin_expect(tab != 3 || Globals::status->getShip() != this->selectedItem->ship, 1)) {
-                                (*this->buttons)[1]->setVisible(true);
-                                int upgTab = tab;
-                                (*this->buttons)[1]->draw();
-                                (*this->buttons)[1]->setPosition(...);
-                                buttonOffset = ...;
-                                if (__builtin_expect(upgTab == 1 && this->upgradeMode, 1)) {
-                                    (*this->buttons)[10]->setPosition(...);
-                                    (*this->buttons)[2]->setVisible(this->upgradeMode);
-                                    ...
+                            int upgTab = tab;
+                            if (tab == 0) { } else if (__builtin_expect(tab != 3 || Globals::status->getShip() != this->selectedItem->ship, 1)) {
+                                (*this->buttons)[1]->setPosition(...);
+                                (*this->buttons)[1]->setVisible(true);
+                                (*this->buttons)[1]->draw();
+                                buttonOffset = ...;
+                                if (__builtin_expect(upgTab == 1 && this->upgradeMode, 1)) {
+                                    (*this->buttons)[2]->setVisible(this->upgradeMode);
+                                    (*this->buttons)[10]->setPosition(...);
+                                    ...
```

| metric | before | after |
|---|---:|---:|
| raw same-address diff | 1,038 | **928** |
| instruction-index alignment | 620 | **469** |
| function size | 9,700 / 9,700 | **9,700 / 9,700** |
| instructions | 3,726 / 3,734 | 3,727 / 3,734 |
| `.text` section diff | 71,416 | **71,306** |
| `.ARM.extab` section diff | 577 | 1,216 |
| `.rodata` section diff | 485 | 485 |
| `.note.gnu.build-id` | 20 | 20 |
| total differing bytes | 72,498 | 73,027 |
| file size | 2,187,328 | 2,187,328 |

`check` green (RATCHET PASS): byte_exact 4507/4524, linked_exact 4507/4524,
size-exact 4524/4524, stub_zero_size 0, missing 0, wrong_type 0, extra 0,
data_exact 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.
`.rel.plt` byte-identical (0), `.ARM.exidx` byte-identical (0), all section
sizes and the file size exact.

## Why it works

The original's ship body is laid out at `0x149b44` with the order
`setPosition, setVisible, draw`:

```
orig 149b1e  ldr r0,[sp,#108]
     149b20  ldrd r5,r4,[sp,#132]
     149b24  cbz r0,149b96
     149b26  cmp r0,#3
     149b28  bne.n 149b44
     149b2a  ldr r0,[pc,#340]
     149b2c  ldr r1,[r4,#104]
     149b2e  add r0,pc
     149b30  ldr r0,[r0,#0]
     149b32  ldr r4,[r1,#12]
     149b34  ldr r0,[r0,#0]
     149b36  blx getShip
     149b3a  cmp r4,r0
     149b3c  ldrd r5,r4,[sp,#132]
     149b40  beq.w 14a6d0
     149b44  body (setPosition, setVisible, draw, getWidth)
```

The committed wave-190 body was `setVisible, draw, setPosition` with
`int upgTab = tab;` after the first call, so the guard was 36 B and the body
84 B (net 120 B).  Hoisting the snapshot above the guard makes clang keep
`this` in r4 across the `getShip` call (no `ldrd` reload), so the guard is
38 B and the body is the original's 80 B: the whole body lands at the
original addresses and matches instruction-for-instruction.

Region same-address diffs (raw 928): blueprint 50, ship 27 (was 159),
buylist 89, free-credits 674, list 13, rest 74, guard/upgrade/switch/
footer/tail 0.

## Cost: the LSDA record grows

The hoisted snapshot is in scope across the `getShip` call, so clang gives
`getShip` its own cleanup scope.  The render LSDA record grows from
`diffbytes 38` to `677`:

```
orig  row 39 (2508, 150, 9664)   row 40 (2696, 140, 9562)
ours  row 39 (2508, 118, 9666)   row 40 (2654, 4, 9328)   row 41 (2696, 140, 9588)
```

The extra row shifts the call-site table, so `.ARM.extab` diff grows
577 -> 1,216 (+639) while `.text` drops 71,416 -> 71,306 (-110).  Net total
differing bytes +529.  This is a raw/same-address win, not a total-byte win;
both are reported above.

## Negative results measured this wave (all size 9,700 unless stated)

* `switch (count & 0x7fffffff)` (documented original source form): +4 B
  (9,704), text 379,199 / extab 1,425 -- the mask forces the second
  buy-list String temp into `sl` and adds a tbb `nop`.
* Ship-guard operand flip `selectedItem->ship != getShip()`: raw 1,043,
  text 71,421, extab 577 (total 72,503) -- worse than committed.
* `3 != tab`, `0 == tab`, `1 == upgTab`, `tab == 1` in the check: raw 1,038
  (identical bytes).
* Guard without `__builtin_expect`: 9,704 B, raw 4,322.
* Separate `if (tab == 1 && upgradeMode)` after the guard (the shape that
  should give the original's `cbz`): 9,700 B but text 77,260 / extab 19,007
  (raw 6,882).  Clang threads the `tab == 0` edge past the check to the far
  continuation (`cmp`+`beq.w` instead of `cbz`) in every form tried:
  `if (tab != 0 && ...)`, `goto`, `switch`, `if (tab == 1) { if (...) }`,
  `volatile` snapshot, bool-guard, `(void)` shadow.  No form produced a
  `cbz` at `0x149b24`.
* Decl-at-body-start (correct row-39 `(2508,150)`, extab 586-587) always
  pays the +4 B `cmp`+`beq.w` and reloads `this`/layout after `getShip`,
  so the body shifts +4: raw 1,186-1,196, text 71,564-71,574.
* Buy-list String declaration order (`description` first, one DeclStmt,
  `= ""`, default ctor) does not move the description temp into r6.
* `Layout *lay` hoists, free-credits button orders, buttons[17] orders:
  unchanged from the wave-190/191 notes.

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

Scratch (not committed): `/home/debian/GalaxyOnFire2/scratch/w13522/`
(`try.py`, `regiondiff.py`, `secdiff.py`, `sweep1.py`..`sweep17.py`).
