# `Level::createShip` — wave 151 / worker w9616 (continuation)

Target `_ZN5Level10createShipEiiiP8Waypointbb`, original 2,764 B @ `0xc0214`.
Baseline (wave-150 master `db9658a51`): raw same-address diff **2,056**,
instruction-index alignment **480**, size 2,764.

## Mission

Wave 150 (w9616) established that the original entry map is
`status r4, rnd r5, nextInt r6`, while ours was `status r6, rnd r5,
nextInt r4`, and that several source shapes *do* reach `status r4`
(`do/while`, `expect(shipClass==0,1)`, `empPoints`-before-`hitpoints`) but
none at the exact 2,764 B size.  This pass searched for a size-exact
variant and reports its entry map.

## Landed: `empPoints` before `hitpoints` + `expect(shipClass==1,1)`

```cpp
    int level = Globals::status->getLevel() > 20 ? 20 : Globals::status->getLevel();
    int empPoints = Globals::status->getLevel() > 20 ? 20 : Globals::status->getLevel();
    empPoints = empPoints * 5 + 40;
    int hitpoints = (Globals::status->gameWon() ? 180 : mission * 4) + level * 14 + 20;
    ...
    if (__builtin_expect(shipClass == 1, 1)) {
```

This is the **only** source shape found at exactly 2,764 B that puts
`Globals::status` in r4 and `AERandom::nextInt` in r6.

| variant | size | raw | aligned | status / nextInt |
|---|---:|---:|---:|---|
| HEAD (`wy wz wx`) | 2,764 | **2,056** | **480** | r6 / r4 |
| **landed (`empfirst` + `expect(sc1e,1)`)** | **2,764** | 2,146 | 501 | **r4 / r6** |

The raw and aligned both rise: the `empPoints`/`hitpoints` swap is not the
original's binary order (the original computes `hitpoints` first, then
`empPoints` at `c0314`), so the aligned metric penalises the reorder even
though the *register map* is now the original's.  This is a deliberate
trade: the requested RA state is reachable at the exact size.  Gate stays
green (`byte_exact 4507`, `linked_exact 4507`, size 4524/4524, data
843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370).  All 27
section sizes unchanged.

## Entry register map (requested)

Original (2,764 B, `0xc0214`):

| value | reg | note |
|---|---|---|
| `this` | r9 | |
| `type` | fp (r11) | `mov fp,r3` before the mission call |
| `shipClass` | r5 → sl → r6 | `mov r5,r2`, `mov sl,r5` after the call, `mov r6,sl` at `c02b4` |
| `race` | `[sp,#60]` | `str r1,[sp,#60]` |
| `Status *` | **r4** | `ldr r4,[r0,#0]`; vtable reloaded `ldr r0,[r4,#0]` |
| `AERandom *` | r5 | |
| `&nextInt` | **r6** | dies at `c0282`, r6 reused for shipClass |
| `mission` | r8 | |
| `jitterX` | `[sp,#36]` | all three jitters spilled |

Landed (2,764 B):

| value | reg | note |
|---|---|---|
| `this` | r9 | same |
| `type` | sl (r10) | `mov sl,r3` (original: fp) |
| `shipClass` | `[sp,#60]` | `strd r1,r2,[sp,#56]` spills race+shipClass (original: register) |
| `race` | `[sp,#56]` | (original: `[sp,#60]`) |
| `Status *` | **r4** | `ldr r4,[r0,#0]` — **matches** |
| `AERandom *` | r5 | matches |
| `&nextInt` | **r6** | `ldr r6,[r1,#0]` — **matches** |
| `mission` | r8 | matches |
| `jitterX` | fp (r11) | (original: `[sp,#36]`) |

So the requested `status r4 / rnd r5 / nextInt r6` rotation is reproduced;
the residual map difference is `type` (sl vs fp) and `shipClass`
(spilled vs r5→sl→r6), i.e. the same shipClass-spill-weight problem wave
150 identified.

## Sweep results (all full link, exact TU)

### `empPoints`-before-`hitpoints` + one `__builtin_expect`

| expect site | size | raw | aligned | status / nextInt |
|---|---:|---:|---:|---|
| none | 2,760 | 2,026 | 504 | r4 / r6 |
| `mission==0x9a,0` | 2,768 | 2,393 | 553 | r4 / r6 |
| `mission==0x9a,1` | 2,760 | 2,024 | 504 | r4 / r6 |
| **`shipClass==1,0`** | **2,764** | 2,277 | 517 | **r4 / r6** |
| **`shipClass==1,1`** | **2,764** | 2,146 | 501 | **r4 / r6** |
| `shipClass==0,0` | 2,708 | 2,409 | 1,428 | r4 / r6 |
| `shipClass==0,1` | 2,748 | 2,527 | 1,321 | r4 / r6 |
| `mission 49..56` | 2,784 | 2,411 | 536 | r4 / r6 |
| `missionPtr`, `type==0x2c||0x31` | 2,760 | 2,026 | 504 | r4 / r6 |

Adding `expect(mission==0x9a,1)`, `expect(sc1c,1)`, `expect(missionPtr,*)`
or `expect(type==0x2c||0x31,*)` on top of the landed variant coalesces to
the same 2,146 / 501 body.  `expect(sc1e,1)` + `expect(mission 49..56)`
gives 2,788 / 535.

### `wy/wz/wx` declaration permutation on the landed shape

| order | size | raw | aligned | status / nextInt |
|---|---:|---:|---:|---|
| `wy wz wx` (landed) | 2,764 | 2,146 | 501 | r4 / r6 |
| `wz wy wx` | 2,764 | 2,145 | **498** | r4 / r6 |
| `wy wx wz` | 2,764 | 2,144 | 500 | r4 / r6 |
| `wz wx wy` | 2,764 | 2,161 | 503 | r4 / r6 |
| `wx wy wz` | 2,764 | 2,161 | 505 | r4 / r6 |
| `wx wz wy` | 2,764 | 2,146 | 501 | r4 / r6 |

`wz wy wx` is 3 aligned better but changes the wave-150 landed spill-slot
choice; the landed permutation was kept to minimise the diff.

### `do/while` family (all `status r4 / nextInt r6`)

| variant | size | raw | aligned |
|---|---:|---:|---:|
| `do/while` | 2,788 | 2,417 | 705 |
| `+ expect(sc1c,1)` | 2,740 | 2,479 | 1,100 |
| `+ expect(sc0,1)` | 2,760 | 2,497 | 1,426 |
| `+ expect(sc1e,0)` | 2,776 | 2,459 | 731 |

None is 2,764 B.  `do/while` also re-rolls the whole map (`this`+`race`
spilled, `type` sl, `shipClass` r8, `mission` r9).

### Null-guard family (wave-148 lever, new placements)

`if (Globals::status == nullptr) …` inserted after the coordinate block
gives `status r4 / nextInt r6` with the **best alignment seen** but
overshoots the size:

| guard body (after coords) | size | raw | aligned |
|---|---:|---:|---:|
| `return nullptr;` | 2,788 | 2,470 | **462** |
| `group = false;` | 2,792 | 2,442 | **428** |
| `hostile = false;` | 2,772 | 2,423 | 508 |
| `empPoints = 0;` | 2,768 | 2,412 | 505 |
| `type = 0;` | 2,780 | 2,541 | 1,078 |
| `race = 0;` | 2,760 | 2,544 | 1,067 |

`group = false` after coords is the best aligned status-r4 shape found
(428, i.e. -52 vs baseline) but costs +28 B.  Shaving exactly 24–28 B
from that shape without disturbing the RA is the obvious next lever.
Guard placement matters: after `setEmpData` the same `group = false`
guard gives 2,788 / 431; after `armor` 2,796 / 502; before the coords
the guard does **not** produce r4.

### Declaration-hoist / local-copy levers (all coalesce to 2,056 / 480)

Hoisting `int empPoints;`, `int hitpoints;`, `int armor;`, `int empTime;`,
or introducing `int sc = shipClass;`, `int rc = race;`, `int ty = type;`
all produce the identical baseline body (GVN/coalescing).

## Conclusion / open item

`status r4 / rnd r5 / nextInt r6` **is reachable at exactly 2,764 B** with
`empPoints`-before-`hitpoints` + `expect(shipClass==1,1)`; the landed
variant is that shape.  The full original map (`type` fp, `shipClass`
r5→sl→r6, `race` `[sp,#60]`) still requires the shipClass spill weight to
rise; the null-guard-after-coords lever does it at 2,788–2,792 B with
aligned 428–462, so the remaining task is a size-neutral producer of that
weight.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9716/fdiff.py _ZN5Level10createShipEiiiP8Waypointbb
python3 /dev/shm/w9716/align_diff.py _ZN5Level10createShipEiiiP8Waypointbb
```

Scratch `/dev/shm/w9716/`: `trymod.py` (marker-based body splice + build +
score + entry-map extractor), `gen.py` (empfirst / do-while generators),
`sweep2.py` … `sweep13.py` (expect / guard / placement sweeps), `sweep*.log`.
