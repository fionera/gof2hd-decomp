# Radar::draw pass (wave122, w7209) — literal-pool/block-placement sweep, no source win

**Status:** research note, 2026-11-06. Worktree `wt/w7209`, based on `master`
`29cd275f6` (wave-121 final). **No source change landed**; this commit contains
this note only. The target remains the pristine baseline
`src/game/weapons/Radar.cpp`.

Target `_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).
Original 9908 B / 3492 normalised instructions; ours 9908 B / 3484.
Baseline unrounded `difflib.SequenceMatcher(None, on, un, autojunk=False).ratio()`
= **74.541284 %** (official `verify.py` display 74.5), `M = 2600` matching
instructions. Gate at start and end: `RATCHET PASS`, byte **4506**, linked
**4506**, data 843/843, imports 207, lint CLEAN, drift 0.

## 1. Commands

```sh
cd /home/debian/GalaxyOnFire2/wt/w7209
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6        # RATCHET PASS
cmake --build cmake-build-match --target drift -j6        # 0 fields
cmake --build cmake-build-match --target lint             # CLEAN
```

Scratch harness `/dev/shm/w7209/score.py` mirrors
`tools/verify/verify.py`'s `disassemble`/`reconcile` and reproduces the
official 74.541284 % exactly. Per-variant loop: edit `Radar.cpp`,
`cmake --build cmake-build-match --target link_so -j6`, re-score.

## 2. Literal-pool content and first-use order are already identical

The function has **45 pool words** in both binaries, in the **same symbol
order**. The first island is the same five words in the same order:

| word | symbol | first use |
|---|---|---|
| `0x000e1c9a` | `__stack_chk_guard` | `12e064` |
| `0x000e2166` | `Globals::Canvas` | `12e084` |
| `0x000e20bc` | `Globals::status` | `12e09a` |
| `0x000e2224` | `PaintCanvas::DrawImage2D` | `12e1da` |
| `0x000e1fda` | `Globals::font` | `12e2b4` |

The normalised pool sequences are identical (45 entries each), including the
three `str:: ` / `Globals::layout` / `String::~String` words, the
`0x46bb8000 / 0x476a6000 / 0xc6bb8000 / 0x42c80000` float island, and the
`pool:addr` / `pool:pic:local` tail. There is **no missing source constant or
table**. Only the *addresses* of the pools differ:

* original: `12e340, 12e79c, 12ebf0, 12f200, 12f794, 12faf8, 12feac, 1302dc`
* ours:     `12e2e0, 12e7a8, 12ebaa, 12f21c, 12f780, 12fb14` (6 pools)

## 3. First-pool displacement is a symptom of the `status` load

Original prologue tail:

```
12e2de b.n   12e2fc            ; waypoint onScreen block skips scannerAvailable
12e2e0 ldrb  r0,[r4,#427]      ; scannerAvailable (branch target of cbz at 12e29a)
...
12e2fc mov   r1,r4             ; otherLock chain
12e2fe ldr.w r0,[r1,#12]!      ; lockedAsteroid
12e304 cbnz  r0,12e312
12e306 ldr   r0,[r4,#56]       ; lockedGasCloud
12e308 cbnz  r0,12e312
12e30a ldr   r0,[r4,#28]       ; field_0x1c
12e30e beq.w 12ebc2            ; cold: otherLock = field_0x14 != 0
12e312 movs  r5,#1             ; hot: otherLock = 1
12e314 ldr.w r0,[r4,#312]      ; landmarks
12e318 cbz   r0,12e372
12e31a ldr   r6,[sp,#156]      ; &Globals::status  <- AFTER chain + landmarks check
12e31c ldr   r0,[r6,#0]        ; Globals::status
...
12e340 <first literal pool>
```

Ours baseline loads `status` at `12e310`, before the chain:

```
12e2de b.n   12e310
12e2e0 <first literal pool>    ; 12e2e0 is a cold gap -> island slot
12e2f4 ldrb  r0,[r4,#427]      ; scannerAvailable moved after the pool
12e2f8 cbz   r0,12e310
...
12e310 ldr   r5,[sp,#156]      ; &Globals::status
12e312 mov   r1,r4
12e31a ldr   r6,[r5,#0]        ; Globals::status
12e31c cbnz  r0,12e32a
...
12e32a mov.w r9,#1             ; hot: otherLock = 1
```

Moving the five-word island alone to the original slot is worth only
**+0.14 %** (74.541284 -> 74.6846), so the pool address is not the dominant
residual.

## 4. The chain shape is coupled to the live `status` local

The original chain is the straight form
`if (a != null || b != null || c != null) otherLock = 1; else otherLock = d != 0;`
(`cbnz a`, `cbnz b`, `cmp c,0; beq cold`, `movs r5,#1`). With the live
`Status *status = Globals::status;` local the baseline reproduces that shape
(except `r9` for `otherLock` and the cold block at the function end). Removing
the local (using `Globals::status` directly) flips the chain to
`cbz a, continue; mov r9,#1; ... itt eq; ...` and scores only **72.266017 %**.
Declaring `Status *status;` before and assigning after the chain also flips it
(73.284630 %).

27 chain-expect combinations (outer expect × per-term expects) were measured.
The best is the baseline shape (74.541284 %), also reproduced by
`__builtin_expect(gasCloud == nullptr, 1)` + `__builtin_expect(field_0x1c == nullptr, 0)`
with no outer expect. No combination exceeded baseline.

Direct-globals OR/AND forms with per-term expects top out at **73.115506 %**
(`a || __builtin_expect(b,1) || c`), still below the local-status baseline.

## 5. `status` declaration-position sweep

| variant | size | raw % |
|---|---:|---:|
| baseline (`Status *status` before chain) | 9908 | **74.541284** |
| `Status *const status` / `auto *status` / `Status &status = *Globals::status` | 9908 | 74.541284 |
| `Status *status;` before, `status = Globals::status;` after chain | 9920 | 73.284630 |
| guard uses `Globals::status`, no local | 9912 | 72.266017 |
| `Status *status` after `SetColor` | 9912 | 71.213641 |
| `Status *status` after `lockConfirmed = 0` | 9924 | 70.915079 |
| `Status *status` before the waypoint block | 9884 | 69.758759 |
| `Status **statusPtr = &Globals::status` | 9912 | 72.266017 |
| local `status` + `status = Globals::status` after chain | 9920 | 73.284630 |
| `__builtin_expect(status != nullptr,1)` in guard, direct calls | 9904 | 71.981646 |

The only source shape that gives both the straight chain and the original
`status`-load position would need the local to be live across the chain
*without* being loaded before it; no spelling found this wave achieves that.

## 6. Block-placement sweep (planet-lock tail vs landmark loop)

Original layout: planet block -> cold autopilot -> planet-lock tail
(`12e748`) -> pool -> landmark loop (`12e7a8`) -> landmark tail -> cold
`otherLock` (`12ebc2`) -> pool -> enemy section.

Ours baseline: planet block -> cold autopilot -> invented recompute
(`12e722`) -> landmark loop (`12e776`) -> planet-lock tail (`12ebd4`) ->
enemy section; cold `otherLock` at `1305fa`.

Variants measured:

| variant | size | raw % |
|---|---:|---:|
| baseline | 9908 | **74.541284** |
| remove invented recompute | 9832 | 63.451850 |
| remove recompute + guard expect 0 | 9840 | 56.806095 |
| remove recompute + guard expect 1 | 9752 | 64.867206 |
| planet tail moved before recompute | 9928 | 71.451082 |
| planet block moved before landmark block | 9808 | 56.825717 |
| planet tail `__builtin_expect(...,1)` | 9904 | 71.791195 |
| planet tail `__builtin_expect(...,0)` | 9924 | 61.450436 |
| landmark guard `__builtin_expect(...,0)` | 9936 | 56.742457 |
| landmark guard `__builtin_expect(...,1)` | 9812 | 67.328156 |
| landmark tail `__builtin_expect(...,0/1)` | 9916/9912 | 63.040366 / 70.996416 |
| scanner/gas/asteroid expects (0/1) | — | 26.9–73.5 |

Removing the recompute does move the planet-lock tail to `12e758` (original
`12e748`), but the landmark loop is then placed at `12f466` (after the enemy
section) and the score collapses. The recompute is load-bearing for the
landmark-loop placement, but it also pushes the planet-lock tail after the
landmark loop.

## 7. `vpush` / float live-range sweep

The original saves `d8-d13`; the baseline saves `d8-d11`. Only shapes that keep
the enemy crosshair floats live across the `mi`/`VectorLength` calls restore
`d8-d13`:

| variant | size | raw % |
|---|---:|---:|
| baseline | 9908 | **74.541284** |
| distance-first `inSight` (float locals) | 9896 | 68.053364 |
| distance-first + direct `Globals::status` | 9904 | 68.511615 |
| bounds-first + live `crossX/crossY` in the taken arm | 9936 | 72.623139 |
| `crossX/crossY` at top + `inSight && crossX==crossX` | 9872 | 64.279551 |
| `crossX/crossY` before the enemy loop | 9880 | 62.661313 |
| `crossX/crossY` at top, bounds-first | 9836 | 62.658228 |

`vpush {d8-d13}` is reachable, but every such shape re-rolls the frame
(`sub sp,#304`, `mode` slot) and costs 2–12 points.

## 8. Neutral-tweak combinations

175 combinations (singles/pairs/triples) of the previously neutral tweaks
(`Status *const`, `campaign` uint32_t, `onPlanetCourse` int, `field_0x14 !=
nullptr`, `uint8_t notOtherLock/notTurretMode`, unfolded `i==1||i==3`, planet
loop `int i`, `int size`, `x = x + mode`, `onScreen != 0`) were measured. **None
exceeded 74.541284 %**; every combination stayed at the baseline score or
below.

## 9. Remaining root cause

The function is a tuned local optimum. The residual is dominated by two
coupled compiler decisions that source-level reshaping could not steer
independently:

1. **`Status *status` load position** — the live local gives the original
   straight `otherLock` chain but forces `&Globals::status` to be loaded before
   the chain (`12e310`), making `12e2e0` a cold gap and moving the first pool
   from `12e340` to `12e2e0`. Removing the local moves the pool but flips the
   chain (`cbz`/`itt eq`) and costs ~2.3 points.
2. **Cold-block order** — original places the planet-lock tail before the
   landmark loop and the first cold `otherLock` block at `12ebc2`; the
   baseline places the landmark loop first and the cold block at `1305fa`.
   The invented planet-course recompute is what keeps the landmark loop near
   `12e776`, but it also pushes the planet-lock tail to `12ebd4`.

The literal pool itself is already correct in content and first-use order;
only its address (a consequence of (1)) differs. No source-only edit found this
wave improves the baseline.

## 10. Artifacts

Scratch (not committed, `/dev/shm/w7209`): `Radar.base.cpp` (pristine source),
`score.py`, `runs.txt`, `grid.py`, `chaingrid.py`, `expectgrid.py`,
`norec_grid.py`, `search.py`. The analysis is self-contained in this note;
the scratch files are reproducible from the commands in §1.
