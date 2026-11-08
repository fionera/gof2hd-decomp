# Radar::draw wave 151 (w9803) — witnessed `bossSeen` re-test; why 68 never reaches `sl`

**Status:** source change landed, 2026-11-08. Worktree `wt/w9803`, based on `master`
(`9d491d46a`, wave 151). Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`).

Read with `radar-draw-wave151-w9703.md`, `radar-draw-wave149-w9606.md`,
`radar-draw-wave148-w9401.md`.

Baseline (HEAD): original 9908 B; ours 9908 B; raw same-address diff **8,620**;
instruction-index alignment **1,530**.

## 1. New witnessed structural difference: the first `bossSeen` site re-tests `shipGroup`

Original, inactive-enemy branch (`12ed18`..`12ed5c`):

```
12ed18 ldr.w r0,[r8,#40]      ; shipGroup
12ed1c cmp r0,#10
12ed1e bne.w 12f184           ; != 10 -> continue
12ed22 ...isActive / isDead / isDying...
12ed48 ldr.w r0,[r4,#444]     ; this->field_0x1bc
12ed4c adds r0,#1
12ed4e str.w r0,[r4,#444]
12ed52 ldr.w r0,[r8,#40]      ; shipGroup RELOADED
12ed56 cmp r0,#10
12ed58 it eq
12ed5a moveq r6,#1            ; bossSeen = (shipGroup == 10)
12ed5c b.n 12f184
```

Ours emitted `mov.w r9,#1` unconditionally (5 instructions, no reload). The reload is
required because the store to `this->field_0x1bc` may alias `enemy->shipGroup`, so the
original source re-tests the field after the increment:

```cpp
if (enemy->shipGroup == 10 && enemy->player->isActive() && !enemy->isDead() && !enemy->isDying()) {
    this->field_0x1bc++;
    bossSeen = enemy->shipGroup == 10;   // was: bossSeen = true;
}
```

This is a real source difference (call it `S`). It costs **+8 B** (9908 -> 9916) and
improves alignment **1530 -> 1527**. `S` alone cannot land (size-exact required).

## 2. Why 68 lands in `sl` in the original but in `r6` in ours (post-RA LICM)

`-mllvm -print-machineinstrs` on the current base shows the mechanism:

* `# After Greedy Register Allocator`: the planet loop's `t2MOVi 68` is allocated to
  `$r11` and the enemy loop's to `$r6`.
* `# After Machine Loop Invariant Code Motion` (the **post-RA** LICM): the planet-loop
  `$r11 = t2MOVi 68` is hoisted into the preheader (`bb.44`), because `r11` is not
  clobbered in that loop. The enemy-loop `$r6 = t2MOVi 68` is **not** hoisted, because
  `r6` is also used for the `17` argument inside the loop (`movs r6,#17; str r6,[sp];
  movs r6,#68; str r6,[sp,#4]`).

So the enemy-loop 68 only becomes a loop-invariant register if the RA gives it a
register that is *not* the scratch `r6`. In the original the enemy loop is

```
r6  = bossSeen (live whole loop)   -> r6 cannot also hold 17/68
r9  = &this->lockLabel (live whole loop)
sl  = 68
fp  = i
r5  = locked / 17
wreck -> spilled [sp,#152]
```

Ours is a permutation: `r9 = bossSeen`, `sl = wreck`, `r6 = 17/68`, `r5 = i`,
`&lockLabel` spilled to `[sp,#76]`, `fp` scratch. Because `bossSeen` sits in `r9`
instead of `r6`, `r6` is free and the RA reuses it for both `17` and `68`.

The prerequisite for `sl = 68` is therefore **`bossSeen` in `r6`** (or any other
occupation of `r6`), not a bool->int widening. Every widening tested on this base
(`c`, `cst`, `cstb`, `T`, `E`, `M`, `W`, `K`, `H`, `A`, `O`, `S` and pairs) leaves
`r6_68 = 5`, `sl68 = 0`.

## 3. Sweep results on this base (size / raw / aligned)

| variant | usize | raw | align |
|---|---:|---:|---:|
| baseline | 9908 | 8620 | 1530 |
| `S` (bossSeen re-test) | 9916 | 8647 | 1527 |
| `c` (`bool wreck`) / `cstb` | 9892 | 8573 | 1533 |
| `S,H` (`int bossSeen`) | 9876 | 8561 | 1497 |
| `S,T,H` | 9872 | 8376 | 1445 |
| `c,S,T` | 9900 | 8489 | 1485 |
| `S,T,A` | 9908 | 8755 | 1463 |
| **`S,M,A` (landed)** | **9908** | **8587** | **1565** |

`S,T,H` (8376 / 1445) is the best shape found but is 36 B short. `S,H` is 32 B short.
No 9908 B shape improves both metrics; `S,M,A` is the only 9908 B shape that improves
the raw metric.

## 4. Landed change

```diff
-    bool missionBlocksPlanets = false;
+    int missionBlocksPlanets = false;
...
-        bool alwaysEnemy = false;
+        int alwaysEnemy = false;
...
-                    bossSeen = true;
+                    bossSeen = enemy->shipGroup == 10;   // witnessed re-test
```

* raw same-address **8,620 -> 8,587** (-33)
* instruction-index alignment **1,530 -> 1,565** (+35)
* size 9,908 B (frame 296); all 27 section sizes exact.

## 5. Gate (after)

```
compared 4524  avg 99.93%  byte_exact 4507  linked_exact 4507  fuzzy_perfect 4507
stub_zero_size 0  orig_zero_size 0  missing 0 (wrong_type 0  absent 0)  extra 0
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

## 6. Open questions

1. Occupy `r6` with `bossSeen` (the original's home) without changing the function size.
   `S` does move `bossSeen` off `r9` for the first site but the RA still picks `r9`;
   `H` (`int bossSeen`) shrinks the body by 32 B. A 9,908 B shape with `bossSeen` in
   `r6` is the remaining lever for `sl = 68` + `strd r6,sl` / `strd r5,sl`.
2. `S,T,H` (9872, 8376/1445) needs a witnessed +36 B body addition.
3. `S,H` (9876, 8561/1497) needs +32 B.
