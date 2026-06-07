# GenericShader1::UpdateMeshData
elf 0x8ed8a body 344
Sig: undefined __thiscall UpdateMeshData(GenericShader1 * this, Mesh * param_1, Engine * param_2)

## decompile
```c

/* AbyssEngine::GenericShader1::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) */

void __thiscall
AbyssEngine::GenericShader1::UpdateMeshData(GenericShader1 *this,Mesh *param_1,Engine *param_2)

{
  if (-1 < *(int *)(this + 0x34)) {
    glUniformMatrix4fv(*(int *)(this + 0x34),1,0,param_2 + 0x104);
  }
  if (-1 < *(int *)(this + 0x38)) {
    glUniformMatrix3fv(*(int *)(this + 0x38),1,0,param_2 + 0x204);
  }
  if (this[9] != (GenericShader1)0x0) {
    if (-1 < *(int *)(this + 0x3c)) {
      glUniform3f(*(int *)(this + 0x3c),*(undefined4 *)(param_2 + 0x330),
                  *(undefined4 *)(param_2 + 0x334),*(undefined4 *)(param_2 + 0x338));
    }
    if (-1 < *(int *)(this + 0x40)) {
      glUniform3f(*(int *)(this + 0x40),*(undefined4 *)(param_2 + 0x34c),
                  *(undefined4 *)(param_2 + 0x350),*(undefined4 *)(param_2 + 0x354));
    }
    if (-1 < *(int *)(this + 0x48)) {
      glUniform4fv(*(int *)(this + 0x48),1,param_2 + 0xd0);
    }
    if (-1 < *(int *)(this + 0x4c)) {
      glUniform4fv(*(int *)(this + 0x4c),1,param_2 + 0x2a8);
    }
    if (-1 < *(int *)(this + 0x50)) {
      glUniform4fv(*(int *)(this + 0x50),1,param_2 + 0x298);
    }
    if (-1 < *(int *)(this + 0x54)) {
      glUniform4fv(*(int *)(this + 0x54),1,param_2 + 0x2b8);
    }
    this[9] = (GenericShader1)0x0;
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
  return;
}

```

## target disasm
```
  0009ed8a: push {r4,r5,r6,r7,lr}
  0009ed8c: add r7,sp,#0xc
  0009ed8e: push.w r11
  0009ed92: sub sp,#0x8
  0009ed94: mov r5,r0
  0009ed96: ldr r0,[r0,#0x34]
  0009ed98: mov r6,r2
  0009ed9a: mov r4,r1
  0009ed9c: cmp r0,#0x0
  0009ed9e: blt 0x0009edac
  0009eda0: add.w r3,r6,#0x104
  0009eda4: movs r1,#0x1
  0009eda6: movs r2,#0x0
  0009eda8: blx 0x00070990
  0009edac: ldr r0,[r5,#0x38]
  0009edae: cmp r0,#0x0
  0009edb0: blt 0x0009edbe
  0009edb2: add.w r3,r6,#0x204
  0009edb6: movs r1,#0x1
  0009edb8: movs r2,#0x0
  0009edba: blx 0x0007099c
  0009edbe: ldrb r0,[r5,#0x9]
  0009edc0: cbz r0,0x0009ee26
  0009edc2: ldr r0,[r5,#0x3c]
  0009edc4: cmp r0,#0x0
  0009edc6: blt 0x0009edd2
  0009edc8: add.w r3,r6,#0x330
  0009edcc: ldmia r3,{r1,r2,r3}
  0009edce: blx 0x000709b4
  0009edd2: ldr r0,[r5,#0x40]
  0009edd4: cmp r0,#0x0
  0009edd6: blt 0x0009ede2
  0009edd8: add.w r3,r6,#0x34c
  0009eddc: ldmia r3,{r1,r2,r3}
  0009edde: blx 0x000709b4
  0009ede2: ldr r0,[r5,#0x48]
  0009ede4: cmp r0,#0x0
  0009ede6: blt 0x0009edf2
  0009ede8: add.w r2,r6,#0xd0
  0009edec: movs r1,#0x1
  0009edee: blx 0x00070984
  0009edf2: ldr r0,[r5,#0x4c]
  0009edf4: cmp r0,#0x0
  0009edf6: blt 0x0009ee02
  0009edf8: add.w r2,r6,#0x2a8
  0009edfc: movs r1,#0x1
  0009edfe: blx 0x00070984
  0009ee02: ldr r0,[r5,#0x50]
  0009ee04: cmp r0,#0x0
  0009ee06: blt 0x0009ee12
  0009ee08: add.w r2,r6,#0x298
  0009ee0c: movs r1,#0x1
  0009ee0e: blx 0x00070984
  0009ee12: ldr r0,[r5,#0x54]
  0009ee14: cmp r0,#0x0
  0009ee16: blt 0x0009ee22
  0009ee18: add.w r2,r6,#0x2b8
  0009ee1c: movs r1,#0x1
  0009ee1e: blx 0x00070984
  0009ee22: movs r0,#0x0
  0009ee24: strb r0,[r5,#0x9]
  0009ee26: ldr r0,[r5,#0x20]
  0009ee28: cmp r0,#0x0
  0009ee2a: blt 0x0009ee30
  0009ee2c: blx 0x00070a5c
  0009ee30: ldr r0,[r5,#0x24]
  0009ee32: cmp r0,#0x0
  0009ee34: blt 0x0009ee3a
  0009ee36: blx 0x00070a5c
  0009ee3a: ldr r0,[r5,#0x28]
  0009ee3c: cmp r0,#0x0
  0009ee3e: blt 0x0009ee44
  0009ee40: blx 0x00070a5c
  0009ee44: ldr r0,[r5,#0x2c]
  0009ee46: cmp r0,#0x0
  0009ee48: blt 0x0009ee4e
  0009ee4a: blx 0x00070a5c
  0009ee4e: ldr r0,[r5,#0x30]
  0009ee50: cmp r0,#0x0
  0009ee52: blt 0x0009ee58
  0009ee54: blx 0x00070a5c
  0009ee58: ldr r0,[r5,#0x20]
  0009ee5a: cmp r0,#0x0
  0009ee5c: blt 0x0009ee72
  0009ee5e: ldr r1,[r4,#0x4]
  0009ee60: movs r2,#0x0
  0009ee62: movs r3,#0x0
  0009ee64: strd r2,r1,[sp,#0x0]
  0009ee68: movs r1,#0x3
  0009ee6a: movw r2,#0x1406
  0009ee6e: blx 0x000709c0
  0009ee72: ldr r0,[r5,#0x24]
  0009ee74: cmp r0,#0x0
  0009ee76: blt 0x0009ee8c
  0009ee78: ldr r1,[r4,#0x8]
  0009ee7a: movs r2,#0x0
  0009ee7c: movs r3,#0x0
  0009ee7e: strd r2,r1,[sp,#0x0]
  0009ee82: movs r1,#0x2
  0009ee84: movw r2,#0x1406
  0009ee88: blx 0x000709c0
  0009ee8c: ldr r0,[r5,#0x28]
  0009ee8e: cmp r0,#0x0
  0009ee90: blt 0x0009eea6
  0009ee92: ldr r1,[r4,#0x10]
  0009ee94: movs r2,#0x0
  0009ee96: movs r3,#0x0
  0009ee98: strd r2,r1,[sp,#0x0]
  0009ee9c: movs r1,#0x3
  0009ee9e: movw r2,#0x1406
  0009eea2: blx 0x000709c0
  0009eea6: ldr r0,[r5,#0x2c]
  0009eea8: cmp r0,#0x0
  0009eeaa: blt 0x0009eec0
  0009eeac: ldr r1,[r4,#0x14]
  0009eeae: movs r2,#0x0
  0009eeb0: movs r3,#0x0
  0009eeb2: strd r2,r1,[sp,#0x0]
  0009eeb6: movs r1,#0x3
  0009eeb8: movw r2,#0x1406
  0009eebc: blx 0x000709c0
  0009eec0: ldr r0,[r5,#0x30]
  0009eec2: cmp r0,#0x0
  0009eec4: blt 0x0009eeda
  0009eec6: ldr r1,[r4,#0x18]
  0009eec8: movs r2,#0x0
  0009eeca: movs r3,#0x0
  0009eecc: strd r2,r1,[sp,#0x0]
  0009eed0: movs r1,#0x3
  0009eed2: movw r2,#0x1406
  0009eed6: blx 0x000709c0
  0009eeda: add sp,#0x8
  0009eedc: pop.w r11
  0009eee0: pop {r4,r5,r6,r7,pc}
```
