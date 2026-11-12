# `Level::createShip` — wave 183 (w12718): LSDA/guard structure and the
# fixed/volumes register swap; no size-exact raw win

Worktree `wt/w12718`, branch `wave183-w12718`, base `master 9555474d8`
(wave 182).  Target `_ZN5Level10createShipEiiiP8Waypointbb`
(`src/game/world/Level.cpp`, 2,764 B, VMA `0xc0214`, 1,117 insns).
Ground truth `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2 `-Oz`).

**Status: note-only.**  `src/` and `CMakeLists.txt` are left at the wave-182
base.  Entry and exit measurements are identical:

| metric | value |
|---|---:|
| raw same-address diff | **1,213** |
| instruction-index alignment | **396** |
| size | **2,764 / 2,764** |
| insns (orig/ours) | **1,117 / 1,117** |
| `.ARM.extab` | **156,312 B**, base `0x1a3b78`, content diff **646** |
| `.rel.plt` | byte-identical (0) |
| full `check` | `byte_exact 4507/4524`, `linked_exact 4507/4524`, size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370, `RATCHET PASS` |

## 1. New evidence: the original LSDA has no `EmpGuard` rows

`_work/reconstructions/extab_lsda_decode.py` on the original and our build
shows the original `createShip` LSDA is 336 B / 57 rows, but its first row is
a *gap*:

```
orig row 0: (0, 406, 0, 0)          ; no landing pad for offsets 0..406
orig row 1: (406, 30, 2488, 0)      ; `new Player(...)` ctor cleanup
orig row 2: (436, 94, 0, 0)         ; gap, includes setEmpData at 474
orig row 3: (530, 32, 2482, 0)      ; `new PlayerFixedObject(...)` cleanup
...
```

The 28 call-site rows are exactly the 28 `new` expressions (Player,
PlayerFixedObject, Array, 24× BoundingAAB, PlayerFighter).  In particular the
`getCurrentCampaignMission()` call at offset 28 and the `setEmpData` call at
offset 474 are **not** call-site rows in the original.

Our current 3-guard source (`mission`, `setEmpData`, fighter `setShipGroup`)
instead emits

```
ours row 0: (28, 4, 2496, 0)        ; mission guard
ours row 1: (32, 374, 0, 0)
ours row 2: (406, 30, 2486, 0)
...
```

so the mission guard splits the original `(0,406,0,0)` row.  Dropping the
mission guard gives the structurally correct 2-guard shape, whose first two
rows match the original exactly:

```
2-guard row 0: (0, 406, 0, 0)       == original
2-guard row 1: (406, 30, 2488, 0)   == original
```

but its record is 332 B (`extab` 156,308, 4 B short).  The mission guard is
the only construct that adds exactly +4 extab bytes (its 5-byte row at offset
28 fits in the existing padding); every other guard tried adds +8 or +12.
This re-confirms the wave-177/180 conclusion.

## 2. The 36-byte 2-guard/3-guard gap is entirely landing-pad code

The 2-guard body (`int mission = getCurrentCampaignMission();`) is
instruction-for-instruction identical to the 3-guard body through the whole
main function.  The 36 raw bytes between 2-guard (1,177) and 3-guard (1,213)
are all in the landing-pad region `0xc0b90..0xc0bd6`:

```
orig  : mov r8,r0 ; mov r0,sl / fp / r9 / r5 ; blx _ZdlPv
3-guard: mov r4,r0 ; mov r0,r9 / r5          ; blx _ZdlPv
2-guard: mov r4,r0 ; mov r0,r5               ; blx _ZdlPv
```

The original deletes the four `new` pointers from `sl`, `fp`, `r9`, `r5`
(fighter, volumes, fixed, player/BoundingAAB temp).  Our main body instead
puts `fixed` in `fp`, `volumes` in `r9`, `this` in `sl`, so the landing pads
use `r4`/`r5`/`r9`.  This is the same register swap that dominates the main
body; fixing it would recover both the landing-pad bytes and the
`0x1c0..0x57f` / `0x840..0x9bf` bytes at once.

## 3. Register assignment (unchanged from wave 177)

| value | original | ours |
|---|---|---|
| `this` | spilled to `[sp,#56]`, reloaded at `0xc03fe` / `0xc071e` / `0xc0b5e` | `sl` from `0xc03fc`, partially live |
| `fixed` | `r9` | `fp` (via `r6` across the ctor) |
| `volumes` | `fp` | `r9` |
| `x` float bits | `sl` (`vmov sl,s20`, reused) | `r6` (`vmov r6,s20`) |
| `y` float bits | `r8` | `r8` |
| `z` float bits | `r4` | `r3` then `sl` |
| `race` | reloaded to `r6` | reloaded to `r4` |
| `type` | `[sp,#52]` | `[sp,#52]` |

The trigger remains the coordinate block: the original keeps `int x,y,z` in
`fp/r4/r8` across `setEmpData` and converts all three afterwards
(`0xc040c..0xc0422`); our build converts `y`/`z` before the call and sinks
`x` below it (`0xc03dc..0xc0424`).  Because `volumes` then outranks `fixed` in
the greedy allocator's spill-weight queue, `volumes` gets `r9` and `fixed`
gets `fp`.

## 4. Sweeps measured this wave (all size 2,764 and extab 156,312 unless noted)

* **Guard subsets** (8 combinations of mission / setEmpData / fighter
  setShipGroup): only the 3-guard base is exact-extab at 1,213.  `drop S`
  = 1,209 / extab 156,304; `drop M` = 1,177 / 156,308; `drop E` = 2,187 /
  156,308 (size 2,780); `drop M,S` = 1,436 / 2,760.
* **Guard additions on `drop M`** (needs +4, never found): nextInt×3,
  getLevel×2, gameWon, hardCoreMode, inAlienOrbit, new Player, new Array,
  getShipGroup (both branches), addObject, setBV, setLevel, setWreckedMeshId
  (all five), ArraySetLength (all four), delete cargo.  Each adds +8/+12.
* **Guard additions on `drop S`** (needs +8): the only exact-extab forms are
  1,214 (guard around `ship->setLevel(this)` or the fighter
  `getShipGroup`) and 1,217–1,243 for the rest; none beat 1,213.
* **Coordinate forms**: `int x, float y, float z` (base) 1,213; `int x,int yi,
  float y,float z` 1,213; `int x,int yi,int zi,float y,float z` 1,213;
  all-int `int x,y,z` 2,238 / size 2,752; `float x,y,z` from ints 2,465 /
  2,732; float-declared-before / assigned-after 2,526 / 2,740.
* **Jitter call × declaration order** (36 combinations): call order
  `(jitter3,jitter2,jitter1)` is best at 1,213 for every declaration order;
  `(1,3,2)`/`(3,1,2)` 1,214; `(2,3,1)` 1,215; `(1,2,3)`/`(2,1,3)` 1,216.
* **Waypoint declaration/assignment**: `int wpz,wpx,wpy` 1,213; `wpz,wpy,wpx`
  1,214; `wpx,wpy,wpz` / `wpy,wpx,wpz` 1,552; assignment order irrelevant.
* **Clamp polarity/if-forms**: level `>20?20:getLevel` 1,213; `<=20?getLevel:20`
  1,227; `>=21?20:getLevel` 1,213; empPoints `<=20?getLevel:20` 1,213;
  `>20?20:getLevel` 1,220; `if` forms 1,213–1,347.
* **`__builtin_expect`**: shipClass 0/1, mission==0x9a, mission chain,
  type==0x2c/0x31, type==0x33, type==14, hardCoreMode, waypoint!=nullptr —
  all 1,213 or worse (most change size).
* **`missionPtr` forms**: current 1,213; no-expect 1,217; swapped 1,217;
  swapped+expect 1,214; `!(==1||==23)` 1,217.
* **`new` expression forms**: normal `new PlayerFixedObject(...)` /
  `new PlayerFighter(...)` 2,235–2,357 / size 2,768–2,772; placement-new of
  `Array` 2,220 / 2,756; separate `BoundingAAB *b` temporaries not tried
  (would add stores).
* **Aliases / qualifiers**: `self`, `lm`, `volumes` reference, `volumes`
  alias, `const`, `volatile` on `volumes`/`this` — 1,213 or worse.
* **Per-TU flags on `Level.cpp`**: removing `-enable-earlycse-memssa=false`
  1,213; `-enable-post-misched=false` 1,213; `-misched-postra=false` 1,213;
  `-enable-if-conversion=false` 1,213; `-disable-postra-machine-sink` 1,213
  (createMission regresses 3,630→3,638); `-regalloc=basic` 2,548 / 2,740;
  `-disable-machine-sink` 2,440 / 2,736; `-enable-tail-merge=false`
  2,784 / 3,016.  No flag flips the fixed/volumes swap.

## 5. Conclusion / next lever

No size-exact, `.rel.plt`-identical raw win was found.  The whole residual is
one allocation decision: `volumes` vs `fixed` for `r9`/`fp`, triggered by the
`int x,y,z`-across-`setEmpData` coordinate block.  A source form that keeps
`int x,y,z` live across the call **and** keeps the current size (2,764) would
be the next thing to try; every int form tested is 12–32 B short.  The
landing-pad decode (`extab_lsda_decode.py`) is the cheapest way to check a
candidate: the original's rows 0/1 are `(0,406,0,0)` / `(406,30,2488,0)`.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12718
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN5Level10createShipEiiiP8Waypointbb
python3 _work/reconstructions/extab_lsda_decode.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so
cmake --build cmake-build-match --target check -j2
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w12718/`: `Level_base.cpp`
(wave-182 base), `h.py` (build + raw/size/insn/extab/rel harness),
`runvar.py`, `buckets.py`, and the `sweep_*.py` guard/coordinate/jitter/
clamp/expect/flag sweeps.
