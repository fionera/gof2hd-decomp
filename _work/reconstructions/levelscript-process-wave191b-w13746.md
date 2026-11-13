# LevelScript::process — wave 191b / w13746: mission-0/1, mission-89, mission-106 order

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13746`, branch
`wave191b-w13746`, base master `4c4d81cf7`.

Session start: raw **52,008**, aligned **20,733**, `.text` diffbytes 72,954,
`.ARM.extab` 564, `.rel.plt` md5 `289b2a069f8504d1e8a3919e43915e04`.
Session end: raw **54,170**, aligned **20,683**, `.text` diffbytes 75,116,
`.ARM.extab` 563, `.rel.plt` md5 unchanged (`289b2a06…`).

**Coordinator merge note (2026-11-13):** harvested onto `4d0cc4910`
(which already contained b44ab4d83's block move and w13748's case-67 fix).
Kept: the block-move revert, mission-0 s4 faithful order, mission-89
case-3 else order.  Dropped: mission-106 s4 (`3d508fa27`, previously
rejected as raw +1,704 / aligned +3).  Result on that base: raw
**53,589 -> 52,672** (-917), aligned **20,577 -> 20,656** (+79), size
57,232 exact, `.text` 74,984 -> 74,067, `.ARM.extab` 564 -> 561,
`.rel.plt` restored to the original md5 `289b2a06`.

## Landed commits (each `check` = RATCHET PASS)

| commit | change | raw | aligned | .text | .extab |
|---|---|---:|---:|---:|---:|
| `55bc87951` | mission-106 s1 store order + s6 status-field order | 52,008 → 52,063 | 20,733 → 20,722 | 72,954 → 73,009 | 564 |
| `655165a63` | mission-1 intro: read `enemyPos.z` before `setToSleep`; `(float)delta * 0.2f * depth` | 52,063 → 52,066 | 20,722 → 20,710 | 73,009 → 73,012 | 564 |
| `f3e167b0f` | mission-89: `moveForward(delta)` after the `m_nState<=2` approach block | 52,066 → 52,070 | 20,710 → 20,702 | 73,012 → 73,016 | 564 |
| `82bb289ff` | mission-89 s1: re-fetch `(*m_pLevel->getEnemies())[0]` in `setDirection` | 52,070 → 52,109 | 20,702 → 20,681 | 73,016 → 73,055 | 564 |
| `3d508fa27` | mission-106 s4: `setFixed(true); setAIDisabled(false); setHitpoints(0); timer=0; state=5` | 52,109 → 53,813 | 20,681 → 20,678 | 73,055 → 74,759 | 564 → 563 |
| `b44ab4d83` | mission-0 s3 faithful teardown order + s3 block moved before s5 | 53,813 → 54,761 | 20,678 → 20,610 | 74,759 → 75,707 | 563 → 566 |
| `2124da335` | mission-0 s4 faithful teardown order + **revert of the s3-before-s5 block move** | 54,761 → **54,170** | 20,610 → **20,683** | 75,707 → 75,116 | 566 → 563 |

## Original disassembly facts used

### mission-0 state 3 (`0x138a10`)

`strd 0 → +0x90` first, `m_nState = 4`, `setComputerControlled(false)`,
`field_5e = 0`, `radar+0x48 = 1`, `getPlayer()->setCollide(true)`,
`m_bCinematicBreak = 0`, `setLookAtCam(false)`, `setTarget(geometry)`,
`field_0x12 = 1`, `m_bStartSequence = 0`.  A duplicate `m_nScriptTimerA = 0`
store is required for the 8-byte `strd` block width; dropping it makes the
function 57,224 B.

### mission-0 state 4 (`0x138d2a`) — landed in `2124da335`

Exact instruction order: `stop`, `play(143)`, `setTurretMode(false)`,
`resetCamera(m_pLevel)`, `setFreeLookMode(false)`, `enableFirstPersonCam(false)`,
`hideShipForFirstPersonCameraView(false)`, `m_bCinematicBreak = 1`,
`m_pLevel->getPlayer()->setCollide(false)`, `m_pRadar->enabled = 0`,
`setComputerControlled(true)`, `removeAllGuns`, `setLookAtCam(true)`,
`setTarget(player->geometry)`, `setRotation(0, 1.5707964f, 0)`,
`setPosition(player->geometry->getPosition())`, `translate(25000,-200,-1000)`,
`m_nState = 5`.  The `Level *self` local and the `Vector playerPos` temp were
dropped; the block now matches `0x138d3a-0x138df6` instruction-for-instruction.

### mission-89 case-3 `else` (`0x13c496`)

`switchSunForSupernovaIntro` → `startFade(false,-1,10000)` →
`enemies[2]->setVisible(true)` → `for (i = 3; i < count; i++) setHitpoints(0)`
→ `strd 0 → +0x90` → `m_nState = 4`.  Our `switchSun` call actually lives in
the mission-42 state-7 block (~line 2160) for `.rel.plt` ordering; do not move
it blindly.

### mission-106 state 2 (`0x13be96`)

No `m_bCinematicBreak = 0` store.  Dropping ours makes the function 57,224 B,
so it needs a +4 B compensation before it can land.

### mission-106 state 6 (`0x13d834`)

As landed in `55bc87951`.  The distinct tail block `0x145b50` uses station 111,
`field_f4 = getCurrentSecondaryWeaponIndex()`, `setGammaHP(100)`.

## `.rel.plt` discovery (important)

`b44ab4d83` moved the mission-0 state-3 block ahead of state 5 in the source.
That silently swapped two `.rel.plt` slots
(`StarSystem::switchPlanetForIntro` ↔ `PlayerStation::setVisible`, md5
`a8d49cee`), which `check` does not catch (it compares `.rel.plt` only via
byte_exact, which stayed at 4508).  Reverting that block move in `2124da335`
restores the original md5 `289b2a06…`.  **Do not re-add the s3-before-s5
move.**  The faithful state-3 *statement* order from `b44ab4d83` is kept
(metrics-neutral).

Related: mission-0 dispatch compare order in our build is `cmp 4 / cmp 5 /
cmp 3` (`0x1389bc..`); the original is `cmp 4 / cmp 3 / cmp 5`
(`0x1389c8..`).  Source block order `4,3,5` produces the original compares but
breaks `.rel.plt`; `4,5,3` (current) is the accepted trade.

## Measured rejected candidates (size-exact unless noted)

| candidate | raw | aligned | note |
|---|---:|---:|---|
| mission-89 s3 V8 (switchSun, timer then state) | 52,101 | 20,723 | aligned +1 |
| same + `i < enemies->count` | 52,101 | 20,723 | no change |
| same without switchSun | 52,240 | 20,721 | aligned −1 |
| mission-89 s3 V4 (`state=4` first) | 52,065 | 20,722 | raw +2 |
| mission-89 s3 V6 (state then timer) | 52,101 | 20,723 | |
| mission-0 s3 fully faithful (dup store removed) | 54,354 | 20,758 | size **57,224** |
| mission-89 s3 `state=4` moved to end | 54,772 | 20,610 | raw +11 |
| mission-106 s2 drop `m_bCinematicBreak=0` | 53,784 | 20,678 | size exact, aligned unchanged |
| mission-0 s4 faithful only (no s3 revert) | 54,170 | 20,683 | `.rel.plt` broken (md5 `0f8d5452`) |
| mission-0 s3 order-only variant (no block move) | 54,756 | 20,600 | `.rel.plt` broken |
| mission-0 block order `453` | 53,813 | 20,678 | rel.plt OK, but worse than `435`+s4 |
| mission-0 block order `435` | 54,761 | 20,610 | rel.plt broken |
| mission-0 block order `354` | 54,775 | 20,696 | rel.plt broken |

Only the `4,5,3` block order keeps both size and `.rel.plt`.

## Final gate

```
raw        52,008 -> 54,170
aligned    20,733 -> 20,683
size       57,232 / 57,232   (exact)
.text      diffbytes 72,954 -> 75,116; size/base unchanged (1,259,464 B)
.ARM.extab diffbytes 564 -> 563; size/base exact 156,312 B @ 0x1a3b78
.ARM.exidx 0, .rodata 0, .data 0 (unchanged)
.rel.plt   md5 289b2a06… (byte-identical, 29,392 B)
check      RATCHET PASS (byte_exact 4508/4524, linked 4508, size 4524/4524,
           data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370)
```

## Scratch

`/home/debian/GalaxyOnFire2/scratch/w13746/` (`measure.sh`, `secs.py`,
`perm.py`, `perm_m0.py`, `base_b44.cpp`, `variantB.cpp`, `chain_parts.json`,
`m0parts.json`, disassembly dumps `orig.dis`, `ours_b44.dis`, `ours_cur.dis`,
`ours_3d.dis`, `obj_b44.dis`).
