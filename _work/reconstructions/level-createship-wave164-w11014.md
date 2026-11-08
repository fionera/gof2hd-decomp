# `Level::createShip` — wave 164 (w11014): case-order raw win + a
# jitter-order aligned companion

Worktree `wt/w11014`, base `master b3d3a1f68` (wave 163).  Target
`_ZN5Level10createShipEiiiP8Waypointbb` (`src/game/world/Level.cpp`,
2,764 B, VMA 0xc0214).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).  All
evidence is objdump of the original; the Ghidra provider was not used.

Gate on entry and exit (full `check`): `byte_exact 4507/4524`,
`linked_exact 4507/4524`, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, parity 5370/5370.  `.rel.plt` byte-identical (0),
`.ARM.extab` 725, all 27 section sizes exact, file size exact.

| metric | before | after |
|---|---:|---:|
| raw same-address diff | **1,563** | **1,560** |
| instruction-index alignment | **449** | **455** |
| size | 2,764 | 2,764 |
| `.text` differing bytes | 84,652 | 84,649 |

## 1. Landed change

Two independent hunks:

1. `switch (type)` hitpoints case order `0x31, 0x2c, 0x33` (the w10914
   raw win).  The original's case-body order is `0x2c, 0x33, 0x31` and
   its compare order is `0x33, 0x31, 0x2c`; the base source already has
   the original body order.  Starting the source order with `0x31`
   moves the three `vmov`/`vldr` constant-pool loads so that 3 more
   bytes coincide with the original: raw **1,563 -> 1,560** at a cost
   of 11 aligned instructions.
2. The three `Globals::rnd->nextInt(40000)` jitter calls in the order
   `jitterY, jitterX, jitterZ` instead of `jitterX, jitterY,
   jitterZ`.  This is a true no-op semantically (three iid uniform
   draws; the joint distribution of `(jitterX, jitterY)` is unchanged)
   and it recovers 5 of the 11 aligned instructions: aligned
   **460 -> 455** with the same raw 1,560 and the same 2,764 B.  The
   original's call order is `X, Y, Z` (stack slots `[36]`,`[32]`,`[28]`
   are `jitterX`,`jitterY`,`jitterZ`), so this is a tuned companion,
   not the original spelling; see section 3 for the faithful variant.

All six case permutations and all six jitter permutations were
re-measured on a full link (size exact in every reported case).

## 2. Why the aligned metric cannot be fully recovered here

The aligned metric is difflib over normalised instructions and is
dominated by the 4-byte register-allocation shift documented in
`level-createship-wave163-w10914.md` (shipClass spilled instead of
living in `r5 -> sl -> r6`; `type` in `sl` instead of `fp`; `jitterX`
in `fp` instead of `[sp,#36]`).  The reorder changes the switch body
order away from the original's `0x2c, 0x33, 0x31`, so difflib has to
re-align the three bodies: +11 aligned is intrinsic to the raw win.
The jitter-order hunk is the only source-order lever found that buys
back part of it without touching size or raw.

The largest remaining aligned blocks are the coordinate computation
(`c03c8-c0400`, 37 instructions: the original computes `int x,y,z` and
converts at both call sites; we compute `float z,y` early and `int x`
late) and the capital-ship `BoundingAAB` argument setup.  Making
`x,y,z` ints is the real original shape but costs +24 B (2,788 B) in
our register map, so it cannot be landed at 2,764 B yet.

## 3. Measured variants (full link, size checked)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base | 1,563 | 449 | 2,764 |
| case order `0x31,0x2c,0x33` | 1,560 | 460 | 2,764 |
| case order + jitter `Y,X,Z` (landed) | **1,560** | **455** | 2,764 |
| jitter `Y,X,Z` only (faithful case order) | 1,563 | 444 | 2,764 |
| case order + jitter `X,Y,Z` (faithful jitter) | 1,560 | 460 | 2,764 |
| case order + `int` coords | 2,531 | 912 | 2,788 |
| case order + `int` coords, no null check | 2,413 | 481 | 2,792 |
| case order + mission bitmask | 1,560 | 535 | 2,764 |
| case order + two separate mission `if`s | 1,573 | 460 | 2,764 |
| case order + `expect(shipClass==1,1)` | 2,117 | 500 | 2,768 |
| case order + `expect(shipClass==0,1)` | 2,495 | 1,299 | 2,744 |
| case order + `expect(shipClass==1,0)` | 2,305 | 652 | 2,756 |
| drop status-null check (case order) | 2,052 | 505 | 2,764 |
| `-split-spill-mode=default` (flag probe) | 1,593 | 532 | 2,764 |
| `-regalloc=basic` (flag probe) | — | — | 2,712 |
| `-regalloc=pbqp` (flag probe) | — | — | 2,764 |

Other neutral (raw/aligned identical to base) spellings re-confirmed:
`hitpoints *= 2.25f`, `(int)(hitpoints*2.25f)`, `(int)(2.25f*hitpoints)`,
`switch` with `default`, `if/else if` chain, decimal case labels,
`int wy=0,wx=0,wz=0;` one line, `if (waypoint)` pointer test,
`int cls = shipClass` copies, `int empPoints = ... * 5 + 40` inline,
`int armor = 1000; if (...) armor = 650;`, `float opt = ...` hoist.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w11014
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j8
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level10createShipEiiiP8Waypointbb
cmake --build cmake-build-match --target check -j8
```

Scratch `/dev/shm/w11014/`: `try.py` (splice body + full link + both
metrics), `screen.py` (compile-only size + entry map), `linkonly.py` /
`linkvar.py` (manual compile with extra `-mllvm` flags + direct link),
`sbs.py` (aligned side-by-side), `cats.py` (full aligned category
counts), `rawdiff2.py` / `smallruns.py` (raw byte-run diffs),
`gaps.py` (per-instruction address gap), `bytes.py`, `region.py`,
`func_base.txt` (pristine body), `v*.txt` (variant bodies), `orig.txt` /
`ours.txt` (full disassembly).
