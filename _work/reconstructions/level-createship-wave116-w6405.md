# Level::createShip — wave 116 (w6405): real-new allocation sweep, no landable gain

Status: **analysis only, no source change landed** (2026-11-05).
Target `_ZN5Level10createShipEiiiP8Waypointbb` (`Level::createShip`,
original 2764 B @ `0xc0214`, Thumb entry `0xc0215`). Branch
`wave116-w6405`, base `d672d349f`.

## Result

```
                                   before (HEAD d672d349f)   after
createShip fuzzy (raw)             67.366317                 67.366317
createShip fuzzy (report, 1 dp)    67.4                      67.4
createShip size                    2764 B                    2764 B
createShip LSDA record             336 B / 57 rows           336 B / 57 rows
.ARM.extab size / addr             0x26294 @0x1a3b78         0x26294 @0x1a3b78
.ARM.exidx size / addr             0x6f70 @0x19cc08          0x6f70 @0x19cc08
.text size / addr                  0x1337c8 @0x69440         0x1337c8 @0x69440
.rodata size                       0x41c27 (269351)          0x41c27 (269351)
file size                          2,187,328 (exact)         2,187,328 (exact)
section headers                    27 (exact)                27 (exact)
byte_exact / linked_exact          4501 / 4501               4501 / 4501
imports / data / lint / drift      207 / 843 / CLEAN / 0     207 / 843 / CLEAN / 0
```

No variant beat the baseline. The gate was re-run on the pristine source
after the sweep: `RATCHET PASS`, byte 4501 / linked 4501, avg 99.92,
data 843/843, imports 207, lint CLEAN, drift 0.

Note: `.ARM.extab` is currently **0x26294** (4 B short of the original
0x26298), not exact. The remaining delta is `Generator::createMission`
(168 B vs 172 B); `Hud::draw` is exact (476 B) and `createShip` is exact
(336 B / 57 rows). This is the wave-115 harvest state, not a regression
from this pass.

The scorer is `tools/verify/verify.py`'s `score()`, which is
`round(difflib.SequenceMatcher(None, on, un, autojunk=False).ratio()*100, 1)`.
The raw ratio is quoted below because several variants round to the same
1-dp value.

## What was swept this wave

All measurements are full `link_so` + official verifier. `size` is
`usize`; `n` is orig/ours instruction count; `rec` is the createShip
`.ARM.extab` record size (original = 336 B / 57 rows).

| variant | raw % | pct | size | n | rec |
|---|---:|---:|---:|---|---:|
| **HEAD baseline** (`i/f/f`, coords after ctor, real new) | **67.366317** | **67.4** | 2764 | 1007/994 | 336 |
| `int x,y,z` (i/i/i) | 63.126551 | 63.1 | 2792 | 1007/1008 | 336 |
| coords `z` before `y` (i/f/f) | 67.166417 | 67.2 | 2764 | 1007/994 | 336 |
| coords after `setEmpData` in source (i/f/f) | 67.366317 | 67.4 | 2764 | 1007/994 | 336 |
| coords after `setEmpData` in source (i/i/i) | 63.126551 | 63.1 | 2792 | 1007/1008 | 336 |
| coords before `new Player` (i/i/i) | 62.332506 | 62.3 | 2792 | 1007/1008 | 336 |
| coords before `new Player` (i/f/f) | 62.151394 | 62.2 | 2788 | 1007/1001 | 336 |
| old wave-92 placement-new + try/trap body | 74.379345 | 74.4 | 2764 | 1007/1007 | 328 |
| old body + real new, no try | 64.635365 | 64.6 | 2768 | 1007/995 | 336 |
| old body + real new + try/trap | 63.736264 | 63.7 | 2772 | 1007/995 | 352 |
| current body + try/trap | 63.500000 | 63.5 | 2768 | 1007/993 | 352 |
| i/i/i + try/trap | 61.607143 | 61.6 | 2796 | 1007/1009 | 352 |
| i/i/i + try/empty catch | 64.218362 | 64.2 | 2776 | 1007/1008 | 352 |
| i/i/i + try/rethrow | 46.865672 | 46.9 | 2776 | 1007/1003 | 372 |
| `cargo_if` (current) | 67.366317 | 67.4 | 2764 | 1007/994 | 336 |
| cargo null-check | 65.134865 | 65.1 | 2768 | 1007/995 | 336 |
| `volumes` before `fixed` | 61.030515 | 61.0 | 2748 | 1007/992 | 336 |
| `cargo_if` + `volumes` before `fixed` | 61.030515 | 61.0 | 2748 | 1007/992 | 336 |
| cargo null + `volumes` before `fixed` | 61.600000 | 61.6 | 2752 | 1007/993 | 336 |
| `setShipGroup(..., group)` instead of `hostile` | 64.902646 | 64.9 | 2768 | 1007/996 | 336 |
| `int w[3]` / `int j[3]` arrays (i/f/f) | 66.966517 | 67.0 | 2764 | 1007/994 | 336 |
| `int w[3]` / `int j[3]` arrays (i/i/i) | 63.089916 | 63.1 | 2788 | 1007/1006 | 336 |
| faithful waypoint slots (`int wz,wx,wy`) i/f/f | 66.766617 | 66.8 | 2764 | 1007/994 | 336 |
| faithful waypoint slots i/i/i | 62.990561 | 63.0 | 2788 | 1007/1006 | 336 |
| `float zero = 0.0f` (5 positions/forms) | 67.366317 | 67.4 | 2764 | 1007/994 | 336 |
| `0` / `0.0` for the AAB `ex` argument | 67.366317 | 67.4 | 2764 | 1007/994 | 336 |
| `-0.0f` for the AAB `ex` argument | 65.234765 | 65.2 | 2784 | 1007/995 | 336 |
| explicit `float fx,fy,fz` before the branch | 57.057654 | 57.1 | 2784 | 1007/1005 | 336 |
| `PlayerFixedObject *ship` instead of `KIPlayer *` | 47.741935 | 47.7 | 2756 | 1007/1008 | 336 |
| `return static_cast<...>(ship)` / `ship = 0` | 67.366317 | 67.4 | 2764 | 1007/994 | 336 |
| `missionPtr` condition 4 forms (or/nested/bool/local) | 67.366317 | 67.4 | 2764 | 1007/994 | 336 |
| jitter decl XYZ / XZY | 67.366317 | 67.4 | 2764 | 1007/994 | 336 |
| jitter decl YXZ / YZX / ZXY / ZYX | 67.266–67.300 | 67.3 | 2764 | 1007/993–994 | 336 |
| spawn expression `jX+wpX-20000`, `-20000+wpX+jX`, `wpX-20000+jX` | 67.366317 | 67.4 | 2764 | 1007/994 | 336 |
| `int wpY=0,wpZ=0,wpX=0` init form | 67.366317 | 67.4 | 2764 | 1007/994 | 336 |
| `int wpX=0,wpY=0,wpZ=0` init form | 67.166417 | 67.2 | 2764 | 1007/994 | 336 |
| `if (waypoint)` instead of `!= nullptr` | 67.366317 | 67.4 | 2764 | 1007/994 | 336 |
| `empPoints` after `level`, before `hitpoints` | 63.487332 | 63.5 | 2760 | 1007/1006 | 336 |
| `empPoints` after `hitpoints`, before type chain | 65.667166 | 65.7 | 2764 | 1007/994 | 336 |
| `empPoints` after type chain, before mission block | 66.066967 | 66.1 | 2764 | 1007/994 | 336 |
| wave-115 target-prologue shape (`i/i/i`, ep before level, ternary time) | 62.462761 | 62.5 | 2800 | 1007/1007 | 336 |
| old body + real new + target-prologue shape | 63.005492 | 63.0 | 2772 | 1007/996 | 336 |
| `switch (race)` case order 0123 / 0321 / 0231 / 0132 | 67.366317 | 67.4 | 2764 | 1007/994 | 336 |

## Key findings

1. **The 336 B / 57-row LSDA is tied to real `new`.** The 74.4 % wave-92
   body uses placement `new` + `try { setEmpData } catch (...) { trap; }`
   and has a 328 B / 54-row record. Every `try` variant measured this wave
   gives 352 B (catch-all) or 372 B (rethrow), so the `try` anchor cannot
   be combined with the 336 B budget. The current real-new shape is the
   only 336 B family.

2. **The residual is register allocation, not source shape.** The original
   keeps the spawn `x`/`y`/`z` integers live across `Player::setEmpData`
   (`fp`/`r4`/`r8`) and converts them at the two ctor calls. Our real-new
   build computes `y`/`z` as floats before the call and sinks the `x`
   integer after it (PostRA machine sink); the spawn block is 4 B longer
   and the whole fixed branch shifts. Source reordering (before/after
   `new Player`, before/after `setEmpData`, expression forms) does not
   move it.

3. **Fixed-branch x↔zero swap.** In the case-0 AAB block the original
   allocates `x=sl`, `y=r8`, `z=r4`, zero=`r6`; ours allocates `x=r6`,
   `y=r8`, `z=r4`, zero=`sl`. Only `x` and the zero are swapped, and that
   costs two operand mismatches per `BoundingAAB` call (24 calls). A named
   `float zero = 0.0f` (any scope/const form), `0`, `0.0`, and explicit
   `float fx/fy/fz` do not move the allocator.

4. **The prologue shape is reachable but costs the getLevel order.**
   `empPoints` before `level` + ternary `empTime` reproduces the original
   `mov fp,r3` / `mov r5,r2` / `ldr r4` prologue (wave 115), but emits the
   `empPoints` `getLevel` pair before the `hitpoints` chain instead of
   after the mission block; the reorder costs ~5 points (62.5 vs 67.4).

5. **The literal-pool offsets are downstream of (2).** The 24 AAB calls
   each load 3–5 constants with `ldr rX,[pc,#N]`; the offsets differ by
   4–8 B because the spawn block is 4 B longer and the instruction count
   is 13 lower (994 vs 1007). Matching the spawn placement alone
   (coords before `new Player`) aligns the offsets but changes the global
   allocation and scores 62.2–62.3.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6405
cmake --build cmake-build-match --target check -j6
python3 tools/verify/verify.py \
  --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN5Level10createShipEiiiP8Waypointbb
```

Scratch helpers (all under `/dev/shm/w6405`): `Level.cpp.base` (pristine
source), `measure.sh` (full link + verify), `rawscore.py` (unrounded
official ratio), `recsize.py` (createShip LSDA record size from the
exidx/extab), `regionscore.py` (100-instruction region scores),
`idxdump.py` (side-by-side instruction dump), and the `var_*.py` sweeps
(`var_iii`, `var_batch1/2`, `var_744`, `var_744b`, `var_pro`, `var_order`,
`var_slots`, `var_struct`, `var_zero*`, `var_mp`, `var_jit`, `var_expr`,
`var_ship`, `var_wp`, `var_ep`, `var_case`).
