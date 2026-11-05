# Level::createShip — wave 119 (w6734): catch-type / LSDA / RA sweep, no score win

Status: **analysis only, no source change** (2026-11-05).
Target `_ZN5Level10createShipEiiiP8Waypointbb` (`Level::createShip`,
original 2764 B @ `0xc0214`, Thumb entry `0xc0215`, 1007 insns).
Branch `wave119-w6734`, base `6552d3af5`.

## Result

HEAD (placement-new for Fixed/Fighter + `try { setEmpData } catch (...)`)
is unchanged:

```
createShip fuzzy (raw)            73.485601
createShip size                   2764 B
createShip LSDA record            328 B  (cs_len 299, 54 sites, TType 0x00)
.ARM.extab size                   0x26290 (original 0x26298, -8 B)
byte_exact / linked_exact         4505 / 4505
imports / data / lint / drift     207 / 843 / CLEAN / 0
```

`check`: **RATCHET PASS**.  No variant measured this wave beat 73.485601.

## The 336 B record

The faithful real-`new` body reproduces the original's 336 B LSDA exactly
(TType `0xff`, cs_len 315, 57 call sites, empty action table) but its
cleanup landing pads change Greedy RA from the first instruction
(`strd r1,r2,[sp,#56]` / `mov sl,r3` instead of
`str r1,[sp,#60]` / `mov fp,r3` / `mov r5,r2`).  Best 336 B shape is
`body_rnm_tcargo` (real `new` for Player/Fixed/Array/Fighter plus
`try { delete fighter->cargo; } catch (...) {}`):

```
pct 66.766617   size 2764   extab 0x26298   cs_len 315   57 sites
```

Every 336 B shape found scores 61-67; every shape that scores >73 keeps
the 328 B record.

### Typed-catch route (336 B at 73.5) is blocked by the gate

`catch (T&)` + `catch (...)` gives the original's 336 B record at 73.5 %
(body 2764), but the LSDA type table's `R_ARM_TARGET2` to `_ZTI<T>` forces
a reference from `Level.o`.  `Level.o` is line 113/203 of
`tools/source_order.txt`; every `_ZTI*` in the original is either
first-referenced after `Level.o` (the 11 GOT typeinfos at dynsym >= 5316)
or has no GOT slot (adds a `.rel.dyn` entry).  Measured:

| catch type | byte_exact | note |
|---|---:|---|
| `std::bad_alloc` | 4234 | `_ZTISt9bad_alloc` 5524 -> 3438 |
| `std::exception` | 4229 | renumber |
| `std::bad_exception` | 4241 | renumber |
| `std::bad_cast` / `bad_typeid` | 1793 | new GOT slot |
| `int` / `KIPlayer` / `PlayerFighter` / `PlayerFixedObject` / `BoundingVolume` | 1793 | new GOT slot |
| `AbyssEngine::IApplicationModule` | 4505 | but `.rel.dyn` 0x7048 -> 0x7050 |
| `AbyssEngine::String` / `MTitle` / `Fixed` | 1793 | new GOT slot |

No gate-safe catch type exists: the 11 typeinfos with GOT slots are all
first referenced after `Level.o`, so any of them renumbers `.dynsym`
(and ~359 literal-pool words), and every earlier typeinfo needs a new
GOT slot.

## Semantic finding: HEAD's jitter mapping is crosswise

The original's three `rnd->nextInt(40000)` results land at
`sp36 / sp32 / sp28` (call 1/2/3) and the spawn block is

```
x = wpX + call1 - 20000
y = wpY + call2 - 20000
z = wpZ + call3 - 20000
```

(Thumb `0xc03d6 add r0,r1` with `r1 = [sp,#36]` = call 1 -> `fp` = x;
`0xc03d0 add r2,r3` with `r3 = [sp,#32]` = call 2 -> `r4` = y;
`0xc03e0 add r3,r4` with `r4 = [sp,#28]` = call 3 -> `r8` = z.)

HEAD declares `jitterY, jitterZ, jitterX` (calls Y,Z,X) but keeps
`x = wpX + jitterX`, so its `x` uses call 3, `y` call 1, `z` call 2 —
a crosswise mapping that differs from the binary.  The faithful mapping
(any declaration order, `x` <- call 1) measures **73.386296** (all six
declaration orders and all six spawn statement orders tie at 73.386296;
`wpZ,wpX,wpY` / `wpZ,wpY,wpX` are the best waypoint orders).  The
0.1-point HEAD advantage is a coincidence: HEAD's third jitter happens to
land in `sp28`, the same slot the original uses for `z`.

This is a real behavioural bug in the landed tree (spawn offsets are
permuted); it is reported here rather than fixed because the fix is a
0.1-point fuzzy regression and the coordinator's metric is the score.

## Measurement table (official scorer, full link)

All rows 2764 B unless noted.  HEAD = placement-new + catch-all.

| variant | raw % | note |
|---|---:|---|
| **HEAD** | **73.485601** | 328 B record |
| faithful jitter mapping (6 decl x 6 stmt orders) | 73.386296 | x <- call 1 |
| `setShipGroup(..., group)` (wrong arg) | 73.945409 | 1008 insns |
| faithful + `group` (wrong arg) | 73.846154 | 1008 insns |
| cargo switch order 2c/33/31 | 73.485601 | |
| hitpoints if-chain 33/31/2c | 72.889772 | |
| switch(race) source order 0,1,2,3 | 66.93148 | literal-pool order |
| waypoint assignment order (6) | 73.485601 | |
| jitter decl XYZ / YXZ | 73.386296 | |
| coords `i/i/f`, `i/f/i`, `f/i/i` | 68.10 / 67.26 / 66.97 | |
| coords `f/f/i`, `f/i/f` | 72.29 / 72.43 | |
| coords all-int (any order) | 60.87-61.50 | 2752-2756 B |
| coords all-float | 60.83 | |
| coords inside try before setEmpData | 73.485601 | |
| coords inside try after setEmpData | 71.499503 | |
| coords before `new Player` (mixed) | 60.490736 | 2776 B |
| Player placement-new (all three placement) | 64.71173 | 2760 B |
| real new all three (mixed coords) | 66.666667 | 336 B |
| real new all three + tcargo | 66.766617 | **336 B** |
| real new all three + group | 64.203694 | 2768 B |
| real new all three + try(setEmpData) | 63.168416 | 2768 B |
| real new all-int + try | 61.469712 | 2792 B |
| try around `new Player` | 60.990099 | 2780 B |
| try around `new Array` | 64.720435 | 2792 B |
| try around `setLevel` | 65.31219 | 2784 B |
| try around whole branch | 46.889 | 2792 B |
| empty catch / return nullptr / throw / player=nullptr | 64.77 / 65.48 / 49.17 / 66.53 | |
| `__builtin_unreachable` catch | 65.506958 | |
| `switch(shipClass)` | 39.378758 | |
| `if (shipClass != 1)` | 34.9 | |
| fixed branch first | 33.683158 | |
| nested `if` forms | 73.485601 / 65.230461 | |
| `KIPlayer *ship = 0` / `PlayerFixedObject *ship` | 73.485601 / 34.682659 | |
| `Level *self = this` / `this->` / mission cond reorder | 73.485601 | |
| cargo `delete` order swapped | 47.088104 | |
| fixed/volumes declaration hoists (5) | 73.485601 | |
| `hostile != 0` / `(bool)` / `?:` / `!!` | 73.485601 | |
| `lodManager` temp / `this->lodManager` | 73.485601 | |

## Reproduction

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 tools/verify/verify.py --show _ZN5Level10createShipEiiiP8Waypointbb
cmake --build cmake-build-match --target check -j6
```

Scratch helpers (not committed): `/dev/shm/w6734/{splice.py,rebuild.sh,
measure.py,batch.py,full2.py,lsda2.py,ndiff.py}` and ~90 body variants
`body_*.cpp`; `Level.cpp.head` is the pristine HEAD copy.
