# `Level::createShip` — wave 121 (w7107): faithful spawn coordinates

Target `_ZN5Level10createShipEiiiP8Waypointbb` (`Level::createShip`,
original 2764 B @ `0xc0214`, Thumb entry `0xc0215`, 1007 insns).
Base master `bd061a489`. Worktree left clean.

**Status: landed.** `createShip` 74.4 → **73.8 %** (fuzzy, official
scorer), body stays 2764 B, insns 1007 / 1008. The score dips 0.6 pts
because the previous body used a *tuned* spawn slot-map that was not
the original's semantics; this pass replaces it with the real
semantics and documents the register-allocation residual.

```
                                    before (HEAD)        after
createShip fuzzy (official)         74.4                 73.8
createShip size                     2764 B               2764 B
createShip insns (orig/ours)        1007 / 1007          1007 / 1008
byte_exact / linked_exact           4505 / 4505          4505 / 4505
drift / lint                        0 / CLEAN            0 / CLEAN
imports                             207                  207
```

`check`: **RATCHET PASS** — `verify: byte 4505 linked 4505 stubs 0
missing 0 extra 0 avg 99.94`, `data: exact 843 of 843`, `sodiff:
imports 207`, lint CLEAN, `drift` 0 fields.

## 1. The bug in the landed (wave-119) body

The original coordinate block is unambiguous (Thumb disasm):

```
c023c  ldr.w r1,[r0,#292] ; str r1,[sp,#44]   ; sp44 = waypoint->x
c0242  ldr.w r1,[r0,#296] ; str r1,[sp,#48]   ; sp48 = waypoint->y
c0246  ldr.w r0,[r0,#300] ; str r0,[sp,#40]   ; sp40 = waypoint->z
c026e  str r0,[sp,#36]                        ; sp36 = 1st nextInt
c0278  str r0,[sp,#32]                        ; sp32 = 2nd nextInt
c028c  str r5,[sp,#28]                        ; sp28 = 3rd nextInt
...
c03c8  ldr r2,[sp,#48] ; ldr r3,[sp,#32]      ; wy, jy
c03cc  ldr r0,[sp,#44] ; ldr r1,[sp,#36]      ; wx, jx
c03d2  ldr r3,[sp,#40] ; ldr r4,[sp,#28]      ; wz, jz
c03dc  sub.w fp, r0, r1                       ; x = wx + jx - 20000
c03e2  subs  r4, r2, r1                       ; y = wy + jy - 20000
c03e4  sub.w r8, r3, r1                       ; z = wz + jz - 20000
```

i.e. **x = waypoint->x + 1st, y = waypoint->y + 2nd,
z = waypoint->z + 3rd**, all `int`, computed between the `Player`
ctor and `setEmpData`.  The wave-119 body instead used

```
wz = waypoint->x; wy = waypoint->y; wx = waypoint->z;
jitterY = 1st; jitterZ = 2nd; jitterX = 3rd;
x = jitterZ + jitterX - 20000;          // 2nd + 3rd, no waypoint
y = (float)(wy + jitterY - 20000);      // wy + 1st
z = (float)(wz + wx - 20000);           // wx + wz
```

which is a *different spawn position* (wrong axis mapping and wrong
jitter pairing).  It scored higher only because the fuzzy LCS happens
to line up better.

## 2. What changed

`src/game/world/Level.cpp`, `createShip()` body only:

```diff
-    int wx, wz, wy;
-    if (waypoint != nullptr) {
-        wz = waypoint->x;
-        wy = waypoint->y;
-        wx = waypoint->z;
-    } else {
-        wz = 0; wy = 0; wx = 0;
-    }
-    int jitterY = Globals::rnd->nextInt(40000);
-    int jitterZ = Globals::rnd->nextInt(40000);
-    int jitterX = Globals::rnd->nextInt(40000);
+    int wx = waypoint != nullptr ? waypoint->x : 0;
+    int wy = waypoint != nullptr ? waypoint->y : 0;
+    int wz = waypoint != nullptr ? waypoint->z : 0;
+    int jx = Globals::rnd->nextInt(40000);
+    int jy = Globals::rnd->nextInt(40000);
+    int jz = Globals::rnd->nextInt(40000);
...
-    int x = jitterZ + jitterX - 20000;
-    float y = (float) (wy + jitterY - 20000);
-    float z = (float) (wz + wx - 20000);
+    float z = (float) (wz + jz - 20000);
+    float y = (float) (wy + jy - 20000);
+    int x = wx + jx - 20000;
```

The ternary waypoint form is what keeps the original's **first 98
instructions** byte-for-byte (prologue `str r1,[sp,#60]; mov r9,r0;
mov fp,r3; mov r5,r2; ... mov sl,r5`, hitpoint chain, empPoints clamp,
`Player` ctor).  The `if/else` form produces the same prologue but a
different stack-slot assignment for the waypoint words (see §4).

Verified in the emitted code: `z = wp->z + jz`, `y = wp->y + jy`,
`x = wp->x + jx`, with the capital/fighter ctors receiving
`(x, y, z) = (s20, s18, s16)` in that order.

## 3. Variants measured (official one-function scorer)

| variant | % | size | insns |
|---|---:|---:|---:|
| wave-119 tuned body (HEAD) | **74.4** | 2764 | 1007/1007 |
| **this pass: ternary wp + faithful mixed (x int, y/z float), order z,y,x** | **73.8** | 2764 | 1007/1008 |
| same, order x,y,z | 73.6 | 2764 | 1007/1008 |
| same, order y,x,z / y,z,x | 73.6 | 2764 | 1007/1008 |
| `if/else` wp + faithful mixed | 72.5–72.6 | 2764 | 1007/1007 |
| ternary wp + all-`int` x,y,z (fully faithful) | 61.3–61.6 | 2752–2756 | 1007/1004 |
| `if/else` wp + all-`int` x,y,z | 60.7–61.7 | 2748–2800 | 1007/1003 |
| all-`int`, coords before `Player` ctor | 61.2–61.4 | 2800 | 1007/1006 |
| all-`int`, coords at top (after jitters) | 61.0–61.9 | 2752–2796 | 1007/1005 |
| real `new` both ships + all-`int` | 63.7–64.2 | 2772–2796 | 1007/995 |
| real `new` fixed only, all-`int` | 65.2 | 2764 | 1007/1006 |
| `float fx = (float)x` before/after `setEmpData` | 73.6 | 2764 | 1007/1008 |
| `Level *self = this` + `self->missionPtr` | 61.6 | 2756 | 1007/1006 |
| `setShipGroup(..., hostile)` | 73.2 | 2764 | 1007/1007 |
| `int empTime = 15000` before the `if` | 73.6 | 2764 | 1007/1008 |

The all-`int` form is the *most* faithful (matches the original's
`int x,y,z` live in `fp/r4/r8` across `setEmpData`, and duplicates the
`vcvt.f32.s32` in both branches like the original) but scores 12 pts
lower because it changes the global Greedy register allocation.

## 4. Remaining root cause (why faithful is not yet byte-exact)

The residual is the same one the wave-119 note called out, now with
the coordinate semantics fixed:

1. **Stack-slot colouring of the waypoint/jitter words.**  The
   original assigns `sp44/48/40` (waypoints) and `sp36/32/28`
   (jitters).  The faithful ternary body assigns `sp32/48/44` and
   `sp28/40/36` — the first two words (`wp->x`, `wp->y`) match, the
   rest are permuted.  The slot colouring is driven by the reload
   order in the coordinate block, which the scheduler reorders
   regardless of source order (all six declaration orders give the
   same slots).
2. **Capital-ship register permutation.**  Original `r9 = fixed`,
   `fp = volumes`; ours `fp = fixed`, `r9 = volumes`.  This follows
   from `x`: in the original `x` is an `int` in `fp` until the
   `PlayerFixedObject` ctor (`vmov s0, fp; vcvt.f32.s32 s20`), so
   `fp` is occupied when `ship = nullptr` is materialised and
   `fixed` gets `r9`.  In the mixed body `x` is converted to `s18`
   before `setEmpData`, freeing `fp`; `ship` then takes `fp` and
   `fixed` follows it.  Every `ldr r0,[fp,#4]` vs `[r9,#4]` in the
   `BoundingAAB` zoo (≈20 sites) is this single swap.
3. **`BoundingAAB` argument setup.**  Original `vmov sl,s20` (x),
   `vmov r8,s18` (y), `vmov r4,s16` (z), `movs r6,#0`; ours
   `vmov r6,s20`, `vmov r8,s18`, `vmov r3,s16; mov sl,r3`,
   `movs r0,#0`.  Also a consequence of the capital-branch register
   map (`r4` holds `race` in ours, `type` in the original).
4. **`str.w r9,[sp,#56]` scheduling.**  Original spills `this` after
   `add.w sl,r0,#40`; ours spills it one instruction earlier, before
   `cmp r6,#1`.  A one-instruction shift that costs the `ble`/`b`
   offsets in the `getLevel` clamp.
5. **EH shape.**  The `try/catch` anchor (needed for the 2764 B body)
   gives LSDA 328 B / 54 rows / `ttype 0x00`; the original is
   real-`new` cleanup-only, 336 B / 57 rows / `ttype 0xff`.  The
   wave-119 note showed real `new` moves the prologue and costs more
   score than it recovers.

Fixing (2)/(3) requires getting `x` to stay an `int` in `fp` through
`setEmpData`, which is exactly the all-`int` form that scores 61.6 %
because the extra integer live ranges change the *first* Greedy
decision (`this` is spilled instead of kept in `r9`).  That first
decision is the wall; it is the same wall documented in wave 119 for
the real-`new` prologue.  A future pass that reproduces the original's
cleanup landing pads (or otherwise pins `this` to `r9`) should retry
the all-`int` coordinate block on top of this faithful body.
