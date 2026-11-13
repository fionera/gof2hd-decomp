# Level::createMission — wave 191b w13773

Target: `_ZN5Level13createMissionEv` (0xaffd8, 17,172 B). Base master `9d4d7dd76`
(raw 2,635 / aligned 595). All variants below keep size 17,172 and `.rel.plt`
byte-identical.

## Landed: pirateFortress loop loads `gun` after the store

The `pirateFortress` turret loop originally read

```
KIPlayer *gun = (*this->enemies)[i];
(*this->enemies)[i] = this->createStaticObject(nullptr, 6774, true);
```

which hoists the `enemies[i]` reload *before* `createStaticObject`.  The
original instead stores first and only then reloads the slot into `gun`
(0xb340c..0xb3420):

```
b3408  blx  createStaticObject
b340c  ldr.w r1, [r5, #248]
b3410  str   r4, [sp, #116]        ; spill loop index i
b3412  ldr   r1, [r1, #4]
b3414  str.w r0, [r1, r4, lsl #2]  ; enemies[i] = obj
b3418  ldr.w r0, [r5, #248]
b341c  ldr   r1, [sp, #116]        ; reload i
b341e  ldr   r0, [r0, #4]
b3420  ldr.w r4, [r0, r1, lsl #2]  ; gun = enemies[i]
```

Moving the `KIPlayer *gun = ...` line below the assignment reproduces that
order.  Measured: raw **2,635 -> 2,641** (+6), aligned **595 -> 592** (-3),
size 17,172/17,172 exact, `.text` differing bytes 74,885 -> 74,891 (+6),
`.ARM.extab` 561 unchanged.


## Landed: case-2 route coordinates as a brace initializer

The original case-2 block evaluates the route coordinates in the order

```
coords[0] = -20000 - rnd->nextInt(20000);
coords[1] = 0;
coords[2] = -20000 - rnd->nextInt(20000);
coords[3] = coords[4] = coords[5] = 0;
```

(0xb16d4..0xb1724: `str r4,[sp,#316]` for `coords[1]` lands between the two
`nextInt` calls, then `strd r4,r4,[sp,#324]` / `str r4,[sp,#332]` for
`coords[3..5]`), and reads `goods` only after `enemyRoute = new Route(...)`.
Writing the six-element initializer in that order and moving the
`getProductionGoodAmount()` call below the `Route` construction reproduces it.

`asteroidPos` stays declared after `ArraySetLength`: moving it to the top of
the case makes the case-2 block instruction-exact but leaves the whole rest of
the function 2 B short (the case-1 first loop hoists `enemyRace` out of the
loop, where the original reloads it at 0xb1608), which costs raw +593 and
`.ARM.extab` +3.

Measured: raw **2,641 -> 2,681** (+40), aligned **592 -> 581** (-11), size
17,172/17,172 exact, `.text` 74,891 -> 74,931 (+40), `.ARM.extab` 561 -> 562
(+1).

## Measured variants (all size-exact unless noted)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base | 2,635 | 595 | 17,172 |
| **pirateFortress `gun` after store (landed)** | **2,641** | **592** | **17,172** |
| pirateFortress `Vector p` removed (`v = MatrixRotateVector(...)` both) | 16,661 | 2,114 | 17,312 (FAIL) |
| pirateFortress second rotate via named `q` temp | 2,879 | 638 | 17,172 |
| raid block: move `int guns = alienGuns + fortressGuns;` after the `if` | 4,165 | 596 | 17,164 (FAIL) |
| alienGuns tail reorder (`setPosition` before `shipGroup`) | 2,641 | 777 | 17,172 |
| plain `bigAlienWave ? A : B` ternary (7 spellings) | 4,425 | 786 | 17,156 (FAIL) |
| stationPirates `vC` reorder | 2,643 | 599 | 17,172 |
| case-2: `asteroidPos` top + `new Route` before `goods` | 3,270 | 593 | 17,172 |
| case-2: brace-init coords, `goods` after `Route` (landed) | 2,681 | 581 | 17,172 |
| case-2: brace-init coords + `asteroidPos` at top | 3,274 | 578 | 17,172 |
| case-2: brace-init coords, `goods` before `Route` | 2,662 | 589 | 17,172 |

## Remaining shape notes

* Original `pirateFortress` rotate output temp is `sp+212` for both rotations
  and is reused for the final position vector; ours uses `sp+212` for the first
  and `sp+200` for the second (plus an extra `sp+188` input temp).  Removing the
  named `Vector p` local makes both rotations direct temporaries but perturbs
  the whole-function register allocation (size 17,312), so `p` is load-bearing.
* Raid block: original computes `guns = alienGuns + fortressGuns` at the loop
  setup (0xb37e6 `add.w r5, sl, fp` from the two cached values), i.e. after the
  `raidRace == 8` branch.  Moving our `int guns` below the `if` matches that
  shape but lands 8 B short (17,164), so it needs a compensating change.
* Case-183 `return;` still merges with the case-4 tail epilogue
  (0xb20f6 `b.w b02ca` vs ours `b.w b41d8`); the jump-table entry at 0xb02ae
  differs by one halfword as a consequence.

## Landed: `stationPirates` store/evaluation order

The original `stationPirates` loop at `0xb3b00` evaluates, in order:

```text
new Route(coords, 3)
route->getWaypoint()
createStaticObject(waypoint, 14243, true)
store into (*this->enemies)[i]
route->getWaypoint()
getPosition()  -> pirateBase
(*this->enemies)[i]->setToSleep()
Globals::gameText->getText(441)
(*this->enemies)[i]->name = *...
```

The checked-in source read `route->getWaypoint()->getPosition()` into
`pirateBase` and called `setToSleep`/`name` on the *old* `(*enemies)[i]`
before the `createStaticObject` store.  Reordering the block to

```cpp
Route *route = new Route(coords, 3);
(*this->enemies)[i] = this->createStaticObject(route->getWaypoint(), 14243, true);
pirateBase = route->getWaypoint()->getPosition();
(*this->enemies)[i]->setToSleep();
(*this->enemies)[i]->name = *Globals::gameText->getText(441);
```

matches the Thumb order.  On today's HEAD (raw 2,681 / aligned 581) this
measures raw **2,689** (+8), aligned **574** (-7), insn 6204/6197, size
17,172/17,172 exact, `.text` 74,931 -> 74,939, `.ARM.extab` 562 unchanged.
(The 2,643/599 row in the table above was measured on the older 2,635 base.)

## Next clusters (post-landing)

* `0xb3b00`/`0xb3c00` — `stationPirates` escort loop (register roles still
  differ) and the `b3bfe..b3c48` `mov fp,sl; movw sl,#10000` shuffle.
* `0xb3a00` — pirate-fortress tail: original reuses `sp+212` for both
  rotations; ours uses `sp+212`/`sp+200`/`sp+188`.
* `0xb37e6` raid block: original computes `guns = alienGuns + fortressGuns`
  after the `raidRace == 8` branch; moving `int guns` below the `if` gives
  raw 4,165 / aligned 596 but size 17,164 — needs +8 B compensation.
* `0xb2a9e..0xb2d02` case-1 wanted loop: original uses `r1` as the loop
  counter and spills after `cmp`/`eor`/`orrs`; ours uses `r6` (pure
  RA/declaration-order artifact).

## Landed: `stationPirates` escort `Vector pos` inline initializer

The original escort loop interleaves the `pirateBase` component loads with
the `nextInt` calls (`b3be8 vldr s16,[sp,#312]` before the first `blx`,
`b3bfe vldr s18,[sp,#316]` before the second, `b3c2a vldr s24,[sp,#320]`
before the third).  Writing the three offsets inline in the braced
initializer

```cpp
Vector pos = {pirateBase.x + (float) (Globals::rnd->nextInt(20000) * (Globals::rnd->nextInt(2) == 0 ? 1 : -1) + 10000),
              pirateBase.y + (float) (...),
              pirateBase.z + (float) (...)};
```

instead of naming `ox`/`oy`/`oz` first reproduces that order and removes
the register shuffle.  Measured on the post-stationPirates HEAD
(raw 2,689 / aligned 574): raw **2,705** (+16), aligned **559** (-15),
insn 6204/6197, size 17,172/17,172 exact, `.text` 74,939 -> 74,955,
`.ARM.extab` 562 unchanged.

## Landed: case-4 hunter loop drops the `Globals *globals` local

The original case-4 tail keeps `this` in `r8` (`0xb40b6 ldr.w r8,[sp,#124]`)
and `Globals::globals` in `sl`, and emits `strd r0, fp, [sp]` at `0xb4198`
with `fp == 1`.  The checked-in source cached `Globals *globals =
Globals::globals;`, which pinned `this` in `sl` and `globals` in `r9`/`fp`.
Calling `Globals::globals->getRandomEnemyFighter(8)` directly (dropping the
local) lets clang put `this` in `r8` and restores the `strd`/`fp=1` shape.

Measured on the post-w13773 HEAD (raw 2,705 / aligned 559): raw **2,812**
(+107), aligned **519** (-40), size 17,172/17,172 exact, insn 6202/6197.
`.text` 74,955 -> 75,062 (+107); `.ARM.extab` 562 -> 587 (+25);
`.ARM.exidx` 0; `.rel.plt` byte-identical.  `check` RATCHET PASS
(byte_exact 4508 unchanged).

Remaining tail delta after this change: the loop index is in `sl` (original
`r9`) and `Globals::globals` in `fp` (original `sl`); the `enemies` local is
still cached in `r9` where the original reloads `this->enemies`.

## Measured variants (session 5; all keep `.rel.plt` byte-identical)

| variant | raw | aligned | size | `.text` | `.ARM.extab` |
|---|---:|---:|---:|---:|---:|
| HEAD (post-w13773) | 2,705 | 559 | 17,172 | 74,955 | 562 |
| **drop `globals` local (landed)** | **2,812** | **519** | **17,172** | **75,062** | **587** |
| drop `globals` + `Vector sp` removed (`sp2`) | 2,900 | 506 | 17,172 | 75,150 | 587 |
| drop `enemies` local (`t1`) | 2,862 | 527 | 17,168 (FAIL) | - | - |
| drop both locals (`t3`) | 2,865 | 447 | 17,168 (FAIL) | - | - |
| drop `enemies` + reload bound (`t5`) | 2,862 | 527 | 17,168 (FAIL) | - | - |
| drop `globals` + reload bound (`t4` == t3) | 2,865 | 447 | 17,168 (FAIL) | - | - |
| drop `globals` + `(*enemies)[i]` body (`t2c`) | 2,864 | 524 | 17,164 (FAIL) | - | - |
| explicit `return;` after nameLeader if/else | 11,834 | 1,654 | 17,196 (FAIL) | - | - |

`t3` is the best aligned variant (447) but is 4 B short: the shortfall is in
the exception landing-pad region (`0xb422e..0xb42d0`), where the original has
two extra `b.n` entries and ours has one extra `nop`.  It needs an exact +4 B
compensation to be landable.

## Landed: `extraAliens` reads `this->spawnPosition` directly

The original `extraAliens` block loads the coordinates straight from
`this->spawnPosition` (`vldr s2,[sl,#396]`, `vcvt`, `vstr [sp,#312]`) rather
than copying the vector into a local.  Dropping `Vector sp` and initializing
`int coords[3]` from `this->spawnPosition.{x,y,z}` fixes that shape.

Measured on the post-t2 HEAD (raw 2,812 / aligned 519): raw **2,900** (+88),
aligned **506** (-13), size 17,172/17,172 exact, insn 6202/6197.
`.text` 75,062 -> 75,150 (+88); `.ARM.extab` 587 unchanged; `.ARM.exidx` 0;
`.rel.plt` byte-identical.  `check` RATCHET PASS (byte_exact 4508 unchanged).

Remaining delta in this block: the original interleaves load/convert/store
(`vldr s2; vcvt; vstr` then y load+convert, z convert, z store, y store);
ours emits three converts then three stores (x, z, y order).
