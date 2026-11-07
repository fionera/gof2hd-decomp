# LevelScript::process — wave 135 / w7766 (getPlayer receiver fixes + size-exact -80 companion)

Status: **code change landed, 2026-11-07**. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**. Owned TU:
`src/game/world/LevelScript.cpp`. Base master `bbffef482` (wave-134).

## Result

```
                         before (master)   after (landed)
official verify pct      58.9              56.6
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
process .text diffbytes  54465             55225            (+760)
whole .text diffbytes    85022             85888            (+866)
.ARM.extab size          156312            156312           (unchanged)
.rodata/.data/.bss/.got  exact             exact
.rel.plt                 exact             exact
byte_exact               4506              4506
linked_exact             4506              4506
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
RATCHET                  PASS              PASS
```

The fuzzy pct dips 2.3 and the function's differing-byte count grows by 760 B. This is the
price of the four semantically-correct `getPlayer` receiver fixes: they re-roll clang's
`MachineBlockPlacement` / register colouring for the whole 57 KB body (w7749 measured the same
effect: 3 fixes alone = 56.7 %). The hard gate (byte_exact / linked_exact / section sizes /
`.rel.plt`) is unchanged, so the change is admissible under the no-degradation policy.

## 1. The `getPlayer` receiver bugs (all four confirmed against objdump)

The original caches `player` in `[sp,#84]` in the prologue and reuses it; our source re-fetched
it with `m_pLevel->getPlayer()` at four sites. Each fix is a pure receiver swap:

| source line | before | after | original evidence |
|---|---|---|---|
| 947 | `m_pLevel->getPlayer()->rotate(0,angle,angle)` | `player->rotate(...)` | `0x137da6 ldr r0,[sp,#84]` |
| 1786 | `m_pLevel->getPlayer()->geometry->getPosition()` | `player->geometry->getPosition()` | preceded by `setTarget(player->geometry)` |
| 3243 | `if (m_pLevel->getPlayer()->isInRocketControl())` | `if (player->isInRocketControl())` | after `resetCamera(m_pLevel)` |
| 4320 | `if (m_pLevel->getPlayer()->isInRocketControl())` | `if (player->isInRocketControl())` | case-40 state 1 |

The fourth site is the w7745 post-merge lead; it is the same bug and is landed here.

## 2. The size / `.rel.plt` budget (the hard part)

The four fixes alone measure **57344 B** (+112). The `delta < 0` first-reference anchor is
**exactly -80 B** when trimmed to its single `PlayerStation::setVisible(false)` statement, and
that trim keeps `.rel.plt` **byte-identical** (readelf `.rel.plt` symbol list, 3674 entries,
`identical True`). That gives 57256 B with the anchor trim alone.

The remaining -24 B had to come from faithful code, not from more anchor surgery. The
`delta < 0` block is layout-critical: removing it whole (-88) swaps `.rel.plt` at index
2816 (`PlayerStation::setVisible` vs `PlayerEgo::deleteHackingGame`).

Measured single-statement costs **inside** the `delta < 0` block (on the 57228 base):

| added statement | Δ size |
|---|---:|
| `switchSkyboxForIntro()` | 0 |
| `player->PlayEngineSound()` | 0 |
| `player->StopEngineSound()` | 0 |
| `switchPlanetForIntro()` | +8 |
| `particleRenderBoolPtr->reset()` | +8 |
| `particleEmitBoolPtr->reset()` | +8 |
| any two of the above | +16 |
| three | +24 |
| four | +32 |

So the block has 4 B of alignment slack; it cannot supply a +4.

## 3. Faithful cleanups measured (all `.rel.plt`-safe)

| edit | Δ size |
|---|---:|
| case 4: hoist `StarSystem *stars4 = m_pLevel->getStarSystem()` (2 calls) | -8 |
| case 24 loop 2 (`enemies->count` / `(*enemies)[i]`) | -8 |
| case 24 loop 1 header only | +8 |
| case 24 loop 1 header+body (i.e. un-cache) | **+36** |
| case 135 `enemies->operator[](i)->setVisible(true)` | -8 |
| case 24: cache `Array<KIPlayer*> *landmarks = m_pLevel->getLandmarks()` (4 uses) | **-24** |
| case 24: cache landmarks, 3 uses only | -8 |
| `int routePoints[3] = {(int)camPos.x,(int)camPos.y,(int)camPos.z + 30000}` (drop the `(int)(…+30000.0f)` fold) | -8 |
| `m_pLevel->getPlayer()->resetGunDelay()` -> `player->resetGunDelay()` | 0, -8 or -16 depending on site |
| prologue `m_nTimeLimit = 0;` | +16 |
| prologue `field_0x4 = 0;` | +44 |
| prologue `m_nScriptTimerB = 0;` | +96 |
| mission-0 state-0 `m_nTimeLimit = 0;` | +8 |
| mission-0 `KIPlayer *enemy = (*enemies)[i]` cache removed | +60 |

The landed combination is:

```
4 getPlayer fixes                       +112
delta<0 anchor trimmed to 1 statement    -88   (measured on this base)
case 4 hoist                              -8
case 24 loop 2 cache                      -8
case 24 landmarks cache                  -24
case 135 cache                            -8
resetGunDelay site 1 (line 3048)         -16
                                        -----
net                                       -4   -> 57228
```

and then the case-24 **loop 1 un-cache** (+36) plus the same `resetGunDelay` site (-16) plus the
landmarks cache (-24) lands exactly on **57232**:

```
57228 (4 fixes + trims above) + 36 (loop-1 un-cache) - 16 (resetGunDelay) - 24 (landmarks) = 57232
```

The loop-1 un-cache is also a faithfulness win: `Level::getEnemies` call count goes 30 -> 32
(original 34) instead of staying at 30. The landmarks cache takes `Level::getLandmarks`
17 -> 16 (original 15).

## 4. Why not a better pct?

Every `≡ 4 (mod 8)` lever found is a block-placement re-roll of the same kind, so the 57232 B
surface is narrow:

* `delta<0` trim to 1 statement: -80/-88, `.rel.plt` exact, pct 57.6 (with the case-24/135
  caches) — but 4 B short of the target.
* case-24 loop-1 un-cache: +36 (the only other `≡ 4` lever) — this is what closes the 4 B.
* w7749's "3 fixes + remove `delta<0` entirely" gave 55272 diff bytes (56.7); this landing is
  55225 (56.6), i.e. marginally better than the known result but still worse than the
  54465-byte baseline.

No combination of the measured levers produced a 57232 B body with a smaller process diff than
55225. The remaining ~760 B is the getPlayer-fix block re-roll, not the anchor.

## 5. Call-count delta vs the original (landed build)

```
  50   51  AEGeometry::getPosition
  23   24  AEGeometry::setPosition(Vector const&)
  37   36  PaintCanvas::TransformGetTransform
   9   10  ApplicationManager::SetCurrentApplicationModule
  33   34  AEMath::Vector::operator=
   1    2  PlayerStation::setVisible
  81   82  TargetFollowCamera::setTarget
  34   32  Level::getEnemies
  15   16  Level::getLandmarks
  23   25  Level::getPlayer
   1    2  Status::getStanding
   1    2  Standing::setStanding
  41   42  PlayerEgo::getPosition
   2    3  PlayerEgo::setAutoTurret
   1    2  PlayerEgo::autoTurretIsEnabled
```

The `getStanding`/`setStanding`/`setAutoTurret`/`autoTurretIsEnabled`/`PlayerStation::setVisible`
extras are the three PLT-order anchors (standing, auto-turret, `delta<0`). The remaining
`+1` calls (`AEGeometry::getPosition`, `AEGeometry::setPosition`, `SetCurrentApplicationModule`,
`Vector::operator=`, `TargetFollowCamera::setTarget`, `PlayerEgo::getPosition`) are a real
duplicated statement block still to be found — they are the next -40..-48 B lead.

## 6. Negative results (do not re-run)

* Adding a store (`m_nState`, `m_nTimeLimit`, `field_0x4`, `field_0x10`, `field_0x8c`,
  `field_0xa9`, `m_bRenderParticles`, `m_bStartSequence`, `m_bCinematicBreak`) inside the
  `delta < 0` block is **free** (0 B) — the block has 4 B of slack; two stores cost +8.
* Adding a store immediately before the block is also free.
* Prologue stores are *very* expensive (+16..+96) because they re-colour the whole body.
* Changing float literals (`6.2831855f`, `1.52587890625e-05f`) does not change the size — the
  literal pool has free slots.
* `m_pLevel->getEnemies()->operator[](i)` -> `(*m_pLevel->getEnemies())[i]` and
  `(*enemies)[i]` are all byte-identical to `enemies->operator[](i)`.
* `for (unsigned int …)` vs `for (int …)` in the case-24 loops is a no-op.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
cmake --build cmake-build-match --target drift -j6      # 0 fields
```

Scratch helpers in `/dev/shm/w7766/` (not tracked): `m.sh` (build+verify+pct+`.rel.plt`),
`sz.sh` (size only), `try.py` / `trysz.py` (edit harness), `relplt.py` (readelf `.rel.plt`
comparator), `annot.py` (PLT-annotated objdump), `align.py` / `align2.py` (normalised
instruction diff). `LevelScript.57228.cpp` and `LevelScript.57232.cpp` are the two candidate
snapshots.

## Open leads

1. Find the duplicated statement block behind the six `+1` calls listed in §5 — removing it is
   ~-40..-48 B and would let the `delta < 0` anchor be restored (better layout, higher pct).
2. The `Level::getPlayer` call count is 25 vs 23 after the landing even though the source has
   only the four receiver fixes (the 57228 B snapshot is 23/23). The two extra PLT calls are a
   clang block-duplication artifact of the new layout, not a source-level call. Locating the
   duplicated block would recover both the count and some of the 760 diff bytes.

## Post-merge note (wave 135 coordinator)

The source change was **not landed**: the four `getPlayer` receiver fixes cost
+760 diff bytes on the wave-135 base (54,465 -> 55,225) even though they are
semantically correct.  The note is kept as the semantic lead; a future pass
must pair them with a block-placement companion before landing.
