# Radar::draw pass (wave106, w5411) — landmark hit-box right bound as `screenX < left + 2*size`

**Status:** landed source change, 2026-11-04. Worktree `wt/w5411`, branch `wave106-w5411`,
base `486f8a119` (master after wave105). Target `_ZN5Radar4drawEP6PlayerP3Hudi`.

## 0. Result

One-line, size-neutral, raw-improving operand swap in the landmark hit box:

```cpp
// before
if (left < this->screenX && this->screenY < top + size + size && topOk &&
    left + size + size > this->screenX) {

// after
if (left < this->screenX && this->screenY < top + size + size && topOk &&
    this->screenX < left + size + size) {
```

```
                                   raw %        norm        osize  usize  report
before (base 486f8a119)            73.972210    3492/3489   9908   9908   74.0
after  (this commit)               74.029509    3492/3489   9908   9908   74.0
```

`+0.0573` raw percentage points, body size unchanged at 9908 B, one fewer differing
instruction (the normalized instruction count is unchanged at 3492/3489, but the
SequenceMatcher longest-match improves). The rounded report score stays 74.0.

## 1. Exact commands

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
```

Per-variant measurement used the official verifier API (`/dev/shm/w5411/measure.py`), which
disassembles both functions, reconciles branch targets and data references, and reports
`difflib.SequenceMatcher(...).ratio()` on the reconciled normalized instruction stream.

## 2. Gate (after the landed change)

```
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.91
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
drift:  0 field(s) across 0 class(es)
lint:   CLEAN
```

`tools/verify_baseline.json` was not touched; `report.json` was not modified by the commit.

## 3. Why the swap is real

The original Thumb condition (Ghidra `radar-draw` line 281-285 / `orig.dis` landmark hit box)
evaluates, in order:

```
left < screenX
&& screenY < top + size*2
&& top < screenY
&& screenX < left + size*2
```

The pre-pass source spelled the last comparison as `left + size + size > this->screenX`
(same predicate, operands reversed). Clang 7.0.2 at `-Oz` allocates the comparison
differently for the two spellings: the `screenX < ...` form matches one more original
instruction (the `cmp` operand order / branch sense), and the measured raw ratio rises
from 73.972210 to 74.029509 with identical 9908 B size. This is a genuine source-level
improvement, not a metric artifact: the body size is unchanged and the normalized stream
is unchanged in length.

## 4. Measured dead ends (this pass, all 9908 B unless noted)

Baseline for this table is the landed 74.029509.

### 4.1 Landmark hit-box conjunct order

| variant | raw % | norm |
|---|---:|---|
| landed (left, y-upper, topOk, right) | **74.029509** | 3492/3489 |
| `topOk` first | 73.886263 | 3492/3489 |
| right bound before `topOk` | 73.818390 | 3492/3490 |
| inline `top < screenY` (drop `topOk`) | 73.886263 | 3492/3489 |
| `top` first | 73.886263 | 3492/3489 |
| right bound first | 73.789745 | 3492/3490 |
| y-upper first | 73.743017 | 3492/3489 |

### 4.2 Landmark guard `!landmarkHit && i != 3 && notTurretMode && notOtherLock`

| permutation | raw % | norm |
|---|---:|---|
| landed | **74.029509** | 3492/3489 |
| `!landmarkHit && !notTurretMode && i != 3 && !notOtherLock` | 73.896848 | 3492/3488 |
| `!landmarkHit && !notOtherLock && i != 3 && !notTurretMode` | 73.982808 | 3492/3488 |
| `i != 3` first | 72.216650 | 3492/3487 (9904 B) |
| `!notTurretMode` first | 70.738351 | 3492/3483 (9924 B) |
| `!landmarkHit && !notTurretMode && !notOtherLock && i != 3` | 72.216650 | 3492/3487 (9904 B) |
| `!landmarkHit && i != 3 && !notOtherLock && !notTurretMode` | 70.778941 | 3492/3479 (9904 B) |

The Ghidra guard order is `(!landmarkHit && !turretMode) && (i != 3 && !otherLock)`; the
closest measured spelling (`lm_ol_i3_tm`) is 73.982808, still below the landed form.

### 4.3 Planet guard `!otherLock && !planetHit && !turretMode`

| permutation | raw % | usize |
|---|---:|---:|
| landed | **74.029509** | 9908 |
| `!turretMode && !planetHit && !otherLock` (Ghidra order) | 74.029509 | 9908 |
| `!planetHit && !otherLock && !turretMode` | 74.029509 | 9908 |
| `!planetHit && !turretMode && !otherLock` | 57.795660 | 9852 |
| `!turretMode && !otherLock && !planetHit` | 57.795660 | 9852 |
| `!otherLock && !turretMode && !planetHit` | 57.795660 | 9852 |

The Ghidra order is neutral; every other permutation loses the 9908 B size.

### 4.4 Enemy in-sight bound order

| variant | raw % | usize |
|---|---:|---:|
| landed (x-lower, y-lower, x-upper, y-upper, distance) | **74.029509** | 9908 |
| y-pair first | 70.844063 | 9936 |
| x-pair then y-pair | 70.844063 | 9936 |
| swap x/y upper bounds | 70.844063 | 9940 |

The original computes `VectorLength(egoPos - targetPos)` *first* (Ghidra), but the faithful
distance-first spelling measures 67.459862 (9896 B, pass 18 table) — the tuned bounds-first
order scores higher.

### 4.5 Local-pointer CSEs (neutral)

| variant | raw % |
|---|---:|
| `KIPlayer *landmark = landmarks->data()[i]` in hit box | 74.029509 |
| `KIPlayer *planet = planetTargets->data()[i]` in planet lock | 74.029509 |

Clang already CSEs the repeated `data()[i]` loads; the local spellings are exactly neutral.

### 4.6 Combinations

`lm_ol_i3_tm` + planet Ghidra order = 73.982808; `lm_ol_i3_tm` + `ph_ol_tm` = 73.982808;
`lm_tm_i3_ol` + planet Ghidra order = 73.896848. No combination beat 74.029509.

## 5. Proven vs hypothesis

**Proven (measured):**
* The single-line right-bound swap is size-neutral (9908 B) and raw-improving
  (73.972210 -> 74.029509).
* The source logic is faithful block-for-block to the Ghidra decompilation; the residual
  is ARM backend block placement, register allocation, stack-slot colouring and
  literal-pool slotting.
* The duplicate post-planet-loop autopilot block is load-bearing for the hard 9908 B size
  (removing it gives 9756 B / 62.31, pass 18).
* Guard-conjunct reorderings and local-pointer CSEs are neutral or worse; the landed
  spelling is the best measured shape.

**Hypothesis (not proven):**
* The first `otherLock` cold block sits at `0xb72` in the original and `0x25aa` in ours in
  every size-9908 spelling tried; the 0x44 landmark-loop block shift and the cold-block
  placement are decided by `MachineBlockPlacement` and likely need the original TU /
  source shape, not a local statement spelling.
* `vpush {d8-d13}` (original) vs `{d8-d11}` (ours) is crosshair-float liveness across the
  enemy in-sight `VectorLength` call; the faithful distance-first order scores lower on the
  fuzzy metric, so this is a codegen/register-pressure residual rather than a missing
  statement.

## 6. Evidence / scratch

All scratch under `/dev/shm/w5411` (not committed):
`Radar.cur.cpp` (pre-pass source), `Radar.best.cpp` (landed source), `measure.py`,
`runvars.py`, `dump2.py`, `diffregions.py`, `orig.dis`, `ours_swap.dis`,
`tryvar11.py`/`tryvar29.py` (earlier in-sight sweeps), `ghidra-draw.c`.
