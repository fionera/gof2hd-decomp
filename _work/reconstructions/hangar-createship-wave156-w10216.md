# HangarWindow::render + Level::createShip — wave 156 (w10216)

Worktree `wt/w10216`, base `master cf74f4ee6` (wave 155).  Targets
`_ZN12HangarWindow6renderEv` (`src/game/ui/HangarWindow.cpp`, 9700 B, VMA
0x1490d8) and `_ZN5Level10createShipEiiiP8Waypointbb`
(`src/game/world/Level.cpp`, 2764 B, VMA 0xc0214).  Ground truth is the
original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Metrics: raw same-address diff (`/dev/shm/w10216/fdiff.py`), instruction-index
alignment (`/dev/shm/w10216/align_diff.py`, worktree-pointed copy), and the
`.ARM.extab` LSDA row decoder (`_work/reconstructions/extab_lsda_decode.py`).

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, data 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370.

| function | raw before | raw after | aligned before | aligned after | size |
|---|---:|---:|---:|---:|---:|
| `HangarWindow::render` | 1965 | 1965 | 291 | 291 | 9700 |
| `Level::createShip` | 1564 | **1564** | 452 | **527** | 2764 |

## 1. `Level::createShip`: the mission-range test is a `switch`, not two `if`s

The original's mission-range test (fn offset 0xe8..0x100, VMA 0xc02fc) is a
single bitmask:

```
sub.w r0, r8, #49
cmp   r0, #7
bhi   ...
uxtb  r0, r0
movs  r1, #143        ; 0x8f = bits {0,1,2,3,7}
lsr.w r0, r1, r0
lsls  r0, r0, #31
it ne
movne.w r5, #270
```

i.e. `mission - 49` in {0,1,2,3,7} = {49,50,51,52,56}.  The landed wave-155 body
used two tests (`if (mission == 56) … else if (mission >= 49 && mission <= 52)`),
which compiles to `cmp r0,#4; it cc; movcc` plus a separate `cmp r8,#56`.
A `switch (mission) { case 49: case 50: case 51: case 52: case 56: … }`
reproduces the original bitmask **byte-identically** (verified in the
disassembly at 0xc02f8..0xc030c).

Landed: the switch form.  It is semantically identical (same five missions).

### Why the aligned metric reads 527

The aligned diff's extra 75 instructions are all in the literal-pool /
race-switch jump-table region at the end of the function, not in code: the
same-address diff of the last 200 bytes is **1 byte** for both bodies, and the
aligned diff restricted to the code region (first 940 instructions) is
**448 for both**.  The switch adds a `default` edge which changes the
instruction-alignment of the trailing data decode.  Real code diff is
unchanged.

### Variants measured (raw / aligned / size)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base (two `if`s) | 1564 | 452 | 2764 |
| **`switch (mission)` (landed)** | **1564** | **527** | **2764** |
| `if (mission == 56 \|\| (mission >= 49 && mission <= 52))` | 1577 | 457 | 2764 |
| `if ((unsigned)(mission-49) <= 3u \|\| mission == 56)` | 1564 | 527 | 2764 |
| `if (mission - 49 <= 3 \|\| mission == 56)` (signed) | 2144 | 519 | 2760 |
| `switch (mission - 49) { case 0..3,7 }` | 1564 | 527 | 2764 |
| `if (mission == 49 \|\| … \|\| mission == 56)` | 1564 | 527 | 2764 |
| `switch` with duplicated case bodies / reversed / default | 1564 | 527 | 2764 |

The `type`/`shipClass`/`race` register permutation is unchanged by every
source spelling tried this wave: `int sc = shipClass;` (top or after mission),
`const int sc`, `int t = type;`, `const int t`, both, jitter reorder Z/Y/X,
jitter decl-then-assign, `do { entry } while (0)`, `volatile` jitters,
level/hitpoints-before-jitters — all coalesce to 1564/527/2764.  The residual
is the PGO spill-weight choice (original: type→fp, shipClass→sl, jitters→stack;
ours: type→sl, jitterX→fp, shipClass→stack).

## 2. `HangarWindow::render`: rows 39/40 need the block move; no raw win

Base LSDA (156 rows, 948 B, 676 differing bytes).  Rows 39/40:

```
39 orig (2508,150,9664)  ours (2508,118,9664)   getShip LP outer vs padX
40 orig (2696,140,9562)  ours (2654,182,9514)   getShip..upgradeMode
```

The original's padX lifetime starts between `getShip` and the first
`setPosition`, and ends after the upgrade block, so the upgrade block must be
lexically inside the same scope as padX.  The only source shape that does this
is w10109's move (upgrade block inside the inner `if`).  New shapes measured
this wave:

| variant | raw | aligned | extab | size | row39 | row40 |
|---|---:|---:|---:|---:|---|---|
| base | **1965** | **291** | 676 | 9700 | (2508,118,9664) | (2654,182,9514) |
| **move + `volatile int *padX`** | 6502 | 580 | **253** | 9704 | (2508,150,9668) | (2700,140,9516) |
| move + `volatile int padX` | 7811 | 842 | 296 | 9700 | (2512,148,9666) | (2706,134,9514) |
| move + `int padX` (dead) | 6502 | 580 | 253 | 9704 | same as pointer form |
| move + `volatile int *padX` + `padY` | 6502 | 580 | 253 | 9704 | same |
| move + `volatile int padX` + `padY` | 8372 | 852 | 323 | 9704 | (2512,148,9670) | (2706,134,9518) |
| move + `__builtin_expect(tab != 0, 1)` | 6305 | 437 | 259 | 9696 | (2508,152,9660) | (2674,26,9352) |
| move + `__builtin_expect(inner, 1)` | 6508 | 583 | 253 | 9704 | (2508,150,9668) | (2700,140,9542) |
| `bool ok = !(tab==3 && getShip()==…)`, padX after | 2276 | 410 | 689 | 9700 | (2508,118,9666) | (2654,4,9328) |
| padX before `if`, assigned after getShip | 6606 | 838 | 706 | 9708 | (2508,236,9672) | (2782,62,9428) |

The `volatile int *padX` move is the only shape with exact row-39 start/length
and row-40 length; it is 4 B too large (9704) and costs raw 1965→6502.  No
shape fixes rows 39/40 while keeping the base text.  Base is kept.

## 3. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10216
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10216/fdiff.py _ZN5Level10createShipEiiiP8Waypointbb
python3 /dev/shm/w10216/align_diff.py _ZN5Level10createShipEiiiP8Waypointbb
python3 /dev/shm/w10216/fdiff.py _ZN12HangarWindow6renderEv
python3 /dev/shm/w10216/align_diff.py _ZN12HangarWindow6renderEv
python3 _work/reconstructions/extab_lsda_decode.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so
cmake --build cmake-build-match --target check -j6   # RATCHET PASS
```

Scratch in `/dev/shm/w10216`: `fdiff.py`, `align_diff.py`, `sbs.py`,
`sbs2.py`, `rawtail.py`, `gen.py`/`measure.sh` (HangarWindow variants),
`genl.py`/`genl2.py`/`genl3.py`/`measurel*.sh` (createShip variants),
`HangarWindow.base.cpp`, `Level.base.cpp`.
