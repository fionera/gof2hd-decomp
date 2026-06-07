# GlowShader::UpdateMeshData
elf 0x92070 body 186
Sig: undefined __thiscall UpdateMeshData(GlowShader * this, Mesh * param_1, Engine * param_2)

## decompile
```c

/* AbyssEngine::GlowShader::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) */

void __thiscall
AbyssEngine::GlowShader::UpdateMeshData(GlowShader *this,Mesh *param_1,Engine *param_2)

{
  int iVar1;
  undefined4 uVar2;
  
  if (-1 < *(int *)(this + 0x28)) {
    glUniformMatrix4fv(*(int *)(this + 0x28),1,0,param_2 + 0x104);
  }
  if (-1 < *(int *)(this + 0x2c)) {
    glUniformMatrix3fv(*(int *)(this + 0x2c),1,0,param_2 + 0x204);
  }
  if (-1 < *(int *)(this + 0x20)) {
    glEnableVertexAttribArray();
  }
  if (-1 < *(int *)(this + 0x24)) {
    glEnableVertexAttribArray();
  }
  if (param_1[0x5c] == (Mesh)0x0) {
    if (-1 < *(int *)(this + 0x20)) {
      glVertexAttribPointer(*(int *)(this + 0x20),3,0x1406,0,0,*(undefined4 *)(param_1 + 4));
    }
    iVar1 = *(int *)(this + 0x24);
    if (iVar1 < 0) {
      return;
    }
    uVar2 = *(undefined4 *)(param_1 + 8);
  }
  else {
    glBindBuffer(0x8892,*(undefined4 *)(param_1 + 0x60));
    glVertexAttribPointer(*(undefined4 *)(this + 0x20),3,0x1406,0,0,0);
    glBindBuffer(0x8892,*(undefined4 *)(param_1 + 0x68));
    iVar1 = *(int *)(this + 0x24);
    uVar2 = 0;
  }
  glVertexAttribPointer(iVar1,2,0x1406,0,0,uVar2);
  return;
}

```

## target disasm
```
  000a2070: push {r4,r5,r6,r7,lr}
  000a2072: add r7,sp,#0xc
  000a2074: push.w r11
  000a2078: sub sp,#0x8
  000a207a: mov r4,r0
  000a207c: ldr r0,[r0,#0x28]
  000a207e: mov r6,r2
  000a2080: mov r5,r1
  000a2082: cmp r0,#0x0
  000a2084: blt 0x000a2092
  000a2086: add.w r3,r6,#0x104
  000a208a: movs r1,#0x1
  000a208c: movs r2,#0x0
  000a208e: blx 0x00070990
  000a2092: ldr r0,[r4,#0x2c]
  000a2094: cmp r0,#0x0
  000a2096: blt 0x000a20a4
  000a2098: add.w r3,r6,#0x204
  000a209c: movs r1,#0x1
  000a209e: movs r2,#0x0
  000a20a0: blx 0x0007099c
  000a20a4: ldr r0,[r4,#0x20]
  000a20a6: cmp r0,#0x0
  000a20a8: blt 0x000a20ae
  000a20aa: blx 0x00070a5c
  000a20ae: ldr r0,[r4,#0x24]
  000a20b0: cmp r0,#0x0
  000a20b2: blt 0x000a20b8
  000a20b4: blx 0x00070a5c
  000a20b8: ldrb.w r0,[r5,#0x5c]
  000a20bc: cbz r0,0x000a20ee
  000a20be: ldr r1,[r5,#0x60]
  000a20c0: movw r0,#0x8892
  000a20c4: blx 0x0006ed04
  000a20c8: ldr r0,[r4,#0x20]
  000a20ca: movs r6,#0x0
  000a20cc: movs r1,#0x3
  000a20ce: movw r2,#0x1406
  000a20d2: movs r3,#0x0
  000a20d4: strd r6,r6,[sp,#0x0]
  000a20d8: blx 0x000709c0
  000a20dc: ldr r1,[r5,#0x68]
  000a20de: movw r0,#0x8892
  000a20e2: blx 0x0006ed04
  000a20e6: ldr r0,[r4,#0x24]
  000a20e8: strd r6,r6,[sp,#0x0]
  000a20ec: b 0x000a2116
  000a20ee: ldr r0,[r4,#0x20]
  000a20f0: cmp r0,#0x0
  000a20f2: blt 0x000a2108
  000a20f4: ldr r1,[r5,#0x4]
  000a20f6: movs r2,#0x0
  000a20f8: movs r3,#0x0
  000a20fa: strd r2,r1,[sp,#0x0]
  000a20fe: movs r1,#0x3
  000a2100: movw r2,#0x1406
  000a2104: blx 0x000709c0
  000a2108: ldr r0,[r4,#0x24]
  000a210a: cmp r0,#0x0
  000a210c: blt 0x000a2122
  000a210e: ldr r1,[r5,#0x8]
  000a2110: movs r2,#0x0
  000a2112: strd r2,r1,[sp,#0x0]
  000a2116: movs r1,#0x2
  000a2118: movw r2,#0x1406
  000a211c: movs r3,#0x0
  000a211e: blx 0x000709c0
  000a2122: add sp,#0x8
  000a2124: pop.w r11
  000a2128: pop {r4,r5,r6,r7,pc}
```
