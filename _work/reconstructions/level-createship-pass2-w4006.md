# Level::createShip — pass 2 (wave 90, w4006)

**Status:** analysis only, no source change landed (2026-11-03). Worktree
`wt/w4006`, branch `wave90-w4006`, base `360c7467`.
Target: `_ZN5Level10createShipEiiiP8Waypointbb` (original 2764 B @ `0xc0214`,
original symbol value `0xc0215` Thumb). Current verifier score **74.1 %**
(1007 original insns / 1008 ours).

Gate before and after (unchanged, no code change):

```text
byte_exact 4498/4524  linked_exact 4498/4524  size-exact 4524/4524
stub_zero_size 0  missing 0  wrong_type 0  extra 0  data_exact 843/843
imports 207  .text 0x1337c8  .rodata 0x41b67  .ARM.exidx 0x6f70 exact
.ARM.extab 0x26290 (-8)  lint CLEAN  drift 0  RATCHET PASS
```

The committed source is a local optimum. The faithful source shape (no
`try`, plain `new T(...)`) reproduces the original's LSDA *exactly* but
scores 64–65 % because the register allocator then picks a different
prologue. The committed baseline (mixed spawn types + `try` + placement
`new`) matches the original's prologue and first ~170 instructions exactly,
which is why it scores 74.1 %.

---

## 1. Original spawn: all-int, adds before the plain call, vcvt after

`objdump -d` of the original, `0xc03c4..0xc0426` (image base 0x10000;
the function is at `0xc0214` in the ELF, `0xd0214` in Ghidra):

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

All three coordinates are **integers** computed **before** `setEmpData`;
all three `vcvt.f32.s32` are emitted **after** the call. The conversions
are duplicated in both shipClass paths (`c040c..c0422` for class 1,
`c06c0..c06d8` for class 0). The original prologue is:

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

`/dev/shm/w4006/decomp_cs.txt` (headless Ghidra 12.1.2, base 0x10000,
`DecompLong.java`, `createShip @ 0xd0214`):

```c
iVar1 = Status::hardCoreMode();
this_00 = operator_new(0x114);
iVar14 = 1000;
if (iVar1 != 0) iVar14 = 0x28a;
Player::Player(this_00,iVar14,iVar6,1,1,0);
iVar3 = local_54 + iVar3 + -20000;      /* x int */
iVar1 = iVar12 + iVar4 + -20000;        /* y int */
fVar20 = (float)(iVar2 + iVar5 + -20000);/* z int, converted at the ctor */
Player::setEmpData(this_00,iVar7,iVar13);
this_02 = (PlayerFixedObject *)0x0;
if (param_2 == 0) {
  this_02 = operator_new(0x2f0);
  fVar19 = (float)VectorSignedToFloat(iVar3,...);  /* x */
  fVar20 = (float)VectorSignedToFloat(fVar20,...); /* z */
  VectorSignedToFloat(iVar1,...);                  /* y */
  PlayerFighter::PlayerFighter(this_02,param_3,param_1,this_00,0,
                               fVar19,extraout_s1_00,fVar20,SUB41(fVar19,0));
  ...
}
if (param_2 != 1) goto LAB_000d0b70;
this_02 = operator_new(0x1bc);
VectorSignedToFloat(iVar3,...);   /* x */
VectorSignedToFloat(iVar1,...);   /* y */
VectorSignedToFloat(fVar20,...);  /* z */
PlayerFixedObject::PlayerFixedObject(this_02,param_3,param_1,this_00,0,
                                     fVar20,extraout_s1,extraout_s2);
```

`param_3` is `type`, `param_1` is `race`; both constructors receive
`(type, race, player, geom, x, y, z, ...)`. The `PlayerFixedObject`
`(type, race)` semantic fix is already committed (`94e8bc54`).

## 3. Current committed baseline

The committed source (`src/game/world/Level.cpp:5168`) is:

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

with placement `new` for `PlayerFighter`/`PlayerFixedObject`. Its prologue
is byte-identical to the original through `c0256`; the first divergence is
the `wz` stack slot (`[sp,#40]` original vs `[sp,#32]` ours) at `c0258`,
followed by the jitter stack slots. Its spawn region is:

```text
c03c4  blx  Player::Player
c03c8  ldr  r0,[sp,#44]   ; wx
c03ca  movw r4,#20000
c03ce  ldr  r1,[sp,#36]   ; jX
c03d0  ldr  r2,[sp,#40]   ; wz
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
c03f6  ldrd r1,r0,[sp,#28] ; jZ, wz
c03fa  cmp  r6,#0
c03fc  ldr.w sl,[sp,#56]
c0400  add  r0,r1
c0402  sub.w r4,r0,r4      ; z recomputed after the call
```

So x and y adds are before the call (with their vcvt), z is after. The
original has all three adds before and all three vcvt after. Apart from
the `wz`/jitter stack-slot colouring and the placement of the `this`
spill, the early body matches.

## 4. LSDA: the original is no-try + plain `new`

Parsing `.ARM.exidx`/`.ARM.extab` for `0xc0214` (`/dev/shm/w4006/lsda.py`,
`lsda_detail.py`):

| build | extab size | LSDA header | call-site len | sites | pads |
|---|---|---|---|---|---|
| **original** | **0x150** | **`ff ff 01`** (LPStart omit, TType omit, uleb) | **315** | **57** | cleanup-only, no `__cxa_begin_catch` |
| committed baseline (`try` + placement new) | 0x148 | `ff 00 b4 02` (TType present) | 299 | 54 | includes catch pad `__cxa_begin_catch; udf #254` |
| **no-try + plain `new`** | **0x150** | **`ff ff 01`** | **315** | **57** | cleanup-only |

The no-try + plain-`new` shape reproduces the original EH record *exactly*
(0x150, `ff ff 01`, 315/57, all `actionEntry == 0`, no TType, no
`__cxa_begin_catch`). The original landing pads are:

```text
c0b8a..c0bb8  b.n 0xc0bcc          ; shared "delete r5" pad (25 sites)
c0bba         mov r8,r0; mov r0,sl; b 0xc0bd0   ; delete PlayerFighter
c0bc0         mov r8,r0; mov r0,fp; b 0xc0bd0   ; delete Array
c0bc6         mov r8,r0; mov r0,r9; b 0xc0bd0   ; delete PlayerFixedObject
c0bcc         mov r8,r0; mov r0,r5; b 0xc0bd0   ; delete Player
c0bd0         blx _ZdlPv
c0bd4         mov r0,r8
c0bd6         blx _Unwind_Resume
```

Call-site 1 (`start=0x196 len=0x1e`, i.e. `0xc03aa..0xc03c8`) is the
`new Player` cleanup; call-site 2 (`start=0x1b4 len=0x5e`, i.e.
`0xc03c8..0xc0426`) has `lp=0`, so the spawn adds and `setEmpData` are a
plain, non-invoke region. The original source therefore has **no
try/catch** and uses **plain `new T(...)`** expressions.

## 5. The faithful shape loses the prologue

Replacing the committed baseline's `try`/placement-new with the faithful
no-try + plain-new shape (keeping the same spawn types) gives the exact
original LSDA but a different entry allocation:

```text
faithful no-try + plain-new, all-int spawn, spawn before `armor`:
c0220  str  r3,[sp,#60]   ; type   (original: str r1,[sp,#60] = race)
c0222  mov  r9,r0         ; this
c0224  str  r1,[sp,#56]   ; race
c0226  mov  fp,r2         ; shipClass (original: mov fp,r3 = type)
c0228  ldr  r0,[pc,#568]
c022a  add  r0,pc
c022c  ldr  r6,[r0,#0]    ; r6 = &status (original: r4 = &status)
...
c0262  ldr  r4,[r0,#0]    ; r4 = &rnd    (original: r5 = &rnd)
c0264  ldr  r5,[r1,#0]    ; r5 = fn ptr  (original: r6 = fn ptr)
```

The allocator spills `type` and `race` at entry and keeps `shipClass` in
`fp`, instead of spilling only `race`, keeping `type` in `fp` and
`shipClass` in `r5`. The GOT-pointer registers rotate (`r6/r4/r5` instead
of `r4/r5/r6`). This is a whole-function register-allocation difference,
not a local scheduling one.

## 6. Root cause: scheduler / allocator coupling

The original's all-int adds stay before the plain call, so `x`, `y`, `z`
are live across `setEmpData` in `fp`, `r4`, `r8`; that live set plus
`player` (`r5`), `shipClass` (`r6`), `empPoints` (`sl`) and `empTime`
(`r9`) exactly fills the seven available callee-saved GPRs (`r4,r5,r6,
r8,r9,sl,fp`; `r7` is the frame pointer), which forces the original
allocation.

In our builds the adds do **not** stay before the call:

* With `try`, `setEmpData` is an invoke. The adds are in the pre-invoke
  block; `MachineSink` moves them into the post-invoke block (the common
  dominator of the branch uses). The committed baseline ends up with x/y
  before and z after the call.
* With no-try + plain `new`, the call is a plain `CALL` in the middle of
  the block, and the SelectionDAG scheduler (`list-hybrid`, `-Oz`) sinks
  the int adds past the call to sit next to their `vcvt` uses. The
  `-mllvm -print-after-all` dump shows the adds already after the call at
  `Expand ISel Pseudo-instructions`, before `MachineSink` ever runs.

`-mllvm -pre-RA-sched=source` does put the adds before the call but
re-schedules the whole function (w3907 measured 52.0 %). It is a flag,
not a source shape.

## 7. Variant measurements (official verifier, `autojunk=False`)

All measurements are `score1.py` on the built `libgof2hdaa.so`. The
committed baseline is 74.1.

### 7.1 Spawn types (committed-baseline shape, spawn after the ctor)

| types (x,y,z) | fuzzy | size |
|---|---|---|
| `iff` (committed baseline) | **74.1** | 2764 |
| `fff` | 64.4–65.0 | 2748 |
| `iif` | 68.4–68.5 | 2780–2784 |
| `ifi` | 67.4–67.5 | 2780–2784 |
| `iii` | 60.6–61.1 | 2752 |
| `fii` | 66.9 | 2780 |

`iii` puts the call at instruction 3 after the ctor and all three adds
after it; `fff` puts all three adds and all three vcvt before it; `iff`
puts x/y before and z after. No type combination gives "adds before,
vcvt after".

### 7.2 Faithful no-try + plain-new, all-int spawn

| spawn position | fuzzy | size | prologue |
|---|---|---|---|
| before `armor` | 64.2 | 2796 | `str r3,[sp,#60]; mov r9,r0; str r1,[sp,#56]; mov fp,r2` |
| after `armor` | 64.2 | 2796 | same |
| after ctor | 63.1 | 2788 | `str r3,[sp,#60]; mov r9,r0; str r1,[sp,#56]; mov sl,r2` |
| after `setEmpData` | 63.1 | 2788 | same |

Sweeping all 6 spawn declaration orders and all 6 jitter orders on the
"before `armor`" shape gives 63.8–64.3; the prologue is always
`str r3,[sp,#60]; mov r9,r0; ...`. Sweeping the 6 waypoint declaration
orders × 6 jitter orders gives 63.1–63.4 with the same prologue.

### 7.3 Faithful no-try + plain-new, other spawn types

| types | spawn position | fuzzy | prologue |
|---|---|---|---|
| `fff` | before `armor` | 57.1 | `str r2,[sp,#60]; mov r6,r3; str r0,[sp,#48]; mov sl,r1` |
| `fff` | after ctor | 58.4 | `str r1,[sp,#56]; mov r9,r0; ...; mov sl,r3` |
| `iff` | before `armor` | 60.2 | `strd r1,r2,[sp,#56]; mov r9,r3` |
| `iff` | after ctor | 64.2 | `strd r1,r2,[sp,#56]; mov r9,r0` |
| `iii` | before `armor` | 64.2 | `str r3,[sp,#60]; mov r9,r0; str r1,[sp,#56]; mov fp,r2` |

### 7.4 Committed-baseline tweaks (all ≤ 74.1)

* Jitter order: `xyz` 74.0, `xzy` 73.9, `yxz` 74.0, **`yzx` 74.1**
  (committed), `zxy` 73.9, `zyx` 74.0.
* Spawn declaration order: `xzy`, `zxy`, `zyx` all 74.1; `xyz`, `yxz`,
  `yzx` 73.9.
* Waypoint declaration order: `wx,wz,wy` (committed) 74.1; `wx,wy,wz`
  73.9; `wz,wx,wy` / `wz,wy,wx` 72.9; `wy,wx,wz` / `wy,wz,wx` 72.6.
  The assignment order inside the `if`/`else` does not change the score.
* Moving the spawn inside/after the `try` block: 72.0.
* Catch body: `__builtin_trap()` 74.1 (committed); empty 65.1;
  `throw;` 49.4; `return nullptr` 66.1; `player = nullptr` 66.4;
  `delete player` 64.9; `(void)player` 65.1; `getLevel()` 45.4;
  `__builtin_unreachable()` 66.2. Every catch variant keeps a TType
  table (`ff 00 b4 02`), so none is faithful to the original LSDA.

## 8. What remains

The remaining root cause is the register-allocation / scheduling coupling:

1. The original is **all-int**, with the three adds before a **plain**
   `setEmpData` call and the three `vcvt.f32.s32` after it. The ints are
   live across the call in `fp`, `r4`, `r8`, which forces the original
   prologue (`race` spilled, `type` in `fp`, `shipClass` in `r5`).
2. Our committed baseline matches the prologue but is **not faithful**:
   it has a `try/catch` (TType, 54 sites, 0x148) and placement `new`,
   and its spawn is mixed (`int x`, `float z`, `float y`).
3. The faithful no-try + plain-`new` shape reproduces the original LSDA
   exactly (0x150, `ff ff 01`, 57 sites) but the scheduler sinks the int
   adds past the plain call, which changes the whole-function allocation
   and drops the score to 64–65.

To land a strict improvement, one needs a no-try + plain-`new` source
shape in which the all-int adds stay before `setEmpData` **and** the
entry allocation stays `race→[sp,#60]`, `type→fp`, `shipClass→r5`.
Ideas not yet exhausted:

* A source construct that makes the add results have a use in the
  pre-call block (so `isSafeToMove`/`AllUsesDominatedByBlock` keeps them
  there) without adding instructions the original does not have.
* A different expression form for the coordinate computation
  (`wx + (jitterX - 20000)`, separate add/sub statements, etc.) that
  changes the SelectionDAG scheduling without changing the arithmetic.
* A declaration/initialisation order that flips the allocator's choice
  between `fp` and `r6` for `shipClass`/`x`.

Until then the committed baseline (74.1) is the best known local optimum
and no code change is justified: `byte_exact` does not rise.

## 9. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w4006
cmake --build cmake-build-match --target link_so -j8
python3 /dev/shm/w4006/score1.py                 # 74.1, 2764 B
python3 /dev/shm/w4006/lsda.py                   # extab/LSDA of the built .so
python3 /dev/shm/w4006/lsda_detail.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so 0xc0214
```

Scratch helpers live in `/dev/shm/w4006` (`score1.py`, `variant2.py`,
`sweep2.py`–`sweep5.py`, `typesweep.py`, `possweep.py`, `posweep2.py`,
`wpsweep.py`, `wpsweep2.py`, `jitsweep.py`, `lsda.py`,
`lsda_detail.py`, `diff1.py`, `orig.asm`, `decomp_cs.txt`).
