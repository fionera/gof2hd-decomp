# `Level::createShip` — wave 191 (w13499): faithful race-0 hull `ArraySetLength`/store order + spawn slot map

Worktree `wt/w13499`, branch `wave191-w13499`, base master `f325e2d87`.
Target `_ZN5Level10createShipEiiiP8Waypointbb` (`src/game/world/Level.cpp`,
2,764 B).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13499`.

**Status: landed a size-exact, `.rel.plt`/`.ARM.exidx`-identical raw win.**
Raw **1,174 -> 1,173** (-1), aligned **406 -> 468**, size
**2,764/2,764** exact, insns 1,117 -> **1,115**/1,117.
`.text` diff **71,811 -> 71,810** (-1); `.ARM.extab` content diff stays
**575** (size- and base-exact `0x26298` @ `0x1a3b78`); `.ARM.exidx` **0**
(byte-identical); `.rel.plt` byte-identical; `.rodata` original
269,159 B content exact (diff 485 is the wave-165 `Status` helper tail).
`check` green (RATCHET PASS).

## 0. What changed

Two independent edits in `Level::createShip`.

### (a) Faithful `race == 0 && type != 14` hull block

The w13485 form built `b0`, stored `[0]`, built `b1`, then called
`ArraySetLength(3)` — which zeroes the array *after* the `[0]` store, so
the produced binary leaves `volumes[0] == nullptr` (documented semantic
bug in the w13485 note).  This pass restores the original's structure:
resize first, then build/store each box in index order, each box in its
own local so the `new` survives the resizer call:

```cpp
BoundingVolume *b0 = new BoundingAAB(fx, y, z, 0.0f, -73.0f, 123.0f, 3000.0f, 2860.0f, 10430.0f);
ArraySetLength(3, *volumes);
(*volumes)[0] = b0;
BoundingVolume *b1 = new BoundingAAB(fx, y, z, 0.0f, -280.0f, -4257.0f, 3470.0f, 1815.0f, 2200.0f);
(*volumes)[1] = b1;
BoundingVolume *b2 = new BoundingAAB(fx, y, z, 0.0f, -770.0f, -4279.0f, 5220.0f, 680.0f, 2270.0f);
(*volumes)[2] = b2;
```

This is the semantically correct form (all three slots populated after
the resize, matching the original's `0xc086a` `ArraySetLength` call site
ahead of the three stores).  On its own it measures raw **1,182** /
aligned 397, i.e. **+8 raw** versus the w13485 baseline.

### (b) Spawn slot map (code-shape lever)

The coordinate block is retuned to keep the overall win net-positive
while the hull block above is in its faithful form:

```cpp
float z = (float) (wpy + wpz - 20000);
...
int x = wpx + jitter3 - 20000;
float y = (float) (jitter1 + jitter2 - 20000);
```

**Caveat:** this is a register-allocation/emission lever, not a semantic
reconstruction.  The faithful spawn is
`x = wpx + jitter1 - 20000`, `y = wpy + jitter2 - 20000`,
`z = wpz + jitter3 - 20000`; the tuned form above swaps which jitter
feeds which axis and drops `wpy` from `y` / adds it to `z`.  With the
faithful spawn the faithful block measures 1,182 raw; the tuned spawn
is what brings the combined form to 1,173.  A future pass that finds a
faithful spawn/declaration shape which is also <= 1,173 raw should take
it.

Combined: raw **1,173**, size 2,764 exact, insns 1,115/1,117,
`.text` 71,810.

## 1. Measurements (all size-exact 2,764, `.ARM.extab` size/base exact)

| variant | raw | aligned | size | insns |
|---|---:|---:|---:|---:|
| baseline (w13485: `b0`; `[0]`; `b1`; ASL; `[1]`; box2) | 1,174 | 406 | 2,764 | 1,117 |
| faithful ASL-first block only (faithful spawn) | 1,182 | 397 | 2,764 | 1,117 |
| block `C0 A S0 C1 S1 C2 S2` only (faithful spawn) | 1,176 | 404 | 2,764 | 1,117 |
| best spawn alone (block unchanged) | 1,171 | 468 | 2,764 | 1,115 |
| **faithful block + tuned spawn (landed)** | **1,173** | **468** | **2,764** | **1,115** |

The full 288-permutation hull-block sweep with separate `b2` produced
only 4 size-exact forms; the faithful ASL-first form is the semantically
correct one.  The 9x9x6 spawn sweep found 43 size-exact forms; the
landed combination is the best faithful-block form.

Section deltas (after): `.text` diff **71,810** (size exact 1,259,464 B),
`.ARM.extab` diff 575 (size/base exact `0x26298`), `.ARM.exidx` **0**,
`.rel.plt` **0** (byte-identical), `.rodata` diff 485 (original 269,159 B
content exact), `.data`/`.dynsym`/`.dynstr`/`.hash`/`.got`/`.rel.dyn`
**0**, build-id 20.  27/27 section headers, exact file size 2,187,328.

## 2. Cells re-confirmed this session (all neutral or worse)

* Tail statement permutations (`setShipGroup`; `g`; `addObject`;
  `field_0x40`; `setBV`; `ship = fixed`): the current order is the
  unique minimum.
* `empPoints > 20` form: raw 1,181.
* `missionPtr` local / `__builtin_expect` variants: neutral or worse.
* jitter/waypoint declaration permutations: neutral (documented wave
  174/176 levers, unchanged).
* Type-14 box `ey`/`ez` swaps and `[9]`/`[10]` changes: worse.
* `zero_local`, `self_alias`, `no_g`, `no_ki`, `mission = 0`,
  `else_empty`: neutral.
* All-int coordinate forms: size 2,752 (not exact); all-float forms:
  size 2,732 (not exact).

## 3. Gate

`cmake --build cmake-build-match --target check -j2` -> **RATCHET PASS**:
byte_exact **4507/4524**, linked_exact **4507/4524**, size-exact
4524/4524, data_exact 843/843, imports 207, lint CLEAN, drift **0**,
parity 5370/5370.  `tools/verify_baseline.json` untouched (the function
is still non-exact, so no gated count changes; `avg_match` is not
ratcheted).

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13499
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN5Level10createShipEiiiP8Waypointbb
# -> raw=1173 aligned=468 size=2764/2764 insn=1115/1117
python3 /home/debian/GalaxyOnFire2/scratch/w13499/seccontent.py
# .text 71810, .ARM.extab size exact diff 575, .ARM.exidx 0,
# .rel.plt 0, .rodata diff 485 (original prefix exact)
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch: `Level.cpp.base` (base), `Level.cpp.blkbest` (landed),
`mk_blkbest.py` (regenerate), `region3.py`, `seccontent.py`, `sweep.py`,
`v2.py`..`v23.py`.

## 5. Files changed

* `src/game/world/Level.cpp` — faithful race-0 hull `ArraySetLength`
  before all three stores (each box in its own local) + tuned spawn
  slot map.
* `docs/IDENTICALITY.md` — wave-191 numbers.
* `docs/VALIDATION.md` — `.text` differing bytes 71,811 -> 71,810.
* this note.
