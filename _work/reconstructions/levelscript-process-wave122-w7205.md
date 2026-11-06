# LevelScript::process — wave 122 / w7205 (case-154 state-7 fidelity fix)

Status: **code change landed, 2026-11-06**. Branch `wave122-w7205`, base master
`29cd275f6`. Target `_ZN11LevelScript7processEi` (`LevelScript::process`),
original `0x137d18`, **57232 B (0xdf90)**, 20169 normalised instructions.

## Result

```
                         before (master)   after (landed)
raw ratio (difflib)      60.243241227      60.254688355     (+0.011447128)
orig / ours insns        20169 / 20038     20169 / 20037
orig / ours size         57232 / 57232     57232 / 57232    (size-exact)
byte_exact               4506              4506
linked_exact             4506              4506
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN             CLEAN
drift                    0                 0
RATCHET                  PASS              PASS
```

Metric = `difflib.SequenceMatcher(None, onorm, unorm, autojunk=False).ratio()*100`
on `tools/verify/verify.py`'s reconciled instruction streams of the linked `.so`
(helper `/dev/shm/w7205/buildscore.py`, same definition as the wave-119/121
notes).  The before number was re-measured from `git show HEAD:...LevelScript.cpp`,
not copied from the previous note.

## Landed changes (case 154 only, `src/game/world/LevelScript.cpp`)

1. **state 7** (line ~4648).  The original is
   `if ((*enemies)[0]->isDocked()) { m_nScriptTimerA = 0; m_nState++; }`;
   the source had `{ m_pLevel->objectivesB = nullptr; m_nState = 8; }`.

   ```diff
   -                        m_pLevel->objectivesB = nullptr;
   -                        m_nState = 8;
   +                        m_nScriptTimerA = 0;
   +                        m_nState++;
   ```

   Evidence — original `0x14349c`:

   ```
   14349c: ldr  r0, [sp, #96]        ; enemies
   14349e: ldr  r0, [r0, #4]
   1434a0: ldr  r0, [r0, #0]         ; (*enemies)[0]
   1434a2: blx  KIPlayer::isDocked()
   1434a6: cmp  r0, #0
   1434a8: beq.w 0x144e36
   1434ac: ldr  r0, [sp, #88]        ; this
   1434ae: movs r1, #0
   1434b0: mov  r2, r0
   1434b2: ldr  r0, [r0, #28]        ; m_nState
   1434b4: strd r1, r1, [r2, #144]   ; m_nScriptTimerA = 0 (64-bit)
   1434b8: adds r0, #1
   1434ba: str  r0, [r2, #28]        ; m_nState++
   1434bc: b.w  0x144e36
   ```

   `m_nScriptTimerA` is the 64-bit timer at `this+144` (the `+= delta` sites
   use `adds`/`adc` on the pair), so `strd r1,r1` is exactly `= 0`; `m_nState`
   is the word at `this+28`, loaded/`adds`/stored rather than a constant store.

2. **state 10** (line ~4706).  Removed the bogus second increment after
   `m_nState = 11;`:

   ```diff
                            m_nState = 11;
   -                        m_nState++;
                            m_nScriptTimerA = 0;
   ```

   This is **codegen-neutral**: the edit-1-only file and the edit-1+edit-2 file
   both score `60.254688355%` with 20037 instructions.  It is kept because the
   original stores 11 once (`movs r0,#11; str r0,[r5,#28]` at `0x1435a2`) and a
   double increment is plainly wrong even though clang folds the second store.

## Dead ends measured this session (all reverted)

| variant | ratio | insns |
|---|---:|---:|
| baseline (master) | 60.243241 | 20038 |
| **case 154 state-7 fix (landed)** | **60.254688** | **20037** |
| move mission-5 arm before mission-26 (else-if) | 60.163653 | size-exact |
| same, as separate `if` | 57.980000 | 57484 B |
| mission-0 state arms 3/5/4 permutations (all 6) | <= 60.243241 | — |
| case 154 state-10 faithful rewrite (`m_nScriptTime=0`, delete in 2nd `if`, no `m_nState++`) | 59.49 | 57240 B |
| case 154 state-10 drop `m_nScriptTimerA=0` | 59.48 | 57228 B |
| case 154 state-10 move `m_nScriptTime` | 59.37 | — |
| case 154 state-10 move `delete` | 59.48 | 57252 B |
| mission-0 state 16 `enableFillScreen(true)` (+reorders) | 59.27–59.31 | +68–84 B |
| case 125 state-arm permutations | <= 59.498582 | — |

## Remaining root cause

The residual is still **block ordering (MachineBlockPlacement) + register
allocation**, not missing code:

* `this` is spilled to `[sp,#88]` by the original (328 reloads) and kept in
  `sl` by ours (195 reloads); the stack slots themselves already match.
* Largest remaining misplacements: case 131 state 2 (orig `0x13c8fc` vs ours
  `0x13c518`), case 125 state-0 tail (orig `0x139ff6` vs ours `0x139d92`),
  case 154 state 10 (orig `0x14354c` vs ours `0x1436a4`).
* Call-count deltas (orig/ours): `getPlayer` 23/28, `getStarSystem` 15/17,
  `getLandmarks` 15/17, `setTarget` 81/82, `SetCurrentApplicationModule` 9/10,
  `ParticleSystemManager::reset` 2/4, `PlayerStation::setVisible` 1/2,
  `switchPlanetForIntro`/`switchSkyboxForIntro`/`startSmokeEmission`/
  `PlayEngineSound`/`StopEngineSound`/`autoTurretIsEnabled`/`getStanding`/
  `setStanding` 1/2, `translate` 57/56, `getEnemies` 34/33,
  `TransformGetTransform` 37/36.  The +1 duplicates come from deliberate
  `.rel.plt`-order anchors in the source (case-125 state-3 `if (delta < 0)`
  block ~line 3960; `setAutoTurret(player->autoTurretIsEnabled())` ~line 1809;
  `setStanding(0,100)` ~lines 1536/2012) — removing them changes `.rel.plt`
  and breaks byte_exact elsewhere.

## Reproduction

```sh
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
cmake --build cmake-build-match --target drift -j6      # 0 fields
python3 /dev/shm/w7205/buildscore.py src/game/world/LevelScript.cpp tag
```

`/dev/shm/w7205` helpers are scratch only; the note and the two-line source
change are the sole committed artifacts.
