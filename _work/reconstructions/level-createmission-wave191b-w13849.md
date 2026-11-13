# Level::createMission — wave 191b w13849

Target: `_ZN5Level13createMissionEv` (`0xaffd8`, 17,172 B). Base master `055321398`
(raw 3,373 / aligned 463 / size 17,172 exact / insn 6198/6197). `.rel.plt` stays
byte-identical (0 differing bytes).

## Landed: alien-turret table select anchor on top of w13841 W

The alien turret loop chooses between `kAlienStationTurrets` and `kAlienOutpostTurrets`
with a table pointer. The original emits a two-way branch on `bigAlienWave` only:

```
b32c6  ldr  r0, [sp, #64]        ; bigAlienWave
b32c8  cbz  r0, b32d0
b32ca  ldr  r1, [pc, #732]       ; station table
b32cc  add  r1, pc
b32ce  b.n  b32d4
b32d0  ldr  r1, [pc, #728]       ; outpost table
b32d2  add  r1, pc
```

HEAD's `- (i >> 31) * 4` selector materialises the sign bit and compares it, which is not
the original shape. Replacing it with the additive disjunction

```cpp
const float (*t)[6] = (bigAlienWave | (i < 0)) ? kAlienStationTurrets : kAlienOutpostTurrets;
```

makes clang emit a branchless select `orr.w r1, r1, sl, lsr #31; cmp r1, #0; it ne;
movne r5, r0`, which is one step closer to the original's `cbz` than the sign-bit
`and`/`cmp` form. `(bigAlienWave | (i >> 31))` produces identical codegen; `+ (i < 0)`
and `||` are worse (see table).

This is landed together with the three w13841 variant-W hunks (already harvested in
`c992c1370` on the sibling branch):

1. pirateFortress second rotate via a named `Vector q = ...; v = q;` (reused `sp+212` temp).
2. raid `guns = alienGuns + fortressGuns` computed *after* the `raidRace == 8` stream-out `if`.
3. case-4 hunter loop drops the dead `if (hunterCount == 0) hunterCount = 1;` guard (the
   loop bound reads `this->enemies->size()` directly, leaving the function 4 B short; the
   guard was only ever the size compensator).

Measured from the base: raw **3,373 -> 4,265**, aligned **463 -> 386** (-77),
size 17,172/17,172 exact, insn 6198/6197. `.text` differing bytes 75,770 -> 76,662;
`.ARM.extab` content 544 -> 594; `.ARM.exidx` 0; `.rodata` 0. The raw rise is
branch-offset/instruction-shift; the aligned drop is the structural progress (same
convention as the w13820/w13799 passes). Gate `check` green: byte_exact 4508,
linked_exact 4508, size 4524/4524, data_exact 843/843, imports 207, lint CLEAN,
parity 5370/5370.

## Experiments this pass (all reverted; no better win)

| variant | raw | aligned | size | note |
|---|---:|---:|---:|---|
| HEAD `055321398` | 3,373 | 463 | 17,172 ✓ | |
| **landed (W + `\| (i < 0)`)** | **4,265** | **386** | **17,172 ✓** | |
| table `\| (i >> 31)` | 4,265 | 386 | 17,172 ✓ | identical codegen |
| table `+ (i < 0)` | 4,489 | 468 | 17,172 ✓ | worse |
| table `\|\|` | 4,963 | 514 | 17,176 ✗ | size-fatal |
| table `((i<0)?1:bigAlienWave)` | 4,963 | 514 | 17,176 ✗ | size-fatal |
| extraAliens bound `pirates + start + extraAliens` | 4,295 | 464 | 17,160 ✗ | size-fatal |
| extraAliens bound `start + pirates + ...` | 4,265 | 386 | 17,172 ✓ | neutral |
| `int start = pirateEscorts + guns + stationPirates` | 4,265 | 386 | 17,172 ✓ | neutral |
| stationPirates `first = stationPirates + guns` | 4,265 | 386 | 17,172 ✓ | neutral |
| pirateBase declared after `first` | 4,265 | 386 | 17,172 ✓ | neutral |
| escort `const int tenK = 10000` | 4,265 | 386 | 17,172 ✓ | neutral |
| case-4 `__builtin_expect(roll == 0, 0)` | 3,412 | 467 | 17,172 ✓ | worse |
| swap case 4 / case 184 blocks | 4,265 | 386 | 17,172 ✓ | neutral |
| drop hunterCount guard alone | 3,547 | 455 | 17,168 ✗ | 4 B short |

## Remaining aligned clusters (landed)

Aligned residual 386. The dominant clusters are unchanged from w13841:
`b3752..b39a0` raid block (r4/r8 colouring swap), `b3274..b33a2` alien turret loop
(register colouring plus the remaining `i<0` sign-bit select vs the original `cbz`),
`b33da..b3512` pirateFortress body (index spill / vector temp slots), and
`b4152..b42c6` shared tail (this/i/globals colouring rotation). These are allocator
artefacts; the prologue is byte-identical through `0xb005e`, so no source-level alias,
naming, or statement-order change tried so far moves the colouring.
