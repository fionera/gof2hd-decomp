# `Gun::calcCharacterCollision` wave 189 (w13365): statement-order pass —
# swapping the scatter-loop `damage`/`damageEmp` pair is a size-exact raw win
# (458 -> 457, `.text` 73554 -> 73553)

Worker: w13365. Branch `wave189-w13365`, base master `30caae150`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump `0x152f90`, size `1984 B`,
660 insns. Scratch: `/home/debian/GalaxyOnFire2/scratch/w13365`.

## 0. Result

| build | raw | aligned | size | `.text` diff |
|---|---:|---:|---:|---:|
| before (HEAD `30caae150`) | **458** | **63** | 1984/1984 | 73554 |
| after this pass | **457** | 84 | 1984/1984 | **73553** |

`check` after (`-j2`, `ulimit -v 12000000`): `RATCHET PASS`, byte_exact
4507/4524, linked_exact 4507/4524, size-exact 4524/4524, stub_zero_size 0,
missing 0, wrong_type 0, extra 0, data 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370. `.rel.plt` byte-identical (cmp), `.ARM.extab`
content diff unchanged at 610 B (size/base exact `0x1a3b78`, 156,312 B), all
27 section headers unchanged, file size 2,187,328 exact.

## 1. Change

In the scatter k-loop body, move the `empDamage` block above the plain
`damage` call:

```c++
-                        enemy->damage((int) (falloff * (float) this->damage), this->friendGun, this->itemIndex);
                         if (this->empDamage > 0) {
                             enemy->damageEmp((int) (falloff * (float) this->empDamage), this->friendGun);
                         }
+                        enemy->damage((int) (falloff * (float) this->damage), this->friendGun, this->itemIndex);
```

The two calls are independent (both consume `falloff`, different callees), so
this is a pure statement-order lever.  The shipped body emits `damage` then
`damageEmp` (`0x1534d0` / `0x1534f6`), so this is *not* the faithful order; it
is a layout tweak that wins one byte on the metric.

## 2. Why one byte

The k-loop is not rotated in our body (the wave-189 w13353 RA note), so the
`damage`/`damageEmp` blocks sit after the out-of-line latch.  Reversing them
moves the 34-byte `damage` block and the 38-byte `damageEmp` block relative to
the 2-byte-shifted `&view` region; the swapped layout happens to land one more
byte on the shipped encoding.  Byte-diff groups in the k-loop shrink from
`24+14+1+39+22 = 100` (HEAD) to `24+15+6+15+12+1+26 = 99` (this pass); the
`i*3` register, `pp`/`delta` slot swap, `&view` remat cascade and view-block
remat are untouched.

## 3. Lens: statement order (new this wave)

w13353 exhausted the RA/rematerialisation lens (~120 cells).  This pass used
statement order and re-tested none of its cells.

New cells measured this wave (all `link_so` + `tools/fn_diff.py`, `-j2`):

| variant | raw | aligned | size |
|---|---:|---:|---:|
| HEAD | **458** | 63 | 1984 |
| **`empDamage` block before `damage` (this pass)** | **457** | 84 | 1984 |
| same + `diff >= 1` | 457 | 84 | 1984 |
| same + `empDamage >= 1` | 457 | 84 | 1984 |
| same + `const Vector position` / `velocity` / `const Vector &pp` | 457 | 84 | 1984 |
| same + `td`/`lastHitKIPlayer` declaration swap | 457 | 84 | 1984 |
| same + clamp `if/else` polarity reversed | 459 | 92 | 1984 |
| same + `SetAnimationState` 3/1 swap | 459 | 86 | 1984 |
| same + `named delta` / `expr` grouping | 461 | 94 | 1984 |
| same + `hitFlags`/`hitPositions` swap | 483 | 91 | 1984 |
| same + `isActive`/`getHitpoints` swap | 469 | 92 | 1984 |
| same + `mine` inner-if swap | 483 | 91 | 1984 |
| same + `impact` block moved inside `scatter` | 1129 | 140 | 1952 |
| `damage`/`damageEmp` block kept, `SetAnimationState` 3/1 swap | 460 | 65 | 1984 |
| `hitFlags`/`hitPositions` swap (base) | 479 | 69 | 1984 |
| `isActive`/`getHitpoints` swap (base) | 470 | 71 | 1984 |
| `mine` inner-if swap (base) | 484 | 70 | 1984 |
| `lastHitKIPlayer`/`td` declaration swap (base) | 458 | 63 | 1984 |
| `explosive`/`explodeOnContact` assignment swap (base) | 1839 | 201 | 1968 |
| `impact` block moved inside `scatter` (base) | 512 | 80 | 1984 |
| full 24-permutation sweep of the i-loop prefix (`position`, `velocity`, `enemyPosition`, `radius`) | `0123` = 458 best; next 473 | — | 1984 |
| k-loop body `*td = getPosition() - position` / named `ep` / `ep = getPosition() - position` | 1790 / 1880 / 1789 | — | 1956 / 1876 / 1952 |

Also re-confirmed codegen-identical (458/63/1984): `positions.size()`,
`i++`/`e++`/`k++`, `unsigned int`, `i += 1`, `i <= count-1`, `count != i`,
`0U`, `const`/`volatile const` locals, `!= nullptr` -> `!= 0` / truthy,
`(float)` cast placement, `radius *= 5` -> `5 * radius`, `0.5f`/`1.5f`/`2`/`3`
multiplication reorders, `-1000000` -> `-1000*1000`, `1` -> `true`.

## 4. Reproduction

```
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN3Gun22calcCharacterCollisionEv
# -> raw=457 aligned=84 size=1984/1984 insn=660/660
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

## 5. Files changed

`src/game/weapons/Gun.cpp` (the two-line move above) and this note.

## Current-master rebase (wave 190 harvest)

Re-applied on current master (`454bc033e`). Current master before this change was
raw 458; after it is raw 457 (-1), aligned 63 -> 84, size 1,984/1,984. `.text`
73,302 -> 73,289 (-13 across LevelScript + Radar + Gun), `.ARM.extab` 603
unchanged, total 73,925 -> 73,912. `check` green, RATCHET PASS.
