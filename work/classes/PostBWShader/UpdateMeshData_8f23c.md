# PostBWShader::UpdateMeshData
elf 0x8f23c body 152
Sig: undefined __thiscall UpdateMeshData(PostBWShader * this, Mesh * param_1, Engine * param_2)

## decompile
```c

/* AbyssEngine::PostBWShader::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) */

void __thiscall
AbyssEngine::PostBWShader::UpdateMeshData(PostBWShader *this,Mesh *param_1,Engine *param_2)

{
  undefined4 uVar1;
  undefined4 uVar2;
  
  glUniformMatrix4fv(*(undefined4 *)(this + 0x24),1,0,param_2 + 0x104);
  if (this[9] != (PostBWShader)0x0) {
    this[9] = (PostBWShader)0x0;
  }
  glEnableVertexAttribArray(*(undefined4 *)(this + 0x20));
  glEnableVertexAttribArray(*(undefined4 *)(this + 0x28));
  if (param_1[0x5c] == (Mesh)0x0) {
    glVertexAttribPointer(*(undefined4 *)(this + 0x20),3,0x1406,0,0,*(undefined4 *)(param_1 + 4));
    uVar1 = *(undefined4 *)(this + 0x28);
    uVar2 = *(undefined4 *)(param_1 + 8);
  }
  else {
    glBindBuffer(0x8892,*(undefined4 *)(param_1 + 0x60));
    glVertexAttribPointer(*(undefined4 *)(this + 0x20),3,0x1406,0,0,0);
    glBindBuffer(0x8892,*(undefined4 *)(param_1 + 0x68));
    uVar1 = *(undefined4 *)(this + 0x28);
    uVar2 = 0;
  }
  glVertexAttribPointer(uVar1,2,0x1406,0,0,uVar2);
  return;
}

```

## target disasm
```
  0009f23c: push {r4,r5,r6,r7,lr}
  0009f23e: add r7,sp,#0xc
  0009f240: push.w r11
  0009f244: sub sp,#0x8
  0009f246: mov r4,r0
  0009f248: ldr r0,[r0,#0x24]
  0009f24a: add.w r3,r2,#0x104
  0009f24e: mov r5,r1
  0009f250: movs r1,#0x1
  0009f252: movs r2,#0x0
  0009f254: movs r6,#0x0
  0009f256: blx 0x00070990
  0009f25a: ldrb r0,[r4,#0x9]
  0009f25c: cbz r0,0x0009f260
  0009f25e: strb r6,[r4,#0x9]
  0009f260: ldr r0,[r4,#0x20]
  0009f262: blx 0x00070a5c
  0009f266: ldr r0,[r4,#0x28]
  0009f268: blx 0x00070a5c
  0009f26c: ldrb.w r0,[r5,#0x5c]
  0009f270: cbz r0,0x0009f2a2
  0009f272: ldr r1,[r5,#0x60]
  0009f274: movw r0,#0x8892
  0009f278: blx 0x0006ed04
  0009f27c: ldr r0,[r4,#0x20]
  0009f27e: movs r6,#0x0
  0009f280: movs r1,#0x3
  0009f282: movw r2,#0x1406
  0009f286: movs r3,#0x0
  0009f288: strd r6,r6,[sp,#0x0]
  0009f28c: blx 0x000709c0
  0009f290: ldr r1,[r5,#0x68]
  0009f292: movw r0,#0x8892
  0009f296: blx 0x0006ed04
  0009f29a: ldr r0,[r4,#0x28]
  0009f29c: strd r6,r6,[sp,#0x0]
  0009f2a0: b 0x0009f2c0
  0009f2a2: ldr r1,[r5,#0x4]
  0009f2a4: movs r6,#0x0
  0009f2a6: ldr r0,[r4,#0x20]
  0009f2a8: movw r2,#0x1406
  0009f2ac: movs r3,#0x0
  0009f2ae: strd r6,r1,[sp,#0x0]
  0009f2b2: movs r1,#0x3
  0009f2b4: blx 0x000709c0
  0009f2b8: ldr r0,[r4,#0x28]
  0009f2ba: ldr r1,[r5,#0x8]
  0009f2bc: strd r6,r1,[sp,#0x0]
  0009f2c0: movs r1,#0x2
  0009f2c2: movw r2,#0x1406
  0009f2c6: movs r3,#0x0
  0009f2c8: blx 0x000709c0
  0009f2cc: add sp,#0x8
  0009f2ce: pop.w r11
  0009f2d2: pop {r4,r5,r6,r7,pc}
```
