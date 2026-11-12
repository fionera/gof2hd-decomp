# Radar::draw wave 191 (w13487) — three operand-order cells: raw 6,058 -> 6,057

**Status:** landed, 2026-11-12. Worktree `wt/w13487`, branch `wave191-w13487`,
base `master` `f325e2d87`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave190-w13493.md` (base 6,058),
`radar-draw-wave190-w13474.md` (statement-order/regalloc sweep),
`radar-draw-wave189-w13319.md` (register map / `campaign`-in-`sl` family).

## 0. The change (three `&&` operand reorderings, all pure predicates)

```diff
-                if (this->screenX > left && this->screenX < left + size && this->screenY > top &&
-                    top + size > this->screenY) {
+                if (this->screenY > top && top + size > this->screenY && this->screenX < left + size &&
+                    this->screenX > left) {
...
-                if (!(this->egoPos.x - this->targetPos.x > 24000.0f) && !(this->egoPos.x - this->targetPos.x < -24000.0f) &&
-                    !(this->egoPos.y - this->targetPos.y > 24000.0f) && !(this->egoPos.y - this->targetPos.y < -24000.0f) &&
-                    !(this->egoPos.z - this->targetPos.z > 24000.0f) && !(this->egoPos.z - this->targetPos.z < -24000.0f) &&
+                if (!(this->egoPos.x - this->targetPos.x < -24000.0f) && !(this->egoPos.x - this->targetPos.x > 24000.0f) &&
+                    !(this->egoPos.y - this->targetPos.y < -24000.0f) && !(this->egoPos.y - this->targetPos.y > 24000.0f) &&
+                    !(this->egoPos.z - this->targetPos.z < -24000.0f) && !(this->egoPos.z - this->targetPos.z > 24000.0f) &&
                     enemy->getType() != 16917) {
...
-            if (this->tractorMode2 && enemy->isDead() && wreck && this->field_0x1c == nullptr) {
+            if (this->tractorMode2 && wreck && enemy->isDead() && this->field_0x1c == nullptr) {
```

All three reorder only side-effect-free comparisons / a const getter (`isDead()`),
so semantics are unchanged.  The planet-box cell is the only one that moves a
byte by itself (-1); the 24000-chain cell is raw-neutral but aligned -13 and the
tractor cell is raw-neutral but aligned -75, and the three compose without
disturbing the frame layout.

| metric | base | after |
|---|---:|---:|
| raw same-address diff | 6,058 | **6,057 (-1)** |
| instruction-index alignment | 2,051 | **1,974 (-77)** |
| function size | 9,908 / 9,908 | 9,908 / 9,908 |
| instruction count | 3,542 / 3,532 | 3,541 / 3,532 |
| `.text` differing bytes (whole `.so`) | 71,811 | **71,810 (-1)** |
| `.ARM.extab` differing bytes | 575 | 575 (no growth) |
| `.ARM.exidx` / `.rel.plt` | identical | identical |
| `.note.gnu.build-id` | 20 | 20 |

`RATCHET PASS`: byte_exact 4,507 / linked_exact 4,507 / size 4,524 / data 843 /
imports 207 / lint CLEAN / drift 0 / parity 5,370.

## 1. Measured cells (all size-exact 9,908 unless noted; base raw 6,058)

### Operand-order permutations of multi-term conditions

| site | cells | best raw | note |
|---|---:|---:|---|
| planet box (`screenX>left && ...`) | 24 | **6,057** | y-pair first + x-pair reversed wins; x-pair must be upper-first |
| enemy 24000 `&&` chain | 48 | 6,058 | all-pairs-reversed is raw-neutral, aligned 2,038 (best aligned of any cell) |
| tractor `tractorMode2 && isDead && wreck && ...` | 24 | 6,058 | `wreck` before `isDead()`: aligned 1,976 |
| landmark box (`screenY < top+size+size && ...`) | 24 | 6,058 | several aligned 1,963-1,972, no raw win |
| `!landmarkHit && i != 3 && notTurretMode && notOtherLock` | 24 | 6,058 | `i != 3` after the loaded bools: aligned 1,969 |
| `!otherLock && !planetHit && !turretMode` | 6 | 6,058 | `!planetHit` first: 9,952 B (fatal) |
| planet-loop guard (`campaign>=2 && ...`) | 24 | 6,058 | most change size |
| enemy slot box (`screenX>left && ... size*2`) | 24 | 6,058 | every permutation changes size (9,892-9,904) |
| gas slot box | 24 | 6,058 | base is best; all others >= 7,153 |
| `inSight` box | 24 | 6,058 | best 6,062 |
| boss arm (`shipGroup==10 && isActive && ...`) | 24 | 6,058 | no win |
| `landmarkHit && !isDockingToAsteroid && ...` | 24 | 6,058 | no win |
| `planetHit && ...` / `stealFlagByte && ...` | 6+6 | 6,058 | 6,059-6,060 |
| `lockedAsteroid==nullptr && ...` (`otherLockFlag`) | 6 | 6,058 | no win |
| `wreck && hasTractor13 || field_0x1b0 && ...` | 2 | 6,058 | 8,418 |
| asteroid slot box (7 terms, curated) | 6 | 6,058 | y-first 6,070, x-reversed 7,426 |

### Other cells tried this wave (all neutral or worse)

- decl-order permutations of the prologue locals (`mission`/`missionBlocksPlanets`/
  `alwaysEnemy*`/`turretMode`/`alienOrbit`/`campaign`): 6 placements, best 6,058;
  `campaign` first = 8,562/9,900.
- assignment-order swap `alienOrbit` <-> `campaign` 6,065; `turretMode` <-> `alienOrbit` 6,079.
- `missionBlocksPlanets` spelling: `= 0`, `== 172 ? 0 : 1`, `!(type == 172)` all 6,058.
- `missionBlocksPlanets` uses: `|| missionBlocksPlanets`, `!= 0`, `> 0`, operand swap -> 6,058-6,312.
- `bossSeen` materialisation: `= true` (9,900 B), `if (shipGroup==10) bossSeen=true`
  (9,992 B), `!(shipGroup ^ 10)` 6,058, `bossSeen || ...` 8,578/9,920.
- `isDying()||isDead()` swaps at the `enemies:` label, boss early-continue,
  wreck test, tractor test: 6,058 each.
- `size >> 1` -> `size / 2` (9,924 B), `left`/`top` declaration swap 6,058,
  `left < screenX` spelling 6,063.
- enemy-hit 5-term chain reorder (`!enemyHit` first) 8,760/9,972;
  `isActive() || (field_0x74 || ...)` 8,478/9,904.

## 2. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6057 aligned= 1974 size=9908/9908 insn=3541/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13487/`: `fast.py` (1 s
compile+link harness), `tryv.py`, `diff2.py`, `seq.py`, `lines.py`,
`q_base.cpp` / `c_pl_e24_trac.cpp` (the landed combination).
