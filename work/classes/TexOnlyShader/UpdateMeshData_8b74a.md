# TexOnlyShader::UpdateMeshData
elf 0x8b74a body 152
Sig: undefined __thiscall UpdateMeshData(TexOnlyShader * this, Mesh * param_1, Engine * param_2)

## decompile
```c

/* AbyssEngine::TexOnlyShader::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) */

void __thiscall
AbyssEngine::TexOnlyShader::UpdateMeshData(TexOnlyShader *this,Mesh *param_1,Engine *param_2)

{
  undefined4 uVar1;
  undefined4 uVar2;
  
  glUniformMatrix4fv(*(undefined4 *)(this + 0x28),1,0,param_2 + 0x104);
  if (this[9] != (TexOnlyShader)0x0) {
    this[9] = (TexOnlyShader)0x0;
  }
  glEnableVertexAttribArray(*(undefined4 *)(this + 0x20));
  glEnableVertexAttribArray(*(undefined4 *)(this + 0x24));
  if (param_1[0x5c] == (Mesh)0x0) {
    glVertexAttribPointer(*(undefined4 *)(this + 0x20),3,0x1406,0,0,*(undefined4 *)(param_1 + 4));
    uVar1 = *(undefined4 *)(this + 0x24);
    uVar2 = *(undefined4 *)(param_1 + 8);
  }
  else {
    glBindBuffer(0x8892,*(undefined4 *)(param_1 + 0x60));
    glVertexAttribPointer(*(undefined4 *)(this + 0x20),3,0x1406,0,0,0);
    glBindBuffer(0x8892,*(undefined4 *)(param_1 + 0x68));
    uVar1 = *(undefined4 *)(this + 0x24);
    uVar2 = 0;
  }
  glVertexAttribPointer(uVar1,2,0x1406,0,0,uVar2);
  return;
}

```

## target disasm
```
  0009b74a: push {r4,r5,r6,r7,lr}
  0009b74c: add r7,sp,#0xc
  0009b74e: push.w r11
  0009b752: sub sp,#0x8
  0009b754: mov r4,r0
  0009b756: ldr r0,[r0,#0x28]
  0009b758: add.w r3,r2,#0x104
  0009b75c: mov r5,r1
  0009b75e: movs r1,#0x1
  0009b760: movs r2,#0x0
  0009b762: movs r6,#0x0
  0009b764: blx 0x00070990
  0009b768: ldrb r0,[r4,#0x9]
  0009b76a: cbz r0,0x0009b76e
  0009b76c: strb r6,[r4,#0x9]
  0009b76e: ldr r0,[r4,#0x20]
  0009b770: blx 0x00070a5c
  0009b774: ldr r0,[r4,#0x24]
  0009b776: blx 0x00070a5c
  0009b77a: ldrb.w r0,[r5,#0x5c]
  0009b77e: cbz r0,0x0009b7b0
  0009b780: ldr r1,[r5,#0x60]
  0009b782: movw r0,#0x8892
  0009b786: blx 0x0006ed04
  0009b78a: ldr r0,[r4,#0x20]
  0009b78c: movs r6,#0x0
  0009b78e: movs r1,#0x3
  0009b790: movw r2,#0x1406
  0009b794: movs r3,#0x0
  0009b796: strd r6,r6,[sp,#0x0]
  0009b79a: blx 0x000709c0
  0009b79e: ldr r1,[r5,#0x68]
  0009b7a0: movw r0,#0x8892
  0009b7a4: blx 0x0006ed04
  0009b7a8: ldr r0,[r4,#0x24]
  0009b7aa: strd r6,r6,[sp,#0x0]
  0009b7ae: b 0x0009b7ce
  0009b7b0: ldr r1,[r5,#0x4]
  0009b7b2: movs r6,#0x0
  0009b7b4: ldr r0,[r4,#0x20]
  0009b7b6: movw r2,#0x1406
  0009b7ba: movs r3,#0x0
  0009b7bc: strd r6,r1,[sp,#0x0]
  0009b7c0: movs r1,#0x3
  0009b7c2: blx 0x000709c0
  0009b7c6: ldr r0,[r4,#0x24]
  0009b7c8: ldr r1,[r5,#0x8]
  0009b7ca: strd r6,r1,[sp,#0x0]
  0009b7ce: movs r1,#0x2
  0009b7d0: movw r2,#0x1406
  0009b7d4: movs r3,#0x0
  0009b7d6: blx 0x000709c0
  0009b7da: add sp,#0x8
  0009b7dc: pop.w r11
  0009b7e0: pop {r4,r5,r6,r7,pc}
```
