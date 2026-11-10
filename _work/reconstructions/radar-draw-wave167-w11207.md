# Radar::draw wave 167 (w11207) — residual branch layout / cleanup pads

**Status:** analysis pass, no source change landed (2026-11-10). Worktree
`wt/w11207`, branch `wave167-w11207`, based on `master` `d00057e6d`. Target
`src/game/weapons/Radar.cpp`, `_ZN5Radar4drawEP6PlayerP3Hudi`
(`Radar::draw(Player*, Hud*, int)`), size 9,908 B.

Read with `radar-draw-wave166-w11107.md` (sound-ladder expects + decl order,
7,549 -> 7,489) and `radar-draw-wave164-w11008.md` (ladder + faithful tail).

## 0. Entry state (master d00057e6d, incl. w11118 LICM flag)

raw **7,487**, aligned **1,454**, size **9,908/9,908**, insn 3,536/3,532,
`.text` differing **82,898**, `.ARM.extab` **660**, `.rel.plt` **0**.
`check` = RATCHET PASS (byte 4507, linked 4507, size 4524, data 843,
imports 207, lint CLEAN, parity 5370).

## 1. Root cause of the residual: a global sl/fp + stack-slot permutation

The first differing byte is at function offset 0x0e:

| | original | ours |
|---|---|---|
| `str r3,[sp,#?]` (mode) | `#100` | `#104` |
| `str r2,[sp,#?]` (hud) | `#84` | `#80` |
| zero constant | `mov.w fp,#0` | `mov.w sl,#0` |
| `turretMode` | `str r0,[sp,#148]` | `str r0,[sp,#152]` |
| `campaign` | `mov sl,r0` (register) | `str r0,[sp,#144]` (spilled) |

`campaign = Globals::status->getCurrentCampaignMission()` (source line 355) is
used exactly once (`campaign >= 2`, line 532).  The original keeps it in `sl`
from `0x12e0f8` to `0x12e38a`; ours spills it to `sp+144`.  Because `sl` is
therefore free, the `plasmaInRange = 0; lockConfirmed = 0` constant takes `sl`
(original: `fp`), and `fp` is then free to hoist the repeated DrawImage2D
height literal `68` (`mov.w fp,#68` at `0x12e782`; original rematerialises
`movs r6,#68` per call).  That single allocation difference cascades into the
uniform ~41% instruction mismatch across the whole function (diff blocks are
spread evenly, ~40 per 100 instructions, not localised).

No source lever found to move `campaign` back into a register (see §3).

## 2. Branch/block-layout findings (structural, not regalloc)

* **Campaign block (line 555).** Original: body inline at `0x12e474`
  (`blt 0x12e474` for `cur < 148`, `blt 0x12e4a8` for `cur < 152`); skip out
  of line at `0x12e4a8`.  Ours (`expect(...,0)`): body out of line at
  `0x12e5ca` (`blt.w` / `bge.w`), skip inline.  This is the wave-164
  `expect(...,0)` trade-off; flipping it was re-measured and is still worse
  (§3).
* **Landmark loop setup (lines 418-420).** Original computes
  `notTurretMode`/`notOtherLock` at `0x12e7a8-0x12e7dc` immediately before the
  loop condition (`ldr [r4,#312]; ldr [r0]; cmp sl,r1; bcs`), fall-through
  into the body.  Ours computes the same at `0x12f042-0x12f070` and then
  `b.n` to the condition at `0x12f3d8`; the setup is also interleaved with
  the `mov.w fp,#68` hoist.  Pure block-placement difference.
* **Sound block (line 1295).** Original places the `field_0x1bc < 3` ladder
  at `0x130348` (after the epilogue) and the ambient body at `0x13038e`;
  ours places the ladder at `0x130318` (after the epilogue + literal pool)
  and the ambient body at `0x1305b0`.  The 8-word switch literal pool is at
  `0x1302dc` in the original (right after `b.n` at `0x1302da`) but at
  `0x130304` in ours.  Also `(current != 151 && alwaysEnemySeen)` compiles to
  `eors.w r2,r2,#1` in the original but `eor.w r2,r2,#1; lsls r2,r2,#31`
  in ours (one extra 2-byte instruction; compensated elsewhere).
* **Route block (lines 394-409) is essentially identical** — a single
  `beq.w` target-index difference at line 412.
* **Cleanup pads (end of function).** Both have the same 5 `StringD1Ev`
  call sites in the same order (sp+232, 196, 184, 220, 208).  Landing-pad
  entries differ: original 6 (`mov r4,r0` at 0x1306ae/b8/c2/cc/d2/e0), ours
  7 (+0x1306d6).  Original's final pad destroys sp+232; ours' final pad
  destroys sp+220 and a separate pad destroys sp+232.  The extra pad is the
  sp+220 site the wave-164 note identified.  All expression respellings that
  keep the same codegen leave the pads unchanged.

## 3. Measured variants (all rejected; base raw 7,487 / aligned 1,454 / 9,908 B)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base | **7,487** | **1,454** | 9,908 |
| `const int campaign` | 7,487 | 1,454 | 9,908 |
| `unsigned int campaign` | 7,488 | 1,454 | 9,908 |
| swap `alienOrbit`/`campaign` decls | 7,494 | 1,459 | 9,908 |
| `bool alienOrbit` | 8,432 | 1,475 | 9,912 |
| `expect(campaign >= 2, 1)` | 8,474 | 1,783 | 9,908 (`.rel.plt` +3!) |
| `expect(campaign >= 2, 0)` | 9,681 | 3,247 | 9,980 |
| all 12 valid decl orders of `otherLock`/`landmarkHit`/`notTurretMode`/`notOtherLock` | 7,487-7,488 | 1,454 | 9,908 |
| `alwaysEnemySeen != 0` / `== 1` (sound cond) | 7,487 | 1,454 | 9,908 |
| `!(current == 151 \|\| !alwaysEnemySeen)` | 7,487 | 1,454 | 9,908 |
| `(current == 151 ? 0 : alwaysEnemySeen)` | 7,565 | 1,478 | 9,912 |
| `(alwaysEnemySeen && current != 151)` | 7,565 | 1,478 | 9,912 |
| ladder `<3` -> `>2` | 7,554 | 1,462 | 9,872 |
| ladder `<5` -> `>4` | 7,487 | 1,455 | 9,908 |
| campaign cond no expect | 8,866 | 1,608 | 9,976 |
| campaign cond `expect(...,1)` | 9,284 | 1,680 | 9,964 |
| landmark `onScreen` expect 0 | 9,517 | 2,871 | 9,840 |
| landmark `screenX > ...` expect 0 | 9,532 | 2,872 | 9,840 |
| planet guard expect 0 | 8,613 | 2,698 | 9,912 |
| `isDockingToPlanet()` expect 0 | 8,778 | 1,734 | 9,880 (`.rel.plt` +20) |
| `getCurrentCampaignMission() != 120` expect 0 | 7,494 | 1,460 | 9,908 |
| `i == 0 && !alienOrbit` -> `!alienOrbit && i == 0` | 8,578 | 1,537 | 9,920 |
| `i <= 1` -> `i < 2` | 7,487 | 1,454 | 9,908 |
| offset ternary `i != 0 ? originY : originX` | 7,487 | 1,454 | 9,908 |
| `(i & 0x7fffffff) == 3` -> `i == 3` | 7,487 | 1,454 | 9,908 |
| `(i & 0x7fffffff) == 1` -> `i == 1` | 7,487 | 1,454 | 9,908 |
| `landmarkHit` moved first in the 4-way guard | 8,196 | 1,419 | 9,904 |
| lockImage if-chain -> ternary | 7,488 | 1,456 | 9,908 |
| image if-chain -> ternary (both arms) | 7,487 | 1,454 | 9,908 |
| landmark String expr: paren inner / paren both / paren first / `String(": ",false)` | 7,487 | 1,454 | 9,908 |
| landmark String expr: `String(String(x))` (drop copy) | 8,660 | 2,679 | 9,880 |

## 4. Conclusion

The residual is dominated by a single global register-allocation / stack-slot
choice (`campaign` spilled in ours, kept in `sl` in the original) plus the
resulting block placement and literal-pool placement.  None of the local
source levers tried moves that allocation.  The function is a local optimum
at raw 7,487 / aligned 1,454 / 9,908 B; no source change was landed and the
tree is clean.

Next idea worth trying (not attempted): force `campaign` out of the spill by
extending its live range artificially or by restructuring the planet guard so
the value is used before the long call sequence — but any such edit changes
the surrounding codegen and must be measured against 7,487.
