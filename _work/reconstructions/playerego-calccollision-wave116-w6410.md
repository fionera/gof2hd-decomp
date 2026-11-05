# PlayerEgo::calcCollision — wave 116 (w6410): landed shape still the best; latch/RA
# coupling re-confirmed

**Status:** research note, 2026-11-05. Worktree `wt/w6410`, branch
`wave116-w6410`, base `d672d349f`. **No source change landed** — the
checked-in 87.6436781609 % / 968 B body (the "landed" body: nested
`Vector surface` block, `A1` = `if (i > objects->size() - 1) continue;`,
`A2` = `if (objects->size() == i) continue;`, no `i == 0` in the visible
arm) is still the best measured. This note extends
`playerego-calccollision-wave114-w6211.md` and `-wave115-w6310.md`
(and waves 107–113).

Target: `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE`
(orig 968 B, 347 normalised instructions, 87.6436781609 % exact).

## TL;DR

* Baseline re-verified: **87.6436781609 % / 968 B / 347-349** normalised
  instructions; `check` RATCHET PASS with byte **4501**, linked **4501**,
  size 4524, data 843/843, imports 207, lint CLEAN, drift 0.
* ~150 new builds this wave (all on the checked-in body or on the
  anchor-free / `M1` bodies) find **nothing above 87.6436781609 %**.
* The residual delta is exactly the one wave 114/115 described and it is
  a genuine two-way coupling:
  - **landed** (`A1` = `i > size-1`): original register file *and*
    original frame (112 B), but the loop latch is placed at `loc_c6`
    instead of `loc_2c0`, so every `continue` target differs (~20
    normalised instructions).
  - **`M1`** (`A1` = `i >= size`, no `A2`): original block order (latch
    at `loc_2c0`) at 968 B, but `r9`/`fp` swap (`i`=`r9`,
    `&dockOffsetVec`=`fp`; original is `i`=`fp`, `&dockOffsetVec`=`r9`).
* **New measurements** below confirm the mechanism and exhaust the
  remaining source-shape levers: surface-scope/placement, `i0` placement
  and spelling, steal-arm `continue` form (inner / outer / early
  `if (impact == zero) continue;`), loop-condition spelling, loop
  variable declaration form, direct member access instead of
  `Vector &pull` / `Vector &drift`, drift/pull hoisting, redundant
  checks at the loop end, and `__builtin_expect` were all re-tested.

## 1. Exact scores (this wave)

All sizes are 968 B unless stated; `lens` is orig/ours normalised
instructions. `i0` = the original's `if (i == 0 && object->field_0x71)`
guard in the visible arm (the landed body is missing it — the original
clearly has it: `cmp.w fp,#0 / bne / ldrb [r8,#113] / cbz / movs #1 /
strb [r5,#564]` at `0x9f374`).

```
landed (A1=gt, A2, nested surface, no i0)                 87.6436781609  968  347/349
M1      (A1=ge, no A2, nested surface, no i0)             87.3000000000  968  347/345
landed, early-continue steal arm (A1=gt, A2, no i0)       87.4000000000  968  347/349
M1, early-continue steal arm (A1=ge, no A2, no i0)        86.7000000000  968  347/345
L3      (A1=ge, A2, nested surface, no i0)                85.5000000000  976  347/348
landed + i0                                               85.0000000000  976  347/352
M1 + i0                                                   79.4000000000  976  347/348
A1=gt, no A2, nested surface, i0                          82.6000000000  968  347/348
A1=gt, no A2, nested surface, no i0                       79.5000000000  964  347/347
A1=gt, no A2, loop-scope surface, i0                      71.2000000000 1000  347/366
A1=ge, no A2, loop-scope surface, i0                      72.4000000000 1004  347/363
```

Anchor-free (faithful: no A1/A2, `i0` present, inner steal `continue`):

```
loop-scope Vector surface                                63.8-64.1 %   968  347/349
  + Vector &drift hoisted to loop scope                  72.7 %        968  347/349
  + Vector &drift hoisted to function scope              72.7 %        968  347/349
  + Vector &pull hoisted to loop scope                   64.1 %        968  347/349
nested { Vector surface ... } block                      59.8 %        996  347/352
  + drift hoisted                                        49.5 %        996  347/352
nested block + steal `continue` hoisted out of the if    68.3 %        956  347/344
```

(NB: wave 115 reported 72.4 % for the drift-hoist variant; this session
reproduces 72.7 %. The wave-115 "nested surface lands in r6" claim does
not reproduce on `body_af_nested` — there `r5` = `&temp(sp#32)` and
`r6` = `&__stack_chk_guard`.)

## 2. Why the landed shape is the local maximum

The landed shape reproduces the **original register file exactly**:

```
r4 = objects   r5 = this   r6 = &temp(sp#32)   r8 = object
r9 = &dockOffsetVec / strength   sl = distance   fp = i
frame = 112 B, surface shares sp#32 with the velocity / drift temps
```

and the **original frame size** (112 B). Its only structural deviation is
the loop latch address (`loc_c6` vs the original `loc_2c0`) plus the
missing `i0` guard. `ndiff` of the normalised streams shows the entire
delta is:

1. `str.w r9,[sp,#20]` scheduled one slot later in the prologue,
2. the latch block (7 instructions: `ldr [r4] / subs / cmp / bls /
   add i / b`) spliced between the `outerCollide` test and `getType`,
3. every `continue` therefore targeting `loc_c6` instead of `loc_2c0`,
4. the resulting ±8 B block-offset shifts (`loc_178` vs `loc_182`,
   `add r1,pc,#436` vs `#428`, …).

`M1` has the original block order but swaps `r9`/`fp`, so it loses more
instructions than the landed shape does (87.3 vs 87.6).

## 3. Mechanism (re-confirmed)

* `A1` = `i > objects->size() - 1` compiles to
  `ldr r0,[r4]; subs r0,#1; cmp i,r0; bls getType` — the **fall-through
  is the latch**. `MachineBlockPlacement::selectBestSuccessor` keeps the
  first successor on an exact 50/50 tie, so the latch is chained right
  after the `A1` block (`loc_c6`).
* `A1` = `i >= objects->size()` compiles to `ldr r0,[r4]; cmp i,r0;
  bcs latch` — the **fall-through is the body**, so the latch is chained
  last (`loc_2c0`, the original). But this spelling also flips the
  greedy allocator's `r9`/`fp` assignment.
* Any extra `objects->size()` compare in the wormhole arm (`A2`,
  folded into the mining test, `size == 0`, `size <= i`, `i >= size`)
  restores the original register file but costs 8 B (976 B).
* In the anchor-free shapes the GOT-loaded `&__stack_chk_guard` vreg is
  live across the whole function and gets `r6`; it is stored to
  `[sp,#24]` only near the exit. In the original (and in the landed
  shape) it is spilled to `[sp,#24]` in the prologue, freeing `r6` for
  `&temp(sp#32)`. The landed shape's `A1`/`A2` `objects->size()` loads
  are what tip that allocation.
* A nested `{ Vector surface ... }` block is required for the stack
  colouring to merge `surface` into `sp#32` (frame 112). Without it the
  faithful shape gets a 120 B frame and `surface` at `sp#104`
  (computed per iteration in `r8`). With it, BranchFolder tail-merges
  two one-instruction `b <latch>` tails (`BFCHANGED=1`), re-runs
  `buildCFGChains()` and **rotates the loop** (996 B, header at the
  function end). Hoisting the steal arm's `continue` out of the
  `if (impact != 0)` block avoids the rotation (956 B) but is 12 B short
  and scores 68.3 %.

## 4. Dead ends measured this wave

* Surface scope: loop / nested / function / direct inline temp /
  `Vector surface(...)` ctor form / `Vector surface; surface = ...`.
* `i0` placement: before `setPosition` 64.9/1004, before the visible
  check 76.3/988, after `hitCamera` 77.6/984, at loop top 63.6/988.
* `i0` spelling: `!i`, nested `if`, `object->field_0x71 && i == 0`,
  `i + 1 == 1` — all codegen-identical to the base.
* Steal-arm form: inner `continue`, outer `continue`, early
  `if (impact == zero) continue;` — best 87.4 (nested + A1=gt + A2).
* Loop condition: `i <= objects->size() - 1` 61.2/968,
  `objects->size() > i` 63.5, `i != objects->size()` 63.8,
  `++i` identical, `i` declared before the loop identical,
  `unsigned` vs `int` 63.8, `while` 58.5/980.
* Direct member access instead of `Vector &pull` / `Vector &drift`:
  codegen-identical (87.6 on the landed shape).
* Redundant `if (i >= size) continue;` at the end of the loop body:
  dead-code eliminated, no change.
* `else`-chain forms, `collision != Vector{...}` swap: identical / 87.4.

## 5. Conclusion

The original source has **no** redundant `objects->size()` checks (the
original is 968 B with `i0` present), yet its register file is the one
the landed body's anchors reproduce. No source shape found so far gives
both the original block order (latch last) and the original register
file at 968 B. 87.6436781609 % / 968 B stands.
