# `Level::createShip` — wave 191 (w13479b): second lens, Ghidra source shape

Worktree `wt/w13479`, branch `wave191b-w13479`, base master `7c06dae7a`.
Target `_ZN5Level10createShipEiiiP8Waypointbb` (`src/game/world/Level.cpp`,
2,764 B, VMA `0xc0214`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13479`.

**Status: no landable win; `src/` left at the base.**  Base is raw **1,174**,
aligned **406**, size **2,764/2,764**, insns **1,117/1,117**, `.text` diff
**71,809**, `.ARM.extab` **156,312 B diff 575**, `.ARM.exidx` **0**,
`.rel.plt` byte-identical.  `check` green (RATCHET PASS, byte_exact
4507/4524).

This is the second independent `createShip` lens.  The first (w13485) landed
the race-0 hull `b1`/`ArraySetLength` store-order lever (raw 1,176 -> 1,174).
This pass reconstructed the *authoritative source shape* from a working
headless Ghidra decompile and re-swept every remaining cell against it.  The
base is a hard local optimum: every semantically-faithful reconstruction is
**larger** than 1,174, and the residual is a global callee-saved rotation, not
a semantic mismatch.

## 0. Working headless Ghidra decompile (new capability)

```
cd /home/debian/GalaxyOnFire2 && ulimit -v 12000000 && ADDR=0xd0214 timeout 300 \
  /tmp/ghidra_12.1.2_PUBLIC/support/analyzeHeadless /home/debian/GalaxyOnFire2 GalaxyOnFire2 \
  -process android_2.0.16_libgof2hdaa.so -noanalysis -scriptPath /home/debian/ghidra_scripts \
  -postScript DecompAt2.java
```

Full decompile saved at
`/home/debian/GalaxyOnFire2/scratch/w13479/ghidra_createShip.txt` (500
lines).  Address = image base `0x10000` + `0xc0214`.

### 0a. Authoritative source shape (from the decompile)

* **Jitters**: 1st `nextInt` -> `iVar3`, 2nd -> `iVar4`, 3rd -> `iVar5`.
  Waypoint fields `+0x124` -> `local_54` (x), `+0x128` -> `iVar12` (y),
  `+300` -> `iVar2` (z).
* **Coordinates are all `int`, computed *before* `setEmpData`**:
  `x = wpx + iVar3 - 20000`, `y = wpy + iVar4 - 20000`,
  `z = wpz + iVar5 - 20000` (identity jitter mapping, x/y/z int).
  They are converted to `float` only at the two ctor calls
  (`PlayerFighter` and `PlayerFixedObject`).
* **`setEmpData` is unguarded**; the mission `getCurrentCampaignMission`
  call is unguarded; `Player`/`PlayerFixedObject`/`PlayerFighter` use real
  `new` (try-ranges).
* **Race-0 non-14 hull is the faithful order**:
  `ArraySetLength(3)` first, then box0/store0, box1/store1, box2, then a
  shared `LAB_000d0b2a` stores box2 at `+8`.
* **`setWreckedMeshId` is a shared tail**: races 0/1/2 set `r1` = the wreck
  id and jump to `c0b2c`; race 3 calls it inline (`0x477c`) and jumps past
  the shared call to `setBV`; `default` jumps straight to `setBV`.
* **Tail order**: `setBV(volumes)` -> `getShipGroup` -> virtual
  `setShipGroup` -> `addObject(fixed->geometry)` -> `field_0x40 = 1`.
* **Cargo cleanup order**: `0x2c`, `0x33`, `0x31` (0x2c and 0x31 share the
  body; 0x33 sets `field_0x25 = 0`).

## 1. What the residual actually is

Register map (orig vs ours, from disassembly):

| value | orig | ours |
|---|---|---|
| `this` | `r9` -> `[sp,#56]` -> `r6` | `r9` -> `[sp,#56]` -> `sl` |
| `fixed` | `r9` | `fp` |
| `volumes` | `fp` | `r9` |
| `x` (int, live to ctor) | `fp` | `r4` |
| `y` | `r4` | `r6` |
| `z` | `r8` | `s16` (converted before `setEmpData`) |
| `race` | `r6` | `r4` |
| `type` | `r4` (kept) | `[sp,#52]` (reloaded) |

Stack slots: orig `wpz/wpx/wpy = 40/44/48`, `jitter3/1/2 = 36/32/28`;
ours `wpz/wpx/wpy = 48/40/36`, `jitter3/1/2 = 44/32/28`.  `this=56`,
`type=52`, `race=60` in both.

The diff is dominated by (a) the coordinate block (`0x1b4..0x23e`, ~125 B),
(b) the race-0 hull/`ArraySetLength` block (`0x866..0x92e`, ~160 B), and
(c) a systematic callee-saved rotation that shifts the rest of the stream.
Nothing in the diff is a semantic mismatch.

## 2. Measurements this session (all size-exact unless noted)

Base = raw **1,174** / aligned **406**.

| variant | raw | aligned | size |
|---|---:|---:|---|
| base | **1,174** | 406 | 2,764 |
| mission `!=1 && expect(!=23)` (`exp2`; correct `cmp #1; cmpne #23`) | 1,175 | **391** | 2,764 |
| mission `expect(!=23) && !=1` | 1,175 | 391 | 2,764 |
| mission `!=23 && !=1` / no-expect | 1,181 | 397 | 2,764 |
| `empPoints` `>20 ? 20 : getLevel` (`gt`) | 1,181 | 404 | 2,764 |
| `empPoints` `getLevel() < 21 ? ... : 20` (`lt21`) | 1,174 | 406 | 2,764 |
| `level`/`empPoints` `unsigned` | 1,174 | 406 | 2,764 |
| `hitpoints` `unsigned` | 1,177 | 409 | 2,764 |
| all-int `x,y,z` before `setEmpData`, convert at ctor (implicit) | 2,108 | 521 | **2,752** |
| all-int `x,y,z` before `new Player` | 2,108 | 521 | 2,752 |
| all-int `x,y,z` + `float fx/fy/fz` locals | 2,221 | 750 | 2,760 |
| all-int `x,y,z` + real `new` (both ctors) | 2,384 | 467 | **2,792** |
| real `new PlayerFixedObject` only | 2,434 | 504 | 2,768 |
| real `new PlayerFighter` only | 2,278 | 466 | 2,764 |
| `x`/`y`/`z` all before guard (`zxy` / `xyz` / `zyx`) | 1,195 / 1,191 / 1,195 | 407/406/407 | 2,764 |
| `int x, y, z;` assigned inside the `EmpGuard` scope | 2,244 | 505 | 2,752 |
| no `setEmpData` guard | 2,180 | 506 | 2,780 |
| `notop` (remove top guard) | **1,138** | 379 | 2,764 |
| `notop` + `setLevel` guard | 1,174 | 406 | 2,764 |
| `notop` + `gameWon` guard | 1,202 | 414 | 2,764 |
| `notop` + `hardCore`/`getShipGroup`/`addObject` guard | 2,465 / 2,374 / 2,354 | ~500 | 2,768/2,776/2,784 |
| faithful hull (`ASL` first, 3 stores in order, `L001122`) | 1,182 | 397 | 2,764 |
| hull `box0;ASL;store0;box1;store1;box2;store2` | 1,176 | 404 | 2,764 |
| hull `box0;store0;box1;store1;ASL;box2;store2` | 1,174 | 406 | 2,764 |
| faithful tail (`setBV` first) | 1,180 | 402 | 2,764 |
| `setWreckedMeshId` hoist via `wreckId` + `if (race<=2)` | 2,554 | 1,085 | 2,760 |
| `setWreckedMeshId` ternary after if/else | 2,596 | 1,104 | 2,776 |
| `if (type == 14)` inverted | 1,174 | 406 | 2,764 |
| cargo `2c,33,31` (Ghidra order) | 1,174 | 406 | 2,764 |
| cargo `switch` | 1,175/1,178 | 391 | 2,764 |
| `ship` initialiser moved (3 positions) | 1,174 | 406 | 2,764 |
| `Level *self = this` (3 positions) | 1,174 | 406 | 2,764 |
| `EmpGuard` `noexcept` / `throw()` | 1,174 | 406 | 2,764 |
| `EmpGuard` `= default` / empty struct | 2,180 | 479 | 2,776 |
| `(void) missionPtr;` after `empPoints` | 1,174 | 406 | 2,764 |
| `int mp = missionPtr;` after / at top | 2,274 / 2,481 | 471/478 | 2,772 |
| `if (missionPtr == -1) return nullptr;` after `empPoints` | 2,455 | 622 | 2,788 |

### 2a. Exhaustive hull interleaving sweep

All 140 interleavings of `{box0, box1, box2, store0, store1, store2,
ArraySetLength}` with the boxes in construction order and the stores in
index order (`sweep_hull4.py`): the unique minimum is **1,174**, attained
by both the base (`box0;store0;box1;ASL;store1;box2;store2`) and
`box0;store0;box1;store1;ASL;box2;store2`.  Of the 48 *semantically
correct* interleavings (`ASL` before every store) the minimum is **1,176**
(`box0;ASL;store0;box1;store1;box2;store2`); the faithful Ghidra order is
**1,182**.

### 2b. Random multi-lever search

160 random combinations of 18 transforms (gt ternary, faithful tail,
faithful hull, hull `0011L22`, cargo order, mission `exp2`/`rev_exp`,
`empTime` init, waypoint z-first, jitter order, `fx` after `new`, `ship`
split, no `g`, no `kiFighter`, `geo` local, `self` alias, armor invert,
`mission = 0`): best is **1,174** (the base).  `exp2` alone is the only
sub-1,180 non-base result at 1,175/391.

## 3. The two hard blockers

1. **All-int coordinates are 12 B short.**  The original computes `x,y,z`
   as `int`s before `setEmpData` and converts at the ctor (6 `vcvt` total).
   Every source form that expresses this compiles to 2,748–2,760 B (12–16 B
   *smaller* than the original).  Clang's scheduler sinks the int
   computation past `setEmpData` (and past `new Player`) regardless of
   declaration order, scope, or `EmpGuard` placement; the base's hybrid
   (`float z` before the guard, `int x` / `float y` after) is the only
   size-exact form and is the unique 1,174 optimum of the 96-form
   `sweep_xyzfull.py` sweep.
2. **`notop` is extab-fatal.**  Removing the top `EmpGuard` gives raw
   **1,138** (-36) and `.text` 71,773, but the LSDA loses one 5-byte row:
   `.ARM.extab` becomes **156,308** (4 B short), which shifts `.rodata` and
   `.ARM.exidx` (diff 0 -> 1,307) and explodes the extab content diff
   (575 -> 122,299).  Every compensating guard adds **two** rows
   (156,316/156,320), never the single 4-byte row needed; a guard at a
   try-range boundary is the only +5-byte row and the mission row itself
   is that boundary.

## 4. Conclusion

The base is a hard local optimum.  The residual is a clang callee-saved
rotation (`fixed=fp` vs `r9`, `volumes=r9` vs `fp`, `z` converted early
into `s16`, `type` reloaded from `[sp,#52]`) driven by the coordinate
types, not by any remaining semantic mismatch.  No size-exact,
`.rel.plt`-identical, net-positive win was found; `src/` is left at the
base.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13479
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN5Level10createShipEiiiP8Waypointbb
# -> raw=1174 aligned=406 size=2764/2764 insn=1117/1117
python3 /home/debian/GalaxyOnFire2/scratch/w13479/b_sec.py \
  cmake-build-match/libgof2hdaa.so
# .text diff 71809, .ARM.extab 156312 diff 575, .ARM.exidx 0, .rel.plt 0
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch: `sweep_hull3.py`, `sweep_hull4.py`, `randsearch2.py`,
`try_faith*.py`, `try_coord2.py`, `try_nosemp*.py`, `try_notop*.py`,
`try_wreck*.py`, `try_mp*.py`, `try_emp*.py`, `try_guard3.py`,
`try_shippos.py`, `cmp_secs.py`, `cmp_exp2.py`, `cmp_b3.py`, `cmp_t1.py`,
`ghidra_createShip.txt`.

## 6. Files changed

* this note only (no `src/` change; tree left clean at the base).
