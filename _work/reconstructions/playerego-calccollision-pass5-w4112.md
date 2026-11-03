# PlayerEgo::calcCollision — pass 5 (w4112): the anchor *pair*, and why an
# anchor can never pay for itself

**Status:** research note, 2026-11-03. Worktree `wt/w4112`, branch
`wave91-w4112`, base `a4557300`. **No source change landed.** The committed
shape (one artificial `objects->size() <= i` anchor after `outerCollide`, no
final `i == 0` guard) is still the best at **87.3 % / 968 B**.

This pass closes the question left open by pass 4
(`playerego-calccollision-pass4-w4012.md`): *can the original register
assignment (`dock = r9`, `i = fp`, `surface = r6`, impact scratch `sl`) be
reached at all?* **Yes — but only with a second anchor, and the second anchor
costs more than the register swap is worth.** The exact original RA is
reproduced for the first time (85.5 % / 976 B, and 85.7 % / 984 B with the
real `i == 0` guard). The reason it does not win is quantified in §6: the
verifier normalises branch targets to `loc_<byte-offset-from-function-start>`,
so **any** size change shifts every later target and costs far more than the
three instructions the anchor actually is.

Target: `_ZN9PlayerEgo13calcCollisionEP5ArrayIP8KIPlayerE` (968 B, 87.3 %,
347 original instructions vs 345 ours).

---

## 1. Ground truth: Ghidra decompilation of the original

Produced with headless Ghidra 12.1.2 on
`android_2.0.16_libgof2hdaa.so_ghidra` (`DecompCalc.java`, `-noanalysis`).
Inlined verbatim so this note is self-contained:

```c
/* PlayerEgo::calcCollision(Array<KIPlayer*>*) */

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

**There is no `objects->size()` check inside the loop body** — the only size
load is the loop-header test at `9f134`. The committed anchor is artificial.

## 2. Original vs committed: exactly five differences

| # | diff | orig | ours (committed) |
|---|---|---|---|
| 1 | dock/i register swap | `add.w r9,r5,#328`; `mov.w fp,#0` | `add.w fp,r5,#328`; `mov.w r9,#0` |
| 2 | preheader spill | `str.w r9,[sp,#20]` at `9f12a` | absent (spilled twice at use sites instead) |
| 3 | artificial anchor | absent | `ldr r0,[r4,#0]; cmp r0,r9; bls.w` at `9f190` |
| 4 | real final guard | `cmp.w fp,#0; bne.n` at `9f374` | absent |
| 5 | collision-block shuffle | `mov sl,r5; mov r5,r6; add r6,sp,#96; … mov r6,r5; mov r5,sl; mov r1,r6` | `add.w sl,sp,#96; … mov r1,sl` |

Post-register-allocation table (from `objdump -d` of the linked `.so`,
extracted by matching the preheader `add.w X, r5, #328`, the `mov.w Y, #0`
loop counter, the `add Z, sp, #32` surface temp and the `add.w W, sp, #96`
impact temp):

| value | original | committed | pass-5 two-anchor shape |
|---|---|---|---|
| `this` | r5 | r5 | r5 |
| `objects` | r4 | r4 | r4 |
| `object` | r8 | r8 | r8 |
| `dockOffsetVec` | **r9** | fp | **r9** |
| `i` | **fp** | r9 | **fp** |
| `surface` temp (sp+32) | r6 | r6 | r6 |
| impact temp (sp+96) | sl | sl | sl |

So the committed shape has *one* wrong pair; everything else is already
correct. The fp↔r9 rename alone is worth **+5.8 points** (87.3 → 93.1,
measured with `verify.score` after textually renaming fp↔r9 on our normalised
stream).

## 3. Pass-4 recap: why `dock = r9` forces the impact scratch to `sl`

With `dock = r9`, `r9` is live across the whole loop, so the impact temp
(`add.w X, sp, #96` before `getPosition`) must take another callee-saved
register. The allocator picks `sl`, exactly as the original shows at
`9f27a: add.w sl, sp, #96`. With `dock = fp` (committed) `r9` is free, so the
impact temp takes `r9`… except in the committed build it takes `sl` anyway
because `i` is in `r9`. Either way `sl` is the scratch in the committed build,
which is why diff #5 is *not* fixed by the RA swap alone.

In the faithful (`rg`) shape `dock = r9` **and** the impact scratch is also
`r9` (dock is stack-resident and `r9` is reused as a scratch), which creates a
shared restore block that makes MachineBlockPlacement rotate the loop
(996 B, 60.1 %). The original has `dock = r9` and scratch `sl`, so no shared
restore block and no rotation.

## 4. New this pass: the anchor *pair* reproduces the exact original RA

The committed anchor alone gives `dock = fp`. A **second** anchor of the same
form placed at the top of the wormhole block, *in addition to* the committed
one, flips the allocation to the exact original assignment:

```c
        if (object->getType() == 16994 && object->isVisible()) {
            if (((PlayerWormHole *) object)->isShrinking() || this->miningGame != 0)
                continue;
            if (objects->size() <= i)          // <-- second anchor (new)
                continue;
            Vector &pull = this->moveTarget;
            ...
```

Result: `this=r5 obj=r4 dock=r9 i=fp surf=r6 scratch=sl` — **all five correct
for the first time** — at 976 B / **85.5 %** (`N6`). Adding the real
`i == 0` guard gives 984 B / **85.7 %** (`N5`). Both are still below the
committed 87.3 %.

Key facts about the second anchor:

* It must reference `objects->size()` **and** the loop counter. Anchors that
  only touch `i` do not flip the RA:
  `i > 1000u` → 80.8 % / 972 B (dock=fp); `i == 0xffffffffu` and `i + 1 == 0`
  are folded away entirely (87.3 % identical); `i != 0` / `i > 0` → 79.4 %
  (dock=fp).
* Branches on `objects->size()` alone do flip it, in either direction:
  `size() == 0`, `size() != 0`, `size() < i`, `size() > i`, `size() <= i`,
  `size() >= i`, `i + 1 > size()` all give `dock=r9 i=fp surf=r6 scratch=sl`.
  Scores 83.2 – 85.5 % (the branch direction changes the block layout).
* **Moving** the committed anchor to the wormhole top (instead of adding a
  second one) gives `dock = r0` (spilled) at 70.8 % — it is the *pair* that
  matters, not the position.
* `__builtin_expect` on existing branches is free in code size but does not
  flip the RA: `__builtin_expect(!outerCollide(...), 0)` → 87.3 % identical;
  `__builtin_expect(strength >= 1, 1)` → 66.3 %; `(playing, 1)` → 64.6 %;
  `(getType()==16994, 0)` → 48.0 % (`dock=r9 i=fp surf=r6` but `this=sl` and
  the loop rotates, 1000 B).

## 5. Why the anchor can never pay for itself (the offset effect)

`verify.score` is `difflib.SequenceMatcher(None, a, b, autojunk=False).ratio()`
over the **normalised** instruction streams. `disassemble()` renders branch
targets as `loc_<offset-from-function-start>` (e.g. `loc_386`), so a branch to
the same *logical* block at a different byte offset is a **mismatch**.

Consequences, measured on the committed source:

* The committed build is 968 B (same as the original) but its internal offsets
  are already off by −4 (missing preheader spill) before the wormhole block.
* Adding the second anchor (+8 B) shifts every later branch target by +8, so
  every `loc_*` after the anchor mismatches. That is why a 3-instruction,
  8-byte insertion costs ≈ 7.6 points (≈ 26 normalised instructions), not the
  ≈ 0.9 points a pure insertion would cost.
* The real `i == 0` guard is 2 instructions / 4 B and costs ≈ 12.7 points when
  the rest does not match (100 → 87.3 for the committed shape).

Direct diff of the committed `.so` against the two-anchor `.so` (normalised,
`cmp2.py` in §8) shows the *only* changes are the fp↔r9 swap and the anchor —
confirming that the two-anchor shape is "committed + swap + anchor" and that
the anchor's offset shift is the entire cost.

## 6. Full variant table (this pass)

All measured with the official verifier's `score()` (`autojunk=False`) and the
RA extracted from the linked `.so`. Base = 87.3 / 968 / `r5,r4,fp,r9,r6,sl`.
"RA" column is `this/dock/i/surf/scratch`.

### 6.1 Anchor moved to the top of the wormhole block (committed anchor kept)

| variant | pct | size | RA |
|---|---:|---:|---|
| `size() <= i` (`N6`) | **85.5** | 976 | r5/r9/fp/r6/sl |
| `size() <= i` + real guard (`N5`) | **85.7** | 984 | r5/r9/fp/r6/sl |
| `i >= size()` | 85.5 | 976 | r5/r9/fp/r6/sl |
| `size() == 0` | 85.5 | 976 | r5/r9/fp/r6/sl |
| `size() > i` / `size() < i` | 85.5 | 976 | r5/r9/fp/r6/sl |
| `size() != 0` | 83.2 | 976 | r5/r9/fp/r6/sl |
| `i + 1 > size()` | 85.3 | 980 | r5/r9/fp/r6/sl |
| `i > 1000u` | 80.8 | 972 | r5/fp/r9/r6/sl |
| `i >= 1000u` | 80.6 | 976 | r5/fp/r9/r6/sl |
| `i > 0xffffu` | 80.1 | 972 | r5/fp/r9/r6/sl |
| `i == 1000u` | 80.8 | 972 | r5/fp/r9/r6/sl |
| `i != 0` / `i > 0` | 79.4 | 972 | r5/fp/r9/r6/sl |
| `i == 0xffffffffu` / `i + 1 == 0` / `i < 0` | 87.3 | 968 | r5/fp/r9/r6/sl (folded away) |

### 6.2 Anchor at the committed position, different conditions

| variant | pct | size | RA |
|---|---:|---:|---|
| `size() <= i` (committed) | **87.3** | 968 | r5/fp/r9/r6/sl |
| `i >= size()` | 87.3 | 968 | r5/fp/r9/r6/sl |
| `size() >= i` | 87.3 | 968 | r5/fp/r9/r6/sl |
| `size() > 0xffff` | 79.8 | 960 | r5/fp/r9/r6/sl |
| `size() < i` / `size() > i` / `size() == i` / `size()-i==0` / `i-size==0` / `i>size` / `i==size` | 79.5 | 960 | r5/fp/r9/r6/sl |
| `i+1 > size()` / `size() < i+1` | 79.1 | 964 | r5/fp/r9/r6/sl |
| `size() <= i+1` | 79.4 | 972 | r5/fp/r9/r6/sl |
| `size() == 0` / `size() == 0xffffffff` | 65.5 | 996 | sl/fp/r9/r5/sl |
| `size() != 0` | 77.7 | 960 | r5/fp/r9/r6/sl |
| `i >= 0xffff` | 67.5 | 960 | r5/r6/sl/r3/? |

### 6.3 Anchor at other positions (committed anchor removed)

| position | pct | size | RA |
|---|---:|---:|---|
| top of wormhole (moved) | 70.8 | 984 | r5/r0/r9/r6/sl |
| before `if (strength >= 1)` (moved) | 53.6 | 1004 | r9/fp/sl/r5/? |
| before `if (strength >= 1)` (added) | 83.1 | 984 | r5/r9/fp/r6/sl |
| after `isPlaying` (added) | 83.1 | 984 | r5/r9/fp/r6/sl |
| before `strength =` (added) | 81.9 | 980 | r5/r9/fp/r6/sl |
| before `VectorNormalize` (added) | 81.3 | 988 | r5/r0/fp/r6/sl |
| before `translate` / `hitCamera` / `distance<1000` (added) | 84.1 / 84.4 / 84.4 | 980 | r5/fp/r9/r6/sl |
| before `stealFlagByte` (added) | 60.6 | 996 | r5/r0/r9/r6/? |
| before collision `isVisible` (added) | 72.0 | 992 | r5/r0/r9/r6/sl |
| end of loop body (added) | 79.3 | 980 | r5/fp/r9/r6/sl |
| cold `strength < 1` block (added) | 84.2 | 976 | r5/fp/r9/r6/sl |
| `play()` block (added) | 74.9 | 972 | r5/r0/r9/r6/fp |

### 6.4 Loop form / declarations / branch hints (all at the committed anchor)

| variant | pct | size | RA |
|---|---:|---:|---|
| `for (int i …)` | 87.0 | 968 | r5/fp/r9/r6/sl |
| `for (unsigned i …)` / `size_t` / `++i` / `i != size()` | 87.3 / 87.3 / 87.3 / 87.0 | 968 | r5/fp/r9/r6/sl |
| hoist `n = objects->size()` | 75.0 | 976 | r5/r0/r9/r6/? |
| `i` declared outside the `for` | 87.3 | 968 | r5/fp/r9/r6/sl |
| `Vector &dock = this->dockOffsetVec` before the loop | 87.3 | 968 | r5/fp/r9/r6/sl |
| `const Vector &dock` / `const Vector *dock` | 87.3 | 968 | r5/fp/r9/r6/sl |
| top-block reorder (dock before status) | 85.5 | 968 | r5/fp/r9/r6/sl |
| top-block reorder (`field_0x71` first) | 86.4 | 968 | r5/fp/r9/r6/sl |
| `!i` / `0 == i` / `i < 1` / nested `if` | 87.3 | 968 | r5/fp/r9/r6/sl |
| `if (!object)` instead of `== nullptr` | 87.3 | 968 | r5/fp/r9/r6/sl |
| `outerCollide(this->geometry->getPosition())` temp form | 87.3 | 968 | r5/fp/r9/r6/sl |
| `const Vector surface` | 87.3 | 968 | r5/fp/r9/r6/sl |
| surface braces removed | 72.2 | 1000 | r5/r9/fp/r3/? |
| surface block before the `i == 0` block | 73.2 | 976 | r5/fp/sl/r6/fp |
| early-out `strength < 1` restructure | 87.0 | 968 | r5/fp/r9/r6/sl |
| early-out + guard | 79.4 | 976 | r5/fp/r9/r6/sl |
| `__builtin_expect(!outerCollide, 0)` | 87.3 | 968 | r5/fp/r9/r6/sl |
| `__builtin_expect(strength >= 1, 1)` | 66.3 | 964 | r5/sl/r9/r3/sl |
| `__builtin_expect(strength >= 1, 0)` | 68.5 | 1000 | r5/r9/fp/?/? |
| `__builtin_expect(playing, 1)` / `(playing, 0)` | 64.6 / 46.5 | 980 / 972 | r5/fp/r9/r6/sl |
| `__builtin_expect(getType()==16994, 1)` | 58.4 | 968 | r5/r8/r9/r3/? |
| `__builtin_expect(getType()==16994, 0)` | 48.0 | 1000 | sl/r9/fp/r6/r9 |
| `__builtin_expect(stealFlagByte, 1)` / `(…, 0)` | 60.5 / 54.5 | 992 / 1004 | r5/r0/sl/r6/r9 |
| `__builtin_expect(!isVisible(), 0)` | 71.6 | 996 | sl/r9/fp/r5/sl |
| `__builtin_expect(object == nullptr, 0)` | 78.4 | 964 | r5/fp/sl/r6/r9 |
| guard added to the committed anchor | 79.4 | 976 | r5/fp/r9/r6/sl |
| no anchor + guard (`rg`) | 59.8 | 996 | sl/r9/fp/r5/r9 |
| no anchor, no guard | 59.8 | 988 | sl/r9/fp/r5/r9 |

## 7. What this proves / what remains

**Proved:**

1. The original's exact register assignment **is** reachable from C++ source:
   `this=r5, objects=r4, dock=r9, i=fp, surface=r6, impact=sl`. The committed
   anchor plus a second `objects->size() <= i` anchor at the top of the
   wormhole block produces it (85.5 % / 976 B; 85.7 % / 984 B with the real
   guard).
2. The lever is *not* the branch weight alone. Anchors that reference only
   `i` never flip the RA; anchors that reference `objects->size()` (in either
   direction) do. This is consistent with the anchor keeping `objects` (r4)
   and `i` (fp) live/weighted across the wormhole block, which changes the
   greedy eviction order for `dock`.
3. The committed shape's 87.3 % is *not* limited by the RA swap alone: the
   swap is worth 5.8 points, but any anchor that buys it costs ≥ 7.6 points
   because of the `loc_<offset>` branch-target normalisation. A correct-RA
   shape therefore cannot beat 87.3 % while it carries an anchor.
4. `__builtin_expect` on existing branches is size-free but does not flip the
   RA (best: `!outerCollide` → 87.3 % identical).

**Remaining (the actual win condition):** a shape with `dock=r9, i=fp,
surf=r6, scratch=sl` that is **968 B** and does **not** rotate. Since the
original has no anchor, this requires the faithful (no-anchor) shape to stop
rotating. Pass 4 established the rotation cause: in the faithful shape the
impact scratch takes `r9` (dock's register), producing a shared restore block
whose frequency (63) fails `hasBetterLayoutPredecessor` against the
`hitCamera → latch` edge (93). The next lever is therefore **not** another
anchor but a source shape that keeps `dock` in `r9` *and* makes the impact
scratch take `sl` **without** adding a branch — i.e. a live-range/priority
difference between `dock` and the impact scratch, not a CFG difference.

**Do not** land any of the 85.x variants: they are strictly worse than the
committed 87.3.

## 8. Reproduction

```sh
# baseline
cmake --build cmake-build-match --target check -j8        # RATCHET PASS

# score / RA of the current source
python3 <scratch>/score.py        # prints pct/size/insns for calcCollision
python3 <scratch>/ra_dis.py       # prints this/obj/dock/i/surf/scratch

# the two-anchor shape (85.5 %):
#   insert, before `Vector &pull = this->moveTarget;`:
#       if (objects->size() <= i)
#           continue;
#   and, in the collision block, change
#       if (object->field_0x71)
#   to
#       if (i == 0 && object->field_0x71)          # -> 85.7 %, 984 B

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
