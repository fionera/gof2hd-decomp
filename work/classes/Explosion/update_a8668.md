# Explosion::update
elf 0xa8668 body 422
Sig: undefined __thiscall update(Explosion * this, int param_1, TargetFollowCamera * param_2)

## decompile
```c

/* Explosion::update(int, TargetFollowCamera*) */

void __thiscall Explosion::update(Explosion *this,int param_1,TargetFollowCamera *param_2)

{
  uint uVar1;
  uint *puVar2;
  float fVar3;
  int iVar4;
  bool bVar5;
  int iVar6;
  int *piVar7;
  uint *puVar8;
  uint in_fpscr;
  float fVar9;
  float fVar10;
  longlong lVar11;
  Matrix aMStack_4c [12];
  Vector aVStack_40 [12];
  Vector aVStack_34 [12];
  int local_28;
  
  piVar7 = *(int **)(DAT_000b8818 + 0xb867e);
  local_28 = *piVar7;
  if (this[0x20] != (Explosion)0x0) {
    puVar8 = *(uint **)(DAT_000b881c + 0xb8696);
    lVar11 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar8);
    bVar5 = SUB41(param_1,0);
    AbyssEngine::Transform::Update(lVar11,bVar5);
    if (*(int *)(*(int *)(this + 4) + 0x14) != -1) {
      lVar11 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar8);
      AbyssEngine::Transform::Update(lVar11,bVar5);
    }
    if (*(int *)(this + 8) != 0) {
      uVar1 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar8);
      AbyssEngine::Transform::Update((ulonglong)uVar1,bVar5);
    }
    puVar2 = *(uint **)(this + 0xc);
    if (puVar2 != (uint *)0x0) {
      for (uVar1 = 0; uVar1 < *puVar2; uVar1 = uVar1 + 1) {
        lVar11 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar8);
        AbyssEngine::Transform::Update(lVar11,bVar5);
        puVar2 = *(uint **)(this + 0xc);
      }
    }
    if ((param_2 != (TargetFollowCamera *)0x0) && (*(uint *)this < 2)) {
      AEGeometry::getPosition();
      uVar1 = *puVar8;
      AbyssEngine::PaintCanvas::CameraGetCurrent();
      AbyssEngine::PaintCanvas::CameraGetLocal(uVar1);
      AbyssEngine::AEMath::MatrixGetPosition(aMStack_4c);
      AbyssEngine::AEMath::operator-(aVStack_34,aVStack_40);
      fVar3 = (float)AbyssEngine::AEMath::VectorLength(aVStack_34);
      iVar4 = AbyssEngine::PaintCanvas::TransformGetTransform(*puVar8);
      if (*(int *)(iVar4 + 0x110) < 0x7d1) {
        fVar9 = (float)VectorSignedToFloat(*(int *)(iVar4 + 0x110),
                                           (byte)((in_fpscr & 0xfffffff) >> 0x16) & 3);
        fVar10 = DAT_000b8810;
        if (fVar3 < DAT_000b8810) {
          fVar10 = fVar3;
        }
        TargetFollowCamera::setRumblePercentage
                  ((1.0 - fVar10 / DAT_000b8810) * (fVar9 / DAT_000b8814 + 1.0),(int)param_2);
      }
    }
    iVar4 = *(int *)(this + 0x14);
    uVar1 = *(uint *)(this + 0x18) + param_1;
    iVar6 = *(int *)(this + 0x1c) + (param_1 >> 0x1f) + (uint)CARRY4(*(uint *)(this + 0x18),param_1)
    ;
    *(uint *)(this + 0x18) = uVar1;
    *(int *)(this + 0x1c) = iVar6;
    if (((int)((iVar4 - iVar6) - (uint)(*(uint *)(this + 0x10) < uVar1)) < 0 !=
         (SBORROW4(iVar4,iVar6) != SBORROW4(iVar4 - iVar6,(uint)(*(uint *)(this + 0x10) < uVar1))))
       && (fVar3 = (float)reset(this), param_2 != (TargetFollowCamera *)0x0)) {
      TargetFollowCamera::setRumblePercentage(fVar3,(int)param_2);
    }
  }
  if (*piVar7 != local_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  000b8668: push {r4,r5,r6,r7,lr}
  000b866a: add r7,sp,#0xc
  000b866c: push {r8,r9,r10,r11}
  000b8670: sub sp,#0x34
  000b8672: mov r5,r0
  000b8674: ldr r0,[0x000b8818]
  000b8676: mov r11,r2
  000b8678: mov r10,r1
  000b867a: add r0,pc
  000b867c: ldr r4,[r0,#0x0]
  000b867e: ldr r0,[r4,#0x0]
  000b8680: str r0,[sp,#0x30]
  000b8682: ldrb.w r0,[r5,#0x20]
  000b8686: cmp r0,#0x0
  000b8688: beq.w 0x000b87fa
  000b868c: str r4,[sp,#0x8]
  000b868e: ldr r0,[0x000b881c]
  000b8690: ldr r1,[r5,#0x4]
  000b8692: add r0,pc
  000b8694: ldr.w r9,[r0,#0x0]
  000b8698: ldr r1,[r1,#0xc]
  000b869a: ldr.w r0,[r9,#0x0]
  000b869e: blx 0x00072088
  000b86a2: asr.w r4,r10, asr #0x1f
  000b86a6: mov.w r8,#0x0
  000b86aa: mov r2,r10
  000b86ac: mov r3,r4
  000b86ae: str.w r8,[sp,#0x0]
  000b86b2: blx 0x0006f7cc
  000b86b6: ldr r0,[r5,#0x4]
  000b86b8: ldr r1,[r0,#0x14]
  000b86ba: adds r0,r1,#0x1
  000b86bc: beq 0x000b86d2
  000b86be: ldr.w r0,[r9,#0x0]
  000b86c2: blx 0x00072088
  000b86c6: mov r2,r10
  000b86c8: mov r3,r4
  000b86ca: str.w r8,[sp,#0x0]
  000b86ce: blx 0x0006f7cc
  000b86d2: ldr r0,[r5,#0x8]
  000b86d4: cbz r0,0x000b86ec
  000b86d6: ldr r1,[r0,#0xc]
  000b86d8: ldr.w r0,[r9,#0x0]
  000b86dc: blx 0x00072088
  000b86e0: movs r1,#0x0
  000b86e2: mov r2,r10
  000b86e4: mov r3,r4
  000b86e6: str r1,[sp,#0x0]
  000b86e8: blx 0x0006f7cc
  000b86ec: ldr r0,[r5,#0xc]
  000b86ee: cbz r0,0x000b871e
  000b86f0: movs r6,#0x0
  000b86f2: mov.w r8,#0x0
  000b86f6: b 0x000b8718
  000b86f8: ldr r0,[r0,#0x4]
  000b86fa: ldr.w r1,[r0,r8,lsl #0x2]
  000b86fe: ldr.w r0,[r9,#0x0]
  000b8702: ldr r1,[r1,#0xc]
  000b8704: blx 0x00072088
  000b8708: mov r2,r10
  000b870a: mov r3,r4
  000b870c: str r6,[sp,#0x0]
  000b870e: blx 0x0006f7cc
  000b8712: ldr r0,[r5,#0xc]
  000b8714: add.w r8,r8,#0x1
  000b8718: ldr r1,[r0,#0x0]
  000b871a: cmp r8,r1
  000b871c: bcc 0x000b86f8
  000b871e: mov r6,r11
  000b8720: cmp.w r11,#0x0
  000b8724: beq 0x000b87cc
  000b8726: ldr r0,[r5,#0x0]
  000b8728: cmp r0,#0x1
  000b872a: bhi 0x000b87cc
  000b872c: add.w r11,sp,#0x18
  000b8730: ldr r1,[r5,#0x4]
  000b8732: mov r0,r11
  000b8734: blx 0x000720b8
  000b8738: ldr.w r4,[r9,#0x0]
  000b873c: mov r0,r4
  000b873e: blx 0x000717f4
  000b8742: mov r1,r0
  000b8744: mov r0,r4
  000b8746: blx 0x0006ff1c
  000b874a: add.w r8,sp,#0xc
  000b874e: mov r1,r0
  000b8750: mov r0,r8
  000b8752: blx 0x0006f16c
  000b8756: add r4,sp,#0x24
  000b8758: mov r1,r11
  000b875a: mov r2,r8
  000b875c: mov r0,r4
  000b875e: blx 0x0006ec38
  000b8762: mov r0,r4
  000b8764: blx 0x0006ec44
  000b8768: ldr r1,[r5,#0x4]
  000b876a: mov r4,r0
  000b876c: ldr.w r0,[r9,#0x0]
  000b8770: ldr r1,[r1,#0xc]
  000b8772: blx 0x00072088
  000b8776: ldr.w r0,[r0,#0x110]
  000b877a: cmp.w r0,#0x7d0
  000b877e: bgt 0x000b87cc
  000b8780: vldr.32 s4,[pc,#0x8c]
  000b8784: vmov s0,r4
  000b8788: vmov s2,r0
  000b878c: vldr.32 s6,[pc,#0x84]
  000b8790: vcmpe.f32 s0,s4
  000b8794: mov r0,r6
  000b8796: vmrs apsr,fpscr
  000b879a: vcvt.f32.s32 s2,s2
  000b879e: vmov.f32 s8,s4
  000b87a2: mov.w r2,#0x32
  000b87a6: it mi
  000b87a8: vmov.mi.f32 s8,s0
  000b87ac: vdiv.f32 s0,s2,s6
  000b87b0: vdiv.f32 s2,s8,s4
  000b87b4: vmov.f32 s4,0x3f800000
  000b87b8: vadd.f32 s0,s0,s4
  000b87bc: vsub.f32 s2,s4,s2
  000b87c0: vmul.f32 s0,s2,s0
  000b87c4: vmov r1,s0
  000b87c8: blx 0x000728ec
  000b87cc: add.w r3,r5,#0x10
  000b87d0: ldmia r3,{r0,r1,r2,r3}
  000b87d2: adds.w r2,r2,r10
  000b87d6: adc.w r3,r3,r10, asr #0x1f
  000b87da: subs r0,r0,r2
  000b87dc: sbcs.w r0,r1,r3
  000b87e0: strd r2,r3,[r5,#0x18]
  000b87e4: ldr r4,[sp,#0x8]
  000b87e6: bge 0x000b87fa
  000b87e8: mov r0,r5
  000b87ea: blx 0x00072c88
  000b87ee: cbz r6,0x000b87fa
  000b87f0: mov r0,r6
  000b87f2: movs r1,#0x0
  000b87f4: movs r2,#0x0
  000b87f6: blx 0x000728ec
  000b87fa: ldr r0,[sp,#0x30]
  000b87fc: ldr r1,[r4,#0x0]
  000b87fe: subs r0,r1,r0
  000b8800: ittt eq
  000b8802: add.eq sp,#0x34
  000b8804: pop.eq.w {r8,r9,r10,r11}
  000b8808: pop.eq {r4,r5,r6,r7,pc}
  000b880a: blx 0x0006e824
```
