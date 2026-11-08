# Small tail wave 156 (w10111): Gun / ModStation / MenuTouchWindow / MGame
# new hypotheses — no source win (note only)

Status: **note only, no `src/` change**. Worktree `wt/w10111`, base master
`f4e0ea40b` (wave 154). Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Read with `small-tail-wave155-w10017.md`, `small-tail-b-wave154-w10018.md`,
`small-tail-wave154-w9917.md`.

Gate on entry and exit (no src edit): `byte_exact 4507`, `linked_exact 4507`,
size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0, parity
5370/5370, RATCHET PASS, avg 99.93.  All four bodies stay size-exact.

Metrics: raw same-address diff bytes (`/dev/shm/w10111/fdiff.py`) and
instruction-index alignment (`/dev/shm/w10111/align_diff.py`, path-adjusted
copy of the committed tool).

| fn | raw before | raw after | aligned before | aligned after | size |
|---|---:|---:|---:|---:|---:|
| `Gun::calcCharacterCollision` (`_ZN3Gun22calcCharacterCollisionEv`) | 463 | 463 | 57 | 57 | 1984 |
| `ModStation::ModStation` C1 (`_ZN10ModStationC1Ev`) | 43 | 43 | 12 | 12 | 1008 |
| `MenuTouchWindow::OnTouchBegin` (`_ZN15MenuTouchWindow12OnTouchBeginEiiPv`) | 29 | 29 | 15 | 15 | 1896 |
| `MGame::OnUpdate` (`_ZN5MGame8OnUpdateEv`) | 37 | 37 | 109 | 109 | 11440 |

## 1. MGame::OnUpdate — the `mov r0,r5` pair is a Greedy-RA consequence of
##    `%142` (the `Status *st` value) occupying r5; fresh loads need r5 free

The w10017 landed shape is the closest: the whole 11440-byte body is
byte-identical to the original except

```
17e2b6  orig ldr.w r2,[r9]   ours ldr.w r5,[r9]      (condition field_0x100)
17e2bc  orig ldrd  r2,r3,[r2,#256]  ours ldrd r2,r3,[r5,#256]
17f192  orig ldr.w r0,[r9]   ours mov r0,r5          (getCurrentCampaignMission)
17f1b2  ours extra mov r0,r5                          (getPlayingTime)
```

Verified against the Greedy-RA MIR dump (`-mllvm -print-after-all`,
`_ZN5MGame8OnUpdateEv`): `%142:gpr` is the `Status *st` value, live across
`createRadioMessage` and copied to `$r0` at both call sites.  `%2930:gpr` is
`&Globals::status` and is the value the original reloads (`t2LDRi12 %2930, 0`)
at both sites.  Because `%142` occupies r5, the allocator places `%2930` in
r9; removing `st` (pure direct) lets `%2930` take r5 and the whole chain
re-colours (11392 B, raw 5148).

New shapes measured this wave (all 11440 B unless noted; no win):

| shape | raw | aligned | size |
|---|---:|---:|---:|
| landed (`st` for ccm + store) | **37** | **109** | 11440 |
| `st` only at ccm, store direct (`(void) st` before store) | 37 | 109 | 11440 |
| `Status &st = *Globals::status` (reference) | 37 | 109 | 11440 |
| `Status *const st` | 37 | 109 | 11440 |
| `(*&*&st)` spelling | 37 | 109 | 11440 |
| `st` + dead `if (st->field_0x100==0 && st->field_0x104==0) {}` | 37 | 109 | 11440 |
| `st` after `nextInt` (condition then uses r2) | 922 | 127 | 11444 |
| `st` after `nextInt`, ccm direct / store `st` | 922 | 127 | 11444 |
| `(void) st` after the store | 914 | 109 | 11436 |
| `volatile Status *st` | 331 | 116 | 11440 |
| `__builtin_expect(st != 0, 1)` around the store | 928 | 131 | 11444 |
| `(st ? st : Globals::status)->getPlayingTime()` | 934 | 132 | 11448 |
| pure direct (no `st`) | 5148 | 597 | 11392 |

Conclusion: the two `mov`s cannot be removed while keeping `%2930` in r9 with
any local-`st` spelling; the `st` value is exactly what forces the r9
allocation.  A source shape that keeps r9 without a second live `Status*`
value is still open.  The landed 37/109 shape is retained.

## 2. ModStation::ModStation — pool *content* is identical; only the
##    register colouring and PostRA schedule differ

The literal-pool words at `0xd56b4`/`0xd56b8` are the same GOT-relative
offsets in both binaries.  The residual is:

```
orig  d5438 ldr r3,[pc]   ; YawIPad      ours d5438 ldr r4,[pc] ; YawIPad
      d543a ldr r4,[pc]   ; Yaw                d543a ldr r3,[pc] ; Yaw
      d544a mov r0,r3     ; default            d544a cmp r1,#0
      d544c cmp r1,#0                          d544c mov r0,r3
      d5456 ldr r3,[r0,r8,lsl#2]  ; yaw        d5456 str r8,[sp,#184]
      d546e mov r4,r2     ; pitch save         d545a mov r4,r2
                                               d545c ldr r3,[r0,r8,lsl#2]
```

i.e. the select is the same `iPad ? YawIPad : Yaw` (assume-true MOVCC); the
original keeps `mov r0,r3` before `cmp` and schedules the volatile yaw load
immediately after the MOVCC.  Dropping `volatile` fixes the pool order but
the PostRA scheduler sinks the `Yaw` LDRLIT and hoists `mov r4,r2` (44/18).

New shapes measured this wave (all 1008 B and 43/12 unless noted):

| shape | raw | aligned |
|---|---:|---:|
| HEAD (both `volatile int` loads) | **43** | **12** |
| `__builtin_expect(iPad != 0, 1)` if/else | 43 | 12 |
| `__builtin_expect(iPad == 0, 0)` if/else | 43 | 12 |
| `__builtin_expect(iPad, 1) ? ... : ...` | 43 | 12 |
| `__builtin_expect(iPad, 0) ? ... : ...` | 43 | 12 |
| `!iPad ? Yaw : YawIPad` | 43 | 12 |
| `yawTable = YawIPad; if (!iPad) yawTable = Yaw;` | 43 | 12 |
| `yawTable = Yaw; if (iPad) yawTable = YawIPad;` | 43 | 12 |
| `if (iPad) ... else ...` | 43 | 12 |
| `yawTable` declared before `pitchTable` | 46 | 13 |
| `float pitch` declared before `float yaw` | 43 | 12 |
| `const volatile float *yawTable` (pointer volatile) | 278 | 149 |
| `iPad == 1 ? YawIPad : Yaw` (IR `icmp eq 1`) | 688 | 55 (1000 B) |

The `iPad == 1` form is the only one that flips the IR select to
`select %cond, YawIPad, Yaw` (true operand first), but `icmp eq 1` cannot
lower to a plain MOVCC and the body shrinks to 1000 B.  No source form keeps
the volatile schedule *and* the non-volatile pool/register order.

## 3. MenuTouchWindow::OnTouchBegin — HEAD 29/15 still beats every new shape

The 29-byte residual is the case-15 cluster (register colouring
`r3/r6/r2/r5` vs `r2/r3/r6/r4`, C cached vs direct) plus the independent
`ip`/`r4` tie at `0x129fd2`.  New shapes this wave (all 1896 B unless noted):

| shape | raw | aligned | size |
|---|---:|---:|---:|
| HEAD (A,B direct; C ref; D direct) | **29** | **15** | 1896 |
| `scrollbarHit = (x > ... && y < ...)` bool expression | 331 | 83 | 1892 |
| `scrollbarHit = 0; if (...) scrollbarHit = 1;` | 362 | 87 | 1896 |
| `Layout *layout = Globals::layout` + all direct | 358 | 76 | 1892 |
| `Layout *layout` + A via pointer | 358 | 76 | 1892 |
| `Layout *layout` + dead use in drag body | 358 | 76 | 1892 |
| `Layout &layout` + second if via `layout.buttonInsetX` | 314 | 74 | 1888 |
| `int top = Globals::layout->field_0x20_top;` local | 316 | 33 | 1892 |
| all four operands direct | 358 | 76 | 1892 |

No shape improves both metrics; HEAD is retained.

## 4. Gun::calcCharacterCollision — i*3 stays in r5; 463/57 is the floor

The landed HEAD already has the original's stack layout (`getPosition` sret at
`sp+44`, subtraction temp at `sp+104`).  The residual is the register pair:

```
orig 15306c add.w r4, fp, fp, lsl #1   ; i*3 -> r4
     1530d2 add r4, sp, #44            ; sret -> r4
     1530dc add r5, sp, #104           ; delta -> r5
ours 15306c add.w r5, fp, fp, lsl #1   ; i*3 -> r5
     1530d2 add r4, sp, #104           ; delta -> r4
     1530dc add r5, sp, #44            ; sret -> r5
```

New shapes measured this wave (all 1984 B unless noted):

| shape | raw | aligned | size |
|---|---:|---:|---:|
| HEAD | **463** | **57** | 1984 |
| `for (i, i3=0; ...; ++i, i3+=3)` + `data()[i3/3]` | 1732 | 299 | 1968 |
| `data()[3*i/3]` | 1750 | 326 | 1964 |
| hoisted `const Vector *posData/velData` | 1649 | 182 | 1992 |

The explicit-`i3` forms from wave 155 still fold back to the same
`t2ADDrs` scaling (463/57).  Forcing `i*3` into r4 without changing the
1984-byte size remains open.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10111
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w10111/fdiff.py <mangled>
python3 /dev/shm/w10111/align_diff.py <mangled>
```

Scratch (not committed), `/dev/shm/w10111/`: `fdiff.py`, `align_diff.py`,
`rdiff.py`, `dis.sh`, `measure.sh`, `sweep_mg.py`, `sweep_mg2.py`,
`sweep_ms*.py`, `sweep_mtw*.py`, `sweep_gun.py`, `irsel.py`, `*.base.cpp`,
`onupd_greedy.mir`, `ms_greedy.mir`, `ms_isel.mir`.

## 6. Files changed

* `_work/reconstructions/small-tail-wave156-w10111.md` (this note).

Gate: byte 4507 / linked 4507, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, RATCHET PASS, avg 99.93.
