# MGame::OnTouchEnd (8080 B) — w4809 (wave 98): the tail cluster is still the
# layout blocker; the missing content is now identified (unguarded hud0 body +
# a third `getCampaignMission()` call), but every faithful shape scores lower

Target: `_ZN5MGame10OnTouchEndEiiPv` (Ghidra entry 0x18a144, file VMA 0x17a144,
8080 B).  Baseline (unchanged by this pass): **78.3565 %**, 8080 B, 2775 original /
2774 ours branch-target-normalised instructions.  Gate re-verified green at the end
of the pass: byte_exact **4500** / linked_exact **4500** / size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, RATCHET PASS, `.text` 0x1337c8.

This pass lands **no code change**.  It (1) re-derives the exact MBP work-list
decision that puts the tail cluster at the end, (2) records the `d_swap400` data
point that the original's `[0x400 body][shared blx]` tie *is* reachable but costs
7.5 pts, (3) **identifies two genuine content differences that every prior note
missed** — the original's `hudTouchFlags == 0` body is *unguarded* and the mission
ternary re-calls `getCampaignMission()` — and shows that reconstructing them
faithfully (`m_mcrecall + h_both`) gives a byte-faithful 8076 B body that scores
only 76.52, and (4) measures ~90 further source shapes; none beats 78.3565.

Prior notes: `mgame-ontouchend-wave97-w4707.md` (newest),
`mgame-ontouchend-wave96-w4604.md`, `mgame-ontouchend-wave95-w4509.md`,
`mgame-ontouchend-wave94-w4409.md`, `mgame-ontouchend-hblp-1unit-w4107.md`,
`mgame-ontouchend-tailmerge-mbp-reorder_w4007.md`,
`mgame-ontouchend-78-1-8080-b_w3807.md`.

## 0. TU / source-layout witness

`source file list dump/gof2hd_1.1.19_ios.cpp` confirms only that
`Classes/Game/MGame.cpp` owns `_MGame::OnTouchEnd(int, int, void*)`; there are no
TU-local statics for this function.  Everything below is reconstructed from the
Android 2.0.16 binary (Ghidra + objdump) and the exact repo build.

## 1. New content findings (authoritative objdump, file VMA 0x17a144)

### 1.1 The `hudTouchFlags == 0` body is UNGUARDED and calls closeHudMenu first

Original, 0x17b01a (the `hudTouchFlags == 0` body):

```
17b01a: f8db 0074   ldr.w  r0, [fp, #116]      ; this->hud   (no null test!)
17b01e: f6ec ecf2   blx    Hud::closeHudMenu
17b022: 2400        movs   r4, #0
17b024: 4658        mov    r0, fp
17b026: f88b 40d6   strb.w r4, [fp, #214]      ; hudMenuOpen = 0
17b02a: e9cb 4426   strd   r4, r4, [fp, #152]  ; touch0Id = touch1Id = nullptr
17b02e: f88b 405d   strb.w r4, [fp, #93]       ; pauseOpen = 0
17b032: f6ed e92c   blx    MGame::resumeSounds
17b036: f89b 00c8   ldrb.w r0, [fp, #200]      ; _bc8
17b03a: 2800        cmp    r0, #0
17b03c: d0d4        beq.n  0x17afe8
17b03e: f8db 0058   ldr.w  r0, [fp, #88]       ; player
17b042: f88b 40c8   strb.w r4, [fp, #200]      ; _bc8 = 0
17b046: 2800        cmp    r0, #0
17b048: d0ce        beq.n  0x17afe8
17b04a: f6e7 ee2a   blx    PlayerEgo::resetGunDelay
17b04e: e7cb        b.n    0x17afe8
```

The repo baseline instead emits an extra `if (this->hud != nullptr)` guard
(`ldr; movs r4,#0; strb; cbz; blx; ...`, 58 B vs the original's 54 B) and stores
`hudMenuOpen = 0` *before* the call.  The faithful source is:

```cpp
if (this->hudTouchFlags == 0) {
    this->hud->closeHudMenu();
    this->hudMenuOpen = 0;
    this->touch0Id = nullptr;
    this->touch1Id = nullptr;
    this->pauseOpen = 0;
    resumeSounds();
    if (this->_bc8) {
        this->_bc8 = 0;
        if (this->player != nullptr) {
            this->player->resetGunDelay();
        }
    }
}
```

This saves 4 bytes (`h_unguard` → 8076 B).  Note the original *does* keep the
`player != nullptr` check, so only the `hud` check is spurious.

### 1.2 The mission ternary re-calls `getCampaignMission()` (3 calls, not 2)

The original calls `Status::getCampaignMission()` **three** times in this function
(0x17add2, 0x17ade8, 0x17af78); the repo baseline calls it twice.  The third call is
in the `mission` ternary's else arm:

```
17af5e: ldr.w  r0, [r8]
17af62: blx    Status::getMission
17af66: blx    Mission::hasWon
17af6a: mov    r1, r0
17af6c: ldr.w  r0, [r8]
17af70: cbz    r1, 0x17af78
17af72: blx    Status::getMission
17af76: b.n    0x17af7c
17af78: blx    Status::getCampaignMission     ; <-- extra call
17af7c: mov    r5, r0
```

so the faithful source keeps the `campaign` local for `keepObjectives` but re-fetches
in the ternary:

```cpp
Mission *campaign = (Mission *) (intptr_t) Globals::status->getCampaignMission();
bool keepObjectives = campaign->hasWon() ? !Globals::status->getMission()->isCampaignMission() : false;
Mission *mission = Globals::status->getMission()->hasWon() ? Globals::status->getMission()
                                                           : (Mission *) (intptr_t) Globals::status->getCampaignMission();
```

(`m_nocamp` also gets three calls, but it *reorders* the two statements; the original
keeps `keepObjectives` first.  `m_mcrecall` is the order-faithful spelling.)

### 1.3 The faithful pair is size-faithful, but scores lower

`m_mcrecall + h_both` = **76.5236 %**, 8076 B, 2775/2771.  A multiset comparison of
the normalised instruction streams against the original now leaves only
layout-dependent differences:

```
orig-only:  2 b.n, 2 beq.n, 2 nop, cmp r0,#0, beq.w, bne.n, movs r0,#0,
            cmp r1,#0, strb.w r0,[fp,#93], cmp.w r9,#0, strb.w r0,[fp,#214]
ours-only:  2 b.w, movs r1,#0, bne.w, cbz r0, cmp.w r9,#-1, bgt.n,
            cbz r1, strb.w r1,[fp,#214], strb.w r1,[fp,#93]
```

i.e. the remaining 4-byte size delta is exactly the two branches that are `b.n` in
the original but out of range (`b.w`) in ours, plus the `cbz`/`cmp+beq.w` choice and
alignment `nop`s.  There is **no remaining content difference** in this function
apart from the mining guard (`cmp r9,#0 / beq` vs `cmp r9,#-1 / bgt`, both 6 B — see
§6).  The 76.52 score is therefore purely block order.

## 2. The original block layout (authoritative, offsets rel. to 0x17a144)

```
0x0c02 dispatch(hudMenuOpen)+UseKhadorDrive   0x0c64 freeCamTouchEnd
0x0c7c 0x200 body (ldrd; movs r1,#1; b.n 0xe9a)   0x0c84 MS (hasWon|getCampaignMission|hasWon)
0x0c9c keepObjectives TRUE arm   0x0cc4 touchesStream   0x0d48 0x800 body
0x0d68 maneuverTouchEnd   0x0d7c menuWindow cluster   0x0dee setAutoPilotToProgrammedStation
0x0e00 starMap cleanup   0x0e0e starMap=nullptr   0x0e16 keepObjectives FALSE arm (mov.w sl,#0)
0x0e1a MC chain   0x0e94 0x400 body (falls through)   0x0e9a shared blx initHudMenu
0x0e9e test hudTouchFlags==0   0x0ea4 final tail isAutoPilot/epilogue   0x0ed2 __stack_chk_fail
0x0ed6 hud0 body   0x0f3a mission-failed freelance   0x0f72 MC freelance branch   0x0f9e campaign switch chain
```

The original's weaponSlot body tail (`0x17ba24: b.w 0x17afe8`) jumps straight to the
`isAutoPilot` tail, skipping the `hudTouchFlags == 0` test; the wingman body tail
(`0x17bf4a: b.w 0x17afe2`) jumps to the test.  This is the structural fact that makes
the tail cluster early in the original.

## 3. The `[0x400 body][shared blx]` tie is reachable (`d_swap400`)

The baseline's 0x200 body falls through into the shared `blx initHudMenu`; the
original's 0x400 body does.  Swapping the `&0x800` and `&0x400` dispatch arms
(`d_swap400`) flips the tie (verified: the 0x400 arm falls through into the shared
`blx`) but reorders the whole bit-test chain and costs ~7.5 pts:

| variant | size | official | insns | 0x400 tie |
|---|---|---|---|---|
| baseline | 8080 | **78.4** | 2774 | branch-back |
| `d_swap400` | 8068 | 70.9 | — | **fall-through** |
| `d_swap400 + t_nested` | 8084 | 73.2 | — | fall-through |
| `d_swap400 + t_elseif` | 8084 | 73.3 | — | fall-through |
| `b_exp` (`__builtin_expect` on the 0x200 test) | 8076 | 72.2 | — | fall-through |

The MBP arithmetic (re-derived, matches wave-94/95/96/97): with the 0x200 body
`bb.204 f=1854461460` and the shared blx `bb.205 f=2318076825`, the other predecessor
`bb.227` (0x400 body) `f=463615365` gives

```
prededge = 463615365 * 80/100 = 370892291.91 -> 370892291
candedge = 1854461460 * 20/100 = 370892292.34 -> 370892292
370892291 >= 370892292  is false  -> no conflict -> 0x200 chains into the shared tail
```

The boundary is `F200 <= 4*F400 - 2`; with `F200 = E/2`, `F400 = E/8` this is
`E/2 <= E/2 - 2`, impossible.  Making the 0x200 test the 3rd bit test (`d_swap400`)
gives `F200 = E/8` and flips it.

## 4. Why the tail cluster stays at the end (re-verified MBP trace)

`llc -O2 -debug-only=block-placement` on the exact repo IR (`clang++ -S -emit-llvm`
with the CMake flags, `sed 's/, variables: ![0-9]*//'`), second (post-RA) run:

```
HBLP: BB=%bb.349 L2114 f=3708922921 Succ=%bb.350 L2140 f=5934276674 unsched=3
...
Merging from %bb.348 (weaponSlot body) to %bb.349 (test)
Merging from %bb.349 to %bb.350 (isAutoPilot)
```

The test chains to `isAutoPilot` (the baseline's `[test][isAutoPilot]` pair is
correct), but the test itself is placed only when its **last predecessor**, the
weaponSlot body (`bb.348 f=434639404`), is scheduled — and that body is placed after
the whole campaign-switch chain.  Hence the whole `[hudTouchFlags][isAutoPilot]
[epilogue]` cluster lands at the end.

With `t_nested` (the weaponSlot path skips the test, so the weaponSlot body is no
longer a test predecessor) the test *does* move early (position 212, right after the
0x800 body), but `isAutoPilot` is then blocked from chaining to it by a different
predecessor, the camera-switch `maneuverTouchEnd` block (`bb.212 f=741784584`):

```
HBLP: BB=%bb.228 L2115 f=3274283516 Succ=%bb.229 L2144 f=5934276674 unsched=5
HBLP pred %bb.212 f=741784584
HBLP   prededge=741784584*0.8=593427667  candedge=2046427197*0.2=409285439
      CONFLICT pred %bb.212  (non-cold CFG conflict)
```

`t_nested` is 75.0766 / 8084 B.  It also costs 4 B (the weaponSlot body must branch
over the test).  `t_elseif`/`t_wingelse` are byte-identical to the baseline (78.3565).

## 5. Measured variants (official `tools/verify/verify.py`, OnTouchEnd row only)

All variants patch only the mission-success region, the `hudMenuOpen` tail, the
mining guard, or the camera switch.  Exact ratios (`SequenceMatcher.ratio()*100`):

| variant | source shape | size | exact % | insns |
|---|---|---|---|---|
| baseline | campaign local, KO first, CB then IA, guarded hud0 | 8080 | **78.3565** | 2774 |
| `t_elseif` / `t_wingelse` / `m_koif` / `m_cwon` / `m_ne0` / `a_local` / `a_swap` / `e_swap` | canonicalised away | 8080 | 78.3565 | 2774 |
| `m_mcrecall` | re-fetch getCampaignMission in mission ternary | 8080 | 78.2624 | 2773 |
| `m_nocamp` | mission first, no campaign local | 8080 | 78.1042 | 2774 |
| `h_both` | unguarded hud0, closeHudMenu first | 8076 | 76.6180 | 2772 |
| `m_mcrecall + h_both` | both content fixes | 8076 | 76.5236 | 2771 |
| `t_nested` | weaponSlot skips test (nested wingman) | 8084 | 75.0766 | 2774 |
| `hud0_first` | hud0 block before weaponSlot | 8056 | 72.5207 | 2771 |
| `mine_amt` | faithful `if (amount != 0)` | 8076 | 71.4 | 2773 |
| `d_swap400` | swap 0x800/0x400 arms | 8068 | 70.9 | — |

~90 shapes were measured in total (mission region × tail shape × hud0 guard ×
camera-switch spelling × mining guard × `isAutoPilot` spelling).  The baseline cell
is the unique maximum.

## 6. The mining guard is size-neutral and stays

The only genuine content difference in the first half is the mining guard:

```
orig: cmp.w r9, #0 ; beq.n    (6 B)
ours: cmp.w r9, #-1 ; bgt.n   (6 B)   from `int count = -amount; if (count > 0)`
```

The faithful `if (amount != 0)` is also 6 B but changes the block layout enough to
cost ~7 pts (71.4 at 8076 B), so the cheaper `count > 0` spelling stays.

## 7. Conclusion

The baseline 78.3565 % / 8080 B is a strict local optimum.  The two remaining
content differences are now identified and their faithful reconstruction is
size-faithful (8076 B, the 4-byte delta being branch encoding only), but it scores
76.52 because the tail cluster still lands at the end.  The tail cluster is blocked
by two independent MBP decisions:

1. the 0x200-body → shared-blx tie (1-unit near-miss, only `d_swap400` flips it, at
   a 7.5-pt cost), and
2. the `isAutoPilot` chain, which is blocked by the weaponSlot body (baseline) or by
   `maneuverTouchEnd` (`t_nested`).

No source shape found in this pass improves on 78.3565.

## 8. Reproduction

```
# gate
cmake --build cmake-build-match --target check -j8        # RATCHET PASS, byte 4500
# per-function row
python3 -c "import json;r=json.load(open('cmake-build-match/verify/report.json'));print([x for x in r['rows'] if x['name']=='_ZN5MGame10OnTouchEndEiiPv'])"
# exact ratio (not rounded)
python3 /dev/shm/w4809/fastscore2.py
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

Scratch harness (not tracked): `/dev/shm/w4809/` — `v4.py` (named source-shape
edits, combinable with `+`), `fastscore2.py` (exact per-function ratio),
`multiset.py` (normalised instruction multiset diff), `diffhunks.py`,
`genmir.sh` + `order.py` (post-placement MIR block order with source lines),
`mbp_base.txt` / `mbp_nested2.txt` / `mbp_mcrh.txt` (MBP traces), `orig_ote.s`
(original disassembly).
