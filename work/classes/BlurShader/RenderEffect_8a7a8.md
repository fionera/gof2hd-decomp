# BlurShader::RenderEffect
elf 0x8a7a8 body 748
Sig: undefined __stdcall RenderEffect(FBOContainer * param_1, FBOContainer * * param_2, Engine * param_3, float param_4, Vector param_5)

## decompile
```c

/* AbyssEngine::BlurShader::RenderEffect(AbyssEngine::FBOContainer*, AbyssEngine::FBOContainer*&,
   AbyssEngine::Engine*, float, AbyssEngine::AEMath::Vector) */

void AbyssEngine::BlurShader::RenderEffect
               (int param_1,undefined4 param_2,int *param_3,Matrix *param_4,undefined4 param_5,
               undefined4 param_6,undefined4 param_7,undefined4 param_8)

{
  undefined4 uVar1;
  undefined4 *puVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  int iVar5;
  int *piVar6;
  uint in_fpscr;
  float fVar7;
  float fVar8;
  undefined4 extraout_s1;
  undefined4 extraout_s1_00;
  undefined4 extraout_s1_01;
  undefined4 extraout_s1_02;
  undefined4 extraout_s1_03;
  undefined4 extraout_s1_04;
  undefined4 extraout_s1_05;
  undefined4 extraout_s1_06;
  undefined8 uVar9;
  undefined4 extraout_s3;
  undefined4 extraout_s3_00;
  undefined4 extraout_s3_01;
  undefined4 extraout_s3_02;
  undefined4 extraout_s3_03;
  undefined4 extraout_s3_04;
  undefined4 extraout_s3_05;
  undefined4 extraout_s3_06;
  undefined4 uVar10;
  float fVar11;
  undefined4 uVar12;
  undefined4 local_90 [5];
  undefined4 local_7c;
  undefined4 local_78;
  undefined4 uStack_74;
  undefined4 uStack_70;
  undefined4 uStack_6c;
  undefined8 uStack_68;
  undefined8 uStack_60;
  undefined4 local_58;
  undefined4 local_50;
  undefined4 uStack_4c;
  undefined4 uStack_48;
  int local_44;
  
  uVar4 = 0;
  uVar3 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  uVar10 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  uVar12 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  piVar6 = *(int **)(DAT_0009aab4 + 0x9a7c4);
  local_44 = *piVar6;
  local_50 = param_6;
  uStack_4c = param_7;
  uStack_48 = param_8;
  *(undefined4 *)(param_4 + 0x3b4) = 0;
  *(undefined4 *)(param_4 + 0x3b8) = uVar3;
  *(undefined4 *)(param_4 + 0x3bc) = uVar10;
  *(undefined4 *)(param_4 + 0x3c0) = uVar12;
  *(undefined4 *)(param_4 + 0x3a4) = 0;
  *(undefined4 *)(param_4 + 0x3a8) = uVar3;
  *(undefined4 *)(param_4 + 0x3ac) = uVar10;
  *(undefined4 *)(param_4 + 0x3b0) = uVar12;
  *(undefined4 *)(param_4 + 0x394) = 0;
  *(undefined4 *)(param_4 + 0x398) = uVar3;
  *(undefined4 *)(param_4 + 0x39c) = uVar10;
  *(undefined4 *)(param_4 + 0x3a0) = uVar12;
  *(undefined4 *)(param_4 + 900) = 0;
  *(undefined4 *)(param_4 + 0x388) = uVar3;
  *(undefined4 *)(param_4 + 0x38c) = uVar10;
  *(undefined4 *)(param_4 + 0x390) = uVar12;
  uVar1 = Engine::GetDisplayWidth();
  fVar7 = (float)VectorSignedToFloat(uVar1,(byte)(in_fpscr >> 0x16) & 3);
  *(float *)(param_4 + 900) = 2.0 / fVar7;
  uVar1 = Engine::GetDisplayHeight();
  uStack_60 = DAT_0009aaa0;
  uStack_68 = DAT_0009aa98;
  fVar7 = (float)VectorSignedToFloat(uVar1,(byte)(in_fpscr >> 0x16) & 3);
  *(undefined4 *)(param_4 + 0x3ac) = DAT_0009aaac;
  uVar1 = DAT_0009aaa8;
  *(undefined4 *)(param_4 + 0x3c0) = 0x3f800000;
  *(undefined4 *)(param_4 + 0x3b4) = uVar1;
  *(undefined4 *)(param_4 + 0x3b8) = 0x3f800000;
  puVar2 = (undefined4 *)((uint)local_90 | 4);
  *(float *)(param_4 + 0x398) = -(2.0 / fVar7);
  *puVar2 = uVar4;
  puVar2[1] = uVar3;
  puVar2[2] = uVar10;
  puVar2[3] = uVar12;
  local_90[0] = 0x3f800000;
  local_7c = 0x3f800000;
  local_58 = 0x3f800000;
  local_78 = uVar4;
  uStack_74 = uVar3;
  uStack_70 = uVar10;
  uStack_6c = uVar12;
  Engine::SetWorldViewMatrix(param_4);
  glDisable(0xb71);
  glDepthMask(0);
  glDisable(0xbe2);
  glUseProgram(*(undefined4 *)(param_1 + 0x24));
  glActiveTexture(0x84c0);
  FBOContainer::Activate();
  if ((FBOContainer *)*param_3 == (FBOContainer *)0x0) {
    glBindFramebuffer(0x8d40,*(undefined4 *)(param_4 + 0x40c));
    if (*(int *)(**(int **)(param_4 + 0x30) + 0x30) == 2) {
      uVar4 = Engine::GetDisplayWidth();
      uVar3 = Engine::GetDisplayHeight();
    }
    else {
      uVar4 = Engine::GetDisplayHeight();
      uVar3 = Engine::GetDisplayWidth();
    }
    glViewport(0,0,uVar4,uVar3);
    uVar4 = extraout_s3_00;
    uVar3 = extraout_s1_00;
  }
  else {
    FBOContainer::BeginCapture((FBOContainer *)*param_3);
    uVar4 = extraout_s3;
    uVar3 = extraout_s1;
  }
  if (-1 < *(int *)(param_1 + 0x28)) {
    glEnableVertexAttribArray();
    uVar4 = extraout_s3_01;
    uVar3 = extraout_s1_01;
  }
  if (-1 < *(int *)(param_1 + 0x30)) {
    glEnableVertexAttribArray();
    uVar4 = extraout_s3_02;
    uVar3 = extraout_s1_02;
  }
  if (-1 < *(int *)(param_1 + 0x2c)) {
    glUniformMatrix4fv(*(int *)(param_1 + 0x2c),1,0,param_4 + 0x104);
    uVar4 = extraout_s3_03;
    uVar3 = extraout_s1_03;
  }
  iVar5 = *(int *)(param_1 + 0x38);
  if (-1 < iVar5) {
    if (*(int *)(**(int **)(param_4 + 0x30) + 0x30) == 2) {
      uVar4 = Engine::GetDisplayWidth();
      fVar11 = (float)VectorSignedToFloat(uVar4,(byte)(in_fpscr >> 0x16) & 3);
      fVar7 = 1.0;
      uVar4 = Engine::GetDisplayHeight();
    }
    else {
      uVar4 = Engine::GetDisplayHeight();
      fVar7 = 1.0;
      fVar11 = (float)VectorSignedToFloat(uVar4,(byte)(in_fpscr >> 0x16) & 3);
      uVar4 = Engine::GetDisplayWidth();
    }
    fVar8 = (float)VectorSignedToFloat(uVar4,(byte)(in_fpscr >> 0x16) & 3);
    glUniform2f(iVar5,fVar7 / fVar11,fVar7 / fVar8);
    uVar4 = extraout_s3_04;
    uVar3 = extraout_s1_04;
  }
  iVar5 = *(int *)(param_1 + 0x44);
  if (-1 < iVar5) {
    puVar2 = (undefined4 *)AEMath::Vector::operator[]((int)&local_50);
    uVar4 = *puVar2;
    puVar2 = (undefined4 *)AEMath::Vector::operator[]((int)&local_50);
    glUniform2f(iVar5,uVar4,*puVar2);
    uVar4 = extraout_s3_05;
    uVar3 = extraout_s1_05;
  }
  if (-1 < *(int *)(param_1 + 0x40)) {
    glUniform1f(*(int *)(param_1 + 0x40),*(undefined4 *)(param_1 + 0x58));
    uVar4 = extraout_s3_06;
    uVar3 = extraout_s1_06;
  }
  if (-1 < *(int *)(param_1 + 0x3c)) {
    uVar9 = FloatVectorMax(CONCAT44(uVar3,param_5),CONCAT44(uVar4,DAT_0009aab0),2,0x20);
    glUniform1f(*(int *)(param_1 + 0x3c),(float)uVar9 * *(float *)(param_1 + 0x5c));
  }
  if (-1 < *(int *)(param_1 + 0x28)) {
    glVertexAttribPointer
              (*(int *)(param_1 + 0x28),3,0x1406,0,0,*(undefined4 *)(*(int *)(param_4 + 0x380) + 4))
    ;
  }
  if (-1 < *(int *)(param_1 + 0x30)) {
    glVertexAttribPointer
              (*(int *)(param_1 + 0x30),2,0x1406,0,0,*(undefined4 *)(*(int *)(param_4 + 0x380) + 8))
    ;
  }
  glClear(0x4000);
  iVar5 = Engine::GetDisplayWidth();
  Engine::GetDisplayHeight();
  Engine::DrawQuad((int)param_4,0,0,iVar5);
  if (-1 < *(int *)(param_1 + 0x28)) {
    glDisableVertexAttribArray();
  }
  if (-1 < *(int *)(param_1 + 0x30)) {
    glDisableVertexAttribArray();
  }
  if (*param_3 != 0) {
    FBOContainer::EndCapture();
  }
  glEnable(0xbe2);
  glBlendFunc(0x302,0x303);
  glActiveTexture(0x84c0);
  if (*piVar6 != local_44) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  0009a7a8: push {r4,r5,r6,r7,lr}
  0009a7aa: add r7,sp,#0xc
  0009a7ac: push {r7,r8,r9,r10,r11}
  0009a7b0: vpush {d8,d9,d10}
  0009a7b4: sub sp,#0x58
  0009a7b6: mov r10,r0
  0009a7b8: ldr r0,[0x0009aab4]
  0009a7ba: mov r8,r2
  0009a7bc: add.w r2,r7,#0xc
  0009a7c0: add r0,pc
  0009a7c2: mov r4,r1
  0009a7c4: vmov.i32 q4,#0x0
  0009a7c8: add.w r12,sp,#0x48
  0009a7cc: ldr.w r11,[r0,#0x0]
  0009a7d0: mov r6,r3
  0009a7d2: ldmia r2,{r0,r1,r2}
  0009a7d4: ldr.w r3,[r11,#0x0]
  0009a7d8: stm.w r12!,{r0,r1,r2}
  0009a7dc: add.w r0,r6,#0x3b4
  0009a7e0: str r3,[sp,#0x54]
  0009a7e2: vst1.32 {d8,d9},[r0]
  0009a7e6: add.w r0,r6,#0x3a4
  0009a7ea: vst1.32 {d8,d9},[r0]
  0009a7ee: add.w r0,r6,#0x394
  0009a7f2: vst1.32 {d8,d9},[r0]
  0009a7f6: add.w r0,r6,#0x384
  0009a7fa: vst1.32 {d8,d9},[r0]
  0009a7fe: mov r0,r6
  0009a800: blx 0x0006e8cc
  0009a804: vmov s0,r0
  0009a808: mov r0,r6
  0009a80a: vmov.f32 s20,0x40000000
  0009a80e: vcvt.f32.s32 s0,s0
  0009a812: vdiv.f32 s0,s20,s0
  0009a816: vstr.32 s0,[r6,#0x384]
  0009a81a: blx 0x0006e8d8
  0009a81e: vmov s0,r0
  0009a822: adr r1,[0x9aa98]
  0009a824: vld1.64 {d16,d17},[r1]
  0009a828: mov.w r2,#0x3f800000
  0009a82c: vcvt.f32.s32 s0,s0
  0009a830: ldr r1,[0x0009aaac]
  0009a832: str.w r1,[r6,#0x3ac]
  0009a836: add r1,sp,#0x8
  0009a838: ldr r0,[0x0009aaa8]
  0009a83a: str.w r2,[r6,#0x3c0]
  0009a83e: strd r0,r2,[r6,#0x3b4]
  0009a842: orr r0,r1,#0x4
  0009a846: vdiv.f32 s0,s20,s0
  0009a84a: vneg.f32 s0,s0
  0009a84e: vstr.32 s0,[r6,#0x398]
  0009a852: vst1.32 {d8,d9},[r0]
  0009a856: add.w r0,r1,#0x18
  0009a85a: vst1.64 {d8,d9},[r0]
  0009a85e: add.w r0,r1,#0x28
  0009a862: vst1.64 {d16,d17},[r0]
  0009a866: mov r0,r6
  0009a868: str r2,[sp,#0x8]
  0009a86a: str r2,[sp,#0x1c]
  0009a86c: str r2,[sp,#0x40]
  0009a86e: blx 0x0006fae4
  0009a872: movw r0,#0xb71
  0009a876: blx 0x0006edc4
  0009a87a: movs r0,#0x0
  0009a87c: blx 0x0006eec0
  0009a880: movw r0,#0xbe2
  0009a884: blx 0x0006edc4
  0009a888: ldr.w r0,[r10,#0x24]
  0009a88c: blx 0x00070960
  0009a890: movw r0,#0x84c0
  0009a894: blx 0x0006fb8c
  0009a898: mov r0,r4
  0009a89a: blx 0x0006ee48
  0009a89e: ldr.w r0,[r8,#0x0]
  0009a8a2: cbz r0,0x0009a8aa
  0009a8a4: blx 0x0006ee54
  0009a8a8: b 0x0009a8ea
  0009a8aa: ldr.w r1,[r6,#0x40c]
  0009a8ae: movw r0,#0x8d40
  0009a8b2: blx 0x0006ee6c
  0009a8b6: ldr r0,[r6,#0x30]
  0009a8b8: ldr r0,[r0,#0x0]
  0009a8ba: ldr r0,[r0,#0x30]
  0009a8bc: cmp r0,#0x2
  0009a8be: bne 0x0009a8d0
  0009a8c0: mov r0,r6
  0009a8c2: blx 0x0006e8cc
  0009a8c6: mov r4,r0
  0009a8c8: mov r0,r6
  0009a8ca: blx 0x0006e8d8
  0009a8ce: b 0x0009a8de
  0009a8d0: mov r0,r6
  0009a8d2: blx 0x0006e8d8
  0009a8d6: mov r4,r0
  0009a8d8: mov r0,r6
  0009a8da: blx 0x0006e8cc
  0009a8de: mov r3,r0
  0009a8e0: movs r0,#0x0
  0009a8e2: movs r1,#0x0
  0009a8e4: mov r2,r4
  0009a8e6: blx 0x0006eda0
  0009a8ea: ldr.w r0,[r10,#0x28]
  0009a8ee: cmp r0,#0x0
  0009a8f0: blt 0x0009a8f6
  0009a8f2: blx 0x00070a5c
  0009a8f6: ldr.w r0,[r10,#0x30]
  0009a8fa: cmp r0,#0x0
  0009a8fc: blt 0x0009a902
  0009a8fe: blx 0x00070a5c
  0009a902: ldr.w r0,[r10,#0x2c]
  0009a906: cmp r0,#0x0
  0009a908: blt 0x0009a916
  0009a90a: add.w r3,r6,#0x104
  0009a90e: movs r1,#0x1
  0009a910: movs r2,#0x0
  0009a912: blx 0x00070990
  0009a916: ldr.w r4,[r10,#0x38]
  0009a91a: cmp r4,#0x0
  0009a91c: blt 0x0009a978
  0009a91e: ldr r0,[r6,#0x30]
  0009a920: ldr r0,[r0,#0x0]
  0009a922: ldr r0,[r0,#0x30]
  0009a924: cmp r0,#0x2
  0009a926: bne 0x0009a942
  0009a928: mov r0,r6
  0009a92a: blx 0x0006e8cc
  0009a92e: vmov s0,r0
  0009a932: mov r0,r6
  0009a934: vmov.f32 s16,0x3f800000
  0009a938: vcvt.f32.s32 s18,s0
  0009a93c: blx 0x0006e8d8
  0009a940: b 0x0009a95a
  0009a942: mov r0,r6
  0009a944: blx 0x0006e8d8
  0009a948: vmov s0,r0
  0009a94c: mov r0,r6
  0009a94e: vmov.f32 s16,0x3f800000
  0009a952: vcvt.f32.s32 s18,s0
  0009a956: blx 0x0006e8cc
  0009a95a: vmov s0,r0
  0009a95e: mov r0,r4
  0009a960: vdiv.f32 s2,s16,s18
  0009a964: vcvt.f32.s32 s0,s0
  0009a968: vmov r1,s2
  0009a96c: vdiv.f32 s0,s16,s0
  0009a970: vmov r2,s0
  0009a974: blx 0x00070c48
  0009a978: ldr.w r4,[r10,#0x44]
  0009a97c: cmp r4,#0x0
  0009a97e: blt 0x0009a9a0
  0009a980: add.w r9,sp,#0x48
  0009a984: movs r1,#0x0
  0009a986: mov r0,r9
  0009a988: blx 0x00070c84
  0009a98c: ldr r5,[r0,#0x0]
  0009a98e: mov r0,r9
  0009a990: movs r1,#0x1
  0009a992: blx 0x00070c84
  0009a996: ldr r2,[r0,#0x0]
  0009a998: mov r0,r4
  0009a99a: mov r1,r5
  0009a99c: blx 0x00070c48
  0009a9a0: ldr.w r0,[r10,#0x40]
  0009a9a4: cmp r0,#0x0
  0009a9a6: blt 0x0009a9b0
  0009a9a8: ldr.w r1,[r10,#0x58]
  0009a9ac: blx 0x00070978
  0009a9b0: ldr.w r0,[r10,#0x3c]
  0009a9b4: cmp r0,#0x0
  0009a9b6: blt 0x0009a9d4
  0009a9b8: vldr.32 s0,[r7,#0x8]
  0009a9bc: vldr.32 s2,[pc,#0xf0]
  0009a9c0: vldr.32 s4,[r10,#0x5c]
  0009a9c4: vmax.f32 d0,d0,d1
  0009a9c8: vmul.f32 s0,s0,s4
  0009a9cc: vmov r1,s0
  0009a9d0: blx 0x00070978
  0009a9d4: ldr.w r0,[r10,#0x28]
  0009a9d8: cmp r0,#0x0
  0009a9da: blt 0x0009a9f4
  0009a9dc: ldr.w r1,[r6,#0x380]
  0009a9e0: movs r2,#0x0
  0009a9e2: movs r3,#0x0
  0009a9e4: ldr r1,[r1,#0x4]
  0009a9e6: strd r2,r1,[sp,#0x0]
  0009a9ea: movs r1,#0x3
  0009a9ec: movw r2,#0x1406
  0009a9f0: blx 0x000709c0
  0009a9f4: ldr.w r0,[r10,#0x30]
  0009a9f8: cmp r0,#0x0
  0009a9fa: blt 0x0009aa14
  0009a9fc: ldr.w r1,[r6,#0x380]
  0009aa00: movs r2,#0x0
  0009aa02: movs r3,#0x0
  0009aa04: ldr r1,[r1,#0x8]
  0009aa06: strd r2,r1,[sp,#0x0]
  0009aa0a: movs r1,#0x2
  0009aa0c: movw r2,#0x1406
  0009aa10: blx 0x000709c0
  0009aa14: mov.w r0,#0x4000
  0009aa18: blx 0x0006eed8
  0009aa1c: mov r0,r6
  0009aa1e: blx 0x0006e8cc
  0009aa22: mov r4,r0
  0009aa24: mov r0,r6
  0009aa26: blx 0x0006e8d8
  0009aa2a: str r0,[sp,#0x0]
  0009aa2c: mov r0,r6
  0009aa2e: movs r1,#0x0
  0009aa30: movs r2,#0x0
  0009aa32: mov r3,r4
  0009aa34: blx 0x0006e8a8
  0009aa38: ldr.w r0,[r10,#0x28]
  0009aa3c: cmp r0,#0x0
  0009aa3e: blt 0x0009aa44
  0009aa40: blx 0x00070a44
  0009aa44: ldr.w r0,[r10,#0x30]
  0009aa48: cmp r0,#0x0
  0009aa4a: blt 0x0009aa50
  0009aa4c: blx 0x00070a44
  0009aa50: ldr.w r0,[r8,#0x0]
  0009aa54: cbz r0,0x0009aa5a
  0009aa56: blx 0x0006ee60
  0009aa5a: movw r0,#0xbe2
  0009aa5e: blx 0x0006edb8
  0009aa62: movw r0,#0x302
  0009aa66: movw r1,#0x303
  0009aa6a: blx 0x0006fe68
  0009aa6e: movw r0,#0x84c0
  0009aa72: blx 0x0006fb8c
  0009aa76: ldr r0,[sp,#0x54]
  0009aa78: ldr.w r1,[r11,#0x0]
  0009aa7c: subs r0,r1,r0
  0009aa7e: itttt eq
  0009aa80: add.eq sp,#0x58
  0009aa82: vpop.eq {d8,d9,d10}
  0009aa86: add.eq sp,#0x4
  0009aa88: pop.eq.w {r8,r9,r10,r11}
  0009aa8c: it eq
  0009aa8e: pop.eq {r4,r5,r6,r7,pc}
  0009aa90: blx 0x0006e824
```
