# BumpShaderRefract::UpdateMeshData
elf 0x8f93a body 392
Sig: undefined __thiscall UpdateMeshData(BumpShaderRefract * this, Mesh * param_1, Engine * param_2)

## decompile
```c

/* AbyssEngine::BumpShaderRefract::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) */

void __thiscall
AbyssEngine::BumpShaderRefract::UpdateMeshData
          (BumpShaderRefract *this,Mesh *param_1,Engine *param_2)

{
  undefined4 uVar1;
  int iVar2;
  uint in_fpscr;
  float fVar3;
  float fVar4;
  
  if (-1 < *(int *)(this + 0x2c)) {
    glUniformMatrix4fv(*(int *)(this + 0x2c),1,0,param_2 + 0x104);
  }
  if (-1 < *(int *)(this + 0x40)) {
    glUniformMatrix4fv(*(int *)(this + 0x40),1,0,param_2 + 0x1c4);
  }
  if (this[9] != (BumpShaderRefract)0x0) {
    if (-1 < *(int *)(this + 0x44)) {
      glUniform1i(*(int *)(this + 0x44),param_1[0x85]);
    }
    glUniform4fv(*(undefined4 *)(this + 0x48),1,param_2 + 0xd0);
    iVar2 = *(int *)(this + 0x38);
    if (-1 < iVar2) {
      uVar1 = Engine::GetDisplayWidth();
      fVar4 = (float)VectorSignedToFloat(uVar1,(byte)(in_fpscr >> 0x16) & 3);
      uVar1 = Engine::GetDisplayHeight();
      fVar3 = (float)VectorSignedToFloat(uVar1,(byte)(in_fpscr >> 0x16) & 3);
      glUniform2f(iVar2,1.0 / fVar4,1.0 / fVar3);
    }
    glActiveTexture(0x84c7);
    Engine::ActivateRefractFBO();
    this[9] = (BumpShaderRefract)0x0;
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
  iVar2 = *(int *)(this + 0x20);
  if (param_1[0x5c] == (Mesh)0x0) {
    if (-1 < iVar2) {
      glVertexAttribPointer(iVar2,3,0x1406,0,0,*(undefined4 *)(param_1 + 4));
    }
    if (-1 < *(int *)(this + 0x24)) {
      glVertexAttribPointer(*(int *)(this + 0x24),2,0x1406,0,0,*(undefined4 *)(param_1 + 8));
    }
    iVar2 = *(int *)(this + 0x28);
    if (iVar2 < 0) {
      return;
    }
    uVar1 = *(undefined4 *)(param_1 + 0xc);
  }
  else {
    if (-1 < iVar2) {
      glBindBuffer(0x8892,*(undefined4 *)(param_1 + 0x60));
      glVertexAttribPointer(*(undefined4 *)(this + 0x20),3,0x1406,0,0,0);
    }
    if (-1 < *(int *)(this + 0x24)) {
      glBindBuffer(0x8892,*(undefined4 *)(param_1 + 0x68));
      glVertexAttribPointer(*(undefined4 *)(this + 0x24),2,0x1406,0,0,0);
    }
    if (*(int *)(this + 0x28) < 0) {
      return;
    }
    glBindBuffer(0x8892,*(undefined4 *)(param_1 + 0x78));
    uVar1 = 0;
    iVar2 = *(int *)(this + 0x28);
  }
  glVertexAttribPointer(iVar2,4,0x1406,0,0,uVar1);
  return;
}

```

## target disasm
```
  0009f93a: push {r4,r5,r6,r7,lr}
  0009f93c: add r7,sp,#0xc
  0009f93e: push.w r8
  0009f942: vpush {d7,d8,d9}
  0009f946: mov r4,r0
  0009f948: ldr r0,[r0,#0x2c]
  0009f94a: mov r6,r2
  0009f94c: mov r8,r1
  0009f94e: cmp r0,#0x0
  0009f950: blt 0x0009f95e
  0009f952: add.w r3,r6,#0x104
  0009f956: movs r1,#0x1
  0009f958: movs r2,#0x0
  0009f95a: blx 0x00070990
  0009f95e: ldr r0,[r4,#0x40]
  0009f960: cmp r0,#0x0
  0009f962: blt 0x0009f970
  0009f964: add.w r3,r6,#0x1c4
  0009f968: movs r1,#0x1
  0009f96a: movs r2,#0x0
  0009f96c: blx 0x00070990
  0009f970: ldrb r0,[r4,#0x9]
  0009f972: cbz r0,0x0009f9dc
  0009f974: ldr r0,[r4,#0x44]
  0009f976: cmp r0,#0x0
  0009f978: blt 0x0009f982
  0009f97a: ldrb.w r1,[r8,#0x85]
  0009f97e: blx 0x0007096c
  0009f982: ldr r0,[r4,#0x48]
  0009f984: add.w r2,r6,#0xd0
  0009f988: movs r1,#0x1
  0009f98a: blx 0x00070984
  0009f98e: ldr r5,[r4,#0x38]
  0009f990: cmp r5,#0x0
  0009f992: blt 0x0009f9ca
  0009f994: mov r0,r6
  0009f996: blx 0x0006e8cc
  0009f99a: vmov s0,r0
  0009f99e: mov r0,r6
  0009f9a0: vmov.f32 s16,0x3f800000
  0009f9a4: vcvt.f32.s32 s18,s0
  0009f9a8: blx 0x0006e8d8
  0009f9ac: vmov s0,r0
  0009f9b0: mov r0,r5
  0009f9b2: vdiv.f32 s2,s16,s18
  0009f9b6: vcvt.f32.s32 s0,s0
  0009f9ba: vmov r1,s2
  0009f9be: vdiv.f32 s0,s16,s0
  0009f9c2: vmov r2,s0
  0009f9c6: blx 0x00070c48
  0009f9ca: movw r0,#0x84c7
  0009f9ce: blx 0x0006fb8c
  0009f9d2: mov r0,r6
  0009f9d4: blx 0x00070c3c
  0009f9d8: movs r0,#0x0
  0009f9da: strb r0,[r4,#0x9]
  0009f9dc: ldr r0,[r4,#0x20]
  0009f9de: cmp r0,#0x0
  0009f9e0: blt 0x0009f9e6
  0009f9e2: blx 0x00070a5c
  0009f9e6: ldr r0,[r4,#0x24]
  0009f9e8: cmp r0,#0x0
  0009f9ea: blt 0x0009f9f0
  0009f9ec: blx 0x00070a5c
  0009f9f0: ldr r0,[r4,#0x28]
  0009f9f2: cmp r0,#0x0
  0009f9f4: blt 0x0009f9fa
  0009f9f6: blx 0x00070a5c
  0009f9fa: ldrb.w r1,[r8,#0x5c]
  0009f9fe: ldr r0,[r4,#0x20]
  0009fa00: cbz r1,0x0009fa66
  0009fa02: cmp r0,#0x0
  0009fa04: blt 0x0009fa26
  0009fa06: ldr.w r1,[r8,#0x60]
  0009fa0a: movw r0,#0x8892
  0009fa0e: blx 0x0006ed04
  0009fa12: ldr r0,[r4,#0x20]
  0009fa14: movs r1,#0x0
  0009fa16: strd r1,r1,[sp,#0x0]
  0009fa1a: movs r1,#0x3
  0009fa1c: movw r2,#0x1406
  0009fa20: movs r3,#0x0
  0009fa22: blx 0x000709c0
  0009fa26: ldr r0,[r4,#0x24]
  0009fa28: cmp r0,#0x0
  0009fa2a: blt 0x0009fa4c
  0009fa2c: ldr.w r1,[r8,#0x68]
  0009fa30: movw r0,#0x8892
  0009fa34: blx 0x0006ed04
  0009fa38: ldr r0,[r4,#0x24]
  0009fa3a: movs r1,#0x0
  0009fa3c: strd r1,r1,[sp,#0x0]
  0009fa40: movs r1,#0x2
  0009fa42: movw r2,#0x1406
  0009fa46: movs r3,#0x0
  0009fa48: blx 0x000709c0
  0009fa4c: ldr r0,[r4,#0x28]
  0009fa4e: cmp r0,#0x0
  0009fa50: blt 0x0009fab8
  0009fa52: ldr.w r1,[r8,#0x78]
  0009fa56: movw r0,#0x8892
  0009fa5a: blx 0x0006ed04
  0009fa5e: movs r1,#0x0
  0009fa60: ldr r0,[r4,#0x28]
  0009fa62: str r1,[sp,#0x0]
  0009fa64: b 0x0009faaa
  0009fa66: cmp r0,#0x0
  0009fa68: blt 0x0009fa80
  0009fa6a: ldr.w r1,[r8,#0x4]
  0009fa6e: movs r2,#0x0
  0009fa70: movs r3,#0x0
  0009fa72: strd r2,r1,[sp,#0x0]
  0009fa76: movs r1,#0x3
  0009fa78: movw r2,#0x1406
  0009fa7c: blx 0x000709c0
  0009fa80: ldr r0,[r4,#0x24]
  0009fa82: cmp r0,#0x0
  0009fa84: blt 0x0009fa9c
  0009fa86: ldr.w r1,[r8,#0x8]
  0009fa8a: movs r2,#0x0
  0009fa8c: movs r3,#0x0
  0009fa8e: strd r2,r1,[sp,#0x0]
  0009fa92: movs r1,#0x2
  0009fa94: movw r2,#0x1406
  0009fa98: blx 0x000709c0
  0009fa9c: ldr r0,[r4,#0x28]
  0009fa9e: cmp r0,#0x0
  0009faa0: blt 0x0009fab8
  0009faa2: ldr.w r1,[r8,#0xc]
  0009faa6: movs r2,#0x0
  0009faa8: str r2,[sp,#0x0]
  0009faaa: str r1,[sp,#0x4]
  0009faac: movs r1,#0x4
  0009faae: movw r2,#0x1406
  0009fab2: movs r3,#0x0
  0009fab4: blx 0x000709c0
  0009fab8: vpop {d7,d8,d9}
  0009fabc: pop.w r8
  0009fac0: pop {r4,r5,r6,r7,pc}
```
