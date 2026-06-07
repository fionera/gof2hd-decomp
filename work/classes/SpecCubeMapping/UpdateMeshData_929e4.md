# SpecCubeMapping::UpdateMeshData
elf 0x929e4 body 364
Sig: undefined __thiscall UpdateMeshData(SpecCubeMapping * this, Mesh * param_1, Engine * param_2)

## decompile
```c

/* AbyssEngine::SpecCubeMapping::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) */

void __thiscall
AbyssEngine::SpecCubeMapping::UpdateMeshData(SpecCubeMapping *this,Mesh *param_1,Engine *param_2)

{
  Mesh MVar1;
  code *pcVar2;
  code *pcVar3;
  undefined1 *puVar4;
  
  puVar4 = &stack0xfffffff8;
  if (this[9] != (SpecCubeMapping)0x0) {
    glUniform4fv(*(undefined4 *)(this + 0x58),1,param_2 + 0xd0);
    pcVar2 = *(code **)(DAT_000a2b50 + 0xa2a0e);
    (*pcVar2)(*(undefined4 *)(this + 0x48),1,param_2 + 0x2cc);
    (*pcVar2)(*(undefined4 *)(this + 0x4c),1,param_2 + 0x2fc);
    (*pcVar2)(*(undefined4 *)(this + 0x50),1,param_2 + 0x2e4);
    glUniform1f(*(undefined4 *)(this + 0x54),*(undefined4 *)(param_2 + 0x2c8));
    this[9] = (SpecCubeMapping)0x0;
  }
  glUniform1f(*(undefined4 *)(this + 0x44),*(undefined4 *)(param_2 + 0xcc));
  glUniformMatrix4fv(*(undefined4 *)(this + 0x2c),1,0,param_2 + 0x104);
  glUniformMatrix3fv(*(undefined4 *)(this + 0x30),1,0,param_2 + 0x204);
  glUniform4f(*(undefined4 *)(this + 0x38),*(undefined4 *)(param_2 + 0x330),
              *(undefined4 *)(param_2 + 0x334),*(undefined4 *)(param_2 + 0x338),
              *(undefined4 *)(param_2 + 0x378),puVar4);
  glUniform3f(*(undefined4 *)(this + 0x34),*(undefined4 *)(param_2 + 0x34c),
              *(undefined4 *)(param_2 + 0x350),*(undefined4 *)(param_2 + 0x354));
  pcVar2 = *(code **)(DAT_000a2b54 + 0xa2a86);
  (*pcVar2)(*(undefined4 *)(this + 0x20));
  (*pcVar2)(*(undefined4 *)(this + 0x28));
  (*pcVar2)(*(undefined4 *)(this + 0x24));
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
  }
  else {
    pcVar2 = *(code **)(DAT_000a2b58 + 0xa2aa0);
    (*pcVar2)(0x8892,*(undefined4 *)(param_1 + 0x60));
    pcVar3 = *(code **)(DAT_000a2b5c + 0xa2ab4);
    (*pcVar3)(*(undefined4 *)(this + 0x20),3,0x1406,0,0,0);
    (*pcVar2)(0x8892,*(undefined4 *)(param_1 + 0x68));
    (*pcVar3)(*(undefined4 *)(this + 0x28),2,0x1406,0,0,0);
    (*pcVar2)(0x8892,*(undefined4 *)(param_1 + 0x6c));
    (*pcVar3)(*(undefined4 *)(this + 0x24),3,0x1406,0,0,0);
  }
  return;
}

```

## target disasm
```
  000a29e4: push {r4,r5,r6,r7,lr}
  000a29e6: add r7,sp,#0xc
  000a29e8: push {r6,r7,r8,r9,r11}
  000a29ec: mov r4,r0
  000a29ee: ldrb r0,[r0,#0x9]
  000a29f0: mov r6,r2
  000a29f2: mov r9,r1
  000a29f4: cbz r0,0x000a2a34
  000a29f6: ldr r0,[r4,#0x58]
  000a29f8: add.w r2,r6,#0xd0
  000a29fc: movs r1,#0x1
  000a29fe: blx 0x00070984
  000a2a02: ldr r1,[0x000a2b50]
  000a2a04: add.w r2,r6,#0x2cc
  000a2a08: ldr r0,[r4,#0x48]
  000a2a0a: add r1,pc
  000a2a0c: ldr r5,[r1,#0x0]
  000a2a0e: movs r1,#0x1
  000a2a10: blx r5
  000a2a12: ldr r0,[r4,#0x4c]
  000a2a14: add.w r2,r6,#0x2fc
  000a2a18: movs r1,#0x1
  000a2a1a: blx r5
  000a2a1c: ldr r0,[r4,#0x50]
  000a2a1e: add.w r2,r6,#0x2e4
  000a2a22: movs r1,#0x1
  000a2a24: blx r5
  000a2a26: ldr.w r1,[r6,#0x2c8]
  000a2a2a: ldr r0,[r4,#0x54]
  000a2a2c: blx 0x00070978
  000a2a30: movs r0,#0x0
  000a2a32: strb r0,[r4,#0x9]
  000a2a34: ldr.w r1,[r6,#0xcc]
  000a2a38: ldr r0,[r4,#0x44]
  000a2a3a: blx 0x00070978
  000a2a3e: ldr r0,[r4,#0x2c]
  000a2a40: add.w r3,r6,#0x104
  000a2a44: movs r1,#0x1
  000a2a46: movs r2,#0x0
  000a2a48: mov.w r8,#0x0
  000a2a4c: blx 0x00070990
  000a2a50: ldr r0,[r4,#0x30]
  000a2a52: add.w r3,r6,#0x204
  000a2a56: movs r1,#0x1
  000a2a58: movs r2,#0x0
  000a2a5a: blx 0x0007099c
  000a2a5e: add.w r3,r6,#0x330
  000a2a62: ldmia r3,{r1,r2,r3}
  000a2a64: ldr r0,[r4,#0x38]
  000a2a66: vldr.32 s0,[r6,#0x378]
  000a2a6a: vstr.32 s0,[sp]
  000a2a6e: blx 0x000709a8
  000a2a72: add.w r3,r6,#0x34c
  000a2a76: ldmia r3,{r1,r2,r3}
  000a2a78: ldr r0,[r4,#0x34]
  000a2a7a: blx 0x000709b4
  000a2a7e: ldr r1,[0x000a2b54]
  000a2a80: ldr r0,[r4,#0x20]
  000a2a82: add r1,pc
  000a2a84: ldr r5,[r1,#0x0]
  000a2a86: blx r5
  000a2a88: ldr r0,[r4,#0x28]
  000a2a8a: blx r5
  000a2a8c: ldr r0,[r4,#0x24]
  000a2a8e: blx r5
  000a2a90: ldrb.w r0,[r9,#0x5c]
  000a2a94: cbz r0,0x000a2af2
  000a2a96: ldr r0,[0x000a2b58]
  000a2a98: ldr.w r1,[r9,#0x60]
  000a2a9c: add r0,pc
  000a2a9e: ldr r5,[r0,#0x0]
  000a2aa0: movw r0,#0x8892
  000a2aa4: blx r5
  000a2aa6: ldr r1,[0x000a2b5c]
  000a2aa8: movw r2,#0x1406
  000a2aac: ldr r0,[r4,#0x20]
  000a2aae: movs r3,#0x0
  000a2ab0: add r1,pc
  000a2ab2: strd r8,r8,[sp,#0x0]
  000a2ab6: ldr r6,[r1,#0x0]
  000a2ab8: movs r1,#0x3
  000a2aba: blx r6
  000a2abc: ldr.w r1,[r9,#0x68]
  000a2ac0: movw r0,#0x8892
  000a2ac4: blx r5
  000a2ac6: ldr r0,[r4,#0x28]
  000a2ac8: movs r1,#0x2
  000a2aca: movw r2,#0x1406
  000a2ace: movs r3,#0x0
  000a2ad0: strd r8,r8,[sp,#0x0]
  000a2ad4: blx r6
  000a2ad6: ldr.w r1,[r9,#0x6c]
  000a2ada: movw r0,#0x8892
  000a2ade: blx r5
  000a2ae0: ldr r0,[r4,#0x24]
  000a2ae2: movs r1,#0x3
  000a2ae4: movw r2,#0x1406
  000a2ae8: movs r3,#0x0
  000a2aea: strd r8,r8,[sp,#0x0]
  000a2aee: blx r6
  000a2af0: b 0x000a2b4a
  000a2af2: ldr.w r1,[r9,#0x4]
  000a2af6: movw r2,#0x1406
  000a2afa: ldr r0,[r4,#0x20]
  000a2afc: movs r3,#0x0
  000a2afe: strd r8,r1,[sp,#0x0]
  000a2b02: movs r1,#0x3
  000a2b04: blx 0x000709c0
  000a2b08: ldrb.w r0,[r9,#0x0]
  000a2b0c: tst r0,#0x2
  000a2b10: beq 0x000a2b2e
  000a2b12: ldr.w r1,[r9,#0x8]
  000a2b16: movs r2,#0x0
  000a2b18: ldr r0,[r4,#0x28]
  000a2b1a: movs r3,#0x0
  000a2b1c: strd r2,r1,[sp,#0x0]
  000a2b20: movs r1,#0x2
  000a2b22: movw r2,#0x1406
  000a2b26: blx 0x000709c0
  000a2b2a: ldrb.w r0,[r9,#0x0]
  000a2b2e: lsls r0,r0,#0x1d
  000a2b30: bpl 0x000a2b4a
  000a2b32: ldr.w r1,[r9,#0x10]
  000a2b36: movs r2,#0x0
  000a2b38: ldr r0,[r4,#0x24]
  000a2b3a: movs r3,#0x0
  000a2b3c: strd r2,r1,[sp,#0x0]
  000a2b40: movs r1,#0x3
  000a2b42: movw r2,#0x1406
  000a2b46: blx 0x000709c0
  000a2b4a: pop.w {r2,r3,r8,r9,r11}
  000a2b4e: pop {r4,r5,r6,r7,pc}
```
