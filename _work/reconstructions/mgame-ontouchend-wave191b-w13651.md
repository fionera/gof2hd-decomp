# MGame::OnTouchEnd — wave191b w13651: faithful arm-body / branch-layout reconstruction (note only)

Target: `_ZN5MGame10OnTouchEndEiiPv` in `src/game/menu/MGame.cpp` (original 8,080 B
@ `0x17a144`, Thumb, NDK r18b clang 7.0.2 `-Oz`). Worktree `wt/w13651`, branch
`wave191b-w13651`, base master `8dc75802b`. Scratch `/home/debian/GalaxyOnFire2/scratch/w13651`.

Prior notes: `mgame-ontouchend-wave191b-w13639.md` (non-134 cluster map, no win),
`-w13622.md` (campaign-134 store swap, landed), `-w13611.md` (diff-cluster map +
case-local reorder/hint sweep), `-w13600.md` (statement/declaration/chain-order sweep).

## Result

Base unchanged. `check` green (byte_exact 4508/4524, linked_exact 4508/4524,
size-exact 4524/4524, data_exact 843/843, imports 207, lint CLEAN, drift 0,
parity 5370/5370). `MGame::OnTouchEnd` raw **2,878**, aligned **1,582**,
size **8,080/8,080**. Whole-file `.text` differing bytes **71,196**,
`.ARM.extab` **577** (156,312 B), `.ARM.exidx` **0**, `.rodata` **0**.

No candidate beat raw 2,878. The base is a strict local minimum for raw across
every faithful arm-body order and every combination of them (1,023 measured
subsets). The residual is MachineBlockPlacement cold-block placement, not a
local statement-order error.

## Fresh evidence: the exact original campaign-arm order (from Thumb)

The original's `getCurrentCampaignMission()` compare order (ascending address) is

```
15, 22, 43, 42, 65, 81, 74, 95, 96, [126], 127, 134, 144, 155, 161, 162
```

Ours is the source order `74, 162, 15, 43, 42, 22, 65, 120, 96, 127, 100, 95,
110, 81, 134, 144, 126, 155, 161` (clang lays the chain out in source order).
Reordering the source chain to the original order reproduces the original arm
*order* exactly, but is **size-fatal**: 7,848 B (−232), because the Play block
then sits at the front of the chain and the shorter branch distances let the
Thumb2 size-reduction pass shrink ~232 B of 32-bit branches. The original's
8,080 B depends on the Play block sitting *inside* the chain (after the 43 arm),
which lengthens those branches. Raw 7,546 / aligned 2,461.

## Faithful arm-body orders (each measured alone on the base)

Decoded store/call order from the original disassembly. "Faithful" = the order
the original actually emits.

| arm | original order | variant | raw | aligned | size |
|---|---|---|---:|---:|---:|
| 65 | `switch=1`, getters 64/5c/60/68, `initStream=1`, new/loadStation(100)/depart/delete, active, module 2 | P65 | 2,890 | **1,552** | 8,080 |
| 100 | getters 64/5c/60/68, `initStream=0`, `switch=0`, depart(120), module 5, active | P100 | 3,621 | 1,565 | **8,048** |
| 110 | getters 64/5c/60/68, `switch=0`, `initStream=0`, depart(10), module 5, `enterSpaceLounge=1`, active | P110 | 2,904 | 1,566 | 8,080 |
| 126 | `switch=1` first, then getters, `initStream=0`, depart(120), active, module 2 | P126 | 2,881 | **1,560** | 8,080 |
| 162 | getters 64/5c/60/68, `switch=0`, `initStream=0`, depart(93), active, module 5 | P162 | 2,879 | 1,613 | 8,080 |
| 95 | `switch=1` first | P95 | 2,918 | 1,574 | 8,080 |
| 81 | `switch=1` first | P81 | 2,883 | 1,573 | 8,080 |
| back-to-station | `switch=0`, getters 64/5c/60/68, active, module 5 | PBT | 2,888 | 1,569 | 8,080 |
| 144 | `field_60`/`field_68` swap (unfaithful, aligned probe) | P144 | 2,882 | 1,776 | 8,080 |
| 134 | `field_5c`/`field_60` swap (already landed) | P134 | 2,878 | 1,582 | 8,080 |
| 161 | `switch=0` before `initStream=0` | P161 | 2,880 | 1,584 | 8,080 |

Arms 15, 43, 120, 144, 155 already match the original store/call order exactly.

## Exhaustive subset sweep (1,023 subsets of the 10 faithful-body patches)

Every subset of `{P126,P110,PBT,P95,P81,P65,P144,P134,P161,P162}` was built and
link-measured. Best raw remains **2,878** (base / `{P134}` alone); the best
aligned with raw ≤ 2,878 is the base itself (1,582). Best aligned overall is
`{P95,P81,P65,P144,P134}` at 1,490 but raw 2,939. No raw-tie/aligned-win exists
in this patch space.

## Instant-action branch layout (cluster 6)

The original is `if (isInstantActionMission()) {…; return;} else if
(isCampaignMission()) {…} else {reward}` with the instant body as fall-through;
ours is campaign-first with `instantAction` kept live. The faithful form (and
the faithful reward order `setFreelanceMission` before `showMissionRewardMessage`)
is locally right but shifts the whole pre-chain block layout:

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base (campaign-first) | **2,878** | 1,582 | 8,080 |
| instant-first + faithful reward order | 4,497 | 1,878 | 8,080 |
| instant-first (reward unchanged) | 4,467 | 1,878 | 8,080 |

## Mining-report cluster (`0x4a4`, 22 B) and `count` condition

The original rematerialises the `String("t ")` temp address *after* the first
`operator+`; ours hoists it before. Every expression spelling tried ties or
loses (all size-exact unless noted):

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base | **2,878** | 1,582 | 8,080 |
| single-line `+=` | 2,878 | 1,582 | 8,080 |
| `String("t ")` first (`countfirst`) | 2,896 | 1,592 | 8,080 |
| named `String unit("t ")` | 2,916 | 1,730 | 8,080 |
| right-grouped `(String("t ")+getText)` | 3,194 | 1,639 | 8,072 |
| wrap getText in `String(...)` | 7,580 | 2,953 | 8,008 |
| `count != 0` / `if (count)` / `if (amount)` | 6,637 | 2,188 | 8,088 |
| `count >= 0 && count != 0` / `count > 0` | 2,880 | 1,583 | 8,080 |

The original emits `cmp r9,#0; beq` (skip when `count == 0`); ours emits `bgt`.
The faithful `!= 0` form is +8 B (fatal); the size-exact `&&`/`> 0` forms are
+2 raw. The 1-byte branch-condition mismatch cannot be bought without a net loss.

## Other probes

| variant | raw | aligned | size |
|---|---:|---:|---:|
| `if (won)` inverted to `if (!won) … else …` | 2,878 | 1,582 | 8,080 (code-identical) |
| `__builtin_expect(soundsMuted,0)` on `else if` | 2,878 | 1,582 | 8,080 (code-identical) |
| `__builtin_expect(soundsMuted,1)` | 4,326 | 1,600 | 8,096 |
| `__builtin_expect(!IsCategoryEnabled(2) && !soundsMuted, 1)` | 4,411 | 1,667 | 8,084 |
| `__builtin_expect(!IsCategoryEnabled(2) && !soundsMuted, 0)` | 3,937 | 1,587 | 8,088 |
| move `if (menuTouchOpen)` before `if (cutsceneActive)` | 5,460 | 2,237 | 8,148 |
| faithful chain order (source reorder) | 7,546 | 2,461 | 7,848 |
| faithful chain order + all faithful bodies | 7,556 | 2,459 | 7,864 |

## `.ARM.extab` / `.ARM.exidx`

Every candidate kept `.ARM.extab` at 156,312 B and `.ARM.exidx` at 28,528 B.
The 577 differing `.ARM.extab` bytes are at `0x1a9ad7`.. (a different function's
LSDA range, not `MGame::OnTouchEnd` @ `0x17a144`), and are unchanged by every
candidate; `.ARM.exidx` is byte-identical (0 differing bytes).

## Conclusion

The 2,878-byte residual is dominated by MachineBlockPlacement's cold-block
ordering: the `movs r6,#0` weaponSlot=0 stub and the soundsMuted Play/Stop
blocks are placed after the pre-chain in ours but inside the campaign chain
(after the 43 arm) in the original. The campaign chain itself is byte-identical
in content; only its address is shifted (+180 B in `0x17b414..0x17b8c6`, +140 B
in `0x17bc1e..0x17bcfa`). Reconstructing every arm's store/call order faithfully
does not move those cold blocks and does not reduce raw. A further pass needs a
source shape that makes clang place the weaponSlot/Play cold blocks at the
original addresses while keeping the 8,080 B size, not more arm-body reorders.

## Reproduction

Scratch `/home/debian/GalaxyOnFire2/scratch/w13651`: `measure.sh` (build +
fn_diff + extab size), `blockmap.py`/`blockdelta.py`/`blocklcs.py` (basic-block
order maps), `callseq.py` (call-sequence diff), `sweep.py`/`sweep2.py`/
`sweep3.py` (faithful-body subset sweeps), `mine.py`/`minecond.py` (mining
expression/condition sweep), `hint.py` (soundsMuted hint sweep), `extabdiff.py`
(section byte diffs), `ndiff.py` (normalised instruction diff).
