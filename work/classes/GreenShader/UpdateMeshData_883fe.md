# GreenShader::UpdateMeshData
elf 0x883fe body 128
Sig: undefined __stdcall UpdateMeshData(Mesh * param_1, Engine * param_2)

## decompile
```c

/* AbyssEngine::GreenShader::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) */

void AbyssEngine::GreenShader::UpdateMeshData(Mesh *param_1,Engine *param_2)

{
  int iVar1;
  undefined4 uVar2;
  int in_r2;
  
  if (-1 < *(int *)(param_1 + 0x34)) {
    glUniformMatrix4fv(*(int *)(param_1 + 0x34),1,0,in_r2 + 0x104);
  }
  if (-1 < *(int *)(param_1 + 0x20)) {
    glEnableVertexAttribArray();
  }
  if (-1 < *(int *)(param_1 + 0x24)) {
    glEnableVertexAttribArray();
  }
  if (-1 < *(int *)(param_1 + 0x28)) {
    glEnableVertexAttribArray();
  }
  if (-1 < *(int *)(param_1 + 0x2c)) {
    glEnableVertexAttribArray();
  }
  if (-1 < *(int *)(param_1 + 0x30)) {
    glEnableVertexAttribArray();
  }
  if (param_2[0x5c] == (Engine)0x0) {
    iVar1 = *(int *)(param_1 + 0x20);
    if (iVar1 < 0) {
      return;
    }
    uVar2 = *(undefined4 *)(param_2 + 4);
  }
  else {
    glBindBuffer(0x8892,*(undefined4 *)(param_2 + 0x60));
    uVar2 = 0;
    iVar1 = *(int *)(param_1 + 0x20);
  }
  glVertexAttribPointer(iVar1,3,0x1406,0,0,uVar2);
  return;
}

```

## target disasm
```
  000983fe: push {r2,r3,r4,r5,r7,lr}
  00098400: add r7,sp,#0x10
  00098402: mov r4,r0
  00098404: ldr r0,[r0,#0x34]
  00098406: mov r5,r1
  00098408: cmp r0,#0x0
  0009840a: blt 0x00098418
  0009840c: add.w r3,r2,#0x104
  00098410: movs r1,#0x1
  00098412: movs r2,#0x0
  00098414: blx 0x00070990
  00098418: ldr r0,[r4,#0x20]
  0009841a: cmp r0,#0x0
  0009841c: blt 0x00098422
  0009841e: blx 0x00070a5c
  00098422: ldr r0,[r4,#0x24]
  00098424: cmp r0,#0x0
  00098426: blt 0x0009842c
  00098428: blx 0x00070a5c
  0009842c: ldr r0,[r4,#0x28]
  0009842e: cmp r0,#0x0
  00098430: blt 0x00098436
  00098432: blx 0x00070a5c
  00098436: ldr r0,[r4,#0x2c]
  00098438: cmp r0,#0x0
  0009843a: blt 0x00098440
  0009843c: blx 0x00070a5c
  00098440: ldr r0,[r4,#0x30]
  00098442: cmp r0,#0x0
  00098444: blt 0x0009844a
  00098446: blx 0x00070a5c
  0009844a: ldrb.w r0,[r5,#0x5c]
  0009844e: cbz r0,0x00098462
  00098450: ldr r1,[r5,#0x60]
  00098452: movw r0,#0x8892
  00098456: blx 0x0006ed04
  0009845a: movs r1,#0x0
  0009845c: ldr r0,[r4,#0x20]
  0009845e: str r1,[sp,#0x0]
  00098460: b 0x0009846e
  00098462: ldr r0,[r4,#0x20]
  00098464: cmp r0,#0x0
  00098466: blt 0x0009847c
  00098468: ldr r1,[r5,#0x4]
  0009846a: movs r2,#0x0
  0009846c: str r2,[sp,#0x0]
  0009846e: str r1,[sp,#0x4]
  00098470: movs r1,#0x3
  00098472: movw r2,#0x1406
  00098476: movs r3,#0x0
  00098478: blx 0x000709c0
  0009847c: pop {r2,r3,r4,r5,r7,pc}
```
