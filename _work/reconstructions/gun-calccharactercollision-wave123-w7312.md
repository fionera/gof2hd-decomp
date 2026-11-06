# `Gun::calcCharacterCollision` wave 123 (w7312): HEAD's 93.4 % still wins;
# the new `scatter`-`expect` family reaches the correct pp/delta slots *and*
# `pp=r4`, but with `i*3` in `r6`; no shape gets all four shipped properties

Worker: w7312. Base master `1c8c9b20e` (wave 122). Original:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).
Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump `0x152f90`, size `1984 B`,
650 instructions. Scratch: `/dev/shm/w7312/` (not tracked).

## 0. Result

| build | gate byte/linked | target pct | size |
|---|---:|---:|---:|
| before (HEAD `1c8c9b20e`) | **4506 / 4506** | **93.4000** | 1984 |
| after this pass | **4506 / 4506** | **93.4000** | 1984 |

No code change: analysis only. `check` after: `RATCHET PASS`, byte 4506/4524,
linked 4506/4524, avg 99.94 %, lint CLEAN, drift 0. `git status --short` clean.

## 1. Residual (unchanged from waves 119-122)

HEAD is still the wave-121 shape. Calls are identical: 59 `blx` in the same
order to the same targets (verified by extracting the `<symbol>` of every
`blx` from both disassemblies and diffing). The literal pool is at the same
addresses (`0x1532f4`..) and holds the same constants (`0.0`, `0.2`,
`-1000000.0`, GOT `__stack_chk_guard`). The residual is purely RA/scheduling:

| property | shipped | HEAD |
|---|---|---|
| `this` / `enemy` / `td` / `i` | r9 / r8 / r6 / fp | same |
| `i*3` | **r4** | **r5** |
| `playerPosition` (`pp`) | **r4 / sp+44** | r4 / sp+104 |
| `delta` | **r5 / sp+104** | r5 / sp+44 |
| k-loop layout | **rotated** `[body,latch,header]` | `[header,body,latch]` |
| `ldr.w fp,[sp,#40]` reload after `isAsteroid` | present | replaced by `add r4,sp,#104` |

43 unmatched instructions per side: `i*3` register (9), `pp`/`delta` slots
(2), the two extra `add r4,sp,#104` (2 + branch-target shifts), view-block
rematerialisation (4), k-loop rotation (12), branch targets the rest.

## 2. MIR root cause (reproduced this wave)

`-mllvm -print-after-all` on HEAD and on the expression form shows:

* `Merge disjoint stack slots` maps `fi#4 -> stack.10` and `fi#5 -> stack.11`;
  `stack.10` is the align-8 60 B slot (`sp+104`), `stack.11` the align-4 slot
  (`sp+44`).
* `Early Machine Loop Invariant Code Motion` hoists both `t2ADDri %stack.10`
  and `t2ADDri %stack.11` into the i-loop preheader (bb.8), `stack.10` first.
* Greedy RA then colours `stack.10 -> r4`, `stack.11 -> r5`; `i*3`
  (`t2ADDrs %16, %16, 10`) starts later in the loop body, so it gets `r6`
  when both stack addresses are live, or shares the register of whichever
  stack address is *not* hoisted.

The statement form (`Vector playerPosition = ...; VectorLength(enemyPosition -
playerPosition)`) makes `pp = fi#4 = stack.10 = sp+104`; i.e. HEAD's wrong
slot. The expression form (`VectorLength(enemyPosition - getPlayer()->
getPosition())`) makes `pp = fi#5 = stack.11 = sp+44`; i.e. the *shipped*
slot. So the expression form is required for the slot assignment.

## 3. New this wave: the `scatter`-`expect` family

Adding `} else if (__builtin_expect(scatter, 1)) {` on top of the expression
form flips the LICM hoist order (`stack.11` before `stack.10`), so
`pp = stack.11 -> r4` and `delta = stack.10 -> r5`: **both the slots and the
`pp` register become correct**. The cost is that `i*3` is pushed to `r6`
(both stack addresses are live in the preheader) and the loop is not rotated.

| variant | pct | size | `i*3` | `pp` | `delta` |
|---|---:|---:|---|---|---|
| HEAD | **93.4** | 1984 | r5 | r4 / 104 | r5 / 44 |
| expression form (`expr1`) | 92.6 | 1984 | r5 | r5 / 44 | r4 / 104 |
| expression + `pgnew` | 85.6 | 1980 | **r4** | **r4 / 44** | **r5 / 104** |
| expression + `pgnew` + plain k | 86.6 | 1980 | **r4** | **r4 / 44** | **r5 / 104** |
| `scatexp` + expression | 86.0 | 1980 | r6 | **r4 / 44** | **r5 / 104** |
| `scatexp` + expression + `pgnew` | 86.5 | 1984 | r6 | **r4 / 44** | **r5 / 104** |
| `scatexp` + expression + `pgnew` + plain k | 86.9 | 1984 | r6 | **r4 / 44** | **r5 / 104** |
| `scatexp` + expression + `emp0` | 85.3 | 1984 | r6 | **r4 / 44** | **r5 / 104** |
| `scatexp` + expression + `isRocket0` | 85.4 | 1984 | r6 | **r4 / 44** | **r5 / 104** |

`pgnew` = move the `if (this->playerGun)` block after `enemy->setHitVector`
(the wave-121 lever). It is what restores `i*3 = r4` in the expression form.

### 3.1 The `emp0` / `isRocket0` 1984 B anchors (re-measured)

The wave-121/122 anchors still reproduce exactly:

| variant | pct | size | map |
|---|---:|---:|---|
| expression + `pgnew` + plain k + `empDamage` expect 0 | 90.1 | 1984 | all four correct |
| expression + `pgnew` + plain k + `isRocket` expect 0 | **90.8** | 1984 | all four correct |
| expression + `pgnew` + plain k + `isRocket` expect 1 | 86.1 | 1980 | all four correct |

`expp_kplain_isrocket0` has 660 instructions and ends at the same address as
the original, but still loses 9.2 % because (a) the `ldr.w fp,[sp,#40]` and
`ldr r6,[sp,#24]` reloads after `isAsteroid` are absent, and (b) the
`explodeOnContact`/`td` spill slots are transposed (`sp+24`/`sp+20` vs the
shipped `sp+20`/`sp+24`). Both are StackColoring/MachineScheduler artefacts
that the expression form introduces and no source spelling tried here moves
back.

## 4. Other levers tried this wave (all <= 93.4)

* k-loop forms on HEAD: `__builtin_expect(distance < magnitude, 1)` -> 64.5
  (RA collapses to `this=r4`, 1908 B); `!__builtin_expect(distance >=
  magnitude, 1)` -> 93.4 (identical to HEAD); `while (k < size) { ...; ++k; }`
  -> 93.4 (identical); `for (unsigned k = 0; __builtin_expect(k < size, 1);
  ++k)` -> 54.9; `__builtin_expect` on the i-loop condition -> 93.4.
* falloff clamp as two `if`s / `min`/`max` -> 86.3 / 86.3 (size 1960);
  `empDamage` block before `damage` -> 91.8.
* `mine` / `scatter` / `bomb` as `int` instead of `bool` -> 93.4 (identical);
  `scatter` via ternary / explicit `(int)` cast -> 93.4; `scatter` declared
  before `mine` -> 92.2; `explosive` computed before `mine`/`scatter` -> 51.8.
* `explodeOnContact` / `explosive` assigned directly (`= bomb || mine`,
  `int explosive = bomb || isRocketLike(...)`) -> 93.4 (identical).
* `const Vector position` / `const Vector velocity` -> 93.4; `positions[i]`
  instead of `positions.data()[i]` -> 93.4; `Vector *posData =
  positions.data()` before the loop -> 59.8; inside the loop -> 93.1.
* `Array<Player*> *enemies = this->enemies` hoisted -> 93.4.
* `hitFlags`/`hitPositions` order swap -> 92.8; `lifetimes`/`lastHitKIPlayer`
  order swap -> 92.9; `unsigned distance` -> 92.9.
* `enemyPosition` declared before `position`/`velocity` -> 87.1.
* `if (mine) {...} if (scatter) {...}` (separate `if`s) -> 61.7 on the
  statement form, 92.6 on the expression form.
* `Vector delta = enemyPosition - playerPosition; VectorLength(delta)` ->
  93.4 (identical to HEAD).
* radius split into `else if (distance > 10000)` -> 82.2; `(float) radius *
  3.0f` -> 93.1; nested `getKIPlayer()` -> 80.9.
* `__builtin_expect` on the scatter inner `getKIPlayer()/field_0x71` -> 83.0 /
  79.1; on `distance > 20000` -> 83.3; on `distance > 10000` -> 92.6; on
  `useCustomRadius` -> 92.6; on the mbox first term -> 83.7.
* `volatile int dv = this->damage` -> 86.2; plain `this->damage` -> 84.4;
  `(float) damage * 0.75f` -> 81.2.
* explicit `unsigned i3 = i * 3` (used via `data()[i3 / 3]`) -> 58.2 /
  57.9 (the division changes the IR).

## 5. Root cause / conclusion

The shipped function simultaneously needs

1. `this=r9, enemy=r8, td=r6` (HEAD has this),
2. `i*3=r4`,
3. `pp=r4/sp+44, delta=r5/sp+104`,
4. the k-loop rotated,

and, in addition, the `isAsteroid` post-call reloads (`ldr.w fp,[sp,#40]`,
`ldr r6,[sp,#24]`) and the `explodeOnContact`/`td` spill slots at
`sp+20`/`sp+24`.

The expression form fixes (3) but swaps the `pp`/`delta` registers; the
`scatter`-`expect` family fixes (3) *and* the registers but forces `i*3` to
`r6`; `pgnew` restores `i*3=r4` but shrinks the function to 1980 B and
transposes the spill slots. The `emp0`/`isRocket0` anchors restore 1984 B and
all four properties but still pay the missing reloads and the transposed
spill slots, capping them at 90.1 / 90.8 %.

HEAD remains the best compromise because its instruction offsets are exactly
the shipped ones everywhere except the five clusters above, and the verifier
compares intra-function branch targets literally. Best measured variant is
still HEAD at **93.4000 % / 1984 B** (607/650).

## 6. Reproduction

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

MIR: `clang++ ... -mllvm -print-after-all -mllvm
-filter-print-funcs=_ZN3Gun22calcCharacterCollisionEv` (the NDK clang is a
release build, so `-debug-only=regalloc` is unavailable; `-print-after-all`
still shows the `Early Machine LICM`, `Merge disjoint stack slots`, `Greedy
Register Allocator`, `Stack Slot Coloring` and `Prologue/Epilogue` MIR).
