# ModStation::ModStation ctor — wave 119 (w6931): literal-pool order is invariant for `far = pitch`

Status: **dead-end sweep, no landed source change.** Worktree `wt/w6931`, branch
`wave119-w6931`, base `9c284a233`. Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a,
NDK r18b clang 7.0.2, `-Oz`). This note is self-contained and extends
`modstation-ctor-wave119-w6909.md` / `-w6723.md` / `-w6717.md` /
`modstation-ctor-wave112-w6018.md`.

Target: `_ZN10ModStationC1Ev` / `_ZN10ModStationC2Ev`, 1008 B,
`src/game/menu/ModStation.cpp` (ctor at line 273). Baseline **11 diff bytes / 3 runs,
99.2 %** for both C1 and C2. Gate after the sweep: **RATCHET PASS**, byte_exact 4505 /
linked_exact 4505, avg 99.94, size-exact 4524/4524, data 843/843, imports 207, lint
CLEAN, drift 0, stubs 0, missing 0, extra 0.

## 1. Baseline source and residual

```cpp
    const float *pitchTable = gHangarCamPitch;
    const float *yawTable = Globals::iPad ? gHangarCamYawIPad : gHangarCamYaw;
    int pitchBits = *(const volatile int *) &pitchTable[race];
    int yawBits = *(const volatile int *) &yawTable[race];
    float yaw = *(const float *) &yawBits;
    float pitch = *(const float *) &pitchBits;
    MatrixSetRotation(nearKey, pitch, yaw, 0.0f,
                      (AbyssEngine::AEMath::RotationOrder) 2);
    ...
    MatrixSetRotation(farKey, yaw, (Globals::iPad ? gHangarCamYawIPad : gHangarCamYaw)[race], 0.0f,
                      (AbyssEngine::AEMath::RotationOrder) 2);
```

`fdiff.py _ZN10ModStationC1Ev` (function-relative, base `0xd52d0`):

```text
+0x187..+0x18f (9B)  orig=5030282002f8cd80b8 ours=cd80b8f85040282002
+0x19d (1B)         orig=40 ours=23
+0x19f (1B)         orig=14 ours=40
```

i.e. the whole 1008-byte function is instruction-identical except the near-call tail:

```text
orig:  ldr.w r3,[r0,r8,lsl#2]   ; yaw -> r3, dead after the call
       movs  r0,#2
       str.w r8,[sp,#184]
       ...
       mov   r0,r8
       mov   r4,r2              ; pitch preserved in r4 for the far call
ours:  str.w r8,[sp,#184]
       ldr.w r4,[r0,r8,lsl#2]   ; yaw -> r4 (live across the call)
       movs  r0,#2
       ...
       mov   r3,r4              ; extra copy
       mov   r0,r8
```

The baseline's `far = yaw` is semantically wrong (the far rotation's x must be `pitch`,
see `rea.json` / the far call at `0xd54f0`: `mov r2,r4`), but it is the only shape that
reproduces the original literal pool.

## 2. New result: for `far = pitch` the pool order is *invariant*, not "reversed module order"

`-w6909.md` §2 concluded that for `far = pitch` the ARM literal pool is the **reverse of
the module-global first-reference order**, so swapping the ternary spelling
(`!iPad ? Yaw : YawIPad`) "fixes the pool but swaps `.rodata`". That conclusion is based
on comparing the raw pool *words*; the words are pc-relative and move when `.rodata`
moves, so they are not comparable across `.rodata` layouts. Resolving each pool word
against the `add rX, pc` that consumes it (`/dev/shm/w6931/pool.py`) gives:

| build | `.rodata` order | pool order (by `ldr` order) |
|---|---|---|
| original | Pitch `0x1fe030`, YawIPad `0x1fe058`, Yaw `0x1fe080` | **Pitch, YawIPad, Yaw** |
| baseline (`far = yaw`, spelling A) | Pitch, YawIPad, Yaw | **Pitch, YawIPad, Yaw** |
| `far = pitch`, spelling A (`iPad ? YawIPad : Yaw`) | Pitch, YawIPad, Yaw | Pitch, **Yaw**, **YawIPad** |
| `far = pitch`, spelling B (`!iPad ? Yaw : YawIPad`) | Pitch, **Yaw**, **YawIPad** | Pitch, **Yaw**, **YawIPad** |
| `far = pitch`, spelling B + `(void)Pitch; (void)YawIPad;` at ctor top | Pitch, YawIPad, Yaw | Pitch, **Yaw**, **YawIPad** |

So for **every** `far = pitch` shape the pool is `{Pitch, Yaw, YawIPad}` (the yaw value
is dead after the near call, so its table-address LDRLIT is selected *before* the other
one). Spelling B did not fix the pool — it only swapped the two 40-byte tables in
`.rodata` so the resulting addresses happen to increase. `far = pitch` therefore cannot
be byte-exact with the original `.rodata` layout, confirming `-w6909.md` §4 by a
different route.

For `far = yaw` the pool *does* follow the `.rodata` order (checked with the swapped
ternary: `.rodata` Pitch, Yaw, YawIPad → pool Pitch, Yaw, YawIPad), which is why the
baseline keeps the correct pool.

## 3. What the original source actually was

The original near block loads yaw into `r3` and lets it die, and preserves **pitch** in
`r4`; the far block then re-does the yaw select from the two spilled table pointers
(`0xd54e0`..`0xd54f2`) and reloads yaw. So the original IR must have `yaw` live across
the call (2 uses → correct pool) while the register allocator *rematerialised* the yaw
load instead of keeping it in a callee-saved register. The faithful source is therefore

```cpp
    float pitch = gHangarCamPitch[race];
    float yaw   = (Globals::iPad ? gHangarCamYawIPad : gHangarCamYaw)[race];
    MatrixSetRotation(nearKey, pitch, yaw, 0.0f, (RotationOrder) 2);
    ...
    MatrixSetRotation(farKey, pitch, yaw, 0.0f, (RotationOrder) 2);   // same value
```

With our clang 7.0.2 the RA makes the opposite choice: it keeps `yaw` in `r4`, spills
`pitch` to `[sp,#184]` and emits `mov r4,r3` — and the far block becomes 24 bytes
shorter (the select is not re-done). Measured: **984 B, 84.2 %**. `far = pitch` (fresh
yaw in the far call) gives the original register shape but the wrong pool: **1008 B,
98.4 %**. The baseline (`far = yaw`) keeps the pool and scores **1008 B, 99.2 %**.

## 4. Variants measured this session

All 1008 B unless stated. `bytes`/`runs` = `fdiff.py` raw-byte diff vs the original;
`pct` = `tools/verify` fuzzy score. Baseline = 11 B / 3 runs / 99.2 %.

| variant | bytes | runs | pct | pool |
|---|---:|---:|---:|---|
| **baseline (`far = yaw`, volatile-int loads)** | **11** | **3** | **99.2** | **Pitch, YawIPad, Yaw** |
| `far = pitch` (faithful far x) | 43 | 8 | 98.4 | Pitch, Yaw, YawIPad |
| `far = pitch`, swapped ternary | 43 | 8 | — | Pitch, Yaw, YawIPad (`.rodata` swapped) |
| `far = pitch`, swapped ternary + `(void)Pitch;(void)YawIPad;` at ctor top | 48 | 11 | — | Pitch, Yaw, YawIPad |
| `far = pitch`, swapped ternary + `(void)YawIPad;` after the pitch line | 43 | 8 | — | Pitch, Yaw, YawIPad |
| `far = pitch`, swapped ternary + `const float *yawIPadTable = gHangarCamYawIPad;` | 43 | 8 | — | Pitch, Yaw, YawIPad |
| `far = (pitch, yaw)` — faithful, both live | 604 | 72 | 84.2 | Pitch, YawIPad, Yaw |
| `far = (pitch, yaw)`, yaw loaded first | 601 | 74 | — | Pitch, YawIPad, Yaw |
| `far = (pitch, yawTable[race])` | 604 | 72 | — | Pitch, YawIPad, Yaw |
| `far = (yaw, pitch)` | 604 | 72 | — | Pitch, YawIPad, Yaw |
| `far = (yaw, fresh)` = baseline | 11 | 3 | 99.2 | Pitch, YawIPad, Yaw |
| plain `float pitch/yaw` loads (no volatile-int bitcast) | 672 | 85 | — | — |
| plain loads + `far = pitch` | 667 | 86 | — | Pitch, YawIPad, Yaw |
| baseline with `pitch` declared before `yaw` | 11 | 3 | — | Pitch, YawIPad, Yaw |
| baseline with forward declarations + assignments | 11 | 3 | — | Pitch, YawIPad, Yaw |
| baseline with `const float yaw/pitch` | 11 | 3 | — | Pitch, YawIPad, Yaw |
| baseline with `unsigned` bitcasts | 11 | 3 | — | Pitch, YawIPad, Yaw |
| baseline with swapped array *definitions* | 11 | 3 | — | Pitch, YawIPad, Yaw |
| `far = yaw` + swapped ternary | 13 | 5 | — | Pitch, Yaw, YawIPad (`.rodata` swapped) |

Array definition order in the source has **no** effect on `.rodata`; only the first
*source reference* order does (clang's deferred static-global emission). `(void)arr;`
does emit a real reference (it shifts `.rodata` and costs a `ldr`+`add`), so it cannot be
used to pin the order for free.

## 5. Conclusion

The ctor is at a hard local optimum for this compiler. The original needs three things at
once:

1. `.rodata` = Pitch, YawIPad, Yaw (fixed by the source first-reference order);
2. literal pool = Pitch, YawIPad, Yaw — only produced when `yaw` is **live** across the
   near call (`far = yaw`), i.e. the semantically-wrong baseline;
3. near block: yaw → `r3` (dead), pitch → `r4` (`mov r4,r2`) — only produced when `yaw`
   is **dead** (`far = pitch`) or when the RA rematerialises the yaw load.

(2) and (3) are mutually exclusive at source level: our clang keeps the live `yaw` in
`r4` instead of rematerialising it. The remaining 11 bytes are the post-RA placement of
`str.w r8` plus the `ldr.w r4` / `mov r3,r4` shape produced by that RA choice.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6931
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6   # RATCHET PASS, byte 4505 linked 4505
python3 tools/verify/verify.py --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN10ModStationC1Ev
```

Literal-pool order must be resolved through the `add rX, pc` that consumes each word
(raw pool words are pc-relative and move with `.rodata`); `pool.py` in this session does
that. Original resolved targets: `r0 -> 0x1fe030 (Pitch)`, `r3 -> 0x1fe058 (YawIPad)`,
`r4 -> 0x1fe080 (Yaw)`.
