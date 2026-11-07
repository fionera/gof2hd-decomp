# LevelScript::process — wave 134 / w7745 (mission-chain compare order, case-154 dir fix, resetGunDelay receiver)

Status: **code change landed, 2026-11-07**. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**. Owned TU:
`src/game/world/LevelScript.cpp`. Base master `ec0acea1e` (wave-133 docs head).

## Result

```
                         before (master)   after (landed)
official verify pct      59.9              59.9
score.py pct             58.9              58.7
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
orig / ours insns        20169 / 20044     20169 / 20042
rel.plt                  identical         identical
byte_exact               4506              4506
linked_exact             4506              4506
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
RATCHET                  PASS              PASS
```

The official `verify` percentage is unchanged at 59.9; the difflib `score.py` number
dips 0.2 because the corrections re-roll `MachineBlockPlacement` for the cold
case-154 block. The three edits are individually verified against the original
disassembly.

## 1. NEW: the special-mission compare chain order is wrong (mission 5 vs 26)

Following every `blx getCurrentCampaignMission` to its following `cmp r0,#imm` gives
the original's linear dispatch order:

```
ORIGINAL  0, 1, 2, 5, 81, 26, 78, 50, 89, 51, 42, 106, 95/99/109/119/126/133/160/161
OURS      0, 1, 2, 81, 26, 78, 5, 89, 50, 51, 42, 106, ...
```

(2 is the `mission >= 2` guard at 0x137fe8, not an equality test.)

The original's *shared tail* (0x13812a) checks mission 5 first at `0x13812e`
(`cmp r0,#5`), and the false path goes to the mission-26 check at `0x1382dc`
(`cmp r0,#26`). Therefore the original source order of the "pre-switch specials"
is **5, 26, 50, 51**, while our source had **26, 5, 50, 51** (lines 1499/1506).
Swapping the two blocks makes the specials order match the original exactly.
The swap alone is size-neutral (57232 B) and keeps `.rel.plt` byte-identical.

The relative placement of the specials against the 81/78/89 chain is still wrong
(the original puts the whole 5/26/50/51/42 chain in the shared tail *before* the
mission-81 check at 0x1381b4; ours keeps it at the end of the if/else chain).
Moving the whole specials block before the mission-0x51 arm was tried and is
**not** viable: 57324 B (+92) and `.rel.plt` differs (the block contains the
`goto wormholeExitBody` label and the `Standing::setStanding` first reference).

## 2. Case-154 state 2: stale `m_dirVec` (w7728 §3.1, not landed then)

Original `0x142f9a..0x14305a`:
```
MatrixGetPosition
operator=            m_tempVec = pos
GetDirVector
operator=            m_dirVec = dir
vldr 0.008 ; operator* ; operator+=   m_tempVec += dir*(step*0.008)
getRightVector ; operator= ; vldr -0.05 ; operator* ; operator+=
GetUpVector    ; operator= ; vldr 0.0051 ; operator* ; operator+=
```
Our source had the first `+=` *before* `GetDirVector` (using the stale
`m_dirVec`), then overwrote `m_dirVec` twice. Fixed to the faithful order.
This edit alone is **-4 B** (57228) and keeps `.rel.plt` identical.

## 3. `resetGunDelay` receiver: the original always calls `getPlayer()` fresh

All ten `PlayerEgo::resetGunDelay` call sites in the original are preceded by
`ldr r0,[level,#24]; blx Level::getPlayer`:

```
13dd60  ldr r0,[r5,#24]  ; blx getPlayer ; blx resetGunDelay
13e104  ldr r0,[r6,#24]  ; blx getPlayer ; blx resetGunDelay
13f888  ldr r0,[r9,#24]  ; blx getPlayer ; blx resetGunDelay
14028c  ldr r0,[r5,#24]  ; blx r4 (getPlayer)
140424  ldr r0,[r6,#24]  ; blx getPlayer ; blx resetGunDelay
140f5a  ldr r0,[r5,#24]  ; blx getPlayer ; blx resetGunDelay
1419f6  ldr r0,[r6,#24]  ; blx getPlayer ; blx resetGunDelay
142212  ldr r0,[r6,#24]  ; blx getPlayer ; blx resetGunDelay
143b38  ldr r0,[fp,#24]  ; blx getPlayer ; blx resetGunDelay
144982  ldr r0,[sl,#24]  ; blx getPlayer ; blx resetGunDelay
```

Two source sites still used the cached `player` local (lines 3179 and 4683,
the latter is the `setFixed(false)` body). Both changed to
`m_pLevel->getPlayer()->resetGunDelay()`. This is a semantic no-op but matches
the original's call shape. (The w7705 note had claimed the opposite for the
`setFixed(false)` site; the disassembly at `0x14041a..0x140424` clearly reloads
`level->getPlayer`.)

## Size accounting (measured on master ec0acea1e)

| edit | size | rel.plt |
|---|---:|---|
| case-154 state-2 fix | 57228 (-4) | SAME |
| resetGunDelay 3179+4683 -> getPlayer | 57236 (+4) | SAME |
| case-154 + both resetGunDelay | **57232 (0)** | SAME |
| mission 5/26 swap | 57232 (0) | SAME |
| mission 5/26 swap + case-154 + both resetGunDelay | **57232 (0)** | SAME |

Deltas are not additive; the landed combination is the last row.

## 4. Anchor status (unchanged this pass)

The four source-level anchors are still present and still required for
`.rel.plt` order:
* `delta < 0` first-reference block (case 125 state 3, line ~3975)
* `if (delta == 0x7d) getStanding()->setStanding(...)` (line ~2009)
* `player->setAutoTurret(player->autoTurretIsEnabled())` (line ~1811)
* the state-0 volatile pad

Removing the `delta < 0` block is **-80 B** (57152) and rotates `.rel.plt`
(`PlayerStation::setVisible` 2836 <-> `PlayerEgo::deleteHackingGame` 2837).
The only way to remove it without another anchor is to make MBP place the real
case-42 state-7 `PlayerStation::setVisible` block (ours `0x13e5e8`, original
`0x13da1e`) before the case-125 `deleteHackingGame` block (ours `0x13d97c`,
original `0x13ded4`). That is a block-placement problem, not a missing
statement. The remaining "missing" calls (`translate` x2, `forceUpdate`,
`TransformGetTransform`, `setPosition(fff)` x1) are tail-merges, not missing
source statements: the source has 57 `m_pCamera->translate` statements and the
original emits 57 calls, while ours emits 55.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
```
