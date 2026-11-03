# Level::createShip — pass 3 (wave 91, w4106)

**Status:** analysis only, no source change landed (2026-11-03). Worktree
`wt/w4106`, branch `wave91-w4106`, base `a4557300`.
Target: `_ZN5Level10createShipEiiiP8Waypointbb` (original 2764 B @ `0xc0214`,
Thumb entry `0xc0215`). Current verifier score **74.1 %** (osize 2764,
usize 2764, `byte False`).

Gate before and after (unchanged, no code change):

```text
byte_exact 4498/4524  linked_exact 4498/4524  size-exact 4524/4524
stub_zero_size 0  missing 0  wrong_type 0  extra 0  data_exact 843/843
imports 207  lint CLEAN ✓  drift 0  RATCHET PASS
```

This pass continues `level-createship-pass2-w4006.md` (wave 90). It adds:
the exact pre-RA (`Expand ISel Pseudo-instructions`) machine-code evidence
that the **SelectionDAG list scheduler**, not `MachineSink`, emits the
`setEmpData` call first; a minimal reproducer; the LLVM 7.0.2 scheduler
source path that decides the order; and a wider variant sweep. No source
shape found beats 74.1 %.

---

## 1. Original spawn (authoritative disassembly)

`objdump -d` of `android_2.0.16_libgof2hdaa.so`, ELF addresses (image base
`0x10000`; Ghidra address = ELF + `0x10000`):

```text
c03c4  blx  _ZN6PlayerC1Eiiiii          ; new Player(...)
c03c8  ldr  r2,[sp,#48]   ; wy
c03ca  ldr  r3,[sp,#32]   ; jY
c03cc  ldr  r0,[sp,#44]   ; wx
c03ce  ldr  r1,[sp,#36]   ; jX
c03d0  add  r2,r3         ; y = wy + jY
c03d2  ldr  r3,[sp,#40]   ; wz
c03d4  ldr  r4,[sp,#28]   ; jZ
c03d6  add  r0,r1         ; x = wx + jX
c03d8  movw r1,#20000
c03dc  sub.w fp,r0,r1     ; x -> fp
c03e0  add  r3,r4         ; z = wz + jZ
c03e2  subs r4,r2,r1      ; y -> r4
c03e4  sub.w r8,r3,r1     ; z -> r8
c03e8  mov  r0,r5         ; player
c03ea  mov  r1,sl         ; empPoints
c03ec  mov  r2,r9         ; empTime
c03ee  blx  _ZN6Player10setEmpDataEii   ; plain (non-invoke) call
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

So: **all three coordinates are integers in `fp`, `r4`, `r8`; all three
adds/subs are emitted before the plain `blx setEmpData`; all three
`vcvt.f32.s32` are emitted after it and are duplicated in both
`shipClass` branches** (`c040c..c0422` for class 1, `c06c0..c06d8` for
class 0). The original prologue keeps `type` in `fp` and `shipClass` in
`r5`, spilling only `race`:

```text
c0214  push {r4,r5,r6,r7,lr}
c0216  add  r7,sp,#12
c0218  stmdb sp!,{r7,r8,r9,sl,fp}
c021c  vpush {d0-d10}
c0220  str  r1,[sp,#60]   ; race
c0222  mov  r9,r0         ; this
c0224  ldr  r0,[pc,#572]  ; Globals::status GOT
c0226  mov  fp,r3         ; type
c0228  mov  r5,r2         ; shipClass
c022a  add  r0,pc
c022c  ldr  r4,[r0,#0]    ; r4 = &Globals::status
```

## 2. Ghidra decompile (same conclusion)

Headless Ghidra 12.1.2, project `/home/debian/GalaxyOnFire2/GalaxyOnFire2`,
`android_2.0.16_libgof2hdaa.so`, `DecompLong.java`, `createShip @ 0xd0214`:

```c
iVar3 = local_54 + iVar3 + -20000;      /* x int */
iVar1 = iVar12 + iVar4 + -20000;        /* y int */
fVar20 = (float)(iVar2 + iVar5 + -20000);/* z int, converted at the ctor */
Player::setEmpData(this_00, iVar7, iVar13);
```

The source shape is therefore the faithful one:

```cpp
int x = wx + jitterX - 20000;
int y = wy + jitterY - 20000;
int z = wz + jitterZ - 20000;
player->setEmpData(empPoints, empTime);
```

with implicit `int -> float` conversion at each `PlayerFighter` /
`PlayerFixedObject` constructor call. `Player::setEmpData` is `void`
(confirmed by disassembling `_ZN6Player10setEmpDataEii @ 0xa2a34`: it
ends in `blx updateDamageRate; str r4,[r5,#228]; pop`, no meaningful
`r0`), and the caller ignores `r0`.

## 3. The committed baseline

`src/game/world/Level.cpp:5168` (74.1 %):

```cpp
int armor = Globals::status->hardCoreMode() ? 650 : 1000;
Player *player = new Player(armor, hitpoints, 1, 1, 0);
int x = wx + jitterX - 20000;
float z = (float) (wz + jitterZ - 20000);
float y = (float) (wy + jitterY - 20000);
try {
    player->setEmpData(empPoints, empTime);
} catch (...) {
    __builtin_trap(); // lint: arm_builtin
}
```

with placement `new` for `PlayerFighter`/`PlayerFixedObject`. It matches
the original prologue through `c0256`; the first divergence is the `wz`
stack slot (`[sp,#40]` original vs `[sp,#32]` ours) at `c0258`. Its spawn
region is:

```text
c03c4  blx  Player::Player
c03c8  ldr  r0,[sp,#44]   ; wx
c03ca  movw r4,#20000
c03ce  ldr  r1,[sp,#36]   ; jX
c03d0  ldr  r2,[sp,#40]   ; jitterY
c03d2  add  r0,r1         ; x
c03d4  ldr  r1,[sp,#48]   ; wy
c03d6  subs r0,r0,r4      ; x
c03d8  add  r1,r2         ; y
c03da  subs r1,r1,r4      ; y
c03dc  vmov s0,r1
c03e0  vcvt.f32.s32 s18,s0 ; y
c03e4  vmov s0,r0
c03e8  vcvt.f32.s32 s16,s0 ; x
c03ec  mov  r0,r5
c03ee  mov  r1,sl
c03f0  mov  r2,r9
c03f2  blx  setEmpData
c03f6  ldrd r1,r0,[sp,#28]
c03fa  cmp  r6,#0
c03fc  ldr.w sl,[sp,#56]
c0400  add  r0,r1
c0402  sub.w r4,r0,r4      ; z recomputed after the call
```

x and y are converted before the call; z is recomputed after it.

## 4. Root cause: the SelectionDAG (pre-RA) list scheduler

The faithful no-try + plain-`new` shape reproduces the original LSDA
*exactly* (`.ARM.extab 0x150`, LSDA header `ff ff 01`, 315-byte call-site
region, 57 sites, cleanup-only, no TType, no `__cxa_begin_catch`) but
scores 63–65 % because the whole-function register allocation changes.

The adds are already sunk at the **first machine-code dump**, i.e. before
`MachineSink` runs. `-mllvm -print-after-all` on the faithful build
(`Level.cpp` compiled with the exact `compile_commands.json` flags plus
`-mllvm -print-after-all`), dump
`*** IR Dump After Expand ISel Pseudo-instructions ***`, block `bb.17`
(`%ir-block.109`, the block after `new Player`):

```text
bb.17 (%ir-block.109):
; predecessors: %bb.16
  successors: %bb.18(0x30000000), %bb.31(0x50000000)

  ADJCALLSTACKDOWN 0, 0, 14, $noreg, implicit-def dead $sp, implicit $sp
  DBG_VALUE debug-use %26:gpr, ..., !"empPoints"
  DBG_VALUE debug-use %25:gpr, ..., !"empTime"
  $r0 = COPY %31:gpr
  $r1 = COPY %26:gpr
  $r2 = COPY %25:gpr
  tBL 14, $noreg, @_ZN6Player10setEmpDataEii, ...
  ADJCALLSTACKUP 0, 0, 14, $noreg, implicit-def dead $sp, implicit $sp
  DBG_VALUE debug-use %5:gprnopc, ..., !"wz"
  %250:gprnopc = t2ADDrr %5:gprnopc, %8:rgpr
  %251:rgpr = t2MOVi16 20000
  %252:gprnopc = t2SUBrr killed %250:gprnopc, %251:rgpr
  DBG_VALUE debug-use %252:gprnopc, ..., !"z"
  DBG_VALUE debug-use %4:gprnopc, ..., !"wy"
  %253:gprnopc = t2ADDrr %4:gprnopc, %7:rgpr
  %254:gprnopc = t2SUBrr killed %253:gprnopc, %251:rgpr
  DBG_VALUE debug-use %254:gprnopc, ..., !"y"
  DBG_VALUE debug-use %6:gprnopc, ..., !"wx"
  %255:gprnopc = t2ADDrr %6:gprnopc, %9:rgpr
  %256:gprnopc = t2SUBrr killed %255:gprnopc, %251:rgpr
  DBG_VALUE debug-use %256:gprnopc, ..., !"x"
  %34:gpr = COPY %252:gprnopc
  %33:gpr = COPY %254:gprnopc
  %32:gpr = COPY %256:gprnopc
  t2CMPri %132:gprnopc, 0, 14, $noreg, implicit-def $cpsr
  t2Bcc %bb.31, 1, $cpsr
  t2B %bb.18, 14, $noreg
```

The call is first, then the adds in reverse source order (z, y, x). This
is the output of `ScheduleDAGRRList` (`-pre-RA-sched=list-hybrid`, the
`-Oz` default), not of `MachineSink`.

### 4.1 Scheduler source path (LLVM 7.0.2)

`llvm/lib/CodeGen/SelectionDAG/ScheduleDAGRRList.cpp`:

* `RegReductionPQBase::getNodePriority()` returns the **Sethi–Ullman
  number** (`CalcNodeSethiUllmanNumber`, computed over *data* preds only;
  chain preds skipped), with special cases `CopyToReg/TokenFactor -> 0`,
  no-successor -> `0xffff`, no-pred -> `0`.
* `BURRSort()` compares priorities and, when they tie and either node is
  a call, falls back to `getNodeOrdering()` (`SDNode::getIROrder()`).
* `hybrid_ls_rr_sort::operator()` short-circuits to `BURRSort` whenever
  either node `isCall`.
* `ListScheduleBottomUp()` pushes the pick order into `Sequence` and then
  `std::reverse`s it: the **first scheduled node is emitted last**.

The call's operands are `player` (the `new Player` call result, whose own
operand tree contains the whole `hitpoints` float computation),
`empPoints` and `empTime`; the coordinate adds' operands are only two
stack loads and a constant. The call therefore has the higher
Sethi–Ullman number and is emitted first. (Experimentally, changing the
call's operand count 1..5 or the depth of the coordinate expression does
not flip the order — see §6.)

## 5. Minimal reproducer

`/dev/shm/w4106/m/mini.cpp`, compiled with the same target flags
(`-target armv7-none-linux-androideabi16 -march=armv7-a -mthumb
-mfpu=neon -mfloat-abi=softfp -std=c++11 -Oz`):

```cpp
extern void setEmp(int, int);
extern void ctor(int, float, float, float);
int f(int a, int b, int wx, int wy, int wz, int jx, int jy, int jz) {
    int x = wx + jx - 20000;
    int y = wy + jy - 20000;
    int z = wz + jz - 20000;
    setEmp(a, b);
    if (a == 0) ctor(a, x, y, z);
    else ctor(a, x, y, z);
    return x + y + z;
}
```

```text
00000000 <_Z1fiiiiiiii>:
   0: b5f0        push {r4,r5,r6,r7,lr}
   2: af03        add  r7,sp,#12
   4: f84d 8d04   str.w r8,[sp,#-4]!
   8: 461c        mov  r4,r3
   a: 4615        mov  r5,r2
   c: 4680        mov  r8,r0
   e: f7ff fffe   bl   <_Z6setEmpii>   ; <-- call FIRST
  12: f107 0308   add.w r3,r7,#8
  16: 6978        ldr  r0,[r7,#20]
  18: cb0e        ldmia r3,{r1,r2,r3}
  1a: 4408        add  r0,r1
  1c: 18a9        adds r1,r5,r2
  1e: f644 6220   movw r2,#20000
  22: 1a8d        subs r5,r1,r2
  24: 18e1        adds r1,r4,r3
  26: 1a86        subs r6,r0,r2
  28: 1a8c        subs r4,r1,r2
  ...
```

`-mllvm -pre-RA-sched=source` on the same file puts the adds before the
call (source order), confirming the hybrid scheduler is what reorders.

## 6. What does and does not flip the order (mini repro)

All variants keep the same arithmetic and only change source shape. The
column is the emitted order (`C` = call, `A` = coordinate add/sub).

| variant | order |
|---|---|
| baseline (adds, then call) | call first |
| call before adds in source | call first |
| `int e = setEmp_i(...); return e + ...` (call **return value used**) | **adds first** |
| `int e = setEmp_i(...); (void)e;` | call first |
| `int e = setEmp_i(...); return (e & 0) + ...` | call first |
| `int e = setEmp_i(...); return e * 0 + ...` | call first |
| `(void)(x + y + z);` before call | call first |
| `if (x == 0x7fffffff) {}` before call | call first |
| `(void)(x, y, z);` before call | call first |
| `if (x) {}` before call | call first |
| `setEmp((x, y, z, a), b)` | call first |
| `setEmp(a + (x - x), b)` | call first |
| `setEmp(x ? a : a, b)` | call first |
| call with 1..5 operands (constants) | call first |
| coordinate expression made deeper (`wx2 = wx+a; jx2 = jx+b; ...`) | call first |
| `int x = wx + (jx - 20000)` / `(wx - 20000) + jx` / two statements | call first |
| `const int k = 20000;` | call first |
| `int x, y, z;` declared, then assigned | call first |

The only reproducible flip is a **genuine data successor on the call**
(the used return value). `Player::setEmpData` returns `void` in the
original, so this cannot be used without changing the ABI (and the
wave-90 experiment of changing it to `int`/`Player*` lost: 63.1 % /
65.2 %, prologue wrong).

## 7. Real-function variant sweep (official verifier, `autojunk=False`)

All numbers are `score_cs.py` on the built `libgof2hdaa.so`; original
size 2764 B. The committed baseline is **74.1 / 2764**.

### 7.1 no-try + plain `new`

| spawn types (x,y,z) | fuzzy | size |
|---|---|---|
| `iii` | 63.1 | 2788 |
| `iif` | 65.2 | 2772 |
| `ifi` | 61.2 | 2772 |
| `iff` | 64.2 | 2768 |
| `fii` | 60.6 | 2772 |
| `fif` | 63.7 | 2768 |
| `ffi` | 62.6 | 2768 |
| `fff` | 58.4 | 2776 |

Spawn declaration-order sweep (`iii`, after ctor): `xyz` 63.1, `xzy`
62.8, `yxz` 63.1, `yzx` 63.1, `zxy` 62.8, `zyx` 62.7.

### 7.2 no-try + placement `new`

| spawn | fuzzy | size |
|---|---|---|
| `iff` | 70.0 | 2776 |
| committed mixed (`int x`, `float z`, `float y`) | 70.0 | 2776 |
| `iii` | 60.8 | 2748 |

### 7.3 try + placement `new`

| spawn | fuzzy | size |
|---|---|---|
| `iii` | 60.7 | 2752 |

### 7.4 try + plain `new`

| spawn | fuzzy | size |
|---|---|---|
| `iii` | 61.6 | 2792 |
| `iff` | 63.2 | 2772 |

### 7.5 spawn before the `Player` ctor

| spawn / new | fuzzy | size |
|---|---|---|
| `iii` plain | 64.2 | 2796 |
| `iff` plain | 60.2 | 2772 |
| `iii` placement | 61.6 | 2796 |
| `iff` placement | 65.7 | 2788 |

### 7.6 call-first source, `ship` position, expression forms

| variant | fuzzy | size |
|---|---|---|
| call before adds, `iii` | 63.1 | 2788 |
| call before adds, `iff` | 64.2 | 2768 |
| two-statement adds, `iii` | 63.1 | 2788 |
| `const int k = 20000`, `iii` | 63.1 | 2788 |
| `wx - 20000 + jx`, `iii` | 63.1 | 2788 |
| declare `x,y,z` then assign, `iii` | 63.1 | 2788 |
| `ship` before spawn / after spawn / after call, `iii` | 63.1 | 2788 |
| `ship` before spawn / after spawn, `iff` | 64.2 | 2768 |

### 7.7 jitter declaration × assignment sweep (committed baseline)

36 combinations. **The declaration order is irrelevant** (all six rows
identical for a given assignment order). Assignment order decides:

| assignment order | fuzzy |
|---|---|
| `YZX` (committed) | **74.1** |
| `XYZ`, `YXZ`, `ZYX` | 74.0 |
| `XZY`, `ZXY` | 73.9 |

### 7.8 waypoint × jitter sweep (committed baseline)

36 combinations. Best is the committed `wx,wz,wy` declaration with `YZX`
jitter assignment = **74.1**. All other waypoint orders are 72.4–74.0.

## 8. What remains

The blocker is exactly as stated in the wave-90 note, now proven at the
pre-RA machine-code level:

1. The original is all-int with the three adds before a **plain**
   `setEmpData` call and the three `vcvt.f32.s32` after it (duplicated
   in both `shipClass` branches).
2. The faithful no-try + plain-`new` shape reproduces the original LSDA
   exactly but the SelectionDAG list scheduler (`list-hybrid`) emits the
   call first, which changes the whole-function register allocation and
   drops the score to 63–65 %.
3. The committed baseline matches the original prologue but is not
   faithful (`try` + placement `new`, mixed `int`/`float` spawn) and its
   spawn region differs (x/y converted before the call, z recomputed
   after).

To land a strict improvement one needs a no-try + plain-`new` source
shape in which the all-int adds stay before `setEmpData` **and** the
entry allocation stays `race -> [sp,#60]`, `type -> fp`,
`shipClass -> r5`. The only known lever that flips the scheduler is a
genuine data successor on the call, which `void Player::setEmpData`
cannot provide. Remaining untried ideas:

* A source construct that gives the call a data successor without
  changing the emitted call (e.g. a value-returning wrapper that is
  fully inlined and whose result folds away) — but every such fold tried
  in §6 removed the successor and restored call-first.
* A `try`-free shape whose `ship = nullptr` `CopyToReg` stays in the
  same block as the call (the original has `mov r9,#0` before the
  `cmp/beq`; our builds sink it into the fallthrough block). Moving the
  declaration did not change the score.
* Matching the original's `wz`/`jitterY` stack-slot swap
  (`wz -> [sp,#40]`, `jitterY -> [sp,#32]`; ours is the reverse). The
  36-way waypoint × 36-way jitter sweeps found no order that does it
  while keeping the prologue.

Until then the committed baseline (74.1 %) is the best known local
optimum and no code change is justified: `byte_exact` does not rise.

## 9. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w4106
cmake --build cmake-build-match --target link_so -j8
python3 /dev/shm/w4106/score_cs.py          # 74.1, 2764 B, byte False
python3 tools/verify/verify.py \
  --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN5Level10createShipEiiiP8Waypointbb
```

Scratch helpers live in `/dev/shm/w4106` (`score_cs.py`, `mkvariant.py`,
`mini.cpp`, `m/` mini-repro harness, `faithful.pipeline.txt`,
`sched_src.cpp` = LLVM 7.0.2 `ScheduleDAGRRList.cpp`).
