# `Status::missionCompleted` — wave 123 (w7305): the A-then-B order is a
# branch-weight effect; `!prof` reproduced exactly, but not expressible from C++

Status: **note-only** (no source change landed).  Worktree `wt/w7305`, base
master `1c8c9b20e` (wave-122 final snapshot).  Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(sha256 `d4c17be6...bed9483`, armeabi-v7a, NDK r18b clang 7.0.2 r328903,
`-Oz`, image base 0x10000).

Target: `_ZN6Status16missionCompletedEbbx` (`Status::missionCompleted(bool,
bool, long long)`), original `0xabf38`, size **1432 B**.

| | baseline (HEAD `1c8c9b20e`) |
|---|---:|
| function score | **71.0 %** |
| function size | 1432 B |
| gate | byte 4506 / linked 4506 |
| lint / drift | CLEAN / 0 |

## 0. Summary

The residual is entirely the **case-body placement order**.  The original is
two ascending runs

```
167/174  150 151 152 153 154 155 158 162 166 170 184 189      (run A)
         156 157 160 163 164 165 168 169 171 172              (run B)
latch
```

while our plain `switch (m->getType())` emits ascending **value** order
(`150 151 ... 155 156 157 158 160 162 ... 172 184 189`).  The prologue, the
two-TBH dispatch, the low group and the shared tails are already
byte-identical; only the body order differs.

**New result this pass:** the original body order is exactly what
`MachineBlockPlacement` produces when the high jump table's edges carry
`!prof` branch weights with **A > B**.  A hand-patched `base.ll` with
`branch_weights` `{default 1, 167 2, 174 2, A 2, B 1, low 1}` compiled with
LLVM 7.0.0 `llc` reproduces the original order **body for body** (see §2).
`__builtin_expect` on the switch condition is the only C++ construct that
emits `!prof` on a switch, but `LowerExpectIntrinsic::handleSwitchExpect`
sets exactly **one** weight to 2000 and all others to 1 — it cannot express
"A > B".

Every source shape that reproduces the order changes the function size
(minimum 1376 B / 1408 B, next 1440 B), and a size change collapses
`byte_exact` (4506 -> 2327 for +8 B, measured).  So no gate-green source
win was found; the exact mechanism is recorded here.

## 1. Baseline evidence (measured this pass)

```sh
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target verify -j6
# report.json row: pct 71.0, osize 1432, usize 1432
cmake --build cmake-build-match --target check -j6   # RATCHET PASS byte 4506 linked 4506
```

`verify --show _ZN6Status16missionCompletedEbbx`: the prologue
(`push/add/stmdb/ldrd/mov r6,r0/mov.w fp,#0/mov r8,r1` …) and the whole
dispatch (`sub.w r1,r0,#150; cmp r1,#39; bhi; tbh` + 40-entry table, then
`sub r1,r0,#8; cmp #6; bhi; tbh` + 7-entry table, then `cmp r0,#0`) are
byte-identical.  First difference is the high TBH table halfwords at
`0xabff2`, i.e. the body addresses.

Register allocation of the original (and of our plain switch):
`this` = r6 (spilled to `[sp,#16]`), `i` = fp, `m` = sl, `atStation` = r8.

## 2. The key experiment: `!prof` branch weights with A > B give the
##    exact original order

`base.ll` is the NDK-generated IR for the current `Status.cpp`
(`clang++ -S -emit-llvm`).  The `switch i32 %52` in
`@_ZN6Status16missionCompletedEbbx` has 29 cases.  Adding

```llvm
  ], !prof !999
...
!999 = !{!"branch_weights", i32 1, i32 2, i32 2, i32 1, ..., i32 2, ..., i32 1}
```

(default 1, 167 = 2, 174 = 2, every A body 2, every B body 1, low 1) and
running

```sh
llc -mtriple=thumbv7-none-linux-androideabi -mattr=+neon -float-abi=soft \
    -filetype=asm -o p.s p.ll
python3 sorder.py p.s      # decodes the TBH target order from the asm
```

gives

```
ORDER: 167/174 150 151 152 153 154 155 158 162 166 170 184 189
       156 157 160 163 164 165 168 169 171 172 <default>
```

i.e. **exactly the original**.  The same holds for weights 3/1, 4/1, 10/1
and for 167/174 = 2 (its two case values sum to 4).  With weights 1/10
(B > A) the order flips to B-then-A, and with no `!prof` it is value order.
So the order is a pure edge-frequency effect, not a source-order effect.

Mechanism: `SelectionDAGBuilder::buildJumpTable` adds the jump-table
successors in `Table` (ascending value) order and
`MachineBasicBlock::normalizeSuccProbs()` keeps the per-body
`JTProbs[Succ]` sum.  `MachineBlockPlacement::selectBestCandidateBlock`
then picks the **maximum-frequency** candidate, keeping the first on ties.
With all bodies at equal frequency (no `!prof`) the worklist is drained in
successor order = value order.  With A > B, the A bodies are all strictly
hotter than the B bodies, so they are drained first, and within A the
tie-break gives ascending value order — which is run A.  Run B follows.
`167/174` is first because its two case values give it double weight.

## 3. Why `__builtin_expect` cannot express A > B

`switch (__builtin_expect(m->getType(), K))` does emit `!prof` on the
switch (verified: the IR has `!{!"branch_weights", i32 1, ..., i32 2000,
...}` with the 2000 at the slot of case `K`).  But
`LowerExpectIntrinsic.cpp`:

```cpp
static bool handleSwitchExpect(SwitchInst &SI) {
  ...
  unsigned n = SI.getNumCases();
  SmallVector<uint32_t, 16> Weights(n + 1, UnlikelyBranchWeight);   // all 1
  if (Case == *SI.case_default())
    Weights[0] = LikelyBranchWeight;                                // 2000
  else
    Weights[Case.getCaseIndex() + 1] = LikelyBranchWeight;          // 2000
  SI.setMetadata(LLVMContext::MD_prof, ...);
```

Exactly one successor gets weight 2000; every other weight is 1.  Nested
`__builtin_expect` does not help: `handleSwitchExpect` runs once per block
terminator and then the expect calls are erased.  `__builtin_expect_with_probability`
does not exist in clang 7.0.2 (compile error).  There is no other C++
construct that emits `!prof` on a `switch` (PGO would, but that is a build
flag).

Measured `switch (__builtin_expect(m->getType(), K))` variants (NDK build,
official single-symbol score):

| K | usize | order |
|---|---:|---|
| (none, base) | 1432 | value |
| 150 | 1404 | `150 default 167/174 151 152 ...` |
| 155 | 1404 | `155 default 167/174 150 151 ...` |
| 160 | 1388 | `160 default 167/174 150 ...` |
| 167 | 1396 | `167/174 default 150 151 ... 172 184 189` |
| 170 | 1392 | `170 default ...` |
| 189 | 1420 | `189 default 167/174 150 ...` |
| 0 | 1428 | `default 167/174 150 ...` |
| 14 | 1432 | scattered (`156 151/152/154 153 173 185 ...`), pct 16.0 |
| not-a-case (999, -1, 149) | 1400 | `default` group pulled forward |

Only K = 14 keeps 1432 B, and its order is scrambled (pct 16.0).  All the
other K values change the size, so they are not gate-green.

## 4. The hole-filling trick: exact order, wrong size

The high range `[150,189]` has 40 slots but only 24 case values; the 16
holes (`159 161 173 175 176 177 178 179 180 181 182 183 185 186 187 188`)
currently point at the default (latch).  Giving each A body one extra
`case <hole>:` label (and 167/174 two) raises every A body's `JTProbs` to
2/… while the B bodies stay at 1/… — i.e. it synthesises the A > B
`!prof` weights **through the case-cluster probabilities** (`sortAndRangeify`
merges adjacent same-destination clusters and sums their `Prob`; here the
sum is over the two case values pointing at the same body).

Result (NDK build):

```
size 1376
HIGH: 167/174 150 151 152 153 154 155 158 162 166 170 184 189
      156 157 160 163 164 165 168 169 171 172 <default>
```

— the **exact original order**.  But the function shrinks 1432 -> 1376
(-56 B) and the register allocation changes:

```
original / base : this = r6 (spill [sp,#16]), i = fp, m = sl
hole-filled     : this = fp,                  i = sl, m = r9
```

so the bodies are renamed and the official score is only **44.8 %**
(`pct 44.8, usize 1376`).  Adding `__builtin_expect(m->getType(), 167)`
on top restores 167/174 to the front but gives 1408 B, pct 34.2.
Partial fills (k = 0..4 A bodies) stay at 1432 B but only reorder the
first few A bodies and score 67.2 / 66.x / ... / 60.9 — all worse than the
71.0 baseline.

Measured hole-fill sizes (all with the 16 holes assigned, varying the
"extra" hole placement and the assignment order): **always 1376**.

## 5. Two-switch shapes (re-measured with the NDK build)

Splitting the switch so that the B bodies have a second (later) predecessor
reproduces the order; the second dispatch is the cost.

| shape | usize | order | pct |
|---|---:|---|---:|
| `[{167/174}+A]` then `[low+B]`, cached `int` | **1440** | exact | 43.5 |
| `[{167/174}+A]` then `[low+B]`, `short` | 1444 | exact | — |
| `[{167/174}+A]` then `[low+B]`, re-call `getType()` | 1400 | broken | 34.8 |
| `[{167/174}+low+A]` then `[B]`, cached `int` | 1508 | exact | 52.1 |
| `[{167/174}+A]` then `[B]` then `[low]` | 1480 | exact | 40.0 |
| `[{167/174}+A]` then `[low]` then `[B]` | 1580 | exact | 48.7 |
| `[low]` then `[{167/174}+A]` then `[B]` | 1512 | exact | 39.2 |

A sweep of all 16 low-group splits between the two switches and four cached
types (`int`, `short`, `unsigned short`, `unsigned char`) found **1440 B as
the minimum** exact-order shape.  Removing the `(unsigned short)` size
anchor does not change it.  The +8 B is the second switch's
`sub/cmp/bhi + tbb` dispatch; it cannot be removed without losing the
second predecessor that delays the B bodies.

## 6. Other shapes tried (all at 1432 B unless noted)

- 4 landed `__builtin_expect(...,1)` body hints (166, 184, 157, 163) —
  baseline 71.0.  A greedy single-hint sweep over all 36 body conditions
  (value 0 and 1) found **no addition that beats 71.0 at 1432 B**; every
  other 1432-B single hint scores <= 71.0.
- loop-condition / `hasWon()` / `m == 0` / campaign-check hints (value 0/1,
  all 16 combinations): order unchanged, sizes 1368-1452.
- `default:` placed first, `default: break;`, nested `do{}while(0)`,
  `if(1)`, cached `int` — value order, 1432 B.
- `switch (__builtin_expect(...))` with K over 0..19 and 145..199: only
  K = 14 is 1432 B, order scrambled.
- `default` = case-150 body / case-189 body: 1412-1424 B, wrong order.
- A-group fallthrough (semantically wrong): 1388 B, exact order, 49.7.
- duplicate A case labels: 1376 B, A-then-B-ish, 45.8.

## 7. Root cause (unchanged, now pinned to weights)

`SelectionDAGBuilder::buildJumpTable` adds the high-TBH successors in
ascending case-value order and `MachineBlockPlacement`'s
`selectBestCandidateBlock` drains them by block frequency, keeping the
first on ties.  The original's A-then-B layout is what you get when the A
bodies are strictly hotter than the B bodies, i.e. when the switch carries
`!prof` `branch_weights` with A > B (proved in §2).  Our plain switch has
uniform edge probabilities, so the bodies drain in value order.

The original's dispatch, prologue and low group are byte-identical to
ours, so the original's IR differs from ours **only** in the switch's
branch weights (consistent with the original having been PGO-built).  From
plain C++11 the only `!prof` producer is `__builtin_expect`, which sets a
single weight (2000) and leaves the rest at 1, so it cannot express A > B;
synthesising the weights via extra case labels (the hole-filling trick)
does reproduce the order but changes the register allocation and shrinks
the function to 1376 B, which is not gate-green.

## 7b. New this pass: an if/else-if chain with `__builtin_expect` does emit
##     `!prof` on the switch, but with uninitialised (garbage) weights

clang 7's front end folds a chain of `if (x == C1) ... else if (x == C2) ...`
into a `SwitchInst` (verified: `vif.cpp`, a mechanically generated if/else
chain over the same bodies, compiles to a 1432-B function with the *same*
value-order layout, i.e. it is the same switch).  If each condition is
wrapped in `__builtin_expect`, the folded switch carries `!prof`
`branch_weights`:

```
!5 = !{!"branch_weights", i32 0, i32 -1778235696, i32 1257729, i32 621,
       i32 0, i32 0, ...}
```

These weights are **independent of the expected values** (changing every
`1` to `0` gives byte-identical metadata) and depend only on the number of
cases / chain order.  They are uninitialised `SmallVector` contents (a
clang-7 bug: the if-to-switch fold sizes the weight vector but never fills
it), so they are deterministic for a given source but semantically
meaningless.  For the real 23-body chain only the first 1-3 successor
weights are non-zero (e.g. `0, -1835992721, 1228865, 606, 0, ...`), which
makes only the *first* case hot -- the same effect as a single
`__builtin_expect` on the switch condition, not A > B.  Measured variants
(chain order A-then-B, B-then-A, low-first, low-last) all gave
1396-1404 B and never the original order, so this is not a usable route
either.  It does, however, show a second way the original could have
acquired `!prof` weights without PGO.

## 8. Reproduction

```sh
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target verify -j6     # pct 71.0, 1432/1432
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506

# the !prof proof (local instrumented LLVM 7.0.0):
#   base.ll = clang++ -S -emit-llvm of src/game/mission/Status.cpp
#   patch the `switch i32 %52` terminator with
#     ], !prof !999
#     !999 = !{!"branch_weights", i32 1, i32 2, i32 2, i32 1, ..., i32 2, ..., i32 1}
#   (default 1, 167/174 2, A 2, B 1, low 1)
#   llc -mtriple=thumbv7-none-linux-androideabi -mattr=+neon -float-abi=soft \
#       -filetype=asm -o p.s p.ll && python3 sorder.py p.s
#   -> ORDER: 167/174 150 151 152 153 154 155 158 162 166 170 184 189
#              156 157 160 163 164 165 168 169 171 172 <default>
```

Scratch (not committed): `/dev/shm/w7305/` — `base.cpp`/`base.ll` (current
source snapshot), `gen.py` (switch-partition builder), `eval.py` (size +
TBH body-order decoder), `cand.sh` (copy -> link_so -> official
single-symbol score -> restore), `score1.py`, `sorder.py`, `decode.py`,
`p_2_1_*.ll`/`p_2_1_*.s` (the `!prof` proof variants),
`swk{0,4,7,12}.cpp` (hole-fill variants), `w_a_0A_lowB.cpp` (1440-B
two-switch), `greedy2.py`/`sweep3.py` (hint sweeps), `vif*.cpp`
(if/else-chain variants), `genchain.py` (if/else-chain generator).
