# Level::createShip — pass 5 (wave 92, w4258)

**Status:** analysis only, no source change landed (2026-11-03). Worktree
`wt/w4258`, branch `wave92-w4258`, base `b379d00d` (HEAD `e3379c7c`).
Target: `_ZN5Level10createShipEiiiP8Waypointbb` (original 2764 B @
`0xc0214`, Thumb entry `0xc0215`). Current verifier score **74.1 %**
(osize 2764, usize 2764, `byte False`).

Gate before and after (unchanged, no code change):

```text
byte_exact 4499/4524  linked_exact 4499/4524  size-exact 4524/4524
stub_zero_size 0  missing 0  wrong_type 0  extra 0  data_exact 843/843
imports 207  .text 0x1337c8  .rodata 0x41b67  lint CLEAN ✓  drift 0
RATCHET PASS
```

(The wave-92 prompt quotes 4498; the checked-in
`tools/verify_baseline.json` and a fresh `check` run both say **4499**.)

This pass continues `level-createship-pass4-w4226.md`. It re-derives the
target stack-slot permutation from the original disassembly, confirms the
committed FI/colour order with `-print-after-all` `DBG_VALUE` evidence,
shows that the target map is exactly the *no-sink* FI order, and reports a
new 48-way spawn-type × spawn-order sweep plus the all-float / all-int
controls. No source shape beats 74.1 %.

---

## 1. Authoritative slot maps (corrected labels)

Slot offset is `60 - 4k` for FI colour `k` (nine 4-byte spill slots;
frame = `push {r4,r5,r6,r7,lr}` 20 B + `stmdb {r7,r8,r9,sl,fp}` 20 B +
`vpush {d0-d10}` 88 B = 128 B).

Original (`objdump -d`, ELF `0xc0214`):

```text
c023c  ldr.w r1,[r0,#292]   ; waypoint->x
c0240  str   r1,[sp,#44]    ; waypoint->x -> [sp,#44]
c0242  ldr.w r1,[r0,#296]   ; waypoint->y
c0246  ldr.w r0,[r0,#300]   ; waypoint->z
c024a  str   r1,[sp,#48]    ; waypoint->y -> [sp,#48]
c0258  str   r0,[sp,#40]    ; waypoint->z -> [sp,#40]   (merge store)
c026e  str   r0,[sp,#36]    ; jitterX (1st nextInt)
c0278  str   r0,[sp,#32]    ; jitterY (2nd nextInt)
c028c  str   r5,[sp,#28]    ; jitterZ (3rd nextInt)
```

Committed build (`Level.cpp.obj`, same block):

```text
28  ldr.w r1,[r0,#292] ; 2c str r1,[sp,#44]   waypoint->x -> 44
2e  ldr.w r1,[r0,#296] ; 36 str r1,[sp,#48]   waypoint->y -> 48
32  ldr.w r0,[r0,#300] ; 44 str r0,[sp,#32]   waypoint->z -> 32
5a  str r0,[sp,#40]    ; jitterY (1st nextInt)
64  str r0,[sp,#36]    ; jitterZ (2nd nextInt)
78  str r5,[sp,#28]    ; jitterX (3rd nextInt)
```

So the committed source's `wx`/`wz` name swap is *not* the issue: the
first two waypoint slots already match. The whole residual difference is
the last four colours:

| value | original | committed |
|---|---|---|
| `waypoint->y` (`wy`) | `[sp,#48]` k3 | `[sp,#48]` k3 |
| `waypoint->x` (`wz`) | `[sp,#44]` k4 | `[sp,#44]` k4 |
| `waypoint->z` (`wx`) | **`[sp,#40]` k5** | **`[sp,#32]` k7** |
| `jitterX` | **`[sp,#36]` k6** | **`[sp,#28]` k8** |
| `jitterY` | **`[sp,#32]` k7** | **`[sp,#40]` k5** |
| `jitterZ` | **`[sp,#28]` k8** | **`[sp,#36]` k6** |

Colour order (low colour = low offset = first assigned):

```text
original  : race, this, type, wy, wz, wx, jX, jY, jZ
committed : race, this, type, wy, wz, jY, jZ, wx, jX
```

## 2. FI creation order and the colour permutation (committed build)

`-mllvm -print-after-all -mllvm
-filter-print-funcs=_ZN5Level10createShipEiiiP8Waypointbb` on the
committed source; the post-`Greedy Register Allocator` dump names the
frame object for each variable via `DBG_VALUE %stack.N, 0, !"name"`:

```text
stack.0 = this      stack.3 = wx (waypoint->z)   stack.6 = jitterX
stack.1 = type      stack.4 = wy (waypoint->y)   stack.7 = jitterY
stack.2 = race      stack.5 = wz (waypoint->x)   stack.8 = jitterZ
```

`StackSlotColoring::InitializeSlots` builds `SSIntervals` from the
`LiveStacks` map sorted by slot register (i.e. FI index) and then
`std::stable_sort`s by `li.weight` (`ssc.cpp`); `ColorSlot` assigns the
lowest free colour in that order. The post-colouring dump gives:

```text
race -> 0   this -> 1   type -> 2   wy -> 3   wz -> 4
jY   -> 5   jZ   -> 6   wx   -> 7   jX -> 8
```

which is exactly the committed slot table in §1.

The weight is `LiveIntervals::getSpillWeight(false, true, MBFI, MI) =
block_freq(MI.parent) / entry_freq` summed over every non-debug FI
reference (`LiveIntervals.cpp:860`, `StackSlotColoring.cpp:156`). In the
committed build the spawn is split by the `invoke`:

```text
bb.23 (pre-invoke, freq ~0x7ffff800)   z = wz + jZ,  y = wy + jY
bb.26 (post-invoke, freq ~0x7ffff800)  x = wx + jX
```

`bb.23` is fractionally hotter than `bb.26` (the landing-pad edge takes
`0x800` of `0x80000000`), so `{wy, wz, jY, jZ}` (referenced in `bb.23`)
outweigh `{wx, jX}` (referenced only in `bb.26`). The four heavier
intervals keep FI order among themselves, giving colours 3,4,5,6 =
`wy, wz, jY, jZ`; the two lighter ones take 7,8 = `wx, jX`. That is the
observed permutation. The tiny weight gap is decisive because
`IntervalSorter` is a plain float `>`.

## 3. The target is the no-sink FI order

The original has **no `invoke`**: `setEmpData` is a plain `blx` at
`0xc03ee`, and all six spawn loads + three adds are in one basic block
(`0xc03c8..0xc03e4`). All six FI weights are therefore equal, the
`stable_sort` degenerates to FI order, and the colour order equals the FI
creation order. The original's colour order from §1 is therefore its FI
order:

```text
target FI/colour order : wy, wz, wx, jX, jY, jZ
committed FI order     : wx, wy, wz, jX, jY, jZ
```

i.e. the first three are a left rotation `wx,wy,wz -> wy,wz,wx`, and the
jitter triple is unchanged. The committed build cannot reach it while the
`invoke` sinks `x` (and with it `wx`/`jX`) into `bb.26`.

## 4. The all-float spawn reproduces the target slot map exactly

A new 48-way sweep (8 `int`/`float` type combinations × 6 spawn-statement
orders, committed `try` + placement `new` + `switch`) found exactly one
family that reproduces the original's *complete* slot map:

```cpp
float x = (float) (wx + jitterX - 20000);
float y = (float) (wy + jitterY - 20000);
float z = (float) (wz + jitterZ - 20000);
```

Measured with the official verifier (`autojunk=False`):

| variant | score | usize | waypoint slots | jitter slots |
|---|---|---:|---|---|
| committed (`int x`, `float z`, `float y`) | **74.1** | 2764 | x=44 y=48 z=32 | jY=40 jZ=36 jX=28 |
| all-float, order `x y z` | 64.8 | 2748 | x=44 y=48 **z=40** | **jX=36 jY=32 jZ=28** |
| all-float, order `y x z` | 65.0 | 2748 | x=44 y=48 **z=40** | **jX=36 jY=32 jZ=28** |
| all-int, order `x y z` | 60.7 | 2752 | x=40 y=48 | — |

The all-float build has no integer spawn value to sink, so all six values
are referenced in one block, the weights tie, and the colour order is the
FI order — which is exactly `wy, wz, wx, jX, jY, jZ`. Its slot map is
byte-for-byte the original's.

But the all-float build loses the entry allocation. Its prologue is

```text
mov r9, r0        ; this   -> r9
mov sl, r3        ; type   -> sl        (original: fp)
mov r6, r2        ; shipClass -> r6     (original: r5)
mov r5, r1        ; race   -> r5        (original: [sp,#60])
str r5, [sp,#56]  ; race spilled
```

while the committed (and original) prologue is

```text
str r1, [sp,#60]  ; race   -> [sp,#60]
mov r9, r0        ; this   -> r9
mov fp, r3        ; type   -> fp
mov r5, r2        ; shipClass -> r5
```

The prologue match is worth far more than the slot match: the all-float
build drops 74.1 → 64.8/65.0 even though its slots are perfect.

## 5. The true original shape is all-int, and it also loses the prologue

The original's conversions are **after** the plain `setEmpData` call and
duplicated in both `shipClass` branches:

```text
c06c0  vmov s0,fp   ; x (int, kept in fp across the call)
c06c6  vmov s2,r8   ; z (int, r8)
c06cc  vmov s4,r4   ; y (int, r4)
c06d0  vcvt.f32.s32 s0,s0
c06d4  vcvt.f32.s32 s2,s2
c06d8  vcvt.f32.s32 s4,s4
```

so the source is all-int (`int x, y, z;` passed to the float ctors), not
all-float. Our all-int build (committed `try` + placement `new`) sinks
**all three** ints past the invoke:

```text
1b8  blx setEmpData
1bc  ldrd r1,r0,[sp,#36]   ; x recomputed after the call
1c0  ldr  r2,[sp,#32]
1c2  add  r0,r1
1c8  subs r4,r0,#20000
```

and its prologue becomes `shipClass -> [sp,#60]`, `type -> r9`,
`this -> [sp,#56]`, `race -> sl` — 60.6–61.6 %. The 36-way all-int
jitter-order × spawn-order sweep and the 6-way waypoint-declaration sweep
never change that prologue, so the all-int shape cannot be combined with
the committed entry allocation.

## 6. Other controls measured this pass

| variant | score | usize |
|---|---:|---:|
| committed | **74.1** | 2764 |
| `try` + placement `new` + if-chain 0x33/0x31/0x2c | 69.9 | 2760 |
| `try` + real `new` (no placement) | 63.4 | 2772 |
| no `try` + real `new` + if-chain (the real LSDA shape) | 65.9 | 2764 |
| no `try` + placement `new` + switch + mixed spawn | 70.0 | 2776 |
| no `try` + placement `new` + switch + all-int | 60.8 | 2748 |
| `catch (...) { }` (empty) | 65.1 | 2772 |
| `catch (int) { trap; }` | 66.1 | 2772 |
| `catch (...) { throw; }` | 49.4 | 2740 |
| switch case orders 0x2c/0x33/0x31 (current) | **74.1** | 2764 |
| switch case orders 0x2c/0x31/0x33 | **74.1** | 2764 |
| switch case orders 0x33/0x31/0x2c | 73.5 | 2764 |
| switch case orders 0x33/0x2c/0x31 | 73.5 | 2764 |
| switch case orders 0x31/0x2c/0x33 | 73.2 | 2764 |
| switch case orders 0x31/0x33/0x2c | 73.2 | 2764 |
| `int x` computed before `new Player` | 74.1 | 2764 |
| `int x` + explicit `(float) x` at both ctors | 74.1 | 2764 |
| `int x; float xf = (float) x;` + `xf` in ctors | 74.1 | 2764 |
| `KIPlayer *kiFighter = fighter;` removed | 74.1 | 2764 |

The spawn-type × spawn-order sweep top rows (48 builds):

```text
74.1  int x float y float z   orders xzy, zxy, zyx
73.9  int x float y float z   orders xyz, yxz, yzx
72.8  float x int y float z
72.4  float x float y int z
65.0  float x float y float z  (order yxz; target slots)
60.7  int x int y int z
```

## 7. Conclusion / what remains

* The residual slot difference is a **4-cycle of the last four colours**
  (`wx,jX` move from 7,8 to 5,6; `jY,jZ` move from 5,6 to 7,8).
* It is caused by the committed `try`/`invoke` sinking `x`, which gives
  `wx`/`jX` a slightly lower `StackSlotColoring` weight than the four
  values used in the pre-invoke block.
* The target colour order is the **no-sink FI order**
  `wy, wz, wx, jX, jY, jZ`; the all-float spawn reproduces it exactly but
  breaks the entry allocation (74.1 → 64.8/65.0).
* The true original shape is all-int with a plain call; in our build the
  all-int spawn also breaks the entry allocation (60.6–61.6 %).
* The entry allocation (race→`[sp,#60]`, type→`fp`, shipClass→`r5`,
  this→`r9`) and the target slot map currently require *different*
  integer-pressure profiles, so they cannot both be reproduced by the
  shapes tried here.
* No source change is justified: 74.1 remains the best measured score and
  the only score-positive variants (e.g. the reverted
  `PlayerFixedObject(race, type)` 74.2) are semantically wrong.

## 8. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w4258
cmake --build cmake-build-match --target check -j8
python3 tools/verify/verify.py \
  --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN5Level10createShipEiiiP8Waypointbb
```

Scratch helpers (all under `/dev/shm/w4258`): `harness2.py` (fast
link+verify harness on a repo copy), `probe3.py` (link + slot-map
extractor), `gen.py` (spawn/decl/jitter body generator), `fidump.py`
(`-print-after-all` FI→variable extractor), `sweep_decl.py` /
`sweep_slots.py` / `sweep_wp2.py` (earlier sweeps), `afterall_cs.txt`
(committed `-print-after-all` dump).
