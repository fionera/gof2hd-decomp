# `Level::createShip` — wave 125 (w7510): two-call clamp diamonds + coordinate-block order (no source win)

Target `_ZN5Level10createShipEiiiP8Waypointbb` (`Level::createShip`, original
2764 B @ `0xc0214`, Thumb entry `0xc0215`, 1007 insns).
Base `02513de33` (wave-124 final). Worktree `/home/debian/GalaxyOnFire2/wt/w7510`.
Owned TU: `src/game/world/Level.cpp` (createShip only).

**Status: analysis only — no `src/` change landed.** The original's
`level`/`empPoints` clamps are two-call branch diamonds and its coordinate
block is computed before `setEmpData`; both were reproduced in source, but
neither improves the real-`new` body. The best real-`new` shape remains the
wave-124 body at **70.5 %**, 2764 B, 336 B cleanup-only LSDA.

## Result

```
                                   original        HEAD (02513de33)   best diamond
createShip fuzzy (official)        —               70.5000            68.6000
createShip size                    2764 B          2764 B             2780 B
createShip insns (orig/ours)       1007            1007 / 1006        1007 / 1012
createShip LSDA record             336 B           336 B              336 B
createShip LSDA rows               57              57                 57
createShip LSDA ttype              0xff            0xff               0xff
.text differing bytes              0               85,975             86,0xx
.ARM.exidx differing bytes         0               0                  0
.ARM.extab size/base               0x26298         0x26298            0x26298
.ARM.extab differing bytes         0               1,474              1,4xx
.rel.plt differing bytes           0               0                  0
.note.gnu.build-id differing       0               20                 20
byte_exact / linked_exact          —               4506 / 4506        4506 / 4506
avg_match                          —               99.93              99.93
drift / lint                       —               0 / CLEAN          0 / CLEAN
imports                            —               207                207
```

`check` on the unchanged HEAD: **RATCHET PASS** — `verify: byte 4506 linked
4506 stubs 0 missing 0 extra 0 avg 99.93`, `data: exact 843 of 843`,
`sodiff: imports 207`, lint `CLEAN`, drift 0.

## 1. Ground truth: the two structural deltas

### 1a. `level` and `empPoints` are two-call diamonds

Original (`0xc0284`..`0xc029a`, `0xc0314`..`0xc0328`):

```
c0284  ldr   r0,[r4,#0]        ; Globals::status
c0286  blx   getLevel
c028a  cmp   r0,#20
c028c  str   r5,[sp,#28]       ; jitterZ (scheduled)
c028e  ble   c0294
c0290  movs  r5,#20
c0292  b     c029c
c0294  ldr   r0,[r4,#0]
c0296  blx   getLevel
c029a  mov   r5,r0
```

i.e. `MIN(getLevel(), 20)` with `#define MIN(a,b) ((a) > (b) ? (b) : (a))`
(same form as `STAR_MIN` in `StarSystem.cpp`), not the one-call `if` form
currently in the source. The same diamond is repeated for `empPoints`.

### 1b. The coordinate block is before `setEmpData`

Original (`0xc03c8`..`0xc03ee`):

```
c03c8  ldr  r2,[sp,#48]   ; wy
c03ca  ldr  r3,[sp,#32]   ; jitterY
c03cc  ldr  r0,[sp,#44]   ; wx
c03ce  ldr  r1,[sp,#36]   ; jitterX
c03d0  add  r2,r3
c03d2  ldr  r3,[sp,#40]   ; wz
c03d4  ldr  r4,[sp,#28]   ; jitterZ
c03d6  add  r0,r1
c03d8  movw r1,#20000
c03dc  sub.w fp,r0,r1    ; x
c03e0  add  r3,r4
c03e2  subs r4,r2,r1     ; y
c03e4  sub.w r8,r3,r1    ; z
c03e8  mov  r0,r5
c03ea  mov  r1,sl        ; empPoints
c03ec  mov  r2,r9        ; empTime
c03ee  blx  setEmpData
```

`x`, `y`, `z` are all `int`; the float conversions happen later, once per
consumer (`PlayerFighter` ctor at `0xc06c0`..`0xc06d8`, `PlayerFixedObject`
ctor at `0xc040c`..`0xc0422`). HEAD computes the block after `setEmpData`
(`0xc03c8`..`0xc03e8` in our build) and uses `float z`, `float y`, `int x`.

## 2. What was tried (official one-function scorer, full link)

All rows keep the faithful wave-124 call-site args (`PlayerFighter(type,
race, ...)`, `getShipGroup(type, race, ...)`, `PlayerFixedObject(type, race,
...)`) and real `new` for both ships.

| variant | % | size | insns | LSDA |
|---|---:|---:|---:|---:|
| **HEAD: one-call clamps, status guard, mixed coords** | **70.5** | 2764 | 1007/1006 | 336 |
| two-call ternary/if-else/assignment clamps, guard, mixed coords | 68.6 | 2780 | 1007/1012 | 336 |
| same, no status guard | 67.0 | 2764 | 1007/994 | 336 |
| two-call clamps, guard, all-`int` coords | 62.2 | 2788 | 1007/1004 | 336 |
| two-call clamps, no guard, all-`int` coords | 60.8 | 2792 | 1007/1009 | 336 |
| one-call clamps, guard, all-`int` coords | 61.5 | 2772 | 1007/998 | 336 |
| one-call clamps, no guard | 63.6 | 2748 | 1007/988 | 336 |
| `const int` clamps (two-call) | 68.7 | 2772 | 1007/1009 | 336 |
| `Vector pos` / `struct Coord` / `int[3]` coords before `setEmpData` | 57.5 / 61.5 / 61.5 | 2776 / 2772 / 2772 | — | 336 |
| guard moved between coords and `setEmpData` | 63.1 / 64.4 | 2776 / 2788 | — | 336 |
| `KIPlayer *ship` before `setEmpData` | 70.5 | 2764 | 1007/1006 | 336 |
| switch case order `0x33,0x31,0x2c` | 70.5 | 2764 | 1007/1006 | 336 |
| all 6 waypoint orders × 6 jitter orders, all-`int` + diamonds | 60.5–62.3 | 2788–2792 | — | 336 |

The two-call diamond **is** reproduced structurally: with the diamond + guard
body the clamp regions align instruction-for-instruction with the original
(`EQ` blocks of 10, 8 and 9 instructions in the normalized stream). The score
still falls to 68.6 because the two extra `getLevel` calls and their
downstream register-allocation cascade cost more matching instructions than
the clamp region gains. The prologue is unchanged by the diamond
(`str r2,[sp,#60]` = shipClass, `str r1,[sp,#52]` = race, `mov sl,r3` = type).

The coordinate block order is a **scheduler/RA consequence**, not a source
order: with all-`int` coords (the faithful form) the body grows to 2788–2792 B
and the block still sinks after `setEmpData`; forcing it with `Vector pos`,
a local `struct Coord`, an `int[3]`, `volatile`, or a dummy use before
`setEmpData` either sinks anyway or adds code and loses 3–13 points.

## 3. The only shape that matches the original RA is placement-new

The wave-119 placement-new + `try` body (real `Player`, placement `new` for
both ships, `try { setEmpData } catch (...) { __builtin_trap(); }`, two-call
diamonds, no status guard) with the faithful wave-124 call-site args and
faithful spawn scores **72.4 %**, stays **2764 B**, and has **1007/1007**
insns — the only shape found that reproduces the original prologue and the
whole callee-saved permutation. But it is not EH-identical:

```
LSDA record        328 B / 54 rows / ttype 0x00 (try/catch type table)
.ARM.extab         0x26290 (8 B short)
.ARM.exidx         1,327 differing bytes (landing-pad offsets)
.bss               1 differing byte
```

so it violates the real-`new` / 336 B cleanup-only / `.ARM.exidx`
byte-identity constraints. Real `new` + `try` is 352 B; real `new` + an RAII
guard is 344 B; a nested `NoopGuard` around `setEmpData` adds exactly +8 B but
drops the score to 67.1.

## 4. Remaining root cause

The real-`new` cleanup pointers (`Player`/`BoundingAAB` in r5,
`PlayerFixedObject` in r9, `Array<BoundingVolume*>` in fp, `PlayerFighter` in
sl) plus the four incoming arguments exceed the seven usable callee-saved
registers. Greedy RA spills `shipClass` and `race` and puts `type` in `sl`;
the original spills only `race`, keeps `type` in `fp` and `shipClass` in
`r5 -> sl`, then re-seats the whole allocation (`fixed` r9, `volumes` fp,
`fighter` sl). The diamond clamps and the coordinate-block order are
downstream symptoms of that one decision. No source form tried in waves
119–125 moves the first Greedy decision while keeping the real-`new` 336 B
cleanup-only LSDA and 2764 B.

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
python3 _work/reconstructions/exidx_table.py --diff \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so
```

Scratch helpers (not committed): `/dev/shm/w7510_*` (one-function scorer,
variant generator, section-byte diff, LSDA diff).
