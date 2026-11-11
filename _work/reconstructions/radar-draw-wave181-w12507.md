# Radar::draw wave 181 (w12507) — stale `arm_builtin` re-hints + two statement swaps (6,356 -> 6,340)

**Status:** landed, 2026-11-11. Worktree `wt/w12507`, branch `wave181-w12507`,
base `master` `1e270aa0e` (wave-180 harvest) plus the w12508/w12509 music-tail
reorders (`70cddec45`, `9924f47d5`), i.e. the 6,356 shape. Target
`src/game/weapons/Radar.cpp`, `_ZN5Radar4drawEP6PlayerP3Hudi`
(`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave180-w12407.md` / `-w12408.md` / `-w12409.md` (the
6,379 base) and `radar-draw-wave181-w12508.md` (the 6,356 music-tail shape this
pass builds on). This note is self-contained: every number was measured in this
worktree on the 6,356 source.

## 0. Baseline and landed result

| metric | base (6,356 shape) | landed |
|---|---:|---:|
| function size | 9,908 | **9,908** |
| raw same-address diff | 6,356 | **6,340** (-16) |
| instruction-index alignment | 1,756 | **1,757** |
| instruction count | 3,542 ours / 3,532 orig | 3,542 / 3,532 |
| whole-`.so` `.text` differing bytes | 76,177 | **76,161** (-16) |
| `.ARM.exidx` differing bytes | 0 | 0 |
| `.ARM.extab` differing bytes | 713 (size/base exact at `0x1a3b78`, 156,312 B) | **713** (unchanged) |
| `.rel.plt` | byte-identical | **byte-identical** |
| `.rodata` / `.data` / `.dynsym` / `.dynstr` / `.hash` / `.rel.dyn` / `.got` | 0 | 0 |
| `.note.gnu.build-id` differing bytes | 20 | 20 |
| file size | 2,187,328 | 2,187,328 |
| total differing bytes | 76,910 | **76,894** |

Gate after the change: byte_exact **4,507/4,524**, linked_exact
**4,507/4,524**, size-exact **4,524/4,524**, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data_exact **843/843**, imports **207**, lint **CLEAN**,
drift **0**, parity **5,370/5,370**. `RATCHET PASS`.

## 1. Landed change (two cells, net size 0)

```diff
@@ cargo-null cold arm (line ~896)
-                                if (cargo == nullptr) {  // lint: arm_builtin
+                                if (__builtin_expect(cargo == nullptr, 0)) {  // lint: arm_builtin

@@ non-combat music guard (line ~1302)
-        if ((Globals::status->getCurrentCampaignMission() != 145)) {  // lint: arm_builtin
+        if (__builtin_expect((Globals::status->getCurrentCampaignMission() != 145), 1)) {  // lint: arm_builtin
```

Both lines still carried a `// lint: arm_builtin` waiver although the hint had
been removed by an earlier pass. Re-adding the hint is semantics-preserving
(pure branch-weight hints; conditions and evaluation order unchanged).

A third cell was landed in the same pass (raw 6,351 -> 6,350, `.text`
76,172 -> 76,171, total 76,905 -> 76,904, size 9,908, `.ARM.extab` unchanged
at 713, `.rel.plt` identical):

```diff
@@ dock-nav arrival guard (line ~886, 24-space indent)
-                        if (this->field_0x1a9) {
+                        if (__builtin_expect(this->field_0x1a9, 1)) {  // lint: arm_builtin
```

`field_0x1a9` is the `ldrb [r4,#425]` byte tested at `0x1300a6` before the
dock-nav arrival block (the `lockedEnemy != dockNavPtr` arm). Wrapping it in
`expect(...,1)` is the only add-hint winner on the 6,351 source: the 175 plain
`if` conditions were each wrapped at weights 0 and 1 (350 builds) and exactly
three sites -- the `lockedEnemy != dockNavPtr` else-if, this `field_0x1a9`
guard, and the six-way `egoPos/targetPos` +/-24000 float condition -- produce
byte-identical output at raw 6,350. They are the same basic block; the hint
belongs on the `field_0x1a9` guard because that is the byte the original tests
immediately before the `cbz`-style skip. Pairing any two of the three is
size-fatal (9,900 B) or neutral.

### 1.1 Landmark-loop store order (6,350 -> 6,341)

Swapping the two independent stores in the landmark loop (line ~451) is a further
raw win of -9 with size unchanged:

```diff
-                this->egoPos = player->getPosition();
-                this->targetPos = this->landmarks->data()[i]->player->getPosition();
+                this->targetPos = this->landmarks->data()[i]->player->getPosition();
+                this->egoPos = player->getPosition();
```

Both right-hand sides are pure getters on distinct objects, so the swap is
semantics-preserving. It is *not* a call-order match: the original emits
`player->getPosition()` first (12e8de) and the landmark's second (12e8f8),
while the swapped source emits them the other way round in our build. The -9
comes purely from the changed live-range/register pressure around the two
`Vector::operator=` calls; raw is the landing metric. `.text` 76,171 -> 76,162,
`.ARM.extab` unchanged at 713, `.rel.plt` identical.

The swap was found by an adjacent-statement sweep: 134 pairs of consecutive
same-indent single-line statements in `Radar::draw` were exchanged one at a
time (each a full build + `fn_diff`). Only this pair beats the base; the next
best is `hasScanner`/`asteroidLockTimer` at 6,349, landed as a second swap
(6,341 -> 6,340, `.text` 76,161, total 76,894, size 9,908, `.ARM.extab` 713,
`.rel.plt` identical):

```diff
-                    auto hasScanner = this->hasScanner19;
-                    this->asteroidLockTimer += mode;
+                    this->asteroidLockTimer += mode;
+                    auto hasScanner = this->hasScanner19;
```

`hasScanner19` is a plain member load and `asteroidLockTimer += mode` touches a
different member, so the exchange is semantics-preserving.

## 2. Why these two sites

The wave-180 sweeps only removed/flipped the *existing* 30 hints. w12408 did
try adding hints to every plain `if` on the 6,407 base and found none, but the
base has since moved by 28 bytes (w12407/8/9 + w12508/9). Re-running the
add-hint sweep on the 6,356 shape finds exactly two winners:

| site | hint | raw | size |
|---|---|---:|---:|
| base | — | 6,356 | 9,908 |
| `cargo == nullptr` (line 896) | `expect(...,0)` | **6,352** | 9,908 |
| `getCurrentCampaignMission() != 145` (line 1302) | `expect(...,1)` | **6,354** | 9,908 |
| **both (landed)** | | **6,351** | **9,908** |

The two cells are nearly additive (-4, -2, combined -5). Both sites are exactly
the ones whose `arm_builtin` waivers were left behind when the hints were
removed in wave 177 (`8900e772b`, `00896c5f8`), so the original source evidently
carried these hints; the reconstruction had simply dropped them.

## 3. Search evidence

Every measurement is a full `link_so` + `tools/fn_diff.py` on this worktree
(`-j2`, `ulimit -v 12000000`).

### 3.1 Add-hint sweep (177 plain `if` conditions x 2 weights = 354)

Each plain `if` condition (one not already starting with `__builtin_expect`)
was wrapped in `__builtin_expect((cond), 0)` and `__builtin_expect((cond), 1)`.
Only two variants beat 6,356: `add94w0` = 6,375 and `add167w1` = 6,377 on the
6,379 base (i.e. `cargo == nullptr` and `getCurrentCampaignMission() != 145`).
Every other add is neutral or worse. On the 6,356 base the same two cells give
6,352 / 6,354.

### 3.2 Existing-hint re-sweep on the 6,356 shape

All 30 current `__builtin_expect` sites were removed and flipped (60 variants):
no single edit beats 6,356; the best are neutral. All C(30,2)=435 removal pairs
were also built on the 6,379 base: the unique minimum is 6,379, no pair wins.
This confirms the wave-180/181 finding that the existing hint space is
exhausted.

### 3.3 Alias/type re-sweep on the 6,379 base

Removing the immutable locals (`stationId`, `currentStation`, `warpGate`,
`campaignMission`, `freelance`, `hasScanner`, `race`, `enemy`, `locked`, `cx`,
`cy`, `target`, `hull`, `emp`, `delta`, `e`, `cargo`, `current`, `frames`,
`elapsedInt`, `total`, `sparks`, `range`, `candidate`, `last`, `canvas`,
`asteroid`, `index`, `distance`) and the type changes (`missionBlocksPlanets`,
`alienOrbit`, `campaign`, `turretMode`, `alwaysEnemySeen`, `alwaysEnemy`,
`otherLock`, `notTurretMode`, `notOtherLock`, `landmarkHit`, `planetHit`,
`marked`, `bossSeen`, `enemyHit`, `wreck`, `inSight`, `gasHit`, `hasDockNav`,
`asteroidHit`, `candidateWreck`, `navWreck`, `ambient`, `onPlanetCourse`,
`otherLockFlag`, `topOk`, `isTarget`) were all measured: none beats 6,379.
Declaration reorderings around `campaign`/`missionBlocksPlanets`/`alwaysEnemy*`
are neutral or worse.

## 4. Remaining residual

Unchanged: the original keeps `campaign` in `sl` (`12e0f8 mov sl,r0`,
`12e38a cmp.w sl,#2`) and later reuses `sl` for the stations array, with the
planet-loop index in `fp`; ours spills `campaign` to `sp+0x90`, keeps the
zero/`planetHit`/`otherLock` stack slots rotated, and uses `sl` for the loop
index. The greedy-RA trace shows `campaign` is live from 2,128 B to 23,992 B
and is spilled because the `onPlanetCourse` block clobbers `r11`; the original
instead spills `autoPilotTarget` to `sp+0x98` and shuffles `sl`/`fp`/`r8`
through `r9`. That cascade is the dominant residual (the enemy bar block, the
planet tail and the stack-slot rotation all follow from it).

## 5. Reproduction

```sh
ulimit -v 12000000
# 6,356 shape = master + w12508/w12509 music-tail commits, then the two hints
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6351 aligned= 1726 size=9908/9908 insn=3542/3532
cmake --build cmake-build-match --target check      # RATCHET PASS
cmake --build cmake-build-match --target drift      # 0 field(s)
```

Scratch: `/home/debian/GalaxyOnFire2/scratch/w12507/` — `Radar.base.cpp`
(wave-180 base), `Radar.w12508.cpp` (6,356 shape), `sweep.py` (build+measure
harness), `addexpect.py` (plain-if finder/wrapper), `type_sweep.py`,
`alias_sweep.py`, `derive_sweep.py`, `reorder_sweep.py`, `combo_sweep.py`,
`secdiff.py`, and the `*.json` measurement dumps.
