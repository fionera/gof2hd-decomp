# Globals::Globals C1/C2 (80.5%, 612 B) — w5509 (wave 107)

Status: research note, 2026-11-04. Worktree `wt/w5509`, branch `wave107-w5509`,
base `f857b5171`. **No source change landed** — the baseline source order is still
the best fuzzy shape (80.5%). This commit contains only this note.

## TL;DR

* Baseline and final: **`_ZN7GlobalsC1Ev` / `_ZN7GlobalsC2Ev` = 80.5%, 612 B**
  (size-exact; 225 vs 227 normalised entries, M = 182,
  `2·182/452 = 0.8053097345132744`). Pool LCS **30/31** (`status` at slot 6).
  `movw #583` at normalised index 139.
* The only shape that puts `status` at its true pool slot 26 (pool LCS **31/31**)
  is still **variant B** (`recordHandler` last of the early pointer block,
  `status` in the late block after `lastStationMusicPlayed`): **78.8%**, M = 178.
* New this pass: a **full single-token move sweep over all 66 statements**
  (66·65 insertion moves + 132 quality-block position moves, 4 422 compiles)
  finds **no move that beats 80.5**; a settings-block insertion hill climb, a
  late-block hill climb and an 800-eval simulated anneal all return the
  baseline. The baseline is a genuine local optimum of the source-order space.
* New this pass: the **4-byte code shortfall is not the `movw`**. Both builds
  contain exactly one `movw #583` (4 B); it is only its *position* that differs
  (orig index 34, ours 139). The 4 bytes are the `imageFactory` store: the
  original uses the 32-bit `ldr.w ip,[r0]` / `str.w r2,[ip]` pair (8 B), ours
  uses the 16-bit `ldr r0,[r0]` / `str r2,[r0]` pair (4 B). Both builds have
  exactly **186 code instructions**; the original is 472 B, ours 468 B + 2 `nop`.
* The `movw` hoist and pool 31/31 remain mutually exclusive in every order
  tried: the quality-block positions that reproduce `movw` at index 34
  (A: q53–q55, B: q52–q55) all cost 4–7 pool words.

## 1. Commands

```bash
cd /home/debian/GalaxyOnFire2/wt/w5509
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS (unchanged)
```

Gate at baseline (unchanged, re-run this pass):

```text
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.91
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Fast per-variant scorer (scratch, `/dev/shm/w5509`, not tracked): `compile.sh`
is the exact repo compile line for `Globals.cpp`; `score.py` reproduces the
official `verify.py` fuzzy number, M and pool order from the resulting
`Globals.o`; `gen.py` rebuilds the constructor body from an ordered token list
(66 tokens: 14 pointer stores + `tintR` + `this+0` + 26 settings stores +
`secondary` decl + 19 late stores + the 9-line quality block inserted at any
position). `pscore.py` is the same scorer parallelised over the NDK clang.

## 2. Ground truth (re-derived from the shipped `.so` this pass)

`_ZN7GlobalsC1Ev` @ `0xe31e0`, 612 B = 472 B code + 16 B float pool
(`0x3f000000, 0x3f000000, 0x3f19999a, 0x3f19999a`) + 124 B (31-word) GOT pool
@ `0xe33c8`.

### 2.1 Original GOT pool order (31 words)

```text
Canvas, lastSpace, lastStation, lcmf, lcmfc, options, recordHandler,
shipTemplate, gameText, font, fontAlien, fontLangSelect, rnd, globals,
layout, appManager, generator, hints, gameLoaded, gameSaving, initMem,
imageFactory, achievements, galaxy, ias, iaw, status, iat, iapn, items, ships
```

The first 7 are the prologue hoist (`ldr rX,[pc,#N]` + `add rX,pc` at
`e31e8..e3202`); the remaining 24 are materialised inline, in store order.

### 2.2 Original machine store order (56 stores)

```text
Canvas, shipTemplate, gameText, font, fontAlien, fontLangSelect, rnd,
globals, layout, appManager, recordHandler, generator, this+0,
opt:0x14 (vst1: 0.5,0.5,0.6,0.6), opt:0x0c (0x0101),
opt:0x00/0x04 (strd 0.5,0.5), opt:0x08 (0.5),
opt:0x24/0x28 (strd 0.5,1.0), opt:0x2c (0.5), opt:0x0e (1), opt:0x30 (1),
opt:0x10, opt:0x40, opt:0x50, opt:0x39, opt:0x35, opt:0x31,
hints:0x04, hints:0x13, opt:0x3f, opt:0x3b, opt:0x4e, opt:0x4c, opt:0x48,
hints:0x00,
gameLoaded, gameSaving, lastSpace(-1), initMem, lastStation(-1),
lcmfc(0), lcmf(-1), opt:0x54 (resWidth), opt:0x58 (resHeight),
ias, iaw, iat(-1), iapn, items, status, opt:0x60 (0x0100),
galaxy, achievements, imageFactory, ships, this+4 (soundResources)
```

### 2.3 The prologue of variant B is byte-identical to the original through `0x52`

Variant B (`recordHandler` last of the early block, `status` late) reproduces
the original's first **33 instructions / 0x52 bytes** exactly — same
registers, same order, same pool words. The first difference is index 34:
the original has `movw r6,#583`, variant B has `str r2,[r0,#0]` (shipTemplate).
The `movw` is not missing from variant B; it appears at index 135 with
register `r0` instead of `r6`.

### 2.4 The 4-byte shortfall is the `imageFactory` store, not the `movw`

Both builds have **186 code instructions**. The original is 472 B; ours is
468 B + two `nop`s (pool alignment). The two extra bytes come from two
16-bit → 32-bit encodings:

```text
orig  e3324: ldr.w ip,[r0]     (4 B)   ours: ldr r0,[r0]   (2 B)
orig  e33a8: str.w r2,[ip]     (4 B)   ours: str r2,[r0]   (2 B)
```

i.e. the original keeps `&imageFactory` in `ip` across a 53-instruction live
range (index 128 → 181) and reuses `ip` after the `lcmfc` store (`ldr.w
ip,[r6]` index 33 → `str.w r2,[ip]` index 122). Our allocator keeps the
`imageFactory` address live for only 2 instructions. Because the pool address
depends on the code length, this 4-byte delta shifts **every** `ldr rX,[pc,#N]`
immediate in both the baseline and variant B, which is the dominant source of
the residual body mismatch.

## 3. Measured variants (this pass)

All scores are the official `verify.py` fuzzy % on `_ZN7GlobalsC1Ev`
(`2·M/452`); `pool` is the LCS of the 31 GOT words; `movw@` is the normalised
index of `movw #583` (original: **34**).

| variant | score | M | pool | status slot | movw@ |
|---|---:|---:|---:|---:|---:|
| **baseline (HEAD)** | **80.5** | **182** | **30/31** | 6 | 139 |
| variant B (`recordHandler` last, `status` late) | 78.8 | 178 | **31/31** | **26** | 135 |
| variant B + original settings tail | 77.4 | 175 | 31/31 | 26 | 133 |
| variant B + grouped `if (a) {w=..;h=..;}` | 77.4 | 175 | 31/31 | 26 | — |
| variant B + ternary `resWidth = a ? .. : ..` | 76.5 | 173 | 31/31 | 26 | — |
| variant B + direct stores (no locals) | 78.8 | 178 | 31/31 | 26 | 135 |
| variant B + `unsigned` locals | 78.8 | 178 | 31/31 | 26 | 135 |
| variant B + `int v54; int v58;` declared first | 78.8 | 178 | 31/31 | 26 | 135 |
| variant B + `else if` chains | 27.5 | 62 | 19/31 | 26 | — |
| variant B + `v54` decl hoisted to top | 46.5 | 105 | 26/31 | 26 | — |
| variant B + `imageFactory` duplicated early | 78.8 | 178 | 31/31 | 26 | 135 |
| baseline + original settings tail | 79.2 | 179 | 30/31 | 6 | 137 |
| baseline + grouped `if` | 79.2 | 179 | 30/31 | 6 | — |
| baseline + `v54` decl hoisted to top | 48.2 | 109 | 26/31 | 5 | — |
| baseline + `imageFactory` duplicated early | 80.5 | 182 | 30/31 | 6 | 139 |
| baseline + early `settings->resWidth = 0x247` (direct form) | 33.6 | 76 | 17/31 | 5 | — |
| baseline + early `settings->resWidth = 0x247` (local form, dup store) | 80.5 | 182 | 30/31 | 6 | 139 |
| baseline + early `resWidth` at top of body | 34.1 | 77 | 17/31 | 6 | — |
| baseline + early `resWidth` after `status` | 32.3 | 73 | 17/31 | 6 | — |
| pool order as source order | 29.0 | 65 | 22/31 | 11 | — |
| original machine store order as source | 60.6 | 136 | 18/31 | 30 | — |
| pool-ordered late block (`LATE_P`) | 70.4 | 159 | 28/31 | 26 | — |
| original-store-ordered late block (`LATE_C`) | 74.8 | 169 | 24/31 | 26 | — |

`status` position sweeps (everything else at baseline):

* `status` inside the early block, positions 0..13:
  79.2, 79.2, 79.6, 80.1 ×10, **80.5** (pos 13 = baseline), pool 27..30.
* `status` in the late block (baseline early block, `recordHandler` 2nd):
  55.3–77.4, pool 25–28 (never 31/31 — `recordHandler` is not hoisted).

Quality-block position sweeps (the 9-line `fv`/`v54`/`v58` block inserted
before token *q* of the 60-token order):

| order | 80.5/78.8 region | `movw@34` region | score there | pool there |
|---|---|---|---:|---:|
| A (baseline, `status` early) | q39–q44 | **q53–q55** | 73.0 / 71.7 / 72.1 | 24 / 23 / 24 |
| B (`status` late) | q38–q43 | **q52–q55** | 75.7 / 75.2 / 74.3 / 74.8 | 25 / 24 / 24 / 25 |

So the `movw` hoist *is* reachable from source, but only by moving the quality
block into the middle of the late block (between `items`/`ships` and `lcmf`),
which re-allocates the hoist set (`status`, `iat`, `iapn`, `items` move into
the prologue; `lastStation`, `lcmf`, `lcmfc` drop out) and costs 4–7 pool
words. A late-block hill climb with the quality block pinned at q12–q15 of
variant B (the `movw@34` region) tops out at 76.1–77.0.

Search coverage (all negative):

* full single-token insertion sweep over all 66 statements from the baseline
  (4 290 moves) + 120 quality-position moves → max **80.5** (the baseline);
* insertion hill climb over the 26-token settings block → **80.5**;
* 800-eval simulated anneal over the whole 60-token order → **80.5**;
* late-block insertion hill climb with the quality block pinned at each of
  q12..q15 of variant B → 76.5 / 76.1 / 76.1 / 77.0.

## 4. What is proven vs hypothesis

**Proven (measured):**

1. The baseline is a local optimum under single-token insertion moves over the
   whole statement order, under settings-block insertion moves, and under an
   800-eval simulated anneal. Nothing in this neighbourhood beats 80.5.
2. Variant B is the unique shape found that reproduces the original's 31-word
   pool order exactly. It is worth **−1.7 pts** (M 182 → 178) because the
   missing `movw` at index 34 shifts every subsequent `ldr rX,[pc,#N]`.
3. Variant B's first 33 instructions are byte-identical to the original.
4. The `movw #583` hoist is reachable (quality block at A-q53..q55 /
   B-q52..q55) but always at the cost of 4–7 pool words.
5. The 4-byte code shortfall is the `imageFactory` `ldr.w ip` / `str.w ip`
   pair, not the `movw` (both builds contain exactly one `movw #583`).
6. `int v54 = 0x247;` hoisted to the top of the body is catastrophic
   (48.2 / 46.5): it pins a callee-saved register across the whole early block
   and displaces the GOT hoist set.
7. An early `settings->resWidth = 0x247;` store in the *local-variable* form is
   store-to-store forwarded away and compiles to the baseline byte-for-byte;
   in the direct form it forces `settings` live from the top and collapses to
   33.6.

**Hypothesis (not proven):**

* The original source is variant B's shape: `recordHandler` last in the early
  pointer block, `status` late (after `instantActionWave`), quality block in
  the late block. Its 78.8 is a metric artifact of the two 16-bit encodings
  (`imageFactory`) plus the missing `movw` hoist, not a semantic difference.
* The `movw r6,#583` hoist is a pre-RA scheduler decision: `r6` is the GOT
  base for `lcmfc` and is freed by `ldr.w ip,[r6]` at index 33, so the
  scheduler can sink the `movw` there. In every order tried, the same
  scheduling pressure that produces the hoist also changes which 7 GOT
  addresses are hoisted, so the pool and the hoist cannot both be satisfied.
* Getting the `imageFactory` address into `ip` (a 53-instruction live range)
  would fix the 4-byte shift and therefore most of the `[pc,#N]` mismatches in
  both shapes, but no source form found so far makes our allocator keep it
  live: `nullptr` / cast / `static_cast` / reference / volatile / `int` /
  `long` / pointer-split / RAII-helper / duplicated-early-store all compile to
  the same 2-instruction form.

## 5. Recommendation

Keep the baseline (80.5). If a later pass wants the faithful pool order, the
change is exactly: move `Globals::recordHandler = 0;` to the end of the early
pointer block and move `Globals::status = 0;` to the late block after
`Globals::instantActionWave = 0;` — that is variant B (78.8, pool 31/31), and
it should be re-measured against whatever the then-current baseline is.
