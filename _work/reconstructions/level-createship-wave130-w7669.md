# `Level::createShip` — wave 130 (w7669): guard/cleanup + zero-register hypothesis (no source win)

Target `_ZN5Level10createShipEiiiP8Waypointbb` (`Level::createShip`, original
2764 B @ `0xc0214`, Thumb entry `0xc0215`, 1007 insns).
Base `003a9623a` (wave-129 final). Worktree `/home/debian/GalaxyOnFire2/wt/w7669`.
Owned TU: `src/game/world/Level.cpp` (createShip only).

**Status: analysis only — no `src/` change landed.** The checked-in body stays the
best at **70.5 %** (70.541480 unrounded), 2764 B, 1007/1006 insns, exact 336 B /
57-row / `ttype 0xff` cleanup-only LSDA. This pass attacked the wave-129-w7648
"guard is the sink trigger / zero register is the 12 B" hypothesis directly and
found that **the guard is not what determines the exception pointer**; the
coordinate *type* (float vs int) is.

## Result

```
                                   original        HEAD (003a9623a)
createShip exact ratio              —               70.541480
createShip size                    2764 B          2764 B
createShip insns (orig/ours)       1007            1007 / 1006
createShip LSDA record             336 B           336 B
createShip LSDA rows               57              57
createShip LSDA ttype              0xff            0xff
byte_exact / linked_exact          —               4506 / 4506
data / imports / lint / drift      —               843 / 207 / CLEAN / 0
```

`check` on the unchanged tree: **RATCHET PASS** — `verify: byte 4506 linked
4506 stubs 0 missing 0 extra 0 avg 99.94`, `data: exact 843 of 843`,
`sodiff: imports 207`, lint `CLEAN`, drift 0.

## 1. Removing the guard does NOT flip the exception pointer

Wave-129-w7648 concluded the guard was the sink trigger. That is true for the
*coordinate placement* but **not** for the exception pointer. Measured with the
verifier's own scorer (`/dev/shm/w7669/exact.py`):

| variant | raw % | size | insns | exc reg | note |
|---|---:|---:|---:|---|---|
| **HEAD: fii coords after `new`, guard** | **70.5415** | 2764 | 1007/1006 | r6 | best |
| fii coords after `new`, **no guard** | 63.5589 | 2748 | 1007/988 | r6 | guard removed, still r6 |
| iii (all-int) coords after `new`, no guard | 60.0 | 2776 | 1007/1003 | r6 | faithful, still r6 |
| iii (all-int) coords after `new`, guard | 61.5 | 2772 | 1007/998 | r6 | |
| iii (all-int) coords **before** `new`, guard | 61.5 | 2772 | 1007/998 | r6 | |
| iii (all-int) coords **before** `new`, no guard | 61.5 | 2776 | 1007/1002 | r6 | |
| **fii coords before `new`, guard** | 62.8 | 2768 | 1007/998 | **r8** | flips pointer |
| fii coords before `new`, no guard | 58.3 | 2752 | 1007/988 | **r8** | |
| `Player *player;` declared first, assigned after coords | 62.8 | 2768 | 1007/998 | **r8** | same as above |

So the exception pointer is `r6` for **every** all-int body, with or without the
guard, and only flips to `r8` when the coordinate arithmetic is **float-typed**
(`float z, float y`) and is emitted before the `new Player` assignment. The
original is all-int (`vcvt.f32.s32` at the ctor call) and computes the coords
after the ctor, so neither faithful spelling gives `r8`.

This corrects wave-129-w7648 finding 1: the guard is a *compensating hack* that
reproduces the original's coordinate placement, but it is not the cause of the
r6/r8 split.

## 2. The zero-register 12 B is a consequence, not a lever

The original folds the 4th `BoundingAAB` argument into `strd r6,r0,[sp]` on the
even volumes (persistent `r6 = 0`). Ours rematerialises `movs r0,#0; str r0,[sp]`
on every volume. That is +12 B. The zero is only kept in a register when the
exception PHI is **not** in `r6`. All of these spellings folded back to the same
rematerialised `movs r0,#0` and changed nothing (70.5415 %, 2764 B, r6):

* `float zero = 0.0f;` at function scope / branch scope / `static const float`
* `float zero = (float)(x - x);`
* `float zero = Globals::options[0x2c] - Globals::options[0x2c];`
* `0.f` / `0` (int) instead of `0.0f`
* `Vector pos((float)x,...)` instead of `Vector pos = {...}`

In the no-guard all-int body the zero *does* get a register (`sl`) and the
`strd` fold happens — but the exception pointer is still `r6`, so the two
registers are simply swapped (`sl`=zero/`r6`=x instead of `r6`=zero/`sl`=x).

## 3. Try/cleanup shapes

A `try { ... } catch (...) { }` around the ship branch moves the landing pads
but gives `exc = r4` (not `r8`) and drops the score to 27.8 % (2796 B); the
catch-all type table also changes the LSDA. `try` around the whole body gives
25–44 % (2808–2816 B). None keep the 336 B `ttype 0xff` record.

## 4. Other sweeps (all ≤ 70.5415)

Guard position/condition (`{}` braces, `nullptr ==`, local `Status *st`,
delete-in-guard), `empTime` block before the switch / before `empPoints`,
hitpoints adjust before `empTime`, `int cls = shipClass`/`int t = type` copies,
`cls` dispatch, waypoint read order `x,y,z` vs `z,y,x`, coordinate declaration
order, `Vector pos` direct vs ctor, clamp `>` vs `>=` vs ternary. The
checked-in body is a local maximum.

## 5. Remaining lever

The exception pointer is `r6` because the original's cleanup landing pads are
emitted at the end of the IR block list (short PHI live range, `r8` free after
`z` dies), while clang 7.0.2 interleaves the pads with the branches for every
source shape tried (verified on minimal `new A(); if (...) new B();` examples:
the first pad is always emitted after the first branch body, before the else
branch). The all-int faithful body therefore cannot reach `r8` by source
spelling alone. A shape that makes clang defer the `new Player` cleanup pad to
the end of the function while keeping the 336 B `ttype 0xff` record is still
open.

## 6. Reproduction

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
python3 tools/verify/verify.py \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN5Level10createShipEiiiP8Waypointbb
```

Scratch helpers (not committed): `/dev/shm/w7669/{exact.py,exreg.py,tester.py,
tester2.py,shapes.py,v*.json,s*.json}`.
