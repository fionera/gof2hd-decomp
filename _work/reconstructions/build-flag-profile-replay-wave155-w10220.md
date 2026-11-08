# Build-provenance replay at wave 155 (w10220): per-TU opt levels, `-mllvm`,
# and a synthetic sample profile on all 17 residuals

Status: **research note only**, 2026-11-08. Worktree `wt/w10220`, base
master `cf74f4ee6` (wave 155). Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000). Scratch
`/dev/shm/w10220`. No `src/`, `tools/`, `CMakeLists.txt`, baseline,
allowlist or doc change is landed; this note is the only commit.

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370. `.text` 86,044 differing bytes, `.ARM.extab`
1,292, `.rel.plt` 0, `.note.gnu.build-id` 20. All 27 section sizes exact;
file size 2,187,328 B.

## 0. Bottom line

1. **No per-TU optimization level beats `-Oz`.** Re-running the full
   13-TU x 5-level matrix at the wave-155 tree reproduces the wave-154
   conclusion: `-Oz` is the minimum of both raw and aligned diff for all 17
   residuals. The closest non-`-Oz` cell is `Layout::Layout` at `-Os`
   (raw 5,879 / aligned 2,132 vs 5,837 / 1,986 at `-Oz`).
2. **No `-mllvm` knob improves the gate.** 21 variants re-measured. Six are
   exactly neutral (`byte 4507 / linked 4507 / size 4524`, raw sum 91,924,
   aligned sum 25,895): `-slp-threshold=0`, `-inline-threshold=1`,
   `-inline-threshold=5`, `-unroll-threshold=0`, `-enable-gvn-hoist=false`,
   `-profile-guided-section-prefix=false`. Every other variant is strictly
   worse on both metrics.
3. **A synthetic sample profile is not a win.** A line-based profile covering
   all 17 residuals (uniform 1000 samples on every DWARF line) changes 15
   function sizes and collapses the gate (`byte 1668`, `size 4509` with
   `-profile-guided-section-prefix=false`). Even the header-only profile
   (entry counts but no line samples) is exactly neutral, and a single
   line-0 sample is worse (`byte 4044`, raw 93,287, aligned 26,043).
4. **No flag or profile makes any of the 17 residuals byte-exact.** The
   residual set remains source-shape / TU-shape work.
5. Nothing is landable: the canonical `-Oz` build remains the best on every
   measured axis.

## 1. Baseline

```text
verify: byte 4507  linked 4507  size 4524/4524  avg 99.93
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0  imports 207
lint CLEAN   drift 0   symbol parity 5370/5370
RATCHET PASS
```

Residual raw / aligned / size at the wave-155 tree (17 names, 15 unique
bodies; `LayoutC1/C2` and `ModStationC1/C2` are aliases):

| function | raw | aligned | size |
|---|---:|---:|---:|
| `Layout::Layout` (C1/C2) | 5,837 | 1,986 | 7,072 |
| `LevelScript::process` | 55,123 | 17,180 | 57,232 |
| `Level::createShip` | 1,564 | 452 | 2,764 |
| `Level::createMission` | 5,487 | 774 | 17,172 |
| `Status::missionCompleted` | 722 | 320 | 1,432 |
| `Radar::draw` | 8,490 | 1,456 | 9,908 |
| `MGame::OnTouchEnd` | 4,391 | 903 | 8,080 |
| `MGame::OnUpdate` | 37 | 109 | 11,440 |
| `Gun::calcCharacterCollision` | 463 | 57 | 1,984 |
| `HangarWindow::render` | 1,965 | 291 | 9,700 |
| `SpaceLounge::drawLounge` | 175 | 41 | 1,948 |
| `Hud::draw` | 1,325 | 249 | 9,332 |
| `Generator::createMission` | 393 | 52 | 1,996 |
| `MenuTouchWindow::OnTouchBegin` | 29 | 15 | 1,896 |
| `ModStation::ModStation` (C1/C2) | 43 | 12 | 1,008 |

Raw sum over the 17 names **91,924**; aligned sum **25,895** (23,897 over
the 15 unique bodies, the wave-155 headline).

## 2. Per-TU `-Oz/-O1/-O2/-Os/-O3` sweep

Harness: `tools/per_tu_opt_sweep.py` with
`tools/per_tu_flags.cmake` (each cell is a full independent scratch build;
`-Oz` reproduces the canonical gate exactly). 13 TUs x 5 levels.

### 2.1 Residual raw/aligned (lower is better)

| TU / function | -Oz | -O1 | -O2 | -Os | -O3 |
|---|---|---|---|---|---|
| `Layout.cpp` `LayoutC1` | **5837/1986** | 7819/4766 | 6113/2173 | 5879/2132 | 6484/3739 |
| `LevelScript.cpp` `process` | **55123/17180** | 59841/37291 | 61597/34016 | 55828/24482 | 64543/34986 |
| `Level.cpp` `createShip` | **1564/452** | 2979/1613 | 2951/1095 | 2876/1066 | 2968/1097 |
| `Level.cpp` `createMission` | **5487/774** | 18220/8827 | 18778/10645 | 17565/6032 | 19763/7838 |
| `Status.cpp` `missionCompleted` | **722/320** | 1877/855 | 1692/778 | 1328/628 | 1764/979 |
| `Radar.cpp` `draw` | **8490/1456** | 10189/5068 | 10085/4568 | 9958/4545 | 10301/4022 |
| `MGame.cpp` `OnTouchEnd` | **4391/903** | 8081/3103 | 9402/3766 | 7828/2521 | 9768/3865 |
| `MGame.cpp` `OnUpdate` | **37/109** | 11493/4707 | 14025/5638 | 10987/1884 | 14051/5680 |
| `Gun.cpp` `calcCharacterCollision` | **463/57** | 1911/549 | 1927/346 | 1879/322 | 1932/349 |
| `HangarWindow.cpp` `render` | **1965/291** | 9608/4925 | 9569/2214 | 9439/2124 | 9871/5316 |
| `SpaceLounge.cpp` `drawLounge` | **175/41** | 1965/453 | 1861/224 | 1858/236 | 1899/230 |
| `Hud.cpp` `draw` | **1325/249** | 8874/1943 | 9475/1998 | 9123/1666 | 9659/2321 |
| `Generator.cpp` `createMission` | **393/52** | 2127/857 | 2083/903 | 1974/596 | 2408/960 |
| `MenuTouchWindow.cpp` `OnTouchBegin` | **29/15** | 2084/689 | 1876/737 | 1873/736 | 1939/743 |
| `ModStation.cpp` `ModStationC1` | **43/12** | 957/290 | 946/44 | 919/42 | 1048/98 |

(`LayoutC2` and `ModStationC2` are identical to their C1 rows.)

### 2.2 Full gate / `.text` VMA per cell

| TU | level | byte | linked | size | `.text` VMA |
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
| `Radar.cpp` | `-O2` | 1470 | 4498 | 4514 | 431136 |
| `Radar.cpp` | `-Os` | 1378 | 4500 | 4516 | 431168 |
| `Radar.cpp` | `-O3` | 1377 | 4498 | 4514 | 431136 |
| `Level.cpp` | `-O1` | 1363 | 4433 | 4466 | 433600 |
| `Level.cpp` | `-O2` | 1351 | 4448 | 4465 | 430400 |
| `Level.cpp` | `-Os` | 1353 | 4455 | 4474 | 430720 |
| `Level.cpp` | `-O3` | 1351 | 4448 | 4465 | 430400 |
| `LevelScript.cpp` | `-O1` | 1453 | 4499 | 4515 | 432032 |
| `LevelScript.cpp` | `-O2` | 1369 | 4500 | 4516 | 431104 |
| `LevelScript.cpp` | `-Os` | 1378 | 4501 | 4517 | 431104 |
| `LevelScript.cpp` | `-O3` | 1369 | 4499 | 4515 | 431104 |

The `.text` column is the section VMA (`0x69440`), which is fixed by the
link layout; the actual section size changes with the cell and is reflected
in `size_exact` (the canonical `.text` size is 1,259,464 B). The gate
collapse for a single-TU override is the address-shift artefact
described in `per-tu-opt-level-sweep-w10115.md`: the TU's function sizes
change, every later TU shifts, and every `bl`/literal-pool word into the
shifted range differs. The residual-function columns are the meaningful
comparison.

**Conclusion:** `-Oz` is optimal for every residual on both metrics. No TU
was compiled at another level.

## 3. Targeted `-mllvm` matrix

Harness: `/dev/shm/w10220/mllvm_parallel.py` (same flags and verifier as
`mllvm_sweep_w10116.py`, but one build directory per variant so variants run
in parallel). `rawsum` / `alignsum` are the sums over the 17 residual names.

| variant | byte | linked | size | `.text` | rawsum | alignsum |
|---|---:|---:|---:|---:|---:|---:|
| base | **4507** | **4507** | **4524** | **1259464** | **91924** | **25895** |
| `-slp-threshold=0` | 4507 | 4507 | 4524 | 1259464 | 91924 | 25895 |
| `-slp-threshold=1` | 1637 | 4503 | 4521 | 1259560 | 95070 | 26619 |
| `-slp-threshold=2` | 1637 | 4499 | 4515 | 1259528 | 95158 | 26444 |
| `-slp-threshold=4` | 1452 | 4407 | 4427 | 1256744 | 107681 | 29120 |
| `-slp-threshold=8` | 1484 | 4393 | 4412 | 1256712 | 116152 | 29250 |
| `-slp-max-reg-size=1` | 1453 | 4391 | 4411 | 1257012 | 116032 | 29231 |
| `-tail-merge-threshold=0` | 1324 | 3855 | 3875 | 1357704 | 137397 | 34844 |
| `-tail-merge-threshold=1` | 1338 | 3854 | 3875 | 1357704 | 137401 | 34844 |
| `-tail-merge-threshold=2` | 1346 | 4074 | 4105 | 1334024 | 134893 | 33136 |
| `-tail-merge-threshold=4` | 1367 | 4357 | 4373 | 1270536 | 121230 | 29689 |
| `-inline-threshold=0` | 1356 | 4458 | 4498 | 1259112 | 97013 | 27122 |
| `-inline-threshold=1` | 4507 | 4507 | 4524 | 1259464 | 91924 | 25895 |
| `-inline-threshold=5` | 4507 | 4507 | 4524 | 1259464 | 91924 | 25895 |
| `-unroll-threshold=0` | 4507 | 4507 | 4524 | 1259464 | 91924 | 25895 |
| `-unroll-threshold=50` | 1554 | 4504 | 4521 | 1259432 | 94907 | 26410 |
| `-unroll-threshold=150` | 1413 | 4498 | 4516 | 1260200 | 96373 | 26875 |
| `-enable-gvn-hoist=false` | 4507 | 4507 | 4524 | 1259464 | 91924 | 25895 |
| `-enable-load-pre=false` | 1402 | 4194 | 4331 | 1257800 | 123731 | 31802 |
| `-disable-early-taildup=true` | 3594 | 4493 | 4517 | 1259464 | 93993 | 26766 |
| `-profile-guided-section-prefix=false` | 4507 | 4507 | 4524 | 1259464 | 91924 | 25895 |

The six neutral variants confirm the effective `-Oz` defaults: SLP threshold
0, inline threshold <= 5, unroll threshold 0, GVN hoist inert, and no profile
data for the section-prefix pass. Every non-neutral variant is worse on both
metrics. **No `-mllvm` knob is a win.**

## 4. Synthetic sample profile on all 17 residuals

### 4.1 Profile construction

A text-format sample profile was generated from the DWARF line table of an
unstripped link (`/dev/shm/w10220/unstripped.so`). For each residual mangled
name the generator finds the symbol range, takes the line at the function
entry as the base, and emits `offset = line - base` for every line in the
range (negative offsets, i.e. inlined header lines below the function's
declaration line, are dropped because the text format rejects them):

```text
_ZN6LayoutC1Ev:1000:0
 0: 1000
 1: 1000
 ...
```

Variants (all built with the canonical flags plus the extra flags):

| id | extra flags |
|---|---|
| `sample` | `-fprofile-sample-use=all.prof` (section-prefix pass ON) |
| `sample_prefixoff` | `-fprofile-sample-use=all.prof -mllvm -profile-guided-section-prefix=false` |
| `sample_prefixoff_sink0` | above + `-mllvm -max-uses-for-sinking=0` |
| `sample_header` | header-only profile (entry counts, no line samples) + prefix off |
| `sample_line0` | header + one `0: 1000` line per function + prefix off |

### 4.2 Results

| variant | byte | linked | size | `.text` | rawsum | alignsum |
|---|---:|---:|---:|---:|---:|---:|
| base | **4507** | **4507** | **4524** | **1259464** | **91924** | **25895** |
| `sample` | 1369 | 4507 | 4509 | 1261256 | 146947 | 79219 |
| `sample_prefixoff` | 1668 | 4506 | 4509 | 1261256 | 146589 | 79021 |
| `sample_prefixoff_sink0` | 1668 | 4507 | 4509 | 1260840 | 145825 | 78957 |
| `sample_header` | 4507 | 4507 | 4524 | 431168 | 91924 | 25895 |
| `sample_line0` | 4044 | 4507 | 4524 | 431168 | 93287 | 26043 |

Per-residual (raw/aligned, size ours/orig) for the two most interesting
variants:

| function | base | `sample_prefixoff` | `sample_header` | `sample_line0` |
|---|---|---|---|---|
| `LayoutC1` | 5837/1986 7072/7072 | 6845/4133 7300/7072 | 5837/1986 7072/7072 | 5837/1986 7072/7072 |
| `LevelScript::process` | 55123/17180 57232/57232 | 55593/32843 57320/57232 | 55123/17180 57232/57232 | 55123/17180 57232/57232 |
| `Level::createShip` | 1564/452 2764/2764 | 2502/1496 2800/2764 | 1564/452 2764/2764 | 1564/452 2764/2764 |
| `Level::createMission` | 5487/774 17172/17172 | 16918/8929 17500/17172 | 5487/774 17172/17172 | 5493/785 17172/17172 |
| `Status::missionCompleted` | 722/320 1432/1432 | 1372/714 1352/1432 | 722/320 1432/1432 | 722/320 1432/1432 |
| `Radar::draw` | 8490/1456 9908/9908 | 10454/5379 10704/9908 | 8490/1456 9908/9908 | 8490/1456 9908/9908 |
| `MGame::OnTouchEnd` | 4391/903 8080/8080 | 7668/3862 8052/8080 | 4391/903 8080/8080 | 4395/906 8080/8080 |
| `MGame::OnUpdate` | 37/109 11440/11440 | 11055/5429 11548/11440 | 37/109 11440/11440 | 51/121 11440/11440 |
| `Gun::calcCharacterCollision` | 463/57 1984/1984 | 1869/624 1916/1984 | 463/57 1984/1984 | 463/57 1984/1984 |
| `HangarWindow::render` | 1965/291 9700/9700 | 9291/6029 9724/9700 | 1965/291 9700/9700 | 1981/306 9700/9700 |
| `SpaceLounge::drawLounge` | 175/41 1948/1948 | 1873/571 1960/1948 | 175/41 1948/1948 | 184/48 1948/1948 |
| `Hud::draw` | 1325/249 9332/9332 | 9138/3369 9532/9332 | 1325/249 9332/9332 | 1335/259 9332/9332 |
| `Generator::createMission` | 393/52 1996/1996 | 1971/769 2064/1996 | 393/52 1996/1996 | 393/52 1996/1996 |
| `MenuTouchWindow::OnTouchBegin` | 29/15 1896/1896 | 1859/651 1944/1896 | 29/15 1896/1896 | 29/15 1896/1896 |
| `ModStationC1` | 43/12 1008/1008 | 668/45 1008/1008 | 43/12 1008/1008 | 695/57 1008/1008 |

Observations:

* The **header-only** profile is bit-for-bit neutral: `function_entry_count`
  alone does not move these bodies. The body changes come from the line
  samples (branch weights) and the resulting block layout / sinking.
* The **uniform all-lines** profile changes 15 function sizes and increases
  both metrics on every residual. It is not a win.
* The **single line-0** profile keeps all sizes exact but still worsens the
  aggregate raw (91,924 -> 93,287) and aligned (25,895 -> 26,043) sums.
* With the section-prefix pass left ON, the profiled functions are renamed
  `.text.hot.*` and moved to the front of `.text`; `byte_exact` collapses to
  1369 even though `linked_exact` stays 4507. `-mllvm
  -profile-guided-section-prefix=false` keeps the addresses but is still not
  a win (byte 1668, `size 4509`).
* **No residual becomes byte-exact under any sample-profile variant.** The
  closest residuals (`MenuTouchWindow::OnTouchBegin` raw 29, `MGame::OnUpdate`
  raw 37) are unchanged by the header-only profile and worse under every
  line-sampled profile.

For reference, the wave-154 IR-level flat `!prof` vector for
`Status::missionCompleted` (not a build flag or sample profile) still gives
raw 558 / aligned 47 at size 1432; it is the only known way to improve that
function, and it remains a source/IR-shape experiment, not a build-provenance
win.

## 5. Conclusion

* The original is a single `-Oz` build; no TU was compiled at `-O1/-O2/-Os/-O3`.
* The stock clang 7.0.2 `-Oz` `-mllvm` defaults are the original's; no knob
  improves the gate, and the six neutral knobs are consistent with defaults.
* A synthetic sample profile on all 17 residuals is a regression, with or
  without `-profile-guided-section-prefix=false`. The flat switch-weight
  signature remains unreplayed by a line-based sample profile.
* The wave-155 gate is unchanged: **4507/4524 byte, 4507/4524 linked,
  4524/4524 size, `.text` 86,044, `.ARM.extab` 1,292, raw sum 91,924,
  aligned sum 25,895 (23,897 over the 15 unique bodies)**.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10220
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6

# per-TU opt-level matrix (13 TUs x 5 levels, ~5 min)
python3 tools/per_tu_opt_sweep.py --build-root /dev/shm/w10220/pertu --jobs 13

# targeted -mllvm matrix (21 variants, parallel build dirs)
python3 /dev/shm/w10220/mllvm_parallel.py --scratch /dev/shm/w10220/mllvm --jobs 6 --par 4

# synthetic sample profile
python3 /dev/shm/w10220/gen_sample_profile.py          # writes all.prof
/dev/shm/w10220/sample_build.sh sample_prefixoff \
  '-fprofile-sample-use=/dev/shm/w10220/all.prof -mllvm -profile-guided-section-prefix=false'
python3 tools/fn_diff.py --ours /dev/shm/w10220/sample/sample_prefixoff/libgof2hdaa.so \
  _ZN6Status16missionCompletedEbbx _ZN5MGame8OnUpdateEv ... --json
```

Scratch (not committed): `/dev/shm/w10220/` — `pertu/`, `mllvm/`, `sample/`,
`all.prof`, `header.prof`, `line0.prof`, `unstripped.so`, `gen_sample_profile.py`,
`mllvm_parallel.py`, `sample_build.sh`.

## 7. Files touched

* `_work/reconstructions/build-flag-profile-replay-wave155-w10220.md` (this note).
* No source, tool, CMake, baseline, allowlist or doc change.
