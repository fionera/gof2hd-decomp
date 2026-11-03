# MGame::OnTouchEnd (8080 B) — w3826: tail-merge direction is the whole layout

Target: `_ZN5MGame10OnTouchEndEiiPv` (Ghidra entry 0x18a144, file VMA 0x17a144, 8080 B).
Baseline (unchanged by this pass): **78.4%**, 8080 B, 2775 original / 2774 ours instructions.
Gate unchanged: byte_exact 4498 / linked_exact 4498, .text 0x1337c8, lint CLEAN, drift 0.

This pass landed **no code change** (every candidate below measured worse). It records the
mechanism that owns the remaining ~21% and the measurements that bound it.

## The single root cause

The `Hud::initHudMenu` tail merge. Original:

```
17adc0: ldrd r0,r2,[fp,#116] ; movs r1,#1 ; b.n 17afde     ; 0x200 case (branches forward)
...
17afd8: ldrd r0,r2,[fp,#116] ; movs r1,#2                  ; 0x400 case
17afde: blx Hud::initHudMenu                               ; shared tail, falls through
17afe2: ldr r0,[fp,#248] ; cbz r0,17b01a                   ; if (hudTouchFlags == 0)
```

Ours merges the other way: the 0x200 case at 0x17adc6 owns the `blx` (0x17adcc) and `b.w`s to
the test at 0x17b930; the 0x400 case at 0x17af72 branches *back* to it. The original places the
whole cluster `[0x400 body, shared blx, hudTouchFlags test, final tail, hudTouchFlags==0 body]`
contiguously at 0x17afd8..0x17b04e; ours splits it. That one placement cascades into ~1817
side-by-side diff lines (`verify.py --show`), while the normalized instruction multiset differs
by only ~15 real instructions.

## Which pass does the merge, and how it chooses

`-mllvm -enable-tail-merge=false` leaves each case with its own `blx` + `b.w <test>`, so the
merge is not in the IR. Instrumented MIR (`-mllvm -print-after-all`) shows the pre-MBP
`Control Flow Optimizer` still has **four** `initHudMenu` calls (13307/13496/13522/13541) — the
standalone post-PEI BranchFolder does not merge them. The merge is done by the BranchFolder that
`MachineBlockPlacement` runs internally with `AfterBlockPlacement=true`
(LLVM 7.0.2 `MBP.cpp` ~2792), i.e. *after* the first `buildCFGChains()`, and MBP then re-runs
`buildCFGChains()` because the merge changed the CFG.

Choice of the surviving tail (`BranchFolding.cpp`, LLVM 7.0.2):

* `ComputeSameTails` fills `SameTails` walking `MergePotentials` from the **end**, and
  `MergePotentials` is sorted by (hash, block number) ascending. So `SameTails[0]` is the
  **higher** block number and `SameTails[1]` the lower one.
* `CreateCommonTailOnlyBlock` picks `PredBB` (the layout predecessor of the merge target) if it
  is one of the tails; otherwise the smallest `EstimateRuntime(begin, tailStart)` with `<=`
  (i.e. ties go to the **later** `SameTails` index = the **lower** block number).
* Both candidates' prefixes are `ldrd r0,r2,[fp,#116]` (mayLoad → 2) + `movs r1,#N` (1) = 3, so
  the runtime estimate always ties. The decision is therefore purely
  `PredBB` / block-number order.

Measured block identities in our build (MIR after `Control Flow Optimizer`, pre-MBP):
0x200 body = `bb.390 (%ir-block.2428)`, 0x400 body = `bb.416 (%ir-block.2450)`, merge target
(test) = `bb.449 (%ir-block.2569)` with predecessors `bb.390, bb.414, bb.416, bb.421, bb.448,
bb.443, bb.444`. 0x200 < 0x400, and `PredBB` is the weaponSlot body (not a tail), so the tie
rule picks bb.390 → the 0x200 case keeps the `blx`. The original must have had either the 0x400
body at a lower block number or the 0x400 body as `PredBB` of the test block.

## Measurements (all reverted; baseline 78.4% / 8080 B)

| variant | result |
|---|---|
| swap 0x200/0x400 in the source chain | 78.2% / 8080 B — merge **flips** (0x400 owns the `blx`), but the emitted test order becomes 0x400-first |
| move the 0x200 case to the end of the chain | 73.4% / 8068 B — merge flips, test order wrong |
| move the 0x400 case to the end of the chain | 73.8% / 8068 B — merge does **not** flip |
| invert outer test: `if (!(flags & 0x200)) { ...rest... } else { initHudMenu(1); }` | 78.4% / 8080 B — clang canonicalises it back, byte-identical code |
| nested `else { if (wingmanCommand>=0) {...} if (hudTouchFlags==0) {...} }` | 75.1% / 8084 B — reproduces the original CFG (see below) but merge still does not flip |
| nested + faithful hud block | 74.9% / 8080 B |
| 3rd `getCampaignMission` call (fidelity fix) | 78.3% / 8080 B |
| mining `if (amount != 0) { ... String(-amount) ... }` | 71.4% / 8076 B — clang flips the counter to positive |
| faithful `hudTouchFlags == 0` block (below) | 76.6% / 8076 B — block becomes instruction-identical to the original, layout shifts |
| closeHudMenu-first only (keep null check) | 78.3% / 8080 B |
| faithful hud block + 3-call campaign | 76.5% / 8076 B |
| all three content fixes | 70.8% / 8072 B |

## Real CFG difference the source hides

Original: the **weaponSlot** body ends `b.w 17afe8` (jumps straight to the final tail, skipping
the `hudTouchFlags == 0` test); the **wingman** body ends `b.w 17afe2` (goes through the test);
the 0x800/0x1000 cases also go to 0x17afe2. Ours falls through from the weaponSlot body into the
test (0x17b92c → 0x17b930), so the test block has one extra predecessor and is chained with the
weaponSlot body — which is exactly why the merged tail cannot be chained in front of it.

The source shape that reproduces the original CFG is
`if (weaponSlot >= 0) {...} else { if (wingmanCommand >= 0) {...} if (hudTouchFlags == 0) {...} }`
(the "nested" row above). It is semantically identical and gets the CFG right, but the extra
`else` re-orders the IR blocks and costs 3.3 points.

## Content differences still present (multiset, literal-pool offsets masked)

1. Mining guard: orig `cmp.w r9,#0 ; beq` (tests `amount != 0`), ours `cmp.w r9,#-1 ; bgt`
   (`int count = -amount; if (count > 0)`). Every `!= 0` spelling makes clang 7.0.2 canonicalise
   the decrementing counter to an incrementing one (`sub.w r9,#1` → `add.w r8,#1`, `rsb` → `mov`)
   and shrinks the body to 8076 B.
2. Mission-success dispatch: orig calls `Status::getCampaignMission` **3×** (the `||` guard,
   `keepObjectives`, and the `mission` ternary); ours hoists `Mission *campaign` and calls it 2×.
   Re-fetching in the ternary reproduces 3 calls but scores 78.3%.
3. `hudTouchFlags == 0` block: original is unguarded and calls `closeHudMenu` **first**:

```
ldr r0,[fp,#116] ; blx closeHudMenu ; movs r4,#0 ; mov r0,fp
strb.w r4,[fp,#214] ; strd r4,r4,[fp,#152] ; strb.w r4,[fp,#93] ; blx resumeSounds
ldrb.w r0,[fp,#200] ; cmp r0,#0 ; beq <final tail>
ldr r0,[fp,#88] ; strb.w r4,[fp,#200] ; cmp r0,#0 ; beq <final tail> ; blx resetGunDelay ; b <final tail>
```

   Ours has `if (this->hud != nullptr)` and sets `hudMenuOpen = 0` before `closeHudMenu`.
   The exact source that reproduces the original block byte-for-byte is

```cpp
if (this->hudTouchFlags == 0) {
    this->hud->closeHudMenu();
    this->hudMenuOpen = 0;
    this->touch0Id = nullptr;
    this->touch1Id = nullptr;
    this->pauseOpen = 0;
    resumeSounds();
    if (this->_bc8) {
        this->_bc8 = 0;
        if (this->player != nullptr) {
            this->player->resetGunDelay();
        }
    }
}
```

   but landing it drops the function to 76.6% / 8076 B purely through layout.
4. Register allocation: orig `movs r4,#0` + `strb.w r4,[fp,#200]`, ours `movs r1,#0` +
   `strb.w r1,[fp,#200]`; orig `mov r1,r0` after `getMission`/`hasWon`, ours `mov r5,r0` then
   `mov r0,r5` (the hoisted `campaign` pointer).

## Suggested next attack

The merge direction only flips when the 0x400 body becomes the test block's layout predecessor
(or gets a lower MachineBasicBlock number). Both are decided inside MBP's `buildCFGChains` /
`hasBetterLayoutPredecessor`, so the lever is the *chain structure feeding the test block*: in
the original **no** block falls through into the test except the shared tail (weaponSlot jumps to
the final tail, 0x800/0x1000/wingman branch). The nested-`else` shape gets that CFG right but
re-orders the IR blocks; a shape that both keeps the original IR block order and removes the
weaponSlot→test fallthrough has not been found. Do not chase flags — the flag matrix is settled.
