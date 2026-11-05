# Globals::Globals C1/C2 — wave 114 (w6207)

Status: analysis only, **no source change landed**. Worktree `wt/w6207`, branch
`wave114-w6207`. Target `_ZN7GlobalsC1Ev` / `_ZN7GlobalsC2Ev`
(`src/game/core/Globals.cpp`), 612 B.

## TL;DR

The wave-113 baseline is the best **pool-faithful** shape found: fuzzy
**78.7611 %** (M 178), GOT pool **31/31**, `movw r6,#583` at normalised index
**34**, size 612 B. Exhaustive and heuristic searches over the early-block and
late-block statement orders, the settings-block order, the `secondary`
declaration position, and several source reformulations found **no variant that
raises the fuzzy score while keeping both structural properties**. The only
higher-scoring shape is the older tuned **80.5 %** variant (pool 30/31,
`movw`@139), which trades one pool entry and the `movw` hoist for fuzzy
alignment; wave 113 deliberately chose the pool-faithful shape.

## Original vs ours (normalised stream, index by index)

The first **96** normalised instructions match exactly. The remaining
differences are three:

1. **Early store order** (orig idx 96-103):
   `E F H A B C D G` =
   `hints+4`, `hints+0x13`, `options+0x3f`, `options+0x3b`, `options+0x4e`,
   `options+0x4c`, `options+0x48`, `hints+0`.
   Ours is `A B E F C G ... D ... H` (D and H sink past the late-block start).
2. **`imageFactory` address lifetime**: the original loads
   `&Globals::imageFactory` into `ip` at idx 128 (`ldr.w ip,[r0]`) and keeps it
   live until the store at idx 181 (`str.w r2,[ip]`). Ours stores imageFactory
   early and rematerialises the address
   (`ldr r0,[pc]/add r0,pc/ldr r0,[r0]/str r2,[r0]`).
3. **`resHeight` temp register**: the original uses `r0`
   (`movw r0,#513` @129, `movwge r0,#730`, `movwls r0,#365`,
   `str r0,[r1,#88]`); ours uses `r6` because `r0` holds the rematerialised
   imageFactory address.

Original store order (objdump): `Canvas, shipTemplate, gameText, font,
fontAlien, fontLangSelect, rnd, globals, layout, appManager, recordHandler,
generator, this->field_0x0, glowR/glowG/tintR/tintG (vst1), options+0x0c,
colorR/G/B, tintB, brightness, contrast, options+0x0e, options+0x30,
options+0x10, options+0x40, options+0x50, options+0x39, options+0x35,
options+0x31, hints+4, hints+0x13, options+0x3f, options+0x3b, options+0x4e,
options+0x4c, options+0x48, hints+0, gameLoaded, gameSaving, lastSpace,
initMemoryWarning, lastStation, lcmfc, lcmf, resWidth, resHeight, iAScore,
iAWave, iAType, iAPlayerName, items, status, options+0x60, galaxy, achievements,
imageFactory, ships, soundResources`.

Original GOT pool order: `Canvas, lastSpace, lastStation, lcmf, lcmfc, options,
recordHandler, shipTemplate, gameText, font, fontAlien, fontLangSelect, rnd,
globals, layout, appManager, generator, hints, gameLoaded, gameSaving,
initMemoryWarning, imageFactory, achievements, galaxy, iAScore, iAWave, status,
iAType, iAPlayerName, items, ships`.

## Measurements (all scratch under /dev/shm/w6207)

| variant | pct | pool | movw@ | size |
|---|---:|---:|---:|---:|
| **committed baseline (wave 113)** | **78.7611** | **31/31** | **34** | 612 |
| old 80.5 shape (`Globals.805.cpp`) | 80.5 | 30/31 | 139 | 612 |
| natural/original late order | 49.1 | 23/31 | – | 612 |
| natural late + `EFHABCDG` early | 54.9 | 23/31 | – | 612 |
| full store-order-as-source | 52.7 | 18/31 | – | 612 |
| header (.bss) declaration order | 34.4 | 20/31 | – | 500 |

Search results:

- **Exhaustive 40,320 early-block permutations** (baseline late order, full
  compile+gold link): best **78.8**, 330 permutation ties. No permutation
  reproduces the original early store order; the maximum early-store LCS vs the
  original is **4** (`E F H G`), because the scheduler always groups the three
  `hints` stores.
- Exhaustive early-permutation sweep with the *natural* (original) late order:
  best **56.6**, pool 23.
- 4,000 random late-block shuffles: best **73.9**.
- GA over the full 28-unit order (256 pop, 60 gens): best **78.8**.
- GA seeded with the 805 variant over a (pre, early, late) 3-part genome:
  best **80.5** (the 805 shape).
- Flat 42-unit GA seeded with both the true baseline and the 805 shape,
  fitness = (pool 31 && movw 34, pct), 60 gens: converged to the baseline
  **78.8** at gen 0, never improved.
- Flat 42-unit GA, fitness = (pct, pool), 80 gens: converged to the 805 shape
  **80.5**, never improved.
- Single-swap + single-move neighbourhood of the late block (570 variants):
  best **78.8**; only 2 pool-31/movw-34 ties (baseline and
  `L2,L3,L4,L5,L1,...`).
- `imageFactory` (L5) position sweep: pos 4 (baseline) = **78.8**, pool 31,
  movw 34; pos 0-3 = 77.4-78.3 pool 30; pos 5-8 = 78.3 pool 30; pos 9-11 =
  77.4-77.9 pool 29; pos 12+ = 74.3/54.9 pool 26, no movw.
- Settings-block (`O1..O19`) GA with the true baseline seed: **78.8**.
- `secondary` declaration position sweep (0-18) with early `EFHABCDG` and
  `ABCDEFGH`: 77.9 / 78.8, identical everywhere (the compiler CSEs the hints
  address).
- Struct reformulations (packed struct with real int16/int32 members,
  reference instead of pointer, direct `hints` access) and quality-block
  reformulations (separate `v58`, direct field stores, `v54` hoisted): all
  **78.8**.

## Conclusion

The wave-113 baseline is kept. The remaining schedule difference is a post-RA
scheduler artifact driven by whole-function register allocation: no source
order in the explored space reproduces the original's interleaved early store
order (`E F H A B C D G`) while retaining the 31/31 GOT pool and the
`movw #583` hoist. The 80.5 % shape remains the best *fuzzy* shape but is not
pool-faithful.

## Gate

`cmake --build cmake-build-match --target check -j6` (unchanged source):
`byte 4500 / linked 4500 / data 843 / imports 207 / lint CLEAN / drift 0`,
`RATCHET PASS`; 27 sections, file size 2,187,328 B, `.ARM.extab` 0x26298.
