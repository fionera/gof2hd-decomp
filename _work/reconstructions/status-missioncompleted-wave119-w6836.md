# `Status::missionCompleted` — wave 119 (w6836): the A/B split is two increasing
# runs, branch weights reproduce the original layout exactly, and the rank
# table is confirmed at its ceiling

Status: **research note (no source change landed)**.  Worktree `wt/w6836`,
branch `wave119-w6836`, base master `fdb68f8ee`.  Original binary:
`../_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a, NDK r18b clang
7.0.2 r328903, `-Oz`, image base 0x10000).

Target: `_ZN6Status16missionCompletedEbbx` (`Status::missionCompleted(bool,
bool, long long)`), original `0xabf38`, size **1432 B**.  Baseline row:
**92.5 % / 1432 B** (byte False, linked False).  Gate after this pass (source
unchanged):

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
# verify: byte 4505  linked 4505  stubs 0  missing 0  extra 0  avg 99.94
# data:   exact 843 of 843 exported objects
# sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
# RATCHET PASS
```

Nothing beat 92.5 %.  This note records the new structural facts found this
wave, the exact body-size ledger, and every variant measured.

## 1. The original body order is two *increasing* runs

Write the 22 big bodies in the original's address order and their
`type - 150` values:

```
A = 150 151 152 153 154 155 158 162 166 170 184 189   (12 bodies, increasing)
B = 156 157 160 163 164 165 168 169 171 172           (10 bodies, increasing)
```

Both groups are strictly increasing in *value* order.  That is why the
plain switch's worklist (value order) can produce A in the right order, and
it is also why the two-switch variant below reproduces the original layout.

## 2. Two sequential switches reproduce the original layout (1504 B)

`Status_seq.cpp` splits the switch in two:

```cpp
int mtype = m->getType();
switch (mtype) { case 0: case 11: ... case 167: case 174:
                 case 150: ... case 189: /* A */ }
switch (mtype) { case 156: ... case 172: /* B */ }
```

The MBP trace shows the first switch lowers to **two** jump tables (a low
8..14 table plus a 150..189 table) and the second switch to a **third**
table for B.  The A bodies are then drained from `BlockWorkList` in value
order (which is A's order) and the B table is placed afterwards, so the
resulting layout is exactly the original's:

`167/174, 150,151,152,153,154,155,158,162,166,170,184,189, 156,...,172`.

Measured: **1504 B, 47.1 %** (official verify).  The +72 B comes from
`mtype` being held in `fp` for the whole function (`mov fp,r0` instead of
`mov r6,r0`/`mov.w fp,#0`), which shifts every callee-saved register and
enlarges body 167/174 (22 B vs 16 B) and the low group (+32 B).

This is the first *C++-only* shape that produces the original body order.

## 3. Branch weights reproduce the original layout exactly (1368 B)

Hand-editing `plain.ll` to add `!prof !{!"branch_weights", ...}` on the big
switch with `default=1, low=1, 167/174=2, A=2, B=1` gives the exact
original layout (verified with `order.py` target decode):

```
rank 0..39 -> 167/174, 150,151,152,153,154,155,158,162,166,170,184,189,
              156,157,160,163,164,165,168,169,171,172, default
```

i.e. the A bodies (weight 2) are all picked before the B bodies (weight 1)
by `selectBestCandidateBlock`, and ties inside each group fall back to the
worklist's value order.  Size **1368 B** (the weights also change the
prologue/body 8 allocation, `-64 B`).  So the *only* thing standing between
the plain switch and a byte-exact layout is unequal case frequencies.

Plain C++ cannot attach `!prof` to a `switch`:

* `__builtin_expect` on the switch operand becomes a `select`;
* an `if/else-if` chain with `__builtin_expect` is folded by
  `FoldValueComparisonIntoPredecessors`, but the weights degenerate
  (multiplied and rescaled at every fold) and the range split flips;
* the chain also loses the jump table entirely: a 27-group chain on the
  real function measured **1448 B / 16.2 %**, TBH at `+0x1d6` (vs `+0xaa`),
  value order.

## 4. Rank-table measurements (all worse than committed)

All built with `cmake --build cmake-build-match --target link_so` and scored
with the same normalisation as `tools/verify/verify.py`.

| variant | size | pct |
|---|---:|---:|
| committed rank table | 1432 | **92.5** |
| body 166 `hasEquipment(m->getProductionGoodIndex(), 1)` (re-query) | 1440 | 75.8 |
| re-query + drop `(unsigned short)` cast | 1440 | 75.8 |
| drop `(unsigned short)` cast | 1432 | 74.9 |
| re-query with `Array<Item*>* cargo` hoisted | 1440 | 75.8 |
| re-query with `amount` declared first | 1440 | 75.8 |
| `const int index/amount` | 1432 | 92.5 |
| `unsigned index/amount` | 1432 | 92.5 |
| `Array<Item*>* cargo = ship->getCargo()` first | 1432 | 91.9 |
| plain switch, type labels, A-then-B source order, re-query | 1432 | 59.4 |
| two sequential switches on cached `int mtype` | 1504 | 47.1 |
| `__builtin_expect` if-chain (27 groups) | 1448 | 16.2 |

The re-query is 104 B (original 100 B) because the rank-table dispatch
clobbers `r0` and the allocator spills `this` to `[sp,#16]`, using
`mov r1,r6; mov r6,r0` for `amount` instead of the original's single
`mov r5,r0`.  `index` lands in `r5` (not `r9`/`sb`) and `amount` in `r6`.

## 5. Body-size ledger (why the ±4 shifts exist)

Measured from the TBH target deltas between consecutive bodies:

| body | original | ours | delta | cause |
|---|---:|---:|---:|---|
| 166 (case 10) | 100 | 96 | -4 | cached `index` instead of the re-query |
| 170 (case 11) | 14 | 14 | 0 | shifted -4 |
| 184 (case 12) | 58 | 58 | 0 | shifted -4 |
| 189 (case 13) | 60 | 68 | +8 | `MISSION_CASE` + `Globals::status` literal pool (8 B) emitted inside the body |
| 156 (case 14) | 42 | 42 | 0 | shifted +4 |
| 168 (case 20) | 30 | 26 | -4 | `uxth` (+2) but no nop/pool in the span |
| all others | — | — | 0 | |

Cumulative offsets: `0` through body 166, `-4` for 170/184/189, `+4` for
156..168, `0` from 169 on.  Net 0, so the function size stays 1432 B.
Fixing all three (166 +4, 189 -8, 168 +4) would put every body at the
original address and make all 40 TBH entries match; every partial fix
makes the score worse (measured: `nocast` 74.9, re-query 75.8).

## 6. The rank table is at its ceiling (re-confirmed)

The TBH is indexed by the rank, and the layout order is the rank order, so
the two cannot be decoupled: the original's slot for type `T` is `T-150`,
while the layout order needs an *increasing* rank sequence along
`A then B`.  The committed assignment gives 18 positional TBH matches
(difflib ratio 0.725 on the 40-entry table).  A DP that maximises positional
matches reaches 24 matches but only ratio 0.675 (the extra matches break the
sequence); a 3000-sample random search over monotone 22-rank assignments plus
the two free 167/174 ranks found nothing above 0.725.  This matches the
w6805/w6826 result.

## 7. Conclusion

* The original layout is the MBP **fallback** order (w6805) and, equivalently,
  the **frequency order** produced by `A=2, B=1` branch weights (§3).
* The plain switch already has the exact dispatch
  (`sub.w r1,r0,#150; cmp r1,#39; bhi; tbh`) and the exact body allocation
  (`r6=this, sl=m, fp=i, sb=index`) but lays the bodies out in value order.
* The rank table buys the layout at the cost of the dispatch (one extra
  `ldrb`), the TBH slots (capped by the LIS), a `MISSION_CASE` literal-pool
  word inside body 189, and a different body-166 allocation.
* The only remaining route to a byte-exact body order is a source shape that
  gives the big switch `!prof` weights (`A=2, B=1`) — not expressible in
  plain C++ with clang 7.

## 8. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6   # RATCHET PASS, byte 4505
python3 /dev/shm/w6836/scorefn.py                    # 92.5 %, 523/524 insns
```

Scratch (not committed): `/dev/shm/w6836/` — `scorefn.py` (single-function
verify score), `tbh.py` (TBH target decode from the linked `.so`),
`diffobj.py`, `disours.py`, `ranks.py` (rank-assignment DP + random search),
`Status_plain.cpp` / `Status_seq.cpp` / `Status_chain.cpp` (the three source
shapes), `plain_wt.ll` (hand-edited `!prof` IR), and the instrumented LLVM
7.0.2 `llc` (`/tmp/llvm_android_702b/build2/bin/llc`, `MBP_TRACE`).
