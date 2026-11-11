# `Level::createMission` (`_ZN5Level13createMissionEv`) — wave 179 / w12313

Target: `src/game/world/Level.cpp`, function at VMA `0xaffd8`, size **17,172 B**
(`0x4314`).  Branch `wave179-w12313`, base master `64014ce6f`.
Ground truth `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, armeabi-v7a).

## 1. Result

**One size-exact code win landed.**  Three independent, semantics-preserving
store-order changes (two turret loops + the inner `switch (k)` in the `case 183`
convoy block).

```text
                                     size            raw    aligned  insn
base 64014ce6f                       17172/17172     4467   952      6208/6197
landed (this pass)                   17172/17172     4455   967      6208/6197
```

Section bytes (measured in this worktree):

```text
.text        size 1,259,464 / 1,259,464   differing bytes 77,936 -> 77,924  (-12)
.ARM.extab   size   156,312 /   156,312   differing bytes    700 ->    700  (0)
.ARM.exidx   size    28,528 /    28,528   differing bytes      0 ->      0
.rel.plt     size    29,392 /    29,392   byte-identical (cmp clean)
.rodata/.data/.bss/.got/.dynsym/.dynstr/.hash/.rel.dyn   exact
```

`.text` and `.ARM.extab` stay size- and base-exact (`.ARM.extab` @ `0x1a3b78`).
The function is still not byte-exact (it is one of the 17 residual functions),
so the ratcheted `byte_exact` count is unchanged.

Gate after the change: `byte_exact 4507/4524`, `linked_exact 4507/4524`,
`size_exact 4524/4524`, `stub_zero_size 0`, `missing 0`, `wrong_type 0`,
`extra 0`, `data_exact 843/843`, imports **207**, lint **CLEAN ✓**, drift **0**,
symbol parity **5370/5370**.  `RATCHET PASS`; no baseline update needed.

## 2. Landed source shape

### 2a. Alien-turret loop (`if (alienGuns != 0)`, `Level.cpp:1965`)

```cpp
            ((PlayerTurret *) gun)->setScaling(6.0f);
-           gun->shipGroup = localRace;
-           gun->cargo = nullptr;
+           gun->cargo = nullptr;
+           gun->shipGroup = localRace;
```

### 2b. Pirate-fortress turret loop (`if (pirateFortress)`, `Level.cpp:1985`)

```cpp
            ((PlayerTurret *) gun)->setScaling(6.0f);
-           gun->cargo = nullptr;
-           gun->shipGroup = 1;
+           gun->shipGroup = 1;
+           gun->cargo = nullptr;
             t += 6;
```

Both are independent non-volatile stores to distinct fields of the same object;
their order is unobservable.  Individually the alien swap is raw 4464 and the
pirate-fortress swap raw 4461; together 4458.

### 2c. Inner `switch (k)` of the `case 183` convoy block (`Level.cpp:2190`)

The four bodies are permutations of the same three independent `float` stores to
the local `offset[3]` (fully overwritten before the `setPosition` read):

```cpp
                        case 0:                    // order 2,0,1
                            offset[2] = -50000.0f;
                            offset[0] = 20000.0f;
                            offset[1] = -3000.0f;
                            break;
                        case 1:                    // order 1,0,2
                            offset[1] = -7000.0f;
                            offset[0] = 10000.0f;
                            offset[2] = -30000.0f;
                            break;
                        case 2:                    // order 2,1,0 (unchanged)
                            offset[2] = -36000.0f;
                            offset[1] = 0.0f;
                            offset[0] = 25000.0f;
                            break;
                        default:                   // order 2,0,1 (unchanged)
                            offset[2] = -45000.0f;
                            offset[0] = 12000.0f;
                            offset[1] = 6000.0f;
                            break;
```

`case 0` and `case 1` are the only ones that changed (from the wave-177/178
`case0=0,1,2` / `case1=2,0,1` shape).  The `default` comment about literal-pool
ordering was dropped because the pool order now differs (see §4).

## 3. Mechanism

The raw metric counts differing bytes at the same address.  `createMission` is
17 KB and not byte-exact, so the residual is dominated by register/stack-slot
allocation, not by wrong semantics.  These store permutations perturb clang's
`MachineScheduler` / `StackSlotColoring` / induction-variable strength reduction
just enough to move the emitted bytes onto the original's addresses:

* The two turret loops are otherwise identical and share the same `setScaling`
  / `cargo` / `shipGroup` tail.  Reordering the two stores changes the
  post-call schedule of the loop tail (original: `cargo` then `shipGroup`), and
  the surrounding stack-slot rotation follows.
* In the convoy `switch (k)` the store order changes the size/order of the four
  case bodies.  That in turn changes clang's choice of induction variable for
  the enclosing `for (k = 0; k < 4; k++)` loop: the original uses the index form
  `str.w r0,[r1,r6,lsl #2]` (r6 = 10..13); the "natural" all-`102` order makes
  clang strength-reduce to a byte offset (`str r0,[r1,r4]`, r4 = 40,44,...) and
  the function comes out 8 B short (17,164).  The landed `201/102/210/201`
  permutation keeps the index form and the exact 17,172 B size.

## 4. Measured variants (all real `link_so` + `tools/fn_diff.py`)

### Turret-loop store permutations (on top of the two cargo swaps)

Full 4! sweep of `setPosition` (P), `setScaling` (S), `cargo` (C), `shipGroup`
(G) after the rotation block.  Alien loop, best size-exact rows:

```text
order   raw   aligned        order   raw   aligned
PSCG    4458  953   (landed)  PCSG    4463  953
PSGC    4461  953            PCGS    4463  953
PGCS    4461  953            PGSC    4463  953
SPCG    4462  953            CPSG    4466  953
```

Pirate-fortress loop (same sweep): `PSGC` = 4458 (landed), `PSCG` = 4464,
`PCSG` = 4473, `PGSC` = 4475, …  Both loops are already at their optimum in the
landed shape.

### `switch (k)` store orders (on top of the cargo swaps)

`caseN` is the order of the three `offset[]` assignments.  Full 6×6 sweeps of
`case0`×`case1` (with `case2=210, default=201`) and of `case2`×`default` (with
`case0=201, case1=102`); best size-exact rows:

```text
case0 case1 case2 default   raw    aligned
201   102   210   201       4455   967   LANDED
201   012   210   201       4456   960
102   201   210   201       4458   953   (wave-178 shape)
210   102   210   201       4458   958
012   201   210   201       4459   942
102   102   210   201       4459   959
201   102   210   102       4459   955
```

Coordinate descent from `(201,102,210,201)` over each of the four bodies is a
strict local optimum.  The source order of the `case` labels does not matter
(`0,1,2` and `0,2,1` produce identical code; other orders move the whole
function and are much worse).

### Other measured (size-exact unless noted)

```text
variant                                         raw     size
bigAlienWave (drop the dead `(i < 0)` term)     6080    17160 (-12, not landable)
all-102 switch order (matches original pool)   10886    17164 (-8,  not landable)
declaration order BPA (bools)                   6720    17172
`field_0x74`/`shipGroup` swap in enemies[8]     4458    17172
`field_0x74`/`shipGroup` swap in enemies[9]     4461    17172
crateCaptured/cargo swap (case 9)               4461    17172
crateCaptured/cargo swap (case 2)               4456    17172
wreck setDockingType/field_0x70 swap            4505    17172
wreck setPosition/setDeadButSelectable swap     4477    17172
miningPlant setDockingType/setMoving swap       4463    17172
case4Tail `enemies` local before `globals`      4455    17172
```

The `(bigAlienWave + (i < 0))` term is provably dead (`i >= 0` in the loop) and
the original's disassembly at `0xb32c6` tests only `bigAlienWave` (`cbz` on the
`sp+64` bool, branch-selected table pointer); dropping it gives the right
branch shape but clang still emits a `it/moveq` cmov and the function lands
12 B short, so it is not landable on its own.

## 5. Reproduce

```sh
cd /home/debian/GalaxyOnFire2/wt/w12313
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
cmake --build cmake-build-match --target check -j2
cmake --build cmake-build-match --target drift -j2
cmake --build cmake-build-match --target diff -j2
```

Expected: `raw=4455 aligned=967 size=17172/17172`, `RATCHET PASS`,
`DRIFT: 0`, `SYMBOL PARITY ✓`.

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w12313/`
(`Level.base.cpp`, `Level.win1.cpp`, `Level.best.cpp`, `patch_block_w1.py`,
`combo.py`, `batch.py`, `dd.py`, `sizewalk.py`, `rawmap.py`, `addrwalk.py`).
