# `Level::createShip` — wave 190 (w13419): wreck-box [9] ey/ez operand order

Worktree `wt/w13419`, branch `wave190-w13419`, base master `03b9dfe03`.
Target `_ZN5Level10createShipEiiiP8Waypointbb` (`src/game/world/Level.cpp`,
2,764 B, VMA `0xc0214`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13419`.

**Status: landed a size-exact, `.rel.plt`/`.ARM.exidx`-identical raw win.**
Raw **1,190 -> 1,189** (-1), aligned **403 -> 397** (-6), size
**2,764/2,764** exact, insns 1,117/1,117.  `.text` diff
**72,934 -> 72,933** (-1); `.ARM.extab` content diff stays **603**
(size- and base-exact 156,312 B @ `0x1a3b78`); `.ARM.exidx` 0;
`.rel.plt` byte-identical; `.rodata` content exact over the original
269,159 B.

## 0. What changed

The `race == 0 && type == 14` wreck-volume block ends with the two
15860/2510/10910 hull boxes.  Wave 190 w13403 swapped the *statement*
order to `[10]` then `[9]`, but left the `[9]` constructor's `ey`/`ez`
operands in the order `-15852.0f, 2004.0f`.  Decoding the original's
`BoundingAAB` call at `0xc0672` (store to `volumes[9]`, offset 36)
shows the original passes `ey = 2004.0f`, `ez = -15852.0f`, and emits
`[9]` *before* `[10]`:

```
orig c0654  ldr r0,[pc] -> 2510     ; height
orig c0656  ldr r1,[pc] -> 15860    ; width
orig c0658  ldr r2,[pc] -> 10910    ; depth
orig c065e  ldr r0,[pc] -> 2004     ; ey
orig c0662  ldr r2,[pc] -> -15852   ; ez
orig c066a  strd r4,r0,[sp]         ; ex=0, ey=2004
orig c066e  strd r2,r1,[sp,#8]      ; ez=-15852, width=15860
```

The current source had `[10]` first and `[9]` as
`(0, -15852, 2004, 15860, 2510, 10910)`.  Changing it to

```cpp
(*volumes)[9]  = new BoundingAAB(fx, y, z, 0.0f, 2004.0f, -15852.0f, 15860.0f, 2510.0f, 10910.0f);
(*volumes)[10] = new BoundingAAB(fx, y, z, 0.0f, -2610.0f, -15852.0f, 15860.0f, 2510.0f, 10910.0f);
```

restores the original argument pairing and emission order.

## 1. Measurements

| variant | raw | aligned | size | .text | extab |
|---|---:|---:|---:|---:|---:|
| base (`[10]` then `[9]`, `[9]` = -15852,2004) | 1,190 | 403 | 2,764 | 72,934 | 603 |
| **`[9]` then `[10]`, `[9]` = 2004,-15852 (landed)** | **1,189** | **397** | **2,764** | **72,933** | **603** |
| `[10]` then `[9]`, `[9]` = 2004,-15852 | 1,192 | 399 | 2,764 | 72,936 | 603 |
| `[9]` then `[10]`, `[9]` = -15852,2004 | 1,192 | 396 | 2,764 | 72,936 | 603 |

The landed form is the unique minimum of the four.  The literal-pool
words at `0xc0820..0xc0834` are now `2510, 15860, 10910, -15852, 2004,
-2610` in ours vs `2510, 15860, 10910, 2004, -15852, -2610` in the
original — the two unique `[9]` constants are still emitted in the
opposite order (the scheduler loads `ez` before `ey` in our build), so
`0xc082c..0xc0834` still differs; that residual is a scheduler load
order, not a source-value error.

## 2. Other cells re-measured (all size-exact unless noted)

On the landed base, re-checked with `.ARM.extab` size/base exact and
`.ARM.exidx` 0 as the acceptance filter:

* coordinate type/shape sweep: `iii` 2,417 (size 2,752), `iff` 1,207,
  `fif` 1,214, `ffi` 1,220, `iif` 2,235 (size 2,768), `ifi` 2,382
  (size 2,768), `fff` 2,464 (size 2,732); the landed
  `float z` before / `int x` + `float y` after stays best.
* `EmpGuard` removal: top guard 1,154 raw but `.ARM.extab` 156,308
  (4 B short, gate-fatal); setEmpData guard 2,181 (size 2,780);
  both 2,181 (size 2,780).
* real `new` for `PlayerFighter`/`PlayerFixedObject` + no guards:
  1,988, size 2,764, extab 156,312 (diff 602) but `.text` 73,732.
* `ship = nullptr` moved after `empTime`/`Player`/`z`/guard/`y`:
  neutral (1,189).  `volumes` declared before `fixed`: neutral.
  `ship = fighter` before the LOD block: neutral.
* empPoints forms `40 + 5*...`, `5*... + 40`, `(... + 8)*5`,
  `... * 5; ... += 40`: neutral.  `> 20 ? 20 : getLevel()`: 1,196.
* natural jitter pairing (first->x, second->y, third->z): 1,191.

## 3. Gate

`cmake --build cmake-build-match --target check -j2` -> **RATCHET PASS**:
byte_exact **4507/4524**, linked_exact **4507/4524**, size-exact
4524/4524, data_exact 843/843, imports 207, lint CLEAN, drift **0**,
parity 5370/5370.  `tools/verify_baseline.json` untouched.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13419
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN5Level10createShipEiiiP8Waypointbb
# -> raw=1189 aligned=397 size=2764/2764 insn=1117/1117
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

## 5. Files changed

* `src/game/world/Level.cpp` — `[9]` wreck box emitted before `[10]`
  with the original `ey`/`ez` operand order.
* `docs/IDENTICALITY.md` — wave-190 numbers.
* this note.
