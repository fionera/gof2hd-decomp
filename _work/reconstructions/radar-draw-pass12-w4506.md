# Radar::draw pass 12 — the duplicate autopilot block is provably extra, and the
# loop-exit target / cold-tail map that follows from it (w4506)

**Status:** research note, 2026-11-03. Worktree `wt/w4506`, branch
`wave95-w4506`, base `4a88bd6e`. **No source change landed**: no variant that
keeps `Radar::draw` at 9908 bytes beats the baseline verifier ratio
(0.73925501). This commit contains this note only.

## 0. Baseline (measured before and after this pass)

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8
```

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

## 1. New proof that the duplicate campaign autopilot block is extra

`field_0x1a8` lives at `this+0x1a8` (byte).  Counting `strb.w ..., [r4,#424]`
(`424 == 0x1a8`) in the two bodies:

| binary | store sites |
|---|---|
| original | **one**: `0x12e1d0` (the top-level `field_0x1a8` block) |
| ours | **two**: `0x12e1d0` and `0x12ebe8` (the duplicate `if (!alienOrbit)` block) |

The original never writes `field_0x1a8` inside the campaign block; it only
*reads* it (`ldrb.w r0,[r4,#424]` at `0x12f0ee`, `0x12f7fc`, `0x12fb46`,
`0x130470` — the same four read sites as ours).  So the duplicate block is
semantically extra, exactly as w3804/w4202 established.

## 2. New: the original's *second* `otherLock` is the same if/else shape as the first

Ghidra (working headless decomp, `ghidra_radar.txt` lines 183-190 and 379-385)
gives the two `otherLock` computations as:

```c
  piVar16 = param_1 + 3;                       /* &lockedAsteroid (offset 0xc) */
  if (((*piVar16 == 0) && (param_1[0xe] == 0)) && (param_1[7] == 0)) {
    bVar33 = param_1[5] != 0;                  /* field_0x14 (offset 0x14) */
  } else {
    bVar33 = true;
  }
  ...
  if (((*piVar16 == 0) && (param_1[0xe] == 0)) && (param_1[7] == 0)) {
    bVar33 = param_1[9] != 0;                  /* lockedStation (offset 0x24) */
  } else {
    bVar33 = true;
  }
```

i.e. the **second** `otherLock` is *not* a plain `A || B || C || D` in the
original's source: it is the same `if (all three null) X = lockedStation != 0;
else X = 1;` shape as the first.  The disassembly confirms it:

```
12e376: ldr  r0,[sp,#124]   ; &lockedAsteroid
12e378: ldr  r0,[r0,#0]
12e37a: cbnz r0, 12e388
12e37c: ldr  r0,[r4,#56]    ; lockedGasCloud
12e37e: cbnz r0, 12e388
12e380: ldr  r0,[r4,#28]    ; field_0x1c
12e382: cmp  r0,#0
12e384: beq.w 12ebce       ; -> cold: otherLock = lockedStation != 0
12e388: movs r0,#1
```

Our source's `A || B || C || D` already lowers to exactly this shape, so no
source change is needed (and none of the 24 orderings / explicit if-else
spellings measured in w4406 or here improves it — see §6).

## 3. New: the loop-exit target is the whole structural story

The planet loop's exit branch and what it lands on:

| | original | ours |
|---|---|---|
| loop-exit branch | `12e3dc bcs.w 12e748` | `12e3dc bcs.w 12e72c` |
| loop-exit target block | `12e748` = **planet-lock tail** (`!turretMode && !landmarkHit`) | `12e72c` = **duplicate autopilot block** |
| top-level autopilot cold block | `12e6ee` (ends `b.n 12e1bc`) | `12e6d8` (ends `b.n 12e1bc`) |
| planet-lock tail | `12e748` (immediately after the cold autopilot block) | `12ebec` (after the landmark loop) |

So both builds put the top-level autopilot cold block *between* the loop body
and the loop-exit target; the original then lands on the planet-lock tail,
ours lands on the duplicate block and pushes the planet-lock tail past the
landmark loop.  Removing the duplicate block (`delcamp`) restores the original
target (`0x708` vs `0x6f8`) but re-rolls the whole allocation and drops the
body to 9740 B / 62.3 %.

Cold-tail map (function-relative, base `0x12e050`):

```
original: 0xb72 otherLock#1-cold  0xb7e otherLock#2-cold  0xb8a missionBlocksPlanets
          0xbcc planetLockTimer=0/field_0x18=0   0xbd4 enemies:
ours:     0xb3a otherLock#2-cold  0xb46 missionBlocksPlanets  0xb84 duplicate block (cold half)
          0xb9c planet-lock tail  0xbda planetLockTimer=0   0xbe6 enemies:
          0x2480 otherLock#1-cold (at the very end of the function)
```

The original's `otherLock#1` cold half is `ldr r5,[r4,#20]; cmp r5,#0; it ne;
movne r5,#1; b.w 12e314` at `0x12ebc2`; ours is the identical five instructions
but with `r9` instead of `r5` and at `0x1304d0`.  The positive-OR spelling
(`if (A || B || C) X=1; else X = field_0x14 != 0;`) does move it into the cold
tail, but costs 4 bytes (9904) and 1.7 points (72.2 %).

## 4. New: the planet-loop body is 30 bytes shorter in ours

Region `[getStations .. top-level getAutoPilotTarget]` (planet loop body only):

```
original  0x12e3c2 .. 0x12e6f4  = 0x332 (818 B)
ours      0x12e3c4 .. 0x12e6d8  = 0x314 (788 B)   -0x1e
```

Instruction-level causes (all register-allocation / block-placement, not
missing statements):

1. **`stations` lives in `sl` in the original, in `r6` in ours.**
   ```
   orig: mov sl,r0 ; mov.w fp,#0 ; movs r0,#0 ; str.w sl,[sp,#116] ; str r0,[sp,#152]
   ours: mov r6,r0 ; movs r0,#0 ; mov.w fp,#0 ; mov sl,r6        ; str r0,[sp,#148]
   ```
   Ours needs one extra `mov` because the `getStations()` result is parked in
   `r6` before being moved to `sl`.  Every later `stations->data()[i]` then
   reads `[r6,#4]` instead of `[sl,#4]`.
2. **`!otherLock && !planetHit && !turretMode` uses `ldrd` in ours.**
   ```
   orig: ldr r0,[sp,#144] ; ldr r1,[sp,#148] ; orrs r0,r1 ; ldr r1,[sp,#152] ; orrs r0,r1
   ours: ldrd r0,r1,[sp,#144]                ; orrs r0,r1 ; ldr r1,[sp,#152] ; orrs r0,r1
   ```
   because in ours `planetHit` is at `[sp,#148]`, adjacent to `otherLock`
   (`[sp,#144]`); in the original `planetHit` is at `[sp,#152]` (not adjacent,
   `turretMode` is at `#148`).  This is the stack-slot rotation of w4406 §4,
   restated: original `turretMode=#148, planetHit=#152`; ours
   `planetHit=#148, turretMode=#152`.
3. **The loop latch is inline in the original, out of line in ours.**
   ```
   orig: 12e4ea beq.n 12e4f2 ; 12e4ec add.w fp,fp,#1 ; 12e4f0 b.n loop ; 12e4f2 <planet-hit block>
   ours: 12e4e8 bne.w 12e5f8 ; 12e4ec <planet-hit block>              ; 12e5f8 add.w fp,fp,#1 ; b.n loop
   ```
   i.e. the original falls through the loop latch into the planet-hit block;
   ours branches out to a shared latch placed after the planet-hit block.

All three are MachineBlockPlacement / StackSlotColoring artifacts.  §6 records
the source-shape sweeps that failed to move them.

## 5. `v_dup_end` reproduces the instruction *count* exactly

Moving the duplicate `if (!alienOrbit)` block to the end of the campaign block
(after the planet-lock tail) gives

```
usize 9948  ratio 70.2 %  normalized stream 3492 / 3492   (exact count match)
```

i.e. the instruction *sequence length* becomes exactly the original's, but the
block order is wrong (45-instruction insert at the loop exit) and the body is
40 B too big.  It is the closest purely-structural match found so far and is
the natural base for a future pass that can shave 40 B.

## 6. Variant sweep (official verifier, `autojunk=False`; baseline 9908 / 73.9255)

All rows are the landed `Radar.cpp` plus the listed edit(s).

### 6.1 Declaration / type / spelling (all neutral unless noted)

| variant | usize | ratio |
|---|---:|---:|
| `int marked` -> `bool marked` | 9928 | 70.9 |
| `bool isTarget` -> `int isTarget` | 9908 | 73.9 |
| `planetHit` declared before `stations` | 9908 | 73.9 |
| planet loop `uint32_t i` -> `int i` (2 spellings) | 9908 | 73.9 |
| `stationId`/`currentStation`/`warpGate` decl order (3) | 9908/9904 | 73.9/70.2 |
| `int offset = ... ? ... : ...` -> if/else | 9908 | 73.9 |
| `notOtherLock`/`notTurretMode` -> `bool` (2) | 9900 | 72.0/70.6 |
| landmark skip-test reorders (2) | 9864/9860 | 65.9 |
| `data()[i] == nullptr` -> `!data()[i]` | 9908 | 73.9 |
| `!onScreen` -> `onScreen == 0` | 9908 | 73.9 |
| `alwaysEnemySeen` -> `bool` | 9908 | 71.5 |
| `alienOrbit` -> `int` | 9904 | 72.2 |
| mission `getType()` constant order (2) | 9908 | 73.8 |
| `missionBlocksPlanets` -> `int` | 9904 | 72.7 |
| `planetHit` moved to function scope (6 positions) | 9908 | 73.9 |
| `else if (field_0x14 == 0)` -> `!field_0x14` | 9908 | 73.9 |
| `if (field_0x14 != 0)` -> `if (field_0x14)` | 9908 | 73.9 |
| `field_0x18 != 0` -> `field_0x18` | 9908 | 73.9 |
| `lockedStation/lockedAsteroid/lockedGasCloud/lockedEnemy != nullptr` -> bare pointer (4) | 9908 | 73.9 |
| `candidateWreck` -> `int` | 9908 | 73.9 |
| planet-hit `planetDockIndex`/`field_0x18` store order (2, Ghidra order) | 9908 | 73.9 |
| landmark loop `int i` -> `uint32_t i` | 9916 | 72.1 |
| `(i & 0x7fffffff) == 1 \|\| ... == 3` -> `i == 1 \|\| i == 3` | 9908 | 73.9 |

### 6.2 `__builtin_expect` anchors

| variant | usize | ratio |
|---|---:|---:|
| `expect(onScreen, 1)` -> `expect(onScreen, 0)` | 9860 | 65.4 |
| `expect(onScreen, 1)` -> plain `if (onScreen)` | 9896 | 71.0 |
| `expect((i&0x7fffffff)==1\|\|...==3, 0)` -> `,1` | 9908 | 73.9 |
| -> plain (2 spellings) | 9912/9908 | 72.1/73.9 |
| first `otherLock` `expect(cond, 0)` -> `expect(cond, 1)` | 9908 | 73.9 |
| -> `expect(!(A\|\|B\|\|C), 0)` | 9908 | 73.9 |
| -> `expect(!(A\|\|B\|\|C), 1)` | 9912 | 70.8 |
| -> `expect(A\|\|B\|\|C, 0)` (positive OR + expect) | 9912 | 70.8 |

### 6.3 Enemy in-sight test (the `vpush {d8-d13}` lever)

| variant | usize | ratio |
|---|---:|---:|
| baseline (screen-first, interleaved, `int cx/cy`) | 9908 | 73.9 |
| `bool inSight` declared before `int cx/cy` | 9908 | 73.9 |
| direct `bool inSight = <expr>` | 9908 | 73.9 |
| `float crossX/crossY` then `int cx/cy` | 9908 | 73.9 |
| nested `if (bounds) { if (VectorLength < 60000) ... }` | 9908 | 73.9 |
| local `int size = field_0x124` | 9908 | 73.9 |
| distance-first, x-pair/y-pair (original shape) | 9924 | 71.2 |
| screen-first, x-pair/y-pair | 9936 | 70.7 |

### 6.4 Asteroid dead predicate

| variant | usize | ratio |
|---|---:|---:|
| baseline (`bool dead; if (dead && !hasCargo)`) | 9908 | **73.9255** |
| `if (!hasCargo && dead)` | 9908 | 73.7717 |
| `if (!hasCargo && (isDead()\|\|isDying()))` | 9912 | 72.0 |
| `!(hasCargo && dead) && dead` (original 4-call shape) | 9928 | 71.2 |

### 6.5 Combinations

| variant | usize | ratio |
|---|---:|---:|
| `!hasCargo && dead` + positive-OR `otherLock` | 9904 | 72.1 |
| `!hasCargo && dead` + explicit if/else 2nd `otherLock` | 9916 | 71.9 |
| `!hasCargo && dead` + `i != 3` first | 9908 | 73.6 |
| `!hasCargo && dead` + all three | 9908 | 72.1 |
| positive-OR `otherLock` + `v_dup_end` | 9904 | 72.2 |
| duplicate block made identical to the top-level block (tail-merged) | 9888 | 65.6 |
| duplicate block without `goingToPlanet`/`isDockingToPlanet` | 9880 | 66.2 |

Nothing reaches 73.9255 at 9908 bytes.  Note the last two rows: making the
duplicate block *identical* to the top-level one lets the tail merger collapse
them (call count drops by 7), which is semantically right but re-rolls the
layout and loses 20-28 bytes.

## 7. Conclusion

* The statement set is settled.  The duplicate campaign autopilot block is
  provably extra (one `strb [this,#0x1a8]` in the original, two in ours).
* The remaining residual is entirely block placement + stack-slot colouring:
  1. the loop-exit target (planet-lock tail vs duplicate block),
  2. `otherLock#1`'s cold half at `0x2480` (ours) vs `0xb72` (orig),
  3. `planetHit`/`turretMode` swapped between `[sp,#148]`/`[sp,#152]`
     (which is what turns two `ldr` into one `ldrd`),
  4. `stations` parked in `r6` instead of `sl` (+1 `mov`),
  5. the loop latch inline vs out of line,
  6. `vpush {d8-d11}` vs `{d8-d13}` (needs the distance-first in-sight shape,
     which costs 16 B and 2.7 points).
* The landed baseline is a strong local optimum: ~60 variants across two
  passes (w4406 §7 + this note) leave it at 9908 / 73.9255.

### What remains

1. A source shape that removes the duplicate block **and** keeps the body at
   9908 B.  `v_dup_end` already reproduces the exact instruction count
   (3492/3492) at 9948 B, so the target is "shave 40 B from `v_dup_end`
   without re-rolling the layout".
2. A source shape that makes `getStations()` land directly in `sl`
   (i.e. one fewer live value at the campaign-block preamble).  The
   `planetHit`/`stations` initialisation order was swept without effect.
3. A source shape that puts `planetHit` in `[sp,#152]` (after `turretMode` in
   `[sp,#148]`).  Declaration position and type do not move it.

## 8. Evidence / scratch (all under `/dev/shm/w4506`, not committed)

* `ghidra_radar.txt` — full 1673-line headless Ghidra decompilation of
  `Radar::draw` (function body file VA `0x12e050..0x130704`); `DecompRadar6.java`
  is the script.
* `measure.py` / `measure2.py` — official-verifier ratio (1-decimal / raw).
* `diff.py`, `bigdiff.py`, `region.py`, `eqmap2.py` — normalized-stream,
  largest-diff-block, per-region and per-0x100-byte-bucket diff tools.
* `sweep.py`, `spec1.json` … `spec13.json` — the variant sweeps in §6.
* `try.sh`, `restore.sh`, `Radar.base.cpp` — build/measure/restore harness and
  the pristine baseline source.
* `orig.txt`, `ours.txt` — full objdump of both bodies.
