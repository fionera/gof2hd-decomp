# EnergyShield::UpdateMeshData
elf 0x8a40c body 444
Sig: undefined __thiscall UpdateMeshData(EnergyShield * this, Mesh * param_1, Engine * param_2)

## decompile
```c

/* AbyssEngine::EnergyShield::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) */

void __thiscall
AbyssEngine::EnergyShield::UpdateMeshData(EnergyShield *this,Mesh *param_1,Engine *param_2)

{
  undefined4 uVar1;
  int iVar2;
  code *pcVar3;
  code *pcVar4;
  uint in_fpscr;
  float fVar5;
  float fVar6;
  
  if (-1 < *(int *)(this + 0x2c)) {
    glUniformMatrix4fv(*(int *)(this + 0x2c),1,0,param_2 + 0x104);
  }
  if (-1 < *(int *)(this + 0x30)) {
    glUniformMatrix4fv(*(int *)(this + 0x30),1,0,param_2 + 0x184);
  }
  if (this[9] != (EnergyShield)0x0) {
    if (-1 < *(int *)(this + 0x34)) {
      glUniform3f(*(int *)(this + 0x34),*(undefined4 *)(param_2 + 0x34c),
                  *(undefined4 *)(param_2 + 0x350),*(undefined4 *)(param_2 + 0x354));
    }
    if (-1 < *(int *)(this + 0x44)) {
      glUniform4fv(*(int *)(this + 0x44),1,param_2 + 0xd0);
    }
    if (-1 < *(int *)(this + 0x48)) {
      glUniform3fv(*(int *)(this + 0x48),1,param_2 + 800);
    }
    iVar2 = *(int *)(this + 0x58);
    if (-1 < iVar2) {
      uVar1 = Engine::GetDisplayWidth();
      fVar6 = (float)VectorSignedToFloat(uVar1,(byte)(in_fpscr >> 0x16) & 3);
      uVar1 = Engine::GetDisplayHeight();
      fVar5 = (float)VectorSignedToFloat(uVar1,(byte)(in_fpscr >> 0x16) & 3);
      glUniform2f(iVar2,1.0 / fVar6,1.0 / fVar5);
    }
    glActiveTexture(0x84c7);
    Engine::ActivateRefractFBO();
    glUniform1f(*(undefined4 *)(this + 0x50),*(undefined4 *)(param_1 + 0x24));
    glUniform1f(*(undefined4 *)(this + 0x54),*(float *)(param_1 + 0x24) / 3.0);
    this[9] = (EnergyShield)0x0;
  }
  if (-1 < *(int *)(this + 0x20)) {
    glEnableVertexAttribArray();
  }
  if (-1 < *(int *)(this + 0x24)) {
    glEnableVertexAttribArray();
  }
  if (-1 < *(int *)(this + 0x28)) {
    glEnableVertexAttribArray();
  }
  if (param_1[0x5c] == (Mesh)0x0) {
    if (-1 < *(int *)(this + 0x20)) {
      glVertexAttribPointer(*(int *)(this + 0x20),3,0x1406,0,0,*(undefined4 *)(param_1 + 4));
    }
    if (-1 < *(int *)(this + 0x24)) {
      glVertexAttribPointer(*(int *)(this + 0x24),2,0x1406,0,0,*(undefined4 *)(param_1 + 8));
    }
    if (-1 < *(int *)(this + 0x28)) {
      glVertexAttribPointer(*(int *)(this + 0x28),3,0x1406,0,0,*(undefined4 *)(param_1 + 0x10));
    }
  }
  else {
    pcVar4 = *(code **)(DAT_0009a5c8 + 0x9a514);
    (*pcVar4)(0x8892,*(undefined4 *)(param_1 + 0x60));
    pcVar3 = *(code **)(DAT_0009a5cc + 0x9a52a);
    (*pcVar3)(*(undefined4 *)(this + 0x20),3,0x1406,0,0,0);
    (*pcVar4)(0x8892,*(undefined4 *)(param_1 + 0x68));
    (*pcVar3)(*(undefined4 *)(this + 0x24),2,0x1406,0,0,0);
    (*pcVar4)(0x8892,*(undefined4 *)(param_1 + 0x6c));
    (*pcVar3)(*(undefined4 *)(this + 0x28),3,0x1406,0,0,0);
  }
  return;
}

```

## target disasm
```
  0009a40c: push {r4,r5,r6,r7,lr}
  0009a40e: add r7,sp,#0xc
  0009a410: push {r8,r9,r11}
  0009a414: vpush {d7,d8,d9}
  0009a418: mov r4,r0
  0009a41a: ldr r0,[r0,#0x2c]
  0009a41c: mov r6,r2
  0009a41e: mov r8,r1
  0009a420: cmp r0,#0x0
  0009a422: blt 0x0009a430
  0009a424: add.w r3,r6,#0x104
  0009a428: movs r1,#0x1
  0009a42a: movs r2,#0x0
  0009a42c: blx 0x00070990
  0009a430: ldr r0,[r4,#0x30]
  0009a432: cmp r0,#0x0
  0009a434: blt 0x0009a442
  0009a436: add.w r3,r6,#0x184
  0009a43a: movs r1,#0x1
  0009a43c: movs r2,#0x0
  0009a43e: blx 0x00070990
  0009a442: ldrb r0,[r4,#0x9]
  0009a444: cmp r0,#0x0
  0009a446: beq 0x0009a4e6
  0009a448: ldr r0,[r4,#0x34]
  0009a44a: cmp r0,#0x0
  0009a44c: blt 0x0009a458
  0009a44e: add.w r3,r6,#0x34c
  0009a452: ldmia r3,{r1,r2,r3}
  0009a454: blx 0x000709b4
  0009a458: ldr r0,[r4,#0x44]
  0009a45a: cmp r0,#0x0
  0009a45c: blt 0x0009a468
  0009a45e: add.w r2,r6,#0xd0
  0009a462: movs r1,#0x1
  0009a464: blx 0x00070984
  0009a468: ldr r0,[r4,#0x48]
  0009a46a: cmp r0,#0x0
  0009a46c: blt 0x0009a478
  0009a46e: add.w r2,r6,#0x320
  0009a472: movs r1,#0x1
  0009a474: blx 0x00070a50
  0009a478: ldr r5,[r4,#0x58]
  0009a47a: cmp r5,#0x0
  0009a47c: blt 0x0009a4b4
  0009a47e: mov r0,r6
  0009a480: blx 0x0006e8cc
  0009a484: vmov s0,r0
  0009a488: mov r0,r6
  0009a48a: vmov.f32 s16,0x3f800000
  0009a48e: vcvt.f32.s32 s18,s0
  0009a492: blx 0x0006e8d8
  0009a496: vmov s0,r0
  0009a49a: mov r0,r5
  0009a49c: vdiv.f32 s2,s16,s18
  0009a4a0: vcvt.f32.s32 s0,s0
  0009a4a4: vmov r1,s2
  0009a4a8: vdiv.f32 s0,s16,s0
  0009a4ac: vmov r2,s0
  0009a4b0: blx 0x00070c48
  0009a4b4: movw r0,#0x84c7
  0009a4b8: blx 0x0006fb8c
  0009a4bc: mov r0,r6
  0009a4be: blx 0x00070c3c
  0009a4c2: ldr.w r1,[r8,#0x24]
  0009a4c6: ldr r0,[r4,#0x50]
  0009a4c8: blx 0x00070978
  0009a4cc: vmov.f32 s0,0x40400000
  0009a4d0: vldr.32 s2,[r8,#0x24]
  0009a4d4: ldr r0,[r4,#0x54]
  0009a4d6: vdiv.f32 s0,s2,s0
  0009a4da: vmov r1,s0
  0009a4de: blx 0x00070978
  0009a4e2: movs r0,#0x0
  0009a4e4: strb r0,[r4,#0x9]
  0009a4e6: ldr r0,[r4,#0x20]
  0009a4e8: cmp r0,#0x0
  0009a4ea: blt 0x0009a4f0
  0009a4ec: blx 0x00070a5c
  0009a4f0: ldr r0,[r4,#0x24]
  0009a4f2: cmp r0,#0x0
  0009a4f4: blt 0x0009a4fa
  0009a4f6: blx 0x00070a5c
  0009a4fa: ldr r0,[r4,#0x28]
  0009a4fc: cmp r0,#0x0
  0009a4fe: blt 0x0009a504
  0009a500: blx 0x00070a5c
  0009a504: ldrb.w r0,[r8,#0x5c]
  0009a508: cbz r0,0x0009a56a
  0009a50a: ldr r0,[0x0009a5c8]
  0009a50c: ldr.w r1,[r8,#0x60]
  0009a510: add r0,pc
  0009a512: ldr r6,[r0,#0x0]
  0009a514: movw r0,#0x8892
  0009a518: blx r6
  0009a51a: ldr r1,[0x0009a5cc]
  0009a51c: mov.w r9,#0x0
  0009a520: ldr r0,[r4,#0x20]
  0009a522: movw r2,#0x1406
  0009a526: add r1,pc
  0009a528: movs r3,#0x0
  0009a52a: strd r9,r9,[sp,#0x0]
  0009a52e: ldr r5,[r1,#0x0]
  0009a530: movs r1,#0x3
  0009a532: blx r5
  0009a534: ldr.w r1,[r8,#0x68]
  0009a538: movw r0,#0x8892
  0009a53c: blx r6
  0009a53e: ldr r0,[r4,#0x24]
  0009a540: movs r1,#0x2
  0009a542: movw r2,#0x1406
  0009a546: movs r3,#0x0
  0009a548: strd r9,r9,[sp,#0x0]
  0009a54c: blx r5
  0009a54e: ldr.w r1,[r8,#0x6c]
  0009a552: movw r0,#0x8892
  0009a556: blx r6
  0009a558: ldr r0,[r4,#0x28]
  0009a55a: movs r1,#0x3
  0009a55c: movw r2,#0x1406
  0009a560: movs r3,#0x0
  0009a562: strd r9,r9,[sp,#0x0]
  0009a566: blx r5
  0009a568: b 0x0009a5be
  0009a56a: ldr r0,[r4,#0x20]
  0009a56c: cmp r0,#0x0
  0009a56e: blt 0x0009a586
  0009a570: ldr.w r1,[r8,#0x4]
  0009a574: movs r2,#0x0
  0009a576: movs r3,#0x0
  0009a578: strd r2,r1,[sp,#0x0]
  0009a57c: movs r1,#0x3
  0009a57e: movw r2,#0x1406
  0009a582: blx 0x000709c0
  0009a586: ldr r0,[r4,#0x24]
  0009a588: cmp r0,#0x0
  0009a58a: blt 0x0009a5a2
  0009a58c: ldr.w r1,[r8,#0x8]
  0009a590: movs r2,#0x0
  0009a592: movs r3,#0x0
  0009a594: strd r2,r1,[sp,#0x0]
  0009a598: movs r1,#0x2
  0009a59a: movw r2,#0x1406
  0009a59e: blx 0x000709c0
  0009a5a2: ldr r0,[r4,#0x28]
  0009a5a4: cmp r0,#0x0
  0009a5a6: blt 0x0009a5be
  0009a5a8: ldr.w r1,[r8,#0x10]
  0009a5ac: movs r2,#0x0
  0009a5ae: movs r3,#0x0
  0009a5b0: strd r2,r1,[sp,#0x0]
  0009a5b4: movs r1,#0x3
  0009a5b6: movw r2,#0x1406
  0009a5ba: blx 0x000709c0
  0009a5be: vpop {d7,d8,d9}
  0009a5c2: pop.w {r8,r9,r11}
  0009a5c6: pop {r4,r5,r6,r7,pc}
```
