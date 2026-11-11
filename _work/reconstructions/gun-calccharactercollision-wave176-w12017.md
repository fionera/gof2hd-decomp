# `Gun::calcCharacterCollision` wave 176 (w12017): fold the magnitude-distance
# difference into a local; raw 463 -> 458 at the same 1984 B frame

Worker: w12017. Branch `wave176-w12017`, base master `b21088985`. Original:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump `0x152f90`, size `1984 B`,
TU `src/game/weapons/Gun.cpp`. Scratch: `/home/debian/GalaxyOnFire2/scratch/w12017`.

## 0. Result

| build | raw diff bytes | size | pct | gate byte/linked |
|---|---:|---:|---:|---:|
| before (HEAD `b21088985`) | **463** | 1984 | 93.4 | 4507 / 4507 |
| after (this pass) | **458** | 1984 | 93.4 | 4507 / 4507 |

`check` after: `RATCHET PASS`, byte 4507/4524, linked 4507/4524,
size-exact 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0.
`.rel.plt` byte-identical (0). Only the function body changed; the symbol
size, address, frame (208 B) and import set are unchanged.

## 1. Change

In the scatter k-loop (`for (unsigned k = 0; k < enemies->size(); ++k)`),
replace the comparison + falloff expression

```c++
int distance = (int) AbyssEngine::AEMath::VectorLength(*td);
if (__builtin_expect(distance < this->magnitude, 0)) {  // lint: arm_builtin
    float falloff = (float) (this->magnitude - distance) / (float) this->magnitude;
```

with a named difference that is reused by both the test and the falloff:

```c++
int distance = (int) AbyssEngine::AEMath::VectorLength(*td);
int diff = this->magnitude - distance;
if (__builtin_expect(diff > 0, 0)) {  // lint: arm_builtin
    float falloff = (float) diff / (float) this->magnitude;
```

The two forms are semantically identical for `int` game values
(`distance < magnitude` iff `magnitude - distance > 0`); the `expect(...,0)`
weight is preserved, so the `this`=r9 / 1984 B layout is retained.

## 2. Why it moves the bytes

Baseline (HEAD, `0x153480`):

```
153480  cmp   r0, r1          ; magnitude vs distance
153482  bgt.n 153488          ; body if magnitude > distance
153484  adds  r5, #1          ; latch
153486  b.n   153440
153488  vmov  s0, r0
15348c  subs  r0, r0, r1      ; magnitude - distance (second subtraction)
15348e  vmov.f32 s30, s20
153492  vmov  s2, r0
```

After (this pass, `0x153480`):

```
153480  subs  r1, r0, r1      ; magnitude - distance (once)
153482  cmp   r1, #1
153484  bge.n 15348a
153486  adds  r5, #1
153488  b.n   153440
15348a  vmov  s0, r0
15348e  vmov  s2, r1
153492  vcvt.f32.s32 s0, s0
```

The shipped body is the rotated-loop form (`cmp r0,r1; ble 1534fa` then a
separate `subs r0,r0,r1` at `0x153480`); the new form still is not the shipped
loop layout, but its `subs`/`vmov` halfwords land on five more shipped bytes
(the `0x15347f..0x15349b` and `0x15349c..` runs shrink by 3 + 2 bytes). The
frame, prologue/epilogue, `.rel.plt` and every other region are unchanged.

## 3. Measured alternatives (all local fast harness, 0.7 s build+link)

| variant | raw | size |
|---|---:|---:|
| HEAD (`distance < magnitude`, expect 0) | 463 | 1984 |
| `diff = magnitude - distance; diff > 0` (this pass) | **458** | 1984 |
| `diff >= 1` / `diff != 0` / falloff from `distance` again | 458 / 1980 / 458 | 1984 |
| `magnitude - distance > 0` inline | 458 | 1984 |
| `magnitude - distance >= 1` inline | 458 | 1984 |
| `distance - magnitude < 0` | 463 | 1984 |
| `distance <= magnitude - 1` | 463 | 1984 |
| `diff` computed before a `distance < magnitude` test | 463 | 1984 |
| `expr` form (pp temporary, shipped slots) | 467 | 1984 |
| `expr` + diff | 462 | 1984 |
| `expect(...,1)` / no hint / `continue` form | 1908 B | — |
| `while` k-loop | 1940 B | — |
| `do { } while` k-loop | 1988 B | — |
| reverse k-loop | 1988 B | — |
| `Matrix view` volatile-pointer / reference / copy variants | 463 | 1984 |
| inner/outer loop `while`, cached counts, `int` counters | 463-518 | 1984 |

The 1984 B frame is uniquely tied to the unlikely-body layout produced by
`expect(distance < magnitude, 0)`; every "rotated k-loop" shape (likely or no
hint) collapses to 1908 B because `this` moves from r9 to r4 (the high-register
`.w` encodings disappear). That is the same root cause documented in
`gun-calccharactercollision-w7610.md`, `-wave131-w7701.md`,
`-wave132-w7714.md` and `-wave133-w7739.md`: the shipped is the `expr` grouping
without the LICM hoist, and the hoist is unconditional for the trivially
rematerializable `t2ADDri %stack.10` / `%stack.11`.

## 4. Reproduction

```
ulimit -v 12000000
cmake --build cmake-build-match --target check -j2     # RATCHET PASS
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN3Gun22calcCharacterCollisionEv
```

## 5. Files changed

`src/game/weapons/Gun.cpp` (the three-line change above) and this note.
Gate: byte 4507 / linked 4507, size-exact 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, `.rel.plt` identical.
