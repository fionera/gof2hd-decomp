# GenericShader2::UpdateMeshData
elf 0x8e2ba body 344
Sig: undefined __thiscall UpdateMeshData(GenericShader2 * this, Mesh * param_1, Engine * param_2)

## decompile
```c

/* AbyssEngine::GenericShader2::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) */

void __thiscall
AbyssEngine::GenericShader2::UpdateMeshData(GenericShader2 *this,Mesh *param_1,Engine *param_2)

{
  if (-1 < *(int *)(this + 0x34)) {
    glUniformMatrix4fv(*(int *)(this + 0x34),1,0,param_2 + 0x104);
  }
  if (-1 < *(int *)(this + 0x38)) {
    glUniformMatrix3fv(*(int *)(this + 0x38),1,0,param_2 + 0x204);
  }
  if (this[9] != (GenericShader2)0x0) {
    if (-1 < *(int *)(this + 0x3c)) {
      glUniform3f(*(int *)(this + 0x3c),*(undefined4 *)(param_2 + 0x330),
                  *(undefined4 *)(param_2 + 0x334),*(undefined4 *)(param_2 + 0x338));
    }
    if (-1 < *(int *)(this + 0x40)) {
      glUniform3f(*(int *)(this + 0x40),*(undefined4 *)(param_2 + 0x34c),
                  *(undefined4 *)(param_2 + 0x350),*(undefined4 *)(param_2 + 0x354));
    }
    if (-1 < *(int *)(this + 0x48)) {
      glUniform4fv(*(int *)(this + 0x48),1,param_2 + 0xd0);
    }
    if (-1 < *(int *)(this + 0x4c)) {
      glUniform4fv(*(int *)(this + 0x4c),1,param_2 + 0x2a8);
    }
    if (-1 < *(int *)(this + 0x50)) {
      glUniform4fv(*(int *)(this + 0x50),1,param_2 + 0x298);
    }
    if (-1 < *(int *)(this + 0x54)) {
      glUniform4fv(*(int *)(this + 0x54),1,param_2 + 0x2b8);
    }
    this[9] = (GenericShader2)0x0;
  }
  if (-1 < *(int *)(this + 0x20)) {
    glEnableVertexAttribArray();
  }
  if (-1 < *(int *)(this + 0x24)) {
    glEnableVertexAttribArray();
  }
  if (-1 < *(int *)(this + 0x28)) {
    glEnableVertexAttribArray();
  }
  if (-1 < *(int *)(this + 0x2c)) {
    glEnableVertexAttribArray();
  }
  if (-1 < *(int *)(this + 0x30)) {
    glEnableVertexAttribArray();
  }
  if (-1 < *(int *)(this + 0x20)) {
    glVertexAttribPointer(*(int *)(this + 0x20),3,0x1406,0,0,*(undefined4 *)(param_1 + 4));
  }
  if (-1 < *(int *)(this + 0x24)) {
    glVertexAttribPointer(*(int *)(this + 0x24),2,0x1406,0,0,*(undefined4 *)(param_1 + 8));
  }
  if (-1 < *(int *)(this + 0x28)) {
    glVertexAttribPointer(*(int *)(this + 0x28),3,0x1406,0,0,*(undefined4 *)(param_1 + 0x10));
  }
  if (-1 < *(int *)(this + 0x2c)) {
    glVertexAttribPointer(*(int *)(this + 0x2c),3,0x1406,0,0,*(undefined4 *)(param_1 + 0x14));
  }
  if (-1 < *(int *)(this + 0x30)) {
    glVertexAttribPointer(*(int *)(this + 0x30),3,0x1406,0,0,*(undefined4 *)(param_1 + 0x18));
  }
  return;
}

```

## target disasm
```
  0009e2ba: push {r4,r5,r6,r7,lr}
  0009e2bc: add r7,sp,#0xc
  0009e2be: push.w r11
  0009e2c2: sub sp,#0x8
  0009e2c4: mov r5,r0
  0009e2c6: ldr r0,[r0,#0x34]
  0009e2c8: mov r6,r2
  0009e2ca: mov r4,r1
  0009e2cc: cmp r0,#0x0
  0009e2ce: blt 0x0009e2dc
  0009e2d0: add.w r3,r6,#0x104
  0009e2d4: movs r1,#0x1
  0009e2d6: movs r2,#0x0
  0009e2d8: blx 0x00070990
  0009e2dc: ldr r0,[r5,#0x38]
  0009e2de: cmp r0,#0x0
  0009e2e0: blt 0x0009e2ee
  0009e2e2: add.w r3,r6,#0x204
  0009e2e6: movs r1,#0x1
  0009e2e8: movs r2,#0x0
  0009e2ea: blx 0x0007099c
  0009e2ee: ldrb r0,[r5,#0x9]
  0009e2f0: cbz r0,0x0009e356
  0009e2f2: ldr r0,[r5,#0x3c]
  0009e2f4: cmp r0,#0x0
  0009e2f6: blt 0x0009e302
  0009e2f8: add.w r3,r6,#0x330
  0009e2fc: ldmia r3,{r1,r2,r3}
  0009e2fe: blx 0x000709b4
  0009e302: ldr r0,[r5,#0x40]
  0009e304: cmp r0,#0x0
  0009e306: blt 0x0009e312
  0009e308: add.w r3,r6,#0x34c
  0009e30c: ldmia r3,{r1,r2,r3}
  0009e30e: blx 0x000709b4
  0009e312: ldr r0,[r5,#0x48]
  0009e314: cmp r0,#0x0
  0009e316: blt 0x0009e322
  0009e318: add.w r2,r6,#0xd0
  0009e31c: movs r1,#0x1
  0009e31e: blx 0x00070984
  0009e322: ldr r0,[r5,#0x4c]
  0009e324: cmp r0,#0x0
  0009e326: blt 0x0009e332
  0009e328: add.w r2,r6,#0x2a8
  0009e32c: movs r1,#0x1
  0009e32e: blx 0x00070984
  0009e332: ldr r0,[r5,#0x50]
  0009e334: cmp r0,#0x0
  0009e336: blt 0x0009e342
  0009e338: add.w r2,r6,#0x298
  0009e33c: movs r1,#0x1
  0009e33e: blx 0x00070984
  0009e342: ldr r0,[r5,#0x54]
  0009e344: cmp r0,#0x0
  0009e346: blt 0x0009e352
  0009e348: add.w r2,r6,#0x2b8
  0009e34c: movs r1,#0x1
  0009e34e: blx 0x00070984
  0009e352: movs r0,#0x0
  0009e354: strb r0,[r5,#0x9]
  0009e356: ldr r0,[r5,#0x20]
  0009e358: cmp r0,#0x0
  0009e35a: blt 0x0009e360
  0009e35c: blx 0x00070a5c
  0009e360: ldr r0,[r5,#0x24]
  0009e362: cmp r0,#0x0
  0009e364: blt 0x0009e36a
  0009e366: blx 0x00070a5c
  0009e36a: ldr r0,[r5,#0x28]
  0009e36c: cmp r0,#0x0
  0009e36e: blt 0x0009e374
  0009e370: blx 0x00070a5c
  0009e374: ldr r0,[r5,#0x2c]
  0009e376: cmp r0,#0x0
  0009e378: blt 0x0009e37e
  0009e37a: blx 0x00070a5c
  0009e37e: ldr r0,[r5,#0x30]
  0009e380: cmp r0,#0x0
  0009e382: blt 0x0009e388
  0009e384: blx 0x00070a5c
  0009e388: ldr r0,[r5,#0x20]
  0009e38a: cmp r0,#0x0
  0009e38c: blt 0x0009e3a2
  0009e38e: ldr r1,[r4,#0x4]
  0009e390: movs r2,#0x0
  0009e392: movs r3,#0x0
  0009e394: strd r2,r1,[sp,#0x0]
  0009e398: movs r1,#0x3
  0009e39a: movw r2,#0x1406
  0009e39e: blx 0x000709c0
  0009e3a2: ldr r0,[r5,#0x24]
  0009e3a4: cmp r0,#0x0
  0009e3a6: blt 0x0009e3bc
  0009e3a8: ldr r1,[r4,#0x8]
  0009e3aa: movs r2,#0x0
  0009e3ac: movs r3,#0x0
  0009e3ae: strd r2,r1,[sp,#0x0]
  0009e3b2: movs r1,#0x2
  0009e3b4: movw r2,#0x1406
  0009e3b8: blx 0x000709c0
  0009e3bc: ldr r0,[r5,#0x28]
  0009e3be: cmp r0,#0x0
  0009e3c0: blt 0x0009e3d6
  0009e3c2: ldr r1,[r4,#0x10]
  0009e3c4: movs r2,#0x0
  0009e3c6: movs r3,#0x0
  0009e3c8: strd r2,r1,[sp,#0x0]
  0009e3cc: movs r1,#0x3
  0009e3ce: movw r2,#0x1406
  0009e3d2: blx 0x000709c0
  0009e3d6: ldr r0,[r5,#0x2c]
  0009e3d8: cmp r0,#0x0
  0009e3da: blt 0x0009e3f0
  0009e3dc: ldr r1,[r4,#0x14]
  0009e3de: movs r2,#0x0
  0009e3e0: movs r3,#0x0
  0009e3e2: strd r2,r1,[sp,#0x0]
  0009e3e6: movs r1,#0x3
  0009e3e8: movw r2,#0x1406
  0009e3ec: blx 0x000709c0
  0009e3f0: ldr r0,[r5,#0x30]
  0009e3f2: cmp r0,#0x0
  0009e3f4: blt 0x0009e40a
  0009e3f6: ldr r1,[r4,#0x18]
  0009e3f8: movs r2,#0x0
  0009e3fa: movs r3,#0x0
  0009e3fc: strd r2,r1,[sp,#0x0]
  0009e400: movs r1,#0x3
  0009e402: movw r2,#0x1406
  0009e406: blx 0x000709c0
  0009e40a: add sp,#0x8
  0009e40c: pop.w r11
  0009e410: pop {r4,r5,r6,r7,pc}
```
