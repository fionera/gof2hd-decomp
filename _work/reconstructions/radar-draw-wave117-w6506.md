# Radar::draw pass (wave117, w6506) — chain-shape / block-placement / anchor sweep

**Status:** research note, 2026-11-05. Worktree `wt/w6506`, branch
`wave117-w6506`, based on current `master`. **No source change landed**; this
commit contains this note only. The target remains the pristine baseline.

Target `_ZN5Radar4drawEP6PlayerP3Hudi` in `src/game/weapons/Radar.cpp`
(the prompt's `src/game/ui/Radar.cpp` path is stale). Original 9908 B.

## 0. Baseline and gate

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
```

```
_ZN5Radar4drawEP6PlayerP3Hudi: osize 9908 usize 9908 raw 74.029509 %
verify: byte 4501  linked 4501  stubs 0  missing 0  extra 0  avg 99.92
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS   (lint CLEAN, drift 0)
```

Scores below are the official verifier API
(`difflib.SequenceMatcher(None, on, un, autojunk=False).ratio()` at six
decimals). Baseline instruction count: **3492 original / 3489 ours**
(2584 matched). Structural state re-measured this wave and unchanged:
**27 sections** on both sides, exact file size **2,187,328 B**, `.text`
**0x1337c8** on both sides, `.ARM.extab` **0x26294 (ours) vs 0x26298
(original)** — the known 4 B shortfall; `.rodata` 0x41c27 vs 0x41b67.

## 1. What the baseline actually emits vs the original

Function-relative block order (offsets from `0x12e050`):

```
original                                baseline
2ac first otherLock chain               2ac first otherLock chain
2c4 landmark/station guard              2e8 landmark/station guard
326 second otherLock chain              33a second otherLock chain
338 planet guard                        33a planet guard
376 planet loop header                  33a planet loop header
69e cold autopilot (8 calls)            682 cold autopilot
6f8 planet tail (17 insns)              6dc onPlanetCourse recompute (10 calls, 33 insns)
758 landmark setup                      730 landmark setup
784 landmark loop header                760 landmark loop header
b42 landmark post-loop                  b10 landmark post-loop
bc2 cold blocks                         b8c cold blocks
bd4 enemies                             bbe recompute tail (7 insns)
                                        ba0 planet tail (17 insns)
                                        bea enemies
```

The only semantic surplus is the post-planet-loop `onPlanetCourse` recompute
(10 calls, ~33 instructions). It is **load-bearing for the fuzzy score**: it
occupies almost exactly the original's planet-tail slot and keeps the
landmark loop hot at `0x760` (original `0x784`). Deleting it (`V_nodup`,
9756 B) restores the exact original call multiset but lets
MachineBlockPlacement sink the landmark loop to `0x1474`; score 62.314026.

The largest remaining mismatch hunks (SequenceMatcher opcodes, baseline):

| orig insns | ours insns | region |
|---:|---:|---|
| 32 | 7 | landmark generic-else `DrawString` tail merge (`0x960`) |
| 28 | 8 | enemy in-sight block, part 1 |
| 0 | 33 | post-loop recompute |
| 5 | 28 | enemy in-sight block, part 2 |
| 25 | 8 | asteroid duplicated `dead` predicate |
| 23 | 4 | first `otherLock` chain (`cbz`/`itt` vs `cbnz`) |
| 1 | 26 | asteroid blip block placement |
| 1 | 23 | landmark post-loop block placement |
| 1 | 21 | landmark guard / literal-pool placement |
| 17 | 0 | planet tail at its expected slot |

The residual gap is almost entirely **block placement** (all intra-function
branch targets are compared literally as `loc_<offset>`), not missing
statements.

## 2. First `otherLock` chain — the original's `cbnz` shape is reachable

Original `0x2b4..0x2c4`:

```
2b4 cbnz r0, 2c2      ; lockedAsteroid != null -> set
2b6 ldr  r0,[r4,#56]  ; lockedGasCloud
2b8 cbnz r0, 2c2      ; != null -> set
2ba ldr  r0,[r4,#28]  ; field_0x1c
2bc cmp  r0,#0
2be beq.w cold        ; == 0 -> cold
2c2 movs r5,#1        ; shared set block (2 B)
2c4 ldr  r0,[r4,#312] ; landmarks
```

The baseline emits `cbz` + `itt eq` + a **duplicated** `mov.w r9,#1`
(4 B) set block; `otherLock` lives in r9 and `Globals::status` in r5,
whereas the original keeps `otherLock` in r5 and status in r6.

**New this wave:** enlarging the set block with a second statement makes
clang emit the original `cbnz` chain with one shared set block.  Adding
`this->field_0x2c = 1;` to the `else` branch gives

```
cbnz r0, set
ldr  r0,[r4,#56]
cbnz r0, set
ldr  r0,[r4,#28]
cmp  r0,#0
beq.w cold
set: mov.w r9,#1
     str.w r9,[r4,#44]   ; the padding statement
```

i.e. the correct control-flow shape, but the set block is 8 B (not 2 B),
`otherLock` is still r9, and the extra store costs more than the chain
alignment gains: **9896 B / 72.083692 %** (`BL1`).

## 3. Variants measured this wave

All are single-edit changes on the pristine baseline unless noted.
`usize` is the emitted function size; `raw %` is the official fuzzy score.

### 3a. First-chain source forms (all canonicalise to the baseline)

| variant | usize | raw % |
|---|---:|---:|
| baseline (nested negative AND + expects) | 9908 | **74.029509** |
| `P1` positive OR `if/else` | 9908 | 72.797593 |
| `P2` `int otherLock = 0;` + positive OR | 9908 | 72.797593 |
| `P3` ternary `? 1 : (d != 0)` | 9908 | 72.797593 |
| `P4` negative AND `if/else`, no expects | 9908 | 72.797593 |
| `Q1` `if/else if/else if/else` (three `=1` blocks) | 9908 | 72.797593 |
| `Q2` same, trailing newline | 9908 | 72.797593 |
| `Q3` `bool otherLock` + `if/else if/else if/else` | 9880 | 67.824340 |
| `Q4` `init 0`; three separate `if (x) otherLock=1;` | 9928 | 64.744693 |
| `Q5` fully nested `if (a==null) if (b==null) if (c==null)` | 9908 | 72.797593 |
| `R1` positive `goto setOther` chain | 9908 | 72.797593 |
| `R2` pointer-truth `goto` chain | 9908 | 72.797593 |
| `R3` `int otherLock = 1; if (AND) otherLock = d!=0;` | 9908 | 72.797593 |
| `R4` negative AND `if/else` | 9908 | 72.797593 |
| `R5` positive OR `if/else` | 9908 | 72.797593 |
| `R6` `bool anyLock = OR; if (anyLock) ...` | 9908 | 72.797593 |
| `BL1` baseline + `field_0x2c = 1` in set block | 9896 | 72.083692 |
| `BL2` baseline + `field_0x2c` set in both arms | 9916 | 71.896922 |
| `BL3` baseline + two stores in both arms | 9920 | 71.915259 |

### 3b. Landmark guard spelling

| variant | usize | raw % |
|---|---:|---:|
| baseline (`!(idx==109 || idx==110 || ...)`) | 9908 | **74.029509** |
| `G_A` positive `idx != 109 && idx != 110 && (idx != 111 \|\| campaign <= 93)` | 9912 | 64.184448 |
| `G_B` same, nested `if (landmarks != nullptr) { ... }` | 9912 | 64.184448 |
| `G_C` `!(idx==109) && !(idx==110) && !(idx==111 && ...)` | 9908 | 74.029509 |

`G_A`/`G_B` still hoist the `Globals::status` load above the chain; the
original loads it only after the `landmarks` null check.

### 3c. Planet-tail / recompute anchor placement

The recompute is the only tuned anchor. Moving it and/or adding hints:

| variant | usize | raw % | landmark loop @ |
|---|---:|---:|---|
| baseline | 9908 | **74.029509** | 0x760 |
| `V_nodup` (recompute deleted) | 9756 | 62.314026 | 0x1474 |
| `V2` nodup + `expect(landmarks != nullptr, 1)` | 9752 | 64.818024 | 0x6c8 |
| `N_A` nodup + `expect(!turretMode && !landmarkHit, 1)` | 9752 | 61.040462 | sunk |
| `N_B` nodup + both hints | 9756 | 63.971119 | 0x6c8 |
| `N_C` nodup + `expect(field_0x14 == 0, 1)` | 9756 | 62.314026 | sunk |
| `N_D` N_A + N_C | 9752 | 61.040462 | sunk |
| `N_E` N_B + N_C | 9756 | 63.971119 | 0x6c8 |
| `L1` nodup + `expect(loop condition, 1)` | 9956 | 46.240115 | — |
| `L2` nodup + `expect(skip, 0)` | 10020 | 28.404891 | — |
| `L3` both | 9952 | 46.063218 | — |
| `YA` recompute moved after planet tail | 9948 | 70.293486 | 0x7d4 |
| `YA_L` YA + `expect(landmarks != nullptr, 1)` | 9860 | 64.808162 | — |
| `YA_P` YA + `expect(!turretMode && !landmarkHit, 1)` | 9944 | 68.920080 | 0x7d4 |
| `YA_LP` YA + both | 9864 | 63.908046 | — |
| `A1` recompute before second chain | 9868 | 61.052026 | — |
| `A2` recompute after second chain | 9868 | 60.879563 | — |
| `A3` recompute before planet loop | 9868 | 63.977028 | — |
| `A4` recompute after planet block | 9936 | 62.067976 | — |
| `PT_OUT` planet tail moved outside campaign block | 9924 | 55.717971 | 0x1464 |
| `ORD1` landmark block moved after planet block | 9788 | 56.036973 | 0x7d2 |
| `GOTO1` explicit `goto enemies` after planet tail | 9908 | 74.029509 (identical) |
| `GOTO2` GOTO1 + planet-tail hint | 9904 | 71.281169 | 0x760 |
| `PL1` planet loop condition `expect(...,1)` | 10104 | 37.464306 | — |
| `PL0` planet loop condition `expect(...,0)` | 9836 | 53.944532 | 0x756 |

### 3d. Enlarging the recompute to shift the landmark loop

Adding unused calls before the recompute shifts the landmark block right
(each `getPlayer()->isDockingToAsteroid()` adds 0xc B):

| variant | usize | raw % | landmark loop @ |
|---|---:|---:|---|
| baseline | 9908 | **74.029509** | 0x760 |
| `X1` +1 unused call | 9920 | 72.784538 | 0x778 |
| `X2` +2 unused calls | 9932 | 72.121299 | **0x784** |
| `X3` +3 | 9944 | 72.077736 | 0x790 |
| `X4` +4 | 9952 | 72.036561 | 0x798 |
| `X5` +5 | 9956 | 72.044546 | 0x79c |
| `X6` +6 | 9964 | 72.003425 | 0x7a4 |
| `X7` +7 | 9968 | 71.982882 | 0x7a8 |
| `X8` +8 | 9976 | 71.913316 | 0x7b0 |

`X2` lands the landmark loop header exactly on the original `0x784`, yet
scores 1.9 points *lower*: the extra calls clobber r0–r3 and re-colour the
whole function (region match total 2521 vs 2584).  Register allocation, not
the loop address, dominates.

Redundant stores (`field_0x1a8 = onPlanetCourse` ×N, `planetDockIndex =
planetDockIndex` ×N) are optimised away (identical hash).  Non-call padding
(`field_0x2c += field_0x30` ×N) is emitted in the cold path and does not
move the loop (`PAD1..8`: 72.25–72.76 %, loop 0x774–0x77c).

### 3e. Landmark `i <= 1` block

| variant | usize | raw % |
|---|---:|---:|
| baseline | 9908 | **74.029509** |
| `S1` generic else with a real `String label` copy | 9928 | 73.195876 |
| `S2` `field_0x4 * 1` | 9908 | 74.029509 |
| `S3` local `labelX`/`labelY` | 9908 | 74.029509 |
| `R2` `i < 2` | 9908 | 72.797593 |
| `R3` `(i & 0x7fffffff) <= 1` | 9908 | 72.797593 |

The generic-else `DrawString` tail-merges with the `i==0` branch's second
`DrawString`; only a real `String` copy separates them, and the ctor/dtor
traffic costs more than the 32→7 hunk gains.

### 3f. Enemy / asteroid / boolean spellings (all no-ops or worse)

| variant | usize | raw % |
|---|---:|---:|
| baseline | 9908 | **74.029509** |
| `EN_E1` distance-first float crosshair X,X,Y,Y | 9896 | 67.459862 |
| `C1` = `AS_A2` duplicated asteroid `dead` | 9928 | 71.465369 |
| `C2` = `EN_E1` | 9896 | 67.459862 |
| `C3` AS_A2 + EN_E1 | 9896 | 67.459862 |
| `B2..B10` boolean respellings (`!x`↔`x==0`, `x`↔`x!=0`) | 9908 | 74.029509 (identical) |
| `M1`/`M2` local `int m = mode` | 9908 | 74.029509 (identical) |
| `M3` `(int) mode` | 9908 | 74.029509 (identical) |

Boolean spellings are fully canonicalised by clang: `!x`, `x == 0`,
`x != 0`, `if (ptr)` all emit identical code.

## 4. Conclusion

No measured variant beat the pristine baseline **9908 B / 74.029509 % /
3492/3489**.  This wave confirmed and extended the wave-113/115/116
findings:

1. The original's first `otherLock` chain (`cbnz` + shared 2-byte set
   block) is reachable from source only by enlarging the set block, which
   forces `otherLock` into r9 and costs more than it gains (`BL1`).
2. The post-loop recompute is a tuned block-placement anchor.  Its removal
   sinks the landmark loop; moving it after the planet tail puts the planet
   tail at the right address but pushes the loop 0x50 late.
3. Adding call padding can land the landmark loop header exactly on the
   original `0x784` (`X2`), but the extra calls re-colour the whole
   function and lose 1.9 points.
4. The residual gap is dominated by MachineBlockPlacement choices (planet
   tail vs landmark block ordering, literal-pool placement, the
   `vpush {d8-d13}` vs `{d8-d11}` crosshair-float liveness), not by
   missing statements.

A byte-faithful reconstruction would need all of: (a) delete the recompute,
(b) keep the planet tail hot at `0x6f8`, (c) keep the landmark loop hot at
`0x784`, (d) restore the duplicated asteroid `dead` predicate, (e) un-merge
the landmark else-branch `DrawString`, (f) keep the crosshair floats live
across `AEMath::operator-`, and (g) put `otherLock` in r5 — each individually
reachable from source, but each moves MachineBlockPlacement enough to lose
more matched instructions than it gains.

The worktree was restored to the pristine `Radar.cpp` before the final gate.
`check` is green: byte 4501, linked 4501, lint CLEAN, drift 0, data 843/843,
imports 207, `RATCHET PASS`.

## 5. Scratch

All scratch under `/dev/shm/w6506` (not committed): `Radar.base.cpp`
(pristine baseline source), `try.sh`, `score.py`, `region.py`, `loopaddr.py`,
`difftool.py`, `idxmap.py`, `orig.dis`/`ours.dis`, `rea.json`/`rea.c`
(Ghidra), and every variant listed above (`P1..P4`, `Q1..Q5`, `R1..R6`,
`BL1..BL3`, `G_A..G_C`, `N_A..N_E`, `L1..L3`, `YA*`, `A1..A4`, `X1..X8`,
`Y1..Y7`, `Z1..Z5`, `W1..W7`, `V1..V7`, `PAD1..PAD8`, `B2..B10`,
`M1..M3`, `S1..S3`, `C1..C3`, `PT_OUT`, `ORD1`, `GOTO1`, `GOTO2`,
`PL0`, `PL1`).
