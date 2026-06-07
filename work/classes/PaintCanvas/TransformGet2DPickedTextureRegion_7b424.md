# PaintCanvas::TransformGet2DPickedTextureRegion
elf 0x7b424 body 354
Sig: undefined __stdcall TransformGet2DPickedTextureRegion(Transform * param_1, int param_2, int param_3, int param_4, int param_5)

## decompile
```c

/* AbyssEngine::PaintCanvas::TransformGet2DPickedTextureRegion(AbyssEngine::Transform*, int, int,
   int, int) */

void AbyssEngine::PaintCanvas::TransformGet2DPickedTextureRegion
               (Transform *param_1,int param_2,int param_3,int param_4,int param_5)

{
  uint uVar1;
  undefined4 uVar2;
  int *piVar3;
  uint uVar4;
  uint in_fpscr;
  float fVar5;
  float in_s1;
  float extraout_s1;
  Mesh *pMVar6;
  undefined4 uVar7;
  float fVar8;
  int in_stack_00000004;
  uint in_stack_00000008;
  Matrix aMStack_a0 [12];
  float local_94;
  float local_90;
  undefined4 local_8c;
  Vector aVStack_88 [60];
  int local_4c;
  
  piVar3 = *(int **)(DAT_0008b58c + 0x8b446);
  uVar4 = 0;
  fVar5 = (float)VectorSignedToFloat((in_stack_00000004 >> (in_stack_00000008 & 0xff)) >>
                                     (in_stack_00000008 & 0xff),(byte)(in_fpscr >> 0x16) & 3);
  pMVar6 = (Mesh *)VectorSignedToFloat((param_4 >> (in_stack_00000008 & 0xff)) >>
                                       (in_stack_00000008 & 0xff),(byte)(in_fpscr >> 0x16) & 3);
  local_4c = *piVar3;
  do {
    do {
      if (*(uint *)(param_3 + 0x3c) <= uVar4) {
        uVar4 = 0;
        uVar7 = VectorSignedToFloat(in_stack_00000004,(byte)(in_fpscr >> 0x16) & 3);
        fVar5 = (float)VectorSignedToFloat(param_5,(byte)(in_fpscr >> 0x16) & 3);
        fVar8 = (float)VectorSignedToFloat(param_4,(byte)(in_fpscr >> 0x16) & 3);
        goto LAB_0008b4d8;
      }
      MeshIntersect((AbyssEngine *)param_1,fVar5,in_s1,pMVar6);
      fVar5 = *(float *)param_1;
      uVar4 = uVar4 + 1;
      uVar1 = in_fpscr & 0xfffffff;
      in_fpscr = uVar1 | (uint)(fVar5 == -1.0) << 0x1e;
      in_s1 = extraout_s1;
    } while ((byte)(in_fpscr >> 0x1e) != 0);
    fVar5 = *(float *)(param_1 + 4);
    in_fpscr = uVar1 | (uint)(fVar5 == -1.0) << 0x1e;
  } while ((byte)(in_fpscr >> 0x1e) != 0);
  goto LAB_0008b568;
  while( true ) {
    __aeabi_memcpy(aVStack_88,*(undefined4 *)(*(int *)(param_3 + 0x50) + uVar4 * 4),0x3c);
    local_94 = fVar8;
    local_90 = fVar5;
    local_8c = uVar7;
    AEMath::MatrixInverseTransformVector(aMStack_a0,aVStack_88);
    AEMath::Vector::operator=((Vector *)&local_94,(Vector *)aMStack_a0);
    TransformGet2DPickedTextureRegion
              (param_1,param_2,*(int *)(*(int *)(param_3 + 0x50) + uVar4 * 4),(int)local_94,
               (int)local_90);
    uVar4 = uVar4 + 1;
    if ((*(float *)param_1 != -1.0) && (*(float *)(param_1 + 4) != -1.0)) break;
LAB_0008b4d8:
    uVar2 = DAT_0008b588;
    if (*(uint *)(param_3 + 0x4c) <= uVar4) {
      *(undefined4 *)param_1 = DAT_0008b588;
      *(undefined4 *)(param_1 + 4) = uVar2;
      break;
    }
  }
LAB_0008b568:
  if (*piVar3 == local_4c) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

```

## target disasm
```
  0008b424: push {r4,r5,r6,r7,lr}
  0008b426: add r7,sp,#0xc
  0008b428: push {r7,r8,r9,r10,r11}
  0008b42c: vpush {d8,d9,d10,d11}
  0008b430: sub sp,#0x68
  0008b432: mov r5,r2
  0008b434: ldrd r2,r6,[r7,#0xc]
  0008b438: mov r4,r0
  0008b43a: ldr r0,[0x0008b58c]
  0008b43c: mov r8,r1
  0008b43e: asr.w r1,r3,r6
  0008b442: add r0,pc
  0008b444: asrs r2,r6
  0008b446: mov r9,r3
  0008b448: asrs r2,r6
  0008b44a: ldr r3,[r0,#0x0]
  0008b44c: asr.w r0,r1,r6
  0008b450: vmov s0,r2
  0008b454: mov.w r11,#0x0
  0008b458: vmov s2,r0
  0008b45c: vcvt.f32.s32 s0,s0
  0008b460: vcvt.f32.s32 s2,s2
  0008b464: str r3,[sp,#0xc]
  0008b466: vmov.f32 s16,0xbf800000
  0008b46a: ldr r0,[r3,#0x0]
  0008b46c: str r0,[sp,#0x64]
  0008b46e: vmov r6,s0
  0008b472: vmov r10,s2
  0008b476: ldr r0,[r5,#0x3c]
  0008b478: cmp r11,r0
  0008b47a: bcs 0x0008b4ae
  0008b47c: ldr r0,[r5,#0x40]
  0008b47e: mov r1,r10
  0008b480: mov r2,r6
  0008b482: ldr.w r3,[r0,r11,lsl #0x2]
  0008b486: mov r0,r4
  0008b488: blx 0x0006fd54
  0008b48c: vldr.32 s0,[r4]
  0008b490: add.w r11,r11,#0x1
  0008b494: vcmpe.f32 s0,s16
  0008b498: vmrs apsr,fpscr
  0008b49c: beq 0x0008b476
  0008b49e: vldr.32 s0,[r4,#0x4]
  0008b4a2: vcmpe.f32 s0,s16
  0008b4a6: vmrs apsr,fpscr
  0008b4aa: beq 0x0008b476
  0008b4ac: b 0x0008b568
  0008b4ae: ldr r0,[r7,#0xc]
  0008b4b0: add.w r10,sp,#0x10
  0008b4b4: add r6,sp,#0x1c
  0008b4b6: mov.w r11,#0x0
  0008b4ba: vmov s0,r0
  0008b4be: ldr r0,[r7,#0x8]
  0008b4c0: vcvt.f32.s32 s18,s0
  0008b4c4: vmov s0,r0
  0008b4c8: vcvt.f32.s32 s20,s0
  0008b4cc: vmov s0,r9
  0008b4d0: add.w r9,sp,#0x28
  0008b4d4: vcvt.f32.s32 s22,s0
  0008b4d8: ldr r0,[r5,#0x4c]
  0008b4da: cmp r11,r0
  0008b4dc: bcs 0x0008b562
  0008b4de: ldr r0,[r5,#0x50]
  0008b4e0: movs r2,#0x3c
  0008b4e2: ldr.w r1,[r0,r11,lsl #0x2]
  0008b4e6: mov r0,r9
  0008b4e8: blx 0x0006f1e4
  0008b4ec: mov r0,r10
  0008b4ee: mov r1,r9
  0008b4f0: mov r2,r6
  0008b4f2: vstr.32 s20,[sp,#0x20]
  0008b4f6: vstr.32 s22,[sp,#0x1c]
  0008b4fa: vstr.32 s18,[sp,#0x24]
  0008b4fe: blx 0x0006f4b4
  0008b502: mov r0,r6
  0008b504: mov r1,r10
  0008b506: blx 0x0006eb3c
  0008b50a: ldr r0,[r5,#0x50]
  0008b50c: mov r1,r8
  0008b50e: vldr.32 s0,[sp,#0x1c]
  0008b512: vldr.32 s2,[sp,#0x20]
  0008b516: vldr.32 s4,[sp,#0x24]
  0008b51a: ldr.w r2,[r0,r11,lsl #0x2]
  0008b51e: vcvt.s32.f32 s4,s4
  0008b522: vcvt.s32.f32 s2,s2
  0008b526: ldr r0,[r7,#0x10]
  0008b528: vcvt.s32.f32 s0,s0
  0008b52c: str r0,[sp,#0x8]
  0008b52e: mov r0,r4
  0008b530: vstr.32 s4,[sp,#0x4]
  0008b534: vstr.32 s2,[sp]
  0008b538: vmov r3,s0
  0008b53c: blx 0x0006fd60
  0008b540: vldr.32 s0,[r4]
  0008b544: add.w r11,r11,#0x1
  0008b548: vcmpe.f32 s0,s16
  0008b54c: vmrs apsr,fpscr
  0008b550: beq 0x0008b4d8
  0008b552: vldr.32 s0,[r4,#0x4]
  0008b556: vcmpe.f32 s0,s16
  0008b55a: vmrs apsr,fpscr
  0008b55e: beq 0x0008b4d8
  0008b560: b 0x0008b568
  0008b562: ldr r0,[0x0008b588]
  0008b564: strd r0,r0,[r4,#0x0]
  0008b568: ldr r0,[sp,#0x64]
  0008b56a: ldr r1,[sp,#0xc]
  0008b56c: ldr r1,[r1,#0x0]
  0008b56e: subs r0,r1,r0
  0008b570: itttt eq
  0008b572: add.eq sp,#0x68
  0008b574: vpop.eq {d8,d9,d10,d11}
  0008b578: add.eq sp,#0x4
  0008b57a: pop.eq.w {r8,r9,r10,r11}
  0008b57e: it eq
  0008b580: pop.eq {r4,r5,r6,r7,pc}
  0008b582: blx 0x0006e824
```
