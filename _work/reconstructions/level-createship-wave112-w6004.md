# Level::createShip — wave 112 (w6004): waypoint field order + spawn mapping

Status: **source change landed** (wave-112, 2026-11-05).
Target `_ZN5Level10createShipEiiiP8Waypointbb` (`Level::createShip`, original
2764 B @ `0xc0214`). Builds on `level-createship-wave111-realnew.md`.

## Result

```
                                   before (HEAD 5d98bd391)   after
createShip fuzzy                   66.3668                  66.6334
createShip size                    2764 B                   2764 B
.text size                         0x1337c8                 0x1337c8
.ARM.extab size                    0x26298                  0x26298
.rodata size                       0x41c27                  0x41c27
file size                          2,187,328 (exact)        2,187,328 (exact)
byte_exact / linked_exact          4500 / 4500              4500 / 4500
lint / drift                       CLEAN / 0                CLEAN / 0
```

The landed change is ordinary C++:

* Rename the waypoint locals to the real field mapping and fix the spawn
  expression, which had been scrambled:
  `wpX = waypoint->x`, `wpY = waypoint->y`, `wpZ = waypoint->z`, and
  `x = wpX + jitterX - 20000`, `y = (float)(wpY + jitterY - 20000)`,
  `z = (float)(wpZ + jitterZ - 20000)`. The old code assigned
  `wz = waypoint->x`, `wy = waypoint->y`, `wx = waypoint->z` and then used
  `jitterZ + jitterX` for x and `wz + wx` for z.
* Declare the waypoint locals `int wpY, wpZ, wpX;` (declaration order only;
  the assignments are unchanged). This is the register-allocation / stack-slot
  order the original uses.
* Declare the jitter locals in call order `jitterX, jitterY, jitterZ`.

## Measurement table (full link, official scorer)

| variant | fuzzy | size | extab |
|---|---|---|---|
| HEAD baseline | 66.3668 | 2764 | 0x26298 |
| spawn/field mapping only | 66.4336 | 2764 | 0x26298 |
| **mapping + decl `Y,Z,X` (landed)** | **66.6334** | 2764 | 0x26298 |
| decl `Z,Y,X` | 66.2338 | 2764 | exact |
| decl `Z,X,Y` | 66.0340 | 2764 | exact |
| wp ternary init | 66.0340 | 2764 | exact |
| wp init 0 + if | 66.6334 | 2764 | exact |
| `switch(type)` instead of if-chain | 64.7029 | 2768 | 0x26298 |
| placement-new only | 68.7159 | 2776 | 0x26284 |
| try-anchor only | 62.8686 | 2768 | 0x262a8 |
| **placement-new + try + switch** | **72.4926** | 2764 | **0x26290 (-8)** |
| placement-new + try + switch + decl `Y,Z,X` | 72.3932 | 2764 | 0x26290 |
| all-int coords | 62.7976 | 2792 | exact |
| coords before `new Player` | 58.9000 | 2772 | exact |
| coords right after jitter | 56.1473 | 2776 | exact |
| `empPoints` block hoisted | 61.2774 | 2768 | exact |
| cargo split (2c / 33 / 31) | 64.5709 / 64.6031 | 2768 | exact |
| outer `switch(shipClass)` | 41.8651 | 2768 | exact |
| two `if`s instead of else-if | 32.6693 | 2696 | exact |
| `PlayerFixedObject *ship` | 34.3797 | 2704 | 0x2629c |
| inverted `case 0` | 32.1275 | 1728 | 0x26200 |
| waypoint / jitter / float decl permutations | no change (66.4336) | | |

The 72.49 % shape (placement-new + try anchor + `switch(type)`) is the best raw
score but costs 8 B of `.ARM.extab` (`0x26290`), i.e. it regresses the
section-exact state. It was not landed. `Hud::draw` is +4 and
`Generator::createMission` is -4; a compensating pair would be needed to keep
the section exact.

## Structural findings

* Original 1007 insns, ours 995; both 2764 B.
* Original prologue: `str r1,[sp,#60]` (race), `mov fp,r3` (**type -> fp**),
  `mov r5,r2` (shipClass -> r5), GOT base -> `r4`. Ours:
  `strd r1,r2,[sp,#56]`, type -> `sl`, GOT -> `r6`. This early
  register-allocation difference dominates the fuzzy cost.
* Region scores (orig offsets): prologue `0x0-0x1b4` 64.74 %, spawn
  `0x1b4-0x478` 54.04 %, capital-ship volumes `0x478-0x918` 68.26 %, tail
  `0x918-0xacc` 92.31 %.
* Original spawn loads: `wp->y` -> sp48, `wp->x` -> sp44, `wp->z` -> sp40;
  jitter 1st -> sp36, 2nd -> sp32, 3rd -> sp28; spawn uses 1st for x, 2nd for
  y, 3rd for z (all ints computed, `vcvt` after `setEmpData`).
* `level-createship-pass6-w4264.md` documents that the target slot map and the
  `type -> fp` prologue are mutually exclusive in every faithful family swept
  (20k+ variants).

## Remaining `.ARM.extab` deltas

`Hud::draw` +4 (480 vs 476) and `Generator::createMission` -4 (168 vs 172)
cancel exactly; createShip's own record is 328 B at extab offset 45428 (orig
45432) and differs in 3 words (record+32/+96/+176, landing-pad call-site
offsets +4). Fixing the pair or the record offsets is out of scope here.
