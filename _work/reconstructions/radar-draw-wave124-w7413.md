# Radar::draw pass (wave 124, w7413) — tail-before-landmark landed (74.541284 -> 75.268817 %, 9908 B)

**Status:** source change landed, 2026-11-06. Worktree `wt/w7413`, based on current
`master` (`6f8ee2320`, wave-123 final). Target `_ZN5Radar4drawEP6PlayerP3Hudi`
(`Radar::draw(Player*, Hud*, int)`) in `src/game/weapons/Radar.cpp`.

## Result

| metric | before | after |
|---|---:|---:|
| raw `difflib` ratio | **74.541284 %** | **75.268817 %** |
| matching instructions `M` | 2600 | 2625 |
| size | 9908 B | **9908 B** |
| `.text` differing bytes | 87,014 | **86,711** |
| `.ARM.exidx` differing bytes | 0 | **0** (still byte-identical) |
| `.ARM.extab` differing bytes | 1,446 | **1,440** (size/base still exact, 156,312 B) |
| `.rodata` / `.rel.plt` / build-id | 0 / 4 / 20 | 0 / 4 / 20 |

Full gate after the change:

```
compared 4524  avg 99.94%  byte_exact 4506  linked_exact 4506  fuzzy_perfect 4506
stub_zero_size 0  orig_zero_size 0  missing 0 (wrong_type 0  absent 0)  extra 0
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
=== DRIFT: 0 field(s) across 0 class(es) ===
CLEAN ✓ (no new soft violations; hard invariants 0)
SYMBOL PARITY ✓ (5370/5370)
```

`.ARM.exidx` is still byte-identical (the function is still 9908 B, so its exidx entry is
unchanged); `.ARM.extab` is still size- and base-exact (`0x26298` @ `0x1a3b78`), only its
createShip landing-pad content bytes moved (1,446 -> 1,440).

## The five source edits

All in `Radar::draw`; no other TU touched.

1. **Planet-course recompute reduced to the target comparison** (the invented duplicate
   block after the planet loop). The baseline recomputed
   `goingToPlanet() && !isDockingToPlanet() && autoPilotTarget == targets[stationIndex]`;
   the original computes the `goingToPlanet`/`isDockingToPlanet` half once in the prologue
   and only the target comparison is repeated here. Dropping those two calls shrinks the
   block enough that the planet-lock tail can be laid out *before* the landmark loop while
   keeping the function at 9908 B:

   ```cpp
   onPlanetCourse = autoPilotTarget == ((Array<int> *) this->planetTargets)->data()[stationIndex];
   ```

2. **`field_0x14 == 0` arm stores `field_0x18` before `planetLockTimer`** (the
   `norec_swapstores` trick from wave 119, now load-bearing in the opposite direction).
   This prevents clang from hoisting the common `planetLockTimer = 0` before the branch and
   merging the two arms; the planet-lock tail therefore ends in a branch to the enemies and
   stays contiguous with the planet loop instead of falling through into the landmark block:

   ```cpp
   } else if (this->field_0x14 == 0) {
       this->field_0x18 = 0;
       this->planetLockTimer = 0;
   } else {
   ```

3. **Landmark guard uses `Globals::status` directly** (remove the `Status *status` local).
   With the shorter recompute the register pressure drops and the direct global gives the
   original's straight `cbnz` `otherLock` chain without the local's early load.

4. **Unfold the landmark `i == 1 || i == 3` arm** into two `else if` arms. The original is
   an unfolded `bic` + `cmp #1` + `cmp #3`; the folded `(i | 2) == 3` form is 4 B shorter.

5. **Enemy loop re-tests `isDead() || isDying()` for `dead`** (one extra pair of calls, the
   original's 17/13 vs the baseline's 16/12 call counts). This is the last 8 B that brings
   the body to exactly 9908 B.

## Block-layout evidence

Before (baseline): the landmark guard branches to the landmark setup at `12e776`, the
landmark loop runs to `12eb02`, and the planet-lock tail is at `12ebec` (after the landmark
block). The invented recompute occupies the slot between the cold prologue-else and the
landmark loop.

After: the planet-lock tail is at `12e776` and the landmark guard now branches to
`12e7c4` (the landmark setup), i.e. **the planet-lock tail is laid out before the landmark
loop, exactly as in the original** (original tail `12e748`, setup `12e7a8`). The downstream
sections stay aligned: `PlayerGasCloud::getSparks` is at `12f454` (original `12f460`,
unchanged from baseline), and the enemies/gas/asteroid/music blocks keep their baseline
addresses.

The remaining landmark-loop body is still ~0x2c late (setup `12e7c4` vs original `12e7a8`;
body `12e800` vs `12e7d4`) because the planet loop is 0x26 B shorter and the literal pool is
placed after the setup instead of before it. That is the residual root cause; the tail/loop
ordering itself now matches.

## Variants measured (all on the pristine baseline unless noted)

The winning combination was found by an exhaustive 128-subset sweep over
`{tb, ll, unf, ast, tail, ph, os}` on top of `rec=P2, st=1, sw=1`:

| variant | size | raw % |
|---|---:|---:|
| **baseline** | 9908 | 74.541284 |
| `rec=P2, st=1, sw=1` | 9892 | 74.709511 |
| `rec=P2, st=1, sw=1, unf=1` | 9892 | 74.070885 |
| `rec=P2, st=1, sw=1, unf=1, ast=1` | **9908** | **75.268817** |
| `rec=P2, st=1, sw=1, unf=1, ast=1, tail=1` / `ph=1` | 9908 | 75.268817 (tie) |
| `rec=full, tb=1` (wave-123 tail-before-recompute) | 9928 | 71.451082 |
| `rec=none, sw=1` (faithful, no recompute) | 9828 | 71.445020 |
| `rec=mid, sw=1` | 9880 | 73.679679 |
| `rec=full, st=1, sw=1, unf=1` | 9928 | 72.987683 |

`tail=1` (`!landmarkHit && !turretMode`) and `ph=1` (`!planetHit && !otherLock &&
!turretMode`) are codegen-identical to the winning shape and were not landed (kept the diff
minimal).

## Root cause of the remaining residual

The planet loop body is 0x26 B shorter than the original's (register allocation / 32-bit
vs 16-bit encodings: 192 vs 196 instructions), and the literal pool is placed after the
landmark setup (`12e7f4`) rather than before it (`12e79c`). Both push the landmark body
~0x2c late, so the landmark `isVisible`/`update`/`calcDistance`/`DrawString` block still
mismatches positionally even though the tail/loop order now matches. Fixing the planet loop
encoding is the next lever.

## Commands

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6     # RATCHET PASS
cmake --build cmake-build-match --target drift -j6     # 0 fields
cmake --build cmake-build-match --target lint          # CLEAN
cmake --build cmake-build-match --target diff -j6      # SYMBOL PARITY 5370/5370
```

Scratch harness `/dev/shm/w7413/score.py` mirrors `tools/verify/verify.py`'s
`disassemble`/`reconcile` and reproduces the official unrounded ratio exactly.
