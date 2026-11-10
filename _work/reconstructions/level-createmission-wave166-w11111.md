# w11111 (wave 166): `Level::createMission` — frame map after w11010,
# phi/block/decl-order sweep (no raw win; 5,436 / 767 / 17,172 B unchanged)

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w11111`, base `85c1f88df`
(wave 165).  Continues `level-createmission-wave164-w11010.md` and
`level-createmission-wave163-w10913.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, `0x4314` = 17,172 B).

## Baseline (re-measured, pristine HEAD)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   raw 5436   align 767   insn 6204/6197
gate: byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, parity 5370/5370, RATCHET PASS
```

The w11010 `expect(comingFromAlienWorld,0)` landing is present and holds:
`guard` = `%stack.82` = sp+88 (2 refs), `raid` = `%stack.83` = sp+84
(17 refs), exactly the original's pair.

## 1. Frame map (current build, `-mllvm -print-machineinstrs`)

Frame 496 B.  `sp+N` is relative to the post-`sub sp,#400` SP.  The 28
4-byte spill colours are fi#73..fi#100 = sp+124 down to sp+16.  Colour ->
variable comes from the post-SSC `DBG_VALUE %stack.N`; note that SSC
**reuses** a colour for several non-overlapping variables, so the mapping
below is the *first* variable that owns each colour, not an exclusive one.

```text
sp+124 fi#73  hunterCount        sp+68  fi#87  raidRace
sp+120 fi#74  wingRoute          sp+64  fi#88  bigAlienWave
sp+116 fi#75  stationRace        sp+60  fi#89  alienWave
sp+112 fi#76  localRace          sp+56  fi#90  pirateFortress
sp+108 fi#77  attackerCount      sp+52  fi#91  (temp, shared)
sp+104 fi#78  stationIndex       sp+48  fi#92  (temp, shared)
sp+100 fi#79  total              sp+44  fi#93  stationPirates
sp+ 96 fi#80  (temp, shared)     sp+40  fi#94  extraAliens
sp+ 92 fi#81  end                sp+36  fi#95  blueprint
sp+ 88 fi#82  __stack_chk_guard  sp+32  fi#96  pirates
sp+ 84 fi#83  raid               sp+28  fi#97  nameLeader
sp+ 80 fi#84  (temp, shared)     sp+24  fi#98  convoy
sp+ 76 fi#85  jumperCount        sp+20  fi#99  pirateEscorts
sp+ 72 fi#86  bigShipsCount      sp+16  fi#100 (temp, shared)
```

Pre-SSC slot -> variable (from the Virtual-Register-Rewriter dump):
`%stack.77 pirates, 78 blueprint, 79 stationPirates, 80 nameLeader,
81 extraAliens, 82 localRace, 83 raidRace, 84 convoy, 85 raid,
90 pirateEscorts, 91 bigShipsCount, 93 jumperCount, 97 pirateFortress,
98 bigAlienWave, 101 alienWave, 102 wingRoute, 103 guns, 107 stationRace,
109 enemyRace, 110 stationIndex, 112/115/116 total, 120 stations,
121 attackerRace, 124 clientRace, 126 stationType, 127 escortsEnd,
128 escorts, 129 end, 130 attackerCount, 132 first, 133 offset,
139 hunterCount, 142 station, 143 ship`.

Post-SSC colours: `73 hunterCount, 74 wingRoute, 75 stationRace,
76 localRace, 77 attackerCount, 78 stationIndex, 79 total, 81 end,
83 raid, 85 jumperCount, 86 bigShipsCount, 87 raidRace, 88 bigAlienWave,
89 alienWave, 90 pirateFortress, 93 stationPirates, 94 extraAliens,
95 blueprint, 96 pirates, 97 nameLeader, 98 convoy, 99 pirateEscorts`.

### Original vs ours, per-offset `[sp,#N]` reference counts

```text
off   orig ours        off   orig ours
  0    30   31          92    13   15   <-- end
  4    21   22          96    15   15
  8    20   20         100    28   28
 16     2    2         104    18   20   <-- stationIndex
 20     4    4         108    36   35   <-- attackerCount
 24     4    4         112    30   30
 28     3    3         116    37   34   <-- stationRace
 32     8    8         120    28   27   <-- wingRoute
 36     3    3         124    57   57
 40     4    6   <--   188     2    2
 44     6    6         196     2    2
 48     6    5   <--   200     3    4
 52     6    5   <--   204     3    3
 56     7    7         208     3    4
 60     7    7         212     3    2
 64     7    7         216     3    2
 68     7    7         220     3    2
 72     5    5         224     4    4
 76     4    4         228     3    3
 80     4    4         232     4    4
 84    17   17         260     1    1
 88     2    2         264     1    1
```

The `guard`/`raid` pair (88/84) is now exact.  The remaining count deltas
are small (1-3 refs) and are **not** a variable permutation: the same
colour is reused for several non-overlapping variables, and the residual
codegen (block order / scheduling) changes where a reload is placed.

## 2. Sweep: phi node order, block order, local declaration order

Every variant below was built with `link_so -j2` and measured with
`tools/fn_diff.py`.  `raw` is the same-address byte diff (ground truth).

| variant | size | raw | aligned | note |
|---|---:|---:|---:|---|
| HEAD (baseline) | 17172 | **5436** | **767** | best |
| convoy `switch(k)`: swap source case 1/2 | 17172 | 5436 | 767 | clang canonicalises |
| convoy `switch(k)` -> if-chain, 6 orders | 17124 | 11596 | 845 | loses the shared `str[380]` join, -48 B |
| `raid` if/else polarity swap | 17172 | 5436 | 767 | neutral |
| `stationIndex == 78` if/else polarity swap | 17172 | 5436 | 767 | neutral |
| `raidRace` ternary polarity swap | 17172 | 5436 | 767 | neutral |
| `raiders` ternary polarity swap | 17172 | 5436 | 767 | neutral |
| `stationRace` ternary polarity swap | 17172 | 5436 | 767 | neutral |
| `friendCount` ternary polarity swap | 17172 | 5436 | 767 | neutral |
| `extraAliens` moved to top / after nameLeader / braced | 17172 | 5436 | 767 | neutral |
| `extraAliens` `int` -> `unsigned` | 17172 | 5439 | 768 | +3 |
| `stationIndex` `int` -> `unsigned` | 17172 | 5436 | 767 | neutral |
| `localRace` `int` -> `unsigned` | 17172 | 5436 | 767 | neutral |
| `int slot = ...` sum: `p+e+sp` / `sp+e+p` | 17172 | 5436 | 767 | neutral |
| `int slot = ...` sum: `p+sp+e` / `sp+p+e` | 17356 | 16811 | 2456 | size +184 |
| `ArraySetLength` sum reorder (3 forms) | 17172 | 5436 | 767 | neutral |
| `ArraySetLength` sum: `e+p+sp+b` | 17288 | 16708 | 2768 | size +116 |
| `stationIndex` declared after `raidRoll` | 17172 | 5454 | 777 | +18 |
| `stationIndex` declared after `pirates` | 17172 | 5466 | 777 | +30 |
| `stationIndex` declared after `security` | 17180 | 15009 | 1015 | size +8 |
| `hasWanted = wanted != nullptr` (fold) | 17148 | 14230 | 1000 | size -24 |

The `stationIndex`-after-`raidRoll` variant is the interesting one: it
reproduces the pre-w11010 raw (5,454) **with the same sp+84/sp+88 pair**
(17/2 both), i.e. the 18-byte delta is not in the guard/raid slots at
all — it is an instruction-scheduling difference elsewhere.  The current
`stationIndex`-before-`raidRoll` declaration order is therefore
load-bearing for 18 raw bytes, but not through the frame map.

## 3. Conclusion

* The frame map is already correct after w11010: `guard` at sp+88,
  `raid` at sp+84, and every other colour is stable under all phi /
  block / declaration-order permutations tried.
* No further spill-slot permutation is reachable from source: the
  remaining per-offset count deltas come from colour *reuse* plus the
  residual block-order/scheduling differences, not from swapped slots.
* The residual raw 5,436 is dominated by three block-order regions
  (offsets ~0x1400-0x1a00 convoy switch, ~0x2e00-0x3500 merged station
  predicate, ~0x3700-0x4300 blueprint/alien sums).  The convoy
  `switch(k)` body order `0,1,2,default` is still not reachable: clang
  emits `0,2,1` (case0 fallthrough) or `2,0,1` (case2 fallthrough) only;
  an if-chain loses the shared join store (-48 B).
* Nothing landed: every size-exact variant is raw-neutral or worse, and
  the tree is left clean at the baseline.

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
cmake --build cmake-build-match --target check -j2
```

Scratch: `/dev/shm/w11111/{orig.dis,ours.dis,sprefs.py,spdiff.py,spdiff2.py,
spalign.py,rawregions.py,dumpcmp.py,variant*.py,measure.sh,genpmi.sh,genll.sh,
pmi.txt,Level.ll,Level.obj,Level.cpp.base}`.
