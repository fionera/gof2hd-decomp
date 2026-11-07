# Radar::draw pass (wave 128, w7650) — loop nesting / block order / literal-pool sweep, no source win

**Status:** research note, 2026-11-07. Worktree `wt/w7650`, based on current
`master` (`fee528cbe`). **No source change landed**; this commit contains this
note only. The target remains the pristine baseline
`src/game/weapons/Radar.cpp`
(`sha256 878a70b21ffaf509ff68130767f6630cc7a0895958fe1eed12ba0b7413bb522c`).

Target `_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).
Original 9908 B / 3492 normalised instructions; ours 9908 B / 3483.
Baseline unrounded `difflib.SequenceMatcher(None, on, un, autojunk=False).ratio()`
= **75.268817 %** (official display 75.3). Gate at start and end:
`RATCHET PASS`, byte **4506**, linked **4506**, data 843/843, imports 207,
lint CLEAN, drift 0, avg 99.94.

Read with `radar-draw-wave127-w7628.md`, `radar-draw-wave126-w7606.md`,
`radar-draw-planet-loop-wave125-w7512.md`, `radar-draw-body-wave122-w7210.md`.
This wave tested the assignment's stated hypothesis directly: **planet/ship
loop nesting, block order, and literal-pool/register allocation**. It adds
three pieces of evidence the earlier notes did not have: (a) the original's
first `otherLock` guard is a `cbnz` chain and the compiler canonicalises every
source spelling of it back to our `cbz`/`itt` shape; (b) the original's
landmark guard outlines the `==111 && campaign>93` check, ours keeps it inline;
(c) the original's ship-loop `inSight` test schedules `VectorLength` before the
integer hit test, but reordering the source condition regresses by 10 points.

## 1. Commands

```sh
cd /home/debian/GalaxyOnFire2/wt/w7650
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6     # ~0.9 s per variant
cmake --build cmake-build-match --target check -j6        # RATCHET PASS
```

Per-variant loop: edit `Radar.cpp`, `link_so`, score the single symbol with a
scratch harness mirroring `tools/verify/verify.py`'s
`disassemble`/`reconcile` (reproduces 75.268817 % exactly). Scratch in
`/dev/shm/w7650` (not committed): `Radar.base.cpp`, `score.py`, `harness.py`,
`dump3.py`, `opdiff.py`, `regiondiff.py`, `base_o.txt`/`base_u.txt`
(per-instruction normalised dumps), `ghidra_radar.txt`.

## 2. Literal pools (measured, baseline)

The pool **words are identical and in the same order**; only the GOT addresses
and the pool positions differ.

| pool | original | ours | words |
|---|---:|---:|---|
| first | `12e340` (fn `0x2f0`) | `12e310` (fn `0x2c0`) | 5: `__stack_chk_guard`, `Globals::Canvas`, `Globals::status`, `PaintCanvas::DrawImage2D`, `Globals::font` |
| second | `12e79c` (fn `0x74c`) | `12e7f4` (fn `0x7a4`) | 3: `StarSystem::orbitPlanetIndex`, `PlayerEgo::crosshairPos`, `Globals::font` |

The first pool sits inside our first `otherLock` block (`12e2b4`–`12e2e4`),
splitting it with a `b.n` over the pool; the original's first `otherLock`
block is contiguous (`12e2b4`–`12e2f0`) and the pool follows it. The second
pool sits before the original's landmark setup (`12e7a8`) because the setup
falls through into the top-tested loop head (`12e7d4`); ours puts the pool
after the setup because our setup ends in `b.n 12eb48` (the rotated loop
condition) and the pool is emitted after that water block.

## 3. Block map (baseline, re-measured from the dumps)

| block | original | ours |
|---|---:|---:|
| first `otherLock` | `12e2b4`–`12e2f0` (`cbnz` chain) | `12e2b4`–`12e2e4` (`cbz`+`itt`, pool in middle) |
| landmark guard 109/110 | inline `12e2c2`–`12e2ec` | inline `12e2e8`–`12e322` |
| landmark guard 111/campaign | outlined `12e304`–`12e320` | inline `12e30a`–`12e322` |
| campaign `isTarget` | `12e424` reloads `stations->data()[i]` into `sl`, restores `sl` from `[sp,#116]` | `12e3??` compares `stationId` in `r8` directly |
| planet-lock tail | `12e748` | `12e776` |
| landmark setup | `12e758`–`12e782` | `12e774`–`12e7a2` |
| landmark loop head | `12e784` (top-tested) | `12eb48` (bottom-tested; setup branches to it) |
| landmark body | `12e790`–`12e9c8` | `12e7b0`–`12eaf4` |
| enemies | `12ec24` | `12ec28` |

The original's ship-loop `inSight` test (`12ef58`–`12ef6e`) computes
`AEMath::operator-` and `VectorLength` **before** the integer hit test; ours
computes the integer hit test first and `VectorLength` last (`12ef5e`–`12ef82`).
That is a scheduling/layout difference, not a source-order difference: putting
`VectorLength` first in the source condition regresses badly (see §4).

## 4. Variants measured this wave

All rows are single edits on the pristine baseline unless noted. `usize` is the
emitted function size; `raw %` is the unrounded ratio.

| variant | usize | raw % |
|---|---:|---:|
| **baseline** | 9908 | **75.268817** |
| first `otherLock` positive OR, plain | 9908 | 74.093190 |
| first `otherLock` positive OR, outer `expect(...,1)` | 9908 | 75.240143 |
| first `otherLock` positive OR, inner `expect` | 9892 | 72.862880 |
| first `otherLock` ternary | 9908 | 74.093190 |
| first `otherLock` nested ifs | 9908 | 74.093190 |
| first `otherLock` operand permutations (A,G,C)/(A,C,G) | 9908 | 74.093190 |
| first `otherLock` operand permutations (G,A,C)/(G,C,A) | 9896 | 72.033290 |
| first `otherLock` operand permutations (C,G,A)/(C,A,G) | 9928 | 67.584229 |
| landmark loop size cached (`unsigned`/`int`) | 9900 | 71.416284 |
| landmark loop `(int) size()` cast | 9908 | 75.268817 (tie) |
| landmark guard inverted (`i != 2 && ptr && visible`) | 9908 | 75.268817 (tie) |
| landmark guard split into three `continue`s | 9908 | 75.268817 (tie) |
| landmark loop local `KIPlayer *landmark` | 9816 | 68.899109 |
| landmark body trailing `continue;` / `else continue` | 9908 | 75.268817 (tie) |
| planet loop local `KIPlayer *planet` | 9908 | 75.268817 (tie) |
| planet `stationId` before `update()` | 9908 | 75.010753 |
| planet `stationId` before null check | 9908 | 75.010753 |
| planet `currentStation` declared first | 9860 | 72.826556 |
| planet `warpGate` declared first | 9860 | 70.295176 |
| campaign `isTarget` operands swapped | 9908 | 75.268817 (tie) |
| campaign `isTarget` inlined into the `if` | 9900 | 72.018349 |
| campaign `int targetStation` local | 9908 | 75.268817 (tie) |
| campaign `stations->data()[i]` (`camp_reload`) | 9904 | 73.444221 |
| campaign reload, operands swapped | 9912 | 71.844104 |
| campaign reload via local / comma expression | 9904 | 73.444221 |
| campaign reload + `stationId` before `update()` | 9904 | 73.386866 |
| campaign reload via alias pointer | 9904 | 73.444221 |
| campaign reload via `volatile` alias | 9912 | 72.972198 |
| campaign assignment expression (`stationId = stations->data()[i]`) | 9916 | 73.613301 |
| campaign reload + extra reload | 9924 | 72.104358 |
| `marked` hoisted before the planet loop | 9876 | 71.931084 |
| `textOffset` hoisted before the planet loop | 9928 | 71.912694 |
| both hoisted | 9868 | 71.418307 |
| `marked` address-taken (`int *markedPtr = &marked`) | 9908 | 75.211470 |
| landmark guard split (`!(109\|\|110) && !(111&&campaign)`) | 9908 | 75.268817 (tie) |
| landmark guard nested `if` | 9908 | 75.268817 (tie) |
| planet condition `campaign > 1` | 9908 | 75.268817 (tie) |
| planet condition `planetTargets` first | 9900 | 70.606800 |
| planet condition `!alienOrbit` first | 9872 | 47.861039 |
| planet condition `!isDockingToPlanet()` first | 9896 | 70.952928 |
| `stationId`/`currentStation`/`warpGate` as `unsigned`/`uint32_t` | 9908 | 75.268817 (tie) |
| ship `inSight`: `VectorLength` first | 9908 | 64.684762 |
| ship `inSight`: `VectorLength` first, x/y grouped | 9896 | 68.751793 |
| ship `inSight`: `VectorLength` first + `else` | 9892 | 68.331181 |

The baseline is a strong local optimum. Every shape that changes the first
`otherLock` block, the landmark loop rotation, or the campaign reload re-rolls
the planet-loop register split and loses 2–10 points.

## 5. Root cause (unchanged, now with pool evidence)

The residual is a coupled fixed point:

1. The original's first `otherLock` is a `cbnz` chain (positive OR) and its
   landmark guard outlines the `==111 && campaign>93` check; ours canonicalises
   every source spelling to `cbz`/`itt` with the 111 check inline. This moves
   the first literal pool from `12e340` to `12e310`.
2. The original's campaign block reloads `stations->data()[i]` into `sl` and
   restores `sl` from `[sp,#116]`; writing that source shape (`camp_reload`)
   makes clang spill `stations` to `[sp,#148]` and move `stationId` to `r9`,
   `currentStation` to `r6`, so the whole loop re-rolls (9904 B, 73.44 %).
3. The original's landmark loop is top-tested; ours is bottom-tested. The
   setup therefore ends in `b.n` and the second pool lands after the setup
   (`12e7f4`) instead of before it (`12e79c`). No loop spelling tried
   (`while`, `!=`, signed cast, cached size, guard inversion, separate
   `continue`s, trailing `continue`) un-rotates it.
4. The ship-loop `inSight` block is scheduled differently (`VectorLength`
   before the integer hit test in the original), but source-order changes
   regress by 7–10 points, so the scheduler is reacting to register pressure,
   not to the `&&` order.

A real win needs a source shape that simultaneously (a) keeps the first
`otherLock` block contiguous, (b) keeps the single campaign reload with
`stations` in `sl`+spill, and (c) un-rotates the landmark loop. None of the
spellings tried in this wave reached that fixed point; the baseline remains
the best `difflib` score at 75.268817 %.

## 6. Gate / section deltas (baseline, unchanged)

```
compared 4524  avg 99.94%  byte_exact 4506  linked_exact 4506  fuzzy_perfect 4506
stub_zero_size 0  orig_zero_size 0  missing 0 (wrong_type 0  absent 0)  extra 0
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Section deltas vs the original: `.text` 85,475 differing bytes (size/base
exact), `.ARM.extab` 1,433, `.note.gnu.build-id` 20. `.rodata`, `.data`,
`.bss`, `.got`, `.ARM.exidx`, `.rel.plt`, `.rel.dyn`, `.dynsym`, `.dynstr`,
`.hash` are byte-identical.
