# Radar::draw pass (wave119, w6910) — literal-pool placement is a *water* problem

**Status:** research note, 2026-11-05. Worktree `wt/w6910`, branch
`wave119-w6910`, based on current `master` (after `radar-draw-wave119-w6837.md`).
**No source change landed**; this commit contains this note only. The target
remains the pristine baseline.

Target `_ZN5Radar4drawEP6PlayerP3Hudi` in `src/game/weapons/Radar.cpp`.
Original 9908 B / 3492 normalised instructions; ours 9908 B / 3484.
Baseline unrounded `difflib.SequenceMatcher(None, on, un, autojunk=False).ratio()`
= **74.426606 %** (official `verify.py` display 74.4). 2596 instructions match.
896 original-side instructions are mismatched (404 of them branches).

Gate at start and end: `RATCHET PASS`, byte 4505, linked 4505, data 843/843,
imports 207, lint CLEAN, drift 0. No section/extab change (note-only commit).

## 1. Exact commands used

```sh
cd /home/debian/GalaxyOnFire2/wt/w6910
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6        # RATCHET PASS

NDK=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$NDK -d --start-address=0x12e050 --stop-address=0x130740 \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so > /dev/shm/w6910/orig.asm
$NDK -d --start-address=0x12e050 --stop-address=0x130740 \
  cmake-build-match/libgof2hdaa.so > /dev/shm/w6910/ours.asm
```

Per-variant loop (≈2.5 s each): edit `Radar.cpp`, `cmake --build
cmake-build-match --target link_so -j6`, then score the single symbol with a
scratch `difflib` harness that mirrors `tools/verify/verify.py`'s
`disassemble`/`reconcile` (identical 74.426606 % on the baseline).

## 2. Proven: the pool is placed at the highest *water* within range of the first CP user

The pool is the `ARMConstantIslandPass` (`llvm/lib/Target/ARM/ARMConstantIslandPass.cpp`,
release/7.x).  Two facts from that pass drive everything:

* **Water** = a basic block that does not fall through into the next block
  (`initializeFunctionInfo`: `if (!BBHasFallthrough(&MBB)) WaterList.push_back(&MBB);`).
  In Thumb code that is almost always a block ending in an unconditional
  `b`/`b.n`/`b.w` (or a return).
* **Placement**: all CPEs start in one block at the end of the function
  (`doInitialConstPlacement`).  For each CP user in address order,
  `handleConstantPoolUser` calls `findAvailableWater`, which walks `WaterList`
  **from the end backwards** and returns the first water that is in range with
  zero padding growth.  So the pool lands at the *highest-addressed water*
  within `MaxDisp` of the **first** user.  (`optimizeThumb2Instructions` later
  shrinks the 32-bit `t2LDRpci` to the 16-bit `tLDRpci`; the observed `48xx`
  encoding means the placement range is the Thumb-1 literal range,
  `MaxDisp = 255*4 = 1020`.)

The first CP user in `Radar::draw` is the `Globals::Canvas` load for
`SetColor` at **12e064** (function offset 0x14).  Its `tLDRpci` pc base is
`12e068`.

| | highest water ≤ 12e068+1020 = 12e464 | distance | pool address |
|---|---|---:|---:|
| original | **12e33c** (`b.n 12e376`, landmark A\|\|B skip) | 724 | **12e340** |
| ours     | **12e30c** (`b.n 12e338`, otherLock if-conversion) | 676 | **12e310** |

The next water after each is out of range (orig 12e4a6 → 1086; ours 12e49a →
1074), which is why the pool stops where it does.  The 5 pool words are
identical on both sides; only the address differs.  **This is proven by the
pass source plus the observed water list, not inferred from the diff.**

The water list before 12e464:

```
original: 12e1a6, 12e2de, 12e33c        (next 12e4a6)
ours:     12e1a6, 12e2de, 12e30c        (next 12e49a)
```

So the whole 0x30 pool displacement is caused by **one missing water at
12e33c**: eliminate the 12e30c `b.n` and create the 12e33c `b.n` and the pool
moves to 12e340 with no change in body size.

### 2.1 Sensitivity check (first-use statement order)

Moving `Globals::Canvas->SetColor(0xffffffff);` from the top of the function to
after the campaign-mission block (so the first pool use becomes
`Globals::status` at the same pc) moves the pool from 12e310 to 12e30c
(`9904 B / 72.039002 %`).  The pool really is driven by the first use; it is
not a fixed per-TU `.text` artifact.

## 3. Proven: the 12e30c water is the otherLock if-conversion

Original otherLock chain (12e2fc–12e312):

```
12e2fc  mov  r1, r4
12e2fe  ldr  r0, [r1, #12]!     ; lockedAsteroid, r1 = &lockedAsteroid
12e302  str  r1, [sp, #124]
12e304  cbnz r0, 12e312         ; a != 0 -> set1
12e306  ldr  r0, [r4, #56]      ; lockedGasCloud
12e308  cbnz r0, 12e312
12e30a  ldr  r0, [r4, #28]      ; field_0x1c
12e30c  cmp  r0, #0
12e30e  beq.w 12ebc2            ; all null -> cold (otherLock = field_0x14 != 0)
12e312  movs r5, #1             ; single shared set1
```

Ours (12e2fc–12e338):

```
12e2fc  mov  r1, r4
12e2fe  ldr  r0, [r1, #12]!
12e302  str  r1, [sp, #124]
12e304  cbz  r0, 12e324         ; a == 0 -> check b   <-- inverted polarity
12e306  mov.w r9, #1            ; set1 #1
12e30a  ldr  r5, [sp, #156]     ; status load
12e30c  b.n  12e338             ; <-- THE WATER
12e324  ldr  r0, [r4, #56]
12e326  ldr  r5, [sp, #156]
12e328  cmp  r0, #0
12e32a  itt  eq
12e32c  ldreq r0, [r4, #28]
12e32e  cmpeq r0, #0
12e330  beq.w 1305fa            ; all null -> cold
12e334  mov.w r9, #1            ; set1 #2
12e338  ...landmark guard...
```

The extra `b.n` at 12e30c is the only reason the pool is 0x30 early.

### 3.1 Root cause: PHI elimination splits `set1` into three blocks

`clang -mllvm -print-machineinstrs` (NDK r18b clang 7.0.2, exact build flags)
shows the IR is canonicalised to the **AND form with a 4-input phi** for every
spelling of the condition (`a||b||c`, `!(a==0&&b==0&&c==0)`, ternary,
if/else-if, goto, `init = d!=0`):

```
%269 = phi i32 [ %267, %263 ], [ 1, %259 ], [ 1, %255 ], [ 1, %251 ]
```

After *Eliminate PHI nodes for register allocation* this becomes **three
separate blocks**, each materialising the constant on its own edge:

```
bb.28: ldr a; cmp; bcc bb.30            (37.5% a==0 / 62.5% a!=0)
bb.29: %2511 = 1; b bb.35               (a != 0)
bb.30: ldr b; cmp; bcc bb.32
bb.31: %2511 = 1; b bb.35               (b != 0)
bb.32: ldr c; cmp; bcc bb.34
bb.33: %2511 = 1; b bb.35               (c != 0)
bb.34: %2511 = d != 0
bb.35: %17 = COPY %2511; ...landmark...
```

`BranchProbabilityInfo`'s **zero/pointer heuristic** (`calcZeroHeuristics`:
pointer compare against null → 5/8 non-null) gives bb.28 the 62.5 % edge to
bb.29, so `MachineBlockPlacement` lays bb.29 out as the fall-through.  bb.29
also receives the `ldr r5,[sp,#156]` status load, so it is
`mov; ldr; b` and cannot tail-merge with bb.31/bb.33 (`mov` only).  Hence two
`mov r9,#1` sites and the `b.n` water.

In the original, `otherLock` lives in **r5** (a low reg), so the landmark guard
must use **r6** for `Globals::status`; the shared `set1` is just `movs r5,#1`,
and the status pointer is materialised in the skip blocks (`mov r5,r6` /
`ldr r5,[sp,#156]`).  A single shared `set1` has three predecessors, so
`hasBetterLayoutPredecessor` defers it past the b/c checks → the natural
`cbnz/cbnz/cmp+beq` chain.

## 4. Hypothesis (supported, not proven): `Status *status` recovers the chain but not the water

Introducing a real local before the otherLock block

```cpp
Status *status = Globals::status;          // before `int otherLock;`
... status->getStation()->getIndex() ...   // in the landmark guard
```

does produce the **original's straight chain** in the real function:

```
12e312 mov  r1, r4
12e314 ldr  r0, [r1, #12]!
12e318 str  r1, [sp, #124]
12e31a ldr  r6, [r5, #0]
12e31c cbnz r0, 12e32a
12e31e ldr  r0, [r4, #56]
12e320 cbnz r0, 12e32a
12e322 ldr  r0, [r4, #28]
12e324 cmp  r0, #0
12e326 beq.w 1305f2
12e32a mov.w r9, #1
12e32e ldr  r0, [r4, #312]
```

but eliminating the 12e30c water exposes the next-highest water (12e2de), so
the pool moves to **12e2e0** (0x30 *earlier*), not 12e340.  Score drops to
**72.536928 %** (9900 B).  The landmark skip block (12e33c water) is still
missing, so the two changes do not compose.  This is a hypothesis for a future
pass: get both the straight chain *and* the landmark A||B skip block and the
pool should land at 12e340.

### 4.1 Why the landmark A\|\|B skip block will not materialise

The original's skip block exists because the C&&D check block is laid out
*between* the A\|\|B skip and the join, forcing `b.n 12e376`.  Ours merges the
A\|\|B skip with the C&&D skip at the join.  Every source spelling tried
(if/else-if with empty arms, `landmarkHit=false` arms, `goto`, `Status *status`
+ all of the above) is tail-merged by clang; the skip block never gets its own
`b`.  The original's two skip blocks differ only in *how r5 is reloaded*
(`mov r5,r6` vs `ldr r5,[sp,#156]`) — again a register-allocation artifact of
`otherLock` living in r5.

## 5. Standalone reproduction (control experiment)

Compiling the same condition shape in isolation with the same flags
(`clang++ -target armv7-none-linux-androideabi16 -march=armv7-a -mthumb
-mfpu=neon -mfloat-abi=softfp -std=c++11 -fPIC -Oz -g`) emits the original's
straight chain:

```
ldr r1,[r0]
cbnz r1,.LBB0_3
ldr r1,[r0,#8]
cbnz r1,.LBB0_3
ldr r1,[r0,#12]
cbz r1,.LBB0_7
.LBB0_3: movs r1,#1
...
.LBB0_7: ldr r1,[r0,#16]; cmp; it ne; movne r1,#1; b .LBB0_4
```

The PHI pass output is identical to the real function's (three `mov 1` blocks),
so the difference is purely `MachineBlockPlacement`'s layout-successor choice,
which in the real function is driven by the surrounding block frequencies and
by the extra status load in bb.29.  This proves the *source* shape is not the
obstacle; the *layout* is.

## 6. Variant sweep (all on the pristine baseline)

| variant | usize | raw % |
|---|---:|---:|
| **baseline** | 9908 | **74.426606** |
| OR plain `a!=null\|\|b!=null\|\|c!=null` | 9908 | 73.193807 |
| OR `__builtin_expect(...,1)` | 9908 | 74.397936 |
| OR `__builtin_expect(...,0)` | 9900 | 71.014076 |
| AND plain `a==0&&b==0&&c==0` | 9908 | 73.193807 |
| OR ternary | 9908 | 73.193807 |
| `bool anyLock` then int | 9908 | 73.193807 |
| if / else-if / else-if chain | 9908 | 73.193807 |
| `int otherLock = d!=0;` + if/else-if | 9848 | 71.418307 |
| `int otherLock = d!=0;` + OR | 9900 | 71.520896 |
| OR expect(a!=null,0) each | 9900 | 73.318996 |
| OR expect(a!=null,1) each | 9916 | 60.266514 |
| AND expect(a==null,1) first | 9900 | 73.318996 |
| AND expect(a==null,0) first | 9912 | 72.380679 |
| AND expect all null likely | 9900 | 71.014076 |
| OR expect all non-null unlikely | 9900 | 71.014076 |
| **`Status *status` before otherLock (expect form)** | 9900 | 72.536928 |
| `Status *status` + OR plain | 9900 | 72.995841 |
| `Status *status` + if/else-if landmark | 9900 | 72.536928 |
| `Status *status` + goto landmark | 9900 | 72.536928 |
| landmark `goto` (no status) | 9908 | 74.426606 (identical) |
| landmark if/else-if empty arms | 9908 | 74.426606 (identical) |
| landmark if/else-if `landmarkHit=false` arms | 9908 | 74.426606 (identical) |
| `notOtherLock`/`notTurretMode` hoisted above the if | 9912 | 72.266017 |
| inline `!turretMode && !otherLock` | 9880 | 72.463768 |
| remove landmark-loop `__builtin_expect`s | 9900 | 71.291386 |
| move `SetColor` after mission block | 9904 | 72.039002 |

None beats the baseline.  The best *faithful* shape (straight otherLock chain)
is 9900 B / 72.536928 %, exactly the same trade-off w6837 documented for the
inSight block: a locally-correct block re-lays-out the function and loses more
branch-target matches than it gains.

## 7. Residual accounting (baseline)

`difflib.SequenceMatcher` on the baseline with selective masking:

| masking | ratio |
|---|---:|
| none | 74.426606 |
| `[sp, #N]` → `[sp, #S]` | 76.089450 |
| `[sp, #N]` + `loc_XXXX` → wildcard | 84.805046 |

So ~1.7 points are pure stack-slot colouring (`mode` at sp#100 orig vs sp#112
ours, `turretMode`/`planetHit` swapped at 148/152) and ~8.7 more are
branch-target offsets.  The otherLock/landmark region accounts for ~40 of the
896 mismatched instructions; the rest is the known recompute / planet-tail /
enemy-tail block-placement residual documented in w6817/w6736.

## 8. Conclusion

The literal-pool displacement is **fully explained** and is a *water-list*
consequence of the otherLock if-conversion, not a rodata/TU/link-order issue:

1. `ARMConstantIslandPass` puts the pool at the highest water within 1020 B of
   the first CP user (12e064).
2. Ours has a water at 12e30c (the `b.n` produced by the split `set1` blocks);
   the original's first water is 12e33c (the landmark A\|\|B skip block).
3. The split is caused by PHI elimination + the null-pointer branch heuristic
   (62.5 % non-null) choosing the wrong layout successor, compounded by the
   status load landing in bb.29 and preventing tail-merge.
4. `Status *status = Globals::status;` before the otherLock block recovers the
   original's straight chain but moves the pool the wrong way (to 12e2e0); the
   landmark skip block is still needed to reach 12e340.

The baseline remains the best measured shape at **74.426606 % / 9908 B**.
No source change is committed; the gate is unchanged (`RATCHET PASS`, byte
4505, linked 4505).
