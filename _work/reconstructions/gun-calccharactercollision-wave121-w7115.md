# `Gun::calcCharacterCollision` wave 121 (w7115): the `playerGun` statement
# position is the lever that restores `enemy=r8/td=r6` in the rotated
# k-loop shapes; a shape with all four shipped properties at 1984 B exists
# (`emp0`, 90.1 %) but no shape beats HEAD's 93.4 %

Worker: w7115. Base master `bd061a489` (wave 120). Original:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).
Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump `0x152f90`, size `1984 B`,
650 instructions. Scratch: `/dev/shm/w7115/` (not tracked).

## 0. Result

| build | gate byte/linked | target pct | matched | size |
|---|---:|---:|---:|---:|
| before (HEAD `bd061a489`) | **4505 / 4505** | **93.4000** | **607/650** | 1984 |
| after this pass | **4505 / 4505** | **93.4000** | **607/650** | 1984 |

No code change: analysis only. `check` after: `RATCHET PASS`, byte 4505/4524,
linked 4505/4524, size-exact 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0, avg 99.94 %. `git status --short` clean.

## 1. The shipped map vs HEAD (re-measured)

| property | shipped | HEAD |
|---|---|---|
| `this` | r9 | r9 |
| `enemy` | r8 | r8 |
| `td` (`&this->targetDir`) | r6 | r6 |
| `i` (projectile loop) | fp | fp |
| `i*3` | **r4** | **r5** |
| `playerPosition` (`pp`) | **r4 / sp+44** | r4 / sp+104 |
| `delta` (`enemyPosition - pp`) | **r5 / sp+104** | r5 / sp+44 |
| k-loop layout | **rotated** `[body,then,latch,header]` | `[header,body,latch]` |
| k-loop `&enemy->getPosition()` sret | rematerialised `0x153444` | hoisted to i-loop preheader |

Structural diff of the two 650-instruction streams: 43 unmatched each side,
exactly the clusters already documented in
`gun-calccharactercollision-wave119-w6940.md` (i*3 9, pp/delta 2, two
`add r4,sp,#104` 2, view-block remat 4, k-loop rotation 12, branch targets
the rest).

## 2. New finding: the `playerGun` block position is the enemy/td lever

The rotated k-loop shapes previously known (wave 119) all lost `enemy=r8`,
`td=r6`. Moving the `this->playerGun` block **after** `enemy->setHitVector`
restores them in every rotated shape tried:

```cpp
reinterpret_cast<uint8_t *>(&enemy->field_54)[0] = 1;
enemy->setHitVector(-velocity.x, -velocity.y, -velocity.z);
if (this->playerGun) {
    ((Level *) this->level)->field_30 = 1;
}
```

(the shipped source order is field54 / playerGun / setHitVector, so this is a
codegen-shape reorder, not a semantic change). With this reorder the rotated
variants stop swapping `enemy`/`td` and stop moving `&enemy->radius` out of
`sl`.

## 3. A shape with all four shipped properties at 1984 B (`emp0`, 90.1 %)

Base: the two-line expression form (w6928 §2.1) for the scatter distance

```cpp
int distance = (int) AbyssEngine::AEMath::VectorLength(
        enemyPosition - ((Level *) this->level)->getPlayer()->getPosition());
```

plus the `playerGun` reorder above, plus a plain `if (distance < magnitude)`
k-loop (no cold `__builtin_expect`), plus

```cpp
if (__builtin_expect(this->empDamage != kNoAttribute, 0)) { ... }
```

gives **all four shipped properties at once**:

```
this=r9 enemy=r8 td=r6 i3=r4 pp=r4/sp+44 delta=r5/sp+104 kloop=rotated
size=1984  pct=90.1  nmatch=585/650
```

The `empDamage` expect is only a size/offset anchor: it restores the 1984 B
size (without it the same shape is 1980 B / 86.6 %). The remaining structural
diff is small but persistent:

* `explosive`/`td` spill slots are transposed (`[sp,#20]` vs `[sp,#24]`),
* the two `ldr.w fp,[sp,#40]; ldr r6,[sp,#24]` reloads after `isAsteroid`
  (`0x153196`, `0x153260`) are absent; ours reloads `fp`/`td` later and has two
  extra `ldr r6,[sp,#20]`,
* `field_54[0]=1` uses `mov.w fp,#1` instead of `movs r1,#1`,
* the `empDamage` expect changes the branch polarity of that block.

Those five items keep `emp0` at 90.1 %, below HEAD's 93.4 %: HEAD wins because
its instruction **offsets** are exactly the shipped ones (same 1984 B size and
same block layout everywhere except the k-loop), and the verify score compares
intra-function branch targets literally.

## 4. Measurements this wave (official verifier path)

Format: `pct / size / this,enemy,td,i3 / pp,delta / k-loop`.

| variant | pct | size | map |
|---|---:|---:|---|
| HEAD | **93.4** | 1984 | r9,r8,r6,r5 / r4+104, r5+44 / top |
| expression form (F) | 92.6 | 1984 | r9,r8,r6,r5 / r5+44, r4+104 / top |
| F + plain k + `expect(scatter,1)` | 81.6 | 1980 | r9,**r6**,**r8**,r4 / r4+44, r5+104 / rotated |
| F + `playerGun` reorder (F_pgnew) | 85.6 | 1980 | r9,r8,r6,**r4** / **r4+44, r5+104** / top |
| F_pgnew + plain k | 86.6 | 1980 | r9,r8,r6,r4 / r4+44, r5+104 / rotated |
| F_pgnew + plain k + `expect(scatter,1)` | 86.9 | 1984 | r9,r8,r6,**r6** / r4+44, r5+104 / rotated |
| **`emp0`** (§3) | **90.1** | **1984** | **r9,r8,r6,r4 / r4+44, r5+104 / rotated** |
| `emp0` + `expect(empDamage==kNoAttribute,1)` | 80.3 | 1980 | r9,r8,r5,r4 / r4+44, r6+104 |
| `emp0` + HEAD scatter form | 89.0 | 1984 | r9,r8,r6,r4 / r5+104, r4+44 / rotated |
| `emp0` + cold k expect | 85.1 | 1984 | r9,r8,r6,r4 / r4+44, r5+104 / top |
| `emp0` + `expect(mine,0)` | 82.5 | 1988 | r9,r8,r6,r6 / r4+44, r5+104 / rotated |
| `do { } while(k<size)` k-loop | 83.9 | 1988 | r9,r8,r6,r5 / r4+104, r5+44 / rotated |
| `while(true){ ...; if(++k>=size) break; }` | 83.9 | 1988 | r9,r8,r6,r5 / r4+104, r5+44 / rotated |
| `for(;;){ if(k>=size) break; ... }` | 93.4 | 1984 | identical to HEAD |

### 4.1 Statement-order permutations (HEAD body, A=field54, B=playerGun,
### C=setHitVector, D=lifetimes, E=lastHitKIPlayer)

| order | pct | size | map |
|---|---:|---:|---|
| `ABCDE` (HEAD) | **93.4** | 1984 | r9,r8,r6,r5 / r4+104, r5+44 / top |
| `BACDE` | 86.9 | 1984 | r9,r8,r6,r5 / r4+104, r5+44 / top |
| `ACBDE` (= F_pgnew on HEAD) | 84.5 | 1980 | r9,r8,r6,**r4** / r5+104, r4+44 / top |
| `ABDCE` / `ABDEC` / `ABCED` | 92.9 / 92.5 / 92.9 | 1984 | HEAD map |
| `DABCE` / `ADBCE` | 91.2 | 1984 | HEAD map |
| `CABDE` | 92.5 | 1984 | HEAD map |
| `AEBCD` / `ADEBC` | 86.0 / 84.9 | 1984 | i3=r6 |

No permutation of `A..E` reaches `i3=r4` with `enemy=r8` and the HEAD scatter
slots; the only ones that reach `i3=r4` (the `ACBDE` family) transpose the
scatter registers.

### 4.2 Other levers tried (all 93.4 or worse)

* scatter forms: HEAD, one-line expression, two-line expression, named
  `delta`, `delta` declared first, `delta = enemyPosition` + `-=`,
  `VectorLength(operator-(...))`, `auto`, `float len`, `(void)&pp`,
  `Vector *ppp = &pp`, pp at function scope.
* k-loop forms: `for`/`while`/`do`/`for(;;)`, `k != size`, cached `size()`,
  `int`/`unsigned`/`size_t`, `k++`/`++k`, separate `other` pointer, direct
  `(*this->enemies)[k]`, `Vector ep` temp, `*td = ep - position`.
* `td` as pointer / reference / member access / declared inside the k-loop /
  declared at the top.
* loop index types (`int`, `unsigned`, `size_t`, declared at function scope),
  `i != count`, post-increment.
* `explosive`/`explodeOnContact` as `int`/`bool`, declaration order, `td`
  before them, `field_54` written as `uint16_t` / byte pointer / local `one`.
* ~40 `__builtin_expect` placements (isActive, hitpoints, mine, egoFlag,
  empDamage, owner, impact, geometries, explosive, scatter, isAsteroid,
  outerCollide, deadFlag, field_0x71, mbox polarity, loop conditions) x {0,1}
  on both HEAD and the rotated shapes.

## 5. Root cause / conclusion

The shipped function requires, simultaneously:

1. `this=r9, enemy=r8, td=r6` (HEAD has this),
2. `i*3=r4`,
3. `pp=r4/sp+44, delta=r5/sp+104`,
4. the k-loop rotated.

(2)+(3)+(4) are reachable together (the `emp0` shape, and `F_pgnew` without
rotation), but the rotated layout also perturbs the `explosive`/`td` spill
slots and the placement of the two `fp`/`td` reloads after `isAsteroid`.
Those are MachineScheduler/StackColoring artefacts that no source spelling
tried this wave (or in waves 106-119) moves back. HEAD remains the best
compromise: it keeps the shipped instruction offsets (1984 B, same block
layout) and pays only the `i*3` register and the `pp`/`delta` slot transpose,
which the SequenceMatcher score tolerates better than the rotated layout's
offset shifts.

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

`emp0` source shape (relative to HEAD `src/game/weapons/Gun.cpp`):

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

// size anchor
if (__builtin_expect(this->empDamage != kNoAttribute, 0)) { ... }
```
