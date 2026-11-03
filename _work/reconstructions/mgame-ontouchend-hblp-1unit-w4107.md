# MGame::OnTouchEnd (8080 B) — w4107: corrected HBLP arithmetic, the 1-unit near-miss,
# and why the `goto` shape flips the merge but still loses

Target: `_ZN5MGame10OnTouchEndEiiPv` (Ghidra entry 0x18a144, file VMA 0x17a144, 8080 B).
Baseline (unchanged by this pass): **78.4 %**, 8080 B.
Gate unchanged: byte_exact 4498 / linked_exact 4498, size 4524/4524, data 843/843, imports 207,
`.text` 0x1337c8, `.rodata` 0x41b67, `.ARM.exidx` 0x6f70, `.ARM.extab` 0x26290, lint CLEAN, drift 0.

This pass landed **no code change**. It corrects the HBLP arithmetic of the prior note
(`mgame-ontouchend-tailmerge-mbp-reorder_w4007.md` got `D` wrong), proves the merge-flip is a
**1-unit near-miss** at `StaticLikelyProb = 80`, and measures every source shape that flips it.
The flip does produce the original `[0x400 body][shared blx][test]` cluster, but MBP then places
the *mission block* (`getMission()->hasWon() ? getMission() : getCampaignMission()`) somewhere
else, the function grows to 8084 B, and the fuzzy score drops.  Baseline stays the best shape.

## 1. Corrected HBLP arithmetic (`BranchProbability::D == 2^31`, not 2^32)

LLVM 7 `BranchProbability` stores the numerator over `D = 1u << 31 = 0x80000000`
(`BranchProbability.h: static const uint32_t D = 1u << 31;`).  `StaticLikelyProb = 80` gives

```
N80 = (80*D + 50)/100 = 1717986918
N20 = D - N80        =  429496730
```

and `hasBetterLayoutPredecessor` (MBP.cpp, LLVM 7.0.0 == 7.0.2 here) fires the conflict when

```
floor(PredEdgeFreq * N80 / D) >= floor(CandidateEdgeFreq * N20 / D)
```

(the code is `if (PredEdgeFreq * HotProb >= CandidateEdgeFreq * HotProb.getCompl())`; the
operator is `BlockFrequency * BranchProbability` which is a 64-bit multiply then a `udiv` by `D`).

### 1.1 The near-miss at the second `buildCFGChains` (goto shape, bb.204 -> bb.227)

```
HBLP: BB=%bb.204 L0 f=1854461460 Succ=%bb.227 L0 f=2318076825 unsched=1
HBLP pred %bb.226 f=463615365
HBLP   prededge=463615365 *hp=370892291 candedge=1854461460 *cp=370892292
```

- `463615365 * 1717986918 / 2147483648 = 370892291.9136...` -> **hp = 370892291**
- `1854461460 *  429496730 / 2147483648 = 370892292.3454...` -> **cp = 370892292**

`hp < cp` by exactly **1** -> **no conflict**, so the 0x200 body (bb.204) wins the tail
(bb.227) and MBP chains `[0x200 body][shared blx]`; the 0x400 body (bb.226) is left as a
separate block that branches into the shared tail.  That is the baseline layout.

A one-unit change of `prededge` flips it:

| prededge | hp | cp | conflict |
|---|---|---|---|
| 463615364 | 370892291 | 370892292 | no |
| **463615365** | **370892291** | **370892292** | **no** |
| 463615366 | 370892292 | 370892292 | **yes** |
| 463615367 | 370892293 | 370892292 | yes |

The frequencies are the natural if/else-chain weights: with `entry = 3708922920`,
`0x200 = entry/2`, `0x800 = entry/4`, `0x400 = entry/8`, common tail `= 0x200+0x400 = 5*entry/8`.
Because `0x400 == 0x200/4` exactly, `hp` and `cp` differ only by the floor of a 0.43-wide
fraction — hence the 1-unit margin.

### 1.2 The structural conflict that survives the flip (bb.227 -> bb.228, the test)

```
HBLP: BB=%bb.227 L0 f=2318076825 Succ=%bb.228 L2115 f=3274283516 unsched=2
HBLP pred %bb.211 (0x800 body) f=927230730
HBLP   prededge=927230730 *hp=741784583 candedge=2318076825 *cp=463615365
      CONFLICT pred %bb.211 predfreq=927230730 candfreq=2318076825
    Not a candidate: %bb.228 -> 100.00% (prob) (non-cold CFG conflict)
```

`927230730*0.8 = 741784584 >= 2318076825*0.2 = 463615365` — **structural**, not a near-miss.
For the common tail to win the test it would need `candfreq >= 4 * 0x800_freq`, i.e.
`>= entry`, impossible since `common tail <= entry`.  This is why, even after the merge flips,
the test is not chained to the shared tail: MBP merges the tail to the *next block in the block
list* instead, and the test is eventually placed after the 0x800 body (baseline) or after the
0x400 body (goto shape).

## 2. The original cluster (authoritative objdump)

```
17adc0: e9db 021d  ldrd    r0, r2, [fp, #116]      ; 0x200 body
17adc4: 2101       movs    r1, #1
17adc6: e10a       b.n     17afde                  ; forward to shared tail
...
17ae8c: ...                                         ; 0x800 body (closeHudMenu/resumeSounds/useCloak)
17aeaa: e09a       b.n     17afe2                  ; -> test (branch, not fall-through)
...
17afd8: e9db 021d  ldrd    r0, r2, [fp, #116]      ; 0x400 body
17afdc: 2102       movs    r1, #2
17afde: f6ed e9ce  blx     6837c <Hud::initHudMenu>; shared tail (0x400 falls through)
17afe2: f8db 00f8  ldr.w   r0, [fp, #248]          ; test: if (hudTouchFlags == 0)
17afe6: b1c0       cbz     r0, 17b01a
17afe8: ...                                         ; final tail (isAutoPilot / return)
...
17b01a: ...                                         ; hudTouchFlags == 0 body
17b04e: e7cb       b.n     17afe8
...
17b9da: ...                                         ; weaponSlot body
17ba24: ... b.w 17afe8                              ; -> final tail (skips the test)
```

So the original cluster is `[0x400 body][shared blx][test][final tail][hud==0 body]`, with the
0x200 body far earlier branching forward into the shared tail, and the weaponSlot body jumping
straight to the final tail.  (The weaponSlot path is only reachable with a nonzero
`hudTouchFlags`, so skipping the `== 0` test is correct and the original compiler did it.)

## 3. The baseline cluster (ours, 78.4 %)

```
17adc6: e9db 021d  ldrd    r0, r2, [fp, #116]      ; 0x200 body
17adca: 2101       movs    r1, #1
17adcc: f6ed ead6  blx     6837c <Hud::initHudMenu>; shared tail (0x200 falls through)
17add0: f000 bdae  b.w     17b930                  ; -> test
...
17ae9a: ...                                         ; 0x800 body
...
17af6c: e9db 021d  ldrd    r0, r2, [fp, #116]      ; 0x400 body
17af70: 2102       movs    r1, #2
17af72: e72b       b.n     17adcc                  ; BACKWARD into the shared tail
...
17b930: f8db 00f8  ldr.w   r0, [fp, #248]          ; test (far away, after weaponSlot body)
17b952: ...                                         ; epilogue
```

The two differences from the original are (a) which body owns the shared `blx` (0x200 vs 0x400)
and (b) where the test lands (after the weaponSlot body vs after the shared tail).

## 4. The `goto afterHudTouchTest` shape: flips the merge, but not enough

Adding `goto afterHudTouchTest;` after the weaponSlot body (so that body jumps to the final tail,
matching the original CFG) makes the test's fall-through predecessor the **0x400 body**, so
`CreateCommonTailOnlyBlock` picks `PredBB` and the 0x400 body becomes the common tail:

```
CTOB: i=0 %bb.417 == PredBB -> pick
Splitting %bb.417, size 1
Using common tail in %bb.466 for %bb.390
```

With `-static-likely-prob=81` (or the local patched-llc `FORCED CONFLICT` hack at
`MachineBlockPlacement.cpp:1437`) the second `buildCFGChains` also rejects the 0x200 body's claim
on the tail, and the emitted `.s` is exactly the original cluster:

```
.LBB22_206:  ldrd r0, r2, [r11, #116]      ; 0x200 body
             movs r1, #1
             b    .LBB22_229               ; forward to shared tail
...
.LBB22_213:  ... closeHudMenu/resumeSounds/useCloak
             b    .LBB22_230               ; forward to test
...
.LBB22_228:  ldrd r0, r2, [r11, #116]      ; 0x400 body
             movs r1, #2
.LBB22_229:  bl   Hud::initHudMenu         ; shared tail (0x400 falls through)
.LBB22_230:  ldr.w r0, [r11, #248]         ; test
             cbz  r0, .LBB22_234
.LBB22_231:  ... isAutoPilot ...           ; final tail
.LBB22_232:  ... stack check / return
.LBB22_233:  bl   __stack_chk_fail
.LBB22_234:  ... hudTouchFlags == 0 body
```

So the cluster is reproduced.  But the score is **76.0 % / 8084 B** (worse than baseline).
Two things go wrong:

### 4.1 The mission block moves

The original places the `keepObjectives = false; mission = getMission()->hasWon() ? getMission()
: getCampaignMission(); isInstantActionMission()` block at **0x17af5a**, immediately after the
starmap block (`0x17af44 .. 0x17af58`, `b.n 17b004`), i.e. **before** the 0x400 body.

In the goto/expect build it lands at **0x17b04a**, after the epilogue, the literal pool and the
`hudTouchFlags == 0` body.  Consequently the epilogue sits at 0x17af84 instead of 0x17b004, and
the far `b` at 0x17a7f2 grows from `b.n` to `b.w` (+4 B -> 8084).  All intra-function branch
targets after that point shift, which is what the fuzzy score measures.

### 4.2 The `b.n`/`b.w` size cliff

At 0x17a7f2 the original is `b.w 17b004` (distance 0x812 > 2048) + `nop`; ours is `b.n 17af84`
(distance 0x792 < 2048).  A 128-byte epilogue displacement is the whole +4 B.  Any shape that
moves the epilogue more than ~20 bytes closer to 0x17a7f2 flips the encoding and the size.

## 5. Mining-loop condition: the compiler canonicalises the counter

The original mining loop keeps a **negative** counter and tests `!= 0`:

```
17a58c: mov.w r9, #0
...
17a5b6: sub.w r9, r9, #1          ; amount--
...
17a5ca: cmp.w r9, #0              ; if (amount != 0)
17a5ce: beq.n 17a652
...
17a5da: rsb   r1, r9, #0          ; String(-amount)
```

Our source is `int count = -amount; if (count > 0) { ... String(count) ... }`, which emits
`cmp.w r9, #-1; bgt` — same size, same counter, one different instruction.  Rewriting it as
`if (amount != 0) { ... String(-amount) ... }` is faithful to the original but makes clang
canonicalise `amount--`/`-amount` into a **positive** counter (`add.w r8, r8, #1`) and the whole
function's register allocation flips r8/r9/r10 (measured: 71.4 % / 8076 B).  Every source spelling
tried (`if (amount)`, `if (amount < 0)`, `unsigned amount`, `for(...; amount--)`, separate
`count = -amount` with `if (count != 0)`) canonicalises the same way; only the signed
`count > 0` on a separate `int count` keeps the negative counter.  The 1-instruction delta is
therefore not independently landable without the rest of the layout.

## 6. Measured variants (all reverted; baseline 78.4 % / 8080 B)

Official verifier (`tools/verify/verify.py`, `--top 0`, autojunk=False), OnTouchEnd row only.

| variant | source shape | size | score |
|---|---|---|---|
| baseline | flat flag chain, `count > 0`, weaponSlot falls into test | 8080 | **78.4** |
| `v_goto` | + `goto afterHudTouchTest` after weaponSlot body | 8084 | 75.1 |
| `e_insta0` | goto + `__builtin_expect(mission->isInstantActionMission(), 0)` | 8084 | 76.0 |
| `g_GOTO_EXPH0` | goto + `__builtin_expect(hudTouchFlags == 0, 0)` | 8088 | 72.7 |
| `g_GOTO_EXPH1` | goto + `__builtin_expect(hudTouchFlags == 0, 1)` | 8084 | 75.1 |
| `g_GOTO_HUD_EXPI` | goto + faithful unguarded hud block + expect(insta,0) | **8080** | **76.1** |
| `g_C3_GOTO_EXPI` | goto + 3rd `getCampaignMission()` + expect(insta,0) | 8088 | 76.0 |
| `g_MINE_GOTO_EXPI` | + mining `amount != 0` | 8084 | 72.8 |
| `g_MINE_C3_GOTO_HUD_EXPI` | all content fixes + goto + expect(insta,0) | 8084 | 72.9 |
| `g_GOTO_EXPI_EXPH0` | goto + expect(insta,0) + expect(htf==0,0) | 8092 | 75.8 |
| mining only | `if (amount != 0) String(-amount)` | 8076 | 71.4 |
| `m_u_unsigned` | `unsigned amount`, `if (amount != 0)` | 8076 | 71.4 |
| `-static-likely-prob=75` | per-file `-mllvm` on MGame.cpp | 8096 | 69.6 |
| `-static-likely-prob=79` | " | 8084 | 72.8 |
| `-static-likely-prob=81` | " | 8076 | 73.4 |
| `-static-likely-prob=82` | " | 8068 | 73.1 |
| `-static-likely-prob=85` | " | 8076 | 74.8 |

Prior-session numbers are in `mgame-ontouchend-tailmerge-mbp-reorder_w4007.md` §7 and
`mgame-ontouchend-tailmerge-branchfolder_w3826.md`; they are unchanged.

Note the `-static-likely-prob` rows: they are a **whole-TU** global option, so they also perturb
every other function in MGame.cpp; none beat 78.4 even before the gate is considered.

## 7. Why the baseline still wins

The score is `difflib` over the normalised instruction stream, where every intra-function branch
target is compared as a function-relative `loc_XXX`.  A block move therefore costs *all* branches
that target it, plus every branch after the next size change.  Measured with branch targets
masked (`ldsum.py`), the baseline's layout drift is smooth:

```
delta +0x0  until 0x17a5e8 (mining block)
delta +0x4 / +0x2 (mining reorder)
delta +0x0  until 0x17ada8
delta +0x2  at 0x17ada8   (b.n -> b.w because the test is far)
delta +0x4 ... +0x1c progressively through 0x17af7e
delta -0xbc at 0x17b05c   (mission block / 0x400 body / test region)
```

The goto shape instead oscillates (+0x52 at 0x17aeac, +0x2c at 0x17af86, -0x8, -0xe, -0x18,
-0x36, -0x40, -0x64 ...), i.e. it fixes the cluster but scrambles more of the rest.

The instruction *multiset* is essentially identical in every variant (2865 vs 2865 instructions;
only ~20 distinct normalised instructions differ, mostly literal-pool words and `b.n`/`b.w`
spellings), so the entire remaining gap is block placement, not missing code.

## 8. What remains

A source shape that satisfies **all** of:

1. the 0x400 body is the common tail (needs `PredBB` = 0x400 body, i.e. the weaponSlot body must
   jump to the final tail, matching the original CFG);
2. the 0x200 body loses the bb.204 -> bb.227 HBLP conflict (needs `prededge >= 463615366`, a
   ~2e-9 relative frequency nudge, or a different BFI `Min` mass);
3. the **mission block** stays immediately after the starmap block (0x17af5a), not after the
   `hudTouchFlags == 0` body;
4. the epilogue lands at 0x17b004 so the far `b` at 0x17a7f2 stays `b.w` (size 8080).

(1) and (2) are solved by `goto` + `__builtin_expect(isInstantActionMission(), 0)`; (3) is the
open problem.  Candidate levers not yet tried:

- Move the `if (hudTouchFlags == 0)` block's source position (before/after the weaponSlot body)
  while keeping the goto — this changes which block is "next in the list" after the common tail.
- Restructure the `starMap` deletion block (`delete this->starMap; this->starMap = nullptr;`,
  MGame.cpp:1504) relative to the mission-success chain, since the mission block currently
  attaches to the starmap block in the original.
- Reconstruct the `mission` computation as a separate expression (the 3rd
  `getCampaignMission()` call) *without* the goto, and see whether the extra call changes the
  block list enough to move the mission block.

## 9. Reproduction

```
# baseline gate
cmake --build cmake-build-match --target check -j8        # RATCHET PASS, byte 4498
# per-function row
python3 -c "import json;r=json.load(open('cmake-build-match/verify/report.json'));print([x for x in r['rows'] if x['name']=='_ZN5MGame10OnTouchEndEiiPv'])"
# HBLP arithmetic (D = 2^31)
python3 -c "D=1<<31;N80=(80*D+50)//100;N20=D-N80;p=463615365;c=1854461460;print(p*N80//D,c*N20//D)"
#   -> 370892291 370892292   (no conflict by exactly 1)
# layout drift (branch targets masked)
python3 /dev/shm/w4107/ldsum.py cmake-build-match/libgof2hdaa.so
```

Scratch harness (not tracked): `/dev/shm/w4107/{ccs.sh,cc_nodbg.sh,batch.sh,ldsum.py,ldiff.py,
mdiff2.py,cluster.py,gen.py}` and the variant sources `v_*.cpp`, `e_*.cpp`, `g_*.cpp`, `t_v*.cpp`.
The local patched LLVM 7 build used for the `FORCED CONFLICT` experiment is
`/home/debian/llvm7build/build/bin/llc` (source hack at
`llvm-7.0.0.src/lib/CodeGen/MachineBlockPlacement.cpp:1437`, backup
`/dev/shm/w4107/MBP.cpp.orig`); it is scratch only and is not part of the repo.
