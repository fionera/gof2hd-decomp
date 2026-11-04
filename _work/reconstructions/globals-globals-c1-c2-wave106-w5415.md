# Globals::Globals C1/C2 (80.5%, 612 B) — w5415 (wave 106)

Status: research note, 2026-11-04. Worktree `wt/w5415`, branch `wave106-w5415`,
base `486f8a119`. **No source change landed** — the baseline source order is still
the best fuzzy shape (80.5%). This commit contains only this note.

## TL;DR

* Baseline and final: **`_ZN7GlobalsC1Ev` / `_ZN7GlobalsC2Ev` = 80.5%, 612 B**
  (size-exact; 225 vs 227 normalised entries, M = 182,
  `2·182/452 = 0.8053097345132744`). Pool LCS **30/31** (`status` at slot 6).
* The only source order found that puts `status` at its true pool slot 26
  (pool LCS **31/31**) is **variant B**: `recordHandler` moved to the end of the
  early pointer block and `status` moved into the late block after
  `lastStationMusicPlayed`. It scores **78.8%** (M = 178) — a 1.7-point
  regression, because the prologue hoist set changes and the 4-byte-shorter
  body shifts every `ldr rX,[pc,#N]` immediate.
* A third shape, **Q@9** (variant-B early block, `QUALITY` inserted at position 9
  of the late block), reproduces the original prologue **byte-for-byte through
  0x104** — including the missing `movw r6,#583` hoist at normalised index 34 —
  but its pool drops to **24/31** (75.7%). The movw hoist and pool 31/31 are
  mutually exclusive under every ordering tried.
* Single-token hill climbs over the early block, settings tail and late block
  (insertion + pairwise swap), plus 400-eval simulated annealing over the whole
  order, all converge back to the baseline 80.5 (status early) or variant B 78.8
  (status late). The baseline is a strong local optimum.

## 1. Commands

```bash
cd /home/debian/GalaxyOnFire2/wt/w5415
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6     # RATCHET PASS
```

Fast per-variant scorer used for the search (`/dev/shm/w5415`, scratch only):
`compile.sh` is the exact repo compile line for `Globals.cpp`; `score.py`
reproduces the official `verify.py` fuzzy number and pool order from the
resulting `Globals.o`; `mk.py` rewrites only the `Globals::Globals()` body.

Gate at baseline (unchanged): `byte_exact 4500 / linked_exact 4500`,
`size-exact 4524`, `data 843/843`, imports 207, lint CLEAN, drift 0,
**RATCHET PASS**.

## 2. Ground truth (re-verified this pass)

Original `_ZN7GlobalsC1Ev` @ `0xe31e0`, 612 B, 31-word GOT pool @ `0xe33c8`:

```text
Canvas, lastSpace, lastStation, lcmf, lcmfc, options, recordHandler,
shipTemplate, gameText, font, fontAlien, fontLangSelect, rnd, globals,
layout, appManager, generator, hints, gameLoaded, gameSaving, initMem,
imageFactory, achievements, galaxy, ias, iaw, status, iat, iapn, items, ships
```

The first 7 words are the prologue hoist (`ldr rX,[pc,#N]` + `add rX,pc` at
`e31e8..e3202`). The original's machine store order (annotated from the `.so`
GOT relocations) is:

```text
Canvas(0x38), shipTemplate(0x5a), gameText(0x62), font(0x6a),
fontAlien(0x72), fontLangSelect(0x7a), rnd(0x82), globals(0x8a),
layout(0x92), appManager(0x9a), recordHandler(0x9e), generator(0xa8),
opt+12, opt+8, opt+44, opt+14, opt+48, opt+16, opt+64, opt+80, opt+57,
opt+53, opt+49, hints+4, hints+19, opt+63, opt+59, opt+78, opt+76, opt+72,
hints+0, gameLoaded(0x10c), gameSaving(0x114), lastSpace(0x118),
initMem(0x120), lastStation(0x124), lcmfc(0x12e), lcmf(0x138),
resWidth(0x16a), resHeight(0x17e), ias(0x18a), iaw(0x192), iat(0x1a0),
iapn(0x1a8), items(0x1b0), status(0x1b4), short60(0x1bc), galaxy(0x1c0),
achievements(0x1c6), imageFactory(0x1c8), ships(0x1cc), soundResources
```

## 3. Measured variants

All scores are the official `verify.py` fuzzy % on `_ZN7GlobalsC1Ev`
(`2·M/452`); `pool` is the LCS of the 31 GOT words. `movw@` is the normalised
index of `movw r6,#583` (original: 34).

| variant | score | M | pool | status slot | movw@ | notes |
|---|---|---|---|---|---|---|
| **baseline (HEAD)** | **80.5%** | **182** | **30/31** | 6 | 139 | best fuzzy |
| variant B: `recordHandler` last of early block, `status` after `lastStation` | 78.8% | 178 | **31/31** | 26 | 135 | only 31/31 order found |
| Q@9: variant-B early, `QUALITY` at late pos 9, `status` late | 75.7% | 171 | 24/31 | 26 | **34** | prologue byte-identical through 0x104 |
| Q@9 with `status` early | 73.0% | 165 | 24/31 | 6 | **34** | same prologue, pool worse |
| baseline + `QUALITY` at late pos 9 | 73.0% | 165 | 24/31 | 6 | **34** | movw hoist costs pool |
| baseline + `QUALITY` at late pos 10/11 | 71.7/72.1% | 162/163 | 23-24/31 | 6 | **34** | |
| settings tail in original machine order | 79.2% | 179 | 30/31 | 6 | 139 | `flag39,int35,int31,s1,f13,flag3f,int3b,flag4e,short4c,int48,s0` |
| same tail, variant-B early, `status` late | 77.4% | 175 | 31/31 | 26 | 135 | |
| original machine store order as source | 44.5–54.9% | 100–124 | 22–27/31 | — | — | scheduler reorders it |
| `recordHandler` at early pos 0..11, `status` late | 77.4–77.9% | 175/176 | 28–29/31 | 2 | 139 | only pos 12 (last) gives 31/31 |
| `status` at late pos 0..15 (variant-B early) | 56.6–78.8% | 128–178 | 28–31/31 | 3–30 | 135 | only pos 6 gives 31/31 |
| `imageFactory` moved near/at end of late block | 77.0/56.6% | 174/128 | 26/31 | 26 | — | pool breaks |
| duplicate late `status = 0` | 77.0% | 174 | 27/31 | — | — | stores merge, hoist set flips |

Search coverage (all negative):
* insertion hill climb over the 13-token early block, 11-token settings tail and
  16-token late block (status late) → variant B (78.8);
* pairwise-swap hill climb over the same blocks → variant B;
* insertion + pairwise-swap hill climb with status early → baseline (80.5);
* 400-eval simulated anneal over the whole order (status early) → baseline;
* 600-eval simulated anneal with `QUALITY` fixed at late pos 9 → Q@9 (75.7);
* hill climb on pool LCS with `QUALITY` fixed at pos 9 reached pool 30/31 with
  the *correct* hoist set (`Canvas,lastSpace,lastStation,lcmf,lcmfc,options,
  recordHandler`) but the late-block store order then scores only 61.1%.

## 4. What is proven vs hypothesis

**Proven (measured):**

1. The baseline body is 152/190 normalised code entries matched, pool 30/31.
   Variant B's body is 147/190, pool 31/31. The pool point (+1) does not pay
   for the net 5 lost body entries.
2. Variant B loses exactly 7 body entries and gains 2 (the late `status`
   store). The 7 lost entries are generic store blocks whose decimal
   `ldr rX,[pc,#N]` immediate shifts by 4 when `status` leaves the prologue:
   orig indices 65–68 (`appManager` block + `generator` load), 87 (`hints`
   load), 116 (`gameSaving` store), 147 (`iaw` load). `verify.py` compares the
   decimal `[pc,#N]` literally (only `0x...` hex operands are masked).
3. `movw r6,#583` is hoisted to normalised index 34 (right after the last use
   of r6, the `lcmfc` GOT load) **iff** the quality block is ISel-scheduled
   late enough — reproducible by putting `QUALITY` at late positions 9–12.
   At that point the prologue/early/settings block is byte-identical to the
   original through `0x104`.
4. Every attempt to combine the movw hoist with pool 31/31 fails: the hoist
   moves the late-block GOT load order, costing 6–7 pool words.
5. The original's machine store order is not a viable source order (44.5–54.9%)
   because the -Oz scheduler reorders independent stores and re-allocates the
   zero/address registers.

**Hypothesis (not proven):**

* The original source likely had `status = 0` late (after `iaw`) and
  `recordHandler = 0` early enough to be the 7th hoisted load. Variant B is the
  closest reproduction of that shape; its lower fuzzy score is a metric artifact
  of the missing 4-byte `movw r6,#583` hoist, not a semantic difference.
* The `imageFactory` 32-bit `ldr.w ip` / `str.w r2,[ip]` form in the original
  (ours is 16-bit `ldr r0` / `str r2,[r0]`) is a register-allocation consequence
  of the original's longer live range; no source-level way to force it was
  found.
