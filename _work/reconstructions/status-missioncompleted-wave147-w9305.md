# Wave 147 (w9305): `Status::missionCompleted` — clang 7's *only* source producer
# of switch `!prof` is `switch (__builtin_expect(t, K))` (`handleSwitchExpect`);
# it emits a *single-hot-case* vector, which is outside the winning family, so
# the source-order layout is still unreachable from source.  Note-only.

Status: **note-only** (no `src/` change landed).  Gate on the unchanged tree:
`byte_exact 4507/4524`, `linked_exact 4507/4524`, size 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0, `RATCHET PASS`.  Worktree `wt/w9305`, base
master `9c9744ca5` (wave 146).  Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).

Target: `_ZN6Status16missionCompletedEbbx` — original `0xabf38`, **1432 B**,
baseline raw same-address diff **722 B** (re-measured this wave).

## 0. New results

### 0.1 NEW: LLVM 7 *does* have `handleSwitchExpect` — the mechanism is now known
`llvm/lib/Transforms/Scalar/LowerExpectIntrinsic.cpp` (release/7.x) contains
`handleSwitchExpect(SwitchInst&)`:

```cpp
CallInst *CI = dyn_cast<CallInst>(SI.getCondition());
if (Fn->getIntrinsicID() != Intrinsic::expect) return false;
SwitchInst::CaseHandle Case = *SI.findCaseValue(ExpectedValue);
unsigned n = SI.getNumCases();
SmallVector<uint32_t, 16> Weights(n + 1, UnlikelyBranchWeight);   // 1
if (Case == *SI.case_default()) Weights[0] = LikelyBranchWeight;  // 2000
else Weights[Case.getCaseIndex() + 1] = LikelyBranchWeight;
SI.setMetadata(LLVMContext::MD_prof, ...);
```

So `switch (__builtin_expect(t, K))` **is** a real front-end producer of switch
branch weights in clang 7.0.2.  Verified in the `-Oz` `.ll`:

```
switch i32 %52, label %380 [ ... ], !prof !9299
!9299 = !{!"branch_weights", i32 1, i32 2000, i32 1, i32 1, ...}
```

(`K` a case → that case gets 2000, everything else 1; `K` not a case → the
default gets 2000.)  This is the *only* source-level producer of switch weights
besides PGO.  `__builtin_expect_with_probability` **does not exist in clang
7.0.2** (no `BI__builtin_expect_with_probability` in `CGBuiltin.cpp`), and
`__builtin_unpredictable` only sets `MD_unpredictable` (measured: no change,
722/1432).

### 0.2 The winning weight family (1296-vector sweep, real optimised IR)
Using a fast `.ll` -> `.o` scorer (`/dev/shm/w9305/fast.py`: size from the
object symbol table + block-rank sequence from the `tbh` table), sweeping
`(default=1, A, low, B, C)` over `1..6` where the IR case order is
`167,174 (A), 8,0,11,13,14 (low), 150..189 minus C (B), 156..172 (C)`:

**23 winners** (size 1432 *and* rank 40/40):
```
(A,low,B,C) = (2,5,4,3) (3,4,3,2) (3,5,4,3) (3,6,4,3) (3,6,5,4)
              (4,4,3,2) (4,5,4,3) (4,6,4,3) (4,6,5,4)
              (5,4,3,2) (5,5,4,3) (5,6,4,3) (5,6,5,4) (5,6,6,5)
              (6,2,2,1) (6,3,3,2) (6,4,3,2) (6,4,4,3)
              (6,5,4,3) (6,5,5,4) (6,6,4,3) (6,6,5,4) (6,6,6,5)
```
Load-bearing shape: **`B = C + 1`**, `low >= B`, `A >= B-2`.  The original
vector is *graded* (at least three distinct weight levels across the switch),
not "one hot case".

Confirmed end-to-end: `(1,3,4,3,2)` and the w9214 vector
`1,3,3,4,4,4,4,4,3x12,2x10` both give 1432 B and rank 40/40 (the w9214 vector
re-measures at raw **642** in this harness, not 558 — see 0.4).

### 0.3 All source-producible vectors are outside the family
`handleSwitchExpect` can only produce `{1,...,2000 at K,...,1}` (or
`{2000,1,...}`), i.e. `B = C = low = 1` for every K — never `B = C+1`.
Measured with the real TU + real gold link (raw / size):

| source form | raw | size |
|---|---:|---:|
| master | **722** | 1432 |
| `switch (__builtin_expect(t, 167))` | 1277 | 1392 |
| `switch (__builtin_expect(t, 8))` | 1291 | 1384 |
| `switch (__builtin_expect(t, 0))` | 1319 | 1424 |
| `switch (__builtin_expect(t, 14))` (only size-exact K) | 1295 | 1432 |
| `switch (__builtin_expect(t, 1))` (default hot) | 1317 | 1400 |
| `switch (__builtin_unpredictable(t))` | 722 | 1432 |
| `default:` first or last | 722 | 1432 |
| if-chain `expect(t==c,1)` (27 arms) | 1380 | 1456 |
| if-chain `expect(t==c,0)` (27 arms) | 1343 | 1440 |
| every case-body guard wrapped in `expect(...,1)` (28 sites) | 1276 | 1364 |
| every `if` in the switch bodies wrapped (41 sites) | 1298 | 1372 |

The if-chain fold produces *geometric* weights (`ValidTotalSuccWeight` scaling,
w144 §0.3/§0.4), also not `B = C+1`.

### 0.4 MachineBlockPlacement dump: only the tail reorders
`-mllvm -print-after-all -mllvm -filter-print-funcs=...` shows the MBB order
after *Branch Probability Basic Block Placement* is **identical for the base
and the injected-weights build for the first 37 blocks**; the orders diverge
only from layout position 37.  So the fix is a localised tail reordering
driven by the *relative* probabilities, not a global re-layout — which is why
a single-hot-case vector scrambles it instead of fixing it.

### 0.5 Harness correction (matters for future waves)
`ninja libgof2hdaa.so` after hand-compiling `Status.cpp.obj` **recompiles the
object from the checked-in source**: ninja hashes the command line, and the
hand compile's command differs.  w9018/w9214's `score.sh` avoided this by
re-running the gold link directly; do the same
(`ninja -t commands libgof2hdaa.so | tail -1`).  With the direct link, the
w9214 vector gives raw **642** in this tree (not 558) — the 558 figure was
from a slightly different base.

## 1. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9305/fdiff.py _ZN6Status16missionCompletedEbbx   # 722 / 1432

# fast .ll -> .o scorer (size + tbh block-rank match):
python3 /dev/shm/w9305/fast.py 1,3,3,4,4,4,4,4,3,3,3,3,3,3,3,3,3,3,3,3,2,2,2,2,2,2,2,2,2,2
# -> (1432, 40)
```

Scratch (`/dev/shm/w9305/`): `fdiff.py`, `score.sh` (source -> .o -> checked-in
gold link -> raw), `score_ll.sh`, `inject.py` (insert `!prof` into the real
switch), `fast.py` (size+rank scorer), `sweep.py` (the 1296-vector sweep),
`blocks.py` (rank comparison on the linked `.so`), `tbh.py` (decode both `tbh`
tables), `linkcmd.sh`, `pall.txt`/`pbase.txt` (machine dumps).

## 2. Open items

1. The original's weights are **graded** (`B = C+1`, `low >= B`).  No clang-7
   source form emits a graded switch weight vector: `__builtin_expect` gives a
   single hot case, the if-chain fold gives geometric weights, and
   `__builtin_expect_with_probability` is absent.  The remaining lever is a
   build-level profile (`-fprofile-instr-use` on this TU), which is outside the
   fixed build recipe.
2. If a graded vector is ever produced, the within-block residual (642 B in
   this harness) still needs the w9214 §0.3 fixes (166 pgi-first, drop the 157
   expect, keep the 158 tail out of the shared `cmp r0,#0`).
