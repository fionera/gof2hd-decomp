# LevelScript::process — wave 191b / w13631: case-92 state-4 camera block

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13631`, branch
`wave191b-w13631`, base master `4abbb1be8` (raw **51,513**, aligned 22,816,
`.text` 71,197, `.ARM.extab` 577, `.rel.plt` md5
`289b2a069f8504d1e8a3919e43915e04`).

## Lens

Chosen cluster: **case 92 state 4** (source L3046-3072), a contiguous
camera-follow block.  It is disjoint from w13599 (case-102 awake hoist) and
w13570 (case-14/42 teardown, mission-0 state-6 timer).

Fresh same-address diff map on this base: case 157 L4796-5226 (4,584 diff
bytes), case 113 L3587-3951 (3,389), case 93/92 L2946-3201 (2,939), case 156
L4516-4795 (2,881), case 104 L3310-3586 (2,810), case 90/80 L2623-2838
(2,225).  Distance-1 statement sweeps over each of those clusters found only
neutral (51,513) or worse cells; no size-exact raw win.  A single-statement
move sweep over case 92 state 4 found the win below.

## Original disassembly (state 4)

The original state-4 body is `0x13ff1c..0x140116` (entered after state 3's
`m_nState++` at `0x13fede`; state 5 starts at `0x14011a`).  Its statement
order is:

```
m_nScriptTimerA += delta;                 @0x13ff2e/0x13ff52
m_nScriptTimerB = delta*0.05 + timerB;    @0x13ff2a..0x13ff5e
leader = (*enemies)[0]->geometry;         @0x13ff62
local  = *CameraGetLocal(CameraGetCurrent()); @0x13ff70..0x13ff88
m_tempVec = MatrixGetPosition(local);     @0x13ff8c
m_dirVec  = leader->getDirection();       @0x13ffb2
m_tempVec += m_dirVec * (delta*1.7);      @0x13ffc0..0x13ffde
m_dirVec  = leader->getRightVector();     @0x13fff2
m_tempVec += m_dirVec * (-delta*0.03);    @0x13fffc..0x140026
m_dirVec  = leader->getUpVector();        @0x140030
m_tempVec += m_dirVec * (delta*0.05*0.004); @0x14003a..0x140054
setFixed(true);                           @0x14005c
MatrixSetTranslation(local, m_tempVec);   @0x140066
setLocal(local);                          @0x1400b6
for (i=0;i<3;i++) moveForward(speed*delta); @0x1400c6..0x1400f4
if (timerA > 2000) { state++; timerA = 0; } @0x1400f6..0x140116
```

Notable differences from our source (not landed, see table): the original has
**no `wcamera->setTarget(leader)` in state 4**, uses the fresh
`getDirection()` result for the first `+=` (our source has a dead
`m_dirVec = getDirection()` after the add), and calls `setLocal` before the
`moveForward` loop (ours calls it after).

## Landed change (two statement moves in the same block)

```cpp
                 } else if (m_nState == 4) {
                     TargetFollowCamera *wcamera = m_pCamera;
-                    m_nScriptTimerA += delta;
                     AEGeometry *leader = (*enemies)[0]->geometry;
+                    wcamera->setFixed(true);
                     AbyssEngine::PaintCanvas *canvas = Globals::Canvas;
                     wcamera->setTarget(leader);
+                    m_nScriptTimerA += delta;
                     Matrix local = *...CameraGetLocal(...);
...
-                    wcamera->setFixed(true);
                     AbyssEngine::AEMath::MatrixSetTranslation(local, m_tempVec);
```

`setFixed(true)` only stores `TargetFollowCamera::fixed`; `setTarget` stores
the target pointer; `m_nScriptTimerA += delta` is a plain 64-bit member add.
None of the intervening loads (`leader`, `Globals::Canvas`, `CameraGetLocal`)
read the fixed flag or the timer, and `CameraGetLocal` does not depend on
either store.  Semantics preserving.

Measured alone on the real build (exact NDK r18b `-Oz` compile + full gold
link):

```
raw        51,513 -> 51,509  (-4)
aligned    20,845 -> 20,870  (+25; mnemonic-sequence diff, same tool both sides)
size       57,232 / 57,232   (exact)
.text      71,197 -> 71,193  (-4)
.ARM.extab 577 (unchanged; size/base exact 156,312 B @ 0x1a3b78)
.ARM.exidx 0, .rodata 0, .data 0 (unchanged)
.rel.plt   md5 289b2a06… (byte-identical, 29,392 B)
check      RATCHET PASS (byte_exact 4508/4524, linked 4508, size 4524/4524,
           data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370)
```

## Candidate table (all size-exact unless noted)

| candidate | raw | size | note |
|---|---:|---:|---|
| baseline | 51,513 | 57,232 | |
| **landed: setFixed hoist + timerA after setTarget** | **51,509** | **57,232** | safe |
| same, + move `m_nScriptTimerA` after the `+= m_dirVec` add | 51,509 | 57,232 | safe, no further gain |
| move `MatrixSetTranslation` before `m_tempVec = MatrixGetPosition` | 51,508 | 57,232 | **unsafe**: uses stale `m_tempVec` |
| move `delete frame` before `CameraGetCurrent` (case-80 state 6) | 51,512 | 57,232 | **unsafe**: use-after-free |
| state-4 `setLocal` before the `moveForward` loop (faithful) | 53,280 | 57,240 | +8 |
| state-4 remove `setTarget` (faithful; original has none) | 52,857 | 57,224 | -8 |
| state-4 `getDirection` before the first add (faithful) | 52,866 | 57,224 | -8 |
| state-4 setLocal-before-loop + remove setTarget | 51,566 | 57,232 | faithful-ish, +53 raw |
| state-4 setLocal-before-loop + getDirection fix | 51,527 | 57,232 | faithful-ish, +14 raw |
| state-2 `setTarget` before `getPosition` (faithful) | 51,514 | 57,232 | +1 |
| state-2 `m_nState++` instead of `= 10` (original) | 51,581 | 57,232 | +68 |
| state-3 full faithful order (getUpVector before third add, setFixed late) | 51,583 | 57,232 | +70 |
| case-125 loop `awake()` before `getPosition()` (original) | 54,474 | 57,240 | +8 size |
| case-125 teardown full original order | 51,517 | 57,232 | +4 |
| case-125 state-0 `m_nScriptTimerA = 0` after `setRoute` (original) | 51,514 | 57,232 | +1 |

The faithful state-4 reconstruction is a net size change (-16 B) because the
original omits `setTarget` and our source has an extra dead `getDirection`
store; it cannot land without a compensating +16 reshape in the same block.
The landed pair is a codegen-coincidence win of the kind accepted by the
wave-190/191 notes: the gate is the same-address byte diff and the move is
semantics preserving.

## Sweeps that produced no size-exact raw win on this base

* d1 transpositions, case 157 L4796-5226, case 113 L3587-3951, case 92/93
  L2946-3201, case 156 L4516-4795, case 104 L3310-3586, case 80 L2623-2838,
  case 125 L3973-4103.
* single-statement move sweeps over case 125 (L3973-4103), case 80 state 6
  (L2654-2682), case 80 state 8/9 (L2738-2775), case 92 state 5 (L3072-3100),
  case 157 state 0/1 (L4796-4860): only the unsafe cells listed above beat
  51,509.

Scratch harness (`fast.py`, `sweep.py`, `movesweep.py`, `try*.py`,
`aligned.py`) lives in `/home/debian/GalaxyOnFire2/scratch/w13631/`.
