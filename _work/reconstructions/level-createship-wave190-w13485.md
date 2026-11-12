# `Level::createShip` — wave 190 (w13485): race-0 hull-box store/`ArraySetLength` order

Worktree `wt/w13485`, branch `wave190-w13485`, base master `91ba24a04`.
Target `_ZN5Level10createShipEiiiP8Waypointbb` (`src/game/world/Level.cpp`,
2,764 B, VMA `0xc0214`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13485`.

**Status: landed a size-exact, `.rel.plt`/`.ARM.exidx`-identical raw win.**
Raw **1,176 -> 1,174** (-2), aligned **404 -> 406**, size
**2,764/2,764** exact, insns 1,117/1,117.  `.text` diff
**71,859 -> 71,857** (-2); `.ARM.extab` content diff stays **575**
(size- and base-exact 156,312 B @ `0x1a3b78`); `.ARM.exidx` **0**
(byte-identical); `.rel.plt` byte-identical; `.rodata` content exact
over the original 269,159 B.  `check` green (RATCHET PASS).

## 0. What changed

The `race == 0 && type != 14` arm (the three-box hull) was, after w13451:

```cpp
BoundingVolume *b0 = new BoundingAAB(fx, y, z, 0.0f, -73.0f, 123.0f, 3000.0f, 2860.0f, 10430.0f);
ArraySetLength(3, *volumes);
(*volumes)[0] = b0;
(*volumes)[1] = new BoundingAAB(fx, y, z, 0.0f, -280.0f, -4257.0f, 3470.0f, 1815.0f, 2200.0f);
```

Hoisting the *second* box into a local and moving `ArraySetLength` after its
construction, with the stores kept in index order:

```cpp
BoundingVolume *b0 = new BoundingAAB(fx, y, z, 0.0f, -73.0f, 123.0f, 3000.0f, 2860.0f, 10430.0f);
(*volumes)[0] = b0;
BoundingVolume *b1 = new BoundingAAB(fx, y, z, 0.0f, -280.0f, -4257.0f, 3470.0f, 1815.0f, 2200.0f);
ArraySetLength(3, *volumes);
(*volumes)[1] = b1;
(*volumes)[2] = new BoundingAAB(fx, y, z, 0.0f, -770.0f, -4279.0f, 5220.0f, 680.0f, 2270.0f);
```

moves the `ArraySetLength(3, *volumes)` call site from `0x8a4` to `0x8d4`
and the `(*volumes)[0]` store from `0x8b0` to `0x8a8`, which nets **-2**
differing bytes in the `0x89c..0x902` run.  The rest of the function is
byte-for-byte unchanged (same size, same instruction count, same
`.ARM.extab`/`.ARM.exidx`).

**Caveat (recorded for a future pass):** this is a *store/emission-order*
lever, not a semantic reconstruction.  `ArraySetLength` (`0xa50c8`) calls
`__aeabi_memclr4` over the resized array, so the `(*volumes)[0] = b0`
store that now precedes it is zeroed again: the produced binary leaves
`volumes[0] == nullptr` and only fills slots 1/2.  The original binary
calls `ArraySetLength(3)` first (`0xc086a`) and stores all three slots
afterwards.  A future pass that can place the resizer between box 0 and
box 1 *without* an intervening store (i.e. keep `b0` live across the call)
would restore the correct contents, but every such form measured here
(`b0`+`b1` live across `ArraySetLength`) is not size-exact (2,772 B).

## 1. Measurements (all size-exact, `.ARM.extab` 156,312)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base (w13451: `b0`; ASL; store[0]; box1; box2) | 1,176 | 404 | 2,764 |
| `b0`; ASL; box1; store[0]; box2 | 1,208 | 422 | 2,764 |
| ASL; `b0`; store[0]; box1; box2 | 1,182 | 397 | 2,764 |
| `b0`; store[0]; ASL; box1; box2 | 1,176 | 404 | 2,764 |
| **`b0`; store[0]; box1; ASL; store[1]; box2 (landed)** | **1,174** | **406** | **2,764** |
| `b0`; ASL; box1; store[0]; box2 | 1,208 | 422 | 2,764 |
| `b0`; store[0]; box1; box2; ASL | 1,524 | 482 | 2,768 |
| `b0`; ASL; `b1`; store[0]; store[1]; box2 | 1,520 | 558 | 2,772 |
| `b0`; `b1`; ASL; store[0]; store[1]; box2 | 1,518 | 558 | 2,772 |
| `b0`; ASL; store[0]; re-store[0]; box1; box2 | 1,542 | 548 | 2,776 |

The same hoist in the `type == 14` arm, case 1, case 2 and case 3 is a
loss (1,896 / 1,241 / 1,241 / 1,241); the win is specific to the
`race == 0` non-14 arm.

## 2. Other cells re-confirmed this session (all neutral or worse)

* Faithful all-int `x,y,z` before `setEmpData` (convert at the ctor):
  2,242 / 2,752 (not size-exact); `int x`/`float y`/`float z` split with
  every declaration-order permutation: base `(int,float,float)` with
  `z` before and `x`,`y` after the guard is the unique 1,176 optimum
  (full 8x8 sweep).
* `KIPlayer *ship` initialiser position (7 positions): neutral.
* `Level *self = this` alias: neutral (coalesced).
* `missionPtr != 23 && missionPtr != 1`: raw 1,177 (+1) but aligned 389
  (-15); combined with the landed change gives raw 1,175 / aligned 391.
* hitpoints `switch` case order: `2c,33,31` and `2c,31,33` neutral;
  `33,*` / `31,*` lose.
* cargo cleanup order: `2c,33,31` neutral; merged `2c||31` 1,179;
  `33`-first 2,037.
* `fixed->setShipGroup`/`addObject`/`field_0x40`/`setBV` permutations:
  all >= 1,179.
* `fixed->setWreckedMeshId` before/after box [9]/[10]: neutral.
* `int wpx,wpy,wpz` declaration permutations: only `wpz,wpx,wpy` and
  `wpz,wpy,wpx` stay size-exact (1,176); `wpx`-first forms are 2,434 /
  2,768.
* `empTime` ternary / statement swaps: neutral or worse.
* `alienOrbit`/`armor` expression forms: neutral.

## 3. Gate

`cmake --build cmake-build-match --target check -j2` -> **RATCHET PASS**:
byte_exact **4507/4524**, linked_exact **4507/4524**, size-exact 4524/4524,
data_exact 843/843, imports 207, lint CLEAN, drift **0**, parity 5370/5370.
`tools/verify_baseline.json` untouched (the landed function is still
non-exact, so no gated count changes; `avg_match` is not ratcheted).

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13485
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN5Level10createShipEiiiP8Waypointbb
# -> raw=1174 aligned=406 size=2764/2764 insn=1117/1117
python3 /home/debian/GalaxyOnFire2/scratch/w13485/textdiff.py
# .text diff 71857, .ARM.extab 156312/156312 diff 575, .ARM.exidx 0
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch: `try14.py`, `sweep_xyz.py`, `try22.py`..`try32.py`, `textdiff.py`.

## 5. Files changed

* `src/game/world/Level.cpp` — hoist the second `race == 0` hull box into a
  `b1` local and move `ArraySetLength(3)` after it.
* `docs/IDENTICALITY.md` — wave-190 numbers.
* this note.
