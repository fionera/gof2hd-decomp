# ModStation constructor — wave 191b (w13575): LDRLIT-order / schedule sweep

Worker `w13575`, branch `wave191b-w13575`, base master `58eac6932`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Scratch `/home/debian/GalaxyOnFire2/scratch/w13575`.
Predecessor note: `modstation-ctor-wave191b-w13550.md` (read that first; this
note only adds the new IR/`.s` evidence and the new source shapes measured).

Target `_ZN10ModStationC1Ev` / `_ZN10ModStationC2Ev` (aliases at `0xd52d0`,
1008 B).  Status: **no source change landed; no raw win found.**  `check` is
green at the entry counts (`RATCHET PASS`, byte_exact 4507/4524,
linked_exact 4507/4524, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370, `.rel.plt` byte-identical, `.ARM.extab` 577).

## Result

| symbol | VMA | size | raw / aligned | insn |
|---|---|---:|---:|---:|
| `_ZN10ModStationC1Ev` | `0xd52d0` | 1008 | **11 / 5** | 374/374 |
| `_ZN10ModStationC2Ev` (alias) | `0xd52d0` | 1008 | **11 / 5** | 374/374 |

The landed form is unchanged:

```cpp
int pitchBits = *(const volatile int *) &pitchTable[race];
int yawBits   = *(const volatile int *) &yawTable[race];
float yaw   = *(const float *) &pitchBits;   // yaw   == pitchBits
float pitch = *(const float *) &yawBits;     // pitch == yawBits
MatrixSetRotation(nearKey, yaw, pitch, 0.0f, (RotationOrder) 2);
...
MatrixSetRotation(farKey, pitch, (Globals::iPad ? gHangarCamYawIPad : gHangarCamYaw)[race], 0.0f,
                  (RotationOrder) 2);
```

## New IR evidence: the far-call operand alone controls the near-block LDRLIT order

Compiling the landed form and the semantically faithful form
(`nearKey(pitch, yaw)`, `farKey(pitch, ...)`) to IR shows the **only** IR
difference is the far call's first float operand:

```
<   invoke ... MatrixSetRotation(... %5, float %108, float %139, ...)   ; landed: far = yawBits
>   invoke ... MatrixSetRotation(... %5, float %107, float %139, ...)   ; faithful: far = pitchBits
```

The near-block IR (`%100 = select (icmp eq), Yaw, YawIPad`, the volatile
loads, the bitcasts) is byte-identical.  Nevertheless the near-block machine
code differs:

| | landed (11) | faithful (43) |
|---|---|---|
| near LDRLITs | `ldr r0,Pitch; ldr r3,YawIPad; ldr r4,Yaw` | `ldr r0,Pitch; ldr r4,Yaw; ldr r3,YawIPad` |
| pool | `Pitch, YawIPad, Yaw` | `Pitch, Yaw, YawIPad` |
| near yaw arg | `ldr.w r4,[...]; ... mov r3,r4` | `ldr.w r3,[...]` (direct) |
| live-out copy | none (`r4` already yawBits) | `mov r4,r2` (early, at `0xd545a`, after the pitch load) |

So the far operand changes both the LDRLIT emission order and the schedule.
This is the whole residual: the landed form buys the correct pool order at the
cost of `mov r3,r4` instead of `mov r4,r2`; the faithful form buys the
`mov r4,r2` schedule at the cost of the pool flip.

The original has **both**: pool `Pitch, YawIPad, Yaw` **and** `mov r4,r2`
late (at `0xd546e`, after `mov r0,r8`).  The faithful form gets the
`mov r4,r2` instruction but schedules it early (at `0xd545a`); the landed form
gets the pool order but uses `mov r3,r4`.  No C++ shape tried reaches both.

## New source shapes measured (all size-exact 1008 B unless noted)

| shape | raw / aligned | note |
|---|---:|---|
| HEAD landed form | **11 / 5** | floor |
| faithful `farKey(pitch)` (all spellings) | 43 / 12 | pool flips |
| `!Globals::iPad ? Yaw : YawIPad` (swapped select, both sites) | 43 / 12 | InstCombine canonicalises back |
| `iPad == 0 ? Yaw : YawIPad`, `iPad < 1 ? ...`, `!!iPad ? ...` | 43 / 12 | same |
| `(iPad ^ 1) ? YawIPad : Yaw` | 44 / 13 | `eor` survives |
| landed form with `farKey(yaw)` (i.e. far = pitchBits), F_STD decl order | 43 / 12 | SSA numbering does not matter |
| `reinterpret_cast<const float &>` faithful (pitch first, near `(pitch,yaw)`, far `pitch`) | 43 / 12 | |
| `reinterpret_cast<const float &>` best (`pitch=pitchBits`, `yaw=yawBits`, near `(yaw,pitch)`, far `yaw`) | **16 / 10** | best sub-43 shape; near args semantically swapped |
| `reinterpret_cast` faithful with near `(yaw,pitch)`, far `yaw` | 16 / 10 | same shape family |
| if/else yaw-table selection (`if (iPad) yawTable = YawIPad; else ...`) | 43 / 12 | canonicalised to select |
| near/far argument-order permutations (2 decl x 2 near x 2 far) | best 11 / 5 | `std_yp_pY` is HEAD; others 13, 16, 18, 43, 44, 45, 46 |
| near arg = fresh `(select)[race]` instead of the `yaw` local, far `pitch` | 43 / 12 | same IR |
| pitch load moved before the select | 736 / 106 (960 B) | size-fatal |
| `volatile int` / `const volatile int` bit locals | 793 / 133 (968 B) | size-fatal |
| far pitch via `*(const volatile float *)&pitchBits` | 631 / 178 (976 B) | size-fatal |
| far pitch via `__builtin_memcpy` / union | 603 / 92 (976 B) | size-fatal |
| far pitch via `volatile int farBits` | 757 / 178 (968 B) | size-fatal |
| far second arg = `yaw` local (no reload) | 604 / 71 (984 B) | size-fatal |
| far `yawTable[race]` (single select, 2 uses) | 604 / 71 (984 B) | size-fatal |
| `yawTable[race]` in far call, landed form | 653 / 92 (968 B) | size-fatal |

All candidates were measured with `measure.sh` (`link_so -j2`,
`ulimit -v 12000000`) and `tools/fn_diff.py`.

## Hand-IR `selswap` measured end-to-end: 49 / 19

The hand-edited IR `selswap` (`icmp ne` + `select(ne), YawIPad, Yaw`, i.e. the
non-canonical select form) was force-linked into the `.so` (compile the `.ll`
with `-Xclang -disable-llvm-passes -c`, drop the object into the build dir, run
the gold link command directly).  It gives the right pool order
(`Pitch, YawIPad, Yaw`) and `mov r4,r2`, but measures **49 / 19**: the ISel
emits `it ne; movne` and swaps the register assignment (`r4=YawIPad`,
`r3=Yaw`) relative to the original (`it eq; moveq`, `r3=YawIPad`, `r4=Yaw`).
So even the non-canonical select form is not the original codegen.  Note that
the normal front-end pipeline canonicalises this IR back to `icmp eq` +
`select(eq), Yaw, YawIPad` anyway.

## `.s`-level confirmation of the ISel order

Compiling both source forms to `.s` shows the near block directly:

```
; landed (pool LCPI0_7=Pitch, _8=YawIPad, _9=Yaw)
ldr r0, .LCPI0_7
ldr r3, .LCPI0_8
ldr r4, .LCPI0_9
...
; faithful (pool LCPI0_7=Pitch, _8=Yaw, _9=YawIPad)
ldr r0, .LCPI0_7
ldr r4, .LCPI0_8
ldr r3, .LCPI0_9
```

The register assignment is the same in both (Yaw -> `r4`, YawIPad -> `r3`);
only the **emission order of the two LDRLIT pseudos** differs.  The ARM
`select_cc` lowering visits the select operands in an order that is sensitive
to the far-block live-out, which is why the far operand flips the pool.

## Conclusion

The 11-byte residual is a post-ISel LDRLIT-order + post-RA schedule tie, not a
wrong constant, callee, or initializer.  The original achieves both the correct
literal-pool order and the `pitchBits`-live `mov r4,r2` schedule; no C++ source
shape tried in this pass (or in w13550) reproduces both.  The current 11 form
is the measured floor for clang 7.0.2 on this TU.  No source change landed; the
tree is clean.

## Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
    _ZN10ModStationC1Ev _ZN10ModStationC2Ev
# raw=11 aligned=5 size=1008/1008 insn=374/374
cmake --build cmake-build-match --target check -j2
```
