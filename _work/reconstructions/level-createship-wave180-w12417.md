# `Level::createShip` — wave 180 (w12417): `empPoints` ternary polarity, raw 1,221 -> 1,214

Worktree `wt/w12417`, branch `wave180-w12417`, base `master 99e341e9b`
(wave 179 harvest).  Target `_ZN5Level10createShipEiiiP8Waypointbb`
(`src/game/world/Level.cpp`, 2,764 B, VMA `0xc0214`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2 `-Oz`).

**Status: source change landed.**  The `empPoints` clamp ternary is written in
the opposite polarity:

```cpp
// before (wave 179 base)
int empPoints = (Globals::status->getLevel() > 20 ? 20 : Globals::status->getLevel()) * 5 + 40;
// after
int empPoints = (Globals::status->getLevel() <= 20 ? Globals::status->getLevel() : 20) * 5 + 40;
```

`createShip` raw **1,221 -> 1,214** (-7), aligned **394 -> 396** (+2),
size **2,764/2,764** exact, insns **1,117/1,117** (exact).  `.text`
differing bytes **76,870 -> 76,863** (-7).  `.ARM.extab` stays the original
**336 B / 57 rows / `ttype 0xff`** cleanup-only record, size- and base-exact
(`156,312 B` @ `0x1a3b78`), content diff unchanged **709**; `.ARM.exidx`
byte-identical; `.rel.plt` byte-identical (0).  File size 2,187,328 exact,
27/27 section headers exact.

## 1. Result

| metric | before (wave 179) | after |
|---|---:|---:|
| raw same-address diff | **1,221** | **1,214** |
| instruction-index alignment | **394** | 396 |
| size | 2,764 | 2,764 |
| insns (orig/ours) | 1,117 / 1,117 | **1,117 / 1,117** |
| `.text` differing bytes | 76,870 | **76,863** |
| `.ARM.extab` size | 156,312 (exact) | 156,312 (exact) |
| `.ARM.extab` differing bytes | 709 | 709 |
| `.ARM.exidx` differing bytes | 0 | 0 |
| `.rel.plt` | byte-identical | byte-identical |

Full `check`: `byte_exact 4507/4524`, `linked_exact 4507/4524`,
size-exact 4524/4524, stub 0, missing 0, wrong_type 0, extra 0,
data 843/843, imports 207, lint CLEAN, drift 0, symbol parity 5370/5370,
`RATCHET PASS`.

## 2. Why it helps

The two forms are semantically identical (`min(getLevel(), 20) * 5 + 40`) and
call `getLevel()` the same number of times for every value, but the branch
polarity changes which arm is the fall-through.  In the wave-179 body the
`empPoints` block is laid out as

```
cmp  r0,#20
ble.n <else: getLevel>
movs r0,#20
b.n  <merge>
<else: getLevel>
```

i.e. `ble` over the `movs #20` arm.  The reversed form emits

```
cmp  r0,#20
str.w r9,[sp,#56]      ; this-spill
bgt.n <movs #20>
b.n  <else: getLevel>
movs r0,#20
```

which flips the fall-through and re-places the `this` spill relative to the
`ble`/`bgt`.  The 0x100..0x13f raw-diff bucket drops from 26 to 19 bytes; every
other bucket is unchanged.  The `this` spill is still at `0xc031c` (the
original keeps it at `0xc0332`), so this is a partial fix of the same
spill-placement residual, not the full register swap.

Measured equivalent clamp forms (all size 2,764, insn 1,117):

| form | raw |
|---|---:|
| `getLevel() > 20 ? 20 : getLevel()` (wave-179 base) | 1,221 |
| `20 < getLevel() ? 20 : getLevel()` | 1,221 |
| `21 <= getLevel() ? 20 : getLevel()` | 1,221 |
| **`getLevel() <= 20 ? getLevel() : 20` (landed)** | **1,214** |
| `getLevel() < 21 ? getLevel() : 20` | 1,214 |
| `20 >= getLevel() ? getLevel() : 20` | 1,214 |
| `21 > getLevel() ? getLevel() : 20` | 1,214 |

The same reversal on the `level` clamp is *worse* (1,228), so only the
`empPoints` clamp is changed.

## 3. Sweeps that did not beat 1,214 (all size 2,764, extab 156,312)

* **Coordinate statement order** (6 permutations on the landed body): 012 /
  102 / 120 = 1,214; 021 / 201 / 210 = 1,218.
* **Jitter declaration order** (6 permutations): 012 = 1,214; 021 / 201 =
  1,215; 102 / 120 / 210 = 1,216-1,217 (three change insn to 1,118).
  J123 pairing (x=first result) = 1,217.
* **Tail statement order** (all 120 permutations of `setShipGroup, addObject,
  field_0x40=1, setBV, ship=fixed`): base 01234 = 1,214; 01423 / 04123 / 40123
  also 1,214; everything else 1,215-1,220.
* **`missionPtr` LOD guard hint** forms: landed
  `__builtin_expect(missionPtr != 1, 1) && missionPtr != 23` = 1,214; the
  compare-order-preserving `missionPtr != 23 && __builtin_expect(missionPtr != 1, 1)`
  = 1,214; `missionPtr != 1 && __builtin_expect(missionPtr != 23, 1)` = 1,215;
  `__builtin_expect(missionPtr != 1 && missionPtr != 23, 1)` = 1,218; no hint
  = 1,218.
* **Mission-chain order** (all 120 permutations of the `mission == 49/50/51/52/56`
  chain): canonicalises to 1,214.
* **`switch (type)` case order**: 0x2c,0x33,0x31 (landed) and 0x2c,0x31,0x33
  = 1,214; other orders 1,237-1,242.
* **`level` clamp reversal / `empPoints` from `level` / `empPoints` moved
  before `hitpoints`** — all worse (1,228-2,480) or size-inexact.
* **Other ternaries reversed** (`gameWon`, `hardCoreMode`, `type == 14`,
  `mission == 0x9a` operand order): no change (1,214).

## 4. Guard-shape sweep (still no exact-extab win)

Re-confirmed on the wave-180 base: the 2-guard shape (`setEmpData`+`setLevel`,
no `mission` guard) is raw **1,185** but `.ARM.extab` **156,308** (4 B short).
The `mission` guard is the only construct that adds exactly +4 extab bytes
(its 5-byte LSDA row `(28,4,2496,0)` splits the original `(0,406,0,0)` row),
and it costs 36 raw -> 1,221.  Every other extra guard adds +8 or +12 extab
bytes (BoundingAAB x24, `new Player`, `new Array`, `setBV`, `setWreckedMeshId`,
`addObject`, `getShipGroup`, `nextInt` x3, `getLevel` x2, PlayerFighter/PFO
ctors, `ArraySetLength`), which shifts `.rodata` by +16 and pollutes the
`.text` diff.  Guard combinations measured: `mission+emp` 1,216/156,304;
`emp+lvl` 1,185/156,308; `mission+lvl` 2,187/156,308; `mission+emp+lvl`
1,221/156,312.  Adding `setBV`/`inAlienOrbit` to `mission+emp` reaches
156,312 but at 1,227/1,228.  No exact-extab shape beats 1,214.

## 5. Remaining mismatch structure (unchanged)

The residual is still dominated by the fixed (`shipClass == 1`) branch
`0xc03d4..0xc06d4` / `0xc0a54..0xc0b94` register swap (`fixed` r9 vs r6,
`volumes` fp vs r9, float bits `sl/r8/r4` vs `r6/r8/sl`) and the `this` spill
at `0xc031c` vs `0xc0332`.  The trigger is the coordinate block: the original
keeps `int x,y,z` in `fp/r4/r8` across `setEmpData` and converts after, while
our build converts y/z before the call and sinks x below it.  The all-int
body is 2,752 B (12 B short); the size-exact int/float combinations are only
`int x, float y, float z` (1,214), `float x, int y, float z` (1,230) and
`float x, float y, int z` (1,233).

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12417
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN5Level10createShipEiiiP8Waypointbb
python3 _work/reconstructions/extab_lsda_decode.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so
cmake --build cmake-build-match --target check -j2
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w12417/`: `Level_base.cpp`
(wave-179 base), `run.py` (splice + forced recompile + measure harness),
`measure.py`, `idiff2.py`, `gdiff.py`, `buckets.py`, `esize.py`, and the
`v_*`/`s_*`/`g_*`/`b_*`/`c_*`/`d_*`/`e_*`/`f_*`/`h_*`/`i_*`/`j_*`/`k_*`/
`n_*`/`o_*`/`p_*`/`q_*`/`r_*`/`s2_*` variant files (coordinate int/float and
order, guard-shape, spill-order, ternary-polarity sweeps).
