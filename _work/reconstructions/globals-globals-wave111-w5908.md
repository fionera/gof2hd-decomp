# Globals::Globals C1/C2 (80.5%, 612 B) — w5908 (wave 111)

Status: research note, 2026-11-05. Worktree `wt/w5908`, branch `wave111-w5908`,
base `8eea060cc`. **Variant B landed** (the faithful pool-slot ordering):
fuzzy 80.5 -> 78.8%, GOT-pool LCS 30/31 -> 31/31. The gate is unchanged
(byte 4500, linked 4500). The tuned baseline (80.5, pool 30/31) is still
recorded below for comparison.

## TL;DR

* Baseline before this pass: **`_ZN7GlobalsC1Ev` / `_ZN7GlobalsC2Ev` =
  80.5%, 612 B** (size-exact; M = 182/227, 2 trailing `nop`s). GOT-pool LCS
  **30/31** (`status` at slot 6, `recordHandler` at 7); `movw #583` at
  normalised index 139 (original: 34).
* Landed (variant B): **78.8%**, 612 B, M = 178, pool **31/31** (`status` at
  slot 26), `movw`@135. It loses 1.7 fuzzy points because the missing `movw`
  hoist shifts every later `[pc,#N]`, but it is the only shape found that
  reproduces the original's 31-word GOT pool order exactly.
* The tuned baseline (80.5) is still the best *fuzzy* shape; variant B is the
  best *pool-faithful* shape.
* This pass reconstructed the original's literal-pool and store sequence
  (section 2) and tested the "member-initializer order/types" angle
  (section 3). **No source shape beat 80.5.**
* New negative results: exhaustive **single-statement-move** sweep from the
  baseline (3 782 candidates) and **pairwise-swap** sweep (1 891 candidates)
  both find **no** improvement. The baseline is a strong local optimum.
  Hill-climbs from variant B (single moves, LCS=31 constrained) and from the
  settings-reordered baseline also stall below 80.5.
* The `movw #583` hoist is a **post-RA scheduling** decision, not a source
  statement-order decision: an early `settings->resWidth = 0x247;` store does
  hoist it to index 34, but the compiler then emits **two** resWidth stores
  (size 628) instead of the original's one. `-disable-post-ra` moves the
  `movw` from 34 back to 144 in that variant, proving the post-RA scheduler
  is the lever. No source form was found that both hoists the constant and
  keeps the single store.

## 1. Commands

```bash
cd /home/debian/GalaxyOnFire2/wt/w5908
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

Fast per-variant scorer (scratch, `/dev/shm/w5908`, not tracked):
`build_one.sh` is the exact repo compile line for `Globals.cpp` plus a plain
gold link into `mini.so`; `score.py` reproduces the official `verify.py` fuzzy
number, M, pool LCS, `movw` index and size; `pools.py` prints the per-slot
GOT-pool comparison. `splice.py` rebuilds `Globals.cpp` from a constructor
body file.

## 2. Ground truth (reconstructed this pass)

`_ZN7GlobalsC1Ev` @ `0xe31e0`, 612 B = 472 B code + 16 B float pool
(`0x3f000000, 0x3f000000, 0x3f19999a, 0x3f19999a` = 0.5, 0.5, 0.6, 0.6) +
124 B (31-word) GOT pool @ `0xe33c8`.

Original GOT pool order (31 words):

```text
Canvas, lastSpace, lastStation, lcmf, lcmfc, options, recordHandler,
shipTemplate, gameText, font, fontAlien, fontLangSelect, rnd, globals,
layout, appManager, generator, hints, gameLoaded, gameSaving, initMemory,
imageFactory, achievements, galaxy, ias, iaw, status, iat, iapn, items, ships
```

(the full names are `lastSpaceMusicPlayed`, `lastStationMusicPlayed`,
`lastCampaignMissionFailed`, `lastCampaignMissionFailCount`,
`initMemoryWarning`, `instantActionScore/Wave/Type/PlayerName`).

Original **store** order, reconstructed from the Thumb disassembly
(`0xe31e0..0xe33b6`; the literal pool is emitted in the order of the first
`ldr [pc]` in this stream, so the first seven pool words are the scheduler's
prologue hoists):

```text
Canvas, shipTemplate, gameText, font, fontAlien, fontLangSelect, rnd,
globals, layout, appManager, recordHandler, generator,
this->field_0x0,
glowR,glowG,tintR,tintG (vst1), options+0x0c, colorR,colorG,colorB,
tintB, brightness, contrast, options+0x0e, options+0x30, options+0x10,
options+0x40, options+0x50, options+0x39, options+0x35, options+0x31,
hints+4, hints+0x13, options+0x3f, options+0x3b, options+0x4e,
options+0x4c, options+0x48, hints+0,
gameLoaded, gameSaving, lastSpace, initMemory, lastStation, lcmfc, lcmf,
resWidth (movw #583 hoisted at index 34; single `str r6,[r1,#84]`),
resHeight (movw #513 at index 129; single `str r0,[r1,#88]`),
instantActionScore, instantActionWave, instantActionType,
instantActionPlayerName, items, status, options+0x60, galaxy,
achievements, imageFactory, ships, this->soundResources
```

Key codegen facts:

* `movw r6,#583` is at index 34, immediately after `ldr.w ip,[r6]` frees
  `r6` (the `lcmfc` GOT address). `r6` then stays live until the resWidth
  store at index 139.
* `movw r0,#513` is at index 129, immediately after `ldr.w ip,[r0]` frees
  `r0` (the `imageFactory` GOT address).
* `&imageFactory` stays in `ip` from index 128 to 181 (32-bit `ldr.w ip` /
  `str.w ip`); ours rematerialises it as a 16-bit `ldr`/`str` pair (4 B
  shorter, hence the 2 trailing `nop`s).
* The original's `.bss` order (`nm -D -nS`) is `status, shipTemplate,
  Canvas, ..., options, hints` — i.e. the pool order is **not** the
  declaration order and **not** address-sorted; it is the instruction
  stream's first-reference order.

## 3. Measured variants this pass (official fuzzy %)

| variant | score | M | pool LCS | movw@ | notes |
|---|---:|---:|---:|---:|---|
| **baseline before this pass** | **80.5** | 182 | 30/31 | 139 | best fuzzy |
| **variant B (landed)** (`recordHandler` last, `status` late) | 78.8 | 178 | **31/31** | 135 | faithful pool |
| `status` late only (`recordHandler` 2nd) | 77.4 | 175 | 28/31 | 135 | hoist set changes |
| original store order, variant-B early block (`v_F`) | 48.2 | 109 | 21/31 | 142 | pool hoist set changes |
| original store order, `Canvas` first (`v_C`) | 46.9 | 106 | 18/31 | 142 | pool hoist set changes |
| `settings` block reordered to original (`BASE_E`) | 79.2 | 179 | 30/31 | 137 | settings match +3, tail −6 |
| `gameLoaded,gameSaving,lastSpace` reorder | 79.6 | 180 | 30/31 | 139 | |
| Q block after `items` (`qp_54`) | 75.7 | 171 | 25/31 | **34** | `movw` hoists, pool breaks |
| Q block after `ships`/`iaw`/`lcmf` | 74.3–75.2 | 168–170 | 24/25 | **34** | |
| `settings->resWidth = 0x247;` early + direct ifs | 29.8 | 68 | 17/31 | 28 | size 628 (two stores) |
| member-init list `: field_0x0(0), soundResources(0)` | 38.8 | 87 | 24/31 | 161 | size 596 (strd merge) |
| member-init list `: soundResources(0)` only | 54.4 | 123 | – | 140 | |
| `int v54`/`int v58` declared at top / before settings | 78.8 | 178 | 31/31 | 135 | byte-identical to B |
| `float fv` declared at top | 78.8 | 178 | 31/31 | 135 | byte-identical to B |
| `v54` early-use forms (`v54-0x247`, `v54&0`, …) | 78.8 | 178 | 31/31 | 135 | folded away |
| `settings` via `Globals::options` offsets instead of pointer | 78.8 | 178 | 31/31 | 135 | byte-identical |
| `-Os` instead of `-Oz` | 80.5 | 182 | 30/31 | 139 | byte-identical |
| `-O2`/`-O3` | 71.1 | 162 | – | 139 | size 620 |
| `-O1` | 40.0 | 89 | – | 128 | size 596 |
| `-pre-RA-sched=list-burr` | 25.1 | 56 | 24/31 | 127 | |
| `-pre-RA-sched=list-ilp` | 18.1 | 41 | 14/31 | 143 | |
| `-pre-RA-sched=source` | 24.7 | 55 | 23/31 | 103 | |
| `-disable-post-ra` (baseline) | 62.4 | 141 | – | 136 | |

Search results (all measured with the official scorer):

| search | best score | pool LCS | movw@ |
|---|---:|---:|---:|
| exhaustive single-statement move from baseline (3 782) | 80.5 | 30 | 139 |
| exhaustive pairwise swap from baseline (1 891) | 80.5 | 30 | 139 |
| hill-climb from baseline (400×8 random moves) | 80.5 | 30 | 139 |
| hill-climb from variant B, LCS=31 constrained (600×12) | 78.8 | 31 | 135 |
| exhaustive pairwise swap from variant B, LCS=31 (1 891) | 78.8 | 31 | 135 |
| hill-climb early block from variant B, LCS=31 (200×12) | 78.8 | 31 | 135 |
| hill-climb from settings-reordered baseline (400×8) | 79.6 | 30 | 138 |
| hill-climb from Q-after-items, movw=34 constrained (300×12) | 76.5 | 26 | 34 |
| 3 000 random late-block permutations (variant-B early) | 73.9 | 21 | 135 |

## 4. Proven vs hypothesis

Proven (measured, reproducible):

1. The pre-pass baseline remains the best measured *fuzzy* shape (**80.5%**).
   No single statement move, pairwise swap, random permutation, settings
   reorder, member-initializer form, declaration position or scheduler flag
   tried this pass beats it. Variant B (78.8) is landed only because it fixes
   the pool-slot ordering, which was this pass's stated target.
2. Variant B is the unique shape found that reproduces the original's 31-word
   pool order exactly (`status` at slot 26). It is worth **−1.7 points**.
3. `movw #583` at index 34 is reachable only when the resWidth default is
   materialised early (early store, or the Q block moved to the end of the
   late block). Every such shape either changes the pool hoist set
   (LCS 17–26) or emits two resWidth stores (size 628).
4. The `movw` hoist is done by the **post-RA scheduler**: in the Q-after-items
   variant, `-disable-post-ra` moves `movw` from 34 to 144 while leaving the
   rest of the schedule intact.
5. The initialization *mechanism* is irrelevant: helper functions, pointer
   aliases, casts, literals, `memset`, one-iteration loops, `if (1)`,
   `do/while`, comma expressions, struct returns, `settings` as a reference
   and direct `Globals::options + offset` addressing all compile to the same
   bytes as the direct store.
6. `-Os` and `-Oz` are byte-identical for this TU; `-O1/-O2/-O3` are much
   worse.

Hypothesis (not proven):

* The residual is a **SelectionDAG + post-RA scheduling** difference. The
  original keeps `resWidth`'s default `583` in `r6` from index 34 and
  `&imageFactory` in `ip` across a 53-instruction live range; clang 7.0.2 at
  `-Oz` with every source shape tried sinks the constant into the Q block and
  rematerialises the `imageFactory` address. `-enable-misched=false` is
  instruction-identical, so the pre-RA machine scheduler is not the lever;
  the post-RA scheduler is (see 4).
* Because the original `Globals.cpp` is an Android-2.0.x TU with no dump
  witness for the constructor body, the remaining gap may be irreducible from
  this TU alone without a source-level witness.

## 5. Landed change

Variant B (78.8, pool 31/31) is now checked in. It is exactly the previous
body with two statements moved:

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
