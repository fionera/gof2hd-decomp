# Radar::draw wave 191b (w13587) — full pair-flip sweep + zero/mbp split cells, no win

**Status:** complete, 2026-11-13. Worktree `wt/w13587`, branch `wave191b-w13587`,
base `master` `2a2fcd43e`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.
Read with `radar-draw-wave191b-w13562.md` (RA root cause), `-w13574.md`
(eviction weights), `-w13582.md` (queue recheck). **No source change landed**:
the worktree is clean; every measured cell is raw-neutral (6,013) or worse and
size-exact unless noted.

## 0. Baseline (verified on this worktree)

| metric | value |
|---|---:|
| raw same-address diff | 6,013 |
| instruction-index alignment | 1,965 |
| function size | 9,908 / 9,908 |
| instructions | 3,541 ours / 3,532 orig |
| `.rel.plt` | byte-identical |

Build: `cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs`
then `cmake --build cmake-build-match --target link_so -j2` (inside
`ulimit -v 12000000`).

## 1. New: the complete 30-site × 2 pair-flip sweep (435 cells)

w13562 §2.1 flipped each of the 30 `__builtin_expect` sites in `draw`
individually.  This pass flipped **every pair** of the 30 sites (435
combinations, both values swapped at once) with the 1 s cell harness.
**Result: no win.**  `pairsweep2.log`:

* 435/435 measured, all `size=9908/9908` unless noted;
* best raw = **6,013** (the base colouring, reached by ~200 of the pairs — the
  second flip is dead);
* every strictly different colouring is worse: 6,015–9,500, and the
  size-changing ones are 9,888–9,992.

So the expect-flip space is exhausted: no pair of branch-probability changes
moves `campaign` out of `%stack.45`.

## 2. `missionBlocksPlanets` hint-status sweep (fresh angle)

w13582 §2.6 initialised `missionBlocksPlanets` from `mission` (hint = r5).
Repeated for every physical register available at the declaration point, each
with the size-exact `else { missionBlocksPlanets = 0; }` arm:

```cpp
int missionBlocksPlanets = (int) (intptr_t) X;
if (...) missionBlocksPlanets = mission->getType() != 172;
else     missionBlocksPlanets = 0;
```

| X | raw | size |
|---|---:|---:|
| base (`= false`) | **6,013** | 9,908 |
| `mission` (r5) | 6,013 | 9,908 |
| `player` (r1) | 6,013 | 9,908 |
| `hud` (r2) | 6,013 | 9,908 |
| `mode` (r3) | 6,013 | 9,908 |
| `this` (r0) | 6,013 | 9,908 |

A COPY hint on `%2487` does not move the eviction.

## 3. Zero-split attempts (shorten `%2487`'s weight)

The greedy eviction is weight-driven (`shouldEvict`: `A.weight > B.weight`),
and `%2487`'s weight is dominated by its **two hot `strb` uses in bb.1**
(`plasmaInRange = 0`, `lockConfirmed = 0`, block freq 0.625).  If those two
uses could be given a *different* zero vreg, `%2487`'s weight would fall from
2.60 to ~1.35 (campaign is 1.26).  Every spelling below still CSEs into the
same `t2MOVi 0` vreg:

| cell | raw | size |
|---|---:|---:|
| base | **6,013** | 9,908 |
| `plasmaInRange = plasmaInRange & 0` (both) | 6,013 | 9,908 |
| `= plasmaInRange - plasmaInRange` | 6,013 | 9,908 |
| `= plasmaInRange * 0` | 6,013 | 9,908 |
| `= plasmaInRange ^ plasmaInRange` | 6,013 | 9,908 |
| `lockConfirmed = plasmaInRange` (store→load fwd) | 6,013 | 9,908 |
| `= 0u` / `= (char) 0` | 6,013 | 9,908 |
| `int missionBlocksPlanets = 0;` | 6,013 | 9,908 |
| `int missionBlocksPlanets;` + `else { = 0; }` | 6,013 | 9,908 |
| `int missionBlocksPlanets = (int) (intptr_t) mission;` (no else) | 9,401 | 9,896 |

Moving the two stores after the mission `if` (or after `turretMode`) *does*
split the block but is size-fatal: **9,441 / 9,892**.  Moving them before the
`enabled` check: 9,481 / 9,892.

## 4. `otherLock` / campaign-guard frequency cells

`otherLock` is itself a long-lived vreg (`%124`, span 33,376, weight 2.99) and
sits immediately above `%2487` in the queue, so it was probed too:

| cell | raw | size |
|---|---:|---:|
| base | **6,013** | 9,908 |
| `expect(lockedAsteroid != nullptr, 0)` | 6,049 | 9,908 |
| `expect(lockedGasCloud != nullptr, 0)` | 6,049 | 9,908 |
| `expect(field_0x1c != nullptr, 0)` | 8,520 | 9,912 |
| `expect(lockedStation != nullptr, 0)` | 6,013 | 9,908 |
| whole `otherLock` with `expect(...,0)` | 8,717 | 9,988 |
| whole `otherLock` with `expect(...,1)` | 6,013 | 9,908 |
| bitwise-`\|` `otherLock` | 8,728 | 9,936 |
| reversed `otherLock` chain | 6,116 | 9,908 |
| `expect(campaign >= 2, 1)` | 8,644 | 9,920 |
| `expect(campaign >= 2, 0)` | 9,088 | 9,936 |
| `expect(planetTargets != nullptr, 0/1)` | 6,013 | 9,908 |
| `campaign >= 2 && campaign != -1 && ...` (folded) | 6,013 | 9,908 |
| `campaign >= 2 && campaign < 100000 && ...` | 8,640 | 9,920 |

### 4b. `campaign` vreg-copy-hint attempts

`VirtRegMap::hasKnownPreference()` can set the queue's bit 30 when the COPY
hint is an *unassigned virtual* register (`getPhys(v)==0==getPhys(hint)`).
Routing the call result through a second local was tried to create such a
vreg→vreg copy; clang coalesces all of them away:

| cell | raw | size |
|---|---:|---:|
| `int campaignAlias = campaign;` used in the guard | 6,013 | 9,908 |
| `int campaignTmp = getCurrentCampaignMission(); int campaign = campaignTmp;` | 6,013 | 9,908 |
| same, guard on `campaignTmp` | 6,013 | 9,908 |

## 5. Declaration-block adjacent swaps (fresh)

All 8 adjacent swaps in the `turretMode … planetTargets` declaration block
(lines 354–362) are worse: 6,017–9,180, all size-exact except
`campaign`↔`enemies` (9,180 / 9,904).

Also re-measured: `int otherLock` → `unsigned char` (6,013/9,908);
`int otherLock` → `bool` (9,249/9,860); `notTurretMode`/`notOtherLock` → `bool`
(9,561/9,868, 9,503/9,876).

## 6. Weight model recovered from the pre-RA MIR

Using the block frequencies printed in `prera.mir` (bb.0 successors
37.5 %/62.5 %) and `normalizeSpillWeight = UseDefFreq / (Size + 400)`:

| vreg | what | size | weight |
|---|---|---:|---:|
| `%385` | `__stack_chk_guard` GOT | 105,792 | 2.71 |
| `%380` | `this` | 105,776 | 282.5 |
| `%395` | `Globals::status` GOT | 104,000 | 16.1 |
| `%391` | `Globals::Canvas` GOT | 96,208 | 8.47 |
| `%416` | `turretMode` | 87,632 | 7.15 |
| `%124` | `otherLock` | 33,376 | 2.99 |
| `%2487` | shared zero + `missionBlocksPlanets` | 36,400 | **2.60** |
| `%420` | `alienOrbit` | 22,528 | 1.63 |
| `%422` | `campaign` | 22,320 | **1.26** |

`%2487` is **not** rematerializable in the LLVM sense: its defs are five
`t2MOVi 0` plus `t2SUBri`/`t2MOVCCi`, and `t2MOVCCi` has `isMoveImm` but not
`isRematerializable` in `ARMInstrThrumb2.td`, so the `*0.5` remat discount in
`CalcSpillWeights` does **not** apply.  That is why its weight (2.60) stays
above `campaign`'s (1.26) and it evicts `campaign` from `sl`.

## 7. Conclusion

The pre-RA IR is invariant under every source spelling tried here and in
w13562/w13574/w13582 (≈40 new cells + 435 pair flips this pass).  The residual
is exactly the `%2487` (→`sl`) vs `%422` (→spill) colouring; the only knobs
that reach it (`-mllvm -split-spill-mode=size`) are size-fatal.  No size-exact
raw win was found; `src/` is untouched and the worktree is clean.

## 8. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6013 aligned= 1965 size=9908/9908 insn=3541/3532
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13587/`: `cell.py` (1 s
compile+link+strip+fn_diff), `batch.py`, `cells9.json`–`cells17.json` (the
sweeps above), `pairsweep2.py` / `pairsweep2.log` (435-pair flip),
`prera.mir` / `postra.mir` / `greedy.mir` (MIR dumps), `sim.py` (block-frequency
+ weight extraction), `o2.dis` / `u2.dis` (original/ours disassembly).
