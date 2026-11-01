# PLT / .dynsym order: the last 14 lines (w3418)

Status: research note (2026-11-01). Worktree `wt/w3418`, branch `wave87-w3418`,
based on master `fff40b4b`. Every number below was measured against
`../_work/bins/android_2.0.16_libgof2hdaa.so` with the repo's NDK r18b gold 1.12.
`src/`, `CMakeLists.txt` and `tools/` are unchanged in the committed tree; the two
experiments below were reverted before committing.

## TL;DR

* The measured baseline is **byte_exact 4171 / linked_exact 4495** (the task brief's
  `byte 4115` is stale; `docs/IDENTICALITY.md` already records 4171 after the
  `.rodata` output-group pass). Full gate: stubs 0, missing 0, extra 0, avg 99.89,
  data 843/843, imports 207, lint `CLEAN`, drift 0.
* The **`.plt` symbol diff is exactly 14 lines**, and it decomposes into **two
  independent groups**:
  1. **4 lines / 2 symbols** — `FileRead::loadStaticCollision` /
     `FileRead::loadStationCollision`. They are first referenced inside
     `Level::getBoundingVolume`, which our object emits immediately after
     `Level::createStaticObject`; the original emits it after
     `Level::createRadioMessages`.
  2. **10 lines / 5 symbols** — `Standing::setStanding`,
     `PlayerEgo::autoTurretIsEnabled`, `PlayerEgo::setAutoTurret`,
     `PlayerEgo::deleteHackingGame`, `StarSystem::switchSunForSupernovaIntro`.
     All five are first referenced inside the single function
     `LevelScript::process(int)`, whose machine-code call layout differs from the
     original's.
* The **`.dynsym` diff is exactly 14 gaps** (18 symbol occurrences on each side,
  `difflib` ratio 0.996648, 5352/5370 matched, 5226/5370 same position). The 14
  gaps are 5 distinct issues (Matrix/Vector/BSphere interleaving,
  RecordHandler/LODManager, ResourceTexture ctor, libzip `zip_err_*`, and the
  `_edata`/`sha256_block_data_order_neon` tail).
* Fixing group 1 is worth **+4 byte_exact** (inferred: `Level::getBoundingVolume`,
  `Level::createRadioMessages`, `PlayerStation::PlayerStation(Station*)` C1 and C2
  currently differ *only* in the two FileRead PLT offsets). No codegen-preserving
  way to defer the PLT-emission position was found; the obvious move is a measured
  regression.
* Fixing group 2 requires reconstructing `LevelScript::process` (currently 54.6%),
  not a PLT/link-order tweak.

## 1. Baseline

```
cmake --preset default
cmake --build cmake-build-match --target check -j8
```

```
verify: byte 4171  linked 4495  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`cmake --build cmake-build-match --target lint` -> `CLEAN ✓`;
`--target drift` -> `0 field(s) across 0 class(es)`.
`tools/verify_baseline.json` agrees: `byte_exact 4171`, `linked_exact 4495`.

## 2. Method

PLT symbol sequence (same command as `plt-dynsym-order.md`):

```sh
OD=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OD -d -j .plt ../_work/bins/android_2.0.16_libgof2hdaa.so | grep -o "<[^>]*@plt>:" | sed 's/[<>:]//g' > /tmp/plt_o.txt
$OD -d -j .plt cmake-build-match/libgof2hdaa.so        | grep -o "<[^>]*@plt>:" | sed 's/[<>:]//g' > /tmp/plt_u.txt
diff /tmp/plt_o.txt /tmp/plt_u.txt
```

Both files have **3674** PLT entries. `.dynsym` dump and `difflib` alignment use the
same parser/algorithm as `_work/reconstructions/dynsym_order_audit.md`
(`dynsym_dump.py` + `SequenceMatcher(autojunk=False)` over the 5370 defined symbols).

First-reference order was measured directly from the objects, in `link_order.rsp`
order, with `objdump -r` (relocation sections are in the same order as their
`.text.*` target sections; verified for `Level.cpp.obj`: reloc section indices
267/273/291/320 follow `.text` indices 159/162/173/190). For our binary this
first-reference sequence reproduces the PLT order of the relevant block exactly:

```
seq 6037  AEGeometry::setLodChildTransform   Level.cpp.obj .text.Level::createStaticObject       off 0x190c
seq 6040  FileRead::loadStaticCollision      Level.cpp.obj .text.Level::getBoundingVolume        off 0x002e
seq 6041  FileRead::loadStationCollision     Level.cpp.obj .text.Level::getBoundingVolume        off 0x0034
seq 6044  Player::setEmpData                 Level.cpp.obj .text.Level::createShip               off 0x01de
seq 6045  PlayerFighter ctor                 Level.cpp.obj .text.Level::createShip               off 0x04e8
seq 6054  Array<RadioMessage> ctor           Level.cpp.obj .text.Level::createRadioMessages      off 0x016a
seq 6056  RadioMessage ctor                  Level.cpp.obj .text.Level::createRadioMessages      off 0x0190
seq 6087  Status::incKills                   Level.cpp.obj .text.Level::enemyDied                off 0x003a
```

So **PLT order == input-object reloc-scan order**, and the input-object reloc-scan
order == the object's `.text.*` section order (which, with `-ffunction-sections`, is
the function emission order). The section-ordering file changes the final `.text`
addresses but *not* the PLT order.

## 3. The 14-line PLT diff

```
1950a1951,1952
> _ZN8FileRead19loadStaticCollisionEi@plt
> _ZN8FileRead20loadStationCollisionEi@plt
1957,1958d1958
< _ZN8FileRead19loadStaticCollisionEi@plt
< _ZN8FileRead20loadStationCollisionEi@plt
2817d2816
< _ZN8Standing11setStandingEii@plt
2822,2823d2820
< _ZN9PlayerEgo19autoTurretIsEnabledEv@plt
< _ZN9PlayerEgo13setAutoTurretEb@plt
2830c2827,2829
< _ZN10StarSystem26switchSunForSupernovaIntroEv@plt
---
> _ZN8Standing11setStandingEii@plt
> _ZN9PlayerEgo19autoTurretIsEnabledEv@plt
> _ZN9PlayerEgo13setAutoTurretEb@plt
2831a2831,2832
> _ZN9PlayerEgo17deleteHackingGameEv@plt
> _ZN10StarSystem26switchSunForSupernovaIntroEv@plt
2837d2837
< _ZN9PlayerEgo17deleteHackingGameEv@plt
```

Exact PLT addresses:

| symbol | orig | ours |
|---|---|---|
| `FileRead::loadStaticCollision` | `0x643b0` | `0x64368` |
| `FileRead::loadStationCollision` | `0x643bc` | `0x64374` |
| `Standing::setStanding` | `0x66c00` | `0x66c78` |
| `PlayerEgo::autoTurretIsEnabled` | `0x66c3c` | `0x66c84` |
| `PlayerEgo::setAutoTurret` | `0x66c48` | `0x66c90` |
| `StarSystem::switchSunForSupernovaIntro` | `0x66c9c` | `0x66cb4` |
| `PlayerEgo::deleteHackingGame` | `0x66cf0` | `0x66ca8` |

### 3.1 Group 1 — FileRead pair (4 diff lines)

Both symbols are first referenced **only** inside `Level::getBoundingVolume`
(`Level.cpp.obj` relocs at `+0x2e` / `+0x34`). The original's PLT order is:

```
1950 AEGeometry::setLodChildTransform   (createStaticObject, off 0x190c)
1951 Player::setEmpData                 (createShip,          off 0x01de)
1952 PlayerFighter ctor                 (createShip,          off 0x04e8)
1953 Array<RadioMessage> ctor           (createRadioMessages, off 0x016a)
1954 ArraySetLength<RadioMessage>       (createRadioMessages, off 0x0174)
1955 RadioMessage ctor (4 args)         (createRadioMessages, off 0x0190)
1956 RadioMessage ctor (5 args)         (createRadioMessages, off 0x02ea)
1957 FileRead::loadStaticCollision      (getBoundingVolume,   off 0x002e)
1958 FileRead::loadStationCollision     (getBoundingVolume,   off 0x0034)
```

Ours inserts the FileRead pair at 1951-1952, i.e. immediately after
`createStaticObject`, and shifts the `createShip`/`createRadioMessages` symbols down
by six slots. The object section order confirms why:

| our `Level.cpp.obj` section | index |
|---|---:|
| `.text._ZN5Level18createStaticObjectEP8Waypointib` | 159 |
| `.text._ZN5Level17getBoundingVolumeEiP10AEGeometry` | 162 |
| `.text._ZN5Level10createShipEiiiP8Waypointbb` | 173 |
| `.text._ZN5Level11createRouteEi` | 358 |
| `.text._ZN5Level14setPlayerRouteEP5Route` | 380 |
| `.text._ZN5Level19createRadioMessagesEi` | 386 |

`Level::createStaticObject` calls `getBoundingVolume` at source line 1327, so clang
creates the `getBoundingVolume` `Function` object during `createStaticObject`'s
codegen and emits its section immediately after it. In the original the
`getBoundingVolume` body is scanned after `createRadioMessages`. The original
source-layout dump (`_work/original_layout/original_layout.json`,
`gof2hd_1.1.19_ios` -> `Classes/Game/Level.cpp`) lists the definitions in exactly
that order:

```
16 _Level::createStaticObjects()
31 _Level::createStaticObject(Waypoint*, int, bool)
33 _Level::createShip(int, int, int, Waypoint*, bool, bool)
34 _Level::createRoute(int)
35 _Level::setPlayerRoute(Route*)
36 _Level::createRadioMessages(int)
39 _Level::getBoundingVolume(int, AEGeometry*)
49 _Level::enemyDied(int, bool)
```

and the original `.text` addresses agree (`createStaticObject` `0xbe6c1`,
`createShip` `0xc0215`, `createRadioMessages` `0xc0de5`, `getBoundingVolume`
`0xc3e49`). So the original's emission order is the source/definition order; our
clang emits `getBoundingVolume` at `min(definition, first-reference)` = the
`createStaticObject` call site.

**Measured impact if the pair moved back:** the two FileRead slots are the *only*
difference in four pct-100 functions:

| function | current diff |
|---|---|
| `Level::getBoundingVolume` | 2 `blx` words (FileRead pair) |
| `Level::createRadioMessages` | 6 `blx` words (3 symbols x 2 call sites: `Array<RadioMessage>`, `ArraySetLength`, `RadioMessage`), shifted by the FileRead insertion |
| `PlayerStation::PlayerStation(Station*)` C1 | 6 `blx` words (FileRead pair) |
| `PlayerStation::PlayerStation(Station*)` C2 | 6 `blx` words (FileRead pair) |

So a correct move would be **+4 byte_exact** (4171 -> 4175), with no other section
movement. That is the single highest-value remaining PLT item.

**Controlled experiment (regression, reverted).** Moving the whole
`Level::getBoundingVolume` definition from line 5906 to before
`Level::createStaticObject` (line 1275) moved the FileRead pair *earlier*, not
later:

```
before: byte 4171  linked 4495   PLT diff 14 lines
after:  byte 4169  linked 4495   PLT diff 25 lines
```

The new PLT order put `FileRead::loadStaticCollision/loadStationCollision` and
`BoundingAAB` at 1944-1946 (before `createStaticObject`), confirming that the
emission position follows the definition position when the definition is earlier
than the first reference. The experiment is a strict regression, so it was not
committed.

### 3.2 Group 2 — `LevelScript::process` call layout (10 diff lines)

All five symbols are first referenced only in `LevelScript::process(int)`
(`LevelScript.cpp.obj`, one `.text` section; `setAutoTurret`/`autoTurretIsEnabled`
are also referenced later by `MGame::OnTouchEnd` and `Hud::draw`, but the PLT slot
is created at the first reference in `LevelScript::process`). The original PLT
order therefore records the original machine-code call order:

| original PLT order | symbol | our call offset |
|---:|---|---:|
| 2817 | `Standing::setStanding` | `0x404a` |
| 2818 | `TargetFollowCamera::setRotationAroundTarget` | `0x1eba` |
| 2819 | `Player::setShootingEnabled` | `0x207a` |
| 2820 | `Mission::setType` | `0x2138` |
| 2821 | `AEGeometry::isVisible` | `0x2412` |
| 2822 | `PlayerEgo::autoTurretIsEnabled` | `0x4734` |
| 2823 | `PlayerEgo::setAutoTurret` | `0x4746` |
| 2824 | `PlayerWormHole::open` | `0x2804` |
| 2825 | `Level::flashScreen` | `0x28c4` |
| 2826 | `StarSystem::scaleSunDuringSupernovaIntro` | `0x33fa` |
| 2827 | `StarSystem::updateSupernova` | `0x3408` |
| 2828 | `PlayerEgo::isInWormhole` | `0x3d62` |
| 2829 | `PlayerWormHole::freeMissionLock` | `0x3e50` |
| 2830 | `StarSystem::switchSunForSupernovaIntro` | `0x566e` |
| 2831 | `PlayerEgo::setDockingState` | `0x4ccc` |
| 2832 | `StarSystem::switchPlanetForIntro` | `0x62d2` |
| 2833 | `Level::switchSkyboxForIntro` | `0x62da` |
| 2834 | `ParticleSystemManager::reset` | `0x62f8` |
| 2835 | `PlayerEgo::startSmokeEmission` | `0x6424` |
| 2836 | `PlayerStation::setVisible` | `0x677e` |
| 2837 | `PlayerEgo::deleteHackingGame` | `0x5442` |
| 2838 | `LevelScript::lookBehind` | `0x6b18` |

The original order is: `setStanding` before `setRotationAroundTarget`;
`autoTurretIsEnabled`/`setAutoTurret` immediately after `AEGeometry::isVisible`;
`switchSunForSupernovaIntro` before `setDockingState`; `deleteHackingGame` after
`PlayerStation::setVisible`. Our compiled order is the left-to-right order of the
`our call offset` column. This is a basic-block-layout difference in a function
that is only **54.6%** matched, so it is a symptom of the unreconstructed switch
body, not a TU/PLT-order problem. Reordering source cases is not a safe
PLT-only experiment; it needs the `LevelScript::process` reconstruction.

## 4. The 14 `.dynsym` gaps

`difflib` over the 5370 defined symbols: **16 matching blocks, 14 gaps, 18
original / 18 our symbol occurrences, ratio 0.996648, 5352 matched, 5226 same
position**. The 14 gaps are 5 distinct issues:

| # | issue | orig symbols | our owner | action |
|---:|---|---|---|---|
| 1 | Matrix/Vector/BSphere interleaving | `MatrixRotateVector`, `MatrixTransformVector`, `BSphere::Merge(Transform)` | `Matrix.cpp`, `BSphere.cpp` | original `.dynsym` interleaves them inside the Vector block; our link order is `Vector.cpp`(19), `BSphere.cpp`(20), `Matrix.cpp`(29) and our `.symtab` is alphabetical. Not a TU-membership error. |
| 2 | RecordHandler vs LODManager | `RecordHandler::recordStoreWrite`, `recordStoreWritePreview` vs `curTouchSize`, `maxTouchSize`, `touches` | `RecordHandler.cpp`, `LODManager.cpp` | original puts RecordHandler before the LODManager data; our link order is `LODManager.cpp`(84) before `RecordHandler.cpp`(105) because LODManager's lowest `.text` is `0x9518d` vs RecordHandler `0xcba2d`. |
| 3 | ResourceTexture ctor | `ResourceTexture::ResourceTexture(String const&, float)` | `ResourceMaterial.cpp` (weak) | move to the ResourceTexture TU. |
| 4 | libzip `zip_err_*` | `_zip_err_str`, `_zip_err_type`, `_zip_nerr_str` | archive members | libzip archive-member order; same class as the already-known `zip_err_str`/`zip_open` issue. |
| 5 | archive tail | `_edata`, `_end`, `__bss_start`, `__cxa_begin_cleanup`, `__cxa_call_unexpected` vs `sha256_block_data_order_neon` | archive/link tail | OpenSSL/libc++ archive order; the original puts the OpenSSL NEON symbol after the linker-provided `_edata`/`_end` symbols, ours before. |

All the functions involved are already **byte-exact** (`MatrixRotateVector`,
`MatrixTransformVector`, `BSphere::Merge`, `recordStoreWrite`,
`recordStoreWritePreview`, `ResourceTexture` ctor). These gaps therefore do not
affect `byte_exact`; they only move `.dynsym` positions.

**Controlled experiment (regression, reverted).** Adding a
`RecordHandler.cpp.obj` -> `LODManager.cpp.obj` swap to
`tools/gen_link_order.py` (so RecordHandler links before LODManager):

```
before: byte 4171  linked 4495  dynsym ratio 0.996648, 16 blocks, 5352 matched
after:  byte 3084  linked 4495  dynsym ratio 0.954749, 72 blocks, 5127 matched
```

The link order is tightly coupled to `.rodata`/`.data`/GOT placement; the `.text`
address heuristic is better overall. Reverted.

## 5. Proven vs hypothesis

**Proven (command-backed):**

* Baseline gate is byte 4171 / linked 4495 (not 4115), lint `CLEAN`, drift 0.
* The `.plt` diff is exactly 14 lines and splits into the FileRead pair (4 lines)
  and the `LevelScript::process` group (10 lines).
* PLT order == input-object reloc-scan order == object `.text.*` section order;
  the section-ordering file does not change it. Verified by the first-reference
  scan reproducing our PLT order and by moving `getBoundingVolume` (PLT diff
  14 -> 25).
* `Level::getBoundingVolume` is emitted immediately after
  `Level::createStaticObject` in our object (section indices 159 -> 162) because
  `createStaticObject` calls it; the original emits it after `createRadioMessages`
  (original `.text` `0xc3e49` after `createRadioMessages` `0xc0de5`; source dump
  positions 36 -> 39; original PLT 1953-1956 -> 1957-1958).
* `getBoundingVolume`, `createRadioMessages`, `PlayerStation` C1/C2 differ *only*
  in the FileRead PLT offsets (disassembly diff).
* All five group-2 symbols are first referenced in `LevelScript::process`, and the
  original PLT order differs from our call-offset order at the four points listed
  in §3.2.
* The `.dynsym` diff is 14 gaps / 5 issues; all involved functions are byte-exact.
* Moving `getBoundingVolume` earlier is -2 byte_exact; swapping
  RecordHandler/LODManager is -1087 byte_exact.

**Hypothesis (not yet proven):**

* The original clang emitted out-of-line functions in **definition order**, while
  clang 7.0.2 in this tree emits at `min(definition, first-reference)`. The
  standalone reproduction (`A` calls `B`, `B` defined later -> IR order `A,B,C,D`)
  shows the lazy `Function` creation; no flag that restores definition order was
  found. A codegen-preserving way to defer `getBoundingVolume`'s section past
  `createRadioMessages` is the open problem for group 1.
* Fixing group 1 would be **+4 byte_exact** (4171 -> 4175). This is inferred from
  the four functions' disassembly diffs, not measured (the move is not yet found).
* Group 2 cannot be fixed without reconstructing `LevelScript::process`; the
  function is 54.6% and its call offsets are a layout artifact of the switch body.

## 6. Reproduction commands

```sh
cd /home/debian/GalaxyOnFire2/wt/w3418
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8

# PLT diff
OD=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OD -d -j .plt ../_work/bins/android_2.0.16_libgof2hdaa.so | grep -o "<[^>]*@plt>:" | sed 's/[<>:]//g' > /tmp/plt_o.txt
$OD -d -j .plt cmake-build-match/libgof2hdaa.so        | grep -o "<[^>]*@plt>:" | sed 's/[<>:]//g' > /tmp/plt_u.txt
diff /tmp/plt_o.txt /tmp/plt_u.txt

# first reference of a symbol (object order = link_order.rsp, section order = objdump -r)
python3 /tmp/w3418/firstref.py <mangled-symbol>...
```

The `dynsym_dump.py` / `firstref.py` / `firstref_all.py` scripts used for the
tables are self-contained in the commands above; the key logic is the ELF32
`.dynsym` parser from `dynsym_order_audit.md` and the `objdump -r` scan in
`link_order.rsp` order.
