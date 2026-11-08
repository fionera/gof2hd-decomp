# Build provenance at wave 156 (w10320): per-TU opt levels, `-mllvm`, unwind/stack-protector
# cells, and synthetic sample profiles on all 17 residuals

Status: **research note only**, 2026-11-08. Worktree `wt/w10320`, base master `08575efff`
(wave 156). Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000). Scratch `/dev/shm/w10320`. No `src/`,
`tools/`, `CMakeLists.txt`, baseline, allowlist or doc change is landed; this note is the only
commit.

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0,
parity 5370/5370. `.text` **85,831** differing bytes, `.ARM.extab` **1,292**,
`.rel.plt` 0, `.note.gnu.build-id` 20. All 27 section sizes exact; file size 2,187,328 B.

## 0. Bottom line

1. **No per-TU optimization level beats `-Oz` at the wave-156 tree.** The full 13-TU x 5-level
   matrix reproduces the wave-154/155 conclusion. `-Oz` is the minimum of both raw and aligned
   diff for every one of the 17 residual names. The closest non-`-Oz` cell is `Layout::Layout`
   at `-Os` (raw 5,862 / aligned 2,128 vs 5,822 / 1,984 at `-Oz`).
2. **No `-mllvm` knob improves the gate.** 16 variants re-measured. Five are exactly neutral
   (`byte 4507 / linked 4507 / size 4524`, raw sum 91,696, aligned sum 25,682):
   `-slp-threshold=0`, `-inline-threshold=1`, `-unroll-threshold=0`,
   `-enable-gvn-hoist=false`, `-profile-guided-section-prefix=false`. Every other variant is
   strictly worse on both metrics.
3. **`-fno-unwind-tables` now really is applied (launcher cell) and is a regression.**
   w10119's `CMAKE_CXX_FLAGS` cell was overridden by the target's later `-funwind-tables`; a
   compiler-launcher cell that appends after all arguments gives `byte 2878`, raw 92,238,
   aligned 26,787 (base 4507 / 91,696 / 25,682). The canonical `-funwind-tables` is confirmed.
4. **`-fstack-protector-strong` is the only viable stack-protector spelling.** `-fstack-protector`
   (basic) and `-fno-stack-protector` both collapse the gate to `byte 1377 / linked ~3785 /
   size ~3790`; `-fstack-protector-all` gives `byte 83`. The original is `strong`.
5. **A global 7:1 branch-weight default is not a win.** `-mllvm -likely-branch-weight=7
   -mllvm -unlikely-branch-weight=1` with the per-file overrides disabled gives
   `byte 2045`, raw 103,043, aligned 27,602.
6. **A synthetic sample profile is not a win.** A uniform line-based profile on all 17 residuals
   gives `byte 1668`, raw 146,639, aligned 79,369 (with
   `-mllvm -profile-guided-section-prefix=false`). The header-only profile is exactly neutral.
   Per-function profiles on the three closest residuals make each of them worse.
7. **No flag or profile makes any of the 17 residuals byte-exact.** The residual set remains
   source-shape / TU-shape work.
8. Nothing is landable: the canonical `-Oz` build remains the best on every measured axis.

## 1. Baseline

```text
verify: byte 4507  linked 4507  size 4524/4524  avg 99.93
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0  imports 207
lint CLEAN   drift 0   symbol parity 5370/5370
RATCHET PASS
```

Residual raw / aligned / size at the wave-156 tree (17 names, 15 unique bodies;
`LayoutC1/C2` and `ModStationC1/C2` are aliases):

| function | raw | aligned | size |
|---|---:|---:|---:|
| `Layout::Layout` (C1/C2) | 5,822 | 1,984 | 7,072 |
| `LevelScript::process` | 55,095 | 17,131 | 57,232 |
| `Level::createShip` | 1,564 | 452 | 2,764 |
| `Level::createMission` | 5,487 | 774 | 17,172 |
| `Status::missionCompleted` | 722 | 320 | 1,432 |
| `Radar::draw` | 8,320 | 1,296 | 9,908 |
| `MGame::OnTouchEnd` | 4,391 | 903 | 8,080 |
| `MGame::OnUpdate` | 37 | 109 | 11,440 |
| `Gun::calcCharacterCollision` | 463 | 57 | 1,984 |
| `HangarWindow::render` | 1,965 | 291 | 9,700 |
| `SpaceLounge::drawLounge` | 175 | 41 | 1,948 |
| `Hud::draw` | 1,325 | 249 | 9,332 |
| `Generator::createMission` | 393 | 52 | 1,996 |
| `MenuTouchWindow::OnTouchBegin` | 29 | 15 | 1,896 |
| `ModStation::ModStation` (C1/C2) | 43 | 12 | 1,008 |

Raw sum over the 17 names **91,696**; aligned sum **25,682**. Over the 15 unique bodies
(excluding the two C2 aliases) the sums are **85,831** and **23,686** — the unique raw sum is
exactly the `.text` differing-byte count.

## 2. Per-TU `-Oz/-O1/-O2/-Os/-O3` sweep

Harness: `tools/per_tu_opt_sweep.py` with `tools/per_tu_flags.cmake` (each cell is a full
independent scratch build; `-Oz` reproduces the canonical gate exactly). 13 TUs x 5 levels.

### 2.1 Residual raw/aligned/size (lower is better)

| TU / function | -Oz | -O1 | -O2 | -Os | -O3 |
|---|---|---|---|---|---|
| `Layout.cpp` `LayoutC1` | **5822/1984/7072** | 7819/4766/8020 | 6113/2170/7120 | 5862/2128/7072 | 6492/3746/7292 |
| `LevelScript.cpp` `process` | **55095/17131/57232** | 59833/37258/61576 | 61604/33985/63236 | 55913/24322/57608 | 64601/34955/66232 |
| `Level.cpp` `createShip` | **1564/452/2764** | 2979/1613/3060 | 2951/1095/3064 | 2876/1066/2988 | 2968/1097/3080 |
| `Level.cpp` `createMission` | **5487/774/17172** | 18220/8827/18784 | 18778/10645/19312 | 17565/6032/18176 | 19763/7838/20288 |
| `Status.cpp` `missionCompleted` | **722/320/1432** | 1877/855/1940 | 1692/778/1760 | 1328/628/1456 | 1764/979/1840 |
| `Radar.cpp` `draw` | **8320/1296/9908** | 10195/4835/10508 | 10061/4430/10384 | 9975/4275/10284 | 10470/4213/10816 |
| `MGame.cpp` `OnTouchEnd` | **4391/903/8080** | 8081/3103/8448 | 9402/3766/9956 | 7828/2521/8288 | 9768/3865/10152 |
| `MGame.cpp` `OnUpdate` | **37/109/11440** | 11493/4707/12020 | 14025/5638/14324 | 10987/1884/11504 | 14051/5680/14420 |
| `Gun.cpp` `calcCharacterCollision` | **463/57/1984** | 1911/549/2000 | 1927/346/2032 | 1879/322/1984 | 1932/349/2040 |
| `HangarWindow.cpp` `render` | **1965/291/9700** | 9608/4925/9940 | 9569/2214/9904 | 9439/2124/9816 | 9871/5316/10180 |
| `SpaceLounge.cpp` `drawLounge` | **175/41/1948** | 1965/453/2056 | 1861/224/1976 | 1858/236/1972 | 1899/230/2004 |
| `Hud.cpp` `draw` | **1325/249/9332** | 8874/1943/9452 | 9475/1998/9740 | 9123/1666/9528 | 9659/2321/9940 |
| `Generator.cpp` `createMission` | **393/52/1996** | 2127/857/2224 | 2083/903/2164 | 1974/596/2080 | 2408/960/2464 |
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
| `Generator.cpp` | `-Os` | 1668 | 4499 | 4516 | 431168 |
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
| `Layout.cpp` | `-O3` | 1369 | 4478 | 4497 | 431104 |
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
| `Radar.cpp` | `-Os` | 1369 | 4500 | 4516 | 431168 |
| `Radar.cpp` | `-O3` | 1377 | 4498 | 4514 | 431136 |
| `Level.cpp` | `-O1` | 1363 | 4433 | 4466 | 433600 |
| `Level.cpp` | `-O2` | 1351 | 4448 | 4465 | 430400 |
| `Level.cpp` | `-Os` | 1353 | 4455 | 4474 | 430720 |
| `Level.cpp` | `-O3` | 1351 | 4448 | 4465 | 430400 |
| `LevelScript.cpp` | `-O1` | 1453 | 4499 | 4515 | 432032 |
| `LevelScript.cpp` | `-O2` | 1369 | 4499 | 4516 | 431104 |
| `LevelScript.cpp` | `-Os` | 1378 | 4501 | 4517 | 431104 |
| `LevelScript.cpp` | `-O3` | 1443 | 4499 | 4515 | 431104 |

**Conclusion:** `-Oz` is optimal for every residual on both metrics. No TU was compiled at
another level.

## 3. Targeted `-mllvm` / unwind / stack-protector matrix

Harness: `/dev/shm/w10320/run_variant.py` (one build directory per variant, compiler-launcher
cell so the extra flags are appended *after* the canonical flags and after the target's
`-ffunction-sections -funwind-tables`). `rawsum` / `alignsum` are the sums over the 17 residual
names; `base` is the canonical `-Oz` build.

| variant | flags | byte | linked | size | `.text` | rawsum | alignsum |
|---|---|---:|---:|---:|---:|---:|---:|
| base | — | **4507** | **4507** | **4524** | **431168** | **91696** | **25682** |
| `slp0` | `-mllvm -slp-threshold=0` | 4507 | 4507 | 4524 | 431168 | 91696 | 25682 |
| `inline1` | `-mllvm -inline-threshold=1` | 4507 | 4507 | 4524 | 431168 | 91696 | 25682 |
| `unroll0` | `-mllvm -unroll-threshold=0` | 4507 | 4507 | 4524 | 431168 | 91696 | 25682 |
| `gvnhoist` | `-mllvm -enable-gvn-hoist=false` | 4507 | 4507 | 4524 | 431168 | 91696 | 25682 |
| `prefixoff` | `-mllvm -profile-guided-section-prefix=false` | 4507 | 4507 | 4524 | 431168 | 91696 | 25682 |
| `slp1` | `-mllvm -slp-threshold=1` | 1637 | 4503 | 4521 | 431168 | 94841 | 26409 |
| `inline0` | `-mllvm -inline-threshold=0` | 1356 | 4458 | 4498 | 432384 | 96885 | 26907 |
| `unroll50` | `-mllvm -unroll-threshold=50` | 1554 | 4504 | 4521 | 431168 | 94684 | 26197 |
| `loadpre` | `-mllvm -enable-load-pre=false` | 1410 | 4194 | 4332 | 431168 | 123822 | 32451 |
| `earlytaildup` | `-mllvm -disable-early-taildup=true` | 2700 | 4493 | 4516 | 431168 | 94040 | 26533 |
| `tailmerge0` | `-mllvm -tail-merge-threshold=0` | 1338 | 3855 | 3875 | 431168 | 142603 | 34581 |
| `bw7` | `-mllvm -likely-branch-weight=7 -mllvm -unlikely-branch-weight=1` (+ `GOF2_NO_PER_FILE_WEIGHTS=ON`) | 2045 | 4504 | 4518 | 431168 | 103043 | 27602 |
| `nounwind` | `-fno-unwind-tables` | 2878 | 4507 | 4524 | 431168 | 92238 | 26787 |
| `sp_basic` | `-fstack-protector` | 1377 | 3786 | 3794 | 431168 | 138577 | 38441 |
| `sp_all` | `-fstack-protector-all` | 83 | 969 | 981 | 431168 | 103095 | 28976 |
| `sp_none` | `-fno-stack-protector` | 1377 | 3783 | 3790 | 431168 | 138588 | 38443 |

The five neutral `-mllvm` cells confirm the effective `-Oz` defaults: SLP threshold 0,
inline threshold <= 1, unroll threshold 0, GVN hoist inert, and no profile data for the
section-prefix pass. Every non-neutral variant is worse on both metrics.

### 3.1 `-fno-unwind-tables` launcher cell (new)

w10119's `-fno-unwind-tables` cell was ineffective because `target_compile_options(gof2
PRIVATE -ffunction-sections -funwind-tables)` appends `-funwind-tables` after `CMAKE_CXX_FLAGS`.
`/dev/shm/w10320/run_variant.py` uses `CMAKE_CXX_COMPILER_LAUNCHER` with a script that appends
the extra flags after the whole argument list:

```sh
#!/bin/sh
comp="$1"; shift
exec "$comp" "$@" -fno-unwind-tables
```

The applied cell gives `byte 2878 / linked 4507 / size 4524`, raw 92,238, aligned 26,787 —
matching the wave-155 archaeology's `-fno-unwind-tables` count (2,878) and worse than base on
both residual metrics. Every residual is unchanged or worse; e.g. `MGame::OnUpdate` 37/109 ->
164/223, `MenuTouchWindow::OnTouchBegin` 29/15 -> 49/35, `SpaceLounge::drawLounge` 175/41 ->
203/125. The canonical `-funwind-tables` is confirmed.

### 3.2 Stack-protector variants

`-fstack-protector` (basic) and `-fno-stack-protector` are indistinguishable on the gate
(`byte 1377`, `linked ~3785`, `size ~3790`) and both are far below `strong` (4507). Every
residual gets much worse (e.g. `MGame::OnUpdate` 37/109 -> 10,817/1,069). `-fstack-protector-all`
collapses to `byte 83`. **`-fstack-protector-strong` is the original's spelling.**

### 3.3 Global 7:1 branch-weight default

With the per-file `StarMap.cpp` / `SpaceLounge.cpp` 7:1 overrides disabled
(`GOF2_NO_PER_FILE_WEIGHTS=ON`), a global `-mllvm -likely-branch-weight=7
-mllvm -unlikely-branch-weight=1` gives `byte 2045`, raw 103,043, aligned 27,602. It is not a
win. (`LevelScript::process` aligned 17,127 is 4 below base, but raw 55,203 and size 57,228 are
worse, so the gate falls.)

## 4. Synthetic sample profile on all 17 residuals

### 4.1 Profile construction

A text-format sample profile is generated from the DWARF line table of an unstripped link
(`/dev/shm/w10320/unstripped.so`, produced by replaying the gold link command without the strip
step). For each residual mangled name the generator finds the symbol range, takes the line at
the function entry as the base, and emits `offset = line - base` for every line in the range
(negative offsets, i.e. inlined header lines below the function's declaration line, are dropped
because the text format rejects them):

```text
_ZN6LayoutC1Ev:1000:0
 0: 1000
 1: 1000
 ...
```

Variants (all built with the canonical flags plus the extra flags):

| id | extra flags |
|---|---|
| `sample_all` | `-fprofile-sample-use=all.prof -mllvm -profile-guided-section-prefix=false` |
| `sample_all_prefixon` | `-fprofile-sample-use=all.prof` (section-prefix pass ON) |
| `sample_header` | header-only profile (entry counts, no line samples) + prefix off |
| `sample_line0` | header + one `0: 1000` line per function + prefix off |
| `sample_menutouch` | profile only `MenuTouchWindow::OnTouchBegin` + prefix off |
| `sample_mgameupdate` | profile only `MGame::OnUpdate` + prefix off |
| `sample_modstation` | profile only `ModStation::ModStation` C1/C2 + prefix off |

### 4.2 Results

| variant | byte | linked | size | `.text` | rawsum | alignsum |
|---|---|---:|---:|---:|---:|---:|
| base | **4507** | **4507** | **4524** | **431168** | **91696** | **25682** |
| `sample_all` | 1668 | 4507 | 4509 | 431168 | 146639 | 79369 |
| `sample_all_prefixon` | 1369 | 4507 | 4509 | 431168 | 146968 | 79563 |
| `sample_header` | 4507 | 4507 | 4524 | 431168 | 91696 | 25682 |
| `sample_line0` | 4044 | 4507 | 4524 | 431168 | 93059 | 25830 |
| `sample_menutouch` | 2693 | 4507 | 4523 | 431168 | 95722 | 26649 |
| `sample_mgameupdate` | 3108 | 4507 | 4523 | 431168 | 102779 | 31209 |
| `sample_modstation` | 4507 | 4507 | 4524 | 431168 | 92914 | 25744 |

Per-residual raw/aligned/size for the targeted profiles:

| function | base | `sample_menutouch` | `sample_mgameupdate` | `sample_modstation` |
|---|---|---|---|---|
| `MenuTouchWindow::OnTouchBegin` | 29/15/1896 | 1859/653/1944 | 29/15/1896 | 29/15/1896 |
| `MGame::OnUpdate` | 37/109/11440 | 792/219/11440 | 11032/5429/11548 | 37/109/11440 |
| `ModStation::ModStation` | 43/12/1008 | 48/15/1008 | 48/15/1008 | 652/43/1008 |

Observations:

* The **header-only** profile is bit-for-bit neutral: `function_entry_count` alone does not
  move these bodies.
* The **uniform all-lines** profile changes 15 function sizes and increases both metrics on
  every residual. It is not a win.
* The **single line-0** profile keeps all sizes exact but still worsens the aggregate raw
  (91,696 -> 93,059) and aligned (25,682 -> 25,830) sums; `ModStation` alone goes 43/12 ->
  695/57.
* With the section-prefix pass left ON, the profiled functions are renamed `.text.hot.*` and
  moved to the front of `.text`; `byte_exact` collapses to 1369 even though `linked_exact`
  stays 4507. `-mllvm -profile-guided-section-prefix=false` keeps the addresses but is still
  not a win (`byte 1668`, `size 4509`).
* The **per-function** profiles are the cleanest test of the "profile replay" hypothesis: each
  changes only its own function, and each makes that function worse. `MenuTouchWindow` 29 ->
  1,859 raw; `MGame::OnUpdate` 37 -> 11,032 raw; `ModStation` 43 -> 652 raw.
* **No residual becomes byte-exact under any sample-profile variant.**

For reference, the wave-154 IR-level flat `!prof` vector for `Status::missionCompleted` (not a
build flag or sample profile) still gives raw 558 / aligned 47 at size 1432; it is the only
known way to improve that function, and it remains a source/IR-shape experiment.

## 5. Conclusion

* The original is a single `-Oz` build; no TU was compiled at `-O1/-O2/-Os/-O3`.
* The stock clang 7.0.2 `-Oz` `-mllvm` defaults are the original's; no knob improves the gate,
  and the five neutral knobs are consistent with defaults.
* `-funwind-tables` and `-fstack-protector-strong` are confirmed as the original's flags; the
  alternatives collapse the gate.
* A global 7:1 branch-weight default is not a win.
* A synthetic sample profile on all 17 residuals (uniform, header-only, line-0, or per-function)
  is a regression or exactly neutral. The flat switch-weight signature remains unreplayed by a
  line-based sample profile.
* The wave-156 gate is unchanged: **4507/4524 byte, 4507/4524 linked, 4524/4524 size,
  `.text` 85,831, `.ARM.extab` 1,292, raw sum 91,696 (85,831 over the 15 unique bodies),
  aligned sum 25,682 (23,686 over the 15 unique bodies)**.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10320
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6

# per-TU opt-level matrix (13 TUs x 5 levels)
python3 tools/per_tu_opt_sweep.py --build-root /dev/shm/w10320/pertu --jobs 13 \
  --out /dev/shm/w10320/pertu_results.json

# targeted flag matrix (compiler-launcher cell)
python3 /dev/shm/w10320/run_variant.py --spec /dev/shm/w10320/spec_mllvm.json \
  --out /dev/shm/w10320/mllvm_results.json --jobs 6

# synthetic sample profile
python3 /dev/shm/w10320/gen_sample_profile.py uniform /dev/shm/w10320/all.prof
python3 /dev/shm/w10320/run_variant.py --spec /dev/shm/w10320/spec_sample.json \
  --out /dev/shm/w10320/sample_results.json --jobs 4
```

### 6.1 Harness (self-contained)

`/dev/shm/w10320/run_variant.py` writes a per-variant launcher that appends the extra flags
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

`/dev/shm/w10320/gen_sample_profile.py` parses `objdump --dwarf=decodedline` on the unstripped
link, maps each residual symbol range to its line offsets, and writes the text profile:

```python
inr = [(ad, ln) for ad, ln in R if a <= ad < a + sz]
base = inr[0][1]
offs = sorted(set(ln - base for ad, ln in inr if ln >= base))
body = ['%s:1000:0' % name] + [' %d: 1000' % o for o in offs]
```

## 7. Files touched

* `_work/reconstructions/build-flag-profile-replay-wave156-w10320.md` (this note).
* No source, tool, CMake, baseline, allowlist or doc change.
