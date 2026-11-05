# LevelScript::process — wave 111 (w5903): cached-player slot at the 2001-tick stop

Status: **code change landed, 2026-11-05**. Branch `wave111-w5903`, base master
`7df93bd5a` (wave 110). Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**,
20169 instructions.

## Result

```
                         before (HEAD)     after
official raw ratio       57.189063         57.191193      (+0.002130)
orig / ours insns        20169 / 20026     20169 / 20026
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
byte_exact               4500              4500
linked_exact             4500              4500
size-exact               4524/4524         4524/4524
stub_zero_size           0                 0
missing/wrong_type/extra 0/0/0             0/0/0
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN             CLEAN
RATCHET                  PASS              PASS
file size                2187328           2187328        (unchanged)
sections                 27                27             (unchanged)
.ARM.extab               156308 (4 B short) 156308 (unchanged)
.rodata                  269351            269351         (unchanged)
```

The raw number is `difflib.SequenceMatcher(None, onorm, unorm, autojunk=False)
.ratio()*100` on `tools/verify/verify.py`'s reconciled instruction streams of
the linked `.so`.  No section address/size changed; the fix is size-neutral.

## What changed (one file, `src/game/world/LevelScript.cpp`)

Line 790 (pre-change), the mission-0 intro "2001-tick stop" arm:

```diff
-                        m_pLevel->getPlayer()->setVisible(false);
+                        player->setVisible(false);
```

This is a genuine content fix, not a metric artefact.  The original's copy of
this block is at `0x13d0ea`:

```
13d0ea: 9815        ldr   r0, [sp, #84]   ; cached PlayerEgo* slot
13d0ec: 2100        movs  r1, #0
13d0ee: f729 ecfe   blx   PlayerEgo::setVisible(bool)
```

i.e. it reloads the `player` local from its spill slot (`sp+84`) and does **not**
re-fetch `m_pLevel->getPlayer()`.  Ours emitted `ldr.w r0,[sl,#24]` +
`blx Level::getPlayer` before the call.  Dropping the re-fetch moves our
`Level::getPlayer` call count 30 -> 29 (original 23) and keeps the function at
exactly 57232 B.

## Register-split investigation (status r9 vs r4)

Goal of the wave was the `Globals::status` register split.  Findings:

* Original prologue (`0x137d18`): `getMessages` -> r5, `getPlayer` -> `[sp+84]`,
  `getEnemies` -> r8, then `ldr r4,[r0]` puts the **status value in r4**.  Frame
  slots: `sp+72` messages/delta, `sp+80` `&m_pLevel`, `sp+84` player,
  `sp+88` `this`, `sp+92` status, `sp+96` enemies.
* Ours: same frame slots, but the allocator keeps `&m_pLevel` in r4, enemies in
  r8 and the status value in **r9**; the original spills all of them and
  reloads.  Counted reloads: `[sp,#92]` orig 60 vs ours 40; `[sp,#96]` 176 vs
  181; `[sp,#84]` 169 vs 164.
* The split is a *global* allocation consequence, not a local source-shape
  difference: a register-insensitive diff (all `rN` -> `rX`) only lifts the
  ratio 57.19 -> 60.43, and additionally masking immediates gives 62.84.  So
  register naming is worth at most ~3.2 points; the remaining ~37 points are
  instruction-sequence/block-layout differences.
* The dominant structural mismatch is per-case block order.  Example: original
  `case 73` dispatches a `tbh` on `m_nState` at `0x13983e` with bodies at
  `0x13984a` (state 0), `0x13db3c` (1), `0x13db76` (2), `0x13dbd8` (3); ours
  emits the same bodies at different addresses (state-3 scatter block at
  `0x13e5d8`).  Overall ours has 7 `tbh` in `0x13905e..0x13a9b0` where the
  original has 1, i.e. the switch-body emission order still differs widely.

Forcing status into r4 by declaring a `Status *status = Globals::status;` local
before the getters does produce `ldr.w r4,[r9]` (status value in r4), but the
whole-function ratio drops to 56.285 (57316 B) because the prologue call order
changes to status/enemies/player/messages.

## Measurements (official linked raw ratio, 57232 B base)

### Prologue getter order (wave-111 sweep, before this fix)

| order | % | size |
|---|---:|---:|
| `enemies, player, messages` (HEAD) | 57.189063 | 57232 |
| `player, enemies, messages` | 57.187640 | 57232 |
| `messages, player, enemies` (original call order) | 56.335292 | 57316 |
| `messages, enemies, player` | 56.318948 | 57316 |
| `enemies, messages, player` | 56.313968 | 57316 |
| `player, messages, enemies` | 56.318948 | 57316 |

### Single-site `m_pLevel->getPlayer()` -> `player` (source line; size-neutral rows in bold)

Only **line 790** improves.  Every other site is worse or neutral, and most
shrink the function (breaks size-exactness).

| line | % | size |
|---|---:|---:|
| **790** | **57.191193** | **57232** |
| 740 | 57.182664 | 57232 |
| 1903 | 57.181241 | 57232 |
| 4570 | 57.176266 | 57232 |
| 4563 | 57.172712 | 57232 |
| 1801 | 57.154231 | 57232 |
| 2853 | 57.189063 | 57232 |
| 3610 / 3611 / 3615 / 3616 | 57.187640 | 57232 |
| 4695 | 57.187640 | 57232 |
| 689 | 57.122241 | 57224 |
| 3754 | 57.122241 | 57224 |
| 5081 | 57.117265 | 57224 |
| 4281 | 57.113710 | 57224 |
| 1466 / 1468 / 1472 / 4229 | 57.112289 | 57224 |
| 5030 | 57.112289 | 57228 |
| 1594 | 57.100202 | 57224 |
| 3976 | 57.095225 | 57224 |
| 3171 | 57.085272 | 57224 |
| 4595 | 57.068923 | 57224 |
| 2995 / 2997 | 57.027673 | 57216 |
| 2999 | 57.007043 | 57208 |
| 4820 | 56.964419 | 57224 |
| 947 | 54.063472 | 57380 |
| 1459 | 53.945338 | 57380 |
| 4030 | 47.460748 | 57404 |

Lines 947 / 1459 grow the function by 148 B: using the long-lived `player`
local there extends its live range across the mission-0 block and forces extra
spills.  Line 4030 similarly grows by 172 B.

### Combinations (all worse than line 790 alone)

| sites | % | size |
|---|---:|---:|
| 790 + 1903 | 57.183372 | 57232 |
| 790 + 4570 | 57.178396 | 57232 |
| 790 + 4563 | 57.174843 | 57232 |
| 790 + 3610/3611/3615/3616 | 57.148544 | 57216 |
| 3610/3611/3615/3616 | 57.146412 | 57216 |
| 790 + 740 | 57.130772 | 57224 |
| 790 + 1801 | 57.102336 | 57224 |
| 790+740+1903+4563+4570+1801 | 57.073899 | 57208 |
| all of the above | 57.064649 | 57192 |

### Other rejected variants (wave-111)

| variant | % | size |
|---|---:|---:|
| `Status *status` local used everywhere (MPE order) | 50.667464 | 57232 |
| `Status *status` declared before getters | 56.285487 | 57316 |
| cache `getCurrentCampaignMission()` for first two checks | 51.552903 | 57564 |
| `Level *level = m_pLevel;` + MPE getters (`levelbody`) | 46.95 | 57068 |
| all `m_pLevel->getEnemies()` -> `enemies` | 43.74 | 56860 |
| all `m_pLevel->getPlayer()` -> `player` | 54.13 | 57192 |
| `initStreamOutPosition`/`setAutoPilot`/wingmen-loop reorders | ~54.7 | 57304 |

## Reproduce

```
python3 /dev/shm/w5903/fast.py <variant.cpp> <tag>   # compile one TU, private gold
                                                     # link, official metric (~2.5 s)
cmake --build cmake-build-match --target check -j6   # RATCHET PASS
```
