# Status / Generator / SpaceLounge tail (wave 155, w10110): new
# source-level branch-weight hypotheses — no landed `src/` change

Status: **analysis only, no `src/` / `CMakeLists.txt` change**. Worktree
`wt/w10110`, base master `f4e0ea40b` (wave 154). Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000). Scratch `/dev/shm/w10110`.

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370.

Read first: `status-sample-profile-w10020.md`,
`pgo-switchweight-wave152-w9920.md`, `small-tail-b-wave154-w10018.md`.

Metrics: raw same-address diff (`/dev/shm/w10110/fdiff.py`) and
instruction-index alignment (`/dev/shm/w10110/align_diff.py`, a path-adjusted
copy of the committed tool).

| fn | before raw/aligned/size | after raw/aligned/size |
|---|---|---|
| `Status::missionCompleted` (`0xabf38`) | 722 / 320 / 1432 | 722 / 320 / 1432 |
| `Generator::createMission` (`0x971b8`) | 393 / 52 / 1996 | 393 / 52 / 1996 |
| `SpaceLounge::drawLounge` (`0x170d38`) | 175 / 41 / 1948 | 175 / 41 / 1948 |

## 0. Bottom line

1. **Status: the 4>3>2 plateau is still not source-producible in clang 7.0.2.**
   New this wave: an exhaustive search over all 127 non-empty `__builtin_expect`
   masks on a 7-arm grouped if-chain (A/B/C/C13/C14/D/E) and on the 29-arm
   source-order if-chain. The if-chain always folds to a geometric/`sum`-scaled
   `!prof` vector (best measured 1225/561/1352, size regression), and the
   grouped chain gives `[1, 264132x7, 22011x12, 1x10]` (1319/541/1420). A
   `switch` with `__builtin_expect(type, K)` is one-hot by construction
   (`handleSwitchExpect`), and `__builtin_expect_with_probability` is absent.
2. **Generator: the `rawBonus - rem` source form is semantically right but
   costs 4 bytes.** It reproduces the original's `str/ldr [sp,#32]` rem spill
   and `subne r5,r6,r0` (raw 393 -> 304), but the RA keeps `rem` in `r9`
   (spilling `agent` instead) and the stack frame grows 96 -> 104, so the
   function is 2000 B (gate: `byte_exact 4507 -> 4412`). The alternative
   `else if` grouping (1964 B) and moving `agentName` down (1924 B) are worse.
3. **SpaceLounge: the `pad * 2` right arm still flips the colouring.** It gives
   the 2-instruction clamp but 445/54; the `w - (inset + width + pad*2)`
   association gives 196/47; neither beats HEAD 175/41.

## 1. Status — source-level switch-weight search

The winning `!prof` vector (w9920) is confirmed:
`default 1, {167,174,8,0,11,13,14}=4, D=3, E=2` -> **558 / 47 / 1432**
(`pgo_switchweight_w9920.py groups 4 4 4 3 2`). The family is broad:
`F=6,D=4,E=3` and `F=8,D=6,E=4` also give 558/47/1432, so the requirement is
really "three nearly-flat levels", not the exact integers.

New measurements this wave (all real TU compile + checked-in gold link, via
`/dev/shm/w10110/score_src.py`; sizes are `missionCompleted`):

| hypothesis | raw | aligned | size |
|---|---:|---:|---:|
| HEAD | **722** | **320** | **1432** |
| loop-condition `__builtin_expect(i < size, 1)` | 722 | 320 | 1432 |
| `__builtin_expect(m->hasWon(), 0)` | 722 | 320 | 1432 |
| remove the four case-body expects | 894 | 340 | 1428 |
| 29-arm source-order if-chain, all arms expected (best mask) | 1225 | 561 | 1352 |
| grouped F/D/E if-chain, all arms expected | 1319 | 541 | 1420 |
| grouped F/D/E if-chain, `-likely 4 -unlikely 3` | 1255 | 519 | 1372 |
| `!prof` reference `groups 4 4 4 3 2` | **558** | **47** | **1432** |

The exhaustive mask search (`/dev/shm/w10110/search.py`) enumerates all 127
non-empty subsets of the 7 groups; every result is size 1352 (the if-chain
fold changes the switch body) and the best raw is 1225. So the if-chain
route is a structural dead end independent of the weights.

Why the fold cannot make a plateau: `FoldValueComparisonIntoPredecessors`
scales the predecessor's true case by the *sum* of the successor's weights
(`ValidTotalSuccWeight`) and leaves the successor cases alone. With plain
arms the vector is `1, 2, 4, 8, ...`; with `(T,F)` arms it is geometric with
ratio `T/F`. There is no source knob for a flat 4/3/2.

`handleSwitchExpect` (LLVM 7.0.1 `LowerExpectIntrinsic.cpp`) sets exactly one
case (or the default, when the expected value is not a case) to
`-likely-branch-weight` and every other successor to `-unlikely-branch-weight`.
`__builtin_expect(x, c)` emits `llvm.expect(x, c)` where `c` is the expected
*value*, not a weight, so "graded 4/3/2 values" cannot grade the weights.

## 2. Generator — `rawBonus - rem`

The original's bonus block is

```
97706: mov r0, r6            ; rawBonus
97708: blx __aeabi_idivmod   ; r1 = rem
9770c: adds r5, r1, r6       ; bonus = rawBonus + rem
9770e: str r1, [sp, #32]     ; spill rem
97710: movs r1, #50
97712: mov r0, r5
97714: blx __aeabi_idivmod   ; bonus % 50
97718: ldr r0, [sp, #32]     ; rem
9771a: cmp r1, #0
9771c: it ne
9771e: subne r5, r6, r0      ; bonus = rawBonus - rem
```

HEAD compiles `bonus = rawBonus + rawBonus - bonus;` to `rsbne r5,r5,r6,lsl#1`
(no spill). Changing the source to `bonus = rawBonus - rem;` reproduces the
spill and `subne`, but the RA keeps `rem` in `r9` and spills `agent`, so the
frame grows 96 -> 104 and the function is 2000 B:

| variant | raw | aligned | size | gate |
|---|---:|---:|---:|---|
| HEAD | **393** | **52** | **1996** | PASS |
| `bonus = rawBonus - rem;` | 304 | 112 | 2000 | `byte_exact` 4412 |
| `rem` hoisted out of the `else` block | 304 | 112 | 2000 | — |
| `volatile int rem` + `rawBonus - rem` | 702 | 91 | 2000 | — |
| `volatile int rem` + `rawBonus + rawBonus - bonus` | 592 | 34 | 2000 | — |
| `else if` second-if grouping | 1701 | 363 | 1964 | — |
| `agentName` moved down to `new Mission` | 1763 | 485 | 1924 | — |
| second-switch case reorder (15 first) | 393 | 52 | 1996 | — |

The `rawBonus - rem` form is the right source shape; the remaining blocker is
the `r9` colouring tie (`agent` vs `rem`). Forcing `agent` to stay in `r9`
would both restore the 1996 B frame and drop the branch-target shifts.

## 3. SpaceLounge — right-edge clamp

HEAD uses `x = w - inset + (-width - pad - pad)` (4-instruction clamp) and
`pad + pad` for `top`; it is 175/41/1948. The original's clamp is
`addgt.w r1, r6, sl; subgt r5, r0, r1` (2 instructions, `sl = pad*2`).

| right arm | raw | aligned | size |
|---|---:|---:|---:|
| HEAD `+ (-width - pad - pad)` | **175** | **41** | **1948** |
| `- (width + pad * 2)` | 445 | 54 | 1948 |
| `+ (-width - pad * 2)` | 445 | 54 | 1948 |
| `- width - pad * 2` | 445 | 54 | 1948 |
| `- (pad * 2 + width)` | 445 | 54 | 1948 |
| `w - (inset + width + pad * 2)` | 196 | 47 | 1948 |

`top = ... (pad * 2)` vs `(pad + pad)` makes no difference. The `pad*2`
right arm gives the 2-instruction clamp but the RA colours `pad` -> `fp` and
`x` -> `r9` (the same flip HEAD already has) and inserts `mov r5, fp` before
both arms, so it loses on raw and aligned. No shape found that keeps
`pad` in `r9` / `x` in `r5`.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10110
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10110/fdiff.py <mangled>
python3 /dev/shm/w10110/align_diff.py <mangled>
python3 /dev/shm/w10110/score_src.py <mangled> <tu>   # source variant -> raw/aligned/size
W9920_SCRATCH=/dev/shm/w10110/w9920 \
  python3 _work/reconstructions/pgo_switchweight_w9920.py groups 4 4 4 3 2
```

Scratch (`/dev/shm/w10110/`): `fdiff.py`, `align_diff.py`, `showdiff.py`,
`showidx.py`, `score_src.py`, `genchain.py`, `search.py`, `SimplifyCFG.cpp`,
`LowerExpectIntrinsic.cpp`, `CGBuiltin.cpp`, `*.bak` pristine sources.

## 5. Files changed

* `_work/reconstructions/status-generator-spacelounge-wave155-w10110.md`
  (this note).
