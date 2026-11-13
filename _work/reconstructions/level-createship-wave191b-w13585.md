# `Level::createShip` — wave 191b (w13585): spawn/hull sweeps, no landable raw win

Worktree `wt/w13585`, branch `wave191b-w13585`, base master `2a2fcd43e`.
Target `_ZN5Level10createShipEiiiP8Waypointbb` (`src/game/world/Level.cpp`,
2,764 B, VMA `0xc0214`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`).  Scratch `scratch/w13585`.

**Status: no landable win.  Tree left at the pristine baseline.**
Raw **1,164**, aligned **400**, size **2,764/2,764** exact, insns
1,117/1,117, `.text` contentdiff **71,219**, `.ARM.extab` **156,312**
(contentdiff **577**), `.ARM.exidx` diff **0**, `.rel.plt` identical.

## 0. The real residual is a global callee-saved rotation

Reading the original disassembly (not just Ghidra) fixes the exact shape:

* **Spawn block** (original `0xc03c8..0xc03e4`): three *int* coordinates are
  computed in one block **before** `setEmpData`, from interleaved loads:
  `fp = wpx + jitter1 - 20000`, `r4 = wpy + jitter2 - 20000`,
  `r8 = wpz + jitter3 - 20000` (jitter1/2/3 = first/second/third `nextInt`,
  stored at `[sp,#36]/[sp,#32]/[sp,#28]`).  The three `vcvt.f32.s32`
  conversions happen **after** `new PlayerFixedObject`, immediately before
  the ctor (`s20=x, s18=y, s16=z`).  Both the `PlayerFighter` and
  `PlayerFixedObject` ctors receive `(x,y,z) = (s20,s18,s16)`.
* **Prologue** (original): `this=r9`, `type=fp`, `shipClass=r5->sl`,
  `race=[sp,#60]`, `mission=r8`.  Ours: `type=r9`, `this=[sp,#56]`,
  `race=sl`, `shipClass=[sp,#60]`.
* **Hull block** (original): `fixed=r9`, `volumes=fp`, coordinate regs
  `x=sl, y=r8, z=r4`, `0=r6`.  Ours: `fixed=fp`, `volumes=r9`,
  `x=r6, y=r8, z=sl`, `0` rematerialised in `r0`.
* **Landing pads**: original has four delete variants (`sl`,`fp`,`r9`,`r5`)
  and saves the exception in `r8`; ours has two (`r9`,`r5`) and uses `r4`
  (placement `new` for `PlayerFighter`/`PlayerFixedObject`).

The hull block's *instruction sequence* is otherwise identical to the
original — the 869-byte hull residual is almost entirely this register
rotation (`[fp,#4]` vs `[r9,#4]`, `mov r3,r4` vs `mov r3,sl`, …).

## 1. The faithful spawn form is size-wrong

Ghidra's shape (`int x,y; float z` declared before `setEmpData`) is
faithful but clang sinks the ints past the call:

| variant | raw | aligned | size |
|---|---:|---:|---:|
| baseline (w13501 hack: `z=wpx+wpy`, `x=wpz+j3`, `y=j1+j2+j3`) | **1,164** | 400 | 2,764 |
| faithful `int x=wpx+j1; int y=wpy+j2; float z=(float)(wpz+j3);` before guard | 2,251 | 515 | **2,768** |
| same, no `setEmpData` guard | 1,487 | 517 | 2,764 (extab 156,308) |
| all-int (no float) | 2,297 | 489 | **2,752** |
| all-float before guard | 2,468 | 995 | **2,732** |
| `int zi` before, `float z=(float)zi` after guard | 1,346 | 435 | 2,764 |

The faithful form is 4 B too big because the whole register allocation
changes (clang keeps `jitter3` in `fp` from the first `nextInt` instead of
spilling it).  Dropping the `setEmpData` guard restores the size but shifts
the LSDA (156,308) and is still +323 raw.

## 2. Spawn expression sweeps (all size-exact 2,764, extab 577)

Holding the guard structure, every assignment of `{(wx,j1),(wy,j2),(wz,j3)}`
to `(x,y,z)` with `z` float-before and `x`/`y` after:

| z | x | y | raw |
|---|---|---|---:|
| wx+j3 | wy+j1 | wz+j2 | 1,171 |
| wx+j3 | wz+j2 | wy+j1 | 1,171 |
| wy+j1 | wx+j3 | wz+j2 | 1,170 |
| wy+j1 | wz+j2 | wx+j3 | 1,169 |
| wz+j2 | wx+j3 | wy+j1 | 1,173 |
| wz+j2 | wy+j1 | wx+j3 | 1,173 |

`z = two waypoints` is better.  Full 18-way sweep of
`z ∈ {wx+wy, wx+wz, wy+wz}` × `x`,`y` pairs from the remaining four values:
best **1,165** (`z=wx+wy`, `x=wz+j1`, `y=j2+j3`; or `x=j2+j3`, `y=wz+j1`
with aligned 400).  The landed hack (three-jitter `y`, `j3` used twice) is
still best at 1,164.  Commuting the operands (`wpy+wpx`, `jitter3+wpz`,
`jitter3+jitter2+jitter1`, `jitter2+jitter3+jitter1`), swapping `x`/`y`
declaration order, and `x=wz+j1` / `x=wz+j2` are all neutral (1,164).

## 3. Hull / volume setup

* **AAB literal values**: the original's `.text` literal pool really does
  carry `(ey,ez)` swapped relative to our source for volumes 1,3,5,7,9
  (0-based), and volume 9 additionally has `w`/`d` swapped
  (`w=15860 h=2510 d=10910`).  But the pool *slot* order is
  `h,w,d,ez,ey` in both builds: the original loads `ey` before `ez`, ours
  loads `ez` before `ey`, so swapping the values puts them in the wrong
  slots.  Measured: swap vol 1 = 1,170; swap vol 9 = 1,170; swap all five
  = **1,194**; using local float temps for `ey`/`ez` = 1,170 (pool order
  unchanged).  Not a win.
* **`ArraySetLength`/`setWreckedMeshId`/`setBV`/`field_0x40` reorders**:
  `setWreckedMeshId(0x4780)` before the 11 AABs = 2,048;
  `field_0x40=1` before `addObject` = 1,170;
  `setBV` before `field_0x40` = 1,167; `setBV` first = 1,170.
* **Declaration/alias churn**: `Array` no-paren / separate assign /
  `volumes` declared first / both declared first = all neutral (1,164).
  `KIPlayer *ship` → `PlayerFixedObject *ship` or `void *ship` = 2,337 /
  2,788.  `Level *self = this` alias = neutral.  `int rc = race` used in
  the ctors/switch = neutral.  `const int K = 20000` = neutral.
  `int sc = shipClass` = neutral.  `switch (shipClass)` = 2,278.
  `float zero = 0.0f` for the AAB `ex` arg = neutral (clang rematerialises
  `movs r0,#0`).
* **Clamp**: `(level<=20?level:20)*5+40` = 1,167; `(level>20?20:level)*5+40`
  = 1,174; `40+(...)`, `level<21`, `20>=level`, `5*level`, `5*20` all
  1,164 (neutral).  The landed folded form stays best.
* **`EmpGuard` shape**: `~EmpGuard() noexcept` = neutral;
  `~EmpGuard() = default` = 2,182/2,776; explicit `EmpGuard(){}` = neutral;
  `int mission = 0` = neutral.

## 4. The `notop` win cannot be landed with extab 577

Dropping the top `EmpGuard` is a real -36 raw (1,128) and size-exact, but
the LSDA record drops 336 → 332 (tlen 315 → 310) and every later record
shifts (`.ARM.extab` 156,308, `.ARM.exidx` diff 1,307).  To restore the
336 B record the call-site table must reach tlen 312..315, i.e. **+2..+5
bytes**.  Exhaustive guard sweep on `notop` (each guard measured with the
`recsize` parser):

* `+0` (no row, no code): guard around `delete fighter->cargo`,
  `fighter->setShipGroup` (virtual call).
* `+1` (row only, but +16 B code → size 2,780): guard around an
  individual `(*volumes)[i] = new BoundingAAB(...)` statement.  Two such
  guards give **record 336 / tlen 312** but size 2,780 (not exact).
* `+6` (tlen 316, record 340): guard at jitter1/2/3, `getLevel`, `gameWon`,
  `new Player`, `ArraySetLength`, `setWreckedMeshId`, `getShipGroup`
  (fighter), `addObject` (fighter), `setLevel`.
* `+10`: guard at the first `getLevel`; `+11` at `setShipGroup`.

The only `+5` row is the top guard itself (start=28 < 128 with an empty
preceding region), so `notop` + top guard = the baseline exactly.  No
combination of {top, setEmpData, setShipGroup, getShipGroup(fighter),
new Player, setLevel} toggles reaches tlen 315 with raw < 1,164.

## 5. Other measured dead ends

`return` directly from each branch = 2,464/2,748; `missionPtr` without
`__builtin_expect` = 1,171 (and 1,172 with the `getShipGroup` guard);
cargo order `2c/33/31` = neutral; `fx` cast / ctor arg casts = neutral;
`z = (float)(wpx+wpy) - 20000.0f` = 2,129/2,776.

## 6. Admissible-but-not-a-win

`drop setShipGroup guard + guard getShipGroup(fighter)`:
raw **1,165** (+1), aligned 400, size 2,764, **record 336**,
`.ARM.extab` contentdiff **576** (one byte *better* than baseline).
This is the only record-336 variant found with a lower extab diff, but it
is +1 raw, so it is not landed.

## 7. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13585
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN5Level10createShipEiiiP8Waypointbb      # raw=1164 aligned=400 size=2764/2764
python3 scratch/w13585/secdiff.py            # .text 71219, extab 577, exidx 0
```

Scratch: `measure.sh`, `secdiff.py`, `adiff.py`, `lsda.py`, `vtry.py`,
`fh.py`, `b13..b30.py`, `notop3..notop6.py`, `combsearch.py`,
`bigsearch.py`, `poolcmp.py`.

## 8. Files changed

* this note only; `src/game/world/Level.cpp` is unchanged from `HEAD`.
