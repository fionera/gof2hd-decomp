# ModStation::ModStation ctor — wave 119 (w6909): literal-order proof + dead-end sweep

Status: **dead-end sweep, no landed source change.** Worktree `wt/w6909`,
branch `wave119-w6909`, base `50d2df436`. Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`). This note is self-contained and
extends `modstation-ctor-wave119-w6723.md` / `-w6717.md` with one new
structural result: for the faithful `far = pitch` shape the ARM literal-pool
order is the **reverse of the module-global reference order**, so `far =
pitch` can *never* reproduce both the original `.rodata` layout and the
original literal pool at source level. The only shape that does is the
baseline's `far = yaw`, whose 11-byte residual is a post-RA scheduling
artifact.

Target: `_ZN10ModStationC1Ev` / `_ZN10ModStationC2Ev`, 1008 B,
`src/game/menu/ModStation.cpp` (ctor at line 273). Baseline **11 diff bytes /
4 runs, 99.2 %** for both C1 and C2. Gate after the sweep: **RATCHET PASS**,
byte_exact 4505 / linked_exact 4505, avg 99.94, size-exact 4524/4524, data
843/843, imports 207, lint CLEAN, drift 0, stubs 0, missing 0, extra 0.

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

Baseline diff (`fdiff.py`, function-relative, function base `0xd52d0`):

```text
+0x186 (1B)  orig=50 ours=cd
+0x188..+0x18f (8B)  orig=28300220cdf8b880 ours=b88050f828400220
+0x19c (1B)  orig=40 ours=23
+0x19e (1B)  orig=14 ours=40
```

Original Thumb (`d52d0` base):

```text
d5456: f850 3028  ldr.w r3,[r0,r8,lsl#2]   ; near yaw -> r3 (dead after call)
d545a: 2002       movs  r0,#2
d545c: f8cd 80b8  str.w r8,[sp,#184]       ; spill race
...
d546c: 4640       mov   r0,r8
d546e: 4614       mov   r4,r2             ; preserve pitch across the call
d5470: f78d edf6  blx   MatrixSetRotation
```

Ours (baseline):

```text
d5456: f8cd 80b8  str.w r8,[sp,#184]
d545a: f850 4028  ldr.w r4,[r0,r8,lsl#2]   ; near yaw -> r4 (live across call)
d545e: 2002       movs  r0,#2
...
d546c: 4623       mov   r3,r4             ; extra copy
d546e: 4640       mov   r0,r8
d5470: f78d edf6  blx   MatrixSetRotation
```

The original's far call is `(pitch, freshYaw)`; the baseline's `far = yaw`
is a semantic bug that happens to preserve the original DAG order, literal
pool and every byte up to `moveq r0,r4`.

## 2. New result: literal-pool order is the reverse of the module-global order

The ARM constant pool entries are `tLDRLIT_ga_pcrel` pseudos emitted in the
SelectionDAG ISel DFS order (see §2 of `-w6723.md`). For the faithful
`far = pitch` shape (yaw dead after the near call) that order is always the
**reverse** of the order in which the internal `gHangarCam*` globals are
first referenced in the IR (which is also the `.rodata` emission order):

| first-reference order (`.rodata`) | far = pitch literal pool |
|---|---|
| `Pitch, YawIPad, Yaw` (original `.rodata`) | `Pitch, Yaw, YawIPad` |
| `Pitch, Yaw, YawIPad` | `Pitch, YawIPad, Yaw` |

The original binary has **both** `Pitch, YawIPad, Yaw`:

* `.rodata`: `gHangarCamPitch` @`0x1fe030`, `gHangarCamYawIPad` @`0x1fe058`
  (index 8 = 2.35f), `gHangarCamYaw` @`0x1fe080` (index 8 = 2.5f);
* literal pool @`0xd56b0`: `{0x128bf0, 0x128c16, 0x128c38}` →
  `Pitch, YawIPad, Yaw` (add-pc targets `0x1fe030/0x1fe058/0x1fe080`).

So `far = pitch` is structurally unable to be byte-exact: keeping the
original `.rodata` forces the pool to `Pitch, Yaw, YawIPad`, and swapping the
ternary spelling to `!iPad ? Yaw : YawIPad` fixes the pool but swaps the two
40-byte yaw tables in `.rodata` (a `data_exact` regression). This is now
confirmed with six independent spellings that all leave `.rodata` correct
and all give the same wrong pool (`f08b1200 3a8c1200 0c8c1200` = `Pitch,
Yaw, YawIPad`): plain ternary, `if/else`, second local pointer, reassignment,
`yaw = fresh` reload, `int i3 = race*3`, and dead `(void)` references.

**New landable trick** (not an improvement, but removes the `.rodata`
coupling): inserting `(void) gHangarCamYawIPad;` between the `pitchTable` and
`yawTable` lines pins the first-reference order to `Pitch, YawIPad, Yaw`
regardless of the ternary spelling, so the swapped ternary
(`!iPad ? Yaw : YawIPad`) becomes landable — but it still measures 43 B / 8
runs, i.e. no gain over plain `far = pitch`.

The baseline (`far = yaw`) escapes the reversal because yaw is live across
the near call: the DAG root set changes and ISel emits `YawIPad` before
`Yaw`, matching the original pool. The price is the register shape in §1.

## 3. Variants measured this session

All 1008 B unless a size is given. `fresh` =
`(Globals::iPad ? gHangarCamYawIPad : gHangarCamYaw)[race]`. "pool" is the
resolved literal-pool target order; "ro" is the `.rodata` index-8 check
(2.35 / 2.5). Baseline = 11 B / 4 runs / 99.2 %.

| variant | diff B | runs | pool | ro |
|---|---:|---:|---|---|
| **baseline (`far = yaw`)** | **11** | **4** | **Pitch, YawIPad, Yaw** | **OK** |
| `far = pitch` (`farp`) | 43 | 8 | Pitch, Yaw, YawIPad | OK |
| farp, far y = `yawTable[race]` (local ptr) | 580 | 72 (984 B) | — | OK |
| farp, far y = volatile-int bitcast of `yawTable[race]` | 580 | 72 (984 B) | — | OK |
| farp, far y = `*(volatile float*)&yawTable[race]` | 698 | 42 (992 B) | — | OK |
| farp, far y = volatile-int bitcast of `fresh` | 43 | 8 | Pitch, Yaw, YawIPad | OK |
| farp, near y inline ternary (no `yaw` local) | 689 | 92 (1000 B) | — | OK |
| farp, both loads plain float | 667 | 86 | Pitch, YawIPad, Yaw | OK |
| farp, plain pitch + volatile-int yaw | 666 | 87 | — | OK |
| farp, volatile-int pitch + plain-float yaw | 46 | 7 | Pitch, Yaw, YawIPad | OK |
| farp, `yawBits` loaded before `pitchBits` | 744 | 40 (1000 B) | — | OK |
| farp, `float pitch` declared before `float yaw` | 43 | 8 | Pitch, Yaw, YawIPad | OK |
| farp, far x = `*(float*)&pitchBits` / `(float)pitch` / `pitch*1.0f` / `-(-pitch)` | 43 | 8 | Pitch, Yaw, YawIPad | OK |
| farp, far x = `pitch + 0.0f` | 636 | 44 (1020 B) | — | OK |
| farp, `if/else` instead of ternary | 43 | 8 | Pitch, Yaw, YawIPad | OK |
| farp, `yawTable = Yaw; if (iPad) yawTable = YawIPad;` | 43 | 8 | Pitch, Yaw, YawIPad | swapped |
| farp, `yawTable` declared before `pitchTable` | 46 | 9 | — | swapped |
| farp, swapped ternary alone | 43 | 8 | Pitch, YawIPad, Yaw | swapped |
| farp, swapped ternary + `(void)gHangarCamYawIPad;` mid | 43 | 8 | Pitch, Yaw, YawIPad | **OK** |
| farp, `(void)gHangarCamYawIPad;` before `pitchTable` | 46 | 9 | — | shifted |
| farp, `(void)gHangarCamYaw;` mid (between table lines) | 43 | 8 | Pitch, YawIPad, Yaw | swapped |
| farp, far y = `yaw` (local) | 580 | 72 (984 B) | — | OK |
| farp, far x = `yaw`, far y = `pitch` | 580 | 72 (984 B) | — | OK |
| farp, far = (`yaw`,`yaw`) | 613 | 33 (968 B) | — | bad |
| farp, far = (`pitch`,`pitch`) | 738 | 34 (968 B) | — | bad |
| farp, far y via a second local pointer `yawTable2` | 43 | 8 | Pitch, Yaw, YawIPad | OK |
| farp, `yawTable` reassigned before far | 43 | 8 | Pitch, Yaw, YawIPad | OK |
| farp, `yaw = fresh;` reload before far | 43 | 8 | Pitch, Yaw, YawIPad | OK |
| farp, `yaw = yawTable[race];` reload before far | 580 | 72 (984 B) | — | OK |
| farp, `float farPitch = pitch;` before near | 43 | 8 | Pitch, Yaw, YawIPad | OK |
| farp, `Matrix farKey;` before near | 687 | 45 (1000 B) | — | OK |
| farp, far translation before near call | 631 | 90 (992 B) | — | OK |
| farp, far translation before the loads | 701 | 77 (984 B) | — | OK |
| farp, `int i3 = race*3` before/after near | 43 | 8 | Pitch, Yaw, YawIPad | OK |
| farp, `(void) Globals::iPad / race / Globals::status / gHangarCamPitch;` | 43 | 8 | Pitch, Yaw, YawIPad | OK |
| farp, `yaw` declared+loaded after the near call | 62 | 8 | — | OK |
| farp, `const float &yaw = yawTable[race]` | 676 | 69 (976 B) | — | bad |
| farp, `const volatile float &yaw` | 636 | 95 (992 B) | — | OK |
| baseline, far index `farRace` / `(unsigned)race` / `race+0` | 11 | 4 | Pitch, YawIPad, Yaw | OK |
| baseline, near y = `yawTable[race]` / `fresh` | 11 | 4 | Pitch, YawIPad, Yaw | OK |
| baseline, `(void)gHangarCamYawIPad;` at 7 inter-line positions | 11 | 4 | Pitch, YawIPad, Yaw | OK |
| baseline, `(void)gHangarCamYawIPad;` before everything | 15 | 6 | — | shifted |
| baseline, `(void)gHangarCamYaw;` at 7 inter-line positions | 11 | 4 | Pitch, YawIPad, Yaw | OK |
| baseline, `(void)gHangarCamYaw;` between table lines | 43 | 8 | Pitch, YawIPad, Yaw | swapped |
| baseline, `pitch = pitch;` / `(void) pitch;` | 11 | 4 | Pitch, YawIPad, Yaw | OK |
| baseline, `if (pitch != pitch) { yaw = pitch; }` | 686 | 37 (1032 B) | — | bad |
| baseline, 6 valid load-order permutations | 11×3 / 694×3 | 4 / 92 | — | OK |
| baseline, `int i3 = race*3` before/after near | 11 | 4 | Pitch, YawIPad, Yaw | OK |
| baseline, `(void) Globals::iPad / race / Globals::status / gHangarCamPitch;` | 11 | 4 | Pitch, YawIPad, Yaw | OK |

Prior-wave variants (fences, `__builtin_assume`, `Vector` overload, 4-arg
overload, `memcpy`/`union` bitcasts, `posTable`/`farPos` locals, named x/y/z
locals, NRVO, `int` tables, `volatile` references, `race` type changes,
`Matrix` reorder, etc.) are tabulated in `-w6717.md` §3, `-w6723.md` §4 and
`modstation-ctor-wave112-w6018.md`; none beats 11 B / 4 runs either.

## 4. Conclusion

The ctor is at a hard local optimum for this compiler. Two constraints are
mutually exclusive at source level:

* `far = pitch` is the faithful shape and gives the original's register
  shape (yaw → `r3` dead, pitch → `r4` live, `mov r4,r2`), but the literal
  pool is forced to `Pitch, Yaw, YawIPad` (43 B / 8 runs, 98.4 %).
* `far = yaw` keeps yaw live across the call, which flips the DAG/ISel order
  back to the original `Pitch, YawIPad, Yaw` pool and matches every byte up
  to `moveq r0,r4`, at the cost of the near yaw load landing in `r4` plus an
  extra `mov r3,r4` and a shifted `str.w r8` (11 B / 4 runs, 99.2 %).

The remaining 11 bytes are the post-RA placement of `str.w r8` and the yaw
load, which is decided by the InlineSpiller / post-RA list scheduler after
register allocation and is not reachable from source without changing build
flags (`-mllvm -misched-postra=false` reproduces the original register shape
in a scratch object but leaves the literal order and the `str r8`/`mov
r4,r2` schedule permuted).

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w6909
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6   # RATCHET PASS, byte 4505 linked 4505
python3 tools/verify/verify.py --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN10ModStationC1Ev
```

Literal-pool / `.rodata` check:

```python
import struct
b = open('cmake-build-match/libgof2hdaa.so','rb').read()
b[0xd56b0:0xd56c0].hex()                      # f08b1200168c1200388c120016ac1300 (original)
struct.unpack_from('<10f', b, 0x1fe058)[8]    # 2.35  (YawIPad)
struct.unpack_from('<10f', b, 0x1fe080)[8]    # 2.5   (Yaw)
```
