# `Level::createMission` (`_ZN5Level13createMissionEv`) — wave 187 / w13114

Target `src/game/world/Level.cpp`, function VMA `0xaffd8`, size **17,172 B**
(`0x4314`). Branch `wave187-w13114`, base master `8b5d4281c` (wave-186
harvest, raw **3,517**). Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, armeabi-v7a, image base `0x10000`).

Scope: continue the bool/order sweep on the wave-186 body (`bool` vs `int`,
multiply vs branch, declaration order, `Globals::*` hoists). Land only
size-exact, `.rel.plt`-identical raw wins.

## 1. Result

**No size-exact raw win.** `src/` is left at the wave-186 base. Every measured
candidate was neutral (3,517) or worse; the function is at a local optimum for
the requested axes. Gate re-run on the untouched tree:

```text
_ZN5Level13createMissionEv   raw=3517 aligned=1009 size=17172/17172 insn=6205/6197
check: byte_exact 4507/4524 linked_exact 4507/4524 size_exact 4524/4524
       data_exact 843/843 imports 207 lint CLEAN drift 0 parity 5370/5370
       RATCHET PASS
```

## 2. Sweeps (all real `link_so` + `tools/fn_diff.py`)

Base is raw **3,517**, aligned **1,009**, size **17,172/17,172**.

### 2a. `bool` / `int` / `unsigned` / `char` on every flag

Neutral (3,517, size exact): `resetStation` (`int`/`unsigned`/`char`),
`alienRace`, `hard`, `alienGuns` (`bigAlienWave == 1` -> `bigAlienWave`),
`stationPirates` (`unsigned` -> `int`), `blueprint` (`unsigned` -> `int`),
`extraAliens` (`int` -> `unsigned`), `pirateEscorts` (`int` -> `unsigned`).

Worse or size-fatal (representative):

```text
raid int/unsigned/char          3745
hasWanted int/unsigned/char    12519 (size 17148)
nameLeader int/unsigned/char   13635 (size 17180)
introSystem int/unsigned/char  14844 (size 17180)
bigAlienWave int/unsigned/char 12445 (size 17156)
alienWave int/unsigned/char    12414 (size 17148)
pirateFortress int/unsigned/char 12377 (size 17160)
siegedStation int/unsigned/char 16475 (size 17308)
```

`bool raid` -> `unsigned`/`char` is identical to `int` (3,745), so the
compiler canonicalises the type.

### 2b. Multiply vs branch / shift

All worse, none size-fatal:

```text
hostileCount = (int)(h + (opt44-0.5f)*h)  ->  h*(opt44+0.5f)      3521
hunterCount  = (int)(h + (opt44-0.5f)*h)  ->  h*(opt44+0.5f)      3532
raiders      = raiders + (opt44-0.5f)*raiders -> raiders *= ...   3521
case-1 attackerCount same rewrite                                 5230
alienPercent = (int)alienChance << hc  ->  * (hc + 1) / * (hc?2:1) 13945 (size 17188)
extraAliens  = (n+2) << hc             ->  * (hc + 1) / * (hc?2:1) 14215 (size 17196)
pirateEscorts ternary -> stationPirates * (...)                   12759
```

The `nextInt(2) == 0 ? 1 : -1` idiom occurs 15 times in the function.
Replacing all 15 with `nextInt(2) != 0 ? -1 : 1` or `nextInt(2) ? -1 : 1`
is neutral (3,517); the arithmetic form `1 - 2*nextInt(2)` is size-fatal
(17,132).

### 2c. Declaration order

Moving `int extraAliens = 0;` to five different anchors (before
`stationPirates`, before `pirates = 0`, before `stationIndex`, before
`introSystem`, before `raid`) is neutral (3,517). Moving the *computation*
block (`alienChance`/`alienPercent`/`extraAliens`) before `stationPirates`
is size-fatal (17,176); before `hasWanted` is 3,929.

Moving `int pirateEscorts = ...` after the `extraAliens` block (or after
`hasWanted`, `nameLeader`) is size-fatal (17,336). The declaration-order axis
is exhausted: scalar locals are SSA values, so their stack slots are chosen by
the register allocator, not by source declaration order.

### 2d. `Globals::*` hoists

```text
float opt44 hoist before pirateEscorts        3548
float opt44 hoist before the hostileCount math 8791
int hc hoist for alienPercent/extraAliens     4125
SolarSystem *system hoist at block start      15419 (size 17188)
SolarSystem *system hoist at function start   16511 (size 17268)
int campaignMission hoist at function start   16390 (size 17232)
Station *station hoist at block start         16036 (size 17216)
```

The compiler already caches `Globals::status`, `Globals::rnd`,
`Globals::globals` and `Globals::gameText` in registers/stack slots; explicit
hoists only disturb the schedule.

### 2e. Loop-variable signedness

`int` -> `unsigned` (with a cast on the bound) for the friend, jumper,
station, pirate-gun, raid, pirates, case-183 `i`/`k`, case-1 attacker,
case-9, case-10 station, case-15 raider and case-184 loops: all neutral
(3,517) or +1/+2 (3,518/3,519). `unsigned` -> `int` for the existing
`unsigned` loops: neutral or +1. The `case-2` convoy loop is sensitive:
`unsigned j` neutral, `convoyEnd+j < (int)size()` neutral, but `j < total -
convoyEnd`, `j < goods` and `j < (unsigned)total` are size-fatal
(17,144/17,152).

### 2f. Local statement reorders (non-adjacent)

```text
case-2 convoy: swap cargo=nullptr / crateCaptured()=0        3518
case-9 setPosition calls in ascending index order            3523 (aligned 980)
pirate loop: createStaticObject before pirateBase=...        3519
combinations of the above                                    3520..3526
```

The case-9 ascending order is semantically the original's *execution* order
(original calls `+1,+2,+3,+4`), but it is **not** a raw win: the function's
literal pools order constants by source first use, and the base source order
(`+3` before `+2`) matches the original pool. The ascending variant changes
the pool and loses 8 bytes in `0x3a80..0x3af0` (the only region that moves);
everything after is byte-identical. This is a useful constraint for a future
pass: execution order and pool order are decoupled.

## 3. Best remaining lead: the `sp#44` / `sp#48` spill swap

`pirateEscorts` and `extraAliens` are the only two locals whose spill slots are
swapped relative to the original. The swap is visible at 12 sites:

```text
source line   original   ours
1616 (store pirateEscorts)   sp#48   sp#44
1645 (store extraAliens)     sp#44   sp#48
1861 (ArraySetLength)        both    both (swapped)
2033/2035 (first/loop)       both    both (swapped)
2059 (escortsEnd)            sp#48   sp#44
2067/2068 (extraAliens blk)  both    both (swapped)
2077 (miningPlantIndex)      both    both (swapped)
2088 (blueprint slot)        both    both (swapped)
```

Every use is a 2-byte `[sp,#44]`/`[sp,#48]` encoding difference, so fixing the
swap is worth ~24 raw bytes. It is **not** reachable through any of the
declaration/type/expression-order changes above: the two values are SSA and
the slots are assigned by the register allocator's spill ordering. Moving the
`extraAliens` *definition* (the `if` that creates its phi) before
`pirateEscorts` requires moving the whole `alienChance`/`alienPercent` block,
which is size-fatal (17,176). A future pass should try to influence the spill
order directly (e.g. by restructuring the live ranges at line 1861), not by
declaration order.

## 4. Other measured residuals (unchanged)

The largest raw runs are shifts, not local opcode differences:

```text
124 B @0xb3b74   stationPirates loop tail (12-byte shift from the escorts loop)
105 B @0xb1686   case-1 second loop (`mov r1,fp` extra move)
 90 B @0xb3a3c   escorts loop setPosition
 80 B @0xb397c   escorts loop prologue (extra `str r5,[sp,#104]`)
 72 B @0xb19a2   case-2 convoy loop `new`
```

The tail jump table has 19 branches in ours vs 17 in the original (two extra
EH landing pads to the `delete r5` cleanup at `0xb42a4`); removing them would
need a compensating 4-byte growth elsewhere.

## 5. Reproduce

```sh
cd /home/debian/GalaxyOnFire2/wt/w13114
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
cmake --build cmake-build-match --target check -j2
```

Expected: `raw=3517 aligned=1009 size=17172/17172`, `.text` diff 74,274,
`.ARM.extab` diff 647, `.rel.plt` diff 0, `RATCHET PASS`.

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w13114/`
(`measure.sh`, `annotate.py`, `rawruns.py`, `sweep*.py`, `Level.base.cpp`,
`unstripped.so`).
