# `Gun::calcCharacterCollision` wave 113 (w6112): the shipped join reloads are
# reachable (`nohf_d` = 87.0 % with `fp=i`/`td=r6` at both `isAsteroid` joins),
# but every source that re-adds `hitFlags[i]=1` flips `fp` back to the shared
# `1` constant; HEAD's tuned `F H K L P` body stays the maximum at 90.0 %

Worker: w6112. Branch: `wave113-w6112`, base master `a979ace18`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).
Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump `0x152f90`, size `1984 B`, 660 insns.
Scratch: `/dev/shm/w6112/`.

## 0. Result

**No code change landed.** HEAD (`src/game/weapons/Gun.cpp`: `int dmg = this->damage` +
four `enemy->damage(...)` sites + tail `F H K L P` + the `expect(box_x < r, 1)` anchor)
is still the best measured shape: **90.0 %**, 1984/1984 B.

| build | gate byte/linked | target pct | target size |
|---|---:|---:|---:|
| before (HEAD `a979ace18`) | **4500 / 4500** | **90.0** | **1984** |
| after this pass (note only) | **4500 / 4500** | **90.0** | **1984** |

Gate after: byte 4500/4524, linked 4500/4524, size-exact 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0, `RATCHET PASS`. No ELF section / `.ARM.extab` change
(27 sections, `.ARM.extab` `0x26294`, file size 2,187,328 B).

`F` = `field_54[0] = 1`, `P` = `if (playerGun) level->field_30 = 1`,
`H` = `setHitVector`, `L` = `lifetimes[i] = -1000000`, `K` = `lastHitKIPlayer = getKIPlayer()`.

## 1. The residual is the two `isAsteroid` join reloads (6 B), not the damage block

The shipped build reloads **both** `i` and `td` at the two `isAsteroid` join targets:

```
153196: ldr   r0, [sp, #36]      ; mine
153198: ldr.w fp, [sp, #40]      ; i
15319c: ldr   r6, [sp, #24]      ; td
15319e: cmp   r0, #0
1531a0: bne.n 15325a             ; if (mine) continue
...
153260: ldr   r0, [sp, #36]
153262: ldr.w fp, [sp, #40]      ; i
153266: ldr   r6, [sp, #24]      ; td
```

HEAD has neither (`join_fp=False`, `join_td=False`); it reloads `i` per use
(`0x153330`, `0x15336e`, `0x153374`, `0x1533f4`) and keeps `td` in `r6` from the
prologue. The two reloads are `ldr.w` (4 B) + `ldr` (2 B) = **6 B**, exactly the
"6-byte spill-reload layout shift" in the hint. Because they sit before the first
literal pool, their absence shifts HEAD's pool 16 B early (`[pc,#832]` vs `#848`),
which also costs the 7 `ldr [pc,#N]` immediate matches.

## 2. New this pass: `nohf_d` reproduces the shipped join reloads *and* `td` in `r6`

`tree` body + `F P H L K` + `expect(distance < magnitude, 0)` and **no**
`hitFlags[i] = 1` store:

| property | shipped | `nohf_d` |
|---|---|---|
| size | 1984 | 1972 |
| `this` / `enemy` | `r9` / `r8` | **`r9` / `r8`** |
| `td` | `r6` | **`r6`** |
| `td` slot | `sp+24` | **`sp+24`** |
| `i3` (`i*3`) | `r4` | `r5` |
| `fp=i` join reload | yes | **yes** |
| `td` join reload | yes | **yes** |
| pct | — | **87.0** |

Its first half is the shipped first half with `r4`/`r5` swapped and the
`hitFlags` store removed; the tail is instruction-for-instruction shipped
(modulo the 4-byte shift). Adding `hitFlags[i] = 1` back gives `hf_plain`
(1980 B, 84.8 %): the tail is shipped-minus-the-`fp`-reload, but `join_fp`
flips to **False** and the reload moves from the join to `0x153274`.

## 3. Measured tables (official verifier; `link_so` + `tools/verify/verify.py` scoring)

### 3.1 `hitFlags` formulations on `nohf_d` — all flip `join_fp` to False

| variant | pct | size | i3 | td slot | join_fp | join_td |
|---|---:|---:|---|---|---|---|
| `nohf_d` (no store) | **87.0** | 1972 | r5 | 24 | **T** | T |
| `this->hitFlags[i] = 1;` before `hitPositions` | 84.8 | 1980 | r5 | 24 | F | T |
| `= true` / `(uint8_t)1` / `'\1'` / `!0` / `(bool)1` / `(unsigned char)1` | 84.8 | 1980 | r5 | 24 | F | T |
| `*(this->hitFlags + i) = 1` | 84.8 | 1980 | r5 | 24 | F | T |
| `uint8_t *hf = ...; hf[i] = 1` | 84.8 | 1980 | r5 | 24 | F | T |
| `hitFlags[i] \|= 1` / `= hitFlags[i] + 1` | 84.8 | 1980 | r5 | 24 | F | T |
| store **after** `hitPositions` | 84.2 | 1980 | r5 | 24 | F | T |
| store after the `impact` block (`if (scatter)`) | 84.8 | 1980 | r5 | 24 | F | T |
| duplicate store (`hf_x2`) | 84.8 | 1980 | r5 | 24 | F | T |
| `hitFlags[(i*3)/3] = 1` | 73.9 | 1988 | r5 | 24 | **T** | T |
| store in a separate `if (scatter)` before `impact` | 82.5 | 1992 | r6 | 20 | **T** | T |
| store inside the inner scatter loop (`if (k==0)`) | 85.2 | 1984 | r6 | – | T | F |

`(i*3)/3` and the separate `if (scatter)` are the only forms that keep the join
reloads, but both change `enemy` (`r9`) or the slots and score far below HEAD.

### 3.2 Tail order is irrelevant in the `nohf_d` shape

All **120** permutations of `F P H L K` on `nohf_d` produce the **same** code
(87.0, 1972 B, `i3=r5`, `td=24`, `join_fp=T`, `join_td=T`): the scheduler
normalises the tail. Same for `hf_plain` (all 120 → 84.8, 1980 B, `join_fp=F`).
Tail order only matters when the `int dmg` hoist is present (HEAD family).

### 3.3 `expect` sweeps

* On HEAD (tree + `F H K L P` + anchor), all 19 sites × {0,1}: no improvement.
  `isRocket=1`, `playerGun=1`, `falloff_hi=0` are codegen-identical to HEAD (90.0);
  `emp=1` 89.5; `owner=1` 88.0; everything else ≤ 85.3.
* On `tree + F P H L K` (no other expect): `mine_radius=0` **85.5** (1984 B,
  `this=r9`, `i3=r4`, `td=r8`, `td slot=28`, scatter if-body inline);
  `distance_mag=0` 84.8; `d0+isRocket=0` **88.2**; `d0+emp=0` **88.0**.
* On `phi + F P H L K`: `isRocket=0` 88.5, `emp=0` 87.9; best pair
  `isRocket=0+geometries=1` **89.4** (1984 B, `i3=r4`, `td=r6`, `enemy=r8`).
* On `mr0` (`mine_radius=0`): `+distance_mag=0` 82.3 (`enemy=r8`, `td slot=24`,
  `join_td=T`, `i3=r6`); `+owner=0` 85.4.
* On `nohf_d`: `impact=1`/`emp_pos=0`/`playerGun=1` 87.0 (identical), `isRocket=0`
  86.1, `scatter=1` 84.9, `mine_radius=0` 83.1, `eoc=0` 81.2.
* 220-sample random search over `{tree,direct,phi} × 120 tails × 0–3 expect
  sites` (seed 6112): best is **90.0** (`tree + F H K L P`, `playerGun=1` ≡ HEAD).

### 3.4 Damage-form grid (with anchor, no other expect)

| form + tail | pct | size | this | i3 | td | join_fp |
|---|---:|---:|---|---|---|---|
| tree + `F H K L P` (HEAD) | **90.0** | 1984 | r9 | r4 | 20 | F |
| tree + `F P H L K` | 64.8 | 1912 | r4 | r5 | – | F |
| direct + `F P H L K` | 64.1 | 1920 | r4 | r5 | – | F |
| phi + `F P H L K` | 61.8 | 1904 | r4 | r5 | – | F |
| phi + `F H K L P` | 84.1 | 1980 | r9 | r4 | 20 | F |
| hybrid (`dmg` init + single call) + `F H K L P` | 63.3 | 1900 | r4 | r5 | – | F |

The `int dmg = this->damage;` hoist inside the loop is what keeps `this=r9`:
without it (`direct`, `phi`, hybrid) the allocator demotes `this` to `r4` and the
whole function's register map rotates. HEAD's hoist also puts `sl = this->damage`
in the damage block (shipped uses `sl = sp+192`), which is the main remaining
structural difference in the middle block.

### 3.5 Other measured (all ≤ 90.0)

* `td` as `Vector &`, or no `td` local at all (direct `this->targetDir`): identical
  to HEAD (90.0). `td` declared before `explosive`: 67.1.
* `bool`→`int` for `mine`/`scatter`/`explosive`; `for (int i/e …)`: identical.
* `Vector &position/velocity`: 50.1. `enemyPosition` first: 79.0. `float radius`: 58.9.
* `dmg` used in all four arms (`dmg_all`): 84.6. `dmg` used only in the first plain
  arm: 90.0 (identical). `dmg` hoisted above `empDamage`: 85.0; above `ki`: 63.9;
  above `radius`: 80.3; at function top: 72.7.
* Moving the `lifetimes[i]` store earlier: after join 75.4, before `emp` 75.4,
  before `owner` 75.3, before `impact` 84.7, after `impact` 84.3.
* Removing `hitFlags` from HEAD: 64.5 (this=r4). Removing `hitPositions`: 62.3.
  Removing both: 64.0 (`join_fp=T` but `this=r4`).
* `do { … } while` rewrite of the outer loop: 76.6; both loops: 59.3.
* Extra `(void)` reads of `lifetimes[i]`/`targetDir.x`/`explosive`/`td->x`:
  optimised away (identical 90.0).
* `explosive`/`td` stack slots: HEAD has `td=sp+20`, `explosive=sp+24`; shipped is
  the reverse. No source variant with `i3=r4` reproduces the shipped slot order
  (waves 107–112 already established the anti-correlation).

## 4. Why the shipped map is still out of reach

The `hitFlags[i] = 1` store is the pivot. With it, HEAD keeps `this=r9`, `i3=r4`
and the shipped tail order is unavailable; without it (`nohf_d`), the allocator
reproduces the shipped `fp=i`/`td=r6` join reloads but colours `i*3` to `r5` and
the function shrinks to 1972 B. Every attempt to re-add the store while keeping
the join reloads either rotates `enemy` to `r9` (`(i*3)/3`, 73.9) or moves the
reload off the join (`hitFlags[i]=1`, 84.8). The two goals are mutually exclusive
under this compiler's greedy allocator for every source shape measured here.

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

Scratch harnesses (`/dev/shm/w6112/`): `run.py` (splice body + `link_so` + score),
`sig.py` (register signature), `variants.py` (`base`, damage forms, tail halves,
19 expect sites), `tail_nohf.py` / `tail_hfplain.py` (120-tail sweeps),
`bigsweep.py` (6 expect sets × 120 tails), `sweep_hf.py` / `sweep_nohf_hf.py`
(hitFlags formulations), `sweep_phi.py` / `sweep_phi2.py` (phi expect sweeps),
`sweep_mr0.py` / `sweep_mr0d0.py` (`mine_radius` sweeps), `sweep_i3div.py`,
`sweep_pressure.py`, `sweep_dmgpos.py`, `sweep_scatlayout.py`, `sweep_loops.py`,
`randsearch.py` (220 random samples), `cc_mir.sh` / `mkb.sh` (single-TU MIR).

## 6. Files changed

None under `src/` (note only).

## 7. References

`_work/reconstructions/gun-calccharactercollision-wave112-w6011.md`,
`...-wave111-w5914.md`, `...-wave110-w5814.md`, `...-w5317.md`,
`...-faithful-shape-w4010.md`.
