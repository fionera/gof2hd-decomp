# LevelScript::process — wave 161 / w10704: the extra 13-entry `tbh` is mission 0

Target `_ZN11LevelScript7processEi` (original `0x137d18`, 57,232 B = 0xdf90), size-exact.
Worktree `wt/w10704`, base master `01c7f92d6` (wave 160).
Scratch `/dev/shm/w10704/`.

## Baseline (HEAD, unchanged)

```
process raw diffbytes    55,048
process aligned insns    17,049
orig / ours size         57232 / 57232     (size-exact)
.text diffbytes          85,518
.ARM.extab diffbytes     1,258   (size 156,312 exact)
.rel.plt                 0        (BYTE-IDENTICAL)
.rodata/.data/.bss/.ARM.exidx  byte-identical
byte_exact 4507 / linked_exact 4507 / size 4524 / data 843 / imports 207
lint CLEAN / drift 0 / RATCHET PASS
```

## Finding: the extra `tbh` is the mission-0 state dispatch

w10603 counted 15 `tbh` in the original and 16 in ours, with the extra one a
13-entry table at `0x139068`.  That table is the **mission-0 `m_nState`
dispatch** (the `else { switch (m_nState) { case 6..15 } }` at
`LevelScript.cpp:754`), not any of the per-mission switch tables.

Proof: the table's default target in the baseline is `0x13d54c`, which is
exactly the mission-0 state-16 handler (`cmp r0,#16; bne` →
`SetCurrentApplicationModule(2)` / `nextCampaignMission(true)` /
`switch_to_target_setting = 1`), and table entry 0 (`state 3`) points at the
mission-0 state-3 body (`m_pCamera->translate(delta*0.2,0,delta*2.2)` then
`messages[8]->isOver()` → `m_nState = 4`).

### Original shape

The original lowers the mission-0 dispatch as **three compares then a
10-entry table**:

```
0x1389c8  cmp r1,#4 ; beq 0x138d2a        ; state 4 body is placed far away
0x1389ce  cmp r1,#3 ; bne 0x138fd8        ; state 3 body falls through (local)
0x1389d4  <state-3 body>
0x138fd8  cmp r1,#5 ; bne 0x139b1a        ; state 5 body falls through (local)
0x138fde  <state-5 body>                  ; (checks messages[12] inside)
0x139b16  ldr r1,[sl,#28]
0x139b1a  subs r0,r1,#6
0x139b1c  cmp r0,#9
0x139b1e  bhi 0x13f906                    ; default -> state-16 check
0x139b22  tbh [pc,r0,lsl #1]              ; 10 entries: states 6..15
```

So the original's source order is **4, 3, 5** (state 3 before state 5), and
the state-3 arm is *not* merged into the switch.

### Our shape (baseline)

```
0x1389b8  cmp r1,#5 ; beq 0x138da0        ; state 5 body (checks messages[12])
0x1389be  cmp r1,#4 ; bne 0x139060        ; state 4 body falls through
0x1389c4  <state-4 body>
0x13905c  ldr r1,[fp,#28]
0x139060  subs r0,r1,#3
0x139062  cmp r0,#12
0x139064  bhi 0x13d54c                    ; default -> state-16 check
0x139068  tbh [pc,r0,lsl #1]              ; 13 entries: states 3..15
0x139086  <state-3 body>                  ; state 3 folded into the table
```

LLVM folds the last simple compare (`m_nState == 3`) into the following
switch, widening the table from `6..15` to `3..15`; states 4 and 5 become
padding entries that jump to the default.  This is a block-placement /
`SimplifyCFG` artifact of the source order **4, 5, 3** (state 3 is the last
`else if` before the `else` switch).

## Source shape that removes it

Swapping the state-3 and state-5 `else if` arms (source order **4, 3, 5**,
matching the original) removes the extra table:

```c++
        } else if (m_nState == 4) {
            ...
        } else if (m_nState == 3) {          // moved before the state-5 arm
            ...
        } else if (m_nState == 5 && ((RadioMessage *) ((*messages)[12]))->isOver()) {
            ...
        } else {
            switch (m_nState) { case 6..15 ... }
```

Measured (size stays 57,232 B, gate stays green):

| variant | raw | aligned | size | tbh | .rel.plt |
|---|---:|---:|---:|---:|---:|
| **HEAD (4,5,3)** | **55,048** | **17,049** | **57,232** | **16** | **0** |
| swap (4,3,5) | 55,059 | 17,732 | 57,232 | **15** | **10 B (5 slots rotated)** |

The table becomes a 10-entry `subs #6; cmp #9` at `0x13b144`, matching the
original's `0x139b22`, and the `tbh` count drops to 15.  But the backend
places the state-3 body far away (instead of state 4), so both metrics rise
slightly, and — critically — **`.rel.plt` stops being byte-identical**: five
`R_ARM_JUMP_SLOT` entries rotate because the mission-0 switch case bodies
move and change the first-reference order of
`StarSystem::switchPlanetForIntro`, `Level::switchSkyboxForIntro`,
`ParticleSystemManager::reset`, `PlayerEgo::startSmokeEmission`,
`PlayerStation::setVisible`.  Under the "never regress `.rel.plt`" invariant
the swap is therefore **not landed**; it is the candidate for a future wave
that can also re-pin the PLT order.

## All six arm permutations (size is the gate)

Blocks are the state-4 / state-5 / state-3 `else if` arms; `LevelScript.cpp`
baseline order is 4,5,3.

| source order | raw | aligned | size | note |
|---|---:|---:|---:|---|
| 4,5,3 (HEAD) | 55,048 | 17,049 | 57,232 | extra 13-entry table |
| **4,3,5** | **55,059** | **17,732** | **57,232** | table fixed, `.rel.plt` 10 B |
| 3,4,5 | 55,113 | 17,726 | 57,248 (+16) | fatal |
| 3,5,4 | 55,227 | 16,927 | 57,224 (-8) | fatal |
| 5,3,4 | 55,414 | 19,386 | 57,240 (+8) | fatal |
| 5,4,3 | 55,414 | 19,386 | 57,240 (+8) | fatal |

Other shapes tried (all fatal or worse):

* state-5 condition as a nested `if (messages[12]->isOver())` instead of the
  compound `&&` (both 4,5,3 and 4,3,5): raw 55,343 / aligned 18,653, size
  exact, but no table fix benefit over the plain swap.
* hoist `messages[8]->isOver()` into the state-3 condition (compound
  `m_nState == 3 && msg8->isOver()`): raw 54,924, size 57,264 (+32), fatal.
* move the state-3 arm after the switch inside the `else`: identical to the
  4,3,5 swap (55,059 / 17,732 / 57,232, `.rel.plt` 10 B).
* move the state-16 check before the switch: raw 54,917, size 57,264 (+32),
  fatal.
* `__builtin_expect(m_nState == 4, 0)` on the 4,3,5 order: raw 54,886 (best
  raw seen) but size 57,240 (+8), fatal; `.rel.plt` still differs.
* `__builtin_expect(m_nState == 3, 1)` on the 4,5,3 order: removes the table
  (15 `tbh`) but size 57,428 (+196), fatal.

## m_nState dispatch audit (missions 0/92/154/157/158)

w10602 had already verified arm order and condition polarity for these.  The
table shapes in the baseline are:

| mission | original | ours (baseline) |
|---|---|---|
| 0 | cmp 4, 3, 5 + 10-entry `tbh` 6..15 | cmp 5, 4 + **13-entry `tbh` 3..15** |
| 92 | 7-entry `tbh` 1..7 (`subs #1; cmp #6`) | 7-entry `tbh` 1..7 (same) |
| 154 | 10-entry `tbh` 1..10 (`subs #1; cmp #9`) | 10-entry `tbh` 1..10 (same) |
| 157 | if-chain, partial `tbh` | if-chain, partial `tbh` |
| 158 | cmp 3/2/1 chain | cmp 3/2/1 chain (same) |

Mission 0 is the only one whose table shape differs.  (A separate 4-entry
`tbh` in the original at `0x13cad4`, `subs #2; cmp #3`, has a 6-entry
`subs #2; cmp #5` counterpart in ours around mission 145; it is a
compiler-lowering difference of the same kind but was not chased this wave.)

## Open questions

1. The 4,3,5 swap is the correct source order but the backend's block
   placement (state 3 far instead of state 4) does not match the original's
   PGO-like weights.  A `__builtin_expect` combination that keeps size at
   57,232 B was not found; the best (`e4`) is +8 B.
2. `.rel.plt` order is coupled to the mission-0 switch-body layout.  Any
   future landing of the 4,3,5 swap must also restore the five rotated
   `R_ARM_JUMP_SLOT` entries.
3. The mission-145-style `subs #2; cmp #3` vs `subs #2; cmp #5` table
   difference is a second, independent lowering mismatch worth a dedicated
   pass.
