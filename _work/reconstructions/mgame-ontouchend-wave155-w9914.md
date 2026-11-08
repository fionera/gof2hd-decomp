# MGame::OnTouchEnd — wave 155 (w9914): both tail-merge owners correct; the 4391/903 pair is not reachable from the dock chain

Status: source change landed, 2026-11-08. Worktree `/home/debian/GalaxyOnFire2/wt/w9914`,
base `4a6104a43` (wave 152 final). Owned TU `src/game/menu/MGame.cpp` (`OnTouchEnd` only).
Scratch `/dev/shm/w9914`. Full `check` green before and after: byte **4507/4524**,
linked **4507/4524**, size **4524/4524**, data **843/843**, imports **207**, lint CLEAN,
drift 0, RATCHET PASS. Target `_ZN5MGame10OnTouchEndEiiPv`, original 8080 B @ `0x17a144`.

| | base (wave 152) | w9814 (wave 151 base) | this wave |
|---|---:|---:|---:|
| size | 8080 | 8080 | **8080** |
| raw same-address diff | 4391 | 4425 | **4424** |
| aligned (align_diff) | 903 | 924 | **923** |
| shared `blx dockToAsteroid` owner | stream (wrong) | asteroid (right) | **asteroid (right)** |
| shared `blx hudEvent(6)` owner | stream (wrong) | autopilot (right) | **autopilot (right)** |

(The w9814 note measured on the wave-151 base; re-measured on wave-152 it is 4424/923,
i.e. the wave-152 `count >= 0` change is worth exactly the 1 raw byte.)

## 1. Landed shape

The w9814 shape, rebased onto wave 152 and reformatted (`} else if` on one line):

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

Owner detection (`/dev/shm/w9914/owner3.py`, walks back from the shared `blx` to the
previous branch and classifies the fall-through block):

```
orig  dockToAsteroid owner ast@0x17bcf4   hudEvent owner auto@0x17bb5c
ours  dockToAsteroid owner ast@0x17bc66   hudEvent owner auto@0x17baec
```

## 2. Why the 4391/903 pair is not reachable from the dock chain

The raw metric is dominated by the *whole* divergent region, not the dock chain.  The
two streams re-synchronise at `0x17bf7e` (literal pool, shift 0).  Region breakdown for
the base:

| region | bytes | differing |
|---|---:|---:|
| `0x17a144..0x17adc0` (pre) | 3196 | 114 |
| `0x17adc0..0x17bb3a` (pre-dock) | 3450 | 3277 |
| `0x17bb3a..0x17bf7e` (dock) | 1092 | 986 |
| `0x17bf7e..end` (post) | 342 | 14 |

The first divergence is the `initHudMenu(1)`/`initHudMenu(2)` tail merge at `0x17adc0`:
the original owns the shared block at the `(2)` site (`0x17afde`), ours at the `(1)`
site (`0x17adca`).  That single ownership flip reorders the whole 4.5 KB cold region,
so almost every byte between `0x17adc0` and `0x17bf7e` differs regardless of the dock
chain shape.  The dock chain can only move the last ~1 KB, and its cost is the
coincidental same-address byte matches (base 106 matching bytes, w9814 78).

Swapping the `hudTouchFlags & 0x200` / `& 0x400` source arms (semantically identical,
mutually exclusive bits) does **not** flip the `initHudMenu` owner: 4394/906.

## 3. New hypotheses measured (all size 8080 unless noted)

All are `ast/dock/stream` source order unless stated.  "owners" = dockToAsteroid /
hudEvent.

| variant | raw | aligned | owners |
|---|---:|---:|---|
| base `expect(stream,1)`, no local | **4391** | **903** | stream / stream |
| **landed: `expect(dock,1)` + local** | **4424** | **923** | **ast / auto** |
| `expect(dock,1)`, no local | 4667 | 911 | ast / auto |
| `expect(dock,1)+expect(stream,1)`, no local | 4669 | 906 | ast / auto |
| `expect(dock,1)`, local, inner `expect(!_bc8,1)` | 4406 | 940 | ast / dock |
| `expect(auto,1)` + local | 4424 | 953 | ast / auto |
| common `dockToAsteroid` after the if-chain (`doDock` flag) | 4660 | 951 | stream / auto |
| common `hudEvent` after the if-chain (`doDock`, single call) | 4703 | 960 | stream / stream |
| `goto` join, both calls (`doDock`/`doHudEvent`) | 4705 | 976 | — |
| `goto` join, both calls, `expect(dock,1)` | 4636 | 1007 | ast / auto |
| `goto` join, hudEvent only | 4673 | 926 | stream / auto |
| `goto` join, hudEvent only, `expect(dock,1)` | 4646 | 971 | ast / auto |
| `goto` join, dockToAsteroid only, `expect(dock,1)` | 4513 | 973 | ast / auto |
| `stream/dock/ast`, no hint, local | 4403 | 956 | ast / auto |
| `dock/stream/ast`, no hint, local | 4408 | 964 | ast / auto |
| `initHudMenu(1)/(2)` arms swapped, `expect(stream,1)` | 4394 | 906 | stream / stream |
| `initHudMenu` arms swapped, `expect(dock,1)` + local | 4427 | 926 | ast / auto |
| outer `!isDockedToDockingPoint && !isLandingOrTakingOff` hinted hot | 4889 | 1851 | ast / dock |
| outer guard hinted cold | 4734 | 1840 | ast / auto |
| `PlayerEgo *player = this->player;` local through the chain | 5312 | 1091 | ast / auto |
| local declared at chain top (`nullptr`/uninit) or `const` | 4424 | 923 | ast / auto |

`expect(dock,0)` and `expect(!cond,1)` forms, per-arm hint-value combinations, and every
permutation of the three tests were also swept (~150 builds); the Pareto frontier for
correct owners is `(4403, 956)`, `(4424, 923)`, `(4667, 911)`, `(4669, 906)`.  The
landed `(4424, 923)` is the only point that keeps the original test order
(asteroid → dockingPoint → stream) and the original 8080 B.

## 4. Honest assessment

Both independent tail-merge owners are now faithful (asteroid owns `dockToAsteroid`,
autopilot owns `hudEvent(6)`), which was the wave's mission.  The w9813 metric pair
(4391/903) was **not** recovered: the dock chain cannot pay for it because the raw
metric is dominated by the unrelated `initHudMenu(1)/(2)` ownership flip at `0x17adc0`,
and the aligned metric by the `failed == 1` / `hasWon` region (wave-153 note §5).  The
price of the faithful owners is **+33 raw / +20 aligned** over the base, and the landed
shape is Pareto-optimal among the ~150 measured correct-owner shapes.

Next lever (not this wave): flip the `initHudMenu` merge owner without re-laying the
function — the original owns it at the `(2)` site, ours at `(1)`.  A hint there re-lays
to 8016 B (6072/1161), so the source shape needs a different anchor.

## 5. Reproduction

```
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9914/fdiff.py _ZN5MGame10OnTouchEndEiiPv      # 4424 / 8080
python3 /dev/shm/w9914/align_diff.py _ZN5MGame10OnTouchEndEiiPv  # 923
python3 /dev/shm/w9914/owner3.py                                 # ast / auto
cmake --build cmake-build-match --target check -j6               # RATCHET PASS
```

Scratch: `/dev/shm/w9914/{fdiff.py,align_diff.py,owner3.py,harness.py,sweep1..3.py,
custom.py,custom2.py,custom3.py,custom4.py,regiondiff.py,diffruns.py,pair.py,base.cpp}`.
The authoritative binary is
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
