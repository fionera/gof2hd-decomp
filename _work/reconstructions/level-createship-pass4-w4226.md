# Level::createShip — pass 4 (wave 92, w4226)

**Status:** analysis only, no source change landed (2026-11-03). Worktree
`wt/w4226`, branch `wave92-w4226`, base `b379d00d` (HEAD `666d7c5e`).
Target: `_ZN5Level10createShipEiiiP8Waypointbb` (original 2764 B @ `0xc0214`,
Thumb entry `0xc0215`). Current verifier score **74.1 %** (osize 2764,
usize 2764, `byte False`).

Gate before and after (unchanged, no code change):

```text
byte_exact 4498/4524  linked_exact 4498/4524  size-exact 4524/4524
stub_zero_size 0  missing 0  wrong_type 0  extra 0  data_exact 843/843
imports 207  .text 0x1337c8  .rodata 0x41b67  lint CLEAN ✓  drift 0
RATCHET PASS
```

This pass continues `level-createship-pass3-w4106.md`. It corrects the
stack-slot labels used by the earlier notes (§1), derives the exact
`StackSlotColoring` colour permutation from a `-print-after-all` dump of
the committed build (§2), explains *why* the committed try/placement-`new`
shape cannot reproduce the original colouring (§3), and reports a
systematic 216-way jitter/waypoint/spawn sweep plus the declaration-list
and all-float controls (§4–§6). No source shape found beats 74.1 %.

---

## 1. Corrected semantic stack-slot map

The committed source deliberately swaps the `wx`/`wz` *names*
(`src/game/world/Level.cpp:5168`):

```cpp
int wx, wz, wy;
if (waypoint != nullptr) {
    wz = waypoint->x;   // wz holds waypoint->x
    wy = waypoint->y;
    wx = waypoint->z;   // wx holds waypoint->z
} else { wz = 0; wy = 0; wx = 0; }
...
int x = wx + jitterX - 20000;
float z = (float) (wz + jitterZ - 20000);
float y = (float) (wy + jitterY - 20000);
```

Because of that swap, the slot labels in the pass-2/pass-3 notes
(`wz@40`, `wx@44`) refer to the *source names*, not to the semantic
coordinate. Reading the disassembly by dataflow (the `PlayerFixedObject`
ctor takes `x` in `[sp,#4]`, `y` in `[sp,#8]`, `z` in `[sp,#12]`, so
`s20=x, s18=y, s16=z`) gives the real map:

| value | original | committed |
|---|---|---|
| `race` | `[sp,#60]` | `[sp,#60]` |
| `this` | `[sp,#56]` | `[sp,#56]` |
| `type` | `[sp,#52]` | `[sp,#52]` |
| `waypoint->y` (`wy`) | `[sp,#48]` | `[sp,#48]` |
| `waypoint->x` (`wz`) | `[sp,#44]` | `[sp,#44]` |
| `waypoint->z` (`wx`) | **`[sp,#40]`** | **`[sp,#32]`** |
| `jitterX` | **`[sp,#36]`** | **`[sp,#28]`** |
| `jitterY` | **`[sp,#32]`** | **`[sp,#40]`** |
| `jitterZ` | **`[sp,#28]`** | **`[sp,#36]`** |

Original spawn (ELF addresses; `objdump -d`):

```text
c03c8  ldr  r2,[sp,#48]   ; wy
c03ca  ldr  r3,[sp,#32]   ; jY
c03cc  ldr  r0,[sp,#44]   ; wx (waypoint->x)
c03ce  ldr  r1,[sp,#36]   ; jX
c03d0  add  r2,r3         ; y = wy + jY
c03d2  ldr  r3,[sp,#40]   ; wz (waypoint->z)
c03d4  ldr  r4,[sp,#28]   ; jZ
c03d6  add  r0,r1         ; x = wx + jX
c03d8  movw r1,#20000
c03dc  sub.w fp,r0,r1     ; x int
c03e0  add  r3,r4         ; z = wz + jZ
c03e2  subs r4,r2,r1      ; y int
c03e4  sub.w r8,r3,r1     ; z int
```

Committed spawn (`Level_base.o`, offsets relative to the section):

```text
1b4  ldr  r0,[sp,#44]   ; wx (waypoint->x)
1b6  movw r4,#20000
1ba  ldr  r1,[sp,#36]   ; jZ
1bc  ldr  r2,[sp,#40]   ; jY
1be  add  r0,r1         ; z = waypoint->x + jZ
1c0  ldr  r1,[sp,#48]   ; wy
1c2  subs r0,r0,r4      ; z int -> s16
1c4  add  r1,r2         ; y = wy + jY
1c6  subs r1,r1,r4      ; y int -> s18
1c8  vmov s0,r1
1cc  vcvt s18,s0
1d0  vmov s0,r0
1d4  vcvt s16,s0
1d8  mov  r0,r5
1da  mov  r1,sl
1dc  mov  r2,r9
1de  bl   setEmpData
1e2  ldrd r1,r0,[sp,#28]  ; r1=jX, r0=waypoint->z
1e6  cmp  r6,#0
1e8  ldr.w sl,[sp,#56]
1ec  add  r0,r1           ; x = waypoint->z + jX
1ee  sub.w r4,r0,r4       ; x int -> s20
```

So the committed build converts `y` and `z` **before** the call and
recomputes `x` **after** it, while the original computes all three ints
before the call and converts all three after it (duplicated in both
`shipClass` branches). The prologue is identical.

**Slot difference is a 4-cycle of the last four colours**: the first five
(`race`, `this`, `type`, `waypoint->y`, `waypoint->x`) match exactly; the
remaining four are rotated

```text
original : waypoint->z, jitterX, jitterY, jitterZ
committed: jitterY,    jitterZ, waypoint->z, jitterX
```

## 2. LLVM 7.0.2 evidence: FI creation order and the colour permutation

`-mllvm -print-after-all` on the committed `Level.cpp` (same flags as
`build_obj.sh`) gives the machine function after `Greedy Register
Allocator` and after `Stack Slot Coloring`. The spill-slot creation order
(FI index) is fixed by `VirtRegMap::createSpillSlot` calls from
`InlineSpiller::spillAll`; the *colour* is then chosen by
`StackSlotColoring::ColorSlot` (lowest free FI, intervals sorted by
`li.weight` with `std::stable_sort`).

Creation order (FI index before colouring), read from the `%stack.N`
operands and `DBG_VALUE`s in the post-RA dump:

```text
stack.0 = this        stack.3 = waypoint->z   stack.6 = jitterX
stack.1 = type        stack.4 = waypoint->y   stack.7 = jitterY
stack.2 = race        stack.5 = waypoint->x   stack.8 = jitterZ
```

Colouring permutation (post-`Stack Slot Coloring` dump):

```text
race       -> stack.0        waypoint->z -> stack.7
this       -> stack.1        jitterX     -> stack.8
type       -> stack.2        jitterY     -> stack.5
waypoint->y-> stack.3        jitterZ     -> stack.6
waypoint->x-> stack.4
```

`PrologEpilogInserter` then lays the nine 4-byte objects out at
`[SP-68-4k]`; the frame's final SP is `S-128` (`push {r4,r5,r6,r7,lr}` =
20 B + `stmdb {r7,r8,r9,sl,fp}` = 20 B + `vpush {d0-d10}` = 88 B), so
`fi#k` lands at `[sp, 60-4k]`. That reproduces the table in §1 exactly.

The original's colour assignment (offset `60-4k`) is therefore:

```text
k=0 race        k=3 waypoint->y  k=6 jitterX
k=1 this        k=4 waypoint->x  k=7 jitterY
k=2 type        k=5 waypoint->z  k=8 jitterZ
```

i.e. the original's colour order is
`race, this, type, waypoint->y, waypoint->x, waypoint->z, jitterX, jitterY, jitterZ`.

## 3. Why the committed shape cannot reach it

`StackSlotColoring::ScanForSpillSlotRefs` sets

```text
li.weight += LiveIntervals::getSpillWeight(false, true, MBFI, MI)
           = block_freq(MI.parent) / entry_freq
```

for every non-debug FI reference (`LiveIntervals.cpp:860`,
`StackSlotColoring.cpp:156`). `ColorSlot` then walks the intervals in
descending weight order (stable, so ties keep FI order) and assigns the
lowest free colour.

In the committed build the `try`/`catch` around `setEmpData` creates a
landing pad, and `MachineSink` sinks the `int x` computation past the
invoke. The spawn therefore spans **two** blocks:

```text
bb.23 (before the invoke)  z = waypoint->z + jitterZ   (stack.7 + stack.6)
                           y = waypoint->y + jitterY   (stack.3 + stack.5)
bb.26 (after the invoke)   x = waypoint->x + jitterX   (stack.4 + stack.8)
```

`bb.23` and `bb.26` have different block frequencies (the landing pad
forces `bb.26` to a distinct frequency), so `waypoint->x` and `jitterX`
get a lower weight than the other four and are pushed to the end of the
colour order — exactly the observed rotation. The original has **no
`try`**: all six spawn values are used in one block, all six weights are
equal, and the colour order collapses to the FI creation order.

Reproducing the original therefore needs *both* a no-`try` shape (single
spawn block) *and* the original FI creation order
`race, this, type, waypoint->y, waypoint->x, waypoint->z, jitterX, jitterY, jitterZ`.
The faithful no-`try` shape loses the prologue match (§6), so the two
requirements currently conflict.

## 4. 216-way jitter / waypoint / spawn sweep

Scratch harness `/dev/shm/w4226/sweep3b.py`: for each variant it rewrites
the body, compiles `Level.cpp` with the exact `build_obj.sh` flags, and
parses the waypoint-branch stores, the jitter-call stores and the spawn
loads from the object. Swept dimensions:

* waypoint assignment order — all 6 permutations of
  `wz = waypoint->x`, `wy = waypoint->y`, `wx = waypoint->z`;
* jitter declaration order — all 6 permutations of `jitterX/Y/Z`;
* spawn declaration order — all 6 permutations of the three
  `x`/`y`/`z` initialisers (types held at the committed `int x`,
  `float z`, `float y`).

Result (216 builds):

```text
waypoint->z slot : 32 in 216/216 variants
jitter slot tuple: (28,40,36) x72, (40,28,36) x72, (40,36,28) x72
target           : waypoint->z=40, jitter=(36,32,28)   -> 0 hits
```

Slot 32 is *always* taken by `waypoint->z`; the three jitter values are
always a permutation of `{28,36,40}`. The target requires `jitterY` to
take 32 and `waypoint->z` to take 40, which this family cannot produce.

## 5. Declaration-list order and all-float controls

`int wx, wz, wy;` declaration-list order (6 permutations, base assignment
order) does not move `waypoint->z` off slot 32; it only reorders the
`else`-branch zero stores (`48,44,32` vs `48,32,44` etc.).

All-float spawn (`float x/y/z` with the same `try` + placement `new`)
collapses the score to **64.4–65.0 %** at 2748 B for all six declaration
orders:

| spawn | score | size |
|---|---|---|
| `fff` xyz | 64.8 | 2748 |
| `fff` zyx | 64.9 | 2748 |
| `fff` yzx | 64.6 | 2748 |
| `fff` xzy | 64.4 | 2748 |
| `fff` yxz | 65.0 | 2748 |
| `fff` zxy | 64.5 | 2748 |
| `ffi` xyz | 64.8 | 2748 |

So the committed mixed `int x` / `float z,y` shape is load-bearing for the
74.1 local optimum; the real source is almost certainly all-float (the
ctors take `float` positions and the original keeps ints in core
registers until the call), but the faithful shape does not match the
committed register allocation.

## 6. Faithful no-`try` shape (control)

`func_faithful.cpp`: no `try`, plain `new T(...)`, all-int spawn, faithful
`wx = waypoint->x` naming. Official verifier: **63.1 %**, usize 2788.
Object slot map: `waypoint->x=36`, `waypoint->y=44`, `waypoint->z=40`,
`jitterY` stays in `fp`, `jitterZ=28`, `jitterX=24`. It gets
`waypoint->z=40` right (§1 target) but the rest wrong, and the prologue
spills `type` (`[sp,#60]`) and `race` (`[sp,#56]`) with `shipClass` in
`sl`, instead of only `race` with `type -> fp` / `shipClass -> r5`.

Other controls measured this pass (official verifier, `autojunk=False`):

| variant | score | size |
|---|---|---|
| committed baseline | **74.1** | 2764 |
| `PlayerFixedObject(race, type, ...)` (semantically wrong) | 74.2 | 2764 |
| `PlayerFighter(race, type, ...)` | 73.9 | 2764 |
| both ctor arg orders swapped | 74.0 | 2764 |
| `int x` + dead use `if (x == 0x7fffffff) z = y;` before the `try` | 74.1 | 2764 |
| all-int spawn + `try` + placement `new` | 60.6 | 2752 |

The 74.2 `PlayerFixedObject(race, type)` variant is the one reverted by
`94e8bc54` (74.2 -> 74.1) to match Ghidra's `(type, race)`; it is a pure
register-allocation coincidence, not a source shape to land.

## 7. What remains

1. A no-`try` shape whose entry allocation is `race -> [sp,#60]`,
   `type -> fp`, `shipClass -> r5` (the faithful shape spills `type` and
   `race`). The original's LSDA is cleanup-only; the committed `try`
   exists only to buy the prologue.
2. A no-`try` shape whose FI creation order is
   `race, this, type, waypoint->y, waypoint->x, waypoint->z, jitterX, jitterY, jitterZ`.
   With one spawn block the colouring is the identity on that order, so
   this is the *only* remaining degree of freedom.
3. The `setEmpData` scheduler order (pass 3): the SelectionDAG list
   scheduler emits the call before the three int adds unless the call has
   a genuine data successor. Both problems must be solved together.

No source change is justified: `byte_exact` does not rise (4498 before and
after) and the only score-positive variant (74.2) re-introduces the
`PlayerFixedObject` argument-order bug that `94e8bc54` fixed.

## 8. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w4226
cmake --build cmake-build-match --target check -j8
python3 tools/verify/verify.py \
  --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN5Level10createShipEiiiP8Waypointbb
```

Scratch helpers (all under `/dev/shm/w4226`): `harness_full.py` (apply a
whole-function variant, link, verify), `objsweep.py` (apply a body and
compile a single object), `slotmap3.py` (dataflow slot extractor),
`sweep3b.py` (the 216-way sweep, output in `sweep3b.out`), `sweep4.py`
(declaration-list and faithful controls), `ra_cs.txt` / `ssc_after.txt`
(post-RA and post-colouring machine dumps for the committed build).
