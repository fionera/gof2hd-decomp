# PlayerFixedObject::setPosition
elf 0x153eec body 194
Sig: undefined __stdcall setPosition(float param_1, float param_2, float param_3)

## decompile
```c

/* PlayerFixedObject::setPosition(float, float, float) */

void PlayerFixedObject::setPosition(float param_1,float param_2,float param_3)

{
  int in_r0;
  int iVar1;
  Matrix *pMVar2;
  uint *puVar3;
  int *piVar4;
  float in_r1;
  float in_r2;
  float in_r3;
  uint uVar5;
  int *piVar6;
  float extraout_s0;
  float extraout_s0_00;
  float fVar7;
  float extraout_s1;
  float extraout_s1_00;
  float fVar8;
  float extraout_s2;
  float extraout_s2_00;
  float fVar9;
  
  piVar6 = *(int **)(DAT_00163fb0 + 0x163f04);
  iVar1 = *piVar6;
  *(float *)(in_r0 + 0x58) = in_r1;
  *(float *)(in_r0 + 0x5c) = in_r2;
  *(float *)(in_r0 + 0x60) = in_r3;
  *(int *)(in_r0 + 0x178) = (int)in_r1;
  *(int *)(in_r0 + 0x17c) = (int)in_r2;
  *(int *)(in_r0 + 0x180) = (int)in_r3;
  AEGeometry::setPosition((float)(int)in_r3,param_2,(float)(int)in_r2);
  pMVar2 = (Matrix *)AEGeometry::getMatrix();
  AbyssEngine::AEMath::Matrix::operator=((Matrix *)(*(int *)(in_r0 + 4) + 4),pMVar2);
  AEGeometry::getPosition();
  AbyssEngine::AEMath::Vector::operator=((Vector *)(in_r0 + 0x2c),(Vector *)&stack0xffffffcc);
  puVar3 = *(uint **)(in_r0 + 0x128);
  fVar7 = extraout_s0;
  fVar8 = extraout_s1;
  fVar9 = extraout_s2;
  if (puVar3 != (uint *)0x0) {
    for (uVar5 = 0; uVar5 < *puVar3; uVar5 = uVar5 + 1) {
      piVar4 = *(int **)(puVar3[1] + uVar5 * 4);
      (**(code **)(*piVar4 + 4))
                (piVar4,*(undefined4 *)(in_r0 + 0x2c),*(undefined4 *)(in_r0 + 0x30),
                 *(undefined4 *)(in_r0 + 0x34));
      puVar3 = *(uint **)(in_r0 + 0x128);
      fVar7 = extraout_s0_00;
      fVar8 = extraout_s1_00;
      fVar9 = extraout_s2_00;
    }
  }
  if (*(int *)(in_r0 + 0x124) != 0) {
    AEGeometry::setPosition(fVar7,fVar8,fVar9);
  }
  if (*piVar6 != iVar1) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  00163eec: push {r4,r5,r6,r7,lr}
  00163eee: add r7,sp,#0xc
  00163ef0: push {r3,r4,r5,r6,r7,r8,r9,r10,r11}
  00163ef4: mov r4,r0
  00163ef6: ldr r0,[0x00163fb0]
  00163ef8: vmov s0,r3
  00163efc: add.w r12,r4,#0x58
  00163f00: add r0,pc
  00163f02: vmov s2,r2
  00163f06: vmov s4,r1
  00163f0a: mov r8,r3
  00163f0c: ldr.w r10,[r0,#0x0]
  00163f10: mov r9,r2
  00163f12: mov r11,r1
  00163f14: ldr.w r0,[r10,#0x0]
  00163f18: str r0,[sp,#0x10]
  00163f1a: vcvt.s32.f32 s2,s2
  00163f1e: vcvt.s32.f32 s4,s4
  00163f22: vcvt.s32.f32 s0,s0
  00163f26: ldr r0,[r4,#0x8]
  00163f28: stm.w r12!,{r1,r2,r3}
  00163f2c: vstr.32 s4,[r4,#0x178]
  00163f30: vstr.32 s2,[r4,#0x17c]
  00163f34: vstr.32 s0,[r4,#0x180]
  00163f38: blx 0x00073048
  00163f3c: ldr r0,[r4,#0x8]
  00163f3e: blx 0x000721cc
  00163f42: mov r1,r0
  00163f44: ldr r0,[r4,#0x4]
  00163f46: adds r0,#0x4
  00163f48: blx 0x0006f148
  00163f4c: add r5,sp,#0x4
  00163f4e: ldr r1,[r4,#0x8]
  00163f50: mov r0,r5
  00163f52: blx 0x000720b8
  00163f56: add.w r0,r4,#0x2c
  00163f5a: mov r1,r5
  00163f5c: blx 0x0006eb3c
  00163f60: ldr.w r0,[r4,#0x128]
  00163f64: cbz r0,0x00163f88
  00163f66: movs r5,#0x0
  00163f68: b 0x00163f82
  00163f6a: ldr r0,[r0,#0x4]
  00163f6c: add.w r3,r4,#0x2c
  00163f70: ldr.w r0,[r0,r5,lsl #0x2]
  00163f74: ldr r6,[r0,#0x0]
  00163f76: ldmia r3,{r1,r2,r3}
  00163f78: ldr r6,[r6,#0x4]
  00163f7a: blx r6
  00163f7c: ldr.w r0,[r4,#0x128]
  00163f80: adds r5,#0x1
  00163f82: ldr r1,[r0,#0x0]
  00163f84: cmp r5,r1
  00163f86: bcc 0x00163f6a
  00163f88: ldr.w r0,[r4,#0x124]
  00163f8c: cbz r0,0x00163f98
  00163f8e: mov r1,r11
  00163f90: mov r2,r9
  00163f92: mov r3,r8
  00163f94: blx 0x00073048
  00163f98: ldr r0,[sp,#0x10]
  00163f9a: ldr.w r1,[r10,#0x0]
  00163f9e: subs r0,r1,r0
  00163fa0: ittt eq
  00163fa2: add.eq sp,#0x14
  00163fa4: pop.eq.w {r8,r9,r10,r11}
  00163fa8: pop.eq {r4,r5,r6,r7,pc}
  00163faa: blx 0x0006e824
```
