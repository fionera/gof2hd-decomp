# HangarWindow::render — size-exact `cbz` on the `tab == 0` edge (wave 163, w10916)

Worktree `wt/w10916`, base `e51db2fc7` (wave 162).  Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Target: `_ZN12HangarWindow6renderEv`, 9,700 B, `.ARM.extab` record 948 B / 156 rows.

## Result (landed)

| metric | before (wave 162) | after |
|---|---:|---:|
| raw same-address diff | 2,514 | **1,979** |
| instruction-index alignment | 485 | **551** |
| function size | 9,700 | **9,700** |
| its `.ARM.extab` record diff | 151 | **676** |
| total `.ARM.extab` diff | 725 | **1,250** |
| `.text` diff | 85,042 | **84,507** |
| `.rel.plt` diff | 0 | **0** (byte-identical) |
| all section sizes | exact | **exact** |

`check` green: byte_exact 4507/4524, linked_exact 4507/4524, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.

The landed shape emits the original's exact dispatch:

```
149b1a: cmp r0, #0
149b1c: beq.n 149bf0        ; !isShip
149b1e: ldr r4, [sp, #136]  ; (this, for the volatile read)
149b20: ldr.w r0, [r4, #180]
149b24: ldr r0, [sp, #108]  ; tab
149b26: ldr r5, [sp, #132]
149b28: cbz r0, 149b96      ; tab == 0 -> the tab == 1 check   <-- the cbz
149b2a: cmp r0, #3
149b2c: bne.n 149b44        ; tab != 3 -> buttons[1]
```

The original is `ldr r0,[sp,#108]; ldrd r5,r4,[sp,#132]; cbz; cmp #3; bne`.
The extra `ldr r4`/`ldr.w` pair is the volatile `scrollOffset` read; the
`ldrd` is split into two `ldr`s, so the dispatch is 8 B longer than the
original's but the function stays 9,700 B (the `cbz` saves the 4 B that the
old `cmp;beq.w` form spent, and the rest is compensated by the shorter
`getShip` operand order).

## How the `cbz` is obtained

The w10816 note established that JumpThreading folds the `tab == 0` edge
through the `tab == 1 && upgradeMode` check to the merge because the check
block's duplication cost is 5 (`getJumpThreadDuplicationCost`, threshold 6).
The block is:

```
1823: %gep = getelementptr this, 84
      %load = load upgradeMode
      %ne = icmp ne %load, 0
      %eq = icmp eq tab, 1
      %and = and %ne, %eq
      br %and
```

`getJumpThreadDuplicationCost` counts `getelementptr+load+icmp+icmp+and = 5`;
a non-intrinsic call costs 4, an intrinsic call costs 2 (LLVM 7
`JumpThreading.cpp`).  To stop the fold we need cost > 6.

**`__builtin_expect` does not work**: `LowerExpectIntrinsic` runs before
JumpThreading (the `!prof` metadata is already on the branch), so it adds no
IR cost.  **`llvm.lifetime` markers are not emitted** for unused locals at
`-Oz` (SROA removes them).

**`__builtin_assume` works.**  `llvm.assume` is lowered in CodeGenPrepare,
after JumpThreading, so it survives and costs 2 each.  Two assumes raise the
block cost to 9 > 6 and stop the fold.  The `tab >= 0` assume is required:
without it the second assume is CSE'd / the block cost stays <= 6.

The `tab >= 0` assume alone can make the backend emit a `bic r0, r0,
#0x80000000` before the `tab == 3` compare (killing the `cbz`).  The
`volatile` read of `this->scrollOffset` at the top of the ship branch changes
the register/block layout so the `bic` is not emitted and the plain zero test
(`cbz`) survives.  The read itself is `ldr r4,[sp,#136]; ldr.w r0,[r4,#180]`
(8 B), which the `ldrd` split absorbs.

The upgrade check must be **outside** the `if (tab != 0)` block (as in the
original source); with it nested inside, the `tab == 0` edge goes to the merge
and the backend emits `cmp;beq.w`, not `cbz`.

## Trials (full build + fdiff + align_diff + extab_record_map)

`raw` / `al` / `size` / total extab / tab==0 `cbz`.  "B" is the w10616 base
shape (upgrade outside, `volatile int *padX = 0`).

| variant | raw | al | size | extab | cbz |
|---|---:|---:|---:|---:|---|
| landed (wave 162) | 2,514 | 485 | 9,700 | 725 | no |
| B (base) | 1,965 | 291 | 9,700 | 1,250 | no |
| B + `& (Globals::w > 0)` | 6,572 | 848 | 9,704 | 1,693 | yes |
| B + `tab == 1 & upgradeMode` | 7,442 | 1,385 | 9,704 | 1,741 | yes |
| B + `assume(tab>=0); assume(tab!=7)` | 6,315 | 331 | 9,696 | 1,282 | yes |
| B + `assume(tab>=0); assume(tab!=7)` + `padX[1]` | 6,240 | 661 | 9,696 | 1,450 | yes |
| B + `assume(tab>=0); assume(tab!=7)` + credits | 8,673 | 767 | 9,676 | 1,272 | no |
| **B + `assume(tab>=0); assume(tab!=7)` + volatile scrollOffset** | **1,979** | **551** | **9,700** | **1,250** | **yes** |
| same + `padX[1]` | 2,452 | 576 | 9,700 | 1,459 | yes |
| same + second volatile read | 6,537 | 838 | 9,704 | 1,693 | yes |
| upgrade inside + assumes (no expect) | 2,327 | 417 | 9,700 | 1,448 | no |
| landed + assumes (expect kept) | 6,514 | 671 | 9,704 | 1,257 | no (isShip cbz only) |

The landed variant is a **raw win (-535 B)** and the first size-exact
`tab == 0` `cbz`.  The cost is the `.ARM.extab` record: row 39 stays
`(2508, 118, 9664)` instead of `(2508, 150, 9664)`, so its 1-byte ULEB
shifts every later row and the record diff rises 151 -> 676.  The section
size stays exact (948 B / 156 rows), so the gate is unaffected.

## Open questions for wave 164

1. The row-39 length is 118 (needs >= 128).  The `getShip` invoke is in
   row 40 (LP 9538) instead of row 39 (LP 9664): it unwinds to the
   two-String cleanup, not the name cleanup.  A source shape that makes
   `getShip` unwind to LP 9664 while keeping the `cbz` would drop the
   record diff to ~151 and the total below 725.
2. The 8-B volatile read is a codegen anchor, not original source.  If the
   `bic` from `assume(tab>=0)` can be avoided another way (e.g. a `tab`
   type/range change), the dispatch could match the original byte-for-byte.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10916
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10916/fdiff.py _ZN12HangarWindow6renderEv       # 1979, 9700/9700
python3 /dev/shm/w10916/align_diff.py _ZN12HangarWindow6renderEv  # 551
python3 _work/reconstructions/extab_record_map.py                 # 1250
cmake --build cmake-build-match --target check -j6                # RATCHET PASS
```
