# TextureShader::UpdateMeshData
elf 0x910f8 body 346
Sig: undefined __thiscall UpdateMeshData(TextureShader * this, Mesh * param_1, Engine * param_2)

## decompile
```c

/* AbyssEngine::TextureShader::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) */

void __thiscall
AbyssEngine::TextureShader::UpdateMeshData(TextureShader *this,Mesh *param_1,Engine *param_2)

{
  TextureShader *pTVar1;
  TextureShader *pTVar2;
  byte bVar3;
  float *pfVar4;
  undefined4 uVar5;
  int iVar6;
  byte *pbVar7;
  undefined4 uVar8;
  
  pbVar7 = *(byte **)(DAT_000a1254 + 0xa110e);
  bVar3 = *pbVar7;
  glUniformMatrix4fv(*(undefined4 *)(this + (uint)bVar3 * 4 + 0x34),1,0,param_2 + 0x104);
  if (-1 < *(int *)(this + (uint)bVar3 * 4 + 0x4c)) {
    glUniformMatrix4fv(*(int *)(this + (uint)bVar3 * 4 + 0x4c),1,0,param_2 + 0x1c4);
  }
  if (-1 < *(int *)(this + (uint)bVar3 * 4 + 0x84)) {
    glUniform1f(*(int *)(this + (uint)bVar3 * 4 + 0x84),1.0 - *(float *)(param_1 + 0x1c));
  }
  if (this[9] != (TextureShader)0x0) {
    glUniform4fv(*(undefined4 *)(this + (uint)bVar3 * 4 + 0x3c),1,param_2 + 0xd0);
    if (-1 < *(int *)(this + (uint)bVar3 * 4 + 0x54)) {
      glUniform1i(*(int *)(this + (uint)bVar3 * 4 + 0x54),param_1[0x85]);
    }
    iVar6 = *(int *)(this + (uint)bVar3 * 4 + 0x5c);
    if (-1 < iVar6) {
      pfVar4 = AEMath::Vector::operator_cast_to_float_((Vector *)(param_2 + 0x3f0));
      glUniform3fv(iVar6,1,pfVar4);
    }
    if (-1 < *(int *)(this + (uint)bVar3 * 4 + 0x6c)) {
      glUniform1f(*(int *)(this + (uint)bVar3 * 4 + 0x6c),*(undefined4 *)(param_2 + 1000));
    }
    if (-1 < *(int *)(this + (uint)bVar3 * 4 + 100)) {
      glUniform1f(*(int *)(this + (uint)bVar3 * 4 + 100),*(undefined4 *)(param_2 + 0x3ec));
    }
    if (-1 < *(int *)(this + (uint)bVar3 * 4 + 0x74)) {
      glUniform1i(*(int *)(this + (uint)bVar3 * 4 + 0x74),*pbVar7);
    }
    if (-1 < *(int *)(this + (uint)bVar3 * 4 + 0x7c)) {
      glUniform3f(*(int *)(this + (uint)bVar3 * 4 + 0x7c),*(undefined4 *)(param_2 + 0x34c),
                  *(undefined4 *)(param_2 + 0x350),*(undefined4 *)(param_2 + 0x354));
    }
    this[9] = (TextureShader)0x0;
  }
  if ((int)((uint)(byte)*param_1 << 0x1e) < 0) {
    pTVar1 = this + (uint)bVar3 * 4 + 0x24;
    glEnableVertexAttribArray(*(undefined4 *)pTVar1);
    pTVar2 = this + (uint)bVar3 * 4 + 0x2c;
    glEnableVertexAttribArray(*(undefined4 *)pTVar2);
    if (param_1[0x5c] == (Mesh)0x0) {
      glVertexAttribPointer(*(undefined4 *)pTVar1,3,0x1406,0,0,*(undefined4 *)(param_1 + 4));
      uVar5 = *(undefined4 *)pTVar2;
      uVar8 = *(undefined4 *)(param_1 + 8);
    }
    else {
      glBindBuffer(0x8892,*(undefined4 *)(param_1 + 0x60));
      glVertexAttribPointer(*(undefined4 *)pTVar1,3,0x1406,0,0,0);
      glBindBuffer(0x8892,*(undefined4 *)(param_1 + 0x68));
      uVar5 = *(undefined4 *)pTVar2;
      uVar8 = 0;
    }
    glVertexAttribPointer(uVar5,2,0x1406,0,0,uVar8);
  }
  return;
}

```

## target disasm
```
  000a10f8: push {r4,r5,r6,r7,lr}
  000a10fa: add r7,sp,#0xc
  000a10fc: push {r6,r7,r8,r9,r10}
  000a1100: mov r9,r0
  000a1102: ldr r0,[0x000a1254]
  000a1104: add.w r3,r2,#0x104
  000a1108: mov r6,r2
  000a110a: add r0,pc
  000a110c: mov r8,r1
  000a110e: movs r1,#0x1
  000a1110: movs r2,#0x0
  000a1112: ldr.w r10,[r0,#0x0]
  000a1116: ldrb.w r0,[r10,#0x0]
  000a111a: add.w r5,r9,r0, lsl #0x2
  000a111e: ldr r0,[r5,#0x34]
  000a1120: blx 0x00070990
  000a1124: ldr r0,[r5,#0x4c]
  000a1126: cmp r0,#0x0
  000a1128: blt 0x000a1136
  000a112a: add.w r3,r6,#0x1c4
  000a112e: movs r1,#0x1
  000a1130: movs r2,#0x0
  000a1132: blx 0x00070990
  000a1136: ldr.w r0,[r5,#0x84]
  000a113a: cmp r0,#0x0
  000a113c: blt 0x000a1152
  000a113e: vmov.f32 s0,0x3f800000
  000a1142: vldr.32 s2,[r8,#0x1c]
  000a1146: vsub.f32 s0,s0,s2
  000a114a: vmov r1,s0
  000a114e: blx 0x00070978
  000a1152: ldrb.w r0,[r9,#0x9]
  000a1156: cbz r0,0x000a11ca
  000a1158: ldr r0,[r5,#0x3c]
  000a115a: add.w r2,r6,#0xd0
  000a115e: movs r1,#0x1
  000a1160: blx 0x00070984
  000a1164: ldr r0,[r5,#0x54]
  000a1166: cmp r0,#0x0
  000a1168: blt 0x000a1172
  000a116a: ldrb.w r1,[r8,#0x85]
  000a116e: blx 0x0007096c
  000a1172: ldr r4,[r5,#0x5c]
  000a1174: cmp r4,#0x0
  000a1176: blt 0x000a118a
  000a1178: add.w r0,r6,#0x3f0
  000a117c: blx 0x00070b28
  000a1180: mov r2,r0
  000a1182: mov r0,r4
  000a1184: movs r1,#0x1
  000a1186: blx 0x00070a50
  000a118a: ldr r0,[r5,#0x6c]
  000a118c: cmp r0,#0x0
  000a118e: blt 0x000a1198
  000a1190: ldr.w r1,[r6,#0x3e8]
  000a1194: blx 0x00070978
  000a1198: ldr r0,[r5,#0x64]
  000a119a: cmp r0,#0x0
  000a119c: blt 0x000a11a6
  000a119e: ldr.w r1,[r6,#0x3ec]
  000a11a2: blx 0x00070978
  000a11a6: ldr r0,[r5,#0x74]
  000a11a8: cmp r0,#0x0
  000a11aa: blt 0x000a11b4
  000a11ac: ldrb.w r1,[r10,#0x0]
  000a11b0: blx 0x0007096c
  000a11b4: ldr r0,[r5,#0x7c]
  000a11b6: cmp r0,#0x0
  000a11b8: blt 0x000a11c4
  000a11ba: add.w r3,r6,#0x34c
  000a11be: ldmia r3,{r1,r2,r3}
  000a11c0: blx 0x000709b4
  000a11c4: movs r0,#0x0
  000a11c6: strb.w r0,[r9,#0x9]
  000a11ca: ldrb.w r0,[r8,#0x0]
  000a11ce: lsls r0,r0,#0x1e
  000a11d0: bpl 0x000a124c
  000a11d2: mov r4,r5
  000a11d4: ldr.w r0,[r4,#0x24]!
  000a11d8: blx 0x00070a5c
  000a11dc: ldr.w r0,[r5,#0x2c]!
  000a11e0: blx 0x00070a5c
  000a11e4: ldrb.w r0,[r8,#0x5c]
  000a11e8: cbz r0,0x000a121e
  000a11ea: ldr.w r1,[r8,#0x60]
  000a11ee: movw r0,#0x8892
  000a11f2: blx 0x0006ed04
  000a11f6: ldr r0,[r4,#0x0]
  000a11f8: movs r4,#0x0
  000a11fa: movs r1,#0x3
  000a11fc: movw r2,#0x1406
  000a1200: movs r3,#0x0
  000a1202: strd r4,r4,[sp,#0x0]
  000a1206: blx 0x000709c0
  000a120a: ldr.w r1,[r8,#0x68]
  000a120e: movw r0,#0x8892
  000a1212: blx 0x0006ed04
  000a1216: ldr r0,[r5,#0x0]
  000a1218: strd r4,r4,[sp,#0x0]
  000a121c: b 0x000a1240
  000a121e: ldr.w r1,[r8,#0x4]
  000a1222: movw r2,#0x1406
  000a1226: ldr r0,[r4,#0x0]
  000a1228: movs r4,#0x0
  000a122a: movs r3,#0x0
  000a122c: strd r4,r1,[sp,#0x0]
  000a1230: movs r1,#0x3
  000a1232: blx 0x000709c0
  000a1236: ldr r0,[r5,#0x0]
  000a1238: ldr.w r1,[r8,#0x8]
  000a123c: strd r4,r1,[sp,#0x0]
  000a1240: movs r1,#0x2
  000a1242: movw r2,#0x1406
  000a1246: movs r3,#0x0
  000a1248: blx 0x000709c0
  000a124c: pop.w {r2,r3,r8,r9,r10}
  000a1250: pop {r4,r5,r6,r7,pc}
```
