# `RocketGun::update(int)` — pass 16 (w5315, wave 105)

**Status:** research note, 2026-11-04. Worktree `wt/w5315`, branch
`wave105-w5315`, base `835748c18`. **No source change landed**: every source
shape tried this pass scores <= 86.9 %. Tree clean; gate unchanged
(byte 4500, linked 4500, size 4524/4524, data 843/843, imports 207, lint
CLEAN, drift 0).

Target `_ZN9RocketGun6updateEi`: original **1908 B / 635 insns**, ours
**1908 B / 636 insns**, official verify score **86.9**.

Earlier notes: `rocketgun-update-wf210.md` (pass 15), `rocketgun-update-wf109.md`
(pass 14), `rocketgun-update-w103.md` (pass 13), `rocketgun-update-w5010.md`
(pass 12), `-w4910`, `-w4812`, `-w4710`, `-w4512`, `-w4412`, `-w4265`,
`-w4248`, `-w4216`, `-w4111`, `-w4011`, `-w3831`, `-w3811`.

---

## 1. Baseline (NDK r18b clang 7.0.2, this worktree)

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
```
```
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports 207
RATCHET PASS
```

`_ZN9RocketGun6updateEi`: 1908 B both sides, **635 vs 636** instructions,
**86.9 %**. Every other `RocketGun` function is byte-exact.

---

## 2. REA evidence: the body is semantically faithful

`rea decompile ... --procedure _ZN9RocketGun6updateEi` (Ghidra 12.1.2,
`/dev/shm/w5315/rea-update.json`) confirms the checked-in source statement by
statement:

* `Gun::update(gun, elapsed)` first;
* muzzle block: `Level::getPlayer`, `PlayerEgo::getPosition`,
  `barrel = gun->offset; barrel.z -= 100.0f`,
  `MatrixRotateVector(player->player->transform[0], barrel)`,
  `AEGeometry::setPosition`, `setScaling(scale,scale,scale)` with
  `scale = (float)(int)((delayTimer / -200.0f + 1.0f) * 255.0f) / 255.0f`,
  `MatrixGetDir`, `setDirection(forward, {0,1,0})`;
* `wasFiring = gun->delayActive`;
* `hitSmall` launch block: `activeShotId = -1`, `fadeTimer = 0`,
  `hitSmall = 0`, `TransformGetLocal` + `MatrixSetTranslation` on the local
  transform, `frame = *TransformGetLocal`, `side = normalize(cross(up,heading))`,
  `up = normalize(cross(heading,side))`, write the 9 rotation words,
  `TransformSetLocal`; then the `trailMatrices == nullptr` / `z == 50000` /
  else trail branches and the cluster-salvo recursion;
* `active && count == 1 && positions[0].z == 50000` fade block;
* the loop: `ahead = positions[i] + velocities[i] * dt * 0.5f`;
  `seekEnemy` under `homing && (trailMatrices || lifetimes[i] < initialLifetime - 1000)`;
  the cluster corkscrew (`__aeabi_uidiv(total*i, count)`, `Sinf`, `Cosf`,
  `VectorCross`, `VectorNormalize`, two `+=` scaled by `s+s`/`c+c` and `dt`);
  the trail matrix update; the trail-timer fade.

No semantic discrepancy was found. **The residual is codegen only.**

---

## 3. Exact remaining diff (normalised instruction stream)

`objdump` of both functions, registers/immediates normalised, gives
`difflib.ratio = 0.9296` (official score 86.9). The diff is exactly:

1. **Prologue spill merge.** Original:
   ```
   15edea: strd r4, sl, [sp, #16]   ; elapsed@16, this@20 (one strd)
   ```
   Ours:
   ```
   15edda: str  r5, [sp, #12]       ; elapsed@12
   15eddc: str.w sl, [sp, #20]      ; this@20
   ```
   Same program point, but the two slots are not adjacent, so no `strd`
   (+1 instruction on our side).
2. **`elapsed` register.** Original `mov r4, r1` at entry; ours `mov r5, r1`.
   This is *the* remaining register permutation; every other register
   (`sl = this`, `fp = i*12`, `r8 = i`, `r9 = i*60`) already matches.
3. **`s`/`c` spill.** Original spills both trig results:
   `str r0,[sp,#8]` (`s`) and `str r0,[sp,#12]` (`c`). Ours keeps `s` in `r6`
   and spills only `c` to `[sp,#8]`.
4. **`trailMatrices` local.** Ours materialises the local in `[sp,#16]`
   (`str r0,[sp,#16]` at `15f17a`) and reloads it in the loop; the original
   has no slot and reloads `ldr.w r0,[sl,#216]` three times
   (`15f32c`, `15f370`, `15f39a`).
5. Minor: the `padx`/`unused` anchor stores (`str r4,[sp,#28]`,
   `str r5,[sp,#28]`) and the resulting `nop` padding; the `i*60` cursor is
   `r9` in ours vs `r4` in the original.

The block layout already matches: `itemEmit` is inline at `0x15f024`
(`cmp r2,#232` at `0x15f032`) on both sides; the cluster block is at
`0x15f072` on both. So this is not a block-placement problem.

---

## 4. Stack-slot weights (re-measured with the self-built clang 7.0.2)

`/tmp/llvm_android_702b/build2/bin/clang++` (same flags) reproduces the NDK
clang 7.0.2 object **byte for byte** for this function (verified by diffing
the `.o` disassembly), so its `-mllvm -debug-only=stack-slot-coloring` trace
is authoritative for the shipped build:

```
Spill slot intervals:
SS#32 weight:1.131250e+01   ; elapsed
SS#33 weight:2.000000e+00   ; __stack_chk_guard GOT ptr
SS#34 weight:4.774219e+01   ; dt
SS#35 weight:3.248438e+01   ; trailMatrices local
SS#36 weight:3.674219e+01   ; this
SS#37 weight:7.265625e+00   ; c
Color spill slot intervals:
Assigning fi#34 to fi#32   ; dt   -> colour 32 -> [sp,#24]
Assigning fi#36 to fi#33   ; this -> colour 33 -> [sp,#20]
Assigning fi#35 to fi#34   ; trail-> colour 34 -> [sp,#16]
Assigning fi#32 to fi#35   ; elapsed->colour 35 -> [sp,#12]
Assigning fi#37 to fi#36   ; c    -> colour 36 -> [sp,#8]
Assigning fi#33 to fi#37   ; guard-> colour 37 -> [sp,#4]
```

Final map (ours): `dt@24, this@20, trail@16, elapsed@12, c@8, guard@4`.
Original: `dt@24, this@20, elapsed@16, c@12, s@8, guard@4`.

The offsets are **weight rank**, not creation order. To get `elapsed@16`
next to `this@20` (and therefore the `strd`), `elapsed` (11.31) must outrank
the `trailMatrices` local (32.48). That is only possible if the local's
weight drops below 11.31 or disappears entirely.

---

## 5. What was tried this pass (~130 new shapes)

All measured with a single-symbol oracle (`/dev/shm/w5315/score.py`, imports
`tools/verify/verify.py`; `try.sh <variant.cpp>` = copy + `ninja link_so` +
score, ~0.6 s/variant). Baseline 86.9 / 636 insns.

### 5.1 Removing / weakening the `trailMatrices` local

| variant | pct | insns |
|---|---|---|
| baseline | **86.9** | 636 |
| remove local (member everywhere), keep anchors | 57.6 | 626 (1880 B) |
| remove local + `unused` | 57.5 | 624 (1876 B) |
| remove local + `unused` + `padx` | 57.4 | 622 (1872 B) |
| `const bool hasTrail = this->trailMatrices != nullptr` (member accesses) | 66.4 | 633 (1892 B) |
| `Array<Matrix> *volatile trailMatrices` | 57.0 | 628 (1880 B) |
| `Array<Matrix> *const trailMatrices` | 86.9 | 636 |
| all 8 subsets of the 3 in-loop local uses (A=`==nullptr`, B=`!=nullptr`, C=`(*tm)[i]`) | best 86.9 (none), next 68.8 | 626-635 |

Removing the local changes `this`'s register from `sl` to `fp` and the whole
allocation collapses; the local is load-bearing for `this = sl`.

### 5.2 The `elapsed` weight

The weights are canonicalised: `dt` inside/outside the loop, `(float)elapsed`
in place of `dt`, `dt` declared after the local, `s`/`c` at loop/function
scope, `s`/`c` in one declaration, `float s, c; s=...; c=...` — **all** produce
the identical weight set `{11.31, 2.0, 47.74, 32.48, 36.74, 7.27}` and 86.9.
Moving the fade-timer `elapsed` use one block earlier
(`if (trailTimers) { timer=...; left = timer - elapsed; if (ahead.z==50000 && timer>0) ... }`)
also leaves the weights unchanged (86.2, compiler sinks it back).

### 5.3 Block layout / source structure

The `itemEmit` block is already inline on both sides (section 3). An inverted
outer condition (`if (trailMatrices != nullptr) {...} else {kind...}`) with the
`itemEmit` code nested in the else reproduces the original's block layout
exactly (`cmp r2,#232` at `0x15f032`, cluster at `0x15f072`) and scores the
same **86.9 / 636**; with the `goto itemEmit` form also 86.9. No layout
variant beats 86.9, and removing `padx`/`unused`/`expect` drops it.

### 5.4 Register-pressure probes in the cluster block

| variant | pct |
|---|---|
| `const Vector *pos = &positions.data()[i]` live across `Sinf` | 86.9 |
| `const Vector *vel = &velocities.data()[i]` live across `Sinf` | 84.9 |
| `vel` + `upv` pointers live across `Sinf` | 75.4 |
| `Matrix frame`/`side`/`up`/`heading` reordered | <= 86.9 |

None frees `r6` (held by `s`) or moves `s` to a spill slot.

---

## 6. Conclusion and next hypothesis

The residual is the same fixed point the previous 15 passes reached, now with
the exact weight trace and a byte-identical self-built-clang reproduction:

```
ours:     dt 47.742 > this 36.742 > trail 32.484 > elapsed 11.312 > c 7.266 > guard 2.0
original: dt 47.742 > this 43.523 > elapsed 11.312 > c 7.266 = s 7.266 > guard 2.0
```

To match, one source shape must simultaneously:

1. **drop the `trailMatrices` slot** (the original has no local — three
   `ldr.w r0,[sl,#216]` in the trail block);
2. **keep `this` below `dt`** — without the local, `this`'s weight rises to
   **72.27** (two hot trail-block reloads, because the `MatrixSetTranslation`
   sret pointer takes `sl`), flipping `this@24`/`dt@20`;
3. **spill `s`** so the sixth colour exists and `c` moves to `[sp,#12]`.

The remaining lever is the register allocator's choice of the
`MatrixSetTranslation` sret pointer (`sl` in ours, `r6` in the original) and
of the ahead-computation temp (`r6` in ours, `sl` in the original). Those two
choices are what put the loop-body `this` reload in the original and the two
trail-block `this` reloads in ours. No expression shape, declaration order,
lifetime extension, pointer hoist, or block-structure variant tried
(this pass plus passes 2-15, ~730 variants total) moves them.

**Next hypothesis:** force `s` to spill by creating a genuine *core-register*
value live across `Sinf`/`Cosf` that the allocator cannot rematerialise —
e.g. an address-taken `Matrix *` or a `volatile`-qualified pointer used by the
trail block — so `r6` is occupied and the sret pointer must take `sl`; then
remove the `trailMatrices` local and see whether `this`'s weight stays below
`dt`. The `volatile` pointer variants tried so far (`volatile` local) changed
the whole allocation, so the probe must be a *separate* live range, not a
qualifier on the existing local.

---

## 7. Reproduction

```
# gate
cmake --build cmake-build-match --target check -j6

# side-by-side
python3 tools/verify/verify.py \
  --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN9RocketGun6updateEi

# REA decompile
GHIDRA_INSTALL_DIR=/tmp/ghidra_12.1.2_PUBLIC rea decompile \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --procedure _ZN9RocketGun6updateEi --format json

# slot weights (self-built clang 7, /tmp/llvm_android_702b/build2/bin/clang++)
/dev/shm/w5315/cc7.sh src/game/weapons/RocketGun.cpp -mllvm -debug-only=stack-slot-coloring
```

Scratch lives in `/dev/shm/w5315`: `base.cpp`, `cc7.sh`, `try.sh`,
`score.py`, `rea-update.json`, `orig.asm`, `base.asm`, `sbs.txt`, and the
~130 `v_*.cpp`/`w*.cpp`/`x*.cpp`/`y*.cpp`/`z*.cpp`/`p*.cpp`/`q*.cpp`
variants; none of it is in the repo.
