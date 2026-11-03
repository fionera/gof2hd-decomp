# MGame::OnTouchEnd (8080 B) — w4007: tail-merge direction vs. MBP re-placement

Target: `_ZN5MGame10OnTouchEndEiiPv` (Ghidra entry 0x18a144, file VMA 0x17a144, 8080 B).
Baseline (unchanged by this pass): **78.4 %**, 8080 B, 2775 original / 2774 ours instructions.
Gate unchanged: byte_exact 4498 / linked_exact 4498, size 4524, data 843/843, imports 207,
`.text` 0x1337c8, lint CLEAN, drift 0.

This pass landed **no code change**. It records the exact mechanism that owns the remaining
~21 %, the MIR/HBLP evidence, and every measured source shape (this session + w3807/w3826).
The prior note `mgame-ontouchend-tailmerge-branchfolder_w3826.md` established the tail-merge
mechanism; this note adds the second half: *why flipping the merge direction is not enough* —
MachineBlockPlacement re-places the 0x200 body immediately before the shared tail in the second
`buildCFGChains()` pass, because the 0x800 body wins the `hasBetterLayoutPredecessor` (HBLP)
conflict at `StaticLikelyProb = 80 %`.

## 1. The original layout (authoritative, objdump of the original .so)

Switch dispatch chain (note the order 0x200, 0x800, 0x400 and the `tst` first test):

```
17ad4c: f8db 00f8  ldr.w   r0, [fp, #248]          ; hudTouchFlags
17ad50: f410 7f00  tst.w   r0, #512                ; 0x200
17ad54: d134       bne.n   17adc0                  ; -> 0x200 body
17ad56: 0501       lsls    r1, r0, #20             ; 0x800
17ad58: f100 8098  bmi.w   17ae8c                  ; -> 0x800 body
17ad5c: 0541       lsls    r1, r0, #21             ; 0x400
17ad5e: f100 813b  bmi.w   17afd8                  ; -> 0x400 body
17ad62: 0481       lsls    r1, r0, #18             ; 0x2000
17ad64: f100 823e  bmi.w   17b1e4
...                                                  ; 0x4000..0x100000
17ad9a: 04c0       lsls    r0, r0, #19             ; 0x1000
17ad9c: f140 8121  bpl.w   17afe2                  ; -> test
17ada0: 4658       mov     r0, fp
17ada2: f6ed eaf8  blx     UseKhadorDrive
17ada6: e11c       b.n     17afe2                  ; -> test
```

The 0x200 body sits immediately after the dispatch chain, and **branches forward** to the
shared tail; the 0x400 body is the fall-through into the shared tail:

```
17adc0: e9db 021d  ldrd    r0, r2, [fp, #116]      ; 0x200 body
17adc4: 2101       movs    r1, #1
17adc6: e10a       b.n     17afde                  ; forward to shared tail
...
17adc8: ...                                         ; mission-success block (hasWon etc.)
...
17ae8c: ...                                         ; 0x800 body (closeHudMenu/resumeSounds/useCloak)
17aeaa: e09a       b.n     17afe2                  ; -> test (branch, not fall-through)
...
17afd8: e9db 021d  ldrd    r0, r2, [fp, #116]      ; 0x400 body
17afdc: 2102       movs    r1, #2
17afde: f6ed e9ce  blx     Hud::initHudMenu        ; shared tail (0x400 falls through)
17afe2: f8db 00f8  ldr.w   r0, [fp, #248]          ; test: if (hudTouchFlags == 0)
17afe6: b1c0       cbz     r0, 17b01a
17afe8: ...                                         ; final tail (isAutoPilot / return)
...
17b01a: ...                                         ; hudTouchFlags == 0 body
17b04e: e7cb       b.n     17afe8                  ; back to final tail
...
17b9da: ... Ship::getEquipment / setCurrentSecondaryWeaponIndex
17ba24: ... b.w 17afe8                              ; weaponSlot body -> final tail
```

So the original layout of the whole cluster is

```
[0x400 body][shared blx][test][final tail][hudTouchFlags==0 body] ... [weaponSlot body]
```

and the 0x200 body is a forward branch into the shared tail. The weaponSlot body is far away
(0x17b9da) and jumps to the final tail, i.e. it does **not** fall through into the test.

## 2. Which pass merges the tail, and how it chooses

`-mllvm -enable-tail-merge=false` leaves each case with its own `blx` (4 calls, 8084 B), so the
merge is not in the IR. The merge is performed by the `BranchFolder` that
`MachineBlockPlacement` runs internally with `AfterBlockPlacement=true` (LLVM 7.0.x
`MBP.cpp`, `tryTailMergeBlocks`), followed by a second `buildCFGChains()`.

`BranchFolding.cpp` (LLVM 7.0.0 source, identical to 7.0.2 for these paths):

```cpp
unsigned BranchFolder::ComputeSameTails(unsigned CurHash,
                                        unsigned MinCommonTailLength,
                                        MachineBasicBlock *SuccBB,
                                        MachineBasicBlock *PredBB) {
  ...
  for (MPIterator CurMPIter = std::prev(MergePotentials.end()), ...
       CurMPIter != B && CurMPIter->getHash() == CurHash; --CurMPIter) {
    for (MPIterator I = std::prev(CurMPIter); I->getHash() == CurHash; --I) {
      ...
      if (ProfitableToMerge(...)) {
        if (CommonTailLen > maxCommonTailLength) {
          SameTails.clear(); ... SameTails.push_back(SameTailElt(CurMPIter, TrialBBI1));
        }
        if (HighestMPIter == CurMPIter && CommonTailLen == maxCommonTailLength)
          SameTails.push_back(SameTailElt(I, TrialBBI2));
      }
      ...
    }
  }
```

`MergePotentials` is sorted by `(hash, block number)` ascending, and `ComputeSameTails` walks it
from the **end**, so `SameTails[0]` is the **higher** block number and `SameTails[1]` the lower.

```cpp
bool BranchFolder::CreateCommonTailOnlyBlock(MachineBasicBlock *&PredBB,
                                             MachineBasicBlock *SuccBB,
                                             unsigned maxCommonTailLength,
                                             unsigned &commonTailIndex) {
  commonTailIndex = 0;
  unsigned TimeEstimate = ~0U;
  for (unsigned i = 0, e = SameTails.size(); i != e; ++i) {
    // Use PredBB if possible; that doesn't require a new branch.
    if (SameTails[i].getBlock() == PredBB) { commonTailIndex = i; break; }
    // Otherwise, make a (fairly bogus) choice based on estimate ...
    unsigned t = EstimateRuntime(SameTails[i].getBlock()->begin(),
                                 SameTails[i].getTailStartPos());
    if (t <= TimeEstimate) { TimeEstimate = t; commonTailIndex = i; }
  }
  ...
```

Both candidate prefixes are `ldrd r0,r2,[fp,#116]` (mayLoad -> 2) + `movs r1,#N` (1) = **3**, so
`EstimateRuntime` always ties. The decision is therefore purely `PredBB` / block-number order:
`PredBB` if it is one of `SameTails`, else the **later** `SameTails` index (the lower block number).

## 3. Baseline MIR trace (our build, `-mllvm -print-after=block-placement`)

Pre-MBP MIR block identities (`dbgtest.txt`):

| block | role | source line | successors |
|---|---|---|---|
| bb.390 | 0x200 body (`initHudMenu(1)`) | 2056 | bb.450 |
| bb.415 | 0x800 body (`closeHudMenu`/`useCloak`) | 2059 | bb.450 |
| bb.416 | 0x400 test (`& 0x400`) | 2063 | bb.418, bb.417 |
| bb.417 | 0x400 body (`initHudMenu(2)`) | 2064 | bb.450 |
| bb.421/422 | weaponSlot body | 2093 | bb.422 / bb.450 |
| bb.449 | wingman body | 2111 | bb.450 |
| bb.450 | test `if (hudTouchFlags == 0)` | 2114 | bb.451, bb.460 |

The internal BranchFolder call:

```
TryTailMergeBlocks: %bb.390, %bb.415, %bb.417, %bb.422, %bb.449, %bb.444, %bb.445
  with successor %bb.450
  which has fall-through from %bb.422
Common tail length of %bb.417 and %bb.390 is 1

CTOB: i=0 %bb.417 runtime=3
CTOB:   PredBB=%bb.422
CTOB: i=1 %bb.390 runtime=3
CTOB:   PredBB=%bb.422

Splitting %bb.390, size 1
Using common tail in %bb.467 for %bb.417,
```

`SameTails` contains only `{bb.417, bb.390}` (the other five blocks have a different hash).
`PredBB` is bb.422 (the weaponSlot body), which is **not** in `SameTails`; the runtime tie goes to
`i=1`, i.e. **bb.390 (the 0x200 body)**. That is why our 0x200 case owns the shared `blx`.

The layout consequence is visible in `h_base.s`: `.LBB22_207` = `movs r1,#1; bl initHudMenu;
b.w test`, and the test is placed after the weaponSlot body (`.LBB22_368` -> `.LBB22_369`).

## 4. The goto shape flips the merge, but MBP re-places the 0x200 body

Adding `goto afterHudTouchTest;` after the weaponSlot body (so the weaponSlot body jumps to the
final tail, matching the original CFG) changes `PredBB` of the merge target to bb.417:

```
TryTailMergeBlocks: %bb.390, %bb.415, %bb.417, %bb.449, %bb.444, %bb.445
  with successor %bb.450
  which has fall-through from %bb.417
Common tail length of %bb.417 and %bb.390 is 1

CTOB: i=0 %bb.417 == PredBB -> pick

Splitting %bb.417, size 1
Using common tail in %bb.466 for %bb.390
```

So the 0x400 body becomes the common tail (the desired direction). But the second
`buildCFGChains()` renumbers the blocks and re-places them: the 0x200 body (now bb.204) is
processed before the 0x400 body (bb.226), and its only successor is the common tail (bb.227):

```
Selecting best successor for: %bb.204        ; 0x200 body
HBLP: BB=%bb.204 L2056 f=1854461460 Succ=%bb.227 L0 f=2318076825 unsched=1
    Candidate: %bb.227, probability 100.00% (CFG break)
    Selected: %bb.227
Merging from %bb.204 to %bb.227
Selecting best successor for: %bb.227        ; common tail
HBLP: BB=%bb.227 L0 f=2318076825 Succ=%bb.228 L2115 f=3274283516 unsched=2
HBLP pred %bb.211 (0x800 body) f=927230730
HBLP   prededge=927230730 *hp=741784583 candedge=2318076825 *cp=463615365
      CONFLICT pred %bb.211 predfreq=927230730 candfreq=2318076825
    Not a candidate: %bb.228 -> 100.00% (prob) (non-cold CFG conflict)
Merging from %bb.227 to %bb.205              ; next block in the list, not the test
```

The emitted layout is still `[0x200 body][shared blx][b test]` (`.LBB22_206` = `movs r1,#1`,
`.LBB22_207` = `bl; b test`); the 0x400 body is a separate block branching to `.LBB22_207`.
The test is placed after the 0x800 body (`.LBB22_214` -> `.LBB22_215`), not after the shared tail.
Result: **75.1–75.8 % / 8084 B** (size change also drops byte_exact).

## 5. The HBLP conflict, and why the 0x800 body always wins

`MachineBlockPlacement.cpp` (`hasBetterLayoutPredecessor`), with
`HotProb = getLayoutSuccessorProbThreshold(BB) = StaticLikelyProb = 80 %` (no profile):

```cpp
BlockFrequency CandidateEdgeFreq = MBFI->getBlockFreq(BB) * RealSuccProb;
...
BlockFrequency PredEdgeFreq = MBFI->getBlockFreq(Pred) * MBPI->getEdgeProbability(Pred, Succ);
if (PredEdgeFreq * HotProb >= CandidateEdgeFreq * HotProb.getCompl()) {
  BadCFGConflict = true;
  break;
}
```

i.e. `predfreq * 0.8 >= candfreq * 0.2`, or `predfreq * 4 >= candfreq`.

Baseline (at bb.390, the 0x200 body; `dbgtest.txt`):

```
HBLP: BB=%bb.390 L2056 f=1854461460 Succ=%bb.450 L2114 f=3708922921 unsched=4
HBLP pred %bb.415 (0x800 body) f=927230730
HBLP   prededge=927230730 *hp=741784583 candedge=1854461460 *cp=370892292
      CONFLICT pred %bb.415 predfreq=927230730 candfreq=1854461460
```

`927230730 * 4 = 3708922920 >= 1854461460` -> conflict; the 0x200 body's edge to the test is
rejected, and the test is eventually chained after the weaponSlot body (bb.422).

Goto variant (at bb.227, the common tail):

```
HBLP: BB=%bb.227 L0 f=2318076825 Succ=%bb.228 L2115 f=3274283516
HBLP pred %bb.211 (0x800 body) f=927230730
HBLP   prededge=927230730 *hp=741784583 candedge=2318076825 *cp=463615365
      CONFLICT pred %bb.211 predfreq=927230730 candfreq=2318076825
```

`927230730 * 4 = 3708922920 >= 2318076825` -> conflict; the common tail's edge is rejected, and
the test is chained after the 0x800 body.

The frequencies are the natural if/else-chain weights: 0x200 = 50 % (1854461460), 0x800 = 25 %
(927230730), 0x400 = 12.5 % (463615365). The common tail is 0x200+0x400 = 2318076825. For the
common tail to win the conflict it would need `candfreq >= 4 * 927230730 = 3708922920`, i.e. a
60 % larger common tail, or the 0x800 body would need to fall below 25 % (12.5 % would give
`463615365 * 4 = 1854461460 < 2318076825`, no conflict). The test order fixes the 0x800 case at
25 %, so the conflict is structural.

## 6. Ghidra decompilation of the region (source shape is a flat if/else chain)

```c
uVar14 = *(uint *)(this + 0xf8);                 // hudTouchFlags
if ((uVar14 & 0x200) == 0) {
    if (-1 < (int)(uVar14 << 0x14)) {            // !(uVar14 & 0x800)
        if (-1 < (int)(uVar14 << 0x15)) {        // !(uVar14 & 0x400)
            if ((int)(uVar14 << 0x12) < 0) { iVar8 = 0; }          // 0x2000
            else if ((int)(uVar14 << 0x11) < 0) { iVar8 = 1; }     // 0x4000
            else if ((int)(uVar14 << 0x10) < 0) { iVar8 = 2; }     // 0x8000
            else { ... 0x10000/0x20000/0x40000/0x80000/0x100000/0x1000 ... }
            ... weaponSlot ... goto LAB_0018afe8;                  // -> final tail
        }
        pHVar34 = ...; pLVar30 = ...; iVar8 = 2;
        goto LAB_0018afde;                                          // 0x400 body -> shared tail
    }
    this[0xd6] = 0; Hud::closeHudMenu(...); this[0x5d] = 0;
    resumeSounds(this); useCloak(this);                             // 0x800 body
}
else {
    pHVar34 = ...; pLVar30 = ...; iVar8 = 1;
LAB_0018afde:
    Hud::initHudMenu(pHVar34, iVar8, pLVar30);                      // shared tail
}
LAB_0018afe2:
if (*(int *)(this + 0xf8) == 0) { ... }                             // test / hudTouchFlags==0 body
LAB_0018afe8:
iVar8 = PlayerEgo::isAutoPilot(...); ...                            // final tail
```

The decompilation confirms: flat `if/else if` chain, 0x200 first, 0x800 second, 0x400 third;
weaponSlot body jumps to the final tail; the `hudTouchFlags == 0` block is unguarded and calls
`closeHudMenu` first (the w3826 "faithful hud" block).

## 7. Measured variants (all reverted; baseline 78.4 % / 8080 B)

Rough = `fastscore2.py` (baseline 79.08); official = `verify/report.json`.

### This session (w4007)

| variant | source shape | size | score |
|---|---|---|---|
| `v_nestedchain` | `if (0x200) ... else { if (0x800) ... else if (0x400) ... }` | 8080 | 79.08 rough / **78.4 official** |
| `v_switch` | `switch (hudTouchFlags) { case 0x200: ... }` | 8104 | 72.82 rough |
| `v_swap800400` | source order 0x200, 0x400, 0x800 | 8068 | 71.76 rough |
| `v_goto_swap800400` | goto + source order 0x200, 0x400, 0x800 | 8084 | 73.96 rough |
| `v_goto_expect200likely` | goto + `__builtin_expect(0x200, 1)` | 8044 | 71.47 rough |
| `v_goto_expect800unlikely` | goto + `__builtin_expect(0x800, 0)` | 8080 | 72.53 rough |
| `v_goto_expect800unlikely_400likely` | goto + expect(0x800,0) + expect(0x400,1) | 8044 | 71.50 rough |
| `v_goto_expect800likely` | goto + `__builtin_expect(0x800, 1)` | 8044 | 71.16 rough |
| `v_goto800` | goto weaponSlot + goto after 0x800 and 0x1000 | 8084 | 75.85 rough |
| `v_goto800only` | goto after 0x800 and 0x1000 only | 8076 | 73.68 rough |
| `v_campaign3_only` | 3rd `getCampaignMission` call | 8080 | 78.98 rough / **78.3 official** |
| `v_swap_campaign3` | swap 0x200/0x400 + 3rd campaign call | 8080 | 78.88 rough / **78.2 official** |

`v_goto_expect800unlikely` is the only variant that produces the original cluster
`[0x400 body][shared blx][test]` (`.LBB22_213`/`.LBB22_214`/`.LBB22_215`), but the
`__builtin_expect` makes the 0x800 body cold and MBP places it at `.LBB22_436`, after the test,
instead of before the 0x400 body. Adding a moderate per-file `-likely/-unlikely-branch-weight`
(20:12, 80:1, 7:1) changes the size (8084–8092 B) and does not recover the original layout.

### Prior sessions (w3807 / w3826, re-verified where noted)

| variant | result |
|---|---|
| swap 0x200/0x400 in the source chain | 78.2 % / 8080 B — merge flips, test order becomes 0x400-first |
| move 0x200 to end of chain | 73.4 % / 8068 B |
| move 0x400 to end of chain | 73.8 % / 8068 B |
| invert outer test `if (!(flags & 0x200)) ... else ...` | byte-identical |
| nested `else { if (wingmanCommand>=0) ... if (hudTouchFlags==0) ... }` | 75.1 % / 8084 B |
| nested + faithful hud | 74.9 % / 8080 B |
| 3rd `getCampaignMission` call | 78.3 % / 8080 B |
| mining `if (amount != 0) { String(-amount) }` | 71.4 % / 8076 B |
| faithful `hudTouchFlags == 0` block (unguarded, closeHudMenu first) | 76.6 % / 8076 B |
| closeHudMenu-first only (keep null check) | 78.3 % / 8080 B |
| faithful hud + 3-call campaign | 76.5 % / 8076 B |
| all three content fixes | 70.8 % / 8072 B |
| `v_goto` (goto after weaponSlot) | 75.1–75.8 % / 8084 B |
| `v_testfirst` (test block before weaponSlot bodies) | 73.06 % / 8056 B |
| `v_goto_campaign3` | 75.86 % / 8088 B |
| `v_goto_faithfulhud` | 75.43 % / 8080 B |
| `v_goto_expect200b` (`expect((flags&0x200)!=0, 0)`) | 72.29 % rough |
| `v_goto_expect400` (`expect(!!(flags&0x400), 1)`) | 69.53 % / 8028 B |
| `v_swap_faithfulhud` | 77.27 % / 8076 B |
| `v_swap_faithfulhud_campaign3` | 77.17 % / 8076 B |
| `v_faithfulhud_only` | 77.38 % / 8076 B |
| `v_mining` | 72.28 % / 8076 B |
| mining `amount++` then `amount = -amount; if (amount != 0)` | exact `cmp #0; beq`, ~1.2 pts loss |
| `-branch-fold-placement=false` | 4 separate `blx`, 8084 B |
| `-enable-tail-merge=false` | 4 separate `blx`, no merge |

## 8. What this proves / what remains

Proved:

1. The 0x200/0x400 `Hud::initHudMenu` merge is done by `BranchFolder` inside MBP
   (`AfterBlockPlacement=true`), not by the standalone post-PEI branch folder.
2. The surviving tail is chosen by `CreateCommonTailOnlyBlock`: `PredBB` if it is in `SameTails`,
   else the runtime tie (both prefixes are 3) goes to the **lower block number**. Baseline
   `PredBB` is the weaponSlot body, so bb.390 (0x200) wins.
3. The `goto afterHudTouchTest` CFG shape flips the merge (the 0x400 body becomes the common
   tail), but MBP's second `buildCFGChains()` re-places the 0x200 body immediately before the
   shared tail and then chains the test after the 0x800 body, because the 0x800 body wins the
   HBLP conflict at `HotProb = 80 %` (`927230730 * 4 >= 2318076825`).
4. The conflict is structural for the original test order (0x200 = 50 %, 0x800 = 25 %,
   0x400 = 12.5 %). The common tail would need `candfreq >= 4 * 0x800_freq` to win.
5. A `switch` on exact values is not the source shape (it lowers to a `cmp/beq` chain, 8104 B,
   72.8 %); the original is the flat bit-mask if/else chain, as Ghidra shows.
6. `v_nestedchain` is byte-identical to baseline (78.4 %), so the nesting of the flag chain is
   not the lever.

Remains:

- A source shape that (a) keeps the 0x200-first test order, (b) makes the 0x400 body the common
  tail, and (c) makes the 0x800 body's edge to the test lose the HBLP conflict **without**
  making the 0x800 body cold, so MBP places `[0x400 body][shared blx][test][final tail]`
  contiguously. The natural if/else weights cannot do (c); the only measured lever that flips
  the conflict is `__builtin_expect(0x800, 0)`, which also makes the 0x800 body cold and
  misplaces it after the test.
- The remaining content deltas (3rd `getCampaignMission` call, mining `amount != 0`, faithful
  unguarded `hudTouchFlags == 0` block) are each independently correct against Ghidra but cost
  layout points; they should be landed together with the layout fix, not before it.

## 9. Reproduction

```
# baseline
cmake --build cmake-build-match --target check -j8          # RATCHET PASS, byte 4498
# per-function score
python3 -c "import json;r=json.load(open('cmake-build-match/verify/report.json'));print([x for x in r['rows'] if x['name']=='_ZN5MGame10OnTouchEndEiiPv'])"
# MIR / MBP trace (LLVM 7.0.0 build, same paths as 7.0.2)
llc -march=thumb -mcpu=cortex-a8 -mtriple=thumbv7-none-linux-androideabi16 -O2 \
    -debug-only=branch-folder,block-placement h_<variant>70.ll -o /dev/null
```

Scratch harness (not tracked): `/dev/shm/w4007/{variants.py,harness.py,harness2.py,fastscore2.py}`.
