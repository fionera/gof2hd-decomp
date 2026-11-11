# `Level::createShip` — wave 169 (w11515): mission guard is a switch (bitmask),
# raw-neutral at 2,764 B; RA map still not reachable from source order

Worktree `wt/w11515`, branch `wave169-w11515`, base `master 00c403d5a` (wave 169).
Target `_ZN5Level10createShipEiiiP8Waypointbb` (`src/game/world/Level.cpp`,
2,764 B, VMA 0xc0214).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`).  Evidence: objdump of the original
and of our linked `.so`, plus clang `-mllvm -print-machineinsns` scratch
`/dev/shm/w11515/`.

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

The `mission` guard was an if/else chain:

```cpp
if (mission == 56) {
    hitpoints = 270;
} else if (mission >= 49 && mission <= 52) {
    hitpoints = 270;
}
```

The original emits a **bitmask test** (objdump `c02fc`..`c0310`):

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

That is exactly clang's lowering of a `switch` over `{49,50,51,52,56}`
(or of `(mission >= 49 && mission <= 52) || mission == 56`).  The source is
now the switch:

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

The emitted bitmask now matches the original instruction-for-instruction
(same opcodes/operands; only the containing-function address differs).
Raw is unchanged at 1,560 because the bitmask is 2 B longer than our old
if/else chain, which flips the 2-byte jump-table alignment nop before the
`race` switch (ours gains a `nop` at the table, the original has none) and
shifts the rest of the function; the two effects cancel in the total size
(2,764 B).  `aligned` moves 455 -> 530 purely from difflib re-alignment of
the shifted tail.  This is a raw-neutral structural-fidelity commit, not a
raw win.

Alternative spellings measured the same or worse: `(mission >= 49 &&
mission <= 52) || mission == 56` is identical (1,560/530); `mission == 56
|| (...)` is 1,573/460; `mission == 49 || ... || mission == 56` is
1,560/530; all switch case orders are 1,560/530.

## 2. RA-map sweep (no 2,764 B exact map)

Entry map (original vs committed):

| value | original | committed |
|---|---|---|
| `race` | `[sp,#60]` | `[sp,#56]` (`strd`) |
| `shipClass` | `r5 -> sl -> r6` | `[sp,#60]` (`strd`) |
| `type` | `fp` | `sl` |
| `this` | `r9` (spill `[56]`) | `r9` (spill `[52]`) |
| `jitterX/Y/Z` | `[36]/[32]/[28]` | `fp,[28],[24]` |

New variants measured this wave (full link, size checked; base = 1,560/455/2,764):

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base | **1,560** | **455** | **2,764** |
| mission switch (landed) | **1,560** | 530 | 2,764 |
| mission switch + jitter `X,Y,Z` | 1,560 | 535 | 2,764 |
| mission switch + coords `wx,wy,wz` | 1,561 | 538 | 2,764 |
| mission switch + `switch(type)` all 6 case orders | 1,560-1,564 | 519-535 | 2,764 |
| `if`-chain type orders `0x31,0x2c,0x33` / `0x31,0x33,0x2c` | 1,560 | 455 | 2,764 |
| `if`-chain type orders starting `0x33` | 2,394 | 1,351-1,362 | 2,764 |
| `int cls = shipClass` copies (5 placements) | 1,560 | 455 | 2,764 |
| `int rc = race` / `int ty = type` copies | 1,560 | 455 | 2,764 |
| `mission,level,hitpoints` after jitters (`mv_MLH`) | 1,606 | 570 | 2,764 |
| `mv_MLH` + mission switch | 1,604-1,605 | 555 | 2,764 |
| `mission,hitpoints` after jitters | 2,633 | 1,149 | 2,780 |
| branch swap (`if (shipClass) capital else fighter`) | 2,430 | 1,129 | 2,756 |
| branch swap + `mv_MLH` | 2,595 | 1,167 | 2,748 |
| `empPoints` before type switch | 1,579 | 555 | 2,764 |
| `empPoints` before `hitpoints` | 2,304 | 537 | 2,768 |
| `armor` before the `shipClass` block | 2,434 | 532 | 2,776 |

`mv_MLH` still reproduces the partial map `this->r9, type->r6,
shipClass->sl, race->[60]` at 2,764 B (wave 168's result), but at raw
1,606.  The branch swap moves the map further away (`this->r5,
race->r9, type->sl, shipClass->fp`) and drops the size to 2,756/2,748.
No source spelling found this wave puts `type` in `fp` and starts
`shipClass` in `r5` at 2,764 B; the residual is the greedy allocator's
spill-weight ordering (wave 168 §4), not declaration order.

## 3. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w11515
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level10createShipEiiiP8Waypointbb
cmake --build cmake-build-match --target check -j2
```

Scratch `/dev/shm/w11515/`: `batch.py`/`try.py` (splice + build + fn_diff +
entry map), `v1..v9.json` (variant grids), `sbs.py` (instruction
side-by-side), `mi.txt` (machineinstrs dump), `body_*.txt`.
