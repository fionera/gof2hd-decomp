# Radar::draw wave 140 (w8509) — hunk classification + `alwaysEnemy`/asteroid-`wreck` int locals: raw 8346 -> 8279 at 9908 B, `.rel.plt` exact

**Status:** source change landed, 2026-11-07. Worktree `wt/w8509`, based on `master`
(`d4857f174`). Target `src/game/weapons/Radar.cpp`, function
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).

Original 9908 B / 3492 normalised instructions; ours 9908 B. Baseline for this
wave is the w8409 nested-`inSight` + `Vector delta` state (raw same-address diff
**8346**). After this change: raw **8279** (-67), size **9908**, `.rel.plt`
**byte-identical** (0 differing bytes of 29392). Gate before and after:
`RATCHET PASS`, byte **4507**, linked **4507**, size 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0, avg 99.93.

Read with `radar-draw-wave139-w8409.md`, `radar-draw-wave139-w8410.md`,
`radar-draw-wave138-w8309.md`, `radar-draw-wave136-w8109.md`.

## 1. Why the raw metric is dominated by misalignment

The nested-`inSight` shape re-rolls block placement, so the two functions are
instruction-for-instruction *equivalent* over most of the body but shifted by a
few bytes at every block boundary. The 8,346 raw diff bytes are spread almost
uniformly: 20 of the 256-byte windows differ in 245-255 bytes. This is a
**shift** artifact, not 8,346 bytes of wrong code. Classification of the final
variant by source region (same-address raw diff):

| region | span | diff bytes |
|---|---:|---:|
| prologue + route waypoint | 0x12e050-0x12e304 | **12** |
| `otherLock` guard | 0x12e304-0x12e3fe | **213 / 250** |
| planet-targets loop | 0x12e3fe-0x12ec09 | 1719 / 2059 |
| enemy loop | 0x12ec09-0x12f460 | 1844 / 2135 |
| gas loop | 0x12f460-0x12f98c | 1256 / 1324 |
| asteroid loop | 0x12f98c-0x13066a | 3170 / 3294 |
| tail | 0x13066a-0x13073c | ~0 |

The prologue/route is essentially exact. The one *dense* local hunk is the
`otherLock` guard (85 % of its bytes differ), already attacked by w8410. Every
other region is 83-96 % "different" purely because the shift delta oscillates
between -32 and +52 bytes (measured with an instruction-aligned diff). The
largest contiguous raw run (318 B at 0x13000a) is the enemy lock-timer block
sitting at a different address in the two binaries, not a local codegen error.

## 2. NEW: the per-loop `alwaysEnemy` update is a real source difference

The enemy-loop `alwaysEnemy` update is the clearest *local* (non-shift) delta
found this wave. Original (0x12ede2):

```
12ede2 blx Player::isAlwaysEnemy
12ede6 cmp r0,#0
12ede8 ldr r0,[sp,#80]      ; alwaysEnemy
12edea it ne
12edec movne r0,#1          ; select: isAlwaysEnemy ? 1 : alwaysEnemy
12edee str r0,[sp,#80]
```

Baseline (bool `alwaysEnemy`) emitted an OR instead:

```
12ede8 blx isAlwaysEnemy
12edec cmp r0,#0
12edee it ne
12edf0 movne r0,#1
12edf2 ldr r1,[sp,#80]
12edf4 orrs r1,r0           ; alwaysEnemy |= isAlwaysEnemy
12edf6 str r1,[sp,#80]
```

Declaring the local `int alwaysEnemy = false;` (the surrounding
`alwaysEnemySeen`/`bossSeen`/`enemyHit` are already int-flavoured in the
original source style) makes clang emit the original **select** shape:

```
12ede8 blx isAlwaysEnemy
12edec cmp r0,#0
12edee ldr r0,[sp,#96]
12edf0 mov.w r1,#1
12edf4 it ne
12edf6 movne r0,r1
12edf8 str r0,[sp,#96]
```

The residual is only the `mov.w r1,#1; movne r0,r1` rematerialisation (4 B)
and the stack slot (#96 vs #80), not the control-flow shape.

## 3. NEW: `int` asteroid `wreck` restores the 9908 B size

`int alwaysEnemy` alone is **9904 B** (4 B short) with raw **8233**. The
asteroid-loop `wreck` local, declared `int` instead of `bool`, adds exactly the
missing 8 B of the asteroid block and brings the function back to **9908 B**:

```cpp
int alwaysEnemy = false;                                   // enemy loop
int wreck = asteroid->hasCargo && (asteroid->isDead() || asteroid->isDying());  // asteroid loop
```

Measured matrix (single edits on the w8409 `delta` baseline unless noted):

| variant | usize | raw diff | `.rel.plt` |
|---|---:|---:|---:|
| **w8409 baseline** | **9908** | **8346** | **0** |
| `int alwaysEnemy` | 9904 | 8233 | 0 |
| `int alwaysEnemy` + `int asteroid wreck` (**landed**) | **9908** | **8279** | **0** |
| `int alwaysEnemy` + `int bossSeen` | 9892 | 8045 | 0 |
| `int alwaysEnemy` + `int bossSeen` + `int asteroid wreck` | 9900 | 8017 | 0 |
| `int alwaysEnemy` + `int bossSeen` + `int enemy wreck` + `int asteroid wreck` | 9908 | 8356 | 0 |
| `int alwaysEnemy` + `int enemyHit` | 9928 | 8255 | 0 |

`int alwaysEnemy + int bossSeen + int asteroid wreck` is the best raw (8017) but
is 8 B short; no faithful `bool->int` local found that adds exactly 8 B without
re-rolling the enemy tail. `int enemy wreck` does add 8 B but costs 77 raw bytes
in the enemy loop, so the landed pair is `alwaysEnemy` + asteroid `wreck`.

## 4. Hunks tried and ruled out this wave

* Remove the invented post-loop `onPlanetCourse` recompute: 9860 B, raw 9206
  (the original calls `getAutoPilotTarget` once, but the 48 B are not
  recoverable without another change).
* Drop the outer `__builtin_expect(..., 0)` on the `otherLock` guard: 9896 B,
  raw 9129.
* `!(B || other || turret)` tail with the `delta` local: 9892 B, raw 8629,
  `.rel.plt` diff 6.
* `!(B || other || turret)` tail with no `delta` local: 9908 B, raw 8577,
  `.rel.plt` diff 6 (confirms w8309/w8409).
* `bool alwaysEnemy = 0` / `unsigned int alwaysEnemy`: identical to `bool`
  (8346) / 9904 raw 8233 respectively.

## 5. Open questions

* The `alwaysEnemy` select is still 4 B longer (`mov.w r1,#1; movne r0,r1` vs
  `movne r0,#1`) and uses sp#96 vs the original sp#80. Stack-slot assignment is
  global; fixing it needs the `otherLock` guard / `stations` spill changes.
* `int alwaysEnemy + int bossSeen + int asteroid wreck` at 9900/8017 is the
  best raw state; recovering the missing 8 B *without* touching the enemy tail
  is the next target.
* The `otherLock` guard remains the densest local hunk (213/250); w8410's
  `la_e_o0` (117/132 guard bytes) is still 4 B too big.

## 6. Gate / section deltas

Before (w8409 delta baseline): byte 4507, linked 4507, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, avg 99.93.
After: byte **4507**, linked **4507**, size 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0, avg **99.93**. All 27 ELF section sizes
unchanged; `.rel.plt` content byte-identical (29392 B, 0 differing).
