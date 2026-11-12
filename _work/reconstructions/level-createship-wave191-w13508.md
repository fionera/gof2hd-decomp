# `Level::createShip` — wave 191 (w13508): faithful race-0 `ArraySetLength`/box order

Worktree `wt/w13508`, branch `wave191b-w13508`, base master `d4b605b7a`.
Target `_ZN5Level10createShipEiiiP8Waypointbb` (`src/game/world/Level.cpp`,
2,764 B, VMA `0xc0214`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13508`.

**Status: landed a size-exact, `.rel.plt`/`.ARM.exidx`-identical
structural win.**  Raw **1,174 -> 1,182** (+8), aligned **406 -> 397**
(-9), verify pct **69.7 -> 70.3**.  `.text` diff **71,491 -> 71,499**
(+8); `.ARM.extab` content diff **577 -> 576** (-1) at unchanged size
156,312 B @ `0x1a3b78`; `.ARM.exidx` **0** (byte-identical); `.rel.plt`
byte-identical; `.data` diff 3 -> 2; `.rodata` 485 unchanged.  `check`
green (RATCHET PASS).

## 0. What changed

The race-0 non-14 hull block was restored to the original's exact
`ArraySetLength`/store order:

```cpp
ArraySetLength(3, *volumes);
(*volumes)[0] = new BoundingAAB(fx, y, z, 0.0f, -73.0f, 123.0f, 3000.0f, 2860.0f, 10430.0f);
(*volumes)[1] = new BoundingAAB(fx, y, z, 0.0f, -280.0f, -4257.0f, 3470.0f, 1815.0f, 2200.0f);
(*volumes)[2] = new BoundingAAB(fx, y, z, 0.0f, -770.0f, -4279.0f, 5220.0f, 680.0f, 2270.0f);
```

The w13485 form (`b0; store[0]; b1; ASL; store[1]; box2`) had moved the
resizer between box 1 and box 2 for a raw-only -2.  That form is
semantically wrong: `ArraySetLength` (`0xa50c8`) calls
`__aeabi_memclr4` over the resized array, so the pre-resize stores are
zeroed and slots 0/1 are lost.  The original disassembly at `0xc0a7e`
(`movs r0,#3; mov r1,fp; blx ArraySetLength`) is followed by the three
`new BoundingAAB` / `str` pairs in index order, and Ghidra's decompile
(`0xd0214`) confirms:

```
ArraySetLength<BoundingVolume*>(3,(Array *)this_01);
pBVar10 = operator_new(0x2c); BoundingAAB::BoundingAAB(...); **data = pBVar10;
pBVar10 = operator_new(0x2c); BoundingAAB::BoundingAAB(...); *(data+4) = pBVar10;
pBVar10 = operator_new(0x2c); BoundingAAB::BoundingAAB(...); *(data+8) = pBVar10;
```

## 1. Waypoint store / stack-slot survey

`get_struct_layout`/disassembly put `Waypoint::x/y/z` at `0x124/0x128/0x12c`
(our class matches).  The original loads them into stack slots
`wpx=0x2c, wpy=0x30, wpz=0x28`; our compiler assigns
`wpx=0x28, wpy=0x24, wpz=0x30` (5 one-byte `str`/`ldr` diffs).  Every
source form tried leaves the assignment unchanged: all 6 waypoint
declaration permutations, all 6 jitter declaration permutations (36
combinations), interleaved declarations, initialized declarations,
ternary forms, branch inversion, pointer aliases, and all 24 statement
orders of `{x,y,z,setEmpData}`.  The slot layout is a register-allocator
artifact (the `this` spill at `0xc031c` vs the original `0xc0332` is the
root; it also swaps the `fixed`/`volumes`/`type`/`race` register roles
for the whole hull block).  No source-level lever found.

## 2. Measurements (all size-exact 2,764, `.rel.plt` identical)

| variant | raw | aligned | pct | .text | extab | data |
|---|---:|---:|---:|---:|---:|---:|
| base (w13485 split ASL) | 1,174 | 406 | 69.7 | 71,491 | 577 | 3 |
| **faithful ASL first (landed)** | **1,182** | **397** | **70.3** | **71,499** | **576** | **2** |
| faithful ASL + tail `setBV` first | 1,188 | 393 | 70.4 | 71,505 | 576 | 2 |
| tail `setBV` first only | 1,180 | 402 | 69.8 | 71,497 | 577 | 3 |
| `0a12` (b0, ASL, b1, b2) | 1,176 | 404 | — | — | — | — |

The faithful ASL order is the best net section result: `.text` +8 but
`.ARM.extab` -1 and `.data` -1 (total +6), and the verify pct rises
0.6.  The tail call order (`setBV` before `setShipGroup`, matching the
original decompile `PlayerFixedObject::setBV(...); getShipGroup(...);
setShipGroup(...); addObject(...); field_0x40=1`) is also faithful but
costs +6 raw for only +0.1 pct, so it was not landed.

## 3. Rejected cells (this session)

* `EmpGuard` around `getCurrentCampaignMission` removed: raw **1,138**
  (-36), aligned 379, pct 70.6, but `.ARM.extab` size 156,308 (4 B
  smaller) shifts every later LSDA and `.ARM.exidx` diff explodes to
  1,307; `.text` 71,455.  Net section loss (extab content diff 122,287).
* All 24 tail statement orders: base (`setShipGroup, addObject,
  field_0x40, setBV`) is the unique 1,174 optimum.
* All 24 `{ASL, box0+store0, box1+store1, box2+store2}` orders:
  faithful `a012` = 1,182; base split `01a2` = 1,174.
* `int y`/all-int x/y/z before `setEmpData`: size 2,752/2,768, not
  size-exact.
* `float fx`/`fy`/`fz`, `Level *self`, `PlayerFixedObject *ship`,
  `const int t/r`, `bool isWreck`, `AEGeometry *g` placement,
  `ship = fixed` placement, `volumes` aliases: neutral or worse.

## 4. Gate

`cmake --build cmake-build-match --target check -j2` -> **RATCHET PASS**:
byte_exact **4507/4524**, linked_exact **4507/4524**, size-exact
4524/4524, data_exact 843/843, imports 207, lint CLEAN, drift 0,
parity 5370/5370.  `tools/verify_baseline.json` untouched.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13508
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN5Level10createShipEiiiP8Waypointbb
# -> raw=1182 aligned=397 size=2764/2764 insn=1117/1117
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

## 6. Files changed

* `src/game/world/Level.cpp` — race-0 non-14 `ArraySetLength`/box order.
* `docs/IDENTICALITY.md` — wave-191 numbers.
* this note.
