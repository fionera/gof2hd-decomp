# Level::createShip — wave 117 (w6505): placement-new + try shape with correct ctor order

Status: **source change landed** (2026-11-05).
Target `_ZN5Level10createShipEiiiP8Waypointbb` (`Level::createShip`,
original 2764 B @ `0xc0214`, Thumb entry `0xc0215`). Branch
`wave117-w6505`, base `c22678d64`.

## Result

```
                                   before (HEAD c22678d64)   after
createShip fuzzy (raw)             67.366317                 73.485601
createShip fuzzy (report, 1 dp)    67.4                      73.5
createShip size                    2764 B                    2764 B
createShip LSDA record             336 B                     328 B
.ARM.extab size / addr             0x26294 @0x1a3b78         0x2628c @0x1a3b78
.ARM.exidx size / addr             0x6f70 @0x19cc08          0x6f70 @0x19cc08
.text size / addr                  0x1337c8 @0x69440         0x1337c8 @0x69440
.rodata size                       0x41c27 (269351)          0x41c27 (269351)
file size                          2,187,328 (exact)         2,187,328 (exact)
section headers                    27 (exact)                27 (exact)
byte_exact / linked_exact          4501 / 4501               4501 / 4501
imports / data / lint / drift      207 / 843 / CLEAN / 0     207 / 843 / CLEAN / 0
```

`check` on the final tree: **RATCHET PASS**, byte 4501 / linked 4501,
avg 99.92, data 843/843, imports 207, lint CLEAN, drift 0.

**Section regression to report:** `.ARM.extab` moves 0x26294 → 0x2628c
(−8 B vs HEAD; the original is 0x26298, so the residual grows from −4 B
to −12 B). Cause: the placement-new shape removes the `new` cleanup
records for `Player`/`PlayerFighter`/`PlayerFixedObject`, and the
`try/catch(...)` replaces them with a catch record; the createShip LSDA
record shrinks 336 B → 328 B. `.text`, `.ARM.exidx`, `.rodata`, `.data`,
`.bss`, the 27 section headers and the 2,187,328 B file size are unchanged.

## Landed diff

Four independent, ordinary-C++ changes in `src/game/world/Level.cpp`
(`Level::createShip` only):

1. `PlayerFighter` allocation switched from real `new` to placement new:
   ```cpp
   PlayerFighter *fighter = (PlayerFighter *) ::operator new(sizeof(PlayerFighter)); // lint: operator_call
   new (fighter) PlayerFighter(type, race, player, nullptr, x, y, z, hostile);
   ```
2. Same placement-new for `PlayerFixedObject` (the argument order stays
   the semantically correct `(type, race, ...)`, not wave-89's
   `(race, type)`).
3. `player->setEmpData(empPoints, empTime);` wrapped in
   ```cpp
   try {
       player->setEmpData(empPoints, empTime);
   } catch (...) {
       __builtin_trap(); // lint: arm_builtin
   }
   ```
4. Spawn declarations reordered to `int x …; float z …; float y …;`
   (z before y), jitter call order **Y, Z, X**, and waypoint declaration
   order `int wpZ, wpX, wpY;` (assignments unchanged: `wpX←x`,
   `wpY←y`, `wpZ←z`).

Semantics are unchanged from the correct body: `x = wpX + jitterX`,
`y = wpY + jitterY`, `z = wpZ + jitterZ`; `getShipGroup(type, race,
group)`; `setShipGroup(..., type, hostile)`; `PlayerFixedObject(type,
race, …)`.

Provenance: this is the wave-89 (`36c65db70`) body shape (placement-new
+ try) with the correct `PlayerFixedObject(type, race, …)` argument
order, plus the tuned jitter/declaration orders found this wave. The
wave-89 body verbatim (wrong `(race, type)`) scores 74.143921 but is a
real bug; the fully correct variant lands at 73.485601.

## Why this shape

* The 67.4 HEAD body is the plain real-`new` body. Greedy RA picks
  `status→r6` instead of `r4` and spills `shipClass`, so the whole
  function is a near-miss.
* The `try` anchor pins `status→r4`, `type→fp`, `shipClass→r5` and
  keeps the body at 2764 B. Removing it (plain `setEmpData`) drops to
  70.238095 and grows the body to 2776 B.
* Placement-new keeps the body at 2764 B with the try; with real `new`
  plus the try the body grows to 2768–2772 B and the score falls to
  ~63.5.
* With this shape the prologue and the whole stats block are
  instruction-identical to the original:
  `str r1,[sp,#60] / mov r9,r0 / ldr r0,[pc,#572] / mov fp,r3 /
  mov r5,r2 / add r0,pc / ldr r4,[r0,#0] / ldr r0,[r4,#0] /
  blx getCurrentCampaignMission / mov r8,r0 / ldr r0,[r7,#8] /
  mov sl,r5 / cbz r0`, and the jitter block
  (`str r0,[sp,#36] / str r0,[sp,#32] / mov r5,r0 /
  str r5,[sp,#28] / ldr r0,[r4,#0] / cmp r0,#20 / str r5,[sp,#28]`)
  matches too.

## Residual (what is still different)

* **Spawn block.** The original computes all three coordinate ints
  (`fp=x`, `r4=y`, `r8=z`) *before* `setEmpData` and converts them at
  each ctor call site. Our source computes `y`/`z` as floats, so clang
  sinks the int add + `vcvt` for `y`/`z` into the capital branch and
  computes `x` after the call. Making all three `int` fixes the order
  but destroys the RA (`this` spills in the prologue, body 2752 B,
  60.5–61.0 %); making all three `float` is worse (61.2 %). The
  `int x` + `float y,z` split is the best trade-off.
* **Capital-branch register permutation.** Original converts
  `x→s20`, `y→s18`, `z→s16` and keeps `x` in `sl`, `y` in `r8`, `z` in
  `r4`; ours converts `x→s16`, `y→s18`, `z→s20` and keeps `x` in `r4`,
  `y` in `r6`, `z` in `sl`. The stack-argument slots are correct.
* **Waypoint stack slots.** Original `wpX→sp44`, `wpY→sp48`,
  `wpZ→sp40`; ours `wpX→sp32`, `wpY→sp48`, `wpZ→sp44`. All six
  declaration orders were swept; `wpZ,wpX,wpY` and `wpZ,wpY,wpX` tie
  at 73.485601, every other order is lower.
* **`setShipGroup` third argument.** The original passes `hostile`
  (`r8` after the `PlayerFighter` ctor). Passing `group` instead scores
  73.945409 but is wrong; it is the single biggest remaining cheap
  point and was deliberately not taken.

## Measurement table (official scorer, full link)

All rows are `raw %` from `tools/verify/verify.py`'s
`difflib.SequenceMatcher` ratio on the reconciled instruction streams;
`size` is `usize`; `n` is orig/ours instruction count.

| variant | raw % | size | n |
|---|---:|---:|---|
| **HEAD baseline** | 67.366317 | 2764 | 1007/994 |
| wave-89 body verbatim (wrong `(race,type)`) | 74.143921 | 2764 | 1007/1008 |
| wave-89 body, ctor arg order fixed | 74.044665 | 2764 | 1007/1008 |
| wave-92 body (x/z swapped, `group`) | 74.379345 | 2764 | 1007/1007 |
| wave-92 + spawn fix (`group`) | 73.746898 | 2764 | 1007/1008 |
| wave-92 + hostile | 73.919523 | 2764 | 1007/1006 |
| **wave-89 body + arg fixed + z/y spawn + jitter YZX + `wpZ,wpX,wpY`** | **73.485601** | **2764** | **1007/1007** |
| same, `wpZ,wpY,wpX` decl | 73.485601 | 2764 | 1007/1007 |
| same but jitter XYZ / YXZ | 73.386296 | 2764 | 1007/1007 |
| same but jitter YZX w/o wp reorder | 72.293942 | 2764 | 1007/1007 |
| same but spawn XZY / ZXY / ZYX | 73.485601 | 2764 | 1007/1007 |
| same but spawn XYZ / YXZ / YZX | 73.286991 | 2764 | 1007/1007 |
| same but `setShipGroup(..., group)` | 73.945409 | 2764 | 1007/1008 |
| same but `setShipGroup(..., group)` + jitter YZX (V1) | 73.945409 | 2764 | 1007/1008 |
| same but hitpoints `switch` instead of if-chain | 73.485601 | 2764 | 1007/1007 |
| same but cargo `if` instead of `switch` | 73.485601 | 2764 | 1007/1007 |
| same but `KIPlayer *kiFighter` intermediate | 73.485601 | 2764 | 1007/1007 |
| same but `if (waypoint)` / chained zero init | 73.485601 | 2764 | 1007/1007 |
| same but spawn moved before `new Player` | 60.490736 | 2776 | 1007/990 |
| same but `wpX,wpY,wpZ` decl | 72.230502 | 2764 | 1007/1006 |
| both placement-new, **no try** | 70.238095 | 2776 | 1007/1009 |
| both placement-new + try, spawn `x,y,z` | 71.932439 | 2764 | 1007/1006 |
| same + z before y | 72.194638 | 2764 | 1007/1007 |
| all-int spawn (6 orders) | 60.54–60.96 | 2752 | 1007/1004–1005 |
| all-float spawn | 61.232604 | 2768 | 1007/1005 |
| int temps + all-float x/y/z | 60.83–61.23 | 2772 | 1007/1005 |
| real new + try (HEAD body) | 63.500000 | 2768 | 1007/993 |
| placement-new fixed only + try | 61.080813 | 2772 | 1007/1010 |
| placement-new fighter only + try | 63.300000 | 2768 | 1007/993 |
| real new Player only (rest placement) | 64.711730 | 2760 | 1007/1005 |
| real new fighter only (rest placement) | 61.607143 | 2768 | 1007/1009 |
| real new fixed only (rest placement) | 62.968516 | 2768 | 1007/994 |
| real new Player+fighter | 62.959285 | 2764 | 1007/1007 |
| real new Player+fixed | 64.332166 | 2764 | 1007/992 |
| real new fighter+fixed | 63.168416 | 2768 | 1007/994 |
| real new all three | 63.168416 | 2768 | 1007/994 |
| try/empty catch | 64.767616 | 2760 | 1007/994 |
| try/rethrow | 49.173761 | 2744 | 1007/990 |

`.ARM.extab` sizes for the real-new variants: all-placement 0x2628c,
real Player 0x26288, real fighter 0x26298, real fixed 0x26298,
Player+fighter 0x26294, Player+fixed 0x26294, fighter+fixed 0x262a4,
all three 0x2629c, empty catch 0x2628c, rethrow 0x262a0. None of the
0x26298 rows beat 73.485601.

## Reproduction

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 tools/verify/verify.py --show _ZN5Level10createShipEiiiP8Waypointbb
cmake --build cmake-build-match --target check -j6
```

Scratch helpers used this wave (not committed): `/dev/shm/w6505/cur.cpp`
(the landed body snapshot), `gen.py`/`sweep1.py` (declaration/jitter/
spawn permutation sweeps), `splice.py` (function-body splicing),
`lsda.py`/`norm.py`/`diffA.txt` (LSDA + instruction-stream diffing).
