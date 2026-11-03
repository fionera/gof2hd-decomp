# PlayerEgo::calcCollision — pass 8 (w4413): the `operator!=` argument order,
# and why the anchor both fixes and breaks the allocation

**Status:** research note, 2026-11-03, wave 94. Worktree `wt/w4413`, branch
`wave94-w4413`, base master `f262bd4b`. **No source change landed.** The
committed shape (one artificial `objects->size() <= i` anchor after
`outerCollide`, no final `i == 0` guard) is still the best at
**87.3 % / 968 B**.

Target: `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE` (orig 968 B,
347 instructions; ours 345). Gate unchanged: `byte_exact 4499/4524`,
`linked_exact 4499/4524`, size-exact 4524, data 843/843, imports 207,
lint CLEAN, drift 0, `.text 0x1337c8`, `.rodata 0x41b67`,
`.ARM.exidx 0x6f70`, `.ARM.extab 0x26290 (−8)`.

This pass adds three things the earlier passes did not have:

1. **A real source-fidelity defect in the committed source**: the collision
   block's vector comparison is written `Vector{0,0,0} != collision`, but the
   binary calls `operator!=(collision, zero)`. The faithful form is
   `collision != Vector{0.0f, 0.0f, 0.0f}`. It costs 0.3 points (87.3 → 87.0)
   and is therefore *not* landed (see §6).
2. **The RAGreedy mechanism**, read off the LLVM 7.0.0 source: the queue is
   ordered by **live-range size**, not spill weight; eviction is by **spill
   weight**. `dockOffsetVec`'s address is dequeued before `i` and takes `r9`;
   the anchor gives `i` a hot use, which raises `i`'s weight above `dock`'s,
   so `i` evicts `dock` from `r9` and `dock` falls to `fp`.
3. **A correction to pass 7's "85.5 %" data point.** That figure is *two*
   anchors (the original one after `outerCollide` **plus** a second one inside
   the `getType()==16994` block), not one relocated anchor. It is the only
   shape measured so far with the **fully correct** register file
   (`this=r5, objects=r4, object=r8, dock=r9, i=fp, surface=r6, canary=[sp,#24]`),
   but it is 8 bytes long and scores 85.5. A *single* anchor inside the
   `getType` block scores 63.0 (`this=r9, dock=r5`).

---

## 1. Ground truth: the original body (Ghidra, inlined)

Same body as pass 5/6/7 §1 (headless Ghidra 12.1.2, image base 0x10000):

```c
void __thiscall PlayerEgo::calcCollision(PlayerEgo *this,Array *param_1)
{
  float fVar1; float fVar2; int *piVar3; Vector *this_00; undefined4 *puVar4;
  int *piVar5; int iVar6; float fVar7; Matrix *pMVar8; Matrix *pMVar9;
  KIPlayer *this_01; uint uVar10; undefined4 *puVar11; uint uVar12;
  uint in_fpscr; float extraout_s0; float extraout_s0_00; float fVar13;
  float extraout_s1; float extraout_s2;
  undefined8 local_88; undefined4 local_80; undefined4 local_48;
  undefined4 uStack_44; undefined4 local_40; int local_3c;

  fVar2 = DAT_000af47c;  fVar1 = DAT_000af470;
  piVar3 = *(int **)(DAT_000af480 + 0xaf0e6);
  local_3c = *piVar3;
  if ((param_1 != (Array *)0x0) &&
     ((this[0x356] == (PlayerEgo)0x0 || (2 < *(int *)(this + 0x1c4) - 1U)))) {
    this_00 = (Vector *)(this + 0xec);
    puVar4 = *(undefined4 **)(DAT_000af484 + 0xaf11e);
    piVar5 = *(int **)(DAT_000af488 + 0xaf132);
    for (uVar12 = 0; uVar12 < *(uint *)param_1; uVar12 = uVar12 + 1) {
      this_01 = *(KIPlayer **)(*(int *)(param_1 + 4) + uVar12 * 4);
      if (this_01 != (KIPlayer *)0x0) {
        if ((uVar12 == 0) && (iVar6 = Status::inAlienOrbit((Status *)*puVar4), iVar6 == 0)) {
          fVar7 = (float)AbyssEngine::AEMath::VectorLength((Vector *)(this + 0x148));
          in_fpscr = in_fpscr & 0xfffffff;
          if ((fVar7 < fVar1) && (this_01[0x71] != (KIPlayer)0x0)) {
            this[0x234] = (PlayerEgo)0x1;
          }
        }
        AEGeometry::getPosition();
        iVar6 = (**(code **)(*(int *)this_01 + 0x40))(this_01,(Vector *)&local_88);
        if (iVar6 != 0) {
          iVar6 = KIPlayer::getType(this_01);
          if ((iVar6 == 0x4262) && (iVar6 = KIPlayer::isVisible(this_01), iVar6 != 0)) {
            iVar6 = PlayerWormHole::isShrinking((PlayerWormHole *)this_01);
            if ((iVar6 == 0) && (*(int *)(this + 0x1e4) == 0)) {
              (**(code **)(*(int *)this_01 + 0x28))((Vector *)&local_88,this_01);
              AbyssEngine::AEMath::Vector::operator=(this_00,(Vector *)&local_88);
              AEGeometry::getPosition();
              AbyssEngine::AEMath::Vector::operator-=(this_00,(Vector *)&local_88);
              fVar7 = (float)AbyssEngine::AEMath::VectorLength(this_00);
              uVar10 = 40000 - (int)fVar7;
              iVar6 = FModSound::isPlaying(*piVar5);
              if ((int)uVar10 < 1) {
                if (iVar6 != 0) { FModSound::stop((FModSound *)*piVar5,0x22); }
              }
              else {
                if (iVar6 == 0) {
                  FModSound::play((FModSound *)*piVar5,0x22,(Vector *)0x0,(Vector *)0x0,extraout_s0);
                }
                else {
                  local_88 = 0; local_80 = 0;
                  (**(code **)(*(int *)this_01 + 0x28))(&local_48,this_01);
                  FModSound::updateEvent3DAttributes
                            ((FModSound *)*piVar5,0x22,(Vector *)&local_48,(Vector *)&local_88,false);
                }
                AbyssEngine::AEMath::VectorNormalize((AEMath *)&local_88,this_00);
                AbyssEngine::AEMath::Vector::operator=(this_00,(Vector *)&local_88);
                local_88 = *(undefined8 *)this_00;
                local_80 = *(undefined4 *)(this + 0xf4);
                AbyssEngine::AEMath::Vector::operator=((Vector *)(this + 0xe0),(Vector *)&local_88);
                fVar13 = (float)VectorSignedToFloat(uVar10 >> 8,(byte)(in_fpscr >> 0x16) & 3);
                AbyssEngine::AEMath::Vector::operator*=((Vector *)(this + 0xe0),fVar13);
                AEGeometry::translate(*(Vector **)(this + 8));
                hitCamera(this);
                if ((int)fVar7 < 1000) { this[0x25] = (PlayerEgo)0x1; }
                puVar11 = *(undefined4 **)(DAT_000af48c + 0xaf41e);
                pMVar8 = (Matrix *)AbyssEngine::PaintCanvas::TransformGetLocal
                                   ((PaintCanvas *)*puVar11,*(uint *)(*(int *)(this + 8) + 0xc));
                pMVar9 = (Matrix *)AbyssEngine::PaintCanvas::TransformGetLocal
                                   ((PaintCanvas *)*puVar11,*(uint *)(*(int *)(this + 4) + 0xc));
                AbyssEngine::AEMath::operator*((AEMath *)&local_88,pMVar8,pMVar9);
                AbyssEngine::AEMath::Matrix::operator=((Matrix *)(*(int *)this + 4),(Matrix *)&local_88);
              }
            }
          }
          else if (this_01[0x3c] == (KIPlayer)0x0) {
            iVar6 = KIPlayer::isVisible(this_01);
            if (iVar6 != 0) {
              AEGeometry::getPosition();
              (**(code **)(*(int *)this_01 + 0x58))((Vector *)&local_48,this_01,(Vector *)&local_88);
              local_88 = 0; local_80 = 0;
              iVar6 = AbyssEngine::AEMath::operator!=((Vector *)&local_48,(Vector *)&local_88);
              if (iVar6 != 0) {
                AEGeometry::setPosition(*(Vector **)(this + 8));
                puVar11 = *(undefined4 **)(DAT_000af490 + 0xaf342);
                pMVar8 = (Matrix *)AbyssEngine::PaintCanvas::TransformGetLocal
                                   ((PaintCanvas *)*puVar11,*(uint *)(*(int *)(this + 8) + 0xc));
                pMVar9 = (Matrix *)AbyssEngine::PaintCanvas::TransformGetLocal
                                   ((PaintCanvas *)*puVar11,*(uint *)(*(int *)(this + 4) + 0xc));
                AbyssEngine::AEMath::operator*((AEMath *)&local_88,pMVar8,pMVar9);
                AbyssEngine::AEMath::Matrix::operator=((Matrix *)(*(int *)this + 4),(Matrix *)&local_88);
                if ((uVar12 == 0) && (this_01[0x71] != (KIPlayer)0x0)) {
                  this[0x234] = (PlayerEgo)0x1;
                }
              }
LAB_000af386:
              hitCamera(this);
            }
          }
          else {
            iVar6 = KIPlayer::isDying(this_01);
            if (((iVar6 == 0) && (iVar6 = KIPlayer::isDead(this_01), iVar6 == 0)) &&
               (((this[0x1c0] == (PlayerEgo)0x0 && (this[0x356] == (PlayerEgo)0x0)) ||
                (this_01 != *(KIPlayer **)(this + 0x1bc))))) {
              AEGeometry::getPosition();
              (**(code **)(*(int *)this_01 + 0x50))((Vector *)&local_88,this_01,(Vector *)&local_48);
              local_48 = 0; uStack_44 = 0; local_40 = 0;
              iVar6 = AbyssEngine::AEMath::operator!=((Vector *)&local_88,(Vector *)&local_48);
              if (iVar6 != 0) {
                fVar7 = (float)Player::setHitVector(*(Player **)(this_01 + 4),extraout_s0_00,
                                                    extraout_s1,extraout_s2);
                Player::setBombForce(*(Player **)(this_01 + 4),fVar7);
                Player::damage(*(Player **)(this_01 + 4),9999);
                Player::damage(*(Player **)this,0x14);
                if (this[0x398] != (PlayerEgo)0x0) {
                  *(float *)(*(int *)this + 0x60) = *(float *)(*(int *)this + 0x60) + fVar2;
                }
                goto LAB_000af386;
              }
            }
          }
        }
      }
    }
  }
  if (*piVar3 != local_3c) { __stack_chk_fail(); }
  return;
}
```

There is no `objects->size()` check inside the loop body — the only size load
is the loop-header test at `9f134`. The committed anchor is artificial. The
final guard is real:

```
9f374: f1bb 0f00  cmp.w  fp, #0
9f378: d105       bne.n  9f386          ; i != 0 -> skip
9f37a: f898 0071  ldrb.w r0, [r8, #113] ; object->field_0x71
9f37e: b110       cbz    r0, 9f386
9f380: 2001       movs   r0, #1
9f382: f885 0234  strb.w r0, [r5, #564]
```

---

## 2. NEW: the collision-block `operator!=` argument order is wrong in our source

`AbyssEngine::AEMath::operator!=(const Vector &, const Vector &)` is a free
function, so the two references arrive in `r0` and `r1` in source order. The
two call sites in the binary disagree with our source in one place:

**Impact block (matches our source).** Original `9f294`–`9f2a0`:

```
9f294: 2000        movs   r0, #0
9f296: 4669        mov    r1, sl          ; sl = sp+96
9f298: e9cd 0018   strd   r0, r0, [sp,#96] ; zero vector at sp+96
9f29c: 901a        str    r0, [sp,#104]
9f29e: 4630        mov    r0, r6          ; r6 = sp+32 = &impact
9f2a0: f7c3 ebf6   blx    operator!=
```

`r0 = &impact`, `r1 = &zero` → `impact != Vector{0,0,0}`. Our source has
`if (impact != Vector{0.0f, 0.0f, 0.0f})` — **correct**.

**Collision block (does NOT match our source).** Original `9f31c`–`9f32c`:

```
9f31c: 2000        movs   r0, #0
9f31e: e9cd 0008   strd   r0, r0, [sp,#32] ; zero vector at sp+32
9f322: 900a        str    r0, [sp,#40]
9f324: 4630        mov    r0, r6          ; r6 = sp+96 = &collision
9f326: 462e        mov    r6, r5          ; restore r6 = &surface (sp+32)
9f328: 46a8        mov    r5, sl          ; restore r5 = this
9f32a: 4631        mov    r1, r6          ; r1 = &zero (sp+32)
9f32c: f7c3 ebb0   blx    operator!=
```

`r0 = &collision`, `r1 = &zero` → `collision != Vector{0,0,0}`.

Our committed build instead emits `r0 = &zero (sp+32)`, `r1 = &collision
(sp+96)`:

```
256: 2000        movs   r0, #0
258: 4689        mov    r1, sl          ; sl = sp+96 = &collision
25a: e9cd 0008   strd   r0, r0, [sp,#32]
25e: 900a        str    r0, [sp,#40]
260: 4630        mov    r0, r6          ; r6 = sp+32 = &zero
262: f7ff fffe   blx    operator!=
```

So the committed source line

```cpp
if (Vector{0.0f, 0.0f, 0.0f} != collision) {
```

is a **source-fidelity defect**. The original is the same shape as the impact
block:

```cpp
if (collision != Vector{0.0f, 0.0f, 0.0f}) {
```

Measured (official verifier, `autojunk=False`): `committed` 87.3 →
`committed + cmp` **87.0** (968 B, same RA). It is a fidelity fix that costs
0.3 points, so it is not landed here (§6).

---

## 3. NEW: how RAGreedy actually orders and evicts (LLVM 7.0.0 source)

Read from `/home/debian/llvm7build/llvm-7.0.0.src/lib/CodeGen/RegAllocGreedy.cpp`:

* `RAGreedy::enqueue` (line 680): the priority queue is keyed on
  **`Prio = (1u << 29) + LI->getSize()`** for global ranges (line 733). It is
  *not* keyed on spill weight. Longest live range is dequeued first.
* `RAGreedy::shouldEvict` (line 897): a queued vreg may evict an interfering
  one iff **`A.weight > B.weight`**, where `LiveInterval::weight` is
  `sum over defs/uses of blockFreq/entryFreq`
  (`LiveIntervals::getSpillWeight`, LiveIntervals.cpp:860).
* `RAGreedy::tryRegionSplit` compares `calcSpillCost()` (entry-block freq +
  exit-block freq = **2.0**) against `calculateRegionSplitCost`, whose
  `StaticCost` is `sum over use-blocks of (#spill insns × blockFreq)`
  (`addSplitConstraints`, line 1234) plus `calcGlobalSplitCost` (line 1608).
  Split wins iff the region cost is **< 2.0**.

Applied to this function (NDK `-Oz` MIR, `lrm_c.txt` / `lrm_v1.txt`):

| value | vreg (committed) | vreg (faithful) | first def | note |
|---|---|---|---|---|
| `&__stack_chk_guard` | `%12` | `%12` | 64B | longest range → dequeued first |
| `this` | `%9` | `%9` | 32B | |
| `objects` | `%10` | `%10` | 16B | |
| `dockOffsetVec` addr | **`%0`** | **`%0`** | **384B** | dequeued before `i` |
| `&surface` (stack.8) | `%34` | `%34` | 544B | |
| `&collision` (stack.5) | `%75` | `%74` | 640B | |
| `i` | **`%175`** | **`%174`** | **720B** | dequeued after `dock` |

`dock`'s live range starts 336 slots before `i`'s, so `dock` is dequeued first
and takes `r9` (the first free callee-saved register after `r4/r5/r6/r8`).
That is exactly the original's assignment (`dock=r9, i=fp`).

The artificial anchor adds a use of `i` in `bb.12` (`%ir-block.83`, a 50/50
branch right after `outerCollide`):

```
1744B  %38:gprnopc = t2LDRi12 %10:gprnopc, 0   ; objects->size
1760B  t2CMPrr %38:gprnopc, %175:rgpr          ; i
1776B  t2Bcc %bb.42, 9                         ; bls -> continue
```

That extra hot use raises `%175`'s spill weight above `%0`'s, so `i` **evicts**
`dock` from `r9`; `dock` falls to `fp` (`i` then takes `r9`). The same
perturbation is what makes the canary's `$r6` region split cost exceed 2.0, so
the canary spills. **The anchor is load-bearing for both halves of the
remaining diff** — which is why no anchor-free shape has yet produced
`dock=r9 / i=fp / canary=[sp,#24]` simultaneously.

The faithful shape already has the right `dock/i` pair; its only defect is the
canary, which region-splits to `$r6` (cost 1.448 = 2 × 0.724) instead of
spilling (cost 2.0), and in doing so pushes `this` to `sl` and `surface` to
`r5`.

---

## 4. NEW: the "85.5 %" data point is TWO anchors, and it has the exact RA

Pass 7 §4.3 recorded "anchor placed after `getType`/`isVisible` with no guard =
85.5 % / 976 B, correct `dock=r9 / i=fp`". Re-measured this pass, that number
is produced by **keeping** the original `outerCollide` anchor **and adding a
second one** inside the `getType()==16994` block:

```cpp
if (object->getType() == 16994 && object->isVisible()) {
    if (objects->size() <= i)          // <-- second anchor
        continue;
    if (((PlayerWormHole *) object)->isShrinking() || this->miningGame != 0)
        continue;
```

That shape is the **only** one measured so far with the *fully correct*
register file:

```
this=r5  objects=r4  object=r8  dock=r9  i=fp  surface=r6  canary=[sp,#24]
```

but it is 348 instructions (976 B) and scores **85.5** (85.2 with the §2 cmp
fix; 85.7 with the real guard added as well). The extra anchor costs more than
the dock/i swap is worth.

A *single* anchor relocated into the `getType` block (outerCollide anchor
removed) is **63.0 %** with `this=r9, dock=r5` — i.e. the pass-7 note's
description of that data point was inaccurate.

---

## 5. Variant table (official `verify.score`, this pass)

Base = committed 87.3 / 968 B / `this=r5 dock=fp i=r9 surf=r6 canary=sp24`.
"faithful" = anchor removed + real `i == 0` guard + §2 cmp fix.

### 5.1 Faithful-shape perturbations (all worse)

| variant | pct | size | RA (this/dock/i/surf/canary) |
|---|---:|---:|---|
| faithful | 60.1 | 996 | sl/r9/fp/r5/? |
| faithful, `const Vector surface` | 60.1 | 996 | sl/r9/fp/r5/? |
| faithful, `const Vector impact` / `collision` / all | 60.1 | 996 | sl/r9/fp/r5/? |
| faithful, surface block unscoped (no `{}`) | 63.8 | 968 | r5/sl/fp/r6/? |
| faithful, `i == 0u` / `0 == i` / `stealFlagByte != 0` | 60.1 | 996 | sl/r9/fp/r5/? |
| faithful, `!i` | 62.7 | 940 | r5/r0/r9/r3/? |
| faithful, impact/collision copy-ctor form | 60.1 | 996 | sl/r9/fp/r5/? |
| faithful, `Vector zero` named local | 60.1 | 996 | sl/r9/fp/r5/? |
| faithful, `Vector surface;` at loop top, assign in place | 58.9 | 984 | r5/r0/sl/r6/sp24 |
| faithful, `Vector collision;` at loop top | 63.4 | 1000 | r5/fp/sl/r6/sp24 |
| faithful, `Vector impact;` at loop top | 63.7 | 996 | r5/r0/sl/r6/sp24 |
| faithful, both at loop top | 62.9 | 1024 | r5/fp/sl/r6/sp24 |
| faithful, `surface`/`collision`/`impact` assignment form | 58.8 / 51.9 / 46.7 | 1004 / 1020 / 996 | — |
| faithful, named `Matrix *m1/*m2` in the collision block | **69.7** | 980 | r5/r0/sl/r6/sp24 |
| faithful, named `PlayerWormHole *` | 60.1 | 996 | sl/r9/fp/r5/? |
| faithful, `KIPlayer *const object` | 60.1 | 996 | sl/r9/fp/r5/? |
| faithful + cold `objects->size() < i + 1` before collision | 57.2 | 1016 | fp/r6/r9/r5/sp28 |
| faithful + cold `i >= objects->size()` | 57.3 | 1012 | fp/r6/r9/r5/sp28 |
| faithful + `outerCollide(getPosition())` (no named surface) | 60.1 | 996 | sl/r9/fp/r5/? |

### 5.2 Anchor shapes (the only ones that spill the canary)

| variant | pct | size | RA |
|---|---:|---:|---|
| committed (1 anchor after `outerCollide`, no guard) | **87.3** | 968 | r5/fp/r9/r6/sp24 |
| committed + §2 cmp fix | 87.0 | 968 | r5/fp/r9/r6/sp24 |
| 2 anchors (after `outerCollide` + inside `getType`), no guard | 85.5 | 976 | **r5/r9/fp/r6/sp24** |
| 2 anchors + §2 cmp fix | 85.2 | 976 | **r5/r9/fp/r6/sp24** |
| 2 anchors + real guard | 85.7 | 984 | **r5/r9/fp/r6/sp24** |
| 2 anchors + real guard + §2 cmp fix | 85.4 | 984 | **r5/r9/fp/r6/sp24** |
| single anchor moved inside `getType` (no outerCollide anchor) | 63.0 | 988 | r9/r5/fp/r6/sp24 |

Anchor *position* around the `outerCollide` region is irrelevant (pass 7
measured that placing it after the null check / top block / `outerCollide` /
`isVisible` / wormhole / impact all give the identical 87.3 and the identical
RA); only *inside* the `getType` block does it change the allocation.

---

## 6. Why the §2 cmp fix is not landed

`collision != Vector{0.0f, 0.0f, 0.0f}` is provably the original source form
(§2), but it measures 87.0 against the committed 87.3 while leaving the gate
counts identical (calcCollision is not byte-exact either way, so
`byte_exact`/`linked_exact` stay 4499). It is a *fidelity* improvement, not a
score improvement, so this pass lands the note only. It should be landed
together with whatever shape finally fixes the register allocation, so the
history does not carry a 0.3-point dip on its own.

---

## 7. What this pass proved / what remains

**Proved:**

1. The collision-block comparison in our source has its operands in the wrong
   order. The binary calls `operator!=(&collision, &zero)`; we emit
   `operator!=(&zero, &collision)`. The impact block is already correct.
2. RAGreedy's queue is ordered by live-range **size**, eviction by spill
   **weight**. `dockOffsetVec`'s address (def at 384B) is dequeued before `i`
   (def at 720B), so the anchor-free shape correctly gives `dock=r9, i=fp`.
   The anchor's hot use of `i` raises `i`'s weight enough to evict `dock`
   from `r9`, which is exactly the committed `dock=fp, i=r9` swap.
3. The "85.5 %" shape from pass 7 is two anchors, and it is the only shape
   measured so far with the complete original register file
   (`r5/r9/fp/r6/sp24`). It is 8 bytes long, so it scores 85.5.
4. Every anchor-free shape that spills the canary also loses `dock=r9`
   (dock lands in `r0`, `fp` or `sl`), and every shape that keeps `dock=r9`
   without the anchor leaves the canary in `r6`.

**Remaining (the win condition):** a shape in which the canary's `$r6`
region-split cost exceeds 2.0 **without** adding a hot use of `i`. The lever
is interference in the loop body, not source control flow: the canary must be
unable to find a cheap `$r6` region while `dock` keeps `r9`. Candidate levers
not yet exhausted: a `Vector`/pointer local whose live range straddles a call
in the *wormhole* or *impact* block (the collision block was the only one
swept, and naming anything there hoists `movw r6,#16994` into `r6`); and a
`Vector surface` lifetime that starts before the `i == 0` block but whose
stack slot is still reusable for `velocity`/`zero` (the 112-byte frame proves
`surface` is dead before those uses, so a plain scope change is ruled out).

---

## 8. Reproduction

```sh
# gate
cmake --build cmake-build-match --target check -j8        # RATCHET PASS

# one-function score / RA (scratch harness, /dev/shm/w4413)
python3 /dev/shm/w4413/fastscore.py     # pct / sizes / insn counts
python3 /dev/shm/w4413/sig.py           # this/dock/i/surf/canary

# official side-by-side
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE

# NDK -Oz MIR (vreg -> value map used in §3)
cd cmake-build-match
CMD=$(python3 -c "import json;print([e['command'] for e in json.load(open('compile_commands.json')) if 'PlayerEgo.cpp' in e['file']][0])")
CMD=${CMD/ -o CMakeFiles\/gof2.dir\/src\/game\/ship\/PlayerEgo.cpp.obj / -mllvm -print-after-all -o \/dev\/shm\/w4413\/ndk.o }
eval "$CMD" > /dev/shm/w4413/ndk_pa.log 2>&1
```

RAGreedy source references (local LLVM 7.0.0 tree):
`/home/debian/llvm7build/llvm-7.0.0.src/lib/CodeGen/RegAllocGreedy.cpp`
lines 680–760 (`enqueue`), 897–915 (`shouldEvict`), 1234–1290
(`addSplitConstraints`), 1608–1700 (`calcGlobalSplitCost`), 1897–1990
(`calculateRegionSplitCost`); `lib/CodeGen/LiveIntervals.cpp` 854–866
(`getSpillWeight`).
