# Level::createShip — pass 7 (wave 95, w4507)

**Status:** analysis only, no source change landed (2026-11-03). Worktree
`wt/w4507`, branch `wave95-w4507`, base `4a88bd6e`. Target:
`_ZN5Level10createShipEiiiP8Waypointbb` (original 2764 B @ `0xc0214`,
Thumb entry `0xc0215`). Current verifier score **74.1439 %**
(orig 1007 insns, ours 1008, osize 2764, usize 2764, `byte False`).

Gate before and after (unchanged, no code change):

```text
byte_exact 4500/4524  linked_exact 4500/4524  size-exact 4524/4524
stub_zero_size 0  missing 0  wrong_type 0  extra 0  data_exact 843/843
imports 207  .text 0x1337c8  .rodata 0x41b67  lint CLEAN  drift 0
RATCHET PASS
```

This pass continues `level-createship-pass6-w4264.md`. It (a) decodes the
original's `.ARM.exidx`/`.ARM.extab` LSDA call-site table exactly and
proves `setEmpData` is a **plain call, not an invoke**; (b) re-derives the
local declaration / assignment / jitter order and the FI/colour order
from Ghidra + a fresh `-print-after-all` GRA dump of the committed build;
and (c) measures every faithful source degree of freedom around the
committed shape (declaration order, assignment order, zero order, jitter
order, spawn order, spawn types, try placement/body, placement-vs-real
`new`, ctor argument order, statement reorder). No faithful shape beats
**74.1439 %**. The only strict improvement found is the known semantic
`PlayerFixedObject(race, type)` swap at **74.2432 %**, which contradicts
the binary (the original passes `(type, race)`) and was already reverted
by `94e8bc54`; it is therefore not re-landed here.

---

## 1. The original has no `try` at `setEmpData` (LSDA proof)

The original `createShip` exidx entry is at `.ARM.exidx` `0x1ab09c`
(generic model, personality `0x5eb60`). Its LSDA starts at `0x1ab0ad`
(`LPStart=0xff` omit, `TType=0x01` uleb128, call-site table length
`0xbb 0x02` = 315 bytes, **57 call-site records**). Decoding the
`(start, length, landing-pad, action)` ulebs gives:

```text
 #  start   len    lp      range (absolute)      meaning
 0  0x000  0x196  0x000   0xc0214..0xc03aa      prologue..hardCoreMode
 1  0x196  0x01e  0x9b8   0xc03aa..0xc03c8      new Player            (delete r5)
 2  0x1b4  0x05e  0x000   0xc03c8..0xc0426      spawn + setEmpData    <-- lp 0
 3  0x212  0x020  0x9b2   0xc0426..0xc0446      PlayerFixedObject ctor(delete r9)
 4  0x232  0x008  0x000   0xc0446..0xc044e      Array operator new
 5  0x23a  0x004  0x9ac   0xc044e..0xc0452      Array ctor            (delete fp)
 6  0x23e  0x04e  0x000   0xc0452..0xc04a0      switch + first alloc
 7  0x28c  0x01e  0x990   0xc04a0..0xc04be      first BoundingAAB ctor(delete r5)
 ... every BoundingAAB ctor has its own lp (0x97a..0x990) ...
56  0x90c  0x1c0  0x000   0xc0b20..0xc0ce0      tail
```

Every non-zero landing pad is a **cleanup-only** pad
(`delete <reg>; _Unwind_Resume`), i.e. the implicit cleanup of a `new`
expression. There is no `__cxa_begin_catch` anywhere in the function.
Record 2 (`0xc03c8..0xc0426`, which contains the whole spawn computation
and the `blx setEmpData` at `0xc03ee`) has **landing pad 0**: it is not in
any EH region. Therefore the faithful source has a plain
`player->setEmpData(empPoints, empTime);` and **no `try`/`catch`**.

The committed source's

```cpp
try {
    player->setEmpData(empPoints, empTime);
} catch (...) {
    __builtin_trap(); // lint: arm_builtin anchor: Level::createShip; ...
}
```

is therefore an anchor: it creates an `invoke` at `setEmpData` that the
original does not have. Removing it alone drops the score to 70.04 %
(2776 B), because the DAG scheduler then hoists the plain call above the
spawn adds; the anchor is kept for the `.text` size (2764 B).

## 2. Exact local order and FI/colour order

Ghidra (`createShip @ 0xd0214`) decompiles the waypoint/jitter/spawn
locals as:

```c
local_54 = waypoint->x;   iVar12 = waypoint->y;   iVar2 = waypoint->z;
iVar3 = rnd(40000);       iVar4 = rnd(40000);     iVar5 = rnd(40000);
iVar3 = local_54 + iVar3 + -20000;                 /* x */
iVar1 = iVar12   + iVar4 + -20000;                 /* y */
fVar20 = (float)(iVar2 + iVar5 + -20000);          /* z */
Player::setEmpData(this_00, iVar7, iVar13);
```

i.e. faithful names `wx=x, wy=y, wz=z`, jitter calls in **X, Y, Z** order,
spawn order **x, y, z**, and all three coordinates live as integers across
the call (the `vcvt.f32.s32` are emitted after it, duplicated in both
`shipClass` branches at `0xc0412..0xc0422` and `0xc06d0..0xc06d8`).

The committed source instead uses the name swap `wz=waypoint->x,
wy=waypoint->y, wx=waypoint->z`, jitter call order **Y, Z, X**, and the
mixed type pattern `int x; float z; float y;`. This is semantically
different (x uses `waypoint->z` and the 3rd rnd) but scores 1.5 points
higher because of the stack-slot colouring.

Authoritative slot maps (`[sp,60-4k]`, k = colour index):

```text
original : wy@48  wz@44  wx@40  jX@36  jY@32  jZ@28   (colour order)
committed: wy@48  wz@44  jY@40  jZ@36  wx@32  jX@28
```

The committed pre-colouring FI creation order (fresh
`-mllvm -print-after-all -mllvm -filter-print-funcs=...` GRA dump,
`DBG_VALUE %stack.N` names) is

```text
stack.3 = wx   stack.4 = wy   stack.5 = wz
stack.6 = jX   stack.7 = jY   stack.8 = jZ
```

i.e. `wx, wy, wz, jX, jY, jZ`. The original's colour order (all weights
tie, no invoke) is its FI order `wy, wz, wx, jX, jY, jZ`. The 4-cycle
`(wx, wy, wz, jX) -> (wy, wz, wx, jX)` is the residual difference.

`StackSlotColoring` sorts by descending `LiveIntervals::getSpillWeight`
and stable-sorts ties. In the committed build the `invoke` splits the
spawn: `wy, wz, jY, jZ` are referenced in the pre-invoke block
(`bb.23`), `wx, jX` only in the post-invoke block (`bb.26`). The former
four therefore colour first (48,44,40,36) and the latter two last
(32,28). Reordering the source declarations does not change this: the
weight grouping is fixed by the invoke, and the target interleaving
(`wx, jX` between `wz` and `jY`) needs all six weights to tie.

## 3. Faithful sweeps (full build, official verifier)

Every variant below is a full `Level.cpp` rewrite compiled with the exact
`compile_commands.json` command and scored with
`tools/verify/verify.py` (`autojunk=False`).

### 3.1 Declaration / assignment / zero / jitter / spawn order

Committed field mapping and types, one dimension at a time:

| dimension | best | score |
|---|---|---|
| waypoint declaration order (6) | `int wx, wz, wy;` (committed) | **74.1439** |
| waypoint assignment order (6) | `wz=x; wy=y; wx=z;` (committed) | **74.1439** |
| else-zero order (6) | any | 74.1439 |
| jitter call order (6) | Y,Z,X (committed) | **74.1439** |
| spawn declaration order (6) | x,z,y / z,x,y / z,y,x | **74.1439** |
| `if (waypoint)` vs `!= nullptr` vs `!= 0` | any | 74.1439 |

The declaration order is load-bearing even when the parsed slot map is
identical: `int wy; int wz; int wx;` keeps the same prologue but drops to
72.59 %, `int wx; int wy; int wz;` gives 73.88 %.

### 3.2 Spawn types

| types (x,z,y) | score |
|---|---|
| `int x, float z, float y` (committed) | **74.1439** |
| `float x, float z, float y` | 64.36 |
| `int x, int z, int y` | 60.64 |
| `int x, int y, float z` (Ghidra's `(float)` cast on z) | 68.19 |
| `float x, int y, int z` | 61.60 |

### 3.3 `try` placement / body

| variant | score | size |
|---|---|---|
| `try { setEmpData } catch (...) { trap }` (committed) | **74.1439** | 2764 |
| no try | 70.0397 | 2776 |
| `try` around `new Player` | 61.1276 | 2792 |
| `try` around `PlayerFixedObject` ctor | 65.9395 | 2780 |
| `try` around `PlayerFighter` ctor | 65.7086 | 2780 |
| empty catch | 65.0722 | 2772 |
| `catch (...) { throw; }` | 49.3988 | 2740 |
| `catch (...) { delete player; }` | 64.9079 | 2780 |
| `catch (...) { __builtin_abort(); }` | 71.5988 | 2764 |
| `catch (...) { __builtin_unreachable(); }` | 66.1699 | 2760 |

### 3.4 `new` form and ctor argument order

| variant | score |
|---|---|
| placement `new` for fighter/fixed (committed) | **74.1439** |
| real `new` for fighter/fixed | 63.4049 |
| real `new` + no try | 64.7029 |
| `PlayerFixedObject(race, type, ...)` | **74.2432** |
| `PlayerFighter(race, type, ...)` | 73.9454 |
| both swapped | 74.0447 |
| `getShipGroup(race, type, ...)` | 74.0447 |
| `setShipGroup(race, type, ...)` | 74.1439 |

### 3.5 Statement reorder / local aliases

| variant | score |
|---|---|
| `new Array` before `new PlayerFixedObject` | 59.2040 |
| `setShipGroup` before the race switch | 46.2537 |
| `setBV` before the race switch | 67.8908 |
| `empPoints` before `hitpoints` | 74.1439 |
| `level` after `hitpoints` | 74.1439 |
| cache `level` for `empPoints` | 65.0722 |
| `int kind = type; int faction = race;` locals | 74.1439 |
| `(void) x;` / `if (x == ...) {}` before/in the try | 74.1439 |
| inline spawn expressions into the ctors | 74.1439 |
| inline spawn expressions into the ctors + volumes | 63.2673 |
| `KIPlayer *ship = 0;` | 74.1439 |
| `PlayerFixedObject *ship` | 34.9825 |

## 4. Why the target slot map and the prologue are exclusive

The target colour order `wy, wz, wx, jX, jY, jZ` is the FI order of a
build with **no invoke** at `setEmpData` (all six weights tie). The
prologue `str r1,[sp,#60]; mov r9,r0; mov fp,r3; mov r5,r2` (race spilled,
this→r9, type→fp, shipClass→r5) is produced by the committed mixed-type
build, which has the invoke. Every no-try shape (mixed, all-int, real or
placement `new`, all six declaration orders) loses at least one of
`race60`/`type_fp`; the all-int no-try prologue is
`str r2,[sp,#60]; mov r9,r3; str r0,[sp,#56]; mov sl,r1` (shipClass
spilled, this spilled, type→r9, race→sl). The all-float shape reaches the
target slot map but its prologue is `race→[sp,#56], type→sl,
shipClass→r6` (64.36 %).

The residual `.text` diff is therefore not a single local fix: the spawn
region (13 instructions) and the downstream capital-ship volume section
(`volumes` in `fp` vs `r9`, `x` in `sl` vs `r6`, `z` in `r4` vs `sl`,
`race` in `r6` vs `r4`) are one coupled allocation. The current 74.1439 %
is the faithful local optimum.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w4507
cmake --preset default
cmake -S . -B cmake-build-match \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8
python3 tools/verify/verify.py \
  --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN5Level10createShipEiiiP8Waypointbb
```

Scratch helpers under `/dev/shm/w4507`: `variants.py` (full-build variant
generator), `score.py` (official-ratio scorer), `diffmap.py` (difflib
opcode map), `batch*.py` (the sweeps above), `pa_base.txt` (fresh
`-print-after-all` GRA/SSC dump of the committed build), `minilink.sh` +
`harness.py`/`fast.py`/`swscore.py` (single-function mini-link proxy,
baseline 66.9975 mini vs 74.1439 full).
