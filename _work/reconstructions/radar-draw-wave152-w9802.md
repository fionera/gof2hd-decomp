# Radar::draw wave 152 (w9802) — a String copy with the copy ctor elided keeps the LSDA at 19 rows

**Status:** source change landed, 2026-11-08. Worktree `wt/w9802`, based on `master`
(`9d491d46a`, wave 151). Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).

Read with `radar-draw-wave151-w9702.md`, `radar-draw-wave151-w9702-pad.md`,
`radar-draw-wave148-w9401.md`, `radar-draw-wave149-w9501.md`.

Baseline (HEAD): original 9,908 B; ours 9,908 B; raw same-address diff **8,620**;
instruction-index alignment **1,530**; `.text` 0x1337c8; `.ARM.extab` 0x26298
(156,312); `.rodata` 0x1c9e10; byte_exact 4507.

## 1. Why the literal `String labelCopy2 = this->lockLabel` cannot be 19 rows

The w9702 String shape adds a **copy ctor invoke** in the landmark `else` arm.
Every throwing call that is inside a cleanup scope gets its own LSDA call-site
row (LLVM emits one landing-pad stub per invoke; the original's 11 LP offsets
are all distinct). The copy ctor is therefore +1 LP row, and the `DrawString`
that now takes a temporary is +1 LP row (the original's else-arm `DrawString`
passes the member `this->lockLabel`, so it has LP=0 and lives inside the
`(2404,158,0)` gap).

Measured on the current base, all at 9,908 B or close:

| shape | usize | raw | align | LSDA bytes / rows |
|---|---:|---:|---:|---:|
| HEAD baseline | 9908 | 8620 | 1530 | 131 / 19 |
| `String labelCopy2 = this->lockLabel;` (copy-init) | 9892 | 8658 | 1678 | 142 / 21 |
| copy-init + `int asteroidHit` | 9904 | 8390 | 1826 | 142 / 21 |
| copy-init + `int asteroidHit` + `bool landmarkHit` | 9908 | 8627 | 1586 | 142 / 21 |
| `String c; c = this->lockLabel;` (default+assign) | 9908 | 8579 | 1840 | 142 / 21 |
| **copy-init from the calcDistance prvalue (landed)** | **9908** | **8588** | **2188** | **131 / 19** |

So the literal copy-init shape is 142 B / 21 rows and grows `.ARM.extab` by 12,
which pushes `.rodata` from 0x1c9e10 to 0x1c9e20 (16-aligned) and collapses
byte_exact 4507 -> 4044. The w9801 pad hack (force gold's
`stub_group_auto_padding` cap to 0x30) is the only way to land that shape, and
it is a linker change.

## 2. The new hypothesis: copy from the prvalue, so the copy ctor is elided

`String labelCopy2 = this->calcDistance(...)` initialises `labelCopy2` directly
from the returned prvalue — C++11 copy elision, so there is **no copy-ctor
call**. Then `this->lockLabel = labelCopy2;` and `DrawString(..., labelCopy2, ...)`
share the same cleanup (destroy `labelCopy2`), so LLVM merges them into **one**
call-site row. The else arm therefore has 1 LP row instead of the original's 2,
and the whole function's LSDA stays at **19 rows / 131 bytes** — exactly the
baseline record size.

Landed source:

```diff
-                        this->lockLabel = this->calcDistance(landmark->posX, landmark->posY, landmark->posZ,
-                                                             this->cameraX, this->cameraY,
-                                                             this->cameraZ);
-                        Globals::Canvas->DrawString(radarFont(), this->lockLabel, this->screenX + offset,
+                        String labelCopy2 = this->calcDistance(landmark->posX, landmark->posY, landmark->posZ,
+                                                               this->cameraX, this->cameraY,
+                                                               this->cameraZ);
+                        this->lockLabel = labelCopy2;
+                        Globals::Canvas->DrawString(radarFont(), labelCopy2, this->screenX + offset,
                                                     this->screenY + rowHeight, false);
...
-        bool planetHit = false;
+        int planetHit = false;
```

The `int planetHit` widening is the size companion: the String-copy shape alone
is 9,880 B; `int planetHit` grows it by 28 B to the exact 9,908 B (a sweep of
~20 bool->int / expect toggles found `planetHit` as the only single companion
that lands on 9,908; `int asteroidHit` gives 9,892, `bool landmarkHit` 9,888,
`int enemyHit` 9,896).

## 3. LSDA row tables (reported as requested)

Original record @ 0x1c203c, **132 B / 19 rows** (11 LP!=0):

```
(0,568,0) (568,8,9818) (576,1818,0) (2394,10,9856) (2404,158,0)
(2562,10,9852) (2576,16,9858) (2592,12,9842) (2604,16,9832) (2638,20,9822)
(2658,78,0) (2736,14,9820) (2750,1256,0) (4006,8,9872) (4014,1796,0)
(5810,8,9870) (5818,788,0) (6606,8,9816) (6614,3294,0)
```

HEAD baseline record @ 0x1c203c, **131 B / 19 rows** (11 LP!=0):

```
(0,568,0) (568,8,9814) (576,1864,0) (2440,10,9852) (2450,102,0)
(2552,10,9848) (2566,16,9854) (2582,14,9838) (2596,12,9828) (2620,20,9818)
(2640,78,0) (2718,10,9816) (2728,1262,0) (3990,12,9872) (4002,1780,0)
(5782,8,9866) (5790,856,0) (6646,8,9812) (6654,3254,0)
```

Landed shape record @ 0x1c203c, **131 B / 19 rows** (11 LP!=0). The else-arm
`operator=`+`DrawString` are the single `(2458,42,9852,0)` row:

```
(0,568,0) (568,8,9814) (576,1882,0) (2458,42,9852) (2500,52,0)
(2552,10,9848) (2566,20,9854) (2586,10,9838) (2596,12,9828) (2620,20,9818)
(2640,78,0) (2718,10,9816) (2728,1258,0) (3986,8,9872) (3994,1802,0)
(5796,8,9866) (5804,1040,0) (6844,8,9812) (6852,3056,0)
```

For comparison, the literal copy-init shape's record is 142 B / 21 rows; the
extra two rows split the original `(2404,158,0)` gap into
`(2440,26,0) (2466,18,9844) (2484,52,0)` (copy-ctor LP + the two gaps).

## 4. Gate (after)

```
verify: byte 4507  linked 4507  stubs 0  missing 0  extra 0  avg 99.93
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

All 27 section sizes and VMAs are identical to the original (`.text` 0x1337c8,
`.ARM.exidx` 0x19cc08, `.ARM.extab` 0x26298, `.rodata` 0x1c9e10). `.rodata`,
`.data`, `.bss`, `.ARM.exidx` byte-identical; `.rel.plt` byte-identical.

* raw same-address **8,620 -> 8,588** (-32)
* instruction-index alignment **1,530 -> 2,188** (+658)
* `.text` differing bytes 86,744 -> 86,712 (-32)

The aligned regression is the price of the copy: the original keeps
`&this->lockLabel` in `r6` (spilled to `[sp,#104]`) across `operator=` and
`DrawString`; the landed shape keeps `&labelCopy2` in `r9` and moves the
`StringD1Ev` after the `DrawString`. It is a structural match to the original's
"one live String across both calls" but not yet the same register.

## 5. Open questions

1. The literal copy-init shape needs `.ARM.extab` -12 elsewhere (or the w9801
   pad). The landed shape sidesteps it by eliding the copy ctor; if a source
   shape can keep the copy-ctor codegen *and* share the `DrawString` landing
   pad, the literal shape becomes landable.
2. `r6` vs `r9`: the original spills `&lockLabel` to `[sp,#104]` and reloads it;
   the landed shape keeps `&labelCopy2` in `r9`. A shape that makes the
   temporary's address spill would close the aligned gap.
3. `int planetHit` is only a size companion; it does not improve either metric
   on its own (String shape alone is 8,359 / 1,673 at 9,880 B).
