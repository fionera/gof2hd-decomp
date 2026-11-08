# `HangarWindow::render` — wave 148 (w9514): faithful `setSplitText`/`draw`
# order; padX lifetime explains the row-39/40 split

Target: `_ZN12HangarWindow6renderEv` (`src/game/ui/HangarWindow.cpp`), original
`0x1490d8`, size `0x25e4` = 9700 B. Worktree `/home/debian/GalaxyOnFire2/wt/w9514`,
base `fece77a64` (wave 148). Metrics: raw same-address diff
(`/dev/shm/w9514/fdiff.py`), instruction-index alignment
(`/dev/shm/w9514/align_diff.py`), `.ARM.extab` LSDA row diff
(`_work/reconstructions/extab_lsda_decode.py`).

## Result (landed)

**Yes — the credit-loop call order is restored to the original's asm order at
9700 B.**  `.ARM.extab` stays 948 B / 156 rows; `.text` size exact.

| metric | before | after |
|---|---:|---:|
| raw same-address diff | 1967 | **1965** |
| align_diff non-matching insns | 301 | **291** |
| `.ARM.extab` record diff (fn 0x1490d8) | 676 | 676 |
| rows | 156 | 156 |
| differing rows | 43 | 43 |
| fn size | 9700 | 9700 |

Gate: byte_exact **4507 -> 4507**, linked_exact **4507 -> 4507**, size 4524/4524,
data 843/843, imports 207, lint CLEAN, RATCHET PASS.

The change is one line: `setSplitText(price)` before `draw()` instead of after.
w9016 had swapped them (raw 1731 -> 1719) purely as a metric hack; the original's
asm order is `replaceTextKeepSize`, `setSplitText`, `draw` (0x14ab9a / 0x14abac /
0x14abbc), so this restores the faithful order and improves the aligned metric.

## Row table (fn 0x1490d8), before -> after

The row table is **unchanged** by this pass (43 differing rows before and after).
The rows the mission targets:

```
 1  orig (684,20,9544)    ours (684,20,9534)      landing-pad offset only
39  orig (2508,150,9664)  ours (2508,118,9664)    list-render row boundary
40  orig (2696,140,9562)  ours (2654,182,9514)    getShip call-site row
60  orig (3806,20,9514)   ours (3806,20,9516)     landing-pad offset only
96  orig (5918,198,0)     ours (5918,196,0)       credit-loop start
97  orig (6116,8,9216)    ours (6114,8,9216)
98  orig (6124,128,0)     ours (6122,132,0)
99  orig (6252,10,9652)   ours (6254,10,9652)
101-135 shifted 2-6 B (credit-loop RA)
126 orig (6824,210,9654)  ours (6826,202,9654)    merged, 8 B shorter
127 orig (7034,390,0)     ours (7028,390,0)
```

## New findings

### 1. Row 39/40 is caused by `padX`'s `llvm.lifetime.start` (root cause found)

The `volatile int *padX = 0;` sink is a lifetime-tracked local.  In the raw
clang IR (`-Xclang -disable-llvm-passes`) the ship branch is:

```
; <label>:2339:                    ; ship-branch body
  call void @llvm.lifetime.start.p0i8(i64 4, i8* %2340)   ; padX
  store i32* null, i32** %82
  ...
  %2348 = invoke getShip ... unwind label %2397          ; padX cleanup
; <label>:2397:
  %2398 = landingpad { i8*, i32 } cleanup
  call void @llvm.lifetime.end.p0i8(i64 4, i8* %2401)     ; padX
  br label %2722
```

`isShip` unwinds to `%620` (the outer cleanup), `getShip` unwinds to `%2397`
(the padX cleanup).  That is exactly the row-39/40 split: the original's
`getShip` unwinds to the outer cleanup, so its lifetime-tracked local must start
**after** `getShip`.  Moving `padX` into the inner `if` body (after the
condition) does move the row boundary the right way (row 39 length 118 -> 146
vs the original 150) but shrinks the function to **9676 B**; adding a second
local (`padY`) gives **9692 B**.  The 9700-byte size and the row boundary are
coupled through padX's lifetime: no source shape tried keeps both.  This is a
sharper diagnosis than w9403's "PGO-layout consequence" — the split is created
by clang CodeGen's lifetime marker, not by an -Oz pass.

### 2. Rows 96-135: `sl` holds the layout *value*, not `&Globals::layout`

Original credit loop (0x14abc0):
```
ldr.w r2,[sl]        ; r2 = Globals::layout
ldr   r2,[r2,#44]    ; r2 = layout->field_0x2c_rowHeight
```
Ours:
```
ldr.w r2,[sl,#44]    ; folded: sl already holds the layout value
```
The original keeps `&Globals::layout` in `sl` (so every use is a double load);
ours keeps the value (one load).  That is 2 of the 8-byte row-126 deficit.  The
other 6 B: the original reloads `[sp,#132]` (= `&this->buttons`) for each of the
three button accesses and does `ldr r0,[r0]; ldr r0,[r0,#4]` (6 B/access); ours
keeps `&this->buttons` in `r5` and does `ldr r0,[r5]; ldr r0,[r0,#4]`
(4 B/access).  The original's `r5` is `&price`; ours is `&buttons`.

The `sl` role is the w8914 lever: `Layout *lay = Globals::layout;` makes
`sl` = the value.  Removing `lay` (direct `Globals::layout`) makes `sl` =
description and spills `&Globals::layout` to `[sp,#128]` (9704 B, 145 rows).
`Layout **`, `Layout *&`, `Layout *const &`, `Layout *volatile *`, and a
volatile deref all give the same 9704 B / 145 rows.  No source shape tried
produced `sl = &Globals::layout` together with `description = r6`.

### 3. Variants measured this wave

| variant | raw | align | extab | rows | size |
|---|---:|---:|---:|---:|---:|
| base (w9403) | 1967 | 301 | 676 | 43 | 9700 |
| **A: setSplitText before draw (landed)** | **1965** | **291** | **676** | **43** | **9700** |
| M1: y uses `Globals::layout`, draw uses `lay` | 1939 | 418 | 676 | 42 | 9700 |
| P1/P2: `lay` at loop top | 1980 | 291 | 676 | 44 | 9700 |
| price declared first | 1966 | 565 | 676 | 43 | 9700 |
| direct `Globals::layout` (no `lay`) | 3308 | 735 | 710 | 145 | 9704 |
| `Layout **layp = &Globals::layout` | 3308 | 735 | 710 | 145 | 9704 |
| `Layout *&lay` / `const&` / `volatile *` | 3304-3308 | 735-758 | 710 | 145 | 9704 |
| padX inside inner if body | 8610 | 799 | 760 | 156 | 9676 |
| padX + padY inside body | 6676 | 594 | 715 | 152 | 9692 |
| footer `TouchButton *b` local | 3524 | 520 | 725 | 146 | 9684 |
| footer `Array* buttons` local | 9108 | 1318 | 783 | 156 | 9620 |
| ship branch `bool sameShip` | 6545-6693 | 601-927 | 703-715 | 151 | 9692-9704 |
| S17 upgradeMode inside `if (tab != 0)` | 1965 | 291 | 676 | 43 | 9700 (byte-identical to A) |

## Reproduction

```sh
python3 /dev/shm/w9514/fdiff.py _ZN12HangarWindow6renderEv        # 1965, 9700
python3 /dev/shm/w9514/align_diff.py _ZN12HangarWindow6renderEv  # 291
python3 _work/reconstructions/extab_lsda_decode.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so                                 # 676, 43 rows
cmake --build cmake-build-match --target check -j6                 # RATCHET PASS
```

Scratch (not committed), `/dev/shm/w9514/`: `fdiff.py`, `align_diff.py`,
`rowdiff.py`, `meas.sh`, `batch.py`..`batch11.py`, `HangarWindow.A.cpp`,
`HangarWindow.base.cpp`, `HangarWindow.raw.ll`, `extab_*.txt`, `rows_*.txt`.

## Open leads

* Row 39/40: a lifetime-tracked local declared **after** `getShip` but whose
  presence does not shrink the function.  The padX sink must stay before the
  `if` for the 9700-byte register allocation; the missing piece is a second
  lifetime marker that starts at the inner if body without changing the global
  allocation (every local added there costs 8-24 B).
* Rows 96-135: force `sl = &Globals::layout` in the credit loop while keeping
  `description` in `r6`.  `lay` frees `sl` but stores the value; direct
  `Globals::layout` stores the address but gives `sl` to `description`.
