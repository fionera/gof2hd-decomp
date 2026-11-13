# Per-TU non-optimization flag sweep on the 8 residual TUs (w13913)

Status: research note, 2026-11-13. Worktree `wt/w13913`, branch `wave192-w13913`,
base `9aca40eaa`. **No flag was landed: no non-optimization per-TU flag improves a
residual function while keeping `byte_exact` at 4508.** This commit contains only
this note.

## TL;DR

* Question: "we have no clue if the flags are correct". Answer: for the 8 TUs that
  own the residual functions, the canonical flag set in `AGENTS.md` is **validated
  against 19 non-optimization flag hypotheses** (160 cells = 8 TUs x (19 flags + base)).
* Baseline gate reproduced in every base cell: `byte_exact 4508/4524`,
  `linked_exact 4508/4524`, `size_exact 4524/4524`, `missing 0`, `extra 0`.
* **0 cells** beat 4508; **0 cells** keep 4508 *and* change the residual. Every
  residual improvement costs `byte_exact` (best case 4508 -> 4480).
* `-O1/-O2/-Os/-O3` per TU were already swept by `tools/per_tu_opt_sweep.py` and
  are not repeated here.

## Method

Per cell:

```sh
cmake -S . -B <scratch>/sweep/<tu-slug> \
  -DCMAKE_PROJECT_INCLUDE=tools/per_tu_flags.cmake \
  -DGOF2_AB_TU=<src/.../Tu.cpp> -DGOF2_AB_FLAGS=<flag>
cmake --build <build> --target link_so -j2
tools/fn_diff.py --ours <build>/libgof2hdaa.so <mangled...> --json
tools/verify/verify.py --build-dir <build>/verify --original <orig> --ours <so> ...
```

`per_tu_flags.cmake` appends the cell flag after the canonical set and after the
per-TU `COMPILE_OPTIONS` in `CMakeLists.txt`, so the cell flag wins. Builds ran in
`(ulimit -v 12000000; ...)` with `-j2`.

Raw data: `scratch/w13913/sweep_results.json` (160 records, authoritative),
`scratch/w13913/table.txt` (flat rendering), `scratch/w13913/sweep.py` (driver).

Note: only gate counts and the residual's `fn_diff` scores were recorded per cell;
per-cell `.text` / `.ARM.extab` sizes were not, so the section deltas are unchanged
from the landed tree (`.text` 76,882 / `.ARM.extab` 597 / `.ARM.exidx` 0 /
`.rodata` 0) because nothing was landed.

## Candidate flags (one per cell)

`-fno-strict-aliasing`, `-fwrapv`, `-fno-delete-null-pointer-checks`,
`-fno-sized-deallocation`, `-fno-threadsafe-statics`, `-fvisibility=hidden`,
`-fomit-frame-pointer`, `-fno-omit-frame-pointer`, `-mno-unaligned-access`,
`-fno-math-errno`, `-fno-exceptions`, `-fno-rtti`, `-fno-inline-functions`,
`-fno-vectorize`, `-fno-slp-vectorize`, `-fno-jump-tables`, `-fno-common`,
`-fno-builtin`, `-ffreestanding`.

## Results

### baseline
| TU | residual function(s) | raw / aligned / size |
|---|---|---|
| `src/game/gui/Hud.cpp` | `_ZN3Hud4drawExxP9PlayerEgobjj` | 19 / 6 / 9332 |
| `src/game/gui/Radar.cpp` | `_ZN5Radar4drawEP6PlayerP3Hudi` | 6909 / 1303 / 9908 |
| `src/game/menu/MGame.cpp` | `_ZN5MGame10OnTouchEndEiiPv`, `_ZN5MGame8OnUpdateEv` | 3544 / 697 / 8080, 16 / 6 / 11440 |
| `src/game/mission/LevelScript.cpp` | `_ZN11LevelScript7processEi` | 53208 / 18243 / 57232 |
| `src/game/mission/Status.cpp` | `_ZN6Status16missionCompletedEbbx` | 45 / 28 / 1432 |
| `src/game/ui/HangarWindow.cpp` | `_ZN12HangarWindow6renderEv` | 1395 / 396 / 9700 |
| `src/game/ui/Layout.cpp` | `_ZN6LayoutC1Ev`, `_ZN6LayoutC2Ev` | 5614 / 1993 / 7072, 5614 / 1993 / 7072 |
| `src/game/world/Level.cpp` | `_ZN5Level10createShipEiiiP8Waypointbb`, `_ZN5Level13createMissionEv` | 1190 / 366 / 2764, 4265 / 386 / 17172 |

### neutral
- `src/game/gui/Hud.cpp`: `-fno-strict-aliasing`, `-fwrapv`, `-fno-delete-null-pointer-checks`, `-fno-sized-deallocation`, `-fno-threadsafe-statics`, `-fvisibility=hidden`, `-fomit-frame-pointer`, `-fno-omit-frame-pointer`, `-mno-unaligned-access`, `-fno-math-errno`, `-fno-exceptions`, `-fno-rtti`, `-fno-inline-functions`, `-fno-vectorize`, `-fno-slp-vectorize`, `-fno-jump-tables`, `-fno-common`, `-fno-builtin`, `-ffreestanding`
- `src/game/gui/Radar.cpp`: `-fno-strict-aliasing`, `-fwrapv`, `-fno-delete-null-pointer-checks`, `-fno-sized-deallocation`, `-fno-threadsafe-statics`, `-fvisibility=hidden`, `-fomit-frame-pointer`, `-fno-omit-frame-pointer`, `-mno-unaligned-access`, `-fno-math-errno`, `-fno-exceptions`, `-fno-rtti`, `-fno-inline-functions`, `-fno-vectorize`, `-fno-slp-vectorize`, `-fno-jump-tables`, `-fno-common`, `-fno-builtin`, `-ffreestanding`
- `src/game/menu/MGame.cpp`: `-fno-delete-null-pointer-checks`, `-fno-sized-deallocation`, `-fno-threadsafe-statics`, `-fno-omit-frame-pointer`, `-fno-math-errno`, `-fno-vectorize`, `-fno-common`
- `src/game/mission/LevelScript.cpp`: `-fno-strict-aliasing`, `-fwrapv`, `-fno-delete-null-pointer-checks`, `-fno-sized-deallocation`, `-fno-threadsafe-statics`, `-fvisibility=hidden`, `-fomit-frame-pointer`, `-fno-omit-frame-pointer`, `-mno-unaligned-access`, `-fno-math-errno`, `-fno-exceptions`, `-fno-rtti`, `-fno-inline-functions`, `-fno-vectorize`, `-fno-slp-vectorize`, `-fno-jump-tables`, `-fno-common`, `-fno-builtin`, `-ffreestanding`
- `src/game/mission/Status.cpp`: `-fno-delete-null-pointer-checks`, `-fno-sized-deallocation`, `-fno-threadsafe-statics`, `-fno-omit-frame-pointer`, `-fno-math-errno`, `-fno-rtti`, `-fno-vectorize`, `-fno-slp-vectorize`, `-fno-common`
- `src/game/ui/HangarWindow.cpp`: `-fno-delete-null-pointer-checks`, `-fno-sized-deallocation`, `-fno-threadsafe-statics`, `-fno-omit-frame-pointer`, `-fno-math-errno`, `-fno-rtti`, `-fno-vectorize`, `-fno-slp-vectorize`, `-fno-common`
- `src/game/ui/Layout.cpp`: `-fno-strict-aliasing`, `-fwrapv`, `-fno-delete-null-pointer-checks`, `-fno-sized-deallocation`, `-fno-threadsafe-statics`, `-fno-omit-frame-pointer`, `-fno-math-errno`, `-fno-rtti`, `-fno-vectorize`, `-fno-common`
- `src/game/world/Level.cpp`: `-fno-delete-null-pointer-checks`, `-fno-sized-deallocation`, `-fno-threadsafe-statics`, `-fno-omit-frame-pointer`, `-fno-math-errno`, `-fno-rtti`, `-fno-vectorize`, `-fno-common`

### non-neutral
| TU | flag | residual raw / aligned / size (delta vs base) | byte_exact |
|---|---|---|---|
| `src/game/menu/MGame.cpp` | `-fno-strict-aliasing` | 7012 / 995 / 8080 (+3468/+298/+0); 9525 / 688 / 11440 (+9509/+682/+0) | 3104 |
| `src/game/menu/MGame.cpp` | `-fwrapv` | 6644 / 1326 / 8080 (+3100/+629/+0); 702 / 127 / 11440 (+686/+121/+0) | 3107 |
| `src/game/menu/MGame.cpp` | `-fvisibility=hidden` | MISSING; MISSING | 1670 |
| `src/game/menu/MGame.cpp` | `-fomit-frame-pointer` | 7709 / 2695 / 8080 (+4165/+1998/+0); 10632 / 2158 / 11440 (+10616/+2152/+0) | 3112 |
| `src/game/menu/MGame.cpp` | `-mno-unaligned-access` | 3826 / 717 / 8080 (+282/+20/+0); 762 / 116 / 11440 (+746/+110/+0) | 3094 |
| `src/game/menu/MGame.cpp` | `-fno-exceptions` | 7589 / 2261 / 8080 (+4045/+1564/+0); 9234 / 1876 / 11440 (+9218/+1870/+0) | 1369 |
| `src/game/menu/MGame.cpp` | `-fno-rtti` | 3586 / 714 / 8080 (+42/+17/+0); 152 / 106 / 11440 (+136/+100/+0) | 2929 |
| `src/game/menu/MGame.cpp` | `-fno-inline-functions` | 7849 / 2208 / 8080 (+4305/+1511/+0); 10399 / 1097 / 11440 (+10383/+1091/+0) | 1359 |
| `src/game/menu/MGame.cpp` | `-fno-slp-vectorize` | 3844 / 716 / 8080 (+300/+19/+0); 10504 / 145 / 11440 (+10488/+139/+0) | 4277 |
| `src/game/menu/MGame.cpp` | `-fno-jump-tables` | 3551 / 700 / 8080 (+7/+3/+0); 702 / 127 / 11440 (+686/+121/+0) | 3108 |
| `src/game/menu/MGame.cpp` | `-fno-builtin` | 4233 / 727 / 8080 (+689/+30/+0); 742 / 116 / 11440 (+726/+110/+0) | 3125 |
| `src/game/menu/MGame.cpp` | `-ffreestanding` | 4233 / 727 / 8080 (+689/+30/+0); 742 / 116 / 11440 (+726/+110/+0) | 3125 |
| `src/game/mission/Status.cpp` | `-fno-strict-aliasing` | 119 / 32 / 1432 (+74/+4/+0) | 2315 |
| `src/game/mission/Status.cpp` | `-fwrapv` | 45 / 28 / 1432 (+0/+0/+0) | 4480 |
| `src/game/mission/Status.cpp` | `-fvisibility=hidden` | MISSING | 1385 |
| `src/game/mission/Status.cpp` | `-fomit-frame-pointer` | 1377 / 479 / 1432 (+1332/+451/+0) | 2415 |
| `src/game/mission/Status.cpp` | `-mno-unaligned-access` | 45 / 28 / 1432 (+0/+0/+0) | 2339 |
| `src/game/mission/Status.cpp` | `-fno-exceptions` | 217 / 32 / 1432 (+172/+4/+0) | 1498 |
| `src/game/mission/Status.cpp` | `-fno-inline-functions` | 1442 / 532 / 1432 (+1397/+504/+0) | 1375 |
| `src/game/mission/Status.cpp` | `-fno-jump-tables` | 1312 / 370 / 1432 (+1267/+342/+0) | 1840 |
| `src/game/mission/Status.cpp` | `-fno-builtin` | 126 / 32 / 1432 (+81/+4/+0) | 2296 |
| `src/game/mission/Status.cpp` | `-ffreestanding` | 126 / 32 / 1432 (+81/+4/+0) | 2296 |
| `src/game/ui/HangarWindow.cpp` | `-fno-strict-aliasing` | 1866 / 328 / 9700 (+471/-68/+0) | 2772 |
| `src/game/ui/HangarWindow.cpp` | `-fwrapv` | 1867 / 482 / 9700 (+472/+86/+0) | 1387 |
| `src/game/ui/HangarWindow.cpp` | `-fvisibility=hidden` | MISSING | 1575 |
| `src/game/ui/HangarWindow.cpp` | `-fomit-frame-pointer` | 9320 / 2192 / 9700 (+7925/+1796/+0) | 2759 |
| `src/game/ui/HangarWindow.cpp` | `-mno-unaligned-access` | 1869 / 484 / 9700 (+474/+88/+0) | 2768 |
| `src/game/ui/HangarWindow.cpp` | `-fno-exceptions` | 9271 / 4797 / 9700 (+7876/+4401/+0) | 2058 |
| `src/game/ui/HangarWindow.cpp` | `-fno-inline-functions` | 9430 / 2292 / 9700 (+8035/+1896/+0) | 1443 |
| `src/game/ui/HangarWindow.cpp` | `-fno-jump-tables` | 3741 / 540 / 9700 (+2346/+144/+0) | 2774 |
| `src/game/ui/HangarWindow.cpp` | `-fno-builtin` | 1868 / 484 / 9700 (+473/+88/+0) | 3733 |
| `src/game/ui/HangarWindow.cpp` | `-ffreestanding` | 1868 / 484 / 9700 (+473/+88/+0) | 3733 |
| `src/game/ui/Layout.cpp` | `-fvisibility=hidden` | MISSING; MISSING | 1506 |
| `src/game/ui/Layout.cpp` | `-fomit-frame-pointer` | 5899 / 2291 / 7072 (+285/+298/+0); 5899 / 2291 / 7072 (+285/+298/+0) | 2447 |
| `src/game/ui/Layout.cpp` | `-mno-unaligned-access` | 5614 / 1993 / 7072 (+0/+0/+0); 5614 / 1993 / 7072 (+0/+0/+0) | 2458 |
| `src/game/ui/Layout.cpp` | `-fno-exceptions` | 5615 / 1997 / 7072 (+1/+4/+0); 5615 / 1997 / 7072 (+1/+4/+0) | 1369 |
| `src/game/ui/Layout.cpp` | `-fno-inline-functions` | 5614 / 1997 / 7072 (+0/+4/+0); 5614 / 1997 / 7072 (+0/+4/+0) | 1369 |
| `src/game/ui/Layout.cpp` | `-fno-slp-vectorize` | 6885 / 4122 / 7072 (+1271/+2129/+0); 6885 / 4122 / 7072 (+1271/+2129/+0) | 2447 |
| `src/game/ui/Layout.cpp` | `-fno-jump-tables` | 5614 / 1993 / 7072 (+0/+0/+0); 5614 / 1993 / 7072 (+0/+0/+0) | 2471 |
| `src/game/ui/Layout.cpp` | `-fno-builtin` | 5615 / 1997 / 7072 (+1/+4/+0); 5615 / 1997 / 7072 (+1/+4/+0) | 1378 |
| `src/game/ui/Layout.cpp` | `-ffreestanding` | 5615 / 1997 / 7072 (+1/+4/+0); 5615 / 1997 / 7072 (+1/+4/+0) | 1378 |
| `src/game/world/Level.cpp` | `-fno-strict-aliasing` | 1251 / 370 / 2764 (+61/+4/+0); 16682 / 3372 / 17172 (+12417/+2986/+0) | 2352 |
| `src/game/world/Level.cpp` | `-fwrapv` | 1229 / 413 / 2764 (+39/+47/+0); 4884 / 448 / 17172 (+619/+62/+0) | 2359 |
| `src/game/world/Level.cpp` | `-fvisibility=hidden` | MISSING; MISSING | 1320 |
| `src/game/world/Level.cpp` | `-fomit-frame-pointer` | 2497 / 604 / 2764 (+1307/+238/+0); 16343 / 3512 / 17172 (+12078/+3126/+0) | 2458 |
| `src/game/world/Level.cpp` | `-mno-unaligned-access` | 1232 / 370 / 2764 (+42/+4/+0); 13803 / 778 / 17172 (+9538/+392/+0) | 2347 |
| `src/game/world/Level.cpp` | `-fno-exceptions` | 2491 / 1433 / 2764 (+1301/+1067/+0); 16634 / 4380 / 17172 (+12369/+3994/+0) | 1369 |
| `src/game/world/Level.cpp` | `-fno-inline-functions` | 2263 / 621 / 2764 (+1073/+255/+0); 17833 / 5847 / 17172 (+13568/+5461/+0) | 1486 |
| `src/game/world/Level.cpp` | `-fno-slp-vectorize` | 1240 / 439 / 2764 (+50/+73/+0); 13424 / 956 / 17172 (+9159/+570/+0) | 2359 |
| `src/game/world/Level.cpp` | `-fno-jump-tables` | 2187 / 1090 / 2764 (+997/+724/+0); 16715 / 3371 / 17172 (+12450/+2985/+0) | 1369 |
| `src/game/world/Level.cpp` | `-fno-builtin` | 1264 / 370 / 2764 (+74/+4/+0); 13725 / 1044 / 17172 (+9460/+658/+0) | 2347 |
| `src/game/world/Level.cpp` | `-ffreestanding` | 1264 / 370 / 2764 (+74/+4/+0); 13725 / 1044 / 17172 (+9460/+658/+0) | 2347 |

## Interpretation

* `-fomit-frame-pointer` is always harmful and `-fno-omit-frame-pointer` is always
  neutral, so the canonical build already keeps frame pointers on these TUs
  (consistent with `-fstack-protector-strong` at `-Oz`).
* `-mno-unaligned-access` is harmful wherever it is not neutral, so canonical
  ARMv7 unaligned access is right.
* `-fno-strict-aliasing` and `-fwrapv` are harmful wherever they are not neutral,
  so canonical strict aliasing / no-wrap semantics are right.
* `-fno-exceptions`, `-fno-rtti`, `-fno-inline-functions`, `-fno-jump-tables`,
  `-fno-builtin` / `-ffreestanding` and `-fno-slp-vectorize` are harmful wherever
  they are not neutral, so canonical exceptions/RTTI/inlining/jump tables/builtins/
  SLP are right.
* `-fvisibility=hidden` drops residuals out of the dynamic symbol table
  (`fn_diff` MISSING) in Layout/Level/MGame/HangarWindow/Status; it is neutral in
  Hud/Radar/LevelScript only because those functions stay visible. It is a loss
  everywhere except where it is a no-op.
* `-fno-common` is neutral per TU on all 8 TUs, but prior research
  (`untested_compile_flags_w3695.md`) showed a global `-fno-common` fails to link
  (`multiple definition of g_android_back_button_pressed`), so it is not usable.
* Hud, Radar and LevelScript are insensitive to **all 19** flags: their residual
  is source shape, not a flag difference.

Near misses that are still losses (residual improved, gate broken):
`HangarWindow -fno-strict-aliasing` aligned 396 -> 328 (`byte_exact` 2772);
`Status -fno-strict-aliasing` raw 45 -> 119 (`byte_exact` 2315);
`MGame -fno-rtti` OnTouchEnd raw +42 (`byte_exact` 2929);
`Level -fwrapv` createMission raw +619 (`byte_exact` 2359).

## Conclusion

The canonical non-optimization flag set is consistent with the original binary on
every residual TU. The remaining 14 residual rows are source/TU-shape problems, not
missing compile flags. Nothing to land; `tools/verify_baseline.json` untouched.
