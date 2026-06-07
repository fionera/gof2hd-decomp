# ColorMixAdd::UpdateMeshData
elf 0x922a2 body 230
Sig: undefined __thiscall UpdateMeshData(ColorMixAdd * this, Mesh * param_1, Engine * param_2)

## decompile
```c

/* AbyssEngine::ColorMixAdd::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) */

void __thiscall
AbyssEngine::ColorMixAdd::UpdateMeshData(ColorMixAdd *this,Mesh *param_1,Engine *param_2)

{
  undefined4 uVar1;
  undefined4 uVar2;
  
  glUniformMatrix4fv(*(undefined4 *)(this + 0x28),1,0,param_2 + 0x104);
  if (-1 < *(int *)(this + 0x34)) {
    glUniformMatrix4fv(*(int *)(this + 0x34),1,0,param_2 + 0x1c4);
  }
  if (-1 < *(int *)(this + 0x3c)) {
    glUniform1i(*(int *)(this + 0x3c),param_1[0x85]);
  }
  if (this[9] != (ColorMixAdd)0x0) {
    glUniform4fv(*(undefined4 *)(this + 0x2c),1,param_2 + 0xd0);
    if (-1 < *(int *)(this + 0x38)) {
      glUniform1f(*(int *)(this + 0x38),1.0 - *(float *)(param_1 + 0x1c));
    }
    this[9] = (ColorMixAdd)0x0;
  }
  if ((int)((uint)(byte)*param_1 << 0x1e) < 0) {
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
  }
  return;
}

```

## target disasm
```
  000a22a2: push {r4,r5,r6,r7,lr}
  000a22a4: add r7,sp,#0xc
  000a22a6: push.w r11
  000a22aa: sub sp,#0x8
  000a22ac: mov r4,r0
  000a22ae: ldr r0,[r0,#0x28]
  000a22b0: add.w r3,r2,#0x104
  000a22b4: mov r6,r2
  000a22b6: mov r5,r1
  000a22b8: movs r1,#0x1
  000a22ba: movs r2,#0x0
  000a22bc: blx 0x00070990
  000a22c0: ldr r0,[r4,#0x34]
  000a22c2: cmp r0,#0x0
  000a22c4: blt 0x000a22d2
  000a22c6: add.w r3,r6,#0x1c4
  000a22ca: movs r1,#0x1
  000a22cc: movs r2,#0x0
  000a22ce: blx 0x00070990
  000a22d2: ldr r0,[r4,#0x3c]
  000a22d4: cmp r0,#0x0
  000a22d6: blt 0x000a22e0
  000a22d8: ldrb.w r1,[r5,#0x85]
  000a22dc: blx 0x0007096c
  000a22e0: ldrb r0,[r4,#0x9]
  000a22e2: cbz r0,0x000a230e
  000a22e4: ldr r0,[r4,#0x2c]
  000a22e6: add.w r2,r6,#0xd0
  000a22ea: movs r1,#0x1
  000a22ec: blx 0x00070984
  000a22f0: ldr r0,[r4,#0x38]
  000a22f2: cmp r0,#0x0
  000a22f4: blt 0x000a230a
  000a22f6: vmov.f32 s0,0x3f800000
  000a22fa: vldr.32 s2,[r5,#0x1c]
  000a22fe: vsub.f32 s0,s0,s2
  000a2302: vmov r1,s0
  000a2306: blx 0x00070978
  000a230a: movs r0,#0x0
  000a230c: strb r0,[r4,#0x9]
  000a230e: ldrb r0,[r5,#0x0]
  000a2310: lsls r0,r0,#0x1e
  000a2312: bpl 0x000a2380
  000a2314: ldr r0,[r4,#0x20]
  000a2316: blx 0x00070a5c
  000a231a: ldr r0,[r4,#0x24]
  000a231c: blx 0x00070a5c
  000a2320: ldrb.w r0,[r5,#0x5c]
  000a2324: cbz r0,0x000a2356
  000a2326: ldr r1,[r5,#0x60]
  000a2328: movw r0,#0x8892
  000a232c: blx 0x0006ed04
  000a2330: ldr r0,[r4,#0x20]
  000a2332: movs r6,#0x0
  000a2334: movs r1,#0x3
  000a2336: movw r2,#0x1406
  000a233a: movs r3,#0x0
  000a233c: strd r6,r6,[sp,#0x0]
  000a2340: blx 0x000709c0
  000a2344: ldr r1,[r5,#0x68]
  000a2346: movw r0,#0x8892
  000a234a: blx 0x0006ed04
  000a234e: ldr r0,[r4,#0x24]
  000a2350: strd r6,r6,[sp,#0x0]
  000a2354: b 0x000a2374
  000a2356: ldr r1,[r5,#0x4]
  000a2358: movs r6,#0x0
  000a235a: ldr r0,[r4,#0x20]
  000a235c: movw r2,#0x1406
  000a2360: movs r3,#0x0
  000a2362: strd r6,r1,[sp,#0x0]
  000a2366: movs r1,#0x3
  000a2368: blx 0x000709c0
  000a236c: ldr r0,[r4,#0x24]
  000a236e: ldr r1,[r5,#0x8]
  000a2370: strd r6,r1,[sp,#0x0]
  000a2374: movs r1,#0x2
  000a2376: movw r2,#0x1406
  000a237a: movs r3,#0x0
  000a237c: blx 0x000709c0
  000a2380: add sp,#0x8
  000a2382: pop.w r11
  000a2386: pop {r4,r5,r6,r7,pc}
```
