# Radar::draw wave 188 (w13211) — expect/alias re-sweep on the 6,309 body: no size-exact raw win

**Status:** analysis complete, 2026-11-12. **No size-exact raw win found**; `src/` is left
at the base. This pass commits only this note. Worktree `wt/w13211`, branch
`wave188-w13211`, base `master` `ce16f45f1` (wave-187 harvest 5). Target
`src/game/weapons/Radar.cpp`, `_ZN5Radar4drawEP6PlayerP3Hudi`
(`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-planetlock-store-order-wave187-w13109.md` (the 6,311 -> 6,309
store-order/`||` cell that is in this body), `radar-draw-wave187-w13110.md` (the
`otherLock`/`campaign` register-allocation lever), `radar-draw-wave186-w13011.md`
(expect/alias re-sweep on the 6,311 body + MIR evidence) and
`radar-draw-wave184-w12811.md` (the 6,315 -> 6,311 hull-bar cell). This note is
self-contained: every number below was measured in this worktree on the 6,309 base.

## 0. Baseline (verified, unchanged)

| metric | value |
|---|---:|
| function size | 9,908 / 9,908 |
| raw same-address diff | **6,309** |
| instruction-index alignment | 1,858 |
| instruction count | 3,541 ours / 3,532 orig |
| `.text` differing bytes (whole `.so`) | 74,223 |
| `.ARM.extab` differing bytes | 639 (size/base exact at `0x1a3b78`, 156,312 B) |
| `.note.gnu.build-id` differing bytes | 20 |
| total differing bytes | 74,882 |
| file size | 2,187,328 |
| gate | byte 4,507 / linked 4,507 / size 4,524 / data 843 / imports 207 / lint CLEAN / drift 0 / parity 5,370 |

`RATCHET PASS` with `src/` at the base. No code change is committed by this pass;
only this note.

## 1. Expect sweep — all 32 existing sites (remove + weight-flip = 64 builds)

Every `__builtin_expect(EXPR, W)` in `draw` was removed (replaced by `(EXPR)`) and
weight-flipped. No edit beats 6,309.

| edit | raw | aligned | insn | size |
|---|---:|---:|---:|---:|
| base | 6,309 | 1,858 | 3,541 | 9,908 |
| `flip17` (`carriesFriendCargoFlag`, lock-image arm) | 6,309 | **1,834** | **3,540** | 9,908 |
| `rm4` (`screenX > centerX - field_0x128`) | 6,309 | 1,858 | 3,541 | 9,908 |
| `rm10` (`field_0x18 != 0`) | 6,309 | 1,858 | 3,541 | 9,908 |
| `rm12` / `fl12` (inner `isDockingToPlanet`) | 6,309 | 1,858 | 3,541 | 9,908 |
| `rm15` / `fl15` (`enemy->player->enemyFlagsLo`) | 6,309 | 1,858 | 3,541 | 9,908 |
| nearest worse: `rm0` (`!mission->isEmpty()`) | 6,310 | 1,858 | 3,541 | 9,908 |
| `rm11` / `fl11` (outer `isDockingToPlanet` expect) | 6,311 | 1,867 | 3,541 | 9,908 |
| `rm13` (`enemyFlagsLo` lock-image test) | 6,312 | 1,863 | 3,541 | 9,908 |
| `rm23` (`dockNavPtr != current`) | 6,312 | 1,858 | 3,541 | 9,908 |

`flip17` is raw-neutral but drops one instruction (3,540) and improves the
instruction-index alignment 1,858 -> 1,834; it is a free stepping stone, not a raw
win. This reproduces the wave-185/186 result on the new body.

## 2. Add-expect sweep — 170 plain `if`/`while` conditions x 2 weights (340 builds)

Every condition in `draw` without an existing `__builtin_expect` was wrapped whole
in `__builtin_expect((cond), 0)` and `__builtin_expect((cond), 1)`. No edit beats
6,309; many are byte-identical. This includes the planet-lock block changed by
w13109 (`planetHit && ...`, `planetLockTimer > field_0x1b8`, `missionBlocksPlanets`,
`field_0x1a8`, the `||` timer guard, `field_0x14` tests) and the whole music tail.

## 3. Alias sweep — 86 block-scoped local inlines

Every initialised local declaration in `draw` was inlined into its enclosing brace
block (single- and multi-use; assigned locals skipped; `.size()` member calls
excluded from the `size` substitution). No edit is a win. Raw-neutral edits:

* byte-identical (6,309 / 1,858 / 3,541): `system@42`, `size@102`, `left@103`,
  `top@104`, `topOk@105`, `landmark@133`, `landmark@139`, `size@261`, `left@262`,
  `cx@387`, `target@394`, `e@479`, `size@508`, `cargo@562`, `current@581`,
  `total@598`, `current@929`.
* same raw, different bytes: `blip@583` (6,309 / **1,843** / **3,540**),
  `size@629` / `left@630` / `top@631` / `size@677` / `level@754` / `size@777`
  (6,309 / 1,860 / 3,541), `left@678` (6,309 / 1,862 / 3,541).

Nearest worse: `techFont@127` 6,310, `lmFont@121` 6,311, `enFont@469` 6,312,
`stationIndex@43` 6,313, `left@509` 6,317. `blip@583` is another free stepping
stone (drops one instruction), not a win.

## 4. Combination sweeps (no win)

Every measurement is a full incremental `link_so -j2` + `tools/fn_diff.py`
(`ulimit -v 12000000`; scratch `/home/debian/GalaxyOnFire2/scratch/w13211/`).

* **All 32 single-edit neutrals x all pairs (496 builds):** no pair below 6,309.
  Best pairs are raw-neutral; `flip17 + blip@583` is 6,309 / 1,857 / **3,539**
  (two fewer instructions), still not a raw win.
* **Curated text cells x all neutrals (160 builds):** no win. The w13109-listed
  neutral cells were re-measured on this body: `field_0x1a8 != 0`,
  `enemy` wreck `isDying()/isDead()` swap, `lockedEnemy` `isDead()/isDying()` swap,
  the `lockImage` arm swap (aligned 1,854) and the skip-guard
  `!hasCargo && isDying()` swap are all raw-neutral 6,309; the gas-cloud
  `isDead()/isDying()` swap is 6,310.
* **Special triples (1,140 builds over {`flip17`, `blip@583`, `rm4/10/12/15`,
  `fl12/15`, the five text cells, `size/left/top` in the gas and asteroid
  crosshair blocks}):** no triple below 6,309.
* **All `isDead()`/`isDying()` operand swaps (8 sites) and their pairs:** no win;
  the asteroid-guard/wreck swaps are 6,311, the gas-cloud swap 6,310.
* **Boolean-local type sweep** (`landmarkHit`, `planetHit`, `bossSeen`, `enemyHit`,
  `gasHit`, `inSight`, `marked`, `asteroidHit`, `topOk`, `wreck`,
  `candidateWreck`, `hasDockNav`, `navWreck`, `alwaysEnemySeen`, `alwaysEnemy`,
  `otherLockFlag` to `int`/`char`/`uint8_t`): no win; `gasHit` as `char`/`uint8_t`
  is 6,311 (size exact), everything else is size-fatal or much worse.

## 5. `campaign` register-allocation lever re-checked on the 6,309 body

The w13110 lever was re-run on this body. The results are unchanged from the
6,311 base (the w13109 `||`/store-order cell is in a different block and does not
move the allocation):

| variant | raw | size | insn | frame |
|---|---:|---:|---:|---|
| base | **6,309** | 9,908 | 3,541 | `0x128` |
| `bool otherLock` | 9,247 | **9,856** | 3,510 | `0x130` |
| `volatile int otherLock` | 9,501 | 9,868 | 3,528 | — |
| `KIPlayer *volatile none = nullptr` for the two stores | 9,229 | 9,868 | 3,514 | — |
| `volatile int zero; otherLock = zero;` | 8,976 | 9,924 | 3,523 | — |
| `bool otherLock` + `field_0x44 = 0` | 8,450 | 9,900 | 3,537 | — |
| `bool otherLock` + `volatile int zero` (effects cancel) | 6,476 | **9,908** | 3,541 | `0x128` |

Every construct that breaks the `%1212 -> %122` zero coalescing and flips
`campaign` into `sl` also changes the frame to `0x130` (or otherwise changes the
size by -52…+24 B); the only size-exact `bool`+`volatile` combination cancels back
to the spilled allocation. `otherLock = this->lockedEnemy != nullptr;` after the
two pointer stores folds to the same constant and is byte-identical (6,309).

## 6. Conclusion

The 6,309 body is the wave-187 w13109 optimum. The expect sweep (existing 32
sites, plus 170 add-hint conditions), the alias sweep (86 declarations), all
pairs/triples of the neutral cells, the boolean-type sweep and the
`campaign`-allocation lever produce **no size-exact raw win**. The residual is
still the `campaign` spill (`sp+0x90` vs `sl`) and its stack-slot/register cascade;
the two free stepping stones (`flip17`, `blip@583`, together 3,539 instructions
and aligned 1,857) do not move the raw count.

## 7. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6309 aligned= 1858 size=9908/9908 insn=3541/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13211/`: `Radar.base.cpp` (the 6,309
base), `harness.py` (build+measure), `alias_sweep.py`, `add_expect.py`,
`combo.py` / `combo2.py` (pairs/triples), `type_sweep.py`, `deadswap.py`,
`campaign.py`, `rhs.py`, `filler.py` / `filler2.py`, and the
`*_results*.json` / `pairs.json` / `special_*_triples.json` dumps.
