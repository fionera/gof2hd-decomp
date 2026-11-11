# MGame::OnTouchEnd — wave 168 (w11313): reward-chain 3-cycle sweep + epilogue analysis

Status: analysis only, no source change landed, 2026-11-11. Worktree
`/home/debian/GalaxyOnFire2/wt/w11313`, branch `wave168-w11313`, base
`631c26f0b`. Scratch `/dev/shm/w11313`. Target
`_ZN5MGame10OnTouchEndEiiPv`, original 8,080 B @ `0x17a144`.

Entry/exit state (unchanged): raw same-address diff **3,775**, aligned
**1,454**, size **8,080/8,080**, `.rel.plt` byte-identical (0), gate
byte **4507** linked **4507**, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, RATCHET PASS.

## Reward-chain order: 3-cycles

Wave-167 (w11212) left the source chain order at
`15,74,65,42,43,81,22,95,161,110,100,120,126,134,96,155,144,162`
and showed it is a local optimum for pairwise swaps and single-block
moves.  This pass ran the **full 3-cycle neighbourhood** (all
`C(18,3)*2 = 816` distinct 3-cycles) on top of that order: **no candidate
beats 3,775 raw**; every one is >= 3,775.  The landed order is therefore
also a 3-cycle local optimum.  (Scratch `hill.py`; each candidate is a
full MGame.cpp rebuild + `tools/fn_diff.py`.)

## Original block order (from the binary)

The original's reward-chain compare order in address order is

```
15, 22, 43, 42, 65, 81, 74, 95, 96, 100, 110, 120, 126, 127, 134,
144, 155, 161, 162
```

with `96` and `127` as two separate checks that share one body
(`cmp #96; beq body` at 0x17b718, `cmp #127; beq.w body` at 0x17bc30).
Our source keeps them as one `(96 || 127)` arm, so the compiler emits the
two compares adjacent; this is one residual structural delta.

Reordering the source chain to the original *address* order
(`15,22,43,42,65,81,74,95,96,100,110,120,126,134,144,155,161,162`)
gives raw **4,696**, aligned **943**, size **8,084** (gate-fatal +4).
So the original source order is *not* the address order; the placement
pass reorders it, and matching the address order alone is not a win.

## The shared epilogue at 0x17b004

Original: the canary-check/return block is at **0x17b004**; the
`hudTouchFlags == 0` body is cold at **0x17b01a** (immediately after the
epilogue), and the reward chain is cold from **0x17b106**.  Ours: the
main epilogue is at **0x17ba22** and the `hudTouchFlags == 0` check+body
is hot inline at **0x17aeb2** (right after the `useCloak` arm).

Measured shapes on the wave-167 base (all size unless noted):

| shape | raw | aligned | size | epilogue |
|---|---:|---:|---:|---:|
| wave-167 base | **3,775** | 1,454 | 8,080 | 0x17ba22 |
| `expect(hudTouchFlags == 0, 0)` | 5,922 | 1,603 | 8,076 | 0x17af90 |
| `expect(won, 1)` | 4,746 | 1,366 | 8,084 | — |
| arm order 0x800,0x200,0x400 | 6,083 | 1,616 | 8,064 | — |
| `expect(won,1)` + arm order | 4,245 | 1,227 | 8,076 | — |
| `expect(flags==0,0)` + arm order | 5,926 | 1,628 | 8,076 | — |

### The 4-byte shift at 0x17a7f2

`expect(hudTouchFlags == 0, 0)` produces the *right block structure*
(hot tail -> epilogue -> cold `flags==0` body) but the epilogue lands at
0x17af80/0x17af90, so the `b` at 0x17a7f2 is a 2-byte `b.n` instead of
the original 4-byte `b.w 17b004`.  That shifts the following literal pool
by -4 and makes every later byte differ (raw ~5.9k).

### bool `Mission::hasWon` + cold `flags==0` arm

Changing `Mission::hasWon` to return `bool` (original: `cmp r0,#0`; ours:
`cmp r0,#1`) and dropping the redundant `== true` / `!= false` makes the
`won` computation match the original exactly, and with
`expect(flags==0,0)` the epilogue moves to **0x17afec** (canary check),
only **0x18** short of 0x17b004.  But the `b` at 0x17a7f2 still targets
0x17afec (distance 0x7fa < 0x7fe) so it stays `b.n`; and the cold
`flags==0` body is placed at 0x17bf36, so the check is
`cmp; beq.w` (6 B) instead of the original `cbz` (2 B): size 8,084.
Raw 5,803.  The bool change alone is 6,042 raw / 8,076 B.

To flip 0x17a7f2 to `b.w` the epilogue must reach >= 0x17aff4, i.e. the
hot region needs **+8 bytes**.  The remaining gap is the cold-region
order: the original puts the `flags==0` body first (0x17b01a), then the
menuTouchOpen/mining/failed/reward blocks; ours puts the mining-else
block first (0x17b002) and the `flags==0` body last (0x17bf36).
Controlling that cold order (without a size change) is the next lever.

## `.rel.plt`

Still byte-identical (0 difference).  The OnTouchEnd-only first-use order
is `dockToStream, useCloak, maneuverTouchEnd, isInstantActionMission,
resetStartSequenceOver` (indices 3320..3324); every call site is in the
hot region before the reward chain, so chain permutations keep it intact.
`expect(won,0)` rotates 13 slots (wave-167 note), so avoid it.

## Reproduction

```
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5MGame10OnTouchEndEiiPv
cmake --build cmake-build-match --target check -j2
```

Scratch: `/dev/shm/w11313/{hill.py,reorder2.py,delta.py,dd.py,chain.py,
regions.py,try.sh,MGame.wave167.cpp}`.
