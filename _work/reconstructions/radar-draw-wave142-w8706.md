# Radar::draw wave 142 (w8706) — cbnz-chain guard: `bool` reproduces it exactly, int cannot (no source win)

**Status:** research note, 2026-11-08. Worktree `wt/w8706`, based on `master`
(`1d10d0694`). **No source change landed**; this commit contains this note only.
Target `src/game/weapons/Radar.cpp`, `_ZN5Radar4drawEP6PlayerP3Hudi`
(`Radar::draw(Player*, Hud*, int)`).

Original 9908 B; ours 9908 B. Baseline raw same-address diff **8106 B**
(the w8610 state: `int otherLock`, `expect(A,0) && expect(B,1) && expect(C,1)`,
campaign operand swap, `bool wreck`). `.text` differing bytes **84,567**.
Gate before and after: `RATCHET PASS`, byte **4507**, linked **4507**,
size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0.

Read with `radar-draw-wave141-w8610.md`, `radar-draw-wave141-w8609.md`,
`radar-draw-wave140-w8509.md`, `radar-draw-wave131-w7697.md`.

## 1. Mission hypothesis and answer

The mission asked for a source shape that keeps the 4 B hot else and lengthens
the hot guard block by 20 B so the first literal pool lands at `12e340`
(original) instead of `12e2e0` (ours), using the original's `cbnz` chain and
`int otherLock` register-pressure shapes.

**Answer: the `cbnz` chain is a property of `bool otherLock`, not of `int`
register pressure.** With `bool otherLock` and the *whole* condition under one
`__builtin_expect(..., 0)`, clang emits the original's guard instruction for
instruction:

```
ours (bool + whole-cond expect)          original
12e310 mov   r1, r4                      12e2fc mov   r1, r4
12e312 ldr.w r0, [r1, #12]!              12e2fe ldr.w r0, [r1, #12]!
12e316 str   r1, [sp, #132]              12e302 str   r1, [sp, #124]
12e318 cbnz  r0, 12e326                  12e304 cbnz  r0, 12e312
12e31a ldr   r0, [r4, #56]               12e306 ldr   r0, [r4, #56]
12e31c cbnz  r0, 12e326                  12e308 cbnz  r0, 12e312
12e31e ldr   r0, [r4, #28]               12e30a ldr   r0, [r4, #28]
12e320 cmp   r0, #0                      12e30c cmp   r0, #0
12e322 beq.w 1305f4                      12e30e beq.w 12ebc2
12e326 movs  r5, #1                      12e312 movs  r5, #1
12e328 ldr.w r0, [r4, #312]              12e314 ldr.w r0, [r4, #312]
12e32c cbz   r0, 12e36a                  12e318 cbz   r0, 12e372
```

But that shape is **9780 B (128 B short)**, raw **8871**. Every `int`
spelling (`expect` on the whole condition, on the last operand only, on a
`bool` temporary, ternary, nested `if`, bitwise `&`) sinks the B/C checks into
a cold block and keeps `cmp`+`beq.w` + `mov.w r9,#1`; the `int` phi is what
lets clang duplicate/sink the else. Adding int locals to the `bool` shape can
reach 9908 B (`int asteroidHit+asteroidWreck+gasHit+topOk` = 9908, raw 8691)
but the added bytes land in the loops and the raw is far worse than 8106.

## 2. The literal pool is independent of the guard shape

The pool at `12e2e0` (ours) vs `12e340` (original) is **not** moved by the
guard shape. `bool` (cbnz chain) and `int` (cbz chain) both dump it at
`12e2e0`. It is dumped right after the `b.n` at `12e2de` (the onScreen block's
branch over the scanner block); the original defers it past the guard to the
`b.n` at `12e33c`. The code before `12e2de` is byte-identical in both builds
(shift 0 through `12e2de`), so the deferral is a global ARMConstantIslands
fixed point, not a local guard property.

## 3. `int asteroidWreck` is the faithful type but breaks `.text` alignment

w8509 landed `int asteroid wreck`; w8610 changed it to `bool` to shave the last
4 B. On the current base `int asteroidWreck` gives **9912 B, raw 8097**
(-9 raw) and `int asteroidWreck + int missionBlocksPlanets` gives
**9912 B, raw 8082** (-24 raw). But the 4 B growth is *not* absorbed by
padding: `.text` differing bytes jump **84,567 -> 96,936** because every
function after `Radar::draw` shifts by 4 B. So the function must stay exactly
9908 B.

## 4. Variant matrix (all measured on the current base)

| variant | usize | raw | `.text` diff |
|---|---:|---:|---:|
| **baseline (int A0B1C1)** | **9908** | **8106** | **84,567** |
| `bool otherLock`, whole-cond expect | 9780 | 8871 | — |
| `bool` + `int planetHit` | 9836 | 8717 | — |
| `bool` + `int bossSeen+planetHit` | 9844 | 8650 | — |
| `bool` + 7 int locals (`bossSeen,inSight,enemyWreck,enemyHit,planetHit,asteroidHit,asteroidWreck`) | 9876 | 8706 | — |
| `bool` + `int asteroidHit+asteroidWreck+gasHit+topOk` | **9908** | 8691 | — |
| `bool noLock` + `int otherLock` + `int notOtherLock=!noLock` | 9844 | 8854 | — |
| `int asteroidWreck` | 9912 | 8097 | 96,936 |
| `int asteroidWreck+missionBlocksPlanets` | 9912 | 8082 | — |
| `int` whole-cond expect | 9904 | 8463 | — |
| `int` `A && B && expect(C,0)` | 9904 | 8463 | — |
| `int` `A && B && C` plain | 9908 | 8723 | — |
| `int` `A0 && B && C` | 9908 | 8137 | — |
| `int` `A0 && B1 && C0` | 9908 | 8131 | — |
| `int` `A0 && B1 && C1` (**baseline**) | 9908 | 8106 | — |

## 5. Root cause / open questions

The original guard is `bool otherLock` with one `__builtin_expect(A==0 && B==0
&& C==0, 0)`; the `!prof` lands on the *last* branch, so the first two checks
stay hot and only the then block is cold. Our 9908-B `int` shape buys the size
with a sunk cold block that duplicates the else, which is why its guard is
`cbz`+`itt`, not `cbnz`. To land the cbnz chain at 9908 B one must recover the
128 B that the `bool` shape loses *elsewhere* (the planet-loop block order /
the single-vs-double `getAutoPilotTarget` recompute), not in the guard. The
`.text` alignment constraint means any 4 B size change is fatal to the rest of
the section, so the 9908 B target is hard.

## 6. Gate (after, source unchanged)

```
compared 4524  avg 99.93%  byte_exact 4507  linked_exact 4507  fuzzy_perfect 4507
stub_zero_size 0  orig_zero_size 0  missing 0 (wrong_type 0  absent 0)  extra 0
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Section deltas vs the original: `.text` **84,567** differing bytes (size/base
exact 1,259,464), `.ARM.extab` 1,395 (size exact 156,312), `.rel.plt` **0**
(byte-identical, 29,392 B), `.note.gnu.build-id` 20. All 27 section sizes exact.
