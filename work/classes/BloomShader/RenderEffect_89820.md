# BloomShader::RenderEffect
elf 0x89820 body 1156
Sig: undefined __thiscall RenderEffect(BloomShader * this, FBOContainer * param_1, Engine * param_2)

## decompile
```c

/* AbyssEngine::BloomShader::RenderEffect(AbyssEngine::FBOContainer*, AbyssEngine::Engine*) */

void __thiscall
AbyssEngine::BloomShader::RenderEffect(BloomShader *this,FBOContainer *param_1,Engine *param_2)

{
  int *piVar1;
  char *pcVar2;
  undefined4 uVar3;
  undefined4 *puVar4;
  int iVar5;
  int iVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  Engine *pEVar9;
  code *pcVar10;
  code *pcVar11;
  code *pcVar12;
  uint in_fpscr;
  float fVar13;
  undefined4 uVar14;
  undefined4 uVar15;
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
  
  piVar1 = *(int **)(DAT_00099bd4 + 0x9983a);
  local_44 = *piVar1;
  pcVar2 = *(char **)(DAT_00099bd8 + 0x9983e);
  *(undefined4 *)(param_2 + 0x3e4) = *(undefined4 *)(this + 4);
  if (*pcVar2 != '\0') {
    *pcVar2 = '\0';
    InternalInit((Engine *)this);
    FBOContainer::BeginCapture(*(FBOContainer **)(this + 0x70));
    glClearColor(0,0,0,0x3f800000);
    glClear(0x4000);
    FBOContainer::EndCapture();
  }
  uVar7 = 0;
  uVar8 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  uVar14 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  uVar15 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  *(undefined4 *)(param_2 + 0x3b4) = 0;
  *(undefined4 *)(param_2 + 0x3b8) = uVar8;
  *(undefined4 *)(param_2 + 0x3bc) = uVar14;
  *(undefined4 *)(param_2 + 0x3c0) = uVar15;
  *(undefined4 *)(param_2 + 0x3a4) = 0;
  *(undefined4 *)(param_2 + 0x3a8) = uVar8;
  *(undefined4 *)(param_2 + 0x3ac) = uVar14;
  *(undefined4 *)(param_2 + 0x3b0) = uVar15;
  *(undefined4 *)(param_2 + 0x394) = 0;
  *(undefined4 *)(param_2 + 0x398) = uVar8;
  *(undefined4 *)(param_2 + 0x39c) = uVar14;
  *(undefined4 *)(param_2 + 0x3a0) = uVar15;
  *(undefined4 *)(param_2 + 900) = 0;
  *(undefined4 *)(param_2 + 0x388) = uVar8;
  *(undefined4 *)(param_2 + 0x38c) = uVar14;
  *(undefined4 *)(param_2 + 0x390) = uVar15;
  uVar3 = Engine::GetDisplayWidth();
  fVar13 = (float)VectorSignedToFloat(uVar3,(byte)(in_fpscr >> 0x16) & 3);
  *(float *)(param_2 + 900) = 2.0 / fVar13;
  uVar3 = Engine::GetDisplayHeight();
  uStack_50 = DAT_00099cc0;
  local_58 = DAT_00099cb8;
  fVar13 = (float)VectorSignedToFloat(uVar3,(byte)(in_fpscr >> 0x16) & 3);
  *(undefined4 *)(param_2 + 0x3ac) = DAT_00099bdc;
  uVar3 = DAT_00099cb4;
  *(undefined4 *)(param_2 + 0x3c0) = 0x3f800000;
  *(undefined4 *)(param_2 + 0x3b4) = uVar3;
  *(undefined4 *)(param_2 + 0x3b8) = 0x3f800000;
  puVar4 = (undefined4 *)((uint)local_80 | 4);
  *(float *)(param_2 + 0x398) = -(2.0 / fVar13);
  *puVar4 = uVar7;
  puVar4[1] = uVar8;
  puVar4[2] = uVar14;
  puVar4[3] = uVar15;
  local_80[0] = 0x3f800000;
  local_6c = 0x3f800000;
  local_48 = 0x3f800000;
  local_68 = uVar7;
  uStack_64 = uVar8;
  uStack_60 = uVar14;
  uStack_5c = uVar15;
  Engine::SetWorldViewMatrix((Matrix *)param_2);
  glDisable(0xb71);
  glDepthMask(0);
  glDisable(0xbe2);
  glUseProgram(*(undefined4 *)(this + 0x20));
  glActiveTexture(0x84c0);
  FBOContainer::Activate();
  FBOContainer::BeginCapture(*(FBOContainer **)(this + 0x34));
  glEnableVertexAttribArray(*(undefined4 *)(this + 0x24));
  glEnableVertexAttribArray(*(undefined4 *)(this + 0x2c));
  pEVar9 = param_2 + 0x104;
  glUniformMatrix4fv(*(undefined4 *)(this + 0x28),1,0);
  glVertexAttribPointer
            (*(undefined4 *)(this + 0x24),3,0x1406,0,0,
             *(undefined4 *)(*(int *)(param_2 + 0x380) + 4));
  glVertexAttribPointer
            (*(undefined4 *)(this + 0x2c),2,0x1406,0,0,
             *(undefined4 *)(*(int *)(param_2 + 0x380) + 8));
  glClear(0x4000);
  iVar5 = Engine::GetDisplayWidth();
  Engine::GetDisplayHeight();
  Engine::DrawQuad((int)param_2,0,0,iVar5);
  glDisableVertexAttribArray(*(undefined4 *)(this + 0x24));
  glDisableVertexAttribArray(*(undefined4 *)(this + 0x2c));
  for (iVar5 = 6; iVar5 != 0; iVar5 = iVar5 + -1) {
    glUseProgram(*(undefined4 *)(this + 0x38));
    glActiveTexture(0x84c0);
    FBOContainer::Activate();
    FBOContainer::BeginCapture(*(FBOContainer **)(this + 0x50));
    pcVar11 = *(code **)(DAT_00099ccc + 0x99a1c);
    (*pcVar11)(*(undefined4 *)(this + 0x3c));
    (*pcVar11)(*(undefined4 *)(this + 0x44));
    glUniformMatrix4fv(*(undefined4 *)(this + 0x40),1,0,pEVar9);
    pcVar10 = *(code **)(DAT_00099cd0 + 0x99a4a);
    (*pcVar10)(*(undefined4 *)(this + 0x3c),3,0x1406,0,0,
               *(undefined4 *)(*(int *)(param_2 + 0x380) + 4));
    (*pcVar10)(*(undefined4 *)(this + 0x44),2,0x1406,0,0,
               *(undefined4 *)(*(int *)(param_2 + 0x380) + 8));
    uVar7 = VectorSignedToFloat(*(undefined4 *)(*(int *)(this + 0x50) + 0xc),
                                (byte)(in_fpscr >> 0x16) & 3);
    glUniform1f(*(undefined4 *)(this + 0x4c),uVar7);
    glClear(0x4000);
    iVar6 = Engine::GetDisplayWidth();
    Engine::GetDisplayHeight();
    Engine::DrawQuad((int)param_2,0,0,iVar6);
    pcVar12 = *(code **)(DAT_00099cd4 + 0x99aa6);
    (*pcVar12)(*(undefined4 *)(this + 0x3c));
    (*pcVar12)(*(undefined4 *)(this + 0x44));
    glUseProgram(*(undefined4 *)(this + 0x54));
    glActiveTexture(0x84c0);
    FBOContainer::Activate();
    FBOContainer::BeginCapture(*(FBOContainer **)(this + 0x6c));
    (*pcVar11)(*(undefined4 *)(this + 0x58));
    (*pcVar11)(*(undefined4 *)(this + 0x60));
    glUniformMatrix4fv(*(undefined4 *)(this + 0x5c),1,0,pEVar9);
    (*pcVar10)(*(undefined4 *)(this + 0x58),3,0x1406,0,0,
               *(undefined4 *)(*(int *)(param_2 + 0x380) + 4));
    (*pcVar10)(*(undefined4 *)(this + 0x60),2,0x1406,0,0,
               *(undefined4 *)(*(int *)(param_2 + 0x380) + 8));
    uVar7 = VectorSignedToFloat(*(undefined4 *)(*(int *)(this + 0x6c) + 0x10),
                                (byte)(in_fpscr >> 0x16) & 3);
    glUniform1f(*(undefined4 *)(this + 0x68),uVar7);
    glClear(0x4000);
    iVar6 = Engine::GetDisplayWidth();
    Engine::GetDisplayHeight();
    Engine::DrawQuad((int)param_2,0,0,iVar6);
    (*pcVar12)(*(undefined4 *)(this + 0x58));
    (*pcVar12)(*(undefined4 *)(this + 0x60));
  }
  if (**(uint **)(DAT_00099cc8 + 0x99b62) < 4) {
                    /* WARNING: Could not recover jumptable at 0x00099b6c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    (*(code *)(&switchD_00099b6c::switchdataD_00099b70 +
              (uint)(&switchD_00099b6c::switchdataD_00099b70)[**(uint **)(DAT_00099cc8 + 0x99b62)] *
              2))();
    return;
  }
  glUseProgram(*(undefined4 *)(this + 0x74));
  glActiveTexture(0x84c0);
  FBOContainer::Activate();
  glActiveTexture(0x84c1);
  FBOContainer::Activate();
  glBindFramebuffer(0x8d40,*(undefined4 *)(param_2 + 0x40c));
  if (*(int *)(**(int **)(param_2 + 0x30) + 0x30) == 2) {
    uVar7 = Engine::GetDisplayWidth();
    uVar8 = Engine::GetDisplayHeight();
  }
  else {
    uVar7 = Engine::GetDisplayHeight();
    uVar8 = Engine::GetDisplayWidth();
  }
  glViewport(0,0,uVar7,uVar8);
  glEnableVertexAttribArray(*(undefined4 *)(this + 0x78));
  glEnableVertexAttribArray(*(undefined4 *)(this + 0x80));
  glUniformMatrix4fv(*(undefined4 *)(this + 0x7c),1,0,pEVar9);
  glVertexAttribPointer
            (*(undefined4 *)(this + 0x78),3,0x1406,0,0,
             *(undefined4 *)(*(int *)(param_2 + 0x380) + 4));
  glVertexAttribPointer
            (*(undefined4 *)(this + 0x80),2,0x1406,0,0,
             *(undefined4 *)(*(int *)(param_2 + 0x380) + 8));
  glClear(0x4000);
  iVar5 = Engine::GetDisplayWidth();
  Engine::GetDisplayHeight();
  Engine::DrawQuad((int)param_2,0,0,iVar5);
  glDisableVertexAttribArray(*(undefined4 *)(this + 0x78));
  glDisableVertexAttribArray(*(undefined4 *)(this + 0x80));
  glEnable(0xbe2);
  glBlendFunc(0x302,0x303);
  glActiveTexture(0x84c0);
  if (*piVar1 != local_44) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  00099820: push {r4,r5,r6,r7,lr}
  00099822: add r7,sp,#0xc
  00099824: push {r7,r8,r9,r10,r11}
  00099828: vpush {d8,d9,d10}
  0009982c: sub sp,#0x60
  0009982e: mov r4,r0
  00099830: ldr r0,[0x00099bd4]
  00099832: mov r6,r1
  00099834: ldr r1,[0x00099bd8]
  00099836: add r0,pc
  00099838: mov r5,r2
  0009983a: add r1,pc
  0009983c: ldr r0,[r0,#0x0]
  0009983e: str r0,[sp,#0xc]
  00099840: ldr r2,[r0,#0x0]
  00099842: ldr r0,[r1,#0x0]
  00099844: str r2,[sp,#0x5c]
  00099846: ldr r1,[r4,#0x4]
  00099848: str.w r1,[r5,#0x3e4]
  0009984c: ldrb r1,[r0,#0x0]
  0009984e: cbz r1,0x0009987e
  00099850: movs r1,#0x0
  00099852: strb r1,[r0,#0x0]
  00099854: mov r0,r4
  00099856: mov r1,r5
  00099858: blx 0x00070be8
  0009985c: ldr r0,[r4,#0x70]
  0009985e: blx 0x0006ee54
  00099862: movs r0,#0x0
  00099864: movs r1,#0x0
  00099866: movs r2,#0x0
  00099868: mov.w r3,#0x3f800000
  0009986c: blx 0x0006eecc
  00099870: mov.w r0,#0x4000
  00099874: blx 0x0006eed8
  00099878: ldr r0,[r4,#0x70]
  0009987a: blx 0x0006ee60
  0009987e: vmov.i32 q4,#0x0
  00099882: add.w r0,r5,#0x3b4
  00099886: vst1.32 {d8,d9},[r0]
  0009988a: add.w r0,r5,#0x3a4
  0009988e: vst1.32 {d8,d9},[r0]
  00099892: add.w r0,r5,#0x394
  00099896: vst1.32 {d8,d9},[r0]
  0009989a: add.w r0,r5,#0x384
  0009989e: vst1.32 {d8,d9},[r0]
  000998a2: mov r0,r5
  000998a4: blx 0x0006e8cc
  000998a8: vmov s0,r0
  000998ac: mov r0,r5
  000998ae: vmov.f32 s20,0x40000000
  000998b2: vcvt.f32.s32 s0,s0
  000998b6: vdiv.f32 s0,s20,s0
  000998ba: vstr.32 s0,[r5,#0x384]
  000998be: blx 0x0006e8d8
  000998c2: vmov s0,r0
  000998c6: adr.w r1,0x99cb8
  000998ca: vld1.64 {d16,d17},[r1]
  000998ce: mov.w r2,#0x3f800000
  000998d2: vcvt.f32.s32 s0,s0
  000998d6: ldr r1,[0x00099bdc]
  000998d8: str.w r1,[r5,#0x3ac]
  000998dc: add r1,sp,#0x20
  000998de: ldr r0,[0x00099cb4]
  000998e0: str.w r2,[r5,#0x3c0]
  000998e4: strd r0,r2,[r5,#0x3b4]
  000998e8: orr r0,r1,#0x4
  000998ec: vdiv.f32 s0,s20,s0
  000998f0: vneg.f32 s0,s0
  000998f4: vstr.32 s0,[r5,#0x398]
  000998f8: vst1.32 {d8,d9},[r0]
  000998fc: add.w r0,r1,#0x18
  00099900: vst1.64 {d8,d9},[r0]
  00099904: add.w r0,r1,#0x28
  00099908: vst1.64 {d16,d17},[r0]
  0009990c: mov r0,r5
  0009990e: str r2,[sp,#0x20]
  00099910: str r2,[sp,#0x34]
  00099912: str r2,[sp,#0x58]
  00099914: blx 0x0006fae4
  00099918: movw r0,#0xb71
  0009991c: blx 0x0006edc4
  00099920: movs r0,#0x0
  00099922: str r5,[sp,#0x1c]
  00099924: mov.w r8,#0x0
  00099928: blx 0x0006eec0
  0009992c: movw r0,#0xbe2
  00099930: blx 0x0006edc4
  00099934: ldr r0,[r4,#0x20]
  00099936: blx 0x00070960
  0009993a: movw r0,#0x84c0
  0009993e: blx 0x0006fb8c
  00099942: mov r0,r6
  00099944: str r6,[sp,#0x8]
  00099946: blx 0x0006ee48
  0009994a: mov r6,r4
  0009994c: ldr.w r0,[r6,#0x34]!
  00099950: blx 0x0006ee54
  00099954: ldr.w r0,[r6,#-0x10]
  00099958: blx 0x00070a5c
  0009995c: ldr.w r0,[r6,#-0x8]
  00099960: blx 0x00070a5c
  00099964: ldr r1,[sp,#0x1c]
  00099966: movs r2,#0x0
  00099968: ldr.w r0,[r6,#-0xc]
  0009996c: add.w r3,r1,#0x104
  00099970: movs r1,#0x1
  00099972: str r3,[sp,#0x10]
  00099974: blx 0x00070990
  00099978: ldr r0,[sp,#0x1c]
  0009997a: movw r2,#0x1406
  0009997e: movs r3,#0x0
  00099980: ldr.w r1,[r0,#0x380]
  00099984: ldr.w r0,[r6,#-0x10]
  00099988: ldr r1,[r1,#0x4]
  0009998a: strd r8,r1,[sp,#0x0]
  0009998e: movs r1,#0x3
  00099990: blx 0x000709c0
  00099994: ldr r0,[sp,#0x1c]
  00099996: movw r2,#0x1406
  0009999a: movs r3,#0x0
  0009999c: ldr r5,[sp,#0x1c]
  0009999e: ldr.w r0,[r0,#0x380]
  000999a2: ldr r1,[r0,#0x8]
  000999a4: ldr.w r0,[r6,#-0x8]
  000999a8: str r1,[sp,#0x4]
  000999aa: movs r1,#0x2
  000999ac: str.w r8,[sp,#0x0]
  000999b0: blx 0x000709c0
  000999b4: mov.w r0,#0x4000
  000999b8: blx 0x0006eed8
  000999bc: mov r0,r5
  000999be: blx 0x0006e8cc
  000999c2: mov r9,r0
  000999c4: mov r0,r5
  000999c6: blx 0x0006e8d8
  000999ca: str r0,[sp,#0x0]
  000999cc: mov r0,r5
  000999ce: movs r1,#0x0
  000999d0: movs r2,#0x0
  000999d2: mov r3,r9
  000999d4: blx 0x0006e8a8
  000999d8: ldr.w r0,[r6,#-0x10]
  000999dc: blx 0x00070a44
  000999e0: ldr.w r0,[r6,#-0x8]
  000999e4: blx 0x00070a44
  000999e8: ldr.w r8,[sp,#0x10]
  000999ec: add.w r0,r6,#0x38
  000999f0: str r0,[sp,#0x14]
  000999f2: movs r0,#0x6
  000999f4: b 0x00099b56
  000999f6: str r0,[sp,#0x18]
  000999f8: ldr r0,[r4,#0x38]
  000999fa: ldr r6,[r6,#0x0]
  000999fc: blx 0x00070960
  00099a00: movw r0,#0x84c0
  00099a04: blx 0x0006fb8c
  00099a08: mov r0,r6
  00099a0a: blx 0x0006ee48
  00099a0e: ldr r0,[r4,#0x50]
  00099a10: blx 0x0006ee54
  00099a14: ldr r1,[0x00099ccc]
  00099a16: ldr r0,[r4,#0x3c]
  00099a18: add r1,pc
  00099a1a: ldr.w r10,[r1,#0x0]
  00099a1e: blx r10
  00099a20: ldr r0,[r4,#0x44]
  00099a22: blx r10
  00099a24: ldr r0,[r4,#0x40]
  00099a26: movs r1,#0x1
  00099a28: movs r2,#0x0
  00099a2a: mov r3,r8
  00099a2c: blx 0x00070990
  00099a30: ldr.w r0,[r5,#0x380]
  00099a34: movs r5,#0x0
  00099a36: movw r2,#0x1406
  00099a3a: movs r3,#0x0
  00099a3c: ldr r1,[r0,#0x4]
  00099a3e: ldr r0,[r4,#0x3c]
  00099a40: strd r5,r1,[sp,#0x0]
  00099a44: ldr r1,[0x00099cd0]
  00099a46: add r1,pc
  00099a48: ldr.w r9,[r1,#0x0]
  00099a4c: movs r1,#0x3
  00099a4e: blx r9
  00099a50: ldr r0,[sp,#0x1c]
  00099a52: movw r2,#0x1406
  00099a56: movs r3,#0x0
  00099a58: ldr.w r0,[r0,#0x380]
  00099a5c: ldr r1,[r0,#0x8]
  00099a5e: ldr r0,[r4,#0x44]
  00099a60: strd r5,r1,[sp,#0x0]
  00099a64: movs r1,#0x2
  00099a66: blx r9
  00099a68: ldrd r0,r1,[r4,#0x4c]
  00099a6c: vldr.32 s0,[r1,#0xc]
  00099a70: vcvt.f32.s32 s0,s0
  00099a74: vmov r1,s0
  00099a78: blx 0x00070978
  00099a7c: mov.w r0,#0x4000
  00099a80: blx 0x0006eed8
  00099a84: ldr r0,[sp,#0x1c]
  00099a86: blx 0x0006e8cc
  00099a8a: mov r6,r0
  00099a8c: ldr r0,[sp,#0x1c]
  00099a8e: blx 0x0006e8d8
  00099a92: str r0,[sp,#0x0]
  00099a94: movs r1,#0x0
  00099a96: ldr r0,[sp,#0x1c]
  00099a98: movs r2,#0x0
  00099a9a: mov r3,r6
  00099a9c: blx 0x0006e8a8
  00099aa0: ldr r0,[0x00099cd4]
  00099aa2: add r0,pc
  00099aa4: ldr.w r11,[r0,#0x0]
  00099aa8: ldr r0,[r4,#0x3c]
  00099aaa: blx r11
  00099aac: ldr r0,[r4,#0x44]
  00099aae: blx r11
  00099ab0: ldr r0,[r4,#0x54]
  00099ab2: blx 0x00070960
  00099ab6: movw r0,#0x84c0
  00099aba: blx 0x0006fb8c
  00099abe: ldr r0,[r4,#0x50]
  00099ac0: blx 0x0006ee48
  00099ac4: ldr r0,[r4,#0x6c]
  00099ac6: blx 0x0006ee54
  00099aca: ldr r0,[r4,#0x58]
  00099acc: blx r10
  00099ace: ldr r0,[r4,#0x60]
  00099ad0: blx r10
  00099ad2: ldr r0,[r4,#0x5c]
  00099ad4: movs r1,#0x1
  00099ad6: movs r2,#0x0
  00099ad8: mov r3,r8
  00099ada: blx 0x00070990
  00099ade: ldr r0,[sp,#0x1c]
  00099ae0: movw r2,#0x1406
  00099ae4: movs r3,#0x0
  00099ae6: ldr.w r0,[r0,#0x380]
  00099aea: ldr r1,[r0,#0x4]
  00099aec: ldr r0,[r4,#0x58]
  00099aee: strd r5,r1,[sp,#0x0]
  00099af2: movs r1,#0x3
  00099af4: blx r9
  00099af6: ldr r0,[sp,#0x1c]
  00099af8: movw r2,#0x1406
  00099afc: movs r3,#0x0
  00099afe: ldr.w r0,[r0,#0x380]
  00099b02: ldr r1,[r0,#0x8]
  00099b04: ldr r0,[r4,#0x60]
  00099b06: strd r5,r1,[sp,#0x0]
  00099b0a: movs r1,#0x2
  00099b0c: blx r9
  00099b0e: ldr r5,[sp,#0x1c]
  00099b10: ldrd r0,r1,[r4,#0x68]
  00099b14: vldr.32 s0,[r1,#0x10]
  00099b18: vcvt.f32.s32 s0,s0
  00099b1c: vmov r1,s0
  00099b20: blx 0x00070978
  00099b24: mov.w r0,#0x4000
  00099b28: blx 0x0006eed8
  00099b2c: mov r0,r5
  00099b2e: blx 0x0006e8cc
  00099b32: mov r6,r0
  00099b34: mov r0,r5
  00099b36: blx 0x0006e8d8
  00099b3a: str r0,[sp,#0x0]
  00099b3c: mov r0,r5
  00099b3e: movs r1,#0x0
  00099b40: movs r2,#0x0
  00099b42: mov r3,r6
  00099b44: blx 0x0006e8a8
  00099b48: ldr r0,[r4,#0x58]
  00099b4a: blx r11
  00099b4c: ldr r0,[r4,#0x60]
  00099b4e: blx r11
  00099b50: ldr r0,[sp,#0x18]
  00099b52: ldr r6,[sp,#0x14]
  00099b54: subs r0,#0x1
  00099b56: cmp r0,#0x0
  00099b58: bne.w 0x000999f6
  00099b5c: ldr r0,[0x00099cc8]
  00099b5e: add r0,pc
  00099b60: ldr r0,[r0,#0x0]
  00099b62: ldr r0,[r0,#0x0]
  00099b64: cmp r0,#0x3
  00099b66: bhi 0x00099b78
  00099b68: ldrd r6,r8,[sp,#0x8]
  00099b6c: tbb [pc,r0]
  00099b74: ldr r6,[r4,#0x34]
  00099b76: b 0x00099b80
  00099b78: ldr.w r8,[sp,#0xc]
  00099b7c: b 0x00099b8c
  00099b7e: ldr r6,[r4,#0x6c]
  00099b80: add.w r0,r4,#0x70
  00099b84: str r0,[sp,#0x14]
  00099b86: ldr r0,[sp,#0x14]
  00099b88: ldr.w r9,[r0,#0x0]
  00099b8c: ldr r0,[r4,#0x74]
  00099b8e: blx 0x00070960
  00099b92: movw r0,#0x84c0
  00099b96: blx 0x0006fb8c
  00099b9a: mov r0,r6
  00099b9c: blx 0x0006ee48
  00099ba0: movw r0,#0x84c1
  00099ba4: blx 0x0006fb8c
  00099ba8: mov r0,r9
  00099baa: blx 0x0006ee48
  00099bae: ldr.w r1,[r5,#0x40c]
  00099bb2: movw r0,#0x8d40
  00099bb6: blx 0x0006ee6c
  00099bba: ldr r0,[r5,#0x30]
  00099bbc: ldr r0,[r0,#0x0]
  00099bbe: ldr r0,[r0,#0x30]
  00099bc0: cmp r0,#0x2
  00099bc2: bne 0x00099be0
  00099bc4: mov r0,r5
  00099bc6: blx 0x0006e8cc
  00099bca: mov r6,r0
  00099bcc: mov r0,r5
  00099bce: blx 0x0006e8d8
  00099bd2: b 0x00099bee
  00099be0: mov r0,r5
  00099be2: blx 0x0006e8d8
  00099be6: mov r6,r0
  00099be8: mov r0,r5
  00099bea: blx 0x0006e8cc
  00099bee: mov r3,r0
  00099bf0: movs r0,#0x0
  00099bf2: movs r1,#0x0
  00099bf4: mov r2,r6
  00099bf6: blx 0x0006eda0
  00099bfa: ldr r0,[r4,#0x78]
  00099bfc: blx 0x00070a5c
  00099c00: ldr.w r0,[r4,#0x80]
  00099c04: blx 0x00070a5c
  00099c08: ldr r0,[r4,#0x7c]
  00099c0a: movs r1,#0x1
  00099c0c: ldr r3,[sp,#0x10]
  00099c0e: movs r2,#0x0
  00099c10: movs r6,#0x0
  00099c12: blx 0x00070990
  00099c16: ldr.w r1,[r5,#0x380]
  00099c1a: movw r2,#0x1406
  00099c1e: ldr r0,[r4,#0x78]
  00099c20: movs r3,#0x0
  00099c22: ldr r1,[r1,#0x4]
  00099c24: strd r6,r1,[sp,#0x0]
  00099c28: movs r1,#0x3
  00099c2a: blx 0x000709c0
  00099c2e: ldr.w r1,[r5,#0x380]
  00099c32: movw r2,#0x1406
  00099c36: ldr.w r0,[r4,#0x80]
  00099c3a: movs r3,#0x0
  00099c3c: ldr r1,[r1,#0x8]
  00099c3e: strd r6,r1,[sp,#0x0]
  00099c42: movs r1,#0x2
  00099c44: blx 0x000709c0
  00099c48: mov.w r0,#0x4000
  00099c4c: blx 0x0006eed8
  00099c50: mov r0,r5
  00099c52: blx 0x0006e8cc
  00099c56: mov r6,r0
  00099c58: mov r0,r5
  00099c5a: blx 0x0006e8d8
  00099c5e: str r0,[sp,#0x0]
  00099c60: mov r0,r5
  00099c62: movs r1,#0x0
  00099c64: movs r2,#0x0
  00099c66: mov r3,r6
  00099c68: blx 0x0006e8a8
  00099c6c: ldr r0,[r4,#0x78]
  00099c6e: blx 0x00070a44
  00099c72: ldr.w r0,[r4,#0x80]
  00099c76: blx 0x00070a44
  00099c7a: movw r0,#0xbe2
  00099c7e: blx 0x0006edb8
  00099c82: movw r0,#0x302
  00099c86: movw r1,#0x303
  00099c8a: blx 0x0006fe68
  00099c8e: movw r0,#0x84c0
  00099c92: blx 0x0006fb8c
  00099c96: ldr r0,[sp,#0x5c]
  00099c98: ldr.w r1,[r8,#0x0]
  00099c9c: subs r0,r1,r0
  00099c9e: itttt eq
  00099ca0: add.eq sp,#0x60
  00099ca2: vpop.eq {d8,d9,d10}
  00099ca6: add.eq sp,#0x4
  00099ca8: pop.eq.w {r8,r9,r10,r11}
  00099cac: it eq
  00099cae: pop.eq {r4,r5,r6,r7,pc}
  00099cb0: blx 0x0006e824
```
