# PGO profile derivation from the original block layout (wave 148, w9416)

Status: **tool + analysis, no landed `src/` change** (the best variant is still
the unprofiled build). Worktree `wt/w9416`, base `cbe3b5f7f` (wave 147).
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0.

Target: `Radar::Radar` (`_ZN5RadarC2EP5Level`, `0x12d484`, 2488 B) — body
byte-exact, `.ARM.extab` record 7 bytes swapped (LP 2462/2464).

## 0. Bottom line

1. **New tool** `_work/reconstructions/pgo_layout_w9416.py` derives the
   per-block frequency vector of a function from its (original) block layout:
   it maps each frontend PGO counter to its IR basic block, runs
   `llc -debug-only=block-freq` on the canonical IR, and reads the integer
   block-frequency vector in RPO order. The RPO order is identical for the
   instrumented and canonical IR (instrumentation adds no CFG edges), so the
   counter→frequency mapping is position-based, not name-based.
2. **The derived vector for `Radar::Radar` is**
   `[44907140, 22453377, 22453281, 5613288, 11226586, 22453195, 28066223,
   4420364, 132749082, 126591288, 117919719, 21260078, 174406732, 168956522,
   13421933, 5033220, 13421859, 8388646, 13421847, 13421798]`.
3. **Replaying it with `-fprofile-instr-use` does not reproduce the body.**
   The function grows to 2544 B (+56) and the raw same-address diff is 2281
   (extab record grows to 272 B). The reason is structural: a profile attaches
   `!prof` to *every* branch, whereas the unweighted build is driven by
   `BranchProbabilityInfo`'s heuristics; the two do not agree even when the
   frequencies are matched. A single `__builtin_expect` (one `!prof` site)
   costs only ~116 B (w9317 §7.1); a full profile costs ~2000 B.
4. **No profile variant zeroed the extab while keeping the body exact.** The
   best body diff seen was 1664 B (RPO shift −2), still with a 256 B extab
   record. The base (unprofiled) build remains the best measured variant:
   `extab 7, body 0`.

## 1. Tool

```sh
python3 _work/reconstructions/pgo_layout_w9416.py \
  --src src/game/weapons/Radar.cpp --func _ZN5RadarC2EP5Level \
  --addr 0x12d484 --dump                 # counter -> block -> frequency
python3 _work/reconstructions/pgo_layout_w9416.py ... --profile-vector --measure
python3 _work/reconstructions/pgo_layout_w9416.py ... --profile 8=1 13=1 --measure
```

It uses `ninja -t commands` for the exact TU compile line, the NDK r18b
`llvm-profdata`, and the coordinator's instrumented LLVM 7 `llc`
(`/tmp/llvm_android_702b/build2/bin/llc`) only as a `-debug-only=block-freq`
oracle. `--measure` gold-links with the checked-in link command and compares
the function's `.ARM.extab` record.

## 2. Counter map (Radar::Radar, 20 counters)

| counter | source | role |
|---:|---|---|
| 0 | 74 | entry |
| 1 | 142 | `inAlienOrbit()` ternary (String 415) |
| 2 | 144 | `getStation()->getIndex() == 101` ternary |
| 3,4,5 | 148 | `inAlienOrbit && dlc1Won && !inEmptyOrbit` chain |
| 6 | 156 | `playerGuns != nullptr` |
| 7 | 157 | playerGuns loop |
| 8,9,10 | 158/161/164 | `isRocketGun` / `isBombGun` / `isMineGun` |
| 11 | 170 | `enemyGuns != nullptr` |
| 12 | 171 | enemyGuns loop |
| 13 | 172 | enemyGuns `isRocketGun` |
| 14 | 246 | `asteroidSlots = new int[5]` |
| 15 | 257 | `turret != nullptr` |
| 16 | 273 | `autoTurret` |
| 17,18 | 280 | `autoTurret != nullptr` |
| 19 | 286 | end |

## 3. Measured variants (extab = `.ARM.extab` record diff for 0x12d484;
body = raw same-address `.text` diff; align = instruction-index diff)

| variant | extab | body | size |
|---|---:|---:|---:|
| base (no profile) | **7** | **0** | 2488 |
| flat profile (all counters 1) | 256 | 2006 | 2496 |
| flat profile (all counters 1000) | 256 | 2003 | 2496 |
| derived block-frequency vector | 256 | 2281 | 2544 |
| derived vector, RPO shift −2 | 256 | 1664 | 2508 |
| derived vector, RPO shift −1 | 256 | 2013 | 2508 |
| derived vector, RPO shift +1 | 256 | 2026 | 2528 |
| derived vector, RPO shift +2 | 256 | 2031 | 2528 |
| single-counter sweep c0..c19 ∈ {1,100,100000} | 256 (all) | 1825–2284 | 2496–2584 |

Every profile changes the `.text` layout (the function is never 2488 B again),
so the 7 extab bytes cannot be isolated from a body regression with this
mechanism.

## 4. Interpretation

`Radar::Radar`'s body is byte-identical to the **unweighted** build. If the
original had been compiled with `-fprofile-instr-use`, the `!prof` metadata
would have been present on every branch, and (as the table shows) any such
metadata changes the `-Oz` block layout by ~2 KB. The original's body therefore
looks like a **heuristic (no-profile) build**, not a `-fprofile-instr-use`
build. The 7-byte LP swap is then an artefact of a *different* tie-break, not
of a recoverable profile:

* the two pads truncate to 519/519 because `Scaled64::toInt()` truncates
  519.2697 and 519.5021 (w9317 §7);
* the original's tie-break put the function-level pad (`%175`) first, ours puts
  the `enemyGuns` pad (`%372`) first;
* a profile can move the floats, but it also re-lays the body.

This falsifies the simple "replay the original's profile" hypothesis for this
function. The remaining candidates are (a) a compiler-internal difference
(clang revision / `MachineBlockPlacement` worklist order), or (b) a profile
whose weights happen to reproduce the heuristic body exactly — not found by
the 20-counter single-axis sweep or the derived vector.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w9416
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 _work/reconstructions/pgo_layout_w9416.py \
  --src src/game/weapons/Radar.cpp --func _ZN5RadarC2EP5Level \
  --addr 0x12d484 --dump
```

Scratch (`/dev/shm/w9416`): `fdiff.py`, `align_diff.py`, `extabfn.py`,
`measure.sh`, `linkcmd.sh`, `mkprof.py`, `mapcounters.py`, `bfreq.py`,
`shiftsweep.py`, `Radar.canon.obj`.

## 6. Open questions

1. Is the original's body really a no-profile build? Check another body-exact
   function with a known `!prof` witness (e.g. `Status::missionCompleted` is
   *not* body-exact, so it cannot answer this). A body-exact function whose
   switch table order is source order would be decisive.
2. If the original *was* profiled, the profile must be a fixed point of the
   layout (weights → same layout). Search for counter vectors that reproduce
   the 2488 B body, then inspect their pad frequencies.
