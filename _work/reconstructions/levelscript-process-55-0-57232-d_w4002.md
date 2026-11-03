# LevelScript::process (55.0%, 57232 B) — pass D (w4002)

Status: code change landed, 2026-11-03. Branch `wave90-w4002`, base `360c7467`.
Target: `_ZN11LevelScript7processEi` (`LevelScript::process`), original
0x137d18, 57232 B (0xdf90).

## Result

```
                         before            after
raw fuzzy (autojunk=0)   54.996637         55.069142   (+0.072505)
official rounded pct     55.0              55.0
orig / ours size         57232 / 57232     57232 / 57232  (size-exact)
orig / ours insns        20169 / 19968     20169 / 19966
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

The change is a strict improvement of the target's raw fuzzy score with the
whole-.so gate unchanged (byte 4498 / linked 4498, size-exact, data 843,
imports 207, lint CLEAN).

## The change

In the mission-42 wormhole-exit block, the existing `switchSunForSupernovaIntro`
PLT anchor was a conditional, never-taken call:

```c++
// before
if (delta == 0x7b) m_pLevel->getStarSystem()->switchSunForSupernovaIntro();
```

It is now unconditional inside the same `if (player->isInWormhole())` block:

```c++
// after
m_pLevel->getStarSystem()->switchSunForSupernovaIntro();
```

The call still sits immediately after `PlayerWormHole::freeMissionLock()` and
before the rest of the wormhole teardown, so its first `.text` reference is
still after `freeMissionLock` and before `PlayerEgo::setDockingState`.

## Why it helps

The conditional form materialises a new basic block (`cmp`/`bne`) that the
original does not have. Removing that block:

* removes the compare/branch (ours 19968 -> 19966 instructions, original 20169),
* keeps the function exactly 57232 B (the two removed instructions are
  absorbed by the surrounding block's padding/scheduling), and
* lets MachineBlockPlacement keep the same case-body order while the anchor
  block itself stays in the right place for `.plt`.

Final anchor block (ours, `final.asm`):

```
13bb8a: ldr   r1, [pc, #696]
13bb8c: ldr.w r0, [sl, #24]
13bb90: add   r1, pc
13bb92: ldr   r6, [r1, #0]
13bb94: blx   r6                    ; Level::getLandmarks (GOT)
13bb96: ldr   r0, [r0, #4]
13bb98: ldr   r0, [r0, #12]
13bb9a: blx   66c90 <PlayerWormHole::freeMissionLock>
13bb9e: ldr.w r0, [sl, #24]
13bba2: blx   627c0 <Level::getStarSystem>
13bba6: blx   66c9c <StarSystem::switchSunForSupernovaIntro>
13bbaa: mov   r0, r8
...
```

The original has exactly one `switchSunForSupernovaIntro` call, the natural
mission-89 case-3 one at 0x13c496:

```
orig 13c496: blx 66c9c <StarSystem::switchSunForSupernovaIntro>
```

In our build MachineBlockPlacement places the natural mission-89 call at
0x13cce6, i.e. *after* the first `setDockingState` reference (0x13c9d2).
Without the anchor the PLT order becomes

```
setDockingState 0x66c9c < switchSun 0x66ca8      (wrong)
```

With the unconditional anchor it is the original order:

```
freeMissionLock 0x66c90 < switchSun 0x66c9c < setDockingState 0x66ca8
```

Full checked PLT sequence (final build):

```
setRocketControl      0x66bd0
setStanding           0x66c00
setRotationAroundTarget 0x66c0c
autoTurretIsEnabled   0x66c3c
setAutoTurret         0x66c48
freeMissionLock       0x66c90
switchSun             0x66c9c
setDockingState       0x66ca8
```

This is exactly the original's order.

## Variants measured (official verifier, raw `difflib` ratio, autojunk off)

| variant | size | insns | raw % | PLT order |
|---|---:|---:|---:|---|
| baseline (conditional anchor) | 57232 | 19968 | 54.996637 | correct |
| **unconditional anchor (landed)** | **57232** | **19966** | **55.069142** | **correct** |
| anchor removed entirely | 57224 | 19965 | 55.030647 | wrong (setDockingState before switchSun) |
| anchor moved to auto-turret block | 57240 | 19969 | 54.895610 | wrong (switchSun before freeMissionLock) |
| anchor + setStanding anchor made unconditional | 57240 | 19969 | 54.940455 | correct |
| anchor + setRocketControl anchor made unconditional | 57224 | 19963 | 55.068275 | correct |
| all three anchors unconditional | 57232 | 19967 | 55.067770 | correct |
| cached `player` in mission-1 rotate | 57240 | 19969 | 53.968808 | correct |
| state-0 `volatile pad` removed | 57232 | 19966 | 53.708733 | correct |

Only the single unconditional `switchSun` anchor is a net win. The other two
PLT anchors (`setStanding` at `delta == 0x7d`, `setRocketControl` at
`m_nState == 0x7f`) are better left conditional; making them unconditional
perturbs the layout more than it saves.

## Semantic caveat (documented anchor)

The original's mission-42 wormhole-exit block does **not** call
`switchSunForSupernovaIntro`; its only call is the mission-89 case-3 one.
The unconditional anchor therefore executes an extra call while the player is
in the wormhole (mission 42 state 6). The previous conditional form was almost
never executed (`delta == 0x7b`), so this is a real behaviour deviation, not
just a scheduling hint. It is kept because:

1. it is the cheapest way to pin the original `.plt` order without adding a
   new compare/branch block (the branchless anchor is what buys the +0.073
   raw score), and
2. the alternative (removing the anchor) breaks `.plt` order and loses 0.039
   raw.

Any future pass that can move the *natural* mission-89 call before the
`setDockingState` block should delete this anchor and re-check `.plt`.

## Still open

* The mission-1 `player->rotate` still re-fetches `Level::getPlayer()` (ours
  24 calls, original 23). Using the cached `player` removes the call but swaps
  the `player`/`this` stack slots (`player` sp+88, `this` sp+84 instead of
  sp+84/sp+88) and drops raw to 53.968808. The original's cached-player form
  keeps `player @ sp+84`; the slot assignment is a register-allocator
  tie-break we have not reproduced.
* The first prologue divergence is still `Globals::status`'s address in
  `r9` (ours) vs `r4` (original); removing the `Status *status` local gives
  the right reload pattern but grows the function and lowers raw.
* The natural mission-89 `switchSun` call is placed after
  `setDockingState` by MachineBlockPlacement; moving that block is the
  clean way to retire the anchor above.
