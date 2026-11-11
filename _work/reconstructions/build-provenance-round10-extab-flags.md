# Build provenance round 10 (w11520): LSDA row census, HangarWindow cleanup
# scope, and the surviving per-TU knobs on the wave-169 base

Status: **one landed `src/` win** (2026-11-11).
Worktree `wt/w11520`, branch `wave169-w11520`, base master `00c403d5a`
(wave 169).  Scratch `/dev/shm/w11520`.  Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).

Gate on entry: **RATCHET PASS**, `byte_exact 4507/4524`, `linked_exact
4507/4524`, size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0,
parity 5370/5370.  `.text` **81,642** differing bytes, `.ARM.extab`
**1,292**, `.rel.plt` 0.

Gate on exit: **RATCHET PASS** with the same counts; `.text` **81,637**
(-5), `.ARM.extab` **1,292** (unchanged), `.rel.plt` 0.

## 0. Bottom line

1. **Landed:** `HangarWindow::render` upgrade-guard operand order
   (`this->upgradeMode && upgTab == 1` instead of `upgTab == 1 &&
   this->upgradeMode`).  Raw 1,970 -> **1,965**, aligned 294 -> **291**,
   size 9,700 B, `.ARM.extab` record unchanged at 948 B / 156 rows /
   676 diff bytes.  Whole `.text` 81,642 -> **81,637**.
2. The `.ARM.extab` 1,292-byte residual is now fully enumerated below
   (11 records, 300 differing row tuples).  It decomposes into
   **body-layout shifts** (start/length moves and downstream LP shifts),
   **landing-pad identity swaps** (two adjacent identical thunks), and
   **one cleanup-scope difference** (`HangarWindow::render` rows 39/40).
3. The HangarWindow row-39 ULEB problem is a cleanup-scope difference:
   the original's `Status::getShip` call is in the name-only cleanup
   (LP 9664), ours in the four-String cleanup (LP 9512).  The wave-160
   shape (`dummy` + `__builtin_expect` + `volatile padX[1]`) fixes the
   record to 151 bytes but costs **+549 raw** (1,965 -> 2,514); the
   wave-169 block-order shape keeps the raw win and leaves the record at
   676.  No size-exact shape found keeps raw <= 1,965 and reduces the
   record.
4. The four surviving per-TU knobs (`LevelScript -disable-postra-machine-
   sink`, `Radar -disable-postra-machine-licm`, `Level
   -enable-earlycse-memssa=false`, `Layout +prof-unpr`) are **completely
   neutral on the wave-169 base**: removing each leaves the whole `.text`
   diff at exactly 81,637.  The task's `-mllvm -enable-arm-prof-unpr` and
   `-mllvm -enable-postra-machine-licm=false` spellings are rejected by
   clang 7.0.2; the real knobs are `-Xclang -target-feature -Xclang
   +prof-unpr` and `-mllvm -disable-postra-machine-licm`.

## 1. Landed change

`src/game/ui/HangarWindow.cpp`, `HangarWindow::render` ship branch:

```cpp
int upgTab = tab;
if (tab != 0) {
    if (!(tab == 3 && Globals::status->getShip() == this->selectedItem->ship)) {
        ... buttons[1] ...
        if (this->upgradeMode && upgTab == 1) {   // was: upgTab == 1 && this->upgradeMode
            ... buttons[2]/[10] ...
        }
    }
}
```

| metric | before | after |
|---|---:|---:|
| raw same-address diff | 1,970 | **1,965** |
| instruction-index alignment | 294 | **291** |
| function size | 9,700 | 9,700 |
| instructions | 3734/3734 | 3734/3734 |
| record diff bytes | 676 | 676 |
| whole `.text` diff | 81,642 | **81,637** |

The nested `if (tab != 0) { if (!(...)) }` form is codegen-neutral versus
the flat `&&` form (both 1,970 before the operand swap); only the operand
order moves bytes.  The `upgTab` snapshot is still required: dropping it
gives 6,731 raw / 730 record bytes.

## 2. `.ARM.extab` row census (11 records, 1,292 bytes)

Row tuples are `(start, length, landing-pad, action)` relative to the
function start.  "diff rows" counts tuples that differ; "start/len" is
the subset where start or length differ (body layout); "lp-only" is the
subset where only the landing-pad identity differs.

| record | owner | len | rows | diff rows | start/len | lp-only |
|---|---|---:|---:|---:|---:|---:|
| 0x137d18 | `LevelScript::process` | 348 | 46 | 46 | 46 | 0 |
| 0xaffd8 | `Level::createMission` | 956 | 148 | 97 | 42 | 55 |
| 0x1490d8 | `HangarWindow::render` | 948 | 156 | 43 | 41 | 2 |
| 0xc0214 | `Level::createShip` | 336 | 57 | 48 | 38 | 10 |
| 0x12e050 | `Radar::draw` | 132 | 19 | 15 | 13 | 2 |
| 0x17a144 | `MGame::OnTouchEnd` | 280 | 44 | 16 | 16 | 0 |
| 0x163b90 | `Hud::draw` | 476 | 77 | 11 | 6 | 5 |
| 0x170d38 | `SpaceLounge::drawLounge` | 168 | 25 | 10 | 4 | 6 |
| 0x971b8 | `Generator::createMission` | 172 | 26 | 6 | 0 | 6 |
| 0x12d484 | `Radar::Radar` | 256 | 39 | 7 | 0 | 7 |
| 0xd52d0 | `ModStation::ModStation` | 112 | 15 | 1 | 1 | 0 |
| **total** | | | | **313** | **207** | **93** |

The per-record row dumps are reproducible with
`/dev/shm/w11520/allrows.py` (scratch, not committed); the durable
classification is the table above.

### 2.1 Pure landing-pad identity swaps (no row geometry change)

* `Radar::Radar` (7 rows): rows 1/3/11/13/15 use the function-level
  cleanup (orig 2462, ours 2464); rows 19/21 use the `enemyGuns` loop
  cleanup (orig 2464, ours 2462).  The two pads are adjacent identical
  `b.n` trampolines; only the identity is swapped.  Wave-147 traced an
  exact 519/519 block-frequency tie; the original's PGO weights break it
  the other way.  No `-mllvm` branch-weight/BPI/MBP knob moved the tie.
* `Generator::createMission` (6 rows): every LP is ours = orig - 2
  (1912/1910, 1910/1908, ...).  This is a single 2-byte body shift
  upstream of the landing-pad thunk table, not a cleanup-scope change.
* `Level::createMission` (55 lp-only rows): the LPs are ours = orig - 4
  (17096 vs 17100) through the first half and -6/-8 in the tail.  These
  are downstream shifts of the body layout (see 2.2).
* `Hud::draw` (5 lp-only rows): rows 6/9/18/21/57 are ours = orig - 2;
  the `float filled` VarDecl split and LP address shifts documented in
  `hud-draw-extab-lsda-w4273.md` are still present.
* `SpaceLounge::drawLounge` (6 lp-only rows): the `{-10:1}` row is an
  identity swap; the `{+2:5}` rows are address shifts downstream of the
  176-byte body.

### 2.2 Body-layout records

* `LevelScript::process` (46/46 rows): every row start and length moves
  because the 57,232-byte body is laid out differently.  The LPs are
  close (57,048-57,172 vs 57,062-57,172) but the call-site boundaries
  are not.  This is the largest single record (185 diff bytes) and is a
  pure body-layout consequence.
* `Level::createMission` (42 start/len rows): the first 36 rows differ by
  +4 in the LP and 0-6 bytes in the start; rows 37-49 are the long
  zero-LP gaps whose lengths differ by 2-4 bytes; rows 119-147 are the
  tail where the body shifts by 4-16 bytes.  Body layout.
* `Level::createShip` (38 start/len rows): rows 0-30 shift by 2-10 bytes
  and the LPs move by +6; rows 31-56 are a clean +6 LP shift.  Body
  layout.
* `Radar::draw` (13 start/len rows): rows 1-14 shift by 2-30 bytes; rows
  3-12 are the `otherLock` cold-block region.  Body layout.
* `MGame::OnTouchEnd` (16/16 rows): rows 17-18 shift by 4 bytes; rows
  30-43 are the epilogue/chain placement.  Body layout.
* `ModStation::ModStation` (1 row): row 3 is `(390,30,922)` orig vs
  `(400,20,922)` ours; the LP is identical.  The 43-byte body diff is
  the `MatrixSetRotation` 4th-argument register allocation.  No
  regalloc/spill knob moved it (round 9, 44 knobs).

## 3. `HangarWindow::render`: the one cleanup-scope difference

Record 948 B / 156 rows.  The row-value diffs are rows 1, 39, 40, 48,
51, 60 and the second-half body shifts (rows 96-135).  Rows 39/40 are
the only cleanup-scope difference:

```
row  orig (start,len,lp)      ours (start,len,lp)
 39  (2508, 150, 9664)        (2508, 118, 9664)
 40  (2696, 140, 9562)        (2654, 182, 9512)
 48  (3406,   4, 9512)        (3406,   4, 9514)
 51  (3644,   4, 9512)        (3644,   4, 9514)
 60  (3806,  20, 9514)        (3806,  20, 9516)
```

LP 9664 = `0x14b698` destroys `sp+312` only (`name`).  LP 9512 =
`0x14b5f0` destroys `sp+276`, `sp+288`, `sp+300`, `sp+312`.  LP 9562 =
`0x14b61a` destroys `sp+288`, `sp+300`, `sp+312`.  LP 9514 = `0x14b5f2`
destroys `sp+276` then chains to the same three.

The original's `Status::getShip` call (fn+2654) is in row 39, i.e. the
name-only cleanup; ours is the first call of row 40, i.e. the four-String
cleanup.  Row 39's length is therefore 150 (2-byte ULEB) in the original
and 118 (1-byte ULEB) in ours; that one encoding byte shifts every
subsequent row, which is the bulk of the 676-byte record diff.  The
`sp+276/288/300` slots are the `formatCredits`/`String` temporaries of
the `item->isShip()` name/price block; the original's cleanup region
excludes `sp+276` for the getShip call and for the button-1 calls.

### 3.1 Shapes measured on the wave-169 base

All are size-exact at 9,700 B unless noted.

| shape | raw | aligned | record | rows |
|---|---:|---:|---:|---:|
| wave-169 base (flat `&&`, `upgTab==1 && upgrade`) | 1,970 | 294 | 676 | 156 |
| **landed: nested ifs, `upgrade && upgTab==1`** | **1,965** | **291** | 676 | 156 |
| nested ifs, `upgTab==1 && upgrade` | 1,970 | 294 | 676 | 156 |
| + `__builtin_expect(tab != 0, 1)` | 1,965 | 291 | 676 | 156 |
| + `volatile int *dummy` | 1,965 | 291 | 676 | 156 |
| + dummy + expect | 1,965 | 291 | 676 | 156 |
| + `volatile int padX[1]` inside the inner if | 2,276 | 410 | 697 | 157 |
| wave-160 shape: dummy + expect + padX + upgrade inside | 2,514 | 482 | **151** | 156 |
| same, `upgradeMode && tab==1` order | 2,514 | 485 | **151** | 156 |
| wave-160 shape without padX | 2,249 | 405 | 464 | 155 |
| wave-160 shape without expect | 2,282 | 413 | 697 | 157 |
| wave-169 base + `getCredits()` after the guard | 8,552 | 739 | 762 | 156 (size 9,680) |
| wave-169 base + `SetColor(0xffffffff)` after the guard | 8,559 | 764 | 760 | 156 (size 9,688) |

The `padX` sink is the only thing that moves the getShip call into the
name-only cleanup (record 151), but it also perturbs the body layout
(+311 raw) and changes the record to 956 B / 157 rows on the wave-169
base.  The wave-160 shape's 151-byte record is the best extab result
found; it costs +549 raw.  Since raw same-address bytes are the
byte-identicality ground truth, the landed shape keeps the raw win.

Adding a 4-byte call after the guard (the wave-160 `getCredits()`
near-miss) now *shrinks* the function to 9,680 B on the wave-169 base
(the call lets the `Globals::status` load be shared), but the record
worsens to 762 and the raw to 8,552; it is not size-exact.

## 4. Surviving per-TU knobs on the wave-169 base

Each knob was removed in turn (CMakeLists property commented out,
reconfigure, rebuild, whole-`.text` same-address diff):

| TU | knob | whole `.text` with | without |
|---|---|---:|---:|
| LevelScript | `-mllvm -disable-postra-machine-sink` | 81,637 | 81,637 |
| Radar | `-mllvm -disable-postra-machine-licm` | 81,637 | 81,637 |
| Level | `-mllvm -enable-earlycse-memssa=false` | 81,637 | 81,637 |
| Layout | `-Xclang -target-feature -Xclang +prof-unpr` | 81,637 | 81,637 |

The owned residual functions are also byte-identical with and without
each knob (`LevelScript::process` 53,970; `Radar::draw` 7,127;
`Level::createShip` 1,560 / `Level::createMission` 5,435; `Layout`
C1/C2 5,650).  The 1-2 byte wins recorded in the round-5/6/7/8 notes
were absorbed by the wave-169 source changes; the knobs are now dead
weight but harmless.  They are left in place (removing them is not a
measured byte win).

Spelling check on clang 7.0.2:

* `-mllvm -enable-arm-prof-unpr` -> rejected (unknown option).
* `-mllvm -arm-prof-unpr` / `-mllvm -enable-prof-unpr` / `-mllvm
  -prof-unpr` -> rejected.
* `-mllvm -enable-postra-machine-licm=false` -> rejected; the knob is
  `-mllvm -disable-postra-machine-licm`.
* `-mllvm -enable-earlycse-memssa=true` -> accepted (the default; it
  cancels the Level override and is neutral).

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w11520
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN12HangarWindow6renderEv
python3 _work/reconstructions/extab_record_map.py
cmake --build cmake-build-match --target check -j2
```

Scratch artifacts: `/dev/shm/w11520/{variants.py,runvar.py,ins.py,
oldshape.py,comb.py,more.py,more2.py,more3.py,allrows.py,classify.py,
toggle.py,toggle2.py,secdiff.py}`.

## 6. Files changed

* `src/game/ui/HangarWindow.cpp` — upgrade-guard operand order.
* `_work/reconstructions/build-provenance-round10-extab-flags.md` — this note.
