# LevelScript::process — wave 134 / w7744 (case-70 state-1 state advance + state-2 body order)

Status: **code change landed, 2026-11-07**. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**. Owned TU:
`src/game/world/LevelScript.cpp`. Base master `ec0acea1e` (wave-133 head).

## Result

```
                         before (master)   after (landed)
official verify pct      58.8516           58.8566
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
orig / ours insns        20169 / 20044     20169 / 20044
process .text diffbytes  54465             54564   (offset-wise; see below)
whole .text diffbytes    85244             85242   (-2)
.ARM.extab diffbytes     1416              1416
.rodata/.data/.bss       0                 0
.ARM.exidx / .rel.plt    0 / 0             0 / 0   (byte-identical)
byte_exact               4506              4506
linked_exact             4506              4506
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
RATCHET                  PASS              PASS
```

Metric: `difflib.SequenceMatcher(None, onorm, unorm, autojunk=False).ratio()*100`
on `tools/verify/verify.py`'s reconciled streams. The `process .text diffbytes`
figure is an offset-wise byte comparison of the two function bodies; because the
two bodies have different internal block placement it is only a rough indicator —
the difflib ratio and the whole-`.text` count are the meaningful numbers.

## Landed change 1 — case 70 state 1: the state machine never advanced

Original state-1 tail (`0x13c156`..`0x13c16c`):

```
13c156  ldr r0,[r4,#24] ; ldr r0,[r0,#0] ; blx LODManager::forceUpdate
13c162  movs r0,#2
13c164  str  r0,[r4,#28]        ; m_nState = 2
13c166  ldr  r0,[sp,#84]        ; player
13c168  strb.w r8,[r0,#36]      ; player->freeze = 0
13c16c  b.w  tail
```

Our source had `m_pHud->visible = 1;` where the original stores `m_nState = 2`
(there is no `m_pHud` store anywhere in the state-1 body). This is a real
progression bug: without it case 70 stalls at state 1 forever (the following
`if (m_nState == 2)` block is only reached if something else sets state 2).
Replacing the spurious HUD store with `m_nState = 2;` is **size-neutral**.

## Landed change 2 — case 70 state 2: forceUpdate first, setRotate last

Original state 2 (`0x13ec3c`..`0x13ece8`):

```
13ec3c  forceUpdate(delta,false)             <-- top of the body, every frame
13ec4c  if (messages[2]->isOver()) {
13ec5c    setLookAtCam(false); setTarget(player->geometry);
13ec66    setComputerControlled(false); m_pHud->visible=1; m_pRadar->enabled=1;
13ec88    resetCamera(m_pLevel); forceUpdate(delta,false);
13ec9c    m_bCinematicBreak = 0; m_nState = 3;
          lead->setMaxHitpoints(enemies[0]->player->getMaxHitpoints()*3);
13ecda    enemies[0]->setSpeed(10.0f);
13ece2    enemies[0]->setRotate(5)          <-- LAST statement, inside the if
13ece8  }
```

Our source had `setRotate(5)` before the `isOver()` early-out and the trailing
`forceUpdate` at the bottom.  Reordered to the original.  **Size-neutral**
(57232 B), `.rel.plt` unchanged.  This re-lands the wave-132 `cf7ad5d72` fix
that `251d725ec` had reverted on master.

## Measured but NOT landed (all need a size companion)

| edit | size | pct | note |
|---|---:|---:|---|
| case 70 else-if chain (state 2 checked before state 1) | 57220 (-12) | 58.16 | faithful CFG; clang still emits `cmp #1` first |
| case 154 state 2 dead `m_dirVec` (GetDirVector after use) | 57228 (-4) | 58.67 | real bug; needs +4 |
| case 154 state 1 faithful tail | +8 | — | see below |
| case 125 state 2 tail `setFreeze(true)` after `setVisible(false)` | 57232 (0) | 58.8537 | faithful order, no pct win |
| case 125 state 0/1 `else if` (w7726 finding 4) | +24 | — | real progression bug |

### case 154 state 1 — three genuine bugs (net +8 B)

Original `0x13e956`..`0x13e97a` and `0x14445a`..`0x144520`:

```
if (messages[1]->isOver()) { m_nScriptTimerA += delta; }      // NOT setFixed
if (m_nScriptTimerA >= 1501) {
  for shipGroup==9 { setActive(false); setVisible(false); }
  setTarget(player->geometry);
  m_tempVec = player->getPosition();
  m_tempVec += {-350,400,-1500};
  setPosition(m_tempVec);
  setFixed(false);                                            // here, not in isOver
  m_nState++;
  forceUpdate(delta,false);
  m_nScriptTimerA = 0;
}
```

Our source instead calls `m_pCamera->setFixed(false)` inside the `isOver()` block
and omits `m_nScriptTimerA += delta`, and ends the timer block with a spurious
`(*enemies)[0]->setActive(true)` (the original has no `setActive(true)` in this
state at all).  The faithful body measures **+8 B**; pairing it with the -4 B
state-2 dead-assignment fix leaves +4 B, which no faithful companion was found
for in the time box.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
cmake --build cmake-build-match --target drift -j6      # 0 fields
```

Scratch helpers in `/dev/shm/w7744/` (not tracked): `score.py` (size/pct/
diffbytes), `parse.py` (address-range disassembly dump), `opcodes.py`
(difflib opcode map of the two reconciled streams), `orig.dis`/`ours.dis`.
