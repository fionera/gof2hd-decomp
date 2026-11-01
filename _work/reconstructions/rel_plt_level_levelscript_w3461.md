# `.rel.plt` Level / LevelScript order (w3461)

Status: research note (2026-11-01). Worktree `wt/w3461`, branch `wave87-w3461`,
based on master `bd1a1bb7`. Original: `../_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`). No source or flag change was committed:
no controlled experiment was a strict improvement.

## TL;DR

At this tree the measured baseline is **byte_exact 4265 / linked_exact 4496**
(the task brief's 4182/4495 is stale), with `lint CLEAN`, `drift 0`, all ELF
section sizes/bases exact.

`.rel.plt` has 3674 entries in both files. All 3674 `r_offset`s and all relocation
types are identical. The remaining differences are:

| class | count | cause |
|---|---:|---|
| same name, different `r_info` symbol index | **181** | `.dynsym` insertion/hash order |
| name-order difference | **29** in two contiguous groups | first-reference order in `Level.cpp` (8) and `LevelScript.cpp` (21) |
| total `r_info` bytes differing | **210** | 181 + 29 |

The two name-order groups are exactly the ones already identified in
`data_rel_dyn_plt_finish_w3456.md`, but the current tree has 4265 byte-exact and
210 index differences (the older note measured 4263 and 134 index differences).

* **Group A (8 slots, `Level.cpp`)** is caused by clang emitting
  `Level::getBoundingVolume`'s body immediately after `Level::createStaticObject`
  (its first caller), while the original's `getBoundingVolume` body is scanned
  after `Level::createRadioMessages`. The original PLT order is therefore
  `createStaticObject` -> `createShip` -> `createRadioMessages` ->
  `getBoundingVolume`; ours is `createStaticObject` -> `getBoundingVolume` ->
  `createShip` -> `createRadioMessages`.
* **Group B (21 slots, `LevelScript.cpp`)** is entirely the first-reference order
  inside `LevelScript::process(int)`. All 21 symbols have their first relocation
  in that one section. The function is still far from reconstructed
  (call-sequence difflib ratio 0.6795; 2356 original calls vs 2353 ours), so a
  PLT-only reorder is not safe.

## 1. Baseline and reproduction

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8
```

Measured:

```
verify: byte 4265  linked 4496  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓
drift: 0 field(s) across 0 class(es)
```

`.rel.plt` section: `0x5751c / 0x72d0` in both files. `.dynsym` is
5578 entries in both; **5241/5578 are at the same index** at this baseline
(the 181 same-name index differences are downstream of that order).

### `.rel.plt` extraction

```python
import subprocess, re, difflib

def rel(path):
    out = subprocess.check_output(['readelf', '-rW', path], text=True)
    rows = []
    for line in out.splitlines():
        m = re.match(r'^([0-9a-f]+)\s+([0-9a-f]+)\s+R_ARM_JUMP_SLOT\s+([0-9a-f]+)\s*(.*)$', line)
        if m:
            rows.append((int(m.group(1), 16), int(m.group(2), 16),
                         int(m.group(3), 16), m.group(4).strip()))
    return rows

o = rel('../_work/bins/android_2.0.16_libgof2hdaa.so')
u = rel('cmake-build-match/libgof2hdaa.so')
print(len(o), len(u))
print('same offset', sum(a[0] == b[0] for a, b in zip(o, u)))
print('same info',   sum(a[1] == b[1] for a, b in zip(o, u)))
print('same name',   sum(a[3] == b[3] for a, b in zip(o, u)))
print('name diff',   sum(a[3] != b[3] for a, b in zip(o, u)))
print('same-name/info-diff',
      sum(a[3] == b[3] and a[1] != b[1] for a, b in zip(o, u)))
```

Outputs:

```
3674 3674
same offset 3674
same info 3464
same name 3645
name diff 29
same-name/info-diff 181
```

The name-order groups are printed by:

```python
on = [x[3] for x in o]
un = [x[3] for x in u]
sm = difflib.SequenceMatcher(None, on, un, autojunk=False)
for tag, i1, i2, j1, j2 in sm.get_opcodes():
    if tag != 'equal':
        print(tag, i1, i2, j1, j2)
```

## 2. Group A — 8 slots, `Level.cpp`

### Observed order

```
idx  original                                      ours
1943 PlayerJunk::PlayerJunk                       PlayerJunk::PlayerJunk
1944 PlayerTurret::PlayerTurret                   PlayerTurret::PlayerTurret
1945 PlayerFixedObject::setWreckedMeshId          PlayerFixedObject::setWreckedMeshId
1946 Level::getBoundingVolume                     Level::getBoundingVolume
1947 PlayerFixedObject::setBV                     PlayerFixedObject::setBV
1948 BoundingAAB::BoundingAAB                     BoundingAAB::BoundingAAB
1949 AEGeometry::setLodChildTransform             AEGeometry::setLodChildTransform
1950 Player::setEmpData                           FileRead::loadStaticCollision
1951 PlayerFighter::PlayerFighter                 FileRead::loadStationCollision
1952 Array<RadioMessage>::Array                    Player::setEmpData
1953 ArraySetLength<RadioMessage>                  PlayerFighter::PlayerFighter
1954 RadioMessage::RadioMessage (4 args)           Array<RadioMessage>::Array
1955 RadioMessage::RadioMessage (5 args)           ArraySetLength<RadioMessage>
1956 FileRead::loadStaticCollision                 RadioMessage::RadioMessage (4 args)
1957 FileRead::loadStationCollision                RadioMessage::RadioMessage (5 args)
```

The original `Level::getBoundingVolume` is at `0xc3e49`, after
`Level::createRadioMessages` at `0xc0de5`. `Level::createStaticObject`
(`0xbe6c0`) calls it directly at `0xbebee`, `0xbee36`, `0xbefce`,
`0xbf112`, `0xbfdaa`:

```sh
OD=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OD -d --start-address=0xbe6c0 --stop-address=0xc0214 \
  ../_work/bins/android_2.0.16_libgof2hdaa.so | grep getBoundingVolume
```

### Why our PLT order differs

`Level.cpp.obj` section order (with `-ffunction-sections`) is:

```
[266] .text._ZN5Level18createStaticObjectEP8Waypointib
[272] .text._ZN5Level17getBoundingVolumeEiP10AEGeometry
[290] .text._ZN5Level10createShipEiiiP8Waypointbb
[319] .text._ZN5Level19createRadioMessagesEi
```

The final `.text` order is correct because the section-ordering file puts
`getBoundingVolume` after `createRadioMessages`:

```
2409 .text._ZN5Level18createStaticObjectEP8Waypointib
2411 .text._ZN5Level10createShipEiiiP8Waypointbb
2414 .text._ZN5Level19createRadioMessagesEi
2417 .text._ZN5Level17getBoundingVolumeEiP10AEGeometry
```

But gold scans relocations in **input-object section order**, not final `.text`
order. Therefore the two `FileRead` relocations in `getBoundingVolume` are
scanned before `createShip`/`createRadioMessages` in our link, while in the
original they are scanned after `createRadioMessages`.

This is the same conclusion as `plt_dynsym_last14.md` and
`data_rel_dyn_plt_finish_w3456.md`; the new evidence here is that the original
body placement is not reachable by moving the definition or by the tested
compile flags.

### Controlled experiments

All full-gate experiments were run with:

```sh
cmake --build cmake-build-match --target check -j8
```

Full-gate experiments:

| experiment | byte | linked | result |
|---|---:|---:|---|
| baseline | **4265** | 4496 | PASS |
| move `Level::getBoundingVolume` definition before `createStaticObject` | **4263** | 4496 | FAIL -2 |
| split `getBoundingVolume` into `LevelBoundingVolume.cpp` | **2771** | 4495 | FAIL -1494 |

Compile-only experiments (object section order inspected; no link):

| experiment | `Level.cpp.obj` section order |
|---|---|
| move `getBoundingVolume` definition to end of file | GV 46, CR 54 (unchanged) |
| move `createStaticObjects` definition before `createStaticObject` / after `createRadioMessages` / end | GV 46, CR 54 (unchanged) |
| move `createStaticObject` definition after `createRadioMessages` | GV 46, CR 54 (unchanged) |
| compile `Level.cpp` with `-fno-function-sections` | `createStaticObject, getBoundingVolume, createShip, createRoute, setPlayerRoute, createRadioMessages` (GV still before CR) |

The move-before experiment is the most informative: it moves the `FileRead`
pair to 1943/1944 (too early) and also moves the `Level::getBoundingVolume`
PLT slot from 1946 to 1949. It is not a partial fix; it reorders a larger block.

The separate-TU split proves the mechanism: moving the body to a later object
moves the `FileRead` pair to 1986/1987 (too late), but adds one TU marker and
changes `.ARM.exidx` from `0x6f70` to `0x6f78` and `.rodata` from `0x1c9e10` to
`0x1c9e20`, so the gate collapses to 2771/4495. It is not a candidate fix
without also reproducing the original TU-boundary/marker layout.

Compile-only flag sweep (each variant compiled `Level.cpp` and checked whether
`getBoundingVolume`'s section came after `createRadioMessages`): `-fno-inline`,
`-fno-inline-functions`, `-fno-inline-functions-called-once`,
`-fno-function-sections`, `-fno-vectorize`, `-fno-slp-vectorize`,
`-fno-threadsafe-statics`, `-fno-use-cxa-atexit`, `-fno-sized-deallocation`,
`-fno-delete-null-pointer-checks`, `-fno-strict-overflow`, `-fwrapv`,
`-fno-strict-aliasing`, `-fno-common`, `-fno-builtin`, `-fno-rtti`,
`-fno-unwind-tables`, `-fno-asynchronous-unwind-tables`,
`-fno-stack-protector`, `-fstack-protector`, `-fstack-protector-all`, `-Os`,
`-O2`, `-O1`, `-O3`, `-fno-omit-frame-pointer`, `-mno-unaligned-access`.
**None** put `getBoundingVolume` after `createRadioMessages`.

The compile-only section-order check is:

```python
# after compiling a variant to /tmp/Level.o
import re, subprocess
txt = subprocess.check_output(['readelf', '-SW', '/tmp/Level.o'], text=True)
names = [m.group(1) for m in
         (re.search(r'\[\s*\d+\]\s+(\.text\.\S+)\s+PROGBITS', l) for l in txt.splitlines())
         if m]
gv = names.index('.text._ZN5Level17getBoundingVolumeEiP10AEGeometry')
cr = names.index('.text._ZN5Level19createRadioMessagesEi')
print(gv, cr, 'GV after CR' if gv > cr else 'GV before CR')
```

## 3. Group B — 21 slots, `LevelScript.cpp`

All 21 symbols have their first `R_ARM_THM_CALL` relocation in
`.text._ZN11LevelScript7processEi`. The original and current sequences are:

```
orig 2816..2836:
  Standing::setStanding
  TargetFollowCamera::setRotationAroundTarget
  Player::setShootingEnabled
  Mission::setType
  AEGeometry::isVisible
  PlayerEgo::autoTurretIsEnabled
  PlayerEgo::setAutoTurret
  PlayerWormHole::open
  Level::flashScreen
  StarSystem::scaleSunDuringSupernovaIntro
  StarSystem::updateSupernova
  PlayerEgo::isInWormhole
  PlayerWormHole::freeMissionLock
  StarSystem::switchSunForSupernovaIntro
  PlayerEgo::setDockingState
  StarSystem::switchPlanetForIntro
  Level::switchSkyboxForIntro
  ParticleSystemManager::reset
  PlayerEgo::startSmokeEmission
  PlayerStation::setVisible
  PlayerEgo::deleteHackingGame

ours 2816..2836:
  TargetFollowCamera::setRotationAroundTarget
  Player::setShootingEnabled
  Mission::setType
  AEGeometry::isVisible
  PlayerWormHole::open
  Level::flashScreen
  StarSystem::scaleSunDuringSupernovaIntro
  StarSystem::updateSupernova
  PlayerEgo::isInWormhole
  PlayerWormHole::freeMissionLock
  Standing::setStanding
  PlayerEgo::autoTurretIsEnabled
  PlayerEgo::setAutoTurret
  PlayerEgo::setDockingState
  PlayerEgo::deleteHackingGame
  StarSystem::switchSunForSupernovaIntro
  StarSystem::switchPlanetForIntro
  Level::switchSkyboxForIntro
  ParticleSystemManager::reset
  PlayerEgo::startSmokeEmission
  PlayerStation::setVisible
```

Our first-occurrence relocation offsets in `LevelScript::process`:

| symbol | first offset in our `.text.LevelScript::process` |
|---|---:|
| `TargetFollowCamera::setRotationAroundTarget` | `0x1eb2` |
| `Player::setShootingEnabled` | `0x2072` |
| `Mission::setType` | `0x2130` |
| `AEGeometry::isVisible` | `0x240a` |
| `PlayerWormHole::open` | `0x27fc` |
| `Level::flashScreen` | `0x28bc` |
| `StarSystem::scaleSunDuringSupernovaIntro` | `0x33f2` |
| `StarSystem::updateSupernova` | `0x3400` |
| `PlayerEgo::isInWormhole` | `0x3dfa` |
| `PlayerWormHole::freeMissionLock` | `0x3ee8` |
| `Standing::setStanding` | `0x4042` |
| `PlayerEgo::autoTurretIsEnabled` | `0x472c` |
| `PlayerEgo::setAutoTurret` | `0x473e` |
| `PlayerEgo::setDockingState` | `0x4cc2` |
| `PlayerEgo::deleteHackingGame` | `0x5436` |
| `StarSystem::switchSunForSupernovaIntro` | `0x5662` |
| `StarSystem::switchPlanetForIntro` | `0x61b2` |
| `Level::switchSkyboxForIntro` | `0x61ba` |
| `ParticleSystemManager::reset` | `0x61d8` |
| `PlayerEgo::startSmokeEmission` | `0x6464` |
| `PlayerStation::setVisible` | `0x677e` |

The call sequences are far from equal:

```python
# original process: 0x137d18..0x145ca8
# ours: .text._ZN11LevelScript7processEi relocations
# (extract bl/blx names from objdump, then:)
difflib.SequenceMatcher(None, original_calls, our_calls, autojunk=False)
```

Measured: **2356 original calls**, **2353 ours**, ratio **0.6795**, 76 matching
blocks. The first divergence is already at 0-based call index 6 (the 7th call). This is consistent with the
current `LevelScript::process` fuzzy score (54.4% in `report.json`) and means group B is a
`LevelScript::process` reconstruction task, not a link-order knob.

## 4. Proven vs hypothesis

**Proven by commands above:**

1. Baseline is **4265/4496**, lint `CLEAN`, drift 0; all section sizes/bases
   exact.
2. `.rel.plt` has 3674 entries, identical offsets and types; 181
   same-name/different-index entries and 29 name-order entries in two groups
   (8 `Level.cpp`, 21 `LevelScript.cpp`).
3. Group A is caused by the object-section order of
   `Level::getBoundingVolume`: our clang emits its body immediately after
   `createStaticObject`, before `createShip`/`createRadioMessages`; the
   original body is after `createRadioMessages`.
4. Moving the `getBoundingVolume` definition before `createStaticObject` is a
   regression: byte 4265 -> 4263, and the `FileRead` pair moves to 1943/1944.
5. `-fno-function-sections`, `-fno-inline`, `-O1/2/3/Os`, and the other listed
   flags do not move `getBoundingVolume` after `createRadioMessages`.
6. Splitting `getBoundingVolume` into its own TU moves the `FileRead` pair to
   1986/1987 but costs 1494 byte-exact (2771/4495) through the extra TU marker
   and `.ARM.exidx`/`.rodata` shifts.
7. Group B is entirely inside `LevelScript::process`; the call sequences differ
   substantially (2356 vs 2353 calls, ratio 0.6795, 76 matching blocks).

**Hypothesis / not yet proven:**

1. The original clang emitted `getBoundingVolume` at its definition/source
   position rather than at first use. The original address order and PLT order
   prove the body was scanned after `createRadioMessages`, but the exact
   source/compiler reason is not reproduced by any tested flag or source move.
2. Group B can be fixed by reconstructing `LevelScript::process` to the
   original call order. A PLT-only source reorder is not safe because the
   function body is still 54.4% fuzzy and the first divergence is at 0-based call index 6.
3. The 181 same-name `.rel.plt` index differences disappear only when the
   `.dynsym` insertion order matches; that is a separate TU/link-order problem
   (`dynsym_relcount.md`).

## 5. No committed source/flag change

No experiment was a strict improvement. The only committed artifact is this
note. The worktree was left clean after reverting all experiments.
