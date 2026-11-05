# Level::createShip — wave 119 (w6705): real-`new` 336-byte LSDA, no score win

Status: **analysis only, no source change** (2026-11-05).
Target `_ZN5Level10createShipEiiiP8Waypointbb` (`Level::createShip`,
original 2764 B @ `0xc0214`, Thumb entry `0xc0215`). Branch `wave119-w6705`,
base `47a1396bd`.

## Result

HEAD (wave-117 placement-new + `try`/`catch(...)`) is unchanged:

```
createShip fuzzy (raw)            73.500000
createShip size                   2764 B
createShip LSDA record            328 B (original: 336 B)
.ARM.extab size / addr            0x26290 @0x1a3b78 (original 0x26298)
byte_exact / linked_exact         4503 / 4503
imports / data / lint / drift     207 / 843 / CLEAN / 0
```

`check`: **RATCHET PASS**.

The faithful real-`new` body below reproduces the original's 336-byte LSDA
record exactly (same call-site count, same empty action table, same
`.ARM.extab` size `0x26298`), but the cleanup landing pads keep the
allocated pointer live across each ctor and flip Greedy RA from the
original's prologue to `strd r1,r2,[sp,#56]`.  The all-int (semantically
faithful) body is 2788 B and **fails the gate** (`byte_exact` 4503 ->
2372, because the 24 B size delta shifts every later function's literal
pool).  The gate-safe 336-byte shape is the mixed `int x / float z /
float y` variant at **66.7 %** (`byte_exact` stays 4503).  Every 336-byte
shape found this wave scores 61.3-66.7; every shape that scores > 70 keeps
the 328-byte record.  No variant scored > 73.5.

## The original LSDA (parsed from `.ARM.extab` @ `0x1ab09c`)

```
personality word   0x7feb3ac4 (prel31 -> __gxx_personality_v0)
unwind opcodes     c94d9702 f0800082 b0b0b0ab   (12 bytes)
LSDA @ record+16:
  LPStart enc      0xff  (omit)
  TType enc        0xff  (omit)          <- no type table at all
  call-site enc    0x01  (uleb128)
  call-site len    315
  call sites       57, every action == 0 (cleanup only)
  action table     empty
  record           16 + 5 + 315 = 336
```

28 of the 57 call sites carry a non-zero landing pad: the 24
`new BoundingAAB(...)` ctors plus the `new Player`, `new PlayerFixedObject`,
`new PlayerFighter` and `new Array` ctors. The four shared delete+`_Unwind_Resume`
stubs are at `0xc0bba` (deletes `sl`), `0xc0bc0` (`fp`), `0xc0bc6` (`r9`),
`0xc0bcc` (`r5`); the 24 `b.n` trampolines at `0xc0b8a..0xc0bb8` route the
BoundingAAB cleanups to them. So the original really uses real `new` for all
three ship objects (`blx _Znwj` at `0xc03a4` Player, `0xc0408`
PlayerFixedObject, `0xc0448` Array, `0xc06bc` PlayerFighter).

HEAD's record is 328 B because it has only 54 call sites (the
PlayerFixedObject and PlayerFighter ctors are placement-new, so their
cleanup entries are absent) and pays the catch-all overhead (TType enc
`0x00`, ttbase 308, cslen 299, action table `01 00 00 00`).

## Original register map (Thumb, from the disassembly)

```
r4   status -> hardCoreMode result
r5   shipClass -> rnd -> level/hitpoints -> player
r6   nextInt fn ptr -> shipClass -> race -> this
r8   mission -> z
r9   this -> empTime -> ship/fixed
sl   shipClass -> empPoints -> fighter
fp   type -> hitpoints -> x -> volumes
sp#60 race   sp#56 this   sp#52 type
sp#44 wpX    sp#48 wpY    sp#40 wpZ
sp#36 jitterX sp#32 jitterY sp#28 jitterZ
```

Coordinates are **`int x, y, z`**, computed between the `Player` ctor and
`setEmpData` (`0xc03c8..0xc03e4`), and converted with `vcvt.f32.s32` at
each ctor call site (3 conversions in each branch). `x` lives in `fp`,
`y` in `r4`, `z` in `r8`; the BoundingAAB loop reloads them from
`s20/s18/s16` into `sl/r8/r4` per call.

## Faithful real-`new` body (all-int coords; 336 B, 62.7 %, 2788 B, gate FAIL)

This is the closest source shape to the original: real `new` for all three
ship objects, no `try`/`catch`, all-int coordinates computed before
`new Player` (which the scheduler keeps between the ctor and `setEmpData`,
matching the original).  It makes `.ARM.extab` size- and base-exact at
`0x26298`, but the 24 B of wider Thumb2 encodings (the `mov.w sl,#0` /
`str.w sl,[sp]` BoundingAAB zero argument and the `ldr.w r9` reloads)
push the function to 2788 B and the repo-wide `byte_exact` down to 2372.

```cpp
PlayerFixedObject *Level::createShip(int race, int shipClass, int type, Waypoint *waypoint, bool hostile, bool group) {
    int mission = Globals::status->getCurrentCampaignMission();
    int wpZ, wpX, wpY;
    if (waypoint != nullptr) {
        wpX = waypoint->x;
        wpY = waypoint->y;
        wpZ = waypoint->z;
    } else {
        wpX = 0;
        wpY = 0;
        wpZ = 0;
    }
    int jitterY = Globals::rnd->nextInt(40000);
    int jitterZ = Globals::rnd->nextInt(40000);
    int jitterX = Globals::rnd->nextInt(40000);
    int level = Globals::status->getLevel() > 20 ? 20 : Globals::status->getLevel();
    int hitpoints = (Globals::status->gameWon() ? 180 : mission * 4) + level * 14 + 20;
    if (type == 0x2c) {
        hitpoints = (int) ((float) hitpoints * 2.25f);
    } else if (type == 0x31) {
        hitpoints = (int) ((float) hitpoints * 17.0f);
    } else if (type == 0x33) {
        hitpoints = (int) ((float) hitpoints * 1.7f);
    }
    if (mission == 49 || mission == 50 || mission == 51 || mission == 52 || mission == 56) {
        hitpoints = 270;
    }
    int empPoints = (Globals::status->getLevel() > 20 ? 20 : Globals::status->getLevel()) * 5 + 40;
    int empTime;
    if (shipClass == 1) {
        hitpoints *= type == 14 ? 25 : 5;
        empPoints *= 3;
        empTime = 45000;
    } else {
        empTime = 15000;
    }
    hitpoints = (int) ((float) hitpoints + (*reinterpret_cast<float *>(&Globals::options[0x2c]) - 0.5f) * (float) hitpoints);
    if (mission == 0x9a) {
        hitpoints <<= Globals::status->inAlienOrbit() & (race == 9);
    }
    int armor = Globals::status->hardCoreMode() ? 650 : 1000;
    int x = wpX + jitterX - 20000;
    int y = wpY + jitterY - 20000;
    int z = wpZ + jitterZ - 20000;
    Player *player = new Player(armor, hitpoints, 1, 1, 0);
    player->setEmpData(empPoints, empTime);

    KIPlayer *ship = nullptr;
    if (shipClass == 0) {
        PlayerFighter *fighter = new PlayerFighter(type, race, player, nullptr, x, y, z, hostile);
        fighter->setShipGroup(Globals::globals->getShipGroup(type, race, group), type, hostile);
        if (missionPtr != 1 && missionPtr != 23) {
            lodManager->addObject(fighter->parentGeometry != nullptr ? fighter->parentGeometry : fighter->geometry);
        }
        switch (type) {
        case 0x2c:
        case 0x31:
            delete fighter->cargo;
            fighter->cargo = nullptr;
            break;
        case 0x33:
            fighter->field_0x25 = 0;
            break;
        }
        ship = fighter;
    } else if (shipClass == 1) {
        // capital ships: hull collision boxes per race
        PlayerFixedObject *fixed = new PlayerFixedObject(type, race, player, nullptr, x, y, z);
        Array<BoundingVolume *> *volumes = new Array<BoundingVolume *>();
        switch (race) {
            case 0:
                if (type == 14) {
                    ArraySetLength(11, *volumes);
                    (*volumes)[0] = new BoundingAAB(x, y, z, 0.0f, -2240.0f, 21608.0f, 760.0f, 640.0f, 2890.0f);
                    (*volumes)[1] = new BoundingAAB(x, y, z, 0.0f, 18392.0f, -2342.0f, 1260.0f, 3560.0f, 3670.0f);
                    (*volumes)[2] = new BoundingAAB(x, y, z, 0.0f, -786.0f, 8926.0f, 7410.0f, 7740.0f, 15540.0f);
                    (*volumes)[3] = new BoundingAAB(x, y, z, 0.0f, 9656.0f, -4950.0f, 820.0f, 1040.0f, 7150.0f);
                    (*volumes)[4] = new BoundingAAB(x, y, z, 0.0f, 3420.0f, -452.0f, 1310.0f, 940.0f, 4880.0f);
                    (*volumes)[5] = new BoundingAAB(x, y, z, 0.0f, -9018.0f, -270.0f, 7520.0f, 6870.0f, 20470.0f);
                    (*volumes)[6] = new BoundingAAB(x, y, z, 0.0f, -5088.0f, -7962.0f, 1300.0f, 2900.0f, 13210.0f);
                    (*volumes)[7] = new BoundingAAB(x, y, z, 0.0f, -11604.0f, 3526.0f, 3260.0f, 880.0f, 10640.0f);
                    (*volumes)[8] = new BoundingAAB(x, y, z, 0.0f, 5212.0f, -12332.0f, 3260.0f, 2640.0f, 8410.0f);
                    (*volumes)[9] = new BoundingAAB(x, y, z, 0.0f, -15852.0f, 2004.0f, 15860.0f, 2510.0f, 10910.0f);
                    (*volumes)[10] = new BoundingAAB(x, y, z, 0.0f, -2610.0f, -15852.0f, 15860.0f, 2510.0f, 10910.0f);
                    fixed->setWreckedMeshId(0x4780);
                } else {
                    ArraySetLength(3, *volumes);
                    (*volumes)[0] = new BoundingAAB(x, y, z, 0.0f, -73.0f, 123.0f, 3000.0f, 2860.0f, 10430.0f);
                    (*volumes)[1] = new BoundingAAB(x, y, z, 0.0f, -280.0f, -4257.0f, 3470.0f, 1815.0f, 2200.0f);
                    (*volumes)[2] = new BoundingAAB(x, y, z, 0.0f, -770.0f, -4279.0f, 5220.0f, 680.0f, 2270.0f);
                    fixed->setWreckedMeshId(0x477e);
                }
                break;
            case 3:
                ArraySetLength(2, *volumes);
                (*volumes)[0] = new BoundingAAB(x, y, z, 0.0f, -199.0f, 4708.0f, 980.0f, 1530.0f, 1240.0f);
                (*volumes)[1] = new BoundingAAB(x, y, z, 0.0f, -14.0f, -98.0f, 4500.0f, 1405.0f, 8860.0f);
                fixed->setWreckedMeshId(0x477c);
                break;
            case 2:
                ArraySetLength(3, *volumes);
                (*volumes)[0] = new BoundingAAB(x, y, z, 0.0f, -85.0f, 24.0f, 4335.0f, 1245.0f, 10880.0f);
                (*volumes)[1] = new BoundingAAB(x, y, z, 0.0f, 710.0f, 292.0f, 2990.0f, 935.0f, 11450.0f);
                (*volumes)[2] = new BoundingAAB(x, y, z, 0.0f, 1510.0f, -2886.0f, 2750.0f, 1010.0f, 2750.0f);
                fixed->setWreckedMeshId(0x477d);
                break;
            default:
                break;
            case 1:
                ArraySetLength(5, *volumes);
                (*volumes)[0] = new BoundingAAB(x, y, z, 0.0f, 351.0f, 5827.0f, 3930.0f, 1590.0f, 2070.0f);
                (*volumes)[1] = new BoundingAAB(x, y, z, 0.0f, 616.0f, 4573.0f, 4440.0f, 2120.0f, 1035.0f);
                (*volumes)[2] = new BoundingAAB(x, y, z, 0.0f, 780.0f, 3303.0f, 4970.0f, 2620.0f, 1895.0f);
                (*volumes)[3] = new BoundingAAB(x, y, z, 0.0f, 456.0f, -309.0f, 2695.0f, 3530.0f, 6600.0f);
                (*volumes)[4] = new BoundingAAB(x, y, z, 0.0f, 248.0f, -5587.0f, 3325.0f, 3020.0f, 4205.0f);
                fixed->setWreckedMeshId(0x477f);
                break;
        }
        fixed->setBV(volumes);
        fixed->setShipGroup(Globals::globals->getShipGroup(type, race, false), type, false);
        lodManager->addObject(fixed->geometry);
        fixed->field_0x40 = 1;
        ship = fixed;
    }
    ship->setLevel(this);
    return (PlayerFixedObject *) ship;
}
```

Its prologue is `strd r1,r2,[sp,#56]; mov r9,r0; ...; mov sl,r3` (race and
shipClass both spilled, type -> `sl`) instead of the original
`str r1,[sp,#60]; mov r9,r0; ...; mov fp,r3; mov r5,r2`.

### Gate-safe 336 B variant (66.7 %, 2764 B, `byte_exact` 4503)

Changing only the three coordinate lines of the body above to the mixed
form (and moving them after `new Player`, as in HEAD) keeps the function
size at 2764 B and the gate green while keeping the 336-byte LSDA:

```cpp
    Player *player = new Player(armor, hitpoints, 1, 1, 0);
    int x = wpX + jitterX - 20000;
    float z = (float) (wpZ + jitterZ - 20000);
    float y = (float) (wpY + jitterY - 20000);
    player->setEmpData(empPoints, empTime);
```

With this shape the section layout is: file size 2,187,328 B (exact),
`.text` 0x1337c8 (exact), `.ARM.extab` **0x26298 (exact)**, `.ARM.exidx`
0x6f70 (exact), `.rel.dyn` 0x7048 (exact), `.rel.plt` 0x72d0 (exact),
`.rodata` 0x41c27 (pre-existing +0xc0 from the mission rank table).
`check` prints **RATCHET PASS** (byte 4503 / linked 4503 / data 843 /
imports 207), so it can be landed if the extab exactness is judged worth
the 73.5 -> 66.7 fuzzy drop.

## Measurement table (official scorer, full link)

`rec` is the createShip LSDA record size; all rows 2764 B unless noted.
HEAD = placement-new for PlayerFixedObject/PlayerFighter, real `new` for
Player and Array, `try { setEmpData } catch (...) { __builtin_trap(); }`.

| variant | raw % | size | rec | extab | gate |
|---|---:|---:|---:|---:|---|
| **HEAD (placement-new + try)** | **73.500000** | 2764 | 328 | 0x26290 | PASS |
| real `new` all three, no try (int x / float z / float y) | 66.700000 | 2764 | **336** | **0x26298** | PASS |
| real `new` all three, no try (all-int x,y,z after ctor) | 63.000000 | 2788 | **336** | **0x26298** | FAIL byte 2372 |
| real `new` all three, no try (all-int, coords before `new Player`) | 62.700000 | 2788 | **336** | **0x26298** | FAIL byte 2372 |
| real `new` all three, no try (all-int z,y,x decl order) | 62.200000 | 2788 | **336** | **0x26298** | FAIL byte |
| real `new` all three, no try (all-int y,z,x decl order) | 62.800000 | 2788 | **336** | **0x26298** | FAIL byte |
| real new Player only + try (== HEAD) | 73.500000 | 2764 | 328 | 0x26290 | PASS |
| real new PlayerFixedObject only + try | 63.000000 | 2768 | 340 | 0x2629c |
| real new PlayerFighter only + try | 61.600000 | 2768 | 340 | 0x2629c |
| real new Fixed+Fighter + try | 63.200000 | 2768 | 352 | 0x262a8 |
| real new all three + try | 63.200000 | 2768 | 352 | 0x262a8 |
| placement F/G, real P, no try | 69.500000 | 2780 | 316 | 0x26284 |
| placement F/G, real P, no try + real new Fixed | 62.100000 | 2760 | 328 | 0x26290 |
| placement F/G, real P, no try + real new Fighter | 65.200000 | 2784 | 328 | 0x26290 |
| placement F/G, real P, no try + real new Fixed+Fighter | 66.700000 | 2764 | **336** | **0x26298** | PASS |
| no-try base + try around `new Array` | 64.500000 | 2792 | 332 | 0x26294 |
| no-try base + try around `setLevel` | 65.100000 | 2784 | **336** | **0x26298** |
| no-try base + try around `setShipGroup` | 65.400000 | 2780 | **336** | **0x26298** |
| no-try base + try around `addObject` | 65.100000 | 2784 | **336** | **0x26298** |
| no-try base + try around `setLevel`, empty catch | 65.400000 | 2788 | **336** | **0x26298** |
| no-try base + try around `setLevel`, `return nullptr` | 64.600000 | 2792 | **336** | **0x26298** |
| HEAD + try around `setLevel` (two catch-alls) | 66.400000 | 2768 | 340 | 0x2629c |
| HEAD + try around `setShipGroup` | 66.400000 | 2764 | 340 | 0x2629c |
| HEAD + try around `new Array` | 65.000000 | 2776 | **336** | **0x26298** |
| HEAD with `try` widened over `new Player` | 61.300000 | 2792 | **336** | **0x26298** |

Source-form sweeps that all stayed at 73.5 / 328: the six x/y/z
declaration and computation permutations on the HEAD body (73.3-73.5);
`hitpoints` term reorderings; `switch` instead of the type if-chain;
`!shipClass`, `shipClass == 0u`, `mission == 154`; explicit `int t = type;
int r = race;` temporaries before both ctors; `(int)` casts on the ctor
args; `nullptr` -> `0` / `(AEGeometry *)0`; `0.0f` -> `0`; named
`float zero` / `const float zero` / `int zero`; `Player *player; player =
new Player(...)`; `Player *player(new Player(...))`; `::new`; `ship`
declared at the top / after `new Player`; caching `Globals::status` in a
local pointer or reference (60.2); caching `Globals::rnd` (62.6);
`int armor` inlined into the `new Player` call (70.4); empty or rethrowing
catch bodies (64.8 / 49.2); `level` computed with an `if` (62.1);
`empPoints` before `hitpoints` (58.8); `shipClass` used early via a
`capital`/`small` flag (60.9-62.7).

## Why the real-`new` shape scores lower

The original's prologue is reproduced bit-for-bit by HEAD (placement-new +
`try`), but real `new` changes the Greedy RA from the first instruction:

```
orig:  str r1,[sp,#60] ; mov r9,r0 ; ldr r0,[pc] ; mov fp,r3 ; mov r5,r2
ours:  strd r1,r2,[sp,#56] ; mov r9,r0 ; ldr r0,[pc] ; mov sl,r3
```

With real `new`, the allocated pointer must stay live across the ctor for
the cleanup landing pad; the allocator then spills `shipClass` (and `race`)
instead of keeping `shipClass` in `r5`/`sl`/`r6`, and `type` moves from
`fp` to `sl`. That cascades through the whole function. The all-int body
(2788 B) shows the size effect concretely:

* the BoundingAAB `0.0f` argument materialises in `sl` (high register):
  `mov.w sl,#0` + `str.w sl,[sp]` = 8 B, instead of the original's
  `movs r6,#0` + `str r6,[sp]` = 4 B, once per race case (5x, +20 B);
* `this` is reloaded into `r9`: `ldr.w r9,[sp,#52]` (4 B) instead of the
  original's `ldr r6,[sp,#56]` (2 B), 3x (+6 B);
* `x` lands in `r6` rather than `sl`, and `z` in `r6`/`r9` rather than
  `r8`.

The mixed `int x / float z / float y` variant avoids those wide encodings
(its y/z conversions are hoisted and reused, so it has 994 instructions
instead of 1007) and lands at exactly 2764 B, but the RA is still the
real-`new` one, hence 66.7.

The `try`/`catch(...)` in HEAD is a tuning hack that restores the
original's RA (Greedy RA sees the same live ranges as the original's
cleanup-only EH), but it adds the catch-all type/action table (+6 B) and
HEAD is missing the placement-new ctor call sites (-16 B), netting 328 B.
Any additional `try` (or real `new`) that reaches 336 B also disturbs the
RA and drops the score to 61-66.

## Reproduction

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 tools/verify/verify.py --show _ZN5Level10createShipEiiiP8Waypointbb
cmake --build cmake-build-match --target check -j6
```

Scratch helpers (not committed): `/dev/shm/w6705/measure.py` (splice body,
link, verify, report pct/size/LSDA), `ndiff.py` (normalised diff blocks),
`pro.py` (prologue dump), `lsda2.py` (ARM EHABI LSDA parser),
`gen/` (~120 measured variant bodies).
