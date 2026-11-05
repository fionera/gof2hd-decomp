# ModStation ctor + MGame::OnUpdate — wave 118 (w6619): DAG-order study, no source win

Status: **dead-end sweep, no landed source change.** Worktree `wt/w6619`,
branch `wave118-w6619`, base `3d4a98a3e`. Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`). This note is self-contained: it
records the exact baseline residuals, the mechanism behind each, and every
variant measured this wave (including the ones that only shift the score
sideways).

Targets:

* `_ZN10ModStationC1Ev` / `C2Ev`, 1008 B — baseline **11 diff bytes / 4 byte
  runs, 99.2 %** (wave-112 shape, unchanged).
* `_ZN5MGame8OnUpdateEv`, 11440 B — baseline **41 diff bytes / 1 byte run,
  99.9 %** (wave-112 shape, unchanged).

Nothing beat either baseline. Every `src/` variant was reverted; only this note
is new. The gate is unchanged: byte 4501 / linked 4501, lint CLEAN, drift 0,
imports 207, data 843/843.

## 1. ModStation ctor — the far call is `(pitch, freshYaw)`

The source file is `src/game/menu/ModStation.cpp` (not `src/game/ui/`), ctor at
`ModStation::ModStation`.

Baseline source (the shape that scores 11 B):

```cpp
    const float *pitchTable = gHangarCamPitch;
    const float *yawTable = Globals::iPad ? gHangarCamYawIPad : gHangarCamYaw;
    int pitchBits = *(const volatile int *) &pitchTable[race];
    int yawBits = *(const volatile int *) &yawTable[race];
    float yaw = *(const float *) &yawBits;
    float pitch = *(const float *) &pitchBits;
    MatrixSetRotation(nearKey, pitch, yaw, 0.0f, (RotationOrder) 2);
    ...
    MatrixSetRotation(farKey, yaw, (Globals::iPad ? gHangarCamYawIPad : gHangarCamYaw)[race],
                      0.0f, (RotationOrder) 2);
```

Baseline diff (function-relative, function starts at `0xd52d0`):

```text
+0x186..+0x186 (1B)  orig=50 ours=cd
+0x188..+0x18f (8B)  orig=28300220cdf8b880 ours=b88050f828400220
+0x19c..+0x19c (1B)  orig=40 ours=23
+0x19e..+0x19e (1B)  orig=14 ours=40
```

### 1.1 Ground truth: the original far call is `(pitch, freshYaw)`

Original Thumb (function-relative offsets; `MatrixSetRotation(Matrix&, float x,
float y, float z, RotationOrder)` is sret: `r0`=sret, `r1`=&matrix, `r2`=x,
`r3`=y, stack=z/order):

```text
166: ldr   r0, [pc,#632]        ; gHangarCamPitch
168: ldr   r3, [pc,#632]        ; gHangarCamYawIPad
16a: ldr   r4, [pc,#636]        ; gHangarCamYaw
16c: add   r0, pc
16e: add   r3, pc
170: ldrb.w r1, [sl]            ; Globals::iPad
174: add   r4, pc
176: ldr.w r2, [r0, r8, lsl#2]  ; pitch  (volatile int load)
17a: mov   r0, r3
17c: cmp   r1, #0
17e: str   r3, [sp,#180]
180: str   r4, [sp,#176]
182: it    eq
184: moveq r0, r4               ; yawTable = iPad ? YawIPad : Yaw
186: ldr.w r3, [r0, r8, lsl#2]  ; near yaw  -> r3, dead after the call
188: movs  r0, #2
18a: str.w r8, [sp,#184]        ; spill race
18e: movs  r1, #0
190: add.w r8, sp, #256
194: strd  r1, r0, [sp]
198: add   r1, sp, #320
19c: mov   r0, r8
19e: mov   r4, r2               ; preserve pitch across the call
1a0: blx   MatrixSetRotation
...
210: ldrb.w r0, [sl]
214: cmp   r0, #0
216: ldrd  r0, r1, [sp,#176]
21a: it    eq
21c: moveq r1, r0
21e: ldr   r0, [sp,#184]
220: mov   r2, r4               ; far x = pitch
222: ldr.w r3, [r1, r0, lsl#2]  ; far y = fresh table load
```

So the original's far call is `(pitch, yawTable[race])`, i.e. the faithful
source is `far = pitch`, not `far = yaw`. The baseline's `far = yaw` is a real
semantic bug that happens to score better in bytes.

### 1.2 The 11 B vs 43 B trade

With `far = pitch` the yaw value dies after the near call; with `far = yaw` it
is live across the call. That single IR difference (verified by `-emit-llvm`:
the only change is `float %107` vs `float %108` in the far `invoke`) flips the
SelectionDAG's topological order of the two yaw-table constants and hence the
scheduling of the whole block.

`-mllvm -print-machineinstrs`, `After Instruction Selection`, `bb.12`:

```text
baseline (far = yaw)                 far = pitch
%114 LDRLIT Globals::iPad            %114 LDRLIT Globals::iPad
%115 t2LDRi12 %114                   %115 LDRLIT gHangarCamPitch
%116 LDRLIT gHangarCamPitch          %116 t2LDRi12 %114
%117 LDRLIT gHangarCamYawIPad        %117 LDRLIT gHangarCamYaw
%118 LDRLIT gHangarCamYaw            %118 LDRLIT gHangarCamYawIPad
```

The `t2MOVCCr` operand order is `(YawIPad, Yaw)` in both, so the CMOV is the
same; only the constant *creation order* differs. `AssignTopologicalOrder`
(`SelectionDAG.cpp`) numbers zero-operand nodes in `AllNodes` order, and the
yaw load's live-out status changes the DAG's root set, which changes that
order. Result: the baseline's literal pool is `{Pitch, YawIPad, Yaw}` (matching
the original) while `far = pitch` gives `{Pitch, Yaw, YawIPad}`.

The literal-pool *words* are `target - (addr+4)` (LLVM's ARM assembler does not
align the addend PC), so raw word values are not order-comparable; the
instruction order is. `far = pitch` (43 B / 8 runs) is:

```text
166: ldr r0, pitch
168: ldr r4, Yaw            <- swapped vs original
16a: ldr r3, YawIPad
16c: add r0, pc
16e: ldrb.w r1, [sl]
172: add r4, pc
174: ldr.w r2, [r0, r8, lsl#2]
178: add r3, pc
17a: cmp r1, #0
17c: mov r0, r3
17e: str r3, [sp,#180]
180: str r4, [sp,#176]
182: it eq
184: moveq r0, r4
186: str.w r8, [sp,#184]    <- yaw load sunk below the spill
18a: mov r4, r2             <- pitch copy hoisted
18c: ldr.w r3, [r0, r8, lsl#2]
190: movs r0, #2
...
```

The instruction *set* is identical to the original; only the schedule differs.
So the two remaining constraints are mutually exclusive at source level:

* `far = pitch` gives the original's register shape (yaw -> r3 dead, pitch ->
  r4 live) but flips the DAG order and the schedule (43 B).
* `far = yaw` gives the original's DAG order and prefix byte-for-byte but
  keeps yaw live into the far call (r4 + `mov r3,r4`, 11 B).

### 1.3 Variants measured this wave (all 1008 B unless noted)

`far` is `pitch` unless stated. `fresh` = `(Globals::iPad ? gHangarCamYawIPad :
gHangarCamYaw)[race]`.

| variant | diff B | runs |
|---|---:|---:|
| **baseline (`far = yaw`, volatile-int bit loads)** | **11** | **4** |
| `far = pitch` (`fp`) | 43 | 8 |
| `far = pitch`, `char pad = Globals::iPad` local | 43 | 8 |
| `far = pitch`, ternary spelled `!= 0` / `== 0` / `!iPad` (both sites) | 43 | 8 |
| `far = pitch`, near y = `yawTable[race]` (fresh) | 43 | 8 |
| `far = pitch`, near/far both fresh ternary | 43 | 8 |
| `far = pitch`, near y = `yaw`, far y = fresh | 43 | 8 |
| `far = pitch`, near y = `yawTable[race]`, far y = fresh | 44 | 7 |
| `far = pitch`, volatile-int pitch + plain-float yaw, near y = `yaw`, far y = fresh (`vp_py`) | 44 | 7 |
| `far = pitch`, volatile-int pitch + plain-float yaw, near y = `yaw`, far y = `yawTable[race]` | 676 | 69 (976 B) |
| `far = pitch`, plain-float pitch + volatile-int yaw, far y = fresh | 666 | 87 |
| `far = pitch`, plain-float loads, far y = fresh | 667 | 86 |
| `far = pitch`, plain-int bit loads, far y = fresh | 667 | 86 |
| `far = pitch`, `float &yaw = *(float*)&yawTable[race]`, far y = fresh | 667 | 86 |
| `far = pitch`, `const volatile float &yaw`, far y = fresh | 750 | 39 (1016 B) |
| `far = pitch`, `int &yaw` bit reference, far y = fresh | 724 | 33 |
| `far = pitch`, volatile-int pitch + volatile-float yaw, far y = fresh | 528 | 90 (1016 B) |
| `far = pitch`, far y = `yawTable[race]` / `yaw` / `*(float*)&yawBits` | 580 | 72 (984 B) |
| `far = pitch`, both calls `yawTable[race]` | 580 | 72 (984 B) |
| `far = pitch`, near y = fresh, far y = `yaw` | 580 | 72 (984 B) |
| `far = pitch`, near y = `yaw`, far y = `yawTable[race]` | 580 | 72 (984 B) |
| `far = pitch`, near y = `yaw`, far y = `yaw` (`far = (pitch, yaw)`) | 580 | 72 (984 B) |
| `far = pitch`, plain-float loads, far y = `yaw` | 701 | 79 (984 B) |
| `far = pitch`, plain-float loads, both `yawTable[race]` | 701 | 79 (984 B) |
| `far = pitch`, volatile-int pitch + plain-float yaw, far y = `yaw` | 676 | 69 (976 B) |

Every variant that keeps the DAG order `{Pitch, YawIPad, Yaw}` either keeps
`yaw` live (11 B shape) or re-schedules the block. Every variant that makes
`yaw` dead gives 43-44 B. No source spelling of the ternary changes the IR
(InstCombine canonicalises all of them to `select (icmp eq i8 %iPad, 0), @Yaw,
@YawIPad`), so the DAG order cannot be steered from the ternary spelling.

### 1.4 Note on measurement coupling

`MGame::OnUpdate`'s byte diff depends on the ModStation variant chosen: the
literal-pool words in MGame encode absolute data addresses, and a ModStation
change can shift the data layout enough to move MGame's score (e.g. the
`fp_noyaw` variant made MGame read 754 B / 674 runs while ModStation itself was
1008 B). Always measure both targets with the same `libgof2hdaa.so`.

## 2. MGame::OnUpdate — the `st` anchor is the whole residual

Baseline diff (function-relative; function starts at `0x17c8d8`):

```text
+0x17ba..+0x17ba (1B) orig=00 ours=50
+0x17bc..+0x17e3 (40B) permutation
```

Original (`0x17e088..`):

```text
ldr   r0, [pc,#872]
add   r0, pc
ldr.w r9, [r0]                 ; r9 = &Globals::status
ldr.w r0, [r9]                 ; ccm: reload Status*
blx   getCurrentCampaignMission
cmp   r0, #23
bne   ...
ldr.w r0, [sl]                 ; player
blx   isMining
cmp   r0, #0
bne   ...
ldr.w r0, [r9]                 ; getMission: reload
blx   getMission
blx   isEmpty
cmp   r0, #0
beq   ...
ldr.w r0, [r9]                 ; getPlayingTime: reload
blx   getPlayingTime
ldr.w r2, [r9]                 ; field_0x100: reload
```

Ours (baseline `Status *st = Globals::status;` at the top of the m23 block,
used at the `getPlayingTime` site):

```text
ldr   r0, [pc,#872]
add   r0, pc
ldr.w r9, [r0]                 ; r9 = &Globals::status
ldr.w r5, [r9]                 ; r5 = Status*  (the anchor)
mov   r0, r5                   ; ccm uses the cached pointer
blx   getCurrentCampaignMission
...
ldr.w r0, [r9]                 ; getMission reload
...
mov   r0, r5                   ; getPlayingTime uses the cached pointer
```

The anchor is what forces the long `Globals::status` GOT-base interval into
`r9` (without it the whole function recolours: 11392 B / 5476 diff / 522
runs). But the anchor's `Status*` is live across the calls, so the allocator
puts it in `r5` and IR-level forwarding merges the ccm's `Globals::status`
load into it, producing `ldr r5,[r9]; mov r0,r5` instead of `ldr r0,[r9]`.

### 2.1 Variants measured this wave (all 11440 B unless noted)

| variant | diff B | runs | size |
|---|---:|---:|---:|
| **baseline (`Status *st` at m23 top, used at play)** | **41** | **1** | 11440 |
| no anchor (all sites `Globals::status`) | 5476 | 522 | 11392 |
| `Status **stp = &Globals::status`, `(*stp)->getPlayingTime()` | 5148 | 212 | 11392 |
| `Status *&st = Globals::status`, `st->getPlayingTime()` | 5148 | 212 | 11392 |
| `Status **stp`, all sites `(*stp)->...` | 5148 | 212 | 11392 |
| `Status *volatile st = Globals::status`, all sites `Globals::status` | 6092 | 308 | 11340 |
| `Status *volatile st = Globals::status`, play uses `st` | 6076 | 317 | 11340 |
| ccm uses `(*(Status *volatile *) &Globals::status)` | 853 | 36 | 11440 |
| ccm volatile + play volatile | 5148 | 212 | 11392 |
| `Status *st` anchor before the `elapsedTime > 5000` if | 8721 | 533 | 11392 |
| `Status *st` anchor before the `m_nScriptTime` if | 8724 | 534 | 11392 |
| `Status *st` anchor after the ccm | 5446 | 537 | 11400 |
| `Status *st` anchor after `getMission` | 5434 | 551 | 11396 |
| `long long lastVisit` before the if | 5040 | 270 | 11432 |
| `this->camera != nullptr` first/last in the m23 condition | 5063/5036 | 272/271 | 11448 |

The 16-way four-site sweep (`ccm`, `mission`, `play`, `field` each either
`Globals::status` = G or `st` = S) from wave 117 still holds: only
`play = S`, `mission = G`, `field = G` gives 41 B / 1 run (the ccm is
CSE-merged with the anchor, so G and S are equivalent there). Every other
combination is >= 4966 B.

### 2.2 Why this is a hard local optimum

The original has no anchor at all: it simply reloads `Globals::status` at each
of the four sites and the compiler CSEs only the GOT base (`&Globals::status`)
into `r9`. Our no-anchor build splits the GOT-base interval and gives the
first piece `r5`, which recolours the entire function (11392 B, 128 B smaller
frame). The `st` anchor is a substitute for the original's `r5` occupancy
(the original's `r5` holds `&Level::initStreamOutPosition`, loaded at
`0x17e034` and stored at `0x17e044`, i.e. dead before the m23 block but still
interfering with the long GOT-base interval). Every attempt to break the
anchor's CSE with the ccm load either adds a load (`853 B / 36 runs`) or
recolours the whole function. `Status **` / `Status *&` anchors are
rematerialised away (identical to no anchor).

## 3. Structural state (unchanged by this wave)

```text
sections            27 (exact; same count both binaries)
file size           2,187,328 B (exact, both)
.text               0x1337c8 @ 0x69440   (size/base exact)
.ARM.exidx          0x6f70   @ 0x19cc08  (size/base exact)
.ARM.extab          0x26294 ours / 0x26298 orig   (4 B short, pre-existing)
.rodata             0x41c27 ours / 0x41b67 orig   (+0xC0 rank table)
.got                0x430c   @ 0x20fcf4  (size/base exact)
```

Gate after the sweep (source restored to baseline): **RATCHET PASS**,
byte_exact 4501 / linked_exact 4501, size-exact 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0, stubs 0, missing 0, extra 0.
