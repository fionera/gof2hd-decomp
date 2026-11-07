# LevelScript::process — wave 132 / w7705 (state-6/state-10 sound swap, case-40 state fix, size exact)

Status: **code change landed, 2026-11-07**. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**. Owned TU:
`src/game/world/LevelScript.cpp`. Base master `70e451ffe`.

## Result

```
                         before (master)   after (landed)
official verify pct      59.9              59.2
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
process .text diffbytes  54623             54507
whole .text diffbytes    85379             85235
.ARM.extab diffbytes     1420              1420
.ARM.exidx diffbytes     0                 0
.rodata / .data          0 / 0             0 / 0
.rel.plt diffbytes       0                 0
byte_exact               4506              4506
linked_exact             4506              4506
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
RATCHET                  PASS              PASS
```

The raw fuzzy drops 59.9 -> 59.2 because the corrected statements re-roll the global
`MachineBlockPlacement`/register colouring; the function's own differing-byte count improves
by 116 B and the whole `.text` by 144 B. This is the same trade-off recorded for wave 129.

## Landed changes (all verified against objdump of the original)

1. **Mission-0 state 6 (orig `0x139b70`..`0x139c96`) — the sound pair is swapped with state 10.**
   Our state 6 played `159`, zeroed the timer, then played `160`; the original plays `160`,
   *stops* `161`, and repositions the docking-bay geometry first:
   ```
   139c30  r0=7 ; str [sl,#28]          m_nState = 7
   139c36  ldr r4,[sl,#216]             m_pGeometry5
   139c40  getPosition(player)
   139c48  AEGeometry::setPosition(pos)  <-- was missing
   139c5e  SetAnimationState(3,nullptr)
   139c72  SetAnimationState(1,nullptr)
   139c7a  play(160)
   139c8c  FModSound::stop(161)         <-- was missing
   139c96  strd r8,r8,[r6]              timerA = 0
   ```
   The spurious `play(159)` and the first `m_nScriptTimerA = 0` are removed.

2. **Mission-0 state 10 (orig `0x13cfd0`) — `stop(161)` -> `play(159)`.**
   ```
   13cfd0  play(159,nullptr,nullptr,0.0f)
   13cff4  m_nState = 11
   13d002  m_pGeometry5->setPosition(player->getPosition())
   13d01c  SetAnimationState(1,nullptr)
   13d592  m_nScriptTimerA = 0
   ```
   `play(159)` therefore moves from state 6 to state 10, and `stop(161)` from state 10 to
   state 6 — a genuine sound-state bug pair.

3. **Case 40 state 4 (orig `0x13e78a`..`0x13e808`) — `speed = 0` -> `m_nState = 5`.**
   ```
   13e7fc  setPosition(0,0,-200000)
   13e804  KIPlayer::setActive(false)
   13e808  b.w 141b54   ->  movs r0,#5 ; b.w 1438d4  ->  str r0,[this+28]
   ```
   There is no store to `speed` in the original; the state machine advances to 5.

4. **Case 131 state 2 (w7687 semantic fix 1) — restore the `dir * 2.01` term.**  The source
   overwrote the `GetDirVector()` scratch with the right vector before using it; the original
   is `GetDirVector -> += dir*(delta*2.01) -> getRightVector -> += right*(delta*0.03) ->
   GetUpVector -> += up*(delta*0.01)`.

5. **Case 157 (w7687 semantic fix 2) — `m_pGeometry2->setVisible(false)`.**
   Original `0x14543a`: `ldr r0,[sl,#192]` (`this+0xc0`), not the `enemies[i]` KIPlayer.

6. **Case 125 state 2 — 3-float `setPosition` overload.**  Original `0x13b7dc` does
   `ldmia r3,{r1,r2,r3}`; source now spells `m_pCamera->setPosition(m_tempVec.x, m_tempVec.y,
   m_tempVec.z)` (the missing 8th `TargetFollowCamera::setPosition(f,f,f)` call).

7. **Case 125 shared tail — `translate(-delta*0.3, 0, 0)` for states 5..8.**
   Original `0x13c88c`: `subs r0,#5; cmp r0,#3; bhi` then
   `translate((float)-delta * 0.3f, 0, 0)`.

8. **`player->resetGunDelay()`** at the `setFixed(false)` body — the receiver is the cached
   `player` local, not a fresh `m_pLevel->getPlayer()`.  Semantically identical; the -4 B
   compensator that keeps the function at exactly 57232 B.

## The size / `.rel.plt` constraint (important for future waves)

The four w7687 edits alone measure **57228 B**, and — crucially — **they reorder `.rel.plt`**
(the first-reference order flips at index 2805: `RadioMessage::isTriggered` vs
`PlayerFixedObject::setMoving`).  A 57228-byte `process` therefore fails the ratchet
(`byte_exact 4506 -> 4498`) even before the symbol shift.  Both problems are fixed by landing
the set above at exactly 57232 B; the `.rel.plt` order is then identical to master.

Measured deltas on the pristine master (each edit alone keeps `.rel.plt` identical):

| edit | size | delta |
|---|---:|---:|
| case-131 state-2 reorder | 57224 | -8 |
| case-157 `m_pGeometry2` | 57228 | -4 |
| case-125 3-float setPosition | 57240 | +8 |
| case-125 shared translate | 57232 | 0 |
| state-6 faithful body | +8 (on the 4-edit base) | |
| state-10 `play(159)` | +8 (on the 4-edit base) | |
| `player->resetGunDelay()` | -4 (on the 4-edit base) | |
| case-40 `m_nState = 5` | -8 (on the 4-edit base) | |

Note the handoff's earlier table had `setpos` and `tail` swapped (setpos is +8, tail is 0 on
master); the w7687 note's "0 B / +8 B" labels were measured on a different base.

## Reproduction

```sh
cmake --build cmake-build-match --target check -j6          # RATCHET PASS, byte 4506
```
Scratch helpers in `/dev/shm/w7705/` (not tracked): `v4b.py` (subset sweep over the four
w7687 edits with `.rel.plt` comparison), `v5.py` (extra-edit sweep on the 4-edit base),
`vrel.py` (`.rel.plt` comparator), `build_size.sh`.
