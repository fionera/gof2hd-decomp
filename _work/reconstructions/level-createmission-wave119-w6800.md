# w6800: `Level::createMission` (92.293729 -> 92.293729 %, 17172 B) — wave-119
# table-select lowering, raid/guard spill-slot audit, bool/type and case-order
# sweeps; no source win

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w6800`, branch `wave119-w6800`,
based on current master. Ground truth:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; function `0xaffd8`-`0xb42ec`, 17172 B).

This continues `level-createmission-wave119-w6782.md`.  No variant beat the
baseline; `src/game/world/Level.cpp` is unchanged and this note is the only
commit.

## Baseline (pristine HEAD)

```text
_ZN5Level13createMissionEv   official 92.293729 %   size 17172/17172 B
                             6055 orig / 6065 ours insns
gate: byte 4505/4524, linked 4505/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, RATCHET PASS
ELF: 27 sections, file 2,187,328 B, .text 0x1337c8 (exact),
     .rodata 0x41c27 (first 0x41b67 bytes byte-identical to the original;
     the trailing 0xc0 is the mission-case table), .ARM.extab 0x26294
     (original 0x26298, 4 B short; was 8 B short at wave 118).
```

## 1. First divergences (address-aligned diff)

The reconciled instruction streams are identical up to normalised index 27.
The first differences are:

```text
idx  28   orig str.w fp,[sp,#88]      ours str.w fp,[sp,#84]   ; guard ptr spill
idx 257   jump-table .short 0x0903    0x0905                  ; case 1 target
idx 258   jump-table .short 0x0a14    0x0a15                  ; case 2 target
idx 260   jump-table .short 0x0b8a    0x0b89                  ; case 4 target
idx 272   orig ldr.w fp,[sp,#88]      ours ldr.w fp,[sp,#84]   ; guard ptr reload
idx 451   orig str r0,[sp,#84]        ours str r0,[sp,#88]     ; raid spill
idx 1872  orig ldr r1,[pc,#408]       ours ldr r1,[pc,#408]    ; first *address*
          @0x1448                     @0x1456                  ; divergence (case-183
                                                               ; switch body layout)
```

So the whole first 1871 instructions are address-aligned; the only value
differences before that are the guard/raid slot swap and three jump-table
halfwords (case 1/2/4 body offsets, caused by downstream block placement).
The first code-size/layout divergence is the case-183 convoy switch.

## 2. Guard/raid spill slot: confirmed weights, and why the swap is hard

The guard pointer (`fp` = `&__stack_chk_guard`) is spilled to one 4-byte slot
and `raid` to the other; the original has guard at `sp+88` / raid at `sp+84`,
ours is mirrored.  From the MIR before `stack-slot-coloring`
(`-print-after-all`, Virtual Register Rewriter dump, `%stack.74` is the guard
spill):

```text
guard: 2 refs  = store at fn offset 0x4c (BB1) + reload in the single epilogue
guard weight   = freq(BB1) + freq(epilogue) = 0.625 + 0.56876 = 1.19376
raid:  17 refs (14 stores + 3 loads); our build's weight is ~1.4187
```

Both builds have exactly one `__stack_chk_fail` site and the guard store is at
the same function offset 0x4c, so the guard weight is identical in both.  For
the original to give the guard the "better" colour, the original's **raid**
weight must be below 1.19376.  Every `__builtin_expect` spelling that flips the
slot (see the w6782 table) costs >= 7 pp elsewhere, and the pure swap is only
worth +0.31 pp.  `int`/`char`/`unsigned char` raid all land at 92.128713 (the
`bool` type is worth +0.165 pp by itself); `bool raid = false;` + inverted
condition, `raid` declared before `security`, and `raid` initialised first then
cleared are either identical or much worse (`raid_init_then_clear` 84.884967,
17228 B, 6072 insns).

## 3. Alien-turret table select: the original is a real branch, ours is a cmov

The original's in-loop select is a genuine diamond:

```text
orig b32c6: ldr r0,[sp,#64]      ; bigAlienWave
     b32c8: cbz r0,b32d0
     b32ca: ldr r1,[pc]          ; kAlienStationTurrets
     b32cc: add r1,pc
     b32ce: b b32d4
     b32d0: ldr r1,[pc]          ; kAlienOutpostTurrets
     b32d2: add r1,pc
     b32d4: add.w r0,r1,r9       ; t + k*24
```

Ours (baseline anchor `(bigAlienWave + (i < 0)) ? ... : ...`) lowers the
`select` to `cmp.w r1,r9,asr #31; it ne; movne r6,r0`.  Root cause found this
wave:

* The front-end already emits a `select` for the ternary; a statement `if/else`
  also becomes a `select` in the final IR.
* `SimplifyCFG::FoldTwoEntryPHINode` folds the diamond because both incoming
  values are constant GEPs (`DominatesMergePoint` cost 0 <
  `PHINodeFoldingThreshold` = 2).  This happens even when the condition is a
  `volatile` load.
* `LICM` then hoists the select out of the loop whenever the condition is
  loop-invariant (plain `bigAlienWave`), which is why the anchor `(i < 0)` is
  needed.  With the anchor the select stays in the loop but the ARM backend
  lowers it to a conditional move, not a branch.

Measured variants (all `official % / size / insns`; baseline
92.293729 / 17172 / 6065):

| variant | official % | size | insns |
|---|---:|---:|---:|
| **baseline ternary + `(i<0)` anchor** | **92.293729** | 17172 | 6065 |
| statement `if (bigAlienWave + (i<0)) t=A; else t=B;` | 92.293729 | 17172 | 6065 |
| statement-if, initialiser `t = B; if (cond) t = A;` | 92.293729 | 17172 | 6065 |
| statement `if (bigAlienWave)` (no anchor) | 88.380543 | 17140 | 6054 |
| `switch (bigAlienWave) { case 0: B; default: A; }` | 88.380543 | 17140 | 6054 |
| `if (bigAlienWave == 1)` / `bigAlienWave ? A : B` | 88.380543 | 17140 | 6054 |
| `volatile int bigAlienWave` + anchor | 87.740444 | 17140 | 6058 |
| `volatile int bigAlienWave` + statement-if | 87.749711 | 17140 | 6059 |
| `const float (*volatile t)[6]` + statement-if | 68.902189 | 17244 | 6142 |
| `const float (*volatile t)[6]` + anchor | 70.170548 | 17244 | 6141 |
| `const float (*volatile t)[6]` + ternary | 86.962255 | 17188 | 6079 |

The `volatile` pointer variants do produce the original's real branch, but the
volatile store/load costs 72 extra bytes, so they are not competitive.

## 4. Case-183 convoy switch: GPR stores now match; body/literal order does not

The current build already uses the original's GPR `ldr r1,[pc]; str r1,[sp,#N]`
stores (the VFP sinking reported in w6773 is gone).  The remaining difference is
block/literal-pool order:

```text
original literal order: -3000, 20000, -50000, -7000, 10000, -30000,
                        25000, -36000, 6000, 12000, -45000   (k0, k1, k2, default)
baseline literal order: 25000, -36000, -3000, 20000, -50000, -7000,
                        10000, -30000, -45000, 6000, 12000   (k2, k0, k1, default)
```

The original lays the bodies out `[k0][k1][k2][default][shared tail]` with
descending tests (`cmp #2`, `cmp #1`, `cbnz`); ours is `[k0][k2][k1]` with the
same descending tests.  All six source orders were re-measured (if-chain and
`switch`):

| source order | official % | literal order |
|---|---:|---|
| **2,1,0 (baseline)** | **92.293729** | k2, k0, k1, default |
| 2,0,1 | 92.293729 | k2, k0, k1, default |
| 1,0,2 | 92.161716 | k1, k0, k2, default |
| 1,2,0 | 92.161716 | k1, k0, k2, default |
| 0,1,2 | 92.145215 | k0, k2, k1, default |
| 0,2,1 | 92.145215 | k0, k2, k1, default |

`switch` case orders give exactly the same numbers.  Reordering the stores to
the original's `offset[1], offset[0], offset[2]` order in all branches drops to
88.606760 / 17224 / 6075; per-branch it is 92.244224 (k1), 92.178218 (k2),
91.791106 (k0).  The default branch's six permutations were also measured:
`1,2,0` is the baseline, `2,0,1`/`2,1,0` give 92.277228, the rest are worse.

## 5. Turret body order (re-confirmed with the current build)

| body | official % | size | insns |
|---|---:|---:|---:|
| **baseline `A Ef B C D F G`** | **92.293729** | 17172 | 6065 |
| `A B C D Ef F G` (fresh setScaling after setPosition) | 92.293729 | 17172 | 6065 |
| `A B C D Ef G F` | 92.277228 | 17172 | 6065 |
| `A B C D E F G` (cached) | 92.176927 | 17172 | 6063 |
| `A B C D E G F` (cached, cargo then shipGroup) | 92.160423 | 17172 | 6063 |
| `PlayerTurret *gun` + cached setScaling | 90.922594 | 17164 | 6063 |
| `PlayerTurret *gun` + fresh setScaling | 92.293729 | 17172 | 6065 |

## 6. `&&` at lines 1904 / 1924 (re-measured)

| variant | official % | size |
|---|---:|---:|
| **baseline `i == X && alienWave`** | **92.293729** | 17172 |
| `!((i != X) | (alienWave == 0))` | 92.062706 | 17172 |
| `(i == X) & (alienWave != 0)` | 92.293729 | 17172 |
| `(i == X) & alienWave` | 75.689058 | 17172 |
| `!((i != X) | (pirateFortress == 0))` | 88.685318 | 17172 |
| `(i == X) & pirateFortress` | 63.045811 | 17312 |

The original's `subs/mov/it/movne/eor/orrs` OR-of-negations shape is only
reached by the `!((i != X) | ...)` spelling, which loses on block placement.

## 7. Bool vs int for every boolean local

| local changed to `int` | official % | size |
|---|---:|---:|
| **baseline (all `bool`)** | **92.293729** | 17172 |
| `resetStation` | 92.293729 | 17172 |
| `hard` | 92.293729 | 17172 |
| `alienRace` | 92.293729 | 17172 |
| `hasWanted` | 89.050372 | 17148 |
| `nameLeader` | 88.839396 | 17180 |
| `introSystem` | 80.227948 | 17164 |
| `siegedStation` | 73.474430 | 17352 |
| `raid` | 92.128713 | 17172 |

`char raid` and `unsigned char raid` also give 92.128713, so the `bool` type of
`raid` is worth +0.165 pp on its own.

## 8. Residual map (unchanged)

* `this` r4 (orig) vs r5 (ours) from the alien-turret preheader onward;
  `gun` r6 vs r4; `&return-slot` r5 vs r8; `k*24` r9 vs fp; `i` fp vs r9;
  `t` sl vs r6.  Same for the pirate-escort loop (`this` sl vs fp,
  `pirateBase` fp vs sl) and the raid loop (`i` sl vs fp).
* guard/raid slot swap (section 2).
* secondary slot cycle sp+40/44/48 (`pirateEscorts`, `extraAliens`,
  `stationPirates`).
* case-183 convoy body/literal order (section 4).
* `&&` at 1904/1924 OR-of-negations (section 6).
* `*bigShipsCount > 0`: orig `cmp #0; ble`, ours `cmp #1; blt`.
* jump-table halfwords for cases 1/2/4 differ by 1-2 (downstream block
  placement, not a source difference).

## 9. Commands / scratch

```text
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w6800/score.py                 # official %, size, insns
python3 /dev/shm/w6800/sdump.py _ZN5Level13createMissionEv LO HI
python3 /dev/shm/w6800/sweep.py <variants.json> # apply/build/score/restore
cmake --build cmake-build-match --target check -j6
```

Scratch under `/dev/shm/w6800` (`Level.base.cpp` = pristine HEAD,
`objdump.txt` = `objdump -d -l` of the `.obj` section, `all.txt` =
`-print-after-all`, `bfi.txt` = `-print-machine-bfi`), variant JSONs
`v_if*.json`, `v_vol*.json`, `v_body.json`, `v_pt.json`, `v_raid*.json`,
`v_and*.json`, `v_conv*.json`, `v_case.json`, `v_sw.json`, `v_bools*.json`,
`v_decl.json`, and `convtest.py` (case-183 literal-pool order probe).
