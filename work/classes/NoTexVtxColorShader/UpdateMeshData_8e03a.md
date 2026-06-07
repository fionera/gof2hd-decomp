# NoTexVtxColorShader::UpdateMeshData
elf 0x8e03a body 132
Sig: undefined __thiscall UpdateMeshData(NoTexVtxColorShader * this, Mesh * param_1, Engine * param_2)

## decompile
```c

/* AbyssEngine::NoTexVtxColorShader::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) */

void __thiscall
AbyssEngine::NoTexVtxColorShader::UpdateMeshData
          (NoTexVtxColorShader *this,Mesh *param_1,Engine *param_2)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  
  glUniformMatrix4fv(*(undefined4 *)(this + 0x28),1,0,param_2 + 0x104);
  if (this[9] != (NoTexVtxColorShader)0x0) {
    glUniform4fv(*(undefined4 *)(this + 0x2c),1,param_2 + 0xd0);
    this[9] = (NoTexVtxColorShader)0x0;
  }
  glEnableVertexAttribArray(*(undefined4 *)(this + 0x20));
  glEnableVertexAttribArray(*(undefined4 *)(this + 0x24));
  uVar1 = *(undefined4 *)(this + 0x20);
  if (param_1 == (Mesh *)0x0) {
    uVar2 = *(undefined4 *)(param_2 + 0x348);
    uVar3 = 2;
  }
  else {
    glVertexAttribPointer(uVar1,3,0x1406,0,0,*(undefined4 *)(param_1 + 4));
    uVar2 = *(undefined4 *)(param_1 + 0xc);
    uVar1 = *(undefined4 *)(this + 0x24);
    uVar3 = 4;
  }
  glVertexAttribPointer(uVar1,uVar3,0x1406,0,0,uVar2);
  return;
}

```

## target disasm
```
  0009e03a: push {r4,r5,r6,r7,lr}
  0009e03c: add r7,sp,#0xc
  0009e03e: push.w r8
  0009e042: sub sp,#0x8
  0009e044: mov r5,r0
  0009e046: ldr r0,[r0,#0x28]
  0009e048: add.w r3,r2,#0x104
  0009e04c: mov r6,r2
  0009e04e: mov r4,r1
  0009e050: movs r1,#0x1
  0009e052: movs r2,#0x0
  0009e054: mov.w r8,#0x0
  0009e058: blx 0x00070990
  0009e05c: ldrb r0,[r5,#0x9]
  0009e05e: cbz r0,0x0009e070
  0009e060: ldr r0,[r5,#0x2c]
  0009e062: add.w r2,r6,#0xd0
  0009e066: movs r1,#0x1
  0009e068: blx 0x00070984
  0009e06c: strb.w r8,[r5,#0x9]
  0009e070: ldr r0,[r5,#0x20]
  0009e072: blx 0x00070a5c
  0009e076: ldr r0,[r5,#0x24]
  0009e078: blx 0x00070a5c
  0009e07c: ldr r0,[r5,#0x20]
  0009e07e: cbz r4,0x0009e0a0
  0009e080: ldr r1,[r4,#0x4]
  0009e082: movs r6,#0x0
  0009e084: movw r2,#0x1406
  0009e088: movs r3,#0x0
  0009e08a: strd r6,r1,[sp,#0x0]
  0009e08e: movs r1,#0x3
  0009e090: blx 0x000709c0
  0009e094: ldr r1,[r4,#0xc]
  0009e096: ldr r0,[r5,#0x24]
  0009e098: strd r6,r1,[sp,#0x0]
  0009e09c: movs r1,#0x4
  0009e09e: b 0x0009e0ac
  0009e0a0: ldr.w r1,[r6,#0x348]
  0009e0a4: movs r2,#0x0
  0009e0a6: strd r2,r1,[sp,#0x0]
  0009e0aa: movs r1,#0x2
  0009e0ac: movw r2,#0x1406
  0009e0b0: movs r3,#0x0
  0009e0b2: blx 0x000709c0
  0009e0b6: add sp,#0x8
  0009e0b8: pop.w r8
  0009e0bc: pop {r4,r5,r6,r7,pc}
```
