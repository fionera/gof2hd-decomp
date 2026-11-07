# w7776 (wave-135): `Level::createMission` — tail-loop / alien-block source-order sweep (no source win)

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w7776`, based on master `bbffef482`.
Continues `level-createmission-wave134-w7756.md`, `…-wave133-w7733.md`,
`…-wave132-w7710.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; `_ZN5Level13createMissionEv` @ `0xaffd8`, size `0x4314` = 17172 B).
Manual Thumb disassembly (objdump) plus the Ghidra decompile from the wave-134 note.
Build/flags unchanged (NDK r18b clang 7.0.2, `-Oz`).

## Baseline (pristine HEAD, re-measured this session)

```text
_ZN5Level13createMissionEv   92.1 % (report row)   size 17172/17172 B
.text diff_bytes 85022 (whole section)
raw fn differing bytes 5761 (same-offset byte diff inside the 17172-byte symbol)
gate: byte 4506/4524, linked 4506/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, RATCHET PASS
```

`raw fn differing bytes` (positional byte diff of the symbol body) is a finer
metric than the `difflib` `fn bytediff` used by wave 134: it is exactly the part
of the section diff that lives inside this function, so a source change that
does not move the function boundary moves `.text` 1:1 with it.  Use it for A/B.

## New hypothesis tested: rebuild the tail loop and the alien blocks statement-by-statement

### 1. Tail loop (`case4Tail`, `b416e..b41d0`)

The original loop body is:

```text
b416e: ldr.w r0, [sl]              ; Globals::globals
b4172: movs  r1, #8
b4174: blx   getRandomEnemyFighter ; type (r5)
b417a: ldr   r0, [sp,#120]         ; &Globals::rnd (GOT slot)
b417c: ldr.w r4, [r8,#264]         ; this->playerRoute
b4180: ldr   r6, [r0]              ; rnd
b4182..b4194: Route::length / AERandom::nextInt / Route::getWaypoint
b4198: strd  r0, fp, [sp]          ; wp, true
b419c: str   r0, [sp,#8]           ; false
b41a0: mov   r0, r8                ; this
b41a2: movs  r1, #8
b41a4: movs  r2, #0
b41a6: mov   r3, r5                ; type
b41a8: blx   createShip
b41ac: ldr.w r1, [r8,#248]         ; this->enemies
b41b0: ldr   r1, [r1,#4]
b41b2: str.w r0, [r1,r9,lsl #2]    ; enemies->data[i] = ship
b41b6: ldr.w r0, [r8,#248]         ; reload (*enemies)[i]
b41ba: ldr   r0, [r0,#4]
b41bc: ldr.w r0, [r0,r9,lsl #2]
b41c0: blx   setToSleep
b41c4: add.w r9, r9, #1
b41c8: ldr.w r0, [r8,#248]         ; re-read this->enemies->size() every iteration
b41cc: ldr   r0, [r0,#0]
b41ce: cmp   r9, r0
b41d0: bcc.n b416e
```

Register map: **original** `this=r8`, `i=r9`, `Globals::globals=sl`,
`Globals::rnd=[sp,#120]`, `true=fp`.  **Ours** `this=r9`, `i=r8`,
`Globals::globals=r4`, `Globals::rnd=sl`, `true=fp`.  It is a clean permutation
of the same values; the instruction sequence matches one-for-one after the
register rename.  `[sp,#120]` in the original is the GOT slot for
`Globals::globals` (`0x210218`), so the original reuses the same spill slot for
`&Globals::rnd` in the tail.

The `int idx` temp landed in wave 134 is the best formulation of this loop:
evaluating the waypoint index first lets clang hoist `true` into `fp` and emit
`strd r0,fp,[sp]`.  Reverting to the one-liner makes `raw fn` 5761 -> 5763.

### 2. Alien-turret loop (`if (alienGuns != 0)`, `b329a..b3394`)

Original statement order is exactly:

```text
createStaticObject(6772)
enemies[i] = ...
gun = enemies[i]
gun->player->setMaxHitpoints(1000)
t = bigAlienWave ? kAlienStationTurrets : kAlienOutpostTurrets   ; cbz branch
v = rotate({t[3],t[4],t[5]}); gun->geometry->setRotation(v)
v = rotate({t[0],t[1],t[2]}); gun->setPosition(base+v)
gun->setScaling(6.0f)          ; after setPosition
gun->cargo = nullptr           ; before shipGroup
gun->shipGroup = localRace
```

The checked-in source instead has `setScaling` between the first rotate and
`setRotation`, `shipGroup` before `cargo`, and the `(bigAlienWave + (i < 0))`
anchor.  Rebuilding the source in the original order makes the function
*more* different, not less:

| alien-turret variant | raw fn bytes |
|---|---:|
| **checked-in (setScaling mid, shipGroup/cargo, `(i<0)` anchor)** | **5761** |
| setScaling after setRotation | 5762 |
| setScaling after setPosition, fresh `(*enemies)[i]` receiver | 5865 |
| fully faithful order, `gun` receiver (setScaling last, cargo/shipGroup) | 5911 |
| setScaling after setPosition, `gun` receiver | 5911 |
| `((PlayerTurret *) gun)->setScaling` in the checked-in position | 6437 (size 17164) |
| no `gun` local (all `(*enemies)[i]`) | 6618 |
| `if (bigAlienWave)` statement instead of the `(i<0)` ternary | 6632 |

The reason is register allocation: the original keeps `this` in **r4** through
the whole loop (`gun`=r6, `k*24`=r9, `i`=fp), while ours reloads `this` into
**r5** at `b3280` (`ldr r5,[sp,#124]`) and uses `gun`=r4, `i`=r9, `k*24`=fp.
The extra reload is 4 bytes and is the first divergence of the loop; the
checked-in mid-loop `setScaling` + fresh load is what keeps `this` in r4 before
the reload and is the cheapest shape.

The fortress-turret loop (`if (pirateFortress)`) has the same `shipGroup`/`cargo`
order mismatch; swapping it alone is exactly neutral (raw fn 5761).

### 3. `case 183` offset if-chain / literal pool

The original's `offset[0..2]` switch body layout is **0,1,2,default** and the
literal pool follows it (`-3000,20000,-50000 | -7000,10000,-30000 |
25000,0,-36000 | 6000,12000,-45000`).  Ours lays the bodies out
**0,2,1,default** and the pool reorders to `-3000,20000,-50000 |
25000,0,-36000 | -7000,10000,-30000 | ...`, which is what shifts every
`ldr [pc,#…]` in the 0x13b0–0x1600 block by 4 bytes (the 1417-byte regional
residual).  The values themselves are correct (verified against the pool).

All 6 if-chain permutations and all 6 `switch` case orders were re-measured
with the positional metric: `0123` and `0213` both give 5761; `2013`/`2103`
give 5764; `1023`/`1203` give 5769.  Clang 7.0.2 canonicalises `0213` back to
`0123`, so the body layout 0,2,1 cannot be steered from the source order.

### 4. `raid`/`fp` stack-slot swap

The very first byte difference in the function is `b0024`:
`str.w fp,[sp,#88]` (original) vs `str.w fp,[sp,#84]` (ours).  `[sp,#88]` is
the saved caller `fp`; `[sp,#84]` is `bool raid` in the original, and the two
are swapped in ours (17 uses each).  This is a register-allocator/PEI slot
assignment order, not a source-order decision.  Moving the `bool raid;`
declaration, initialising it, making it a ternary, or changing its type
(`int` 5988, `unsigned char` 5988) does not flip the swap; `bool` and the
current position are the cheapest (5761).

## Other variants measured this session (fresh `link_so` each)

| variant | raw fn bytes | size |
|---|---:|---:|
| **baseline** | **5761** | **17172** |
| `int i` instead of `unsigned i` in tail loop | 5761 | 17172 |
| local `KIPlayer *ship` temp in tail loop | 5761 | 17172 |
| `bool bigAlien = … && alienWave; if (bigAlien)` | 5761 | 17172 |
| `bool raid` moved before `int security` | 5761 | 17172 |
| `raid` as a ternary initialiser | 5761 | 17172 |
| `int type` after `int idx` in tail loop | 5761 | 17172 |
| `int type` before `int idx` in tail loop | 5880 | 17172 |
| one-liner tail loop (no `idx` temp) | 5763 | 17172 |
| alien cargo/shipGroup swap only | 5763 | 17172 |
| `bool raid` -> `int` / `unsigned char` | 5988 | 17172 |
| faithful tail `this->enemies->size()` re-read + `(void)n` | 6494 | 17172 |
| `(i == X) & alienWave` branchless `&&` (both conditions) | 14726 | 17196 |
| `bool raid` moved to function top | 7221 | 17172 |
| tail loop `i < (unsigned) hunterCount` | 5902 | 17164 |

## Conclusion

No source win this wave.  The remaining residual is three independent backend
decisions that no source spelling tested flips:

1. `this`/`i`/`Globals::globals`/`Globals::rnd` register permutation in the tail
   (`this` r8-vs-r9, `i` r9-vs-r8, globals sl-vs-r4, rnd `[sp,#120]`-vs-sl).
2. `raid`@84 / saved-`fp`@88 stack-slot swap (first byte difference at `b0024`).
3. `case 183` block layout 0,1,2 vs 0,2,1, which reorders the literal pool and
   shifts every PC-relative load in the block by 4 bytes.

The alien-turret loop's faithful statement order is *not* the byte-matching
order under clang 7.0.2: the checked-in mid-loop `setScaling` + fresh
`(*enemies)[i]` receiver is the cheapest shape (5761 vs 5865/5911).

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w7776/rawfn.py      # positional byte diff inside the symbol
python3 /dev/shm/w7776/measure.py    # .text diff + report row + difflib bytediff
python3 /dev/shm/w7776/diff.py LO HI # per-instruction diff in an address range
python3 /dev/shm/w7776/regions.py    # group differing bytes by instruction
cmake --build cmake-build-match --target check -j6   # RATCHET PASS
```

Scratch: `/dev/shm/w7776/{Level.base.cpp,rawfn.py,measure.py,diff.py,regions.py,
runvar.py,apply.py,gen_if.py,gen_switch.py,orig.asm,ours.asm,v*.txt}`.

## Gate counts (before -> after)

```text
byte_exact      4506 -> 4506   (4506/4524)
linked_exact    4506 -> 4506   (4506/4524)
size_exact      4524 -> 4524
data_exact       843 -> 843
imports          207 -> 207
lint            CLEAN   -> CLEAN
drift           0       -> 0
RATCHET         PASS    -> PASS
.text diff      85022 -> 85022
```
