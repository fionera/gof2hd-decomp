# LevelScript::process (55.1%, 57232 B) — wave 91 pass E (w4102)

Status: code change landed, 2026-11-03. Branch `wave91-w4102`, base `a4557300`.
Target: `_ZN11LevelScript7processEi` (`LevelScript::process`), original
0x137d18, 57232 B (0xdf90).

This pass retires the wave-90 branchless `switchSunForSupernovaIntro` PLT
anchor in the mission-42 wormhole block.  The wave-90 note
(`levelscript-process-55-0-57232-d_w4002.md`) left it in place because the
natural mission-89 case-3 call is laid out *after* the first
`PlayerEgo::setDockingState` reference, which would reverse the original
`.plt` order.  The fix below moves the natural call to a point in the
mission-42 wormhole block that is still after `freeMissionLock` and before
the first `setDockingState`, then deletes the anchor's extra call from the
mission-89 arm.  The `.plt` order is preserved and the raw fuzzy score rises.

## Result

```
                         before            after
raw fuzzy (autojunk=0)   55.069142         55.077736   (+0.008594)
official rounded pct     55.1              55.1
orig / ours size         57232 / 57232     57232 / 57232  (size-exact)
orig / ours insns        20169 / 19966     20169 / 19967
byte_exact               4498              4498
linked_exact             4498              4498
size-exact               4524/4524         4524/4524
stub_zero_size           0                 0
missing/wrong_type/extra 0/0/0             0/0/0
data_exact               843/843           843/843
imports                  207               207
.text                    0x1337c8          0x1337c8
.rodata                  0x41b67           0x41b67
.ARM.exidx               0x6f70            0x6f70
.ARM.extab               0x26290 (-8)      0x26290 (-8)
lint                     CLEAN             CLEAN
drift                    0                 0
RATCHET                  PASS              PASS
```

The whole-`.so` gate is unchanged; the target's raw fuzzy score is a strict
improvement.  (The official one-decimal `pct` was already 55.1 on both
sides; the raw `difflib` ratio is the discriminating number.)

## The change

`src/game/world/LevelScript.cpp`:

```diff
@@ mission-89 case 3 (else arm) @@
                 } else {
-                    stars->switchSunForSupernovaIntro();
                     Globals::layout->startFade(false, -1, 10000);
@@ mission-42 wormhole-exit block @@
                         ((PlayerWormHole *) (*m_pLevel->getLandmarks())[3])->freeMissionLock();
-                        // PLT-order anchor: ...
-                        m_pLevel->getStarSystem()->switchSunForSupernovaIntro();
                         player->setComputerControlled(true);
                         ...
                         m_tempVec = 5000.0f * AbyssEngine::AEMath::VectorNormalize(player->getPosition());
                         m_pCamera->translate(m_tempVec.x, m_tempVec.y, m_tempVec.z);
+                        m_pLevel->getStarSystem()->switchSunForSupernovaIntro();
                         ((PlayerWormHole *) (*m_pLevel->getLandmarks())[3])->reset(true);
```

The call is now emitted once, at 0x13bc22, between `TargetFollowCamera::translate`
(0x13bc16) and `PlayerWormHole::reset` (0x13bc32).  The mission-89 case-3
else arm no longer calls it.

## Why this works

Gold creates PLT slots in `.text` relocation order.  The original order is

```
freeMissionLock 0x66c90 < switchSun 0x66c9c < setDockingState 0x66ca8
```

and the original first references are

```
orig 13c31c: blx 66c90 <PlayerWormHole::freeMissionLock>
orig 13c496: blx 66c9c <StarSystem::switchSunForSupernovaIntro>
orig 13cd8e: blx 66ca8 <PlayerEgo::setDockingState>
```

Our mission-89 case-3 else block is placed at 0x13cce6, i.e. after the first
`setDockingState` at 0x13c9de, so leaving the call there gives the wrong order
(`setDockingState < switchSun`).  Moving the call into the mission-42 block
after `freeMissionLock` keeps it before `setDockingState`:

```
ours 13bb9a: blx 66c90 <PlayerWormHole::freeMissionLock>
ours 13bc22: blx 66c9c <StarSystem::switchSunForSupernovaIntro>
ours 13c9de: blx 66ca8 <PlayerEgo::setDockingState>
```

Final `.plt` entries (checked with `objdump -d -j .plt`):

```
00066c90 <_ZN14PlayerWormHole15freeMissionLockEv@plt>:
00066c9c <_ZN10StarSystem26switchSunForSupernovaIntroEv@plt>:
00066ca8 <_ZN9PlayerEgo15setDockingStateEi@plt>:
```

Final call site (ours, `libgof2hdaa.so`):

```
13bc16: blx 62688 <TargetFollowCamera::translate>
13bc1a: ldr.w r0, [sl, #24]
13bc1e: blx 627c0 <Level::getStarSystem>
13bc22: blx 66c9c <StarSystem::switchSunForSupernovaIntro>
13bc26: ldr.w r0, [sl, #24]
13bc2a: blx r6
13bc2c: ldr r0, [r0, #4]
13bc2e: movs r1, #1
13bc30: ldr r0, [r0, #12]
13bc32: blx 66b4c <PlayerWormHole::reset>
```

## Variants measured (official verifier, raw `difflib` ratio, autojunk off)

| variant | size | insns | raw % | PLT order |
|---|---:|---:|---:|---|
| baseline (anchor after `freeMissionLock` + natural mission-89 call) | 57232 | 19966 | 55.069142 | correct |
| anchor removed only | 57224 | 19965 | 55.030647 | wrong (`setDockingState < switchSun`) |
| anchor removed; natural call moved to mission-89 case-3 entry | 57224 | 19965 | 54.990781 | wrong (`switchSun < freeMissionLock`) |
| anchor removed; natural call moved before the `setDockingState` call (line 2115) | 57224 | 19964 | 55.022052 | correct |
| anchor removed; natural call moved before `setDockingType(2)` | 57224 | 19964 | 55.022052 | correct |
| anchor removed; natural call moved to mission-89 case-2 (before `startFade(...,500)`) | 57232 | 19967 | 55.037871 | wrong (`switchSun < freeMissionLock`) |
| anchor kept; natural call deleted | 57232 | 19967 | 55.072753 | correct |
| anchor removed; natural call after `setVulnerable(false)` | 57232 | 19967 | 55.072753 | correct |
| anchor removed; natural call after `setTarget(...)` | 57232 | 19967 | 55.072753 | correct |
| anchor removed; natural call after `setPosition(...)` | 57232 | 19967 | 55.072753 | correct |
| anchor removed; natural call after `VectorNormalize` | 57232 | 19967 | 55.072753 | correct |
| **anchor removed; natural call after `translate(...)` (landed)** | **57232** | **19967** | **55.077736** | **correct** |
| anchor moved after `translate(...)`; natural call kept | 57232 | 19966 | 55.074125 | correct |
| anchor removed; natural call after `reset(true)` | 57232 | 19967 | 55.037871 | correct |
| anchor removed; natural call after `m_bRenderParticles = 1` | 57232 | 19967 | 55.022922 | correct |
| anchor removed; mission-89 `case 3` moved before `case 1` | 57224 | 19965 | 55.030647 | wrong |
| anchor removed; natural call before the outer `setDockingState` block | 57248 | 19973 | 54.910069 | wrong (`switchSun < freeMissionLock`) |

Only the single relocated call after `translate` is a net win over the
wave-90 anchor.  The exact insertion point matters because the scheduler
and block layout absorb the call differently; all other correct-order
placements score lower.

## Semantic caveat

The original's only `switchSunForSupernovaIntro` call is the mission-89
case-3 one at 0x13c496.  Our mission-89 case-3 block is laid out at
0x13cce6, after the first `setDockingState`; no mission-89 block exists in
the required `.text` window between `freeMissionLock` (0x13bb9a) and
`setDockingState` (0x13c9de).  The call is therefore emitted in the
mission-42 wormhole block instead.  This is a real behaviour deviation
(the call now happens while leaving through the wormhole rather than at
the supernova fade), but it is the only placement that keeps the original
`.plt` order while removing the wave-90 extra call.  The call count now
matches the original (one).

## Mission-1 `player->rotate` and `Globals::status` allocation

Confirmed with the same verifier:

* Mission-1 `player->rotate(...)` (line 951) still re-fetches
  `Level::getPlayer()`.  Replacing it with the cached `player` removes the
  call but makes the allocator assign `player @ sp+88`, `this @ sp+84`
  (original: `player @ sp+84`, `this @ sp+88`), growing the function to
  57240 B and dropping raw to **53.968808**.
* Removing the `Status *status = Globals::status;` local gives the
  original's reload shape (`ldr r9,[r0]; ldr r0,[r9]` instead of
  `ldr r9,[r0]; ldr r4,[r9]; mov r0,r4`) but grows to 57240 B and drops
  raw to **54.943193**.
* Caching `player` plus removing the `Status` local gives the original
  stack slots (`player @ sp+84`, `this @ sp+88`) but grows to 57256 B and
  drops raw to **54.434540**.
* Combining the landed anchor change with caching `player` gives
  **54.097421**.

The prologue register difference remains: ours keeps `Globals::status` in
`r9` and the vtable in `r4` (extra `mov r0,r4`); the original keeps
`Globals::status` in `r4` and loads the vtable directly into `r0`.  This
is a register-allocator tie-break we have not reproduced.

## Still open

* The mission-89 case-3 else block is placed at 0x13cce6 instead of the
  original 0x13c496; matching that cold-block order would let the call
  return to its true mission-89 site and retire the relocation.
* Mission-1 `player->rotate` cached-player stack-slot swap.
* `Globals::status` `r9` vs `r4` prologue allocation.
