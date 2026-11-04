# `Gun::calcCharacterCollision` wave 109 (w5715): `join_fp` is reachable but only with
# `i*3` out of `r4`; a 400-sample random source search does not beat 90.0

Worker: w5715. Branch: `wave109-w5715`, base master `7c34c4098`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).
Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump `0x152f90`, size `1984 B`, 650 insns.
Scratch: `/dev/shm/w5715/`.

## 0. Result

**No code change landed.** HEAD (`src/game/weapons/Gun.cpp`, `int dmg = this->damage` +
four `enemy->damage(...)` call sites + tail `F H K L P`) is still the best measured shape:
**90.0 %**, 1984/1984 B, 585/650 normalized instructions (official verifier).

| build | gate byte/linked | target pct | target size | target matched |
|---|---:|---:|---:|---:|
| before (HEAD `7c34c4098`) | **4500 / 4500** | **90.0** | **1984** | **585/650** |
| after this pass (note only) | **4500 / 4500** | **90.0** | **1984** | **585/650** |

Gate after: byte 4500/4524, linked 4500/4524, size-exact 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0, `RATCHET PASS`. No ELF section / `.ARM.extab` change
(no code change; the 28-section / `.ARM.extab` `0x26294` state of wave 108 is untouched).

`F` = `field_54[0] = 1`, `P` = `if (playerGun) level->field_30 = 1`,
`H` = `setHitVector`, `L` = `lifetimes[i] = -1000000`, `K` = `lastHitKIPlayer = getKIPlayer()`.

## 1. What this pass adds

Wave 108 established the 5-field reload/register signature (`i3`, `join_fp`, `join_td`,
`expl`, size) and the **anti-correlation**: every source shape that produces the shipped
`ldr.w fp,[sp,#40]` at the asteroid join (`join_fp=True`) moves `i*3` out of `r4`; every
shape that keeps `i3=r4` keeps `i` in the spill slot through the join.  It had observed
`join_fp=True` only with `i3 ∈ {r5, r6}` in 840 tail×expect variants.

This pass (a) re-confirms the faithful body, (b) **finds `join_fp=True` with `i3=r6` in a
completely different corner of the space** (an `expect(isAsteroid,1)` + `expect(explosive,0)`
+ `expect(ki->field_0x71,1)` combination), so the property is real and not an artefact of
the `distance=0` family, and (c) runs a 400-sample random multi-knob search that still
tops out at HEAD's 90.0.  The shipped `i3=r4 ∧ join_fp=True` remains unreached.

Signature extractor: `sig.py` (reads the first `add.w rX,fp,fp,lsl #1`; a `ldr r0,[sp,#36]`
followed within 2 insns by `ldr.w fp,[sp,#40]` / `ldr r6,[sp,#20|24]`).

Shipped: `(i3=r4, join_fp=True, join_td=True, expl=20, size=1984)`.
HEAD:    `(i3=r4, join_fp=False, join_td=False, expl=24, size=1984)`.

## 2. Measured table (official verifier; `link_so` + `tools/verify/verify.py` scoring)

| variant | pct | usize | `i3` | `join_fp` | `join_td` | note |
|---|---:|---:|---|---|---|---|
| **tree, tail `F H K L P` (= HEAD)** | **90.0** | **1984** | **r4** | False | False | global max of every space swept |
| tree, tail `F K H L P` | 89.7 | 1984 | r4 | False | False | |
| tree, tail `K P F H L` + `expect(distance,0)` | 88.6 | 1984 | r5 | False | True | best non-HEAD (wave 108) |
| faith (phi dmg), tail `F P H L K` + `expect(rocket,0)` | 88.5 | 1984 | r4 | False | False | new: `i3=r4` with the phi shape |
| faith (phi dmg), tail `F P H L K` + `expect(emp,0)` | 87.9 | 1984 | r4 | False | False | |
| **faithful2: phi dmg, `F P H L K`, `expect(mine_rad,0)+expect(ki71,0)+expect(distance,0)`** | **87.6** | **1984** | **r6** | False | **True** | closest source shape; matches tail, slots 24/20, td join, single-call damage block |
| faith (phi dmg), tail `F H K L P` (no expects) | 84.1 | 1980 | r4 | False | False | 4 B short |
| tree, tail `F P H L K` + `expect(distance,0)` | 84.8 | 1980 | r5 | False | True | |
| faith, tail `F P H L K` + `expect(distance,0)` (single site) | 83.3 | 1976 | r5 | False | True | |
| faith, tail `F P H L K` + `expect(mine_rad,0)` (single site) | 81.4 | 1980 | r4 | False | False | |
| tree, tail `P K F H L`, `expect(isAsteroid,1)+expect(explosive,0)+expect(ki71,1)` | 47.2 | 1956 | **r6** | **True** | False | **new `join_fp=True` corner** |
| tree, tail `P K F H L`, `expect(isAsteroid,1)+expect(explosive,0)+expect(ki71,1)+expect(distance,0)` | 50.0 | 1972 | r6 | True | False | |

Structural / type knobs measured on the faithful2 base (all 1984 B unless noted):

| variant | pct | `i3` | `join_fp` | `join_td` |
|---|---:|---|---|---|
| `if (!mine) { body } else { continue; }` in the asteroid block | 87.6 | r6 | False | True |
| `enemy->setBombForce(0.0f);` hoisted above `if (mine) continue;` | 87.1 | r6 | False | True |
| `for (int i …)` / `for (int e …)` | 87.5–87.6 | r6 | False | True |
| `for (int k …)` in the scatter loop | 87.6 | r6 | False | True |
| `int radius` assigned by `if/else` instead of the ternary | 87.6 | r6 | False | True |
| `if (!mine) { body }` (no else; mine falls into the damage block) | 50.4 | — | False | False |
| `goto next_projectile;` instead of `continue` | 50.7 | — | False | False |
| `while (i < count)` probe | 58.2 | — | False | False |

400-sample random search (`{tree, faith} × random tail × 0–4 random expects on 15 sites`):
max 90.0 (HEAD itself), next 89.7 (`F K H L P`), then 87.2.  The only `join_fp=True`
samples were `i3=r6` and scored ≤ 50.6.

## 3. Proven vs hypothesis

**Proven (measured this pass):**
1. HEAD is still the global maximum over the new spaces: all 120 tails of the faithful2
   expect set, all single-site expects on the faithful body, 176 `expect(isAsteroid)` ×
   tail × expect-subset variants, 648 `{ast,expl,ki71,mine_rad,dist}` × 9-tail variants,
   and 400 random multi-knob samples.
2. `join_fp=True` **is** reachable outside the `distance=0` family: the
   `expect(isAsteroid,1)+expect(explosive,0)+expect(ki71,1)` corner produces it (47.2 %,
   `i3=r6`).  This strengthens wave 108's anti-correlation: across every observed sample,
   `join_fp=True` implies `i3 ∈ {r5,r6}`; `i3=r4` never co-occurs with `join_fp=True`.
3. The faithful body can keep `i3=r4` (with `expect(rocket,0)` or `expect(emp,0)`, 87.9–88.5)
   but then has `join_fp=False` — the two properties remain mutually exclusive in the swept
   source space.
4. `if (!mine) { body } else { continue; }` is codegen-identical to `if (mine) continue;`
   (87.6), so the asteroid-block guard spelling is not a lever.
5. The faithful2 body (phi damage, `F P H L K`, mine_rad/ki71/dist expects) is the closest
   *source-level* reconstruction: it matches the shipped tail order, both spill slots
   (`td=[sp,#24]`, `explosive=[sp,#20]`), the `td` asteroid-join reload and the single-call
   damage block; its only residuals are `i3` in `r6` and the missing `fp` join reload.

**Hypothesis (not proven):**
1. The `fp` join reload is a LLVM-7 `SpillPlacement` region split for the loop counter's
   live range around the `isAsteroid` diamond (w5013's interference analysis), and the same
   interference that triggers it evicts `i*3` from `r4`.  If true, no statement-level source
   reorder reaches the shipped `i3=r4 ∧ join_fp=True`; only a different value numbering
   (a source temporary that changes `i*3`'s live range) could.
2. The `explosive`/`td` slot swap (`20`/`24`) is stack-object creation order, not source
   declaration order.

## 4. Reproduction

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

Scratch harnesses (`/dev/shm/w5715/`): `splice.py <body.cpp>` (splice a body into
`Gun.cpp`, `link_so`, score), `score1.py` (official-verifier scorer), `sig.py` (the 5-field
signature), `body_head.cpp` / `faithful.cpp` / `faithful2.cpp` (drop-in bodies),
`sweep_faith.py`, `sweep_tails_f2.py`, `sweep_shapes.py`, `sweep_single_f.py`,
`sweep_struct.py`, `sweep_struct2.py`, `sweep_quick2.py`, `sweep_tree_tail.py`,
`sweep_ast.py`, `sweep_ast2.py`, `randsearch2.py` (the sweeps above), `rea.json`
(fresh REA decompile; confirms the `int dmg` phi and the `F P H L K` tail order).

## 5. Files changed

None under `src/` (note only).
