# PlayerEgo::calcCollision — pass 6 (w4230): the fp/r9 tie is a vreg-number
# tie-break, and the faithful shape's `this = sl` is the canary's fault

**Status:** research note, 2026-11-03. Worktree `wt/w4230`, branch
`wave92-w4230`, base `b379d00d`. **No source change landed.** The committed
shape (one artificial `objects->size() <= i` anchor after `outerCollide`, no
final `i == 0` guard) is still the best at **87.3 % / 968 B**.

Target: `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE` (968 B, 87.3 %,
347 original instructions vs 345 ours).

This pass was asked to *enumerate the remaining diff bytes and find a
faithful source shape that makes the allocator choose the original
`dock = r9` / impact `sl` registers without the anchor pair.* The answer is
that the swap is a **virtual-register-number tie-break** between two live
intervals of *identical* size, and the faithful shape's blocker is not the
impact scratch at all — it is `&__stack_chk_guard` taking `r6`, which pushes
`this` out of `r5` into `sl`. Every faithful shape tested trades one correct
pair for another. The anchor is load-bearing for `this = r5 / surface = r6 /
object = r8`, and it is exactly what makes `i` win the tie.

---

## 1. Ground truth: the original body (Ghidra, inlined)

Produced with headless Ghidra 12.1.2 on
`android_2.0.16_libgof2hdaa.so_ghidra` (`DecompCalc.java`, `-noanalysis`).
Inlined verbatim (same body as pass 5 §1):

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

There is no `objects->size()` check inside the loop body — the only size
load is the loop-header test at `9f134`. The committed anchor is artificial.

---

## 2. The remaining diff, enumerated exactly

Official verifier (`verify.score`, `autojunk=False`) on the committed build:

```
orig insns 347   ours 345   score 87.3
equal 302
replace blocks 33   (45 original lines / 43 ours)
insert blocks  3    (anchor 3 insns; a `str.w fp,[sp,#20]`; one `mov r1,sl`)
delete blocks  3    (preheader `str.w r9,[sp,#20]`; final `cmp.w fp,#0; bne.n`)
```

The 33 replace blocks collapse to **five conceptual differences**:

| # | diff | original | committed | # replace blocks |
|---|---|---|---|---|
| 1 | `dock`/`i` register swap | `add.w r9,r5,#328`; `mov.w fp,#0` | `add.w fp,r5,#328`; `mov.w r9,#0` | 22 |
| 2 | preheader dock spill | `str.w r9,[sp,#20]` at `9f12a` | — (spilled at the use sites instead) | 2 |
| 3 | artificial anchor | — | `ldr r0,[r4,#0]; cmp r0,r9; bls.w latch` at `9f190` | 1 insert |
| 4 | real final guard | `cmp.w fp,#0; bne.n 9f386` at `9f374` | — | 1 delete |
| 5 | collision-block shuffle | `mov sl,r5; mov r5,r6; add r6,sp,#96; … mov r6,r5; mov r5,sl; mov r1,r6` | `add.w sl,sp,#96; … mov r1,sl` | 1 |

The other replace blocks are `loc_<offset>` branch-target shifts caused by
the anchor (+12 B) and the missing preheader spill (−4 B). The literal pools
are both five GOT words at `9f480..9f490`; only link-dependent GOT deltas
differ and the verifier masks them.

Post-register-allocation table (matched from the preheader
`add.w X,r5,#328`, the `mov.w Y,#0` counter, the `add Z,sp,#32` surface temp
and the `add.w W,sp,#96` impact temp):

| value | original | committed | faithful scoped | faithful unscoped |
|---|---|---|---|---|
| `this` | r5 | r5 | **sl** | r5 |
| `objects` | r4 | r4 | r4 | r4 |
| `object` | r8 | r8 | r8 | **r9** |
| `dockOffsetVec` | **r9** | fp | r9 | **sl** |
| `i` | **fp** | r9 | fp | fp |
| `surface` temp (sp+32) | r6 | r6 | **r5** | r6 |
| impact temp (sp+96) | **sl** | sl | r9 | r9 |
| collision temp (sp+96) | r6 | sl | **r6** | r6 |
| `&__stack_chk_guard` | `[sp,#24]` | `[sp,#24]` | **r6** | `[sp,#24]` |

So the committed build has *one* wrong pair (dock/i) and everything else
right. The faithful scoped shape has the *right* dock/i pair and the right
collision temp, but `this`/`surface`/canary are permuted and the loop
rotates.

---

## 3. New evidence: the fp/r9 choice is a vreg-number tie-break

Pass 3/4 established the *spill-weight* mechanism (`pull` vs `dock`). This
pass adds the missing half: **why the tie is even close enough to be decided
by an eviction.** The two values have *identical* live-interval size.

### 3.1 MIR live-interval measurement

Extracted from `-mllvm -print-after-all` dumps of the committed source
(`paall_anchor.log`, "IR Dump After Live Register Matrix" at line 3065218)
and of the faithful shape (`paall_nested.log`, line 3064994). A script
parsed the MIR CFG and ran standard backward liveness
(`livein = use ∪ (liveout − def)`):

```
committed:  %0 (dock) def 384B, use 1184B; live-in to every loop block
            %175 (i)   def 720B, uses 784/864/944/1760B, latch 8320B
            size(%0) = 451 blocks-worth, size(%175) = 451   <-- exact tie
faithful:   %0 (dock) size 451, %174 (i) size 451          <-- exact tie
```

Both values are loop-invariant/loop-carried and live across the entire loop
body, so their interval lengths are identical to the block. `RAGreedy::
enqueue` (LLVM 7.0.0, `lib/CodeGen/RegAllocGreedy.cpp:678`) is:

```cpp
  const unsigned Size = LI->getSize();
  ...
  Prio = (1u << 29) + Size;                 // global ranges
  Prio |= (1u << 31);
  if (VRM->hasKnownPreference(Reg))
    Prio |= (1u << 30);
  CurQueue.push(std::make_pair(Prio, ~Reg));   // max-heap; tie -> lower vreg first
```

Equal `Size` ⇒ the max-heap tie-break is `~Reg`, i.e. **the lower virtual
register number is dequeued first**. That is the entire fp/r9 decision.

### 3.2 The greedy pass renumbers the dock vreg above `i`

The LRM dump (state immediately before greedy) has `dock = %0`, `i = %175`.
The `-print-after=greedy` dump of the *same* function has `dock = %189`
(committed) / `%245` (faithful), while `i` keeps `%175` / `%174`:

```
lrm_cc.txt:64      384B  %0:gprnopc   = t2ADDri %9:gprnopc, 328
gra_cc.txt:73      384B  %189:gprnopc = t2ADDri %9:gprnopc, 328
gra_cc.txt:83      720B  %175:rgpr    = t2MOVi 0
lrm_nested.txt:64  384B  %0:gprnopc   = t2ADDri %9:gprnopc, 328
gra_nested.txt:72  384B  %245:gprnopc = t2ADDri %9:gprnopc, 328
gra_nested.txt:81  736B  %174:rgpr    = t2MOVi 0
```

So at the moment the eviction decision is made, `i` has the *lower* number
and wins the tie. The renumbering is a consequence of the greedy pass
splitting/spilling the dock interval (the renumbered vreg owns the def and
the `%stack.19` spill/reload chain); it is not something the source can
address directly. **This is why every "hoist `Vector &dock`" attempt is
neutral**: it changes the IR order but not the post-split numbering.

### 3.3 The actual eviction sequence (pass 3, re-confirmed)

Committed (anchor) shape:

```
%10 objects -> r4
%9  this    -> r5
%2  drift   -> r6
%1  pull    -> r6   (evicts %2; %2 -> r8)
%0  dock    -> r6   (evicts %1; %1 -> r9)      <-- swap happens here
%34 surface -> r6   (evicts %0; %0 -> r10/fp)
%12 GOT     -> r9
%175 i      -> r9   (evicts %12)
```

Faithful shape:

```
%0 dock weight 2.398674e-04   %1 pull weight 2.533436e-04
$r9 is available at cost 1
assigning %0 to $r9
```

`pull`'s weight is `UseDefFreq / (Size + 25*InstrDist)`; the anchor's 50/50
branch halves the wormhole block's frequency, dropping `pull` from 2.53e-4
to 1.52e-4, which lets `dock` evict it. The whole swap is a branch-probability
effect of the anchor.

---

## 4. New evidence: the faithful shape's real blocker is the stack canary

Pass 4 said the faithful shape rotates because the impact scratch takes `r9`
(dock's register) and creates a shared restore block. That is true, but it is
downstream: **the impact scratch takes `r9` only because `sl` is already
occupied by `this`.**

### 4.1 Where `this = sl` comes from

Faithful scoped + real guard, preheader and collision block (objdump of the
object file):

```
  e:  mov   sl, r0            ; this = sl
 10:  ldr   r0, [pc, #956]
 12:  mov   r4, r1            ; objects = r4
 14:  cmp   r1, #0
 16:  add   r0, pc
 18:  ldr   r6, [r0, #0]      ; r6 = &__stack_chk_guard   <-- !!
 1a:  ldr   r0, [r6, #0]      ; canary
 1c:  str   r0, [sp, #108]
 ...
 4a:  add   r5, sp, #32       ; surface temp = r5
 ...
23c:  ldr   r1, [sl, #8]      ; this->geometry
240:  mov   r0, r5            ; &surface
242:  mov   r9, r6            ; save the canary pointer out of r6
244:  blx   getPosition
248:  ldr   r0, [r8]          ; object vtable
24c:  add   r6, sp, #96       ; collision temp = r6   (matches original!)
...
```

In the original and in the committed build the canary pointer is **spilled**
(`str r0,[sp,#24]`) and `r6` holds the surface temp; `this` stays in `r5`.
In the faithful shape `&__stack_chk_guard` is a long-lived value that keeps
`r6` (or `r9`), which cascades:

* `surface` takes `r5` (short interval, high spill weight),
* `this` is evicted to `sl`,
* `sl` is therefore unavailable for the impact scratch,
* the impact scratch takes `r9` (dock's register),
* the impact `continue` path needs a shared `ldr r9,[sp,#20]` restore block,
* MachineBlockPlacement rejects the `hitCamera → latch` fallthrough
  (`CONFLICT predfreq=63 candfreq=93`, pass 4 §3.1) and rotates the loop.

The committed anchor happens to spill the canary, which is why it is the only
shape found with `this = r5 / surface = r6 / object = r8`. The anchor is
therefore not merely a branch-weight knob — it is the *only* lever found that
gets the canary out of `r6`.

### 4.2 The collision-block shuffle is present in the faithful shape

Contrary to the committed build (which emits `add.w sl,sp,#96` for the
collision temp), the faithful scoped shape emits the original's `r6`
collision temp:

```
original   9f30c: mov sl,r5 ; 9f30e: mov r5,r6 ; 9f310: add r6,sp,#96
faithful    242: mov r9,r6 ; 24c: add r6,sp,#96
```

The difference is only *which* register is saved around the reuse (`sl` =
`this` in the original, `r9` = canary in the faithful shape). So diff #5 is
a consequence of the same `this = sl` problem, not an independent issue.

---

## 5. Full variant table (official `verify.score`; pass-4 rows marked)

Base = committed 87.3 / 968 / `this=r5 dock=fp i=r9 surf=r6 scratch=sl`.
"RA" is `this/dock/i/surf/scratch`; `-` = not matched by the extractor.

### 5.1 Faithful (no anchor) shapes — all ≤ 64.1 %

| variant | pct | size | RA |
|---|---:|---:|---|
| scoped surface, real guard | 59.8 | 996 | sl/r9/fp/r5/r9 |
| scoped surface, no guard | 59.8 | 988 | sl/r9/fp/r5/r9 |
| temp-form `outerCollide(getPosition())`, guard | 59.8 | 996 | sl/r9/fp/r5/r9 |
| `Vector surface(this->geometry->getPosition())`, guard | 59.8 | 996 | sl/r9/fp/r5/r9 |
| unscoped `Vector surface`, guard | 64.1 | 968 | r5/sl/fp/r6/r9 |
| unscoped `Vector surface`, no guard | 64.1 | 968 | r5/sl/fp/r6/r9 |
| surface computed before the `i == 0` block, guard | 64.1 | 968 | r5/sl/fp/r6/r9 |
| `Vector surface; surface = getPosition();`, guard | 58.4 | 984 | r5/r0/sl/r6/- |
| ditto, no guard | 58.6 | 976 | r5/r0/sl/r6/- |
| ditto + anchor | 63.8 | 996 | r5/r0/sl/-/- |
| `Vector surface;` at loop top, assign after `i == 0` | 58.4 | 984 | r5/r0/sl/r6/- |
| hoisted `Vector &dock = this->dockOffsetVec` + guard | 59.8 | 996 | sl/r9/fp/r5/r9 |
| hoisted `Vector &dock` + hoisted `Status *status` | 48.1 | 996 | fp/sl/sl/r5/r8 |
| `Vector surface` at function scope | 44.7 | 1028 | r8/-/sl/-/- |
| `while` + `i++` in body (pass 4) | 66.2 | 988 | r5/r0/r9/-/- |

### 5.2 Committed shape (anchor) micro-variants — all 87.3 or worse

| variant | pct | size | RA |
|---|---:|---:|---|
| committed | **87.3** | 968 | r5/fp/r9/r6/sl |
| + hoisted `Vector &dock` (ref or pointer) | 87.3 | 968 | r5/fp/r9/r6/sl |
| `int i` / `++i` / `i != size()` / `size() > i` | 87.3 | 968 | r5/fp/r9/r6/sl |
| `!object` / `!objects` / `auto` everywhere / no `this->` | 87.3 | 968 | r5/fp/r9/r6/sl |
| `i` declared outside the loop | 87.3 | 968 | r5/fp/r9/r6/sl |
| top-block perms `(orbit,len,flag)` | 87.3 | 968 | r5/fp/r9/r6/sl |
| top-block perms `(len,orbit,flag)` / `(len,flag,orbit)` | 85.5 | 968 | r5/fp/r9/r6/sl |
| top-block perms `(orbit,flag,len)` | 76.7 | 976 | r5/fp/r9/r6/fp |
| top-block perms `(flag,orbit,len)` / `(flag,len,orbit)` | 72.8 | 984 / 976 | r5/fp/sl/r6/fp or r9 |
| + real final guard | 79.4 | 976 | r5/fp/r9/r6/sl |
| + guard + hoisted dock | 79.4 | 976 | r5/fp/r9/r6/sl |
| `Vector surface(this->...getPosition())` + anchor | 79.4 | 976 | r5/fp/r9/r6/sl |
| `Vector surface; surface = ...` + anchor | 64.9 | 968 | r5/r0/sl/-/- |
| named `Vector position` for the collision call | 70.2 | 976 | r5/fp/r9/-/- |
| named `Vector position` for the impact call | 76.9 | 968 | r5/fp/r9/-/- |
| both named | 68.8 | 976 | r5/fp/r9/-/- |

### 5.3 Synthetic score components (from pass 5, re-used)

Textual edits of the normalised stream:

| edit | score |
|---|---:|
| committed (baseline) | 87.3 |
| swap `fp`↔`r9` only | **92.2** (pass 4 quotes 93.1 for its raw variant) |
| collision-block shuffle only | 88.6 |
| swap + collision-block shuffle | **93.5** |

So the swap is worth ≈ 5 points and is the only lever big enough to matter;
the collision shuffle is worth ≈ 1.3 points but cannot be added without
changing the size (the named-temp form costs +8 B and drops to 70.2).

---

## 6. What this pass proved / what remains

**Proved:**

1. The `dock`/`i` choice is a **vreg-number tie-break on two exactly
   equal-sized live intervals** (`size = 451` each, measured from the MIR
   CFG). `RAGreedy::enqueue` breaks the tie by `~Reg` (lower vreg first).
2. The greedy pass renumbers the dock interval to `%189` (committed) /
   `%245` (faithful), **above** `i`'s `%175`/`%174`, so `i` wins the tie in
   both shapes. This explains why every source-level "hoist dock" attempt is
   neutral.
3. The faithful shape's `this = sl` is caused by **`&__stack_chk_guard`
   keeping `r6`** (or `r9`), not by the impact scratch per se. `surface`
   then takes `r5`, `this` is evicted to `sl`, `sl` is unavailable for the
   impact temp, the impact temp takes `r9` (dock), and the shared restore
   block rotates the loop.
4. The collision-block shuffle (`r6` collision temp) is present in the
   faithful shape; it is the same `this = sl` consequence.
5. The anchor is the only lever found that spills the canary and produces
   `this=r5 / surface=r6 / object=r8` — and it is exactly what makes `i`
   beat `dock` in the eviction sequence.

**Remaining (the win condition):** a **faithful** (no-anchor) shape that
(a) spills `&__stack_chk_guard`, (b) keeps `this=r5`, `surface=r6`,
`object=r8`, and (c) leaves `sl` free for the impact scratch, so that
`dock=r9` survives without a restore block. Since (a) and (c) are both
consequences of the same register-pressure balance, the remaining lever is
the *canary's* live interval — which is compiler-generated and not
expressible in source. A future pass could try `-fno-stack-protector`-like
source shapes (e.g. removing the local `Vector` temporaries that force the
canary) or a TU-level build flag experiment, but that is outside this
function's source.

**Do not land** any of the 59–79 % variants: they are strictly worse than
the committed 87.3.

---

## 7. Reproduction

```sh
# gate
cmake --build cmake-build-match --target check -j8        # RATCHET PASS

# one-function score / RA
python3 /dev/shm/w4230/score.py        # pct / size / insns
python3 /dev/shm/w4230/build_variant.py  # insns + this/dock/i/surf/scratch RA

# MIR dumps (~55 s each)
clang++ ... -mllvm -print-after-all -o /dev/shm/w4230/pe_x.o
#   committed: paall_anchor.log  (LRM line 3065218, greedy line 3065950)
#   faithful : paall_nested.log  (LRM line 3064994, greedy line 3065731)

# official verifier
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE
```

Gate at this commit (unchanged): `byte_exact 4498 / linked_exact 4498 /
size-exact 4524`, `stub_zero_size 0`, `missing 0`, `wrong_type 0`, `extra 0`,
`data_exact 843/843`, imports 207, `.text 0x1337c8`, `.rodata 0x41b67`,
`.ARM.exidx 0x6f70` exact, `.ARM.extab 0x26290` (−8), lint `CLEAN ✓`,
drift 0 (`RATCHET PASS`).
