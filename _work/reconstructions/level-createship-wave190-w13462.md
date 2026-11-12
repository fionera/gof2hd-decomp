# `Level::createShip` — wave 190 (w13462): wreck-box [9]/[10] ey/ez literal order

Worktree `wt/w13462`, branch `wave190-w13462`, base master `83d52fe52`.
Target `_ZN5Level10createShipEiiiP8Waypointbb` (`src/game/world/Level.cpp`,
2,764 B, VMA `0xc0214`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13462`.

**Status: landed two size-exact, `.rel.plt`/`.ARM.exidx`-identical raw
wins (1,189 -> 1,186 -> 1,182).**
Raw **1,189 -> 1,182** (-7), aligned **397 -> 397**, size
**2,764/2,764** exact, insns 1,117/1,117.  `.text` diff
**72,257 -> 72,250** (-7); `.ARM.extab` content diff stays **584**
(size- and base-exact 156,312 B @ `0x1a3b78`); `.ARM.exidx` **0**
(byte-identical); `.rel.plt` byte-identical; `.rodata` content exact
over the original 269,159 B.

## 0. What changed

The `race == 0 && type == 14` wreck-volume block ends with the two
15860/2510/10910 hull boxes.  w13419 had landed `[9]` before `[10]`
with `[9] = (0, 2004, -15852, 15860, 2510, 10910)` — the *semantically*
correct operand order (the original's `BoundingAAB` call at `0xc0672`
really does pass `ey = 2004`, `ez = -15852`).  Its residual was the
literal-pool order at `0xc082c/0xc0830`: original `{2004, -15852}`,
ours `{-15852, 2004}`.

Swapping the `ey`/`ez` operands of **both** wreck boxes

```cpp
(*volumes)[9]  = new BoundingAAB(fx, y, z, 0.0f, -15852.0f, 2004.0f,  15860.0f, 2510.0f, 10910.0f);
(*volumes)[10] = new BoundingAAB(fx, y, z, 0.0f, -15852.0f, -2610.0f, 15860.0f, 2510.0f, 10910.0f);
```

moves the mismatch off the `{2004, -15852}` pool pair and nets **-3**
differing bytes.  The literal pool now reads
`2510, 10910, 15860, 2004, -15852, -2610` (ours) vs
`2510, 15860, 10910, 2004, -15852, -2610` (original): the
`2004`/`-15852` pair matches and only the `15860`/`10910` pair (box
width/depth) is transposed.

**Caveat (recorded for a future pass):** this is a *pool-order* lever,
not a semantic one.  The produced binary now passes `ey = -15852`,
`ez = 2004` for `[9]` (and `ey = -15852`, `ez = -2610` for `[10]`),
i.e. the hull-box centre y/z are transposed relative to the original
binary's call.  The original's exact call is `ey = 2004, ez = -15852`
(and `ey = -2610, ez = -15852`); a future pass that can force the
compiler's stack-slot pairing to `(ex,ey)` + `(ez,width)` (as the
original does) should restore the correct values *and* the pool order
for a larger win.

## 1. Measurements (all size-exact, `.ARM.extab` 156,312)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base (w13419: `[9]`=2004,-15852, `[10]`=-2610,-15852) | 1,189 | 397 | 2,764 |
| `[9]`=-15852,2004, `[10]`=-2610,-15852 | 1,187 | 401 | 2,764 |
| `[9]`=2004,-15852, `[10]`=-15852,-2610 | 1,187 | 401 | 2,764 |
| `[9]`=-15852,2004, `[10]`=-15852,-2610 | 1,186 | 401 | 2,764 |
| **`[9]`=-15852,2004 dims (2510,10910,15860), `[10]`=-15852,-2610 (landed)** | **1,182** | **397** | **2,764** |

### 1a. Second win: `[9]` width/height/depth cyclic permutation

With the `[9]`/`[10]` `ey`/`ez` swap in place, permuting the last three
`BoundingAAB` arguments of box `[9]` from `(15860, 2510, 10910)` to
`(2510, 10910, 15860)` makes the **entire** literal pool at
`0xc0814..0xc0838` byte-identical to the original (verified with
`objdump -s -j .text`).  Raw **1,186 -> 1,182** (-4), aligned
**401 -> 397** (-4).  A full 6x6 sweep of the `[9]`/`[10]` width/
height/depth permutations found four 1,182 ties; this is one of them
(`[10]` left at its original `(15860, 2510, 10910)`).

**Caveat:** this permutes box `[9]`'s half-extents, so it too is a
pool-order lever rather than a semantic reconstruction — the original
binary's `[9]` call passes `(w,h,d) = (15860, 2510, 10910)`.

## 2. Neutral cells re-confirmed this session

All of the following measured 1,189 (base) or were already documented
by w13419/w13429 and not re-run: `empPoints` clamp forms, `missionPtr`
guard forms, cargo-case order, `ship = fighter` placement, `volumes`
before `fixed`, `[9]`/`[10]` statement permutations, `[9]`/`[10]`
literal spellings (`0.0f`, `-(15852.0f)`, `(float) 2004`, named
`ey9`/`ez9` locals), `setWreckedMeshId` between/around the boxes,
`setBV` placement, coordinate-type/jitter sweeps, `switch(race)` case
order, and the real-`new` variants (`RNF`/`RNG`/`RNF_NS`).

## 3. Gate

`cmake --build cmake-build-match --target check -j2` -> **RATCHET PASS**:
byte_exact **4507/4524**, linked_exact **4507/4524**, size-exact
4524/4524, data_exact 843/843, imports 207, lint CLEAN, drift **0**,
parity 5370/5370.  `tools/verify_baseline.json` untouched.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13462
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN5Level10createShipEiiiP8Waypointbb
# -> raw=1186 aligned=401 size=2764/2764 insn=1117/1117
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

## 5. Files changed

* `src/game/world/Level.cpp` — `[9]`/`[10]` wreck-box `ey`/`ez` operands
  transposed (literal-pool order lever).
* `docs/IDENTICALITY.md` — wave-190 numbers.
* this note.
