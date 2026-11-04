# `Gun::calcCharacterCollision` wave 108 (w5616): the asteroid-join `fp` reload is
# anti-correlated with `i*3 = r4`; 840 tail×expect variants, no source shape has both

Worker: w5616. Branch: `wave108-w5616`, base master `8f98ec4d6`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).
Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump `0x152f90`, size `1984 B`, 650 insns.
Scratch: `/dev/shm/w5616/`.

## 0. Result

**No code change landed.** The tree body (HEAD: `int dmg = this->damage` + four
`enemy->damage(...)` call sites + tail `F H K L P`) is still the best measured shape:
**90.0 %**, 1984/1984 B, 585/650 normalized instructions (official verifier).

| build | gate byte/linked | target pct | target size | target matched |
|---|---:|---:|---:|---:|
| before (HEAD `8f98ec4d6`) | **4500 / 4500** | **90.0** | **1984** | **585/650** |
| after this pass (note only) | **4500 / 4500** | **90.0** | **1984** | **585/650** |

Gate after: byte 4500/4524, linked 4500/4524, size-exact 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0, `RATCHET PASS`. No ELF section / `.ARM.extab` change
(no code change; 27 sections, `.ARM.extab` `0x26298` untouched).

`F` = `field_54[0] = 1`, `P` = `if (playerGun) level->field_30 = 1`,
`H` = `setHitVector`, `L` = `lifetimes[i] = -1000000`, `K` = `lastHitKIPlayer = getKIPlayer()`.

## 1. What this pass added: a machine-readable reload/register signature

Prior passes reasoned from full diffs. This pass reduced the target to a 5-field signature
extracted from the built `.so`, so a sweep can ask "does this variant reproduce the shipped
*reload placement*?" directly instead of eyeballing 650 instructions:

| field | how it is read |
|---|---|
| `i3` | register of the first `add.w rX, fp, fp, lsl #1` (the `i*3` word index) |
| `join_fp` | `ldr r0,[sp,#36]` immediately followed (≤2 insns) by `ldr.w fp,[sp,#40]` |
| `join_td` | same `ldr r0,[sp,#36]` followed (≤2 insns) by `ldr r6,[sp,#24]`/`[sp,#20]` |
| `expl` | stack offset of the `explosive` spill (`20` shipped, `24` in HEAD) |
| `size` | function size |

Shipped signature: `i3=r4 join_fp=True join_td=True expl=20 size=1984`.
HEAD signature:     `i3=r4 join_fp=False join_td=False expl=24 size=1984`.

The shipped asteroid join is

```
153196: ldr r0,[sp,#36]      ; mine
153198: ldr.w fp,[sp,#40]    ; i     <- join_fp
15319c: ldr r6,[sp,#24]      ; td    <- join_td
15319e: cmp r0,#0
1531a0: bne 15325a
```

and the same pair reappears at the non-asteroid join (`0x153262`/`0x153266`). HEAD reloads
the same two values later, at the `explosive`/`geometries` diamond (`0x153366`/`0x15336a`
and `0x153372`/`0x153376`), so the *count* of reloads is identical (5 `fp` + 3 `td` in both)
and the 1984 B size matches; only the placement (and therefore every later branch target)
differs.

## 2. The new negative result: `join_fp` and `i3 = r4` never co-occur

840 variants were compiled and signature-scanned: 7 `(damage shape, expect set)` bases × all
120 tail permutations of `{F,P,H,L,K}`:

| base | expects | best pct | best tail | `i3` | `join_fp` | `join_td` |
|---|---|---:|---|---|---|---|
| tree | — | **90.0** | `FHKLP` | r4 | False | False |
| tree | `distance=0` | 88.6 | `KPFHL` | **r5** | False | **True** |
| tree | `mine_radius=0` | 81.8 | — | r6 | False | False |
| tree | `mine_radius=0,distance=0` | ≤ 85 | — | r6 | False | False |
| oracle | — | ≤ 84 | — | r4/r6 | False | False |
| oracle | `distance=0` | 83.3 | `FPHLK` | r5 | False | True |
| oracle | `mine_radius=0,distance=0` | 87.5 | `PFHLK` | r6 | False | True |

**Not one of the 840 variants has `i3=r4` together with `join_fp=True`.** The two properties
are anti-correlated in every reachable source shape: every variant that produces the shipped
`fp` reload at the join moves `i*3` out of `r4` (to `r5` or `r6`), and every variant that
keeps `i*3 = r4` (HEAD, and the `oracle+mine0+impact1+isRocket0` body) keeps `i` in the
spill slot through the join. The shipped binary has both.

## 3. Measured table (official verifier; `link_so` + `tools/verify/verify.py` scoring)

| variant | pct | size | `i3` | `join_fp` | `join_td` | `expl` |
|---|---:|---:|---|---|---|---|
| **tree, tail `F H K L P` (= HEAD)** | **90.0** | **1984** | **r4** | False | False | 24 |
| tree, tail `F K H L P` / `K F H L P` | 89.7 | 1984 | r4 | False | False | 24 |
| tree, `expect(isRocket,0)`, tail `P H K F L`, `hitFlags[i]=true` | 89.5 | 1984 | r4 | False | False | 24 |
| tree, `expect(empdmg,1)`, tail `F H K L P` | 89.5 | 1984 | r4 | False | False | 24 |
| tree, `expect(distance,0)`, tail `K P F H L` | 88.6 | 1984 | r5 | False | **True** | 20 |
| tree, `expect(distance,0)+expect(impact,1)+expect(isRocket,0)`, `F P H L K` | 88.2 | 1984 | r5 | False | True | 20 |
| tree, `expect(owner,1)`, `F H K L P` | 88.0 | 1984 | r4 | False | False | 24 |
| **oracle dmg + `expect(mine,0)+expect(distance,0)+expect(ki->field_0x71,0)`, `F P H L K`** | **87.6** | **1984** | r6 | False | **True** | 20 |
| tree, `expect(distance,0)`, `F P H L K` | 84.8 | 1980 | r5 | False | True | 20 |
| oracle + `expect(mine,0)+expect(impact,1)+expect(isRocket,0)`, `F P H L K` | 84.1 | 1984 | r4 | False | False | 20 |
| tree + `expect(mine,0)`, `F P H L K` (= w5317's closest map) | 85.5 | 1984 | r4 | False | False | 20 |
| tree + `expect(mine,0)`, `F P H L K`, `td` declared before `lastHitKIPlayer=0` | 85.5 | 1984 | r4 | False | False | 20 |
| tree + `expect(mine,0)`, `F P H L K`, extra `*td = this->targetDir;` in radius branch | 81.7 | 1992 | r4 | False | False | 20 |
| tree, `bool asteroid = enemy->isAsteroid(); if (asteroid)` | 90.0 | 1984 | r4 | False | False | 24 |
| tree, `if (enemy->egoFlag \|\| (...)) {} else continue;` (inverted guard) | 90.0 | 1984 | r4 | False | False | 24 |
| tree, `if (explosive) {...} if (!explosive && geometries) {...}` (split, not else-if) | 90.0 | 1984 | r4 | False | False | 24 |
| tree, `} else { if (geometries) {...} }` (nested) | 90.0 | 1984 | r4 | False | False | 24 |
| tree, `for (int k …)` in scatter loop | 90.0 | 1984 | r4 | False | False | 24 |
| tree, `i += 1` instead of `++i` | 90.0 | 1984 | r4 | False | False | 24 |
| tree, `expect(ki71,1)` / `expect(playerGun,1)` / `expect(isRocket,1)` | 90.0 | 1984 | r4 | False | False | 24 |
| tree, `int outer` | 84.0 | 1980 | r4 | False | False | 24 |
| tree, `bool hit = …; if (!hit)` | 83.7 | 1988 | r4 | False | False | 24 |
| tree, `n = positions.count` cached in the loop header | 82.0 | 1988 | r4 | False | False | 24 |
| tree, `positions.data()`/`velocities.data()` cached above the loop | 74.1 | 1992 | r4 | False | False | 24 |
| tree, `hitPositions` store before `hitFlags` | 75.6 | 1964 | — | False | False | — |
| tree, scatter loop uses a fresh `Player *other` | 55.8 | 1888 | — | False | False | — |
| tree, `float radius` | 46.5 | 1904 | — | False | False | — |
| tree, `if (!isAsteroid) { mine… } else { asteroid… }` (blocks swapped) | 49.6 | 1920 | — | False | False | — |
| tree, `td` declared at function top (before the null check) | 69.2 | 1968 | r4 | False | False | 20 |
| tree, `td = &targetDir;` inside the inner loop | 61.8 | 1948 | — | False | False | — |

### 3.1 Single-site `__builtin_expect` sweep on HEAD (`F H K L P`)

Every site × {0,1}; only the codegen-identical ones (90.0) and the best losers are shown.

| site | 0 | 1 |
|---|---:|---:|
| `mine` (radius branch) | 81.8 / 1992 | 69.5 / 2000 |
| `distance < magnitude` (scatter radius) | 69.3 / 1944 | 85.2 / 1984 |
| `ki->field_0x71` | 82.6 / 1988 | **90.0** (identical) |
| `isActive()` | 44.8 / 1916 | 80.7 |
| `getHitpoints() < 1` | 45.7 / 1916 | 80.7 |
| `owner == nullptr \|\| enemyFlagsLo` | 87.4 | 88.0 |
| `empDamage != kNoAttribute` | 84.4 / 1988 | 89.5 |
| `impact != nullptr` | 84.1 / 1992 | 75.9 / 1964 |
| `geometries != nullptr` | 73.6 / 1988 | 60.3 / 1908 |
| `playerGun` | 64.3 / 1920 | **90.0** (identical) |
| `isAsteroid()` | 59.0 / 1964 | 43.6 / 1968 |
| `outer` | 54.3 / 1968 | 56.7 / 1936 |
| `impact->isRocket()` | 85.3 / 1988 | **90.0** (identical) |
| `scatter` | 55.0 / 1940 | 61.2 / 1888 |
| `explosive` | 56.2 / 1908 | 73.5 / 1988 |
| `explodeOnContact` | 84.7 | 60.1 / 2000 |
| `mine` inside the asteroid block | 54.1 / 1948 | 59.1 / 1940 |

No single site beats 90.0. A full 325-pair cross product on `F H K L P` also tops out at
90.0 (the pairs `{ki71,playerGun}`, `{ki71,isRocket}`, `{playerGun,isRocket}` are all
codegen-identical to HEAD); on `F P H L K` the best pair is 88.2.

## 4. Proven vs hypothesis

**Proven (measured this pass):**
1. HEAD (`tree dmg`, tail `F H K L P`) is still the global maximum of every space swept here:
   all 120 tails × 7 damage/expect bases, all single-site expects, all expect pairs on both
   `F H K L P` and `F P H L K`, and 500 random multi-knob samples
   (`{damage shape} × {0–3 expects} × {tail} × {td/pos/loop/flag-type shape}`).
2. Every source shape that produces the shipped `fp` asteroid-join reload moves `i*3` out of
   `r4`. `join_fp=True` was observed only with `i3 ∈ {r5, r6}`; `i3=r4` was observed only
   with `join_fp=False`. The shipped `i3=r4 ∧ join_fp=True` is not reachable in this space.
3. The `td`-join half *is* reachable: `expect(distance,0)` (on either damage shape) produces
   `join_td=True` and the shipped `explosive` slot `[sp,#20]`, but costs `i3 = r5` and
   ~1.4–5.2 pct.
4. The faithful body (REA-confirmed: single `damage` call with an `int dmg` phi, tail
   `F P H L K`, `F P H L K` source order) is `oracle dmg + expect(mine,0) +
   expect(distance,0) + expect(ki->field_0x71,0)` at **87.6 %** / 1984 B. It reproduces the
   shipped tail order, both spill slots, the `td` join reload and the single-call damage
   block, but has `i3 = r6` and no `fp` join reload.
5. Several plausible source rewrites are *codegen-identical* to HEAD (90.0), so they are
   free evidence about the original spelling but not levers: named `bool asteroid`, inverted
   `egoFlag` guard, split/nested `explosive`/`geometries` blocks, `int k` scatter loop,
   `i += 1`, and `expect` on `ki71`/`playerGun`/`isRocket` with value 1.

**Hypothesis (not proven):**
1. The `fp` join reload is an LLVM-7 `SpillPlacement` region-split decision for the loop
   counter's live range around the `isAsteroid` diamond (w5013's interference analysis), and
   the same interference that triggers it is what evicts `i*3` from `r4`. If true, no
   statement-level source reorder can reach the shipped stream; only a different *value
   numbering* (e.g. a source-level temporary that changes `i*3`'s live range) could.
2. The `explosive`/`td` slot swap (`20`/`24`) is stack-object creation order, not source
   declaration order: `td` is spilled first in HEAD even though `explosive` is assigned
   first. Moving the `td` declaration above `lastHitKIPlayer = 0` is codegen-identical.

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
# -> pct=90.0 osize=1984 usize=1984
```

Scratch harnesses (`/dev/shm/w5616/`): `try.py` (splice a body into `Gun.cpp`, `link_so`,
score), `score_fn.py` (official-verifier scorer), `feat2.py` (the 5-field signature),
`mkbody2.py` (body generator: damage shape × expect set × tail × shape knobs),
`libvar.py` (tail permutation), `scan_join.py`, `scan_pairs.py`, `scan_tails2.py`,
`randsearch2.py`.

The two bodies referenced above (drop-in replacements for
`void Gun::calcCharacterCollision()` … up to `void Gun::calcLevelCollision()`):

* **HEAD / tree, tail `F H K L P`** — the shipped tree in `src/game/weapons/Gun.cpp`
  (unchanged): `int dmg = this->damage;` before the owner test, four `enemy->damage(...)`
  calls, and the post-damage statements in order `F H K L P`.
* **faithful (87.6 %)** — same tree body with (a) the damage block replaced by

```cpp
            int dmg;
            if (this->owner == nullptr || this->owner->enemyFlagsLo) {
                if (enemy->egoFlag && ((Level *) this->level)->getPlayer() != nullptr &&
                    ((Level *) this->level)->getPlayer()->isDockedToDockingPoint()) {
                    dmg = (int) ((float) this->damage * 3.0f * 0.25f);
                } else {
                    dmg = this->damage;
                }
            } else if (enemy->egoFlag) {
                dmg = (int) ((float) this->damage * 0.2f);
            } else {
                dmg = this->damage;
            }
            enemy->damage(dmg, this->friendGun, this->itemIndex);
```

  (b) the post-damage statements reordered to `F P H L K`, and (c) three hints:
  `if (__builtin_expect(mine, 0))` on the radius branch,
  `__builtin_expect(enemy->getKIPlayer()->field_0x71, 0)`, and
  `if (__builtin_expect(distance < this->magnitude, 0))` in the scatter loop.

## 6. Files changed

`_work/reconstructions/gun-calccharactercollision-wave108-w5616.md` only (note).
Gate: byte 4500 / linked 4500, size-exact 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, `RATCHET PASS`.
