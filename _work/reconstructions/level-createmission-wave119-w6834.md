# w6834: `Level::createMission` (92.293729 -> 92.293729 %, 17172 B) — wave-119
# convoy k-chain permutation / store-order / default-position sweep; no source win

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w6834`, branch `wave119-w6834`,
based on current master. Ground truth:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; function `0xaffd8`-`0xb42ec`, 17172 B).
Continues `level-createmission-wave119-w6814.md` (and the four notes before it).
`src/game/world/Level.cpp` is unchanged; this note is the only commit.

## Baseline (pristine HEAD, re-measured this wave)

```text
_ZN5Level13createMissionEv   official 92.293729 %   size 17172/17172 B
                             6055 orig / 6065 ours insns
gate: byte 4505/4524, linked 4505/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, RATCHET PASS
ELF: 27 sections, file 2,187,328 B, .ARM.extab 0x26290 (8 B short, createShip only).
```

## 0. The only size divergence before the EH tail

The reconciled streams are address-aligned up to the case-183 convoy switch
(`0x1434`).  The original k-chain is 68 B (`0x1434`-`0x1478`); ours is 70 B
(`0x1434`-`0x147a`), so every PC-relative pool offset and branch target after it
is shifted until the two streams re-sync (they do again at `0x1a14` and `0x2ada`).

Original layout (`0x1434`..`0x1476`):

```text
cmp r1,#2 ; beq body2 ; cmp r1,#1 ; beq body1 ; cbnz default ; body0 (fallthrough)
body0 = 0x1448  body1 = 0x1454  body2 = 0x1460  default = 0x146c
shared tail = 0x1476  (default's last `str [sp,#380]`; default falls through into it)
```

Ours (baseline, descending source `if (k==2) … else if (k==1) … else if (k==0) …`):

```text
cmp r1,#0 ; beq body0 ; cmp r1,#1 ; beq body1 ; cmp r1,#2 ; bne default ; body2 (fallthrough)
body2 = 0x144a  body0 = 0x1456  body1 = 0x1462  default = 0x1470
```

The branch-order difference is a `SelectionDAGBuilder::lowerWorkItem` sort:
`llvm::sort` orders equal-probability clusters by ascending case value, so with
no `!prof` metadata the chain is always `[0,1,2]`.  The original's `[2,1,0]`
therefore needs either non-equal probabilities or an unfolded if-chain.

## 1. Source-order permutations of the four bodies

All six permutations of `if (k==A) … else if (k==B) … else if (k==C) … else D`
(semantics unchanged) were measured.  Only the two orders that start with
`k==2` reproduce the baseline; the ascending orders do produce the original
`cmp#2; beq; cmp#1; beq; cbnz` test sequence, but the *body layout* comes out
`[0][2][1]` (not `[0][1][2]`) and the shared tail becomes body1's store, adding
one `b.n`:

| variant (chain order) | official % | size | insns |
|---|---:|---:|---:|
| **baseline `2,1,0`** | **92.293729** | 17172 | 6065 |
| `2,0,1` | 92.293729 | 17172 | 6065 |
| `0,1,2` | 92.145215 | 17172 | 6065 |
| `0,2,1` | 92.145215 | 17172 | 6065 |
| `1,0,2` | 92.161716 | 17172 | 6065 |
| `1,2,0` | 92.161716 | 17172 | 6065 |

A minimal reproducer (`k4.cpp`, same NDK flags) confirms the mechanism:
source order `0,1,2` compiles to `cmp#2; beq; cmp#1; beq; cmp#0; …` with body
layout `[0][2][1]`; source order `2,1,0` compiles to `cbz; cmp#1; cmp#2`.

## 2. `switch` formulations (default position matters)

`switch (k)` with cases 0,1,2 and `default` written **last** gives the ascending
if-chain result (92.145215).  Writing `default` **first** restores the baseline
layout (92.293729) regardless of the case order in the source text, because
`SwitchInst` keeps cases sorted by value and the block order only depends on the
IR block order of `default`:

| variant | official % | size | insns |
|---|---:|---:|---:|
| **`switch`, default first (any case order)** | **92.293729** | 17172 | 6065 |
| `switch`, default last, case order `2,0,1` / `2,1,0` | 92.293729 | 17172 | 6065 |
| `switch`, default last, case order `1,0,2` / `1,2,0` | 92.161716 | 17172 | 6065 |
| `switch`, default last, case order `0,1,2` / `0,2,1` | 92.145215 | 17172 | 6065 |
| nested ascending `if/else` | 92.145215 | 17172 | 6065 |

The six `switch` case orders with `default` last score exactly like the six
`if`-chain orders — the case order in the source text does not survive to the
block layout (the case list is value-sorted and the layout is driven by the
lowered comparison chain).

## 3. Store order inside the bodies / default body

The original emits `offset[1], offset[0], offset[2]` in every body (the
`offset[2]` store is the shared tail).  Changing the *source* store order is
load-bearing (the DAG scheduler keeps the third store last):

| variant | official % | size | insns |
|---|---:|---:|---:|
| **baseline body order `0,1,2`, default `1,2,0`** | **92.293729** | 17172 | 6065 |
| bodies `1,0,2` (all) | 88.606760 | 17172 | 6075 |
| bodies `1,0,2` + ascending chain | 88.590272 | 17172 | 6075 |
| default `1,0,2` | 89.653465 | 17172 | 6065 |
| default `0,1,2` | 88.606760 | 17172 | 6075 |
| default `2,0,1` | 92.277228 | 17172 | 6065 |
| default `2,1,0` | 92.277228 | 17172 | 6065 |
| default `0,2,1` | 91.774606 | 17172 | 6066 |

The literal-pool order confirms this: the original pool at `0x15e4` is
`-3000, 20000, -50000, -7000, 10000, -30000, 25000, -36000, 6000, 12000, -45000`
(ascending body order); ours starts `25000, -36000, -3000, …` (ours' body layout
order) and carries the two extra words `6000, 12000` into the earlier pool.

## 4. `__builtin_expect` on the convoy chain — confirmed dead end

Any `!prof` metadata on the k-chain collapses the *whole function* to
6058 insns / 62.8 %, independent of the weight values:

| variant | official % | size | insns |
|---|---:|---:|---:|
| ascending chain, `expect(...,1)` on all three | 62.841575 | 17172 | 6058 |
| ascending chain, weights 2/2/2, 1/2/3, 3/2/1, 2/3/4 | 62.841575 | 17172 | 6058 |
| descending chain, same four weight sets | 62.808553 | 17172 | 6058 |

So the original cannot have had `__builtin_expect` here (its global block
placement matches the no-metadata build).

## 5. Other new variants measured this wave (all ≤ baseline)

| variant | official % | size | insns |
|---|---:|---:|---:|
| **baseline** | **92.293729** | 17172 | 6065 |
| `if ((i == X) & alienWave)` (branchless `&&`) | 75.689058 | 17172 | 6063 |
| `if (alienWave & (i == X))` | 75.689058 | 17172 | 6063 |
| `if (alienWave && i == X)` (operand swap) | 92.293729 | 17172 | 6065 |
| turret select `(bigAlienWave && i >= 0)` / `!=0` / swapped / if-else | 92.128713 | 17172 | 6065 |
| turret select `(bigAlienWave \| (i < 0))` | 92.244224 | 17172 | 6065 |
| turret select `(bigAlienWave \|\| i < 0)` / `(i<0 \|\| …)` / `!=0 \|\|` | 92.128713 | 17172 | 6065 |
| turret select `((bigAlienWave != 0) \| (i < 0))` | 92.244224 | 17172 | 6065 |
| turret `t = outpost; if (bigAlienWave + (i<0)) t = station;` | 92.293729 | 17172 | 6065 |
| `Vector v;` instead of `Vector v = {0,0,0};` (turret block) | 92.293729 | 17172 | 6065 |
| turret `i += 1, k += 1` / `Matrix rotation = Matrix()` | 92.293729 | 17172 | 6065 |
| `bool raid = false;` at top of function / `bool raid;` at top | 91.452145 | 17172 | 6065 |
| `volatile bool raid;` | 82.483077 | 17172 | 6059 |
| `bool *raidAddr = &raid; (void) raidAddr;` | 91.947195 | 17172 | 6065 |
| convoy-loop `setRotationSpeed` before `setSpeed` | 92.227723 | 17172 | 6065 |
| convoy-loop `cargo` after `crateCaptured` | 92.293729 | 17172 | 6065 |
| convoy-loop hitpoints before speed/rotation | 91.657728 | 17172 | 6064 |
| convoy-loop without the `enemies` alias | 89.976075 | 17172 | 6066 |
| `*bigShipsCount >= 1` / `0 < *bigShipsCount` | 92.293729 | 17172 | 6065 |
| `int bigShips = *bigShipsCount; if (bigShips > 0 …)` | 92.293729 | 17172 | 6065 |
| `(bool) *bigShipsCount` | 89.666557 | 17172 | 6061 |
| `*bigShipsCount != 0 && *bigShipsCount > 0` | 83.677174 | 17172 | 6063 |
| `Vector offset;` / `Vector offset = {0,0,0};` (case 183) | 88.636176 | 17172 | 6080 |
| `float offset[3] = {0,0,0};` | 89.158416 | 17172 | 6065 |
| `for (int k = 0; k < 4; ++k)` / `k <= 3` / `int i = k + 10` / `unsigned k` | 92.293729 | 17172 | 6065 |
| ternary-chain `offset[i] = k==2 ? … : …` (desc/asc) | 89.281003 / 89.066623 | 17172 | 6073 |
| `switch` with explicit `case 3` and empty default | 92.218830 | 17172 | 6064 |
| `switch` without default + separate `if (k == 3)` | 89.129179 | 17172 | 6060 |
| default body tested first (`if (k==3) … else if (k==0) …`) | 91.312598 | 17172 | 6066 |

## 6. Conclusion

HEAD remains a strict local optimum at **92.293729 %** (17172 B, 6055/6065
insns).  ~60 new variants were measured this wave on top of the ~260 from the
preceding wave-119 notes; none beat the baseline.

The convoy switch is now fully characterised:

1. The original's test order `[2,1,0]` requires `P(2) > P(1) > P(0)` in
   `lowerWorkItem`, which only `!prof` metadata can supply — but any metadata on
   this chain re-places the whole function (62.8 %).
2. The original's body layout `[0][1][2][default]` with the default's last store
   as the shared tail is not reachable from any `if`/`switch` source order:
   ascending sources give `[0][2][1]` and an extra `b.n` (2 B, 0.15 pp).
3. An unfolded if-chain (source order `2,1,0`) keeps the baseline layout but
   gives test order `[0,1,2]`.

The remaining 7.7 % is unchanged from `level-createmission-wave119-w6814.md`
§7: greedy register allocation, the guard/`raid` stack-slot colour order
(17 insns), and the branch-vs-cmov / branchless-`&&` if-conversion decisions.

## 7. Commands / scratch

```text
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w6834/dump.py          # official %, size, insns, diff.txt
python3 /dev/shm/w6834/try.py vN.json   # apply/build/score/restore a batch
python3 /dev/shm/w6834/mkasm2.py        # orig_addr.txt / ours_addr.txt (with addresses)
python3 /dev/shm/w6834/delta.py         # content-aligned address-delta census
```

Scratch under `/dev/shm/w6834` (`Level.base.cpp` = pristine HEAD copy,
`v1.json`…`v31.json` = variant batches, `k1..k4.cpp` = standalone k-chain
reproducers).
