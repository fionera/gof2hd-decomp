# ModStation constructor — wave 191b (w13550): 11-byte post-RA schedule plateau

Worker `w13550`, branch `wave191b-w13550`, base master `e79990318`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Scratch `/home/debian/GalaxyOnFire2/scratch/w13550`.

Target `_ZN10ModStationC1Ev` / `_ZN10ModStationC2Ev` (aliases at `0xd52d0`,
1008 B).  Status: **no source change landed; no raw win found.**  The tree is
clean and `check` is green at the entry counts.

## Result

| symbol | VMA | size | raw / aligned | insn |
|---|---|---:|---:|---:|
| `_ZN10ModStationC1Ev` | `0xd52d0` | 1008 | **11 / 5** | 374/374 |
| `_ZN10ModStationC2Ev` (alias) | `0xd52d0` | 1008 | **11 / 5** | 374/374 |

Gate on entry and exit (full `check`, `-j2`, `ulimit -v 12000000`):
`RATCHET PASS`, `byte_exact 4507/4524`, `linked_exact 4507/4524`,
size 4524/4524, `stub_zero_size 0`, `missing 0`, `wrong_type 0`, `extra 0`,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.
`.rel.plt` byte-identical; `.ARM.extab` unchanged at 576.

## The exact 11-byte difference

Function-relative, original vs ours (four runs, all in the nearKey
`MatrixSetRotation` call setup):

```
+0x186 (1B)  orig f850 3028  ldr.w r3,[r0,r8,lsl#2]  |  ours f8cd 80b8  str.w r8,[sp,#184]
+0x188 (8B)  orig 28300220cdf8b880                   |  ours b88050f828400220
             (ldr.w r3; movs r0,#2; str.w r8)        |  (str.w r8; ldr.w r4; movs r0)
+0x19c (1B)  orig 4640        mov r0,r8               |  ours 4623        mov r3,r4
+0x19e (1B)  orig 4614        mov r4,r2               |  ours 4640        mov r0,r8
```

The instruction *multiset* differs by one 2-byte `mov`: the original has
`mov r4,r2` (save `pitchBits` for the farKey call) at the end of the call
setup; ours has `mov r3,r4` (copy the live `yawBits` into the nearKey
argument register).  The rest is a post-RA schedule tie: the original issues
the yaw load directly into `r3`, then `movs r0,#2`, then spills `race`
(`str.w r8,[sp,#184]`); ours spills `race` first, loads yaw into `r4`, and
copies it to `r3` just before the call.

Root cause: the original keeps `pitchBits` (the first table value) live
across the nearKey call and uses it for the farKey rotation.  Ours keeps
`yawBits` (the second value) live, because that is the form that reproduces
the original `.text` literal-pool order (`Pitch, YawIPad, Yaw`).

## Why the semantically faithful form is worse

The faithful source shape

```cpp
float yaw   = *(const float *) &yawBits;
float pitch = *(const float *) &pitchBits;
MatrixSetRotation(nearKey, pitch, yaw, 0.0f, ...);
...
MatrixSetRotation(farKey, pitch, yawTable[race], 0.0f, ...);
```

keeps `pitchBits` live and gives the original's post-RA schedule (yaw load
into `r3`, `mov r4,r2` late), but flips the two yaw-table `LDRLIT` leaves:
the pool becomes `Pitch, Yaw, YawIPad` instead of `Pitch, YawIPad, Yaw`.
Measured raw **43 / aligned 12** (8 runs), including the two swapped pool
words at `+0x3e4`/`+0x3e8` and the table-pointer register swap.

The landed 11 form (`yaw = pitchBits`, `pitch = yawBits`, farKey uses
`pitch`) is a source-shape device: it keeps the pool order correct at the
cost of keeping the wrong value live.  This was already documented in
`modstation-ctor-wave112-w6018.md` and `modstation-ctor-wave183-w12719.md`.

## Sweep (all size-exact 1008 B unless noted)

Every source family below bottoms out at **11**; the only sub-11 result is
the semantically wrong `yaw`-live form already in HEAD.

| family | best raw | note |
|---|---:|---|
| current 11 form | **11** | pool correct, schedule tie |
| faithful (`farKey(pitch)`) | 43 | pool order flips |
| faithful + `__atomic_signal_fence` (any position/order) | 42 | one byte better than 43 |
| 2-fence IR shapes (pitchPtr between fences) | 44/18 or 39/11 | two non-merged fences reproduced, but no gain |
| bitcast spellings (`reinterpret_cast`, union, inline) | 11 | same IR |
| `const`/`volatile` locals, declaration orders | 11 | same IR |
| ternary spellings (`!=0`, `!=false`, `bool`, `&arr[0]`, if/else) | 11 | same IR |
| load orders (yaw first) | 702/1000 B | size-fatal |
| plain float loads | 667/59 | 375 insns, `ldrd`/`vldr` shape |
| volatile float / volatile pointer loads | 351/1000 B | size-fatal |
| `race` type (`unsigned`, `long`, `size_t`, `int32_t`, ...) | 11 | same IR |
| race if/else -> ternary / nested / switch | 11 / 11 / 769 | switch size-fatal |
| camKey / farKey translation rewrites | 11 | canonicalised to same code |
| `__builtin_expect` on `Globals::iPad` | 11 | same select |
| moving `Matrix farKey` declaration | 944/976 B | stack layout breaks |
| assigning the `MatrixSetRotation` return value | 629/992 B | sret copy |

The two-fence IR shape is reproducible from C++ by putting a
`const float *pitchPtr = &pitchTable[race];` between the two
`__atomic_signal_fence` calls (the GEP prevents the optimiser from merging
the fences).  It does not reach the hand-crafted `best16d` schedule; the
best measured two-fence variants are 44/18.

## Conclusion

The 11-byte residual is a post-RA `ScheduleDAGRRList` tie, not a wrong
constant, callee, or initializer.  The original achieves both the correct
literal-pool order and the `pitchBits`-live schedule; no C++ source shape
tried here reproduces both.  The current 11 form is the measured floor for
this compiler and TU.  No source change was landed; the tree is clean.

## Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
    _ZN10ModStationC1Ev _ZN10ModStationC2Ev
# raw=11 aligned=5 size=1008/1008 insn=374/374
cmake --build cmake-build-match --target check -j2
```
