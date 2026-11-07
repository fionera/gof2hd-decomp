# LevelScript::process — wave 131 / w7689 (early/mid cases + mission-0 prologue audit)

Status: **no source change landed, 2026-11-07**. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, 20169 normalised
instructions. Owned TU: `src/game/world/LevelScript.cpp`. Base master `7c6933c2d`.
Scratch helpers in `/dev/shm/w7689/` (not tracked).

## Result

```
                         before (master)   after (no code change)
raw ratio (difflib)      59.879643905      59.879643905
official verify pct      59.9              59.9
orig / ours insns        20169 / 20045     20169 / 20045
orig / ours size         57232 / 57232     57232 / 57232    (size-exact)
process .text diffbytes  54651             54651
byte_exact               4506              4506
linked_exact             4506              4506
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
RATCHET                  PASS              PASS
```

Metric = `difflib.SequenceMatcher(None, onorm, unorm, autojunk=False).ratio()*100`
on `tools/verify/verify.py`'s reconciled streams (`/dev/shm/w7689/score.py`).
`process .text diffbytes` is the byte-diff count over the function's own 57232-byte
range (`/dev/shm/w7689/bytediff.py`).

## New hypothesis tested

The user's hypothesis: reconstruct the early/mid mission cases (0–120) and the
mission-0 prologue statement-by-statement, focusing on the cases with the largest
diff bytes.  All work below was checked against manual Thumb disassembly of the
original (Ghidra provider was unavailable in this worktree: `rea doctor` reports
`ghidra-installation config_drift` because
`/home/debian/ghidra-extract/ghidra_12.1.2_PUBLIC` no longer exists).

## Finding 1 — the pre-switch `else if` chain has the wrong shape

The original's pre-switch dispatch is a chain of `getCurrentCampaignMission()`
comparisons.  Decoding the branch targets gives the **source** order:

```
mission 0  (0x137d62 -> body 0x137e84)
mission 1  (0x137d7c -> body 0x137d82)
shared tail (0x137f7c)
mission 5  (0x13812a)
mission 26 (0x1382dc)
mission 50 (0x1383e4)
mission 51 (0x13876c)
mission 81 (0x1381b2)
mission 78 (0x138332)
mission 89 (0x13857a)
mission 106 (0x138c80)
mission 95/99/109/119/126/133/160/161 (0x138e44)
main switch guard (0x13879e)
```

Evidence: each failed comparison branches to the next check, and each successful
body branches to the main guard (`b.n 0xa86` / `b.w 0x264`):

```
137d7e  bne.w 0x49a   ; mission != 1 -> 0x1381b2 (mission 81)
13812a  mission == 5
138134  bne.w 0x5c4   ; -> 0x1382dc (mission 26)
1382dc  mission == 26
1382e4  bne.w 0x6cc   ; -> 0x1383e4 (mission 50)
1383e4  mission == 50
1383ec  bne.w 0xa54   ; -> 0x13876c (mission 51)
13876c  mission == 51
13877e  bne.n 0xa86   ; -> 0x13879e (main guard)
1381b2  mission == 81
1381ba  bne.w 0x614   ; -> 0x13832c (mission 78)
13832c  mission == 78
138334  bne.w 0x85c   ; -> 0x138574 (mission 89)
138574  mission == 89
13857c  bne.w 0xf62   ; -> 0x138c7a (mission 106)
138c7a  mission == 106
138c82  bne.w 0x112c  ; -> 0x138e44 (mission 95 group)
138e44  mission 95/99/.../161
138ea0  bne.w 0x264   ; -> shared tail
```

Our source instead groups `81,78,89,106,95` into the **first** chain (as `else if`
of mission 0/1) and leaves `26,5,50,51` in the second chain (as `else if` of
`m_bStartSequence`).  The emitted comparison order is therefore

```
ours:   81, 5, 78, 26, 89, 50, 51, 106, 95
orig:   5, 26, 50, 51, 81, 78, 89, 106, 95
```

This is a real behavioural difference too: in the original, a mission-81/78/89/106/95
intro is *not* gated on `m_bStartSequence`; in ours it is in the first chain and runs
regardless.

### Experiment: move the whole chain into the `else` block

The faithful structure is:

```cpp
if (mission == 0) { ... }
else if (mission == 1) { ... }
else if (mission == 81) { ... }
...
else if (mission == 95 || ...) { ... }
else {
    if (m_bStartSequence) { ... }
    if (mission == 5) { ... }
    else if (mission == 26) { ... }
    else if (mission == 50) { ... }
    else if (mission == 51) { ... }
}
```

Measured (`/dev/shm/w7689/reorder2.py` / `reorder3.py`):

| variant | size | orig/ours insns | raw % | process bytediff |
|---|---:|---:|---:|---:|
| **baseline** | **57232** | **20169/20045** | **59.879644** | **54651** |
| whole chain inside `else` (5,26,50,51,81,78,89,106,95) | 57332 | 20169/20039 | 52.646240 | — |
| only 5/26 swapped in the second chain | 57232 | 20169/20045 | 59.800070 | 54651 |

The faithful shape reproduces the original comparison order exactly
(`5,26,50,51,81,78,89,106,95`) but costs **+100 B** and drops the fuzzy score by
7.2 points because MBP places the 81/78/89/106/95 bodies after the main switch
instead of before it.  It is not landable without a −100 B companion.

## Finding 2 — mission-0 prologue is statement-faithful

Decoded and compared the mission-0 prologue bodies instruction-for-instruction:

* state 0 (`0x137e9c..0x137f76`): `m_nState=1`, player `setPosition(18000,-12000,-40000)`,
  camera `setPosition(-12000,2000,-500)`, `MatrixSetTranslation`, the three
  `enemies[i]->setPosition` calls, the 3-iteration `setDirection`/`setVisible` loop,
  `lodManager->forceUpdate(30,false)`.  **Exact match.**
* state 1/2 (`0x1383fe..0x138538`): pulse wobble (`getPulseValue(0.0005)-0.5`), the
  three `geometry->translate(0,wobble,0)` calls, `msg6->isOver()` -> state 2 +
  camera `setTarget`/`setPosition(-5000,300,-5000)`, then `sound->isPlaying(142)` /
  `stop` / `play(142)`, `translate(delta*0.2,0,delta*2.2)`, `msg7->isOver()` ->
  state 3 + the exhaust/visible/awake/`field_0x128=50000`/`enemyFlagsLo=1` loop.
  **Exact match.**
* state 3 (`0x1389d4`): `translate(delta*0.2,0,delta*2.2)`, `msg8->isOver()` ->
  state 4 + `setComputerControlled(false)`, `field_5e=0`, radar=1, `setCollide(true)`,
  `m_bCinematicBreak=0`, `setLookAtCam(false)`, `setTarget(player->geometry)`,
  `field_0x12=1`, `m_bStartSequence=0`.  **Exact match.**
* state 5 (`0x138fde..0x1390bc`): `getPosition`, `getSpeed`, the `speed==2.0f`
  block (play/update 157, play/update 158, stop music, play 161), update 161,
  `setSpeed(speed*0.98)`, `getSpeed`, `msg13->isOver()` -> `translate(25000,-200,-1000)`,
  `msg14->isTriggered()` -> timerA=0, state 6.  **Exact match.**

No content or statement-order difference was found in the mission-0 prologue.

## Finding 3 — the missing `translate(-7000,500,17000)` (open lead)

The original has **57** `TargetFollowCamera::translate` calls; ours has **55**.
Mapping the call sites by relative order shows the missing one is at original
`0x13a4b8`, in the body `0x13a442..0x13a4d4`:

```
13a43e  ldr.w r9,[sp,#84]          ; player
13a442  player->isInRocketControl()
13a448  cbz   ...
13a44a  resetCamera(m_pLevel)
13a454  player->setRocketControl(nullptr,nullptr)
13a45e  player->killLiberator()
13a464  m_pCamera->setRumblePercentage(0.0f, 0)
13a476  m_bCinematicBreak = 1; m_pHud->visible = 0; m_pRadar->enabled = 0
13a48a  m_pCamera->setLookAtCam(true)
13a490  m_pCamera->setPosition((*enemies)[0]->getPosition())
13a4b8  m_pCamera->translate(-7000.0f, 500.0f, 17000.0f)   <-- missing in ours
13a4bc  player->player->setVulnerable(false)
13a4c6  player->setFreeze(true)
13a4ce  m_nState = 2; m_nScriptTimerA = 0; m_nState++
```

The literal-pool words are the same `(-7000, 500, 17000)` as case-40 state 0
(`0xc5dac000`, `0x43fa0000`, `0x4684d000` at `0x13a64c..0x13a654`), but this is a
**different state body** (it sets `m_nState = 2`, not 1, and has the
rocket-control/killLiberator teardown).  Our source has only one
`translate(-7000.0f, 500.0f, 17000.0f)` (case 40 state 0, line 1922).  The owning
case/state was not identified in this pass — this is the concrete next lead for
the early/mid-case hypothesis.

## Other checks this pass

* Case 102 state 0 (`0x13a658..0x13a76a`): our source's
  `m_pCamera->translate(delta*0.6, delta*0.8, -delta*2.5)` **is** present in the
  original at `0x13aa9a` (the body is split by MBP), so it is not an extra call.
* Case 40 state 0 (`0x13a4d8..0x13a61e`): the transport setup, camera
  `setPosition`/`translate`/`setTarget`, `forceUpdate`, `m_nState=1` are all
  statement-faithful; the wave-130 case-40 corrections remain the right ones.
* Global call-multiset audit (`/dev/shm/w7689/calls.py`) confirms the known
  `.rel.plt` anchors (case-24 `setAutoTurret(autoTurretIsEnabled())`, case-125
  state-3 dead `delta<0` first-reference block) are the only extra named calls
  versus the original; they are deliberate and keep `.rel.plt` exact.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
python3 /dev/shm/w7689/score.py                         # 59.879643905
python3 /dev/shm/w7689/bytediff.py                      # process 54651
```

Scratch helpers (`common.py`, `score.py`, `bytediff.py`, `regions.py`,
`callseq.py`, `calls.py`, `callers.py`, `dump.py`, `tbh.py`, `reorder.py`,
`reorder2.py`, `reorder3.py`, `LevelScript.orig.cpp`) live in `/dev/shm/w7689/`
and are not part of the tree.
