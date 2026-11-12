# Radar::draw wave 190 (w13371) — station-blip frame reassociation: raw 6,308 -> 6,306
# (size-exact, `.rel.plt` identical); plus a tractor2 operand reorder (aligned 1,870 -> 1,780)

**Status:** landed, 2026-11-12. Worktree `wt/w13371`, branch `wave190-w13371`, base `master`
`0f54626b3`. Target `src/game/weapons/Radar.cpp`, `_ZN5Radar4drawEP6PlayerP3Hudi`
(`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave189-w13356.md` (ternary inversion), `radar-draw-wave189-w13346.md`
(register map / music-block gating) and `radar-draw-wave189-w13331.md`. This pass is an
independent lens from the parallel w13370 worker (whose cells are the `onPlanetCourse`
pointer/`volatile` variants): it works the landmark / station / enemy branch draw order and the
`otherLock` data flow.

## 0. The win

Two source edits, both in the landmark/station and enemy loops:

```cpp
// station-blip frame (in the `if (!turretMode)` block after the landmark loop)
- frame = (int) ((float) (blip->getRawFrameCount() - 1) *
-                ((float) this->stationLockTimer / (float) this->field_0x1b8));
+ frame = (int) ((float) this->stationLockTimer * (float) (blip->getRawFrameCount() - 1) /
+                (float) this->field_0x1b8);

// enemy tractor2 block
- if (this->tractorMode2 && wreck && enemy->isDead() && this->field_0x1c == nullptr) {
+ if (this->tractorMode2 && enemy->isDead() && wreck && this->field_0x1c == nullptr) {
```

| metric | base | after |
|---|---:|---:|
| raw same-address diff | **6,308** | **6,306** |
| instruction-index alignment | 1,858 | **1,780** |
| function size | 9,908 / 9,908 | 9,908 / 9,908 |
| instruction count | 3,541 / 3,532 | 3,542 / 3,532 |
| `.text` differing bytes (whole `.so`) | 73,516 | **73,514** |
| `.ARM.extab` differing bytes | 610 | 610 (no growth) |
| `.rel.plt` | byte-identical | **byte-identical** |
| `.data` / `.dynamic` / `.rel.dyn` | byte-identical | **byte-identical** |
| build-id differing bytes | 20 | 20 |
| total differing bytes | 74,146 | **74,144** |

`RATCHET PASS`: byte_exact 4,507 / linked_exact 4,507 / size 4,524 / data 843 / imports 207 /
lint CLEAN / drift 0 / parity 5,370. The function was already non-byte-exact, so `byte_exact` is
unchanged; the gain is in the raw residual and `.text`.

The station-blip reassociation is the raw/`.text` win (`.text` -2, aligned 1,858 -> 1,870 on its
own). The tractor2 operand reorder is raw/`.text`-neutral on its own (raw stays 6,308) but it is
a large *structural* win (aligned 1,858 -> 1,768 alone); combined with the blip edit the pair
lands at raw 6,306 / aligned 1,780, i.e. the same `.text` win with a much better aligned residual
than the blip edit alone.

## 1. Baseline (verified)

| metric | value |
|---|---:|
| function address | `0x12e050` |
| function size | 9,908 / 9,908 |
| raw same-address diff | **6,308** |
| instruction-index alignment | 1,858 |
| instruction count | 3,541 ours / 3,532 orig |
| `.text` differing bytes (whole `.so`) | 73,516 |
| `.ARM.extab` differing bytes | 610 (size/base exact at `0x1a3b78`, 156,312 B) |
| `.rel.plt` | byte-identical (29,392 B) |
| build-id | 20 |
| total differing bytes | 74,146 |
| gate | byte 4,507 / linked 4,507 / size 4,524 / data 843 / imports 207 / lint CLEAN / drift 0 / parity 5,370 |

## 2. `otherLock` data flow — all four computations checked

There are four `otherLock` writes in the function. Each was compared against the original
disassembly:

1. **First** (`otherLockFlag`, `lockedAsteroid`/`lockedGasCloud`/`field_0x1c` then `field_0x14`):
   `12e2fc`-`12e312` and the out-of-line `12ebc2` false path are byte-identical to the original.
2. **Second** (`lockedAsteroid || lockedGasCloud || field_0x1c || lockedStation`, after the
   landmark loop): `12e376`-`12e388` + `12ebce` are byte-identical (same `ldr [r1,#12]!` /
   `str [sp,#124]` shape and the same `it ne / movne` materialisation of `lockedStation`).
3. **Third** (`lockedStation || (dockNavPtr && !lockedEnemy) || field_0x1c || field_0x14`, before
   the gas-cloud block): the original evaluates `(dockNavPtr && !lockedEnemy)`, `field_0x1c`,
   `lockedStation`, then the out-of-line `field_0x14` at `12f8a6`; the base is
   `lockedStation, dockNav, field_0x1c, field_0x14`. Reordering to the original's order is
   **worse** (raw 6,320) because the preceding zero-store block is 4 B short; the base order is
   kept. (Same conclusion as w13356's 24-permutation table.)
4. **Fourth** (asteroids, `(hasDockNav && !lockedEnemy) || field_0x1c || lockedStation ||
   field_0x14`): `12f972`-`12f986` + `12fdae` are byte-identical to the original.

So the `otherLock` data flow is already correct; no win there.

## 3. Draw-order cells measured this pass (all reverted except the landed pair)

Every row is a full incremental `link_so -j2` + `tools/fn_diff.py`
(`ulimit -v 12000000`; scratch `/home/debian/GalaxyOnFire2/scratch/w13371/`).

### 3.1 Station-blip frame arithmetic (landmark/station branch)

The original's station-blip frame at `13040c`-`130440` computes `(timer / max) * (count - 1)`
(`vdiv` then `vmul`). The base source already emits that shape, but it sits in a shifted tail;
reassociating the source to the same `timer * (count - 1) / max` form the planet blip uses
(wave-183 landing) moves two bytes into agreement at the same addresses.

| edit | raw | aligned | size |
|---|---:|---:|---:|
| base | **6,308** | 1,858 | 9,908 |
| **station blip `timer*(count-1)/max` (`st_a`)** | **6,306** | 1,870 | **9,908** |
| station blip `(count-1)*timer/max` (`st_b`) | 6,308 | 1,867 | 9,908 |
| station blip `timer/max*(count-1)` (`st_c`) | 6,307 | 1,871 | 9,908 |
| station blip `timer*((count-1)/max)` (`st_d`) | 6,440 | 1,876 | 9,916 |
| station blip `(timer*(count-1))/max` (`st_e`) | 6,422 | 1,877 | 9,904 |
| planet blip back to `(count-1)*(timer/max)` (`pl_a`) | 6,310 | 1,850 | 9,908 |
| planet blip `timer/max*(count-1)` (`pl_b`) | 6,313 | 1,860 | 9,908 |
| planet blip `timer*((count-1)/max)` (`pl_c`) | 8,922 | 1,940 | 9,876 |
| duplicate `this->blipSprite` instead of the local | 6,456 | 2,008 | 9,916 |
| move the local load after the if/else | 6,456 | 2,008 | 9,916 |

### 3.2 Enemy tractor2 operand order (landed, aligned-only on its own)

| edit | raw | aligned | size |
|---|---:|---:|---:|
| base | **6,308** | 1,858 | 9,908 |
| **`tractorMode2 && isDead() && wreck && ...`** | **6,308** | **1,768** | **9,908** |
| `tractorMode2 && wreck && isDead() && ...` (base) | 6,308 | 1,858 | 9,908 |
| `st_a` + tractor2 reorder (landed pair) | **6,306** | **1,780** | **9,908** |

### 3.3 Landmark hit guard — all 24 `&&` permutations

Base `(!landmarkHit && i != 3 && notTurretMode && notOtherLock)` is the best raw value
(6,308). The only other size-exact permutations are `(0,1,3,2)` (6,308 / 1,858) and
`(0,2,1,3)` / `(0,3,1,2)` (6,344 / 1,822). Everything else is size-fatal.

### 3.4 Landmark crosshair box — all 24 `&&` permutations

Base `(screenY < top+2*size && topOk && screenX < left+2*size && left < screenX)` is the best
raw value (6,308). Best non-base: `(3,0,1,2)` = 6,314 / 1,808.

### 3.5 Station branch cells

| edit | raw | aligned | size |
|---|---:|---:|---:|
| base | **6,308** | 1,858 | 9,908 |
| `!otherLock && !planetHit && !turretMode` (all 6 perms) | 6,308 | 1,858 | 9,908 |
| station crosshair box (all 24 perms) | 6,308 (base) | 1,858 | 9,908 |
| `campaign >= 2` guard reorder | 8,837 | 2,935 | 9,916 |
| campaignMission guard: `isVisible` before `isEmpty` | 9,153 | 2,760 | 9,944 |
| `116` chain: range test first | 6,308 | 1,863 | 9,908 |
| `125` chain: `isFreighterMissionStation` first | 6,312 | 1,807 | 9,908 |
| freelance guard: `isVisible` before `isEmpty` | 8,416 | 1,856 | 9,912 |
| planet-dock guard: docking-point before asteroid | 6,309 | 1,858 | 9,908 |
| `planetLockTimer < 1` -> `<= 0` | 6,308 | 1,858 | 9,908 |
| `field_0x18 != field_0x14` -> `field_0x14 != field_0x18` | 6,308 | 1,858 | 9,908 |
| `expect(field_0x18 != 0, 1) && ...` -> `field_0x18 != 0 && expect(..., 1)` | 6,308 | 1,858 | 9,908 |

### 3.6 Landmark statement/guard order

| edit | raw | aligned | size |
|---|---:|---:|---:|
| base | **6,308** | 1,858 | 9,908 |
| `isVisible` before the null `expect` | 8,399 | 1,861 | 9,912 |
| `i == 2` moved last | 8,504 | 1,857 | 9,904 |
| landmark bounds: screenY pair first | 8,157 | 1,850 | 9,896 |
| `!landmarkHit && notTurretMode && i != 3 && notOtherLock` | 6,344 | 1,822 | 9,908 |
| `!landmarkHit && notOtherLock && i != 3 && notTurretMode` | 6,344 | 1,822 | 9,908 |
| `i != 3` -> `i < 3` (image) | 6,308 | 1,859 | 9,908 |
| `i <= 1` -> `i < 2` | 6,308 | 1,858 | 9,908 |
| `i == 0 && !alienOrbit` -> `!alienOrbit && i == 0` | 8,521 | 1,928 | 9,920 |
| swap the two `else if` image branches | 8,543 | 1,857 | 9,900 |
| `(i & 0x7fffffff) == 3` -> `i == 3` | 6,308 | 1,858 | 9,908 |
| `(i & 0x7fffffff) == 1` -> `i == 1` | 6,308 | 1,858 | 9,908 |

### 3.7 Enemy branch cells (distinct from w13370)

| edit | raw | aligned | size |
|---|---:|---:|---:|
| base | **6,308** | 1,858 | 9,908 |
| `scannerAvailable && enemies` -> `enemies && scannerAvailable` | 6,308 | 1,863 | 9,908 |
| boss guard: `isActive` before `shipGroup == 10` | 6,308 | 1,863 | 9,908 |
| boss guard: `!isDying` before `!isDead` | 6,308 | 1,858 | 9,908 |
| `wreck = (isDead()||isDying()) && hasCargo` | 9,802 | 3,511 | 10,196 |
| continue guard: `!hasCargo && ...` | 8,572 | 1,871 | 9,916 |
| steal guard: exclusion test before `stealFlagByte` | 8,538 | 2,004 | 9,920 |
| steal guard inner OR swap | 8,538 | 2,003 | 9,920 |
| `isAlwaysEnemy() && field_0x42` | 8,550 | 1,934 | 9,900 |
| 24000 checks: y before x | 8,648 | 1,983 | 9,888 |
| 24000 checks: `>` before `<` | 6,308 | 1,871 | 9,908 |
| frame/bar image: cargo-flag first | 6,308 | 1,869 | 9,908 |
| lock image: `enemyFlagsLo` first | 6,308 | 1,854 | 9,908 |
| `inSight` box: screenY first | 9,307 | 2,758 | 9,988 |
| tractor guard: docking-point test before autopilot | 8,171 | 1,772 | 9,912 |
| docking guard: `!(...)` before asteroid | 8,615 | 2,003 | 9,872 |
| `navWreck`: dead/dying before `hasCargo` | 8,362 | 1,982 | 9,888 |

## 4. Composition checks

| edit | raw | aligned | size |
|---|---:|---:|---:|
| `st_a` | 6,306 | 1,870 | 9,908 |
| `st_a` + landmark hit reorder | 6,342 | 1,834 | 9,908 |
| `st_a` + crosshair box `(3,0,1,2)` | 6,312 | 1,820 | 9,908 |
| `st_a` + station crosshair reorder | 6,306 | 1,877 | 9,908 |
| `st_a` + `116`-chain reorder | 6,306 | 1,875 | 9,908 |
| `st_a` + lock-image reorder | 6,306 | 1,866 | 9,908 |
| **`st_a` + tractor2 reorder (landed)** | **6,306** | **1,780** | **9,908** |

## 5. Verification

`cmake --build cmake-build-match --target check` -> `RATCHET PASS` (byte 4,507 / linked 4,507 /
size 4,524 / data 843 / imports 207 / lint CLEAN / drift 0 / parity 5,370). `.rel.plt`,
`.data`, `.dynamic` and `.rel.dyn` are byte-identical; `.ARM.extab` stays at 610 differing bytes
(size/base exact); `.text` 73,516 -> 73,514; total (text+extab+build-id) 74,146 -> 74,144.

## 6. Current-master rebase (wave 190 harvest)

The worker branch was based on `0f54626b3` (Radar raw 6,308). Current master already
contains the w13364 `camZ` loop hoist (raw 6,301). Re-applied the two source edits on
current master (`192143be0`):

```text
raw       6,301 -> 6,299  (-2)
aligned   1,876 -> 1,798
size      9,908 / 9,908 (exact)
.text     73,304 -> 73,302 (-2)
.ARM.extab 603 unchanged
total     73,927 -> 73,925 (-2)
```

`check` green: byte_exact 4507/4524, linked_exact 4507/4524, size_exact 4524/4524,
data_exact 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370, RATCHET PASS.
