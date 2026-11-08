# Hud::draw — wave 143 (w8814): the 78th LSDA entry is the `filled` lifetime marker, not the `filled` computation

**Status:** source change landed, 2026-11-08 (wave 143). Worktree `wt/w8814`,
base `master bf083ab41` (wave 142). Target `_ZN3Hud4drawExxP9PlayerEgobjj`
(`src/game/ui/Hud.cpp`, `Hud::draw`), original size **9332 B** (VMA `0x163b90`).

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size-exact 4524/4524, `data_exact 843/843`, imports **207**,
lint **CLEAN**, drift **0**, `stub_zero_size 0`, `missing 0`, `wrong_type 0`,
`extra 0`, symbol parity 5370/5370.

Raw same-address diff (primary metric): **1362 -> 1325 B** (function stays
**9332 B**). `.ARM.extab` stays **156,312 B** (base `0x1a3b78`), `.rodata` stays
`0x1c9e10`, `.rel.plt` byte-identical. The landed source is the dock2 local order
**`barYOffset, barH, text, halfW`** (YBTH). It is **METRIC-ONLY** (see §3): the
emitted call order is not the original's.

---

## 0. TL;DR

1. The wave-142 note blamed the 78th call-site entry on the `filled`
   *computation* sitting between `DrawImage2D` and `DrawRegion2D`. That is the
   wrong root cause. The split is the **`filled` local's lifetime-marker cleanup
   scope**: Clang IRGen pushes `CallLifetimeEnd` (`NormalEHLifetimeMarker`, a
   non-EH scope) at *every* local `VarDecl`, and `getInvokeDestImpl` caches the
   landing pad on `EHStack.begin()`; a non-EH scope pushed after `DrawImage2D`
   has no cached pad, so `DrawRegion2D`/`GetTextWidth`/`DrawString` get a second
   landing pad. The `filled` computation itself is a codegen no-op.
2. Swept all 24 permutations of the four dock2 locals
   `{halfW(H), barH(B), barYOffset(Y), String text(T)}`. The best extab-exact
   order is **YBTH: raw 1325, 9332 B, `.ARM.extab` 156,312 exact, `.rodata`
   0x1c9e10 exact**. `BTYH` 1326, `BYTH` 1327, `BTHY` 1328, `YHTB` 1359,
   `HYTB` 1361 are also extab-exact. The faithful text-first orders `THBY`
   (w8715) and `THYB` are 1352 / 1367 with extab +4 (156,316).
3. **METRIC-ONLY caveat.** The original's dock2 call order is
   `getText` (0x165bbe) / `String` ctor (0x165bc8) -> `GetImage2DWidth`
   (0x165bd4) -> `GetImage2DHeight` (0x165bf4). YBTH emits `GetImage2DHeight`
   at 0x165bba *before* `getText` at 0x165bd8. The coordinator may keep the
   faithful `THBY` (raw 1352) if source fidelity wins over the 27-byte metric.
4. YBTH's extab-size exactness is a **count coincidence**, not a structural
   match: YBTH has 3 dock2 call-site entries (`GetImage2DWidth` + the
   `filled`-split pair) vs the original's 3 (`GetImage2DWidth` +
   `GetImage2DHeight` + the merged draw range). The wave-142 base has 3 too
   (`GetImage2DHeight` + the split pair). The faithful `THBY` has 4 (width +
   height + split pair), hence extab +4. To get the original's *content* the
   `filled` split must be merged while keeping both width and height entries;
   every source shape that does so grows the frame (§4).

## 1. The landed source change

Replaces the wave-142 dock2 block

```cpp
            int halfW = (int) ((float) Globals::Canvas->GetImage2DWidth(this->image_0x37c) * 0.5f);
            String text(*Globals::gameText->getText(this->field_0x277 ? 318 : 317), false);
            int barH = Globals::Canvas->GetImage2DHeight(this->image_0x37c);
            int barYOffset = Globals::layout->field_0x218;
```

with

```cpp
            int barYOffset = Globals::layout->field_0x218;
            int barH = Globals::Canvas->GetImage2DHeight(this->image_0x37c);
            String text(*Globals::gameText->getText(this->field_0x277 ? 318 : 317), false);
            int halfW = (int) ((float) Globals::Canvas->GetImage2DWidth(this->image_0x37c) * 0.5f);
```

Nothing else in the function changes.

## 2. The 24-permutation sweep (all 9332 B unless noted)

| order | raw | `.ARM.extab` | `.rodata` |
|---|---:|---|---|
| **YBTH** | **1325** | **156312 exact** | **0x1c9e10** |
| BTYH | 1326 | exact | exact |
| BYTH | 1327 | exact | exact |
| BTHY | 1328 | exact | exact |
| THBY (w8715, faithful) | 1352 | 156316 | 0x1c9e20 |
| YHTB | 1359 | exact | exact |
| HYTB | 1361 | exact | exact |
| THYB | 1367 | 156316 | 0x1c9e20 |
| H/B/Y-first permutations (16) | 1900–1906 | exact | exact, 9328 B |

## 3. Why the split happens (IR-level, NEW)

The dock2 IR (frontend, `clang -Oz -S -emit-llvm`) has four landing-pad blocks
for the draw sequence:

```
; <label>:2752  landingpad cleanup ; br %2805   <- unwind of GetImage2DWidth
; <label>:2754  landingpad cleanup ; br %2805   <- unwind of GetImage2DHeight
; <label>:2801  landingpad cleanup ; br %2805   <- unwind of SetColor, DrawImage2D
; <label>:2803  landingpad cleanup ; br %2805   <- unwind of DrawRegion2D, GetTextWidth, DrawString
```

`CodeGenFunction::getInvokeDestImpl` (clang/lib/CodeGen/CGException.cpp) does:

```cpp
  llvm::BasicBlock *LP = EHStack.begin()->getCachedLandingPad();
  if (LP) return LP;
  ...
  for (EHScopeStack::iterator ir = EHStack.begin(); true; ++ir) {
    ir->setCachedLandingPad(LP);
    if (!isNonEHScope(*ir)) break;
  }
```

`EmitAutoVarAlloca` (CGDecl.cpp) pushes `EHStack.pushCleanup<CallLifetimeEnd>(
NormalEHLifetimeMarker, ...)` for every local `VarDecl`. That is a **non-EH
scope**. When `float filled = ...;` is declared between `DrawImage2D` and
`DrawRegion2D`, the new lifetime-marker scope is `EHStack.begin()` at the
`DrawRegion2D` invoke, it has no cached pad, so a new landing pad is built and
cached on the enclosing `String` cleanup scope; the two earlier invokes keep the
old pad. Hence the split. Declaring `filled` before `SetColor` (or removing the
`VarDecl` by inlining) merges the pads — confirmed in the IR — but grows the
frame (§4).

The wave-142 note's claim that "moving `filled` before either call is a codegen
no-op" is wrong: it is a codegen no-op only for the *asm scheduling* of the
`filled` arithmetic, not for the EH scopes.

## 4. Negative results (all measured)

| variant | raw | size | `.ARM.extab` |
|---|---:|---:|---:|
| YBTH (landed) | **1325** | **9332** | **156312** |
| THBY (w8715) | 1352 | 9332 | 156316 |
| THBY + `const float filled` | 1352 | 9332 | 156316 |
| THBY + `float filled = progress * halfW` (no cast) | 1352 | 9332 | 156316 |
| THBY + `double filled` | 1949 | 9344 | 156316 |
| THBY + `float filled;` before `SetColor`, assign after `DrawImage2D` | 8001 | 9368 | 156312 |
| THBY + `float filled = ...` before `SetColor` | 8005 | 9368 | 156312 |
| THBY + inlined `filled` expression | 8001 | 9368 | 156312 |
| THBY + `float filled = ...` before `y` | 8005 | 9368 | 156312 |
| THBY + `float filled = ...` before `barYOffset` | 8020 | 9372 | 156312 |
| THBY + `float filled;` after `halfW` (before `text`) | 8025 | 9372 | 156308 |
| THBY + `float filled;` at the top of the outer bar block, assign after `DrawImage2D` | 8001 | 9368 | 156312 |
| THBY + `float filled = ...` after `halfW` (before `text`) | 8023 | 9372 | 156308 |
| THBY + `barYOffset` after `top` / after `y` | 1432 | 9332 | 156316 |

Every source shape that merges the `filled` landing pads (extab 156312) grows
the frame 9332 -> 9368/9372. The growth is a **global register-allocation
change**, not an extra alloca: the `filled` value becomes live across the
`SetColor`/`DrawImage2D` calls (or the alloca is kept), the prologue stack slots
shift by +4 (`[sp,#88]` -> `[sp,#92]`), and the damage region gains
`mov sl, fp` / `mov fp, r8` shuffles. So the original's structure (merged pads
*and* 9332 B) is not reachable from either the wave-142 base or w8715 by moving
the `filled` declaration alone.

## 5. Reproduction

```
cd /home/debian/GalaxyOnFire2/wt/w8814
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN3Hud4drawExxP9PlayerEgobjj
```

Scratch harness in `/dev/shm/w8814`: `fdiff.py`, `measure.sh`,
`regiondiff.py`, `lsda2.py` (4-uleb call-site decoder), `perm4.py`,
`sweep_filled2.py`, `sweep_filled3.py`, `sweep_yoff.py`, `lpcheck.py`
(prints the dock2 `invoke`/`unwind` structure from `-emit-llvm`), `genir.sh`,
`Hud.base.cpp` (wave-142 base), `Hud.tf.cpp` (w8715 THBY), `Hud.ybth.cpp`
(landed YBTH).
