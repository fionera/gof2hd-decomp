# w5716: `Level::createMission` (92.094405 %, 17172 B) — wave-109:
# selector / convoy-chain / stack-slot sweep; no size-preserving improvement

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w5716`, branch `wave109-w5716`,
base `7c34c4098`. Ground truth:
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
(`/dev/shm/w5716/Level.base.cpp`, a byte copy of HEAD `Level.cpp`).

## 1. Convoy offset if-chain (`case 183`, `for (int k = 0; k < 4; k++)`)

The original tests `k` in **descending** order and lays the bodies out
`case0, case1, case2, default`:

```text
001440 beq.n  0x1460      ; cmp #2 -> case2
001442 cmp    r1, #1
001444 beq.n  0x1454      ; case1
001446 cbnz   r1, 0x146c  ; default
001448 case0 ...          ; fallthrough
001454 case1 ...
001460 case2 ...          ; uses `str.w sl` for offset[1]=0
00146c default ...
001476 join: str r1,[sp,#380]
```

Our baseline (source if-chain written 2,1,0) emits tests 0,1,2 and body order
`case2, case0, case1, join, default`.  Clang canonicalises every source
spelling.  Measured (all size 17172 / 6063 insns):

| variant | pct |
|---|---:|
| **baseline (if-chain 2,1,0)** | **92.094405** |
| `switch (k)` cases 2,1,0 | 92.094405 |
| `switch (k)` cases 2,0,1 | 92.094405 |
| `switch (k)` cases 0,1,2 | 91.945866 |
| `switch (k)` cases 0,2,1 | 91.945866 |
| `switch (k)` cases 1,0,2 | 91.962370 |
| `switch (k)` cases 1,2,0 | 91.962370 |
| if-chain 0,1,2 (ascending) | 91.945866 |
| nested if (2 / 1 / 0) | 92.094405 |

The original body order `0,1,2` is **not reachable** from any of the six
permutations: clang emits either `0,2,1` (case0 fallthrough) or `2,0,1`
(case2 fallthrough), never `0,1,2`.

## 2. `raid` / stack-guard stack-slot swap (`sp+84` <-> `sp+88`)

Frame layout (from `-mllvm -print-machineinstrs`, PEI dump): after
StackSlotColoring `raid` is `%stack.82` and the `__stack_chk_guard` value is
`%stack.83`; offsets are assigned in *reverse* index order
(`stack.81`->92, `stack.82`->88, `stack.83`->84, `stack.84`->80).  The
original has `raid` at 84 and the guard at 88, i.e. the colour order is
reversed.  StackSlotColoring colours by decreasing spill weight, and (wave-106)
our `raid` weight 1.197958 is just above the guard's 1.193760 — a 0.0042
margin.  Every flip found changes the CFG and costs >= 5 pp:

| variant | size | ours insns | pct | sp84 |
|---|---:|---:|---:|---:|
| **baseline** | **17172** | **6063** | **92.094405** | **2** |
| `(security == 3) && raid` | 17196 | 6065 | 87.095710 | **17** |
| `raid && security == 3` | 17172 | 6063 | 92.094405 | 2 |
| `raid & (security == 3)` | 17172 | 6063 | 92.094405 | 2 |
| `(security == 3) & (int) raid` | 17172 | 6063 | 92.094405 | 2 |
| `char raid` / `unsigned char raid` / `unsigned raid` | 17172 | 6063 | 91.929361 | 2 |
| `int raid` (wave-106) | 17172 | 6063 | 91.863344 | 2 |
| `bool raid;` moved before `security` | 17172 | 6063 | 92.094405 | 2 |
| `bool raid;` moved before `stationIndex` | 17172 | 6063 | 91.252682 | 2 |
| `raid` decl+init moved after `raidCoords`/`Route` | 17304 | 6040 | 71.004547 | 4 |
| `raid = raid;` / `raid = raid \|\| false;` before `if (raid)` | 17172 | 6063 | 92.094405 | 2 |

The slot swap is downstream of block frequencies: the `&&` form adds a
short-circuit block that flips the weight ordering but re-lays the whole
function.

## 3. In-loop alienGuns table select

The original's select is a real branch inside the loop
(`ldr r0,[sp,#64]; cbz; ldr r1,[pc]; add r1,pc; b; ...`) followed by a shared
body.  Our `(bigAlienWave + (i < 0)) ? A : B` anchor keeps the select in the
loop but lowers it branchless (`cmp.w r1,r2,asr #31; it ne; movne`).  Every
plain-condition spelling is LICM-hoisted out of the loop.  Measured:

| variant | size | ours insns | pct |
|---|---:|---:|---:|
| **baseline anchor** | **17172** | **6063** | **92.094405** |
| `if (bigAlienWave + (i<0)) t=A; else t=B;` | 17172 | 6063 | 92.094405 |
| `t=B; if (bigAlienWave + (i<0)) t=A;` | 17172 | 6063 | 92.094405 |
| `t=A; if (!(bigAlienWave + (i<0))) t=B;` | 17172 | 6063 | 92.094405 |
| `switch (bigAlienWave + (i<0))` case 0/default | 17172 | 6063 | 92.094405 |
| `if` with two statements per arm (force speculation cost) | 17172 | 6063 | 92.094405 |
| `if (bigAlienWave) t=A; else t=B;` (no anchor) | 17140 | 6058 | 88.186246 |
| `t=B; if (bigAlienWave) t=A;` (no anchor) | 17140 | 6058 | 88.186246 |
| faithful both (no anchor + `enemies->size()` reload) | 17132 | 6053 | 88.486951 |

Named component floats (`f3..f5`, `f0..f2`, all six) are neutral: the
`ldrd`-vs-`ldr` load pairing in the vector construction is unchanged (it is
`ARMLoadStoreOptimizer`, not source shape; `-mllvm -arm-load-store-opt=false`
reproduces the original but is a compile flag).

## 4. `case4Tail` hunter loop

The original reloads `this->enemies->size()` every iteration and hoists the
`true` argument into `fp` (`mov.w fp,#1` before the loop).  Our cached
`unsigned n = this->enemies->size()` is a size-tuning win:

| variant | size | ours insns | pct |
|---|---:|---:|---:|
| **baseline (`unsigned n` cache)** | **17172** | **6063** | **92.094405** |
| `int n` cache | 17172 | 6063 | 92.094405 |
| `for (i < enemies->size())` (faithful reload) | 17164 | 6058 | 91.257327 |
| `for (i < (unsigned) enemies->size())` | 17164 | 6058 | 91.257327 |
| `unsigned n = hunterCount` | 17164 | 6060 | 91.225753 |
| `for (i < (unsigned) hunterCount)` | 17164 | 6060 | 91.225753 |
| named `bool alwaysTrue` / `const int one` for the `true` arg | 17172 | 6063 | 92.094405 |
| `createShip(..., 1, 0)` instead of `true,false` | 17172 | 6063 | 92.094405 |
| cached `Route *route = this->playerRoute` | 17168 | 6061 | 90.937603 |

## 5. Loop / declaration spelling sweep (all size 17172, 6063 insns)

| variant | pct |
|---|---:|
| `for (int i = start, k = 0; ...)` comma-init | 92.094405 |
| `int k = 0;` before `Matrix rotation` | 92.094405 |
| `Vector base` before `Vector v` | 91.747813 |
| `Vector base; base = ...;` (split decl/init) | 89.059279 (17204 B, 6074) |
| `for (int k = 0; k < alienGuns; k++) { int i = ...+k; }` | 86.956522 (17188 B, 6066) |
| `int i = ...; for (int k = 0; k < alienGuns; k++, i++)` | 88.070668 (17172 B, 6058) |

## Proven vs hypothesis

Proven (measured this pass):

* The convoy if-chain body order `0,1,2` is unreachable by any source
  permutation; every spelling either matches the baseline (`2,0,1`) or loses
  0.13-0.15 pp (`0,2,1`).
* The `raid`/guard slot swap is a StackSlotColoring weight ordering.  Only
  CFG-changing spellings flip it (`(security==3) && raid` flips it and costs
  5.0 pp); type changes (`char`/`unsigned`/`int`) and declaration moves do not.
* The in-loop table select is always speculated to a branchless `select` when
  the condition is loop-variant; the only branchy form duplicates the body and
  costs 3.4 pp.
* The `case4Tail` `unsigned n` cache is worth +0.84 pp over the faithful
  `enemies->size()` reload.
* The alienGuns vector `ldrd` pairing is `ARMLoadStoreOptimizer` output; source
  reshuffling (named floats, brace-init, parens) does not change it.

Hypothesis (not landed):

* The original's `orrs` merged station predicate (`(i != first) | !alienWave`)
  and its branchy in-loop select both change the block frequencies that drive
  the `raid`/guard StackSlotColoring weights.  Fixing either would probably fix
  the 18 pure `sp+84`/`sp+88` swaps as a side effect, but every spelling that
  produces them re-lays the function and loses more than the swap gains.
* The 0x1444 body order `0,1,2` likely comes from the original's switch
  lowering being fed the cases in a different order than any C++ spelling we
  can write (or from a slightly different clang revision); it is a 8-instruction
  region and not worth further effort.

## Commands

```sh
cd /home/debian/GalaxyOnFire2/wt/w5716
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6     # RATCHET PASS

# fast single-function score (official verifier normalisation, unrounded)
python3 /dev/shm/w5716/score.py
python3 /dev/shm/w5716/score.py _ZN5Level13createMissionEv diff   # difflib opcodes

# frame-layout inspection (analysis only; not part of the build)
/dev/shm/w5716/cc.sh -mllvm -print-machineinstrs -o /tmp/L.o   # stderr has the dumps
```

Scratch (not committed): `/dev/shm/w5716/` (`Level.base.cpp`, `score.py`,
`score2.py`, `dump.py`, `batch.py`, `cc.sh`, `t1.py`..`t22.py`, `mibase.txt`,
`cm_pei.txt`, `cm_ssc.txt`).

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
