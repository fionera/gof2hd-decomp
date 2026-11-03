# MGame::OnTouchEnd (8080 B) — w4234 pass 7: the Ghidra outer-test form, the exact
# HBLP predicate, and the real content deltas that cannot be landed alone

Target: `_ZN5MGame10OnTouchEndEiiPv` (Ghidra entry 0x18a144, file VMA 0x17a144, 8080 B).
Baseline (unchanged by this pass): **78.4 %**, 8080 B, 2775 original / 2774 ours instructions.
Gate unchanged: byte_exact 4498 / linked_exact 4498, size 4524, data 843/843, imports 207,
`.text` 0x1337c8, `.rodata` 0x41b67, lint CLEAN, drift 0.

This pass landed **no code change**. It (1) re-derives the `hasBetterLayoutPredecessor`
predicate exactly and corrects the previous note's off-by-one, (2) tests the *Ghidra-faithful*
outer-test form `if ((flags & 0x200) == 0) { chain } else { initHudMenu(1); }` and shows it
changes the IR block order but not the final layout, (3) identifies two genuine content deltas
(the 3rd `getCampaignMission` call + the `isInstantActionMission` early return, and the mining
`amount != 0` test) and measures them, and (4) records that the mining delta is **size-negative**
(8080 → 8076) and therefore cannot be landed alone without breaking `byte_exact`.

Prior notes: `mgame-ontouchend-78-1-8080-b_w3807.md`,
`mgame-ontouchend-tailmerge-branchfolder_w3826.md`,
`mgame-ontouchend-tailmerge-mbp-reorder_w4007.md`,
`mgame-ontouchend-hblp-nearmiss-w4234.md`.

## 0. The iOS 1.1.19 dump

`source file list dump/gof2hd_1.1.19_ios.cpp` is a *symbol/TU* dump, not decompiled source.  It
confirms the TU membership only: `Classes/Game/MGame.cpp` owns
`_MGame::OnTouchEnd(int, int, void*)` (and the `(int,int)` overload), i.e. the Android 2.0.16
function is a plain `MGame.cpp` member with no extra TU-local statics.  The iOS 1.1.4 Mach-O
oracle (`_work/bins/ios_1.1.4_HD`) is stripped (`llvm-nm` shows only undefined/exported
symbols), so it cannot be used to locate or disassemble the function; it adds no structural
evidence beyond the TU mapping.

## 1. The exact HBLP predicate (and the correction to the previous note)

`MachineBlockPlacement::hasBetterLayoutPredecessor` (LLVM 7.0.0, same paths as 7.0.2):

```cpp
BranchProbability HotProb = getLayoutSuccessorProbThreshold(BB);   // StaticLikelyProb = 80 %
BlockFrequency CandidateEdgeFreq = MBFI->getBlockFreq(BB) * RealSuccProb;
...
BlockFrequency PredEdgeFreq = MBFI->getBlockFreq(Pred) * MBPI->getEdgeProbability(Pred, Succ);
if (PredEdgeFreq * HotProb >= CandidateEdgeFreq * HotProb.getCompl())  // <-- '>='
  BadCFGConflict = true;
```

`BranchProbability(80,100)` is stored as `N/D`, `D = 2^31`:

```
N = (80 * 2^31 + 50) / 100 = 1717986918
C = D - N                  = 429496730
```

`BlockFrequency::operator*` uses `BranchProbability::scale`, an exact 128-bit floor division
(`lib/Support/BranchProbability.cpp`, `::scale<D>`).  With the measured block frequencies from
the second `buildCFGChains()` of the `goto` shape:

```
F200 = 1854461460   (0x200 body, bb.204)
F400 =  463615365   (0x400 body, bb.226)

F400 * N / D = 370892291.9136449  -> 370892291      (prededge * HotProb)
F200 * C / D = 370892292.3454204  -> 370892292      (candedge * HotProb.getCompl())
370892291 >= 370892292  is FALSE  -> no conflict -> the 0x200 body chains into the shared tail
```

**Corrected predicate.**  The previous note stated the conflict fires iff `F200 <= 4*F400 - 1`.
Exact integer evaluation shows the boundary is `F200 <= 4*F400 - 2`:

| F200 | F400 | floor(F400*N/D) | floor(F200*C/D) | conflict |
|---|---|---|---|---|
| 1854461460 | 463615365 | 370892291 | 370892292 | no (baseline) |
| 1854461459 | 463615365 | 370892291 | 370892292 | **no** (previous note said yes) |
| 1854461458 | 463615365 | 370892291 | 370892291 | yes |

Equivalently, because `0.8*F400` has fractional part `(F400 mod 5) * 0.2` and the
`N/D = 0.79999999953` error contributes a fixed `-0.216`:

```
conflict  <=>  F400 mod 5 in {2, 3, 4}
```

The measured `F400 = 463615365 = 5 * 92723073` is `0 mod 5`, i.e. the one residue class out of
five that is guaranteed **not** to conflict.  A shift of `E` (the dispatch-block integer
frequency) by ~16 units (relative 4e-9) or of `F400` by 2 units would flip it.

The second HBLP check (`shared tail -> test`, pred = 0x800 body) is structural:

```
pred  = 927230730 * N/D = 741784583
cand  = 2318076825 * C/D = 463615365        (f(shared tail) = F200 + F400)
conflict  (741784583 >= 463615365)
```

To avoid it the shared tail would need `freq >= 3708922912`, i.e. 60 % more than
`F200 + F400 = 5/8 * E`.  With the natural if/else weights (`F800 = E/4`) this is impossible, so
the second pass can never chain the test after the shared tail unless the 0x800 body is not a
predecessor of the test (or its frequency is biased).

## 2. The Ghidra-faithful outer-test form does not change the layout

Ghidra decompiles the outer test as

```c
if ((uVar14 & 0x200) == 0) {
    if (-1 < (int)(uVar14 << 0x14)) { ... 0x800 ... }
    ...
} else {
    pHVar34 = ...; iVar8 = 1;
LAB_0018afde:
    Hud::initHudMenu(pHVar34, iVar8, pLVar30);
}
```

i.e. the source is `if (!(hudTouchFlags & 0x200)) { <whole chain> } else { initHudMenu(1); }`,
not `if (hudTouchFlags & 0x200) { initHudMenu(1); } else if (0x800) ...`.  This form was
applied and measured:

```c
if (!(this->hudTouchFlags & 0x200)) {
    if (this->hudTouchFlags & 0x800) { ... }
    else if (this->hudTouchFlags & 0x400) { this->hud->initHudMenu(2, this->level); }
    else if (...) { ... }
} else {
    this->hud->initHudMenu(1, this->level);
}
```

* The IR block order **does** change.  In the `-g0` `.ll` the `initHudMenu(1)` block moves
  *after* the `initHudMenu(2)` block: baseline labels `initHudMenu(1) = %1990`,
  `initHudMenu(2) = %2002`; invert labels `initHudMenu(2) = %1998`,
  `initHudMenu(1) = %2036`.  So the 0x200 body now has a *higher* IR block number than the
  0x400 body.
* The emitted code is nevertheless unchanged in the parts that matter: dispatch is still
  `tst #512; bne <0x200 body>` / `lsls #20; bmi <0x800 body>` / `lsls #21; bmi <0x400 body>`,
  and the 0x200 body still owns the shared `blx` (`b1: BLX`, `b2: B->shared`).
* Official score **78.4 % / 8080 B** — identical to the baseline (the previous w3807 note also
  called this form "byte-identical").

Conclusion: `CreateCommonTailOnlyBlock`'s tie is **not** decided by the IR/MIR block number in
the way the w4007 note assumed; ISel/MBP renumber the machine blocks so the 0x400 body is still
the higher-numbered candidate.  Reordering the *source* blocks is not a lever.

## 3. Two genuine content deltas found by a branch-target-normalised diff

`tools/verify/verify.py` compares intra-function branch targets literally (`loc_<off>`), so the
layout cascade hides the real content differences.  Normalising `loc_<hex>` to `loc` and
re-running `difflib` gives a content-only ratio (`/dev/shm/w4234/contentdiff.py`):

```
baseline : ratio 81.28   orig 2775 insns / ours 2774
```

The remaining content deltas in the mission-success block are real:

**(a) 3rd `getCampaignMission` call + `isInstantActionMission` early return.**
The original at 0x17af5a..0x17afc0 is

```
17af5e  getMission; hasWon
17af6a  mov r1,r0; ldr [r8]; cbz r1, 17af78
17af72  getMission
17af76  b 17af7c
17af78  getCampaignMission          <-- 3rd call (ours reuses the `campaign` local)
17af7c  mov r5,r0; isInstantActionMission
17af82  cbz r0, 17afa6
17af84  ... switch_to_target_setting=2; active=0; SetCurrentApplicationModule(1); return
17afa6  mov r0,r5; isCampaignMission
17afb8  nextCampaignMission(true)
```

i.e. the source is

```c
Mission *mission = Globals::status->getMission()->hasWon()
                       ? Globals::status->getMission()
                       : (Mission *) (intptr_t) Globals::status->getCampaignMission();
if (mission->isInstantActionMission()) { ...; return; }
if (mission->isCampaignMission()) { nextCampaignMission(true); } else { ... }
```

whereas we currently reuse the `campaign` local and test `isCampaignMission()` before
`isInstantActionMission()`.  Applying the faithful form:

* content-only ratio **81.28 -> 81.73**, instruction count **2774 -> 2775** (exactly the
  original's 2775);
* official score **78.1 % / 8080 B** (down 0.3) because the resulting layout shifts a few
  branch offsets the wrong way.  Not landed: not a strict improvement.

**(b) Mining `amount != 0`.**  The original at 0x17a5xx is `cmp r9,#0; beq` (r9 = `amount`),
ours is `cmp r9,#-1; bgt` (r9 = `-count`).  The faithful source is

```c
if (amount != 0) {
    report += String("\n") + String(-amount) + String("t ") + *Globals::gameText->getText(1274 + refined);
}
```

instead of `int count = -amount; if (count > 0) { ... String(count) ... }`.  It is semantically
equivalent but **size-negative**: the function becomes 8076 B, which shifts `.text` and drops
`byte_exact` from 4498 to **4478** (20 functions) — a hard ratchet failure.  It cannot be landed
alone; it needs a compensating +4 B elsewhere in the same function.

## 4. Measured variants (all reverted; baseline 78.4 % / 8080 B)

Every row was measured with the official `check` target (report.json `pct`/`osize`/`usize`).
"content ratio" is the branch-target-normalised `difflib` ratio from `contentdiff.py`.

| variant | size | official | content ratio | byte_exact | gate |
|---|---|---|---|---|---|
| baseline (`flat`) | 8080 | **78.4** | 81.28 | 4498 | PASS |
| `invert` (Ghidra outer-test form, §2) | 8080 | 78.4 | — | 4498 | PASS |
| `campaign` (§3a) | 8080 | 78.1 | 81.73 | 4498 | PASS |
| `campaign` + `fw` | 8080 | 78.1 | 81.73 | 4498 | PASS |
| `fw` alone | 8080 | 78.3 | 81.28 | 4498 | PASS |
| `ft` + `fw` | 8076 | 76.6 | — | **4478** | FAIL (size) |
| `campaign` + mining (§3b) | 8076 | 71.2 | — | **4478** | FAIL (size) |
| `campaign` + `fw` + `ft` + mining | 8072 | 70.6 | 79.61 | **4281** | FAIL (size) |
| `goto` + `fw` + `ft` | 8080 | 74.8 | — | 4498 | PASS |
| `goto` (w4007) | 8082 | 75.1–75.8 | — | — | FAIL (size) |
| `invert` + `goto` | 8082 | (not measured) | — | — | — |

`fw` = weaponSlot body ordered `pauseOpen=0; hudMenuOpen=0; resumeSounds(); closeHudMenu()`
(matches Ghidra / the original 0x17ba0c..0x17ba24: `movs r0,#0; strb [fp,#93]; strb [fp,#214];
blx resumeSounds; blx closeHudMenu`);
`ft` = unguarded `hudTouchFlags == 0` block with `closeHudMenu` first (matches Ghidra).

**Harness caveat.** `gen.py <shape> [fw] [ft]` regenerates the *whole* hudMenuOpen block from the
pristine `MGame_base.cpp`, so it silently discards any edit made outside that block (e.g.
`apply_campaign.py`, `apply_mining.py`) if run afterwards.  The `campaign`/mining combinations in
the table were re-applied by hand after `gen.py`; the `fw`/`ft`/`goto` rows are pure shape
variants.  This is why the earlier w4007 numbers for "goto + campaign3" and "goto + faithfulhud"
cannot be compared directly with these.

The important consequence: **every faithful content fix we know of (campaign, fw, ft, mining)
is score-neutral or score-negative on its own**, because the score is dominated by the ~200
branch-offset hunks of the layout cascade.  Landing them before the layout is fixed only moves
the score down.

## 5. What remains

1. The 0x200 body must lose the HBLP conflict (`F200 <= 4*F400 - 2`, equivalently
   `F400 mod 5 in {2,3,4}`) **and** the shared tail must then chain to the test (which needs
   `F800` to be less than 5/32 of `E`, i.e. a biased 0x800 test or a different predecessor set).
   Both are properties of the whole-function BFI, not of the hudMenuOpen block: the integer
   scale is `8 / min_block_float` and the coldest block is one of the ~22 blocks in the big
   shared-tail merge group (float 1.231230818e-11, int 8).
2. The two faithful content deltas in §3 are correct against Ghidra but must be landed together
   with a +4 B size compensation (the mining fix) and a layout shift that does not lose more
   branch-offset matches than it gains.
3. The `hudTouchFlags == 0` block (`ft`) and the weaponSlot ordering (`fw`) are also correct but
   each costs ~2–4 B; the original is 8080 B, so our current function must be ~4 B too large
   somewhere else (the mining `count` local is the known 4 B).

## 6. Reproduction

```
cmake --build cmake-build-match --target check -j8          # RATCHET PASS, byte 4498
python3 -c "import json;r=json.load(open('cmake-build-match/verify/report.json'));print([x for x in r['rows'] if x['name']=='_ZN5MGame10OnTouchEndEiiPv'])"
# fast layout probe (~0.5 s): prints b1/b2 ownership + shared-tail->test adjacency
python3 /dev/shm/w4234/sig.py
# content-only diff (branch targets normalised)
python3 /dev/shm/w4234/contentdiff.py _ZN5MGame10OnTouchEndEiiPv
# MBP trace (pristine LLVM 7.0.0)
/home/debian/llvm7build/build/bin/llc -march=thumb -mcpu=cortex-a8 \
    -mtriple=thumbv7-none-linux-androideabi16 -O2 \
    -debug-only=branch-folder,block-placement base.ll -o /dev/null
```

Scratch harness (not tracked): `/dev/shm/w4234/{sig.py,contentdiff.py,apply_campaign.py,
apply_mining.py,apply_invert.py,base.ll,inv.ll,base_trace.txt,inv_trace.txt}`.
