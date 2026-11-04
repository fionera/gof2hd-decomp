# Globals::Globals C1/C2 (80.5%, 612 B) — w5808 (wave 110)

Status: research note, 2026-11-04. Worktree `wt/w5808`, branch `wave110-w5808`,
base `390f58052`. **No source change landed** — the checked-in constructor is
still the best measured fuzzy shape (80.5%). This commit contains only this
note.

## TL;DR

* Baseline and final: **`_ZN7GlobalsC1Ev` / `_ZN7GlobalsC2Ev` = 80.5%, 612 B**
  (size-exact; M = 182, 2 trailing `nop`s). Pool LCS **30/31** (`status` at
  slot 6, `recordHandler` at 7); `movw #583` at normalised index 139
  (original: 34).
* The only shape that reproduces the original's 31-word GOT pool order
  (`status` at slot 26) is still **variant B** (`recordHandler` last in the
  early pointer block, `status` in the late block after
  `lastStationMusicPlayed`): **78.8%**, M = 178, pool **31/31**. It loses
  1.7 points because the missing `movw` hoist shifts every later `[pc,#N]`.
* This pass re-tested the wave-109 hint ("different declaration/initialization
  mechanism: helper function, static initializer order, different literal
  form"). **Every mechanism tried compiles byte-identically to the direct
  store**: helper functions (return / pointer-to-pointer / reference),
  pointer aliases, `memset`, `if (1)`, `do { } while (0)`, one-iteration
  `for`, comma expressions, `(Status *)0`, `nullptr`, `0L`, `(int)(0x247u)`,
  `0x247 + 0`, `0x247 * 1`, `583`, `(void)v54`, `v54 = v54`, and a
  `{int w,h;}` struct-returning helper. The mechanism is not the lever.
* New negative searches this pass: 2 000 random full statement orders (best
  **64.3%**) and 5 000 random late-block orders (best **73.5%**). The
  baseline/variant-B orders are not reachable by random permutation; they are
  special.
* Reordering the static *definitions* in `Globals.cpp` (`status` moved to the
  top / bottom of the file) does not change the constructor at all.

## 1. Commands

```bash
cd /home/debian/GalaxyOnFire2/wt/w5808
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
```

Gate at baseline (re-run this pass, unchanged tree):

```text
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.92
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Fast per-variant scorer (scratch, `/dev/shm/w5808`, not tracked):
`build_one.sh` is the exact repo compile line for `Globals.cpp` plus a plain
gold link into `mini.so`; `score.py` reproduces the official `verify.py` fuzzy
number, M, pool LCS, `movw` index and size; `gen.py` rebuilds the constructor
body from an ordered token list; `psrch.py` / `platesrch.py` are parallel
random-order searches.

## 2. Ground truth (unchanged)

`_ZN7GlobalsC1Ev` @ `0xe31e0`, 612 B = 472 B code + 16 B float pool
(`0x3f000000, 0x3f000000, 0x3f19999a, 0x3f19999a`) + 124 B (31-word) GOT pool
@ `0xe33c8`.

Original GOT pool order (31 words):

```text
Canvas, lastSpace, lastStation, lcmf, lcmfc, options, recordHandler,
shipTemplate, gameText, font, fontAlien, fontLangSelect, rnd, globals,
layout, appManager, generator, hints, gameLoaded, gameSaving, initMem,
imageFactory, achievements, galaxy, ias, iaw, status, iat, iapn, items, ships
```

Baseline pool is identical except `status` is at slot 6 (`recordHandler` is
the original's slot 6). The original keeps `&imageFactory` in `ip` from index
128 to 181 (32-bit `ldr.w ip` / `str.w ip`, 8 B); ours uses the 16-bit
`ldr r0` / `str r2` pair (4 B), hence 468 B + 2 `nop`s.

Variant B is exactly the baseline with two statements moved:

```cpp
// early pointer block: move recordHandler to the end
Globals::shipTemplate = 0;
Globals::Canvas = 0;
...
Globals::generator = 0;
Globals::recordHandler = 0;

// late block: insert status after lastStationMusicPlayed
Globals::lastStationMusicPlayed = -1;
Globals::status = 0;
```

## 3. Measured variants this pass (official fuzzy %)

| variant | score | M | pool LCS | movw@ | notes |
|---|---:|---:|---:|---:|---|
| **baseline HEAD** | **80.5** | 182 | 30/31 | 139 | best |
| variant B (recordHandler last, status late) | 78.8 | 178 | **31/31** | 135 | faithful pool |
| baseline early + status late (recordHandler 2nd) | 77.4 | 175 | 28/31 | 135 | hoist set changes |
| variant B + status after each late marker (15 positions) | 56.6–78.8 | 128–178 | 28–31 | 135 | best = after lastStation |
| variant B + Q at late positions q0–q19 | 59.7–78.8 | 135–178 | 24–31 | 34–162 | q12–q15 give movw@34 but 74.3–75.7 |
| variant B + helper (return / ptr / ref / struct / void) | 78.8 | 178 | 31/31 | 135 | byte-identical |
| variant B + literal forms (`nullptr`, `(Status*)0`, `0L`, …) | 78.8 | 178 | 31/31 | 135 | byte-identical |
| variant B + `memset`, `if(1)`, `do/while`, `for(1)`, comma | 78.3–78.8 | 177–178 | 30–31 | 135 | comma forms −0.5 |
| variant B + settings reorder (flagAt3f/intAt48) | 78.8 | 178 | 31/31 | 135 | byte-identical |
| baseline + settings reorder (flagAt3f/intAt48) | 80.5 | 182 | 30/31 | 139 | byte-identical |
| baseline + `status` definition moved top/bottom of file | 80.5 | 182 | 30/31 | 139 | byte-identical |
| 2 000 random full statement orders | ≤64.3 | ≤145 | ≤21 | — | early+settings+late |
| 5 000 random late-block orders (baseline/variant-B early) | ≤73.5 | ≤166 | ≤23 | 34–143 | not reachable by random |
| `-mllvm -enable-misched=false` | 78.8 | 178 | 31/31 | 135 | no change on B |
| `-mllvm -misched=converge/ilpmax/ilpmin/...` | 78.8 | 178 | 31/31 | 135 | no change on B |
| `-g0` / `-fno-omit-frame-pointer` | 78.8 | 178 | 31/31 | 135 | no change on B |
| `-mllvm -pre-RA-sched=source` | 24.7 | 55 | 23/31 | 103 | dead end |

## 4. Proven vs hypothesis

Proven (measured, reproducible):

1. The checked-in baseline remains the best measured shape (**80.5%**). No
   source-order permutation, random search, helper mechanism, literal form or
   declaration-order change tried this pass beats it.
2. Variant B is the unique shape found that reproduces the original's 31-word
   pool order exactly (`status` at slot 26). It is worth **−1.7 points**.
3. The `status` pool slot is fixed by moving `recordHandler` to the end of the
   early pointer block and `status` to the late block; moving only `status`
   (leaving `recordHandler` second) changes the 7-load prologue hoist set and
   drops the pool to 28/31.
4. `movw #583` at index 34 is reachable only by moving the whole Q block into
   the late block (variant B q12–q15), and every such order costs 3–6 pool
   words (best 75.7).
5. The initialization *mechanism* is irrelevant: helper functions, pointer
   aliases, casts, literals, `memset`, one-iteration loops, `if (1)`,
   `do/while`, comma expressions and struct returns all compile to the same
   bytes as the direct `Globals::status = 0;`.
6. The static definition order in `Globals.cpp` does not affect the
   constructor's pool order or schedule.
7. Random full-order and late-block searches do not find the baseline or
   variant B; they are special orders, not a broad optimum.

Hypothesis (not proven):

* The residual is a **SelectionDAG `list-hybrid` scheduling difference**:
  the original keeps `resWidth`'s default `583` in `r6` from index 34 and
  keeps `&imageFactory` in `ip` across a 53-instruction live range; clang
  7.0.2 at `-Oz` sinks the constant into `r0` inside the Q block and
  rematerialises the `imageFactory` address for every source shape tried.
  `-enable-misched=false` is instruction-identical, so the decision is made
  before the pre-RA machine scheduler.
* Because the original `Globals.cpp` is an Android-2.0.x TU with no dump
  witness for the constructor body, the remaining gap may be irreducible from
  this TU alone without a source-level witness.

## 5. Recommendation

Keep the baseline (80.5). If a later pass wants the faithful pool order, the
change is exactly variant B (78.8, pool 31/31); it should be re-measured
against whatever the then-current baseline is.
