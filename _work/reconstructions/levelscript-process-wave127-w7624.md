# LevelScript::process — wave 127 / w7624 (case-125 tail order + case-154 state-1 setTarget)

Status: **code change landed, 2026-11-07**. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, 20169
normalised instructions. Owned TU: `src/game/world/LevelScript.cpp`.
Base master `4bbe88776` (wave-126 final).

## Result

```
                         before (master)   after (landed)
raw ratio (difflib)      60.238266968      60.241742937     (+0.003475969)
official verify pct      60.2              60.2             (same, rounded)
orig / ours insns        20169 / 20038     20169 / 20039
orig / ours size         57232 / 57232     57232 / 57232    (size-exact)
process .text diffbytes  54566             54550            (-16)
whole .text diffbytes    91367             91351            (-16)
byte_exact               4506              4506
linked_exact             4506              4506
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
RATCHET                  PASS              PASS
.rel.plt name diff       0                 0                (byte-identical)
```

Metric = `difflib.SequenceMatcher(None, onorm, unorm, autojunk=False).ratio()*100`
on `tools/verify/verify.py`'s `disassemble()` + `reconcile()` streams of the
linked `.so`.  `process .text diffbytes` is the byte-diff count over the
function's own 57232-byte range; `whole .text diffbytes` sums the same metric
over every same-size common function (baseline 91367).

## Landed change 1 — case 125 (`m_nState == 2`) tail order

Original `0x13b6b8..0x13b7ee` (confirmed by both the Ghidra decompile and the
Thumb disassembly) is:

```
13b73c  PlayerEgo::setFreeze(true)
13b744  leader = (*enemies)[4]->geometry
13b74a  m_pCamera->setTarget(leader)          <- BEFORE getPosition
13b756  m_tempVec = leader->getPosition()
13b76c  m_tempVec += leader->getDirection() * 4000.0f
13b798  m_tempVec += leader->getUpVector() * -500.0f    <- up before right
13b7b6  m_tempVec += leader->getRightVector() * -500.0f
13b7d2  m_pCamera->setPosition(m_tempVec)
13b7e0  m_nScriptTimerA = 0                    <- timer last
13b7ee  m_nState++
```

Landed the faithful order (`setTarget, getPosition, direction, up, right,
setPosition, timer, m_nState++`).  This is a pure statement reorder; size stays
57232 and `.rel.plt` is unchanged.

## Landed change 2 — case 154 (`m_nState == 1`) state-1 tail

Original state-1 tail is split cold at `0x144444..0x144520`:

```
144444  (timer unchanged when msg1 is not over)
14444c  cmp timer, #1501
14445a  for each enemy with shipGroup == 9: setActive(false); setVisible(false)
1444be  m_pCamera->setTarget(player->geometry)   <- missing in our source
1444c8  m_tempVec = player->getPosition()
1444de  m_tempVec += {-350, 400, -1500}
1444f2  m_pCamera->setPosition(m_tempVec)
1444fc  m_pCamera->setFixed(false)
144508  m_nState++
144514  LODManager::forceUpdate(delta, false)
14451c  m_nScriptTimerA = 0
```

The wave-126 pass had already moved the state-0/1 boundary; it left the
`setTarget(player->geometry)` out and had the `m_nScriptTimerA += delta` inside
the `>= 1501` block instead of the preceding `if (msg1->isOver())` block.  This
pass adds the missing `setTarget` and removes the misplaced inner increment.
(The `if (msg1->isOver()) m_pCamera->setFixed(false);` block is still not the
binary's `timer += delta`; replacing it was measured and regresses both the
fuzzy score and the byte delta — see the table below — so it is left for a pass
that can also fix the surrounding block placement.)

## Experiments measured this pass (not landed; all reverted)

Baseline: raw 60.238266968, process diffbytes 54566, size 57232.

| variant | raw % | size | process diffbytes |
|---|---:|---:|---:|
| **landed (case125 tail + case154 setTarget)** | **60.241742937** | **57232** | **54550** |
| case-154 setTarget only | 60.241742937 | 57232 | 54555 |
| case-125 tail only | 60.238266968 | 57232 | 54561 |
| case-154 state-1 fully faithful (timer in msg1 block, `m_nState++`, `setFixed` after setPosition) | 59.171391624 | 57240 | — |
| case-154 state-1 `setTarget` + remove `setActive(true)` | 60.213395677 | 57232 | 54569 |
| case-154 state-1 `setTarget` + `m_nState++` | 60.243241227 | 57232 | 54573 |
| case-125 state-1 `setName` before `setActive` | 60.235270711 | 57232 | 54548 |
| case-125 state-1 reorder + case-154 setTarget | 59.513505447 | 57224 | — |
| case-125 state-2 rnd call order (getPosition before nextInt) | 59.499117113 | 57224 | — |
| case-154 state-2 camera block (GetDirVector before first `+=`) | 60.117389574 | 57232 | 54558 |
| case-154 state-4 camera block faithful | 58.963390370 | 57240 | — |
| case-154 state-6 camera block faithful | 59.416036610 | 57244 | — |
| case-157 state-5 remove `m_bCinematicBreak` from strong branch | 59.500062181 | 57224 | — |
| case-157 state-5 trigger block (`getPosition` before `+=`, `m_bCinematicBreak`/`m_pHud` in place) | 59.457846307 | 57236 | — |
| case-145 state-1 `setActive(true)` instead of `setTarget(player->geometry)` | 60.198955484 | 57232 | 54563 |

The case-125 `setName`-before-`setActive` reorder is faithful (original
`0x14b46a..0x14b4a0`: `setName`, `~String`, `setActive`, `field_0x74=0`) and has
the best byte delta (54548) but a small fuzzy dip and a bad interaction with the
case-154 change (size 57224).  It is the obvious next candidate once a +8 B
companion is found.

## Ground-truth note

The Ghidra decompile in `/dev/shm/w5304/rea-process.c` uses image-base-0x10000
addresses: a `LAB_0014xxxx` label maps to ELF address `0x14xxxx - 0x10000`
(e.g. `LAB_001493fe` = ELF `0x1393fe`).  This is why the decompile's labels can
look larger than the function's `0x137d18..0x145ca8` range.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
cmake --build cmake-build-match --target drift -j6      # 0 fields
cmake --build cmake-build-match --target lint           # CLEAN
python3 /dev/shm/w7624/score.py                         # 60.241742937
python3 /dev/shm/w7624/textdiff.py                      # process 54550
```

Scratch helpers (`score.py`, `regions.py`, `textdiff.py`, `measure.py`,
`test*.py`, `LevelScript.base.cpp`) live in `/dev/shm/w7624/` and are not part
of the tree.
