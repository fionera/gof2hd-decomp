# Radar::draw wave 187 (w13110) — register-allocation pass: the `otherLock` zero-coalescing lever

**Status:** analysis complete, 2026-11-12. **No size-exact raw win found**; `src/` is left
at the base. This pass commits only this note. Worktree `wt/w13110`, branch
`wave187-w13110`, base `master` `8b5d4281c`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave186-w13010.md` (the greedy trace and weight accounting),
`radar-draw-wave186-w13011.md` (MIR evidence) and `radar-draw-wave186-w13009.md`
(landing-pad pass). This note is self-contained: every number below was measured in
this worktree on the 6,311 base.

## 0. Baseline (verified, unchanged)

| metric | value |
|---|---:|
| function size | 9,908 / 9,908 |
| raw same-address diff | **6,311** |
| instruction-index alignment | 1,903 |
| instruction count | 3,544 ours / 3,532 orig |
| gate | byte 4,507 / linked 4,507 / size 4,524 / data 843 / imports 207 / lint CLEAN / drift 0 / parity 5,370 |

`RATCHET PASS` with `src/` at the base. No code change is committed by this pass.

## 1. The exact lever (re-derived and confirmed)

`VirtRegAuxInfo::calculateSpillWeight` is `totalWeight / (Size + 25*InstrDist)`, where
`totalWeight = sum over defs/uses of blockFreq/entryFreq` (LLVM 7.0.2
`LiveIntervals::getSpillWeight`; no loop-depth factor). The greedy queue is ordered by
**interval size** (`Prio = (1<<29)+Size`, plus `1<<30` for a physical copy hint), *not* by
weight; weight is only used for eviction.

Relevant vregs on the 6,311 base (assertions `llc`, exact NDK clang 7.0.2 codegen):

| vreg | value | interval | Size | weight |
|---|---|---|---:|---:|
| `%417` | `campaign` | `[2128,24448)` | 22,320 | 5.946482e-05 |
| `%2481` | shared zero (`lockConfirmed=0`, `plasmaInRange=0`, `missionBlocksPlanets`) | `[240,36608)` | 36,368 | 7.631079e-05 |
| `%122` | `otherLock` | `[23952,57232)` | 33,280 | 9.069345e-05 |

`%417` is copy-hinted `$r0`, so it is allocated first in the hinted group; it takes `$r11`
(fp), is evicted by `%411` (`turretMode`) to `$r6`, then to `$r10` (sl). `%2481` (longer,
non-hinted) is popped next, evicts `%38` from `$r11` and takes fp. Then `%122` (weight
9.07e-05 > 7.63e-05) evicts `%2481` from `$r11`; `%2481` re-queues, evicts `%417` from
`$r10`, and `%417` is spilled to `sp+0x90`. That is exactly the residual: the original has
`campaign` in `sl` (`12e0f8 mov sl,r0`, `12e38a cmp.w sl,#2`) and the zero in `fp`.

So the single lever is: **`%122` must not evict `%2481` from `$r11`**, i.e. lower
`%122`'s weight below 7.63e-05, or raise `%2481`'s above 9.07e-05.

### 1.1 `%122` weight decomposition (WTRACE, patched Android llc)

| mbb | freq | MI | w |
|---|---:|---|---:|
| 173 | 2.3056e9 | `%1150 = ORRrr %122,%2507` (line 596 `!otherLock && !planetHit && !turretMode`) | 0.9361 |
| 213 | 7.1398e8 | `%122 = MOVi 0` (`otherLock = false`, line 677) | 0.2899 |
| 213 | 7.1398e8 | `STRi12 %122,[r4,#4]` (`lockedEnemy = nullptr`, line 676) | 0.2899 |
| 213 | 7.1398e8 | `STRi12 %122,[r4,#8]` (`dockNavPtr = nullptr`, line 678) | 0.2899 |
| 121 | 9.5198e8 | `MOVi 1` (line 533 OR chain) | 0.3865 |
| 320 | 2.9072e8 | `MOVCCi` / `CMPri` / `ORRrr %411` (line 870) | 0.2361 / 0.1180 / 0.1180 |
| 123/125/126 | — | `MOVi 1`, `MOVCCi`, `LDRi12`, `CMPri` | 0.1450 / 0.0544 / 0.0652 / 0.0326 / 0.0326 |
| **total** | | | **3.0634** |

The two `STRi12` at mbb 213 are **19 % of `%122`'s weight** (0.58 / 3.06) — exactly the
margin needed (9.07e-05 × 0.81 = 7.35e-05 < 7.63e-05).

### 1.2 Why those two stores are uses of `%122`

In the machine IR the `enemies:` block is

```
38864B  %1212:rgpr = t2MOVi 0          ; the shared zero (machine CSE)
38880B  t2STRi12 %1212, %375, 4        ; this->lockedEnemy  = nullptr
38896B  t2STRi12 %1212, %375, 8        ; this->dockNavPtr   = nullptr
38928B  %122:rgpr = COPY %1212         ; otherLock = false (phi input)
```

`RegisterCoalescer` merges `%1212` into `%122` (`merge %122:2@38928r into %1212:0@38864r`),
so the two pointer stores become uses of the `otherLock` vreg. The original binary's
enemies block is **byte-identical** (`12ec3e movs r1,#0; movs r0,#0; str r1,[r4,#4];
str r0,[sp,#144]; str r1,[r4,#8]`) — the two separate `movs` prove the original also has
two zero vregs, but its `otherLock` vreg does *not* own the two pointer stores, i.e. the
original's coalescer did not merge the phi zero with the store zero. The source construct
that produces that split is what this pass tried to find.

## 2. Breaking the coalescing *does* flip the allocation

Confirmed experimentally: every variant that prevents the `%1212 → %122` merge moves
`campaign` into `sl` (`12e0f6/12e0f8 mov sl,r0`). But every one is size-fatal:

| edit | raw | size | insn | camp |
|---|---:|---:|---:|---|
| base | **6,311** | 9,908 | 3,544 | spill `sp+0x90` |
| `bool otherLock` (i1 zero ≠ i32 store zero) | 9,241 | **9,856** | 3,513 | **sl** |
| `bool otherLock` + `notOtherLock = !otherLockFlag` | 9,241 | **9,856** | 3,513 | **sl** |
| `volatile int otherLock` / `int volatile otherLock` | 9,497 | **9,868** | 3,529 | **sl** |
| `KIPlayer *volatile none = nullptr;` for the two stores | 9,223 | **9,868** | 3,517 | **sl** |
| `KIPlayer *const volatile none` | 9,223 | **9,868** | 3,517 | **sl** |
| `volatile int zero = 0; otherLock = zero;` | 8,979 | **9,924** | 3,526 | spill `sp+0x9c` |
| `volatile bool zero = false; otherLock = zero;` | 9,167 | **9,932** | 3,528 | spill `sp+0x9c` |
| `volatile char zero` | 9,018 | **9,928** | 3,527 | spill `sp+0x9c` |
| `KIPlayer *volatile none = this->lockedEnemy;` | 9,019 | **9,928** | 3,528 | spill `sp+0x9c` |
| `bool killed; if (killed) {stores} if (killed) otherLock = false;` | 9,246 | **9,940** | 3,536 | **sl** |
| `bool otherLock` + `this->field_0x44 = 0;` | 8,448 | **9,900** | 3,540 | spill |
| `bool otherLock` + `volatile int zero` (the two effects cancel) | 6,478 | **9,908** | 3,544 | spill |

The last row is the only size-exact `bool`+`volatile` combination found; the allocation
flips back to the base (campaign spilled) and the raw is 167 worse than the base.

The campaign-in-`sl` variants are 40–52 B *shorter* than the original (9,856–9,868 vs
9,908): removing the `campaign` spill/reload saves more bytes than the original's
compensating shape residuals (the extra `.ARM.extab` landing pad, the if-converted
`navWreck`/`dockNavPtr` loads, the `bossSeen` `subs/clz/lsrs` vs `it eq; moveq`, the
music-tail cold-block placement). Because `raw` is a **same-address** byte diff, any early
size change mis-aligns the whole tail, which is why these variants measure ~9,000 raw even
though their allocation matches the original.

**Frame-size note.** The base and the original both use `sub sp,#296` (`0x128`); the
`campaign` spill to `sp+0x90` reuses an existing slot. Every campaign-in-`sl` construct
found here grows the frame to `sub sp,#304` (`0x130`, `bool otherLock`) or changes it
otherwise, so even the prologue stops matching. A *free* coalescing break (one that keeps
the frame at `0x128`) was not found: `static int zero; otherLock = zero;` folds to the
constant (identical 6,311), and `static volatile int zero;` / `static volatile bool zero;`
give 9,932 with `campaign` still spilled (`sp+0x9c`).

## 3. Other experiments measured this pass (all reverted, size must stay 9,908)

Every variant is a full incremental `link_so -j2` + `tools/fn_diff.py`
(`ulimit -v 12000000`; scratch `/home/debian/GalaxyOnFire2/scratch/w13110/`).

### 3.1 `enemies:` block rewrites that keep the coalescing (all byte-identical 6,311 / 9,908)

* statement order permutations of `lockedEnemy=nullptr; otherLock=false; dockNavPtr=nullptr;`
  (4 non-base orders);
* `otherLock = 0; / (bool)0; / !1; / 1-1; / 0x0; / (int)false; / (int)(bool)false; / otherLockFlag = false;`
* `this->lockedEnemy = (KIPlayer *) 0;` and `= 0;` for both stores;
* `this->lockedEnemy = this->dockNavPtr = nullptr;` (chained);
* `this->dockNavPtr = this->lockedEnemy;` (forwarded to null);
* `KIPlayer *none = nullptr;` (non-volatile) for both stores;
* `bool tmp = false; otherLock = tmp;` and `bool zero = false;` used for the stores;
* guard operand swap `isDead() || isDying()`;
* `bool killed` hoist + two `if (killed)` blocks;
* `if (this->lockedEnemy == nullptr) otherLock = false;` after the stores (K2) — this one
  *does* flip to `sl` but size 9,940;
* `int otherLock` -> `char` / `uint8_t` / `long` / `int32_t` / `unsigned` (all identical
  except `unsigned` 6,312).

### 3.2 `bossSeen` shapes (the `subs/clz/lsrs` vs `it eq; moveq` residual)

`bossSeen = (x == 10) ? true : bossSeen;` 9,223 / 9,996; `if (x == 10) bossSeen = true;`
9,213 / 9,988; `bossSeen = bossSeen || x == 10;` 9,208 / 9,976; `bossSeen |= x == 10;`
9,223 / 9,996. All size-fatal; the base clz form is the only size-exact one.

### 3.3 Extra zero store

`this->field_0x44 = 0;` (or `field_0x2c`/`field_0x20`) after the three statements: 6,417 /
9,908 / insn 3,543 (aligned 1,883) — size-exact but 106 raw worse; camp still spilled.

## 4. Conclusion

The `campaign`-in-`sl` residual is caused by `otherLock` (`%122`) evicting the shared zero
from `$r11`, and 19 % of `%122`'s weight is the pair of pointer null-stores that the
register coalescer merged into the `otherLock` vreg. Breaking that merge flips the
allocation to the original (`campaign` in `sl`) but every source construct that breaks it
(`bool otherLock`, `volatile` locals, a separate `if`) changes the function size by
−40…+32 B; the only size-exact combination (`bool otherLock` + `volatile int zero`)
cancels back to the spilled allocation. With size sacred and `raw` a same-address diff, no
size-exact raw win is available from the register-allocation lever alone on this base.

## 5. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6311 aligned= 1903 size=9908/9908 insn=3544/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13110/`: `Radar.base.cpp` (6,311 base),
`harness.py` (build+measure+campaign register), `batch_ol.py`, `batch2.py`…`batch11.py`
(the cells above), `Radar.v1.cpp` (the `volatile` variant), `v1.dis`, `orig.dis`,
`ours.dis`, `aldiff.py`, `wtrace.log` (WTRACE per-MI weights), `regalloc702.log` (full
greedy trace), and `llvm702/` (assertions-enabled Android LLVM 7.0.2 `llc`).
