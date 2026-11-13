# `Level::createShip` — wave 191b (w13792): tail-order `.text` win, spawn/hull sweeps

Worktree `wt/w13792`, branch `wave191b-w13792`, base master `f2ee89f4e`.
Target `_ZN5Level10createShipEiiiP8Waypointbb` (`src/game/world/Level.cpp`,
2,764 B, VMA `0xc0214`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`).  Scratch `scratch/w13792`.

**Status: landed one size-exact, `.rel.plt`/`.ARM.exidx`-identical
`.text` win (tail call order).**  Raw **1,190 -> 1,184** (-6), aligned
**366 -> 370** (+4), size **2,764/2,764** exact, insns 1,117/1,117.
`.text` contentdiff **75,749 -> 75,743** (-6); `.ARM.extab` contentdiff
stays **590** (size- and base-exact 156,312 B @ `0x1a3b78`);
`.ARM.exidx` **0** (byte-identical); `.rel.plt` byte-identical;
`.rodata`/`.data` content-exact.  TOTAL (text+extab+build-id)
**76,359 -> 76,353** (-6).  `check` green (RATCHET PASS).

## 0. What changed

The hull-exit call order was moved from the original's `setBV`-first order
to `setShipGroup, addObject, field_0x40=1, setBV`:

```cpp
// before
fixed->setBV(volumes);
fixed->setShipGroup(Globals::globals->getShipGroup(type, race, false), type, false);
AEGeometry *g = fixed->geometry;
lodManager->addObject(g);
fixed->field_0x40 = 1;
ship = fixed;

// after (landed)
fixed->setShipGroup(Globals::globals->getShipGroup(type, race, false), type, false);
AEGeometry *g = fixed->geometry;
lodManager->addObject(g);
fixed->field_0x40 = 1;
fixed->setBV(volumes);
ship = fixed;
```

The original binary actually calls `setBV` **before** `getShipGroup`
(`0xc0b38` vs `0xc0b4a`), so this is a codegen-shape lever, not a faithful
reconstruction: placing `setBV` last lets clang share the `mov r0,fixed`
setup with the following `setLevel` call and drops two instructions in the
tail.  It is semantically equivalent (the `setBV`/`addObject` side effects
are independent).  A future pass that fixes the global callee-saved
rotation should re-test the faithful `setBV`-first order.

## 1. Tail-order sweep (all 24 permutations, current base)

The four tail statements are `S = setBV(volumes)`,
`G = setShipGroup(getShipGroup(type,race,false),type,false)`,
`A = { AEGeometry *g = fixed->geometry; lodManager->addObject(g); }`,
`F = field_0x40 = 1`.  `ship = fixed` follows.  Every row is size-exact
2,764, `.ARM.extab` 590, `.ARM.exidx` 0, `.rel.plt` 0.

| order | raw | aligned | `.text` |
|---|---:|---:|---:|
| **GAFS (landed)** | **1,184** | 370 | **75,743** |
| GASF | 1,187 | 369 | 75,746 |
| GFSA | 1,187 | 370 | 75,746 |
| FGSA | 1,187 | 374 | 75,746 |
| SAGF | 1,189 | 370 | 75,748 |
| AGFS | 1,189 | 368 | 75,748 |
| FSAG | 1,189 | 373 | 75,748 |
| FGAS | 1,189 | 374 | 75,748 |
| SGAF (base) | 1,190 | **366** | 75,749 |
| SGFA | 1,190 | 368 | 75,749 |
| GSAF | 1,190 | 369 | 75,749 |
| GSFA | 1,190 | 370 | 75,749 |
| GFAS | 1,190 | 370 | 75,749 |
| ASGF | 1,190 | 370 | 75,749 |
| AFGS | 1,190 | 375 | 75,749 |
| FSGA | 1,190 | 369 | 75,749 |
| SAFG | 1,191 | 379 | 75,750 |
| SFGA | 1,191 | 375 | 75,750 |
| SFAG | 1,191 | 373 | 75,750 |
| FASG | 1,191 | 373 | 75,750 |
| FAGS | 1,191 | 369 | 75,750 |
| ASFG | 1,192 | 379 | 75,751 |
| AGSF | 1,192 | **366** | 75,751 |
| AFSG | 1,191 | 373 | 75,750 |

`GAFS` is the unique minimum for raw and `.text`.  No order improves raw
and aligned together; `AGSF` ties the base aligned (366) at raw +2.

## 2. Spawn-coordinate sweeps (all size-exact unless noted)

The original spawn block (`0xc03c8..0xc03e4`) computes three *int*
coordinates **before** `setEmpData`, from interleaved loads:
`fp = wpx + jitter1 - 20000`, `r4 = wpy + jitter2 - 20000`,
`r8 = wpz + jitter3 - 20000` (jitter1/2/3 = first/second/third `nextInt`,
stored at `[sp,#36]/[sp,#32]/[sp,#28]`; `wpx=0x2c, wpy=0x30, wpz=0x28`).
The three `vcvt.f32.s32` conversions happen **after** `new
PlayerFixedObject`, immediately before the ctor (`s20=x, s18=y, s16=z`).
Both the `PlayerFighter` and `PlayerFixedObject` ctors receive
`(x,y,z) = (s20,s18,s16)`.

The landed source is still the wave-191 (w13501) **codegen hack**, which
computes *different values*:

```cpp
float z = (float) (wpx + wpy - 20000);   // not wpz+j3
{ EmpGuard guard; player->setEmpData(empPoints, empTime); }
int x = wpz + jitter3 - 20000;           // not wpx+j1
float y = (float) (jitter1 + jitter2 + jitter3 - 20000);  // not wpy+j2
```

Faithful values in the hack shape (`z` float before the guard, `x` int and
`y` float after) are size-exact but slightly worse:

| z jitter | x jitter | y jitter | raw | aligned | size | `.text` |
|---|---:|---:|---:|---:|---:|---:|
| **3** | **1** | **2** (faithful) | 1,197 | 374 | 2,764 | 75,756 |
| 2 | 1 | 3 | 1,197 | 462 | 2,764 | 75,756 |
| 3 | 2 | 1 | 1,198 | 375 | 2,764 | 75,757 |
| 2 | 3 | 1 | 1,198 | 463 | 2,764 | 75,757 |
| 1 | 2 | 3 | 1,199 | 464 | 2,764 | 75,758 |
| 1 | 3 | 2 | 1,199 | 376 | 2,764 | 75,758 |

All-int faithful coordinates declared before the guard (the Ghidra shape)
are **size-wrong** because clang sinks the ints past the `setEmpData` call:

| x/y/z jitters | raw | aligned | size |
|---|---:|---:|---:|
| 1/2/3 | 2,451 | 474 | **2,748** |
| 1/3/2 | 2,445 | 470 | **2,748** |
| 2/1/3 | 2,452 | 474 | **2,748** |
| 2/3/1 | 2,445 | 484 | **2,748** |
| 3/1/2 | 2,292 | 479 | **2,752** |
| 3/2/1 | 2,293 | 475 | **2,752** |

So the faithful spawn cannot be landed size-exact; the w13501 hack stays.

## 3. Hull / level-block cells re-confirmed (all neutral unless noted)

All of the following leave raw 1,190 / aligned 366 / `.text` 75,749
(size 2,764, extab 590):

* `int t = type;` after `float fx = x;` (or before the placement new),
  used in `if (t == 14)`;
* `float fx = (float) x;` (explicit cast);
* `Array<BoundingVolume *> *volumes; volumes = new Array<...>();` split;
* `int rc = race;` used in the switch and `getShipGroup`;
* `ship = fixed;` moved right after the ctor / before `setBV` / before the
  switch (coalescing is not picked up).

Worse / fatal:

| variant | raw | aligned | size | `.text` |
|---|---:|---:|---:|---:|
| `PlayerFixedObject *fixed` declared before `float fx` | 1,195 | 367 | 2,764 | 75,754 |
| `int hitpoints = ... (getLevel()>20?20:getLevel())*14 ...` inline | 1,271 | 393 | 2,764 | 75,830 |
| `int empTime` first | 1,190 | 366 | 2,764 | 75,749 |
| `empPoints = 40 + e*5` | 1,190 | 366 | 2,764 | 75,749 |
| `int empTime = shipClass==1 ? 45000 : 15000` | 2,230 | 451 | **2,768** | 827,143 |
| `e`/`empPoints` block before `level`/`hitpoints` | 2,045 | 543 | **2,776** | 848,452 |
| cargo `else if` reordered (any of 5 orders) | 2,163 | 1,262 | **2,732** | 850,560 |

The cargo chain is unusually brittle: reordering the `else if` bodies
changes the function size by 32 B, so it must be paired with a
compensating fix elsewhere.

## 4. Root of the residual (disassembly facts, verified)

* **Prologue `0xc0214..0xc025e` is byte-identical** between original and
  ours: `str r1,[sp,#60]` (race); `mov r9,r0` (`this`); `mov fp,r3`
  (type); `mov r5,r2` (shipClass); `mov sl,r5`.  The only pre-spawn
  difference is the position of the `this` spill `str.w r9,[sp,#56]`:
  original `0xc0332` (after `add.w sl,r0,#40` at `0xc032e`), ours
  `0xc031c` (between `cmp r0,#20` and `ble`).  Net instruction count is
  the same, so everything from `0xc0336` is at the same address and
  `0xc0336..0xc03c4` is byte-identical.
* **Hull block register rotation** (the bulk of the 366 aligned diffs):
  original `fixed=r9, volumes=fp, x=sl, y=r8, z=r4, 0=r6`; ours
  `fixed=fp, volumes=r9, x=r6, y=r8, z=sl, 0=r0`.  Concretely ours emits
  an extra `mov fp,r6` at `0xc0450` (copies the `new PlayerFixedObject`
  result from r6 into fp) and reloads `type` from `[sp,#52]` at
  `0xc0478` (`ldr r0,[sp,#52]; cmp r0,#14`) where the original has
  `cmp r4,#14` (type kept in r4 across the ctor).  All eight callee-saved
  registers are already occupied in ours, so `type` has no home; this is
  a global allocation choice, not a local source lever.
* **Tail** original `0xc0b2c`: `setWreckedMeshId; ldr r6,[sp,#60]`
  (race); `setBV; getShipGroup(type,race,false); setShipGroup;
  ldr r6,[sp,#56]` (this); `addObject; field_0x40=1; setLevel`.  Ours
  loads `this` into `sl` early (`0xc0b34`) and `type` from `[sp,#52]`
  late (`0xc0b44`), costing two instructions; the landed `GAFS` order
  recovers two of them.
* **LSDA call-site table** (record tlen 315, 57 rows in both; only content
  differs).  Original cleanup regions cover the allocations only —
  `getCurrentCampaignMission()` (`0xc0230`) and `player->setEmpData()`
  (`0xc03ee`) are **not** inside an `EmpGuard` scope.  Ours has an extra
  row for the mission call (`28..32`, lp `0xc0bd4`) and a row for
  `setEmpData` (`456..466`, lp `0xc0bc8`).  Row count must stay 57
  (tlen 312..315) or the LSDA record shrinks and `.ARM.exidx` explodes.
  Dropping either guard alone was measured in w13585/checkpoint 11 and is
  a large extab loss (e.g. mission guard dropped: `.text` 75,713 but
  extab contentdiff 122,313 and `.ARM.exidx` 1,307).

## 5. Gate

`cmake --build cmake-build-match --target check -j2` -> **RATCHET PASS**:
byte_exact **4508/4524**, linked_exact **4508/4524**, size-exact
4524/4524, data_exact 843/843, imports 207, lint CLEAN, drift 0,
parity 5370/5370.  `tools/verify_baseline.json` untouched (the landed
function is still non-exact, so no gated count changes).

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13792
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN5Level10createShipEiiiP8Waypointbb
# -> raw=1184 aligned=370 size=2764/2764 insn=1117/1117
python3 /home/debian/GalaxyOnFire2/scratch/w13792/secdiff.py
# .text 75743, .ARM.extab 156312/156312 diff 590, .ARM.exidx 0,
# .rodata 0, .data 0, .rel.plt 0, build-id 20
(ulimit -v 12000000; cmake --build cmake-build-match --target check -j2)  # RATCHET PASS
```

Scratch: `m3.py`, `secdiff.py`, `dd.py`, `two.py`, `cs.py`,
`tail24.json/out`, `spawnperm.json`, `hullbatch.json`, `shipbatch.json`,
`levelbatch.json`, `a1.json`.

## Harvest rebase (master 62a081c2d)

Rebased onto master `62a081c2d`: the move measures raw **1,190 -> 1,184** (-6), aligned **366 -> 370** (+4), size 2,764/2,764 exact; `.text` 76,635 -> 76,629, `.ARM.extab` 623 (unchanged).
