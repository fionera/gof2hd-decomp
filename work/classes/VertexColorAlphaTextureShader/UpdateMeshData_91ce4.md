# VertexColorAlphaTextureShader::UpdateMeshData
elf 0x91ce4 body 570
Sig: undefined __thiscall UpdateMeshData(VertexColorAlphaTextureShader * this, Mesh * param_1, Engine * param_2)

## decompile
```c

/* AbyssEngine::VertexColorAlphaTextureShader::UpdateMeshData(AbyssEngine::Mesh*,
   AbyssEngine::Engine*) */

void __thiscall
AbyssEngine::VertexColorAlphaTextureShader::UpdateMeshData
          (VertexColorAlphaTextureShader *this,Mesh *param_1,Engine *param_2)

{
  code *pcVar1;
  code *pcVar2;
  
  if (-1 < *(int *)(this + 0x38)) {
    glUniformMatrix4fv(*(int *)(this + 0x38),1,0,param_2 + 0x104);
  }
  if (-1 < *(int *)(this + 0x3c)) {
    glUniformMatrix3fv(*(int *)(this + 0x3c),1,0,param_2 + 0x204);
  }
  if (this[9] != (VertexColorAlphaTextureShader)0x0) {
    if (-1 < *(int *)(this + 0x40)) {
      glUniform3f(*(int *)(this + 0x40),*(undefined4 *)(param_2 + 0x330),
                  *(undefined4 *)(param_2 + 0x334),*(undefined4 *)(param_2 + 0x338));
    }
    if (-1 < *(int *)(this + 0x44)) {
      glUniform3f(*(int *)(this + 0x44),*(undefined4 *)(param_2 + 0x34c),
                  *(undefined4 *)(param_2 + 0x350),*(undefined4 *)(param_2 + 0x354));
    }
    if (-1 < *(int *)(this + 0x4c)) {
      glUniform4fv(*(int *)(this + 0x4c),1,param_2 + 0xd0);
    }
    if (-1 < *(int *)(this + 0x50)) {
      glUniform4fv(*(int *)(this + 0x50),1,param_2 + 0x2a8);
    }
    if (-1 < *(int *)(this + 0x54)) {
      glUniform4fv(*(int *)(this + 0x54),1,param_2 + 0x298);
    }
    if (-1 < *(int *)(this + 0x58)) {
      glUniform4fv(*(int *)(this + 0x58),1,param_2 + 0x2b8);
    }
    this[9] = (VertexColorAlphaTextureShader)0x0;
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
  if (-1 < *(int *)(this + 0x34)) {
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
    if (-1 < *(int *)(this + 0x34)) {
      glVertexAttribPointer(*(int *)(this + 0x34),4,0x1406,0,0,*(undefined4 *)(param_1 + 0xc));
    }
  }
  else {
    pcVar1 = *(code **)(DAT_000a1f20 + 0xa1dcc);
    (*pcVar1)(0x8892,*(undefined4 *)(param_1 + 0x60));
    pcVar2 = *(code **)(DAT_000a1f24 + 0xa1de2);
    (*pcVar2)(*(undefined4 *)(this + 0x20),3,0x1406,0,0,0);
    (*pcVar1)(0x8892,*(undefined4 *)(param_1 + 0x68));
    (*pcVar2)(*(undefined4 *)(this + 0x24),2,0x1406,0,0,0);
    (*pcVar1)(0x8892,*(undefined4 *)(param_1 + 0x6c));
    (*pcVar2)(*(undefined4 *)(this + 0x28),3,0x1406,0,0,0);
    (*pcVar1)(0x8892,*(undefined4 *)(param_1 + 0x70));
    (*pcVar2)(*(undefined4 *)(this + 0x2c),3,0x1406,0,0,0);
    (*pcVar1)(0x8892,*(undefined4 *)(param_1 + 0x74));
    (*pcVar2)(*(undefined4 *)(this + 0x30),3,0x1406,0,0,0);
    (*pcVar1)(0x8892,*(undefined4 *)(param_1 + 0x78));
    (*pcVar2)(*(undefined4 *)(this + 0x34),4,0x1406,0,0,0);
  }
  return;
}

```

## target disasm
```
  000a1ce4: push {r4,r5,r6,r7,lr}
  000a1ce6: add r7,sp,#0xc
  000a1ce8: push {r6,r7,r8,r9,r11}
  000a1cec: mov r4,r0
  000a1cee: ldr r0,[r0,#0x38]
  000a1cf0: mov r6,r2
  000a1cf2: mov r8,r1
  000a1cf4: cmp r0,#0x0
  000a1cf6: blt 0x000a1d04
  000a1cf8: add.w r3,r6,#0x104
  000a1cfc: movs r1,#0x1
  000a1cfe: movs r2,#0x0
  000a1d00: blx 0x00070990
  000a1d04: ldr r0,[r4,#0x3c]
  000a1d06: cmp r0,#0x0
  000a1d08: blt 0x000a1d16
  000a1d0a: add.w r3,r6,#0x204
  000a1d0e: movs r1,#0x1
  000a1d10: movs r2,#0x0
  000a1d12: blx 0x0007099c
  000a1d16: ldrb r0,[r4,#0x9]
  000a1d18: cbz r0,0x000a1d7e
  000a1d1a: ldr r0,[r4,#0x40]
  000a1d1c: cmp r0,#0x0
  000a1d1e: blt 0x000a1d2a
  000a1d20: add.w r3,r6,#0x330
  000a1d24: ldmia r3,{r1,r2,r3}
  000a1d26: blx 0x000709b4
  000a1d2a: ldr r0,[r4,#0x44]
  000a1d2c: cmp r0,#0x0
  000a1d2e: blt 0x000a1d3a
  000a1d30: add.w r3,r6,#0x34c
  000a1d34: ldmia r3,{r1,r2,r3}
  000a1d36: blx 0x000709b4
  000a1d3a: ldr r0,[r4,#0x4c]
  000a1d3c: cmp r0,#0x0
  000a1d3e: blt 0x000a1d4a
  000a1d40: add.w r2,r6,#0xd0
  000a1d44: movs r1,#0x1
  000a1d46: blx 0x00070984
  000a1d4a: ldr r0,[r4,#0x50]
  000a1d4c: cmp r0,#0x0
  000a1d4e: blt 0x000a1d5a
  000a1d50: add.w r2,r6,#0x2a8
  000a1d54: movs r1,#0x1
  000a1d56: blx 0x00070984
  000a1d5a: ldr r0,[r4,#0x54]
  000a1d5c: cmp r0,#0x0
  000a1d5e: blt 0x000a1d6a
  000a1d60: add.w r2,r6,#0x298
  000a1d64: movs r1,#0x1
  000a1d66: blx 0x00070984
  000a1d6a: ldr r0,[r4,#0x58]
  000a1d6c: cmp r0,#0x0
  000a1d6e: blt 0x000a1d7a
  000a1d70: add.w r2,r6,#0x2b8
  000a1d74: movs r1,#0x1
  000a1d76: blx 0x00070984
  000a1d7a: movs r0,#0x0
  000a1d7c: strb r0,[r4,#0x9]
  000a1d7e: ldr r0,[r4,#0x20]
  000a1d80: cmp r0,#0x0
  000a1d82: blt 0x000a1d88
  000a1d84: blx 0x00070a5c
  000a1d88: ldr r0,[r4,#0x24]
  000a1d8a: cmp r0,#0x0
  000a1d8c: blt 0x000a1d92
  000a1d8e: blx 0x00070a5c
  000a1d92: ldr r0,[r4,#0x28]
  000a1d94: cmp r0,#0x0
  000a1d96: blt 0x000a1d9c
  000a1d98: blx 0x00070a5c
  000a1d9c: ldr r0,[r4,#0x2c]
  000a1d9e: cmp r0,#0x0
  000a1da0: blt 0x000a1da6
  000a1da2: blx 0x00070a5c
  000a1da6: ldr r0,[r4,#0x30]
  000a1da8: cmp r0,#0x0
  000a1daa: blt 0x000a1db0
  000a1dac: blx 0x00070a5c
  000a1db0: ldr r0,[r4,#0x34]
  000a1db2: cmp r0,#0x0
  000a1db4: blt 0x000a1dba
  000a1db6: blx 0x00070a5c
  000a1dba: ldrb.w r0,[r8,#0x5c]
  000a1dbe: cmp r0,#0x0
  000a1dc0: beq 0x000a1e70
  000a1dc2: ldr r0,[0x000a1f20]
  000a1dc4: ldr.w r1,[r8,#0x60]
  000a1dc8: add r0,pc
  000a1dca: ldr r5,[r0,#0x0]
  000a1dcc: movw r0,#0x8892
  000a1dd0: blx r5
  000a1dd2: ldr r1,[0x000a1f24]
  000a1dd4: mov.w r9,#0x0
  000a1dd8: ldr r0,[r4,#0x20]
  000a1dda: movw r2,#0x1406
  000a1dde: add r1,pc
  000a1de0: movs r3,#0x0
  000a1de2: strd r9,r9,[sp,#0x0]
  000a1de6: ldr r6,[r1,#0x0]
  000a1de8: movs r1,#0x3
  000a1dea: blx r6
  000a1dec: ldr.w r1,[r8,#0x68]
  000a1df0: movw r0,#0x8892
  000a1df4: blx r5
  000a1df6: ldr r0,[r4,#0x24]
  000a1df8: movs r1,#0x2
  000a1dfa: movw r2,#0x1406
  000a1dfe: movs r3,#0x0
  000a1e00: strd r9,r9,[sp,#0x0]
  000a1e04: blx r6
  000a1e06: ldr.w r1,[r8,#0x6c]
  000a1e0a: movw r0,#0x8892
  000a1e0e: blx r5
  000a1e10: ldr r0,[r4,#0x28]
  000a1e12: movs r1,#0x3
  000a1e14: movw r2,#0x1406
  000a1e18: movs r3,#0x0
  000a1e1a: strd r9,r9,[sp,#0x0]
  000a1e1e: blx r6
  000a1e20: ldr.w r1,[r8,#0x70]
  000a1e24: movw r0,#0x8892
  000a1e28: blx r5
  000a1e2a: ldr r0,[r4,#0x2c]
  000a1e2c: movs r1,#0x3
  000a1e2e: movw r2,#0x1406
  000a1e32: movs r3,#0x0
  000a1e34: strd r9,r9,[sp,#0x0]
  000a1e38: blx r6
  000a1e3a: ldr.w r1,[r8,#0x74]
  000a1e3e: movw r0,#0x8892
  000a1e42: blx r5
  000a1e44: ldr r0,[r4,#0x30]
  000a1e46: movs r1,#0x3
  000a1e48: movw r2,#0x1406
  000a1e4c: movs r3,#0x0
  000a1e4e: strd r9,r9,[sp,#0x0]
  000a1e52: blx r6
  000a1e54: ldr.w r1,[r8,#0x78]
  000a1e58: movw r0,#0x8892
  000a1e5c: blx r5
  000a1e5e: ldr r0,[r4,#0x34]
  000a1e60: movs r1,#0x4
  000a1e62: movw r2,#0x1406
  000a1e66: movs r3,#0x0
  000a1e68: strd r9,r9,[sp,#0x0]
  000a1e6c: blx r6
  000a1e6e: b 0x000a1f18
  000a1e70: ldr r0,[r4,#0x20]
  000a1e72: cmp r0,#0x0
  000a1e74: blt 0x000a1e8c
  000a1e76: ldr.w r1,[r8,#0x4]
  000a1e7a: movs r2,#0x0
  000a1e7c: movs r3,#0x0
  000a1e7e: strd r2,r1,[sp,#0x0]
  000a1e82: movs r1,#0x3
  000a1e84: movw r2,#0x1406
  000a1e88: blx 0x000709c0
  000a1e8c: ldr r0,[r4,#0x24]
  000a1e8e: cmp r0,#0x0
  000a1e90: blt 0x000a1ea8
  000a1e92: ldr.w r1,[r8,#0x8]
  000a1e96: movs r2,#0x0
  000a1e98: movs r3,#0x0
  000a1e9a: strd r2,r1,[sp,#0x0]
  000a1e9e: movs r1,#0x2
  000a1ea0: movw r2,#0x1406
  000a1ea4: blx 0x000709c0
  000a1ea8: ldr r0,[r4,#0x28]
  000a1eaa: cmp r0,#0x0
  000a1eac: blt 0x000a1ec4
  000a1eae: ldr.w r1,[r8,#0x10]
  000a1eb2: movs r2,#0x0
  000a1eb4: movs r3,#0x0
  000a1eb6: strd r2,r1,[sp,#0x0]
  000a1eba: movs r1,#0x3
  000a1ebc: movw r2,#0x1406
  000a1ec0: blx 0x000709c0
  000a1ec4: ldr r0,[r4,#0x2c]
  000a1ec6: cmp r0,#0x0
  000a1ec8: blt 0x000a1ee0
  000a1eca: ldr.w r1,[r8,#0x14]
  000a1ece: movs r2,#0x0
  000a1ed0: movs r3,#0x0
  000a1ed2: strd r2,r1,[sp,#0x0]
  000a1ed6: movs r1,#0x3
  000a1ed8: movw r2,#0x1406
  000a1edc: blx 0x000709c0
  000a1ee0: ldr r0,[r4,#0x30]
  000a1ee2: cmp r0,#0x0
  000a1ee4: blt 0x000a1efc
  000a1ee6: ldr.w r1,[r8,#0x18]
  000a1eea: movs r2,#0x0
  000a1eec: movs r3,#0x0
  000a1eee: strd r2,r1,[sp,#0x0]
  000a1ef2: movs r1,#0x3
  000a1ef4: movw r2,#0x1406
  000a1ef8: blx 0x000709c0
  000a1efc: ldr r0,[r4,#0x34]
  000a1efe: cmp r0,#0x0
  000a1f00: blt 0x000a1f18
  000a1f02: ldr.w r1,[r8,#0xc]
  000a1f06: movs r2,#0x0
  000a1f08: movs r3,#0x0
  000a1f0a: strd r2,r1,[sp,#0x0]
  000a1f0e: movs r1,#0x4
  000a1f10: movw r2,#0x1406
  000a1f14: blx 0x000709c0
  000a1f18: pop.w {r2,r3,r8,r9,r11}
  000a1f1c: pop {r4,r5,r6,r7,pc}
```
