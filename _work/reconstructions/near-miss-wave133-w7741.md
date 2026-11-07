# Near-miss trio wave 133 (w7741): MenuTouchWindow / ModStation / MGame — new evidence, no source win

Status: **no landed source change.** Worktree `wt/w7741`, base `144fc16b0`. Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a, NDK r18b
clang 7.0.2, `-Oz`, image base 0x10000). Extends
`menutouchwindow-ontouchbegin-wave132-w7719.md`, `modstation-ctor-wave130-w7682.md`,
`mgame-onupdate-wave129-w7661.md`.

Gate before/after: byte_exact **4506 -> 4506**, linked_exact **4506 -> 4506**, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, RATCHET PASS.

## 1. MenuTouchWindow::OnTouchBegin — the original's case-15 block, read from objdump

Target `_ZN15MenuTouchWindow12OnTouchBeginEiiPv`, 1896 B @0x129bd4. The whole residual is the
case-15 `scrollbarHit` block. Original (`objdump` of the binary, `sl`=this, `fp`=&Globals::Canvas):

```text
12a1bc  ldr   r4, [sp, #16]        ; &Globals::layout  (remat from prologue spill)
12a1da  ldr   r0, [pc, #316]       ; &Globals::w
12a1dc  ldr   r1, [pc, #316]       ; &Globals::Canvas
12a1de  add   r0, pc
12a1e0  ldr.w r3, [sl, #312]       ; this->heapBufB
12a1e4  add   r1, pc
12a1e6  ldr.w r6, [sl, #480]       ; this->field_0x1e0
12a1ea  ldr   r0, [r0, #0]         ; &Globals::w
12a1ec  ldr.w fp, [r1]             ; fp = &Globals::Canvas
12a1f0  ldr   r2, [r4, #0]         ; Globals::layout value   (fresh load #1)
12a1f2  ldr.w r1, [r3, r6, lsl #2]
12a1f6  ldr   r5, [r0, #0]         ; Globals::w
12a1f8  ldr.w r0, [fp]             ; Globals::Canvas
12a1fc  ldr   r2, [r2, #40]        ; layout->buttonInsetX
12a1fe  subs  r5, r5, r2
12a200  blx   GetImage2DWidth
12a204  subs  r0, r5, r0
12a206  cmp   r0, r8
12a208  bge   12a232
12a20a  ldr.w r0, [sl, #312]       ; heapBufB
12a20e  ldr.w r1, [sl, #480]       ; field_0x1e0
12a212  ldr   r2, [sp, #16]        ; &Globals::layout  (remat #2)
12a214  ldr.w r1, [r0, r1, lsl #2]
12a218  ldr   r2, [r2, #0]         ; Globals::layout value   (fresh load #2)
12a21a  ldr.w r0, [fp]             ; Globals::Canvas
12a21e  ldr   r5, [r2, #12]        ; layout->field_0xc_leftMargin
12a220  ldr   r6, [r2, #32]        ; layout->field_0x20_top
12a222  blx   GetImage2DHeight
12a226  adds  r1, r6, r5
12a228  add   r0, r1
12a22a  cmp   r0, r9
12a22c  ble   12a232
12a22e  movs  r0, #1
12a230  b     12a234
12a232  movs  r0, #0
12a234  ldr.w r1, [sl, #308]       ; heapBufA
12a238  strb.w r0, [sl, #473]      ; scrollbarHit
12a23c  ldr   r0, [sp, #16]        ; &Globals::layout  (remat #3)
12a23e  ldr   r1, [r1, #0]
12a240  ldr   r2, [r0, #0]         ; Globals::layout value   (fresh load #3)
12a242  ldr.w r0, [fp]             ; Globals::Canvas
12a246  ldr   r5, [r2, #40]        ; layout->buttonInsetX
12a248  blx   GetImage2DWidth
12a24c  add   r0, r5
12a24e  cmp   r0, r8
```

Three facts that the wave-132 note did not pin down:

1. **`fp` = `&Globals::Canvas`, held across the whole block.** `&Globals::layout` is *remat'd
   from `[sp, #16]`* at each of the three uses (12a1bc / 12a212 / 12a23c).
2. **The `Globals::layout` value is loaded fresh in all three sub-blocks** (12a1f0, 12a218,
   12a240). There is no cached `%29` anywhere in the original.
3. The prologue spills `&Globals::layout` to `[sp, #16]` at 0x129c14 (`str r1,[sp,#16]`), so the
   remats are stack reloads, not GOT reloads.

HEAD (`far` = current source) instead has `%29` (the inner `Layout &layout` reference's value)
live across the first call, used for `buttonInsetX` (bb.42) and `field_0x20_top` (bb.44). That
liveness shifts the whole local assignment by one register:

| value | original | HEAD |
|---|---|---|
| heapBufB | r3 | r2 |
| field_0x1e0 | r6 | r3 |
| Globals::layout value | r2 | r6 (live across call) |
| Globals::w | r5 | r4 |
| buttonInsetX | r2 | r5 |
| &Globals::Canvas | fp | fp |
| &Globals::layout | remat [sp,#16] | remat [sp,#16] |

So HEAD already has the right *GOT-pointer* handling; the 16 differing instructions are the
one-register shift caused by `%29`.

### 1.1 Variant D is the exact original IR but flips the fp choice

The all-direct form (variant `D`: `int right = Globals::w - Globals::layout->buttonInsetX;` then
`Globals::layout->` everywhere) reproduces the original's local register roles exactly
(heapBufB=r3, field_0x1e0=r6, layout=r2) but the allocator then puts `&Globals::layout` in `fp`
and `&Globals::Canvas` in `r5` (53 object-level diffs). The original does the opposite.

So the residual is exactly one Greedy-RA "keep vs remat" choice on the `&Globals::layout` reload:
`D` keeps it in `fp` across the block; the original remats it at each use and gives `fp` to
`&Globals::Canvas`. This matches the wave-132 conclusion, now confirmed at the disassembly level
(the wave-132 note's `%508`/`%29` story is consistent with this).

### 1.2 New source variants measured this wave (all reverted)

Harness `/dev/shm/w7741/mtw/` (`build.sh` exact per-TU flags, `score.py` object-level instruction
diff vs the original, `c15.py` case-15 side-by-side). HEAD object score = **42** (18 verify
structural lines). Lower is better.

| variant | score | note |
|---|---:|---|
| HEAD (committed) | **42** | `%29` live, correct fp |
| D (all-direct + `right` local) | 53 | exact IR, fp flipped |
| M1 `layout` ref for buttonInsetX only | 53 | |
| M2 `layout` ref for `field_0x20_top` only | 50 | best new |
| M3 `layout` ref in third `if` only | 55 | |
| M4 D without any `layout` ref | 53 | |
| M5 D + `(void)layout.field_0x20_top;` in third `if` | 53 | load eliminated |
| M6 D + `if (layout.field_0x20_top == -1)` | 71 | extra branch |
| M7 D + `(void)Globals::layout->field_0x20_top;` | 53 | load eliminated |
| N1 `layout` ref + `right` local | 55 | |
| N2 `layout` ref declared after the first `if` | 53 | |
| N3 `layout` ref after first `if`, used in body | 53 | |
| N5 D + `(Globals::layout->field_0x24, true)` | 53 | folded |
| N6 `Layout *lp = Globals::layout;` local | 55 | |
| N7 D + `Globals::layout->field_0x20_top;` stmt | 53 | eliminated |
| O1 D using the *function-scope* `layout` variable | 337 | changes whole-function liveness |
| O2 D all-direct (repeat) | 53 | |
| O3 swap the `x`/`y` condition order | 73 | |
| O4 D + `Layout *lp` local | 52 | |
| O5 third `if` uses function-scope `layout` | 334 | |

No shape beat HEAD. The user's suggested lever ("keep `%29` live across the first call without
emitting a use of it") was probed with `(void)` loads, comma-folded loads, dead comparisons and
`layout`-reference/`Globals::layout` mixes; every one either keeps the load (changing the pool /
schedule) or is eliminated before regalloc.

### 1.3 Warning: the llvm7build `llc` regalloc logs are not authoritative for this TU

`/home/debian/llvm7build/build/bin/llc` is vanilla LLVM 7.0.0; the NDK clang is Android's
7.0.2 fork. On the same `-g0 -emit-llvm` module the two disagree on this function: clang emits
695 instructions with `push.w {r3,r4,r5,r6,r7,r8,r9,r10,r11}` and `mov r5,r3`, llc (with
`-relocation-model=pic -float-abi=softfp` and the exact `-mattr` from the module) emits 651 with
`push.w {r5,r6,r7,r8,r9,r10,r11}` and no `mov r5,r3`. The llc MIR also keeps `&Globals::layout`
in `r11` where clang's final code remats it into `r4`. So wave-132's log-derived interval
statements describe llc's allocation, not the shipped one; treat them as a hint only.

## 2. ModStation::ModStation — faithful `far = pitch` re-measured

Target `_ZN10ModStationC1Ev`/`C2`, 1008 B @0xd52d0. Committed baseline (`far = yaw`, volatile-int
bit loads) = **5 differing instructions** (verify.py `--show`). Faithful `far = pitch` was rebuilt
this wave and gives **17 structural diff lines** (1008 B), confirming wave-130's 10-instruction
figure is the same residual.

New hard datum: the original's literal pool at `0xd56b0` is
`0x00128bf0, 0x00128c16, 0x00128c38, 0x0013ac16` = `gHangarCamPitch, gHangarCamYawIPad,
gHangarCamYaw, <string>`, i.e. pool order **Pitch, YawIPad, Yaw**. The faithful variant emits the
two yaw LDRLITs in the opposite order (Yaw then YawIPad) and schedules `mov r4,r2`; the baseline
emits the right pool order but `mov r3,r4`.

The remaining lever is exactly the `NodeQueueId` tie-break between the `gHangarCamYawIPad` and
`gHangarCamYaw` LDRLITs (wave-130 §2: no `Comparing latency` line). No source spelling found this
wave that changes it while keeping the faithful register roles; the 720 declaration-order
permutations of wave-130 still cover the obvious space.

## 3. MGame::OnUpdate — not attempted this wave

Time-boxed out. The wave-129 residual (`0x17e08c..0x17e0bb`, mission-23 idle contact,
`src/game/menu/MGame.cpp:2766-2772`) and the two untried levers (cross-TU `Globals.h` `&Globals::status`
hint, or a body construct adding a `Globals::status` use inside `bb.242`) remain open. The
llvm7build MIR block map from wave 129 is subject to the §1.3 caveat.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w7741
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN15MenuTouchWindow12OnTouchBeginEiiPv
```

MenuTouchWindow variant harness (self-contained, `/dev/shm/w7741/mtw/`): `build.sh <src.cpp>
<out.o>` compiles one TU with the exact per-TU flags; `score.py <obj>` diffs the object's
`.text.<fn>` against the original function bytes (branch targets normalised, jump table and
literal pool skipped); `c15.py <obj>` prints the case-15 block side by side. `batch4.py`,
`batch5.py`, `batch6.py` generate the M/N/O variants above.
