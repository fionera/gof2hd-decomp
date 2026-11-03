# MGame::OnTouchEnd (8080 B) — w4604 (wave 96): the mission-continuation block
# *can* be placed before the 0x400 body, but every shape that does so is a
# score regression; the blocker is a hard BFI ratio, not a source-shape puzzle

Target: `_ZN5MGame10OnTouchEndEiiPv` (Ghidra entry 0x18a144, file VMA 0x17a144, 8080 B).
Baseline (unchanged by this pass): **78.4 %**, 8080 B, 2775 original / 2774 ours
branch-target-normalised instructions.  Gate unchanged and green:
byte_exact **4500** / linked_exact **4500** / size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, `.text` 0x1337c8.

This pass lands **no code change**.  It re-derives the original cluster from the binary,
records the *exact* MachineBlockPlacement block frequencies that decide the order, and
measures 19 further source shapes.  The main new result is negative but decisive:

> "MC before the 0x400 body" is **achievable** (three independent shapes do it), but in
> every one of them the score drops.  The ordering is decided by a single frequency
> comparison `freq(keepObjectives false arm) > freq(0x400 body)` that the zero-heuristic
> pins at `0.375 * F_ko` vs `0.5 * F_400test`; with the witnessed `hasWon()` branch
> probabilities the inequality cannot be satisfied without changing the branch predicate
> (and hence the emitted `cmp/beq`).

Prior notes: `mgame-ontouchend-wave94-w4409.md` (baseline cluster + HBLP arithmetic),
`mgame-ontouchend-78-1-8080-b_w3807.md`, `mgame-ontouchend-tailmerge-branchfolder_w3826.md`,
`mgame-ontouchend-tailmerge-mbp-reorder_w4007.md`, `mgame-ontouchend-hblp-1unit-w4107.md`,
`mgame-ontouchend-wave92-pass7-w4234.md`, `mgame-ontouchend-wave92-w4255.md`.

## 0. TU / source-layout witness

`source file list dump/gof2hd_1.1.19_ios.cpp` confirms only that `Classes/Game/MGame.cpp`
owns `_MGame::OnTouchEnd(int, int, void*)`; there are no TU-local statics for this function.
Everything below is reconstructed from the Android 2.0.16 binary (Ghidra + objdump).

## 1. The original cluster (authoritative objdump, file VMA 0x17a144)

Block map (`origblocks.py`, offsets relative to 0x17a144):

```
0x0c02  dispatch (hudMenuOpen) + UseKhadorDrive arm      (one contiguous run 0xc02..0xc64)
0x0c64  freeCamTouchEnd                       (else of `if (hudMenuOpen)`)
0x0c7c  0x200 body   ldrd r0,r2,[fp,#116]; movs r1,#1; b.n 0xe9a
0x0c84  MS           hasWon | getCampaignMission | hasWon
0x0c9c  keepObjectives TRUE arm  getCampaignMission|hasWon|getMission|isCampaignMission
0x0cc4  touchesStream (dockToStream ...)
0x0d48  0x800 body   closeHudMenu|resumeSounds|useCloak
0x0d68  maneuverTouchEnd
0x0d7c  menuWindow cluster
0x0dee  setAutoPilotToProgrammedStation
0x0e00  starMap cleanup   StarMapD1 + _ZdlPv
0x0e0e  starMap = nullptr; b.n epilogue
0x0e16  keepObjectives FALSE arm   mov.w sl, #0
0x0e1a  MC chain     getMission|hasWon|getMission|isInstantActionMission|isCampaignMission|nextCampaignMission
0x0e94  0x400 body   ldrd r0,r2,[fp,#116]; movs r1,#2   (falls through)
0x0e9a  shared blx   Hud::initHudMenu
0x0ea4  test         if (hudTouchFlags == 0)
0x0ec0  final tail   isAutoPilot / return
0x0ed2  __stack_chk_fail
0x0ed6  hud0 body    closeHudMenu (UNGUARDED)|resumeSounds|resetGunDelay
0x0f3a  mission-failed freelance block
0x0f72  MC freelance branch (setFreelanceMission|getReward|getBonus|showMissionRewardMessage)
0x0f9e  campaign-mission switch chain
```

So the original's order around the target is

```
[starMap cleanup][keepObjectives FALSE][MC chain][0x400 body][shared blx][test]
```

and ours is

```
[starMap cleanup][0x400 body][mission-failed freelance][changeCredits][bb.230]
[keepObjectives FALSE][MC chain]
```

In the branch-target-normalised instruction stream the original's MC block starts at
index 1222 (`mov.w sl, #0`) and runs to 1257; ours starts at 1264 and runs to 1299.
Where the original has the MC block (1222) ours has the 0x400 body (1224..1226).

## 2. Why MBP puts the 0x400 body first — exact frequencies

`llc -O2 -print-after=block-placement` (LLVM 7.0.0, thumb/cortex-a8) plus
`-debug-only=block-placement` gives the *second* (post-RA) placement run.  Block
frequencies from that run (E = 3708922921 = freq of the `hudMenuOpen` dispatch block):

| block | bb | freq | note |
|---|---|---|---|
| 0x200 test | 188 | 3708922921 | E |
| 0x800 test | 189 | 1854461460 | E/2 |
| 0x400 test | 190 |  927230730 | E/4 |
| 0x2000 test | 191 |  463615365 | E/8 |
| 0x200 body | 204 | 1854461460 | |
| shared tail | 205 | 2318076825 | |
| MS (getMission hasWon) | 206 | 941102413 | |
| keepObjectives test | 208 |  808759886 | |
| keepObjectives TRUE arm | 209 |  505474929 | 0.625 * 808759886 |
| **keepObjectives FALSE arm** | **231** | **303284957** | **0.375 * 808759886** |
| mission ternary | 232 |  808759886 | |
| **0x400 body** | **227** | **463615365** | **0.5 * 927230730** |
| mission-failed freelance | 228 |  392126005 | |
| starMap cleanup | 225 | 1568504022 | |

`selectBestCandidateBlock` picks the highest-frequency chain head from `BlockWorkList`
(`MachineBlockPlacement.cpp:1582`).  After the starMap-cleanup chain is placed the
worklist still holds both the 0x400 body (463615365) and the MC chain head
(303284957), so the 0x400 body wins:

```
463615365 / 303284957 = 1.5286
```

The MC chain head is the `keepObjectives` **false** arm, because that arm is a critical-edge
split of the `phi` at the ternary merge (`%700 = phi i1 [%699, %695], [false, %690]` in the
IR), so it is its own MBB and its own chain head.  Its frequency is exactly the branch
probability of `campaign->hasWon() == 0`.

### The zero heuristic pins that probability at 37.5 %

`BranchProbabilityInfo::calcZeroHeuristics` (LLVM 7.0.0) gives
`icmp ne X, 0` -> taken 20/32 = 62.5 %, `icmp eq X, 0` -> taken 12/32 = 37.5 %.
For `bool keepObjectives = campaign->hasWon() ? !... : false;` clang emits

```
%694 = icmp eq i8 %693, 0
br i1 %694, label %700, label %695      ; %700 = merge (false arm), %695 = true arm
```

i.e. the false arm is the *taken* edge of an `icmp eq`, so it is 37.5 % no matter how the
ternary is spelled (`?:`, `if/else`, `&&`, `bool x = false; if (...) x = ...;` — all four
were measured and are byte-identical: 78.4 %, 8080 B).

For MC to be picked first the false arm would need `> 463615365`, i.e. `> 0.573 * F_ko`;
the only way is a 62.5 % branch, which requires the *taken* edge to be the false arm — that
is `br (X != 0), %false_arm, %true_arm`, i.e. the semantically wrong
`hasWon() ? false : !isCampaignMission()`.

## 3. "MC before the 0x400 body" is achievable — and always worse

Three independent shapes do move the MC chain in front of the 0x400 body:

| shape | MC block pos | 0x400 body pos | official | size |
|---|---|---|---|---|
| baseline | 238 | 228 | **78.4** | 8080 |
| `m3v` mission ternary first, then `isInstantAction`, then `keepObjectives` | **211** | 231 | 78.2 | 8080 |
| `d1` hudMenuOpen arm order `A B D C E F G H I J K L` (0x400 becomes 4th test) | **250** | 262 | 75.8 | 8072 |
| `v_eq1` `campaign->hasWon() == 1` (50/50 instead of 62.5/37.5) | 235 | 228 | 75.1 | 8084 |

`m3v` puts the MC chain into the *main* chain right after `keepObjectives` (the `phi`
disappears, so the mission ternary block becomes the successor of the true arm and no
HBLP conflict exists) — MC ends up 17 blocks too early.  `d1` lowers the 0x400 test to the
4th slot (E/8 -> 0x400 body E/16 = 231807682 < 303284957) and MC does move in front of it,
but the reordered dispatch tests cost far more than the MC block gains.  `v_eq1` confirms
the frequency model exactly: 404379943 > 392126005 moves MC in front of the
mission-failed freelance block, but it is still < 463615365 so the 0x400 body stays first.

The target position is block ~228 (immediately after the starMap cleanup, instruction
~1224).  That needs `freq(MC head)` in `(463615365, 588189008)` — a narrow window that the
witnessed branch probabilities cannot reach.

## 4. Measured variants (official `tools/verify/verify.py`, OnTouchEnd row only)

All variants patch only the mission-success region (source lines 1555-1572) or the
`if (this->hudMenuOpen)` arm order (lines 2062-2091); everything else is baseline.

| variant | source shape | size | official | insns |
|---|---|---|---|---|
| baseline | `campaign` local, `?:` keepObjectives, isCampaign before isInstantAction | 8080 | **78.4** | 2775/2774 |
| `v_ko_init` | `bool keepObjectives = false; if (campaign->hasWon()) keepObjectives = !...;` | 8080 | 78.4 | 2775/2774 |
| `v_koA` | `bool keepObjectives = campaign->hasWon(); if (keepObjectives) keepObjectives = !...;` | 8080 | 78.4 | 2775/2774 |
| `v_koB` | `campaign->hasWon() && !isCampaignMission()` | 8080 | 78.4 | 2775/2774 |
| `v_koC` | `!isCampaignMission() && campaign->hasWon()` | 8092 | 69.1 | 2775/2780 |
| `m1v` | `mission` ternary written as `if/else` | 8080 | 78.4 | 2775/2774 |
| `cF` | faithful: no `campaign` local, isInstantAction **before** isCampaign | 8080 | 78.1 | 2775/2775 |
| `m5v` | `cF` + `mission` ternary as `if/else` | 8080 | 78.1 | 2775/2775 |
| `m6v` | no `campaign` local, isCampaign first (baseline order) | 8080 | 78.3 | 2775/2773 |
| `m3v` | mission ternary first, isInstantAction, keepObjectives, isCampaign | 8080 | 78.2 | 2775/2778 |
| `m7v` | mission ternary, keepObjectives, isInstantAction, isCampaign | 8084 | 75.1 | 2775/2778 |
| `v3` | isCampaign dispatch moved after `changeCredits` + `m_nTimeLimit` | 8080 | 77.7 | 2775/2775 |
| `m4v` | `cF` + `changeCredits`/`m_nTimeLimit` before the isCampaign dispatch | 8080 | 77.6 | 2775/2774 |
| `r_failed` | `if (!failed) { if (hasWon...) {...} else {...} } else { ...failed... }` | 8080 | 78.3 | 2775/2774 |
| `o1`/`o2` | outer `else if` tests `getCampaignMission()->hasWon()` first | 8084 | 76.5 | 2775/2775 |
| `v_vol` | `volatile bool keepObjectives` (forces a stack slot, kills the phi) | 8052 | 75.7 | 2775/2761 |
| `v_eq1` | `campaign->hasWon() == 1` (50/50 heuristic) | 8084 | 75.1 | 2775/2775 |
| `d1` | arm order `A B D C E F G H I J K L` | 8072 | 75.8 | 2775/2772 |
| `d2` | arm order `A B D E F G H I J K L C` (0x400 last) | 8068 | 73.8 | 2775/2772 |
| `d3` | arm order `A C B D E F G H I J K L` (0x800 third) | 8068 | 70.9 | 2775/2771 |

`cF` is still the most *faithful* shape (exact 2775/2775 instruction multiset, matching the
Ghidra order) but scores 78.1 because the campaign fix perturbs the block placement.

## 5. What remains

1. The MC/0x400 ordering is decided by `freq(keepObjectives false arm) > freq(0x400 body)`,
   i.e. `0.375 * F_ko > 0.5 * F_400test`.  With the witnessed `hasWon()` zero-heuristic
   (62.5/37.5) and the witnessed 3rd-position bit test (E/4 -> body E/8) this is
   `303284957 > 463615365`, false by a factor 1.53.
2. The only levers that flip it also change emitted code (dispatch arm order, `cmp #1`,
   `volatile`, moving the ternary into the main chain) and each costs more than the
   ~40-instruction MC alignment is worth.
3. Therefore no strict improvement was found in this pass.  The baseline remains the best
   scoring shape at 78.4 % / 8080 B.

## 6. Reproduction

```
# gate
cmake --build cmake-build-match --target check -j8        # RATCHET PASS, byte 4500
# per-function row
python3 -c "import json;r=json.load(open('cmake-build-match/verify/report.json'));print([x for x in r['rows'] if x['name']=='_ZN5MGame10OnTouchEndEiiPv'])"
# MBP block order + frequencies (llc 7.0.0; strip `variables:` fields first)
sed 's/, variables: ![0-9]*//' MGame.ll > MGame2.ll
/home/debian/llvm7build/build/bin/llc -march=thumb -mcpu=cortex-a8 \
  -mtriple=thumbv7-none-androideabi16 -O2 -debug-only=block-placement MGame2.ll -o /dev/null
# the second (post-RA) run is the one that matters; the `[MBP] Function:` listing that
# follows it is the final block order.
```

Scratch harness (not tracked): `/dev/shm/w4604/` — `cc_ll.sh` (exact repo flags),
`one.py` (official per-function score), `run.sh` / `runall2.sh` (build+measure),
`order2.sh` + `order3.py` (MBP block order with tags), `align.py` (difflib opcode summary),
`mbpgen.sh` (full `-debug-only=block-placement` dump), `gen_mc.py` / `gen_disp.py` /
`gen_ko.py` / `gen_outer.py` (source-shape generators), and the candidate files
`v_*.cpp`, `m*.cpp`, `d*.cpp`, `o*.cpp`, `cF.cpp`, `r_failed.cpp`.
