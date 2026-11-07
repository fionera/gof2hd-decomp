# `Level::createShip` — wave 132 (w7693): coordinate declaration order, `z y x`

Target `_ZN5Level10createShipEiiiP8Waypointbb` (`Level::createShip`, original
2764 B @ `0xc0214`, Thumb entry `0xc0215`, 1007 insns).
Base `4afd0643a` (wave-131 final). Worktree `/home/debian/GalaxyOnFire2/wt/w7693`.
Owned TU: `src/game/world/Level.cpp` (createShip only).

**Status: source change landed.**  `createShip` **73.2 -> 73.4** raw (73.4,
still 2764 B, 1007/1007 insns).  The only source change is the declaration
order of the three spawn-coordinate locals: `z`, `y`, `x` instead of `x`, `y`,
`z`.  The three expressions and their types are unchanged (`int x`,
`float y`, `float z`).  `.text` differing bytes, `.ARM.extab` size/content and
every gate count are unchanged from wave 131.

## Result

```
                                   original        HEAD (4afd0643a)    landed
createShip raw fuzzy               —               73.187686           73.4
createShip size                    2764 B          2764 B              2764 B
createShip insns (orig/ours)       1007            1007 / 1007         1007 / 1007
createShip LSDA record             336 B           328 B               328 B
createShip LSDA rows               57              54                  54
createShip LSDA ttype              0xff            0x00                0x00
.text differing bytes              —               85,167              85,167
.ARM.extab size                    156,312         156,304             156,304
.ARM.extab differing bytes         —               108,023             108,023
byte_exact / linked_exact          —               4506 / 4506         4506 / 4506
```

`check` on the landed tree: **RATCHET PASS** — `verify: byte 4506 linked
4506 stubs 0 missing 0 extra 0 avg 99.94`, `data: exact 843 of 843`,
`sodiff: imports 207`, lint `CLEAN`, drift 0.

## 1. Why the order matters

The three spawn locals are spilled to fixed frame slots, and the slot numbers
are assigned by the register allocator's stack-slot colouring.  The colouring
order follows the order in which the values are first defined / spilled, so
the *declaration order* of the three independent expressions selects which
frame slot each value lands in.  The original's slots are

```
jitter1 = [sp,#36]   jitter2 = [sp,#32]   jitter3 = [sp,#28]
wpx     = [sp,#44]   wpy     = [sp,#48]   wpz     = [sp,#40]
```

With the wave-131 `x, y, z` declaration the compiler assigned `wpx=[sp,#44]`,
`jitter1=[sp,#36]`, `wpy=[sp,#48]` correctly but put `jitter2=[sp,#40]` and
`wpz=[sp,#28]` (swapped relative to the original).  Declaring `z, y, x`
changes the colouring order and makes the coordinate block's load/store
sequence line up with the original for a further ~0.2 % of instructions.

`z, y, x` is semantically identical to `x, y, z`: the three expressions are
pure, independent, and only their values are used later.  The `nextInt(40000)`
draw order is untouched (the jitters are still declared `jitter1, jitter2,
jitter3` in that order, so the RNG sequence is unchanged).

## 2. Sweep evidence

All rows are full-link, one-function official scorer, with the wave-131 body
(`try`/`catch` barrier, placement `new`, two-call `getLevel` clamp).

### 2a. Coordinate type × declaration order (48 variants)

Types are `i` = `int expr`, `f` = `float (float)(expr)`; the three letters are
`x y z`.  Order is the declaration order.

| types | best order(s) | raw % | size |
|---|---|---:|---:|
| `iff` | `zyx`, `zxy`, `xzy` | **73.4** | 2764 |
| `iff` | `yzx`, `yxz`, `xyz` | 73.2 | 2764 |
| `fif` | `zyx`, `zxy`, `yzx` | 72.5 | 2764 |
| `ffi` | `zyx`, `yzx`, `yxz` | 72.3 | 2764 |
| `iif` | all | 68.3 | 2780 |
| `ifi` | all | 67.2-67.3 | 2780-2784 |
| `iii` | all | 61.5 | 2752 |
| `fff` | all | 57.2-58 | — |

The wave-131 `iff` type is confirmed best; `iif`/`iii` (the faithful all-int
spawn) are much worse because they let the whole coordinate block sink below
`setEmpData`.

### 2b. Jitter declaration order × coordinate order (36 variants, `iff`)

`j231` + `czyx`/`czxy`/`cxzy` reaches **73.5**, but `j231` changes the RNG
draw order (`jitter2` gets the first `nextInt`), so it is not faithful.  With
the faithful `j123` jitter order the best is **73.4** (`czyx`/`czxy`/`cxzy`).
The landed change keeps `j123`.

### 2c. Waypoint read order (6 variants, `iff`, `czyx`)

All six orders give 73.4 — the waypoint read order does not affect the
colouring.

## 3. Other new hypotheses tried this wave (no win)

* **`std::atomic_signal_fence` / `__c11_atomic_signal_fence`** between the
  coordinates and `setEmpData`, no `try`: 69.7 % (identical to the plain
  no-`try` body).  A signal fence only orders memory operations; the
  coordinate values are pure SSA, so the machine scheduler still sinks them.
* **`__sync_synchronize()`** (full `dmb` barrier): 69.7 %, +4 B (the `dmb`),
  still sinks.
* **Local RAII guard** (`struct EmpGuard { ~EmpGuard() {} }; { EmpGuard g;
  player->setEmpData(...); }`): 65.2 %, 2760 B.  It *does* produce a
  cleanup-only LSDA (`ttype 0xff`) but with only 54 rows and a 320 B record,
  and the code shrinks to 2760 B.
* **Coordinates inside the `try`** (assign to `int x, y, z` declared outside):
  61.5 %, 2752 B (same as the all-int `try` body).
* **Coordinates before `new Player`** with `iff` types + `try`: 62.5 %,
  2776 B; without `try`: 66.0 %, 2796 B.
* **`-mllvm -pre-RA-sched=source`** (diagnostic only): the coordinate block
  is *still* emitted after `setEmpData`, so the reordering is not the
  pre-RA machine scheduler.

## 4. Open item (unchanged from wave 131)

The 336 B cleanup-only LSDA (`ttype 0xff`, 57 rows) still requires a
no-`try` shape that keeps the coordinate block above `setEmpData`.  Every
barrier tried so far either changes the EH shape (catch type, row count) or
lets the block sink.  The row count itself tracks the code layout (the
no-`try` body has 53 rows here, not 57), so recovering the exact record
probably requires matching the whole function first.

## 5. Reproduction

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
python3 tools/verify/verify.py \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN5Level10createShipEiiiP8Waypointbb
```

Scratch helpers (not committed): `/dev/shm/w7693/{score.py,apply.py,
build_score.sh,try.sh,insdiff.py,lsda.py,lsdacount.py,mir.sh,ir.sh,gen_*.cpp,
g2_*.cpp,g3_*.cpp}`.
