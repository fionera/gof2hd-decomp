# Anchor audit — wave 92 (w4268)

Status: code changes + analysis, 2026-11-03. Branch `wave92-w4268`, base
`b379d00d`, HEAD at audit start `bbf4480b` (`Level::createScene pass 93`).

This pass audits every remaining source-level codegen anchor in `src/`:
`volatile`, `__builtin_expect`, `__builtin_prefetch`, `__builtin_assume`,
`__builtin_annotation`, `__builtin_trap`, the atomic fences, and padding locals.
For each site the anchor was removed in isolation, the TU rebuilt, and the
official verifier run.  A site is *removable* only when the function's body size
and bytes are unchanged; sites that change only the raw fuzzy score are recorded
with the measured delta.  `__builtin_prefetch` no longer occurs anywhere in
`src/`.

All addresses below are the real ELF addresses (Ghidra address − 0x10000).
Scratch scripts and reports live in `/dev/shm/w4268`.

## Gate

```
                         before            after
byte_exact               4499              4499
linked_exact             4499              4499
size-exact               4524/4524         4524/4524
stub_zero_size           0                 0
missing/wrong_type/extra 0/0/0             0/0/0
data_exact               843/843           843/843
imports                  207               207
.text                    0x1337c8          0x1337c8
.rodata                  0x41b67           0x41b67
.ARM.exidx               0x6f70            0x6f70
.ARM.extab               0x26298           0x26298
lint                     CLEAN             CLEAN
drift                    0                 0
RATCHET                  PASS              PASS
```

No per-function row changed (`report.json` diff: 0 rows).  The `.ARM.extab`
baseline measured in this worktree is `0x26298` (size-exact); the task brief's
`0x26290 (-8)` is stale for this HEAD.

## Removed anchors (proved no-ops)

Nine sites were removed (the table has eight rows: one row covers the two
identical `button->OnTouchEnd` expects).  Each removal leaves the target
function's size and bytes identical; the whole gate is unchanged.

| site | function | evidence |
|---|---|---|
| `StarMap.cpp` `__builtin_expect(starSystemRoot != 0 && stations != 0, 1)` | `StarMap::draw` | body 2644 B before/after, bytes identical |
| `Generator.cpp` `__builtin_expect(rnd->nextInt(5) == 0, 0)` | `Generator::getShipBuyList` | body 2244 B, bytes identical |
| `ModStation.cpp` `__builtin_expect(getCurrentCampaignMission() < 13, 0)` | `ModStation::OnKeyPress` | body 2516 B, bytes identical |
| `MenuTouchWindow.cpp` two `__builtin_expect(button->OnTouchEnd(x, y), 1)` | `MenuTouchWindow::OnTouchEnd` | body 8132 B, bytes identical |
| `MenuTouchWindow.cpp` `__builtin_expect(choiceWindow->OnTouchEnd(...) == 0, 1)` | `MenuTouchWindow::OnTouchEnd` | body 8132 B, bytes identical |
| `Level.cpp` first `volatile` cast in `list = *(Array<KIPlayer *> *volatile *) &enemies` | `Level::createFighterTurrets` | body 272 B, bytes identical; the *loop-tail* reload is load-bearing (see below) |
| `HangarWindow.h` `volatile uint8_t dialogActive` → `uint8_t` | all `HangarWindow` methods | every row in `report.json` identical; `render` 9700 B, `OnTouchEnd` 5828 B, both byte-exact |
| `ModStation.cpp` `__atomic_signal_fence(__ATOMIC_SEQ_CST)` in ctor | `ModStation::ModStation` | body 1008 B; raw fuzzy `98.35164835164835` identical before/after |

The `Level::createFighterTurrets` case is split: the initialisation load can be
a plain member read, but the loop-increment reload must stay
`*(Array<KIPlayer *> *volatile *) &enemies`; making that one plain too changes
the 272-byte body (raw match 100.0 → 93.2).

## Kept anchors (binary-proven)

Every kept site now carries a same-line `// anchor: <function>; <evidence>`
comment.  Summary of the measurements:

| site | function | removal effect |
|---|---|---|
| `PlayerEgo.cpp` `expect(target == nullptr, 0)` | `PlayerEgo::update` | body 8852 → 8844 |
| `PlayerEgo.cpp` `expect(turretActive, 0)` | `PlayerEgo::update` | body 8852 → 8868 |
| `LevelScript.cpp` `volatile int pad = delta` | `LevelScript::process` | body 57232 → 57224 |
| `Level.cpp` loop-tail volatile reload | `Level::createFighterTurrets` | body bytes change (100.0 → 93.2) |
| `Level.cpp` `__builtin_trap()` in `catch (...)` | `Level::createShip` | body 2764 → 2772 |
| `StarMap.cpp` `expect(getStationEnumIndex(93) >= 0, 1)` | `StarMap::drawOnScreenInfo` | body 3488 → 3480 |
| `StarMap.cpp` `expect(i >= systemPositions->size(), 0)` | `StarMap::draw` | body 2644 → 2636 |
| `StarSystem.cpp` `*(AERandom *volatile *) &Globals::rnd` | `StarSystem::StarSystem` | body 2900 → 2892 |
| `Generator.cpp` `expect(pick == 10, 1)` | `Generator::createMission` | body 1996 → 1988; genuine source-shape hint (AGENTS.md), kept |
| `GameRecord.cpp` `Status *volatile *statusRef` | `GameRecord::load` | body bytes change (byte-exact currently) |
| `SpaceLounge.cpp` `expect(agent->isKnown(), 1)` | `SpaceLounge::drawLounge` | body 1948 → 1952 |
| `SpaceLounge.cpp` `__builtin_annotation(result, "sound")` | `SpaceLounge::getSpecificSoundForRace` | body 116 → 106 |
| `ModStation.cpp` two `*(const volatile int *)` table reads | `ModStation::ModStation` | raw match 98.35 → 85.9 / 97.8 |
| `ModStation.cpp` `Status *volatile *statusPtr` | `ModStation::checkHints` | body 2748 → 2756 |
| `ModStation.cpp` `volatile int *` `field_0xb4` read | `ModStation::OnRender2D` | body bytes change (byte-exact currently) |
| `TouchButton.cpp` `__builtin_assume(canvasTop != 0)` | `TouchButton::draw` | body 1376 → 1380 |
| `HangarWindow.cpp` `volatile int pad3` | `HangarWindow::render` | body 9700 → 9696 |
| `HangarWindow.cpp` `volatile int padS` | `HangarWindow::render` | body 9700 → 9696 |
| `HangarWindow.cpp` `volatile int pad0b` | `HangarWindow::render` | body 9700 unchanged; `.ARM.extab` 0x26298 → 0x2628c (see below) |
| `HangarWindow.cpp` `volatile float textYf` | `HangarWindow::render` | body 9700 → 9752 |
| `HangarWindow.cpp` `*(TouchButton **volatile *) &buttons->data_` | `HangarWindow::OnTouchEnd` | body 5828 → 5824 |
| `MenuTouchWindow.cpp` `*(volatile uint16_t *) &field_0x171` | `MenuTouchWindow` ctor | raw match 94.247 → 94.157 |
| `MenuTouchWindow.cpp` `*(volatile uint8_t *) &messageShowing` | `MenuTouchWindow::update` | body bytes change (byte-exact currently) |
| `MenuTouchWindow.cpp` `*(ChoiceWindow *volatile *) &choiceWindow` | `MenuTouchWindow::OnTouchEnd` | body 8132 → 8120 |
| `MenuTouchWindow.cpp` `expect(loadGame(...) == 0, 0)` | `MenuTouchWindow::OnTouchEnd` | raw match 97.9 → 97.2 |
| `String.cpp` `__builtin_assume(!reachedEnd || d == -1)` | `String::Compare` | body bytes change (byte-exact currently) |
| `Gun.cpp` `expect(enemyPosition.x - ... < r, 1)` | `Gun::calcCharacterCollision` | raw match 90.0 → 84.7 |
| `RocketGun.cpp` `expect(hasGeometry && delayActive, 1)` | `RocketGun::update` | raw match 86.9 → 84.6 |
| `RocketGun.cpp` `volatile int padx` | `RocketGun::update` | raw match 86.9 → 86.7 |
| `RocketGun.cpp` `volatile int unused` | `RocketGun::update` | raw match 86.9 → 85.8 |
| `Radar.cpp` three `__builtin_expect` sites | `Radar::draw` | body 9908 → 9612 / 9896 / 9912 |
| `BombGun.cpp` `*(Gun *volatile *) &this->gun` | `BombGun::update` | body bytes change (byte-exact currently) |
| `VirtualInput.cpp` `volatile int &hackingVisible` | `ActualizeMouseVisibilty` | body bytes change (byte-exact currently) |
| `VirtualInput.cpp` three `__builtin_expect` sites | `ActualizeMouseVisibilty` | body 592 → 540 / 588 / bytes change |
| `Status.cpp` `__atomic_signal_fence` in `setStation` | `Status::setStation` | body bytes change (byte-exact currently) |
| `Status.cpp` four `__atomic_signal_fence` in `resetGame` | `Status::resetGame` | body bytes change (byte-exact currently) |
| `FileInterfaceAndroid.cpp` fence in `Read` | `FileInterfaceAndroid::Read` | body 360 → 356 |
| `FileInterfaceAndroid.cpp` fence in `Write` | `FileInterfaceAndroid::Write` | body bytes change (byte-exact currently) |

## The `pad0b` / `.ARM.extab` trade-off

`HangarWindow::render` contains `volatile int pad0b = count;` inside the
credit-list scoped block.  Removing it is a strict fuzzy improvement:

```
raw fuzzy (autojunk=False)   78.78871300757055 -> 79.35590421139554
body size                    9700 -> 9700
```

but it shrinks `.ARM.extab` from `0x26298` (size-exact) to `0x2628c`
(−12 bytes).  The anchor is therefore kept: the audit treats the exact extab
size as part of the gate.  The same-line comment records both facts.

## PLT-order anchors in `LevelScript::process`

The three never-taken/relocated calls that force `.plt` slot order were audited by
removing each one and comparing `.plt` and `.rel.plt` byte-for-byte with the
original:

| anchor | `.plt`/`.rel.plt` without it | verdict |
|---|---|---|
| `if (m_nState == 0x7f) player->setRocketControl(nullptr, nullptr);` | identical (0 diff bytes) | no longer a PLT anchor; kept only as a **size anchor** (body 57232 → 57208, layout/byte_exact collapse) |
| `if (delta == 0x7d) ... setStanding(0, 100);` | `.rel.plt` differs by 26 bytes | still load-bearing for PLT order |
| `player->setAutoTurret(player->autoTurretIsEnabled());` | `.rel.plt` differs by 18 bytes | still load-bearing for PLT order |

The first call's comment was updated from "PLT-order anchor" to "Size anchor"; the
other two now record the measured `.rel.plt` delta.

## What remains

The remaining anchors are all binary-proven: every one changes body size, body
bytes, or a measured raw fuzzy score when removed.  They are documented inline
with the function name and the measured delta.  No `__builtin_prefetch` sites
remain.  The only anchor that is removable for fuzzy but retained for section
identity is `HangarWindow::render`'s `pad0b`.
