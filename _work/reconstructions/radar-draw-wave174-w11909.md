# Radar::draw wave 174 (w11909) — frame-path `blip` locals + planet-lock expect + `int missionBlocksPlanets`

**Status:** landed, 2026-11-11. Worktree `wt/w11909`, branch `wave174-w11909`,
base `master` `be9dd429b`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave173-w11809.md` (planet-lock OR swap + frame-test expect),
`radar-draw-wave173-w11808.md` (campaign/`sl` interval accounting) and
`radar-draw-wave171-w11708.md` (landing-pad chain table).

## 0. Baseline (verified on `be9dd429b`)

| metric | base | landed |
|---|---:|---:|
| function size | 9,908 | 9,908 |
| raw same-address diff | 6,849 | **6,833** |
| instruction-index alignment | 1,577 | **1,568** |
| instruction count | 3,541 ours / 3,532 orig | 3,536 / 3,532 |
| `.text` differing bytes | 80,897 | **80,881** |
| `.ARM.exidx` differing bytes | 0 | 0 |
| `.ARM.extab` differing bytes | 675 | 675 |
| `.rel.plt` | byte-identical (0) | byte-identical (0) |

Gate after the change (same as base): byte_exact **4,507/4,524**, linked_exact
**4,507/4,524**, size-exact **4,524/4,524**, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data_exact **843/843**, imports **207**, lint **CLEAN**,
drift 0, parity **5,370/5,370**. `RATCHET PASS`.

## 1. Landed change (one commit, four sites)

```diff
-    bool missionBlocksPlanets = false;
+    int missionBlocksPlanets = false;
...
                 if (this->stationLockTimer > 0) {
+                    Sprite *blip = this->blipSprite;
                     int frame;
                     if (__builtin_expect(this->field_0x28 != 0, 1) && this->field_0x28 != this->lockedStation) {
-                        frame = (int) ((float) (this->blipSprite->getRawFrameCount() - 1) *
+                        frame = (int) ((float) (blip->getRawFrameCount() - 1) *
                                        ((float) this->stationLockTimer / (float) this->field_0x1b8));
                     } else {
-                        frame = this->blipSprite->getRawFrameCount() - 1;
+                        frame = blip->getRawFrameCount() - 1;
                     }
-                    this->blipSprite->setFrame(frame);
+                    blip->setFrame(frame);
...
-                    if (!this->lockConfirmed) {
+                    if (__builtin_expect(!this->lockConfirmed, 0)) {  // lint: arm_builtin
                         Globals::sound->play(26, nullptr, nullptr, 0.0f);
                     }
                     this->field_0x14 = this->field_0x18;
...
                 if (this->planetLockTimer < 1 || missionBlocksPlanets) {
                     goto enemies;
                 }
+                Sprite *blip = this->blipSprite;
                 int frame;
                 if (__builtin_expect((this->field_0x18 != 0), 1) && this->field_0x18 != this->field_0x14) {
-                    frame = (int) ((float) (this->blipSprite->getRawFrameCount() - 1) *
+                    frame = (int) ((float) (blip->getRawFrameCount() - 1) *
                                    ((float) this->planetLockTimer / (float) this->field_0x1b8));
```

All four sites are size-neutral **as a set**: `int missionBlocksPlanets` is -4 B,
the station `Sprite *blip` local is -4 B, the planet `Sprite *blip` local is +4 B,
and the planet-lock `expect(!lockConfirmed, 0)` is +8 B (its landing-pad/cleanup
effect, established in wave 173). Net 0 -> 9,908 B.

### 1.1 Why each site is the right shape

* **`int missionBlocksPlanets`.** The original spills this flag as a 4-byte word
  (`12e3ba: f8cd b070 str.w fp,[sp,#112]`, reloads `12e74c 9d1c ldr r5,[sp,#112]`,
  `12ea20 f8dd b070 ldr.w fp,[sp,#112]`, `12eb3c f8dd b070 ldr.w fp,[sp,#112]`),
  and consumes it as an integer in the planet-lock guard
  (`130492: ea55 0001 orrs.w r0,r5,r1`). Declaring it `int` is the source shape
  that keeps that word-width spill/load and is also the -4 B size compensator.
* **Station `Sprite *blip` local.** The original loads the sprite pointer into
  `r5` in *both* arms and reuses that same `r5` for `setFrame`
  (`13040c ldr.w r5,[r4,#244]; 130410 mov r0,r5` / `130544 ldr.w r5,[r4,#244];
  130548 mov r0,r5` / `130550 mov r0,r5`). A local `blip` used for both
  `getRawFrameCount()` calls and for `setFrame()` is exactly that reuse.
* **Planet `Sprite *blip` local.** Same idiom in the planet frame path
  (`1304a4/1304ea ldr.w r5,[r4,#244]`, `1304f6 mov r0,r5`); the local is used
  for the first arm's `getRawFrameCount()` and makes the pointer a single SSA
  value there. The else arm keeps `this->blipSprite` (the original reloads it).
* **Planet-lock `expect(!lockConfirmed, 0)`.** `!lockConfirmed` is the lock
  re-acquire test in the planet block (`130454 f894 0218 ldrb.w r0,[r4,#536];
  130458 b950 cbnz r0,...`). The expect is a codegen no-op *inside* the block
  (the `ldrb/cbnz` shape is unchanged) but its +8 B landing-pad/cleanup effect
  is exactly what re-balances the -8 B from the two frame locals. This is the
  "different size-exact shape" for the lock-landmark expect: on its own it is
  size-fatal (9,916), but with the frame locals + `int mbp` it is 9,908.

## 2. Variants measured (all reverted unless landed; size must stay 9,908)

### 2.1 Lock-landmark `!lockConfirmed` expect (station block, line ~500)

| shape | raw | aligned | size |
|---|---:|---:|---:|
| plain `if (!lockConfirmed)` (base) | 6,849 | 1,577 | 9,908 |
| `expect(!lockConfirmed, 0)` | 6,869 | 1,599 | **9,916** (+8) |
| `expect(lockConfirmed == 0, 1)` | 6,849 | 1,577 | 9,908 (no-op) |
| `expect(lockConfirmed != 0, 1)` | 6,849 | 1,577 | 9,908 (no-op) |
| `expect(lockConfirmed == 0, 0)` | 6,869 | 1,599 | 9,916 |
| `expect(lockConfirmed != 1, 1)` | 6,864 | — | 9,908 |

### 2.2 Planet-lock `!lockConfirmed` expect (line ~634)

| shape | raw | aligned | size |
|---|---:|---:|---:|
| plain (base) | 6,849 | 1,577 | 9,908 |
| `expect(!lockConfirmed, 0)` | 6,871 | 1,712 | **9,916** (+8) |
| `expect(lockConfirmed == 0, 1)` | 6,849 | 1,577 | 9,908 (no-op) |
| `expect(lockConfirmed == 0, 0)` | 6,871 | 1,712 | 9,916 |

### 2.3 Frame-path `Sprite *blip` locals (on the base source)

`S1` = station blip declared at top, used in both arms + `setFrame`;
`S2` = station blip at top, first arm only; `S5` = station blip assigned in
both arms, used for `setFrame`; `P2` = planet blip at top, first arm only;
`P6` = planet blip assigned in both arms, used for `setFrame`.

| shape | raw | aligned | size |
|---|---:|---:|---:|
| S0+P0 (base) | 6,849 | 1,577 | 9,908 |
| S1+P0 | 6,896 | 1,572 | 9,904 (-4) |
| S2+P0 | 6,912 | 1,552 | 9,900 (-8) |
| S0+P2 | 6,877 | 1,573 | 9,912 (+4) |
| S0+P6 | 6,896 | 1,576 | 9,912 (+4) |
| S1+P2 | 6,862 | 1,546 | 9,904 (-4) |
| S5+P0 | 6,897 | 1,578 | 9,912 (+4) |
| S5+P6 | 6,880 | 1,577 | 9,916 (+8) |
| `int missionBlocksPlanets` alone | 6,900 | 1,583 | 9,904 (-4) |

### 2.4 Size-exact combinations with `int missionBlocksPlanets`

| shape | raw | aligned | size |
|---|---:|---:|---:|
| **S1+P2 + planet `expect(!lockConfirmed,0)` (landed)** | **6,833** | **1,568** | **9,908** |
| S2+P6 + planet expect | 6,822 | 1,600 | 9,908 |
| S2+P2 + station `expect(!lockConfirmed,0)` | 6,861 | 1,547 | 9,908 |
| S0+P6 | 6,844 | 1,582 | 9,908 |
| S5+P0 | 6,842 | 1,584 | 9,908 |
| S1+P6 + planet expect | 6,862 | 1,574 | 9,908 |
| S2+P6 + station expect | 6,870 | 1,601 | 9,908 |

The landed cell (and its codegen-equivalent `S3`/`S4` station variants) is the
only measured shape that improves **both** raw (6,849 -> 6,833) and aligned
(1,577 -> 1,568) with a size-exact body. `S2+P6+expect` has the
lowest raw (6,822) but gives back 32 aligned instructions; `S2+P2+station
expect` has the lowest aligned (1,547) but is raw-worse.

### 2.5 Frame-test / `isDockingToPlanet` / declaration-order sweeps

On the landed source, F1/F2/F4 (frame-test expect on second operand, no expect,
swapped operands) and G1/G2 (plain `isDockingToPlanet`, `expect(...,0)`) all
measure identically (6,833 / 1,568 / 9,908); F3 is 6,850/1,594, F5 is
6,889/1,610/9,912, G3 is 6,834/1,578. Permutations of the early declaration
block (`plasmaInRange`, `lockConfirmed`, `SetColor`, `mission`, `mbp`,
`alwaysEnemySeen`, `alwaysEnemy`) are neutral or worse; the base order is kept.

## 3. Campaign-allocation residual (not cracked)

The dominant residual is unchanged: the original keeps `campaign` in `sl`
(`12e0f8 mov sl,r0`, `12e38a cmp.w sl,#2`) and `missionBlocksPlanets` in `fp`,
while ours still spills `campaign` to `sp+144` (`12e0f8 str r0,[sp,#144]`) and
uses `sl` for the shared zero. The landed change does **not** move `campaign`
into `sl` (verified in the disassembly). The only lever found that does is
`turretMode` as `int`, which puts `campaign` in `sl` but shrinks the function to
9,840 B (raw 9,243 / aligned 2,023), consistent with the wave-173 finding that
"campaign-in-`sl` and 9,908 B never co-occur" without ~40 B of code we still
lack elsewhere. Fixing that is the next lever.

## 4. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
cmake --build cmake-build-match --target check -j2
```

Scratch (all under `scratch/w11909/tmp/`): `radar_base.cpp` (base source),
`cand.cpp`/`cand_final.cpp` (landed source), `eval.py`, `comp.py`,
`grid.py`/`grid2.py`/`grid3.py`/`grid4.py`/`grid5.py`/`grid6.py`/`grid7.py`,
`search1.py`..`search10.py`, `perm.py`, `aldiff.py`, and the per-variant
`diff_*.txt` dumps. `textdiff.py` computes the `.text`/`.ARM.exidx`/`.ARM.extab`
byte counts used above.
