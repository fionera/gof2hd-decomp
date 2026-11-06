# `Level::createShip` — wave 122 (w7208): real-`new` RA vs LSDA

Target `_ZN5Level10createShipEiiiP8Waypointbb` (`Level::createShip`,
original 2764 B @ `0xc0214`, Thumb entry `0xc0215`, 1007 insns).
Base `29cd275f6` (wave-121 final). Worktree left clean.

**Status: analysis only — no `src/` change landed.** The current body is
73.8 %, 2764 B, 1007/1008 insns, with a 328 B LSDA record (54 rows,
`ttype 0x00`, a try/catch type table). The original is real `new` with a
336 B cleanup-only record (57 rows, `ttype 0xff`).

## 1. Question

Can a real-`new` source shape reproduce **both** the original prologue
and the 336 B LSDA record, with the faithful (all-`int`) coordinates?

## 2. Answer

| goal | result |
|---|---|
| 336 B cleanup-only LSDA record | **YES** — exact record shape |
| original prologue / register allocation | **NO** — every real-`new` shape tried shifts the Greedy allocation |

Replacing the two placement-`new` ship allocations with real `new` and
dropping the `try/catch` anchor gives exactly the original record shape:

* 336 B total record (16 B personality+unwind prefix + 320 B LSDA);
* LSDA header `ff ff 01 bb 02`: `lpStartEncoding 0xff`,
  `ttypeEncoding 0xff`, `callSiteEncoding 0x01`, call-site table
  length `0x02bb` = 315;
* 57 call-site rows, every action `0` (cleanup only);
* no type table, no action table;
* the 12-byte unwind-opcode prefix (`c9 4d 97 02 f0 80 00 82 b0 b0 b0 ab`)
  is byte-identical; only the first word (personality `prel31`) differs
  because the record address differs.

The prologue does **not** match: the real-`new` body starts

```
strd r1, r2, [sp, #56]    ; race + shipClass spilled
mov  r9, r0               ; this
ldr  r0, [pc, #576]
mov  sl, r3               ; type in sl
add  r0, pc
ldr  r6, [r0, #0]         ; status in r6
```

whereas the original / current placement-`new` body starts

```
str  r1, [sp, #60]        ; race only
mov  r9, r0               ; this
ldr  r0, [pc, #572]
mov  fp, r3               ; type in fp
mov  r5, r2               ; shipClass in r5
add  r0, pc
ldr  r4, [r0, #0]         ; status in r4
```

## 3. Measurements (official one-function scorer)

| variant | % | size | insns | LSDA |
|---|---:|---:|---:|---|
| current base: placement new + try/catch + mixed coords | **73.8** | 2764 | 1007/1008 | 328 B, 54 rows, ttype 0x00 |
| **A: real new, no try/catch, mixed coords** | **64.3** | 2768 | 1007/996 | **336 B, 57 rows, ttype 0xff** |
| C: real new, no try/catch, faithful all-int coords | 62.7 | 2788 | 1007/1005 | **336 B, 57 rows, ttype 0xff** |
| D: real new + try/catch, mixed coords | 63.3 | 2772 | 1007/996 | type table |
| E: real new + try/catch, all-int coords | 61.2 | 2792 | 1007/1006 | type table |
| B: placement new, no try/catch, mixed coords | 70.2 | 2764 | 1007/996 | 316 B, 53 rows |
| M: real new + empty catch, mixed coords | 66.1 | 2780 | 1007/999 | type table |
| AE: real new + `empTime` ternary | 66.6 | 2764 | 1007/995 | 336 B |
| T1: real new + local `Status *status` | 64.1 | 2740 | 1007/997 | — |
| Z: real new + inline armor expression | 62.2 | 2756 | 1007/990 | — |
| AB: real new + `if` instead of level ternary | 63.7 | 2752 | 1007/990 | — |
| AL: real new + all-float coords | 58.4 | 2776 | 1007/1007 | — |
| AN1: placement new + all-int + try/catch | 60.7 | 2752 | 1007/1005 | — |
| AN2: placement new + all-int, no try/catch | 60.9 | 2748 | 1007/1004 | — |

The mixed-coordinate real-`new` body (A) is the best real-`new` score and
the smallest layout delta (+4 B). The faithful all-int body (C) is the
correct source semantics but costs another 1.6 points and +20 B.

## 4. LSDA evidence

Original record @ `0x1ab09c`, 336 B:

```
0000  c4 3a eb 7f  c9 4d 97 02  f0 80 00 82  b0 b0 b0 ab
0010  ff ff 01 bb 02  00 96 03 00 00 96 03 1e b8 13 00 ...
```

Variant A record @ `0x1ab0bc`, 336 B:

```
0000  a4 3a eb 7f  c9 4d 97 02  f0 80 00 82  b0 b0 b0 ab
0010  ff ff 01 bb 02  00 96 03 00 00 96 03 1e bc 13 00 ...
```

The first word is the `prel31` personality pointer (`__gxx_personality_v0@plt`);
it differs only because the record moved by `+0x20`. The 12-byte
unwind-opcode prefix and the LSDA header are identical. Parsing the
call-site table gives 57 rows in both, all action `0`; the original's
landing pads are `0x9b8` (r5), `0x9b2` (r9), `0x9ac` (fp), `0x9a6`
(sl), and the shared BoundingAAB pad `0x9b8`; variant A has only
`0x9bc` (r5), `0x9b4`/`0x9b6` (fp), `0x9ae` (r9), and the shared
BoundingAAB pad `0x9bc` — the `sl` (fighter) pad is absent because
variant A's fighter lives in `fp`.

Current base record @ `0x1ab09c`, 328 B:

```
0000  c4 3a eb 7f  c9 4d 97 02  f0 80 00 82  b0 b0 b0 ab
0010  ff 00 b4 02 01 ab 02  00 96 03 ...
```

`ttypeEncoding 0x00`, classInfoOffset `0x134`, call-site length
`0x12b` = 299, 54 rows, action `1` present. This is the try/catch
type table the real-`new` body removes.

## 5. RA evidence (why the prologue does not follow)

The real-`new` cleanup landing pads require the four heap pointers to
stay in callee-saved registers:

| object | original | variant A |
|---|---|---|
| `Player` / `BoundingAAB` | r5 | r5 |
| `PlayerFighter` | **sl** | fp |
| `Array<BoundingVolume*>` | **fp** | r9 |
| `PlayerFixedObject` | **r9** | fp |

This extra interference flips the first Greedy decision at the top of
the function:

| value | original | variant A |
|---|---|---|
| `status` | r4 | r6 |
| `rnd` vtable / `nextInt` | r6 | r4 |
| `type` | fp | sl |
| `shipClass` | r5, then sl | spilled to `[sp,#56]` |
| first jitter `jx` | `[sp,#36]` | fp |

The flip cascades through the coordinate registers (original `x=fp`,
`y=r4`, `z=r8`; variant A `x/y/z` different) and then through the
capital-ship branch (`fixed` fp vs r9, `volumes` r9 vs fp, `fighter`
fp vs sl). The original's source must pin the first decision; the
current reconstruction does not.

## 6. Variants tried (all real-`new` unless noted)

* declaration orders: all 6 waypoint orders × all 6 jitter orders, with
  mixed and all-int coordinates; no prologue match;
* all-int coordinates declared at top / after jitters / before `Player`
  ctor / after `Player` ctor; 60.7–63.1 %;
* `float x,y,z` all-float; 58.4 %;
* local `Status *status`, local `AERandom *rnd`, both, both in either
  order; 63.3–64.1 %;
* `const` locals for waypoint/jitter words; 64.3 %;
* `KIPlayer *ship` declared before/after `Player`, before/after the
  coordinates, `nullptr` vs `0`; 64.3 %;
* `PlayerFighter`/`PlayerFixedObject` declared at function scope;
  64.3 %;
* `ship = fighter` / `ship = fixed` immediately after allocation, and
  using `ship` for the virtual call; 64.3 %;
* `empTime` forms (`= 15000` before the `if`, ternary, ternary after
  the `if`, double assignment); 63.9–66.6 % (AE is the best but still
  no prologue match);
* `level` ternary vs `if`; 63.7 %;
* inline `hardCoreMode() ? 650 : 1000` instead of an `armor` local;
  62.2 %;
* placement new + all-int (with/without try/catch); 60.7–60.9 %;
* real new + empty catch / rethrow catch / delete-and-rethrow catch;
  46.5–66.1 %, all leave a type table.

## 7. Concrete patch proposal

Faithful real-`new` body (variant C; use variant A if the smallest
layout delta is preferred):

```diff
--- a/src/game/world/Level.cpp
+++ b/src/game/world/Level.cpp
@@
-    try {
-        player->setEmpData(empPoints, empTime);
-    } catch (...) {
-        __builtin_trap(); // lint: arm_builtin anchor: Level::createShip; removal grows body 2764 -> 2772
-    }
+    player->setEmpData(empPoints, empTime);
@@
-        PlayerFighter *fighter = (PlayerFighter *) ::operator new(sizeof(PlayerFighter)); // lint: operator_call
-        new (fighter) PlayerFighter(type, race, player, nullptr, x, y, z, hostile);
+        PlayerFighter *fighter = new PlayerFighter(type, race, player, nullptr, x, y, z, hostile);
@@
-        PlayerFixedObject *fixed = (PlayerFixedObject *) ::operator new(sizeof(PlayerFixedObject)); // lint: operator_call
-        new (fixed) PlayerFixedObject(type, race, player, nullptr, x, y, z);
+        PlayerFixedObject *fixed = new PlayerFixedObject(type, race, player, nullptr, x, y, z);
@@
-    float z = (float) (wz + jz - 20000);
-    float y = (float) (wy + jy - 20000);
-    int x = wx + jx - 20000;
+    int z = wz + jz - 20000;
+    int y = wy + jy - 20000;
+    int x = wx + jx - 20000;
```

The `int z/y/x` hunk is the faithful coordinate form (original computes
all three as `int` and converts at the ctor calls). If only the LSDA is
wanted, omit that hunk (variant A, 64.3 %, 2768 B).

## 8. Gate impact

Without a layout compensation the real-`new` body grows 2764 -> 2768 B
(variant A) or 2764 -> 2788 B (variant C). The `.text` section is
32-byte aligned, so either growth rounds `.text` from `0x1337c8` to
`0x1337e8` (+0x20). That shifts every later section and makes
address-dependent literal pools differ:

```
variant A `check`:  byte_exact 4506 -> 2373, linked_exact 4506,
                    avg 99.93, .text 0x1337e8, .ARM.extab 0x26298
```

`.ARM.extab` becomes exact at `0x26298` (the 336 B record is the right
size), but `.text` is +0x20. To keep the gate green, shave 4 B from
another `Level.cpp` function (variant A) or 24 B (variant C) so
`.text` stays `0x1337c8`; then `.ARM.extab` stays `0x26298` and
`byte_exact` should stay 4506. The wave-121 note used the same
layout-anchor technique for a 12 B body change.

## 9. Remaining root cause

The LSDA goal is solved by real `new`. The prologue/RA goal is not:
the real-`new` cleanup landing pads change the first Greedy decision
(`status` vs `rnd` vtable) and the whole allocation cascades. The
original's source shape that pins that first decision is still
unknown. A future pass should either find that shape or reproduce the
original's cleanup landing pads/register map by a source-level
restructuring; the current placement-`new` + `try/catch` body remains
the best score (73.8 %) but carries the 328 B / 54-row type-table
LSDA.
