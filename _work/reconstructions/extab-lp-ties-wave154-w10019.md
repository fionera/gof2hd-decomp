# Extab LP-identity ties (wave 154, w10019): HangarWindow row 39/40 is
# getShip's landing pad; every lifetime-start move costs size or extab

Status: **analysis only, no landed `src/` change** (base is already best).
Worktree `wt/w10019`, base `4417db0b4` (wave 153). Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370.

| metric | base |
|---|---:|
| `.text` differing bytes | 86,623 |
| `.ARM.extab` differing bytes | 1,265 |
| `HangarWindow::render` raw / aligned / extab | 1965 / 291 / 676 |
| fn size | 9700 |

## 0. Re-derived mechanism (the row split is one landing-pad assignment)

The LSDA call-site table is built by `EHStreamer::computeCallSiteTable`
(LLVM 7 `lib/CodeGen/AsmPrinter/EHStreamer.cpp`): adjacent invoke ranges are
merged when `Site.LPad == Prev.LPad`.  The landing pad of an invoke is
`FuncInfo.MBBMap[EHPadBB]` (`SelectionDAGBuilder.cpp:6409`, `MF.addInvoke`),
i.e. the IR `unwind label`.

`HangarWindow::render` rows 39/40:

```
orig row 39 (2508,150,9664)  = getIndex..getShip, LP = outer cleanup
orig row 40 (2696,140,9562)  = setPosition..upgradeMode draw, LP = padX cleanup
ours row 39 (2508,118,9664)  = getIndex..DrawString/isShip
ours row 40 (2654,182,9514)  = getShip..upgradeMode draw, LP = padX cleanup
```

So the *only* structural difference is that the original's `getShip` invoke
(`0x149b36`, fn offset 2654) unwinds to the **outer** cleanup, while ours
unwinds to the **padX** cleanup.  The code layout is otherwise already right
(getShip at 2654, setPosition at 2698 in both).  If getShip's LP were the
outer cleanup, row 39 would automatically become `(2508,150,9664)` and row 40
`(2696,140,9562)` — the exact original rows.

The original's padX cleanup LP 9562 is `b.n 0x14b698` (a lifetime.end-only
trampoline to the outer cleanup); ours LP 9514 is the same kind of trampoline.
So the original's local is lifetime-tracked from *after* getShip to the end of
the ship branch (setPosition through the upgradeMode draw), i.e. its
`llvm.lifetime.start` is sunk to the first use after the `getShip` call.

## 1. Why moving `padX` inside the inner `if` does not fix it

`padX` is dead (`volatile int *padX = 0;` never read), so SROA promotes the
alloca and deletes its lifetime markers.  The padX landing-pad block then
becomes `landingpad; br outer`, identical to the outer cleanup, and
`SimplifyCFG::TryToMergeLandingPad` is free to redirect the getShip unwind
edge.  Declaring `padX` inside the inner `if` body does move getShip's raw-IR
unwind label to the outer cleanup (`%620`), but the padX cleanup block ends up
covering only the setPosition argument setup (40 B), and the function shrinks
to 9676 B (register allocation keeps `&this->buttons` in `r6` instead of
reloading `[sp,#88]`, the original's 8 B/access form).

## 2. New sweep (all measured; raw / aligned / extab / size / row39 / row40)

| variant | raw | align | extab | size | row39 ours | row40 ours |
|---|---:|---:|---:|---:|---|---|
| **base (`padX` before `if (tab != 0)`)** | **1965** | **291** | **676** | **9700** | (2508,118,9664) | (2654,182,9514) |
| `padX` inside inner if | 8610 | 799 | 760 | 9676 | (2488,146,9640) | (2678,40,9434) |
| `padX` before + `padY` inside | 6676 | 594 | 715 | 9692 | (2508,118,9658) | (2654,4,9442) |
| `padX` before + `padY` after block | 6693 | 601 | 715 | 9692 | (2508,118,9658) | (2654,82,9488) |
| `padX` inside + `padY` after block | 6204 | 582 | 695 | 9696 | **(2508,150,9662)** | (2700,40,9456) |
| `int *volatile padX` before | 7317 | 728 | 733 | 9708 | (2508,118,9672) | (2658,186,9522) |
| `int *volatile padX` inside | 8504 | 750 | 758 | 9680 | (2488,146,9644) | (2682,40,9438) |
| `volatile int padX` before | 7317 | 728 | 733 | 9708 | (2508,118,9672) | (2658,186,9522) |
| `volatile int padX` inside | 8504 | 750 | 758 | 9680 | (2488,146,9644) | (2682,40,9438) |
| `{ padX }` own scope before + `padY` inside | 8610 | 799 | 760 | 9676 | (2488,146,9640) | (2678,40,9434) |
| `{ padX }` own scope before + `padY` after | 6606 | 838 | 706 | 9708 | (2508,236,9672) | (2782,62,9428) |
| `TouchButton *button` local inside | 8627 | 761 | 762 | 9664 | (2488,146,9628) | (2678,28,9422) |
| `Array<TouchButton*> *buttons` local inside | 6989 | 752 | 727 | 9696 | (2508,146,9660) | (2702,32,9454) |
| hoist `getShip` into `bool sameShip` | 2559 | 659 | 686 | 9700 | (2508,118,9666) | (2646,4,9380) |
| dead uses (`padX = &tab`, `if (padX)`) | 1965 | 291 | 676 | 9700 | unchanged | unchanged |

Only `padX` inside + `padY` after the block reproduces row 39's **length 150**
(and the getShip→outer-cleanup split), but it is 4 B short (9696) and its
extab diff is 695, worse than the base 676.  Every variant that moves the
lifetime start after getShip loses 4-24 B; every variant that keeps 9700 B
leaves getShip in the padX cleanup.  No variant beats extab 676.

### Row table (base, 43 differing rows)

```
 1  orig (684,20,9544)      ours (684,20,9534)     LP offset only
39  orig (2508,150,9664)    ours (2508,118,9664)   getShip LP (outer vs padX)
40  orig (2696,140,9562)    ours (2654,182,9514)   getShip..upgradeMode
60  orig (3806,20,9514)     ours (3806,20,9516)    LP offset only
96  orig (5918,198,0)       ours (5918,196,0)      credit-loop start
97  orig (6116,8,9216)      ours (6114,8,9216)
98  orig (6124,128,0)       ours (6122,132,0)
99  orig (6252,10,9652)     ours (6254,10,9652)
101-125 shifted 2-6 B (credit-loop RA)
126 orig (6824,210,9654)    ours (6826,202,9654)   merged, 8 B shorter
127-135 shifted 2-6 B
```

### Row table with `padX` inside + `padY` after the block (row 39 exact)

```
36  orig (2336,52,9664)     ours (2336,52,9662)
37  orig (2412,30,9454)     ours (2412,30,9452)
38  orig (2456,34,9334)     ours (2456,34,9330)
39  orig (2508,150,9664)    ours (2508,150,9662)   length exact, LP -2
40  orig (2696,140,9562)    ours (2700,40,9456)    padX region too short
41  orig (2854,6,9664)      ours (2772,62,9416)
42  orig (2874,26,9356)     ours (2852,6,9662)
```

## 3. Conclusion / open question

The row 39/40 boundary is a single landing-pad identity: the original's
`getShip` unwinds to the outer cleanup.  Reproducing it needs a
lifetime-tracked local whose `lifetime.start` is sunk to *after* getShip while
the register allocator still spills `&this->buttons` (the original's
`ldr r0,[sp,#88]; ldr r0,[r0]; ldr r0,[r0,#4]` 8 B/access form).  A dead
`volatile int *` sink cannot do both: SROA deletes its lifetime markers, so
the padX cleanup merges with the outer cleanup and the shape collapses.
The remaining lever is a non-promotable local (surviving `lifetime.end`) whose
scope is the whole ship branch and whose first use is after getShip — e.g. a
real `String`/aggregate temporary in the inner `if` body.  That is a body
change, not a sink tweak.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10019
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6          # RATCHET PASS
python3 /dev/shm/w10019/fdiff.py _ZN12HangarWindow6renderEv  # 1965, 9700
python3 /dev/shm/w10019/align_diff.py _ZN12HangarWindow6renderEv  # 291
python3 /dev/shm/w10019/rows.py 0x1490d8                    # 43 rows
python3 /dev/shm/w10019/sweep.py base inside two_sink pad_after_block_inside
```

Scratch (not committed), `/dev/shm/w10019/`: `fdiff.py`, `align_diff.py`,
`rows.py`, `sweep.py`, `HangarWindow.base.cpp`, `HangarWindow.inside.cpp`,
`base.asm`, `inside.asm`, `cgp_before.txt` (print-before-all IR dump),
`llvm-7.0.0.src/` (EHStreamer/SimplifyCFG source).
