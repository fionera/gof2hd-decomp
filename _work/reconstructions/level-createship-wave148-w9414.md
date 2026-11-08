# `Level::createShip` — wave 148 (w9414): the `status`→r4 register is
# reachable only with a (non-witnessed) null guard; every faithful lever
# coalesces or re-rolls the map.  Note-only.

Status: **note-only** (no `src/` change landed; master body kept).
Gate on the unchanged tree: `byte_exact 4507/4524`, `linked_exact 4507/4524`,
size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0.
Worktree `wt/w9414`, base master `cbe3b5f7f` (wave 147).
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).

Target `_ZN5Level10createShipEiiiP8Waypointbb` (`Level::createShip`),
original 2764 B @ `0xc0214`.  Metrics: raw same-address diff bytes
(`/dev/shm/w9414/fdiff.py`) and instruction-index alignment
(`_work/reconstructions/align_diff.py`); the official verify fuzzy is quoted
where it was measured.

## 0. Baselines reproduced

```
master (float z, float y, int x)   raw 1978  size 2764  aligned 486  fuzzy 70.6
iif    (int x, int y, float z)     raw 1647  size 2768  aligned 495
iii    (int x, int y, int z)       raw 2333  size 2792  aligned 482  fuzzy 60.1
```

## 1. New result: the exact original register map *is* reachable — with a guard

The original map is `r4=status, r5=rnd, r6=nextInt, r8=mission, r9=this,
sl=shipClass→empPoints, fp=type`, with `race` at `[sp,#60]` and all three
jitters spilled.  Placing the (unfaithful) null guard
`if (Globals::status == nullptr) return nullptr;` at different points of the
`iif` body shows the map is a real RA state, not an artefact:

| guard after … | status | nextInt | type | raw | size | aligned |
|---|---|---|---:|---:|---:|---:|
| (none, `iif`) | r6 | r5 | [sp] | 1647 | 2768 | 495 |
| `setEmpData` | **r4** | **r6** | sl | 2521 | 2796 | 494 |
| `int x = …` (coords) | **r4** | **r6** | **fp** | 2511 | 2800 | 453 |
| `Player *player = new` | **r4** | **r6** | sl | 2367 | 2808 | 515 |
| `armor = …` | r6 | r4 | r9 | 2468 | 2788 | 443 |
| `empPoints = …` | r6 | r5 | sl | 2457 | 2776 | 891 |
| `hitpoints = …` | r6 | r5 | sl | 2447 | 2784 | 888 |
| `level = …` | r6 | r5 | sl | 2499 | 2784 | 877 |
| `jitterZ = …` | r6 | r5 | [sp] | 2359 | 2772 | 493 |
| `wz = …` | r6 | r5 | r4 | 2359 | 2768 | 895 |
| top / after mission | r6 | r5 | r4 | 2466 | 2776 | 862 |

**`guard after the coordinate block` gives the exact original map**
(`status r4`, `nextInt r6`, `type fp`) at aligned **453** (vs 486) but costs
+36 B (2800).  So the wave-147 premise is confirmed: putting `status` in r4
cascades `shipClass` into `sl`→`r6`, `empPoints` into `sl` and `type` into
`fp`.  The problem is purely that the only source producer found for it is a
null check the original does not contain (the original's `r4` dies at
`c0398`/`hardCoreMode`, before `new Player`; there is no post-`setEmpData`
`status` use).

On the faithful `iii` (int,int,int) shape the same guard gives
`status r4`, `type fp`, raw 2521, size 2796, aligned **465** — the best
alignment seen for any shape, but still +32 B.

## 2. The mission's suggested faithful levers — all neutral or worse

Measured on the real TU, full link, one function:

| variant | raw | size | status reg |
|---|---:|---:|---|
| `iif` baseline | 1647 | 2768 | r6 |
| `Status *st = Globals::status;` all uses | 2457 | 2768 | **r5** (entry spills this/race/shipClass via `stmia`) |
| `Status *st` only for the mission call | 1647 | 2768 | r6 (coalesced) |
| `Status *st` for mission + level clamp | 2478 | 2784 | r6 |
| `Status *st` + `(void) st` | 1647 | 2768 | r6 (coalesced) |
| `Status *st` + `if (st == nullptr) return nullptr;` at top | 2522 | 2780 | r6 |
| `int shipType = type;` used everywhere | 1647 | 2768 | r6 (coalesced) |
| `int sc = shipClass;` used everywhere | 1647 | 2768 | r6 (coalesced) |
| both locals | 1647 | 2768 | r6 (coalesced) |
| mission statement moved after the waypoint reads | 1688 | 2768 | r6 |
| mission moved after the jitters | 1711 | 2768 | r6 |
| waypoint reads moved before mission | 1647 | 2768 | r6 |
| explicit `if (waypoint) {…} else {…}` (entry split) | 1683 | 2768 | r6 |

The `Status*` local is the only lever that changes the map, but it moves
`status` to **r5**, not r4, and forces the entry to spill `this` too
(`stmia r3!, {r0,r1,r2}`), which is worse than the master entry.

## 3. `__builtin_expect` sweep (256 combinations)

All 8 branches of the `iif` body were given `__builtin_expect(cond, 0|1)` in
every combination (waypoint, `mission==0x9a`, `shipClass==1` empTime,
`shipClass==0`, `shipClass==1` capital, mission 49..56, missionPtr,
`type==0x2c||0x31`).  **No combination produces `status r4` at 2764 B.**
The best raw at 2764 B is `expect(mission == 0x9a, 1)` alone
(raw **1420**, aligned 496, official fuzzy 67.0 — the expect flips the
`bne`/fall-through direction to match the original's `c0380 bne` layout, but
the instruction stream is 3 insns shorter and the fuzzy drops).
The best alignment at a near-exact 2760 B is
`expect(mission==0x9a,1) + expect(mission in 49..56,1)` (aligned **436**,
raw 2377); adding any further expect to reach 2764 B pushes alignment back
to ≥494.

## 4. Conclusion / open item

The `status`→r4 map is a genuine RA state (guard-after-coords reproduces it
exactly), but the original's branch weights are the only plausible faithful
producer and no combination of the 8 source-level `__builtin_expect` sites
reaches it.  The residual is consistent with the original's PGO block
frequencies, which a flat profile cannot express (cf. wave 146's `!prof`
finding for `Status::missionCompleted`).  Nothing landable: every 2764 B
variant is worse than master on at least one of raw/aligned/fuzzy.

## 5. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j16
python3 /dev/shm/w9414/fdiff.py _ZN5Level10createShipEiiiP8Waypointbb
python3 /dev/shm/w9414/align_diff.py _ZN5Level10createShipEiiiP8Waypointbb
```
Scratch `/dev/shm/w9414/`: `try.py` (replace body + rebuild + score),
`mapinfo.py` (entry register map), `sweep.py` (256-combination expect sweep),
`batch1..9.py` (levers), `sbs.py` (side-by-side aligned disassembly),
`exact.py` / `ratio.py`, `body_master.txt` / `body_iif.txt` / `body_iii.txt`.
