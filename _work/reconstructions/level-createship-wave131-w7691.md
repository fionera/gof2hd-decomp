# `Level::createShip` — wave 131 (w7691): guard/cleanup + zero-register hypothesis (no source win)

Target `_ZN5Level10createShipEiiiP8Waypointbb` (`Level::createShip`, original
2764 B @ `0xc0214`, Thumb entry `0xc0215`, 1007 insns).
Base `7c6933c2d` (wave-130 final). Worktree `/home/debian/GalaxyOnFire2/wt/w7691`.
Owned TU: `src/game/world/Level.cpp` (createShip only).

**Status: analysis only — no `src/` change landed.** The checked-in body stays the
best at **70.541480** (official 70.5), 2764 B, 1007/1006 insns, exact 336 B /
57-row / `ttype 0xff` cleanup-only LSDA. This pass attacked the wave-130
"guard/cleanup + zero-register 12 B" hypothesis with new experiments and found
that the coordinate *placement* is decided by the declaration position relative
to `new Player`, not by the guard, and that the exception register can be forced
to `r8` by changing a coordinate's type — but only at a large score cost.

## Result

```
                                   original        HEAD (7c6933c2d)
createShip exact ratio              —               70.541480
createShip size                    2764 B          2764 B
createShip insns (orig/ours)       1007            1007 / 1006
createShip LSDA record             336 B           336 B
createShip LSDA rows               57              57
createShip LSDA ttype              0xff            0xff
byte_exact / linked_exact          —               4506 / 4506
data / imports / lint / drift      —               843 / 207 / CLEAN / 0
```

`check` on the unchanged tree: **RATCHET PASS** — `verify: byte 4506 linked
4506 stubs 0 missing 0 extra 0 avg 99.94`, `data: exact 843 of 843`,
`sodiff: imports 207`, lint `CLEAN`, drift 0.

## 1. New finding: declaration position, not the guard, decides the coordinate sink

Wave-129-w7648 concluded that the `Globals::status == nullptr` guard is what
sinks the coordinate arithmetic below `setEmpData`. That is only true for the
checked-in *mixed-float* body. Measured with the verifier's own scorer
(`/dev/shm/w7691/exact.py`):

| body (coords declared …) | guard | types | coords before `setEmpData`? | exact % | size |
|---|---|---|---:|---:|---:|
| after `new Player` | yes | int/float/float (HEAD) | no | **70.541480** | 2764 |
| after `new Player` | no | int/float/float | no | 63.558897 | 2748 |
| after `new Player` | yes | all int | no | 61.546135 | 2772 |
| after `new Player` | no | all int | no | 60.000000 | 2776 |
| **before `new Player`** | yes | int/float/float | **yes** | 62.842893 | 2768 |
| **before `new Player`** | no | all int | **yes** | 61.722250 | 2776 |

So: if `x/y/z` are declared after `new Player`, the scheduler sinks them below
`setEmpData` regardless of the guard and regardless of int/float type. If they
are declared before `new Player`, they stay between the `Player` ctor and
`setEmpData` (the original's position) regardless of the guard. The guard is a
*compensating hack* that keeps the instruction count aligned, not the sink
trigger.

## 2. New finding: the exception pointer CAN be forced to `r8` by a coordinate type

The cleanup pads use `mov r6,r0` in the checked-in body and `mov r8,r0` in the
original. The r6/r8 choice is coupled to which values occupy r6/r8: in the base
`shipClass` is spilled to `[sp,#60]`, so r6 is free and the exception PHI takes
it. Changing a coordinate's type changes the pressure and flips the pad:

| coord types (order x,y,z) | exact % | size | pad register |
|---|---:|---:|---|
| `int,float,float` (HEAD z,y,x) | **70.541480** | 2764 | r6 |
| `int,int,int` | 61.546135 | 2772 | r6 |
| `float,float,float` | 57.512438 | 2776 | r6 |
| `float,int,int` | 58.077304 | 2788 | **r8** |
| `float,float,int` | 60.873883 | 2768 | **r8** |
| `float,int,float` | 68.951813 | 2764 | r6 |
| `int,float,int` | 60.119048 | 2784 | r6 |

`{x:float,y:int,z:int}` and `{x:float,y:float,z:int}` do move the exception PHI
to r8, confirming the mechanism, but the extra `vcvt`/register moves cost more
than the 12-byte `strd` fold saves. The original is all-int, so this is not the
original spelling.

## 3. The faithful body is correct but allocates registers differently

The faithful source — ternary (double-call) `MIN` clamps, no guard, all-int
`x/y/z` declared before `new Player` — reaches the original's coordinate
placement and 1007/1002 insns, but scores only **61.722250** (2776 B). The
prologue diverges in a register rotation:

```
original                          faithful
str r1,[sp,#60]   ; race          strd r1,r2,[sp,#56]  ; race,shipClass
mov r9,r0                         mov r9,r0
mov fp,r3         ; type          mov fp,r3            ; type (matches)
mov r5,r2         ; shipClass     ldr r6 = Globals
ldr r4 = Globals                  ...
r5 = rnd / r6 = nextInt           r4 = rnd / r5 = nextInt
```

i.e. Globals/rnd/nextInt are r6/r4/r5 instead of r4/r5/r6, `shipClass` is
spilled instead of living in r5/sl, and `jitterX` lives in sl instead of
`[sp,#36]`. The ternary clamps themselves add blocks that re-roll the whole
allocation (the base's single-call `if` is a local optimum against the current
neighbouring code).

Converting the `shipClass` if/else chain to a `switch (shipClass)` improves the
faithful body **62.332506 -> 64.416873** (2796 B) — the only shape this pass
found that moves the faithful body materially — but it is still far below the
base, and applying the same switch to the base collapses it to 41.18 %.

## 4. Zero-register initialization

Named zeros all fold back to the rematerialised `movs r0,#0; str r0,[sp]`:

* `float zero = 0.0f;` (function/branch scope, `static const`)
* `float zero = (float)(x - x);`
* `float zero = pos.x * 0.0f;`
* `0.f` / `0` (int) instead of `0.0f`

The original folds the zero into `strd r6,r0,[sp]` only because r6 is free
(shipClass lives in r6/sl there). When r6 is freed in our body by making `x`
float, the zero does move to r6, but the coordinate code grows by more than the
12 B saved. The zero is a consequence of the r6/r8 split, not a lever.

## 5. Guard/cleanup shapes tried (all <= 70.541480)

| guard / shape | exact % | size |
|---|---:|---:|
| `Globals::status == nullptr` (HEAD) | **70.541480** | 2764 |
| `!Globals::status` | 70.541480 | 2764 |
| `Globals::status != nullptr` inverted | 70.541480 | 2764 |
| guard after `ship = nullptr` | 70.541480 | 2764 |
| `ship` declared before `setEmpData` / before `new Player` | 70.541480 | 2764 |
| `if (shipClass == 2) return nullptr;` | 65.573770 | 2764 |
| `if (Globals::rnd == nullptr) return nullptr;` | 65.607940 | 2768 |
| `if (player == nullptr) return nullptr;` | 63.558897 | 2748 |
| `if (waypoint == nullptr) return nullptr;` | 59.215102 | 2784 |
| `if (shipClass > 1) return nullptr;` | 57.724789 | 2784 |
| guard before `setEmpData` | 51.297405 | 2780 |
| guard before the coords | 57.933762 | 2800 |

Micro-variants that leave the code identical: `empTime = 15000` initialiser,
`armor` if/else, `switch (mission)` for the 270 hp set, `field_0x25 = false`,
`field_0x40 = true`, `missionPtr` comparison order — all 70.541480. An explicit
`if (fighter->cargo != nullptr)` around the delete is worse (66.236346, 2768 B);
plain `delete` already emits the `cbz`.

## 6. Remaining lever

The base is a local maximum. The original's register map
(`Globals=r4, rnd=r5, nextInt=r6, type=fp, shipClass=sl->r6, empPoints=sl,
race=[sp,#60]`) is only reachable if `shipClass` stays in a callee-saved
register instead of being spilled. In every faithful spelling the ternary
clamps or the switch re-roll that allocation. A shape that keeps the base's
single-call clamp and guard while keeping `shipClass` in r6 (and thus the
exception PHI in r8 and the zero in r6) is still open.

## 7. Reproduction

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

Scratch helpers (not committed): `/dev/shm/w7691/{exact.py,try*.py,Level.cpp.base,
diff_base.txt,mir_base.txt,Level_base.ll}`.
