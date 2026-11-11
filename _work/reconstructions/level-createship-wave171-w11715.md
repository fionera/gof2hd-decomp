# `Level::createShip` — wave 171 (w11715): RA-map sweep; mission guard bitmask landed

Worktree `wt/w11715`, branch `wave169-w11715`, base `master 41ef4c8b6` (wave 171).
Target `_ZN5Level10createShipEiiiP8Waypointbb` (`src/game/world/Level.cpp`,
2,764 B, VMA 0xc0214).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so` (NDK r18b
clang 7.0.2 `-Oz`).

Gate on entry and exit (full `check`): `byte_exact 4507/4524`,
`linked_exact 4507/4524`, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370, `.rel.plt` byte-identical.

| metric | before | after |
|---|---:|---:|
| raw same-address diff | **1,560** | **1,560** |
| instruction-index alignment | **455** | **530** |
| size | 2,764 | 2,764 |
| insns | 1,117 | 1,118 |

## 1. Landed change

The `mission` guard is the original's single bitmask test over `[49,56]`
(objdump `c02fc`..`c0310`: `sub.w r0,r8,#49; cmp r0,#7; uhi; uxtb; movs r1,#143;
lsr.w; lsls #31; it ne; movne.w r5,#270`).  Replaced the if/else chain with the
equivalent `switch` (same as wave-170 w11615, re-measured on master):

```cpp
switch (mission) {
    case 49:
    case 50:
    case 51:
    case 52:
    case 56:
        hitpoints = 270;
        break;
}
```

Raw is unchanged at 1,560: the bitmask is 2 B longer than the if/else, which
flips the 2-byte jump-table alignment nop before the `race` switch (ours gains
a `nop`; size still exactly 2,764 B).  `aligned` moves 455 -> 530 purely from
difflib re-alignment of the shifted tail.  Raw-neutral structural-fidelity
commit.

## 2. RA-map sweep (no source spelling found that flips `type` to `fp`)

Original vs committed entry map:

| value | original | committed |
|---|---|---|
| `race` | `[sp,#60]` | `[sp,#56]` (`strd`) |
| `shipClass` | `r5 -> sl -> r6` | `[sp,#60]` (`strd`) |
| `type` | `fp` (r11) | `sl` (r10) |
| `this` | `r9` (spill `[56]`) | `r9` (spill `[52]`) |
| `jitterX/Y/Z` | `[36]/[32]/[28]` | `[32]/[28]/[24]` |
| `wy/wx/wz` | `[44]/[48]/[40]` | `[44]/[36]/[40]` |

The whole residual cascades from one choice: `type` gets `sl` instead of `fp`.
With `type` in `fp`, `sl` is free and `shipClass` is region-split into `sl`
(+`r6`), `jitterX` spills to `[32]`, and every later stack slot shifts +4.

### Why `type` gets `sl`

Parsed the pre-regalloc `-mllvm -print-machineinstrs` block.  Virtual-register
live spans (slot units, entry..last use):

```
this 26464 > type 26176 > race 25984 > ship 21808 > fighter 21488 > volumes 18320
     > ... > shipClass 6960
```

LLVM 7 `RAGreedy::enqueue` (`Prio = (1<<29) + Size` for global ranges) therefore
processes `type` second, after `this`.  The default ARM GPR allocation order
(`RegisterClassInfo::compute`: volatile first, then CSR aliases) is
`r0..r3,r12,lr,r4,r5,r6,r7,r8,r9,r10,r11`.  In the committed build `r4`=status,
`r5`=hitpoints, `r8`=mission, `r9`=this are already taken, so `type` takes the
first free CSR `r10` (`sl`).  The original must have had `sl` occupied at that
point; the `RAGreedy::tryAssignCSRFirstTime` pre-splitting heuristic (which
produces the original's `mov r5,r2; ...; mov sl,r5` split of `shipClass`) is
sensitive to split cost, not to any source-order spelling we found.

### Variants measured (full link, size-checked; base = 1,560/455/2,764)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base (if/else guard) | 1,560 | 455 | 2,764 |
| **mission switch (landed)** | **1,560** | **530** | **2,764** |
| type switch order `0x33,0x31,0x2c` | 1,563 | 446 | 2,764 |
| type order + mission switch | 1,563 | 521 | 2,764 |
| jitter decl `X,Y,Z` | 1,560 | 460 | 2,764 |
| jitter `X,Y,Z` + type order | 1,563 | 451 | 2,764 |
| jitter `Z,Y,X` / split decls | 1,560 | 460 | 2,764 |
| waypoint loads `x,y,z` | 1,560 | 455 | 2,764 |
| `int cls = shipClass` (all/early/late uses) | 1,560 | 455 | 2,764 |
| `int t = type` (all/early/late uses) | 1,560 | 455 | 2,764 |
| `empPoints` before type switch | 1,563 | 478 | 2,764 |
| mission guard before type switch | 1,569 | 460 | 2,764 |
| hitpoints before level | 1,560 | 455 | 2,764 |
| jitter block before waypoint block | 1,601 | 568 | 2,764 |
| level before jitters | 2,650 | 1,383 | 2,796 |
| empPoints before hitpoints | 2,303 | 559 | 2,768 |
| armor before empPoints | 2,446 | 545 | 2,776 |
| player ctor before type switch | 2,612 | 790 | 2,632 |
| `empTime = 15000` initializer | 1,560 | 455 | 2,764 |
| `type == 14` as if/else | 1,560 | 455 | 2,764 |
| armor/player before empTime | 2,597 | 1,209 | 2,744 |
| armor before hitpoints adjust | 2,456 | 525 | 2,788 |
| `bool capital = shipClass == 1` | 2,453 | 1,133 | 2,712 |
| `if (shipClass != 1) ... else` | 2,453 | 1,133 | 2,712 |
| dispatch capital-first | 2,406 | 706 | 2,796 |
| `volumes/fixed/fighter` declared at top | 1,560 | 455 | 2,764 |
| `wx` declared before `wy` | 1,561 | 458 | 2,764 |
| `int wy,wx,wz;` then assign | 1,560 | 455 | 2,764 |

Key findings:

* The jitter declaration order is codegen-invariant: `jitterX` first and
  `jitterY` first emit identical bytes (the two `nextInt(40000)` calls are
  reordered/relabelled by the backend).  The original's first call feeds `x`
  (`[36]` -> `[48]` wx) and the second feeds `y`; the committed source already
  computes `x = wx + jitterX` with `jitterX` second, so it is consistent.
* `int cls = shipClass` / `int t = type` copies always coalesce (mem2reg +
  register coalescer), regardless of placement.
* Declaring `fighter`/`fixed`/`volumes` at function scope is erased by mem2reg.
* `bool capital` / `!= 1` restructure merges the two dispatch comparisons and
  shrinks the function by 52 B; not the original shape.
* The +4 stack shift is not reachable by declaration order; the original's
  lowest spill slot is `[28]` (never `[24]`), i.e. its frame-index colouring
  differs by one slot.

## 3. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w11715
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level10createShipEiiiP8Waypointbb
cmake --build cmake-build-match --target check -j2
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w11715/`: `base_fn.cpp` (extracted
function), `try.sh`/`screen.sh` (pristine-restore + rebuild + fn_diff + entry
map), `v*.cpp`/`a*.cpp`/`b*.cpp`/`c2.cpp`/`d*.cpp`/`e*.cpp`/`f*.cpp`/`g*.cpp`
(variants), `mi.txt` (machineinstrs), `orig_createShip.txt`,
`ours_createShip.txt`, `ra7.cpp`/`allocorder.cpp`/`rci.cpp`/`csw.cpp` (LLVM 7
sources used for the allocation-order analysis).
