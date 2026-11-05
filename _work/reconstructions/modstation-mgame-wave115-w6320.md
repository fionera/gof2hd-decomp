# ModStation ctor + MGame::OnUpdate — wave 115 (w6320): redundant fence removed, both residuals re-swept

Status: **landed a byte-identical source cleanup** (removed the redundant
`__atomic_signal_fence` in `ModStation::ModStation`) and **records the new
measurements**. Worktree `wt/w6320`, branch `wave115-w6320`, base `b847d5d46`.
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`).

Targets:

* `_ZN10ModStationC1Ev` / `C2Ev`, 1008 B — baseline **11 diff bytes / 4 runs,
  99.2 %** (wave-112 shape, unchanged).
* `_ZN5MGame8OnUpdateEv`, 11440 B — baseline **41 diff bytes / 1 run, 99.9 %**
  (wave-112 shape, unchanged).

Nothing beat either baseline. The only landed change is the fence removal: the
two links differ **only in the 20-byte `.note.gnu.build-id` descriptor** (the
build-id is not deterministic between links); every other section, including
the whole `ModStation::ModStation` body, is byte-identical (`cmp -l` reports
exactly 20 differing bytes at file offsets 477..496, inside `.note.gnu.build-id`
at `0x1cc`). The gate counts are unchanged. The wave-112 note claimed the fence
was load-bearing, but that was the *hand-IR* two-fence shape; at the source
level the volatile loads already pin the order and the fence is dead.

## 1. ModStation — the residual is the post-RA reuse of the just-killed Yaw pointer

Baseline (wave-112 shape), 11 B / 4 runs:

```text
        orig                                    ours
d5456   ldr.w r3,[r0,r8,lsl#2]   ; yaw      d5456  str.w r8,[sp,#184]
d545a   movs  r0,#2                         d545a  ldr.w r4,[r0,r8,lsl#2] ; yaw
d545c   str.w r8,[sp,#184]                  d545e  movs  r0,#2
...                                         ...
d546c   mov   r0,r8                         d546c  mov   r3,r4   ; extra copy
d546e   mov   r4,r2                         d546e  mov   r0,r8
```

The original keeps `pitch` in `r4` (`mov r4,r2`) across the near call and loads
`yaw` straight into argument register `r3`. Ours keeps `yaw` in `r4` (the
just-killed `Yaw` pointer register) and copies it to `r3`. The far call is
therefore the only source-level lever: the original's far call is
`(pitch, freshYaw)`, ours is `(yaw, freshYaw)`.

### 1.1 far=pitch is register-correct but schedule-wrong

Changing the far call to `pitch` gives **42 B / 7 runs** (98.6 %), size 1008 B.
The register allocation becomes exactly the original's (`r2=pitch`, `r3=yaw`,
`r4=pitch` saved with `mov r4,r2`, no `mov r3,r4`), but the pre-RA scheduler
sinks the `Yaw` LDRLIT below the `ldrb` because `yaw` is now dead after the near
call:

```text
        orig                                    far=pitch
d5436   ldr r0,[pc]  ; Pitch                 ldr r0,[pc]  ; Pitch
d5438   ldr r3,[pc]  ; YawIPad                ldr r3,[pc]  ; YawIPad
d543a   ldr r4,[pc]  ; Yaw                    add r0,pc
d543c   add r0,pc                            ldrb r1,[sl]
d543e   add r3,pc                            ldr r4,[pc]  ; Yaw
d5440   ldrb r1,[sl]                         add r3,pc
d5444   add r4,pc                            ldr r2,[r0,r8,lsl#2] ; pitch
d5446   ldr r2,[r0,r8,lsl#2] ; pitch         add r4,pc
...
d5456   ldr r3,[r0,r8,lsl#2] ; yaw           str r8,[sp,#184]
d545a   movs r0,#2                           mov r4,r2
d545c   str r8,[sp,#184]                     ldr r3,[r0,r8,lsl#2] ; yaw
```

The IR is otherwise identical: the front-end bitcast order is still
`%yaw` before `%pitch` (verified with `-emit-llvm -S`); only the far call's
first argument changes from `%107` (yaw) to `%108` (pitch). InstCombine does
**not** revert the bitcasts. The DAG scheduler sinks the `Yaw` LDRLIT because
the yaw value is no longer live-out.

### 1.2 Variants measured this wave (all 1008 B unless noted)

Baseline (far=yaw): **11 B / 4 runs / 99.2 %**.

| variant | diff B | runs | size |
|---|---:|---:|---:|
| far=pitch | 42 | 7 | 1008 |
| far=pitch + fence after `yawBits` | 42 | 7 | 1008 |
| far=pitch + fence between pitch/yaw loads | 42 | 7 | 1008 |
| far=pitch + fence after `float yaw` | 42 | 7 | 1008 |
| far=pitch + fence after near call | 42 | 7 | 1008 |
| far=pitch, `float pitch` declared first | 42 | 7 | 1008 |
| far=pitch, `const float yaw` | 42 | 7 | 1008 |
| far=pitch, `farPitch` copy (before/after near) | 42 | 7 | 1008 |
| far=pitch, `(float) pitch` | 42 | 7 | 1008 |
| far=pitch, `yawBits` non-volatile | 43 | 7 | 1008 |
| far=pitch, `pitchBits` non-volatile | 629 | 81 | 1008 |
| far=pitch, `__builtin_assume(yaw != 12345)` | 42 | 7 | 1008 |
| far=pitch, `__builtin_assume(yaw == yaw)` | 42 | 7 | 1008 |
| far=pitch, `__builtin_assume(yaw < 1e30)` | 42 | 7 | 1008 |
| far=pitch, `__builtin_assume(yawBits != 0x7f800000)` | 42 | 7 | 1008 |
| far=pitch, `if (yaw == 12345) {}` | 42 | 7 | 1008 |
| far=pitch, `yaw = yaw;` | 42 | 7 | 1008 |
| far=pitch, `(void)(yaw * 1.0f)` | 42 | 7 | 1008 |
| far=pitch, `float yawCopy = yaw;` (dbg.value only) | 42 | 7 | 1008 |
| far=pitch, `yaw` order arg `2 + (yaw==12345?0:0)` | 42 | 7 | 1008 |
| far=pitch, far `z = yaw * 0.0f` | 755 | 74 | 1028 |
| far=pitch, far `z = yaw` | 728 | 89 | 1016 |
| far=pitch, far `z = pitch` | 537 | 94 | 1008 |
| far=pitch, far `z = yaw - yaw` | 746 | 81 | 1024 |
| far=pitch, far y = `yaw` (reuse) | 580 | 72 | 984 |
| far=pitch, far y = `*(float*)&yawBits` | 580 | 72 | 984 |
| far=pitch, far y via `yawTable[race]` | 580 | 72 | 984 |
| far=pitch, `volatile float yaw` | 551 | 37 | 1008 |
| far=pitch, `const volatile float yaw` | 551 | 37 | 1008 |
| far=pitch, `volatile float pitch` | 676 | 77 | 1008 |
| far=pitch, `volatile yawKeep = yaw` | 630 | 98 | 992 |
| far=pitch, `float yawKeep = yaw` | 580 | 72 | 984 |
| far=pitch, `volatile int yawKeep = yawBits` | 561 | 52 | 1008 |
| far=pitch, near args `(yaw, pitch)` | 579 | 38 | 1000 |
| far=pitch, near `(pitch, 0, yaw)` | 579 | 38 | 1000 |
| far=pitch, `volatile yawTable` | 698 | 42 | 992 |
| far=pitch, no `pitchBits` local (inline pitch) | 671 | 84 | 1008 |
| no locals, direct `gHangarCamPitch[race]` both calls | 667 | 86 | 1008 |
| invert near ternary | 13 | 6 | 1008 |
| invert far ternary | 11 | 4 | 1008 (tie) |
| invert both ternaries | 13 | 6 | 1008 |
| `yawTable` before `pitchTable` | 16 | 7 | 1008 |
| `yawBits` before `pitchBits` | 694 | 92 | 1000 |
| `pitchTable` after fence | 16 | 7 | 1008 |
| **no fence** | **11** | **4** | **1008 (identical .so)** |
| `int race` before `Matrix nearKey` | 11 | 4 | 1008 (tie) |
| `Matrix farKey` declared early | 924 | 32 | 968 |

Every shape that keeps `yaw` live into the far call is 11 B (the baseline) or
worse; every far=pitch shape is >= 42 B. The 11 B / 4 shape is a local optimum.

## 2. MGame::OnUpdate — the address register (r9 vs r5) is the whole residual

Baseline: **41 B / 1 run / 11440 B / 99.9 %** at `+0x17bb..+0x17e3`
(absolute `0x17e093..0x17e0bb`). The whole residual is the mission-23
idle-contact condition.

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

The `st` anchor is load-bearing for the *address register*: without it the
address is an `RS_Split` range with `Prio = Size`, it is dequeued after the
global ranges, and the first free register `r5` wins. The whole function then
recolors (11392 B, 5148 B / 212 runs). The anchor is a multi-MBB global range
that takes `r5` first, forcing the address to `r9`. But because the anchor is
loaded *before* the ccm and no call intervenes, machine CSE merges it with the
ccm's load, producing the `mov r0,r5`.

Every attempt to break the CSE while keeping the anchor either recolors the
whole function or adds an instruction:

* `Status *&st = Globals::status;` (address anchor) gives the **correct m23
  block** (`ldr r5,[r0]; ldr r0,[r5]` at each site) but the address is in `r5`,
  and the function is 11392 B / 5148 B / 212 runs.
* A `__atomic_signal_fence` after `Status *st` breaks the CSE (`ldr r0,[r9]` at
  the ccm) but leaves an extra `ldr r5,[r9]`, giving 853 B / 36 runs.
* Using `st` at the field load instead of getPlayingTime makes the field
  `ldrd r2,r3,[r5,#256]` (one instruction shorter), giving 11436 B / 4993 B /
  301 runs.
* Every other `st` placement (`ccm`, `getMission`, `getPlayingTime`, all four)
  is >= 41 B or recolors.

### 2.2 Variants measured this wave

Baseline: **41 B / 1 run / 11440 B / 99.9 %**.

| variant | size | diff B | runs |
|---|---:|---:|---:|
| **baseline (`st` at getPlayingTime)** | **11440** | **41** | **1** |
| `st` at ccm only | 11440 | 5195 | 213 |
| `st` at getMission | 11440 | 5172 | 241 |
| `st` at field only | 11436 | 4993 | 301 |
| `st` at ccm + getPlayingTime | 11440 | 41 | 1 |
| `st` assigned after ccm (comma) | 11392 | 5145 | 214 |
| `st` assigned after isMining (comma) | 11392 | 5145 | 214 |
| `st` assigned after isEmpty (comma) | 11392 | 5148 | 212 |
| `st` assigned at field (`(st=Globals::status)->field`) | 11392 | 5148 | 212 |
| no `st` | 11392 | 5148 | 212 |
| `Status *&st = Globals::status` (address) | 11392 | 5148 | 212 |
| `Status **stp = &Globals::status` | 11392 | 5148 | 212 |
| `Status *const &st` | 11392 | 5148 | 212 |
| `Status *volatile &st` | 11392 | 5148 | 212 |
| `Status &st = *Globals::status` | 11440 | 41 | 1 |
| `Status *const st` | 11440 | 41 | 1 |
| `st` via `reinterpret_cast` | 11440 | 41 | 1 |
| `st` + fence before | 11440 | 41 | 1 |
| `st` + fence after | 11440 | 853 | 36 |
| `PlayerEgo *p = this->player` anchor (p at isMining) | 11440 | 861 | 37 |
| `PlayerEgo *p` anchor, no `st` | 11396 | 5124 | 242 |
| `LevelScript *ls` anchor | 11456 | 6155 | 357 |
| `float elapsed` anchor | 11440 | 41 | 1 |
| ref at OnUpdate top | 11392 | 5148 | 212 |
| value `st` at OnUpdate top | 11372 | 10822 | 418 |

Nothing beats 41. The best non-baseline shape by run structure is the
`PlayerEgo` anchor (861 B / 37 runs): the address is in `r9` and the m23 block
is the original's, but the anchor adds `ldr r4,[sl]` and changes the isMining
call to `mov r0,r4`, shifting the block by 4 B.

## 3. Structural state (unchanged)

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

## 4. Gate

```text
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.92
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS      (goal_lint CLEAN, drift 0)
```

Before -> after: byte 4500 -> 4500, linked 4500 -> 4500. ModStation 11 B / 4
runs / 1008 B; MGame::OnUpdate 41 B / 1 run / 11440 B. The landed fence removal
changes only the `.note.gnu.build-id` descriptor.

## 5. Scratch (not committed)

`/dev/shm/w6320/` — `measure.sh`, `bytediff.py`, `sweep*.py`, `msweep*.py`,
`base.ll`/`fp.ll` (`-emit-llvm -S` dumps), `show.txt`, `mgame_ref_show.txt`.

## 6. References

* `_work/reconstructions/modstation-mgame-wave114-w6220.md` — DAG live-out /
  r5-anchor sweeps.
* `_work/reconstructions/modstation-mgame-wave113-w6120.md` — far=pitch 42/7,
  Status-anchor sweeps.
* `_work/reconstructions/modstation-ctor-wave112-w6018.md` — landed 11 B / 4
  runs shape.
* `_work/reconstructions/mgame-onupdate-wave112-w6019.md` — greedy-RA r5/r9
  mechanism.
