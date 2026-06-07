# Transform::InCameraVF
elf 0x77478 body 300
Sig: undefined __thiscall InCameraVF(Transform * this, Matrix * param_1, Camera * param_2)

## decompile
```c

/* AbyssEngine::Transform::InCameraVF(AbyssEngine::AEMath::Matrix*, AbyssEngine::Camera*) */

void __thiscall AbyssEngine::Transform::InCameraVF(Transform *this,Matrix *param_1,Camera *param_2)

{
  undefined8 uVar1;
  undefined4 uVar2;
  Vector *pVVar3;
  int *piVar4;
  float fVar5;
  undefined8 uVar6;
  longlong lVar7;
  float fVar8;
  undefined8 uVar9;
  Matrix local_88 [60];
  Matrix aMStack_4c [12];
  float local_40;
  float fStack_3c;
  float local_38;
  undefined4 local_34;
  undefined4 uStack_30;
  undefined4 local_2c;
  int local_28;
  
  pVVar3 = (Vector *)local_88;
  piVar4 = *(int **)(DAT_000875a4 + 0x8748e);
  local_28 = *piVar4;
  if (((param_2 == (Camera *)0x0) || (this[0x17c] == (Transform)0x0)) ||
     ((*(int *)(this + 0x3c) == 1 && (*(short *)(**(int **)(this + 0x40) + 2) == 0)))) {
    uVar2 = 1;
  }
  else {
    local_34 = 0;
    uStack_30 = 0;
    local_2c = 0;
    local_40 = *(float *)(this + 0xe0);
    fStack_3c = local_40;
    local_38 = local_40;
    if (param_1 == (Matrix *)0x0) {
      AEMath::MatrixTransformVector(local_88,(Vector *)this);
      AEMath::Vector::operator=((Vector *)&local_34,(Vector *)local_88);
      AEMath::MatrixRotateVector(local_88,(Vector *)this);
    }
    else {
      AEMath::operator*(local_88,param_1);
      AEMath::MatrixTransformVector(aMStack_4c,(Vector *)local_88);
      AEMath::Vector::operator=((Vector *)&local_34,(Vector *)aMStack_4c);
      AEMath::operator*(local_88,param_1);
      pVVar3 = (Vector *)aMStack_4c;
      AEMath::MatrixRotateVector((Matrix *)pVVar3,(Vector *)local_88);
    }
    AEMath::Vector::operator=((Vector *)&local_40,pVVar3);
    uVar1 = CONCAT44(fStack_3c,local_40);
    uVar9 = FloatVectorNeg(uVar1,2,2);
    uVar6 = FloatVectorCompareGreaterThan(uVar1,0,2);
    lVar7 = VectorBitwiseSelect(uVar6,uVar1,uVar9);
    if ((float)((ulonglong)lVar7 >> 0x20) < (float)lVar7) {
      lVar7 = lVar7 << 0x20;
    }
    fVar5 = -local_38;
    if (0.0 < local_38) {
      fVar5 = local_38;
    }
    fVar8 = (float)((ulonglong)lVar7 >> 0x20);
    if (fVar5 < fVar8) {
      fVar5 = fVar8;
    }
    uVar2 = CameraIsSphereinViewFrustum
                      ((Vector *)&local_34,*(float *)(this + 0xe4) * fVar5,
                       (Matrix *)(*(float *)(this + 0xe4) * fVar5),(Camera *)0x0);
  }
  if (*piVar4 != local_28) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(uVar2);
  }
  return;
}

```

## target disasm
```
  00087478: push {r4,r5,r6,r7,lr}
  0008747a: add r7,sp,#0xc
  0008747c: push {r8,r9,r10,r11}
  00087480: sub sp,#0x64
  00087482: mov r5,r0
  00087484: ldr r0,[0x000875a4]
  00087486: mov r11,r2
  00087488: mov r6,r1
  0008748a: add r0,pc
  0008748c: cmp r2,#0x0
  0008748e: ldr r4,[r0,#0x0]
  00087490: ldr r0,[r4,#0x0]
  00087492: str r0,[sp,#0x60]
  00087494: beq 0x00087508
  00087496: ldrb.w r0,[r5,#0x17c]
  0008749a: cbz r0,0x00087508
  0008749c: ldr r0,[r5,#0x3c]
  0008749e: cmp r0,#0x1
  000874a0: bne 0x000874aa
  000874a2: ldr r0,[r5,#0x40]
  000874a4: ldr r0,[r0,#0x0]
  000874a6: ldrh r0,[r0,#0x2]
  000874a8: cbz r0,0x00087508
  000874aa: movs r0,#0x0
  000874ac: add.w r8,r5,#0xd4
  000874b0: strd r0,r0,[sp,#0x54]
  000874b4: cmp r6,#0x0
  000874b6: str r0,[sp,#0x5c]
  000874b8: ldr.w r0,[r5,#0xe0]
  000874bc: strd r0,r0,[sp,#0x48]
  000874c0: str r0,[sp,#0x50]
  000874c2: beq 0x0008750c
  000874c4: mov r9,sp
  000874c6: mov r1,r6
  000874c8: mov r0,r9
  000874ca: mov r2,r5
  000874cc: blx 0x0006f82c
  000874d0: add.w r10,sp,#0x3c
  000874d4: mov r1,r9
  000874d6: mov r2,r8
  000874d8: mov r0,r10
  000874da: blx 0x0006f688
  000874de: add r0,sp,#0x54
  000874e0: mov r1,r10
  000874e2: blx 0x0006eb3c
  000874e6: mov r8,sp
  000874e8: mov r1,r6
  000874ea: mov r0,r8
  000874ec: mov r2,r5
  000874ee: blx 0x0006f82c
  000874f2: add.w r9,sp,#0x3c
  000874f6: add r6,sp,#0x48
  000874f8: mov r1,r8
  000874fa: mov r0,r9
  000874fc: mov r2,r6
  000874fe: blx 0x0006f694
  00087502: mov r0,r6
  00087504: mov r1,r9
  00087506: b 0x00087532
  00087508: movs r0,#0x1
  0008750a: b 0x00087590
  0008750c: mov r6,sp
  0008750e: mov r1,r5
  00087510: mov r0,r6
  00087512: mov r2,r8
  00087514: blx 0x0006f688
  00087518: add r0,sp,#0x54
  0008751a: mov r1,r6
  0008751c: blx 0x0006eb3c
  00087520: add r6,sp,#0x48
  00087522: mov r8,sp
  00087524: mov r0,r8
  00087526: mov r1,r5
  00087528: mov r2,r6
  0008752a: blx 0x0006f694
  0008752e: mov r0,r6
  00087530: mov r1,r8
  00087532: blx 0x0006eb3c
  00087536: add r0,sp,#0x48
  00087538: movs r2,#0x0
  0008753a: mov r3,r11
  0008753c: vld1.32 {d16},[r0:64]!
  00087540: vneg.f32 d17,d16
  00087544: vcgt.f32 d2,d16,#0
  00087548: vldr.32 s0,[r0]
  0008754c: vbsl d2,d16,d17
  00087550: add r0,sp,#0x54
  00087552: vneg.f32 s2,s0
  00087556: vcmpe.f32 s4,s5
  0008755a: vmrs apsr,fpscr
  0008755e: vcmpe.f32 s0,#0
  00087562: it gt
  00087564: vmov.gt.f32 s5,s4
  00087568: vmrs apsr,fpscr
  0008756c: it gt
  0008756e: vmov.gt.f32 s2,s0
  00087572: vcmpe.f32 s5,s2
  00087576: vmrs apsr,fpscr
  0008757a: it gt
  0008757c: vmov.gt.f32 s2,s5
  00087580: vldr.32 s0,[r5,#0xe4]
  00087584: vmul.f32 s0,s0,s2
  00087588: vmov r1,s0
  0008758c: blx 0x0006f874
  00087590: ldr r1,[sp,#0x60]
  00087592: ldr r2,[r4,#0x0]
  00087594: subs r1,r2,r1
  00087596: ittt eq
  00087598: add.eq sp,#0x64
  0008759a: pop.eq.w {r8,r9,r10,r11}
  0008759e: pop.eq {r4,r5,r6,r7,pc}
  000875a0: blx 0x0006e824
```
