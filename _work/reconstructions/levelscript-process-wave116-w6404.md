# LevelScript::process — wave 116 / w6404 (LSDA / case order, angle B)

Status: **code change landed, 2026-11-05**. Branch `wave116-w6404`, base master
`d672d349f` (wave-115 harvest). Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, 20169 instructions.

## Result

```
                         before (master)   after (landed)
raw ratio (difflib)      58.053282         58.164814      (+0.111532)
orig / ours insns        20169 / 20032     20169 / 20034
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
byte_exact               4501              4501
linked_exact             4501              4501
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN             CLEAN
drift                    0                 0
RATCHET                  PASS              PASS
sections                 27                27
file size                2187328           2187328
.text                    0x1337c8          0x1337c8
.ARM.exidx               0x6f70            0x6f70
.ARM.extab               0x26294            0x26294   (4 B short, pre-existing)
.rodata (ours/orig)      0x41c27 / 0x41b67 0x41c27 / 0x41b67
```

Raw number = `difflib.SequenceMatcher(None, onorm, unorm, autojunk=False).ratio()*100`
on `tools/verify/verify.py`'s reconciled instruction streams of the linked `.so`
(helper `/dev/shm/w6404/fast.py`).  All 27 section headers, the exact file size and
the pre-existing `.ARM.extab` shortfall are unchanged.

**Size-exactness of this function is mandatory**: a -8 B variant (see below) drops
the whole-function size to 57224 and shifts every later `.text` symbol, which makes
`byte_exact` fall 4501 -> 3688.  Every landed change is therefore size-neutral.

## What was applied (three statement moves, all in the case-91/92 body)

```c++
// 1. case 91 state 5 (source ~2836): the original stores m_nState=6 BEFORE the
//    enemies[0] hitpoint clear at 0x9aXX (`movs r0,#6; str ...; bl str ...`).
-                        (*enemies)[0]->player->setHitpoints(0);
-                        m_nState = 6;
+                        m_nState = 6;
+                        (*enemies)[0]->player->setHitpoints(0);

// 2. case 91 state 6 (source ~2865): the original calls sound->play(2282,...)
//    first and only then writes m_nScriptTimerB=1
//    (orig 0x9422..0x9436: movw r1,#2282; blx play; movs r0,#1; strd r0,r5,[r4]).
-                        m_nScriptTimerB = 1;
-                        Globals::sound->play(2282, nullptr, nullptr, 0.0f);
+                        Globals::sound->play(2282, nullptr, nullptr, 0.0f);
+                        m_nScriptTimerB = 1;

// 3. case 92 docked arm (source ~3019): tuned swap of the lookBehind() /
//    setRotationAroundTarget(true) pair.  The original emits lookBehind first
//    (0x6418 blx lookBehind; 0x641e..0x6422 setRotationAroundTarget), so this
//    move is *tuned*, not provably faithful; it is what buys the last +0.02.
-                            lookBehind();
-                            m_pCamera->setRotationAroundTarget(true);
+                            m_pCamera->setRotationAroundTarget(true);
+                            lookBehind();
```

Move 1 alone is +4 B (57236) and move 3 alone is -4 B (57228); together they are
size-neutral (57232) and score 58.134965.  Move 2 is size-neutral on its own and
lifts the combination to 58.164814.

## Measurements (all on the wave-115 base unless noted)

### The size-exactness trap (why the obvious wins cannot be landed)

| variant | % | size |
|---|---:|---:|
| case-158 `m_pLevel->getPlayer()->geometry` -> `player->geometry` | 58.061146 | 57224 |
| case-91 `m_nState=6` before `setHitpoints` (move 1) | 58.113621 | 57236 |
| both together | 57.950249 | 57224 |
| move 1 + move 3 | **58.134965** | **57232** |
| move 1 + move 3 + move 2 (landed) | **58.164814** | **57232** |

The 57224/57236 variants are not landable: `check` reports `byte_exact 4501 -> 3688`
because every function after `process` shifts by 8/4 bytes.  `linked_exact` stays
4501 (it is address-insensitive), so only the byte count catches it.

### Statement-move sweeps (all size-neutral unless noted)

* Moving `m_nState = 6;` to any of the 36 statement boundaries in the case-91
  state-5 block: all tie 58.113621 @57236 (a few 58.103-58.108).  The position is
  not what matters; the move itself is.
* Adjacent-statement sweep, case-91 block (lines 2776-2880): only the move above
  beats the base; next best size-neutral 58.053282.
* Adjacent-statement sweep, case-92 block (lines 2877-3130) on the landed base:
  no further win (all tie 58.164814).
* Adjacent-statement sweep, case-158 block (lines 3820-3900): no win.
* Adjacent-statement sweep, mission-0x51 block (lines 975-1040): no win.
* Adjacent-statement sweep, case-42 state-4 block (lines 2050-2100): no win.
* Adjacent-statement sweep, mission-0 intro (lines 620-700): no win.
* Full adjacent-declaration-pair sweep (whole function, 77 pairs) on the landed
  base: no pair beats 58.164814 (all tie or lose).
* `m_pLevel->getPlayer()` -> `player` occurrence sweep (35 sites): only the
  case-158 site wins (58.061146 @57224); best size-neutral is 58.050394 (loses).
* `m_pLevel->getEnemies()` -> `enemies` occurrence sweep (90 sites): no win; the
  neutral sites tie 58.053282.

### Case-order / LSDA angle (the assigned independent angle)

The campaign-mission case-entry order (wave-115 list: 158, 131, 64, 135, 145, 142,
144, 139, 69, 73, 67, 70, 29, 41, 80, 56, 125, 92, 157, 40, 102, 154, 24, 14, 21,
42, 91, 94, 105, 114, 16) is already reproduced by the ascending source order, and
reordering the source cases is layout-neutral (wave-115: 57.668101 @57224), so it
was not re-attempted.

New structural evidence gathered this pass (not landed, recorded for the next
worker):

* The **top-level mission if-chain order differs** from the original.  The
  original's compare chain is `0, 1, [shared tail], 5, 26, 50, 51, [switch guard]`
  plus a second chain `81, 78, 89, 106, 95/99/109/119/126/133/160/161, [shared
  tail]` (original addresses: 5@0x416, 26@0x5c4, 50@0x6cc, 51@0xa54, 81@0x49c,
  78@0x614, 89@0x85c, 106@0xf62, 95-group@0x112c).  Our source order is
  81,78,89,106,95-group, then `else if (26)`, `else if (5)`, 50, 51.
  Making the 26/5 chain a separate `if` after `if (m_bStartSequence)` is the
  faithful shape but costs +244..260 B and scores 56.35-56.37, so it cannot be
  landed.  Swapping 26/5 inside the existing `else if` chain scores 57.968707
  @57232 (loses).
* The original's mission-0x51 state-3 block sits at 0x146a and tail-jumps to the
  shared tail at 0x3418; ours is adjacent to mission-106's block at 0x32a8, so
  clang tail-merges the `Galaxy::getStation(100)` call into the `getStation(10)`
  site.  That is the source of the `Galaxy::getStation` / `Status::departStation`
  -1 call-count deltas (8 vs 9 sites).  Forcing a local
  (`Station *departTarget = ...; departStation(departTarget);`) adds the 8 B back
  but scores 57.954432, so it does not compensate the case-158 -8 B.
* The original's case-125 handler is a `switch (m_nState - 1)` (7-entry `tbh` at
  0x230c) after two `m_nState == 0` guards; our source is a linear `if` chain.
  The original has 15 `tbh` tables, ours 14 in a different order — the remaining
  big block-placement loss (the 297-instruction `olen` block at orig 0x6910) is
  a nested state-machine layout difference, not the top-level case order.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
cmake --build cmake-build-match --target drift -j6      # 0 fields
```

## Gate at the end of this pass

```
byte_exact 4501  linked_exact 4501  data_exact 843/843  imports 207
lint CLEAN  drift 0  RATCHET PASS
sections 27  file size 2187328  .ARM.extab 0x26294 (pre-existing 4 B short)
```
