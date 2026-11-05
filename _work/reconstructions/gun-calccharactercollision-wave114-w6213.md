# `Gun::calcCharacterCollision` wave 114 (w6213): a named `playerPosition`
# temporary in the scatter-radius block moves the `sp+44`/`sp+104` temps into
# the shipped order and lifts the tuned body 90.0 % -> 92.8 % (1984 B)

Worker: w6213. Branch: `wave114-w6213`, base master `d8557f7e2`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).
Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump `0x152f90`, size `1984 B`, 650 insns.
Scratch: `/dev/shm/w6213/`.

## 0. Result

| build | gate byte/linked | target pct | matched | size |
|---|---:|---:|---:|---:|
| before (HEAD `d8557f7e2`) | **4500 / 4500** | **90.0000** | **585/650** | **1984** |
| after this pass | **4500 / 4500** | **92.7692** (report 92.8) | **603/650** | **1984** |

Gate after: byte 4500/4524, linked 4500/4524, size-exact 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0, `RATCHET PASS`. No ELF section / `.ARM.extab` change
(27 sections, `.text 0x1337c8`, `.ARM.exidx 0x6f70`, `.ARM.extab 0x26298`,
`.rodata 0x41c27` — the last is the pre-existing mission-table delta), file size 2,187,328 B.

## 1. The landed change

On top of the wave-114 working base (tail `F P H L K`, `hitFlags[i] = 1` as the last
statement of the inner `k` loop inside the `__builtin_expect(distance < magnitude, 0)`
block, `// lint: arm_builtin` on the same line) the only new edit is in the scatter-radius
block:

```cpp
            } else if (scatter) {
                Vector playerPosition = ((Level *) this->level)->getPlayer()->getPosition();
                int distance = (int) AbyssEngine::AEMath::VectorLength(enemyPosition - playerPosition);
```

instead of

```cpp
            } else if (scatter) {
                int distance = (int) AbyssEngine::AEMath::VectorLength(
                        enemyPosition - ((Level *) this->level)->getPlayer()->getPosition());
```

This is semantically identical (the `getPosition()` result is a by-value temporary either
way) and is a normal source shape. It changes the two stack temporaries of the subtraction
from `r4 = sp+44 / r5 = sp+104` to `r4 = sp+104 / r5 = sp+44`, which is the order the
shipped binary uses for the delta/`getPosition` pair, and it removes the argument-shuffle
`mov`s around `AEMath::mi`/`VectorLength` (instructions 110-119). It does **not** fix the
`i*3` register (still `r5` vs shipped `r4`).

`F` = `field_54[0] = 1`, `P` = `if (playerGun) level->field_30 = 1`,
`H` = `setHitVector`, `L` = `lifetimes[i] = -1000000`, `K` = `lastHitKIPlayer = getKIPlayer()`.

## 2. Measured tables (official verifier path; `link_so` + `tools/verify` scoring)

All variants below are on top of the wave-114 base unless stated otherwise. Score is the
`fast.py` full-precision positional ratio (identical metric to `report.json`, which rounds
to one decimal).

### 2.1 Scatter-radius temporary spelling

| variant | pct | size | matched |
|---|---:|---:|---:|
| **named `playerPosition` (landed)** | **92.7692** | 1984 | **603** |
| `const Vector playerPosition` | 92.7692 | 1984 | 603 |
| `const int distance` | 92.7692 | 1984 | 603 |
| named `playerPosition` + named `delta` | 92.7692 | 1984 | 603 |
| named `PlayerEgo *player` then `playerPosition` | 92.7692 | 1984 | 603 |
| named `playerPosition` declared after `radius` | 92.1538 | 1984 | 599 |
| `playerPosition` before `MatrixGetPosition` | 63.4749 | 1956 | 411 |
| baseline (inline temporary) | 92.0000 | 1984 | 598 |

### 2.2 Damage-form grid on the new base

| form | pct | size | matched |
|---|---:|---:|---:|
| `int dmg = this->damage;` + tree (landed) | **92.7692** | 1984 | **603** |
| `const int dmg` / `unsigned dmg` / `(int)` cast | 92.7692 | 1984 | 603 |
| `int dmg;` (uninitialised tree) | 91.3846 | 1984 | 594 |
| `int dmg` declared at function top, assigned at block | 92.7692 | 1984 | 603 |
| middle arm uses `dmg` instead of `this->damage` | 92.7692 | 1984 | 603 |
| `int dmg` before the `empDamage` block | 87.4519 | 1980 | 568 |
| `int dmg` at loop top / after `velocity` / after `radius` | 80.1533 | 1996 | 523 |
| `int dmg` after `getKIPlayer` | 68.2515 | 1988 | 445 |
| per-arm `int dmg` | 83.7423 | 2000 | 546 |
| direct `this->damage` in every arm | 83.7423 | 2000 | 546 |
| `int &dmg` / `const int &dmg` / `int *dmg` | 83.7423 | 2000 | 546 |
| `volatile int dmg` | 67.9012 | 1968 | 440 |

### 2.3 Tail permutations (120 measured)

| tail | pct | matched |
|---|---:|---:|
| **`F P H L K` (landed)** | **92.7692** | **603** |
| `F P H K L` / `F P L H K` | 92.3077 | 600 |
| `F P K H L` | 92.1538 | 599 |
| `K F P H L` | 92.0000 | 598 |
| `F P L K H` / `F P K L H` / `H F P L K` | 91.8462 | 597 |

### 2.4 `hitFlags[i] = 1` insertion-point sweep (22 line boundaries in the scatter block)

| insertion point | pct | size |
|---|---:|---:|
| first stmt of the `expect` block (before `falloff`) | 92.7692 | 1984 |
| before `if (falloff > 1.0f)` | 92.7692 | 1984 |
| before `if (empDamage > 0)` | 92.7692 | 1984 |
| inside `empDamage` body | 92.7692 | 1984 |
| **after the `empDamage` block (landed)** | **92.7692** | 1984 |
| after the `enemy->damage(...)` call | 86.8360 | 1980 |
| at the `if (scatter)` head (faithful to the binary's store site) | 85.6043 | 1980 |
| after `hitPositions` | 84.9885 | 1980 |
| after the `expect` block / k-loop tail | 76.5661 | 1968 |
| inside the `for` header region / before `k` loop | 73.6–76.6 | 1968–1976 |

Note: the shipped binary stores `hitFlags[i] = 1` once at the head of the `if (scatter)`
block (`0x153426`), not inside the `k` loop. The faithful placement measures 85.6 %;
the tuned placement (inside the `expect` block) is worth +7.2 points on this base.

### 2.5 `__builtin_expect` sites on the new base

All 10 tested sites (`mine` outer, `isActive`, `hitpoints`, `mine*5`, `ki->field_0x71`,
`distance > 20000`, `egoFlag/field_0x8c`, `outer`, `mine continue`, `explosive`) are
≤ 92.7692; `useCustomRadius` is codegen-identical. The two existing expects
(`box_x < r`, 1 — load-bearing anchor; `distance < magnitude`, 0) stay.

### 2.6 Loop/index spellings

| variant | pct |
|---|---:|
| explicit `i3 = i*3`, `i+0`, `int i`, `positions.size()`, `positions[i]` | 92.7692 |
| `Vector *posData/velData` hoisted | 92.4615 |
| `int e` outer counter | 92.6154 |
| `int k` inner counter / `hitFlags[i] = true` | 92.7692 |
| `hitPositions` store after the `k` loop | 58.8778 |
| cached `k < n` bound | 68.6728 |

## 3. Remaining residual (47 mismatched insns)

1. **`i*3` is `r5`, shipped `r4`** (and the `sp+44`/`sp+104` temps are consequently
   swapped). Six instructions (`add.w rX, fp, fp, lsl #1`, two indexed loads,
   `mov r6, rX`, the restore, and three later `add ... lsl #2`). Every spelling of the
   index tried (`i3` local, `int i`, pointer hoist, `positions[i]`) keeps `r5`.
2. **`hitFlags` store site** — shipped at the scatter-block head, landed inside the
   `expect` block; 3 instructions.
3. **`sl` holds `this->damage`** in the landed body (the `int dmg` hoist) while the shipped
   body reloads `[r9,#96]` per damage arm and uses `sl = sp+192` (the `position`
   address). Removing the hoist restores the shipped damage block but costs more
   elsewhere (`int dmg;` 91.38, direct 83.74).
4. **Literal-pool offset** — landed pool is 4 B closer than shipped (`[pc,#844]` vs
   `[pc,#848]`); three `ldr`/`vldr` immediates.

## 4. Reproduction

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6        # RATCHET PASS
```

Scratch helpers: `fast.py` (score), `cmp.py` (opcode diff), `sbs.py` (side-by-side),
`runv.py` (splice + `link_so` + score, with stale-.so detection), `base_A.cpp` (landed body).
