# Wave 159 merge tests (coordinator)

Base: `a72d115ea` (wave-158 docs + w10502 + w10504), gate green
(byte_exact 4507/4524).  Every candidate was applied to the base tree,
rebuilt with `link_so`, measured with `/dev/shm/fdiff.py` (raw same-address
bytes) and `_work/reconstructions/align_diff.py` (instruction-index
alignment), then reverted.  `git apply --3way` was used for the branch diffs
(`git diff cfde1f5f2 wave159-wNNNN -- src/`).

Baseline on the base tree:

| fn | raw | aligned | size |
|---|---:|---:|---:|
| `LevelScript::process` | 55,062 | 17,055 | 57,232 |
| `Radar::draw` | 8,124 | 1,320 | 9,908 |
| `Level::createShip` | 1,563 | 449 | 2,764 |
| `Level::createMission` | 5,487 | 774 | 17,172 |
| `MGame::OnUpdate` | 37 | 109 | 11,440 |

## Landed

* **w10503** (LevelScript case-154 state-9 `expect` + case-16 `m_nState++`):
  raw **55,062 -> 55,055** (-7), aligned **17,055 -> 17,059** (+4),
  size-exact.  Small raw win; the aligned metric is explicitly indicative
  for this 57 KB function.
* **w10509** (createMission: both-unnamed alien block, drop the redundant
  `p2`/`field_0x70`/`cargo` pre-store): raw **5,487 -> 5,652** (+165),
  aligned **774 -> 709** (-65), size-exact.  Structural win; the raw number
  is dominated by shift artefacts in a 17 KB function.
* **w10518** (MGame::OnUpdate: mission-122 `getCurrentCampaignMission`
  receiver `Globals::status` instead of the live `Status *st`): raw
  **37 -> 318** (+281), aligned **109 -> 19** (-90), size-exact.  The 19
  remaining mismatches are two real differences (see below) plus cascaded
  branch-target indices.

## Measured, not landed

| candidate | raw | aligned | size | why not |
|---|---:|---:|---:|---|
| w10505 (Radar duplicate `stationLockTimer` store + drop tail `String`) | 8,520 | **1,273** | 9,908 | beats w10504 on aligned (-47) but loses 396 raw; both size-exact |
| w10505 hunk 1 on top of w10504 | 8,532 | 1,319 | **9,920** | +12 B -> gate-fatal |
| w10510 (createMission `this=r4` hoisted bound) | 6,381 | 1,341 | 17,172 | worse than base on both metrics (a stepping stone, not a landing) |
| w10509 + w10510 (either order) | 6,558 | 1,436 | **17,156** | -16 B -> gate-fatal |
| w10517 (createShip `switch(type)` case order `0x31,0x2c,0x33`) | 1,560 | 460 | 2,764 | raw -3 but aligned +11 |
| w10503 on the pre-w10502 base | 55,087 | 17,133 | 57,232 | superseded by the w10502 landing |

## MGame::OnUpdate: the last 19

With w10518 applied, the only real differences are:

1. `ldr.w r2,[r9]` (orig) vs `ldr.w r5,[r9]` (ours) at the mission-122
   elapsed-time field read, plus the dependent `ldrd` base.  The original
   reloads `Globals::status` into r2; ours coalesces the load with the
   `Status *st` local in r5.
2. `b.w` straight to the join (orig) vs `mov r0,r5; b.w` (ours) at the end
   of the mission-122 block: ours reuses the live `st` (=r5) as the
   `getPlayingTime` receiver, the original reloads `[r9]`.

Sweep of the block's receivers (all size-exact unless noted):

| shape | raw | aligned | size |
|---|---:|---:|---:|
| `st` only for `getPlayingTime` (w10518) | 318 | **19** | 11,440 |
| `st` for radioBase + `getPlayingTime` (= base) | 37 | 109 | 11,440 |
| `st` for radioBase only | 914 | 109 | 11,436 |
| `st` for store + `getPlayingTime` | 5,201 | 757 | 11,488 |
| `st` for all three uses | 5,206 | 758 | 11,488 |
| `st` declared after `radioIndex` | 922 | 127 | 11,444 |
| `st` declared after `radioBase` | 908 | 125 | 11,444 |
| no `st` local at all | 5,148 | 597 | 11,392 |

The no-local shape shrinks the stack frame by exactly 48 B, so the original
frame really does contain the `st` local (or 48 B of equivalent locals);
the open problem is making the allocator spill/reload it instead of keeping
it in r5.  A follow-up should try declaring `st` in an enclosing scope, or
adding an unrelated 48 B local, and re-checking the r2/r5 tie.

## LevelScript declaration reorder on the post-w10502 base

The wave-158 lead (`messages,status,enemies,player` reorder, -172 raw /
-116 aligned at +60 B on the old base) does **not** transfer: on the
post-w10502 tree the same reorder is **-8 B** (57,224) and therefore
gate-fatal.  w10501's 120-cell permutation sweep (24 declaration orders x 5
`st->` receiver masks) found only size-exact cells that are *worse* than the
base in raw bytes (best 55,207 vs base 55,062), so no reorder landed.

## w10501 (late landing) does not combine with w10502

w10501's branch (based on `cfde1f5f2`) lands the w10403 declaration reorder
(`messages,status,enemies,player`) + all eight mission-106 state-6
`Globals::status` uses through `st` + two genuine 64-bit timer-zero restores
(`m_nScriptTimerB = 0` in c106 states 1 and 4).  On its own base that is
size-exact 57,232 with raw **55,094 -> 55,043** (-51) but aligned
**17,129 -> 17,873** (+744, the prologue re-colours `this` fp->r6 and spills
`&Globals::status`).

On the post-w10502/w10503 base the same shape is **57,276 (+44)** and
therefore gate-fatal.  Partial shapes on the current base:

| shape | raw | aligned | size |
|---|---:|---:|---:|
| reorder + all-8 `st` + timerB (w10501) | 54,387 | 16,920 | **57,276** |
| reorder + all-8 `st` + timerB, w10503 reverted | 54,385 | 16,916 | **57,276** |
| original decl order + all-8 `st` + timerB | 55,216 | 17,047 | **57,216** |
| original decl order + original `st` mask + timerB | 55,224 | 17,053 | **57,240** |

So the timerB restores alone cost +8 B on the current base; the reorder costs
+44 B.  Neither has a size-exact companion, and the reorder's aligned cost
(+744) is much larger than w10502's gain (-74).  Kept w10502; w10501's note is
`levelscript-process-prologue-sl-wave158-w10501.md`.
