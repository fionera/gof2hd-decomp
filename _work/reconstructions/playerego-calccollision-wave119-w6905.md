# PlayerEgo::calcCollision — wave 119 (w6905): 95.24 % → 95.53 % at 968 B

**Status:** source change landed, 2026-11-05. Worktree `wt/w6905`, branch
`wave119-w6905`, base `50d2df436` (current master, wave 119). Extends
`playerego-calccollision-wave119-w6738.md` (94.37 %) and
`-wave119-w6732.md` (95.24 %, the shape this note starts from).

Target: `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE`
(orig 968 B, 347 normalised instructions).

## TL;DR

* **Landed: 95.238095 % → 95.526696 %** (`347/346` normalised
  instructions, **968 B**, unchanged size). `check` = **RATCHET PASS**:
  byte_exact **4505**, linked_exact **4505**, size-exact 4524,
  data_exact 843/843, imports 207, lint **CLEAN**, drift **0**.
  `.text` 1,259,464 B (size-exact), 27 sections, file 2,187,328 B.
  `.ARM.extab` still 0x26290 (8 B short, unchanged — `createMission`).
* The win is replacing the w6732 **tail pad**
  (`inWormhole = 1`, 4 B) with the **real original tail**
  `if (i == 0 && object->field_0x71) collidesWithStationFlag = object->field_0x71;`
  (16 B).  Storing `object->field_0x71` instead of the constant `1`
  makes clang reuse the loaded byte and drop the `movs r0, #1`, which
  is exactly the 2 B needed to fit the real `cmp.w fp,#0; bne.n` test
  inside the 968 B budget.  With `= 1` the same tail is 972 B and the
  score collapses to 87.8 % (pool/branch shift).
* The two redundant `objects->size()` guards of w6732 are still needed
  to keep the loop unrotated and the original register file
  (`this=r5`, `temp=r6`, `distance=sl`, `i=fp`).  They are unchanged.
* Remaining delta: 16 instructions (was 17) — the two guards (5 insns),
  the visible-arm register shuffle (8 insns), the branch-target/pool
  shifts the A2 guard causes (6 insns), and the dropped `movs r0,#1`
  (1 insn, the price of fitting the real tail at 968 B).
* ~120 builds this wave.  Nothing else at 968 B beats 95.53 %.

## 1. The landed body (tail region)

```c++
        if (!object->isVisible())
            continue;
        Vector collision = object->projectCollisionOnSurface(this->geometry->getPosition());
        // Redundant guard (the loop already guarantees i < objects->size()).
        // Together with the wormhole arm's `size() == i` guard it reproduces
        // the original's register file (this=r5, temp=r6, distance=sl) and
        // keeps the function at the original 968 bytes.
        if (objects->empty())
            continue;
        if (Vector{0.0f, 0.0f, 0.0f} != collision) {
            this->geometry->setPosition(collision);
            reinterpret_cast<Matrix &>(this->player->transform[0]) =
                *(Matrix *) Globals::Canvas->TransformGetLocal(this->geometry->transform)
                * *(Matrix *) Globals::Canvas->TransformGetLocal(this->field_0x4->transform);
            // The original stores the flag value straight through: that drops
            // the `movs r0, #1` the constant store would need, which is what
            // keeps the real `i == 0` test inside the 968-byte budget.
            if (i == 0 && object->field_0x71)
                this->collidesWithStationFlag = object->field_0x71;
        }
        this->hitCamera();
```

Everything else (prologue, both redundant guards, wormhole arm, steal
arm) is byte-for-byte the w6732 body.

## 2. Why the tail spelling is load-bearing

The original tail is 18 B:

```
9f374: cmp.w fp, #0        ; 4
9f378: bne.n 9f386         ; 2
9f37a: ldrb.w r0,[r8,#113] ; 4
9f37e: cbz r0, 9f386       ; 2
9f380: movs r0, #1         ; 2
9f382: strb.w r0,[r5,#564] ; 4
```

The w6732 pad was 16 B (`ldrb/cbz/movs/strb.w inWormhole/strb.w flag`)
and omitted the `i == 0` test.  Spelling the real test with `= 1`
gives the original's 18 B but pushes the function to 972 B (code 934 →
2 B pool padding → 972), which shifts every branch target and the
literal pool and collapses the score to 87.8 %.

Storing the loaded byte (`= object->field_0x71`) removes the
`movs r0, #1` (the byte is already 0/1), so the tail is 16 B, the code
region stays at 932 B, the pool stays at `0x9f470`, and the function
stays at 968 B.  The `cmp.w fp,#0` + `bne.n` and the `strb.w` now match
the original; only the `movs` is missing.

## 3. Measurements (all 968 B unless stated)

`pct` is the verify.py normalised-instruction ratio (raw, unrounded).

```
w6732 landed (pad inWormhole=1)                          95.238095  347/346
NEW  real tail, collidesWithStationFlag = field_0x71     95.526696  347/346  <- landed
     ... tail condition `object->field_0x71 && i == 0`    95.526696  347/346
     ... tail nested `if (i == 0) { if (f71) ... }`       95.526696  347/346
     ... tail with constant 1                             87.8       347/348  972
     ... tail `= field_0x71 != 0` / `(bool)` / `= 1`      87.8       347/348  972
     ... tail `if (i == 0)` only                          88.3       347/346
     ... tail `if (i == 0) = 1`                           74.1       347/349  980
     ... tail `if (f71)` only                             87.2       347/343  960
     ... tail `if (i != 0 || f71)`                        94.4       347/346
     ... top i0 store `= field_0x71`, tail `= 1`          94.7       347/346
     ... top i0 store `= field_0x71`, tail `= field_0x71` 87.7       347/346
     ... compare `collision != Vector{...}` (unswapped)   95.2       347/346
     ... compare `Vec3{...}` / full spelling / paren      95.5       347/346
     ... compare `!(Vector{...} == collision)`            94.9       347/346
     ... SZ guard `objects->empty()`                     95.5       347/346
     ... SZ guard `objects->size() == i`                  85.6       347/349  976
     ... A2 guard `objects->size() == 0`                  95.5       347/346
     ... A2 guard merged into the isShrinking condition   95.5       347/346
     ... visible-arm `Vector &c = collision` alias        95.5       347/346
     ... visible-arm `AEGeometry *g` / `KIPlayer *o`      95.5       347/346
     ... visible-arm `Vector collision(...)` ctor form    95.5       347/346
     ... named `Vector surface` in the visible arm        66.3       347/353 1004
     ... surface block without braces                     56.6       347/363 1000
     ... outerCollide(this->geometry->getPosition())      95.5       347/346
     ... loop `size_t i` / `++i` / `objects->count`       95.5       347/346
     ... loop `int i` / `i != size`                       94.9       347/346
     ... loop size hoisted into `n`                       77.8       347/350  992
faithful (no guards, real tail `= 1`)                    59.8       347/352  996
faithful (no guards, real tail `= field_0x71`)           59.7       347/350  992
faithful + expect(i < size, 1)                           49.3       347/351  964
faithful + expect(i < size, 0)                           48.5       347/341  924
faithful + expect(i == 0, 1) / (!isVisible,0) / (coll!=0,1)  68-71    347/347  960
```

Guard-position sweeps with the new tail (A2 at wormhole head /
after isShrinking / before `Vector &pull`; SZ after
`projectCollisionOnSurface`) are all the same code at 95.5 %.
Moving either guard (end of wormhole arm, steal arm, visible-arm end,
loop top, before hitCamera, before the compare) drops to 62-70 %.

## 4. Dead ends measured this wave (don't redo)

* **Visible-arm register shuffle.** The original keeps `&collision` in
  `r6` across the `projectCollisionOnSurface` call and rotates
  `this`→`sl`, `&surface`→`r5` (6 insns); ours rematerialises
  `add r0, sp, #96` / `add r1, sp, #96` (2 insns, 4 B).  No source
  spelling tried reproduces the shuffle: named `surface`, `Vector`
  ctor form, `Vec3`/full-type spellings, `const`, `Vector&`/`const
  Vector&` aliases, `AEGeometry*`/`KIPlayer*`/`PlayerEgo*`/`Player*`
  locals, `PaintCanvas*` hoist, `Vector copy = collision`, dummy
  `float cx`, `(void)0`, `hitCamera()` inside the block, both compare
  orders.  All either give the same 95.5 % code or a worse layout.
  This is a greedy-RA tie-break (which of `&surface`/`&collision`
  stays in `r6`), not a source-shape difference.
* **`__builtin_expect` sweeps** on the faithful and landed bodies:
  loop condition (0/1), null check, steal flag, getType, isVisible,
  outerCollide, isShrinking, strength, playing, distance, volatile,
  collision, alienOrbit, tail — best is 95.5 % (no change) or worse.
* **Loop forms**: `while`, `do/while`, `i != size`, `++i`, hoisted
  size, `objects->count`, `objects->data_[i]` — identical or worse.
* **Guard expressions**: `size()==0`, `empty()`, `size()==i`,
  `size()<=i`, `size()-i==0`, `i>=size()`, `data_==nullptr`,
  `capacity_==0`, merged into the isShrinking `||` — all the same code
  at the landed positions.
* **Tail pads** that keep 968 B with `= 1`: `inWormhole=1` (w6732),
  `freeze=1`, `field_0x1c=1` (964 B), `collide=1`, `field_0xb0=1`,
  two separate `if (f71)` blocks — none beat the real tail.

## 5. Remaining delta to the original (16 instructions)

1. A2 guard `ldr r0,[r4,#0]; cmp fp,r0; beq.w` (3 insns, 8 B) and the
   6 branch-target/pool offsets it shifts (`bne.n`/`beq.n` to the steal
   arm, `beq.n` to the visible arm, `cbnz`/`cbz` in the steal arm,
   `add r1,pc,#436`→`#428`).
2. SZ guard `ldr r0,[r4,#0]; cbz r0` (2 insns, 4 B).
3. Visible-arm register shuffle (8 insns): the original's
   `mov sl,r5; mov r5,r6; add r6,sp,#96` / `mov r6,r5; mov r5,sl;
   mov r1,r6` versus our `add r0,sp,#96` / `add r1,sp,#96`.
4. The dropped `movs r0,#1` in the tail (1 insn) — the 2 B that buy the
   real `i == 0` test at 968 B.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6905
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE
```

Scratch harness (`/dev/shm/w6905`): `score.py`, `ratio.py`,
`bytesdiff.py`, `splice.py`, `build.sh`, `ops.py`, `mirfor.sh`,
`sweep1.py`…`sweep16.py`, `body_*.txt`.

## 7. Gate

`byte_exact 4505 / linked_exact 4505 / size-exact 4524 / data_exact
843 / imports 207`, lint `CLEAN`, drift 0, `RATCHET PASS`.
27 sections, file size 2,187,328 B, `.text` 1,259,464 B,
`.ARM.extab` 0x26290 (8 B short, unchanged).
