# Radar::draw pass (wave111, w5905) — cold-block placement and first-chain codegen sweep

**Status:** research note, 2026-11-05. Worktree `wt/w5905`, branch
`wave111-w5905`, base `8eea060cc`. No source change landed; this commit contains
this note only. The target remains the pristine baseline.

Target `_ZN5Radar4drawEP6PlayerP3Hudi` in `src/game/weapons/Radar.cpp`
(the prompt's `src/game/ui/Radar.cpp` path is stale).

## 0. Baseline and gate

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
```

```
_ZN5Radar4drawEP6PlayerP3Hudi: osize 9908 usize 9908 byte False linked False
  raw 74.029509 %  norm 3492/3489
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.92
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS   (lint CLEAN, drift 0)
```

Per-variant scores use the official verifier API
(`/dev/shm/w5905/measure.py`, `difflib.SequenceMatcher(None, on, un,
autojunk=False).ratio()` at six decimals). The rounded report score stays 74.0
for everything in 73.95..74.04.

## 1. The unresolved first `otherLock` chain

Original `0x2b4..0x2c2` (function-relative):

```
2b4 cbnz r0, 0x2c2       ; lockedAsteroid != null
2b6 ldr  r0, [r4,#56]    ; lockedGasCloud
2b8 cbnz r0, 0x2c2
2ba ldr  r0, [r4,#28]    ; field_0x1c
2bc cmp  r0, #0
2be beq.w 0xb72          ; all null -> cold block 1
2c2 movs r5, #1          ; otherLock = 1 in r5
```

Original cold block 1 at `0xb72`:

```
b72 ldr  r5, [r4,#20]    ; field_0x14
b74 cmp  r5, #0
b76 it   ne
b78 movne r5, #1
b7a b.w  0x2c4
```

Baseline source is the negative-AND form:

```cpp
int otherLock;
if (__builtin_expect(this->lockedAsteroid == nullptr &&
                     __builtin_expect(this->lockedGasCloud == nullptr, 1) &&
                     this->field_0x1c == nullptr, 0)) {
    otherLock = this->field_0x14 != 0;
} else {
    otherLock = 1;
}
```

It produces the split/if-converted form instead:

```
2b4 cbz  r0, 0x2d4
2b6 mov.w r9, #1          ; otherLock in r9
2ba ldr  r5, [sp,#156]    ; status in r5
2bc b.n  0x2e8
...
2d4 ldr  r0, [r4,#56]
2d6 ldr  r5, [sp,#156]
2d8 cmp  r0, #0
2da itt  eq
2dc ldreq r0, [r4,#28]
2de cmpeq r0, #0
2e0 beq.w 0xb3c
2e4 mov.w r9, #1
```

The original has `otherLock` in `r5` and the status pointer in `r6`; the
baseline has `otherLock` in `r9` and status in `r5`. The cold block is at
`0x25aa` (end of function) instead of `0xb72`. The second `otherLock` chain at
`0x326..0x33a` already has the original `cbnz` shape, so the difference is
context/register colouring, not a missing statement.

## 2. First-chain expression sweep (new this wave)

All variants replace only the first `otherLock` block on the pristine baseline.

| first-chain spelling | usize | raw % | first chain |
|---|---:|---:|---|
| landed nested-expect negative-AND | 9908 | **74.029509** | `cbz`, r9 |
| plain negative-AND | 9908 | 72.797593 | `cbz`, r9 |
| positive OR | 9908 | 72.797593 | `cbz`, r9 |
| `int o = a||b||c; if (o) o=1; else o=(d!=0)` | 9908 | 72.797593 | `cbz`, r9 |
| `int o = a||b||c; o = o ? 1 : (d!=0)` | 9908 | 72.797593 | `cbz`, r9 |
| `int o = a||b||c||d` | 9904 | 70.549419 | `cmp/itt`, r5 |
| `int o = (a||b||c) ? 1 : (d!=0)` | 9908 | 72.797593 | `cbz`, r9 |
| `int o = !(a==0 && b==0 && c==0 && d==0)` | 9904 | 72.206304 | `cbz`, r9 |
| `if (a==0 && b==0 && c==0 && d==0) o=0; else o=1` | 9888 | 73.243476 | `cbnz` at 0x2c8 |
| same with `__builtin_expect(...,1)` | 9908 | 73.868195 | `cmp/bne` |
| `int o = a||b||c; o |= d` | 9892 | 65.415473 | `cbnz` at 0x2b4 |
| `int o = a||b||c; if (d) o=1` | 9892 | 65.415473 | `cbnz` at 0x2b4 |
| nested ternary `a? (b? (c? d : 1) : 1) : 1` | 9908 | 72.797593 | `cbz`, r9 |
| sequential `if (!o) o = ...` (4 terms) | 9904 | 70.549419 | `cmp/itt`, r5 |
| lambda returning bool OR4 | 9904 | 70.549419 | `cmp/itt`, r5 |
| static inline helper returning bool OR4 | 9904 | 70.549419 | `cmp/itt`, r5 |
| static inline helper returning int OR4 | 9908 | 72.797593 | `cbz`, r9 |

The `cbnz` shape is reachable (`o |= d`, `if (d) o=1`) but it evaluates
`field_0x14` early and/or targets `0x2ba`, not the original `0x2c2`; the score
falls to 65.4 %. `int o = a||b||c||d` is the only simple form that puts
`otherLock` in `r5`, but it uses `cmp/itt`, is 4 B short, and moves cold block 1
to `0x674` (70.55 %).

## 3. `__builtin_expect` placement on the first chain

A 20-form placement sweep (hints on individual terms, pairs, whole condition,
and swapped then/else) produced no improvement. Best entries:

| hints | usize | raw % |
|---|---:|---:|
| landed `(a=none, b=1, c=none, whole=0)` | 9908 | **74.029509** |
| whole `0` only | 9908 | 74.000859 |
| `c=0`, whole `0` | 9908 | 74.000859 |
| `b=0`, whole `0` | 9912 | 71.985105 |
| whole `1` | 9912 | 70.915877 |
| `a=1` | 9904 | 71.680275 |

The full 256-combination sweep was already recorded in
`radar-draw-wave110-w5806.md`; this wave's placement sweep confirms the same
maximum.

## 4. Type / declaration-order sweep

Baseline types are `int otherLock`, `bool landmarkHit`, `int notOtherLock`,
`int notTurretMode`. A 16-way int/bool matrix found no better combination:

| otherLock / landmarkHit / notOtherLock / notTurretMode | usize | raw % |
|---|---:|---:|
| int / bool / int / int (baseline) | 9908 | **74.029509** |
| int / bool / int / bool | 9900 | 72.180020 |
| int / bool / bool / int | 9892 | 70.203613 |
| int / bool / bool / bool | 9892 | 70.203613 |
| bool / bool / int / int | 9852 | 67.097701 |
| bool / bool / bool / bool | 9824 | 67.645366 |

Other one-at-a-time type changes:

| change | usize | raw % |
|---|---:|---:|
| `bool onPlanetCourse` -> `int` | 9908 | 74.029509 |
| `int campaign` -> `unsigned int` | 9908 | 74.029509 |
| `int alwaysEnemySeen` -> `unsigned int` | 9908 | 74.029509 |
| `bool alienOrbit` -> `int` | 9908 | 74.018906 |
| `bool gasHit` -> `int` | 9908 | 74.000859 |
| `bool inSight` -> `int` (enemy block) | 9908 | 74.029509 |
| `bool navWreck` -> `int` | 9908 | 74.029509 |
| `bool hasDockNav` -> `int` | 9908 | 74.029509 |
| `bool dead` -> `int` | 9908 | 74.029509 |
| `bool isTarget` -> `int` | 9908 | 74.029509 |
| `int marked` -> `bool` | 9928 | 71.023216 |
| `bool planetHit` -> `int` | 9896 | 71.492045 |
| `bool topOk` -> `int` | 9908 | 73.886263 |
| `bool bossSeen` -> `int` | 9904 | 72.169676 |
| `bool enemyHit` -> `int` | 9924 | 70.072993 |
| `bool wreck` -> `int` | 9916 | 72.011453 |
| `bool turretMode` -> `int` | 9888 | 66.169511 |

Declaration-order variants (move `landmarkHit` before the chain, declare then
assign, initialize `otherLock` to 0/1, move `notOtherLock` immediately after the
chain) were all 9908 B / 74.029509 % except moving both `not*` locals out
(73.972210 %) and moving `notOtherLock` out as `bool` (9900 B / 72.180020 %).

## 5. Second-chain, guard, and helper variants

The second `otherLock` assignment
(`a || b || c || lockedStation`) already matches the original `cbnz` shape.
Spellings tried (ternary, `!!`, if/else, `if`-only, expect 0/1, negative-AND,
sequential `if (!o)`) were neutral at 9908 B / 74.029509 % or worse; the best
non-baseline was `if`-only / `expect(...,1)` at 9912 B / 72.021764 %.

Guard restructuring (nested `if (landmarks)`, positive AND terms, positive OR
terms) was neutral at 9908 B / 74.029509 % or worse (`and_terms2` 9912 B /
64.184448 %). `__builtin_expect` on `landmarks != nullptr` or on the whole guard
dropped to 56–71 %.

A local `Status *status = Globals::status` scoped to the guard was 9912 B /
72.032078 %; moving it before the first chain was 9900 B / 72.141015 %.
Lambdas / static inline helpers for the first chain were 9904–9908 B and
70.5–72.8 %.

## 6. `int otherLock = a||b||c||d` combinations

This is the only simple form that puts `otherLock` in `r5`, so it was combined
with neutral changes and with the `tail_before_dup` block-placement variant:

| variant | usize | raw % |
|---|---:|---:|
| `int o = a||b||c||d` | 9904 | 70.549419 |
| + `bool onPlanetCourse` -> `int` | 9904 | 70.549419 |
| + `int campaign` -> `unsigned` | 9904 | 70.549419 |
| + all neutral type changes | 9904 | 70.549419 |
| + `int marked` -> `bool` | 9908 | 65.634409 |
| + `bool planetHit` -> `int` | 9892 | 69.977038 |
| + `bool topOk` -> `int` | 9928 | 70.515759 |
| + `tail_before_dup` | 9928 | 70.758640 |

The `tail_before_dup` variant moves the planet tail to `0x6fa` (original
`0x6f8`) but leaves the duplicate post-loop `getAutoPilotTarget` recompute
between the tail and the landmark body; it scores 70.29–70.76 %, not above the
baseline.

## 7. On-screen predicate hint sweeps

The full 3^5 = 243 hint sweep on the landmark on-screen predicate was repeated
on top of `int o = a||b||c||d`; the best was still the unhinted
`a||b||c||d` at 9904 B / 70.549419 %. A 3^3 = 27 sweep on the plain negative-AND
first chain also peaked at the landed hint pattern, 9908 B / 72.797593 %.

## 8. Conclusion

No measured variant beat the pristine baseline **9908 B / 74.029509 % /
3492/3489**. The original first-chain `cbnz`/cold-block-1 shape is reachable
only in source forms that either evaluate `field_0x14` early, move the cold
block to `0x674`, or lose the `r5` colouring; the baseline's `cbz`/`itt` form
with cold block 1 at `0x25aa` remains the best fuzzy score. The remaining
difference is dominated by `MachineBlockPlacement` and register allocation, not
by a local statement spelling.

The worktree was restored to the pristine `Radar.cpp` before the final gate.
`check` is green: byte 4500, linked 4500, lint CLEAN, drift 0, data 843/843,
imports 207, `RATCHET PASS`.

## 9. Scratch

All scratch under `/dev/shm/w5905` (not committed): `Radar.base.cpp`,
`measure.py`, `measure.sh`, `trychain.sh`, `mkchain.py`, `orig.all`,
`ours_base.all`, `rea.json`, `ghidra.c`, and the sweep scripts
`sweep_chain.py`, `sweep_expect.py`, `sweep_types.py`, `sweep_types2.py`,
`sweep_order.py`, `sweep_notlocals*.py`, `sweep_second.py`, `sweep_guard*.py`,
`sweep_lambda.py`, `sweep_helper.py`, `sweep_or4_combo.py`,
`sweep_or4_use.py`, `sweep_or_else.py`, `sweep_onscreen_or4.py`,
`sweep_onscreen_plain_small.py`.
