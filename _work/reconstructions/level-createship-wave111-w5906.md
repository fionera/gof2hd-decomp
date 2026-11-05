# Level::createShip — wave 111 (w5906): 332-byte LSDA, `.ARM.extab` exact

**Status:** source change landed. Target
`_ZN5Level10createShipEiiiP8Waypointbb` (original 2764 B @ `0xc0214`,
Thumb entry `0xc0215`, 1007 insns). The createShip LSDA record is now
**332 B / 54 rows / ttype 0x00 / cslen 300** (was 328 B / 54 rows /
cslen 299), and `.ARM.extab` is **0x26298** (was 0x26294; original
0x26298). The fuzzy score drops **74.4 -> 61.9** because the source shape
that produces the 332-byte record changes register allocation and
literal-pool placement; the gate is still green.

## 0. Baseline and landed gate

```text
before: verify byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.92
        data 843/843  imports 207  lint CLEAN  drift 0
        .text 0x1337c8  .ARM.exidx 0x6f70  .ARM.extab 0x26294 (4 B short)
        createShip 74.4 %, 2764 B, LSDA 328 B / cslen 299 / 54 rows
after:  verify byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.91
        data 843/843  imports 207  lint CLEAN  drift 0
        .text 0x1337c8  .ARM.exidx 0x6f70  .ARM.extab 0x26298 (exact)
        createShip 61.9 %, 2764 B, LSDA 332 B / cslen 300 / 54 rows
```

The `.text`, `.ARM.exidx`, `.data`, `.got`, `.rel.dyn` sizes and the
2,187,328-byte file size are unchanged. `.rodata` remains at its previous
size (0x41c27; still different from the original 0x41b67) and was not
touched by this pass. The only section-size change is `.ARM.extab`
0x26294 -> 0x26298.

## 1. Landed diff (`src/game/world/Level.cpp`)

```diff
         if (type == 0x2c || type == 0x31) {
-            delete fighter->cargo;
-            fighter->cargo = nullptr;
+            if (fighter->cargo != nullptr) {
+                delete fighter->cargo;
+                fighter->cargo = nullptr;
+            }
         } else if (type == 0x33) {
...
     } else if (shipClass == 1) {
         // capital ships: hull collision boxes per race
+        Array<BoundingVolume *> *volumes = new Array<BoundingVolume *>();
         PlayerFixedObject *fixed = (PlayerFixedObject *) ::operator new(sizeof(PlayerFixedObject)); // lint: operator_call
         new (fixed) PlayerFixedObject(type, race, player, nullptr, x, y, z);
-        Array<BoundingVolume *> *volumes = new Array<BoundingVolume *>();
```

Both edits are ordinary C++: an explicit null check before deleting the
fighter cargo, and allocating the collision-volume array before the
`PlayerFixedObject` allocation in the capital-ship branch. The original
`delete` null check is still emitted once; the explicit guard changes the
branch layout enough to move the call-site table from 299 to 300 bytes.

## 2. Why 332 is the section-closing size

The original createShip record is **336 B / 57 rows / ttype 0xff**
(real `new`, no try). The current placement-new + catch-all shape is
328 B / 54 rows / ttype 0x00. The other `.ARM.extab` records in the
current image are net +4 relative to the original, so the total is 4
bytes short. A createShip record of **332 B** makes the total exactly
0x26298; a 336-byte createShip record would make the total 0x2629c and
shift `.rodata`/`.got` unless another record is reduced by 4 (the
wave-107 `Hud::draw` dock-order route).

All 332-byte shapes measured in this pass have `cslen = 300` and
`ttype = 0x00` (54 rows). The 57-row real-`new` family always measured
336 B (`cslen = 315`, `ttype = 0xff`). No 332 B / 57-row shape was found;
the task hint's "57 rows" appears to describe the 336-byte original, not
the 332-byte section-closing record.

## 3. Measured variants (full link, official verifier)

`pct` is the official fuzzy score; `body` is the linked function size;
`rec`/`cslen` are the createShip `.ARM.extab` record and call-site table
lengths; `byte` is whole-`.so` `byte_exact`.

| variant | pct | body | rec | cslen | rows | ttype | extab | byte |
|---|---:|---:|---:|---:|---:|---|---:|---:|
| base (placement new, catch-all, switch) | 74.4 | 2764 | 328 | 299 | 54 | 0x00 | 0x26294 | 4500 |
| `cargo_if` only | 73.4 | 2768 | 328 | 299 | 54 | 0x00 | 0x26294 | n/a |
| `vol_before` only | 59.3 | 2760 | 332 | 300 | 54 | 0x00 | 0x26298 | n/a |
| **landed (`cargo_if` + `vol_before`)** | **61.9** | **2764** | **332** | **300** | **54** | **0x00** | **0x26298** | **4500** |
| `emp_ne0` (`shipClass != 0`) | 39.3 | 2764 | 332 | 300 | 54 | 0x00 | 0x26298 | n/a |
| `type_split` + `vol_before` | 59.3 | 2760 | 332 | 300 | 54 | 0x00 | 0x26298 | n/a |
| faithful real `new` both, if-chain, no try | 66.0 | 2764 | 336 | 315 | 57 | 0xff | 0x2629c | n/a |

The faithful real-`new` shape is the original's LSDA exactly (336 B /
57 rows), but its record is 4 bytes larger than the current `.ARM.extab`
budget; landing it alone gives `.ARM.extab` 0x2629c and drops
`byte_exact`. The wave-107 note records the `Hud::draw` -4 compensation
that makes that route gate-green; this pass deliberately kept the change
inside `Level.cpp` and targeted the 332-byte section-closing record.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w5906
cmake --build cmake-build-match --target check -j6
cmake --build cmake-build-match --target drift -j6
python3 tools/verify/verify.py \
  --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN5Level10createShipEiiiP8Waypointbb
```

Scratch harnesses (this pass, `/dev/shm/w5906`): `fast.sh` (compile-only
section probe), `measure.py` (official verifier + section sizes),
`decobj.py` (object LSDA decoder), `cmp_extab.py` (per-function extab
record map), `probe*.py` / `search*.py` (variant sweeps).
