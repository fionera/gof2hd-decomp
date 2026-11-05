# Level::createShip — wave 118 (w6605): 336-byte LSDA study, no gate-safe source win

Status: **analysis only, no source change** (2026-11-05).
Target `_ZN5Level10createShipEiiiP8Waypointbb` (`Level::createShip`,
original 2764 B @ `0xc0214`, Thumb entry `0xc0215`). Branch `wave118-w6605`,
base `3d4a98a3e`.

## Result

HEAD (wave 117 placement-new + `try`/`catch(...)`) is unchanged:

```
createShip fuzzy (raw)            73.500000
createShip size                   2764 B
createShip LSDA record            328 B (original: 336 B)
.ARM.extab size / addr            0x2628c @0x1a3b78 (original 0x26298)
byte_exact / linked_exact         4501 / 4501
imports / data / lint / drift     207 / 843 / CLEAN / 0
```

`check`: **RATCHET PASS**.

Every 336-byte shape found this wave is either a score regression or breaks
the gate. The best 336-byte shape that still scores 73.5 is
`catch (std::bad_alloc &)` before the catch-all, but it drops
`byte_exact` 4501 -> 4234 through a gold `.dynsym` reorder (see below), so it
cannot be landed.

## What the original LSDA actually is

Parsing the original `.ARM.extab` record at `0x1ab09c` (personality word at
+0, ARM unwind opcodes at +4..+15, LSDA at +16):

```
LPStart enc  0xff (omit)
TType enc    0xff (omit)          <- no type table at all
call enc     0x01 (uleb128)
call len     315
call sites   57, every action == 0 (cleanup only)
action table empty
record       16 + 5 + 315 = 336 -> rounded to 336
```

So the original has **no catch clause**: all 57 call sites are `new`
cleanup landing pads. The original really does use real `new` for all three
objects — `blx _Znwj` at `0xc03a4` (Player), `0xc0408` (PlayerFixedObject),
`0xc0448` (Array), `0xc06bc` (PlayerFighter) — and the four cleanup stubs at
`0xc0bba/0xc0bc0/0xc0bc6/0xc0bcc` (`delete` + `_Unwind_Resume`).

HEAD's record is structurally different: TType enc `0x00` (absptr), ttbase
offset 308, call len 299, 54 call sites, action table `01 00` + padding
(the catch-all). The 3-site / 16-byte difference is exactly the three
`new` cleanups that placement-new removes.

Other original facts confirmed from the Thumb stream (useful for future
passes):

* `x`, `y`, `z` are `int` (`fp`, `r4`, `r8`) computed **before**
  `setEmpData` (`0xc03c8..0xc03e4`) and converted with `vcvt` at each ctor
  call site (3 conversions in each branch).
* Stack slots: `wpX` sp44, `wpY` sp48, `wpZ` sp40; `jitterX` sp36,
  `jitterY` sp32, `jitterZ` sp28.
* `PlayerFixedObject(type, race, player, nullptr, x, y, z)`;
  `PlayerFighter(type, race, player, nullptr, x, y, z, hostile)`;
  `setShipGroup(getShipGroup(type, race, group), type, hostile)`.
* The hitpoint if-chain is emitted `0x33`, `0x31`, `0x2c` (HEAD already
  matches this despite the opposite source order).

## Measurement table (official scorer, full link)

`raw %` from `tools/verify/verify.py`; `rec` is the createShip LSDA record
size; `byte` is the repo-wide `byte_exact`; `rel.dyn` is the section size.
All rows 2764 B unless noted.

| variant | raw % | rec | extab | byte | rel.dyn | gate |
|---|---:|---:|---:|---:|---:|---|
| **HEAD: placement-new both + try/catch(...)** | **73.500000** | 328 | 0x2628c | **4501** | 0x7048 | PASS |
| HEAD + `catch (std::bad_alloc &)` before catch-all | 73.500000 | **336** | 0x26294 | 4234 | 0x7048 | FAIL byte |
| HEAD + `catch (std::exception &)` before catch-all | 73.500000 | **336** | 0x26294 | 4230 | 0x7048 | FAIL byte |
| HEAD + `catch (int)` before catch-all | 73.500000 | **336** | 0x26294 | 1793 | 0x7050 | FAIL byte |
| HEAD + `catch (KIPlayer &)` before catch-all | 73.500000 | **336** | 0x26294 | 1793 | 0x7050 | FAIL byte |
| HEAD + `catch (std::bad_alloc &)` only (no catch-all) | 65.400000 | 332 | 0x26290 | 2372 (2772 B) | 0x7048 | FAIL size |
| real `new` all three, no try (int x / float z / float y) | 66.700000 | **336** | 0x26294 | **4501** | 0x7048 | PASS |
| real `new` all three, no try (float x / float z / int y) | 66.200000 | **336** | 0x26294 | **4501** | 0x7048 | PASS |
| real `new` all three, no try (float x / int z / float y) | 65.100000 | **336** | 0x26294 | **4501** | 0x7048 | PASS |
| real `new` all three, no try, all-int coords | 62.900000 | **336** | 0x26294 | 2372 (2788 B) | 0x7048 | FAIL size |
| real `new` all three + try | 63.200000 | 352 | 0x262a4 | 2363 (2768 B) | 0x7048 | FAIL size |
| real `new` fighter only + try | 61.600000 | 340 | 0x26298 | 2372 (2768 B) | 0x7048 | FAIL size |
| real `new` fixed only + try | 63.000000 | 340 | 0x26298 | 2372 (2768 B) | 0x7048 | FAIL size |
| placement-new both + try, all-int coords | 60.600000 | 328 | 0x2628c | 3103 (2752 B) | 0x7048 | FAIL size |
| HEAD, `if (mp==1||mp==23) {} else {...}` | 73.500000 | 328 | 0x2628c | 4501 | 0x7048 | PASS |
| HEAD, local `AEGeometry *geom` in the LOD `if` | 73.500000 | 328 | 0x2628c | 4501 | 0x7048 | PASS |
| HEAD, `bool skipLod` / `!(...)` forms | 73.500000 | 328 | 0x2628c | 4501 | 0x7048 | PASS |
| HEAD, race cases reordered 0,3,2,1,default | 73.500000 | 328 | 0x2628c | 4501 | 0x7048 | PASS |
| HEAD, race cases reordered 0,1,2,3,default | 66.900000 | 328 | 0x2628c | 4501 | 0x7048 | PASS |
| HEAD, race cases reordered 1,2,3,0,default | 66.500000 | 328 | 0x2628c | 4501 | 0x7048 | PASS |
| HEAD, `ship = fighter` before LOD `if` | 61.600000 | 340 | 0x26298 | 2372 (2768 B) | 0x7048 | FAIL size |
| HEAD, `ship = fighter` before cargo switch | 63.200000 | 352 | 0x262a4 | 2363 (2768 B) | 0x7048 | FAIL size |
| HEAD, cargo switch before LOD `if` | 63.000000 | 340 | 0x26298 | 2372 (2768 B) | 0x7048 | FAIL size |

## Why the typed catch cannot be landed

`catch (std::bad_alloc &)` reuses the existing GOT slot for
`_ZTISt9bad_alloc` (0x20ecec), so `.rel.dyn` stays 0x7048 and every section
address is unchanged. But the new typeinfo reference in `Level.o` makes gold
emit `_ZTISt9bad_alloc` at `.dynsym` index 3438 instead of 5524 (gold orders
dynamic symbols by first reference; `Level.o` is early in
`tools/source_order.txt`). That reorder rewrites `.dynsym`, `.hash`,
`.gnu.version`, `.rel.dyn`, `.rel.plt` and — through link-time relocation
addends — 359 literal-pool words in `.text` (e.g. `MiningGame::update`'s
jump table word `0x000f1732` becomes `0x000f1736`, while the original has
`0x000f1732`). 267 functions lose raw-byte equality; `linked_exact` stays
4501 because the disassembly is unchanged. `catch (int)` / `catch (KIPlayer&)`
are worse: they also need a new GOT slot, so `.rel.dyn` grows 0x7048 ->
0x7050 and `.rel.plt`/`.plt`/`.text` all shift by 8 (byte 1793).

A local (anonymous-namespace) catch type avoids the `.dynsym` reorder but
emits a typeinfo object: `.rel.dyn` +0x18, `.rodata` +0x30,
`.data.rel.ro` +0x10, `.got` +4 and `.text` shifts by 0x20 — also a gate
failure.

## Why the faithful real-`new` shape scores lower

The real-`new` body is the correct source shape (336 B, gate-safe), but its
register allocation diverges from the original from the first instruction:

```
orig:  str r1,[sp,#60] ; mov fp,r3 ; mov r5,r2   (race spilled, type->fp, shipClass->r5)
ours:  strd r1,r2,[sp,#56] ; mov sl,r3           (race+shipClass spilled, type->sl)
```

The cleanup landing pads keep the allocated pointer live across the ctor,
which changes the greedy allocation; the original keeps `type` in `fp`,
`shipClass` in `r5`/`sl`/`r6`, `this` in `r9`, `status` in `r4`. Copying
`shipClass`/`type` into locals, swapping the declaration orders, and
reordering the spawn/`ship` statements were all tried; none reproduce the
original prologue, and all 336-byte real-`new` variants stay at 65.1-66.7.

The remaining HEAD diffs are the same class of RA/literal-pool artefacts:
coordinate computation sunk after `setEmpData` (original computes three ints
before it), `ship`/`fighter` coalesced into one register (original uses
`r9`/`sl`), the LOD `addObject` block placed out-of-line (original places it
fall-through), and the `0.0f` BoundingAAB argument rematerialised with
`movs r0,#0` instead of reusing a zeroed register.

## Reproduction

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 tools/verify/verify.py --show _ZN5Level10createShipEiiiP8Waypointbb
cmake --build cmake-build-match --target check -j6
```

Scratch helpers (not committed): `/dev/shm/w6605/measure2.py` (splice body,
link, verify, report pct/size/LSDA/byte/linked/rel.dyn), `fast.py` +
`fastcompile.sh` (compile-only), `lsda2.py` (ARM EHABI LSDA parser),
`diffblocks.py` (instruction-stream diff blocks).
