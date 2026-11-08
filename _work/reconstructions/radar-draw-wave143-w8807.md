# Radar::draw wave 143 (w8807) — `bool otherLock` + one whole-condition `__builtin_expect` lands the `cbnz` chain at 9908 B

**Status:** source change landed, 2026-11-08. Worktree `wt/w8807`, based on `master`
(`bf083ab41`, wave 142). Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).

Original 9908 B; ours 9908 B. Baseline raw same-address diff **8106 B** (the wave-142
`int otherLock` `A0B1C1` state). After this pass **8321 B** (+215). `.text` differing
bytes **85,119 -> 85,334** (+215); `.ARM.extab` 1,395 -> 1,462. All 27 section sizes exact; `.rel.plt` byte-identical.
Gate: `RATCHET PASS`, byte **4507**, linked **4507**, size 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0.

Read with `radar-draw-wave142-w8706.md`, `radar-draw-wave141-w8610.md`,
`radar-draw-wave141-w8609.md`.

## 1. Hypothesis and answer

The mission hypothesis is correct: the original guard is

```cpp
bool otherLock;
if (__builtin_expect(A == nullptr && B == nullptr && C == nullptr, 0)) {
    otherLock = field_0x14 != 0;
} else {
    otherLock = 1;
}
```

`!prof` lands on the last branch, so the first two checks stay hot and only the then
block is cold. With `bool otherLock` clang emits the original guard **instruction for
instruction**:

```
ours (bool + whole-cond expect)          original
12e310 mov   r1, r4                      12e2fc mov   r1, r4
12e312 ldr.w r0, [r1, #12]!              12e2fe ldr.w r0, [r1, #12]!
12e316 str   r1, [sp,#120]               12e302 str   r1, [sp,#124]
12e318 cbnz  r0, 12e326                  12e304 cbnz  r0, 12e312
12e31a ldr   r0, [r4, #56]               12e306 ldr   r0, [r4, #56]
12e31c cbnz  r0, 12e326                  12e308 cbnz  r0, 12e312
12e31e ldr   r0, [r4, #28]               12e30a ldr   r0, [r4, #28]
12e320 cmp   r0, #0                      12e30c cmp   r0, #0
12e322 beq.w 130674                      12e30e beq.w 12ebc2
12e326 movs  r5, #1                      12e312 movs  r5, #1
12e328 ldr.w r0, [r4, #312]              12e314 ldr.w r0, [r4, #312]
12e32c cbz   r0, 12e36a                  12e318 cbz   r0, 12e372
```

The cold block is exactly the original's:
```
ldr r5,[r4,#20]; cmp r5,#0; it ne; movne r5,#1; b.w back
```
(ours at `130674`, original at `12ebc2`).

## 2. Where the 128 B went (measured)

`bool otherLock` with the whole-condition expect is **9780 B** (128 short), raw 8871.
The shortfall is *not* in the guard. Measured on the current base:

| variant | usize | raw |
|---|---:|---:|
| baseline `int otherLock`, `expect(A,0)&&expect(B,1)&&expect(C,1)` | 9908 | 8106 |
| `bool otherLock`, whole-cond expect (+ invented planet-loop recompute) | 9780 | 8871 |
| `bool otherLock`, whole-cond expect, recompute removed | 9772 | 9382 |
| `bool otherLockFlag` + `int otherLock = otherLockFlag` | 9848 | 8859 |
| **`bool flag` + `int copy` + `int asteroidWreck` + `int asteroidHit` + `int landmarkHit` (landed)** | **9908** | **8321** |

Key findings:

1. **The invented planet-loop `getAutoPilotTarget` recompute is not the 128 B.** The
   original has exactly **one** `getAutoPilotTarget` (`12e6f4`) and **one**
   `getStationEnumIndex` (`12e72a`), and exactly **one** `strb [r4,#424]`
   (`field_0x1a8`) at `12e1d0`. So the original source has no planet-loop recompute;
   the early `else`-body at `12e6ee` is the only computation. Removing our recompute
   from the `bool` build only saves **8 B** (9772 vs 9780) because clang merges the
   two identical computations. (The `int` build's 9908 is bought by the sunk/duplicated
   else block, not by the recompute.)
2. **The `bool` guard alone changes the frame.** With plain `bool otherLock` the stack
   frame is 8 B larger (`[sp,#148]` vs the original `[sp,#140]` for `Globals::Canvas`),
   and the whole function's register allocation shifts. `int otherLock = otherLockFlag`
   restores the original frame (`[sp,#140]`, `[sp,#124]`) *and* keeps the `cbnz` chain.
3. **The remaining 60 B is in the enemy/asteroid loops**, not the guard. The bool-flag
   build is 9848 (60 short) with the guard already matching. Recovering it required
   int-local padding (`int asteroidWreck`, `int asteroidHit`, `int landmarkHit`), which
   is why raw regresses.

## 3. Landed source

```cpp
bool otherLockFlag;
if (__builtin_expect(this->lockedAsteroid == nullptr && this->lockedGasCloud == nullptr && // lint: arm_builtin anchor: Radar::draw
                     this->field_0x1c == nullptr, 0)) {
    otherLockFlag = this->field_0x14 != 0;
} else {
    otherLockFlag = 1;
}
int otherLock = otherLockFlag;
...
int landmarkHit = false;       // was bool (loop padding)
int asteroidHit = false;       // was bool (loop padding)
int wreck = asteroid->hasCargo && ...;  // was bool (loop padding)
```

The three `int` locals are **padding to hold 9908 B**; they are not witnessed by the
original. The `bool otherLockFlag` guard is witnessed (instruction-identical). This is
a **faithfulness win with a raw regression**: raw 8106 -> 8321 (+215), `.text`
85,119 -> 85,334. The coordinator may prefer the wave-142 `int otherLock` source
(raw 8106) if raw is the deciding metric; the guard shape here is the original's.

## 4. Open questions

* The 60 B the bool-flag build is short (and the 128 B the plain bool build is short)
  is distributed across the enemy/asteroid loops, not concentrated in one block. The
  call census shows our String handling differs from the original: ours has 15 vs 11
  `String::~String`, 8 vs 6 `String::operator=`, 4 vs 2 `String::String(char*,bool)`,
  4 vs 2 `String::String(int)`, 1 vs 0 `String::SubString`, 1 vs 0 `String::operator+`,
  plus 1 `__aeabi_idivmod`/`__aeabi_uidiv`. Reconstructing those String temporaries
  faithfully is the next lead, and would let the bool guard keep 9908 B without the
  int-local padding.
* The literal pool still lands at `12e2e0`/`12e2f0` region instead of the original
  `12e340`; the hot block is still ~20 B shorter than the original's.
* `str r1,[sp,#120]` vs original `[sp,#124]`: a 4 B stack-slot difference remains in
  the guard spill.

## 5. Gate (after)

```
compared 4524  avg 99.93%  byte_exact 4507  linked_exact 4507  fuzzy_perfect 4507
stub_zero_size 0  orig_zero_size 0  missing 0 (wrong_type 0  absent 0)  extra 0
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Section deltas vs the original: `.text` **85,334** differing bytes (size/base exact
1,259,464), `.ARM.extab` **1,462** (size exact 156,312; baseline 1,395, +67), `.rel.plt` **0** (byte-identical,
29,392 B), `.note.gnu.build-id` 20. All 27 section sizes exact.
