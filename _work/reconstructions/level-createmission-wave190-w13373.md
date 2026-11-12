# `Level::createMission` (`_ZN5Level13createMissionEv`) — wave 190 / w13373

Target `src/game/world/Level.cpp`, function VMA `0xaffd8`, size **17,172 B**
(`0x4314`). Branch `wave190-w13373`, base master `0f54626b3` (wave-190 base,
raw **3,427**). Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, armeabi-v7a, image base `0x10000`).

Scope: land the exact **56 B `case 183` convoy switch** (wave-189 w13350 §8
had it at 17,164 B) by finding the missing +8 B compensation, and adopt the
faithful `case 4` condition-first form.

## 1. Result — landed

```text
                                     size            raw    aligned  insn
base 0f54626b3                       17172/17172     3427   1003     6206/6197
landed                               17172/17172     3244   1156     6210/6197
```

Raw **3,427 -> 3,244** (-183), size exact 17,172 B. `.text` differing bytes
73,516 -> 73,333 (-183); `.ARM.extab` differing bytes 610 -> 603 (-7);
build-id 20 (unchanged). Total (text+extab+build-id) **74,146 -> 73,956**
(-190). `.rel.plt` byte-identical, `.ARM.exidx` diff 0, 27/27 headers,
file size 2,187,328 exact.

Gate: `byte_exact 4507/4524`, `linked_exact 4507/4524`,
`size_exact 4524/4524`, `stub_zero_size 0`, `missing 0`, `wrong_type 0`,
`extra 0`, `data_exact 843/843`, imports **207**, lint **CLEAN**, drift **0**,
`RATCHET PASS`. No baseline update.

## 2. The changes (all faithful except the spill lever)

### 2a. `case 183` convoy switch — exact 56 B shape + correct values

The original switch (`0xb1418..0xb1450`) is 56 B with every arm storing
`offset[1]`, `offset[0]`, `offset[2]` (o2 last) and one shared
`str r1,[sp,#380]`. The base carried a permutation of the values and a
different per-arm store order (60 B). Replacing it with the original values
and o1,o0,o2 order reproduces the 56 B dispatch. Two `volatile` stores are
needed to keep clang from sinking the common o2 store and turning the switch
into a stack-table lookup (w13350 §8a):

```cpp
case 0:  offset[1] = -3000.0f; offset[0] = 20000.0f; offset[2] = -50000.0f; break;
case 1:  offset[1] = -7000.0f; offset[0] = 10000.0f; offset[2] = -30000.0f; break;
case 2:  *(volatile float *) (offset + 1) = 0.0f;
         offset[0] = 25000.0f; offset[2] = -36000.0f; break;
default: offset[1] = 6000.0f; offset[0] = 12000.0f;
         *(volatile float *) (offset + 2) = -45000.0f; break;
```

This also fixes the convoy offsets to the original values (the base's
permutation was semantically wrong).

### 2b. `case 183` tail — original order

Original: `player->setPosition(-30000,5000,-100000)` ->
`stationPosition = player->getPosition()` -> `timeLimit = 181000` ->
`objectivesA = new Objective(3,181000,this)`. The base had
`stationPosition` first and `timeLimit` last, i.e. it captured the *old*
player position (a real semantic bug). Reordering to the original is worth
raw 3,308 -> 3,255 on top of 2a.

### 2c. `case 2` objectives — original order

Original allocates `objectivesA` (`Objective(18,0,convoyEnd,this)`) first and
`objectivesB` (`Objective(18,convoyEnd,total,this)`) second
(`0xb1992..0xb19bc`). The base had B then A. Swapping to A then B is worth
raw 3,255 -> 3,244.

### 2d. `case 4` — condition-first form

```cpp
int roll = Globals::rnd->nextInt(2);
if (roll != 0) {
    this->playerRoute = this->createRoute(Globals::rnd->nextInt(2) + 2);
    goto case4Tail;
}
goto case4Else;
```

This is semantically identical to the base (no extra RNG call) and matches
the original's condition-first codegen (`nextInt` -> `cmp` -> branch ->
`nextInt`). Size/raw neutral on its own; kept because it is the original
form and the task's requested test.

### 2e. The +8 B size lever: `volatile int offset`

The exact 56 B switch makes the function 17,164 B (-8). The entire -8 is a
literal-pool alignment cascade: with the switch fixed, the `type 10` body
starts at `0xb1db4` (4 mod 8) instead of the original `0xb1db8` (0 mod 8),
so its literal pool omits the 4 B `0xbf00bf00` pad entry and the body is
4 B short; the shift itself is another 4 B. Adding **+4 B anywhere in the
`type 1`/`type 2` bodies** moves `type 10` to 0 mod 8, the pad entry
reappears (+4) and the function returns to 17,172.

`volatile int offset` in the `case 2` first convoy loop is that lever: it
forces `offset` to be spilled/reloaded instead of kept in `r9`, which is
exactly the register-allocation shape the original has (the original spills
`offset` to `[sp,#120]` and keeps `convoyEnd` in `fp`). It is a codegen
lever, not a semantic change (same values, same RNG consumption).

Measured alternatives that also hit 17,172 (all worse raw):
`volatile int escorts` 3,261 (aligned 820), `volatile float difficulty`
5,058, `volatile float opt44` 5,069, `volatile int convoyEnd` 14,097.
Non-volatile spellings of `offset` (`unsigned`, `long`, `= i*2000`,
declaration moves, `(void)offset`) are all 17,164.

## 3. TBH target deltas after the landing

```text
type   orig      ours      delta
 1     b14b2     b14b2       0
 2     b16d4     b16d4       0
 4     b19c0     b19c0       0
 6     b1a00     b1a00       0
 7     b1b20     b1b20       0
 9     b1c9c     b1c9c       0
10     b1db8     b1db8       0
12     b2128     b2128       0
15     b2368     b2368       0
```

All nine case bodies now start at the original addresses. The residual raw
3,244 is dominated by `0x3200-0x3e00` callee-saved register rotation
(~1,890 B) and the `0x1400-0x1a00` per-case body instruction schedules
(~1,004 B), not by any address shift.

## 4. Reproduce

```sh
cd /home/debian/GalaxyOnFire2/wt/w13373
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
cmake --build cmake-build-match --target check -j2
```

Expected: `raw=3244 aligned=1156 size=17172/17172`, `.text` diff 73,333,
`.ARM.extab` diff 603, `.rel.plt` diff 0, `RATCHET PASS`.

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w13373/`
(`measure.sh`, `sweep.py`, `combo.py`, `tbh.py`, `alignsum.py`,
`blockdiff.py`, `secdiff.py`, `Level.base.cpp`, `Level.clean.cpp`,
`Level.s56base4.cpp`, `Level.best.cpp`).
