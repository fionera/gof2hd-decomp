# LevelScript::process — wave 119 / w6728 (adjacent-statement sweep on the merged base)

Status: **code change landed, 2026-11-05**. Branch `wave119-w6728`, base master
`fcd88b45b` (wave-119 harvest of w6704). Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, 20169
instructions.

## Result

```
                         before (master)   after (landed)
raw ratio (difflib)      58.488783         58.496667      (+0.007884)
orig / ours insns        20169 / 20037     20169 / 20035
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
byte_exact               4503              4503
linked_exact             4503              4503
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN             CLEAN
drift                    0                 0
RATCHET                  PASS              PASS
sections                 27                27
file size                2187328           2187328
.text                    0x1337c8          0x1337c8
.ARM.extab (ours/orig)   0x26290 / 0x26298 0x26290 / 0x26298  (pre-existing 8 B short)
.rodata (ours/orig)      0x41c27 / 0x41b67 0x41c27 / 0x41b67  (pre-existing)
```

Raw number = `difflib.SequenceMatcher(None, onorm, unorm, autojunk=False).ratio()*100`
on `tools/verify/verify.py`'s `disassemble()` + `reconcile()` streams of the linked
`.so` (helper `/dev/shm/w6728/score_so.py`, which returns the unrounded ratio —
`verify.score()` rounds to 0.1 and cannot see sub-0.05 moves).

## What was applied (two size-neutral statement reorders)

### 1. Mission-89 `m_nState == 4` (source ~1322): `startFade` before `m_nState = 5`

```c++
                     if (m_nScriptTimerA <= 7000 && m_nScriptTimerA + delta > 7000) {
-                        m_nState = 5;
                         Globals::layout->startFade(true, 255, delta + 1000);
+                        m_nState = 5;
                     }
```

Ground truth (orig `0x13b3ba` state-4 body): the `startFade` call is emitted at
`0x13b412`, then the code loads `m_nScriptTimerA` and branches to the shared
timer-add block at `0x13df5c`; the `m_nState = 5` store lives at `0x13df70`
inside that shared `>= 8000` block. So `startFade` before the state store is the
faithful order. (The master source keeps the store in the 7000-crossing arm,
which is itself a wave-118 tuning — see the note below.) This move alone:
**58.488783 -> 58.493757**, 57232 B, 20035 insns.

### 2. Mission-92 state-3 spawn loop (source ~2363): declaration swap

```c++
                     for (int i = 5; i != 9; i++) {
-                        KIPlayer *pirate = (*enemies)[i];
                         int dx = Globals::rnd->nextInt(10000) + 15000;
+                        KIPlayer *pirate = (*enemies)[i];
```

Semantically neutral (two independent declarations; the RNG call order is
unchanged). Tuned for register allocation. Combined with move 1:
**58.496667**, 57232 B, 20035 insns.

## Measurements (all 57232 B unless stated)

### Faithful alternative for move 1 (not landed)

Putting `m_nState = 5` back in a separate `if (m_nScriptTimerA >= 8000)` after
`m_nScriptTimerA += delta` (the literal original structure) is size-exact but
scores **58.485263** — slightly below the master base. The original's shared
timer block is reached from several cases, so re-materialising the check in this
one case does not reproduce the original layout.

### Full adjacent/distance-2 statement sweep

A 3201-variant sweep over the whole process body (every adjacent same-indent
`;`-statement pair, plus every distance-2 swap) was run twice: once on master
and once on the landed base. Harness: compile one `LevelScript.cpp` with the
exact `compile_commands.json` flags, gold-link against the checked-in objects,
score with `verify.py`'s own disassembler (`/dev/shm/w6728/harness.py`,
`/dev/shm/w6728/sweep.py`, `/dev/shm/w6728/sweep_b.py`).

Master best (the two landed moves): 58.496667. On the landed base **no** swap
beats 58.496667 (2922 scored variants; next best 58.495212). Other master
candidates and their effect:

| variant | % | size |
|---|---:|---:|
| move 1 only (`startFade` first) | 58.493757 | 57232 |
| move 1 + move 2 (landed) | **58.496667** | **57232** |
| move 1 + case-92 `m_pHud->visible` before `setComputerControlled` | 58.496667 | 57232 |
| move 1 + case-92 `m_pHud->visible` before `setTarget` (dist-2) | 58.496667 | 57232 |
| move 2 only (declaration swap) | 58.491692 | 57232 |
| case-105 particle `setPosition`/`setDirection` swap | 58.490238 | 57232 |
| case-145 `m_tempVec`/`setDirection` swap | 58.490238 | 57232 |
| move 2 + any of the above | 58.31 | 57240 (not landable) |

The case-92 `m_pHud->visible` reorders are *tuned*: the original emits
`setLookAtCam`, `setTarget`, `setComputerControlled`, `hud.visible`,
`radar.enabled` (orig `0x13ec62..0x13ec84`), i.e. the master order. They tie the
landed score but are less faithful, so they were not taken.

### Structural dead ends re-measured on this base (all not landable)

| variant | % | size |
|---|---:|---:|
| mission-5/26 chain reordered to 5,26,50,51 (kept `else if` on the tail) | 58.4 | 57232 |
| mission-5/26 chain made independent of `m_bStartSequence` (faithful) | 56.6 | 57452 |
| independent + 5-first | 56.6 | 57468 |
| case-125 state-0 `&&` -> nested `if` (faithful) | 58.496667 | 57232 |
| delete the case-125 `if (delta < 0)` first-reference anchor | 58.277124 | 57152 |

The case-125 nested-`if` is size-exact and score-neutral; it was left alone to
keep the change minimal.

## Structural findings (recorded, not landed)

* The original's pre-switch chain is `0,1,81,78,89,106,144,131,158`, then the
  shared `m_bStartSequence` tail (`0x137f7c`), then an **independent**
  `if (mission==5) else if (26) else if (50) else if (51)` chain, then the
  mission switch. Ours attaches the 26/5/50/51 chain as an `else if` of the tail
  and orders it 26,5,50,51. Making it independent is +220 B; reordering alone is
  size-exact but scores 58.4.
* The original's case-125 state-0 is a nested `if (m_nState == 0) { if
  (msg1->isTriggered()) ... }`, not `&&`. Converting is size- and score-neutral.
* Case-125's emitted body is 1038 B in ours vs 98 B in the original because the
  original places the state-1 arm at `0x13b43e` and the state-2 check at
  `0x13b518` (after the case-114/16 bodies); ours keeps them adjacent. This is
  the largest single block-placement divergence left and is not reachable by
  statement reordering alone.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4503
cmake --build cmake-build-match --target lint  -j6      # CLEAN
cmake --build cmake-build-match --target drift -j6      # 0 fields
python3 /dev/shm/w6728/score_so.py
```

**Build gotcha:** the build is timestamp-driven; `touch
src/game/world/LevelScript.cpp` before `link_so` after restoring a variant.
