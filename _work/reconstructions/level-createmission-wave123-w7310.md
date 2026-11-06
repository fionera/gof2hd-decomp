# w7310: `Level::createMission` (92.326733 -> 92.326733 %, 17172 B) — wave-123
# nameLeader dead fall-through, case-4 `this` hoist, turret SLP pairs, tail slot; no source win

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w7310`, branch `wave123-w7310`,
based on current master (`1c8c9b20e`).  Ground truth:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(sha256 `d4c17be6…bed9483`, image base 0x10000; function `0xaffd8`-`0xb42ec`,
17172 B).  Continues `level-createmission-wave121-w7112.md` and
`…-wave122-w7214.md`.  `src/game/world/Level.cpp` is unchanged; this note is
the only commit.

## Baseline (pristine HEAD, re-measured this session)

```text
_ZN5Level13createMissionEv   92.326733 %   size 17172/17172 B
                             6055 orig / 6065 ours insns
gate: byte 4506/4524, linked 4506/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, RATCHET PASS
```

The fuzzy ratio is `2*M/(6055+6065)` with `M = 5595`.  The call sequence is
already identical (wave-122), and every literal-pool word matches exactly
(the reconciled diff has **zero** `pool:` entries), so there is no wrong
callee and no wrong constant left.

## 1. New evidence: the `nameLeader` block has a dead return in the original

The original's `if (nameLeader)` block at `+0x40b2` returns on **both** paths:

```text
0x40b2  ldr   r0, [sp, #28]      ; nameLeader
0x40b4  cmp   r0, #0
0x40b6  beq.w 0x2f2              ; !nameLeader -> return
0x40ba  …  blx getText(1663)     ; then `(*enemies)[0]->name = …`
0x40da  b.w   0x2f2              ; nameLeader -> return  (extra branch)
0x40de  ldr.w r8, [sp, #124]     ; case4Else
```

Our source falls through from the name assignment into `case4Else`, so the
`b.w 0x2f2` at `+0x40da` is missing.  This is a real CFG difference, not RA.
Every source spelling that adds the return was measured:

| variant | % | size |
|---|---:|---:|
| **baseline (fall-through to `case4Else`)** | **92.326733** | 17172 |
| `if (nameLeader) { … } else return; return;` | 88.432004 | 17152 |
| `if (!nameLeader) return; name = …; return;` | 88.432004 | 17152 |
| move `case4Else`/`case4Tail` outside the `isEmpty` block + `return;` | 88.432004 | 17152 |
| `goto case4Return;` (shared return label after `case4Tail`) | 88.432004 | 17152 |
| `if (nameLeader) { …; return; }` (no else) | 91.237624 | 17172 |
| `if (!nameLeader) return; name = …;` (no trailing return) | 92.326733 | 17172 |

With an explicit return, MachineBlockPlacement moves the whole `nameLeader`
block to the end of the function (after `case4Else`/`case4Tail`, at `+0x41de`)
and tail-merges the two returns, shrinking the function by 20 B.  The original
keeps the block in place, before `case4Else`.  No source spelling reproduces
"returning block kept before the shared tail": the `goto`/shared-label forms
are canonicalised back to a single return.

## 2. The `case 4` block hoists `this` before the `cmp`

Original `case 4` (`+0x40…`, norm index 2365-2377):

```text
ldr.w r0,[fp]; movs r1,#2; blx nextInt
cmp   r0,#0
beq.w case4Else
ldr.w r0,[fp]; movs r1,#2; blx nextInt
adds  r1,r0,#2; blx createRoute
ldr.w r8,[sp,#124]       ; `this` loaded AFTER createRoute
str.w r0,[r8,#264]
b.w   case4Tail
```

Ours loads `this` (`ldr.w sl,[sp,#124]`) **before** the `cmp`.  All spellings
tried (`if (nextInt(2) == 0) goto case4Else;`, `Route *r = createRoute(…);
this->playerRoute = r;`, a `case4Route:` label) canonicalise to the same IR
and score 92.326733 / 17172.

## 3. Turret loops: SLP load/store pairs and the `this` register

The alien-turret loop is the dominant residual (~2.7 pp in wave-118).  The
original keeps `this` in **r4** (clobbered by `&rotation` and reloaded at the
loop end), `gun` in r6, `v` in r5, `k*24` in r9, `i` in fp, table in sl.  Ours
keeps `this` in **r5** (live across the whole loop), `gun` in r4, `v` in r8,
`i` in r9, `k*24` in fp, table in r6.  The cause is the fresh
`(*this->enemies)[i]` load used for `setScaling`: it keeps `this` live across
both `MatrixRotateVector` calls, so RA cannot reuse r4 for `&rotation`.

The original also does **not** form SLP pairs in the vector construction:
it loads `t[2], t[4], t[3], t[1], t[5]` as separate `ldr`s and stores the two
`Vector`s with `str`; ours forms `ldrd r1,r2,[r0,#12]` / `strd r1,r2,[sp,#200]`
for `{t[3],t[4],t[5]}`.  The pirate-fortress loop shows the same split:
original `ldmia r2,{r0,r1,r2}` / `stmia r3!,{r0,r1,r2}` for `{t[0],t[1],t[2]}`,
ours `ldrd r0,r1,[sl,#-8]` + `ldr r2,[sl]`.

Measured variants (all against baseline 92.326733 / 17172):

| variant | % | size |
|---|---:|---:|
| **baseline (fresh `(*enemies)[i]` setScaling, `i<0` anchor)** | **92.326733** | 17172 |
| faithful: cached `gun`, setScaling after setPosition, cargo then shipGroup | 88.384380 | 17140 |
| same + `i<0` anchor | 92.193431 | 17172 |
| same + anchor, shipGroup then cargo | 92.209936 | 17172 |
| fresh `(*enemies)[i]` setScaling after setPosition + anchor | 92.260726 | 17172 |
| cached `gun` setScaling before setRotation (anchor) | 90.955603 | 17164 |
| fresh load into a local `gun2`, setScaling before setRotation | 92.326733 | 17172 |
| `Vector a={…}; Vector b={…};` before the first rotate | 90.853558 | 17156 |
| named `float t0..t5` locals | 92.326733 | 17172 |
| `Vector b` declared just before the second rotate | 88.602789 | 17164 |
| `Array<KIPlayer*> *enemies = this->enemies;` cache in the loop | 90.898589 | 17164 |
| cached loop bound `int start/end` | 87.284429 | 17180 |
| `start` cached, end expression unchanged | 92.326733 | 17172 |
| anchor `(bigAlienWave ^ (i<0))` | 92.326733 | 17172 |
| anchor `(bigAlienWave \| (i<0))` | 92.277228 | 17172 |
| anchor `(bigAlienWave + (i==-1))` | 88.676289 | 17180 |
| anchor `(bigAlienWave + (k<0))` / `+ (k==-1)` | 88.413577 | 17140 |

## 4. `case4Tail`: `this`/`globals` swap and `fp = 1`

Original tail loop (`+0x4184`): `this` in r8, `Globals::globals` in sl,
`Globals::rnd` reloaded from `sp+120` each iteration, `fp = 1` (the `hostile`
argument), condition reloads `this->enemies->size()`, and the two stack
arguments are stored with `strd r0,fp,[sp]`.  Ours (baseline) caches `n` in a
`volatile` at `sp+312`, keeps `Globals::rnd` in fp, materialises `1` with
`movs r0,#1; str r0,[sp,#4]`, and swaps `this`/`globals` (this=sl, globals=r8).

| variant | % | size |
|---|---:|---:|
| **baseline `volatile unsigned n` latch** | **92.326733** | 17172 |
| faithful `i < this->enemies->size()` (no latch) | 91.341312 | 17164 |
| faithful + `const bool hostile = true` | 91.341312 | 17164 |
| faithful + `1, 0` | 91.341312 | 17164 |
| faithful + `(unsigned)` cast | 91.341312 | 17164 |
| faithful + `Array<KIPlayer*> *enemies` cache | 91.144673 | 17164 |
| baseline + `const bool hostile = true` | 92.326733 | 17172 |
| baseline + `bool hostile = hunterCount != 0` | 91.042560 | 17180 |
| baseline + `1, 0` | 92.326733 | 17172 |
| `volatile unsigned long n` / `const volatile` / `+ 0` / `unsigned volatile` | 92.326733 | 17172 |

The faithful form is 8 B short; the original's extra bytes are the Thumb32
`mov.w fp,#1` + `strd r0,fp,[sp]` pair, which only appear if `Globals::rnd` is
spilled instead of kept in fp.

## 5. `raid` / `__stack_chk_guard` slot swap (`sp+84` <-> `sp+88`)

Re-confirmed: `(security == 3) && raid` flips the slot (17 refs) but re-lays
the function and costs ~5 pp; `raid && (security == 3)` is neutral (slot does
not flip).  Polarity rewrites (`if (raid)`, `if (raid != 0)`, `if (!mission)`,
`if (inAlienOrbit() != 0)`, `if (isEmpty() != 0)`, `if (nameLeader != 0)`,
`if (alienGuns)`) are all neutral.  `campaign >= 32 && raid` and
`raid & (campaign >= 32)` are also neutral; `campaign >= 32 && raid` (with
`raid` second) costs 3-4 pp.

## 6. `case 183` offset chain (re-check)

Original tests `2,1,0` and lays bodies out `k0,k1,k2,default,join`; ours tests
`0,1,2` and lays out `k2,k0,k1,join,default` (one extra `b.n` after the join,
+2 instructions).  New forms tried this session:

| variant | % | size |
|---|---:|---:|
| **baseline if-chain 2,1,0, default body y,z,x** | **92.326733** | 17172 |
| `switch` c210 with `case 3: default:` sharing the default body | 92.326733 | 17172 |
| `switch` default-first / default-middle | 89.235595 | 17164 |
| `if (k > 2) default; else if (k==2)…` | 88.606760 | 17224 |
| pre-initialise `offset` to the default, then if-chain | 89.266843 | 17148 |
| `switch` c210 with default body y,x,z | 89.686469 | 17188 |

The `k1`/`k2` body order is a MachineBlockPlacement decision; no source form
reaches `k0,k1,k2,default,join`.

## 7. Misc re-checks (all neutral, 92.326733 / 17172)

* `case 2` convoy `enemies` cache / `convoyShip` local: dropping either costs
  2-3 pp (90.0-90.2 / 17180-17188).
* raid-loot `spawnPosition` via a local `Vector pos` (88.654179 / 17164) or
  named `sx/sy/sz` (91.527102 / 17172).
* `if ((security == 3) & raid)` -> `raid & (security == 3)` neutral.
* `if (raid && campaign>=32 …)` -> `campaign>=32 && raid && …` 88.938382 /
  17180; `raid & (campaign>=32)` 88.628680 / 17188.
* `if (mission == nullptr)` -> `!mission`, `if (mission->isEmpty())` ->
  `!= 0`, etc.: neutral.

## 8. Conclusion

HEAD remains a strict local optimum at **92.326733 %** (17172 B, 6055/6065
insns).  ~55 new variants were measured this session on top of the ~1000 in
the preceding wave-118/119/121/122 notes; none beat the baseline.  The
residual is entirely backend:

1. alien-turret `this` r5-vs-r4 / `gun` r4-vs-r6 / SLP `ldrd`-vs-`ldr`
   (~2.7 pp) — a global register permutation driven by the fresh
   `(*enemies)[i]` setScaling load;
2. `case4Tail` `this`/`globals` swap and `fp=1`-vs-`fp=rnd` (~1 pp);
3. `raid`/guard `sp+84`/`sp+88` swap (17 sites, ~0.28 pp) — needs the shared
   epilogue frequency to rise from 0.568760 to > 0.572958;
4. `nameLeader` extra `b.w 0x2f2` (4 B) — MBP moves the block if the return
   is spelled explicitly;
5. `case 183` k1/k2 body order (+2 insns) — MBP;
6. branchless `&&` at `+0x2e04`/`+0x2f52` — LICM hoists `!alienWave`;
7. pirate-fortress `ldmia`-vs-`ldrd` SLP pair.

## 9. Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=… -DGOF2_FMOD_LIBDIR=…
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w7310/score.py            # 6-decimal ratio, size, insns
python3 /dev/shm/w7310/diff.py             # reconciled side-by-side diff
python3 /dev/shm/w7310/abstract.py         # register/slot-free structural diff
python3 /dev/shm/w7310/dumpi.py FN LO HI   # index-labelled disassembly
python3 /dev/shm/w7310/calls.py            # call sequence with symbol names
python3 /dev/shm/w7310/batch.py v*.txt     # apply/build/score/restore variants
cmake --build cmake-build-match --target check -j6
```

Scratch under `/dev/shm/w7310` (`Level.base.cpp` = pristine HEAD copy;
`v_*.txt` = the ~55 variant specs for this session).
