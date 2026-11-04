# Level::createShip — wave 107 (w5505): real `new`, `.ARM.extab` restored to 0x26298

**Status:** source change landed. Target
`_ZN5Level10createShipEiiiP8Waypointbb` (original 2764 B @ `0xc0214`,
Thumb entry `0xc0215`, 1007 insns). Fuzzy **73.8 -> 66.0** (tuned
placement-new shape -> faithful real-`new` shape, see §3). The structural
target is met: the createShip LSDA record is now **336 B / 57 rows /
ttype 0xff** (was 328 B / 54 rows / ttype 0x00), and `.ARM.extab` is
**0x26298** (was 0x26294; original 0x26298). A supporting `Hud::draw`
declaration-order change supplies the -4 that keeps the section total at
the original size (see §4).

Gate before and after (full `check` + `drift`):

```text
before: byte 4500/4524  linked 4500/4524  size 4524/4524  data 843/843
        imports 207  .text 0x69440/0x1337c8  .ARM.exidx 0x6f70
        .ARM.extab 0x26294  .rodata 0x1c9e10/0x41b67
        .rel.dyn 0x504d4/0x7048  .plt 0x5e7ec/0xac4c
        lint CLEAN  drift 0  RATCHET PASS
after:  byte 4500/4524  linked 4500/4524  size 4524/4524  data 843/843
        imports 207  .text 0x69440/0x1337c8  .ARM.exidx 0x6f70
        .ARM.extab 0x26298  .rodata 0x1c9e10/0x41b67
        .rel.dyn 0x504d4/0x7048  .plt 0x5e7ec/0xac4c
        lint CLEAN  drift 0  RATCHET PASS
```

## 1. The landed diff

`src/game/world/Level.cpp`:

```diff
-    switch (type) {
-        case 0x2c: hitpoints = (int) ((float) hitpoints * 2.25f); break;
-        case 0x33: hitpoints = (int) ((float) hitpoints * 1.7f);  break;
-        case 0x31: hitpoints = (int) ((float) hitpoints * 17.0f); break;
-    }
+    if (type == 0x33) {
+        hitpoints = (int) ((float) hitpoints * 1.7f);
+    } else if (type == 0x31) {
+        hitpoints = (int) ((float) hitpoints * 17.0f);
+    } else if (type == 0x2c) {
+        hitpoints = (int) ((float) hitpoints * 2.25f);
+    }
...
-    try {
-        player->setEmpData(empPoints, empTime);
-    } catch (...) {
-        __builtin_trap(); // lint: arm_builtin anchor
-    }
+    player->setEmpData(empPoints, empTime);
...
-        PlayerFighter *fighter = (PlayerFighter *) ::operator new(sizeof(PlayerFighter)); // lint: operator_call
-        new (fighter) PlayerFighter(type, race, player, nullptr, x, y, z, hostile);
+        PlayerFighter *fighter = new PlayerFighter(type, race, player, nullptr, x, y, z, hostile);
...
-        PlayerFixedObject *fixed = (PlayerFixedObject *) ::operator new(sizeof(PlayerFixedObject)); // lint: operator_call
-        new (fixed) PlayerFixedObject(type, race, player, nullptr, x, y, z);
+        PlayerFixedObject *fixed = new PlayerFixedObject(type, race, player, nullptr, x, y, z);
```

`src/game/ui/Hud.cpp` (dock block only, no semantic change):

```diff
-            String text = ...;
             float progress = ...;
-            barOffset = ...;
             int barW = ...;
+            String text = ...;
             int barH = ...;
+            barOffset = ...;
```

## 2. Why real `new` is the structural fix

The original's LSDA has **ttype 0xff (omit), no type table, no action
table** — i.e. cleanup-only landing pads. The only cleanup actions in the
original are `_ZdlPv` on the three allocation sites. The original calls
`_Znwj` at `0xc03a4` (Player), `0xc0408` (PlayerFixedObject) and
`0xc06bc` (PlayerFighter); the placement-new shape calls the same
`_Znwj`s but the ctor call is not inside a new-expression, so clang emits
no cleanup row for the PlayerFighter/PlayerFixedObject ctors.

Measured landing-pad call sites (`lp != 0` rows):

| shape | `_Znwj` calls | cleanup call sites | LSDA rows |
|---|---|---|---|
| placement new (wave 106) | 28 | 26 | 53 (no try) / 54 (catch-all) |
| original | 28 | 28 | 57 |
| real `new` both | 28 | 28 | 57 |

So the +8 B is exactly the two missing new-expression cleanup rows, with
`ttype` reverting from 0x00 to the original 0xff. The `switch` -> if-chain
change is what keeps the body at 2764 B (real `new` + `switch` is 2768).

## 3. Measured variant table

All rows are full-link measurements of `_ZN5Level10createShip...`
(`score1.py` / `extab_len.py`, `verify.py` resolver).

| variant | fuzzy | body | record | rows | ttype | extab | gate |
|---|---|---|---|---|---|---|---|
| wave-106 HEAD (placement new, switch, catch-all) | 73.8 | 2764 | 328 | 54 | 0x00 | 0x26294 | PASS |
| placement new, switch, no try | 70.2 | 2776 | 316 | 53 | 0xff | 0x26288 | n/a |
| placement new, if-chain, catch-all | 69.6 | 2760 | 328 | 54 | 0x00 | 0x26294 | n/a |
| real new both, switch, no try | 64.3 | 2768 | 336 | 57 | 0xff | 0x2629c | n/a |
| **real new both, if-chain, no try (landed)** | **66.0** | **2764** | **336** | **57** | **0xff** | **0x2629c** | **PASS** |
| real new both, if-chain, catch-all | 62.5 | 2768 | 352 | 58 | 0x00 | 0x262ac | n/a |
| `catch (std::bad_alloc&)` + `catch (...)` | 73.8 | 2764 | 336 | 54 | 0x00 | 0x2629c | **FAIL** |
| `catch (LocalShipError&)` + `catch (...)` | 73.8 | 2764 | 336 | 54 | 0x00 | 0x2629c | **FAIL** |

The two `catch` variants hit 336 B at the tuned 73.8 % but break
`byte_exact` (see §5). The landed real-`new` shape is the only variant
that is 336 B, 2764 B body, and gate-green.

## 4. The Hud -4 (why the section total is exact)

Per-record deltas after the createShip change alone (Hud at its wave-106
`ABCDEF` order):

```text
LevelScript::process  +4 (352 vs 348)
Hud::draw             +4 (480 vs 476)
createShip             0 (336 vs 336)
Generator::createMission -4 (168 vs 172)
total                 +4  ->  .ARM.extab 0x2629c, .rodata 0x1c9e20 (+0x10)
```

`0x2629c` pushes `.rodata` to the next 16-byte boundary, which changes
every PC-relative literal-pool load and drops `byte_exact`. Reordering
the Hud dock block to `B D A E C F` (progress, barW, text, barH,
barOffset, fade) gives the original's 476 B record and brings the total
back to `0x26298`; `.rodata` stays at `0x1c9e10` and the gate stays green.

All 22 permutations that give Hud record 476 / body 9332 were measured
full-link; the best is 93.3 % (`BDAECF`, `BDAEFC`, `CDFABE`, `DCFABE`,
`DFCABE`). The wave-106 `ABCDEF` order scores 94.4 % but its record is
480 (+4), which is exactly what we needed to cancel. So Hud trades
94.4 -> 93.3 to buy the createShip extab fix.

## 5. Dead ends (measured, do not repeat)

* **Typed catch.** `catch (std::bad_alloc&) + catch(...)` is 336 B at
  73.8 %, but the LSDA type table needs an `R_ARM_TARGET2` reference to
  `_ZTISt9bad_alloc`. Gold moves that symbol's GOT slot from `0x210678`
  (libc++abi region) to `0x210368` (Level.o's region), shifting every
  GOT slot in `[0x210368, 0x210678)` by +4. 267 functions load those
  slots from literal pools -> `byte_exact 4500 -> 4233`. Same for
  `catch(int)`, `catch(std::exception&)`, `catch(std::bad_exception&)`.
* **Local-class catch.** `namespace { struct LocalShipError{}; }` +
  `catch (LocalShipError&)` is also 336 B / 73.8 %, but the local
  typeinfo adds 3 `.rel.dyn` entries (vtable, name, LSDA) and a 16-byte
  `.data.rel.ro` object: `.rel.dyn 0x7048 -> 0x7060`, `.plt`/`.text`
  shift +0x20, `byte_exact 4500 -> 1742`.
* **Extra try/catch scopes.** Wrapping the placement ctors or
  `lodManager->addObject` in `try { } catch (...) { __builtin_trap(); }`
  gives 340 B / 56 rows and grows the body (2768-2788); `throw;` gives
  348 B / 57 rows and 49.4 %.
* **Faithful-shape fuzzy.** The real-`new` shape's register allocation
  differs from the original from the second instruction (`strd r1,r2,
  [sp,#56]` vs `str r1,[sp,#60]`; ours spills `shipClass`, the original
  keeps it in r6 and spills `this`/`type`). The wave-106 placement-new
  shape happens to match the original's allocation exactly. Attempts to
  recover it (local `shipClass` copy, `armor` if/else, `0` vs `nullptr`,
  explicit casts, all six hitpoint if-chain orders) all stay at 66.0 %.
  The original's 1007 instructions vs our 995 are the 12 extra landing
  pad/EH instructions.

## 6. Files

* `src/game/world/Level.cpp` — createShip shape (real `new`, no try,
  if-chain).
* `src/game/ui/Hud.cpp` — dock declaration order `B D A E C F`.
* This note.

## 7. Doc follow-up

`docs/VALIDATION.md` (lines ~99-102) and `docs/IDENTICALITY.md`
(lines ~34, ~39, ~118-119, ~261-262, ~290, ~331) still state
`.ARM.extab` is `0x26294` / 4 B short and list the createShip -8 and
Hud +4 deltas. Those numbers are now stale: `.ARM.extab` is `0x26298`,
createShip is 336 (exact), Hud is 476 (exact), and the only remaining
record deltas are `LevelScript::process +4` / `Generator::createMission
-4` (total 0). Left uncommitted per the wave-107 worker instruction not
to touch docs.
