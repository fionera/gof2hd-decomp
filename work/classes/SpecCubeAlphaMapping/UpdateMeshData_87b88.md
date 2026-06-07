# SpecCubeAlphaMapping::UpdateMeshData
elf 0x87b88 body 388
Sig: undefined __thiscall UpdateMeshData(SpecCubeAlphaMapping * this, Mesh * param_1, Engine * param_2)

## decompile
```c

/* AbyssEngine::SpecCubeAlphaMapping::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) */

void __thiscall
AbyssEngine::SpecCubeAlphaMapping::UpdateMeshData
          (SpecCubeAlphaMapping *this,Mesh *param_1,Engine *param_2)

{
  Mesh MVar1;
  int iVar2;
  code *pcVar3;
  code *pcVar4;
  undefined4 uVar5;
  undefined1 *puVar6;
  
  puVar6 = &stack0xfffffff8;
  if (this[9] != (SpecCubeAlphaMapping)0x0) {
    uVar5 = 0x3f800000;
    iVar2 = *(int *)(param_1 + 0x30);
    if (((iVar2 != 0) && (*(undefined4 **)(iVar2 + 0x24) != (undefined4 *)0x0)) &&
       (*(int *)(iVar2 + 0x28) == 4)) {
      uVar5 = **(undefined4 **)(iVar2 + 0x24);
    }
    glUniform1f(*(undefined4 *)(this + 0x58),uVar5);
    glUniform4fv(*(undefined4 *)(this + 0x54),1,param_2 + 0xd0);
    pcVar3 = *(code **)(DAT_00097d0c + 0x97bd4);
    (*pcVar3)(*(undefined4 *)(this + 0x44),1,param_2 + 0x2cc);
    (*pcVar3)(*(undefined4 *)(this + 0x48),1,param_2 + 0x2fc);
    (*pcVar3)(*(undefined4 *)(this + 0x4c),1,param_2 + 0x2e4);
    glUniform1f(*(undefined4 *)(this + 0x50),*(undefined4 *)(param_2 + 0x2c8));
    this[9] = (SpecCubeAlphaMapping)0x0;
  }
  glUniformMatrix4fv(*(undefined4 *)(this + 0x2c),1,0,param_2 + 0x104);
  glUniformMatrix3fv(*(undefined4 *)(this + 0x30),1,0,param_2 + 0x204);
  glUniform4f(*(undefined4 *)(this + 0x38),*(undefined4 *)(param_2 + 0x330),
              *(undefined4 *)(param_2 + 0x334),*(undefined4 *)(param_2 + 0x338),
              *(undefined4 *)(param_2 + 0x378),puVar6);
  glUniform3f(*(undefined4 *)(this + 0x34),*(undefined4 *)(param_2 + 0x34c),
              *(undefined4 *)(param_2 + 0x350),*(undefined4 *)(param_2 + 0x354));
  pcVar3 = *(code **)(DAT_00097d10 + 0x97c42);
  (*pcVar3)(*(undefined4 *)(this + 0x20));
  (*pcVar3)(*(undefined4 *)(this + 0x28));
  (*pcVar3)(*(undefined4 *)(this + 0x24));
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
    pcVar3 = *(code **)(DAT_00097d14 + 0x97c5c);
    (*pcVar3)(0x8892,*(undefined4 *)(param_1 + 0x60));
    pcVar4 = *(code **)(DAT_00097d18 + 0x97c70);
    (*pcVar4)(*(undefined4 *)(this + 0x20),3,0x1406,0,0,0);
    (*pcVar3)(0x8892,*(undefined4 *)(param_1 + 0x68));
    (*pcVar4)(*(undefined4 *)(this + 0x28),2,0x1406,0,0,0);
    (*pcVar3)(0x8892,*(undefined4 *)(param_1 + 0x6c));
    (*pcVar4)(*(undefined4 *)(this + 0x24),3,0x1406,0,0,0);
  }
  return;
}

```

## target disasm
```
  00097b88: push {r4,r5,r6,r7,lr}
  00097b8a: add r7,sp,#0xc
  00097b8c: push {r6,r7,r8,r9,r11}
  00097b90: mov r4,r0
  00097b92: ldrb r0,[r0,#0x9]
  00097b94: mov r6,r2
  00097b96: mov r9,r1
  00097b98: cbz r0,0x00097bfa
  00097b9a: vmov.f32 s0,0x3f800000
  00097b9e: ldr.w r1,[r9,#0x30]
  00097ba2: cbz r1,0x00097bb2
  00097ba4: ldr r0,[r1,#0x24]
  00097ba6: cbz r0,0x00097bb2
  00097ba8: ldr r1,[r1,#0x28]
  00097baa: cmp r1,#0x4
  00097bac: it eq
  00097bae: vldr.eq.32 s0,[r0]
  00097bb2: vmov r1,s0
  00097bb6: ldr r0,[r4,#0x58]
  00097bb8: blx 0x00070978
  00097bbc: ldr r0,[r4,#0x54]
  00097bbe: add.w r2,r6,#0xd0
  00097bc2: movs r1,#0x1
  00097bc4: blx 0x00070984
  00097bc8: ldr r0,[0x00097d0c]
  00097bca: add.w r2,r6,#0x2cc
  00097bce: movs r1,#0x1
  00097bd0: add r0,pc
  00097bd2: ldr r5,[r0,#0x0]
  00097bd4: ldr r0,[r4,#0x44]
  00097bd6: blx r5
  00097bd8: ldr r0,[r4,#0x48]
  00097bda: add.w r2,r6,#0x2fc
  00097bde: movs r1,#0x1
  00097be0: blx r5
  00097be2: ldr r0,[r4,#0x4c]
  00097be4: add.w r2,r6,#0x2e4
  00097be8: movs r1,#0x1
  00097bea: blx r5
  00097bec: ldr.w r1,[r6,#0x2c8]
  00097bf0: ldr r0,[r4,#0x50]
  00097bf2: blx 0x00070978
  00097bf6: movs r0,#0x0
  00097bf8: strb r0,[r4,#0x9]
  00097bfa: ldr r0,[r4,#0x2c]
  00097bfc: add.w r3,r6,#0x104
  00097c00: movs r1,#0x1
  00097c02: movs r2,#0x0
  00097c04: mov.w r8,#0x0
  00097c08: blx 0x00070990
  00097c0c: ldr r0,[r4,#0x30]
  00097c0e: add.w r3,r6,#0x204
  00097c12: movs r1,#0x1
  00097c14: movs r2,#0x0
  00097c16: blx 0x0007099c
  00097c1a: add.w r3,r6,#0x330
  00097c1e: ldmia r3,{r1,r2,r3}
  00097c20: ldr r0,[r4,#0x38]
  00097c22: vldr.32 s0,[r6,#0x378]
  00097c26: vstr.32 s0,[sp]
  00097c2a: blx 0x000709a8
  00097c2e: add.w r3,r6,#0x34c
  00097c32: ldmia r3,{r1,r2,r3}
  00097c34: ldr r0,[r4,#0x34]
  00097c36: blx 0x000709b4
  00097c3a: ldr r1,[0x00097d10]
  00097c3c: ldr r0,[r4,#0x20]
  00097c3e: add r1,pc
  00097c40: ldr r5,[r1,#0x0]
  00097c42: blx r5
  00097c44: ldr r0,[r4,#0x28]
  00097c46: blx r5
  00097c48: ldr r0,[r4,#0x24]
  00097c4a: blx r5
  00097c4c: ldrb.w r0,[r9,#0x5c]
  00097c50: cbz r0,0x00097cae
  00097c52: ldr r0,[0x00097d14]
  00097c54: ldr.w r1,[r9,#0x60]
  00097c58: add r0,pc
  00097c5a: ldr r5,[r0,#0x0]
  00097c5c: movw r0,#0x8892
  00097c60: blx r5
  00097c62: ldr r1,[0x00097d18]
  00097c64: movw r2,#0x1406
  00097c68: ldr r0,[r4,#0x20]
  00097c6a: movs r3,#0x0
  00097c6c: add r1,pc
  00097c6e: strd r8,r8,[sp,#0x0]
  00097c72: ldr r6,[r1,#0x0]
  00097c74: movs r1,#0x3
  00097c76: blx r6
  00097c78: ldr.w r1,[r9,#0x68]
  00097c7c: movw r0,#0x8892
  00097c80: blx r5
  00097c82: ldr r0,[r4,#0x28]
  00097c84: movs r1,#0x2
  00097c86: movw r2,#0x1406
  00097c8a: movs r3,#0x0
  00097c8c: strd r8,r8,[sp,#0x0]
  00097c90: blx r6
  00097c92: ldr.w r1,[r9,#0x6c]
  00097c96: movw r0,#0x8892
  00097c9a: blx r5
  00097c9c: ldr r0,[r4,#0x24]
  00097c9e: movs r1,#0x3
  00097ca0: movw r2,#0x1406
  00097ca4: movs r3,#0x0
  00097ca6: strd r8,r8,[sp,#0x0]
  00097caa: blx r6
  00097cac: b 0x00097d06
  00097cae: ldr.w r1,[r9,#0x4]
  00097cb2: movw r2,#0x1406
  00097cb6: ldr r0,[r4,#0x20]
  00097cb8: movs r3,#0x0
  00097cba: strd r8,r1,[sp,#0x0]
  00097cbe: movs r1,#0x3
  00097cc0: blx 0x000709c0
  00097cc4: ldrb.w r0,[r9,#0x0]
  00097cc8: tst r0,#0x2
  00097ccc: beq 0x00097cea
  00097cce: ldr.w r1,[r9,#0x8]
  00097cd2: movs r2,#0x0
  00097cd4: ldr r0,[r4,#0x28]
  00097cd6: movs r3,#0x0
  00097cd8: strd r2,r1,[sp,#0x0]
  00097cdc: movs r1,#0x2
  00097cde: movw r2,#0x1406
  00097ce2: blx 0x000709c0
  00097ce6: ldrb.w r0,[r9,#0x0]
  00097cea: lsls r0,r0,#0x1d
  00097cec: bpl 0x00097d06
  00097cee: ldr.w r1,[r9,#0x10]
  00097cf2: movs r2,#0x0
  00097cf4: ldr r0,[r4,#0x24]
  00097cf6: movs r3,#0x0
  00097cf8: strd r2,r1,[sp,#0x0]
  00097cfc: movs r1,#0x3
  00097cfe: movw r2,#0x1406
  00097d02: blx 0x000709c0
  00097d06: pop.w {r2,r3,r8,r9,r11}
  00097d0a: pop {r4,r5,r6,r7,pc}
```
