# Radar::draw wave 190 (w13456) — asteroid-lock branch statement swap
# raw 6,265 -> 6,239 (-26), size-exact, `.rel.plt` identical

**Status:** landed, 2026-11-12. Worktree `wt/w13456`, branch `wave190-w13456`, base
`master` `83d52fe52`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave190-w13448.md` (station-lock blip call order, the 6,265 base),
`-w13437.md`, `-w13435.md`, `-w13433.md` and the campaign/`sl` family
`radar-draw-campaign-sl-wave164..167-*.md`.

## 0. The change

In the asteroid-lock block (`if (!turretMode && !enemyHit)`), the final statement of the
`if (this->lockedAsteroid != nullptr) { ... }` block and the first statement of the
following `else` branch are exchanged:

```cpp
                 if (this->lockedAsteroid != nullptr) {
                     ...
-                    this->lockedAsteroid = nullptr;
+                    this->asteroidLockTimer += mode;
                 }
                 ...
                 } else {
-                    this->asteroidLockTimer += mode;
+                    this->lockedAsteroid = nullptr;
                     auto hasScanner = this->hasScanner19;
```

The timer increment now sits in the guarded branch and the redundant null store in
the `else` (`lockedAsteroid` is already null there because of the guard); the two fields
are disjoint. The move lets clang emit the candidate-wreck `isDead`/`isDying` block in
the original's exact instruction order. The -26 is a register-allocation /
cold-block-placement effect, not a local byte fix:

* fixed vs base: `12fde6..12fe15` (44 B) plus `12fe1a`/`12fe1b` (the whole
  `cbz`/`hasCargo`/`isDead`/`isDying` sequence), `12fe6d..12fea0` (9 B), and
  `130350` (1 B);
* newly differing: `12e05e`, `12e9aa`, `12f85e` (single bytes) and the
  `12fe1e..12febc` cluster (30 B) — the candidate-wreck select and the
  `asteroidLockTimer > field_0x1b8 - 200` test.

| metric | base | after |
|---|---:|---:|
| raw same-address diff | 6,265 | **6,239 (-26)** |
| instruction-index alignment | 2,010 | 2,014 |
| function size | 9,908 / 9,908 | 9,908 / 9,908 |
| instruction count | 3,541 / 3,532 | 3,541 / 3,532 |
| `.text` differing bytes (whole `.so`) | 72,257 | **72,231 (-26)** |
| `.ARM.extab` differing bytes | 584 | 584 (no growth, size/base exact) |
| `.rel.plt` | byte-identical | **byte-identical** |
| build-id differing bytes | 20 | 20 |
| total (text+extab+build-id) | 72,861 | **72,835 (-26)** |

`RATCHET PASS`: byte_exact 4,507 / linked_exact 4,507 / size 4,524 / data 843 /
imports 207 / lint CLEAN / drift 0 / parity 5,370.

## 1. How it was found

Targeted statement swap in the asteroid-lock block after the wave-189/190 notes had
exhausted the campaign/`sl` rotation, type/volatile sweeps and the `-mllvm` regalloc
knobs. Scratch reproducer `/home/debian/GalaxyOnFire2/scratch/w13456/swap6239.py`
(0-based indices 1201 / 1213 in `src/game/weapons/Radar.cpp`).

## 2. Negative results measured this pass (all reverted)

All measured with `ulimit -v 12000000`, `link_so -j2`, `tools/fn_diff.py`.

* **Top declaration permutations** — all 4! orders of `mission`,
  `missionBlocksPlanets`, `alwaysEnemySeen`, `alwaysEnemy`: neutral 6,265.
* **Local type sweeps** — `alienOrbit` as `bool` 6,274 (size-exact), as
  `short`/`unsigned char` neutral 6,265, `volatile` 9,359 (size-fatal 9,868);
  `turretMode` `volatile` 9,605/9,968, as `int` 9,214/9,860;
  `missionBlocksPlanets` `bool` 6,422/9,912; `alwaysEnemySeen` `int` 9,004;
  `alwaysEnemy` `int` 8,575; `autoPilotTarget` `volatile` 9,323/9,876;
  `marked` `int` 8,426/9,912; planet-box `size` `unsigned` 6,270; planet loop
  `i` as `int`/`long`/`size_t` neutral, `u16`/`u8` 8,499/9,920.
* **`otherLock` OR-chain** (replacing the `otherLockFlag` if/else): 9,233/9,860.
* **`bossSeen = bossSeen || ...`**: 9,214/9,976.
* **`campaign`** `unsigned` 6,266, `const` neutral; hoisting it after `SetColor`
  8,621/8,586 (size 9,900).
* **`plasmaInRange`/`lockConfirmed` store swap** 6,267; `SetColor` first 6,296.
* **`stations`** as reference neutral, `volatile` 9,837/10,248, `const` neutral;
  `planetTargets` local alias 6,935.
* **`Status *st`** copy in the planet loop 8,761/9,900, `Status *&st` neutral,
  block-scoped `st` 9,842/10,220.
* **Planet-loop declaration swaps** (`stationId`/`currentStation`, `warpGate`,
  `campaignMission`/`freelance`, `marked`, `left`/`top`): neutral or size-fatal.
* **`__builtin_expect` on the asteroid guards** (`!turretMode && !enemyHit`,
  `!asteroidHit || isDockingToAsteroid`, `!candidateWreck`,
  `lockedAsteroid != nullptr`): 6,947-9,325 (size-fatal or worse).
* **`candidateWreck` hoisted before the `lockedAsteroid` block**: 7,058/9,892.
* **Music tail** `play(140)` <-> second `play(151)` swap: 6,264 (-1, size-exact);
  other `play(151)` permutations neutral. Superseded by this -26.
* **`field_0x1bc < 3` play-block inversion**: neutral; other inversions
  6,412/9,916. Flipping `expect(field_0x1bc < 3/5, 0)`: 6,411/7,263/7,279.
* **`-regalloc-csr-first-time-cost`** 0/50/100/500/1000/1500/2500/3000/4000/
  10000/20000: all neutral 6,265.
* **Removing `const auto camZ`** (use `this->cameraZ`): 6,340.

## 3. Root cause still open

The dominant residual remains the callee-saved rotation documented in
`radar-draw-campaign-sl-wave166-w11106.md`: original `zero/missionBlocksPlanets` in `fp`,
`campaign` in `sl`, `marked` on the stack (`sp+132`); ours `zero/mbp` in `sl`, `campaign`
spilled (`sp+144`), `marked` in `fp`. The `campaign` spill costs the `12e38a`
`ldr`/`cmp` pair and the `stations` spill costs a `ldr [sp,#132]` reload at each of its
~6 loop uses, which is the bulk of the `12e3d0`-`12ef50` shift cascade.

## 4. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6239 aligned= 2014 size=9908/9908 insn=3541/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```
