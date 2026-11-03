# MGame::OnTouchEnd (8080 B) — w3807 pass

Target: `_ZN5MGame10OnTouchEndEiiPv` (Ghidra entry 0x18a144, file VMA 0x17a144, 8080 B).
Before: 78.0901% (HEAD `41804126`, after `ca43091c`). After: **78.4%** (8080 B, 2775/2774 insns).
Gate unchanged: byte_exact 4498 / linked_exact 4498, .text 0x1337c8, lint CLEAN, drift 0.

## What landed
Reverted the `ca43091c` OnTouchEnd hunk (it reordered the mission-success dispatch to put
`isInstantActionMission()` before the campaign `nextCampaignMission`/`showMissionRewardMessage`
dispatch and fetched the campaign mission per call). The pre-`ca43091c` shape — hoist
`Mission *campaign`, compute `keepObjectives`, pick `mission`, then campaign dispatch, then
instant-action — scores 78.4%. Also hoisted `bool failed = ...->hasFailed(); if (failed)` to
match the original's `r4` bool (byte-identical codegen, kept for shape fidelity).

The emitted order is *not* the source order: with campaign-dispatch-first source, clang still
emits `isInstantActionMission` first (0x17b008) and the campaign dispatch after (0x17b03c),
matching the original 0x17af7e / 0x17afa6.

## Instruction-level findings (original vs ours)
The function is content-correct: the normalized-instruction multiset differs by only 67/66
instructions out of 2775, and every remaining diff is block placement / register allocation /
literal-pool offsets. Verified block-for-block matches (identical except addresses):
- cutscene `hasFailed` prologue 0x17ab08–0x17ab4c (r4 bool, `getMission` reload, `cmp r4,#0`).
- mission-success dispatch 0x17af5a–0x17afbe (`getMission`/`hasWon`/`isInstantActionMission`/
  `isCampaignMission`/`nextCampaignMission`).
- `changeCredits`/`m_nTimeLimit` 0x17b0e2–0x17b116.
- mission chain 0x17b130–0x17b1c4 (`==22` empty branch, `==43` FileRead).
- 96/127 shared body: original tests 96 (`cmp #0x60; bne`) then later 127
  (`cmp #0x7f; beq 0x17b722`); ours emits the same two tests with the same shared target.
- `menuTouchOpen`/`inCinematicMode` block 0x17b816–0x17b872.

## Remaining root cause: block ordering (not source semantics)
First structural divergence is the `Hud::initHudMenu` tail merge at original 0x17adc0:
```
17adc0: ldrd r0,r2,[fp,#116] ; movs r1,#1 ; b.n 17afde   ; 0x200 case
...
17afd8: ldrd r0,r2,[fp,#116] ; movs r1,#2
17afde: blx Hud::initHudMenu                              ; shared tail (0x400 case falls through)
17afe2: ldr r0,[fp,#248] ; cbz r0, 17b01a                 ; if (hudTouchFlags == 0)
17afe8: ... final tail
```
Ours merges the other way: the 0x400 case branches *back* to the 0x200 case's `blx`, which then
`b.w`s to the `hudTouchFlags == 0` test at 0x17b930. Consequence: the original places the
`hudTouchFlags == 0` test/tail/epilogue and the freelance-mission-failed body (0x17b07e)
immediately after the 0x400 case, while ours leaves the weaponSlot bodies (0x2000..0x10000)
before that test. That single layout choice cascades into ~1800 misaligned lines in the
side-by-side and is the dominant score loss.

Related measured facts:
- `weaponSlot >= 0` path: original jumps straight to the final tail (0x17ba24 `b.w 17afe8`);
  ours falls through the redundant `hudTouchFlags == 0` test. The original's CFG proves
  `flags != 0` on that path.
- Moving the `if (hudTouchFlags == 0)` block before the weaponSlot block (separate `if`) drops
  to 72.5% / 8056 B; as an `else if` on the chain 74.9% / 8108 B; as the final `else if`
  75.1% / 8084 B. `__builtin_expect(weaponSlot >= 0, 0)` is a no-op; `__builtin_expect(failed,0)`
  drops to 71.3% / 8064 B.

## Mining-report guard (2-byte encoding only)
Original 0x17a5ca `cmp.w r9,#0 ; beq` (tests `amount != 0`). Ours `cmp.w r9,#-1 ; bgt`
(`int count = -amount; if (count > 0)`). Every `!= 0` spelling (`if (amount)`, `if (count)`,
`if (count != 0)`, `if (amount != 0)`, unsigned `count`) makes clang 7.0.2 flip the decrementing
counter to a positive one (`sub.w r9,#1` -> `add.w r8,#1`, `rsb` -> `mov`), shrinking the body
to 8076 B and dropping the whole function to 71.2–71.4%. The `> 0` form keeps the negative
counter and the `rsb` but selects `cmp #-1; bgt`. The landed `eb8d3a64` shape
(`amount++` then `amount = -amount; if (amount != 0)`) does reproduce the exact `cmp #0; beq`
mining block but costs ~1.2 pts overall through the global register-allocation shift.

## Other confirmed non-issues
- `bool failed` hoist and `goto finishMission` placement are byte-identical to the un-hoisted /
  inline forms.
- `if (!hudMenuOpen) { switch } else { ... }` inversion is byte-identical.
- The 96/127 combined condition already emits the original's two-test shared-body shape.
