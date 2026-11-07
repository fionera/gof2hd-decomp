# `Gun::calcCharacterCollision` wave 129 (w7654): the Android-clang RA is not
# the vanilla-7.0.0 llc RA; the `expr` form is confirmed, but the residual
# `r4`/`r5` split is a compiler-build difference, not a source-shape one

Worker: w7654. Base master `fee528cbe`. Original:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump `0x152f90`, size `1984 B`,
650 insns. Scratch: `/dev/shm/w7654/` (not tracked).

## 0. Result

No code change. HEAD is still the best measured shape.

| build | gate byte/linked | target pct | size |
|---|---:|---:|---:|
| before (HEAD `fee528cbe`) | **4506 / 4506** | **93.4** | 1984 |
| after this pass (note only) | **4506 / 4506** | **93.4** | 1984 |

`check` after: `RATCHET PASS`, byte 4506/4524, linked 4506/4524, size-exact
4524/4524, data 843/843, imports 207, lint CLEAN, drift 0.

## 1. New finding: the local `llc` is not the original compiler's RA

Wave 128 (w7633) used `/home/debian/llvm7build/build/bin/llc` to trace the
greedy allocator. That `llc` is built from **vanilla LLVM 7.0.0** (plus the
wave-90..128 `getenv` instrumentation). The original `.so` was built with the
**Android clang 7.0.2 at r328903** (`1d739ffb0366421d383e04ff80ec2ee591315116`),
whose `lib/CodeGen/RegAllocGreedy.cpp` is a *different file*:

* it is self-contained (the `RAGreedy` class moved into the `.cpp`; the Android
  tree has no `RegAllocGreedy.h`);
* it drops `isSplitBenefitWorthCost()` / `huge-size-for-split` (the 7.0.0
  "rematerializable interval > 5000 slots -> skip region split" shortcut);
* it adds the `EvictionTrack` / `canReassign` / `splitCanCauseLocalSpill`
  machinery and the "No live bundles, defer to splitSingleBlocks()" check;
* it replaces `DEBUG(...)` with `LLVM_DEBUG(...)`.

I fetched that file from `android.googlesource.com/toolchain/llvm` at the
`1d739f…` commit and rebuilt `llc` with it (one-line `#define DEBUG(X)
LLVM_DEBUG(X)` shim). The Android-source `llc` was then restored to the
instrumented 7.0.0 source afterwards.

## 2. The Android-source `llc` *does* produce the shipped allocation

On the shipped `expr` body

```cpp
int distance = (int) AbyssEngine::AEMath::VectorLength(
        enemyPosition - ((Level *) this->level)->getPlayer()->getPosition());
```

the Android-source `llc` (thumbv7 triple, no `-disable-fp-elim`) emits:

```
mov r6, r4                 ; spill i*3 (r4)
bl  Level::getPlayer
add r4, sp, #36            ; pp
bl  PlayerEgo::getPosition
add r5, sp, #96            ; delta
...
mov r2, r4 / mov r0, r5    ; operator-(delta, enemyPosition, pp)
...
add r5, sp, #96            ; view block: one add, reused for memcpy /
                           ; MatrixSetTranslation / TransformSetLocal
add r4, sp, #96            ; k-loop getPosition sret (separate remat)
```

i.e. `i*3 = r4`, `pp = r4`, `delta = r5`, and a single-add view block — the
shipped shape (modulo the llc's 200-byte frame vs the shipped's 208).

The **actual clang 7.0.2 integrated backend** on the same source emits
`i*3 = r5`, `pp = r5/sp+44`, `delta = r4/sp+104`, and rematerialises `sp+104`
three times in the view block (the wave-128 `expr` result).

So the residual is not the source semantics: it is a codegen difference
between the `llc` driver and the clang driver on the *same* `.ll`. The only
visible MIR difference is the frame layout (llc `sub sp,#200` with view at
`sp+96`; clang `sub sp,#208` with view at `sp+104`), which shifts every slot
index and changes the greedy split. The clang frame (`208`, `sp+44`/`sp+104`)
is the shipped one, so the clang build is the faithful one; the llc is a
useful but *not* faithful proxy for the original RA.

## 3. Source shapes tried this wave (all keep `i*3 = r5` with the `expr` slots)

Fast harness `/dev/shm/w7654/{compile.sh,score.py,check.py,batch*.py}`
(0.2 s per build; local score is the official normaliser, ~2 points below the
official `verify` number).

Format: `pct(local) / size / i3 / pp / delta / tail / n`.

Scatter spellings on the `expr` base (all 1984 B, 650 insns unless noted):

| variant | pct | i3 | pp | delta | notes |
|---|---:|---|---|---|---|
| `expr` baseline | 91.4 | r5 | r5/44 | r4/104 | shipped slots, swapped regs |
| `stmt` (HEAD) | 92.2 | r5 | r4/104 | r5/44 | shipped regs, swapped slots |
| `Vector delta = enemyPosition - getPlayer()->getPosition()` | 91.4 | r5 | r5/44 | r4/104 | same as `expr` |
| `Vector pp = ...; Vector delta = enemyPosition - pp` | 92.2 | r5 | r4/104 | r5/44 | same as `stmt` |
| `const Vector &pp = getPlayer()->getPosition()` | 92.2 | r5 | r4/104 | r5/44 | same as `stmt` |
| `auto pp = getPlayer()->getPosition()` | 92.2 | r5 | r4/104 | r5/44 | same as `stmt` |
| `Vector pp(getPlayer()->getPosition())` | 92.2 | r5 | r4/104 | r5/44 | same as `stmt` |
| `int distance; distance = ...` | 91.4 | r5 | r5/44 | r4/104 | same as `expr` |

View-block spellings on `expr` (all 1984 B, 650 insns unless noted):

| variant | pct | i3 | pp | delta | notes |
|---|---:|---|---|---|---|
| `Matrix &vr = view` | 91.4 | r5 | r5/44 | r4/104 | identical |
| `Matrix *vp = &view` | 91.4 | r5 | r5/44 | r4/104 | identical |
| `const Matrix view` + cast | 91.4 | r5 | r5/44 | r4/104 | identical |
| `Matrix vcopy = view` after MatrixSetTranslation | 75.4 | r6 | — | — | 2000 B |
| `Matrix view` before the two SetAnimationState | 82.0 | r5 | r5/44 | r4/104 | 2040 B |
| `PaintCanvas *canvas = Globals::Canvas` (whole block) | 78.7 | r6 | r4/44 | r5/104 | 1976 B, tail ABCE |
| `canvas` + `unsigned geometry = geometries[i]` | 67.2 | **r4** | **r4/44** | **r5/104** | 1964 B, tail CE |
| `unsigned geometry = geometries[i]` only | 71.0 | r5 | r5/44 | r4/104 | 1972 B |
| `MatrixSetTranslation(view, position)` (Vector overload) | 85.2 | r5 | r5/44 | r4/104 | 1992 B |

K-loop spellings on `expr` (all 1984 B, 650 insns unless noted):

| variant | pct | i3 | pp | delta | notes |
|---|---:|---|---|---|---|
| `Vector ep = enemy->getPosition(); *td = ep;` | 91.4 | r5 | r5/44 | r4/104 | identical |
| `Vector &d = *td; d = ...; d -= position` | 91.4 | r5 | r5/44 | r4/104 | identical |
| `this->targetDir` direct | 91.4 | r5 | r5/44 | r4/104 | identical |
| `*td = enemy->getPosition() - position` | 60.8 | — | — | — | 1956 B, tail CE |
| `Vector ep = ...; ep -= position; *td = ep` | 59.9 | — | — | — | 1892 B |

The `canvas` + `geometry` variant is the only one that reaches the shipped
`i*3=r4, pp=r4/44, delta=r5/104` triple, but it drops 20 instructions (1964 B)
and the `ABCDE` tail, so it is not the shipped body.

## 4. Conclusion

The shipped body is the `expr` scatter form with the shipped 208-byte frame
(`pp=sp+44`, `delta=sp+104`, `view=sp+104`). The Android clang 7.0.2 RA does
produce the shipped allocation on that IR when driven through `llc` with a
200-byte frame, but the clang driver's 208-byte frame changes the slot indices
and the greedy split, leaving the long-lived `%513 = COPY %498` of `&stack.10`
and `i*3 = r5`. No source shape tried this wave (or waves 101–128) both
removes that COPY and keeps the 1984 B frame / `ABCDE` tail / shipped
`pp=r4, delta=r5`.

Best measured variant remains **HEAD at 93.3846 % / 1984 B (650 insns)**.

Open question for the next wave: find what makes the clang driver's frame 208
instead of the llc's 200 (the 8-byte delta is the whole difference between the
two RA outcomes). Candidates: the stack-slot alignment of the two 60-byte
`Matrix` objects (`fi#10` align=8 vs `fi#11` align=4), or the `-g` debug
`DBG_VALUE`s that the clang driver keeps and that the `-g0` `.ll` path drops
before stack-slot colouring.

## 5. Reproduction

```
cmake --build cmake-build-match --target check -j6        # RATCHET PASS
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN3Gun22calcCharacterCollisionEv
```

Android RA source: `https://android.googlesource.com/toolchain/llvm/+/1d739ffb0366421d383e04ff80ec2ee591315116/lib/CodeGen/RegAllocGreedy.cpp?format=TEXT`.

## 6. Files changed

`_work/reconstructions/gun-calccharactercollision-wave129-w7654.md` only (note).
Gate: byte 4506 / linked 4506, size-exact 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0.
