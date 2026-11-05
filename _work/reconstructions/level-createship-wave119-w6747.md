# Level::createShip — wave 119 (w6747): spawn-shape / LSDA sweep, no source win

Status: **dead end** (2026-11-05). Target
`_ZN5Level10createShipEiiiP8Waypointbb` (`Level::createShip`, original 2764 B
@ `0xc0214`, Thumb entry `0xc0215`). Branch `wave119-w6747`, base
`d2a5ce778`. No source change landed; the tree is unchanged apart from this
note.

## Result

```
createShip fuzzy (raw)            74.400000  (unchanged)
createShip size                   2764 B      (unchanged)
createShip extab entry            328 B       (original: 336 B)
.ARM.extab size / addr            0x26294 @0x1a3b78 (original 0x26298)
.text                             0x1337c8    (unchanged)
byte_exact / linked_exact         4505 / 4505 (unchanged)
imports / data / lint / drift     207 / 843 / CLEAN / 0
```

`check`: **RATCHET PASS**. No section, base, import, data or symbol movement.

## Where the .ARM.extab 4-byte gap actually is (measured, not assumed)

The wave-118 note said "createShip -8 only, Generator::createMission exact at
172 B". That is no longer true after the wave-119 createMission passes. Parsing
`.ARM.exidx` -> `.ARM.extab` per function (`prel31` on the exidx word 2, entry
size = next entry target - target) gives:

| function | original | ours | delta |
|---|---:|---:|---:|
| `Generator::createMission(Agent*, Array<SolarSystem*>*)` | 172 | 176 | **+4** |
| `Level::createShip(int,int,int,Waypoint*,bool,bool)` | 336 | 328 | **-8** |
| every other entry | — | — | 0 |

So the section is 4 B short because createShip is 8 B short and createMission
is 4 B over. Closing createShip alone (+8) would overshoot the section to
`0x2629c`; a createShip entry of exactly **332 B** would make the section
exactly `0x26298` (measured below).

### LSDA structure (parsed from the extab bytes)

Original entry @ `0x1ab09c` (336 B): 16-byte header, then LSDA at +0x10 with
`LPStart=0xff`, **`TType=0xff` (omit -> no type table)**, `callSite=0x01`,
table len 315, **57 call sites, every action 0** (cleanup only). All 57 sites
are `new`-expression cleanups (`operator delete` + `_Unwind_Resume`); the
original has **no catch handler at all**.

Ours @ `0x1ab0a0` (328 B): LSDA at +0x12, `LPStart=0xb4`, **`TType=0x02`
(uleb -> 1-entry type table)**, `callSite=0x01`, table len 299, **54 sites,
actions {0, 1}** (the `catch (...)`). The 3 missing sites are the three
`new` cleanups that placement-new removes; the type table + action entry is
the catch-all.

## What was swept this wave (all measured with a full link)

`pct` is the official `tools/verify/verify.py` fuzzy ratio; `extab` is the
whole `.ARM.extab` section size. Baseline row in bold.

| variant | pct | size | extab | text |
|---|---:|---:|---:|---:|
| **HEAD: placement-new + try, tuned spawn (x int, y/z float)** | **74.4** | 2764 | 0x26294 | 0x1337c8 |
| jitter declarations X,Y,Z (original order) + tuned spawn | 74.2 | 2764 | 0x26294 | 0x1337c8 |
| faithful all-int spawn (`wpX+jX`, `wpY+jY`, `wpZ+jZ`), jitter YZX | 61.0 | 2752 | 0x26294 | 0x1337c8 |
| faithful all-int spawn, jitter XYZ | 61.6 | 2752 | 0x26294 | 0x1337c8 |
| faithful all-int spawn, jitter XYZ, no try | 61.7 | 2748 | 0x26288 | 0x1337c8 |
| faithful all-int + real new x3, no try | 63.1 | 2788 | 0x2629c | 0x1337e8 |
| faithful all-int + real new x3 + try | 61.6 | 2792 | 0x262ac | 0x1337e8 |
| faithful mixed (`wpX+jX` int, `wpY+jY`/`wpZ+jZ` float), jitter XYZ | 73.6 | 2764 | 0x26294 | 0x1337c8 |
| all six all-int statement orders (xyz..zyx) | 60.7-61.6 | 2752-2756 | 0x26294 | 0x1337c8 |
| all-int spawn before `new Player` / after the try | 61.6 | 2752 | 0x26294 | 0x1337c8 |
| mixed spawn before `new Player` | 60.8 | 2776 | 0x26294 | 0x1337e8 |
| mixed: `x` line moved before `new Player` / after the try | 74.4 | 2764 | 0x26294 | 0x1337c8 |
| Java-shaped `int spawnX = waypoint ? waypoint->x : 0;` + all-int | 61.6 | 2752 | 0x26294 | 0x1337c8 |
| Java-shaped + mixed | 73.6 | 2764 | 0x26294 | 0x1337c8 |
| Java-shaped `spawnX += nextInt(40000) - 20000` | 49.3 | 2740 | 0x2629c | 0x1337c8 |
| `int ix/iy/iz` then `float x/y/z` | 64.8 | 2748 | 0x26294 | 0x1337c8 |
| `float y = (float)(wy + jitterY) - 20000.0f` | 70.8 | 2772 | 0x26294 | 0x1337e8 |
| `x -= 20000;` / `-20000 + ...` / `const int off` / `const int x` | 74.4 | 2764 | 0x26294 | 0x1337c8 |
| `const float zero = 0.0f;` used as the 4th `BoundingAAB` arg | 74.4 | 2764 | 0x26294 | 0x1337c8 |
| `Array<BoundingVolume*> *volumes` allocated **before** `fixed` | 59.3 | 2760 | **0x26298** | 0x1337c8 |
| `volumes` declared before, allocated after `fixed` | 74.4 | 2764 | 0x26294 | 0x1337c8 |
| `ship = 0` / `ship = (KIPlayer*)fighter` / `static_cast` return | 74.4 | 2764 | 0x26294 | 0x1337c8 |
| x expression sweep: `jitterX+jitterZ` | 74.4 | 2764 | 0x26294 | 0x1337c8 |
| x = `wz+jitterX` (faithful) / `wx+jitterX` / `wx+jitterX+0` | 40.0 | 2760 | **0x26298** | 0x1337c8 |
| x = `wy+jitterX` | 64.7 | 2780 | 0x26294 | 0x1337e8 |
| x = `jitterX+jitterY` / `jitterY+jitterX` / `jitterZ+jitterY` / `jitterY+jitterZ` / `wz+jitterZ` | 39.4-39.8 | 2756-2760 | **0x26298** | 0x1337c8 |
| y = `wx+jitterY` / `wz+jitterY` / `jitterY+jitterX` | 69.2-69.3 / 68.8 | 2748 | 0x26294 | 0x1337c8 |
| y = `wy+jitterX` / `wy+jitterZ` / `wy+wy` | 39.4-39.9 | 2760-2768 | **0x26298** | 0x1337c8 |
| z = `wx+wz` | 74.4 | 2764 | 0x26294 | 0x1337c8 |
| z = `wy+wx` / `wx+wx` / `wz+wz` / `wx+jitterZ` / `wz+jitterZ` | 68.7-69.4 | 2748-2760 | 0x26294 | 0x1337c8 |
| z = `jitterZ+jitterX` | 42.2 | 2728 | 0x26294 | 0x1337a8 |

`empPoints` rewrites (`40 + x*5`, parens) and `empTime` declaration moves are
codegen-identical at 74.4.

## Why the faithful all-int spawn cannot be landed

The original really is all-int: `fp = wpX + jitterX - 20000`,
`r4 = wpY + jitterY - 20000`, `r8 = wpZ + jitterZ - 20000`, computed **before**
`setEmpData` and converted with `vcvt.f32.s32` at each of the six ctor call
sites. But every all-int source shape we can write makes clang 7.0.2 pick a
different *entry* register allocation and then **sink** the coordinate
computation below `setEmpData`:

```
original                     ours (all-int)
str r1,[sp,#60]  ; race      str r2,[sp,#60]  ; shipClass
mov r9, r0       ; this      mov r9, r3       ; type
mov fp, r3       ; type      str r0,[sp,#56]  ; this
mov r5, r2       ; shipClass mov sl, r1       ; race
...
add sl, r0, #40  ; empPoints add fp, r0, #40  ; empPoints
str r9, [sp,#56] ; spill this str sl, [sp,#52]
```

That rotates the GPR assignment for the whole 24-`BoundingAAB` body
(original `x=sl, z=r4, volumes=fp, zero=r6`; ours `x=r6, z=sl, volumes=r9,
zero=r0`), which is where most of the 25 % mismatch lives. Statement order,
declaration order, the Java `spawnX += ...` shape, `const`, declaration-vs-
assignment split, spawn position relative to `new Player`/the try, and real
`new` vs placement-new were all tried; none reproduces the original prologue.

The mixed form (int `x`, float `y`/`z`) is a strong local optimum: it keeps the
entry RA identical to the original through `0xc03c4` (the `Player` ctor call),
and every single-coordinate expression/type/order perturbation measured this
wave is worse (best alternative: 73.6).

## The 336-byte entry is reachable, but not with this score

* `Array<BoundingVolume*> *volumes` allocated before `fixed` gives a **332 B**
  createShip entry and makes the whole section exactly `0x26298`, but the code
  shrinks to 2760 B and the score collapses to 59.3.
* Real `new` for all three objects (no try) gives the exact 336 B cleanup-only
  LSDA shape but 63-67 % and `.text` 0x1337e8.
* A typed catch (`catch (std::bad_alloc &)` before `catch (...)`) restores a
  336 B entry and keeps 74.4, but it makes gold emit `_ZTISt9bad_alloc` at
  `.dynsym` ~3325 instead of 5524 (Level.o is TU #113) and rewrites
  `.dynsym`/`.hash`/`.rel.dyn`, collapsing `byte_exact` to ~3851. `Level.o`
  currently references no `_ZTI*` at all, and the only typeinfos already in
  `.got` (`_ZTISt9bad_alloc`, `_ZTISt9exception`, `_ZTIDn`, `_ZTIv`, the
  `__cxxabiv1` ones) all sit at dynsym >= 5316, i.e. after Level.o, so *any*
  typed catch reorders dynsym. `catch (KIPlayer &)` / `catch (BoundingAAB &)`
  additionally need a fresh GOT slot (`.rel.dyn` 0x7048 -> 0x7050, `.text`
  shifts by 8).

## Reproduction

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
python3 tools/verify/verify.py --show _ZN5Level10createShipEiiiP8Waypointbb
```

Scratch helpers (not committed): `/dev/shm/w6747/fastscore.py` (per-function
fuzzy score in <1 s, reusing `tools/verify/verify.py` internals),
`/dev/shm/w6747/m2.sh` (link + fast score + extab/text sizes),
`/dev/shm/w6747/diffblocks.py` (difflib diff blocks with instruction text),
`/dev/shm/w6747/extab.py` (per-function `.ARM.exidx`/`.ARM.extab` sizes),
`/dev/shm/w6747/v.py` (variant builder), `Level.baseline.cpp` (pristine copy).
