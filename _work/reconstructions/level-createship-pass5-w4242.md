# Level::createShip — pass 5 (wave 92, w4242)

**Status:** analysis only, no source change landed (2026-11-03). Worktree
`wt/w4242`, branch `wave92-w4242`, base `b379d00d`. Target:
`_ZN5Level10createShipEiiiP8Waypointbb` (original 2764 B @ `0xc0214`,
Thumb entry `0xc0215`). Current verifier score **74.1439 %**
(osize 2764, usize 2764, `byte False`).

Gate before and after (unchanged, no code change):

```text
byte_exact 4498/4524  linked_exact 4498/4524  size-exact 4524/4524
stub_zero_size 0  missing 0  wrong_type 0  extra 0  data_exact 843/843
imports 207  .text 0x1337c8  .rodata 0x41b67  lint CLEAN ✓  drift 0
RATCHET PASS
```

This pass continues `level-createship-pass4-w4226.md`. It reconstructs the
**exact local declaration/assignment order from Ghidra** (§1), then sweeps
that order and every nearby slot-affecting degree of freedom with the
official verifier (§2–§5). The target slot map is reachable, but only in
source shapes that lose the prologue or the instruction count; no shape
beats 74.1439 % while staying faithful. The only score-positive variant is
the semantic `PlayerFixedObject(race, type)` swap already rejected by
`94e8bc54` (§6).

---

## 1. Exact local declaration/assignment order (Ghidra + disasm)

Ghidra (project `GalaxyOnFire2`, program
`android_2.0.16_libgof2hdaa.so`, `createShip @ 0xd0214`) decompiles the
waypoint and jitter locals as:

```c
local_54 = *(int *)(waypoint + 0x124);   /* waypoint->x  = A */
iVar12   = *(int *)(waypoint + 0x128);   /* waypoint->y  = B */
iVar2    = *(int *)(waypoint + 300);     /* waypoint->z  = C */

iVar3 = rnd(40000);                      /* jitterX = j1 */
iVar4 = rnd(40000);                      /* jitterY = j2 */
iVar5 = rnd(40000);                      /* jitterZ = j3 */

iVar3 = local_54 + iVar3 + -20000;       /* x = A + j1 */
iVar1 = iVar12   + iVar4 + -20000;       /* y = B + j2 */
fVar20 = (float)(iVar2 + iVar5 + -20000);/* z = C + j3 */
```

So the **faithful source order** is:

```cpp
int wx = waypoint->x;  int wy = waypoint->y;  int wz = waypoint->z;
int jitterX = rnd; int jitterY = rnd; int jitterZ = rnd;
int x = wx + jitterX - 20000;
int y = wy + jitterY - 20000;
int z = wz + jitterZ - 20000;
```

with all three ints live across `setEmpData` and converted at the
constructor call in each `shipClass` branch. The committed source instead
uses a rotated jitter order (`jitterY, jitterZ, jitterX`) and swapped
waypoint names (`wz = waypoint->x`, `wy = waypoint->y`,
`wx = waypoint->z`), i.e. it computes `x = waypoint->z + jitterX` and
`z = waypoint->x + jitterZ` — a semantic x/z swap that is load-bearing for
the current 74.1439 %.

Authoritative original slot map (ELF `objdump`, `fi#k` at `[sp,60-4k]`):

```text
A = waypoint->x -> [sp,#44]   j1 = jitterX -> [sp,#36]
B = waypoint->y -> [sp,#48]   j2 = jitterY -> [sp,#32]
C = waypoint->z -> [sp,#40]   j3 = jitterZ -> [sp,#28]
```

Committed slot map:

```text
A -> [sp,#44]   j1 -> [sp,#28]
B -> [sp,#48]   j2 -> [sp,#40]
C -> [sp,#32]   j3 -> [sp,#36]
```

The first five colours (`race,this,type,wy,wx`) are identical; the last
four are the 4-cycle `(C,j1,j2,j3) -> (j2,j3,C,j1)`.

The committed object's spawn region (verified on the current build, not the
stale pass-4 text) computes `y` and `z` before the `setEmpData` invoke and
`x` after it:

```text
1b4 ldr r0,[sp,#44]   ; A (wz)
1ba ldr r1,[sp,#36]   ; j3
1bc ldr r2,[sp,#40]   ; j2
1be add r0,r1         ; z = A + j3
1c0 ldr r1,[sp,#48]   ; B (wy)
1c2 subs r0,r0,r4     ; z - 20000
1c4 add r1,r2         ; y = B + j2
1c6 subs r1,r1,r4     ; y - 20000
1c8 vmov s0,r1 ; vcvt s18   ; y converted BEFORE the call
1d0 vmov s0,r0 ; vcvt s16   ; z converted BEFORE the call
1de blx setEmpData
1e2 ldrd r1,r0,[sp,#28]     ; j1, C
1ec add r0,r1               ; x = C + j1
1ee sub.w r4,r0,r4          ; x - 20000
208 vmov s0,r4 ; vcvt s20   ; x converted AFTER the call
```

The original computes all three ints before the call and converts all
three after it (duplicated in both branches). That difference is the
remaining structural mismatch in the spawn region.

## 2. Why the target slot map needs a weight change

`StackSlotColoring::ScanForSpillSlotRefs` adds
`1 * block_freq(MI.parent)/entry_freq` for every non-debug FI reference
(`llvm/lib/CodeGen/StackSlotColoring.cpp`, LLVM 7.0.2), then
`InitializeSlots` does a `std::stable_sort` by descending weight and
`ColorSlot` assigns the lowest free colour.

The committed pre-colouring FI creation order (Greedy RA dump) is

```text
stack.3=C  stack.4=B  stack.5=A  stack.6=j1  stack.7=j2  stack.8=j3
```

so among the last four the creation order is already `C,j1,j2,j3` — the
target. The observed colour order `j2,j3,C,j1` therefore comes from
weights: `j2,j3` are loaded in the pre-invoke block `bb.23`, while `C,j1`
are loaded in the post-invoke block `bb.26`. If `C,j1` were referenced in
the same block as `j2,j3`, all four weights would tie and the stable sort
would preserve `C,j1,j2,j3`.

This is confirmed by the `(void)(x+y+z);` control: it forces the `x`
computation into `bb.23`, and the resulting slot map is exactly the target
`{A:44,B:48,C:40}` / `[36,32,28]` — but the extra live ranges change the
entry allocation and the prologue becomes
`race->[sp,#56], type->sl, shipClass->r6` (score 64.8649, 2748 B).

## 3. Faithful-order sweep (1296 variants)

Scratch harness `/dev/shm/w4242/sweep.py`: full `Level.cpp` variants
compiled with the exact `compile_commands.json` command, slots parsed from
the object. Dimensions: faithful waypoint assignment (`wx=x,wy=y,wz=z`),
6 waypoint declaration orders, 6 waypoint assignment orders, 6 jitter
declaration/assignment orders, 6 spawn declaration orders; committed
mixed types (`int x`, `float z`, `float y`), `try` + placement `new`.

```text
variants 1296   prologue_ok 1296   target hits 0
waypoint->x slot: 32 in every variant
distinct maps:
  252x {A:32,B:48,C:44} jit (28,40,36)
  252x {A:32,B:48,C:44} jit (40,28,36)
  252x {A:32,B:48,C:44} jit (40,36,28)
  180x {A:32,B:44,C:48} jit (28,40,36)
  180x {A:32,B:44,C:48} jit (40,28,36)
  180x {A:32,B:44,C:48} jit (40,36,28)
```

So the faithful mapping pins `waypoint->x` to slot 32 and can never reach
`{44,48,40}`. The faithful order also scores worse with the official
verifier: **72.5919 %** (`jitterX,Y,Z`) and **72.6912 %** (`Y,Z,X`) vs the
committed **74.1439 %**.

## 4. Type-pattern sweep (288 variants)

Committed waypoint mapping, `try` + placement `new`, all 8 `int/float`
patterns for `(x,z,y)`, 6 jitter orders, 6 spawn orders.

```text
variants 288   prologue_ok 108   target hits 0
```

Prologue-preserving patterns are only:

```text
(int x, float z, float y)   -> {A:44,B:48,C:32}  jit permutation of {28,36,40}
(float x, float z, int y)   -> {A:48,B:32,C:44}  jit (28,40,36)
(float x, int z, float y)   -> {A:32,B:48,C:44}  jit (40,28,36)
```

None has `C=40` together with `j1=36,j2=32,j3=28`.

## 5. Prologue-preserving pattern sweep (2592 variants)

The two non-committed prologue-preserving patterns above, all 6 waypoint
declaration orders, 6 assignment orders, 6 jitter orders and 6 spawn
orders:

```text
variants 2592   prologue_ok 2592   target hits 0
best maps: {A:48,B:44,C:32} or {A:48,B:32,C:44} or {A:44,B:48,C:32},
           jit a permutation of {28,36,40}
```

The target map is unreachable in the `try` family with a matching prologue.

## 6. Shapes that do reach the target slot map

Two families reach `{A:44,B:48,C:40}` / `[36,32,28]`, both by moving all
three coordinate computations before the invoke:

1. Dead use before the try, e.g. `(void)(x + y + z);` (or
   `if (x + y + z == 0x7fffffff) {}`). Target slots, but the prologue
   becomes `race->[sp,#56], type->sl, shipClass->r6`; official score
   **64.8649 %**, 2748 B.

2. Int compute + float conversion after the call:

   ```cpp
   int x = wx + jitterX - 20000;
   int z = wz + jitterZ - 20000;
   int y = wy + jitterY - 20000;
   try { player->setEmpData(empPoints, empTime); } catch (...) { ... }
   float zf = (float) z;
   float yf = (float) y;
   /* ctor uses x, yf, zf */
   ```

   Target slots `{A:44,B:48,C:40}` / `[36,32,28]`, prologue keeps
   `race->[sp,#60]` and `shipClass->r5` but loses `type->fp`
   (`type->sl`); official score **70.4885 %**, 2772 B (999 insns vs the
   original 1007 — the conversions are hoisted out of the two branches).

The faithful all-int no-`try` shape scores 61.83 %; the
committed-mapping no-`try` int-compute shape has `race->[sp,#56]`,
`type->sl` and slots `{A:40,B:44,C:48}` / `[36,28,32]`.

## 7. The only score-positive variant is the rejected semantic swap

Official verifier (`autojunk=False`), exact ratios:

| variant | score | size |
|---|---|---|
| committed baseline | **74.1439** | 2764 |
| `PlayerFixedObject(race, type, ...)` (semantic x/z of kind/param2) | **74.2432** | 2764 |
| `PlayerFighter(race, type, ...)` | 73.9454 | 2764 |
| both ctor orders swapped | 74.0447 | 2764 |
| `PlayerFixedObject(type, race, ..., z, y, x)` | 74.0447 | 2764 |
| `PlayerFighter(type, race, ..., z, y, x, hostile)` | 74.1439 | 2764 |

The 74.2432 gain is exactly one instruction: the swapped call makes the
compiler emit `mov r1,r4` where the original has `mov r1,r4` (type->r1),
while the committed call emits `ldr r1,[sp,#52]`. The instruction bytes
match, the values do not: the original ctor call is
`(this, type, race, player, nullptr, x, y, z)` (disasm
`c0426 ldr r4,[sp,#52]` = type, `c042a ldr r6,[sp,#60]` = race,
`c0432 mov r1,r4`, `c0438 mov r2,r6`). This is the same variant reverted
by `94e8bc54`; it is a register-allocation coincidence, not a source
shape to land.

Other controls measured this pass (official verifier):

| variant | score |
|---|---|
| `int ctorRace = race; int ctorType = type; PlayerFixedObject(ctorType, ctorRace, ...)` | 74.1439 |
| `int ctorType = type; int ctorRace = race; ...` | 74.1439 |
| `Player *player; player = new Player(...)` | 74.1439 |
| `KIPlayer *ship; ship = nullptr;` | 74.1439 |
| `fixed->setShipGroup(...)` before `fixed->setBV(...)` | 74.0447 |
| `catch (...) { throw; }` | 49.3988 |
| `catch (...) { }` | 65.0722 |
| `Player *player = new Player(hardCoreMode() ? 650 : 1000, ...)` | 70.8395 |

## 8. What remains

1. A `try` shape whose entry allocation is still
   `race->[sp,#60], type->fp, shipClass->r5` but whose coordinate
   computations all live in the pre-invoke block (so the stack-slot
   weights tie and the stable sort keeps `C,j1,j2,j3`). Every attempt to
   force the `x` computation before the invoke either changes the entry
   allocation (`voidsum`) or hoists the `vcvt`s out of the branches
   (int-compute family), costing more than the slot swap gains.
2. The spawn conversion placement: the original keeps three ints live
   across the call and duplicates `vcvt.f32.s32` in both `shipClass`
   branches. The committed mixed `int x` / `float z,y` shape is the only
   one that keeps the prologue, and it converts `y,z` before the call.
3. The `BoundingAAB` construction region is the largest remaining diff
   block (11 vs 13 instructions for the first box); it is dominated by
   whole-function register allocation (`z` in `r4` vs `sl`, `y` in `sl`
   vs `r6`, `nullptr` in `r6` vs `r0`), not by a source-order choice.

No source change is justified: no faithful shape beats 74.1439 %, and the
only score-positive shape (74.2432) is the semantic ctor swap that
`94e8bc54` deliberately reverted.

## 9. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w4242
cmake --preset default \
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

Scratch helpers (all under `/dev/shm/w4242`): `harness.py` (compile one
variant and parse slots), `exact_score.py` (unrounded official ratio),
`score_variant.py` (build + score), `sweep.py` (1296 faithful-order sweep),
`sweep_types.py` (288 type-pattern sweep), `sweep_pro.py` (2592
prologue-pattern sweep), `sweep_conv.py` (int-compute/float-after-call
sweep), `diff_blocks.py` (largest alignment mismatches), `pa2.txt`
(`-print-after-all` of the committed build).
