# Radar::draw wave 176 (w12007) — cargo-null cold arm + two music-tail hints

**Status:** source change landed, 2026-11-11. Worktree `wt/w12007`, branch
`wave176-w12007`, base `master` `b21088985` (wave 175 merge). Target
`src/game/weapons/Radar.cpp`, `_ZN5Radar4drawEP6PlayerP3Hudi`
(`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave175-w11905.md` (campaign/`sl` residual),
`radar-draw-wave175-w11906.md` (`shipGroup` re-test + `bool wreck`),
`radar-draw-wave175-w11907.md` (planet-lock expect relocation) and
`radar-draw-wave175-w11908.md` (whole-function expect + alias sweep).

## 0. Baseline (rebuilt on this base)

| metric | value |
|---|---:|
| function size | 9,908 |
| raw same-address diff | 6,785 |
| instruction-index alignment | 1,657 |
| instructions | 3,540 ours / 3,532 orig |
| whole-`.so` `.text` differing bytes | 80,166 |
| `.ARM.exidx` differing bytes | 0 |
| `.ARM.extab` differing bytes | 746 (size/base exact) |
| `.rel.plt` | byte-identical (0) |
| gate | RATCHET PASS (byte 4,507 / linked 4,507 / size 4,524 / data 843 / imports 207 / lint CLEAN / drift 0 / parity 5,370) |

## 1. Landed change (three `__builtin_expect` cells)

```diff
                                 Array<int> *cargo = this->lockedEnemy->cargo;
-                                if (cargo == nullptr) {
+                                if (__builtin_expect(cargo == nullptr, 0)) {  // lint: arm_builtin
                                     hud->hudEvent(22, this->level->getPlayer(), 0);
...
         if ((uint32_t) (current - 136) > 15 || ((1 << (current - 136)) & 0xe071) == 0 ||
-            (current != 151 && alwaysEnemySeen)) {
+            __builtin_expect((current != 151 && alwaysEnemySeen), 0)) {  // lint: arm_builtin
...
             if (!ambient) {
                 Globals::sound->stop(current);
-                if (Globals::status->inAlienOrbit() || Globals::status->getStation()->isAttackedByAliens()) {
+                if (__builtin_expect((Globals::status->inAlienOrbit()), 0) || Globals::status->getStation()->isAttackedByAliens()) {  // lint: arm_builtin
                     Globals::sound->play(145, nullptr, nullptr, 0.0f);
```

All three are pure branch-weight annotations; the conditions, short-circuit
order and side effects are unchanged.

* **`cargo == nullptr` (enemy lock cargo tail, line ~895).** `cargo` is the
  `Array<int> *` from `this->lockedEnemy->cargo`. The null arm is the rare
  "no cargo" `hudEvent(22)` path; the non-null arm is the cargo scan. Marking
  the null arm cold is the primary win: **raw 6,785 -> 6,778 (-7)** on its
  own, size 9,908 exact.
* **`current != 151 && alwaysEnemySeen` (music-event guard, line ~1263).**
  This is the third top-level operand of the `field_0x1bc >= 1` music-event
  guard; it is the cold arm (the original reaches the sound-ladder body by
  fall-through). On its own: raw 6,784, size 9,908.
* **`Globals::status->inAlienOrbit()` (ambient-music tail, line ~1312).**
  The original tests this operand with `cbnz` and the following
  `isAttackedByAliens()` with a 32-bit `cmp/beq.w`; hinting the first operand
  unlikely re-places the cold arm. On its own: raw 6,783, size 9,908.

The three cells interact through block layout: the combined body is
**raw 6,776 (-9)**, not the sum of the individual deltas.

## 2. Result

| metric | base | landed |
|---|---:|---:|
| function size | 9,908 | **9,908** |
| raw same-address diff | 6,785 | **6,776** |
| instruction-index alignment | 1,657 | 1,705 |
| instruction count | 3,540 / 3,532 | 3,540 / 3,532 |
| whole-`.so` `.text` differing bytes | 80,166 | **80,157** |
| `.ARM.exidx` differing bytes | 0 | 0 |
| `.ARM.extab` differing bytes | 746 | 746 |
| `.rel.plt` | byte-identical (0) | byte-identical (0) |
| `.rodata` / `.data` / `.bss` | 0 / 0 / 0 | 0 / 0 / 0 |

The aligned count rises 1,657 -> 1,705 (+48) because the cold arms are moved
out of line; raw same-address diff is the fleet's primary local metric and
falls by 9 bytes. All 27 section sizes remain exact; `.rel.plt` stays
byte-identical.

`check` after the change: byte_exact **4,507/4,524**, linked_exact
**4,507/4,524**, size-exact **4,524/4,524**, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data_exact **843/843**, imports **207**, lint
**CLEAN**, drift **0**, parity **5,370/5,370**. `RATCHET PASS`.

## 3. Search performed (all size-exact cells only)

Baseline for every cell: 6,785 / 1,657 / 9,908. All builds were
`cmake --build cmake-build-match --target link_so -j2` under
`ulimit -v 12000000`; scratch is
`/home/debian/GalaxyOnFire2/scratch/w12007/`.

* **Whole-function single-line `if`/`while` expect sweep** (246 cells, every
  single-line condition wrapped in `__builtin_expect((cond), 0/1)`): only
  two size-exact raw wins, the cargo null check (6,778) and the
  `getCurrentCampaignMission() != 145` music-tail guard (6,783).
* **Top-level operand expect sweep** (378 cells, every top-level `&&`/`||`
  operand of every condition): only the `inAlienOrbit()` operand (6,783) and
  the `current != 151 && alwaysEnemySeen` operand (6,784) won.
* **Multi-line whole-condition expect sweep** (52 cells): no raw win.
* **Combination search** over cargo + up to two of
  {`inAlienOrbit` expect, `alwaysEnemySeen` expect, campaign-145 expect,
  `bossSeen` nested-if, `rowHeight` scope moves, far-locked `Player *target`
  alias, planet/asteroid `Hud *h` aliases, `lockedEnemy`/`dockNavPtr`
  aliases}: two combinations tie at **6,776 / 1,705 / 9,908**:
  `cargo + inAlienOrbit + alwaysEnemySeen` and
  `cargo + alwaysEnemySeen + campaign-145`. The landed form uses the
  `inAlienOrbit` cell because the original's `cbnz` / `cmp/beq.w` shape at
  that site is directly witnessed; the campaign-145 cell is a
  codegen-equivalent alternative.
* **Type/shape variants** (all reverted): `int wreck` 9,916; `int bossSeen`
  9,904; `int enemyHit` 9,932; `int alwaysEnemy` 9,912; `int landmarkHit`
  9,928; `int planetHit` 9,944; `int marked` 9,912; `int gasHit` 9,904;
  `int asteroidHit` 9,920; `int turretMode` 9,852; `int onPlanetCourse`
  neutral. `bossSeen` nested-if 6,799; `bossSeen = true` 9,900;
  `bossSeen |=` 9,916. `bool`/`char`/`short`/`uint8_t` `wreck` are 9,916.
* **Planet/station sprite locals** (`S0`/`S1`/`S5`, `P0`/`P1`/`P2`/`P5`/`P6`):
  current `S1+P6` is the best raw shape; `S0+P0` is 6,824/9,908.
* **`Globals::layout` reload variants** (`rowHeight` in one branch, per-branch
  locals, `layoutField(0x4)`): `rowHeight` in the else branch only gives
  6,796/9,908; no raw win.
* **Block-scope `Array<KIPlayer *> *` aliases** for
  `planetTargets`/`enemies`/`landmarks`: 8,919/9,904, 9,086/9,940,
  9,597/9,832; all worse.

## 4. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
cmake --build cmake-build-match --target check -j2
cmake --build cmake-build-match --target drift -j2
```

Scratch: `/home/debian/GalaxyOnFire2/scratch/w12007/`
(`Radar.cpp.base`, `harness2.py`, `variants*.json`, `sweep11.out`,
`sweep_op.out`, `sweep_whole_multi.out`, `sweep23.out`, `secdiff.py`,
`gotlist.py`, `skel.py`).
