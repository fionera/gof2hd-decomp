# LevelScript::process — wave 168 / w11302: setPosition overloads, take 2

Target `_ZN11LevelScript7processEi` (orig `0x137d18`, 57,232 B), size-exact.
Worktree `wt/w11302`, branch `wave168-w11302`, base master `631c26f0b`.
Scratch `/dev/shm/w11302/`.

## Baseline (wave-167 w11201 head)

```
process raw 54,031  aligned 17,764  size 57232/57232  .rel.plt 0
byte_exact 4507 / size 4524 / imports 207 / lint CLEAN / drift 0
```

## Result landed

| shape | raw | aligned | size |
|---|---:|---:|---:|
| baseline | 54,031 | 17,764 | 57,232 |
| reorder + 885 + 3688 | **54,160** | 17,779 | **57,232** |

Changes:
1. `LevelScript.cpp:885` (case 12) — `setPosition(m_tempVec.x,y,z)` -> `setPosition(m_tempVec)`
   (Vector overload).  Original block `0x13d406..0x13d434` is now instruction-identical.
2. `LevelScript.cpp:3688` (case 6 tail) — `setPosition(*camPos)` ->
   `setPosition(camPos->x, camPos->y, camPos->z)` (fff overload).  Original block
   `0x13b7d2..0x13b7dc` matches.
3. `LevelScript.cpp:4905` (mission-78 state-5 exit) — reordered to the original's emitted
   order: `setVisible(true); setActive(true); setVulnerable(false); field_0x140 = 0;
   forceUpdate(delta,false); m_nScriptTimerA = 0; m_nScriptTimerB = 0; m_nState = 6;`.
   Original `0x144bcc..0x144c0e` emits exactly this order and `movs r0,#6` for the state
   store (so the source is `m_nState = 6`, not `m_nState++`).

Histogram after: TargetFollowCamera `setPosition(Vector)` 59/59 and `setPosition(fff)`
8/8 (was 59/58 and 8/9 before the reorder).

## The 4605 site is still blocked on +4

`LevelScript.cpp:4605` (state-6 exit) `setPosition(m_tempVec.x,y,z)` -> `setPosition(m_tempVec)`
is a genuine overload fix: the original block `0x1446b6..0x1446ce` is then
instruction-identical.  Measured on top of the landed shape it is **-4 B** (57,228) and
needs a +4 companion.  Candidates tested and rejected this pass:

* `m_nScriptTimerB = 0` at the state-6 block: +8 (57,236).
* `m_nState++` instead of `m_nState = 6`: 0 net (layout).
* `m_nState = 6; m_nScriptTimerA = 0;` in the reorder block (no timerB): -8.

Subset measurements on the reorder base (`/dev/shm/w11302/try2.sh`):

```
reorder only          57,232  54,074
reorder + 885         57,232  54,100
reorder + 3688        57,232  54,134
reorder + 4605        57,228  54,123
reorder + 885+3688    57,232  54,160   <- landed
reorder + all three   57,228  54,208
```

## Remaining lead: the +4

The original has 45 `strd rX,rX,[..,#144]` (m_nScriptTimerA = 0) sites; ours has 42 plus
9 `vst1.64` (orig 8).  At least one genuine `m_nScriptTimerA = 0` is missing (the reorder
block is one: orig `strd`, ours 12-byte vector store because of the extra timerB=0).
Finding one more 4-byte timerA-zero site would make the 4605 fix size-exact.  The
`forceUpdate` histogram is still 66/65; the missing call was not pinned (the state-5
block's forceUpdate is present in both).

## Reproduction

```sh
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
```
