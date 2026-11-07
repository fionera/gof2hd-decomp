# `Level::createScene` — wave-128 w7655: the residual is a 2-cycle
# `{status, capacity}` over `{r6, sl}`, not the 3-cycle of waves 126/127

**Status:** dead-end sweep, 2026-11-07. Worktree `wt/w7655`, base `fee528cbe`.
Target `_ZN5Level11createSceneEv` (`src/game/world/Level.cpp`, `createScene`
only). **No source change landed** — the wave-124 landed source remains the
best at **93.554563 % / 2252 B**, 783 original / 784 ours normalized
instructions. Full gate re-measured green: **byte_exact 4506 / linked_exact
4506**, avg 99.94 %, data 843/843, imports 207, lint CLEAN, drift 0,
symbol parity 5370/5370.

Follow-up to `level-createscene-wave127-w7634.md` (which followed
`…-wave126-w7611.md`, `…-wave125-w7517.md`, `…-wave124-w7418.md`).

## TL;DR

The wave-126/127 notes described the residual as a three-value register
cycle `{capacity, spotUsed, ownHangar}`. This pass re-measured the **no-cap**
shape precisely and found a cleaner, more useful decomposition:

* **no-cap** (`int capacity` used directly in the two spot `nextInt`s) is
  **`ownHangar=r4, spotUsed=r4, rnd=r8, race=r5` — i.e. four of the five
  target values are already correct**, exactly the "4/5" the task
  hypothesis described, but for a *different* four than wave-123.
  Its only errors are **`capacity=sl` and `status=r6`**, a pure
  **two-value swap** over `{r6, sl}`.
* The landed wave-124 `cap` split flips that swap (`status=sl`,
  `capacity=r4`) and additionally rotates `ownHangar r4→r5`,
  `spotUsed r4→r6`, and adds the `uxth`.

So the true residual is not a 3-cycle; it is a **2-cycle `{status, capacity}`
over `{r6, sl}`** plus the `cap`-split side effects. Closing the function
needs a shape where `status=sl` **and** `capacity=r6` with no `uxth`.

## 1. Exact maps (re-verified this pass)

Original (`0xb42ec`, raw disassembly):

| value | original |
|---|---|
| `spotUsed` | r4 |
| `race` | r5 |
| `capacity` | r6 |
| `&Globals::rnd` | r8 |
| `i` | r9 |
| `&Globals::status` | sl (spilled `[sp,#40]` in parts loop) |
| `type` | fp |
| `ownHangar` | r4 (then `[sp,#24]`) |
| `spots` / `hangarPart` | `[sp,#44]` |

| source | pct | size | insns | own | capacity | spot | status | rnd | race |
|---|---|---|---|---|---|---|---|---|---|
| **wave-124 (landed)** | **93.554563** | 2252 | 784 | r5 | r4 | r6 | sl | r8 | r5 |
| no-cap (`int capacity` in spot calls) | 80.305927 | 2228 | 786 | **r4** | **sl** | **r4** | **r6** | r8 | r5 |
| `int cap = (unsigned short) capacity` | 78.776291 | 2228 | 786 | r6 | r5 | r4 | — | r8 | r5 |

The no-cap row is the new datum: **`own=r4, spot=r4, rnd=r8, race=r5` all
match the original**; only `status`/`capacity` are transposed.

## 2. Why the `cap` split is the pivot

The `cap` split (`unsigned short cap = (unsigned short) capacity;`) changes
capacity's live range from one long interval (`b4932..b4a5a`) into a short
`capacity` (`b4932..b497c`) plus a long `cap` (`b497c..b4a5a`). It is the
only lever found that moves `status` into `sl` (the no-cap shape puts
`status` in `r6`), but it also makes the long value land in `r4` and pushes
`ownHangar` to `r5` / `spotUsed` to `r6`.

The LLVM 7 greedy allocator (`RAGreedy::enqueue`) orders *global* live
ranges by `(1u<<29) + Size` (largest first), so `capacity` is dequeued
before the short `race` in the wave-124 shape and takes `r4`. For the
original map, `race` must be coloured `r5` **before** `capacity` is
coloured, so that `capacity` cannot take `r5` (they overlap across the
loop) and falls to `r6`; `ownHangar` then takes the first free register
`r4` in allocation order.

## 3. Variants measured this pass (all ≤ 93.554563 %)

Every row was compiled with the repo's exact NDK r18b `-Oz` flags; the
map was extracted from the linked `.so` with `/dev/shm/w7655/sig.py`.

### 3.1 Preamble statement-order permutation (exhaustive)

All 7 valid permutations of `{ownHangar, rnd, capacity, cap, shipCount}`
that keep the 2252 B / 784-instruction size produce **byte-identical code**
to the landed source (same map `own=r5 capacity=r4 spot=r6 status=sl`):

`ABCDE, ABCED, ACBDE, ACBED, ACDBE, BACDE, BACED`
(A=ownHangar, B=rnd, C=capacity, D=cap, E=shipCount).

Putting `capacity` before `ownHangar` (`CADBE`) drops to the 2276 B basin
(86.818758 %). **Statement order around the ownHangar branch is not the
lever.**

### 3.2 `ownHangar` expression forms (all identical)

`if`/`&&`/ternary/`int`/`int &&` — all byte-identical.

### 3.3 `capacity` / `cap` type and placement forms

| variant | pct | size | map |
|---|---|---|---|
| `const int capacity` | 93.554563 | 2252 | identical |
| `(int) kHangarCapacity[sceneRace]` | 93.554563 | 2252 | identical |
| `kHangarCapacity[sceneRace] + 0` | 93.554563 | 2252 | identical |
| `unsigned int capacity` | 93.426930 | 2252 | identical |
| `1 + capacity` | 93.554563 | 2252 | identical |
| `short cap`, `const unsigned short cap`, implicit conversion | 93.554563 | 2252 | identical |
| `cap` declared after the ownHangar block | 93.554563 | 2252 | identical |
| `cap` recomputed from the table | 93.554563 | 2252 | identical |
| `long`/`unsigned`/`size_t`/`int&` `cap` | 80.305927 | 2228 | no-cap basin |
| `unsigned short cap` used for `new[]`/zero loop | 79.389313 | 2236 | — |
| `unsigned short cap` used in the `min` | 89.356278 | 2256 | own=r5 cap=r4 |
| `unsigned short cap` retry only | 75.079669 | 2228 | own=r6 cap=r5 spot=r6 |
| `unsigned short cap` first call only | 69.587302 | 2236 | — |

### 3.4 Loop / geo / race forms

| variant | pct | size | map |
|---|---|---|---|
| `geo` inline (`ship->geometry->setRotation`) | 89.923469 | 2256 | own=r5 cap=r4 spot=r6 |
| `geo` after `setToSleep` | 89.923469 | 2256 | same |
| `race` hoisted / zero-initialised outside loop | 93.554563 | 2252 | identical |
| `ships` if/else instead of ternary | 93.554563 | 2252 | identical |
| reversed `min` (`size < capacity`) | 93.426930 | 2252 | identical |

## 4. Remaining root cause / next angle

The residual is the greedy-allocator colouring order of
`{status, capacity}` over `{r6, sl}` (plus the `cap`-split rotation).
The no-cap shape already has `own=r4, spot=r4, rnd=r8, race=r5`; it only
needs `status` and `capacity` swapped. A shape that raises the pressure
enough to put `status` in `sl` **without** splitting `capacity` (so
`capacity` stays one long interval and lands in `r6`) would close it.
Every `cap`-split spelling tried adds the `uxth` and rotates
`own/spot`; every no-split spelling leaves `capacity=sl, status=r6`.

The most promising unexplored lever is the **CSR-first-time heuristic**
(`RAGreedy::tryAssignCSRFirstTime`): the largest range (`status`) is
pre-split instead of taking the first free CSR, and that decision is
sensitive to the split cost / block frequencies. A source shape that
changes `status`'s split cost (e.g. a different number of `Globals::status`
uses in the ship loop) without changing the emitted call sequence is the
next thing to try.

## 5. Reproduction

Scratch is `/dev/shm/w7655`:

* `Level.cpp.base` — pristine landed `src/game/world/Level.cpp`;
* `Level.cpp.w123` — the wave-123 source (`bfe6d18d2:src/game/world/Level.cpp`);
* `fast.py` — single-function unrounded `difflib` score + size/insns;
* `sig.py` — extracts `own/capacity/spotUsed/status/rnd/race` from the
  linked `.so`;
* `run.py`, `gen.py`, `gen2.py`, `gen3.py`, `perm.py` — variant runners;
* `v1.json` … `v13.py` — the variant sets above;
* `mi124.txt` — `-mllvm -print-machineinstrs` dump of the landed source
  (`-mllvm -filter-print-funcs=_ZN5Level11createSceneEv`), used to read the
  pre/post-RA vregs (`%649`=ownHangar, `%27`=capacity).

```sh
OBJDUMP=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so --objdump $OBJDUMP --show _ZN5Level11createSceneEv
cmake --build cmake-build-match --target check -j6   # byte 4506 linked 4506
```
