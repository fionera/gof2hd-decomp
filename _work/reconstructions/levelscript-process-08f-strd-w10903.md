# LevelScript::process — wave 163b / w10903: size-companion sweep for the 0.8f block

Follow-up to `levelscript-process-08f-plt-safe-w10903.md`.  Target
`_ZN11LevelScript7processEi` (`LevelScript::process`), original 57,232 B.
Base master `e51db2fc7`; the 0.8f block itself is committed as `39c034629`
(hoisted to the top of the mission-0x4e arm, `.rel.plt` byte-identical,
3x `0.8f` loads, size 57,264 = **+32 B**, gate RED).

## The +32 B is exactly the block, and the baseline is exactly size-neutral

Proof: removing the `break` at the end of `case 11` so the in-switch `if`
becomes reachable produces **size 57,232 (delta 0), raw 54,698, aligned
17,456, .rel.plt 0 diffs — but only 2x `0.8f` loads**.  clang constant-folds
the condition because `case 11` sets `m_nState = 12` immediately before the
`if`, so `m_nState >= 5 && m_nState <= 8` is statically false.  The emitted
code is byte-identical to the baseline.

Consequence: the baseline is size-exact *while missing the 46-byte block*.
It therefore carries +46 B of other code that the original does not have.
Restoring the block (+46) must be paid for by removing ~46 B elsewhere.

## Where the +46 B of extra code is: 8 extra PLT calls (= 32 B)

`blx`-to-`@plt` histogram, baseline vs original (committed tree is the same
set plus the block's own `translate`):

```
 +1 AEGeometry::moveForward(float)
 +1 AEGeometry::setPosition(Vector const&)
 +1 TargetFollowCamera::setPosition(float,float,float)
 +1 Level::getStarSystem()
 +1 Level::getPlayer()
 +1 Status::getStanding()
 +1 Standing::setStanding(int,int)
 +1 PlayerEgo::getPosition()
 +1 PlayerEgo::setAutoTurret(bool)
 +1 PlayerEgo::autoTurretIsEnabled()
 -1 LODManager::forceUpdate(int,bool)
 -1 TargetFollowCamera::setPosition(Vector const&)
```

Net +8 `blx` = **+32 B**, which is exactly the residual.  Two of the extra
pairs (`getStanding`/`setStanding` at `LevelScript.cpp:2022` and
`setAutoTurret(autoTurretIsEnabled())` at `LevelScript.cpp:1817`) are the
deliberate PLT-order anchors documented in the source; removing either
rotates `.rel.plt` (26 B / 18 B per the in-source comments).  The other six
are genuine source duplicates and are the remaining lever: removing all
eight would land the block at exactly 57,232.

## `strd` vs two `str.w` (measured, only -12 B available)

Three sites in `process` store a 64-bit zero as two `str.w` where the
original uses one `strd` (4 B each):

```
13b5fc  str.w r0,[fp,#144] / str.w r0,[fp,#148]   (mission-0x4e case 5)
13ffac  str.w r8,[fp,#144] / str.w r8,[fp,#148]
142d88  str.w r1,[fp,#144] / str.w r1,[fp,#148]
```

The original's `13b8d4` is `movs r0,#0; strd r0,r0,[sl,#144]` (4 B total).
`m_nScriptTimerA = 0;`, `= 0LL;`, `long long zz = 0;` all still emit the
two-`str.w` form.  Even fixing all three only buys -12 B, not -32.

## Placement sweep (all measured, `.rel.plt` diffs / size delta)

| shape | size delta | .rel.plt | 0.8f loads | raw | aligned |
|---|---:|---:|---:|---:|---:|
| baseline (block dead in switch) | 0 | 0 | 2 | 54,698 | 17,456 |
| `case 11` break removed (block folded) | 0 | 0 | 2 | 54,698 | 17,456 |
| hoisted, no `st78` | +40 | 0 | 3 | 55,416 | 17,456 |
| hoisted + `st78` (**committed**) | **+32** | **0** | **3** | **55,380** | **17,681** |
| hoisted + `st78`, `__builtin_expect(...,0)` | +44 | 0 | 3 | — | — |
| hoisted + `st78`, `__builtin_expect(...,1)` | +48 | 0 | 3 | — | — |
| after `if (m_nScriptTime>=7901)` | +48 | 0 | 3 | 55,512 | 17,462 |
| inside `if (m_nScriptTime>=7901)` | +48 | 0 | 3 | 55,421 | 17,727 |
| faithful post-switch | -12 | **30** | 3 | 55,382 | 19,938 |
| faithful post-switch + `st78` | -12 | 30 | 3 | 55,382 | 19,938 |

The faithful post-switch placement is the only shape *smaller* than the
original, but it rotates `.rel.plt` (30 slots: `RadioMessage::isTriggered`
moves after `moveForward`/`setFreeLookMode`/`getSpeed`, and mission-89's
state-4 `scaleSun`/`updateSupernova` move before `setRocketControl`).

## Status

`39c034629` is the best measured shape: 3x `0.8f` loads, `.rel.plt`
byte-identical, size 57,264 (+32).  It is **not landable as-is**: the +32 B
.text growth shifts every later function and the ratchet drops
`byte_exact` 4507 -> 2834.  Because the no-degradation gate requires section
sizes not to regress, the source change is reverted in this tree (back to
the 57,232 B baseline, `byte_exact` 4507, gate green); the block shape is
kept in the note and in `39c034629` for the coordinator.  Landing it needs
the -32 B companion from the six non-anchor duplicate PLT calls listed
above.
