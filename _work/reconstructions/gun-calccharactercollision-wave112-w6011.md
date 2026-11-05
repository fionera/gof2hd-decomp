# `Gun::calcCharacterCollision` wave 112 (w6011): the `F P H L K` corner reaches
# the shipped prologue + tail + `td` map at 88.0 %, but `i*3` still lands in `r5`;
# HEAD's tuned `F H K L P` body stays the maximum at 90.0 %

Worker: w6011. Branch: `wave112-w6011`, base master `5d98bd391`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).
Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump `0x152f90`, size `1984 B`, 650 insns.
Scratch: `/dev/shm/w6011/`.

## 0. Result

**No code change landed.** HEAD (`src/game/weapons/Gun.cpp`: `int dmg = this->damage` +
four `enemy->damage(...)` sites + tail `F H K L P`) is still the best measured shape:
**90.0 %**, 1984/1984 B (`report.json`: `pct 90.0, byte False, linked False`).

| build | gate byte/linked | target pct | target size | target matched |
|---|---:|---:|---:|---:|
| before (HEAD `5d98bd391`) | **4500 / 4500** | **90.0** | **1984** | **424/650** |
| after this pass (note only) | **4500 / 4500** | **90.0** | **1984** | **424/650** |

Gate after: byte 4500/4524, linked 4500/4524, size-exact 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0, `RATCHET PASS`. No ELF section / `.ARM.extab` change
(27 sections, `.ARM.extab` `0x26294`, file size 2,187,328 B).

`F` = `field_54[0] = 1`, `P` = `if (playerGun) level->field_30 = 1`,
`H` = `setHitVector`, `L` = `lifetimes[i] = -1000000`, `K` = `lastHitKIPlayer = getKIPlayer()`.

## 1. The oracle confirms the real source shape

`/home/debian/GalaxyOnFire2/_work/oracles/Galaxy-on-Fire-2-Unity-Remake/Reference/decompiled/native/game/Gun.c`
(`Gun::calcCharacterCollision` @ `0x0017e154`, 1968 B, KiritoJPK clang-3.8 `-O2`) shows the
real source unambiguously (this re-confirms waves 101/103):

* **phi damage form**: `dmg` is assigned in each arm and there is a *single*
  `Player::damage(enemy, dmg, friendGun, itemIndex)` after the merge;
* tail order **`F P H L K`** (field_54[0]=1, playerGun, setHitVector, lifetimes[i],
  lastHitKIPlayer);
* **no `td` local**: the scatter loop writes `this->targetDir` directly
  (`Vector::operator=(this_01, …)` where `this_01 = this + 0xd8`); the pointer is a
  compiler materialisation, so `Vector *td`, `Vector &td` and direct `this->targetDir` are
  all source-equivalent (measured: identical codegen, §3.2);
* plain `for` loops.

The shipped Android binary agrees on every one of those points (it reloads `this->damage` at
all three sites `0x1532b8`/`0x1532d4`/`0x153308`, has the `F P H L K` order at
`0x153316`…`0x153350`, and keeps `td` in `r6`).

## 2. New this pass: the `F P H L K` corner at 88.0 % reproduces the *shipped* prologue,
## tail and `td` map — only `i*3` and the `fp=i` join reload are wrong

`tree` damage form + `F P H L K` + `expect(distance < this->magnitude, 0)` +
`expect(this->empDamage != kNoAttribute, 0)` on top of the existing
`expect(box_x < r, 1)`:

| property | shipped | this variant |
|---|---|---|
| size | 1984 | **1984** |
| tail order | `F P H L K` | **`F P H L K`** |
| `explosive` slot | `sp+20` | **`sp+20`** |
| `td` slot | `sp+24` | **`sp+24`** |
| `this` | `r9` | **`r9`** |
| `enemy` | `r8` | **`r8`** |
| `td` | `r6` | **`r6`** |
| `td` join reload | yes (`0x15319c`/`0x153266`) | **yes** |
| `i*3` | `r4` | **`r5`** |
| `fp=i` join reload | yes (`0x153198`/`0x153262`) | **no** |
| pct | — | **88.0** (350/650 positional) |

So the shipped prologue (`mov r6,r9; str.w r2,[r6],#216; str r2,[sp,#20]; str r6,[sp,#24]`)
is reproduced instruction-for-instruction (same registers and slots; only the PC-relative
literal-pool offsets differ by 16 B), and the tail is in the shipped order. The residual is exactly
the two `ldr.w fp,[sp,#40]` join reloads plus the `r4`/`r5` swap of `i*3` and the scatter
temp pointer (the whole first half of the function then differs only by the 16-byte literal
pool offset).

The `expect(box_x < r, 1)` anchor is load-bearing: removing it drops this variant
**88.0 → 79.8** (and 1984 B stays).

## 3. Measured tables (official verifier; `link_so` + `tools/verify/verify.py` scoring)

### 3.1 All 120 tail permutations × `expect(empDamage != kNoAttribute, 0)` (tree body)

Best non-HEAD results:

| tail | pct | size | `i3` | `expl` | `td` | matched |
|---|---:|---:|---|---|---|---:|
| `P H K F L` | **88.9** | 1984 | r4 | 24 | 20 | 361/650 |
| `P K H F L` | 88.6 | 1984 | r4 | 24 | 20 | 361/650 |
| `F H P L K` / `F H K L P` | 84.4 | 1984/1988 | r4 | 24 | 20 | 164/165 |
| `F P H L K` (shipped order) | 61.3 | 1916 | r0 | 20 | – | 40 |
| **`F H K L P` (HEAD, no expect)** | **90.0** | **1984** | **r4** | **24** | **20** | **424** |

Every tail that keeps `this=r9` and `i3=r4` also keeps `expl=24 / td=20`; the shipped slot
order only appears with `i3∈{r5,r6}`.

### 3.2 `td` spelling is irrelevant (tree body)

`Vector *td = &this->targetDir` / `Vector &td = this->targetDir` / no local at all
(`this->targetDir = …; this->targetDir -= position; VectorLength(this->targetDir)`) are
**codegen-identical** on both `F H K L P` (90.0) and `F P H L K` (64.8 / 88.0 with the
expects). This confirms the oracle: the real source has no `td` local.

### 3.3 `F P H L K` expect sweep (tree body, 12 sites × subsets)

| variant | pct | size | `i3` | `join_td` | `expl` | `td` |
|---|---:|---:|---|---|---|---|
| `expect(scatter_mag,0)` + `expect(emp,0)` | **88.0** | 1984 | r5 | T | 20 | 24 |
| `expect(scatter_mag,0)` | 84.8 | 1980 | r5 | T | 20 | 24 |
| `expect(mine,0)` | 85.5 | 1984 | r4 | F | 20 | – |
| `expect(mine,0)` + `expect(owner,0)` | 81.9 | 1980 | r4 | F | 20 | – |
| `expect(mine,0)` + `expect(scatter_mag,0)` | 82.3 | 1984 | r6 | T | 20 | 24 |
| no expect | 64.8 | 1912 | r0 | F | 20 | – |

The 12-site exhaustive subset sweep (299 builds) and an 18-site single-expect sweep on top of
the 88.0 variant (36 builds) both peak at **88.0**; a 400-sample random expect search
(seeded 6011) peaks at **85.5** and never produces `join_fp=True`.

### 3.4 Structural variants on the 88.0 base (all ≤ 88.0)

`const unsigned i3 = i*3` local (88.0, identical), `const Vector position/velocity` (88.0,
identical), `unsigned radius` (87.1), `int` loop counters (87.8), `mine != 0` (88.0),
`int radius = enemy->radius; if (useCustomRadius) …` (88.0), radius before enemyPosition
(87.4), enemyPosition before velocity (84.9), position/velocity swapped (84.8),
`float distance` (54.8), `view = MatrixSetTranslation(...)` (79.1).

### 3.5 Damage-form grid (3 forms × 2 tails × 3 `td` spellings × 6 expect sets = 108 builds)

| damage form + tail | best pct | note |
|---|---:|---|
| tree + `F H K L P` (HEAD) | **90.0** | 1984 B, `expl=24/td=20` |
| tree + `F P H L K` + scat_mag+emp | 88.0 | 1984 B, `expl=20/td=24` |
| phi + `F H K L P` + emp | 87.9 | 1984 B |
| phi + `F P H L K` + mine+ki71+scat_mag (wave-109 `faithful2`) | 87.6 | 1984 B, `i3=r6` |
| phi + `F P H L K` + scat_mag+emp | 82.3 | 1980 B |
| direct + `F P H L K` + scat_mag | 83.7 | 1996 B |
| hybrid (dmg init + single call) + `F H K L P` | 84.1 | 1980 B |

## 4. Why the shipped map is still out of reach

The shipped `fp` holds `i` from the `isAsteroid` join (`0x153198`/`0x153262`) through the
`L` store, and `td` in `r6` from the same join. Both are *spill reloads hoisted to the
join block*. In every `F P H L K` source shape that restores the shipped slots the allocator
places the same reloads one block later — at the damage-block entry (`0x153274`) and at the
post-geometry reload (`0x1533e6`) — and colours `i*3` to `r5`. Conversely, every shape that
keeps `i*3=r4` keeps the shared `1` constant in `fp` (so `F`/`P` are separated by the `H`
and `K` calls) and therefore has `expl=24 / td=20`.

This is the same greedy-allocator anti-correlation reported in waves 107–111, now pinned to
the exact instruction pair: the two `ldr.w fp,[sp,#40]` join reloads are the whole
difference between 88.0 and the shipped layout. 88.9 (`P H K F L` + emp) is the best
non-HEAD tail, but it still has `expl=24 / td=20`.

## 5. Reproduction

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
# report.json -> {"name":"_ZN3Gun22calcCharacterCollisionEv","pct":90.0,"byte":false,"linked":false}
```

Scratch harnesses (`/dev/shm/w6011/`): `gen2.py` (damage form × tail × expect generator),
`gen3.py` (3 damage forms × 2 tails × 3 `td` spellings × 6 expect sets), `sweep2.py`
(18-site expect sweep on the 88.0 base), `sweep_tails_emp.py` (120 tails × emp),
`sweep_phkfl.py` (second expect on `P H K F L`), `batch3.py`–`batch8.py` (structural
variants), `score.py` / `sig.py` (official-verifier scorer + 5-field register signature),
`try.py` (splice body + `link_so` + score), `ndiff.py` (normalised instruction diff).

## 6. Files changed

None under `src/` (note only).
