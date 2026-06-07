# BumpShaderV2::UpdateMeshData
elf 0x8d8c8 body 506
Sig: undefined __thiscall UpdateMeshData(BumpShaderV2 * this, Mesh * param_1, Engine * param_2)

## decompile
```c

/* AbyssEngine::BumpShaderV2::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) */

void __thiscall
AbyssEngine::BumpShaderV2::UpdateMeshData(BumpShaderV2 *this,Mesh *param_1,Engine *param_2)

{
  code *pcVar1;
  code *pcVar2;
  
  if (-1 < *(int *)(this + 0x38)) {
    glUniformMatrix4fv(*(int *)(this + 0x38),1,0,param_2 + 0x104);
  }
  if (-1 < *(int *)(this + 0x3c)) {
    glUniformMatrix3fv(*(int *)(this + 0x3c),1,0,param_2 + 0x204);
  }
  if (this[9] != (BumpShaderV2)0x0) {
    if (-1 < *(int *)(this + 0x40)) {
      glUniform3f(*(int *)(this + 0x40),*(undefined4 *)(param_2 + 0x330),
                  *(undefined4 *)(param_2 + 0x334),*(undefined4 *)(param_2 + 0x338));
    }
    if (-1 < *(int *)(this + 0x44)) {
      glUniform3f(*(int *)(this + 0x44),*(undefined4 *)(param_2 + 0x34c),
                  *(undefined4 *)(param_2 + 0x350),*(undefined4 *)(param_2 + 0x354));
    }
    if (-1 < *(int *)(this + 0x50)) {
      glUniform4fv(*(int *)(this + 0x50),1,param_2 + 0xd0);
    }
    if (-1 < *(int *)(this + 0x54)) {
      glUniform4fv(*(int *)(this + 0x54),1,param_2 + 0x2a8);
    }
    if (-1 < *(int *)(this + 0x58)) {
      glUniform4fv(*(int *)(this + 0x58),1,param_2 + 0x298);
    }
    if (-1 < *(int *)(this + 0x5c)) {
      glUniform4fv(*(int *)(this + 0x5c),1,param_2 + 0x2b8);
    }
    this[9] = (BumpShaderV2)0x0;
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
    pcVar1 = *(code **)(DAT_0009dac4 + 0x9d9a6);
    (*pcVar1)(0x8892,*(undefined4 *)(param_1 + 0x60));
    pcVar2 = *(code **)(DAT_0009dac8 + 0x9d9bc);
    (*pcVar2)(*(undefined4 *)(this + 0x20),3,0x1406,0,0,0);
    (*pcVar1)(0x8892,*(undefined4 *)(param_1 + 0x68));
    (*pcVar2)(*(undefined4 *)(this + 0x24),2,0x1406,0,0,0);
    (*pcVar1)(0x8892,*(undefined4 *)(param_1 + 0x6c));
    (*pcVar2)(*(undefined4 *)(this + 0x28),3,0x1406,0,0,0);
    (*pcVar1)(0x8892,*(undefined4 *)(param_1 + 0x70));
    (*pcVar2)(*(undefined4 *)(this + 0x2c),3,0x1406,0,0,0);
    (*pcVar1)(0x8892,*(undefined4 *)(param_1 + 0x74));
    (*pcVar2)(*(undefined4 *)(this + 0x30),3,0x1406,0,0,0);
  }
  return;
}

```

## target disasm
```
  0009d8c8: push {r4,r5,r6,r7,lr}
  0009d8ca: add r7,sp,#0xc
  0009d8cc: push {r6,r7,r8,r9,r11}
  0009d8d0: mov r4,r0
  0009d8d2: ldr r0,[r0,#0x38]
  0009d8d4: mov r6,r2
  0009d8d6: mov r8,r1
  0009d8d8: cmp r0,#0x0
  0009d8da: blt 0x0009d8e8
  0009d8dc: add.w r3,r6,#0x104
  0009d8e0: movs r1,#0x1
  0009d8e2: movs r2,#0x0
  0009d8e4: blx 0x00070990
  0009d8e8: ldr r0,[r4,#0x3c]
  0009d8ea: cmp r0,#0x0
  0009d8ec: blt 0x0009d8fa
  0009d8ee: add.w r3,r6,#0x204
  0009d8f2: movs r1,#0x1
  0009d8f4: movs r2,#0x0
  0009d8f6: blx 0x0007099c
  0009d8fa: ldrb r0,[r4,#0x9]
  0009d8fc: cbz r0,0x0009d962
  0009d8fe: ldr r0,[r4,#0x40]
  0009d900: cmp r0,#0x0
  0009d902: blt 0x0009d90e
  0009d904: add.w r3,r6,#0x330
  0009d908: ldmia r3,{r1,r2,r3}
  0009d90a: blx 0x000709b4
  0009d90e: ldr r0,[r4,#0x44]
  0009d910: cmp r0,#0x0
  0009d912: blt 0x0009d91e
  0009d914: add.w r3,r6,#0x34c
  0009d918: ldmia r3,{r1,r2,r3}
  0009d91a: blx 0x000709b4
  0009d91e: ldr r0,[r4,#0x50]
  0009d920: cmp r0,#0x0
  0009d922: blt 0x0009d92e
  0009d924: add.w r2,r6,#0xd0
  0009d928: movs r1,#0x1
  0009d92a: blx 0x00070984
  0009d92e: ldr r0,[r4,#0x54]
  0009d930: cmp r0,#0x0
  0009d932: blt 0x0009d93e
  0009d934: add.w r2,r6,#0x2a8
  0009d938: movs r1,#0x1
  0009d93a: blx 0x00070984
  0009d93e: ldr r0,[r4,#0x58]
  0009d940: cmp r0,#0x0
  0009d942: blt 0x0009d94e
  0009d944: add.w r2,r6,#0x298
  0009d948: movs r1,#0x1
  0009d94a: blx 0x00070984
  0009d94e: ldr r0,[r4,#0x5c]
  0009d950: cmp r0,#0x0
  0009d952: blt 0x0009d95e
  0009d954: add.w r2,r6,#0x2b8
  0009d958: movs r1,#0x1
  0009d95a: blx 0x00070984
  0009d95e: movs r0,#0x0
  0009d960: strb r0,[r4,#0x9]
  0009d962: ldr r0,[r4,#0x20]
  0009d964: cmp r0,#0x0
  0009d966: blt 0x0009d96c
  0009d968: blx 0x00070a5c
  0009d96c: ldr r0,[r4,#0x24]
  0009d96e: cmp r0,#0x0
  0009d970: blt 0x0009d976
  0009d972: blx 0x00070a5c
  0009d976: ldr r0,[r4,#0x28]
  0009d978: cmp r0,#0x0
  0009d97a: blt 0x0009d980
  0009d97c: blx 0x00070a5c
  0009d980: ldr r0,[r4,#0x2c]
  0009d982: cmp r0,#0x0
  0009d984: blt 0x0009d98a
  0009d986: blx 0x00070a5c
  0009d98a: ldr r0,[r4,#0x30]
  0009d98c: cmp r0,#0x0
  0009d98e: blt 0x0009d994
  0009d990: blx 0x00070a5c
  0009d994: ldrb.w r0,[r8,#0x5c]
  0009d998: cmp r0,#0x0
  0009d99a: beq 0x0009da30
  0009d99c: ldr r0,[0x0009dac4]
  0009d99e: ldr.w r1,[r8,#0x60]
  0009d9a2: add r0,pc
  0009d9a4: ldr r5,[r0,#0x0]
  0009d9a6: movw r0,#0x8892
  0009d9aa: blx r5
  0009d9ac: ldr r1,[0x0009dac8]
  0009d9ae: mov.w r9,#0x0
  0009d9b2: ldr r0,[r4,#0x20]
  0009d9b4: movw r2,#0x1406
  0009d9b8: add r1,pc
  0009d9ba: movs r3,#0x0
  0009d9bc: strd r9,r9,[sp,#0x0]
  0009d9c0: ldr r6,[r1,#0x0]
  0009d9c2: movs r1,#0x3
  0009d9c4: blx r6
  0009d9c6: ldr.w r1,[r8,#0x68]
  0009d9ca: movw r0,#0x8892
  0009d9ce: blx r5
  0009d9d0: ldr r0,[r4,#0x24]
  0009d9d2: movs r1,#0x2
  0009d9d4: movw r2,#0x1406
  0009d9d8: movs r3,#0x0
  0009d9da: strd r9,r9,[sp,#0x0]
  0009d9de: blx r6
  0009d9e0: ldr.w r1,[r8,#0x6c]
  0009d9e4: movw r0,#0x8892
  0009d9e8: blx r5
  0009d9ea: ldr r0,[r4,#0x28]
  0009d9ec: movs r1,#0x3
  0009d9ee: movw r2,#0x1406
  0009d9f2: movs r3,#0x0
  0009d9f4: strd r9,r9,[sp,#0x0]
  0009d9f8: blx r6
  0009d9fa: ldr.w r1,[r8,#0x70]
  0009d9fe: movw r0,#0x8892
  0009da02: blx r5
  0009da04: ldr r0,[r4,#0x2c]
  0009da06: movs r1,#0x3
  0009da08: movw r2,#0x1406
  0009da0c: movs r3,#0x0
  0009da0e: strd r9,r9,[sp,#0x0]
  0009da12: blx r6
  0009da14: ldr.w r1,[r8,#0x74]
  0009da18: movw r0,#0x8892
  0009da1c: blx r5
  0009da1e: ldr r0,[r4,#0x30]
  0009da20: movs r1,#0x3
  0009da22: movw r2,#0x1406
  0009da26: movs r3,#0x0
  0009da28: strd r9,r9,[sp,#0x0]
  0009da2c: blx r6
  0009da2e: b 0x0009dabc
  0009da30: ldr r0,[r4,#0x20]
  0009da32: cmp r0,#0x0
  0009da34: blt 0x0009da4c
  0009da36: ldr.w r1,[r8,#0x4]
  0009da3a: movs r2,#0x0
  0009da3c: movs r3,#0x0
  0009da3e: strd r2,r1,[sp,#0x0]
  0009da42: movs r1,#0x3
  0009da44: movw r2,#0x1406
  0009da48: blx 0x000709c0
  0009da4c: ldr r0,[r4,#0x24]
  0009da4e: cmp r0,#0x0
  0009da50: blt 0x0009da68
  0009da52: ldr.w r1,[r8,#0x8]
  0009da56: movs r2,#0x0
  0009da58: movs r3,#0x0
  0009da5a: strd r2,r1,[sp,#0x0]
  0009da5e: movs r1,#0x2
  0009da60: movw r2,#0x1406
  0009da64: blx 0x000709c0
  0009da68: ldr r0,[r4,#0x28]
  0009da6a: cmp r0,#0x0
  0009da6c: blt 0x0009da84
  0009da6e: ldr.w r1,[r8,#0x10]
  0009da72: movs r2,#0x0
  0009da74: movs r3,#0x0
  0009da76: strd r2,r1,[sp,#0x0]
  0009da7a: movs r1,#0x3
  0009da7c: movw r2,#0x1406
  0009da80: blx 0x000709c0
  0009da84: ldr r0,[r4,#0x2c]
  0009da86: cmp r0,#0x0
  0009da88: blt 0x0009daa0
  0009da8a: ldr.w r1,[r8,#0x14]
  0009da8e: movs r2,#0x0
  0009da90: movs r3,#0x0
  0009da92: strd r2,r1,[sp,#0x0]
  0009da96: movs r1,#0x3
  0009da98: movw r2,#0x1406
  0009da9c: blx 0x000709c0
  0009daa0: ldr r0,[r4,#0x30]
  0009daa2: cmp r0,#0x0
  0009daa4: blt 0x0009dabc
  0009daa6: ldr.w r1,[r8,#0x18]
  0009daaa: movs r2,#0x0
  0009daac: movs r3,#0x0
  0009daae: strd r2,r1,[sp,#0x0]
  0009dab2: movs r1,#0x3
  0009dab4: movw r2,#0x1406
  0009dab8: blx 0x000709c0
  0009dabc: pop.w {r2,r3,r8,r9,r11}
  0009dac0: pop {r4,r5,r6,r7,pc}
```
