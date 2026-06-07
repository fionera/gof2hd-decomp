# GlowPPShader::RenderEffect
elf 0x8d120 body 1210
Sig: undefined __thiscall RenderEffect(GlowPPShader * this, FBOContainer * param_1, FBOContainer * * param_2, Engine * param_3)

## decompile
```c

/* AbyssEngine::GlowPPShader::RenderEffect(AbyssEngine::FBOContainer*, AbyssEngine::FBOContainer*&,
   AbyssEngine::Engine*) */

void __thiscall
AbyssEngine::GlowPPShader::RenderEffect
          (GlowPPShader *this,FBOContainer *param_1,FBOContainer **param_2,Engine *param_3)

{
  undefined4 uVar1;
  undefined4 *puVar2;
  int iVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  int *piVar6;
  Engine *pEVar7;
  code *pcVar8;
  int iVar9;
  code *pcVar10;
  code *pcVar11;
  uint in_fpscr;
  float fVar12;
  float fVar13;
  undefined4 uVar14;
  undefined4 uVar15;
  undefined4 local_80 [5];
  undefined4 local_6c;
  undefined4 local_68;
  undefined4 uStack_64;
  undefined4 uStack_60;
  undefined4 uStack_5c;
  undefined8 uStack_58;
  undefined8 uStack_50;
  undefined4 local_48;
  int local_44;
  
  piVar6 = *(int **)(DAT_0009d504 + 0x9d140);
  local_44 = *piVar6;
  if (**(char **)(DAT_0009d500 + 0x9d13e) != '\0') {
    **(char **)(DAT_0009d500 + 0x9d13e) = '\0';
    InternalInit((Engine *)this);
    FBOContainer::BeginCapture(*(FBOContainer **)(this + 0xa0));
    glClearColor(0,0,0,0x3f800000);
    glClear(0x4000);
    FBOContainer::EndCapture();
  }
  uVar4 = 0;
  uVar5 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  uVar14 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  uVar15 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  *(undefined4 *)(param_3 + 0x3b4) = 0;
  *(undefined4 *)(param_3 + 0x3b8) = uVar5;
  *(undefined4 *)(param_3 + 0x3bc) = uVar14;
  *(undefined4 *)(param_3 + 0x3c0) = uVar15;
  *(undefined4 *)(param_3 + 0x3a4) = 0;
  *(undefined4 *)(param_3 + 0x3a8) = uVar5;
  *(undefined4 *)(param_3 + 0x3ac) = uVar14;
  *(undefined4 *)(param_3 + 0x3b0) = uVar15;
  *(undefined4 *)(param_3 + 0x394) = 0;
  *(undefined4 *)(param_3 + 0x398) = uVar5;
  *(undefined4 *)(param_3 + 0x39c) = uVar14;
  *(undefined4 *)(param_3 + 0x3a0) = uVar15;
  *(undefined4 *)(param_3 + 900) = 0;
  *(undefined4 *)(param_3 + 0x388) = uVar5;
  *(undefined4 *)(param_3 + 0x38c) = uVar14;
  *(undefined4 *)(param_3 + 0x390) = uVar15;
  uVar1 = Engine::GetDisplayWidth();
  fVar12 = (float)VectorSignedToFloat(uVar1,(byte)(in_fpscr >> 0x16) & 3);
  *(float *)(param_3 + 900) = 2.0 / fVar12;
  uVar1 = Engine::GetDisplayHeight();
  uStack_50 = DAT_0009d5f8;
  uStack_58 = DAT_0009d5f0;
  fVar12 = (float)VectorSignedToFloat(uVar1,(byte)(in_fpscr >> 0x16) & 3);
  *(undefined4 *)(param_3 + 0x3ac) = DAT_0009d508;
  uVar1 = DAT_0009d50c;
  *(undefined4 *)(param_3 + 0x3c0) = 0x3f800000;
  *(undefined4 *)(param_3 + 0x3b4) = uVar1;
  *(undefined4 *)(param_3 + 0x3b8) = 0x3f800000;
  puVar2 = (undefined4 *)((uint)local_80 | 4);
  *(float *)(param_3 + 0x398) = -(2.0 / fVar12);
  *puVar2 = uVar4;
  puVar2[1] = uVar5;
  puVar2[2] = uVar14;
  puVar2[3] = uVar15;
  local_80[0] = 0x3f800000;
  local_6c = 0x3f800000;
  local_48 = 0x3f800000;
  local_68 = uVar4;
  uStack_64 = uVar5;
  uStack_60 = uVar14;
  uStack_5c = uVar15;
  Engine::SetWorldViewMatrix((Matrix *)param_3);
  glDisable(0xb71);
  glDepthMask(0);
  glDisable(0xbe2);
  glUseProgram(*(undefined4 *)(this + 0x20));
  glActiveTexture(0x84c0);
  FBOContainer::Activate();
  FBOContainer::BeginCapture(*(FBOContainer **)(this + 0x3c));
  glEnableVertexAttribArray(*(undefined4 *)(this + 0x24));
  glEnableVertexAttribArray(*(undefined4 *)(this + 0x2c));
  pEVar7 = param_3 + 0x104;
  glUniformMatrix4fv(*(undefined4 *)(this + 0x28),1,0);
  glVertexAttribPointer
            (*(undefined4 *)(this + 0x24),3,0x1406,0,0,
             *(undefined4 *)(*(int *)(param_3 + 0x380) + 4));
  glVertexAttribPointer
            (*(undefined4 *)(this + 0x2c),2,0x1406,0,0,
             *(undefined4 *)(*(int *)(param_3 + 0x380) + 8));
  glClear(0x4000);
  iVar9 = Engine::GetDisplayWidth();
  Engine::GetDisplayHeight();
  Engine::DrawQuad((int)param_3,0,0,iVar9);
  glDisableVertexAttribArray(*(undefined4 *)(this + 0x24));
  glDisableVertexAttribArray(*(undefined4 *)(this + 0x2c));
  fVar12 = 1.0;
  for (iVar9 = 3; iVar9 != 0; iVar9 = iVar9 + -1) {
    glUseProgram(*(undefined4 *)(this + 0x40));
    glActiveTexture(0x84c0);
    FBOContainer::Activate();
    FBOContainer::BeginCapture(*(FBOContainer **)(this + 0x58));
    pcVar10 = *(code **)(DAT_0009d604 + 0x9d320);
    (*pcVar10)(*(undefined4 *)(this + 0x44));
    (*pcVar10)(*(undefined4 *)(this + 0x4c));
    glUniformMatrix4fv(*(undefined4 *)(this + 0x48),1,0,pEVar7);
    pcVar8 = *(code **)(DAT_0009d608 + 0x9d352);
    (*pcVar8)(*(undefined4 *)(this + 0x44),3,0x1406,0,0,
              *(undefined4 *)(*(int *)(param_3 + 0x380) + 4));
    (*pcVar8)(*(undefined4 *)(this + 0x4c),2,0x1406,0,0,
              *(undefined4 *)(*(int *)(param_3 + 0x380) + 8));
    fVar13 = (float)VectorSignedToFloat(*(undefined4 *)(*(int *)(this + 0x58) + 0xc),
                                        (byte)(in_fpscr >> 0x16) & 3);
    glUniform1f(*(undefined4 *)(this + 0x54),fVar12 / fVar13);
    glClear(0x4000);
    iVar3 = Engine::GetDisplayWidth();
    Engine::GetDisplayHeight();
    Engine::DrawQuad((int)param_3,0,0,iVar3);
    pcVar11 = *(code **)(DAT_0009d60c + 0x9d3b2);
    (*pcVar11)(*(undefined4 *)(this + 0x44));
    (*pcVar11)(*(undefined4 *)(this + 0x4c));
    glUseProgram(*(undefined4 *)(this + 0x5c));
    glActiveTexture(0x84c0);
    FBOContainer::Activate();
    FBOContainer::BeginCapture(*(FBOContainer **)(this + 0x74));
    (*pcVar10)(*(undefined4 *)(this + 0x60));
    (*pcVar10)(*(undefined4 *)(this + 0x68));
    glUniformMatrix4fv(*(undefined4 *)(this + 100),1,0,pEVar7);
    (*pcVar8)(*(undefined4 *)(this + 0x60),3,0x1406,0,0,
              *(undefined4 *)(*(int *)(param_3 + 0x380) + 4));
    (*pcVar8)(*(undefined4 *)(this + 0x68),2,0x1406,0,0,
              *(undefined4 *)(*(int *)(param_3 + 0x380) + 8));
    fVar13 = (float)VectorSignedToFloat(*(undefined4 *)(*(int *)(this + 0x74) + 0x10),
                                        (byte)(in_fpscr >> 0x16) & 3);
    glUniform1f(*(undefined4 *)(this + 0x70),fVar12 / fVar13);
    glClear(0x4000);
    iVar3 = Engine::GetDisplayWidth();
    Engine::GetDisplayHeight();
    Engine::DrawQuad((int)param_3,0,0,iVar3);
    (*pcVar11)(*(undefined4 *)(this + 0x60));
    (*pcVar11)(*(undefined4 *)(this + 0x68));
  }
  if (**(uint **)(DAT_0009d600 + 0x9d47a) < 4) {
                    /* WARNING: Could not recover jumptable at 0x0009d484. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*(code *)(&switchD_0009d484::switchdataD_0009d488 +
              (uint)(&switchD_0009d484::switchdataD_0009d488)[**(uint **)(DAT_0009d600 + 0x9d47a)] *
              2))();
    return;
  }
  glUseProgram(*(undefined4 *)(this + 0x78));
  glActiveTexture(0x84c0);
  FBOContainer::Activate();
  glActiveTexture(0x84c1);
  FBOContainer::Activate();
  if (*param_2 == (FBOContainer *)0x0) {
    glBindFramebuffer(0x8d40,*(undefined4 *)(param_3 + 0x40c));
    if (*(int *)(**(int **)(param_3 + 0x30) + 0x30) == 2) {
      uVar4 = Engine::GetDisplayWidth();
      uVar5 = Engine::GetDisplayHeight();
    }
    else {
      uVar4 = Engine::GetDisplayHeight();
      uVar5 = Engine::GetDisplayWidth();
    }
    glViewport(0,0,uVar4,uVar5);
  }
  else {
    FBOContainer::BeginCapture(*param_2);
  }
  glEnableVertexAttribArray(*(undefined4 *)(this + 0x7c));
  glEnableVertexAttribArray(*(undefined4 *)(this + 0x84));
  glUniformMatrix4fv(*(undefined4 *)(this + 0x80),1,0,pEVar7);
  glVertexAttribPointer
            (*(undefined4 *)(this + 0x7c),3,0x1406,0,0,
             *(undefined4 *)(*(int *)(param_3 + 0x380) + 4));
  glVertexAttribPointer
            (*(undefined4 *)(this + 0x84),2,0x1406,0,0,
             *(undefined4 *)(*(int *)(param_3 + 0x380) + 8));
  glClear(0x4000);
  iVar9 = Engine::GetDisplayWidth();
  Engine::GetDisplayHeight();
  Engine::DrawQuad((int)param_3,0,0,iVar9);
  glDisableVertexAttribArray(*(undefined4 *)(this + 0x7c));
  glDisableVertexAttribArray(*(undefined4 *)(this + 0x84));
  if (*param_2 != (FBOContainer *)0x0) {
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
  0009d120: push {r4,r5,r6,r7,lr}
  0009d122: add r7,sp,#0xc
  0009d124: push {r7,r8,r9,r10,r11}
  0009d128: vpush {d8,d9,d10}
  0009d12c: sub sp,#0x68
  0009d12e: str r2,[sp,#0xc]
  0009d130: mov r5,r0
  0009d132: ldr r0,[0x0009d500]
  0009d134: mov r4,r1
  0009d136: ldr r1,[0x0009d504]
  0009d138: mov r6,r3
  0009d13a: add r0,pc
  0009d13c: add r1,pc
  0009d13e: ldr r0,[r0,#0x0]
  0009d140: ldr r2,[r1,#0x0]
  0009d142: str r2,[sp,#0x10]
  0009d144: ldrb r1,[r0,#0x0]
  0009d146: ldr r2,[r2,#0x0]
  0009d148: cmp r1,#0x0
  0009d14a: str r2,[sp,#0x64]
  0009d14c: beq 0x0009d180
  0009d14e: movs r1,#0x0
  0009d150: strb r1,[r0,#0x0]
  0009d152: mov r0,r5
  0009d154: mov r1,r6
  0009d156: blx 0x00070e88
  0009d15a: ldr.w r0,[r5,#0xa0]
  0009d15e: blx 0x0006ee54
  0009d162: movs r0,#0x0
  0009d164: movs r1,#0x0
  0009d166: movs r2,#0x0
  0009d168: mov.w r3,#0x3f800000
  0009d16c: blx 0x0006eecc
  0009d170: mov.w r0,#0x4000
  0009d174: blx 0x0006eed8
  0009d178: ldr.w r0,[r5,#0xa0]
  0009d17c: blx 0x0006ee60
  0009d180: vmov.i32 q4,#0x0
  0009d184: add.w r0,r6,#0x3b4
  0009d188: vst1.32 {d8,d9},[r0]
  0009d18c: add.w r0,r6,#0x3a4
  0009d190: vst1.32 {d8,d9},[r0]
  0009d194: add.w r0,r6,#0x394
  0009d198: vst1.32 {d8,d9},[r0]
  0009d19c: add.w r0,r6,#0x384
  0009d1a0: vst1.32 {d8,d9},[r0]
  0009d1a4: mov r0,r6
  0009d1a6: blx 0x0006e8cc
  0009d1aa: vmov s0,r0
  0009d1ae: mov r0,r6
  0009d1b0: vmov.f32 s20,0x40000000
  0009d1b4: vcvt.f32.s32 s0,s0
  0009d1b8: vdiv.f32 s0,s20,s0
  0009d1bc: vstr.32 s0,[r6,#0x384]
  0009d1c0: blx 0x0006e8d8
  0009d1c4: vmov s0,r0
  0009d1c8: adr.w r1,0x9d5f0
  0009d1cc: vld1.64 {d16,d17},[r1]
  0009d1d0: mov.w r2,#0x3f800000
  0009d1d4: vcvt.f32.s32 s0,s0
  0009d1d8: ldr r1,[0x0009d508]
  0009d1da: str.w r1,[r6,#0x3ac]
  0009d1de: add r1,sp,#0x28
  0009d1e0: ldr r0,[0x0009d50c]
  0009d1e2: str.w r2,[r6,#0x3c0]
  0009d1e6: strd r0,r2,[r6,#0x3b4]
  0009d1ea: orr r0,r1,#0x4
  0009d1ee: vdiv.f32 s0,s20,s0
  0009d1f2: vneg.f32 s0,s0
  0009d1f6: vstr.32 s0,[r6,#0x398]
  0009d1fa: vst1.32 {d8,d9},[r0]
  0009d1fe: add.w r0,r1,#0x18
  0009d202: vst1.64 {d8,d9},[r0]
  0009d206: add.w r0,r1,#0x28
  0009d20a: vst1.64 {d16,d17},[r0]
  0009d20e: mov r0,r6
  0009d210: str r2,[sp,#0x28]
  0009d212: str r2,[sp,#0x3c]
  0009d214: str r2,[sp,#0x60]
  0009d216: blx 0x0006fae4
  0009d21a: movw r0,#0xb71
  0009d21e: blx 0x0006edc4
  0009d222: movs r0,#0x0
  0009d224: str r6,[sp,#0x24]
  0009d226: mov.w r8,#0x0
  0009d22a: blx 0x0006eec0
  0009d22e: movw r0,#0xbe2
  0009d232: blx 0x0006edc4
  0009d236: ldr r0,[r5,#0x20]
  0009d238: blx 0x00070960
  0009d23c: movw r0,#0x84c0
  0009d240: blx 0x0006fb8c
  0009d244: mov r0,r4
  0009d246: str r4,[sp,#0x14]
  0009d248: blx 0x0006ee48
  0009d24c: mov r4,r5
  0009d24e: ldr.w r0,[r4,#0x3c]!
  0009d252: blx 0x0006ee54
  0009d256: ldr.w r0,[r4,#-0x18]
  0009d25a: blx 0x00070a5c
  0009d25e: ldr.w r0,[r4,#-0x10]
  0009d262: blx 0x00070a5c
  0009d266: ldr r1,[sp,#0x24]
  0009d268: movs r2,#0x0
  0009d26a: ldr.w r0,[r4,#-0x14]
  0009d26e: add.w r3,r1,#0x104
  0009d272: movs r1,#0x1
  0009d274: str r3,[sp,#0x18]
  0009d276: blx 0x00070990
  0009d27a: ldr r0,[sp,#0x24]
  0009d27c: movw r2,#0x1406
  0009d280: movs r3,#0x0
  0009d282: mov.w r10,#0x3
  0009d286: ldr.w r1,[r0,#0x380]
  0009d28a: ldr.w r0,[r4,#-0x18]
  0009d28e: ldr r1,[r1,#0x4]
  0009d290: strd r8,r1,[sp,#0x0]
  0009d294: movs r1,#0x3
  0009d296: blx 0x000709c0
  0009d29a: ldr r0,[sp,#0x24]
  0009d29c: movw r2,#0x1406
  0009d2a0: movs r3,#0x0
  0009d2a2: ldr r6,[sp,#0x24]
  0009d2a4: ldr.w r0,[r0,#0x380]
  0009d2a8: ldr r1,[r0,#0x8]
  0009d2aa: ldr.w r0,[r4,#-0x10]
  0009d2ae: str r1,[sp,#0x4]
  0009d2b0: movs r1,#0x2
  0009d2b2: str.w r8,[sp,#0x0]
  0009d2b6: blx 0x000709c0
  0009d2ba: mov.w r0,#0x4000
  0009d2be: blx 0x0006eed8
  0009d2c2: mov r0,r6
  0009d2c4: blx 0x0006e8cc
  0009d2c8: mov r9,r0
  0009d2ca: mov r0,r6
  0009d2cc: blx 0x0006e8d8
  0009d2d0: str r0,[sp,#0x0]
  0009d2d2: mov r0,r6
  0009d2d4: movs r1,#0x0
  0009d2d6: movs r2,#0x0
  0009d2d8: mov r3,r9
  0009d2da: blx 0x0006e8a8
  0009d2de: ldr.w r0,[r4,#-0x18]
  0009d2e2: blx 0x00070a44
  0009d2e6: ldr.w r0,[r4,#-0x10]
  0009d2ea: blx 0x00070a44
  0009d2ee: vmov.f32 s16,0x3f800000
  0009d2f2: ldr.w r8,[sp,#0x18]
  0009d2f6: add.w r0,r4,#0x38
  0009d2fa: str r0,[sp,#0x1c]
  0009d2fc: b 0x0009d46a
  0009d2fe: ldr r0,[r5,#0x40]
  0009d300: blx 0x00070960
  0009d304: movw r0,#0x84c0
  0009d308: blx 0x0006fb8c
  0009d30c: mov r0,r4
  0009d30e: blx 0x0006ee48
  0009d312: ldr r0,[r5,#0x58]
  0009d314: blx 0x0006ee54
  0009d318: ldr r1,[0x0009d604]
  0009d31a: ldr r0,[r5,#0x44]
  0009d31c: add r1,pc
  0009d31e: str.w r10,[sp,#0x20]
  0009d322: ldr.w r10,[r1,#0x0]
  0009d326: blx r10
  0009d328: ldr r0,[r5,#0x4c]
  0009d32a: blx r10
  0009d32c: ldr r0,[r5,#0x48]
  0009d32e: movs r1,#0x1
  0009d330: movs r2,#0x0
  0009d332: mov r3,r8
  0009d334: blx 0x00070990
  0009d338: ldr.w r0,[r6,#0x380]
  0009d33c: movs r6,#0x0
  0009d33e: movw r2,#0x1406
  0009d342: movs r3,#0x0
  0009d344: ldr r1,[r0,#0x4]
  0009d346: ldr r0,[r5,#0x44]
  0009d348: strd r6,r1,[sp,#0x0]
  0009d34c: ldr r1,[0x0009d608]
  0009d34e: add r1,pc
  0009d350: ldr.w r9,[r1,#0x0]
  0009d354: movs r1,#0x3
  0009d356: blx r9
  0009d358: ldr r0,[sp,#0x24]
  0009d35a: movw r2,#0x1406
  0009d35e: movs r3,#0x0
  0009d360: ldr.w r0,[r0,#0x380]
  0009d364: ldr r1,[r0,#0x8]
  0009d366: ldr r0,[r5,#0x4c]
  0009d368: strd r6,r1,[sp,#0x0]
  0009d36c: movs r1,#0x2
  0009d36e: blx r9
  0009d370: ldrd r0,r1,[r5,#0x54]
  0009d374: vldr.32 s0,[r1,#0xc]
  0009d378: vcvt.f32.s32 s0,s0
  0009d37c: vdiv.f32 s0,s16,s0
  0009d380: vmov r1,s0
  0009d384: blx 0x00070978
  0009d388: mov.w r0,#0x4000
  0009d38c: blx 0x0006eed8
  0009d390: ldr r0,[sp,#0x24]
  0009d392: blx 0x0006e8cc
  0009d396: mov r4,r0
  0009d398: ldr r0,[sp,#0x24]
  0009d39a: blx 0x0006e8d8
  0009d39e: str r0,[sp,#0x0]
  0009d3a0: movs r1,#0x0
  0009d3a2: ldr r0,[sp,#0x24]
  0009d3a4: movs r2,#0x0
  0009d3a6: mov r3,r4
  0009d3a8: blx 0x0006e8a8
  0009d3ac: ldr r0,[0x0009d60c]
  0009d3ae: add r0,pc
  0009d3b0: ldr.w r11,[r0,#0x0]
  0009d3b4: ldr r0,[r5,#0x44]
  0009d3b6: blx r11
  0009d3b8: ldr r0,[r5,#0x4c]
  0009d3ba: blx r11
  0009d3bc: ldr r0,[r5,#0x5c]
  0009d3be: blx 0x00070960
  0009d3c2: movw r0,#0x84c0
  0009d3c6: blx 0x0006fb8c
  0009d3ca: ldr r0,[r5,#0x58]
  0009d3cc: blx 0x0006ee48
  0009d3d0: ldr r0,[r5,#0x74]
  0009d3d2: blx 0x0006ee54
  0009d3d6: ldr r0,[r5,#0x60]
  0009d3d8: blx r10
  0009d3da: ldr r0,[r5,#0x68]
  0009d3dc: blx r10
  0009d3de: ldr r0,[r5,#0x64]
  0009d3e0: movs r1,#0x1
  0009d3e2: movs r2,#0x0
  0009d3e4: mov r3,r8
  0009d3e6: ldr.w r10,[sp,#0x20]
  0009d3ea: blx 0x00070990
  0009d3ee: ldr r0,[sp,#0x24]
  0009d3f0: movw r2,#0x1406
  0009d3f4: movs r3,#0x0
  0009d3f6: ldr.w r0,[r0,#0x380]
  0009d3fa: ldr r1,[r0,#0x4]
  0009d3fc: ldr r0,[r5,#0x60]
  0009d3fe: strd r6,r1,[sp,#0x0]
  0009d402: movs r1,#0x3
  0009d404: blx r9
  0009d406: ldr r0,[sp,#0x24]
  0009d408: movw r2,#0x1406
  0009d40c: movs r3,#0x0
  0009d40e: ldr.w r0,[r0,#0x380]
  0009d412: ldr r1,[r0,#0x8]
  0009d414: ldr r0,[r5,#0x68]
  0009d416: strd r6,r1,[sp,#0x0]
  0009d41a: movs r1,#0x2
  0009d41c: blx r9
  0009d41e: ldr r6,[sp,#0x24]
  0009d420: ldrd r0,r1,[r5,#0x70]
  0009d424: vldr.32 s0,[r1,#0x10]
  0009d428: vcvt.f32.s32 s0,s0
  0009d42c: vdiv.f32 s0,s16,s0
  0009d430: vmov r1,s0
  0009d434: blx 0x00070978
  0009d438: mov.w r0,#0x4000
  0009d43c: blx 0x0006eed8
  0009d440: mov r0,r6
  0009d442: blx 0x0006e8cc
  0009d446: mov r4,r0
  0009d448: mov r0,r6
  0009d44a: blx 0x0006e8d8
  0009d44e: str r0,[sp,#0x0]
  0009d450: mov r0,r6
  0009d452: movs r1,#0x0
  0009d454: movs r2,#0x0
  0009d456: mov r3,r4
  0009d458: blx 0x0006e8a8
  0009d45c: ldr r0,[r5,#0x60]
  0009d45e: blx r11
  0009d460: ldr r0,[r5,#0x68]
  0009d462: blx r11
  0009d464: sub.w r10,r10,#0x1
  0009d468: ldr r4,[sp,#0x1c]
  0009d46a: ldr r4,[r4,#0x0]
  0009d46c: cmp.w r10,#0x0
  0009d470: bne.w 0x0009d2fe
  0009d474: ldr r0,[0x0009d600]
  0009d476: add r0,pc
  0009d478: ldr r0,[r0,#0x0]
  0009d47a: ldr r0,[r0,#0x0]
  0009d47c: cmp r0,#0x3
  0009d47e: bhi 0x0009d498
  0009d480: ldrd r9,r8,[sp,#0xc]
  0009d484: tbb [pc,r0]
  0009d48c: ldr r0,[r5,#0x3c]
  0009d48e: str r0,[sp,#0x14]
  0009d490: add.w r0,r5,#0xa0
  0009d494: str r0,[sp,#0x1c]
  0009d496: b 0x0009d4a6
  0009d498: ldrd r9,r8,[sp,#0xc]
  0009d49c: b 0x0009d4aa
  0009d49e: add.w r0,r5,#0xa0
  0009d4a2: str r0,[sp,#0x1c]
  0009d4a4: str r4,[sp,#0x14]
  0009d4a6: ldr r0,[sp,#0x1c]
  0009d4a8: ldr r4,[r0,#0x0]
  0009d4aa: ldr r0,[r5,#0x78]
  0009d4ac: blx 0x00070960
  0009d4b0: movw r0,#0x84c0
  0009d4b4: blx 0x0006fb8c
  0009d4b8: ldr r0,[sp,#0x14]
  0009d4ba: blx 0x0006ee48
  0009d4be: movw r0,#0x84c1
  0009d4c2: blx 0x0006fb8c
  0009d4c6: mov r0,r4
  0009d4c8: blx 0x0006ee48
  0009d4cc: ldr.w r0,[r9,#0x0]
  0009d4d0: cbz r0,0x0009d4d8
  0009d4d2: blx 0x0006ee54
  0009d4d6: b 0x0009d52a
  0009d4d8: ldr.w r1,[r6,#0x40c]
  0009d4dc: movw r0,#0x8d40
  0009d4e0: blx 0x0006ee6c
  0009d4e4: ldr r0,[r6,#0x30]
  0009d4e6: ldr r0,[r0,#0x0]
  0009d4e8: ldr r0,[r0,#0x30]
  0009d4ea: cmp r0,#0x2
  0009d4ec: bne 0x0009d510
  0009d4ee: mov r0,r6
  0009d4f0: blx 0x0006e8cc
  0009d4f4: mov r4,r0
  0009d4f6: mov r0,r6
  0009d4f8: blx 0x0006e8d8
  0009d4fc: b 0x0009d51e
  0009d510: mov r0,r6
  0009d512: blx 0x0006e8d8
  0009d516: mov r4,r0
  0009d518: mov r0,r6
  0009d51a: blx 0x0006e8cc
  0009d51e: mov r3,r0
  0009d520: movs r0,#0x0
  0009d522: movs r1,#0x0
  0009d524: mov r2,r4
  0009d526: blx 0x0006eda0
  0009d52a: ldr r0,[r5,#0x7c]
  0009d52c: blx 0x00070a5c
  0009d530: ldr.w r0,[r5,#0x84]
  0009d534: blx 0x00070a5c
  0009d538: ldr.w r0,[r5,#0x80]
  0009d53c: movs r1,#0x1
  0009d53e: ldr r3,[sp,#0x18]
  0009d540: movs r2,#0x0
  0009d542: movs r4,#0x0
  0009d544: blx 0x00070990
  0009d548: ldr.w r1,[r6,#0x380]
  0009d54c: movw r2,#0x1406
  0009d550: ldr r0,[r5,#0x7c]
  0009d552: movs r3,#0x0
  0009d554: ldr r1,[r1,#0x4]
  0009d556: strd r4,r1,[sp,#0x0]
  0009d55a: movs r1,#0x3
  0009d55c: blx 0x000709c0
  0009d560: ldr.w r1,[r6,#0x380]
  0009d564: movw r2,#0x1406
  0009d568: ldr.w r0,[r5,#0x84]
  0009d56c: movs r3,#0x0
  0009d56e: ldr r1,[r1,#0x8]
  0009d570: strd r4,r1,[sp,#0x0]
  0009d574: movs r1,#0x2
  0009d576: blx 0x000709c0
  0009d57a: mov.w r0,#0x4000
  0009d57e: blx 0x0006eed8
  0009d582: mov r0,r6
  0009d584: blx 0x0006e8cc
  0009d588: mov r4,r0
  0009d58a: mov r0,r6
  0009d58c: blx 0x0006e8d8
  0009d590: str r0,[sp,#0x0]
  0009d592: mov r0,r6
  0009d594: movs r1,#0x0
  0009d596: movs r2,#0x0
  0009d598: mov r3,r4
  0009d59a: blx 0x0006e8a8
  0009d59e: ldr r0,[r5,#0x7c]
  0009d5a0: blx 0x00070a44
  0009d5a4: ldr.w r0,[r5,#0x84]
  0009d5a8: blx 0x00070a44
  0009d5ac: ldr.w r0,[r9,#0x0]
  0009d5b0: cbz r0,0x0009d5b6
  0009d5b2: blx 0x0006ee60
  0009d5b6: movw r0,#0xbe2
  0009d5ba: blx 0x0006edb8
  0009d5be: movw r0,#0x302
  0009d5c2: movw r1,#0x303
  0009d5c6: blx 0x0006fe68
  0009d5ca: movw r0,#0x84c0
  0009d5ce: blx 0x0006fb8c
  0009d5d2: ldr r0,[sp,#0x64]
  0009d5d4: ldr.w r1,[r8,#0x0]
  0009d5d8: subs r0,r1,r0
  0009d5da: itttt eq
  0009d5dc: add.eq sp,#0x68
  0009d5de: vpop.eq {d8,d9,d10}
  0009d5e2: add.eq sp,#0x4
  0009d5e4: pop.eq.w {r8,r9,r10,r11}
  0009d5e8: it eq
  0009d5ea: pop.eq {r4,r5,r6,r7,pc}
  0009d5ec: blx 0x0006e824
```
