# `Gun::calcCharacterCollision` wave 126 (w7610): the residual is a pure `r4`/`r5`
# swap; the shared `view`/k-loop stack address is hoisted by Early Machine LICM

Worker: w7610. Base master `bf365a964`. Original:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump `0x152f90`, size `1984 B`,
650 insns. Scratch: `/dev/shm/w7610/` (not tracked).

## 0. Result

No code change. HEAD is still the best measured shape.

| build | gate byte/linked | target pct (official) | size |
|---|---:|---:|---:|
| before (HEAD `bf365a964`) | **4506 / 4506** | **93.4** | 1984 |
| after this pass (note only) | **4506 / 4506** | **93.4** | 1984 |

`check` after: `RATCHET PASS`, byte 4506/4524, linked 4506/4524, size-exact
4524/4524, data 843/843, imports 207, lint CLEAN, drift 0.

## 1. The `expr` scatter form is the shipped body with `r4`/`r5` swapped

The `expr` spelling (no named `playerPosition`, subtraction as an argument)

```c++
int distance = (int) AbyssEngine::AEMath::VectorLength(
        enemyPosition - ((Level *) this->level)->getPlayer()->getPosition());
```

compiles to **91.4 %** (local clone; official 92.6) at 1984 B / 650 insns, and
its *stack slots* are exactly the shipped ones:

| value | shipped | `expr` | HEAD (`stmt`) |
|---|---|---|---|
| `i*3` | **r4** | r5 | r5 |
| `playerPosition` | **r4 / sp+44** | r5 / sp+44 | r4 / sp+104 |
| `delta` | **r5 / sp+104** | r4 / sp+104 | r5 / sp+44 |
| `explosive` | sp+20 | sp+20 | sp+20 |
| `td` | sp+24 | sp+24 | sp+24 |
| `fp40` | 5 | 4 | 4 |

The whole `expr` diff against the shipped stream is the `r4`↔`r5` swap plus its
consequences (`mov r6,r4`/`mov r4,r6` vs `mov r6,r5`/`mov r5,r6`, the two
`ldr.w fp,[sp,#40]` reloads that become `add r4,sp,#104`, the `view` block
rematerialisation, the k-loop layout and the pool offsets).  In other words,
**if `i*3` gets `r4`, the `expr` form should be byte-exact.**

## 2. Why `i*3` gets `r5`: the merged `view`/k-loop address is hoisted

`-mllvm -print-after-all` on the `expr` build:

* `Merge disjoint stack slots` merges the `Matrix view` alloca (`%11`, 60 B) and
  the k-loop `enemy->getPosition()` sret alloca (`%13`, 12 B) into one
  `%stack.10` (the two live ranges are disjoint: `view` is in the geometry block,
  the k-loop temp is in the scatter block).
* `Early Machine Loop Invariant Code Motion` hoists `%308 = t2ADDri %stack.10, 0`
  to `bb.5`, the i-loop preheader, together with the other stack addresses.
* After RA the address is a single vreg `%513 = COPY %504` created at `bb.43`
  (post-`isAsteroid`) and live to the k-loop (`16080B`).  Its live range is
  longer than `i*3`'s (`%526`, `2896B`→`10624B`), so greedy RA gives `%513`
  `r4` and `i*3` `r5`.

The shipped instead rematerialises `sp+104` separately at the `view` block
(`0x1533c8`, `add r5,sp,#104`) and at the k-loop (`0x153444`,
`add r4,sp,#104`), so its address vreg is short and `i*3` wins `r4`.  This is
the same root cause documented in `wave125-w7518.md` §5, now confirmed to be
the *only* remaining difference for the `expr` body.

## 3. Variants measured this wave

Fast harness `/dev/shm/w7610/{gen,check,score}.py` (0.25 s per build; local
score is the official normaliser, ~1.2 points below the official `verify`
number).  Format: `pct / size / i3 / pp / delta / fp40 / n`.

| variant | pct | size | i3 | pp | delta | fp40 |
|---|---:|---:|---|---|---|---:|
| HEAD `stmt` | 92.2 | 1984 | r5 | r4/104 | r5/44 | 4 |
| `expr` | 91.4 | 1984 | r5 | r5/44 | r4/104 | 4 |
| `expr` + `do { } while (++k < size())` | 81.9 | 1988 | r5 | r5/44 | r4/104 | 4 |
| `expr` + `expect(mine,0)` | 73.8 | 1984 | r5 | — | — | 4 |
| `expr` + `expect(scatter,0)` | 71.9 | 1992 | r4 | — | — | 4 |
| `expr` + `expect(distance,1)` | 64.1 | 1908 | r0 | — | — | 0 |
| `expr` + `expect(geometries,1)` | 70.9 | 1956 | r0 | r4/44 | r5/104 | 0 |
| `expr` + `expect(owner,0)` | 87.5 | 1984 | r5 | r5/44 | r4/104 | 4 |
| `expr` + `view` before the two `SetAnimationState` | 83.2 | 1988 | r5 | r5/44 | r4/104 | 4 |
| `expr` + `Matrix translated = MatrixSetTranslation(...)` | 81.7 | 1988 | r6 | r5/44 | r4/104 | 4 |
| `expr` + `view = MatrixSetTranslation(...)` | 85.2 | 1992 | r5 | r5/44 | r4/104 | 4 |
| `expr` + named `Vector ep = enemy->getPosition()` | 91.4 | 1984 | r5 | r5/44 | r4/104 | 4 |
| `expr` + `this->targetDir` direct | 91.4 | 1984 | r5 | r5/44 | r4/104 | 4 |
| `expr` + `Vector delta;` before `pp` | 62.4 | 1964 | — | r4/44 | r4/104 | 0 |
| `expr` + `Vector ep = enemyPosition;` at i-loop top | 53.6 | 1976 | — | r5/44 | r4/104 | 0 |
| `expr` + `Matrix view;` at i-loop top | 51.6 | 2024 | r4 | — | — | 0 |
| `expr` + `Matrix view;` at function top | 54.5 | 2016 | r4 | r5/104 | r4/44 | 3 |
| `stmt` + `Matrix view;` at function top | 55.4 | 2016 | r4 | r4/44 | r5/104 | 3 |
| `stmt` + `Matrix view(...)` 12-arg ctor at function top | 55.4 | 2016 | r4 | r4/44 | r5/104 | 3 |
| `stmt` + `MatrixElements viewbuf` at function top | 62.7 | 1952 | r4 | r4/44 | r5/104 | 3 |
| `expr` + `MatrixElements viewbuf` at function top | 61.7 | 1952 | r4 | r5/104 | r4/44 | 3 |
| `expr` + `char viewbuf[60]` at i-loop top | 61.4 | 1948 | r4 | r5/104 | r4/44 | 3 |

**Every** way found to stop the IR merge (declare `view` in a scope that spans
the k-loop, or replace it with a ctor-less buffer) does give `i*3 = r4`, but it
also changes the frame (2016/1952/1948 B instead of 1984), the `view` block
rematerialisation (`fp40 = 3`) and the tail (`s20`/`s24`), so the score falls to
~55–63 %.  The extra code is the out-of-line `Matrix` default ctor (32 B) or the
array-next-to-stack-guard placement (-32 B), both of which the shipped does not
have.

## 4. Conclusion

The shipped body is the `expr` scatter form.  Its only residual is the greedy
allocator's choice of `r4` for the merged `view`/k-loop stack address instead of
`i*3`.  No source shape tried this wave (or in waves 101–125) both prevents the
`view`/k-loop alloca merge and keeps the 1984 B frame / `ABCDE` tail / `fp40=5`.
HEAD's `stmt` body (`F H K L P` tail + `int dmg`) remains the best fuzzy match
at **93.4 %**.

## 5. Reproduction

```
cmake --build cmake-build-match --target check -j8        # RATCHET PASS
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN3Gun22calcCharacterCollisionEv
```

## 6. Files changed

`_work/reconstructions/gun-calccharactercollision-w7610.md` only (note).
Gate: byte 4506 / linked 4506, size-exact 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0.
