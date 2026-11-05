# w6788: `Level::createMission` (92.293729 -> 92.293729 %, 17172 B) — wave-119
# continuation sweep: case-183 body/pool order, turret table select, `&&`-vs-`|`,
# frame-slot 84/88, and ~60 further source-shape probes; no source win

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w6788`, branch `wave119-w6788`,
based on current master (`cb5a82070`).  Ground truth:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; `_ZN5Level13createMissionEv` = 0xaffd8-0xb42ec, 17172 B).
This continues `level-createmission-wave119-w6782.md` and
`level-createmission-wave119-w6773.md`.  `src/game/world/Level.cpp` is unchanged;
this note is the only commit.

## Baseline (pristine HEAD, re-measured)

```text
_ZN5Level13createMissionEv   official 92.293729 %   size 17172/17172 B
                             6055 orig / 6065 ours insns
gate: byte 4505/4524, linked 4505/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, RATCHET PASS
ELF: 27 sections, file 2,187,328 B, .ARM.extab 0x26290 (8 B short, createShip only).
```

`score.py` (official ratio) is the metric quoted everywhere below; every variant
keeps 27 sections and file size 2,187,328 B unless noted.

## 1. Case-183 convoy switch: the body order is `[0][1][2][default]`, ours `[0][2][1][default]`

The original literal pool at 0xb15bc..0xb15e8 is (verified by reading raw bytes):

```text
case0: -3000 20000 -50000
case1: -7000 10000 -30000
case2: 25000 -36000
default: 6000 12000 -45000
```

so the original's *body* order is `[case0][case1][case2][default]`, while its
*test* chain is descending (`cmp #2; beq; cmp #1; beq; cbnz`).  Our baseline
emits bodies `[case0][case2][case1][default]` and pool
`[-3000,20000,-50000, 25000,-36000, -7000,10000,-30000, -45000,6000,12000]`.
A plain `switch` on `k` reproduces the descending tests but not the body order.

All six source case permutations were rebuilt and their pools read back:

| source case order | % | pool body order |
|---|---:|---|
| 012 | 92.145215 | `[0][2][1][d]` |
| 021 | 92.145215 | `[0][2][1][d]` |
| 102 | 92.161716 | `[1][0][2][d]` |
| 120 | 92.161716 | `[1][0][2][d]` |
| 201 | 92.293729 | `[2][0][1][d]` |
| 210 | 92.293729 | `[2][0][1][d]` |

The first-listed case is always the fallthrough block; the remaining two are
never emitted in the order needed to get `[0][1][2]` (the `{1,2}` pair always
comes out `[2][1]`).  `switch` with `default` written first (`d,0,1,2`) is
byte-identical to the baseline if-chain 2,1,0 (92.293729).  Nested
`else { if ... }` forms are also identical to the flat forms
(`nested012` 92.145215, `nested210` 92.293729).

Per-case assignment order was also swept (it controls the per-case store order
and hence the pool order inside each case):

| variant | % | size | insns |
|---|---:|---:|---:|
| **baseline (default `[1],[2],[0]`)** | **92.293729** | 17172 | 6065 |
| default `[2],[0],[1]` | 92.277228 | 17172 | 6065 |
| default `[0],[2],[1]` | 91.774606 | 17172 | 6066 |
| default `[1],[0],[2]` | 89.653465 | 17188 | 6065 |
| default `[0],[1],[2]` | 88.606760 | 17224 | 6075 |
| case0 `[1],[0],[2]` | 91.791106 | 17172 | 6066 |
| case0 `[1],[2],[0]` | 91.741605 | 17172 | 6066 |
| case1 `[1],[0],[2]` | 92.244224 | 17172 | 6065 |
| case2 `[1],[0],[2]` / `[1],[2],[0]` | 92.178218 | 17172 | 6065 |
| `float offset[3]` hoisted out of the loop | 92.277228 | 17172 | 6065 |
| `Vector offset` + `.x/.y/.z` | 89.917492 | 17176 | 6065 |

Conclusion: the baseline per-case assignment orders are already score-optimal;
the residual `[0][1][2]`-vs-`[0][2][1]` body order is a
`MachineBlockPlacement` decision not reachable from the source order.

## 2. Turret table select: original is an in-loop `cbz` branch, ours an in-loop `select`

Original (b32ee..b32fc):

```text
ldr  r0,[sp,#64]      ; bigAlienWave
cbz  r0, 0x32f8
ldr  r1,[pc]          ; kAlienStationTurrets
add  r1, pc
b    0x32fc
0x32f8: ldr r1,[pc]   ; kAlienOutpostTurrets
add  r1, pc
0x32fc: add.w r0, r1, r9   ; r9 = k*24
```

Ours (baseline) uses `cmp.w r1, r9, asr #31; it ne; movne r6, r0` — a select.
A faithful `if` statement produces the branch but LICM hoists it out of the
loop, which costs ~4 %:

| variant | % | size | insns |
|---|---:|---:|---:|
| **baseline anchor `(bigAlienWave + (i < 0)) ? .. : ..`** | **92.293729** | 17172 | 6065 |
| plain ternary / `bigAlienWave == 1` / `if`-statement (branch hoisted) | 88.380543 | 17140 | 6054 |
| `if`-statement with the `(i < 0)` anchor (if-converted back) | 92.293729 | 17172 | 6065 |
| `const float *t = bigAlienWave ? table[k] : table[k]` (row pointer) | 83.706334 | 17156 | 6054 |
| same without anchor | 83.702114 | 17156 | 6057 |
| anchor `(i == -1)` | 88.643299 | 17180 | 6070 |
| anchor `(bigAlienWave && i >= 0)` / `(i >= 0 && bigAlienWave)` | 92.128713 | 17172 | 6065 |
| `bool bigAlienWave` | 89.228229 | 17156 | 6060 |
| `bool alienWave` | 89.615321 | 17156 | 6059 |
| both `bool` | 89.404192 | 17164 | 6063 |
| `volatile int bigAlienWave` | 87.740444 | 17140 | 6058 |
| `volatile int alienWave` | 66.782178 | 17148 | 6065 |
| both `volatile int` | 70.223837 | 17104 | 6052 |

The `(i < 0)` anchor is load-bearing: removing it costs 3.9 %.  The original's
in-loop branch cannot be reproduced by an `if` statement because clang
if-converts and then LICMs it.

## 3. `&&` at 1904/1924: original is a branchless OR-of-negations

Original (b2e04):

```text
subs.w r0, fp, r3       ; i - (friendCount+field_164)
mov    r1, r0
it     ne
movne  r1, #1           ; r1 = (i != X)
ldr    r2, [sp,#60]     ; alienWave
eor.w  r2, r2, #1       ; !alienWave   (compiler knows alienWave in {0,1})
orrs   r1, r2
bne.w  <else>
```

Ours short-circuits (`ldr alienWave; cmp #0; beq; cmp i,X; bne`).  Writing the
condition as a bitwise OR does produce the branchless form but changes global
register allocation:

| variant | % | size | insns |
|---|---:|---:|---:|
| **baseline `i == X && alienWave`** | **92.293729** | 17172 | 6065 |
| `!(i != X \|\| !alienWave)` | 92.293729 | 17172 | 6065 |
| `!(i != X \|\| alienWave == 0)` | 92.062706 | 17172 | 6065 |
| `!(i != X \| !alienWave)` (branchless OR emitted) | 82.981182 | 17168 | 6061 |
| `(i == X) & alienWave` | 84.139408 | 17204 | 6082 |
| `bool atStart; atStart & alienWave` | 84.139408 | 17204 | 6082 |

The bitwise-OR form gets the exact `subs/it ne/movne/orrs/bne` shape but loads
a different stack slot and regresses ~9 points elsewhere.

## 4. Frame slots 84/88: it is `raid`, not the stack guard

The w6782 note described the swap as "stack-guard pointer vs raidRace".  A
re-check shows the two slots hold:

```text
orig  [sp,#88] = Globals::status pointer (2 uses; str.w fp at 0x004c)
orig  [sp,#84] = `raid`                   (17 uses; first store at 0x04e8)
ours  [sp,#84] = Globals::status pointer (2 uses)
ours  [sp,#88] = `raid`                   (17 uses)
```

The stack canary is at `[sp,#396]` in both.  `raid`'s slot is a
stack-colouring/RA decision, not a declaration-order one:

| variant | % |
|---|---:|
| **baseline `bool raid;` after `security`** | **92.293729** |
| `bool raid;` moved after `pirates` | 92.293729 |
| `bool raid = false;` + redundant `raid = false` | 92.293729 |
| `if (!(introSystem && !hardCoreMode)) raid = ...` | 92.293729 |
| `bool raid;` moved after `stationIndex` / `raidRoll` / `freelance` / `introSystem` | 91.452145 |
| `raid = raidRoll < ...; if (introSystem...) raid = false;` | 84.884967 (17228, 6072) |
| `int raid` | 92.128713 |

## 5. `volatile unsigned n` (case4Tail hunter loop) is load-bearing

The original reloads `this->enemies->size()` every iteration
(`ldr.w r0,[r8,#248]; ldr r0,[r0,#0]; cmp r9,r0; bcc`).  Our baseline caches it
in a `volatile unsigned n` at `[sp,#312]`, which is *not* what the original
does, but removing the volatile costs 1 %:

| variant | % | size | insns |
|---|---:|---:|---:|
| **baseline `volatile unsigned n`** | **92.293729** | 17172 | 6065 |
| `volatile int n` | 92.293729 | 17172 | 6065 |
| plain `unsigned n` | 92.145215 | 17172 | 6065 |
| no `n` (direct `this->enemies->size()`) | 91.308296 | 17164 | 6060 |
| `volatile unsigned n9` added to the case-9 loop | 90.936082 | 17184 | 6070 |

## 6. Other probes (all <= baseline)

| variant | % | size | insns |
|---|---:|---:|---:|
| `if (alienGuns)` / `if (pirateFortress != 0)` / `if (stationPirates != 0)` / `if (convoy != 0)` / `if (blueprint != 0)` / `if (extraAliens > 0)` / `if (raid != 0)` | 92.293729 | 17172 | 6065 |
| `int guns` moved next to `fortressGuns` | 79.772127 | 17164 | 6057 |
| `int k = i - start` computed in the body | 70.031286 | 17316 | 6091 |
| turret body `rotate, setRotation, rotate, setPosition, setScaling, shipGroup, cargo` | 92.176927 | 17172 | 6063 |
| turret body with `setScaling` last | 90.570085 | 17188 | 6066 |
| fortress body with `setScaling` before `setRotation` | 92.260726 | 17172 | 6065 |
| `gun->cargo` before `gun->shipGroup` | 92.277228 | 17172 | 6065 |
| `bool hard` before `int raidType` | 92.277228 | 17172 | 6065 |
| `const int raidType` | 92.293729 | 17172 | 6065 |
| `*bigShipsCount` truthy / `!= 0` | 89.666557 | 17164 | 6061 |
| `0 < *bigShipsCount` / `>= 1` | 92.293729 | 17172 | 6065 |
| `(security == 3) && raid` | 87.262828 | 17196 | 6067 |
| `raid && security == 3` | 92.293729 | 17172 | 6065 |
| `alienWave ? (bigAlienWave ? 8 : 7) : 0` | 92.244224 | 17172 | 6065 |
| `int hard` | 92.293729 | 17172 | 6065 |
| `unsigned guns/first/escortsEnd/start/total/hunterCount` | 92.293729 | 17172 | 6065 |
| `unsigned attackerCount` | 92.277228 | 17172 | 6065 |
| `unsigned raidRoll` | 92.260726 | 17172 | 6065 |
| `unsigned security` | 92.169321 | 17172 | 6064 |
| `unsigned stationIndex` / `localRace` / `pirates` / `raidRace` | 92.293729 | 17172 | 6065 |
| `const int raidRoll` | 92.293729 | 17172 | 6065 |

## 7. Where the residual lives

`mapdiff2` block census of the baseline: 310 diff blocks, 559 mismatched
normalised insns, concentrated at orig idx 4500-5500 (source lines 1850-2070:
station loop tail, alien-turret loop, raid block, pirate loops).  The first
divergence is instruction 28 (`str.w fp,[sp,#88]` vs `[sp,#84]`, the `raid`
slot above); the first *code-size* divergence is the case-183 VFP literal
hoist documented in `level-createmission-wave119-w6773.md`.  Everything else
inspected is register allocation (`r0`/`r1`/`r4`/`r5`/`r8`/`r9`/`sl`/`fp`
permutations), literal-pool `[pc,#N]` displacement changes, and block
placement — no further front-end (control-flow / statement / constant)
difference was found in this pass.

Best measured variant remains the pristine baseline:
**92.293729 % / 17172 B / 6065 insns**.
