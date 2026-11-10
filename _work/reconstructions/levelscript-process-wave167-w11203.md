# LevelScript::process — wave 167 / w11203: mission-0 state machine order + state-16 block

Target `_ZN11LevelScript7processEi` (original `0x137d18`, 57,232 B = 0xdf90),
size-exact.  Worktree `wt/w11203`, base master `d00057e6d` (wave 167).  Scratch
`/dev/shm/w11203/`.

## Baseline (HEAD, unchanged, tree left clean)

```
process raw diffbytes    54,095
process aligned insns    17,621
orig / ours size         57232 / 57232   (size-exact)
.rel.plt                 0        (BYTE-IDENTICAL)
byte_exact 4507 / linked_exact 4507
```

## Mission-0 state switch: physical body order

Original `tbh` @`0x139b22` (base `0x139b26`, 10 entries, states 6..15), decoded
from the actual bytes:

```
state  6 -> 0x139b70   state 10 -> 0x13cf76   state  7 -> 0x13d022
state  8 -> 0x13d13c   state  9 -> 0x13d1de   state 11 -> 0x13d2d4
state 12 -> 0x13d3b8   state 13 -> 0x13d440   state 15 -> 0x13d4c6
state 14 -> 0x13d4f4
```
Physical order **6, 10, 7, 8, 9, 11, 12, 13, 15, 14**; state 16 is the `bhi`
default (`0x13f906`, with a state reload at `0x13f902` for the case-15 path).

HEAD (source order 4,5,3) still lowers to a **13-entry** table (states 3..15)
at `0x139050`; physical order `9,15,6,7,8,10,11,12,13,14` after the inline
state-3 body.  The 4,3,5 source order gives the 10-entry table, but on this
base its physical order is `6,7,8,9,10,13,15,11,12,14` — **not** the
original's `6,10,7,8,9,11,12,13,15,14`.  So the w11102 claim that 4,3,5
reproduces the original physical order does not hold on the wave-167 base.

## Hoisted common tails (original)

The original shares case tails (BranchFolder), and the layout groups them:

| body | tail target | shared tail |
|---|---|---|
| case 7 (state 8) | `0x13d4ba` (inside case 13) | `m_nState = X; m_nScriptTimerA = 0;` |
| case 13 (state 14) | falls through `0x13d4ba` | same |
| case 12 (state 13) | `0x13d4ec` (inside case 15) | `m_nState = X;` |
| case 15 (state 16) | falls through `0x13d4ec` | same |
| case 10 (state 11) | `0x13d592` (inside case 14) | `m_nScriptTimerA = 0;` |
| case 14 (state 15) | falls through `0x13d592` | same |
| state 4 body | `0x13d4ec` (case 15's store) | `m_nState = 5;` |

In HEAD the state-4 body instead shares case 12's store; in the 4,3,5 shape it
also shares case 12.  The tail owner is layout-dependent, so matching the
original's tail grouping needs the original's body order, not just source
order.

## Real bug found: mission-0 state-16 block

Original `0x13f902`..`0x13f94e` + `0x13b156`..`0x13b15e`:

```
if (m_nState == 16) {
    if (!Globals::layout->isFading()) {
        Globals::layout->enableFillScreen(true);      // 0x13f924  (MISSING in HEAD)
        Globals::status->nextCampaignMission(true);   // 0x13f92e
        Globals::switch_to_target_setting = 1;        // 0x13f940
        Globals::appManager->SetCurrentApplicationModule(2); // 0x13f944
        Level::initStreamOutPosition = 0;             // 0x13b158
        return 0;
    }
}
```

HEAD has **no `enableFillScreen(true)`** and instead an **extra
`m_nScriptTimerA += delta;`** (there is no 64-bit add to `this+0x90` anywhere
on the state-16 path in the original).  Fixing both makes the block
instruction-for-instruction identical (22 insns, 76 B + 8 B tail), but the
function becomes **57,224 B (-8)** — fatal on its own.  The original therefore
has an 8-byte chunk elsewhere that HEAD's spurious timer add was accidentally
compensating.

Measured:

| shape | raw | aligned | size |
|---|---:|---:|---:|
| HEAD | 54,095 | 17,621 | 57,232 |
| state-16 fix (enableFillScreen, no timer) | 54,540 | 17,617 | 57,224 |
| state-16 fix + `Status *volatile` | 54,393 | 18,434 | **57,232** |
| state-16 fix + timer add (HEAD order) | 54,858 | 17,623 | 57,240 |
| state-16 fix + 4,3,5 | 54,995 | 18,317 | 57,224 |
| state-16 fix + 4,3,5 + timer add | 55,342 | 18,322 | 57,240 |

`Status *volatile status = Globals::status;` reproduces the original's
`ldr r0,[sp,#92]; ldr r0,[r0,#0]` status reload (the original spills
`&Globals::status` to `[sp,#92]`; HEAD keeps it in `r9`), and is the only
size-exact combination found.  It is still **+298 raw** vs HEAD, so it was
not landed (raw must not worsen).

## Arm permutations (HEAD base, no other change)

All six source orders of the 4/5/3 arms were built:

| order | raw | aligned | size | tbh |
|---|---:|---:|---:|---|
| 4,5,3 (HEAD) | 54,095 | 17,621 | 57,232 | 13 (3..15) |
| 4,3,5 | 55,384 | 18,323 | 57,240 | 10 (6..15) |
| 3,4,5 | 55,333 | 18,310 | 57,248 | — |
| 3,5,4 | 54,531 | 17,516 | 57,232 | — |
| 5,3,4 | 55,473 | 19,934 | 57,280 | — |
| 5,4,3 | 55,473 | 19,934 | 57,280 | — |

No permutation beats HEAD on raw.  Case-label order inside the switch does
not change the emitted layout (all permutations byte-identical), so the body
order is MachineBlockPlacement-driven, not source-order-driven.

## `__builtin_expect` sweep on the mission-0 switch (no wins)

All case-level `if`s (lines 727/730/739/763/789/794/805/809/838/842/855/865/
882/895/909/919/929/930) were hinted 0 and 1.  Best results: line 789 e=1,
809/842/865/739/763/805/838/855 all neutral (== 54,095); everything else
worse or size-changing.  Line 767 e=1 (case 6 message) = 54,168 (size exact).
So the switch layout is not recoverable from branch weights.

## Conclusion

* Original mission-0 switch: 10-entry table, physical order
  `6,10,7,8,9,11,12,13,15,14`, shared tails `7->13`, `12->15`, `10->14`,
  state-4->15.
* HEAD's 13-entry table and `9,15,6,...` order are a real structural
  mismatch, but the 4,3,5 swap alone is a raw regression (+1,289) and +8 B.
* Genuine source bug: mission-0 state 16 is missing
  `Globals::layout->enableFillScreen(true)` and has a spurious
  `m_nScriptTimerA += delta;`.  Fixing it is -8 B (fatal) because HEAD is
  compensating for a missing 8-byte chunk elsewhere in the function.
* Nothing landed; tree left clean.
