# `Gun::calcCharacterCollision` wave 115 (w6315): drop the `dmg` hoist, move
# `hitFlags[i] = 1` to the scatter head, and read `this->damage` through a
# `const volatile` reference in the 0.2f arm — 92.7692 % -> 93.3846 % (1984 B)

Worker: w6315. Branch: `wave115-w6315`, base master `b847d5d46`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).
Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump `0x152f90`, size `1984 B`, 650 insns.
Scratch: `/dev/shm/w6315/`.

## 0. Result

| build | gate byte/linked | target pct | matched | size |
|---|---:|---:|---:|---:|
| before (HEAD `b847d5d46`) | **4500 / 4500** | **92.7692** (report 92.8) | 603/650 | 1984 |
| after this pass | **4500 / 4500** | **93.3846** (report 93.4) | 607/650 | 1984 |

Gate after: byte 4500/4524, linked 4500/4524, size-exact 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0, `RATCHET PASS`. No ELF section / `.ARM.extab` change
(27 sections, `.text 0x1337c8`, `.ARM.exidx 0x6f70`, `.ARM.extab 0x26298`,
`.rodata 0x41c27` — the last is the pre-existing mission-table delta), file size 2,187,328 B.

## 1. The landed change

Three source edits inside `Gun::calcCharacterCollision()`:

```cpp
-            int dmg = this->damage;
             if (this->owner == nullptr || this->owner->enemyFlagsLo) {
                 if (enemy->egoFlag && ((Level *) this->level)->getPlayer() != nullptr &&
                     ((Level *) this->level)->getPlayer()->isDockedToDockingPoint()) {
                     enemy->damage((int) ((float) this->damage * 3.0f * 0.25f), this->friendGun, this->itemIndex);
                 } else {
-                    enemy->damage(dmg, this->friendGun, this->itemIndex);
+                    enemy->damage(this->damage, this->friendGun, this->itemIndex);
                 }
             } else if (enemy->egoFlag) {
-                enemy->damage((int) ((float) this->damage * 0.2f), this->friendGun, this->itemIndex);
+                const volatile int &dv = this->damage;
+                enemy->damage((int) ((float) dv * 0.2f), this->friendGun, this->itemIndex);
             } else {
-                enemy->damage(dmg, this->friendGun, this->itemIndex);
+                enemy->damage(this->damage, this->friendGun, this->itemIndex);
             }
```

and, in the scatter block, `this->hitFlags[i] = 1;` moved from the tail of the
`__builtin_expect(distance < this->magnitude, 0)` block to the head of `if (scatter)`
(i.e. immediately before `this->hitPositions.data()[i] = position;`), which is the
original's store site (`0x153426`).

This is the faithful shape the binary shows: no `dmg` local, `hitFlags` before
`hitPositions`. The `const volatile int &dv` in the `0.2f` arm exists only to keep that
arm's `this->damage` load separate from the plain-damage arms' load (see §2.2); with a
plain `this->damage` the compiler merges them and the function grows to 2000 B.

`F` = `field_54[0] = 1`, `P` = `if (playerGun) level->field_30 = 1`,
`H` = `setHitVector`, `L` = `lifetimes[i] = -1000000`, `K` = `lastHitKIPlayer = getKIPlayer()`.
The tail order `F P H L K` is unchanged (120-permutation sweep on the new body: `FPHLK`
93.3846, next `FPLHK`/`FPHKL` 92.9231).

## 2. Why this works: the shipped damage block has *three separate* `[r9,#96]` loads

Shipped (objdump `0x152f90`):

```
15328a: ldr.w r0, [r9, #4]        ; this->owner
15328e: cbz   r0, 153296
153290: ldrb.w r0, [r0, #92]      ; owner->enemyFlagsLo
153294: cbz   r0, 1532ca
153296: ldrb.w r0, [r8, #105]     ; enemy->egoFlag
15329a: add.w sl, sp, #192        ; sl = &position
15329e: cbz   r0, 153304
1532a0.. : getPlayer() != null && isDockedToDockingPoint()
1532b8: vldr  s0, [r9, #96]       ; (1) arm A: this->damage, float load
1532b8.. : vcvt/vmul 3.0/vmul 0.25
1532c8: b.n   1532e0
1532ca: ldrb.w r0, [r8, #105]     ; else branch
1532ce: add.w sl, sp, #192
1532d2: cbz   r0, 153304
1532d4: vldr  s0, [r9, #96]       ; (2) arm C: this->damage, float load
1532d4.. : vcvt/vmul 0.2
1532e0: ldr.w r3, [r9, #88]       ; friendGun
1532e4: ldrb.w r2, [r9, #249]     ; itemIndex
1532e8: vcvt.s32.f32 s0, s0
1532ec: vmov  r1, s0
1532f0: b.n   153310
1532f4: <literal pool>
153304: ldr.w r3, [r9, #88]       ; arms B and D (tail-merged)
153308: ldr.w r1, [r9, #96]       ; (3) this->damage, int load
15330c: ldrb.w r2, [r9, #249]
153310: mov   r0, r8
153312: blx   _ZN6Player6damageEibi
```

So the original has **no `dmg` local**, arms B and D are tail-merged into one block, and
`sl` holds `&position` (materialised in both the `if` and `else` branches at `15329a` /
`1532ce`). The old HEAD hoisted `int dmg = this->damage;` into `sl`, used it in arms B/D,
and materialised `&position` *after* the call — which is exactly the 4 bytes that shifted
the literal pool from `1532f4` (shipped) to `1532f0` (old HEAD) and cost the four
`ldr [pc,#N]` immediate matches.

### 2.1 Why the `volatile` is needed

With plain `this->damage` in all four arms the emitted IR has GVN merge arm C's and arm
D's loads into a single `%341 = load i32, i32* %59` in the `else` block (verified with
`-mllvm -print-after-all`: pre-GVN the damage block has four separate loads, post-GVN the
`else` block has one). Arm D then no longer matches arm B (`r3/r1/r2` vs `r3/mov/r2`), the
machine-level tail merge fails, and the function grows to **2000 B** (arm D becomes an
out-of-line `ldr r3; b <middle of arm B>` block).

A `const volatile int &dv = this->damage;` in the `0.2f` arm forces that arm's load to
stay separate (volatile loads are never CSE'd), which reproduces the shipped damage block
instruction-for-instruction and brings the function back to exactly **1984 B**.

This is a codegen-shaping device, not a claim about the original source: the original
presumably reached the same IR some other way. Measured equivalents (all 93.3846 / 1984):
`volatile int &dv`, `const volatile int *dv = &this->damage` + `*dv`,
`volatile int *dv`, and putting the volatile read in arm D instead of arm C.
`volatile int dv = this->damage;` (by value) is worse (86.2414 / 1988).

### 2.2 `Gun::ignite()` cross-check

`_ZN3Gun6igniteEv` is byte-exact (100.0000 %, 1044 B, 353/353). Its scatter block is the
same code shape and confirms the original author's ordering:

```cpp
if (distance < this->magnitude) {
    this->hitFlags[i] = 1;
    this->hitPositions.data()[i] = this->basePos;
    float range = (float) this->magnitude;
    float remaining = (float) (this->magnitude - distance);
    float falloff = remaining / range;
    ...
}
```

i.e. `hitFlags` **before** `hitPositions`, and `range`/`remaining`/`falloff` as three
named locals — which is why the `hitFlags` move in §1 is the faithful site.

## 3. Measured tables (official verifier path; `link_so` + `tools/verify` scoring)

### 3.1 Damage-block form × `hitFlags` site × `expect` (on the wave-114 base)

| dmg form | hitFlags | expect | pct | size |
|---|---|---|---|---:|
| hoist, used in B (old HEAD) | tail of expect block | `0` | 92.7692 | 1984 |
| hoist, used in B | scatter head | `0` | 85.6043 | 1980 |
| hoist, used in B | scatter head | none / `1` | 64.2857 | 1912 |
| direct, no local | scatter head | `0` | 84.4206 | 1996 |
| direct, no local | tail | `0` | 83.7423 | 2000 |
| direct, no local | either | none / `1` | 60.65 – 64.04 | 1904 – 1920 |
| **direct + `volatile` arm C** | **scatter head** | **`0`** | **93.3846** | **1984** |
| direct + `volatile` arm C | tail | `0` | 85.9339 | 1988 |
| hoist + `volatile` arm C | tail | `0` | 92.9231 | 1984 |
| hoist + `volatile` arm C | scatter head | `0` | 85.7583 | 1980 |

The `__builtin_expect(distance < this->magnitude, 0)` is load-bearing: without it (or with
polarity `1`) the inner loop body becomes hot, `this` is demoted from `r9` to `r4`, and the
whole register map rotates (1908 B).

### 3.2 `dmg` usage matrix (which arm reads the local) — 16 combos × 2 `hitFlags` sites

| A | B | C | D | hitFlags=tail | hitFlags=head |
|---|---|---|---|---:|---:|
| `this` | `dmg` | `this` | `dmg` | **92.7692 / 1984** | 85.6043 / 1980 |
| `this` | `dmg` | `this` | `this` | 92.7692 / 1984 | 85.6043 / 1980 |
| `this` | `dmg` | `dmg` | `this`/`dmg` | 92.7692 / 1984 | 85.6043 / 1980 |
| `this` | `this` | `this`/`dmg` | `this`/`dmg` | 83.7423 / 2000 | 84.4206 / 1996 |
| `dmg` | `this` | … | … | 85.1227 / 2000 | 84.4206 / 1996 |
| `dmg` | `dmg` | … | … | 86.3077 / 1984 | 84.2186 / 1980 |

Rule: **arm B must read `dmg`** (or arm A must) for the 1984-byte layout; and whenever a
`dmg` local exists it lands in `sl`, so `&position` is materialised after the call.

### 3.3 Phi form (`int dmg;` assigned in each arm, one `damage(dmg,…)` after)

| variant | pct | size |
|---|---:|---:|
| `int dmg;` (no init) / `= 0` / `= this->damage` | 83.6672 | 1980 |
| phi + `hitFlags` at scatter head | 84.0401 | 1976 |

Structurally the closest to the shipped damage block (shared call site, `sl = &position`,
arms B/D merged) but still merges arm C/D's loads via GVN and comes out 4 B short.

### 3.4 Declaration-order permutation (all 24 orders of position/velocity/enemyPosition/radius)

`pos vel ep rad` (HEAD) = **92.7692 / 1984**; next `pos vel rad ep` 92.1538, then
`pos rad vel ep` 91.5385. `i*3` stays `r5` in every order that keeps 1984 B.

### 3.5 `__builtin_expect` site sweep — 23 sites × {0,1}, on old HEAD and on the new body

On old HEAD: no site beats 92.7692. Neutral: `enemies == nullptr`, `egoFlag && docked`,
`playerGun`, `impact != nullptr`, `falloff > 1.0f`. Next: `empDamage != kNoAttribute`
92.1538, `owner == nullptr || …` 90.6154, `mine` (outer) 88.7692.
On the new body: no site beats 93.3846 (`playerGun`, `impact`, `isRocket`, `empDamage > 0`,
`falloff > 1.0f` are codegen-identical; `empDamage != kNoAttribute, 1` drops to 92.7692).

### 3.6 Tail-order sweep (120 permutations) on the new body

`FPHLK` **93.3846**; `FPLHK` / `FPHKL` 92.9231; `FPKHL` 92.7692; `KFPHL` 92.6154;
`HFPLK` / `FPLKH` / `FPKLH` 92.4615; everything else ≤ 92.3077.

### 3.7 Other spellings (all 93.3846 / 1984, i.e. codegen-identical)

`const Vector playerPosition`; `const int distance`; `this->hitPositions[i]`;
`this->hitFlags[i] = true`; `volatile const int *dv = &this->damage`; `volatile int &dv`;
`volatile int *dv`; volatile read in arm D instead of arm C.
Worse: inline `getPosition()` in the scatter-radius block (92.6154);
`volatile int dv = this->damage;` by value (86.2414 / 1988).

## 4. Remaining residual (43 mismatched insns, was 47)

1. **`i*3` is `r5`, shipped `r4`** (8 instructions: `add.w rX, fp, fp, lsl #1`, the two
   indexed loads, `mov r6, rX`, the restore, and three later `add … lsl #2`). In the emitted
   IR the `COPY` of `&position` used as the `outerCollide(const Vector&)` argument evicts
   `this` from `r4`; the allocator then gives `i*3` `r5`. Every index spelling and every
   declaration order tried keeps `r5` whenever the function is 1984 B.
2. **Scatter-radius temporaries swapped** — shipped `r4 = sp+44` (playerPosition) /
   `r5 = sp+104` (delta); ours the other way round (2 instructions).
3. **`isAsteroid` join reloads** — shipped reloads `fp` (`i`) and `td` at both joins
   (`ldr.w fp,[sp,#40]`, `ldr r6,[sp,#24]`); ours rematerialises `add r4, sp, #104` (2
   instructions).
4. **Inner `k`-loop rotation** — shipped places the loop condition at the bottom
   (`movs r5,#0; b cond; body; adds r5,#1; cond: …; bcc body`); ours places it at the top
   (`movs r5,#0; cond: …; bcs exit; body; adds r5,#1; b cond`). Flipping the `expect`
   polarity makes the body hot but collapses the function to 1908 B and `this` to `r4`.

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
```

Scratch harnesses (`/dev/shm/w6315/`): `score.py` (full-precision verifier metric),
`run.py` (apply a `MOD(src)` variant + `link_so` + score), `mksbs.py` / `sbs2.py`
(opcode-aligned side-by-side), `gen.py`, `vgen2.py`–`vgen8.py`, `vvol*.py`, `vnat.py`,
`perm.py`, `perm2.py`, `sweep_exp.py`, `sweep_v2.py`, `dmgmatrix.py`, `tail_sweep.py`,
`mk_ir.sh` (emit the project-clang IR), `ir/direct.ll` and `all.txt` (IR +
`-print-after-all` dumps used for the GVN analysis in §2.1).

## 6. Files changed

`src/game/weapons/Gun.cpp` (the three edits in §1). No header, no data, no linker change.

## 7. References

`_work/reconstructions/gun-calccharactercollision-wave114-w6213.md`,
`...-wave113-w6112.md`, `...-wave112-w6011.md`, `...-faithful-shape-w4010.md`.
