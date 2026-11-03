# Radar::draw — planet-block tail / duplicate-autopilot analysis (w3824)

**Status:** research note, 2026-11-03. Worktree `wt/w3824`, branch `wave89-w3824`,
base `5710f6d0`. No source change landed: every candidate below is
codegen-neutral or lowers the fuzzy score. This commit contains this note only.

## Baseline

```
Radar::draw  osize 9908  usize 9908  pct 73.9  byte False  linked False
gate         byte 4498  linked 4498  size 4522  .text 0x1337c8  lint CLEAN  drift 0
```

`check` = RATCHET PASS. The function is size-exact and byte-inexact.

## 1. The planet-block tail is NOT missing

The wave-88 note suggested the duplicate `if (!alienOrbit)` autopilot block was
compensating for missing planet-block tail code. It is not: our source already
has the tail and it compiles to the same shape as the original.

Original tail (`0x12e748`), entered from the planet loop exit (`0x12e3dc bcs`):

```
12e748 ldr r0,[sp,#148]   ; turretMode
12e74a ldr r1,[sp,#108]   ; landmarkHit
12e74c ldr r5,[sp,#112]   ; Status*
12e74e orrs r0,r1
12e750 bne 12ec24         ; -> enemies:
12e754 ldr r0,[r4,#20]    ; field_0x14
12e756 cbz r0,12e768
12e758 ldr r1,[r4,#24]    ; field_0x18
12e75a cmp r0,r1
12e75c itt eq
12e75e moveq r0,#1
12e760 strbeq.w r0,[r4,#536]   ; lockConfirmed = 1
12e764 movs r0,#0
12e766 str r0,[r4,#20]         ; field_0x14 = 0
12e768 ldr r0,[sp,#152]   ; planetHit
12e76a lsls r0,r0,#31
12e76c beq 12e78a
12e76e ... getPlayer / isDockingToAsteroid / isDockingToDockingPoint ...
12e78a ldr r0,[r4,#20]
12e78c cmp r0,#0
12e78e beq.w 12ec1c      ; planetLockTimer=0; field_0x18=0
12e792 movs r0,#0
12e794 str.w r0,[r4,#416] ; planetLockTimer = 0
```

Ours (`0x12ebec`):

```
12ebec ldr r1,[sp,#108]   ; landmarkHit
12ebee ldr r0,[sp,#152]   ; turretMode
12ebf0 orrs r0,r1
12ebf2 bne.n 12ec36
12ebf4 ldr r0,[r4,#20]
...
12ec2e str.w r0,[r4,#416]
```

Instruction-for-instruction the same, only the stack-slot colours differ
(ours turretMode `[sp,#152]`, landmarkHit `[sp,#108]`; original turretMode
`[sp,#148]`, landmarkHit `[sp,#108]`). The tail is therefore already faithful;
"reconstruct the planet-block tail first" is a no-op.

## 2. The duplicate autopilot block is real, and the 168 B lives in the enemy block

Call counts: original **344**, baseline **351**, `nodup` **342**. Our source has
a second `if (!alienOrbit) { getAutoPilotTarget(); getStationEnumIndex(); ...
field_0x1a8 = ... }` after the planet loop (`0x12e72c..0x12e77e` plus the
second `strb field_0x1a8` at `0x12ebd4..0x12ebe8`); the original has exactly one
`getAutoPilotTarget` (`0x12e6f4`), one `getStationEnumIndex` (`0x12e72a`) and
one `strb field_0x1a8` (`0x12e1d0`). Deleting the duplicate block:

```
nodup  usize 9740  pct 62.3
```

so the duplicate is worth 168 B. The original's 168 B is not in the planet
block; the largest single contributor is the **enemy block's
`if (!isDockingToAsteroid() && !(...))` layout**:

```
callgap after isDockingToAsteroid:  original 11 insns, ours 46 insns   (delta -35)
```

Original (`0x12f3ac`): body is the fall-through, `bne.n 12f40e` skips to the
`otherLock` recompute. Ours (`0x12f3da`): `beq.w 12f888` jumps *to* the body
and the skip path (three `and #1` bool normalisations) is the fall-through.
This is a branch-weight/basic-block-placement difference, not a source
expression difference: rewriting the condition as nested `if`s, as an `&&`
chain, or as an early `goto` all canonicalise to the identical 9908/73.9 body.
Adding `__builtin_expect(..., 1)` flips the polarity but re-rolls the rest of
the function (9892/68.0).

Secondary contributors (each a real source-shape difference, none individually
worth the size change):

* **Landmark `if (i <= 1)` block.** Ours tail-merges the else-branch
  `DrawString` with the special-branch's final `DrawString`
  (`0x12e9e0 b 0x12eaf2`); the original emits two separate calls
  (`0x12e9e0` and `0x12eb30`). The two calls have different arguments
  (`screenX+offset`/`screenY+field_0x4` vs `screenX+originX`/`screenY+field_0x4*2`),
  so this is a pure tail-merge decision, not expressible in source without
  changing the call sites.
* **`(i & 0x7fffffff) == 1 || (i & 0x7fffffff) == 3`.** Original emits
  `bic r0,sl,#0x80000000; cmp r0,#1; beq; cmp r0,#3; bne`. Ours emits
  `orr r0,sl,#2; cmp r0,#3` (the mask is folded because our loop var is
  `int` and provably non-negative). Changing the landmark loop var to
  `uint32_t` produces `and #0x7fffffff; orr #2; cmp #3` (9916/72.1) or
  `orr; bic; cmp` (9912/72.1) — neither matches and both lower the score.
* **`otherLock` first computation.** Original is a `cbnz` chain
  (`0x12e304 cbnz; 0x12e308 cbnz; 0x12e30c cmp; 0x12e30e beq.w 0x12ebc2`);
  ours is `cbz` + `itt eq; ldreq; cmpeq; beq.w 0x1304d0` because of the
  `__builtin_expect(..., 0)` on the all-null test. Removing the expect
  (9904/72.2) or rewriting as `!= || != || !=` with expect-1 (9908/73.9,
  codegen-identical) does not improve the score.

## 3. The inSight reorder is correct but re-rolls the allocator

The original's enemy `inSight` (Ghidra decomp, `0x13ef50..0x13efc0`) is:

```c
enemy->field_0x73 = 0;
int cx = (int) PlayerEgo::crosshairPos.x;   // float load hoisted, vcvt deferred
int cy = (int) PlayerEgo::crosshairPos.y;
if (VectorLength(egoPos - targetPos) < 60000.0f &&
    cx - size < screenX && screenX < cx + size &&
    cy - size < screenY && screenY < cy + size) {
    inSight = true;
}
enemy->field_0x73 = inSight;
```

i.e. distance first, then x-pair, then y-pair. Our source evaluates the
rectangle first, x/y interleaved. The reorder matches the local block
(`vldr s26/s24` before `AEMath::operator-`, `vcvt` after, `vpush {d8-d13}`)
but costs +16 B and drops the whole-function score:

```
baseline (rect first, x/y interleaved, distance last)  9908  73.9
distance first, x-pair then y-pair                      9924  71.2
distance first, nested if                               9928  64.3
distance first, direct (int)crosshairPos.x              9948  62.2
```

The +16 B is the extra `vpush` pair (`d8-d13` vs `d8-d11`) plus the deferred
`vcvt`; the score drop is the allocator spilling `locked` to the stack where
the original keeps it in `r5`.

## 4. Variant sweep (all measured with the official verifier)

| variant | usize | pct |
|---|---:|---:|
| baseline | 9908 | **73.9** |
| nodup (delete duplicate autopilot block) | 9740 | 62.3 |
| nodup + insight | 9764 | 61.4 |
| insight (distance-first, x-pair/y-pair) | 9924 | 71.2 |
| insight2/insight3 (near bool first) | 9928 | 64.3 |
| insight + inline_stationid | 9920 | 71.2 |
| insight + simpdup | 9900 | 64.8 |
| insight + bool_otherlock | 9896 | 48.0 |
| insight + expect-1 on isDockingToAsteroid | 9888 | 66.7 |
| expect-1 on isDockingToAsteroid | 9892 | 68.0 |
| bool_otherlock | 9876 | 67.8 |
| otherlock_or (`!= || != || !=`) | 9876 | 67.8 |
| otherlock_or + expect-1 | 9908 | 73.9 |
| notOtherLock/notTurretMode as bool | 9900 | 72.0 |
| bool_alwaysenemy | 9908 | 71.5 |
| bool_marked | 9928 | 70.9 |
| inline_stationid | 9904 | 71.7 |
| simpdup (autopilot target only) | 9880 | 66.2 |
| move `alwaysEnemySeen = alwaysEnemy` after the block | 9904 | 72.1 |
| landmark loop var `uint32_t` | 9916 | 72.1 |
| landmark loop var `uint32_t`, no expect on `(i&0x7fffffff)` | 9912 | 72.1 |
| nested `if (!isDockingToAsteroid()) { if (!...) }` | 9908 | 73.9 |
| `&&`-chain form of the same condition | 9908 | 73.9 |
| early-`goto` form of the same condition | 9908 | 73.9 |

No variant beats 73.9 at 9908 B.

## 5. Root cause

The remaining 26% is **whole-function register allocation and basic-block
placement**, not a wrong constant, callee, field or missing body. The single
most important remaining root cause is the enemy block's
`if (!isDockingToAsteroid() && !(isDockingToDockingPoint() || turretMode ||
otherLock))`: the original lays the body out as the fall-through and branches
to the `otherLock` recompute; ours lays the skip path out as the fall-through
and branches to the body. That ~35-instruction shift cascades into the
allocator (the `locked` pointer and the crosshair x/y live ranges), which is
also why the correct `inSight` reorder cannot be landed alone. A pass that
makes the enemy-block body the fall-through *without* re-rolling the rest of
the function, and then removes the duplicate autopilot block, is the next
worthwhile target.
