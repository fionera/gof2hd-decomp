# Level::createMission — wave 191b w13803 (pass 26)

Target: `_ZN5Level13createMissionEv` (`0xaffd8`, 17,172 B). Base `1ca813f1e`
(pass-25 state: raw 3,496 / aligned 471 / size 17,172 exact, `.text` 75,787,
`.ARM.extab` 590).  All variants below keep size 17,172 and `.rel.plt`
byte-identical.

## Landed: case-4 hunter loop reads `this->enemies` directly

The original tail loop reloads `this->enemies` on every iteration:

```
b41c8  ldr.w r0, [r8, #248]   ; this->enemies
b41cc  ldr   r0, [r0, #0]     ; ->size()
b41ce  cmp   r9, r0
b41d0  bcc.n b416e
```

Our source kept a local `Array<KIPlayer *> *enemies = this->enemies;` and looped
`i < enemies->size()`, which the compiler hoists into a register before the loop
(the pass-25 build even spills `this` to `[sp,#124]` and keeps `enemies` in
`r9`).  Dropping the local so the loop bound is `this->enemies->size()` matches
the original reload exactly (and moves the `hunterCount` spill from `[sp,#120]`
to `[sp,#124]`, as in the original `b4156`/`b41da`).

Removing the local alone leaves the function **4 B short** (17,168, `.ARM.exidx`
diff 1) because the pass-25 local was accidentally compensating for a 4-byte
deficit elsewhere in the body (the `if (raid)` block is still 16 B shorter than
the original).  A `if (hunterCount <= 0) hunterCount = 1;` guard before the loop
restores the 4 bytes with a real `cmp/it/movle` (6 B emitted, net +4 after the
2-byte loop-preamble difference).  The guard is dead code for the reachable
`hunterCount` range, but it is the only size-exact restore found; every other
candidate (case-4 branch forms, `guns` moved below the spawn block, extraAliens
accumulation order, tail `return;`, nameLeader forms, `n`/`const`/`volatile`
loop locals) either left the function short or regressed the aligned count.

Measured (from the pass-25 baseline):

| variant | raw | aligned | size | `.text` | extab | exidx |
|---|---:|---:|---:|---:|---:|---:|
| pass-25 baseline | 3,496 | 471 | 17,172 ✓ | 75,787 | 590 | 0 |
| **local dropped + `<= 0` guard (landed)** | **3,376** | **463** | 17,172 ✓ | **75,667** | **547** | 0 |
| local dropped only | 3,549 | 399 | 17,168 ✗ | 77,932 | 590 | 1 |
| local dropped + `== 0` guard | 3,375 | 463 | 17,172 ✓ | 75,666 | 547 | 0 |
| local dropped + `int n = size()` | 3,534 | 472 | 17,172 ✓ | 75,825 | 590 | 0 |
| body-local hybrid (`(*enemies)[i]` store) | 3,372 | 473 | 17,172 ✓ | 75,663 | 565 | 0 |
| `guns` moved below spawn block | 4,787 | 473 | 17,168 ✗ | 79,170 | 603 | 1 |
| local dropped + `guns` below spawn | 4,791 | 478 | 17,164 ✗ | 829,286 | 605 | 1,645 |

## Remaining aligned clusters (after pass 26)

Largest first (orig bytes / ours bytes / delta):

* `b3752..b39a0` raid block, 590/574 (+16) — original keeps `this` in `r8`,
  `i` in `r9`, and spills `&Globals::globals` to `[sp,#120]`; ours keeps `this`
  in `sl`, `i` in `r8`, `&Globals::globals` in `r9`.  `guns = alienGuns +
  fortressGuns` is computed *after* the `raidRace == 8` spawn block in the
  original (`b37e6`), before it in ours.
* `b4152..b42c6` tail, 372/378 (-6) — the guard's 6-byte `cmp/it/movle`.
* `b3274..b33a2` alien turret loop, 302/304 (-2).
* `b33da..b3512`, 312/310 (+2).
* `b3dd0..b3dfe` extraAliens loop, 46/54 (-8) — original init order
  `pirates + start + extraAliens + friendCount + field_164 + field_168 +
  hostileCount`, limit `start + extraAliens + pirates + ...`; original spills
  `extraAliens` to `[sp,#92]`, ours to `[sp,#104]`.
* `b3a5c..b3a98`, 60/64 (-4).
* `b19bc..b1a00` case-4 goto, 68/70 (-2) — ours hoists `ldr.w sl,[sp,#124]`
  before `cmp r0,#0`; the original loads `r8` only on the else path (`b19e0`)
  and again at `b19fc` on the tail path.
* `b3ba0..b3bb8`, 24/20 (+4); `b4092..b410a` nameLeader tail, 120/114 (+6);
  `b3c16..b3c32` pirate-escort `pos` order, 28/34 (-6).

The 4-byte body deficit that the guard papers over is most likely inside the
`if (raid)` block (16 B short), but no source reshaping of that block found so
far restores size without a larger aligned regression.
