# Level::createMission — wave 191b / w13526 sweep summary

Worker w13526, base master `a5abfa894`. No new net-positive raw win;
the tree was left clean at the existing landed commit.

## Summary

I worked `_ZN5Level13createMissionEv` (`src/game/world/Level.cpp`) from the wave-191 baseline and **found no further net-positive raw win**, so the tree is left clean at the existing landed commit.

### Final state (verified)
- `git status --porcelain` → **empty**
- HEAD = `a5abfa894 Level::createMission w13526: case-2 loot setter order (-2 raw)`
- `raw=2659 aligned=871 size=17172/17172 insn=6206/6197`
- `check` → **RATCHET PASS** (byte 4507/4524, linked 4507/4524, stubs 0, missing 0, extra 0, data 843/843, imports 207, lint CLEAN)
- Scratch trimmed to 2.5 MB under `/home/debian/GalaxyOnFire2/scratch/w13526`

### What I established (all measured with `link_so` + `tools/fn_diff.py`)

**Case-183 `switch (k)` arm layout (0xb1418)** — the residual is that ours lays the arms out `0,2,1,default` while the original is `0,1,2,default`. All 24 source-arm permutations were tried (`ABDC`, `ADBC`, `ACDB`, `ADCB`, `BACD`, `BADC`, `DABC`, …): best is the current `0,1,default,2` at 2659; every other order is 2660–5379. `case 3: default:` is neutral; `case 3:` alone 5379. The layout is a `MachineBlockPlacement` decision, not source order (confirmed in the emitted IR: block order is `2989,2990,2991,2992` = source order, yet machine layout differs). The case-2 body's volatile-store-first order is load-bearing (moving it → 5360, removing either `volatile` → 17,168/17,188 size-fatal).

**Case-1 `enemyRace` loop-head reload** — root cause is pre-RA `MachineLICM` hoisting a dereferenceable invariant stack-slot load. Confirmed via `MachineLICM.cpp`: it hoists at `!CanCauseHighRegPressure` before the `AvoidSpeculation` check, so the non-dominating load block doesn't help. Forcing `-mllvm -disable-postra-machine-licm` per-TU gives `raw=15797 size=17188` (fatal); `+cheap-predicable-cpsr` makes it worse (17196). All non-volatile spellings (`int*`, `const int&`, copy, array, `unsigned`) coalesce and stay neutral; `volatile` load → 17,232 fatal.

**Case-2 prologue (0xb16d4)** — the original zeroes `asteroidPos` first, then interleaves `coords[0]; coords[1]=0; coords[2]; coords[3..5]=0`, then `Route`, then `goods`. Faithful order variants measure 2680/2699/3251/3255/3257; the current (zeros first, `goods` before `Route`) is best at 2659. `goods` after `Route` alone is 2678 (aligned improves 871→863, raw regresses).

**Case-2 loot block (0xb1930)** — brute-forced all 32 combinations of `(*enemies)` vs `(*this->enemies)` for the 5 statements: only the all-local form (current) is 2659; all others are 4496–4503 (size-exact) or size-fatal. Declaring the local before the loop is 17,188 fatal.

**Alien turret / fortress / raid region (0xb3200–0xb3dff, ~1,900 diff bytes)** — dominated by a global register-allocation split (original keeps `this` in `r4`, `raidRace` in `r8`, loop index in `fp`, table offset in `r9`; ours uses `r8`/`sl`/`sl`/`r6`). I verified the original really does load `gun` *after* the `createStaticObject` store and calls `setRotation` *between* the two `MatrixRotateVector` calls (our source is semantically inverted there), but every faithful variant is a raw regression: gun-after 2660 (alienGuns) / 2665 (fortress) / 2666 (both); setRotation-between 2669; all 8 combos 2660–2675. `Level *self = this` coalesces (neutral).

**Other sweeps (all neutral or worse)**: adjacent-statement swaps over lines 1958–2100 (126 swaps), 2140–2270, 2400–2440; all pairwise swaps in the case-2 prologue (line 2279–2295) and the wanted block (1862–1876); all 120 permutations of the case-4 tail's `opt44/hunters/new Array/hunterCount/ArraySetLength`; `hasWanted && i == 0` spellings (`&`, `!i`, `i<1`, `i<=0`, `0==i` — all neutral; `i==0 && hasWanted` is 17,164 fatal); `unsigned`/`++i` loop forms; `int k`/`unsigned k`/`long k`.

**Per-TU flag probes** (`-mllvm`, hand-edited `build.ninja`, restored afterwards): `tail-merge-threshold=0` fatal, `=500/1000` neutral; `tail-dup-placement-threshold=2`, `disable-early-taildup`, `enable-misched=false`, `enable-post-misched=false`, `regalloc=greedy` neutral; `disable-postra-machine-sink` 2667; `+cheap-predicable-cpsr` 17,180 fatal; `disable-machine-cse` 18,572; `disable-machine-dce` 17,180; `join-liveintervals=false` 17,824; `split-spill-mode=size` 17,240.

No commit was made (per the handoff: do not commit neutral changes).