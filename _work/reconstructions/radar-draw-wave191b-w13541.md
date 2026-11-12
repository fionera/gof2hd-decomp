# Radar::draw wave 191b (w13541) — enemy-guard operand order ABCDE: raw 6,015 -> 6,013

**Status:** landed, 2026-11-12. Worktree `wt/w13541`, branch `wave191b-w13541`,
base `master` `23a6ebf8b`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave191b-w13535.md` (base 6,015, `lockedEnemy` isDying/isDead
order), `radar-draw-wave191b-w13528.md` (gas/planet/type/statement sweeps) and
`radar-draw-wave191-w13489.md` (the ADEBC guard reorder that this pass partially
reverts).

## 0. The change

The per-enemy guard (`Radar.cpp:835`) was in w13489's reordered form
`A D E B C` (`isAutoPilot`, `isDockedToDockingPoint`, exclude-flag, enemyHit/field_0x24,
wreck/isDead).  The pre-w13489 source order is `A B C D E`:

```cpp
-            if (!this->level->getPlayer()->isAutoPilot() &&
-                __builtin_expect(!this->level->getPlayer()->isDockedToDockingPoint(), 1) && // lint: arm_builtin anchor: Radar::draw
-                !(enemy->countsAsEnemyExcludeFlag != 0 && !wreck) &&
-                (!enemyHit || __builtin_expect((enemy->field_0x24), 0)) &&  // lint: arm_builtin
-                (!wreck || enemy->isDead())) {
+            if (!this->level->getPlayer()->isAutoPilot() &&
+                (!enemyHit || __builtin_expect((enemy->field_0x24), 0)) &&  // lint: arm_builtin
+                (!wreck || enemy->isDead()) &&
+                __builtin_expect(!this->level->getPlayer()->isDockedToDockingPoint(), 1) && // lint: arm_builtin anchor: Radar::draw
+                !(enemy->countsAsEnemyExcludeFlag != 0 && !wreck)) {
```

All five operands are pure getters/field reads, so the short-circuit order is
semantically irrelevant.  The `__builtin_expect` hints stay attached to their own
operands (`B` is the `,0` hint, `D` the `,1` anchor).

## 1. How it was found

An exhaustive 120-permutation sweep of the five guard terms on the current 6,015 base
(scratch `cells_guard.json`, `try.py`).  `ABCDE` is the unique raw winner; every other
size-exact permutation is 6,015 or worse:

| order | raw | aligned |
|---|---:|---:|
| **ABCDE** | **6,013** | 1,965 |
| ABECD | 6,015 | 1,976 |
| ABEDC | 6,015 | 1,968 |
| ADCBE | 6,015 | 1,957 |
| ADEBC (base) | 6,015 | 1,941 |
| DAECB | 6,041 | 1,937 |
| ADCEB | 6,129 | 2,048 |

The aligned metric is 24 worse, but the task's landing rule is a net-positive raw
(same-address) win with size exact and `.rel.plt` identical.  w13489 had landed ADEBC
at raw +2 / aligned -24; on the post-w13535 base the reverse trade is raw -2.

## 2. Measured result

| metric | base | after |
|---|---:|---:|
| raw same-address diff | 6,015 | **6,013 (-2)** |
| instruction-index alignment | 1,941 | 1,965 |
| function size | 9,908 / 9,908 | 9,908 / 9,908 |
| instruction count | 3,541 / 3,532 | 3,541 / 3,532 |
| `.text` differing bytes (whole `.so`) | 71,487 | **71,485 (-2)** |
| `.ARM.extab` differing bytes | 586 | 586 (no growth) |
| `.ARM.exidx` differing bytes | 0 | 0 |
| `.rel.plt` | byte-identical | **byte-identical** |
| `.data` differing bytes | 0 | 0 |

`RATCHET PASS`: byte_exact 4,507 / linked_exact 4,507 / size 4,524 / data 843 /
imports 207 / lint CLEAN / drift 0 / parity 5,370.

## 3. Negative cells re-measured on the 6,015 base

The assigned gas-cloud / planet-box / 24000 / tractor cells were re-measured on the
current base (the w13535 -12 B shift invalidated the old 6,016 numbers).  All are
raw-neutral or worse; none composes with `ABCDE`:

| cell | raw |
|---|---:|
| base | 6,015 |
| enemy 24000 upper-first (`EN`) | 6,015 |
| tractor guard `A C B D` (`TR`) | 6,015 |
| `carriesFriendCargoFlag` expect 1 (`CF`) | 6,015 |
| gas chain ego−target x-y-z | 6,020 |
| planet box `ABCD` | 6,016 |
| gas-spark `+`/`int top` (`SPint`) | 6,031 |
| gas-spark `+`/`float top` (`SPflt`) | 6,019 |
| `offset` type `unsigned/long/short/uint32/int32/size_t` | 6,015 (identical code) |
| `marked` as `int/char/short/uchar/uint/long/int8/uint8/uint16/int32` | 8,465 / 9,912 B (fatal) |
| `plasmaInRange`/`lockConfirmed` zero swap | 6,017 |
| `isDead`/`isDying` swaps at 9 sites | 6,015-6,024 |
| `ABCDE` + EN/TR/CF/PBOX (all 15 combos) | 6,013-6,014 |

A statement-rotation sweep (430 same-depth moves, distance <= 8 lines) found no raw win
below 6,015.  Moving the gas `DrawImage2D` after the 24,000 box (the original's emitted
order) is 6,113; the full original order (draw after the box, unconditional
`field_0x73=0` after the draw) is 6,107.

## 4. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6013 aligned= 1965 size=9908/9908 insn=3541/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13541/`: `try.py` (1 s
compile+link+fn_diff harness), `cells_guard.json` / `cells9.json` (guard permutation and
combination cells), `sweep_rot.py` / `run_rot.py` (statement-rotation sweep),
`detail.py` / `samediff.py` / `shift.py` (alignment / per-address diff tools),
`allpass.txt` (MIR dump, delete after use).
