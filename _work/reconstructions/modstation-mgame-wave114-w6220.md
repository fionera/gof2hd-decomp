# ModStation ctor + MGame::OnUpdate — wave 114 (w6220): DAG live-out / r5-anchor sweeps

Status: **research pass, no `src/` change landed.** Worktree `wt/w6220`, branch
`wave114-w6220`, base `d8557f7e2`. Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`).

Two targets were re-swept with new hypotheses:

* `_ZN10ModStationC1Ev` / `C2Ev`, 1008 B — baseline **11 diff bytes / 4 runs,
  99.2 %** (wave-112 shape, unchanged).
* `_ZN5MGame8OnUpdateEv`, 11440 B — baseline **41 diff bytes / 1 run, 99.9 %**
  (wave-112 shape, unchanged).

Nothing beat either baseline. This note records the new measurements, the
sharpened mechanism for both residuals (ModStation = SelectionDAG live-out
float; MGame = the original's `r5` is `&this->camera`, not a GOT base), and
every dead end tried this wave.

## 1. ModStation — the residual is decided in the SelectionDAG, not the scheduler

The wave-113 note located the far=pitch/far=yaw split in the "pre-RA Machine
Instruction Scheduler" output. This wave pins it one pass earlier: the two
shapes already differ in the **`Expand ISel Pseudo-instructions`** dump (i.e.
the SelectionDAG scheduler output), before any machine scheduler runs.

`-mllvm -print-after-all` of bb.12 (near `MatrixSetRotation`), both shapes:

```text
baseline (far = yaw)                    far = pitch
%114 LDRLIT iPad                        %114 LDRLIT iPad
%115 ldr  [got]                         %115 ldr  [got]
%116 LDRLIT Pitch                       %116 LDRLIT Pitch
%117 LDRLIT YawIPad                     %117 LDRLIT YawIPad
%118 LDRLIT Yaw                         LIFETIME_END %stack.2
LIFETIME_END %stack.2                   %118 ldrb  [iPad]
%119 ldrb  [iPad]                       %119 LDRLIT Yaw
CompilerBarrier                         CompilerBarrier
%120 ldr  pitch                         %120 ldr  pitch
cmp                                     cmp
%121 MOVCC %117, %118                   %121 MOVCC %117, %119
%122 ldr  yaw                           %122 ldr  yaw
%15:spr = VMOVSR %122   ; yaw           %15:spr = VMOVSR %120   ; pitch
```

The only IR difference between the two builds is the far call's first argument
(`%108` yaw vs `%109` pitch); bb.12's IR is identical. The DAG nevertheless
orders the `Yaw` LDRLIT differently because the **live-out float gets a
`VMOVSR`** (a materialised bitcast), and that consumer pulls its whole chain
(`LDRLIT Yaw` -> `MOVCC` -> `ldr yaw` -> `VMOVSR`) ahead of the `ldrb`:

* far = yaw -> `yaw` is live-out -> `VMOVSR(yaw)` -> `Yaw` LDRLIT early
  (baseline, 11 B / 4 runs);
* far = pitch -> `pitch` is live-out -> `VMOVSR(pitch)` -> `Yaw` LDRLIT sinks
  below the `ldrb` (far=pitch, 42 B / 7 runs).

The pre-RA and post-RA machine schedulers then faithfully carry that DAG order
through; the register assignment is otherwise exactly the original's
(`r2 = pitch`, `r3 = yaw`, `r4 = pitch` saved with `mov r4,r2`). So the
original source is semantically `far = pitch`, and its DAG must have had the
`Yaw` chain pulled early by a *second* live-out float use. No source shape
found reproduces that without changing the far call.

### 1.1 Residual bytes (baseline)

```text
        orig                                    ours
d5456   ldr.w r3,[r0,r8,lsl#2]   ; yaw      d5456  str.w r8,[sp,#184]
d545a   movs  r0,#2                         d545a  ldr.w r4,[r0,r8,lsl#2] ; yaw
d545c   str.w r8,[sp,#184]                  d545e  movs  r0,#2
...                                         ...
d546c   mov   r0,r8                         d546c  mov   r3,r4   ; extra copy
d546e   mov   r4,r2                         d546e  mov   r0,r8
```

```text
+0x186..+0x186 (1B)   +0x188..+0x18f (8B)   +0x19c..+0x19c (1B)   +0x19e..+0x19e (1B)
```

### 1.2 Variants measured this wave (all 1008 B unless noted)

Baseline (far = yaw, fresh yaw table load): **11 B / 4 runs / 99.2 %**.

| variant | diff B | runs | score |
|---|---:|---:|---:|
| far = pitch, fresh yaw (semantically clean) | 42 | 7 | 98.6 |
| far = pitch, fresh yaw, `float pitch` declared first | 42 | 7 | 98.6 |
| far = pitch, fresh yaw, `const float` locals | 42 | 7 | 98.6 |
| far = pitch, fresh yaw, `pitch; yaw;` then assignments | 42 | 7 | 98.6 |
| far = pitch, fresh yaw, ternary `!iPad ? Yaw : YawIPad` | 42 | 7 | 98.6 |
| far = pitch, fresh yaw, ternary `iPad == 0 ? ...` | 42 | 7 | 98.6 |
| far = pitch, fresh yaw, ternary `iPad != 0 ? ...` | 42 | 7 | 98.6 |
| far = pitch, `yaw` local as y | 580 | 72 | 84.2 (984 B) |
| far = yaw, `yaw` local as y | 613 | 33 | 79.4 (968 B) |
| far = pitch, `yawTable[race]` (local pointer) | 580 | 72 | 84.2 (984 B) |
| far = pitch, `*(volatile float*)&yawTable[race]` | 698 | 42 | 82.4 (992 B) |
| far = yaw, `*(volatile float*)&yawTable[race]` | 698 | 42 | 82.4 (992 B) |
| far = pitch, far z = `yaw - yaw` | 746 | 81 | 79.1 (1024 B) |
| far = pitch, far z = `yaw * 0.0f` / `0.0f * yaw` | 755 | 74 | 78.6 (1028 B) |
| far = pitch, far z = `yaw + (-yaw)` | 746 | 81 | 79.1 (1024 B) |
| near y = fresh table load, far = pitch | 649 | 35 | 85.7 |
| near y = fresh table load, far = yaw | 686 | 48 | 86.9 (1016 B) |
| near y = `*(float*)&yawBits` (no float local), far = pitch | 42 | 7 | 98.6 |
| near y = `*(float*)&yawBits`, far = yaw | 11 | 4 | 99.2 |
| plain floats (no volatile bitcasts), far = fresh yaw | 672 | 85 | 88.9 |
| plain floats, far = fresh pitch | 667 | 86 | 86.7 |
| plain floats, far = `yaw` local | 701 | 79 | 78.4 (984 B) |
| `float farYaw = yawTable[race];` before near call | 580 | 72 | 84.2 (984 B) |

Every far=pitch shape is >= 42 B; every shape that keeps `yaw` live into the
far call is 11 B (the baseline) or worse. The 11 B / 4 runs shape is a local
optimum.

### 1.3 Additional far=pitch variants (post-commit)

These were measured after `85c7ea070`, with `MGame.cpp` pristine.  They test
whether the far call can keep the original's `pitch` argument while still
forcing the `Yaw` literal materialisation early through a cheaper live-out.

| variant | size | diff B | runs | score |
|---|---:|---:|---:|---:|
| far=pitch, fresh yaw, `yawBits` non-volatile (`pitchBits` volatile) | 1008 | 43 | 7 | 97.5 |
| far=pitch, fresh yaw, both bit loads non-volatile | 1008 | 629 | 81 | 85.9 |
| far=pitch, fresh yaw, fence **after** the near call | 1008 | 42 | 7 | 98.6 |
| far=yaw, fence **after** the near call | 1008 | 11 | 4 | 99.2 (tie) |
| far=pitch, volatile far load, non-volatile near yaw load | 992 | 699 | 40 | 83.5 |
| far=pitch, second arg = `yaw` local (also `const float`) | 984 | 580 | 72 | 84.2 |
| far=pitch, second arg = `*(const float *)&yawBits` | 984 | 580 | 72 | 84.2 |

Reusing the near `yaw` value in any spelling lets clang keep it in a register
and shortens the function by 24 B (1008 -> 984), so the original really does
reload the table word at the far call.  Removing the volatile qualifier on
`yawBits` does not make the far load CSE with it (43 B / 7 runs), and moving
the compiler barrier after the near call changes nothing (42 B / 7 for
far=pitch, 11 B / 4 tie for far=yaw).  The landed 11 B / 4 shape remains the
best measured variant.

## 2. MGame::OnUpdate — the original's r5 is `&this->camera`, not a GOT base

Baseline: **41 B / 1 run / 11440 B / 99.9 %** at `+0x17bb..+0x17e3`
(absolute `0x17e093..0x17e0bb`). The whole residual is the mission-23
idle-contact condition.

```text
original (r9 = &Globals::status)         ours (r5 = st, r9 = &Globals::status)
17e08c ldr.w r9,[r0]                    17e074 ldr.w r9,[r0]
17e090 ldr.w r0,[r9]   ; reload         17e078 ldr.w r5,[r9]   ; st
17e094 blx   getCurrentCampaignMission  17e07c mov   r0,r5
17e098 cmp   r0,#23                     17e07e blx   getCurrentCampaignMission
17e09a bne   17e176                     17e082 cmp   r0,#23
17e09c ldr.w r0,[sl]                    17e084 bne   17e15e
17e0a0 blx   isMining                   17e086 ldr.w r0,[sl]
17e0a4 cmp   r0,#0                      17e08a blx   isMining
17e0a6 bne   17e176                     17e08e cmp   r0,#0
17e0a8 ldr.w r0,[r9]   ; reload         17e090 bne   17e15e
17e0ac blx   getMission                 17e092 ldr.w r0,[r9]   ; reload
17e0b0 blx   isEmpty                    17e096 blx   getMission
17e0b4 cmp   r0,#0                      17e09a blx   isEmpty
17e0b6 beq   17e176                     17e09e cmp   r0,#0
17e0b8 ldr.w r0,[r9]   ; reload         17e0a0 beq   17e15e
17e0bc blx   getPlayingTime              17e0a2 mov   r0,r5
17e0c0 ldr.w r2,[r9]   ; reload         17e0a4 blx   getPlayingTime
                                        17e0a8 ldr.w r2,[r9]
```

Both blocks are 52 B; the difference is a pure permutation: the original has
four `ldr r0/r2,[r9]` reloads, ours has three reloads plus a `st` anchor load
and two `mov r0,r5`.

### 2.1 New: what actually occupies r5 in the original

Wave 112/113 assumed the original simply had no long-lived value in r5. The
disassembly shows otherwise: **r5 holds `&this->camera` (`fp + 244`) across
the camera block**, and that value is what keeps the status GOT base out of
r5:

```text
17da24 mov   r5, fp
17da2a ldr.w r0, [r5, #244]!   ; r5 = &this->camera, r0 = this->camera
17dae2 ldr   r0, [r5, #0]      ; this->camera (setFirstPersonMatrix path)
17db06 ldr   r0, [r5, #0]      ; this->camera (hideShipForFirstPersonCam)
17db2e ldr   r0, [r5, #0]      ; this->camera (hideShipForFirstPersonCameraView)
```

This is the `this->camera->...` cluster at `src/game/menu/MGame.cpp:3316-3327`.
The value dies at `17db2e`, so r5 is free again at the m23 block, but the
greedy allocator's `tryAssign` for the long status GOT-base interval still
sees r5 as interfering with the later scratch uses (`17f216` onwards), so it
picks r9. In our no-`st` build the allocator instead splits the GOT-base
interval and gives the first piece r5, which recolours the whole function
(11392 B / 5148 diff / 212 runs / 87.2 %).

The `st` anchor is therefore a *substitute* for the original's r5 occupancy;
it works, but it is CSE-merged with the ccm load, which is exactly the source
of the `mov r0,r5`.

### 2.2 Variants measured this wave

Baseline: **41 B / 1 run / 99.9 %**.

| variant | size | diff B | runs | score |
|---|---:|---:|---:|---:|
| baseline (`st` used at m23 getPlayingTime) | 11440 | 41 | 1 | 99.9 |
| no `st` (all four sites `Globals::status`) | 11392 | 5148 | 212 | 87.2 |
| `Status **statusPtr = &Globals::status;` + `(*statusPtr)->…` ×4 | 11392 | 5148 | 212 | 87.2 |
| `st` assigned after ccm (`(st = Globals::status)->getMission()`) | 11396 | 5129 | 240 | 87.1 |
| `st` at ccm + getMission | 11396 | 5129 | 240 | 87.1 |
| `st` at getMission + getPlayingTime | 11436 | 4982 | 303 | 92.5 |
| `st` declared before the outer `elapsedTime > 5000` if | 11392 | 8721 | 533 | 60.1 |
| `st` at m23 block top (same as baseline placement) | 11440 | 41 | 1 | 99.9 |

The last row is the baseline placement with the comment line removed: it is
byte-identical to the baseline, which confirms the anchor position (not the
comment) is what matters.

Post-commit follow-up measurements (same pristine source, after `85c7ea070`):

| variant | size | diff B | runs | score |
|---|---:|---:|---:|---:|
| `__atomic_signal_fence` **before** `Status *st` | 11440 | 41 | 1 | 99.9 (tie) |
| `__atomic_signal_fence` **after** `Status *st` | 11440 | 853 | 36 | 99.0 |
| `Status *volatile st` | 11340 | 6076 | 317 | 82.1 |
| fence + `st->field_0x100` (both uses via `st`) | 11436 | 4990 | 303 | 92.5 |
| no fence + `st->field_0x100` | 11436 | 4981 | 307 | 92.7 |
| `st` assigned after ccm + `st->field_0x100` | 11392 | 5145 | 214 | 87.1 |

The fence-before-`st` tie confirms the anchor mechanism is insensitive to a
compiler barrier at that position; every shape that routes the ccm load through
`st` (rather than letting CSE merge it) recolours the whole function.

### 2.3 Measurement caveat

`ModStation.cpp` and `MGame.cpp` share one link.  Changing the *other* TU can
shift the `.rodata`/literal-pool layout and add spurious pool-word diffs to
the function under test (e.g. the baseline MGame reads 41 B with the pristine
ModStation but 754 B with a size-changing ModStation variant).  Every number
in this note was re-measured with the other TU pristine.

## 3. Structural state (unchanged — no `src/` change)

```text
sections            27 (exact; same count both binaries)
file size           2,187,328 B (exact, both)
.text               0x1337c8 @ 0x69440   (size/base exact)
.ARM.exidx          0x6f70   @ 0x19cc08  (size/base exact)
.ARM.extab          0x26298  @ 0x1a3b78  (size/base exact)
.rodata             0x41c27 ours / 0x41b67 orig (+0xC0 rank table)
.got                0x430c   @ 0x20fcf4  (size/base exact)
.data               0x296c   @ 0x214000  (size/base exact)
.bss                0x69ac   @ 0x216970  (size/base exact)
```

No section, `.ARM.extab` or file-size delta was introduced.

## 4. Gate (note-only pass, pristine source restored)

```text
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.92
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS      (goal_lint CLEAN, drift 0)
```

Before -> after: byte 4500 -> 4500, linked 4500 -> 4500. ModStation 11 B / 4
runs / 1008 B; MGame::OnUpdate 41 B / 1 run / 11440 B.

## 5. Scratch (not committed)

`/dev/shm/w6220/` — `measure.py`, `build.sh`, `mir.sh`, `mir1.sh`,
`ModStation.base.cpp`, `MGame.base.cpp`, `try*.py`, `sweep2.py`-`sweep4.py`,
`mir_fp2.txt` (far=pitch full `-print-after-all` dump), `mir_base.txt`
(baseline dump).

## 6. References

* `_work/reconstructions/modstation-mgame-wave113-w6120.md` — far=pitch 42/7,
  Status-anchor sweeps.
* `_work/reconstructions/modstation-ctor-wave112-w6018.md` — landed 11 B / 4 runs shape.
* `_work/reconstructions/modstation-ctor-pass5-w4263.md` §6.2 — hand-IR `best16d`.
* `_work/reconstructions/mgame-onupdate-wave112-w6019.md` — greedy-RA r5/r9
  mechanism, 40 shapes.
* `_work/reconstructions/residual-taxonomy-wave111-w5920.md` — diff taxonomy.
