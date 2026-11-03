# StarMap::draw / MGame::OnUpdate / MenuTouchWindow::OnTouchBegin / ModStation ctor — wave 91 (w4119)

Status: analysis pass + one landed improvement (ModStation ctor fence ordering).
Worktree `wt/w4119`, branch `wave91-w4119`, base master `a4557300`.
Every number below was measured in this worktree against
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so` with the tree's
NDK r18b clang 7.0.2 and the official `tools/verify/verify.py` (`autojunk=False`).
Scratch drivers live in `/dev/shm/w4119` (`fdiff.py`, `harness*.py`).

Build note: this is a git worktree, so the default `../_work` paths do not resolve.
Configure with
`-DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
-DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs`.
`FN=... verify-fn` is broken here (`--show` receives no argument); call
`tools/verify/verify.py --show <mangled>` directly.

## Gate (before -> after this pass)

| metric | before | after |
|---|---:|---:|
| byte_exact | 4498 / 4524 | **4498 / 4524** |
| linked_exact | 4498 / 4524 | **4498 / 4524** |
| size-exact | 4524 / 4524 | 4524 / 4524 |
| stub_zero_size / missing / wrong_type / extra | 0 / 0 / 0 / 0 | 0 / 0 / 0 / 0 |
| data_exact | 843 / 843 | 843 / 843 |
| imports | 207 | 207 |
| lint | CLEAN | CLEAN |
| drift | 0 | 0 |

`.text 0x1337c8`, `.rodata 0x41b67`, `.ARM.exidx 0x6f70` exact, `.ARM.extab 0x26290` (-8).
The only landed source change is the ModStation ctor fence move; it leaves every gate
count unchanged and improves the official per-function match 98.4 -> 98.6 (raw diff
42 -> 39 bytes), so no baseline update is needed.

## 1. StarMap::draw — 15 bytes, r8/fp role swap + hoisted `this->systems`

Current source (`Array<SolarSystem *> &systems = *this->systems;` bound before the
second loop, `i < systems.size()`): **2644 B, 15 differing bytes / 10 runs**.
Function @ `0xc90f4`.

Raw runs (all 15 bytes):

```
+0x247..+0x248  orig=08    ours=0b      ; c933b  add.w r8,sp,#136   vs  add.w fp,sp,#136
+0x24c..+0x24d  orig=41    ours=59      ; c9341  mov r1,r8           vs  mov r1,fp
+0x26c..+0x26d  orig=dd    ours=d4      ; c9361  ldr.w fp,[sp,#16]  vs  ldr.w r8,[r4,#84]
+0x26e..+0x270  orig=10b0  ours=5480    ; c9362  [sp,#16]           vs  [r4,#84]
+0x2b0..+0x2b1  orig=41    ours=59      ; c93b1  mov r1,r8           vs  mov r1,fp
+0x2be..+0x2c0  orig=5846  ours=0498    ; c93b2  mov r0,fp           vs  ldr r0,[sp,#16]
+0x3e2..+0x3e4  orig=606d  ours=d8f8    ; c94d6  ldr r0,[r4,#84]     vs  ldr.w r0,[r8]
+0x3e5..+0x3e6  orig=68    ours=00      ; c94d8  ldr r0,[r0,#0]      vs  (folded)
+0x8d0..+0x8d2  orig=5846  ours=0498    ; animated-route block, same mov/ldr swap
+0x972..+0x974  orig=5846  ours=0498    ; animated-route block, same mov/ldr swap
```

Original (route loop preheader/body):

```
c9338  add.w  r8, sp, #136        ; r8 = &Vector temp (sp+136)
c9360  ldr.w  fp, [sp, #16]       ; fp = &this->scratchVector (this+0x78)
c93a4  mov    r1, r8              ; Vector::operator= source
c93b2  mov    r0, fp              ; Vector::operator= destination
c94d6  ldr    r0, [r4, #84]       ; this->systems   (re-read every iteration)
c94d8  ldr    r0, [r0, #0]        ; ->size()
```

Ours:

```
c9338  add.w  fp, sp, #136        ; fp = &Vector temp
c9360  ldr.w  r8, [r4, #84]       ; r8 = this->systems (hoisted)
c93a4  mov    r1, fp
c93b2  ldr    r0, [sp, #16]       ; &scratchVector spilled to stack
c94d6  ldr.w  r0, [r8]            ; ->size()  (hoisted, no re-read)
```

The reference is a loop-invariant SSA value, so clang keeps `this->systems` in r8; that
displaces the `Vector` temp into fp and `&this->scratchVector` onto the stack. The
original re-reads `this->systems` and keeps `&this->scratchVector` in fp.

### 1.1 Direct form and the 8-byte deficit

`for (uint32_t i = 0; i < this->systems->size(); i++)` restores the re-read and puts the
`Vector` temp back in r8, but the function shrinks to **2636 B (-8)** with **1656 diff
bytes** (102 runs). The direct form's loop header is byte-identical to the original:

```
c94ce  ldr    r0, [r4, #84]       ; this->systems
c94d0  ldr    r0, [r0, #0]        ; ->size()
c94d2  cmp    r6, r0
c94d4  bcc.w  c9376
```

The 8 missing bytes are the hoisted `Globals::Canvas` reload, exactly as w4020 said:

```
original:  c9454 ldr.w sl,[sp,#20]   ; Canvas value (cached in [sp,#20] at entry)
           c948a mov.w sl,#0         ; restore the 64-bit zero constant for the sbc
direct:    (sl = Canvas hoisted before the loop; fp = zero constant;
            c9454/c948a collapse to ldr r0,[sl] / vmla)
```

Both forms keep `vdiv.f32 s20, s0, s18` with `s18 = 100.0f` (constant at `c93f0 =
0x42c80000`); the direct form does **not** constant-fold `/100.0f` into `*0.01f`. The
8-byte deficit is entirely the Canvas/zero register assignment, not the pulse arithmetic.

The three loop-invariant values (`Vector` temp, `&scratchVector`, zero/Canvas) compete
for r8/fp/sl:

| form | r8 | fp | sl | scratchVector | Canvas |
|---|---|---|---|---|---|
| original | temp | &scratchVector | zero | (in fp) | [sp,#20] |
| current (ref bound) | systems | temp | zero | [sp,#16] | [sp,#20] |
| direct | temp | zero | Canvas | [sp,#16] | (in sl) |

The original is the only one that keeps `&scratchVector` in fp while re-reading
`this->systems`.

### 1.2 Variants measured

All with the official verifier; `size` is ours, `diff` is raw differing bytes.

| variant | size | diff |
|---|---:|---:|
| current (`Array& systems = *this->systems`, `systems.size()`) | 2644 | **15** |
| `Array<SolarSystem *> *const systems = this->systems; systems->size()` | 2644 | 15 |
| direct (`this->systems->size()`) | 2636 | 1656 |
| direct + `this->systems->count` / `(*this->systems).size()` / `.count` | 2636 | 1656 |
| direct + `Vector &sv = this->scratchVector;` (loop top / before loop / spans both loops) | 2636 | 1656 |
| direct + `Vector *const sv = &this->scratchVector;` (all reads through `sv`) | 2636 | 1656 |
| direct + `sv`/`sv2` used for the hidden check and pulse reads | 2636 | 1656 |
| direct + named `Vector tmp` / `Vector((...))` / direct `Vector` copy | 2636 | 1656 |
| direct + `long long t = getPlayingTime();` / `long long t = 101 - ...` / `101LL` | 2636 | 1656 |
| direct + `Array<...> *volatile systems = this->systems;` | 2640 | 1826 |
| direct + `PaintCanvas *canvas = Globals::Canvas;` inside the pulse `if` | 2636 | 1535 |
| direct + `PaintCanvas *canvas` at loop top | 2652 | 2199 |
| direct + `(*this->systems, this->systems)->size()` | 2636 | 1656 |
| `Vector &sv` bound before the first loop, used through the animated-route block | 2644 | 15 |
| `Array<SolarSystem *> &systems` (base) + `sv` spanning both loops | 2644 | 15 |

No variant reaches 2644/0. A `Vector &` local is fully rematerializable
(`ldr r0,[sp,#16]`) and does not force fp; the allocator still hoists Canvas into sl.

### 1.3 Flag sweep (re-confirmed)

`-regalloc-csr-first-time-cost` 0..65536 on StarMap: all 2636/1656 (direct) — no effect.
Earlier passes covered `-hoist-cheap-insts`, `-disable-postra-machine-licm`,
`-disable-machine-licm`, `-disable-licm-promotion`, `-avoid-speculation`,
`-licm-max-num-uses-traversed`, `-optimize-regalloc`,
`-spp-rematerialization-threshold`, `-enable-deferred-spilling`,
`-sink-insts-to-avoid-spills`, `-regalloc=basic/pbqp` (see the w3840/w4020 notes).

### 1.4 What is proved / what remains

Proved: the 15 bytes are exactly the r8/fp role swap plus the hoisted `this->systems`
load; the direct form's -8 is the hoisted `Globals::Canvas` reload (`ldr.w sl,[sp,#20]`
+ `mov.w sl,#0`), and the pulse keeps a real `vdiv.f32` in both forms. The allocator
prefers Canvas (2 uses/iteration) over `&scratchVector` (1 use) once sl is free.

Remaining: a source shape that makes the allocator keep `&this->scratchVector` in fp
while re-reading `this->systems`. A `Vector &` local is not enough (rematerialized).

## 2. MGame::OnUpdate — 41 bytes, cached `Status*` in r5

Current source (`Status *st = Globals::status;` used at `getPlayingTime`):
**11440 B, 41 differing bytes / 1 run** (`+0x17bb..+0x17e4`). Function @ `0x17c8d8`.

Original (`r9 = &Globals::status`, loaded at `17e08c`):

```
17e08c  ldr.w r9, [r0]        ; r9 = &Globals::status
17e090  ldr.w r0, [r9]        ; Globals::status
17e094  blx   Status::getCurrentCampaignMission
17e0a8  ldr.w r0, [r9]        ; getMission
17e0b8  ldr.w r0, [r9]        ; getPlayingTime
17e0c0  ldr.w r2, [r9]        ; field_0x100
```

Ours (`Status *st = Globals::status;` used at getPlayingTime):

```
17e08c  ldr.w r9, [r0]
17e090  ldr.w r5, [r9]        ; st
17e094  mov   r0, r5          ; CSE'd with the first use
17e096  blx   getCurrentCampaignMission
17e0aa  ldr.w r0, [r9]        ; getMission (fresh)
17e0ba  mov   r0, r5          ; getPlayingTime (st)
17e0c0  ldr.w r2, [r9]        ; field_0x100 (fresh)
```

`ldr r5,[r9]` + `mov r0,r5` + `mov r0,r5` = 4+2+2 = 8 B; original
`ldr r0,[r9]` + `ldr r0,[r9]` = 4+4 = 8 B. Size is exactly neutral.

### 2.1 Why removing `st` fails

All `Globals::status->` gives **11392 B (-48)** and **5148 diff bytes**. The allocator
then puts `&Globals::status` in **r5** instead of r9 and spills it to `[sp,#36]` before
the `Globals::gameText` use at `17e122`; the whole function's allocation shifts. The
48-byte deficit is the wide `ldr.w rX,[r9]` encodings (4 B each) that the original pays
throughout the region; with r5 they become 2-byte `ldr rX,[r5]`.

### 2.2 r5 liveness in the original

r5 uses in the original: `17e034` (`ldr r5,[r4,#0]`, the
`Globals::switch_to_target_setting` address), `17e044` (`strb r0,[r5,#0]`), then the
next r5 use is `17e12a` (`ldr r5,[r0,#0]`, `Globals::gameText`). **r5 is not referenced
anywhere between `17e044` and `17e12a`.** The `&this->camera` value that lives in r5
from `17da24` (`ldr.w r0,[r5,#244]!` = `this->camera`, r5 = this+0xf4) is last used at
`17db2e` and overwritten at `17dce4`, long before the condition. So r5 is free at
`17e08c`; the original's choice of r9 is a whole-function greedy-allocator decision
(the `&Globals::status` interval spans `17e08c..17e846`, with a spill at `17e31c`), not
a forced live value. The `st` local is a reconstruction hack that reproduces r9 by
occupying r5 at the start.

### 2.3 Variants measured

| variant | size | diff |
|---|---:|---:|
| current (`Status *st` used at getPlayingTime) | 11440 | **41** |
| `Status *const st` used at getPlayingTime | 11440 | 41 |
| `Status &st = *Globals::status;` used at getPlayingTime | 11440 | 41 |
| `st` used at getCurrentCampaignMission + getPlayingTime | 11440 | 41 |
| no `st` (all `Globals::status->`) | 11392 | 5148 |
| `Status *&st` / `Status **st` (`(*st)->...`) | 11392 | 5196 |
| `st` used at getMission + getPlayingTime | 11436 | 4986 |
| `st` used at getPlayingTime + field_0x100 | 11436 | 4985 |
| `st` used at getCurrentCampaignMission only, getPlayingTime direct | 11392 | 5196 |
| `Status **st = &Globals::status` used at all four sites | 11392 | 5196 |
| `TargetFollowCamera *&camera = this->camera;` + no `st` | 11392 | 5982 |

### 2.4 What is proved / what remains

Proved: the 41 bytes are exactly the `ldr r5,[r9]; mov r0,r5` / `mov r0,r5` caching of
`Status*`; the `st` local is required for the 11440 B size because without it the
allocator picks r5 for `&Globals::status` and spills it before `17e122`. r5 is not
referenced in the condition region in the original.

Remaining: a source witness for the allocation that puts `&Globals::status` in r9 while
leaving r5 free. Every direct spelling either removes the size (11392) or keeps the
`mov` (41). The original almost certainly has no `Status *st` local at all; the r9
choice is a property of the whole-function IR that our reconstruction does not
reproduce.

## 3. MenuTouchWindow::OnTouchBegin — 71 bytes, two allocator tie-breaks

Current source (`Layout &layout = *Globals::layout; int top = Globals::layout->field_0x20_top;`
plus a direct second `if`): **1896 B, 71 differing bytes / 8 runs**. Function @ `0x129bd4`.

Raw runs:

```
+0x401..+0x402  orig=c1    ours=41    ; case 8: ip vs r4 (listEntryWidth)
+0x404..+0x405  orig=63    ours=23
+0x60c..+0x63a  orig=...   ours=...   ; case 15: layout-address / top scheduling (46 B)
+0x63c..+0x646  orig=...   ours=...   ; case 15 (10 B)
+0x64a..+0x654  orig=...   ours=...   ; case 15 (10 B)
+0x672..+0x673  orig=95    ours=94    ; r5 vs r4 (buttonInsetX)
+0x678..+0x679  orig=28    ours=20
+0x748..+0x749  orig=0c    ours=0e
```

Case 15 original vs ours (layout pointer / `top`):

```
original:  12a1f0 ldr r2,[r4,#0]
           12a1f6 ldr r5,[r0,#0]        ; layout->field_0
           12a1fc ldr r2,[r2,#40]       ; layout->buttonInsetX
           12a1fe subs r5,r5,r2
           ...
           12a212 ldr r2,[sp,#16]       ; layout pointer
           12a218 ldr r2,[r2,#0]        ; layout->field_0
           12a21e ldr r5,[r2,#12]       ; field_0xc_leftMargin
           12a220 ldr r6,[r2,#32]       ; field_0x20_top (read after GetImage2DHeight)
ours:      12a1e0 ldr r6,[r4,#0]
           12a1f2 ldr r4,[r0,#0]
           12a1f4 ldr r0,[r6,#32]       ; top hoisted
           12a1fa str r0,[sp,#12]
           12a200 ldr r5,[r6,#40]
           12a202 subs r4,r4,r5
           ...
           12a222 ldr r2,[sp,#12]       ; saved top
           12a224 ldr r1,[r6,#12]
```

The original keeps `&Globals::layout` in `[sp,#16]` and `&Globals::Canvas` in fp; ours
keeps the layout value in r6 and `top` at `[sp,#12]`.

### 3.1 Variants measured

| variant | size | diff |
|---|---:|---:|
| current (`Layout &layout` + hoisted `top`) | 1896 | **71** |
| `Layout *layout = Globals::layout;` + hoisted `top` | 1896 | 71 |
| `Layout &layout` + `int top = layout.field_0x20_top;` | 1896 | 71 |
| faithful direct `Globals::layout->...` (no locals) | 1892 | 369 |
| no `top` local (`layout.field_0x20_top` in the expression) | 1892 | 312 |
| `Layout *layout` + no `top` | 1892 | 312 |

The faithful direct shape is 4 B short (1892 vs 1896); the `top` local is needed for the
size but costs the scheduler ordering. No variant reaches 1896/0.

### 3.2 What is proved / what remains

Proved: the residual is two allocator tie-breaks — case 8 (`ip` vs `r4` for
`listEntryWidth`) and case 15 (`fp`/`r6`/`[sp,#12]` vs `fp`/`r4`/`[sp,#16]` for the
layout address and `top`). The direct shape drops 4 bytes because the layout pointer is
reloaded from `[sp,#16]` instead of being kept in r6.

Remaining: a source shape that keeps the 1896 size while letting the layout pointer
stay in `[sp,#16]` and `top` be read after the `GetImage2DHeight` call.

## 4. ModStation::ModStation (C1/C2) — fence ordering, 42 -> 39 bytes

Both `_ZN10ModStationC1Ev` / `C2Ev` are 1008 B @ `0xd52d0`. The ctor uses
`__atomic_signal_fence(__ATOMIC_SEQ_CST)` plus `*(const volatile int *)` reads of
`gHangarCamPitch` / `gHangarCamYaw` to reproduce the original's load ordering.

### 4.1 Landed change

Move the fence **before** the two table-pointer declarations:

```cpp
    // before
    const float *pitchTable = gHangarCamPitch;
    const float *yawTable = Globals::iPad ? gHangarCamYawIPad : gHangarCamYaw;
    __atomic_signal_fence(__ATOMIC_SEQ_CST);
    int pitchBits = *(const volatile int *) &pitchTable[race];

    // after
    __atomic_signal_fence(__ATOMIC_SEQ_CST);
    const float *pitchTable = gHangarCamPitch;
    const float *yawTable = Globals::iPad ? gHangarCamYawIPad : gHangarCamYaw;
    int pitchBits = *(const volatile int *) &pitchTable[race];
```

Measured effect (both C1 and C2):

| variant | size | raw diff | runs | official pct |
|---|---:|---:|---:|---:|
| before (fence after tables) | 1008 | 42 | 7 | 98.4 |
| **after (fence before tables)** | 1008 | **39** | 8 | **98.6** |

Before runs:

```
+0x16a..+0x17a (16B)  +0x186..+0x187 (1B)  +0x188..+0x18d (5B)
+0x18e..+0x19f (17B)  +0x3e0..+0x3e1 (1B)  +0x3e4..+0x3e5 (1B)
+0x3e8..+0x3e9 (1B)
```

After runs:

```
+0x169..+0x16a (1B)   +0x16b..+0x16c (1B)  +0x16e..+0x17a (12B)
+0x186..+0x187 (1B)   +0x188..+0x18d (5B)  +0x18e..+0x19f (17B)
+0x3e4..+0x3e5 (1B)   +0x3e8..+0x3e9 (1B)
```

The residual is post-RA scheduler ordering of the yaw load vs the race spill and the
`pitchBits` save. Original vs ours around `0xd5436`:

```
original: d5436 ldr r0,[pc]   d5438 ldr r3,[pc]   d543a ldr r4,[pc]
          d543c add r0,pc     d543e add r3,pc     d5440 ldrb r1,[sl]
          d5444 add r4,pc     d5446 ldr r2,[r0,r8,lsl#2]
          d544a mov r0,r3     d544c cmp r1,#0     d544e str r3,[sp,#180]
          d5450 str r4,[sp,#176] d5452 it eq      d5454 moveq r0,r4
          d5456 ldr r3,[r0,r8,lsl#2] d545a movs r0,#2 d545c str r8,[sp,#184]
          d5460 movs r1,#0    d5462 add r8,sp,#256 d5466 strd r1,r0,[sp]
          d546a add r1,sp,#320 d546c mov r0,r8    d546e mov r4,r2
          d5470 blx MatrixSetRotation
ours:     ... same instructions, but `str r8,[sp,#184]` is at d5456 (before
          `mov r4,r2` / `ldr r3`) and `mov r4,r2` is at d545a (after).
```

### 4.2 Scheduler flag sweep (neutral)

Per-TU post-RA scheduler flags on ModStation all stay **1008 B / 42 bytes**:
`-disable-post-ra`, `-misched-postra`, `-enable-post-misched=false`,
`-misched=ilpmin/ilpmax/converge`, `-break-anti-dependencies=all/none`,
`-disable-sched-*`, `-max-sched-reorder=0/1000`, `-misched-bottomup/topdown`,
`-misched-cluster`, `-disable-2addr-hack`. The residual is not flag-controllable.

### 4.3 Other variants (all worse or equal)

`fence_after_pitch` 44 B; no fence 43 B; `yaw_first` 44 B; `if/else` table select 42 B;
`bool ipad` local 42 B; both table pointers first 42 B; negated ternary 39 B (equal to
the landed shape). The landed shape is the local optimum found.

## 5. Conclusion

- **StarMap::draw**: 15-byte r8/fp role swap + hoisted `this->systems`; direct form
  re-reads correctly but is 8 B short because Canvas is hoisted into sl. A `Vector &`
  local is rematerialized and does not force fp. No source shape reaches 2644/0.
- **MGame::OnUpdate**: 41-byte `Status*` cache in r5. The `st` local is needed for the
  11440 B size; every direct spelling moves `&Globals::status` to r5 and spills. r5 is
  dead in the original's condition region, so r9 is a whole-function allocator choice.
- **MenuTouchWindow::OnTouchBegin**: 71 bytes, two allocator tie-breaks (case 8
  `ip`/`r4`; case 15 layout pointer / `top`). Direct shape is 1892/365; the `top` local
  restores the 1896 size but costs the scheduler ordering.
- **ModStation ctor**: landed the fence-before-tables ordering. Raw diff 42 -> 39 bytes,
  official pct 98.4 -> 98.6, gate unchanged (byte 4498, linked 4498, data 843/843,
  imports 207, lint CLEAN, drift 0). Residual is post-RA scheduler ordering of the yaw
  load vs the race spill and `pitchBits` save; no scheduler flag changes it.

All four targets are source-shape / register-allocation residuals, not missing code,
wrong constants, wrong callees, or flag problems. The single most important remaining
root cause for StarMap and MGame is the same: a loop-invariant value that the original
keeps in a specific callee-saved register (fp for `&StarMap::scratchVector`; r9 for
`&Globals::status`) while our reconstruction reaches only indirectly, and every direct
spelling makes clang choose a different register.
