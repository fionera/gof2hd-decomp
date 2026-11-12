# `Level::createMission` (`_ZN5Level13createMissionEv`) — wave 191 / w13512

Target `src/game/world/Level.cpp`, function VMA `0xaffd8`, size **17,172 B**
(`0x4314`). Branch `wave191b-w13512`, base master `d71a1eece`. Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, armeabi-v7a, image base `0x10000`).

Continues `level-createmission-wave191-w13502.md` / `...w13502b.md`
(case-2 convoy loot order + non-volatile `offset`, case-2 escort call order).

```text
                                     size            raw    aligned  insn
base d71a1eece                       17172/17172     2669    865     6206/6197
landed                               17172/17172     2661    863     6206/6197
```

## 1. Result — landed

| metric | before | after |
|---|---:|---:|
| size | 17,172 | 17,172 (exact) |
| raw | 2,669 | **2,661** (-8) |
| aligned | 865 | **863** (-2) |
| insn | 6206/6197 | 6206/6197 |
| `.text` diff | 71,478 | **71,470** (-8) |
| `.ARM.extab` diff | 577 | 577 |
| `.rodata` sh_size | 269,644 | 269,644 |
| build-id diff | 20 | 20 |

`.rel.plt` byte-identical, 27/27 headers, `.ARM.exidx` 0, file size 2,187,328
exact. `check`: **RATCHET PASS** (byte_exact 4507/4524, linked_exact
4507/4524, size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0).

## 2. The change: case-2 escort `offset` update is the loop latch

The original's case-2 convoy escort loop (`0xb1792..0xb1888`) ends with

```
0xb187a  add.w r9, r9, #2000    ; offset += 2000
0xb187e  str.w r9, [sp, #120]   ; spill the new offset
0xb1882  add.w r8, r8, #1       ; i++
0xb1886  cmp   r8, fp
0xb1888  blt.n 0xb1792
```

i.e. the offset store is emitted *before* the loop-counter increment.  The
source had `offset += 2000;` as the last statement of the body, which clang
scheduled *after* the `i++` (`add.w r8,r8,#1; str.w r9,[sp,#120]`).  Moving
the update into the `for` increment — `for (int i = 0; i < convoyEnd;
i++, offset += 2000)` and deleting the body statement — restores the
original latch order.  Semantics are identical (the increment expression
runs at the end of every iteration, exactly where the body statement ran).

## 3. Measurements (all raw/aligned; size 17,172 unless noted)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base | 2,669 | 865 | 17,172 |
| **offset in for-increment (landed)** | **2,661** | **863** | **17,172** |
| drop `enemies` local in loot loop (E2) | 10,000 | 889 | 17,180 |
| `KIPlayer *ship` local for all loot stores (E3) | 15,400 | 1,150 | 17,164 |
| drop `ship` local in escort loop (E4) | 2,669 | 865 | 17,172 |

## 4. Residual / neutral cells (do not repeat)

* Case-183 offset switch (`k` 0..3): the original emits the case-1 block
  before the case-2 block (`0xb142c` = case 1, `0xb1438` = case 2) and the
  literal pool follows that block order (`-7000, 10000, -30000, 25000,
  -36000`).  Ours emits case 2 first (`25000, -36000, -7000, 10000,
  -30000`).  Moving `case 2` before `default` (SW) is raw **2,670** (+1);
  `case 2` before `case 1` (SW2) and `case 0,2,1,default` are also 2,670.
  The 5-byte run at `0xb1430..0xb1435` is the case-2 `offset[1]=0` store
  (`str.w sl`) versus the original's `ldr; str` pair; the block order is a
  MachineBlockPlacement choice, not source order.
* Case-1 `enemyRace` loop-head reload: the original reloads
  `ldr r5,[sp,#116]` inside the loop (`0xb1608`); ours hoists it to the
  preheader (`0xb158c`).  `%2700` is an SSA phi, so this is a spill/reload
  regalloc coin-flip, not a source-level load.  Plain/`const`/local
  spellings are neutral; `volatile` is size-fatal (17,232 B).  The 2-byte
  shift propagates through the case-1 loops and re-syncs by `0xb16d4`.
* Case-9 `setPosition` order: the original's five calls are in ascending
  index order (`+0,+1,+2,+3,+4`); reordering the source to that order (P1)
  matches the instruction stream exactly but the literal pool sits 12 B
  earlier in ours (accumulated drift from the case-1/pirate regions), so
  the `ldr` offsets differ: raw **2,675** (+6), aligned 838.  Not landable
  until the upstream drift is fixed.
* Case-4 tail: the original order is `hunters` (getDifficulty) ->
  `opt44` -> `hunterCount` -> `new Array` (T4b), which reproduces the
  instruction stream exactly, but `this` is in `r8` (original) vs `sl`
  (ours) and `hunterCount` spills to `[sp,#124]` vs `[sp,#116]`:
  raw **2,673** (+4), aligned 848.
* `stationPirates` create loop: the original order is
  `createStaticObject` -> `enemies[i]=` -> `pirateBase=getPosition()` ->
  `setToSleep` -> `name` (SP).  SP reproduces the stream but is size-fatal
  (17,176 B, raw 3,595).
* Case-2 loot block: the original reloads `this->enemies` for each
  statement (no `enemies` cache) and keeps the ship pointer across the
  `cargo`/`crateCaptured` stores; dropping the local is size-fatal
  (17,180 B).
* Case-2 escort `offset += 2000` before `i++` is the landed fix; the same
  ordering idea was neutral for the case-4 tail (`E4`).

## 5. Reproduce

```sh
cd /home/debian/GalaxyOnFire2/wt/w13512
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
(ulimit -v 12000000; cmake --build cmake-build-match --target check -j2)
```

Expected: `raw=2661 aligned=863 size=17172/17172`, `.text` diff 71,470,
`.ARM.extab` diff 577, `.rel.plt` diff 0, `RATCHET PASS`.

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w13512/`
(`range.py`, `allruns.py`, `cumdelta.py`, `secdiff.py`, `map3.py`, `v.py`,
`exp.py`).
