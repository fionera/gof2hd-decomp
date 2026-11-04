# MGame::OnTouchEnd (8080 B) — w4907 (wave 99): the faithful *nested* shape
# (`t_nested + h_both`, size-exact 8080 B) really does move the tail cluster
# from 0x17ec to 0xd6e — but it still scores 74.9, not 78.4; the 0x200/0x400
# shared-blx tie is an exact 0.1F tie broken by 1 unit of integer truncation

Target: `_ZN5MGame10OnTouchEndEiiPv` (Ghidra entry 0x18a144, file VMA 0x17a144,
8080 B).  Baseline (unchanged by this pass): **78.3565 %**, 8080 B, 2775 original /
2774 ours branch-target-normalised instructions.  Gate re-verified green at the end
of the pass: byte_exact **4500** / linked_exact **4500** / size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, RATCHET PASS, `.text` 0x1337c8.

This pass lands **no code change**.  It (1) corrects the wave-94..98 HBLP
arithmetic (the conflict predicate is `PredEdgeFreq*HotProb >=
CandidateEdgeFreq*Compl`, and the trace values are 370892291 vs 370892292, so
there is **no** conflict — the 0x200 body *does* own the shared `blx`), (2) shows
the tie is an exact `0.1F` tie in real arithmetic (`0.8*f400 == 0.2*f200`) that
integer truncation resolves by one unit, (3) identifies a **new content
difference** — the original calls `Mission::isInstantActionMission()` *before*
`Mission::isCampaignMission()`/`nextCampaignMission()`, the repo source has them
reversed — and (4) measures the fully faithful `t_nested + h_both` shape, which
is size-exact at 8080 B and moves the tail cluster to 0xd6e but scores 74.9.

Prior notes: `mgame-ontouchend-wave98-w4809.md` (newest),
`mgame-ontouchend-wave97-w4707.md`, `mgame-ontouchend-wave96-w4604.md`,
`mgame-ontouchend-wave95-w4509.md`, `mgame-ontouchend-wave94-w4409.md`,
`mgame-ontouchend-hblp-1unit-w4107.md`.

## 0. TU / source-layout witness

`source file list dump/gof2hd_1.1.19_ios.cpp` confirms only that
`Classes/Game/MGame.cpp` owns `_MGame::OnTouchEnd(int, int, void*)`; there are no
TU-local statics for this function.  Everything below is reconstructed from the
Android 2.0.16 binary (Ghidra + objdump) and the exact repo build.

## 1. Corrected HBLP arithmetic (this supersedes the wave-94..98 wording)

`MachineBlockPlacement::hasBetterLayoutPredecessor` (LLVM 7.0.0,
`lib/CodeGen/MachineBlockPlacement.cpp:1437`) tests

```cpp
if (PredEdgeFreq * HotProb >= CandidateEdgeFreq * HotProb.getCompl())
  BadCFGConflict = true;
```

i.e. **`pred*hp >= cand*cp` is the conflict**, not the reverse.  The live trace
(`llc -O2 -debug-only=block-placement`, second/post-RA run) for the baseline is

```
HBLP: BB=%bb.204 L2056 f=1854461460 Succ=%bb.205 L0 f=2318076825 unsched=1
HBLP pred %bb.204 f=1854461460 succchain=0 chain=1 eqBB=1
HBLP pred %bb.227 f=463615365 succchain=0 chain=0 eqBB=0
HBLP   prededge=463615365 *hp=370892291 candedge=1854461460 *cp=370892292
    Candidate: %bb.205, probability: 0x80000000 / 0x80000000 = 100.00% (CFG break)
    Selected: %bb.205
Merging from %bb.204 to %bb.205
```

so `370892291 >= 370892292` is **false** → no conflict → `bb.204` (the `0x200`
body) chains into `bb.205` (the shared `blx initHudMenu`).  That is exactly what
the repo binary does (`17adc6: ldrd; movs r1,#1; blx 17adcc`), and it is the
mirror of the original, where the `0x400` body owns the `blx`
(`17afd8: ldrd; movs r1,#2; blx 17afde; <falls through to the test>`).

The tie is **exact**, not "1 unit of slack":

| block | IR line | freq |
|---|---|---|
| `bb.188` `0x200` test | 2053 | `F = 3708922921` |
| `bb.204` `0x200` body | 2056 | `F/2   = 1854461460` |
| `bb.227` `0x400` body | 2064 | `F/8   =  463615365` |

`0.8 * (F/8) = 0.1F` and `0.2 * (F/2) = 0.1F`.  `BranchProbability::scale`
truncates (`N = (80<<32)/100 = 3435973836`, `Compl = 858993460`), which turns the
tie into `370892291` vs `370892292` — a one-unit miss.  Any source shape that
keeps `f200 = 4*f400` keeps the `0x200` body as the owner.

The second blocker is independent and much larger.  When `bb.205` (shared `blx`,
`f = 2318076825`) is placed, its successor `bb.349` (the `hudTouchFlags == 0`
test, `f = 3708922921`) is rejected because `bb.212` (the `0x800` body,
`f = 927230730`) is also a predecessor:

```
HBLP: BB=%bb.205 L0 f=2318076825 Succ=%bb.349 L2114 f=3708922921 unsched=3
HBLP pred %bb.212 f=927230730 succchain=0 chain=0 eqBB=0
HBLP   prededge=927230730 *hp=741784583 candedge=2318076825 *cp=463615365
      CONFLICT pred %bb.212 predfreq=927230730 candfreq=2318076825
```

`0.2F >= 0.125F` — no small perturbation flips this one.  The test is therefore
placed only when its last predecessor, the weaponSlot body (`bb.348`), is
scheduled, which is why the whole `[test][isAutoPilot][epilogue][stack_chk]
[hud0 body]` cluster lands at the end of the function in the baseline.

## 2. New content finding: `isInstantActionMission()` comes FIRST

The original's mission-success block (file VMA 0x17af5e):

```
17af5e: ldr.w  r0, [r8]
17af62: blx    Status::getMission
17af66: blx    Mission::hasWon
17af6a: mov    r1, r0
17af6c: ldr.w  r0, [r8]
17af70: cbz    r1, 17af78
17af72: blx    Status::getMission
17af76: b.n    17af7c
17af78: blx    Status::getCampaignMission      ; <-- 3rd call
17af7c: mov    r5, r0
17af7e: blx    Mission::isInstantActionMission ; <-- FIRST
17af82: cbz    r0, 17afa6
   ... instant-action body (switch_to_target_setting = 2; active = 0;
       SetCurrentApplicationModule(1); return) ...
17afa6: mov    r0, r5
17afa8: blx    Mission::isCampaignMission      ; <-- SECOND
17afac: mov    r1, r0
17afae: ldr.w  r0, [r8]
17afb2: cmp    r1, #0
17afb4: beq    17b0b6                          ; else: setFreelanceMission + reward msg
17afb8: movs   r1, #1
17afba: blx    Status::nextCampaignMission
17afbe: b.n    17b0e2
...
17b0e2: blx    Mission::getReward             ; changeCredits(...) AFTER the if/else
17b0e8: blx    Mission::getBonus
17b0f8: blx    Status::changeCredits
```

The repo source has the two checks in the opposite order:

```cpp
if (mission->isCampaignMission()) { Globals::status->nextCampaignMission(true); }
else { Globals::status->setFreelanceMission(Mission::empty);
       Globals::layout->showMissionRewardMessage(...); }
if (mission->isInstantActionMission()) { ...; return; }
Globals::status->changeCredits(...);
```

The faithful order is

```cpp
if (mission->isInstantActionMission()) { ...; return; }
if (mission->isCampaignMission()) { Globals::status->nextCampaignMission(true); }
else { Globals::status->setFreelanceMission(Mission::empty);
       Globals::layout->showMissionRewardMessage(...); }
Globals::status->changeCredits(...);
```

This is a genuine source-order difference (not a block-placement artefact: the
`cbz`/`beq` targets at 0x17af82 and 0x17afb4 pin the order), but reconstructing it
faithfully costs score: `ia_first` = 76.4 / 8076 B, `ia_first + m_mcrecall` =
78.1 / 8080 B.

## 3. The fully faithful tail shape: `t_nested + h_both` (size-exact 8080 B)

Wave 98 measured `t_nested` (75.08 / 8084 B) and `h_both` (76.62 / 8076 B)
separately, but **not their sum**.  The original's tail proves the two belong
together:

* the weaponSlot body tail is `17ba24: b.w 17afe8` — it jumps **past** the
  `hudTouchFlags == 0` test to the `isAutoPilot` tail, i.e. the weaponSlot body is
  *not* a predecessor of the test;
* the wingman body tail is `17bf4a: b.w 17afe2` — it jumps **to** the test;
* the `hudTouchFlags == 0` body is unguarded and calls `closeHudMenu()` first
  (wave-98 §1.1).

So the faithful source nests the test under the `else` of the weaponSlot check:

```cpp
if (weaponSlot >= 0) { ... }
else {
    if (wingmanCommand >= 0) { ... }
    if (this->hudTouchFlags == 0) { ... }   // unguarded, closeHudMenu first
}
```

Size accounting: the nested form adds the 4-byte `b.w` over the test; the
unguarded hud0 body removes 4 bytes (`58 -> 54`).  Net **8080 B, exactly the
original size** (wave-98's `t_nested`+`h_both` were never combined, so this was
not visible before).

The payoff is real but partial: the tail cluster moves from `0x17ec` (baseline)
to **`0xd6e`** (the last bit-test `bpl.w` at `17ad9c` now targets `17aeb2`, and
`17aeb2` is the `ldr [fp,#248]` test).  The original's test is at `0xe9e`, so
0x130 B of blocks still sit between it and the dispatch.  Score: **74.9**
(8080 B).  Adding `m_mcrecall` gives 73.8 / 8084 B.

Combining the nested shape with `d_swap400` (which makes the `0x400` body own the
shared `blx`, exactly as the original does) still loses: 74.6 / 8080 B
(`d_swap400 + t_nested + h_both`), 73.5 / 8084 B (+ `m_mcrecall`).

## 4. Measured variants (official `tools/verify/verify.py`, OnTouchEnd row only)

All variants patch only the mission-success region, the `hudMenuOpen` tail, the
mining guard, or the bit-test order.  `pct` is the report's rounded value
(baseline exact = 78.3565).

| variant | source shape | size | pct |
|---|---|---|---|
| baseline | campaign local, KO first, CB then IA, guarded hud0, flat wingman | 8080 | **78.4** |
| `m_mcrecall` | + 3rd `getCampaignMission()` in the mission ternary | 8080 | 78.3 |
| `ia_first` | `isInstantActionMission` block before the campaign if/else | 8076 | 76.4 |
| `ia_first + m_mcrecall` | both content fixes | 8080 | 78.1 |
| `h_both` | unguarded hud0, closeHudMenu first | 8076 | 76.6 |
| `m_mcrecall + h_both` | both | 8076 | 76.5 |
| `0xA00` | `(flags & 0xA00) != 0` on the `0x800` arm | 8080 | 73.2 |
| `d_swap400 + h_both + m_mcrecall` | swap `0x800`/`0x400` arms + both fixes | 8068 | 70.9 |
| `(flags & 0xC00) != 0 + h_both` | widen the `0x400` arm test | 8088 | 75.9 |
| `hud0_first` | move the `flags == 0` block to the top of `hudMenuOpen` | 8084 | 75.1 |
| `count != 0` | faithful mining guard (`cmp r9,#0; beq`) | 8076 | 71.4 |
| `m_mcrecall + count != 0` | both | 8076 | 71.4 |
| `0x800` arm last | move the `0x800` arm to the end of the bit-test chain | 8068 | 73.5 |
| `test_first` | `flags == 0` block before the weaponSlot check | 8056 | 72.5 |
| **`t_nested + h_both`** | **faithful nested wingman + unguarded hud0** | **8080** | **74.9** |
| `t_nested + h_both + m_mcrecall` | + 3rd call | 8084 | 73.8 |
| `d_swap400 + t_nested + h_both` | original's `0x400`-owns-`blx` shape | 8080 | 74.6 |
| `d_swap400 + t_nested + h_both + mcr` | + 3rd call | 8084 | 73.5 |

The baseline cell remains the unique maximum.

## 5. Why the baseline still wins

`SequenceMatcher` rewards the baseline's *linear* layout: its first ~1230
instructions match the original almost instruction-for-instruction (only branch
targets to the epilogue differ).  Every faithful content fix moves a block and
breaks a longer common run somewhere else.  The remaining 190 hunks are
dominated by 128 single-instruction `loc_` replacements (843 instructions in
`replace` opcodes, 40 `insert`, 20 `delete`) — i.e. the same instructions at
different function-relative offsets, not different code.

The call-symbol multiset is already **identical** to the original except for the
single missing 3rd `getCampaignMission()` (464 vs 463 calls; every other callee
count matches).  The call-sequence `SequenceMatcher` ratio is 85.2 % — i.e. the
remaining gap is block order, not content.

## 6. Reproduction

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
# block order with source lines
/home/debian/llvm7build/build/bin/llc -march=thumb -mcpu=cortex-a8 \
  -mtriple=thumbv7-none-androideabi16 -O2 -print-after=block-placement \
  MGame2.ll -o /dev/null 2> mir.txt
```

Scratch harness (not tracked): `/dev/shm/w4907/` — `score.sh` (link_so + verify
row), `asm.sh` (repo-flag compile), `ours_dis.sh` (objdump the linked `.so`),
`batch1.py`..`batch6.py` (named source-shape edits), `callseq.py` (call-symbol
sequence diff), `blocks.py` / `blockcalls2.py` (basic-block decomposition),
`mbp.txt` (full `-debug-only=block-placement` dump), `orig_ote.s` (original
disassembly).
