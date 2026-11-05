# w6514: `Level::createMission` (92.145215 -> 92.293729 %, 17172 B) — wave-117:
# `volatile` size() latch in the case-4 hunter loop is the only measured win

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w6514`, branch `wave117-w6514`,
base `c22678d64`. Ground truth:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; function `0xaffd8`-`0xb431c`, 17172 B).

## Baseline (before this pass)

```text
_ZN5Level13createMissionEv   official 92.145215 %   size 17172 B (size-exact)
                             shape (regs+pc collapsed) 95.049505 %
                             nopc (pc collapsed only)  92.821782 %
                             6055 orig insns / 6065 ours
gate: byte 4501/4524, linked 4501/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, RATCHET PASS
ELF shape unchanged: 27 sections, file 2,187,328 B, .ARM.extab 0x26294 (4 B short).
```

`shape`/`nopc` use the same normalisation as `/dev/shm/w6514/score.py`
(register names + `[pc,#N]` collapsed / only `[pc,#N]` collapsed).

## 1. The win: `volatile unsigned n` in the case-4 hunter loop

`case4Tail` (source line ~2124) sizes `this->enemies`, then iterates a second
time over `this->enemies->size()` latched in `n`:

```cpp
    ArraySetLength(hunterCount, *this->enemies);
    volatile unsigned n = this->enemies->size();   // was: unsigned n
    for (unsigned i = 0; i < n; i++) {
        ...
    }
    this->objectivesA = new Objective(18, 0, hunterCount, this);
```

`n` is only the loop bound (`Objective` uses `hunterCount`), so at `-Oz` clang
folds the second `size()` read into the loop back-edge and the latch is dead.
`volatile` forces the reload and reproduces the original's loop shape.

```text
_ZN5Level13createMissionEv   official 92.293729 %   shape 95.049505 %   nopc 92.970297 %
                             6055 orig / 6065 ours insns, size 17172/17172
```

+0.148514 pp official, +0.148515 pp nopc, shape unchanged, same size and
instruction count. Equivalent spellings (`volatile int n`, `++i`, `i != n`,
`const volatile`) all produce the identical 92.293729 and identical code, so
the minimal `volatile unsigned` form is kept.

## 2. Dead ends measured this wave (all <= 92.293729)

| variant | official % | note |
|---|---|---|
| baseline (no volatile) | 92.145215 | |
| `volatile unsigned n` (kept) | **92.293729** | 17172 B |
| convoy `if (k==2)/else if(k==1)/else if(k==0)` reordered to `0,1,2` | 92.145215 | shape 94.900990, nopc **93.069307** (better branch order, worse regalloc) |
| same reorder as `switch (k) { case 0..2, default }` | 92.145215 | identical codegen to the reordered if-chain |
| `(i == 0) & hasWanted` / `hasWanted & (i == 0)` / `i == 0 & hasWanted` | 92.145215 | neutral |
| `(i==0) && hasWanted` / `i==0 && hasWanted` | 80.481967 | regress |
| turret `(bigAlienWave + (i < 0))` -> `\|\|`, `!=0`, `>0`, `>=1`, `(i<=-1)` | 92.145215 | neutral |
| turret `(bigAlienWave \|\| i<0)`, `\|`, `==1`, `*2` | regress | |
| turret explicit if/else or `bigAlienWave ? :` | 88.231894 | 17140 B |
| `raidRace == 8 && initStreamOutPosition` reorderings | 92.145215 | neutral |
| `&` forms of that condition | 88.590050 | regress |
| `localRace` as `unsigned` | 92.145215 | neutral |
| `localRace` short/char/int8_t/unsigned short | 61.97-72.11 | regress |
| `raid` as char/short/int/unsigned | 91.980198 | regress |
| `raid` declared earlier | 91.303630 | regress |
| `security > 0 && hardCoreMode` reorder | 72.616599 | regress |
| `(security == 3) & raid` -> `raid && security==3` / `raid & (security==3)` | 92.145215 | neutral |
| `&&` form of that | 87.114338 | regress |
| `alienWave && i == ...` | 92.145215 | neutral |
| `&` forms | 75.540518 | regress |
| loop `for (i = 0; i < this->enemies->size(); i++)` (no latch) | 91.308296 | 17164 B |
| `int n` / `int i` / `count` / `i != n` | 92.145215 | neutral |
| `volatile` on count/stationIndex/raidRoll/pirates/security/raid/raidRace/raiders/extraFriend/stationPirates/pirateEscorts/blueprint/alienPercent/extraAliens/convoy/bigAlienWave/alienWave/pirateFortress/alienGuns/fortressGuns/guns/hunters/hunterCount | 59-91.8 | regress |
| convoy `offset` store orders `(0,2,1)` | 92.062706 | regress |
| `Vector offset` variants | 89.77 / 90.21 | regress |
| `int convoyCoords[9]` | 90.209675 | regress |
| plain struct for convoy coords | 90.792079 | regress |
| `Coords6`/`RaidCoords3`/`WingCoords3`/`ShuttleCoords6` -> plain `int[N]` | 92.145215 | neutral |
| `int blueprint` / `int stationPirates` / `int extraFriend` / `int n` / `int hunterCount` | 92.145215 | neutral |
| `missionPtr`/`rnd->reset()`/`createScene()` order swaps | 92.145215 or 78.8 | neutral/worse |
| `bool introSystem` direct-init / `if (!mission)` / brace variants | 92.145215 | neutral |
| `hard`/`raidType` declaration swap | 92.128713 | regress |
| `KIPlayer *ship` caching in raid loot block | 90.798052 | regress |
| `unsigned hunterCount` | 92.277228 | regress |
| `hunters + 2.0f` | 91.072607 | regress |

## 3. Residual (unchanged by this pass)

* Stack-slot swap `status` (`sp+88` orig / `sp+84` ours) vs `raid`
  (`sp+84` / `sp+88`) — first mismatch at instruction 28; 17 refs.
  StackSlotColoring weight model: `raid` 0.01488 (21 refs) > `status`
  0.01221 (21 refs) in our build, so `raid` gets the lower slot. The original
  evidently ranked `status` first. Not reachable from source shape tried so far.
* Convoy `if (k==2) ... else if (k==1) ... else if (k==0) ...` block layout:
  original orders bodies 2,1,0 with tests 2,1,nonzero; our baseline orders
  bodies 0,1,2 with tests 0,1,2. Reordering the source to `0,1,2` fixes the
  test order and lifts nopc to 93.069307 but drops official to 92.145215
  (regalloc), so it is not kept.
* Raid-loot `setPosition` chain (source ~2375-2380): literal-pool pc offsets
  differ by 20 B and `movs r1,#0` is not hoisted the same way.
* `.ARM.extab` still 4 B short (0x26294 vs 0x26298); 27 sections and
  2,187,328 B file size unchanged.

## 4. Commands

```
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w6514/score.py
cmake --build cmake-build-match --target check -j6
```

Scratch analysis tools (not tracked): `/dev/shm/w6514/{score,annot,clusters,
regions,oplines,structlines,weights,slinemap}.py`, `Level.orig.cpp`,
`Level.vol.cpp`, `Level.s`, `show.txt`, `show_vol.txt`, `mi.txt`.
