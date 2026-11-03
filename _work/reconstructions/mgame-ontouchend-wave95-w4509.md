# MGame::OnTouchEnd (8080 B) — w4509 (wave 95): the mission-continuation block is
# blocked from MBP's worklist by its *second* predecessor, not by frequency;
# moving it before the 0x400 body is achievable but not sufficient

Target: `_ZN5MGame10OnTouchEndEiiPv` (Ghidra entry 0x18a144, file VMA 0x17a144, 8080 B).
Baseline (unchanged by this pass): **78.4 %**, 8080 B, 2865 original / 2864 ours
branch-target-normalised instructions.
Gate unchanged and re-verified at the end of this pass: byte_exact **4500** / linked_exact
**4500**, size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0, RATCHET PASS.

This pass lands **no code change**.  It (1) identifies the exact `MachineBlockPlacement`
decision that puts the mission-continuation chain after the 0x400 body, (2) records the
real block frequencies at that decision, (3) shows that the MC chain *would* win the
frequency comparison (808 759 886 vs 463 615 365) if it were in the work list, and that
it is kept out of the work list only because its second predecessor (the
`keepObjectives = false` block) has not been placed yet, and (4) measures a source
reorder that *does* put MC before the 0x400 body — verified with
`-print-after=block-placement` — but costs 8 bytes and drops the score to 75.2 %.

Prior notes: `mgame-ontouchend-78-1-8080-b_w3807.md`,
`mgame-ontouchend-tailmerge-branchfolder_w3826.md`,
`mgame-ontouchend-tailmerge-mbp-reorder_w4007.md`,
`mgame-ontouchend-hblp-1unit-w4107.md`,
`mgame-ontouchend-wave92-pass7-w4234.md`,
`mgame-ontouchend-wave92-w4255.md`,
`mgame-ontouchend-wave94-w4409.md`.

## 0. Method / tooling

The wave-94 note used `-print-after=block-placement` on llc 7.0.0.  This pass additionally
uses the *instrumented* llc in `/home/debian/llvm7build/build/bin/llc` with

```
llc -march=thumb -mcpu=cortex-a8 -mtriple=thumbv7-none-linux-androideabi16 -O2 \
    -debug-only=block-placement MGame2.ll -o /dev/null
```

(`MGame2.ll` = `clang++ -S -emit-llvm` with the repo's exact flags, then
`sed 's/, variables: ![0-9]*//'`).  The debug stream prints, for every
`selectBestCandidateBlock` call, the whole `BlockWorkList` with each candidate's
`MBFI->getBlockFreq`, and then the chosen block.  **This is the decisive evidence**: the
work-list is *not* ordered by block number, it is a maximum-frequency scan
(`MachineBlockPlacement::selectBestCandidateBlock`, LLVM 7.0.0
`lib/CodeGen/MachineBlockPlacement.cpp:1581`).

`-debug-only=block-placement` output for a function is emitted *before* that function's
`[MBP] Function: <name>` header (the header is printed in `buildCFGChains`, after
`buildChain`).  The OnTouchEnd chain-forming trace therefore lives between the
`Pre-computing triangle chains.` marker at line 9474 and the `[MBP] Function:` header at
line 15198 of the full debug dump.

## 1. Block identities (pre-placement MIR numbering)

| MIR block | source | role |
|---|---|---|
| `bb.132` | MGame.cpp:1496 | starMap `OnTouchEnd` chain head |
| `bb.136` | MGame.cpp:1506 | `this->starMap = nullptr;` — last block of the starMap cleanup, ends in `return` |
| `bb.143` | MGame.cpp:1527 | `bool failed = ...hasFailed();` (cutsceneActive block) |
| `bb.160` | MGame.cpp:1555 | `keepObjectives` test (`getCampaignMission()->hasWon()`) |
| `bb.161` | **L0** (no source line) | `keepObjectives = false` (`mov r0, #0`), the *false* arm of the ternary |
| `bb.162` | MGame.cpp:1556 | `keepObjectives = !getMission()->isCampaignMission()` true arm |
| `bb.163` | MGame.cpp:1557 | `Mission *mission = ...` — **head of the mission-continuation (MC) chain** |
| `bb.390` | MGame.cpp:2056 | 0x200 body (`hud->initHudMenu(1, level)`) |
| `bb.415` | MGame.cpp:2058-2062 | 0x800 body |
| `bb.417` | MGame.cpp:2064 | 0x400 body (`hud->initHudMenu(2, level)`) |

## 2. The decisive `selectBestCandidateBlock` call

After `bb.136` (the starMap cleanup, which has **no successors** — it ends in `return`),
`selectBestSuccessor` returns null, so MBP falls back to `selectBestCandidateBlock`.  The
debug dump prints the whole work list (`ote_chain0b.txt:2924-2938`):

```
    %bb.143 ('') -> 0.0006034970275 (freq)
    %bb.362 ('') -> 0.0001650019406 (freq)
    %bb.417 ('') -> 0.0007135219065 (freq)      <-- 0x400 body, WINS
    %bb.419 ('') -> 0.0003567609525 (freq)
    %bb.424 ('') -> 0.0001783804762 (freq)
    %bb.426 ('') -> 0.00008919023735 (freq)
    %bb.428 ('') -> 0.00004459511868 (freq)
    %bb.430 ('') -> 0.00002229755934 (freq)
    %bb.432 ('') -> 0.00001114877967 (freq)
    %bb.434 ('') -> 0.000005574389834 (freq)
    %bb.436 ('') -> 0.000002787194148 (freq)
    %bb.266 ('') -> 0.0002036802457 (freq)
    %bb.161 ('') -> 0.0004667672322 (freq)      <-- keepObjectives=false, second
    %bb.303 ('') -> 0.0005028356096 (freq)
    %bb.278 ('') -> 0.0004190296739 (freq)
Merging from %bb.136 ('') to %bb.417 ('')
```

`bb.417` is chosen because it is the highest-frequency candidate.  Note that **`bb.163`
(the MC chain) is not in the work list at all**.

The raw `MBFI` frequencies behind the printed values (from the `HBLP:` lines) are:

| block | source | raw freq |
|---|---|---|
| `bb.390` (0x200 body) | 2056 | 1 854 461 460 |
| `bb.132` (starMap) | 1496 | 941 102 413 |
| `bb.415` (0x800 body) | 2058 | 927 230 730 |
| `bb.160` (keepObjectives test) | 1555 | 808 759 886 |
| `bb.163` (**MC chain**) | 1557 | **808 759 886** |
| `bb.162` (keepObjectives true arm) | 1556 | 505 474 929 |
| `bb.417` (0x400 body) | 2064 | 463 615 365 |
| `bb.143` (cutsceneActive) | 1527 | 392 126 005 |
| `bb.161` (keepObjectives false arm) | L0 | 303 284 957 |

## 3. Why the MC chain is not in the work list

`MachineBlockPlacement::markChainSuccessors` only pushes a block into `BlockWorkList`
when `UnscheduledPredecessors` reaches 0, i.e. **all** of its predecessors have been
placed.  `bb.163` has two predecessors:

```
bb.163 (%ir-block.794):
; predecessors: %bb.162, %bb.161
```

`bb.162` is placed early (it is the 62.5 % arm of `bb.160`), but `bb.161` is only placed
at layout position 229 (after `bb.303`), so `bb.163` stays out of the work list until
then.  This is the whole mechanism:

```
HBLP: BB=%bb.160 ('') L1555 f=808759886 Succ=%bb.161 ('') L0    f=303284957 unsched=0
    Candidate: %bb.161 (''), probability: 0x30000000 / 0x80000000 = 37.50%
    Setting it as best candidate
HBLP: BB=%bb.160 ('') L1555 f=808759886 Succ=%bb.162 ('') L1556 f=505474929 unsched=0
    Candidate: %bb.162 (''), probability: 0x50000000 / 0x80000000 = 62.50%
    Setting it as best candidate
    Selected: %bb.162 ('')
Merging from %bb.160 ('') to %bb.162 ('')
Selecting best successor for: %bb.162 ('')
HBLP: BB=%bb.162 ('') L1556 f=505474929 Succ=%bb.163 ('') L0 f=808759886 unsched=1
...
      CONFLICT pred %bb.161 ('') predfreq=303284957 candfreq=505474929
    Not a candidate: %bb.163 ('') -> 0x80000000 / 0x80000000 = 100.00% (prob) (non-cold CFG conflict)
```

`unsched=1` on the `bb.162 -> bb.163` edge is exactly `bb.161` still being unplaced.

**Consequence**: `bb.163`'s own frequency (808 759 886) is *larger* than `bb.417`'s
(463 615 365).  If `bb.161` had been placed before `bb.136`'s chain ended, the MC chain
would have won the frequency scan and been laid out before the 0x400 body.

`bb.161`'s frequency is `f(bb.160) * 0.375 = 808 759 886 * 37.5 % = 303 284 957`.
The 37.5 % is `BranchProbabilityInfo::calcZeroHeuristics` (`ZH_TAKEN_WEIGHT = 20`,
`ZH_NONTAKEN_WEIGHT = 12` in `lib/Analysis/BranchProbabilityInfo.cpp:116-117`): the
ternary is lowered to `br i1 %tobool, label %true_arm, label %false_arm` with
`tobool = icmp ne i8 %hasWon, 0`, so the *true* arm gets 62.5 % and the *false* arm
(`bb.161`) gets 37.5 %.  To flip the decision by branch weights alone `bb.161` would need
`> 463 615 365`, i.e. the false arm would have to be the 62.5 % edge
(`f(bb.160) * 0.625 = 505 474 929 > 463 615 365`).  Every source spelling tried
(`!hasWon() ? false : X`, `hasWon() && !isCampaign`, explicit `if`/`else`,
`bool k = false; if (...) k = ...;`) is canonicalised by clang back to the same
`icmp ne` + 62.5/37.5 shape (verified: identical `-print-after=block-placement` order).

## 4. The reorder that *does* move MC before the 0x400 body

Swapping the two statements

```cpp
Mission *campaign = (Mission *) (intptr_t) Globals::status->getCampaignMission();
bool keepObjectives = campaign->hasWon() ? !Globals::status->getMission()->isCampaignMission() : false;
Mission *mission = Globals::status->getMission()->hasWon() ? Globals::status->getMission() : campaign;
```

to

```cpp
Mission *campaign = (Mission *) (intptr_t) Globals::status->getCampaignMission();
Mission *mission = Globals::status->getMission()->hasWon() ? Globals::status->getMission() : campaign;
bool keepObjectives = campaign->hasWon() ? !Globals::status->getMission()->isCampaignMission() : false;
```

changes the IR block order enough that the MC chain is placed **before** the 0x400 body.
Verified with `-print-after=block-placement` (0-based block index in the final MIR order):

| variant | B200 | starMap | cutscene | MC | B400 | size | official |
|---|---|---|---|---|---|---|---|
| baseline | 204 | 158 | 152 | **231** | **227** | 8080 | **78.4** |
| reorder (mission first) | 204 | 212 | 230 | **210** | **229** | 8088 | 75.2 |
| reorder + `expi` | 204 | 158 | 152 | **208** | **233** | 8084 | 75.0 |

So the wave-94 blocker ("MC is placed after the 0x400 body") is **solved** by the
reorder, but the overall layout gets worse: the reorder also moves the starMap chain and
the cutsceneActive block, and the function grows by 8 bytes (the reordered literal pools
need extra alignment).  The instruction *sequence* is unchanged (a textual diff of the
`.s` files shows only branch-target renumbering), so this is a pure layout/padding loss.

The original's full block order (from the binary) is

```
[cutsceneActive 0x17aaf6][failed 0x17ab3c][starMap 0x17abb6][0x200 body 0x17adc0]
[MS 0x17adc8][keepObjectives 0x17ade0][MC 0x17af5e][0x400 body 0x17afd8][shared blx][test]
```

i.e. the original places the cutsceneActive block **before** the starMap chain and the
0x200 body, while the baseline places it at position 230 (after the 0x400 body).  Matching
the MC position alone therefore does not reproduce the original layout.

## 5. Measured variants (official `tools/verify/verify.py`, autojunk=False)

| variant | source shape | size | official |
|---|---|---|---|
| baseline | flat chain, `count > 0` | 8080 | **78.4** |
| `mission` before `keepObjectives` | see §4 | 8088 | 75.2 |
| `mission` before `keepObjectives` + `expi` | see §4 + wave-92 `expi` | 8084 | 75.0 |
| `Mission::hasFailed` -> `bool` | `.h`/`.cpp` return type | 8088 | 77.2 |
| `hasFailed` -> `bool` + campaign order | + `isInstantAction` before `isCampaign` | 8088 | 77.2 |
| campaign order (Ghidra order) | `isInstantAction` before `isCampaign` | 8080 | 78.1 |
| no `campaign` local (both arms refetch) | wave-94 `gen_camp2` | 8080 | 78.3 |
| split `== 96` / `== 127` cases | two separate `else if`s | 8100 | 74.6 |

The `== 96` / `== 127` split is worth recording: the original really does have two
separate `getCurrentCampaignMission()` tests (`cmp r0,#96` at 0x17b71c and `cmp r0,#127`
at 0x17bc34) that share one body (`0x17b722`).  The **existing combined source**

```cpp
} else if (mission->isCampaignMission() &&
           (Globals::status->getCurrentCampaignMission() == 96 ||
            (mission->isCampaignMission() &&
             Globals::status->getCurrentCampaignMission() == 127))) {
```

already reproduces that: clang emits the `== 96` test in place and sinks the redundant
`isCampaignMission() && == 127` arm to a separate block (`.LBB22_421`/`.LBB22_422`) that
lands at the same relative position as the original's 0x17bc30.  Splitting it into two
`else if`s makes the compiler re-test `isCampaignMission()` and grows the function by 20
bytes, so the combined form is the faithful one.

## 6. What remains

1. The MC placement is no longer a mystery: it is a work-list eligibility problem, not a
   frequency problem.  `bb.163` (freq 808 759 886) beats `bb.417` (463 615 365) whenever
   it is eligible.
2. To get the original layout the *whole* cluster must move, not just MC:
   `[cutsceneActive][failed][starMap][0x200][MS][keepObjectives][MC][0x400]`.
   The cutsceneActive block (`bb.143`, freq 392 126 005) currently lands at position 230;
   in the original it is the very first block of the cluster.  Any source shape that
   raises its work-list priority (or makes it the fall-through of the preceding chain)
   would move the whole cluster.
3. The reorder in §4 is the closest shape found so far but loses 8 bytes to literal-pool
   alignment.  If the alignment loss can be recovered (e.g. by also fixing the starMap /
   cutscene placement so the pools line up), it becomes a candidate improvement.

## 7. Reproduction

```
# gate
cmake --build cmake-build-match --target check -j8        # RATCHET PASS, byte 4500
# per-function row
python3 -c "import json;r=json.load(open('cmake-build-match/verify/report.json'));print([x for x in r['rows'] if x['name']=='_ZN5MGame10OnTouchEndEiiPv'])"
# MBP work-list / frequency trace (instrumented llc)
clang++ <repo flags> -S -emit-llvm -o MGame.ll src/game/menu/MGame.cpp
sed 's/, variables: ![0-9]*//' MGame.ll > MGame2.ll
/home/debian/llvm7build/build/bin/llc -march=thumb -mcpu=cortex-a8 \
  -mtriple=thumbv7-none-linux-androideabi16 -O2 -debug-only=block-placement \
  MGame2.ll -o /dev/null
# block order (final MIR)
/home/debian/llvm7build/build/bin/llc -march=thumb -mcpu=cortex-a8 \
  -mtriple=thumbv7-none-linux-androideabi16 -O2 -print-after=block-placement \
  MGame2.ll -o /dev/null
```

Scratch harness (not tracked): `/dev/shm/w4509/` — `cc_ll.sh` (exact repo flags),
`ordercheck2.sh` (auto-locates MC/0x400/0x200/starMap/cutscene source lines and prints
their final block index), `measure.sh` (link + verify + OnTouchEnd row),
`contentdiff.py` (branch-target-normalised difflib), `mbp_full.txt` (full
`-debug-only=block-placement` dump), `ote_chain0b.txt` (OnTouchEnd chain-forming slice),
`o_full.dis` (original disassembly 0x17a144..0x17c0d4).
