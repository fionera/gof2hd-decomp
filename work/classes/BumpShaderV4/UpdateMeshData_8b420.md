# BumpShaderV4::UpdateMeshData
elf 0x8b420 body 506
Sig: undefined __thiscall UpdateMeshData(BumpShaderV4 * this, Mesh * param_1, Engine * param_2)

## decompile
```c

/* AbyssEngine::BumpShaderV4::UpdateMeshData(AbyssEngine::Mesh*, AbyssEngine::Engine*) */

void __thiscall
AbyssEngine::BumpShaderV4::UpdateMeshData(BumpShaderV4 *this,Mesh *param_1,Engine *param_2)

{
  code *pcVar1;
  code *pcVar2;
  
  if (-1 < *(int *)(this + 0x34)) {
    glUniformMatrix4fv(*(int *)(this + 0x34),1,0,param_2 + 0x104);
  }
  if (-1 < *(int *)(this + 0x38)) {
    glUniformMatrix3fv(*(int *)(this + 0x38),1,0,param_2 + 0x204);
  }
  if (this[9] != (BumpShaderV4)0x0) {
    if (-1 < *(int *)(this + 0x3c)) {
      glUniform3f(*(int *)(this + 0x3c),*(undefined4 *)(param_2 + 0x330),
                  *(undefined4 *)(param_2 + 0x334),*(undefined4 *)(param_2 + 0x338));
    }
    if (-1 < *(int *)(this + 0x40)) {
      glUniform3f(*(int *)(this + 0x40),*(undefined4 *)(param_2 + 0x34c),
                  *(undefined4 *)(param_2 + 0x350),*(undefined4 *)(param_2 + 0x354));
    }
    if (-1 < *(int *)(this + 0x4c)) {
      glUniform4fv(*(int *)(this + 0x4c),1,param_2 + 0xd0);
    }
    if (-1 < *(int *)(this + 0x50)) {
      glUniform4fv(*(int *)(this + 0x50),1,param_2 + 0x2a8);
    }
    if (-1 < *(int *)(this + 0x54)) {
      glUniform4fv(*(int *)(this + 0x54),1,param_2 + 0x298);
    }
    if (-1 < *(int *)(this + 0x58)) {
      glUniform4fv(*(int *)(this + 0x58),1,param_2 + 0x2b8);
    }
    this[9] = (BumpShaderV4)0x0;
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
  if (param_1[0x5c] == (Mesh)0x0) {
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
  }
  else {
    pcVar1 = *(code **)(DAT_0009b61c + 0x9b4fe);
    (*pcVar1)(0x8892,*(undefined4 *)(param_1 + 0x60));
    pcVar2 = *(code **)(DAT_0009b620 + 0x9b514);
    (*pcVar2)(*(undefined4 *)(this + 0x20),3,0x1406,0,0,0);
    (*pcVar1)(0x8892,*(undefined4 *)(param_1 + 0x68));
    (*pcVar2)(*(undefined4 *)(this + 0x24),2,0x1406,0,0,0);
    (*pcVar1)(0x8892,*(undefined4 *)(param_1 + 0x6c));
    (*pcVar2)(*(undefined4 *)(this + 0x28),3,0x1406,0,0,0);
    (*pcVar1)(0x8892,*(undefined4 *)(param_1 + 0x70));
    (*pcVar2)(*(undefined4 *)(this + 0x2c),3,0x1406,0,0,0);
    (*pcVar1)(0x8892,*(undefined4 *)(param_1 + 0x74));
    (*pcVar2)(*(undefined4 *)(this + 0x30),3,0x1406,0,0,0);
  }
  return;
}

```

## target disasm
```
  0009b420: push {r4,r5,r6,r7,lr}
  0009b422: add r7,sp,#0xc
  0009b424: push {r6,r7,r8,r9,r11}
  0009b428: mov r4,r0
  0009b42a: ldr r0,[r0,#0x34]
  0009b42c: mov r6,r2
  0009b42e: mov r8,r1
  0009b430: cmp r0,#0x0
  0009b432: blt 0x0009b440
  0009b434: add.w r3,r6,#0x104
  0009b438: movs r1,#0x1
  0009b43a: movs r2,#0x0
  0009b43c: blx 0x00070990
  0009b440: ldr r0,[r4,#0x38]
  0009b442: cmp r0,#0x0
  0009b444: blt 0x0009b452
  0009b446: add.w r3,r6,#0x204
  0009b44a: movs r1,#0x1
  0009b44c: movs r2,#0x0
  0009b44e: blx 0x0007099c
  0009b452: ldrb r0,[r4,#0x9]
  0009b454: cbz r0,0x0009b4ba
  0009b456: ldr r0,[r4,#0x3c]
  0009b458: cmp r0,#0x0
  0009b45a: blt 0x0009b466
  0009b45c: add.w r3,r6,#0x330
  0009b460: ldmia r3,{r1,r2,r3}
  0009b462: blx 0x000709b4
  0009b466: ldr r0,[r4,#0x40]
  0009b468: cmp r0,#0x0
  0009b46a: blt 0x0009b476
  0009b46c: add.w r3,r6,#0x34c
  0009b470: ldmia r3,{r1,r2,r3}
  0009b472: blx 0x000709b4
  0009b476: ldr r0,[r4,#0x4c]
  0009b478: cmp r0,#0x0
  0009b47a: blt 0x0009b486
  0009b47c: add.w r2,r6,#0xd0
  0009b480: movs r1,#0x1
  0009b482: blx 0x00070984
  0009b486: ldr r0,[r4,#0x50]
  0009b488: cmp r0,#0x0
  0009b48a: blt 0x0009b496
  0009b48c: add.w r2,r6,#0x2a8
  0009b490: movs r1,#0x1
  0009b492: blx 0x00070984
  0009b496: ldr r0,[r4,#0x54]
  0009b498: cmp r0,#0x0
  0009b49a: blt 0x0009b4a6
  0009b49c: add.w r2,r6,#0x298
  0009b4a0: movs r1,#0x1
  0009b4a2: blx 0x00070984
  0009b4a6: ldr r0,[r4,#0x58]
  0009b4a8: cmp r0,#0x0
  0009b4aa: blt 0x0009b4b6
  0009b4ac: add.w r2,r6,#0x2b8
  0009b4b0: movs r1,#0x1
  0009b4b2: blx 0x00070984
  0009b4b6: movs r0,#0x0
  0009b4b8: strb r0,[r4,#0x9]
  0009b4ba: ldr r0,[r4,#0x20]
  0009b4bc: cmp r0,#0x0
  0009b4be: blt 0x0009b4c4
  0009b4c0: blx 0x00070a5c
  0009b4c4: ldr r0,[r4,#0x24]
  0009b4c6: cmp r0,#0x0
  0009b4c8: blt 0x0009b4ce
  0009b4ca: blx 0x00070a5c
  0009b4ce: ldr r0,[r4,#0x28]
  0009b4d0: cmp r0,#0x0
  0009b4d2: blt 0x0009b4d8
  0009b4d4: blx 0x00070a5c
  0009b4d8: ldr r0,[r4,#0x2c]
  0009b4da: cmp r0,#0x0
  0009b4dc: blt 0x0009b4e2
  0009b4de: blx 0x00070a5c
  0009b4e2: ldr r0,[r4,#0x30]
  0009b4e4: cmp r0,#0x0
  0009b4e6: blt 0x0009b4ec
  0009b4e8: blx 0x00070a5c
  0009b4ec: ldrb.w r0,[r8,#0x5c]
  0009b4f0: cmp r0,#0x0
  0009b4f2: beq 0x0009b588
  0009b4f4: ldr r0,[0x0009b61c]
  0009b4f6: ldr.w r1,[r8,#0x60]
  0009b4fa: add r0,pc
  0009b4fc: ldr r5,[r0,#0x0]
  0009b4fe: movw r0,#0x8892
  0009b502: blx r5
  0009b504: ldr r1,[0x0009b620]
  0009b506: mov.w r9,#0x0
  0009b50a: ldr r0,[r4,#0x20]
  0009b50c: movw r2,#0x1406
  0009b510: add r1,pc
  0009b512: movs r3,#0x0
  0009b514: strd r9,r9,[sp,#0x0]
  0009b518: ldr r6,[r1,#0x0]
  0009b51a: movs r1,#0x3
  0009b51c: blx r6
  0009b51e: ldr.w r1,[r8,#0x68]
  0009b522: movw r0,#0x8892
  0009b526: blx r5
  0009b528: ldr r0,[r4,#0x24]
  0009b52a: movs r1,#0x2
  0009b52c: movw r2,#0x1406
  0009b530: movs r3,#0x0
  0009b532: strd r9,r9,[sp,#0x0]
  0009b536: blx r6
  0009b538: ldr.w r1,[r8,#0x6c]
  0009b53c: movw r0,#0x8892
  0009b540: blx r5
  0009b542: ldr r0,[r4,#0x28]
  0009b544: movs r1,#0x3
  0009b546: movw r2,#0x1406
  0009b54a: movs r3,#0x0
  0009b54c: strd r9,r9,[sp,#0x0]
  0009b550: blx r6
  0009b552: ldr.w r1,[r8,#0x70]
  0009b556: movw r0,#0x8892
  0009b55a: blx r5
  0009b55c: ldr r0,[r4,#0x2c]
  0009b55e: movs r1,#0x3
  0009b560: movw r2,#0x1406
  0009b564: movs r3,#0x0
  0009b566: strd r9,r9,[sp,#0x0]
  0009b56a: blx r6
  0009b56c: ldr.w r1,[r8,#0x74]
  0009b570: movw r0,#0x8892
  0009b574: blx r5
  0009b576: ldr r0,[r4,#0x30]
  0009b578: movs r1,#0x3
  0009b57a: movw r2,#0x1406
  0009b57e: movs r3,#0x0
  0009b580: strd r9,r9,[sp,#0x0]
  0009b584: blx r6
  0009b586: b 0x0009b614
  0009b588: ldr r0,[r4,#0x20]
  0009b58a: cmp r0,#0x0
  0009b58c: blt 0x0009b5a4
  0009b58e: ldr.w r1,[r8,#0x4]
  0009b592: movs r2,#0x0
  0009b594: movs r3,#0x0
  0009b596: strd r2,r1,[sp,#0x0]
  0009b59a: movs r1,#0x3
  0009b59c: movw r2,#0x1406
  0009b5a0: blx 0x000709c0
  0009b5a4: ldr r0,[r4,#0x24]
  0009b5a6: cmp r0,#0x0
  0009b5a8: blt 0x0009b5c0
  0009b5aa: ldr.w r1,[r8,#0x8]
  0009b5ae: movs r2,#0x0
  0009b5b0: movs r3,#0x0
  0009b5b2: strd r2,r1,[sp,#0x0]
  0009b5b6: movs r1,#0x2
  0009b5b8: movw r2,#0x1406
  0009b5bc: blx 0x000709c0
  0009b5c0: ldr r0,[r4,#0x28]
  0009b5c2: cmp r0,#0x0
  0009b5c4: blt 0x0009b5dc
  0009b5c6: ldr.w r1,[r8,#0x10]
  0009b5ca: movs r2,#0x0
  0009b5cc: movs r3,#0x0
  0009b5ce: strd r2,r1,[sp,#0x0]
  0009b5d2: movs r1,#0x3
  0009b5d4: movw r2,#0x1406
  0009b5d8: blx 0x000709c0
  0009b5dc: ldr r0,[r4,#0x2c]
  0009b5de: cmp r0,#0x0
  0009b5e0: blt 0x0009b5f8
  0009b5e2: ldr.w r1,[r8,#0x14]
  0009b5e6: movs r2,#0x0
  0009b5e8: movs r3,#0x0
  0009b5ea: strd r2,r1,[sp,#0x0]
  0009b5ee: movs r1,#0x3
  0009b5f0: movw r2,#0x1406
  0009b5f4: blx 0x000709c0
  0009b5f8: ldr r0,[r4,#0x30]
  0009b5fa: cmp r0,#0x0
  0009b5fc: blt 0x0009b614
  0009b5fe: ldr.w r1,[r8,#0x18]
  0009b602: movs r2,#0x0
  0009b604: movs r3,#0x0
  0009b606: strd r2,r1,[sp,#0x0]
  0009b60a: movs r1,#0x3
  0009b60c: movw r2,#0x1406
  0009b610: blx 0x000709c0
  0009b614: pop.w {r2,r3,r8,r9,r11}
  0009b618: pop {r4,r5,r6,r7,pc}
```
