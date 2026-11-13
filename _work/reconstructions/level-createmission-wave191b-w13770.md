# `Level::createMission` (`_ZN5Level13createMissionEv`) — wave 191b / w13770

Target `src/game/world/Level.cpp`, function VMA `0xaffd8`, size **17,172 B**
(`0x4314`). Branch `wave191b-w13770`, base master `c2b7cc227`. Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, armeabi-v7a, image base 0x10000).

Continues `level-createmission-wave191b-w13755.md` (case-2 loot tail),
`...w13749.md` (return relocation / `strd`), `...w13705.md` (case-2
coords/goods order).  Task: work the remaining mission cases in source
order, one Thumb block at a time, keeping size 17,172 exact.

## Landed change: `inPirateLootOrbit` setter order

The raid hostile loop's `inPirateLootOrbit` branch at `0xb3836` does, in
order:

```text
ldr   r5, [ship, #4]        ; p = ship->player
bl    Player::getMaxHitpoints
lsls  r1, r0, #1            ; * 2
bl    Player::setHitpoints
ldr   r0, [enemies, #data_]
...                         ; reload ship
ldr   r2, [r1, #28]         ; vtable setSpeed
blx   r2                    ; setSpeed(3.5f)
ldr   r0, [enemies, #data_]
...                         ; reload ship
movs  r1, #3
bl    PlayerFighter::setRotate
```

The checked-in source emitted `setRotate`, `setSpeed`, then the
`player`/`setHitpoints` pair.  Reordering the branch to

```cpp
Player *p = (*this->enemies)[i]->player;
p->setHitpoints(p->getMaxHitpoints() * 2);
(*this->enemies)[i]->setSpeed(3.5f);
((PlayerFighter *) (*this->enemies)[i])->setRotate(3);
```

matches the original call order.

## Measurement

All variants size 17,172/17,172 exact (measured this session).

| variant | raw | aligned | `.text` | `.ARM.extab` |
|---|---:|---:|---:|---:|
| baseline (`c2b7cc227`) | 2,627 | 605 | 74,717 | 561 |
| e1: case-2 `goods` after `new Route` | 2,646 | 597 | 74,736 | 562 |
| vC: `stationPirates` loop reorder alone | 2,635 | 609 | 74,725 | 561 |
| **vD: `inPirateLootOrbit` reorder alone (landed)** | **2,635** | **595** | **74,725** | **561** |
| e1+vC | 2,654 | 590 | 74,744 | 562 |
| e1+vD | 2,654 | 587 | 74,744 | 562 |
| vC+vD | 2,643 | 599 | 74,733 | 561 |
| e1+vC+vD | 2,662 | 580 | 74,752 | 562 |
| e5: case-2 coords/goods/asteroidPos full reorder | 3,260 | 591 | 75,350 | 565 |
| e5+vC+vD | 3,276 | 574 | 75,366 | 565 |

Landed **vD alone**: aligned 605 -> 595 (-10), raw 2,627 -> 2,635 (+8),
`.text` 74,717 -> 74,725 (+8), `.ARM.extab` 561 unchanged, `.rel.plt`
byte-identical.  The larger aligned wins (e1+vC+vD -> 580) cost raw +35,
`.text` +35 and `.ARM.extab` +1, so they were left for a later pass.

Rejected (not size-exact or no effect):

* e4 `(bigAlienWave - (i < 0) * 4) ? ...` -> `bigAlienWave ? ...`:
  size 17,156 (-16), not landable.
* e6 `int race = localRace;` in the jumper loop: byte-identical to base.
* e6c `const int &race = localRace;`: byte-identical to base.
* e6b `int race = SolarSystem::getRace();`: size 17,180 (+8), rejected.
* e6d `volatile` cast: size explodes, rejected.

## Remaining call-order deltas (after vD)

`calldiff.py` (810 calls each side) still reports:

* `0xb171c` / `0xb173a` — case-2 `getProductionGoodAmount` position: ours
  calls it one slot early.  `e1` fixes the call order but costs raw +19 /
  `.text` +19 / `.ARM.extab` +1.
* `0xb3894..0xb3912` — `blx r5` vs `blx r6` for the indirect `nextInt`
  calls; register naming only (pure RA), no call-order change.
* `0xb3b24..0xb3b86` — `stationPirates` loop: the original emits
  `getStation; getPirateStationIndex; new Route; getWaypoint;
  createStaticObject; store; getWaypoint; getPosition; Vectora;
  setToSleep; getText; Stringa`; ours emits `setToSleep/getText/Stringa`
  before `getWaypoint/createStaticObject`.  `vC` fixes the call order but
  costs aligned +14 when applied alone (609 vs 595).

Known structural deltas from `...w13749.md` remain: the missing
`b.w b02ca` at `b40b2` (return relocation, +24 size) and the missing
`strd r0,fp,[sp]` at `b4198` (`this` kept in `sl` vs the original `r8`).

## Reproduce

```sh
cd /home/debian/GalaxyOnFire2/wt/w13770
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
```

Expected: `raw=2635 aligned=595 size=17172/17172 insn=6205/6197`.
