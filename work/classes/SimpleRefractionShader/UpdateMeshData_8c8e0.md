# SimpleRefractionShader::UpdateMeshData
elf 0x8c8e0 body 528
Sig: undefined __thiscall UpdateMeshData(SimpleRefractionShader * this, Mesh * param_1, Engine * param_2)

## decompile
```c

/* AbyssEngine::SimpleRefractionShader::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) */

void __thiscall
AbyssEngine::SimpleRefractionShader::UpdateMeshData
          (SimpleRefractionShader *this,Mesh *param_1,Engine *param_2)

{
  undefined4 uVar1;
  undefined4 uVar2;
  code *pcVar3;
  code *pcVar4;
  uint in_fpscr;
  float fVar5;
  float fVar6;
  
  if (-1 < *(int *)(this + 0x34)) {
    glUniformMatrix4fv(*(int *)(this + 0x34),1,0,param_2 + 0x104);
  }
  if (this[9] != (SimpleRefractionShader)0x0) {
    if (-1 < *(int *)(this + 0x40)) {
      glUniform1f(*(int *)(this + 0x40),0xc0000000);
    }
    if (-1 < *(int *)(this + 0x44)) {
      glUniform4fv(*(int *)(this + 0x44),1,param_2 + 0xd0);
    }
    if (-1 < *(int *)(this + 0x48)) {
      glUniform3f(*(int *)(this + 0x48),*(undefined4 *)(param_2 + 0x34c),
                  *(undefined4 *)(param_2 + 0x350),*(undefined4 *)(param_2 + 0x354));
    }
    uVar2 = *(undefined4 *)(this + 0x50);
    uVar1 = Engine::GetDisplayWidth();
    fVar6 = (float)VectorSignedToFloat(uVar1,(byte)(in_fpscr >> 0x16) & 3);
    uVar1 = Engine::GetDisplayHeight();
    fVar5 = (float)VectorSignedToFloat(uVar1,(byte)(in_fpscr >> 0x16) & 3);
    glUniform2f(uVar2,1.0 / fVar6,1.0 / fVar5);
    glActiveTexture(0x84c7);
    Engine::ActivateRefractFBO();
    glUniform1f(*(undefined4 *)(this + 0x4c),*(undefined4 *)(param_1 + 0x20));
    this[9] = (SimpleRefractionShader)0x0;
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
  if (-1 < *(int *)(this + 0x2c)) {
    glEnableVertexAttribArray();
  }
  if (-1 < *(int *)(this + 0x30)) {
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
    if (-1 < *(int *)(this + 0x2c)) {
      glVertexAttribPointer(*(int *)(this + 0x2c),3,0x1406,0,0,*(undefined4 *)(param_1 + 0x14));
    }
    if (-1 < *(int *)(this + 0x30)) {
      glVertexAttribPointer(*(int *)(this + 0x30),3,0x1406,0,0,*(undefined4 *)(param_1 + 0x18));
    }
  }
  else {
    pcVar4 = *(code **)(DAT_0009caf0 + 0x9c9d0);
    (*pcVar4)(0x8892,*(undefined4 *)(param_1 + 0x60));
    pcVar3 = *(code **)(DAT_0009caf4 + 0x9c9e6);
    (*pcVar3)(*(undefined4 *)(this + 0x20),3,0x1406,0,0,0);
    (*pcVar4)(0x8892,*(undefined4 *)(param_1 + 0x68));
    (*pcVar3)(*(undefined4 *)(this + 0x24),2,0x1406,0,0,0);
    (*pcVar4)(0x8892,*(undefined4 *)(param_1 + 0x6c));
    (*pcVar3)(*(undefined4 *)(this + 0x28),3,0x1406,0,0,0);
    (*pcVar4)(0x8892,*(undefined4 *)(param_1 + 0x70));
    (*pcVar3)(*(undefined4 *)(this + 0x2c),3,0x1406,0,0,0);
    (*pcVar4)(0x8892,*(undefined4 *)(param_1 + 0x74));
    (*pcVar3)(*(undefined4 *)(this + 0x30),3,0x1406,0,0,0);
  }
  return;
}

```

## target disasm
```
  0009c8e0: push {r4,r5,r6,r7,lr}
  0009c8e2: add r7,sp,#0xc
  0009c8e4: push {r8,r9,r11}
  0009c8e8: vpush {d7,d8,d9}
  0009c8ec: mov r4,r0
  0009c8ee: ldr r0,[r0,#0x34]
  0009c8f0: mov r6,r2
  0009c8f2: mov r8,r1
  0009c8f4: cmp r0,#0x0
  0009c8f6: blt 0x0009c904
  0009c8f8: add.w r3,r6,#0x104
  0009c8fc: movs r1,#0x1
  0009c8fe: movs r2,#0x0
  0009c900: blx 0x00070990
  0009c904: ldrb r0,[r4,#0x9]
  0009c906: cmp r0,#0x0
  0009c908: beq 0x0009c98c
  0009c90a: ldr r0,[r4,#0x40]
  0009c90c: cmp r0,#0x0
  0009c90e: blt 0x0009c918
  0009c910: mov.w r1,#0xc0000000
  0009c914: blx 0x00070978
  0009c918: ldr r0,[r4,#0x44]
  0009c91a: cmp r0,#0x0
  0009c91c: blt 0x0009c928
  0009c91e: add.w r2,r6,#0xd0
  0009c922: movs r1,#0x1
  0009c924: blx 0x00070984
  0009c928: ldr r0,[r4,#0x48]
  0009c92a: cmp r0,#0x0
  0009c92c: blt 0x0009c938
  0009c92e: add.w r3,r6,#0x34c
  0009c932: ldmia r3,{r1,r2,r3}
  0009c934: blx 0x000709b4
  0009c938: mov r0,r6
  0009c93a: ldr r5,[r4,#0x50]
  0009c93c: blx 0x0006e8cc
  0009c940: vmov s0,r0
  0009c944: mov r0,r6
  0009c946: vmov.f32 s16,0x3f800000
  0009c94a: vcvt.f32.s32 s18,s0
  0009c94e: blx 0x0006e8d8
  0009c952: vmov s0,r0
  0009c956: mov r0,r5
  0009c958: vdiv.f32 s2,s16,s18
  0009c95c: vcvt.f32.s32 s0,s0
  0009c960: vmov r1,s2
  0009c964: vdiv.f32 s0,s16,s0
  0009c968: vmov r2,s0
  0009c96c: blx 0x00070c48
  0009c970: movw r0,#0x84c7
  0009c974: blx 0x0006fb8c
  0009c978: mov r0,r6
  0009c97a: blx 0x00070c3c
  0009c97e: ldr.w r1,[r8,#0x20]
  0009c982: ldr r0,[r4,#0x4c]
  0009c984: blx 0x00070978
  0009c988: movs r0,#0x0
  0009c98a: strb r0,[r4,#0x9]
  0009c98c: ldr r0,[r4,#0x20]
  0009c98e: cmp r0,#0x0
  0009c990: blt 0x0009c996
  0009c992: blx 0x00070a5c
  0009c996: ldr r0,[r4,#0x24]
  0009c998: cmp r0,#0x0
  0009c99a: blt 0x0009c9a0
  0009c99c: blx 0x00070a5c
  0009c9a0: ldr r0,[r4,#0x28]
  0009c9a2: cmp r0,#0x0
  0009c9a4: blt 0x0009c9aa
  0009c9a6: blx 0x00070a5c
  0009c9aa: ldr r0,[r4,#0x2c]
  0009c9ac: cmp r0,#0x0
  0009c9ae: blt 0x0009c9b4
  0009c9b0: blx 0x00070a5c
  0009c9b4: ldr r0,[r4,#0x30]
  0009c9b6: cmp r0,#0x0
  0009c9b8: blt 0x0009c9be
  0009c9ba: blx 0x00070a5c
  0009c9be: ldrb.w r0,[r8,#0x5c]
  0009c9c2: cmp r0,#0x0
  0009c9c4: beq 0x0009ca5a
  0009c9c6: ldr r0,[0x0009caf0]
  0009c9c8: ldr.w r1,[r8,#0x60]
  0009c9cc: add r0,pc
  0009c9ce: ldr r6,[r0,#0x0]
  0009c9d0: movw r0,#0x8892
  0009c9d4: blx r6
  0009c9d6: ldr r1,[0x0009caf4]
  0009c9d8: mov.w r9,#0x0
  0009c9dc: ldr r0,[r4,#0x20]
  0009c9de: movw r2,#0x1406
  0009c9e2: add r1,pc
  0009c9e4: movs r3,#0x0
  0009c9e6: strd r9,r9,[sp,#0x0]
  0009c9ea: ldr r5,[r1,#0x0]
  0009c9ec: movs r1,#0x3
  0009c9ee: blx r5
  0009c9f0: ldr.w r1,[r8,#0x68]
  0009c9f4: movw r0,#0x8892
  0009c9f8: blx r6
  0009c9fa: ldr r0,[r4,#0x24]
  0009c9fc: movs r1,#0x2
  0009c9fe: movw r2,#0x1406
  0009ca02: movs r3,#0x0
  0009ca04: strd r9,r9,[sp,#0x0]
  0009ca08: blx r5
  0009ca0a: ldr.w r1,[r8,#0x6c]
  0009ca0e: movw r0,#0x8892
  0009ca12: blx r6
  0009ca14: ldr r0,[r4,#0x28]
  0009ca16: movs r1,#0x3
  0009ca18: movw r2,#0x1406
  0009ca1c: movs r3,#0x0
  0009ca1e: strd r9,r9,[sp,#0x0]
  0009ca22: blx r5
  0009ca24: ldr.w r1,[r8,#0x70]
  0009ca28: movw r0,#0x8892
  0009ca2c: blx r6
  0009ca2e: ldr r0,[r4,#0x2c]
  0009ca30: movs r1,#0x3
  0009ca32: movw r2,#0x1406
  0009ca36: movs r3,#0x0
  0009ca38: strd r9,r9,[sp,#0x0]
  0009ca3c: blx r5
  0009ca3e: ldr.w r1,[r8,#0x74]
  0009ca42: movw r0,#0x8892
  0009ca46: blx r6
  0009ca48: ldr r0,[r4,#0x30]
  0009ca4a: movs r1,#0x3
  0009ca4c: movw r2,#0x1406
  0009ca50: movs r3,#0x0
  0009ca52: strd r9,r9,[sp,#0x0]
  0009ca56: blx r5
  0009ca58: b 0x0009cae6
  0009ca5a: ldr r0,[r4,#0x20]
  0009ca5c: cmp r0,#0x0
  0009ca5e: blt 0x0009ca76
  0009ca60: ldr.w r1,[r8,#0x4]
  0009ca64: movs r2,#0x0
  0009ca66: movs r3,#0x0
  0009ca68: strd r2,r1,[sp,#0x0]
  0009ca6c: movs r1,#0x3
  0009ca6e: movw r2,#0x1406
  0009ca72: blx 0x000709c0
  0009ca76: ldr r0,[r4,#0x24]
  0009ca78: cmp r0,#0x0
  0009ca7a: blt 0x0009ca92
  0009ca7c: ldr.w r1,[r8,#0x8]
  0009ca80: movs r2,#0x0
  0009ca82: movs r3,#0x0
  0009ca84: strd r2,r1,[sp,#0x0]
  0009ca88: movs r1,#0x2
  0009ca8a: movw r2,#0x1406
  0009ca8e: blx 0x000709c0
  0009ca92: ldr r0,[r4,#0x28]
  0009ca94: cmp r0,#0x0
  0009ca96: blt 0x0009caae
  0009ca98: ldr.w r1,[r8,#0x10]
  0009ca9c: movs r2,#0x0
  0009ca9e: movs r3,#0x0
  0009caa0: strd r2,r1,[sp,#0x0]
  0009caa4: movs r1,#0x3
  0009caa6: movw r2,#0x1406
  0009caaa: blx 0x000709c0
  0009caae: ldr r0,[r4,#0x2c]
  0009cab0: cmp r0,#0x0
  0009cab2: blt 0x0009caca
  0009cab4: ldr.w r1,[r8,#0x14]
  0009cab8: movs r2,#0x0
  0009caba: movs r3,#0x0
  0009cabc: strd r2,r1,[sp,#0x0]
  0009cac0: movs r1,#0x3
  0009cac2: movw r2,#0x1406
  0009cac6: blx 0x000709c0
  0009caca: ldr r0,[r4,#0x30]
  0009cacc: cmp r0,#0x0
  0009cace: blt 0x0009cae6
  0009cad0: ldr.w r1,[r8,#0x18]
  0009cad4: movs r2,#0x0
  0009cad6: movs r3,#0x0
  0009cad8: strd r2,r1,[sp,#0x0]
  0009cadc: movs r1,#0x3
  0009cade: movw r2,#0x1406
  0009cae2: blx 0x000709c0
  0009cae6: vpop {d7,d8,d9}
  0009caea: pop.w {r8,r9,r11}
  0009caee: pop {r4,r5,r6,r7,pc}
```
