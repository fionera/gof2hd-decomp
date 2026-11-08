# w8913 (wave-143): `Level::createMission` — pirate-fortress gun loop is a
# pointer-cache loop, not an indexed `kBaseTurrets[k]` loop

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w8913`, based on master
`f5093474f` (wave 143). Continues `level-createmission-wave143-w8812.md`,
`…-w8811.md`, `…-wave139-w8411.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, size `0x4314` = 17,172 B). Ghidra MCP
unavailable; NDK `objdump` is the authority. Raw same-address byte diff is the
primary metric (`/dev/shm/w8913/fdiff.py` points at this worktree's build).

## Baseline (pristine HEAD, re-measured)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   raw diff 5597
.text diff 85,851   .ARM.extab 1,499   .rel.plt 0   .note.gnu.build-id 20
.rodata/.data/.bss/.ARM.exidx exact
gate: byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, parity 5370/5370, RATCHET PASS
```

## 1. Original pirate-fortress loop (Thumb, `b33fe`..`b34dc`)

The loop body is:

```text
b33fe  createStaticObject(this, nullptr, 6774, true)
b340c  (*this->enemies)[i] = result
b3418  gun = (*this->enemies)[i]                 (cached in r4)
b3424  gun->player->setMaxHitpoints(1000)
b342e  t = kBaseTurrets + k*24
b343e  Vector{t[3],t[4],t[5]} -> MatrixRotateVector(rotation, tmp)
b345a  gun->geometry->setRotation(v)
b3462  Vector{t[0],t[1],t[2]} -> MatrixRotateVector(rotation, tmp)
b34ae  gun->setPosition(base + v)                (virtual, slot 0x44)
b34b6  ((PlayerTurret *)gun)->setScaling(6.0f)
b34be  gun->cargo = nullptr
b34c6  gun->shipGroup = 1
b34ca  t += 24                                   (adds r6,#24)
b34cc  i = reload [sp,#116]; i++
b34d2  bound = fortressGuns + friendCount + field_164 + field_168
```

There is **no RNG call** in this loop.  The loop-carried values are `i` (spilled
to `[sp,#116]` because `r4` is reused for `gun`) and the hardpoint pointer `t`
(`r6`, incremented by 24 = 6 floats each iteration).

## 2. The table access is a pointer increment, not an index

The original preheader is `ldr r6,[pc,#448]; add r6,pc` — `r6` is the **base** of
`kBaseTurrets` — and the body uses `add.w r2,r6,#12` / `ldmia r6` and
`adds r6,#24`.  HEAD spelled the table as `int k = 0; ... kBaseTurrets[k]` with
`k++` in the for-increment.  clang 7.0.2 strength-reduced that to a pointer
**biased to `kBaseTurrets[k]+20`** (`add.w sl,r3,#20`), so the body emitted

```text
ldrd r0,r1,[sl,#-8] ; ldr r2,[sl]      ; t[3..5]
sub.w r2,sl,#20     ; ldmia r2          ; t[0..2]   <-- extra 4 B
```

and assigned `this`/`t`/`i` to `r6`/`r5`/`r8` instead of the original's
`r5`/`r6`/`r4`.

Fix (landed): make the pointer explicit and loop-carried.

```cpp
const float *t = kBaseTurrets[0];
for (int i = this->friendCount + this->field_164 + this->field_168;
     i < fortressGuns + this->friendCount + this->field_164 + this->field_168; i++) {
    ...
    v = MatrixRotateVector(rotation, Vector{t[3], t[4], t[5]});
    gun->geometry->setRotation(v);
    v = MatrixRotateVector(rotation, Vector{t[0], t[1], t[2]});
    gun->setPosition(Vector{base.x + v.x, base.y + v.y, base.z + v.z});
    ((PlayerTurret *) gun)->setScaling(6.0f);
    gun->shipGroup = 1;
    gun->cargo = nullptr;
    t += 6;
}
```

This emits `add.w r2,r5,#12` / `ldmia r5` / `adds r5,#24` (the original's
addressing) and removes the extra `sub.w`.

## 3. Measured variants (all faithful; no call/store/RNG-order change)

| variant | size | raw diff |
|---|---:|---:|
| HEAD (`int k`, `kBaseTurrets[k]`) | 17172 | 5597 |
| **pointer `t = kBaseTurrets[0]; t += 6;` (landed)** | **17172** | **5592** |
| `t += 6` in for-increment | 17172 | 5592 |
| `const float (*t)[6] = kBaseTurrets; t++;` | 17172 | 5592 |
| `unsigned i` | 17172 | 5592 |
| `&kBaseTurrets[0][0]` | 17172 | 5592 |
| `Vector(...)` parens | 17172 | 5592 |
| `t` declared before `base` | 17172 | 5592 |
| `Level *self = this` / `int first` / `i` hoisted | 17172 | 5592 |
| cargo/shipGroup source swap | 17172 | 5592 |
| `int k` kept alongside `t` | 17172 | 5597 |
| named `Vector tv1/tv2` temporaries | 17172 | 6438 |
| `Array<KIPlayer*> *enemies` / `&enemies` local | 17172 | 6440 |
| `int i` declared before `base` | 17164 | 5867 |

## 4. NEW evidence: the residual is inherited register allocation

Removing the alien loop's `(i < 0)` codegen anchor (`bigAlienWave ? A : B` plain
ternary) makes the pirate loop body **exactly** the original's register map:

```text
r5 = this   r4 = i (then gun)   r6 = t   r8 = &rotation
r9 = &result   sl = &temp   fp = &v   i spilled to [sp,#116]
```

The plain-select build gives `17136 B / raw 6579` (the alien loop's select is
hoisted and if-converted, 36 B shorter).  With the anchor (HEAD) the alien loop
leaves `this` in `r5`, so the pirate preheader uses `r5` for the base `this`
(instead of `r4`) and the body gets `r6=this, r5=t, r8=i`.  The remaining
pirate-loop diff (249 of 276 B in `b33d0`..`b34e4`) is therefore:

1. `this` in `r5` vs `r4` at the preheader (`add.w r2,r5,#352` vs `r4`),
2. `r9`/`sl` swapped for the MatrixRotateVector sret/temp pointers,
3. `fortressGuns` spill slot `0x30` vs `0x28` (`ldr r4,[sp,#48]` vs `ldr r3,[sp,#40]`).

None of these is reachable from the pirate-loop source alone; they are the
wave-142 alien-gun-loop allocation blocker.  `bool alienWave` (which would fix
the alien guard) drops the function to 17,120 B / raw 12,484, so it is not
usable without recovering the missing bytes.

## 5. Gate counts (before -> after)

```text
byte_exact      4507 -> 4507   (4507/4524)
linked_exact    4507 -> 4507   (4507/4524)
size_exact      4524 -> 4524
data_exact       843 -> 843
imports          207 -> 207
lint            CLEAN   -> CLEAN
drift           0       -> 0
parity          5370/5370 -> 5370/5370
RATCHET         PASS    -> PASS
.text diff      85,851 -> 85,846   (-5)
.ARM.extab      1,499  -> 1,499
.rel.plt        0      -> 0
.note.gnu.build-id 20  -> 20
_ZN5Level13createMissionEv raw diff 5597 -> 5592
```

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6
python3 /dev/shm/w8913/fdiff.py _ZN5Level13createMissionEv
python3 /dev/shm/w8913/rdiff.py _ZN5Level13createMissionEv 0xb33d0 0xb34e4
```

Scratch: `/dev/shm/w8913/{Level.base.cpp,fdiff.py,rdiff.py,hunks.py,hunks2.py,
apply.py,apply2.py,batch.py,batchfull.py,mkvar.py,measure.sh,orig_pf.asm,
ours_pf.asm,v_p1.cpp,v_p2.cpp,...,v_plain_bool.cpp}`.
