# Radar::draw wave 190 (w13474) — statement-order / literal-pool / regalloc sweep
# no size-exact raw win found (base stays raw 6,065 / aligned 2,043 / 9,908 B)

**Status:** complete, 2026-11-12, no source change landed. Worktree `wt/w13474`,
branch `wave190-w13474`, base `master` `db07789dd`. Target
`src/game/weapons/Radar.cpp`, `_ZN5Radar4drawEP6PlayerP3Hudi`
(`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave190-w13456-pass3.md` (the 6,155 base),
`radar-draw-wave190-w13465.md` (gas-cloud hoist + `currentStation` hoist,
6,218 -> 6,176), `radar-draw-wave190-w13433.md` and
`radar-draw-campaign-sl-wave167-w11206.md` (the `campaign`-in-`sl` /
`missionBlocksPlanets`-in-`fp` root cause).

## 0. Baseline (verified)

| metric | value |
|---|---:|
| function size | 9,908 / 9,908 |
| raw same-address diff | 6,065 |
| instruction-index alignment | 2,043 |
| instructions | 3,541 ours / 3,532 orig |
| `.rel.plt` | byte-identical |
| `check` | RATCHET PASS (byte 4,507 / linked 4,507 / size 4,524 / data 843 / imports 207 / lint CLEAN / drift 0) |

## 1. Fast measurement harness (new, reusable)

`scratch/w13474/fast.py` compiles `Radar.cpp` alone with the exact ninja
command line and relinks the whole `.so` with the checked-in gold link command
(substituting only the Radar object and the output paths). One measurement is
**~1.0 s** instead of the ~25 s of a full `cmake --build`, which is what made
the sweeps below affordable (about 1,600 cells in this pass).

```python
import fast
fast.run('/path/to/Radar.cpp')   # -> (raw, aligned, ours_size, orig_size)
```

Gotcha found the hard way: the compile command's `-o` object path has no `./`
while the link command's does — substituting one string for both silently
links a stale object and every variant measures identical. `fast.py` now
asserts both substitutions.

## 2. Statement-order sweeps (all size-exact, none < 6,065)

Every same-brace-depth single-statement move in the whole `Radar::draw` body,
forward and backward, plus the declaration block inside the planet loop:

| sweep | cells | best raw | note |
|---|---:|---:|---|
| forward move, distance 2-6, whole body | 328 | 6,065 | no win |
| backward move, distance 2-6, whole body | 328 | 6,065 | no win (new: previous passes only moved forward) |
| forward move, distance 7-12, whole body | 630 | 6,065 | no win |
| forward move incl. declarations, planet-loop prologue lines 540-640, dist <= 15 | 124 | 6,065 | no win |
| `&&` operand swap, top level | 32 | 6,065 | no win |
| `\|\|` operand swap, top level | 19 | 6,065 | no win |
| exhaustive same-depth pair swaps (previous model, `pairsweep3_results.json`) | 755 | 6,065 | no win |

The w13456-pass3 music-tail `play` id order and the w13465 gas-cloud /
`currentStation` cells are already landed and are the only statement-order wins
that exist on this base.

## 3. Register-allocation lever (the dominant residual)

The whole `12e3d0`-`12f6d0` cascade is a single allocation rotation:

* **original:** `campaign` in `sl` (`12e0f8 mov sl,r0`, used once at
  `12e38a cmp.w sl,#2`), `missionBlocksPlanets` in `fp` (spilled to `sp+112`
  before the loop), `i` in `fp`, `stations` in `sl` (spilled to `sp+116`
  around clobbers), `marked` **in memory** at `sp+132` (word stores, and the
  `ldr/cmp/it ne/movne/str` normalisation at `12e67a`).
* **ours:** `missionBlocksPlanets` in `sl`, `campaign` spilled at `sp+144`,
  `i` in `sl`, `stations` spilled at `sp+132`, `marked` in `fp`.

`sl` is provably *free* over `12e0f8`-`12e3ba` in ours (no `sl` use in the
disassembly), yet clang spills `campaign` there. The decision is global, not
local: the landmark loop uses the identical register set (r4/r5/r6/r8/r9) in
both, and the 12e050-12e350 code is instruction-identical apart from the
`sl`/`fp` swap. So the trigger has to be a weight/priority difference in the
loop body, almost certainly the `marked` interval.

Measured on this base (all size-exact unless noted):

| variant | raw | size | verdict |
|---|---:|---:|---|
| base | **6,065** | 9,908 | — |
| `marked` as `int`/`short`/`long`/`signed char`/`char`/`uchar`/`uint8_t` | 8,466 | 9,912 | +4 B |
| `volatile int marked` | 9,087 | 9,904 | -4 B |
| `volatile unsigned char marked` | 8,823 | 9,912 | +4 B |
| `marked` declared at the top of the loop body | 6,065 | 9,908 | neutral |
| `marked` declared outside the loop, reset at loop top | 6,065 | 9,908 | neutral |
| `bool *markedPtr = &marked` (address escape) | 6,065 | 9,908 | neutral |
| `campaign` split decl/assign, `(int)` cast, `const`, `unsigned` | 6,065-6,066 | 9,908 | neutral |
| `volatile int campaign` | 9,200 | 9,892 | -16 B |
| guard `campaign > 1` / `2 <= campaign` / `(unsigned)campaign >= 2u` | 6,065-6,066 | 9,908 | neutral |
| planet-loop `i` hoisted out of the `for`, `int i`, `while` form | 6,065-9,459 | 9,908 | neutral/worse |
| `stations` as `*const`, `stations` declared before `planetHit` | 6,065 | 9,908 | neutral |
| `stationId` removed (inline `stations->data()[i]`) | 9,327 | 9,996 | worse |
| `wreck` as `unsigned` | 6,065 | 9,908 | neutral |
| `wreck` as `bool` | 6,906 | 9,904 | worse |
| `asteroidHit`/`enemyHit` as `int` | 7,582 / 8,615 | 9,924 / 9,940 | worse |

`marked` is definitely a word-sized memory variable in the original (word
`str`/`ldr` at `sp+132`, plus the `it ne/movne` bool normalisation at
`12e67a`), but no source spelling tried here reproduces that *and* the
`campaign`/`missionBlocksPlanets` swap.

## 4. Branch-vs-predication cells (structural, not just registers)

Two places where the original branches and ours predicates:

* **tractor block** (`Radar.cpp:1164`,
  `if (this->field_0x1c == nullptr && wreck && this->tractorMode2)`):
  original `ldrb [r4,#431]; cbz; ldr [r8]; cbnz; eors sl,#1; itttt eq` vs ours
  `ldr [r8]; eor sl,#1; cmp; it ne; movne; orrs; bne; ldrb [r4,#431]; cbz`.
  Reordering to `tractorMode2 && field_0x1c == nullptr && wreck` (and the
  nested-if form) gives 6,067 (+2); `__builtin_expect` variants 6,065-7,624.
* **music tail** (`Radar.cpp:1265`,
  `(uint32_t)(current-136) > 15 || expect(current != 151 && alwaysEnemySeen) || ((1 << (current-136)) & 0xe071) == 0`):
  the original tests range, then mask, then `current == 151`, then
  `alwaysEnemySeen`; ours merges `current != 151 && alwaysEnemySeen` into a
  branchless `it ne/movne/ands`. Reordering (mask before the enemy test,
  De Morgan, split bools, `expect` inside) all measure 6,065-7,183.


## 4b. Gas-cloud `camZ` hoist (structural, but a net loss)

`Radar.cpp:957` hoists `const auto camZ = this->cameraZ;` to the gas-loop
preheader, so ours emits `vldr s16,[r4,#580]` (cameraZ) at `12f43c` and the two
box-test constants into `s18`/`s20`.  The original loads only the two constants
at `12f440`/`12f444` and loads cameraZ lazily at the use site
(`12f6e6 vldr s6,[r4,#580]`).  So the original source almost certainly writes
`this->cameraZ` directly in the `calcDistance(...)` call at line 1009.  Every
spelling tried is nonetheless a net raw loss on this base:

| variant | raw | size |
|---|---:|---:|
| base (`const auto camZ`, preheader) | **6,065** | 9,908 |
| `this->cameraZ` inline (no local) | 6,181 | 9,908 |
| `camZ` declared inside the `cloud == locked` block | 6,184 | 9,908 |
| `camZ` declared at the top of the loop body | 6,077 | 9,908 |
| `camZ` hoisted outside `if (gasClouds != nullptr)` | 6,065 | 9,908 |
| `const float` / `auto` instead of `const auto` | 6,065 | 9,908 |

The `12f43c` block itself matches the original exactly with the inline form,
but the gas-region diff rises 736 -> 852 bytes because the register rotation
elsewhere in the gas block (and the `lockedStation` block at `12f408`) moves
the wrong way.

## 5. Per-TU compiler flags (matching knobs)

The current `Radar.cpp` options (`-mllvm -disable-postra-machine-licm
-Xclang -target-feature -Xclang +cheap-predicable-cpsr`) are locally optimal:

| flags | raw | size |
|---|---:|---:|
| current | **6,065** | 9,908 |
| without `-disable-postra-machine-licm` | 6,067 | 9,908 |
| without `+cheap-predicable-cpsr` | 6,265 | 9,908 |
| neither | 6,267 | 9,908 |
| `-mllvm -disable-postra-machine-sink` | 6,221 | 9,908 |
| `-mllvm -split-spill-mode=size` | 9,416 | 9,892 |
| `-mllvm -split-spill-mode=default` | 8,964 | 9,956 |
| `-mllvm -regalloc=basic` / `=fast` | 9,830 / 13,696 | 10,152 / 13,952 |
| `-mllvm -join-liveintervals=false` | 10,137 | 10,408 |
| `-mllvm -old-thumb2-ifcvt`, `-enable-if-conversion=false`, `-disable-early-ifcvt` | 6,065 | 9,908 |
| `+prof-unpr` / `+use-misched` / `+long-calls` | 6,901 / 9,452 / 11,890 | 9,912 / 10,012 / 12,208 |

## 6. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6065 aligned= 2043 size=9908/9908 insn=3541/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13474/`: `fast.py` (1 s
compile+link harness), `sw.py`/`sw2.py`/`sw3.py` (forward/backward statement
moves), `andswap.py`/`orswap.py`, `flagsweep.py`, `dd.py` (aligned instruction
diff), `v.py` (variant helper), `Radar.base.cpp` (the 6,065 base).
