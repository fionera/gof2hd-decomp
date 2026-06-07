# GenericShader::UpdateMeshData
elf 0x8e980 body 520
Sig: undefined __thiscall UpdateMeshData(GenericShader * this, Mesh * param_1, Engine * param_2)

## decompile
```c

/* AbyssEngine::GenericShader::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) */

void __thiscall
AbyssEngine::GenericShader::UpdateMeshData(GenericShader *this,Mesh *param_1,Engine *param_2)

{
  code *pcVar1;
  code *pcVar2;
  
  if (-1 < *(int *)(this + 0x34)) {
    glUniformMatrix4fv(*(int *)(this + 0x34),1,0,param_2 + 0x104);
  }
  if (-1 < *(int *)(this + 0x38)) {
    glUniformMatrix3fv(*(int *)(this + 0x38),1,0,param_2 + 0x204);
  }
  if (this[9] != (GenericShader)0x0) {
    if (-1 < *(int *)(this + 0x3c)) {
      glUniform3f(*(int *)(this + 0x3c),*(undefined4 *)(param_2 + 0x330),
                  *(undefined4 *)(param_2 + 0x334),*(undefined4 *)(param_2 + 0x338));
    }
    if (-1 < *(int *)(this + 0x40)) {
      glUniform3f(*(int *)(this + 0x40),*(undefined4 *)(param_2 + 0x34c),
                  *(undefined4 *)(param_2 + 0x350),*(undefined4 *)(param_2 + 0x354));
    }
    if (-1 < *(int *)(this + 0x50)) {
      glUniform4fv(*(int *)(this + 0x50),1,param_2 + 0xd0);
    }
    if (-1 < *(int *)(this + 0x54)) {
      glUniform3fv(*(int *)(this + 0x54),1,param_2 + 0x2cc);
    }
    if (-1 < *(int *)(this + 0x58)) {
      glUniform3fv(*(int *)(this + 0x58),1,param_2 + 0x2fc);
    }
    if (-1 < *(int *)(this + 0x5c)) {
      glUniform3fv(*(int *)(this + 0x5c),1,param_2 + 0x2e4);
    }
    if (-1 < *(int *)(this + 0x60)) {
      glUniform1f(*(int *)(this + 0x60),*(undefined4 *)(param_2 + 0x2c8));
    }
    this[9] = (GenericShader)0x0;
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
    pcVar1 = *(code **)(DAT_0009eb88 + 0x9ea6c);
    (*pcVar1)(0x8892,*(undefined4 *)(param_1 + 0x60));
    pcVar2 = *(code **)(DAT_0009eb8c + 0x9ea82);
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
  0009e980: push {r4,r5,r6,r7,lr}
  0009e982: add r7,sp,#0xc
  0009e984: push {r6,r7,r8,r9,r11}
  0009e988: mov r4,r0
  0009e98a: ldr r0,[r0,#0x34]
  0009e98c: mov r6,r2
  0009e98e: mov r8,r1
  0009e990: cmp r0,#0x0
  0009e992: blt 0x0009e9a0
  0009e994: add.w r3,r6,#0x104
  0009e998: movs r1,#0x1
  0009e99a: movs r2,#0x0
  0009e99c: blx 0x00070990
  0009e9a0: ldr r0,[r4,#0x38]
  0009e9a2: cmp r0,#0x0
  0009e9a4: blt 0x0009e9b2
  0009e9a6: add.w r3,r6,#0x204
  0009e9aa: movs r1,#0x1
  0009e9ac: movs r2,#0x0
  0009e9ae: blx 0x0007099c
  0009e9b2: ldrb r0,[r4,#0x9]
  0009e9b4: cbz r0,0x0009ea28
  0009e9b6: ldr r0,[r4,#0x3c]
  0009e9b8: cmp r0,#0x0
  0009e9ba: blt 0x0009e9c6
  0009e9bc: add.w r3,r6,#0x330
  0009e9c0: ldmia r3,{r1,r2,r3}
  0009e9c2: blx 0x000709b4
  0009e9c6: ldr r0,[r4,#0x40]
  0009e9c8: cmp r0,#0x0
  0009e9ca: blt 0x0009e9d6
  0009e9cc: add.w r3,r6,#0x34c
  0009e9d0: ldmia r3,{r1,r2,r3}
  0009e9d2: blx 0x000709b4
  0009e9d6: ldr r0,[r4,#0x50]
  0009e9d8: cmp r0,#0x0
  0009e9da: blt 0x0009e9e6
  0009e9dc: add.w r2,r6,#0xd0
  0009e9e0: movs r1,#0x1
  0009e9e2: blx 0x00070984
  0009e9e6: ldr r0,[r4,#0x54]
  0009e9e8: cmp r0,#0x0
  0009e9ea: blt 0x0009e9f6
  0009e9ec: add.w r2,r6,#0x2cc
  0009e9f0: movs r1,#0x1
  0009e9f2: blx 0x00070a50
  0009e9f6: ldr r0,[r4,#0x58]
  0009e9f8: cmp r0,#0x0
  0009e9fa: blt 0x0009ea06
  0009e9fc: add.w r2,r6,#0x2fc
  0009ea00: movs r1,#0x1
  0009ea02: blx 0x00070a50
  0009ea06: ldr r0,[r4,#0x5c]
  0009ea08: cmp r0,#0x0
  0009ea0a: blt 0x0009ea16
  0009ea0c: add.w r2,r6,#0x2e4
  0009ea10: movs r1,#0x1
  0009ea12: blx 0x00070a50
  0009ea16: ldr r0,[r4,#0x60]
  0009ea18: cmp r0,#0x0
  0009ea1a: blt 0x0009ea24
  0009ea1c: ldr.w r1,[r6,#0x2c8]
  0009ea20: blx 0x00070978
  0009ea24: movs r0,#0x0
  0009ea26: strb r0,[r4,#0x9]
  0009ea28: ldr r0,[r4,#0x20]
  0009ea2a: cmp r0,#0x0
  0009ea2c: blt 0x0009ea32
  0009ea2e: blx 0x00070a5c
  0009ea32: ldr r0,[r4,#0x24]
  0009ea34: cmp r0,#0x0
  0009ea36: blt 0x0009ea3c
  0009ea38: blx 0x00070a5c
  0009ea3c: ldr r0,[r4,#0x28]
  0009ea3e: cmp r0,#0x0
  0009ea40: blt 0x0009ea46
  0009ea42: blx 0x00070a5c
  0009ea46: ldr r0,[r4,#0x2c]
  0009ea48: cmp r0,#0x0
  0009ea4a: blt 0x0009ea50
  0009ea4c: blx 0x00070a5c
  0009ea50: ldr r0,[r4,#0x30]
  0009ea52: cmp r0,#0x0
  0009ea54: blt 0x0009ea5a
  0009ea56: blx 0x00070a5c
  0009ea5a: ldrb.w r0,[r8,#0x5c]
  0009ea5e: cmp r0,#0x0
  0009ea60: beq 0x0009eaf6
  0009ea62: ldr r0,[0x0009eb88]
  0009ea64: ldr.w r1,[r8,#0x60]
  0009ea68: add r0,pc
  0009ea6a: ldr r5,[r0,#0x0]
  0009ea6c: movw r0,#0x8892
  0009ea70: blx r5
  0009ea72: ldr r1,[0x0009eb8c]
  0009ea74: mov.w r9,#0x0
  0009ea78: ldr r0,[r4,#0x20]
  0009ea7a: movw r2,#0x1406
  0009ea7e: add r1,pc
  0009ea80: movs r3,#0x0
  0009ea82: strd r9,r9,[sp,#0x0]
  0009ea86: ldr r6,[r1,#0x0]
  0009ea88: movs r1,#0x3
  0009ea8a: blx r6
  0009ea8c: ldr.w r1,[r8,#0x68]
  0009ea90: movw r0,#0x8892
  0009ea94: blx r5
  0009ea96: ldr r0,[r4,#0x24]
  0009ea98: movs r1,#0x2
  0009ea9a: movw r2,#0x1406
  0009ea9e: movs r3,#0x0
  0009eaa0: strd r9,r9,[sp,#0x0]
  0009eaa4: blx r6
  0009eaa6: ldr.w r1,[r8,#0x6c]
  0009eaaa: movw r0,#0x8892
  0009eaae: blx r5
  0009eab0: ldr r0,[r4,#0x28]
  0009eab2: movs r1,#0x3
  0009eab4: movw r2,#0x1406
  0009eab8: movs r3,#0x0
  0009eaba: strd r9,r9,[sp,#0x0]
  0009eabe: blx r6
  0009eac0: ldr.w r1,[r8,#0x70]
  0009eac4: movw r0,#0x8892
  0009eac8: blx r5
  0009eaca: ldr r0,[r4,#0x2c]
  0009eacc: movs r1,#0x3
  0009eace: movw r2,#0x1406
  0009ead2: movs r3,#0x0
  0009ead4: strd r9,r9,[sp,#0x0]
  0009ead8: blx r6
  0009eada: ldr.w r1,[r8,#0x74]
  0009eade: movw r0,#0x8892
  0009eae2: blx r5
  0009eae4: ldr r0,[r4,#0x30]
  0009eae6: movs r1,#0x3
  0009eae8: movw r2,#0x1406
  0009eaec: movs r3,#0x0
  0009eaee: strd r9,r9,[sp,#0x0]
  0009eaf2: blx r6
  0009eaf4: b 0x0009eb82
  0009eaf6: ldr r0,[r4,#0x20]
  0009eaf8: cmp r0,#0x0
  0009eafa: blt 0x0009eb12
  0009eafc: ldr.w r1,[r8,#0x4]
  0009eb00: movs r2,#0x0
  0009eb02: movs r3,#0x0
  0009eb04: strd r2,r1,[sp,#0x0]
  0009eb08: movs r1,#0x3
  0009eb0a: movw r2,#0x1406
  0009eb0e: blx 0x000709c0
  0009eb12: ldr r0,[r4,#0x24]
  0009eb14: cmp r0,#0x0
  0009eb16: blt 0x0009eb2e
  0009eb18: ldr.w r1,[r8,#0x8]
  0009eb1c: movs r2,#0x0
  0009eb1e: movs r3,#0x0
  0009eb20: strd r2,r1,[sp,#0x0]
  0009eb24: movs r1,#0x2
  0009eb26: movw r2,#0x1406
  0009eb2a: blx 0x000709c0
  0009eb2e: ldr r0,[r4,#0x28]
  0009eb30: cmp r0,#0x0
  0009eb32: blt 0x0009eb4a
  0009eb34: ldr.w r1,[r8,#0x10]
  0009eb38: movs r2,#0x0
  0009eb3a: movs r3,#0x0
  0009eb3c: strd r2,r1,[sp,#0x0]
  0009eb40: movs r1,#0x3
  0009eb42: movw r2,#0x1406
  0009eb46: blx 0x000709c0
  0009eb4a: ldr r0,[r4,#0x2c]
  0009eb4c: cmp r0,#0x0
  0009eb4e: blt 0x0009eb66
  0009eb50: ldr.w r1,[r8,#0x14]
  0009eb54: movs r2,#0x0
  0009eb56: movs r3,#0x0
  0009eb58: strd r2,r1,[sp,#0x0]
  0009eb5c: movs r1,#0x3
  0009eb5e: movw r2,#0x1406
  0009eb62: blx 0x000709c0
  0009eb66: ldr r0,[r4,#0x30]
  0009eb68: cmp r0,#0x0
  0009eb6a: blt 0x0009eb82
  0009eb6c: ldr.w r1,[r8,#0x18]
  0009eb70: movs r2,#0x0
  0009eb72: movs r3,#0x0
  0009eb74: strd r2,r1,[sp,#0x0]
  0009eb78: movs r1,#0x3
  0009eb7a: movw r2,#0x1406
  0009eb7e: blx 0x000709c0
  0009eb82: pop.w {r2,r3,r8,r9,r11}
  0009eb86: pop {r4,r5,r6,r7,pc}
```
