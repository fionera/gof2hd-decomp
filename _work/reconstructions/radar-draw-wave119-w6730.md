# Radar::draw pass (wave119, w6730) — structural-reconstruction sweep

**Status:** research note, 2026-11-05. Worktree `wt/w6730`, branch
`wave119-w6730`, based on current `master`. **No source change landed**; this
commit contains this note only. The target remains the pristine baseline.

Target `_ZN5Radar4drawEP6PlayerP3Hudi` in `src/game/weapons/Radar.cpp`
(original 9908 B). Read with the earlier notes
`radar-draw-wave119-w6706.md`, `radar-draw-wave118-w6606.md` and
`radar-draw-wave117-w6506.md`; this wave repeats their conclusion after a
further structural sweep and records the measurements.

## 0. Baseline, gate and structural state

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
```

```
_ZN5Radar4drawEP6PlayerP3Hudi: osize 9908 usize 9908
official verify score 74.0   (exact difflib ratio 74.029509 %)
verify: byte 4503  linked 4503  stubs 0  missing 0  extra 0  avg 99.93
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS   (lint CLEAN, drift 0)
```

Instruction counts: **3492 original / 3489 ours**, 2584 positionally equal.
Structural state re-measured this wave:

* **27 sections** on both sides, exact file size **2,187,328 B**.
* `.text` is now **size- and base-exact at 0x1337c8** on both sides (the
  wave-118 note's `0x133808 vs 0x1337c8` 0x40 B surplus is gone from current
  master — nothing in this wave changed it).
* `.ARM.extab` **0x26290 (ours) vs 0x26298 (original)** — the known 8 B
  shortfall (`createShip` only; `Generator::createMission` is exact at 172 B).
* `.rodata` **0x41c27 vs 0x41b67** — the known 0xC0 B mission-case-table tail.
* `.ARM.exidx`, `.data`, `.bss`, `.got`, `.dynsym`, `.dynstr`, `.hash`,
  `.rel.plt`, `.rel.dyn` are all size- and base-exact.

Scores below are the official verifier API
(`difflib.SequenceMatcher(None, on, un, autojunk=False).ratio()` at six
decimals); a variant must beat **74.05 %** to show up as 74.1 in the report.

## 1. The two global obstacles (confirmed again)

The residual gap is almost entirely **register allocation and literal-pool
placement**, not missing statements. Two properties dominate:

1. **First `otherLock` chain.** Original (`0x2b4`):

   ```
   cbnz r0, set        ; lockedAsteroid != null
   ldr  r0,[r4,#56]    ; lockedGasCloud
   cbnz r0, set
   ldr  r0,[r4,#28]    ; field_0x1c
   cmp  r0,#0
   beq.w cold
   set: movs r5,#1     ; 2 B, otherLock in r5
   ```

   Baseline emits `cbz r0, rest` + `mov.w r9,#1` (4 B) + a branch, with the
   rest of the chain outlined *after a literal pool at 0x2c0*. The original's
   pool is at 0x2f0, so every subsequent `ldr rN,[pc,#imm]` immediate differs
   by 0x30 until the next island. The cause is the register split:
   **original `otherLock`=r5 / `status`=r6; baseline `otherLock`=r9 /
   `status`=r5.** Because `status` is live across the landmark block in the
   baseline, r5 is unavailable and the set block needs the 4 B `mov.w r9,#1`.

2. **Planet-tail `missionBlocksPlanets` check.** Original (`0x2436`):

   ```
   ldr  r0,[r4,#416]   ; planetLockTimer
   movs r1,#0
   cmp  r0,#1
   it   lt
   movlt r1,#1
   orrs r0,r5,r1       ; missionBlocksPlanets | (planetLockTimer < 1)
   bne  skip
   ```

   Baseline emits only `cmp r0,#1; blt skip`. Clang propagates the earlier
   `if (missionBlocksPlanets) { hudEvent(21); goto enemies; }` into the
   fall-through and drops the second check (a valid but different
   optimization). Spelling the condition as `!(missionBlocksPlanets ||
   planetLockTimer < 1)`, as `planetLockTimer >= 1 && !missionBlocksPlanets`,
   as `planetLockTimer > 0`, or through a local copy all canonicalise back to
   the baseline.

## 2. Variants measured this wave

All are single-edit changes on the pristine baseline unless noted. `usize`
is the emitted function size; `raw %` is the unrounded ratio; `n` is the
positionally-equal instruction count (of 3492).

### 2a. First `otherLock` chain spellings

| variant | usize | raw % | n |
|---|---:|---:|---:|
| **baseline** (nested negative AND + `expect(...,0)` outer, `expect(...,1)` inner) | 9908 | **74.029509** | 2584 |
| outer `expect` only, no inner | 9908 | 74.000859 | 2583 |
| `bool noLock = AND; if (expect(noLock,0))` | 9908 | 74.000859 | 2583 |
| De Morgan `expect(!(a\|\|b\|\|c),0)` | 9908 | 74.000859 | 2583 |
| `expect(a==null,0) && expect(b==null,0) && expect(c==null,0)` | 9924 | 60.789926 | 2124 |
| all three `expect(...,1)` | 9912 | 70.915877 | 2470 |
| outer `expect(...,1)` | 9912 | 70.915877 | 2470 |
| `expect(a==null,1) && expect(b==null,1) && c==null` | 9916 | 71.888873 | 2510 |
| `expect(a==null,0) && expect(b==null,1) && c==null` | 9908 | 71.891117 | 2509 |
| `expect(b==null && c==null, 1)` | 9908 | 74.029509 | 2584 (tie) |
| `expect(b==null,1) && expect(c==null,1)` | 9908 | 74.029509 | 2584 (tie) |
| nested `if (a==null) if (b==null) if (c==null)` | 9908 | 72.797593 | 2541 |
| nested two-level `if (a==null && b==null)` | 9908 | 72.797593 | 2541 |
| `int otherLock = 1; if (AND) otherLock = d != 0;` | 9908 | 72.797593 | 2541 |
| ternary `AND ? (d != 0) : 1` | 9908 | 72.797593 | 2541 |
| `bool noLock = AND; otherLock = noLock ? ... : 1` | 9908 | 72.797593 | 2541 |
| positive OR `if/else` | 9908 | 72.797593 | 2541 |
| `int otherLock = a != null \|\| b != null \|\| c != null \|\| d != 0` | 9904 | 70.549419 | 2459 |
| `expect(a==null,1) && b==null && c==null` (no outer) | 9904 | 71.680275 | 2502 |

The baseline's particular pairing (outer `expect(...,0)`, inner
`expect(lockedGasCloud==nullptr,1)`) is worth exactly one instruction over
every other spelling. `BL1` of wave-117 (padding the set block) remains the
only form that reproduces the original `cbnz` chain, and it scores 72.08.

### 2b. Recompute-block res spellings (the tuned anchor)

| variant | usize | raw % | n |
|---|---:|---:|---:|
| **baseline** full 10-call recompute | 9908 | **74.029509** | 2584 |
| cache `Globals::status` in a local `st` | 9912 | 72.203924 | 2521 |
| use `this->planetTargets` for the target array | 9908 | 74.029509 | 2584 (tie) |
| store directly to `field_0x1a8` (no local) | 9908 | 74.029509 | 2584 (tie) |
| hoist `goingToPlanet`/`isDockingToPlanet` into locals | 9920 | 72.477458 | 2532 |
| cache `Array<int>* targets` | 9900 | 70.654206 | 2457 |
| single `getSystem()` instead of two | 9916 | 72.490334 | 2531 |
| swap `getSystem()` before `getAutoPilotTarget()` | 9920 | 72.250859 | 2523 |
| duplicate `getAutoPilotTarget()` call | 9924 | 72.131617 | 2521 |
| only `getAutoPilotTarget() == targets[planetDockIndex]` | 9788 | 62.252993 | 2158 |
| only `goingToPlanet() && !isDockingToPlanet()` | 9840 | 71.143063 | 2474 |
| only `field_0x1a8 = onPlanetCourse` | 9760 | 62.160601 | 2152 |

The full 10-call block is load-bearing: every reduction loses 2-12 points.

### 2c. Landmark `i <= 1` DrawString un-merge

| variant | usize | raw % | n |
|---|---:|---:|---:|
| **baseline** (tail-merged call) | 9908 | **74.029509** | 2584 |
| `String label` in the generic else arm | 9896 | 72.646511 | 2535 |
| `String label` in the `i==0` arm | 9912 | 70.485465 | 2461 |
| `String label` in both arms | 9884 | 66.542431 | 2321 |
| local `int x`/`int y` in the else arm | 9908 | 74.029509 | 2584 (tie) |
| local `int x` only / `int y` only | 9908 | 74.029509 | 2584 (tie) |
| `String label` + local x/y in the else arm | 9912 | 72.685583 | 2536 |

The local x/y forms are optimised away; only a real `String` copy separates
the two `blx DrawString` sites, and its ctor/dtor traffic costs more than the
32→7 hunk gains.

### 2d. Enemy `inSight` / `vpush {d8-d13}`

| variant | usize | raw % | n |
|---|---:|---:|---:|
| **baseline** (`vpush {d8-d11}`) | 9908 | **74.029509** | 2584 |
| `VectorLength` first, `x>,y>,x<,y<` order | 9948 | 63.375431 | 2208 |
| `VectorLength` first + `int size` | 9920 | 62.661684 | 2180 |
| `float len = VectorLength(...)` local | 9936 | 62.415625 | 2173 |
| `float fcx/fcy` then `int cx/cy` | 9908 | 74.029509 | 2584 (tie) |
| `Vector cross = crosshairPos` then `int cx/cy` | 9908 | 74.029509 | 2584 (tie) |

The original's `vpush {d8-d13}` (s24/s26 hold the int-converted crosshair
coordinates across `VectorLength`) is reachable only by the distance-first
order, which re-colours the whole function and costs ~10 points.

### 2e. Asteroid duplicated `dead` predicate

| variant | usize | raw % | n |
|---|---:|---:|---:|
| **baseline** (`bool dead = ...; if (dead && !hasCargo)`) | 9908 | **74.029509** | 2584 |
| `if (hasCargo && dead) {} else if (dead) continue;` | 9928 | 71.465369 | 2497 |
| `if (!(hasCargo && dead) && dead) continue;` | 9928 | 71.465369 | 2497 |
| `bool dead; if (dead) { if (!hasCargo) continue; } else if (dead) continue;` | 9928 | 72.113321 | 2520 |

### 2f. Booleans, declarations, misc

| variant | usize | raw % | n |
|---|---:|---:|---:|
| **baseline** | 9908 | **74.029509** | 2584 |
| all `bool` locals in `draw` → `int` | 9912 | 67.880178 | 2368 |
| `int missionBlocksPlanets` | 9904 | 72.815812 | 2542 |
| `int m = mode;` + use `m` | 9908 | 74.029509 | 2584 (tie) |
| `Status *status = Globals::status;` before the landmark guard | 9912 | 72.032078 | 2515 |
| unused `Status *status = Globals::status;` | 9908 | 74.029509 | 2584 (tie) |
| cache `Globals::status` in a local for the whole function | 10176 | 44.760563 | 1589 |
| move `bool landmarkHit` to the top | 9908 | 74.029509 | 2584 (tie) |
| move `bool bossSeen`/`bool enemyHit` to the top | 9880 | 69.182209 | 2411 |
| planet-tail condition `planetLockTimer >= 1 && !missionBlocksPlanets` | 9912 | 72.547616 | 2533 |
| planet-tail condition via a local `bool blocks` | 9908 | 74.029509 | 2584 (tie) |
| planet-tail `planetLockTimer > 0` | 9908 | 74.029509 | 2584 (tie) |
| planet-tail De Morgan `!(missionBlocksPlanets \|\| planetLockTimer < 1)` | 9908 | 74.029509 | 2584 (tie) |

## 3. Enemy-loop register split (measured, not yet actionable)

The enemy loop shows the same global re-colouring:

```
original                          baseline
0xc80 mov.w sl,#68                0xc9e mov.w sl,#68
0xc88 mov   fp,#0     ; i         0xca6 movs  r5,#0   ; i
0xc90 movs  r6,#0     ; bossSeen  0xca6/0xd1a bossSeen -> [sp,#108]
0xd16 wreck -> [sp,#152]          0xd26 wreck -> r6
0xcaa i in fp, bossSeen in r6     i in r5, bossSeen spilled
```

The original's `i`=fp / `bossSeen`=r6 / `wreck`=spill is the mirror image of
the baseline's `i`=r5 / `bossSeen`=spill / `wreck`=r6. Fixing it requires
matching the whole function's allocation, not a local edit.

## 4. Conclusion

No measured variant beat the pristine baseline **9908 B / 74.029509 % /
2584 positional**. This wave confirms the wave-115/116/117/118/119 finding:
the baseline is a strong local optimum. The invented post-planet-loop
`onPlanetCourse` recompute is wrong by the call graph (the original has one
copy of the autopilot comparison, outlined from the prologue to `0x69e`) but
it is exactly what keeps the baseline's global register allocation and the
landmark-loop placement (`0x760` vs original `0x784`) close enough to score
74 %.

The remaining levers, in order of expected value, are the two global
properties of §1: `otherLock`/`status` in `r5`/`r6` (which would also make
the first chain a 2-byte `movs r5,#1` and move the first literal pool from
`0x2c0` to `0x2f0`), and the planet-tail `missionBlocksPlanets` check that
clang now propagates away. Both are allocator/scheduler artefacts of the
exact original source shape, not local statements.

## 5. Scratch

All scratch under `/dev/shm/w6730` (not committed): `Radar.base.cpp`
(pristine baseline source), `score.py` (official verifier API), `try.sh`,
`diff2.py` (opcode dump with addresses), `map.py`/`extract.py` (per-window
match map), plus the ~60 `v_*.cpp` variants listed above.
