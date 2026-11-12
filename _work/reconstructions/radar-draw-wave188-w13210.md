# Radar::draw wave 188 (w13210) — register-allocation pass: campaign `sl` vs `sp+0x90`

**Status:** analysis complete, 2026-11-12. **No size-exact raw win found**; `src/` is left
at the base. This pass commits only this note. Worktree `wt/w13210`, branch
`wave188-w13210`, base `master` `ce16f45f1`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave187-w13110.md` (the `otherLock` zero-coalescing lever),
`radar-draw-wave187-w13111.md` (expect/alias re-sweep), `radar-draw-wave186-w13010.md`
(the greedy trace) and `radar-draw-wave167-w11206.md` (the `missionBlocksPlanets`
register root cause). This note is self-contained: every number below was measured
in this worktree on the 6,309 base.

## 0. Baseline (verified, unchanged)

| metric | value |
|---|---:|
| function size | 9,908 / 9,908 |
| raw same-address diff | **6,309** |
| instruction-index alignment | 1,858 |
| instruction count | 3,541 ours / 3,532 orig |
| `.rel.plt` | byte-identical |
| gate | byte 4,507 / linked 4,507 / size 4,524 / data 843 / imports 207 / lint CLEAN / drift 0 / parity 5,370 |

`RATCHET PASS` with `src/` at the base. No code change is committed by this pass.

## 1. Prologue / slot map (original vs ours)

Both functions push all eight callee-saved registers and use `sub sp,#296` (`0x128`),
so the frame size is already exact. The residual is one register/slot rotation:

| value | original | ours |
|---|---|---|
| shared zero (`plasmaInRange`/`lockConfirmed`/`missionBlocksPlanets` init) | **fp** (r11) | **sl** (r10) |
| `campaign` | **sl** (r10), `12e0f8 mov sl,r0`; used at `12e38a cmp.w sl,#2` | **sp+0x90**, `12e0f8 str r0,[sp,#144]`; reloaded at `12e38c` |
| `otherLock` | sp+0x90 (`12e38e`) | sp+0x94 (`12e38a`) |
| `turretMode` | sp+0x94 (`12e0e8`) | sp+0x98 (`12e0e8`) |
| `planetHit` | sp+0x98 (`12e3d2`) | sp+0x90 (`12e3ce`) |
| `missionBlocksPlanets` spill | sp+0x70 (`12e3ba str.w fp`) | sp+0x74 (`12e3ba str.w sl`) |
| `stations` | sl (`12e3c6 mov sl,r0`), spilled to sp+0x74 | sp+0x84 (`12e3c6`) |
| planet-loop index `i` | fp (`12e3c8 mov.w fp,#0`) | sl (`12e3c8 mov.w sl,#0`) |
| `mode` arg | sp+0x64 | sp+0x6c |

Everything before `12e0f8` is structurally identical except the zero register
(`mov.w fp,#0` vs `mov.w sl,#0`) and the `mode` slot. The whole planet-loop body
(`0x400`–`0x1800`) then differs because `campaign`/`stations`/`marked`/`planetHit`
all land in different registers/slots. Fixing the allocation would cascade through
the body, so it remains the single highest-value residual.

## 2. Greedy-weight evidence (current 6,309 base)

`VirtRegAuxInfo::calculateSpillWeight` is `totalWeight / (Size + 25*InstrDist)`;
the greedy queue is ordered by interval size (`Prio = (1<<29)+Size`, plus `1<<30`
for a physical copy hint), weight is used only for eviction. Re-derived from the
`Live Register Matrix` dump (`afterall.txt`, `weights.py`):

| vreg | value | totalWeight | def | use |
|---|---|---:|---:|---:|
| `%2482` | shared zero | **2.5998** | 1.2985 | 1.3013 |
| `%124`/`%122` | `otherLock` | **2.0778** | — | — |
| `%419` | `campaign` | **1.2268** | 0.6250 | 0.6018 |
| `%2481` | `missionBlocksPlanets` | 0.4867 | 0.4867 | 0 |

`campaign` has a single use (`cmp #2`) over a ~0x292-byte live range, so its spill
weight is the lowest of the three. The zero wins a callee-saved register first; in
ours it takes `sl`, and `campaign` is evicted to `sp+0x90`. In the original the zero
takes `fp`, `campaign` takes `sl`, and `otherLock` is the one evicted (`sp+0x90`).

The wave-187 finding still holds: the two pointer null-stores in the `enemies:` block
(`lockedEnemy = nullptr; dockNavPtr = nullptr;`) are coalesced into the `otherLock`
vreg and inflate its weight. Breaking that coalescing (`int otherLockFlag`, `bool
otherLock`, `volatile`) does move `campaign` into `sl`, but also changes the
`missionBlocksPlanets` store placement (the shared zero splits into per-block
`MOVi 0` + `str`), so the instruction stream changes and the size moves.

## 3. What was measured this pass (all reverted; size must stay 9,908)

Every variant is a full incremental `link_so -j2` + `tools/fn_diff.py`
(`ulimit -v 12000000`; scratch `/home/debian/GalaxyOnFire2/scratch/w13210/`).

### 3.1 Singles — 100 edits on the 6,309 base

All 32 `__builtin_expect` sites (remove + weight-flip), plus declaration/type edits.
Only 38 are size-exact; the best are raw-neutral:

| edit | raw | aligned | size | zero | campaign |
|---|---:|---:|---:|---|---|
| base | **6,309** | 1,858 | 9,908 | sl | sp+0x90 |
| `e17_flip` (`carriesFriendCargoFlag` 0→1) | 6,309 | **1,834** | 9,908 | sl | sp+0x90 |
| `e8_rm` | 6,315 | **1,824** | 9,908 | sl | sp+0x90 |
| `e14_rm` | 6,384 | **1,821** | 9,908 | sl | sp+0x90 |
| `e22_rm` | 6,990 | **1,788** | 9,908 | sl | sp+0x90 |
| `e27_flip` | 8,526 | **1,771** | 9,908 | sl | sp+0x90 |
| `camp_first` (campaign declared first) | 8,562 | 1,848 | 9,900 | **fp** | sp+0x90 |
| `bossSeen_int` | 9,136 | 2,775 | 9,932 | **fp** | sp+0x94 |
| `turret_bool` (`int turretMode`) | 9,238 | 2,694 | 9,856 | **fp** | **sl** (frame 304) |
| `OLFint` (`int otherLockFlag`) | 9,442 | 1,944 | 9,896 | r2 (split) | **sl** |
| `marked_vol` | 9,707 | 3,401 | 10,188 | fp | sp+0x98 |

No single edit beats 6,309.

### 3.2 Pairs and triples of the size-exact cells

* All pairs of the 38 size-exact singles (703 builds): **no win**; best 6,309
  (`e17_flip + <neutral>`, aligned 1,834). The neutrals (`hasDockNav_int`,
  `alien_uint`, `not_uint`, `not_char`, `OL_char`, `OL_uint`, `OL_long`,
  `enabled_eq0`, `e4_rm`, `e10_rm`, `e12_*`, `e15_*`) are byte-identical to the base.
* All triples of the 14 best-aligned size-exact singles (364 builds): **no win**;
  best 6,309 (`e17_flip + <two neutrals>`, aligned 1,834).
* Random 2–4 edit combinations (499 builds) and 3–5 edit combinations (400 builds)
  over the full 100-edit pool: several hits with `zero=fp` + `campaign=sl`, but all
  are raw-worse because the extra edits change the instruction stream:
  `bossSeen_int+camp_first+e1_flip+not_char` is the only **size-exact** one
  (raw 9,254, aligned 3,127, zero=fp, campaign=sl); `marked_vol+OLF_direct_bool`
  is zero=fp/campaign=sl but 9,836 B; `camp_first+OLFint` is zero=fp but 9,912 B
  and `campaign` is still spilled (`mov r9,r0` then `str r9,[sp,#0x90]`).

### 3.3 The handoff's `OLFint` exact-size combinations

`OLFint` alone is 9,896 B (−12) with `campaign` in `sl` but the shared zero split
into low registers (`movs r2,#0`), so even at size 9,908 the prologue differs.
Measured combinations that land exactly on 9,908 B are raw-worse:

| combination | raw | size | zero | campaign |
|---|---:|---:|---|---|
| `OLFint+marked_int+mbp_bool+e29_rm` | 9,276 | 9,908 | r2 | sl |
| `OLFint+marked_int+mbp_bool+e16_rm` | 9,322 | 9,904 | r2 | sl |
| `OLFint+marked_int+mbp_bool+e10_flip` | 9,282 | 9,904 | r2 | sl |
| `OLFint+bossSeen_int` | 9,441 | 9,956 | fp | sp+0x94 |

The `OLFint` direction is fundamentally raw-hostile: it matches `campaign` but not
the shared zero, and the early 2-byte `movs r2,#0` shifts every following address.

### 3.4 Declaration order / lifetime / one-use aliases (all neutral or worse)

`campaign` as `long`/`int32_t`/`const`/`unsigned`/two-step/`>1`/`2<=`, an alias
(`int campaignAlias`, `const int&`, `bool campaignReady`, `campaign >= 2` hoisted),
and `missionBlocksPlanets` forms (`int`/`long`/`char`/`short`/`unsigned`/`bool`,
`= 0` vs `= false`, no-initializer, separate temp, `?:`/`if`/`!!` assignment) are all
byte-identical to the base or size-fatal. Moving the `missionBlocksPlanets`
declaration before/after `SetColor`/`Mission*` is neutral or worse (`mission_first`
7,002; `mbp_before_setcolor` 6,996). Chained `plasmaInRange = lockConfirmed = 0`
is 6,311 (+2). The compiler folds every alias away, so the vreg structure is
unchanged.

## 4. Conclusion

The `campaign`-in-`sl` residual is real and is the dominant body mismatch, but it is
not reachable by a source edit that leaves the instruction stream untouched. Every
edit that flips the allocation (`int otherLockFlag`, `bool otherLock`, `volatile`,
`camp_first`, `turret_bool`, `bossSeen_int`, `marked_vol`) also changes the shared
zero's live range or the `missionBlocksPlanets` store placement, which either moves
the size off 9,908 or shifts the whole tail. On this base the raw 6,309 is the best
measured value; no size-exact raw win is available from the allocation lever alone.

## 5. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6309 aligned= 1858 size=9908/9908 insn=3541/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13210/`: `Radar.base.cpp` (6,309 base),
`harness.py`, `pool2.py` (100-edit pool), `singles2.json` (all singles),
`pair3.py` / `pairs3.json` (703 pairs), `triple2.py` (364 triples),
`randsearch.py` / `randsearch2.py` (random combinations), `campreg3.py` (campaign
register reader), `afterall.txt` (full MIR dump), `lrm_base_fresh.txt` (Live
Register Matrix), `weights.py` (spill-weight re-derivation), `showdiff.py`
(aligned-diff viewer).
