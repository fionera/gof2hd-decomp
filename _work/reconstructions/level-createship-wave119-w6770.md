# Level::createShip — wave 119 (w6770): spill-hoist / block-placement sweep, no source win

Status: **analysis only, no source change** (2026-11-05).
Target `_ZN5Level10createShipEiiiP8Waypointbb` (`Level::createShip`,
original 2764 B @ `0xc0214`, Thumb entry `0xc0215`, 1007 insns).
Branch `wave119-w6770`, base `026aadb4e`.

## Result

HEAD (placement-new for Fixed/Fighter + tuned `i/f/f` spawn +
`try { setEmpData } catch (...) { __builtin_trap(); }`) is unchanged:

```
createShip fuzzy (official scorer)  74.400000   (749 of 1007 insns equal)
createShip size                     2764 B
createShip LSDA record              328 B  (TType 0x00, cs_len 299, 54 sites, action 01 00)
.ARM.extab size                     0x2628c (original 0x26298, -12 B)
.text                               0x1337c8 (exact)
.rel.dyn                            0x7048   (exact)
.got                                0x20fcf4 / 0x430c (exact)
byte_exact / linked_exact           4505 / 4505
imports / data / lint / drift       207 / 843 / CLEAN / 0
```

`check`: **RATCHET PASS**.  No variant measured this wave beat 74.400000.

## The first divergence is one hoisted spill store

The first 98 instructions (through `0xc031a`) are byte-identical.  The
first mismatch is the spill of `this` (`r9`) to `[sp,#56]`:

```
orig  0xc031a  cmp   r0,#20
      0xc031c  ble   ...
      0xc031e  movs  r0,#20
      0xc0320  b     ...
      0xc0322  ldr   r0,[r4,#0]
      0xc0324  blx   Status::getLevel
      0xc0328  add.w r0,r0,r0,lsl#2
      0xc032c  cmp   r6,#1          ; shipClass
      0xc032e  add.w sl,r0,#40      ; empPoints
      0xc0332  str.w r9,[sp,#56]    ; <-- spill of `this` here
      0xc0336  str.w fp,[sp,#52]    ; spill of `type`
      0xc033a  bne   ...
ours  0xc031a  cmp   r0,#20
      0xc031c  str.w r9,[sp,#56]    ; <-- spill of `this` here (bb.14, before the branch)
      0xc0320  ble   ...
      ...
      0xc0330  cmp   r6,#1
      0xc0332  add.w sl,r0,#40
      0xc0336  str.w fp,[sp,#52]
      0xc033a  bne   ...
```

This single scheduling difference cascades into the whole fixed-object
branch: the original keeps `x=sl, y=r8, z=r4, fixed=r9, volumes=fp,
zero=r6`; ours gets `x=r6, y=r8, z=sl, fixed=fp, volumes=r9`.  Every
`BoundingAAB` call in the race switch then differs by one register
operand, which is the bulk of the remaining 258 mismatching instructions.

### MIR evidence (NDK clang 7.0.2, `-mllvm -print-after-all`)

The store is inserted by `HoistSpillHelper::hoistAllSpills`
(`InlineSpiller.cpp`), which hoists equal-valued spills up the dominator
tree to the cheapest block where a sibling vreg is live at the insert
point.  In our build it lands in `bb.14` (the `getLevel() > 20` ternary
block, 50 % frequency) at the last insert point, i.e. between
`t2CMPri` and `t2Bcc`:

```
bb.14 (%ir-block.65):
  2592B  tBL Status::getLevel
  2624B  %196 = COPY $r0
  2640B  t2CMPri %196, 20
  2648B  t2STRi12 %681, %stack.0   ; hoisted spill of `this`
  2656B  t2Bcc %bb.16, 13
```

The original's store is in the *join* block (`bb.16`), after
`add.w sl,r0,#40` and `cmp r6,#1`.  The original also has a single
reload of `this` before `lodManager->addObject` (`0xc0b5e`); our build
has reloads at the end of every race case (`0xc06b0`, `0xc0b34`, …)
because the allocator keeps `this` in callee-saved `sl` across
`setBV`/`setShipGroup`.  Both are downstream of the same hoist decision.

## LSDA / GOT analysis (confirms w6755 note)

`python3 /dev/shm/w6770/lsda3.py` on both binaries:

| | orig | ours |
|---|---:|---:|
| sites | 57 | 54 |
| TType | `0xff` (no type table) | `0x00` |
| cs_len | 315 | 299 |
| actions | all 0 (cleanup only) | one catch-all (`action=1`) |
| record bytes | 336 | 328 |

The 3 missing cleanup sites are the `new`-expression cleanups for the
two placement-new objects (`PlayerFighter` ctor @ `0xc06dc..0xc06fc`,
`PlayerFixedObject` ctor @ `0xc0426..0xc0446`) plus one range marker.
The original is a faithful real-`new`, no-try body; our tuned body is
placement-new + a catch-all.

A typed catch + catch-all restores the 336-byte record at unchanged
score/size, but `Level.o` then references `_ZTI<T>`, which gold inserts
into `.dynsym` at Level.o's position (~index 3438) instead of the
libc++abi position (5316+).  The GOT is laid out in dynsym order, so
every GOT slot after 3438 moves and `byte_exact` collapses:

```
orig  _ZTISt13bad_exception  dynsym 5316  GOT 0x210644
ours  _ZTISt13bad_exception  dynsym 3438  GOT 0x210368
```

Measured this wave (official scorer + fast raw-byte counter):

| catch type | score | size | byte_exact |
|---|---:|---:|---:|
| `std::bad_exception` | 74.4 | 2764 | 4242 |
| `decltype(nullptr)` | 74.4 | 2764 | 4242 |
| `std::bad_alloc` | 74.4 | 2764 | 4234 |
| `std::exception` | 74.4 | 2764 | 4230 |

All are below the gate floor (4503), so the typed-catch route cannot
land without also fixing the GOT order (which no source-level change
can do: the reference must come from Level.o, which is linked before
libc++abi).

## Measurement table (official scorer, full link)

All rows 2764 B and 74.400000 % unless noted.  New rows this wave:

| variant | pct | size | note |
|---|---:|---:|---|
| **HEAD** | **74.4** | 2764 | baseline |
| no try (plain `setEmpData`) | 70.5 | 2776 | body grows |
| `Level *self = this;` + `setLevel(self)` | 74.4 | 2764 | tie |
| `self` + `self->missionPtr`/`self->lodManager` | 74.4 | 2764 | tie |
| `int mp = missionPtr;` local | 74.4 | 2764 | tie |
| `this->missionPtr` / `this->lodManager` | 74.4 | 2764 | tie |
| empPoints before mission check | 73.4 | 2764 | |
| mission check after empPoints | 73.4 | 2764 | |
| `int empTime = 15000;` then `if (shipClass==1)` | 74.4 | 2764 | tie |
| `empTime = shipClass==1 ? 45000 : 15000` | 72.3 | 2764 | |
| two separate `if (shipClass==1)` | 74.1 | 2764 | |
| `40 + (...) * 5` | 74.4 | 2764 | tie |
| `(...) * 5; empPoints += 40;` | 74.4 | 2764 | tie |
| `(...) * 5; empPoints = empPoints + 40;` | 74.4 | 2764 | tie |
| `1 == shipClass` | 74.4 | 2764 | tie |
| `!shipClass` / `0 == shipClass` / `shipClass < 1` | 74.4 | 2764 | tie |
| `shipClass == 1` branch first | 19.3 | 720 | |
| `switch (shipClass)` | 39.4 | 2760 | |
| `KIPlayer *ship` declared before `Player *player` | 74.4 | 2764 | tie |
| `ship = fighter;` before cargo block | 74.4 | 2764 | tie |
| `ship = (KIPlayer *) fighter;` | 74.4 | 2764 | tie |
| `ship = fighter;` after ctor, use `ship` in addObject | 74.4 | 2764 | tie |
| `KIPlayer *kiFighter` declared outside the if | 74.4 | 2764 | tie |
| no `kiFighter` local (direct `fighter->…`) | 74.4 | 2764 | tie |
| empty-then `if (missionPtr==1||==23) {} else {…}` | 74.4 | 2764 | tie |
| `!(missionPtr==1||==23)` | 74.4 | 2764 | tie |
| `AEGeometry *geo` local for the ternary | 74.4 | 2764 | tie |
| `fixed->setShipGroup` before `setBV` | 74.3 | 2764 | |
| `addObject` before `setShipGroup` | 74.3 | 2764 | |
| `field_0x40` before `addObject` | 74.3 | 2764 | |
| `field_0x40 = true` | 74.4 | 2764 | tie |
| spawn decl order `yxz` / `yzx` | 74.4 | 2764 | tie |
| spawn decl order `xzy` / `zxy` / `zyx` | 74.3 | 2764 | |
| real `new` fixed only | 63.6 | 2768 | |
| real `new` fighter only | 65.4 | 2784 | |
| real `new` both | 63.7 | 2772 | |
| real `new` both + faithful int spawn + no try | 63.1 | 2788 | |

The score is extremely robust: ~40 distinct source shapes all land on
exactly 74.400000 (many produce different `.so` bytes but the same
number of matching instructions).  The remaining 258 mismatches are
allocator-driven (spill hoist + register rotation), not source-shape
driven.

## Reproduction

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 tools/verify/verify.py --show _ZN5Level10createShipEiiiP8Waypointbb
cmake --build cmake-build-match --target check -j6
```

Scratch helpers (not committed): `/dev/shm/w6770/{ls.sh,score1.py,be.py,
sw.py,v1.py…v13.py,lsda3.py,mir.txt,orig.asm,ours.asm}` and the pristine
body copy `/dev/shm/w6770/Level.base.cpp`.

## Dead ends recorded

- Moving the spill store by changing statement order / declaration
  order / variable forms: all 74.4 or lower; the hoist target never
  moves out of `bb.14`.
- Typed catch: restores the 336-byte LSDA but reorders the GOT
  (`byte_exact` 4230–4242).
- Real `new` (faithful body): makes the LSDA exact (336 B) but drops
  the score to 63.1–67.4.
- `setShipGroup`/`addObject`/`field_0x40` reordering: 74.3.
