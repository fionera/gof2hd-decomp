# DrawFBOShader::UpdateMeshData
elf 0x88788 body 152
Sig: undefined __thiscall UpdateMeshData(DrawFBOShader * this, Mesh * param_1, Engine * param_2)

## decompile
```c

/* AbyssEngine::DrawFBOShader::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) */

void __thiscall
AbyssEngine::DrawFBOShader::UpdateMeshData(DrawFBOShader *this,Mesh *param_1,Engine *param_2)

{
  undefined4 uVar1;
  undefined4 uVar2;
  
  glUniformMatrix4fv(*(undefined4 *)(this + 0x4c),1,0,param_2 + 0x104);
  if (this[9] != (DrawFBOShader)0x0) {
    this[9] = (DrawFBOShader)0x0;
  }
  glEnableVertexAttribArray(*(undefined4 *)(this + 0x48));
  glEnableVertexAttribArray(*(undefined4 *)(this + 0x50));
  if (param_1[0x5c] == (Mesh)0x0) {
    glVertexAttribPointer(*(undefined4 *)(this + 0x48),3,0x1406,0,0,*(undefined4 *)(param_1 + 4));
    uVar1 = *(undefined4 *)(this + 0x50);
    uVar2 = *(undefined4 *)(param_1 + 8);
  }
  else {
    glBindBuffer(0x8892,*(undefined4 *)(param_1 + 0x60));
    glVertexAttribPointer(*(undefined4 *)(this + 0x48),3,0x1406,0,0,0);
    glBindBuffer(0x8892,*(undefined4 *)(param_1 + 0x68));
    uVar1 = *(undefined4 *)(this + 0x50);
    uVar2 = 0;
  }
  glVertexAttribPointer(uVar1,2,0x1406,0,0,uVar2);
  return;
}

```

## target disasm
```
  00098788: push {r4,r5,r6,r7,lr}
  0009878a: add r7,sp,#0xc
  0009878c: push.w r11
  00098790: sub sp,#0x8
  00098792: mov r4,r0
  00098794: ldr r0,[r0,#0x4c]
  00098796: add.w r3,r2,#0x104
  0009879a: mov r5,r1
  0009879c: movs r1,#0x1
  0009879e: movs r2,#0x0
  000987a0: movs r6,#0x0
  000987a2: blx 0x00070990
  000987a6: ldrb r0,[r4,#0x9]
  000987a8: cbz r0,0x000987ac
  000987aa: strb r6,[r4,#0x9]
  000987ac: ldr r0,[r4,#0x48]
  000987ae: blx 0x00070a5c
  000987b2: ldr r0,[r4,#0x50]
  000987b4: blx 0x00070a5c
  000987b8: ldrb.w r0,[r5,#0x5c]
  000987bc: cbz r0,0x000987ee
  000987be: ldr r1,[r5,#0x60]
  000987c0: movw r0,#0x8892
  000987c4: blx 0x0006ed04
  000987c8: ldr r0,[r4,#0x48]
  000987ca: movs r6,#0x0
  000987cc: movs r1,#0x3
  000987ce: movw r2,#0x1406
  000987d2: movs r3,#0x0
  000987d4: strd r6,r6,[sp,#0x0]
  000987d8: blx 0x000709c0
  000987dc: ldr r1,[r5,#0x68]
  000987de: movw r0,#0x8892
  000987e2: blx 0x0006ed04
  000987e6: ldr r0,[r4,#0x50]
  000987e8: strd r6,r6,[sp,#0x0]
  000987ec: b 0x0009880c
  000987ee: ldr r1,[r5,#0x4]
  000987f0: movs r6,#0x0
  000987f2: ldr r0,[r4,#0x48]
  000987f4: movw r2,#0x1406
  000987f8: movs r3,#0x0
  000987fa: strd r6,r1,[sp,#0x0]
  000987fe: movs r1,#0x3
  00098800: blx 0x000709c0
  00098804: ldr r0,[r4,#0x50]
  00098806: ldr r1,[r5,#0x8]
  00098808: strd r6,r1,[sp,#0x0]
  0009880c: movs r1,#0x2
  0009880e: movw r2,#0x1406
  00098812: movs r3,#0x0
  00098814: blx 0x000709c0
  00098818: add sp,#0x8
  0009881a: pop.w r11
  0009881e: pop {r4,r5,r6,r7,pc}
```
