# Level::createMission — wave 191b w13761

Target: `_ZN5Level13createMissionEv` (`src/game/world/Level.cpp`), size 17,172 B.
Base master `629d8ea54`; previous landing `e2c9a1201` (raw 2,605 / aligned 653).

## Landed change: stationPirates source order + case-4 tail alias removal

### 1. stationPirates first loop — real source-order swap

The binary at `0xb3af0..0xb3ba0` orders the pirate-station loop as

```
Route ctor
getWaypoint -> createStaticObject(..., 14243, true) -> store enemies[i]
getWaypoint -> getPosition() -> Vector::assign into pirateBase (sp+312)
reload enemies[i] -> setToSleep()
getText(441) -> name store
```

The checked-in source stored `setToSleep`/`name` first and only then assigned
`pirateBase` and `createStaticObject`.  Restoring the binary order:

```cpp
Route *route = new Route(coords, 3);
(*this->enemies)[i] = this->createStaticObject(route->getWaypoint(), 14243, true);
pirateBase = route->getWaypoint()->getPosition();
(*this->enemies)[i]->setToSleep();
(*this->enemies)[i]->name = *Globals::gameText->getText(441);
```

Alone this is **size-fatal** (+4, 17,176).

### 2. case-4 tail — drop the `enemies` alias

`case4Tail` had `Array<KIPlayer *> *enemies = this->enemies;` and used it for
the loop bound.  The original keeps `this` in `r8` for the whole late region
(`ldr.w r8,[r8,#248]`, `str.w r5,[r8,#40]` at `0xb41e8`) and uses `r9` for the
index; the alias forces `this` into `sl` and costs 4 bytes elsewhere.  Removing
it (`this->enemies->size()`) alone is size-fatal (−4).

The two changes together are size-exact and are the unique combination found
that is both size-exact and aligned-improving.

## Measurement (vs previous landing `e2c9a1201`)

| variant | raw | aligned | size | insn | `.text` | `.ARM.extab` |
|---|---:|---:|---:|---:|---:|---:|
| previous landing `e2c9a1201` | 2,605 | 653 | 17,172 | 6,206 | 74,045 | 561 |
| reorder only (`mkvar_pir.py base`) | 3,528 | 624 | **17,176** | 6,207 | shift | shift |
| alias removal only (`v_c4size.py`) | 2,761 | 644 | **17,168** | 6,202 | — | — |
| **reorder + alias removal (landed)** | **3,498** | **604** | **17,172** | **6,203** | **74,938** | **593** |

Original instruction count 6,197.  The landed shape removes 3 of the 9 extra
instructions (6,206 → 6,203) and cuts aligned mismatch 653 → 604 (−49).
`.text` diffbytes grows 74,045 → 74,938 (+893) and `.ARM.extab` grows
561 → 593 (+32); the aligned saving (−49) exceeds the extab growth (+32), so
this is a net aligned win.  `.ARM.exidx` 0, `.rodata` 0, `.rel.plt`
byte-identical, size-exact 17,172.

Gate: `RATCHET PASS` — byte_exact 4508, linked_exact 4508, size-exact 4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.

## Residual drift after the landing

- `0xb3b96` post-stationPirates loop-bound build order.
- `0xb40ba` case-4 tail (`movs r0,#24` / `vcvt.s32.f32` order).
- `0xb4172` case4Tail loop regalloc (orig `r8`/`r9`/`r5`, ours `sl`/`fp`/`r6`).
- `0xb41da` Objective ctor (`ldr r3,[sp,#124]` / `str r5,[r8,#40]`).

## Second landing: drop the case-4 tail `Globals *globals` local

The original `case4Tail` loop reloads `Globals::globals` through the static
address (`sl = &Globals::globals`, `ldr r0,[sl]` every iteration) instead of
hoisting a `Globals *globals` local.  Removing

```cpp
Globals *globals = Globals::globals;
...
8, 0, globals->getRandomEnemyFighter(8),
```

and calling `Globals::globals->getRandomEnemyFighter(8)` directly matches that
shape.

| variant | raw | aligned | size | insn | `.text` | `.ARM.extab` |
|---|---:|---:|---:|---:|---:|---:|
| first landing (`0ff88300c`) | 3,498 | 604 | 17,172 | 6,203 | 74,938 | 593 |
| **drop `globals` local (landed)** | **3,497** | **596** | **17,172** | **6,201** | **74,937** | **593** |

aligned 604 → 596 (−8), insn 6,203 → 6,201 (orig 6,197), `.ARM.extab`
unchanged at 593.  Remaining case4Tail drift is a pure register swap
(`this` orig `r8` / ours `sl`; index orig `r9` / ours `r8`; globals-address
orig `sl` / ours `r9`), driven by long-range liveness from `0xb2c8e`
(`mov sl, r4`) where the original later clobbers `sl` and reloads `this`
at `0xb40b6` (`ldr.w r8,[sp,#124]`).

## Third landing: case-2 `getProductionGoodAmount()` after `new Route(...)`

The original case-2 prologue stores the `Route` ctor result and only then
calls `getProductionGoodAmount()`; ours loaded `goods` first.  Moving

```cpp
int goods = mission->getProductionGoodAmount();
this->enemyRoute = new Route(coords, 6);
```

to

```cpp
this->enemyRoute = new Route(coords, 6);
int goods = mission->getProductionGoodAmount();
```

matches the call/store order.

| variant | raw | aligned | size | insn | `.text` | `.ARM.extab` |
|---|---:|---:|---:|---:|---:|---:|
| second landing (`93629f3b7`) | 3,497 | 596 | 17,172 | 6,201 | 74,937 | 593 |
| **goods after Route (landed)** | **3,517** | **588** | **17,172** | **6,201** | **74,957** | **594** |

aligned 596 → 588 (−8), size-exact, same instruction count (6,201 vs orig
6,197).  Raw +20 / `.text` +20 / `.ARM.extab` +1; the aligned saving (−8)
exceeds the extab growth (+1), so it is a net aligned win.  `.ARM.exidx` 0,
`.rodata` 0, `.rel.plt` byte-identical.

Gate: `RATCHET PASS` — byte_exact 4508, linked_exact 4508, size-exact 4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.

Case-2 call order now matches, but zero-init ordering still differs: ours
zeroes `sp+316/324/332` before the first `nextInt`; orig zeroes `sp+272/280`
first.
