# MGame::OnTouchEnd (8080 B) — w4409 (wave 94): the faithful mission-continuation
# shape is content-exact (2775/2775) but block placement still puts the mission
# continuation after the 0x400 body; no strict improvement landed

Target: `_ZN5MGame10OnTouchEndEiiPv` (Ghidra entry 0x18a144, file VMA 0x17a144, 8080 B).
Baseline (unchanged by this pass): **78.4 %**, 8080 B, 2775 original / 2774 ours
branch-target-normalised instructions (2865 / 2864 raw).
Gate unchanged: byte_exact **4499** / linked_exact **4499**, size 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0.

This pass lands **no code change**.  It (1) re-verifies the original cluster and the
mission-continuation block from the binary, (2) inlines the Ghidra decompilation of the
mission-success region, (3) records the *actual* MBP block-placement order for the baseline
and for the `expi` shape (the earlier w4255 note cited wrong block numbers for the
mission-continuation blocks), (4) measures four further source combinations, and (5) shows
that the remaining blocker is a **chain-head priority inversion**: both the baseline and
every `goto`/`expi` shape place the 0x400 body *before* the mission-continuation chain,
while the original places the mission-continuation chain *before* the 0x400 body.

Prior notes: `mgame-ontouchend-78-1-8080-b_w3807.md`,
`mgame-ontouchend-tailmerge-branchfolder_w3826.md`,
`mgame-ontouchend-tailmerge-mbp-reorder_w4007.md`,
`mgame-ontouchend-hblp-1unit-w4107.md`,
`mgame-ontouchend-wave92-pass7-w4234.md`,
`mgame-ontouchend-wave92-w4255.md`.

## 0. TU / source-layout witness

`source file list dump/gof2hd_1.1.19_ios.cpp` confirms only that `Classes/Game/MGame.cpp`
owns `_MGame::OnTouchEnd(int, int, void*)`; there are no TU-local statics for this function.
The iOS 1.1.4 Mach-O (`_work/bins/ios_1.1.4_HD`) is stripped and adds no structural
evidence.  Everything below is reconstructed from the Android 2.0.16 binary
(Ghidra + objdump).

## 1. The original cluster (authoritative objdump)

`_work/bins/android_2.0.16_libgof2hdaa.so`, file VMA 0x17a144..0x17c0a4:

```
17adc0: e9db 021d  ldrd   r0, r2, [fp, #116]     ; 0x200 body
17adc4: 2101       movs   r1, #1
17adc6: e10a       b.n    17afde                   ; forward to the shared blx
17adc8: f6e8 edb2  blx    63930 <Mission::hasWon>   ; mission-success block
17adcc: b940       cbnz   r0, 17ade0
17adce: f8d8 0000  ldr.w  r0, [r8]
17add2: f6e7 ed44  blx    6285c <Status::getCampaignMission>
17add6: f6e8 edac  blx    63930 <Mission::hasWon>
17adda: 2800       cmp    r0, #0
17addc: f000 8165  beq.w  17b0aa                   ; resetStartSequenceOver; end
17ade0: f8d8 0000  ldr.w  r0, [r8]                  ; keepObjectives block
17ade4: f8cd a018  str.w  sl, [sp, #24]
17ade8: f6e7 ed38  blx    6285c <Status::getCampaignMission>
17adec: f6e8 eda0  blx    63930 <Mission::hasWon>
17adf0: 2800       cmp    r0, #0
17adf2: f000 80b2  beq.w  17af5a                   ; keepObjectives = false -> MC
17adf6: f8d8 0000  ldr.w  r0, [r8]
17adfa: f6e7 ed2a  blx    62850 <Status::getMission>
17adfe: f6e8 ec96  blx    6372c <Mission::isCampaignMission>
17ae02: f080 0a01  eor.w  sl, r0, #1                 ; keepObjectives = !isCampaign
17ae06: e0aa       b.n    17af5e                   ; -> MC
...
17ae8c: f8db 0074  ldr.w  r0, [fp, #116]     ; 0x800 body
17ae90: 2400       movs   r4, #0
17ae92: f88b 40d6  strb.w r4, [fp, #214]     ; hudMenuOpen = 0
17ae96: f6ec edb6  blx    67a04 <Hud::closeHudMenu>
17ae9a: 4658       mov    r0, fp
17ae9c: f88b 405d  strb.w r4, [fp, #93]      ; pauseOpen = 0
17aea0: f6ed e9f4  blx    6828c <MGame::resumeSounds>
17aea4: 4658       mov    r0, fp
17aea6: f6ed ea82  blx    683ac <MGame::useCloak>
17aeaa: e09a       b.n    17afe2                   ; -> test (branch, not fall-through)
...
17af5a: f04f 0a00  mov.w  sl, #0                   ; mission continuation, keepObjectives=0
17af5e: f8d8 0000  ldr.w  r0, [r8]
17af62: f6e7 ec76  blx    62850 <Status::getMission>
17af66: f6e8 ece4  blx    63930 <Mission::hasWon>
17af6a: 4601       mov    r1, r0
17af6c: f8d8 0000  ldr.w  r0, [r8]
17af70: b111       cbz    r1, 17af78
17af72: f6e7 ec6e  blx    62850 <Status::getMission>   ; mission = getMission()
17af76: e001       b.n    17af7c
17af78: f6e7 ec70  blx    6285c <Status::getCampaignMission> ; else campaign
17af7c: 4605       mov    r5, r0
17af7e: f6ed ea22  blx    683c4 <Mission::isInstantActionMission>
17af82: b180       cbz    r0, 17afa6
17af84: 4813       ldr    r0, [pc, #76]           ; instant-action: return
17af86: 2102       movs   r1, #2
17af88: 4478       add    r0, pc
17af8a: 6800       ldr    r0, [r0, #0]
17af8c: 6001       str    r1, [r0, #0]             ; switch_to_target_setting = 2
17af8e: 2000       movs   r0, #0
17af90: f88b 0054  strb.w r0, [fp, #84]      ; active = 0
17af94: f8db 0008  ldr.w  r0, [fp, #8]
17af98: 9932       ldr    r1, [sp, #200]
17af9a: f8d9 2000  ldr.w  r2, [r9]
17af9e: 1a51       subs   r1, r2, r1
17afa0: d139       bne.n  17b016
17afa2: 2101       movs   r1, #1
17afa4: e3fd       b.n    17b7a2                   ; SetCurrentApplicationModule(1)
17afa6: 4628       mov    r0, r5
17afa8: f6e8 ebc0  blx    6372c <Mission::isCampaignMission>
17afac: 4601       mov    r1, r0
17afae: f8d8 0000  ldr.w  r0, [r8]
17afb2: 2900       cmp    r1, #0
17afb4: f000 807f  beq.w  17b0b6                   ; freelance branch
17afb8: 2101       movs   r1, #1
17afba: f6ea e85a  blx    65070 <Status::nextCampaignMission>
17afbe: e090       b.n    17b0e2
17afc0: 511a       str    r2, [r3, r4]             ; literal pool (0x17afc0..0x17afd7)
...
17afd8: e9db 021d  ldrd   r0, r2, [fp, #116]     ; 0x400 body
17afdc: 2102       movs   r1, #2
17afde: f6ed e9ce  blx    6837c <Hud::initHudMenu>  ; shared tail (0x400 falls through)
17afe2: f8db 00f8  ldr.w  r0, [fp, #248]     ; test: if (hudTouchFlags == 0)
17afe6: b1c0       cbz    r0, 17b01a
17afe8: f8da 0000  ldr.w  r0, [sl]           ; final tail (isAutoPilot / return)
17afec: f6eb ec16  blx    6681c <PlayerEgo::isAutoPilot>
17aff0: f080 0001  eor.w  r0, r0, #1
17aff4: ea50 0008  orrs.w r0, r0, r8
17aff8: bf02       ittt   eq
17affa: 48ef       ldreq  r0, [pc, #956]
17affc: f04f 517e  moveq.w r1, #1065353216
17b000: e9cb 106c  strdeq r1, r0, [fp, #432]
17b004: 9832       ldr    r0, [sp, #200]      ; epilogue (stack check)
17b006: f8d9 1000  ldr.w  r1, [r9]
17b00a: 1a08       subs   r0, r1, r0
17b00c: bf02       ittt   eq
17b00e: b033       addeq  sp, #204
17b010: e8bd 0f00  ldmiaeq.w sp!, {r8, r9, sl, fp}
17b014: bdf0       popeq  {r4, r5, r6, r7, pc}
17b016: f6e3 ec06  blx    5e824 <__stack_chk_fail>
17b01a: f8db 0074  ldr.w  r0, [fp, #116]     ; hudTouchFlags == 0 body
17b01e: f6ec ecf2  blx    67a04 <Hud::closeHudMenu>  ; UNGUARDED (no null check)
17b022: 2400       movs   r4, #0
17b024: 4658       mov    r0, fp
17b026: f88b 40d6  strb.w r4, [fp, #214]
17b02a: e9cb 4426  strd   r4, r4, [fp, #152]
17b02e: f88b 405d  strb.w r4, [fp, #93]
17b032: f6ed e92c  blx    6828c <MGame::resumeSounds>
17b036: f89b 00c8  ldrb.w r0, [fp, #200]
17b03a: 2800       cmp    r0, #0
17b03c: d0d4       beq.n  17afe8
17b03e: f8db 0058  ldr.w  r0, [fp, #88]
17b042: f88b 40c8  strb.w r4, [fp, #200]
17b046: 2800       cmp    r0, #0
17b048: d0ce       beq.n  17afe8
17b04a: f6e7 ee2a  blx    62ca0 <PlayerEgo::resetGunDelay>
17b04e: e7cb       b.n    17afe8
...
17b9cc: ...                                ; shared weaponSlot body (r6 = slot)
17ba24: f7ff bae0  b.w    17afe8                   ; weaponSlot -> final tail (skips test)
...
17bee2: ...                                ; shared wingman body (r8 = command)
17bf4a: f7ff b84a  b.w    17afe2                   ; wingman -> test
```

So the original layout is

```
[0x200 body][MS][keepObjectives][starMap chain][0x800 body][maneuver][menuWindow]
[starMap cleanup][MC][0x400 body][shared blx][test][final tail][epilogue][hud0 body]
... [weaponSlot body] ... [wingman body]
```

with:
* the **0x200 body** branching **forward** into the shared `blx`;
* the **0x400 body** falling **through** into the shared `blx` (it owns the tail);
* the **test** falling through from the shared `blx`;
* the **weaponSlot body** jumping to the final tail (skipping the test);
* the **wingman body** jumping to the test;
* the **hudTouchFlags == 0 body** reached from the test, unguarded `closeHudMenu`.

## 2. The baseline cluster (ours, 78.4 %)

```
17adc6: e9db 021d  ldrd   r0, r2, [fp, #116]  ; 0x200 body
17adca: 2101       movs   r1, #1
17adcc: f6ed ead6  blx    6837c <Hud::initHudMenu>  ; shared tail (0x200 falls through)
17add0: f000 bdae  b.w    17b930              ; -> test (far away)
17add4: f6e8 edac  blx    63930 <Mission::hasWon>   ; MS block, right after
...
17af6c: e9db 021d  ldrd   r0, r2, [fp, #116]  ; 0x400 body
17af70: 2102       movs   r1, #2
17af72: e72b       b.n    17adcc              ; BACKWARD into the shared tail
...
17afe8: f04f 0a00  mov.w  sl, #0              ; MC block (after the 0x400 body)
...
17b8d6: 2603       movs   r6, #3              ; weaponSlot body
...
17b930: f8db 00f8  ldr.w  r0, [fp, #248]     ; test (after the weaponSlot body)
17b934: b1c0       cbz    r0, 17b968
17b936: ...                                  ; final tail
17b952: ...                                  ; epilogue
17b968: ...                                  ; hudTouchFlags == 0 body
...
17bf4a: e4f1       b.n    17b930              ; wingman -> test
```

Differences from the original:
1. the **0x200 body** owns the shared `blx` (0x400 branches back);
2. the **test** is placed after the weaponSlot body (which falls through into it);
3. the **weaponSlot body** falls through to the test instead of jumping to the final tail;
4. the **MC chain** is placed after the 0x400 body (original: before it).

## 3. Ghidra decompilation of the mission-success region

`DecompOte.java` (headless Ghidra 12.1.2, image base 0x10000, `-noanalysis`) on
`0x18a144`; the mission-success region (lines 930-1005 of `ote_decomp.txt`) is:

```c
        iVar7 = Mission::hasWon(pMVar25);
        if (iVar7 == 0) {
          pMVar25 = (Mission *)Status::getCampaignMission((Status *)*piVar42);
          iVar7 = Mission::hasWon(pMVar25);
          if (iVar7 == 0) {
            LevelScript::resetStartSequenceOver(*(LevelScript **)(this + 0x7c));
            goto LAB_0018b8e2;
          }
        }
        pMVar25 = (Mission *)Status::getCampaignMission((Status *)*piVar42);
        iVar7 = Mission::hasWon(pMVar25);
        if (iVar7 == 0) {
          uVar14 = 0;
        }
        else {
          pMVar25 = (Mission *)Status::getMission((Status *)*piVar42);
          uVar14 = Mission::isCampaignMission(pMVar25);
          uVar14 = uVar14 ^ 1;
        }
        pMVar25 = (Mission *)Status::getMission((Status *)*piVar42);
        iVar7 = Mission::hasWon(pMVar25);
        if (iVar7 == 0) {
          pMVar25 = (Mission *)Status::getCampaignMission((Status *)*piVar42);
        }
        else {
          pMVar25 = (Mission *)Status::getMission((Status *)*piVar42);
        }
        iVar7 = Mission::isInstantActionMission(pMVar25);
        if (iVar7 != 0) {
          **(undefined4 **)(iRam0018afd4 + 0x18af8c) = 2;
          this[0x54] = (MGame)0x0;
          uVar24 = *(undefined4 *)(this + 8);
          if (*piVar44 == local_28) {
            uVar27 = 1;
            goto LAB_0018b7a2;
          }
          goto LAB_0018b016;
        }
        iVar7 = Mission::isCampaignMission(pMVar25);
        if (iVar7 == 0) {
          Status::setFreelanceMission
                    ((Status *)*piVar42,(Mission *)**(undefined4 **)(iRam0018b3c0 + 0x18b0bc));
          iVar7 = **(int **)(iRam0018b3c4 + 0x18b0c8);
          cVar5 = Mission::getReward(pMVar25);
          cVar6 = Mission::getBonus(pMVar25);
          Layout::showMissionRewardMessage(iVar7,(bool)(cVar6 + cVar5));
        }
        else {
          Status::nextCampaignMission(SUB41((Status *)*piVar42,0));
        }
        pSVar35 = (Status *)*piVar42;
        iVar7 = Mission::getReward(pMVar25);
        iVar8 = Mission::getBonus(pMVar25);
        Status::changeCredits(pSVar35,iVar8 + iVar7);
        **(undefined4 **)(this + 0x7c) = 0;
        iVar7 = Mission::isCampaignMission(pMVar25);
        if ((iVar7 == 0) ||
           (iVar7 = Status::getCurrentCampaignMission((Status *)*piVar42), iVar7 != 0xf)) {
          ... campaign-mission switch chain (15/22/43/42/65/81/74/95/96|127/100/110/
              120/126/134/144/155/161/162) ...
```

In source terms this is exactly:

```cpp
} else if (Globals::status->getMission()->hasWon() ||
           ((Mission *) (intptr_t) Globals::status->getCampaignMission())->hasWon()) {
    bool keepObjectives = ((Mission *) (intptr_t) Globals::status->getCampaignMission())->hasWon()
                              ? !Globals::status->getMission()->isCampaignMission()
                              : false;
    Mission *mission = Globals::status->getMission()->hasWon()
                           ? Globals::status->getMission()
                           : (Mission *) (intptr_t) Globals::status->getCampaignMission();
    if (mission->isInstantActionMission()) {          // BEFORE the campaign dispatch
        Globals::switch_to_target_setting = 2;
        this->active = 0;
        this->applicationManager->SetCurrentApplicationModule(1);
        return;
    }
    if (mission->isCampaignMission()) {
        Globals::status->nextCampaignMission(true);
    } else {
        Globals::status->setFreelanceMission(Mission::empty);
        Globals::layout->showMissionRewardMessage(mission->getReward() + mission->getBonus(), false);
    }
    Globals::status->changeCredits(mission->getReward() + mission->getBonus());
    this->levelScript->m_nTimeLimit = 0;
    ... campaign-mission switch chain ...
```

The **current** `src/game/menu/MGame.cpp` differs from this in exactly one respect: it
tests `isCampaignMission()` **before** `isInstantActionMission()` (lines 1558-1568).  The
`gen_campaign.py` variant below restores the Ghidra order.

## 4. MBP block placement: the real blocker is chain-head priority

`-print-after=block-placement` MIR (llc 7.0.0, `-march=thumb -mcpu=cortex-a8
-mtriple=thumbv7-none-linux-androideabi16 -O2`; `variables:` fields stripped for the
7.0.0 reader) gives the final block order.  Block identities:

| block | contents | baseline bb | expi bb |
|---|---|---|---|
| 0x200 body | `ldrd; movs r1,#1; initHudMenu` | 204 | 227 |
| shared tail | `initHudMenu` (tail-merged) | 205 | 227 |
| 0x400 body | `ldrd; movs r1,#2; initHudMenu` | 227 | 226 |
| MC chain | `getMission/hasWon/isInstantActionMission/...` | 235-237 | 242-247 |

Placement positions (1-based, in the final MIR order):

```
baseline: 0x200 body = 205, shared tail = 206, 0x400 body = 228, MC = 235..237
expi:     0x200 body = 206, 0x400 body = 234,      MC = 242..247
```

So in **both** shapes the 0x400 body is placed **before** the MC chain.  The original
places the MC chain at 0x17af5a (before the 0x400 body at 0x17afd8).  The 0x80-byte
shift reported in the w4255 note is the consequence: the MC chain (~0x7E B) is lifted
out from in front of the 0x400 body and re-inserted after it, so every address after
0x17afd8 moves by ~0x80.

The earlier w4255 note attributed the shift to "mission-continuation blocks
`%bb.158/159/160/162`".  Those blocks are actually the **starMap** chain
(`StarMap::OnTouchEnd`, `getStarSystem/initLight`, jump-cancel, `cutsceneActive`
test); the MC chain is `%bb.242..247` in the same dump.  The conclusion (MC moves
after the 0x400 body) is unchanged, but the block numbers in the old note are wrong.

## 5. Measured variants

Official `tools/verify/verify.py` (`--top 0`, autojunk=False), OnTouchEnd row only.
"content" is the branch-target-normalised difflib ratio (orig/ours instruction counts
in parentheses).

| variant | source shape | size | official | content |
|---|---|---|---|---|
| baseline | flat chain, `count > 0` | 8080 | **78.4** | 81.28 (2775/2774) |
| `gen_campaign.py` | Ghidra order: `isInstantAction` before `isCampaign` | 8080 | 78.1 | **81.73 (2775/2775)** |
| `gen_camp3.py` | keep `campaign` local, only ternary refetches | 8080 | 78.3 | 81.18 (2775/2773) |
| `gen_camp2.py` | both keepObjectives and the ternary refetch `getCampaignMission()` | 8080 | 78.3 | 81.18 (2775/2773) |
| `gen_expi.py` | inverted outer test + `goto afterHudTouchTest` | 8084 | 75.1 | — |
| `gen_ft.py` | unguarded hudTouchFlags==0 block, closeHudMenu first | 8076 | 76.6 | — |
| `gen_nested.py` | weaponSlot `else { if (wingman) ...; if (flags==0) ... }` | 8084 | 75.1 | — |
| `gen_expect400.py` | `__builtin_expect((flags & 0x400) != 0, 1)` | 8028 | 70.6 | — |
| **new** campaign+nested | both of the above | 8084 | 74.9 | — |
| **new** campaign+unguard | both of the above | 8076 | 76.4 | — |
| **new** nested+unguard | both of the above | 8080 | 76.3 | — |
| **new** campaign+nested+unguard | all three | 8080 | 76.1 | — |
| mining `if (amount != 0) { int count = -amount; ... }` | faithful mining loop | 8076 | 71.4 | — |
| mining `if (amount != 0) String(-amount)` | faithful mining loop | 8076 | 71.4 | — |

The w4255 note's full 2^5 grid over `{invert, goto, FW, FT, campaign}` had its best
cell at baseline / invert = 78.4 %.  The four new combinations above (campaign+nested,
campaign+unguard, nested+unguard, campaign+nested+unguard) and the two faithful mining
spellings are all strictly worse.  **No strict improvement was found.**

`gen_campaign.py` is the single most faithful shape: it reproduces the original's
instruction multiset exactly (2775/2775 normalised instructions, ratio 81.73, vs the
baseline's 2774 and 81.28), because the original really does test
`isInstantActionMission()` before `isCampaignMission()`.  But it scores 78.1 because
the campaign fix perturbs the block placement and moves the MC chain even further.

The faithful mining loop (`cmp.w r9,#0; beq` vs our `cmp.w r9,#4294967295; bgt`) is
also correct against the binary, but in this source context it makes clang keep
`-amount` in a callee-saved register and reload another one, costing 4 bytes and
shifting everything after it (8080 -> 8076, score 71.4).

## 6. HBLP arithmetic (unchanged, re-verified)

`MachineBlockPlacement::hasBetterLayoutPredecessor` (LLVM 7.0.0, same code as 7.0.2):

```cpp
BranchProbability HotProb = getLayoutSuccessorProbThreshold(BB);   // StaticLikelyProb = 80 %
BlockFrequency CandidateEdgeFreq = MBFI->getBlockFreq(BB) * RealSuccProb;
BlockFrequency PredEdgeFreq = MBFI->getBlockFreq(Pred) * MBPI->getEdgeProbability(Pred, Succ);
if (PredEdgeFreq * HotProb >= CandidateEdgeFreq * HotProb.getCompl())
  BadCFGConflict = true;
```

`BranchProbability(80,100)` is stored as `N / D`, `D = 2^31`:

```
N = (80 * 2^31 + 50) / 100 = 1717986918
C = D - N                  =  429496730
```

The natural if/else-chain BFI frequencies are `F200 = E/2`, `F800 = E/4`,
`F400 = E/8` with `E = 3708922920`, so `F200 = 1854461460`, `F400 = 463615365`:

```
F400 * N / D = 370892291.9136449  -> 370892291      (prededge * HotProb)
F200 * C / D = 370892292.3454204  -> 370892292      (candedge * HotProb.getCompl())
370892291 >= 370892292  is FALSE  -> no conflict -> the 0x200 body chains into the shared tail
```

`F400 = 463615365 = 5 * 92723073` is `0 mod 5`, the one residue class out of five that
is guaranteed not to conflict.  Equivalently the boundary is `F200 <= 4*F400 - 2`.

The second HBLP check (`shared tail -> test`, pred = 0x800 body) is structural:

```
pred  = 927230730 * N/D = 741784583
cand  = 2318076825 * C/D = 463615365        (f(shared tail) = F200 + F400)
conflict  (741784583 >= 463615365)
```

To avoid it the shared tail would need `freq >= 3708922912`, ~60 % more than
`F200 + F400 = 5/8 * E`; with the natural if/else weights (`F800 = E/4`) this is
impossible.  Both numbers are properties of the whole-function BFI and are unchanged
from the w4107 note.

## 7. What remains

1. **The cluster can be made faithful.**  The `goto` forms (`gen_expi.py`,
   `v_EXPI_HUD`) make the 0x400 body own the shared `blx` and put the test right after
   it.  The blocker is no longer the cluster; it is the MC chain placement.
2. **MC must be placed before the 0x400 body.**  In the original the order is
   `[starMap cleanup][MC][0x400 body][shared blx][test]`; in every shape measured so far
   MBP's worklist places the 0x400 body before the MC chain.  Since both are chain
   heads, the fix must change either their relative block frequency or the chain
   structure (e.g. make MC the fall-through successor of the starMap cleanup, or make
   the 0x400 body part of an already-placed chain).
3. **The faithful content deltas are correct but score-neutral or score-negative on
   their own**:
   * `isInstantActionMission` before `isCampaignMission` (`gen_campaign.py`): exact
     instruction multiset, but layout shifts -> 78.1;
   * mining `if (amount != 0)`: matches `cmp #0; beq`, but costs 4 B -> 8076 / 71.4;
   * unguarded `hudTouchFlags == 0` block: matches the original, but costs 4 B ->
     8076 / 76.6;
   * weaponSlot `goto`/nested: matches the original's jump-to-final-tail, but shifts
     the cluster -> 75-76 %.
   They need the MC placement fixed first.
4. **No source shape found in this pass is a strict improvement over 78.4 %.**  The
   baseline remains the best-scoring shape.

## 8. Reproduction

```
# gate
cmake --build cmake-build-match --target check -j8        # RATCHET PASS, byte 4499
# per-function row
python3 -c "import json;r=json.load(open('cmake-build-match/verify/report.json'));print([x for x in r['rows'] if x['name']=='_ZN5MGame10OnTouchEndEiiPv'])"
# HBLP arithmetic (D = 2^31)
python3 -c "D=1<<31;N80=(80*D+50)//100;N20=D-N80;print(463615365*N80//D, 1854461460*N20//D)"
#   -> 370892291 370892292   (no conflict by exactly 1)
# MBP block order (needs llc 7.0.0; strip `variables:` fields first)
sed 's/, variables: ![0-9]*//' MGame.ll > MGame2.ll
/home/debian/llvm7build/build/bin/llc -march=thumb -mcpu=cortex-a8 \
  -mtriple=thumbv7-none-linux-androideabi16 -O2 -print-after=block-placement \
  MGame2.ll -o /dev/null 2>&1 | sed -n '/_ZN5MGame10OnTouchEndEiiPv/,/End machine code/p'
```

Scratch harness (not tracked): `/dev/shm/w4409/` — `cc_ll.sh` (exact repo flags),
`contentdiff.py` (branch-target-normalised difflib), `measure.sh` (verify + report row),
`gen_combo2.py` (campaign/nested/unguard combinations), `gen_mine.py` (faithful mining
loop), `gen_campaign.py`, `gen_expi.py`, `gen_ft.py`, `gen_nested.py`, `gen_expect400.py`,
`orig.dis`, `ours.dis`, `base_ote_mir.txt`, `expi_mir_ote.txt`, `ote_decomp.txt`.
