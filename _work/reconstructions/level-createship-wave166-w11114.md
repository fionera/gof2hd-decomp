# `Level::createShip` — wave 166 (w11114): faithful jitter call order

Worktree `wt/w11114`, base `master 85c1f88df` (wave 165).  Target
`_ZN5Level10createShipEiiiP8Waypointbb` (`src/game/world/Level.cpp`,
2,764 B, VMA 0xc0214).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).  All
evidence is objdump of the original.

Gate on entry and exit (full `check`): `byte_exact 4507/4524`,
`linked_exact 4507/4524`, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, parity 5370/5370.  `.rel.plt` byte-identical (0).

| metric | before | after |
|---|---:|---:|
| raw same-address diff | **1,560** | **1,560** |
| instruction-index alignment | **455** | **460** |
| size | 2,764 | 2,764 |
| `.text` differing bytes | 83,147 | 83,147 |

## 1. Landed change

The three `Globals::rnd->nextInt(40000)` jitter calls are back in the
original's call order **`jitterX, jitterY, jitterZ`** (wave 164 had
landed the tuned companion `Y,X,Z`).  The original's stack slots
`[sp,#36]`,`[32]`,`[28]` are `jitterX`,`jitterY`,`jitterZ`, so `X,Y,Z`
is the real source order.  Raw is unchanged (1,560); aligned moves
455 -> 460 (difflib re-alignment only).  This is a raw-neutral
structural-fidelity commit, not a raw win.

## 2. Register-map sweep (no 2,764 B map found)

The dominant residual is still the 4-byte RA shift documented in
`level-createship-wave163-w10914.md` / `-wave164-w11014.md`:

| value | original | ours |
|---|---|---|
| `shipClass` | `r5 -> sl -> r6` | `[sp,#60]` |
| `type` | `fp` | `sl` |
| `jitterX` | `[sp,#36]` | `fp` |
| `status` | `r4` | `r4` |
| `this` | `r9` (spill `[56]`) | `r9` (spill `[52]`) |

The original's coordinate locals are **ints initialised directly from
the waypoint** (waypoint stores land in `[44]`,`[48]`,`[40]`), then
`x += jitterX - 20000` etc. before `setEmpData`; there is no null check
in the emitted code (it is dead: `Globals::status` is dereferenced at
entry).  Our source keeps `wy/wx/wz` + `float y,z` + late `int x`.

Full-link measurements (size checked, all 2,764 B unless noted):

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base (wave 164, jitter `Y,X,Z`) | 1,560 | 455 | 2,764 |
| **faithful jitter `X,Y,Z` (landed)** | **1,560** | **460** | **2,764** |
| faithful jitter + case `0x2c,0x33,0x31` | 1,563 | 449 | 2,764 |
| faithful jitter + case `0x33,0x31,0x2c` | 1,563 | 451 | 2,764 |
| faithful jitter + `if (!shipClass)` | 1,560 | 460 | 2,764 |
| faithful jitter + mission bitmask | 1,573 | 465 | 2,764 |
| faithful jitter, null check dropped | 2,052 | 505 | 2,764 |
| `int x,y,z` (D: base with int coords) | 2,531 | 888 | **2,788** |
| `int x,y,z`, null check before `setEmpData` | 2,553 | 482 | 2,796 |
| `int x,y,z`, null check dropped | 2,413 | 501 | 2,792 |
| waypoint-direct int `x,y,z`, no null check | 2,413 | 484 | 2,792 |
| waypoint-direct int `x,y,z` + mission bitmask | 2,405 | 484 | 2,792 |
| waypoint-direct int `x,y,z`, coords before `new Player` | 2,340 | 502 | 2,792 |
| `float y,z` + `int x` before `new Player` | 2,495 | 511 | 2,780 |
| `if/else` null check anchoring `int x` before `setEmpData` | 2,485 | 559 | 2,796 |

Key findings:

* Moving the coordinate block before `setEmpData` **does** free `fp`
  from `jitterX` and puts `type` in `fp` (the original's slot), but the
  allocator then parks `shipClass` in `fp` and `type` in `sl` (swapped
  relative to the original) and the shape costs +28..+32 B.
* The waypoint-direct int shape is what the original source really is
  (stores go straight to the coordinate slots), but it is robustly
  2,792 B in our build: `type` spills to `[60]`, `status` moves to
  `r6`, `race`/`this` shuffle.  Every declaration / assignment /
  coordinate-order permutation (all 6 decl orders, all 6 assignment
  orders, all 6 coordinate orders, `cls` copies at 5 placements) was
  measured; all 2,792 B and the same map.
* `__builtin_expect` on the `shipClass == 0/1` guards changes the map
  but costs size (2,756..2,816 B) and raw (2,279..2,565).
* The residual is therefore the original's PGO-like branch weights
  (wave 163 note), not a pure source-order spelling; reproducing the
  exact `r5 -> sl -> r6` split at 2,764 B was not found this wave.

## 3. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w11114
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level10createShipEiiiP8Waypointbb
cmake --build cmake-build-match --target check -j2
```

Scratch `/dev/shm/w11114/`: `splice.py`/`gen.py`/`gen2.py` (body
splicers), `measure.sh` (build + fn_diff), `runvar.py`, `sbs.py`
(instruction-level side-by-side), `body_*.txt` (measured variants),
`mi.txt` (clang `-print-machineinstrs` dump).
