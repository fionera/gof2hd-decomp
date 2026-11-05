# w6782: `Level::createMission` (92.293729 -> 92.293729 %, 17172 B) — wave-119
# table-select / branch-shape / frame-slot re-sweep; no source win

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w6782`, branch `wave119-w6782`,
based on current master. Ground truth:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; function `0xaffd8`-`0xb42ec`, 17172 B).

This continues `level-createmission-wave119-w6768.md`. The baseline is unchanged
and no variant beat it. New measurements and one new diagnostic (the 84/88 frame
slot swap) are recorded below. `src/game/world/Level.cpp` is unchanged; this note
is the only commit.

## Baseline (pristine HEAD)

```text
_ZN5Level13createMissionEv   official 92.293729 %   size 17172/17172 B
                             6055 orig / 6065 ours insns
gate: byte 4505/4524, linked 4505/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, RATCHET PASS
ELF: 27 sections, file 2,187,328 B, .ARM.extab 0x26290 (8 B short, createShip only).
```

## 1. New diagnostic: the 84/88 frame-slot swap (worth ~17 insns)

The first instruction of the function already differs:

```text
orig  b0024: str.w fp, [sp, #88]     ; fp = &__stack_chk_guard
ours  b0024: str.w fp, [sp, #84]
...
orig  b04c0: str   r0, [sp, #84]     ; raidRace (line 1581)
ours  b04c0: str   r0, [sp, #88]
```

Both slots are RA spill slots (4 bytes each); the stack-guard pointer and
`raidRace` are simply swapped.  The swap is visible on ~17 instructions
(`[sp,#84]`/`[sp,#88]` at source lines 1584, 1593, 1610, 1621, 1653, 1676, 1681,
1693, 1701, 1712, 1725, 1755, 1851, 1859 and the guard reload at `b02ca`).
A full `[sp,#N]` census of the two disassemblies shows every other offset is
identical in value (only frequencies differ); 84/88 is the only *value* swap.
The frame size (400 B) and the object count are unchanged, so this is a
spill-slot colouring/order difference, not a layout difference.

Perturbations tried to flip it (all scored the same or worse):

| variant | official % | size | insns |
|---|---:|---:|---:|
| **baseline** | **92.293729** | 17172 | 6065 |
| `unsigned raidRace` | 92.293729 | 17172 | 6065 |
| `int raidRace; if (...) ... else ...` | 92.293729 | 17172 | 6065 |
| `(rnd->nextInt(100) < 75) ? 8 : ...` (parens) | 92.293729 | 17172 | 6065 |
| `raidRace` initialiser moved after `raiders` | 85.403086 | 17188 | 6064 |

`-mllvm -print-after-all` frame-object dumps for the baseline were inspected;
the two objects are both 4-byte spill slots and their relative frame index is
not reachable from any source-level declaration order tried.

## 2. `*bigShipsCount > 0` (line 1837): `cmp #0; ble` vs `cmp #1; blt`

```text
orig  b0eac: cmp r0, #0
orig  b0eae: ble.n b0eec
ours  b0eac: cmp r0, #1
ours  b0eae: blt.n b0eec
```

Semantically identical (`x > 0` vs `x >= 1`); the original uses the canonical
compare-against-zero form.  `0 < *bigShipsCount`, `*bigShipsCount >= 1` and the
baseline spelling all produce the same `cmp #1; blt` in this context (a small
isolated `if (*p > 0 && r == 0)` does emit `cmp #0` + `movgt`, so the difference
is a backend branch-folding decision, not a front-end one).  Swapping the `&&`
operands (`localRace == 0 && *bigShipsCount > 0`) collapses the score to
74.190885 %, so the source order must stay as-is.

## 3. Case-183 convoy switch: all six case orders

The original comparison chain is descending (`cmp #2; beq; cmp #1; beq; cbnz`)
with the `k == 0` body as fallthrough and the block layout
`[case0][case1][case2][default][shared tail]`.  An explicit `switch (k)` with
cases written ascending reproduces the descending chain but lays the bodies out
`[case0][case2][case1][tail][default]`.  All six source permutations were
measured:

| switch case order | official % | size | insns |
|---|---:|---:|---:|
| **baseline if-chain 2,1,0** | **92.293729** | 17172 | 6065 |
| switch 2,0,1 | 92.293729 | 17172 | 6065 |
| switch 2,1,0 | 92.293729 | 17172 | 6065 |
| switch 1,0,2 | 92.161716 | 17172 | 6065 |
| switch 1,2,0 | 92.161716 | 17172 | 6065 |
| switch 0,1,2 | 92.145215 | 17172 | 6065 |
| switch 0,2,1 | 92.145215 | 17172 | 6065 |
| if-chain 0,1,2 | 92.145215 | 17172 | 6065 |

The ascending switch gets the comparison chain right but loses more on block
placement than it gains.  No source order produces the original's
`[0][1][2][default][tail]` layout.

## 4. Turret table select: the front-end emits `select`, not a diamond

`-emit-llvm` on the baseline shows the table choice is already a `select` in the
*first* IR dump (before any pass):

```llvm
%1753 = ashr i32 %1733, 31
%1754 = icmp eq i32 %1111, %1753
%1755 = select i1 %1754, [6 x float]* @kAlienOutpostTurrets, [6 x float]* @kAlienStationTurrets
```

so the original's in-loop `cbz` diamond cannot come from a C++ ternary.  Forms
tried this wave (all still fold to `select` in the final `.ll`, all score
92.293729 or worse):

- `if (bigAlienWave + (i < 0)) t = station; else t = outpost;` — **identical
  code to the baseline anchor** (92.293729, 17172, 6065).  The earlier note's
  "if/else = 88.38" applies only to the *un-anchored* form.
- `switch (bigAlienWave + (i < 0)) { case 0: outpost; default: station; }`
  — still a `select`.
- 3-way switch (0 / 1 / default) — still a `select`.
- 4-way switch — becomes a `switch.table` jump table (would add `.rodata`).

## 5. Turret loop: faithful cached-`gun` forms re-measured

Original body order is `A(createStaticObject) B(setRotation) C(rotate2)
D(setPosition) E(setScaling via gun) G(cargo) F(shipGroup)`.

| body | official % | size | insns |
|---|---:|---:|---:|
| **baseline `A Ef B C D F G`** | **92.293729** | 17172 | 6065 |
| `A B C D E G F` cached `gun` | 92.160423 | 17172 | 6063 |
| `A B C D E G F` fresh receiver | 92.227723 | 17172 | 6065 |
| `A Ef B C D G F` (cargo before shipGroup) | 92.277228 | 17172 | 6065 |
| `PlayerTurret *gun = (PlayerTurret *) (*enemies)[i]`, baseline order | 90.922594 | 17164 | 6063 |
| `PlayerTurret *gun`, faithful `A B C D E G F` | 92.160423 | 17172 | 6063 |
| pirateFortress loop `D E G F` instead of `D E F G` | 92.244224 | 17172 | 6065 |
| pirateFortress loop fresh setScaling receiver | 90.859594 | 17180 | 6067 |

The faithful cached-`gun` form still splits `this` at the loop header
(`ldr r5,[sp,#124]` in the preheader) and additionally spills `i`
(`str fp,[sp,#116]`), so it does not recover the original's
`this`=r4 / `gun`=r6 / `&v`=r5 allocation.

## 6. Loop form / declaration / index-order variants

| variant | official % | size | insns |
|---|---:|---:|---:|
| **baseline `for` loop** | **92.293729** | 17172 | 6065 |
| `while` loop (`i++; k++;` in body) | 92.293729 | 17172 | 6065 |
| `do { ... } while (...)` | 87.298836 | 17172 | 6062 |
| flat `const float *t` + `t[k*6+n]` | 92.293729 | 17172 | 6065 |
| `Vector v;` (default ctor) instead of `{0,0,0}` | 92.293729 | 17172 | 6065 |
| `int k` before `Vector base` | 92.293729 | 17172 | 6065 |
| `Vector base` before `Vector v` | 92.128713 | 17172 | 6065 |
| `(*enemies)[field_164 + friendCount]` index order | 92.260726 | 17172 | 6065 |
| loop bound `... + field_168 + alienGuns` order | 92.293729 | 17172 | 6065 |
| cache `Array<KIPlayer*> *enemies = this->enemies` | 90.807064 | 17164 | 6063 |
| separate `Vector r1/r2` rotate temporaries | 90.626807 | 17148 | 6054 |
| fresh `(*enemies)[i]->player->setMaxHitpoints(1000)` | 92.293729 | 17172 | 6065 |

## 7. `&&` at lines 1904 / 1924 (re-confirmed)

`bool isAlienStation = (i == X) && alienWave; if (isAlienStation) ...` and the
`& (alienWave != 0)` spelling are both optimized back to the baseline
(92.293729).  `alienWave & (i == X)` (integer `&`) drops to 75.689058
(17172 / 6063).  The original's `(i != X) | !alienWave` OR-of-negations form
remains unreachable from any `&&`/`&`/`bool` spelling tried.

## 8. Commands / scratch

```text
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w6782/score.py                 # official %, size, insns
python3 /dev/shm/w6782/blocks2.py               # difflib block census
python3 /dev/shm/w6782/sweep.py <variants.json> # apply/build/score/restore
```

Scratch under `/dev/shm/w6782` (Level.base.cpp = pristine HEAD copy,
objmap.json/idxline.json = instruction -> source-line map built by relinking
`Level.cpp.obj` unstripped and using `addr2line`).
