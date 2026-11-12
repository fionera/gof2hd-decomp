# `Level::createMission` (`_ZN5Level13createMissionEv`) — wave 191 / w13526

Target `src/game/world/Level.cpp`, function VMA `0xaffd8`, size **17,172 B**
(`0x4314`). Branch `wave191b-w13526`, base master `9fdef1695`. Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, armeabi-v7a, image base `0x10000`).

Continues `level-createmission-wave191-w13512.md` / `...w13493.md`.

```text
                                     size            raw    aligned  insn
base 9fdef1695                       17172/17172     2661    863     6206/6197
landed                               17172/17172     2659    871     6206/6197
```

## 1. Result — landed

| metric | before | after |
|---|---:|---:|
| size | 17,172 | 17,172 (exact) |
| raw | 2,661 | **2,659** (-2) |
| aligned | 863 | 871 (+8) |
| insn | 6206/6197 | 6206/6197 |
| `.text` diff | 71,378 | **71,376** (-2) |
| `.ARM.extab` diff | 577 | 577 |
| `.rel.plt` diff | 0 | 0 |
| `.ARM.exidx` diff | 0 | 0 |

`check`: **RATCHET PASS** (byte_exact 4507/4524, linked_exact 4507/4524,
size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0).

## 2. The change: case-2 convoy loot `setRotationSpeed`/`setSpeed` order

The case-2 second (loot) loop had

```cpp
(*enemies)[convoyEnd + j]->setSpeed(0.0f);
(*enemies)[convoyEnd + j]->setRotationSpeed(0.0f);
```

Swapping the two independent setters

```cpp
(*enemies)[convoyEnd + j]->setRotationSpeed(0.0f);
(*enemies)[convoyEnd + j]->setSpeed(0.0f);
```

is semantically identical (both just store 0 into independent fields) and
moves two more `.text` bytes into agreement.  The original's emitted block is
`str r9,[r1,#80]` (the inlined `setSpeed(0)` field store) followed by the
`setRotationSpeed` virtual/PLT call; the swapped source lets clang schedule the
inlined field store ahead of the call in the same shape.

## 3. Residual / neutral cells

* The case-1 `enemyRace` loop-head reload (`ldr r5,[sp,#116]` at `0xb1608`)
  is still hoisted into the preheader (`0xb158c`).  Re-tested on this base:
  plain/`const`/local/pointer/array/struct/`__builtin_memcpy` spellings are
  all neutral (2,661); `volatile` remains size-fatal.
* Case-2 `asteroidPos` placement sweep (p1..p7) and the faithful
  `Route -> goods` order remain raw regressions; the current p7 placement is
  still the best raw shape.
* Case-4 tail faithful order (`hunters -> opt44 -> hunterCount -> new Array`)
  is 2,665 raw / 846 aligned (aligned win, raw loss).
* Adjacent-statement swap sweep over the case-2 block (lines 2277-2322)
  found only this -2 swap; the next best are neutral (2,661).

## 4. Reproduce

```sh
cd /home/debian/GalaxyOnFire2/wt/w13526
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
(ulimit -v 12000000; cmake --build cmake-build-match --target check -j2)
```

Expected: `raw=2659 aligned=871 size=17172/17172`, `.text` diff 71,376,
`.ARM.extab` diff 577, `.rel.plt` diff 0, `RATCHET PASS`.

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w13526/`
(`m.sh`, `diff.py`, `alignmap.py`, `sweep.py`, `Level.cpp.base`).
