# CubeNormalMapping::UpdateMeshData
elf 0x90cf0 body 472
Sig: undefined __thiscall UpdateMeshData(CubeNormalMapping * this, Mesh * param_1, Engine * param_2)

## decompile
```c

/* AbyssEngine::CubeNormalMapping::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) */

void __thiscall
AbyssEngine::CubeNormalMapping::UpdateMeshData
          (CubeNormalMapping *this,Mesh *param_1,Engine *param_2)

{
  Mesh MVar1;
  code *pcVar2;
  code *pcVar3;
  
  if (this[9] != (CubeNormalMapping)0x0) {
    pcVar2 = *(code **)(DAT_000a0ec8 + 0xa0d0e);
    (*pcVar2)(*(undefined4 *)(this + 0x5c),1,param_2 + 0xd0);
    (*pcVar2)(*(undefined4 *)(this + 0x54),1,param_2 + 0x2cc);
    (*pcVar2)(*(undefined4 *)(this + 0x58),1,param_2 + 0x2fc);
    (*pcVar2)(*(undefined4 *)(this + 0x60),1,param_2 + 0x2e4);
    glUniform1f(*(undefined4 *)(this + 100),*(undefined4 *)(param_2 + 0x2c8));
    this[9] = (CubeNormalMapping)0x0;
  }
  glUniform1f(*(undefined4 *)(this + 0x50),*(undefined4 *)(param_2 + 0xcc));
  glUniformMatrix4fv(*(undefined4 *)(this + 0x34),1,0,param_2 + 0x104);
  glUniformMatrix3fv(*(undefined4 *)(this + 0x38),1,0,param_2 + 0x204);
  glUniform3f(*(undefined4 *)(this + 0x40),*(undefined4 *)(param_2 + 0x330),
              *(undefined4 *)(param_2 + 0x334),*(undefined4 *)(param_2 + 0x338));
  glUniform3f(*(undefined4 *)(this + 0x3c),*(undefined4 *)(param_2 + 0x34c),
              *(undefined4 *)(param_2 + 0x350),*(undefined4 *)(param_2 + 0x354));
  pcVar2 = *(code **)(DAT_000a0ecc + 0xa0d88);
  (*pcVar2)(*(undefined4 *)(this + 0x20));
  (*pcVar2)(*(undefined4 *)(this + 0x28));
  (*pcVar2)(*(undefined4 *)(this + 0x24));
  (*pcVar2)(*(undefined4 *)(this + 0x2c));
  (*pcVar2)(*(undefined4 *)(this + 0x30));
  if (param_1[0x5c] == (Mesh)0x0) {
    glVertexAttribPointer(*(undefined4 *)(this + 0x20),3,0x1406,0,0,*(undefined4 *)(param_1 + 4));
    MVar1 = *param_1;
    if (((byte)MVar1 & 2) != 0) {
      glVertexAttribPointer(*(undefined4 *)(this + 0x28),2,0x1406,0,0,*(undefined4 *)(param_1 + 8));
      MVar1 = *param_1;
    }
    if ((int)((uint)(byte)MVar1 << 0x1d) < 0) {
      glVertexAttribPointer
                (*(undefined4 *)(this + 0x24),3,0x1406,0,0,*(undefined4 *)(param_1 + 0x10));
    }
    if (-1 < *(int *)(this + 0x2c)) {
      glVertexAttribPointer(*(int *)(this + 0x2c),3,0x1406,0,0,*(undefined4 *)(param_1 + 0x14));
    }
    if (-1 < *(int *)(this + 0x30)) {
      glVertexAttribPointer(*(int *)(this + 0x30),3,0x1406,0,0,*(undefined4 *)(param_1 + 0x18));
    }
  }
  else {
    pcVar2 = *(code **)(DAT_000a0ed0 + 0xa0dac);
    (*pcVar2)(0x8892,*(undefined4 *)(param_1 + 0x60));
    pcVar3 = *(code **)(DAT_000a0ed4 + 0xa0dc0);
    (*pcVar3)(*(undefined4 *)(this + 0x20),3,0x1406,0,0,0);
    (*pcVar2)(0x8892,*(undefined4 *)(param_1 + 0x68));
    (*pcVar3)(*(undefined4 *)(this + 0x28),2,0x1406,0,0,0);
    (*pcVar2)(0x8892,*(undefined4 *)(param_1 + 0x6c));
    (*pcVar3)(*(undefined4 *)(this + 0x24),3,0x1406,0,0,0);
    (*pcVar2)(0x8892,*(undefined4 *)(param_1 + 0x70));
    (*pcVar3)(*(undefined4 *)(this + 0x2c),3,0x1406,0,0,0);
    (*pcVar2)(0x8892,*(undefined4 *)(param_1 + 0x74));
    (*pcVar3)(*(undefined4 *)(this + 0x30),3,0x1406,0,0,0);
  }
  return;
}

```

## target disasm
```
  000a0cf0: push {r4,r5,r6,r7,lr}
  000a0cf2: add r7,sp,#0xc
  000a0cf4: push {r6,r7,r8,r9,r11}
  000a0cf8: mov r4,r0
  000a0cfa: ldrb r0,[r0,#0x9]
  000a0cfc: mov r6,r2
  000a0cfe: mov r9,r1
  000a0d00: cbz r0,0x000a0d3e
  000a0d02: ldr r1,[0x000a0ec8]
  000a0d04: add.w r2,r6,#0xd0
  000a0d08: ldr r0,[r4,#0x5c]
  000a0d0a: add r1,pc
  000a0d0c: ldr r5,[r1,#0x0]
  000a0d0e: movs r1,#0x1
  000a0d10: blx r5
  000a0d12: ldr r0,[r4,#0x54]
  000a0d14: add.w r2,r6,#0x2cc
  000a0d18: movs r1,#0x1
  000a0d1a: blx r5
  000a0d1c: ldr r0,[r4,#0x58]
  000a0d1e: add.w r2,r6,#0x2fc
  000a0d22: movs r1,#0x1
  000a0d24: blx r5
  000a0d26: ldr r0,[r4,#0x60]
  000a0d28: add.w r2,r6,#0x2e4
  000a0d2c: movs r1,#0x1
  000a0d2e: blx r5
  000a0d30: ldr.w r1,[r6,#0x2c8]
  000a0d34: ldr r0,[r4,#0x64]
  000a0d36: blx 0x00070978
  000a0d3a: movs r0,#0x0
  000a0d3c: strb r0,[r4,#0x9]
  000a0d3e: ldr.w r1,[r6,#0xcc]
  000a0d42: ldr r0,[r4,#0x50]
  000a0d44: blx 0x00070978
  000a0d48: ldr r0,[r4,#0x34]
  000a0d4a: add.w r3,r6,#0x104
  000a0d4e: movs r1,#0x1
  000a0d50: movs r2,#0x0
  000a0d52: mov.w r8,#0x0
  000a0d56: blx 0x00070990
  000a0d5a: ldr r0,[r4,#0x38]
  000a0d5c: add.w r3,r6,#0x204
  000a0d60: movs r1,#0x1
  000a0d62: movs r2,#0x0
  000a0d64: blx 0x0007099c
  000a0d68: add.w r3,r6,#0x330
  000a0d6c: ldmia r3,{r1,r2,r3}
  000a0d6e: ldr r0,[r4,#0x40]
  000a0d70: blx 0x000709b4
  000a0d74: add.w r3,r6,#0x34c
  000a0d78: ldmia r3,{r1,r2,r3}
  000a0d7a: ldr r0,[r4,#0x3c]
  000a0d7c: blx 0x000709b4
  000a0d80: ldr r1,[0x000a0ecc]
  000a0d82: ldr r0,[r4,#0x20]
  000a0d84: add r1,pc
  000a0d86: ldr r5,[r1,#0x0]
  000a0d88: blx r5
  000a0d8a: ldr r0,[r4,#0x28]
  000a0d8c: blx r5
  000a0d8e: ldr r0,[r4,#0x24]
  000a0d90: blx r5
  000a0d92: ldr r0,[r4,#0x2c]
  000a0d94: blx r5
  000a0d96: ldr r0,[r4,#0x30]
  000a0d98: blx r5
  000a0d9a: ldrb.w r0,[r9,#0x5c]
  000a0d9e: cmp r0,#0x0
  000a0da0: beq 0x000a0e32
  000a0da2: ldr r0,[0x000a0ed0]
  000a0da4: ldr.w r1,[r9,#0x60]
  000a0da8: add r0,pc
  000a0daa: ldr r5,[r0,#0x0]
  000a0dac: movw r0,#0x8892
  000a0db0: blx r5
  000a0db2: ldr r1,[0x000a0ed4]
  000a0db4: movw r2,#0x1406
  000a0db8: ldr r0,[r4,#0x20]
  000a0dba: movs r3,#0x0
  000a0dbc: add r1,pc
  000a0dbe: strd r8,r8,[sp,#0x0]
  000a0dc2: ldr r6,[r1,#0x0]
  000a0dc4: movs r1,#0x3
  000a0dc6: blx r6
  000a0dc8: ldr.w r1,[r9,#0x68]
  000a0dcc: movw r0,#0x8892
  000a0dd0: blx r5
  000a0dd2: ldr r0,[r4,#0x28]
  000a0dd4: movs r1,#0x2
  000a0dd6: movw r2,#0x1406
  000a0dda: movs r3,#0x0
  000a0ddc: strd r8,r8,[sp,#0x0]
  000a0de0: blx r6
  000a0de2: ldr.w r1,[r9,#0x6c]
  000a0de6: movw r0,#0x8892
  000a0dea: blx r5
  000a0dec: ldr r0,[r4,#0x24]
  000a0dee: movs r1,#0x3
  000a0df0: movw r2,#0x1406
  000a0df4: movs r3,#0x0
  000a0df6: strd r8,r8,[sp,#0x0]
  000a0dfa: blx r6
  000a0dfc: ldr.w r1,[r9,#0x70]
  000a0e00: movw r0,#0x8892
  000a0e04: blx r5
  000a0e06: ldr r0,[r4,#0x2c]
  000a0e08: movs r1,#0x3
  000a0e0a: movw r2,#0x1406
  000a0e0e: movs r3,#0x0
  000a0e10: strd r8,r8,[sp,#0x0]
  000a0e14: blx r6
  000a0e16: ldr.w r1,[r9,#0x74]
  000a0e1a: movw r0,#0x8892
  000a0e1e: blx r5
  000a0e20: ldr r0,[r4,#0x30]
  000a0e22: movs r1,#0x3
  000a0e24: movw r2,#0x1406
  000a0e28: movs r3,#0x0
  000a0e2a: strd r8,r8,[sp,#0x0]
  000a0e2e: blx r6
  000a0e30: b 0x000a0ec2
  000a0e32: ldr.w r1,[r9,#0x4]
  000a0e36: movw r2,#0x1406
  000a0e3a: ldr r0,[r4,#0x20]
  000a0e3c: movs r3,#0x0
  000a0e3e: strd r8,r1,[sp,#0x0]
  000a0e42: movs r1,#0x3
  000a0e44: blx 0x000709c0
  000a0e48: ldrb.w r0,[r9,#0x0]
  000a0e4c: tst r0,#0x2
  000a0e50: beq 0x000a0e6e
  000a0e52: ldr.w r1,[r9,#0x8]
  000a0e56: movs r2,#0x0
  000a0e58: ldr r0,[r4,#0x28]
  000a0e5a: movs r3,#0x0
  000a0e5c: strd r2,r1,[sp,#0x0]
  000a0e60: movs r1,#0x2
  000a0e62: movw r2,#0x1406
  000a0e66: blx 0x000709c0
  000a0e6a: ldrb.w r0,[r9,#0x0]
  000a0e6e: lsls r0,r0,#0x1d
  000a0e70: bpl 0x000a0e8a
  000a0e72: ldr.w r1,[r9,#0x10]
  000a0e76: movs r2,#0x0
  000a0e78: ldr r0,[r4,#0x24]
  000a0e7a: movs r3,#0x0
  000a0e7c: strd r2,r1,[sp,#0x0]
  000a0e80: movs r1,#0x3
  000a0e82: movw r2,#0x1406
  000a0e86: blx 0x000709c0
  000a0e8a: ldr r0,[r4,#0x2c]
  000a0e8c: cmp r0,#0x0
  000a0e8e: blt 0x000a0ea6
  000a0e90: ldr.w r1,[r9,#0x14]
  000a0e94: movs r2,#0x0
  000a0e96: movs r3,#0x0
  000a0e98: strd r2,r1,[sp,#0x0]
  000a0e9c: movs r1,#0x3
  000a0e9e: movw r2,#0x1406
  000a0ea2: blx 0x000709c0
  000a0ea6: ldr r0,[r4,#0x30]
  000a0ea8: cmp r0,#0x0
  000a0eaa: blt 0x000a0ec2
  000a0eac: ldr.w r1,[r9,#0x18]
  000a0eb0: movs r2,#0x0
  000a0eb2: movs r3,#0x0
  000a0eb4: strd r2,r1,[sp,#0x0]
  000a0eb8: movs r1,#0x3
  000a0eba: movw r2,#0x1406
  000a0ebe: blx 0x000709c0
  000a0ec2: pop.w {r2,r3,r8,r9,r11}
  000a0ec6: pop {r4,r5,r6,r7,pc}
```
