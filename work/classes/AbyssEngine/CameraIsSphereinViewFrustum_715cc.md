# AbyssEngine::CameraIsSphereinViewFrustum
elf 0x715cc body 428
Sig: undefined __stdcall CameraIsSphereinViewFrustum(Vector * param_1, float param_2, Matrix * param_3, Camera * param_4)

## decompile
```c

/* AbyssEngine::CameraIsSphereinViewFrustum(AbyssEngine::AEMath::Vector const&, float,
   AbyssEngine::AEMath::Matrix*, AbyssEngine::Camera*) */

void AbyssEngine::CameraIsSphereinViewFrustum
               (Vector *param_1,float param_2,Matrix *param_3,Camera *param_4)

{
  undefined4 uVar1;
  float fVar2;
  float fVar3;
  int in_r3;
  Matrix *pMVar4;
  Matrix *pMVar5;
  int *piVar6;
  float fVar7;
  Matrix aMStack_a4 [12];
  AEMath aAStack_98 [12];
  Matrix aMStack_8c [12];
  Matrix aMStack_80 [64];
  undefined4 local_40;
  undefined4 uStack_3c;
  undefined4 local_38;
  int local_34;
  
  piVar6 = *(int **)(DAT_0008177c + 0x815f4);
  local_34 = *piVar6;
  uVar1 = 1;
  if ((float)param_3 != 0.0 && **(char **)(DAT_00081778 + 0x815f2) != '\0') {
    pMVar5 = (Matrix *)(in_r3 + 0xc);
    local_40 = 0;
    uStack_3c = 0;
    local_38 = 0;
    if (param_4 == (Camera *)0x0) {
      pMVar4 = aMStack_80;
    }
    else {
      __aeabi_memcpy(aMStack_80,pMVar5,0x3c);
      AEMath::Matrix::operator*=(aMStack_80,(Matrix *)param_4);
      pMVar4 = aMStack_8c;
      pMVar5 = aMStack_80;
    }
    AEMath::MatrixInverseTransformVector(pMVar4,(Vector *)pMVar5);
    AEMath::Vector::operator=((Vector *)&local_40,(Vector *)pMVar4);
    AEMath::MatrixGetPosition(aMStack_8c);
    AEMath::operator-((Vector *)aMStack_80,param_1);
    AEMath::MatrixGetDir(aMStack_a4);
    AEMath::operator-(aAStack_98,(Vector *)aMStack_a4);
    AEMath::VectorNormalize((AEMath *)aMStack_8c,(Vector *)aAStack_98);
    fVar2 = (float)AEMath::VectorDot((Vector *)aMStack_80,(Vector *)aMStack_8c);
    param_2 = *(float *)(in_r3 + 8) + (float)param_3;
    if ((fVar2 <= param_2) &&
       (param_2 = *(float *)(in_r3 + 4) - (float)param_3,
       -1 < (int)((uint)(fVar2 < param_2) << 0x1f))) {
      AEMath::MatrixGetRight((Matrix *)aAStack_98);
      AEMath::VectorNormalize((AEMath *)aMStack_8c,(Vector *)aAStack_98);
      fVar3 = (float)AEMath::VectorDot((Vector *)aMStack_80,(Vector *)aMStack_8c);
      param_2 = *(float *)(in_r3 + 0x54) * (float)param_3;
      fVar7 = fVar2 * *(float *)(in_r3 + 0x48) * *(float *)(in_r3 + 0x50);
      if ((fVar3 <= fVar7 + param_2) &&
         (param_2 = -fVar7 - param_2, -1 < (int)((uint)(fVar3 < param_2) << 0x1f))) {
        AEMath::MatrixGetUp((Matrix *)aAStack_98);
        AEMath::VectorNormalize((AEMath *)aMStack_8c,(Vector *)aAStack_98);
        fVar3 = (float)AEMath::VectorDot((Vector *)aMStack_80,(Vector *)aMStack_8c);
        param_2 = *(float *)(in_r3 + 0x58) * (float)param_3;
        fVar2 = fVar2 * *(float *)(in_r3 + 0x48);
        if ((fVar3 <= fVar2 + param_2) &&
           (param_2 = -fVar2 - param_2, -1 < (int)((uint)(fVar3 < param_2) << 0x1f))) {
          uVar1 = 1;
          goto LAB_0008175a;
        }
      }
    }
    uVar1 = 0;
  }
LAB_0008175a:
  if (*piVar6 != local_34) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(param_2,uVar1);
  }
  return;
}

```

## target disasm
```
  000815cc: push {r4,r5,r6,r7,lr}
  000815ce: add r7,sp,#0xc
  000815d0: push {r8,r9,r10}
  000815d4: vpush {d8,d9}
  000815d8: sub sp,#0x78
  000815da: vmov s16,r1
  000815de: mov r6,r0
  000815e0: ldr r0,[0x00081778]
  000815e2: mov r4,r2
  000815e4: vcmpe.f32 s16,#0
  000815e8: ldr r2,[0x0008177c]
  000815ea: vmrs apsr,fpscr
  000815ee: add r0,pc
  000815f0: add r2,pc
  000815f2: mov r8,r3
  000815f4: ldr r0,[r0,#0x0]
  000815f6: ldr.w r10,[r2,#0x0]
  000815fa: ldrb r2,[r0,#0x0]
  000815fc: ldr.w r0,[r10,#0x0]
  00081600: str r0,[sp,#0x74]
  00081602: mov.w r0,#0x1
  00081606: it ne
  00081608: cmp.ne r2,#0x0
  0008160a: beq.w 0x0008175a
  0008160e: add.w r9,r8,#0xc
  00081612: movs r0,#0x0
  00081614: cmp r4,#0x0
  00081616: strd r0,r0,[sp,#0x68]
  0008161a: str r0,[sp,#0x70]
  0008161c: beq 0x00081638
  0008161e: add r5,sp,#0x28
  00081620: mov r1,r9
  00081622: movs r2,#0x3c
  00081624: mov r0,r5
  00081626: blx 0x0006f1e4
  0008162a: mov r0,r5
  0008162c: mov r1,r4
  0008162e: blx 0x0006f4a8
  00081632: add r4,sp,#0x1c
  00081634: mov r1,r5
  00081636: b 0x0008163c
  00081638: add r4,sp,#0x28
  0008163a: mov r1,r9
  0008163c: mov r0,r4
  0008163e: mov r2,r6
  00081640: blx 0x0006f4b4
  00081644: add r0,sp,#0x68
  00081646: mov r1,r4
  00081648: blx 0x0006eb3c
  0008164c: add r4,sp,#0x1c
  0008164e: mov r1,r9
  00081650: mov r0,r4
  00081652: blx 0x0006f16c
  00081656: add r5,sp,#0x28
  00081658: mov r1,r6
  0008165a: mov r2,r4
  0008165c: mov r0,r5
  0008165e: blx 0x0006ec38
  00081662: add r4,sp,#0x4
  00081664: mov r1,r9
  00081666: mov r0,r4
  00081668: blx 0x0006f4c0
  0008166c: add r6,sp,#0x10
  0008166e: mov r1,r4
  00081670: mov r0,r6
  00081672: blx 0x0006f4cc
  00081676: add r4,sp,#0x1c
  00081678: mov r1,r6
  0008167a: mov r0,r4
  0008167c: blx 0x0006ec80
  00081680: mov r0,r5
  00081682: mov r1,r4
  00081684: blx 0x0006ec68
  00081688: vldr.32 s0,[r8,#0x8]
  0008168c: vmov s18,r0
  00081690: vadd.f32 s0,s0,s16
  00081694: vcmpe.f32 s18,s0
  00081698: vmrs apsr,fpscr
  0008169c: bgt 0x00081758
  0008169e: vldr.32 s0,[r8,#0x4]
  000816a2: vsub.f32 s0,s0,s16
  000816a6: vcmpe.f32 s18,s0
  000816aa: vmrs apsr,fpscr
  000816ae: bmi 0x00081758
  000816b0: add r4,sp,#0x10
  000816b2: mov r1,r9
  000816b4: mov r0,r4
  000816b6: blx 0x0006f4e4
  000816ba: add r5,sp,#0x1c
  000816bc: mov r1,r4
  000816be: mov r0,r5
  000816c0: blx 0x0006ec80
  000816c4: add r0,sp,#0x28
  000816c6: mov r1,r5
  000816c8: blx 0x0006ec68
  000816cc: vldr.32 s0,[r8,#0x48]
  000816d0: vldr.32 s2,[r8,#0x50]
  000816d4: vmul.f32 s6,s18,s0
  000816d8: vldr.32 s4,[r8,#0x54]
  000816dc: vmul.f32 s0,s4,s16
  000816e0: vmul.f32 s4,s6,s2
  000816e4: vmov s2,r0
  000816e8: vadd.f32 s6,s4,s0
  000816ec: vcmpe.f32 s2,s6
  000816f0: vmrs apsr,fpscr
  000816f4: bgt 0x00081758
  000816f6: vneg.f32 s4,s4
  000816fa: vsub.f32 s0,s4,s0
  000816fe: vcmpe.f32 s2,s0
  00081702: vmrs apsr,fpscr
  00081706: bmi 0x00081758
  00081708: add r4,sp,#0x10
  0008170a: mov r1,r9
  0008170c: mov r0,r4
  0008170e: blx 0x0006f4d8
  00081712: add r5,sp,#0x1c
  00081714: mov r1,r4
  00081716: mov r0,r5
  00081718: blx 0x0006ec80
  0008171c: add r0,sp,#0x28
  0008171e: mov r1,r5
  00081720: blx 0x0006ec68
  00081724: vldr.32 s0,[r8,#0x58]
  00081728: vldr.32 s2,[r8,#0x48]
  0008172c: vmul.f32 s0,s0,s16
  00081730: vmul.f32 s4,s18,s2
  00081734: vmov s2,r0
  00081738: vadd.f32 s6,s4,s0
  0008173c: vcmpe.f32 s2,s6
  00081740: vmrs apsr,fpscr
  00081744: bgt 0x00081758
  00081746: vneg.f32 s4,s4
  0008174a: vsub.f32 s0,s4,s0
  0008174e: vcmpe.f32 s2,s0
  00081752: vmrs apsr,fpscr
  00081756: bpl 0x00081774
  00081758: movs r0,#0x0
  0008175a: ldr r1,[sp,#0x74]
  0008175c: ldr.w r2,[r10,#0x0]
  00081760: subs r1,r2,r1
  00081762: itttt eq
  00081764: add.eq sp,#0x78
  00081766: vpop.eq {d8,d9}
  0008176a: pop.eq.w {r8,r9,r10}
  0008176e: pop.eq {r4,r5,r6,r7,pc}
  00081770: blx 0x0006e824
  00081774: movs r0,#0x1
  00081776: b 0x0008175a
```
