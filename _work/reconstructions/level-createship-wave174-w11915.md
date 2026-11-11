# `Level::createShip` — wave 174 (w11915): cleanup-only 336 B LSDA with the invoke barrier

Worktree `wt/w11915`, branch `wave174-w11915`, base `master be9dd429b`
(wave 173).  Target `_ZN5Level10createShipEiiiP8Waypointbb`
(`src/game/world/Level.cpp`, 2,764 B, VMA `0xc0214`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2 `-Oz`).

**Status: source change landed.**  `createShip` raw **1,560 -> 1,232**
(-328), aligned **530 -> 422**, size **2,764/2,764** exact, insns
**1,118/1,117**.  The function's `.ARM.extab` record is back to the original
**336 B / 57 rows / `ttype 0xff`** cleanup-only shape (cs_len 315, every
action 0), so `.ARM.extab` is size- and base-exact again and `.ARM.exidx` is
byte-identical.

## 1. Result

| metric | before (wave 173) | after |
|---|---:|---:|
| raw same-address diff | **1,560** | **1,232** |
| instruction-index alignment | **530** | **422** |
| size | 2,764 | 2,764 |
| insns (orig/ours) | 1,117 / 1,118 | 1,117 / 1,118 |
| createShip LSDA record | 336 B / 57 rows / `ttype 0xff` | **336 B / 57 rows / `ttype 0xff`** |
| createShip LSDA `cs_len` | 315 | 315 |
| `.text` differing bytes | 80,897 | **80,569** |
| `.ARM.extab` size | `0x26298` (exact) | `0x26298` (exact) |
| `.ARM.extab` differing bytes | 675 | 746 |
| `.ARM.exidx` differing bytes | 0 | **0** |
| `.rel.plt` | byte-identical | **byte-identical** |
| file size | 2,187,328 | 2,187,328 (exact) |

Full `check`: `byte_exact 4507/4524`, `linked_exact 4507/4524`,
size-exact 4524/4524, stub 0, missing 0, wrong_type 0, extra 0,
data 843/843, imports 207, lint CLEAN, drift 0, symbol parity 5370/5370,
`RATCHET PASS`.

## 2. Why the wave-173 `try`/`catch` record was 8 B short

w11817's landed shape wraps `setEmpData` in

```cpp
try { player->setEmpData(empPoints, empTime); }
catch (...) { __builtin_trap(); }
```

That makes `setEmpData` an invoke (the scheduling barrier that keeps the
coordinate block above the call), but `catch (...)` adds a catch-all type
table entry.  The resulting LSDA is **328 B / 54 rows / `ttype 0x00`**:
the type table costs 8 bytes and the two placement-`new` sites have no
`operator delete` cleanup pads (26 `new` pads + 1 catch pad = 27 landing-pad
rows, 27 no-pad rows).  The original is a **cleanup-only** record:
`ttype 0xff` (no type table), 57 rows, 28 landing pads (every `new`
expression) + 29 no-pad rows.  The 8-byte record shortfall shifts every
later `.ARM.exidx` prel31 entry (0 -> 1,327 differing bytes) and moves
`.ARM.extab` to `0x26290`.

## 3. The landed shape: a cleanup guard instead of a catch

The barrier does not need a catch clause.  A local object with a non-trivial
destructor whose lifetime spans the call makes the call an invoke with a
**cleanup** landing pad, and the personality stays `__gxx_personality_v0`
with `ttype 0xff` (no type table, no `.rel.dyn` relocation, so the PLT and
every `.text` PLT call stay put).  The landed source defines one local RAII
type and uses it in three scopes:

```cpp
struct EmpGuard {
    ~EmpGuard() {}
};
int mission;
{
    EmpGuard guard;
    mission = Globals::status->getCurrentCampaignMission();
}
...
{
    EmpGuard guard;
    player->setEmpData(empPoints, empTime);
}
...
{
    EmpGuard guard;
    ship->setLevel(this);
}
```

The `setEmpData` guard is the scheduling barrier: it reproduces the exact
w11817 prologue (`str r1,[sp,#60]` race, `mov r9,r0`, `mov fp,r3` type,
`mov r5,r2` shipClass, `mov sl,r5`; the first differing byte is still
`0x2c`, i.e. the first 44 bytes are byte-identical).  The other two guards
are real EH scopes that add the rows needed to reach the original record
size:

| shape | rows | record | `ttype` | raw | size |
|---|---:|---:|---|---:|---:|
| wave-173 `try`/`catch(...)` around `setEmpData` | 54 | 328 | 0x00 | 1,199 | 2,764 |
| one guard around `setEmpData` | 54 | 320 | 0xff | 1,454 | 2,760 |
| + guard around `mission` | 55 | 328 | 0xff | 1,227 | 2,764 |
| + guard around `mission` + `empPoints` | 57 | 336 | 0xff | 1,233 | 2,764 |
| **+ guard around `mission` + `ship->setLevel`** | **57** | **336** | **0xff** | **1,232** | **2,764** |
| + guard around `mission` + `getLevel` clamp | 57 | 336 | 0xff | 1,257 | 2,764 |
| + guard around `mission` + `gameWon` | 57 | 336 | 0xff | 1,261 | 2,764 |
| + guard around `mission` + `fighter->setShipGroup` | 57 | 336 | 0xff | 1,232 | 2,764 |

The `ship->setLevel` guard is the cheapest second scope (it is the last call
in the function, so its landing-pad thunk sits at the end of the code and
shifts the least).  The `mission` guard is the only single-call scope that
adds exactly one row; the `empPoints`/`getLevel`/`gameWon`/`inAlienOrbit`
scopes contain two calls and add two rows.

The rest of the body is the wave-131 text that w11817 landed: `int wpz, wpx,
wpy;` if/else waypoint block, `jitter1/2/3`, type-switch order
`0x2c,0x33,0x31`, the five-way mission if-chain, `empPoints` as one
expression, placement `new` for `PlayerFighter`/`PlayerFixedObject`, direct
`x`/`y`/`z` ctor arguments, and no `Globals::status == nullptr` guard.

## 4. Why not a typed catch

`catch (KIPlayer &)` + `catch (...)` also gives a 336 B record (1,238 raw,
2,764 B exact), but the type table references `_ZTI8KIPlayer`, adding one
`.rel.dyn` relocation.  That grows `.rel.dyn` by 8 bytes, moves `.rel.plt`
and `.plt` by 8 bytes, and changes every `bl` PLT immediate in `.text`
(`.text` differing bytes 80,897 -> 134,033).  `catch (int)` is worse: it
adds an undefined `_ZTIi` import/PLT entry.  The cleanup guard has no type
table and no relocation, so the PLT and `.rel.plt` stay byte-identical.

## 5. Variants measured (full link, size-checked)

| variant | raw | aligned | size | record |
|---|---:|---:|---:|---|
| wave-173 base | 1,560 | 530 | 2,764 | 336 / 57 / 0xff |
| w11817 placement new + `try`/`catch(...)` | 1,199 | 351 | 2,764 | 328 / 54 / 0x00 |
| placement new + guard around `setEmpData` | 1,454 | 419 | 2,760 | 320 / 54 / 0xff |
| plain new + guard around `setEmpData` | 2,336 | 460 | 2,768 | 344 / 58 / 0xff |
| plain new + `try`/`catch(...)` | 2,333 | 461 | 2,768 | 352 / 58 / 0x00 |
| coordinates before `new Player`, plain new, no try | 2,550 | 539 | 2,788 | 336 / 57 / 0xff |
| placement new + `catch(KIPlayer&)`+`catch(...)` | 1,238 | 355 | 2,764 | 336 / 54 / 0x00 |
| **landed: placement new + 3 cleanup guards** | **1,232** | **422** | **2,764** | **336 / 57 / 0xff** |

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w11915
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level10createShipEiiiP8Waypointbb
cmake --build cmake-build-match --target check -j2
cmake --build cmake-build-match --target drift -j2
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w11915/`: `Level_base.cpp`
(pristine wave-173 `Level.cpp`), `Level_w131.cpp` (wave-131 function),
`mkclean.py` (landed-shape generator), `measure.sh` (build + raw/aligned/
size + LSDA row dump), `lsda_any.py` (generic LSDA parser), plus the
variant generators `mk*.py`.
