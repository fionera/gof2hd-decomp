# BloomShader::RenderEffect
elf 0x89cd8 body 1096
Sig: undefined __thiscall RenderEffect(BloomShader * this, FBOContainer * param_1, FBOContainer * * param_2, Engine * param_3)

## decompile
```c

/* AbyssEngine::BloomShader::RenderEffect(AbyssEngine::FBOContainer*, AbyssEngine::FBOContainer*&,
   AbyssEngine::Engine*) */

void __thiscall
AbyssEngine::BloomShader::RenderEffect
          (BloomShader *this,FBOContainer *param_1,FBOContainer **param_2,Engine *param_3)

{
  undefined4 uVar1;
  undefined4 *puVar2;
  int iVar3;
  int iVar4;
  int *piVar5;
  Engine *pEVar6;
  code *pcVar7;
  code *pcVar8;
  code *pcVar9;
  uint in_fpscr;
  float fVar10;
  undefined4 uVar11;
  undefined4 uVar12;
  undefined4 uVar13;
  undefined4 uVar14;
  undefined4 local_80 [5];
  undefined4 local_6c;
  undefined4 local_68;
  undefined4 uStack_64;
  undefined4 uStack_60;
  undefined4 uStack_5c;
  undefined8 local_58;
  undefined8 uStack_50;
  undefined4 local_48;
  int local_44;
  
  piVar5 = *(int **)(DAT_0009a02c + 0x99cf8);
  local_44 = *piVar5;
  if (**(char **)(DAT_0009a028 + 0x99cf6) != '\0') {
    **(char **)(DAT_0009a028 + 0x99cf6) = '\0';
    InternalInit((Engine *)this);
    FBOContainer::BeginCapture(*(FBOContainer **)(this + 0x70));
    glClearColor(0,0,0,0x3f800000);
    glClear(0x4000);
    FBOContainer::EndCapture();
  }
  uVar11 = 0;
  uVar12 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  uVar13 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  uVar14 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  *(undefined4 *)(param_3 + 0x3b4) = 0;
  *(undefined4 *)(param_3 + 0x3b8) = uVar12;
  *(undefined4 *)(param_3 + 0x3bc) = uVar13;
  *(undefined4 *)(param_3 + 0x3c0) = uVar14;
  *(undefined4 *)(param_3 + 0x3a4) = 0;
  *(undefined4 *)(param_3 + 0x3a8) = uVar12;
  *(undefined4 *)(param_3 + 0x3ac) = uVar13;
  *(undefined4 *)(param_3 + 0x3b0) = uVar14;
  *(undefined4 *)(param_3 + 0x394) = 0;
  *(undefined4 *)(param_3 + 0x398) = uVar12;
  *(undefined4 *)(param_3 + 0x39c) = uVar13;
  *(undefined4 *)(param_3 + 0x3a0) = uVar14;
  *(undefined4 *)(param_3 + 900) = 0;
  *(undefined4 *)(param_3 + 0x388) = uVar12;
  *(undefined4 *)(param_3 + 0x38c) = uVar13;
  *(undefined4 *)(param_3 + 0x390) = uVar14;
  uVar1 = Engine::GetDisplayWidth();
  fVar10 = (float)VectorSignedToFloat(uVar1,(byte)(in_fpscr >> 0x16) & 3);
  *(float *)(param_3 + 900) = 2.0 / fVar10;
  uVar1 = Engine::GetDisplayHeight();
  uStack_50 = DAT_0009a138;
  local_58 = DAT_0009a130;
  fVar10 = (float)VectorSignedToFloat(uVar1,(byte)(in_fpscr >> 0x16) & 3);
  *(undefined4 *)(param_3 + 0x3ac) = DAT_0009a144;
  uVar1 = DAT_0009a140;
  *(undefined4 *)(param_3 + 0x3c0) = 0x3f800000;
  *(undefined4 *)(param_3 + 0x3b4) = uVar1;
  *(undefined4 *)(param_3 + 0x3b8) = 0x3f800000;
  puVar2 = (undefined4 *)((uint)local_80 | 4);
  *(float *)(param_3 + 0x398) = -(2.0 / fVar10);
  *puVar2 = uVar11;
  puVar2[1] = uVar12;
  puVar2[2] = uVar13;
  puVar2[3] = uVar14;
  local_80[0] = 0x3f800000;
  local_6c = 0x3f800000;
  local_48 = 0x3f800000;
  local_68 = uVar11;
  uStack_64 = uVar12;
  uStack_60 = uVar13;
  uStack_5c = uVar14;
  Engine::SetWorldViewMatrix((Matrix *)param_3);
  glDisable(0xb71);
  glDepthMask(0);
  glDisable(0xbe2);
  glUseProgram(*(undefined4 *)(this + 0x20));
  glActiveTexture(0x84c0);
  FBOContainer::Activate();
  FBOContainer::BeginCapture(*(FBOContainer **)(this + 0x34));
  glEnableVertexAttribArray(*(undefined4 *)(this + 0x24));
  glEnableVertexAttribArray(*(undefined4 *)(this + 0x2c));
  pEVar6 = param_3 + 0x104;
  glUniformMatrix4fv(*(undefined4 *)(this + 0x28),1,0);
  glVertexAttribPointer
            (*(undefined4 *)(this + 0x24),3,0x1406,0,0,
             *(undefined4 *)(*(int *)(param_3 + 0x380) + 4));
  glVertexAttribPointer
            (*(undefined4 *)(this + 0x2c),2,0x1406,0,0,
             *(undefined4 *)(*(int *)(param_3 + 0x380) + 8));
  glClear(0x4000);
  iVar3 = Engine::GetDisplayWidth();
  Engine::GetDisplayHeight();
  Engine::DrawQuad((int)param_3,0,0,iVar3);
  glDisableVertexAttribArray(*(undefined4 *)(this + 0x24));
  glDisableVertexAttribArray(*(undefined4 *)(this + 0x2c));
  for (iVar3 = 6; iVar3 != 0; iVar3 = iVar3 + -1) {
    glUseProgram(*(undefined4 *)(this + 0x38));
    glActiveTexture(0x84c0);
    FBOContainer::Activate();
    FBOContainer::BeginCapture(*(FBOContainer **)(this + 0x50));
    pcVar9 = *(code **)(DAT_0009a14c + 0x99ec4);
    (*pcVar9)(*(undefined4 *)(this + 0x3c));
    (*pcVar9)(*(undefined4 *)(this + 0x44));
    glUniformMatrix4fv(*(undefined4 *)(this + 0x40),1,0,pEVar6);
    pcVar8 = *(code **)(DAT_0009a150 + 0x99ef4);
    (*pcVar8)(*(undefined4 *)(this + 0x3c),3,0x1406,0,0,
              *(undefined4 *)(*(int *)(param_3 + 0x380) + 4));
    (*pcVar8)(*(undefined4 *)(this + 0x44),2,0x1406,0,0,
              *(undefined4 *)(*(int *)(param_3 + 0x380) + 8));
    uVar11 = VectorSignedToFloat(*(undefined4 *)(*(int *)(this + 0x50) + 0xc),
                                 (byte)(in_fpscr >> 0x16) & 3);
    glUniform1f(*(undefined4 *)(this + 0x4c),uVar11);
    glClear(0x4000);
    iVar4 = Engine::GetDisplayWidth();
    Engine::GetDisplayHeight();
    Engine::DrawQuad((int)param_3,0,0,iVar4);
    pcVar7 = *(code **)(DAT_0009a154 + 0x99f4e);
    (*pcVar7)(*(undefined4 *)(this + 0x3c));
    (*pcVar7)(*(undefined4 *)(this + 0x44));
    glUseProgram(*(undefined4 *)(this + 0x54));
    glActiveTexture(0x84c0);
    FBOContainer::Activate();
    FBOContainer::BeginCapture(*(FBOContainer **)(this + 0x6c));
    (*pcVar9)(*(undefined4 *)(this + 0x58));
    (*pcVar9)(*(undefined4 *)(this + 0x60));
    glUniformMatrix4fv(*(undefined4 *)(this + 0x5c),1,0,pEVar6);
    (*pcVar8)(*(undefined4 *)(this + 0x58),3,0x1406,0,0,
              *(undefined4 *)(*(int *)(param_3 + 0x380) + 4));
    (*pcVar8)(*(undefined4 *)(this + 0x60),2,0x1406,0,0,
              *(undefined4 *)(*(int *)(param_3 + 0x380) + 8));
    uVar11 = VectorSignedToFloat(*(undefined4 *)(*(int *)(this + 0x6c) + 0x10),
                                 (byte)(in_fpscr >> 0x16) & 3);
    glUniform1f(*(undefined4 *)(this + 0x68),uVar11);
    glClear(0x4000);
    iVar4 = Engine::GetDisplayWidth();
    Engine::GetDisplayHeight();
    Engine::DrawQuad((int)param_3,0,0,iVar4);
    (*pcVar7)(*(undefined4 *)(this + 0x58));
    (*pcVar7)(*(undefined4 *)(this + 0x60));
  }
  if (**(uint **)(DAT_0009a148 + 0x9a006) < 4) {
                    /* WARNING: Could not recover jumptable at 0x0009a014. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*(code *)(&switchD_0009a014::switchdataD_0009a018 +
              (uint)(&switchD_0009a014::switchdataD_0009a018)[**(uint **)(DAT_0009a148 + 0x9a006)] *
              2))();
    return;
  }
  glUseProgram(*(undefined4 *)(this + 0x74));
  glActiveTexture(0x84c0);
  FBOContainer::Activate();
  glActiveTexture(0x84c1);
  FBOContainer::Activate();
  if (*param_2 != (FBOContainer *)0x0) {
    FBOContainer::BeginCapture(*param_2);
  }
  glEnableVertexAttribArray(*(undefined4 *)(this + 0x78));
  glEnableVertexAttribArray(*(undefined4 *)(this + 0x80));
  glUniformMatrix4fv(*(undefined4 *)(this + 0x7c),1,0,pEVar6);
  glVertexAttribPointer
            (*(undefined4 *)(this + 0x78),3,0x1406,0,0,
             *(undefined4 *)(*(int *)(param_3 + 0x380) + 4));
  glVertexAttribPointer
            (*(undefined4 *)(this + 0x80),2,0x1406,0,0,
             *(undefined4 *)(*(int *)(param_3 + 0x380) + 8));
  glClear(0x4000);
  iVar3 = Engine::GetDisplayWidth();
  Engine::GetDisplayHeight();
  Engine::DrawQuad((int)param_3,0,0,iVar3);
  glDisableVertexAttribArray(*(undefined4 *)(this + 0x78));
  glDisableVertexAttribArray(*(undefined4 *)(this + 0x80));
  glEnable(0xbe2);
  glBlendFunc(0x302,0x303);
  glActiveTexture(0x84c0);
  if (*param_2 != (FBOContainer *)0x0) {
    FBOContainer::EndCapture();
  }
  if (*piVar5 != local_44) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  00099cd8: push {r4,r5,r6,r7,lr}
  00099cda: add r7,sp,#0xc
  00099cdc: push {r7,r8,r9,r10,r11}
  00099ce0: vpush {d8,d9,d10}
  00099ce4: sub sp,#0x60
  00099ce6: str r2,[sp,#0xc]
  00099ce8: mov r5,r0
  00099cea: ldr r0,[0x0009a028]
  00099cec: mov r4,r1
  00099cee: ldr r1,[0x0009a02c]
  00099cf0: mov r6,r3
  00099cf2: add r0,pc
  00099cf4: add r1,pc
  00099cf6: ldr r0,[r0,#0x0]
  00099cf8: ldr r2,[r1,#0x0]
  00099cfa: str r2,[sp,#0x10]
  00099cfc: ldrb r1,[r0,#0x0]
  00099cfe: ldr r2,[r2,#0x0]
  00099d00: cmp r1,#0x0
  00099d02: str r2,[sp,#0x5c]
  00099d04: beq 0x00099d34
  00099d06: movs r1,#0x0
  00099d08: strb r1,[r0,#0x0]
  00099d0a: mov r0,r5
  00099d0c: mov r1,r6
  00099d0e: blx 0x00070be8
  00099d12: ldr r0,[r5,#0x70]
  00099d14: blx 0x0006ee54
  00099d18: movs r0,#0x0
  00099d1a: movs r1,#0x0
  00099d1c: movs r2,#0x0
  00099d1e: mov.w r3,#0x3f800000
  00099d22: blx 0x0006eecc
  00099d26: mov.w r0,#0x4000
  00099d2a: blx 0x0006eed8
  00099d2e: ldr r0,[r5,#0x70]
  00099d30: blx 0x0006ee60
  00099d34: vmov.i32 q4,#0x0
  00099d38: add.w r0,r6,#0x3b4
  00099d3c: vst1.32 {d8,d9},[r0]
  00099d40: add.w r0,r6,#0x3a4
  00099d44: vst1.32 {d8,d9},[r0]
  00099d48: add.w r0,r6,#0x394
  00099d4c: vst1.32 {d8,d9},[r0]
  00099d50: add.w r0,r6,#0x384
  00099d54: vst1.32 {d8,d9},[r0]
  00099d58: mov r0,r6
  00099d5a: blx 0x0006e8cc
  00099d5e: vmov s0,r0
  00099d62: mov r0,r6
  00099d64: vmov.f32 s20,0x40000000
  00099d68: vcvt.f32.s32 s0,s0
  00099d6c: vdiv.f32 s0,s20,s0
  00099d70: vstr.32 s0,[r6,#0x384]
  00099d74: blx 0x0006e8d8
  00099d78: vmov s0,r0
  00099d7c: adr r1,[0x9a130]
  00099d7e: vld1.64 {d16,d17},[r1]
  00099d82: mov.w r2,#0x3f800000
  00099d86: vcvt.f32.s32 s0,s0
  00099d8a: ldr r1,[0x0009a144]
  00099d8c: str.w r1,[r6,#0x3ac]
  00099d90: add r1,sp,#0x20
  00099d92: ldr r0,[0x0009a140]
  00099d94: str.w r2,[r6,#0x3c0]
  00099d98: strd r0,r2,[r6,#0x3b4]
  00099d9c: orr r0,r1,#0x4
  00099da0: vdiv.f32 s0,s20,s0
  00099da4: vneg.f32 s0,s0
  00099da8: vstr.32 s0,[r6,#0x398]
  00099dac: vst1.32 {d8,d9},[r0]
  00099db0: add.w r0,r1,#0x18
  00099db4: vst1.64 {d8,d9},[r0]
  00099db8: add.w r0,r1,#0x28
  00099dbc: vst1.64 {d16,d17},[r0]
  00099dc0: mov r0,r6
  00099dc2: str r2,[sp,#0x20]
  00099dc4: str r2,[sp,#0x34]
  00099dc6: str r2,[sp,#0x58]
  00099dc8: blx 0x0006fae4
  00099dcc: movw r0,#0xb71
  00099dd0: blx 0x0006edc4
  00099dd4: movs r0,#0x0
  00099dd6: mov.w r8,#0x0
  00099dda: blx 0x0006eec0
  00099dde: movw r0,#0xbe2
  00099de2: blx 0x0006edc4
  00099de6: ldr r0,[r5,#0x20]
  00099de8: blx 0x00070960
  00099dec: movw r0,#0x84c0
  00099df0: blx 0x0006fb8c
  00099df4: mov r0,r4
  00099df6: str r4,[sp,#0x8]
  00099df8: blx 0x0006ee48
  00099dfc: mov r4,r5
  00099dfe: ldr.w r0,[r4,#0x34]!
  00099e02: blx 0x0006ee54
  00099e06: ldr.w r0,[r4,#-0x10]
  00099e0a: blx 0x00070a5c
  00099e0e: ldr.w r0,[r4,#-0x8]
  00099e12: blx 0x00070a5c
  00099e16: ldr.w r0,[r4,#-0xc]
  00099e1a: add.w r3,r6,#0x104
  00099e1e: movs r1,#0x1
  00099e20: movs r2,#0x0
  00099e22: str r3,[sp,#0x14]
  00099e24: blx 0x00070990
  00099e28: ldr.w r1,[r6,#0x380]
  00099e2c: movw r2,#0x1406
  00099e30: ldr.w r0,[r4,#-0x10]
  00099e34: movs r3,#0x0
  00099e36: ldr r1,[r1,#0x4]
  00099e38: strd r8,r1,[sp,#0x0]
  00099e3c: movs r1,#0x3
  00099e3e: blx 0x000709c0
  00099e42: ldr.w r0,[r6,#0x380]
  00099e46: movw r2,#0x1406
  00099e4a: movs r3,#0x0
  00099e4c: ldr r1,[r0,#0x8]
  00099e4e: ldr.w r0,[r4,#-0x8]
  00099e52: strd r8,r1,[sp,#0x0]
  00099e56: movs r1,#0x2
  00099e58: blx 0x000709c0
  00099e5c: mov.w r0,#0x4000
  00099e60: blx 0x0006eed8
  00099e64: mov r0,r6
  00099e66: blx 0x0006e8cc
  00099e6a: mov r10,r0
  00099e6c: mov r0,r6
  00099e6e: blx 0x0006e8d8
  00099e72: str r0,[sp,#0x0]
  00099e74: mov r0,r6
  00099e76: movs r1,#0x0
  00099e78: movs r2,#0x0
  00099e7a: mov r3,r10
  00099e7c: blx 0x0006e8a8
  00099e80: ldr.w r0,[r4,#-0x10]
  00099e84: blx 0x00070a44
  00099e88: ldr.w r0,[r4,#-0x8]
  00099e8c: blx 0x00070a44
  00099e90: ldr.w r9,[sp,#0x14]
  00099e94: add.w r0,r4,#0x38
  00099e98: str r0,[sp,#0x18]
  00099e9a: movs r0,#0x6
  00099e9c: b 0x00099ffa
  00099e9e: str r0,[sp,#0x1c]
  00099ea0: ldr r0,[r5,#0x38]
  00099ea2: ldr r4,[r4,#0x0]
  00099ea4: blx 0x00070960
  00099ea8: movw r0,#0x84c0
  00099eac: blx 0x0006fb8c
  00099eb0: mov r0,r4
  00099eb2: blx 0x0006ee48
  00099eb6: ldr r0,[r5,#0x50]
  00099eb8: blx 0x0006ee54
  00099ebc: ldr r1,[0x0009a14c]
  00099ebe: ldr r0,[r5,#0x3c]
  00099ec0: add r1,pc
  00099ec2: ldr.w r11,[r1,#0x0]
  00099ec6: blx r11
  00099ec8: ldr r0,[r5,#0x44]
  00099eca: blx r11
  00099ecc: ldr r0,[r5,#0x40]
  00099ece: movs r1,#0x1
  00099ed0: movs r2,#0x0
  00099ed2: mov r3,r9
  00099ed4: blx 0x00070990
  00099ed8: ldr.w r0,[r6,#0x380]
  00099edc: movs r2,#0x0
  00099ede: movs r3,#0x0
  00099ee0: movs r4,#0x0
  00099ee2: ldr r1,[r0,#0x4]
  00099ee4: ldr r0,[r5,#0x3c]
  00099ee6: str r2,[sp,#0x0]
  00099ee8: movw r2,#0x1406
  00099eec: str r1,[sp,#0x4]
  00099eee: ldr r1,[0x0009a150]
  00099ef0: add r1,pc
  00099ef2: ldr.w r10,[r1,#0x0]
  00099ef6: movs r1,#0x3
  00099ef8: blx r10
  00099efa: ldr.w r0,[r6,#0x380]
  00099efe: movw r2,#0x1406
  00099f02: movs r3,#0x0
  00099f04: ldr r1,[r0,#0x8]
  00099f06: ldr r0,[r5,#0x44]
  00099f08: strd r4,r1,[sp,#0x0]
  00099f0c: movs r1,#0x2
  00099f0e: blx r10
  00099f10: ldrd r0,r1,[r5,#0x4c]
  00099f14: vldr.32 s0,[r1,#0xc]
  00099f18: vcvt.f32.s32 s0,s0
  00099f1c: vmov r1,s0
  00099f20: blx 0x00070978
  00099f24: mov.w r0,#0x4000
  00099f28: blx 0x0006eed8
  00099f2c: mov r0,r6
  00099f2e: blx 0x0006e8cc
  00099f32: mov r4,r0
  00099f34: mov r0,r6
  00099f36: blx 0x0006e8d8
  00099f3a: str r0,[sp,#0x0]
  00099f3c: mov r0,r6
  00099f3e: movs r1,#0x0
  00099f40: movs r2,#0x0
  00099f42: mov r3,r4
  00099f44: blx 0x0006e8a8
  00099f48: ldr r0,[0x0009a154]
  00099f4a: add r0,pc
  00099f4c: ldr.w r8,[r0,#0x0]
  00099f50: ldr r0,[r5,#0x3c]
  00099f52: blx r8
  00099f54: ldr r0,[r5,#0x44]
  00099f56: blx r8
  00099f58: ldr r0,[r5,#0x54]
  00099f5a: blx 0x00070960
  00099f5e: movw r0,#0x84c0
  00099f62: blx 0x0006fb8c
  00099f66: ldr r0,[r5,#0x50]
  00099f68: blx 0x0006ee48
  00099f6c: ldr r0,[r5,#0x6c]
  00099f6e: blx 0x0006ee54
  00099f72: ldr r0,[r5,#0x58]
  00099f74: blx r11
  00099f76: ldr r0,[r5,#0x60]
  00099f78: blx r11
  00099f7a: ldr r0,[r5,#0x5c]
  00099f7c: movs r1,#0x1
  00099f7e: movs r2,#0x0
  00099f80: mov r3,r9
  00099f82: blx 0x00070990
  00099f86: ldr.w r0,[r6,#0x380]
  00099f8a: movs r4,#0x0
  00099f8c: movw r2,#0x1406
  00099f90: movs r3,#0x0
  00099f92: ldr r1,[r0,#0x4]
  00099f94: ldr r0,[r5,#0x58]
  00099f96: strd r4,r1,[sp,#0x0]
  00099f9a: movs r1,#0x3
  00099f9c: blx r10
  00099f9e: ldr.w r0,[r6,#0x380]
  00099fa2: movw r2,#0x1406
  00099fa6: movs r3,#0x0
  00099fa8: ldr r1,[r0,#0x8]
  00099faa: ldr r0,[r5,#0x60]
  00099fac: strd r4,r1,[sp,#0x0]
  00099fb0: movs r1,#0x2
  00099fb2: blx r10
  00099fb4: ldrd r0,r1,[r5,#0x68]
  00099fb8: vldr.32 s0,[r1,#0x10]
  00099fbc: vcvt.f32.s32 s0,s0
  00099fc0: vmov r1,s0
  00099fc4: blx 0x00070978
  00099fc8: mov.w r0,#0x4000
  00099fcc: blx 0x0006eed8
  00099fd0: mov r0,r6
  00099fd2: blx 0x0006e8cc
  00099fd6: mov r4,r0
  00099fd8: mov r0,r6
  00099fda: blx 0x0006e8d8
  00099fde: str r0,[sp,#0x0]
  00099fe0: mov r0,r6
  00099fe2: movs r1,#0x0
  00099fe4: movs r2,#0x0
  00099fe6: mov r3,r4
  00099fe8: blx 0x0006e8a8
  00099fec: ldr r0,[r5,#0x58]
  00099fee: blx r8
  00099ff0: ldr r0,[r5,#0x60]
  00099ff2: blx r8
  00099ff4: ldr r0,[sp,#0x1c]
  00099ff6: ldr r4,[sp,#0x18]
  00099ff8: subs r0,#0x1
  00099ffa: cmp r0,#0x0
  00099ffc: bne.w 0x00099e9e
  0009a000: ldr r0,[0x0009a148]
  0009a002: add r0,pc
  0009a004: ldr r0,[r0,#0x0]
  0009a006: ldr r0,[r0,#0x0]
  0009a008: cmp r0,#0x3
  0009a00a: bhi 0x0009a020
  0009a00c: ldr.w r8,[sp,#0x10]
  0009a010: ldrd r4,r9,[sp,#0x8]
  0009a014: tbb [pc,r0]
  0009a01c: ldr r4,[r5,#0x34]
  0009a01e: b 0x0009a032
  0009a020: ldrd r9,r8,[sp,#0xc]
  0009a024: b 0x0009a03e
  0009a030: ldr r4,[r5,#0x6c]
  0009a032: add.w r0,r5,#0x70
  0009a036: str r0,[sp,#0x18]
  0009a038: ldr r0,[sp,#0x18]
  0009a03a: ldr.w r10,[r0,#0x0]
  0009a03e: ldr r0,[r5,#0x74]
  0009a040: blx 0x00070960
  0009a044: movw r0,#0x84c0
  0009a048: blx 0x0006fb8c
  0009a04c: mov r0,r4
  0009a04e: blx 0x0006ee48
  0009a052: movw r0,#0x84c1
  0009a056: blx 0x0006fb8c
  0009a05a: mov r0,r10
  0009a05c: blx 0x0006ee48
  0009a060: ldr.w r0,[r9,#0x0]
  0009a064: cbz r0,0x0009a06a
  0009a066: blx 0x0006ee54
  0009a06a: ldr r0,[r5,#0x78]
  0009a06c: blx 0x00070a5c
  0009a070: ldr.w r0,[r5,#0x80]
  0009a074: blx 0x00070a5c
  0009a078: ldr r0,[r5,#0x7c]
  0009a07a: movs r1,#0x1
  0009a07c: ldr r3,[sp,#0x14]
  0009a07e: movs r2,#0x0
  0009a080: movs r4,#0x0
  0009a082: blx 0x00070990
  0009a086: ldr.w r1,[r6,#0x380]
  0009a08a: movw r2,#0x1406
  0009a08e: ldr r0,[r5,#0x78]
  0009a090: movs r3,#0x0
  0009a092: ldr r1,[r1,#0x4]
  0009a094: strd r4,r1,[sp,#0x0]
  0009a098: movs r1,#0x3
  0009a09a: blx 0x000709c0
  0009a09e: ldr.w r1,[r6,#0x380]
  0009a0a2: movw r2,#0x1406
  0009a0a6: ldr.w r0,[r5,#0x80]
  0009a0aa: movs r3,#0x0
  0009a0ac: ldr r1,[r1,#0x8]
  0009a0ae: strd r4,r1,[sp,#0x0]
  0009a0b2: movs r1,#0x2
  0009a0b4: blx 0x000709c0
  0009a0b8: mov.w r0,#0x4000
  0009a0bc: blx 0x0006eed8
  0009a0c0: mov r0,r6
  0009a0c2: blx 0x0006e8cc
  0009a0c6: mov r4,r0
  0009a0c8: mov r0,r6
  0009a0ca: blx 0x0006e8d8
  0009a0ce: str r0,[sp,#0x0]
  0009a0d0: mov r0,r6
  0009a0d2: movs r1,#0x0
  0009a0d4: movs r2,#0x0
  0009a0d6: mov r3,r4
  0009a0d8: blx 0x0006e8a8
  0009a0dc: ldr r0,[r5,#0x78]
  0009a0de: blx 0x00070a44
  0009a0e2: ldr.w r0,[r5,#0x80]
  0009a0e6: blx 0x00070a44
  0009a0ea: movw r0,#0xbe2
  0009a0ee: blx 0x0006edb8
  0009a0f2: movw r0,#0x302
  0009a0f6: movw r1,#0x303
  0009a0fa: blx 0x0006fe68
  0009a0fe: movw r0,#0x84c0
  0009a102: blx 0x0006fb8c
  0009a106: ldr.w r0,[r9,#0x0]
  0009a10a: cbz r0,0x0009a110
  0009a10c: blx 0x0006ee60
  0009a110: ldr r0,[sp,#0x5c]
  0009a112: ldr.w r1,[r8,#0x0]
  0009a116: subs r0,r1,r0
  0009a118: itttt eq
  0009a11a: add.eq sp,#0x60
  0009a11c: vpop.eq {d8,d9,d10}
  0009a120: add.eq sp,#0x4
  0009a122: pop.eq.w {r8,r9,r10,r11}
  0009a126: it eq
  0009a128: pop.eq {r4,r5,r6,r7,pc}
  0009a12a: blx 0x0006e824
```
