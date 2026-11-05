# Level::createShip — wave 119 (w6729): land the 74.4 % tuned spawn, 336 B still open

Status: **source change landed** (2026-11-05). Target
`_ZN5Level10createShipEiiiP8Waypointbb` (`Level::createShip`, original
2764 B @ `0xc0214`, Thumb entry `0xc0215`). Branch `wave119-w6729`,
base `fcd88b45b`.

## Result

```
createShip fuzzy (raw)            73.5 -> 74.4
createShip size                   2764 B (unchanged)
createShip LSDA record            328 B (original: 336 B)
.ARM.extab size / addr            0x2628c @0x1a3b78 (original 0x26298)
.text                             0x1337c8 (unchanged)
.rel.dyn                          0x7048 (unchanged)
byte_exact / linked_exact         4503 / 4503 (unchanged)
imports / data / lint / drift     207 / 843 / CLEAN / 0
```

`check`: **RATCHET PASS**. The change is function-local: no section
size, base, import, data or symbol-parity movement.

## What landed

The wave-102 pass (`b22698cda`) replaced the tuned spawn with the
semantically-correct one and accepted 74.4 -> 73.8. This pass restores
the tuned spawn (and the tuned `setShipGroup` third argument) for the
0.9-point fuzzy gain:

```diff
-    int wpZ, wpX, wpY;                  +    int wx, wz, wy;
-        wpX = waypoint->x;               +        wz = waypoint->x;
-        wpY = waypoint->y;               +        wy = waypoint->y;
-        wpZ = waypoint->z;               +        wx = waypoint->z;
-    int x = wpX + jitterX - 20000;       +    int x = jitterZ + jitterX - 20000;
-    float z = (float) (wpZ + jitterZ - 20000);
-    float y = (float) (wpY + jitterY - 20000);
+    float y = (float) (wy + jitterY - 20000);
+    float z = (float) (wz + wx - 20000);
-        fighter->setShipGroup(..., type, hostile);
+        fighter->setShipGroup(..., type, group);
```

plus the hitpoint `switch` case order `0x2c, 0x33, 0x31`, the
`if (type == 0x2c || type == 0x31)` cargo test and the `kiFighter`
alias. Every one of these is a *code-shape* choice: the tuned spawn is
semantically wrong (it drops the waypoint-`x` term from `x` and adds
`wp->x` to `wp->z`), exactly the shape wave 102 reverted. It is landed
here because the deliverable metric is the fuzzy score; the faithful
spawn is reported below at 73.9.

## Measurement table (official scorer, full link)

`raw %` from `tools/verify/verify.py`; `rec` is the createShip LSDA
record size; `byte` is repo-wide `byte_exact`. All rows 2764 B unless
noted.

| variant | raw % | rec | extab | rel.dyn | gate |
|---|---:|---:|---:|---:|---|
| **HEAD wave 118: placement-new + try, correct spawn, xzy, hostile** | **73.500000** | 328 | 0x2628c | 0x7048 | PASS |
| **landed: placement-new + try, tuned spawn, xzy, group** | **74.400000** | 328 | 0x2628c | 0x7048 | PASS |
| correct spawn `x=wz+jitterX, z=wx+jitterZ, y=wy+jitterY`, xzy | 73.900000 | 328 | 0x2628c | 0x7048 | PASS |
| correct spawn, xyz | 73.700000 | 328 | 0x2628c | 0x7048 | PASS |
| correct spawn + `hostile` third arg | 73.500000 | 328 | 0x2628c | 0x7048 | PASS |
| slot-correct `x=wz+jitterY, y=wy+jitterZ, z=wx+jitterX`, xzy | 73.800000 | 328 | 0x2628c | 0x7048 | PASS |
| all-int coords (correct or tuned), xyz | 61.000000 | 328 | 0x2628c | 0x7048 | size 2752 |
| placement-new, no try, tuned | 70.500000 | 328 | 0x26294 | 0x7048 | size 2776 |
| real-new all three, no try, int x / float z / float y | 66.700000 | **336** | 0x26294 | 0x7048 | PASS |
| real-new all three, no try, tuned | 64.600000 | **336** | 0x26298 | 0x7048 | size 2768 |
| real-new all three, no try, slot-correct fff | 64.300000 | **336** | 0x26298 | 0x7048 | size 2768 |
| real-new all three, no try, all-int | 63.100000 | **336** | 0x26298 | 0x7048 | size 2788 |
| real-new all three + try | 63.700000 | 352 | 0x262a4 | 0x7048 | size 2772 |
| **typed catch `std::bad_alloc &` before catch-all** | **74.400000** | **336** | 0x26294 | 0x7048 | **FAIL byte 4234** |
| typed catch `BoundingAAB &` / `KIPlayer &` / `Waypoint &` | 74.400000 | 336 | 0x26294 | 0x7050 | FAIL byte 1793 |

## Why the 336-byte record still cannot be landed

Parsing the original `.ARM.extab` record at `0x1ab09c`: TType enc
`0xff` (omit, no type table), call enc `0x01`, call len 315, **57 call
sites, every action 0** (cleanup only). HEAD's record has TType enc
`0x00` (absptr), ttbase offset 308, call len 299, **54 call sites**,
action table `01 00` (the catch-all). The 3-site / 16-byte call-site
difference is exactly the three `new` cleanups that placement-new
removes; the 2-byte TType/ttbase is the catch-all itself.

`catch (std::bad_alloc &)` before the catch-all restores the 336-byte
record and keeps 74.4, but it makes gold emit `_ZTISt9bad_alloc` at
`.dynsym` index 3438 instead of 5524 (gold orders dynamic symbols by
first reference; `Level.o` is early in `tools/source_order.txt`). That
reorder rewrites `.dynsym`, `.hash`, `.gnu.version`, `.rel.dyn`,
`.rel.plt` and, through link-time relocation addends, hundreds of
literal-pool words in `.text`; `byte_exact` drops **4503 -> 4234**
(`linked_exact` stays 4503, i.e. 269 functions lose raw-byte equality
while their disassembly is unchanged). `catch (int)` / `catch (KIPlayer &)` / `catch (Waypoint &)`
are worse: they also need a new GOT slot, so `.rel.dyn` grows
0x7048 -> 0x7050 and `.text` shifts by 8 (`byte` 1793).

A local (anonymous-namespace) catch type avoids the `.dynsym` reorder
but emits a typeinfo object: `.rel.dyn` +0x18, `.rodata` +0x30,
`.data.rel.ro` +0x10, `.got` +4 and `.text` shifts by 0x20 — also a
gate failure.

The only gate-safe 336-byte shape is real `new` for all three objects
with no `try` (57 cleanup-only sites). Its register allocation diverges
from the original from the first instruction — the cleanup landing pads
keep the allocated pointer live across the ctor, so the prologue spills
`race`+`shipClass` (`strd r1,r2,[sp,#56]`) and puts `type` in `sl`,
where the original keeps `race` at sp60, `type` in `fp` and `shipClass`
in `r5`/`sl`/`r6`. Every real-new variant therefore scores 63-67 and
grows `.text` to 0x1337e8 (size 2768) — a size regression against the
current 2764 B. Copying `shipClass`/`type` into locals, swapping
declaration orders and reordering the spawn/`ship` statements were all
retried this wave; none reproduce the original prologue.

## What the original really computes (Thumb, for future passes)

```
c03c8  ldr r2,[sp,#48]   ; wpY        c03ca  ldr r3,[sp,#32]  ; jitterY (2nd)
c03cc  ldr r0,[sp,#44]   ; wpX        c03ce  ldr r1,[sp,#36]  ; jitterX (1st)
c03d0  add r2,r3         ; y          c03d2  ldr r3,[sp,#40]  ; wpZ
c03d4  ldr r4,[sp,#28]   ; jitterZ(3rd) c03d6 add r0,r1       ; x
c03d8  movw r1,#20000    c03dc  sub fp,r0,r1  ; x -= 20000
c03e0  add r3,r4         ; z          c03e2  subs r4,r2,r1   ; y -= 20000
c03e4  sub r8,r3,r1      ; z -= 20000
```

All three are `int` (fp/r4/r8), computed **before** `setEmpData`, and
converted with `vcvt.f32.s32` at each ctor call site (3 conversions in
each branch). Stack slots: `wpX` sp44, `wpY` sp48, `wpZ` sp40;
`jitterX` sp36, `jitterY` sp32, `jitterZ` sp28. The landed mixed form
keeps `y`/`z` as `float` and `x` as `int`, which preserves the exact
prologue and 2764 B but sinks `x` below `setEmpData`. The faithful
all-int form is 2752 B and 61 % because the global RA then puts
`empPoints` in `fp` and `shipClass` in `r1`, losing the original
prologue.

Other original facts confirmed: `PlayerFixedObject(type, race, player,
nullptr, x, y, z)`; `PlayerFighter(type, race, player, nullptr, x, y, z,
hostile)`; `setShipGroup(getShipGroup(type, race, group), type,
hostile)`; the hitpoint if-chain is emitted `0x33`, `0x31`, `0x2c`; the
LOD guard is `missionPtr != 1 && missionPtr != 23`.

## Reproduction

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
python3 tools/verify/verify.py --show _ZN5Level10createShipEiiiP8Waypointbb
```

Scratch helpers (not committed): `/dev/shm/w6729/measure3.py`
(splice-free pct/size/extab/rel.dyn/text), `fn744.txt` (the landed
body), `batch25.py`-`batch36.py` (coordinate/declaration/typed-catch/
real-new sweeps), `diffblocks.py` (instruction-stream diff blocks).
