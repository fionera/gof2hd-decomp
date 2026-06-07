# PostBWShader::RenderEffect
elf 0x8f030 body 490
Sig: undefined __thiscall RenderEffect(PostBWShader * this, FBOContainer * param_1, Engine * param_2)

## decompile
```c

/* AbyssEngine::PostBWShader::RenderEffect(AbyssEngine::FBOContainer*, AbyssEngine::Engine*) */

void __thiscall
AbyssEngine::PostBWShader::RenderEffect(PostBWShader *this,FBOContainer *param_1,Engine *param_2)

{
  undefined4 *puVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  int iVar4;
  undefined4 uVar5;
  int *piVar6;
  uint in_fpscr;
  float fVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  undefined4 local_70 [5];
  undefined4 local_5c;
  undefined4 local_58;
  undefined4 uStack_54;
  undefined4 uStack_50;
  undefined4 uStack_4c;
  undefined8 uStack_48;
  undefined8 uStack_40;
  undefined4 local_38;
  int local_34;
  
  uVar2 = 0;
  uVar3 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  uVar8 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  uVar9 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  piVar6 = *(int **)(DAT_0009f238 + 0x9f04c);
  local_34 = *piVar6;
  uVar5 = *(undefined4 *)(this + 4);
  *(undefined4 *)(param_2 + 0x3b4) = 0;
  *(undefined4 *)(param_2 + 0x3b8) = uVar3;
  *(undefined4 *)(param_2 + 0x3bc) = uVar8;
  *(undefined4 *)(param_2 + 0x3c0) = uVar9;
  *(undefined4 *)(param_2 + 0x3a4) = 0;
  *(undefined4 *)(param_2 + 0x3a8) = uVar3;
  *(undefined4 *)(param_2 + 0x3ac) = uVar8;
  *(undefined4 *)(param_2 + 0x3b0) = uVar9;
  *(undefined4 *)(param_2 + 0x394) = 0;
  *(undefined4 *)(param_2 + 0x398) = uVar3;
  *(undefined4 *)(param_2 + 0x39c) = uVar8;
  *(undefined4 *)(param_2 + 0x3a0) = uVar9;
  *(undefined4 *)(param_2 + 900) = 0;
  *(undefined4 *)(param_2 + 0x388) = uVar3;
  *(undefined4 *)(param_2 + 0x38c) = uVar8;
  *(undefined4 *)(param_2 + 0x390) = uVar9;
  *(undefined4 *)(param_2 + 0x3e4) = uVar5;
  uVar5 = Engine::GetDisplayWidth();
  fVar7 = (float)VectorSignedToFloat(uVar5,(byte)(in_fpscr >> 0x16) & 3);
  *(float *)(param_2 + 900) = 2.0 / fVar7;
  uVar5 = Engine::GetDisplayHeight();
  uStack_40 = DAT_0009f228;
  uStack_48 = DAT_0009f220;
  fVar7 = (float)VectorSignedToFloat(uVar5,(byte)(in_fpscr >> 0x16) & 3);
  *(undefined4 *)(param_2 + 0x3ac) = DAT_0009f234;
  uVar5 = DAT_0009f230;
  *(undefined4 *)(param_2 + 0x3c0) = 0x3f800000;
  *(undefined4 *)(param_2 + 0x3b4) = uVar5;
  *(undefined4 *)(param_2 + 0x3b8) = 0x3f800000;
  puVar1 = (undefined4 *)((uint)local_70 | 4);
  *(float *)(param_2 + 0x398) = -(2.0 / fVar7);
  *puVar1 = uVar2;
  puVar1[1] = uVar3;
  puVar1[2] = uVar8;
  puVar1[3] = uVar9;
  local_70[0] = 0x3f800000;
  local_5c = 0x3f800000;
  local_38 = 0x3f800000;
  local_58 = uVar2;
  uStack_54 = uVar3;
  uStack_50 = uVar8;
  uStack_4c = uVar9;
  Engine::SetWorldViewMatrix((Matrix *)param_2);
  glDisable(0xb71);
  glDepthMask(0);
  glDisable(0xbe2);
  glUseProgram(*(undefined4 *)(this + 4));
  glActiveTexture(0x84c0);
  FBOContainer::Activate();
  glBindFramebuffer(0x8d40,*(undefined4 *)(param_2 + 0x40c));
  glClear(0x4100);
  if (*(int *)(**(int **)(param_2 + 0x30) + 0x30) == 2) {
    uVar2 = Engine::GetDisplayWidth();
    uVar3 = Engine::GetDisplayHeight();
  }
  else {
    uVar2 = Engine::GetDisplayHeight();
    uVar3 = Engine::GetDisplayWidth();
  }
  glViewport(0,0,uVar2,uVar3);
  glEnableVertexAttribArray(*(undefined4 *)(this + 0x20));
  glEnableVertexAttribArray(*(undefined4 *)(this + 0x28));
  glUniformMatrix4fv(*(undefined4 *)(this + 0x24),1,0,param_2 + 0x104);
  glVertexAttribPointer
            (*(undefined4 *)(this + 0x20),3,0x1406,0,0,
             *(undefined4 *)(*(int *)(param_2 + 0x380) + 4));
  glVertexAttribPointer
            (*(undefined4 *)(this + 0x28),2,0x1406,0,0,
             *(undefined4 *)(*(int *)(param_2 + 0x380) + 8));
  glClear(0x4000);
  iVar4 = Engine::GetDisplayWidth();
  Engine::GetDisplayHeight();
  Engine::DrawQuad((int)param_2,0,0,iVar4);
  glDisableVertexAttribArray(*(undefined4 *)(this + 0x20));
  glDisableVertexAttribArray(*(undefined4 *)(this + 0x28));
  glEnable(0xbe2);
  glBlendFunc(0x302,0x303);
  glActiveTexture(0x84c0);
  *(undefined4 *)(param_2 + 0x7c) = 0xffffffff;
  if (*piVar6 != local_34) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  0009f030: push {r4,r5,r6,r7,lr}
  0009f032: add r7,sp,#0xc
  0009f034: push.w r8
  0009f038: vpush {d8,d9,d10}
  0009f03c: sub sp,#0x48
  0009f03e: mov r5,r0
  0009f040: ldr r0,[0x0009f238]
  0009f042: vmov.i32 q4,#0x0
  0009f046: mov r6,r1
  0009f048: add r0,pc
  0009f04a: mov r4,r2
  0009f04c: ldr.w r8,[r0,#0x0]
  0009f050: ldr.w r0,[r8,#0x0]
  0009f054: str r0,[sp,#0x44]
  0009f056: add.w r0,r2,#0x3b4
  0009f05a: ldr r1,[r5,#0x4]
  0009f05c: vst1.32 {d8,d9},[r0]
  0009f060: add.w r0,r2,#0x3a4
  0009f064: vst1.32 {d8,d9},[r0]
  0009f068: add.w r0,r2,#0x394
  0009f06c: vst1.32 {d8,d9},[r0]
  0009f070: add.w r0,r2,#0x384
  0009f074: vst1.32 {d8,d9},[r0]
  0009f078: mov r0,r2
  0009f07a: str.w r1,[r2,#0x3e4]
  0009f07e: blx 0x0006e8cc
  0009f082: vmov s0,r0
  0009f086: mov r0,r4
  0009f088: vmov.f32 s20,0x40000000
  0009f08c: vcvt.f32.s32 s0,s0
  0009f090: vdiv.f32 s0,s20,s0
  0009f094: vstr.32 s0,[r4,#0x384]
  0009f098: blx 0x0006e8d8
  0009f09c: vmov s0,r0
  0009f0a0: adr r1,[0x9f220]
  0009f0a2: vld1.64 {d16,d17},[r1]
  0009f0a6: mov.w r2,#0x3f800000
  0009f0aa: vcvt.f32.s32 s0,s0
  0009f0ae: ldr r1,[0x0009f234]
  0009f0b0: str.w r1,[r4,#0x3ac]
  0009f0b4: add r1,sp,#0x8
  0009f0b6: ldr r0,[0x0009f230]
  0009f0b8: str.w r2,[r4,#0x3c0]
  0009f0bc: strd r0,r2,[r4,#0x3b4]
  0009f0c0: orr r0,r1,#0x4
  0009f0c4: vdiv.f32 s0,s20,s0
  0009f0c8: vneg.f32 s0,s0
  0009f0cc: vstr.32 s0,[r4,#0x398]
  0009f0d0: vst1.32 {d8,d9},[r0]
  0009f0d4: add.w r0,r1,#0x18
  0009f0d8: vst1.64 {d8,d9},[r0]
  0009f0dc: add.w r0,r1,#0x28
  0009f0e0: vst1.64 {d16,d17},[r0]
  0009f0e4: mov r0,r4
  0009f0e6: str r2,[sp,#0x8]
  0009f0e8: str r2,[sp,#0x1c]
  0009f0ea: str r2,[sp,#0x40]
  0009f0ec: blx 0x0006fae4
  0009f0f0: movw r0,#0xb71
  0009f0f4: blx 0x0006edc4
  0009f0f8: movs r0,#0x0
  0009f0fa: blx 0x0006eec0
  0009f0fe: movw r0,#0xbe2
  0009f102: blx 0x0006edc4
  0009f106: ldr r0,[r5,#0x4]
  0009f108: blx 0x00070960
  0009f10c: movw r0,#0x84c0
  0009f110: blx 0x0006fb8c
  0009f114: mov r0,r6
  0009f116: blx 0x0006ee48
  0009f11a: ldr.w r1,[r4,#0x40c]
  0009f11e: movw r0,#0x8d40
  0009f122: blx 0x0006ee6c
  0009f126: mov.w r0,#0x4100
  0009f12a: blx 0x0006eed8
  0009f12e: ldr r0,[r4,#0x30]
  0009f130: ldr r0,[r0,#0x0]
  0009f132: ldr r0,[r0,#0x30]
  0009f134: cmp r0,#0x2
  0009f136: bne 0x0009f148
  0009f138: mov r0,r4
  0009f13a: blx 0x0006e8cc
  0009f13e: mov r6,r0
  0009f140: mov r0,r4
  0009f142: blx 0x0006e8d8
  0009f146: b 0x0009f156
  0009f148: mov r0,r4
  0009f14a: blx 0x0006e8d8
  0009f14e: mov r6,r0
  0009f150: mov r0,r4
  0009f152: blx 0x0006e8cc
  0009f156: mov r3,r0
  0009f158: movs r0,#0x0
  0009f15a: movs r1,#0x0
  0009f15c: mov r2,r6
  0009f15e: blx 0x0006eda0
  0009f162: ldr r0,[r5,#0x20]
  0009f164: blx 0x00070a5c
  0009f168: ldr r0,[r5,#0x28]
  0009f16a: blx 0x00070a5c
  0009f16e: ldr r0,[r5,#0x24]
  0009f170: add.w r3,r4,#0x104
  0009f174: movs r1,#0x1
  0009f176: movs r2,#0x0
  0009f178: movs r6,#0x0
  0009f17a: blx 0x00070990
  0009f17e: ldr.w r1,[r4,#0x380]
  0009f182: movw r2,#0x1406
  0009f186: ldr r0,[r5,#0x20]
  0009f188: movs r3,#0x0
  0009f18a: ldr r1,[r1,#0x4]
  0009f18c: strd r6,r1,[sp,#0x0]
  0009f190: movs r1,#0x3
  0009f192: blx 0x000709c0
  0009f196: ldr.w r1,[r4,#0x380]
  0009f19a: movw r2,#0x1406
  0009f19e: ldr r0,[r5,#0x28]
  0009f1a0: movs r3,#0x0
  0009f1a2: ldr r1,[r1,#0x8]
  0009f1a4: strd r6,r1,[sp,#0x0]
  0009f1a8: movs r1,#0x2
  0009f1aa: blx 0x000709c0
  0009f1ae: mov.w r0,#0x4000
  0009f1b2: blx 0x0006eed8
  0009f1b6: mov r0,r4
  0009f1b8: blx 0x0006e8cc
  0009f1bc: mov r6,r0
  0009f1be: mov r0,r4
  0009f1c0: blx 0x0006e8d8
  0009f1c4: str r0,[sp,#0x0]
  0009f1c6: mov r0,r4
  0009f1c8: movs r1,#0x0
  0009f1ca: movs r2,#0x0
  0009f1cc: mov r3,r6
  0009f1ce: blx 0x0006e8a8
  0009f1d2: ldr r0,[r5,#0x20]
  0009f1d4: blx 0x00070a44
  0009f1d8: ldr r0,[r5,#0x28]
  0009f1da: blx 0x00070a44
  0009f1de: movw r0,#0xbe2
  0009f1e2: blx 0x0006edb8
  0009f1e6: movw r0,#0x302
  0009f1ea: movw r1,#0x303
  0009f1ee: blx 0x0006fe68
  0009f1f2: movw r0,#0x84c0
  0009f1f6: blx 0x0006fb8c
  0009f1fa: mov.w r0,#0xffffffff
  0009f1fe: str r0,[r4,#0x7c]
  0009f200: ldr r0,[sp,#0x44]
  0009f202: ldr.w r1,[r8,#0x0]
  0009f206: subs r0,r1,r0
  0009f208: itttt eq
  0009f20a: add.eq sp,#0x48
  0009f20c: vpop.eq {d8,d9,d10}
  0009f210: pop.eq.w r8
  0009f214: pop.eq {r4,r5,r6,r7,pc}
  0009f216: blx 0x0006e824
```
