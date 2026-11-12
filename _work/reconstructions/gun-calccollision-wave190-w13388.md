# `Gun::calcCharacterCollision` wave 190 (w13388): sinking the `falloff`
# clamp below the EMP block is a size-exact raw win (457 -> 455)

Worker: w13388. Branch `wave190-w13388`, base master `01f5edc5d`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump `0x152f90`, size `1984 B`,
660 insns. Scratch: `/home/debian/GalaxyOnFire2/scratch/w13388`.

## 0. Result

| build | raw | aligned | size | `.text` diff |
|---|---:|---:|---:|---:|
| before (HEAD `01f5edc5d`) | **457** | **84** | 1984/1984 | 73,274 |
| after this pass | **455** | 93 | 1984/1984 | **73,272** |

`check` after (`-j2`, `ulimit -v 12000000`): `RATCHET PASS`, byte_exact
4507/4524, linked_exact 4507/4524, size-exact 4524/4524, stub_zero_size 0,
missing 0, wrong_type 0, extra 0, data 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370.

## 1. Change

In the scatter k-loop body, sink the `falloff` clamp **below** the EMP block
(it only feeds the two damage calls, so this is a pure statement move):

```c++
                        float falloff = (float) diff / (float) this->magnitude;
+                       if (this->empDamage > 0) {
+                           enemy->damageEmp((int) (falloff * (float) this->empDamage), this->friendGun);
+                       }
                        if (falloff > 1.0f) {
                            falloff = 1.0f;
                        } else if (falloff < 0.0f) {
                            falloff = 0.0f;
                        }
-                       if (this->empDamage > 0) {
-                           enemy->damageEmp((int) (falloff * (float) this->empDamage), this->friendGun);
-                       }
                        enemy->damage((int) (falloff * (float) this->damage), this->friendGun, this->itemIndex);
```

This is *not* the shipped order (the original emits the clamp between the
div and the `damageEmp` block at `0x153496`-`0x1534ae`); it is a layout tweak
that wins two bytes on the raw metric.  The landed wave-189 `damage`/
`damageEmp` swap is kept.

## 2. Why two bytes

The k-loop diff groups shrink from
`26+24+19+15+15+13+12+9+6+4+4+3+2+2` (HEAD, ~150 B) to
`28+28+19+14+13+9+9+9+8+4+4+3+2+2` (this pass, ~133 B): the clamp's
`vcmpe`/`vmrs`/`bgt`/`vmov`/`it mi`/`vmovmi` block moves next to the
`damageEmp` block, which lets the two `vldr s0,[r9,#96]` / `vldr r0,[r9,#100]`
literal loads share a better schedule.  The 226-byte `r4`/`r5` phase-shift
run at `0x153198`-`0x15327e` is untouched (see §3).

## 3. Residual (unchanged root cause)

The remaining 455 bytes are still dominated by the `i*3`/`&stack.10` `r4`/`r5`
swap (wave-126 w7610 / wave-189 w13353): Early Machine LICM hoists the merged
`view`/k-loop `ADDri %stack.10` to the i-loop preheader, greedy RA gives that
long range `r4`, and `i*3` gets `r5`.  The 2-byte phase shift at `0x153198`
(`add r4,sp,#104` in ours vs `ldr.w fp,[sp,#40]` in the shipped body) is the
consequence and accounts for ~226 of the 455 raw bytes.

## 4. Cells measured this pass (all `link_so` + `tools/fn_diff.py`, `-j2`)

All variants keep 1984 B unless stated; none of them beats 455.

| variant | raw | aligned | size |
|---|---:|---:|---:|
| HEAD (before) | 457 | 84 | 1984 |
| **clamp sunk below the EMP block (this pass)** | **455** | 93 | 1984 |
| clamp sunk below the EMP block + `>=`/`<=` clamp | 455 | 93 | 1984 |
| `damage` before `damageEmp` (faithful order) | 458 | 63 | 1984 |
| `damage` before `damageEmp` + clamp last | 458 | 63 | 1984 |
| clamp sunk below both damage calls | 1794 | 171 | 1940 |
| clamp after `damage` only | 458 | 63 | 1984 |
| two separate clamp `if`s | 1021 | 131 | 1960 |
| clamp ternary / `min`/`max` | 1021 / 1017 | 131 / 125 | 1960 / 1968 |
| clamp branch order swapped | 459 | 92 | 1984 |
| `empDamage != 0` / `empDamage` (plain) | 1036 | 122 | **1980** |
| `empDamage >= 1` / `0 < empDamage` | 457 | 84 | 1984 |
| `empDamage` local before the clamp | 459 | 84 | 1984 |
| `damage` local before the clamp | 457 | 84 | 1984 |
| `const volatile int &mv = this->magnitude` in the k-loop | 460 | 86 | 1984 |
| `const volatile int &dv = this->damage` in the k-loop | 457 | 84 | 1984 |
| `const volatile int &ev = this->empDamage` in the k-loop | 458 | 84 | 1984 |
| `diff >= 1` / `diff != 0` | 457 / 1037 | 84 / 126 | 1984 / **1980** |
| `expect(magnitude > distance, 0)` (+ `diff` inside the `if`) | 462 | 78 | 1984 |
| `expect(diff > 0, 1)` | 1893 | 269 | **1908** |
| `if (!expect(diff > 0, 0)) continue;` | 457 | 84 | 1984 |
| `expr` scatter form (`VectorLength(enemyPosition - getPlayer()->getPosition())`) | 461 | 94 | 1984 |
| `expr` + `hitPositions` after the k-loop | 1858 | 333 | **1952** |
| `hitPositions[i]` / `*(data()+i)` / `enemy->data()[k]` / `td` reference | 457 | 84 | 1984 |
| `view` block: `Matrix &`/`Matrix *`/`const`/`reinterpret_cast`/`void *src` | 457 | 84 | 1984 |
| `view` block: `Matrix view; view = *(...)` | 1888 | 268 | **2056** |
| `view = MatrixSetTranslation(view, ...)` | 1097 | 107 | **1992** |
| `Matrix view;` at i-loop scope | 2024 (w13353) | — | 2024 |
| `Vector kpos` at i-loop/function/scatter scope (used in the k-loop) | 1818 / 1819 / 1690 | 324 / 304 / 285 | **1928 / 1972 / 2008** |
| `alignas(8)` / `alignas(16)` on the k-loop temp | 457 / 1863 | 84 / 236 | 1984 / **1996** |
| `volatile unsigned i` / `volatile int distance` | 513 / 937 | 89 / 118 | 1984 |
| `volatile` on `explosive`/`explodeOnContact`/`bomb`/`mine`/`scatter`/`radius` | 1860/1139/1895/1850/1818/1657 | — | 1972/1976/1944/2000/1992/1972 |
| k-loop `int`/`unsigned long`/`!=`/`i += 1`/`size() > k`/post-inc | 457 | 84 | 1984 |
| k-loop `while` / `do-while` / cached bound | 1750 / 1032 / 1713 | 259 / 156 / 291 | **1940 / 1988 / 1960** |
| `expect` on the k-loop / i-loop condition | 1800 / 457 | 367 / 84 | **1948 / 1984** |
| `expect(scatter, 0)` | 1759 | 359 | **1936** |
| `hitFlags`/`hitPositions` swap | 478 | 90 | 1984 |
| `hitFlags` + `hitPositions` after the k-loop | 1821 | 288 | **1904** |
| `impact` block after the scatter block | 1129 | 140 | **1952** |
| `setHitVector`/`lifetimes`/`lastHitKIPlayer` reorders | 486 / 473 | 90 | 1984 |
| `playerGun`/`field_54` reorder | 1152 | 128 | 1984 |
| `enemy->isAsteroid()` `if (!mine) {...} else continue` | 457 | 84 | 1984 |
| `Vector position`/`velocity`/`enemyPosition` declaration reorders | 472 / 540 / 511 | 107 / 128 / 123 | 1984 |
| `radius` declared before `enemyPosition` | 517 | 93 | 1984 |
| `KIPlayer *ki` hoisted above the radius block | 1742 | 320 | **1952** |
| `ki` local for the scatter radius `getKIPlayer()` pair | 1452 | 148 | **1976** |
| `(unsigned)` cast on the falloff divisor | 457 | 86 | 1984 |
| `const int diff` / `const int mag` local | 457 | 84 | 1984 |
| `int`/`unsigned` `radius` volatile | 1657 | 329 | **1972** |
| `impact` inner `if` removed | 1086 | 126 | **1980** |
| `empDamage != -1` (literal) | 1338 | 125 | **1976** |
| `view` block `memcpy` | 1923 | 249 | **2064** |
| `Matrix view` before the two `SetAnimationState` | 1163 | 155 | **1988** |
| `MatrixSetTranslation` result assigned back | 1097 | 107 | **1992** |

## 5. Reproduction

```
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN3Gun22calcCharacterCollisionEv
# raw=455 aligned=93 size=1984/1984
```

## Current-master harvest (wave 190)

Applied on master `a9c426e5a`. The reorder still wins: raw **457 -> 455**
(-2), aligned **84 -> 93** (+9), size exact 1,984 B. `.text`
**73,018 -> 73,016** (-2); `.ARM.extab` 603 and build-id 20 unchanged;
total (text+extab+build-id) **73,641 -> 73,639** (-2). `check` ->
RATCHET PASS.
