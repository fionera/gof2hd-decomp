# `Level::createMission` (`_ZN5Level13createMissionEv`) — wave 183 / w12714

Target: `src/game/world/Level.cpp`, function at VMA `0xaffd8`, size **17,172 B**
(`0x4314`). Branch `wave183-w12714`, base master `9555474d8` (wave-182 harvest 5).
Ground truth `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, armeabi-v7a, image base `0x10000`).

This pass continues the wave-182 bool/order sweep on the current body. Nothing
from waves 181/182 is re-landed; the edit below is on top of the wave-182 tree
(raw 3630, `.text` 75,015, `.ARM.extab` 646).

## 1. Result

**One size-exact code win landed: a local `Vector` copy in the `extraAliens`
spawn block.**

```text
                                     size            raw    aligned  insn
base 9555474d8                       17172/17172     3630   1127     6206/6197
after (final)                        17172/17172     3626   1138     6206/6197
```

Section bytes (measured in this worktree):

```text
.text        size 1,259,464 / 1,259,464   differing bytes 75,015 -> 75,011  (-4)
.ARM.extab   size   156,312 /   156,312   differing bytes    646 ->    646  (0)
.ARM.exidx   size    28,528 /    28,528   differing bytes      0 ->      0
.rel.plt     size    29,392 /    29,392   byte-identical (diff 0)
.rodata/.data/.bss/.got/.dynsym/.dynstr/.hash/.rel.dyn   exact
.note.gnu.build-id 20 (unchanged count)
```

Net total-byte win **-4** (75,681 -> 75,677).  `.text` and `.ARM.extab` stay
size- and base-exact (`.ARM.extab` @ `0x1a3b78`, 156,312 B).  The function is
still one of the 15 non-byte-exact bodies, so the ratcheted `byte_exact` count
is unchanged.

Gate after the change: `byte_exact 4507/4524`, `linked_exact 4507/4524`,
`size_exact 4524/4524`, `stub_zero_size 0`, `missing 0`, `wrong_type 0`,
`extra 0`, `data_exact 843/843`, imports **207**, lint **CLEAN ✓**, drift **0**,
symbol parity **5370/5370**, 27/27 headers, file size 2,187,328 exact.
`RATCHET PASS`; no baseline update needed.

## 2. Landed edit

`extraAliens` block (`~line 2064`), the `coords[3]` construction that follows
the three `spawnPosition.{x,y,z} += ...` RNG updates:

```cpp
         this->spawnPosition.z += (float) (Globals::rnd->nextInt(50000) * (Globals::rnd->nextInt(2) == 0 ? 1 : -1) + 20000);
-        int coords[3] = {(int) this->spawnPosition.x, (int) this->spawnPosition.y, (int) this->spawnPosition.z};
+        Vector sp = this->spawnPosition;
+        int coords[3] = {(int) sp.x, (int) sp.y, (int) sp.z};
         Route *route = new Route(coords, 3);
```

`spawnPosition` is a `Vector` member (`Level.h` offset `0x18c`); `Vector` is a
plain `{float x, y, z}` aggregate, so the copy is bit-identical and the three
`(int)` conversions read exactly the same values.  RNG call order is untouched
(the copy is after all three `+=` statements).  The copy gives the register
allocator a single base register for the three loads instead of three
`this+0x18c/0x190/0x194` address computations, which is what the original
emits.

Measured alone on the wave-182 base: raw 3630 -> 3626, `.text` 75,015 ->
75,011, `.ARM.extab` 646 unchanged, size 17172/17172, `.rel.plt` diff 0.
Region-local diff (function offsets `0x3200..0x4260`) 2309 -> 2305.

Equivalent spellings (`int coords[3]; coords[0]=...`, redundant
`(int)(float)` casts, a trailing `(void) sp;`) all measure the same 3626, i.e.
the compiler canonicalises them; the copy itself is the win.

## 3. Sweep methodology and measured variants (all size-exact unless noted)

* **Adjacent-statement swap**, full body (419 candidates): only `L1985`
  (3624) and `L2050` (3628) beat 3630, but both read a stale slot / reorder
  RNG draws, so neither is semantics-preserving.  `L1965` (3629) is the same
  stale-read pattern (`gun` read before `createStaticObject` stores it).
* **Non-adjacent statement moves** (distances 2–5) over the two hot clusters
  (lines 1907–1999, 2360–2400; 120 candidates): only the literal `1965/1968`
  swap (3629, stale read) beat 3630.
* **Call-order alignment**: the original's PLT call multiset is identical
  (672/672) but four call *orders* differ from ours.  Reordering our source to
  match them all measures worse: first convoy loop `setAlwaysEnemy` before
  `setRoute` 3638; second convoy loop `setSpeed`/`setRotationSpeed`/hitpoints
  `speed_first` size-fatal (17164), `hit_mid` 3642; raid loop
  hitpoints/setSpeed/setRotate 3638; `stationPirates` `createStaticObject`
  before `pirateBase` 3632.  All 15 non-empty combinations of those four edits
  are >= 3630.  So the residual is *not* explained by call order — the
  original's block placement / register allocation differs globally.
* **`switch (k)` convoy offsets**: the original emits `offset[1], offset[0]`
  then a shared tail `offset[2]` for **all four** cases (decoded from the
  literal pool: case0 = -3000/20000/-50000, case1 = -7000/10000/-30000,
  case2 = 25000/0/-36000, default = 6000/12000/-45000).  Setting all four
  source cases to that order makes clang collapse the switch (vldr/vstr
  `s16/s18/s20` form), size 17164 — not size-exact.  Single-case permutations
  are all >= 3633.
* **`Vector` local for the raid-loop `setPosition(this->spawnPosition)`**
  (`~line 2020`): 4054, much worse.
* **`j < goods` / `j < total` / `(int)size()-convoyEnd`** for the second convoy
  loop: all size-fatal (17144–17164); the `(unsigned)(convoyEnd+j) < size()`
  form is required.
* **`bool`↔`int`, `unsigned` loop counters, `Globals::*` hoists, `1 - 2*n`
  vs `n==0?1:-1`, `bigAlienWave + 2*(i<0)` spellings**: re-confirmed on this
  body — neutral or worse, matching the wave-182 notes.

## 4. Remaining residuals (unchanged shape)

```text
vma        len  source
0xb3b74    124  pirate-escort `name = getText(441)` / `setToSleep` block
0xb328a     94  alien-turret loop: `(bigAlienWave + 2 * (i < 0))` select is a
                branchless `it/movne` in ours, a `cbz` branch in the original
0xb3a3e     90  pirate raider `setPosition` float schedule
0xb397e     80  raid-loop `createShip`/`getWaypoint` schedule
0xb1688    105  case-2 first convoy loop: ours keeps `offset` in `r9` and
                spills `convoyEnd`; the original spills `offset` to `[sp,#120]`
                and keeps `convoyEnd` in `fp`
```

The two clusters that dominate the raw count are function offsets
`0x1400..0x1a12` (case-2 convoy loops, 647 B) and `0x3200..0x4260`
(raid / stationPirates / extraAliens / convoy / blueprint, 2305 B).  In both,
the original reloads `this` from `[sp,#124]` into `r4` where ours keeps it in
`r8`, and the loop-invariant `&rotation` / `&v` / `k*24` registers are a
different permutation — a whole-function allocation difference that no local
source reorder tried here reproduces.

## 5. Reproduce

```sh
cd /home/debian/GalaxyOnFire2/wt/w12714
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
cmake --build cmake-build-match --target check -j2
cmake --build cmake-build-match --target drift -j2
cmake --build cmake-build-match --target lint -j2
cmake --build cmake-build-match --target diff -j2
```

Expected: `raw=3626 aligned=1138 size=17172/17172`, `.text` diff 75,011,
`.ARM.extab` diff 646, `.rel.plt` diff 0, `RATCHET PASS`, `DRIFT: 0`,
`SYMBOL PARITY ✓`, `CLEAN ✓`.

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w12714/`
(`tryit.py`, `sweep.py`, `reg.py`, `dd.py`, `mv.py`, `mv3.py`, `t*.py`,
`Level.cpp.base`, `orig.dis`, `ours.dis`).
