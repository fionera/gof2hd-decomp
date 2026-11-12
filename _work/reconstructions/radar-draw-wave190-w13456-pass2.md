# Radar::draw wave 190 (w13456 pass 2) — asteroid timer store swap
# raw 6,239 -> 6,156 (-83), size-exact, `.rel.plt` identical

**Status:** landed, 2026-11-12. Worktree `wt/w13456`, branch `wave190-w13456`.
Target `src/game/weapons/Radar.cpp`, `_ZN5Radar4drawEP6PlayerP3Hudi`, 9,908 B.
Read with `radar-draw-wave190-w13456.md` (pass 1, the 6,239 base).

## 0. The change

Swap the `this->asteroidLockTimer = 0;` store in the `if (best > -1)` block with
the `this->asteroidLockTimer += mode;` at the end of the following
`if (this->lockedAsteroid != nullptr)` block (both at the same brace depth, 12 lines
apart):

```cpp
             if (best > -1) {
                 if (this->field_0x10 != this->asteroids->data()[best]) {
-                    this->asteroidLockTimer = 0;
+                    this->asteroidLockTimer += mode;
                 }
                 ...
             if (!turretMode && !enemyHit) {
                 if (this->lockedAsteroid != nullptr) {
                     ...
-                    this->asteroidLockTimer += mode;
+                    this->asteroidLockTimer = 0;
                 }
```

The swap re-perturbs the IR block order and the cold-block placement. The -83 is a
global layout effect: the `12e050..12e3cc` prologue stays identical, and the whole
planet-loop cascade after `12e3d0` re-aligns over a long stretch. Function size stays
9,908 (instruction count 3,541 / 3,532).

| metric | base | after |
|---|---:|---:|
| raw same-address diff | 6,239 | **6,156 (-83)** |
| instruction-index alignment | 2,014 | 2,021 |
| function size | 9,908 / 9,908 | 9,908 / 9,908 |
| instruction count | 3,541 / 3,532 | 3,541 / 3,532 |
| `.text` differing bytes (whole `.so`) | 72,231 | **72,148 (-83)** |
| `.ARM.extab` differing bytes | 584 | 584 (no growth, size/base exact) |
| `.rel.plt` | byte-identical | **byte-identical** |
| build-id differing bytes | 20 | 20 |
| total (text+extab+build-id) | 72,835 | **72,752 (-83)** |

`RATCHET PASS`: byte_exact 4,507 / linked_exact 4,507 / size 4,524 / data 843 /
imports 207 / lint CLEAN / drift 0 / parity 5,370.

## 1. How it was found

Automated non-adjacent statement-swap sweep over every pair of simple statements
(`;`-terminated, non-control) at the same brace depth within 15 lines: 791 pairs,
`/home/debian/GalaxyOnFire2/scratch/w13456/sweep_pairs.py` (log
`sweep_pairs.log`). Only this pair beats 6,239; no other pair is below 6,245.
Reproducer `swap6156.py` (0-based indices 1187 / 1201 on the pass-1 source).

Neutral ties at 6,239 (same-depth image-pointer assignment swaps in the hud
block): `frameImage = image_0xe4/0xe8`, `barImage = image_0xd4/0xd8`,
`lockImage = image_0x64/0x5c/0x74`, `image = image_0x7c/0x80/0x84`.

## 2. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6156 aligned= 2021 size=9908/9908 insn=3541/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```
