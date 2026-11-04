# Radar::draw pass 17 (wf204) — a size-neutral `otherLock` hint moves the cold block
# 0x74 bytes earlier (raw 73.868195 -> 73.896848 %, report 73.9 -> 73.9)

**Status:** landed change + analysis note, 2026-11-04. Worktree `wt/wf204`, branch
`wave102-wf204`, base `53d9ee4d5`.

## 0. Baseline and result

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8
```

Before (HEAD `53d9ee4d5`):

```
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.91
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
_ZN5Radar4drawEP6PlayerP3Hudi: osize 9908 usize 9908
  raw 73.868195 %  norm 3492/3488  aligned 274/3492  report 73.9
```

After (this commit):

```
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.91
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS   (lint CLEAN, drift 0)
_ZN5Radar4drawEP6PlayerP3Hudi: osize 9908 usize 9908
  raw 73.896848 %  norm 3492/3488  aligned 275/3492  report 73.9
```

The body size is unchanged at **9908 B**, so no later function's PC-relative pool
load shifts and `byte_exact`/`linked_exact` are unchanged (4500/4500).

## 1. The change

Only the first `otherLock` block changes: the second conjunct gets its own
`__builtin_expect(..., 1)` hint (same-line `arm_builtin` waiver, as required by
`tools/goal_lint.py`):

```diff
     int otherLock;
-    if (__builtin_expect(this->lockedAsteroid == nullptr && this->lockedGasCloud == nullptr && // lint: arm_builtin anchor: Radar::draw; plain form shrinks body 9908 -> 9612
+    if (__builtin_expect(this->lockedAsteroid == nullptr && // lint: arm_builtin anchor: Radar::draw
+                                 __builtin_expect(this->lockedGasCloud == nullptr, 1) && // lint: arm_builtin anchor: Radar::draw
                                  this->field_0x1c == nullptr,
                          0)) {
```

This is a pure branch hint (no semantic change). It does **not** change the
`otherLock` block layout (still `cbz`/`mov r9,#1`/`b.n`), but it moves the cold
`otherLock = field_0x14 != 0` block from `0x261e` to `0x25aa` — 0x74 bytes
earlier, one normalized instruction closer to the original's `0xb72`.

## 2. The residual is still the ARM backend block layout, not the source

The original's first chain is the textbook short-circuit lowering with the
"any non-null" block placed **after** the test chain:

```
02b4  cbnz r0, 0x2c2     ; lockedAsteroid != null -> otherLock = 1
02b6  ldr  r0, [r4,#56]  ; lockedGasCloud
02b8  cbnz r0, 0x2c2
02ba  ldr  r0, [r4,#28]  ; field_0x1c
02bc  cmp  r0, #0
02be  beq.w 0xb72        ; all null -> cold: otherLock = field_0x14 != 0
02c2  movs r5, #1        ; otherLock = 1 (r5)
02c4  ldr.w r0,[r4,#312] ; landmarks
```

ours is the same CFG with the "any non-null" block placed **first**:

```
02b4  cbz  r0, 0x2d4     ; a == 0 -> evaluate b/c
02b6  mov.w r9, #1       ; otherLock = 1 (r9)
02ba  ldr  r5, [sp,#156]
02bc  b.n  0x2e8         ; <-- first unconditional branch after the pool's last use
02be  nop
02c0  pool (5 words)     ; original pool is at 0x2f0, after the station-index b.n
02d4  ldr  r0, [r4,#56]
02d6  ldr  r5, [sp,#156]
02d8  cmp  r0, #0
02da  itt  eq
02dc  ldreq r0, [r4,#28]
02de  cmpeq r0, #0
02e0  beq.w 0x261e       ; cold block, far tail
02e4  mov.w r9, #1
```

The `b.n` at `0x2bc` is the first unconditional branch after the last use of the
first constant pool (the loads at `0x14/0x34/0x4a/0x18a/0x264`), so
`ARMConstantIslandPass` dumps the pool there (`0x2c0`) instead of the original's
`0x2f0`. That single block-layout difference is why all five early
`ldr rX,[pc,#N]` immediates differ. It is a backend `MachineBlockPlacement`
decision; every source spelling tried (below) produces the same `cbz` layout.

## 3. Sweep (all measured with the official verifier API, `autojunk=False`)

Baseline = 9908 B / 73.868195 %. Every variant below keeps `osize 9908`.

### 3.1 `otherLock` source spellings

| variant | usize | raw % | pool |
|---|---:|---:|---:|
| baseline (negative AND + `expect(whole,0)`) | 9908 | 73.868195 | 0x2c0 |
| **landed: + `expect(b,1)`** | **9908** | **73.896848** | 0x2c0 |
| + `expect(b,1) + expect(c,1)` | 9908 | 73.896848 | 0x2c0 |
| + `expect(c,1)` only | 9908 | 73.868195 | 0x2c0 |
| + `expect(c,0)` only | 9908 | 73.868195 | 0x2c0 |
| positive OR, no expect | 9904 | 72.169676 | 0x2c0 |
| positive OR + `expect(whole,1)` | 9908 | 73.868195 | 0x2c0 |
| ternary `a ? 1 : b ? 1 : c ? 1 : d` | 9904 | 72.169676 | 0x2c0 |
| `int otherLock = 1; if (a&&b&&c) otherLock = d;` | 9904 | 72.169676 | 0x2c0 |
| default-then-if (`otherLock = d; if (a||b||c) otherLock = 1;`) | 9928 | 69.621343 | 0x2d0 |
| bitwise `|` OR | 9952 | 63.731536 | — |
| `expect(a,1)` inner | 9916 | 71.755944 | 0x290 |
| `expect(a,1)` inner, no outer expect | 9904 | 71.518625 | 0x298 |
| `expect(a,1)` inner + outer `expect(...,0)` | 9916 | 71.755944 | 0x290 |
| `expect(a,1) && expect(b,1) && c` (no outer) | 9900 | 72.703826 | — |
| `expect(a,0)` inner | 9900 | 71.502294 | 0x290 |
| `expect(a,0)` inner, no outer | 9912 | 71.823521 | — |
| `expect(b,0)` inner | 9900 | 71.559633 | 0x2c0 |
| `goto` spellings (4 forms) | 9904 | 72.169676 | 0x2c0 |
| nested-if / if-else-chain spellings | 9904 | 72.169676 | 0x2c0 |

The `expect(a,1)` family is the only one that produces the original's
**`bne`-to-cold-true-block + eval-chain-fall-through** shape (e.g.
`cmp; bne.w 0x258a; ldr b; ...; itt eq; ldreq c; cmpeq; beq.w 0x25b0; mov r9,#1`),
but every such variant is 9900–9916 B, i.e. not the hard 9908 B size. The
`expect(a,1)` block also still if-converts `b && c` into `itt eq; ldreq; cmpeq`.

### 3.2 `inSight` (enemy on-screen) shape

The original is distance-first (`vpush {d8-d13}`, `cx`/`cy` live across
`operator-`/`VectorLength` in `s26`/`s24`); the landed source is bounds-first
(`vpush {d8-d11}`). Re-measured with the current (wave-100/101) baseline:

| variant | usize | raw % |
|---|---:|---:|
| landed bounds-first | **9908** | **73.868195** |
| distance-first, x-pair/y-pair | 9924 | 71.275225 |
| distance-first, interleaved x/y | 9952 | 63.165447 |
| distance-first, `float cx/cy` | 9928 | 69.855363 |
| distance-first, inline `(int)crosshairPos.x` | 9928 | 62.494617 |
| distance-first, direct `bool inSight = ...` | 9928 | 64.541035 |

Distance-first is the faithful shape but costs +16 B (two extra
`ldr r6,[sp,#144]` reloads and an extra `b.n` from register allocation: ours keeps
`inSight` in `r0`/`r6` differently from the original's `movs r6,#0` /
`movs r6,#1`). At 9908 B it cannot be landed without a compensating -16 B
elsewhere.

### 3.3 Type / declaration micro-sweep (all size-neutral or worse)

`alwaysEnemySeen`, `missionBlocksPlanets`, `turretMode`, `alienOrbit`,
`onPlanetCourse`, `landmarkHit`, `bossSeen`, `enemyHit`, `alwaysEnemy`, `wreck`,
`hasDockNav`, `inCenter`, `slotCount`, `asteroidHit`, `dead`, `candidateWreck`,
`ambient`, `planetHit`, `marked` — each `bool`↔`int`/`uint32_t` swap was measured.
Only `onPlanetCourse`, `hasDockNav`, `slotCount`, `dead`, `candidateWreck`,
`ambient` stay at 9908 / 73.868195; all others are worse. Combining the neutral
swaps with the landed hint is also exactly 73.896848 (no further gain).

## 4. Proven vs hypothesis

**Proven (measured):**
* The landed hint is size-neutral (9908 B) and improves the raw verifier ratio
  73.868195 -> 73.896848 % (aligned 274 -> 275 of 3492); the official report
  percentage is unchanged at 73.9.
* The cold `otherLock` block moves 0x261e -> 0x25aa.
* The full gate is unchanged: byte 4500, linked 4500, size 4524/4524,
  data 843/843, imports 207, lint CLEAN, drift 0.
* Every `otherLock` spelling that keeps 9908 B keeps the `cbz`/`b.n` layout; the
  `expect(a,1)` family that gets the original `bne` layout is 9900–9916 B.
* Distance-first `inSight` is 9924 B (+16) and loses ~2.6 raw points.

**Hypothesis (not proven):**
* The remaining ~26 % mismatch is dominated by the ARM backend's block placement
  (pool island at 0x2c0 vs 0x2f0, cold block at 0x25aa vs 0xb72), the
  `vpush {d8-d11}` vs `{d8-d13}` register allocation, and the stack-slot
  rotation (`mode` sp#112 vs sp#100, `textOffset`/`stations`/`marked` slots).
  Fixing these requires the original's exact source/TU shape, not a local
  spelling change.
* The original's planet-tail autopilot block is not a second copy: the original
  has one autopilot block at 0x69e (reached from the initial `else` branch) and
  the planet tail at 0x6f8 does not recompute `onPlanetCourse`. Our source still
  has the extra planet-tail copy; removing it shrinks the body (~9836 B in pass
  13) and cannot be landed while the 9908 B size is a hard constraint.

## 5. Evidence / scratch (all under `/dev/shm/wf204`, not committed)

* `Radar.base.cpp` — pristine baseline source (SHA-1
  `52dd15a0ac276e783142b79cd9cc05fd7c4217ffc22af1b7102631e4ef45bb97`).
* `measure.py` — official verifier API for `_ZN5Radar4drawEP6PlayerP3Hudi`.
* `sweep.py`, `sweep2.py`, `sweep3.py`, `sweep4.py`, `sweep5.py`, `sweep6.py`,
  `sweep7.py`, `sweep_insight.py`, `sweep_types.py` — the variant generators.
* `inspect.py`, `region.py`, `structdiff.py`, `align3.py` — disassembly
  block/pool/region diff tools.
* `orig.raw.dis`, `ours.raw.dis`, `show_base.txt`, `structdiff_base.txt`,
  `align3_base.txt` — full annotated dumps.
