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

## 5. Additional cells swept (all size-exact unless noted)

Re-measured on the landed base (raw 1,190) with `.ARM.extab` size/base
exact and `.ARM.exidx` 0 as the acceptance filter:

* coordinate-local placement: all 8 before/after-guard combinations of
  `x`/`y`/`z` (as `int x`, `float y`, `float z`) x 6 declaration orders.
  The landed `z` before / `x`,`y` after is best; `y` before is 1,196,
  `x` before is 1,350, `x`,`y` before is 1,496, all three before is 1,354,
  `x`,`z` before is 2,242/2,768.
* `EmpGuard` placement: removing the top guard gives raw 1,154 but
  `.ARM.extab` shrinks to 156,308 B (size no longer exact) and
  `.ARM.exidx` changes by 1,307 B - net loss.  Empty guard blocks are
  optimised away; a function-scope guard is 2,796 B.  Replacing the top
  guard with a guard around `getLevel`/`gameWon`/`hardCoreMode`/
  `inAlienOrbit`/`getShipGroup`/`setWreckedMeshId` yields 156,316..156,320 B
  extab (not exact).
* tail statement order: all 120 permutations of
  `setShipGroup` / `g`+`addObject` / `field_0x40` / `setBV` / `ship=fixed`.
  The landed order is the unique minimum (1,190); `ship=fixed` first also
  1,190, everything else 1,193..1,198.
* `race` case-0 hull-box order: all 11 rotations + reverse (1,283..1,324),
  plus all 120 permutations of the case-1 5-box block (1,216..1,259).
* `switch (race)` `default:` first/last: neutral.
* `if (type == 14)` inverted (`!=` with swapped bodies): neutral.
* `shipClass == 1` block: `empPoints *= 3` before `hitpoints *= ...` and
  `empTime` first: neutral (compiler canonicalises).
* `missionPtr` condition: `noexpect` 1,199; `expect` on the second term
  1,193; flipped order neutral.
* `level` ternary `<=` form: 1,206; `if`-statement forms: 2,776 B.
* `empPoints` `> 20 ? 20 : getLevel()` form: 1,197.
* `armor` inlined into the `Player` ctor: 2,760 B; `player` after `z`:
  2,780 B; `volumes` before `fixed`: 2,772 B.
* `Level *self = this;` / `this->missionPtr` / `missionPtr` local: neutral
  or worse (the `missionPtr` local is 2,748 B).
* `float zero = 0.0f;` local for the hull-box 4th argument: neutral.
* `ship = fighter` before the cargo cleanup / before the LOD block:
  neutral (compiler canonicalises).
