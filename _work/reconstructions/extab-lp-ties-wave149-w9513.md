# Extab LP-identity ties (wave 149, w9513): the tie needs P(autoTurret != nullptr)
# halved; every source spelling that halves it also moves `.text`

Status: **analysis only, no landed `src/` change** (new hypotheses tested, all
fail the byte-exact constraint). Worktree `wt/w9513`, base `fece77a64` (wave
148). Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0. `.ARM.extab` 1,309 differing bytes at exact 156,312 B.

Targets and unchanged metrics (raw same-address diff / instruction-index
alignment / extab bytes):

| fn | bodyraw | aligned | extab |
|---|---:|---:|---:|
| `Radar::Radar` (`_ZN5RadarC2EP5Level`, `0x12d484`) | **0** | **0** | **7** |
| `SpaceLounge::drawLounge` (`0x170d38`) | 176 | 42 | 14 |
| `ModStation::ModStation` (`0xd52d0`) | 43 | 12 | 2 |

`Radar::Radar` is the only one with bodyraw 0, but **both `C1` and `C2` are
byte-exact** in `report.json`, so any `.text` change costs `byte_exact -2`.
That is the hard constraint this wave ran into.

## 0. Why the tie is a two-condition problem (recap + new confirmation)

`MBPTRACE` on the landed source (`/tmp/llvm_android_702b/build2/bin/llc`,
`MBP_TRACE=1`):

```
CAND %bb.60  freq=519     <- %175 function-level cleanup pad
CAND %bb.87  freq=519     <- %372 enemyGuns+Image2D cleanup pad
CAND %bb.195 freq=8       <- machine min, the `delete` pad of `new Array<KIPlayer*>`
```

`%bb.195` is IR block `%771` (`landingpad; extractvalue; extractvalue; call
@_ZdlPv; br`), whose single predecessor is `%759` (first op `bitcast`, the
`new Array` bitcast). Its frequency is

```
freq(%771) = P(autoTurret != nullptr) * freq(%755) / 2^20
```

with `freq(%755) = 0.29888` and `P = 0.625` (pointer heuristic 20/32). The
scaled pad frequencies are `519.2697 * k` and `519.5021 * k` with
`k = 8 / freq(%771) = 0.625 / P`. The integer tie breaks iff
`floor(519.2697 k) < floor(519.5021 k)`, which happens for `k` in narrow
windows around the integers: `k = 2` → 1038.54 / 1039.00 (split), `k = 4` →
2077.08 / 2078.01 (split), `k = 5` → 2596.35 / 2597.51 (split). `k = 1`
(base), `1.25` (`P = 0.5`) and `2.5` (`P = 0.25`) all stay tied.

So a split needs `P(autoTurret != nullptr) ∈ {0.3125, 0.15625, 0.125, ...}`.
clang-7's `BranchProbabilityInfo` can only *multiply* heuristics
(`calcPointerHeuristics` 20/32, `calcZeroHeuristics` 20/32, a bare `br i1`
default 1/2, loop 124/128 & 4/128, cold-call 4/68, `__builtin_expect`
2000:1). There is no single source construct that yields 0.3125 — it is
`0.625 * 0.5`, i.e. **a second real condition**.

## 1. New hypothesis A: a second condition on the `autoTurret` guard

All variants below were measured on the real TU + gold link
(`measure.py` = raw same-address `.text` diff; `extab.py` = differing
`.ARM.extab` bytes for `0x12d484`):

| variant | extab | bodyraw | size |
|---|---:|---:|---|
| base | **7** | **0** | 2488 |
| `if (autoTurret != nullptr && this->players == nullptr)` | 41 | 176 | 2492 |
| `if (this->players == nullptr && autoTurret != nullptr)` | 41 | 176 | 2492 |
| nested `if (autoTurret) { if (this->players == nullptr) { … } }` | 41 | 176 | 2492 |
| `if (__builtin_expect(autoTurret != nullptr, 1) && this->players == nullptr)` | 41 | 176 | 2492 |
| `if (autoTurret != nullptr && Globals::status->getShip() != nullptr)` | 46 | 179 | 2496 |
| `if (autoTurret != nullptr && !Globals::status->inAlienOrbit())` | 46 | 179 | 2496 |
| `if ((unsigned)autoTurret > 2)` (single cmp, P = 0.5, k = 1.25) | 40 | 165 | 2488 |
| `if (autoTurret != nullptr && (unsigned)autoTurret > 2)` | 40 | 165 | 2488 |

The second condition *does* halve `P` (the MBP trace moves the min from 8 to
~4 as predicted), but it also adds a block and a new landing pad, so the LSDA
row structure changes and extab **rises** to 40/41/46. `(unsigned)autoTurret >
2` is the cheapest spelling (same instruction count, immediate `#2` instead of
`#0`) yet still costs 165 body bytes because `P = 0.5` reorders the machine
blocks of the tail.

**Conclusion:** the tie cannot be broken from source without moving `.text`.

## 2. New hypothesis B: `__builtin_expect` on the `labelStrings` / ternary guards

The `%175` pad's predecessors include the early `labelStrings` region, whose
weights are `1.0 / 0.5 / 0.25 / 0.125` (bare `br i1` bools default to 1/2).
New probes (all `Radar::Radar`):

| variant | extab | bodyraw |
|---|---:|---:|
| base | **7** | **0** |
| `expect(inAlienOrbit(), 1) && dlc1Won()` | 7 | 0 |
| `expect(inAlienOrbit(), 0) && dlc1Won()` | 88 | 1477 |
| `expect(dlc1Won(), 0) && !inEmptyOrbit()` | 84 | 1765 |
| `!expect(inEmptyOrbit(), 0)` | 7 | 0 |
| `!expect(inEmptyOrbit(), 1)` | 78 | 1799 |
| `expect(getIndex() == 101, 1)` | 136 | 1597 |
| `expect(getIndex() == 101, 0)` | 83 | 1842 |
| `expect(scanner != nullptr, 1)` (hasScanner19) | 7 | 0 |
| `expect(turret != nullptr, 1) && …` | 7 | 0 |
| `expect(getAttribute(50) != 0, 1)` | 7 | 0 |
| `expect(autoTurret != nullptr, 1)` | 10 | 6 |
| `expect(autoTurret != nullptr, 0)` | 56 | 167 |

`expect(inAlienOrbit(), 1)` is body-exact and moves `%175` from 519 to 684 —
the **wrong direction** (we need `%175 < %372`). Every variant that lowers
`%175` flips the `inAlienOrbit` block layout (1.5–1.8 KB). The `turret` /
`scanner` / `getAttribute(50)` expects are body-exact but have no effect on
either pad (their invokes unwind to `%746`, not `%175`/`%372`).

Combinations of body-exact expects (`alien+turret`, `alien+empty`,
`alien+turret+empty`) all stay at **extab 7 / body 0**.

## 3. New hypothesis C: `expect` on the playerGuns / enemyGuns loop bodies

Re-measured and extended:

| variant | extab | bodyraw |
|---|---:|---:|
| `expect((*playerGuns)[i]->isRocketGun(), 0)` | **3** | 116 |
| `expect((*playerGuns)[i]->isMineGun(), 0)` | **3** | 122 |
| `expect((*playerGuns)[i]->isBombGun(), 0)` | 60 | 1655 |
| `expect((*playerGuns)[i]->isRocketGun(), 1)` | 11 | 12 |
| `expect((*playerGuns)[i]->isBombGun(), 1)` | 11 | 12 |
| `expect((*playerGuns)[i]->isMineGun(), 1)` | 11 | 12 |
| `expect((*enemyGuns)[i]->isRocketGun(), {0,1})` | 57 / 11 | 1526 / 12 |
| `expect(i < playerGuns->size(), 1)` (loop cond) | 25 | 43 |
| `expect(i < enemyGuns->size(), 1)` (loop cond) | 26 | 43 |
| `rocket0 + expect(autoTurret != nullptr, 1)` | 6 | 122 |
| `mine0 + expect(autoTurret != nullptr, 1)` | 6 | 128 |
| `rocket0 + turret expect` | 3 | 116 |
| `rocket0 + alien expect` | 10 | 116 |
| `rocket0 + mine0` | 61 | 1638 |

`rocket0` / `mine0` **do** fix the LP swap (rows 1/3/11/13/15 → 2462, rows
19/21 → 2464, exactly the original) but invert the small `isRocketGun` if-body
layout (~116 B) and leave extab 3 (row 18 shifts, per wave 147). Body 116 >
0 ⇒ `byte_exact` would drop 4507 → 4505 (C1 + C2), a hard ratchet failure.
Not landable.

## 4. Row table (unchanged, `Radar::Radar`)

| row | orig | ours | scope |
|---:|---|---|---|
| 1 | `(68,310,2462)` | `(68,310,2464)` | `%175` function-level |
| 3 | `(388,14,2462)` | `(388,14,2464)` | `%175` |
| 11 | `(610,64,2462)` | `(610,64,2464)` | `%175` |
| 13 | `(710,6,2462)` | `(710,6,2464)` | `%175` |
| 15 | `(736,6,2462)` | `(736,6,2464)` | `%175` |
| 19 | `(892,6,2464)` | `(892,6,2462)` | `%372` enemyGuns |
| 21 | `(998,624,2464)` | `(998,624,2462)` | `%372` |

`2462 = 0x12de22`, `2464 = 0x12de24`; both are `b.n 0x12de28` trampolines to
the member-`String` dtor cleanup. Only the row→trampoline assignment differs.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w9513
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
python3 /dev/shm/w9513/measure.py _ZN5RadarC2EP5Level   # bodyraw
python3 /dev/shm/w9513/extab.py                          # extab diffbytes
```

MBP trace (instrumented clang, not committed):

```sh
MBP_TRACE=1 /tmp/llvm_android_702b/build2/bin/llc \
  -mtriple=armv7-none-linux-androideabi16 -mattr=+neon -float-abi=soft -Oz \
  -debug-only=block-freq Radar.ll -o /dev/null
```

Scratch (not committed), `/dev/shm/w9513/`: `tryv.py`..`tryv5.py`,
`measure.py`, `extab.py`, `blocks.py`, `ll.sh`, `Radar.base.cpp`, `Radar.ll`,
`mbp2.txt`.

## 6. Open questions

1. A source spelling that halves `P(autoTurret != nullptr)` **without adding an
   instruction**: `0.625 * 0.5` needs a second condition; the only zero-cost
   candidate is a bool-valued call whose result is already live (none exists in
   this tail).
2. A spelling that lowers `freq(%755) = 0.29888` (the loop-exit frequency)
   without moving the `asteroidSlots` / `enemyGuns` loops — the loop header
   frequency (9.564) is renormalised, so `__builtin_expect` on the loop
   condition leaves the exit frequency at the entry frequency.
3. The original's PGO weights (`.profdata`/`.profraw`) would settle this
   directly; the heuristic build cannot reach `P = 0.3125`.
