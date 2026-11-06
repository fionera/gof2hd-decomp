# `Status::missionCompleted` — wave 124 (w7407): the original layout is the
# source order; `!prof` weights 200:199:150:1 reproduce the A-run at 1432 B
# (71.0 -> 78.3 %) but no C++ construct emits them

Status: **note-only** (no source change landed).  Worktree `wt/w7407`, base
master `6f8ee2320` (wave-123 final snapshot).  Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(sha256 `d4c17be6...bed9483`, armeabi-v7a, NDK r18b clang 7.0.2 r328903,
`-Oz`, image base 0x10000).

Target: `_ZN6Status16missionCompletedEbbx` (`Status::missionCompleted(bool,
bool, long long)`), original `0xabf38`, size **1432 B**.

| | baseline (HEAD `6f8ee2320`) |
|---|---:|
| function score | **71.0 %** |
| function size | 1432 B |
| gate | byte 4506 / linked 4506 |
| lint / drift | CLEAN / 0 |

## 0. Summary

Three new results this pass:

1. The original's **whole** body layout is exactly the *source order* of the
   case labels — not just the high-group A/B split.  Decoding both TBH tables
   of the original gives `167/174, 8, 11, 13, 14, 150 151 152 153 154 155 158
   162 166 170 184 189, 156 157 160 163 164 165 168 169 171 172, default`,
   which is precisely the order the case blocks are written in
   `src/game/mission/Status.cpp`.
2. Patching the switch's `!prof branch_weights` in `base.ll` with
   `default 1, A 200, B 199, low 150` reproduces the **high-group A-run at
   exactly 1432 B** and raises the official score from **71.0 % to 78.3 %**.
   This proves the residual is a pure branch-weight / block-frequency effect.
3. The low group (`8, 11, 13, 14`) cannot be pulled in front of the high
   group at 1432 B by any weight ratio (grid search, §3), so even the best
   weights only match 24 of 28 body groups.  And no C++11 construct emits
   multi-case switch weights: `__builtin_expect` sets exactly one weight to
   2000 (nested expects fold to one), and the if/else-if fold's
   uninitialised weights are 0 after the first 7 successors (§4).

## 1. Baseline evidence (measured this pass)

```sh
cmake --build cmake-build-match --target link_so -j6
python3 score1.py        # pct 71.0  osize 1432  usize 1432
cmake --build cmake-build-match --target check -j6
# verify: byte 4506  linked 4506  stubs 0  missing 0  extra 0  avg 99.94
# data:   exact 843 of 843 exported objects
# sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
# RATCHET PASS
```

The prologue, the two TBH dispatches (`sub.w r1,r0,#150; cmp r1,#39; bhi;
tbh` and `sub.w r1,r0,#8; cmp r1,#6; bhi; tbh`), the register allocation
(`this`=r6 spilled to `[sp,#16]`, `i`=fp, `m`=sl, `atStation`=r8) and the
whole shared-tail region from `0xac45a` to the end are already byte-identical
(the only tail differences are the three PC-relative PIC literal-pool deltas
at `0xac4c4/0xac4c8/0xac4cc`, which `verify.py` masks).  The instruction
multiset of the base and the original matches apart from branch *encodings*
(`b.n` vs `beq.n`, wide vs narrow) and the jump-table words; only the block
order differs.

## 2. The original layout is the source order (both TBH tables decoded)

`dec2.py`/`dec_low.py` decode the two TBH tables of a linked `.so`:

```
original high TBH (0xabfe6, 40 entries), sorted by target address:
  0xac036 [167,174]   0xac110 [150]  0xac116 [151]  0xac11c [152]
  0xac150 [153]       0xac156 [154]  0xac16e [155]  0xac180 [158]
  0xac1dc [162]       0xac242 [166]  0xac2a6 [170]  0xac2b4 [184]
  0xac2ee [189]       0xac32a [156]  0xac354 [157]  0xac388 [160]
  0xac3b6 [163]       0xac3d2 [164]  0xac3d6 [165]  0xac3de [168]
  0xac3fc [169]       0xac400 [171]  0xac41a [172]  0xac45a [default]
original low TBH (0xac052, 7 entries):
  0xac060 [8]  0xac0c4 [11]  0xac0ce [13]  0xac0dc [14]  0xac45a [9,10,12]
```

Concatenated, that is exactly the source order
`167,174, 8, 0/11, 13, 14, 150..189, 156..172`.  Our base instead emits
value order with the low bodies interleaved
(`167/174, 8, 150..155, 156, 11, 157, 13, 158, 14, 160, ...`, low bodies at
`0xac068/0xac18c/0xac196/0xac1a4`).

## 3. `!prof` weights reproduce the A-run at 1432 B — 78.3 %

`base.ll` is `clang++ -S -emit-llvm` of the current `Status.cpp`
(the switch is `switch i32 %52`, 29 cases, successor 0 = default,
successors 1..29 = the case list in source order).  Attaching

```llvm
  ], !dbg !9298, !prof !999999
!999999 = !{!"branch_weights", i32 <default>, i32 <167>, i32 <174>, ... }
```

and rebuilding the TU with the project's exact clang 7.0.2 flags gives
(measured, official single-symbol score):

| weights (default / A / B / low) | size | high-group order | score |
|---|---:|---|---:|
| none (base) | 1432 | value order | 71.0 |
| 1 / 2 / 1 / 1 | 1376 | A then B | — (regalloc rotates) |
| 1 / 51 / 50 / 1 | 1360 | A then B | — |
| 1 / 51 / 50 / 50 | 1436 | A then B | — |
| **1 / 200 / 199 / 150** | **1432** | **A then B** | **78.3** |

The winning vector is
`default 1, 167 200, 174 200, {150,151,152,153,154,155,158,162,166,170,184,189} 200,
{156,157,160,163,164,165,168,169,171,172} 199, {8,0,11,13,14} 150`.
It reproduces the high-group A-run body for body at the original's size and
raises the score 71.0 -> 78.3 (the remaining 21.7 % is the low group, which
this weight set places at `0xac062, 0xac402, 0xac41e, 0xac42a` instead of
`0xac060, 0xac0c4, 0xac0ce, 0xac0dc`).

The ratio is what matters, not the magnitude: every `wa:wb:wl` of the form
`1000:999:700` (and `200:199:150`, `300:299:200`, `400:399:300`,
`500:499:400`, `700:699:500`) gives the same 1432-byte A-run layout.
`A=2,B=1` is too lopsided and rotates the register allocation
(`this`=fp, `i`=sl, `m`=r9) and shrinks the function.

**Low group is not reachable at 1432 B.**  A grid over
`wa ∈ {100..1000}`, `wb ∈ {wa-1,wa-2,wa-5,wa-10,wa/2}`,
`wl ∈ {1,50,100,150,200,300,400,500,700,1000,1500,2000}`, `wd ∈ {1,10,50}`
found only `wl ≈ 0.7·wa` at size 1432, and all of those give LCS 24/28
against the original body sequence (the four low bodies are misplaced).
With `wl > wa` no 1432-byte solution exists at all.
`-disable-block-placement` does not reproduce the source order either
(the pre-MBP MBB order is `153, 150, 155, 156, 181, 174, 151/152/154, ...`),
so the original's layout is not the raw MBB order.

## 4. Why the weights cannot be written in C++11

- `switch (__builtin_expect(t, K))` is the only construct that puts `!prof`
  on a switch.  `LowerExpectIntrinsic::handleSwitchExpect` sets **one**
  weight to 2000 and all others to 1 (`Weights[n+1] = 1`), so it cannot
  express A > B.  Nested expects (`__builtin_expect(__builtin_expect(t,150),151)`)
  fold to a single `llvm.expect`; the emitted metadata still has exactly one
  2000 (§ verified by dumping the .ll).
- `__builtin_expect_with_probability` does not exist in clang 7.0.2.
- An `if (__builtin_expect(t==C1,1)) ... else if (__builtin_expect(t==C2,1)) ...`
  chain *is* folded to a `SwitchInst` and *does* get `!prof`, but the weight
  vector is uninitialised `SmallVector` memory: for the real 29-case chain it
  is `0, -689350850, -689350850, -691152757, 600635, 600635, 600335, 300, 0, 0, ...`
  (only the first 7 successors are non-zero, the rest are 0).  Measured
  sizes 1456-1460 B and scores 7.8-15.8 %, so it is not usable.
- `__builtin_expect` on the *body* conditions (the wave-122 hints) changes
  in-body branch probabilities but not the switch edge probabilities, so the
  layout stays value order.

## 5. Transformed switch values (the assignment's suggestion)

`switch (m->getType() - 150)` with shifted labels (`0..39`) is **byte-identical**
to `switch (m->getType())`: clang emits the same `sub.w r1,r0,#150; cmp
r1,#39; bhi; tbh`, and the successor order is still ascending *value*, so the
order is unchanged (measured 1432 B / 71.0 %, exactly the base).

No other bijection can keep the `sub #150; cmp #39; tbh` dispatch: the
dispatch subtracts the minimum case value, so the switch expression must be
`type - 150` (monotone).  A non-monotone bijection such as `type ^ 1` or
`189 - type` needs an extra `eor`/`rsb` before the range check, which changes
the size, and the resulting order is adjacent-pair-swapped or fully reversed,
not A-then-B.  A rank-based remap (wave-121) needs a `.rodata` table, which
shifts the section and collapses `byte_exact` (4506 -> 4426/4470).

## 6. Hole-fill (case-cluster probability) sweeps re-measured

Filling the 16 unused values `159 161 173 175-183 185-188` with extra `case`
labels on the A bodies synthesises A > B through `sortAndRangeify`'s
`Prob` sum.  Measured sizes (all with the wave-122 body hints already in
`base.cpp`):

| A bodies filled (rest of holes to 167/174) | size | note |
|---|---:|---|
| 0 | 1424 | value order after 167/174 |
| 1-6 | 1412 | partial reorder |
| 7 | 1436 | first 7 A bodies first, regalloc still r6/fp/sl, score 47.7 |
| 8 | 1368 | regalloc rotates |
| 9 | 1364 | |
| 10 | 1368 | |
| 11 (all A bodies) | **1376** | **exact original order**, regalloc rotates, 43.8 |
| 11 + 5 to 167/174 | 1376 | exact original order, 43.8 |
| 11 + 5 unassigned | 1376 | 150 first, then A, then B |

A 400-sample randomised search over high-hole and low-hole assignments found
**no** 1432-byte assignment with a better body order than the base
(best at 1432 is the empty assignment, LCS 18/28).  Low-group hole fills
(`9,10,12` onto `8,13,14`) give 1436/1444/1448, never 1432.

## 7. Other shapes re-measured this pass

| variant | size | pct |
|---|---:|---:|
| base (plain switch, wave-122 hints) | 1432 | **71.0** |
| `switch (type - 150)` | 1432 | 71.0 |
| `default:` first / last | 1432 | 71.0 |
| source order reversed (B then A) | 1468 | 42.8 |
| source order A-then-B, low group last | 1380 | — |
| `switch (__builtin_expect(t,K))` K=150 | 1404 | 22.6 |
| K=167 | 1396 | 27.0 |
| K=8 | 1384 | 18.7 |
| K=0 | 1428 | 25.9 |
| K=14 | 1432 | 16.0 |
| K=189 | 1420 | 26.7 |
| K=172 | 1424 | 26.8 |
| K=156 | 1388 | 20.2 |
| if/else-if chain + expects | 1460 | 15.8 |
| if/else-if chain, A first | 1456 | 7.8 |

## 8. Root cause

`SelectionDAGBuilder::buildJumpTable` adds the jump-table successors in
ascending case-value order and `MachineBlockPlacement::selectBestCandidateBlock`
drains the worklist by `MBFI->getBlockFreq`, keeping the first on ties.
With uniform switch weights the bodies drain in value order; the original's
A-then-B (indeed, source-order) layout is what you get when the A bodies are
*very slightly* hotter than the B bodies and the low bodies are ~0.7x as hot.
The original's dispatch, prologue, register allocation and shared tails are
byte-identical to ours, so the original's IR differs only in the switch's
branch weights — i.e. the shipped `.so` was almost certainly built with
profile data, and the profile is not recoverable from source.

## 9. Reproduction

```sh
# baseline
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w7407/score1.py          # pct 71.0, 1432/1432
cmake --build cmake-build-match --target check -j6   # RATCHET PASS, byte 4506

# the 78.3 % proof (scratch, /dev/shm/w7407):
#   base.ll = clang++ -S -emit-llvm of src/game/mission/Status.cpp
#   attach "], !dbg !9298, !prof !999999" to `switch i32 %52` and append
#   !999999 = !{!"branch_weights", i32 1, i32 200, i32 200, i32 1, i32 1,
#     i32 1, i32 1, i32 1, i32 200, ... i32 199, ... i32 150, ...}
#   (default 1, 167/174 200, A 200, B 199, low 150)
#   sed -E 's/, variables: ![0-9]+//' p.ll > p2.ll
#   clang++ <project flags> -c p2.ll -o Status.cpp.obj
#   /tmp/link.sh            # the exact gold+strip+reorder command from build.ninja
#   python3 score1.py       # pct 78.3, osize 1432, usize 1432
```

Scratch (not committed): `/dev/shm/w7407/` — `base.cpp` (HEAD snapshot),
`cc.sh`, `try.sh`, `batch.sh`, `score1.py` (official single-symbol verifier),
`dec2.py`/`dec_low.py` (TBH decoders), `quick.py`/`pro.py` (size + order +
prologue from a `.o`), `gen3.py` (case-block/hole generator), `wsweep.py` /
`lsearch*.py` (the `!prof` weight search), `genexp.py` (switch-expect probes),
`genchain2.py`/`genchain3.py` (if/else-chain probes).
