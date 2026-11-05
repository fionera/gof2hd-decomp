# MGame::OnTouchEnd (8080 B) — w6308 (wave 115): the 79.9 % variant is
# real but rotates `.rel.plt`; reset-block placement is the blocker

Target: `_ZN5MGame10OnTouchEndEiiPv` (file VMA 0x17a144, 8080 B,
`src/game/menu/MGame.cpp`).  Base: master `b847d5d46`, branch
`wave115-w6308`.

## Result

**No gate-safe improvement found.**  HEAD stays at **8080 B /
78.9189 % / `.rel.plt` diff 0 / RATCHET PASS** (byte 4500, linked 4500,
size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0,
27 sections, file 2,187,328 B, `.text` 0x1337c8, `.ARM.extab` 0x26298).

The wave-114 handoff's `split127 + disp` variant was reproduced exactly:
**8080 B / 79.9063 %** (a real +0.99 pp improvement) — but it rotates
four `.rel.plt` entries and drops `byte_exact` to **4499**, so the
ratchet fails.  This note records the exact reproduction, the PLT
mechanism, and the block-placement analysis that explains why the reset
block cannot be kept early.

## 1. The 79.9063 % variant, reproduced

```bash
cd /home/debian/GalaxyOnFire2/wt/w6308
git checkout -- src/game/menu/MGame.cpp
python3 /dev/shm/w6308/trans.py split127 disp   # see below for the two edits
cmake --build cmake-build-match --target check -j6
#   verify: byte 4499  linked 4500   ->  FAIL  byte_exact 4500 -> 4499
```

The two source edits (both semantic no-ops relative to HEAD):

1. `split127` — replace the combined
   `getCurrentCampaignMission() == 96 || (isCampaignMission() && ... == 127)`
   test with a plain `== 96` test, and insert a separate `== 127` case
   between the `126` and `134` cases (duplicating the case-96 body).
2. `disp` — swap the mission-completion dispatch from
   `if (isInstantActionMission()) ... else if (isCampaignMission()) ...`
   to `if (isCampaignMission()) ... else if (isInstantActionMission()) ...`.

Neither edit changes behaviour.  The `.rel.plt` diff is exactly the
4-cycle:

```
idx 3324  resetStartSequenceOver | removeObjectives
idx 3325  removeObjectives       | goingToWaypoint
idx 3326  goingToWaypoint        | removeRoute
idx 3327  removeRoute            | resetStartSequenceOver
```

## 2. Why the PLT rotates: the reset block moves past `removeObjectives`

`.rel.plt` entries are created in relocation order, and gold processes
each input section's relocations in **offset order** (verified against
`MGame.cpp.obj`: HEAD's OnTouchEnd section relocations are
`reset 0x10e6`, `removeObjectives 0x1142`, `goingToWaypoint 0x1164`,
`removeRoute 0x1178`).  So the PLT order is decided by the *first*
reference to each symbol inside OnTouchEnd, i.e. by the emitted block
order.

| | reset call | first `removeObjectives` | order |
|---|---:|---:|---|
| original | 0x17b0ae | 0x17b270 | reset first |
| HEAD | 0x17b22a | 0x17b286 | reset first |
| `split127+disp` | 0x17b49a | 0x17b2dc | **reset last** |

Only `MGame::successCheck` (which calls `removeObjectives`,
`goingToWaypoint`, `removeRoute`) was byte-exact among the affected
callers, hence exactly one lost function (4500 -> 4499).  `OnUpdate`
also calls `removeObjectives` but is not byte-exact anyway.

## 3. New finding: the reset block is placed right after the case-65 body

In `split127+disp` the cold `else { resetStartSequenceOver(); }` block
lands at `0x17b496`, immediately after the case-65 body
(`0x17b41e-0x17b494`, `switch_to_target_setting = 1; ... departStation
(loadStation(100)); ... SetCurrentApplicationModule(2); return;`) and
just before the case-81 test.  In the original it lands at `0x17b0aa`,
immediately after the failed-mission `changeCredits` block and just
before the freelance path — i.e. *before* the whole campaign switch.

The reset block is a cold block placed by
`MachineBlockPlacement::selectBestCandidateBlock`, which picks the
lowest-frequency unplaced chain.  Its frequency is
`freq(won-test) * P(!campaign->hasWon())`, and the branch probability
comes from `calcZeroHeuristics` (`x == 0` -> 37.5 % / 62.5 %), so the
reset chain is ~17x more frequent than the failed path and is placed
after the case bodies, not before them.

## 4. Why the source cannot move it (measurements)

Every variant below was rebuilt and scored with the same
`tools/verify` machinery (`/dev/shm/w6308/score.py`) and
`readelf -rW` PLT comparison (`/dev/shm/w6308/plt.py`).

| variant | size | fuzzy (4 dp) | plt diff |
|---|---:|---:|---:|
| **HEAD** | **8080** | **78.9189** | **0** |
| `split127 + disp` | 8080 | **79.9063** | 4 |
| `split127 + C_c` (same code) | 8080 | 79.9063 | 4 |
| `disp` (campaign-first, no hoist) | 8088 | 79.0848 | 4 |
| `C_c` (campaign-first, hoist campaign) | 8088 | 79.0848 | 4 |
| `C_i` (campaign-first, hoist instant) | 8092 | 79.3085 | 4 |
| `dispD` / `split127+dispD` | 8092 / 8084 | 79.3085 / 78.4363 | 4 |
| `disp_duptail` (reset duplicates the tail) | 8092 | 79.2507 | 4 |
| `dispE` (instant-first, hoist both) | 8080 | 78.8108 | 0 |
| `dispF` / `I_ci` (hoist both, test instant) | 8080 | 78.7748 | 0 |
| `I_c` (instant-first, hoist campaign) | 8080 | 78.7748 | 0 |
| `I_i` (instant-first, hoist instant) | 8080 | 78.9189 | 0 |
| `nested` / `first` (`else if (!won) { reset } else { won }`) | 8080 | 78.9189 | 0 |
| `keepobj_and` (`hasWon() && !isCampaignMission()`) | 8080 | 78.9189 | 0 |
| `split127` alone | 8100 | 75.1620 | 0 |
| `split127 + dispE` | 8100 | 75.0540 | 0 |
| `split127 + C_i` | 8084 | 78.4363 | 4 |
| `duptail` (HEAD + duplicated tail) | 8084 | 77.1753 | 0 |
| `goto` reset label | 8084 | 77.1753 | 0 |
| `mission_first` (declare `mission` before `keepObjectives`) | 8084 | 76.8732 | 0 |
| `won_swap` (`campaign->hasWon() || hasWon()`) | 8084 | 77.0312 | 0 |
| `beforefailed` (reset checked before `if (failed)`) | 8124 | 78.3672 | 0 |
| `c65first` (move case 65 to the front) | 8100 | 77.5937 | 4 |
| `c65last` (move case 65 to the end) | 8096 | 57.1532 | 4 |
| `expectwon` (`__builtin_expect(..., 1)`) | 8080 | 78.8326 | 9 |
| `expectfailed` (`__builtin_expect(failed, 0)`) | 8072 | 71.3514 | 10 |
| `amount != 0` / `count != 0` | 8072 | 70.9782 | 0 |
| `disp + amount_ne0` / `C_c + amount_ne0` | 8076 | 75.0405 | 4 |

Notes:

* Any size other than 8080 shifts `.text` and drops `byte_exact` to
  3102 (measured on `disp`, 8088 B), so **only 8080-byte variants are
  gate candidates**.
* Moving case 65 does not move the reset block to the front
  (`c65first` still has reset at `0x17b434`, after `removeObjectives`
  at `0x17b32c`); the reset chain is re-placed independently.
* `__builtin_expect` changes the branch weights and moves *more* PLT
  entries (9-10 diffs), so it is not a usable lever.

## 5. Remaining instruction-level difference (unchanged from wave 113)

The instruction multiset is still essentially exact.  The one
non-register difference in the mining loop is:

```
original 17a5ca: cmp.w r9, #0        ; beq.n 17a652
HEAD     17a5ca: cmp.w r9, #-1       ; bgt.n 17a652
```

i.e. the original tests `amount == 0` while HEAD's
`int count = -amount; if (count > 0)` folds to `amount >= 0`.  Writing
`if (amount != 0)` reproduces the original instruction but the compiler
then shrinks the function to 8072 B (score 70.9782 %), so the original
source must differ elsewhere as well.

An **unsigned** comparison does produce the original `cmp #0; beq`:

```cpp
unsigned count = -amount;
if (count > 0) { ... }          // -> cmp.w r8, #0 ; beq.n   (8072 B, 70.9782 %)
int count = -amount;
if (count > 0u) { ... }         // -> same codegen             (8072 B, 70.9782 %)
```

but the register allocator then keeps the counter in `r8` (and reloads
`r9` from `[sp,#24]` at the loop exit) instead of keeping it in `r9`,
and the function shrinks to 8072 B.  So the original's `cmp.w r9,#0`
cannot be reproduced at 8080 B by changing only this comparison.

## 6. Gate / section state (unchanged)

27 sections (exact), file size 2,187,328 B, `.text` 0x1337c8 (size and
base exact), `.ARM.extab` 0x26298 (exact), `.rel.plt` diff 0, imports
207, lint CLEAN, drift 0, `RATCHET PASS` (byte 4500 / linked 4500,
size 4524/4524, data 843/843).

```bash
cd /home/debian/GalaxyOnFire2/wt/w6308
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6   # RATCHET PASS
python3 /dev/shm/w6308/score.py                      # 8080 / 78.9189
```

Scratch (not tracked): `/dev/shm/w6308/` — `score.py` (size/fuzzy/plt),
`rebuild.sh` (touch + link + score, ~1.3 s), `plt.py` (PLT order diff),
`trans.py`/`vars.py`/`vars2.py`/`vars3.py`/`vars4.py`/`vars6.py`
(source transforms), `orig.asm`/`ours.asm` (objdump of
0x17a144..0x17c0d4), `dumpblocks.py`, `mir.sh`/`mirblocks.py`/
`freqs.py` (`-mllvm -print-machineinstrs` + block-frequency
propagation), `rea.json`/`rea.c` (REA decompilation of the function).
