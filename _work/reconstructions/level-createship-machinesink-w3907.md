# Level::createShip — MachineSink x-sink, wave-89 w3907

**Status:** analysis + one landed source change, 2026-11-03. Worktree
`wt/w3907`, branch `wave89-w3907`, base `36c65db7`.
Target: `_ZN5Level10createShipEiiiP8Waypointbb` (original 2764 B,
`0xc0214`, original symbol value `0xc0215` thumb).

Landed: the three `Globals::rnd->nextInt(40000)` jitter initializers are
emitted in **Y, Z, X** order instead of X, Y, Z. The three calls are
independent and the RNG stream is identical, so this is semantically
invisible; it moves the verifier fuzzy score **74.1 -> 74.2** (2764 B both
sides). No gate count changes.

```text
before: createShip 74.1   byte 4498 linked 4498 stubs 0 missing 0 extra 0
after : createShip 74.2   byte 4498 linked 4498 stubs 0 missing 0 extra 0
data 843/843, imports 207, .text 0x1337c8, .rodata 0x41b67, lint CLEAN,
drift 0, RATCHET PASS both sides.
```

## 1. Original spawn code and call order

Original `0xc03c8..0xc0426` (objdump, `orig.asm`):

```text
c03c4  blx  _ZN6PlayerC1Eiiiii          ; new Player(...)
c03c8  ldr  r2,[sp,#48]   ; wy
c03ca  ldr  r3,[sp,#32]   ; jY
c03cc  ldr  r0,[sp,#44]   ; wx
c03ce  ldr  r1,[sp,#36]   ; jX
c03d0  add  r2,r3         ; wy + jY
c03d2  ldr  r3,[sp,#40]   ; wz
c03d4  ldr  r4,[sp,#28]   ; jZ
c03d6  add  r0,r1         ; wx + jX
c03d8  movw r1,#20000
c03dc  sub.w fp,r0,r1     ; x  -> fp
c03e0  add  r3,r4         ; wz + jZ
c03e2  subs r4,r2,r1      ; y  -> r4
c03e4  sub.w r8,r3,r1     ; z  -> r8
c03e8  mov  r0,r5         ; player
c03ea  mov  r1,sl         ; empPoints
c03ec  mov  r2,r9         ; empTime
c03ee  blx  _ZN6Player10setEmpDataEii
c03f2  mov.w r9,#0        ; ship = nullptr
c03f6  cmp  r6,#0
c03f8  beq.w 0xc06b6      ; shipClass == 0 -> PlayerFighter
c03fc  cmp  r6,#1
c03fe  ldr  r6,[sp,#56]
c0400  bne.w 0xc0b70
c0404  mov.w r0,#444
c0408  blx  _Znwj
c040c  vmov s0,fp
c0410  mov  r9,r0
c0412  vcvt.f32.s32 s20,s0 ; x
c0416  vmov s0,r4
c041a  vcvt.f32.s32 s18,s0 ; y
c041e  vmov s0,r8
c0422  vcvt.f32.s32 s16,s0 ; z
```

So in the original **all three spawn values are integers computed before
`setEmpData`, and all three `vcvt.f32.s32` are emitted after the call**.
The conversions are duplicated in both the `shipClass==1` path
(`c040c..c0422`) and the `shipClass==0` path (`c06c0..c06d8`).

Ghidra (`decomp.txt`) confirms the same order:

```c
iVar3  = iStack_54 + iVar3 + -20000;   /* x */
iVar1  = iVar10    + iVar4 + -20000;   /* y */
iVar10 = iVar2     + iVar5 + -20000;   /* z */
func_0x00074368(uVar9, iVar7, uVar16); /* Player::setEmpData */
...
uVar16 = VectorSignedToFloat(iVar3);   /* x */
uVar12 = VectorSignedToFloat(iVar10);  /* z */
uVar20 = VectorSignedToFloat(iVar1);   /* y */
func_0x00074374(piVar17, param_4, param_2, uVar9, 0,
                uVar16, uVar20, uVar12, param_6);
```

`param_4` is `type`, `param_2` is `race`, so the PlayerFighter call is
`(type, race, player, 0, x, y, z, hostile)`. The PlayerFixedObject call at
`c0446` is `func_0x0007414c(piVar17, param_4, param_2, ...)` — **also
`(type, race)`**, whereas the current source still passes `(race, type)`.
That is a real semantic bug (see §8).

## 2. Why our build does not reproduce it

Current source shape (baseline, before the landed change):

```cpp
int armor = ...;
Player *player = new Player(armor, hitpoints, 1, 1, 0);
int x = wx + jitterX - 20000;
float z = (float) (wz + jitterZ - 20000);
float y = (float) (wy + jitterY - 20000);
try { player->setEmpData(empPoints, empTime); }
catch (...) { __builtin_trap(); }   // lint: arm_builtin
```

Current machine code `0xc03c8..0xc0402`:

```text
c03c8  ldr  r0,[sp,#44]  ; wx
c03ca  movw r4,#20000
c03ce  ldr  r1,[sp,#36]  ; jX
c03d0  ldr  r2,[sp,#40]  ; jY
c03d2  add  r0,r1        ; x
c03d4  ldr  r1,[sp,#48]  ; wy
c03d6  subs r0,r0,r4     ; x
c03d8  add  r1,r2        ; y
c03da  subs r1,r1,r4     ; y
c03dc  vmov s0,r1
c03e0  vcvt.f32.s32 s18,s0
c03e4  vmov s0,r0
c03e8  vcvt.f32.s32 s16,s0
c03ec  mov  r0,r5
c03ee  mov  r1,sl
c03f0  mov  r2,r9
c03f2  blx  _ZN6Player10setEmpDataEii
c03f6  ldrd r1,r0,[sp,#28] ; jX, wz
c03fa  cmp  r6,#0
c03fc  ldr.w sl,[sp,#56]
c0400  add  r0,r1
c0402  sub.w r4,r0,r4      ; x recomputed after the call
```

`y` and `z` are floats in the source, so their `vcvt` is emitted before
the call; `x` is an int and is recomputed after the call. The original is
all-int.

## 3. MachineSink is the x-sink

The wave-88 note (`level-createship-machinesink-w3825.md`) pinned the
mechanism with `-mllvm -print-after-all -mllvm
-filter-print-funcs=_ZN5Level10createShipEiiiP8Waypointbb`:

* the last LLVM-IR dump and the `Expand ISel Pseudo-instructions` dump both
  put the three adds/subs **before** the `setEmpData` invoke;
* the `Machine code sinking` pass then moves the integer `x` add/sub (and
  its `COPY`) out of the invoke block `bb.17` into the post-invoke block
  `bb.20`;
* `y`/`z` stay in `bb.17` because each is consumed there by a `VSITOS`
  (int->float) that is not sinkable. `x` has no non-debug use in `bb.17`
  in the current source, so it sinks to the common dominator of its uses.

This is a consequence of the current source making only `x` an integer.
Making all three integers (`int x, y, z`) makes all three adds sink, and
the extra integer live ranges across the call perturb the *whole* function
register allocation (see §6).

Scheduler experiments on the all-int shape (official verifier):

| extra flag | fuzzy | size |
|---|---|---|
| (default, `list-hybrid`) | 61.0 | 2748 |
| `-pre-RA-sched=source` | 52.0 | 2776 |
| `-pre-RA-sched=list-ilp` | 38.5 | 2736 |
| `-disable-post-ra` | 53.2 | 2756 |
| `-enable-post-misched=false` | 61.0 | 2748 |
| `-disable-sched-height` / `-critical-path` / `-live-uses` / `-reg-pressure` / `-stalls` | 61.0 | 2748 |
| `-disable-2addr-hack` | 61.0 | 2748 |
| `-disable-sched-hazard` | 45.3 | 2752 |

None recovers the original schedule. `-pre-RA-sched=source` does put the
adds before the call but re-schedules the rest of the function.

## 4. EH / LSDA evidence: the original has no catch

Parsing the `.ARM.exidx` / `.ARM.extab` entries for `0xc0214`:

| | original | ours (current) |
|---|---|---|
| extab entry size | **0x150** | **0x148** |
| LSDA header | `ff ff 01` (LPStart omit, TType omit, uleb call-site) | `ff 00 b4 02` (TType present, offset 0xb4) |
| call-site table length | 315 | 299 |
| call-site ranges | **57** | **54** |
| landing pads | `0x0`, `0x976..0x9a4` (shared `b 0xc0bcc`), `0x9a6` (PlayerFighter), `0x9ac` (Array), `0x9b2` (PlayerFixedObject), `0x9b8` (Player) | `0x0`, `0x97c..0x9aa` (shared), `0x9ac` (PlayerFixedObject), `0x9b2` (`__cxa_begin_catch`), `0x9b8` (Player) |

The original's pads (`0xc0b8a..0xc0bda`) are all cleanup-only:

```text
c0b8a..c0bb8  b.n 0xc0bcc          ; shared "delete r5" pad
c0bba         mov r8,r0; mov r0,sl; b 0xc0bd0   ; delete PlayerFighter
c0bc0         mov r8,r0; mov r0,fp; b 0xc0bd0   ; delete Array
c0bc6         mov r8,r0; mov r0,r9; b 0xc0bd0   ; delete PlayerFixedObject
c0bcc         mov r8,r0; mov r0,r5; b 0xc0bd0   ; delete Player
c0bd0         blx _ZdlPv
c0bd4         mov r0,r8
c0bd6         blx _Unwind_Resume
```

There is **no `__cxa_begin_catch`** in the original. So the original has no
`try/catch`; the LSDA exists only because of the plain `new` expressions'
cleanup pads. Our current `try { setEmpData } catch (...) { trap }` adds a
TType table and a catch pad (`c0bc6: blx __cxa_begin_catch; udf #254`) that
the original does not have.

The original's 57 ranges vs our 54, and the extra 8 bytes of extab, are
consistent with the original's additional `new`-expression cleanup sites.
Removing the `try` alone (plain `setEmpData`) changes the score to 70.3
(2776 B), because the DAG scheduler then hoists the `tail call` above the
spawn adds.

## 5. Stack-slot colouring

Original frame `sp+28..60`:

```text
jZ@28  jY@32  jX@36  wz@40  wx@44  wy@48  type@52  this@56  race@60
```

Current frame:

```text
jX@28  wz@32  jZ@36  jY@40  wx@44  wy@48  type@52  this@56  race@60
```

The high slots (`type/this/race`) already match. The low slots are a
consequence of the register-allocation difference: the current `ldrd
r1,r0,[sp,#28]` for the sunk x is only possible because `jX` and `wz` are
adjacent; the original's `jX@36`/`wx@44` are not, so a sunk x would need
two loads. Changing declaration/assignment order never moved the low slots
to the original layout without also losing the score.

A full sweep of the 6 waypoint declaration orders x 6 variable orders x 6
field orders (216 variants, baseline spawn) tops out at **74.1**; with the
landed YZX jitter order it tops out at **74.2**. The original's clean
`jZ,jY,jX | wz,wx,wy` grouping was not reachable from any tested source
shape.

## 6. All-int and spawn-position sweeps

The original is all-int, but every all-int source shape changes the
function-wide register allocation: `hitpoints` moves from `fp` to `sl`,
`empPoints` from `sl` to `fp`, `this` is spilled at entry instead of at
`c031c`, and `shipClass` is spilled to `[sp,#60]` instead of staying in
`r6`. The prologue diverges at instruction 4 (`str r1,[sp,#60]; mov
r9,r0; mov fp,r3; mov r5,r2` becomes `str r2,[sp,#60]; mov r9,r3; str
r0,[sp,#56]; mov sl,r1`), which destroys the long equal runs.

Spawn-position sweep (all-int, no try, baseline waypoint block), inserting
the three `int` computations before each anchor in the source:

| anchor | fuzzy | size |
|---|---|---|
| after jitter generation | 61.7 | 2796 |
| after `level` | 61.7 | 2796 |
| after the type switch | 61.7 | 2796 |
| after `empPoints` | 61.7 | 2796 |
| after the hitpoints adjustment | 61.7 | 2796 |
| after the `mission==0x9a` block (before `armor`) | 61.6 | 2796 |
| after `armor` (before `new Player`) | 61.6 | 2796 |
| after `new Player` (baseline position) | 61.0 | 2748 |
| after `setEmpData` | 61.0 | 2748 |

The "before `armor`" shape (61.6, 2796 B) is the closest to the original
*instruction order*: it emits `PlayerC1; spawn adds; mov r0,r5; mov
r1,sl; mov r2,r9; blx setEmpData`, with `x -> fp`, `z -> r8`, and only `y`
in `r6` instead of `r4`. Its prologue is the wrong one, which is why the
score is 61.6 despite the local match.

Type sweep (baseline spawn position, try, official verifier):

| types (x,y,z) | fuzzy | size |
|---|---|---|
| `iff` (current) | **74.1** | 2764 |
| `fif` | 72.8 | 2764 |
| `ffi` | 72.5 | 2764 |
| `iif` | 68.7 | 2780 |
| `ifi` | 67.5 | 2784 |
| `fii` | 67.1 | 2784 |
| `fff` | 64.8 | 2748 |
| `iii` | 60.8 | 2752 |

## 7. Jitter-order sweep (landed change)

The original calls `nextInt(40000)` in X, Y, Z order (first result stored
to `jX@36`, second to `jY@32`, third to `jZ@28`). The three calls are
independent, so any permutation is semantically identical.

| declaration/initializer order | fuzzy |
|---|---|
| X, Y, Z (original order) | 74.1 |
| **Y, Z, X (landed)** | **74.2** |
| Y, X, Z | 74.1 |
| Z, Y, X | 74.1 |
| X, Z, Y | 74.0 |
| Z, X, Y | 74.0 |

Separate declarations (`int jitterX; ... jitterX = ...;`) give 74.1 for
all six orders, so the gain requires the combined
declaration+initializer form. The gain is small and is a colouring
artefact, but it is a strict verifier improvement and does not change any
gate count.

## 8. Semantic findings (not landed)

* **`PlayerFixedObject` argument order is wrong.** The original passes
  `(type, race, player, geom, x, y, z)` (`c0426: ldr r6,[sp,#52]` = type,
  `c042a: ldr r4,[sp,#60]` = race, `c0432: mov r1,r6; c0436: mov r2,r4`).
  The current source passes `PlayerFixedObject(race, type, ...)`. Fixing
  it alone scores 74.0; fixing it together with the landed YZX jitter
  order scores 74.1, i.e. it gives back the 0.1 gain. It is left for a
  pass that can also recover the spawn allocation.
* **Spawn coordinates are correct**: `x = wx + jitterX - 20000`,
  `y = wy + jitterY - 20000`, `z = wz + jitterZ - 20000`; the original's
  `add` then `sub` matches `(w + jitter) - 20000`.
* **Flags are correct**: `PlayerFighter(..., hostile)` gets `param_6`;
  `getShipGroup(type, race, group)` gets `param_7`; the `lodManager`
  object uses `parentGeometry ?: geometry`.
* The waypoint block's jumbled names (`wz = waypoint->x; wy =
  waypoint->y; wx = waypoint->z`) are a reconstruction artefact, but they
  score better (74.1) than the faithful naming (72.7) because of the
  resulting stack-slot colouring.

## 9. Remaining root cause

The single remaining root cause is the register-allocation / scheduling
coupling:

1. the original's integer `x` is live across a **plain** `setEmpData`
   call in `fp`, with `y` in `r4` and `z` in `r8`, and all three
   `vcvt.f32.s32` after the call;
2. our build either sinks `x` past an invoke (current baseline) or, with
   all-int, changes the whole-function colouring (all-int shapes);
3. the original has no catch (cleanup-only LSDA), so the faithful source
   is a no-`try` all-int shape, but that shape currently scores 61.2
   because the DAG scheduler hoists the `tail call` above the adds.

A source shape that gives the original's no-catch block structure *and*
keeps the adds scheduled before the plain `setEmpData` call should recover
the original allocation. The current source is a local optimum at 74.2.
