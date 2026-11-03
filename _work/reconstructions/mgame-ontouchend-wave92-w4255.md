# MGame::OnTouchEnd (8080 B) — w4255 (wave 92): the `expi` form is the first source
# shape that really makes the 0x400 body the common tail — and why it still loses

Target: `_ZN5MGame10OnTouchEndEiiPv` (Ghidra entry 0x18a144, file VMA 0x17a144, 8080 B).
Baseline (unchanged by this pass): **78.4 %**, 8080 B, 2775 original / 2774 ours normalised
instructions (2865 / 2864 raw).
Gate unchanged: byte_exact **4499** / linked_exact **4499**, size 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0.

This pass lands **no code change**.  It (1) reconstructs the original cluster from the binary,
(2) re-derives the `hasBetterLayoutPredecessor` near-miss, (3) shows that the *Ghidra-faithful
outer-test form* (`if (!(flags & 0x200)) { chain } else { initHudMenu(1); }`) **plus the
weaponSlot `goto`** is the first source shape that actually makes the **0x400 body** the common
tail (the shared `blx` falls through from the 0x400 body and the `flags == 0` test falls through
from the `blx` — exactly the original cluster), and (4) proves why it still scores below the
baseline: the whole `[0x400 body][blx][test][final tail][epilogue]` cluster lands **0x80 bytes
early** because the *mission-continuation* block is placed after the epilogue instead of before
the 0x400 body.

Prior notes: `mgame-ontouchend-78-1-8080-b_w3807.md`,
`mgame-ontouchend-tailmerge-branchfolder_w3826.md`,
`mgame-ontouchend-tailmerge-mbp-reorder_w4007.md`,
`mgame-ontouchend-hblp-1unit-w4107.md` (and the w4234 pass-7 note in the w4304 worktree).

## 0. The iOS 1.1.19 dump and the iOS 1.1.4 oracle

`source file list dump/gof2hd_1.1.19_ios.cpp` is a symbol/TU dump, not source.  It confirms only
that `Classes/Game/MGame.cpp` owns `_MGame::OnTouchEnd(int, int, void*)` and the `(int,int)`
overload; there are no extra TU-local statics for this function.  The iOS 1.1.4 Mach-O
(`_work/bins/ios_1.1.4_HD`) is stripped, so it adds no structural evidence.  The source shape
below is therefore reconstructed from the Android 2.0.16 binary (Ghidra + objdump), not from the
iOS dumps.

## 1. The original cluster (authoritative objdump, `_work/bins/android_2.0.16_libgof2hdaa.so`)

```
17ad4c: f8db 00f8  ldr.w  r0, [fp, #248]      ; hudTouchFlags
17ad50: f410 7f00  tst.w  r0, #512            ; 0x200
17ad54: d134       bne.n  17adc0              ; -> 0x200 body
17ad56: 0501       lsls   r1, r0, #20         ; bit 11 = 0x800
17ad58: f100 8098  bmi.w  17ae8c              ; -> 0x800 body
17ad5c: 0541       lsls   r1, r0, #21         ; bit 10 = 0x400
17ad5e: f100 813b  bmi.w  17afd8              ; -> 0x400 body
17ad62: 0481       lsls   r1, r0, #18         ; 0x2000
17ad64: f100 823e  bmi.w  17b1e4
...                                            ; 0x4000 .. 0x100000
17ad9a: 04c0       lsls   r0, r0, #19         ; 0x1000
17ad9c: f140 8121  bpl.w  17afe2              ; -> test
17ada0: 4658       mov    r0, fp
17ada2: f6ed eaf8  blx    68394 <MGame::UseKhadorDrive>
17ada6: e11c       b.n    17afe2              ; -> test
...
17adc0: e9db 021d  ldrd   r0, r2, [fp, #116]  ; 0x200 body
17adc4: 2101       movs   r1, #1
17adc6: e10a       b.n    17afde              ; forward to shared blx
17adc8: ...                                   ; mission-success block (hasWon ...)
...
17ae8c: f8db 0074  ldr.w  r0, [fp, #116]      ; 0x800 body
17ae90: 2400       movs   r4, #0
17ae92: f88b 40d6  strb.w r4, [fp, #214]      ; hudMenuOpen = 0
17ae96: f6ec edb6  blx    67a04 <Hud::closeHudMenu>
17ae9a: 4658       mov    r0, fp
17ae9c: f88b 405d  strb.w r4, [fp, #93]       ; pauseOpen = 0
17aea0: f6ed e9f4  blx    6828c <MGame::resumeSounds>
17aea4: 4658       mov    r0, fp
17aea6: f6ed ea82  blx    683ac <MGame::useCloak>
17aeaa: e09a       b.n    17afe2              ; -> test (branch, not fall-through)
...
17afd8: e9db 021d  ldrd   r0, r2, [fp, #116]  ; 0x400 body
17afdc: 2102       movs   r1, #2
17afde: f6ed e9ce  blx    6837c <Hud::initHudMenu>  ; shared tail (0x400 falls through)
17afe2: f8db 00f8  ldr.w  r0, [fp, #248]      ; test: if (hudTouchFlags == 0)
17afe6: b1c0       cbz    r0, 17b01a
17afe8: ...                                   ; final tail (isAutoPilot / return)
...
17b004: 9832       ldr    r0, [sp, #200]      ; epilogue (stack check)
...
17b01a: f8db 0074  ldr.w  r0, [fp, #116]      ; hudTouchFlags == 0 body
17b01e: f6ec ecf2  blx    67a04 <Hud::closeHudMenu>
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
17b9cc: ...                                   ; shared weaponSlot body (r6 = slot)
17ba24: f7ff bae0  b.w    17afe8              ; weaponSlot body -> final tail
...
17bee2: ...                                   ; shared wingman body (r8 = command)
17bf4a: f7ff b84a  b.w    17afe2              ; wingman body -> test
```

So the original cluster is

```
[0x400 body][shared blx][test][final tail][epilogue][hudTouchFlags == 0 body]
```

with the 0x200 body far earlier (0x17adc0) branching **forward** into the shared `blx`, the
0x800 body (0x17ae8c) branching **forward** to the test, the weaponSlot body jumping straight to
the final tail (skipping the test), and the wingman body jumping to the test.

## 2. The baseline cluster (ours, 78.4 %)

```
17adc6: e9db 021d  ldrd   r0, r2, [fp, #116]  ; 0x200 body
17adca: 2101       movs   r1, #1
17adcc: f6ed ead6  blx    6837c <Hud::initHudMenu>  ; shared tail (0x200 falls through)
17add0: f000 bdae  b.w    17b930              ; -> test (far away)
17add4: f6e8 edac  blx    63930 <Mission::hasWon>   ; mission block, right after
...
17af6c: e9db 021d  ldrd   r0, r2, [fp, #116]  ; 0x400 body
17af70: 2102       movs   r1, #2
17af72: e72b       b.n    17adcc              ; BACKWARD into the shared tail
...
17b930: f8db 00f8  ldr.w  r0, [fp, #248]      ; test (after the weaponSlot body)
17b934: b1c0       cbz    r0, 17b968
17b936: ...                                   ; final tail
17b968: ...                                   ; hudTouchFlags == 0 body
```

Differences from the original: (a) the **0x200** body owns the shared `blx` (0x400 branches
back), and (b) the test is placed **after the weaponSlot body** instead of right after the shared
tail.  The mission block is in the same relative position (right after the 0x200 body) — the
baseline's cluster is only wrong in those two respects.

## 3. Mechanism: BranchFolder tail merge, then MBP re-runs `buildCFGChains`

`-mllvm -enable-tail-merge=false` leaves four separate `blx` calls (8084 B), so the merge is not
in the IR.  It is done by the `BranchFolder` that `MachineBlockPlacement` runs internally with
`AfterBlockPlacement=true`, followed by a second `buildCFGChains()`.

Baseline MBP/BranchFolder trace (standalone IR, `-debug-only=block-placement,branch-folder`):

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

* `bb.390` = 0x200 body, `bb.417` = 0x400 body, `bb.422` = weaponSlot body, `bb.450` = test.
* `PredBB` (the layout predecessor of the test) is the **weaponSlot body** in the baseline,
  because the weaponSlot body falls through to the test.
* `SameTails = [bb.417, bb.390]` (higher block number first).  `CreateCommonTailOnlyBlock`
  (`BranchFolding.cpp`) uses `PredBB` if it is one of `SameTails`; otherwise the
  `EstimateRuntime` tie (`ldrd` = 2 + `movs` = 1 = 3 for both) is broken by `t <= TimeEstimate`,
  so the **later index (lower block number)** wins — `bb.390` (0x200 body).
* Splitting `bb.390` leaves `bb.390 = [ldrd; movs]`, a new block `bb.467 = [blx]`, and rewrites
  `bb.417` to `[ldrd; movs; b bb.467]`.  The second `buildCFGChains()` then decides which body
  is chained in front of `bb.467` (see §4).

The `goto afterHudTouchTest` / `else { if (wingman) ...; if (flags==0) ... }` shapes make the
weaponSlot body jump to the final tail, so `PredBB` becomes the **0x400 body** and CTOB picks
`bb.417`:

```
CTOB: i=0 %bb.417 == PredBB -> pick
Splitting %bb.417, size 1
Using common tail in %bb.466 for %bb.390
```

i.e. the first pass *does* flip the merge.  But the second `buildCFGChains()` re-places the
0x200 body in front of the shared tail (HBLP, §4), so the final layout still has
`[0x200 body][blx]`.

## 4. The HBLP near-miss (exact integer arithmetic)

`MachineBlockPlacement::hasBetterLayoutPredecessor` (LLVM 7.0.0, same paths as 7.0.2):

```cpp
BranchProbability HotProb = getLayoutSuccessorProbThreshold(BB);   // StaticLikelyProb = 80 %
BlockFrequency CandidateEdgeFreq = MBFI->getBlockFreq(BB) * RealSuccProb;
...
BlockFrequency PredEdgeFreq = MBFI->getBlockFreq(Pred) * MBPI->getEdgeProbability(Pred, Succ);
if (PredEdgeFreq * HotProb >= CandidateEdgeFreq * HotProb.getCompl())  // '<=': '>='
  BadCFGConflict = true;
```

`BranchProbability(80,100)` is stored as `N / D`, `D = 2^31`:

```
N = (80 * 2^31 + 50) / 100 = 1717986918
C = D - N                  =  429496730
```

`BlockFrequency::operator*` uses `BranchProbability::scale`, an exact 128-bit floor division.
The natural if/else-chain BFI frequencies are `F200 = E/2`, `F800 = E/4`, `F400 = E/8` with
`E = 3708922920`, so `F200 = 1854461460`, `F400 = 463615365`:

```
F400 * N / D = 370892291.9136449  -> 370892291      (prededge * HotProb)
F200 * C / D = 370892292.3454204  -> 370892292      (candedge * HotProb.getCompl())
370892291 >= 370892292  is FALSE  -> no conflict -> the 0x200 body chains into the shared tail
```

Because `0.8 * F400` has fractional part `(F400 mod 5) * 0.2` and the `N/D = 0.79999999953`
error contributes a fixed `-0.216`:

```
conflict  <=>  F400 mod 5 in {2, 3, 4}
```

The measured `F400 = 463615365 = 5 * 92723073` is `0 mod 5`, i.e. the one residue class out of
five that is guaranteed **not** to conflict.  Equivalently the boundary is
`F200 <= 4*F400 - 2` (the previous w4007 note's `-1` was off by one).

The **second** HBLP check (`shared tail -> test`, pred = 0x800 body) is structural:

```
pred  = 927230730 * N/D = 741784583
cand  = 2318076825 * C/D = 463615365        (f(shared tail) = F200 + F400)
conflict  (741784583 >= 463615365)
```

To avoid it the shared tail would need `freq >= 3708922912`, i.e. 60 % more than
`F200 + F400 = 5/8 * E`.  With the natural if/else weights (`F800 = E/4`) this is impossible, so
the second pass can never chain the test after the shared tail unless the 0x800 body is not a
predecessor of the test (or its frequency is biased).

Both numbers are properties of the whole-function BFI.  `convertFloatingToInteger`
(`lib/Analysis/BlockFrequencyInfoImpl.cpp`) scales by `Min.inverse() << 3`, so the integer scale
is `8 / min_block_float`; the coldest block is one of the ~22 blocks in the big shared-tail
merge group (float 1.231230818e-11, int 8).  A shift of `E` by ~16 units (relative 4e-9) or of
`F400` by 2 units would flip the conflict.  Reordering the *source* blocks does **not** change
it: the `if (!(flags & 0x200)) { chain } else { initHudMenu(1); }` (Ghidra) form gives 78.4 %
/ 8080 B, identical to the baseline.

## 5. The `expi` form: the first shape that really makes the 0x400 body the common tail

Source (diff against the pristine `src/game/menu/MGame.cpp`; only the `hudMenuOpen` block and
the `hudTouchFlags == 0` block change):

```cpp
        if (!(this->hudTouchFlags & 0x200)) {
            if (!(this->hudTouchFlags & 0x800)) {
                if (!(this->hudTouchFlags & 0x400)) {
                    if (this->hudTouchFlags & 0x2000) {
                        weaponSlot = 0;
                    } else if (this->hudTouchFlags & 0x4000) {
                        weaponSlot = 1;
                    } else if (this->hudTouchFlags & 0x8000) {
                        weaponSlot = 2;
                    } else if (this->hudTouchFlags & 0x10000) {
                        weaponSlot = 3;
                    } else if (this->hudTouchFlags & 0x20000) {
                        wingmanCommand = 1;
                    } else if (this->hudTouchFlags & 0x40000) {
                        wingmanCommand = 3;
                    } else if (this->hudTouchFlags & 0x80000) {
                        wingmanCommand = 2;
                    } else if (this->hudTouchFlags & 0x100000) {
                        uint8_t &wingmenFollow = reinterpret_cast<uint8_t &>(Globals::status->field_f8);
                        wingmenFollow ^= 1;
                        wingmanCommand = 0;
                    } else if (this->hudTouchFlags & 0x1000) {
                        UseKhadorDrive();
                    }
                } else {
                    this->hud->initHudMenu(2, this->level);
                }
            } else {
                this->hudMenuOpen = 0;
                this->hud->closeHudMenu();
                this->pauseOpen = 0;
                resumeSounds();
                useCloak();
            }
        } else {
            this->hud->initHudMenu(1, this->level);
        }
        if (weaponSlot >= 0) {
            ...
            this->hud->closeHudMenu();
            goto afterHudTouchTest;          // <-- weaponSlot skips the test
        } else if (wingmanCommand >= 0) {
            ...
        }
        if (this->hudTouchFlags == 0) {
            this->hudMenuOpen = 0;
            this->hud->closeHudMenu();       // <-- unguarded (original has no null check)
            ...
        }
    afterHudTouchTest: ;
```

Emitted cluster (objdump of `goto_expi.so`, 8084 B):

```
17adbc: e9db 021d  ldrd   r0, r2, [fp, #116]  ; 0x200 body
17adc0: 2101       movs   r1, #1
17adc2: e0cc       b.n    17af5e              ; forward to shared blx
17adc4: f6e8 edb4  blx    63930 <Mission::hasWon>   ; mission block, right after
...
17ae8a: ...                                   ; 0x800 body
...
17af58: e9db 021d  ldrd   r0, r2, [fp, #116]  ; 0x400 body
17af5c: 2102       movs   r1, #2
17af5e: f6ed ea0e  blx    6837c <Hud::initHudMenu>  ; shared tail (0x400 falls through)
17af62: f8db 00f8  ldr.w  r0, [fp, #248]      ; test
17af66: b328       cbz    r0, 17afb4
17af68: ...                                   ; final tail
17af84: ...                                   ; epilogue
17afb4: ...                                   ; hudTouchFlags == 0 body
```

This is the original cluster: `[0x400 body][shared blx][test][final tail][epilogue][hud0 body]`,
with the 0x200 body branching forward into the shared `blx`.  **The 0x400 body is the common
tail.**  The `goto` is what makes `PredBB` the 0x400 body in the first BranchFolder pass; the
`if (!(flags & 0x200))` inversion is what keeps the 0x200 body's IR block *after* the 0x400
body, so the second `buildCFGChains()` also keeps the 0x400 body in front of the shared tail
(this is the piece the plain flat chain + `goto` does not do).

## 6. Why `expi` still loses: the mission-continuation block moves 0x80 bytes

Aligning the original and `goto_expi` instruction streams (mnemonic + operands, addresses
ignored) gives the address delta `orig_addr - expi_addr`:

```
delta 0x0    from 0x17a144 to 0x17a5e8   (mining block: -4 / -2 / 0)
delta 0x0    from 0x17a5fe to 0x17a7fa
delta +0x4   at   0x17a7fa               (b.w+nop -> b.n at 0x17a7f2)
delta +0x4   at   0x17adc0
delta +0x2   at   0x17adec
delta +0x2   at   0x17af4a
delta +0x80  at   0x17afd8               <-- the whole cluster jumps 0x80 early
```

The jump is the **mission-continuation block** (`else if (getMission()->hasWon() || ...)`, the
`getMission / hasWon / isInstantActionMission / isCampaignMission` chain).  In the original it is
at `0x17af5a`, immediately before the 0x400 body (with the literal pool at 0x17afc0 in
between).  In `expi` it is at `0x17b04a`, *after* the epilogue.  That block is ~0x7E bytes, which
is exactly the 0x80 shift.  Everything after the cluster is then shifted, so every intra-function
branch target differs and the official (branch-target-literal) score drops to **76.0 %** even
though the content is closer to the original (call-sequence difflib ratio 88.24 % vs the
baseline's 85.22 %).

The `expi` build is also 8084 B (+4): the shifted epilogue turns the far `b.n` at 0x17a7f2 into
a `b.w`+nop somewhere else, and the extra 4 bytes propagate.

## 7. Measured variants (all reverted; baseline 78.4 % / 8080 B)

Official `tools/verify/verify.py` (`--top 0`, autojunk=False), OnTouchEnd row only.

| variant | source shape | size | score |
|---|---|---|---|
| baseline | flat chain, `count > 0`, weaponSlot falls into the test | 8080 | **78.4** |
| `v_A` | Ghidra inverted outer test, flat rest | 8080 | 78.4 |
| `v_FW` | weaponSlot body ordered `pauseOpen; hudMenuOpen; resumeSounds; closeHudMenu` | 8080 | 78.3 |
| `v_CAMPAIGN` | 3rd `getCampaignMission()` + `isInstantActionMission` before `isCampaignMission` | 8080 | 78.1 |
| `v_EXPI_HUD` | inverted outer test + `goto` + unguarded hud block | **8080** | **76.3** |
| `v_EXPI_HUD_CAMP` | `v_EXPI_HUD` + campaign fix | 8080 | 76.1 |
| `goto_expi` | inverted outer test + `goto` + guarded hud block | 8084 | 76.0 |
| `v_GOTO_HUD` | flat chain + `goto` + unguarded hud block | 8080 | 74.9 |
| `v_GOTO_HUD_C3` | + campaign fix | 8084 | 73.8 |
| `v_GOTO_C3` | flat chain + `goto` + campaign fix | 8088 | 75.1 |
| `v_SEPIF` | mission continuation split into a separate `if` | 8084 | 76.6 |
| `v_FT` | unguarded hud block only | 8076 | 76.6 |
| `v_FWFT` | `FW` + `FT` | 8076 | 76.6 |
| `v_EXPI_HUD_MINE` | `v_EXPI_HUD` + mining `amount != 0` | 8076 | 70.0 |
| `v_TESTFIRST` | `flags == 0` block before the weaponSlot handling | 8056 | 72.5 |
| `v_TESTFIRST2` | `flags == 0` block before the flag chain | 8092 | 73.4 |
| `v_FLAGSWITCH` | `switch (hudTouchFlags)` on the full value | 8104 | 72.2 |

A full 2^5 grid over `{invert, goto, FW, FT, campaign}` was measured; the best cell is the
baseline / `invert` at 78.4 %.  Invert alone is a no-op on the layout; every `goto` cell loses
the cluster 0x80 shift.

## 8. Content deltas that cannot be landed alone

Branch-target-normalised difflib on the baseline: **ratio 79.94**, orig 2775 / ours 2774
normalised instructions.  The largest original-only blocks (instruction indices):

| indices | size | block |
|---|---|---|
| 1959–1988 | 30 | mission / instant-action continuation |
| 1728–1834 | 106 | sound / stop-engine block |
| 1263–1318 | 55 | 0x400 body + `flags == 0` body |
| 2268–2300 | 32 | autopilot / `hudEvent` |
| 2135–2166 | 31 | starmap / jump-cancel |

These are all **block reorderings**, not missing code (the instruction multiset is essentially
identical; only ~1 raw instruction differs, the mining counter).  The two genuine content
deltas are:

* **Mission continuation** — the original computes
  `mission = getMission()->hasWon() ? getMission() : getCampaignMission()` (a 3rd
  `getCampaignMission` call) and tests `isInstantActionMission()` *before* `isCampaignMission()`.
  Applying it gives content ratio 81.28 -> 81.73 and the exact 2775 instruction count, but the
  official score drops to 78.1 % because the layout shifts.  Not landed.
* **Mining loop** — the original keeps a negative counter and tests `amount != 0`
  (`cmp r9,#0; beq`), ours is `int count = -amount; if (count > 0)` (`cmp r9,#-1; bgt`).
  The faithful spelling `if (amount != 0) String(-amount)` is **size-negative** (8080 -> 8076),
  which breaks `byte_exact` (4499 -> 4478) unless compensated by +4 B elsewhere.  Not landed.

## 9. What remains

1. The 0x400 body *can* be made the common tail faithfully (`expi`, §5).  The blocker is now the
   **mission-continuation block placement**: it must stay at `0x17af5a` (immediately before the
   0x400 body) instead of moving to `0x17b04a` (after the epilogue).  That block is a successor
   of the mission-success block (`0x17adc8`) and MBP places it with the literal pool.
2. The second HBLP check (`shared tail -> test`, pred = 0x800 body) is structural and cannot be
   avoided with the natural `F800 = E/4`; the test is chained after the 0x800 body in every
   `goto` shape.
3. The two faithful content deltas (§8) are correct against Ghidra but are score-neutral or
   score-negative on their own; they need the layout fixed first.
4. A source shape that keeps the mission continuation at `0x17af5a` *and* the 0x400 body as the
   common tail would land the original cluster; no such shape was found in this pass.

## 10. Reproduction

```
# baseline gate
cmake --build cmake-build-match --target check -j8        # RATCHET PASS, byte 4499
# per-function row
python3 -c "import json;r=json.load(open('cmake-build-match/verify/report.json'));print([x for x in r['rows'] if x['name']=='_ZN5MGame10OnTouchEndEiiPv'])"
# HBLP arithmetic (D = 2^31)
python3 -c "D=1<<31;N80=(80*D+50)//100;N20=D-N80;p=463615365;c=1854461460;print(p*N80//D,c*N20//D)"
#   -> 370892291 370892292   (no conflict by exactly 1)
```

Scratch harness (not tracked): `/dev/shm/w4255/` — `measure.sh`, `score_target.py`,
`content_diff.py`, `emit_ll_oz.sh`, `dbg.sh`, `gen_combo.py`, the `c*.cpp` grid, `v_*.cpp`
variants, `goto_expi.so`, `goto_expi_full.dis`, `orig_full.dis`, `base2_full.dis`,
`ote_oz_trace.txt`.
