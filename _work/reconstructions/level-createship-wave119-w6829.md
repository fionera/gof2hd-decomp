# Level::createShip — wave 119 (w6829): placement-new EH shape lands 70.2 -> 72.1 %

**Status:** analysis landed, source change reverted (2026-11-05). Target
`_ZN5Level10createShipEiiiP8Waypointbb` (`Level::createShip`, original
2764 B @ `0xc0214`, Thumb entry `0xc0215`, 1007 insns). Branch
`wave119-w6829`, base = master `4f36e6260`.

## Result

```
                                   before (HEAD)        after (landed)
createShip fuzzy (raw)             70.200000            72.100000
createShip size                    2764 B               2764 B
createShip insns (orig/ours)       1007 / 1006          1007 / 1007
createShip LSDA record             336 B (exact)        328 B (-8)
.text                              0x1337c8 (exact)     0x1337c8 (exact)
.ARM.exidx                         0x19cc08 / 0x6f70    0x19cc08 / 0x6f70
.ARM.extab                         0x26294              0x2628c  (-8)
.rodata                            0x41c27              0x41c27
file size                          2,187,328 (exact)    2,187,328 (exact)
byte_exact / linked_exact          4505 / 4505          4505 / 4505
```

`check` (final): **RATCHET PASS** — `verify: byte 4505 linked 4505 stubs 0
missing 0 extra 0 avg 99.94`, `data: exact 843 of 843`, `sodiff: imports
207`, lint CLEAN, drift 0.  `.text`, `.ARM.exidx`, `.rodata`, the file
size and all 27 sections are unchanged; the only movement is
`.ARM.extab` 0x26294 -> 0x2628c (createShip's own record 336 -> 328 B).
`.rodata` stays at the same address because `.ARM.extab` ends at
`0x1a3b78 + 0x2628c = 0x1c9e04`, which still rounds up to the 16-aligned
`0x1c9e10`.

## Landed source change (ordinary C++)

Reverts the wave-119 `w6813` "one-call clamp + status null check" tuning
back to the faithful two-call ternaries and the placement-new EH shape
that was HEAD up to `b76b59bc3`, keeping the faithful spawn mapping and
the correct `setShipGroup(..., type, hostile)` argument:

```cpp
-    int level = Globals::status->getLevel();
-    if (level > 20) level = 20;
+    int level = Globals::status->getLevel() > 20 ? 20 : Globals::status->getLevel();
...
-    int empPoints = Globals::status->getLevel();
-    if (empPoints > 20) empPoints = 20;
-    empPoints = empPoints * 5 + 40;
+    int empPoints = (Globals::status->getLevel() > 20 ? 20 : Globals::status->getLevel()) * 5 + 40;
...
-    player->setEmpData(empPoints, empTime);
+    try {
+        player->setEmpData(empPoints, empTime);
+    } catch (...) {
+        __builtin_trap(); // lint: arm_builtin anchor: Level::createShip; removal grows body 2764 -> 2772
+    }
...
-    if (Globals::status == nullptr) return nullptr;
     KIPlayer *ship = nullptr;
...
-        PlayerFighter *fighter = new PlayerFighter(type, race, player, nullptr, x, y, z, hostile);
+        PlayerFighter *fighter = (PlayerFighter *) ::operator new(sizeof(PlayerFighter)); // lint: operator_call
+        new (fighter) PlayerFighter(type, race, player, nullptr, x, y, z, hostile);
...
-        PlayerFixedObject *fixed = new PlayerFixedObject(type, race, player, nullptr, x, y, z);
+        PlayerFixedObject *fixed = (PlayerFixedObject *) ::operator new(sizeof(PlayerFixedObject)); // lint: operator_call
+        new (fixed) PlayerFixedObject(type, race, player, nullptr, x, y, z);
```

The spawn mapping stays the faithful one (`x = wx + jitterX - 20000`,
`y = wy + jitterY - 20000`, `z = wz + jitterZ - 20000`, jitter order
X,Y,Z) and the waypoint fields stay `wx/wy/wz`.

## Why this scores higher

With the two ship ctors as placement-new, the exception-pointer vreg of
the `operator new` cleanup landing pads is allocated to **r8** (as in the
original) instead of r6, and Greedy RA then reproduces the original's
whole callee-saved permutation:

| value | original | landed | w6813 HEAD |
|---|---|---|---|
| status | r4 | r4 | r4 |
| shipClass | r5 -> sl -> r6 | r5 -> sl -> r6 | spilled |
| nextInt fn ptr | r6 | r6 | r6 |
| mission | r8 | r8 | r8 |
| this | r9, spilled at `0xc0332` | r9, spilled at `0xc0332` | r9, spilled at `0xc030a` |
| empPoints | sl | sl | fp |
| type | fp, spilled at `0xc0336` | fp, spilled at `0xc0336` | sl |
| ship / fixed | r9 | r9 | fp |
| volumes | fp | fp | r9 |
| landing-pad exception ptr | r8 | r8 | r6 |

The prologue is instruction-for-instruction identical to the original
(`str r1,[sp,#60]; mov r9,r0; ldr r0,[pc]; mov fp,r3; mov r5,r2; ...;
mov sl,r5`), the instruction count is now exactly 1007, and the whole
capital-ship block (`volumes` in fp, `fixed` in r9) matches.

## Measurements (official scorer, full link; 2764 B unless noted)

| variant | raw % | LSDA | note |
|---|---:|---:|---|
| w6813 HEAD (one-call clamps + `nc`, real new) | 70.2 | 336 | base |
| **landed: placement new + try/trap, faithful spawn, `hostile`** | **72.1** | 328 | |
| placement new + try/trap, faithful spawn, `group` | 72.6 | 328 | wrong 3rd arg |
| placement new + try/trap, tuned spawn, `hostile` | 73.9 | 328 | wrong spawn |
| placement new + try/trap, tuned spawn, `group` (old `e2fa39a15`) | 74.4 | 328 | wrong spawn + arg |
| placement new, no try, faithful spawn | 72.7 | 328 | |
| real new + faithful two-call clamps, no `nc` | 67.4 | 336 | |
| real new + `nc` (w6799) | 68.3 | 336 | 2780 B |

The tuned spawn (`x = jitterZ + jitterX`, `z = wz + wx`) and the `group`
3rd argument are pure RA/pool-placement hacks; the binary says
`x = wx + jitterX`, `z = wz + jitterZ` and `setShipGroup(..., hostile)`
(`mov r3,r8` at `0xc0716`, r8 = 5th arg), so the faithful forms were
landed and the higher-scoring hacks are recorded here only.

## Real-new sweep (dead ends, all measured)

A one-TU mini harness (`mscore2.py`, below) that reproduces the linked
`createShip` byte-for-byte and scores within 0.1 of the official verifier
was used for a large sweep of the real-new family.  Nothing beat 70.2:

* clamp grid (one-call vs two-call ternary x `nc`): 63.7 / 64.2 / 67.1 /
  68.0 / 68.3 / 70.2 — `L1 E1 nc` (HEAD) is the maximum.
* `wx/wy/wz` declaration orders (2), jitter call orders (5), `wp`
  init/ternary/chain/bool forms, `mission` before/after the waypoint
  block: 66.4-70.2.
* `hitpoints` decl-then-assign / reordered sums / `mission*4` first:
  66.0-70.2.
* `switch (type)` case orders (5) and if-chain: 70.2 tie.
* `type == 14 ? 25 : 5` spellings, `bool is14` hoist, cargo switch:
  70.2 tie.
* `armor` if-form, `empTime = 15000` init form, `player` decl-then-assign:
  70.2 tie.
* `ship` `0`/`NULL`/decl-then-assign, `return` cast spellings: 70.2 tie.
* `nc` spellings (`!status`, `== 0`, braces, `return 0`, `return ship`,
  duplicate check, `while`): 70.2 tie.
* extra late reads (`if (shipClass < 0) return nullptr;`, `type < 0`,
  `race < 0`): 41-47 (extra branch costs more than it gains).
* block reorders (`empPoints` before `hitpoints`/`level`/`switch`,
  `armor` early, `player` early): 64.9-70.2.
* `try`/`catch` around `setEmpData`/`armor`/`mission`/`level`/
  `setShipGroup`/`addObject`/`delete cargo`/`setLevel`/`setBV`:
  61.1-70.2.
* 900 random combinations of 40 statement-level rewrites: max 70.2.

A useful diagnostic: deleting the `type == 14 ? 25 : 5` line (one `type`
use) flips `shipClass` from spilled to `fp`, which shows the real-new
allocation is a knife-edge tie-break between `type` and `shipClass` spill
weights.  No semantically neutral rewrite of that use moves it.

## Reproduction

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
cmake --build cmake-build-match --target drift -j6
python3 tools/verify/verify.py \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN5Level10createShipEiiiP8Waypointbb
```

Scratch helpers (not committed): `/dev/shm/w6829/mscore2.py` (one-TU
compile + relocation-patched `verify.disassemble`, accurate to 0.1),
`search1.py`, `search2.py`, `grid.py`, `pro.py`, `e*.json`.

## Revert note: `.rodata` alignment wall

On current master the placement-new shape shrinks createShip's LSDA to 328 B
(original 336 B). With `Generator::createMission` also 4 B short, `.ARM.extab`
becomes 156,296 B instead of 156,304 B. That crosses the 16-byte alignment
boundary: `.ARM.extab` ends at `0x1a3b78 + 0x26288 = 0x1c9e00`, so `.rodata`
moves `0x1c9e10 -> 0x1c9e00` and **462 functions lose `byte_exact`**
(4505 -> 4043, gate FAIL). The current 156,304 B is exactly the minimum that
keeps `.rodata` at `0x1c9e10`; no further `.ARM.extab` byte may be lost until
`Generator::createMission` is restored to 172 B. The src change was therefore
reverted; the 72.23 % shape stays recorded here and can be re-landed only
together with the 172 B Generator LSDA.
