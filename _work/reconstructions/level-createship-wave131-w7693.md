# `Level::createShip` — wave 131 (w7693): exact prologue via a `setEmpData` RA barrier

Target `_ZN5Level10createShipEiiiP8Waypointbb` (`Level::createShip`, original
2764 B @ `0xc0214`, Thumb entry `0xc0215`, 1007 insns).
Base `7c6933c2d` (wave-130 final). Worktree `/home/debian/GalaxyOnFire2/wt/w7693`.
Owned TU: `src/game/world/Level.cpp` (createShip only).

**Status: source change landed.**  `createShip` **70.5 -> 73.2** raw
(**73.187686**), still **2764 B**, now **1007/1007 insns**.  The first ~100
instructions (prologue through the `Player` ctor) are byte-identical to the
original.  The cost is `.ARM.extab` **156,304 B (8 B short)**: the landed
`try`/`catch` record is 328 B (54 rows, `ttype 0x00`) instead of the original's
336 B cleanup-only record.  Whole-`.text` differing bytes fall **85,379 ->
85,167**.

## Result

```
                                   original        HEAD (7c6933c2d)    landed
createShip raw fuzzy               —               70.541480           73.187686
createShip size                    2764 B          2764 B              2764 B
createShip insns (orig/ours)       1007            1007 / 1006         1007 / 1007
createShip LSDA record             336 B           336 B               328 B
createShip LSDA rows               57              57                  54
createShip LSDA ttype              0xff            0xff                0x00
.text differing bytes              —               85,379              85,167
.ARM.extab size                    156,312         156,312             156,304 (-8)
.ARM.extab differing bytes         —               1,435               108,023
byte_exact / linked_exact          —               4506 / 4506         4506 / 4506
data / imports / lint / drift      —               843 / 207 / CLEAN / 0
```

`check` on the landed tree: **RATCHET PASS** — `verify: byte 4506 linked
4506 stubs 0 missing 0 extra 0 avg 99.94`, `data: exact 843 of 843`,
`sodiff: imports 207`, lint `CLEAN`, drift 0.

## 1. The exact prologue (original vs landed)

```
original / landed (byte-identical)          previous HEAD (70.5)
c0220 str r1,[sp,#60]   ; race              str r2,[sp,#60]   ; shipClass
c0222 mov r9,r0         ; this              mov r9,r0
c0224 ldr r0,[pc,#572]                      str r1,[sp,#52]   ; race
c0226 mov fp,r3         ; type              mov sl,r3         ; type
c0228 mov r5,r2         ; shipClass         ldr r0,[pc,#560]
c022a add r0,pc                             add r0,pc
c022c ldr r4,[r0,#0]    ; status            ldr r4,[r0,#0]
...
c0238 mov sl,r5         ; shipClass copy    (none)
```

The original keeps **all seven** callee-saved registers live at entry
(`r4`=status, `r5`=shipClass, `r6`=nextInt fn, `r8`=mission, `r9`=this,
`sl`=shipClass copy, `fp`=type) and spills only `race`.  The previous body
spilled both `shipClass` and `race`, put `type` in `sl`, and left `fp` free.

## 2. What produces it — four coupled source changes

The exact prologue is not reachable by any single change.  It needs all four:

1. **Faithful two-call clamp** for both `level` and `empPoints`:
   `int level = Globals::status->getLevel() > 20 ? 20 : Globals::status->getLevel();`
   (the original calls `getLevel` twice: `c0286`/`c0296` and `c0316`/`c0324`).
2. **Placement `new` for `PlayerFighter` / `PlayerFixedObject`**:
   `::operator new(sizeof(T))` + `new (p) T(...)` (the `Player` itself stays a
   plain `new Player(...)`).
3. **Direct `x`/`y`/`z` arguments** to the `BoundingAAB`/`PlayerFixedObject`
   ctors (no `Vector pos` local).
4. **`try { player->setEmpData(empPoints, empTime); } catch (...) { __builtin_trap(); }`**.

The `try` is the decisive piece: it turns the `setEmpData` call into an
`invoke`, which is a scheduling barrier.  Without it clang 7.0.2's `-Oz`
machine scheduler sinks the whole coordinate block **below** `setEmpData`
(`t2SUBrr ... 20000` at MIR 4576B..4704B, after the `tBL setEmpData` at
4512B), and the prologue reverts to `str r2,[sp,#60]; mov sl,r3`.  With the
`invoke` the coordinate block stays above the call, exactly as in the
original (`c03c8..c03e4` before `c03ee`), and Greedy RA then assigns
`type -> fp`, `shipClass -> r5+sl`, `race -> [sp,#60]`.

Measured one-function variants (full link, official scorer):

| variant | raw % | size | insns | LSDA | note |
|---|---:|---:|---:|---|---|
| **landed** | **73.187686** | 2764 | 1007/1007 | 328 B, 54 rows, `0x00` | exact prologue |
| landed with `setShipGroup(..., type, group)` (unfaithful) | 73.6 | 2764 | 1007/1007 | 328 B | original passes `hostile` |
| landed, no `try` | 69.0 | 2776 | 1007/1009 | 336 B | coords sink |
| landed, no `try`, plain `new` Fighter/Fixed | 68.4 | 2780 | 1007/1012 | 336 B | coords sink |
| previous HEAD | 70.541480 | 2764 | 1007/1006 | 336 B | prologue wrong |
| landed, two `catch` clauses (int+float) | 65.0 | 2776 | 1007/1013 | 336 B | `.ARM.extab` exact again, but +12 B code |
| landed, `catch (int)` | 65.1 | 2772 | 1007/1011 | 332 B | |

## 3. Ground truth (original disassembly)

Coordinate spawn (`0xc03c8..0xc03e4`): all three coordinates are `int`,
computed **after** the `Player` ctor and **before** `setEmpData`, and each
pairs with its own jitter draw:

```
c03c8 ldr r2,[sp,#48]   ; waypoint->y
c03ca ldr r3,[sp,#32]   ; jitter 2nd
c03cc ldr r0,[sp,#44]   ; waypoint->x
c03ce ldr r1,[sp,#36]   ; jitter 1st
c03d0 add r2,r3         ; y = wy + 2nd
c03d2 ldr r3,[sp,#40]   ; waypoint->z
c03d4 ldr r4,[sp,#28]   ; jitter 3rd
c03d6 add r0,r1         ; x = wx + 1st
c03d8 movw r1,#20000
c03dc sub.w fp,r0,r1    ; x
c03e0 add r3,r4
c03e2 subs r4,r2,r1     ; y
c03e4 sub.w r8,r3,r1    ; z
```

`setShipGroup` third argument is `hostile`, not `group`:
`c06ba ldr r6,[r7,#12]` (hostile) -> `c06ca mov r8,r6` ->
`c0716 mov r3,r8`.  The landed body passes `hostile`.

The original's LSDA row 2 (`start=0x1b4 len=0x5e lp=0`) covers the coordinate
block **and** `setEmpData` with no landing pad, i.e. the original source has
no `try`; the landed `try` is an RA/scheduling device, not a faithful EH
shape.

## 4. Why the `try` cannot be replaced by a cheaper barrier

* `Array<int> dummy;` before `setEmpData`: adds a stack frame + ctor/dtor
  calls (2824 B, 23 %).
* `try { setEmpData } catch (...) { }`: 2772 B, 65.5 %.
* `try { setEmpData } catch (...) { throw; }`: 2740 B, 49.2 %.
* `catch (int) { __builtin_trap(); }`: 2772 B, 65.1 %, record 332 B.
* two catch clauses: 2776 B, 65.0 %, record 336 B (`.ARM.extab` exact again,
  but the function is 12 B too big).
* `-mllvm -pre-RA-sched=source` is a diagnostic only (not a build change).

So the 73.2 shape is the only one found that keeps the 2764 B size and the
exact prologue.  Its 8 B `.ARM.extab` cost is the open item: a no-`try` shape
that keeps the coordinate block above `setEmpData` would recover both.

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

Scratch helpers (not committed): `/dev/shm/w7693/{score.py,apply.py,
build_score.sh,pro.sh,insdiff.py,lsda.py,lsdacount.py,mir.sh,ir.sh}`.
