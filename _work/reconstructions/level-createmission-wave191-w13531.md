# `Level::createMission` (`_ZN5Level13createMissionEv`) — wave 191 / w13531

Target `src/game/world/Level.cpp`, function VMA `0xaffd8`, size **17,172 B**
(`0x4314`). Branch `wave191b-w13531`, base master `a303f439e`. Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, armeabi-v7a, image base `0x10000`).

Continues `level-createmission-wave191-w13512.md` / `...w13526.md`
(case-2 convoy escort latch + loot setter order).

```text
                                     size            raw    aligned  insn
base a303f439e                       17172/17172     2659    871     6206/6197
landed                               17172/17172     2656    867     6206/6197
```

## 1. Result — landed

| metric | before | after |
|---|---:|---:|
| size | 17,172 | 17,172 (exact) |
| raw | 2,659 | **2,656** (-3) |
| aligned | 871 | **867** (-4) |
| insn | 6206/6197 | 6206/6197 |
| `.ARM.extab` diff | 577 | 577 |
| `.rel.plt` diff | 0 | 0 |
| `.ARM.exidx` diff | 0 | 0 |

`check`: **RATCHET PASS** (byte_exact 4507/4524, linked_exact 4507/4524,
size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0).

## 2. The change: case-2 convoy loot statement order

The case-2 loot loop's four independent tail statements were ordered
`cargo -> crateCaptured -> setRotationSpeed -> setSpeed`.  A full 4! sweep
of the independent statements (with the `enemies` cache local kept) found
that the best size-exact order is

```cpp
((PlayerFighter *) (*enemies)[convoyEnd + j])->crateCaptured() = 0;
(*enemies)[convoyEnd + j]->setSpeed(0.0f);
(*enemies)[convoyEnd + j]->cargo = nullptr;
(*enemies)[convoyEnd + j]->setRotationSpeed(0.0f);
```

All four write disjoint fields, so the reorder is semantically identical.
It moves the inlined `setSpeed(0)` vtable call ahead of the `cargo` store,
matching the original's call/store schedule more closely (raw -3,
aligned -4).  The `Array<KIPlayer *> *enemies` cache local must stay: using
`this->enemies` directly for any of these statements is size-fatal or
raw-fatal (see table).

## 3. Measurements (all raw/aligned; size 17,172 unless noted)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base (`cargo,crate,rot,spd`) | 2,659 | 871 | 17,172 |
| **`crate,spd,cargo,rot` (landed)** | **2,656** | **867** | **17,172** |
| `crate,cargo,rot,spd` | 2,657 | 871 | 17,172 |
| `crate,cargo,spd,rot` | 2,661 | 863 | 17,172 |
| `crate,rot,spd,cargo` | 2,659 | 864 | 17,172 |
| `spd,crate,cargo,rot` | 2,670 | 871 | 17,172 |
| `crate,spd,cargo,rot`, first stmt direct | 2,656 | 867 | 17,172 |
| `crate,spd,cargo,rot`, any other stmt direct | 4,488+ | 757+ | 17,172 |
| `crate,spd,cargo,rot`, `KIPlayer *ship` local | 15,399 | 1,149 | 17,164 |

## 4. Residual / neutral cells (do not repeat)

* Case-183 `switch (k)` block order: ours lays out `case0, case2, case1,
  default`, original `case0, case1, case2, default`.  Re-measured all 24
  source case permutations on this base: `0,1,D,2` and `0,D,1,2` stay at
  2,659; `0,1,2,D`, `0,D,2,1`, `0,2,1,D`, `0,2,D,1` are 2,660; every
  permutation not starting with case 0 is ~5,370.  `if/else` chains
  (012/210/021/102) are 5,448+.  Case-2 body variants (`float x = 25000`
  hoist, reorder, all-volatile, no-volatile) are neutral or worse
  (no-volatile 2,996; reorder2 5,361).  The 5-byte run at `0xb1432` is a
  MachineBlockPlacement choice, not source order.
* Case-4 dispatch hoist: ours loads `this` into `sl` at `0xb19ca` before
  the `roll != 0` compare; the original loads it at `0xb19e0` inside the
  taken branch.  Seven source spellings (`if`, `if/else`, `!roll`,
  explicit local `Level *self`) all produce identical codegen (2,659).
* Case-4 tail: `hunters` before `opt44` (faithful order) is 2,662 raw /
  865 aligned; moving `new Array` after `hunterCount` is 2,660 / 862;
  dropping the `globals` local is 2,764 / 864; dropping the `enemies`
  local is size-fatal (17,168).
* Case-1 attacker/friends loops (`0xb1608..0xb16d3`, 78-byte runs) are a
  two-byte shift from the known `enemyRace` loop-head reload hoist; not
  touched here.

## 5. Reproduce

```sh
cd /home/debian/GalaxyOnFire2/wt/w13531
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
(ulimit -v 12000000; cmake --build cmake-build-match --target check -j2)
```

Expected: `raw=2656 aligned=867 size=17172/17172`, `.rel.plt` diff 0,
`RATCHET PASS`.

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w13531/`
(`sbs.py`, `win.py`, `bytemap.py`, `sweep*.py`, `Level.cpp.base`).
