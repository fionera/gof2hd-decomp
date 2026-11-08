# `Level::createShip` — wave 150 / worker w9616

Target `_ZN5Level10createShipEiiiP8Waypointbb`, original 2,764 B.
Baseline (wave-149 master `7eed2966e`): raw same-address diff **1,978**,
instruction-index alignment **482**, size 2,764.

## Landed: declare `wy`, `wz`, `wx` (wz before wx)

The wave-149 `wy`-before-`wx` change fixed the *store order* of the
null-waypoint branch but left the frame-index assignment `x→[sp,#48],
y→[sp,#44]`, whereas the original is `x→[sp,#44], y→[sp,#48]`.  Moving
`wz` between them (`wy, wz, wx`) gives the best instruction-index
alignment seen at the exact 2,764 B size:

| variant | size | raw | aligned | entry map |
|---|---:|---:|---:|---|
| HEAD (`wy, wx, wz`) | 2,764 | **1,978** | 482 | status r6, nextInt r4 |
| **landed (`wy, wz, wx`)** | **2,764** | 2,056 | **480** | status r6, nextInt r4 |

The raw increase is a pure same-address shift: the three waypoint spill
slots become `x→[sp,#48], y→[sp,#40], z→[sp,#44]` (vs `x→48, y→44,
z→40`), so every later `ldr [sp,#..]` moves.  The aligned metric improves
because two `b.n` branch mismatches disappear (`operand:b.n` 41 → 39).
Gate: byte_exact 4507/4524, linked_exact 4507/4524, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370,
RATCHET PASS.

## Entry register map (requested)

Original (2,764 B, `0xc0214`):

| value | reg | note |
|---|---|---|
| `this` | r9 | |
| `type` | fp (r11) | `mov fp,r3` before the mission call |
| `shipClass` | sl (r10) | `mov r5,r2` then `mov sl,r5` after the call |
| `race` | `[sp,#60]` | spilled first |
| `&Globals::status` | **r4** | reloads `[r4]` at each use |
| `&Globals::rnd` | r5 | |
| `&AERandom::nextInt` | **r6** | dies at `c0282`, r6 reused for shipClass |
| `mission` | r8 | |
| `jitterX` | `[sp,#36]` | spilled |

Ours (both baseline and landed, 2,764 B):

| value | reg | note |
|---|---|---|
| `this` | r9 | |
| `type` | sl (r10) | `mov sl,r3` before the mission call |
| `shipClass` | `[sp,#60]` | spilled (with race in one `strd`) |
| `race` | `[sp,#56]` | |
| `&Globals::status` | **r6** | |
| `&Globals::rnd` | r5 | |
| `&AERandom::nextInt` | **r4** | |
| `mission` | r8 | |
| `jitterX` | fp (r11) | |

So the residual is exactly the r4/r6 swap of `&Globals::status` /
`&nextInt`; `rnd` is r5 in both.  The original's order is
`status, rnd, nextInt` (forward vreg order); ours is
`nextInt, rnd, status` (reverse).

## New hypotheses measured (all full link, exact TU)

### 1. `status` in r4 *is* reachable — but never at 2,764 B

| construct | size | raw | aligned | status / nextInt |
|---|---:|---:|---:|---|
| `do { mission=...; } while (mission == -1);` | 2,788 | 2,460 | 702 | **r4 / r6** |
| `while (mission == -1) { mission=...; }` | 2,788 | 2,460 | 702 | **r4 / r6** |
| `expect(shipClass==0,1)` + `expect(mission==0x9a,0)` | 2,744 | 2,587 | 1,496 | **r4 / r6** |
| `empPoints` computed before `hitpoints` | 2,760 | 2,217 | 509 | **r4 / r6** |
| `empPoints` before `hitpoints` + `expect(mission==0x9a,0)` | 2,768 | 2,493 | 557 | **r4 / r6** |
| `empPoints` before `hitpoints` + `expect(mission==0x9a,1)` | 2,760 | 2,218 | 509 | **r4 / r6** |
| `expect(shipClass==0,1)` alone | 2,740 | 2,556 | 1,323 | r4 / r6 |
| `expect(mission==0x9a,1)` alone | 2,764 | 1,978 | 482 | r6 / r4 |

The `do/while` and the `shipClass==0` expect both make the fighter arm
hot, which raises `shipClass`'s spill weight so it keeps a register;
`status` then takes r4 and `nextInt` r6.  But every such variant either
changes the size or re-rolls the rest of the map (`type`, `shipClass`,
`mission`, `this` all move), and the aligned count rises.

### 2. The mission's suggested faithful levers all coalesce to baseline

`Status *st = Globals::status;` used only for the entry calls, used for
all calls, `Status *&status = Globals::status;`,
`Status **status = &Globals::status;`, `do { mission=...; } while (0);`,
`int mission; mission = ...;`, `(void) st` — all produce the identical
1,978 / 482 / 2,764 body (GVN folds the local back to the global).

### 3. Reordering the entry statements

| variant | size | raw | aligned |
|---|---:|---:|---:|
| jitters before mission | 2,756 | 2,456 | — |
| mission after waypoint reads | 2,764 | 2,079 | — |
| mission after jitters | 2,760 | 2,349 | — |
| level before jitters | 2,796 | 2,535 | — |
| hitpoints before jitters | 2,800 | 2,549 | — |
| `if (waypoint)` instead of ternary | 2,764 | 2,055 | — |
| coordinates before `new Player` | 2,768 | 2,285 | — |

### 4. All six `wy/wx/wz` declaration permutations

| order | size | raw | aligned |
|---|---:|---:|---:|
| `wy wx wz` (HEAD) | 2,764 | 1,978 | 482 |
| `wy wz wx` (landed) | 2,764 | 2,056 | **480** |
| `wx wy wz` | 2,764 | 1,978 | 486 |
| `wx wz wy` | 2,764 | 2,055 | 480 |
| `wz wy wx` | 2,764 | 2,056 | 480 |
| `wz wx wy` | 2,764 | 2,055 | 480 |

## Conclusion / open item

The r4/r6 rotation is a real RA state reachable from several source
shapes, but every shape that produces it also re-rolls `type`/`shipClass`
and costs either size or alignment.  The landed permutation is the only
2,764 B variant that improves the structural (aligned) metric.  The next
lever is probably the `shipClass` spill weight: the original keeps
`shipClass` in r5→sl and spills `race`, while we spill both.  A faithful
source producer of the original's branch weights (PGO) is still missing.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9616/fdiff.py _ZN5Level10createShipEiiiP8Waypointbb
python3 /dev/shm/w9616/align_diff.py _ZN5Level10createShipEiiiP8Waypointbb
```
Scratch `/dev/shm/w9616/`: `batch.py` (replace body + build + raw/aligned +
entry-map extractor), `gen*.py` (variant generators), `sweep*.py`
(`__builtin_expect` sweeps), `ent.sh` (entry disassembly), `sbs.py`
(side-by-side diff), `mir2.log` (`-mllvm -print-machineinstrs` for the
whole TU, filtered to `createShip`).
