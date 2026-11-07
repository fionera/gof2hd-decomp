# `Level::createShip` — wave 131 (w7709): no-`try` EH-scope sweep for the 336 B record

Target `_ZN5Level10createShipEiiiP8Waypointbb` (`Level::createShip`, original
2764 B @ `0xc0214`, Thumb entry `0xc0215`, 1007 insns).
Base `70e451ffe` (wave-131 master). Worktree `/home/debian/GalaxyOnFire2/wt/w7709`.
Owned TU: `src/game/world/Level.cpp` (createShip only).

**Status: analysis only — no `src/` change landed.**  Master stays at
**70.5765 %** (2764 B, 1007/1005 insns) with the **exact** 336 B / 57-row /
`ttype 0xff` cleanup-only LSDA (`.ARM.extab` 156,312 B).  The w7693 `try`
body is **73.1877 %** (2764 B, 1007/1007 insns) but its record is 328 B /
54 rows / `ttype 0x00` (`.ARM.extab` 156,304 B, 8 B short).  This pass
attacked the w7693 open question — a no-`try` shape that keeps the w7693
allocation *and* the exact record — and found that the two requirements are
coupled through the EH scope: **any EH scope that flips the register
allocation also changes the LSDA.**

## Result

```
                                   original        master (70e451ffe)   w7693 try body
createShip fuzzy                   —               70.5765              73.1877
createShip size                    2764 B          2764 B               2764 B
createShip insns (orig/ours)       1007            1007 / 1005          1007 / 1007
createShip LSDA record             336 B           336 B                328 B
createShip LSDA rows               57              57                   54
createShip LSDA ttype              0xff            0xff                 0x00
.ARM.extab size                    156,312         156,312              156,304 (-8)
byte_exact / linked_exact          —               4506 / 4506          4506 / 4506
data / imports / lint / drift      —               843 / 207 / CLEAN / 0
```

`check` on the unchanged master tree: **RATCHET PASS** — `verify: byte 4506
linked 4506 stubs 0 missing 0 extra 0 avg 99.94`, `data: exact 843 of 843`,
`sodiff: imports 207`, lint `CLEAN`, drift 0.

## 1. The w7693 `try` is a register-allocation lever, not a scheduling barrier

The w7693 note described the `try` as keeping the coordinate block above
`setEmpData`.  The disassembly says otherwise: the landed w7693 body computes
only `y`/`z` (as floats) before `setEmpData`; `x` is still computed after the
call, exactly as in master.  The `try`'s real effect is on **Greedy RA**:

* master / no-`try`: `type -> sl (r10)`, `shipClass -> r5 then fp (r11)`,
  `race -> [sp,#60]`, `shipClass` spilled;
* w7693 `try`: `type -> fp (r11)`, `shipClass -> r5 then sl (r10)`,
  `race -> [sp,#60]`, exact original prologue.

The pre-RA MIR of the two bodies is byte-identical in `bb.0` (same virtual
registers `%127=type`, `%126=shipClass`, `%125=race`, `%124=this`, same COPY
order).  The only difference is the extra landing-pad block created by the
invoke, which changes the live-interval order Greedy RA sees and therefore
which of `r10`/`r11` receives `type`.

## 2. New experiments: EH scopes flip the allocation, but also the LSDA

The decisive new result is that **any** EH scope covering `setEmpData` (or a
`new` whose cleanup is live there) reproduces the w7693 allocation, but the
same scope changes the call-site table:

| variant (source shape) | fuzzy % | usize | LSDA len/rows/ttype | `.ARM.extab` |
|---|---:|---:|---|---:|
| **master** (single-call clamps, plain `new`, guard) | **70.5765** | 2764 | **336 / 57 / 0xff** | **156,312** |
| w7693 body (`try` + placement `new` + direct coords) | 73.1877 | 2764 | 328 / 54 / 0x00 | 156,304 |
| w7693 body + wave-105 spawn hack | 73.7 | 2764 | 328 / 54 / 0x00 | 156,304 |
| w7693 body, `try` removed (placement `new`) | 69.7 | 2780 | 316 / 53 / 0xff | 156,292 |
| w7693 body, plain `new`, no `try` | 66.8 | 2764 | **336 / 57 / 0xff** | **156,312** |
| w7693 body, plain `new`, no `try`, + hack | 67.1 | 2764 | **336 / 57 / 0xff** | **156,312** |
| w7693 body, plain `new`, no `try`, + guard | 68.2 | 2780 | **336 / 57 / 0xff** | **156,312** |
| w7693 body + scoped `Dummy{~Dummy(){}}` around `setEmpData` | 65.2 | 2760 | 54 rows / 0xff | 156,296 |
| w7693 body + scoped `Dummy` + plain `new` | 63.2 | 2768 | 344 / 58 / 0xff | 156,320 |
| master + scoped `Dummy` around `setEmpData` | 67.1 | 2752 | 344 / 58 / 0xff | 156,320 |
| master + `try { setEmpData } catch (...) { trap; }` | 67.1 | 2752 | 352 / 58 / 0x00 | 156,328 |
| master + direct coords + all-int coords | 61.5 | 2772 | **336 / 57 / 0xff** | **156,312** |
| master + double-call clamps + direct + all-int + no guard (faithful) | 62.9 | 2792 | **336 / 57 / 0xff** | **156,312** |

Key observations:

1. **The exact record requires plain `new`.**  Placement `new` removes the
   `PlayerFighter` / `PlayerFixedObject` cleanup landing pads.  The w7693
   body's `try` body has 54 rows; removing the `try` (placement `new`) drops
   to 53; switching to plain `new` restores 57.
2. **The exact record requires no EH scope covering `setEmpData`.**  A scoped
   `Dummy` with an empty destructor gives `ttype 0xff` (cleanup-only, like the
   original) but adds one call-site row (58) and 8 B.  A `try`/`catch` adds a
   type table (`ttype 0x00`) and splits the gap.
3. **The allocation fix is lost with plain `new` + no EH scope.**  Plain `new`
   + no `try` is 66.8 % (w7693 body) / 70.6 % (master); the exact prologue is
   not reproduced.  The w7693 `try` body is the only shape found that has both
   the exact prologue and 2764 B.

## 3. The wave-105 spawn hack (new measurement on this base)

The wave-105 note's "pass-10 hack" expressions

```cpp
int x = jitterZ + jitterX - 20000;   // semantically wrong (jitters only)
float y = (float) (wy + jitterY - 20000);
float z = (float) (wz + wx - 20000); // semantically wrong (waypoint x)
```

reproduce the original's **exact stack-slot map** (`wp->x@44, wp->y@48,
wp->z@40, j1@36, j2@32, j3@28`) and raise the w7693 `try` body
73.1877 -> **73.7 %** (2764 B, 1006 insns).  On master (plain `new`, guard)
the hack leaves the score unchanged at 70.6 % and the exact 336/57 record.
It is semantically wrong, so it is not a candidate for landing on its own.

## 4. Why the two requirements are coupled

`setEmpData` is a plain call in the original: LSDA row 2 is
`start=0x1b4 len=0x5e lp=0` — a gap with no landing pad.  Every source
construct that makes `setEmpData` an `invoke` (a `try`/`catch`, or a live
local with a non-trivial destructor) necessarily gives it a landing pad and
therefore a row with `lp != 0`.  The `try` also adds a catch type
(`ttype 0x00`).  Conversely, every no-EH-scope body leaves Greedy RA free to
choose `type -> sl`, which is the wrong prologue.

The remaining lever is therefore a **non-EH** way to change Greedy RA's
live-interval ordering so that `type -> fp` and `shipClass -> r5/sl`, while
keeping `setEmpData` a plain call and the `new` expressions plain.  Candidates
not yet tried: a source construct that adds a pure control-flow block at the
`setEmpData` boundary (e.g. a `switch`/`goto` that survives `-Oz`), or a
change to the waypoint/jitter live ranges that reorders the intervals without
adding an EH scope.

## 5. Reproduction

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

Scratch helpers (not committed): `/dev/shm/w7709/{score.py,runvar.sh,runw.sh,
mvar.py,wvar.py,sweep_hack.py,lsda.py,mir2.sh,show_*.txt,mir_*.txt}`.
