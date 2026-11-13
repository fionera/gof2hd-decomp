# LevelScript::process — wave 191b / w13646: dispatch + remaining teardown clusters

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13646`, branch
`wave191b-w13646`, base master `4b2b1c10f` (raw **51,513**, aligned **22,174**,
`.text` 71,197, `.ARM.extab` 0x26298 = 156,312 B, `.rel.plt` md5
`289b2a069f8504d1e8a3919e43915e04`).

Second parallel angle: mission-state dispatch / branch polarity /
switch-lowering and the remaining teardown clusters, not the already-landed
case-70/105/102/14/42 blocks.  Every candidate below was compiled with the
exact NDK r18b `-Oz` command from `compile_commands.json` and gold-linked with
the checked-in object list (`ulimit -v 12000000`, `-j2`).

## Result: landed four independent teardown reorders

```
raw        51,513 -> 51,508  (-5)
aligned    22,174 -> 22,154  (-20)
size       57,232 / 57,232   (exact)
.text      71,197 -> 71,192  (-5)
.ARM.extab 577 (size/base exact, unchanged)
.ARM.exidx 0, .rodata 0 (unchanged)
.rel.plt   md5 289b2a06… (byte-identical)
check      RATCHET PASS (byte_exact 4508/4524, linked 4508, size 4524/4524,
           data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370)
```

All four are independent stores/loads:

1. **case 14 state 1** (~1614): `m_pHud->visible = 0;` moves before
   `m_pCamera->translate(1000.0f, 700.0f, 1000.0f);`.  The original emits the
   HUD store at `0x13aa44` and the translate at `0x13aa9a`, so this is also
   the faithful order.
2. **case 70 state 0** (~2491): `m_pRadar->enabled = 0;` swaps with
   `m_pHud->visible = 0;` (two plain flag stores).
3. **case 92 state 4** (~3048): `m_nScriptTimerA += delta;` moves after
   `wcamera->setTarget(leader);`.  The timer increment reads no camera state.
4. **case 105 supernova block** (~3918): `m_nScriptTimerA = 0;` moves before
   the `player->geometry->getDirection()` load.  The timer store does not
   touch the geometry.

## Mission-state dispatch / switch lowering

The dispatch structure is already correct at the branch level.  The one real
switch-lowering divergence is the mission-0 state chain:

```
original: cmp #4 / beq state4; cmp #3 / bne; state3 body; cmp #5 / bne;
          state5 body; subs #6; cmp #9; tbh (10 entries, states 6..15);
          cmp #16
ours:     cmp #5 / beq; cmp #4 / bne; subs #3; cmp #12; tbh (13 entries,
          states 3..15); cmp #16
```

Clang 7 `SimplifyCFG` folds the `else if (m_nState == 3)` arm into the
following switch.  Reordering the source arms to **4,3,5** reproduces the
10-entry `tbh` but costs **+8 B** and changes `.rel.plt`:

| candidate | raw | aligned | size | relplt |
|---|---:|---:|---:|---|
| baseline | 51,513 | 22,174 | 57,232 | `289b2a06` |
| arms 4,3,5 | 54,981 | 21,669 | 57,240 (+8) | `a8d49cee` |
| arms 4,3,5, nested state-5 `if` | 54,485 | 23,053 | 57,232 | `010848e2` |

Not landable under the size-exact / `.rel.plt`-identical gate.  The mission-1,
mission-78, mission-81/89/73/94/145 dispatches are byte-structure identical
(same range checks, same `tbh` entry counts, same fall-through polarity).

## Faithful teardown reorders measured (all size-exact unless noted)

These restore the original's exact Thumb call order but are *not* wins on
this base (the current source is a strong local optimum for raw):

| cluster | raw | aligned | size | relplt |
|---|---:|---:|---:|---|
| case 16 state 0 (full original order) | 51,538 | 23,731 | 57,232 | `289b2a06` |
| case 16 state 1 (full original order) | 53,935 | 21,760 | 57,240 | `289b2a06` |
| case 16 state 3 (full original order) | 51,524 | 21,771 | 57,232 | `289b2a06` |
| case 14 state 1 (full original order) | 51,566 | 21,779 | 57,232 | `289b2a06` |
| case 157 state 4 (full original order) | 51,533 | 21,761 | 57,232 | `289b2a06` |
| mission-0 state 3 body (full original order) | 52,600 | 21,795 | 57,232 | `289b2a06` |
| mission-0 state 16 handler (original order) | 51,519 | 21,795 | 57,232 | `289b2a06` |

## Sweeps (full function, 605-5374)

Fresh distance-1..6 transposition sweeps and 3-statement cyclic rotations on
the current base.  Every size-exact / `.rel.plt`-identical raw win:

| kind | lines | raw | safe? |
|---|---|---:|---|
| d1 | 890/891 | 51,512 | no — `setPosition(m_tempVec)` before the `+=` offset |
| d1 | **1614/1615** | **51,512** | **landed** |
| d1 | 2496/2497 | 51,511 | no — reverts the case-70 forceUpdate move (aligned +723) |
| d1 | **3918/3919** | **51,512** | **landed** |
| d1 | 4855/4856 | 51,512 | no — dead add (`m_tempVec` overwritten) |
| d1 | 4964/4965 | 51,512 | no — `setPosition` uses the un-offset vector |
| d2 | 1613/1615 | 51,512 | safe, but the 1614/1615 form is the better local optimum |
| d2 | 3235/3237 | 51,512 | no — right-vector reload before the add |
| d2 | 4964/4966 | 51,512 | no — camera reads the un-offset vector |
| d3 | 3025/3028 | 51,512 | no — `getDirection` past the `m_tempVec` add |
| d3 | 4614/4617 | 51,512 | no — `setPosition` before `m_tempVec` is rebuilt |
| d4 | **2491/2495** | **51,512** | **landed** |
| d4 | 3460/3464 | 51,512 | no — `getDirection` past the two `m_tempVec` adds |
| d5 | 3027/3032 | 51,512 | safe, but aligned **+1**; not landed |
| d6 | 872/878 | 51,512 | no — `field_0x1c = 156` must precede `PlayEngineSound()` |

## "Move one statement anywhere in the run" sweeps

Case 92 (2946-3201), case 157 (4796-5226) and case 154 (4516-4795):

| range | move | raw | safe? |
|---|---|---:|---|
| case 92 | **L3048 -> L3051** | **51,511** | **landed** (`m_nScriptTimerA += delta` after `setTarget`) |
| case 92 | L3155 -> L3164 | 51,512 | no — `v_dirVecA` is read before its new position |
| case 157 | L4855/L4856, L4964..L4966 | 51,512 | no — same dead-add shapes as above |
| case 154 | L4606 -> L4604 | 51,512 | no — drops the up-vector term from `m_tempVec` |

Case 92's L3048 move is the only one that improves both raw (-2) and aligned
(-12) on its own; combined with the three landed transpositions the final
measurement is raw **51,508**, aligned **22,154**.

## Scratch

Harness and sweep scripts: `/home/debian/GalaxyOnFire2/scratch/w13646/`
(`fast.py`, `sweep.py`, `movesweep.py`, `rotsweep.py`, `blockalign.py`,
`runs.py`).
