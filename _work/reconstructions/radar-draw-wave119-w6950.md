# Radar::draw pass (wave119, w6950) — planet/enemy/asteroid block order, 95 more variants, no source win

**Status:** research note, 2026-11-06. Worktree `wt/w6950`, branch
`wave119-w6950`, based on current `master` (`db1106935`, after
`radar-draw-wave119-w6947.md`). **No source change landed**; this commit
contains this note only. The target remains the pristine baseline.

Target `_ZN5Radar4drawEP6PlayerP3Hudi` in `src/game/weapons/Radar.cpp`.
Original 9908 B / 3492 normalised instructions; ours 9908 B / 3484.
Baseline unrounded `difflib.SequenceMatcher(None, on, un, autojunk=False).ratio()`
= **74.426606 %** (official `verify.py` display 74.4). 2596 instructions match.

Gate at start and end: `RATCHET PASS`, byte 4505, linked 4505, data 843/843,
imports 207, lint CLEAN, drift 0. No section/extab change (note-only commit).

## 1. Exact commands used

```sh
cd /home/debian/GalaxyOnFire2/wt/w6950
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6        # RATCHET PASS

NDK=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$NDK -d --start-address=0x12e050 --stop-address=0x130714 \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so > /dev/shm/w6950/orig.asm
$NDK -d --start-address=0x12e050 --stop-address=0x130714 \
  cmake-build-match/libgof2hdaa.so > /dev/shm/w6950/ours.asm
```

Per-variant loop (≈1 s each): edit `Radar.cpp`, `cmake --build
cmake-build-match --target link_so -j6`, then score the single symbol with a
scratch `difflib` harness that mirrors `tools/verify/verify.py`'s
`disassemble`/`reconcile` (identical 74.426606 % on the baseline). The harness
is `/dev/shm/w6950/try.py`; the variant drivers are
`/dev/shm/w6950/batch*.py`.

## 2. The block order is the whole story (confirmed from Ghidra/Thumb)

The original's layout, in address order:

```
12e2fc  otherLock #1          straight cbnz/cbnz/cmp+beq chain, otherLock in r5
12e314  landmark guard        &status in r6; 109/110 skip block at 12e338
12e376  otherLock #2
12e38a  planet section test
12e3d4  planet loop body
12e6ee  cold tail             prologue's !alienOrbit autopilot computation
12e748  planet-lock tail      ends `b.n 12ec24` (enemies)
12e79c  literal pool (5 words)
12e7a8  landmark loop setup
12e7d4  landmark loop body
12eb42  landmark tail         ends `b.w 12e376` (back to otherLock #2)
12ebc2  cold arms             otherLock #1, otherLock #2, missionBlocksPlanets,
                              planet-lock `field_0x14 == 0`
12ec24  enemies
12f460  gas clouds
12f960  asteroids
```

Ours (baseline):

```
12e2fc  otherLock #1          if-converted, otherLock in r9, &status in r5
12e338  landmark guard        no 109/110 skip block
12e378  otherLock #2
12e38c  planet section test
12e3d4  planet loop body
12e6d2  cold tail
12e72c  invented recompute    `b.n 12ebe8`
12e780  landmark loop setup
12e7b0  literal pool (5 words)
12e7bc  landmark loop body    (head rotated to 12eb00)
12eb42  landmark tail
12ebd4  planet-lock tail      (after the landmark block, not before it)
12ec36  enemies
```

So the two structural differences are:

1. **The planet-lock tail is in the wrong slot.** In the original it is the
   planet loop's fall-through successor, placed *before* the landmark loop;
   in ours the invented recompute occupies that slot and the planet-lock tail
   is placed *after* the landmark block. This is exactly the
   `radar-draw-wave119-w6736.md` §1.1 finding, re-confirmed from the Thumb
   block map.
2. **The recompute is load-bearing.** Deleting it makes the planet-lock tail
   the loop's fall-through successor (matching the original) but its last
   block then falls straight through into the enemies, so clang moves the
   whole landmark loop out of line to `12f45a` (`norec` = 9824 B /
   63.746398 %). The original avoids this because its planet-lock tail ends
   with `b.n 12ec24` (the `field_0x14 == 0` arm is placed *after* the landmark
   block at `12ec1c`), so the landmark loop fits in the gap.

The `b.n`/cold-arm placement is a chicken-and-egg with
`MachineBlockPlacement`/`BranchFolder`: in the original the two arms of
`if (field_0x14 == 0) { planetLockTimer = 0; field_0x18 = 0; } else {
planetLockTimer = 0; }` are *not* adjacent and are not merged; in every
`norec` build clang hoists the common `planetLockTimer = 0` before the branch
and merges them, so the tail falls through to the enemies. Every source
spelling tried (swapped arms, nested `if`, `goto enemies`, `__builtin_expect`
on the `field_0x14` test, `__builtin_expect` on the outer planet-hit test)
leaves the merge in place or re-rolls the layout worse.

**Late finding (best faithful-layout variant).** Swapping the two stores in
the `field_0x14 == 0` arm (`field_0x18 = 0; planetLockTimer = 0;` instead of
`planetLockTimer = 0; field_0x18 = 0;`) prevents the hoist/merge: on the
`norec` shape the planet-lock tail becomes the planet loop's fall-through
successor and ends in a branch to the enemies, so the landmark loop stays
inline. That is `norec + swapped stores` = **9840 B / 70.733813 %**, the best
faithful-layout score found, though still 3.7 points below the baseline. Its
call multiset is exact except 5 `blx r5` (landmark `String` tail-merge), 2
`blx r2` (asteroid dead-check), `String::operator=` 5 vs 6, `isDead` 16 vs 17,
`isDying` 12 vs 13, and one `__aeabi_f2lz` import. Layering the other faithful
fixes on top only lowers the score (`+lockLabel` 9760 B / 68.918724 %,
`+asteroid double dead-check` 9860 B / 69.943941 %, both 9764 B /
63.440705 % with an `expect` on the enemy tail).

## 3. Variant sweep (all on the pristine baseline unless stated)

`usize` is the emitted function size; `raw %` is the unrounded ratio.

| variant | usize | raw % |
|---|---:|---:|
| **baseline** | 9908 | **74.426606** |
| `swap_else` (`else if (==0)` / `else` arms swapped) | 9908 | 74.426606 |
| `nested_else` (nested `if` instead of `else if`) | 9908 | 74.426606 |
| `goto_else` (`goto enemies` in the `else` arm) | 9908 | 74.426606 |
| `goto_swap` (swapped arms + `goto`) | 9908 | 74.426606 |
| `else_exp1` (`expect(field_0x14 == 0, 1)`) | 9908 | 74.426606 |
| `elseif_1` (`expect(field_0x14 == 0, 1)` on else-if) | 9908 | 74.426606 |
| `f14_1` (`expect(field_0x14 != 0, 1)`) | 9908 | 74.426606 |
| `dockp_1` (`expect(field_0x1a8, 1)`) | 9908 | 74.426606 |
| `cond_or` (`!(a \|\| b \|\| c)`) | 9908 | 74.426606 |
| `cond_nest` (nested enemy-tail `if`s) | 9908 | 74.426606 |
| `tail_order` (`!landmarkHit && !turretMode`) | 9908 | 74.426606 |
| `tail_or` (`!(turretMode \|\| landmarkHit)`) | 9908 | 74.426606 |
| `ph_order` (`!planetHit && !otherLock && !turretMode`) | 9908 | 74.426606 |
| `ph_or` | 9908 | 74.426606 |
| `decl_a` (`textOffset` first in planet loop) | 9908 | 74.426606 |
| `decl_c` (`textOffset` before `warpGate`) | 9908 | 74.426606 |
| `marked_0` (`int marked = 0`) | 9908 | 74.426606 |
| `planet_loop_int` (`int i` in planet loop) | 9908 | 74.426606 |
| `asteroid_loop_int` (`int i` in asteroid loop) | 9908 | 74.426606 |
| `gas_loop_int` (`int` bound in gas loop) | 9908 | 74.426606 |
| `guard_nest` (nested landmark guard) | 9908 | 74.426606 |
| `guard_local2` (`Status *status` after the guard) | 9908 | 74.426606 |
| `ins_a`/`ins_b`/`ins_c`/`ins_d` (float crosshair locals, bounds-first) | 9908 | 74.426606 |
| `ll_member` (`this->lockLabel` in the `i==0` arm) | 9908 | 74.029509 |
| `ll_ref` (reference alias) | 9908 | 74.029509 |
| `decl_b` (`currentStation` first) | 9908 | 74.397936 |
| `locked_1` (`expect(!missionBlocksPlanets && timer >= 1, 1)`) | 9908 | 74.408602 |
| `frame_0` (`expect(field_0x18 != 0 && ..., 0)`) | 9908 | 73.785295 |
| `enemy_cond2` (reordered `isDying`/`field_0x74`/`isActive`) | 9908 | 73.910550 |
| `dockp_0` | 9912 | 73.641966 |
| `frame_1` | 9916 | 73.660075 |
| `else_exp0` (`expect(field_0x14 == 0, 0)`) | 9916 | 72.484953 |
| `elseif_0` | 9916 | 72.484953 |
| `status_flat` (`Status *status` before otherLock) | 9900 | 72.536928 |
| `status_nest` (`Status *status` + nested guard) | 9900 | 72.536928 |
| `enemy_cond1` (reordered `field_0x74`/`isDying`) | 9912 | 72.542276 |
| `timer_1` (`expect(timer > field_0x1b8, 1)`) | 9908 | 71.788991 |
| `tail_guard_1` (`expect(!turretMode && !landmarkHit, 1)`) | 9904 | 71.676466 |
| `mbp_0` (`expect(missionBlocksPlanets, 0)`) | 9904 | 71.418328 |
| `decl_d` (`warpGate` first) | 9904 | 70.931570 |
| `decl_e` | 9904 | 71.492173 |
| `guard_local` (`Status *status` inside the guard) | 9856 | 70.212156 |
| `planethit_1` (`expect(planetHit && ..., 1)`) | 9928 | 70.200573 |
| `landmark_int` (`int landmarkHit`) | 9896 | 70.508961 |
| `planet_int` (`int planetHit`) | 9896 | 71.887550 |
| `f14_0` (`expect(field_0x14 != 0, 0)`) | 9932 | 66.580793 |
| `cond_exp1` (`expect(!isDockingToAsteroid() && ..., 1)`) | 9892 | 67.432510 |
| `cond_exp_dock` | 9872 | 61.888313 |
| `cond_exp_sub` | 9876 | 62.063013 |
| `cond_swap` | 9888 | 65.422707 |
| `tail_guard_0` | 9924 | 61.650694 |
| `planethit_0` | 9904 | 62.523271 |
| `timer_0` | 9920 | 61.961346 |
| `locked_0` | 9920 | 65.215523 |
| `mbp_1` | 9916 | 62.065015 |
| `en_exp1` (`expect(scannerAvailable && enemies, 1)`) | 9864 | 64.358569 |
| `en_exp0` | 10068 | 44.819277 |
| `en2_exp` (`expect(enemies != nullptr, 1)`) | 9860 | 59.812815 |
| `lm_exp` (`expect(landmarks != nullptr, 1)`) | 9820 | 66.312311 |
| `gc_exp` (`expect(gasClouds != nullptr, 1)`) | 9892 | 73.324724 |
| `ll_assign` (`String label; label = ...`) | 9948 | 65.999142 |
| `asteroid_two_checks` (`if (hasCargo && dead) {} else if (dead) continue;`) | 9928 | 71.860232 |
| `locklabel+ast` | 9928 | 71.465369 |
| `cond1+ast` | 9912 | 66.877062 |
| `cond1+ll` | 9892 | 67.039747 |
| `cond1+ast+ll` | 9912 | 66.485092 |
| `ins_float` (faithful distance-first, float crosshair) | 9864 | 66.685813 |
| `ins_int` (faithful distance-first, int crosshair) | 9864 | 66.743251 |
| `ins_float_xy` (distance-first, x/y interleaved) | 9944 | 62.805404 |
| `ins_nest` (distance-first nested) | 9932 | 62.902299 |
| `ins_inline` (distance-first, inline `crosshairPos`) | 9924 | 62.201782 |
| `insf_cond1` | 9864 | 66.475233 |
| `insf_ast` | 9884 | 66.245876 |
| `insf_ll` | 9896 | 67.459862 |
| `insf_ast_ll` | 9916 | 67.048546 |
| `norec` (delete the recompute) | 9824 | 63.746398 |
| `norec_swap_else` | 9824 | 63.746398 |
| `norec_nested_else` | 9824 | 63.746398 |
| `norec_goto_else` | 9824 | 63.746398 |
| `norec_goto_swap` | 9824 | 63.746398 |
| `norec_goto_if` | 9824 | 63.746398 |
| `norec_else_exp1` | 9824 | 63.746398 |
| `norec_else_exp0` | 9828 | 63.746398 |
| `norec_else_swap_exp` | 9828 | 63.746398 |
| `swapstores` (baseline + swapped `field_0x14 == 0` stores) | 9880 | 71.076040 |
| `swapstores2` (same, alternate spelling) | 9880 | 71.076040 |
| `norec_swapstores` (delete recompute + swapped stores) | 9840 | 70.733813 |
| `norec_swapstores2` (same, alternate spelling) | 9840 | 70.733813 |
| `ns` (= `norec_swapstores`) | 9840 | 70.733813 |
| `ns_ast` (+ asteroid double dead-check) | 9860 | 69.943941 |
| `ns_ll` (+ `this->lockLabel` in the `i==0` arm) | 9760 | 68.918724 |
| `ns_ast_ll` (+ both) | 9780 | 68.348955 |
| `ns_cond1` (+ `expect` on the enemy tail) | 9820 | 65.599769 |
| `ns_ast_ll_cond1` (+ all three) | 9764 | 63.440705 |
| `norec+ast` | 9844 | 63.135166 |
| `norec+locklabel` | 9756 | 62.314026 |
| `norec+cond1` | 9800 | 53.543080 |
| `norec+cond1+ast` | 9820 | 53.229527 |
| `norec+cond1+ll` | 9736 | 52.249385 |
| `norec_en_exp1` | 9764 | 55.036855 |
| `norec_en_exp0` | 9984 | 43.780245 |
| `norec_lm_exp` | 9740 | 64.643838 |
| `norec_gc_exp` | 9812 | 64.214626 |
| `lm_after_planet` (landmark block moved after the planet block) | 9816 | 56.392496 |
| `lm_after_planet_norec` | 9728 | 67.448595 |
| `lm_norec_cond1` | 9712 | 62.886149 |
| `lm_norec_ast` | 9748 | 66.078403 |
| `lm_norec_cond1_ast` | 9732 | 62.764416 |

No variant beats **74.426606 % / 9908 B**.

## 4. Why the faithful shape cannot be tuned back

The faithful source (distance-first `inSight`, no recompute, asteroid
`isDead`/`isDying` twice, `this->lockLabel` in the `i==0` arm) is
**9772 B / ~61.9 %** (w6706) and reproduces the original's call multiset
exactly (344 calls). But it is 136 B shorter than the original, and every
block after the planet loop shifts by hundreds of bytes. The baseline's
invented recompute (168 B) is exactly the anchor that keeps the landmark loop
inline and the enemy/asteroid sections within ~0x30 of the original.

The remaining residual is:
* **~404 branch mismatches** from the block-layout drift (planet-lock tail
  slot, landmark-loop rotation, otherLock cold arm at `1305fa` vs `12ebc2`).
* **~490 non-branch mismatches**, dominated by the `inSight` block (22+25),
  the invented recompute (24), the otherLock chain (12), and the
  `i==1||i==3` fold (4).
* **Stack-slot colouring**: `mode` is `[sp,#116]` (orig `[sp,#100]`),
  `turretMode`/`planetHit` are swapped at 148/152, and `hud` is
  `[sp,#88]` (orig `[sp,#84]`). The root cause is `vpush {d8-d11}` (ours)
  vs `{d8-d13}` (orig): the original caches `crosshairPos.x/y` in
  `s24`/`s26` across the `VectorLength` call. The faithful distance-first
  `inSight` restores `vpush {d8-d13}` and moves `mode` to `[sp,#104]`
  (closer), but re-rolls the whole function (66.7 %).

## 5. Conclusion

`Radar::draw` remains a very strong `difflib` local optimum at
**74.426606 % / 9908 B**. The blocker is the planet-lock tail's block slot:
the original places it before the landmark loop (its `field_0x14 == 0` arm
after the landmark block), while every source shape we can write either
keeps the invented recompute (which scores best) or lets clang merge the
`field_0x14 == 0` arms and fall through to the enemies (which moves the
landmark loop out of line, -10.7 points).

A future pass that beats 74.426606 % needs one of:

(a) a source shape that keeps the landmark loop inline *and* deletes the
    recompute — i.e. prevents the `planetLockTimer = 0` hoist/merge without
    re-rolling the rest of the function;
(b) a way to get `vpush {d8-d13}` (the `s24`/`s26` crosshair cache) without
    changing the `inSight` evaluation order, which would fix the
    `mode`/`turretMode`/`planetHit` stack slots (~1.7 points of pure slot
    colouring, plus the branch targets they feed);
(c) a `Status *status` shape that gives the original's straight otherLock
    chain *and* the `12e33c` landmark skip block, which would move the
    literal pool from `12e310` to `12e340` (w6910 §4).

The baseline remains the best measured shape at **74.426606 % / 9908 B**.
The best faithful-layout shape is `norec + swapped stores` at
**9840 B / 70.733813 %**; it fixes the block order but loses 3.7 points to
the baseline's favourable (invented) code placement. No source change is
committed; the gate is unchanged (`RATCHET PASS`, byte 4505, linked 4505,
data 843/843, imports 207, lint CLEAN, drift 0).
