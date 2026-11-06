# Radar::draw pass (wave119, w6947) — the `i==1||i==3` arm is a switch, and the layout is the blocker

**Status:** research note, 2026-11-06. Worktree `wt/w6947`, branch
`wave119-w6947`, based on current `master` (`7a031742d`, after
`radar-draw-wave119-w6913.md`). **No source change landed**; this commit
contains this note only. The target remains the pristine baseline.

Target `_ZN5Radar4drawEP6PlayerP3Hudi` in `src/game/weapons/Radar.cpp`.
Original 9908 B / 3492 normalised instructions; ours 9908 B / 3484.
Baseline unrounded `difflib.SequenceMatcher(None, on, un, autojunk=False).ratio()`
= **74.426606 %** (official `verify.py` display 74.4). 2596 instructions match,
404 branch + 492 non-branch instructions are mismatched on the original side.

Gate at start and end: `RATCHET PASS`, byte 4505, linked 4505, data 843/843,
imports 207, lint CLEAN, drift 0. No section/extab change (note-only commit).

## 1. Exact commands used

```sh
cd /home/debian/GalaxyOnFire2/wt/w6947
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6        # RATCHET PASS

NDK=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$NDK -d --start-address=0x12e050 --stop-address=0x130740 \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so > /dev/shm/w6947/orig.asm
$NDK -d --start-address=0x12e050 --stop-address=0x130740 \
  cmake-build-match/libgof2hdaa.so > /dev/shm/w6947/ours.asm
```

Per-variant loop (≈1 s each): edit `Radar.cpp`, `cmake --build
cmake-build-match --target link_so -j6`, then score the single symbol with a
scratch `difflib` harness that mirrors `tools/verify/verify.py`'s
`disassemble`/`reconcile` (identical 74.426606 % on the baseline). The harness
and the variant generator live in `/dev/shm/w6947/score.py` and
`/dev/shm/w6947/sweep.py`.

## 2. New finding: the landmark `else if (i==1||i==3)` arm is a **switch** in the original

The baseline source spells the arm as

```cpp
} else if (__builtin_expect((i & 0x7fffffff) == 1 || (i & 0x7fffffff) == 3, 0)) {
    Globals::Canvas->DrawImage2D(i == 1 ? this->image_0xbc : this->image_0xb8,
                                 this->screenX, this->screenY, 17, 68);
}
```

and clang 7.0.2 folds it to `(i|2)==3` + an if-converted image select
(w6837 §3).  The original instead has the **unfolded** two-compare form:

```
12e9ea: bic.w  r0, sl, #0x80000000     ; i & 0x7fffffff
12e9ee: cmp    r0, #1
12e9f0: beq.n  12e9fc                  ; i == 1 -> image_0xbc
12e9f2: cmp    r0, #3
12e9f4: bne.n  12ea18                  ; i != 3  -> continue
12e9f6: ldr.w  r1, [r4, #184]          ; i == 3 -> image_0xb8
12e9fa: b.n    12ea00
12e9fc: ldr.w  r1, [r4, #188]          ; i == 1 -> image_0xbc
12ea00: <shared DrawImage2D>
```

Two source shapes reproduce this **exactly** (same instruction sequence, same
`bic`/`cmp 1`/`cmp 3` order):

* a `switch (i & 0x7fffffff)` with `case 1` / `case 3` — but clang lowers the
  cases **descending** (`cmp 3` first), so it is *not* the original order;
* an `if/else-if` chain on `(i & 0x7fffffff) == 1` / `== 3` — this lowers
  **ascending** (`cmp 1` first) and matches the original's order exactly.

The `if/else-if` form (`sw2` below) is the faithful source shape.  It is 8 B
shorter than the baseline (9900 B) and has **8 fewer non-branch mismatches**
(484 vs 492) but **37 more branch mismatches** (441 vs 404), because the
`i==1` arm is placed far from the `else-if` arm (targets `12eaee`/`12eae8`,
forcing `beq.w`/`bne.w` instead of the original's `beq.n`/`bne.n`).  Net score
73.211982 %.  The layout, not the arm, is the blocker.

## 3. New finding: the planet-section recompute can be made 1 instruction shorter

The current recompute holds `autoPilotTarget` in a local:

```cpp
int autoPilotTarget = this->level->getPlayer()->getAutoPilotTarget();
...
onPlanetCourse = ... && autoPilotTarget == ((Array<int> *) this->planetTargets)->data()[stationIndex];
```

Inlining the call (`rec_inline`) produces 9908 B / 3485 instructions and
**74.387272 %** — 0.04 points *below* the baseline, because the 4-byte-shorter
recompute moves the literal pool from `12e7b0` to `12e7a8`.  Using
`getStarSystem()->getPlanetTargets()` instead of `this->planetTargets`
(`rec_gs`) gives 9916 B / 72.646511 %.  Making the recompute textually
identical to the prologue's else-branch (`rec_identical`) gives 9928 B /
72.539751 %.  Dropping the `field_0x1a8` store (`rec_no_store`) gives 9876 B /
65.766413 %.  None wins.

## 4. New finding: the `while` form un-rotates the landmark loop but is semantically wrong

Spelling the landmark loop as

```cpp
int i = 0;
while (i < this->landmarks->size()) { ... ++i; }
```

moves the loop head **inline** before the body (`12e7b4` head, `12e7dc` body in
the `norec+store+while` build), which is the original's shape (`12e7d4` head,
`12e7e0` body).  But `continue` in a `while` jumps to the condition, skipping
`++i`, so the loop is an infinite loop; the score is 68.801149 % (baseline) /
67.898383 % (`norec+store`).  A semantically correct `while` needs the
increment at the top or a `goto`, which reintroduces the separate increment
block and the rotation.  `goto nextLandmark` at the bottom of a `for` body is
**identical** to the baseline (74.426606 %).

## 5. Variant sweep (all on the pristine baseline unless stated)

| variant | usize | raw % |
|---|---:|---:|
| **baseline** | 9908 | **74.426606** |
| `xy2` (merged `field_0x11f` arm, no init) | 9908 | 74.426606 (identical) |
| `ph` (`planetHit` hoisted before the planet `if`) | 9908 | 74.426606 (identical) |
| `loop_goto` (`continue` → `goto next`) | 9908 | 74.426606 (identical) |
| `mode_local` (`int drawMode = mode`) | 9908 | 74.426606 (identical) |
| `sw8` (`||` condition + if/else image select) | 9908 | 74.426606 (identical) |
| `rec_inline` (inline `getAutoPilotTarget`) | 9908 | 74.387272 |
| `xy` (init `x`/`y`, merged arm) | 9908 | 74.129282 |
| `xy3` (init `x`/`y`, no merged arm) | 9908 | 74.129282 |
| `sw2` (`if/else-if` on masked `i`) | 9900 | 73.211982 |
| `sw4` (case 3 first, `if/else-if`) | 9900 | 73.183317 |
| `sw3` (switch, case 3 first) | 9916 | 72.943537 |
| `sw` / `sw5` / `sw6` / `sw7` (switch / variants) | 9916 | 72.886214 |
| `sw3+store` | 9924 | 72.969489 |
| `sw+rec_gs` | 9924 | 72.883541 |
| `sw3+rec_gs` | 9924 | 72.940839 |
| `sw2+rec_gs` | 9908 | 71.977077 |
| `sw+store` | 9924 | 72.912190 |
| `sw2+store` | 9908 | 72.005731 |
| `sw2+xy` | 9900 | 72.914875 |
| `sw3+xy` | 9916 | 72.761141 |
| `sw+xy` | 9916 | 72.703826 |
| `sw2+ol_status` | 9900 | 73.211982 |
| `sw+ol_status` | 9916 | 72.886214 |
| `sw3+ol_status` | 9916 | 72.943537 |
| `sw2+ol_or4` | 9924 | 70.129125 |
| `sw+ol_or4` | 9924 | 70.057307 |
| `sw2+lock_locals` | 9904 | 70.584862 |
| `sw2+plock_locals` | 9892 | 69.780771 |
| `hoist` (`if (i<=1)` block moved after the `else-if`, `onScreenHere` local) | 9916 | 72.808023 |
| `hoist+sw2` | 9924 | 72.912190 |
| `sw2+rec_none+store` | 9768 | 69.582792 |
| `sw+rec_none+store` | 9768 | 69.341801 |
| `rec_gs` (recompute via `getStarSystem()->getPlanetTargets`) | 9916 | 72.646511 |
| `rec_identical` (recompute = prologue else-branch) | 9928 | 72.539751 |
| `rec_no_store` (drop `field_0x1a8` store) | 9876 | 65.766413 |
| `or3_else` / `or3_ternary` | 9908 | 73.193807 |
| `or3_else_exp` | 9900 | 71.014076 |
| `or3_else_exp1` | 9924 | 60.575684 |
| `or3_plus4` | 9912 | 71.893364 |
| `skip_hoist` (guard `skipLandmarks` before the `if`) | 9916 | 70.286533 |
| `lock_locals` (hoist two `isDockingTo*` calls) | 9908 | 71.016779 |
| `plock_locals` (same for the planet lock) | 9900 | 70.679278 |
| `lock_locals+plock_locals` | 9892 | 69.008028 |
| `cond_a` (`i != 3` first) | 9904 | 72.870662 |
| `cond_b` (`!landmarkHit && notTurretMode && notOtherLock && i != 3`) | 9904 | 72.669917 |
| `cond_c` (`notTurretMode && notOtherLock && !landmarkHit && i != 3`) | 9904 | 70.333142 |
| `loop_while` (baseline) | 9804 | 68.801149 |
| `norec+store+loop_while` | 9724 | 67.898383 |
| `norec+loop_while` | 9728 | 61.123141 |
| `norec+store+loop_exp1` | 9928 | 46.729506 |
| `norec+store+loop_exp0` | 9804 | 60.697809 |
| `norec+store+ol_or4+loop_exp1` | 9908 | 38.490134 |
| `norec+store+ol_or4+loop_exp0` | 9812 | 58.192823 |
| `norec+store+ol_or4+ins_dist` | 9824 | 60.734341 |
| `norec+store+loop_goto` | 9840 | 70.733813 |

(`norec` = delete the planet-section recompute; `store` = swap
`planetLockTimer`/`field_0x18` in the `field_0x14 == 0` arm; `ol_or4` =
`otherLock = a||b||c||d`; `ol_status` = `Status *status` before `otherLock`;
`ins_dist` = distance-first float `inSight`; `lock_locals`/`plock_locals` =
hoist the two `isDockingTo*` calls into locals.)

## 6. Residual accounting (baseline, unchanged from w6910/w6913)

| masking | ratio |
|---|---:|
| none | 74.426606 |
| `[sp, #N]` → `[sp, #S]` | 76.089450 |
| `[sp, #N]` + `loc_XXXX` → wildcard | 84.805046 |

So ~1.7 points are pure stack-slot colouring (`mode` at sp#100 orig vs sp#112
ours, `turretMode`/`planetHit` swapped at 148/152) and ~8.7 more are
branch-target offsets.  Branch mismatches: 404 of 830 original branches.

The frame difference is not a source-order problem: `ph`, `mode_local` and
`lock_locals` all leave the stack slots unchanged.  The original pushes
`{d8-d13}` (the `inSight` block caches float `crosshairPos` in `s26`/`s24`);
the baseline pushes `{d8-d11}`.  The faithful distance-first `inSight` restores
`{d8-d13}` but re-lays-out the function (w6837 §4, 66.7 %).

## 7. Conclusion

The function is a very strong `difflib` local optimum.  The remaining residual
is **block placement**, not a wrong statement:

1. The landmark `else if (i==1||i==3)` arm is a **switch / if-else-if** in the
   original (`bic` + `cmp 1` + `cmp 3`), not the folded `(i|2)==3` form.  The
   faithful `if/else-if` form reproduces the arm byte-for-byte but places the
   `i==1` arm far away, so the branch encodings become `.w` and the overall
   score drops to 73.21 %.
2. The planet-section recompute is invented (the original has one
   `getStationEnumIndex` and one `strb field_0x1a8`).  Deleting it (`norec`)
   puts the planet-lock tail before the landmark loop, but the tail's last
   block then falls through to the enemies (no `b.n` water), so the loop moves
   out of line.  The `store` reorder restores the `b.n` and the loop order, but
   the loop head is still rotated and the enemies section shifts by -0x62.
3. The baseline's 33-instruction recompute happens to compensate for the
   other size deltas and keeps the enemies section at +0x12, which is why it
   beats every faithful shape.

A future pass that beats 74.426606 % needs either (a) the `if/else-if` arm
*and* a layout that keeps the `i==1` arm within 16-bit branch range, or (b) a
source shape that deletes the recompute while keeping the enemies section at
`12ec24`.

The baseline remains the best measured shape at **74.426606 % / 9908 B**.
No source change is committed; the gate is unchanged (`RATCHET PASS`, byte
4505, linked 4505, data 843/843, imports 207, lint CLEAN, drift 0).
