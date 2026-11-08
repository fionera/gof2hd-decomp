# MGame::OnTouchEnd — wave 151 (w9814): asteroid-owned `dockToAsteroid` + autopilot-owned `hudEvent(6)`

Status: source change landed, 2026-11-08. Worktree `/home/debian/GalaxyOnFire2/wt/w9814`,
base `9d491d46a` (wave 151 final). Owned TU `src/game/menu/MGame.cpp` (`OnTouchEnd` only).
Scratch `/dev/shm/w9814`. Full `check` green before and after: byte **4507/4524**,
linked **4507/4524**, size **4524/4524**, data **843/843**, imports **207**, lint CLEAN,
drift 0, RATCHET PASS. Target `_ZN5MGame10OnTouchEndEiiPv`, original 8080 B @ `0x17a144`.

| | base (wave 151, `expect(stream,1)`) | this wave |
|---|---:|---:|
| size | 8080 | **8080** |
| raw same-address diff | 4392 | **4425** |
| aligned (align_diff) | 904 | **924** |
| shared `blx dockToAsteroid` owner | stream (wrong) | **asteroid (right)** |
| shared `blx hudEvent(6)` owner | stream (wrong) | **autopilot (right)** |
| `.text` differing bytes | 87,271 | **87,304** |

## 1. The finding: the two tail merges are independent

The original has two independent tail merges in the `hudTouchFlags & 0x40` dock chain:

* `dockToAsteroid` is shared by the asteroid arm and the stream arm, and the merged
  `blx` is **owned by the asteroid arm** (`0x17bcf4`, asteroid arg setup falls through).
* `hudEvent(6)` is shared by the autopilot arm and all three dock arms, and the merged
  block is **owned by the autopilot arm** (`0x17bb50`, immediately after `setAutoPilot`).

The wave-151 base (`expect(isDockingToStream(), 1)`) puts both merged blocks at the
stream arm. w9714's `expect(isDockingToAsteroid(), 1)` puts both at the asteroid arm.
Neither reproduces the original's split ownership.

## 2. The landed shape

Move the branch-weight hint from the stream test to the docking-point test and keep the
asteroid target in a named local:

```cpp
if (this->player->isDockingToAsteroid()) {
    KIPlayer *asteroidTarget = this->radar->getLockedAsteroid();
    this->player->dockToAsteroid(asteroidTarget, this->radar);
    this->hud->hudEvent(6, this->player, 0);
} else if (__builtin_expect(this->player->isDockingToDockingPoint(), 1)) { // lint: arm_builtin ...
    this->radar->lockedEnemy = nullptr;
    this->player->dockToDockingPoint(nullptr, this->radar);
    this->hud->hudEvent(6, this->player, 0);
} else if (this->player->isDockingToStream()) {
    this->player->dockToAsteroid(this->radar->lockedStation, this->radar);
    this->hud->hudEvent(6, this->player, 0);
}
```

The `expect(dock, 1)` makes the docking-point arm the hot successor, which flips the
`dockToAsteroid` merge to the asteroid arm while leaving the `hudEvent(6)` merge at the
autopilot arm.  The named `asteroidTarget` local is what keeps the size at exactly 8080
B: without it the same hint produces 8084 B (the asteroid arm grows by the `blx` and the
literal pool moves +4).

Detected owners (`/dev/shm/w9814/owner.py`, region >= `0x17b800`):

```
ours (landed)                         original
dockToAsteroid owner: asteroid@17bc66 dockToAsteroid owner: asteroid@17bcf4
hudEvent(6) owner:    autopilot@17baec hudEvent(6) owner:    autopilot@17bb5c
```

## 3. Variants measured (all full-function, `check`-clean unless noted)

| variant | size | raw | aligned | dock owner | hudEvent owner |
|---|---:|---:|---:|---|---|
| base wave 151 (`expect(stream,1)`) | 8080 | 4392 | 904 | stream | stream |
| **landed (`expect(dock,1)` + local)** | **8080** | **4425** | **924** | **asteroid** | **autopilot** |
| `expect(dock,1)` no local | 8084 | 4668 | 912 | asteroid | autopilot |
| `expect(dock,1)` + local + `expect(stream,1)` | 8080 | 4433 | 927 | asteroid | autopilot |
| `expect(ast,1)` (w9714 shape) | 8080 | 4400 | 923 | asteroid | asteroid |
| no hint (w9614 `ast/dock/stream`) | 8084 | 4406 | 916 | stream | autopilot |
| no hint, `stream/ast/dock` (w9614) | 8080 | 4419 | 917 | asteroid | asteroid |
| `stream/ast/dock` + `expect(dock,1)` | 8080 | 4420 | 922 | asteroid | dockpoint |
| `ast/stream/dock` + `expect(dock,1)` | 8080 | 4386 | 913 | stream | dockpoint |
| `stream/ast/dock` + `expect(stream,1)` | 8080 | 4384 | 926 | stream | other |
| common `dockTarget` after chain + `expect(stream,1)` | 8108 | 4732 | 967 | after chain | — |
| `goto doDock` after chain + `expect(stream,1)` | 8076 | 4678 | 973 | after chain | — |
| `expect(auto,1)` + `expect(ast,1)` | 8076 | 4686 | 938 | asteroid | autopilot |
| `expect(auto,1)` alone | 8076 | 4684 | 935 | asteroid | autopilot |

The `expect(autopilot, 1)` variants keep the right owners but move the outer `else`
(the whole docking chain) out of line and lose 4 bytes (8076 B).

## 4. Honest assessment

The mission target — asteroid-owned `dockToAsteroid` **and** the wave-151 metric pair
(4392 / 904) — was not reached.  The landed shape is the only measured 8080 B variant
whose **two** shared-block owners both match the original; it pays **+33 raw / +20
aligned** for that.  The raw cost is concentrated in the campaign-144 cold cluster: the
asteroid arm now carries the `blx dockToAsteroid` (as the original does), so the cluster
starts `+4` bytes later, and the literal pool moves from `0x17bf64` to `0x17bf84`
(the local-variable form costs `+0x1c` in the asteroid arm).  The best asteroid-owned
raw shape remains w9714's `expect(ast,1)` at 4400/923; the best raw shape overall
remains the base at 4392/904, but it has the wrong merge owners.

The `0x17adc0` `initHudMenu(1)/(2)` tail merge is a second, unrelated ownership flip
(original owns the shared `blx` at the `(2)` site `0x17afde`, ours at the `(1)` site
`0x17adca`) and is responsible for the 4146-byte shifted run that dominates the raw
metric.  A `__builtin_expect(hudTouchFlags & 0x400, 1)` there re-lays the whole function
to 8016 B (6072 / 1161), so it was not touched.

## 5. Reproduction

```
cp /dev/shm/w9814/final.cpp src/game/menu/MGame.cpp
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9814/fdiff.py _ZN5MGame10OnTouchEndEiiPv      # 4425 / 8080
python3 /dev/shm/w9814/align_diff.py _ZN5MGame10OnTouchEndEiiPv  # 924
cmake --build cmake-build-match --target check -j6               # RATCHET PASS
```

Scratch: `/dev/shm/w9814/{fdiff.py,align_diff.py,owner.py,runs.py,sweep.py,
sweep2.py,sweep3.py,sweep4.py,sweep5.py,sweep6.py,sweep7.py,sweep8.py,
gen.py,v_locals.py,base.cpp,final.cpp}`.  The authoritative binary is
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
