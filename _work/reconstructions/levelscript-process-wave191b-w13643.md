# LevelScript::process — wave 191b / w13643: mission-state dispatch / branch-polarity / switch-lowering pass

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13643`, branch
`wave191b-w13643`, base master `cb3afdf0f` (raw **51,513**, aligned **22,816**,
`.text` 71,197, `.ARM.extab` 0x26298 = 156,312 B, `.rel.plt` md5
`289b2a069f8504d1e8a3919e43915e04`).

Third parallel angle: the per-mission **state dispatch**, branch polarity and
`tbh` switch lowering, not the already-landed case-102 / case-14 / case-42
blocks.  Every candidate below was compiled with the exact NDK r18b `-Oz`
command from `compile_commands.json` and gold-linked with the checked-in
object list (`ulimit -v 12000000`, `-j2`).

## Result: no raw win; note-only

The dispatch structure is already correct at the branch level.  The one real
switch-lowering divergence (mission-0 `switch (m_nState)` below) cannot be
reproduced without a size or raw regression.  `src/` is unchanged.

## Mission-0 state dispatch — exact original order (Thumb, original)

```
137e8c  ldr.w   r1, [sl, #28]          ; m_nState
137e90  cmp     r0, #0                 ; introMsg->isOver()
137e92  beq.w   1383fe
137e96  cmp     r1, #0
137e98  bne.w   1383fe                 ; else if (m_nState <= 2)
...
1383fe  cmp     r1, #2
138400  bgt.w   1389c8                 ; m_nState > 2 -> 4/3/5 chain
...
1389c8  cmp     r1, #4
1389ca  beq.w   138d2a                 ; state 4
1389ce  cmp     r1, #3
1389d0  bne.w   138fd8                 ; state 3 fall-through body at 1389d4
138fd8  cmp     r1, #5
138fda  bne.w   139b1a                 ; state 5
139b1a  subs    r0, r1, #6
139b1c  cmp     r0, #9
139b1e  bhi.w   13f906                 ; states outside 6..15
139b22  tbh     [pc, r0, lsl #1]       ; 10 entries: states 6..15
...
13f906  cmp     r1, #16
13f908  bne.w   137f7c                 ; state 16 handler
```

So the original order is **state 4, state 3, state 5, then a 10-entry
`tbh` on states 6..15**, with the state-16 test after the table.

Our baseline emits:

```
1389b0  cmp     r1, #5
1389b2  beq.w   138d88                 ; state 5 first
1389b6  cmp     r1, #4
1389b8  bne.w   139048                 ; state 4
139048  subs    r0, r1, #3
13904a  cmp     r0, #12
13904c  bhi.w   13d598                 ; states outside 3..15
139050  tbh     [pc, r0, lsl #1]       ; 13 entries: states 3..15
```

Clang 7 `SimplifyCFG` folds the `else if (m_nState == 3)` arm into the
following `switch`, producing a 13-entry table covering 3..15 instead of the
original's separate state-3 arm plus a 10-entry 6..15 table.  This is the
documented w12702/w12802 divergence; it is still present on this base.

## Candidate measurements

Baseline: raw **51,513**, aligned **22,816**, size **57,232/57,232**,
`.ARM.extab` **0x26298** (156,312 B), `.rel.plt` byte-identical.
All candidates below are `.rel.plt`-identical; `.ARM.extab` sh_size stayed
`0x26298` for every one (content not separately re-measured except where
noted).

| # | candidate | raw | aligned | size | extab |
|---|---|---:|---:|---:|---:|
| 0 | baseline | 51,513 | 22,816 | 57,232 | 0x26298 |
| 1 | `default: break;` in mission-0 switch | 51,513 | 22,816 | 57,232 | 0x26298 |
| 2 | `default: break;` in mission-1 switch | 51,513 | 22,816 | 57,232 | 0x26298 |
| 3 | state-3 arm as nested `if` inside `else` | 51,513 | 22,816 | 57,232 | 0x26298 |
| 4 | state-3 arm as `case 3` in an outer switch | 51,513 | 22,816 | 57,232 | 0x26298 |
| 5 | state-3 arm moved after the switch | 54,721 | 23,945 | 57,248 (+16) | 0x26298 |
| 6 | `else if (m_nState >= 6)` before switch | 55,238 | 25,031 | 57,256 (+24) | 0x26298 |
| 7 | arms reordered **4,3,5** | 54,981 | 22,713 | 57,240 (+8) | 0x26298 |
| 8 | arms reordered **3,5,4** | 52,914 | 22,877 | 57,232 | 0x26298 |
| 9 | arms reordered **5,4,3** | 55,189 | 24,508 | 57,224 (-8) | 0x26298 |
| 10 | arms reordered **5,3,4** | 55,113 | 22,927 | 57,248 (+16) | 0x26298 |
| 11 | state-3 `__builtin_expect(...,0)` | 54,877 | 24,050 | 57,236 (+4) | 0x26298 |
| 12 | state-3 `__builtin_expect(...,1)` | 55,285 | 24,492 | 57,260 (+28) | 0x26298 |
| 13 | state-5 `__builtin_expect(...,0)` | 55,362 | 24,024 | 57,248 (+16) | 0x26298 |
| 14 | state-5 `__builtin_expect(...,1)` | 55,569 | 24,451 | 57,176 (-56) | 0x26298 |
| 15 | mission-0 state0 `m_nState=1` before setPosition | 51,517 | 22,810 | 57,232 | 0x26298 |
| 16 | mission-0 state0 `m_nState=1` before both setters | 51,517 | 22,810 | 57,232 | 0x26298 |
| 17 | mission-0 state0 `m_nState=1` between setters | 51,514 | 22,818 | 57,232 | 0x26298 |
| 18 | mission-1 `m_nState=2; setTarget; setPosition` | 51,514 | 22,806 | 57,232 | 0x26298 |
| 19 | mission-1 `m_nState=2; setPosition; setTarget` | 51,514 | 22,814 | 57,232 | 0x26298 |
| 20 | chain-A `(float)(delta * -2)` / `-(delta<<1)` | 51,514 | 22,806 | 57,232 | 0x26298 |
| 21 | remove `Status *status` local (use `Globals::status`) | 51,514 | 22,815 | 57,232 | 0x26298 |
| 22 | `if (m_bStartSequence > 0)` | 51,513 | 22,816 | 57,232 | 0x26298 |
| 23 | mission-0 `m_nState == 0 && isOver()` operand swap | 55,797 | 28,083 | 57,440 (+208) | 0x26298 |

### The one structurally-correct candidate

Candidate 7 (arm order **4,3,5**) is the only one that reproduces the
original's 10-entry `tbh` on states 6..15 (`0x13b1d2 subs r0,r1,#6;
cmp r0,#9; tbh`), and it improves aligned by **-103**.  But it costs
**+8 bytes** (57,240) and raw regresses by **+3,468**, so it is not landable
under the size-exact / raw-must-not-regress gate.

Candidate 8 (arm order **3,5,4**) is size-exact and aligned **+61**, but raw
**+1,401**.  Candidate 9 is the only size-*shrinking* variant (-8) but raw
**+3,676** and aligned **+1,692**.

## Branch-polarity census

The dispatch compares were checked against the original at every site:

* campaign switch guard `0x1387a0`: `getMission()->isCampaignMission()`,
  `isEmpty()`, then `getCurrentCampaignMission() == 42` — identical order and
  polarity in both.
* campaign switch range prongs: `sub #131; cmp #14; bls tbh` then
  `sub #64; cmp #9; bls tbh` — identical, and the same 15 `tbh` sites exist
  in both bodies.
* linear compare chain order 14,16,21,24,29,40,41,42,56,80,91,92,94,102,105,
  114,125,154,157,158 — identical.
* mission-1 state dispatch (`0x13866a` original / `0x138648` ours): 5-entry
  `tbh` on states 1..5, identical range check and polarity.
* mission-78 state dispatch (`0x138384` original / `0x138328` ours): 12-entry
  `tbh` on states 1..12, identical range check and polarity; the original's
  `bhi.w 13c88c` shared epilogue is the same block our clang folds directly
  to the shared active tail (semantically equivalent for states outside
  1..12).
* mission-81, mission-89, case-73, case-94, case-145 dispatches: same range
  check, same `tbh` entry count and same fall-through polarity.

The residual dispatch bytes are branch-target offsets (block placement) and
the `this`/`status` callee-saved register swap (`this` in `fp` vs `sl`,
`Globals::status` in `sl` vs `fp`), which the w13570 note already established
as the dominant raw-diff contributor and which no declaration/order reshape
tried here flipped.

## Scratch

Harness and disassembly dumps: `/home/debian/GalaxyOnFire2/scratch/w13643/`
(`diffmap2.py`, `branchdiff.py`, `orig.dis`, `process.dump`).
