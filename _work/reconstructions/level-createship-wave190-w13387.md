# `Level::createShip` — wave 190 (w13387): split fighter cargo cleanup

Worktree `wt/w13387`, branch `wave190-w13387`, base master `dd8cf7a29`.
Target `_ZN5Level10createShipEiiiP8Waypointbb` (`src/game/world/Level.cpp`,
2,764 B, VMA `0xc0214`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13387`.

**Status: landed a size-exact, `.rel.plt`-identical raw win.**  Raw
**1,195 -> 1,192** (-3), aligned **396** unchanged, size
**2,764/2,764** exact, insns **1,117/1,117**.  `.text` diff
**73,289 -> 73,286** (-3); `.ARM.extab` content diff stays **603**
(size- and base-exact 156,312 B @ `0x1a3b78`); `.ARM.exidx` 0;
`.rel.plt` byte-identical; `.rodata` content exact over the original
269,159 B.

## 0. What changed

In the fighter arm the cargo cleanup was

```cpp
if (type == 0x2c || type == 0x31) {
    delete fighter->cargo;
    fighter->cargo = nullptr;
} else if (type == 0x33) {
    fighter->field_0x25 = 0;
}
```

Splitting the shared `0x2c || 0x31` test into two `else if` arms with the
same body

```cpp
if (type == 0x2c) {
    delete fighter->cargo;
    fighter->cargo = nullptr;
} else if (type == 0x31) {
    delete fighter->cargo;
    fighter->cargo = nullptr;
} else if (type == 0x33) {
    fighter->field_0x25 = 0;
}
```

lets clang fold the two independent tests into the original's
`cmp #0x31; it ne; cmpne #0x2c` pair instead of the base's three
separate `cmp/beq` pairs, and the region `0xc0742..0xc074c` becomes
closer to the original.  Everything else in the function is unchanged
(same size, same instruction count, same `.ARM.extab`).

## 1. Measurements

| variant | raw | aligned | size | insn |
|---|---:|---:|---:|---:|
| base (`type == 0x2c \|\| type == 0x31`) | 1,195 | 396 | 2,764 | 1,117/1,117 |
| **split `0x2c`, `0x31`, `0x33` (landed)** | **1,192** | **396** | **2,764** | **1,117/1,117** |
| split `0x2c`, `0x33`, `0x31` | 1,192 | 396 | 2,764 | 1,117/1,117 |
| split `0x31`, `0x2c`, `0x33` | 1,195 | 396 | 2,764 | 1,117/1,117 |
| split `0x31`, `0x33`, `0x2c` | 1,195 | 396 | 2,764 | 1,117/1,117 |
| `switch (type) { 0x2c: case 0x31: ... }` | 1,195 | 396 | 2,764 | 1,117/1,117 |
| `0x33` first, then `0x2c`/`0x31` | 2,038 | 1,297 | 2,764 | 1,118/1,117 |

The win requires the `0x2c` arm to be first; `0x33`-first changes the
whole fighter-arm layout and is much worse.

## 2. Other cells swept (all size-exact unless noted)

This pass also re-measured the remaining callee-saved register rotation
and the `shipClass == 1` branch shape.  Nothing else beat 1,195:

* coordinate-local shapes: all-int before `setEmpData` (2,237/2,752),
  all-float before (2,466/2,732), z-int + late convert (1,359),
  `int x`/`float y` before the guard (1,216); the landed
  `float z` before / `int x` + `float y` after stays best.
* `empPoints` clamp polarity `> 20 ? 20 : getLevel()`: 1,202
  (aligned 394); `< 21` form neutral.
* tail order `setBV` first (matches the original's call order):
  1,201 (aligned 392); `ship = fixed` early / `g` local removal /
  `volumes` declaration swap: neutral; `lodManager->addObject(fixed->geometry)`
  (no `g` local): 1,197.
* `missionPtr` comparison order `missionPtr != 1 && expect(!= 23)`:
  1,196 (aligned 381); removing the expect: 1,199; nested expect: neutral.
* `if (shipClass == 1)` first / `switch (shipClass)` / two separate
  `if`s: sizes 2,792 / 2,788 / 2,744.
* `float fx` placement after `::operator new`: 1,200; `(float)x` at the
  ctor: 1,200; `fy`/`fz` locals: neutral.
* `switch (race)` case orders `0,1,2,3` / `0,3,1,2` / `0,2,3,1`:
  1,342 / 1,280 / 1,347; `0,3,2,1` (current) best.
* real-`new` for `PlayerFighter`/`PlayerFixedObject` + no `EmpGuard`s
  (the faithful 28-LP LSDA shape): size-exact 2,764 but raw 1,989.
* zero-hoisting formulations (`fx - fx`, `fx * 0.0f`, ...): sizes
  2,760..2,772, raw >= 1,811.

## 3. Gate

`cmake --build cmake-build-match --target check -j2` -> **RATCHET PASS**:
byte_exact **4507/4524**, linked_exact **4507/4524**, size-exact 4524/4524,
data_exact 843/843, imports 207, lint CLEAN, drift **0**, parity 5370/5370.
`tools/verify_baseline.json` untouched.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13387
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN5Level10createShipEiiiP8Waypointbb
# -> raw=1192 aligned=396 size=2764/2764 insn=1117/1117
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch: `run.sh` (splice+build+measure), `body_base.txt`, `v_*.txt`
(~140 variant bodies).

## 5. Files changed

* `src/game/world/Level.cpp` — split the fighter cargo cleanup into
  `0x2c` / `0x31` arms.
* `docs/IDENTICALITY.md` — wave-190 numbers.
* this note.

## Current-master harvest (wave 190)

Re-applied the split fighter cargo cleanup on current master (`26da752c0`):
`type == 0x2c` and `type == 0x31` now have separate `delete fighter->cargo`
arms instead of the shared `||`. createShip raw **1,195 -> 1,192** (-3),
aligned 396 unchanged, size 2,764/2,764. `.text` contributes -3 to the
combined 73,248 -> 73,233 harvest. `check` green, RATCHET PASS.
