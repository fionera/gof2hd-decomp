# MGame::OnTouchEnd (8080 B) — w4707 (wave 97): the mission-continuation block is
# still work-list-eligibility-blocked; `reorder_nocamp` buys the MC placement at
# 8080 B but costs 0.3 pts, and the real layout blocker is the tail cluster

Target: `_ZN5MGame10OnTouchEndEiiPv` (Ghidra entry 0x18a144, file VMA 0x17a144, 8080 B).
Baseline (unchanged by this pass): **78.4 %**, 8080 B, 2775 original / 2774 ours
branch-target-normalised instructions.  Gate re-verified green at the end of the pass:
byte_exact **4500** / linked_exact **4500** / size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, RATCHET PASS, `.text` 0x1337c8.

This pass lands **no code change**.  It (1) re-derives the exact MBP work-list decision
from the instrumented llc, (2) records a new data point — the `reorder_nocamp` shape puts
the mission-continuation (MC) ternary *before* the 0x400 body at **8080 B** (no 8-byte
loss) but scores 78.1, (3) measures a 48-shape grid over the mission-success region plus
~25 tail/mining shapes, and (4) localises the dominant remaining layout difference to the
`[hudTouchFlags test][isAutoPilot][epilogue]` tail cluster, which the original places at
0x0e9e-0x0ec0 (in the middle of the function) and we place at 0x17b930-0x17b952 (at the
end).

Prior notes: `mgame-ontouchend-wave96-w4604.md` (newest),
`mgame-ontouchend-wave95-w4509.md`, `mgame-ontouchend-wave94-w4409.md`,
`mgame-ontouchend-hblp-1unit-w4107.md`, `mgame-ontouchend-tailmerge-mbp-reorder_w4007.md`,
`mgame-ontouchend-78-1-8080-b_w3807.md`.

## 0. TU / source-layout witness

`source file list dump/gof2hd_1.1.19_ios.cpp` confirms only that `Classes/Game/MGame.cpp`
owns `_MGame::OnTouchEnd(int, int, void*)`; there are no TU-local statics for this
function.  Everything below is reconstructed from the Android 2.0.16 binary
(Ghidra + objdump) and the exact repo build.

## 1. Re-verified MBP mechanism (instrumented llc 7.0.0)

`/home/debian/llvm7build/build/bin/llc -O2 -debug-only=block-placement` on the exact repo
IR (`clang++ -S -emit-llvm` with the CMake flags, `sed 's/, variables: ![0-9]*//'`).
The second (post-RA) run is the one that decides the final layout.

`selectBestCandidateBlock` (`MachineBlockPlacement.cpp:1582`) scans `BlockWorkList` and
picks the **highest block frequency**.  After the starMap-cleanup chain (`bb.136`,
`this->starMap = nullptr; return;`) is placed, the decisive call is

```
    %bb.143 ('') -> 0.0006034970275 (freq)     cutsceneActive test
    %bb.362 ('') -> 0.0001650019406 (freq)
    %bb.417 ('') -> 0.0007135219065 (freq)     <-- 0x400 body, WINS
    %bb.419 ('') -> 0.0003567609525 (freq)
    ...
    %bb.161 ('') -> 0.0004667672322 (freq)     keepObjectives=false arm
    %bb.303 ('') -> 0.0005028356096 (freq)
    %bb.278 ('') -> 0.0004190296739 (freq)
Merging from %bb.136 ('') to %bb.417 ('')
```

Raw `MBFI` frequencies (from the `HBLP:` lines):

| block | source | raw freq |
|---|---|---|
| 0x200 body | 2058 | 1 854 461 460 |
| starMap chain | 1496 | 941 102 413 |
| 0x800 body | 2059 | 927 230 730 |
| keepObjectives test | 1555 | 808 759 886 |
| MC ternary | 1557 | 808 759 886 |
| keepObjectives true arm | 1556 | 505 474 929 |
| **0x400 body** | 2064 | **463 615 365** |
| cutsceneActive test | 1509 | 392 126 005 |
| **keepObjectives false arm** | L0 | **303 284 957** |

The MC chain head (`bb.163`, freq 808 759 886) is **not in the work list**: its second
predecessor, the `keepObjectives = false` arm (`bb.161`, 303 284 957), is still
unscheduled (`unsched=1` on the `bb.162 -> bb.163` edge).  So the MC block cannot compete
with the 0x400 body even though its frequency is 1.75x higher.

The false arm's frequency is `0.375 * 808 759 886` because clang lowers
`campaign->hasWon() ? ... : false` to `icmp ne i8 %hasWon, 0` and
`BranchProbabilityInfo::calcZeroHeuristics` gives the *taken* (true) edge 20/32 = 62.5 %
and the false edge 12/32 = 37.5 %.  To flip the comparison by branch weights alone the
false arm would need `> 463 615 365`, i.e. `> 0.573 * F_ko`; the only way is a 62.5 %
false edge, which changes the emitted `cmp/beq` (see wave96 §2).

## 2. New data point: `reorder_nocamp` puts the MC ternary before the 0x400 body at 8080 B

Swapping the two statements and dropping the `campaign` local (both arms refetch
`getCampaignMission()`):

```cpp
Mission *mission = Globals::status->getMission()->hasWon()
                       ? Globals::status->getMission()
                       : (Mission *) (intptr_t) Globals::status->getCampaignMission();
bool keepObjectives = ((Mission *) (intptr_t) Globals::status->getCampaignMission())->hasWon()
                          ? !Globals::status->getMission()->isCampaignMission()
                          : false;
```

This is the *only* measured shape that gets the MC ternary test in front of the 0x400
body while keeping the original 8080 B size (no literal-pool alignment loss):

| shape | size | official | insns |
|---|---|---|---|
| baseline | 8080 | **78.4** | 2774 |
| `reorder_nocamp` | 8080 | 78.1 | 2774 |
| `reorder` (campaign local, mission first) | 8088 | 75.2 | 2778 |
| `faithful` (isInstantAction before isCampaign) | 8080 | 78.1 | 2775 |
| `m6v` (no campaign local, keepObjectives first) | 8080 | 78.3 | 2773 |

Post-placement MIR order for `reorder_nocamp` (positions, 0-based):

```
0x200 body = 204, MC ternary test = 208-209, starMap test = 210,
cutscene = 228, 0x400 body = 227, MC false arm = 231, keepObjectives = 232-234
```

So the MC *test* moves in front of the 0x400 body, but its false arm and the whole
`keepObjectives` computation move behind it — the original has the entire group
`[keepObjectives false][MC][0x400 body]` contiguous.  Net effect: 78.1 (-0.3).

## 3. The 48-shape mission-success grid (official score)

All combinations of `{campaign local, no campaign local} x {keepObjectives first, mission
first} x {isCampaign then isInstantAction, isInstantAction then isCampaign} x
{ternary, if/else, &&} x {ternary, if/else}`.  The keepObjectives and mission *spellings*
are all canonicalised by clang — every spelling within a cell is byte-identical.

| camp | order | CB/IA order | pct | size | insns |
|---|---|---|---|---|---|
| yes | KO first | campaign then instant | **78.4** | 8080 | 2774 |
| no  | KO first | campaign then instant | 78.3 | 8080 | 2773 |
| no  | KO first | instant then campaign | 78.1 | 8080 | 2775 |
| no  | mission first | campaign then instant | 78.1 | 8080 | 2774 |
| no  | mission first | instant then campaign | 78.0 | 8080 | 2775 |
| yes | KO first | instant then campaign | 76.4 | 8076 | 2774 |
| yes | mission first | campaign then instant | 75.2 | 8088 | 2778 |
| yes | mission first | instant then campaign | 75.1 | 8084 | 2778 |

The baseline cell is the unique maximum.

## 4. The dominant remaining layout difference: the tail cluster

The first sequence-level difference is at index 134 (a branch to the common epilogue:
`b.w loc_ec0` vs `b.w loc_180e`); everything before it matches exactly.  The single
biggest structural difference is where that common epilogue lands:

```
original: ... [0x400 body 0x0e94][shared blx 0x0e9a][hudTouchFlags test 0x0e9e]
              [isAutoPilot 0x0ea4][epilogue 0x0ec0][stack_chk_fail 0x0ed2]
              [hud0 body 0x0ed6][mission-failed 0x0f3a] ...
ours:     ... [0x400 body 0x0e1a] ... [hudTouchFlags test 0x17b930]
              [isAutoPilot 0x17b936][epilogue 0x17b952] ...
```

The epilogue block (`bb.351`, freq 572 248 573 261, 20 predecessors) is placed only when
its last predecessor is scheduled.  In our build that last predecessor is the
`isAutoPilot` block (`bb.350`), which follows the `hudTouchFlags == 0` test (`bb.349`),
which follows the weaponSlot block tail (`bb.348`).  Hence the whole
`[hudTouchFlags][isAutoPilot][epilogue]` chain is emitted at the very end.

### New evidence: the original's weaponSlot path skips the `hudTouchFlags == 0` test

```
original 0x17ba24:  b.w 0x17afe8   ; weaponSlot block tail -> isAutoPilot (0xea4)
original 0x17bf4a:  b.w 0x17afe2   ; wingman block tail   -> hudTouchFlags test (0xe9e)
ours     0x17b92c:  blx closeHudMenu
ours     0x17b930:  ldr r0,[fp,#248]   ; weaponSlot path *does* test hudTouchFlags
```

So in the original the `hudTouchFlags == 0` block is **not a successor of the weaponSlot
block**: the compiler proved `hudTouchFlags != 0` on that path (weaponSlot >= 0 implies
one of the `& 0x2000/0x4000/0x8000/0x10000` bits is set).  Our baseline does not perform
that proof, so `bb.348` stays a predecessor of `bb.349` and drags the whole tail cluster
to the end.

The natural source shape that reproduces the skip is an `else if` chain:

```cpp
if (weaponSlot >= 0) { ... }
else if (wingmanCommand >= 0) { ... }
else if (this->hudTouchFlags == 0) { ... }
```

It is semantically equivalent (hudTouchFlags == 0 implies weaponSlot < 0 and
wingmanCommand < 0), and it *does* reproduce the skip (verified in the emitted code:
the weaponSlot tail jumps straight to `isAutoPilot`), but it costs 4 bytes and the score
drops to 75.1 (8084 B, 2775 insns) because the `else if` changes the CFG of the
`hudTouchFlags` test's other predecessors.  Combining it with the no-campaign-local
mission shape gives 75.2 (8088 B).

## 5. Measured variants (official `tools/verify/verify.py`, OnTouchEnd row only)

All variants patch only the mission-success region (lines 1553-1569), the
`hudMenuOpen` tail (lines 2054-2148), or the mining guard (lines 1393-1397).

| variant | source shape | size | official | insns |
|---|---|---|---|---|
| baseline | campaign local, KO first, CB then IA | 8080 | **78.4** | 2774 |
| `reorder_nocamp` | mission first, no campaign local | 8080 | 78.1 | 2774 |
| `m6v` | no campaign local, KO first | 8080 | 78.3 | 2773 |
| `elseif_hud0` | `else if (hudTouchFlags == 0)` | 8084 | 75.1 | 2775 |
| `elseif+nocam` | both | 8088 | 75.2 | 2776 |
| `elseif+mine` | both + `if (amount != 0)` | 8080 | 70.0 | 2774 |
| `mine` | `if (amount != 0)` (faithful mining loop) | 8076 | 71.4 | 2773 |
| `amount_lt0` | `if (amount < 0)` | 8084 | 71.5 | 2775 |
| `hud0_before_weapon` | hudTouchFlags block before weaponSlot | 8160 | 73.3 | 2799 |
| `hud0_first` | hudTouchFlags block first in the tail | 8084 | 75.1 | 2776 |
| `cache_flags` | cache `hudTouchFlags` in a local in the tail | 8104 | 75.6 | 2778 |
| `cache_flags_local` | cache only for the final test | 8080 | 78.4 | 2774 |
| `slot_gt`/`slot_0le`/`slot_ne` | `weaponSlot > -1` / `0 <= weaponSlot` / `!= -1` | 8080 | 78.4 | 2774 |
| `wingman_ne` | `wingmanCommand != -1` | 8080 | 78.4 | 2774 |
| `hud0_not`/`hud0_neg` | `!hudTouchFlags` / `(hudTouchFlags) == 0` | 8080 | 78.4 | 2774 |
| `hud_null` | `if (this->hud)` | 8080 | 78.4 | 2774 |
| `autopilot_bool` | `!wasAutoPilot && isAutoPilot()` | 8080 | 78.4 | 2774 |
| `no_failed_local` | `if (getMission()->hasFailed())` | 8080 | 78.4 | 2774 |
| `haswon_local`/`haswon_local2` | cast-free `getCampaignMission()->hasWon()` | 8080 | 78.4 | 2774 |

Every "spelling" variant in the last eight rows is byte-identical to the baseline.

## 6. Diff statistics (baseline vs original)

`difflib` over the branch-target-normalised instruction stream: 190 hunks, 1201 differing
instructions (sum of both sides), 78.4 %.  The largest hunks are all block-order, not
content:

| hunk | orig | ours | size |
|---|---|---|---|
| mission-success / 0x400 / freelance reorder | [1230:1307] | [1234:1242] | 85 |
| campaign-switch block reorder | [1797:1858] | [1900:1915] | 76 |
| campaign-switch block reorder | [1629:1632] | [1623:1686] | 66 |
| campaign-switch block reorder | [1727:1731] | [1781:1826] | 49 |

The only genuine *content* difference in the first half is the mining guard
(`cmp r9,#0; beq` in the original vs `cmp r9,#-1; bgt` in ours, because our source uses
`int count = -amount; if (count > 0)`), and that shape is worth +7 pts over the faithful
`if (amount != 0)` (78.4 vs 71.4), so it stays.

## 7. Conclusion

The baseline 78.4 % / 8080 B is a strict local optimum for the mission-success region:
the 48-shape grid has no cell above it, and the only shape that achieves the MC-before-
0x400 placement at 8080 B (`reorder_nocamp`) is 0.3 pts worse.  The remaining gap is
dominated by the placement of the `[hudTouchFlags][isAutoPilot][epilogue]` tail cluster,
which the original puts at 0x0e9e-0x0ec0 and we put at 0x17b930-0x17b952.  The original's
weaponSlot path provably skips the `hudTouchFlags == 0` test (it branches straight to
`isAutoPilot`), but the `else if` source shape that reproduces that skip costs 4 bytes and
2.3 pts elsewhere.  No strict improvement was found in this pass.

## 8. Reproduction

```
# gate
cmake --build cmake-build-match --target check -j8        # RATCHET PASS, byte 4500
# per-function row
python3 -c "import json;r=json.load(open('cmake-build-match/verify/report.json'));print([x for x in r['rows'] if x['name']=='_ZN5MGame10OnTouchEndEiiPv'])"
# MBP work-list / frequency trace (instrumented llc 7.0.0)
clang++ <repo flags> -S -emit-llvm -o MGame.ll src/game/menu/MGame.cpp
sed 's/, variables: ![0-9]*//' MGame.ll > MGame2.ll
/home/debian/llvm7build/build/bin/llc -march=thumb -mcpu=cortex-a8 \
  -mtriple=thumbv7-none-androideabi16 -O2 -debug-only=block-placement \
  MGame2.ll -o /dev/null
# final block order with source-line tags
/home/debian/llvm7build/build/bin/llc -march=thumb -mcpu=cortex-a8 \
  -mtriple=thumbv7-none-androideabi16 -O2 -print-after=block-placement \
  MGame2.ll -o /dev/null 2> mir.txt
```

Scratch harness (not tracked): `/dev/shm/w4707/` — `build.sh` (link_so + score),
`score.py` (official per-function row), `variant.py` / `gen.py` / `try*.py`
(source-shape generators), `contentdiff.py` (branch-target-normalised difflib),
`dumpseq.py` (side-by-side normalised instruction stream), `genmir.sh` +
`order.py` (post-placement MIR block order with source lines), and the candidate
blocks in `cand/`.
