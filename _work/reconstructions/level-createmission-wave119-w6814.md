# w6814: `Level::createMission` (92.293729 -> 92.293729 %, 17172 B) — wave-119
# tail-block / turret-select / raid-decl / branchless-`&&` sweep; no source win

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w6814`, branch `wave119-w6814`,
based on current master. Ground truth:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; function `0xaffd8`-`0xb42ec`, 17172 B).
This continues `level-createmission-wave119-w6804.md`, `…-w6800.md`,
`…-w6782.md`, `…-w6773.md`, `…-w6711.md`.
`src/game/world/Level.cpp` is unchanged; this note is the only commit.

## Baseline (pristine HEAD, re-measured this wave)

```text
_ZN5Level13createMissionEv   official 92.293729 %   size 17172/17172 B
                             6055 orig / 6065 ours insns
gate: byte 4505/4524, linked 4505/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, RATCHET PASS
ELF: 27 sections, file 2,187,328 B, .ARM.extab 0x26290 (8 B short, createShip only).
```

Score decomposition of the same build (tools/verify normalisation):
raw 92.293729 %, register-masked 94.9835 %, register+immediate-masked
96.7987 % (166 diff blocks).  So ~2.7 pp is pure greedy-allocator colour
choice and ~1.8 pp is stack offsets / literal-pool immediates.

## 0. Where the remaining mismatch lives (top diff blocks)

| block | orig | ours | what |
|---|---|---|---|
| turret table select | `ldr [sp,#64]; cbz; ldr;add;b; ldr;add` (branch) | `cmp; it ne; movne` (cmov) | 13 insns |
| raid-loop bound | `mov r8,r5; add...` | `ldr r4,[sp,#112]; add...` | 12 insns |
| `i==X && alienWave` | branchless `subs;mov;it ne;movne;ldr;eor;orrs;bne` | two short-circuit branches | 8 insns |
| case-183 convoy chain | `cmp #2;beq; cmp #1;beq; cbnz` + bodies `[0][1][2][default]` | `cmp #0;beq; cmp #1;beq; cmp #2;bne` + `[2][0][1][default]` | 8 insns |
| guard/raid spill slot | guard `sp+88`, raid `sp+84` | mirrored | 17 insns (idx 28/272/451/463/504/553/…) |

Everything before reconciled index 1871 is address-aligned.  The first
code-size divergence is still the case-183 convoy switch.

## 1. `*bigShipsCount > 0` — `cmp #0; ble` vs `cmp #1; blt`

Original `0xed4 cmp r0,#0 / 0xed6 ble.n 0xf14`; ours `cmp #1 / blt.n`.
Both are the false branch of the same signed test, but LLVM canonicalises
`icmp sgt x,0` to `icmp sge x,1`, so only an IR `icmp sle x,0` (i.e. a
source `!(x>0)` / `x<=0`) can produce `ble`.  Rewriting the enclosing
`if` in the negated form changes the block layout far more than the one
instruction is worth:

| variant | official % | size | insns |
|---|---:|---:|---:|
| **baseline `*bigShipsCount > 0 && localRace == 0`** | **92.293729** | 17172 | 6065 |
| `localRace == 0 && *bigShipsCount > 0` | 74.190885 | 17172 | 6057 |
| `!(localRace == 0 && *bigShipsCount > 0)` + swapped bodies | 74.207398 | 17164 | 6057 |
| `localRace != 0 \|\| *bigShipsCount <= 0` + swapped bodies | 73.885771 | 17180 | 6061 |
| `localRace != 0 \|\| !(*bigShipsCount > 0)` + swapped bodies | 73.885771 | 17180 | 6061 |
| nested `if (localRace == 0) { if (*bigShipsCount > 0) … }` | 75.224965 | 17272 | 6058 |

The `&&` operand order is load-bearing: with `localRace` first,
SimplifyCFG hoists the `else if (localRace == 1)` test and the whole
chain is laid out differently (it emits `cmp r9,#1; beq` first).  The
current source order is the only one that reproduces the original's
`localRace`-then-`bigShips` branch order.

## 2. `case4Tail` block — hoisting `Globals::rnd` / `playerRoute`

The original's post-`ArraySetLength` block is 6 insns
(`ldr [pc]; mov fp,#1; mov r9,#0; add r0,pc; ldr sl,[r0]; b`), ours 11
(the extra 3 are the `volatile n` cache, plus `fp = Globals::rnd` and a
`r8 = this` reload).  Hoisting the loop's `Globals::rnd` /
`this->playerRoute` / waypoint into locals only perturbs the allocator:

| variant | official % | size | insns |
|---|---:|---:|---:|
| **baseline** | **92.293729** | 17172 | 6065 |
| `AERandom *rnd = Globals::rnd;` before the loop | 91.080122 | 17168 | 6064 |
| `Route *route = this->playerRoute;` before the loop | 92.103309 | 17172 | 6064 |
| both | 90.966092 | 17172 | 6066 |
| `int len = this->playerRoute->length();` in the body | 92.062706 | 17172 | 6065 |
| `Waypoint *wp = …` before `createShip` | 90.940594 | 17172 | 6065 |
| `volatile unsigned n = this->enemies->count;` | 92.293729 | 17172 | 6065 |
| `for (i < this->enemies->count)` (faithful) | 91.308296 | 17164 | 6060 |
| `*(volatile unsigned *)&…->count` | 91.308296 | 17164 | 6060 |
| `for (i = 0, n = …->count; i < n; i++)` | 92.145215 | 17172 | 6065 |
| `for (i = 0, n = …->size(); i < n; i++)` | 92.145215 | 17172 | 6065 |

Re-confirms wave-119-w6804 §1: the faithful re-read form is the correct
source shape but is 8 B short and loses 1 pp from the tail shift.

## 3. Turret table select — the `(i < 0)` anchor is still required

The original lowers the select as a **branch** (`ldr [sp,#64]; cbz;
ldr r1,[pc]; add r1,pc; b; ldr r1,[pc]; add r1,pc`), ours as a **cmov**
(`cmp r1, r9, asr #31; it ne; movne`).  Every plain source form is
loop-invariant and LICM hoists it out of the loop (function shrinks to
17140 B):

| variant | official % | size | insns |
|---|---:|---:|---:|
| **baseline `(bigAlienWave + (i < 0)) ? … : …`** | **92.293729** | 17172 | 6065 |
| `bigAlienWave ? … : …` | 88.380543 | 17140 | 6054 |
| `t = outpost; if (bigAlienWave) t = station;` | 88.380543 | 17140 | 6054 |
| `t = station; if (!bigAlienWave) t = outpost;` | 88.380543 | 17140 | 6054 |
| `if/else` into `t` | 88.380543 | 17140 | 6054 |
| `(bigAlienWave + (k < 0))` | 88.380543 | 17140 | 6054 |
| `(bigAlienWave + (i < 0) != 0)` | 92.293729 | 17172 | 6065 |
| `(bigAlienWave + (i < 0) > 0)` / `>= 1` / `+ 0` | 92.293729 | 17172 | 6065 |
| `(bigAlienWave + (unsigned)(i < 0))` / `(int)` / `? 1 : 0` | 92.293729 | 17172 | 6065 |
| `((i < 0) + bigAlienWave)` (operand swap) | 91.848185 | 17172 | 6065 |
| `((bigAlienWave != 0) + (i < 0))` | 91.848185 | 17172 | 6065 |
| `int bw = …; bw ? … : …` | 92.293729 | 17172 | 6065 |
| `const float (*volatile t)[6] = outpost; if (…) t = station;` | 68.902189 | 17244 | 6142 |
| same with the anchor condition | 70.170548 | 17244 | 6141 |
| `const float (*const tables[2])[6]` lookup | 92.027072 | 17172 | 6061 |
| `volatile int bigAlienWave` | 87.740444 | 17140 | 6058 |
| `volatile int alienWave` | 66.782178 | 17148 | 6065 |

## 4. Turret body statement order (`setScaling` / `setPosition` / `cargo`)

The original order is `rotate(t[3..5]); geometry->setRotation(v);
rotate(t[0..2]); setPosition(base+v); setScaling(6.0f); cargo=0;
shipGroup=race`, and `setScaling` gets the cached `gun` (`mov r0,r6`),
not a fresh `(*enemies)[i]` load.  The current source deliberately uses
the fresh load and issues `setScaling` before `setRotation`; moving it
to the faithful position is a small loss:

| variant | official % | size | insns |
|---|---:|---:|---:|
| **baseline (setScaling before setRotation, fresh load)** | **92.293729** | 17172 | 6065 |
| `setScaling` after `setPosition`, cached `gun` | 92.176927 | 17172 | 6063 |
| `setScaling` after `setPosition`, fresh load | 92.244224 | 17172 | 6065 |
| `setScaling` after `setRotation`, cached `gun` | 90.906090 | 17164 | 6063 |
| `setScaling` before `setRotation`, cached `gun` | 90.922594 | 17164 | 6063 |
| after `setPosition`, cached `gun`, `cargo` before `shipGroup` | 92.160423 | 17172 | 6063 |
| `cargo` before `shipGroup` (alien loop only) | 92.277228 | 17172 | 6065 |
| `cargo` before `shipGroup` (fortress loop only) | 92.244224 | 17172 | 6065 |

## 5. `raid` declaration / type / spill-slot experiments

The guard pointer (`fp`, `&__stack_chk_guard`) is spilled to `sp+88`
(orig) / `sp+84` (ours) at fn offset 0x4c and reloaded on the return
path at 0x2f2; `raid` gets the other slot.  Flipping it would fix 17
instructions (~0.28 pp) but no source form moves it:

| variant | official % | size | insns |
|---|---:|---:|---:|
| **baseline `bool raid;` + if/else** | **92.293729** | 17172 | 6065 |
| `bool raid = false;` + negated `if` | 92.293729 | 17172 | 6065 |
| `bool raid = introSystem && !hardCore ? false : …` | 92.293729 | 17172 | 6065 |
| declaration moved after `RaidCoords3` | 75.721253 | 17332 | 6042 |
| declaration moved after `raidRoll`-compute | 92.293729 | 17172 | 6065 |
| `char` / `unsigned char` / `signed char` / `short` / `unsigned short` / `int` / `unsigned int` / `long` | 92.128713 | 17172 | 6065 |
| `security = security - 1` / `--security` / `security >= 1` | 92.293729 | 17172 | 6065 |
| `raidRoll` before `stationIndex` | 92.211221 | 17172 | 6065 |
| `stationIndex` after `freelance` | 92.194719 | 17172 | 6065 |
| `__builtin_expect(raid,0)` on `if (raid)` | 73.104246 | 17296 | 6051 |
| `__builtin_expect(raid,1)` on `if (raid)` | 84.908926 | 17192 | 6078 |
| `__builtin_expect` on the raid loop (1) | 65.047345 | 17304 | 6090 |
| `__builtin_expect` on the raid loop (0) | 76.005617 | 17284 | 6052 |
| `__builtin_expect` on the friend loop | 60.735233 | 17360 | 6050 |
| `__builtin_expect(alienGuns != 0, 1)` | 91.963696 | 17172 | 6065 |
| `__builtin_expect(pirateFortress, 1)` | 71.614927 | 17156 | 6057 |

## 6. Misc. neutral / negative variants

| variant | official % | size | insns |
|---|---:|---:|---:|
| `int alienGuns = 0; if (alienWave) alienGuns = …` | 92.293729 | 17172 | 6065 |
| `int fortressGuns = 0; if (pirateFortress) fortressGuns = 5;` | 92.293729 | 17172 | 6065 |
| `alienGuns` inner test `bigAlienWave != 0` / plain | 92.244224 | 17172 | 6065 |
| `int raidCoords[3]` instead of `struct RaidCoords3` | 92.293729 | 17172 | 6065 |
| `int coords[6]` instead of `struct Coords6` | 92.293729 | 17172 | 6065 |
| combinations of the four neutral variants above | 92.293729 | 17172 | 6065 |
| case-183 `(*stops)[2]` before `(*stops)[0]` | 92.227723 | 17172 | 6065 |
| case-183 `enemies[8]->shipGroup` before `field_0x74` | 92.293729 | 17172 | 6065 |
| case-183 `enemies[9]->shipGroup` before `field_0x74` | 92.277228 | 17172 | 6065 |
| case-183 drop `delete cargo` before `cargo = nullptr` (e8) | 87.312077 | 17132 | 6051 |
| case-183 `enemies`/`ArraySetLength` before `enemyRoute` | 89.551007 | 17164 | 6061 |

## 7. Conclusion

HEAD remains a strict local optimum at **92.293729 %** (17172 B,
6055/6065 insns).  ~60 new source variants were measured this wave
(plus the ~200 from the four preceding wave-119 notes) and none beat
the baseline; the near-misses cluster at 92.244224 / 92.277228.

The remaining 7.7 % is:

1. greedy register allocation (`this` r4/r5, `gun` r6/r4, `t` sl/r6,
   `&return-slot` r5/r8, `i`/`k*24` role swap in the turret loop),
2. the guard/`raid` stack-slot colour order (17 insns, immovable from
   source), and
3. the branch-vs-cmov table select and branchless-`&&` lowering, both
   of which are SimplifyCFG/if-conversion decisions that only the
   `(i < 0)` anchor / `!((i!=X)|(flag==0))` spelling can influence, and
   both of those spellings cost more in block placement than they gain.

The two genuinely-wrong source shapes remain the ones recorded in
`level-createmission-wave119-w6804.md` §1–2: the `volatile unsigned n`
tail cache (original re-reads `this->enemies->size()`) and the convoy
k-chain body order `[0][1][2][default]` (LLVM only emits `[0][2][1]`).

## 8. Commands / scratch

```text
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w6814/score.py                  # official %, size, insns
python3 /dev/shm/w6814/sweep.py <variants.json>  # apply/build/score/restore
python3 /dev/shm/w6814/opdiff.py                 # reg+imm-masked diff blocks
python3 /dev/shm/w6814/delta2.py                 # per-address size-delta census
```

Scratch under `/dev/shm/w6814` (`Level.base.cpp` = pristine HEAD copy,
`v22.json`…`v38.json` = this wave's variant batches, `orig.addr` /
`ours.addr` = index/address/instruction dumps).
