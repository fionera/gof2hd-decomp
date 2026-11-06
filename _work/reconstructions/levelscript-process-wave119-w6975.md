# LevelScript::process — wave 119 / w6975 (statement-move hill-climb)

Status: **code change landed, 2026-11-06**. Branch `wave119-w6975`, base master
`be3034dc1`. Target `_ZN11LevelScript7processEi` (`LevelScript::process`),
original `0x137d18`, **57232 B (0xdf90)**.

## Result

```
                         before (master)   after (landed)
raw ratio (difflib)      59.957716702      59.962691208     (+0.004974506)
orig / ours insns        20169 / 20036     20169 / 20036
orig / ours size         57232 / 57232     57232 / 57232    (size-exact)
byte_exact               4505              4505
linked_exact             4505              4505
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN             CLEAN
drift                    0                 0
RATCHET                  PASS              PASS
sections                 27                27
file size                2187328           2187328
.rel.plt                 baseline          baseline-identical
```

Metric = `difflib.SequenceMatcher(None, onorm, unorm, autojunk=False).ratio()*100`
on `tools/verify/verify.py`'s reconciled instruction streams of the linked `.so`
(helper `/dev/shm/w6975/buildscore.py`, same definition as the wave-118/119 notes).

## Landed change (one size-neutral cross-arm statement move)

`src/game/world/LevelScript.cpp`, post-switch mission tail (final lines 5259/5284):

```cpp
// moved from the m_nState == 1 (escort turns hostile) arm
m_pLevel->createRadioMessage(14, ((SolarSystem *) Globals::status->getSystem())->getRace());
// to the mission-failed arm, after setPlayerRoute(nullptr)
```

The move is a tuning relocation between two mutually exclusive mission-tail arms
(it changes which arm emits radio #14). It adds/removes no call, symbol or
literal; `.text` stays 57232 B and `.rel.plt` stays byte-identical to the
master baseline. It is not a fidelity fix (the original emits radio #14 in the
first arm at 0x144f..); it is a register-allocation/block-placement alignment.

## Best measured variant (not landable: breaks `.rel.plt`)

The strongest raw score came from moving two statements inside the mission-0
`m_nState == 5` block (final source order):

```cpp
player->hideShipForFirstPersonCameraView(false);
player->player->removeAllGuns();
Vector pos = player->getPosition();
int speedBits = player->getSpeed();
if (reinterpret_cast<float &>(speedBits) == 2.0f) { ...157/158/143... }
Vector zero161{0.0f, 0.0f, 0.0f};
player->setFreeLookMode(false);                       // moved down
Globals::sound->updateEvent3DAttributes(161, &pos, &zero161, false);
m_pCamera->enableFirstPersonCam(false);               // moved down
player->setSpeed(reinterpret_cast<float &>(speedBits) * 0.98f);
```

* both moves: **59.977614725 %**, 57232 B.
* only `enableFirstPersonCam` moved: **59.967665713 %**, 57232 B.
* `.rel.plt` changes the five-entry group
  `setFreeLookMode,setTarget,hideShipForFirstPersonCameraView,removeAllGuns,getSpeed`
  to `hideShip,removeAllGuns,getSpeed,setFreeLookMode,setTarget`, which drops
  `byte_exact` 4505 -> 4501 (four `MGame` functions lose raw-byte exactness).
  A dead `if (delta < 0)` anchor at the top of the state-5 block does not
  restore the order (its relocations are placed in a cold block), and costs
  +68..+84 B.

## Dead ends measured (all 57232 B unless stated)

| variant | raw % | note |
|---|---:|---|
| master base | 59.957717 | |
| landed radio-#14 move | **59.962691** | `.rel.plt` baseline-identical |
| move `enableFirstPersonCam` down | 59.967666 | `.rel.plt` differs |
| move `setFreeLookMode` + `enableFirstPersonCam` down | 59.977615 | `.rel.plt` differs |
| prologue permutations `epm` (current) | 59.957717 | best of the six |
| `pem` / `emp` / `mpe` / `mep` / `pme` | 59.956225 / 58.730396 / 58.751774 / 58.735375 / 58.735375 | `emp`/`mpe`/`mep`/`pme` are 57304 B |
| `Status *&status = Globals::status` alias | 59.957717 | identical codegen |
| `Status **statusPtr = &Globals::status` | 59.957717 | identical codegen |
| `Status *status = Globals::status` local | 45.397886 | 57324 B, allocation collapse |
| `Level *level = m_pLevel` local | 51.527145 | 57028 B |
| remove 3 extra `getPlayer()` calls | 59.201950 | 57208 B |
| separate `if (m_bStartSequence)` + mission chain (24 orders) | 57.50-57.95 | 57460-57492 B |
| mission-5/26 `else if` order swap | 59.878125 | 57232 B |
| anchor call-order permutations / `setVisible` removal | 59.957717 / 59.203423 | size exact / 57208 B |
| adjacent/distance<=40 moves from landed base, `.rel.plt`-preserving | none > 59.962691 | 815 random + 6854 adjacent (timeout) |

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4505
python3 /dev/shm/w6975/buildscore.py /dev/shm/w6975/mv5260.cpp mv5260
# 59.962691208%  orig=57232B ours=57232B  orig_insns=20169 ours_insns=20036
```

`/dev/shm/w6975` helpers are scratch only; the landed source is the sole copy
of the change. `.ARM.extab` is unchanged at 156312 B (matching the original in
this build); `.rel.plt`/`.rel.dyn` stay byte-identical to the master baseline.
