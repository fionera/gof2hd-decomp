# Radar::draw wave 191b (w13614) — RA flip reachable only via a structural change, no size-exact raw win

**Status:** complete, 2026-11-13. Worktree `wt/w13614`, branch `wave191b-w13614`,
base `master` `8c8c1826d`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.
Read with `radar-draw-wave191b-w13593.md` (definitive post-RA map),
`-w13562.md` (queue priority), `-w13574.md`, `-w13582.md`, `-w13587.md`.
**No source change landed**: `src/` is untouched; every measured cell is
raw-neutral (6,013) or worse and size-exact unless noted.

## 0. Baseline (verified on this worktree)

| metric | value |
|---|---:|
| raw same-address diff | 6,013 |
| instruction-index alignment | 1,965 |
| function size | 9,908 / 9,908 |
| instructions | 3,541 ours / 3,532 orig |
| `.text` differing bytes | 71,196 |
| `.ARM.extab` differing bytes | 577 |
| `.ARM.exidx` differing bytes | 0 |
| `.rodata` differing bytes | 0 |
| zero/`missionBlocksPlanets` reg | `sl` (r10) |
| `campaign` | spilled `sp+0x90` |
| original zero / campaign | `fp` (r11) / `sl` (r10) |

Build: `cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs`
then `cmake --build cmake-build-match --target link_so -j2` inside
`ulimit -v 12000000`.

## 1. New evidence: the RA flip *is* reachable, but only by changing the loop IR

The w13593 conclusion ("the whole residual is one greedy-RA range, every
source spelling leaves the pre-RA IR byte-stable") is correct for the
spellings tried so far, but the colouring **can** be flipped: any change that
introduces a *loop-invariant live range across the planet loop* makes the
allocator put the shared zero in `fp` and `campaign` in `sl` — exactly the
original's colouring.  Measured (zero reg / campaign reg from the emitted
`mov.w rX,#0` and the instruction after the `getCurrentCampaignMission` call):

| cell | raw | aligned | size | insn | zero | campaign |
|---|---:|---:|---:|---:|---|---|
| base | **6,013** | **1,965** | 9,908 | 3541 | sl | spill |
| `Array<KIPlayer*> *targets = this->planetTargets;` used for `update` (`T_update`) | 6,697 | 2,079 | 9,908 | 3526 | **fp** | **sl** |
| same, used for `size()`+`data()` (`targets_loop`) | 6,714 | 2,087 | 9,908 | 3527 | **fp** | **sl** |
| hoisted loop bound `uint32_t n = planetTargets->size()` (`n_local`) | 9,184 | 2,219 | **9,948** | 3528 | **fp** | **sl** |
| `campaign` declared before `turretMode`/`alienOrbit` (`camp_first`) | 8,559 | 1,956 | **9,900** | 3541 | **fp** | spill |
| `camp_first` + hoisted bound + `onScreen`-first | 6,737 | 2,121 | 9,908 | 3527 | **fp** | **sl** |
| `camp_first` + `T_update` | 9,075 | 2,154 | **9,912** | 3528 | sl | **fp** |

So the allocator is not locked into the base colouring; the problem is that
every shape that flips it also changes the loop body (the original reloads
`this->planetTargets` from `[r4,#320]` at the loop header `0x12e3d4` every
iteration, i.e. it does **not** hoist the pointer or the bound), so the flip
costs more raw bytes than it saves.  `T_update` is the cleanest: size-exact and
the prologue now matches (`mov.w fp,#0`), but the loop loses one `ldr` per
iteration and the frame still differs (`mode` at `sp+0x6c`, not `sp+0x64`).

## 2. Cells measured this pass (all reverted; base 6,013 / 1,965 / 9,908)

### 2.1 Planet-loop preamble permutations (U=update, O=onScreen guard, S=stationId, C=currentStation, W=warpGate, T=textOffset)

| order | raw | aligned | size |
|---|---:|---:|---:|
| `USCOWT` = base | **6,013** | **1,965** | 9,908 |
| `UOCSWT` | 6,014 | 1,963 | 9,908 |
| `USOCWT` | 6,014 | 1,961 | 9,908 |
| `UOSCTW` | 6,014 | 1,961 | 9,908 |
| `UOTSCW` | 6,014 | 1,961 | 9,908 |
| `UOSTCW` | 6,014 | 1,961 | 9,908 |
| `UCSOWT` | 6,013 | 1,968 | 9,908 |
| `USCWOT` | 6,023 | 2,044 | 9,908 |
| `UOWSCT` | 9,246 | 2,359 | **10,000** |
| `UOSWCT` | 9,244 | 2,359 | **10,000** |

The binary's order is `U O S C W T` (`0x12e3f2`–`0x12e418`); it is size-exact
but raw 6,014 (the +1 is a single branch-displacement byte, `.text` 71,197 vs
71,196).  No permutation gives raw 6,013 with aligned < 1,965.

### 2.2 Semantic fixes (all individually size-exact, all raw-worse)

| cell | raw | aligned | size | insn |
|---|---:|---:|---:|---:|
| base | **6,013** | 1,965 | 9,908 | 3541 |
| music `A,C,B` | 6,036 | 1,963 | 9,908 | 3542 |
| sound ladder `inAlienOrbit` first | 6,028 | 1,960 | 9,908 | 3541 |
| asteroid `= 0` + `+= mode` | 6,188 | 1,957 | 9,908 | 3541 |
| all three | 6,211 | 1,950 | 9,908 | 3542 |
| `onScreen`-first + music | 6,037 | 1,959 | 9,908 | 3542 |
| `onScreen`-first + sound | 6,029 | 1,956 | 9,908 | 3541 |
| `onScreen`-first + asteroid | 6,189 | 1,953 | 9,908 | 3541 |
| `onScreen`-first + all three | 6,212 | **1,946** | 9,908 | 3542 |

The correct-structure combination reaches the best alignment seen (1,946) but
the worst raw, because the wrong RA colouring dominates the byte metric.

### 2.3 Loop / local shapes (the RA-flip family, see §1)

| cell | raw | aligned | size | zero | campaign |
|---|---:|---:|---:|---|---|
| base | **6,013** | **1,965** | 9,908 | sl | spill |
| `targets` local for `update` | 6,697 | 2,079 | 9,908 | fp | sl |
| `targets` local for `size()`+`data()` | 6,714 | 2,087 | 9,908 | fp | sl |
| `targets` local for `data()` only | 9,227 | 2,217 | **9,952** | fp | sl |
| `targets` local for `size()` only | 9,182 | 2,219 | **9,948** | fp | sl |
| `targets` local for the null check only | 9,233 | 2,215 | **9,956** | fp | sl |
| hoisted bound `n` | 9,184 | 2,219 | **9,948** | fp | sl |
| `n` declared but condition still reloads | 9,184 | 2,219 | **9,948** | fp | sl |
| `n` used after the loop (dead guard) | 8,624 | 2,113 | **9,916** | sl | spill |
| `for (int i …)` / `size_t i` / `!=` / `i += 1` / `i++` / cast size | 6,013 | 1,965 | 9,908 | sl | spill |
| `targets` local (all uses) + `onScreen`-first | 8,880 | 2,099 | **9,904** | fp | sl |

### 2.4 Declaration-order / type spellings

| cell | raw | aligned | size | zero | campaign |
|---|---:|---:|---:|---|---|
| base | **6,013** | **1,965** | 9,908 | sl | spill |
| `campaign` before `turretMode` | 8,559 | 1,956 | **9,900** | fp | spill |
| `campaign` between turretMode and alienOrbit | 6,020 | 1,966 | 9,908 | sl | spill |
| alienOrbit, campaign, turretMode | 6,039 | 1,973 | 9,908 | sl | spill |
| campaign, alienOrbit, turretMode | 6,039 | 1,978 | 9,908 | sl | spill |
| alienOrbit, turretMode, campaign | 6,034 | 1,973 | 9,908 | sl | spill |
| `int32_t campaign` | 6,013 | 1,965 | 9,908 | sl | spill |
| `uint32_t campaign` | 6,014 | 1,965 | 9,908 | sl | spill |
| `intptr_t campaign` | 6,013 | 1,965 | 9,908 | sl | spill |
| `missionBlocksPlanets` as `int8_t`/`char`/`unsigned char`/`int16_t`/`long`/`unsigned` | 6,013 | 1,965 | 9,908 | sl | spill |
| `bool otherLock` | 9,249 | 2,415 | **9,860** | fp | sl |
| `bool notTurretMode` | 9,561 | 2,113 | **9,868** | sl | sl |
| `bool notOtherLock` | 9,503 | 2,144 | **9,876** | sl | spill |
| `int planetHit` | 8,460 | 2,143 | **9,912** | sl | spill |
| `int marked` | 8,464 | 2,025 | **9,912** | sl | spill |
| `unsigned stationId` / `unsigned currentStation` / `unsigned warpGate` / `unsigned textOffset` | 6,013 | 1,965 | 9,908 | sl | spill |
| `if (!onScreen) continue;` (no braces) / `== false` / `0 ==` | 6,013 | 1,965 | 9,908 | sl | spill |
| `campaign > 1` / `2 <= campaign` | 6,013 | 1,965 | 9,908 | sl | spill |
| `campaign + 0` | 6,013 | 1,965 | 9,908 | sl | spill |
| `campaign & 0x7fffffff` | 8,626 | 2,123 | **9,916** | sl | fp |
| `campaign > -1 ? campaign : -1` | 9,379 | 2,087 | **9,924** | sl | r0 |
| `planetTargets != nullptr && campaign >= 2 …` | 8,791 | 2,238 | **9,924** | sl | spill |

### 2.5 `-mllvm` probes (diagnostic only)

| flag | raw | aligned | size |
|---|---:|---:|---:|
| `-join-globalcopies=false` | 6,013 | 1,965 | 9,908 |
| `-join-splitedges=false` | 6,013 | 1,965 | 9,908 |
| `-terminal-rule` | 6,015 | 1,969 | 9,908 |
| `-join-liveintervals=false` | 10,156 | 4,624 | 10,452 |
| `-split-spill-mode=size` | 9,415 | 2,250 | 9,892 |
| `-stress-regalloc=13` | 9,691 | 2,336 | 10,084 |
| `-stress-regalloc=12` | 9,876 | 3,725 | 10,316 |
| `-stress-regalloc=11` | 9,997 | 4,297 | 10,332 |

## 3. Conclusion

The campaign/zero colouring is not an immovable greedy-RA tie: a
loop-invariant live range in the planet loop flips it to the original's
`zero=fp / campaign=sl`, and `campaign`-before-`turretMode` alone moves the
zero to `fp`.  But the original's loop reloads `this->planetTargets` from
`[r4,#320]` at the header every iteration (no hoisted pointer/bound), so every
flip shape that is size-exact is structurally wrong and raw-worse
(best 6,697 vs base 6,013).  Conversely every structurally-correct shape
(`onScreen`-first, music `A,C,B`, sound ladder, asteroid arm) leaves the base
colouring and is raw-worse.  No size-exact raw win was found; `src/` is
untouched and the worktree is clean.

## 4. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6013 aligned= 1965 size=9908/9908 insn=3541/3532
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13614/`: `cell.py` (1 s
compile+link+strip+fn_diff cell), `diag.py` (extracts the zero and campaign
registers from the emitted `.so`), `sweep.py` (`apply`/`measure`/`runall`),
`cands1.py`–`cands12.py` (the sweeps above), `Radar.base.cpp` (pristine
source), `ra.txt` (`-print-machineinstrs`), `orig.dis` / `ours.dis`.
