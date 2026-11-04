# MGame::OnTouchEnd (8080 B) — w5707 (wave 109): reset-block placement is the
# `.rel.plt` blocker; no gate-passing gain found

Target: `_ZN5MGame10OnTouchEndEiiPv` (file VMA 0x17a144, 8080 B,
`src/game/menu/MGame.cpp`).  Base: master `7c34c4098`, branch `wave109-w5707`.

## Result

**No source change landed this pass.**  Baseline is unchanged and green:

| shape | size | fuzzy | matched | .rel.plt diff | gate |
|---|---:|---:|---:|---:|---|
| **HEAD (unchanged)** | **8080** | **78.8829** | 1338/2775 | 0 | RATCHET PASS (byte 4500 / linked 4500) |
| `split127+disp` | 8080 | **79.8559** | 1137/2776 | 4 | byte 4499 (FAIL) |
| `split127+disp+mv22_95` | 8096 | **81.98** | 1087/2776 | 4 | size + byte FAIL |
| `split127+disp+mv74_96` | 8092 | 81.8 | 1140/2776 | 4 | size + byte FAIL |
| `split127` alone | 8100 | 75.1 | 1057/2781 | 0 | size |
| `split127+chain_400first` | 8076 | 70.2 | 609/2775 | 2 | size |
| `split127+refined_bool+cnt_ne` | 8084 | 65.0 | 1044/2779 | 0 | size |
| `split127+disp+mv42_81` | 8088 | 76.5 | 1237/2775 | 0 | size |

`check` after restoring HEAD: byte 4500 / linked 4500, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, **RATCHET PASS**.

## 1. The 20-byte gap is pure codegen, not source shape

`split127` (split the combined `96 || 127` case into a faithful `== 96` case
plus a `== 127` case after 126, matching the original's `cmp r0,#127` at
0x17bc34 and its `beq.w 0x17b722` back-edge into the 96 body) is faithful but
8100 B.  The original is 8080 B.  Instruction-multiset comparison of the
normalized disassembly (`split127` vs original) shows the +20 B is:

* 3 × `cbz r0, L` → `cmp r0,#0` + `beq` (+2 B each, +6 B) — the `cbz` targets
  are out of `cbz` range in our layout;
* 3 × `b.n L` → `b.w L` (+2 B each, +6 B) — branch relaxation;
* 3 × extra `nop` (alignment before literal pools, +2 B each, +6 B);
* ~2 B residual.

None of these are source-shape differences; they are all block-layout
artifacts.  This confirms the earlier waves' conclusion and explains why
`disp` (an *unfaithful* swap of the `isInstantActionMission` /
`isCampaignMission` test order) is the only found way to recover the 20 B.

## 2. `.rel.plt` is the blocker for every high-scoring shape

`.rel.plt` order is the order of first reference in the TU.  The original
references `LevelScript::resetStartSequenceOver` (0x17b0ae) **before**
`Level::removeObjectives` (0x17b270).  HEAD keeps that order (reset 0x17b22a,
removeObjectives 0x17b286, diff 0).  `split127+disp` moves the cold reset
block to 0x17b49a, i.e. after removeObjectives/goingToWaypoint/removeRoute,
permuting `.rel.plt[3324..3327]`:

```
slot  original                       split127+disp
3324  LevelScript::resetStartSequenceOver   Level::removeObjectives
3325  Level::removeObjectives          PlayerEgo::goingToWaypoint
3326  PlayerEgo::goingToWaypoint       PlayerEgo::removeRoute
3327  PlayerEgo::removeRoute           LevelScript::resetStartSequenceOver
```

`MGame::successCheck` (1468 B, was 100 % byte/linked-exact) calls
removeObjectives (0x683dc), goingToWaypoint (0x683e8) and removeRoute
(0x683f4) at 0x18025a / 0x18016a / 0x18017a.  The rotation moves all three
PLT stubs by -0xc, so its `blx` immediates change and it loses
byte-exactness: `check` on `split127+disp` gives `byte_exact 4500 -> 4499`,
RATCHET FAIL (verified, not inferred).  There is no source-level way to keep
successCheck byte-identical under the rotated PLT: the call targets are
different symbols at the same addresses.

## 3. The reset block placement is robust against source restructuring

Tried this pass (all measured):

| variant | size | fuzzy | plt |
|---|---:|---:|---:|
| `won_invert` (`if (!(A||B)) reset; else chain`) | 8080 | 78.9 | 0 (code-identical to HEAD) |
| `goto` (`goto reset` / `goto afterWonBody`) | 8080 | 78.9 | 0 (code-identical) |
| `two_if` (`bool won; if(!won) reset; if(won) chain`) | 8080 | 78.9 | 0 (code-identical) |
| `dup_tail` (reset branch duplicates frameTime/tail) | 8084 | 77.1 | 0 |
| `split127+disp+won_invert` | 8080 | 79.9 | 4 |
| `split127+disp+goto` | 8080 | 79.9 | 4 |
| `split127+disp+two_if` | 8080 | 79.9 | 4 |
| `split127+disp+failed_nest` | 8080 | 79.9 | 4 |
| `split127+disp+failed_neg` | 8080 | 79.8 | 4 |
| `split127+disp+dup_tail` | 8084 | 78.3 | 4 |

Clang canonicalises every equivalent spelling of the
`hasWon || campaignHasWon` / `else reset` dispatch back to the same IR, so the
reset block always lands between campaign cases 65 and 81 under `disp`.

## 4. Searches run this pass (wave 109 additions)

### 4a. Exhaustive `mvX_Y` on `split127+disp` (272 variants)

Every ordered pair of the 18 campaign cases was moved (all 272 builds
measured, `/dev/shm/w5707/sweep_sd.log`).  Results:

* No variant is 8080 B **and** `.rel.plt` diff 0.
* The only `.rel.plt` diff-0 variant is `mv42_81`: 8088 B / 76.5 %.
* The best 8080 B variants are 79.9 % with plt 4 (e.g. `mv96_95`, `mv95_74`,
  `mv81_65`, `mv74_81`, `mv65_42`, `mv100_96`, `mv110_100`, `mv120_110`,
  `mv126_120`, `mv144_134`, `mv155_144`, `mv161_155`, `mv162_161`).
* The best overall is `mv22_95`: 8096 B / 82.0 % / plt 4 (size kills
  `byte_exact`: any `.text` size change shifts `.rodata`/`.data` and tanks
  the raw-byte count, e.g. `check` on it gives `byte_exact 3102`).

### 4b. Placement of the `== 127` case (17 variants)

The 127 case was inserted after each of the 18 campaign cases.  Sizes swing
between 8080 and 8100 B:

```
after15 8088  after22 8080  after43 8080  after42 8096  after65 8092
after81 8096  after74 8092  after95 8096  after100 8100 after110 8100
after120 8100 after126 8100 after134 8100 after144 8096 after155 8096
after161 8088 after162 8092
```

`after22`/`after43` are 8080 B but clang then checks 127 **before** 96
(`cmp r0,#127` at 0x17b1f0, `cmp r0,#96` at 0x17b99a), so the fuzzy collapses
to 66.8 %.  `after126` is the faithful `split127` shape: 8100 B / 75.1 %.

### 4c. Size deltas of every single transform on HEAD

```
split127 +20   disp +8   refined_bool -4   chain_400first -16   cnt_ne -8
swap_96_100 +8 swap_96_126 +8 swap_96_134 +12   all others 0
```

Combinations that should have cancelled to 8080 did not, because the
transforms interact: `split127+chain_400first` = 8076,
`split127+chain_400first+refined_bool` = 8076,
`split127+refined_bool+cnt_ne` = 8084,
`split127+disp+mv42_81+cnt_ne` = 8084,
`split127+disp+mv42_81+refined_bool` = 8088,
`split127+disp+mv42_81+chain_400first` = 8072.

### 4d. `__builtin_expect` / won-first experiments

`__builtin_expect` is used elsewhere in the fleet, so it was tried on the
`hasWon` and `hasFailed` conditions (values 0 and 1) on both HEAD and
`split127+disp`.  All are worse:

| variant | size | fuzzy | plt |
|---|---:|---:|---:|
| `split127+disp+won_exp1` | 8080 | 78.8 | 9 |
| `split127+disp+won_exp0` | 7832 | 60.6 | 13 |
| `split127+disp+fail_exp0` | 8072 | 71.3 | 10 |
| `split127+disp+fail_exp1` | 7836 | 58.0 | 10 |
| `head+won_exp1` | 8072 | 75.1 | 9 |
| `head+fail_exp0` | 8068 | 72.6 | 9 |

`won_first` (test `hasWon` before `hasFailed`, `else if (!failed) reset`)
gives 8092 B / 63.4 % / plt 6 on HEAD and 8096 B / 61.3 % / plt 6 on
`split127+disp`.

## 5. Why the layout differs (evidence)

The original block order (VMA) is:

```
0x17adc8  hasWon / getCampaignMission / hasWon   (won test)
0x17ade0  keepObjectives
0x17af5e  mission ternary
0x17af7e  isInstantActionMission dispatch
0x17afb6  campaign/freelance won body
0x17b01a  hud0 body (closeHudMenu, ...)
0x17b07e  mission-failed branch (type 12 -> changeCredits)
0x17b0aa  resetStartSequenceOver
0x17b0b6  freelance won body continuation
0x17b722  case-96 body (shared with 127)
0x17b89c  finishMission (removeObjectives ...)
0x17b8e2  shared tail (frameTime = 0; resetAnalogStick; releaseAllKeys)
```

Our HEAD order is:

```
0x17adc8  won test
0x17af72  mission-failed branch
0x17afa8  mission ternary / isInstantActionMission dispatch
0x17b226  resetStartSequenceOver
0x17b230  campaign chain
0x17b3da  hud0 body
0x17b6dc  finishMission
0x17b722  shared tail
0x17b848  case-96 body
```

So the two large cold clusters (`hud0 body` vs `mission-failed branch`) are
emitted in the opposite order, and `finishMission`/tail sit before the
campaign chain in ours but after it in the original.  This is a
MachineBlockPlacement decision; every equivalent source spelling tried
(`won_neg`, `won_invert`, `goto`, `two_if`, `failed_nest`, `failed_neg`,
`won_first`, `__builtin_expect`) either canonicalises to HEAD's IR or
changes the function size.

## 6. Proven vs hypothesis

**Proven from the binary**

* The 96/127 split (`cmp r0,#127` at 0x17bc34, `beq.w 0x17b722`) and the
  campaign-case order 15,22,43,42,65,81,74,95,96,100,110,120,126,127,134,144,
  155,161,162.
* `.rel.plt[3324..3327]` order and the `successCheck` byte-exactness loss
  (`check` byte 4499).
* `split127+disp` is 8080 B / 79.8559 % / plt 4; `split127+disp+mv22_95` is
  8096 B / 81.98 % / plt 4; any non-8080 size tanks `byte_exact` (3102 for
  the 8096 B shape).
* Clang canonicalises `won_invert`, `goto`, `two_if`, `won_neg`,
  `failed_nest` to the HEAD layout (identical fuzzy/size/plt).
* Exhaustive `mvX_Y` on `split127+disp` finds no 8080 B / plt-0 shape.

**Hypothesis**

* The only remaining lever is MachineBlockPlacement's cold-block order: the
  original places the reset block before the freelance-won body and the
  campaign chain; our build places it after case 65 under `disp` and after
  case 81 under HEAD.  No source spelling found changes that order without
  also changing the 20-byte codegen balance.
* Recovering 8080 B + plt 0 + >78.9 % needs either a faithful 20-byte
  reduction that keeps the HEAD cold-block order, or a way to make the reset
  block hotter than the campaign chain in MBP.

## 7. Exact commands

```bash
cd /home/debian/GalaxyOnFire2/wt/w5707
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6          # RATCHET PASS
python3 /dev/shm/w5707/score.py                             # official fuzzy for this fn
python3 /dev/shm/w5707/pltcmp.py                            # .rel.plt diff count
python3 /dev/shm/w5707/pltshow.py                           # .rel.plt diff with names
python3 /dev/shm/w5707/tr2.py split127 disp                 # apply transforms + rebuild
python3 /dev/shm/w5707/tr2.py split127 disp mv22_95
python3 /dev/shm/w5707/sweep_sd.py                          # exhaustive mvX_Y on split127+disp
python3 /dev/shm/w5707/exp2.py                              # 127-case placement sweep
python3 /dev/shm/w5707/sizedelta.py                         # per-transform size deltas
python3 /dev/shm/w5707/ndiff.py                             # normalized instruction diff
python3 /dev/shm/w5707/multiset.py orig.s s127.s            # instruction multiset diff
```

Scratch (not tracked): `/dev/shm/w5707/` (`tr2.py`, `var1.py`, `var3.py`,
`rnd2.py`, `rnd3.py`, `sweepdisp.py`, `sweep_sd.py`, `exp.py`, `exp2.py`,
`exp3.py`, `exp4.py`, `sizedelta.py`, `score.py`, `pltcmp.py`, `pltshow.py`,
`ndiff.py`, `multiset.py`, `blockseq.py`, `caseseq.py`, `orig.s`, `ours.s`,
`s127.s`, `sd.s`, `sweep_sd.log`, `sizedelta.log`).
