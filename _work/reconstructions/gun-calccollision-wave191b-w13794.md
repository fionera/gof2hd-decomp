# `Gun::calcCharacterCollision` wave 191b (w13794): early-out / Vector-temp /
# float-compare audit — 463/57 plateau holds, no src change

Worker: w13794. Branch `wave191b-w13794`, base master `18079f9f1`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump `0x152f90`, size `1984 B`,
660 insns. Scratch: `/home/debian/GalaxyOnFire2/scratch/w13794`.
Predecessor notes: `gun-calccollision-wave191b-w13766.md` (faithful C-D-E tail
landed as pass 1 `6c9cd5c23`, 463/57 plateau), `...-w13752.md` (StackColoring
merge swap), `...-w13736.md` (RA priority). This pass takes a **distinct angle**:
full Ghidra-decompile vs source audit of the early-out ordering, the two 60-byte
`Vector`/`Matrix` stack temporaries, and the float compare order. The C-D-E
scatter tail is **not** reworked.

## 0. Result

**No `src/` change landed this session.** HEAD re-measured:

| build | raw | aligned | size | insns | `.text` | `.ARM.extab` | `.ARM.exidx` | `.rodata` |
|---|---:|---:|---:|---:|---:|---:|---:|---:|
| HEAD `18079f9f1` | **463** | **57** | 1984/1984 | 660/660 | 0x1337c8 | 0x26298 | 0x6f70 | 0x41b67 |
| E1 (faithful slot flip) | 467 | 67 | 1984/1984 | 660/660 | 0x1337c8 | 0x26298 | 0x6f70 | 0x41b67 |

Section **addresses** are unchanged (`.text`@0x69440, `.ARM.exidx`@0x19cc08,
`.ARM.extab`@0x1a3b78, `.rodata`@0x1c9e10); only the sizes are reproduced above.
`check` was not re-run because `src/` is untouched; the last full gate remains
byte_exact 4508/4524, linked_exact 4508/4524, size 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0, parity 5370/5370.

## 1. What the decompile audit confirmed already matches

`scratch/orig_decomp.c` (full 362-line Ghidra decompile) was compared against
`src/game/weapons/Gun.cpp:570-747` line by line:

- **Early-out ordering** is identical and compiles byte-for-byte:
  `!mine || (enemyFlagsLo != 0 && (ki == 0 || !ki->deadFlag))` then
  `isActive()` then `getHitpoints() > 0`. The mine gate is the `bVar6` short
  circuit in the decompile; the disasm at `0x222..0x2c6` matches except for the
  r4/r5 register swap.
- **`outer` / `outerCollide`** is identical: `bVar4 = outer && !outerCollide`
  (decompile `LAB_0016317c`/`LAB_001631b2`/`LAB_001631b8`), the box test then
  ANDs `!bVar4`. Our `if (!(outer && ki->outerCollide(position)))` is the same
  CFG.
- **Float compare order** is identical: `x < r`, `x > -r`, `y < r`, `y > -r`,
  `z < r`, `z > -r`, all short-circuit `&&`; the disasm `0x2c6..0x36c` is
  byte-identical apart from branch-target addresses. The mine branch re-checks
  with `enemy->radius` in the same order (`0x6xx`), also identical.
- **The two 60-byte stack temporaries** are exactly the original's
  `aVStack_10c` (a `Vector`-sized scratch) and `aAStack_d0` (a `Matrix`-sized
  scratch). The mine branch `VectorNormalize`/`operator*`/`operator+=` chain at
  `0x65a..0x6ac` is byte-identical apart from r4/r5.
- **The C-D-E scatter tail** (`diff = magnitude - distance`; clamp `falloff`;
  `damage`; conditional `damageEmp`) matches, landed by w13766.

Therefore the residual is **not** source-level logic. It is entirely the
physical-register choice: original keeps `i*3` in **r4** and rematerialises the
`sp+104` address per block (`r5` in the view block, `r4` in the k-loop); ours
keeps `i*3` in **r5** and the long-range `sp+104` address in **r4**.

## 2. The residual diff map (fn_diff raw 463 / aligned 57)

All 54 folded diffs trace to one of five clusters:

1. **r4↔r5 swap** at `0x306c, 0x3078, 0x308c, 0x30cc, 0x315a, 0x3436, 0x3608,
   0x361c, 0x3638` (`i*3` / `i*3*4` indexing).
2. **slot swap** at `0x30d2/0x30dc`: orig `add r4,sp,#44` (playerPosition) +
   `add r5,sp,#104` (delta); ours `add r4,sp,#104` + `add r5,sp,#44`.
3. **i reload**: orig `ldr.w fp,[sp,#40]` at `0x3198`/`0x3262`; ours inserts
   `add r4,sp,#104` at `0x3198`/`0x3260` and compensates with an extra
   `ldr.w fp,[sp,#40]` at `0x3278`.
4. **view block** `0x33c8..0x33f4`: orig materialises `add r5,sp,#104` once and
   reuses `r5` (`mov r0/r1/r2,r5`); ours rematerialises
   `add r0,sp,#104` / `add r1,sp,#104` / `add r4,sp,#104` + `mov r2,r4`.
5. **k-loop rotation** `0x3440..0x3506`: orig is header-at-bottom
   (`movs r5,#0; b 0x34fc`; body `0x3442`; latch `adds r5,#1` at `0x34fa`;
   header `0x34fc..0x3504 bcc 0x3442`); ours is header-at-top
   (`ldr r0,[r9,#180]; cmp; bcs.w` guard at `0x3448`; latch `adds; b header`).

Clusters 1–4 are all the long-range `%513` stack-address vreg; cluster 5 is
`MachineBlockPlacement` loop rotation.

## 3. E1 is the only shape that fixes the slot assignment — and it flips RA

`E1` is

```cpp
Vector delta = enemyPosition - ((Level *) this->level)->getPlayer()->getPosition();
int distance = (int) AbyssEngine::AEMath::VectorLength(delta);
```

`scratch/e1b.so` measures **467/67/1984/660**. Its disasm at `0x30c8..0x30ec`
is the original with **r4 and r5 globally swapped** (same instruction count):

```
ORIG 30cc mov r6,r4         E1 30cc mov r6,r5
ORIG 30d2 add r4,sp,#44     E1 30d2 add r5,sp,#44
ORIG 30dc add r5,sp,#104    E1 30dc add r4,sp,#104
ORIG 30e0 mov r2,r4         E1 30e0 mov r2,r5
ORIG 30e2 mov r0,r5         E1 30e2 mov r0,r4
```

So E1 gets the **slot** assignment right but keeps `i*3` in r5 and the
`sp+104` address in r4. HEAD is closer (3 differing insns at that site vs E1's
6) because it at least keeps the `sp+104` address in r4 like the original's
`playerPosition` load. E1's byte decomposition (48 runs vs HEAD 44) is in
`gun-calccollision-wave191b-w13766.md` §1.

## 4. Mechanism (re-confirmed this session)

- Pass order: EarlyTailDuplicate → OptimizePHIs → **StackColoring (pre-RA)** →
  LocalStackSlotAllocation → … → EarlyMachineLICM → MachineCSE → Greedy RA →
  PEI → (post-RA) StackSlotColoring.
- `pre_sc.mir` (line 12842) has **13 stack objects**: guard 4/4, position 12/8,
  velocity 12/8, enemyPosition 12/4, seven 12/4, then 60/8, 60/4, 12/4.
- `cc_lssa.mir` (line 433) after pre-RA StackColoring has **6 slots**:
  `%stack.0` 4/4 (-4), `%stack.1` 12/8 (-16), `%stack.2` 12/8 (-32),
  `%stack.3` 12/4 (-44), `%stack.4` 60/8 (-104), `%stack.5` 60/4 (-164).
- `%stack.4` (60/8, sp+104) is shared by the view alloca, the
  `PlayerEgo::getPosition` sret (playerPosition), the k-loop
  `Player::getPosition` sret (kpos), and the mine-branch
  `VectorNormalize`/`operator*` sret temps. `%stack.5` (60/4, sp+44) is the
  `MatrixSetTranslation` sret + the `operator-` sret (delta).
- `EarlyMachineLICM` hoists `t2ADDri %stack.4` (`%308`) into bb.5
  (`cc_licm.mir:609`); `MachineCSE` folds the per-block ADDri copies onto one
  long-range vreg `%513` (COPY chain `%504` bb.43 / `%506` bb.55 / `%498`
  bb.83; `cc_greedy.mir:1180/1372/1828`).
- Greedy RA priority (`rag.cpp:730`) is `(1u<<29) + Size` for global ranges.
  `%513` spans bb.21 (3604B) → bb.83 (15008B) ≈ **14608** slot units, beating
  `%99` (`i*3`, span ≈ **12832**). `%513` therefore dequeues first and takes
  **r4**; `i*3` falls to **r5**.
- The original avoids this: it recomputes `add rX,sp,#104` per block
  (`0x33c8` view, `0x3444` k-loop, `0x35f8`/`0x3626` mine) and holds no
  callee-saved register for the address. `-mllvm -disable-machine-licm`
  compiles but yields 635 insns / `vpush {d8}` (unusable shape), confirming
  MachineLICM is the hoist mechanism.

## 5. Candidates measured this session (all size-exact 1984/1984 · 660/660
unless noted; sections unchanged from HEAD unless noted)

### 5a. Source-shape rewrites that reproduce HEAD byte-for-byte (463/57)

| variant | change |
|---|---|
| `kpos_named` | `Vector kpos = enemy->getPosition(); *td = kpos;` |
| `kpos_assign` | `Vector kpos = ...; *td = kpos; *td -= position;` |
| `pos_ctor` | `Vector position(this->positions.data()[i]);` |
| `td_ref` | `Vector &td = this->targetDir;` |
| `td_direct` | `this->targetDir = ...; this->targetDir -= position;` |
| `view_ptr` / `view_reint` / `view_const` | pointer/`reinterpret_cast`/`const` camera-local forms |
| `view_ctor` | `Matrix view(*(Matrix *) CameraGetLocal(...));` |
| `view_acopy` | `*reinterpret_cast<Matrix *>(...)` |
| `view_ref` / `view_ptr2` | `Matrix &vr = view;` / `Matrix *vp = &view;` |
| `view_a8` | `alignas(8) Matrix view` |
| `named_delta` | `Vector delta = enemyPosition - playerPosition;` (extra name folded away) |
| `inline2` | `VectorLength(enemyPosition - playerPosition)` (inline arg) |
| `pp_named` | `PlayerEgo *player = getPlayer(); player->getPosition()` |
| `lvl_cached` | `Level *lvl = (Level *)this->level; lvl->getPlayer()->...` |
| `r_float` | `float r = radius;` instead of `(float) radius` |
| `k_int` | `for (int k = ...)` |
| `kwhile` | `unsigned k = 0; while (k < size()) { ...; ++k; }` |

### 5b. Candidates that regress

| variant | raw | aligned | size/insns | note |
|---|---:|---:|---|---|
| **HEAD** | **463** | **57** | 1984/660 | — |
| `E1` | 467 | 67 | 1984/660 | faithful slot flip, RA flipped |
| `inline` (getPosition inline in `operator-`) | 467 | 67 | 1984/660 | same as E1 |
| `mine_scope` (nested-scope `Vector n`) | 465 | 59 | 1984/660 | — |
| `mine_scope2` | 465 | 59 | 1984/660 | — |
| `E1_minescope` | 469 | 69 | 1984/660 | — |
| `i_int` / `e_int` | 464 | 58 | 1984/660 | signed loop var |
| `ie_int` | 465 | 59 | 1984/660 | — |
| `perm_pev` | 508 | 81 | 1984/660 | decl order |
| `perm_vpe` | 478 | 80 | 1984/660 | decl order |
| `perm_vep` | 517 | 96 | 1984/660 | decl order |
| `perm_epv` | 546 | 101 | 1984/660 | decl order |
| `perm_evp` | 546 | 104 | 1984/660 | decl order |
| `minedir` / `minedir2` | 494 | 107 | — | named mine temp |
| `kdowhile` | 1024 | 156 | 1988/662 | do-while |
| `view_a16` / `view_a32` | 1862/1864 | 209 | 1996/664 | over-aligned |
| `view_iloop` | 1871 | 348 | 2024/682 | Matrix hoisted to i-loop |
| `view_fn` | 1890 | 333 | 2016/675 | Matrix at fn top |
| `kexp1` | 1800 | 355 | 1948/654 | `__builtin_expect` on k cond |
| `kexp0` | 1717 | 303 | 1944/648 | — |
| `kpos_top` | 1668 | 271 | 2004/669 | kpos before k-loop |
| `kpos_fn` | 1826 | 292 | 1972/664 | kpos at fn top |
| `e1_kpostop` | 1669 | 262 | 2004/669 | — |
| `radius_first` | 1789 | 199 | 1968/656 | radius before pos/vel |
| `ki_top` | 1680 | 310 | 1952/657 | `KIPlayer *ki` early |
| `mine_top` | 1771 | 188 | 2004/667 | named mineDir at i-loop top |
| `kpos_assign2` | 1771 | 324 | 1972/668 | — |
| `view_memcpy` / `view_memcpy60` | 1923 | 244 | 2064/690 | `__builtin_memcpy` |
| `view_camfirst` | 1448 | 282 | 1968/655 | `unsigned camera` first |
| `view_block` | 1889 | 243 | 2056/686 | nested block copy |
| `pp_fn` | 1795 | 246 | 1960/659 | playerPosition at fn top |
| `delta_fn` | 1587 | 238 | 1964/661 | delta at fn top |
| `assign_delta` | 1643 | 283 | 1964/661 | `delta = ...` after decl |
| `scope_delta` / `delta_first` / `pp_then_delta` | 1643/1644 | 281/283 | 1964/661 | named delta |
| `td_local` | 1818 | 344 | 1956/652 | local `Vector td` |

## 6. Conclusion

The early-out ordering, `outer`/`outerCollide` gate, float compare order, and
both `Vector`/`Matrix` 60-byte temporaries are already faithful and compile
byte-for-byte (modulo r4/r5). The remaining 57 aligned bytes are a single
pre-RA **StackColoring merge** of the view/kpos/mine/delta temporaries into one
`%stack.4`, which MachineLICM hoists and MachineCSE folds into a long-range
`%513`; Greedy RA then gives `%513` r4 (span 14608 > `i*3` 12832) and pushes
`i*3` to r5, cascading into the slot swap, the i reload, the view-block
rematerialisation, and the k-loop rotation.

No source shape found in 40+ candidates beats 463/57. E1 (`467/67`) is the only
shape that reproduces the original's physical slot assignment, but it still
leaves `i*3` in r5. Breaking the merge (separate allocas / hoisting a `Matrix`
out of the branch / over-aligning / `__builtin_memcpy` / named-delta forms) all
grow the frame or instruction count. This is a register-allocation /
block-placement artifact, not a source-logic difference.
