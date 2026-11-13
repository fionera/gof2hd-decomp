# `Level::createShip` — wave 191b (w13598): Ghidra source-shape oracle, no landable win

Worktree `wt/w13598`, branch `wave191b-w13598`, base master `b957fbd12`.
Target `_ZN5Level10createShipEiiiP8Waypointbb` (`src/game/world/Level.cpp`,
2,764 B, VMA `0xc0214`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`).  Scratch `scratch/w13598`.

**Status: no landable win.  Tree left at the pristine baseline.**
Raw **1,164**, aligned **400**, size **2,764/2,764** exact,
`.ARM.extab` **156,312** (content diff **577**), `.ARM.exidx` content
diff **0**, `.rel.plt` byte-identical, `.rodata`/`.data`/`.got`/`.dyn*`
exact.  `check` -> **RATCHET PASS** (byte_exact 4508/4524, imports 207,
lint CLEAN, drift 0, parity 5370/5370).

## 0. New evidence: the real Ghidra decompile of the original

The Ghidra project loads the image at base `0x10000`, so the function is
`0xd0214` (not `0xc0214`).  A headless decompile
(`scratch/w13598/gscripts/DecShip.java`, `analyzeHeadless -process
android_2.0.16_libgof2hdaa.so -noassembly`) gives the original's
source shape.  The relevant parts:

```c
iVar3 = (*pcVar18)(*puVar17, 40000);          // jitter a
iVar4 = (*pcVar18)(*puVar17, 40000);          // jitter b
iVar5 = (*pcVar18)(*puVar17, 40000);          // jitter c
...
iVar6 = Status::getLevel(...); if (iVar6 < 0x15) iVar6 = getLevel(); else iVar6 = 0x14;
iVar7 = gameWon(); iVar13 = iVar1 << 2; if (iVar7) iVar13 = 0xb4;
iVar6 = iVar13 + iVar6 * 0xe + 0x14;
...
this_00 = (Player *)FUN_0006eb24(0x114);      // operator new
Player::Player(this_00, armor, iVar6, 1, 1, 0);
iVar3 = local_54 + iVar3 + -20000;            // x = wpx + jitter
iVar1 = iVar12 + iVar4 + -20000;              // y = wpy + jitter
fVar20 = (float)(iVar2 + iVar5 + -20000);     // z = (float)(wpz + jitter)
Player::setEmpData(this_00, iVar7, iVar13);
this_02 = (PlayerFixedObject *)0x0;           // ship = nullptr, right after setEmpData
if (param_2 == 0) {
  this_02 = (PlayerFixedObject *)FUN_0006eb24(0x2f0);   // regular new
  ... VectorSignedToFloat(x); ... VectorSignedToFloat(y); ... VectorSignedToFloat(z);
  PlayerFighter::PlayerFighter(this_02, type, race, player, 0, x, y, z, hostile);
  uVar11 = Globals::getShipGroup(type, race, group);
  (**(code **)(*(int *)this_02 + 8))(this_02, uVar11, type, hostile);   // setShipGroup
  if (*(int *)(this + 0xc0) != 1 && *(int *)(this + 0xc0) != 0x17)
      LODManager::addObject(*(LODManager **)this, geometry);
  ...
}
if (param_2 != 1) goto tail;
this_02 = (PlayerFixedObject *)FUN_0006eb24(0x1bc);       // regular new
... VectorSignedToFloat(x/y/z) ...
PlayerFixedObject::PlayerFixedObject(this_02, type, race, player, 0, x, y, z);
this_01 = (Array<BoundingVolume*> *)FUN_0006eb24(0xc); Array::Array(this_01);
switch (race) {
case 0:
  if (type != 0xe) {
    ArraySetLength<BoundingVolume*>(3, this_01);          // ASL FIRST
    pBVar10 = new BoundingAAB(...); **(this_01 + 4) = pBVar10;
    pBVar10 = new BoundingAAB(...); *(*(this_01 + 4) + 4) = pBVar10;
    pBVar10 = new BoundingAAB(...); *(*(this_01 + 4) + 8) = pBVar10;
    ...
  }
  ...
}
PlayerFixedObject::setBV(this_02, this_01);               // setBV FIRST
uVar11 = Globals::getShipGroup(type, race, false);
(**(code **)(*(int *)this_02 + 8))(this_02, uVar11, type, 0);
LODManager::addObject(*(LODManager **)this, *(AEGeometry **)(this_02 + 8));
this_02[0x40] = 1;
tail:
(**(code **)(*(int *)this_02 + 0x14))(this_02, this);     // setLevel
return this_02;
```

So the original source is: **all-int spawn coordinates before `setEmpData`**,
`ship = nullptr` immediately after it, **regular `new`** for both
`PlayerFighter` and `PlayerFixedObject`, **no `EmpGuard`s at all**, the
race-0 non-14 `ArraySetLength` **first**, and `setBV` **before**
`getShipGroup`/`setShipGroup`/`addObject`/`field_0x40`.

### 0.1 The LSDA confirms "regular new, no guards"

The original's LSDA has **28 landing pads**; ours has **29**.  Counting
`new` expressions in the function: `Player` (1) + `PlayerFighter` +
`PlayerFixedObject` (2) + `Array<BoundingVolume*>` (1) + 24 hull
`BoundingAAB`s = **28**.  The original's pads are exactly the 28 `new`s
(its shared pad cluster at `0xc0bba..0xc0bd0` deletes `sl`, `fp`, `r9`,
`r5` = PlayerFighter, Array, PlayerFixedObject, Player).  Our baseline
has 26 regular-new pads (the two placement-`new` objects have none) plus
three fake `EmpGuard` pads (top, `setEmpData`, `setShipGroup`) = 29.
Both happen to land on 57 rows / cst 315 / record 336, which is why the
fake-guard form is LSDA-size-exact.

## 1. Why the baseline still wins: the spawn-coordinate form

The faithful all-int spawn block (`x = wpx+jitter3`, `y = wpy+jitter1`,
`z = wpz+jitter2`, all before `setEmpData`) is **size 2,752** (12 bytes
short), never 2,764.  The compiler sinks the three ints past
`setEmpData` (MachineSink moves them from bb.19 to bb.20), and the
resulting register allocation is globally different (prologue
`strd r1,r2,[sp,#56]`, `mov sl,r3`, `this` spilled immediately, ...),
raw **2,297**.  Source position of the block (before `Player`, inside
the guard, after the guard) is irrelevant: all forms give 2,752/2,297.

The landed w13501 form keeps the baseline's allocation by computing
**one** coordinate (`z = (float)(wpx+wpy-20000)`) *before* `setEmpData`
and the other two after.  Its expressions are deliberately
non-faithful; only that shape keeps the first 98 instructions
byte-identical to the original and the size at 2,764.

## 2. Measured this session (all size-exact 2,764 unless noted)

### 2.1 Faithful source shape (Ghidra structure) — size-exact but raw ~2,000

| variant | raw | size | extab | exidx |
|---|---:|---:|---:|---:|
| regular new both, no guards (baseline coords) | 2,096 | 2,764 | 156,312 / 580 | 0 |
| regular new both, no guards, faithful all-int coords | 2,362 | **2,784** | 156,312 / 1,271 | 3,452 |
| + ASL first | 2,363 | 2,784 | 1,270 | 3,452 |
| + setBV first | 2,366 | 2,784 | 1,271 | 3,452 |
| + ASL + setBV (fully faithful, baseline coords) | **1,991** | 2,764 | 156,312 / **575** | 0 |
| regular new both, guards kept | 2,439 | 2,772 | 156,336 | 4,944 |

`faithful-all-basecoords` is the best *structurally faithful* form: size
and record exact, extab content 575 (2 better than baseline), but raw
1,991 because the regular-`new` allocation moves the whole hull block.

### 2.2 Spawn-coordinate sweeps (all before/after permutations, faithful exprs)

8 int/float type patterns x 8 pre-sets = 64 cells; the type pattern is
**irrelevant** (`iif` == `ffi` == `fii` == ... — clang sinks the
`(float)` conversion identically).  Best faithful-expression cell:
`pre=x` -> 2,237 / size 2,764 / extab 597.  Nothing beats 1,164.
Expression reassociations of `z` (`wpy+wpx`, `wpx-20000+wpy`,
`wpx+(wpy-20000)`, `wpx+wpy+-20000`) are all neutral (1,164).
`(float)(wpx+wpy)-20000.0f` and `(float)wpx+(float)wpy-20000.0f` are
worse (2,129 / 2,290).

### 2.3 Tail / hull order (Ghidra-faithful) — all worse

| variant | raw | extab |
|---|---:|---:|
| baseline | **1,164** | 577 |
| `setBV` first | 1,170 | 577 |
| ASL first | 1,172 | 576 |
| `setBV` + ASL | 1,178 | 576 |
| `lodManager->addObject(fixed->geometry)` (no `g` local) | 1,166 | 577 |
| `setBV` before `field_0x40` | 1,167 | 577 |
| `addObject` before `setShipGroup` | 1,169 | 577 |
| `ship = nullptr` right after `setEmpData` | 1,164 | 577 |
| `this->lodManager` / `this->missionPtr` | 1,164 | 577 |
| `switch (shipClass)` instead of if/else-if | 2,278 | 1,279 (size 2,772) |
| `PlayerFixedObject *ship` (typed phi) | 2,337 | 1,296 (size 2,788) |

### 2.4 Guard-placement restoration of the w13529 drop-top win

Drop-top alone is raw **1,128** but extab 156,308 / exidx 1,307.
Re-adding an `EmpGuard` to restore the extab size:

| variant | raw | extab size | exidx |
|---|---:|---:|---:|
| drop top + guard `jitter3` | 2,371 | 156,316 | 2,928 |
| drop top + guard `jitter1` | 2,368 | 156,316 | 2,928 |
| drop top + guard `jitter2` | 1,462 | 156,316 | 1,304 |
| drop top + guard `level` | 1,189 | 156,320 | 1,325 |
| drop top + guard `setLevel` | 1,164 | 156,320 | 1,325 |
| drop top + guard `getShipGroup` | 1,164 | 156,316 | 1,304 |

Only the original top guard restores 156,312 (it is the only 5-byte
row), so the drop-top win cannot be re-landed under the extab rule.

### 2.5 Drop-setShipGroup + one 2-row guard (the only admissible class)

| variant | raw | extab | exidx |
|---|---:|---:|---:|
| drop `setShipGroup` guard (alone) | 1,160 | 156,304 | 1,327 |
| + guard `getShipGroup` | **1,165** | 156,312 / **576** | 0 |
| + guard `setLevel` | 1,165 | 156,312 / 631 | 0 |
| + guard `addObject` | 2,409 | 156,312 / 1,321 | 3,451 (size 2,788) |
| + guard `setBV` | 1,171 | 156,312 / 632 | 0 |
| + guard `setWreckedMeshId(0x4780)` | 1,168 | 156,308 | 1,307 |
| + guard `ArraySetLength(3)` | 1,170 | 156,308 | 1,307 |

`drop setShipGroup + guard getShipGroup` is the best admissible
"restored" form: raw **1,165** (+1 vs baseline) and extab content **576**
(-1), i.e. a text+extab tie (1741 vs 1741), not a raw win.  Combining it
with any neutral lever (`this->lodManager`, `ship` position, clamp
reassociation, `EmpGuard` dummy member, `z` reassociation) stays
1,165/576.

## 3. Root cause of the remaining 1,164 bytes

Four blocks account for 1,146 of the 1,164 differing bytes:

* `0xc0314-0xc0344` (16): the `empPoints` clamp.  The original is
  `min(getLevel(),20)*5+40` with a `ble/movs #20/b` shape; ours is the
  folded `(lv<=20?lv*5:100); +=40` with `bgt/.../b.n`.  The faithful
  `min` form is *worse* (26 bytes in this block, raw 1,174).  The
  `str.w r9,[sp,#56]` (`this` spill) is hoisted to `0xc031c` inside the
  branch in ours, but sits at `0xc0332` after the merge in the original.
  That hoist is already present at Greedy RA (it is not a scheduler
  artifact) and is the root of the whole-function register swap.
* `0xc03c4-0xc0464` (127): spawn coords + `Player` ctor + `new` +
  `Array` ctor.  Ours uses `ldrd r1,r0,[sp,#44]` (wpx/wpy are adjacent
  slots) and converts `z` before `setEmpData`, `y` after, `x` before
  `new`; the original loads wpx/wpy separately and converts all three
  after `new`.
* `0xc0474-0xc0774` (696) and `0xc0a64-0xc0be4` (307): the race-0
  type-14 and non-14 hull blocks.  These are a systematic register swap
  driven by the `this`-spill hoist: original
  `fighter=sl, fixed=r9, volumes=fp, ship=r9`; ours
  `fighter=fp, fixed=fp, volumes=r9, ship=fp`.  The race-0 non-14 block
  also has `ArraySetLength` in the wrong place (ours between b1's `new`
  and b1's store; the original first) — but the faithful order is worse
  (raw 1,172).

The remaining ~18 bytes are literal-pool / GOT addends
(`0xc0838`, `0xc0cdc`, ...) that follow the pool layout and are not
source-addressable.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13598
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN5Level10createShipEiiiP8Waypointbb      # raw=1164 aligned=400 size=2764/2764
python3 scratch/w13598/blockdiff.py          # per-16B diff histogram
python3 scratch/w13598/lsda2.py cmake-build-match/libgof2hdaa.so 0xc0214
# Ghidra oracle (project copy in scratch, base 0x10000):
#   analyzeHeadless <projdir> android_2.0.16_libgof2hdaa.so_ghidra \
#     -process android_2.0.16_libgof2hdaa.so -noanalysis \
#     -scriptPath scratch/w13598/gscripts -postScript DecShip.java
```

Scratch tools: `measure.sh` (build+fn_diff+secdiff), `blockdiff.py`,
`idiff.py` / `rdiff.py` (instruction-level, register-normalised),
`sdiff.py` (shift-aware byte diff), `lsda2.py`, `cc.sh` (single-TU
compile with arbitrary `-mllvm` flags), `DecShip.java`.

## 5. Files changed

* this note only; `src/game/world/Level.cpp` is unchanged from `HEAD`.
