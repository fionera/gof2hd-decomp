# Level::createShip — wave 113 (w6104): setShipGroup flag + cargo switch

Status: **source change landed** (wave-113, 2026-11-05).
Target `_ZN5Level10createShipEiiiP8Waypointbb` (`Level::createShip`, original
2764 B @ `0xc0214`). Builds on `level-createship-wave111-realnew.md` and
`level-createship-wave112-w6004.md`.

## Result

```
                                   before (HEAD a979ace18)   after
createShip fuzzy (raw)             66.6334                   67.2664
createShip fuzzy (report, 1 dp)    66.6                      67.3
createShip size                    2764 B                    2764 B
.text size / addr                  0x1337c8 @0x69440         0x1337c8 @0x69440
.ARM.extab size / addr             0x26298 @0x1a3b78         0x26298 @0x1a3b78
.ARM.exidx size / addr             0x6f70 @0x19cc08          0x6f70 @0x19cc08
.rodata size                       0x41b67 (269159)          0x41b67 (269159)
file size                          2,187,328 (exact)         2,187,328 (exact)
sections                            27 (exact)                27 (exact)
byte_exact / linked_exact          4500 / 4500               4500 / 4500
lint / drift                       CLEAN / 0                 CLEAN / 0
```

Only the known 192-byte `.rodata` codegen-table delta remains; every section
size and base is otherwise exact.

## Landed diff (`src/game/world/Level.cpp`)

1. `fighter->setShipGroup(getShipGroup(type, race, group), type, hostile)` —
   the third argument is `hostile`, not `group`. The original loads the 6th
   incoming parameter (`[r7,#12]`) into `r8` and passes `r8` as the `flag`
   argument (`mov r3,r8` at `0xc0716`); the fixed branch passes `false`
   (`movs r3,#0`). This was a real semantic bug.
2. The cargo block is a `switch (type)` with `case 0x2c/0x31` sharing the
   `delete cargo; cargo = nullptr` body and `case 0x33` setting
   `field_0x25 = 0`. The original checks `0x2c`, then `0x33`, then `0x31`
   (`cmp r4,#44; beq cargo; cmp r0,#51; beq field25; cmp r0,#49`), which is
   the switch lowering, not the `(0x2c || 0x31) else if (0x33)` form (that
   form checks `0x2c`, `0x31`, `0x33`).
3. Removed the `KIPlayer *kiFighter = fighter;` alias; the ternary is written
   directly on `fighter`. (No codegen change; kept for readability.)

All ordinary C++; no attributes, asm, manual data or shims.

## Measurement table (full link, official scorer raw ratio)

| variant | raw % | size |
|---|---|---|
| HEAD baseline | 66.6334 | 2764 |
| setShipGroup `hostile` only | 67.0330 | 2764 |
| + cargo `switch` (landed) | **67.2664** | 2764 |
| + `kiFighter` alias removed | 67.2664 | 2764 |
| cargo if-chain `2c / 33 / 31` | 67.2328 | 2764 |
| cargo `switch`, addObject after cargo in source | 66.8666 | 2764 |
| cargo if-chain, addObject after cargo in source | 66.8666 | 2764 |
| inverted `missionPtr == 1 || == 23` condition | 67.2664 | 2764 |
| nested `if (missionPtr != 1) if (!= 23)` | 67.2664 | 2764 |
| hitpoints `switch(type)` | 67.1664 | 2764 |
| type-chain orders 31/33/2c, 31/2c/33, 2c/31/33 | 67.1664 / 66.9665 / 66.9331 | 2764 |
| `switch(shipClass)` main branch + cargo switch | 63.2258 | 2768 |
| all-int coords + casts at ctors | 62.9960 | 2792 |
| coords `i/i/f` | 63.7854 | 2780 |
| coords `f/i/f` / `f/f/i` / `f/f/f` | 66.1339 / 65.4346 / 56.7312 | 2764 / 2764 / 2784 |
| coords before `new Player` (`i/f/f`) | 61.8526 | 2788 |
| coords after `setEmpData` in source | 67.2664 | 2764 |
| `int armor` inlined into `new Player` | 62.8887 | 2752 |
| `int empTime` declared before `empPoints` | 67.2664 | 2764 |
| wp decl permutations (with landed base) | YZX best 67.2664; XYZ/XZY/YXZ 67.0330; ZYX 66.8332; ZXY 66.6334 | 2764 |
| jitter decl/call order permutations | no change (67.2664) | 2764 |
| `int wpX = 0, wpY = 0, wpZ = 0;` init form | 67.0665 | 2764 |

## Structural findings (remaining deltas)

* Original prologue: `str r1,[sp,#60]` (race), `mov r9,r0` (this),
  `mov fp,r3` (**type -> fp**), `mov r5,r2` (shipClass -> r5), status -> r4,
  then `mov sl,r5` (shipClass -> sl). Ours: `strd r1,r2,[sp,#56]` (race and
  shipClass both spilled), `mov sl,r3` (type -> sl), status -> r6.
  The allocator keeps `shipClass` in `sl` and `type` in `fp` in the original;
  in ours `shipClass` is spilled and `type` takes `sl`. This single choice
  cascades through the whole function (jitterX is kept in `fp` in ours and
  spilled to `sp36` in the original; empPoints goes to `fp` vs `sl`; the
  capital-ship volume array lands in `r9` vs `fp`; the fixed object in `fp`
  vs `r9`; x float bits in `r6` vs `sl`). Region scores (original address
  ranges, matched insns / region insns):
  prologue `0x0-0x1b4` 64.7 %, spawn `0x1b4-0x478` 55.1 %,
  capital volumes `0x478-0x918` 69.6 %, tail `0x918-0xacc` 83.7 %.
* Original spawn: x/y/z are **ints** computed between the `Player` ctor and
  `setEmpData` (`fp,r4,r8`), then converted with `vcvt` separately inside
  each of the two ctor branches. Our `i/f/f` shape converts y/z before the
  branch and keeps x int; the fully faithful `i/i/i` shape scores lower
  (62.9960, 2792 B) because its register allocation is worse everywhere else.
* Original fighter branch layout: the `missionPtr != 1 && != 23`
  `lodManager->addObject` block is inline (fallthrough) and the cargo block
  follows; ours emits the cargo block first and the addObject block
  out-of-line. Source reordering/inverting the condition does not move it.
* `.ARM.extab`: createShip's own record is still 328 B at extab offset 45428
  (original 336 B at 45432); the section total stays exact because
  `Hud::draw` +4 and `Generator::createMission` -4 cancel. The cargo switch
  did not change the extab or the landing-pad count.
