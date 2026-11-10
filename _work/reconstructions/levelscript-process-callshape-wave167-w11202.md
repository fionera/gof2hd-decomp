# LevelScript::process — wave 167 / w11202: call-shape pinning (setPosition overloads)

Target `_ZN11LevelScript7processEi` (orig `0x137d18`, 57,232 B), size-exact.
Worktree `wt/w11202`, branch `wave167-w11202`, base master `d00057e6d`.
Scratch `/dev/shm/w11202/` (`callseq.py`, `ctx.py`, `sites.py`, `match2.py`,
`match3.py`, `fprev.py`, `fnext.py`, `fblock.py`, `mission.py`, `fbool.py`).

## Baseline

```
process raw 54,095  aligned 17,621  size 57232/57232  .rel.plt 0
byte_exact 4507 / size 4524 / imports 207 / lint CLEAN / drift 0
```

## Histogram after w11103 (the wave-166 table is stale)

The w11103 expect sweep changed block layout, so `AEGeometry::moveForward` is
**no longer +1** (40/40).  Current deltas inside `process` (objdump count,
`--start-address=0x137d18 --stop-address=0x145c88`):

| callee | orig | ours | delta |
|---|---:|---:|---:|
| `AEGeometry::setPosition(const Vector&)` | 23 | 24 | +1 |
| `LODManager::forceUpdate(int,bool)` | 66 | 65 | -1 |
| `Level::getStarSystem()` | 15 | 16 | +1 |
| `Level::getPlayer()` | 23 | 24 | +1 |
| `PlayerEgo::getPosition()` | 41 | 42 | +1 |
| `Status::getStanding()` | 1 | 2 | +1 (anchor) |
| `Standing::setStanding(int,int)` | 1 | 2 | +1 (anchor) |
| `PlayerEgo::setAutoTurret(bool)` | 2 | 3 | +1 (anchor) |
| `PlayerEgo::autoTurretIsEnabled()` | 1 | 2 | +1 (anchor) |

`TargetFollowCamera::setPosition(const Vector&)` and
`TargetFollowCamera::setPosition(float,float,float)` are **both 59/8 and equal**
at HEAD (the wave-166 +1/-1 pair is a source-shape bug, see below).

## Genuine source-shape bugs found (setPosition overload)

Three sites in the source use the wrong overload relative to the original.
Each was verified by matching the *whole basic block* (constants + call order),
not just the call sequence.

### 1. `LevelScript.cpp:4604` — fff should be Vector (this is the -1/+1 pair)

Original block at `0x1446b6..0x1446ce` (literal pool `0x1448bc`=4000,
`0x1448c8`=42000):

```
ldr r1,[pc] / ldr r0,[pc] ; {4000,42000}
strd/str -> stack vector
blx VectorpLE
ldr r0,[sl,#20]           ; m_pCamera
mov r1,r6                 ; &m_tempVec
blx TargetFollowCamera::setPosition(const Vector&)
```

Ours at `0x144586..0x14459c` instead does
`add.w r3,r4,#40; ldmia r3,{r1,r2,r3}; blx setPosition(float,float,float)`.

Fix: `m_pCamera->setPosition(m_tempVec);`
Measured (full link): raw 54,095 -> **54,110**, size 57,232 -> **57,228 (-4)**.
This removes the extra fff call (9->8, matching orig) and the block becomes
instruction-for-instruction identical to the original.  **Not landed: the -4
is gate-fatal and no +4 compensation was found this pass.**

### 2. `LevelScript.cpp:885` — fff should be Vector (size-neutral)

Original block `0x13d406..0x13d434` uses `setPosition(const Vector&)` with
`{-2000,-2000,-5000}` (literal `0x13d6ec/0x13d6f0`).  Ours uses fff.
Fix `m_pCamera->setPosition(m_tempVec);` is size-neutral (57,232) but drops
the fff count to 7, so it must be paired with #3.

### 3. `LevelScript.cpp:3687` — Vector should be fff (size-neutral)

Original block `0x13b7d2..0x13b7dc` does
`add.w r3,sl,#40; ldmia r3,{r1,r2,r3}; blx setPosition(fff)` then
`m_nScriptTimerA=0; m_nScriptTimerB=0`.  Ours emits the Vector overload.
Fix `m_pCamera->setPosition(camPos->x, camPos->y, camPos->z);` restores the
fff count but is size-neutral (the address is already in a register, so
`mov r1,rX` and `ldmia` are the same 6 bytes).

**Correct combination** (#1+#2+#3): fff 8/8, Vector 59/59, block shapes
match, but size **57,228 (-4)** and raw 54,209.  The -4 is exactly #1's
`add+ldmia` (6 B) vs `mov r1` (2 B).

## The missing `forceUpdate` (+4 candidate)

`forceUpdate` is 66 (orig) vs 65 (ours).  Block-level and prefix/suffix
matching both point at orig `0x144c02` (block `0x144bd0..0x144c0e`:
`setVisible(true); setActive(true); setVulnerable(false); field_0x140=0;
forceUpdate(delta,false); m_nScriptTimerA=0; m_nState=6`).  But ours has the
same source block at `0x141b5a..0x141b7c` (forceUpdate at `0x141b62`,
`field_0x140=0` at `0x141b70`, `m_nState++`), so that is not the missing call.
Adding a `forceUpdate` after `LevelScript.cpp:4908` gives size 57,252 (+20),
so a forceUpdate is not a 4-byte compensation either.

## Landed

Nothing (source reverted; tree clean).  HEAD remains the best size-exact
shape (raw 54,095).  To land the #1 block fix, a further +4 B of original
code must be found (the `forceUpdate` -1 is the prime suspect but was not
pinned).

## Reproduction

```sh
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
python3 /dev/shm/w11202/callseq.py
python3 /dev/shm/w11202/sites.py '_ZN18TargetFollowCamera11setPositionEfff@plt'
```
