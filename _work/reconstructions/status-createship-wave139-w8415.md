# Wave 139 (w8415): `Status::missionCompleted` — source case order is
# provably irrelevant (MBP sorts by case value); a 3-level switch nest with
# `__builtin_expect` on each default *does* emit per-group `!prof` weights
# (but not 4:3:2:1).  `Level::createShip` — the RA is invariant under every
# remaining non-EH source form tried

Status: **note-only** (no `src/` change landed).  Gate on the unchanged
tree: `byte_exact 4507/4524`, `linked_exact 4507/4524`, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0.  Worktree `wt/w8415`, base
master `bf123bd47`.  Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2 r328903 / LLVM `1d739ffb`, `-Oz`, image base 0x10000).

Targets:
* `_ZN6Status16missionCompletedEbbx` — original `0xabf38`, **1432 B**,
  baseline raw same-address diff **722 B** (71.0 %).
* `_ZN5Level10createShipEiiiP8Waypointbb` — original `0xc0214`, **2764 B**,
  baseline raw diff **1411 B** (70.6 %), exact 336 B / 57-row `ttype 0xff`
  LSDA.

Raw diff bytes (not the PYTHONHASHSEED-dependent fuzzy %) are the metric.

## 0. New results

1. **Source case order is provably irrelevant to the block layout.**
   Moving the whole `case 167/174` body from the top of the switch to the
   bottom (after `case 172`) compiles to a **byte-identical** 1432 B body
   (raw diff stays exactly 722).  So the w8315 observation that the
   original's block order "is the source order" is a coincidence of the
   group ordering, not evidence that MBP preserves source order: with
   equal weights MBP lays the jump-table case blocks out in *case-value*
   (sorted) order regardless of where the bodies sit in the source.

2. **A 3-level switch nest with `__builtin_expect` on every default DOES
   produce a per-group `!prof` vector** (this is new; w8219/w8315 only
   reported the 2-level fold families).  Reduced harness
   (`/dev/shm/w8415/nest2.cpp`), real clang 7.0.2:

   ```
   switch (__builtin_expect(t, 999)) {          // A + low
     case 167: case 174: ...
     case 8: case 0: case 11: case 13: case 14: ...
     default:
       switch (__builtin_expect(t, 999)) {      // B
         case 150..189: ...
         default:
           switch (__builtin_expect(t, 999)) {  // C
             case 156..172: ...
             default: ...
   ```
   emits `branch_weights` (default first, then the 29 cases in source
   order):
   `2000, 143,143, 143x5, 22000x12, 2000x10`
   i.e. default=2000, A=low=143, B=22000, C=2000.  The fold is
   multiplicative and *does* keep a group-uniform structure — but the
   ratio is `22000 : 2000 : 143`, not `4 : 3 : 2 : 1`.

3. **Exhaustive 3-level nest search: no source shape gives 4:3:2:1.**
   All 216 combinations of `{none, default, any case value}` on each of the
   three switch conditions (grouping A+low | B | C) were compiled with the
   real clang and the emitted `!prof` compared proportionally to
   `1,3,3,4,4,4,4,4,3x12,2x10`: **0 hits**
   (`/dev/shm/w8415/brute.py`).  This closes the "real two-switch source
   shape with per-case weights" question: clang 7.0.2 has no source-level
   producer of the original's vector.

4. **Every two/three-switch shape in the real TU loses.**  Measured on the
   full `Status.cpp` TU, raw diff / size:

   | shape | raw diff | size |
   |---|---:|---:|
   | single switch (master) | **722** | 1432 |
   | nested, no expect (split before 8) | 1312 | 1428 |
   | nested, inner `expect(...,156)` | 1247 | 1396 |
   | nested, outer `expect(...,999)` (split before 8) | 1048 | 1448 |
   | nested, outer `expect(...,999)` (split before 0) | 1267 | 1372 |
   | nested, outer `expect(...,999)` (split before 13) | 1289 | 1380 |
   | nested, outer `expect(...,999)` (split before 150) | 1269 | 1380 |
   | nested, outer `expect(...,999)` (split before 156) | 1329 | 1428 |
   | nested, outer `expect(...,999)` (split before 160) | 1317 | 1416 |
   | nested, outer `expect(...,167)` | 1309 | 1404 |

   The single switch remains the local optimum.  (The IR-injection ceiling
   is unchanged at 558 raw diff / 1432 B — confirmed again this wave — but
   it needs `!prof` metadata that no source form emits.)

5. **createShip: every remaining non-EH form is a no-op.**  All of the
   following compile to the **identical** 2764 B / raw diff **1411** body
   (so they change neither the RA nor the LSDA):

   * `if (Globals::status == nullptr) return nullptr;` → `if (!Globals::status)`
   * `KIPlayer *ship` → `PlayerFixedObject *ship` (+ casts)
   * `switch (type)` hitpoint scaling → `if / else if / else if` chain
   * `int x = ...; float z = ...; float y = ...;` declaration reorder
   * `int type = typeArg;` local copy of `type`
   * `int sc = shipClass;` local copy used in the branch tests
   * `this->lodManager` explicit `this`
   * declaring `KIPlayer *ship = nullptr;` before the `Player` allocation

   Moving the guard to before `new Player` gives 2776 B / 2622 diff;
   before the `empTime` block 2808 B / 2509; dropping it 2748 B / 2354.
   Faithful jitter order Z,Y,X gives 2768 B / 1667; waypoint order
   x,y,z gives 1461.  So the current guard placement is still the best
   local optimum, and the 16 B the original spends instead of the guard
   is still unidentified.

## 1. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w8415/fdiff.py _ZN6Status16missionCompletedEbbx
python3 /dev/shm/w8415/fdiff.py _ZN5Level10createShipEiiiP8Waypointbb
```

Scratch (`/dev/shm/w8415/`): `fdiff.py` (raw-diff scorer), `score.sh`
(rebuild + score), `cc.sh` (exact-TU compile), `inject.py` (IR `!prof`
injection, 558 B ceiling), `nest.cpp` / `nest2.cpp` / `brute.py` (3-level
nest `!prof` search), `mkprof.py` / `mkprof2.py` (text `.profdata`,
counter map from w7781), `csvar*.py` (createShip variant sweeps).

## 2. Open items

1. **Status:** the only remaining lever is the `.profdata` route with the
   non-switch counters tuned so every non-switch branch reproduces the
   `BranchProbabilityInfo` heuristic.  The heuristic probabilities for the
   whole function are dumped with
   `-mllvm -print-bpi -mllvm -print-bpi-func-name=_ZN6Status16missionCompletedEbbx`
   (this wave confirmed the switch heuristic is a uniform `1/30` and the
   zero-heuristics are `20:12`); the front-end profile→weight mapping is
   *not* linear in the counters (all-1 counters already emit
   `2:4, 5:4, 4:5, 0:-2147483648, ...`), so it must be inverted empirically.
   A profile that reproduces the heuristics would land the 558 B ceiling,
   but `-fprofile-instr-use` is a whole-TU flag and would re-weight every
   other function in `Status.cpp` unless their counters are tuned too.
2. **createShip:** the RA (`type -> fp`, `shipClass -> r5/sl/r6`,
   `race -> [sp,#60]`, `this` spilled to `[sp,#56]`, `ship -> r9`) is
   invariant under every source form tried.  A different *statement*
   (not a declaration permutation) is needed; the 16 B gap is still open.

## 3. Addendum: *why* the `Globals::status == nullptr` guard is load-bearing

An instruction-sequence alignment of the guard build against the original
(`difflib` over mnemonic + register-normalised operands) shows the guard is
not just 4 extra instructions — it changes the whole register allocation:

* In the **original**, `Globals` (r4) dies at `hardCoreMode()` (there is no
  later `Globals::status` read), so r4 is immediately reused for `armor`
  (`mov r4, r0`).  `shipClass` therefore stays in r6 (`cmp r6,#0` /
  `cmp r6,#1`), `type` in fp, `race` spilled to `[sp,#60]`, and `this` is
  spilled to `[sp,#56]` so that `ship` can take r9 (`mov.w r9,#0`).
* In the **guard build**, `Globals::status` is re-read after `setEmpData`,
  so r4 stays live; `armor` goes to r6 instead, `shipClass` is spilled to
  `[sp,#60]`, `race` to `[sp,#52]`, and `ship` ends up in r8/fp.  The
  prologue's very first instruction already differs (`str r1,[sp,#60]`
  vs `str r2,[sp,#60]`).

So the faithful body (no guard) is the right *semantics* but is 16 B short
(2748 vs 2764) and the RA re-rolls to 2354 raw diff.  The missing 16 B is
a *statement* that (a) keeps `Globals` alive across `setEmpData` yet (b)
does not appear in the original's post-`setEmpData` code — i.e. it must be
consumed before `setEmpData`.  A likely candidate is a `Globals::status`
field read folded into the `x/y/z` or `empPoints` computation, not a
null-check.  Also note the original computes the `x/y/z` subtractions
**before** `setEmpData` (c03c8-c03e4) while both our builds sink them past
the call; a source form that pins those three values live across the call
would also help.

## 4. Scratch layout

```
/dev/shm/w8415/
  fdiff.py        raw same-address diff for one mangled symbol
  score.sh        rebuild link_so + score
  cc.sh           exact-TU compile (ll or obj)
  inject.py       IR !prof injection (558 B Status ceiling)
  nest.cpp nest2.cpp brute.py   3-level switch-nest !prof search
  mkprof.py mkprof2.py          text .profdata generator
  csvar.py csvar2.py csvar3.py csvar4.py   createShip variant sweeps
```
