# TextureVtxColorShader::UpdateMeshData
elf 0x8bf4c body 426
Sig: undefined __thiscall UpdateMeshData(TextureVtxColorShader * this, Mesh * param_1, Engine * param_2)

## decompile
```c

/* AbyssEngine::TextureVtxColorShader::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) */

void __thiscall
AbyssEngine::TextureVtxColorShader::UpdateMeshData
          (TextureVtxColorShader *this,Mesh *param_1,Engine *param_2)

{
  TextureVtxColorShader *pTVar1;
  TextureVtxColorShader *pTVar2;
  TextureVtxColorShader *pTVar3;
  byte bVar4;
  float *pfVar5;
  int iVar6;
  code *pcVar7;
  byte *pbVar8;
  code *pcVar9;
  
  pbVar8 = *(byte **)(DAT_0009c0f8 + 0x9bf62);
  bVar4 = *pbVar8;
  glUniformMatrix4fv(*(undefined4 *)(this + (uint)bVar4 * 4 + 0x3c),1,0,param_2 + 0x104);
  if (-1 < *(int *)(this + (uint)bVar4 * 4 + 0x5c)) {
    glUniformMatrix4fv(*(int *)(this + (uint)bVar4 * 4 + 0x5c),1,0,param_2 + 0x1c4);
  }
  if (-1 < *(int *)(this + (uint)bVar4 * 4 + 0x8c)) {
    glUniform1f(*(int *)(this + (uint)bVar4 * 4 + 0x8c),1.0 - *(float *)(param_1 + 0x1c));
  }
  if (this[9] != (TextureVtxColorShader)0x0) {
    glUniform4fv(*(undefined4 *)(this + (uint)bVar4 * 4 + 0x44),1,param_2 + 0xd0);
    if (-1 < *(int *)(this + (uint)bVar4 * 4 + 0x54)) {
      glUniform1i(*(int *)(this + (uint)bVar4 * 4 + 0x54),param_1[0x85]);
    }
    iVar6 = *(int *)(this + (uint)bVar4 * 4 + 100);
    if (-1 < iVar6) {
      pfVar5 = AEMath::Vector::operator_cast_to_float_((Vector *)(param_2 + 0x3f0));
      glUniform3fv(iVar6,1,pfVar5);
    }
    if (-1 < *(int *)(this + (uint)bVar4 * 4 + 0x74)) {
      glUniform1f(*(int *)(this + (uint)bVar4 * 4 + 0x74),*(undefined4 *)(param_2 + 1000));
    }
    if (-1 < *(int *)(this + (uint)bVar4 * 4 + 0x6c)) {
      glUniform1f(*(int *)(this + (uint)bVar4 * 4 + 0x6c),*(undefined4 *)(param_2 + 0x3ec));
    }
    if (-1 < *(int *)(this + (uint)bVar4 * 4 + 0x7c)) {
      glUniform1i(*(int *)(this + (uint)bVar4 * 4 + 0x7c),*pbVar8);
    }
    if (-1 < *(int *)(this + (uint)bVar4 * 4 + 0x84)) {
      glUniform3f(*(int *)(this + (uint)bVar4 * 4 + 0x84),*(undefined4 *)(param_2 + 0x34c),
                  *(undefined4 *)(param_2 + 0x350),*(undefined4 *)(param_2 + 0x354));
    }
    this[9] = (TextureVtxColorShader)0x0;
  }
  pTVar1 = this + (uint)bVar4 * 4 + 0x24;
  pcVar7 = *(code **)(DAT_0009c0fc + 0x9c02c);
  (*pcVar7)(*(undefined4 *)pTVar1);
  pTVar2 = this + (uint)bVar4 * 4 + 0x2c;
  (*pcVar7)(*(undefined4 *)pTVar2);
  pTVar3 = this + (uint)bVar4 * 4 + 0x34;
  (*pcVar7)(*(undefined4 *)pTVar3);
  if (param_1[0x5c] != (Mesh)0x0) {
    pcVar9 = *(code **)(DAT_0009c100 + 0x9c04e);
    (*pcVar9)(0x8892,*(undefined4 *)(param_1 + 0x60));
    pcVar7 = *(code **)(DAT_0009c104 + 0x9c066);
    (*pcVar7)(*(undefined4 *)pTVar1,3,0x1406,0,0,0);
    (*pcVar9)(0x8892,*(undefined4 *)(param_1 + 0x68));
    (*pcVar7)(*(undefined4 *)pTVar2,2,0x1406,0,0,0);
    (*pcVar9)(0x8892,*(undefined4 *)(param_1 + 0x78));
    (*pcVar7)(*(undefined4 *)pTVar3,4,0x1406,0,0,0);
    return;
  }
  pcVar7 = *(code **)(DAT_0009c108 + 0x9c0ba);
  (*pcVar7)(*(undefined4 *)pTVar1,3,0x1406,0,0,*(undefined4 *)(param_1 + 4));
  (*pcVar7)(*(undefined4 *)pTVar2,2,0x1406,0,0,*(undefined4 *)(param_1 + 8));
  (*pcVar7)(*(undefined4 *)pTVar3,4,0x1406,0,0,*(undefined4 *)(param_1 + 0xc));
  return;
}

```

## target disasm
```
  0009bf4c: push {r4,r5,r6,r7,lr}
  0009bf4e: add r7,sp,#0xc
  0009bf50: push {r5,r6,r7,r8,r9,r10,r11}
  0009bf54: mov r8,r0
  0009bf56: ldr r0,[0x0009c0f8]
  0009bf58: add.w r3,r2,#0x104
  0009bf5c: mov r6,r2
  0009bf5e: add r0,pc
  0009bf60: mov r11,r1
  0009bf62: movs r1,#0x1
  0009bf64: movs r2,#0x0
  0009bf66: ldr.w r10,[r0,#0x0]
  0009bf6a: ldrb.w r0,[r10,#0x0]
  0009bf6e: add.w r5,r8,r0, lsl #0x2
  0009bf72: ldr r0,[r5,#0x3c]
  0009bf74: blx 0x00070990
  0009bf78: ldr r0,[r5,#0x5c]
  0009bf7a: cmp r0,#0x0
  0009bf7c: blt 0x0009bf8a
  0009bf7e: add.w r3,r6,#0x1c4
  0009bf82: movs r1,#0x1
  0009bf84: movs r2,#0x0
  0009bf86: blx 0x00070990
  0009bf8a: ldr.w r0,[r5,#0x8c]
  0009bf8e: cmp r0,#0x0
  0009bf90: blt 0x0009bfa6
  0009bf92: vmov.f32 s0,0x3f800000
  0009bf96: vldr.32 s2,[r11,#0x1c]
  0009bf9a: vsub.f32 s0,s0,s2
  0009bf9e: vmov r1,s0
  0009bfa2: blx 0x00070978
  0009bfa6: ldrb.w r0,[r8,#0x9]
  0009bfaa: cbz r0,0x0009c020
  0009bfac: ldr r0,[r5,#0x44]
  0009bfae: add.w r2,r6,#0xd0
  0009bfb2: movs r1,#0x1
  0009bfb4: blx 0x00070984
  0009bfb8: ldr r0,[r5,#0x54]
  0009bfba: cmp r0,#0x0
  0009bfbc: blt 0x0009bfc6
  0009bfbe: ldrb.w r1,[r11,#0x85]
  0009bfc2: blx 0x0007096c
  0009bfc6: ldr r4,[r5,#0x64]
  0009bfc8: cmp r4,#0x0
  0009bfca: blt 0x0009bfde
  0009bfcc: add.w r0,r6,#0x3f0
  0009bfd0: blx 0x00070b28
  0009bfd4: mov r2,r0
  0009bfd6: mov r0,r4
  0009bfd8: movs r1,#0x1
  0009bfda: blx 0x00070a50
  0009bfde: ldr r0,[r5,#0x74]
  0009bfe0: cmp r0,#0x0
  0009bfe2: blt 0x0009bfec
  0009bfe4: ldr.w r1,[r6,#0x3e8]
  0009bfe8: blx 0x00070978
  0009bfec: ldr r0,[r5,#0x6c]
  0009bfee: cmp r0,#0x0
  0009bff0: blt 0x0009bffa
  0009bff2: ldr.w r1,[r6,#0x3ec]
  0009bff6: blx 0x00070978
  0009bffa: ldr r0,[r5,#0x7c]
  0009bffc: cmp r0,#0x0
  0009bffe: blt 0x0009c008
  0009c000: ldrb.w r1,[r10,#0x0]
  0009c004: blx 0x0007096c
  0009c008: ldr.w r0,[r5,#0x84]
  0009c00c: cmp r0,#0x0
  0009c00e: blt 0x0009c01a
  0009c010: add.w r3,r6,#0x34c
  0009c014: ldmia r3,{r1,r2,r3}
  0009c016: blx 0x000709b4
  0009c01a: movs r0,#0x0
  0009c01c: strb.w r0,[r8,#0x9]
  0009c020: ldr r1,[0x0009c0fc]
  0009c022: mov r4,r5
  0009c024: ldr.w r0,[r4,#0x24]!
  0009c028: add r1,pc
  0009c02a: ldr r6,[r1,#0x0]
  0009c02c: blx r6
  0009c02e: mov r9,r5
  0009c030: ldr.w r0,[r9,#0x2c]!
  0009c034: blx r6
  0009c036: ldr.w r0,[r5,#0x34]!
  0009c03a: blx r6
  0009c03c: ldrb.w r0,[r11,#0x5c]
  0009c040: cbz r0,0x0009c0aa
  0009c042: ldr r0,[0x0009c100]
  0009c044: mov r6,r11
  0009c046: ldr.w r1,[r11,#0x60]
  0009c04a: add r0,pc
  0009c04c: ldr.w r11,[r0,#0x0]
  0009c050: movw r0,#0x8892
  0009c054: blx r11
  0009c056: ldr r1,[0x0009c104]
  0009c058: mov.w r10,#0x0
  0009c05c: ldr r0,[r4,#0x0]
  0009c05e: movw r2,#0x1406
  0009c062: add r1,pc
  0009c064: movs r3,#0x0
  0009c066: strd r10,r10,[sp,#0x0]
  0009c06a: ldr.w r8,[r1,#0x0]
  0009c06e: movs r1,#0x3
  0009c070: blx r8
  0009c072: ldr r1,[r6,#0x68]
  0009c074: movw r0,#0x8892
  0009c078: blx r11
  0009c07a: ldr.w r0,[r9,#0x0]
  0009c07e: movs r1,#0x2
  0009c080: movw r2,#0x1406
  0009c084: movs r3,#0x0
  0009c086: strd r10,r10,[sp,#0x0]
  0009c08a: blx r8
  0009c08c: ldr r1,[r6,#0x78]
  0009c08e: movw r0,#0x8892
  0009c092: blx r11
  0009c094: ldr r0,[r5,#0x0]
  0009c096: movs r1,#0x4
  0009c098: movw r2,#0x1406
  0009c09c: movs r3,#0x0
  0009c09e: strd r10,r10,[sp,#0x0]
  0009c0a2: blx r8
  0009c0a4: pop.w {r1,r2,r3,r8,r9,r10,r11}
  0009c0a8: pop {r4,r5,r6,r7,pc}
  0009c0aa: ldr r0,[r4,#0x0]
  0009c0ac: movs r4,#0x0
  0009c0ae: ldr.w r1,[r11,#0x4]
  0009c0b2: movs r3,#0x0
  0009c0b4: ldr r2,[0x0009c108]
  0009c0b6: add r2,pc
  0009c0b8: strd r4,r1,[sp,#0x0]
  0009c0bc: movs r1,#0x3
  0009c0be: ldr r6,[r2,#0x0]
  0009c0c0: movw r2,#0x1406
  0009c0c4: blx r6
  0009c0c6: ldr.w r1,[r11,#0x8]
  0009c0ca: movw r2,#0x1406
  0009c0ce: ldr.w r0,[r9,#0x0]
  0009c0d2: movs r3,#0x0
  0009c0d4: strd r4,r1,[sp,#0x0]
  0009c0d8: movs r1,#0x2
  0009c0da: blx r6
  0009c0dc: ldr.w r1,[r11,#0xc]
  0009c0e0: movw r2,#0x1406
  0009c0e4: ldr r0,[r5,#0x0]
  0009c0e6: movs r3,#0x0
  0009c0e8: strd r4,r1,[sp,#0x0]
  0009c0ec: movs r1,#0x4
  0009c0ee: blx r6
  0009c0f0: pop.w {r1,r2,r3,r8,r9,r10,r11}
  0009c0f4: pop {r4,r5,r6,r7,pc}
```
