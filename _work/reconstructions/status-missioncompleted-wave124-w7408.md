# `Status::missionCompleted` — wave 124 (w7408): if-chain / nested-switch
# branch-weight sweep (no source win)

Status: **note-only** (no source change landed).  Worktree `wt/w7408`, base
master `6f8ee2320` (wave-123 final snapshot).  Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(sha256 `d4c17be6...bed9483`, armeabi-v7a, NDK r18b clang 7.0.2 r328903,
`-Oz`, image base 0x10000).

Target: `_ZN6Status16missionCompletedEbbx` (`Status::missionCompleted(bool,
bool, long long)`), original `0xabf38`, size **1432 B**.

| | baseline (HEAD `6f8ee2320`) | after this pass |
|---|---:|---:|
| function score (reported) | **71.0 %** | **71.0 %** |
| function score (exact LCS) | **70.992366 %** (523 vs 525 instrs) | same |
| function size | 1432 B | 1432 B |
| gate | byte 4506 / linked 4506 | **byte 4506 / linked 4506** |
| avg / data / imports | 99.94 / 843 / 207 | same |
| lint / drift | CLEAN / 0 | **CLEAN / 0** |
| section deltas | — | **none** (source unchanged) |

## 0. Summary

The residual is still only the case-body placement order.  The original is
`167/174, A, B, default` (A = 150 151 152 153 154 155 158 162 166 170 184
189; B = 156 157 160 163 164 165 168 169 171 172).  The plain
`switch (m->getType())` emits value order and scores 71.0.  Wave 123 proved
that a hand-patched `!prof` `branch_weights` with **A > B** reproduces the
original order exactly, but `__builtin_expect` on a switch sets exactly one
weight to 2000 and leaves the rest at 1.

This pass exhaustively tried the remaining plain-C++ shapes suggested by the
assignment:

1. `if`/`else-if` chains with `__builtin_expect` on the chain conditions
   (the fold emits garbage `!prof` weights from uninitialised
   `SmallVector` contents).
2. Nested switches / nested if-chains in the `default`.
3. Mixed switch/if shapes.
4. Single- and pair-expect sweeps over the original chain order.
5. Random sparse expect maps (200 variants) and random chain permutations.
6. Body-hint value combinations and a full single-hint sweep over every
   `if` in the function.

**No shape improved the score at 1432 B.**  Every shape that produced the
A-then-B order changed the register allocation away from the original
(`this = r6`, `i = fp`, `m = sl`, `atStation = r8`), and the register/body
mismatch cost far more than the order gained.  The plain if-chain with no
expects folds into the *same* 29-case switch as the source switch and scores
exactly 71.0.

## 1. Baseline evidence (measured this pass)

```sh
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target verify -j6
# report.json row: pct 71.0, osize 1432, usize 1432
cmake --build cmake-build-match --target check -j6   # RATCHET PASS byte 4506 linked 4506
cmake --build cmake-build-match --target drift -j6   # DRIFT: 0 field(s)
cmake --build cmake-build-match --target lint  -j6   # CLEAN
```

Exact LCS (not rounded by `verify.py`): **70.992366 %**, 523 normalised
original instructions vs 525 ours.  The prologue, loop header, low TBH and
the shared tails are already byte-identical; the residual is the high-TBH
body order.

## 2. If-chain with `__builtin_expect`: garbage weights are real but
##    unusable

A mechanically generated `if`/`else-if` chain over the same bodies (no
expects) folds back into the same 29-case switch and scores **71.0** at
1432 B.  Adding `__builtin_expect` to the chain conditions makes clang 7
fold the chain into a switch with `!prof` metadata whose weights are
uninitialised `SmallVector` contents (deterministic for a given source but
mostly zero).  The number of non-zero weights depends on the chain order and
on which blocks carry an expect.

Measured variants (official single-symbol verifier; sizes from the NDK
object):

| variant (chain order; expect map) | usize | switch cases | body order | pct |
|---|---:|---:|---|---:|
| plain if-chain, no expects | **1432** | 29 | value | **71.0** |
| original order; all expects = 1 | 1404 | 25 | scrambled | — |
| original order; A=1, B=0, low=0 | 1404 | 25 | `150 167/174 151 ...` | — |
| A,B,low; A=0, B=0, low=1 | 1456 | 25 | **exact A-then-B** | 15.9 |
| A,B,low; all expects = 0 | 1388 | 25 | **exact A-then-B** | 32.6 |
| A9,Blow,A; A=0, B=0, low=1 | **1432** | 25 | A9-then-B | 18.4 |
| A,B,low; A=1, B=0, low=0 | 1408 | 25 | A9-then-default | — |
| B,A,low; A=1, B=0, low=0 | 1400 | 25 | B-then-A | — |

The 25-case limit is the key: once the chain carries expects on the low
group (or on enough blocks), the fold stops after 25 cases and leaves
`14/170/184/189` in a default chain.  That changes the switch shape and the
register allocation.  The `A9,Blow,A; A=0,B=0,low=1` variant is the only
1432-B if-chain found with an A-before-B order, but its prologue is
`this = sl`, `m = r4`, `atStation = sl` (not the original `this = r6`,
`m = sl`, `atStation = r8`), so the exact score is only 18.4 %.

## 3. Single- and pair-expect sweeps over the original chain order

Adding one expect to the original-order chain:

| block | expect | usize | cases | order |
|---|---:|---:|---:|---|
| 167 or 0 | 0/1 | **1432** | no switch | value |
| 158 | 1 | 1436 | 29 | `167/174, default, A, B` |
| 174 | 1 | 1384 | 29 | `167/174, default, A, B` |
| 8 | 1 | 1372 | 29 | `167/174, default, A, B` |
| 13 | 0 | 1372 | 29 | `167/174, default, A, B` |
| 150 | 0 | 1360 | 29 | `167/174, default, A, B` |
| 189 | 1 | 1356 | 29 | `167/174, default, A, B` |
| 156 | 1 | 1356 | 29 | `167/174, default, A, B` |
| 172 | 1 | 1364 | 29 | `167/174, default, A, B` |

The 29-case single-expect variants all put the default (hole) group second,
never last, and their official scores are 24.2–44.7 %.  A pair sweep over
the two size-neutral blocks (167 and 0) combined with every other block
(both expect values, 104 variants) found **no pair with size 1432 and 29
cases**.

## 4. Targeted A/B expect maps (29 cases, expect-free low group)

Keeping the low group expect-free avoids the 25-case fold.  All of the
following are 29-case switches:

| map | usize | order | pct |
|---|---:|---|---:|
| A=1 | 1352 | `167/174, default, A, B` | 24.2 |
| A=0 | 1348 | `167/174, default, B, A` | 24.6 |
| B=1 | 1364 | `167/174, default, A(partial), B` | 35.6 |
| B=0 | 1360 | `167/174, default, A, B` | 38.8 |
| A=0,B=0 | 1356 | `167/174, default, A, B` | 31.0 |
| A=1,B=0 | 1356 | `167/174, default, A, B` | — |
| A=0,B=1 | 1348 | `167/174, 156, default, A, B` | 27.8 |

A random search over 200 sparse expect maps (expects only on A/B blocks,
low group expect-free) produced **200/200 29-case switches, and 0/200 with
the default group last**.  The default successor is always placed second,
even when its weight is 0, because it is the first successor in the jump
table worklist and wins the tie against the zero-weight cases.  This is why
the exact `167/174, A, B, default` order is not reachable from these maps.

## 5. Nested switches / mixed shapes

| shape | usize | cases | order | pct |
|---|---:|---:|---|---:|
| A+low in main switch, B in nested `switch` in `default` | **1432** | 29 | value (merged) | **71.0** |
| A+low in main switch, B in if-chain in `default` | 1392 | 25 | `167/174, default, A, B` | 38.5 |
| `switch (__builtin_expect(type, K))`, K in A/B | 1380–1456 | 29 | K first, then value | 18.3–27.5 |

The nested `switch` is merged back into the main switch by
`SimplifyCFG`/`CodeGenPrepare`, so it is byte-for-byte the plain switch and
scores 71.0.  The nested if-chain is not merged, but it changes the switch
shape (25 cases) and the register allocation, scoring 38.5 %.

## 6. Body-hint sweeps (no improvement)

- All 16 value combinations of the four landed hints (166, 184, 157, 163):
  the best is `(1,1,1,1)` (and `(1,0,1,1)`, the 184 hint is neutral) at
  **70.992366 %**.  Flipping any essential hint drops the score.
- Wrapping every individual `if` condition in the function (45 conditions)
  with `__builtin_expect(..., 1)`: no variant beats 70.992366 %; all
  neutral additions leave the score unchanged.
- This confirms wave 122's greedy result: the four body hints are already
  the maximum at 1432 B.

## 7. Root cause

The original is a PGO-like build whose high-TBH switch carries `!prof`
`branch_weights` with **A > B**.  `SelectionDAGBuilder::buildJumpTable`
adds the jump-table successors in ascending value order and
`MachineBlockPlacement::selectBestCandidateBlock` drains them by block
frequency; A > B makes the A bodies drain first, then B.  Clang 7's only
plain-C++ `!prof` producer on a switch is `__builtin_expect` on the switch
condition, which `LowerExpectIntrinsic::handleSwitchExpect` lowers to
exactly one weight of 2000 and all others 1 — it cannot express A > B.

The if-chain fold can synthesise A > B weights (via the uninitialised
`SmallVector` contents or the case-cluster probabilities), but every shape
that does so either:

- folds only 25 cases and leaves `14/170/184/189` in a default chain, or
- changes the switch shape / block order enough that the register
  allocation moves off the original `this=r6, i=fp, m=sl, atStation=r8`.

The plain switch (or the plain if-chain, which folds to the same switch)
keeps the original prologue, dispatch, low group and register allocation,
and is the best 1432-B shape at 71.0 %.

## 8. Reproduction

```sh
cmake --build cmake-build-match --target check -j6    # RATCHET PASS, byte 4506
cmake --build cmake-build-match --target drift -j6    # 0 fields
cmake --build cmake-build-match --target lint  -j6    # CLEAN

# exact (unrounded) LCS score
python3 - <<'PY'
# see /dev/shm/w7408/exactscore.py (same body as verify.py's score path,
# but prints the raw ratio instead of round(..., 1))
PY
```

Scratch (not committed): `/dev/shm/w7408/` — `genchain.py` (if-chain
generator), `gen_nested.py` / `gen_nested_if.py` (nested shapes),
`helpers.py` (mission-switch/weight/order decoder), `sweep_single.py`,
`search.py`, `randsearch.py`, `permsearch.py`, `pairsearch.py`,
`bodyhint.py`, `hintcomb.py`, `exactscore.py`, `vif.cpp` (plain if-chain),
`q_*`/`s_*`/`tg_*`/`o_*`/`pp_*` variant objects.

## 9. Files

- No source change.  `src/game/mission/Status.cpp` is unchanged.
- This note only.
