# `Level::createShip` + `.ARM.extab` rows — wave 155 / w10112:
# new createShip source hypotheses all coalesce; the extab row-39 fix
# (padX inside the inner `if`) needs 4 more bytes that no source shape adds

Status: **analysis only, no landed `src/` change** (base is already best).
Worktree `wt/w10112`, base `f4e0ea40b` (wave 154). Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370.

| metric | base |
|---|---:|
| `Level::createShip` raw / aligned / size | 1564 / 452 / 2764 |
| `HangarWindow::render` raw / aligned / size | 1965 / 291 / 9700 |
| `HangarWindow::render` `.ARM.extab` diff bytes | 676 (43 differing rows) |

## 1. `Level::createShip` — every new hypothesis coalesces to the landed body

The landed body is wave-154 w10016 (`if (mission == 56) … else if (>=49 && <=52)`,
hitpoints-first, `x = 0` status guard).  Entry map is unchanged: `status r4 /
rnd r5 / nextInt r6`, `type sl`, `race [sp,#56]`, `shipClass [sp,#60]`,
`jitterX fp`.  All measurements are full link + relink, exact TU, clang 7.0.2
`-Oz`; the aligned column is the worktree copy of `align_diff.py`.

### New levers tried this wave (all size 2764 unless noted)

| variant | raw | aligned |
|---|---:|---:|
| base (landed) | **1564** | **452** |
| `int sc = shipClass;` (used everywhere) | 1564 | 452 (coalesced) |
| `int ty = type;` (used everywhere) | 1564 | 452 (coalesced) |
| both `sc` + `ty` locals | 1564 | 452 (coalesced) |
| `const int sc = shipClass;` | 1564 | 452 (coalesced) |
| `const int shipClass` / `const int type` / `const int race` parameter | 1564 | 452 (coalesced) |
| `int empTime = 15000; if (sc==1) { …; empTime=45000; }` | 1564 | 452 (coalesced) |
| `switch (shipClass) { case 1: … default: }` for empTime | 1564 | 452 (coalesced) |
| `int empTime;` declared at function top | 1564 | 452 (coalesced) |
| `float z,y; int x;` declared at function top | 1564 | 452 (coalesced) |
| `do { mission…jitterZ } while (0)` around the entry calls | 1564 | 452 (coalesced) |
| `do { mission = … } while (0)` only | 1564 | 452 (coalesced) |
| `if (shipClass != 0) { if (shipClass == 1) … } else …` (nested) | 1564 | 452 (coalesced) |
| `(unsigned) shipClass == 1u` | 1564 | 452 (coalesced) |
| `__builtin_expect(status == nullptr, 0)` | 1564 | 452 (coalesced) |
| `__builtin_expect(gameWon(), 0)` | 1564 | 452 (coalesced) |
| `__builtin_expect(type == 14, 0)` | 1564 | 452 (coalesced) |
| `(unsigned)(mission - 49) <= 3u` else-if | 1564 | 452 (coalesced) |
| `mission >= 49 && mission < 53` else-if | 1564 | 452 (coalesced) |
| `mission > 48 && mission < 53` else-if | 1564 | 452 (coalesced) |
| `mission - 49 <= 3` else-if (signed) | 2760 B | 2144 | 519 |
| `mission == 56 \|\| (unsigned)(mission-49) <= 3u` | 1577 | 457 |
| `int wx,wy,wz` declaration order (x first) | 1621 | 453 |
| jitters declared before the waypoint reads | 1603 | 557 |
| `Status *st = Globals::status;` local | 2800 B | 2455 | 951 |
| `AERandom *rnd = Globals::rnd;` local | 2768 B | 2295 | 537 |
| `level`/`hitpoints` before the jitter calls | 2808 B | 2539 | 538 |
| `empPoints` before the `switch (type)` | 1575 | 475 |
| `__builtin_expect(waypoint != nullptr, 1)` | 2315 | 497 |
| `__builtin_expect(shipClass == 0, 1)` | 2744 B | 2476 | 1302 |
| `__builtin_expect(shipClass == 1, 0)` | 2768 B | 2417 | 514 |
| `__builtin_expect(mission == 0x9a, 0)` | 2772 B | 2423 | 544 |
| `__builtin_expect(getLevel() > 20, 0)` | 2772 B | 1808 | 540 |
| `if (shipClass == 1)` → `> 0` / `!= 0` / `>= 1` | 2780 B | 2491 | 488 |
| `switch (shipClass) { case 0: …; case 1: … }` for ship creation | 2780 B | 2491 | 488 |
| `if (shipClass == 1) … else if (shipClass == 0) …` (reverse order) | 2796 B | 2378 | 701 |

**Conclusion.**  The `type` / `shipClass` / `race` register permutation is not
reachable from source: every local/const/do-while/expect/order spelling either
GVN-coalesces to the landed 1564/452 body or changes the size.  The residual is
the PGO spill-weight choice that keeps `shipClass` in `r6` and `type` in `fp`
in the original while spilling `race` and all three jitters; our build keeps
`type` in `sl`, `jitterX` in `fp` and spills `race`+`shipClass`.  The landed
body remains the best on both metrics.

## 2. `.ARM.extab` rows 39/40 — the padX-inside fix is 4 B short

`HangarWindow::render` LSDA (156 rows).  Base vs original:

```
row  orig                     ours                     note
 39  (2508, 150, 9664)        (2508, 118, 9664)        getShip LP outer vs padX
 40  (2696, 140, 9562)        (2654, 182, 9514)        getShip..upgradeMode draw
```

Base has 43 differing rows / 676 extab bytes.  Moving `volatile int *padX = 0;`
to the first statement of the inner `if` body moves the getShip invoke to the
outer cleanup (row 39 length 150, LP 9662) but the function shrinks to **9676 B**
and the extab diff rises to **760**.

### New sweep (all measured; raw / aligned / extab / size / row39 / row40)

| variant | raw | align | extab | size | row39 ours | row40 ours |
|---|---:|---:|---:|---:|---|---|
| **base** | **1965** | **291** | **676** | **9700** | (2508,118,9664) | (2654,182,9514) |
| `padX` inside inner if | 8630 | 800 | 760 | 9676 | (2488,146,9640) | (2678,40,9434) |
| inside + `if (padX)` after draw | 8630 | 800 | 760 | 9676 | same | same |
| inside + `padX = &tab` after draw | 8630 | 800 | 760 | 9676 | same | same |
| inside + `volatile int padX` + `if (padX)` | 8517 | 761 | 760 | 9680 | — | — |
| inside + `Array<TouchButton*>*btns` for 4 accesses | 7116 | 782 | 727 | 9696 | (2508,146,9660) | (2702,32,9454) |
| inside + `btns` for 8 accesses | 6743 | 653 | 727 | 9688 | (2508,146,9660) | (2702,32,9454) |
| `btns` at ship-branch top + 4 accesses | 6415 | 679 | 714 | 9696 | (2508,118,9662) | (2658,4,9446) |
| `btns` at ship-branch top + 8 accesses | 6827 | 652 | — | 9680 | — | — |
| **inside + `padY` after the block** | 6387 | 617 | 695 | 9696 | **(2508,150,9662)** | (2700,40,9456) |
| inside + `padY` after + extra dead locals | 6387 | 617 | 695 | 9696 | same | same |
| `volatile bool same` hoisted + padX before | 7615 | 885 | — | 9720 | — | — |
| `bool same` hoisted + `if (same){}` + padX before | 2559 | 659 | 730 | 9700 | (2508,118,9686) | (2646,4,9400) |
| padX declared before / assigned after getShip | 6828 | 629 | 722 | 9692 | (2516,118,9690) | (2662,4,9352) |
| split `if (tab != 0)` + padX between | 8873 | 2955 | — | 9724 | — | — |

### Row table for the best row-39 shape (`padX` inside + `padY` after, 9696 B)

```
row  orig                     ours                     note
  0  (0, 684, 0)              (0, 684, 0)              exact
  1  (684, 20, 9544)          (684, 20, 9532)          LP -12
  3  (750, 20, 9428)          (750, 20, 9426)          LP -2
  5  (806, 20, 9412)          (806, 20, 9408)          LP -4
  9  (968, 154, 9664)         (968, 154, 9662)         LP -2
 38  (2456, 34, 9334)         (2456, 34, 9330)         LP -4
 39  (2508, 150, 9664)        (2508, 150, 9662)        **length exact**, LP -2
 40  (2696, 140, 9562)        (2700, 40, 9456)         padX region too short
 41  (2854, 6, 9664)          (2772, 62, 9416)
 42  (2874, 26, 9356)         (2852, 6, 9662)
```

Rows 0-38 have **identical start/length** to the original; only the LP offsets
(cleanup trampolines at the end of the function) differ.  The function is 4 B
short (9696 vs 9700), so every trampoline sits 4 B lower.

### Why 4 B cannot be recovered from source

The first instruction-count divergence is at fn offset `0x64e` (instruction
index 615), in the `drawBox`/`DrawString` region, **not** in the ship branch:
ours has 4 extra instructions there because `this` is allocated to `sl` where
the original uses `r9`.  That is a global register-allocation difference caused
by the padX/padY change, and it is what makes the function 4 B short.  Adding
dead locals (`padZ`, `padY = &tab`, `if (padY)`) coalesces; making padY live
(`if (padY) { padY = &tab; }`) drops to 9680.  No source shape tested reaches
9700 while keeping row 39 at length 150.

**Conclusion.**  The row 39/40 boundary is fixed structurally by
`padX` inside the inner `if` (getShip unwinds to the outer cleanup), and
`padY` after the block makes row 39's length exactly 150.  The 4-byte size
deficit is a register-allocation side effect in the unrelated draw region, not
a ship-branch lifetime issue.  Since the gate forbids a section-size regression,
the base body is kept.

## 3. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10112
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10112/fdiff.py _ZN5Level10createShipEiiiP8Waypointbb
python3 /dev/shm/w10112/align_diff.py _ZN5Level10createShipEiiiP8Waypointbb
python3 /dev/shm/w10112/fdiff.py _ZN12HangarWindow6renderEv
python3 /dev/shm/w10112/align_diff.py _ZN12HangarWindow6renderEv
python3 /dev/shm/w10112/hw.py          # LSDA row table + extab byte diff
cmake --build cmake-build-match --target check -j6   # RATCHET PASS
```

Scratch `/dev/shm/w10112/`: `fdiff.py` / `align_diff.py` (worktree-pointed
copies), `sbs.py` (aligned side-by-side disasm), `run.py` (createShip body
splice + build + metrics), `runhw.py` (HangarWindow body splice), `hw.py`
(LSDA row parser + extab byte diff), `v_*.cpp` / `vA..vO.cpp` variant bodies.
