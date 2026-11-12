# `Level::createShip` — wave 190 (w13376): rebase w13360 float local + jitter order

Worktree `wt/w13376`, branch `wave190-w13376`, base master `0f54626b3`.
Target `_ZN5Level10createShipEiiiP8Waypointbb` (`src/game/world/Level.cpp`,
2,764 B, VMA `0xc0214`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13376`.

**Status: landed a size-exact, `.rel.plt`-identical raw win.**  Raw
**1,200 -> 1,195** (-5), aligned **484 -> 396** (-88), size
**2,764/2,764** exact, insns 1,116/1,117 -> **1,117/1,117** exact.
`.text` diff **73,516 -> 73,511** (-5); `.ARM.extab` content diff stays
**610** (size- and base-exact 156,312 B @ `0x1a3b78`); `.ARM.exidx` 0;
`.rel.plt` byte-identical; `.rodata` content exact over the original
269,159 B.

## 0. What was rebased

w13360 (branch `wave189-w13360`, commit `3bb2ef859`) landed a `float fx = x;`
staging local for the `shipClass == 1` branch on a base that predated the
w13347 `fixed->geometry` local (their base raw 1,202).  Current master already
contains w13347 (raw 1,200).  Re-applying the w13360 change on top:

```cpp
} else if (shipClass == 1) {
    float fx = x;
    ...
    new (fixed) PlayerFixedObject(type, race, player, nullptr, fx, y, z);
    ...
    (*volumes)[i] = new BoundingAAB(fx, y, z, ...);
```

gives raw **1,195** (the two changes are additive: -2 from w13347, -5 from
w13360's float local).

w13360 also had an **uncommitted** jitter-order experiment
(`jitter1, jitter3, jitter2`, raw 1,196 aligned 397 on its base).  A full
6-permutation sweep on the rebased body found `jitter3, jitter1, jitter2`
strictly dominates it:

| jitter declaration order | raw | aligned | size | insns |
|---|---:|---:|---:|---:|
| 3,2,1 (master base) | 1,195 | 483 | 2,764 | 1,116/1,117 |
| 1,2,3 | 1,197 | 398 | 2,764 | 1,117/1,117 |
| 1,3,2 (w13360 uncommitted) | 1,196 | 397 | 2,764 | 1,117/1,117 |
| 2,1,3 | 1,197 | 485 | 2,764 | 1,116/1,117 |
| 2,3,1 | 1,196 | 484 | 2,764 | 1,116/1,117 |
| **3,1,2 (landed)** | **1,195** | **396** | **2,764** | **1,117/1,117** |
| 3,2,1 + fx (no jitter change) | 1,195 | 483 | 2,764 | 1,116/1,117 |

The three `nextInt(40000)` calls are independent (the RNG state is the only
side effect; the values feed `x`, `y`, `z` separately), so the order is
semantically free.  `3,1,2` keeps the raw win and additionally aligns the
instruction stream (aligned 483 -> 396, insns exact).

## 1. Gate

`cmake --build cmake-build-match --target check -j2` -> **RATCHET PASS**:
byte_exact **4507/4524**, linked_exact **4507/4524**, size-exact 4524/4524,
data_exact 843/843, imports 207, lint CLEAN, drift **0**, parity 5370/5370.
`tools/verify_baseline.json` untouched.

## 2. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13376
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN5Level10createShipEiiiP8Waypointbb
# -> raw=1195 aligned=396 size=2764/2764 insn=1117/1117
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch: `jit_sweep.py` (6-permutation jitter sweep), `Level_fx.cpp`
(rebased body before the jitter change), `sec.py` (section diff).

## 3. Files changed

* `src/game/world/Level.cpp` — `float fx = x;` in the fixed branch plus
  jitter declaration order `3,1,2`.
* `docs/IDENTICALITY.md`, `docs/VALIDATION.md` — wave-190 numbers.
* this note.
