# Radar::draw wave 177 (w12108) — planet-lock guard expect + two wave-176 expect reverts

**Status:** landed, 2026-11-11. Worktree `wt/w12108`, branch `wave177-w12108`,
base `master` `9445ac5fb` (wave-176 merge). Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave176-w12005.md` (landed alias + expect set),
`radar-draw-wave176-w12006.md` (planet-block `FDS` / wreck `WZU` reorders),
`radar-draw-wave176-w12007.md` (cargo / `alwaysEnemySeen` / `inAlienOrbit`
expects) and `radar-draw-wave175-w11907.md` (the `expect(field_0x1a8, 0)`
landing-pad site).

## 0. Baseline (fresh build on `9445ac5fb`)

| metric | base | landed |
|---|---:|---:|
| function size | 9,908 | **9,908** |
| raw same-address diff | 6,713 | **6,659** |
| instruction-index alignment | 1,734 | **1,712** |
| instruction count | 3,540 ours / 3,532 orig | 3,540 / 3,532 |
| whole-`.so` `.text` differing bytes | 79,591 | **79,537** |
| `.ARM.exidx` differing bytes | 0 | 0 |
| `.ARM.extab` differing bytes | 1,321 (size/base exact) | 1,321 |
| `.rel.plt` | byte-identical (0) | byte-identical (0) |
| `.rodata` / `.data` / `.bss` | 0 / 0 / 0 | 0 / 0 / 0 |

Gate after the change: byte_exact **4,507/4,524**, linked_exact
**4,507/4,524**, size-exact **4,524/4,524**, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data_exact **843/843**, imports **207**, lint **CLEAN**,
drift **0**, parity **5,370/5,370**. `RATCHET PASS`.

## 1. Landed change (three `__builtin_expect` cells, net size 0)

```diff
@@ planet-lock outer guard (line 630)
                 this->planetLockTimer += mode;
-                if (this->planetLockTimer > this->field_0x1b8) {
+                if (__builtin_expect(this->planetLockTimer > this->field_0x1b8, 1)) {  // lint: arm_builtin
                     if (missionBlocksPlanets) {
@@ music-event guard (line 1270)
         if ((uint32_t) (current - 136) > 15 || ((1 << (current - 136)) & 0xe071) == 0 ||
-            __builtin_expect((current != 151 && alwaysEnemySeen), 0)) {  // lint: arm_builtin
+            (current != 151 && alwaysEnemySeen)) {
@@ music tail, `field_0x1bc < 1` branch (line 1303)
         this->field_0x54 = 0;
-        if (__builtin_expect((Globals::status->getCurrentCampaignMission() != 145), 1)) {  // lint: arm_builtin
+        if (Globals::status->getCurrentCampaignMission() != 145) {
```

All three are pure branch-weight annotations; conditions, short-circuit
order and side effects are unchanged.

* **`expect(planetLockTimer > field_0x1b8, 1)` (new).** The original's
  planet-lock block at `12ff98` is
  `ldr r0,[r4,#416]; add r0,mode; str r0,[r4,#416]; cmp r0,r1; ble.w 130486`
  — the lock body is the *fall-through* arm and the `(timer < 1) | mbp`
  bail-out is the cold branch. The hint moves our `ble` arm to the same
  shape. On its own: raw 6,713 -> 6,711.
* **`current != 151 && alwaysEnemySeen` plain (wave-176 w12007 revert).**
  The wave-176 `expect(..., 0)` was landed on the wave-175 base; on the
  post-merge base it is raw-worse. Removing it is -1 raw / -22 aligned once
  the other two cells are in.
* **`getCurrentCampaignMission() != 145` plain (wave-176 w12005 revert).**
  This is the big cell: on the post-merge base the wave-176 `expect(..., 1)`
  is worth **-50 raw** to remove. It changes the register allocation of the
  whole music tail (the `cmp r0,#145; beq.w` guard becomes inline at
  `1304f0`, exactly the original's shape) and shortens the planet block by
  ~110 B, which the tail gives back.

The three cells interact through block layout: C alone is 6,663, T alone
6,711, C+T 6,660, and C+T+ae0 **6,659 / 1,712**. The `ae0` cell is what
drops the aligned metric by 22.

## 2. Variant table (all size 9,908 unless noted)

Baseline for every cell: 6,713 / 1,734 / 9,908.

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base | 6,713 | 1,734 | 9,908 |
| `C` (campaign-145 plain) | 6,663 | 1,734 | 9,908 |
| `T` (planet-lock expect) | 6,711 | 1,734 | 9,908 |
| `C+T` | 6,660 | 1,734 | 9,908 |
| **`C+T+ae0` (landed)** | **6,659** | **1,712** | **9,908** |

### 2.1 Planet-block statement order on `C+T+ae0`

`S` = `if (!lockConfirmed) play(26)`, `F` = `field_0x14 = field_0x18`,
`D` = `if (expect(field_0x1a8,0)) dock`.

| order | raw | aligned | size |
|---|---:|---:|---:|
| `SFD` | 6,682 | 1,707 | 9,908 |
| `SDF` | 6,685 | 1,712 | 9,908 |
| `FSD` | 6,760 | 1,678 | 9,908 |
| **`FDS` (base/landed)** | **6,659** | **1,712** | **9,908** |
| `DSF` | 6,684 | 1,710 | 9,908 |
| `DFS` | 6,756 | 1,680 | 9,908 |

### 2.2 Rejected cells on `C+T+ae0`

| cell | raw | aligned | size |
|---|---:|---:|---:|
| plain dock guard (`if (field_0x1a8)`) | 6,740 | 1,707 | **9,904** |
| dock `expect(...,1)` | 6,740 | 1,707 | **9,904** |
| `expect(missionBlocksPlanets,0)` | 7,384 | 1,749 | 9,908 |
| `missionBlocksPlanets != 0` | 6,659 | 1,712 | 9,908 |
| `missionBlocksPlanets` if/else arm swap | 6,659 | 1,712 | 9,908 |
| `planetHit` whole `expect(...,1)` | 8,682 | 2,032 | 9,904 |
| `planetHit` whole `expect(...,0)` | 8,673 | 1,903 | 10,000 |
| `planetHit` first-operand `expect(...,1)` | 8,800 | 1,924 | 9,884 |
| `planetHit` independent-if | 8,595 | 1,696 | 9,896 |
| `!turretMode && !landmarkHit` `expect(...,1)` | 8,004 | 1,770 | 9,908 |
| `field_0x14 != 0` `expect(...,1)` | 6,659 | 1,712 | 9,908 |
| `field_0x14 == field_0x18` `expect(...,1)` | 6,659 | 1,712 | 9,908 |
| `!lockConfirmed` `expect(...,0)` | 6,775 | 1,706 | 9,916 |
| station lock guard `expect(...,1)` | 6,659 | 1,712 | 9,908 |
| `landmarkHit` `expect(...,1)` | 8,515 | 1,842 | 9,908 |
| `lockedStation != nullptr` `expect(...,1)` | 6,659 | 1,712 | 9,908 |
| `field_0x28 != 0` `expect(...,0)` | 6,732 | 1,800 | 9,908 |
| `field_0x18 != 0` plain | 6,659 | 1,712 | 9,908 |
| `isDockingToPlanet` plain | 6,659 | 1,712 | 9,908 |
| `isDockingToPlanet` single `expect(...,1)` | 6,664 | 1,681 | 9,908 |
| asteroid lock guard `expect(...,1)` | 7,205 | 2,037 | **9,904** |
| gas lock guard `expect(...,1)` | 8,144 | 1,737 | 9,912 |
| cargo `== nullptr` plain | 6,675 | 1,690 | 9,908 |
| cargo `expect(...,1)` | 7,074 | 1,750 | 9,912 |
| `inAlienOrbit` plain | 6,662 | 1,712 | 9,908 |
| `inAlienOrbit` `expect(...,1)` | 8,450 | 1,703 | **9,900** |
| `bossSeen` nested-if | 6,683 | 1,683 | 9,908 |
| `bossSeen = true` | 8,475 | 1,740 | **9,900** |
| dead-guard nested-if | 6,659 | 1,712 | 9,908 |
| steal guard `expect(...,1)` | 8,941 | 1,861 | 9,940 |
| `shipGroup == 10` `expect(...,0)` | 6,659 | 1,712 | 9,908 |
| wreck order `UWZ` | 6,661 | 1,706 | 9,908 |
| wreck order `ZWU` | 8,480 | 1,743 | **9,904** |
| tractor store order reversed | 6,660 | 1,708 | 9,908 |
| `egoPos` after `field_0x76 = 1` | 6,660 | 1,704 | 9,908 |
| `(timer < 1) \| missionBlocksPlanets` (int) | 6,659 | 1,712 | 9,908 |
| `(timer < 1) \|\| (mbp != 0)` | 6,663 | 1,718 | 9,908 |
| negated OR | 6,663 | 1,718 | 9,908 |
| independent-if OR | 6,663 | 1,718 | 9,908 |

### 2.3 Whole-function expect re-sweep

Every one of the 27 `__builtin_expect` sites in the function was removed
(27 cells) and flipped 0<->1 (27 cells) on top of `C+T+ae0`. No cell is
below **6,659**; the best removal is neutral (6,659) and the best flip is
neutral (6,659). The landed shape is a local optimum for single-expect
edits.

### 2.4 Plain dock guard size compensation

The original has the dock body *inline* (`13047a beq 130486`), while the
landed `expect(field_0x1a8, 0)` puts it out of line (`bne.w`). Making the
guard plain is 4 B short (9,904). Compensating cells that restore 9,908
(`field_0x18 != 0` `expect(...,0)` -> 6,743; `field_0x1bc >= 1` plain ->
6,738) are all raw-worse than the landed 6,659, so the landing pad is kept.

## 3. Remaining residual

Unchanged: the original keeps `campaign` in `sl` (`12e0f8 mov sl,r0`,
`12e38a cmp.w sl,#2`) and reuses `sl` for `planetTargets` in the planet loop
(`12e4be ldr.w r0,[sl,#4]`), while ours spills `campaign` to `sp+144` and
keeps `planetTargets` in `sp+132`. The three-way rotation
(`otherLock -> r11`, zero/mbp -> `r10`, `campaign` -> spill) still needs a
source shape that raises `campaign`'s spill weight without changing the
block layout; every size-exact campaign-in-`sl` shape measured in wave 175
is raw-worse. The other residual is the tail exception-pad site
(`sp+220` twice vs the original's `220`/`208`), which wave 164 showed is not
source-reachable.

## 4. Reproduction

```sh
ulimit -v 12000000
cp /home/debian/GalaxyOnFire2/scratch/w12108/Radar.final.cpp src/game/weapons/Radar.cpp
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
cmake --build cmake-build-match --target check -j2
cmake --build cmake-build-match --target drift -j2
```

Scratch: `/home/debian/GalaxyOnFire2/scratch/w12108/` —
`Radar.base.cpp` (base source), `Radar.CTae0.cpp` / `Radar.final.cpp`
(landed source), `harness.py` (variant runner), `measure.sh` (build +
`fn_diff`), `sbs.py` / `delta.py` (side-by-side / per-run delta),
`secdiff.py` (section differing-byte counter), `Radar.timer1.cpp`,
`base.so`, `timer1.so`.
