# DrawFBOShader::RenderEffect
elf 0x88560 body 520
Sig: undefined __thiscall RenderEffect(DrawFBOShader * this, FBOContainer * param_1, Engine * param_2)

## decompile
```c

/* AbyssEngine::DrawFBOShader::RenderEffect(AbyssEngine::FBOContainer*, AbyssEngine::Engine*) */

void __thiscall
AbyssEngine::DrawFBOShader::RenderEffect(DrawFBOShader *this,FBOContainer *param_1,Engine *param_2)

{
  undefined4 uVar1;
  undefined4 *puVar2;
  int iVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  code *pcVar6;
  int *piVar7;
  uint in_fpscr;
  float fVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  undefined4 local_78 [5];
  undefined4 local_64;
  undefined4 local_60;
  undefined4 uStack_5c;
  undefined4 uStack_58;
  undefined4 uStack_54;
  undefined8 uStack_50;
  undefined8 uStack_48;
  undefined4 local_40;
  int local_3c;
  
  uVar4 = 0;
  uVar5 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x4);
  uVar9 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0x8);
  uVar10 = *(undefined4 *)((undefined1  [16])0x0 + (undefined1  [16])0xc);
  piVar7 = *(int **)(DAT_00098780 + 0x9857c);
  local_3c = *piVar7;
  *(undefined4 *)(param_2 + 0x3b4) = 0;
  *(undefined4 *)(param_2 + 0x3b8) = uVar5;
  *(undefined4 *)(param_2 + 0x3bc) = uVar9;
  *(undefined4 *)(param_2 + 0x3c0) = uVar10;
  *(undefined4 *)(param_2 + 0x3a4) = 0;
  *(undefined4 *)(param_2 + 0x3a8) = uVar5;
  *(undefined4 *)(param_2 + 0x3ac) = uVar9;
  *(undefined4 *)(param_2 + 0x3b0) = uVar10;
  *(undefined4 *)(param_2 + 0x394) = 0;
  *(undefined4 *)(param_2 + 0x398) = uVar5;
  *(undefined4 *)(param_2 + 0x39c) = uVar9;
  *(undefined4 *)(param_2 + 0x3a0) = uVar10;
  *(undefined4 *)(param_2 + 900) = 0;
  *(undefined4 *)(param_2 + 0x388) = uVar5;
  *(undefined4 *)(param_2 + 0x38c) = uVar9;
  *(undefined4 *)(param_2 + 0x390) = uVar10;
  uVar1 = Engine::GetDisplayWidth();
  fVar8 = (float)VectorSignedToFloat(uVar1,(byte)(in_fpscr >> 0x16) & 3);
  *(float *)(param_2 + 900) = 2.0 / fVar8;
  uVar1 = Engine::GetDisplayHeight();
  uStack_48 = DAT_00098770;
  uStack_50 = DAT_00098768;
  fVar8 = (float)VectorSignedToFloat(uVar1,(byte)(in_fpscr >> 0x16) & 3);
  *(undefined4 *)(param_2 + 0x3ac) = DAT_0009877c;
  uVar1 = DAT_00098778;
  *(undefined4 *)(param_2 + 0x3c0) = 0x3f800000;
  *(undefined4 *)(param_2 + 0x3b4) = uVar1;
  *(undefined4 *)(param_2 + 0x3b8) = 0x3f800000;
  puVar2 = (undefined4 *)((uint)local_78 | 4);
  *(float *)(param_2 + 0x398) = -(2.0 / fVar8);
  *puVar2 = uVar4;
  puVar2[1] = uVar5;
  puVar2[2] = uVar9;
  puVar2[3] = uVar10;
  local_78[0] = 0x3f800000;
  local_64 = 0x3f800000;
  local_40 = 0x3f800000;
  local_60 = uVar4;
  uStack_5c = uVar5;
  uStack_58 = uVar9;
  uStack_54 = uVar10;
  Engine::SetWorldViewMatrix((Matrix *)param_2);
  glDisable(0xb71);
  glDepthMask(0);
  glDisable(0xbe2);
  glUseProgram(*(undefined4 *)(this + 4));
  glActiveTexture(0x84c0);
  FBOContainer::Activate();
  iVar3 = Engine::IsPostEffectActivated(param_2);
  if (iVar3 == 0) {
    glBindFramebuffer(0x8d40,*(undefined4 *)(param_2 + 0x40c));
    if (*(int *)(**(int **)(param_2 + 0x30) + 0x30) == 2) {
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
    Engine::ActivateRender2FracFBO();
  }
  glEnableVertexAttribArray(*(undefined4 *)(this + 0x48));
  glEnableVertexAttribArray(*(undefined4 *)(this + 0x50));
  glUniformMatrix4fv(*(undefined4 *)(this + 0x4c),1,0,param_2 + 0x104);
  glVertexAttribPointer
            (*(undefined4 *)(this + 0x48),3,0x1406,0,0,
             *(undefined4 *)(*(int *)(param_2 + 0x380) + 4));
  glVertexAttribPointer
            (*(undefined4 *)(this + 0x50),2,0x1406,0,0,
             *(undefined4 *)(*(int *)(param_2 + 0x380) + 8));
  pcVar6 = *(code **)(DAT_00098784 + 0x986f2);
  (*pcVar6)(0x4000);
  (*pcVar6)(0x100);
  iVar3 = Engine::GetDisplayWidth();
  Engine::GetDisplayHeight();
  Engine::DrawQuad((int)param_2,0,0,iVar3);
  glDisableVertexAttribArray(*(undefined4 *)(this + 0x48));
  glDisableVertexAttribArray(*(undefined4 *)(this + 0x50));
  glEnable(0xb71);
  (*pcVar6)(0x100);
  iVar3 = Engine::IsPostEffectActivated(param_2);
  if (iVar3 != 0) {
    Engine::DeactivateRender2FracFBO();
  }
  glActiveTexture(0x84c0);
  if (*piVar7 != local_3c) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

```

## target disasm
```
  00098560: push {r4,r5,r6,r7,lr}
  00098562: add r7,sp,#0xc
  00098564: push {r8,r9,r11}
  00098568: vpush {d8,d9,d10}
  0009856c: sub sp,#0x48
  0009856e: mov r9,r0
  00098570: ldr r0,[0x00098780]
  00098572: vmov.i32 q4,#0x0
  00098576: mov r4,r2
  00098578: add r0,pc
  0009857a: mov r6,r1
  0009857c: ldr.w r8,[r0,#0x0]
  00098580: ldr.w r0,[r8,#0x0]
  00098584: str r0,[sp,#0x44]
  00098586: add.w r0,r2,#0x3b4
  0009858a: vst1.32 {d8,d9},[r0]
  0009858e: add.w r0,r2,#0x3a4
  00098592: vst1.32 {d8,d9},[r0]
  00098596: add.w r0,r2,#0x394
  0009859a: vst1.32 {d8,d9},[r0]
  0009859e: add.w r0,r2,#0x384
  000985a2: vst1.32 {d8,d9},[r0]
  000985a6: mov r0,r2
  000985a8: blx 0x0006e8cc
  000985ac: vmov s0,r0
  000985b0: mov r0,r4
  000985b2: vmov.f32 s20,0x40000000
  000985b6: vcvt.f32.s32 s0,s0
  000985ba: vdiv.f32 s0,s20,s0
  000985be: vstr.32 s0,[r4,#0x384]
  000985c2: blx 0x0006e8d8
  000985c6: vmov s0,r0
  000985ca: adr r1,[0x98768]
  000985cc: vld1.64 {d16,d17},[r1]
  000985d0: mov.w r2,#0x3f800000
  000985d4: vcvt.f32.s32 s0,s0
  000985d8: ldr r1,[0x0009877c]
  000985da: str.w r1,[r4,#0x3ac]
  000985de: add r1,sp,#0x8
  000985e0: ldr r0,[0x00098778]
  000985e2: str.w r2,[r4,#0x3c0]
  000985e6: strd r0,r2,[r4,#0x3b4]
  000985ea: orr r0,r1,#0x4
  000985ee: vdiv.f32 s0,s20,s0
  000985f2: vneg.f32 s0,s0
  000985f6: vstr.32 s0,[r4,#0x398]
  000985fa: vst1.32 {d8,d9},[r0]
  000985fe: add.w r0,r1,#0x18
  00098602: vst1.64 {d8,d9},[r0]
  00098606: add.w r0,r1,#0x28
  0009860a: vst1.64 {d16,d17},[r0]
  0009860e: mov r0,r4
  00098610: str r2,[sp,#0x8]
  00098612: str r2,[sp,#0x1c]
  00098614: str r2,[sp,#0x40]
  00098616: blx 0x0006fae4
  0009861a: movw r0,#0xb71
  0009861e: blx 0x0006edc4
  00098622: movs r0,#0x0
  00098624: blx 0x0006eec0
  00098628: movw r0,#0xbe2
  0009862c: blx 0x0006edc4
  00098630: ldr.w r0,[r9,#0x4]
  00098634: blx 0x00070960
  00098638: movw r0,#0x84c0
  0009863c: blx 0x0006fb8c
  00098640: mov r0,r6
  00098642: blx 0x0006ee48
  00098646: mov r0,r4
  00098648: blx 0x0006ee78
  0009864c: cbz r0,0x00098656
  0009864e: mov r0,r4
  00098650: blx 0x0006feec
  00098654: b 0x00098696
  00098656: ldr.w r1,[r4,#0x40c]
  0009865a: movw r0,#0x8d40
  0009865e: blx 0x0006ee6c
  00098662: ldr r0,[r4,#0x30]
  00098664: ldr r0,[r0,#0x0]
  00098666: ldr r0,[r0,#0x30]
  00098668: cmp r0,#0x2
  0009866a: bne 0x0009867c
  0009866c: mov r0,r4
  0009866e: blx 0x0006e8cc
  00098672: mov r6,r0
  00098674: mov r0,r4
  00098676: blx 0x0006e8d8
  0009867a: b 0x0009868a
  0009867c: mov r0,r4
  0009867e: blx 0x0006e8d8
  00098682: mov r6,r0
  00098684: mov r0,r4
  00098686: blx 0x0006e8cc
  0009868a: mov r3,r0
  0009868c: movs r0,#0x0
  0009868e: movs r1,#0x0
  00098690: mov r2,r6
  00098692: blx 0x0006eda0
  00098696: ldr.w r0,[r9,#0x48]
  0009869a: blx 0x00070a5c
  0009869e: ldr.w r0,[r9,#0x50]
  000986a2: blx 0x00070a5c
  000986a6: ldr.w r0,[r9,#0x4c]
  000986aa: add.w r3,r4,#0x104
  000986ae: movs r1,#0x1
  000986b0: movs r2,#0x0
  000986b2: movs r5,#0x0
  000986b4: blx 0x00070990
  000986b8: ldr.w r1,[r4,#0x380]
  000986bc: movw r2,#0x1406
  000986c0: ldr.w r0,[r9,#0x48]
  000986c4: movs r3,#0x0
  000986c6: ldr r1,[r1,#0x4]
  000986c8: strd r5,r1,[sp,#0x0]
  000986cc: movs r1,#0x3
  000986ce: blx 0x000709c0
  000986d2: ldr.w r1,[r4,#0x380]
  000986d6: movw r2,#0x1406
  000986da: ldr.w r0,[r9,#0x50]
  000986de: movs r3,#0x0
  000986e0: ldr r1,[r1,#0x8]
  000986e2: strd r5,r1,[sp,#0x0]
  000986e6: movs r1,#0x2
  000986e8: blx 0x000709c0
  000986ec: ldr r0,[0x00098784]
  000986ee: add r0,pc
  000986f0: ldr r5,[r0,#0x0]
  000986f2: mov.w r0,#0x4000
  000986f6: blx r5
  000986f8: mov.w r0,#0x100
  000986fc: blx r5
  000986fe: mov r0,r4
  00098700: blx 0x0006e8cc
  00098704: mov r6,r0
  00098706: mov r0,r4
  00098708: blx 0x0006e8d8
  0009870c: str r0,[sp,#0x0]
  0009870e: mov r0,r4
  00098710: movs r1,#0x0
  00098712: movs r2,#0x0
  00098714: mov r3,r6
  00098716: blx 0x0006e8a8
  0009871a: ldr.w r0,[r9,#0x48]
  0009871e: blx 0x00070a44
  00098722: ldr.w r0,[r9,#0x50]
  00098726: blx 0x00070a44
  0009872a: movw r0,#0xb71
  0009872e: blx 0x0006edb8
  00098732: mov.w r0,#0x100
  00098736: blx r5
  00098738: mov r0,r4
  0009873a: blx 0x0006ee78
  0009873e: cbz r0,0x00098746
  00098740: mov r0,r4
  00098742: blx 0x0006eea8
  00098746: movw r0,#0x84c0
  0009874a: blx 0x0006fb8c
  0009874e: ldr r0,[sp,#0x44]
  00098750: ldr.w r1,[r8,#0x0]
  00098754: subs r0,r1,r0
  00098756: itttt eq
  00098758: add.eq sp,#0x48
  0009875a: vpop.eq {d8,d9,d10}
  0009875e: pop.eq.w {r8,r9,r11}
  00098762: pop.eq {r4,r5,r6,r7,pc}
  00098764: blx 0x0006e824
```
