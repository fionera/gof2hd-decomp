# `Gun::calcCharacterCollision` wave 128 (w7633): the residual is the RA COPY
# hint on the merged `view`/k-loop stack address, not the source shape

Worker: w7633. Base master `4bbe88776`. Original:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump `0x152f90`, size `1984 B`,
650 insns. Scratch: `/dev/shm/w7633/` (not tracked).

## 0. Result

No code change. HEAD is still the best measured shape.

| build | gate byte/linked | target pct | size |
|---|---:|---:|---:|
| before (HEAD `4bbe88776`) | **4506 / 4506** | **93.4** | 1984 |
| after this pass (note only) | **4506 / 4506** | **93.4** | 1984 |

`check` after: `RATCHET PASS`, byte 4506/4524, linked 4506/4524, size-exact
4524/4524, data 843/843, imports 207, lint CLEAN, drift 0.

## 1. Confirmed: the `expr` body is the shipped shape with `r4`/`r5` swapped

The wave-126 finding is reproduced exactly. The `expr` form

```cpp
int distance = (int) AbyssEngine::AEMath::VectorLength(
        enemyPosition - ((Level *) this->level)->getPlayer()->getPosition());
```

compiles to 92.6154 % (official) at 1984 B / 650 insns, with the shipped stack
slots (`playerPosition = sp+44`, `delta = sp+104`, `enemyPosition = sp+164`,
frame 208) and the shipped tail `A B C D E`. Its scatter block is:

```
mov  r6, r5            ; shipped: mov r6,r4
blx  Level::getPlayer
add  r5, sp, #44       ; shipped: add r4,sp,#44   (playerPosition)
...
add  r4, sp, #104      ; shipped: add r5,sp,#104  (delta)
...
mov  r5, r6            ; shipped: mov r4,r6
```

i.e. a pure `r4`<->`r5` swap. The HEAD `stmt` form scores 93.3846 % because it
keeps `i*3 = r5` but moves `playerPosition` to `sp+104` / `delta` to `sp+44`.

## 2. The mechanism: a long-lived `COPY` of the merged stack address

`-mllvm -print-after-all` on the `expr` build shows the merged
`%stack.10` (`Matrix view` 60 B + k-loop `Player::getPosition()` sret 12 B) is
hoisted to the i-loop preheader `bb.5` as `%308 = t2ADDri %stack.10, 0`.
The post-RA MIR then shows:

```
15008B  %498 = t2ADDri %stack.10, 0
15016B  %513 = COPY %498          ; live to the k-loop (16080B/16176B)
15072B  $r2 = COPY %513           ; TransformSetLocal
16080B  $r0 = COPY %513           ; k-loop getPosition sret
16176B  $r1 = COPY %513           ; k-loop Vector::operator=
```

`%513` is the long-lived vreg. Greedy RA gives it `r4` (its `COPY` carries a
register preference from `%498`), which forces `i*3` (`%526`) to `r5`. The
shipped instead rematerialises the address separately at the view block
(`add r5,sp,#104`) and at the k-loop (`add r4,sp,#104`), so it has no
long-lived `COPY` and `i*3` wins `r4`.

The merged stack slot itself is correct and present in both builds: the
shipped's view and k-loop sret are both at `sp+104`, and the frame is 208 B.

## 3. Variants measured this wave (fast local scorer, official normaliser)

Format: `pct / size / i3 / pp / delta / tail / notes`.

| variant | pct | size | i3 | pp | delta | notes |
|---|---:|---:|---|---|---|---|
| HEAD `stmt` | 93.3846 | 1984 | r5 | r4/sp+104 | r5/sp+44 | best |
| `expr` | 92.6154 | 1984 | r5 | r5/sp+44 | r4/sp+104 | shipped shape, swapped |
| `expr` + `Vector &&pp = ...` | 93.3846 | 1984 | r5 | r4/sp+104 | r5/sp+44 | becomes `stmt` |
| `expr` + `const Vector &pp = ...` | 93.3846 | 1984 | r5 | r4/sp+104 | r5/sp+44 | becomes `stmt` |
| `expr` + named `Vector delta` | 92.6154 | 1984 | r5 | r5/sp+44 | r4/sp+104 | same as `expr` |
| `expr` + `Vector(pp)` copy | 92.6154 | 1984 | r5 | r5/sp+44 | r4/sp+104 | same as `expr` |
| `expr` + `const Vector &ep = enemyPosition` | 92.6154 | 1984 | r5 | r5/sp+44 | r4/sp+104 | same as `expr` |
| `expr` + `Vector ep;` at i-loop top, `ep = getPosition(); *td = ep` | 59.6 | 1976 | r5 | — | — | default ctor + extra copy |
| `expr` + `Vector ep = position;` before `isAsteroid` | 53.3 | 1932 | r5 | — | — | frame 224 |
| `expr` + `MatrixElements viewbuf` at i-loop top, `memcpy` | 62.3 | 1948 | r4 | r5/sp+104 | r4/sp+44 | frame 224, merge prevented |
| `expr` + `hitFlags[i]=1` moved after the k-loop | 73.4 | 1964 | **r4** | r5/sp+44 | r4/sp+104 | extra r4/r5 spills, wrong tail |
| `expr` + `hitPositions[i]=position` moved after the k-loop | 58.7 | 1952 | r5 | r5/sp+44 | r4/sp+104 | wrong order |
| `expr` + `(void)(i*3)` after the k-loop | 92.6154 | 1984 | r5 | r5/sp+44 | r4/sp+104 | optimised away |
| `expr` + `MatrixSetTranslation(..., positions.data()[i].x/y/z)` | 86.2 | 1992 | r5 | — | — | not CSE'd |
| `expr` + `Matrix *viewPtr = &view` | 92.6154 | 1984 | r5 | r5/sp+44 | r4/sp+104 | pointer optimised away |
| `expr` + `td` as `Vector &` | 92.6154 | 1984 | r5 | r5/sp+44 | r4/sp+104 | same as `expr` |
| `expr` + `int i` / `i != count` | 92.4615 | 1984 | r5 | r5/sp+44 | r4/sp+104 | slightly worse |
| `expr` + first mine anchor `expect(...,0)` | 71.5 | 2000 | r5 | — | — | wrong branch layout |
| `expr` + anchor on whole `&&` chain | 82.2 | 1996 | r5 | — | — | wrong branch layout |
| `expr` + anchor on last comparison | 84.4 | 1984 | r5 | — | — | wrong branch layout |

The only variant that reaches `i*3 = r4` with the shipped `expr` slots is the
`hitFlags` move, but it also spills `i*3`/`pp` to `sp+36`/`sp+28` around the
`getPlayer` call, emits `pp = r5` / `delta = r4` (the opposite of the shipped),
and shifts the tail/pool (1964 B). It is not the shipped body.

## 4. Conclusion

The shipped body is the `expr` form. Its only residual is the greedy
allocator's long-lived `COPY` of the merged `view`/k-loop stack address, which
carries a register preference and takes `r4` ahead of `i*3`. The shipped
rematerialises that address at the view block and the k-loop instead, freeing
`r4` for `i*3`. No source shape tried this wave (or in waves 101-127) both
removes the `COPY` and keeps the 1984 B frame / `ABCDE` tail / shipped
`pp=r4, delta=r5`.

Best measured variant remains **HEAD at 93.3846 % / 1984 B (650 insns)**.

## 5. Reproduction

```
cmake --build cmake-build-match --target check -j6        # RATCHET PASS
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN3Gun22calcCharacterCollisionEv
```

## 6. Files changed

`_work/reconstructions/gun-calccharactercollision-wave128-w7633.md` only (note).
Gate: byte 4506 / linked 4506, size-exact 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0.
