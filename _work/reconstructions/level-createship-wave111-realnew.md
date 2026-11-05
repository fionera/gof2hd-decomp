# Level::createShip — wave 111 follow-up: faithful real `new` makes `.ARM.extab` exact

Status: **source change landed** (wave-111 harvest follow-up, 2026-11-05).
Target `_ZN5Level10createShipEiiiP8Waypointbb` (`Level::createShip`, original
2764 B @ `0xc0214`). Supersedes the 332-byte placement-new variant in
`level-createship-wave111-w5906.md`.

## Result

```
                                   before (w5906 332 B)   after (real new)
createShip fuzzy                   61.9                   66.4
createShip LSDA record             332 B / 54 rows        336 B / 57 rows
createShip ttype                    0x00                   0xff
.ARM.extab size                    0x26294 (-4)           0x26298 (exact)
.ARM.exidx differing bytes         1,683                  1,063
.text differing bytes              96,920                 96,739
.rodata                            192 (codegen table)    192 (codegen table)
file size                          2,187,328 (exact)      2,187,328 (exact)
byte_exact / linked_exact          4500 / 4500            4500 / 4500
lint / drift                       CLEAN / 0              CLEAN / 0
```

The original's `createShip` `.ARM.extab` record is 336 B / 57 rows / `ttype
0xff` — the real-`new` shape. The 332-byte record was a section-budget device
that only worked while `LevelScript::process` still carried a +4 record; after
wave 111's `LevelScript::process` reorder (w5804) that record became exact, so
the 332-byte device left the section 4 B short. Restoring the faithful shape
fixes the budget and improves the function.

## Landed diff (`src/game/world/Level.cpp`)

* `PlayerFighter *fighter = new PlayerFighter(...)` instead of
  `::operator new` + placement new.
* `PlayerFixedObject *fixed = new PlayerFixedObject(...)` instead of
  `::operator new` + placement new.
* The `try { player->setEmpData(...) } catch (...) { __builtin_trap(); }`
  anchor is gone; the call is plain.
* The `type` hitpoint multiplier is an `if`/`else if` chain
  (`0x33`, `0x31`, `0x2c`) instead of a `switch`.
* The cargo `delete` is unconditional again (the explicit null check from
  w5906 is reverted), and the collision-volume array is allocated after the
  `PlayerFixedObject` (w5906's hoist is reverted).

All of these are ordinary C++; no placement-new, no `__builtin_trap`, no
manual data.

## Remaining `.ARM.extab` deltas

Per-record deltas now cancel exactly: `Hud::draw` +4 (480 vs 476) and
`Generator::createMission` -4 (168 vs 172). Fixing either one alone would
break the section size again; they must be fixed together (or both left as
the current cancelling pair).
