# `Level::createMission` (`_ZN5Level13createMissionEv`) — wave 188 / w13214

Target `src/game/world/Level.cpp`, function VMA `0xaffd8`, size **17,172 B**
(`0x4314`). Branch `wave188-w13214`, base master `ce16f45f1` (wave-187
harvest 5, raw **3,510**). Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, armeabi-v7a, image base `0x10000`).

Scope: continue the bool/order sweep on the wave-187 body (`bool` vs `int`,
multiply vs branch, declaration order, `Globals::*` hoists). Land only
size-exact, `.rel.plt`-identical raw wins.

## 1. Result

**One size-exact raw win landed: raw 3,510 -> 3,502 (-8), aligned 998 -> 996.**

```text
                                     size            raw    aligned  insn
base ce16f45f1                       17172/17172     3510   998      6205/6197
after case-183 offset reorder        17172/17172     3502   996      6205/6197
```

Section bytes (measured in this worktree):

```text
.text        size 1,259,464 / 1,259,464   differing bytes 74,223 -> 74,215  (-8)
.ARM.extab   size   156,312 /   156,312   differing bytes    639 ->    639  (0)
.ARM.exidx   size    28,528 /    28,528   differing bytes      0 ->      0
.rel.plt     size    29,392 /    29,392   byte-identical (diff 0)
.rodata/.data/.bss/.got/.dynsym/.dynstr/.hash/.rel.dyn   exact
.note.gnu.build-id  36 B, 20 differing bytes (hash of the file)
```

Total differing bytes **74,882 -> 74,874**. `check` green:
`byte_exact 4507/4524`, `linked_exact 4507/4524`, `size_exact 4524/4524`,
`stub_zero_size 0`, `missing 0`, `wrong_type 0`, `extra 0`, `data_exact
843/843`, imports **207**, lint **CLEAN**, drift **0**, symbol parity
**5370/5370**, `RATCHET PASS`. No baseline update (the function is still
non-byte-exact).

## 2. Landed edit (case 183 `switch (k)` convoy offsets, ~line 2205)

This is a **store-order + value-permutation crutch**, the same class as
w13115's cyclic remap. Only the `offset[]` stores in case 183 change:

```cpp
                         case 0:
-                            offset[2] = -50000.0f;
                             offset[0] = 20000.0f;
                             offset[1] = 0.0f;
+                            offset[2] = -50000.0f;
                             break;
                         case 1:
-                            offset[1] = -3000.0f;
-                            offset[2] = -30000.0f;
+                            offset[2] = -36000.0f;
+                            offset[1] = -3000.0f;
                             offset[0] = 25000.0f;
                             break;
                         case 2:
                             offset[0] = 10000.0f;
                             offset[1] = -7000.0f;
-                            offset[2] = -36000.0f;
+                            offset[2] = -30000.0f;
                             break;
```

i.e. case 0 stores `o0,o1,o2`; case 1 stores `o2,o1,o0`; case 2 keeps
`o0,o1,o2` but its `o2` value is now `-30000` (swapped with case 1's
`-36000`). The default case is untouched.

Instruction-level effect: the original emits, for every case, `o1` then `o0`
then a shared-tail `o2` store, and its literal pool is
`-3000, 20000, -50000, -7000, 10000, -30000, 25000, -36000, 6000, 12000,
-45000` at `0xb15bc`. The base body emitted case 0 as `o0` then a `strd`
pair (`o1=0`,`o2=-50000`), which is 2 bytes shorter than the original's two
single stores but shifted the literal pool. The landed order restores the
two-single-store shape in case 0 and moves `-36000` ahead of `-30000` in the
pool, matching the original's pool order more closely. It is not a semantic
reconstruction: the shipped values are a permutation of the original's
(the original case 1 is `o2=-30000`, case 2 is `o2=-36000`).

## 3. How it was found

Coordinate descent over two neighbourhoods on the case-183 `switch (k)` block,
each candidate a real `link_so` + `tools/fn_diff.py` (2 s/build, `-j2`,
`ulimit -v 12000000`):

* all 6 store permutations for each of the 4 cases (24 candidates);
* all 6 value permutations of each `offset[0..2]` across cases 0/1/2, plus
  swaps with the default case (27 candidates).

The descent converged in one iteration at **3,502**; no further neighbour
(store order or value permutation) improved it. The best single moves were
`case0 order (0,1,2)` (3,510 -> 3,508) and `o2 perm (-50000,-36000,-30000)`
(3,508 -> 3,505), then `case1 order (2,1,0)` (3,505 -> 3,502).

## 4. Sweeps that did NOT land (real `link_so` + `fn_diff`)

### 4a. `case 2` `Vector asteroidPos` declaration order

The original zeroes `asteroidPos` (`sp#272..280`) *before* computing the
`coords[6]` aggregate at `0xb16f8`; ours zeroes it after `escorts` at
`0xb17a0`. Moving the declaration earlier does move the zero stores, but the
whole-function raw count gets worse:

```text
asteroidPos before coords        3533
asteroidPos after coords         3526
asteroidPos after enemyRoute     3526
asteroidPos after goods          3527
asteroidPos at base (after escorts) 3510
```

So the base scheduling is the best of the five even though the local region
looks closer to the original in the "before coords" variant.

### 4b. `ArraySetLength` sum-term order (the `sp#44`/`sp#48` spill swap)

Reordering the 12-term sum `pirateEscorts + stationPirates + blueprint +
extraAliens + ...` cannot move the `pirateEscorts`/`extraAliens` spill slots:

```text
swap pirateEscorts/extraAliens in the sum   3510 (neutral)
pirateEscorts last                          3510 (neutral)
stationPirates,blueprint,pirateEscorts,extraAliens first  3510 (neutral)
extraAliens first                           16706 (size 17292, fatal)
reverse order                               5757
```

The two locals are still swapped relative to the original at
`sp#44`/`sp#48`; slot assignment is not reachable from the sum order.

### 4c. Other store orders on the landed base

With the landed case-183 block, all remaining store permutations of cases
1/2/default and all `offset[0]`/`offset[1]` value permutations were measured;
every one is neutral-or-worse (best neutral 3,502, next 3,507). The `o2`
permutation that produced the win is unique.

## 5. Reproduce

```sh
cd /home/debian/GalaxyOnFire2/wt/w13214
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
cmake --build cmake-build-match --target check -j2
```

Expected: `raw=3502 aligned=996 size=17172/17172`, `.text` diff 74,215,
`.ARM.extab` diff 639, `.rel.plt` diff 0, `RATCHET PASS`.

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w13214/`
(`sweep.py`, `descent.py`, `bylines.py`, `annot2.py`, `dis.py`, `secdiff.py`,
`Level.base.cpp`).
