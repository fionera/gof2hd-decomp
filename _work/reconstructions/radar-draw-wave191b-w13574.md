# Radar::draw wave 191b (w13574) — greedy-RA eviction quantified, no size-exact flip

**Status:** complete, 2026-11-13. Worktree `wt/w13574`, branch `wave191b-w13574`,
base `master` `58eac6932`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave191b-w13562.md` (RA root cause) and
`radar-draw-wave191b-w13544.md` (guard ABCDE). **No source change landed**:
every candidate is raw-neutral (6,013) or worse with size exact. `src/` untouched.

## 0. Baseline (verified this pass)

| metric | value |
|---|---:|
| raw same-address diff | 6,013 |
| instruction-index alignment | 1,965 |
| function size | 9,908 / 9,908 |
| instructions | 3,541 ours / 3,532 orig |

## 1. New evidence: it is an **eviction**, not a queue-order inversion

w13562 concluded the queue order was paradoxical (`%422` hinted yet spilled).
The missing piece is `RAGreedy::tryEvict` / `shouldEvict`
(`lib/CodeGen/RegAllocGreedy.cpp:843-857, 1094-1161`): a range that cannot
`tryAssign` **evicts the cheapest interference** by spill weight
(`A.weight > B.weight`).  So the sequence is:

1. `%422` (campaign) is hinted and dequeued first; it gets `sl` (r10).
2. `%2487` (zero/mbp) is dequeued later, finds every CSR taken, and evicts the
   *cheapest* occupant — campaign — because campaign's spill weight is ~39x
   smaller.

Spill weights recomputed from the pre-RA MIR (`mim.txt:395122`, branch
probabilities propagated to block frequencies, `getSpillWeight = (isDef+isUse) *
freq/entryFreq`):

| vreg | what | weight | dominant term |
|---|---|---:|---|
| `%380` | `this` | 3.218 | entry block |
| `%382` | `hud` | 1.000 | entry block |
| `%383` | `mode` | 1.000 | entry block |
| `%381` | `player` | 1.000 | entry block |
| `%2487` | zero + `missionBlocksPlanets` | **0.990** | bb.1 (0.3125) ×3 |
| `%420` | `alienOrbit` | 0.051 | bb.12 |
| `%422` | `campaign` | **0.02551** | bb.12 |
| `%416` | `turretMode` | 0.02551 | bb.12 |
| `%2494` | EH landing-pad `COPY $r0` | 0.0 | cold |

`%2487`'s weight is dominated by the three entry-block instructions
(`t2MOVi 0` @240 + the two `t2STRBi12` zero stores @272/288, block freq
0.3125 each).  Campaign has exactly one def (`COPY $r0` @2128) and one use
(`t2CMPri %422,2` @24448) and therefore cannot survive the eviction.

`%416`'s weight (0.02550917) is *marginally* below campaign's (0.02551238), so
if `%416` were the occupant of the register `%2487` scans first, `%2487` would
evict `%416` instead and campaign would keep `sl`.  That is the only
sub-0.1 % weight gap in the function and is the most plausible lever, but no
source spelling tried reaches it (see §3).

## 2. `%2487`'s live range cannot be shortened from the source

The two zero stores and `missionBlocksPlanets = false` are CSE'd into one
rematerialisable vreg (`%2487:rgpr`, 7 defs / 5 uses, span 240..36,640).
Splitting the vreg does **not** flip the allocation: a probe that makes
`missionBlocksPlanets` a distinct value (`int missionBlocksPlanets = mode;`)
still leaves campaign spilled (raw 8,869, size 9,924).  So the blocker is the
`missionBlocksPlanets` half of the vreg, not the shared zero.

## 3. Cells measured this pass (all reverted; base 6,013 / 9,908)

### 3.1 Campaign declaration position (declaration moved, original removed)

| cell | raw | size |
|---|---:|---:|
| base (turret, alien, campaign) | **6,013** | 9,908 |
| turret, campaign, alien | 6,020 | 9,908 |
| alien, turret, campaign | 6,034 | 9,908 |
| alien, campaign, turret | 6,039 | 9,908 |
| campaign, turret, alien | 8,559 | 9,900 |
| campaign, alien, turret | 6,039 | 9,908 |
| after `planetTargets` | 6,090 | 9,908 |
| after `enemies` | 9,180 | 9,904 |
| after `SetColor` / before `mission` | 8,611 | 9,900 |
| after `missionBlocksPlanets` | 9,180 | 9,900 |
| before `turretMode` | 8,559 | 9,900 |

### 3.2 `missionBlocksPlanets` spelling

| cell | raw | size |
|---|---:|---:|
| `unsigned` / `char` / `uint8_t` / `unsigned char` | 6,013 | 9,908 |
| `(type == 172) ? 0 : 1` / `!(type == 172)` / `? 1 : 0` | 6,013 | 9,908 |
| `bool` (with `false`) | 6,172 | 9,912 |
| `int x; if (...) x=...; else x=false;` | 6,013 | 9,908 |
| declared after `alwaysEnemySeen`/`alwaysEnemy` | 6,013 | 9,908 |
| before the zero stores | 6,013 | 9,908 |
| `= mode` (distinct vreg probe) | 8,869 | 9,924 |

### 3.3 Zero stores

| cell | raw | size |
|---|---:|---:|
| `false` / `0u` / `uint8_t zz=0` temp | 6,013 | 9,908 |
| swapped order | 6,015 | 9,908 |

### 3.4 Campaign type

| cell | raw | size |
|---|---:|---:|
| `const int` | 6,013 | 9,908 |
| `unsigned int` | 6,014 | 9,908 |
| `short` / `char` | 8,491 / 8,446 | 9,912 |
| `volatile int` | 9,208 | 9,892 |

### 3.5 `__builtin_expect` added to un-hinted branches

Block frequencies feed spill weights, so adding/removing a hint *does* move the
RA (it is the only lever that reliably does).  None lands on the original:

| cell | raw | size |
|---|---:|---:|
| `expect(!enabled, 0)` | 6,047 | 9,908 |
| `expect(!enabled, 1)` | 10,178 | 10,472 |
| `expect(route != nullptr, 1)` | 6,013 | 9,908 |
| `expect(route != nullptr, 0)` | 9,188 | 9,924 |
| `expect(current == 143, 0)` | 6,015 | 9,908 |
| `expect(current == 143, 1)` | 6,235 | 9,896 |
| `expect(gasClouds != nullptr, 1)` | 7,799 | 9,900 |
| `expect(scannerAvailable && enemies, 1)` | 9,195 | 9,956 |
| `expect(asteroids ... , 1)` | 9,583 | 10,080 |
| `expect(campaign >= 2, 1)` | 8,644 | 9,920 |
| `expect(lockedAsteroid == nullptr ..., 1)` | 8,723 | 9,900 |
| no-expect on the mission guard | 6,014 | 9,908 |
| `expect(!mission->isEmpty(), 0)` | 9,556 | 9,888 |

### 3.6 Random permutation search

Random shuffles of the six `this->X = getY()` assignments × the three
`turretMode`/`alienOrbit`/`campaign` declarations (40 distinct permutations):
best 6,023, all size exact.  Adjacent-swap space was already covered by
w13562/w13544.

## 4. Scheduling note: the `onScreen` check is probably an RA consequence

In the original the planet-loop body is
`update(); if (!onScreen) continue; stationId = stations->data()[i]; ...`
(0x12e3ee-0x12e3fa), while ours evaluates `stationId`/`currentStation` first.
Moving the check earlier in the source is size-exact but **raw 6,014**
(aligned 1,961): it removes the 0x12e390-0x12e3cd differences but shifts the
loop preamble by 2 bytes because our `stations` is a stack reload
(`ldr r0,[sp,#132]`) where the original's is a register (`ldr r0,[sl,#4]`).
The reordering is therefore most likely a *scheduler consequence* of
`stations` living in `sl` in the original, not a source difference.  It also
makes the asteroid candidate-change move size-fatal (9,912), so it was not
landed.

## 5. Flag harness fix (important for future passes)

`-mllvm` sweeps must link **without** re-running ninja: ninja recompiles
`Radar.cpp.obj` (its `.ninja_deps` entry is stale after a hand-compile) and
silently discards the flags.  Compile with `cmd.txt`, then run the recorded
link line directly (`ninja -t commands link_so | tail -1`).  With that harness:

| flag | raw | size |
|---|---:|---:|
| `-split-spill-mode=size` | 9,415 | 9,892 |
| `-enable-deferred-spilling` | 8,664 | 9,916 |
| `-regalloc=basic` | 9,909 | 10,188 |
| `-regalloc=pbqp` | 9,834 | 10,116 |
| `-regalloc=fast` | 13,685 | 13,940 |
| `-regalloc-csr-first-time-cost=1/1000` | 6,013 | 9,908 |
| `-lcr-max-depth=0`, `-lcr-max-interf=0` | 6,013 | 9,908 |
| `-enable-local-reassign`, `-disable-spill-fusing` | 6,013 | 9,908 |

No flag reproduces the original's colouring.

## 6. Conclusion

The residual is one greedy-RA eviction: `%2487` (weight 0.99) evicts `%422`
(weight 0.0255) from `sl`; the original instead has campaign in `sl` and
`%2487` in `fp`.  Reaching it from the source requires either
(a) dropping `%2487`'s weight below campaign's — impossible while the two
entry-block zero stores share its vreg (they alone contribute 0.94), or
(b) making `%416` (weight 0.02550917) the cheapest interference instead of
campaign (weight 0.02551238) — a 0.0000032 gap no source spelling tried can
cross.  Everything downstream (frame slots, `.ARM.extab`, the ~6,000-byte
tail) follows from this single decision.

## 7. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6013 aligned= 1965 size=9908/9908 insn=3541/3532
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13574/`: `Radar.base.cpp`,
`batch.py` (cell sweep), `flagtry2.py` + `linkcmd.txt` (working `-mllvm`
harness), `weights.py` (spill-weight recomputation from `mim.txt`),
`bytediff.py`, `ndiff.py`, `calldiff.py`, `sbs2.py`, `randsearch.py`.
