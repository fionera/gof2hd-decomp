# ModStation ctor + MGame::OnUpdate — wave 117 (w6519): both residuals re-swept, no improvement

Status: **dead-end sweep, no landed source change.** Worktree `wt/w6519`,
branch `wave117-w6519`, base `c22678d64`. Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`). This note is self-contained: it
records the exact baseline residuals, the new variants measured this wave, and
the mechanism that makes each a local optimum.

Targets:

* `_ZN10ModStationC1Ev` / `C2Ev`, 1008 B — baseline **11 diff bytes / 4 byte
  runs, 99.2 %** (wave-112 shape, unchanged).
* `_ZN5MGame8OnUpdateEv`, 11440 B — baseline **41 diff bytes / 1 byte run,
  99.9 %** (wave-112 shape, unchanged).

Nothing beat either baseline. Every `src/` variant was reverted; only this note
is new. The gate is unchanged: byte 4501 / linked 4501.

## 1. ModStation ctor — the far call's first argument is still the lever

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
    MatrixSetRotation(farKey, yaw, (ternary)[race], 0.0f, (RotationOrder) 2);
```

Baseline diff (function-relative, function starts at `0xd52d0`):

```text
+0x186..+0x186 (1B)  orig=50 ours=cd
+0x188..+0x18f (8B)  orig=28300220cdf8b880 ours=b88050f828400220
+0x19c..+0x19c (1B)  orig=40 ours=23
+0x19e..+0x19e (1B)  orig=14 ours=40
```

The original's far call is `(pitch, freshYaw)`; the baseline's is
`(yaw, freshYaw)`. The original keeps `pitch` in `r4` across the near call
(`mov r4,r2`) and loads `yaw` straight into `r3`; the baseline keeps `yaw` in
`r4` and copies it to `r3` (`mov r3,r4`). Both blocks are 1008 B.

### 1.1 New measurements this wave (all 1008 B unless noted)

| variant | diff B | byte runs |
|---|---:|---:|
| **baseline (far = yaw, near = yaw local)** | **11** | **4** |
| far = pitch, far y = fresh ternary | 43 | 8 |
| far = pitch + `farPitch = pitch` copy | 43 | 8 |
| far = pitch + int-bit locals bitcast at both calls | 43 | 8 |
| far = pitch + union bitcast for pitch | 43 | 8 |
| far = pitch + `yaw * 1.0f` / `yaw - 0.0f` / `(float)(double)yaw` at near y | 43 | 8 |
| far = pitch + `(yaw, pitch)` comma expression | 43 | 8 |
| far = pitch + `pitch * (yaw == yaw ? 1 : 1)` | 43 | 8 |
| far = pitch, `const float yaw` | 43 | 8 |
| far = pitch, near = `yawTable[race]` (fresh load) | 44 | 7 |
| far = pitch, near = `yawTable[race]`, yawTable declared before pitchTable | 44 | 7 |
| far = pitch, `pitchTable` declared after `yawTable` | 46 | 9 |
| far = pitch, `yaw + 0.0f` / `yaw + 0` at near y | 548 | 81 (1020 B) |
| far = pitch, far y = yaw local / `*(float*)&yawBits` / `yawTable[race]` | 580 | 72 (984 B) |
| far = pitch, both ternaries inverted | 688 | 93 (1000 B) |
| far = pitch, near ternary inverted | 561 | 85 |
| far = pitch, no yaw local (both calls ternary) | 689 | 92 (1000 B) |
| far = pitch, `volatile float farPitch` | 611 | 72 (1016 B) |
| far = pitch, `pitch + (yaw - yaw)` | 649 | 76 (1024 B) |
| far = pitch, `pitch + 0.0f * yaw` | 649 | 77 (1028 B) |
| `const volatile float yaw`, far y = yaw local | 618 | 65 (992 B) |

The 11 B / 4 shape remains the best. The mechanism is unchanged from wave 114:
the original's DAG pulls the `Yaw` chain early because a live-out float gets a
materialised `VMOVSR`; with far = pitch the live-out float is `pitch`, so the
`Yaw` LDRLIT sinks below the `ldrb` and the literal-pool order flips from
`{Pitch, YawIPad, Yaw}` to `{Pitch, Yaw, YawIPad}`. Every spelling that keeps
`yaw` live into the far call either reuses the value in a register (984 B) or
recolors.

### 1.2 Why this is a hard local optimum

The two constraints are mutually exclusive at the source level:

* To get the original's register allocation the far call must take `pitch`,
  which makes the local `yaw` dead after the near call.
* A dead `yaw` lets the SelectionDAG sink the `Yaw` LDRLIT below the `ldrb`,
  which flips the constant-pool order (43 B).
* Keeping `yaw` live into the far call (far = yaw, or a real second use)
  restores the pool order but also restores the `ldr r4,[yaw]` + `mov r3,r4`
  shape, i.e. the baseline's 11 B.

The byte metric prefers the 11 B shape.

## 2. MGame::OnUpdate — the `st` anchor is the whole residual

Baseline diff (function-relative; function starts at `0x17c8d8`):

```text
+0x17ba..+0x17ba (1B) orig=00 ours=50
+0x17bc..+0x17e3 (40B) permutation
```

The whole residual is the mission-23 idle-contact condition. The original
reloads `Globals::status` at all four sites and keeps only `&Globals::status` in
`r9`; ours caches the `Status*` in `r5` because the `st` anchor is what forces
the address into `r9`.

### 2.1 Exhaustive four-site sweep (new this wave)

The m23 condition has four `Globals::status` uses: ccm, getMission,
getPlayingTime, field. Enumerating every `st` vs `Globals::status` choice at the
four sites (16 combinations, all 11440 B unless noted):

| ccm | mission | play | field | diff B | runs | size |
|---|---|---|---|---|---|---|
| G | G | G | G | 5148 | 212 | 11392 |
| G | G | G | S | 4993 | 301 | 11436 |
| **G** | **G** | **S** | **G** | **41** | **1** | **11440** |
| G | G | S | S | 4981 | 307 | 11436 |
| G | S | G | G | 5129 | 240 | 11396 |
| G | S | G | S | 4966 | 308 | 11436 |
| G | S | S | G | 4982 | 303 | 11436 |
| G | S | S | S | 5033 | 270 | 11432 |
| S | G | G | G | 5148 | 212 | 11392 |
| S | G | G | S | 4993 | 301 | 11436 |
| **S** | **G** | **S** | **G** | **41** | **1** | **11440** |
| S | G | S | S | 4981 | 307 | 11436 |
| S | S | G | G | 5129 | 240 | 11396 |
| S | S | G | S | 4966 | 308 | 11436 |
| S | S | S | G | 4982 | 303 | 11436 |
| S | S | S | S | 5033 | 270 | 11432 |

Only the two shapes with `play = S`, `mission = G`, `field = G` give 41 B / 1
run (the ccm is CSE-merged with the anchor, so `G` and `S` are equivalent
there). The anchor must be live across the m23 block to force the address into
`r9`; using it at any other site shortens the range and recolors.

### 2.2 New anchor-position / anchor-type variants

| variant | size | diff B | runs |
|---|---:|---:|---:|
| **baseline (`st` at m23 getPlayingTime)** | **11440** | **41** | **1** |
| `Status *st` at m23 top, body uses `st` (field + play) | 11524 | 6230 | 297 |
| `Status *st` at m23 top, body uses `st` (field only) | 11524 | 6234 | 295 |
| `Status *st` at m23 top, body uses `st` (field + play), cond play = st | 11520 | 6233 | 306 |
| `Status *st` moved to m24 condition (m23 all G) | 11440 | 270 | 9 |
| `Status *st` moved to m122 condition (m23 all G) | 11440 | 540 | 20 |
| `Status *st` at m24 + m122 (m23 all G) | 11436 | 4741 | 305 |
| `Status **stp = &Globals::status` used at field | 11392 | 5148 | 212 |
| `Status *st` assigned after ccm, used at play | 11392 | 5148 | 212 |
| `Status *st` assigned after ccm (comma), used at play | 11392 | 5148 | 212 |
| `Status *st` assigned after getMission, used at play | 11392 | 5148 | 212 |
| `long long lastVisit = status->field_0x100` before the if | 11432 | 5040 | 270 |
| `long long lastVisit` + `st` | 11432 | 5040 | 270 |
| `st` + `long long lastVisit` | 11436 | 4982 | 309 |
| `this->camera != nullptr` first in the m23 condition | 11448 | 5063 | 272 |
| `this->camera != nullptr` last in the m23 condition | 11448 | 5036 | 271 |
| `if (this->camera == nullptr) { }` in the m23 body (folded away) | 11440 | 41 | 1 |
| `st` anchor before the `levelScript->m_nScriptTime` if | 11392 | 8724 | 534 |
| `st` anchor before the `elapsedTime > 5000` if | 11392 | 8721 | 533 |
| `memcpy(&st, &Globals::status, 4)` | 11440 | 41 | 1 |
| `*(Status *volatile *) &Globals::status` | 11440 | 41 | 1 |
| `*(Status **) (char/void *) &Globals::status` | 11440 | 41 | 1 |
| `*(void **) &st = *(void **) &Globals::status` | 11440 | 41 | 1 |
| `Status *st2 = *(Status *volatile *) &Globals::status` at the ccm | 11440 | 853 | 36 |

### 2.3 Why this is a hard local optimum

Wave 114 established that the original's `r5` holds `&this->camera`
(`fp + 244`) across the camera block at `17da24..17db2e`; the greedy
allocator's `tryAssign` for the long status GOT-base interval still sees `r5`
as interfering with the later scratch uses (`17f216` onwards), so it picks
`r9`. In our no-`st` build the allocator instead splits the GOT-base interval
and gives the first piece `r5`, which recolours the whole function
(11392 B / 5148 diff / 212 runs, and a 128 B smaller stack frame).

The `st` anchor is a substitute for that `r5` occupancy: it is a multi-MBB
global range that takes `r5` first, forcing the address into `r9`. But because
the anchor is loaded before the ccm and no call intervenes, IR-level forwarding
merges it with the ccm's load, producing `ldr r5,[r9]; mov r0,r5` instead of
`ldr r0,[r9]`. Every attempt to break the CSE while keeping the anchor either
adds a load (`853 B / 36 runs`) or recolours the whole function. Every attempt
to reproduce the original's `r5` occupancy (`this->camera`, a `long long`
`lastVisit`, a `Status**` address anchor) either changes a use site or changes
the stack frame.

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

No section, `.ARM.extab` or file-size delta was introduced.

## 4. Gate

```text
verify: byte 4501  linked 4501  stubs 0  missing 0  extra 0  avg 99.92
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS      (goal_lint CLEAN, drift 0)
```

Before -> after: byte 4501 -> 4501, linked 4501 -> 4501. ModStation 11 B / 4
byte runs / 1008 B; MGame::OnUpdate 41 B / 1 byte run / 11440 B.

## 5. Scratch (not committed)

`/dev/shm/w6519/` — `measure.py`, `tryv.py`, `trym.py`, `run1.py`..`run12.py`,
`runm1.py`..`runm10.py`, `ModStation.base.cpp`, `MGame.base.cpp`,
`base.ll`/`fp.ll` (`-emit-llvm -S` dumps), `mirbase.txt`/`mirfp.txt`/
`mg_nost.txt` (`-mllvm -print-machineinstrs` dumps).

## 6. References

* `_work/reconstructions/modstation-mgame-wave116-w6419.md` — wave-116
  dead-end sweep, full variant tables.
* `_work/reconstructions/modstation-mgame-wave115-w6320.md` — fence removal,
  full variant tables.
* `_work/reconstructions/modstation-mgame-wave114-w6220.md` — DAG live-out
  mechanism, `&this->camera` r5 occupancy.
* `_work/reconstructions/modstation-ctor-wave112-w6018.md` — landed 11 B shape.
* `_work/reconstructions/mgame-onupdate-wave112-w6019.md` — greedy-RA r5/r9
  mechanism.
