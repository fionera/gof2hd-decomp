# AbyssEngine::CameraIsPointinViewFrustum
elf 0x7144c body 374
Sig: undefined __stdcall CameraIsPointinViewFrustum(Vector * param_1, Matrix * param_2, Camera * param_3)

## decompile
```c

/* AbyssEngine::CameraIsPointinViewFrustum(AbyssEngine::AEMath::Vector const&,
   AbyssEngine::AEMath::Matrix*, AbyssEngine::Camera*) */

void AbyssEngine::CameraIsPointinViewFrustum(Vector *param_1,Matrix *param_2,Camera *param_3)

{
  bool bVar1;
  float fVar2;
  float fVar3;
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
  
  piVar6 = *(int **)(DAT_000815c8 + 0x8146a);
  local_34 = *piVar6;
  if (**(char **)(DAT_000815c4 + 0x81466) == '\0') {
    bVar1 = true;
  }
  else {
    pMVar5 = (Matrix *)(param_3 + 0xc);
    local_40 = 0;
    uStack_3c = 0;
    local_38 = 0;
    if (param_2 == (Matrix *)0x0) {
      pMVar4 = aMStack_80;
    }
    else {
      __aeabi_memcpy(aMStack_80,pMVar5,0x3c);
      AEMath::Matrix::operator*=(aMStack_80,param_2);
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
    if ((*(float *)(param_3 + 8) < fVar2) ||
       ((int)((uint)(fVar2 < *(float *)(param_3 + 4)) << 0x1f) < 0)) {
      bVar1 = false;
    }
    else {
      AEMath::MatrixGetUp((Matrix *)aAStack_98);
      AEMath::VectorNormalize((AEMath *)aMStack_8c,(Vector *)aAStack_98);
      fVar3 = (float)AEMath::VectorDot((Vector *)aMStack_80,(Vector *)aMStack_8c);
      bVar1 = false;
      fVar7 = fVar2 * *(float *)(param_3 + 0x48);
      if ((fVar3 <= fVar7) &&
         (-1 < (int)((uint)(fVar3 < -(fVar2 * *(float *)(param_3 + 0x48))) << 0x1f))) {
        AEMath::MatrixGetRight((Matrix *)aAStack_98);
        AEMath::VectorNormalize((AEMath *)aMStack_8c,(Vector *)aAStack_98);
        fVar2 = (float)AEMath::VectorDot((Vector *)aMStack_80,(Vector *)aMStack_8c);
        bVar1 = -1 < (int)((uint)(fVar2 < -(fVar7 * *(float *)(param_3 + 0x50))) << 0x1f) &&
                fVar2 <= fVar7 * *(float *)(param_3 + 0x50);
      }
    }
  }
  if (*piVar6 != local_34) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(bVar1);
  }
  return;
}

```

## target disasm
```
  0008144c: push {r4,r5,r6,r7,lr}
  0008144e: add r7,sp,#0xc
  00081450: push {r8,r9,r10}
  00081454: vpush {d8,d9}
  00081458: sub sp,#0x78
  0008145a: mov r6,r0
  0008145c: ldr r0,[0x000815c4]
  0008145e: mov r4,r1
  00081460: ldr r1,[0x000815c8]
  00081462: add r0,pc
  00081464: mov r8,r2
  00081466: add r1,pc
  00081468: ldr r0,[r0,#0x0]
  0008146a: ldr.w r10,[r1,#0x0]
  0008146e: ldrb r0,[r0,#0x0]
  00081470: ldr.w r1,[r10,#0x0]
  00081474: cmp r0,#0x0
  00081476: str r1,[sp,#0x74]
  00081478: beq 0x000814a4
  0008147a: add.w r9,r8,#0xc
  0008147e: movs r0,#0x0
  00081480: cmp r4,#0x0
  00081482: strd r0,r0,[sp,#0x68]
  00081486: str r0,[sp,#0x70]
  00081488: beq 0x000814a8
  0008148a: add r5,sp,#0x28
  0008148c: mov r1,r9
  0008148e: movs r2,#0x3c
  00081490: mov r0,r5
  00081492: blx 0x0006f1e4
  00081496: mov r0,r5
  00081498: mov r1,r4
  0008149a: blx 0x0006f4a8
  0008149e: add r4,sp,#0x1c
  000814a0: mov r1,r5
  000814a2: b 0x000814ac
  000814a4: movs r0,#0x1
  000814a6: b 0x0008151a
  000814a8: add r4,sp,#0x28
  000814aa: mov r1,r9
  000814ac: mov r0,r4
  000814ae: mov r2,r6
  000814b0: blx 0x0006f4b4
  000814b4: add r0,sp,#0x68
  000814b6: mov r1,r4
  000814b8: blx 0x0006eb3c
  000814bc: add r4,sp,#0x1c
  000814be: mov r1,r9
  000814c0: mov r0,r4
  000814c2: blx 0x0006f16c
  000814c6: add r5,sp,#0x28
  000814c8: mov r1,r6
  000814ca: mov r2,r4
  000814cc: mov r0,r5
  000814ce: blx 0x0006ec38
  000814d2: add r4,sp,#0x4
  000814d4: mov r1,r9
  000814d6: mov r0,r4
  000814d8: blx 0x0006f4c0
  000814dc: add r6,sp,#0x10
  000814de: mov r1,r4
  000814e0: mov r0,r6
  000814e2: blx 0x0006f4cc
  000814e6: add r4,sp,#0x1c
  000814e8: mov r1,r6
  000814ea: mov r0,r4
  000814ec: blx 0x0006ec80
  000814f0: mov r0,r5
  000814f2: mov r1,r4
  000814f4: blx 0x0006ec68
  000814f8: vmov s16,r0
  000814fc: vldr.32 s0,[r8,#0x8]
  00081500: vcmpe.f32 s16,s0
  00081504: vmrs apsr,fpscr
  00081508: bgt 0x00081518
  0008150a: vldr.32 s0,[r8,#0x4]
  0008150e: vcmpe.f32 s16,s0
  00081512: vmrs apsr,fpscr
  00081516: bpl 0x00081534
  00081518: movs r0,#0x0
  0008151a: ldr r1,[sp,#0x74]
  0008151c: ldr.w r2,[r10,#0x0]
  00081520: subs r1,r2,r1
  00081522: itttt eq
  00081524: add.eq sp,#0x78
  00081526: vpop.eq {d8,d9}
  0008152a: pop.eq.w {r8,r9,r10}
  0008152e: pop.eq {r4,r5,r6,r7,pc}
  00081530: blx 0x0006e824
  00081534: add r4,sp,#0x10
  00081536: mov r1,r9
  00081538: mov r0,r4
  0008153a: blx 0x0006f4d8
  0008153e: add r5,sp,#0x1c
  00081540: mov r1,r4
  00081542: mov r0,r5
  00081544: blx 0x0006ec80
  00081548: add r0,sp,#0x28
  0008154a: mov r1,r5
  0008154c: blx 0x0006ec68
  00081550: vldr.32 s0,[r8,#0x48]
  00081554: vmov s2,r0
  00081558: movs r0,#0x0
  0008155a: vmul.f32 s18,s16,s0
  0008155e: vcmpe.f32 s2,s18
  00081562: vmrs apsr,fpscr
  00081566: bgt 0x0008151a
  00081568: vnmul.f32 s0,s16,s0
  0008156c: vcmpe.f32 s2,s0
  00081570: vmrs apsr,fpscr
  00081574: bmi 0x0008151a
  00081576: add r4,sp,#0x10
  00081578: mov r1,r9
  0008157a: mov r0,r4
  0008157c: blx 0x0006f4e4
  00081580: add r5,sp,#0x1c
  00081582: mov r1,r4
  00081584: mov r0,r5
  00081586: blx 0x0006ec80
  0008158a: add r0,sp,#0x28
  0008158c: mov r1,r5
  0008158e: blx 0x0006ec68
  00081592: vldr.32 s0,[r8,#0x50]
  00081596: vmov s4,r0
  0008159a: movs r1,#0x0
  0008159c: movs r0,#0x0
  0008159e: vmul.f32 s2,s18,s0
  000815a2: vnmul.f32 s0,s18,s0
  000815a6: vcmpe.f32 s4,s2
  000815aa: vmrs apsr,fpscr
  000815ae: vcmpe.f32 s4,s0
  000815b2: it le
  000815b4: mov.le r1,#0x1
  000815b6: vmrs apsr,fpscr
  000815ba: it pl
  000815bc: mov.pl r0,#0x1
  000815be: ands r0,r1
  000815c0: b 0x0008151a
```
