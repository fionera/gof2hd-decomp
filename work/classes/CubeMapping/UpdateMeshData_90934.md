# CubeMapping::UpdateMeshData
elf 0x90934 body 364
Sig: undefined __thiscall UpdateMeshData(CubeMapping * this, Mesh * param_1, Engine * param_2)

## decompile
```c

/* AbyssEngine::CubeMapping::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) */

void __thiscall
AbyssEngine::CubeMapping::UpdateMeshData(CubeMapping *this,Mesh *param_1,Engine *param_2)

{
  Mesh MVar1;
  code *pcVar2;
  code *pcVar3;
  undefined1 *puVar4;
  
  puVar4 = &stack0xfffffff8;
  if (this[9] != (CubeMapping)0x0) {
    glUniform4fv(*(undefined4 *)(this + 0x58),1,param_2 + 0xd0);
    pcVar2 = *(code **)(DAT_000a0aa0 + 0xa095e);
    (*pcVar2)(*(undefined4 *)(this + 0x48),1,param_2 + 0x2cc);
    (*pcVar2)(*(undefined4 *)(this + 0x4c),1,param_2 + 0x2fc);
    (*pcVar2)(*(undefined4 *)(this + 0x50),1,param_2 + 0x2e4);
    glUniform1f(*(undefined4 *)(this + 0x54),*(undefined4 *)(param_2 + 0x2c8));
    this[9] = (CubeMapping)0x0;
  }
  glUniform1f(*(undefined4 *)(this + 0x44),*(undefined4 *)(param_2 + 0xcc));
  glUniformMatrix4fv(*(undefined4 *)(this + 0x2c),1,0,param_2 + 0x104);
  glUniformMatrix3fv(*(undefined4 *)(this + 0x30),1,0,param_2 + 0x204);
  glUniform4f(*(undefined4 *)(this + 0x38),*(undefined4 *)(param_2 + 0x330),
              *(undefined4 *)(param_2 + 0x334),*(undefined4 *)(param_2 + 0x338),
              *(undefined4 *)(param_2 + 0x378),puVar4);
  glUniform3f(*(undefined4 *)(this + 0x34),*(undefined4 *)(param_2 + 0x34c),
              *(undefined4 *)(param_2 + 0x350),*(undefined4 *)(param_2 + 0x354));
  pcVar2 = *(code **)(DAT_000a0aa4 + 0xa09d6);
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
    pcVar2 = *(code **)(DAT_000a0aa8 + 0xa09f0);
    (*pcVar2)(0x8892,*(undefined4 *)(param_1 + 0x60));
    pcVar3 = *(code **)(DAT_000a0aac + 0xa0a04);
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
  000a0934: push {r4,r5,r6,r7,lr}
  000a0936: add r7,sp,#0xc
  000a0938: push {r6,r7,r8,r9,r11}
  000a093c: mov r4,r0
  000a093e: ldrb r0,[r0,#0x9]
  000a0940: mov r6,r2
  000a0942: mov r9,r1
  000a0944: cbz r0,0x000a0984
  000a0946: ldr r0,[r4,#0x58]
  000a0948: add.w r2,r6,#0xd0
  000a094c: movs r1,#0x1
  000a094e: blx 0x00070984
  000a0952: ldr r1,[0x000a0aa0]
  000a0954: add.w r2,r6,#0x2cc
  000a0958: ldr r0,[r4,#0x48]
  000a095a: add r1,pc
  000a095c: ldr r5,[r1,#0x0]
  000a095e: movs r1,#0x1
  000a0960: blx r5
  000a0962: ldr r0,[r4,#0x4c]
  000a0964: add.w r2,r6,#0x2fc
  000a0968: movs r1,#0x1
  000a096a: blx r5
  000a096c: ldr r0,[r4,#0x50]
  000a096e: add.w r2,r6,#0x2e4
  000a0972: movs r1,#0x1
  000a0974: blx r5
  000a0976: ldr.w r1,[r6,#0x2c8]
  000a097a: ldr r0,[r4,#0x54]
  000a097c: blx 0x00070978
  000a0980: movs r0,#0x0
  000a0982: strb r0,[r4,#0x9]
  000a0984: ldr.w r1,[r6,#0xcc]
  000a0988: ldr r0,[r4,#0x44]
  000a098a: blx 0x00070978
  000a098e: ldr r0,[r4,#0x2c]
  000a0990: add.w r3,r6,#0x104
  000a0994: movs r1,#0x1
  000a0996: movs r2,#0x0
  000a0998: mov.w r8,#0x0
  000a099c: blx 0x00070990
  000a09a0: ldr r0,[r4,#0x30]
  000a09a2: add.w r3,r6,#0x204
  000a09a6: movs r1,#0x1
  000a09a8: movs r2,#0x0
  000a09aa: blx 0x0007099c
  000a09ae: add.w r3,r6,#0x330
  000a09b2: ldmia r3,{r1,r2,r3}
  000a09b4: ldr r0,[r4,#0x38]
  000a09b6: vldr.32 s0,[r6,#0x378]
  000a09ba: vstr.32 s0,[sp]
  000a09be: blx 0x000709a8
  000a09c2: add.w r3,r6,#0x34c
  000a09c6: ldmia r3,{r1,r2,r3}
  000a09c8: ldr r0,[r4,#0x34]
  000a09ca: blx 0x000709b4
  000a09ce: ldr r1,[0x000a0aa4]
  000a09d0: ldr r0,[r4,#0x20]
  000a09d2: add r1,pc
  000a09d4: ldr r5,[r1,#0x0]
  000a09d6: blx r5
  000a09d8: ldr r0,[r4,#0x28]
  000a09da: blx r5
  000a09dc: ldr r0,[r4,#0x24]
  000a09de: blx r5
  000a09e0: ldrb.w r0,[r9,#0x5c]
  000a09e4: cbz r0,0x000a0a42
  000a09e6: ldr r0,[0x000a0aa8]
  000a09e8: ldr.w r1,[r9,#0x60]
  000a09ec: add r0,pc
  000a09ee: ldr r5,[r0,#0x0]
  000a09f0: movw r0,#0x8892
  000a09f4: blx r5
  000a09f6: ldr r1,[0x000a0aac]
  000a09f8: movw r2,#0x1406
  000a09fc: ldr r0,[r4,#0x20]
  000a09fe: movs r3,#0x0
  000a0a00: add r1,pc
  000a0a02: strd r8,r8,[sp,#0x0]
  000a0a06: ldr r6,[r1,#0x0]
  000a0a08: movs r1,#0x3
  000a0a0a: blx r6
  000a0a0c: ldr.w r1,[r9,#0x68]
  000a0a10: movw r0,#0x8892
  000a0a14: blx r5
  000a0a16: ldr r0,[r4,#0x28]
  000a0a18: movs r1,#0x2
  000a0a1a: movw r2,#0x1406
  000a0a1e: movs r3,#0x0
  000a0a20: strd r8,r8,[sp,#0x0]
  000a0a24: blx r6
  000a0a26: ldr.w r1,[r9,#0x6c]
  000a0a2a: movw r0,#0x8892
  000a0a2e: blx r5
  000a0a30: ldr r0,[r4,#0x24]
  000a0a32: movs r1,#0x3
  000a0a34: movw r2,#0x1406
  000a0a38: movs r3,#0x0
  000a0a3a: strd r8,r8,[sp,#0x0]
  000a0a3e: blx r6
  000a0a40: b 0x000a0a9a
  000a0a42: ldr.w r1,[r9,#0x4]
  000a0a46: movw r2,#0x1406
  000a0a4a: ldr r0,[r4,#0x20]
  000a0a4c: movs r3,#0x0
  000a0a4e: strd r8,r1,[sp,#0x0]
  000a0a52: movs r1,#0x3
  000a0a54: blx 0x000709c0
  000a0a58: ldrb.w r0,[r9,#0x0]
  000a0a5c: tst r0,#0x2
  000a0a60: beq 0x000a0a7e
  000a0a62: ldr.w r1,[r9,#0x8]
  000a0a66: movs r2,#0x0
  000a0a68: ldr r0,[r4,#0x28]
  000a0a6a: movs r3,#0x0
  000a0a6c: strd r2,r1,[sp,#0x0]
  000a0a70: movs r1,#0x2
  000a0a72: movw r2,#0x1406
  000a0a76: blx 0x000709c0
  000a0a7a: ldrb.w r0,[r9,#0x0]
  000a0a7e: lsls r0,r0,#0x1d
  000a0a80: bpl 0x000a0a9a
  000a0a82: ldr.w r1,[r9,#0x10]
  000a0a86: movs r2,#0x0
  000a0a88: ldr r0,[r4,#0x24]
  000a0a8a: movs r3,#0x0
  000a0a8c: strd r2,r1,[sp,#0x0]
  000a0a90: movs r1,#0x3
  000a0a92: movw r2,#0x1406
  000a0a96: blx 0x000709c0
  000a0a9a: pop.w {r2,r3,r8,r9,r11}
  000a0a9e: pop {r4,r5,r6,r7,pc}
```
