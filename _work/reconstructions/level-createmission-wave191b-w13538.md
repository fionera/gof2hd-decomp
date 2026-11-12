# `Level::createMission` (`_ZN5Level13createMissionEv`) — wave 191b / w13538

Target `src/game/world/Level.cpp`, function VMA `0xaffd8`, size **17,172 B**
(`0x4314`). Branch `wave191b-w13538`, base master `23a6ebf8b`. Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, armeabi-v7a, image base `0x10000`).

Continues `level-createmission-wave191-w13512.md` / `...w13526.md` /
`...w13531.md` (case-2 convoy latch + loot store order).

```text
                                     size            raw    aligned  insn
base 23a6ebf8b                       17172/17172     2656    867     6206/6197
landed                               17172/17172     2629    845     6206/6197
```

## 1. Result — landed

| metric | before | after |
|---|---:|---:|
| size | 17,172 | 17,172 (exact) |
| raw | 2,656 | **2,629** (-27) |
| aligned | 867 | **845** (-22) |
| insn | 6206/6197 | 6206/6197 |
| `.text` diff | 71,487 | **71,460** (-27) |
| `.ARM.extab` diff | 586 | 586 |
| `.rodata` diff | 485 | 485 |
| `.rel.plt` diff | 0 | 0 (byte-identical) |
| `.ARM.exidx` diff | 0 | 0 |

27/27 ELF headers, file size 2,187,328 exact. `check`: **RATCHET PASS**
(byte_exact 4507/4524, linked_exact 4507/4524, size 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0). Symbol parity 5370/5370.

## 2. The change: case-183 convoy switch block order

The case-183 `switch (k)` block (`0xb1418..0xb144e`) had the right dispatch
chain but the wrong **block order**: ours emitted `case0, case2, case1,
default`, the original `case0, case1, case2, default`.  Since the literal
pool follows block order, our pool was `25000,-36000, -7000,10000,-30000,
6000,12000,-45000` instead of `-7000,10000,-30000, 25000,-36000, 6000,
12000,-45000`, i.e. 12 B of switch bytes + 24 B of pool + the `ldr` at
`0xb1460` were wrong.  All 24 source permutations of the four case bodies
(w13521/w13531) leave the layout at `case0, case2, case1, default`: the
order is a post-RA MachineBlockPlacement choice driven by the dispatch
successor order (`beq case2`, `beq case1`, `cbnz default`), not source order.

The lever that flips it is the **loop back-edge weight**.  Writing the
convoy loop condition as

```cpp
for (int k = 0; __builtin_expect(k < 4, 1); k++) { // lint: arm_builtin ...
```

makes the loop latch the hot edge, which changes MBP's global chain and
lays the case blocks out in source order.  The switch block then becomes
byte-identical and the literal pool order is restored.  `__builtin_expect`
returns its first argument, so the loop semantics are unchanged.

The case-2 body also needed its two stores in the original order
(`offset[1]` then `offset[0]`).  clang reordered the plain `offset[0]`
store ahead of the volatile `offset[1]` store; making both `offset[0]` and
`offset[2]` volatile stores fixes the in-block order to
`ldr 25000; str.w sl,[376]; str r1,[372]; ldr -36000`, matching the
original case-2 block byte-for-byte.

```cpp
case 2:
    *(volatile float *) (offset + 1) = 0.0f;
    *(volatile float *) (offset + 0) = 25000.0f;
    *(volatile float *) (offset + 2) = -36000.0f;
    break;
```

## 3. Measurements (all raw/aligned; size 17,172 unless noted)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base | 2,656 | 867 | 17,172 |
| case-2 both-volatile only | 2,656 | 867 | 17,172 |
| loop `__builtin_expect(k<4,1)` only | 2,629 | 845 | 17,172 |
| **both (landed)** | **2,629** | **845** | **17,172** |
| `__builtin_expect(k<4,0)` | 11,757 | 1,215 | 17,184 |
| `switch (__builtin_expect(k,0))` | 11,075 | 1,097 | 17,180 |
| `k != 4`, `k <= 3`, `!!(k<4)` | 2,629 | 845 | 17,172 |
| `__builtin_expect(k,0) < 4` | 2,656 | 867 | 17,172 |
| `unsigned k` + expect | 2,629 | 845 | 17,172 |

Expect on the *other* loops (case-1 attacker/friends, case-2 escort/loot,
case-3/5, case-7, case-9, case-10, ...) all grow the frame (17,340+), so
only the case-183 convoy latch is load-bearing.

## 4. Residual / neutral cells (do not repeat)

* Case-183 switch block is now **byte-identical** (`0xb1418..0xb144e`,
  pool `0xb15bc..b15dc`); the case-183 body was already exact.
* Case-1 `enemyRace` loop-head reload hoist (`ldr r5,[sp,#116]` at
  `0xb1608` vs preheader `0xb158c`) is unchanged; the case-1 region
  (`0xb158c..0xb178c`) is the next big residual.
* Case-2 loot block (`0xb1933..0xb19eb`) unchanged; the w13512/w13526/
  w13531 ordering is still best.
* Case-4 tail (`0xb40b2..0xb41ea`) unchanged; `this` in `sl` vs the
  original's `r8`, the cached `enemies` local in `fp`, and the missing
  `b.w` return after the `nameLeader` body are the dominant differences.
  Adding the missing `return;` is size-fatal (+4 B, 17,176) and raw-fatal
  (3,672) on its own.
* Adjacent-statement swap sweep over the whole case-183 body and the
  case-4 tail: no raw win (best neutral 2,656; `hunters` before `opt44`
  2,659/861; `enemies=new` before `hunterCount` 2,657/858).

## 5. Reproduce

```sh
cd /home/debian/GalaxyOnFire2/wt/w13538
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
(ulimit -v 12000000; cmake --build cmake-build-match --target check -j2)
```

Expected: `raw=2629 aligned=845 size=17172/17172`, `.text` diff 71,460,
`.rel.plt` diff 0, `RATCHET PASS`.

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w13538/`
(`sbs.py`, `runs.py`, `sweep.py`, `c4sweep.py`, `perm24.py`, `exp2.py`,
`exp3.py`, `loopsweep.py`, `rollsweep.py`, `diffset.py`).
