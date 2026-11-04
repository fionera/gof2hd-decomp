# Level::createShip — pass 10 (wave 100, w5006)

**Status:** analysis only, no source change landed. Target
`_ZN5Level10createShipEiiiP8Waypointbb` (original 2764 B @ `0xc0214`,
Thumb entry `0xc0215`, 1007 insns). Verifier score stays **74.379345 %**
(report rounds 74.4), `usize == osize == 2764`, 1007 insns on both sides,
757 differing instructions. This pass continues
`level-createship-pass9-w4807.md` and `level-createship-pass8-w4705.md`.

Gate before and after (unchanged, no code change):

```text
byte_exact 4500/4524  linked_exact 4500/4524  size-exact 4524/4524
stub_zero_size 0  missing 0  wrong_type 0  extra 0  data_exact 843/843
imports 207  .text 0x1337c8  .rodata 0x41b67  .ARM.exidx 0x6f70
.ARM.extab 0x26294 (-4)  lint CLEAN  drift 0  RATCHET PASS
```

## 0. What this pass establishes

The committed source (below) is a **strict local optimum of the measured
space**. 60+ new source variants were built and scored with the official
verifier in this pass; none beats 74.379345 %. The two residuals are
unchanged from pass 9:

1. the `this` spill `str.w r9,[sp,#56]` is one basic block too early
   (`0xc031c`, end of `bb.14`) instead of the original `0xc0332`
   (end of `bb.17`);
2. the downstream `shipClass` register colouring
   (`volumes=fp, z=r4, x=sl, y=r8` in the original vs
   `volumes=r9, z=sl, x=r6, y=r8` in ours).

Both are Greedy-RA decisions, and this pass shows *why* they cannot be
reached from source without changing the spawn's `int`/`float` mix, which
is exactly what buys the byte-exact prologue.

## 1. The committed shape (the 74.4 % local optimum)

`src/game/world/Level.cpp:5166`:

```cpp
PlayerFixedObject *Level::createShip(int race, int shipClass, int type, Waypoint *waypoint, bool hostile, bool group) {
    int mission = Globals::status->getCurrentCampaignMission();
    int wx, wz, wy;
    if (waypoint != nullptr) {
        wz = waypoint->x;       // wz = wp->x  -> [sp,#44]
        wy = waypoint->y;       // wy = wp->y  -> [sp,#48]
        wx = waypoint->z;       // wx = wp->z  -> [sp,#40]
    } else {
        wz = 0; wy = 0; wx = 0;
    }
    int jitterY = Globals::rnd->nextInt(40000);   // 1st -> [sp,#36]
    int jitterZ = Globals::rnd->nextInt(40000);   // 2nd -> [sp,#32]
    int jitterX = Globals::rnd->nextInt(40000);   // 3rd -> [sp,#28]
    int level = Globals::status->getLevel() > 20 ? 20 : Globals::status->getLevel();
    int hitpoints = (Globals::status->gameWon() ? 180 : mission * 4) + level * 14 + 20;
    switch (type) {
        case 0x2c: hitpoints = (int) ((float) hitpoints * 2.25f); break;
        case 0x33: hitpoints = (int) ((float) hitpoints * 1.7f);  break;
        case 0x31: hitpoints = (int) ((float) hitpoints * 17.0f); break;
    }
    if (mission == 49 || mission == 50 || mission == 51 || mission == 52 || mission == 56) {
        hitpoints = 270;
    }
    int empPoints = (Globals::status->getLevel() > 20 ? 20 : Globals::status->getLevel()) * 5 + 40;
    int empTime;
    if (shipClass == 1) {
        hitpoints *= type == 14 ? 25 : 5;
        empPoints *= 3;
        empTime = 45000;
    } else {
        empTime = 15000;
    }
    hitpoints = (int) ((float) hitpoints + (*reinterpret_cast<float *>(&Globals::options[0x2c]) - 0.5f) * (float) hitpoints);
    if (mission == 0x9a) {
        hitpoints <<= Globals::status->inAlienOrbit() & (race == 9);
    }
    int armor = Globals::status->hardCoreMode() ? 650 : 1000;
    Player *player = new Player(armor, hitpoints, 1, 1, 0);
    int x = jitterZ + jitterX - 20000;              // int   : 2nd + 3rd
    float y = (float) (wy + jitterY - 20000);       // float : wp->y + 1st
    float z = (float) (wz + wx - 20000);            // float : wp->x + wp->z
    try {
        player->setEmpData(empPoints, empTime);
    } catch (...) {
        __builtin_trap(); // lint: arm_builtin anchor: Level::createShip; removal grows body 2764 -> 2772
    }
    KIPlayer *ship = nullptr;
    /* ... shipClass==0 / shipClass==1 branches, all BoundingAAB literals ... */
    ship->setLevel(this);
    return (PlayerFixedObject *) ship;
}
```

The spawn is a **semantic hack** (pass 8): it is not the original's
all-`int` spawn, but it reproduces the original's
`StackSlotColoring` hot/cold split (`{wp->y, wp->x, wp->z, 1st}` hot,
`{2nd, 3rd}` cold) and therefore the exact stack-slot map
`wy@48 wz@44 wx@40 j1@36 j2@32 j3@28`. It also keeps the exact prologue
(`r9=this, fp=type, r5=shipClass, sl=shipClass, r8=mission, r4=status,
race->[sp,#60]`) and the exact `Player` ctor region.

## 2. Byte-identical regions (unchanged)

```text
orig c0214..c031c  ==  ours c0214..c031c   (100 insns: prologue, waypoint
                                            loads, 3x rnd, level, hitpoints,
                                            type switch, mission 49..56)
orig c0336..c03c8  ==  ours c0336..c03c8   (empPoints tail, shipClass
                                            branch, options adjust,
                                            mission==0x9a, hardCoreMode,
                                            operator new, Player ctor)
```

The first difference is the `this` spill at `0xc031c`; both forms are
4 bytes, so the block shift cancels and the code re-syncs at `0xc0336`.

## 3. The `this` spill is a Greedy-RA *compact-region* choice

Filtered `-print-after-all` (see §8) on the committed source:

```text
Greedy RA / VReg Rewriter, bb.14 (%ir-block.65), the empPoints getLevel block:
  2640B  t2CMPri killed renamable $r0, 20
  2648B  t2STRi12 killed renamable $r9, %stack.0     ; this spill
  2656B  t2Bcc %bb.16, 13
...
bb.17 (%ir-block.73):
  2960B  t2CMPri renamable $r6, 1                     ; shipClass == 1
  2968B  t2STRi12 renamable $r11, %stack.1            ; type spill
  2976B  t2Bcc %bb.19, 0
```

`%stack.0` is the `this` value (`%124`, `%681` after RA): defined at 64B
(`%124 = COPY $r0`), stored once at 2648B, reloaded at 5096B (bb.26,
`shipClass==0`) and at 13640B/17400B/19192B/21336B/24696B (the other
`missionPtr`/`lodManager` uses). The original instead stores at `0xc0332`
(end of `bb.17`, after `cmp r6,#1` / `add sl,r0,#40`) and reloads at
`0xc03fe` (the `bb.17` continuation), i.e. the spill region is only the
two `empTime` arms `{bb.18, bb.19}`, not `{bb.15..bb.26}`.

`this` has **no uses at all** before `0xc0332` in the original (only
`mov r9,r0` at `0xc0222` and the store at `0xc0332`); it is a pure
live-through value. The split point is therefore chosen by
`RAGreedy::calcCompactRegion` / `SpillPlacer` from block frequencies and
interference, not by a source-visible use.

`-mllvm -split-spill-mode=size` (analysis only, not a build change) moves
the store 4 bytes later in the object stream (`0x106` vs `0x102`) but
moves it to `[sp,#52]` (the `type` slot), i.e. it changes the slot map,
so it is not the answer either.

## 4. Why the faithful all-`int` spawn cannot be used

Pass 9 decoded the original spawn exactly:

```text
c03c8  ldr r2,[sp,#48]   ; wp->y      c03ca ldr r3,[sp,#32]  ; 2nd
c03cc  ldr r0,[sp,#44]   ; wp->x      c03ce ldr r1,[sp,#36]  ; 1st
c03d0  add r2,r3         ; wp->y+2nd  c03d2 ldr r3,[sp,#40]  ; wp->z
c03d4  ldr r4,[sp,#28]   ; 3rd        c03d6 add r0,r1        ; wp->x+1st
c03d8  movw r1,#20000
c03dc  sub.w fp,r0,r1    ; x = wp->x + 1st - 20000
c03e0  add r3,r4         ; wp->z+3rd
c03e2  subs r4,r2,r1     ; y = wp->y + 2nd - 20000
c03e4  sub.w r8,r3,r1    ; z = wp->z + 3rd - 20000
c03e8  mov r0,r5 / c03ea mov r1,sl / c03ec mov r2,r9
c03ee  blx Player::setEmpData
```

i.e. all three coordinates are `int` and the three `vcvt.f32.s32` are
emitted *after* the call, duplicated in both `shipClass` branches
(`0xc040c..0xc0422` and `0xc06c6..0xc06d8`). In committed-source naming
the faithful spawn is

```cpp
int x = wz + jitterY - 20000;   // wp->x + 1st
int y = wy + jitterZ - 20000;   // wp->y + 2nd
int z = wx + jitterX - 20000;   // wp->z + 3rd
```

Measured this pass: **61.6 %**, `usize 2752` (12 B short). The reason is
not the spawn block itself but a **global colouring change**: with the
all-`int` spawn the pre-RA scheduler sinks the three spawn adds *below*
`setEmpData` (`0xc03d2..0xc03ea` instead of `0xc03c8..0xc03e4`) and
Greedy RA then picks a different prologue:

```text
original  : str r1,[sp,#60] (race)  mov r9,r0 (this)  mov fp,r3 (type)  mov r5,r2 (shipClass)
all-int   : str r2,[sp,#60] (shipClass)  mov r9,r3 (type)  str r0,[sp,#56] (this)  mov sl,r1 (race)
```

i.e. `this` is spilled at entry, `type` goes to `r9`, `race` to `sl`.
That single global change accounts for most of the 61.6 %.

## 5. Sweeps measured this pass (all with the official verifier)

Every row is a full build + `tools/verify/verify.py` score of
`_ZN5Level10createShipEiiiP8Waypointbb`. `usize` is the body size.

### 5.1 Faithful all-`int` spawn, 6 statement permutations × int/float

| variant | score | usize |
|---|---|---|
| `faith_xyz_int`  (x,y,z order, committed naming) | 61.6 | 2752 |
| `faith_xzy_int`  | 61.6 | 2756 |
| `faith_yxz_int`  | 60.9 | 2752 |
| `faith_yzx_int`  | 61.0 | 2752 |
| `faith_zxy_int`  | 61.6 | 2756 |
| `faith_zyx_int`  | 60.7 | 2752 |
| `faith_xyz_float` | 65.0 | 2748 |
| `faith_xzy_float` | 64.5 | 2748 |
| `faith_yxz_float` | 64.5 | 2748 |
| `faith_yzx_float` | 64.9 | 2748 |
| `faith_zxy_float` | 64.4 | 2748 |
| `faith_zyx_float` | 64.8 | 2748 |
| `floatvars_int_expr` (`float x,y,z; x = int-expr;`) | 65.0 | 2748 |
| `intvars_floatcast` (`int ix..; float x=(float)ix;`) | 65.0 | 2748 |
| `committed_allfloat` (committed pairing, all float) | 64.8 | 2748 |
| `compound_committed` (`x += ...`) | 61.0 | 2752 |
| `deepopen_compound` (DeepOpen `spawnX += jitterX-20000`) | 61.6 | 2752 |
| `deepopen_inline` (rnd inline in the compound assign) | 49.3 | 2740 |

### 5.2 Branch hints (`__builtin_expect`)

| variant | score | usize |
|---|---|---|
| `sc_expect1` (`expect(shipClass==1,1)`) | 65.7 | 2768 |
| `sc_expect0` | 64.8 | 2768 |
| `wp_expect1` (`expect(waypoint!=nullptr,1)`) | 64.3 | 2768 |
| `wp_expect0` | 64.7 | 2768 |
| `emp_expect1` (`expect(mission==0x9a,1)`) | 69.0 | 2760 |
| `emp_expect0` | 60.6 | 2772 |
| `miss49_expect0` | 74.4 | 2764 |

### 5.3 `int armor` placement (very sensitive)

| variant | score | usize |
|---|---|---|
| before `empPoints` | 68.0 | 2764 |
| before `empTime` | 57.0 | 2768 |
| before the options `hitpoints` adjust | 68.3 | 2764 |
| before `mission==0x9a` | 68.5 | 2764 |
| before `new Player` (== committed) | 74.4 | 2764 |
| at the top of the function | 57.7 | 2776 |
| inline in `new Player(...)` | 71.1 | 2760 |

In the `before empPoints` variant the `this` spill jumps to `0xc023a`
(prologue) — proof that the split point is a global RA decision.

### 5.4 `try` extent / catch body

| variant | score | usize |
|---|---|---|
| committed (`try { setEmpData } catch { trap }`) | 74.4 | 2764 |
| no `try` at all | 70.5 | 2776 |
| `catch (...) { }` (empty) | 65.5 | 2772 |
| `catch (...) { throw; }` | 49.8 | 2740 |
| `try` around spawn + `setEmpData` (decls outside) | 70.9 | 2792 |
| `try` around `new Player` only | 62.0 | 2780 |
| `try` around `new Player` + spawn + `setEmpData` | 60.7 | 2828 |

### 5.5 `empPoints` / `empTime` / declaration-order shapes

| variant | score | usize |
|---|---|---|
| `empP_tmp` (`int empLevel = ...; int empPoints = empLevel*5+40;`) | 74.4 | 2764 |
| `empP_if` (`int empPoints = getLevel(); if (empPoints>20) empPoints=20; ...`) | 70.7 | 2756 |
| `empP_noclamp` | 71.2 | 2748 |
| `empP_min` | 70.7 | 2756 |
| `empP_from_level` (`(level>20?20:level)*5+40`) | 64.8 | 2752 |
| `empP_le` (`getLevel() <= 20 ? getLevel() : 20`) | 74.3 | 2764 |
| `level_le` | 74.1 | 2764 |
| `both_le` | 74.0 | 2764 |
| `empP_before_hp` (empPoints before hitpoints) | 56.9 | 2768 |
| `switch_after_empP` | 72.3 | 2764 |
| `m49_before_switch` | 72.8 | 2764 |
| `empTime_default` (`int empTime = 15000; if (shipClass==1) empTime=45000;`) | 74.4 | 2764 |
| `empTime_tern` | 72.3 | 2764 |
| `sc_inverted` (`if (shipClass != 1)`) | 39.3 | 2760 |
| `empTime_first` / `empP_split_decl` / `empTime_init0` / `empTime_init15000` | 74.4 | 2764 |

### 5.6 Neutral 74.4 variants (byte-identical instruction stream)

`(void) this;` before `empPoints`, before `armor`, before the spawn;
`int x,y,z;` declared at the top; `Player *player` declared at the top;
waypoint block written as three ternaries; `hitpoints` declared before
`level`; `Array<BoundingVolume*> *volumes = new Array<BoundingVolume*>;`
(no parens); `KIPlayer *ship = nullptr;` moved before the `try`.

### 5.7 Downstream allocation forms

| variant | score | usize |
|---|---|---|
| committed (`::operator new` + placement `new`) | 74.4 | 2764 |
| `PlayerFixedObject *fixed = new PlayerFixedObject(...)` | 63.6 | 2768 |
| `PlayerFighter *fighter = new PlayerFighter(...)` | 65.4 | 2784 |

So the explicit `::operator new(sizeof(T))` + placement new is required
for the exact size; the plain `new` expression is 4 B longer.

## 6. Residual #2 — downstream colouring

Everything from `0xc03f6` to the epilogue is the same instruction
*sequence* with a different register colouring:

```text
original : volumes = fp   z = r4   x = sl   y = r8   type = r4   race = r6
committed: volumes = r9   z = sl   x = r6   y = r8   type = r6   race = r4
```

`y = r8` matches in both. The 11 `BoundingAAB` constructions alone account
for ~90 of the 757 replaced instructions (each `ldr.w r0,[fp,#4]` vs
`ldr.w r0,[r9,#4]`). This is a direct consequence of the spawn's
`int`/`float` mix: the committed `float y, float z` keep two coordinates
in `s18/s16` and free GP registers, so Greedy RA picks a different
colouring than the original's all-`int` spawn. It cannot be fixed
independently of the spawn.

## 7. Conclusion / next lever

The committed source is a strict local optimum. The prologue, slot map,
FI order, hitpoints/empPoints/empTime arithmetic and `Player` ctor are
already byte-exact. The only remaining lever is the **Greedy-RA
compact-region split of `this`** (residual #1). It is driven by the
spawn's `int`/`float` mix, which is itself what buys the exact prologue.
A shape that keeps the mixed-type prologue *and* the all-`int` spawn
scheduling was not found in pass 9 or this pass.

The known semantic `PlayerFixedObject(race, type)` swap (74.5 %) is still
rejected: the binary passes `(type, race)` (`0xc0426 ldr r4,[sp,#52]` =
type, `0xc042a ldr r6,[sp,#60]` = race, `mov r1,r4`, `mov r2,r6`).

## 8. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w5006
cmake --preset default
cmake -S . -B cmake-build-match \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
python3 tools/verify/verify.py \
  --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN5Level10createShipEiiiP8Waypointbb
```

Incremental single-function iteration (`~3 s` per variant):

```sh
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w5006/score.py      # official ratio for the one symbol
```

Filtered RA dump (`~7 s`, ~15 MB):

```sh
CMD=$(python3 - <<'PY'
import json
cc=json.load(open('cmake-build-match/compile_commands.json'))
for e in cc:
    if e['file'].endswith('/src/game/world/Level.cpp'):
        print(e['command']); break
PY
)
CMD=${CMD/-o CMakeFiles\/gof2.dir\/src\/game\/world\/Level.cpp.obj/-o /dev/shm/w5006/pa.o}
eval "$CMD -mllvm -print-after-all -mllvm -filter-print-funcs=_ZN5Level10createShipEiiiP8Waypointbb" \
  > /dev/shm/w5006/pa_all.txt 2>&1
# then: grep -n '^# \*\*\* IR Dump' pa_all.txt | grep -i 'greedy\|virtual'
```

`-mllvm -split-spill-mode={default,size,speed}` is a useful diagnostic
but is **not** a build change: `size` moves the `this` store to
`[sp,#52]` (wrong slot).

Scratch helpers (this pass, under `/dev/shm/w5006`): `score.py`
(official ratio), `aligndiff.py` (opcode-aligned difflib dump), `gen.py`
/ `region.py` (region text splice + build + score), `batch2.py` ..
`batch16.py` (the sweep harnesses above), `pacmd.sh` (filtered
`-print-after-all`), `lsda.py` (`.ARM.exidx`/LSDA call-site decoder).
