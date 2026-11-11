# `Level::createShip` — wave 167 (w11214): faithful jitter `X,Y,Z`
# landed; the 4-byte RA shift is allocator-weight-driven, not
# declaration-order-driven

Worktree `wt/w11214`, base `master d00057e6d` (wave 166).  Target
`_ZN5Level10createShipEiiiP8Waypointbb` (`src/game/world/Level.cpp`,
2,764 B, VMA 0xc0214).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).  All
evidence is objdump of the original and `-mllvm -print-machineinstrs`
of our TU (scratch `/dev/shm/w11214/`).

Gate on entry and exit (full `check`): `byte_exact 4507/4524`,
`linked_exact 4507/4524`, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, parity 5370/5370.  `.rel.plt` byte-identical (0).

| metric | before | after |
|---|---:|---:|
| raw same-address diff | **1,560** | **1,560** |
| instruction-index alignment | **455** | **460** |
| size | 2,764 | 2,764 |
| `.text` differing bytes | 82,898 | 82,898 |

## 1. Landed change

The three `Globals::rnd->nextInt(40000)` jitter calls are back in the
original's source order **`jitterX, jitterY, jitterZ`** (wave 164 had
landed the tuned companion `jitterY, jitterX, jitterZ`).  The
original's stack slots `[sp,#36]`,`[32]`,`[28]` are
`jitterX`,`jitterY`,`jitterZ`, so `X,Y,Z` is the real source order.
Raw is unchanged (1,560); aligned 455 -> 460 (difflib re-alignment
only).  This is the same raw-neutral fidelity change wave 166 measured
but did not land.

## 2. The 4-byte RA shift is allocator-weight-driven

The original map (from objdump):

| value | original | ours (base) |
|---|---|---|
| `race` | `[sp,#60]` | `[sp,#56]` (strd) |
| `shipClass` | `r5 -> sl -> r6` | `[sp,#60]` (strd) |
| `type` | `fp` (r11) | `sl` (r10) |
| `this` | `r9` (spill `[56]`) | `r9` (spill `[52]`) |
| `status` | `r4` | `r4` |
| `mission` | `r8` | `r8` |
| `jitterX` | `[sp,#36]` | `fp` (spill `[32]`) |
| `jitterY` | `[sp,#32]` | `[sp,#28]` |
| `jitterZ` | `[sp,#28]` | `[sp,#24]` |
| `x` | `[sp,#44]` | `[sp,#44]` |
| `y` | `[sp,#48]` | `[sp,#36]` |
| `z` | `[sp,#40]` | `[sp,#40]` |

The original also spills `type` to `[sp,#52]` at c0336 and `this` to
`[sp,#56]` at c0332; ours spills `this` to `[52]` and `jitterX` to
`[32]`.  The chain is: whoever wins the second callee-saved slot
(`sl` vs `fp`) decides the whole map.

I parsed the **After Greedy Register Allocator** machine IR and
computed LLVM's spill weight (`sum(uses * blockfreq/entryfreq) /
live-range length`) for our base build:

| value | vreg | uses | length | weight |
|---|---|---:|---:|---:|
| `status` | %136 | 10 | 218 | 3.27e7 |
| `type` | %689 | 6 | 146 | 2.55e7 |
| `mission` | %138 | 5 | 150 | 2.06e7 |
| `jitterX` | %715 | 2 | 80 | 1.78e7 |
| `this` | %686 | 2 | 137 | 1.60e7 |
| `shipClass` (reload) | %703 | 3 | 12 | 1.05e7 |

`type` (2.55e7) > `jitterX` (1.78e7), so greedy processes `type` first
and gives it `sl` (r10), then `jitterX` gets `fp` (r11).  In the
original `shipClass` outranks `type`, gets `sl`, and `type` falls to
`fp`; `jitterX` then has no callee-saved slot and is spilled.  Ours
spills `shipClass` at entry, so its measured interval is only the
short reload (%703, weight 1.05e7).

This is the PGO branch-weight effect the wave-163/164 notes identified:
the original's block frequencies make `shipClass`'s hot uses outrank
`jitterX`'s single use in the coordinate block.  Without a profile the
greedy allocator picks the opposite order.

## 3. Sweeps (all full link, size checked)

### 3.1 Float-coordinate shape (the base shape)

Every permutation of declaration order, waypoint-assignment order and
coordinate-computation order is **raw 1560 / aligned 460 / 2,764 B**
with the *same* entry map (`strd r1,r2,[56]; mov r9,r0; mov sl,r3`).
The float shape is completely insensitive to source ordering.

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base (`Y,X,Z`) | 1,560 | 455 | 2,764 |
| **`X,Y,Z` (landed)** | **1,560** | **460** | **2,764** |
| `X,Y,Z` + all 6 coord decl orders | 1,560 | 460 | 2,764 |
| `X,Y,Z` + all 6 coord compute orders | 1,560 | 460 | 2,764 |
| `X,Y,Z` + all 36 wp decl/assign orders | 1,560 | 460 | 2,764 |
| `X,Y,Z` + coords declared at top | 1,560 | 455 | 2,764 |
| `X,Y,Z` + hoisted `type`/`shipClass` bools | 1,560 | 455 | 2,764 |
| `X,Y,Z` + `level += shipClass*0` / `type*0` | 1,560 | 460 | 2,764 |
| `X,Y,Z` + mission bitmask | 1,560 | 530 | 2,764 |
| `Y,X,Z` + mission bitmask | 1,560 | 535 | 2,764 |
| `X,Y,Z` + `__builtin_expect(shipClass==1,0)` (empTime) | 2,114 | 511 | 2,768 |
| `X,Y,Z` + `__builtin_expect(shipClass==0,1)` | 2,498 | 1,293 | 2,744 |

The mission bitmask (`if ((mission >= 49 && mission <= 52) || mission == 56)`)
reproduces the original's `sub/cmp #7/uxtb/movs #143/lsr` byte sequence
(c02f8-c030e) but is raw-neutral and costs 70 aligned instructions, so
it was not landed this wave.

### 3.2 Int-coordinate shape (the real original shape)

The original computes `int x,y,z` and converts at both call sites
(three `vcvt` in each branch).  That shape is **robustly 2,772 B**
(coords before `new Player`) or **2,788 B** (coords after `new Player`),
never 2,764 B:

| variant | raw | aligned | size | entry map |
|---|---:|---:|---:|---|
| int, coords after `new Player` | 2,530-2,532 | 909-914 | 2,788 | `strd r0,r1[56]; mov r6,r3; mov sl,r2` |
| int, coords before `new Player` | 2,441-2,444 | 848-866 | 2,772 | `str r1[60]; mov r5,r0; mov r9,r3; mov sl,r2` |
| int, coords before, no status check | 2,337-2,443 | 480-555 | 2,792 | `strd r1,r2[56]; mov r9,r0; mov fp,r3` |

The int shape's extra size is the status null check (4 instructions /
8 B) plus the extra `vcvt` pair.  Dropping the status check flips
`type` into `fp` and `this` into `r9` (two thirds of the original map)
but costs +20 B because `shipClass`/`jitterX` then swap slots.

All 6 hitpoints case orders were re-measured on the int shape (best
`0x33,0x31,0x2c`: raw 2,441 at 2,772 B); none reaches 2,764 B.

## 4. Conclusion

The residual is **not** reproducible by source-level declaration/use
order changes at 2,764 B.  The float shape is order-invariant and the
int shape is robustly 2,772+ B.  The exact map needs the original's
PGO branch weights (or a different allocator ordering); a synthetic
profile fed to clang 7 remains the next experiment.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w11214
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level10createShipEiiiP8Waypointbb
cmake --build cmake-build-match --target check -j2
```

Scratch `/dev/shm/w11214/`: `gen.py` (body generator with knobs:
coordmode, wp decl/assign order, jitter order, coord order, case order,
status check/placement, `__builtin_expect`, comparison spelling),
`sweep.py` (apply + full link + raw/aligned/size + entry-map signature),
`sig.py`, `sbs.py`, `dis.py`, `weights.py` (greedy-IR spill-weight
parser), `mi.txt` (`-print-machineinstrs` dump), `body_*.txt`.
