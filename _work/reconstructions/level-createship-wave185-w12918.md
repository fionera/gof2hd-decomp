# `Level::createShip` — wave 185 (w12918): coordinate-declaration split, raw 1,213 -> 1,202

Worktree `wt/w12918`, branch `wave185-w12918`, base master `f390b0e68`
(wave 184).  Target `_ZN5Level10createShipEiiiP8Waypointbb`
(`src/game/world/Level.cpp`, 2,764 B, VMA `0xc0214`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`).

**Status: source change landed.**  A size-exact raw win of 11 bytes.

| metric | before | after |
|---|---:|---:|
| raw same-address diff | **1,213** | **1,202** |
| instruction-index alignment | 396 | 484 |
| size | 2,764 / 2,764 | 2,764 / 2,764 |
| insns (orig/ours) | 1,117 / 1,117 | 1,117 / 1,116 |
| `.text` differing bytes | 74,591 | **74,580** |
| `.ARM.extab` size / base | 156,312 @ 0x1a3b78 | 156,312 @ 0x1a3b78 |
| `.ARM.extab` differing bytes | 647 | 647 |
| `.ARM.exidx` differing bytes | 0 | 0 |
| `.rel.plt` | byte-identical | byte-identical |
| `.rodata` | byte-identical | byte-identical |

Full `check`: `byte_exact 4507/4524`, `linked_exact 4507/4524`,
size-exact 4524/4524, stub 0, missing 0, wrong_type 0, extra 0,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370,
`RATCHET PASS`.

## 1. The change

The three coordinate temporaries were split around the `setEmpData`
cleanup scope: `z` is now declared before the guard, `x` and `y` after it.

```cpp
    Player *player = new Player(armor, hitpoints, 1, 1, 0);
    float z = (float) (wpz + jitter3 - 20000);
    {
        EmpGuard guard;
        player->setEmpData(empPoints, empTime);
    }
    int x = wpx + jitter1 - 20000;
    float y = (float) (wpy + jitter2 - 20000);
```

The original body computes all three ints before the call and converts
afterwards; this split is not the original shape, but it makes clang 7
schedule `z`'s int+convert before the call and `x`,`y` after, which
happens to line up more of the downstream register allocation.  The
instruction count drops by one (1,116 vs 1,117) while the size stays
exactly 2,764 B, so the encoding compensates elsewhere.

A systematic sweep of the 24 `(permutation of x,y,z) x (split point
0..3)` declaration placements found this cell uniquely best:

| placement | raw | size |
|---|---:|---:|
| `z` before, `x,y` after (landed) | **1,202** | 2,764 |
| `y` before, `x,z` after | 1,207 | 2,764 |
| all three before (base) | 1,213 | 2,764 |
| `x` before, `y,z` after | 2,051 | 2,752 |
| all three after | 1,361 | 2,764 |

Moving `z` before `new Player` or moving `x,y` before `new Player` grows
the function to 2,776 B (not size-exact).

## 2. Remaining residual

The residual is still dominated by the `fixed`/`volumes` register swap
(`fixed` r9 vs r6/fp, `volumes` fp vs r9) and the `x` float-bit register
(`sl` vs `r6`), triggered by the coordinate block.  The all-int form
(`int x,y,z` with casts at the constructor calls) remains 2,752 B (12 B
short) and raw 2,238.  The stack-slot assignment (`wpx` 44 vs 32,
`wpz` 40 vs 44, first jitter 36 vs 40) and the `empPoints` clamp branch
polarity (`ble` vs `bgt`) are unchanged.

## 3. Sweeps that did not beat 1,202

* All 216 `(waypoint read order) x (nextInt call order) x (jitter
  declaration order)` combinations: only call order
  `(jitter3,jitter2,jitter1)` reaches 1,213; everything else is worse.
* Coordinate statement order permutations (6): `xyz`/`yxz`/`yzx` 1,213,
  `xzy`/`zxy`/`zyx` 1,217.
* Waypoint block forms (ternary, `if (waypoint)`, early-return, z-first
  assignment): 1,213 or worse.
* `level`/`empPoints` clamp polarity, `if`/ternary forms, `__builtin_expect`
  on either clamp: 1,213..2,531 (expect grows the size).
* Per-TU flag sweep on `Level.cpp` (`-enable-stack-coloring=false`,
  `-split-spill-mode=size/default`, `-enable-postra-machine-sink=false`,
  `-disable-postra-machine-licm`, `-regalloc=fast`, `-enable-tail-merge=false`,
  `-enable-if-conversion=false`, `-enable-machine-sink=false`,
  `-misched-postra=false`, `-enable-post-misched=false`): all worse
  (2,512..3,152).
* `x`/`y`/`z` type variants (`unsigned`, one-line declarations, initialized
  declarations): all 1,213.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12918
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN5Level10createShipEiiiP8Waypointbb
cmake --build cmake-build-match --target check -j2
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w12918/`: `Level_base.cpp`,
`m.py` (raw/size/insn + section diff harness), `try*.py`, `sweep_pair.py`,
`sweep_place.py`, `sweep_place2.py`.
