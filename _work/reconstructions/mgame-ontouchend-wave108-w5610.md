# MGame::OnTouchEnd (8080 B) — w5610 (wave 108): block-placement sweep, no gate-passing gain

Target: `_ZN5MGame10OnTouchEndEiiPv` (Ghidra entry 0x18a144, file VMA 0x17a144,
8080 B, `src/game/menu/MGame.cpp`).  Base: master `8f98ec4d6`, branch
`wave108-w5610`.

## Result

**No source change landed this pass.**  Baseline is unchanged and green:

| shape | size | fuzzy | matched | .rel.plt diff | gate |
|---|---:|---:|---:|---:|---|
| **HEAD (unchanged)** | **8080** | **78.8829** | 2189/2775 | 0 | RATCHET PASS (byte 4500 / linked 4500) |
| best tuned, gate-failing | 8080 | **79.8559** | 2216/2775 | 4 | byte 4500 -> 4499 (FAIL) |

`check` after restoring HEAD: `byte 4500 / linked 4500`, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, **RATCHET PASS**.

## 1. The remaining gain is real but blocked by `.rel.plt`

`split127 + disp` (split the combined `96 || 127` case into a faithful
`== 96` case plus a `== 127` case placed after 126, and swap the
`isInstantAction`/`isCampaign` dispatch order) is **8080 B and 79.8559 %**,
i.e. +0.97 % over HEAD.  It is the only size-8080 shape found above HEAD.

It fails the gate for exactly one reason: the `.rel.plt` first-reference
window 3320..3333 permutes by one slot.

```
slot  original                       split127+disp
3320  PlayerEgo::dockToStream        same
3321  MGame::useCloak                same
3322  MGame::maneuverTouchEnd        same
3323  Mission::isInstantActionMission same
3324  LevelScript::resetStartSequenceOver   Level::removeObjectives   <-- shifted
3325  Level::removeObjectives        PlayerEgo::goingToWaypoint
3326  PlayerEgo::goingToWaypoint     PlayerEgo::removeRoute
3327  PlayerEgo::removeRoute         LevelScript::resetStartSequenceOver
3328  LevelScript::skipCutscene      same
3329.. MenuTouchWindow::inCinematicMode, MGame::setCinematicMode,
       Hud::resetAnalogStick, Radar::getLockedEnemy, Level::enableFog  (all same)
```

`MGame::successCheck` (1468 B, 100 % fuzzy, linked-exact) calls
`removeObjectives`/`goingToWaypoint`/`removeRoute`; the one-slot shift changes
its `bl` immediates, so it loses byte-exactness and `byte_exact` drops
4500 -> 4499 (`check` verified).  `linked_exact` stays 4500.

**Proven requirement:** the original's `.rel.plt` order proves that in the
original `resetStartSequenceOver` is first referenced *before*
`removeObjectives`.  In `split127+disp` the reset call site is at 0x17b49a,
`removeObjectives` at 0x17b2dc — reset is 0x1be bytes too late.

## 2. MIR evidence for the reset block placement

`-mllvm -print-machineinstrs` on the `split127+disp` TU (final MIR,
`/dev/shm/w5610/mir_sd_fn.txt`):

```
bb.227 (%ir-block.836)   hasWon(campaign) test   preds bb.226
  successors: bb.341(37.5%), bb.228(62.5%)       ; bb.341 = reset
bb.316 (%ir-block.1551)  removeObjectives        ; placed at position 316
bb.341 (%ir-block.1625)  resetStartSequenceOver  ; placed at position 341
```

So clang's MachineBlockPlacement puts the cold reset block 25 blocks *after*
the `finishMission` block.  In HEAD the same reset block is `bb.300`
(`%ir-block.1591`) and `removeObjectives` is `bb.309` (`%ir-block.1517`),
i.e. reset is placed *before* it.  `disp` changes the IR block order of the
won-body dispatch, which cascades into the cold-block placement order and
moves reset past `removeObjectives`.

## 3. Measurements (official `tools/verify` fuzzy; HEAD = 78.8829 / 8080 / plt 0)

`plt` = differing positions in `.rel.plt` vs the original.

| variant | size | fuzzy | plt | note |
|---|---:|---:|---:|---|
| HEAD | 8080 | 78.8829 | 0 | baseline |
| `split127+disp` | 8080 | **79.8559** | 4 | best overall at 8080 |
| `split127+disp2` | 8080 | 79.8559 | 4 | disp2 == disp |
| `disp` | 8088 | 79.0631 | 4 | |
| `dispD` (both bools, campaign first) | 8092 | 79.3153 | 4 | |
| `dispE` (both bools, instant first) | 8080 | 78.7748 | 0 | size/plt OK, below HEAD |
| `dispF` (both bools, campaign-decl order) | 8080 | 78.7387 | 0 | below HEAD |
| `dispC` (nested campaign) | 8080 | 78.8829 | 0 | code-identical to HEAD |
| `gotowon` (explicit `goto wonBody/touchEndDone`) | 8080 | 78.8829 | 0 | code-identical to HEAD |
| `separateif` (two `if`s instead of `else if`) | 8080 | 78.8829 | 0 | code-identical to HEAD |
| `cntlt` / `cntpos` / `cntneg` / `amtdec` | 8080 | 78.8829 | 0 | code-identical to HEAD |
| `cntne` / `cntne2` / `cntuse` / `cntne4` (`if (amount != 0)`) | 8072 | 70.9550 | 0 | emits `cmp r9,#0` but swaps counter to r8 |
| `nested+exphf0` | 8084 | 73.6577 | 0 | right test->tail order, epilogue at 0x0e3a |
| `nested+exphf0+bool` | 8084 | 72.6126 | 0 | |
| `resetearly` (reset hoisted before the failed branch) | 8136 | 67.2432 | 6 | reset lands at 0x17af8c (before isInstantAction) |
| `wonfirst` (won branch before failed branch) | 8092 | 63.3874 | 6 | |
| `resetmid` / `negwon` / `hoistwon` / `hoistwon2` / `resetgoto` | 8080 | 78.8829 | 0 | code-identical to HEAD |
| `he` (hud0 as `else if` of wingman chain) | 8080 | 76.0000 | 0 | |
| `bool` | 8076 | 76.3243 | 0 | |
| `swapfailed` | 8080 | 78.8108 | 0 | |

Searches run this pass (all filtered to size 8080 and `plt == 0`):
exhaustive pairs over 23 transforms, all pairs `disp`/`dispD`/`dispE`/`split127`
x 28 transforms, and 400 random 2–5 transform combinations over 28 transforms.
**Nothing at 8080 B with `plt == 0` beats HEAD's 78.8829**; every such
combination is either code-identical to HEAD or scores 70–78.8.

## 4. What was tried and why it did not move the needle

* Splitting the `96 || 127` case (`split127`) is *faithful*: the original
  checks `== 96` at 0x17b71c and `== 127` at 0x17bc34, with the 127 check
  branching back to the 96 body at 0x17b722 (`beq.w 17b722`).  It costs +20 B
  on its own (8100 B) because HEAD's redundant
  `(isCampaignMission && ... == 127)` spelling is 20 B smaller than the
  original elsewhere.  Only `disp`'s block re-layout brings it back to 8080.
* `disp` is *not* faithful to the call order (the original tests
  `isInstantActionMission` at 0x17af7e *before* `isCampaignMission` at
  0x17afa8), but it is the only transform that recovers the 20 B.
* `resetearly` puts the reset call before `isInstantActionMission`
  (0x17af8c < 0x17b0d6), i.e. one slot too early, and costs +56 B.
* `nested+exphf0` reproduces the original `hudTouchFlags == 0` test -> tail
  order (test 0x0d68, tail 0x0d72) but the epilogue lands at 0x0e3a instead of
  the original 0x0ec0 and the function is 8084 B.

## 5. Proven vs hypothesis

**Proven from the binary**

* `.rel.plt[3320..3333]` order and the caller sets above (objdump +
  `readelf -rW` of the original).
* The original's 96/127 split and the `beq.w 17b722` back-edge.
* The original's `isInstantActionMission` -> `isCampaignMission` call order.
* `split127+disp` is 8080 B / 79.8559 % and permutes `.rel.plt` by one slot;
  `check` on it gives `byte_exact 4499` (FAIL).
* The MIR block numbers for the reset block and `removeObjectives` in both
  HEAD and `split127+disp`.

**Hypothesis / tuned**

* `disp` is a tuning hack (unfaithful call order) kept only because it is the
  only way found to make `split127` size-neutral.
* The remaining mismatch is entirely MachineBlockPlacement block order.
  Matching it needs the cold `resetStartSequenceOver` block to be placed
  before the `finishMission`/`removeObjectives` block without changing the
  function length or the `.rel.plt` window.

## 6. Exact commands

```bash
cd /home/debian/GalaxyOnFire2/wt/w5610
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
python3 /dev/shm/w5610/score.py                          # official fuzzy for this fn
python3 /dev/shm/w5610/pltcmp.py                         # .rel.plt diff count
python3 /dev/shm/w5610/tr.py split127 disp               # apply transforms + rebuild
python3 /dev/shm/w5610/tr2.py <new transforms...>        # incl. wave-108 transforms
python3 /dev/shm/w5610/rand3.py <seed> <N>               # random 2-5 transform search
bash /dev/shm/w5610/cc.sh                                # single-TU -print-machineinstrs
```

Scratch (not tracked): `/dev/shm/w5610/` (`tr.py`, `tr2.py`, `newt.py` with
`nestedwon`, `cntne`..`cntne4`, `cntuse`, `cntlt`, `cntpos`, `cntneg`,
`amtdec`, `dispC`..`dispF`, `gotowon`, `wonfirst`, `resetearly`,
`separateif`; `score.py`, `pltcmp.py`, `pltorder.py`, `realdiff.py`,
`rand3.py`, `pairdisp.py`, `mir_sd_fn.txt`).
