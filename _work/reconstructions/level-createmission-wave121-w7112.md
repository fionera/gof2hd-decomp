# w7112: `Level::createMission` (92.326733 -> 92.326733 %, 17172 B) — wave-121
# case-183 chain/pool decode, branchless-`&&` LICM, turret-order grid; no source win

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w7112`, branch `wave121-w7112`,
based on current master (`bd061a489`).  Ground truth:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(sha256 `d4c17be6…bed9483`, image base 0x10000; function `0xaffd8`-`0xb42ec`,
17172 B).  Continues `level-createmission-wave119-w6962.md` and
`…-w6970.md`.  `src/game/world/Level.cpp` is unchanged; this note is the only
commit.

## Baseline (pristine HEAD, re-measured this session)

```text
_ZN5Level13createMissionEv   92.326733 %   size 17172/17172 B
                             6055 orig / 6065 ours insns
gate: byte 4505/4524, linked 4505/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, RATCHET PASS
```

The fuzzy ratio is `2*M/(6055+6065)` with `M = 5595`; one extra matching
instruction is `+0.0165 pp` (92.3267 -> 92.3432).  A mnemonic-only alignment
(`difflib` on the opcode field) is **97.953795 %**: ~120 of the 460 unmatched
original instructions differ in opcode, the remaining ~340 are operand-only
(register / stack-slot colouring).  So the residual is dominated by RA, not by
a missing block.

## 1. case-183 `offset` chain: pool decoded byte-for-byte

The original's `for (k = 0; k < 4; k++)` offset switch is at `+0x1434`:

```text
+0x1434  cmp   r1, #2
+0x1440  beq   +0x1460          ; k==2
+0x1442  cmp   r1, #1
+0x1444  beq   +0x1454          ; k==1
+0x1446  cbnz  r1, +0x146c      ; default
+0x1448  k0 body                ; bodies laid out 0,1,2,default
+0x1454  k1 body
+0x1460  k2 body
+0x146c  default body
+0x1476  join: str [sp,#380]     ; offset[2] for every body
```

So the original's **test order is 2,1,0** while its **body address order is
0,1,2,default**.  Each body stores `offset[1]` (y) first, then `offset[0]` (x),
then the join stores `offset[2]` (z).  The literal pool at `0xb15bc` is exactly
the body order:

```text
0xb15bc  -3000   0xb15c0  20000   0xb15c4  -50000     (k0: y,x,z)
0xb15c8  -7000   0xb15cc  10000   0xb15d0  -30000     (k1: y,x,z)
0xb15d4  25000   0xb15d8 -36000                      (k2: x,z; y=0 uses sl)
0xb15dc   6000   0xb15e0  12000   0xb15e4  -45000     (default: y,x,z)
0xb15e8   5000   0xb15ec -100000                      (post-loop literals)
```

The current source (`if (k==2) … else if (k==1) … else if (k==0) … else …`)
emits the same y-first stores but a different layout:

```text
ours (if-chain)   test order 0,1,2   body order 2,0,1,default
ours (switch 012) test order 2,1,0   body order 0,2,1,default
```

The `switch (k) { case 0: … case 1: … case 2: … default: … }` form reproduces
the original **test order** (2,1,0) but MBP still lays the bodies out
`0,2,1,default` (k2 before k1), so it scores lower than HEAD.  All six switch
case orders were measured:

| switch case order | % |
|---|---:|
| `c012` / `c021` | 92.178218 |
| `c102` / `c120` | 92.194719 |
| `c201` / `c210` | **92.326733** (tie) |
| ascending if-chain `0,1,2` | 92.178218 |

`sw_c201`'s pool is `25000,-36000,-3000,20000,-50000,-7000,10000,-30000,
-45000,6000,12000` — still `k2,k0,k1,default`.  Neither source form yields
`0,1,2,default`; the k1/k2 body order is a MachineBlockPlacement decision, not
source.  (Store order `102`/`201` and default-body orders were re-crossed; best
remains the baseline `c210_s012_d120`.)

## 2. Branchless `&&` at `+0x2e04` / `+0x2f2a`: LICM hoists the negation

Original:

```text
+0x2e04  subs.w r0, fp, r3      ; i - X
+0x2e08  mov    r1, r0
+0x2e0a  it     ne
+0x2e0c  movne  r1, #1          ; r1 = (i != X)
+0x2e0e  ldr    r2, [sp,#60]    ; alienWave
+0x2e10  eor.w  r2, r2, #1      ; !alienWave, inline
+0x2e14  orrs   r1, r2
+0x2e16  bne.w  +0x2f2a         ; if ((i!=X)|!alienWave) goto else-if
```

Writing the condition as a value (`if (!((i != X) | !alienWave)) …`) does reach
clang's branchless `EvaluateExprAsBool` path and reproduces 7 of the 8
instructions, but **LICM hoists `!alienWave` into a stack slot in the loop
preheader** (`eor.w r2,r2,#1; str r2,[sp,#116]`), adding 4 B and shifting the
whole block layout:

| variant | % | size | insns |
|---|---:|---:|---:|
| **baseline short-circuit** | **92.326733** | 17172 | 6065 |
| `!((i != X) \| !alienWave)` | 83.283828 | 17176 | 6065 |
| `!((i != X) \| (alienWave ^ 1))` | 85.759050 | 17192 | 6072 |
| `!((i != X) \| (alienWave == 0))` / `(0 == alienWave)` / `(alienWave != 1)` | 88.470744 | 17172 | 6062 |
| anchor `!(alienWave + (i < 0))` (defeats LICM, leaks the anchor) | 92.094405 | 17172 | 6063 |
| anchor `alienWave \| (i<0)` / `^ (i<0)` / `+ (i>>31)` | 89.259198 / 87.126589 / 87.126589 | 17180 | 6067/6063 |
| shared `bool`/`int` `atStation`/`isAlien`/`isPirate` (inside or outside loop, `&&` or `&`) | 92.326733 or worse | | |
| `continue` form `if (cond) { A; continue; }` | 92.326733 (canonicalised) | 17172 | 6065 |
| nested duplicated-C `if (i==X) { … } else { C }` | 80.597750 | 17332 | 6124 |
| nested-continue | 83.319584 | 17164 | 6067 |

The original's inline `eor` is only reachable if `!alienWave` is *not*
loop-invariant, which no source spelling achieves without leaking an anchor.

## 3. Alien-turret loop: 12-way order grid

Original order is createStaticObject, setMaxHitpoints, table select, rotate
(v), `setRotation`, rotate (v2), `setPosition`, `setScaling`, `cargo=nullptr`,
`shipGroup=localRace`.  All 12 combinations of
{setScaling before setRotation / after setRotation / after setPosition} x
{cargo first / shipGroup first} x {fresh `(*enemies)[i]` load / cached `gun`}
were measured:

| variant | % | size |
|---|---:|---:|
| **baseline: before-rotate, shipGroup first, fresh load** | **92.326733** | 17172 |
| before/after-rotate, cargo first, fresh load | 92.310231 | 17172 |
| after-position, cargo first, fresh load | 92.260726 | 17172 |
| after-position, shipGroup first, fresh load | 92.277228 | 17172 |
| any cached-`gun` form | 90.922594-92.209936 | 17164 |

The current source is the best of the 12; the original's `cargo`-before-
`shipGroup` and `setScaling`-after-`setPosition` order both cost more in
downstream block placement than they recover in the loop.

## 4. `raid` stack-slot colouring (`sp+84` vs `sp+88`)

Original keeps `raid` at `sp+84` (17 refs) and the `&Globals::rnd` spill at
`sp+88` (2 refs); ours is swapped.  Type sweep:

| type | % | size |
|---|---:|---:|
| **`bool` (baseline)** | **92.326733** | 17172 |
| `short` / `long` / `unsigned short` / `char = 0` / `int = 0` | 92.161716 | 17172 |
| `volatile bool` | 82.499587 | 17196 |

Unused pad locals (`bool`/`int`/`double`, before and after `raid`) are
dead-stripped and change nothing.  The slot assignment is not movable from
source.

## 5. Misc re-checks (all neutral, 92.326733 / 17172)

* `case4Tail` `createShip(8, 0, …, 1, 0)` / `(…, true, 0)` / `(…, 1, false)`.
* `const bool hostile = true;` hoisted before the tail loop.
* `for (int i = 0; i < (int) n; i++)` in the tail.
* `unsigned hunterCount` alone (92.310231) and the faithful
  `i < this->enemies->size()` re-read (91.341312 / 17164) were re-confirmed.

## 6. Conclusion

HEAD remains a strict local optimum at **92.326733 %** (17172 B, 6055/6065
insns).  ~60 new variants were measured this session on top of the ~700 in the
preceding wave-119 notes; none beat the baseline.  The residual is compiler
register allocation, stack-slot colouring, MachineBlockPlacement and
if-conversion, not source shape.  The three largest addressable clusters and
their blockers are:

1. `raid`/`&Globals::rnd` `sp+84`/`sp+88` swap (17 sites, ~+0.28 pp) — slot
   assignment order, no source spelling moves it.
2. case-183 k-chain body order (`0,1,2,default` vs ours `2,0,1` or `0,2,1`) —
   MBP reorders k1/k2 in MIR before layout.
3. branchless `&&` — LICM hoists `!alienWave`; the inline `eor` is unreachable.

## 7. Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=… -DGOF2_FMOD_LIBDIR=…
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w7112/score.py            # 6-decimal ratio, size, insns
python3 /dev/shm/w7112/dump2.py _ZN5Level13createMissionEv 0x1430 0x1490
python3 /dev/shm/w7112/abstract.py         # register/slot-free structural diff
python3 /dev/shm/w7112/regions.py          # unmatched-original census per 0x400
python3 /dev/shm/w7112/try.py vN.py        # apply/build/score/restore a batch
cmake --build cmake-build-match --target check -j6
```

Scratch under `/dev/shm/w7112` (`Level.base.cpp` = pristine HEAD copy;
`v1.py`…`v40.py` = variant batches; `abstract_now.txt`, `orig_full.txt` =
diffs/disassembly).
