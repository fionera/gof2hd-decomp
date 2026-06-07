# TextureAlphaTestShader::UpdateMeshData
elf 0x892a4 body 290
Sig: undefined __thiscall UpdateMeshData(TextureAlphaTestShader * this, Mesh * param_1, Engine * param_2)

## decompile
```c

/* AbyssEngine::TextureAlphaTestShader::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) */

void __thiscall
AbyssEngine::TextureAlphaTestShader::UpdateMeshData
          (TextureAlphaTestShader *this,Mesh *param_1,Engine *param_2)

{
  TextureAlphaTestShader *pTVar1;
  TextureAlphaTestShader *pTVar2;
  byte bVar3;
  float *pfVar4;
  undefined4 uVar5;
  int iVar6;
  byte *pbVar7;
  undefined4 uVar8;
  
  pbVar7 = *(byte **)(DAT_000993c8 + 0x992ba);
  bVar3 = *pbVar7;
  glUniformMatrix4fv(*(undefined4 *)(this + (uint)bVar3 * 4 + 0x34),1,0,param_2 + 0x104);
  if (this[9] != (TextureAlphaTestShader)0x0) {
    if (-1 < *(int *)(this + (uint)bVar3 * 4 + 0x3c)) {
      glUniform4fv(*(int *)(this + (uint)bVar3 * 4 + 0x3c),1,param_2 + 0xd0);
    }
    iVar6 = *(int *)(this + (uint)bVar3 * 4 + 0x4c);
    if (-1 < iVar6) {
      pfVar4 = AEMath::Vector::operator_cast_to_float_((Vector *)(param_2 + 0x3f0));
      glUniform3fv(iVar6,1,pfVar4);
    }
    if (-1 < *(int *)(this + (uint)bVar3 * 4 + 0x5c)) {
      glUniform1f(*(int *)(this + (uint)bVar3 * 4 + 0x5c),*(undefined4 *)(param_2 + 1000));
    }
    if (-1 < *(int *)(this + (uint)bVar3 * 4 + 0x54)) {
      glUniform1f(*(int *)(this + (uint)bVar3 * 4 + 0x54),*(undefined4 *)(param_2 + 0x3ec));
    }
    if (-1 < *(int *)(this + (uint)bVar3 * 4 + 100)) {
      glUniform1i(*(int *)(this + (uint)bVar3 * 4 + 100),*pbVar7);
    }
    if (-1 < *(int *)(this + (uint)bVar3 * 4 + 0x6c)) {
      glUniform3f(*(int *)(this + (uint)bVar3 * 4 + 0x6c),*(undefined4 *)(param_2 + 0x34c),
                  *(undefined4 *)(param_2 + 0x350),*(undefined4 *)(param_2 + 0x354));
    }
    this[9] = (TextureAlphaTestShader)0x0;
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
  000992a4: push {r4,r5,r6,r7,lr}
  000992a6: add r7,sp,#0xc
  000992a8: push {r6,r7,r8,r9,r10}
  000992ac: mov r9,r0
  000992ae: ldr r0,[0x000993c8]
  000992b0: add.w r3,r2,#0x104
  000992b4: mov r6,r2
  000992b6: add r0,pc
  000992b8: mov r8,r1
  000992ba: movs r1,#0x1
  000992bc: movs r2,#0x0
  000992be: ldr.w r10,[r0,#0x0]
  000992c2: ldrb.w r0,[r10,#0x0]
  000992c6: add.w r5,r9,r0, lsl #0x2
  000992ca: ldr r0,[r5,#0x34]
  000992cc: blx 0x00070990
  000992d0: ldrb.w r0,[r9,#0x9]
  000992d4: cbz r0,0x0009933e
  000992d6: ldr r0,[r5,#0x3c]
  000992d8: cmp r0,#0x0
  000992da: blt 0x000992e6
  000992dc: add.w r2,r6,#0xd0
  000992e0: movs r1,#0x1
  000992e2: blx 0x00070984
  000992e6: ldr r4,[r5,#0x4c]
  000992e8: cmp r4,#0x0
  000992ea: blt 0x000992fe
  000992ec: add.w r0,r6,#0x3f0
  000992f0: blx 0x00070b28
  000992f4: mov r2,r0
  000992f6: mov r0,r4
  000992f8: movs r1,#0x1
  000992fa: blx 0x00070a50
  000992fe: ldr r0,[r5,#0x5c]
  00099300: cmp r0,#0x0
  00099302: blt 0x0009930c
  00099304: ldr.w r1,[r6,#0x3e8]
  00099308: blx 0x00070978
  0009930c: ldr r0,[r5,#0x54]
  0009930e: cmp r0,#0x0
  00099310: blt 0x0009931a
  00099312: ldr.w r1,[r6,#0x3ec]
  00099316: blx 0x00070978
  0009931a: ldr r0,[r5,#0x64]
  0009931c: cmp r0,#0x0
  0009931e: blt 0x00099328
  00099320: ldrb.w r1,[r10,#0x0]
  00099324: blx 0x0007096c
  00099328: ldr r0,[r5,#0x6c]
  0009932a: cmp r0,#0x0
  0009932c: blt 0x00099338
  0009932e: add.w r3,r6,#0x34c
  00099332: ldmia r3,{r1,r2,r3}
  00099334: blx 0x000709b4
  00099338: movs r0,#0x0
  0009933a: strb.w r0,[r9,#0x9]
  0009933e: ldrb.w r0,[r8,#0x0]
  00099342: lsls r0,r0,#0x1e
  00099344: bpl 0x000993c0
  00099346: mov r4,r5
  00099348: ldr.w r0,[r4,#0x24]!
  0009934c: blx 0x00070a5c
  00099350: ldr.w r0,[r5,#0x2c]!
  00099354: blx 0x00070a5c
  00099358: ldrb.w r0,[r8,#0x5c]
  0009935c: cbz r0,0x00099392
  0009935e: ldr.w r1,[r8,#0x60]
  00099362: movw r0,#0x8892
  00099366: blx 0x0006ed04
  0009936a: ldr r0,[r4,#0x0]
  0009936c: movs r4,#0x0
  0009936e: movs r1,#0x3
  00099370: movw r2,#0x1406
  00099374: movs r3,#0x0
  00099376: strd r4,r4,[sp,#0x0]
  0009937a: blx 0x000709c0
  0009937e: ldr.w r1,[r8,#0x68]
  00099382: movw r0,#0x8892
  00099386: blx 0x0006ed04
  0009938a: ldr r0,[r5,#0x0]
  0009938c: strd r4,r4,[sp,#0x0]
  00099390: b 0x000993b4
  00099392: ldr.w r1,[r8,#0x4]
  00099396: movw r2,#0x1406
  0009939a: ldr r0,[r4,#0x0]
  0009939c: movs r4,#0x0
  0009939e: movs r3,#0x0
  000993a0: strd r4,r1,[sp,#0x0]
  000993a4: movs r1,#0x3
  000993a6: blx 0x000709c0
  000993aa: ldr r0,[r5,#0x0]
  000993ac: ldr.w r1,[r8,#0x8]
  000993b0: strd r4,r1,[sp,#0x0]
  000993b4: movs r1,#0x2
  000993b6: movw r2,#0x1406
  000993ba: movs r3,#0x0
  000993bc: blx 0x000709c0
  000993c0: pop.w {r2,r3,r8,r9,r10}
  000993c4: pop {r4,r5,r6,r7,pc}
```
