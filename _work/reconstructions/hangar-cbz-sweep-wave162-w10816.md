# HangarWindow::render — the `tab == 0` `cbz` sweep (wave 162, w10816)

Worktree `wt/w10816`, base `29248c1d0` (wave 161).  Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Target: `_ZN12HangarWindow6renderEv`, 9,700 B, `.ARM.extab` record
948 B / 156 rows.

Baseline on entry (the w10716 landed shape, unchanged at HEAD):

| metric | value |
|---|---:|
| raw same-address diff | 2,514 |
| instruction-index alignment | 485 |
| function size | 9,700 / 9,700 |
| its `.ARM.extab` record diff | 151 (948 B / 156 rows) |
| total `.ARM.extab` diff | 725 |
| `.text` diff | 85,374 |
| `.rel.plt` | byte-identical |

## Result

**No source shape was found that emits `cbz` for the `tab == 0` edge and is
size-exact at 9,700 B with the landed 948/156 record.**  The landed shape is
left unchanged; this note records the sweep and the two new findings.

## Finding 1: the `cbz` is reachable, but costs exactly 4 bytes too many

The original emits

```
149b24: cbz r0, 149b96        ; tab == 0 -> the tab == 1 check
149b26: cmp r0, #3
149b28: bne.n 149b40          ; tab != 3 -> buttons[1]
```

The landed shape emits `cmp r0,#3; beq.n ...; cmp r0,#0; bne.n ...; b.w END`
(the `tab == 0` edge is the `b.w`).  The reason is JumpThreading: the check
block is `upgradeMode && tab == 1` (IR cost 5, threshold 6), so JT duplicates
it onto the `tab == 0` edge and folds `tab == 0 -> tab != 1 -> END`.

Raising the check-block cost above 6 with a non-short-circuited extra
condition (`&`, not `&&`) stops the fold.  Reproduced on the **w10616 base
shape** (`volatile int *padX = 0; if (tab != 0) { ... } if (upgradeMode && tab == 1)`):

```cpp
if ((this->upgradeMode && tab == 1) & (Globals::w > 0)) { ... }
```

This emits the original's exact dispatch:

```
149b1a: cmp r0, #0
149b1c: beq.n 149bf6          ; isShip false -> else
149b1e: ldr r0, [sp, #108]
149b20: ldrd r5, r4, [sp, #132]
149b24: cbz r0, 149b92        ; tab == 0 -> check   <-- the cbz
149b26: cmp r0, #3
149b28: bne.n 149b40
```

but the function is **9,704 B**: the extra condition compiles to
`ldr; cmp; beq.w` (8 B) in the check block while the `cbz` saves only 4 B
(the base's `cmp;beq.w` is 6 B).  Every cheap condition tried
(`Globals::w > 0`, `listWidth != 0`, `y != 0`, `buttonOffset != 0`,
`iconOffsetY != 0`, `hintOffsetX != 0`, `barHeight != 0`, `i < 100000`, ...)
lands at 9,704 B; conditions on values the compiler can fold
(`tab != 2`, `tab < 3`, `tab < 100`, `(tab & 1) != 0`) do not raise the cost
and keep `cmp;beq.w` at 9,700 B.

## Finding 2: the check block is also out of `cbz` range once the fold is stopped

On the **landed** shape the same `& (Globals::w > 0)` trick also stops the
fold, but the check block then lands at `149c0c`, 226 B from the branch
(`cbz` range is +/-126 B), so the backend emits `cmp;bne.n;b.n` instead.
The distance is the non-ship `switch (tab)` body, which MachineBlockPlacement
puts between the dispatch and the ship body.  The original places the
non-ship body after the check block; source order/`__builtin_expect` on
`item->isShip()` did not flip it (E1/E2/E3 below).

## Trials (full build + fdiff + align_diff + extab_record_map)

`raw` / `al` / `size` / record `(olen,len,orow,urow)` / `cbz` on the
`tab == 0` edge.  "base" = w10616 shape (`padX = 0`, upgrade outside, no
`expect`); "landed" = current HEAD shape (`dummy` + `expect(tab!=0,1)` +
`padX[1]` + upgrade inside).

| variant | raw | al | size | record | cbz |
|---|---:|---:|---:|---|---|
| **landed (HEAD)** | **2,514** | **485** | **9,700** | 948/948/156/156 | no |
| base (w10616) | 1,965 | 291 | 9,700 | 948/948/156/156 | no |
| base + `& (Globals::w > 0)` | 6,572 | 848 | **9,704** | 948/948/156/156 | **yes** |
| base + `& (listWidth != 0)` | 6,526 | 833 | **9,704** | 948/948/156/156 | **yes** |
| base + `& (y != 0)` | 6,482 | 833 | **9,704** | 948/948/156/156 | **yes** |
| base + `& (buttonOffset != 0)` | 6,525 | 833 | **9,704** | 948/948/156/156 | **yes** |
| base + `& (iconOffsetY != 0)` | 6,521 | 585 | **9,704** | 948/948/156/156 | **yes** |
| base + `& (i < 100000)` | 6,635 | 699 | 9,712 | 948/948/156/156 | yes |
| base + `& (Globals::w > 0 && Globals::h > 0)` | 6,789 | 611 | 9,716 | 948/948/156/156 | yes |
| base + `& ((upgradeMode \| tab) != 0)` | 2,252 | 544 | 9,700 | 948/948/156/156 | no |
| base + `& (tab != 2)` (folded) | 1,965 | 291 | 9,700 | 948/948/156/156 | no |
| landed + `& (Globals::w > 0)` | 6,609 | 934 | 9,708 | 948/948/156/156 | no |
| landed + `& (listWidth != 0)` | 7,576 | 1,861 | 9,664 | 948/948/156/156 | no |
| landed + `& ((upgradeMode \| tab) != 0)` | 6,504 | 674 | 9,704 | 948/948/156/156 | no |
| landed + `& (tab \| 1) != 0` (folded) | 2,514 | 485 | 9,700 | 948/948/156/156 | no |
| landed + `expect(item->isShip(), 1)` | 8,632 | 2,399 | 9,680 | 948/960/156/158 | no |
| landed + `expect(item->isShip(), 0)` | 8,583 | 1,232 | 9,724 | 948/956/156/157 | no |
| base + `getCredits()` after padX | 8,532 | 736 | 9,680 | 948/948/156/156 | no |
| base + `getCredits()` + `& (Globals::w > 0)` | 8,562 | 778 | 9,688 | 948/948/156/156 | yes |
| base + `getCredits()` + `& (iconOffsetY != 0)` | 8,562 | 778 | 9,688 | 948/948/156/156 | yes |
| no `expect` (dummy + padX[1], nested) | 2,276 | 410 | 9,700 | **956/157** | no |
| `expect` + hoisted upgrade | 6,734 | 681 | 9,692 | 956/157 | no |
| explicit `switch (tab)` (case 0 break) | 2,276 | 410 | 9,700 | 956/157 | no |
| `goto check` on `tab == 0` | 6,725 | 602 | 9,692 | 960/158 | no |
| `tab == 1 && upgradeMode` order | 2,514 | 482 | 9,700 | 948/948/156/156 | no |

The `no expect` / `switch` / `goto` variants emit `cmp;beq.w` at 9,700 B but
drop the record to 956 B / 157 rows, which grows `.ARM.extab` by 8 B
(0x26298 -> 0x262a0), shifts `.rodata`/`.data` and drops `byte_exact` to
4,044.  They are gate-fatal despite the raw win.

## Why the 4-byte compensator is not source-reachable here

The landed shape is already the Pareto point of (raw, aligned, extab): every
shape that keeps the 948/156 record costs 2,514 raw, and every shape with a
smaller raw diff loses the record (and therefore the `.ARM.extab` section
size).  The `cbz` needs +8 B of check-block condition and saves 4 B, so it
is always +4 B.  A 4-byte saving elsewhere in the function was not found;
the `getCredits()` call (the w10616 "credits call") is itself a 4-byte `blx`
and pushes the base to 9,680/9,688, not 9,700.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10816
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10816/fdiff.py _ZN12HangarWindow6renderEv       # 2514, 9700/9700
python3 /dev/shm/w10816/align_diff.py _ZN12HangarWindow6renderEv  # 485
python3 _work/reconstructions/extab_record_map.py --ours cmake-build-match/libgof2hdaa.so
cmake --build cmake-build-match --target check -j6                 # RATCHET PASS
```

Scratch (not committed), `/dev/shm/w10816/`: `fdiff.py`, `align_diff.py`,
`batch.py` (variant generator + build/measure loop), `HangarWindow.base.cpp`,
`cc_ir.sh` (exact-flag `-emit-llvm` + `-mllvm -print-after-all`).
