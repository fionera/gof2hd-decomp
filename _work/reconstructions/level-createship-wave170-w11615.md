# `Level::createShip` — wave 170 (w11615): mission guard is a switch/bitmask
# (raw-neutral at 2,764 B); RA map sweep negative

Worktree `wt/w11615`, branch `wave169-w11615`, base `master 50716a763` (wave 170).
Target `_ZN5Level10createShipEiiiP8Waypointbb` (`src/game/world/Level.cpp`,
2,764 B, VMA 0xc0214).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`).  Evidence: objdump of the original
and of our linked `.so`, plus clang `-mllvm -print-machineinstrs`.

Gate on entry and exit (full `check`): `byte_exact 4507/4524`,
`linked_exact 4507/4524`, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, parity 5370/5370.  `.rel.plt` byte-identical (0).

| metric | before | after |
|---|---:|---:|
| raw same-address diff | **1,560** | **1,560** |
| instruction-index alignment | **455** | **530** |
| size | 2,764 | 2,764 |
| insns (disasm incl. pools) | 1,117 | 1,118 |

## 1. Landed change

The `mission` guard was the if/else chain:

```cpp
if (mission == 56) {
    hitpoints = 270;
} else if (mission >= 49 && mission <= 52) {
    hitpoints = 270;
}
```

The original emits a **single bitmask test** over `[49,56]` (objdump
`c02fc`..`c0310`):

```
sub.w r0, r8, #49
cmp   r0, #7
bhi.n ...
uxtb  r0, r0
movs  r1, #143          ; 0b10001111 = {49,50,51,52,56}
lsr.w r0, r1, r0
lsls  r0, r0, #31
it    ne
movne.w r5, #270
```

The source is now the equivalent `switch`:

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

The emitted region now matches the original instruction-for-instruction
(same opcodes/operands; only the containing-function address differs).
Raw is unchanged at 1,560 because the bitmask is 2 B longer than the
if/else chain, which flips the 2-byte jump-table alignment nop before the
`race` switch (ours gains a `nop` at the table, the original has none) and
shifts the rest of the function; the two effects cancel in the total size
(2,764 B).  `aligned` moves 455 -> 530 purely from difflib re-alignment of
the shifted tail.  This is a raw-neutral structural-fidelity commit.

This is the w11515 wave-169 finding (its branch, not master); it is
re-measured and landed here on top of master 50716a763.

## 2. RA-map sweep on top of the switch (no 2,764 B exact map)

Entry map (original vs committed):

| value | original | committed |
|---|---|---|
| `race` | `[sp,#60]` (`str r1`) | `[sp,#56]` (`strd`) |
| `shipClass` | `r5 -> sl -> r6` | `[sp,#60]` (`strd`) |
| `type` | `fp` (r11) | `sl` (r10) |
| `this` | `r9` (spill `[56]`) | `r9` (spill `[52]`) |
| `jitterX/Y/Z` | `[36]/[32]/[28]` | `fp,[28],[24]` |

Full-link variants measured this wave (all size-checked; base = 1,560/455/2,764):

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base (if/else guard) | 1,560 | 455 | 2,764 |
| **mission switch (landed)** | **1,560** | **530** | **2,764** |
| switch + faithful jitter `X,Y,Z` | 1,560 | 535 | 2,764 |
| switch + faithful type order `0x33,0x31,0x2c` | 1,563 | 521 | 2,764 |
| switch + `int x,y,z` (all int coords) | 2,529 | 889 | 2,800 |
| switch + `int x,y,z`, no null check | 2,413 | 494 | 2,792 |
| switch + no null check | 2,053 | 495 | 2,764 |
| switch + null check before `setEmpData` | 2,425 | 495 | 2,776 |
| switch + `switch(shipClass)` dispatch | 2,507 | 485 | 2,780 |
| `switch(shipClass)` only | 2,508 | 496 | 2,780 |
| `switch(shipClass)` + no null check | 2,040 | 527 | 2,768 |
| `int cls = shipClass` (early/late/split use) | 1,560 | 530 | 2,764 |
| coords after `setEmpData` | 1,560 | 455 | 2,764 |
| coords before `Player` ctor | 2,495 | 511 | 2,780 |
| coords right after jitters | 2,501 | 547 | 2,776 |
| jitters before waypoint block | 1,601 | 568 | 2,764 |
| mission call after jitters | 1,606 | 570 | 2,764 |
| waypoint block before mission call | 2,338 | 544 | 2,768 |
| `ship`/`player` declared at top | 1,560 | 455 | 2,764 |
| `missionPtr` `==1||==23` inverted/else | 1,560 | 455 | 2,764 |
| cargo `switch(type)` | 1,560 | 455 | 2,764 |
| shipClass empTime block moved after armor | 2,515 | 534 | 2,780 |

Key findings:

* `int cls = shipClass` copies coalesce away (all placements identical);
  local declaration position is erased by mem2reg — only def/use order
  matters.
* `switch(shipClass)` does move `race` to `[sp,#60]` and `shipClass` to
  `sl` (two original features), but costs +16 B and puts `type` in `r9`
  and `this` on the stack.
* The scheduler always sinks the coordinate block after `setEmpData`
  (source order before/after the call makes no difference), so the
  original's "coords between `Player` ctor and `setEmpData`" schedule is
  not reachable from source order in this build.
* Greedy RA orders by **live-interval size**, not spill weight
  (`RAGreedy::enqueue`: `Prio = (1<<29) + Size` for global ranges).
  Committed interval sizes: `this` 26416 > `type` 26128 > `race` 25936 >
  `ship` 21648 > `volumes` 18320 > `shipClass` 6912.  `type` is processed
  before `shipClass`, so it takes `sl`; `shipClass` then has no free
  register over its whole range and is spilled.  In the original
  `shipClass` must be processed before `type` (it takes `sl`, forcing
  `type` to `fp`), which requires a different interval-size ordering —
  not a source-order spelling we found.

## 3. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w11615
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level10createShipEiiiP8Waypointbb
cmake --build cmake-build-match --target check -j2
```

Scratch `/dev/shm/w11615/`: `h.py` (splice+build+fn_diff+entry map),
`v1..v11.py` (variant grids), `mi.txt` (machineinstrs dump),
`weights.py` (interval sizes/frequencies), `orig.txt`/`ours.txt`.
