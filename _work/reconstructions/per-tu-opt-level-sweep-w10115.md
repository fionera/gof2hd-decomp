# Per-TU optimization-level sweep (w10115)

Status: research note, 2026-11-08. Worktree `wt/w10115`, base `f4e0ea40b`
(wave 154). No `src/` change and no build-flag change is landed: **every one of
the 17 residual functions is best at `-Oz` on both metrics, and no single-TU
override makes any residual function byte-exact.** This commit adds the
reproducible harness (`tools/per_tu_flags.cmake`, `tools/per_tu_opt_sweep.py`,
`tools/fn_diff.py`) and this note.

## TL;DR

* Canonical gate at this tree (measured before and after the sweep):
  **byte_exact 4507/4524, linked_exact 4507/4524, size-exact 4524/4524,
  imports 207, lint clean, drift 0**, `.text` = 431,168 B (0x69440).
* Hypothesis: some TUs were compiled at a level other than `-Oz` (engine at
  `-O2`, a few files at `-Os`, ...). **Disproved.** For all 13 TUs that own a
  residual function, `-Oz` gives the lowest raw byte diff *and* the lowest
  aligned diff for every residual function; `-O1/-O2/-Os/-O3` are all worse on
  both metrics.
* The closest miss is `ModStation::ModStation` at `-Os`: it keeps the original
  1008 B size but raw diff jumps 43 -> 919 (aligned 12 -> 42). Size equality
  alone does not indicate a matching level.
* The four fully exact control TUs also break at every non-`-Oz` level, so the
  result is not specific to the residual set.
* The full gate collapses for a single-TU override (e.g. 4507 -> ~1350
  `byte_exact`) because the TU's function sizes change, the `.text` base of
  every *later* TU shifts, and every `bl`/literal-pool address into the shifted
  range changes. This is an address-shift artefact, not a code-quality signal;
  the residual-function metrics above are the meaningful comparison. No
  override is landable (`.text` size changes and `byte_exact` falls).

## 1. Baseline

```text
$ cmake --build cmake-build-match --target check -j64
verify: byte 4507  linked 4507  stubs 0  missing 0  extra 0  avg 99.93
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`size-exact` from `report.json` is 4524/4524 and `.text` is 431,168 B. The 17
residual functions are the wave-154 set (all size-exact).

## 2. Method

`GOF2_MATCH_OPT` is a global cache variable, so a per-TU override needs a
source-file property. `tools/per_tu_flags.cmake` is passed as
`CMAKE_PROJECT_INCLUDE` (processed after `project()`, before generation) and
sets `COMPILE_FLAGS` on one TU:

```cmake
set_source_files_properties("${CMAKE_SOURCE_DIR}/${GOF2_AB_TU}"
    PROPERTIES COMPILE_FLAGS "${GOF2_AB_FLAGS}")
```

`COMPILE_FLAGS` is appended after the canonical global flags *and* after any
`COMPILE_OPTIONS`, so the tested `-O` level overrides the canonical `-Oz`.
Verified in `compile_commands.json` (the command ends `... -O2 -o ...Layout.cpp.obj`).

`tools/per_tu_opt_sweep.py` drives the matrix. Each (TU, level) cell gets its
own scratch build directory under `--build-root`, so cells are independent and
run in parallel:

```sh
python3 tools/per_tu_opt_sweep.py \
  --build-root /dev/shm/w10115/build --jobs 17 \
  --controls src/engine/render/PaintCanvas.cpp src/game/ship/PlayerEgo.cpp \
             src/game/world/StarMap.cpp src/engine/render/shaders/GenericShader.cpp
```

Per cell it records:

* `tools/fn_diff.py` raw same-address byte diff and instruction-index aligned
  diff (the two metrics requested; identical definitions to `/dev/shm/fdiff.py`
  and `_work/reconstructions/align_diff.py`),
* the full `tools/verify/verify.py` gate (`byte_exact`, `linked_exact`,
  `size_exact`),
* the linked `.text` size.

The `-Oz` cell in every scratch directory reproduces the canonical numbers
exactly (gate 4507/4507, `.text` 431,168, and the same residual raw/aligned
values), which validates the harness.

## 3. Residual-function matrix

`raw / aligned / size` for each residual function. Lower is better; the `-Oz`
column is the canonical tree.

### 3.1 Residual-function table (raw / aligned / size)

| TU | function | -Oz | -O1 | -O2 | -Os | -O3 |
|---|---|---|---|---|---|---|
| `MGame.cpp` | `_ZN5MGame10OnTouchEndEiiPv` | 4391/903/8080 | 8081/3103/8448 | 9402/3766/9956 | 7828/2521/8288 | 9768/3865/10152 |
| `MGame.cpp` | `_ZN5MGame8OnUpdateEv` | 37/109/11440 | 11493/4707/12020 | 14025/5638/14324 | 10987/1884/11504 | 14051/5680/14420 |
| `ModStation.cpp` | `_ZN10ModStationC1Ev` | 43/12/1008 | 957/290/908 | 946/44/1016 | 919/42/1008 | 1048/98/1088 |
| `ModStation.cpp` | `_ZN10ModStationC2Ev` | 43/12/1008 | 957/290/908 | 946/44/1016 | 919/42/1008 | 1048/98/1088 |
| `SpaceLounge.cpp` | `_ZN11SpaceLounge10drawLoungeEv` | 175/41/1948 | 1965/453/2056 | 1861/224/1976 | 1858/236/1972 | 1899/230/2004 |
| `Generator.cpp` | `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | 393/52/1996 | 2127/857/2224 | 2083/903/2164 | 1974/596/2080 | 2408/960/2464 |
| `Status.cpp` | `_ZN6Status16missionCompletedEbbx` | 722/320/1432 | 1877/855/1940 | 1692/778/1760 | 1328/628/1456 | 1764/979/1840 |
| `HangarWindow.cpp` | `_ZN12HangarWindow6renderEv` | 1965/291/9700 | 9608/4925/9940 | 9569/2214/9904 | 9439/2124/9816 | 9871/5316/10180 |
| `Hud.cpp` | `_ZN3Hud4drawExxP9PlayerEgobjj` | 1325/249/9332 | 8874/1943/9452 | 9475/1998/9740 | 9123/1666/9528 | 9659/2321/9940 |
| `Layout.cpp` | `_ZN6LayoutC1Ev` | 5849/2006/7072 | 7850/4887/8044 | 6134/2185/7136 | 5891/2156/7080 | 6495/3763/7292 |
| `Layout.cpp` | `_ZN6LayoutC2Ev` | 5849/2006/7072 | 7850/4887/8044 | 6134/2185/7136 | 5891/2156/7080 | 6495/3763/7292 |
| `MenuTouchWindow.cpp` | `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | 29/15/1896 | 2084/689/2184 | 1876/737/2000 | 1873/736/2000 | 1939/743/2016 |
| `Gun.cpp` | `_ZN3Gun22calcCharacterCollisionEv` | 463/57/1984 | 1911/549/2000 | 1927/346/2032 | 1879/322/1984 | 1932/349/2040 |
| `Radar.cpp` | `_ZN5Radar4drawEP6PlayerP3Hudi` | 8577/1484/9908 | 10191/5077/10480 | 10084/4534/10388 | 9951/4517/10268 | 10482/4030/10820 |
| `Level.cpp` | `_ZN5Level10createShipEiiiP8Waypointbb` | 1564/452/2764 | 2979/1613/3060 | 2951/1095/3064 | 2876/1066/2988 | 2968/1097/3080 |
| `Level.cpp` | `_ZN5Level13createMissionEv` | 5487/774/17172 | 18220/8827/18784 | 18778/10645/19312 | 17565/6032/18176 | 19763/7838/20288 |
| `LevelScript.cpp` | `_ZN11LevelScript7processEi` | 55327/17176/57232 | 59828/37291/61576 | 61709/27770/63416 | 55943/25073/57724 | 64856/28972/66464 |

`-Oz` is the minimum of both metrics for all 17 rows. The only row where a
non-`-Oz` level is even close is `Layout::Layout` at `-Os` (raw 5891 vs 5849,
aligned 2156 vs 2006) and `ModStation::ModStation` at `-Os` (raw 919 vs 43).

### 3.2 Gate and `.text` per TU x level

The gate is the full 4524-function comparison. `-Oz` is canonical.

| TU | level | byte_exact | linked_exact | size_exact | .text |
|---|---|---:|---:|---:|---:|
| `PaintCanvas.cpp` (control) | -Oz | 4507 | 4507 | 4524 | 431168 |
| `PaintCanvas.cpp` (control) | -O1 | 1339 | 4332 | 4404 | 432448 |
| `PaintCanvas.cpp` (control) | -O2 | 1345 | 4367 | 4394 | 429632 |
| `PaintCanvas.cpp` (control) | -Os | 1352 | 4395 | 4426 | 429792 |
| `PaintCanvas.cpp` (control) | -O3 | 1344 | 4366 | 4392 | 429536 |
| `GenericShader.cpp` (control) | -Oz | 4507 | 4507 | 4524 | 431168 |
| `GenericShader.cpp` (control) | -O1 | 2389 | 4502 | 4522 | 431168 |
| `GenericShader.cpp` (control) | -O2 | 2389 | 4502 | 4522 | 431168 |
| `GenericShader.cpp` (control) | -Os | 2389 | 4503 | 4522 | 431168 |
| `GenericShader.cpp` (control) | -O3 | 1941 | 4502 | 4520 | 431168 |
| `MGame.cpp` | -Oz | 4507 | 4507 | 4524 | 431168 |
| `MGame.cpp` | -O1 | 1355 | 4475 | 4494 | 432352 |
| `MGame.cpp` | -O2 | 1355 | 4477 | 4495 | 431040 |
| `MGame.cpp` | -Os | 2201 | 4485 | 4507 | 431136 |
| `MGame.cpp` | -O3 | 1355 | 4476 | 4494 | 431040 |
| `ModStation.cpp` | -Oz | 4507 | 4507 | 4524 | 431168 |
| `ModStation.cpp` | -O1 | 1356 | 4478 | 4499 | 432992 |
| `ModStation.cpp` | -O2 | 1356 | 4484 | 4501 | 431008 |
| `ModStation.cpp` | -Os | 1369 | 4490 | 4509 | 431104 |
| `ModStation.cpp` | -O3 | 1439 | 4484 | 4500 | 431008 |
| `SpaceLounge.cpp` | -Oz | 4507 | 4507 | 4524 | 431168 |
| `SpaceLounge.cpp` | -O1 | 1412 | 4486 | 4504 | 432672 |
| `SpaceLounge.cpp` | -O2 | 1369 | 4488 | 4504 | 430880 |
| `SpaceLounge.cpp` | -Os | 1369 | 4489 | 4506 | 431040 |
| `SpaceLounge.cpp` | -O3 | 1369 | 4488 | 4504 | 430880 |
| `Generator.cpp` | -Oz | 4507 | 4507 | 4524 | 431168 |
| `Generator.cpp` | -O1 | 1369 | 4490 | 4516 | 432512 |
| `Generator.cpp` | -O2 | 1668 | 4499 | 4516 | 431168 |
| `Generator.cpp` | -Os | 1668 | 4499 | 4516 | 431168 |
| `Generator.cpp` | -O3 | 1659 | 4500 | 4516 | 431168 |
| `Status.cpp` | -Oz | 4507 | 4507 | 4524 | 431168 |
| `Status.cpp` | -O1 | 1346 | 4438 | 4472 | 432704 |
| `Status.cpp` | -O2 | 1351 | 4459 | 4478 | 430464 |
| `Status.cpp` | -Os | 1365 | 4461 | 4480 | 430880 |
| `Status.cpp` | -O3 | 1351 | 4459 | 4477 | 430464 |
| `PlayerEgo.cpp` (control) | -Oz | 4507 | 4507 | 4524 | 431168 |
| `PlayerEgo.cpp` (control) | -O1 | 1337 | 4415 | 4461 | 432576 |
| `PlayerEgo.cpp` (control) | -O2 | 1343 | 4431 | 4456 | 430816 |
| `PlayerEgo.cpp` (control) | -Os | 1356 | 4455 | 4478 | 430944 |
| `PlayerEgo.cpp` (control) | -O3 | 1343 | 4429 | 4454 | 430816 |
| `HangarWindow.cpp` | -Oz | 4507 | 4507 | 4524 | 431168 |
| `HangarWindow.cpp` | -O1 | 1365 | 4483 | 4505 | 432224 |
| `HangarWindow.cpp` | -O2 | 1352 | 4487 | 4505 | 431072 |
| `HangarWindow.cpp` | -Os | 1369 | 4491 | 4507 | 431104 |
| `HangarWindow.cpp` | -O3 | 1352 | 4487 | 4503 | 431040 |
| `Hud.cpp` | -Oz | 4507 | 4507 | 4524 | 431168 |
| `Hud.cpp` | -O1 | 1375 | 4477 | 4498 | 431744 |
| `Hud.cpp` | -O2 | 1353 | 4477 | 4497 | 430944 |
| `Hud.cpp` | -Os | 1354 | 4479 | 4498 | 431104 |
| `Hud.cpp` | -O3 | 1353 | 4477 | 4494 | 430944 |
| `Layout.cpp` | -Oz | 4507 | 4507 | 4524 | 431168 |
| `Layout.cpp` | -O1 | 1369 | 4470 | 4500 | 431328 |
| `Layout.cpp` | -O2 | 1484 | 4480 | 4498 | 431104 |
| `Layout.cpp` | -Os | 1378 | 4486 | 4505 | 431136 |
| `Layout.cpp` | -O3 | 1369 | 4478 | 4497 | 431104 |
| `MenuTouchWindow.cpp` | -Oz | 4507 | 4507 | 4524 | 431168 |
| `MenuTouchWindow.cpp` | -O1 | 1377 | 4472 | 4495 | 432160 |
| `MenuTouchWindow.cpp` | -O2 | 1367 | 4481 | 4497 | 430688 |
| `MenuTouchWindow.cpp` | -Os | 1369 | 4482 | 4499 | 430752 |
| `MenuTouchWindow.cpp` | -O3 | 1367 | 4480 | 4496 | 430688 |
| `Gun.cpp` | -Oz | 4507 | 4507 | 4524 | 431168 |
| `Gun.cpp` | -O1 | 1369 | 4493 | 4511 | 431680 |
| `Gun.cpp` | -O2 | 1377 | 4494 | 4511 | 431168 |
| `Gun.cpp` | -Os | 1365 | 4494 | 4512 | 431168 |
| `Gun.cpp` | -O3 | 1356 | 4494 | 4510 | 431168 |
| `Radar.cpp` | -Oz | 4507 | 4507 | 4524 | 431168 |
| `Radar.cpp` | -O1 | 1368 | 4498 | 4516 | 432256 |
| `Radar.cpp` | -O2 | 1377 | 4498 | 4514 | 431136 |
| `Radar.cpp` | -Os | 1369 | 4500 | 4516 | 431168 |
| `Radar.cpp` | -O3 | 1377 | 4498 | 4514 | 431136 |
| `Level.cpp` | -Oz | 4507 | 4507 | 4524 | 431168 |
| `Level.cpp` | -O1 | 1363 | 4433 | 4466 | 433600 |
| `Level.cpp` | -O2 | 1351 | 4448 | 4465 | 430400 |
| `Level.cpp` | -Os | 1353 | 4455 | 4474 | 430720 |
| `Level.cpp` | -O3 | 1351 | 4448 | 4465 | 430400 |
| `LevelScript.cpp` | -Oz | 4507 | 4507 | 4524 | 431168 |
| `LevelScript.cpp` | -O1 | 1453 | 4499 | 4515 | 432032 |
| `LevelScript.cpp` | -O2 | 1369 | 4500 | 4516 | 431104 |
| `LevelScript.cpp` | -Os | 1378 | 4501 | 4517 | 431104 |
| `LevelScript.cpp` | -O3 | 1369 | 4499 | 4515 | 431104 |
| `StarMap.cpp` (control) | -Oz | 4507 | 4507 | 4524 | 431168 |
| `StarMap.cpp` (control) | -O1 | 1369 | 4486 | 4508 | 432448 |
| `StarMap.cpp` (control) | -O2 | 1369 | 4491 | 4508 | 431104 |
| `StarMap.cpp` (control) | -Os | 1377 | 4491 | 4509 | 431104 |
| `StarMap.cpp` (control) | -O3 | 1377 | 4491 | 4508 | 431104 |

## 4. Why the gate collapses

`verify.py`'s `byte_exact` compares the raw bytes of each function at its own
address. A single-TU opt-level change alters that TU's function sizes
(`size_exact` falls by 5-120), which shifts the `.text` base of every later TU.
Every `bl` to a shifted callee and every literal-pool word that names a shifted
address then differs, so thousands of *otherwise exact* functions lose
`byte_exact`. `linked_exact` is less sensitive because it resolves cross-function
calls by symbol name, but it still drops (4470-4501) because intra-function
branch offsets and pool contents change.

This is why the residual-function raw/aligned columns are the meaningful
comparison for the hypothesis, and why no override is landable: even the best
cell (`Generator.cpp` at `-O2`/`-Os`, `.text` unchanged at 431,168) drops the
gate to 1668/4499 and makes the residual function much worse (393 -> 2083/1974).

## 5. Conclusion

* **No TU was compiled at a different optimization level.** `-Oz` is optimal on
  both raw bytes and aligned instructions for all 17 residual functions, and
  the four exact control TUs also require `-Oz`.
* **No single-TU override makes any residual function byte-exact.** The best
  non-`-Oz` residual raw diff is `Layout::Layout` at `-Os` (5891 vs 5849 at
  `-Oz`); every other cell is far worse.
* The residual gap is therefore not a per-TU opt-level difference. It remains
  source-shape/TU-shape work, consistent with the earlier global flag sweeps
  (`untested_compile_flags_w3695.md`, `worst_function_flag_sensitivity_w3751.md`)
  and the toolchain provenance note (`toolchain-runtime-provenance-wave120-w7007.md`).
* Nothing is landed in `src/`; the gate is unchanged at 4507/4507/4524.

## 6. Reproduction

```sh
# canonical gate
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j64

# full per-TU matrix (17 TUs x 5 levels; ~4 min on 17 parallel workers)
python3 tools/per_tu_opt_sweep.py \
  --build-root /dev/shm/w10115/build --jobs 17 \
  --controls src/engine/render/PaintCanvas.cpp src/game/ship/PlayerEgo.cpp \
             src/game/world/StarMap.cpp src/engine/render/shaders/GenericShader.cpp

# one cell by hand
cmake -S . -B /dev/shm/w10115/build/one -G Ninja \
  -DCMAKE_PROJECT_INCLUDE=tools/per_tu_flags.cmake \
  -DGOF2_AB_TU=src/game/ui/Layout.cpp -DGOF2_AB_FLAGS=-O2 \
  -DGOF2_ORIGINAL_SO=... -DGOF2_FMOD_LIBDIR=...
cmake --build /dev/shm/w10115/build/one --target link_so -j8
python3 tools/fn_diff.py --ours /dev/shm/w10115/build/one/libgof2hdaa.so _ZN6LayoutC1Ev
```

No `--update-baseline` was run; `tools/verify_baseline.json` and
`tools/sodiff_allow.txt` are untouched.
