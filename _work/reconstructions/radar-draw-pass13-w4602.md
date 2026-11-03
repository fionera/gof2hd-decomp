# Radar::draw pass 13 — the autopilot slow path is *not* a duplicate in the
# original; it is the top-level block's cold half placed at 0x6ee (w4602)

**Status:** research note, 2026-11-03. Worktree `wt/w4602`, branch
`wave96-w4602`, base `3bcaeb3a`. **No source change landed**: no variant that
keeps `Radar::draw` at 9908 bytes beats the baseline verifier ratio
(0.73925501). This commit contains this note only.

## 0. Baseline (measured before and after this pass)

```
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`_ZN5Radar4drawEP6PlayerP3Hudi`:

```
osize 9908  usize 9908  byte False linked False
raw SequenceMatcher ratio 73.925501 %   normalized stream 3492 (orig) / 3488 (ours)
```

`Radar.cpp` still carries the scoped `-mllvm -likely-branch-weight=80
-mllvm -unlikely-branch-weight=1` override (`CMakeLists.txt:86-87`).

## 1. Correction to pass 12: the original *does* contain the autopilot
##    computation twice in the source, but only once in the binary

Pass 12 proved that `field_0x1a8` (`this+0x1a8`) is stored **once** in the
original (`0x12e1d0`) and twice in ours (`0x12e1d0`, `0x12ebe8`), and concluded
that the campaign-block copy is extra.  That is still true, but the reason the
copy is invisible in the original is **tail merging**, not absence:

```
12e188 ... 12e1b6   top-level autopilot block
12e1b6  beq.w 12e6ee          ; !alienOrbit && goingToPlanet && !isDockingToPlanet
12e1ba  movs r0,#0            ; else field_0x1a8 = false
12e1bc  <DrawImage2D x4 preamble>
12e1d0  strb.w r0,[r4,#424]   ; the only store
...
12e6ee  ldr r0,[r4,#0]        ; <- cold half of the SAME block
12e6f0  blx getPlayer
12e6f4  blx getAutoPilotTarget
12e6f8  str r0,[sp,#152]
...
12e73c  ldr r1,[sp,#152]
12e73e  subs r0,r1,r0
12e740  clz r0,r0
12e744  lsrs r0,r0,#5
12e746  b.n 12e1bc            ; <- jumps back to the single strb
```

So the original binary has exactly one `getAutoPilotTarget` call, one
`getPlanetTargets` call and one `getStationEnumIndex` call, and the slow path
sits at `0x12e6ee` — **exactly where our duplicate block sits** (`0x12e72c`,
same neighbourhood after the planet-hit block).  Our duplicate block is
therefore a *layout filler* that accidentally reproduces the original's block
position; removing it (pass 12's `nodup`) restores the original loop-exit
target but re-rolls the whole allocation.

Instruction counts (objdump, literal pools excluded):

| region (function-relative) | orig | baseline |
|---|---:|---:|
| 0x000–0x180 top autopilot | 135 | 135 |
| 0x180–0x2b0 draws+route | 98 | 98 |
| 0x2b0–0x330 otherLock#1 | 53 | 51 |
| 0x330–0x3b0 landmark filter | 41 | 42 |
| 0x3b0–0x750 campaign | 350 | 346 |
| 0x750–0x7b0 planet-lock tail | 32 | 32 |
| 0x7b0–0xb60 landmark loop | 342 | 348 |
| 0xb60–0xc00 station-lock tail | 65 | 65 |
| 0xc00–0xc40 cold | 22 | 23 |
| 0xc40–0x1a00 enemies | 1232 | 1226 |
| 0x1a00–0x2700 rest | 1184 | 1187 |
| **total** | **3554** | **3554** |

The baseline's *total* instruction count is exactly the original's; only the
block order differs (the duplicate block is offset by 21 fewer instructions in
the campaign region and 6 more in the landmark loop).

## 2. `nodup` + planet-lock store swap (m1) restores the original block order

Removing the duplicate block and swapping the two stores in the planet-lock
tail

```c++
} else if (this->field_0x14 == 0) {
    this->field_0x18 = 0;        // was: planetLockTimer = 0; field_0x18 = 0;
    this->planetLockTimer = 0;
} else { this->planetLockTimer = 0; }
```

gives **9748 B / 68.7951 %** (norm 3492/3430).  It restores the original block
order:

| | original | m1 |
|---|---|---|
| planet-loop exit branch | `12e3dc bcs.w 12e748` | `12e3e6 bcs.w 12e73c` |
| planet-lock tail | `12e748` | `12e73c` |
| `strbeq.w r0,[r4,#536]` sites | `12e760 / 12eb74 / 12f848` | `12e754 / 12eb2a / 12f7c4` |
| `getAutoPilotTarget` calls | 1 (idx 606) | 1 (idx 606) |
| `add.w r0,r4,#376` | idx 673 | idx 664 |

The 5-point loss is **register rotation**, not statement order.  In m1 the
campaign block keeps `campaign` on the stack (`[sp,#148]`) instead of in `sl`,
`status` in `fp` instead of `r5`, `stations` in `r6` instead of `sl`, and the
planet-loop counter in `sl` instead of `fp`:

```
orig: mov sl,r0 (campaign) ... mov sl,r0 (stations) ; mov fp,#0 (i)
m1:   str r0,[sp,#148]     ... mov r6,r0 (stations) ; mov sl,#0 (i)
```

m1's region counts vs the original: otherLock#1 -7, landmark filter +4,
campaign -8, station-lock tail -12, rest +9 (normalised stream 3430 vs 3492,
62 instructions short).

## 3. New: the enemy in-sight test is distance-first in the original, and that
##    is what selects `vpush {d8-d13}`

Original enemy in-sight block (`0x12ef60` region):

```
1364 ldr r0,[pc]            ; &crosshairPos
1365 movs r6,#0
1366 strb r6,[r8,#115]      ; enemy->field_0x73 = 0
1367 add r5,sp,#232
1368 add r0,pc
1369 ldr r0,[r0,#0]
1370 vldr s26,[r0]          ; crossX -> s26
1371 vldr s24,[r0,#4]       ; crossY -> s24
1372 mov r0,r5
1373 ldrd r2,r1,[sp,#108]
1374 blx operator-          ; egoPos - targetPos
1375 mov r0,r5
1376 vcvt.s32.f32 s24,s24   ; cy
1377 vcvt.s32.f32 s26,s26   ; cx
1378 blx VectorLength
1379 vmov s0,r0
1380 ldr r5,[sp,#116]
1381 vcmpe.f32 s0,s20        ; < 60000.0f
1382 vmrs
1383 bpl 0xf70              ; distance fail -> store inSight (r6=0)
1384 vmov r1,s26             ; cx
1385 ldr r0,[r4,#292]        ; size
1386 ldr r2,[r4,#252]        ; screenX
1387 movs r6,#0
1388 subs r3,r1,r0
1389 cmp r2,r3
1390 ble 0xf70
1391 add r1,r0
1392 cmp r2,r1
1393 bge 0xf70
1394 vmov r1,s24             ; cy
1395 ldr r2,[r4,#256]        ; screenY
1396 subs r3,r1,r0
1397 cmp r2,r3
1398 ble 0xf6e
1399 add r0,r1
1400 movs r6,#0
1401 cmp r2,r0
1402 it lt
1403 movlt r6,#1
1404 b 0xf70
```

The crosshair floats are live **across** `operator-`/`VectorLength`, so they
must live in `s24`/`s26` (callee-saved) — this is why the original's prologue is
`vpush {d8-d13}`.  Our baseline computes the bounds first and the distance last,
so `crossX/crossY` are dead before the calls and the prologue is
`vpush {d8-d11}`.

The closest source shape is **i2** (single `&&` chain, distance first,
`inSight = true` inside):

```c++
enemy->field_0x73 = 0;
int cx = (int) PlayerEgo::crosshairPos.x;
int cy = (int) PlayerEgo::crosshairPos.y;
bool inSight = false;
if (AbyssEngine::AEMath::VectorLength(this->egoPos - this->targetPos) < 60000.0f &&
    this->screenX > cx - this->field_0x124 && this->screenX < cx + this->field_0x124 &&
    this->screenY > cy - this->field_0x124 && this->screenY < cy + this->field_0x124) {
    inSight = true;
}
enemy->field_0x73 = inSight;
```

i2 gives `vpush {d8-d13}` and reproduces the in-sight block almost
instruction-for-instruction (only `locked` moves from `r5` to `r6` and the
result from `r6` to `r0`), but re-rolls the downstream register allocation:

```
i2: usize 9924  ratio 71.1894  norm 3492/3495
```

Per-bucket (orig-relative 0x100 buckets, `match/insns`):

| bucket | baseline | i2 |
|---|---|---|
| 0x0600 | 91.2 % | 91.8 % |
| 0x0a00 | 112.7 % | 112.7 % |
| 0x1200 | 89.5 % | **94.8 %** |
| 0x1800 | 110.7 % | 114.0 % |
| 0x1a00 | 94.0 % | 93.4 % |
| 0x1c00 | 97.4 % | 95.3 % |
| 0x2200 | 99.0 % | 96.9 % |
| 0x2400 | 103.5 % | 102.9 % |
| 0x2600 | 100.0 % | 113.3 % |

The in-sight block gains ~5 points on 172 instructions; the later
asteroid/gas-cloud blocks lose more, because `locked`/`enemy`/`dockNavPtr` all
move to different registers.

Other distance-first spellings are much worse: assigning the whole `&&`
expression to `inSight` (i1/i4) 64.7 %, nested `if`s (i6/i7/i9/i15) 64.7 %,
direct `enemy->field_0x73 = <expr>` (i13/i14) 62.4 %.  Only the single-`&&`
with `inSight = true` (i2) keeps the block recognisable.

## 4. New: the second-best source shape is `m1` + a *shrunk* duplicate block

`m1` plus a duplicate block that only recomputes the comparison (drops the
`goingToPlanet() && !isDockingToPlanet()` terms, which the campaign block
already implies) — call it **md2**:

```diff
@@ campaign block, after the planet loop @@
         if (!alienOrbit) {
             int autoPilotTarget = this->level->getPlayer()->getAutoPilotTarget();
             SolarSystem *system = (SolarSystem *) (intptr_t) Globals::status->getSystem();
             int stationIndex = system->getStationEnumIndex(
                     ((Array<int> *) ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getStations())
                             ->data()[this->planetDockIndex]);
-            this->field_0x1a8 = this->level->getPlayer()->goingToPlanet() &&
-                                !this->level->getPlayer()->isDockingToPlanet() &&
-                                autoPilotTarget == ((Array<int> *) this->planetTargets)->data()[stationIndex];
+            this->field_0x1a8 = autoPilotTarget == ((Array<int> *) this->planetTargets)->data()[stationIndex];
         }
@@ planet-lock tail @@
             } else if (this->field_0x14 == 0) {
-                this->planetLockTimer = 0;
                 this->field_0x18 = 0;
+                this->planetLockTimer = 0;
             } else {
```

```
md2: usize 9880  ratio 73.6872  norm 3492/3478
```

This is the closest alternative local optimum found: it is 28 bytes smaller than
the baseline, has a *smaller* total non-equal span (1167 vs 1257 instructions)
and a much better 0x0a00 bucket (97.5 % vs 112.7 %) and 0x1800 bucket
(104.5 % vs 110.7 %), but its match runs are distributed worse, so the
SequenceMatcher ratio is 0.24 points lower.  Its 20-instruction duplicate block
sits between the top slow path and the planet-lock tail, i.e. exactly where the
original has the planet-lock tail, so the tail is still shifted by 20
instructions.

`md2` is stable under: dropping the `if (!alienOrbit)` wrapper (md4, neutral),
`int target` temporary (x2, neutral), `(bool)` cast (x4, neutral).  It is
destroyed by: reverting the store swap (md3, 62.29 %), moving the duplicate
after the planet-lock tail (md5, 70.38 %), at the end of the function (md7,
54.25 %), before `enemies:` (md8, 68.40 %), after the top-level autopilot block
(md9, 68.42 %), keeping `goingToPlanet` only (md6, 72.09 %), adding the i2
in-sight shape (md2i2, 71.92 %), the positive-OR otherLock (md2o6, 72.45 %).

## 5. Confirmed: `planetHit`/`turretMode` stack-slot swap is the `ldrd` cause

Original `!otherLock && !planetHit && !turretMode`:

```
12e4dc ldr r0,[sp,#144]   ; otherLock
12e4de ldr r1,[sp,#152]   ; planetHit
12e4e0 orrs r0,r1
12e4e2 ldr r1,[sp,#148]   ; turretMode
12e4e4 orrs r0,r1
```

Ours:

```
12e4dc ldrd r0,r1,[sp,#144]  ; otherLock + planetHit (adjacent)
12e4e0 orrs r0,r1
12e4e2 ldr r1,[sp,#152]      ; turretMode
12e4e4 orrs r0,r1
```

Original frame: `turretMode=[sp,#148]`, `planetHit=[sp,#152]`.
Our frame: `planetHit=[sp,#148]`, `turretMode=[sp,#152]`.

The slot order comes from `StackSlotColoring` (`ColorSlot` assigns the lowest
free colour in `SSIntervals` weight order, `IntervalSorter` = descending
`li->weight`), so the swap means our `planetHit`+`autoPilotTarget` colour has a
higher spill weight than `turretMode`'s.  Source-level attempts to move it all
failed: `bool/int/unsigned` for `planetHit`/`turretMode` (t1–t7, 9888–9908,
66.1–71.3 %), `planetHit` at function scope before `turretMode` (t8, neutral),
declaration-position sweeps from pass 8/11 (neutral), `int landmarkHit` (t4,
9908 / 70.27 %).

## 6. `otherLock#1` cold half: the original is the positive-OR shape, but it
##    loses elsewhere

Original (`0x12e2fc`):

```
12e304 cbnz r0,12e312      ; if (lockedAsteroid) otherLock = 1
12e306 ldr r0,[r4,#56]
12e308 cbnz r0,12e312      ; if (lockedGasCloud) otherLock = 1
12e30a ldr r0,[r4,#28]
12e30c cmp r0,#0
12e30e beq.w 12ebc2        ; if field_0x1c == 0 -> cold: otherLock = field_0x14 != 0
12e312 movs r5,#1
```

i.e. `if (A || B || C) otherLock = 1; else otherLock = field_0x14 != 0;`.
Our `__builtin_expect(A==0 && B==0 && C==0, 0)` anchor emits the negative-AND
shape (`cbz r0, ...; itt eq; ldreq; cmpeq; beq 0x2480`) with the cold half at
`0x2480`.  The positive-OR spelling (o6) moves the cold half to the cold tail
but costs 1.8 points:

```
o6: usize 9904  ratio 72.1697  norm 3492/3486
```

`__builtin_expect(A||B||C, 1)` (o11) is neutral (9908 / 73.9255); every other
spelling is worse (o2/o5 70.84, o3/o9 67.82, o4/o6/o8 72.17, o7 69.62,
o10 70.78).

## 7. Branch-weight sweeps (baseline, i2, md2): 80:1 is still optimal

`-mllvm -likely-branch-weight=W -mllvm -unlikely-branch-weight=1` for
`W ∈ {1,2,3,4,5,6,8,10,12,15,20,25,30,40,50,60,70,74,80,90,100,110,130,150,200,300,500,1000,2000}`:

| source | best W | best ratio |
|---|---|---|
| baseline | 74–110 (plateau) | **73.9255** |
| i2 | 2 | 71.7354 |
| md2 | 80–200 | 73.6872 |

No weight beats 73.9255 on any of the three shapes.

## 8. Variant sweeps run this pass (official verifier; baseline 9908 / 73.9255)

All rows are the landed `Radar.cpp` plus the listed edit(s).

### 8.1 Campaign tail / conditions (neutral unless noted)

| variant | usize | ratio |
|---|---:|---:|
| `if (!landmarkHit && !turretMode)` (a1) | 9908 | 73.9255 |
| separate `if` + `goto enemies` after planetHit (a2) | 9908 | 73.9255 |
| nested `if (planetHit) { if (!dockAst && !dockPoint) ... }` (a3) | 9908 | 73.8395 |
| `bool docked = dockAst \|\| dockPoint` (a4) | 9904 | 71.0805 |
| `if (!turretMode) { if (!landmarkHit) ... }` (c1) | 9908 | 73.9255 |
| `if (turretMode) {} else if (!landmarkHit)` (c2) | 9908 | 73.9255 |
| `if (!landmarkHit) { if (!turretMode) ... }` (c3) | 9908 | 73.9255 |
| `if (!(turretMode \|\| landmarkHit))` (c4) | 9908 | 73.9255 |
| dock checks before `planetHit` (c6) | 9904 | 71.0805 |
| `!(dockAst \|\| dockPoint)` (c9) | 9908 | 73.9255 |
| `planetLockTimer >= field_0x1b8 + 1` (c12) | 9908 | 73.9255 |
| `planetLockTimer > 0` for `>= 1` (c13) | 9908 | 73.9255 |
| `stationLockTimer >= 1` for `> 0` (c14) | 9908 | 73.9255 |

### 8.2 Duplicate-block placements / spellings

| variant | usize | ratio |
|---|---:|---:|
| baseline (duplicate after planet loop) | 9908 | **73.9255** |
| duplicate before the planet loop (d1) | 9856 | 63.9080 |
| duplicate after the planet-lock tail (d2 = v_dup_end) | 9948 | 70.1890 |
| duplicate inside `if (!turretMode && !landmarkHit)` (d4) | 9944 | 70.1317 |
| `if (alienOrbit) {} else { dup }` (e2) | 9908 | 73.9255 |
| bare `{ dup }` (e3) | 9908 | 73.9255 |
| duplicate without `goingToPlanet`/`isDockingToPlanet` (e5) | 9880 | 66.2265 |
| m1 + duplicate identical to top-level (md1) | 9924 | 72.4800 |
| m1 + duplicate without dock checks (md2) | 9880 | 73.6872 |
| m1 + duplicate with dock checks (md3-like) | 9916 | 72.0894 |

### 8.3 Enemy in-sight shapes

| variant | usize | ratio |
|---|---:|---:|
| baseline (bounds-first, interleaved) | 9908 | **73.9255** |
| distance-first, single `&&`, `inSight = true` (i2) | 9924 | 71.1894 |
| distance-first, `inSight = <expr>` (i1/i4) | 9928 | 64.7126 |
| distance-first, direct `field_0x73 = <expr>` (i13/i14) | 9916 | 62.4138 |
| distance-first, nested ifs (i6/i7/i9/i15) | 9928 | 64.6554–64.7126 |
| distance-first, `field_0x73 = 1` (i3) | 9920 | 71.7435 |

## 9. Conclusion

* The statement set is settled.  The campaign-block autopilot copy is extra in
  the source, but it is a layout filler that reproduces the original's
  out-of-line top-level slow path at `0x6ee`; removing it restores the block
  order but rotates the registers and loses 5 points.
* The residual is entirely block placement + stack-slot colouring + the
  `vpush {d8-d13}` / `{d8-d11}` split:
  1. loop-exit target (planet-lock tail vs duplicate block),
  2. `landmarkHit`/`campaign`/`status`/`stations`/`i` register rotation in
     `nodup`,
  3. `planetHit`/`turretMode` swapped between `[sp,#148]`/`[sp,#152]`,
  4. `otherLock#1`'s cold half at `0x2480` (ours) vs `0xb72` (orig),
  5. enemy in-sight distance-first (needed for `{d8-d13}`) vs bounds-first,
  6. `stations` in `r6` vs `sl` (+1 `mov`).
* Two independent local optima exist: the landed baseline (9908 / 73.9255,
  wrong duplicate block but right registers) and md2 (9880 / 73.6872, right
  block order but rotated registers).  Neither is byte-exact.

### What remains

1. A source shape that keeps the baseline's register assignment **and** the
   original's block order (i.e. `nodup` without the register rotation).
2. A source shape that makes `planetHit` land in `[sp,#152]` after
   `turretMode` in `[sp,#148]` (spill-weight order).
3. A source shape that produces the distance-first in-sight test without moving
   `locked`/`enemy`/`dockNavPtr` out of `r5`/`r8`.
4. A source shape that puts `otherLock#1`'s cold half in the cold tail without
   the 1.8-point loss of the positive-OR spelling.

## 10. Evidence / scratch (all under `/dev/shm/w4602`, not committed)

* `Radar.base.cpp` — pristine landed baseline (9908).
* `Radar.m1.cpp` — nodup + planet-lock store swap (9748 / 68.7951).
* `Radar.md2.cpp` — m1 + shrunk duplicate block (9880 / 73.6872).
* `Radar.i2.cpp` — baseline + distance-first in-sight (9924 / 71.1894).
* `Radar.o6.cpp` — baseline + positive-OR otherLock (9904 / 72.1697).
* `orig.dis`, `ours.dis`, `ours_m1.dis`, `i1.dis`, `i2.dis` — objdump bodies.
* `ghidra_radar.txt` — full headless Ghidra decompilation of `Radar::draw`.
* `m.sh`, `ratio.py`, `bucket.py`, `region.py`, `norm.py`, `cnt2.py`,
  `sweep_*.py` — measurement harness and the sweeps in §8.
* `pei.cpp`, `ssc.cpp`, `armfl.cpp` — LLVM 7.0.1
  `PrologEpilogInserter`/`StackSlotColoring`/`ARMFrameLowering` sources used to
  confirm the stack-slot ordering rule (objects allocated in frame-index order,
  colour = lowest free index in descending spill-weight order).

---

# Addendum (w4602, same wave 96 pass) — call-count proof, prologue, and a
# wider variant sweep; still no strict improvement

**Status:** note-only addendum, 2026-11-03.  `Radar.cpp` is unchanged
(9908 B / 73.9255 %); `check` re-run after restoring the baseline: byte 4500,
linked 4500, size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0.

This addendum records (a) two corrections to §1/§5 of the main note, (b) the
prologue delta, and (c) a second, larger variant sweep that still does not beat
73.9255.

## A1. Correction: the original has *no* campaign copy of the autopilot
##     computation in the binary — the 0x6ee block is the top-level body

§1 says the original "does contain the autopilot computation twice in source,
but only once in the binary" and that the two copies are tail-merged.  The
binary evidence is stronger and simpler than that:

* `getAutoPilotTarget` is called **once** in the original (`0x12e6f4`) and
  **twice** in our baseline (`0x12e6d8` top-level, `0x12e732` campaign copy).
* `getStationEnumIndex` likewise: **once** (`0x12e72a`) vs **twice**
  (`0x12e70e`, `0x12e754`).
* `goingToPlanet`: **1** (`0x12e18c`) vs **2** (`0x12e18c`, `0x12e760`).
* `isDockingToPlanet`: **3** (`0x12e19e`, `0x12e1b0`, `0x12e3ae`) vs **4**
  (`0x12e19e`, `0x12e1b0`, `0x12e3b0`, `0x12e770`).
* The block at `0x12e6ee` has **exactly one predecessor** in the original
  (`0x12e1b6 beq.w 12e6ee`); nothing in the campaign region branches to it.
* That block calls `getStarSystem()->getPlanetTargets()` (`0x12e6fc`,
  `0x12e700`), i.e. it is the *top-level* `targets` form, not the
  `this->planetTargets` form of our campaign copy.  (Note: the original has
  **two** `getPlanetTargets` calls — `0x12e13c` for the member init and
  `0x12e700` in this block — not one as §1 states.)
* Instruction-for-instruction, the original's `0x12e6ee..0x12e746` block is
  identical to our baseline's top-level block `0x12e6d2..0x12e72a` except for
  the `autoPilotTarget` stack slot (`[sp,#152]` vs `[sp,#148]`).

So the original's campaign block simply does not compute `field_0x1a8`; the
top-level else-if body is placed after the planet loop by the IR block
placement pass.  No tail merging is required.  Consistently, `-mllvm
-print-after-all` (NDK clang 7.0.2) shows **no `Branch Folder` pass** in the
`-Oz` pipeline (the only placement/folding passes are `Branch Probability
Basic Block Placement`, `Tail Duplication`, `Early Tail Duplication`), so
post-RA tail merging cannot be the explanation.

The reason removing the copy (`nodup`, 9740 / 62.2903) is so much worse is
therefore **not** "the copy is a layout filler that reproduces 0x6ee"; it is
that the copy's liveness changes the global register allocation.  The
original's allocation at `0x6ee` (`str r0,[sp,#152]`, `mov r9,fp; mov fp,r8;
mov r8,sl; mov sl,r0`) is the *baseline's* (with-copy) allocation, not
`nodup`'s (`mov r9,r0; mov fp,r0`, no shuffling).  This remains the central
unexplained residual: the binary has one copy but the register allocation of
the two-copy build.

## A2. The `vpush` delta is in the prologue (first 16 bytes)

```
orig 12e058: ed2d 8b0c   vpush {d8-d13}
ours 12e058: ed2d 8b08   vpush {d8-d11}
```

The original keeps the crosshair floats in `s24`/`s26` (d12/d13) across the
`operator-`/`VectorLength` calls in the enemy in-sight block; the baseline
uses `s0/s2/s4` and converts immediately.  `i2` restores `{d8-d13}` but, as
§3 says, re-rolls the integer allocation.  Both builds still `sub sp,#296`,
so the frame offsets are unaffected.

## A3. Second sweep (all measured with the official verifier)

### A3.1 Campaign-copy shape (baseline + edit)

| variant | usize | ratio |
|---|---:|---:|
| baseline (full copy, `this->planetTargets`) | 9908 | **73.9255** |
| v1: full if/else copy with `getStarSystem()->getPlanetTargets()` | 9924 | 72.4903 |
| v2: `&&` copy with `getStarSystem()->getPlanetTargets()` | 9916 | 72.1180 |
| v3: v1 without the `if (!alienOrbit)` wrapper | 9924 | 72.4903 |
| h5: copy without `goingToPlanet`/`isDockingToPlanet` | 9880 | 66.2265 |
| h6: copy with `goingToPlanet` only | 9892 | 70.9696 |
| h1: `field_0x1a8 = targets[planetDockIndex] != 0` | 9756 | 62.2652 |
| h2: `field_0x1a8 = planetTargets != nullptr` | 9748 | 62.2922 |
| h3: `field_0x1a8 = planetDockIndex != 0` | 9752 | 62.2453 |
| h4: `autoPilotTarget == targets[planetDockIndex]` (no system/index) | 9772 | 62.2293 |

So the *full* comparison (including the two dock checks) is required for the
baseline's allocation; every smaller copy loses 4–12 points.

### A3.2 `m1` (nodup + store swap) branch-weight sweep

Previously unswept (`§7` covered only baseline/i2/md2).  `m1` best is **W=1**:

| W | 1 | 2 | 3 | 4 | 5 | 6 | 8 | 10 | 15 | 20 | 60 | 70–200 |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| ratio | **69.6532** | 68.5912 | 67.7633 | 67.8391 | 68.9635 | 67.8777 | 67.7932 | 67.7168 | 68.7374 | 67.6879 | 68.8132 | 68.7951 |

(`usize` 9736–9788.)  No weight approaches 73.9255.

### A3.3 `md2` branch-weight sweep below 80

| W | 1 | 2 | 5 | 15 | 25 | 60 | 70 | 80–200 |
|---|---:|---:|---:|---:|---:|---:|---:|---:|
| ratio | 72.7273 | 73.6404 | 72.3960 | 73.5649 | 73.5649 | 73.6585 | 73.6585 | **73.6872** |

`md2` still peaks below the baseline.

### A3.4 `otherLock#1` spellings (baseline + edit)

| variant | usize | ratio |
|---|---:|---:|
| baseline (`__builtin_expect(A==0 && B==0 && C==0, 0)`) | 9908 | **73.9255** |
| e1 `__builtin_expect(A!=0 \|\| B!=0 \|\| C!=0, 1)` | 9908 | 73.9255 |
| e2/e3 plain positive-OR | 9904 | 72.1697 |
| e4 `__builtin_expect(A==0 && B==0 && C==0, 1)` | 9912 | 70.7825 |
| k1/k2 same with `1` (and redundant re-store) | 9912 | 70.7825 |
| f1 init `otherLock = field_0x14 != 0` then OR-override | 9928 | 69.6213 |
| f3 f1 + redundant else | 9888 | 72.7924 |
| f4 OR including `field_0x14 != 0` | 9888 | 73.1679 |
| g1 nested if/else-if chain | 9904 | 72.1697 |
| g2 nested with `__builtin_expect(...,1)` per test | 9912 | 71.8728 |
| b1 `bool otherLock` | 9876 | 67.8151 |
| b2 `unsigned char otherLock` / b3 `int otherLock = 0` | 9908 | 73.9255 |

Note that the original's *machine* shape (`cbnz` for A and B, then
`cmp/beq` for C, `movs r5,#1` after) is the negative-AND short-circuit layout
with the all-null path cold; our baseline's `__builtin_expect` form emits
`cbz A -> check B,C` + `itt eq` predication instead.  No spelling tried
reproduces the original's `cbnz` shape *and* keeps the downstream layout.

### A3.5 Condition reorderings (`!otherLock && !planetHit && !turretMode`)

| variant | usize | ratio |
|---|---:|---:|
| baseline order | 9908 | **73.9255** |
| `!(otherLock \|\| planetHit \|\| turretMode)` | 9908 | 73.9255 |
| `!otherLock && !(planetHit \|\| turretMode)` | 9908 | 73.9255 |
| `!(otherLock \|\| planetHit) && !turretMode` | 9908 | 73.9255 |
| `!planetHit && !otherLock && !turretMode` | 9908 | 73.9255 |
| `!turretMode && !otherLock && !planetHit` | 9840 | 57.6071 |
| `!otherLock && !turretMode && !planetHit` | 9840 | 57.6071 |

### A3.6 Removing the other `__builtin_expect` anchors

| variant | usize | ratio |
|---|---:|---:|
| baseline | 9908 | **73.9255** |
| `onScreen` anchor removed | 9896 | 71.0213 |
| `(i&0x7fffffff)==1\|\|==3` anchor removed | 9912 | 72.1100 |
| both removed | 9904 | 70.9640 |

### A3.7 `locked` declaration moves (enemies loop)

| variant | usize | ratio |
|---|---:|---:|
| baseline (before `stealFlagByte` block) | 9908 | **73.9255** |
| after the `stealFlagByte` block | 9888 | 66.0166 |
| before `wreck`/`field_0x75` | 9900 | 64.7802 |
| before `this->update(enemy)` | 9892 | 71.8199 |
| hoisted out of the enemies loop | 9892 | 72.2788 |
| i2 + hoisted | 9888 | 64.6822 |

### A3.8 Float-typed crosshair locals (distance-first)

| variant | usize | ratio |
|---|---:|---:|
| i2 (`int cx/cy`, distance first) | 9924 | 71.1894 |
| p1 (`float crossX/Y`, cast at use) | 9924 | 71.1321 |
| p2 (`float`, cast inside the distance `if`) | 9928 | 64.3123 |
| p3 (`float` then `int cx/cy` before the `if`) | 9924 | 71.1894 |

### A3.9 Combinations

| variant | usize | ratio |
|---|---:|---:|
| baseline + store swap | 9916 | 72.0894 |
| baseline + o6 + store swap | 9916 | 71.9748 |
| baseline + i2 + o6 | 9920 | 71.0952 |
| baseline + i2 + store swap | 9932 | 71.1016 |
| baseline + i2 + o6 + store swap | 9932 | 71.0443 |
| m1 + o6 | 9788 | 68.9924 |
| m1 + i2 | 9764 | 67.9752 |
| m1 + o6 + i2 | 9804 | 68.1164 |
| m1 + h1 / h2 / h3 / h4 / h7 | 9768/9760/9760/9784/9900 | 68.72/68.75/68.83/68.68/72.09 |

## A4. Conclusion of the addendum

The landed baseline (9908 / 73.9255) remains the best measured shape.  The
residual is still dominated by (i) the campaign-copy liveness keeping the
original's register allocation while the binary has no copy, (ii) the
`planetHit`/`turretMode`/`mode` stack-slot rotation, and (iii) the enemy
in-sight distance-first shape (needed for `vpush {d8-d13}`) re-rolling the
integer allocation.  No strict improvement was found this pass, so the note
is the deliverable.

## A3.10 New: a local `onPlanetCourse` copy restores the original block order
##       at a 0.057-point cost (q2, 9908 / 73.8682)

The closest alternative found in this addendum keeps the *original's block
order* (planet-lock tail immediately after the autopilot block, at
`0x12e75e`) and is only 0.057 points below the baseline.  Shape:

```c++
bool onPlanetCourse;                       // replaces the direct field stores
if (alienOrbit) {
    onPlanetCourse = goingToPlanet() && !isDockingToPlanet();
} else {
    if (goingToPlanet() && !isDockingToPlanet()) { ...; onPlanetCourse = cmp; }
    else { onPlanetCourse = false; }
}
this->field_0x1a8 = onPlanetCourse;
...
if (!alienOrbit) {
    ...recompute the same comparison into onPlanetCourse...
    this->field_0x1a8 = onPlanetCourse;
}
```

GVN CSEs the campaign recompute with the top-level computation (the
disassembly has exactly one `getAutoPilotTarget`/`getStationEnumIndex` call,
at `0x12e6f4`/`0x12e724`), so the campaign block degenerates to
`ldr r0,[sp,#108]; strb r0,[r4,#424]` at `0x12e758`.  The local's long live
range keeps the register allocation close to the original while the block
order follows the original.  Measured variants:

| variant | usize | ratio |
|---|---:|---:|
| baseline | 9908 | **73.9255** |
| q2 (`bool` local, recompute + store) | 9908 | 73.8682 |
| q6 (`int` local) | 9908 | 73.8682 |
| t1/t2/t3 (initialised `int`/`bool` local) | 9908 | 73.8682 |
| s3 (nested if/else recompute) | 9908 | 73.8682 |
| s4 (store field first, then local) | 9908 | 73.8682 |
| u1/u3 (store moved / redundant re-store) | 9908 | 73.8682 |
| r3 (local = field, then recompute) | 9908 | 73.8682 |
| s1 (recompute via `getStarSystem()->getPlanetTargets()`) | 9924 | 72.5190 |
| q1 (local, campaign store only, no recompute) | 9744 | 62.0789 |
| q7 (function-scope `int`, top-level via local) | 9900 | 71.0232 |
| q8 (campaign `local = field; field = local`) | 9752 | 62.2832 |

Branch-weight sweep of q2: best at W=80–100 (73.8682); W=60–70 73.7536;
W=130–200/500–1000 73.8395; W=1 71.3281; W=2 72.5032.

The entire q2-vs-baseline deficit is **one scheduling slot** in the gas-cloud
loop preamble:

```
orig 12f44c: movs r5,#0 ; movs r1,#0 ; str.w r9,[sp,#144] ; str r1,[sp,#96]
base 12f440: movs r5,#0 ; movs r1,#0 ; str.w r9,[sp,#144] ; str r1,[sp,#96]   (matches)
q2   12f440: movs r1,#0 ; movs r5,#0 ; str r1,[sp,#96]    ; str.w r9,[sp,#144] (swapped)
```

The baseline matches this slot; q2 does not, which is exactly the 2-instruction
matched-length difference (2578 vs 2580).  Everything else in the q2 diff list
is a subset of the baseline's.  No source nudge tried (local type/init, store
position, recompute shape/order) changed this scheduling slot, so q2 is not
landable as a strict improvement.
