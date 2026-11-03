# `SolarSystem` emission order — w3745

Status: landed in this pass (2026-11-03). Branch `wave87-w3745`, based on master
`7e1783ad` (`docs: lock Radio emission order`). Original:
`../_work/bins/android_2.0.16_libgof2hdaa.so` (stripped, armeabi-v7a,
NDK r18b clang 7.0.2, `-Oz`).

This is the next frontier recorded in
`_work/reconstructions/radio_setmessages_placement_w3745.md` §6.

The section-ordering-file gate stays at **byte 4497 / linked 4497**,
`.text 0x1337c8 @ 0x69440`, lint `CLEAN ✓`, drift 0. The gate `.so` is
**byte-identical** before and after, and `section_order.txt` is unchanged.
The plain no-order link improves from **2857 / 4497** to **2860 / 4497**
(`+3` byte-exact), exact addresses `3470 -> 3495` (`+25`), exact-address
prefix `3385 -> 3410` (`+25`). The first plain no-order mismatch moves from
`SolarSystem::getName()` to `GameRecord::GameRecord()`.

## 1. Baseline

`cmake-build-match` was configured with `cmake --preset default`. Baseline
(`cmake --build cmake-build-match --target check -j8`, then `lint`, `drift`):

```text
verify: byte 4497  linked 4497  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓ (no new soft violations; hard invariants 0)
drift: 0 field(s) across 0 class(es)
```

Gate ELF layout (`readelf -SW cmake-build-match/libgof2hdaa.so`):

```text
.text        0x1337c8 @ 0x69440
.ARM.exidx   0x006f70 @ 0x19cc08
.ARM.extab   0x02628c @ 0x1a3b78
.rodata      0x041b67 @ 0x1c9e10
.data.rel.ro 0x002fe8 @ 0x20cbb0
.data        0x00296c @ 0x214000
.bss         0x0069ac @ 0x216970
```

Baseline plain no-order link (`relink_noorder.py`, the exact gold line minus
`--section-ordering-file`):

```text
verify: byte 2857  linked 4497  fuzzy_perfect 4497
addr_match: 3470/4524 same; prefix 3385
first mismatch: original 0x1556f0 SolarSystem::getName()
                ours     0x15571e SolarSystem::getName()
```

Plain no-order section layout (unchanged by the pass):

```text
.text        0x1337e8 @ 0x69440
.ARM.exidx   0x006f68 @ 0x19cc28
.ARM.extab   0x02628c @ 0x1a3b90
.rodata      0x041b67 @ 0x1c9e20
.data.rel.ro 0x002fe8 @ 0x20cbb0
.data        0x00296c @ 0x214000
.bss         0x0069ac @ 0x216970
```

## 2. Original emission order

The original's `.dynsym` address order (`nm -nSD --defined-only`):

```text
0x1555dc  0x0b8  SolarSystem::SolarSystem(...)          # C1 == C2
0x155694  0x058  SolarSystem::~SolarSystem()            # D1 == D2
0x1556ec  0x004  SolarSystem::getIndex()
0x1556f0  0x00e  SolarSystem::getName()
0x1556fe  0x004  SolarSystem::getSecurityLevel()
0x155702  0x004  SolarSystem::getRace()
0x155708  0x018  SolarSystem::getAttackRace()
0x155720  0x04c  SolarSystem::hasPirateBase()
0x15576c  0x022  SolarSystem::stationIsInSystem(int)
0x155790  0x04c  SolarSystem::hasHiddenBlueprint()
0x1557dc  0x004  SolarSystem::getX()
0x1557e0  0x004  SolarSystem::getY()
0x1557e4  0x004  SolarSystem::getZ()
0x1557e8  0x004  SolarSystem::getTextureIndex()
0x1557ec  0x004  SolarSystem::getWarpGateIndex()
0x1557f0  0x006  SolarSystem::getWarpGateEnumIndex()
0x1557f6  0x022  SolarSystem::getStationEnumIndex(int)
0x155818  0x024  SolarSystem::currentOrbitHasWarpGate()
0x15583c  0x01e  SolarSystem::stationIsInSystem(Station*)
0x15585c  0x040  SolarSystem::isFullyDiscovered()
0x15589c  0x006  SolarSystem::setCoords(int, int)
0x1558a2  0x004  SolarSystem::getStations()
0x1558a6  0x004  SolarSystem::getForbiddenGoods()
0x1558aa  0x004  SolarSystem::getRoutes()
0x1558ae  0x028  SolarSystem::systemIsInSystemRoutes(int)
0x1558d6  0x006  SolarSystem::isVisible()
0x1558dc  0x006  SolarSystem::setVisible(bool)
0x1558e2  0x01e  SolarSystem::hasNoOwner()
```

The HD 1.1.19 source-layout dump independently witnesses the same order for
the functions it contains
(`_work/original_layout/original_layout.json`,
`tu_symbols.gof2hd_1.1.19_ios["Classes/Game/SolarSystem.cpp"].symbols`):
`ctor, ctor, dtor, dtor, getIndex, getName, getSecurityLevel, getRace,
getAttackRace, hasPirateBase, stationIsInSystem(int), getX, getY, getZ,
getTextureIndex, getWarpGateIndex, getWarpGateEnumIndex, getStationEnumIndex,
currentOrbitHasWarpGate, stationIsInSystem(Station*), isFullyDiscovered,
getStations, getRoutes, systemIsInSystemRoutes, isVisible, hasNoOwner`.
The Android 2.0.16 binary adds `hasHiddenBlueprint`, `setCoords`,
`getForbiddenGoods` and `setVisible`; their positions are taken from the
Android `.dynsym` above. The TU mapping is `exact(HD)`:
`src/game/world/SolarSystem.cpp` -> `Classes/Game/SolarSystem.cpp`.

## 3. Baseline object order

`readelf -SW cmake-build-match/CMakeFiles/gof2.dir/src/game/world/SolarSystem.cpp.obj`
(section-header order = emission order; weak template copies included):

```text
SolarSystem::SolarSystem(...)
SolarSystem::~SolarSystem()
void ArrayRelease<int>(Array<int>&)
Array<int>::~Array()
SolarSystem::getIndex()
SolarSystem::getRace()
SolarSystem::getSecurityLevel()
SolarSystem::getTextureIndex()
SolarSystem::getX()
SolarSystem::getY()
SolarSystem::getZ()
SolarSystem::getWarpGateIndex()
SolarSystem::getStations()
SolarSystem::getRoutes()
SolarSystem::getForbiddenGoods()
SolarSystem::isVisible()
SolarSystem::getName()
SolarSystem::getAttackRace()
SolarSystem::hasPirateBase()
SolarSystem::stationIsInSystem(int)
SolarSystem::hasHiddenBlueprint()
SolarSystem::getWarpGateEnumIndex()
SolarSystem::getStationEnumIndex(int)
SolarSystem::currentOrbitHasWarpGate()
SolarSystem::stationIsInSystem(Station*)
SolarSystem::isFullyDiscovered()
SolarSystem::setCoords(int, int)
SolarSystem::systemIsInSystemRoutes(int)
SolarSystem::setVisible(bool)
SolarSystem::hasNoOwner()
```

The two weak `Array<int>` copies stay immediately after the destructor (their
first user) and are linker-deduplicated; they are not part of the strong order.

## 4. Landed source change

Only change: reorder the top-level definitions in
`src/game/world/SolarSystem.cpp` so the emitted strong order is the original's.
No body was edited. The three file-static tables
(`kPirateBaseStations`, `kAttackRaceTable`, `kBlueprintStations`) stay in their
original declaration order at the top of the file, so `.rodata` is unchanged.

```text
before (source / object strong order):
  ctor, dtor, getIndex, getRace, getSecurityLevel, getTextureIndex, getX,
  getY, getZ, getWarpGateIndex, getStations, getRoutes, getForbiddenGoods,
  isVisible, getName, getAttackRace, hasPirateBase, stationIsInSystem(int),
  hasHiddenBlueprint, getWarpGateEnumIndex, getStationEnumIndex,
  currentOrbitHasWarpGate, stationIsInSystem(Station*), isFullyDiscovered,
  setCoords, systemIsInSystemRoutes, setVisible, hasNoOwner

after:
  ctor, dtor, getIndex, getName, getSecurityLevel, getRace, getAttackRace,
  hasPirateBase, stationIsInSystem(int), hasHiddenBlueprint, getX, getY, getZ,
  getTextureIndex, getWarpGateIndex, getWarpGateEnumIndex,
  getStationEnumIndex, currentOrbitHasWarpGate, stationIsInSystem(Station*),
  isFullyDiscovered, setCoords, getStations, getForbiddenGoods, getRoutes,
  systemIsInSystemRoutes, isVisible, setVisible, hasNoOwner
```

The resulting object order is exactly the original `.dynsym` order:

```text
SolarSystem::SolarSystem(...)
SolarSystem::~SolarSystem()
void ArrayRelease<int>(Array<int>&)
Array<int>::~Array()
SolarSystem::getIndex()
SolarSystem::getName()
SolarSystem::getSecurityLevel()
SolarSystem::getRace()
SolarSystem::getAttackRace()
SolarSystem::hasPirateBase()
SolarSystem::stationIsInSystem(int)
SolarSystem::hasHiddenBlueprint()
SolarSystem::getX()
SolarSystem::getY()
SolarSystem::getZ()
SolarSystem::getTextureIndex()
SolarSystem::getWarpGateIndex()
SolarSystem::getWarpGateEnumIndex()
SolarSystem::getStationEnumIndex(int)
SolarSystem::currentOrbitHasWarpGate()
SolarSystem::stationIsInSystem(Station*)
SolarSystem::isFullyDiscovered()
SolarSystem::setCoords(int, int)
SolarSystem::getStations()
SolarSystem::getForbiddenGoods()
SolarSystem::getRoutes()
SolarSystem::systemIsInSystemRoutes(int)
SolarSystem::isVisible()
SolarSystem::setVisible(bool)
SolarSystem::hasNoOwner()
```

## 5. Measured before/after

`gate` is `cmake --build cmake-build-match --target check -j8` (the
ordering-file build). `plain` is `relink_noorder.py` (the exact gold line minus
`--section-ordering-file`) followed by `verify.py` and `addr_match.py`.

| state | gate byte/linked | gate `.so` sha256 | plain byte | funcs at orig addr | prefix | first plain mismatch (orig vs ours) |
|---|---:|---|---:|---:|---:|---|
| baseline | **4497 / 4497** | `1ecc20f3…` | **2857** | 3470 / 4524 | 3385 | `SolarSystem::getName()` `0x1556f0` vs `0x15571e` |
| reordered (landed) | **4497 / 4497** | `1ecc20f3…` | **2860** | **3495 / 4524** | **3410** | `GameRecord::GameRecord()` `0x155900` vs `0x15591a` |

The landed delta is **+3 plain byte_exact, +25 functions at their original
address, +25 exact-address prefix**, with the gate counts unchanged.

Per-function plain-link byte changes (`verify.py` `report.json` diff): 3 gains,
0 losses:

```text
SolarSystem::getName()                 False -> True
SolarSystem::currentOrbitHasWarpGate() False -> True
SolarSystem::isFullyDiscovered()       False -> True
```

All 30 `SolarSystem` `.dynsym` rows (C1/C2, D1/D2 and the 26 unique functions)
are now at the original address and size in the plain no-order link:

```sh
NM=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm
$NM -nSD --defined-only ../_work/bins/android_2.0.16_libgof2hdaa.so \
  | grep '_ZN11SolarSystem' | awk '{print $1, $2, $4}' > /tmp/w3745/solar/orig_syms.txt
$NM -nSD --defined-only /tmp/w3745/solar/after_noorder.so \
  | grep '_ZN11SolarSystem' | awk '{print $1, $2, $4}' > /tmp/w3745/solar/after_syms.txt
diff /tmp/w3745/solar/orig_syms.txt /tmp/w3745/solar/after_syms.txt
# no output (identical)
```

The gate `.so` is byte-identical before and after:

```sh
sha256sum /tmp/w3745/solar/gate_base.so /tmp/w3745/solar/gate_after.so
# 1ecc20f3fbd49abce61ac8426fee08743f908610d5d0a6dbb69c1e4d6e4c050b  both
cmp /tmp/w3745/solar/gate_base.so /tmp/w3745/solar/gate_after.so   # 0
diff /tmp/w3745/solar/section_order_base.txt /tmp/w3745/solar/section_order_after.txt
# no output
```

The plain-link `.rodata` is byte-identical before and after (the reorder does
not move the static tables or any literal pool):

```sh
OBJCOPY=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objcopy
$OBJCOPY -O binary --only-section=.rodata /tmp/w3745/solar/base_noorder.so  /tmp/w3745/solar/base.rodata
$OBJCOPY -O binary --only-section=.rodata /tmp/w3745/solar/after_noorder.so /tmp/w3745/solar/after.rodata
cmp /tmp/w3745/solar/base.rodata /tmp/w3745/solar/after.rodata   # 0
```

## 6. Remaining non-byte-exact `SolarSystem` rows

Five `SolarSystem` functions are at the correct address but still differ in
exactly one 32-bit word each. All five differences are `+0x10` and are the
known plain-link layout artifacts (`.rodata` base `0x1c9e20` vs original
`0x1c9e10`, `.text` size `0x1337e8` vs `0x1337c8`):

| function | differing word | original | ours | what |
|---|---|---|---|---|
| `getAttackRace()` | `+0x14` | `0x000a6160` | `0x000a6170` | literal pointer to `kAttackRaceTable` (`+0x10` rodata base) |
| `hasPirateBase()` | `+0x48` | `0x000a60ac` | `0x000a60bc` | literal pointer to `kPirateBaseStations` (`+0x10`) |
| `stationIsInSystem(Station*)` | `+0x18` | `0x2000be91` | `0x2000bea1` | `b.w` to the end-of-`.text` Thumb veneer (`+0x10` text shift) |
| `hasHiddenBlueprint()` | `+0x48` | `0x000ad968` | `0x000ad978` | literal pointer to `kBlueprintStations` (`+0x10`) |
| `getWarpGateEnumIndex()` | `+0x04` | `0x2000beb9` | `0x2000bec9` | `b.w` to the same veneer class (`+0x10`) |

The veneer targets are `0x19c568` / `0x19c578` (original) vs `0x19c588` /
`0x19c598` (plain), i.e. the same shift as the plain `.text` artifact. These
are not caused by the `SolarSystem` ordering; they are the same global
plain-link artifacts documented in
`_work/reconstructions/radiomessage_ctor_placement_w3745.md` §5.2 and
`_work/reconstructions/gun_setoffset_overload_order_w3745.md` §5.

## 7. Next frontier

The first plain no-order mismatch is now:

```text
original 0x155900  GameRecord::GameRecord()   (C1/C2, 0xd4 bytes)
ours     0x15591a  GameRecord::GameRecord()
```

Our plain `GameRecord` order starts with the destructor
(`0x155900 GameRecord::~GameRecord()`), then the constructor at `0x15591a`;
the original starts with the constructor at `0x155900`, then the destructor at
`0x1559d4`, then `load()` at `0x1559f0`. This is a separate TU
(`src/game/mission/GameRecord.cpp`) and was not changed in this pass.

## 8. Exact commands

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8

# baseline plain no-order link + measurements
python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745/solar/base_noorder.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745/solar/verify_base \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745/solar/base_noorder.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745/solar/base_noorder.so

# after the source reorder
cmake --build cmake-build-match --target check -j8
python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745/solar/after_noorder.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745/solar/verify_after \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745/solar/after_noorder.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745/solar/after_noorder.so

# object section order
readelf -SW cmake-build-match/CMakeFiles/gof2.dir/src/game/world/SolarSystem.cpp.obj \
  | sed -n 's/^ *\[ *[0-9]*\] \(\.text\.[^ ]*\).*/\1/p' \
  | sed 's/^\.text\.//' | c++filt

# original address order
NM=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm
$NM -nSD --defined-only ../_work/bins/android_2.0.16_libgof2hdaa.so | grep '_ZN11SolarSystem'

# gate .so A/B (baseline vs landed source)
cp cmake-build-match/libgof2hdaa.so /tmp/w3745/solar/gate_after.so
git stash push -- src/game/world/SolarSystem.cpp
cmake --build cmake-build-match --target link_so -j8
cp cmake-build-match/libgof2hdaa.so /tmp/w3745/solar/gate_base.so
git stash pop
cmake --build cmake-build-match --target link_so -j8
sha256sum /tmp/w3745/solar/gate_base.so /tmp/w3745/solar/gate_after.so
cmp /tmp/w3745/solar/gate_base.so /tmp/w3745/solar/gate_after.so   # 0
```

## 9. Proven vs hypothesis

**Proven by measurement**

* Baseline gate is `byte 4497 / linked 4497`, `.text 0x1337c8 @ 0x69440`,
  lint `CLEAN ✓`, drift 0; plain no-order is `2857 / 4497`, prefix 3385,
  3470 / 4524, first mismatch `SolarSystem::getName()` `0x1556f0` vs
  `0x15571e`.
* The original emits `SolarSystem` in the `.dynsym` address order listed in
  §2; the HD 1.1.19 source-layout dump independently witnesses the same order
  for the functions it contains, and the TU mapping is `exact(HD)`.
* The baseline object emits the strong functions in source order, which is not
  the original order (`getRace`/`getSecurityLevel` swapped, `getName` late,
  `getStations`/`getRoutes`/`getForbiddenGoods` early, `isVisible` early).
* Reordering the definitions is gate-neutral (`4497 / 4497`, gate `.so`
  **byte-identical**, `section_order.txt` identical, lint `CLEAN ✓`, drift 0)
  and makes all 30 `SolarSystem` `.dynsym` rows address- and size-exact in the
  plain no-order link.
* The plain link improves `2857 -> 2860` byte-exact, `3470 -> 3495` exact
  addresses, `3385 -> 3410` exact-address prefix; the per-function diff is
  3 gains / 0 losses (`getName`, `currentOrbitHasWarpGate`,
  `isFullyDiscovered`).
* The plain-link `.rodata` is byte-identical before and after; the static
  tables and all literal pools are unchanged.
* Five `SolarSystem` rows remain non-byte-exact in the plain link solely
  because of the plain `.rodata +0x10` / `.text +0x20` layout artifacts; each
  has exactly one differing `+0x10` word, all accounted for in §6.

**Hypothesis / still open**

* The plain `.text +0x20`, `.ARM.exidx -8` and `.ARM.extab -0xc` are the known
  plain-link artifacts; until those are exact, any function with a `.rodata`
  REL32 literal or an end-of-`.text` veneer branch will stay non-byte-exact
  even when its instruction stream and address are correct.
* The ordering file is still required: the plain link is `2860`, not `4497`.
* The next TU (`GameRecord.cpp`) needs its own emission-order pass: the
  original emits the constructor before the destructor, ours emits the
  destructor first.
