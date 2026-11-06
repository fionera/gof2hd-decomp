# `Gun::calcCharacterCollision` wave 122 (w7216): HEAD's 93.4 % is still
# the best compromise; a new 1984 B anchor (`isRocket`, 90.8 %) does not beat it

Worker: w7216. Base master `29cd275f6` (wave 121). Original:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).
Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump `0x152f90`, size `1984 B`,
650 instructions. Scratch: `/dev/shm/w7216*` (not tracked).

## 0. Result

| build | gate byte/linked | target pct | size |
|---|---:|---:|---:|
| before (HEAD `29cd275f6`) | **4506 / 4506** | **93.4000** | 1984 |
| after this pass | **4506 / 4506** | **93.4000** | 1984 |

No code change: analysis only. `check` after: `RATCHET PASS`, byte 4506/4524,
linked 4506/4524, size-exact 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0, avg 99.93 %. `git status --short` clean.

## 1. Residual map (unchanged from wave 119/121)

HEAD is still exactly the wave-121 shape: `this=r9, enemy=r8, td=r6, i=fp`,
`i*3=r5` (shipped `r4`), `playerPosition=r4/sp+104` (shipped `r4/sp+44`),
`delta=r5/sp+44` (shipped `r5/sp+104`), and the k-loop is not rotated.
The 43 unmatched instructions per side are the same five clusters:
`i*3` register (9), `pp`/`delta` slots (2), two extra `add r4,sp,#104`
(2 + branch-target shifts), view-block rematerialisation (4), k-loop
rotation (12).

## 2. New measurement: full A/B/C/D subset sweep

A = two-line expression form for the scatter distance
(`VectorLength(enemyPosition - getPlayer()->getPosition())`),
B = plain `if (distance < magnitude)` (drop the cold expect),
C = `playerGun` block moved after `setHitVector`,
D = `__builtin_expect(this->empDamage != kNoAttribute, 0)` size anchor.

| subset | pct | size | subset | pct | size |
|---|---:|---:|---|---:|---:|
| none (HEAD) | **93.4** | 1984 | AB | 65.0 | 1908 |
| A | 92.6 | 1984 | AC | 85.6 | 1980 |
| B | 64.5 | 1908 | AD | 84.4 | 1988 |
| C | 84.5 | 1980 | BC | 85.5 | 1980 |
| D | 85.2 | 1988 | BD | 63.4 | 1912 |
| ABC | 86.6 | 1980 | CD | 84.0 | 1984 |
| ABD | 63.9 | 1912 | ACD | 85.1 | 1984 |
| BCD | 89.0 | 1984 | ABCD (`emp0`) | 90.1 | 1984 |

No subset beats HEAD. This reproduces the wave-121 `emp0` result exactly.

## 3. New anchor sweep on the `emp0` shape without D (`ABC`, 86.6 %)

The wave-121 note used D (the `empDamage` expect) only as a 1984 B size
anchor; it also moves the main `damageEmp` block out of line. We swept
`__builtin_expect(cond, 0/1)` over 15 other conditions of the `ABC` base to
look for an anchor that restores 1984 B without moving that block.

| site | hint 0 pct/size | hint 1 pct/size |
|---|---:|---:|
| `this->impact->isRocket()` | **90.8 / 1984** | 86.6 / 1980 |
| `this->impact != nullptr` | 85.6 / 1988 | 86.6 / 1980 |
| `this->empDamage > 0` | 85.3 / 1984 | 84.9 / 1980 |
| `this->owner == nullptr || ...` | 83.5 / 1980 | 84.3 / 1980 |
| `bomb || mine` | 86.6 / 1980 | 86.6 / 1980 |
| `this->enemies == nullptr` | 86.6 / 1980 | 57.2 / 1916 |
| `!enemy->isActive()` | 80.0 / 1980 | 45.9 / 1880 |
| `enemy->getHitpoints() < 1` | 80.3 / 1980 | 47.1 / 1880 |
| `mine` (outer) | 76.9 / 1984 | 83.2 / 1980 |
| `enemy->isAsteroid()` | 57.2 / 1924 | 43.8 / 1968 |
| `explodeOnContact` | 62.5 / 1916 | 60.9 / 2000 |
| `this->playerGun` | 64.8 / 1920 | 65.0 / 1916 |
| `explosive` (emitManual) | 53.2 / 1932 | 59.5 / 1976 |
| `this->geometries != nullptr` | 59.7 / 1976 | 60.5 / 1900 |
| `scatter` (k-loop block) | 56.6 / 1924 | 61.3 / 1888 |

The best new anchor is `isRocket` hint 0: **90.8 % / 1984 B**. Its diff is
smaller than `emp0`'s (290 vs 479 zipped differences), but it still pays the
same five residuals: `explodeOnContact`/`td` spill slots transposed
(`sp+20`/`sp+24`), the two `ldr.w fp,[sp,#40]; ldr r6,[sp,#24]` reloads after
`isAsteroid` absent, `field_54[0]=1` in `fp` instead of `r1`, and the k-loop
not rotated. It does not beat HEAD's 93.4 %.

## 4. Other levers tried this wave (all 93.4 or worse)

* Explicit `Matrix tmp = view; MatrixSetTranslation(tmp, ...);
  TransformSetLocal(..., view);` on HEAD -> 93.4 (compiler folds it back to
  the implicit sret copy); with `TransformSetLocal(..., tmp)` -> 54.1.
  Same on `emp0` -> 90.1 / 50.7.
* `td` declaration/initialisation order (`td` before `lastHitKIPlayer`,
  `td` before the flags, `td` at the very top, `td` declared uninitialised
  at the top and assigned later, `td` as `Vector &`, no `td` local):
  93.4 on HEAD, 90.1 on `emp0`, or 70.6 when `td` is computed before the
  null check.
* `explosive`/`explodeOnContact` as `int`/`bool`, declaration swapped,
  `explosive = 0` initialiser: 90.1 on `emp0` (no codegen change).
* `volatile Vector playerPosition` / `const volatile` does not compile
  (Vector has a non-volatile copy constructor).
* All 14 non-A/B/C/D subset combinations above; `isRocket` anchor sweep.

## 5. Root cause / conclusion

The shipped function simultaneously needs:

1. `this=r9, enemy=r8, td=r6` (HEAD has this),
2. `i*3=r4`,
3. `playerPosition=r4/sp+44, delta=r5/sp+104`,
4. the k-loop rotated.

(2)+(3)+(4) are reachable together (`emp0`, 90.1 %), and a new 1984 B anchor
(`ABC + isRocket0`, 90.8 %) reaches them with a smaller diff, but the rotated
layout still perturbs the `explodeOnContact`/`td` spill slots and the two
`fp`/`td` reloads after `isAsteroid`. Those are MachineScheduler/StackColoring
artefacts that no source spelling tried in waves 106-122 moves back. HEAD
remains the best compromise because its instruction offsets are exactly the
shipped ones (same 1984 B size and same block layout everywhere except the
k-loop), and the verifier compares intra-function branch targets literally.

Best measured variant remains HEAD at **93.4000 % / 1984 B** (607/650).

## 6. Reproduction

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6        # RATCHET PASS
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN3Gun22calcCharacterCollisionEv
```

`ABC + isRocket0` source shape (relative to HEAD `src/game/weapons/Gun.cpp`):

```cpp
// scatter distance: two-line expression form
int distance = (int) AbyssEngine::AEMath::VectorLength(
        enemyPosition - ((Level *) this->level)->getPlayer()->getPosition());

// k-loop: plain if (drop the cold __builtin_expect)
if (distance < this->magnitude) { ... }

// post-damage order: field54, setHitVector, playerGun
reinterpret_cast<uint8_t *>(&enemy->field_54)[0] = 1;
enemy->setHitVector(-velocity.x, -velocity.y, -velocity.z);
if (this->playerGun) { ((Level *) this->level)->field_30 = 1; }

// 1984 B anchor (new this wave)
if (__builtin_expect(this->impact->isRocket(), 0)) {  // lint: arm_builtin
    reinterpret_cast<uint8_t *>(&enemy->field_54)[1] = 1;
}
```
