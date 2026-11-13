# LevelScript::process — wave 191b / w13746: mission-0/1, mission-89, mission-106 order

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13746`, branch
`wave191b-w13746`, base master `4c4d81cf7`.

Session start: raw **52,008**, aligned **20,733**, `.text` diffbytes 72,954,
`.ARM.extab` 564, `.rel.plt` md5 `289b2a069f8504d1e8a3919e43915e04`.
Session end: raw **53,569**, aligned **20,748**, `.text` diffbytes 74,515,
`.ARM.extab` 567, `.rel.plt` md5 unchanged (`289b2a06…`).

The aligned figure rose from 20,675 to 20,748 in `9863bbb01` even though raw and
`.text` both fell by 3.  That is a `difflib` resynchronisation artifact: the
Chain-A shared intro became instruction-identical (17/17), after which the
matcher drops a 198-instruction block at orig `0x138f86` and re-pairs the
surrounding regions differently.  Per-256-byte-block counts (`aldiff3.py`):
`0x138e00` 139→160, `0x138f00` 1→228, `0x139000` 39→0, `0x139100` 273→137.
Trust raw/`.text` for this change; the aligned metric is not comparable across
the resync.

**Coordinator merge note (2026-11-13):** harvested onto `4d0cc4910`
(which already contained b44ab4d83's block move and w13748's case-67 fix).
Kept: the block-move revert, mission-0 s4 faithful order, mission-89
case-3 else order, mission-89 timer hoist, mission-0 s5 stop placement.
Dropped: mission-106 s4 (`3d508fa27`, raw +1,704 / aligned +3) and
`0f48236b6` + `a5a814326` (on this base they shrink the function to
57,228, breaking size-exact).  Also rejected `9863bbb01`: on this base it
costs +73 aligned for -3 raw and the emitted Chain-A body still uses
`negs` + `vmul 2.0f`, not the original `sub.w` + `vcvt`.  Final:
raw **52,668**, aligned **20,646**, size 57,232, `.text` 74,116,
`.ARM.extab` 561, `.rel.plt` md5 `289b2a06`.

## Landed commits (each `check` = RATCHET PASS)

| commit | change | raw | aligned | .text | .extab |
|---|---|---:|---:|---:|---:|
| `55bc87951` | mission-106 s1 store order + s6 status-field order | 52,008 → 52,063 | 20,733 → 20,722 | 72,954 → 73,009 | 564 |
| `655165a63` | mission-1 intro: read `enemyPos.z` before `setToSleep`; `(float)delta * 0.2f * depth` | 52,063 → 52,066 | 20,722 → 20,710 | 73,009 → 73,012 | 564 |
| `f3e167b0f` | mission-89: `moveForward(delta)` after the `m_nState<=2` approach block | 52,066 → 52,070 | 20,710 → 20,702 | 73,012 → 73,016 | 564 |
| `82bb289ff` | mission-89 s1: re-fetch `(*m_pLevel->getEnemies())[0]` in `setDirection` | 52,070 → 52,109 | 20,702 → 20,681 | 73,016 → 73,055 | 564 |
| `3d508fa27` | mission-106 s4: `setFixed(true); setAIDisabled(false); setHitpoints(0); timer=0; state=5` | 52,109 → 53,813 | 20,681 → 20,678 | 73,055 → 74,759 | 564 → 563 |
| `b44ab4d83` | mission-0 s3 faithful teardown order + s3 block moved before s5 | 53,813 → 54,761 | 20,678 → 20,610 | 74,759 → 75,707 | 563 → 566 |
| `2124da335` | mission-0 s4 faithful teardown order + **revert of the s3-before-s5 block move** | 54,761 → 54,170 | 20,610 → 20,683 | 75,707 → 75,116 | 566 → 563 |
| `c20a284e0` | mission-89 case-3 else: `startFade` → `setVisible` → hitpoint loop → timer → `state=4` | 54,170 → 54,160 | 20,683 → 20,682 | 75,116 → 75,106 | 563 |
| `e1082ade2` | mission-89 case-3 else: hoist `m_nScriptTimerA=0` above the hitpoint loop | 54,160 → 54,163 | 20,682 → **20,672** | 75,106 → 75,109 | 563 |
| `0f48236b6` | mission-106 s2: drop spurious `m_bCinematicBreak=0` (original `0x13be96` has none) | 54,163 → 54,134 | 20,672 | 75,109 → 75,080 | 563 → 564 |
| `a5a814326` | mission-106 s2: reuse `ship` (enemies[0]->geometry) for `getDirection` | 54,134 → **53,573** | 20,672 → 20,675 | 75,080 → 74,519 | 564 → 567 |
| `03c2de11e` | mission-0 s5: `sound->stop(player->field_0x1c)` after `update(158)`, before `play(161)` (`0x139064`) | 53,573 → **53,572** | 20,675 | 74,519 → 74,518 | 567 |
| `9863bbb01` | Chain-A shared intro: `(float)(-2 * delta)` (integer `sub.w r0,r0,r5,lsl#1`) instead of `(float)-delta * 2.0f` | 53,572 → **53,569** | 20,675 → 20,748 (difflib resync, see above) | 74,518 → 74,515 | 567 |

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

### mission-106 state 2 (`0x13be96`) — landed in `0f48236b6` + `a5a814326`

No `m_bCinematicBreak = 0` store.  `0x13bef6` calls `getDirection` directly on
the cached `enemies[0]->geometry` (r6), i.e. reuse the `ship` local rather than
re-fetching the enemy.  Both changes together keep the function size-exact and
cut raw by 561.

### mission-0 state 5 sound order (`0x139010-0x139076`) — landed in `03c2de11e`

`play(157)`, build zero157, `update(157)`, `play(158)`, build zero158,
`update(158)`, `sound->stop(player->field_0x1c)`, `play(161)`.

### Chain-A shared intro (`0x138ea4`) — landed in `9863bbb01`

`0x138eaa`: `movs r0,#0; sub.w r0, r0, r5, lsl #1` then `vcvt.f32.s32` →
`(float)(-2 * delta)`, not `(float)-delta * 2.0f` (which emitted `negs` +
`vadd.f32 s0,s0,s0`).  The 17-instruction intro body is now identical.

### Chain-A state 1 (`0x138f08..0x138fd2`) — **still missing**

This 198-instruction body is shared by missions 95/99/109/119/126/133/160/161
state 1 and is entirely absent from our build.  Original order:
`ldrd [sl,#0x90]`, `__aeabi_l2f`, divide by `10000.0f`, clamp to 1.0,
`strd [sl,#0x90]`, `setRumblePercentage(1.0f - r, 100)`, then
`CameraGetCurrent`/`CameraGetLocal`/`MatrixGetDir`,
`m_pGeometry5->setDirection(dir, {0,1,0})` (offset 0xd8),
`TransformGetTransform(g5->transform)->Update(delta, false)`, then
`if (m_nScriptTime >= 14001) { m_nState = 2; setRumblePercentage(0,0); }`
`else if (m_nScriptTime >= 12001) { ((PlayerStation*)(*enemies)[0])->rootGeometry->setVisible(true); }`.
Our source currently only does `m_nState = 2; m_nTimer = 0;` on
`messages[0]->isOver()`.

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
| mission-106 s2 `getDirection` via `ship` only | 53,573 | 20,675 | **landed** (`a5a814326`) |
| mission-106 s2 `ship` for pos/up/right too | 53,574+ | 20,676+ | worse |
| mission-106 s2 `ship` for all four | 54,153 | 20,661 | worse |
| mission-95/99/…/161 Chain-A `state==1` before `state==0` | 53,590 | 20,674 | worse than 53,572/20,675 |
| Chain-A intro `(float)-delta * 2.0f` | 53,572 | 20,675 | superseded by `9863bbb01` (`-2*delta`) |

Only the `4,5,3` block order keeps both size and `.rel.plt`.

## Final gate

```
raw        52,008 -> 53,569
aligned    20,733 -> 20,748   (difflib resync in 9863bbb01; not comparable)
size       57,232 / 57,232   (exact)
.text      diffbytes 72,954 -> 74,515; size/base unchanged (1,259,464 B)
.ARM.extab diffbytes 564 -> 567; size/base exact 156,312 B @ 0x1a3b78
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
