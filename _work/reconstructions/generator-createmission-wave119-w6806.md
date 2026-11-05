# `Generator::createMission` — wave 119 (w6806): `amount` is an alloca in ours, a spill slot in the original

Target: `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE`
(original `0x971b8`, 1996 B, `.ARM.extab` LSDA record **172 B**, `cslen = 151`,
26 call-site rows).

Status: **research note, no source change landed.**  Master keeps the tuned
**83.6 % / 1996 B / 168 B record** shape (`.ARM.extab` `0x26294`, `cslen 147`,
25 rows).  This pass found the *root cause* of the whole 4-byte / 12-byte
`.ARM.extab` + body-size gap (an extra IR-level `alloca` that the previous
note did not identify) and measured ~900 more variants around it.  Nothing
beats 83.6 %; the faithful shape is a **frame-96 / 1980 B / 79.1 %** variant.

```
before (HEAD):  83.6 %  1996 B  .ARM.extab 0x26294  createMission record 168 B (cslen 147, 25 rows)
after:          83.6 %  1996 B  .ARM.extab 0x26294  (unchanged)
```

Gate at the start and end of this pass: `RATCHET PASS`, `byte 4505`,
`linked 4505`, `data 843/843`, imports 207, `lint CLEAN`, `drift 0`,
27 sections, file size 2,187,328 B.

## 1. Root cause: `*(volatile int *) &amount = 0` is an `alloca`

`-mllvm -print-machineinstrs=stack-coloring` on the current TU shows the
frame objects at the stack-coloring pass:

```
Frame Objects:
  fi#0: size=4,  align=4  at location [SP]      <- stack canary
  fi#1: size=8,  align=8  at location [SP]      <- volatile double padA
  fi#2: size=4,  align=4  at location [SP]      <- amount   (!!)
  fi#3..fi#6: size=12, align=4                  <- four AbyssEngine::String
```

and the only `%stack.N` uses are `%stack.0` (canary), `%stack.1` (padA),
`%stack.2` (amount, `store 4 into %ir.7`) and `%stack.3..6` (Strings).

`amount` is a **frame object** only because the source takes its address:
`*(volatile int *) &amount = 0;`.  Frame objects get a fixed slot from PEI and
are *not* coloured by `StackSlotColoring`; ordinary variables are spill slots
and *are* coloured.  That single fact explains the whole divergence:

```
original (frame 96)                 ours (frame 112, `volatile double padA`)
sp+16  galaxy / bonus               sp+20  &__stack_chk_guard
sp+20  &__stack_chk_guard           sp+24  agent / galaxy / bonus
sp+24  agent / difficulty           sp+28  targetStation / difficulty
sp+28  targetStation / amount       sp+32  status
sp+32  status                       sp+36  race / type
sp+36  race / type                  sp+40  rnd
sp+40  rnd                          sp+44..92  four AbyssEngine::String
sp+44..92  four AbyssEngine::String sp+92  amount      <- fi#2, own slot
sp+92  canary                       sp+96  padA        <- fi#1
                                    sp+108 canary
```

In the original, `amount` is an ordinary spill slot that stack-slot colouring
merges with `targetStation`'s (dead) `sp+28` slot.  In ours it is pinned at
`sp+92`, which forces the canary to `sp+108`, which is why the `padA` hack
(12 B of `vmov.i32/vstr/vldr`) exists at all: it was added to recover the
missing 12 body bytes, and it *also* pushes the frame from 96 to 112.

So the original source almost certainly has a plain `amount = 0;`, not the
`*(volatile int *)` spelling.

## 2. The faithful shape: frame 96, 1980 B, 79.1 %

Replacing `*(volatile int *) &amount = 0;` with a plain `amount = 0;` and
deleting `padA` gives **exactly the original's frame (96)** and the original's
slot *counts*, with `amount` coloured instead of pinned:

```
sp refs   orig   plain-amount/no-padA
sp+16      5          5
sp+20      2          2
sp+24      8          9
sp+28     10          6
sp+32     13         13
sp+36     14         14
sp+40     12         12
sp+92      2          2
```

Only `sp+24` / `sp+28` differ (9/6 vs 8/10): the colouring picks
`agent|galaxy|bonus` @24 + `targetStation|difficulty` @28 instead of
`agent|difficulty` @24 + `targetStation|amount` @28.  Body is 1980 B (16 B
short) and the fuzzy score is **79.1 %** — the register allocator keeps
`amount` in `r2` through the switch instead of spilling it, and keeps
`galaxy` in `r6` while spilling `fromSystemPtr`, so most of the saved
instructions are not the original's.

## 3. The one variant that hits 1996 B / 727 insns / frame 96 exactly

```
    Galaxy *galaxy = Globals::galaxy;
    float reward = (galaxy->distance(systems->data()[Globals::status->getStation()->getSystem()],
                                     systems->data()[galaxy->getStation(targetStation)->getSystem()])
                    / 1200.0f + 1.0f) * (float) ((int) ((float) difficulty / 10.0f * 5500.0f) + 1500);
```
plus plain `amount = 0;` and no `padA`:

| variant | pct | usize | insns | frame | cslen/rows |
|---|---:|---:|---:|---:|---:|
| `inline_fsp2` | 78.1 | **1996** | **727/727** | **96** | 141/24 |

Body size, instruction count and frame all match the original exactly, but the
LSDA shrinks to 141/24 and the fuzzy score is only 78.1 % (the `amount`-in-`r2`
and `galaxy`-in-`r6` allocation differences again dominate).

## 4. Measurements (all body 1996 B unless noted)

`V` = `volatile int amount` + plain store; `C` = current `*(volatile int*)&amount`;
`P` = plain `amount = 0;`; `N` = no `padA`.

| variant | pct | usize | frame | cslen/rows |
|---|---:|---:|---:|---:|
| **HEAD** (`C`, `padA`) | **83.6** | 1996 | 112 | 147/25 |
| `V`, `padA` | 83.5 | 1996 | 112 | 147/25 |
| `V`, `N` | 71.9 | 1984 | 96 | 147/25 |
| `C`, `N` | 71.9 | 1984 | 96 | 147/25 |
| `P`, `N` | 79.1 | 1980 | 96 | 147/25 |
| `P`, `N`, `Globals::galaxy` for both calls | 78.8 | 1980 | 96 | 147/25 |
| `P`, `N`, `Globals::galaxy` for `getStation` only (local `galaxy`) | 62.8 | 2016 | 104 | 146/25 |
| `P`, `N`, `inline_fsp2` (see §3) | 78.1 | 1996 | 96 | 141/24 |
| `P`, `N`, `Globals::galaxy` for both, inline `fromSystem` | 79.6 | 1988 | 96 | 147/25 |
| `P`, `padA` | 70.2 | 1992 | 112 | 147/25 |
| `V`, `N`, `Globals::galaxy` for `getStation` only | 65.7 | 2000 | 104 | 146/25 |
| `P`, `N`, `volatile int amount` + `inline_fsp2` | 72.4 | 1996 | 96 | 147/25 |
| `padA` = `volatile int` | 79.4 | 1992 | 104 | 147/25 |
| `padA` = `volatile long long` | 79.5 | 1996 | 112 | 147/25 |
| `padA` = `volatile float` | 79.8 | 1992 | 104 | 147/25 |
| `padA` = `volatile double[1]` | 83.6 | 1996 | 112 | 147/25 |
| `padA` = `volatile int[2]` (two stores) | 70.8 | 1996 | 104 | 147/25 |
| `padA` store twice | 79.6 | 2004 | 112 | 147/25 |
| `int amount = 0;` at top, no cast, `N` | 79.1 | 1980 | 96 | 147/25 |
| `amount` declared after `targetStation` / after `race` / first | 75.5 | 1996 | 104 | 147/25 |
| `int difficulty;` / `int bonus;` / `float reward;` hoisted to top | 83.6 | 1996 | 112 | 147/25 |
| `int itemId = 0;` hoisted to top | 82.4 | 1996 | 112 | 147/25 |

Declaration order of `(level, padA, amount)` is load-bearing: `(level, padA,
amount)` and `(padA, level, amount)` both give 83.6 %, every other order 75.5 %.

### 4.1 Branch-hint sweep (single hints on top of HEAD)

40 single-hint variants (all `if`/`while` conditions in the function, both
polarities).  Best results:

| hint | pct | usize | frame | cslen/rows |
|---|---:|---:|---:|---:|
| `expect(type==8,1)` on the item block | 67.4 | 2004 | 120 | **151/26** |
| `expect(campaign<=15,0/1)` | 78.3 | 1996 | 112 | 157/27 |
| `expect(pick==8,1)` | 53.9 | 2008 | 112 | 147/25 |
| `expect(used->data()[pick],1)` | 81.0 | 1996 | 112 | 147/25 |
| `expect(sum==14,1)` | 83.3 | 1996 | 112 | 147/25 |
| `expect(type==12,*)`, `expect(race<4,*)`, `expect(type==13,*)`, `expect(difficulty>=10,*)`, `expect((type\|4)==12,*)`, `expect(type==7,*)`, `expect(type==6,*)`, `expect((costs+costRem)%50==0,*)` | 83.6 | 1996 | 112 | 147/25 |

`type==8,1` is the **only** single hint that restores `cslen 151 / 26 rows`,
and it costs 16 points (67.4 %).  Adding any second hint on top of it moves
the record to 159/27.  No single hint exceeds 83.6 %.

### 4.2 Structural + hint combinations

several hundred random combinations of 1–4 edits from a 37-edit library (all `if`/`while`
hints, `padA`/`amount`/`galaxy`/`fromSystem` shapes) produced **no variant
above 83.6 %**.  The best 1996 B / frame-96 variants stay in the 72–80 % band
because the `amount`-in-register and `galaxy`-in-`r6` allocations are not
reachable together with the original's `amount`-in-memory / `galaxy`-in-`sp+16`
allocation.

## 5. Why the original allocates `galaxy` to `sp+16`

The original really does reload `Globals::galaxy` for the `getStation` call:

```
9760a: ldr  r0,[pc,#876]   ; &Globals::galaxy (GOT)
97610: ldr  r5,[r0,#0]     ; r5 = &Globals::galaxy
97612: ldr  r0,[r5,#0]     ; galaxy
97614: str  r0,[sp,#16]    ; spill receiver
...
97630: ldr  r0,[r5,#0]     ; fresh load for getStation
```

That is the signature of `Galaxy *galaxy = Globals::galaxy;` **plus**
`Globals::galaxy->getStation(...)` in the argument (the intervening
`Status::getStation()->getSystem()` call prevents CSE of the second load).
Reproducing it (`P`/`N` + local `galaxy` + `Globals::galaxy` for `getStation`)
does give the original's galaxy spill, but costs an extra stack slot
(frame 104) and drops the score to 62.8 %.

When the source uses `Globals::galaxy` for *both* calls, clang keeps the
loaded pointer in callee-saved `r6` across the call and spills
`fromSystemPtr` instead — the mirror image of the original.

## 6. Conclusion

The original's shape is `amount = 0;` (plain, not address-taken) with no
`padA`, which reproduces the original's 96-byte frame exactly.  But at `-Oz`
that shape compiles to 1980 B / 720 insns / 79.1 %, because the register
allocator keeps `amount` in `r2` and `galaxy` in `r6` where the original
spills both.  The tuned HEAD (`volatile double padA` + `*(volatile int*)&amount`)
is a different local optimum at 83.6 % / 1996 B / 147-25.

No variant found in this pass is simultaneously `>= 83.6 %`, 1996 B and
`cslen 151` (26 rows).  `.ARM.extab` therefore stays 4 B short (`0x26294` vs
`0x26298`) from this function.

## 7. Reproduction

```sh
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4505
```

Scratch (not part of the repo): `/dev/shm/w6806/` — `one.py` (compile+link+
score one TU), `score.py`, `measure.py` (LSDA rows), `sbs.py` (normalized
side-by-side diff), `h.py` (variant harness with frame size), `grid.py`
(amount × padA × galaxy × fromSystem grid), `hints2.py` (40 single hints),
`search2.py` / `search3.py` (random edit combinations).

Frame objects dump:
```sh
clang++ <exact NDK r18b flags> -mllvm -print-machineinstrs=stack-coloring \
  -o /dev/shm/w6806/sc.o -c src/game/mission/Generator.cpp
# createMission section starts at "# Machine code for function
# _ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE"
```
