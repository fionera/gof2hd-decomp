# ModStation ctor + MGame::OnUpdate — wave 116 (w6419): both residuals re-swept, no improvement

Status: **dead-end sweep, no landed source change.** Worktree `wt/w6419`,
branch `wave116-w6419`, base `d672d349f`. Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`). This note is self-contained: it
records the exact residual bytes, the mechanism, and every variant measured
this wave (plus the prior-wave tables it builds on).

Targets:

* `_ZN10ModStationC1Ev` / `C2Ev`, 1008 B — baseline **11 diff bytes / 3
  contiguous byte runs, 99.2 %** (wave-112 shape, unchanged).
* `_ZN5MGame8OnUpdateEv`, 11440 B — baseline **41 diff bytes / 2 contiguous
  byte runs, 99.9 %** (wave-112 shape, unchanged).

Both are hard local optima: every variant this wave is worse or ties. Nothing
was committed to `src/`; only this note is new.

## 1. ModStation ctor — the far call's first argument is the whole residual

Baseline source (`src/game/menu/ModStation.cpp`, `ModStation::ModStation`):

```cpp
    const float *pitchTable = gHangarCamPitch;
    const float *yawTable = Globals::iPad ? gHangarCamYawIPad : gHangarCamYaw;
    int pitchBits = *(const volatile int *) &pitchTable[race];
    int yawBits = *(const volatile int *) &yawTable[race];
    float yaw = *(const float *) &yawBits;
    float pitch = *(const float *) &pitchBits;
    MatrixSetRotation(nearKey, pitch, yaw, 0.0f, (RotationOrder) 2);
    ...
    MatrixSetRotation(farKey, yaw, (iPad ? gHangarCamYawIPad : gHangarCamYaw)[race], 0.0f, (RotationOrder) 2);
```

Baseline diff (function-relative, function starts at `0xd52d0`):

```text
+0x187..+0x18f (9B) orig=5030282002f8cd80b8 ours=cd80b8f85040282002
+0x19d..+0x19d (1B) orig=40 ours=23
+0x19f..+0x19f (1B) orig=14 ours=40
```

Decoded (absolute addresses):

```text
        original                                  ours
d5456   ldr.w r3,[r0,r8,lsl#2]  ; yaw           d5456  str.w r8,[sp,#184]
d545a   movs  r0,#2                              d545a  ldr.w r4,[r0,r8,lsl#2] ; yaw
d545c   str.w r8,[sp,#184]                       d545e  movs  r0,#2
...                                              ...
d546c   mov   r0,r8                              d546c  mov   r3,r4   ; extra copy
d546e   mov   r4,r2       ; save pitch           d546e  mov   r0,r8
d5470   blx   MatrixSetRotation                  d5470  blx   MatrixSetRotation
```

The original keeps `pitch` in `r4` across the near call and loads `yaw`
straight into argument register `r3`. Ours keeps `yaw` in `r4` (the far call's
first argument) and copies it to `r3`. The far call's first argument is the
only source-level lever.

### 1.1 far=pitch is register-correct but schedule-wrong

Changing only the far call's first argument to `pitch`:

```cpp
    MatrixSetRotation(farKey, pitch, (iPad ? gHangarCamYawIPad : gHangarCamYaw)[race], 0.0f, (RotationOrder) 2);
```

gives **43 B / 8 runs / 1008 B**. The register allocation becomes exactly the
original's (`r2=pitch`, `r3=yaw`, `r4=pitch` saved with `mov r4,r2`, no
`mov r3,r4`), but the pre-RA scheduler reorders three instructions and the
literal-pool order flips from `{Pitch, YawIPad, Yaw}` to `{Pitch, Yaw,
YawIPad}`:

```text
        original                                  far=pitch
d5436   ldr r0,[pc]  ; Pitch                     ldr r0,[pc]  ; Pitch
d5438   ldr r3,[pc]  ; YawIPad                    ldr r4,[pc]  ; Yaw
d543a   ldr r4,[pc]  ; Yaw                        ldr r3,[pc]  ; YawIPad
d543c   add r0,pc                                 add r0,pc
d543e   add r3,pc                                 ldrb r1,[sl]
d5440   ldrb r1,[sl]                              add r4,pc
d5444   add r4,pc                                 ldr r2,[r0,r8,lsl#2] ; pitch
d5446   ldr r2,[r0,r8,lsl#2] ; pitch              add r3,pc
...
d5456   ldr r3,[r0,r8,lsl#2] ; yaw                str r8,[sp,#184]
d545a   movs r0,#2                                 mov r4,r2
d545c   str r8,[sp,#184]                           ldr r3,[r0,r8,lsl#2] ; yaw
...
d546e   mov r4,r2                                  (no save here)
```

The *instruction set* is identical — only the order of `ldr r3,[yaw]`,
`str r8,[sp,#184]` and `mov r4,r2` differs. The scheduler only sinks the `Yaw`
LDRLIT when the `yaw` value is dead after the near call (far=pitch); with
far=yaw the value is live-out and the LDRLIT is hoisted, which is why the
baseline's pool order is right.

### 1.2 Variants measured this wave (all 1008 B unless noted)

Baseline (far=yaw, `yawBits` volatile): **11 B / 3 runs**.

| variant | diff B | runs | size |
|---|---:|---:|---:|
| **baseline (far=yaw, `yawBits` volatile)** | **11** | **3** | 1008 |
| far=pitch | 43 | 8 | 1008 |
| far=pitch + invert both ternaries (`? Yaw : YawIPad`) | 43 | 8 | 1008 |
| far=pitch + `const int yawBits` | 43 | 8 | 1008 |
| far=pitch + `float pitch` declared before `float yaw` | 43 | 8 | 1008 |
| far=pitch + `float pitch` right after `pitchBits` | 43 | 8 | 1008 |
| far=pitch + `yawBits` non-volatile | 44 | 8 | 1008 |
| far=pitch + invert both + `yawBits` non-volatile | 44 | 8 | 1008 |
| far=pitch + inverted local + `yawBits` non-volatile | 45 | 9 | 1008 |
| far=pitch + `volatile int yawBits` | 563 | 34 | 1008 |
| far=pitch + `pitchBits` non-volatile | 666 | 87 | 1008 |
| far=pitch + both bits non-volatile | 667 | 86 | 1008 |
| far=pitch + inverted local + `pitchBits` non-volatile | 667 | 85 | 1008 |
| far=pitch, inline ternary at both call sites, plain `pitch` | 667 | 86 | 1008 |
| far=pitch, inline ternary at both call sites, volatile `pitchBits` | 689 | 95 | 1000 |
| far=pitch + `float yaw = *(const volatile float *)&yawTable[race]` | 528 | 92 | 1016 |
| far=pitch + `yawBits` first | 744 | 38 | 1000 |
| far=pitch + `yawBits`+`yaw` first | 744 | 38 | 1000 |

Prior-wave variants (see `modstation-mgame-wave115-w6320.md`,
`-wave114-`, `-wave113-`) cover `farPitch` copies, `__builtin_assume`,
`volatile float yaw/pitch`, pointer locals, fences, `if/else` spellings,
`yawTable` ordering, and the `(yaw==yaw?…)` / `yaw-yaw` / `yaw*0.0f` tricks;
none beat 11 B / 3 runs (or 4 instruction runs by `verify.py`'s metric).

### 1.3 Why this is a hard local optimum

The two constraints are mutually exclusive at the source level:

* To get the original's register allocation the far call must take `pitch`,
  which makes the local `yaw` dead after the near call.
* A dead `yaw` lets the pre-RA scheduler sink the `Yaw` LDRLIT below the
  `ldrb`, which flips the constant-pool order.
* Keeping `yaw` live into the far call (far=yaw, or a `volatile`/`assume`
  use) restores the pool order but also restores the `ldr r4,[yaw]` +
  `mov r3,r4` shape, i.e. the baseline's 11 B.
* Making the yaw load non-volatile removes the sink but also removes the
  hoist (`Yaw` LDRLIT late), giving 44 B / 8 runs.

The 11 B shape is the best trade-off between the two.

## 2. MGame::OnUpdate — the `Status*` address register (r9 vs r5)

Baseline diff (function-relative; function starts at `0x17c8d8`):

```text
+0x17ba..+0x17ba (1B) orig=00 ours=50
+0x17bc..+0x17e3 (40B)
  orig=f6e3eb6c2817d16cf8da0000f6e9ed1c2800d166f8d90000f6e4ebd0f6e4ebda2800d05ef8d90000
  ours=4628f6e3eb6c2817d16bf8da0000f6e9ed1c2800d165f8d90000f6e4ebd0f6e4ebda2800d05d4628
```

The whole residual is the mission-23 idle-contact condition:

```text
original (r9 = &Globals::status)          ours (r5 = st, r9 = &Globals::status)
17e08c ldr.w r9,[r0]                     17e08c ldr.w r9,[r0]
17e090 ldr.w r0,[r9]   ; reload          17e090 ldr.w r5,[r9]   ; st
17e094 blx   getCurrentCampaignMission   17e094 mov   r0,r5
17e098 cmp   r0,#23                      17e096 blx   getCurrentCampaignMission
17e09a bne   17e176                      17e09a cmp   r0,#23
17e09c ldr.w r0,[sl]                     17e09c bne   17e176
17e0a0 blx   isMining                    17e09e ldr.w r0,[sl]
17e0a4 cmp   r0,#0                       17e0a2 blx   isMining
17e0a6 bne   17e176                      17e0a6 cmp   r0,#0
17e0a8 ldr.w r0,[r9]   ; reload          17e0a8 bne   17e176
17e0ac blx   getMission                  17e0aa ldr.w r0,[r9]
17e0b0 blx   isEmpty                     17e0ae blx   getMission
17e0b4 cmp   r0,#0                       17e0b2 blx   isEmpty
17e0b6 beq   17e176                      17e0b6 cmp   r0,#0
17e0b8 ldr.w r0,[r9]   ; reload          17e0b8 beq   17e176
17e0bc blx   getPlayingTime               17e0ba mov   r0,r5
17e0c0 ldr.w r2,[r9]   ; reload          17e0bc blx   getPlayingTime
                                         17e0c0 ldr.w r2,[r9]
```

Both blocks are 52 B; the difference is a pure permutation. The original
reloads the `Status*` at all four sites and keeps only `&Globals::status` in
`r9`; ours caches the `Status*` in `r5` because the `st` anchor is what forces
the address into `r9`.

### 2.1 The catch-22

The `st` anchor is load-bearing for the *address register*. Without it the
address is an `RS_Split` range with `Prio = Size`, it is dequeued after the
global ranges, and the first free register `r5` wins. The whole function then
recolors to `r5` (11392 B, 5148 B / 208 runs). The anchor is a multi-MBB
global range that takes `r5` first, forcing the address to `r9`. But because
the anchor is loaded *before* the ccm and no call intervenes, IR-level
forwarding merges it with the ccm's load, producing the `mov r0,r5`.

The IR (`-emit-llvm -S`) shows the merge explicitly:

```llvm
; <label>:1090:
  %1091 = load %class.Status*, %class.Status** @_ZN7Globals6statusE
  %1092 = call i32 @_ZN6Status25getCurrentCampaignMissionEv(%class.Status* %1091)
  ...
  %1102 = call i64 @_ZN6Status14getPlayingTimeEv(%class.Status* %1091)
```

A `volatile` read of `Globals::status` does **not** help: the volatile load is
still forwarded to the following non-volatile load (the same `%1091` is used
at the ccm), so `*(Status *volatile *)&Globals::status` is byte-identical to
the baseline (41 B / 2 runs). Same for `(Status *)*(int *)&Globals::status`,
`(Status *)*(void **)&Globals::status`, and `reinterpret_cast` chains.

### 2.2 Variants measured this wave (all 11440 B unless noted)

Baseline (`st` at getPlayingTime): **41 B / 2 runs**.

| variant | diff B | runs | size |
|---|---:|---:|---:|
| **baseline (`st` at getPlayingTime)** | **41** | **2** | 11440 |
| `st` volatile load | 41 | 2 | 11440 |
| `st` via `(Status *)*(int *)&` | 41 | 2 | 11440 |
| `st` via `(Status *)*(void **)&` | 41 | 2 | 11440 |
| `st` via `(Status **)(void *)&` | 41 | 2 | 11440 |
| `st` via `reinterpret_cast` chain | 41 | 2 | 11440 |
| `st` at ccm + getMission + getPlayingTime | 5303 | 621 | 11436 |
| `st` at all four m23 sites | 5356 | 594 | 11432 |
| `st` at getMission + getPlayingTime + field | 5356 | 594 | 11432 |
| `long long idleLimit = 3600000` local | 727 | 674 | 11440 |
| `st` before `jumpDriveActive` block | 6698 | 334 | 11372 |
| `st` before `elapsedTime > 5000` | 8721 | 535 | 11392 |
| `st` before `shouldSwitchToFreeLookCam` | 10299 | 453 | 11316 |
| `st` before `startSequenceOver` block | 10260 | 485 | 11336 |
| no `st` | 5148 | 208 | 11392 |

Prior-wave variants (see `modstation-mgame-wave115-w6320.md`,
`-wave114-`, `-wave113-`, `mgame-onupdate-wave112-w6019.md`) cover every
`st` placement (ccm / getMission / getPlayingTime / field / comma / top),
`Status *&`, `Status **`, `Status &`, `const`, `volatile`, fences,
`__builtin_assume`, `PlayerEgo *` / `LevelScript *` / `float elapsed`
anchors; none beat 41 B / 2 runs.

### 2.3 Why this is a hard local optimum

The greedy register allocator needs a high-priority multi-MBB range to take
`r5` before the `&Globals::status` range is dequeued. Any such anchor must be
a real value live across the m23 block, and every candidate either (a) is the
`Status*` itself (forwarded into the ccm, adding `ldr r5` + `mov r0,r5`), or
(b) changes a use site (e.g. `mov r0,r5` instead of `ldr r0,[sl]` for
`isMining`), or (c) recolors the whole 11 KB function. The 41 B shape is the
only one that keeps every other byte of the function identical.

## 3. Structural state (unchanged by this wave)

```text
sections            27 (exact; same count both binaries)
file size           2,187,328 B (exact, both)
.text               0x1337c8 @ 0x69440   (size/base exact)
.ARM.exidx          0x6f70   @ 0x19cc08  (size/base exact)
.ARM.extab          0x26294 ours / 0x26298 orig   (4 B short, pre-existing)
.rodata             0x41c27 ours / 0x41b67 orig   (+0xC0 rank table)
.got                0x430c   @ 0x20fcf4  (size/base exact)
.data               0x296c   @ 0x214000  (size/base exact)
.bss                0x69ac   @ 0x216970  (size/base exact)
```

No section, `.ARM.extab` or file-size delta was introduced (no source change).

## 4. Gate

```text
verify: byte 4501  linked 4501  stubs 0  missing 0  extra 0  avg 99.92
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS      (goal_lint CLEAN, drift 0)
```

Before -> after: byte 4501 -> 4501, linked 4501 -> 4501. ModStation 11 B / 3
runs / 1008 B; MGame::OnUpdate 41 B / 2 runs / 11440 B.

## 5. Scratch (not committed)

`/dev/shm/w6419/` — `measure.py`, `v/` (variant sources `b1 b2 c1 c2 c3 e1..e8
m1..m4 r1..r4 w1..w6 x1 x2 y1..y4`, `base.cpp`, `mgame_base.cpp`), plus the
prior wave's `mirorder.py` / `mirsweep.py` / `emitll.sh`.

## 6. References

* `_work/reconstructions/modstation-mgame-wave115-w6320.md` — prior wave,
  redundant-fence removal, full variant tables.
* `_work/reconstructions/modstation-mgame-wave114-w6220.md` — DAG live-out /
  r5-anchor sweeps.
* `_work/reconstructions/modstation-mgame-wave113-w6120.md` — far=pitch 42/7,
  Status-anchor sweeps.
* `_work/reconstructions/modstation-ctor-wave112-w6018.md` — landed 11 B shape.
* `_work/reconstructions/mgame-onupdate-wave112-w6019.md` — greedy-RA r5/r9
  mechanism.
