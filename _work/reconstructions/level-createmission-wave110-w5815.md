# w5815: `Level::createMission` (92.094405 %, 17172 B) — wave-110:
# cbz-selector / convoy-switch / statement-order sweep; no size-preserving improvement

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w5815`, branch `wave110-w5815`,
base `390f58052`. Ground truth:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; real address = Ghidra address - 0x10000).
**No `src/` change is committed**: every variant measured below is neutral or a
regression. The deliverable is this note. The worktree is clean.

## Baseline (unchanged, gate green before and after)

```text
_ZN5Level13createMissionEv   fuzzy 92.094405 %   size 17172 B (size-exact)
                             orig 6055 insns / ours 6063 insns
                             cbz 35 / 34,  [sp,#84] 17 / 2,  [sp,#88] 2 / 17
gate: byte 4500/4524, linked 4500/4524, size 4524/4524, stubs 0, missing 0,
      wrong_type 0, extra 0, data 843/843, imports 207, lint CLEAN, drift 0,
      RATCHET PASS
```

ELF shape is unchanged by this pass (no source change): 28 sections,
`.ARM.extab` 0x26294 (the pre-existing 4-byte-short delta vs 0x26298).

## Method

Every row below is a fresh `cmake --build cmake-build-match --target link_so`
followed by `score.py` (the official verifier's disassembly + reconcile +
`difflib.SequenceMatcher(autojunk=False)` normalisation, unrounded).  All
variants are applied on top of the committed baseline
(`/dev/shm/w5815/Level.base.cpp`, a byte copy of HEAD `Level.cpp`).

The residual is 317 non-equal diff regions / 557 instructions (of 6055).
33 regions are pure `[sp,#N]` slot swaps (18 of them the `raid`/guard
`sp+84`<->`sp+88` swap); the rest is register allocation, scheduling and
literal-pool slotting.  The two largest regions are the alienGuns table
select (19 insns) and the convoy if-chain (13 insns).

## 1. AlienGuns table select (`if (alienGuns != 0)` loop)

The original selects the table with a real in-loop branch:

```text
b32c6  ldr   r0, [sp, #64]        ; bigAlienWave
b32c8  cbz   r0, b32d0
b32ca  ldr   r1, [pc, #732]       ; kAlienStationTurrets
b32cc  add   r1, pc
b32ce  b     b32d4
b32d0  ldr   r1, [pc, #728]       ; kAlienOutpostTurrets
b32d2  add   r1, pc
b32d4  add.w r0, r1, r9           ; row = t + k
```

Our baseline emits a branchless `cmp.w r1, r2, asr #31; it ne; movne` CMOV.
The `(bigAlienWave + (i < 0))` anchor is what keeps the select inside the loop
(without it the select is LICM-hoisted, 17140 B, 88.186 %).  Tested this pass
(all size 17172 / 6063 insns unless noted):

| variant | pct |
|---|---:|
| **baseline ternary + anchor** | **92.094405** |
| `const float (*const t)[6]` | 92.094405 |
| `(const float (*)[6])` through `const void *` | 92.094405 |
| condition `!= 0` / `> 0` / `>= 1` / `!!` | 92.094405 |
| condition `== 1` | 92.028388 |
| `if (cond) t=A; else t=B;` (anchor) | 92.094405 |
| `if (!cond) t=B; else t=A;` (anchor) | 92.094405 |
| `t=B; if (cond) t=A;` (anchor) | 92.094405 |
| `switch (cond) { case 0: t=B; default: t=A; }` (anchor) | 92.094405 |
| `switch (cond) { case 1: t=A; default: t=B; }` (anchor) | 92.028388 |
| `__builtin_expect(cond, 0/1)` ternary or if (anchor) | 92.094405 |
| pointer arithmetic `A + (cond ? 0 : 8)` | 88.496160 (17132 B) |

The branchy form is **not reachable from source**: clang 7.0.2 IRGen emits a
`select` for `?:` and `SimplifyCFG::FoldTwoEntryPHINode` folds the if/else PHI
(both arms are constants, `DominatesMergePoint` returns true regardless of
`-phi-node-folding-threshold`).  `ARMTargetLowering::LowerSELECT_CC` always
returns `getCMOV`, so a `select` can never become a branch.  A surviving PHI
would need a non-speculatable instruction in each arm (e.g. a volatile access),
which costs >= 8 pp in-level (`ifvol_*` 83.5-84.8 %).

## 2. Statement order in the alienGuns loop

Baseline order: `A createStaticObject, B gun=..., C setMaxHitpoints,
D setScaling, E select, F rot1, G setRotation, H rot2, I setPosition,
J shipGroup, K cargo`.

| variant | pct |
|---|---:|
| **baseline (C,D before E; J,K after I)** | **92.094405** |
| all 6 permutations of C,D,E before E | 92.094405 |
| D at positions 2/3/4 (before E) | 92.094405 |
| D after E (=D_at_5) | 90.774055 (17164 B) |
| D after F | 90.757551 (17164 B) |
| D after G | 90.421582 (17188 B) |
| D after H | 92.028388 |
| D after I (faithful original order) | 92.011883 |
| D after J | 92.011883 |
| D after K (last) | 90.823568 (17164 B) |
| C at positions 2/3/4 (before E) | 92.094405 |
| C after E | 90.625516 (17164 B) |
| C after I | 92.077901 |
| JK at positions 2-5 / 8 | 92.077901 |
| JK at position 9 (=after I, baseline) | 92.094405 |
| KJ anywhere | 92.077901 / worse |

The original's real order is `... I, D, K, J` (setScaling after setPosition,
cargo before shipGroup), which scores 92.011883 — the tuned "D before E"
order is worth +0.08 pp.

Fortress loop (`kBaseTurrets`, `if (pirateFortress)`):

| variant | pct |
|---|---:|
| **baseline (D after I)** | **92.094405** |
| D before E | 91.978874 |
| D before C | 91.978874 |
| D after G | 92.061396 |
| JK swap | 92.044892 |

## 3. Convoy `case 183` if-chain (`for (int k = 0; k < 4; k++)`)

The original is a switch-shaped chain: `bic r1, k, #0x80000000; cmp #2;
beq case2; cmp #1; beq case1; cbnz default; case0` with bodies laid out
`case0, case1, case2, default`.  Our baseline if-chain `2,1,0` emits ascending
tests `cmp #0; beq case0; cmp #1; beq case1; cmp #2; bne default` with bodies
`case2, case0, case1`.  Measured this pass:

| variant | pct |
|---|---:|
| **baseline if-chain 2,1,0** | **92.094405** |
| `switch` cases 0,1,2 (default last) | 91.945866 |
| `switch` default first, then 0,1,2 | 92.094405 |
| `switch` cases 0, default, 1, 2 | 91.945866 |
| `switch` cases 0, 1, default, 2 | 91.945866 |
| `switch` cases 2, default, 1, 0 | 92.094405 |
| `switch (i)` cases 10,11,12 | 91.945866 |
| `for (int k = 0, i = 10; k < 4; k++, i++)` comma init | 92.094405 |
| `for (int i = 10; i < 14; i++) { int k = i - 10; }` | 89.508873 (17164 B) |

The `switch` 0,1,2 form reproduces the original's `bic` and descending tests,
but its body layout is `case0, case2, case1` (clang's switch emission order),
which costs more than the test order gains.  No source spelling produces the
original's `case0, case1, case2` layout (wave-109 reached the same conclusion
for all six if-chain/switch permutations).

## 4. `raid` / stack-guard slot swap (`sp+84` <-> `sp+88`)

Frame layout (from `-mllvm -print-machineinstrs`, PEI dump): after
StackSlotColoring `raid` is `%stack.82` and the guard is `%stack.83`; offsets
are assigned in reverse index order (`stack.81`->92, `stack.82`->88,
`stack.83`->84, `stack.84`->80).  Original has `raid` at 84 and the guard at
88.  `StackSlotColoring::getSpillWeight` sums `block_freq/entry_freq` over all
17 `raid` references; our `raid` weight 1.197958 is just above the guard's
1.193760 (margin 0.0042).  Tested this pass:

| variant | pct |
|---|---:|
| **baseline `bool raid; if/else`** | **92.094405** |
| single-expression `bool raid = !(...) && ...` | 92.094405 |
| `bool raid = false; if (!(...)) raid = ...` | 92.094405 |
| ternary initializer | 92.094405 |
| `int raid` (0/1) | 91.929361 |
| `__builtin_expect((security==3)&raid, 0)` | 92.094405 |
| `__builtin_expect(raid, 0)` | 72.868473 (17296 B) |
| `__builtin_expect(raid, 1)` | 83.571016 (17192 B) |
| `__builtin_expect(raid && campaign>=32, 0)` | 88.212489 (17188 B) |

No variant flips the slot without re-laying the function.  The wave-109
finding stands: only CFG-changing spellings flip it (`(security==3) && raid`
flips it and costs 5.0 pp).

## 5. Convoy / loop / condition spelling sweeps

| variant | pct |
|---|---:|
| **baseline** | **92.094405** |
| `if (bigAlienWave)` instead of `== 1` in body1 | 91.780822 |
| `if (1 == bigAlienWave)` (Yoda) | 92.094405 |
| `if ((bigAlienWave & 1) != 0)` | 91.780822 |
| `if (alienGuns)` / `> 0` | 92.094405 |
| `if (pirateFortress != 0)` / `raid != 0` / `convoy != 0` / `blueprint != 0` / `stationPirates != 0` | 92.094405 |
| `const unsigned n` / `int i < (int)n` / `(unsigned)size()` / split decl in case4Tail | 92.094405 |
| `Vector v;` / `Vector v(x,y,z)` / `Vector v{}` in alienGuns prologue | 92.094405 |
| `Matrix rotation = Matrix();` | 92.094405 |
| `int k = 0;` before `Matrix rotation` | 92.094405 |
| `Vector base` before `Vector v` | 91.929361 |
| pirate-escort `Vector spawn = player->getPosition()` | 73.266182 (17312 B) |
| pirate-escort `Vector spawn; spawn = ...` | 73.277782 (17336 B) |
| pirate-escort `float sx/sy/sz` locals | 90.838561 (17168 B) |

## 6. `(i != first) | !alienWave` merged predicate

Original emits `subs.w r0, fp, r3; mov r1, r0; it ne; movne r1, #1;
ldr r2, [sp,#60]; eor r2, #1; orrs r1, r2; bne` (one branch).  Ours emits
short-circuit `ldr r0,[sp,#60]; cmp r0,#0; beq; cmp fp,r3; bne` (two
branches).  Tested: `!(a && b)` (both conditions), `(i != first) | !alienWave`,
`(i != first) | (alienWave == 0)`, `bool atFirst = ...`, `alienWave != 0`,
`__builtin_expect(...,0/1)` — all neutral (92.094405) or worse
(`|` forms 74-85 %).  The `orrs` merge is a SimplifyCFG `FoldBranchToCommonDest`
CFG decision, not a source spelling.

## Proven vs hypothesis

Proven (measured this pass):

* The alienGuns table select cannot be made branchy from C++ source with
  clang 7.0.2 `-Oz`: `?:` is IRGen `select` -> `getCMOV`; if/else PHIs are
  folded by `FoldTwoEntryPHINode` because both arms are constants.
* The convoy switch 0,1,2 form reproduces the original's `bic` + descending
  tests, but clang's body emission order is `0,2,1`, not the original's
  `0,1,2`; the test-order gain is smaller than the layout loss.
* The alienGuns / fortress statement order is already at its measured optimum
  (D before E; J,K after I).  The faithful original order (`I,D,K,J`) scores
  92.011883.
* The `raid`/guard slot swap is a StackSlotColoring weight ordering with a
  0.0042 margin; no size-preserving spelling flips it.

Hypothesis (not landed):

* The original's branchy select and merged `orrs` predicate change the block
  frequencies that drive the `raid`/guard StackSlotColoring weights; fixing
  either would probably also fix the 18 pure `sp+84`/`sp+88` swaps.  Every
  spelling that produces them re-lays the function and loses more than the
  swap gains.
* The branchy select may come from a slightly different clang 7.0.2 revision
  (or a different optimization pipeline) rather than a C++ spelling we can
  write.

## Commands

```sh
cd /home/debian/GalaxyOnFire2/wt/w5815
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6     # RATCHET PASS

# fast single-function score (official verifier normalisation, unrounded)
python3 /dev/shm/w5815/score.py
python3 /dev/shm/w5815/score.py _ZN5Level13createMissionEv diff   # difflib opcodes
python3 /dev/shm/w5815/diffstat.py                                # region sizes

# frame-layout inspection (analysis only; not part of the build)
/dev/shm/w5815/cc.sh -mllvm -print-machineinstrs -o /tmp/L.o
```

Scratch (not committed): `/dev/shm/w5815/` (`Level.base.cpp`, `score.py`,
`diffstat.py`, `dumpaddr.py`, `idx.py`, `cc.sh`, `run2.py`..`run52.py`,
`sel*.c`, `sw.c`, `pa.txt`, `mi.txt`, `Level.ll`, `Level_if.ll`,
`ARMISelLowering.cpp`, `SimplifyCFG.cpp`, `CodeGenPrepare.cpp`,
`StackSlotColoring.cpp`, `LiveIntervals.cpp`).

## Gate counts (before -> after)

```text
byte_exact      4500 -> 4500   (4500/4524)
linked_exact    4500 -> 4500   (4500/4524)
size_exact      4524 -> 4524
stub_zero_size     0 -> 0
missing            0 -> 0
wrong_type         0 -> 0
extra              0 -> 0
data_exact       843 -> 843
imports          207 -> 207
lint            CLEAN   -> CLEAN
drift           0       -> 0
RATCHET         PASS    -> PASS
```
