# `Level::createShip` — wave 189 (w13303): order/data-flow cells canonicalise; base 1,202 holds

Worktree `wt/w13303`, branch `wave189-w13303`, base master `247466fde`
(wave 188).  Target `_ZN5Level10createShipEiiiP8Waypointbb`
(`src/game/world/Level.cpp`, 2,764 B, VMA `0xc0214`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13303`.

**Status: no code win landed; note only.**  Base is raw **1,202**,
aligned 484, size 2,764/2,764, insns 1,116/1,117.  Gate green:
byte_exact 4507/4524, linked_exact 4507, size 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0, parity 5370/5370, RATCHET PASS.
Section diffs: `.text` 74,047, `.ARM.exidx` **0**, `.ARM.extab` 639
(size/base exact at `0x1a3b78`, 156,312 B), `.rodata` 0, `.rel.plt` 0,
`.data` 0, file size 2,187,328 exact.

## 0. Bottom line

Every order/data-flow cell measured this wave lands at **>= 1,202**.
The base is a strong local optimum.  The remaining raw bytes are almost
entirely register-allocation choices, not source order:

* the literal pool is **byte-identical** except its last word
  (`0x0014f6d0` orig vs `0x0014f6d4` ours); the code region
  `0xc0214..0xc0bdc` (2,504 B) carries 1,201 of the 1,202 differing
  bytes;
* the dominant diff regions are `0xc0478..0xc0770` (760 B, 701 diff),
  `0xc0a74..0xc0bdc` (360 B, 316 diff), `0xc03c8..0xc045b` (147 B,
  133 diff) and `0xc031c` (26 B, 19 diff);
* the BoundingAAB blocks differ by a systematic register swap:
  orig `x`->`sl`, zero->`r6`, `volumes`->`fp`; ours `x`->`r6`,
  zero->`sl`, `volumes`->`r9`.  The tail differs by `ship`->`fp` (ours)
  vs `ship`/`fixed`->`r9` (orig).  These are the same coalescing choices
  waves 183/186/187/188 documented.

## 1. Prologue and jitter order

The landed base already emits the original prologue
`str r1,[sp,#60]; mov r9,r0; ldr r0,[pc]; mov fp,r3; mov r5,r2`
(`type`->`fp`, `shipClass`->`r5`), common prefix 16 B.  Swapping the
jitter declaration/initialisation order to `jitter3, jitter1, jitter2`
(scratch `Level_jit312.cpp`) keeps raw **1,202** but improves
aligned **397** and instruction count **1,117/1,117** (the original
count).  It is not a raw win, so it was not landed.

The full 6x6 sweep of `(nextInt call order) x (jitter use mapping
z,y,x)` gives raw 1,202 at eight cells and 1,203/1,204 elsewhere.  The
semantically faithful mapping (`x`<-1st, `y`<-2nd, `z`<-3rd) is 1,204;
the base's `z`<-1st mapping is 1,202.

## 2. Cells swept this wave (all size 2,764 unless noted)

| cell | raw |
|---|---:|
| base | **1,202** |
| jitter order `312` (`j3,j1,j2`) | 1,202 (aligned 397, insn 1117/1117) |
| waypoint read/zero/declaration permutations (6x6x6) | 1,202 (one canonical slot map) |
| waypoint declaration `int wpy,wpx,wpz` / separate decls | 1,202 |
| waypoint decl `xzy`/`xyz`/`yzx`/`yxz` | 2,423 (size 2,768) |
| `if (waypoint)` / `local_wp` | 1,202 |
| waypoint ternary | 1,225 |
| all-before coordinates, mixed types | 1,213 |
| all-before coordinates, natural pairing | 1,216 |
| all-before `int x,y,z` (all-int) | 2,238 (size 2,752) |
| split all-int (`int z` before guard, `int x,y` after) | 2,237 (size 2,752) |
| `float x = (float)(...)` | 2,514 (size 2,740) |
| `empPoints` clamp `> 20 ? 20 : getLevel()` | 1,209 |
| `level` single-call / `if` form | 2,430 (size 2,776) |
| `empPoints` single-call / `if` form | 2,157 (size 2,756) |
| `empPoints = level*5+40` | 2,491 (size 2,748) |
| `missionPtr` no-expect / 23-first / eq-or | 1,203..1,206 |
| `inAlienOrbit() && (race==9)` | 1,202 |
| `inAlienOrbit()` operand swap | 1,219 |
| `hitpoints` ternary / `if` / `0xe` | 1,202 |
| `empPoints = empPoints*3` | 1,202 |
| hitpoints `switch` -> `if` chain | 1,202 |
| mission `if` -> `switch` | 1,202 |
| `lodManager` / `missionPtr` hoist | 2,430 / 2,125 |
| `ship` early / `result` var / `two_ships` / `PlayerFixedObject*` ship | 1,202 (or worse) |
| `switch (shipClass)` | 2,283 (size 2,788) |
| `zero` variable / `const float zero` / `0` / `0.f` | 1,202 |
| guard scope extended over the waypoint block | 1,202 |
| guard over `delete fighter->cargo` (mission guard removed) | 1,166, extab 156,308 |

## 3. EmpGuard compensation sweep (mission guard removed)

Removing the mission `EmpGuard` is a 36-byte text win (raw 1,166) but
shrinks `.ARM.extab` to 156,308 (4 B short, `.ARM.exidx` diff 1,307).
A guard around a *call* adds the missing 4 extab bytes; every such
compensation lands back at >= 1,202:

| compensation | raw | extab |
|---|---:|---:|
| `ship->setLevel(this)` (wave 186) | 1,202 | 156,312 |
| `fixed->setBV(volumes)` (wave 186) | 1,202 | 156,312 |
| `Globals::globals->getShipGroup(...)` (fighter) | 1,202 | 156,316 |
| `ArraySetLength(...)` | 1,205 | — |
| `fixed->setWreckedMeshId(...)` | 1,205 | — |
| `lodManager->addObject(...)` (wave 186) | 1,206 | — |
| `PlayerFixedObject` ctor | 1,211 | — |
| `new Array<BoundingVolume*>()` | 1,213 | — |
| `Globals::status->inAlienOrbit()` | 1,203 | — |
| `Globals::status->getLevel()` / `gameWon()` | 1,227 / 1,230 | — |
| `Globals::status->hardCoreMode()` | 2,452 (size 2,768) | 156,320 |
| `new Player(...)` | 2,452 (size 2,768) | — |
| `PlayerFighter` ctor | 1,806 (size 2,768) | — |
| `::operator new` (fighter/fixed) | 2,267 / 2,086 | — |
| `Globals::rnd->nextInt(...)` | 2,446 (size 2,752) | — |
| non-call guards (`hitpoints=270`, `empPoints*=3`, `ship=...`, `field_0x40=1`, `int x=...`) | 1,166 | 156,308 (adds nothing) |

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13303
ulimit -v 12000000
touch src/game/world/Level.cpp
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN5Level10createShipEiiiP8Waypointbb
# -> raw=1202 aligned=484 size=2764/2764 insn=1116/1117
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch helpers: `Level_base.cpp`, `Level_jit312.cpp`, `measure.sh`,
`sweep_jit.py`, `sweep_decl.py`, `sweep_wp312.py`, `try_forms.py`,
`try_312.py`, `try_cells.py`, `try_emp.py`, `try_level.py`,
`try_hoist.py`, `try_bool.py`, `try_order.py`, `try_ship.py`,
`try_coal.py`, `try_coal2.py`, `try_comp*.py`, `try_nocall.py`.

## 5. Files changed

* this note only (no `src/` change; `Level.cpp` left at the wave-188 base).

## Harvest on wave-189 master (`374b49e55`)

Note-only: the branch landed no source change (raw stays 1,202 / 484 / 2,764).
On current master `.text` is 73,999 and total 74,658.  The dominant residual is
register allocation (BoundingAAB `x`/zero/`volumes` allocation), not source
order; the `jitter3,jitter1,jitter2` order keeps raw 1,202 but improves
aligned to 397 and insn to 1,117/1,117, so it is a candidate for a future
aligned/insn pass rather than a raw win.
