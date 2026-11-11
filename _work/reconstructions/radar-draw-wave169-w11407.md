# Radar::draw wave 169 (w11407) — condition-chain expects

**Status:** complete, 2026-11-11. Worktree `wt/w11407`, branch `wave169-w11407`,
base `master` `ec3ceb07c`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-campaign-sl-wave166-w11106.md`,
`radar-draw-campaign-sl-wave167-w11206.md`,
`radar-draw-wave168-w11307.md`, `radar-draw-wave168-w11308.md`.

## 0. Baseline (verified)

| metric | value |
|---|---:|
| function size | 9,908 |
| raw same-address diff | 7,486 |
| instruction-index alignment | 1,456 |
| instructions | 3,536 ours / 3,532 orig |
| `.rel.plt` | byte-identical |

## 1. Campaign / `sl` allocation (re-confirmed, no lever found)

The whole residual is the campaign-block register allocation: original keeps
`campaign` in `sl` (`12e0f8: mov sl,r0`, `12e38a: cmp.w sl,#2`) and
`missionBlocksPlanets` in `fp`; ours spills `campaign` to `sp+144` and keeps
`missionBlocksPlanets` in `sl`.  This is already visible in the prologue
(`str r3,[sp,#100]` vs `[sp,#104]`, `mov.w fp,#0` vs `mov.w sl,#0`), and it
cascades into the whole campaign block (loop index `fp` vs `sl`, `stations`
in `sl` vs `sp+132`, `marked` in `sp+132` vs `fp`) and the -4-byte shift that
makes every byte from `12e3d0` on differ.

Source shapes tried this pass (all reverted, base 7,486 / 1,456 / 9,908):

| variant | raw | size |
|---|---:|---:|
| swap `plasmaInRange`/`lockConfirmed` stores | 7,488 | 9,908 |
| `Mission*` before `SetColor` | 8,173 | 9,908 |
| bools before `Mission*` | 7,486 | 9,908 |
| `missionBlocksPlanets` `int`/`unsigned`/`char` | 7,641 | 9,912 |
| `campaign` before `turretMode` | 8,537 | 9,900 |
| `campaign` before `alienOrbit` | 7,493 | 9,908 |
| `campaign` `unsigned`/`long`/`const`/`int32_t`/`size_t` | 7,486–7,487 | 9,908 |
| `alienOrbit` `bool` | 8,433 | 9,912 |
| `turretMode` `int` | 9,205 | 9,852 |
| single-expression `missionBlocksPlanets` | 7,487 | 9,908 |
| `missionBlocksPlanets` else-branch / ternary / De Morgan | 7,486–7,487 | 9,908 |
| bools at top / `missionBlocksPlanets` first / zero after mission | 7,486–7,528 | 9,908 |
| `campaign` declared early, assigned in place | 7,486 | 9,908 |
| guard operand reorderings (`planetTargets`/`alienOrbit`/docking first) | 8,474–8,744 | 9,908–9,916 |
| guard `campaign > 1` / `2 <= campaign` / `!(campaign < 2)` | 7,486 | 9,908 |

Per-TU LLVM knobs tried (CMakeLists, all reverted): `-regalloc-csr-first-time-cost`
0/1/2/5/10/20/50/100/500/2000/3000/4000 (neutral), 5000–8000 (7,487),
9000 (7,590), 10000 (7,590), 11000–15000 (9,382, size 9,888),
`-enable-deferred-spilling` (8,378/9,912), `-optimize-regalloc`,
`-disable-spill-hoist`, `-disable-spill-fusing`, `-early-live-intervals`,
`-condsider-local-interval-cost`, `-spp-rematerialization-threshold=0/100`
(all neutral), `-sink-insts-to-avoid-spills` (9,677/10,016),
`-join-liveintervals=false` (10,055/10,304), `-regalloc=basic` (9,833/10,116).

## 2. Landed: condition-chain `__builtin_expect` cells

An automated sweep wrapped each of the 196 `if (...)` conditions in the
function in `__builtin_expect((cond), 0)` and `(cond), 1)` and measured every
cell (scratch `/dev/shm/w11407/auto_expect.py`).  Size-exact raw wins:

| # | condition | expect | raw | aligned | size |
|---|---|---|---:|---:|---:|
| 1 | `enemy->player->enemyFlagsLo` (image chain, line 815) | 0 | 7,486 → **7,304** | 1,456 → 1,554 | 9,908 |
| 2 | `this->dockNavPtr != current` (enemy-lock blip, line 908) | 1 | 7,304 → **7,258** | 1,554 → 1,552 | 9,908 |
| 3 | `(i & 0x7fffffff) == 3` (landmark off-screen blip, line 480) | 1 | 7,258 → **7,251** | 1,552 → 1,540 | 9,908 |
| 4 | `enemyLockTimer > (...) && dockNavPtr != nullptr` (line 905) | 1 | 7,251 → **7,127** | 1,540 → 1,607 | 9,908 |
| 5 | `cloud == locked` (gas-cloud lock blip, line 996) | 1 | 7,127 → **7,127** | 1,607 → 1,606 | 9,908 |

All five are size-exact and `.rel.plt` stays byte-identical; `check` green
(byte 4,507 / linked 4,507 / size 4,524 / data 843 / imports 207 / lint CLEAN).
Final: raw **7,127**, aligned **1,606**, size 9,908.  Whole-`.so` `.text`
differing bytes 82,327 -> **81,968**; `.ARM.extab` 668 -> **712** (the
`(i & 0x7fffffff) == 3` expect alone accounts for 665 -> 712; extab is not
gate-tracked, and the raw win is the primary metric).

## 3. Near misses (not landed)

* `this->lockedAsteroid->isDead()` expect 0: raw 7,312 but size 9,912 (+4).
* `this->field_0x11f` expect 0: raw 7,445 but size 9,912 (+4).
* `landmark !lockConfirmed` expect 0: 7,532 / 9,916 (+8) — unchanged from w11206.
* `current == 143` expect 0: 8,199 / 9,904 (-4).
* `enemyHit` expect 1: 7,347 / 9,908 (worse).
* `wreck` expect 1: 7,651 / 9,908 (worse).

## 4. Reproduction

```sh
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
cmake --build cmake-build-match --target check -j2
# scratch: /dev/shm/w11407/{sweep.py,flag_sweep.py,auto_expect.py,adiff.py,delta.py,regions.py}
```
