# Level::createShip — wave 119 (w6759): real-`new` shape makes the LSDA record exact

Status: **source change landed** (2026-11-05).
Target `_ZN5Level10createShipEiiiP8Waypointbb` (`Level::createShip`,
original 2764 B @ `0xc0214`, Thumb entry `0xc0215`). Branch `wave119-w6759`,
base `4efc90c64`.

## Result

```
                                   before (HEAD)        after (landed)
createShip fuzzy (raw)             74.400000            67.100000
createShip size                    2764 B               2764 B
createShip LSDA record             328 B / 54 rows      336 B / 57 rows  (exact)
createShip ttype                    0x00                 0xff
.ARM.extab size / addr             0x2628c             0x26294
.ARM.exidx                         0x6f70 (exact)       0x6f70 (exact)
.text                              0x1337c8 (exact)     0x1337c8 (exact)
byte_exact / linked_exact          4505 / 4505          4505 / 4505
imports / data / lint / drift      207 / 843 / CLEAN / 0
```

`check` (final, after the landed change): **RATCHET PASS** —
`verify: byte 4505 linked 4505 stubs 0 missing 0 extra 0 avg 99.94`,
`data: exact 843 of 843`, `sodiff: 0/0 imports 207`, lint CLEAN, drift 0.
No section size, base, import, data or symbol-parity movement; the only
section bytes that change are `.text` and `.ARM.extab`.

The landed diff is 4 insertions / 10 deletions, all ordinary C++:

* `PlayerFighter *fighter = new PlayerFighter(...)` instead of
  `::operator new` + placement new.
* `PlayerFixedObject *fixed = new PlayerFixedObject(...)` instead of
  `::operator new` + placement new.
* `player->setEmpData(...)` is a plain call (the `try`/`catch (...)`
  `__builtin_trap()` anchor is gone).
* `fighter->setShipGroup(..., type, hostile)` — the original's third
  argument is `hostile`, not `group` (the wave-119 tuned form used `group`
  for +0.5 fuzzy).

The record now matches the original exactly: TTYPE `0xff` (no type table),
57 call-site rows, every action 0 (cleanup only).  The 3 rows the old
placement-new shape was missing are the real-`new` ctor cleanups for
`PlayerFixedObject` and `PlayerFighter`.

## Remaining `.ARM.extab` delta

The only remaining extab delta is **`Generator::createMission` -4 B**
(ours 168, original 172), which is a different TU (line 95 of
`tools/source_order.txt`) and out of scope here.  With createMission exact
the section would be `0x26298` (original).  `Hud::draw` is already exact.

## Why the fuzzy score drops (74.4 -> 67.1)

The original really uses real `new` for all three ship objects: its
`.ARM.extab` record has 57 cleanup rows and the disassembly has
`blx _Znwj` at `0xc03a4` (Player), `0xc0408` (PlayerFixedObject),
`0xc0448` (Array) and `0xc06bc` (PlayerFighter).  The wave-117/119
placement-new + `try`/`catch` shape is a tuning hack that reproduces the
original's Greedy register allocation (the prologue is byte-identical for
the first ~100 instructions) at the cost of the 8-byte-short LSDA.

Real `new` changes Greedy RA from the first instruction because each
allocated pointer must stay live across its ctor for the cleanup landing
pad:

```
orig/HEAD  str r1,[sp,#60] ; mov r9,r0 ; ldr r0,[pc] ; mov fp,r3 ; mov r5,r2
real new   str r3,[sp,#60] ; mov r9,r0 ; str r1,[sp,#56] ; mov fp,r2
```

(`race` is spilled and `type` lives in `fp` in the original; real `new`
spills `type`/`race` and keeps `shipClass` in `fp`.)  The cascade reaches
the 24 `BoundingAAB` calls (x/y/z land in `r6/r8/r3` instead of
`sl/r8/r4`), which is where most of the 33-point fuzzy loss is.

## Measurement table (official scorer, full link)

`rec` is the createShip LSDA record length; `byte` is repo-wide
`byte_exact` after the full link.  All rows 2764 B unless noted.

| variant | raw % | rec | extab | byte | gate |
|---|---:|---:|---:|---:|---|
| **HEAD: placement-new + try, tuned spawn, `group`** | **74.400000** | 328 | 0x2628c | 4505 | PASS |
| **landed: real new + no try, tuned spawn, `hostile`** | **67.100000** | **336** | **0x26294** | **4505** | **PASS** |
| real new + no try, faithful spawn, `hostile` | 66.700000 | 336 | 0x26294 | 4505 | PASS |
| real new + no try, tuned spawn, `group` | 64.600000 | 336 | 0x26294 | 2372 | FAIL (2768 B) |
| real new + no try, faithful spawn, `group` | 64.200000 | 336 | 0x26294 | 2372 | FAIL (2768 B) |
| real new + no try, all-int coords | 63.100000 | 336 | 0x26294 | 2372 | FAIL (2788 B) |
| real new + no try, note-w6705 body | 62.700000 | 336 | 0x26294 | 2372 | FAIL (2788 B) |
| real new Fixed only (placement Fighter) | 63.600000 | 340 | 0x26298 | 2372 | FAIL (2768 B) |
| real new Fighter only (placement Fixed) | 65.400000 | 340 | 0x26298 | 2372 | FAIL (2784 B) |
| real new all three + try(setEmpData) | 63.700000 | 352 | 0x262a4 | 2372 | FAIL |
| wrap placement Fixed ctor in try | 63.600000 | 336 | 0x26294 | 4505 | PASS |
| wrap placement Fighter ctor in try | 41.300000 | 340 | 0x26298 | — | FAIL (2728 B) |
| cargo null-check + volumes-before-fixed (`cv`) | 61.900000 | 332 | 0x26290 | 4505 | PASS |
| volumes-before-fixed only | 59.300000 | 332 | 0x26290 | 4490 | FAIL |
| cargo null-check only | 73.400000 | 328 | 0x2628c | 4505 | PASS |

### Typed-catch route (336 B at 74.4, but the gate fails)

`catch (T &)` before `catch (...)` keeps 74.4 and yields the 336-byte
record, but the LSDA type table needs an `R_ARM_TARGET2` reference from
`Level.o` (line 113/203).  `Level.o` is earlier than every typeinfo's
existing GOT entry, so gold moves the typeinfo's GOT slot to Level.o's
position; the whole GOT then shifts by 4 and ~282 functions' `.text`
literal-pool words change.

| catch type | raw % | rec | byte_exact | note |
|---|---:|---:|---:|---|
| `decltype(nullptr)` | 74.400000 | 336 | 4242 | `_ZTIDn` GOT 0x210658 -> 0x210368 |
| `std::bad_exception` | 74.400000 | 336 | 4241 | `_ZTISt13bad_exception` 0x210644 -> 0x210368 |
| `std::bad_alloc` / `std::exception` | 74.400000 | 336 | ~4234 | renumber |
| `AbyssEngine::IApplicationModule` | 74.400000 | 336 | 1793 | new GOT slot, `.rel.dyn` 0x7048 -> 0x7050, `.plt` 0x5e7ec -> 0x5e7f4 |
| `BoundingAAB` / `KIPlayer` / `MTitle` | 74.400000 | 336 | 1793 | new GOT slot |

The GOT-slot types shift `.plt` by 8 (every `bl <plt>` changes); the
"late" GOT typeinfos renumber `.dynsym`/`.dynstr`/`.hash` and shift the
GOT.  No catch type keeps `byte_exact` at 4505.

## What the original really computes (Thumb, for future passes)

```
c03c8  ldr r2,[sp,#48]   ; wpY        c03ca  ldr r3,[sp,#32]  ; jitterY (call 2)
c03cc  ldr r0,[sp,#44]   ; wpX        c03ce  ldr r1,[sp,#36]  ; jitterX (call 1)
c03d0  add r2,r3         ; y          c03d2  ldr r3,[sp,#40]  ; wpZ
c03d4  ldr r4,[sp,#28]   ; jitterZ (call 3)
c03d6  add r0,r1         ; x          c03d8  movw r1,#20000
c03dc  sub.w fp,r0,r1    ; x          c03e0  add r3,r4        ; z
c03e2  subs r4,r2,r1     ; y          c03e4  sub.w r8,r3,r1   ; z
```

All three coordinates are `int`, computed between the `Player` ctor and
`setEmpData`, and converted with `vcvt.f32.s32` at each ctor call site.
The original's `setShipGroup` third argument is `hostile`; the fixed
branch uses `false`.  The LOD guard is `missionPtr != 1 && missionPtr != 23`.

## Reproduction

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
python3 tools/verify/verify.py --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so --ours cmake-build-match/libgof2hdaa.so --show _ZN5Level10createShipEiiiP8Waypointbb
python3 _work/reconstructions/exidx_table.py --diff \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so cmake-build-match/libgof2hdaa.so
```

Scratch helpers (not committed): `/dev/shm/w6759/{mk2..mk23}.py`,
`measure.py`, `fastbyte.py`, `quick.sh`, `full.sh`, `orig.asm`,
`ours.asm`, and the per-variant `v_*.asm` dumps.
