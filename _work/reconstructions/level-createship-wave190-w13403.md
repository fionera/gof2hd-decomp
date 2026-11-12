# `Level::createShip` — wave 190 (w13403): BoundingAAB[9]/[10] emission order

Worktree `wt/w13403`, branch `wave190-w13403`, base master `774dcdfc1`.
Target `_ZN5Level10createShipEiiiP8Waypointbb` (`src/game/world/Level.cpp`,
2,764 B, VMA `0xc0214`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13403`.

**Status: landed a size-exact, `.rel.plt`/`.ARM.exidx`-identical raw win.**
Raw **1,192 -> 1,190** (-2), aligned 396 -> 403, size **2,764/2,764**
exact, insns 1,117/1,117.  `.text` diff **72,999 -> 72,997** (-2);
`.ARM.extab` content diff stays **603**, size/base exact (156,312 B @
`0x1a3b78`); `.ARM.exidx` 0; `.rel.plt` byte-identical; `.rodata` content
exact over the original 269,159 B.

## 0. What changed

In the `race == 0 && type == 14` arm the last two hull boxes were emitted
in index order `[9]` then `[10]`.  The two `new BoundingAAB` statements are
independent (separate `volumes` slots, no shared RNG), so their emission
order is semantically free.  Swapping them to emit `[10]` before `[9]`

```cpp
(*volumes)[10] = new BoundingAAB(fx, y, z, 0.0f, -2610.0f, -15852.0f, 15860.0f, 2510.0f, 10910.0f);
(*volumes)[9]  = new BoundingAAB(fx, y, z, 0.0f, -15852.0f, 2004.0f, 15860.0f, 2510.0f, 10910.0f);
```

moves the shared literal-pool loads (2510/15860/10910/2004/-15852/-2610)
into the original's order and removes two differing bytes in the
`c0820` literal-pool region and the surrounding arg setup.  Everything
else in the function is unchanged (same size, same instruction count, same
`.ARM.extab`/`.ARM.exidx`).

## 1. Measurements

| variant | raw | aligned | size | .text | extab | exidx |
|---|---:|---:|---:|---:|---:|---:|
| base (`[9]` then `[10]`) | 1,192 | 396 | 2,764 | 72,999 | 603 | 0 |
| **`[10]` then `[9]` (landed)** | **1,190** | **403** | **2,764** | **72,997** | **603** | **0** |

Also measured and rejected:

* swap `fixed->setWreckedMeshId(0x477d)` / `break` (case 2): raw 1,150,
  but the call becomes unreachable and is deleted (0x477d disappears), a
  semantic bug.
* `EmpGuard guard;` / `mission = ...` swap in the prologue: raw 1,156,
  but it removes the call's EH cleanup row, shrinking `.ARM.extab` to
  156,308 B and changing `.ARM.exidx` (1,327 diff bytes) - a net loss.
* `EmpGuard guard;` / `fighter->setShipGroup(...)` swap: raw 1,188, but
  `.ARM.extab` 156,304 B and `.ARM.exidx` 1,327 diff bytes - net loss.
* adjacent swaps of the other BoundingAAB statements: 1,186..1,257;
  the only clean (extab/exidx-preserving) win is `[10]`/`[9]`.

## 2. Gate

`cmake --build cmake-build-match --target check -j2` -> **RATCHET PASS**:
byte_exact **4507/4524**, linked_exact **4507/4524**, size-exact 4524/4524,
data_exact 843/843, imports 207, lint CLEAN, drift **0**, parity 5370/5370.
`tools/verify_baseline.json` untouched.

## 3. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13403
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN5Level10createShipEiiiP8Waypointbb
# -> raw=1190 aligned=403 size=2764/2764 insn=1117/1117
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

## 4. Files changed

* `src/game/world/Level.cpp` — `[10]` emitted before `[9]` in the
  `race == 0 && type == 14` hull-box arm.
* this note.

## Current-master harvest (wave 190)

Applied on master `dcb6b809c`: raw **1,192 -> 1,190** (-2), aligned
396 -> 403, size exact 2,764 B. `.text` **72,999 -> 72,976** (-23,
combined with the w13399 createMission win); `.ARM.extab` 603 and build-id
20 unchanged. `check` -> RATCHET PASS.
