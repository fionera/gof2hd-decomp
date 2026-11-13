# ModStation constructor — wave 191b (w13738): per-TU flag sweep + live-across re-probe

Worker `w13738`, branch `wave191b-w13738`, base master `22c4b25f5`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Scratch `/home/debian/GalaxyOnFire2/scratch/w13738`.
Predecessor notes (read first): `modstation-ctor-wave191b-w13710.md`, `-w13693`,
`-w13653`, `-w13641`, `modstation-ctor-pass4-w4220.md`.

Target `_ZN10ModStationC1Ev` / `_ZN10ModStationC2Ev` (aliases at `0xd52d0`,
1008 B).  Status: **no source change landed; no raw win below 11 found.**
Tree clean; `check` green at the entry counts.

## Result

| symbol | VMA | size | raw / aligned | insn |
|---|---|---:|---:|---:|
| `_ZN10ModStationC1Ev` | `0xd52d0` | 1008 | **11 / 5** | 374/374 |
| `_ZN10ModStationC2Ev` (alias) | `0xd52d0` | 1008 | **11 / 5** | 374/374 |

Gate on entry and exit (full `check`, `-j2`, `ulimit -v 12000000`):
`RATCHET PASS`, byte_exact **4508/4524**, linked_exact **4508/4524**,
size 4524/4524, `stub_zero_size 0`, `missing 0`, `wrong_type 0`, `extra 0`,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.

Section content deltas re-measured this pass (all section *sizes* exact):
`.text` **72,646**, `.ARM.extab` **564**, `.note.gnu.build-id` **20**;
`.ARM.exidx`, `.rodata`, `.data`, `.rel.plt` all **0**.

## The residual is exactly 11 bytes, all in the near `MatrixSetRotation` tail

`tools/fn_diff.py` reports the 11 differing bytes at function offsets
`0x186, 0x188..0x18f, 0x19c, 0x19e` (absolute `0xd5456..0xd545f`,
`0xd546c`, `0xd546e`).  Everything before `0xd5456` — including the three
literal-pool loads, the `add rX,pc` interleave, `ldrb [sl]`, the pitch load,
`mov r0,r3`/`cmp`/`str r3`/`str r4`/`it eq`/`moveq r0,r4` — is byte-identical.

```
orig 0xd5456: f850 3028  ldr.w r3,[r0,r8,lsl#2]      ; yawBits, direct
orig 0xd545a: 2002       movs  r0,#2                 ; order
orig 0xd545c: f8cd 80b8  str.w r8,[sp,#184]          ; spill race AFTER movs
...
orig 0xd546c: 4640       mov   r0,r8
orig 0xd546e: 4614       mov   r4,r2                 ; save pitchBits for far

ours 0xd5456: f8cd 80b8  str.w r8,[sp,#184]          ; spill race BEFORE load
ours 0xd545a: f850 4028  ldr.w r4,[r0,r8,lsl#2]      ; yawBits -> r4
ours 0xd545e: 2002       movs  r0,#2
...
ours 0xd546c: 4623       mov   r3,r4                 ; copy yawBits into near arg2
ours 0xd546e: 4640       mov   r0,r8
```

So the residual is a 3-instruction rotation (`ldr.w` / `movs` / `str.w`) plus a
2-instruction swap (`mov r0,r8`/`mov r4,r2` vs `mov r3,r4`/`mov r0,r8`).

## The landed 11-byte form is semantically wrong

Landed HEAD source:

```cpp
int pitchBits = *(const volatile int *) &pitchTable[race];
int yawBits   = *(const volatile int *) &yawTable[race];
float yaw   = *(const float *) &pitchBits;   // = pitch
float pitch = *(const float *) &yawBits;     // = yaw
MatrixSetRotation(nearKey, yaw, pitch, 0.0f, (RotationOrder) 2);   // (pitch, yaw)  OK
MatrixSetRotation(farKey,  pitch, (iPad?YawIPad:Yaw)[race], ...); // far x = yaw   WRONG
```

Ghidra (`0xe52d0`, decompile in `scratch/w13738/ghidra/out7.txt`) and the
disassembly both show **both** calls take `(pitchBits, yawBits)`: the near call
passes `r2 = pitchBits`, `r3 = yawBits`, and the far call does
`mov r2,r4` where `r4` was set by the near block's `mov r4,r2` (i.e. `pitchBits`).
The landed form trades the far operand for the correct literal-pool order
(`Pitch,YawIPad,Yaw`); it is byte-close but semantically swaps pitch for yaw in
the far key.  The faithful form is the w13641/w13693 "35 form" below.

## New evidence this pass

### 1. Full per-TU flag sweep on both the landed form and the 35 form (no win)

`scratch/w13738/flagsweep.py` appends one flag to the `ModStation.cpp` FLAGS
line in `cmake-build-match/build.ninja`, relinks, and measures.  42 flags from
`flaglist.txt` plus 20 two-flag combinations were run on **both** sources.
Nothing beats raw 11 on the landed form and nothing beats raw 35 on the 35 form.
Selected rows (size 1008 unless noted):

| flag | landed raw/al | 35-form raw/al |
|---|---:|---:|
| (none) | **11 / 5** | 35 / 10 |
| `-mllvm -enable-post-misched=false` | 11 / 5 | 35 / 10 |
| `-mllvm -enable-misched=false` | 11 / 5 | 35 / 10 |
| `-mllvm -break-anti-dependencies=none` | 11 / 5 | 35 / 10 |
| `-mllvm -disable-sched-{height,critical-path,live-uses,reg-pressure,stalls}` | 11 / 5 | 35 / 10 |
| `-mllvm -disable-postra-machine-licm` | 11 / 5 | 35 / 10 |
| `-mllvm -enable-earlycse-memssa=false` | 11 / 5 | 35 / 10 |
| `-mllvm -disable-2addr-hack` | 11 / 5 | 35 / 10 |
| `-mllvm -misched-cluster=false` | 11 / 5 | 35 / 10 |
| `-mllvm -split-spill-mode=speed` | 11 / 5 | 35 / 10 |
| `-mllvm -split-spill-mode=size` | 17 / 10 | 39 / 13 |
| `-mllvm -misched-postra=true` | 27 / 8 | 51 / 13 |
| `-mllvm -enable-aa-sched-mi` | 55 / 12 | 79 / 17 |
| `-mllvm -disable-postra-machine-sink` | 149 / 29 | 171 / 32 |
| `-mllvm -disable-sched-cycles` | 234 / 112 | 241 / 104 |
| `-mllvm -disable-post-ra` | 418 / 99 | 424 / 99 |
| `-mllvm -disable-block-placement` | 682 / 60 (**1016**) | 684 / 62 (**1016**) |
| `-mllvm -disable-sched-hazard` | 807 / 145 (**1000**) | 275 / 111 |
| `-mllvm -pre-RA-sched=list-ilp` | 833 / 295 | 906 / 303 (**992**) |
| `-mllvm -pre-RA-sched=list-burr` | 886 / 275 (**976**) | 887 / 282 (**976**) |
| `-Xclang -target-feature -Xclang +cheap-predicable-cpsr` | 17 / 10 | 39 / 13 |
| `-mllvm -misched-postra=true -mllvm -enable-post-misched=false` | 418 / 99 | — |
| `-mllvm -enable-post-misched=false -mllvm -break-anti-dependencies=none` | 11 / 5 | — |
| `-mllvm -misched-postra=true -mllvm -{enable-misched=false,break-anti-dependencies=none,enable-aa-sched-mi,disable-postra-machine-licm,disable-2addr-hack}` | 27 / 8 | — |

`-mllvm -enable-post-RA-scheduler=false`, `-enable-gvn-memssa=false`,
`-enable-unsafe-fp-math`, `-enable-machine-outliner=never`,
`-enable-copy-propagation=false`, `-enable-local-reassoc=false`,
`-enable-merge-functions=false`, `-enable-ext-tsp-block-placement=false`,
`-enable-msched-load-cluster=false` all fail to build with clang 7 (unknown
option / crash).

The w4220 claim that `-enable-post-misched=false` reproduces a near-original
tail does **not** survive the link on either source: the `.o` tail moves but the
linked function stays at raw 11 (landed) / 35 (35 form).

### 2. The 35 form is semantically correct and size-exact, but a raw regression

```cpp
const float *pitchTable = gHangarCamPitch;
const float *yawTable = Globals::iPad ? gHangarCamYawIPad : gHangarCamYaw;
int pitchBits = *(const int *) &pitchTable[race];
int yawBits   = *(const volatile int *) &yawTable[race];
__builtin_assume(pitchBits != 0x7f800001);
__atomic_signal_fence(__ATOMIC_SEQ_CST);
float pitch = *(const float *) &pitchBits;
float yaw   = *(const float *) &yawBits;
MatrixSetRotation(nearKey, pitch, yaw, 0.0f, (RotationOrder) 2);
... far translation ...
MatrixSetRotation(farKey, pitch, (iPad?YawIPad:Yaw)[race], 0.0f, (RotationOrder) 2);
```

= **35 / 10, size 1008, `.text` 72,670 (+24 vs 72,646), `.ARM.extab` 564
(unchanged)**.  It reaches the correct literal pool (`Pitch,YawIPad,Yaw`), the
correct instruction *multiset* (`ldr.w r3` direct, `mov r4,r2`) and the correct
far operand, but the post-RA top-down latency scheduler hoists
`str.w r8,[sp,#184]` and `mov r4,r2` above `ldr.w r3`, and interleaves the
three `add rX,pc` differently (`ldr r0;ldr r3;add r0;ldr r4;add r3;ldrb;ldr r2;add r4`
vs the original's `ldr r0;ldr r3;ldr r4;add r0;add r3;ldrb;add r4;ldr r2`).
Because it is a net raw loss it is **not** landed.

### 3. The `__atomic_signal_fence` is a `CompilerBarrier` and splits the block

`__atomic_signal_fence(__ATOMIC_SEQ_CST)` lowers to `CompilerBarrier 7`, which
has `isBarrier = 1` and therefore splits the near block into two post-RA
scheduling regions.  That is what pins `mov r4,r2` before the barrier in the
35 form.  Removing the fence (or moving it after the near call) loses the
literal-pool order and/or the size:

| fence position (with the `pitchBits` assume) | raw/al | size |
|---|---:|---:|
| after both loads (35 form) | 35 / 10 | 1008 |
| after the two `float` locals | 35 / 10 | 1008 |
| before `pitchTable` decl | 701 / 52 | **1000** |
| between the two pointer decls | 701 / 52 | **1000** |
| before the loads | 700 / 63 | **1000** |
| after the near call | 700 / 63 | **1000** |
| after the far translation | 700 / 63 | **1000** |
| no fence at all | 702 / 53 | **1000** |

With both loads `volatile` and no assume, every fence position is 42–43 / 12
and the pool flips to `Pitch,Yaw,YawIPad`.

### 4. New source permutations re-tested (all 35 / 10, size 1008)

Statement/type permutations layered on the 35 form that all reproduce the
identical 35-byte result: `(void) race;` / `(void) pitchBits;` after the yaw
load; `(void) pitch;` after the near call; `__builtin_assume(pitchBits != …)`
after the near call; `const int pitchBits`; `const int yawBits`; both `const`;
`unsigned pitchBits`; `(pitchBits & 0x7f800000) != 0x7f800000`;
`pitchBits < 0x7f800001`; assume placed between the two loads; `race + 0` far
index; `Matrix nearKey, farKey;` declared together.

Forms that are size-fatal or worse: yaw load before the pitch load
(752 / 40, **1000**); `int ipad`-style caching of `Globals::iPad`; inline
`(iPad?YawIPad:Yaw)[race]` in the near call (698 / 52, **1000**);
`swapdecl` (46 / 13); `nearswap` (44 / 14).

## Conclusion

The landed 11-byte form remains the measured floor and is a hard local optimum
under ~200 source permutations and 62 per-TU flag cells this pass (on top of
the ~150 permutations and ~2,000 cells of waves 106–191b).  The last 11 bytes
are a pure post-RA list-scheduler tie: the original keeps
`ldr.w r3` / `movs r0,#2` / `str.w r8` in that order and puts `mov r4,r2`
immediately before the `blx`, while clang 7 emits `str.w r8` / `mov r4,r2`
before the yaw load.  The semantically faithful 35 form reaches the correct
pool and multiset but costs +24 `.text` bytes (`.ARM.extab` unchanged), so it
is not landed.  No source change; tree clean; `check` green at the entry counts.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13738
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
    _ZN10ModStationC1Ev _ZN10ModStationC2Ev
# raw=11 aligned=5 size=1008/1008 insn=374/374
(ulimit -v 12000000; cmake --build cmake-build-match --target check -j2)
# RATCHET PASS byte_exact 4508/4524 linked_exact 4508/4524
```

Flag sweep: `python3 scratch/w13738/flagsweep.py scratch/w13738/flaglist.txt`
(restores `build.ninja` and relinks at the end).
