# `Level::createShip` — wave 186 (w13018): EmpGuard scopes are load-bearing
# for `.ARM.extab`; base 1,202 is the best extab-exact form

Worktree `wt/w13018`, branch `wave186-w13018`, base master `2d02567f6`
(wave 185).  Target `_ZN5Level10createShipEiiiP8Waypointbb`
(`src/game/world/Level.cpp`, 2,764 B, VMA `0xc0214`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13018`.

**Status: no win landed.**  Base is raw **1,202**, aligned 484,
size 2,764/2,764, insns 1,116/1,117, `.text` diff 74,402,
`.ARM.extab` 647 (size/base exact at 0x1a3b78, 156,312 B),
`.ARM.exidx` 0, `.rel.plt` byte-identical.

## 0. Bottom line

Removing either of the two `EmpGuard` scopes is a **text win** but
shrinks `.ARM.extab` and blows up its content diff, so it is a net
loss under the size/base-exact rule.  Every declaration-order,
statement-transposition and coordinate-form variant that keeps the
guards is >= 1,202.

## 1. EmpGuard removals (text win, extab regression)

The `EmpGuard` scopes are not dead weight: each one contributes an EH
cleanup region to `.ARM.extab`.  The original's 156,312-byte extab
matches ours *because* both guards are present.

| variant | raw | size | `.text` diff | `.ARM.extab` size | extab diff |
|---|---:|---:|---:|---:|---:|
| base | **1,202** | 2,764 | 74,402 | 156,312 | 647 |
| remove guard around `mission = getCurrentCampaignMission()` | 1,166 | 2,764 | 74,366 | **156,308** | 122,306 |
| remove guard around `fighter->setShipGroup(...)` | 1,198 | 2,764 | 74,398 | **156,304** | 107,796 |
| remove both | 1,425 | 2,760 | — | — | — |
| remove `setEmpData` guard | 2,187 | 2,780 | — | — | — |

The mission guard is worth 4 extab bytes, the setShipGroup guard 8.
Trying to compensate by adding a guard elsewhere either overshoots or
cancels the register-allocation benefit:

| compensation | raw | extab size |
|---|---:|---:|
| remove mission guard + guard around `getLevel()` | 1,227 | 156,312 |
| remove mission guard + guard around `gameWon()` | 1,230 | 156,312 |
| remove mission guard + guard around `lodManager->addObject` | 1,206 | 156,316 |
| remove mission guard + guard around `ship->setLevel(this)` | 1,202 | 156,312 |
| remove mission guard + guard around `fixed->setBV(volumes)` | 1,202 | 156,312 |
| remove mission guard + guard around `empPoints` | 1,202 | 156,320 (`.rodata` base +0x10) |

Every extab-size-exact compensation lands back at >= 1,202.

## 2. Other sweeps (all size 2,764, all >= 1,202)

| shape | raw |
|---|---:|
| base | **1,202** |
| `ship = fighter;` moved before the `type==0x2c/0x31` block | 1,202 |
| `Array<BoundingVolume*> *volumes;` declared before `fixed`, allocated after | 1,202 |
| `volumes` declared at function top | 1,202 |
| `int empTime;` before `int empPoints` | 1,202 |
| `hitpoints` switch cases 0x31/0x33 swapped | 1,202 |
| `mission == 49..56` condition reversed | 1,202 |
| `ship = fixed;` immediately after the ctor (copy-folded) | 1,202 |
| natural jitter order (`jitter1` = first `nextInt`) | 1,204 |
| `tail` reorder: `lodManager->addObject` first | 1,205 |
| `tail` reorder: `setBV` before `setShipGroup` | 1,206 |
| `tail` reorder: `setShipGroup` after `field_0x40 = 1` | 1,206 |
| `lodManager->addObject` before `setShipGroup` | 1,223 |
| `volumes` declared/allocated before `fixed` | 2,230 (size 2,760) |
| `PlayerFixedObject *fixed` allocated after `volumes` | 2,231 (size 2,760) |
| `hitpoints` computed before `level` | 2,277 (size 2,760) |
| branches swapped (`shipClass==1` first) | 2,483 (size 2,792) |
| `KIPlayer *ship;` uninitialised | 2,418 (size 2,760) |
| remove mission guard + guard around `getLevel()` | 1,227 |
| remove mission guard + guard around `gameWon()` | 1,230 |
| remove mission guard + guard around `hardCoreMode()` | 2,452 (size 2,768) |
| remove mission guard + guard around `empPoints` | 1,202 but extab 156,320, `.rodata` base +0x10 |

### Coordinate form: full 4^3 sweep

For each of `x,y,z` the form was one of `{int before guard, int after
guard, float before, float after}` (64 cells).  The unique best is the
landed wave-185 split (`int x` after, `float y` after, `float z`
before) at **1,202**; next best 1,207, then 1,210, 1,211, 1,213.

Moving the coordinate statements inside the `EmpGuard` scope
(`z` inside; `x,y` inside; all three inside) gives 1,202 / 1,217 /
1,217.

### `-mllvm -disable-machine-sink` (exploratory)

The original computes all three int coordinates *before* the
`setEmpData` call and converts at each ctor site (6 `vcvt`).  The
all-int source form (`int x,y,z` + `(float)` casts) lets clang sink the
computation below the call (size 2,752).  With
`-mllvm -disable-machine-sink` the computation stays before the call
and the size is exactly 2,764, but the register allocation is still
different and raw is 2,051.  Not a win, and the flag is not in the
build recipe.

## 3. Remaining residual

The first textual divergence is the stack-slot assignment of the
waypoint/jitter spills (`wpx` [sp+44] vs [sp+40], `wpy` [sp+48] vs
[sp+36], `wpz` [sp+40] vs [sp+48], `jitter3` [sp+36] vs [sp+44]).
Downstream of that the callee-saved assignment differs:
`ship`/`fixed` = r9 in the original vs `ship` = fp, `fixed` = r6 then
`mov fp,r6` here, and `volumes` = fp vs r9.  The original coalesces
`ship` with both `fighter` and `fixed`; clang 7 here only coalesces
`ship` with `fighter`.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13018
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN5Level10createShipEiiiP8Waypointbb
```

Scratch: `Level_base.cpp`, `measure.sh`, `cs_sweep*.py`,
`cs_coord.py`, `cs_guard*.py`, `secdiff.py`.
