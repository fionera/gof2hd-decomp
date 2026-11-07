# ModStation::ModStation ctor — wave 124 (w7616): EH call-site evidence, no source win

Status: **no landed source change.** Worktree `wt/w7616`, base `bf365a964`
(`MGame::OnTouchEnd pass`). Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`). This note is self-contained and
extends `modstation-ctor-wave119-w6723.md`, `modstation-ctor-wave121-w7120.md`,
`modstation-ctor-wave122-w7220.md`, `modstation-ctor-wave123-w7317.md`.

Target: `_ZN10ModStationC1Ev` / `_ZN10ModStationC2Ev`, 1008 B,
`src/game/menu/ModStation.cpp` (ctor at line ~276). Baseline **11 diff bytes /
4 runs, 99.2 %** for both C1 and C2. Gate after the sweep: **RATCHET PASS**,
byte_exact 4506, linked_exact 4506, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, stubs 0, missing 0, extra 0.

## 1. Baseline source and residual

```cpp
    const float *pitchTable = gHangarCamPitch;
    const float *yawTable = Globals::iPad ? gHangarCamYawIPad : gHangarCamYaw;
    int pitchBits = *(const volatile int *) &pitchTable[race];
    int yawBits = *(const volatile int *) &yawTable[race];
    float yaw = *(const float *) &yawBits;
    float pitch = *(const float *) &pitchBits;
    MatrixSetRotation(nearKey, pitch, yaw, 0.0f, (RotationOrder) 2);
    ...
    MatrixSetRotation(farKey, yaw, (Globals::iPad ? gHangarCamYawIPad : gHangarCamYaw)[race], 0.0f,
                      (RotationOrder) 2);
```

The far call in the original is `(pitch, freshYaw)` (the far block does
`mov r2, r4` with `r4 = pitch`), so the baseline's `far = yaw` is a real
semantic bug that happens to score better in bytes. Residual (function-relative,
base `0xd52d0`) is exactly two reorderings:

```text
+0x186..+0x187 (1B)  orig=50 ours=cd
+0x188..+0x190 (8B)  orig=28300220cdf8b880 ours=b88050f828400220
+0x19c..+0x19d (1B)  orig=40 ours=23
+0x19e..+0x19f (1B)  orig=14 ours=40
```

Original near tail (`d5456`):

```text
d5456  f850 3028  ldr.w r3,[r0,r8,lsl#2]   ; yaw -> r3 (dead after the call)
d545a  2002       movs  r0,#2
d545c  f8cd 80b8  str.w r8,[sp,#184]
d5460  2100       movs  r1,#0
d5462  f50d 7880  add.w r8,sp,#256
d5466  e9cd 1000  strd  r1,r0,[sp]
d546a  a950       add   r1,sp,#320
d546c  4640       mov   r0,r8
d546e  4614       mov   r4,r2             ; pitch preserved in r4
d5470  f78d edf6  blx   MatrixSetRotation
```

Ours (baseline, `far = yaw`):

```text
d5456  f8cd 80b8  str.w r8,[sp,#184]
d545a  f850 4028  ldr.w r4,[r0,r8,lsl#2]   ; yaw -> r4 (live across the call)
d545e  2002       movs  r0,#2
d5460  2100       movs  r1,#0
d5462  f50d 7880  add.w r8,sp,#256
d5466  e9cd 1000  strd  r1,r0,[sp]
d546a  a950       add   r1,sp,#320
d546c  4623       mov   r3,r4             ; extra copy
d546e  4640       mov   r0,r8
d5470  f78d edf6  blx   MatrixSetRotation
```

## 2. New evidence: EH call-site entry #3

Parsing the ctor's LSDA (`.ARM.exidx`/`.ARM.extab`, LSDA at `0x1ada7c`) gives
15 call-site entries. Entries 0,1,2,4..14 are **byte-identical** between the
original and our build; only entry #3 (the near `MatrixSetRotation` invoke)
differs:

| build | entry #3 `(start, length)` | function-relative range |
|---|---|---|
| original | `(0x186, 0x1e)` | `d5456 .. d5474` |
| ours, `far = yaw` (baseline) | `(0x18e, 0x16)` | `d545e .. d5474` |
| ours, `far = pitch` | `(0x190, 0x14)` | `d5460 .. d5474` |

The original's call-site range starts **at the near yaw load** (`d5456`); ours
starts just after it. So the original's `EH_LABEL` is scheduled *before* the
yaw load, whereas in both of our shapes it is scheduled after. This is a
consequence of the same BURR/scheduler height ordering that produces the
`str.w r8` / `ldr.w r3` permutation (wave-122 §2): the yaw load's height (13)
exceeds the `eh_label`'s (8), so the load is emitted before the label. It is
not an independent lever reachable from source, but it is a useful independent
confirmation that the original's near block is the faithful `far = pitch`
shape with a different post-ISel schedule.

## 3. Variants measured this wave

All 1008 B. `far` is `pitch` unless stated. `fresh` =
`(Globals::iPad ? gHangarCamYawIPad : gHangarCamYaw)[race]`.

| variant | diff B | runs |
|---|---:|---:|
| **baseline (`far = yaw`, volatile-int bit loads)** | **11** | **4** |
| `far = pitch` | 43 | 8 |
| near y = `*(float*)&*(volatile int*)&yawTable[race]` (yaw load in the call expr) | 44 | 7 |
| both near args inline volatile-int bit loads | 43 | 8 |
| near y inline, `far = yaw` | 11 | 4 |
| `far = yaw`, near y = `yawTable[race]` plain | 11 | 4 |
| `far = yaw`, near y = `fresh` plain | 11 | 4 |
| `far = pitch`, near y = `yawTable[race]` plain | 43 | 8 |
| `far = pitch`, near y = `fresh` plain | 43 | 8 |
| `pitch`/`yaw` kept as `int` bits, bitcast at each call | 43 | 8 |
| `int pitchKeep = pitchBits` used as far x | 43 | 8 |
| `float pitchArg = pitch` / `yawArg` / both / `const float&` | 43 | 8 |
| near x = fresh volatile pitch load | 43 | 8 |
| near x and y = fresh volatile loads | 43 | 8 |
| `Globals::iPad != 0` in the ternary | 43 | 8 |
| `Globals::iPad == 0 ? Yaw : YawIPad` (swapped) | 43 | 8 |
| `Globals::iPad == 1` in the ternary | 688 | 93 |
| `const volatile int *pitchP/yawP` pointer variables | 43 | 8 |
| yaw load before pitch load | 744 | 40 |
| `farYaw` local computed before the near call | 580 | 72 |
| far x = `*(float*)&pitchBits` / `pitchCopy` | 43 | 8 |
| explicit `if/else` for `yawTable`, `far = pitch` | 43 | 8 |
| explicit `if/else` for `yawTable`, `far = yaw` | 11 | 4 |
| explicit `if/else` loading yaw values (two loads + phi) | 689 | 92 |
| table decls before `camKey`/`MatrixSetTranslation` | 694 | 85 |
| `camKey` assignments after the near call | 878 | 20 |
| near call before `MatrixSetTranslation` | 660 | 89 |
| `Matrix farKey;` declared before the camKey block | 664 | 89 |

Prior-wave variants (fences, `__builtin_assume`, `Vector` overload, 4-arg
overload, `memcpy`/`union` bitcasts, `posTable`/`camPos` locals, named x/y/z
locals, NRVO, `int` tables, `volatile` references, `race` type changes,
`Matrix` reorder, `-misched-postra=false`, etc.) are tabulated in
`modstation-ctor-wave119-w6717.md` §3, `modstation-ctor-wave123-w7317.md` §3.4
and `modstation-ctor-wave112-w6018.md`; none beats 11 B / 4 runs.

## 4. Conclusion

The ctor remains at the same hard local optimum waves 121-123 found. The
target needs three things simultaneously:

1. literal pool / `.rodata` order `{Pitch, YawIPad, Yaw}` (baseline has it;
   `far = pitch` flips the pool to `{Pitch, Yaw, YawIPad}` because the yaw
   live-out changes the DAG root set / BURR heights);
2. near yaw -> `r3` (dead) and pitch -> `r4` (live, `mov r4,r2`);
3. near schedule `ldr.w r3 ; movs r0,#2 ; str.w r8 ; ... ; mov r0,r8 ;
   mov r4,r2 ; blx`.

(1) and (2) are mutually exclusive at source level with clang 7.0.2 `-Oz`: the
live-out that fixes the register roles (`pitch`) also flips the pool order and
the `str.w r8`/`mov r4,r2` schedule. The EH call-site entry independently
confirms that the original near block is the faithful `far = pitch` shape, but
the remaining difference is post-ISel scheduling, not source structure. No
source spelling of the ternary, the loads, the locals, the call argument
expressions, the table-pointer declarations or the surrounding camKey /
translation code moved the score below 11 B / 4 runs.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w7616
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8   # RATCHET PASS, byte 4506 linked 4506
python3 tools/verify/verify.py \
  --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN10ModStationC1Ev
```

LSDA call-site entries (Python, `tools/verify/elf.py`):

```python
from elf import Elf32
def uleb(b,i):
    v=s=0
    while True:
        x=b[i]; i+=1; v|=(x&0x7f)<<s
        if not (x&0x80): return v,i
        s+=7
def entries(path, addr=0x1ada7c, size=0x120):
    b=Elf32(path).bytes_at(addr,size); i=3
    ln,i=uleb(b,i); end=i+ln; out=[]
    while i<end:
        st,i=uleb(b,i); le,i=uleb(b,i); lp,i=uleb(b,i); ac,i=uleb(b,i)
        out.append((st,le,lp,ac))
    return out
entries('cmake-build-match/libgof2hdaa.so')[3]   # (0x18e, 0x16, ...) baseline
```
