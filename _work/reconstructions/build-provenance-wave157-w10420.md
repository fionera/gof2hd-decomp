# Build provenance at wave 157 (w10420): per-TU opt levels, `-mllvm`, unwind/stack-protector
# cells, the w10313 Status profile, and instrumented profiles on all residuals

Status: **research note only**, 2026-11-08. Worktree `wt/w10420`, base master `9e4795206`
(wave 157). Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000). Scratch `/dev/shm/w10420`. No `src/`,
`tools/`, `CMakeLists.txt`, baseline, allowlist or doc change is landed; this note is the only
commit.

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0,
parity 5370/5370. `.text` **85,529** differing bytes (section size 431,168 B),
`.ARM.extab` **1,292**, `.rel.plt` 0, `.note.gnu.build-id` 20. All 27 section sizes exact;
file size 2,187,328 B.

## 0. Bottom line

1. **No per-TU optimization level beats `-Oz` at the wave-157 tree.** The full 13-TU x 5-level
   matrix reproduces the wave-154/155/156 conclusion. `-Oz` is the minimum of both raw and
   aligned diff for every one of the 17 residual names.
2. **No `-mllvm` knob improves the gate.** 17 re-measured cells plus 7 new knobs. Six are
   exactly neutral (`byte 4507 / linked 4507 / size 4524`, raw sum 91,325, aligned sum 25,669):
   `-slp-threshold=0`, `-inline-threshold=1`, `-unroll-threshold=0`, `-enable-gvn-hoist=false`,
   `-profile-guided-section-prefix=false`, `-max-uses-for-sinking=0` (with or without the
   section-prefix pass), `-guards-predicate-pass-branch-weight=1`, `-old-thumb2-ifcvt`. Every
   other variant is strictly worse on both metrics.
3. **`-fno-unwind-tables` is a regression** (launcher cell, applied after the target's
   `-funwind-tables`): `byte 2878`, raw 91,871, aligned 26,788 (base 4507 / 91,325 / 25,669).
   The canonical `-funwind-tables` is confirmed.
4. **`-fstack-protector-strong` is the only viable stack-protector spelling.**
   `-fstack-protector` (basic) gives `byte 1369 / linked 3787 / size 3794`; `-fno-stack-protector`
   `1369 / 3783 / 3790`; `-fstack-protector-all` collapses to `91 / 969 / 981`.
   `-fstack-protector-explicit` is not accepted by clang 7.0.2 (unknown argument).
5. **The w10313 Status profile replays exactly at the wave-157 tree**: `Status::missionCompleted`
   raw 722 -> **1,299**, aligned 320 -> **583**, size 1,432 (exact), `.rel.plt` 0, gate unchanged
   at 4507/4507/4524. It is still not byte-exact, and it is the only residual it touches.
6. **A uniform instrumented profile (`-fprofile-instr-use`, all counters 1, sink0, prefix off)
   on each residual regresses every one of them** (raw 29 -> 1,808 for `OnTouchBegin`,
   37 -> 11,018 for `OnUpdate`, 722 -> 1,333 for `Status`, ...). No residual becomes byte-exact.
7. **No flag or profile makes any of the 17 residuals byte-exact.** The residual set remains
   source-shape / TU-shape work. Nothing is landable: the canonical `-Oz` build is best on every
   measured axis.

## 1. Baseline

```text
verify: byte 4507  linked 4507  size 4524/4524  avg 99.93
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0  imports 207
lint CLEAN   drift 0   symbol parity 5370/5370
RATCHET PASS
```

Residual raw / aligned / size at the wave-157 tree (17 names, 15 unique bodies;
`LayoutC1/C2` and `ModStationC1/C2` are aliases):

| function | raw | aligned | size |
|---|---:|---:|---:|
| `Layout::Layout` (C1/C2) | 5,753 | 1,984 | 7,072 |
| `LevelScript::process` | 55,094 | 17,129 | 57,232 |
| `Level::createShip` | 1,564 | 452 | 2,764 |
| `Level::createMission` | 5,487 | 774 | 17,172 |
| `Status::missionCompleted` | 722 | 320 | 1,432 |
| `Radar::draw` | 8,181 | 1,278 | 9,908 |
| `MGame::OnTouchEnd` | 4,391 | 903 | 8,080 |
| `MGame::OnUpdate` | 37 | 109 | 11,440 |
| `Gun::calcCharacterCollision` | 463 | 57 | 1,984 |
| `HangarWindow::render` | 1,965 | 291 | 9,700 |
| `SpaceLounge::drawLounge` | 175 | 41 | 1,948 |
| `Hud::draw` | 1,325 | 249 | 9,332 |
| `Generator::createMission` | 300 | 59 | 1,996 |
| `MenuTouchWindow::OnTouchBegin` | 29 | 15 | 1,896 |
| `ModStation::ModStation` (C1/C2) | 43 | 12 | 1,008 |

Raw sum over the 17 names **91,325**; aligned sum **25,669**. Over the 15 unique bodies
(excluding the two C2 aliases) the sums are **85,529** and **23,673** — the unique raw sum is
exactly the `.text` differing-byte count.

## 2. Per-TU `-Oz/-O1/-O2/-Os/-O3` sweep

Harness: `tools/per_tu_opt_sweep.py` with `tools/per_tu_flags.cmake` (each cell is a full
independent scratch build; `-Oz` reproduces the canonical gate exactly). 13 TUs x 5 levels.

### 2.1 Residual raw/aligned/size (lower is better)

| TU / function | -Oz | -O1 | -O2 | -Os | -O3 |
|---|---|---|---|---|---|
| `Layout.cpp` `LayoutC1` | **5753/1984/7072** | 7823/4897/8004 | 5998/2194/7120 | 5772/2137/7072 | 6354/2383/7276 |
| `LevelScript.cpp` `process` | **55094/17129/57232** | 59901/36997/61628 | 61605/33985/63236 | 55775/24322/57616 | 64600/34955/66232 |
| `Level.cpp` `createShip` | **1564/452/2764** | 2979/1613/3060 | 2951/1095/3064 | 2876/1066/2988 | 2968/1097/3080 |
| `Level.cpp` `createMission` | **5487/774/17172** | 18220/8827/18784 | 18778/10645/19312 | 17565/6032/18176 | 19763/7838/20288 |
| `Status.cpp` `missionCompleted` | **722/320/1432** | 1877/855/1940 | 1692/778/1760 | 1328/628/1456 | 1764/979/1840 |
| `Radar.cpp` `draw` | **8181/1278/9908** | 10199/4807/10512 | 10058/4438/10388 | 9967/4268/10276 | 10505/4207/10820 |
| `MGame.cpp` `OnTouchEnd` | **4391/903/8080** | 8081/3103/8448 | 9402/3766/9956 | 7828/2521/8288 | 9768/3865/10152 |
| `MGame.cpp` `OnUpdate` | **37/109/11440** | 11493/4707/12020 | 14025/5638/14324 | 10987/1884/11504 | 14051/5680/14420 |
| `Gun.cpp` `calcCharacterCollision` | **463/57/1984** | 1911/549/2000 | 1927/346/2032 | 1879/322/1984 | 1932/349/2040 |
| `HangarWindow.cpp` `render` | **1965/291/9700** | 9608/4925/9940 | 9569/2214/9904 | 9439/2124/9816 | 9871/5316/10180 |
| `SpaceLounge.cpp` `drawLounge` | **175/41/1948** | 1965/453/2056 | 1861/224/1976 | 1858/236/1972 | 1899/230/2004 |
| `Hud.cpp` `draw` | **1325/249/9332** | 8874/1943/9452 | 9475/1998/9740 | 9123/1666/9528 | 9659/2321/9940 |
| `Generator.cpp` `createMission` | **300/59/1996** | 2133/856/2228 | 2082/835/2168 | 1976/566/2076 | 2410/934/2468 |
| `MenuTouchWindow.cpp` `OnTouchBegin` | **29/15/1896** | 2084/689/2184 | 1876/737/2000 | 1873/736/2000 | 1939/743/2016 |
| `ModStation.cpp` `ModStationC1` | **43/12/1008** | 957/290/908 | 946/44/1016 | 919/42/1008 | 1048/98/1088 |

(`LayoutC2` and `ModStationC2` are identical to their C1 rows.)

### 2.2 Full gate / `.text` size per cell

| TU | level | byte | linked | size | `.text` |
|---|---|---:|---:|---:|---:|
| any | `-Oz` | 4507 | 4507 | 4524 | 431168 |
| `MGame.cpp` | `-O1` | 1355 | 4475 | 4494 | 432352 |
| `MGame.cpp` | `-O2` | 1355 | 4477 | 4495 | 431040 |
| `MGame.cpp` | `-Os` | 2201 | 4485 | 4507 | 431136 |
| `MGame.cpp` | `-O3` | 1355 | 4476 | 4494 | 431040 |
| `ModStation.cpp` | `-O1` | 1356 | 4478 | 4499 | 432992 |
| `ModStation.cpp` | `-O2` | 1356 | 4484 | 4501 | 431008 |
| `ModStation.cpp` | `-Os` | 1369 | 4490 | 4509 | 431104 |
| `ModStation.cpp` | `-O3` | 1439 | 4484 | 4500 | 431008 |
| `SpaceLounge.cpp` | `-O1` | 1412 | 4486 | 4504 | 432672 |
| `SpaceLounge.cpp` | `-O2` | 1369 | 4488 | 4504 | 430880 |
| `SpaceLounge.cpp` | `-Os` | 1369 | 4489 | 4506 | 431040 |
| `SpaceLounge.cpp` | `-O3` | 1369 | 4488 | 4504 | 430880 |
| `Generator.cpp` | `-O1` | 1369 | 4490 | 4516 | 432512 |
| `Generator.cpp` | `-O2` | 1668 | 4499 | 4516 | 431168 |
| `Generator.cpp` | `-Os` | 1668 | 4500 | 4516 | 431168 |
| `Generator.cpp` | `-O3` | 1659 | 4500 | 4516 | 431168 |
| `Status.cpp` | `-O1` | 1346 | 4438 | 4472 | 432704 |
| `Status.cpp` | `-O2` | 1351 | 4459 | 4478 | 430464 |
| `Status.cpp` | `-Os` | 1365 | 4461 | 4480 | 430880 |
| `Status.cpp` | `-O3` | 1351 | 4459 | 4477 | 430464 |
| `HangarWindow.cpp` | `-O1` | 1365 | 4483 | 4505 | 432224 |
| `HangarWindow.cpp` | `-O2` | 1352 | 4487 | 4505 | 431072 |
| `HangarWindow.cpp` | `-Os` | 1369 | 4491 | 4507 | 431104 |
| `HangarWindow.cpp` | `-O3` | 1352 | 4487 | 4503 | 431040 |
| `Hud.cpp` | `-O1` | 1375 | 4477 | 4498 | 431744 |
| `Hud.cpp` | `-O2` | 1353 | 4477 | 4497 | 430944 |
| `Hud.cpp` | `-Os` | 1354 | 4479 | 4498 | 431104 |
| `Hud.cpp` | `-O3` | 1353 | 4477 | 4494 | 430944 |
| `Layout.cpp` | `-O1` | 1369 | 4470 | 4500 | 431328 |
| `Layout.cpp` | `-O2` | 1369 | 4479 | 4498 | 431104 |
| `Layout.cpp` | `-Os` | 1506 | 4486 | 4507 | 431136 |
| `Layout.cpp` | `-O3` | 1369 | 4479 | 4497 | 431104 |
| `MenuTouchWindow.cpp` | `-O1` | 1377 | 4472 | 4495 | 432160 |
| `MenuTouchWindow.cpp` | `-O2` | 1367 | 4481 | 4497 | 430688 |
| `MenuTouchWindow.cpp` | `-Os` | 1369 | 4482 | 4499 | 430752 |
| `MenuTouchWindow.cpp` | `-O3` | 1367 | 4480 | 4496 | 430688 |
| `Gun.cpp` | `-O1` | 1369 | 4493 | 4511 | 431680 |
| `Gun.cpp` | `-O2` | 1377 | 4494 | 4511 | 431168 |
| `Gun.cpp` | `-Os` | 1365 | 4494 | 4512 | 431168 |
| `Gun.cpp` | `-O3` | 1356 | 4494 | 4510 | 431168 |
| `Radar.cpp` | `-O1` | 1368 | 4498 | 4516 | 432256 |
| `Radar.cpp` | `-O2` | 1377 | 4498 | 4514 | 431136 |
| `Radar.cpp` | `-Os` | 1378 | 4500 | 4516 | 431168 |
| `Radar.cpp` | `-O3` | 1377 | 4498 | 4514 | 431136 |
| `Level.cpp` | `-O1` | 1363 | 4433 | 4466 | 433600 |
| `Level.cpp` | `-O2` | 1351 | 4448 | 4465 | 430400 |
| `Level.cpp` | `-Os` | 1353 | 4455 | 4474 | 430720 |
| `Level.cpp` | `-O3` | 1351 | 4448 | 4465 | 430400 |
| `LevelScript.cpp` | `-O1` | 1369 | 4499 | 4515 | 432032 |
| `LevelScript.cpp` | `-O2` | 1369 | 4499 | 4516 | 431104 |
| `LevelScript.cpp` | `-Os` | 1471 | 4501 | 4517 | 431104 |
| `LevelScript.cpp` | `-O3` | 1443 | 4499 | 4515 | 431104 |

**Conclusion:** `-Oz` is optimal for every residual on both metrics. No TU was compiled at
another level.

## 3. Targeted `-mllvm` / unwind / stack-protector matrix

Harness: `/dev/shm/w10420/run_variant.py` (one build directory per variant, compiler-launcher
cell so the extra flags are appended *after* the canonical flags and after the target's
`-ffunction-sections -funwind-tables`). `rawsum` / `alignsum` are the sums over the 17 residual
names; `base` is the canonical `-Oz` build.

| variant | flags | byte | linked | size | `.text` | rawsum | alignsum |
|---|---|---:|---:|---:|---:|---:|---:|
| base | — | **4507** | **4507** | **4524** | **431168** | **91325** | **25669** |
| `slp0` | `-mllvm -slp-threshold=0` | 4507 | 4507 | 4524 | 431168 | 91325 | 25669 |
| `inline1` | `-mllvm -inline-threshold=1` | 4507 | 4507 | 4524 | 431168 | 91325 | 25669 |
| `unroll0` | `-mllvm -unroll-threshold=0` | 4507 | 4507 | 4524 | 431168 | 91325 | 25669 |
| `gvnhoist` | `-mllvm -enable-gvn-hoist=false` | 4507 | 4507 | 4524 | 431168 | 91325 | 25669 |
| `prefixoff` | `-mllvm -profile-guided-section-prefix=false` | 4507 | 4507 | 4524 | 431168 | 91325 | 25669 |
| `sink0` | `-mllvm -max-uses-for-sinking=0` | 4507 | 4507 | 4524 | 431168 | 91325 | 25669 |
| `sink0_prefixoff` | `-mllvm -max-uses-for-sinking=0 -mllvm -profile-guided-section-prefix=false` | 4507 | 4507 | 4524 | 431168 | 91325 | 25669 |
| `slp1` | `-mllvm -slp-threshold=1` | 1637 | 4503 | 4521 | 431168 | 94488 | 26395 |
| `inline0` | `-mllvm -inline-threshold=0` | 1356 | 4458 | 4498 | 432384 | 96534 | 26895 |
| `unroll50` | `-mllvm -unroll-threshold=50` | 1554 | 4504 | 4521 | 431168 | 94332 | 26184 |
| `loadpre` | `-mllvm -enable-load-pre=false` | 1404 | 4194 | 4332 | 431168 | 123739 | 32385 |
| `earlytaildup` | `-mllvm -disable-early-taildup=true` | 2700 | 4493 | 4516 | 431168 | 93617 | 26522 |
| `tailmerge0` | `-mllvm -tail-merge-threshold=0` | 1324 | 3855 | 3875 | 431168 | 142404 | 34583 |
| `bw7` | `-mllvm -likely-branch-weight=7 -mllvm -unlikely-branch-weight=1` (+ `GOF2_NO_PER_FILE_WEIGHTS=ON`) | 2111 | 4504 | 4518 | 431168 | 102927 | 27558 |
| `nounwind` | `-fno-unwind-tables` | 2878 | 4507 | 4524 | 431168 | 91871 | 26788 |
| `sp_basic` | `-fstack-protector` | 1369 | 3787 | 3794 | 431168 | 138423 | 38444 |
| `sp_all` | `-fstack-protector-all` | 91 | 969 | 981 | 431168 | 102806 | 28797 |
| `sp_none` | `-fno-stack-protector` | 1369 | 3783 | 3790 | 431168 | 138415 | 38444 |
| `sp_explicit` | `-fstack-protector-explicit` | — | — | — | — | — | — (clang 7.0.2: unknown argument) |

The eight neutral cells confirm the effective `-Oz` defaults: SLP threshold 0,
inline threshold <= 1, unroll threshold 0, GVN hoist inert, no profile data for the
section-prefix pass, and the entry-sinking threshold inert without a profile. Every non-neutral
variant is worse on both metrics.

### 3.1 `-fno-unwind-tables` launcher cell

The launcher appends the flag after the whole compiler argument list (the target's
`-funwind-tables` would otherwise win). The applied cell gives `byte 2878 / linked 4507 /
size 4524`, raw 91,871, aligned 26,788 — the canonical `-funwind-tables` is confirmed. Every
residual is unchanged or worse; e.g. `MGame::OnUpdate` 37/109 -> 164/223,
`MenuTouchWindow::OnTouchBegin` 29/15 -> 49/35, `SpaceLounge::drawLounge` 175/41 -> 203/125.

### 3.2 Stack-protector variants

`-fstack-protector` (basic) and `-fno-stack-protector` are indistinguishable on the gate
(`byte 1369`, `linked ~3785`, `size ~3792`) and both are far below `strong` (4507). Every
residual gets much worse. `-fstack-protector-all` collapses to `byte 91`.
**`-fstack-protector-strong` is the original's spelling.** `-fstack-protector-explicit` is not
accepted by clang 7.0.2.

## 4. New `-mllvm` hypotheses (wave-157)

Seven knobs not previously measured, all with the launcher cell:

| variant | flags | byte | linked | size | `.text` | rawsum | alignsum |
|---|---|---:|---:|---:|---:|---:|---:|
| `ifcvt_old` | `-mllvm -old-thumb2-ifcvt` | 4507 | 4507 | 4524 | 431168 | 91325 | 25669 |
| `guards1` | `-mllvm -guards-predicate-pass-branch-weight=1` | 4507 | 4507 | 4524 | 431168 | 91325 | 25669 |
| `exitbias100` | `-mllvm -block-placement-exit-block-bias=100` | 1741 | 4443 | 4480 | 431168 | 97381 | 26748 |
| `noplace` | `-mllvm -disable-block-placement` | 1254 | 3118 | 4121 | 431168 | 143143 | 72497 |
| `taildup_off` | `-mllvm -disable-tail-duplicate` | 1963 | 4450 | 4501 | 431168 | 94108 | 26171 |
| `gvnsink` | `-mllvm -enable-gvn-sink` | 1349 | 4336 | 4383 | 431168 | 130489 | 35761 |
| `machinesink_off` | `-mllvm -disable-machine-sink` | 1235 | 3737 | 4019 | 431168 | 136176 | 40865 |

`-old-thumb2-ifcvt` and `-guards-predicate-pass-branch-weight=1` are exactly neutral; the other
five are worse. None makes a residual exact.

## 5. The w10313 Status profile replayed on all 17 residuals

The committed `_work/reconstructions/status_instr_1432_relplt0_w10313.proftext` (116 counters,
func hash 2003737435339834133) was replayed on `src/game/mission/Status.cpp` with
`-fprofile-instr-use` + `-mllvm -max-uses-for-sinking=0` +
`-mllvm -profile-guided-section-prefix=false`, then the whole `.so` was relinked and all 17
residuals measured.

| function | baseline raw/aligned/size | w10313 profile |
|---|---|---|
| `Status::missionCompleted` | 722 / 320 / 1432 | **1299 / 583 / 1432** |
| all 16 other residuals | unchanged | unchanged |
| **17-name total** | **91325 / 25669** | **91902 / 25932** |

Full gate with the profile: `byte 4507 / linked 4507 / size 4524` (unchanged). The profile
reproduces the w10313 numbers exactly at the wave-157 tree, keeps the size exact, and touches no
other function. It is still not byte-exact (the original body is the unprofiled heuristic body;
see the w10313 note).

## 6. Uniform instrumented profile on every residual

For each residual TU/function a synthetic instrumented profile with **all counters = 1** was
generated from the TU's IR (`-fprofile-instr-generate -emit-llvm`), merged with
`llvm-profdata`, and compiled with `-fprofile-instr-use` + `-mllvm -max-uses-for-sinking=0` +
`-mllvm -profile-guided-section-prefix=false`. Only the target TU is recompiled; the other
functions in the TU are byte-identical (`other_same == other_total` in every row).

| function | baseline raw/aligned/size | uniform instrumented profile |
|---|---|---|
| `Layout::Layout` (C1/C2) | 5753 / 1984 / 7072 | alias not in IR (C1/C2 share one body) |
| `LevelScript::process` | 55094 / 17129 / 57232 | 55588 / 36313 / 57632 |
| `Level::createShip` | 1564 / 452 / 2764 | 2589 / 1041 / 2776 |
| `Level::createMission` | 5487 / 774 / 17172 | 16637 / 9093 / 17492 |
| `Status::missionCompleted` | 722 / 320 / 1432 | 1333 / 620 / 1416 |
| `Radar::draw` | 8181 / 1278 / 9908 | 9617 / 5838 / 10368 |
| `MGame::OnTouchEnd` | 4391 / 903 / 8080 | 7638 / 4106 / 8168 |
| `MGame::OnUpdate` | 37 / 109 / 11440 | 11018 / 4536 / 11868 |
| `Gun::calcCharacterCollision` | 463 / 57 / 1984 | 1841 / 853 / 1920 |
| `HangarWindow::render` | 1965 / 291 / 9700 | 9337 / 5835 / 9792 |
| `SpaceLounge::drawLounge` | 175 / 41 / 1948 | 1880 / 738 / 2000 |
| `Hud::draw` | 1325 / 249 / 9332 | 8987 / 3882 / 9764 |
| `Generator::createMission` | 300 / 59 / 1996 | 1928 / 907 / 2080 |
| `MenuTouchWindow::OnTouchBegin` | 29 / 15 / 1896 | 1808 / 478 / 1916 |
| `ModStation::ModStation` (C1/C2) | 43 / 12 / 1008 | alias not in IR (C1/C2 share one body) |

Every measured residual is strictly worse on both metrics; several change size, which would
shift `.text` and collapse `byte_exact`. **No residual becomes byte-exact.**

## 7. Conclusion

* The original is a single `-Oz` build; no TU was compiled at `-O1/-O2/-Os/-O3`.
* The stock clang 7.0.2 `-Oz` `-mllvm` defaults are the original's; no knob improves the gate.
  The eight neutral knobs are consistent with defaults.
* `-funwind-tables` and `-fstack-protector-strong` are confirmed as the original's flags; the
  alternatives collapse the gate.
* The w10313 Status profile is a faithful, size-exact provenance artifact but is not a gate win.
* A uniform instrumented profile on every residual is a regression. The flat switch-weight
  signature remains unreplayed by a generic profile.
* The wave-157 gate is unchanged: **4507/4524 byte, 4507/4524 linked, 4524/4524 size,
  `.text` 85,529 differing bytes, `.ARM.extab` 1,292, raw sum 91,325 (85,529 over the 15 unique
  bodies), aligned sum 25,669 (23,673 over the 15 unique bodies)**.

## 8. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10420
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6

# per-TU opt-level matrix (13 TUs x 5 levels)
python3 tools/per_tu_opt_sweep.py --build-root /dev/shm/w10420/pertu --jobs 13 \
  --out /dev/shm/w10420/pertu_results.json

# targeted flag matrix (compiler-launcher cell; run with --jobs 1 to avoid the
# shared third_party/libzip archive race between concurrent variant builds)
python3 /dev/shm/w10420/run_variant.py --spec /dev/shm/w10420/spec_mllvm.json \
  --out /dev/shm/w10420/mllvm_results.json --jobs 1
python3 /dev/shm/w10420/run_variant.py --spec /dev/shm/w10420/spec_new.json \
  --out /dev/shm/w10420/new_results.json --jobs 1

# w10313 Status profile replay
python3 /dev/shm/w10420/status_profile.py

# uniform instrumented profile on every residual
python3 /dev/shm/w10420/prof_all.py
```

### 8.1 Harness (self-contained)

`/dev/shm/w10420/run_variant.py` writes a per-variant launcher that appends the extra flags
after the whole compiler argument list, then configures/builds `link_so`, runs
`tools/verify/verify.py`, and measures `tools/fn_diff.py` raw/aligned on the 17 residual names:

```python
launcher = os.path.join(bd, 'launcher.sh')
open(launcher, 'w').write('#!/bin/sh\ncomp="$1"; shift\nexec "$comp" "$@" %s\n' % flags)
os.chmod(launcher, 0o755)
cfg = ('cmake -S %s -B %s -G Ninja '
       '-DCMAKE_CXX_COMPILER_LAUNCHER=%s -DCMAKE_C_COMPILER_LAUNCHER=%s '
       '-DGOF2_ORIGINAL_SO=%s -DGOF2_FMOD_LIBDIR=%s %s' %
       (ROOT, bd, launcher, launcher, ORIG, FMOD, extra_cmake))
```

`/dev/shm/w10420/profgen.py` is the committed `profgen_w10314.py` with `REPO` pointed at this
worktree; `/dev/shm/w10420/prof_one.py` builds a uniform instrumented profile for one function.

**Important:** concurrent variant builds race on the shared source-tree archive
`third_party/libzip/libzip_gof2.a` (the vendored-library step writes it in place). Two cells
(`slp1`, `gvnhoist`) failed this way in the parallel run and were re-measured with `--jobs 1`;
their values above are from the sequential retry.

## 9. Files touched

* `_work/reconstructions/build-provenance-wave157-w10420.md` (this note).
* No source, tool, CMake, baseline, allowlist or doc change.
