# MGame::OnTouchEnd — wave 139 (w8413): next-largest hunk classified; no size-8080 source win

Status: analysis-only, 2026-11-07. Worktree `/home/debian/GalaxyOnFire2/wt/w8413`,
base `bf123bd47`. Owned TU `src/game/menu/MGame.cpp` (`OnTouchEnd` only). Scratch
`/dev/shm/w8413`. **No source change landed** (base restored; full gate green:
byte 4507, linked 4507, size 4524/4524, data 843/843, imports 207, lint CLEAN).

Target `_ZN5MGame10OnTouchEndEiiPv` (`MGame::OnTouchEnd`), original 8080 B @ `0x17a144`.
Baseline reproduced this wave: **raw same-address diff 4363 B**, size 8080/8080.

## 1. Next-largest hunk after the OptimizeBlock move

Byte-level same-address diff runs (contiguous differing bytes) in the base, largest first:

| rank | addr | len | what it is |
|---:|---|---:|---|
| — | `0x17adae`/`0x17aeb4` | 100/95 | OptimizeBlock join-test move (pinned, w8314) |
| 1 | `0x17b382` | 185 | escort `new Mission(183,…)` block — pure **address shift** |
| 2 | `0x17b5eb` | 131 | `cinematicSteerActive` switch + `enableParticleEffects` |
| 3 | `0x17bdb0`/`0x17be24` | 109/109 | campaign-chain case bodies (134/144/155/161/162) |
| 4 | `0x17b49e` | 105 | campaign-chain case 65/81 bodies |
| 5 | `0x17ba2f` | 100 | campaign-chain 120/126 bodies |
| 6 | `0x17b078` | 99 | failed-mission `getType()==12` / cargo block |
| 7 | `0x17b229` | 84 | failed-mission `getType()==3/5/11` cargo loop |

Almost all of these are **pure address shifts** caused by block placement, not
instruction-content differences. The instruction *multiset* difference (wave 132 §1,
re-confirmed) is only:

* orig-only: `blx Status::getCampaignMission` (the `campaign` temp), `blx
  Mission::isCampaignMission` (merged `96 || 127`), `mov r1,r0` (mission selection)
* ours-only: two `cmp r0,#1` (`hasWon() == true` spellings)
* `cmp r4,#0` vs `#1` (`hasFailed() == true`)
* `cmp.w r9,#0` vs `#-1` (mining `count > 0` vs `amount != 0`)
* `cmp r0,#0` vs `#1` (`menuWindow->OnTouchEnd(...) == true`)

So the **largest genuine content hunk is the merged `96 || 127` campaign case**
(one 108-instruction insertion block in the base). The second is the
`won`/`keepObjectives`/mission-selection chain (`campaign` temp + `== true`
spellings). Everything else is layout.

## 2. The `96 || 127` split is content-correct but +8 B

The original has **two separate `else if` clauses** (wave 132 §2):

```
17b70a  isCampaignMission(); cmp #0; beq.w 17b93c
17b71c  cmp #96;  bne.w 17b93c
17b722  96 body (getStation(98))
...
17bc26  isCampaignMission(); cbz -> 17bc3a
17bc34  cmp #127; beq.w 17b722        ; shared 96 body
17bc3a  isCampaignMission(); cmp #0; beq -> 17bd00
17bc4c  cmp #134; bne -> 17bd00
```

The base merges them (`cmp 96; beq body; cmp 127; bne next`) and has **19**
`isCampaignMission` calls vs the original's **20**. Splitting restores the call and
the emitted order `96,100,110,120,126,127,134` (verified: the base emits `96,127`
adjacent), but costs **+8 B** (8088) because the 96 test's branch target moves from
the near body to the far next case and the 127 test needs a backward branch to the
shared body. Raw diff rises 4363 -> 4674.

## 3. Exhaustive size-8080 variant sweep (all 64 combinations)

Transformations tried independently and combined on the base:

| flag | change | size | raw diff |
|---|---|---:|---:|
| — | base | **8080** | **4363** |
| `baremenu` | drop `== true` on `menuWindow->OnTouchEnd` | 8080 | 4382 |
| `barefailed`+`baremenu` | + drop `== true` on `hasFailed()` | 8080 | 4728 |
| `camp`+`barefailed`+`baremenu` | + inline `getCampaignMission()` (drop temp) | 8080 | 4734 |
| `split96`+`camp`+`barefailed`+`baremenu` | + split 96/127 | 8080 | 5816 |
| `split96`+`barewon`+`barefailed`+`baremenu`+`mining` | all | 8080 | 6388 |

All other 58 combos change the size (8064..8096). **No size-8080 reshape beats the
base's 4363 raw diff.** The base is a strong local optimum: its content "errors"
are load-bearing for the block layout (removing any `== true` flips a branch
direction and re-rolls the whole `failed`/`won` region).

## 4. Why the switch body order cannot be fixed from source

The `cinematicSteerActive` switch (hunk 2) emits `case 1` before `case 0` in the
original but `case 0` before `case 1` in the base:

```
orig  17b64c cmp r0,#1; beq 17b7f4    ; case-1 body far, case-0 falls through
base  17b5d6 cmp r0,#0; beq 17b6c4    ; case-0 body far, case-1 falls through
```

This is **body layout**, not source order: swapping the source `case 1`/`case 0`
blocks, rewriting the switch as an if/else chain, or nesting the `else` all produce
**byte-identical** output (4363/8080). Clang 7 canonicalises the chain to the same
switch lowering and sorts by case value; which body is placed far is decided by
MachineBlockPlacement.

## 5. Conclusion / next lead

The next-largest residual is the campaign-chain block order (`96 || 127` split).
The split is faithful and content-correct but intrinsically +8 B; it can only land
if a size-8080 companion change is found that removes 8 B without re-rolling the
layout. Candidate: the `won`/`campaign`-temp chain is 4 B short of the original
(the original calls `getCampaignMission()` three times, the base twice) — but
removing the temp (`camp`) is +4 B, the wrong sign. The remaining size slack is the
`+6 B` of wider `b.w` encodings from the OptimizeBlock survivor; that is pinned.

## 6. Reproduction

```
cp /dev/shm/w8413/MGame.cpp.base src/game/menu/MGame.cpp
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w8413/fdiff.py _ZN5MGame10OnTouchEndEiiPv   # 4363 / 8080
python3 /dev/shm/w8413/variants.py '[["base",[]]]'          # variant sweep
```

Scratch: `/dev/shm/w8413/{fdiff.py,variants.py,align2.py,blocks.py,orig.dis,ours.dis}`.
The authoritative binary is
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
