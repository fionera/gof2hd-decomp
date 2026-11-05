# w6314: `Level::createMission` (92.145215 %, 17172 B) — wave-115:
# convoy body layout, setScaling slot, and literal-pool shift map; no landable gain

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w6314`, branch `wave115-w6314`,
base `b847d5d46`. Ground truth:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; addresses below are linked `.so` addresses, function
`0xaffd8`–`0xb431c`, 17172 B).

**No `src/` change is committed**: every variant measured below is neutral or a
regression on the committed wave-114 baseline. The deliverable is this note.
The worktree is clean.

## Baseline (unchanged, gate green before and after)

```text
_ZN5Level13createMissionEv   fuzzy 92.145215 %   size 17172 B (size-exact)
                             shape (regs+pc collapsed) 95.825083 %
                             nopc (pc collapsed only)  92.821782 %
                             6055 orig insns / 6065 ours
                             314 raw diff regions, total weight 510
gate: byte 4500/4524, linked 4500/4524, size 4524/4524, stubs 0, missing 0,
      wrong_type 0, extra 0, data 843/843, imports 207, lint CLEAN, drift 0,
      RATCHET PASS
ELF shape unchanged by this pass: 27 sections exact, file 2,187,328 B.
```

Every row below is a fresh `cmake --build cmake-build-match --target link_so -j6`
followed by the official verifier normalisation (unrounded).  `shape` collapses
register names and `[pc,#N]`; `nopc` collapses only `[pc,#N]`.

## 0. Where the residual actually is: the literal pool

The 0.68 pp gap between `shape`/`nopc` (≈92.82) and `official` (92.145) is
68 instructions that differ **only** in `ldr/vldr [pc,#N]` offsets.  They are
not random: they cluster by constant-pool group, and each group is shifted by
the code-length difference of the block that precedes it.

| pc-mismatch group | count | shift | cause |
|---|---:|---:|---|
| case-183 / convoy pool (`b15bc` orig) | 22 | +4 | one extra 2-byte `ldr r5,[sp,#116]` + 2 B align pad before the pool |
| case-9 pool (`b3a98` orig) | 17 | −20 | accumulated code-length differences in case 9 |
| raid/`line 2009` pool | 11 | +12 | regalloc |
| remaining groups | 18 | ±4…±140 | regalloc / block-length drift |

The +4 group is the single biggest *source-reachable* pool defect.  The extra
instruction is the hoisted `enemyRace` reload in the `case 1` setup:

```text
orig b1588  ldr r0,[pc,#120] ; movs r4,#0 ; mov.w r9,#1 ; movs r6,#0 ;
            add r0,pc ; ldr.w r8,[r0] ; b.n b1644 ; nop            (16 B)
ours b158a  ldr r0,[pc,#124] ; movs r4,#0 ; ldr r5,[sp,#116] ;      (18 B)
            mov.w r9,#1 ; add r0,pc ; movs r6,#0 ; ldr.w r8,[r0] ;
            b.n b1646 ; nop
```

The original reloads `enemyRace` **inside** the case-1 loop (`b1608:
ldr r5,[sp,#116]`); clang 7.0.2 hoists the load out of our loop and the
2-byte instruction plus alignment padding pushes the whole convoy pool by 4.
Every spelling that keeps the plain `bigAlienWave`/`enemyRace` load produces
the hoist; `volatile` forces the in-loop reload but destroys the function
(71.37 %).  `const`, `if/else`, `int race = enemyRace;` inside the loop, and a
pre-loop `if (enemyRace < 0) …` are all neutral or worse.

## 1. Convoy `k` chain (source line 2187)

The original tests descending (`cmp #2; beq; cmp #1; beq; cbnz`) with bodies
laid out `case0, case1, case2, default` and pool order
`-3000,20000,-50000 | -7000,10000,-30000 | 25000,-36000 | 6000,12000,-45000`.

clang 7.0.2 lays out an if-chain/switch in source order `(a,b,c)` as
`(a, c, b)`; the reachable layouts are only `(0,2,1)`, `(1,0,2)`, `(2,0,1)`.
`(0,1,2)` — the original — is **not source-reachable** from either an if-chain
or a `switch` (all 6 source permutations of each were built; `switch` and
if-chain are byte-identical).

| source order (if-chain == switch) | official | shape | nopc |
|---|---:|---:|---:|
| **210 (baseline)** | **92.145215** | 95.825083 | 92.821782 |
| 201 | 92.145215 | 95.825083 | 92.821782 |
| 102 | 92.013201 | 95.924092 | 92.920792 |
| 120 | 92.013201 | 95.924092 | 92.920792 |
| 012 | 91.996700 | 95.924092 | 92.920792 |
| 021 | 91.996700 | 95.924092 | 92.920792 |

`012`/`021` reproduce the original's *descending test order* and raise the
structural score (`nopc` +0.099 pp) but the body layout is `(0,2,1)`, so the
convoy pool order becomes `case0, case2, case1, default` and the official
score drops 0.15 pp.  This is the cleanest demonstration that the residual is
pool placement, not instruction selection.

## 2. Convoy default-body store order (line 2192)

The original's default body stores `offset[1]=6000, offset[0]=12000,
offset[2]=-45000` (pool order `6000,12000,-45000`).  Our baseline source is
`offset[1], offset[2], offset[0]` and compiles to `offset[2], offset[1],
offset[0]` (pool `-45000,6000,12000`).  Reordering the source is a strong
lever and mostly a regression:

| source order \ default order | 012 | 021 | 102 | 120 | 201 | 210 |
|---|---:|---:|---:|---:|---:|---:|
| 012 | 88.442 | 89.340 | 89.653 | 91.997 | 92.013 | 91.997 |
| 102 | 88.425 | 89.307 | 89.488 | 92.013 | 91.997 | 91.997 |
| 210 | 88.458 | 91.626 | 89.505 | **92.145** | 92.129 | 92.129 |

`default=102` (the order the original's disassembly shows!) makes clang keep
the three constants in NEON registers (`vmov.f32 s0,s16/s18/s20/s22`) instead
of pool loads, costing ~2.5 pp.  `default=120` is the only order that keeps
the pool-load shape; it is the committed baseline.

## 3. `setScaling` position in the alienGuns loop (line 1968)

The original disassembly issues `setScaling(6.0f)` **after** `setPosition`
(`b3366`), not before `setRotation`.  Moving it there is a real structural
improvement but a pool regression:

| variant | official | shape | nopc |
|---|---:|---:|---:|
| **sc1: after setRotation (baseline)** | **92.145215** | 95.825083 | 92.821782 |
| sc0: after setPosition, before shipGroup (matches orig) | 92.095710 | 95.825083 | **92.920792** |
| sc2: after shipGroup | 92.079208 | 95.792079 | 92.904290 |
| sc3: after cargo | 92.079208 | 95.775578 | 92.887789 |

`sc0` is the "faithful shape scores lower" case: it matches 6 more
instructions when pc offsets are ignored but the pool shift costs 0.05 pp
official.  The wave-113/114 commits (`13953b216`, `c36786a2b`) already swept
this axis; the committed position is the tuned optimum.

sc0 × convoy-order × default-order (36 builds) never beats the baseline;
best is `sc1 210 120` = the baseline itself.

## 4. alienGuns table select (line 1963) — the `cbz`/cmov split

Original (`b32c6`): `ldr r0,[sp,#64]; cbz r0,else; ldr r1,[table1]; add r1,pc;
b join; else: ldr r1,[table2]; add r1,pc; join: add.w r0,r1,r9` — a branch,
inside the loop.

Ours with the `(bigAlienWave + (i < 0))` anchor: `cmp.w r1,r9,asr #31; it ne;
movne` — a cmov, inside the loop.  A plain `bigAlienWave ? … : …` (ternary,
if/else, `switch`, local `t`, `(void)k`, `if (k<0) return;`) is hoisted out of
the loop by LICM and costs 88.23 % (shape 90.81).  `(bigAlienWave | (i<0))`
and `(bigAlienWave | (i>>31))` give 92.096; `+ (i>>31)` and `!= (i<0)` are
byte-identical to the anchor.  `(table + k)` forms are worse (83.55 / 88.23).
No spelling produces the original's in-loop `cbz` without also hoisting.

## 5. Station-pirate block (lines 2029–2053)

The original escort loop (`b3bb8`) and ours are instruction-for-instruction
identical apart from register allocation (`s16/s18/s20/s22/s24` vs
`s4/s6/s16/s18/s0`), the x spill slot (`sp+112` vs `sp+104`) and the
`escortsEnd` spill (`sp+104` vs `sp+92`).  Tried and measured:

| variant | official |
|---|---:|
| **baseline** | **92.145215** |
| `int first = stationPirates + guns` | 92.145215 |
| `i < pirates + first + …` | 92.145215 |
| `i < friendCount + … + escortsEnd` | 92.145215 |
| `i < first + pirates + field_164 + friendCount + …` | 92.112211 |
| `escortsEnd = first + pirates + pirateEscorts` | 90.445545 |
| `escortsEnd = pirates + first + pirateEscorts` | 90.445545 |

## 6. Types, raid loop, blueprint tail, misc.

All neutral (92.145215 / 95.825083 / 92.821782) unless noted:

```text
blueprint as int; stationPirates as int; int coords[6] instead of Coords6;
nameLeader as early-return / !nameLeader; float mult = opt44>0.7?1.4f:1.0f;
sc0 x {ty2,ty3,sp1,sp4,sp5,m2_0..3,or0,or1,or3,or5,td0..4,vr1,vr2}
```

Slight regressions:

```text
pirateEscorts unsigned                92.128713 / 95.808581
extraAliens unsigned                  92.128713 / 95.808581
pirateEscorts+extraAliens unsigned    92.112211 / 95.792079
unsigned i -> int i in case 9 / case 8 loops  92.128713 / 95.808581
raid loop bound hoisted to a local    90.943583 / 94.556252
raidStart + raidEnd locals            88.578973 / 91.153656
```

Dead ends:

```text
float offset[3] = {0,0,0} / = {}      89.009901 / 92.838284
Vector offset; offset.x/y/z           89.768977 / 93.481848
setPosition(offset[0],offset[1],offset[2])  89.052180 / 93.064729
hp as one int expression              83.507789 / 89.689277
```

## 7. Structural facts confirmed this pass

* The convoy `k` chain body layout `(0,1,2)` is unreachable; clang 7.0.2
  always emits `(source[0], source[2], source[1])` for a 4-way chain and the
  constant pool follows the body layout.
* The case-183 block (`b10d2`–`b1120`) is byte-identical between the two
  builds, including its `vld1.64`/`vst1.64` coord copy and the pool word
  `0x00151ef3`.  The 2-byte drift at `b10c4` and the 4-byte drift at `b1598`
  both originate in regalloc/scheduling of the surrounding blocks.
* `sp+116` is a heavily reused scratch slot (20+ sites); the case-1 `ldr r5`
  is the hoisted `enemyRace` load, not a distinct variable.
* `setScaling` after `setPosition` matches the original's instruction order
  but loses 0.05 pp official; the committed order is the tuned optimum.

## 8. Conclusion

The residual is `RegAllocGreedy` register/spill-slot assignment plus the
literal-pool placement that follows from block lengths.  The two
source-reachable structural clusters (convoy body layout and the station-loop
`orrs`) are blocked by clang 7.0.2's block placement and `SinkCmpExpression`
respectively, and every spelling tried for them is neutral or worse.  No
variant beat the committed baseline's **92.145215 % / 17172 B**.

## Commands

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6   # RATCHET PASS
python3 /dev/shm/w6314/score2.py _ZN5Level13createMissionEv
```

Scratch tooling (not committed): `/dev/shm/w6314/score2.py` (official/shape/nopc),
`scorefile.py` (per-source-line diff weight), `bigregions.py`/`shapebig.py`
(diff clusters), `showidx2.py` (side-by-side with original addresses),
`pcmiss2.py` (pc-offset-only mismatches with addresses), `try.py`/`try2.py`/
`try3.py` (apply/build/score harness), `mkconv.py`/`mkconv2.py`/`mksw2.py`
(convoy order permutations), `sc.py` (setScaling position), `tbl*.py` (table
select), `orr*.py` (merged condition), `ty.py`/`sp.py`/`raid.py`/`u.py`/`c1.py`
(type/loop variants).
