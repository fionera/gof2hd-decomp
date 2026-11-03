# `RocketGun::update(int)` — 86.9 %, 1908 B (w3811)

**Status:** research note, 2026-11-03. Worktree `wt/w3811`, branch `wave88-w3811`,
base `41804126`. No source change landed: every faithful source-shape variant tried
this pass scores <= 86.9 %. Tree clean, gate unchanged (byte 4498 / linked 4498,
size-exact 4524, `.text 0x1337c8`, lint `CLEAN ✓`, drift 0).

## TL;DR

* Target `_ZN9RocketGun6updateEi`: original **1908 B / 635 insns**, ours
  **1908 B / 636 insns**, official verify score **86.9**.
* Control flow, call order, constants, field offsets, callee set, literal-pool
  order and block layout already match. **Every remaining difference is
  register colouring** (plus the 16/32-bit encodings that follow from it).
* Single root cause: **`elapsed` lands in `r5` instead of `r4`**. The greedy
  allocator assigns `elapsed` to `r4` first, then the loop-preheader
  stack-address vregs (`%251 = t2ADDri %stack.4, 0` etc.) evict it. Everything
  else (`player` r4 vs r5, the loop `i*60` cursor r9 vs r4+r9 save/restore,
  the `Sinf` result spilling to `[sp,#8]`, the missing `strd r4, sl, [sp,#16]`)
  is downstream of that one swap.
* The eviction is **not** a source-shape accident of our reconstruction: it is
  forced by LLVM's fixed loop-branch heuristic (`LBH_TAKEN_WEIGHT = 124`,
  `LBH_NONTAKEN_WEIGHT = 4`, i.e. backedge probability 31/32). Any
  loop-invariant stack-address vreg that is hoisted to the preheader and used
  in the loop body therefore has a spill weight ~5e-3 .. 1e-2, while
  `elapsed`'s is 9.4e-4.

## 1. Baseline (confirmed in this worktree)

```
cmake --build cmake-build-match --target check -j8
verify: byte 4498  linked 4498  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓        drift: 0 field(s) across 0 class(es)
```

`python3 tools/verify/score_one.py _ZN9RocketGun6updateEi` (the `verify-fn`
target is broken by an unquoted `$ENV{FN}`; use the script directly):

```
orig 1908B ours 1908B  score=86.9  insns 635 vs 636
```

## 2. Exact spill weights (instrumented LLVM 7.0.0)

`VirtRegAuxInfo` was instrumented in
`llvm/lib/CodeGen/CalcSpillWeights.cpp` (`weightCalcHelper`) and `llc` rebuilt
incrementally. The final `calculateSpillWeightsAndHints` call for
`_ZN9RocketGun6updateEi` gives:

| vreg | value | total use-def freq | live size | spill weight |
| --- | --- | --- | --- | --- |
| `%29` | `elapsed` (int param) | **14.38** | **14894** | **9.400839e-04** |
| `%28` | `this` | 226.04 | 14878 | 1.479491e-02 |
| `%251` | `&stack.4` (ahead temp 1) | **51.38** | **5598** | **8.566814e-03** |
| `%252` | `&stack.3` (ahead temp 2) | 57.50 | 5582 | 9.612001e-03 |
| `%255` | `&stack.7` (Matrix frame) | 118.70 | 5550 | 1.007480e-02 |
| `%256` | `&stack.1` (`ahead`) | 44.11 | 5534 | 3.753831e-03 |

`elapsed`'s per-instruction contributions (slot, weight):
`16B 1.0` (def), `224B 1.0` (Gun::update arg), `7104B .195`, `7200B .195`,
`8928B .157`, `9200B 1.0`, `9232B 1.0` (volatile anchor), `14656B 9.69`
(loop trail-timer). Sum 14.38.

`%251`'s: `9312B 1.0` (preheader def), `9920B 31.0`, `10032B 31.0`,
`13280B 12.11`, `13392B 12.11`, `12400B 3.63`, `12512B 3.63`,
`12816B 3.63`, `12928B 3.63`. Sum 51.38.

So `elapsed` is ~9x lighter than the stack-address vregs.

## 3. Allocator mechanism (LLVM 7 `RAGreedy`)

Allocation order is `Prio = (1u << 29) + LI->getSize()` for global ranges
(`RegAllocGreedy.cpp::enqueue`), i.e. longest live range first:

1. `%29` elapsed (size 14894) -> `$r4`
2. `%28` this (14878) -> `$r5`
3. `%247` dt (5808) -> `$r6`
4. `%251` `&stack.4` (5598): `tryAssign` finds `$r8` free at cost 1, so
   `tryEvict(..., CostPerUseLimit=1)` runs. `shouldEvict` is
   `A.weight > B.weight`; r4 holds `%29` (9.4e-4 < 8.57e-3) so it is
   evictable, r5 holds `%28` (1.48e-2 > 8.57e-3) so it is not. `tryEvict`
   keeps the **last** evictable register in allocation order, so `BestPhys = r4`.
   `%29` is unassigned and requeued.
5. `%29` -> `$r8`; then `%386` (the `i++` vreg, weight 2.8e-2) evicts it again.
6. Second round: `%29` finally lands on `$r5`; `%39` (`player`, size 944,
   weight 3.0e-3) is allocated later and takes the now-free `$r4`.

The loop-body `i*60` Matrix cursor then takes `r9` in ours (r4 is taken by
`player` and later by `dt`), whereas the original keeps it in `r4` and uses
`r9` only as a save/restore copy across the cluster-rocket calls.

## 4. Why source shape alone cannot move `elapsed` back to r4

To keep `r4`, `elapsed` must not be evictable, i.e.
`14.38 / (14894+400) > 8.57e-3` -> total use-def freq > 127.6. That needs
~4 extra uses of `elapsed` in the loop body (each is worth ~31). The original
has exactly **one** loop use of `elapsed` (`ldr r2,[sp,#16]` in the trail-timer
block), so its weight is the same 9.4e-4.

The loop-body block frequency is fixed by LLVM's loop branch heuristic:

```
LBH_TAKEN_WEIGHT   = 124
LBH_NONTAKEN_WEIGHT = 4
=> backedge probability 124/128 = 31/32
=> loop body freq = 32x entry, fade block 31x, cluster block 12.1x ...
```

(`BranchProbabilityInfo::calcLoopBranchHeuristics`). This is independent of the
C++ source as long as the loop structure is unchanged. Consequently **any**
loop-invariant stack-address vreg that is hoisted to the preheader and used in
the loop body has weight >= ~5e-3 (even a single loop use gives 31/5598 =
5.5e-3), which is always > `elapsed`'s 9.4e-4.

The only way out is to make the address vregs **local** (not hoisted), so they
are enqueued with local priority after the global ranges. `-mllvm
-disable-machine-licm` does put `elapsed` in `r4`, but the whole-function
colouring collapses: **1884 B / 666 insns / score 48.9**. MachineLICM hoists
the addresses via the early `TII->isTriviallyReMaterializable(MI)` return in
`MachineLICMBase::IsProfitableToHoist`, so the register-pressure guard never
applies to `t2ADDri %stack.N, 0`.

## 5. Stack-slot layout (why the original gets `strd r4, sl, [sp,#16]`)

Frame object creation order (first created -> highest offset):

| | slots |
| --- | --- |
| original | guard 4, **s 8, c 12, elapsed 16**, this 20, dt 24 |
| ours | guard 4, c 8, elapsed 12, **trailMatrices 16**, this 20, dt 24 |

The original spills **both** `Sinf` (`s`) and `Cosf` (`c`) results, has **no**
`trailMatrices` slot, and therefore `elapsed`/`this` are adjacent and merge to
one `strd r4, sl, [sp,#16]`. Ours keeps `s` in `r6` and spills the
`Array<Matrix> *trailMatrices` local to `[sp,#16]`, so the two stores stay
separate and `elapsed` sits at `[sp,#12]`.

The original's loop reloads `this->trailMatrices` from `[sl,#216]` five times
(`0x15f1d8, 0x15f216, 0x15f32c, 0x15f370, 0x15f39a`) — i.e. the original source
uses `this->trailMatrices` directly and has **no local**. Removing our local is
the structurally correct move, but on its own it destroys the colouring
(`1880 B / 626 insns / 57.6 %`, `elapsed` spilled at entry, Matrix cursor on the
stack) because the allocator's second round picks different registers.

## 6. Variants measured this pass (all reverted)

Fast screen = clang `-S -emit-llvm` + self-built LLVM 7.0.0 `llc`
(`/dev/shm/w3811/quick.sh`, reproduces the real build's prologue and size);
confirmed with the full `link_so` + official `score()`.

| variant | size | score | elapsed |
| --- | --- | --- | --- |
| baseline | 1908 | **86.9** | r5 |
| no `life` local (inline `gun->lifetimes[i]` in Sinf) | 1908 | 86.5 | r5 |
| `life` declared after `phase` | 1908 | 86.5 | r5 |
| no `life` + no anchor | 1908 | 85.4 | r5 |
| no `life` + no anchor + no TM local | 1876 | 57.2 | r2 |
| `this->gun->` everywhere in loop | 1908 | 86.9 | r5 |
| no `gun = this->gun` reloads | 1908 | 86.9 | r5 |
| non-const `s`/`c` | 1908 | 86.9 | r5 |
| `const` TM local | 1908 | 86.9 | r5 |
| `(float) elapsed` inline in `ahead` (both sites) | 1908 | 86.9 | r5 |
| `(void) s;` after cluster block | 1908 | 86.9 | r5 |
| function-scope `s`,`c` | 1908 | 86.9 | r5 |
| `volatile float s` | 1912 | 76.1 | r5 |
| `volatile float s`,`c` | 1916 | 76.4 | r5 |
| TM local used only for 2nd null check | 1904 | 64.2 | sl |
| TM local used only for MatrixSetTranslation | 1904 | 64.2 | sl |
| TM local unused (all direct) | 1880 | 57.6 | (spilled) |
| TM local inside loop | 1888 | 59.7 | (spilled) |
| TM local as reference | 1904 | 64.2 | sl |
| no TM local + anchor in loop | 1812 | 47.5 | r2 |
| swap the two `+=` statements | 1908 | 56.6 | r5 |
| cross product before `Sinf`/`Cosf` | 1904 | 57.1 | r5 |
| anchor in loop | 1904 | 70.0 | r2 |
| anchor removed | 1908 | 85.8 | r5 |
| anchor before `dt` / after `g.update` | 1908 | 86.9 | r5 |
| `dt` inside loop | 1880 | -- | r2 |
| signed / `!=` / `i++` loop condition | 1908 | 86.9 | r5 |
| `count` local | 1916 | -- | r5 |
| `player` hoisted to function scope | 1908 | 86.9 | r5 |
| `player` live range extended past loop | 1908 | 86.9 | r5 |
| `ship` inline / as `Matrix *` | 1908 | 86.9 | r5 |
| `Vector ahead;` (no init) / declared late | 1908 | 86.9 | r5 |
| `Gun *g` / `this->gun->update` / member `gun` | 1908 | 86.9 | r5 |
| all 6 permutations of `dt`/anchor/TM declaration order | 1908 | 86.9 | r5 |
| `-mllvm -disable-machine-licm` (diagnostic) | 1884 | 48.9 | **r4** |

## 7. Remaining root cause / next step

**Root cause:** `elapsed` is evicted from `r4` by the loop-preheader
stack-address vregs (`%251 = t2ADDri %stack.4, 0`, `%252`, `%255`, `%256`)
because LLVM's fixed 31/32 loop-backedge heuristic makes those vregs ~9x
heavier than the whole-function `elapsed` live range. The original's colouring
therefore requires the address computations to stay **inside the loop body**
(local ranges, allocated after the global ranges), or a `%251` live range /
weight that this source cannot produce.

**Next step (not attempted):** find a source form whose stack temporaries are
not hoisted by MachineLICM without disabling LICM globally — e.g. an
address-dependent-on-loop-value shape (pointer induction variables for the
`ahead` temporaries) — and then re-check that the total instruction count stays
at 635. A whole-function colouring state (like `-stress-regalloc=12`, r4 but
44.7 %) is not a fix.
