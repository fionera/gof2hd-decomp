# `Level::createShip` — wave 134 (w7761): the original computes all three
# integer coordinates *before* `setEmpData`; master sinks them, and neither
# `Vector pos` hoisting nor all-int locals restores the 73.4 % allocation

Status: **note-only** (no source change landed; gate stays byte 4506 /
linked 4506, `.ARM.extab` 156,312 B exact).  Worktree `wt/w7761`, base master
`ec0acea1e` (wave-133 snapshot).  Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(Thumb entry `0xc0215`, 1007 insns).

Target `_ZN5Level10createShipEiiiP8Waypointbb` (`Level::createShip`),
original 2764 B @ `0xc0214`.

| | baseline (HEAD `ec0acea1e`) |
|---|---:|
| function score | **70.6 %** (1007 vs 1005 normalised insns) |
| function size | 2764 B |
| `.ARM.extab` | 156,312 B (exact; 336 B / 57-row / ttype 0xff record) |

## 0. Summary (new results this pass)

1. **The original keeps all three *integer* coordinates live across
   `setEmpData`.**  Disassembly `0xc03c8..0xc03e4` (before the call at
   `0xc03ee`):
   ```
   c03c8 ldr r2,[sp,#48]  ; wy
   c03ca ldr r3,[sp,#32]  ; jitterY
   c03cc ldr r0,[sp,#44]  ; wz
   c03ce ldr r1,[sp,#36]  ; jitterZ
   c03d0 add r2,r3        ; wy+jitterY
   c03d2 ldr r3,[sp,#40]  ; wx
   c03d4 ldr r4,[sp,#28]  ; jitterX
   c03d6 add r0,r1        ; wz+jitterZ
   c03d8 movw r1,#20000
   c03dc sub.w fp,r0,r1   ; z -> fp
   c03e0 add r3,r4        ; wx+jitterX
   c03e2 subs r4,r2,r1    ; y -> r4
   c03e4 sub.w r8,r3,r1   ; x -> r8
   ```
   The float conversions happen later, once per branch
   (`vcvt.f32.s32` at `0xc040c..0xc0422` in the capital branch and
   `0xc06c0..0xc06d8` in the fighter branch), i.e. the source holds **ints**
   and converts at the `PlayerFighter`/`PlayerFixedObject` constructor call.
   Master instead sinks the arithmetic below the call into the status-check
   block (only `y` is computed there; `x`/`z` are re-derived from the spilled
   slots), which is the live-range change that moves Greedy RA off
   `type -> fp`, `shipClass -> r5/sl`.
   This is a sharper statement than w7737's "the try computes y/z before the
   call": the *original* computes **x, y and z** before the call.
2. **`Vector pos` hoisted above `setEmpData` is not the lever.**
   `int z0,y0,x0; Vector pos={(float)x0,(float)y0,(float)z0};
   player->setEmpData(...)` used by both branches gives **2776 B, 57.5 %**
   (1007/1003 insns), `.ARM.extab` exact.  The extra `Vector` forces the float
   conversions early and grows the function by 12 B.
3. **All-int locals are faithful but wrong-sized.**  `int z,y,x` with
   `(float)` casts at the constructor: **2772 B, 61.5 %** (1007/994 insns),
   `.ARM.extab` exact — reproduces w7709's number, and confirms the int form
   alone does not keep the arithmetic above the call.
4. **Branch-weight hints on the outer branches do not help** (all measured on
   the real TU, `.ARM.extab` shown):
   | variant | score | size | insns | extab |
   |---|---:|---:|---:|---:|
   | `__builtin_expect(shipClass == 0, 1)` | 49.1 | 2752 | 1007/990 | 156,316 |
   | `__builtin_expect(shipClass == 1, 1)` | 35.1 | 2728 | 1007/1004 | 156,316 |
   | `__builtin_expect(Globals::status == nullptr, 0)` | 59.5 | 2760 | 1007/1004 | 156,312 |
   | `__builtin_expect(missionPtr != 1 && missionPtr != 23, 1)` | 70.6 | 2764 | 1007/1005 | 156,312 (neutral) |
   | `switch (__builtin_expect(race, 0))` | 42.0 | 2760 | 1007/997 | 156,312 |

5. **`try` around `new Player` and memory fences do not help** (new this
   pass):
   | variant | score | size | insns | extab |
   |---|---:|---:|---:|---:|
   | `try { player = new Player(...); } catch (...) { return nullptr; }` | 62.1 | 2788 | 1007/1013 | 156,328 |
   | `try { ... } catch (...) { throw; }` | 39.8 | 2796 | 1007/1016 | 156,340 |
   | `try { ... } catch (...) { }` | 59.5 | 2808 | 1007/1018 | 156,328 |
   | `__sync_synchronize()` between coords and call | 67.8 | 2768 | 1007/1006 | 156,312 |
   | `__atomic_thread_fence(__ATOMIC_SEQ_CST)` | 67.8 | 2768 | 1007/1006 | 156,312 |
   | fence + all-int coords | 61.3 | 2776 | 1007/995 | 156,312 |

   The `try`-around-`new` forms all grow `.ARM.extab` (156,328-156,340), so
   they cannot land on size grounds; the fences keep the extab exact but score
   below the 70.6 baseline.

## 1. Why a non-EH scheduling boundary is the remaining lead

w7737 established that the `try` body (73.4 %) is good because `EH_LABEL` is a
`MachineInstr::isPosition()` scheduling boundary; the coordinate arithmetic
cannot be sunk across it.  `isSchedulingBoundary` also returns true for
`DBG_LABEL`/`CFI_INSTRUCTION`/`GC_LABEL`, none of which plain C++ can emit, and
for terminators / inline asm (banned).  A `volatile` local store is not a
scheduling boundary and w7709 measured `volatile x/y/z` at 59.4 % / 2840 B.
The untried forms are:

* an `alloca`/VLA between the coordinate block and `setEmpData` (SP-modifying,
  survives `-Oz`) — this adds code, so it can only pay if it simultaneously
  removes the 16-byte `Globals::status == nullptr` guard that w7737 shows is
  not in the original source;
* a `try` around `new Player` (already an invoke) instead of around
  `setEmpData`, which might add the mid-function landing-pad block without
  changing the `setEmpData` call-site row (w7737 open item 2).

## 2. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w7761/score.py _ZN5Level10createShipEiiiP8Waypointbb
```

Scratch (not committed): `/dev/shm/w7761/` — `score.py`, `cs_test.py`,
`cs_batch.py`, `Level.orig.cpp`, `createship.show`.
