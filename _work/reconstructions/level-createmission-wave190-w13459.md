# `Level::createMission` (`_ZN5Level13createMissionEv`) — wave 190 / w13459

Target `src/game/world/Level.cpp`, function VMA `0xaffd8`, size **17,172 B**
(`0x4314`). Branch `wave190-w13459`, base master `83d52fe52`. Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, armeabi-v7a, image base `0x10000`).

Continues `level-createmission-wave190-w13450.md`. No size-exact raw win was
found on the current base; this note records the new structural cells that
were measured and the exact codegen obstacle in each.

```text
base 83d52fe52                       17172/17172     2982   1123     6210/6197
```

## 1. `stationPirates` first loop — original is `D C A B`, ours `A B C D`

Source (lines ~2038-2044):

```cpp
Route *route = new Route(coords, 3);
(*this->enemies)[i]->setToSleep();                            // A
(*this->enemies)[i]->name = *Globals::gameText->getText(441); // B
pirateBase = route->getWaypoint()->getPosition();             // C
(*this->enemies)[i] = this->createStaticObject(route->getWaypoint(), 14243, true); // D
```

The original emits **D, C, A, B** (`0xb3b4a..0xb3bae`): `createStaticObject`,
then `getWaypoint()->getPosition()` into `pirateBase`, then `setToSleep` and
`name` on the *new* static object.  This is a semantic difference: our base
calls `setToSleep`/`name` on the old ship before overwriting `enemies[i]`.

Full 24-permutation sweep of the four statements (size 17,172 unless noted):

| order | raw | aligned | size |
|---|---|---|---|
| `ABCD` (base) | **2982** | 1123 | 17172 |
| `CABD` | 2987 | 1104 | 17172 |
| `ACBD` | 2988 | 1109 | 17172 |
| `CDAB` | 2988 | 1103 | 17172 |
| `CADB` | 2989 | 1117 | 17172 |
| `CBAD` | 2989 | 1112 | 17172 |
| `CDBA` | 2989 | 1117 | 17172 |
| `CBDA` | 2990 | 1112 | 17172 |
| `ACDB` | 2990 | 1117 | 17172 |
| `BCAD` | 2991 | 1131 | 17172 |
| `BCDA` | 2992 | 1126 | 17172 |
| every `D`-first order | 3892–3908 | 1094–1125 | **17176** |

`DCAB` (the faithful order) is `raw=3908 aligned=1094 size=17176`.  The block
itself then matches the original instruction-for-instruction, but `D`-first
costs +4 bytes: after `createStaticObject` the compiler must reload
`(*this->enemies)[i]` from memory for `setToSleep`/`name`, and that reload is
not compensated elsewhere.  `DCAB` is the best *aligned* result measured on
this function (1094 vs base 1123) but is size-fatal.  Any future pass that
finds a −4 B compensation can land `DCAB` and expect a large raw win.

## 2. `extraAliens` coords — original has no `Vector sp` copy

Source (lines ~2060-2065):

```cpp
this->spawnPosition = this->player->getPosition();
this->spawnPosition.x += ...;
Vector sp = this->spawnPosition;
this->spawnPosition.y += ...;
this->spawnPosition.z += ...;
int coords[3] = {(int) sp.x, (int) sp.y, (int) sp.z};
```

The original converts `spawnPosition` **after all three increments**
(`0xb3dc6..0xb3de2`: loads `[sl,#396]`, `[sl,#400]`, and the z result), i.e.
`coords` = (x_new, y_new, z_new); the `sp` copy is an artefact of our base and
is semantically wrong.  Measured placements of `Vector sp` (all size 17,172):

| `sp` position | raw | aligned |
|---|---|---|
| after `x` (base) | **2982** | 1123 |
| after `y` | 3028 | 1123 |
| after `z` | 3066 | 1121 |
| no `sp` (use `spawnPosition`) | 3070 | 1110 |

Removing `sp` makes the coords block match the original exactly
(`0xb3dbe..0xb3dda`) but shifts the surrounding code.  The base placement is
the best raw.

## 3. `pirateFortress` turret — `setRotation` is scheduled after the 2nd rotate

Source (lines ~1988-1993):

```cpp
Vector p = MatrixRotateVector(rotation, Vector{t[3], t[4], t[5]});
v = p;
gun->geometry->setRotation(v);
v = MatrixRotateVector(rotation, Vector{t[0], t[1], t[2]});
gun->setPosition(Vector{base.x + v.x, base.y + v.y, base.z + v.z});
```

The original emits rotate1, `v = p`, `setRotation(v)`, rotate2 (`0xb331e`,
`0xb3328`, `0xb3330`, `0xb3348`).  Our build emits rotate1, `v = p`, rotate2,
`v = p`, **`setRotation(v)`** (`0xb3322..0xb3358`) — `setRotation` receives the
second rotation.  The IR order is correct (`v=rotate1; setRotation(v);
v=rotate2`); the reorder happens in SelectionDAG (`mir.log` after ISel already
shows `operator=(v,rotate2)` before the `setRotation` call).  Variants tried
(all 17,172 unless noted):

* `setRotation(p)` with `v = p` kept: raw 2986.
* `p = rotate2; v = p;` (two copies): size-fatal 17,224.
* separate `Vector q` for rotate2: raw 3220.
* drop `Vector p`, assign `v` directly twice: size-fatal 17,316.

## 4. Other measured non-wins (size 17,172 unless noted)

* `case 4` tail: swap `float opt44` / `float hunters` declarations (original
  loads `options[44]` after `getDifficulty`): raw 2985.
* `case 4` tail loop: drop the `Array<KIPlayer *> *enemies` local: size-fatal
  17,168; drop the `Globals *globals` local: raw 3087; swap the two locals:
  size-fatal 17,176.
* `case 183` convoy `stops`: swap `(*stops)[0]` / `(*stops)[2]`: raw 2986.
* `raid` loot block: move `spawnPosition.z += ...` before
  `setPosition(spawnPosition)` (the original's order): raw 2983, aligned 1093
  (confirms w13450).
* `case 9` setPosition order, `case 15`/`184` bodies, `case 183` switch arm
  order: already covered by w13427/w13450; not re-swept.

## 5. Reproduce

```sh
cd /home/debian/GalaxyOnFire2/wt/w13459
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
```

Expected: `raw=2982 aligned=1123 size=17172/17172`.

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w13459/`
(`try.sh`, `one.py`, `bytes.py`, `adiff.py`, `sweep_station.py`,
`Level.base.cpp`, `Level.base.ll`, `ourdis.txt`, `mir.log`).
